/*
 *	file: trazer.c
 *	Last updated for version: 2.0
 *	Date of the last update:  Apr 03, 2012
 */


#include "mytypes.h"
#include "rkhtrc.h"
#include "mdebug.h"
#include "myevt.h"
#include "tzparse.h"
#include "evtbl.h"
#include "symbtbl.h"
#include "sigtbl.h"
#include "uevtbl.h"
#include "cfgdep.h"
#include "tzlog.h"
#include "seqdiag.h"
#include <stdio.h>

typedef unsigned long TRZTS_T;
typedef unsigned long TRZNB_T;
typedef unsigned long TRZNE_T;
typedef unsigned long TRZNT_T;
typedef unsigned long TRZES_T;

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

#define get_ts()		( TRAZER_EN_TSTAMP == 1 ) ? ( TRZTS_T )assemble( TRAZER_SIZEOF_TSTAMP ) : 0

#define CTE( te )		((const struct tre_t*)(te))

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

#define PARSER_MAX_SIZE_BUF			64


enum
{
	PARSER_WFLAG, PARSER_COLLECT, PARSER_ESCAPING
};


static rkhui8_t *trb;				/* points to trace event buffer */
static char fmt[ 128 ];
extern FILE *fdbg;

static rkhui8_t state = PARSER_WFLAG;
static rkhui8_t tr[ PARSER_MAX_SIZE_BUF ], *ptr, trix;
static char symstr[ 16 ];

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
h_aosymevt( const void *tre )
{
	TRN_ST trn;

	trn.tobj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	trn.e = (TRZE_T)assemble( sizeof_trze() );
	sprintf( fmt, CTE( tre )->fmt, map_obj( trn.tobj ), map_sig( trn.e ) );

	if( configs.trazer_en_sender == 1 )
	{
		trn.sobj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
		sprintf( fmt+strlen(fmt), ", sndr=%s", map_obj( trn.sobj ) );
		add_to_trntbl( &trn );
	}

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
	
	if( *s == '&' )
		++s;

	sprintf( fmt, CTE( tre )->fmt, obj, s );
	add_to_symtbl( obj, s );
	return fmt;
}

char *
h_symst( const void *tre )
{
	unsigned long obj;
	char *s;
	const char *ao;

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );

	ao = map_obj( obj );
/*
	if( configs.trazer_en_sender == 1 )
	{
		ao = map_obj( obj );
	}
	else
		ao = "null";*/

	obj = (unsigned long)assemble( TRAZER_SIZEOF_POINTER );
	s = assemble_str();
	
	if( *s == '&' )
		++s;
	if( *ao == '&' )
		++ao;

	sprintf( fmt, CTE( tre )->fmt, ao, obj, s );
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


