/*
 *	file: trazer.c
 */


#include "mytypes.h"
#include "rkhtrc.h"
#include "mdebug.h"
#include "myevt.h"
#include "messages.h"
#include "tzparse.h"
#include "evtbl.h"
#include "symbtbl.h"
#include "version.h"
#include "sigtbl.h"
#include "uevtbl.h"
#include "cfgdep.h"
#include "tzlog.h"
#include "seqdiag.h"
#include <stdio.h>
#include <stdarg.h>



/*
#define get_nseq()		nseq = ( RKH_TRC_EN_NSEQ == 1 ) ? tr[ 1 ] : 0;
#define set_to_ts()		trb = ( RKH_TRC_EN_NSEQ == 1 ) ? tr + 2 : tr + 1;
*/

static rkhui8_t lastnseq;
rkhui16_t TSTAMP_TICK_HZ;


/*
#define get_ts()												\
			( RKH_TRC_EN_TSTAMP == 1 ) ? 						\
			( TRZTS_T )assemble( RKH_TRC_SIZEOF_TSTAMP ) : 0
*/
#define CTE( te )	((const struct tre_t*)(te))


/*
 *  dispatch ret code table
 */
static const char *rctbl[] =	
{
	"RKH_OK",
	"RKH_INPUT_NOT_FOUND",
	"RKH_CONDITION_NOT_FOUND",
	"RKH_GUARD_FALSE",
	"RKH_UNKNOWN_STATE",
	"RKH_EXCEED_HCAL_LEVEL",
	"RKH_EXCEED_TRC_SEGS"
};

#define PARSER_MAX_SIZE_BUF			64


enum
{
	PARSER_WFLAG, PARSER_COLLECT, PARSER_ESCAPING
};


static rkhui8_t *trb;				
static char fmt[ 500 ];
extern FILE *fdbg;

static rkhui8_t state = PARSER_WFLAG;
static unsigned char tr[ PARSER_MAX_SIZE_BUF ], *ptr, trix;
static char symstr[ 16 ];
static unsigned long curr_tstamp;


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


static
int
get_nseq( int en_nseq )
{
	if( en_nseq == 1 ) 
	{
		trb = tr + 2;
		return tr[ 1 ];
	}
	else
	{
		trb = tr + 1;
		return 0;
	}
}


static
int
get_ts( int en_ts, int sz_ts )
{
	return ( en_ts == 1 ) ?	( TRZTS_T )assemble( sz_ts ) : 0;
}


int
verify_nseq( rkhui8_t nseq )
{
	int r;

	if( RKH_TRC_EN_NSEQ == 0 )
		return 1;

	r = ( (rkhui8_t)( lastnseq + 1 ) == nseq ) ? 1 : 0;

	lastnseq = nseq;

	return r;
}


static
void
tre_fmtfrom( char *pb, const TRE_T *te, uchar argoff, char narg, ... )
{
    va_list args;
	vector< char * > :: const_iterator i;
	const VARGS_T *pa;
	char arg_fmt[TR_EVT_PRNBUFF_SIZE];
	char n;


	*arg_fmt = '\0';

	pa = &te->va;

	if(	pa->args.size() == 0 )
		return;

	if( argoff > ( pa->args.size() - 1 ) )
		return;


	i = pa->args.begin();
	for( n = 0; (n < argoff) && (i < pa->args.end()); ++i, ++n );

	for( n = 0; (n < narg) && (i < pa->args.end()); ++i, ++n )
	{
		if( *i != NULL )
		{
			if(n != 0)
				strcat( arg_fmt, TR_EVT_PRN_ARG_SEP );

			strcat( arg_fmt, *i );
		}
	}

    va_start( args, narg );

    vsprintf( pb, arg_fmt, args );	

    va_end(args);
}

#define tre_fmt(p,t,narg, ... )									\
			tre_fmtfrom( p,t,0,narg, __VA_ARGS__ )

#define tre_fmtfrom_cat( p, ... )								\
		{														\
			strcat( p, TR_EVT_PRN_ARG_SEP );					\
			tre_fmtfrom( p+strlen(p), __VA_ARGS__ );			\
		}
				
#define tre_fmt_cat(p,t,narg, ... )								\
		{														\
			strcat( p, TR_EVT_PRN_ARG_SEP );					\
			tre_fmtfrom( p+strlen(p),t,0,narg, __VA_ARGS__ );	\
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

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	tre_fmt( fmt, CTE(tre), 1, map_obj( obj ) );
	return fmt;
}


char *
h_2sym( const void *tre )
{
	unsigned long obj1, obj2;

	obj1 = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	obj2 = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	tre_fmt( fmt, CTE( tre ), 2, map_obj( obj1 ), map_obj( obj2 ) );
	return fmt;
}


