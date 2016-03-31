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