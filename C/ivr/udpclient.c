#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#define BUFFER_SIZE 255

void error(const char *msg){perror(msg);exit(1);}

void main(int argc, char **argv){
  if(argc < 3){
    printf("Usage: %s <ip address> <port>\n", argv[0]);
    exit(0);
  }

  int port = atoi(argv[2]);
  int cli_sock;
  struct sockaddr_in6 serverAddr;
  char buffer[BUFFER_SIZE];
  socklen_t addr_size;

  cli_sock = socket(PF_INET6, SOCK_DGRAM, 0);
  memset(&serverAddr, '\0', sizeof(serverAddr));

  serverAddr.sin6_family = AF_INET6;
  serverAddr.sin6_port = htons(port);
  //serverAddr.sin6_scope_id=if_nametoindex(argv[3]);

if(inet_pton(AF_INET6,argv[1],&serverAddr.sin6_addr)<=0)error("\n inet_pton error occured\n\n");

  strcpy(buffer, "Hello Server\n");
  sendto(cli_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
  printf("[+]Data Send: %s", buffer);

}
