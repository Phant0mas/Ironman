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
