#include <stdio.h>		//To include standard input and output functions
#include <stdlib.h>		//macros and functions
#include <sys/types.h>		//To include number of datatypes for system calls
#include <sys/socket.h>		//Functions needed to create,bind,listen,accept
#include <netinet/in.h>		//to include the constants and structures for domain address
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>		//to use inet ntoa(struct in_addr)to convert struct to ip address
#include <time.h>
#include <signal.h>


//Provide colours
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

//TO use CTRL+c to stop the while loop
static volatile int keepRunning = 1;
void
intHandler (int dummy)
{
  keepRunning = 0;

}

void
Printf_green(const char *msg)
{
  printf
    (GRN"\n\n\n\t\t%s\n\n\n\n"RESET,
     msg);
}


void
Printf_red(const char *msg)
{
  printf
    (RED"\n\n\n\t\t%s\n\n\n\n"RESET,
     msg);
}


typedef struct exchange
{
  int type;			/*0 - booking 
				   1 - cancelling by date
				   2 - cancelling by id
				   3.Display Status */



  int hall;			//Old Seminar hall-0(take H1)
  //New Seminar hall 1(take h2)
  int month_type;		//current month-0
  //next month-1
  int event_day;		//month.date1[event_day-1]

  int slot_no;			//month.date1[event_day-1].slots[slot_no]

  char id[15];			//booking id for cancellation

} exchange;


typedef struct slot
{
  int booking_status;
  char name[50];
  char contact[11];
  char booking_id[17];
  char purpose[20];
} slot;

typedef struct date
{
  struct slot slots[7];
  int work_day;
  unsigned char day_status;
} date;

typedef struct month
{
  struct date date1[31];	// need to take care 30 days and Feb
  int mon;
  int year;
} month;

month h1_cur_month, h1_nxt_month, h2_cur_month, h2_nxt_month;
char h1_cur[]={"h1_cur.dat"};
char h1_nxt[]={"h1_nxt.dat"};
char h2_cur[]={"h2_cur.dat"};
char h2_nxt[]={"h2_nxt.dat"};


typedef struct date_status
{

  unsigned char status[31];
  int month;
  int year;
} date_status;

void
Char_to_Binary (unsigned char s)
{
  int binary[8];
  for (int n = 0; n < 8; n++)
    binary[7 - n] = (s >> n) & 1;
  /* print result */
  for (int n = 1; n < 8; n++)
    printf ("%d\t", binary[n]);
  printf ("\n");
}

date_status check_status (char *fname)
{
month mon;
  FILE *fp;
 fp = fopen (fname, "rb");
 fread (&mon, sizeof (mon), 1, fp);
 fclose (fp);

  date_status status;
  int *p;
  status.month = mon.mon;
  status.year = mon.year;
  for (int i = 0; i < 31; i++)
    {
      status.status[i] = mon.date1[i].day_status;
    }
  return status;
}



unsigned char
Slot_to_Binary (int no)
{

  unsigned char result;
  switch (no)

    {
    case 1:
      result = 64;
      break;
    case 2:
      result = 32;
      break;
    case 3:
      result = 16;
      break;
    case 4:
      result = 8;
      break;
    case 5:
      result = 4;
      break;
    case 6:
      result = 2;
      break;
    case 7:
      result = 1;
      break;
    default:
      result = 0;
      break;
    }
  return result;
}


void
Initialise (char *fname,int m,int y)
{
month cur_month;
cur_month.mon = m;
cur_month.year=y;
  for (int i = 0; i < 31; i++)
    {
      cur_month.date1[i].work_day = i;
      cur_month.date1[i].day_status = 0;
      for (int j = 0; j < 7; j++)
	{
	  cur_month.date1[i].slots[j].booking_status = 0;
	}

    }
//Save it in a file
FILE *fp;
fp = fopen (fname, "wb");
fwrite (&cur_month, sizeof (cur_month), 1, fp);
fclose (fp);



  return;
}

