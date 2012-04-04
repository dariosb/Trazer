/*
 * evtbl.cpp
 */
#include <string>
#include <vector>
#include "evtbl.h"
#include "rkhtrc.h"
#include "tzparse.h"
#include "messages.h"

using namespace std;

vector <EVENT_INFO_ST> event_tbl;

#define MAX_EVENT_TBL_SIZE	100

#define MKFI( id, gn, nm, fmt, fargs )	\
	{ { id, "", "", fmt, fargs }, #id, NULL }
	
static FMT_ID_T fmt_id_tbl[] =
{
	MKFI(	RKH_TRCE_MP_INIT,	"MP", "INIT", 
			"mp=%s, nblock=%d",				h_symnblk ),
	MKFI(	 RKH_TRCE_MP_GET, 	"MP", "GET", 
			"mp=%s, nfree=%d", 				h_symnblk ),
	MKFI( 	RKH_TRCE_MP_PUT, 	"MP", "PUT", 
			"mp=%s, nfree=%d", 				h_symnblk ),
	
	/* --- Queue (RQ) ------------------------ */
	MKFI( 	RKH_TRCE_RQ_INIT,	"RQ", "INIT", 
			"rq=%s, sma=%s, nelem=%d",		h_2symnused ),
	MKFI( 	RKH_TRCE_RQ_GET, 	"RQ", "GET", 
			"rq=%s, nused=%d", 				h_symnused ),
	MKFI( 	RKH_TRCE_RQ_FIFO,	"RQ", "POST_FIFO", 
			"rq=%s, nused=%d", 				h_symnused ),
	MKFI( 	RKH_TRCE_RQ_LIFO,	"RQ", "POST_LIFO", 
			"rq=%s, nused=%d", 				h_symnused ),
	MKFI( 	RKH_TRCE_RQ_FULL,	"RQ", "FULL", 
			"rq=%s", 						h_1sym ),
	MKFI( 	RKH_TRCE_RQ_DPT,	"RQ", "DEPLETE", 
			"rq=%s", 						h_1sym ),
	MKFI( 	RKH_TRCE_RQ_GET_LAST,	"RQ", "GET_LAST", 
			"rq=%s", 						h_1sym ),

	/* --- State Machine Application (SMA) --- */
	MKFI( 	RKH_TRCE_SMA_ACT,	"SMA", "ACTIVATE", 
			"sma=%s", 						h_1sym ),
	MKFI( 	RKH_TRCE_SMA_TERM,	"SMA", "TERMINATE", 
			"sma=%s", 						h_1sym ),
	MKFI( 	RKH_TRCE_SMA_GET,	"SMA", "GET_EVENT", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TRCE_SMA_FIFO,	"SMA", "POST_FIFO", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TRCE_SMA_LIFO,	"SMA", "POST_LIFO", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TRCE_SMA_REG,	"SMA", "REGISTER", 
			"sma=%s, prio=%d", 				h_symu8 ),
	MKFI( 	RKH_TRCE_SMA_UNREG,	"SMA", "UNREGISTER", 
			"sma=%s, prio=%d", 				h_symu8 ),

	/* --- State machine (SM) ---------------- */
	MKFI( 	RKH_TRCE_SM_INIT,	"SM", "INIT", 
			"sma=%s, istate=%s", 			h_2sym ),
	MKFI( 	RKH_TRCE_SM_CLRH,	"SM", "CLEAR_HIST", 
			"sma=%s, hist=%s", 				h_2sym ),
	MKFI( 	RKH_TRCE_SM_DCH,	"SM", "DISPATCH", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TRCE_SM_TRN,	"SM", "TRANSITION", 
			"sma=%s, sstate=%s, tstate=%s", h_symtrn ),
	MKFI( 	RKH_TRCE_SM_STATE,	"SM", "CURRENT_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKFI( 	RKH_TRCE_SM_ENSTATE,"SM", "ENTRY_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKFI( 	RKH_TRCE_SM_EXSTATE,"SM", "EXIT_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKFI( 	RKH_TRCE_SM_NENEX,	"SM", "NUM_EN_EX", 
			"sma=%s, nentry=%d, nexit=%d",	h_sym2u8 ),
	MKFI( 	RKH_TRCE_SM_NTRNACT,"SM", "NUM_TRN_ACT", 
			"sma=%s, ntrnaction=%d", 		h_symu8 ),
	MKFI( 	RKH_TRCE_SM_CSTATE,	"SM", "COMP_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKFI( 	RKH_TRCE_SM_DCH_RC,	"SM", "DISPATCH_RCODE", 
			"sma=%s, retcode=%s", 			h_symrc ),

	/* --- Timer (TIM) ----------------------- */
	MKFI( 	RKH_TRCE_TIM_INIT,	"TIM", "INIT", 
			"timer=%s, sig=%s", 			h_symevt ),
	MKFI( 	RKH_TRCE_TIM_START,	"TIM", "START", 
			"timer=%s, sma=%s, ntick=%d", 	h_2symntick ),
	MKFI( 	RKH_TRCE_TIM_RESTART,"TIM", "RESTART", 
			"timer=%s, ntick=%5d", 			h_symntick ),
	MKFI( 	RKH_TRCE_TIM_STOP,	"TIM", "STOP", 
			"timer=%s", 					h_1sym ),
	MKFI( 	RKH_TRCE_TIM_TOUT,	"TIM", "TIMEOUT", 
			"timer=%s", 					h_1sym ),
	MKFI( 	RKH_TRCE_TIM_REM,	"TIM", "REMOVED", 
			"timer=%s", 					h_1sym ),
	MKFI( 	RKH_TRCE_TIM_ATTEMPT_STOP,	"TIM", "ATTEMPT_STOP", 
			"timer=%s", 					h_1sym ),

	/* --- Framework (RKH) ------------------- */
	MKFI( 	RKH_TRCE_RKH_EN,	"RKH", "ENTER", 
			"", 							h_none ),
	MKFI( 	RKH_TRCE_RKH_EX,	"RKH", "EXIT", 
			"", 							h_none ),
	MKFI( 	RKH_TRCE_RKH_EPREG,	"RKH", "EPOOL_REG", 
			"epix =%d, ssize=%d, esize=%d",	h_epreg ),
	MKFI( 	RKH_TRCE_RKH_AE,	"RKH", "ALLOC_EVENT", 
			"esize=%d, sig=%s", 			h_ae ),
	MKFI( 	RKH_TRCE_RKH_GC,	"RKH", "GC", 
			"sig=%s", 						h_evt ),
	MKFI( 	RKH_TRCE_RKH_GCR,	"RKH", "GC_RECYCLE", 
			"sig=%s", 						h_evt ),
	MKFI( 	RKH_TRCE_RKH_DEFER,	"RKH", "DEFER", 
			"rq=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TRCE_RKH_RCALL,	"RKH", "RECALL", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKFI( 	RKH_TRCE_OBJ,		"RKH", "SYM_OBJ", 
			"obj=0x%08X, sym=%s", 			h_symobj ),
	MKFI( 	RKH_TRCE_SIG,		"RKH", "SYM_SIG", 
			"sig=%d, sym=%s", 				h_symsig )
};

FMT_ID_T *
get_evt_id( string *idstr )
{
	FMT_ID_T *p;

	for( p=fmt_id_tbl; p < fmt_id_tbl + RKH_TRCE_USER; ++p )
		if( idstr->compare(p->idstr) == 0 )
			return p;

	return NULL;
}

int
add_to_evtbl( EVENT_INFO_ST *p )
{
	if( p->event.empty() ||
		p->group.empty() ||
		p->name.empty() 
	)
	{
		printf(error_incomplete_trace_evt_data);
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


const
TRE_T *
find_trevt( unsigned char id )
{
	if( id > sizeof(fmt_id_tbl)/sizeof(fmt_id_tbl[0]) )
		return ( TRE_T* )0;
	
	if( fmt_id_tbl[id].evinfo == NULL )
		return ( TRE_T* )0;

	return &(fmt_id_tbl[id].tre);
}

string *
get_evt_comment( unsigned char id )
{
	return &(fmt_id_tbl[id].evinfo->comment);
}
