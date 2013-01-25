/*
 * error.cpp
 */

#include "error.h"
#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void
fatal_error( const char* fmt, ...)
{
    va_list args;

    va_start(args,fmt);

    vfprintf(stderr, fmt,args);

    va_end(args);

	while(!test_key());

	exit( EXIT_FAILURE );
}


	
