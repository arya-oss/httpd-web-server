#ifndef _UTILS_H
#define _UTILS_H

int getAllFiles(const char * path, char * files[]);
void sayHello(int sfd);
void daemonize(char * webroot, char * logpath);
void sendHTML(int sfd, char * html, int length);
void send404(int sfd);
int generateHtml(char * files[], int nfiles, char * title, char ** buffer);
void getMimeType(const char * ftype, char ** mime);

#endif // _UTILS_H