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

//vector <EVENT_INFO_ST> event_tbl;

#define MAX_EVENT_TBL_SIZE	100

#define MKFI( id, gn, nm, fmt, fargs )	\
	{ { id, gn, nm, fmt, fargs }, #id }
	
static FMT_ID_T fmt_id_tbl[] =
{
	MKFI(	RKH_TE_MP_INIT,	"MP", "INIT", 
			"mp=%s, nblock=%d",				h_symnblk ),
	MKFI(	 RKH_TE_MP_GET, 	"MP", "GET", 
			"mp=%s, nfree=%d", 				h_symnblk ),
	MKFI( 	RKH_TE_MP_PUT, 	"MP", "PUT", 
			"mp=%s, nfree=%d", 				h_symnblk ),
	
	/* --- Queue (RQ) ------------------------ */
	MKFI( 	RKH_TE_RQ_INIT,	"RQ", "INIT", 
			"rq=%s, sma=%s, nelem=%d",		h_2symnused ),
	MKFI( 	RKH_TE_RQ_GET, 	"RQ", "GET", 
			"rq=%s, nused=%d", 				h_symnused ),
	MKFI( 	RKH_TE_RQ_FIFO,	"RQ", "POST_FIFO", 
			"rq=%s, nused=%d", 				h_symnused ),
	MKFI( 	RKH_TE_RQ_LIFO,	"RQ", "POST_LIFO", 
			"rq=%s, nused=%d", 				h_symnused ),
	MKFI( 	RKH_TE_RQ_FULL,	"RQ", "FULL", 
			"rq=%s", 						h_1sym ),
	MKFI( 	RKH_TE_RQ_DPT,	"RQ", "DEPLETE", 
			"rq=%s", 						h_1sym ),
	MKFI( 	RKH_TE_RQ_GET_LAST,	"RQ", "GET_LAST", 
			"rq=%s", 						h_1sym ),

	/* --- State Machine Application (SMA) --- */
	MKFI( 	RKH_TE_SMA_ACT,	"SMA", "ACTIVATE", 
			"sma=%s", 						h_1sym ),
	MKFI( 	RKH_TE_SMA_TERM,	"SMA", "TERMINATE", 
			"sma=%s", 						h_1sym ),
	MKFI( 	RKH_TE_SMA_GET,	"SMA", "GET_EVENT", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TE_SMA_FIFO,	"SMA", "POST_FIFO", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TE_SMA_LIFO,	"SMA", "POST_LIFO", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TE_SMA_REG,	"SMA", "REGISTER", 
			"sma=%s, prio=%d", 				h_symu8 ),
	MKFI( 	RKH_TE_SMA_UNREG,	"SMA", "UNREGISTER", 
			"sma=%s, prio=%d", 				h_symu8 ),

	/* --- State machine (SM) ---------------- */
	MKFI( 	RKH_TE_SM_INIT,	"SM", "INIT", 
			"sma=%s, istate=%s", 			h_2sym ),
	MKFI( 	RKH_TE_SM_CLRH,	"SM", "CLEAR_HIST", 
			"sma=%s, hist=%s", 				h_2sym ),
	MKFI( 	RKH_TE_SM_DCH,	"SM", "DISPATCH", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TE_SM_TRN,	"SM", "TRANSITION", 
			"sma=%s, sstate=%s, tstate=%s", h_symtrn ),
	MKFI( 	RKH_TE_SM_STATE,	"SM", "CURRENT_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKFI( 	RKH_TE_SM_ENSTATE,"SM", "ENTRY_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKFI( 	RKH_TE_SM_EXSTATE,"SM", "EXIT_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKFI( 	RKH_TE_SM_NENEX,	"SM", "NUM_EN_EX", 
			"sma=%s, nentry=%d, nexit=%d",	h_sym2u8 ),
	MKFI( 	RKH_TE_SM_NTRNACT,"SM", "NUM_TRN_ACT", 
			"sma=%s, ntrnaction=%d", 		h_symu8 ),
	MKFI( 	RKH_TE_SM_CSTATE,	"SM", "COMP_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKFI( 	RKH_TE_SM_EVT_PROC,	"SM", "EVT_PROCESSED", 
			"sma=%s",		 				h_1sym ),
	MKFI( 	RKH_TE_SM_EVT_NFOUND, "SM", "EVT_NOT_FOUND", 
			"sma=%s",		 				h_1sym ),
	MKFI( 	RKH_TE_SM_GRD_FALSE, "SM", "GUARD_FALSE", 
			"sma=%s",		 				h_1sym ),
	MKFI( 	RKH_TE_SM_CND_NFOUND, "SM", "COND_NOT_FOUND", 
			"sma=%s",		 				h_1sym ),
	MKFI( 	RKH_TE_SM_UNKN_STATE, "SM", "UNKN_STATE", 
			"sma=%s",		 				h_1sym ),
	MKFI( 	RKH_TE_SM_EX_HLEVEL, "SM", "EXCEED_HLEVEL", 
			"sma=%s",		 				h_1sym ),
	MKFI( 	RKH_TE_SM_EX_TSEG, "SM", "EXCEED_TSEG", 
			"sma=%s",		 				h_1sym ),

	/* --- Timer (TIM) ----------------------- */
	MKFI( 	RKH_TE_TIM_INIT,	"TIM", "INIT", 
			"timer=%s, sig=%s", 			h_symevt ),
	MKFI( 	RKH_TE_TIM_START,	"TIM", "START", 
			"timer=%s, sma=%s, ntick=%d", 	h_2symntick ),
	MKFI( 	RKH_TE_TIM_STOP,	"TIM", "STOP", 
			"timer=%s", 					h_1sym ),
	MKFI( 	RKH_TE_TIM_TOUT,	"TIM", "TIMEOUT", 
			"timer=%s", 					h_1sym ),
	MKFI( 	RKH_TE_TIM_REM,	"TIM", "REMOVED", 
			"timer=%s", 					h_1sym ),

	/* --- Framework (RKH) ------------------- */
	MKFI( 	RKH_TE_FWK_EN,	"FWK", "ENTER", 
			"", 							h_none ),
	MKFI( 	RKH_TE_FWK_EX,	"FWK", "EXIT", 
			"", 							h_none ),
	MKFI( 	RKH_TE_FWK_EPREG,	"FWK", "EPOOL_REG", 
			"epix =%d, ssize=%d, esize=%d",	h_epreg ),
	MKFI( 	RKH_TE_FWK_AE,	"FWK", "ALLOC_EVENT", 
			"esize=%d, sig=%s", 			h_ae ),
	MKFI( 	RKH_TE_FWK_GC,	"FWK", "GC", 
			"sig=%s", 						h_evt ),
	MKFI( 	RKH_TE_FWK_GCR,	"FWK", "GC_RECYCLE", 
			"sig=%s", 						h_evt ),
	MKFI( 	RKH_TE_FWK_DEFER,	"FWK", "DEFER", 
			"rq=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TE_FWK_RCALL,	"FWK", "RECALL", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TE_FWK_OBJ,		"FWK", "SYM_OBJ", 
			"obj=0x%08X, sym=%s", 			h_symobj ),
	MKFI( 	RKH_TE_FWK_SIG,		"FWK", "SYM_SIG", 
			"sig=%d, sym=%s", 				h_symsig ),
	MKFI( 	RKH_TE_FWK_FUN,		"FWK", "SYM_FUN", 
			"obj=0x%08X, sym=%s", 			h_symobj ),
	MKFI( 	RKH_TE_FWK_EXE_FUN,	"FWK", "EXE_FUN", 
			"fun=%s", 						h_1sym ),
	MKFI( 	RKH_TE_FWK_TUSR,	"FWK", "SYM_USER", 
			"uevt=%d, sym=%s", 				h_symuevt ),
	MKFI( 	RKH_TE_FWK_TCFG,	"FWK", "TCFG", 
			NULL, 							h_tcfg ),
	MKFI( 	RKH_TE_FWK_ASSERT,	"FWK", "ASSERT", 
			"%s.c (%d)", 					h_assert ),

	MKFI( 	RKH_TE_USER,	"USR", "USR#", 
			NULL, 	NULL )
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

#if 0
int
add_to_evtbl( EVENT_INFO_ST *p )
{
	if( p->event.empty() ||
		p->group.empty() ||
		p->name.empty() 
	)
	{
		printf( "%s", error_incomplete_trace_evt_data);
		return -1;
	}

	if( (p->fmtid = get_evt_id( &p->event )) == NULL )
	{
		printf( error_event_id_unknown, p->event.c_str() );
		return -1;
	}


	if( event_tbl.empty() )
		event_tbl.reserve( MAX_EVENT_TBL_SIZE );

	if( event_tbl.size() == MAX_EVENT_TBL_SIZE )
	{
		printf( "\nThere are no space in Event Table\n" );
		return -1;
	}

	event_tbl.push_back( *p );

	fmt_id_tbl[p->fmtid->tre.id].evinfo = &event_tbl.back(); 
	fmt_id_tbl[p->fmtid->tre.id].tre.group.assign( event_tbl.back().group.c_str() );
	fmt_id_tbl[p->fmtid->tre.id].tre.name.assign( event_tbl.back().name.c_str() );

	p->event.clear();
	p->group.clear();
	p->name.clear();
	p->args.clear();
	p->comment.clear();

	return 0;
}
#endif


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
