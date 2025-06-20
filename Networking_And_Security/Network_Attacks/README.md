# Network Attack Toolskit

These attacks are strickly meant for educational purposes and are meant to demostrate the vulnerability in local networks.

## Ipv4 Attacks

- Arp poison
- DHCP exhaust

## Ipv6 Attacks

- SLAAC spoof
- NS spoof

### Arp poison

- Example usage: ─$ sudo python3 arp.py -t target_ip -g 192.168.1.1 -i eth0
- Functionality:
  - Sends arp request to the target to mimic the router essientially causing a denial of service
    - Router would normally advertise: MAC for router_ip is router_mac
    - Here the attacker is saying: MAC for router_ip is attacker_mac
  - Enables ipv4 forwarding
  - Captures the packets and the target has no idea
- Concern
  - In local netowrks this attack can be very effective due to the lack of security controls and the lack of arp authentication
- Solutions
  - Static arp entries
    - This would ensure that any malicous arp request would be ignored thus defend the attack
    - But can cost overhead of having to statically allocate these entries and reduces dynamic networking.
  - IDS/arpwatch/arpalert
    - These are tools used to detect arp poisons specfically
  - Implementing PKI
    - Can be used as a form of auth within the network to ignore any unverified requests
  - VLAN/isolation
    - while this doesnt directly address Arp attacks this defintelty mitigates the damage by restricting communication.

### Dhcp Exhuastion

- Example usage: python3 DHCP_EXHUASTION
- Functionality:
  - Sends dhcp requests with a spoofed mac address
- Concern
  - At my roomates apartmnet on the cheap router we got from xfinity had no defense against this
  - At home a decent rourter seems to pick up on this type of attack or the dhcp pool at home is only offering 1 ip.
- Solutions
  - Dhcp reservations, similar to static arp tables, the router would serve specfic mac addresses specfic ip's
  - Implement basic Network Access Controls, by having a user authenticate themselves first it will be easier to detect the attacker
  - Behavioral analysis can be used to detect anomalous traffic
