/*
 * tpltbl.h
 */
#ifndef __TPLTBL_H__
#define __TPLTBL_H__

#include "tplfsm.h"

enum
{
	TPL_IDLE,
	TPL_IN_ESC,
#if TPLINK_ADDRESSING_ON
	TPL_WADDR,
#endif
	TPL_WNDATAH,
	TPL_WNDATAL,
	TPL_IN_DATA,
	TPL_WCHK,
	TPL_IN_XMIT,
	TPL_XMIT_ESC,
	TPL_XCHK,
	TPL_WXMIT,

	TPL_NUM_STATES
};

extern const PROCFUNC_T tpltbl[TPL_NUM_STATES];

#endif

