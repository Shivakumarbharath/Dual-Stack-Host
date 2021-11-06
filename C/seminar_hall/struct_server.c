#include <stdio.h>//To include standard input and output functions
#include <stdlib.h>//macros and functions
#include <sys/types.h>//To include number of datatypes for system calls
#include <sys/socket.h>//Functions needed to create,bind,listen,accept
#include <netinet/in.h>//to include the constants and structures for domain address
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>//to use inet ntoa(struct in_addr)to convert struct to ip address
#include <time.h>



void scan(int *p){
scanf("%d",p);
while ((getchar()) != '\n');
return;
}
struct s_day
{
  struct tm Date;		//Date of Booking
  int no_of_slots;
  int first_slot_number;
/*if more than one slot than booking can be done for continues slots only*/
};


struct m_day
{
  struct tm dates[6];
  int no_of_slots_each_day;
  int first_slot_number;
};





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



typedef struct details
{
  char name[20];
  char contact[11];
} det;


   typedef struct full
    {
      request r;
      det contacts;
    }full; 

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



//Function to display the introduction
void disp(){

printf("\t\tServer using IPv4 Protocol on Dual Stack Host\n\n");
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
//accept from client
client_len=sizeof(client_addr);
cli_sock=accept(ser_sock,(struct sockaddr *)&client_addr,&client_len);


if(cli_sock<0)error("Error while Accepting the connection");
printf("Connection Accepted\n");
strcpy(ip,inet_ntoa(client_addr.sin_addr));
printf("ipv4 Address : %s\t",ip);
printf("Port number %d\n\n\n\n",client_addr.sin_port);
//Read from client
int n;
full Recv;
n=recv(cli_sock,(full *)&Recv,sizeof(Recv),0);
if(n<0)error("Error while Recieving information");
Display(Recv);

//Perform Checking

//Send Response


//If availible
//Add to database
//send as Booked


//If not availible
//Continue


//Print time sent
printf("Sent time= %s\n\n",Time());
close(cli_sock);
close(ser_sock);

return 0;

}
