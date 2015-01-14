#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
 int sockfd;
 //int len;
 //struct sockaddr_in address;
 int rc;
 char get_request[] = "GET /\n\n";
 char buf[16384];

 sockfd = socket(AF_INET, SOCK_STREAM, 0);
 if(sockfd < 0) {
	perror("failed to create a socket to talk to server\n");
	return -1;
 }
 printf("Created socket with descriptor %d\n", sockfd);

struct addrinfo *ai;

 getaddrinfo("0.0.0.0", "8080", NULL, &ai);


 //address.sin_family = AF_INET;
 //address.sin_addr.s_addr = inet_addr("0.0.0.0");
 //address.sin_port = htons(8080);
 //len = sizeof(address);
 printf("Connecting to server at 0.0.0.0 port 8080\n");
 //rc = connect(sockfd, (struct sockaddr *)&address, len);
 rc = connect(sockfd, (struct sockaddr *)ai->ai_addr, sizeof(struct sockaddr));
 if (rc < 0)
 {
  perror("oops: client1");
  return -1;
 }
 printf("Connected to server\n");
 rc = send(sockfd, get_request, sizeof(get_request), 0);
 if(rc != sizeof(get_request)) {
 	printf("Failed to send %lu bytes containing %s\n", sizeof(get_request), get_request);
 	exit(1);
 }
 printf("Sent %d bytes to server\n", rc);
 rc = recv(sockfd, buf, sizeof(buf), 0);
 if(rc < 0) {
 	perror("recv from server failed\n");
 	exit(1);
 } else {
 	printf("Server returned %d bytes:\n%s|||", rc, buf);
 }
 printf("Disconnecting from server\n");
 close(sockfd);
 return 0;
}