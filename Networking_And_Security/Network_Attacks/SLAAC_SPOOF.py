import subprocess
import scapy.all as scapy
import time
import random
import os
import socket
import binascii
import netifaces as ni
from netifaces import AF_INET, AF_INET6, AF_LINK
from scapy.layers.inet6 import IPv6, ICMPv6ND_RS, ICMPv6ND_RA, ICMPv6NDOptSrcLLAddr
from scapy.all import *
from scapy.all import sr1, IP, ICMP

# Explanation:
# This tools does a couple of cool things
# Assume RA is not enabled on the target network this should work
# First the script will query to find all routers on the network via NDP_RS(Router Solicitation)
# Once routers are spotted we will kick them off by sending a spoofed NDP_RA(Router Advertisement)
# With a TTL of 0 essintially making them unreachable and booting that router
# Now we can simulate a evil twin like attack and send Router advertisements of our own!
# This script will put us in the middle!


def sniff_for_ND_RA(interface):  # Sniff for Router Advertisement
    # tshark -i en0 -Y  -T json
    # Why is timeout so long? Well because my router sucks and takes a while to respond, feel free to change it
    process = subprocess.run(
        ['tshark', '-i', interface, '-Y', "icmpv6.type == 134", '-T', 'json'], capture_output=True, text=True, timeout=60)


def sendNDP_RS(interface):  # Send Router Solicitation
    # Find the MAC address of the interface specfied
    addrs = ni.ifaddresses(interface)
    mac_info = addrs.get(ni.AF_LINK)

    # Craft RS packet
    base = IPv6(dst="ff02::2")
    router_solicitation = ICMPv6ND_RS()
    src_ll_address = ICMPv6NDOptSrcLLAddr(lladdr=mac_info[0]['addr'])
    ether = Ether(src=mac_info[0]['addr'], dst='33:33:00:00:00:02')
    packet = ether / base / router_solicitation / src_ll_address
    packet.show()
    result = sendp(packet, iface=interface, verbose=True)

    # Sniff result
    # Feel free to adjust timeout paramter
    sniff_for_ND_RA(interface)
    pass


def main():
    # interfaces = netifaces.interfaces()
    # print("Available network interfaces:")
    # print(interfaces)
    # User needs to select interface and thats it
    interface = "en0"  # replace with your interface
    print(ni.ifaddresses(interface)[
          AF_INET6][0]['addr'] + " is the IPv6 address of the interface" + interface)
    pass
    sendNDP_RS(interface)


main()
