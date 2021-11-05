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


struct s_day{
time_t Date;//Date of Booking
int no_of_slots;
int first_slot_number;
/*if more than one slot than booking can be done for continues slots only*/
};

struct m_day{
time_t dates[6];
int no_of_slots_each_day;
int first_slot_number;};

typedef struct request{
int type;/*0 - booking 
		1 - cancelling 
		2 - Rescheduling*/
int hall;/* 0 – old seminar hall
	       1 – new seminar hall*/
int B_day;/*0 - single day
            1- Multiple day*/
union no_of_days{
struct s_day one_day;
struct m_day multiple_days;} booking_days;
}request;

typedef struct details{
char name[15];
char contact[10];
}det;


void disp(){

printf("\t\tClient runnnig on IPv4 ONLY  Host\n\n");
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

disp();
  int cli_sock, portno, n;
  char recvbuffer[255];
  char recvbuffer2[255];
  int size=255;
  struct sockaddr_in serv_addr;

if(argc<2)error("Usage Filename <Ip Address of Server>");
portno=5000;
  cli_sock = socket (AF_INET, SOCK_STREAM, 0);
  if (cli_sock < 0)
    {
      error ("Error opening a socket!!");
    }
printf("Socket Created Successfully\n\n");
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons (portno);
serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
   /* if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }*/ 

  if (connect (cli_sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) <0)error("Connection Failed");
printf("Connected to server successfully\n\n");
det Send;
strcpy(Send.name,"Bharath");
strcpy(Send.contact,"123456789");
bzero(recvbuffer,255);
strcpy(recvbuffer,"Hello");
n=send(cli_sock,(void *)&Send,sizeof(Send),0);
if(n<0)error("Error while Communication");
printf("Client  sent to server %s : %s\t %s\n",argv[1],Send.name,Send.contact);
//Print time sent
printf("Sent time= %s\n\n",Time());



bzero(recvbuffer,sizeof(recvbuffer));
n=recv(cli_sock,recvbuffer,255,0);
if(n<0)error("Error while Communication");
printf("Client Received from server %s : %s\n",argv[1],recvbuffer);
//print time recieved
printf("Received Time =%s\n\n",Time());

close(cli_sock);
return 0;

}
