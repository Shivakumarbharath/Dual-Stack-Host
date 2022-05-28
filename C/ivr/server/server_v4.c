
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#define BUFFER_SIZE 1024

void error(const char *msg){
perror(msg);
exit(1);
}
//send audio function
int send_audio(int ser_sock,char *in,struct sockaddr_in6 cli){
    char buffer[BUFFER_SIZE];
bzero(buffer, BUFFER_SIZE);
FILE *fp;
fp = fopen(in, "rb"); 
     if (fp == NULL) 
         printf("File open failed!\n"); 
     else
         printf("File successfully opened!\n");
int i=0;
//int n;
    while (1) {
//printf("%d\n",i++);
    size_t num_read = fread(buffer, 1, BUFFER_SIZE, fp);
//printf("%d",num_read);
    if (num_read == 0) // end of file.
	//n= sendto(ser_sock, buffer,num_read, 0, (struct sockaddr*)&cli, sizeof(cli));
        break;
int n;
    //n = write(cli_sock, buffer, num_read);
    n= sendto(ser_sock, buffer,num_read, 0, (struct sockaddr*)&cli, sizeof(cli));
    if (n < 0) // Error
        error("ERROR writing to socket\n\n");
    else if (n == 0) // Could handle this too
        break;
    }
   sendto(ser_sock, buffer,0, 0, (struct sockaddr*)&cli, sizeof(cli));
   printf("sending %s file\n ",in);
    printf("File sending complete...\n\n\n");

    if (fp != NULL)  
        fclose(fp);
//confirmation
int choice;
int addr_len=sizeof(cli);
recvfrom(ser_sock, &choice, sizeof(choice), 0, (struct sockaddr*)&cli, &addr_len);
return 0;
}



void main(int argc, char **argv){
  if(argc != 3){
    printf("Usage: %s <port> <network adapter>\n", argv[0]);
    exit(0);
  }

  int port = atoi(argv[1]);
  int ser_sock;
  struct sockaddr_in6 ser, cli;
  char buffer[BUFFER_SIZE];
  socklen_t addr_size;
  int option=1;

  ser_sock = socket(AF_INET6, SOCK_DGRAM, 0);
if(ser_sock<0){error("Error In Socket Creatation");}
  setsockopt(ser_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));//reuse address immediatly after use
  printf("Socket Created Successfully\n\n");
  memset(&ser, '\0', sizeof(ser));
  ser.sin6_family = AF_INET6;
  ser.sin6_port = htons(atoi(argv[1]));
  ser.sin6_addr = in6addr_any;
  ser.sin6_scope_id=if_nametoindex(argv[2]);


  bind(ser_sock, (struct sockaddr*)&ser, sizeof(ser));
  addr_size = sizeof(cli);
//1st exchange to get the address of the client
  recvfrom(ser_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&cli, &addr_size);
  char ip[100];
if(inet_ntop(AF_INET6,&cli,ip,sizeof(ip))==NULL)error("inet_ntop Error");
printf("ipv6 Address : %s\t",ip);
printf("Port Number : %d \n\n\n\n",ntohs(cli.sin6_port));
printf("[+]Data Received: %s", buffer);

//Welcome Note
send_audio(ser_sock,"audio/welcome.mp3",cli);
int choice,converted;
//recvfrom(ser_sock, &choice, sizeof(choice), 0, (struct sockaddr*)&cli, &addr_size);

//Choice of Service
send_audio(ser_sock,"audio/option.mp3",cli);
//int choice,converted;
recvfrom(ser_sock, &choice, sizeof(choice), 0, (struct sockaddr*)&cli, &addr_size);
converted=ntohs(choice);
printf("\n\nThe client Choice is %d\n\n",converted);
//To provide the service
switch(converted){
//Room Availibality Check
//Choose Boys or Girls hostel
case 1:send_audio(ser_sock,"audio/boyorgirl.mp3",cli);
int bg;
recvfrom(ser_sock, &bg, sizeof(bg), 0, (struct sockaddr*)&cli, &addr_size);
printf("bg %d\n",ntohs(bg));
//Get the hostel name
send_audio(ser_sock,"audio/hname.mp3",cli);
recvfrom(ser_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&cli, &addr_size);
printf("name :%s\n\n",buffer);
//get the floor
send_audio(ser_sock,"audio/floor.mp3",cli);
int flr;
recvfrom(ser_sock, &flr, sizeof(flr), 0, (struct sockaddr*)&cli, &addr_size);
printf("floor %d\n",ntohs(flr));
//get the room number
//send_audio(ser_sock,"audio/rno.mp3",cli);
//int rno;
//recvfrom(ser_sock, &rno, sizeof(rno), 0, (struct sockaddr*)&cli, &addr_size);
//printf("Room number %d\n",ntohs(rno));
flr=ntohs(flr);

//Code to check the data base




//since no data base no check
//send the result
if(flr==1){send_audio(ser_sock,"audio/notavail.mp3",cli);}
else{send_audio(ser_sock,"audio/avail.mp3",cli);}
break;

case 2:send_audio(ser_sock,"audio/busnumber.mp3",cli);
int  bus;
recvfrom(ser_sock, &bus, sizeof(bus), 0, (struct sockaddr*)&cli, &addr_size);
bus=ntohs(bus);
printf("Bus number: %d\n",bus);
char num[7];
char p[15]={"audio/"};
sprintf (num, "%d", bus);
strcat(num,".mp3");
strcat(p,num);
if(bus>=1 && bus<=22){
send_audio(ser_sock,p,cli);
}
else{
send_audio(ser_sock,"audio/0.mp3",cli);

}
break;
default:break;
}//switch

send_audio(ser_sock,"audio/thankyou.mp3",cli);

//pclose(ser_sock);
}


