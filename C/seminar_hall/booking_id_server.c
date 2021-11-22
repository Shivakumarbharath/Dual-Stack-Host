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
  char booking_id[17];
}slot;

typedef struct date
{ struct slot slots[7];
  int   work_day;
}date;

typedef struct month
{ struct date date1[31];  // need to take care 30 days and Feb
int mon;
int year;
}month;

month h1_cur_month,h1_nxt_month,h2_cur_month,h2_nxt_month;

void Initialise(month *cur_month)
{
	for(int i=0;i<31;i++)
	{
		cur_month->date1[i].work_day=i;
		for(int j=0;j<7;j++)
		{
			cur_month->date1[i].slots[j].booking_status=0;
			//printf("%d %d\t%d\n",i,j,cur_month->date1[i].slots[j].booking_status);
		}

	}



return;
}

//To create a booking id for the slot
//3110200015-date/month/year/hall/slot of booking
char* booking_id(exchange details,int month,int year){
char id_i[17];
/*
id_i=(details.event_day+1)*100000000;
id_i+=(month*1000000);
id_i+=(year*100);
id_i+=(details.hall+1)*10+(details.slot_no+1);
*/
char day[3];
sprintf(day,"%02d",details.event_day+1);
strcpy(id_i,day);
strcat(id_i,"-");
char mon[3];
sprintf(mon,"%02d",month);
strcat(id_i,mon);
strcat(id_i,"-");
char y[5];
sprintf(y,"%04d",year);
strcat(id_i,y);
strcat(id_i,"-");
char ele[2];
sprintf(ele,"%d",details.hall+1);
strcat(id_i,ele);
strcat(id_i,"-");
char elee[2];
sprintf(elee,"%d",details.slot_no+1);
strcat(id_i,elee);
char *ret=id_i;
return ret;
}


