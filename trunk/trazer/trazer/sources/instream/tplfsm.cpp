/*
 * tplfsm.c
 */

#include "mytypes.h"
#include "tplfsm.h"
#include "tpltbl.h"
#include <string.h>

static PROCFUNC_T ptpl_fsm;
static TPL_STATE_T currst, tempst;

void
init_tplfsm( TPL_STATE_T init_st )
{
	currst = init_st;
	ptpl_fsm = tpltbl[init_st];
}

void
proc_tplfsm( MUInt input, uchar data )
{
	currst = (*ptpl_fsm)(input, data);
	ptpl_fsm = tpltbl[currst];
}

TPL_STATE_T
call_tplst( TPL_STATE_T dest_st, TPL_STATE_T src_st )
{
	tempst = src_st;
	return dest_st;
}

TPL_STATE_T
ret_tplst( MUInt input, uchar data )
{	
	return (*tpltbl[tempst])( input, data );
}

TPL_STATE_T
get_tplfsm_state( void )
{
	return currst;
}

/*
 * ISR's
 */
static MUInt tplfsm_timer;

void
tplfsm_kick_timer( unsigned short time )
{
	tplfsm_timer = time/10;
}

void
tplfsm_timer_isr( void )
{
	if( tplfsm_timer == 0 || --tplfsm_timer )
		return;

	proc_tplfsm( TPLINK_TOUT, 0 );
}

void
tplfsm_rcv_isr( uchar rcv )
{
	proc_tplfsm( TPLINK_RCV, rcv );
}

void
tplfsm_xmit_isr( void )
{
	proc_tplfsm( TPLINK_XMIT, 0 );
}



