import socket 
# Non persistent Connection
host = 'localhost'
port = 12345 
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((host,port))
# Receive data from the server
data = input("Enter two numbers separated by a comma(EX: '4,2'): ")
client_socket.send(data.encode('utf-8'))
response = client_socket.recv(1024).decode('utf-8')
# 1024 is how many bytes reciveed from server
print("Received from server:", response)

    # Close the connection
client_socket.close()