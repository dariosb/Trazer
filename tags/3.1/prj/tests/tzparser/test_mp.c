/**
 *  \file       test_mp.c
 *  \ingroup    test_mp
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

TEST_GROUP(mp);

/* ---------------------------- Local variables ---------------------------- */
static char trazerOut[50];
static RKH_MP_T mp;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
static
void
trazerSendSymbols( void )
{
	RKH_TR_FWK_OBJ( &mp );
    execTrazerParser();
}

/* ---------------------------- Global functions --------------------------- */

TEST_SETUP(mp)
{
    common_test_setup();
    trazerSendSymbols();

    initTesttzlog();
}

TEST_TEAR_DOWN(mp)
{
    common_tear_down();
}

TEST(mp, init)
{
	RKH_TR_MP_INIT(&mp, 123, 40);

    trazerOutExpect(trazerOut, 1, "MP", "INIT", "mp=mp, nb=123, bs=40");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(mp, get)
{
	RKH_TR_MP_GET(&mp, 123, 40);

    trazerOutExpect(trazerOut, 1, "MP", "GET", "mp=mp, nfree=123, nmin=40");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

TEST(mp, put)
{
	RKH_TR_MP_PUT(&mp, 123);

    trazerOutExpect(trazerOut, 1, "MP", "PUT", "mp=mp, nfree=123");
    
    execTrazerParser();

    TEST_ASSERT_EQUAL_STRING( trazerOut, lprintf_Buff );
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/* ------------------------------ End of file ------------------------------ */
