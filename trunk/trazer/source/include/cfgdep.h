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
	rkhui8_t nseq;
	const char *group;
	const char *name;
} TRAZER_DATA_T;


#define PREPARE_TZOUT_FLGS( ets, sts, enseq )			\
				((((ets!=0)<<3)|sts)|((enseq!=0)<<7))

#define TRAZER_TCFG_EN_TSTAMP_MSK		0x08
#define TRAZER_TCFG_SIZEOF_TSTAMP_MSK	0x07
#define TRAZER_TCFG_EN_NSEQ_MSK			0x80


extern CONFIG_T configs;

#define RKH_SMA_EN_TRC_SENDER		configs.trazer_en_sender
#define RKH_TRC_RUNTIME_FILTER		configs.trazer_runtime_filter
#define RKH_TRC_EN_USER_TRACE		configs.trazer_en_user_trace
#define RKH_TRC_ALL					configs.trazer_all
#define RKH_TRC_EN_MP				configs.trazer_en_mp
#define RKH_TRC_EN_RQ				configs.trazer_en_rq
#define RKH_TRC_EN_SMA				configs.trazer_en_sma
#define RKH_TRC_EN_TIM				configs.trazer_en_tim
#define RKH_TRC_EN_SM				configs.trazer_en_sm
#define RKH_TRC_EN_FWK				configs.trazer_en_fwk
#define RKH_TRC_EN_ASSERT			configs.trazer_en_assert
#define RKH_RQ_EN_GET_LWMARK		configs.trazer_rq_en_get_lwmark
#define RKH_MP_EN_GET_LWM			configs.trazer_mp_en_get_lwm
#define RKH_SIZEOF_EVENT			configs.trazer_sizeof_sig
#define RKH_TRC_SIZEOF_TSTAMP		configs.trazer_sizeof_tstamp
#define RKH_TRC_SIZEOF_POINTER		configs.trazer_sizeof_pointer
#define RKH_TIM_SIZEOF_NTIMER		configs.trazer_sizeof_ntimer
#define RKH_MP_SIZEOF_NBLOCK		configs.trazer_sizeof_nblock
#define RKH_RQ_SIZEOF_NELEM			configs.trazer_sizeof_nelem
#define RKH_SIZEOF_ESIZE			configs.trazer_sizeof_esize
#define RKH_TRC_EN_NSEQ				configs.trazer_en_nseq
#define RKH_TRC_EN_CHK				configs.trazer_en_chk
#define RKH_TRC_EN_TSTAMP			configs.trazer_en_tstamp
#define RKH_MP_SIZEOF_BSIZE			configs.trazer_sizeof_bsize
#define RKH_MAX_EPOOL				configs.trazer_max_epool
#define RKH_TRC_RTFIL_SMA_EN			configs.trazer_rtfil_sma_en
#define RKH_TRC_RTFIL_SIGNAL_EN			configs.trazer_rtfil_signal_en

int sizeof_type( int type );

#define sizeof_trze()		sizeof_type( RKH_SIZEOF_EVENT )
#define sizeof_trzts()		sizeof_type( RKH_TRC_SIZEOF_TSTAMP )
#define sizeof_trznt()		sizeof_type( RKH_TIM_SIZEOF_NTIMER )
#define sizeof_trznb()		sizeof_type( RKH_MP_SIZEOF_NBLOCK )
#define sizeof_trzne()		sizeof_type( RKH_RQ_SIZEOF_NELEM )
#define sizeof_trzes()		sizeof_type( RKH_SIZEOF_ESIZE )


void trazer_output( int flg, TRAZER_DATA_T* tz );
/*
const char *get_trheader( int en_tstamp, int sz_tstamp, int en_nseq );
#define trheader		get_trheader()
#define trheader_nts	" - [%3d] %-4s| %-10s : "*/


#endif
