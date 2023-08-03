#!/usr/bin/python3
import socket


def send_packet(path):
    
    packet = f"""GET {path} HTTP/1.1\r
    Host: 127.0.0.1:3490\r
    User-Agent: curl/7.87.0\r
    Accept: */*\r\n\r\n"""
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

# Call the function to send the packet

send_packet("/");
send_packet("/hello");
send_packet("/attacks/out");
