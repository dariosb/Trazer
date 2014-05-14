/*
 * tzlog.h
 */

#ifndef __TZLOG_H__
#define __TZLOG_H__

void lprintf( const char *fmt, ... );
void start_log( const char *fname );

#define cfg_printf(x)		lprintf( "%-34c| %-23s= %d\n", ' ', #x, (x) );
#define cfg_printf_sz(x,f)	lprintf( "%-34c| %-23s= %d\n", ' ', #x, (x*f) );
#define rkhver_printf(x)	lprintf( "%-34c| %-23s= %s\n", ' ', #x, (x) );

#endif
