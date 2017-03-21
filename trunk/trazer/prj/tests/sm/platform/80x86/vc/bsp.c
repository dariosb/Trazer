/*
 *  --------------------------------------------------------------------------
 *
 *                                Framework RKH
 *                                -------------
 *
 *            State-machine framework for reactive embedded systems
 *
 *                      Copyright (C) 2010 Leandro Francucci.
 *          All rights reserved. Protected by international copyright laws.
 *
 *
 *  RKH is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, either version 3 of the License, or (at your option) any
 *  later version.
 *
 *  RKH is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with RKH, see copying.txt file.
 *
 *  Contact information:
 *  RKH web site:   http://sourceforge.net/projects/rkh-reactivesys/
 *  e-mail:         francuccilea@gmail.com
 *  ---------------------------------------------------------------------------
 */

/**
 *  \file       bsp.c
 *  \ingroup    Test
 *
 *  \brief      BSP for 80x86 OS win32
 */

/* -------------------------- Development history -------------------------- */
/*
 *  2015.11.19  LeFr  v2.4.05  ---
 */

/* -------------------------------- Authors -------------------------------- */
/*
 *  LeFr  Leandro Francucci  francuccilea@gmail.com
 */

/* --------------------------------- Notes --------------------------------- */
/* ----------------------------- Include files ----------------------------- */
#include <conio.h>
#include "rkh.h"

#if RKH_CFG_TRC_EN == RKH_ENABLED
#include <time.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>        /* Win32 API for multithreading */
#include <stdlib.h>
#endif

#include "unity.h"
#include "bsp.h"
#include "rkh.h"
#include "unitrazer.h"
//#include "unittrz.h"
#include "tzparse.h"
#include "utzparse.h"

/* ----------------------------- Local macros ------------------------------ */

#define kbmap(c)            ((c) - '0')

/* ------------------------------- Constants ------------------------------- */

#define ESC                 0x1B

/* ---------------------------- Local data types --------------------------- */

/* ---------------------------- Global variables --------------------------- */
/* ---------------------------- Local variables ---------------------------- */

RKH_THIS_MODULE

static rui32_t l_rnd;           /* random seed */
static DWORD tick_msec;         /* clock tick in msec */
rui8_t running;

#if defined(RKH_USE_TRC_SENDER)
static rui8_t l_isr_kbd;
static rui8_t l_isr_tick;
#endif

static UtrzProcessOut out;

/* ----------------------- Local function prototypes ----------------------- */
/* ---------------------------- Local functions ---------------------------- */

static
DWORD WINAPI
isr_tmr_thread(LPVOID par)      /* Win32 thread to emulate timer ISR */
{
    (void)par;
    while (running)
    {
        RKH_TIM_TICK(&l_isr_tick);
        Sleep(tick_msec);
    }
    return 0;
}

static
DWORD WINAPI
isr_kbd_thread(LPVOID par)      /* Win32 thread to emulate keyboard ISR */
{
    int c;

    (void)par;
    while (running)
    {
        c = _getch();
    }
    return 0;
}


static
void
execTrazerParser( rui8_t *p, TRCQTY_T n )
{
	while(n--)
		tzparser_exec(*p++);
}

static
int
utrz_recv(SOCKET s, UtrzProcessOut *p)
{
    char c;
    int n;

    p->status = UT_PROC_BUSY;
/*    while ((n = tcp_trace_recv((SOCKET)s, &c, sizeof(c))) > 0)*/
    {
        utrazer_parse(c, p);
        if (p->status != UT_PROC_BUSY)
        {
            return 1;
        }
    }
    return -1;
}

static
UT_RET_CODE
ut_process(UtrzProcessOut *pOut)
{
/*    if (utrz_recv(tsock, pOut) <= 0)*/
    {
        pOut->status = UT_PROC_FAIL;
    }

    return pOut->status;
}

/* ---------------------------- Global functions --------------------------- */

UtrzProcessOut *
unitrazer_getLastOut(void)
{
    return &out;
}

void
unitrazer_resetOut(void)
{
    out.line = 0;
    out.msg[0] = '\0';
    out.status = UT_PROC_INIT;
}

void
rkh_hook_timetick(void)
{
}

void
rkh_hook_start(void)
{
    DWORD thtmr_id, thkbd_id;
    HANDLE hth_tmr, hth_kbd;

    /* set the desired tick rate */
    tick_msec = 1000UL / BSP_TICKS_PER_SEC;
    running = (rui8_t)1;

    /* create the ISR timer thread */
    hth_tmr = CreateThread(NULL, 1024, &isr_tmr_thread, 0, 0, &thtmr_id);
    RKH_ASSERT(hth_tmr != (HANDLE)0);
    SetThreadPriority(hth_tmr, THREAD_PRIORITY_TIME_CRITICAL);

    /* create the ISR keyboard thread */
    hth_kbd = CreateThread(NULL, 1024, &isr_kbd_thread, 0, 0, &thkbd_id);
    RKH_ASSERT(hth_kbd != (HANDLE)0);
    SetThreadPriority(hth_kbd, THREAD_PRIORITY_NORMAL);
}

void
rkh_hook_exit(void)
{
    RKH_TRC_FLUSH();
#if RKH_CFG_TRC_EN == RKH_ENABLED
    running = (rui8_t)0;
#endif
}

void
rkh_hook_idle(void)             /* called within critical section */
{
    RKH_EXIT_CRITICAL(dummy);
    RKH_TRC_FLUSH();
    /*RKH_WAIT_FOR_EVENTS();*/  /* yield the CPU until new event(s) arrive */
}

void
rkh_hook_putTrcEvt(void)
{
   UT_RET_CODE ret;

   RKH_TRC_FLUSH();
   ret = ut_process(&out);
   UNITY_TEST_ASSERT(ret != UT_PROC_FAIL, out.line, out.msg);
}

void
rkh_assert(RKHROM char * const file, int line)
{
#if 0
    fprintf(stderr, "RKH_ASSERT: [%d] line from %s "
            "file\n", line, file);
#endif
    RKH_TRC_FLUSH();
    RKH_DIS_INTERRUPT();
    RKH_TR_FWK_ASSERT((RKHROM char *)file, line);
    /* __debugbreak(); */
    rkh_fwk_exit();
}

#if RKH_CFG_TRC_EN == RKH_ENABLED
void
rkh_trc_open(void)
{
    rkh_trc_init();

	tzparser_init();
	unitrazer_init();

	utrazer_init();
	unitrazer_init_request();

    RKH_TRC_SEND_CFG(BSP_TS_RATE_HZ);
}

void
rkh_trc_close(void)
{
}

RKH_TS_T
rkh_trc_getts(void)
{
    return (RKH_TS_T)clock();
}

void
rkh_trc_flush(void)
{
    rui8_t *blk;
    TRCQTY_T nbytes;
    RKH_SR_ALLOC();

    FOREVER
    {
        nbytes = (TRCQTY_T)1024;

        RKH_ENTER_CRITICAL_();
        blk = rkh_trc_get_block(&nbytes);
        RKH_EXIT_CRITICAL_();

        if ((blk != (rui8_t *)0))
        {
			execTrazerParser(blk, nbytes);
			break;
        }
        else
        {
            break;
        }
    }
}
#endif

void
bsp_init(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    rkh_fwk_init();
    RKH_TRC_OPEN();
}

/* ------------------------------ End of file ------------------------------ */
