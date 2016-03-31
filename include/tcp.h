#ifndef _TCP_H
#define _TCP_H

int tcpsock_bind(char * host,unsigned int port,unsigned short backlog);
int tcpsock_connect(char * host,unsigned int port);
int tcpsock_accept(int sfd);

#endif // _TCP_H