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

#define BUFFER_SIZE 1024

void error(const char *msg){
perror(msg);// inbuilt function that outputs as an error
exit(1);
}


int recive_play(int sock,struct addrinfo* psinfo){
int n;
int sin6len = sizeof(struct sockaddr_in6);
FILE *out;
char buffer[BUFFER_SIZE];
out = fopen("out.mp3", "wb");
printf("file open");
    while (1) {
n = recvfrom(sock, buffer, BUFFER_SIZE, 0, 
                    (struct sockaddr *)&psinfo->ai_addr, &sin6len);

    //n = read(cli_sock, buffer, BUFFER_SIZE);
    if (n < 0) 
        error("ERROR reading from socket");
    else if (n == 0) // Socket closed. Transfer is complete (or borked)
        break;

    fwrite(buffer, 1, n, out); // Could check fwrite too.
}
    printf("File write complete... You can now use the output file!!\n");
system("mpg123 out.mp3");
system("rm out.mp3");
    if (out != NULL)  
        fclose(out);

return 0;
}


int main(int argc, char* argv[])
{
   int sock;
   int status;
   struct addrinfo sainfo, *psinfo;
   struct sockaddr_in6 sin6;
   int sin6len;
   char buffer[BUFFER_SIZE];

   sin6len = sizeof(struct sockaddr_in6);

   if(argc < 2)
     printf("Specify a port number\n"), exit(1);

   sock = socket(PF_INET6, SOCK_DGRAM,0);

   memset(&sin6, 0, sizeof(struct sockaddr_in6));
   sin6.sin6_port = htons(0);
   sin6.sin6_family = AF_INET6;
   sin6.sin6_addr = in6addr_any;

   status = bind(sock, (struct sockaddr *)&sin6, sin6len);

   if(-1 == status)
     perror("bind"), exit(1);

   memset(&sainfo, 0, sizeof(struct addrinfo));
   memset(&sin6, 0, sin6len);

   sainfo.ai_flags = 0;
   sainfo.ai_family = PF_INET6;
   sainfo.ai_socktype = SOCK_DGRAM;
   sainfo.ai_protocol = IPPROTO_UDP;
   status = getaddrinfo(argv[1], argv[2], &sainfo, &psinfo);

   switch (status) 
     {
      case EAI_FAMILY: printf("family\n");
	break;
      case EAI_SOCKTYPE: printf("stype\n");
	break;
      case EAI_BADFLAGS: printf("flag\n");
	break;
      case EAI_NONAME: printf("noname\n");
	break;
      case EAI_SERVICE: printf("service\n");
	break;
     }
   sprintf(buffer,"Ciao");

   status = sendto(sock, buffer, strlen(buffer), 0,
		     (struct sockaddr *)psinfo->ai_addr, sin6len);
   printf("buffer : %s \t%d\n", buffer, status);
  printf("Port number : %d\n",ntohs(sin6.sin6_port));
status = recvfrom(sock, buffer, BUFFER_SIZE, 0, 
                     (struct sockaddr *)&psinfo->ai_addr, &sin6len);
printf("%s\n",buffer);
recive_play(sock,psinfo);
   // free memory
   freeaddrinfo(psinfo);
   psinfo = NULL;

   shutdown(sock, 2);
   close(sock);
   return 0;
}
