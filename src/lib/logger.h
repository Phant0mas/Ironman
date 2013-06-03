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

#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

char *print_time();

void log_print(char *filename, int line, char *fmt, ...);

#define LOG_PRINT(...) log_print(__FILE__, __LINE__, __VA_ARGS__ )

#endif
