/*
 * evtbl.cpp
 */

#include <stdio.h>
#include "tzlog.h"
#include "evtbl.h"
#include "rkhtrc.h"
#include "tzparse.h"
#include "messages.h"
#include "trzfrmt.h"
#include "evexptbl.h"
#include "tcp.h"

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

#define EXP_TRE_ST(id, cf)		{ &st_##id, cf }

DCLR_EXP_TRE( RKH_TE_NEVENT,    "ERR",  "ERR#", NULL,   NULL );

/* --- Memory Pool events (MP group) ----------------------------------- */
static FMT_EXP_ST exp_tg_mp_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT,         c_none)
};

/* --- Queue events (RQ group) ----------------------------------------- */
static FMT_EXP_ST exp_tg_rq_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT,         c_none)
};

/* --- State Machine Application events (SMA group) -------------------- */
static FMT_EXP_ST exp_tg_sma_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT,         c_none)
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


static FMT_EXP_ST exp_tg_sm_tbl[] =
{
    EXP_TRE_ST( RKH_TE_SM_INIT,        c_sminit),
	EXP_TRE_ST( RKH_TE_SM_CLRH,        c_smclrh),
	EXP_TRE_ST( RKH_TE_SM_TRN,         c_smtrn),
	EXP_TRE_ST( RKH_TE_SM_STATE,       c_smst),  
	EXP_TRE_ST( RKH_TE_SM_ENSTATE,     c_smenst),  
	EXP_TRE_ST( RKH_TE_SM_EXSTATE,     c_smexst),  
	EXP_TRE_ST( RKH_TE_SM_NENEX,       c_smnenex), 
	EXP_TRE_ST( RKH_TE_SM_NTRNACT,     c_smntrna), 
	EXP_TRE_ST( RKH_TE_SM_TS_STATE,    c_smtsst),  
	EXP_TRE_ST( RKH_TE_SM_EVT_PROC,    c_smevpr),  
	EXP_TRE_ST( RKH_TE_SM_EVT_NFOUND,  c_smnfnd),  
	EXP_TRE_ST( RKH_TE_SM_GRD_FALSE,   c_smgf),    
	EXP_TRE_ST( RKH_TE_SM_CND_NFOUND,  c_smnf),    
	EXP_TRE_ST( RKH_TE_SM_UNKN_STATE,  c_smukst),  
	EXP_TRE_ST( RKH_TE_SM_EX_HLEVEL,   c_smexhl),  
	EXP_TRE_ST( RKH_TE_SM_EX_TSEG,     c_smextseg),
	EXP_TRE_ST( RKH_TE_SM_EXE_ACT,     c_smexeact),
    EXP_TRE_ST( RKH_TE_NEVENT,         c_none)
};

/* --- Timer events (TIM group) ---------------------------------------- */
static FMT_EXP_ST exp_tg_tmr_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT,         c_none)
};

/* --- Framework and misc. events (FWK group) -------------------------- */
static FMT_EXP_ST exp_tg_fwk_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT,         c_none)
};

/* --- User events (USR group) ----------------------------------------- */
static FMT_EXP_ST exp_tg_usr_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT,         c_none)
};

/* --- User events (USR group) ----------------------------------------- */
static FMT_EXP_ST exp_tg_ut_tbl[] =
{
    EXP_TRE_ST( RKH_TE_NEVENT,         c_none)
};

static FMT_EXP_ST *exp_tgroups_tbl[] =
{
	exp_tg_mp_tbl, exp_tg_rq_tbl, exp_tg_sma_tbl, 
    exp_tg_sm_tbl, exp_tg_tmr_tbl, exp_tg_fwk_tbl,
    exp_tg_usr_tbl, exp_tg_ut_tbl
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

	for( p = &(exp_tgroups_tbl[grp]->p); (*p)->tre.id != RKH_TE_NEVENT; ++p )
	{
		if( id == (*p)->tre.id )
				return &((*p)->tre);
	}
	return ( TRE_T* )0;
}


CHEK_ARG_T
find_exp_chk_fun( unsigned int id )
{
    FMT_EXP_ST *p;
	static CHEK_ARG_T *pf;
    RKH_TG_T grp;
    RKH_TE_ID_T te;

    grp = GETGRP(id);
    if( grp >= RKH_TG_NGROUP )
	    return ( CHEK_ARG_T )0;

    te = EXTE( id, grp );

	for( p = exp_tgroups_tbl[grp]; p->p->tre.id != RKH_TE_NEVENT; ++p )
	{
		if( id == p->p->tre.id )
			return p->chk;
	}
	return ( CHEK_ARG_T )0;
}


void
c_none( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args )
{
}

#include "sigtbl.h"
#include "symbtbl.h"

void
get_arg_sym( string *s, rui32_t arg )
{
    const char *p;
  	char buff [2+sizeof(long)*8+1];	

    if( (p = search_in_objtbl( arg )) != NULL )
        s->assign(p);
    else if( (p = search_in_sigtbl( arg )) != NULL )
        s->assign(p);
    else
    {
       	sprintf( buff, "0X%X", (unsigned int)arg );
        s->assign(buff);
    }
}

void
c_sm_no_ao( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args )
{
    rui8_t i;
	rui32_t exp_arg, rcv_arg;
	char *arg_t;
	char temp_buff[255];
	string ex_arg_s;
	string rc_arg_s;

    /* trash ao argument not used for this events */
    va_arg( args, rui32_t );
	nargs--;

    for( i=0; nargs--; ++i )
	{
		if( pex->va.args[i].ignored == ARG_IGNORED )
            continue;
        
        exp_arg = pex->va.args[i].value; 
        rcv_arg = va_arg( args, rui32_t );
        if( exp_arg != rcv_arg )
        {
			arg_t =  find_exp_trevt( pex->id )->va.args[i];
			get_arg_sym( &rc_arg_s, rcv_arg );
			sprintf( temp_buff, arg_t, rc_arg_s.c_str() );
			get_arg_sym( &ex_arg_s, exp_arg );
			utrz_ArgExpect_fail( pex->line, find_trevt(pex->id)->name.c_str(), 
					  temp_buff, ex_arg_s.c_str() );
                   
            break;
        }
	}
    utrz_success();
}

