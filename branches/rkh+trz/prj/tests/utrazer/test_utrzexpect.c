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
#include "aotest_act.h"
#include "test_common.h"

/* ----------------------------- Local macros ------------------------------ */
#define str(a)  #a

#define utrzExpect_Ok_check()   \
        {                       \
            UtrzProcessOut *p;  \
            p = ut_getLastOut();    \
            TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);  \
        }

#define utrzExpect_OutOfSequence_check(ex, rc)  \
        {                                       \
            UtrzProcessOut *p;                  \
            p = ut_getLastOut();                \
            TEST_ASSERT_EQUAL(UT_PROC_FAIL, p->status);             \
            TEST_ASSERT_EQUAL_STRING("Out of order Trace event."    \
                    " received: '"rc"' expected: '"ex"'.", p->msg); \
        }


#define utrzExpect_WithUnexpectedArg_check( ev, an, e, r )  \
        {                                               \
            UtrzProcessOut *p;                          \
            p = ut_getLastOut();                        \
            TEST_ASSERT_EQUAL(UT_PROC_FAIL, p->status); \
            TEST_ASSERT_EQUAL_STRING("Event '"ev"' ocurred with unexpected " \
                             "value for argument '"an"="r"' expected "       \
                             "value='"e"'.", p->msg);                        \
        }
             
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

TEST_GROUP(utrzExpect);

/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(utrzExpect)
{
    /* -------- Setup ---------------
     * Establish the preconditions to the test 
     */
    common_test_setup();
}

TEST_TEAR_DOWN(utrzExpect)
{
    /* -------- Cleanup -------------
     * Return the system under test to its initial state after the test
     */
    common_tear_down();
}

/* RKH_TE_SM_INIT */

TEST(utrzExpect, sm_init_Ok)
{
	sm_init_expect(CST(&s21));

    RKH_TR_SM_INIT(aotest, &s21);

    utrzExpect_Ok_check();
}

TEST(utrzExpect, sm_init_OutOfSequence)
{
	sm_init_expect(CST(&s21));

    RKH_TR_SM_ENSTATE(aotest, CST(&s21));

    utrzExpect_OutOfSequence_check( "INIT", "ENSTATE" );
}

TEST(utrzExpect, sm_init_WithUnexpectedArg)
{
	sm_init_expect(CST(&s21));

    RKH_TR_SM_INIT(aotest, &s211);

    utrzExpect_WithUnexpectedArg_check( "INIT", "ist", str(s21), str(s211) )
}

/* RKH_TE_SM_CLRH */

TEST(utrzExpect, sm_clrh_Ok)
{
	sm_clrh_expect(CST(&s21));

    RKH_TR_SM_CLRH(aotest, &s21);

    utrzExpect_Ok_check();
}

TEST(utrzExpect, sm_clrh_OutOfSequence)
{
	sm_clrh_expect(CST(&s21));

    RKH_TR_SM_ENSTATE(aotest, CST(&s21));

    utrzExpect_OutOfSequence_check( "CLRH", "ENSTATE" );
}

TEST(utrzExpect, sm_clrh_WithUnexpectedArg)
{
	sm_clrh_expect(CST(&s21));

    RKH_TR_SM_CLRH(aotest, &s211);
    
    utrzExpect_WithUnexpectedArg_check( "CLRH", "h", str(s21), str(s211) )
}

/* RKH_TE_SM_TRN */

TEST(utrzExpect, sm_trn_Ok)
{
	sm_trn_expect(CST(&s21), CST(&s211));
    
    RKH_TR_SM_TRN(aotest, &s21, &s211);

    utrzExpect_Ok_check();
}

TEST(utrzExpect, sm_trn_OutOfSequence)
{
	sm_trn_expect(CST(&s21), CST(&s211));

    RKH_TR_SM_ENSTATE(aotest, CST(&s21));

    utrzExpect_OutOfSequence_check( "TRN", "ENSTATE" );
}

TEST(utrzExpect, sm_trn_WithUnexpectedArg)
{
	sm_trn_expect(CST(&s21), CST(&s211));

    RKH_TR_SM_TRN(aotest, &s21, &s21);

    utrzExpect_WithUnexpectedArg_check( "TRN", "tst", str(s211), str(s21) )
}

