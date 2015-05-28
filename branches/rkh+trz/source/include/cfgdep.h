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


extern CONFIG_T configs;

#define TRZ_RKH_CFG_RQ_GET_LWMARK_EN		configs.trazer_rq_en_get_lwmark
#define TRZ_RKH_CFG_RQ_SIZEOF_NELEM			configs.trazer_sizeof_nelem

#define TRZ_RKH_CFG_MP_SIZEOF_BSIZE			configs.trazer_sizeof_bsize
#define TRZ_RKH_CFG_MP_SIZEOF_NBLOCK		configs.trazer_sizeof_nblock
#define TRZ_RKH_CFG_MP_GET_LWM_EN			configs.trazer_mp_en_get_lwm

#define TRZ_RKH_CFG_SMA_TRC_SNDR_EN			configs.trazer_en_sender

#define TRZ_RKH_CFG_FWK_SIZEOF_EVT			configs.trazer_sizeof_sig
#define TRZ_RKH_CFG_FWK_MAX_EVT_POOL		configs.trazer_max_epool
#define TRZ_RKH_CFG_FWK_SIZEOF_EVT_SIZE		configs.trazer_sizeof_esize

#define TRZ_RKH_CFGPORT_TRC_SIZEOF_PTR		configs.trazer_sizeof_pointer
#define TRZ_RKH_CFGPORT_TRC_SIZEOF_TSTAMP	configs.trazer_sizeof_tstamp

#define TRZ_RKH_CFG_TRC_RTFIL_EN			configs.trazer_runtime_filter
#define TRZ_RKH_CFG_TRC_USER_TRACE_EN		configs.trazer_en_user_trace
#define TRZ_RKH_CFG_TRC_ALL_EN				configs.trazer_all
#define TRZ_RKH_CFG_TRC_MP_EN				configs.trazer_en_mp
#define TRZ_RKH_CFG_TRC_RQ_EN				configs.trazer_en_rq
#define TRZ_RKH_CFG_TRC_SMA_EN				configs.trazer_en_sma
#define TRZ_RKH_CFG_TRC_TMR_EN				configs.trazer_en_tim
#define TRZ_RKH_CFG_TRC_SM_EN				configs.trazer_en_sm
#define TRZ_RKH_CFG_TRC_FWK_EN				configs.trazer_en_fwk
#define TRZ_RKH_CFG_TRC_ASSERT_EN			configs.trazer_en_assert
#define TRZ_RKH_CFG_TRC_NSEQ_EN				configs.trazer_en_nseq
#define TRZ_RKH_CFG_TRC_CHK_EN				configs.trazer_en_chk
#define TRZ_RKH_CFG_TRC_TSTAMP_EN			configs.trazer_en_tstamp
#define TRZ_RKH_CFG_TRC_RTFIL_SMA_EN		configs.trazer_rtfil_sma_en
#define TRZ_RKH_CFG_TRC_RTFIL_SIGNAL_EN		configs.trazer_rtfil_signal_en

#define TRZ_RKH_CFG_TMR_SIZEOF_NTIMER		configs.trazer_sizeof_ntimer

int sizeof_type( int type );

#define sizeof_trze()		sizeof_type( TRZ_RKH_CFG_FWK_SIZEOF_EVT )
#define sizeof_trzts()		sizeof_type( TRZ_RKH_CFGPORT_TRC_SIZEOF_TSTAMP )
#define sizeof_trznt()		sizeof_type( TRZ_RKH_CFG_TMR_SIZEOF_NTIMER )
#define sizeof_trznb()		sizeof_type( TRZ_RKH_CFG_MP_SIZEOF_NBLOCK )
#define sizeof_trzne()		sizeof_type( TRZ_RKH_CFG_RQ_SIZEOF_NELEM )
#define sizeof_trzes()		sizeof_type( TRZ_RKH_CFG_FWK_SIZEOF_EVT_SIZE )


void trazer_output( int flg, TRAZER_DATA_T* tz );
/*
const char *get_trheader( int en_tstamp, int sz_tstamp, int en_nseq );
#define trheader		get_trheader()
#define trheader_nts	" - [%3d] %-4s| %-10s : "*/


#endif
