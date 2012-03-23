/*
 * tplink.c
 */

#include "mytypes.h"
#include "tplink.h"
#include "tplfsm.h"
#include "tpltbl.h"
#include "tplact.h"
#include "tplhal.h"
#include <string.h>

void
tplink_init( void )
{
	init_tplfsm( TPL_IDLE );

	init_tpl_hal();
}

MInt
#if (TPLINK_ADDRESSING_ON) && (TPLINK_DEV_TYPE == TPLINK_DEV_MASTER)
tplink_send_frame( uchar addr, uchar *pl, ushort qty )
#else
tplink_send_frame( uchar *pl, ushort qty )
#endif
{
#ifdef __TPLINK_DBG__
	ushort i;
	uchar *p;
#endif

	if( get_tplfsm_state() >= TPL_IN_XMIT )
		return -TPLINK_ERR_BUSY;

	if( qty > TPLINK_MAX_PLOAD_SIZE )
		return -TPLINK_ERR_PLOAD_SIZE_EXCEED;

#ifdef __TPLINK_DBG__
	#if (TPLINK_ADDRESSING_ON) && (TPLINK_DEV_TYPE == TPLINK_DEV_MASTER)
	mprintf(( "%s: station = %02d, qty = %02d, buff =", 
										__FUNCTION__, addr, qty ));
	#else
	mprintf(( "%s: qty = %02d, buff =", 
										__FUNCTION__, qty ));

	#endif

	for( i = qty, p = pl; i; --i, ++p )
		mprintf(( " %c", *p ));

	mprintf(( "\n" ));
#endif

#if (TPLINK_ADDRESSING_ON) && (TPLINK_DEV_TYPE == TPLINK_DEV_MASTER)
	start_xmit( addr, pl, qty );
#else
	start_xmit( pl, qty );
#endif
	return TPLINK_OK;
}




