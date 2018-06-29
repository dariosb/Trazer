/**
 *  \file       test_rq.c
 *  \ingroup    test_rq
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
 *  2015.11.11  DaBa  v1.0.00  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  DaBa  Dario Baliña  dariosb@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */

#include "unity_fixture.h"
#include "test_common.h"
#include "rkh.h"
#include "tzparse.h"
#include "tzlog.h"
#include "Mockerror.h"
#include "Mockseqdiag.h"
#include <time.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

TEST_GROUP(rq);

/* ---------------------------- Local variables ---------------------------- */
static char trazerOut[50];
static RKH_RQ_T rq;
static RKH_SMA_T receiver;
static RKHROM RKH_ROM_T base = {0, 0, "receiver"};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
void
trazerSendSymbols( void )
{
	RKH_TR_FWK_OBJ( &receiver );
	RKH_TR_FWK_OBJ( &rq );
    execTrazerParser();
}

/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(rq)
{
    receiver.sm.romrkh = &base;

    common_test_setup();
    trazerSendSymbols();

    initTesttzlog();
}

TEST_TEAR_DOWN(rq)
{
    common_tear_down();
}

TEST(rq, init)
{
	RKH_TR_RQ_INIT(&rq, &receiver, 40);

    trazerOutExpect(trazerOut, 2, "RQ", "INIT", "q=rq, ao=receiver, nelem=40");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(rq, get)
{
	RKH_TR_RQ_GET(&rq, 40);

    trazerOutExpect(trazerOut, 2, "RQ", "GET", "q=rq, nelem=40");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(rq, fifo)
{
	RKH_TR_RQ_FIFO(&rq, 40, 20);

    trazerOutExpect(trazerOut, 2, "RQ", "FIFO", "q=rq, nelem=40, nmin=20");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(rq, lifo)
{
	RKH_TR_RQ_LIFO(&rq, 40, 20);

    trazerOutExpect(trazerOut, 2, "RQ", "LIFO", "q=rq, nelem=40, nmin=20");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(rq, full)
{
	RKH_TR_RQ_FULL(&rq);

    trazerOutExpect(trazerOut, 2, "RQ", "FULL", "q=rq");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(rq, dpt)
{
	RKH_TR_RQ_DPT(&rq);

    trazerOutExpect(trazerOut, 2, "RQ", "DPT", "q=rq");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(rq, getlast)
{
	RKH_TR_RQ_GET_LAST(&rq);

    trazerOutExpect(trazerOut, 2, "RQ", "GET_LAST", "q=rq");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
