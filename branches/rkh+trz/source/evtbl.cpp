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

#define q_s			(char *)"q=%s"
#define mp_s		(char *)"mp=%s"
#define nblock_d	(char *)"nb=%d"
#define bsize_d		(char *)"bs=%d"
#define nfree_d		(char *)"nfree=%d"
#define nelem_d		(char *)"nelem=%d"
#define nmin_d		(char *)"nmin=%d"
#define snr_s		(char *)"snr=%s"
#define pid_d		(char *)"pid=%d"
#define refc_d		(char *)"rc=%d"
#define ao_s		(char *)"ao=%s"
#define sig_s		(char *)"sig=%s"
#define sig_d		(char *)"sig=%d"
#define prio_d		(char *)"prio=%d"
#define ist_s		(char *)"ist=%s"
#define h_s			(char *)"h=%s"
#define st_s		(char *)"st=%s"
#define sst_s		(char *)"sst=%s"
#define tst_s		(char *)"tst=%s"
#define nxtst_s		(char *)"nxtst=%s"
#define nen_d		(char *)"nen=%d"
#define nex_d		(char *)"nex=%d"
#define nta_d		(char *)"nta=%d"
#define nts_d		(char *)"nts=%d"
#define t_s			(char *)"t=%s"
#define ntick_d		(char *)"nt=%d"
#define per_d		(char *)"per=%d"
#define obj_x		(char *)"obj=0x%08X"
#define nm_s		(char *)"nm=%s"
#define func_x		(char *)"fn=0x%08X"
#define func_s		(char *)"fn=%s"
#define file_s		(char *)"%s.c"
#define line_d		(char *)"(%d)"
#define usrtrc_d	(char *)"ut=%d"
#define ep_d		(char *)"ep=%d"
#define ss_d		(char *)"ss=%d"
#define es_d		(char *)"es=%d"
#define p_d			(char *)"p=%d"
#define rt_d		(char *)"rt=%d"
#define aty_s		(char *)"%s"


/* --- Memory Pool events (MP group) ----------------------------------- */
DCLR_TRE( RKH_TE_MP_INIT,		"MP", "INIT",		h_mp_init,	mp_s, nblock_d, bsize_d );
DCLR_TRE( RKH_TE_MP_GET,		"MP", "GET",		h_mp_get,	mp_s, nfree_d, nmin_d  );
DCLR_TRE( RKH_TE_MP_PUT,		"MP", "PUT",		h_symnblk,	mp_s, nfree_d  );

/* --- Queue events (RQ group) ----------------------------------------- */
DCLR_TRE( RKH_TE_RQ_INIT,		"RQ", "INIT",       h_2symnused,q_s, ao_s, nelem_d );
DCLR_TRE( RKH_TE_RQ_GET,		"RQ", "GET",        h_symnused,	q_s, nelem_d );
DCLR_TRE( RKH_TE_RQ_FIFO,		"RQ", "FIFO",       h_rq_ffll,	q_s, nelem_d, nmin_d );
DCLR_TRE( RKH_TE_RQ_LIFO,		"RQ", "LIFO",       h_rq_ffll,	q_s, nelem_d, nmin_d );
DCLR_TRE( RKH_TE_RQ_FULL,		"RQ", "FULL",       h_1sym,		q_s );
DCLR_TRE( RKH_TE_RQ_DPT,		"RQ", "DPT",        h_1sym,		q_s );
DCLR_TRE( RKH_TE_RQ_GET_LAST,	"RQ", "GET_LAST",   h_1sym,		q_s );

/* --- State Machine Application events (SMA group) -------------------- */
DCLR_TRE( RKH_TE_SMA_ACT,		"SMA", "ACT",		h_symu8,	ao_s, p_d );
DCLR_TRE( RKH_TE_SMA_TERM,		"SMA", "TERM",		h_symu8,	ao_s, p_d );
DCLR_TRE( RKH_TE_SMA_GET,		"SMA", "GET",		h_sma_get,	ao_s, sig_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_SMA_FIFO,		"SMA", "FIFO",		h_sma_ff,   ao_s, sig_s, snr_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_SMA_LIFO,		"SMA", "LIFO",		h_sma_lf,   ao_s, sig_s, snr_s, pid_d, refc_d );
DCLR_TRE( RKH_TE_SMA_REG,		"SMA", "REG",		h_symu8,	ao_s, prio_d );
DCLR_TRE( RKH_TE_SMA_UNREG,		"SMA", "UNREG",		h_symu8,	ao_s, prio_d );
DCLR_TRE( RKH_TE_SMA_DCH,       "SMA", "DCH",       h_sma_dch,	ao_s, sig_s, st_s, rt_d );

