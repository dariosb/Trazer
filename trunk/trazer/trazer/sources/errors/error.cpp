/*
 * error.cpp
 */

#include "error.h"
#include <stdio.h>
#include <conio.h>
#include <stdarg.h>
#include <stdlib.h>

void
fatal_error( const char* fmt, ...)
{
    va_list args;

    va_start(args,fmt);

    vfprintf(stderr, fmt,args);

    va_end(args);

	exit( EXIT_FAILURE );
}


	
