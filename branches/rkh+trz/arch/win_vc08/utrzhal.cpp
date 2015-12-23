/*
 * TCP/IP communication for Win32
 */

#include <stdio.h>
#include <conio.h>
#include <winsock2.h>
#include "tcp.h"
#include "tzlog.h"
#include "utrzhal.h"
#include "mydefs.h"

char utrz_msg[1024];

void
rkh_trc_flush(void)
{
    rui8_t *blk;
    TRCQTY_T nbytes;

	for(;;)
    {
        nbytes = (TRCQTY_T)1024;
        blk = rkh_trc_get_block(&nbytes);

        if ((blk != (rui8_t *)0))
        {
			tcpSend(blk, nbytes);
        }
        else
        {
            break;
        }
    }
}

void
utrz_resp( RKH_TE_ID_T e, rui32_t line, char *msg, int nargs, ... )
{
    int i;
    va_list val;
	char *s;

    strcpy( utrz_msg, msg );
    va_start( val, nargs );
    for (i=0;i<nargs;i++)
    {
        s=va_arg(val, char *);
        strcat( utrz_msg, s );
    }
   

    lprintf("line %d:", line); 
    lprintf(utrz_msg); 

	rkh_trc_begin( e );
	RKH_TRC_UI32(line);
	RKH_TRC_STR(utrz_msg);
	rkh_trc_end();
	rkh_trc_flush();
}

