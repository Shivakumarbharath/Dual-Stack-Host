#include <stdio.h>//To include standard input and output functions
#include <stdlib.h>//macros and functions
#include <sys/types.h>//To include number of datatypes for system calls
#include <sys/socket.h>//Functions needed to create,bind,listen,accept
#include <netinet/in.h>//to include the constants and structures for domain address
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>//to use inet ntoa(struct in_addr)to convert struct to ip address
#include <time.h>
#include <signal.h>

//TO use CTRL+c to stop the while loop
static volatile int keepRunning = 1;
void intHandler(int dummy) {
    keepRunning = 0;
}

//To use fgets after scanf 
void scan(int *p){
scanf("%d",p);
while ((getchar()) != '\n');
return;
}

//To store the information for single day booking
struct s_day
{
  struct tm Date;		//Date of Booking
  int no_of_slots;
  int first_slot_number;
/*if more than one slot than booking can be done for continues slots only*/
};

//To store the information for multiple day booking
struct m_day
{
  struct tm dates[6];
  int no_of_slots_each_day;
  int first_slot_number;
};




//Information related to booking
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


//Person who is Booking
typedef struct details
{
  char name[20];
  char contact[11];
} det;

//Struct with both request and details
   typedef struct full
    {
      request r;
      det contacts;
    }full; 

full info[31];


//Struct to send the response
typedef struct response{
full required;
int status;//0 available
	  //1-occupied

}response;


//To display the full information
void Display(full user){
printf("\n\n\n");
if (user.r.type==0)printf("The Type of Service Requested is Booking\n");
if (user.r.type==1)printf("The Type of Service Requested is Cancelling\n");
if (user.r.type==2)printf("The Type of Service Requested is Rescheduling\n");


user.r.hall?printf("Seminar Hall : New Seminar Hall\n"):printf("Seminar Hall : Old Seminar hall\n");
user.r.B_day?printf("Booking Type : Multiple Day\n"):printf("Booking Type : Single Day\n");

if (user.r.B_day==0){
printf("Date Booked %d-%d-%d\n",user.r.booking_days.one_day.Date.tm_mday,user.r.booking_days.one_day.Date.tm_mon,user.r.booking_days.one_day.Date.tm_year);
printf("No of Slots Booked : %d\n",user.r.booking_days.one_day.no_of_slots);
printf("Event Starts from Slot Number : %d\n",user.r.booking_days.one_day.first_slot_number);
}
//else for multiple days

printf("Booked by : %s",user.contacts.name);
printf("Contact Number : %s\n",user.contacts.contact);

return;
}

int One_day_compare(struct s_day db,struct s_day rq )
{

	if(db.Date.tm_mday!=rq.Date.tm_mday)return 0;
	else
	{
		if(db.Date.tm_mon!=rq.Date.tm_mon)return 0;
		else
		{
/*			if(db.no_of_slots==1)
			{
				if(db.first_slot_number!=rq.first_slot_number)return 0;
				else return 1;
			}
			else if(db.first_slot_number==rq.first_slot_number)return 1;
*/			
			{
				for(int k=db.first_slot_number;k<db.first_slot_number+db.no_of_slots-1;k++){
				for(int l=rq.first_slot_number;l<rq.first_slot_number+rq.no_of_slots-1;l++)
				if(k==l)return 1;

			}

		}
	}


}
return 0;
}


response Compare(full rqst,int id){


if(id==0)
{
response send={rqst,0};
return send;

}

for(int i=0;i<id;i++)
{
	full h=info[i];
	if(rqst.r.hall!=h.r.hall)continue;
	if(h.r.B_day==0)
	{
		if(One_day_compare(h.r.booking_days.one_day,rqst.r.booking_days.one_day)==0)
		{
			response send={rqst,0};
			return send;
		}
		else
		{
			response send={h,1};
			return send;
		}


	}

}


}

//Function to display the introduction
void disp(){

printf("\t\tServer using IPv4 Protocol on Dual Stack Host For Seminar Hall Booking \n\n");
}



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
int main(){
//declare variable for server socket,accepted client connection and portnumber
disp();

int ser_sock,cli_sock,portno;
portno=5000;
//declare varble to send and receieve message
char buffer[255];
//To store the ip address of client
char ip[15];
//structure to store server address and client address
struct sockaddr_in serv_addr,client_addr;//from netinet/in
socklen_t client_len;
//create server socket
ser_sock=socket(AF_INET,SOCK_STREAM,0);
//Check for successfull socket creation
if(ser_sock<0){error("Error in Socket creation");}
printf("Socket Created Successfully\n\n");
//clear the contents in the declared variable
bzero((char *)&serv_addr,sizeof(serv_addr));

//Load the port number and server address into the structure
serv_addr.sin_family=AF_INET;
serv_addr.sin_port=htons(portno);
serv_addr.sin_addr.s_addr=INADDR_ANY;

//Bind the socket to address of the server and check if successfull
if(bind(ser_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)error("Error while Binding the socket");
printf("Binding Successfull\n\n");

//Listen by the server

listen(ser_sock,5);
printf("Listening!!!\n\n");

//To remember the sessions conducted and total number of Bookings
int id=-1;
int session=1;

signal(SIGINT, intHandler);//using Ctrl+c
while(keepRunning)
{
//accept from client
	printf("\nWaiting to Accept...\n");
	client_len=sizeof(client_addr);
	sleep(2);
	if(keepRunning==0)break;
	cli_sock=accept(ser_sock,(struct sockaddr *)&client_addr,&client_len);


	if(cli_sock<0)error("Error while Accepting the connection");

//Connected
	printf("\n\nConnection Accepted\n");
	strcpy(ip,inet_ntoa(client_addr.sin_addr));
	printf("ipv4 Address : %s\t",ip);
	printf("Port number %d\n\n\n\n",client_addr.sin_port);
//Read from client
	int n;
	full Recv;
	n=recv(cli_sock,(full *)&Recv,sizeof(Recv),0);
	if(n<0)error("Error while Recieving information");
	Display(Recv);
	if(Recv.r.type==0)id++;
//Perform Checking
	response result=Compare(Recv,id);
//Send Response
	n=send(cli_sock,(void *)&result,sizeof(result),0);

//If availible
	if(result.status==0)
	{
//Add to database
	info[id]=Recv;
	Display(info[id]);
	printf("\nBooked Succesfully\n\n");
	}

//send as Booked


//If not availible
	else{printf("\nSlot Already taken\n\n");}
//Continue

//Print time sent
	printf("Sent time= %s\n\n",Time());
	close(cli_sock);
	printf("Client Socket Closed\n");
}
//Close Server socket
close(ser_sock);
printf("Server Socket Closed\n");
return 0;

}