/* --- State machine events (SM group) --------------------------------- */
DCLR_TRE( RKH_TE_SM_INIT,       "SM",  "INIT",       h_2sym,	ao_s, ist_s );
DCLR_TRE( RKH_TE_SM_CLRH,       "SM",  "CLRH",       h_2sym,	ao_s, h_s );
DCLR_TRE( RKH_TE_SM_TRN,        "SM",  "TRN",        h_symtrn,	ao_s, sst_s, tst_s );
DCLR_TRE( RKH_TE_SM_STATE,      "SM",  "STATE",	     h_2sym,	ao_s, nxtst_s );
DCLR_TRE( RKH_TE_SM_ENSTATE,    "SM",  "ENSTATE",    h_2sym,	ao_s, st_s );
DCLR_TRE( RKH_TE_SM_EXSTATE,    "SM",  "EXSTATE",    h_2sym,	ao_s, st_s );
DCLR_TRE( RKH_TE_SM_NENEX,      "SM",  "NENEX",	     h_sym2u8,	ao_s, nen_d, nex_d );
DCLR_TRE( RKH_TE_SM_NTRNACT,    "SM",  "NTRNACT",    h_sym2u8,	ao_s, nta_d, nts_d );
DCLR_TRE( RKH_TE_SM_TS_STATE,   "SM",  "TS_STATE",	 h_2sym,	ao_s, st_s );
DCLR_TRE( RKH_TE_SM_EVT_PROC,   "SM",  "EVT_PROC",	 h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_EVT_NFOUND, "SM",  "EVT_NFOUND", h_symevt,	ao_s, sig_s );
DCLR_TRE( RKH_TE_SM_GRD_FALSE,  "SM",  "GRD_FALSE",  h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_CND_NFOUND, "SM",  "CND_NFOUND", h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_UNKN_STATE, "SM",  "UNKN_STATE", h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_EX_HLEVEL,  "SM",  "EX_HLEVEL",  h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_EX_TSEG,    "SM",  "EX_TSEG",    h_1sym,	ao_s );
DCLR_TRE( RKH_TE_SM_EXE_ACT,    "SM",  "EXE_ACT",    h_exact,	aty_s, ao_s, st_s, func_s );

/* --- Timer events (TIM group) ---------------------------------------- */
DCLR_TRE( RKH_TE_TMR_INIT,		"TIM", "INIT",		h_tinit,	t_s, sig_s );
DCLR_TRE( RKH_TE_TMR_START,		"TIM", "START",		h_tstart,	t_s, ao_s, ntick_d, per_d );
DCLR_TRE( RKH_TE_TMR_STOP,		"TIM", "STOP",		h_tstop,	t_s, ntick_d, per_d );
DCLR_TRE( RKH_TE_TMR_TOUT,		"TIM", "TOUT",		h_tout,		t_s, sig_s, ao_s );
DCLR_TRE( RKH_TE_TMR_REM,		"TIM", "REM",		h_1sym,		t_s );

/* --- Framework and misc. events (FWK group) -------------------------- */
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
DCLR_TRE( RKH_TE_FWK_SYNC_EVT,	"FWK", "SYNC_EVT",   h_sync,	func_s, snr_s, ao_s );
DCLR_TRE( RKH_TE_FWK_TUSR,      "FWK", "TUSR",       h_symuevt,	usrtrc_d, nm_s );
DCLR_TRE( RKH_TE_FWK_TCFG,      "FWK", "TCFG",       h_tcfg,	NULL );
DCLR_TRE( RKH_TE_FWK_ASSERT,    "FWK", "ASSERT",     h_assert,	file_s, line_d );
DCLR_TRE( RKH_TE_FWK_AO,        "FWK", "AO",         h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_STATE,     "FWK", "STATE",      h_symst,	ao_s, obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_PSTATE,    "FWK", "PSTATE",     h_symst,	ao_s, obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_TIMER,     "FWK", "TIMER",      h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_EPOOL,     "FWK", "EPOOL",      h_symobj,	obj_x, nm_s );
DCLR_TRE( RKH_TE_FWK_QUEUE,     "FWK", "QUEUE",      h_symobj,	obj_x, nm_s );

/* --- User events (USR group) ----------------------------------------- */
DCLR_TRE( RKH_TE_USER,          "USR", "USR#",       NULL,		NULL );

/* --- Unit test harness events (UT group) ----------------------------- */
DCLR_TRE( RKH_TE_UT_INIT,			"UT", "INIT",		NULL,	NULL );
DCLR_TRE( RKH_TE_UT_CLEANUP,		"UT", "CLEANUP",	NULL,	NULL );
DCLR_TRE( RKH_TE_UT_VERIFY,			"UT", "VERIFY",		NULL,	NULL );
DCLR_TRE( RKH_TE_UT_IGNORE_GROUP,	"UT", "IGN_GRP",	h_IgnGroup,	NULL );
DCLR_TRE( RKH_TE_UT_EXPECT,			"UT", "EXPECT",		h_Expect,	line_d, aty_s, aty_s );
DCLR_TRE( RKH_TE_UT_EXPECT_ANYARGS, "UT", "EXP_ANYARG",	h_ExpAnyArgs, line_d, aty_s );
DCLR_TRE( RKH_TE_UT_IGNORE,			"UT", "IGNORE",		NULL,	NULL );
DCLR_TRE( RKH_TE_UT_IGNORE_ARG,		"UT", "IGNORE_ARG",	NULL,	NULL );

