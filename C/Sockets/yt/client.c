#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>		//used to store the information about a given host

void
error (const char *msg)
{
  perror (msg);			//inbuilt function that outputs with stderr with error number
  exit (1);
}

int
main (int argc, char *argv[])
{				//to take the arguments from the command line


  int sockfd, portno, n;
  char buffer[255];
  struct sockaddr_in serv_addr;
  struct hostent *server;

  if (argc < 3)
    {
      printf
	("stderr  Port number Ip address not provided . Program Terminated\nUsage Filename <IP Address> <Port Number>");
      exit (-1);

    }


  portno = atoi (argv[2]);
  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    {
      error ("Error opening a socket!!");
    }

  server = gethostbyname (argv[1]);	//get the host name
  if (server == NULL)
    error ("No such host \nTry Again with the valid host");
  bzero ((char *) &serv_addr, sizeof (serv_addr));	//clears any data whatever it is refereced to
  serv_addr.sin_family = AF_INET;
  bcopy ((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr,
	 server->h_length);
  serv_addr.sin_port = htons (portno);
  if (connect (sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) <
      0)
    error ("Connection failed");
  while (1)
    {
      bzero (buffer, 255);
      printf("Client Sent: ");
      fgets (buffer, 255, stdin);
      n = write (sockfd, buffer, strlen (buffer));
      if (n < 0)
	error ("Wrinting Failed");
      bzero (buffer, 255);
      n = read (sockfd, buffer, 255);
      if (n < 0)
	error ("Reading Failed");
      printf ("Client Recieved : %s", buffer);
      int i = strncmp ("bye", buffer, 3);
      if (i == 0)
	break;
    }

  return 0;
}
