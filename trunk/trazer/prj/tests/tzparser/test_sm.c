/**
 *  \file       test_sm.c
 *  \ingroup    test_sm_group
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

TEST_GROUP(sm);

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
void
trazerSendSymbols( void )
{
	RKH_TR_FWK_OBJ( &receiver );
	RKH_TR_FWK_OBJ( &sender );
	RKH_TR_FWK_OBJ( &state );
    execTrazerParser();
}

/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(sm)
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

TEST_TEAR_DOWN(sm)
{
    common_tear_down();
}

TEST(sm, init)
{
	RKH_TR_SM_INIT(&receiver, &state);

    trazerOutExpect(trazerOut, 3, "SM", "INIT", "ao=receiver, ist=state");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, clrh)
{
	RKH_TR_SM_CLRH(&receiver, &state);

    trazerOutExpect(trazerOut, 3, "SM", "CLRH", "ao=receiver, h=state");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, trn)
{
	RKH_TR_SM_TRN(&receiver, &state, &state);

    trazerOutExpect(trazerOut, 3, "SM", "TRN", "ao=receiver, "
                                "sst=state, tst=state");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, state)
{
	RKH_TR_SM_STATE(&receiver, &state);

    trazerOutExpect(trazerOut, 3, "SM", "STATE", "ao=receiver, "
                                "nxtst=state");
    sdiag_state_Expect((ulong)&receiver, (ulong)&state);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}


TEST(sm, enstate)
{
	RKH_TR_SM_ENSTATE(&receiver, &state);

    trazerOutExpect(trazerOut, 3, "SM", "ENSTATE", "ao=receiver, st=state");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, exstate)
{
	RKH_TR_SM_EXSTATE(&receiver, &state);

    trazerOutExpect(trazerOut, 3, "SM", "EXSTATE", "ao=receiver, st=state");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, nenex)
{
	RKH_TR_SM_NENEX(&receiver, 4, 8);

    trazerOutExpect(trazerOut, 3, "SM", "NENEX", "ao=receiver, "
                                   "nen=4, nex=8");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, ntrnact)
{
	RKH_TR_SM_NTRNACT(&receiver, 4, 8);

    trazerOutExpect(trazerOut, 3, "SM", "NTRNACT", "ao=receiver, "
                                   "nta=4, nts=8");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, tsstate)
{
	RKH_TR_SM_TS_STATE(&receiver, &state);

    trazerOutExpect(trazerOut, 3, "SM", "TS_STATE", "ao=receiver, st=state");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, evtproc)
{
	RKH_TR_SM_EVT_PROC(&receiver);

    trazerOutExpect(trazerOut, 3, "SM", "EVT_PROC", "ao=receiver");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, evtnfound)
{
	RKH_TR_SM_EVT_NFOUND(&receiver, &event);

    trazerOutExpect(trazerOut, 3, "SM", "EVT_NFOUND", "ao=receiver, sig=0X3");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, grdfalse)
{
	RKH_TR_SM_GRD_FALSE(&receiver);

    trazerOutExpect(trazerOut, 3, "SM", "GRD_FALSE", "ao=receiver");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, cndnfound)
{
	RKH_TR_SM_CND_NFOUND(&receiver);

    trazerOutExpect(trazerOut, 3, "SM", "CND_NFOUND", "ao=receiver");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, unkstate)
{
	RKH_TR_SM_UNKN_STATE(&receiver);

    trazerOutExpect(trazerOut, 3, "SM", "UNKN_STATE", "ao=receiver");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, exhlevel)
{
	RKH_TR_SM_EX_HLEVEL(&receiver);

    trazerOutExpect(trazerOut, 3, "SM", "EX_HLEVEL", "ao=receiver");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, extseg)
{
	RKH_TR_SM_EX_TSEG(&receiver);

    trazerOutExpect(trazerOut, 3, "SM", "EX_TSEG", "ao=receiver");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(sm, exeact)
{
	RKH_TR_SM_EXE_ACT(0, &receiver, &state, 0x5A5A5A);

    trazerOutExpect(trazerOut, 3, "SM", "EXE_ACT", "Effect, ao=receiver, "
                                  "st=state, fn=0X5A5A5A");

    sdiag_exec_act_Expect((ulong)&receiver, (ulong)0x5A5A5A);
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
