/*
 * The Ironman Web Server Project.
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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h> 

#include "control.h"
#include "initialization.h"

/* 
 * A function that searches to see if /var/www exists, and if not
 * it creates it.
 * Returns 0 upon creation of file, and 1 if the file was already existent.
 */
int folder_check(void) {
	int success; /* A variable to store the relative success of the operation. */	
    
    /*
     * If the directory www does not exist, it will get created and mkdir will return 0.
     * In any other case, mkdir() will return -1.
     */
	success = mkdir("/var/www", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	
	if(success == 0){
		printf("Folder www was created!!\n");
		return 0;
	} else if (success == -1) {
		printf("Folder www already exists\n");
		return 1;
	} else {
		error("in folder_check while attempting to create file", 3);	
    }
}
			
/*
 * All the fuctions that are critical in initializing the server will be called through this one
 * so we won't have to call each one separately
 */
void initialization(void) {
	
	printf("\n\n");
	printf("Preparing to initialize the Ironman HTTP Server. \n");	
	folder_check(); /* Checking if /var/www folder exists */
}
