/*
 * Battle sheep
 */

#include <ncurses.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void my_error(char *e)
{
	perror(e);
	exit(-1);
}

int main()
{
	int sockfd = 0,
		n = 0;
	char buffer[256];
	struct sockaddr_in servAddr;
	struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(	(sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		my_error("sockfd()");

	server = gethostbyname("95.54.27.100");

	bzero((char *) &servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&servAddr.sin_addr.s_addr, server->h_length);
	servAddr.sin_port = htons(10000);

	if( (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr))) < 0 )
		my_error("connect()");


	char buf[256];
	for(int i = 1; i <= 2; ++i)
	{
		bzero(buf, sizeof(buf));
		snprintf(buf, sizeof(buf), "Connection #%d is successfull\n", i);
		if( (n = write(sockfd, buf, strlen(buf))) < 0)
			my_error("writting to socket");
		bzero(buf, 256);
		if( (n = read(sockfd, buf, 255)) < 0)
			my_error("reading from socket");
		printf("%s\n", buf);
	}
	printf("It work\n");


	while(1)
	{
		printf("Accept to write\n");
		bzero(buffer, sizeof(buffer));
		fgets(buffer, 255, stdin);
		buffer[strlen(buffer) + 1] = '\0';
		if( (n = write(sockfd, buffer, strlen(buffer))) < 0)
			my_error("writing to socket");
		bzero(buffer, 256);
		if( (n = read(sockfd, buffer, 255)) < 0)
			my_error("reading from socket");
		printf("%s\n", buffer);
	}

	return 0;
}

