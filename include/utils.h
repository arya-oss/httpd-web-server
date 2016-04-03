#ifndef _UTILS_H
#define _UTILS_H

char ** scandir(char * path);
void sayHello(int sfd);
void daemonize(char * webroot, char * logpath);

#endif // _UTILS_H