#if 0
    /* RKH_TE_SM_STATE */
   	RUN_TEST_CASE(utrzExpect, sm_state_Ok);
	RUN_TEST_CASE(utrzExpect, sm_state_OutOfSequence);
	RUN_TEST_CASE(utrzExpect, sm_state_WithUnexpectedArg);
    /* RKH_TE_SM_ENSTATE */
	RUN_TEST_CASE(utrzExpect, sm_enstate_Ok);
	RUN_TEST_CASE(utrzExpect, sm_enstate_OutOfSequence);
	RUN_TEST_CASE(utrzExpect, sm_enstate_WithUnexpectedArg);    
    /* RKH_TE_SM_EXSTATE */
	RUN_TEST_CASE(utrzExpect, sm_exstate_Ok);
	RUN_TEST_CASE(utrzExpect, sm_exstate_OutOfSequence);
	RUN_TEST_CASE(utrzExpect, sm_exstate_WithUnexpectedArg);    
    /* RKH_TE_SM_NENEX */
	RUN_TEST_CASE(utrzExpect, sm_nenex_Ok);
	RUN_TEST_CASE(utrzExpect, sm_nenex_OutOfSequence);
	RUN_TEST_CASE(utrzExpect, sm_nenex_WithUnexpectedArg);    
    /* RKH_TE_SM_NTRNACT */
	RUN_TEST_CASE(utrzExpect, sm_ntrnact_Ok);
	RUN_TEST_CASE(utrzExpect, sm_ntrnact_OutOfSequence);
	RUN_TEST_CASE(utrzExpect, sm_ntrnact_WithUnexpectedArg);    
    /* RKH_TE_SM_TS_STATE */
	RUN_TEST_CASE(utrzExpect, sm_tsstate_Ok);
	RUN_TEST_CASE(utrzExpect, sm_tsstate_OutOfSequence);
	RUN_TEST_CASE(utrzExpect, sm_tsstate_WithUnexpectedArg);    
    /* RKH_TE_SM_EVT_PROC */
	RUN_TEST_CASE(utrzExpect, sm_evtproc_Ok);
	RUN_TEST_CASE(utrzExpect, sm_evtproc_OutOfSequence);
    /* RKH_TE_SM_EVT_NFOUND */
	RUN_TEST_CASE(utrzExpect, sm_evtnfound_Ok);
	RUN_TEST_CASE(utrzExpect, sm_evtnfound_OutOfSequence);
	RUN_TEST_CASE(utrzExpect, sm_evtnfound_WithUnexpectedArg);    
    /* RKH_TE_SM_GRD_FALSE */
	RUN_TEST_CASE(utrzExpect, sm_grdfalse_Ok);
	RUN_TEST_CASE(utrzExpect, sm_grdfalse_OutOfSequence);
    /* RKH_TE_SM_CND_NFOUND */
	RUN_TEST_CASE(utrzExpect, sm_cndnfound_Ok);
	RUN_TEST_CASE(utrzExpect, sm_cndnfound_OutOfSequence);
    /* RKH_TE_SM_UNKN_STATE */
	RUN_TEST_CASE(utrzExpect, sm_unkstate_Ok);
	RUN_TEST_CASE(utrzExpect, sm_unkstate_OutOfSequence);
    /* RKH_TE_SM_EX_HLEVEL */
	RUN_TEST_CASE(utrzExpect, sm_exhlevel_Ok);
	RUN_TEST_CASE(utrzExpect, sm_exhlevel_OutOfSequence);
    /* RKH_TE_SM_EX_TSEG */    
   	RUN_TEST_CASE(utrzExpect, sm_extseg_Ok);
	RUN_TEST_CASE(utrzExpect, sm_extseg_OutOfSequence);
#endif

/* RKH_TE_SM_EXE_ACT */

TEST(utrzExpect, sm_exeAct_Ok)
{
    UtrzProcessOut *p;

	sm_exeAct_expect(RKH_SUBTE_SM_EXE_ACT_EN,
                        CST(&s21), foo_set2zero );

    RKH_TR_SM_EXE_ACT( RKH_SUBTE_SM_EXE_ACT_EN, aotest, &s21, foo_set2zero );

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
}

TEST(utrzExpect, sm_exeAct_OutOfSequence)
{
    UtrzProcessOut *p;

	sm_trn_expect(CST(&s21), CST(&s211));

    RKH_TR_SM_ENSTATE(aotest, CST(&s21));

    p = ut_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_FAIL, p->status);
    TEST_ASSERT_EQUAL_STRING("Out of order Trace event. received: 'ENSTATE' "
                             "expected: 'TRN'.", p->msg);
}

TEST(utrzExpect, sm_exeAct_WithUnexpectedArg)
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

/* ------------------------------ End of file ------------------------------ */
