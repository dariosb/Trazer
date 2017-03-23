/**
 *  \file       test_common.h
 *  \ingroup    Test
 *
 *  \brief      Production code.
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
/* --------------------------------- Module -------------------------------- */

#ifndef __TEST_COMMON_H__
#define __TEST_COMMON_H__

/* ----------------------------- Include files ----------------------------- */
#include "rkh.h"
#include "mytypes.h"
#include "unity_fixture.h"
#include <time.h>

/* ---------------------- External C language linkage ---------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */
/* ------------------------------- Data types ------------------------------ */
/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */


TRCQTY_T getTraceStream(rui8_t **p);
void execTrazerParser( void );
void trazerInitConfiguration( void );
void trazerSendSymbols( void );
void trazerOutExpect( char *p, ushort nseq, char *group, char *evt, char *args);
void common_test_setup( void );
void common_tear_down( void );

void test_init( void );

/* -------------------- External C language linkage end -------------------- */

#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */

#endif


