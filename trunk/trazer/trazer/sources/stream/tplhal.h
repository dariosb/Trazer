/*
 * tplhal.h
 */

#ifndef __TPLHAL_H__
#define __TPLHAL_H__

#include "mytypes.h"
#include "tplcfg.h"

void init_tpl_hal( void );

void tpl_choke_xmit( uchar data );
void tpl_xmit( uchar data );
void tpl_stop_xmit( void );

#if __TPLINK_DBG__

#include <stdio.h>
#define mprintf(x)	printf x
#else
#define mprintf(x);
#endif

#endif
