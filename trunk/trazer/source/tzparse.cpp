/*
 *	file: trazer.c
 *	Last updated for version: 2.0
 *	Date of the last update:  Apr 03, 2012
 */


#include "mytypes.h"
#include "rkhtrc.h"
//#include "rkh.h"
//#include "rkhcfg.h"
//#include "my.h"
#include "mdebug.h"
#include "myevt.h"
#include "tzparse.h"
#include "evtbl.h"
#include "symbtbl.h"
#include "sigtbl.h"
#include "cfgdep.h"
#include "tzlog.h"
//#include "rkhtim.h"
#include <stdio.h>

//#define TRAZER_NTRE			event_tbl.size()
//#define TRAZER_NTRG			RKH_TRCG_NGROUP
#define CC(s)				((const char*)(s))

#if 0
#if TRAZER_SIZEOF_TSTAMP == 1
	typedef unsigned char TRZTS_T;
#elif TRAZER_SIZEOF_TSTAMP == 2
	typedef unsigned short TRZTS_T;
#elif TRAZER_SIZEOF_TSTAMP == 4
	typedef unsigned long TRZTS_T;
#else
	typedef unsigned char TRZTS_T;
#endif
#else
	typedef unsigned long TRZTS_T;
#endif

#if 0
#if TRAZER_SIZEOF_NBLOCK == 1
	typedef unsigned char TRZNB_T;
#elif TRAZER_SIZEOF_NBLOCK == 2
	typedef unsigned short TRZNB_T;
#elif TRAZER_SIZEOF_NBLOCK == 4
	typedef unsigned long TRZNB_T;
#else
	typedef unsigned char TRZNB_T;
#endif
#else
	typedef unsigned long TRZNB_T;
#endif

#if 0
#if TRAZER_SIZEOF_NELEM == 1
	typedef unsigned char TRZNE_T;
#elif TRAZER_SIZEOF_NELEM == 2
	typedef unsigned short TRZNE_T;
#elif TRAZER_SIZEOF_NELEM == 4
	typedef unsigned long TRZNE_T;
#else
	typedef unsigned char TRZNE_T;
#endif
#else
	typedef unsigned long TRZNE_T;
#endif

#if 0
#if TRAZER_SIZEOF_NTIMER == 1
	typedef unsigned char TRZNT_T;
#elif TRAZER_SIZEOF_NTIMER == 2
	typedef unsigned short TRZNT_T;
#elif TRAZER_SIZEOF_NTIMER == 4
	typedef unsigned long TRZNT_T;
#else
	typedef unsigned char TRZNT_T;
#endif
#else
	typedef unsigned long TRZNT_T;
#endif

#if 0
#if TRAZER_SIZEOF_ESIZE == 1
	typedef unsigned char TRZES_T;
#elif TRAZER_SIZEOF_ESIZE == 2
	typedef unsigned short TRZES_T;
#elif TRAZER_SIZEOF_ESIZE == 4
	typedef unsigned long TRZES_T;
#else
	typedef unsigned char TRZES_T;
#endif
#else
	typedef unsigned long TRZES_T;
#endif

#if 0
#if TRAZER_EN_NSEQ == 1
	#define get_nseq()		nseq = tr[ 1 ]
	#define set_to_ts()		trb = tr + 2
#else
	#define get_nseq()		nseq = 0
	#define set_to_ts()		trb = tr + 1
#endif
#else

	#define get_nseq()		nseq = ( TRAZER_EN_NSEQ == 1 ) ? tr[ 1 ] : 0;
	#define set_to_ts()		trb = ( TRAZER_EN_NSEQ == 1 ) ? tr + 2 : tr + 1;

static rkhui8_t lastnseq;

static
int
verify_nseq( rkhui8_t nseq )
{
	int r;

	if( TRAZER_EN_NSEQ == 0 )
		return 1;

	r = ( (rkhui8_t)( lastnseq + 1 ) == nseq ) ? 1 : 0;

	lastnseq = nseq;

	return r;
}

#endif

#if 0
#if TRAZER_EN_TSTAMP == 1
	#define get_ts()		( TRZTS_T )assemble( TRAZER_SIZEOF_TSTAMP )
#else
	#define get_ts()		0
#endif
#else
	#define get_ts()		( TRAZER_EN_TSTAMP == 1 ) ? ( TRZTS_T )assemble( TRAZER_SIZEOF_TSTAMP ) : 0