void Display_slot(slot info){

printf("\n\tBooking Details\n\n");
printf("Name : %s\n",info.name);
printf("Contact Number : %s\n\n",info.contact);
int day,month,year,hall,slot;
/*day=info.booking_id/100000000;
month=(info.booking_id/1000000)%100;
year=(info.booking_id/100)%10000;
hall=(info.booking_id%100)/10;
slot=info.booking_id%10;
*/
sscanf(info.booking_id,"%02d-%02d-%04d-%d-%d",&day,&month,&year,&hall,&slot);
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
printf("Booking Id : %s\n\n",info.booking_id);
return;
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

printf("\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\tSeminar Booking Hall Application Server\n\n");
printf("\n\nInitialise the Structure?\n1.Yes\n2.no\n Choice? : ");
int init;
scanf("%d",&init);
if(init==1)
{
Initialise(&h1_cur_month);
Initialise(&h1_nxt_month);
Initialise(&h2_cur_month);
Initialise(&h2_nxt_month);
int m,y;
printf("\n\nINITIALISE MONTH\nWhich month is this? :(1-12)");
scanf("%d",&m);
h1_cur_month.mon=m;
h2_cur_month.mon=m;
h1_nxt_month.mon=m+1;
h2_nxt_month.mon=m+1;
printf("\nWhich Year ? :");
scanf("%d",&y);
h1_cur_month.year=y;
h1_nxt_month.year=y;
h2_cur_month.year=y;
h2_nxt_month.year=y;
}
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
Printf("Socket Created Successfully");
//clear the contents in the declared variable
bzero((char *)&serv_addr,sizeof(serv_addr));

//Load the port number and server address into the structure
serv_addr.sin_family=AF_INET;
serv_addr.sin_port=htons(portno);
serv_addr.sin_addr.s_addr=INADDR_ANY;

//Bind the socket to address of the server and check if successfull
if(bind(ser_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)error("Error while Binding the socket");
Printf("Binding Successfull");

//Listen by the server

listen(ser_sock,5);
Printf("Listening!!!");


signal(SIGINT, intHandler);//using Ctrl+c
while(keepRunning)
{

//accept from client
client_len=sizeof(client_addr);
cli_sock=accept(ser_sock,(struct sockaddr *)&client_addr,&client_len);


if(cli_sock<0)error("Error while Accepting the connection");
Printf("Connection Accepted");
strcpy(ip,inet_ntoa(client_addr.sin_addr));
printf("IPv4 Address : %s\t",ip);
printf("Port number %d\n\n\n\n",client_addr.sin_port);
//Read from client
int n;
/*msg Recv;
bzero(buffer,sizeof(buffer));
n=recv(cli_sock,(msg *)&Recv,sizeof(Recv),0);
if(n<0)error("Error while Communication");
printf("Server Received from client %s : %s\t contact %s",ip,Recv.name,Recv.contact);
//print time recieved
printf("Recieced Time =%s\n\n",Time());
*/

exchange resp;
slot con;


n=recv(cli_sock,(exchange *)&resp,sizeof(resp),0);
if(n<0)error("Error Receieving Status\nTry again");
//if the selection is booking perform checking and if free increament for booking id
switch(resp.type)
{	
	case 0:
	{
	if(resp.hall==0 && resp.month_type==0)
		{
		if(h1_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status==0)
			{
			//make booking id and send the slot
			printf("%d",h1_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status);
			strcpy(h1_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_id,booking_id(resp,h1_cur_month.mon,h1_cur_month.year));
			//printf("Going into the if statment");
			n=send(cli_sock,(void *)&h1_cur_month.date1[resp.event_day].slots[resp.slot_no],sizeof(h1_cur_month.date1[resp.event_day].slots[resp.slot_no]),0);
	        	if(n<0){printf("Error sending Details\nTry again");close(cli_sock);}
		

		        slot recieve;
                	 n=recv(cli_sock,(slot *)&recieve,sizeof(recieve),0);
                	 h1_cur_month.date1[resp.event_day].slots[resp.slot_no]=recieve;
                 	//change the status
                 	h1_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status=1;
			printf("%d",h1_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status);
			Display_slot(recieve);Printf("Booking Successfull!!");

			}
		else
			{
			//send that slot
			 n=send(cli_sock,(void *)&h1_cur_month.date1[resp.event_day].slots[resp.slot_no],sizeof(h1_cur_month.date1[resp.event_day].slots[resp.slot_no]),0);
                	 if(n<0){printf("Error sending Details\nTry again");close(cli_sock);}
                	 printf("\n\nBooking Already Done\n\nDetails\n");
                	 Display_slot(h1_cur_month.date1[resp.event_day].slots[resp.slot_no]);
                	 Printf("Booking Failed!!");
			}
		}
	else if(resp.hall==0 && resp.month_type==1)
		{
        	 if(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_status==0)
        	         { 
        	         //make booking id and send the slot
        	        
                	 strcpy(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_id,booking_id(resp,h1_nxt_month.mon,h1_nxt_month.year));
                 	n=send(cli_sock,(void *)&h1_nxt_month.date1[resp.event_day].slots[resp.slot_no],sizeof(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no]),0);
                 	if(n<0){printf("Error sending Details\nTry again");close(cli_sock);}
                 	slot recieve;
                 	//Change the status
                	
                 	n=recv(cli_sock,(slot *)&recieve,sizeof(recieve),0);
                 	h1_nxt_month.date1[resp.event_day].slots[resp.slot_no]=recieve;
			h1_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_status=1;
			Display_slot(recieve);Printf("Booking Successfull!!");


               		}
            	else
                 	{
                 	//send that slot
                 	 n=send(cli_sock,(void *)&h1_nxt_month.date1[resp.event_day].slots[resp.slot_no],sizeof(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no]),0);
                 	 if(n<0){printf("Error sending Details\nTry again");close(cli_sock);}
                 	 printf("\n\nBooking Already Done\n\nDetails\n");
                 	 Display_slot(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no]);
			Printf("Booking Failed!!");
                 	}

		}
	else if(resp.hall==1 && resp.month_type==0)
		
	        {
	          if(h2_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status==0)
	                  { 
	                  //make booking id and send the slot
	            
	                  strcpy(h2_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_id,booking_id(resp,h2_cur_month.mon,h2_cur_month.year));
	                  n=send(cli_sock,(void *)&h2_cur_month.date1[resp.event_day].slots[resp.slot_no],sizeof(h2_cur_month.date1[resp.event_day].slots[resp.slot_no]),0);
	                  if(n<0){printf("Error sending Details\nTry again");close(cli_sock);}
			slot recieve;
	
		
			n=recv(cli_sock,(slot *)&recieve,sizeof(recieve),0);
			h2_cur_month.date1[resp.event_day].slots[resp.slot_no]=recieve;
			//change the status
			h2_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status=1;
			Display_slot(recieve);Printf("Booking Successfull!!");


        	        }
        	     else
        	          {
        	          //send that slot
        	           n=send(cli_sock,(void *)&h2_cur_month.date1[resp.event_day].slots[resp.slot_no],sizeof(h2_cur_month.date1[resp.event_day].slots[resp.slot_no]),0);
        	           if(n<0){printf("Error sending Details\nTry again");close(cli_sock);}
        	           printf("\n\nBooking Already Done\n\nDetails\n");
        	           Display_slot(h2_cur_month.date1[resp.event_day].slots[resp.slot_no]);
			   Printf("Booking Failed!!");
        	          }
	
		}
	else if(resp.hall==1 && resp.month_type==1)
		
        	{
          	if(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_status==0)
          	        { 
          	        //make booking id and send the slot
          	        
          	        strcpy(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_id,booking_id(resp,h2_nxt_month.mon,h2_nxt_month.year));
          	        n=send(cli_sock,(void *)&h2_nxt_month.date1[resp.event_day].slots[resp.slot_no],sizeof(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no]),0);
          	        if(n<0){printf("Error sending Details\nTry again");close(cli_sock);}
          	        slot recieve;
          	       n=recv(cli_sock,(slot *)&recieve,sizeof(recieve),0);
          	       h2_nxt_month.date1[resp.event_day].slots[resp.slot_no]=recieve;
          	       //change the status
			h2_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_status=1;
			Display_slot(recieve);Printf("Booking Successfull!!");


        	        }
        	     else
                  	{
                  	//send that slot
                   	n=send(cli_sock,(void *)&h2_nxt_month.date1[resp.event_day].slots[resp.slot_no],sizeof(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no]),0);
                        	              if(n<0){printf("Error sending Details\nTry again");close(cli_sock);}
                        printf("\n\nBooking Already Done\n\nDetails\n");
                        Display_slot(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no]);
			Printf("Booking Failed!!");
                        }
                  }
	
