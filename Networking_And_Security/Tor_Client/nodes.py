import socket
import ssl
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

NODES = [
    {
        "name": "Node 1",
        "address": ("127.0.0.1", 5000),
        "public_key": """-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1AO2AqfV6Ox8cGj3BWyj63UlBTO2V22Nw1zvA6vD5Hv7v0IzEoGlNlJzAhC5Xc5I4u8fNlUsA3m4mUNtBDY9P5PAP9TmlypHY9TXiQIjVs9V4A==
-----END PUBLIC KEY-----"""
    },
    {
        "name": "Node 2",
        "address": ("127.0.0.1", 6000),
        "public_key": """-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1AO2AqfV6Ox8cGj3BWyj63UlBTO2V22Nw1zvA6vD5Hv7v0IzEoGlNlJzAhC5Xc5I4u8fNlUsA3m4mUNtBDY9P5PAP9TmlypHY9TXiQIjVs9V4A==
-----END PUBLIC KEY-----"""
    },
    {
        "name": "Node 3",
        "address": ("127.0.0.1", 7000),
        "public_key": """-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA1AO2AqfV6Ox8cGj3BWyj63UlBTO2V22Nw1zvA6vD5Hv7v0IzEoGlNlJzAhC5Xc5I4u8fNlUsA3m4mUNtBDY9P5PAP9TmlypHY9TXiQIjVs9V4A==
-----END PUBLIC KEY-----"""
    }
]

def handle_client_connection(client_socket, private_key):

    encrypted_message = client_socket.recv(4096)
    
    cipher = PKCS1_OAEP.new(private_key)
    decrypted_message = cipher.decrypt(encrypted_message)
    
    print(f"Received encrypted message: {encrypted_message}")
    print(f"Decrypted message: {decrypted_message.decode()}")

    client_socket.send(b"Message received and forwarded")
    client_socket.close()

def start_node_listener(node, private_key):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(node["address"])
    server_socket.listen(5)
    print(f"{node['name']} listening on {node['address']}...")

    while True:
        client_socket, _ = server_socket.accept()
        print(f"Connection from {client_socket.getpeername()} received.")
        handle_client_connection(client_socket, private_key)

def main():
    private_key = RSA.import_key("""-----BEGIN PRIVATE KEY-----
    <Private Key Here>
    -----END PRIVATE KEY-----""")

    for node in NODES:
        start_node_listener(node, private_key)

if __name__ == "__main__":
    main()