//To create a booking id for the slot
//3110200015-date/month/year/hall/slot of booking
char *
booking_id (exchange details, int month, int year)
{
  char id_i[17];

  char day[3];
  sprintf (day, "%02d", details.event_day + 1);
  strcpy (id_i, day);
  strcat (id_i, "-");
  char mon[3];
  sprintf (mon, "%02d", month);
  strcat (id_i, mon);
  strcat (id_i, "-");
  char y[5];
  sprintf (y, "%04d", year);
  strcat (id_i, y);
  strcat (id_i, "-");
  char ele[2];
  sprintf (ele, "%d", details.hall + 1);
  strcat (id_i, ele);
  strcat (id_i, "-");
  char elee[2];
  sprintf (elee, "%d", details.slot_no + 1);
  strcat (id_i, elee);
  char *ret = id_i;
 printf("%s",id_i);
  return ret;
}


void
Display_slot (slot info)
{

  printf (YEL"\n\tBooking Details\n\n"RESET);
  printf (YEL"Name : "BLU"%s\n"RESET, info.name);
  printf (YEL"Contact Number :"BLU" %s\n\n"RESET, info.contact);
  int day, month, year, hall, slot;
/*day=info.booking_id/100000000;
month=(info.booking_id/1000000)%100;
year=(info.booking_id/100)%10000;
hall=(info.booking_id%100)/10;
slot=info.booking_id%10;
*/
  sscanf (info.booking_id, "%02d-%02d-%04d-%d-%d", &day, &month, &year, &hall,
	  &slot);
  printf (YEL"Date :"BLU" %02d/%02d/%d\n\n"RESET, day, month, year);
  if (hall == 1)
    printf (YEL"Seminar Hall : "BLU"Old Seminar Hall\n\n"RESET);
  if (hall == 2)
    printf (YEL"Seminar Hall : "BLU"New Seminar Hall\n\n"RESET);
  char time[12];
  switch (slot)
    {
    case 1:
      strcpy (time, "9AM - 10AM");
      break;
    case 2:
      strcpy (time, "10AM - 11AM");
      break;
    case 3:
      strcpy (time, "11AM - 12PM");
      break;
    case 4:
      strcpy (time, "12PM - 1PM");
      break;
    case 5:
      strcpy (time, "1PM - 2PM");
      break;
    case 6:
      strcpy (time, "2PM - 3PM");
      break;
    case 7:
      strcpy (time, "3PM - 4PM");
      break;
    }
  printf (YEL"Slot Time : "BLU"%s\n\n"RESET, time);
  printf (YEL"Booking Id :"BLU" %s\n\n"RESET, info.booking_id);
  printf(YEL"Purpose :"BLU" %s\n\n"RESET, info.purpose);
  return;
}



//Funtion to display error if any event fails
char *
Time ()
{
  time_t t;
  time (&t);
  return ctime (&t);
}

void
error (const char *msg)
{
  perror (msg);			// inbuilt function that outputs as an error
  exit (1);
}


void
display_status (date_status status)
{
  printf
    (CYN"Date\t     slot-1  slot-2  slot-3  slot-4  slot-5  slot-6  slot-7\n"RESET);

  for (int i = 0; i < 31; i++)
  if(status.status[i]!=0)
    {
      printf (YEL"\n\n%d-%02d-%04d\t"RESET, i + 1, status.month, status.year);
      Char_to_Binary (status.status[i]);
    }
}