char *
h_symtrn( const void *tre )
{
	unsigned long smaobj, ssobj, tsobj;

	smaobj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	ssobj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	tsobj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	tre_fmt( fmt, CTE( tre ), 3, map_obj( smaobj ), map_obj( ssobj ), 
					tsobj == 0 ? map_obj( ssobj ) : map_obj( tsobj ) );
	return fmt;
}


char *
h_symrc( const void *tre )
{
	unsigned long obj;
	unsigned char u8;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	u8 = (unsigned char)assemble( sizeof( char ) );
	tre_fmt( fmt, CTE( tre ), 2, map_obj( obj ), rctbl[ u8 ] );
	return fmt;
}


char *
h_symu8( const void *tre )
{
	unsigned long obj;
	unsigned char u8;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	u8 = (unsigned char)assemble( sizeof( char ) );
	tre_fmt( fmt, CTE( tre ), 2, map_obj( obj ), u8 );
	return fmt;
}


char *
h_mp_init( const void *tre )
{
	unsigned long obj;
	TRZNB_T nblock;
	unsigned long bsize;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	nblock = (TRZNB_T)assemble( sizeof_trznb() );
	bsize = (TRZNB_T)assemble( sizeof_trznb() );
	tre_fmt( fmt, CTE(tre), 3, map_obj( obj ), nblock, bsize  );
	return fmt;
}


char *
h_mp_get( const void *tre )
{
	unsigned long obj;
	TRZNB_T nblock;
	unsigned long nmin;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	nblock = (TRZNB_T)assemble( sizeof_trznb() );

	if( RKH_MP_EN_GET_LWM == 1 )
	{
		nmin = assemble( RKH_MP_SIZEOF_NBLOCK );
		tre_fmt( fmt, CTE(tre), 3, map_obj( obj ), nblock, nmin  );
	}
	else
		tre_fmt( fmt, CTE(tre), 2, map_obj( obj ), nblock );

	return fmt;
}


char *
h_symnblk( const void *tre )
{
	unsigned long obj;
	TRZNB_T nblock;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	nblock = (TRZNB_T)assemble( sizeof_trznb() );
	tre_fmt( fmt, CTE(tre), 2, map_obj( obj ), nblock  );
	return fmt;
}


char *
h_2symnused( const void *tre )
{
	unsigned long obj1, obj2;
	TRZNE_T nelem;

	obj1 = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	obj2 = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	nelem = (TRZNE_T)assemble( sizeof_trzne() );

	tre_fmt( fmt, CTE( tre ), 1, map_obj( obj1 ) );
	tre_fmtfrom_cat( fmt, CTE(tre), 1, 2, map_obj( obj2 ), nelem );
	return fmt;
}


char *
h_symnused( const void *tre )
{
	unsigned long obj;
	TRZNE_T nelem;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	nelem = (TRZNE_T)assemble( sizeof_trzne() );
	tre_fmt( fmt, CTE( tre ), 2, map_obj( obj ), nelem );
	return fmt;
}



char *
h_rq_ffll( const void *tre )
{
	unsigned long obj, nmin;
	TRZNE_T nelem;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	nelem = (TRZNE_T)assemble( sizeof_trzne() );
	if( RKH_RQ_EN_GET_LWMARK == 1 )
	{
		nmin = (unsigned long)assemble( sizeof_trzne() );
		tre_fmt( fmt, CTE( tre ), 3, map_obj( obj ), nelem, nmin );
	}
	else
		tre_fmt( fmt, CTE( tre ), 2, map_obj( obj ), nelem );
	return fmt;
}


char *
h_tstart( const void *tre )
{
	unsigned long obj1, obj2;
	TRZNT_T ntick, per;

	obj1 = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	obj2 = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	ntick = (TRZNT_T)assemble( sizeof_trznt() );
	per = (TRZNT_T)assemble( sizeof_trznt() );

	tre_fmt( fmt, CTE( tre ), 1, map_obj( obj1 ) );
	tre_fmtfrom_cat( fmt, CTE( tre ), 1, 3, map_obj( obj2 ), ntick, per );
	return fmt;
}


char *
h_tstop( const void *tre )
{
	unsigned long obj1;
	TRZNT_T ntick, per;

	obj1 = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	ntick = (TRZNT_T)assemble( sizeof_trznt() );
	per = (TRZNT_T)assemble( sizeof_trznt() );

	tre_fmt( fmt, CTE( tre ), 3, map_obj( obj1 ), ntick, per );
	return fmt;
}