#endif

#if 0

#define MKTR( id, gn, nm, fmt, fargs )	\
				{ id, gn, nm, fmt, fargs }

#define MKS( sig, name )	\
				{ sig, name }

#define MKO( obj, nm )									\
				++i;									\
				objtbl[i].adr = (unsigned long)(obj);	\
				objtbl[i].name = nm

#define EOSIGTBL	\
				{ 0, CC( 0 ) }
#endif

#define CTE( te )		((const struct tre_t*)(te))

#if 0
static const TRE_T traces[] =
{

	/* --- Memory Pool (MP) ------------------ */
	MKTR(	RKH_TRCE_MP_INIT,	"MP", "INIT", 
			"mp=%s, nblock=%d",				h_symnblk ),
	MKTR(	 RKH_TRCE_MP_GET, 	"MP", "GET", 
			"mp=%s, nfree=%d", 				h_symnblk ),
	MKTR( 	RKH_TRCE_MP_PUT, 	"MP", "PUT", 
			"mp=%s, nfree=%d", 				h_symnblk ),
	
	/* --- Queue (RQ) ------------------------ */
	MKTR( 	RKH_TRCE_RQ_INIT,	"RQ", "INIT", 
			"rq=%s, sma=%s, nelem=%d",		h_2symnused ),
	MKTR( 	RKH_TRCE_RQ_GET, 	"RQ", "GET", 
			"rq=%s, nused=%d", 				h_symnused ),
	MKTR( 	RKH_TRCE_RQ_FIFO,	"RQ", "POST_FIFO", 
			"rq=%s, nused=%d", 				h_symnused ),
	MKTR( 	RKH_TRCE_RQ_LIFO,	"RQ", "POST_LIFO", 
			"rq=%s, nused=%d", 				h_symnused ),
	MKTR( 	RKH_TRCE_RQ_FULL,	"RQ", "FULL", 
			"rq=%s", 						h_1sym ),
	MKTR( 	RKH_TRCE_RQ_DPT,	"RQ", "DEPLETE", 
			"rq=%s", 						h_1sym ),
	MKTR( 	RKH_TRCE_RQ_GET_LAST,	"RQ", "GET_LAST", 
			"rq=%s", 						h_1sym ),

	/* --- State Machine Application (SMA) --- */
	MKTR( 	RKH_TRCE_SMA_ACT,	"SMA", "ACTIVATE", 
			"sma=%s", 						h_1sym ),
	MKTR( 	RKH_TRCE_SMA_TERM,	"SMA", "TERMINATE", 
			"sma=%s", 						h_1sym ),
	MKTR( 	RKH_TRCE_SMA_GET,	"SMA", "GET_EVENT", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKTR( 	RKH_TRCE_SMA_FIFO,	"SMA", "POST_FIFO", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKTR( 	RKH_TRCE_SMA_LIFO,	"SMA", "POST_LIFO", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKTR( 	RKH_TRCE_SMA_REG,	"SMA", "REGISTER", 
			"sma=%s, prio=%d", 				h_symu8 ),
	MKTR( 	RKH_TRCE_SMA_UNREG,	"SMA", "UNREGISTER", 
			"sma=%s, prio=%d", 				h_symu8 ),

	/* --- State machine (SM) ---------------- */
	MKTR( 	RKH_TRCE_SM_INIT,	"SM", "INIT", 
			"sma=%s, istate=%s", 			h_2sym ),
	MKTR( 	RKH_TRCE_SM_CLRH,	"SM", "CLEAR_HIST", 
			"sma=%s, hist=%s", 				h_2sym ),
	MKTR( 	RKH_TRCE_SM_DCH,	"SM", "DISPATCH", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKTR( 	RKH_TRCE_SM_TRN,	"SM", "TRANSITION", 
			"sma=%s, sstate=%s, tstate=%s", h_symtrn ),
	MKTR( 	RKH_TRCE_SM_STATE,	"SM", "CURRENT_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKTR( 	RKH_TRCE_SM_ENSTATE,"SM", "ENTRY_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKTR( 	RKH_TRCE_SM_EXSTATE,"SM", "EXIT_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKTR( 	RKH_TRCE_SM_NENEX,	"SM", "NUM_EN_EX", 
			"sma=%s, nentry=%d, nexit=%d",	h_sym2u8 ),
	MKTR( 	RKH_TRCE_SM_NTRNACT,"SM", "NUM_TRN_ACT", 
			"sma=%s, ntrnaction=%d", 		h_symu8 ),
	MKTR( 	RKH_TRCE_SM_CSTATE,	"SM", "COMP_STATE", 
			"sma=%s, state=%s", 			h_2sym ),
	MKTR( 	RKH_TRCE_SM_DCH_RC,	"SM", "DISPATCH_RCODE", 
			"sma=%s, retcode=%s", 			h_symrc ),

	/* --- Timer (TIM) ----------------------- */
	MKTR( 	RKH_TRCE_TIM_INIT,	"TIM", "INIT", 
			"timer=%s, sig=%s", 			h_symevt ),
	MKTR( 	RKH_TRCE_TIM_START,	"TIM", "START", 
			"timer=%s, sma=%s, ntick=%d", 	h_2symntick ),
	MKTR( 	RKH_TRCE_TIM_RESTART,"TIM", "RESTART", 
			"timer=%s, ntick=%5d", 			h_symntick ),
	MKTR( 	RKH_TRCE_TIM_STOP,	"TIM", "STOP", 
			"timer=%s", 					h_1sym ),
	MKTR( 	RKH_TRCE_TIM_TOUT,	"TIM", "TIMEOUT", 
			"timer=%s", 					h_1sym ),
	MKTR( 	RKH_TRCE_TIM_REM,	"TIM", "REMOVED", 
			"timer=%s", 					h_1sym ),
	MKTR( 	RKH_TRCE_TIM_ATTEMPT_STOP,	"TIM", "ATTEMPT_STOP", 
			"timer=%s", 					h_1sym ),

	/* --- Framework (RKH) ------------------- */
	MKTR( 	RKH_TRCE_RKH_EN,	"RKH", "ENTER", 
			"", 							h_none ),
	MKTR( 	RKH_TRCE_RKH_EX,	"RKH", "EXIT", 
			"", 							h_none ),
	MKTR( 	RKH_TRCE_RKH_EPREG,	"RKH", "EPOOL_REG", 
			"epix =%d, ssize=%d, esize=%d",	h_epreg ),
	MKTR( 	RKH_TRCE_RKH_AE,	"RKH", "ALLOC_EVENT", 
			"esize=%d, sig=%s", 			h_ae ),
	MKTR( 	RKH_TRCE_RKH_GC,	"RKH", "GC", 
			"sig=%s", 						h_evt ),
	MKTR( 	RKH_TRCE_RKH_GCR,	"RKH", "GC_RECYCLE", 
			"sig=%s", 						h_evt ),
	MKTR( 	RKH_TRCE_RKH_DEFER,	"RKH", "DEFER", 
			"rq=%s, sig=%s", 				h_symevt ),
	MKTR( 	RKH_TRCE_RKH_RCALL,	"RKH", "RECALL", 
			"sma=%s, sig=%s", 				h_symevt ),
	MKTR( 	RKH_TRCE_OBJ,		"RKH", "SYM_OBJ", 
			"obj=0x%08X, sym=%s", 			h_symobj ),
	MKTR( 	RKH_TRCE_SIG,		"RKH", "SYM_SIG", 
			"sig=%d, sym=%s", 				h_symsig )

};
#endif

#if 0
static const SYMSIG_T sigtbl[] = 			/* signal symbol table */
{
	MKS( ZERO,	"ZERO"	),
	MKS( ONE,	"ONE"	),
	MKS( TWO,	"TWO"	),
	MKS( THREE,	"THREE"	),
	MKS( FOUR,	"FOUR"	),
	MKS( FIVE,	"FIVE"	),
	MKS( SIX,	"SIX"	),
	MKS( SEVEN,	"SEVEN"	),
	MKS( EIGHT,	"EIGHT"	),
	MKS( NINE,	"NINE"	),
	MKS( TOUT,	"TOUT"	),
	MKS( TERM,	"TERM"	),

	EOSIGTBL
};
#endif

static const char *rctbl[] =				/* dispatch ret code table */
{
	"RKH_OK",
	"RKH_INPUT_NOT_FOUND",
	"RKH_CONDITION_NOT_FOUND",
	"RKH_GUARD_FALSE",
	"RKH_UNKNOWN_STATE",
	"RKH_EXCEED_HCAL_LEVEL",
	"RKH_EXCEED_TRC_SEGS"
};

#if 0
#if TRAZER_SIZEOF_TSTAMP == 2
	static const char *trheader = "%5d [%3d] %-4s| %-15s : ";
#else
	static const char *trheader = "%10d [%3d] %-4s| %-15s : ";
#endif
#endif

#define PARSER_MAX_SIZE_BUF			64


enum
{
	PARSER_WFLAG, PARSER_COLLECT, PARSER_ESCAPING
};


//static SYMOBJ_T objtbl[ 128 ];		/* object symbol table */
static rkhui8_t *trb;				/* points to trace event buffer */
static char fmt[ 128 ];
extern FILE *fdbg;

//static ushort my_timer;
static rkhui8_t state = PARSER_WFLAG;
static rkhui8_t tr[ PARSER_MAX_SIZE_BUF ], *ptr, trix;
static char symstr[ 16 ];

#if 0
static
void
make_symtbl( void )
{
	int i = -1;

	MKO( &my, 			"my"		);
	MKO( &my_equeue,	"my_queue"	);
	MKO( &my_timer, 	"my_timer"	);
	MKO( &rkheplist[0], "ep0"		);
	MKO( &rkheplist[1], "ep1"		);
	MKO( &S1, 			"S1"		);
	MKO( &S11, 			"S11"		);
	MKO( &S111, 		"S111"		);
	MKO( &S112, 		"S112"		);
	MKO( &S12, 			"S12"		);
	MKO( &S2, 			"S2"		);
	MKO( &S3, 			"S3" 		);
	MKO( &S31, 			"S31" 		);
	MKO( &S32, 			"S32"		);
	MKO( &C1, 			"C1" 		);
	MKO( &C2, 			"C2" 		);
	MKO( &J, 			"J" 		);
	MKO( &DH, 			"DH" 		);
	MKO( &H, 			"H" 		);

	MKO( 0, 			CC( 0 ) );
}
#endif

#if 0
static
const char *
map_obj( unsigned long adr )
{
	const SYMOBJ_T *p;

	for( p = objtbl; p->name != 0; ++p )
		if( p->adr == adr )
			return p->name;
	return CC( 0 );
}
#endif
#if 0
static
const char *
map_sig( TRZE_T sig )
{
	const SYMSIG_T *p;

	for( p = sigtbl; p->name != 0; ++p )
		if( p->sig == sig )
			return p->name;
	return CC( 0 );
}
#endif
#if 0
static
const
TRE_T *
find_trevt( unsigned char id )
{
	const TRE_T *p;

	for( p = traces; p < traces + RKH_TRCE_USER; ++p )
		if( p->id == id )
			return p;

	return ( TRE_T* )0;
}
#endif


static
unsigned long
assemble( int size )
{
	int n, sh;
	unsigned long d;

	for( d = 0, n = size, sh = 0; n; --n, sh += 8  )
		d |= ( unsigned long )( *trb++ << sh );
	return d;
}


static
char *
assemble_str( void  )
{
	char *p;

	for( p = symstr; *trb != '\0'; ++p, ++trb )
		*p = *trb;
	
	*p = '\0';
	return symstr;
}


char *
h_none( const void *tre )
{
	(void)tre;

	strcpy( fmt, " " );
	return fmt;
}


char *
h_1sym( const void *tre )
{
	unsigned long obj;

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj ) );
	return fmt;
}


char *
h_2sym( const void *tre )
{
	unsigned long obj1, obj2;

	obj1 = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	obj2 = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj1 ), map_obj( obj2 ) );
	return fmt;
}


char *
h_symtrn( const void *tre )
{
	unsigned long smaobj, ssobj, tsobj;

	smaobj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	ssobj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	tsobj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	sprintf( fmt, CTE( tre )->fmt, map_obj( smaobj ), map_obj( ssobj ), 
					tsobj == 0 ? map_obj( ssobj ) : map_obj( tsobj ) );
	return fmt;
}


char *
h_symrc( const void *tre )
{
	unsigned long obj;
	unsigned char u8;

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	u8 = (unsigned char)assemble( sizeof( char ) );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj ), rctbl[ u8 ] );
	return fmt;
}


