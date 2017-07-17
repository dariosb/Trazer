/*
 * Testtzlog.c
 */


#include "tzlog.h"
#include "options.h"
#include "error.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

FILE *flog = NULL;
static char lprintf_Buff[LPRINTF_BUZZ_SIZE];

void
lprintf( const char *fmt, ... )
{
    va_list args;
    char *ps;

    ps = lprintf_Buff + strlen(lprintf_Buff);

    if(ps >=  (lprintf_Buff + LPRINTF_BUZZ_SIZE) )
        ps = lprintf_Buff;

    va_start(args,fmt);

	vsprintf( ps, fmt, args);

#ifndef __TZLOG_STUB__
	if( !options.quiet )
	    vfprintf( stdout, fmt, args);
#endif
	if( flog != NULL )
	{
	    vfprintf( flog, fmt, args);
		fflush(flog);
	}

    va_end(args);

}

void
start_log( const char *fname )
{
	if(flog != NULL)
		fclose(flog);

	flog = NULL;

	if( strlen(fname) == 0 )
		return;
		
	if( ( flog = fopen( fname, "w+" ) ) == NULL )
	{
		fatal_error( "Can't open file %s\n", fname );
	}
}


void
initTesttzlog( void )
{
	memset( lprintf_Buff, '\0', LPRINTF_BUZZ_SIZE );
}


