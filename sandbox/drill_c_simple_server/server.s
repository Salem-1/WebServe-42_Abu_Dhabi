.syntax unified
.global _start

// Constants
.equ AF_INET, 2
.equ SOCK_STREAM, 1
.equ IPPROTO_TCP, 6
.equ SOMAXCONN, 128
.equ SYS_SOCKET, 281
.equ SYS_BIND, 282
.equ SYS_LISTEN, 283
.equ SYS_ACCEPT, 285
.equ SYS_READ, 295
.equ SYS_WRITE, 296
.equ SYS_CLOSE, 6
.equ STDOUT_FILENO, 1

// Socket file descriptors
.equ SERVER_SOCKET, 0
.equ CLIENT_SOCKET, 1

.data
server_address: .space 16
server_port: .hword 8080

client_address: .space 16
client_port: .hword 0

buffer: .space 2000

response: .asciz "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 26\n\nHello from the server!"

.text
.globl _start

_start:
    // Create a socket
    mov x0, AF_INET
    mov x1, SOCK_STREAM
    mov x2, IPPROTO_TCP
    mov x8, SYS_SOCKET
    mov x16, 0
    svc 0
    mov x9, x0
    str x9, [x10, SERVER_SOCKET]

    // Bind the socket to a port
    ldr x0, [x10, SERVER_SOCKET]
    mov x1, xzr
    lsl x1, x1, 48
    str x1, [x10, server_address]
    ldrh x1, server_port
    strh x1, [x10, server_address + 8]
    mov x2, 16
    mov x8, SYS_BIND
    mov x16, 0
    svc 0

    // Listen for connections
    ldr x0, [x10, SERVER_SOCKET]
    mov x1, SOMAXCONN
    mov x8, SYS_LISTEN
    mov x16, 0
    svc 0

server_loop:
    // Accept a connection
    ldr x0, [x10, SERVER_SOCKET]
    mov x1, xzr
    lsl x1, x1, 48
    str x1, [x10, client_address]
    ldr x1, client_port
    strh x1, [x10, client_address + 8]
    mov x2, 16
    mov x8, SYS_ACCEPT
    mov x16, 0
    svc 0
    mov x9, x0
    str x9, [x10, CLIENT_SOCKET]

    // Read data from the client
    ldr x0, [x10, CLIENT_SOCKET]
    ldr x1, buffer
    mov x2, 2000
    mov x8, SYS_READ
    mov x16, 0
    svc 0

    // Print the data to the console
    mov x0, STDOUT_FILENO
    ldr x1, buffer
    mov x2, x0
    ldr x3, =2000
    mov x8, SYS_WRITE
    mov x16, 0
    svc 0

    // Send a HTTP response to the client
    ldr x0, [x10, CLIENT_SOCKET]
    ldr x1, =response
    ldr x2, =26
    mov x8, SYS_WRITE
    mov x16, 0
    svc 0

    // Close the client socket
    ldr x0, [x10, CLIENT_SOCKET]
    mov x8, SYS_CLOSE
    mov x16, 0
    svc 0

    b server_loop

.end
