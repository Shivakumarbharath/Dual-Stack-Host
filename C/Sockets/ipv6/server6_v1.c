#include <stdio.h>//To include standard input and output functions
#include <stdlib.h>//macros and functions
#include <sys/types.h>//To include number of datatypes for system calls
#include <sys/socket.h>//Functions needed to create,bind,listen,accept
#include <netinet/in.h>//to include the constants and structures for domain address
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>//to use inet ntoa(struct in_addr)to convert struct to ip address
#include <time.h>
#include <net/if.h>


//Funtion to display error if any event fails
char *Time(){
time_t t;
time(&t);
return ctime(&t);
} 
void error(const char *msg){
perror(msg);// inbuilt function that outputs as an error
exit(1);
}

//Main function
int main(int argc, char *argv[]){

if(argc<2)error("Usage Filename <Port Number of Server>");





//declare variable for server socket,accepted client connection and portnumber
int ser_sock,cli_sock,portno;
//portno=5000;
//declare varble to send and receieve message
char buffer[255];
//To store the ip address of client
char ip[130];
//structure to store server address and client address
struct sockaddr_in6 serv_addr,client_addr;//from netinet/in
socklen_t client_len;
//create server socket
ser_sock=socket(AF_INET6,SOCK_STREAM,0);
//Check for successfull socket creation
if(ser_sock<0){error("Error in Socket creation");}
printf("Socket Created Successfully\n\n");
//clear the contents in the declared variable
bzero((char *)&serv_addr,sizeof(serv_addr));
int opt=1;

/*if (setsockopt(ser_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
*/


//Load the port number and server address into the structure
serv_addr.sin6_family=AF_INET6;
serv_addr.sin6_port=htons(atoi(argv[1]));
serv_addr.sin6_addr=in6addr_any;
serv_addr.sin6_scope_id=if_nametoindex("enp4s0");

//Bind the socket to address of the server and check if successfull
if(bind(ser_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)error("Error while Binding the socket");
printf("Binding Successfull\n\n");

//Listen by the server

listen(ser_sock,5);
printf("Listening!!!\n\n");
//accept from client
client_len=sizeof(client_addr);
cli_sock=accept(ser_sock,(struct sockaddr *)&client_addr,&client_len);


if(cli_sock<0)error("Error while Accepting the connection");
printf("Connection Accepted\n");
if(inet_ntop(AF_INET6, &client_addr, ip, sizeof(ip))==NULL)error("inet_ntop Error");
printf("ipv6 Address : %s\t",ip);
printf("Port number %d\n\n\n\n",client_addr.sin6_port);
//Read from client
int n;
bzero(buffer,sizeof(buffer));
n=read(cli_sock,buffer,255);
if(n<0)error("Error while Communication");
printf("Server Received from client %s : %s\n",ip,buffer);
//print time recieved
printf("Recieced Time =%s\n\n",Time());

sleep(2);
//Send back ack
//bzero(buffer,sizeof(buffer));
char k[]=" -ack";
strncat(buffer,k,sizeof(k));
n=write(cli_sock,buffer,sizeof(buffer));
if(n<0)error("Error while Communication");
printf("Server sent to client %s : %s\n",ip,buffer);
//Print time sent
printf("Sent time= %s\n\n",Time());
close(cli_sock);
close(ser_sock);

return 0;

}
