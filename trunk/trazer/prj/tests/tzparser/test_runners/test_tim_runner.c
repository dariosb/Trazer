/**
 *  \file       test_tim_runner.c
 *  \ingroup    test_tim_group
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

TEST_GROUP_RUNNER(tim)
{
	RUN_TEST_CASE(tim, init);
	RUN_TEST_CASE(tim, start);
	RUN_TEST_CASE(tim, stop);
	RUN_TEST_CASE(tim, tout);
	RUN_TEST_CASE(tim, rem);
}

/* ------------------------------ End of file ------------------------------ */
