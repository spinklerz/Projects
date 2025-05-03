import subprocess
import scapy.all as scapy
import time
import random
import os
import socket
import json
import netifaces as ni
from netifaces import AF_INET, AF_INET6, AF_LINK
from scapy.layers.inet6 import IPv6, ICMPv6ND_RS, ICMPv6ND_RA, ICMPv6NDOptSrcLLAddr
from scapy.all import *
from scapy.all import sr1, IP, ICMP

# This is not effienct usually, you'd just listen for RA's or use built in tools to check ipv6 routes 

def sniff_for_ND_RA(interface, timeout=120):
    """
    Sniff for Router Advertisements (RA) on the specified interface.
    
    Args:
        interface: Network interface to sniff on
        timeout: Maximum time to wait for RAs in seconds
    
    Returns:
        The first captured RA packet or None if timeout occurs
    """
    print(f"\n[*] Sniffing for Router Advertisements on {interface}...")
    
    # Define a filter function to identify RA packets
    def is_router_advertisement(pkt):
        return (IPv6 in pkt and ICMPv6ND_RA in pkt)
    
    # Define callback function to process RA packets
    def process_ra(pkt):
        print("\n[+] Router Advertisement received!")
        print(f"[+] Source: {pkt[IPv6].src}")
        
        # Extract and display router information
        if ICMPv6ND_RA in pkt:
            ra = pkt[ICMPv6ND_RA]
            print(f"[+] Router Lifetime: {ra.routerlifetime} seconds")
            print(f"[+] Reachable Time: {ra.reachabletime} milliseconds")
            print(f"[+] Retrans Timer: {ra.retranstimer} milliseconds")
            
            # Check if M and O flags are set
            m_flag = bool(ra.M)
            o_flag = bool(ra.O)
            print(f"[+] Managed Address Configuration (M flag): {m_flag}")
            print(f"[+] Other Configuration (O flag): {o_flag}")
            
            # Process options if present
            for opt in pkt.getlayer(ICMPv6NDOptSrcLLAddr, 1):
                print(f"[+] Router MAC: {opt.lladdr}")
                
            # Display full packet details
            print("\n[*] Detailed packet information:")
            pkt.show()
            
            return True  # Stop sniffing after receiving an RA
        
        return False
    
    # Start sniffing
    try:
        print(f"[*] Waiting up to {timeout} seconds for Router Advertisements...")
        result = sniff(iface=interface, 
                      lfilter=is_router_advertisement, 
                      prn=process_ra, 
                      stop_filter=lambda p: process_ra(p),
                      timeout=timeout)
        
        if not result:
            print(f"[-] No Router Advertisements received within {timeout} seconds.")
            return None
            
        return result[0]  # Return the first RA packet
        
    except Exception as e:
        print(f"[-] Error while sniffing: {e}")
        return None

# RS's low key useless for my purposes but spent the time to create it so if others need reference free feel 

def sendNDP_RS(interface):
    # Send Router Solicitation
    # Find the MAC address of the interface specfied
    addrs = ni.ifaddresses(interface)
    mac_info = addrs.get(ni.AF_LINK)

    # Craft RS packet
    # Ethernet for IPv6 multicast
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
    interface = str(input("Insert Interface Name to Send Out Router Solicitation: "))
    sendNDP_RS(interface)
main()
