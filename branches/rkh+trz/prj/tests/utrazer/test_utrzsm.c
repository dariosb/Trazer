/**
 *  \file       test_utrzsm.c
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

TEST_GROUP(utrzsm);

/* ---------------------------- Local variables ---------------------------- */

static RKH_ROM_STATIC_EVENT( eA, A );
static RKH_ROM_STATIC_EVENT( eB, B );
static RKH_ROM_STATIC_EVENT( eC, C );
static RKH_ROM_STATIC_EVENT( eD, D );
static RKH_ROM_STATIC_EVENT( eE, E );
static RKH_ROM_STATIC_EVENT( eF, F );
static RKH_ROM_STATIC_EVENT( eG, G );
static RKH_ROM_STATIC_EVENT( eH, H );
static RKH_ROM_STATIC_EVENT( eI, I );
static RKH_ROM_STATIC_EVENT( eTerm, TERMINATE );

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(utrzsm)
{
    /* -------- Setup ---------------
     * Establish the preconditions to the test 
     */
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

TEST_TEAR_DOWN(utrzsm)
{
    /* -------- Cleanup -------------
     * Return the system under test to its initial state after the test
     */
	unitrazer_verify(); /* Makes sure there are no unused expectations, if */
						/* there are, this function causes the test to fail. */
	unitrazer_cleanup();
}

TEST(utrzsm, expectEventOk)
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

TEST(utrzsm, expectEventOutOfSequence)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));

    RKH_TR_SM_ENSTATE(aotest, CST(&s21));

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_FAIL, p->status);
    TEST_ASSERT_EQUAL_STRING("Out of order Trace event. received: 'ENSTATE' "
                             "expected: 'TRN'.", p->msg);
}

TEST(utrzsm, expectEventWithUnexpectedArg)
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

TEST(utrzsm, ignoreEvt)
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

TEST(utrzsm, ignoreOneArg)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));
    sm_trn_ignoreArg_sourceState();

    RKH_TR_SM_TRN(aotest, &s211, &s211);

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}
/* ------------------------------ End of file ------------------------------ */
