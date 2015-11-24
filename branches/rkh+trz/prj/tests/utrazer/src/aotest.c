/**
 *  \file       aotest.c
 *  \ingroup    Test
 *
 *  \brief      Production code.
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

#include "rkh.h"
#include "aotest.h"
#include "aotest_act.h"

/* ----------------------------- Local macros ------------------------------ */
/* ------------------------------- Constants ------------------------------- */

/*
 *	Defines SMA (active object) 'blinky'.
 */
RKH_SMA_CREATE(Blinky, blinky, 0, HCAL, &led_on, blinky_init, NULL);

/*
 *	Defines states and pseudostates.
 */
RKH_CREATE_BASIC_STATE(led_on, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(led_on)

    RKH_TRREG(TIMEOUT, NULL,   blinky_led_off,     &led_off),

RKH_END_TRANS_TABLE

RKH_CREATE_BASIC_STATE(led_off, NULL, NULL, RKH_ROOT, NULL);
RKH_CREATE_TRANS_TABLE(led_off)

    RKH_TRREG(TIMEOUT, NULL,   blinky_led_on,      &led_on),

RKH_END_TRANS_TABLE

/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */
/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */
/* ------------------------------ End of file ------------------------------ */