//Booking Slot Function
void
Book (int cli_sock, exchange resp, char *fname)
{
  int n;
month h1;
    FILE *fp;
 fp = fopen (fname, "rb");
 fread (&h1, sizeof (h1), 1, fp);
 fclose (fp);

  if (h1.date1[resp.event_day].slots[resp.slot_no].booking_status == 0)
    {
      //make booking id and send the slot
      //printf("%d",h1.date1[resp.event_day].slots[resp.slot_no].booking_status);
      strcpy (h1.date1[resp.event_day].slots[resp.slot_no].booking_id,
	      booking_id (resp, h1.mon, h1.year));
      h1.date1[resp.event_day].day_status |=
	Slot_to_Binary (resp.slot_no + 1);
      //printf("Going into the if statment");
      n =
	send (cli_sock,
	      (void *) &h1.date1[resp.event_day].slots[resp.slot_no],
	      sizeof (h1.date1[resp.event_day].slots[resp.slot_no]), 0);
      if (n < 0)
	{
	  printf (RED"Error sending Details\nTry again"RESET);
	  close (cli_sock);
	}


      slot recieve;
      n = recv (cli_sock, (slot *) & recieve, sizeof (recieve), 0);
	if (n < 0)
        {
          printf (RED"Error Recieving Details\nTry again"RESET);
          close (cli_sock);
        }
      h1.date1[resp.event_day].slots[resp.slot_no] = recieve;
      //change the status
      h1.date1[resp.event_day].slots[resp.slot_no].booking_status = 1;
      printf ("%d",
	      h1.date1[resp.event_day].slots[resp.slot_no].booking_status);
      Display_slot (recieve);
      Printf_green ("Booking Successfull!!");
      Printf_green ("Display Status");
      
    }
  else
    {
      //send that slot
      n =
	send (cli_sock,
	      (void *) &h1.date1[resp.event_day].slots[resp.slot_no],
	      sizeof (h1.date1[resp.event_day].slots[resp.slot_no]), 0);
      if (n < 0)
	{
	  printf ("Error sending Details\nTry again");
	  close (cli_sock);
	}
    
      printf (RED"\n\nBooking Already Done\n\nDetails\n"RESET);
      Display_slot (h1.date1[resp.event_day].slots[resp.slot_no]);
      Printf_red ("Booking Failed!!");
    }
//save it in a file
fp = fopen (fname, "wb");
  fwrite (&h1, sizeof (h1), 1, fp);
 fclose (fp);
date_status stat = check_status (fname);
      display_status (stat);



}

void
Cancel_date (int cli_sock, exchange resp, char *fname)
{
  int n;
    month h1;
  FILE *fp;
  fp = fopen (fname, "rb");
  fread (&h1, sizeof (h1), 1, fp);
  fclose (fp);
//date_status stat=check_status(h1);

  if (h1.date1[resp.event_day].slots[resp.slot_no].booking_status == 1)
    {
      Display_slot (h1.date1[resp.event_day].slots[resp.slot_no]);

      //make booking id=0

      strcpy (h1.date1[resp.event_day].slots[resp.slot_no].booking_id, "");
      strcpy (h1.date1[resp.event_day].slots[resp.slot_no].purpose, "");

      //change the status
      h1.date1[resp.event_day].slots[resp.slot_no].booking_status = 0;
     // printf ("line-273 %d", h1->date1[resp.event_day].day_status);
      h1.date1[resp.event_day].day_status &=
	(~Slot_to_Binary (resp.slot_no + 1));
      //printf ("line-275 %d", h1->date1[resp.event_day].day_status);
      //Printf ("Booking Cancelled!!");
      //remove the name 
      strcpy (h1.date1[resp.event_day].slots[resp.slot_no].name, "");
      strcpy (h1.date1[resp.event_day].slots[resp.slot_no].contact, "");
      char cancel_report[] = { "Booking Cancelled!!" };
      n = send (cli_sock, &cancel_report, sizeof (cancel_report), 0);
      if (n < 0)
	{
	  printf ("Error sending Cancellation Report\nTry again");
	  close (cli_sock);
	}
      //date_status stat = check_status (fname);
      //display_status (stat);
      //Send a string as Booking canceled

    }
  else
    {
      Printf_red ("Cancellation Failed!!");

      //send as This slot Not yet Booked check Your slot again
      char cancel_report[] =
	{ "\n\nThis Slot has not been Booked Please Check the Slot Again\n" };
      n = send (cli_sock, &cancel_report, sizeof (cancel_report), 0);
      if (n < 0)
	{
	  printf ("Error sending Cancellation Report\nTry again");
	  close (cli_sock);
	}

    }

//save in a file
fp = fopen (fname, "wb");
   fwrite (&h1, sizeof (h1), 1, fp);
  fclose (fp);


  return;
}

