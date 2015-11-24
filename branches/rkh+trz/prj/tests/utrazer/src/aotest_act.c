/**
 *  \file       aotest_act.c
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

#include <stdio.h>
#include "rkh.h"
#include "bsp.h"
#include "aotest.h"
#include "aotest_act.h"

/* ----------------------------- Local macros ------------------------------ */

#define CAST_BLINKY(sm_)     ((Blinky*)(sm_))

/* ------------------------------- Constants ------------------------------- */
/* ---------------------------- Local data types --------------------------- */
/* ---------------------------- Global variables --------------------------- */

/*
 *  Declare and allocate the 'blktim' timer, which is responsible for
 *  toggling the LED posting the TIMEOUT signal event to active object
 *  'blinky'.
 */
RKH_TMR_T bkytim;

/* ---------------------------- Local variables ---------------------------- */

/*
 *  Declare and allocate the 'e_tout' event.
 *  The 'e_tout' event with TIMEOUT signal never changes, so it can be
 *  statically allocated just once by means of RKH_ROM_STATIC_EVENT() macro.
 */
static RKH_ROM_STATIC_EVENT(e_tout, TIMEOUT);

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */
/* ---------------------------- Global functions --------------------------- */

/* Defines HSM init function */
void
blinky_init(const Blinky *const me)
{
    RKH_TMR_INIT(&bkytim, &e_tout, NULL);
    blinky_led_on(me, NULL);
}

/* 
 * Defines transition action functions 
 */
void
blinky_led_on(const Blinky *const me, RKH_EVT_T *pe)
{
    (void)pe;

    RKH_TMR_ONESHOT(&bkytim, blinky, LED_ON_TIME);
    bsp_led_on();
    ++CAST_BLINKY(me)->cnt;
}

void
blinky_led_off(const Blinky *const me, RKH_EVT_T *pe)
{
    (void)me;
    (void)pe;

    RKH_TMR_ONESHOT(&bkytim, blinky, LED_OFF_TIME);
    bsp_led_off();
}

/* ------------------------------ End of file ------------------------------ */
