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
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(utrzsm)
{
    /* -------- Setup ---------------
     * Establish the preconditions to the test 
     */
    unitrazer_init();
    sm_ignore();        /* Ignore every trace event of SM group */

    RKH_TR_FWK_AO(blinky);
    RKH_TR_FWK_STATE(blinky, &s0);
    RKH_TR_FWK_STATE(blinky, &s1);
    RKH_TR_FWK_SIG(evAlpha);

    rkh_sma_init_hsm(aoTest);
}

TEST_TEAR_DOWN(utrzsm)
{
    /* -------- Cleanup -------------
     * Return the system under test to its initial state after the test
     */
    unitrazer_cleanup();
    unitrazer_verify(); /* Makes sure there are no unused expectations, if */
                        /* there are, this function causes the test to fail. */
}

TEST(utrzsm, simpleTransition)
{
    ruint ret;

    /* -------- Expectations --------
     * Record the trace event expectations to be met
     */
    sm_trn_expect(&s0, &s1);

    /* -------- Exercise ------------ 
     * Do something to the system 
     */

    /* Each recorded trace event is checked to see that it matches */
    /* the expected trace event exactly. If calls are out of order or */
    /* parameters are wrong, the test immediately fails. */
    ret = rkh_sma_dispatch(aoTest, evAlpha);

    /* -------- Verify --------------
     * Check the expected outcome 
     */
    TEST_ASSERT_EQUAL(RKH_EVT_PROC, ret);
}

/* ------------------------------ End of file ------------------------------ */
