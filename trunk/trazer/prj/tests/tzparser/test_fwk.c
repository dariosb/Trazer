/**
 *  \file       test_fwk.c
 *  \ingroup    test_fwk
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
#define SIGNAL  0x3
#define USERT   RKH_TE_USER+1

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

TEST_GROUP(fwk);

/* ---------------------------- Local variables ---------------------------- */
static char nseq;
static char trazerOut[50];
static RKH_SMA_T receiver;
static RKH_SMA_T sender;
static RKH_EVT_T event;
static RKHROM RKH_ROM_T base = {0, 0, "receiver"};
static RKHROM RKH_ROM_T base1 = {0, 0, "sender"};
static RKH_RQ_T rq;
static RKH_ST_T st = {{RKH_BASIC, "state"}};;
static RKH_ST_T pst = {{RKH_CHOICE, "pseudoState"}};;
static RKH_TMR_T tim;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */

static void dummyFunc(void){}

static
void
trazerSendSymbols( void )
{
    RKH_TR_FWK_OBJ( &receiver );
    ++nseq;
	RKH_TR_FWK_OBJ( &sender );
    ++nseq;
	RKH_TR_FWK_OBJ( &rq );
    ++nseq;
	RKH_TR_FWK_OBJ( &dummyFunc );
    ++nseq;
	RKH_TR_FWK_OBJ( &st );
    ++nseq;

    execTrazerParser();
}

/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(fwk)
{
    event.e = SIGNAL;
    event.pool = 5;
    event.nref = 7;
    receiver.sm.romrkh = &base;
    sender.sm.romrkh = &base1;

	nseq = 0;
    common_test_setup();
    trazerSendSymbols();

    initTesttzlog();
}

TEST_TEAR_DOWN(fwk)
{
    common_tear_down();
}

TEST(fwk, en)
{
	RKH_TR_FWK_EN();

    trazerOutExpect(trazerOut, nseq, "FWK", "EN", " ");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, ex)
{
	RKH_TR_FWK_EX();

    trazerOutExpect(trazerOut, nseq, "FWK", "EX", " ");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, epreg)
{
	RKH_TR_FWK_EPREG(2, 40, 100, 200);

    trazerOutExpect(trazerOut, nseq, "FWK", "EPREG", "ep=2, ss=40, "
                                  "es=100, ps=200");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, ae)
{
	RKH_TR_FWK_AE(2, &event, 1, 2, &receiver);

    trazerOutExpect(trazerOut, nseq, "FWK", "AE", "es=2, sig=0X3, pid=5, "
                                  "rc=7, nb=1, nmin=2, actor=receiver");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, gc)
{
	RKH_TR_FWK_GC(&event, event.pool, 2);

    trazerOutExpect(trazerOut, nseq, "FWK", "GC", "sig=0X3, pid=5, rc=2");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, gcr)
{
	RKH_TR_FWK_GCR(&event, 1, 2, &receiver);

    trazerOutExpect(trazerOut, nseq, "FWK", "GCR", "sig=0X3, pid=5, "
                                  "rc=7, nb=1, nmin=2, actor=receiver");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, obj)
{
    char args[40];

	RKH_TR_FWK_OBJ(&receiver);

    sprintf(args, "obj=0x%08X, nm=%s", &receiver, "receiver");

    trazerOutExpect(trazerOut, nseq, "FWK", "OBJ", args);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, sig)
{
	RKH_TR_FWK_SIG(SIGNAL);

    trazerOutExpect(trazerOut, nseq, "FWK", "SIG", "sig=3, nm=SIGNAL");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, fun)
{
    char args[40];

	RKH_TR_FWK_FUN(&dummyFunc);

    sprintf(args, "fn=0x%08X, nm=%s", &dummyFunc, "dummyFunc");

    trazerOutExpect(trazerOut, nseq, "FWK", "FUN", args);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}


TEST(fwk, tusr)
{
    char args[40];

	RKH_TR_FWK_TUSR(USERT);

    sprintf(args, "ut=%u, nm=%s", USERT-RKH_TE_USER, "USERT");

    trazerOutExpect(trazerOut, nseq, "FWK", "TUSR", args);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, syncevt)
{
	RKH_TR_FWK_SYNC_EVT(dummyFunc, &sender, &receiver);

    trazerOutExpect(trazerOut, nseq, "FWK", "SYNC_EVT", "fn=dummyFunc, "
                                     "snr=sender, ao=receiver");
    
    sdiag_sync_Expect((ulong)&dummyFunc, (ulong)&sender, (ulong)&receiver);
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, cfg)
{
    /*
     * It is not verified the answer.
     */
	RKH_TR_FWK_TCFG(CLOCKS_PER_SEC);
    sdiag_text_Expect(MSC_TARGET_START);

    execTrazerParser();
}

TEST(fwk, assert)
{
	RKH_TR_FWK_ASSERT("file", 20);

    trazerOutExpect(trazerOut, nseq, "FWK", "ASSERT", "(20), file.c");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, ao)
{
    char args[40];

	RKH_TR_FWK_AO(&receiver);

    sprintf(args, "obj=0x%08X, nm=%s", &receiver, "receiver");

    trazerOutExpect(trazerOut, nseq, "FWK", "AO", args);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, state)
{
    char args[40];

	RKH_TR_FWK_STATE(&receiver, &st);

    sprintf(args, "ao=receiver, obj=0x%08X, nm=%s", &st, "state");

    trazerOutExpect(trazerOut, nseq, "FWK", "STATE", args);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, pstate)
{
    char args[50];

	RKH_TR_FWK_PSTATE(&receiver, &pst);

    sprintf(args, "ao=receiver, obj=0x%08X, nm=%s", &pst, "pseudoState");

		trazerOutExpect(trazerOut, nseq, "FWK", "PSTATE", args);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, timer)
{
    char args[40];

	RKH_TR_FWK_TIMER(&tim);

    sprintf(args, "obj=0x%08X, nm=%s", &tim, "tim");

    trazerOutExpect(trazerOut, nseq, "FWK", "TIMER", args);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, epool)
{
	RKH_TR_FWK_EPOOL(1, "ep0");

    trazerOutExpect(trazerOut, nseq, "FWK", "EPOOL", "ep=1, nm=ep0");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, queue)
{
    char args[40];

	RKH_TR_FWK_QUEUE(&rq);

    sprintf(args, "obj=0x%08X, nm=%s", &rq, "rq");

    trazerOutExpect(trazerOut, nseq, "FWK", "QUEUE", args);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(fwk, actor)
{
    char args[40];

	RKH_TR_FWK_ACTOR(&receiver, "receiver");

    sprintf(args, "obj=0x%08X, nm=%s", &receiver, "receiver");

    trazerOutExpect(trazerOut, nseq, "FWK", "ACTOR", args);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
