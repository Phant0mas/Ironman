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

#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INCLUDED

#define WEBROOT "/var/www" /* The server's root directory. */

/* A function to display an error message and then exit 
 * Supports various fatality levels:
 * 0 for Warning;
 * 1 for negligible error;
 * 2 for important but non-fatal error;
 * 3 for fatal error;
 */
void logerror(char *message, int fatality_level);

/* An error checked malloc() wrapper function */
void *ec_malloc(unsigned int size);

/* Dumps raw memory in hex byte and printable format */
void dump(const unsigned char *data_buffer, const unsigned int length);

/* This function accepts an open file descriptor and returns
 * the size of the associated file. Returns -1 on failure.
 */
int get_file_size(int file_descriptor);

#endif
