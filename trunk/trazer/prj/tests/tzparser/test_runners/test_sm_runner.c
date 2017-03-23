/**
 *  \file       test_sm_runner.c
 *  \ingroup    test_sm_group
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

TEST_GROUP_RUNNER(sm)
{
	RUN_TEST_CASE(sm, init);
	RUN_TEST_CASE(sm, clrh);
	RUN_TEST_CASE(sm, trn);
	RUN_TEST_CASE(sm, state);
	RUN_TEST_CASE(sm, enstate);
	RUN_TEST_CASE(sm, exstate);
	RUN_TEST_CASE(sm, nenex);
	RUN_TEST_CASE(sm, ntrnact);
	RUN_TEST_CASE(sm, tsstate);
	RUN_TEST_CASE(sm, evtproc);
	RUN_TEST_CASE(sm, evtnfound);
	RUN_TEST_CASE(sm, grdfalse);
	RUN_TEST_CASE(sm, cndnfound);
	RUN_TEST_CASE(sm, unkstate);
	RUN_TEST_CASE(sm, exhlevel);
	RUN_TEST_CASE(sm, extseg);
	RUN_TEST_CASE(sm, exeact);
}

/* ------------------------------ End of file ------------------------------ */
