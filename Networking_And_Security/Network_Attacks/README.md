# Network Attack Toolkit

These attacks are strictly meant for educational purposes and are meant to demonstrate the vulnerability of local networks.

## IPv4 Attacks

- ARP poison

- DHCP exhaust

## IPv6 Attacks

- SLAAC spoof

## DoS Attacks

- UDP/TCP Flood

## Recon Scanner

- TCP SYN

### ARP poison

- Example usage: sudo python3 arp.py -t target_ip -g 192.168.1.1 -i eth0

- Functionality:

  - Sends an ARP request to the target to mimic the router, essentially causing a denial of service.

    - Assume router is at 192.168.1.1
    - If attacker_ip = 192.168.1.5
    - target = 192.168.1.25 then
    - The request the target sees:
    - src=192.168.1.1 src_mac=attackers_mac
    - dst=192.168.1.25 dst_mac=target_mac
    - And reads: 192.168.1.1 is at attackers_mac

  - Router would normally advertise: MAC for router_ip is router_mac.

  - Here the attacker is saying, MAC for router_ip is attacker_mac.

  - Enables ipv4 forwarding to elegantly receive requests not meant for the attackers machine, and to forwarding the packet to the correct destination. You can think of it as the MiTM machine acting as a router.

  - Captures the packets, and the target has no idea.

- Concern:

  - In local networks this attack can be very effective due to the lack of security controls and the lack of authentication in general.

- Solutions

  - Static ARP entries

  - This would ensure that any malicious ARP request would be ignored, thus defending against the attack.

  - But can cost overhead of having to statically allocate these entries and reduces dynamic networking.

  - IDS/arpwatch/arpalert

  - These are tools used to detect ARP poisons specifically.

  - VLAN/isolation

  - While this doesn't directly address ARP attacks, this definitely mitigates the damage by restricting communication.

### DHCP Exhaustion

- Example usage: python3 DHCP_EXHAUSTION.py

- Functionality:

  - Sends DHCP requests with a spoofed MAC address

- Concern:

  - At my roommate's apartment, the cheap router we got from Xfinity had no defense against this.

  - At home a decent router seems to pick up on this type of attack, or the DHCP pool at home is only offering 1 IP.

- Solutions

  - DHCP reservations, similar to static ARP tables, the router would serve specific MAC addresses to specific IPs.

  - Implement basic network access controls; by having a user authenticate themselves first, it will be easier to detect the attacker.

  - Behavioral analysis can be used to detect anomalous traffic.

## SLAAC spoof

- Example usage: python3 SLAAC_SPOOF.py

- Functionality:

  - The reason for the lack of user input is due to the automated nature of the attack.

  - The attack starts with identifying all the gateways on the network via ping.

  - Then sends out a Neighbor Solicitation (NS) message to get details on the router like its MAC and IP.

  - There are 3 active threads on this attack:

  - Send a router advertisement to spoof the gateway with a cache override option and lifetime of 0, effectively booting that router offline for other devices on that network.

  - Sniffer thread which is used to capture incoming packets

  - Sending a router advertisement routed to the attacker's machine

- Concern:

  - For my local network, having 0 auth for things like router advertisements can cause many issues, like trusting an attacker's machine to act as the router and telling other machines to override the current TTL of a router and set it to 0.

- Solution

  - Use of RA guard on switches, which filters out malicious RAs from unauthorized sources

  - IPv6 source guard, which validates IPv6 addresses with their binding tables

  - Could configure sources to only accept RAs from trusted sources

## Recon L3 Scanner

- Simple TCP Syn Scanner

  - Example: python3 network_scanner.py -t 192.168.1.1 -s syn_scan -F

  - Functionality:

  - Port scanning works by measuring the type of response.

  - In this case we use TCP SYN to scan a network.

  - How this works:

    - Send the target a TCP SYN on some port.

    - Open Port: If the target responds with an ACK flag.

    - Closed Port: if the target responds with an RST flag or ACK + RST flag

    - Filtered Port: If we get no response from the target

  - Effectiveness

  - SYN scans are a go-to port scanning technique due to how stealthy and effective they are.

  - The reason it's stealthy is because when an open port is received, an RST flag is sent right after tearing down the connection. So that the connection will not get logged due to the lack of "connection." Connection is not established.

- Concern:

  - Port scanning is usually an attacker bread and butter, it is used to identify services, version number, and even potential vulnerabilties.

  - It's also important to note that the cost of logging all network traffic on a machine can cause extensive resource overhead.

- Solution:
  - Although difficult to detect IDS/IPS systems can still detect this type of probing.
  - Firewalls are the most common defense, depending on the type of service you are running can greatly improve or reduce detection abilities.
