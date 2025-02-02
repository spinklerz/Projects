import argparse
import ipaddress
import scapy.all as scapy
import concurrent.futures

R = "\033[91m"  # Red text
W = "\033[0m"   # Reset to default
G = "\033[92m"  # Green text

# validating target ip address

def is_valid_ip(address):
    try:
        ipaddress.ip_address(address)
        return True
    except:
        return False


# validating ports

def is_valid_port(port):
    valid_ports = []
    for element in port:
        try: 
            element = int(element)
            if element >= 0 and element <= 65535:
                valid_ports.append(element)
            else:
                print(R,"\t\t\tInvalid port: ", element,W)
                exit(1)
        except:
            print(" \t\t\tPort should be between 0 and 65535\n")
            exit(1)
    return valid_ports

# SYN Scan
def SYN_SCAN(network_interface, target_ip_address, scan_ports, verbose=False):
    syn_pkt = scapy.IP(dst=target_ip_address)/scapy.TCP(dport=int(scan_ports), flags="S") # SYN packet

    response = scapy.sr1(syn_pkt, timeout=1, verbose=0) # send packet and wait for response
    if response and response.haslayer(scapy.TCP):

        if response[scapy.TCP].flags == 'SA': # port is open
            # What is SA flag? SYN(S) + ACK(A)
                ## SA flag is a combination of SYN and ACK flags
                ## SYN(S) is a synchronization flag, which is used 
                ## to synchronize sequence numbers to initiate a connection.
                ## Which means that the server is ready to initiate a connection.
            return 1
        elif response[scapy.TCP].flags == 'RA': # port is closed via RST flag and ACK flag
            # What is RA flag? RST(R) + ACK(A)
                ## RA flag is a combination of RST and ACK flags. 
                ## RST(R) is a reset flag, which is used to reset the connection and is sent by the server.
                ## ACK(A) is an acknowledgment flag, which is used to acknowledge the receipt of the packet.
                ## This means when the server responds with RST + ACK it acknowledges the receipt of the packet 
                ## and requests resets the connection as that port is not open.
            return -2
        elif response[scapy.TCP].flags == 'R': # port is closed via RST flag
            # What is R flag?
                ## R flag is a reset flag, which is used to reset the connection 
                ## and is sent by the server.
                return -1
        else:
            return 0
    else:
        return 0

def log_scan_results(port, result, verbose):
    if result == 1:
        print(f"{G}Port {port} is open{W}\n")
    elif verbose:
        if result == 0:
            print(f"{R}Port {port} is closed (no response){W}\n")
        elif result == -1:
            print(f"{R}Port {port} is closed (RST flag){W}\n")
        elif result == -2:
            print(f"{R}Port {port} is closed (RST + ACK flag){W}\n")

# Parallel scan
def parrallel_scan(target_ip_address, scan_ports_range, network_interface, verbose):
    # Wrapper function for scanning and logging results
    with concurrent.futures.ThreadPoolExecutor(max_workers=100) as executor:
        results = executor.map(
            lambda port: SYN_SCAN(network_interface, target_ip_address, port, verbose), 
            scan_ports_range
        )
    for port, result in zip(scan_ports_range, results):
        log_scan_results(port, result, verbose) 
    return 0 # Success

if __name__ == "__main__":
    # Get arguements from command line
    parse = argparse.ArgumentParser(description="Port Scanner")

    parse.add_argument("-i", "--network_interface",	help="network interface to be used") 				                            # network interface
    parse.add_argument("-t", "--target_ip_address",	help="target ip address to scan ") 				                                # target ip address 
    parse.add_argument("-p", "--scan_ports", 		help="ports to be scanned example -p 21,22")			                        # ports to be scanned
    parse.add_argument("-s", "--scan_type",         help="scan type [syn_scan, ack_scan, fin_scan, xmas_scan, tcp_connect_scan]")	# scan type
    parse.add_argument("-v", "--verbose",           help="verbose mode", action="store_true")			                            # verbose mode

    args 			    = parse.parse_args()
    network_interface 	= args.network_interface
    target_ip_address 	= args.target_ip_address
    scan_ports 		    = args.scan_ports
    scan_type 		    = args.scan_type
    verbose 		    = args.verbose          

    print("############################## Arguments ##############################\n")
    print("Network Interface: ", 	network_interface)
    print("Target IP: ", 		    target_ip_address)
    print("Ports to scan: ", 	    scan_ports)
    print("Scan types: ", 		    scan_type)
    print()

    # validating scan type
    if scan_type not in ["syn_scan", "ack_scan", "fin_scan", "xmas_scan", "tcp_connect_scan"]:
        print("Invalid scan type")
        exit(1)
    if not (is_valid_ip(target_ip_address)):
        print(R, "Invalid IP address", W)
        exit(1)

    if not scan_ports:
        scan_ports_range = list(range(0, 65536))
    else:
        scan_ports_range = is_valid_port(scan_ports.split(","))

    print(scan_ports_range)
    if scan_type == "syn_scan":
        print("############################## Starting SYN SCAN ##############################\n")
        print(parrallel_scan(target_ip_address, scan_ports_range, network_interface, verbose))
        print("\n############################## Ending SYN SCAN ##############################\n")