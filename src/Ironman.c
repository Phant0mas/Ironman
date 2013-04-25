/* The Ironman Web Server Project.
 * 
 * This file is part of the Ironman Web Server Project,
 * a project that aims to build a modular and scalar web server.
 *
 * Copyright The Ironman Web Server Contributors
 * (c) 2013.
 * This file is distributed under the terms of the GNU/GPL v3 license.
 * See the file COPYING in the root folder of the project for more information.
 */

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

#define TRUE   1
#define FALSE  0

int main(void) {
	int sockfd, new_sockfd, opt = TRUE;
	struct sockaddr_in host_addr, client_addr; /* My address information */
	socklen_t sin_size;
	
	initialization(); /* Performing steps necessary for initialization */
	
	printf("Accepting web requests on port %d\n", PORT);
	
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)/*creating a socket,if error it returns -1*/
		logerror("in socket", 3);
	
	/* avoid the pesky "Address already in use" error message. */
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int)) == -1)
		logerror("setting socket option SO_REUSEADDR", 2);

	host_addr.sin_family = AF_INET; /* Host byte order */
	host_addr.sin_port = htons(PORT); /* Short, network byte order */
	host_addr.sin_addr.s_addr = INADDR_ANY; /* Automatically fill with my IP. */
	memset(&(host_addr.sin_zero), '\0', 8); /* Zero the rest of the struct. */
	
	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		logerror("binding to socket", 3);

	if (listen(sockfd, 20) == -1)
		logerror("listening on socket", 2);
		
	while (1) { /* Accept loop. */
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &sin_size);
		if (new_sockfd == -1)
			logerror("accepting connection", 2);
		
		handle_connection(new_sockfd, &client_addr);
	}
	
	return 0;
	
}