char *
h_symuevt( const void *tre )
{
	uchar ue;
	char *s;

	ue = (uchar)assemble( sizeof_trze() );
	s = assemble_str();
	sprintf( fmt, CTE( tre )->fmt, ue, s );
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

	sprintf( fmt, CTE( tre )->fmt, pfname, line );

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
	trb = proc_version_code( trb );

	TRAZER_EN_SENDER = (*trb >> 0) & 0x01;
	TRAZER_RUNTIME_FILTER = (*trb >> 1) & 0x01;
	TRAZER_EN_USER_TRACE = (*trb >> 2) & 0x01;
	TRAZER_ALL = (*trb >> 3) & 0x01;
	TRAZER_EN_MP = (*trb >> 4) & 0x01;
	TRAZER_EN_RQ = (*trb >> 5) & 0x01;
	TRAZER_EN_SMA = (*trb >> 6) & 0x01;
	TRAZER_EN_TIM = (*trb++ >> 7) & 0x01;

	TRAZER_EN_SM = (*trb >> 0) & 0x01;
	TRAZER_EN_FWK = (*trb >> 1) & 0x01;
	TRAZER_EN_ASSERT = (*trb++ >> 2) & 0x01;

	TRAZER_SIZEOF_SIG = (*trb >> 4) & 0x0F;
	TRAZER_SIZEOF_TSTAMP = (*trb++) & 0x0F;
	TRAZER_SIZEOF_POINTER = (*trb >> 4) & 0x0F;
	TRAZER_SIZEOF_NTIMER = (*trb++) & 0x0F;
	TRAZER_SIZEOF_NBLOCK = (*trb >> 4) & 0x0F;
	TRAZER_SIZEOF_NELEM = (*trb++) & 0x0F;
	TRAZER_SIZEOF_ESIZE = (*trb >> 4) & 0x0F;
	TRAZER_EN_NSEQ = (*trb++) & 0x0F;
	TRAZER_EN_CHK = (*trb >> 4) & 0x0F;
	TRAZER_EN_TSTAMP = (*trb++) & 0x0F;

	lprintf( "Trace Setup received from client\n" );

	rkhver_printf( RKH_VERSION );
	cfg_printf( TRAZER_EN_SENDER );
	cfg_printf( TRAZER_RUNTIME_FILTER );
	cfg_printf( TRAZER_EN_USER_TRACE );
	cfg_printf( TRAZER_ALL );
	cfg_printf( TRAZER_EN_MP );
	cfg_printf( TRAZER_EN_RQ );
	cfg_printf( TRAZER_EN_SMA );
	cfg_printf( TRAZER_EN_TIM );
	cfg_printf( TRAZER_EN_SM );
	cfg_printf( TRAZER_EN_FWK );
	cfg_printf( TRAZER_EN_ASSERT );
	cfg_printf( TRAZER_SIZEOF_SIG );
	cfg_printf( TRAZER_SIZEOF_TSTAMP );
	cfg_printf( TRAZER_SIZEOF_POINTER );
	cfg_printf( TRAZER_SIZEOF_NTIMER );
	cfg_printf( TRAZER_SIZEOF_NBLOCK );
	cfg_printf( TRAZER_SIZEOF_NELEM );
	cfg_printf( TRAZER_SIZEOF_ESIZE );
	cfg_printf( TRAZER_EN_NSEQ );
	cfg_printf( TRAZER_EN_CHK );
	cfg_printf( TRAZER_EN_TSTAMP );

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
						lprintf( "%-39c| ", ' ' );	\
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
	for( obj = 0, n = TRAZER_SIZEOF_POINTER, sh = 0; n; --n, sh += 8  )
		obj |= ( unsigned long )( *p++ << sh );

//	lprintf( pfmt->fmt, map_obj( obj ) );
	usrtrz_printf( pfmt->fmt, map_obj( obj ) );

	/* +1 size field included */
	return TRAZER_SIZEOF_POINTER + 1;
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
#define USER_MEMORY_DUMP_START		"%-39c| [ "
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
	if( TRAZER_EN_NSEQ == 1 ) 
	{
		pt += 2;
		tr_size = trix - 2;
	}
	else
	{
		pt += 1;
		tr_size = trix - 1;
	}

	pt += TRAZER_SIZEOF_TSTAMP;
	tr_size -= TRAZER_SIZEOF_TSTAMP;
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

#define GET_TE_GROUP(e)	(rkhui8_t)(((e) >> NGSH) & 7)
#define GET_USR_TE(e)	(rkhui8_t)((e) & 7)

extern TRE_T fmt_usr_tbl;

static
void
parser( void )
{
	const TRE_T *ftr;			/* received trace event */

	TRZTS_T ts;
	rkhui8_t nseq;
	const char *pname;

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

	else if( GET_TE_GROUP( tr[0] ) == RKH_TG_USR )
	{
		ftr = &fmt_usr_tbl;

		get_nseq();

		if( !verify_nseq( nseq ) )
			lprintf( "\n***** May be have lost trace info, sequence are not correlatives\n" );

		set_to_ts();		/* from timestamp field */
		ts = get_ts();

		if( (pname = map_uevt( GET_USR_TE(tr[0]) )) == NULL )
		{
			sprintf( fmt, "%s%d", ftr->name.c_str(), GET_USR_TE( tr[0] ) );
			pname = fmt;
		}

		lprintf( trheader, ts, nseq, ftr->group.c_str(), pname );
		(*ftr->fmt_args)( (const void *)(tr) ); 

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

	lprintf( "   TRAZER_EN_SENDER      = %d\n", TRAZER_EN_SENDER );
	lprintf( "   TRAZER_RUNTIME_FILTER = %d\n", TRAZER_RUNTIME_FILTER );
	lprintf( "   TRAZER_EN_USER_TRACE  = %d\n", TRAZER_EN_USER_TRACE );
	lprintf( "   TRAZER_ALL            = %d\n", TRAZER_ALL );
	lprintf( "   TRAZER_EN_MP          = %d\n", TRAZER_EN_MP );
	lprintf( "   TRAZER_EN_RQ          = %d\n", TRAZER_EN_RQ );
	lprintf( "   TRAZER_EN_SMA         = %d\n", TRAZER_EN_SMA );
	lprintf( "   TRAZER_EN_TIM         = %d\n", TRAZER_EN_TIM );
	lprintf( "   TRAZER_EN_SM          = %d\n", TRAZER_EN_SM );
	lprintf( "   TRAZER_EN_FWK         = %d\n", TRAZER_EN_FWK );
	lprintf( "   TRAZER_EN_ASSERT      = %d\n", TRAZER_EN_ASSERT );
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
	//lprintf( "\n---- BEGIN TRACE SESSION ----\n\n" );
	lprintf( "\n" );
}
