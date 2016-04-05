/**
 * Author: Rajmani Arya
 * filename: main.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/select.h>

#include "../include/utils.h"
#include "../include/tcp.h"

#define BUFSIZE 65536

int sfd, nsfd;
char buffer[BUFSIZE];

int main(int argc, char const *argv[])
{
	char buf[128]; char * tmp; char * wwwroot=NULL; char * logpath=NULL;
	FILE * f = fopen("httpd.conf", "r");
	if(!f) {
		perror("fopen() httpd.conf ");
		exit(EXIT_FAILURE);
	}
	if(fgets(buf, 128, f) == NULL) {
		perror("file empty: ");
		exit(EXIT_FAILURE);
	}
	strtok_r (buf, "=", &tmp);
	if(strcmp(buf, "wwwroot") == 0) {
		wwwroot = tmp;
	} else if(strcmp(buf, "logpath") == 0) {
		logpath = tmp;
	}
	if(fgets(buf, 128, f) == NULL) {
		perror("file empty: ");
		exit(EXIT_FAILURE);
	}
	fclose(f);
	strtok_r (buf, "=", &tmp);
	if(strcmp(buf, "logpath") == 0) {
		logpath = tmp;
	} else if(strcmp(buf, "wwwroot") == 0) {
		wwwroot = tmp;
	}
	if(logpath == NULL) {
		printf("Set logpath\n");
		exit(EXIT_FAILURE);
	} else
		strtok(logpath, "\n");
	if(wwwroot == NULL) {
		printf("Set wwwroot path\n");
		exit(EXIT_FAILURE);
	} else
		strtok(wwwroot, "\n");

	if(access(wwwroot, F_OK|R_OK) < 0) {
		printf("wwwroot path doesn't exist !!%s\n", wwwroot);
		exit(EXIT_FAILURE);
	}

	if(access(logpath, F_OK|R_OK|W_OK) < 0) {
		printf("logpath doesn't exist or access denied !%s\n", logpath);
		exit(EXIT_FAILURE);
	}
	
	daemonize(wwwroot, logpath);

	sfd = tcpsock_bind("127.0.0.1", 8000, 5);
	if(sfd < 0) {
		printf("Error !! Can't create server\n");
		exit(EXIT_FAILURE);
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