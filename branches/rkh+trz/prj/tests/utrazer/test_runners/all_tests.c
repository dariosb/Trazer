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
 *  \file       all_tests.c
 *  \ingroup    Test
 *
 *  \brief      ---
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2015.11.11  LeFr  v2.4.05  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */

#include "rkh.h"
#include "unity_fixture.h"
#include "unitrazer.h"
#include "bsp.h"
#include "aotest.h"


/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */

static 
void 
runAllTests(void)
{
	RUN_TEST_GROUP(utrazer);
	RUN_TEST_GROUP(utrzsm);
	RUN_TEST_GROUP(utrzexeact);
}

/* ---------------------------- Global functions --------------------------- */

int
main(int argc, char *argv[])
{
	bsp_init(argc, argv);
	UnityMain(argc, argv, runAllTests);
    rkh_trc_close();
	for(;;)
        ;
}


void
common_test_setup( void )
{
    ut_resetOut();
    unitrazer_init();
    fwk_ignore();        /* Ignore every trace event of FWK group */
	sm_ts_state_ignore();        /* Ignore every trace event of FWK group */

    RKH_TR_FWK_AO(aotest);
    RKH_TR_FWK_STATE(aotest, &s);
    RKH_TR_FWK_STATE(aotest, &s1);
    RKH_TR_FWK_STATE(aotest, &s11);
    RKH_TR_FWK_STATE(aotest, &s2);
    RKH_TR_FWK_STATE(aotest, &s21);
    RKH_TR_FWK_STATE(aotest, &s211);
    RKH_TR_FWK_SIG(A);

	/* set trace filters */
	RKH_FILTER_ON_GROUP( RKH_TRC_ALL_GROUPS );
	RKH_FILTER_ON_EVENT( RKH_TRC_ALL_EVENTS );
	RKH_FILTER_OFF_EVENT( A );
	RKH_FILTER_OFF_GROUP_ALL_EVENTS( RKH_TG_SM );
	RKH_FILTER_OFF_GROUP_ALL_EVENTS( RKH_TG_FWK );
	RKH_FILTER_OFF_SMA( aotest );

    rkh_sma_init_hsm(aotest);
}

void
common_tear_down( void )
{
	unitrazer_verify(); /* Makes sure there are no unused expectations, if */
						/* there are, this function causes the test to fail. */
	unitrazer_cleanup();    
}
/* ------------------------------ End of file ------------------------------ */
