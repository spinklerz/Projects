import socket

def handle_client(data: str) -> str:
    try: 
        num1, num2 = data.split(',')
        return str(int(num1) + int(num2))
    except ValueError as e:
        return f"Invalid input: {e}"


def tcp_server(host: str, port: int):
    try:
        # 1.) create a socket Object
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # socket.socket is used mainly for communication between devices 
        # socket.AF_INET is used for IPv4 addresses, if we what IPv6 we would use socket.AF_INET6
        # socket.SOCK_STREAM is used for TCP connections, if we wanted UDP we would use socket.SOCK_DGRAM
        # So we expect a TCP connection over IPv4
        # 2.) bind the socket to an address and port
        server_socket.bind((host,port))
        # random port number
        # 3.) listen for incoming connections
        server_socket.listen(1)

        client_socket, client_address = server_socket.accept()
        print(f"Connected by {client_address}") 
        print("Waiting for Client Response...")
        while True:
            # Accept a connection
            data = client_socket.recv(1024).decode()
            if not data:
                break
            # Send a welcome message
            print(f"Received: {data}")
            message = handle_client(data)
            client_socket.send(message.encode('utf-8'))
            # Close the connection
        client_socket.close()
    except socket.gaierror as e:
        print(f"Address-related error connecting to server: {e}")
    except socket.error as e:
        print(f"Socket error: {e}")
    except  Exception as e: 
        print(f"An error occurred: {e}")
    # listen for incoming connections, the argument is the maximum number of queued connections
tcp_server('localhost', 12345) # Call the function to start the server 
# Note: This code will not run successfully as is, as there are some missing parts and errors.
