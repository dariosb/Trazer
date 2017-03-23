/*
 * log.c
 */

#include "stdio.h"

#define LOGNAME "tzparse.log"
#define STREAM_LOGNAME "stream.log"

static FILE *flog, *sflog;

void
logOpen(void)
{
	flog = fopen( LOGNAME, "w+" ); 
	sflog = fopen( STREAM_LOGNAME, "w+b" ); 
}

void
logPrint(char *p)
{
    fprintf(flog, p);
}

void
logStream(char *p, unsigned short len)
{
	fwrite ( p, 1, len, sflog );
}


logClose(void)
{
    fclose(flog);
    fclose(sflog);
}
