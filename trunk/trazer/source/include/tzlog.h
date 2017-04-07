/*
 * tzlog.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TZLOG_H__
#define __TZLOG_H__

#define LPRINTF_BUZZ_SIZE  1000000

extern char lprintf_Buff[];

void lprintf( const char *fmt, ... );
void initTesttzlog(void);

extern void start_log( const char *fname );

#define cfg_printf(x)		lprintf( "%-38c| %-30s= %d\n", ' ', #x, (x) );
#define cfg_printf_sz(x,f)	lprintf( "%-38c| %-30s= %d\n", ' ', #x, (x*f) );
#define rkhver_printf(x)	lprintf( "%-38c| %-30s= %s\n", ' ', #x, (x) );

#define TRAZER_FMT_TSTAMP_8BIT  " %3u|"
#define TRAZER_FMT_TSTAMP_16BIT " %5u|"
#define TRAZER_FMT_TSTAMP_32BIT " %10u|"

#define TRAZER_FMT_NSEQ         " [%3d] |"

#define TRAZER_FMT_TRACE_EVT    " %-4s| %-10s : "


#endif

#ifdef __cplusplus
}
#endif


