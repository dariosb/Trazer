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
    if(flog != NULL)
        fprintf(flog, p);
}

void
logStream(char *p, unsigned short len)
{
    if(sflog != NULL)
	    fwrite( p, 1, len, sflog );
}


logClose(void)
{
    if(flog != NULL)
        fclose(flog);
    if(sflog !=NULL)
        fclose(sflog);
}
