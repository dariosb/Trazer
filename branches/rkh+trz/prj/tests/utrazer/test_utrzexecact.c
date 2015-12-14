/**
 *  \file       test_utrzexeact.c
 *  \ingroup    Test
 *
 *  \brief      Unit test for RKH's state machine module.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2015.11.11  LeFr  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */

#include "unity_fixture.h"
#include "unitrazer.h"
#include "rkh.h"
#include "aotest.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

TEST_GROUP(utrzexeact);

/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(utrzexeact)
{
    /* -------- Setup ---------------
     * Establish the preconditions to the test 
     */
    common_test_setup();
}

TEST_TEAR_DOWN(utrzexeact)
{
    /* -------- Cleanup -------------
     * Return the system under test to its initial state after the test
     */
	unitrazer_verify(); /* Makes sure there are no unused expectations, if */
						/* there are, this function causes the test to fail. */
	unitrazer_cleanup();
}

TEST(utrzexeact, expectEventOk)
{
    UtrzProcessOut *p;

    /* -------- Expectations --------
     * Record the trace event expectations to be met
     */
	sm_trn_expect(CST(&s21), CST(&s211));

    /* -------- Exercise ------------ 
     * Do something to the system 
     */

    /* Each recorded trace event is checked to see that it matches */
    /* the expected trace event exactly. If calls are out of order or */
    /* parameters are wrong, the test immediately fails. */
    RKH_TR_SM_TRN(aotest, &s21, &s211);

    /* -------- Verify --------------
     * Check the expected outcome 
     */
    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

TEST(utrzexeact, expectEventOutOfSequence)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));

    RKH_TR_SM_ENSTATE(aotest, CST(&s21));

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_FAIL, p->status);
    TEST_ASSERT_EQUAL_STRING("Out of order Trace event. received: 'ENSTATE' "
                             "expected: 'TRN'.", p->msg);
}

TEST(utrzexeact, expectEventWithUnexpectedArg)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));

    RKH_TR_SM_TRN(aotest, &s21, &s21);

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_FAIL, p->status);
    TEST_ASSERT_EQUAL_STRING("Event 'TRN' ocurred with unexpected "
                             "value for argument 'tst=s21' expected "
                             "value='s211'.", p->msg);
}

TEST(utrzexeact, ignoreEvt)
{	
    UtrzProcessOut *p;

    sm_trn_ignore();
    sm_evtProc_expect();

    RKH_TR_SM_TRN(aotest, &s21, &s21);

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);

    RKH_TR_SM_EVT_PROC(aotest)

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

TEST(utrzexeact, ignoreOneArg)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));
    sm_trn_ignoreArg_sourceState();

    RKH_TR_SM_TRN(aotest, &s211, &s211);

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

TEST(utrzexeact, ignoreOneArgBeforeExpect)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));
	sm_evtProc_expect();
    sm_trn_ignoreArg_sourceState();

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_FAIL, p->status);
    TEST_ASSERT_EQUAL_STRING("IgnoreArg called before Expect on event 'TRN'."
                                , p->msg);
}
/* ------------------------------ End of file ------------------------------ */
