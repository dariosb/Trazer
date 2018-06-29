/**
 *  \file       test_rq_runner.c
 *  \ingroup    test_rq_group
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

TEST_GROUP_RUNNER(rq)
{
	RUN_TEST_CASE(rq, init);
	RUN_TEST_CASE(rq, get);
	RUN_TEST_CASE(rq, fifo);
	RUN_TEST_CASE(rq, lifo);
	RUN_TEST_CASE(rq, full);
	RUN_TEST_CASE(rq, dpt);
	RUN_TEST_CASE(rq, getlast);
}

/* ------------------------------ End of file ------------------------------ */
