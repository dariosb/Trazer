/**
 *  \file       test_sma.c
 *  \ingroup    test_sma_group
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
#include "rkhtrc.h"
#include "test_common.h"
#include "tzparse.h"
#include "Mocktzlog.h"
#include "Mockerror.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

TEST_GROUP(sma);

/* ---------------------------- Local variables ---------------------------- */
static RKHROM RKH_ROM_T base = {0, 0, "receiver"};
static RKH_SMA_T receiver;
static RKH_SMA_T sender;
static RKH_EVT_T event;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
TRCQTY_T
getTraceStream(rui8_t **p)
{
    rui8_t *blk;
    TRCQTY_T nbytes;

    FOREVER
    {
        nbytes = (TRCQTY_T)1024;
        blk = rkh_trc_get_block(&nbytes);

        if ((blk != (rui8_t *)0))
        {
			*p = blk;
            return nbytes;
        }
        else
        {
			return 0;
        }
    }
}

/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(sma)
{
    /* -------- Setup ---------------
     * Establish the preconditions to the test 
     */
    common_test_setup();

	rkh_fwk_init();
	
	RKH_FILTER_OFF_GROUP_ALL_EVENTS( RKH_TG_SMA );

	rkh_trc_init();

	tzparser_init();

    Mocktzlog_Init();
	
}

TEST_TEAR_DOWN(sma)
{
    /* -------- Cleanup -------------
     * Return the system under test to its initial state after the test
     */
    common_tear_down();
    Mocktzlog_Verify();
    Mocktzlog_Destroy();
}

/**
 *  \addtogroup test_utrzsm_group State machine test group
 *  @{
 *  \name Test cases of State machine test group
 *  @{ 
 */

TEST(sma, act)
{
	TRCQTY_T trcSize, i;
    rui8_t *ptrc, *p;

	RKH_TR_SMA_ACT(&receiver, RKH_GET_PRIO(&receiver), 16);

    trcSize = getTraceStream(&ptrc);
    TEST_ASSERT_NOT_EQUAL(0, trcSize);
    TEST_ASSERT_NOT_NULL(ptrc);
    
    for(i=0, p=ptrc; i<trcSize; ++i, ++ptrc)
        tzparser_exec(*ptrc);

//    TEST_ASSERT_EQUAL_STRING()
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
