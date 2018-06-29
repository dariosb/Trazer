/**
 *  \file       test_log_runner.c
 *  \ingroup    test_log_group
 *
 *  \brief      State machine group runner
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

#include "unity_fixture.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

TEST_GROUP_RUNNER(log)
{
	RUN_TEST_CASE(log, init);

	RUN_TEST_CASE(log, rq);
	RUN_TEST_CASE(log, sma);
	RUN_TEST_CASE(log, sm);
	RUN_TEST_CASE(log, tim);
	RUN_TEST_CASE(log, fwk);

	RUN_TEST_CASE(log, close);

}

/* ------------------------------ End of file ------------------------------ */
