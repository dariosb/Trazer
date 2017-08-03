/**
 *  \file       test_sma_runner.c
 *  \ingroup    test_sma_group
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

TEST_GROUP_RUNNER(sma)
{
	RUN_TEST_CASE(sma, act);
	RUN_TEST_CASE(sma, term);
	RUN_TEST_CASE(sma, get);
	RUN_TEST_CASE(sma, fifo);
	RUN_TEST_CASE(sma, lifo);
	RUN_TEST_CASE(sma, reg);
	RUN_TEST_CASE(sma, unreg);
	RUN_TEST_CASE(sma, defer);
	RUN_TEST_CASE(sma, rcall);
}

/* ------------------------------ End of file ------------------------------ */
