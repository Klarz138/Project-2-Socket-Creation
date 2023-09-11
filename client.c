/*Katherine Larsen
CLIENT SIDE: The goal of the project is to create a socket. A socket is a endpoint for sending and receivng data. When a piece of technology needs to communicate over a network, it will create a socket to est. a connection with another endpoint. --> Like a phone line.
*/
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

#include <sys/socket.h> //Used for establishing network communication between processes running on different machines.vSpecifcally, we are using the connect function

#include <arpa/inet.h> //Allows manpulation of IP addresses and converting between network and host byte order --> Specifically, we are using function inpet_pton

#include <unistd.h> //Allows us to close the socket file descriptor when the client (me) is finhsed communicating with server 

//IP Address of the Server we are connecting to (this information has been changed due to how this repo is public)
#define SERVER_IP "1.11.1.1" 

//Port number of the server we are connecting to 
#define SERVER_PORT 8080

//Create a struct to hold the server address information.
//Sockaddr_in is the data structure to hold internet address info. 
struct sockaddr_in server; 
      
/*
struct sockaddr_in 
{
    short int sin_family;            // Address family (IPV4)
    unsigned short int sin_port;     // Port number 
    struct in_addr sin_addr;         // IP Address of the Server
    unsigned char sin_zero[8];       //  Pad struct to same same as sockaddr  };
*/

int main(int argc, char const *argv[]) 
{
  
//***SETTING UP VARIABLES: 
  
  //Making a socket by calling our function socket 
  int client_socket = socket (AF_INET, SOCK_STREAM, 0); //  AF_INET = Address Family --> specifies communication domain (IPv4) 
  //2.) SOCK_STEAM = Specifies the type of socket. Set the protocol to 0 to allow the OS to choose the appriopate protocol. Based off the project's instructions I am choosing TCP (0) 
  
  //Setting up client message to send to server: 
  char* client_message = "Please work!! This is the client, Katherine! Hi, Blanca Server!";
   
  //set up host response: 
  char host_response[2048]; 

  //Check if Socket was created succesfully: 
  if (client_socket < 0)
  {
    //perror preints a message to the standard error stream that descrives the error condtion. This is more helpful than a simple printf       because otherwise you would have to manually check the value of rrno code perror returns
      perror("1. We're sorry, but there was a failure to create the socket.");
    
      exit(EXIT_FAILURE); 
    
  } //end of if 

  //Setting up the struct: 
   memset(&server, 0, sizeof(server));

  /*  
  Memset allows us to block memeory to a particualr value. Setting struct "server" to all zeros before initilizing it. Making sure that      all uninitialzied fields in the struct are set to 0 (just to be safe) 
  */   

  //Continuing setting up the struct
  server.sin_family = AF_INET; // Set the address family to IPv4

  server.sin_port = htons(SERVER_PORT); // Set the port number we Port number of server. Htons = "host to network short" --> Converts a 16 bit number from host byte to a network btye order. Essentially ensures that the bytes are in the correct order when they are transmitted over network 

//Convert the server IP address so the computer can understand it. If the IP address is invalid or can't be converted return with failure status code using perror
if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) <= 0)
{
  perror("2. We're sorry, but it appears that is an invlaid IP address. Please try again. ");
  
  exit(EXIT_FAILURE);
}

/*
  Attempting to connect to the server If connection fails return the error message/code using perror
    
    connect function will take in 3 arguments: 
  
      1.) client_socket = clients socket file descriptor (an integer value that represents an open socket)

      2.) A pointer to the struct that contains the servers address info, 
          which is then cast as a pointer to the sockaddr struct --> Lol this was most difficult section! Pointers are rough!

      3.) The size of the server struct in bytes
    */
  
  if (connect(client_socket, (struct sockaddr*) &server, sizeof(server)) < 0)  
  {
      perror("3. We're sorry, but seems as though we ran into difficuility connecting the server. Please try again!");
    
      exit (EXIT_FAILURE);
  }//END OF IF 

  //Send function will return number of bytes that were succesffsuly sent. So if the return balue is is less than 0, and error must have occured. 
  if (send(client_socket, client_message, strlen(client_message), 0) < 0)
  {
    perror("4. We're sorry, but it appears as through your message failed to send to the server. Please try again later. ");

  exit(EXIT_FAILURE); 
    }//END OF IF

    //Set up message to be received
  int received_bytes = recv(client_socket, host_response, sizeof(host_response), 0);
  
if (received_bytes < 0) {
    perror("Failed to receive data from server");
    exit(EXIT_FAILURE);
}
else if (received_bytes == 0) {
    printf("Connection closed by server\n");
    exit(EXIT_SUCCESS);
}
else {
    // Null-terminate the received data to turn it into a string
    host_response[received_bytes] = '\0';
}
  printf("\nMessage SENT to the Host:\n%s", client_message);
printf("\n\nYAY! RECEIVED message from server!!: \n%s", host_response); 

  //FINALLY, to be safe and practice code correctly! Close the socket!
  close(client_socket); 
  return 0; 
  
}
  
