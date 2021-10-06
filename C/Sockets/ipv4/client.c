//We need include from standard library
#include <stdio.h>
#include <stdlib.h>

//We need types and socket api which contains most important functionality
#include <sys/types.h>
#include <sys/socket.h>

//we need structures to store the  address information 
#include <netinet/in.h>

int
main ()
{

//create a socket declare an interger for the address
  int network_socket;
  network_socket = socket (AF_INET, SOCK_STREAM, 0);
/*Parameter 1=domain name(internet socket we will pass),
Parameter 2 -to decide the layer 4 protocol udp or tcp 
for tcp-SOCK_STREAM
parameter-just specifies the protocol here 0 as we use tcp -default protocol
There are times where we have to specify the protocol explicitly
*/

//To connect to other socket
//we need to call the connect function in the socket
//But before we call the connect we should be able to specify to the address we have to connect
//netinet contains a structure that defines feilds of port number and ip address
  struct sockaddr_in server_address;
//specify the address family same as family of socket i.e AFINET
  server_address.sin_family = AF_INET;
//specify the port
  server_address.sin_port = htons (9000);
/*data format is slightly different so we need to convert integer to right network byte order
so we use htons(int port)
*/
//finally specify the ip address
//sin_addr is struct 
  server_address.sin_addr.s_addr = INADDR_ANY;	//=0000


//Now we can connect 
  int conn =
    connect (network_socket, (struct sockaddr *) &server_address,
	     sizeof (server_address));
/*Syntax-
connect(current socket to connect ,address(cast the server address structure to sligtly different structure(struct sockaddr*)),size of the server
address )
this fucntion returns an integer so that we can do error handling
0-everything ok
-1-not a good connection
*/

  if (conn == -1)
    {
      printf ("There was an error while connection");
    }
//Now send or recieve data
//to fetch data declare string
  char server_response[256];
  recv (network_socket, &server_response, sizeof (server_response), 0);
/*syntax
recv(current socket,address of the reciver,size of the reciver,optional flags parameter)
*/

//print out the data we get back

  printf ("The server sent the data:\n %s", server_response);


//Close the socket once exchange is done
  close (network_socket);


  return 0;

}
