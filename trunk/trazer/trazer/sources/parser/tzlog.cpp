/*
 * evtbl.cpp
 */

#include "error.h"

#include <stdio.h>
#include <stdarg.h>
#include <conio.h>

FILE *flog;

void
lprintf( const char *fmt, ... )
{
    va_list args;

    va_start(args,fmt);

    vfprintf( stdout, fmt, args);
    vfprintf( flog, fmt, args);

	fflush(flog);

    va_end(args);
}

void
start_log( const char *fname )
{
	if( ( flog = fopen( fname, "w+" ) ) == NULL )
	{
		fatal_error( "Can't open file %s\n", fname );
	}
}
