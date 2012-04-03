/*
 * 	rerror.c
 * 		Run errors
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "rerror.h"

int debugging;

int
debug( const char *fmt, ... )
{
	va_list ap;

	if( debugging )
	{
		va_start( ap, fmt );
		vfprintf( stderr, fmt, ap );
		va_end( ap );
		fputc( '\n', stderr );
		return 1;
	}
	return 1;
}

int
fatal( const char *fmt, ... )
{
	va_list ap;

	va_start( ap, fmt );
	vfprintf( stderr, fmt, ap );
	va_end( ap );
	fputc( '\n', stderr );
	fprintf( stderr, "%s: Input enter to terminate program:", __FUNCTION__ );
	getchar();
	exit( EXIT_FAILURE );
//	return 0;
}

int
inform_error( const char *fmt, ... )
{
	va_list ap;

	va_start( ap, fmt );
	vfprintf( stderr, fmt, ap );
	va_end( ap );
	fputc( '\n', stderr );
	return -1;
}


