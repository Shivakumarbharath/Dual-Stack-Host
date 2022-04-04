
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <net/if.h>
#define BUFFER_SIZE 255
void disp(){
 
 printf("\t\tClient runnnig on IPv6 ONLY  Host\n\n");
 }


char *Time(){
 time_t t;
 time(&t);
 return ctime(&t);
 } 

void
error (const char *msg)
{
  perror (msg);			//inbuilt function that outputs with stderr with error number
  exit (1);
}
int
main (int argc, char *argv[])
{				//to take the arguments from the command line


  int cli_sock, portno, n;
  char buffer[255];
  char recvbuffer2[255];
  int size=255;
  struct sockaddr_in6 serv_addr;
  FILE *out;
if(argc<3)error("Usage Filename <Ip Address of Server> <Portnumber of server> <network adapter>");
disp();
portno=atoi(argv[2]);
  cli_sock = socket (AF_INET6, SOCK_STREAM, 0);
  if (cli_sock < 0)
    {
      error ("Error opening a socket!!");
    }
printf("Socket Created Successfully\n\n");
serv_addr.sin6_family = AF_INET6;
serv_addr.sin6_port = htons (portno);
serv_addr.sin6_scope_id=if_nametoindex(argv[3]);
//serv_addr.sin6_addr=inet_addr(argv[1]);
    if(inet_pton(AF_INET6, argv[1], &serv_addr.sin6_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

  if (connect (cli_sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) <0)error("Connection Failed");
printf("Connected to server successfully\n\n");

/*
bzero(recvbuffer,255);
fgets(recvbuffer,sizeof(recvbuffer),stdin);
n=write(cli_sock,recvbuffer,sizeof(recvbuffer));
if(n<0)error("Error while Communication");
printf("Client  sent to server %s : %s\n",argv[1],recvbuffer);
//Print time sent
printf("Sent time= %s\n\n",Time());



bzero(recvbuffer,sizeof(recvbuffer));
n=read(cli_sock,recvbuffer,255);
if(n<0)error("Error while Communication");
printf("Client Received from server %s : %s\n",argv[1],recvbuffer);
//print time recieved
printf("Received Time =%s\n\n",Time());
*/
// while (1)
//     {
//       bzero (buffer, 255);
//       printf("Client Sent: ");
//       fgets (buffer, 255, stdin);
//       n = write (cli_sock, buffer, strlen (buffer));
//       printf("Time Sent : %s",Time());
//       if (n < 0)
// 	error ("Wrinting Failed");
//       bzero (buffer, 255);
//       n = read (cli_sock, buffer, 255);
//       if (n < 0)
// 	error ("Reading Failed");
//       printf ("Client Recieved : %s", buffer);
//       int i = strncmp ("q", buffer,1);
//       if (i == 0)
// 	break;
//     }

//   return 0;



out = fopen("out.mp3", "wb");
    while (1) {
    n = read(cli_sock, buffer, BUFFER_SIZE);
    if (n < 0) 
        error("ERROR reading from socket");
    else if (n == 0) // Socket closed. Transfer is complete (or borked)
        break;

    fwrite(buffer, 1, n, out); // Could check fwrite too.
}
    printf("File write complete... You can now use the output file!!\n");
system("mpg123 out.mp3");
system("rm out.mp3");
    if (out != NULL)  
        fclose(out);


int c = close(cli_sock);
if(c!=0){error("Error Closing");c=close(cli_sock);if(c==0)printf("Socket closed");}
return 0;

}
