#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

void error(const char *msg){
perror(msg);//inbuilt function that outputs with stderr with error number
exit(1);
}

int main(int argc,char *argv[]){//to take the arguments from the command line

if (argc<2){
fprintf("stderr  Port number not provided . Program Terminated");
exit(0);

}
int sockfd,newsockfd,portno,n;
char buffer[255];
struct sockaddr_in serv_addr,client_addr;
socklen_t client_len;


return 0;

}

