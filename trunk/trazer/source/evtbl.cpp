/*
 * evtbl.cpp
 */

#include "trzfrmt.h"
#include "evtbl.h"

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


DCLR_TRE( RKH_TE_NEVENT,    "ERR",  "ERR#", NULL,   NULL );

/* --- Memory Pool events (MP group) ----------------------------------- */
DCLR_TRE( RKH_TE_MP_INIT,   "MP",   "INIT", h_mp_init,	mp_s,   nblock_d,   bsize_d );
DCLR_TRE( RKH_TE_MP_GET,    "MP",   "GET",	h_mp_get,	mp_s,   nfree_d,    nmin_d  );
DCLR_TRE( RKH_TE_MP_PUT,    "MP",   "PUT",	h_symnblk,  mp_s,   nfree_d  );

static FMT_ID_T *tg_mp_tbl[] =
{
	TRE_ST(RKH_TE_MP_INIT),
	TRE_ST( RKH_TE_MP_GET),
	TRE_ST( RKH_TE_MP_PUT),
    TRE_ST( RKH_TE_NEVENT )
};

/* --- Queue events (RQ group) ----------------------------------------- */
DCLR_TRE( RKH_TE_RQ_INIT,		"RQ", "INIT",       h_2symnused,q_s, ao_s, nelem_d );
DCLR_TRE( RKH_TE_RQ_GET,		"RQ", "GET",        h_symnused,	q_s, nelem_d );
DCLR_TRE( RKH_TE_RQ_FIFO,		"RQ", "FIFO",       h_rq_ffll,	q_s, nelem_d, nmin_d );
DCLR_TRE( RKH_TE_RQ_LIFO,		"RQ", "LIFO",       h_rq_ffll,	q_s, nelem_d, nmin_d );
DCLR_TRE( RKH_TE_RQ_FULL,		"RQ", "FULL",       h_1sym,		q_s );
DCLR_TRE( RKH_TE_RQ_DPT,		"RQ", "DPT",        h_1sym,		q_s );
DCLR_TRE( RKH_TE_RQ_GET_LAST,	"RQ", "GET_LAST",   h_1sym,		q_s );

static FMT_ID_T *tg_rq_tbl[] =
{
	TRE_ST( RKH_TE_RQ_INIT),
	TRE_ST( RKH_TE_RQ_GET),
	TRE_ST( RKH_TE_RQ_FIFO),
	TRE_ST( RKH_TE_RQ_LIFO),
	TRE_ST( RKH_TE_RQ_FULL),
	TRE_ST( RKH_TE_RQ_DPT),
	TRE_ST( RKH_TE_RQ_GET_LAST),
    TRE_ST( RKH_TE_NEVENT )
};

/* --- State Machine Application events (SMA group) -------------------- */
DCLR_TRE( RKH_TE_SMA_ACT,		"SMA", "ACT",		h_sma_act,	ao_s, prio_d, size_d );
DCLR_TRE( RKH_TE_SMA_TERM,		"SMA", "TERM",		h_symu8,	ao_s, prio_d );
DCLR_TRE( RKH_TE_SMA_GET,		"SMA", "GET",		h_sma_get,	ao_s, sig_s, pid_d, refc_d, nelem_d, nmin_d );
DCLR_TRE( RKH_TE_SMA_FIFO,		"SMA", "FIFO",		h_sma_ff,   ao_s, sig_s, snr_s, pid_d, refc_d, nelem_d, nmin_d );
DCLR_TRE( RKH_TE_SMA_LIFO,		"SMA", "LIFO",		h_sma_lf,   ao_s, sig_s, snr_s, pid_d, refc_d, nelem_d, nmin_d );
DCLR_TRE( RKH_TE_SMA_REG,		"SMA", "REG",		h_symu8,	ao_s, prio_d );
DCLR_TRE( RKH_TE_SMA_UNREG,		"SMA", "UNREG",		h_symu8,	ao_s, prio_d );
DCLR_TRE( RKH_TE_SMA_DCH,       "SMA", "DCH",       h_sma_dch,	ao_s, sig_s, st_s, rt_d );

