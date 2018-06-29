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
char lprintf_Buff[LPRINTF_BUZZ_SIZE];

void
lprintf( const char *fmt, ... )
{
    va_list args;
    char *ps;

    ps = lprintf_Buff + strlen(lprintf_Buff);

    if(ps >=  (lprintf_Buff + LPRINTF_BUZZ_SIZE) )
        ps = lprintf_Buff;

	/*
	 * Portability Note:
	 * http://www.gnu.org/software/libc/manual/html_node/Variable-Arguments-Output.html
	 * The value of the va_list pointer is undetermined after the call to vprintf,
	 * so you must not use va_arg after you call vprintf.Instead, you should call va_end
	 * to retire the pointer from service.You can call va_start again and begin fetching
	 * the arguments from the start of the variable argument list. (Alternatively,
	 * you can use va_copy to make a copy of the va_list pointer before calling vfprintf.)
	 * Calling vprintf does not destroy the argument list of your function, merely the 
	 * particular pointer that you passed to it.
	 */
    va_start(args,fmt);
	vsprintf( ps, fmt, args);
	va_end(args);

#ifndef __TZLOG_STUB__
	if (!options.quiet)
	{
		va_start(args, fmt);
		vfprintf(stdout, fmt, args);
		va_end(args);
	}
#endif

	if( flog != NULL )
	{
		va_start(args, fmt);
	    vfprintf( flog, fmt, args);
		va_end(args);
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


