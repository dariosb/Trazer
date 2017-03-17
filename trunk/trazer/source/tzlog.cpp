/*
 * tzlog.cpp
 */

#include "error.h"
#include "options.h"
#include "tzlog.h"
#include <stdio.h>
#include <stdarg.h>

FILE *flog;

void
lprintf( const char *fmt, ... )
{
    va_list args;

    va_start(args,fmt);

	if( !options.quiet )
	    vfprintf( stdout, fmt, args);
	if( flog != NULL )
	    vfprintf( flog, fmt, args);

	fflush(flog);

    va_end(args);
}

void
start_log( const char *fname )
{
	flog = NULL;

	if( strlen(fname) == 0 )
		return;
		
	if( ( flog = fopen( fname, "w+" ) ) == NULL )
	{
		fatal_error( "Can't open file %s\n", fname );
	}
}