DCLR_TRE( RKH_TE_NEVENT,          "ERR", "ERR#",       NULL,		NULL );


static FMT_ID_T *fmt_id_tbl[] =
{
	/* --- Memory Pool events (MP group) ----------------------------------- */
	TRE_ST(RKH_TE_MP_INIT),
	TRE_ST( RKH_TE_MP_GET),
	TRE_ST( RKH_TE_MP_PUT),

    /* --- Queue events (RQ group) ----------------------------------------- */
	TRE_ST( RKH_TE_RQ_INIT),
	TRE_ST( RKH_TE_RQ_GET),
	TRE_ST( RKH_TE_RQ_FIFO),
	TRE_ST( RKH_TE_RQ_LIFO),
	TRE_ST( RKH_TE_RQ_FULL),
	TRE_ST( RKH_TE_RQ_DPT),
	TRE_ST( RKH_TE_RQ_GET_LAST),

    /* --- State Machine Application events (SMA group) -------------------- */	
	TRE_ST( RKH_TE_SMA_ACT),
	TRE_ST( RKH_TE_SMA_TERM),
	TRE_ST( RKH_TE_SMA_GET),
	TRE_ST( RKH_TE_SMA_FIFO),
	TRE_ST( RKH_TE_SMA_LIFO),
	TRE_ST( RKH_TE_SMA_REG),
	TRE_ST( RKH_TE_SMA_UNREG),
	TRE_ST( RKH_TE_SMA_DCH),

    /* --- State machine events (SM group) --------------------------------- */
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

    /* --- Timer events (TIM group) ---------------------------------------- */
	TRE_ST( RKH_TE_TMR_INIT),
	TRE_ST( RKH_TE_TMR_START),
	TRE_ST( RKH_TE_TMR_STOP),
	TRE_ST( RKH_TE_TMR_TOUT),
	TRE_ST( RKH_TE_TMR_REM),


    /* --- Framework and misc. events (FWK group) -------------------------- */
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

    /* --- User events (USR group) ----------------------------------------- */	
	TRE_ST( RKH_TE_USER),

    /* --- Unit test harness events (UT group) ----------------------------- */
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

TRE_T fmt_usr_tbl = 
{
	RKH_TE_USER, "USR", "USR#", {vector<char *>(1, (char *)"2")},  usr_fmt
};


FMT_ID_T *
get_evt_id( string *idstr )
{
	FMT_ID_T *p;

	for( p=fmt_id_tbl[0]; p->tre.id != RKH_TE_NEVENT; ++p )
		if( idstr->compare(p->idstr) == 0 )
			return p;

	return NULL;
}


const
TRE_T *
find_trevt( unsigned int id )
{
	static FMT_ID_T **p;

	for( p=&fmt_id_tbl[0]; (*p)->tre.id != RKH_TE_NEVENT; ++p )
	{
		if( id == (*p)->tre.id )
				return &((*p)->tre);
	}
	return ( TRE_T* )0;
}

const
TRE_T *
point_2_trevt( unsigned int ix )
{
	return &fmt_id_tbl[ix]->tre;
}

string
trevt_name( unsigned int ix )
{
	return fmt_id_tbl[ix]->idstr;
}


/********* Groups tables *******/

#define DCLR_TRG( id, st, nm )		\
	static TRG_T st_##id = 			\
	{ 							    \
	    id, st, nm, 	 			\
	};

#define TRG_ST(id)		&st_##id


DCLR_TRG( RKH_TG_MP,    RKH_MP_START,   "MP" );
DCLR_TRG( RKH_TG_RQ,        RKH_RQ_START,   "RQ" );
DCLR_TRG( RKH_TG_SMA,       RKH_SMA_START,  "SMA" );
DCLR_TRG( RKH_TG_SM,        RKH_SM_START,   "SM" );
DCLR_TRG( RKH_TG_TMR,       RKH_TMR_START,  "TMR" );
DCLR_TRG( RKH_TG_FWK,       RKH_FWK_START,  "FWK" );
DCLR_TRG( RKH_TG_USR,       RKH_USR_START,  "USR" );
DCLR_TRG( RKH_TG_UT,        RKH_UT_START,   "UT" );
DCLR_TRG( RKH_TG_NGROUP,    0,              "NONE" );

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
find_trgroup( unsigned int id )
{
	static TRG_T **p;

	for( p=&grp_tbl[0]; (*p)->id != RKH_TG_NGROUP; ++p )
	{
		if( id == (*p)->id )
			return (*p);
	}
	return (TRG_T *)0;
}
