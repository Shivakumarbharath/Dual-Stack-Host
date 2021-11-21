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

//TO use fgets after scanf
void scan(int *p){
scanf("%d",p);
while ((getchar()) != '\n');
return;
}

void Printf(const char *msg ){printf("\n\n\n-------------------------------------------%s---------------------------------------\n\n\n\n",msg);}
typedef struct exchange
{

int type;/*0 - booking 
				   1 - cancelling 
				   2 - Rescheduling */


int hall;//Old Seminar hall-0(take H1)
	//New Seminar hall 1(take h2)
int month_type;//current month-0
		//next month-1
int event_day;//month.date1[event_day-1]

int slot_no;//month.date1[event_day-1].slots[slot_no]

}exchange;

typedef struct slot
{ int booking_status;
  char name[50];
  char contact[11];
  unsigned long long int booking_id;
}slot;


typedef struct Message{
char name[15];
char contact[10];
}msg;

void disp(){

printf("\t\tClient runnnig on IPv4 ONLY  Host\n\n");
}
void Display_slot(slot info){

printf("\n\tBooking Details\n\n");
printf("Name : %s\n",info.name);
printf("Contact Number : %s\n\n",info.contact);
int day,month,year,hall,slot;
day=info.booking_id/100000000;
month=(info.booking_id/1000000)%100;
year=(info.booking_id/100)%10000;
hall=(info.booking_id%100)/10;
slot=info.booking_id%10;
printf("Date - %02d/%02d/%d\n\n",day,month,year);
if(hall==1)printf("Seminar Hall : Old Seminar Hall\n\n");
if(hall==2)printf("Seminar Hall : New Seminar Hall\n\n");
char time[12];
switch(slot){
case 1:strcpy(time,"9AM - 10AM");break;
case 2:strcpy(time,"10AM - 11AM");break;
case 3:strcpy(time,"11AM - 12PM");break;
case 4:strcpy(time,"12PM - 1PM");break;
case 5:strcpy(time,"1PM - 2PM");break;
case 6:strcpy(time,"2PM - 3PM");break;
case 7:strcpy(time,"3PM - 4PM");break;
}
printf("Slot Time %s\n\n",time);
printf("Booking Id : %llu\n\n",info.booking_id);
return;
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
/*msg Send;
strcpy(Send.name,"Bharath");
strcpy(Send.contact,"123456789");
bzero(recvbuffer,255);
strcpy(recvbuffer,"Hello");
n=send(cli_sock,(void *)&Send,sizeof(Send),0);
if(n<0)error("Error while Communication");
printf("Client  sent to server %s : %s\t %s\n",argv[1],Send.name,Send.contact);*/
//Print time sent
//printf("Sent time= %s\n\n",Time());


exchange req;
slot con;
  printf
    ("\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\tWELCOME TO SEMINAR HALL BOOKING APP\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\tChoose the following\n1.Booking\n2.Cancelling\n3.Rescheduling\n4.Exit\n\n\nchoice? :");
  scan (&req.type);
  if (req.type==4)exit(0);
  //while ((getchar()) != '\n');
  switch (req.type -= 1)
  {
case 0:
    {
      printf
	("\n\n\tYou Have chosen For Booking the Hall\nWhich Seminar Hall do You want to Book\n1.Old Seminar Hall\n2.New Seminar Hall\n\nChoice? : ");
      scan (&req.hall);
//    while ((getchar()) != '\n');
      req.hall -= 1;
      req.
	hall ? printf ("You have chosen New Seminar Hall\n") :
	printf ("You Have chosen Old Seminar hall\n");
     /* printf ("Booking type \n1.Single Day\n2.More than one day\n");
      scan ( &req.B_day);
  //    while ((getchar()) != '\n');
      switch (req.B_day -= 1)
	{
	case 0:
	  printf ("One Day Booking\n");break;
	case 1:
	  printf ("Booking Service not available");
	  exit (1);
	default:
	  printf ("Invalid Choice");
	  exit (1);
	}*/
	printf("\n\n1.Current Month\n2.Next Month\nChoice? :");
	scan(&req.month_type);
	req.month_type-=1;
	req.month_type?printf("\n\nYou have chosen Next Month\n"):printf("\n\nYou have chosen Current Month\n");
	printf("\n\nEnter the date you want to book? : ");
	scan(&req.event_day);
	req.event_day-=1;
	printf("\n\nEnter Slot Number(1-7) : ");
	scan(&req.slot_no);
	req.slot_no-=1;
	//printf("%d\n%d\n%d\n%d\n%d",req.type,req.hall,req.month_type,req.event_day,req.slot_no);
	//Send to server for checking
	n=send(cli_sock,(void *)&req,sizeof(req),0);
      if(n<0)error("Error sending Details\nTry again");

	//receieve response as slot with booking id and booking status
	n=recv(cli_sock,(slot *)&con,sizeof(con),0);
	if(n<0)error("Error Receieving Status\nTry again");
	//if booking status ==0 take details
	if(con.booking_status==0)
	{
		printf ("\n\nThe slot is Free\n\nPlease Provide your Contact Details\nYour Name : ");
		fgets (con.name, sizeof (con.name), stdin);
		printf ("Contact Number : ");
		fgets (con.contact, sizeof (con.contact), stdin);
		n=send(cli_sock,(void *)&con,sizeof(con),0);
      		if(n<0)error("Error sending Details\nTry again");
      		Printf("Booking Successfull");
      		Display_slot(con);
	}
	else
	{
		Printf("Booking Failed");
		printf("\n\nThe Slot Has already been Booked\nDetails Given\n");
		Display_slot(con);
		
	}
	break;
}

case 1:
	{
   	printf
         ("\tYou Have chosen For Cancellation of Booking the Hall\nWhich Seminar Hall did You Book\n1.Old Seminar Hall\n2.New Seminar Hall\n\nChoice? : ");
         scan (&req.hall);
         req.hall -= 1;
         req.hall ? printf ("You had chosed New seminar Hall\n"):printf ("You Had chosed Old seminar hall\n");
  	printf("\n\n1.Current Month\n2.Next Month\nChoice? :");
         scan(&req.month_type);
         req.month_type-=1;
         req.month_type?printf("\n\nYou had chosed Next Month\n"):printf("\n\nYou had chosed Current Month\n");
         printf("\nEnter the date you booked? : ");
         scan(&req.event_day);
         req.event_day-=1;
         printf("\nEnter Slot Number(1-7) : ");
         scan(&req.slot_no);
         req.slot_no-=1;
        //Send when to cancel
         n=send(cli_sock,(void *)&req,sizeof(req),0);
         if(n<0)error("Error sending Details\nTry again");
	char report[255];
	n=recv(cli_sock,&report,sizeof(report),0);
	puts(report);
	Printf("Booking Cancelled Successfully");
	}
default:
    printf ("Service Not Available");
    exit (1);


}
/*
bzero(recvbuffer,sizeof(recvbuffer));
n=recv(cli_sock,recvbuffer,255,0);
if(n<0)error("Error while Communication");
printf("Client Received from server %s : %s\n",argv[1],recvbuffer);
//print time recieved
printf("Received Time =%s\n\n",Time());
*/
close(cli_sock);
printf("\n\nClient Socket Closed\n\n");
return 0;

}
