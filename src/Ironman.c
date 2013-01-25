#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lib/networking.h"
#include "lib/initialization.h"

void handle_connection(int, struct sockaddr_in *); /* Handle web requests */

int main(void) {
	int sockfd, new_sockfd, yes = 1;
	struct sockaddr_in host_addr, client_addr; /* My address information */
	socklen_t sin_size;
	
	initialization(); /* Performing steps necessary for initialization */
	
	folder_check(); /* Checking if /var/www folder exists */

	printf("Accepting web requests on port %d\n", PORT);
	
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		error("in socket", 3);
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		error("setting socket option SO_REUSEADDR", 2);

	host_addr.sin_family = AF_INET; /* Host byte order */
	host_addr.sin_port = htons(PORT); /* Short, network byte order */
	host_addr.sin_addr.s_addr = INADDR_ANY; /* Automatically fill with my IP. */
	memset(&(host_addr.sin_zero), '\0', 8); /* Zero the rest of the struct. */
	
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		error("binding to socket", 3);

	if (listen(sockfd, 20) == -1)
		error("listening on socket", 2);
		
	while (1) { /* Accept loop. */
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &sin_size);
		if (new_sockfd == -1)
			error("accepting connection", 2);
		
		handle_connection(new_sockfd, &client_addr);
	}
	
	return 0;
	
}

/* This function handles the connection on the passed socket from the
 * passed client address. The connection is processed as a web request,
 * and this function replies over the connected socket. Finally, the
 * passed socket is closed at the end of the function.
 */
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr) {
	unsigned char *ptr, request[500], resource[500];
	int fd, length;
	
	length = recv_line(sockfd, request);

	printf("Got request from %s:%d \"%s\"\n", inet_ntoa(client_addr_ptr->sin_addr),
			ntohs(client_addr_ptr->sin_port), request);
			
	ptr = strstr(request, " HTTP/"); // Search for valid-looking request.
	if(ptr == NULL) { // Then this isn't valid HTTP.
		printf(" NOT HTTP!\n");
	} else {
		*ptr = 0; // Terminate the buffer at the end of the URL.
		ptr = NULL; // Set ptr to NULL (used to flag for an invalid request).

		if(strncmp(request, "GET ", 4) == 0) // GET request
			ptr = request+4; // ptr is the URL.
		
		if(strncmp(request, "HEAD ", 5) == 0) // HEAD request
			ptr = request+5; // ptr is the URL.
			
		if(ptr == NULL) { // Then this is not a recognized request.
			printf("\tUNKNOWN REQUEST!\n");
		} else { /* valid request, with ptr pointing to resource name */
			if (ptr[strlen(ptr) - 1] == '/') // For resources ending with '/',
				strcat(ptr, "index.html"); // add 'index.html' to the end.
			strcpy(resource, WEBROOT); // Begin resource with web root path
			strcat(resource, ptr); // and join it with resource path.
			fd = open(resource, O_RDONLY, 0); // Try to open the file.
			printf("\tOpening \'%s\'\t", resource);
			
			if (fd == -1) { /* If file is not found */
				printf(" 404 Not Found\n");
				send_string(sockfd, "HTTP/1.0 404 NOT FOUND\r\n");
				send_string(sockfd, "Server: Ironman HTTP Server\r\n\r\n");
				send_string(sockfd, "<html><head><title>Error: 404 File Not Found on the server</title></head>");
				send_string(sockfd, "<body><h1>URL not found</h1></body></html>\r\n");
				send_string(sockfd, "<body><h2>Ironman HTTP Server\r\n\r\n");
			} else { /* Otherwise, serve up the file */
				printf(" 200 OK\n");
				send_string(sockfd, "HTTP/1.0 200 OK\r\n");
				send_string(sockfd, "Server: Ironman HTTP Server\r\n\r\n");
				
				if(ptr == request + 4) { // Then this is a GET request
					if( (length = get_file_size(fd)) == -1)
						error("getting resource file size", 1);
					
					if( (ptr = (unsigned char *) malloc(length)) == NULL)
						error("allocating memory for reading resource", 2);

					read(fd, ptr, length); // Read the file into memory.
					send(sockfd, ptr, length, 0); // Send it to socket.
					free(ptr); /* Free file memory. */
				}
				
				close(fd); /* Close the file. */
			} /* End if block for file found/not found. */
		} /* End if block for valid request. */
	} /* End if block for valid HTTP. */
	
	shutdown(sockfd, SHUT_RDWR); /* Close the socket gracefully. */
}