char *
h_tout( const void *tre )
{
	unsigned long t, ao;
	TRZE_T sig;


	t = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	sig = (TRZE_T)assemble( sizeof_trze() );
	ao = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );

	tre_fmt( fmt, CTE( tre ), 2, map_obj( t ), map_sig(sig) );
	tre_fmtfrom_cat( fmt, CTE( tre ), 2, 1, map_obj( ao ) );

	return fmt;
}


char *
h_sym2u8( const void *tre )
{
	unsigned long obj;
	unsigned char u8_1, u8_2;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	u8_1 = (unsigned char)assemble( sizeof( char ) );
	u8_2 = (unsigned char)assemble( sizeof( char ) );
	tre_fmt( fmt, CTE( tre ), 3, map_obj( obj ), u8_1, u8_2 );
	return fmt;
}


char *
h_sig2u8( const void *tre )
{
	unsigned long sig;
	unsigned char u8_1, u8_2;

	sig = (TRZE_T)assemble( sizeof_trze() );
	u8_1 = (unsigned char)assemble( sizeof( char ) );
	u8_2 = (unsigned char)assemble( sizeof( char ) );
	tre_fmt( fmt, CTE( tre ), 3, map_sig( sig ), u8_1, u8_2 );
	return fmt;
}


char *
h_evt( const void *tre )
{
	TRZE_T e;

	e = (TRZE_T)assemble( sizeof_trze() );
	tre_fmt( fmt, CTE( tre ), 1, map_sig( e ) );
	return fmt;
}


char *
h_symevt( const void *tre )
{
	unsigned long obj;
	TRZE_T e;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	e = (TRZE_T)assemble( sizeof_trze() );
	tre_fmt( fmt, CTE( tre ), 2, map_obj( obj ), map_sig( e ) );
	return fmt;
}


char *
h_sma_get( const void *tre )
{
	unsigned long ao;
	TRZE_T e;
	unsigned char pid, refc;

	ao = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	e = (TRZE_T)assemble( sizeof_trze() );
	pid = (unsigned char)assemble( sizeof( char ) );
	refc = (unsigned char)assemble( sizeof( char ) );
	tre_fmt( fmt, CTE( tre ), 4, map_obj( ao ), map_sig( e ), pid, refc );
	return fmt;

}

static
char *
h_sma_ffll( const void *tre, TRN_ST *ptrn )
{
	TRN_ST trn;
	unsigned char pid, refc;

	trn.tobj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	trn.e = (TRZE_T)assemble( sizeof_trze() );

	if( RKH_SMA_EN_TRC_SENDER == 1 )
	{
		trn.sobj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
		pid = (unsigned char)assemble( sizeof( char ) );
		refc = (unsigned char)assemble( sizeof( char ) );
		tre_fmt( fmt, CTE( tre ), 5,
					map_obj( trn.tobj ), 
					map_sig( trn.e ), 
					map_obj( trn.sobj ), 
					pid,
			  		refc );

		add_to_trntbl( &trn );
	}
	else
	{
		pid = (unsigned char)assemble( sizeof( char ) );
		refc = (unsigned char)assemble( sizeof( char ) );
		tre_fmt( fmt, CTE( tre ), 2, 
					map_obj( trn.tobj ),
					map_sig( trn.e ));

		tre_fmtfrom_cat( fmt, CTE( tre ), 3, 2, 
					pid,
			  		refc );
	}

	*ptrn = trn;

	return fmt;
}


char *
h_sma_ff( const void *tre )
{
	TRN_ST trn;
	char *p;

	p = h_sma_ffll( tre, &trn );
	
	post_fifo_symevt( trn.tobj, trn.e, curr_tstamp );

	return p;
}


char *
h_sma_lf( const void *tre )
{
	TRN_ST trn;
	char *p;

	p = h_sma_ffll( tre, &trn );
	
	post_lifo_symevt( trn.tobj, trn.e, curr_tstamp );
	
	return p;	
}


char *
h_sma_dch( const void *tre )
{
	unsigned long obj;
	TRZE_T e;
	unsigned long post_tstamp;
	long rt;
		

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	e = (TRZE_T)assemble( sizeof_trze() );

	if( remove_symevt_tstamp( obj, e, &post_tstamp ) < 0 )
		rt = -1;
	else
		rt = ( (curr_tstamp - post_tstamp) >= 0 ) ?
						( curr_tstamp - post_tstamp ) : -1 ;

	tre_fmt( fmt, CTE( tre ), 3, map_obj( obj ), map_sig( e ), rt );
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
	tre_fmt( fmt, CTE( tre ), 3, u8, u32, esize  );
	return fmt;
}


