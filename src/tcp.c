#include "../include/tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

/*=============================================
 *          Author: Rajmani Arya
 * filename: tcp.c
 * Date: 31 March 2016
 * LICENSE: The MIT License
 ============================================*/


/**
 * This function binds a specific IPv4 and Port and Listen for requests with
 * backlog given as parameter
 * @param host is an IPv4 address in char array format
 * @param port is an unsigned integer of range 1024-65535
 * @param backlog is unsigned short of range 1-128 is used for
 * no. of client waits in case of concurrent connection requests
 * @return integer socket file descriptor, on success it return > 0 otherwise -1
 */
int tcpsock_bind(char * host,unsigned int port, unsigned short backlog) {
	struct sockaddr_in addr; int sfd;
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sfd < 0) {
		perror("socket() ");
		return -1;
	}
	memset((char *)&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);
	if(bind(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
		perror("bind() ");
		close(sfd);
		return -1;
	}
	if(listen(sfd, backlog) < 0) {
		perror("listen() ");
		close(sfd);
		return -1;
	}
	return sfd;
}

/**
 * This function creates a tcp socket and connects with IP:Port given as parameter
 * @param  host is an IPv4 address in char array format
 * @param  port port is an unsigned integer of range 1024-65535
 * @return      integer socket file descriptor, on success it return > 0 otherwise -1
 */
int tcpsock_connect(char * host, unsigned int port) {
	struct sockaddr_in addr; int sfd;
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sfd < 0) {
		perror("socket() ");
		return -1;
	}
	memset((char *)&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);

	if(connect(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
		perror("connect() ");
		close(sfd);
		return -1;
	}
	return -1;
}

/**
 * Accepts TCP connection on a socket file descriptor
 * @param  sfd TCP binded socket file desciptor
 * @return     socket file descriptor for client, > 0 if success otherwise -1
 */
int tcpsock_accept(int sfd) {
	struct sockaddr_in addr; int cli_len = sizeof addr;
	int nsfd;
	nsfd = accept(sfd, (struct sockaddr*)&addr, &cli_len);
	if(nsfd < 0) {
		perror("accept() ");
		return -1;
	}
	return nsfd;
}
/* End of tcp.c */