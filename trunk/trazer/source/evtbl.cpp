/*
 * evtbl.cpp
 */
#include <string>
#include <vector>

#include <stdio.h>
#include "evtbl.h"
#include "rkhtrc.h"
#include "tzparse.h"
#include "messages.h"

using namespace std;


#define MAX_EVENT_TBL_SIZE	100

#define MKFI( id, gn, nm, fmt, fargs )	\
	{ { id, gn, nm, fmt, fargs }, #id }
	
static FMT_ID_T fmt_id_tbl[] =
{
	MKFI( RKH_TE_MP_INIT,       "MP",  "INIT",       "mp=%s, nblock=%d",      h_symnblk ),
	MKFI( RKH_TE_MP_GET,        "MP",  "GET",        "mp=%s, nfree=%d",       h_symnblk ),
	MKFI( RKH_TE_MP_PUT,        "MP",  "PUT",        "mp=%s, nfree=%d",       h_symnblk ),
	
	/* --- Queue (RQ) ------------------------ */
	MKFI( RKH_TE_RQ_INIT,       "RQ",  "INIT",       "q=%s, ao=%s, nelem=%d", h_2symnused ),
	MKFI( RKH_TE_RQ_GET,        "RQ",  "GET",        "q=%s, nelem=%d",        h_symnused ),
	MKFI( RKH_TE_RQ_FIFO,       "RQ",  "FIFO",       "q=%s, nelem=%d",        h_symnused ),
	MKFI( RKH_TE_RQ_LIFO,       "RQ",  "LIFO",       "q=%s, nelem=%d",        h_symnused ),
	MKFI( RKH_TE_RQ_FULL,       "RQ",  "FULL",       "q=%s",                  h_1sym ),
	MKFI( RKH_TE_RQ_DPT,        "RQ",  "DPT",        "q=%s",                  h_1sym ),
	MKFI( RKH_TE_RQ_GET_LAST,   "RQ",  "GET_LAST",   "q=%s",                  h_1sym ),

	/* --- State Machine Application (SMA) --- */
	MKFI( RKH_TE_SMA_ACT,       "SMA", "ACT",        "ao=%s",                 h_1sym ),
	MKFI( RKH_TE_SMA_TERM,      "SMA", "TERM",       "ao=%s",                 h_1sym ),
	MKFI( RKH_TE_SMA_GET,       "SMA", "GET",        "ao=%s, sig=%s",         h_symevt ),
	MKFI( RKH_TE_SMA_FIFO,      "SMA", "FIFO",       "ao=%s, sig=%s",         h_aosymevt ),
	MKFI( RKH_TE_SMA_LIFO,      "SMA", "LIFO",       "ao=%s, sig=%s",         h_aosymevt ),
	MKFI( RKH_TE_SMA_REG,       "SMA", "REG",        "ao=%s, prio=%d",        h_symu8 ),
	MKFI( RKH_TE_SMA_UNREG,     "SMA", "UNREG",      "ao=%s, prio=%d",        h_symu8 ),

	/* --- State machine (SM) ---------------- */
	MKFI( RKH_TE_SM_INIT,       "SM",  "INIT",       "ao=%s, ist=%s",         h_2sym ),
	MKFI( RKH_TE_SM_CLRH,       "SM",  "CLRH",       "ao=%s, h=%s",           h_2sym ),
	MKFI( RKH_TE_SM_DCH,        "SM",  "DCH",        "ao=%s, sig=%s",         h_symevt ),
	MKFI( RKH_TE_SM_TRN,        "SM",  "TRN",        "ao=%s, sst=%s, tst=%s", h_symtrn ),
	MKFI( RKH_TE_SM_STATE,      "SM",  "STATE",	     "ao=%s, nxtst=%s",       h_2sym ),
	MKFI( RKH_TE_SM_ENSTATE,    "SM",  "ENSTATE",    "ao=%s, st=%s",          h_2sym ),
	MKFI( RKH_TE_SM_EXSTATE,    "SM",  "EXSTATE",    "ao=%s, st=%s",          h_2sym ),
	MKFI( RKH_TE_SM_NENEX,      "SM",  "NENEX",	     "ao=%s, nen=%d, nex=%d", h_sym2u8 ),
	MKFI( RKH_TE_SM_NTRNACT,    "SM",  "NTRNACT",    "ao=%s, nta=%d, nts=%d", h_sym2u8 ),
	MKFI( RKH_TE_SM_TS_STATE,   "SM",  "TS_STATE",	 "ao=%s, st=%s",          h_2sym ),
	MKFI( RKH_TE_SM_EVT_PROC,   "SM",  "EVT_PROC",	 "ao=%s",                 h_1sym ),
	MKFI( RKH_TE_SM_EVT_NFOUND, "SM",  "EVT_NFOUND", "ao=%s",                 h_1sym ),
	MKFI( RKH_TE_SM_GRD_FALSE,  "SM",  "GRD_FALSE",  "ao=%s",                 h_1sym ),
	MKFI( RKH_TE_SM_CND_NFOUND, "SM",  "CND_NFOUND", "ao=%s",                 h_1sym ),
	MKFI( RKH_TE_SM_UNKN_STATE, "SM",  "UNKN_STATE", "ao=%s",                 h_1sym ),
	MKFI( RKH_TE_SM_EX_HLEVEL,  "SM",  "EX_HLEVEL",  "ao=%s",                 h_1sym ),
	MKFI( RKH_TE_SM_EX_TSEG,    "SM",  "EX_TSEG",    "ao=%s",                 h_1sym ),

	/* --- Timer (TIM) ----------------------- */
	MKFI( RKH_TE_TIM_INIT,      "TIM", "INIT",       "t=%s, sig=%s",          h_symevt ),
	MKFI( RKH_TE_TIM_START,     "TIM", "START",      "t=%s, ao=%s, ntick=%d", h_2symntick ),
	MKFI( RKH_TE_TIM_STOP,      "TIM", "STOP",       "t=%s",                  h_1sym ),
	MKFI( RKH_TE_TIM_TOUT,      "TIM", "TOUT",       "t=%s",                  h_1sym ),
	MKFI( RKH_TE_TIM_REM,       "TIM", "REM",        "t=%s",                  h_1sym ),

	/* --- Framework (RKH) ------------------- */
	MKFI( RKH_TE_FWK_EN,        "FWK", "EN",         "",                      h_none ),
	MKFI( RKH_TE_FWK_EX,        "FWK", "EX",         "",                      h_none ),
	MKFI( RKH_TE_FWK_EPREG,     "FWK", "EPREG",      "ep =%d, ss=%d, es=%d",  h_epreg ),
	MKFI( RKH_TE_FWK_AE,        "FWK", "AE",         "es=%d, sig=%s",         h_ae ),
	MKFI( RKH_TE_FWK_GC,        "FWK", "GC",         "sig=%s",                h_evt ),
	MKFI( RKH_TE_FWK_GCR,       "FWK", "GCR",        "sig=%s",                h_evt ),
	MKFI( RKH_TE_FWK_DEFER,     "FWK", "DEFER",      "q=%s, sig=%s",          h_symevt ),
	MKFI( RKH_TE_FWK_RCALL,     "FWK", "RCALL",      "ao=%s, sig=%s",         h_symevt ),
	MKFI( RKH_TE_FWK_OBJ,       "FWK", "OBJ",        "obj=0x%08X, nm=%s",     h_symobj ),
	MKFI( RKH_TE_FWK_SIG,       "FWK", "SIG",        "sig=%d, nm=%s",         h_symsig ),
	MKFI( RKH_TE_FWK_FUN,       "FWK", "FUN",        "func=0x%08X, nm=%s",    h_symobj ),
	MKFI( RKH_TE_FWK_EXE_FUN,   "FWK", "EXE_FUN",    "func=%s",               h_1sym ),
	MKFI( RKH_TE_FWK_TUSR,      "FWK", "TUSR",       "usrtrc=%d, nm=%s",      h_symuevt ),
	MKFI( RKH_TE_FWK_TCFG,      "FWK", "TCFG",       NULL,                    h_tcfg ),
	MKFI( RKH_TE_FWK_ASSERT,    "FWK", "ASSERT",     "%s.c (%d)",             h_assert ),
	MKFI( RKH_TE_FWK_AO,        "FWK", "AO",         "obj=0x%08X, nm=%s",     h_symobj ),
	MKFI( RKH_TE_FWK_STATE,     "FWK", "STATE",      "ao=%s, obj=0x%08X, nm=%s", h_symst ),
	MKFI( RKH_TE_FWK_PSTATE,    "FWK", "PSTATE",     "ao=%s, obj=0x%08X, nm=%s", h_symst ),
	MKFI( RKH_TE_FWK_TIMER,     "FWK", "TIMER",      "obj=0x%08X, nm=%s",     h_symobj ),
	MKFI( RKH_TE_FWK_EPOOL,     "FWK", "EPOOL",      "obj=0x%08X, nm=%s",     h_symobj ),
	MKFI( RKH_TE_FWK_QUEUE,     "FWK", "QUEUE",      "obj=0x%08X, nm=%s",     h_symobj ),
	MKFI( RKH_TE_USER,          "USR", "USR#",       NULL,                    NULL )
};

TRE_T fmt_usr_tbl = 
{
	RKH_TE_USER, "USR", "USR#", NULL, usr_fmt
};


FMT_ID_T *
get_evt_id( string *idstr )
{
	FMT_ID_T *p;

	for( p=fmt_id_tbl; p->tre.id != RKH_TE_USER; ++p )
		if( idstr->compare(p->idstr) == 0 )
			return p;

	return NULL;
}


const
TRE_T *
find_trevt( unsigned char id )
{
	FMT_ID_T *p;

	for( p=fmt_id_tbl; p->tre.id != RKH_TE_USER; ++p )
	{
		if( id == p->tre.id )
				return &(p->tre);
	}
	return ( TRE_T* )0;
}

const
TRE_T *
point_2_trevt( unsigned char ix )
{
	return &fmt_id_tbl[ix].tre;
}