char *
h_ae( const void *tre )
{
	TRZES_T esize;
	TRZE_T e;
	unsigned char pid, refc;

	esize = (TRZES_T)assemble( sizeof_trzes() );
	e = (TRZE_T)assemble( sizeof_trze() );
	pid = (unsigned char)assemble( sizeof(char) );
	refc = (unsigned char)assemble( sizeof(char) );
	tre_fmt( fmt, CTE( tre ), 4, esize, map_sig( e ), pid, refc );
	return fmt;
}


char *
h_symobj( const void *tre )
{
	unsigned long obj;
	char *s;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	s = assemble_str();
	
	if( *s == '&' )
		++s;

	tre_fmt( fmt, CTE( tre ), 2, obj, s );
	add_to_symtbl( obj, s );
	return fmt;
}

char *
h_symst( const void *tre )
{
	unsigned long obj;
	char *s;
	const char *ao;

	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	ao = map_obj( obj );
	obj = (unsigned long)assemble( RKH_TRC_SIZEOF_POINTER );
	s = assemble_str();
	
	if( *s == '&' )
		++s;
	if( *ao == '&' )
		++ao;

	tre_fmt( fmt, CTE( tre ), 3, ao, obj, s );
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
	tre_fmt( fmt, CTE( tre ), 2, e, s );
	add_to_sigtbl( e, s );
	return fmt;
}


char *
h_symuevt( const void *tre )
{
	uchar ue;
	char *s;

	ue = (uchar)assemble( sizeof_trze() );
	s = assemble_str();
	tre_fmt( fmt, CTE( tre ), 2, ue, s );
	add_to_uevttbl( ue, s );
	return fmt;
}

char *
h_assert( const void *tre )
{
	char *pfname;
	rkhui16_t line;

	pfname = (char *)trb;

	while( *trb++ != '\0' );

	line = (rkhui16_t)assemble( sizeof(rkhui16_t) );

	tre_fmt( fmt, CTE( tre ), 2, pfname, line );

	return fmt;
}

/*
* String describing the RKH version.
*/

#define RKH_VERSION_CODE_STRLEN	6

static char RKH_VERSION[ RKH_VERSION_CODE_STRLEN+1 ];

static
rkhui8_t *
proc_version_code( rkhui8_t *p )
{
	RKH_VERSION[6] = '\0';
	RKH_VERSION[5] = (*p & 0x0F) + '0';
	RKH_VERSION[4] = ((*p++ >> 4) & 0x0F) + '0';
	RKH_VERSION[3] = '.';
	RKH_VERSION[2] = (*p & 0x0F) + '0';
	RKH_VERSION[1] = '.';
	RKH_VERSION[0] = ((*p++ >> 4) & 0x0F) + '0';

	return p;
}


