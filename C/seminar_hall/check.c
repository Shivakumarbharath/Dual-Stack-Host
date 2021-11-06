#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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



int
main ()
{

  
  full arr[10];
int i=0;
while(1)
{

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
//      while ((getchar()) != '\n');
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
//IF available Take contact details


      printf ("Please Provide your Contact Details\nYour Name : ");
      fgets (con.name, sizeof (con.name), stdin);
      printf ("Contact Number : ");
      fgets (con.contact, sizeof (con.contact), stdin);

      arr[i].r =  req;
      arr[i].contacts=con;
//Completed
      printf ("Booking Successfull\n");
      break;
    }
case 1:
    printf ("Service Not Available");
    exit (1);
default:
    printf ("Service Not Available");
    exit (1);
  }





Display(arr[i]);







}








  return 0;
}
