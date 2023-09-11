# Project-2-Socket-Creation
Side Project #2! I created a side project to better help grasp the networking concept of sockets. Sockets serve as an endpoint for communication between two computers over a network. For this project I have created a host and a client.

Here is a detailed explanation of how each the host and client work together: 

Host (Server) Side:

The server code begins by including necessary header files, such as <stdio.h>, <stdlib.h>, <string.h>, and network-related headers like <sys/socket.h> and <arpa/inet.h>.

It defines a constant PORT with the port number (8080) on which the server will listen for incoming connections.

It creates a socket for the server using the socket() function. The AF_INET argument specifies the address family (IPv4), SOCK_STREAM specifies the socket type (TCP), and 0 indicates that the OS should choose the appropriate protocol.

The code sets up the server's address information using a struct sockaddr_in called server. It initializes the address family, IP address (INADDR_ANY), and port number.

The bind() function binds the server socket to the specified IP address and port number.

The server then listens for incoming connections using the listen() function, allowing up to two pending connections in the queue.

When a client connects, the accept() function is called. It accepts the incoming connection, creates a new socket (client_socket) for communication with that client, and returns the file descriptor of the new socket.

The server receives a message from the client using the recv() function and stores it in the client_message buffer.

It sends a response message ("Hello there! I am the Blanca host! Yes, I got your message!") back to the client using the send() function.

After sending the response, the server closes the client_socket and shuts down the host_socket.

Client Side:

The client code also includes necessary header files and defines constants for the server's IP address (SERVER_IP) and port number (SERVER_PORT).

It creates a client socket using the socket() function, specifying the same address family (IPv4) and socket type (TCP).

The client sets up the server's address information, including the IP address and port number, in a struct sockaddr_in called server.

It uses the connect() function to establish a connection to the server. If the connection fails, an error message is printed, and the program exits.

The client sends a message ("Please work!! This is the client, Katherine! Hi, Blanca Server!") to the server using the send() function.

It receives a response message from the server using the recv() function and stores it in the host_response buffer.

The client then prints both the message it sent and the response received from the server.

Finally, it closes the client socket using the close() function.

In summary, the server listens for incoming connections, accepts one client connection, receives a message from the client, sends a response, and then closes the sockets. On the client side, it establishes a connection to the server, sends a message, receives a response, and also closes the socket. This code demonstrates a simple client-server interaction over TCP/IP sockets.