char *
h_tcfg( const void *tre )
{
	rkhui32_t *trb_32;

	trb = proc_version_code( trb );
	trb_32 = (rkhui32_t *)trb;

	RKH_SMA_EN_TRC_SENDER =	(*trb_32 >> 0) & 0x01;
	RKH_TRC_RUNTIME_FILTER =(*trb_32 >> 1) & 0x01;
	RKH_TRC_EN_USER_TRACE = (*trb_32 >> 2) & 0x01;
	RKH_TRC_ALL = 		(*trb_32 >> 3) & 0x01;
	RKH_TRC_EN_MP = 	(*trb_32 >> 4) & 0x01;
	RKH_TRC_EN_RQ = 	(*trb_32 >> 5) & 0x01;
	RKH_TRC_EN_SMA = 	(*trb_32 >> 6) & 0x01;
	RKH_TRC_EN_TIM = 	(*trb_32 >> 7) & 0x01;
	RKH_TRC_EN_SM = 	(*trb_32 >> 8) & 0x01;
	RKH_TRC_EN_FWK = 	(*trb_32 >> 9) & 0x01;
	RKH_TRC_EN_ASSERT = 	(*trb_32 >> 10) & 0x01;
	RKH_RQ_EN_GET_LWMARK = 	(*trb_32 >> 11) & 0x01;
	RKH_MP_EN_GET_LWM =	(*trb_32 >> 12) & 0x01;
	RKH_TRC_RTFIL_SMA_EN =	(*trb_32 >> 13) & 0x01;
	RKH_TRC_RTFIL_SIGNAL_EN = (*trb_32 >> 14) & 0x01;
	RKH_TRC_EN_NSEQ =	(*trb_32 >> 15) & 0x01;
	RKH_TRC_EN_TSTAMP =	(*trb_32 >> 16) & 0x01;
	RKH_TRC_EN_CHK =	(*trb_32 >> 17) & 0x01;

	++trb_32;
	trb = (rkhui8_t *)trb_32;

	RKH_SIZEOF_EVENT = (*trb >> 4) & 0x0F;
	RKH_TRC_SIZEOF_TSTAMP = (*trb++) & 0x0F;

	RKH_TRC_SIZEOF_POINTER = (*trb >> 4) & 0x0F;
	RKH_TIM_SIZEOF_NTIMER = (*trb++) & 0x0F;
	
	RKH_MP_SIZEOF_NBLOCK = (*trb >> 4) & 0x0F;
	RKH_RQ_SIZEOF_NELEM = (*trb++) & 0x0F;

	RKH_SIZEOF_ESIZE = (*trb++) & 0x0F;

	RKH_MP_SIZEOF_BSIZE = (*trb >> 4) & 0x0F;
	RKH_MAX_EPOOL = (*trb++) & 0x0F;
	
	TSTAMP_TICK_HZ = *(rkhui16_t *)trb;

	add_seqdiag_text( SEQDIAG_SEPARATOR_TEXT );
	
	lprintf( "Update RKH Configuration from client\n" );

	rkhver_printf( RKH_VERSION );
	cfg_printf( RKH_SMA_EN_TRC_SENDER );
	cfg_printf( RKH_TRC_RUNTIME_FILTER );
	cfg_printf( RKH_TRC_EN_USER_TRACE );
	cfg_printf( RKH_TRC_ALL );
	cfg_printf( RKH_TRC_EN_MP );
	cfg_printf( RKH_TRC_EN_RQ );
	cfg_printf( RKH_TRC_EN_SMA );
	cfg_printf( RKH_TRC_EN_TIM );
	cfg_printf( RKH_TRC_EN_SM );
	cfg_printf( RKH_TRC_EN_FWK );
	cfg_printf( RKH_TRC_EN_ASSERT );
	cfg_printf( RKH_RQ_EN_GET_LWMARK );
	cfg_printf( RKH_MP_EN_GET_LWM );
	cfg_printf( RKH_TRC_RTFIL_SMA_EN );
	cfg_printf( RKH_TRC_RTFIL_SIGNAL_EN );
	cfg_printf( RKH_TRC_EN_NSEQ );
	cfg_printf( RKH_TRC_EN_TSTAMP );
	cfg_printf( RKH_TRC_EN_CHK );
	cfg_printf_sz( RKH_SIZEOF_EVENT, 8 );
	cfg_printf_sz( RKH_TRC_SIZEOF_TSTAMP, 8 );
	cfg_printf_sz( RKH_TRC_SIZEOF_POINTER, 8 );
	cfg_printf_sz( RKH_TIM_SIZEOF_NTIMER, 8 );
	cfg_printf_sz( RKH_MP_SIZEOF_NBLOCK, 8 );
	cfg_printf_sz( RKH_RQ_SIZEOF_NELEM, 8 );
	cfg_printf_sz( RKH_SIZEOF_ESIZE, 8 );
	cfg_printf_sz( RKH_MP_SIZEOF_BSIZE, 8 );
	cfg_printf( RKH_MAX_EPOOL );
	cfg_printf( TSTAMP_TICK_HZ );

	return (char *)("\n");
}

typedef void (*PRNFUNC_T)( char *p, ulong data );

typedef
struct
{
	char *fmt;
	uchar size;
	PRNFUNC_T pp;
}USR_FMT_T;

typedef char (*DECOFUNC_T)( const rkhui8_t *p, const USR_FMT_T *pfmt );

typedef 
struct
{ 
	USR_FMT_T pf;
	DECOFUNC_T pc;
}USR_TBL_T;

char usr_integer( const rkhui8_t *p, const USR_FMT_T *pfmt );
char usr_string( const rkhui8_t *p, const USR_FMT_T *pfmt );
char usr_mdump( const rkhui8_t *p, const USR_FMT_T *pfmt );
char usr_object( const rkhui8_t *p, const USR_FMT_T *pfmt );
char usr_signal( const rkhui8_t *p, const USR_FMT_T *pfmt );
void i8prn( char *p, ulong data );
void i16prn( char *p, ulong data );
void i32prn( char *p, ulong data );
void u8prn( char *p, ulong data );
void u16prn( char *p, ulong data );
void u32prn( char *p, ulong data );
void strprn( char *p, ulong data );

