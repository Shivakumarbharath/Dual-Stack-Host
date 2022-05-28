#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#define BUFFER_SIZE 1024

void error(const char *msg){perror(msg);exit(1);}

//TO use fgets after scanf
void scan(int *p){
scanf("%d",p);
while ((getchar()) != '\n');
return;
}
//To recieve the audio file play and delete the file from memory
int recive_play(int cli_sock,struct sockaddr_in6 serverAddr){
int n;
FILE *out;
char buffer[BUFFER_SIZE];
out = fopen("out.mp3", "wb");
int addrlen=sizeof(serverAddr);
    while (1) {
//printf("Recieveing... %d\n",n);
    //n = read(cli_sock, buffer, BUFFER_SIZE);
    n=recvfrom(cli_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, &addrlen);
//printf("Recieveing... %d\n",n);

    if (n < 0) 
        error("ERROR reading from socket");
    else if (n == 0) // Socket closed. Transfer is complete (or borked)
        break;

    fwrite(buffer, 1, n, out); // Could check fwrite too.
if(buffer[n-1]==EOF)break;
}
    printf("File write complete... You can now use the output file!!\n");
system("mpg123 out.mp3");
system("rm out.mp3");
    if (out != NULL)  
        fclose(out);
int converted;
//send confirmation
sendto(cli_sock, &converted, sizeof(converted), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
return 0;
}


void main(int argc, char **argv){
  if(argc < 3){
    printf("Usage: %s <ip address> <port>\n", argv[0]);
    exit(0);
  }

  int port = atoi(argv[2]);
  int cli_sock;
  struct sockaddr_in6 serverAddr;
  char buffer[BUFFER_SIZE];
  socklen_t addr_size;

  cli_sock = socket(PF_INET6, SOCK_DGRAM, 0);
  memset(&serverAddr, '\0', sizeof(serverAddr));

  serverAddr.sin6_family = AF_INET6;
  serverAddr.sin6_port = htons(port);
  //serverAddr.sin6_scope_id=if_nametoindex(argv[3]);

if(inet_pton(AF_INET6,argv[1],&serverAddr.sin6_addr)<=0)error("\n inet_pton error occured\n\n");

  strcpy(buffer, "Hello Server\n");
//1st exchange to record the address by the server
  sendto(cli_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  printf("[+]Data Send: %s", buffer);
//Play welcome note
  recive_play(cli_sock,serverAddr);
int choice, converted=0;
//sendto(cli_sock, &converted, sizeof(converted), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
 
//Prompt the choice of service
 recive_play(cli_sock,serverAddr);
printf("\n\nChoice ?:");
//int choice,converted;
scanf("%d",&choice);
converted=htons(choice);
  sendto(cli_sock, &converted, sizeof(choice), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

//TO process each service request
switch(choice){
//Get boys or girls
case 1:recive_play(cli_sock,serverAddr);
printf("Choice?");
int bg,convertedbg;
scan(&bg);
convertedbg=htons(bg);
 sendto(cli_sock, &convertedbg, sizeof(bg), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

//get hostel name
recive_play(cli_sock,serverAddr);
printf("Hostel name:");
fgets (buffer, 255, stdin);
sendto(cli_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

//get floor
recive_play(cli_sock,serverAddr);
printf("Floor:?");
int flr,convertedflr;
scan(&flr);
convertedflr=htons(flr);
sendto(cli_sock, &convertedflr, sizeof(flr), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

//recive_play(cli_sock,serverAddr);
//printf("Room Number:?");
//int rno,convertedrno;
//scan(&rno);
//convertedrno=htons(rno);
// sendto(cli_sock, &convertedrno, sizeof(rno), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

recive_play(cli_sock,serverAddr);

break;
case 2:
//You have chosen bus details
//enter the bus number 
recive_play(cli_sock,serverAddr);
printf("Bus Number : ");
int  bus,convertedbus;
scan(&bus);
if(bus>=1 && bus<=22){
convertedbus=htons(bus);
}
else{
convertedbus=htons(0);
}
 sendto(cli_sock, &convertedbus, sizeof(bus), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
 recive_play(cli_sock,serverAddr);
break;

default:break;
}//switch
recive_play(cli_sock,serverAddr);

}

