# Mininet and POX SDN Topology

## Overview
This project sets up a Software-Defined Networking (SDN) topology using Mininet and POX. The topology consists of multiple switches and hosts, with custom flow rules enforced via a POX controller.

## Components
- **Mininet Topology Script (`final_topo.py`)**
  - Defines a network with multiple switches and hosts.
  - Connects to a remote POX controller.
  - Enforces VLAN-like segmentation between different departments.
- **POX Controller Script (`final_pox.py`)**
  - Implements OpenFlow rules to control traffic flow.
  - Handles ARP, TCP, and ICMP traffic with specific restrictions.
  - Enforces security policies between trusted/untrusted hosts and departments.

## Topology Details
The network consists of:
- **Switches:** `sw1` to `sw6`
- **Hosts:**
  - **Department A:** `h101`, `h102`, `h103`, `h104`
  - **Department B:** `h201`, `h202`, `h203`, `h204`
  - **Trusted Host:** `h_trust`
  - **Untrusted Host:** `h_untrust`
  - **LLM Server:** `h_server`

## Flow Rules Implemented
- **ARP requests** are flooded to allow host discovery.
- **TCP packets** are forwarded using OpenFlow flood rules.
- **ICMP restrictions:**
  - **Trusted host (`h_trust`)** cannot send ICMP to Department B or `h_server`.
  - **Untrusted host (`h_untrust`)** cannot send ICMP to any department or `h_server`.
  - **Department A and Department B** cannot communicate via ICMP.

## Installation & Setup
### Prerequisites
- **Mininet**: Install via:
  ```sh
  sudo apt-get install mininet
  ```
- **POX Controller**: Clone from GitHub:
  ```sh
  git clone https://github.com/noxrepo/pox.git
  ```

### Running the Topology
1. **Start POX Controller:**
   ```sh
   cd pox
   ./pox.py log.level --DEBUG final_pox
   ```
2. **Run Mininet Topology:**
   ```sh
   sudo python3 final_topo.py
   ```
3. **Use Mininet CLI to Test Connectivity:**
   ```sh
   mininet> pingall
   ```

## Testing
- Use `ping` to check connectivity based on enforced rules.
- Use `iperf` to test TCP traffic flow.

## License
This project is open-source under the MIT License.

