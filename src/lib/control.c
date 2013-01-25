#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "control.h"

/* A function to display an error message and then exit 
 * Supports various fatality levels:
 * 0 for Warning;
 * 1 for negligible error;
 * 2 for important but non-fatal error;
 * 3 for fatal error;
 */
//TODO: this has to be renamed, or it is in serious name clashing
// with gnu libc error()
void error(char *message, int fatality_level) {
	char error_message[100];
	
	if (fatality_level == 0) {
		strcpy(error_message, "[!!] Warning: ");
		strncat(error_message, message, 86);
		perror(error_message);
	} else if (fatality_level == 1) {
		strcpy(error_message, "[!!] Error: ");
		strncat(error_message, message, 88);
		perror(error_message);
	} else if (fatality_level == 2) {
		strcpy(error_message, "[!!] Important: ");
		strncat(error_message, message, 84);
		perror(error_message);
	} else if (fatality_level == 3) {
		strcpy(error_message, "[!!] Fatal Error: ");
		strncat(error_message, message, 82);
		perror(error_message);
		exit(-1); /* Only exit the program if a fatal error has occured. */
	}
}

/* An error checked malloc() wrapper function */
void *ec_malloc(unsigned int size) {
	void *ptr;
	
	ptr = malloc(size);
	if (ptr == NULL)
		error("in ec_malloc on memory allocation", 2);
	
	return ptr;
}

/* Dumps raw memory in hex byte and printable format */
void dump(const unsigned char *data_buffer, const unsigned int length) {
	unsigned char byte;
	unsigned int i, j;
	
	for (i = 0; i < length; i++) {
		byte = data_buffer[i];
		printf("%02x ", data_buffer[i]); /* Display byte in hex */
		
		if (((i % 16) == 15) || (i == length - 1)) {
			for (j = 0; j < 15 - (i % 16); j++)
				printf("    ");
			printf("| ");
			
			for (j = (i - (i % 16)); j <= i; j++) { /* Display printable bytes from line. */
				byte = data_buffer[j];
				if ((byte > 31) && (byte < 127)) /* Outside printable char range */
					printf("%c", byte);
				else
					printf(".");
			}
			
			printf("\n"); /* End of the dump line (each line is 16 bytes) */
		} /* End if. */
	} /* End for. */
}

/* This function accepts an open file descriptor and returns
 * the size of the associated file. Returns -1 on failure.
 */
int get_file_size(int file_descriptor) {
	struct stat stat_struct;
	
	if(fstat(file_descriptor, &stat_struct) == -1)
		return -1;

	return (int) stat_struct.st_size;
}