static FMT_ID_T *tg_sma_tbl[] =
{
	TRE_ST( RKH_TE_SMA_ACT),
	TRE_ST( RKH_TE_SMA_TERM),
	TRE_ST( RKH_TE_SMA_GET),
	TRE_ST( RKH_TE_SMA_FIFO),
	TRE_ST( RKH_TE_SMA_LIFO),
	TRE_ST( RKH_TE_SMA_REG),
	TRE_ST( RKH_TE_SMA_UNREG),
	TRE_ST( RKH_TE_SMA_DCH),
    TRE_ST( RKH_TE_NEVENT )
};

/* --- State machine events (SM group) --------------------------------- */
DCLR_TRE( RKH_TE_SM_INIT,       "SM", "INIT",       h_2sym,	  ao_s, ist_s );
DCLR_TRE( RKH_TE_SM_CLRH,       "SM", "CLRH",       h_2sym,	  ao_s, h_s );
DCLR_TRE( RKH_TE_SM_TRN,        "SM", "TRN",        h_symtrn, ao_s, sst_s, tst_s );
DCLR_TRE( RKH_TE_SM_STATE,      "SM", "STATE",	    h_2sym,	  ao_s, nxtst_s );
DCLR_TRE( RKH_TE_SM_ENSTATE,    "SM", "ENSTATE",    h_2sym,	  ao_s, st_s );
DCLR_TRE( RKH_TE_SM_EXSTATE,    "SM", "EXSTATE",    h_2sym,	  ao_s, st_s );
DCLR_TRE( RKH_TE_SM_NENEX,      "SM", "NENEX",	    h_sym2u8, ao_s, nen_d, nex_d );
DCLR_TRE( RKH_TE_SM_NTRNACT,    "SM", "NTRNACT",    h_sym2u8, ao_s, nta_d, nts_d );
DCLR_TRE( RKH_TE_SM_TS_STATE,   "SM", "TS_STATE",	h_2sym,	  ao_s, st_s );
DCLR_TRE( RKH_TE_SM_EVT_PROC,   "SM", "EVT_PROC",	h_1sym,	  ao_s );
DCLR_TRE( RKH_TE_SM_EVT_NFOUND, "SM", "EVT_NFOUND", h_symevt, ao_s, sig_s );
DCLR_TRE( RKH_TE_SM_GRD_FALSE,  "SM", "GRD_FALSE",  h_1sym,	  ao_s );
DCLR_TRE( RKH_TE_SM_CND_NFOUND, "SM", "CND_NFOUND", h_1sym,	  ao_s );
DCLR_TRE( RKH_TE_SM_UNKN_STATE, "SM", "UNKN_STATE", h_1sym,	  ao_s );
DCLR_TRE( RKH_TE_SM_EX_HLEVEL,  "SM", "EX_HLEVEL",  h_1sym,	  ao_s );
DCLR_TRE( RKH_TE_SM_EX_TSEG,    "SM", "EX_TSEG",    h_1sym,	  ao_s );
DCLR_TRE( RKH_TE_SM_EXE_ACT,    "SM", "EXE_ACT",    h_exact,  aty_s, ao_s, st_s, func_s );

static FMT_ID_T *tg_sm_tbl[] =
{
	TRE_ST( RKH_TE_SM_INIT),
	TRE_ST( RKH_TE_SM_CLRH),
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
	TRE_ST( RKH_TE_SM_EXE_ACT),
    TRE_ST( RKH_TE_NEVENT )
};

/* --- Timer events (TIM group) ---------------------------------------- */
DCLR_TRE( RKH_TE_TMR_INIT,	"TIM", "INIT",	h_tinit,  t_s, sig_s );
DCLR_TRE( RKH_TE_TMR_START,	"TIM", "START",	h_tstart, t_s, ao_s, ntick_d, per_d );
DCLR_TRE( RKH_TE_TMR_STOP,	"TIM", "STOP",	h_tstop,  t_s, ntick_d, per_d );
DCLR_TRE( RKH_TE_TMR_TOUT,	"TIM", "TOUT",	h_tout,	  t_s, sig_s, ao_s );
DCLR_TRE( RKH_TE_TMR_REM,	"TIM", "REM",	h_1sym,	  t_s );

static FMT_ID_T *tg_tmr_tbl[] =
{
	TRE_ST( RKH_TE_TMR_INIT),
	TRE_ST( RKH_TE_TMR_START),
	TRE_ST( RKH_TE_TMR_STOP),
	TRE_ST( RKH_TE_TMR_TOUT),
	TRE_ST( RKH_TE_TMR_REM),
    TRE_ST( RKH_TE_NEVENT )
};