char *
h_symu8( const void *tre )
{
	unsigned long obj;
	unsigned char u8;

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	u8 = (unsigned char)assemble( sizeof( char ) );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj ), u8 );
	return fmt;
}


char *
h_symnblk( const void *tre )
{
	unsigned long obj;
	TRZNB_T nblock;

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	nblock = (TRZNB_T)assemble( sizeof_trznb() );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj ), nblock );
	return fmt;
}


char *
h_2symnused( const void *tre )
{
	unsigned long obj1, obj2;
	TRZNE_T nelem;

	obj1 = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	obj2 = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	nelem = (TRZNE_T)assemble( sizeof_trzne() );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj1 ), map_obj( obj2 ), nelem );
	return fmt;
}


char *
h_symnused( const void *tre )
{
	unsigned long obj;
	TRZNE_T nelem;

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	nelem = (TRZNE_T)assemble( sizeof_trzne() );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj ), nelem );
	return fmt;
}


char *
h_2symntick( const void *tre )
{
	unsigned long obj1, obj2;
	TRZNT_T ntick;

	obj1 = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	obj2 = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	ntick = (TRZNT_T)assemble( sizeof_trznt() );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj1 ), map_obj( obj2 ), ntick );
	return fmt;
}


char *
h_symntick( const void *tre )
{
	unsigned long obj1;
	TRZNT_T ntick;

	obj1 = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	ntick = (TRZNT_T)assemble( sizeof_trznt() );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj1 ), ntick );
	return fmt;
}


