from scapy.all import * 
import threading


domain = "cse153.ramakrishnansr.com"
dns_server = "1.1.1.1"
ip = ''

def send_dns_query_A():
	dns = DNS(rd=1,qd=DNSQR(qname=domain, qtype='A'))
	udp = UDP()
	ip = IP(dst=dns_server)
	packet = ip / udp / dns 
	response = sr1(packet)
	ip = response.an[0].rdata
	print(ip)
	print(response.an.rdata)

def send_dns_query_TXT():
	dns = DNS(rd=1, qd=DNSQR(qname=domain, qtype='TXT'))
	udp = UDP()
	ip = IP(dst=dns_server)
	packet = ip / udp / dns 
	response = sr1(packet)
	print(response)

send_dns_query_A()
send_dns_query_TXT() 
