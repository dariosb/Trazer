/**
 *  \file       test_log.c
 *  \ingroup    test_log_group
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
#include "log.h"
#include <time.h>

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */
#define SIGNAL  0x3
#define USERT   RKH_TE_USER+1

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

TEST_GROUP(log);

/* ---------------------------- Local variables ---------------------------- */
static char trazerOut[50];
static RKH_RQ_T rq;
static RKH_TMR_T tmr;
static RKHROM RKH_ROM_T base = {0, 0, "receiver"};
static RKH_SMA_T receiver;
static RKH_SMA_T sender;
static RKH_EVT_T event;
static RKH_ST_T state;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static void dummyFunc(void){}

/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(log)
{
    receiver.sm.romrkh = &base;
    event.e = SIGNAL;
    event.pool = 5;
    event.nref = 7;
    state.base.name = "state";
    tmr.evt = &event;

    initTesttzlog();
}

TEST_TEAR_DOWN(log)
{
    common_tear_down();
}


TEST(log, init)
{
   	rkh_fwk_init();
	
	rkh_trc_init();

	RKH_FILTER_OFF_EVENT(RKH_TRC_ALL_EVENTS);
	RKH_FILTER_OFF_ALL_SMA();
	RKH_FILTER_OFF_ALL_SIGNALS();
    Mockseqdiag_Init();
    Mockerror_Init();

	tzparser_init();

    logOpen();

	RKH_TRC_SEND_CFG(CLOCKS_PER_SEC);
    sdiag_text_Expect(MSC_TARGET_START);
    execTrazerParser();

	RKH_TR_FWK_OBJ( &receiver );
	RKH_TR_FWK_OBJ( &sender );
    execTrazerParser();
	RKH_TR_FWK_OBJ( &state );
	RKH_TR_FWK_OBJ( &tmr );
    execTrazerParser();
  	RKH_TR_FWK_SIG( SIGNAL );
}

TEST(log, close)
{
    logClose();
}

TEST(log, rq)
{
	RKH_TR_RQ_INIT(&rq, &receiver, 40);
    execTrazerParser();
	RKH_TR_RQ_GET(&rq, 40);
    execTrazerParser();
	RKH_TR_RQ_FIFO(&rq, 40, 20);
    execTrazerParser();
	RKH_TR_RQ_LIFO(&rq, 40, 20);
    execTrazerParser();
	RKH_TR_RQ_FULL(&rq);
    execTrazerParser();
	RKH_TR_RQ_DPT(&rq);
    execTrazerParser();
	RKH_TR_RQ_GET_LAST(&rq);
    execTrazerParser();
}

TEST(log, sma)
{
    rui8_t nElem, nMin;
    EVENT_ST evt;

    nElem = 4;
    nMin = 2;
    evt.sobj = (ulong)(&sender);
    evt.tobj = (ulong)(&receiver);
    evt.e = event.e;

	RKH_TR_SMA_ACT(&receiver, RKH_GET_PRIO(&receiver), 16);
    execTrazerParser();
    RKH_TR_SMA_TERM(&receiver, RKH_GET_PRIO(&receiver));
    execTrazerParser();
  	RKH_TR_SMA_GET(&receiver, &event, event.pool, event.nref, nElem, nMin);
    execTrazerParser();
    RKH_TR_SMA_FIFO(&receiver, &event, &sender, event.pool, event.nref, nElem, 
                    nMin);
    sdiag_async_evt_Expect(&evt);
    execTrazerParser();
    RKH_TR_SMA_LIFO(&receiver, &event, &sender, event.pool, event.nref, nElem, 
                    nMin);
    sdiag_async_evt_Expect(&evt);
    execTrazerParser();
    RKH_TR_SMA_REG(&receiver, RKH_GET_PRIO(&receiver));
    execTrazerParser();
	RKH_TR_SMA_UNREG(&receiver, RKH_GET_PRIO(&receiver));
    execTrazerParser();
    RKH_TR_SMA_DCH(&receiver, &event, &state);
    execTrazerParser();
}

