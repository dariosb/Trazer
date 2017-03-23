/**
 *  \file       test_tim.c
 *  \ingroup    test_tim
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

TEST_GROUP(tim);

/* ---------------------------- Local variables ---------------------------- */
static char trazerOut[50];
static RKH_TMR_T tmr;
static RKH_SMA_T receiver;
static RKH_EVT_T event;
static RKHROM RKH_ROM_T base = {0, 0, "receiver"};

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
void
trazerSendSymbols( void )
{
	RKH_TR_FWK_OBJ( &receiver );
	RKH_TR_FWK_OBJ( &tmr );
    execTrazerParser();
}

/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(tim)
{
    event.e = 3;
    event.pool = 5;
    event.nref = 7;
    receiver.sm.romrkh = &base;
    tmr.evt = &event;

    common_test_setup();
    trazerSendSymbols();

    initTesttzlog();
}

TEST_TEAR_DOWN(tim)
{
    common_tear_down();
}

TEST(tim, init)
{
	RKH_TR_TMR_INIT(&tmr, 0x03);

    trazerOutExpect(trazerOut, 2, "TIM", "INIT", "t=tmr, sig=0X3");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(tim, start)
{
	RKH_TR_TMR_START(&tmr, &receiver, 20, 10);

    trazerOutExpect(trazerOut, 2, "TIM", "START", "t=tmr, ao=receiver, "
                                  "nt=20, per=10");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(tim, stop)
{
	RKH_TR_TMR_STOP(&tmr, 20, 10);

    trazerOutExpect(trazerOut, 2, "TIM", "STOP", "t=tmr, nt=20, per=10");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(tim, tout)
{
	RKH_TR_TMR_TOUT(&tmr, tmr.evt->e, &receiver);

    trazerOutExpect(trazerOut, 2, "TIM", "TOUT", "t=tmr, sig=0X3, ao=receiver");

    sdiag_tmrevt_Expect((ulong)&tmr);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(tim, rem)
{
	RKH_TR_TMR_REM(&tmr);

    trazerOutExpect(trazerOut, 2, "TIM", "REM", "t=tmr");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
