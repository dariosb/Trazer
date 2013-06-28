/*
 * evtbl.cpp
 */

#include <stdio.h>
#include "evtbl.h"
#include "rkhtrc.h"
#include "tzparse.h"
#include "messages.h"

using namespace std;


#define DCLR_TRE( id, gn, nm, fargs, ... )							\
	char* arg_##id[] = { __VA_ARGS__ };								\
	static FMT_ID_T st_##id = 										\
	{ 																\
	  {																\
	    id, gn, nm, 	 											\
	    { vector<char *>( (arg_##id),								\
		  &arg_##id[0]+sizeof(arg_##id)/sizeof(arg_##id[0]) ) },	\
	      fargs 													\
      },															\
	  #id															\
	};

#define TRE_ST(id)		&st_##id

#define q_s			"q=%s"
#define mp_s		"mp=%s"
#define nblock_d	"nb=%d"
#define bsize_d		"bs=%d"
#define nfree_d		"nfree=%d"
#define nelem_d		"nelem=%d"
#define nmin_d		"nmin=%d"
#define snr_s		"snr=%s"
#define pid_d		"pid=%d"
#define refc_d		"rc=%d"
#define ao_s		"ao=%s"
#define sig_s		"sig=%s"
#define sig_d		"sig=%d"
#define prio_d		"prio=%d"
#define ist_s		"ist=%s"
#define h_s			"h=%s"
#define st_s		"st=%s"
#define sst_s		"sst=%s"
#define tst_s		"tst=%s"
#define nxtst_s		"nxtst=%s"
#define nen_d		"nen=%d"
#define nex_d		"nex=%d"
#define nta_d		"nta=%d"
#define nts_d		"nts=%d"
#define t_s			"t=%s"
#define ntick_d		"nt=%d"
#define per_d		"per=%d"
#define obj_x		"obj=0x%08X"
#define nm_s		"nm=%s"
#define func_x		"fn=0x%08X"
#define func_s		"fn=%s"
#define file_s		"%s.c"
#define line_d		"(%d)"
#define usrtrc_d	"ut=%d"
#define ep_d		"ep=%d"
#define ss_d		"ss=%d"
#define es_d		"es=%d"
#define p_d			"p=%d"


/* --- Memory Pool (MP) ------------------------ */
DCLR_TRE( RKH_TE_MP_INIT,		"MP", "INIT",		h_mp_init,	mp_s, nblock_d, bsize_d );
DCLR_TRE( RKH_TE_MP_GET,		"MP", "GET",		h_mp_get,	mp_s, nfree_d, nmin_d  );
DCLR_TRE( RKH_TE_MP_PUT,		"MP", "PUT",		h_symnblk,	mp_s, nfree_d  );


/* --- Queue (RQ) ------------------------ */
DCLR_TRE( RKH_TE_RQ_INIT,		"RQ", "INIT",       h_2symnused,q_s, ao_s, nelem_d );
DCLR_TRE( RKH_TE_RQ_GET,		"RQ", "GET",        h_symnused,	q_s, nelem_d );
DCLR_TRE( RKH_TE_RQ_FIFO,		"RQ", "FIFO",       h_rq_ffll,	q_s, nelem_d, nmin_d );
DCLR_TRE( RKH_TE_RQ_LIFO,		"RQ", "LIFO",       h_rq_ffll,	q_s, nelem_d, nmin_d );
DCLR_TRE( RKH_TE_RQ_FULL,		"RQ", "FULL",       h_1sym,		q_s );
DCLR_TRE( RKH_TE_RQ_DPT,		"RQ", "DPT",        h_1sym,		q_s );
DCLR_TRE( RKH_TE_RQ_GET_LAST,	"RQ", "GET_LAST",   h_1sym,		q_s );


/* --- State Machine Application (SMA) --- */
DCLR_TRE( RKH_TE_SMA_ACT,		"SMA", "ACT",		h_symu8,	ao_s, p_d );
DCLR_TRE( RKH_TE_SMA_TERM,		"SMA", "TERM",		h_symu8,	ao_s, p_d );
DCLR_TRE( RKH_TE_SMA_GET,		"SMA", "GET",		h_sma_get,	ao_s, sig_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_SMA_FIFO,		"SMA", "FIFO",		h_sma_ffll, ao_s, sig_s, snr_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_SMA_LIFO,		"SMA", "LIFO",		h_sma_ffll, ao_s, sig_s, snr_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_SMA_REG,		"SMA", "REG",		h_symu8,	ao_s, prio_d );
DCLR_TRE( RKH_TE_SMA_UNREG,		"SMA", "UNREG",		h_symu8,	ao_s, prio_d );


/* --- State machine (SM) ---------------- */
DCLR_TRE( RKH_TE_SM_INIT,       "SM",  "INIT",       h_2sym,	ao_s, ist_s );
DCLR_TRE( RKH_TE_SM_CLRH,       "SM",  "CLRH",       h_2sym,	ao_s, h_s );
DCLR_TRE( RKH_TE_SM_DCH,        "SM",  "DCH",        h_symevt,	ao_s, sig_s );
DCLR_TRE( RKH_TE_SM_TRN,        "SM",  "TRN",        h_symtrn,	ao_s, sst_s, tst_s );
DCLR_TRE( RKH_TE_SM_STATE,      "SM",  "STATE",	     h_2sym,	ao_s, nxtst_s );
DCLR_TRE( RKH_TE_SM_ENSTATE,    "SM",  "ENSTATE",    h_2sym,	ao_s, st_s );
DCLR_TRE( RKH_TE_SM_EXSTATE,    "SM",  "EXSTATE",    h_2sym,	ao_s, st_s );
DCLR_TRE( RKH_TE_SM_NENEX,      "SM",  "NENEX",	     h_sym2u8,	ao_s, nen_d, nex_d );
DCLR_TRE( RKH_TE_SM_NTRNACT,    "SM",  "NTRNACT",    h_sym2u8,	ao_s, nta_d, nts_d );
DCLR_TRE( RKH_TE_SM_TS_STATE,   "SM",  "TS_STATE",	 h_2sym,	ao_s, st_s );
DCLR_TRE( RKH_TE_SM_EVT_PROC,   "SM",  "EVT_PROC",	 h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_EVT_NFOUND, "SM",  "EVT_NFOUND", h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_GRD_FALSE,  "SM",  "GRD_FALSE",  h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_CND_NFOUND, "SM",  "CND_NFOUND", h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_UNKN_STATE, "SM",  "UNKN_STATE", h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_EX_HLEVEL,  "SM",  "EX_HLEVEL",  h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_EX_TSEG,    "SM",  "EX_TSEG",    h_1sym,	ao_s );


/* --- Timer (TIM) ----------------------- */
DCLR_TRE( RKH_TE_TIM_INIT,		"TIM", "INIT",		h_symevt,	t_s, sig_s );
DCLR_TRE( RKH_TE_TIM_START,		"TIM", "START",		h_tstart,	t_s, ao_s, ntick_d, per_d );
DCLR_TRE( RKH_TE_TIM_STOP,		"TIM", "STOP",		h_tstop,	t_s, ntick_d, per_d );
DCLR_TRE( RKH_TE_TIM_TOUT,		"TIM", "TOUT",		h_tout,		t_s, sig_s, ao_s );
DCLR_TRE( RKH_TE_TIM_REM,		"TIM", "REM",		h_1sym,		t_s );


/* --- Framework (RKH) ------------------- */
DCLR_TRE( RKH_TE_FWK_EN,        "FWK", "EN",         h_none,	NULL );
DCLR_TRE( RKH_TE_FWK_EX,        "FWK", "EX",         h_none,	NULL );
DCLR_TRE( RKH_TE_FWK_EPREG,     "FWK", "EPREG",      h_epreg,	ep_d, ss_d, es_d );
DCLR_TRE( RKH_TE_FWK_AE,        "FWK", "AE",         h_ae,		es_d, sig_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_FWK_GC,        "FWK", "GC",         h_sig2u8,	sig_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_FWK_GCR,       "FWK", "GCR",        h_sig2u8,	sig_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_FWK_DEFER,     "FWK", "DEFER",      h_symevt,	q_s, sig_s );
DCLR_TRE( RKH_TE_FWK_RCALL,     "FWK", "RCALL",      h_symevt,	ao_s, sig_s );
DCLR_TRE( RKH_TE_FWK_OBJ,       "FWK", "OBJ",        h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_SIG,       "FWK", "SIG",        h_symsig,	sig_d, nm_s );
DCLR_TRE( RKH_TE_FWK_FUN,		"FWK", "FUN",        h_symobj,	func_x, nm_s );
DCLR_TRE( RKH_TE_FWK_EXE_FUN,	"FWK", "EXE_FUN",    h_1sym,	func_s );
DCLR_TRE( RKH_TE_FWK_TUSR,      "FWK", "TUSR",       h_symuevt,	usrtrc_d, nm_s );
DCLR_TRE( RKH_TE_FWK_TCFG,      "FWK", "TCFG",       h_tcfg,	NULL );
DCLR_TRE( RKH_TE_FWK_ASSERT,    "FWK", "ASSERT",     h_assert,	file_s, line_d );
DCLR_TRE( RKH_TE_FWK_AO,        "FWK", "AO",         h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_STATE,     "FWK", "STATE",      h_symst,	ao_s, obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_PSTATE,    "FWK", "PSTATE",     h_symst,	ao_s, obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_TIMER,     "FWK", "TIMER",      h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_EPOOL,     "FWK", "EPOOL",      h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_QUEUE,     "FWK", "QUEUE",      h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_USER,          "USR", "USR#",       NULL,		NULL );


static FMT_ID_T *fmt_id_tbl[] =
{
	TRE_ST(RKH_TE_MP_INIT),
	TRE_ST( RKH_TE_MP_GET),
	TRE_ST( RKH_TE_MP_PUT),


	/* --- Queue (RQ) ------------------------ */
	TRE_ST( RKH_TE_RQ_INIT),
	TRE_ST( RKH_TE_RQ_GET),
	TRE_ST( RKH_TE_RQ_FIFO),
	TRE_ST( RKH_TE_RQ_LIFO),
	TRE_ST( RKH_TE_RQ_FULL),
	TRE_ST( RKH_TE_RQ_DPT),
	TRE_ST( RKH_TE_RQ_GET_LAST),


	/* --- State Machine Application (SMA) --- */
	TRE_ST( RKH_TE_SMA_ACT),
	TRE_ST( RKH_TE_SMA_TERM),
	TRE_ST( RKH_TE_SMA_GET),
	TRE_ST( RKH_TE_SMA_FIFO),
	TRE_ST( RKH_TE_SMA_LIFO),
	TRE_ST( RKH_TE_SMA_REG),
	TRE_ST( RKH_TE_SMA_UNREG),


	/* --- State machine (SM) ---------------- */
	TRE_ST( RKH_TE_SM_INIT),
	TRE_ST( RKH_TE_SM_CLRH),
	TRE_ST( RKH_TE_SM_DCH),
	TRE_ST( RKH_TE_SM_TRN),
	TRE_ST( RKH_TE_SM_STATE),
	TRE_ST( RKH_TE_SM_ENSTATE),
	TRE_ST( RKH_TE_SM_EXSTATE),
	TRE_ST( RKH_TE_SM_NENEX),
	TRE_ST( RKH_TE_SM_NTRNACT),
	TRE_ST( RKH_TE_SM_TS_STATE),
	TRE_ST( RKH_TE_SM_EVT_PROC),
	TRE_ST( RKH_TE_SM_EVT_NFOUND),
	TRE_ST( RKH_TE_SM_GRD_FALSE),
	TRE_ST( RKH_TE_SM_CND_NFOUND),
	TRE_ST( RKH_TE_SM_UNKN_STATE),
	TRE_ST( RKH_TE_SM_EX_HLEVEL),
	TRE_ST( RKH_TE_SM_EX_TSEG),


	/* --- Timer (TIM) ----------------------- */
	TRE_ST( RKH_TE_TIM_INIT),
	TRE_ST( RKH_TE_TIM_START),
	TRE_ST( RKH_TE_TIM_STOP),
	TRE_ST( RKH_TE_TIM_TOUT),
	TRE_ST( RKH_TE_TIM_REM),


	/* --- Framework (RKH) ------------------- */
	TRE_ST( RKH_TE_FWK_EN),
	TRE_ST( RKH_TE_FWK_EX),
	TRE_ST( RKH_TE_FWK_EPREG),
	TRE_ST( RKH_TE_FWK_AE),
	TRE_ST( RKH_TE_FWK_GC),
	TRE_ST( RKH_TE_FWK_GCR),
	TRE_ST( RKH_TE_FWK_DEFER),
	TRE_ST( RKH_TE_FWK_RCALL),
	TRE_ST( RKH_TE_FWK_OBJ),
	TRE_ST( RKH_TE_FWK_SIG),
	TRE_ST( RKH_TE_FWK_FUN),
	TRE_ST( RKH_TE_FWK_EXE_FUN),
	TRE_ST( RKH_TE_FWK_TUSR),
	TRE_ST( RKH_TE_FWK_TCFG),
	TRE_ST( RKH_TE_FWK_ASSERT),
	TRE_ST( RKH_TE_FWK_AO),
	TRE_ST( RKH_TE_FWK_STATE),
	TRE_ST( RKH_TE_FWK_PSTATE),
	TRE_ST( RKH_TE_FWK_TIMER),
	TRE_ST( RKH_TE_FWK_EPOOL),
	TRE_ST( RKH_TE_FWK_QUEUE),
	TRE_ST( RKH_TE_USER)
};

#if 0
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
#endif

TRE_T fmt_usr_tbl = 
{
	RKH_TE_USER, "USR", "USR#", {vector<char *>(1, "2")},  usr_fmt
};


FMT_ID_T *
get_evt_id( string *idstr )
{
	FMT_ID_T *p;

	for( p=fmt_id_tbl[0]; p->tre.id != RKH_TE_USER; ++p )
		if( idstr->compare(p->idstr) == 0 )
			return p;

	return NULL;
}


const
TRE_T *
find_trevt( unsigned char id )
{
	static FMT_ID_T **p;
	static char i;

/*	for( i=0, p=fmt_id_tbl[i]; p->tre.id != RKH_TE_USER; ++i )
	{
		p=fmt_id_tbl[i];
		if( id == p->tre.id )
				return &(p->tre);
	}*/
	for( p=&fmt_id_tbl[0]; (*p)->tre.id != RKH_TE_USER; ++p )
	{
		if( id == (*p)->tre.id )
				return &((*p)->tre);
	}
	return ( TRE_T* )0;
}

const
TRE_T *
point_2_trevt( unsigned char ix )
{
	return &fmt_id_tbl[ix]->tre;
}
