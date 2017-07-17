/**
 *  \file       test_rtime_runner.c
 *  \ingroup    test_rtime_group
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

TEST_GROUP_RUNNER(rtime)
{
	RUN_TEST_CASE(rtime, normalFifo);
	RUN_TEST_CASE(rtime, normalLifo);
	RUN_TEST_CASE(rtime, getWoutPut);
	RUN_TEST_CASE(rtime, traceStreamLost);
	RUN_TEST_CASE(rtime, getSameEvtFromOtherAO);
}

/* ------------------------------ End of file ------------------------------ */