void
Cancel_id (int cli_sock, exchange resp, char *fname)
{
  int day, month1, year, hall, slot, n;
  sscanf (resp.id, "%02d-%02d-%04d-%d-%d", &day, &month1, &year, &hall, &slot);
  slot--;
  hall--;
  day--;
month h1;
FILE *fp;
   fp = fopen (fname, "rb");
   fread (&h1, sizeof (h1), 1, fp);
   fclose (fp);

  if (h1.date1[day].slots[slot].booking_status == 1)
    {
      Display_slot (h1.date1[day].slots[slot]);
      //make booking id =0
      strcpy (h1.date1[day].slots[slot].booking_id, "");
      strcpy (h1.date1[day].slots[slot].purpose, "");
      //change the status
      h1.date1[day].slots[slot].booking_status = 0;
      h1.date1[day].day_status &= (~Slot_to_Binary (slot + 1));
      Printf_red ("Booking Cancelled!!");

      //remove the name 
      strcpy (h1.date1[day].slots[slot].name, "");
      strcpy (h1.date1[day].slots[slot].contact, "");
      char cancel_report[] = { "Booking Cancelled!!" };
      n = send (cli_sock, &cancel_report, sizeof (cancel_report), 0);
      if (n < 0)
	{
	  printf (RED"Error sending Cancellation Report\nTry again"RESET);
	  close (cli_sock);
	}
    }
  else
    {
      //send as This slot Not yet Booked check Your slot again
      Printf_red ("Cancellation Failed!!");

      char cancel_report[] =
	{ BLU"\n\nThis Slot has not been Booked Please Check the Slot Again\n"RESET };
      n = send (cli_sock, &cancel_report, sizeof (cancel_report), 0);
      if (n < 0)
	{
	  printf ("Error sending Cancellation Report\nTry again");
	  close (cli_sock);
	}
    }
//save in file
fp = fopen (fname, "wb");
   fwrite (&h1, sizeof (h1), 1, fp);
   fclose (fp);


}


