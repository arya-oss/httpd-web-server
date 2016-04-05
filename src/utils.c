#include "../include/utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <time.h>
#include <dirent.h>
#include <gio/gio.h>

/**
 * @param path directory path from root directory from which it scans all
 * directory and files.
 * @param files it stores all filename
 * @return return no. of files in a directory
 */

int getAllFiles(const char * path, char * files[]) {
	DIR * dir; struct dirent * file;
	dir = opendir(path);
	int cnt=0;
	while(file = readdir(dir)) {
		if(file->d_name[0] != '.'){
			files[cnt++] = file->d_name;
		}
	}
	free(dir);
	free(file);
	return cnt;
}

void daemonize(char * webroot, char * logpath)
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

	close(fileno(stdin));
	char logfile[128];
	if(*(logpath+strlen(logpath)-1) == '/') {
		sprintf(logfile, "%serror.log", logpath);
	} else {
		sprintf(logfile, "%s/error.log", logpath);
	}

	freopen(logfile, "w+", stdout);
	freopen(logfile, "w+", stderr);
}

void sayHello(int sfd) {
	char * header = "\r\nHTTP/1.1 200 ok\r\nContent-Type: text/html\r\nServer: AryaHttp 0.0.1 (Ubuntu 64bit)\r\nContent-Length: 135\r\nDate: Tue, 05 Apr 2016 12:28:53 GMT\r\nConnection: keep-alive\r\n\n";
	char * message = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<title>Arya</title>\n</head>\n<body>\n<h1>Hello Arya ! </h1>\n</body>\n</html>";
	if(send(sfd, header, strlen(header), 0) <= 0){
		perror("send() ");
	};
	if(send(sfd, message, strlen(message), 0) <= 0){
		perror("send() ");
	};
}
/**
 * @param sfd socket file descriptor to whom html content to be sent
 * @param html the html content
 * @param length length of html content
 */
void sendHTML(int sfd, char * html, int length) {
	char header[1024] = "\r\nHTTP/1.1 200 ok\r\nContent-Type: text/html\r\nServer: AryaHttp 0.0.1 (Ubuntu 64bit)\r\nContent-Length: ";
	char leng[10];
	sprintf(leng, "%d", length);
	strcat(header, leng);
	strcat(header, "\r\nDate: Tue, 05 Apr 2016 12:28:53 GMT\r\nConnection: keep-alive\r\n\n");
	if(send(sfd, header, strlen(header), 0) <= 0){
		perror("send() ");
	};
	if(send(sfd, html, length, 0) <= 0){
		perror("send() ");
	};
}
/**
 * Send 404 not found response over http protocol
 * @param sfd socket file descriptor
 */
void send404(int sfd) {
	char * header = "\r\nHTTP/1.1 200 ok\r\nContent-Type: text/html\r\nServer: AryaHttp 0.0.1 (Ubuntu 64bit)\r\nContent-Length: 164\r\nDate: Tue, 05 Apr 2016 12:28:53 GMT\r\nConnection: keep-alive\r\n\n";
	char * message = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<title>404 not found</title>\n</head>\n<body>\n<h3>You are looking at wrong place. </h3>\n</body>\n</html>";
	if(send(sfd, header, strlen(header), 0) <= 0){
		perror("send() ");
	};
	if(send(sfd, message, strlen(message), 0) <= 0){
		perror("send() ");
	};
}
/**
 * Genrates listed html content for given array of filenames
 * @param files
 * @param title
 * @param length
 * @param buffer
 * @return integer length of buffer
 */
int generateHtml(char * files[], int length, char * title, char ** buffer) {
	int i;
	char buf[65535] = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"UTF-8\">\n<title> Index of ";
	strcat(buf, title);
	strcat(buf, "</title>\n</head>\n<body>\n<h2>Directory content at ");
	strcat(buf, title);
	strcat(buf, "</h3>\n<ul>\n");
	for (i = 0; i < length; ++i) {
		strcat(buf, "<li><a href=\"");
		strcat(buf, files[i]);
		strcat(buf, "\">");
		strcat(buf, files[i]);
		strcat(buf, "</a></li>\n");
	}
	strcat(buf, "</ul>\n</body>\n</html>");
	*buffer = buf;
	return strlen(buf);
}
/**
 * get Mime Type for any file dependant on glib-2.0 gio/gio.h
 * @param ftype is the filename
 * @param mime it stores the filename of mime
 */
void getMimeType(const char * ftype, char ** mime) {
    gboolean is_sure = FALSE;
    char * content_type = g_content_type_guess(ftype, NULL, 0, &is_sure);
    if(content_type != NULL) {
        *mime = g_content_type_get_mime_type(content_type);
    } else {
    	*mime = malloc(10);
    	strcpy(*mime, "text/html");
    }
    g_free(content_type);
}