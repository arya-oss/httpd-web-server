#include "../include/utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

/**
 * @param path directory path from root directory from which it scans all
 * directory and files.
 * @return return array of filename.
 */

char ** scandir(char * path) {
	return NULL;	
}

static void daemonize(char * webroot, char * logpath)
{
	pid_t pid, sid;
	/* already a daemon */
	if ( getppid() == 1 ) return;
	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then we can exit the parent process. */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* At this point we are executing as the child process */
	/* Change the file mode mask */
	umask(0);
	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}
	/* Change Web root diectory a/c to configuration */
	if ((chdir(webroot)) < 0) {
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	char logfile[128];
	if((logpath+strlen(logpath)-1) == '/') {
		sprintf(logfile, "%serror.log", logpath);
	} else {
		sprintf(logfile, "%s/error.log", logpath);
	}

	freopen(logfile, "w+", stdout);
	freopen(logfile, "w+", stderr);
}

void sayHello(int sfd) {
	char * header = "\r\nHTTP/1.1 200 ok\r\nContent-Type: text/html\r\nServer: AryaHttp 0.0.1 (Ubuntu 64bit)\r\nContent-Length: 135\r\nDate: Mon, 31 Mar 2016 12:28:53 GMT\r\nConnection: keep-alive\r\n\n";
	char * message = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<title>Arya</title>\n</head>\n<body>\n<h1>Hello Arya ! </h1>\n</body>\n</html>";
	if(send(sfd, header, strlen(header), 0) <= 0){
		perror("send() ");
	};
	if(send(sfd, message, strlen(message), 0) <= 0){
		perror("send() ");
	};
}