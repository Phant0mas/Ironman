#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include "control.h"
#include "networking.h"

/* This function accepts a socket file descriptor and a pointer
 * to the null terminated string to send. The function will make sure 
 * all the bytes of teh string are sent. Returns 1 on success and 0 on 
 * failure.
 */
int send_string(int sockfd, unsigned char *buffer) {
	int sent_bytes, bytes_to_send;
	bytes_to_send = strlen(buffer);
	while (bytes_to_send > 0) {
		sent_bytes = send(sockfd, buffer, bytes_to_send, 0);
		if (sent_bytes == -1)
			return 0; /* Return 0 on send error */
		bytes_to_send -= sent_bytes;
		buffer += sent_bytes;
	}
	
	return 1; /* Return 1 on success. */
}

/* This function accepts a socket FD and a ptr to a destination
 * buffer. It will receive from the socket until the EOL byte
 * sequence in seen. The EOL bytes are read from the socket, but
 * the destination buffer is terminated before these bytes.
 * Returns the size of the read line (without EOL bytes).
 */
int recv_line(int sockfd, unsigned char *dest_buffer) {
#define EOL "\r\n" /* End of line byte sequence. */
#define EOL_SIZE 2
	unsigned char *ptr;
	int eol_matched = 0;
	
	ptr = dest_buffer;
	while (recv(sockfd, ptr, 1, 0) == 1) { /* Read a single byte. */
		if (*ptr == EOL[eol_matched]) { /* Does this byte match terminator? */
			eol_matched++;
			if (eol_matched == EOL_SIZE) { /* If all bytes match terminator. */
				*(ptr + 1 - EOL_SIZE) = '\0'; /* Terminate the string. */
				return strlen(dest_buffer); /* Return bytes received. */
			}
		} else {
			eol_matched = 0;
		}

		ptr++; /* Increment the pointer to the next byte. */
	}
	
	return 0; /* Didn't find the end-of-line characters. */
#undef EOL
#undef EOL_SIZE
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
