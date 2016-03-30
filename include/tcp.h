#ifndef _TCP_H
#define _TCP_H

int tcpsock_bind(char * host, int port, int backlog);
int tcpsock_connect(char * host, int port);
int tcpsock_accept(int sfd);

#endif // _TCP_H