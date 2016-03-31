/**
 * Author: Rajmani Arya
 * filename: main.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/select.h>

#include "../include/utils.h"
#include "../include/tcp.h"

#define BUFSIZE 65536

int sfd, nsfd;
char buffer[BUFSIZE];

int main(int argc, char const *argv[])
{
	sfd = tcpsock_bind("127.0.0.1", 8000, 5);
	if(sfd < 0) {
		printf("Error !! Can't create server\n");
		exit(1);
	}
	fd_set rfds;
	while(1) {
		FD_ZERO(&rfds);
		FD_SET(sfd, &rfds);
		if(select(sfd+1, &rfds, NULL, NULL, NULL) < 0) {
			perror("select() ");
			continue;
		}
		if(FD_ISSET(sfd, &rfds)) {
			nsfd = tcpsock_accept(sfd);
			if(nsfd < 0) {
				printf("Accepting Error !!\n");
				continue;
			}
			recv(nsfd, buffer, BUFSIZE, 0);
			printf("%s\n", buffer);
			sayHello(nsfd);
			close(nsfd);
		}
	}
	return 0;
}

/* End of main.c */