/* --- Framework and misc. events (FWK group) -------------------------- */
DCLR_TRE( RKH_TE_FWK_EN,      "FWK", "EN",       h_none,	NULL );
DCLR_TRE( RKH_TE_FWK_EX,      "FWK", "EX",       h_none,	NULL );
DCLR_TRE( RKH_TE_FWK_EPREG,   "FWK", "EPREG",    h_epreg,	ep_d, ss_d, es_d, psize_d );
DCLR_TRE( RKH_TE_FWK_AE,      "FWK", "AE",       h_ae, es_d, sig_s, pid_d, refc_d, nblock_d, nmin_d, actor_s  );
DCLR_TRE( RKH_TE_FWK_GC,      "FWK", "GC",       h_sig2u8,	sig_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_FWK_GCR,     "FWK", "GCR",      h_gcr,	sig_s, pid_d, refc_d, nblock_d, nmin_d, actor_s );
DCLR_TRE( RKH_TE_FWK_DEFER,   "FWK", "DEFER",    h_symevt,	q_s, sig_s );
DCLR_TRE( RKH_TE_FWK_RCALL,   "FWK", "RCALL",    h_symevt,	ao_s, sig_s );
DCLR_TRE( RKH_TE_FWK_OBJ,     "FWK", "OBJ",      h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_SIG,     "FWK", "SIG",      h_symsig,	sig_d, nm_s );
DCLR_TRE( RKH_TE_FWK_FUN,	  "FWK", "FUN",      h_symobj,	func_x, nm_s );
DCLR_TRE( RKH_TE_FWK_EXE_FUN, "FWK", "EXE_FUN",  h_1sym,	func_s );
DCLR_TRE( RKH_TE_FWK_SYNC_EVT,"FWK", "SYNC_EVT", h_sync,	func_s, snr_s, ao_s );
DCLR_TRE( RKH_TE_FWK_TUSR,    "FWK", "TUSR",     h_symuevt,	usrtrc_d, nm_s );
DCLR_TRE( RKH_TE_FWK_TCFG,    "FWK", "TCFG",     h_tcfg,	NULL );
DCLR_TRE( RKH_TE_FWK_ASSERT,  "FWK", "ASSERT",   h_assert,	line_d, file_s);
DCLR_TRE( RKH_TE_FWK_AO,      "FWK", "AO",       h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_STATE,   "FWK", "STATE",    h_symst,	ao_s, obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_PSTATE,    "FWK", "PSTATE", h_symst,	ao_s, obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_TIMER,     "FWK", "TIMER",  h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_EPOOL,     "FWK", "EPOOL",  h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_QUEUE,     "FWK", "QUEUE",  h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_ACTOR,     "FWK", "ACTOR",  h_symobj,	obj_x, nm_s );

static FMT_ID_T *tg_fwk_tbl[] =
{
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
	TRE_ST( RKH_TE_FWK_SYNC_EVT),
	TRE_ST( RKH_TE_FWK_TUSR),
	TRE_ST( RKH_TE_FWK_TCFG),
	TRE_ST( RKH_TE_FWK_ASSERT),
	TRE_ST( RKH_TE_FWK_AO),
	TRE_ST( RKH_TE_FWK_STATE),
	TRE_ST( RKH_TE_FWK_PSTATE),
	TRE_ST( RKH_TE_FWK_TIMER),
	TRE_ST( RKH_TE_FWK_EPOOL),
	TRE_ST( RKH_TE_FWK_QUEUE),
	TRE_ST( RKH_TE_FWK_ACTOR),
    TRE_ST( RKH_TE_NEVENT )
};

/* --- User events (USR group) ----------------------------------------- */
DCLR_TRE( RKH_TE_USER,          "USR", "USR#",       NULL,		NULL );

TRE_T fmt_usr_tbl = 
{
	RKH_TE_USER, "USR", "USR#", {vector<char *>(1, (char *)"2")},  usr_fmt
};

static FMT_ID_T *tg_usr_tbl[] =
{
	TRE_ST( RKH_TE_USER),
    TRE_ST( RKH_TE_NEVENT )
};

