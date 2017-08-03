/*
 *  --------------------------------------------------------------------------
 *
 *                                Framework RKH
 *                                -------------
 *
 *            State-machine framework for reactive embedded systems
 *
 *                      Copyright (C) 2010 Leandro Francucci.
 *          All rights reserved. Protected by international copyright laws.
 *
 *
 *  RKH is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option) any
 *  later version.
 *
 *  RKH is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with RKH, see copying.txt file.
 *
 *  Contact information:
 *  RKH web site:   http://sourceforge.net/projects/rkh-reactivesys/
 *  e-mail:         francuccilea@gmail.com
 *  ---------------------------------------------------------------------------
 */

/**
 *  \file       test_common.c
 *  \ingroup    Test
 *
 *  \brief      ---
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2015.11.11  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña  dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */

#include "test_common.h"
#include "Mockerror.h"
#include "Mockseqdiag.h"
#include "tzlog.h"
#include "log.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

TRCQTY_T
getTraceStream(rui8_t **p)
{
    rui8_t *blk;
    TRCQTY_T nbytes;

    FOREVER
    {
        nbytes = (TRCQTY_T)1024;
        blk = rkh_trc_get_block(&nbytes);

        if ((blk != (rui8_t *)0))
        {
			*p = blk;
            return nbytes;
        }
        else
        {
			return 0;
        }
    }
}

void
execTrazerParser( void )
{
	TRCQTY_T trcSize, i;
    rui8_t *ptrc, *p;

    trcSize = getTraceStream(&ptrc);
    TEST_ASSERT_NOT_EQUAL(0, trcSize);
    TEST_ASSERT_NOT_NULL(ptrc);

    logStream(ptrc, trcSize);

    for(i=0, p=ptrc; i<trcSize; ++i, ++ptrc)
    {
        tzparser_exec(*ptrc);
    }
}

void
trazerInitConfiguration( void )
{
	RKH_TRC_SEND_CFG(CLOCKS_PER_SEC);
    sdiag_text_Expect(MSC_TARGET_START);
    execTrazerParser();
}


void
trazerOutExpect( char *p, ushort nseq, char *group, char *evt, char *args)
{
    sprintf( p, TRAZER_FMT_TSTAMP_32BIT TRAZER_FMT_NSEQ TRAZER_FMT_TRACE_EVT "%s\n", 
                            rkh_trc_getts(), nseq, group, evt, args );
}

void
common_test_setup(void)
{
   	rkh_fwk_init();
	
	rkh_trc_init();

	RKH_FILTER_OFF_EVENT(RKH_TRC_ALL_EVENTS);
	RKH_FILTER_OFF_ALL_SMA();
	RKH_FILTER_OFF_ALL_SIGNALS();
    Mockseqdiag_Init();
    Mockerror_Init();

	tzparser_init();

    trazerInitConfiguration();
}

void
common_tear_down(void)
{
    Mockseqdiag_Verify();
    Mockseqdiag_Destroy();

    Mockerror_Verify();
    Mockerror_Destroy();

    logPrint(lprintf_Buff);
}

void
test_init( void )
{
   	rkh_fwk_init();
	
	rkh_trc_init();

	RKH_FILTER_OFF_EVENT(RKH_TRC_ALL_EVENTS);
	RKH_FILTER_OFF_ALL_SMA();
	RKH_FILTER_OFF_ALL_SIGNALS();

	tzparser_init();

    trazerInitConfiguration();
}

/* ------------------------------ End of file ------------------------------ */
