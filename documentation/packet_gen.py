#!/usr/bin/python3
import socket
import time

def send_packet(path, packet):
    target_host = "127.0.0.1"
    target_port = 3490

    try:
        # Create a socket object
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Connect to the server
        client_socket.connect((target_host, target_port))

        # Send the packet
        client_socket.sendall(packet.encode())

        # Receive the response (you can do something with the response if needed)
        response = client_socket.recv(4096)
        print("Response received:")
        print(response.decode())

    except Exception as e:
        print(f"Error occurred: {e}")

    finally:
        # Close the socket
        client_socket.close()
    return response.decode()

# Call the function to send the packet
path = "/"
packet = f"""GET {path} HTTP/1.1\r
Host: 127.0.0.1:3490\r
User-Agent: curl/7.87.0\r
Accept: */*\r\n\r\n"""
print("Okay response")
time.sleep(0.5)
if "HTTP/1.1 200 " in send_packet(path, packet):
    print("Okay test passed :)")
else:
    print("Okay packet fialed :(")
    exit (1)

packet = f"""GET {path} HTTP/1.1\r
Host: 127.0.0.1:3490\r
Host: 127.0.0.1:3490\r
User-Agent: curl/7.87.0\r
Accept: */*\r\n\r\n"""
print("Bad packet repeated header")
time.sleep(0.5)
if "HTTP/1.1 200 " in send_packet(path, packet):
    print("Repeated header test faiiled :(")
    exit(1)
else:
    print("Okay test passed :")

packet = f"""GET {path} HTTP/1.1\r
User-Agent: curl/7.87.0\r
Accept: */*\r\n\r\n"""
print("Bad packet no host")
time.sleep(0.5)
if "HTTP/1.1 200 " in send_packet(path, packet):
    print("Repeated header test faiiled :(")
    exit(1)
else:
    print("Okay test passed :")
