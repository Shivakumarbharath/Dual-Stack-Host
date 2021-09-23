#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 


struct iotinfo
{
char nn[10];
int tempval;
//timestap
};


int main(int argc, char *argv[])
{
    char nn1[] = "pgdm";
    int sockfd = 0, n = 0;
    // recieving buffer 
    char recvBuff[1024];
    //new client socket to bind with server
    struct sockaddr_in serv_addr; 



    struct iotinfo s; //s of type structure 
    struct iotinfo *p; //p pointer of iotinfo structure

    p = &s; //p of type s by address

    strcpy(p->nn,nn1);


    char sendBuff[] = "nmit";
    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 
    // set the memory for the address 
    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 
    // client Connect request to Server 
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    /*while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    } */

    write(sockfd, (char *)p, sizeof(struct iotinfo));


    if(n < 0)
    {
        printf("\n Read error \n");
    } 

    return 0;
}
