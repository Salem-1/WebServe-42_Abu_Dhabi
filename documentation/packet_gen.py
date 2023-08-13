#!/usr/bin/python3
import socket
import time
import sys



def send_packet(path):
    
    packet = f"""POST {path} HTTP/1.1\r
    Host: 127.0.0.1:3490\r
    User-Agent: curl/7.87.0\r
    Content-Type: application/x-www-form-urlencoded\r
    Content-Length: 19\r
    Accept: */*\r\n\r\n
    comment=Ahmed\0Mahdy"""
    target_host = "127.0.0.1"
    target_port = 5555

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
        
def send_chunked_packet(path, comment):
    
    str = f"""POST {path} HTTP/1.1\r
    Host: 127.0.0.1:3490\r
    User-Agent: curl/7.87.0\r
    Accept: */*\r
    Content-Type: application/x-www-form-urlencoded\r
    Transfer-Encoding: chunked\r\n\r\n"""
    
    length = hex(len("comment=" + comment))
    packet = [str ,
    f"""{length}\r\n""", f"""comment={comment}\r\n""", """0\r\n\r\n""" ]
    target_host = "127.0.0.1"
    target_port = 5555
    


    try:
        # Create a socket object
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Connect to the server
        client_socket.connect((target_host, target_port))

        # Send the packet
        for pack in packet:
            client_socket.sendall(pack.encode())
            # input('A\n')
            # time.sleep(1)

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

# send_chunked_packet("/", sys.argv[1]);
if(len(sys.argv) == 1):
    send_packet("/")
else:
    send_chunked_packet("/", "green\0ahmed")
# send_packet("/hello");
# send_packet("/attacks/out");