static const USR_TBL_T usr_tbl[] =
{
	{ { (char *)"%%0%dd", 	1, i8prn},	usr_integer },	/**< signed 8-bit integer format */
	{ { (char *)"%%0%du",	1, u8prn},	usr_integer },	/**< unsigned 8-bit integer format */
	{ { (char *)"%%0%dd",	2, i16prn},	usr_integer },	/**< signed 16-bit integer format */
	{ { (char *)"%%0%du",	2, u16prn},	usr_integer },	/**< unsigned 16-bit integer format */
	{ { (char *)"%%0%dd",	4, i32prn},	usr_integer },	/**< signed 32-bit integer format */
	{ { (char *)"%%0%du",	4, u32prn},	usr_integer },	/**< unsigned 32-bit integer format */
	{ { (char *)"0x%%0%dX",	4, u32prn},	usr_integer },	/**< signed 32-bit integer in hex format */
	{ { (char *)"%s",		0, NULL},	usr_string },	/**< zero-terminated ASCII string format */
	{ { (char *)"%02X ",	0, NULL},	usr_mdump },	/**< up to 255-bytes memory block format */
	{ {	(char *)"obj=%s", 	0, NULL},	usr_object },	/**< object pointer format */
	{ {	(char *)"func=%s", 	0, NULL},	usr_object },	/**< function pointer format */
	{ {	(char *)"event=%s",	0, NULL},	usr_signal }	/**< event signal format */
};

#define USR_TRC_SEPARATOR	", "

#define usrtrz_printf(p,d)	\
					{		\
						lprintf( "%-34c| ", ' ' );	\
						lprintf( p, d );	\
						lprintf( "\n" );	\
					}

void
i8prn( char *p, ulong data )
{
	usrtrz_printf( (const char *)p, (rkhi8_t)data );
}

void
u8prn( char *p, ulong data )
{
	usrtrz_printf( (const char *)p, (rkhui8_t)data );
}

void
i16prn( char *p, ulong data )
{
	usrtrz_printf( (const char *)p, (rkhi16_t)data );
}

void
u16prn( char *p, ulong data )
{
	usrtrz_printf( (const char *)p, (rkhui16_t)data );
}

void
i32prn( char *p, ulong data )
{
	usrtrz_printf( (const char *)p, (rkhi32_t)data );
}

void
u32prn( char *p, ulong data )
{
	usrtrz_printf( (const char *)p, (rkhui32_t)data );
}

char
usr_object( const rkhui8_t *p, const USR_FMT_T *pfmt )
{
	int n, sh;
	unsigned long obj;

	++p; /* point to oject */
	for( obj = 0, n = RKH_TRC_SIZEOF_POINTER, sh = 0; n; --n, sh += 8  )
		obj |= ( unsigned long )( *p++ << sh );

//	lprintf( pfmt->fmt, map_obj( obj ) );
	usrtrz_printf( pfmt->fmt, map_obj( obj ) );

	/* +1 size field included */
	return RKH_TRC_SIZEOF_POINTER + 1;
}

char
usr_signal( const rkhui8_t *p, const USR_FMT_T *pfmt )
{
	int n, sh;
	TRZE_T e;

	++p; /* point to oject */
	for( e = 0, n = sizeof_trze(), sh = 0; n; --n, sh += 8  )
		e |= (TRZE_T)( *p++ << sh );

	//lprintf( pfmt->fmt, map_sig( e ) );
	usrtrz_printf( pfmt->fmt, map_sig( e ) );

	/* +1 size field included */
	return sizeof_trze() + 1;
}


#define USER_MEMORY_DUMP_START		"%-34c| [ "
#define USER_MEMORY_DUMP_CONTINUE	"%-50c"
#define USER_MEMORY_DUMP_END		"]\n"
#define USER_MEMDUMP_LEN_PER_LINE 	10

char
usr_mdump( const rkhui8_t *p, const USR_FMT_T *pfmt )
{
	rkhui8_t size;
	int i;
	uchar *pd;

	lprintf( USER_MEMORY_DUMP_START, ' ' );
	
	for( i = 0, size = *(p+1), pd = (uchar *)(p + 2); size; --size, ++pd, ++i ) 
	{
		if( i >= USER_MEMDUMP_LEN_PER_LINE )
		{
			i = 0;
			lprintf( USER_MEMORY_DUMP_CONTINUE, ' ' );
		}
		
		lprintf( pfmt->fmt, *pd );
	}

	
	lprintf( USER_MEMORY_DUMP_END );

	/* +1 size field included */
	/* +1 because USR TRACE Format included */
	return *(p+1) + 2;			
}

char
usr_string( const rkhui8_t *p, const USR_FMT_T *pfmt )
{
	//lprintf( pfmt->fmt, p+1 );
	usrtrz_printf( pfmt->fmt, p+1 );

	/* +1 \0 included */
	/* +1 because USR TRACE Format included */
	return strlen((const char *)(p+1)) + 2;	
}

