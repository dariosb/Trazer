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

TEST_GROUP(sma);

/* ---------------------------- Local variables ---------------------------- */
static char trazerOut[50];
static RKHROM RKH_ROM_T base = {0, 0, "receiver"};
static RKH_SMA_T receiver;
static RKH_SMA_T sender;
static RKH_EVT_T event;
static RKH_ST_T state;
static RKH_RQ_T rq;


/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
void
trazerSendSymbols( void )
{
	RKH_TR_FWK_OBJ( &receiver );
	RKH_TR_FWK_OBJ( &sender );
	RKH_TR_FWK_OBJ( &state );
	RKH_TR_FWK_OBJ( &rq );
    execTrazerParser();
}

/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(sma)
{
    common_test_setup();
    receiver.sm.romrkh = &base;
    event.e = 3;
    event.pool = 5;
    event.nref = 7;
    state.base.name = "state";

    trazerSendSymbols();

    initTesttzlog();
}

TEST_TEAR_DOWN(sma)
{
    common_tear_down();
}

TEST(sma, act)
{
	RKH_TR_SMA_ACT(&receiver, RKH_GET_PRIO(&receiver), 16);

    trazerOutExpect(trazerOut, 4, "SMA", "ACT", "ao=receiver, prio=0, size=16");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, term)
{
    RKH_TR_SMA_TERM(&receiver, RKH_GET_PRIO(&receiver));

    trazerOutExpect(trazerOut, 4, "SMA", "TERM", "ao=receiver, prio=0");

    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, get)
{
    rui8_t nElem, nMin;

    nElem = 4;
    nMin = 2;

  	RKH_TR_SMA_GET(&receiver, &event, event.pool, event.nref, nElem, nMin);

    trazerOutExpect(trazerOut, 4, "SMA", "GET", "ao=receiver, sig=0X3, pid=5, "
                                            "rc=7, nelem=4, nmin=2, rt=-1, nseq=0");
    execTrazerParser();
    
    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}


TEST(sma, fifo)
{
    rui8_t nElem, nMin;
    EVENT_ST evt;

    nElem = 4;
    nMin = 2;

    evt.sobj = (ulong)(&sender);
    evt.tobj = (ulong)(&receiver);
    evt.e = event.e;

    RKH_TR_SMA_FIFO(&receiver, &event, &sender, event.pool, event.nref, nElem, 
                    nMin);

    sdiag_async_evt_Expect(&evt);

    trazerOutExpect(trazerOut, 4, "SMA", "FIFO", "ao=receiver, sig=0X3, "
                               "snr=sender, pid=5, rc=7, nelem=4, nmin=2, nseq=1");
    execTrazerParser();
    
    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}


TEST(sma, lifo)
{
    rui8_t nElem, nMin;
    EVENT_ST evt;

    nElem = 4;
    nMin = 2;

    evt.sobj = (ulong)(&sender);
    evt.tobj = (ulong)(&receiver);
    evt.e = event.e;

    RKH_TR_SMA_LIFO(&receiver, &event, &sender, event.pool, event.nref, nElem, 
                    nMin);

    sdiag_async_evt_Expect(&evt);

    trazerOutExpect(trazerOut, 4, "SMA", "LIFO", "ao=receiver, sig=0X3, "
                               "snr=sender, pid=5, rc=7, nelem=4, nmin=2, nseq=1");
    execTrazerParser();
    
    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, reg)
{
    RKH_TR_SMA_REG(&receiver, RKH_GET_PRIO(&receiver));

    trazerOutExpect(trazerOut, 4, "SMA", "REG", "ao=receiver, prio=0");

    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, unreg)
{
	RKH_TR_SMA_UNREG(&receiver, RKH_GET_PRIO(&receiver));

    trazerOutExpect(trazerOut, 4, "SMA", "UNREG", "ao=receiver, prio=0");

    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, defer)
{
	RKH_TR_FWK_DEFER(&rq, &event);

    trazerOutExpect(trazerOut, 4, "SMA", "DEFER", "q=rq, sig=0X3");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, rcall)
{
	RKH_TR_FWK_RCALL(&receiver, &event);

    trazerOutExpect(trazerOut, 4, "SMA", "RCALL", "ao=receiver, sig=0X3");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
