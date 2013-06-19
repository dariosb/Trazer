/*
 * 	config.h
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__


typedef struct
{
	int trazer_en_sender;
	int trazer_runtime_filter;
	int trazer_en_user_trace;
	int trazer_all;
	int trazer_en_mp;
	int trazer_en_rq;
	int trazer_en_sma;
	int trazer_en_tim;
	int trazer_en_sm;
	int trazer_en_fwk;
	int trazer_en_assert;
	int trazer_sizeof_sig;
	int trazer_sizeof_tstamp;
	int trazer_sizeof_pointer;	
	int trazer_sizeof_ntimer;
	int trazer_sizeof_nblock;
	int trazer_sizeof_nelem;
	int trazer_sizeof_esize;
	int trazer_en_nseq;
	int trazer_en_chk;
	int trazer_en_tstamp;
} CONFIG_T;

/*
 * RKH default configuration
 */
#define TRAZER_EN_SENDER_DFT		0
#define TRAZER_RUNTIME_FILTER_DFT	1
#define TRAZER_EN_USER_TRACE_DFT	1
#define TRAZER_ALL_DFT				1
#define TRAZER_EN_MP_DFT			0
#define TRAZER_EN_RQ_DFT			0
#define TRAZER_EN_SMA_DFT			0
#define TRAZER_EN_TIM_DFT			0
#define TRAZER_EN_SM_DFT			0
#define TRAZER_EN_FWK_DFT			1
#define TRAZER_EN_ASSERT_DFT		1
#define	TRAZER_SIZEOF_SIG_DFT		1
#define	TRAZER_SIZEOF_TSTAMP_DFT	4
#define	TRAZER_SIZEOF_POINTER_DFT	4
#define	TRAZER_SIZEOF_NTIMER_DFT	2
#define	TRAZER_SIZEOF_NBLOCK_DFT	1
#define	TRAZER_SIZEOF_NELEM_DFT		1
#define	TRAZER_SIZEOF_ESIZE_DFT		2
#define	TRAZER_EN_NSEQ_DFT			1
#define	TRAZER_EN_CHK_DFT			1
#define	TRAZER_EN_TSTAMP_DFT		1

#define CONFIG_FILE				"trazer.cfg"

extern CONFIG_T configs;

void read_config_file( char *cfg_file );

#endif
