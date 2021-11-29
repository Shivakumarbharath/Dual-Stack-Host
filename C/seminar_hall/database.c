#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void Printf(const char *msg ){printf("\n\n\n-------------------------------------------%s---------------------------------------\n\n\n\n",msg);}

void scan(int *p){
scanf("%d",p);
while ((getchar()) != '\n');
return;
}

typedef struct exchange
{

int type;/*0 - booking 
		 1 - cancelling 
		 2 - cancelling by booking id
		3 - rescheduling */


int hall;//Old Seminar hall-0(take H1)
	//New Seminar hall 1(take h2)
int month_type;//current month-0
		//next month-1
int event_day;//month.date1[event_day-1]

int slot_no;//month.date1[event_day-1].slots[slot_no]

char id[15];//booking id for cancellation
}exchange;


typedef struct slot
{
  int booking_status;
  char name[50];
  char contact[11];
  char booking_id[17];
}slot;

typedef struct date
{
  slot slots[7];
  int work_day;
}date;

typedef struct month
{
   date date1[31];
  int mon;
  int year;
}month;


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

month h1_cur_month,h1_nxt_month,h2_nxt_month,h2_cur_month;

void append ();
void display ();
void displayAll ();
void modify ();
void del ();
void search ();



char mygetch ();

char fname[] = { "mydb.dat" };

int
main ()
{
  int ch;

  while (1)
    {
      system ("clear");

      printf
	("=================Seminar Hall Management System============\n\n");

      printf ("1. Append\n\n");
      printf ("2. Modify\n\n");
      printf ("3. Delete\n\n");
      printf ("4. Search\n\n");
      printf ("5. Display\n\n");
      printf ("6. Display All\n\n");
      printf ("0. Exit\n\n");

      printf ("========================================================\n\n");

      printf ("\nPlease enter your Choice:");
      scanf ("%d", &ch);

      switch (ch)
	{
	case 1:
	  append ();
	  break;

	case 2:
	  modify ();
	  break;

	case 3:
	  del ();
	  break;

	case 4:
	  search ();
	  break;

	case 5:
	  display ();
	  break;

	case 6:
	  displayAll ();
	  break;


	case 0:
	  exit (0);
	}

      mygetch ();
    }

  return 0;
}

void
append ()
{
  FILE *fp;
  Initialise(&h1_cur_month);
 /* struct slot h1;

  fp = fopen (fname, "ab");

  printf ("\nBooking Status:");
  scanf ("%d", &h1.booking_status);
  printf ("\nName:");
  scanf ("%s", h1.name);
  printf ("\nContact Number:");
  scanf ("%s", h1.contact);
  printf ("\nBooking ID:");
  scanf ("%s", h1.booking_id);*/
fp = fopen (fname, "ab");
  fwrite (&h1_cur_month, sizeof (h1_cur_month), 1, fp);

  fclose (fp);
}



void
modify ()
{
  FILE *fp;// *fp1;
  month h1;
  int found = 0, count = 0;
  char booking_id[17];
  fp = fopen (fname, "rb");
 // fp1 = fopen ("temp.dat", "wb");

  //printf ("\nEnter the Booking ID you want to Modify:");
  //scanf ("%s", booking_id);

  /*while (1)
    {
      fread (&h, sizeof (h), 1, fp);

      if (feof (fp))
	{
	  break;
	}
      if (strcmp (h.booking_id, booking_id))
	{
	  found = 1;
	  printf ("\nEnter Booking ID:");
	  scanf ("%s", h.booking_id);

	  fflush (stdin);
	  printf ("\nEnter Name:");
	  scanf ("%s", h.name);
	  printf ("\nEnter Contact Number:");
	  scanf ("%s", h.contact);
	  fwrite (&h, sizeof (h), 1, fp1);
	}
      else
	{
	  fwrite (&h, sizeof (h), 1, fp1);
	}
    }*/

    exchange resp;
    slot con;
    printf("\n\nEnter the date you want to book? : ");
	scan(&resp.event_day);
	resp.event_day-=1;
	printf("\n\nEnter Slot Number(1-7) : ");
	scan(&resp.slot_no);
	resp.slot_no-=1;
    fread(&h1,sizeof(h1),1,fp);
    //Perform checking
    if(h1.date1[resp.event_day].slots[resp.slot_no].booking_status==0)
    {
    printf ("\n\nThe slot is Free\n\nPlease Provide your Contact Details\nYour Name : ");
		fgets (con.name, sizeof (con.name), stdin);
		printf ("Contact Number : ");
		fgets (con.contact, sizeof (con.contact), stdin);

		h1.date1[resp.event_day].slots[resp.slot_no]=con;

		fwrite (&h1, sizeof (h1), 1, fp);
		Printf("Booking Successfull");
    }
    else
	{
		printf("\n\nThe Slot Has already been Booked\nDetails Given\n");
		//Display_slot(con);
		Printf("Booking Failed");
	}
    
  fclose (fp);
 // fclose (fp1);
/*
  if (found == 0)
    {
      printf ("Sorry No Record Found\n\n");
    }
  else
    {
      fp = fopen (fname, "wb");
      fp1 = fopen ("temp.dat", "rb");

      while (1)
	{
	  fread (&h, sizeof (h), 1, fp1);

	  if (feof (fp1))
	    {
	      break;
	    }
	  fwrite (&h, sizeof (h), 1, fp);
	}

    }
  fclose (fp);
  fclose (fp1);*/
}