char
usr_integer( const rkhui8_t *p, const USR_FMT_T *pfmt )
{
	ulong d;
	rkhui8_t l;
	uchar n;

	l = (*p++) >> 4;

	for( d = 0, n = 0; n < pfmt->size; ++n )
		d |= ( unsigned long )( *p++ << (8 * n) );
	
	sprintf( fmt, pfmt->fmt, l );

	pfmt->pp( fmt, d );

	/* +1 because USR TRACE Format included */
	return pfmt->size + 1;	
}

char *
usr_fmt( const void *tre )
{
	const rkhui8_t *pt;
	rkhi8_t tr_size;
	rkhui8_t done;
	const USR_TBL_T *pfmt;

	pt = (const rkhui8_t *)tre;
	if( RKH_TRC_EN_NSEQ == 1 ) 
	{
		pt += 2;
		tr_size = trix - 2;
	}
	else
	{
		pt += 1;
		tr_size = trix - 1;
	}

	pt += RKH_TRC_SIZEOF_TSTAMP;
	tr_size -= RKH_TRC_SIZEOF_TSTAMP;
	tr_size -= 1; /* because checksum */
	lprintf( "User trace information\n" );

	do
	{
		if( (*pt & 0x0F) > RKH_SIG_T )
			break;

		pfmt = &usr_tbl[ *pt & 0x0F ];

		done = pfmt->pc( pt, &pfmt->pf );

		tr_size -= done;
		pt += done;
 
		if( tr_size <= 0 )
			break;

///		lprintf( USR_TRC_SEPARATOR );
	}
	while(1);

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
	if( RKH_TRC_EN_CHK == 1 )
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

	if( RKH_TRC_EN_CHK == 1 )
	{
		for( p = tr, i = trix; i--; ++p )
			dprintf( "0x%X|", *p );
	}
	dprintf( " ----\n" );
}


static
void
proc_tcfg_evt( const TRE_T *ftr )
{
	TRAZER_DATA_T tz_data;
	int tz_flg;

	tz_flg = PREPARE_TZOUT_FLGS(	TRAZER_TCFG_EN_TSTAMP_DFT, 
									TRAZER_TCFG_SIZEOF_TSTAMP_DFT,
									TRAZER_TCFG_EN_NSEQ_DFT );

	tz_data.nseq = get_nseq( TRAZER_TCFG_EN_NSEQ_DFT );
	tz_data.ts = get_ts( TRAZER_TCFG_EN_TSTAMP_DFT, TRAZER_SIZEOF_TSTAMP_DFT );
	tz_data.group = ftr->group.c_str();
	tz_data.name = ftr->name.c_str();

	trazer_output(	tz_flg, &tz_data );

	lprintf( "%s\n", (*ftr->fmt_args)( CTE( ftr ) ) );
}



#define GET_TE_GROUP(e)	(rkhui8_t)(((e) >> NGSH) & 7)
#define GET_USR_TE(e)	(rkhui8_t)((e) & 7)

extern TRE_T fmt_usr_tbl;

