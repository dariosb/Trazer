/**
 *  \file       test_rtime.c
 *  \ingroup    test_rtime_group
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
#include "bsp.h"
#include "tzparse.h"
#include "tzlog.h"
#include "Mockerror.h"
#include "Mockseqdiag.h"
#include <time.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

TEST_GROUP(rtime);

/* ---------------------------- Local variables ---------------------------- */
static char trazerOut[50];
static RKHROM RKH_ROM_T base = {0, 0, "receiver"};
static RKHROM RKH_ROM_T base1 = {0, 0, "sender"};
static RKH_SMA_T receiver;
static RKH_SMA_T sender;
static RKH_EVT_T event;
static RKH_ST_T state;


/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
void
trazerSendSymbols( void )
{
	RKH_TR_FWK_OBJ( &receiver );
	RKH_TR_FWK_OBJ( &sender );
	RKH_TR_FWK_OBJ( &state );
    execTrazerParser();
}

/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(rtime)
{
    common_test_setup();
    receiver.sm.romrkh = &base;
    sender.sm.romrkh = &base1;
    event.e = 3;
    event.pool = 5;
    event.nref = 7;
    state.base.name = "state";

    trazerSendSymbols();

    initTesttzlog();
}

TEST_TEAR_DOWN(rtime)
{
    common_tear_down();
}

TEST(rtime, normalFifo)
{
    rui8_t nElem, nMin;
    EVENT_ST evt;

    nElem = 4;
    nMin = 2;

    evt.sobj = (ulong)(&sender);
    evt.tobj = (ulong)(&receiver);
    evt.e = event.e;

    bsp_set_ts( 0 );
    RKH_TR_SMA_FIFO(&receiver, &event, &sender, event.pool, event.nref, nElem, 
                    nMin);

    sdiag_async_evt_Expect(&evt);

    trazerOutExpect(trazerOut, 3, "SMA", "FIFO", "ao=receiver, sig=SIGNAL, "
                               "snr=sender, pid=5, rc=7, nelem=4, nmin=2, nseq=1");
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );

    initTesttzlog();
    bsp_set_ts( 10 );

  	RKH_TR_SMA_GET(&receiver, &event, event.pool, event.nref, nElem, nMin);

    trazerOutExpect(trazerOut, 4, "SMA", "GET", "ao=receiver, sig=SIGNAL, pid=5, "
                                            "rc=7, nelem=4, nmin=2, rt=10, nseq=1");
    execTrazerParser();
    
    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(rtime, normalLifo)
{
    rui8_t nElem, nMin;
    EVENT_ST evt;

    nElem = 4;
    nMin = 2;

    evt.sobj = (ulong)(&sender);
    evt.tobj = (ulong)(&receiver);
    evt.e = event.e;

    bsp_set_ts( 0 );
    RKH_TR_SMA_LIFO(&receiver, &event, &sender, event.pool, event.nref, nElem, 
                    nMin);

    sdiag_async_evt_Expect(&evt);

    trazerOutExpect(trazerOut, 3, "SMA", "LIFO", "ao=receiver, sig=SIGNAL, "
                               "snr=sender, pid=5, rc=7, nelem=4, nmin=2, nseq=1");
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );

    initTesttzlog();
    bsp_set_ts( 10 );

  	RKH_TR_SMA_GET(&receiver, &event, event.pool, event.nref, nElem, nMin);

    trazerOutExpect(trazerOut, 4, "SMA", "GET", "ao=receiver, sig=SIGNAL, pid=5, "
                                            "rc=7, nelem=4, nmin=2, rt=10, nseq=1");
    execTrazerParser();
    
    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(rtime, getWoutPut)
{
    rui8_t nElem, nMin;
    EVENT_ST evt;

    nElem = 4;
    nMin = 2;

    evt.sobj = (ulong)(&sender);
    evt.e = event.e;

  	RKH_TR_SMA_GET(&receiver, &event, event.pool, event.nref, nElem, nMin);

    trazerOutExpect(trazerOut, 3, "SMA", "GET", "ao=receiver, sig=SIGNAL, pid=5, "
                                            "rc=7, nelem=4, nmin=2, rt=-1, nseq=0");
    execTrazerParser();
    
    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(rtime, traceStreamLost)
{
    char lostStream_txt[500] = "\n***** May be have lost trace info, "
                            "sequence are not correlatives\n";
    rui8_t nElem, nMin;
    EVENT_ST evt;

    nElem = 4;
    nMin = 2;

    evt.sobj = (ulong)(&sender);
    evt.tobj = (ulong)(&receiver);
    evt.e = event.e;

    bsp_set_ts( 0 );
    RKH_TR_SMA_FIFO(&receiver, &event, &sender, event.pool, event.nref, nElem, 
                    nMin);

    sdiag_async_evt_Expect(&evt);

    trazerOutExpect(trazerOut, 3, "SMA", "FIFO", "ao=receiver, sig=SIGNAL, "
                               "snr=sender, pid=5, rc=7, nelem=4, nmin=2, nseq=1");
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );

    initTesttzlog();
	rkh_trc_init();

    bsp_set_ts( 10 );

  	RKH_TR_SMA_GET(&receiver, &event, event.pool, event.nref, nElem, nMin);

    trazerOutExpect(trazerOut, 0, "SMA", "GET", "ao=receiver, sig=SIGNAL, pid=5, "
                                            "rc=7, nelem=4, nmin=2, rt=-1, nseq=0");
    strcat(lostStream_txt, trazerOut);
    execTrazerParser();
    
    TEST_ASSERT_EQUAL_STRING( lostStream_txt, lprintf_Buff );

}


TEST(rtime, getSameEvtFromOtherAO)
{
    rui8_t nElem, nMin;
    EVENT_ST evt;

    nElem = 4;
    nMin = 2;

    evt.sobj = (ulong)(&sender);
    evt.tobj = (ulong)(&receiver);
    evt.e = event.e;

    bsp_set_ts( 0 );
    RKH_TR_SMA_FIFO(&receiver, &event, &sender, event.pool, event.nref, nElem, 
                    nMin);

    sdiag_async_evt_Expect(&evt);

    trazerOutExpect(trazerOut, 3, "SMA", "FIFO", "ao=receiver, sig=SIGNAL, "
                               "snr=sender, pid=5, rc=7, nelem=4, nmin=2, nseq=1");
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );

    initTesttzlog();
    bsp_set_ts( 10 );

  	RKH_TR_SMA_GET(&sender, &event, event.pool, event.nref, nElem, nMin);

    trazerOutExpect(trazerOut, 4, "SMA", "GET", "ao=sender, sig=SIGNAL, pid=5, "
                                            "rc=7, nelem=4, nmin=2, rt=-1, nseq=0");
    execTrazerParser();
    
    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
