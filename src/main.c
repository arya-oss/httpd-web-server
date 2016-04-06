/**
 * Author: Rajmani Arya
 * filename: main.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#include "../include/utils.h"
#include "../include/tcp.h"

// #define DAEMON

#define BUFSIZE 65000

int sfd, nsfd;
char buffer[BUFSIZE];
char * files[128];
int fcount, len;

int main(int argc, char const *argv[])
{
	char buf[128], _buf[128]; char * tmp; char wwwroot[128]; char logpath[128];
	int i, chunks;
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
		strcpy(wwwroot, tmp);
	} else if(strcmp(buf, "logpath") == 0) {
		strcpy(logpath, tmp);
	}
	if(fgets(buf, 128, f) == NULL) {
		perror("file empty: ");
		exit(EXIT_FAILURE);
	}
	fclose(f);
	strtok_r (buf, "=", &tmp);
	if(strcmp(buf, "logpath") == 0) {
		strcpy(logpath, tmp);
	} else if(strcmp(buf, "wwwroot") == 0) {
		strcpy(wwwroot, tmp);
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
	
	#ifdef DAEMON
	daemonize(wwwroot, logpath);
	#else
	if(chdir(wwwroot) < 0) {
		perror("chdir() ");
	}
	#endif

	sfd = tcpsock_bind("127.0.0.1", 8000, 5);
	if(sfd < 0) {
		printf("Error !! Can't create server\n");
		exit(EXIT_FAILURE);
	}
	fd_set rfds;
	// printf("%s %s\n", wwwroot, logpath);
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
			if(recv(nsfd, buffer, BUFSIZE, 0) < 0) {
				close(nsfd);
				continue;
			}
			// printf("%s", buffer);
			if(sscanf(buffer, "GET%s", buf) > 0) { 
				// Get request came so there must be a path after a whitespace
				// buf having path of resource
				if(buf[0] == '/' && strlen(buf) == 1) {
					sprintf(buf, ".");
				} else if(buf[0] == '/') {
					memcpy(_buf, buf+1, strlen(buf));
					strcpy(buf, _buf);
				}
				printf("Requested %s %d\n", buf, (int)strlen(buf));
				fflush(stdout);
				if(access(buf, F_OK|R_OK) < 0) {
					send404(nsfd);
				} else {
					char * html;
					struct stat st;
					stat(buf, &st);
					if(st.st_mode && S_ISDIR(st.st_mode)) {
						fcount = getAllFiles(buf, files);
						len = generateHtml(files, fcount, buf, &html);
						sendHTML(nsfd, html, len);
					} else if(st.st_mode && S_ISREG(st.st_mode)) {
						if(st.st_size < 65000) {
							fcount = st.st_size;
							FILE * f = fopen(buf, "rb");
							sendMIME(nsfd, buf, fcount);
							sendfile(nsfd, fileno(f), 0, fcount);
							fclose(f);
						} else {
							printf("Larger File %s\n", buf);
							fflush(stdout);
							FILE * f = fopen(buf, "rb");
							fcount = st.st_size;
							sendMIME(nsfd, buf, fcount);
							sendfile(nsfd, fileno(f), 0, fcount);
							printf("File size %d sent byte.", fcount);
							fflush(stdout);
							fclose(f);
						}
					} else {
						html = malloc(100);
						strcpy(html, "Not a regular file i.e, link or fifo or special file.");
						len =strlen(html);
						sendHTML(nsfd, html, len);
					}
					// free(html);
				}
			}
			memset(buffer, 0, BUFSIZE);
			close(nsfd);
			// recv(nsfd, _buf, 128, 0); // expermineting to avoid TIME_WAIT
		}
	}
	return 0;
}
/* End of main.c */