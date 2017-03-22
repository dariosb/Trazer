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
#include "test_common.h"
#include "rkh.h"
#include "tzparse.h"
#include "Mockerror.h"
#include "Mockseqdiag.h"
#include "tzlog.h"
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

static
void
execTrazerParser( void )
{
	TRCQTY_T trcSize, i;
    rui8_t *ptrc, *p;

    trcSize = getTraceStream(&ptrc);
    TEST_ASSERT_NOT_EQUAL(0, trcSize);
    TEST_ASSERT_NOT_NULL(ptrc);

    for(i=0, p=ptrc; i<trcSize; ++i, ++ptrc)
        tzparser_exec(*ptrc);
}

static
void
trazerInitConfiguration( void )
{
	RKH_TRC_SEND_CFG(CLOCKS_PER_SEC);
    sdiag_text_Expect(MSC_TARGET_START);
    execTrazerParser();
}

static
void
trazerSendSymbols( void )
{
	RKH_TR_FWK_OBJ( &receiver );
	RKH_TR_FWK_OBJ( &sender );
	RKH_TR_FWK_OBJ( &state );
    execTrazerParser();
}

void
trazerOutExpect( char *p, ushort nseq, char *group, char *evt, char *args)
{
    sprintf( p, TRAZER_FMT_TSTAMP_32BIT TRAZER_FMT_NSEQ TRAZER_FMT_TRACE_EVT "%s\n", 
                            rkh_trc_getts(), nseq, group, evt, args );
}


/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(sma)
{
    /* -------- Setup ---------------
     * Establish the preconditions to the test 
     */
    common_test_setup();
    receiver.sm.romrkh = &base;
    event.e = 3;
    event.pool = 5;
    event.nref = 7;

	rkh_fwk_init();
	
	rkh_trc_init();

	RKH_FILTER_OFF_EVENT(RKH_TRC_ALL_EVENTS);
	RKH_FILTER_OFF_ALL_SMA();
	RKH_FILTER_OFF_ALL_SIGNALS();

    Mockseqdiag_Init();
    Mockerror_Init();

	tzparser_init();

    trazerInitConfiguration();
    trazerSendSymbols();

    initTesttzlog();
}

TEST_TEAR_DOWN(sma)
{
    /* -------- Cleanup -------------
     * Return the system under test to its initial state after the test
     */
    common_tear_down();

    Mockseqdiag_Verify();
    Mockseqdiag_Destroy();

    Mockerror_Verify();
    Mockerror_Destroy();
}

TEST(sma, act)
{
	RKH_TR_SMA_ACT(&receiver, RKH_GET_PRIO(&receiver), 16);

    trazerOutExpect(trazerOut, 3, "SMA", "ACT", "ao=receiver, prio=0, size=16");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, term)
{
    RKH_TR_SMA_TERM(&receiver, RKH_GET_PRIO(&receiver));

    trazerOutExpect(trazerOut, 3, "SMA", "TERM", "ao=receiver, prio=0");

    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, get)
{
    rui8_t nElem, nMin;

    nElem = 4;
    nMin = 2;

  	RKH_TR_SMA_GET(&receiver, &event, event.pool, event.nref, nElem, nMin);

    trazerOutExpect(trazerOut, 3, "SMA", "GET", "ao=receiver, sig=0X3, pid=5, "
                                            "rc=7, nelem=4, nmin=2");
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

    trazerOutExpect(trazerOut, 3, "SMA", "FIFO", "ao=receiver, sig=0X3, "
                               "snr=sender, pid=5, rc=7, nelem=4, nmin=2");
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

    trazerOutExpect(trazerOut, 3, "SMA", "LIFO", "ao=receiver, sig=0X3, "
                               "snr=sender, pid=5, rc=7, nelem=4, nmin=2");
    execTrazerParser();
    
    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, reg)
{
    RKH_TR_SMA_REG(&receiver, RKH_GET_PRIO(&receiver));

    trazerOutExpect(trazerOut, 3, "SMA", "REG", "ao=receiver, prio=0");

    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, unreg)
{
	RKH_TR_SMA_UNREG(&receiver, RKH_GET_PRIO(&receiver));

    trazerOutExpect(trazerOut, 3, "SMA", "UNREG", "ao=receiver, prio=0");

    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sma, dch)
{
    state.base.name = "state";
    event.e = 3;
    event.pool = 5;
    event.nref = 7;

    RKH_TR_SMA_DCH(&receiver, &event, &state);

    trazerOutExpect(trazerOut, 3, "SMA", "DCH", "ao=receiver, sig=0X3, "
                                  "st=state, rt=-1");

    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
