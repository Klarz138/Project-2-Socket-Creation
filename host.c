/* Katherine Larsen 

HOST SIDE: The goal of the project is to create a socket. A socket is a endpoint for sending and receivng data. When a piece of technology needs to communicate over a network, it will create a socket to est. a connection with another endpoint. --> Like a phone line. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h> //Used for establishing network communication between processes running on different machines.vSpecifcally, we are using the connect function

#include <arpa/inet.h> //Allows manpulation of IP addreses and converting between network and host byte order --> Specifically, we are using function inpet_pton

#include <unistd.h> //Allows us to close the socket file descriptor when the client (me) is finhsed communicating with server

// Port number of the server we are connecting to
#define PORT 8080

// Create a struct to hold the server address information.
// Sockaddr_in is the data structure to hold internet address info.
struct sockaddr_in server;

int main(int argc, char const *argv[]) {

  //***SETTING UP VARIABLES: 
  
  // Making a host socket by calling our function socket
  int host_socket = socket(AF_INET, SOCK_STREAM, 0);

  //Making a client socket 
  int client_socket; 

  //addrlen to hold the length of struct 
  int addrlen = sizeof(server);

  //Creating variable to release address 
  int optval = 1;
  
  //Making client message to hold 2048 chars. Want to be sure we give enough space in case message is long 
  char client_message[2048];

  //Pointer to hold host message
  char *host_message = "Hello there! I am the Blanca host! Yes, I got your message!";
  

//Using setsockopt function built into C. This allows some more felxibity as we create the socket. I noticed when I tried to run the host, and then the client. Then the host again, the port would not be automatically released, which lead to an error address already in use. To fix this, I set SO_REUSEADDR to ensure the server socket can be bound to the same port even if it was not properly closed before. 
      setsockopt(host_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  
  // Creating socket file descriptor
  if (host_socket < 0) {
  
    perror("1. We're sorry, but there was a failure to create the socket.");
    //perror prints a message to the standard error stream that describes the error condtion. This is more helpful than a simple printf because otherwise you would have to manually check the value of the error code perror returns

    exit(EXIT_FAILURE);

  } // end of if

  //Setting up the struct: 
   memset(&server, 0, sizeof(server));// Memset allows us to block memeory to a particualr value. Setting struct "server" to all zeros before initilizing it. Making sure that all uninitialzied fields in the struct are set to 0 (just to be safe) 
  
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  // Calling the bind function to bind the server to the specific port 8080. Similar to our connect function we use in the client, both functions take in a file descriptor as their first argument. Both functions also take a pointer to a struct sockadde as their second arg. In this case, it contains the address to which the server socket should be bound. We also have the third argument, which take the size of the struct sockaddr to ensure that the size of the structure being passed is known.
  if (bind(host_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("2. We're sorry but at this time the server could not be binded to the port 8080. Please try again later.");
    exit(EXIT_FAILURE);
  }

  //The server socket is listening for incoming connectings from clients. Our first argrument is the file descriptor --> host_socket. Our second argrument is the max number of connections that can be queued for the server socket. Since there are two other memebers in my group, I set this to 2. This will return a negative value if it is not succesful, with the perror explaining what is wrong. 
  if (listen(host_socket, 2) < 0) {
    perror("Seems as though the server had trouble listening to the client. Please try again...");
    exit(EXIT_FAILURE);
  }

  //Next, we are going to use the accept method so the server can accept incoming connections from the client. When a client attempts to connect to the server, the servers listen method (as implemented above) creates a queue of pending connections for the server socket. The accept method is then used to accept the nect connection from the queue in order, create a new socket for communication with the client, and return a file descriptor for that new socket. 
  /* 
3 Arguments: 
1.) File Descriptor of the server socket that is listening for incoming connections 
2.) A pointer to a struct sockaddr that will be used to store the address of the cleint socket that is connecting to the server 
3.) Pointer that specifices the size of sockaddr) 
    */
  if ((client_socket = accept(host_socket, (struct sockaddr *)&server, (socklen_t *)&addrlen)) < 0) 
  {
    perror("It seems as though the accept call has failed: ");
    exit(EXIT_FAILURE);
  } //END OF IF 

    //Set up message to be receieved
 int receivced_bytes = recv(client_socket, client_message, sizeof(client_message), 0); 
  
  if (receivced_bytes < 0)
  {
    perror("Theres appears to be an erorr with receiving a message from the client: ");
    exit(EXIT_FAILURE);
  } //END OF IF 
  
  //Send function will return number of bytes that were succesffsuly sent. So if the return balue is is less than 0, and error must have occured. 
 if (send(client_socket, host_message, strlen(host_message), 0) < 0) 
 {
  
    perror("4. We're sorry, but it appears as through your message failed to send to the server. Please try again later. ");

  exit(EXIT_FAILURE); 
    }//END OF IF

//Finally, print the host and client message so it is clear what was transmitted
  printf("\nMessage SENT to the client:\n %s\n", host_message);
  printf("\nMessage RECEIVED from the client:\n %s\n", client_message);

  // closing the connected socket for saftey 
  close(client_socket);
  // closing the listening socket for saftey 
  shutdown(host_socket, SHUT_RDWR);
  
  return 0;
} //END OF MAIN 
