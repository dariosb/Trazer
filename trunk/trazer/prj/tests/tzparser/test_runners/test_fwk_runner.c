/**
 *  \file       test_fwk_runner.c
 *  \ingroup    test_fwk_group
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

TEST_GROUP_RUNNER(fwk)
{
	RUN_TEST_CASE(fwk, en);
	RUN_TEST_CASE(fwk, ex);
	RUN_TEST_CASE(fwk, epreg);
	RUN_TEST_CASE(fwk, ae);
	RUN_TEST_CASE(fwk, gc);
	RUN_TEST_CASE(fwk, gcr);
	RUN_TEST_CASE(fwk, defer);
	RUN_TEST_CASE(fwk, rcall);
	RUN_TEST_CASE(fwk, obj);
	RUN_TEST_CASE(fwk, sig);
	RUN_TEST_CASE(fwk, fun);
	RUN_TEST_CASE(fwk, tusr);
	RUN_TEST_CASE(fwk, syncevt);
	RUN_TEST_CASE(fwk, cfg);
	RUN_TEST_CASE(fwk, assert);
	RUN_TEST_CASE(fwk, ao);
	RUN_TEST_CASE(fwk, state);
	RUN_TEST_CASE(fwk, pstate);
	RUN_TEST_CASE(fwk, timer);
	RUN_TEST_CASE(fwk, epool);
	RUN_TEST_CASE(fwk, queue);
    RUN_TEST_CASE(fwk, actor);
}

/* ------------------------------ End of file ------------------------------ */
