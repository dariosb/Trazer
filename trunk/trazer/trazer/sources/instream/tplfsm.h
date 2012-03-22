/*
 * tplfsm.h
 */

#ifndef __TPLFSM_H__
#define __TPLFSM_H__

#include "mytypes.h"
#include "tplink.h"

typedef enum
{
	TPLINK_RCV,
	TPLINK_XMIT,
	TPLINK_TOUT,
	TPLINK_RETESC,

	TPLINK_NUM_INPUTS
}TPLINK_INPUTS;

typedef MUInt TPL_STATE_T;

typedef MUInt(*PROCFUNC_T)(MUInt, uchar);

void init_tplfsm( TPL_STATE_T init_st );

void proc_tplfsm( MUInt input, uchar data );

void tplfsm_kick_timer( unsigned short time );

TPL_STATE_T call_tplst( TPL_STATE_T dest_st, TPL_STATE_T src_st );
TPL_STATE_T ret_tplst( MUInt input, uchar data );
TPL_STATE_T get_tplfsm_state( void );

/*
 * ISR's
 */
void tplfsm_timer_isr( void );

void tplfsm_rcv_isr( uchar rcv );

void tplfsm_xmit_isr( void );

#endif
