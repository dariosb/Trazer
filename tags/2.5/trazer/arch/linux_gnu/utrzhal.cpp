/*
 * TCP/IP communication for Win32
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "tcp.h"
#include "tzlog.h"
#include "utrzhal.h"
#include "mydefs.h"

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

