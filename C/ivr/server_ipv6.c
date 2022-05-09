/* $Id$ 
 * 
 *
 * 
 * 
 */
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/if.h>

#define BUFFER_SIZE 1024

void error(const char *msg){
perror(msg);// inbuilt function that outputs as an error
exit(1);
}

int send_audio(int sock,char *in,struct sockaddr_in6 cli6){
    char buffer[BUFFER_SIZE];
int cli6len = sizeof(struct sockaddr_in6);
bzero(buffer, BUFFER_SIZE);
FILE *fp;
fp = fopen(in, "rb"); 
     if (fp == NULL) 
         printf("File open failed!\n"); 
     else
         printf("File successfully opened!\n");

    while (1) {
    size_t num_read = fread(buffer, 1, BUFFER_SIZE, fp);
    if (num_read == 0) // end of file.
        break;
         
    int n;
    n = sendto(sock, buffer, num_read, 0,
                     (struct sockaddr *)&cli6, cli6len);
    //n = write(cli_sock, buffer, num_read);
    if (n < 0) // Error
        error("ERROR writing to socket");
    else if (n == 0) // Could handle this too
        break;
    }
    printf("File sending complete...\n");

    if (fp != NULL)  
        fclose(fp);
return 0;
}


int main(int argc, char *argv[])
{
   int sock;
   int status;
   struct sockaddr_in6 sin6,cli6;
   int sin6len;
   char buffer[BUFFER_SIZE];
  char ip[130];
   sock = socket(AF_INET6, SOCK_DGRAM,0);

   sin6len = sizeof(struct sockaddr_in6);

   memset(&sin6, 0, sin6len);

   /* just use the first address returned in the structure */

   sin6.sin6_port = htons(atoi(argv[1]));
   sin6.sin6_family = AF_INET6;
   sin6.sin6_addr = in6addr_any;
   sin6.sin6_scope_id=if_nametoindex(argv[2]);
   status = bind(sock, (struct sockaddr *)&sin6, sin6len);
   if(-1 == status)
     perror("bind"), exit(1);

   status = getsockname(sock, (struct sockaddr *)&sin6, &sin6len);

   printf("%d\n",ntohs(sin6.sin6_port));

   status = recvfrom(sock, buffer, BUFFER_SIZE, 0, 
		     (struct sockaddr *)&cli6, &sin6len);
if(inet_ntop(AF_INET6, &cli6, ip, sizeof(ip))==NULL)error("inet_ntop Error");
printf("ipv6 Address : %s\t",ip);
printf("Port number %d\n\n\n\n",ntohs(cli6.sin6_port));
   printf("buffer : %s\n", buffer);
strcpy(buffer,"Send to client");
status = sendto(sock, buffer, strlen(buffer), 0,
                     (struct sockaddr *)&cli6, sin6len);
send_audio(sock,"welcome.mp3",cli6);

   shutdown(sock, 2);
   close(sock);
   return 0;
}
