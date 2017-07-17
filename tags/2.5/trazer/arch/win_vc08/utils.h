/*
 * utils.h
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

char * str2lower( const char *src );
char * str2upper( const char *src );

long find_str_into_file( FILE* pFile, const char* str);

#define ESC		0x1b
int test_key( void );

#endif
