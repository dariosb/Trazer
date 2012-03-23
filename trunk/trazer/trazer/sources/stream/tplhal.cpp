/*
 * tplhal.c
 */

#include "tplhal.h"
#include "serial.h"
#include "timer.h"
#include "tplfsm.h"

#include <stdio.h>

static int enable_xmit = 0;
static char xmit_thr;

static
void
timer_isr( void )
{
	tplfsm_timer_isr();
}

static
void
ser_rx_isr( unsigned char byte )
{
	tplfsm_rcv_isr( byte );
}

static
void
ser_tx_isr( void )
{
	tplfsm_xmit_isr();

	if( enable_xmit )
		tx_data( 0, xmit_thr );
}

static SERIAL_CBACK_T ser_cback =
{
	ser_rx_isr,
	NULL,
	NULL,
	ser_tx_isr,
	NULL,
	NULL,
	NULL
};

/*
 * Public Functions
 */

void
init_tpl_hal( void )
{
	init_serial_hard( 0, &ser_cback );
	connect_serial( 0 );
	init_timer_hard( timer_isr );
	enable_timer_interrupts();
}

void
tpl_choke_xmit( uchar data )
{
	enable_xmit = 1;
	tx_data( 0, data );
}

void
tpl_xmit( uchar data )
{
	xmit_thr = data;
}

void
tpl_stop_xmit( void )
{
	enable_xmit = 0;
}

