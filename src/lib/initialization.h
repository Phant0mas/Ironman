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

#ifndef INITIALIZATION_H_INCLUDED
#define INITIALIZATION_H_INCLUDED

/* A function that searches to see if /var/www exists, and if not
 * it creates it.
 * Returns 0 upon creation of file, and 1 if the file was already existent.
 */
int folder_check(void);

			
/* A basic function printing some (warning) messages */
void initialization(void);
#endif
