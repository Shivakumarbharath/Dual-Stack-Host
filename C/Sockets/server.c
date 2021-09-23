//Include standard library
#include <stdio.h>
#include <stdlib.h>

//Include the socket and types for networking
#include <sys/types.h>
#include <sys/socket.h>

//For storing the socket address include the structure
#include <netinet/in.h>

int main(){
//Message to send 
char server_msg[256]="Hello! The Socket is created successfully and connection is established\n";




//Create socket
int server_socket;
server_socket=socket(AF_INET,SOCK_STREAM,0);

//Address Structure
struct sockaddr_in server_addr;
server_addr.sin_family=AF_INET;
server_addr.sin_port=htons(9000);
server_addr.sin_addr.s_addr=INADDR_ANY;


//Binding the socket to specified IP and port- syntax same as connect function
bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr));

//listen for connections
listen(server_socket, 5);
/*syntax 
listen(current socket ,no of connections to listen)

*/


//integer to hold the client socket

int client_socket;
client_socket=accept(server_socket,NULL,NULL);
//accept the connection from the client
/*
syntax-
accept(current socket,address of client connection)
parameter-structures to have the address of the client 
if we need the infomration of the client we can send the structure so that it will fill the data of the client
*/

//Send the message to the client socket

send(client_socket,server_msg,sizeof(server_msg),0);
//Synatax same as recv


//close the socket
close(server_socket);

return 0;
}