static
void
parser( void )
{
	static const TRE_T *ftr;			/* received trace event */
	TRAZER_DATA_T tz_data;
	int tz_flg;


	if( (ftr = find_trevt( tr[ 0 ] )) != ( TRE_T* )0 )
	{
		if( ftr->fmt_args == ( HDLR_T )0 )		/* Trace event not valid */
		{
			lprintf( unknown_te, tr[0] );
			return;
		}

		if( tr[ 0 ] == RKH_TE_FWK_TCFG )		/* Trace Config event */
		{
			proc_tcfg_evt( ftr );
			return;
		}

		/* Runtime trace events */
	
		tz_flg = PREPARE_TZOUT_FLGS( RKH_TRC_EN_TSTAMP, 
										RKH_TRC_SIZEOF_TSTAMP,
										RKH_TRC_EN_NSEQ );

		tz_data.nseq = get_nseq( RKH_TRC_EN_NSEQ );
		
		if( !verify_nseq( tz_data.nseq ) )
			lprintf( lost_trace_info );

		curr_tstamp = tz_data.ts = get_ts( RKH_TRC_EN_TSTAMP, 
											RKH_TRC_SIZEOF_TSTAMP );
		tz_data.group = ftr->group.c_str();
		tz_data.name = ftr->name.c_str();

		trazer_output(	tz_flg, &tz_data );

		lprintf( "%s\n", (*ftr->fmt_args)( CTE( ftr ) ) );

		return;
	}

	else if( GET_TE_GROUP( tr[0] ) == RKH_TG_USR )
	{
		ftr = &fmt_usr_tbl;

		tz_flg = PREPARE_TZOUT_FLGS( RKH_TRC_EN_TSTAMP, 
										RKH_TRC_SIZEOF_TSTAMP,
										RKH_TRC_EN_NSEQ );

		tz_data.nseq = get_nseq( RKH_TRC_EN_NSEQ );
		
		if( !verify_nseq( tz_data.nseq ) )
			lprintf( lost_trace_info );

		curr_tstamp = tz_data.ts = get_ts( RKH_TRC_EN_TSTAMP, 
											RKH_TRC_SIZEOF_TSTAMP );
		tz_data.group = ftr->group.c_str();

		if( (tz_data.name = map_uevt( GET_USR_TE(tr[0]) )) == NULL )
		{
			sprintf( fmt, "%s%d", ftr->name.c_str(), GET_USR_TE( tr[0] ) );
			tz_data.name = fmt;
		}

		trazer_output(	tz_flg, &tz_data );
		
		(*ftr->fmt_args)( (const void *)(tr) ); 

		return;
	}
	
	lprintf( unknown_te, tr[0] );
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
						lprintf( stream_chkerr );
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

void
trazer_init( void )
{
	lastnseq = 255;
	lprintf( VERSION_STRING_TXT );
	lprintf( "\nUsing local RKH configuration\n\n" );
	lprintf( "   RKH_SMA_EN_TRC_SENDER  = %d\n", RKH_SMA_EN_TRC_SENDER );
	lprintf( "   RKH_TRC_RUNTIME_FILTER = %d\n", RKH_TRC_RUNTIME_FILTER );
	lprintf( "   RKH_TRC_EN_USER_TRACE  = %d\n", RKH_TRC_EN_USER_TRACE );
	lprintf( "   RKH_TRC_ALL            = %d\n", RKH_TRC_ALL );
	lprintf( "   RKH_TRC_EN_MP          = %d\n", RKH_TRC_EN_MP );
	lprintf( "   RKH_TRC_EN_RQ          = %d\n", RKH_TRC_EN_RQ );
	lprintf( "   RKH_TRC_EN_SMA         = %d\n", RKH_TRC_EN_SMA );
	lprintf( "   RKH_TRC_EN_TIM         = %d\n", RKH_TRC_EN_TIM );
	lprintf( "   RKH_TRC_EN_SM          = %d\n", RKH_TRC_EN_SM );
	lprintf( "   RKH_TRC_EN_FWK         = %d\n", RKH_TRC_EN_FWK );
	lprintf( "   RKH_TRC_EN_ASSERT      = %d\n", RKH_TRC_EN_ASSERT );
	lprintf( "   RKH_RQ_EN_GET_LWMARK   = %d\n", RKH_RQ_EN_GET_LWMARK );
	lprintf( "   RKH_MP_EN_GET_LWM      = %d\n", RKH_MP_EN_GET_LWM );
	lprintf( "   RKH_TRC_RTFIL_SMA_EN   = %d\n", RKH_TRC_RTFIL_SMA_EN );
	lprintf( "   RKH_TRC_RTFIL_SIGNAL_EN= %d\n", RKH_TRC_RTFIL_SIGNAL_EN );
	lprintf( "   RKH_TRC_EN_NSEQ        = %d\n", RKH_TRC_EN_NSEQ );
	lprintf( "   RKH_TRC_EN_TSTAMP      = %d\n", RKH_TRC_EN_TSTAMP );
	lprintf( "   RKH_TRC_EN_CHK         = %d\n", RKH_TRC_EN_CHK );
	lprintf( "   RKH_SIZEOF_EVENT       = %d\n", RKH_SIZEOF_EVENT*8 );
	lprintf( "   RKH_TRC_SIZEOF_TSTAMP  = %d\n", RKH_TRC_SIZEOF_TSTAMP*8 );
	lprintf( "   RKH_TRC_SIZEOF_POINTER = %d\n", RKH_TRC_SIZEOF_POINTER*8 );
	lprintf( "   RKH_TIM_SIZEOF_NTIMER  = %d\n", RKH_TIM_SIZEOF_NTIMER*8 );
	lprintf( "   RKH_MP_SIZEOF_NBLOCK   = %d\n", RKH_MP_SIZEOF_NBLOCK*8 );
	lprintf( "   RKH_RQ_SIZEOF_NELEM    = %d\n", RKH_RQ_SIZEOF_NELEM*8 );
	lprintf( "   RKH_SIZEOF_ESIZE       = %d\n", RKH_SIZEOF_ESIZE*8 );
	lprintf( "   RKH_MP_SIZEOF_BSIZE    = %d\n", RKH_MP_SIZEOF_BSIZE*8 );
	lprintf( "   RKH_MAX_EPOOL          = %d\n", RKH_MAX_EPOOL );
	lprintf( "\n" );
}

