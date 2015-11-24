/**
 *  \file       aotest.h
 *  \ingroup    Test
 *
 *  \brief      Production code.
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2015.10.24  LeFr  v2.4.05  Initial version
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* --------------------------------- Module -------------------------------- */

#ifndef __AOTEST_H__
#define __AOTEST_H__

/* ----------------------------- Include files ----------------------------- */

#include "rkh.h"

/* ---------------------- External C language linkage ---------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------- Macros -------------------------------- */
/* -------------------------------- Constants ------------------------------ */

#define LED_OFF_TIME            RKH_TIME_SEC(2)
#define LED_ON_TIME             RKH_TIME_SEC(2)

/* Signals */
enum
{
    TIMEOUT,        /* 'blktim' timeout */
    TERMINATE,      /* press the key escape on the keyboard */
    BKY_NUM_EVENTS
};


/* Declare HSM */
RKH_SMA_DCLR(blinky);

/* Declare states and pseudostates */
RKH_DCLR_BASIC_STATE led_off, led_on;

/* ------------------------------- Data types ------------------------------ */

typedef struct Blinky Blinky;
struct Blinky      /* SMA derived from RKH_SMA_T structure */
{
    RKH_SMA_T sma;  /* base structure */
    rui8_t cnt;     /* private member */
};

/* -------------------------- External variables --------------------------- */
/* -------------------------- Function prototypes -------------------------- */
/* -------------------- External C language linkage end -------------------- */

#ifdef __cplusplus
}
#endif

/* ------------------------------ Module end ------------------------------- */

#endif

/* ------------------------------ End of file ------------------------------ */
