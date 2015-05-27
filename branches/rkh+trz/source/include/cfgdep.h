/*
 * cfgdep.h
 */

#ifndef __CFGDEP_H__
#define __CFGDEP_H__

#include "config.h"
#include "tzparse.h"

/**
 * Trace Config Event default stream format
 */

#define TRAZER_TCFG_EN_NSEQ_DFT			0
#define TRAZER_TCFG_EN_CHK_DFT			1
#define TRAZER_TCFG_EN_TSTAMP_DFT		0
#define TRAZER_TCFG_SIZEOF_TSTAMP_DFT	4	/* 1, 2, 4 in bytes */


/**
 */


typedef struct
{
	TRZTS_T ts;
	rui8_t nseq;
	const char *group;
	const char *name;
} TRAZER_DATA_T;


#define PREPARE_TZOUT_FLGS( ets, sts, enseq )			\
				((((ets!=0)<<3)|sts)|((enseq!=0)<<7))

#define TRAZER_TCFG_EN_TSTAMP_MSK		0x08
#define TRAZER_TCFG_SIZEOF_TSTAMP_MSK	0x07
#define TRAZER_TCFG_EN_NSEQ_MSK			0x80

int sizeof_type( int type );

#define sizeof_trze()		sizeof_type( RKH_CFG_FWK_SIZEOF_EVT )
#define sizeof_trzts()		sizeof_type( RKH_CFGPORT_TRC_SIZEOF_TSTAMP )
#define sizeof_trznt()		sizeof_type( RKH_CFG_TMR_SIZEOF_NTIMER )
#define sizeof_trznb()		sizeof_type( RKH_CFG_MP_SIZEOF_NBLOCK )
#define sizeof_trzne()		sizeof_type( RKH_CFG_RQ_SIZEOF_NELEM )
#define sizeof_trzes()		sizeof_type( RKH_CFG_FWK_SIZEOF_EVT_SIZE )


void trazer_output( int flg, TRAZER_DATA_T* tz );
/*
const char *get_trheader( int en_tstamp, int sz_tstamp, int en_nseq );
#define trheader		get_trheader()
#define trheader_nts	" - [%3d] %-4s| %-10s : "*/


#endif
