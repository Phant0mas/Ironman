#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h> 

#include "control.h"
#include "initialization.h"

/* A function that searches to see if /var/www exists, and if not
 * it creates it.
 * Returns 0 upon creation of file, and 1 if the file was already existent.
 */
int folder_check(void) {
	int success; /* A variable to store the relative success of the operation. */
	int file_descriptor; /* A variable used as the file descriptor. */
	int folder_created; /* A variable to store whether the file was created or not. */
	int removed_file; /* A variable to store whether the pointer file was successfuly erased. */
	
	//TODO: This is a hack. Check to see if there are any better ways to get this implemented.
	success = system("if [ -d /var/www ] ; then touch folder_exists ; fi"); 
	
	if (success == 0) { /* Touch returns 0 on success. */
		file_descriptor = open("./folder_exists", O_RDONLY, S_IRUSR);
		
		/* At this point, if there is an error, and file_descriptor is -1
		 * we assume that, since the user running the software should be
		 * root, then the file doesn't exists, because if it existed
		 * root user would have no issue being able to open it. */
		 if (file_descriptor == -1) {
			 
			/* Default permissions for /var/www are "drwxr-x-r-x" */
			folder_created = mkdir("/var/www", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
			if (folder_created == 0){
				/* We assume that folder_exists exists at this point
				 * so we delete it to finish gracefully.
				 */
				close(file_descriptor);
				remove("./folder_exists");
				return 0;
			} /* End if. */
			else
				error("in folder_check while attempting to create file", 3);
		} else {
			/* At this point we created folder_exists but we have not closed it */
			close(file_descriptor);
			remove("./folder_exists"); /* Deleting the file gracefully. */
			return 1;
		} 
	} /* End if. */
}

			
/* A basic function printing some (warning) messages */
//XXX: This should do more, or get deleted. As is right now
// it's only polluting the code base.
//TODO: This could as well provide some basic information about the computer it
// is running on, such as the port and the ip, host os, etc.
void initialization(void) {
	
	printf("\n\n");
	printf("Preparing to initialize the Ironman HTTP Server. \n");	
}
