/*
 * tpltbl.c
 */

#include "tplink.h"
#include "tpltbl.h"
#include "tplact.h"

const PROCFUNC_T tpltbl[TPL_NUM_STATES] =
{
	proc_tpl_idle,
	proc_tpl_inesc,
#if TPLINK_ADDRESSING_ON
	proc_tpl_waddr,
#endif
	proc_tpl_wndatah,
	proc_tpl_wndatal,
	proc_tpl_indata,
	proc_tpl_wchk,
	proc_tpl_inxmit,
	proc_tpl_xmitesc,
	proc_tpl_xchk,
	proc_tpl_wxmit
};