//Main function
int
main ()
{
//declare variable for server socket,accepted client connection and portnumber

  printf
    ("\n\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\tSeminar Booking Hall Application Server\n\n");
  printf ("\n\nInitialise the Structure?\n1.Yes (Press 1)\n2.No (Press 2)\n Choice? : ");
  int init;
  scanf ("%d", &init);
  if (init == 1)
    {
      
      month h1_cur_month, h1_nxt_month, h2_cur_month, h2_nxt_month;
      int m, y;
      printf ("\n\nINITIALISE MONTH\nWhich month is this? :(1-12)");
      scanf ("%d", &m);
      printf ("\nWhich Year ? :");
      scanf ("%d", &y);
      Initialise (h1_cur,m,y);
      Initialise (h1_nxt,m+1,y);
      Initialise (h2_cur,m,y);
      Initialise (h2_nxt,m+1,y);

    }
  int ser_sock, cli_sock, portno;
  portno = 5000;
//declare varble to send and receieve message
  char buffer[255];
//To store the ip address of client
  char ip[15];
//structure to store server address and client address
  struct sockaddr_in serv_addr, client_addr;	//from netinet/in
  socklen_t client_len;
//create server socket
  ser_sock = socket (AF_INET, SOCK_STREAM, 0);
//Check for successfull socket creation
  if (ser_sock < 0)
    {
      error ("Error in Socket creation");
    }
  Printf_green ("Socket Created Successfully");
//clear the contents in the declared variable
  bzero ((char *) &serv_addr, sizeof (serv_addr));

//Load the port number and server address into the structure
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons (portno);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

//Bind the socket to address of the server and check if successfull
  if (bind (ser_sock, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
    error ("Error while Binding the socket");
  Printf_green ("Binding Successfull");

//Listen by the server

  listen (ser_sock, 5);
  Printf_green ("Listening!!!");


  signal (SIGINT, intHandler);	//using Ctrl+c
  while (keepRunning)
    {

//accept from client
      client_len = sizeof (client_addr);
      cli_sock =
	accept (ser_sock, (struct sockaddr *) &client_addr, &client_len);


      if (cli_sock < 0)
	error ("Error while Accepting the connection");
      Printf_green ("Connection Accepted");
      strcpy (ip, inet_ntoa (client_addr.sin_addr));
      printf (YEL"IPv4 Address :"BLU" %s\t"RESET, ip);
      printf (YEL"Port number "BLU"%d\n\n\n\n"RESET, client_addr.sin_port);
//Read from client
      int n;
      int loop;
      loop=1;
	while(loop){
      exchange resp;
      slot con;


      n = recv (cli_sock, (exchange *) & resp, sizeof (resp), 0);
      if (n < 0)
	error ("Error Receieving Status\nTry again");
//if the selection is booking perform checking and if free increament for booking id
      switch (resp.type)
	{
	case 0:
	  {
	    if (resp.hall == 0 && resp.month_type == 0)
	      {
		Book (cli_sock, resp, h1_cur);
	      }
	    else if (resp.hall == 0 && resp.month_type == 1)
	      {
		Book (cli_sock, resp, h1_nxt);
	      }
	    else if (resp.hall == 1 && resp.month_type == 0)
	      {
		Book (cli_sock, resp, h2_cur);
	      }
	    else if (resp.hall == 1 && resp.month_type == 1)
	      {
		Book (cli_sock, resp, h2_nxt);
	      }

	    break;
	  }


	/*case 1:
	  {
	    if (resp.hall == 0 && resp.month_type == 0)
	      {
		Cancel_date (cli_sock, resp, &h1_cur_month);
	      }

	    else if (resp.hall == 0 && resp.month_type == 1)
	      {
		Cancel_date (cli_sock, resp, &h1_nxt_month);
	      }

	    else if (resp.hall == 1 && resp.month_type == 0)
	      {
		Cancel_date (cli_sock, resp, &h2_cur_month);
	      }

	    else if (resp.hall == 1 && resp.month_type == 1)
	      {
		Cancel_date (cli_sock, resp, &h2_nxt_month);
	      }
	    break;
	  }*/
	case 1:
	  {
	    int day, month1, year, hall, slot;
	    sscanf (resp.id, "%02d-%02d-%04d-%d-%d", &day, &month1, &year,
		    &hall, &slot);
	    slot--;
	    hall--;
	    day--;
	    FILE *fp;
	    month h1_cur_month;
    fp = fopen (h1_cur, "rb");
   fread (&h1_cur_month, sizeof (h1_cur_month), 1, fp);
    fclose (fp);
	    if (h1_cur_month.mon == month1 && hall == 0)
	      {
		Cancel_id (cli_sock, resp, h1_cur);
	      }
	    else if ((h1_cur_month.mon + 1) == month1 && hall == 0)
	      {
		Cancel_id (cli_sock, resp, h1_nxt);
	      }


	    else if (h1_cur_month.mon == month1 && hall == 1)
	      {
		Cancel_id (cli_sock, resp, h2_cur);
	      }
	    else if ((h1_cur_month.mon + 1) == month1 && hall == 1)
	      {
		Cancel_id (cli_sock, resp, h2_nxt);
	      }

	    break;
	  }			// case
	case 2:
	  {
	    Printf_green ("Display Status");
	    date_status s;
	    if (resp.hall == 0 && resp.month_type == 0)
	      {
		s = check_status (h1_cur);
	      }

	    else if (resp.hall == 0 && resp.month_type == 1)
	      {
		s = check_status (h1_nxt);
	      }

	    else if (resp.hall == 1 && resp.month_type == 0)
	      {
		s = check_status (h2_cur);
	      }

	    else if (resp.hall == 1 && resp.month_type == 1)
	      {
		s = check_status (h2_nxt);
	      }
	    int n;
	    n = send (cli_sock, (void *) &s, sizeof (s), 0);
	    if (n < 0)
	      error ("Error sending Details\nTry again");

	    break;

	  }
	case 3:
	{
	Printf_red("Exit Client");
	close (cli_sock);
       Printf_red ("Client Socket closed");
	loop=0;
	break;//while
	}
	default:
	  Printf_red ("Service not availible");
	}			//switch
}//inner while



    }				//while
  close (ser_sock);
  Printf_red ("Server socket closed");
  return 0;


}
