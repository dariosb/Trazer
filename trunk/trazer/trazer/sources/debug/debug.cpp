/*
 * debug.cpp
 */

#include "debug.h"
#include "options.h"
#include <stdio.h>
#include <stdarg.h>
#include <conio.h>

void
dprintf( const char* fmt, ...)
{
    va_list args;

	if( !options.enable_debug )
		return;

    va_start(args,fmt);

    vfprintf( stdout, fmt,args);

    va_end(args);

}
