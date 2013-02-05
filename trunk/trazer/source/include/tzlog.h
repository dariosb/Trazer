/*
 * tzlog.h
 */

#ifndef __TZLOG_H__
#define __TZLOG_H__

void lprintf( const char *fmt, ... );
void start_log( const char *fname );

#define cfg_printf(x)		lprintf( "%-39c| %-23s= %d\n", ' ', #x, (x) );

#endif