char *
h_sym2u8( const void *tre )
{
	unsigned long obj;
	unsigned char u8_1, u8_2;

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	u8_1 = (unsigned char)assemble( sizeof( char ) );
	u8_2 = (unsigned char)assemble( sizeof( char ) );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj ), u8_1, u8_2 );
	return fmt;
}


char *
h_evt( const void *tre )
{
	TRZE_T e;

	e = (TRZE_T)assemble( sizeof_trze() );
	sprintf( fmt, CTE( tre )->fmt, map_sig( e ) );
	return fmt;
}


char *
h_symevt( const void *tre )
{
	unsigned long obj;
	TRZE_T e;

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	e = (TRZE_T)assemble( sizeof_trze() );
	sprintf( fmt, CTE( tre )->fmt, map_obj( obj ), map_sig( e ) );
	return fmt;
}


char *
h_epreg( const void *tre )
{
	unsigned long u32;
	TRZES_T esize;
	unsigned char u8;

	u8 = (unsigned char)assemble( sizeof( char ) );
	u32 = (unsigned long)assemble( sizeof( long ) );
	esize = (TRZES_T)assemble( sizeof_trzes() );
	sprintf( fmt, CTE( tre )->fmt, u8, u32, esize  );
	return fmt;
}


char *
h_ae( const void *tre )
{
	TRZES_T esize;
	TRZE_T e;

	esize = (TRZES_T)assemble( sizeof_trzes() );
	e = (TRZE_T)assemble( sizeof_trze() );
	sprintf( fmt, CTE( tre )->fmt, esize, map_sig( e ) );
	return fmt;
}