break;	}
	
	
case 1:
	{
	
		if(resp.hall==0 && resp.month_type==0)
		{
		if(h1_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status==1)
			{
			Display_slot(h1_cur_month.date1[resp.event_day].slots[resp.slot_no]);

			//make booking id=0
			
			strcpy(h1_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_id,"");
			
                 	//change the status
                 	h1_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status=0;

			Printf("Booking Cancelled!!");
                 	//remove the name 
                 	strcpy(h1_cur_month.date1[resp.event_day].slots[resp.slot_no].name,"");
                 	strcpy(h1_cur_month.date1[resp.event_day].slots[resp.slot_no].contact,"");
			char cancel_report[]={"Booking Cancelled!!"};
			n=send(cli_sock,&cancel_report,sizeof(cancel_report),0);
			if(n<0){printf("Error sending Cancellation Report\nTry again");close(cli_sock);}
			
                 	//Send a string as Booking canceled
                 	

			}
		else
			{
			Printf("Cancellation Failed!!");

			//send as This slot Not yet Booked check Your slot again
			char cancel_report[]={"\n\nThis Slot has not been Booked Please Check the Slot Again\n"};
                         n=send(cli_sock,&cancel_report,sizeof(cancel_report),0);
                         if(n<0){printf("Error sending Cancellation Report\nTry again");close(cli_sock);}

			}
		}
	else if(resp.hall==0 && resp.month_type==1)
		{
        	 if(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_status==1)
        	         { 
        	         //make booking id and send the slot
        	        Display_slot(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no]);
                	 strcpy(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_id,"");
                 	
			h1_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_status=0;
			Printf("Booking Cancelled!!");

			strcpy(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no].name,"");
			strcpy(h1_nxt_month.date1[resp.event_day].slots[resp.slot_no].contact,"");
			char cancel_report[]={"Booking Cancelled!!"};
                         n=send(cli_sock,&cancel_report,sizeof(cancel_report),0);
                         if(n<0){printf("Error sending Cancellation Report\nTry again");close(cli_sock);}

               		}
            	else
                 	{Printf("Cancellation Failed!!");

                         //send as This slot Not yet Booked check Your slot again
                         char cancel_report[]={"\n\nThis Slot has not been Booked Please Check the Slot Again\n"};
                          n=send(cli_sock,&cancel_report,sizeof(cancel_report),0);
                          if(n<0){printf("Error sending Cancellation Report\nTry again");close(cli_sock);}

                        }


		}
	else if(resp.hall==1 && resp.month_type==0)
		
	        {
	          if(h2_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status==1)
	                  { 
	                  //make booking id and send the slot
	            	Display_slot(h2_cur_month.date1[resp.event_day].slots[resp.slot_no]);
	                  strcpy(h2_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_id,"");
	                 //change the status
                         h2_cur_month.date1[resp.event_day].slots[resp.slot_no].booking_status=0;
                         //remove the name 
			Printf("Booking Cancelled!!");

                         strcpy(h2_cur_month.date1[resp.event_day].slots[resp.slot_no].name,"");
                         strcpy(h2_cur_month.date1[resp.event_day].slots[resp.slot_no].contact,"");
                         char cancel_report[]={"Booking Cancelled!!"};
                         n=send(cli_sock,&cancel_report,sizeof(cancel_report),0);
                         if(n<0){printf("Error sending Cancellation Report\nTry again");close(cli_sock);}
 
        	        }
        	     else
        	          {
        	          Printf("Cancellation Failed!!");

                         //send as This slot Not yet Booked check Your slot again
                         char cancel_report[]={"\n\nThis Slot has not been Booked Please Check the Slot Again\n"};
                          n=send(cli_sock,&cancel_report,sizeof(cancel_report),0);
                          if(n<0){printf("Error sending Cancellation Report\nTry again");close(cli_sock);}
                         }

	
		}
	else if(resp.hall==1 && resp.month_type==1)
		
        	{
          	if(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_status==1)
          	        { 
          	        Display_slot(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no]);
          	        //make booking id =0
          	        strcpy(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_id,"");
          	        //change the status
                         h2_nxt_month.date1[resp.event_day].slots[resp.slot_no].booking_status=0;
			Printf("Booking Cancelled!!");

                         //remove the name 
                         strcpy(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no].name,"");
                         strcpy(h2_nxt_month.date1[resp.event_day].slots[resp.slot_no].contact,"");
                         char cancel_report[]={"Booking Cancelled!!"};
                         n=send(cli_sock,&cancel_report,sizeof(cancel_report),0);
                         if(n<0){printf("Error sending Cancellation Report\nTry again");close(cli_sock);}

        	        }
        	     else
                  	  {
                          //send as This slot Not yet Booked check Your slot again
                          Printf("Cancellation Failed!!");
                          char cancel_report[]={"\n\nThis Slot has not been Booked Please Check the Slot Again\n"};
                           n=send(cli_sock,&cancel_report,sizeof(cancel_report),0);
                           if(n<0){printf("Error sending Cancellation Report\nTry again");close(cli_sock);}
                          }

		break;}

	}
}

close(cli_sock);
Printf("Client Socket closed");


}
close(ser_sock);
Printf("Server socket closed");
return 0;


}
