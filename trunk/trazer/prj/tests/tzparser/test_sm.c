/**
 *  \file       test_sm.c
 *  \ingroup    test_utrz
 *  \ingroup    test_utrzsm_group
 *  \brief      Unit test for uTrazer module - State machine test group
 *
 *  \addtogroup test
 *  @{
 *  \addtogroup test_utrz UTrazer
 *  @{
 *  \brief      Unit test for uTrazer module
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
#include "test_common.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

TEST_GROUP(sm);

/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(sm)
{
    /* -------- Setup ---------------
     * Establish the preconditions to the test 
     */
    common_test_setup();
}

TEST_TEAR_DOWN(sm)
{
    /* -------- Cleanup -------------
     * Return the system under test to its initial state after the test
     */
    common_tear_down();
}

/**
 *  \addtogroup test_utrzsm_group State machine test group
 *  @{
 *  \name Test cases of State machine test group
 *  @{ 
 */

TEST(sm, expectEventOk)
{
#if 0
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
    p = unitrazer_getLastOut();
    TEST_ASSERT_EQUAL(UT_PROC_SUCCESS, p->status);
#endif
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