void
del ()
{
  FILE *fp, *fp1;
  struct slot h, h1;
  int found = 0, count = 0;
  char booking_id[17];
  fp = fopen (fname, "rb");
  fp1 = fopen ("temp.dat", "wb");

  printf ("\nEnter the Booking ID you want to Delete:");
  scanf ("%s", booking_id);

  while (1)
    {
      fread (&h, sizeof (h), 1, fp);

      if (feof (fp))
	{
	  break;
	}
      if (strcmp (h.booking_id, booking_id))
	{
	  found = 1;
	}
      else
	{
	  fwrite (&h, sizeof (h), 1, fp1);
	}
    }
  fclose (fp);
  fclose (fp1);

  if (found == 0)
    {
      printf ("Sorry No Record Found\n\n");
    }
  else
    {
      fp = fopen (fname, "wb");
      fp1 = fopen ("temp.dat", "rb");

      while (1)
	{
	  fread (&h, sizeof (h), 1, fp1);

	  if (feof (fp1))
	    {
	      break;
	    }
	  fwrite (&h, sizeof (h), 1, fp);
	}
    }
  fclose (fp);
  fclose (fp1);
}

void
display ()
{
  FILE *fp;
  struct slot h;
  int found = 0;
  char booking_id[17];
  fp = fopen (fname, "rb");

  printf ("\nEnter the Booking ID:");
  scanf ("%s", booking_id);

  while (1)
    {
      fread (&h, sizeof (h), 1, fp);

      if (feof (fp))
	{
	  break;
	}
      if (strcmp (h.booking_id, booking_id))
	{
	  found = 1;
	  printf
	    ("\n========================================================\n\n");
	  printf ("\t\t Details of %s\n\n", h.booking_id);
	  printf
	    ("========================================================\n\n");

	  printf ("Booking Status\tName\tContact Number\n\n");

	  printf ("%d\t", h.booking_status);
	  printf ("%s\t", h.name);
	  printf ("%s\t\n\n", h.contact);

	  printf
	    ("========================================================\n\n");
	}
    }
  if (found == 0)
    {
      printf ("\nSorry No Record Found");
    }
  fclose (fp);
}

void
search ()
{
  FILE *fp;
  struct slot h;
  int found = 0;
  char name[50];

  fp = fopen (fname, "rb");

  printf ("\nEnter the Name:");
  scanf ("%s", name);

  while (1)
    {
      fread (&h, sizeof (h), 1, fp);

      if (feof (fp))
	{
	  break;
	}
      if (strcmp (name, h.name) == 0)
	{
	  printf
	    ("\n========================================================\n\n");
	  printf ("\t\t Details of %s\n\n", h.booking_id);
	  printf
	    ("========================================================\n\n");

	  printf ("Name\tContact Number\tBooking ID\n\n");

	  printf ("%s\t", h.name);
	  printf ("%s\t", h.contact);
	  printf ("%s\t\n\n", h.booking_id);

	  printf
	    ("========================================================\n\n");

	}
    }
  if (found == 0)
    {
      printf ("\nSorry No Record Found");
    }
  fclose (fp);
}

void
displayAll ()
{
  FILE *fp;
  struct slot h;

  fp = fopen (fname, "rb");

  printf ("\n========================================================\n\n");
  printf ("\t\t Seminar Hall Details\n\n");
  printf ("========================================================\n\n");

  printf ("Booking Status\tName\tContact Number\tBooking ID\n\n");

  while (1)
    {
      fread (&h, sizeof (h), 1, fp);

      if (feof (fp))
	{
	  break;
	}
      printf ("%d\t", h.booking_status);
      printf ("%s\t", h.name);
      printf ("%s\t", h.contact);
      printf ("%s\t\n\n", h.booking_id);

    }
  printf ("========================================================\n\n");

  fclose (fp);
}

char
mygetch ()
{
  char val;
  char rel;

  scanf ("%c", &val);
  scanf ("%c", &rel);
  return (val);
}