char *
h_symobj( const void *tre )
{
	unsigned long obj;
	char *s;

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	s = assemble_str();
	sprintf( fmt, CTE( tre )->fmt, obj, s );
	add_to_symtbl( obj, s );
	return fmt;
}


char *
h_symsig( const void *tre )
{
	TRZE_T e;
	char *s;

	e = (TRZE_T)assemble( sizeof_trze() );
	s = assemble_str();
	sprintf( fmt, CTE( tre )->fmt, e, s );
	add_to_sigtbl( e, s );
	return fmt;
}


static
void
parser_init( void )
{
	ptr = tr;
	trix = 0;
}


static
void
parser_collect( rkhui8_t d )
{
	if( ++trix > PARSER_MAX_SIZE_BUF )
	{
		state = PARSER_WFLAG;
		return;
	}
	*ptr++ = d;
}


static
int
parser_chk( void )
{
	rkhui8_t *p, chk;
	int i;
	if( TRAZER_EN_CHK == 1 )
	{
		for( chk = 0, p = tr, i = trix; i--; ++p )
			chk = (rkhui8_t)( chk + *p );
		return chk == 0;	
	}
	return 1;
}

void
show_curr_frm( void )
{
	rkhui8_t *p;
	int i;
	dprintf( "---- |");

	if( TRAZER_EN_CHK == 1 )
	{
		for( p = tr, i = trix; i--; ++p )
			dprintf( "0x%X|", *p );
	}
	dprintf( " ----\n" );
}


