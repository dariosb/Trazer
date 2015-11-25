/*
 * evtbl.cpp
 */

#include <stdio.h>
#include "evtbl.h"
#include "rkhtrc.h"
#include "tzparse.h"
#include "messages.h"
#include "trzfrmt.h"

using namespace std;


#define DCLR_EXP_TRE( id, gn, nm, fargs, ... )							    \
	char* exp_arg_##id[] = { __VA_ARGS__ };								    \
	static FMT_ID_T st_##id = 										        \
	{ 															        	\
	  {																        \
	    id, gn, nm, 	 											        \
	    { vector<char *>( (exp_arg_##id),								    \
		  &exp_arg_##id[0]+sizeof(exp_arg_##id)/sizeof(exp_arg_##id[0]) ) },\
	      fargs 													        \
      },															        \
	  #id															        \
	};

#define EXP_TRE_ST(id)		&st_##id

DCLR_EXP_TRE( RKH_TE_NEVENT,    "ERR",  "ERR#", NULL,   NULL );

/* --- Memory Pool events (MP group) ----------------------------------- */
static FMT_ID_T *exp_tg_mp_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT )
};

/* --- Queue events (RQ group) ----------------------------------------- */
static FMT_ID_T *exp_tg_rq_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT )
};

/* --- State Machine Application events (SMA group) -------------------- */
static FMT_ID_T *exp_tg_sma_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT )
};

/* --- State machine events (SM group) --------------------------------- */
DCLR_EXP_TRE( RKH_TE_SM_INIT,       "SM",  "INIT",       h_1sym,	ist_s );
DCLR_EXP_TRE( RKH_TE_SM_CLRH,       "SM",  "CLRH",       h_1sym,	h_s );
DCLR_EXP_TRE( RKH_TE_SM_TRN,        "SM",  "TRN",        h_2sym,	sst_s, tst_s );
DCLR_EXP_TRE( RKH_TE_SM_STATE,      "SM",  "STATE",	     h_1sym,	nxtst_s );
DCLR_EXP_TRE( RKH_TE_SM_ENSTATE,    "SM",  "ENSTATE",    h_1sym,	st_s );
DCLR_EXP_TRE( RKH_TE_SM_EXSTATE,    "SM",  "EXSTATE",    h_1sym,	st_s );
DCLR_EXP_TRE( RKH_TE_SM_NENEX,      "SM",  "NENEX",	     h_2u8,	    nen_d, nex_d );
DCLR_EXP_TRE( RKH_TE_SM_NTRNACT,    "SM",  "NTRNACT",    h_2u8,	    nta_d, nts_d );
DCLR_EXP_TRE( RKH_TE_SM_TS_STATE,   "SM",  "TS_STATE",	 h_1sym,	st_s );
DCLR_EXP_TRE( RKH_TE_SM_EVT_PROC,   "SM",  "EVT_PROC",	 h_none,    NULL );
DCLR_EXP_TRE( RKH_TE_SM_EVT_NFOUND, "SM",  "EVT_NFOUND", h_1sig,	sig_s );
DCLR_EXP_TRE( RKH_TE_SM_GRD_FALSE,  "SM",  "GRD_FALSE",  h_none,    NULL );
DCLR_EXP_TRE( RKH_TE_SM_CND_NFOUND, "SM",  "CND_NFOUND", h_none,    NULL );
DCLR_EXP_TRE( RKH_TE_SM_UNKN_STATE, "SM",  "UNKN_STATE", h_none,    NULL );
DCLR_EXP_TRE( RKH_TE_SM_EX_HLEVEL,  "SM",  "EX_HLEVEL",  h_none,    NULL );
DCLR_EXP_TRE( RKH_TE_SM_EX_TSEG,    "SM",  "EX_TSEG",    h_none,    NULL );
DCLR_EXP_TRE( RKH_TE_SM_EXE_ACT,    "SM",  "EXE_ACT",    h_exact_no_ao,	aty_s, st_s, func_s );


static FMT_ID_T *exp_tg_sm_tbl[] =
{
    EXP_TRE_ST( RKH_TE_SM_INIT),
	EXP_TRE_ST( RKH_TE_SM_CLRH),
	EXP_TRE_ST( RKH_TE_SM_TRN),
	EXP_TRE_ST( RKH_TE_SM_STATE),
	EXP_TRE_ST( RKH_TE_SM_ENSTATE),
	EXP_TRE_ST( RKH_TE_SM_EXSTATE),
	EXP_TRE_ST( RKH_TE_SM_NENEX),
	EXP_TRE_ST( RKH_TE_SM_NTRNACT),
	EXP_TRE_ST( RKH_TE_SM_TS_STATE),
	EXP_TRE_ST( RKH_TE_SM_EVT_PROC),
	EXP_TRE_ST( RKH_TE_SM_EVT_NFOUND),
	EXP_TRE_ST( RKH_TE_SM_GRD_FALSE),
	EXP_TRE_ST( RKH_TE_SM_CND_NFOUND),
	EXP_TRE_ST( RKH_TE_SM_UNKN_STATE),
	EXP_TRE_ST( RKH_TE_SM_EX_HLEVEL),
	EXP_TRE_ST( RKH_TE_SM_EX_TSEG),
	EXP_TRE_ST( RKH_TE_SM_EXE_ACT),
    EXP_TRE_ST( RKH_TE_NEVENT )
};

/* --- Timer events (TIM group) ---------------------------------------- */
static FMT_ID_T *exp_tg_tmr_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT )
};

/* --- Framework and misc. events (FWK group) -------------------------- */
static FMT_ID_T *exp_tg_fwk_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT )
};

/* --- User events (USR group) ----------------------------------------- */
static FMT_ID_T *exp_tg_usr_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT )
};

/* --- User events (USR group) ----------------------------------------- */
static FMT_ID_T *exp_tg_ut_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT )
};

static FMT_ID_T **exp_tgroups_tbl[] =
{
    exp_tg_mp_tbl, exp_tg_rq_tbl, exp_tg_sma_tbl, exp_tg_sm_tbl,
    exp_tg_tmr_tbl, exp_tg_fwk_tbl, exp_tg_usr_tbl, exp_tg_ut_tbl
};


const
TRE_T *
find_exp_trevt( unsigned int id )
{
	static FMT_ID_T **p;
    RKH_TG_T grp;
    RKH_TE_ID_T te;

    grp = GETGRP(id);
    if( grp >= RKH_TG_NGROUP )
	    return ( TRE_T* )0;

    te = EXTE( id, grp );

	for( p = exp_tgroups_tbl[grp]; (*p)->tre.id != RKH_TE_NEVENT; ++p )
	{
		if( id == (*p)->tre.id )
				return &((*p)->tre);
	}
	return ( TRE_T* )0;
}




