import socket
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP

# Circuit
NODES = [
    {
        "name": "Entry_Node",
        "address": ("10.0.0.221", 65432),
        "public_key": """-----BEGIN PUBLIC KEY-----
MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAwhjtWdncaiZDx63bSmq9
gv1UC29nTY08k/qF47JDX0GX//tIflZtU9Jx6oSS1waPXhm9oU8l8pAzGQK5OuUw
yQTLcDkLuCIzkhHNDLWE9VFRd4N2rbJwyGsrJj1uV0Bf6IJSfY4PyKk2cFcyKb7c
qJO/nbU37VCuLZT4lMwGuDKLqSIcfRx7x8Ple64AfLBRNOwTmrEK3YVtqWvwZZ7n
XxHfkpPG57ZPx72vBveNnSEJ2GWbWZItCEL2jdBAGVHFG17eELFJNPOVfY+dw847
CI4XBvHzS/L/ePKNAZw0aqFbKWft7sNU+CKYcflXYlwrpU4JT3Fz2oispV3L1Ac9
igIBJY/K5VlpX3+CUQG/ApTcFZvoUIn5L+RIOF8oiAsKXlj2NYVOccEM+dnygK2r
i0ZPfyQuisp4x4Eqzie6X7IoYRIv/K9mv/lztVwB6fOB9a6gGLvanTmYVTikDOHh
yzCm8TEmLdpPE9hNGL4FMPr4JLNJT0ZPYy9LCysXAhSQWtMy5KKlVOVBXacv9twv
SRNW9/8M6GdcbNq42V1nD4TQjZMwa4B98+LoGMSExkM9w3s4Dq4grXzkRxwuD3gw
+89KJYMry4pIAmPu1Toa2G0SXffbuhuzWyNS+g8OJyGpdkLVu/0DLdZ+hVVHIo0V
4Wa30bBwGIn5kWvv5S6jIvcCAwEAAQ==
-----END PUBLIC KEY-----"""
    }, 
    {
        "name": "Exit_Node",
        "address": ("10.0.0.221", 65433),
        "public_key": """-----BEGIN PUBLIC KEY-----
MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEA2Q+SO17VNp9j2IxYRWfd
HKdF2K9R23uTQjspjWp+cN5YmqAoeTuEsemAbr65q1kxuAArBs7HMaSVu0PWJ73y
nerkYIlRJ84CDWxP+4FbHeIusjgB+mIoRogr17H9LeVrI3ntasrthqhdUQDwZSA7
AthHrfjJb22T2CewRdMZ2vk3CCjwDu4Xx1RilqWU1Bb8+P0l0WrwUpR5lbYCJm/H
CqQ5N00adVbXu7SIr+A/hwt6n3CdCqPhgGH5t8DqMi07KwHngWdEvssk1dbhNB4Y
tpTytMxBmYaNtvvcXXyo8ubTxqqjs5cmpzFfTy44B2C/HXJuBmTVI7MPaapJrMjQ
u1SaL6G6eFehug1IImWT8lW5VqxP+015b8bS4P9/kplEKfWhjvtrXb1rbvVXCai6
c3/LJ6DV0LTo1vhw0+ME6aJBC8Ldx0Lxa6D5RxJ1h/58CVc17fsZyIdsvpCDREy8
fCqoc+7DP5TtHug3niX0stp1/1Leb0kbVfUBUsfvO+TTlOWDK7bLb863NR4HIxEr
xkT+LJx/BuX0UuR/rUiu4y2fhaX6gQr3/pfBNVNZLwTv9Hni7KX8iDDoBvNkqWJC
IpZBWjnCB51j2YWJwAtqmyGJpEIG3+0TPGsO8xKdAjWubF3Ho9OTl4pZhD8phD5i
Hi5WX6XFySPvEtylDUxYRbsCAwEAAQ==
-----END PUBLIC KEY-----""",
    }
]

def decrypt(data, key):
    cipher = PKCS1_OAEP.new(RSA.import_key(key))
    return cipher.decrypt(data)

def encrypt(data, key):
    cipher = PKCS1_OAEP.new(RSA.import_key(key))
    # Plain text error if i dont cap
    return cipher.encrypt(data[:190])

def send_message(encrypted_msg):
    addr = NODES[0]["address"]
    print(addr)
    with socket.create_connection(addr) as sock:
        sock.sendall(encrypted_msg)
        return sock.recv(4096)


def main():
    # Encryption part (already in your code)
    msg = b"GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n"
    encrypted_msg = encrypt(msg, NODES[1]["public_key"])
    print("First layer encryption (EXIT):", encrypted_msg)
    encrypted_msg = encrypt(encrypted_msg, NODES[0]["public_key"])
    print("Second layer encryption (ENTRY):", encrypted_msg)
    response = send_message(encrypted_msg)
    # print(response.decode(errors='ignore'))

if __name__ == "__main__":
    main()
