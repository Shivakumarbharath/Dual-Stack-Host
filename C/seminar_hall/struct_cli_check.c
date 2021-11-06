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

//To save the information of single day booking
struct s_day
{
  struct tm Date;		//Date of Booking
  int no_of_slots;
  int first_slot_number;
/*if more than one slot than booking can be done for continues slots only*/
};

//To save the information of multiple day booking
struct m_day
{
  struct tm dates[6];
  int no_of_slots_each_day;
  int first_slot_number;
};




//To save the information related to Booking
typedef struct request
{
  int type;			/*0 - booking 
				   1 - cancelling 
				   2 - Rescheduling */
  int hall;			/* 0 – old seminar hall
				   1 – new seminar hall */
  int B_day;			/*0 - single day
				   1- Multiple day */
  union no_of_days
  {
    struct s_day one_day;
    struct m_day multiple_days;
  } booking_days;
} request;


//To save the information of user
typedef struct details
{
  char name[20];
  char contact[11];
} det;

//To save the information of both the above structures

   typedef struct full
    {
      request r;
      det contacts;
    }full; 


//Struct to send the response
  typedef struct response{
  full required;
  int status;//0 available
            //1-occupied
 
 }response;



//To display the information of structure full
void Display(full user){
printf("\n\n\n");
if (user.r.type==0)printf("\n\nThe Type of Service Requested is Booking\n");
if (user.r.type==1)printf("\n\nThe Type of Service Requested is Cancelling\n");
if (user.r.type==2)printf("\n\nThe Type of Service Requested is Rescheduling\n");


user.r.hall?printf("Seminar Hall : New Seminar Hall\n"):printf("Seminar Hall : Old Seminar hall\n");
user.r.B_day?printf("Booking Type : Multiple Day\n"):printf("Booking Type : Single Day\n");

if (user.r.B_day==0){
printf("Date Booked %d-%d-%d\n",user.r.booking_days.one_day.Date.tm_mday,user.r.booking_days.one_day.Date.tm_mon,user.r.booking_days.one_day.Date.tm_year);
printf("No of Slots Booked : %d\n",user.r.booking_days.one_day.no_of_slots);
printf("Event Starts from Slot Number : %d\n",user.r.booking_days.one_day.first_slot_number);
}
//else for multiple days

printf("Booked by : %s",user.contacts.name);
printf("Contact Number : %s\n\n",user.contacts.contact);

return;
}




void disp(){

printf("\t\tWelcome To Seminar Booking Hall\n\n");
}


//Current time
char *Time(){
 time_t t;
 time(&t);
 return ctime(&t);
 } 

//Error printing
void
error (const char *msg)
{
  perror (msg);			//inbuilt function that outputs with stderr with error number
  exit (1);
}


//Main program
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

request req;
det con;
  printf
    ("\t\tWELCOME TO SEMINAR HALL BOOKING APP\n\tChoose the following\n1.Booking\n2.Cancelling\n3.Rescheduling\n4.Exit\n\n\nchoice? :");
  scan (&req.type);
  if (req.type==4)exit(0);
  //while ((getchar()) != '\n');
  switch (req.type -= 1)
  {
case 0:
    {
      printf
	("\tYou Have chosen For Booking the Hall\nWhich Seminar Hall do You want to Book\n1.Old Seminar Hall\n2.New Seminar Hall\n\nChoice? : ");
      scan (&req.hall);
//    while ((getchar()) != '\n');
      req.hall -= 1;
      req.
	hall ? printf ("You have chosen New seminar Hall\n") :
	printf ("You Have chosen Old seminar hall\n");
      printf ("Booking type \n1.Single Day\n2.More than one day\n");
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
	}
//For single day
      printf
	("Enter the Date You want to Book in the Format <DD-MM-YYYY> : ");
      char take[11];
      fgets (take,sizeof(take), stdin);
      sscanf (take, "%2d-%2d-%4d", &req.booking_days.one_day.Date.tm_mday,
	      &req.booking_days.one_day.Date.tm_mon,
	      &req.booking_days.one_day.Date.tm_year);
      printf ("How many Number of Slots?(max 7) : ");
      scan (&req.booking_days.one_day.no_of_slots);
    //  while ((getchar()) != '\n');
      printf
	("Select which Slot to start from\n1.9AM-10AM\n2.10AM-11AM\n3.11AM-12PM\n4.12PM-1PM\n5.1PM-2PM\n6.2PM-3PM\n7.3PM-4PM\n\nChoice : ");
      scan ( &req.booking_days.one_day.first_slot_number);
      //while ((getchar()) != '\n');

//Perform Checking
//printf("Wait for the server...\nChecking for availbality\n\n");

//IF available Take contact details


      printf ("Please Provide your Contact Details\nYour Name : ");
      fgets (con.name, sizeof (con.name), stdin);
      printf ("Contact Number : ");
      fgets (con.contact, sizeof (con.contact), stdin);

      full info = {req,con};
      //Send the details to Server for checking availabality
      n=send(cli_sock,(void *)&info,sizeof(info),0);
      if(n<0)error("Error sending Details\nTry again");
      Display(info);
	printf("\nDetails Sent to Server\nChecking for availibality\n");
	response res;
	n=recv(cli_sock,(response *)&res,sizeof(res),0);
	if(n<0)error("Error Receieving Status\nTry again");
	if(res.status==0)
	{
		printf("Slot(s) Availible\n\n");
	}
	else
	{
		printf("Slot Not Availible\n Details");
		Display(res.required);
	}
//Completed
      //printf ("Booking Successfull\n");
      break;
    }
case 1:
    printf ("Service Not Available");
    exit (1);
default:
    printf ("Service Not Available");
    exit (1);
  }


//close(cli_sock);
printf("Client socket Closed\n\n");
return 0;

}
