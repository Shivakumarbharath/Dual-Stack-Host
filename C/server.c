#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
struct iotinfo {
char nn[10];
int tempval;
};
int main(int argc, char *argv[])
{
    // listenfd - listen socket File Descriptor 
    // connfd - Client Socket File Descriptor 
    int listenfd = 0, connfd = 0;
    int n;
    //Server Address  
    struct sockaddr_in serv_addr; 
struct iotinfo r;
struct iotinfo *p;
p=&r;
char recvBuff[10];
//    char sendBuff[1025]; // Buffer to transmit the data 
    time_t ticks;  

    // Create the Listen Socket 
        listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
//    memset(sendBuff, '0', sizeof(sendBuff)); 
    
    // for AF_INET 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 
    
    //Name the Socket for usage 
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
    // To Accept Incoming Connection - 10 clients it can take 
    listen(listenfd, 10); 

    while(1)
    {
        //Create a new socket for each client 
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
        // data to send to client 
        ticks = time(NULL);
        //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));

	n=read(connfd,(char *)p, sizeof(struct iotinfo));
	printf("%s\n",p-> nn);
       // recvBuff[n]=0;
	//printf("%s\n",recvBuff);
	sleep(1);
        // write to client Socket
        //write(connfd, sendBuff, strlen(sendBuff)); 
        // close the socket 
        close(connfd);
        
     }
}
