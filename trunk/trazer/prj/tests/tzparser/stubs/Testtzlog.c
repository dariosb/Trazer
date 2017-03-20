/*
 * Testtzlog.c
 */

#include "Testtzlog.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

char lprintf_Buff[LPRINTF_BUZZ_SIZE];
char start_log_callFlag;

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

    va_end(args);

}

void
start_log( const char *fname )
{
#if 0
	flog = NULL;

	if( strlen(fname) == 0 )
		return;
		
	if( ( flog = fopen( fname, "w+" ) ) == NULL )
	{
		fatal_error( "Can't open file %s\n", fname );
	}
#else
	start_log_callFlag = 1;
#endif
}


void
initTesttzlog( void )
{
	start_log_callFlag = 0;
	memset( lprintf_Buff, '\0', LPRINTF_BUZZ_SIZE );
}