static
void
parser( void )
{
	const TRE_T *ftr;			/* received trace event */

	TRZTS_T ts;
	rkhui8_t nseq;

	if( ( ftr = find_trevt( tr[ 0 ] ) ) != ( TRE_T* )0 )
	{
		if( ftr->fmt_args != ( HDLR_T )0 )
		{

			get_nseq();

			if( !verify_nseq( nseq ) )
				lprintf( "\n***** May be have lost trace info, sequence are not correlatives\n" );

			set_to_ts();		/* from timestamp field */
			ts = get_ts();
			lprintf( trheader, ts, nseq, ftr->group.c_str(), ftr->name.c_str() );
			lprintf( "%s\n", (*ftr->fmt_args)( CTE( ftr ) ) );
		}
		return;
	}
	
	lprintf( "***** Unknown trace event = (%d)\n", tr[0] );
}


void 
trazer_parse( rkhui8_t d )
{
	switch( state )
	{
		case PARSER_WFLAG:
			if( d == RKH_FLG )
			{
				parser_init();
				state = PARSER_COLLECT;
			}
			break;
		case PARSER_COLLECT:
			if( d == RKH_FLG )
			{
				if( trix > 0 )
				{
					if( parser_chk() )
					{
						show_curr_frm();
						parser();
					}
					else
					{
						lprintf( "\n***** Stream Checksum Error\n" );
						show_curr_frm();
					}
				}

				parser_init();
			}
			else if( d == RKH_ESC )
				state = PARSER_ESCAPING;
			else
				parser_collect( d );
			break;
		case PARSER_ESCAPING:
			if( d == RKH_FLG )
			{
				parser_init();
				state = PARSER_COLLECT;
			}
			else
			{
				parser_collect( (rkhui8_t)(d ^ RKH_XOR) );
				state = PARSER_COLLECT;
			}
			break;
		default:
			break;
	}
}

#include "version.h"
void
trazer_init( void )
{
	lastnseq = 255;
	lprintf(VERSION_STRING_TXT);
	lprintf( "Date = "__DATE__" "__TIME__"\n" );
	lprintf( "\nTrace Setup\n\n" );
//	lprintf( "   Trace events quantity = %d\n", TRAZER_NTRE );
//	lprintf( "Number of trace groups = %d\n", TRAZER_NTRG );
	lprintf( "   TRAZER_SIZEOF_SIG     = %d\n", TRAZER_SIZEOF_SIG );
	lprintf( "   TRAZER_SIZEOF_TSTAMP  = %d\n", TRAZER_SIZEOF_TSTAMP );
	lprintf( "   TRAZER_SIZEOF_POINTER = %d\n", TRAZER_SIZEOF_POINTER );
	lprintf( "   TRAZER_SIZEOF_NTIMER  = %d\n", TRAZER_SIZEOF_NTIMER );
	lprintf( "   TRAZER_SIZEOF_NBLOCK  = %d\n", TRAZER_SIZEOF_NBLOCK );
	lprintf( "   TRAZER_SIZEOF_NELEM   = %d\n", TRAZER_SIZEOF_NELEM );
	lprintf( "   TRAZER_SIZEOF_ESIZE   = %d\n", TRAZER_SIZEOF_ESIZE );
	lprintf( "   TRAZER_EN_NSEQ        = %d\n", TRAZER_EN_NSEQ );
	lprintf( "   TRAZER_EN_CHK         = %d\n", TRAZER_EN_CHK );
	lprintf( "   TRAZER_EN_TSTAMP      = %d\n", TRAZER_EN_TSTAMP );
	lprintf( "   RKH_TRC_ALL           = %d\n", RKH_TRC_ALL );
	lprintf( "   RKH_TRC_EN_MP         = %d\n", RKH_TRC_EN_MP );
	lprintf( "   RKH_TRC_EN_RQ         = %d\n", RKH_TRC_EN_RQ );
	lprintf( "   RKH_TRC_EN_SMA        = %d\n", RKH_TRC_EN_SMA );
	lprintf( "   RKH_TRC_EN_TIM        = %d\n", RKH_TRC_EN_TIM );
	lprintf( "   RKH_TRC_EN_SM         = %d\n", RKH_TRC_EN_SM );
	lprintf( "   RKH_TRC_EN_RKH        = %d\n", RKH_TRC_EN_RKH );
	//lprintf( "\n---- BEGIN TRACE SESSION ----\n\n" );
	lprintf( "\n" );
}
