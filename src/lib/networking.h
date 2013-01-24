#ifndef NETWORKING_H_INCLUDED /*Preprocessor guards, to avoid having the */
#define NETWORKING_H_INCLUDED /* file included multiple times during compilation */

#define PORT 80 /* The port users will be connecting to. */

/* This function accepts a socket file descriptor and a pointer
 * to the null terminated string to send. The function will make sure 
 * all the bytes of teh string are sent. Returns 1 on success and 0 on 
 * failure.
 */
int send_string(int sockfd, unsigned char *buffer);	
		
/* This function accepts a socket FD and a ptr to a destination
 * buffer. It will receive from the socket until the EOL byte
 * sequence in seen. The EOL bytes are read from the socket, but
 * the destination buffer is terminated before these bytes.
 * Returns the size of the read line (without EOL bytes).
 */
int recv_line(int sockfd, unsigned char *dest_buffer);
#endif
