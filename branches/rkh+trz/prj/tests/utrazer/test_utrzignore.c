/**
 *  \file       test_utrzignore.c
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
#include "test_common.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

TEST_GROUP(utrzIgnore);

/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(utrzIgnore)
{
    /* -------- Setup ---------------
     * Establish the preconditions to the test 
     */
    common_test_setup();
}

TEST_TEAR_DOWN(utrzIgnore)
{
    /* -------- Cleanup -------------
     * Return the system under test to its initial state after the test
     */
    common_tear_down();
}


/* RKH_TE_SM_TRN */

TEST(utrzIgnore, sm_trn_Ok)
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

TEST(utrzIgnore, sm_trn_OneArg)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));
    sm_trn_ignoreArg_sourceState();

    RKH_TR_SM_TRN(aotest, &s211, &s211);

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

TEST(utrzIgnore, sm_trn_OneArgBeforeExpect)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));
	sm_evtProc_expect();
    sm_trn_ignoreArg_sourceState();

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_FAIL, p->status);
    TEST_ASSERT_EQUAL_STRING("IgnoreArg called before Expect on event 'TRN'."
                                , p->msg);
	RKH_TR_SM_TRN(aotest, &s21, &s211);
	RKH_TR_SM_EVT_PROC(aotest);
}

/* RKH_TE_SM_EXE_ACT */

TEST(utrzIgnore, sm_exeAct_Ok)
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

TEST(utrzIgnore, sm_exeAct_OneArg)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));
    sm_trn_ignoreArg_sourceState();

    RKH_TR_SM_TRN(aotest, &s211, &s211);

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

TEST(utrzIgnore, sm_exeAct_OneArgBeforeExpect)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));
	sm_evtProc_expect();
    sm_trn_ignoreArg_sourceState();

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_FAIL, p->status);
    TEST_ASSERT_EQUAL_STRING("IgnoreArg called before Expect on event 'TRN'."
                                , p->msg);
	RKH_TR_SM_TRN(aotest, &s21, &s211);
	RKH_TR_SM_EVT_PROC(aotest);
}

/* ------------------------------ End of file ------------------------------ */
