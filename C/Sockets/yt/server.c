#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

void
error (const char *msg)
{
  perror (msg);			//inbuilt function that outputs with stderr with error number
  exit (1);
}

int
main (int argc, char *argv[])
{				//to take the arguments from the command line

  if (argc < 2)
    {
      printf ("stderr  Port number not provided . Program Terminated");
      exit (-1);

    }
  int sockfd, newsockfd, portno, n;
  char buffer[255];
  struct sockaddr_in serv_addr, client_addr;	//internet address in netinet
  socklen_t client_len;		//

  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    {
      error ("Error opening a socket!!");
    }
  bzero ((char *) &serv_addr, sizeof (serv_addr));	//clears any data whatever it is refereced to
  portno = atoi (argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons (portno);
  if (bind (sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
    error ("Binding Failed");
  listen (sockfd, 5);
  client_len = sizeof (client_addr);
  newsockfd = accept (sockfd, (struct sockaddr *) &client_addr, &client_len);

  if (newsockfd < 0)
    error ("Error while accepting");

  while (1)
    {

      bzero (buffer, 255);
      n = read (newsockfd, buffer, sizeof (buffer));
      if (n < 0)
	error ("Error while reading");
      printf ("Client :%s\n", buffer);
      bzero (buffer, 255);
      fgets (buffer, 255, stdin);
      n = write (newsockfd, buffer, strlen (buffer));
      if (n < 0)
	error ("Error on writing");

      int i = strncmp ("Bye", buffer, 3);
      if (i == 0)
	break;
    }

  close (newsockfd);
  close (sockfd);
  return 0;

}