TEST(log, sm)
{
    sdiag_state_Expect((ulong)&receiver, (ulong)&state);
	RKH_TR_SM_INIT(&receiver, &state);
    execTrazerParser();
	RKH_TR_SM_CLRH(&receiver, &state);
    execTrazerParser();
	RKH_TR_SM_TRN(&receiver, &state, &state);
    execTrazerParser();
	RKH_TR_SM_STATE(&receiver, &state);
    execTrazerParser();
	RKH_TR_SM_ENSTATE(&receiver, &state);
    execTrazerParser();
	RKH_TR_SM_EXSTATE(&receiver, &state);
    execTrazerParser();
	RKH_TR_SM_NENEX(&receiver, 4, 8);
    execTrazerParser();
	RKH_TR_SM_NTRNACT(&receiver, 4, 8);
    execTrazerParser();
	RKH_TR_SM_TS_STATE(&receiver, &state);
    execTrazerParser();
	RKH_TR_SM_EVT_PROC(&receiver);
    execTrazerParser();
	RKH_TR_SM_EVT_NFOUND(&receiver, &event);
    execTrazerParser();
	RKH_TR_SM_GRD_FALSE(&receiver);
    execTrazerParser();
	RKH_TR_SM_CND_NFOUND(&receiver);
    execTrazerParser();
	RKH_TR_SM_UNKN_STATE(&receiver);
    execTrazerParser();
	RKH_TR_SM_EX_HLEVEL(&receiver);
    execTrazerParser();
	RKH_TR_SM_EX_TSEG(&receiver);
    execTrazerParser();
	RKH_TR_SM_EXE_ACT(0, &receiver, &state, 0x5A5A5A);
    sdiag_exec_act_Expect((ulong)&receiver, (ulong)0x5A5A5A);
    execTrazerParser();
}

TEST(log, tim)
{
	RKH_TR_TMR_INIT(&tmr, 0x03);
    execTrazerParser();
	RKH_TR_TMR_START(&tmr, &receiver, 20, 10);
    execTrazerParser();
	RKH_TR_TMR_STOP(&tmr, 20, 10);
    execTrazerParser();
	RKH_TR_TMR_TOUT(&tmr, tmr.evt->e, &receiver);
    sdiag_tmrevt_Expect((ulong)&tmr);
    execTrazerParser();
	RKH_TR_TMR_REM(&tmr);
    execTrazerParser();
}

TEST(log, fwk)
{
	RKH_TR_FWK_EN();
    execTrazerParser();
	RKH_TR_FWK_EX();
    execTrazerParser();
	RKH_TR_FWK_EPREG(2, 40, 100, 200);
    execTrazerParser();
	RKH_TR_FWK_AE(2, &event, 1, 2, &receiver);
    execTrazerParser();
	RKH_TR_FWK_GC(&event, event.pool, 2);
    execTrazerParser();
	RKH_TR_FWK_GCR(&event, 1, 2, &receiver);
    execTrazerParser();
	RKH_TR_FWK_DEFER(&rq, &event);
    execTrazerParser();
	RKH_TR_FWK_RCALL(&receiver, &event);
    execTrazerParser();
	RKH_TR_FWK_OBJ(&receiver);
    execTrazerParser();
	RKH_TR_FWK_SIG(SIGNAL);
    execTrazerParser();
	RKH_TR_FWK_FUN(&dummyFunc);
    execTrazerParser();
	RKH_TR_FWK_TUSR(USERT);
    execTrazerParser();
	RKH_TR_FWK_SYNC_EVT(dummyFunc, &sender, &receiver);
    sdiag_sync_Expect((ulong)&dummyFunc, (ulong)&sender, (ulong)&receiver);
    execTrazerParser();
	RKH_TR_FWK_ASSERT("file", 20);
    execTrazerParser();
	RKH_TR_FWK_AO(&receiver);
    execTrazerParser();
	RKH_TR_FWK_STATE(&receiver, &state);
    execTrazerParser();
	RKH_TR_FWK_PSTATE(&receiver, &state);
    execTrazerParser();
	RKH_TR_FWK_TIMER(&tmr);
    execTrazerParser();
	RKH_TR_FWK_EPOOL(1, "ep0");
    execTrazerParser();
	RKH_TR_FWK_QUEUE(&rq);
    execTrazerParser();
	RKH_TR_FWK_ACTOR(&receiver, "receiver");
    execTrazerParser();
}
