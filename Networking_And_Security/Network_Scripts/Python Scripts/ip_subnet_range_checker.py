import ipaddress 
# This import allows for easy manipulation of network addresses, subnets, etc ...

def range_checker(ip: str, subnet: str ) -> bool:
    '''
    This will take two parameters, an ip address and a subnet mask.
    It will return True if the ip address is within the subnet range, and False otherwise.
    We Will assume network addresses start at 0
    in form range_checker('192.168.10.0', '255.255.255.0'))
    '''
    network = ipaddress.ip_network(f'{ip}/{subnet}', strict=False) # Create a network object from the subnet
    print(network)
    return ipaddress.ip_address(ip) in network # Check if the ip address is in the network

print(range_checker('192.168.10.0', '255.255.255.0')) # True
    