/* --- Unit test harness events (UT group) ----------------------------- */
DCLR_TRE( RKH_TE_UT_INIT,			"UT", "INIT",	    h_utInit,	 line_d );
DCLR_TRE( RKH_TE_UT_CLEANUP,		"UT", "CLEANUP",    h_utCleanup, line_d );
DCLR_TRE( RKH_TE_UT_VERIFY,			"UT", "VERIFY",	    h_utVerify,  line_d );
DCLR_TRE( RKH_TE_UT_IGNORE_GROUP,	"UT", "IGN_GRP",    h_IgnGroup,  line_d, aty_s );
DCLR_TRE( RKH_TE_UT_EXPECT,			"UT", "EXPECT", 	h_Expect,    line_d, aty_s, aty_s );
DCLR_TRE( RKH_TE_UT_EXPECT_ANYARGS, "UT", "EXP_ANYARG",	h_ExpAnyArg, line_d, aty_s );
DCLR_TRE( RKH_TE_UT_IGNORE,			"UT", "IGNORE",		h_IgnEvt,    line_d, aty_s );
DCLR_TRE( RKH_TE_UT_IGNORE_ARG,		"UT", "IGNORE_ARG",	h_IgnArg,	 line_d, aty_s, arg_d );


/* --- Trace Event Groups Table ---------------------------------------- */
static FMT_ID_T *tg_ut_tbl[] =
{
    TRE_ST( RKH_TE_UT_INIT ),
    TRE_ST( RKH_TE_UT_CLEANUP ),
    TRE_ST( RKH_TE_UT_VERIFY ),
    TRE_ST( RKH_TE_UT_IGNORE_GROUP ),
    TRE_ST( RKH_TE_UT_EXPECT ),
    TRE_ST( RKH_TE_UT_EXPECT_ANYARGS ),
    TRE_ST( RKH_TE_UT_IGNORE ),
    TRE_ST( RKH_TE_UT_IGNORE_ARG ),
    TRE_ST( RKH_TE_NEVENT )
};


static FMT_ID_T **tgroups_tbl[] =
{
    tg_mp_tbl, tg_rq_tbl, tg_sma_tbl, tg_sm_tbl,
    tg_tmr_tbl, tg_fwk_tbl, tg_usr_tbl, tg_ut_tbl
};


const
TRE_T *
find_trevt( unsigned int id )
{
	static FMT_ID_T **p;
    RKH_TG_T grp;
    RKH_TE_ID_T te;

    grp = GETGRP(id);
    if( grp >= RKH_TG_NGROUP )
	    return ( TRE_T* )0;

    te = EXTE( id, grp );

	for( p = tgroups_tbl[grp]; (*p)->tre.id != RKH_TE_NEVENT; ++p )
	{
		if( id == (*p)->tre.id )
				return &((*p)->tre);
	}
	return ( TRE_T* )0;
}


/********* Groups tables *******/

#define DCLR_TRG( id, nm )		\
	static TRG_T st_##id = 	    \
	{ 							\
	    id, nm, 	 			\
	};

#define TRG_ST(id)		&st_##id


DCLR_TRG( RKH_TG_MP,        "MP" );
DCLR_TRG( RKH_TG_RQ,        "RQ" );
DCLR_TRG( RKH_TG_SMA,       "SMA" );
DCLR_TRG( RKH_TG_SM,        "SM" );
DCLR_TRG( RKH_TG_TMR,       "TMR" );
DCLR_TRG( RKH_TG_FWK,       "FWK" );
DCLR_TRG( RKH_TG_USR,       "USR" );
DCLR_TRG( RKH_TG_UT,        "UT" );
DCLR_TRG( RKH_TG_NGROUP,    "NONE" );

static TRG_T *grp_tbl[] =
{
    TRG_ST(RKH_TG_MP),
    TRG_ST(RKH_TG_RQ),
    TRG_ST(RKH_TG_SMA),
    TRG_ST(RKH_TG_SM),
    TRG_ST(RKH_TG_TMR),
    TRG_ST(RKH_TG_FWK),
    TRG_ST(RKH_TG_USR),
    TRG_ST(RKH_TG_UT),

    TRG_ST(RKH_TG_NGROUP) 
};

const
TRG_T *
find_trgroup( RKH_TG_T grp )
{
	static TRG_T **p;

	for( p=&grp_tbl[0]; (*p)->grp != RKH_TG_NGROUP; ++p )
	{
		if( grp == (*p)->grp )
			return (*p);
	}
	return (TRG_T *)0;
}
