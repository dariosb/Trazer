#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "rerror.h"
#include "getopt.h"
#include "lang.h"
#include "serial.h"
#include "mydefs.h"
#include "timer.h"
#include "tplink.h"
#include "tplact.h"
#include "options.h"
#include "messages.h"
#include "error.h"
#include "tzparse.h"


static char *args = "d:D:hH";

static
void
show_help( char *name )
{
	fatal( "Usage: %s [-d {0|1} -h]", name );
}

MUInt rcv;

static TPLFRM_T tplink_buff;
static TPLFRM_T tst_frame =
{
	0,
	6, 
	{ 0x40, 0x30, 0xF1, 0x10, 0x20, 0x50 }
};


typedef struct
{
	ushort x;
	ushort y;
	ushort z;
	uchar m;
}ACCEL_T;

typedef struct
{
	ushort hoard;	// grain hoard.
	ushort pqty;     // pails quantity.
	ushort flow;	// grain volume by second.
} GRSENS_T;

typedef struct
{
	uchar cmd;
	uchar m;
	GRSENS_T h;
	ACCEL_T a;
	uchar hum;
}CBOX_ST;


#define grs_read(x)				x.hoard=0x3456;x.pqty=0x78;x.flow=0x9ab
#define get_accel_data(v)		(v).x=0xCDE;(v).y=0xF01;(v).z=0x234
#define get_movdet()			1
#define get_humedity_sensor()	2

static CBOX_ST cb;
static uchar xmit_buff[sizeof(CBOX_ST)];

#if ( __TPLINK_SYSTEM_ENDIANNES__ == __TPL_LITTLE_ENDIANNES__ )

int endian_val=1;
char *endiannes= (char *)&endian_val;
#define is_little_endian()	(*endiannes)

void
h2ns( ushort *x )
{
	ushort ret=0;

	if( is_little_endian() )
	{
		ret = (*x & 0xFF) << 8;
		ret |= ((*x >> 8)& 0xFF);
		*x = ret;
	}
}

#define n2hs(x)	h2ns(x)

#else

#define n2hs(x)	
#define h2ns(x)

#endif

void
tplink_onrcv( TPLFRM_T *p )
{
	uchar *pl;
	ushort len;
	ushort cmd;

	memcpy(tplink_buff.pload, p->pload, p->qty);
	tplink_buff.qty = p->qty;

	pl = tplink_buff.pload;
	len = tplink_buff.qty;

	printf("Valid Frame RCV :");
	do
		printf("  %X", *pl++ );
	while( --len );

	printf("\n-------------\n");
	cmd = *(uchar *)(tplink_buff.pload);
	
	if( cmd != 0x01 )
	{
		printf("Unknown Command\n");
		return;
	}

	cb.cmd = 0x12;

	grs_read( cb.h );
	get_accel_data( cb.a );
	cb.m = get_movdet();
	cb.hum = get_humedity_sensor();

	h2ns( &cb.h.hoard );
	h2ns( &cb.h.pqty );
	h2ns( &cb.h.flow );

	h2ns( &cb.a.x );
	h2ns( &cb.a.y );
	h2ns( &cb.a.z );

	tplink_send_frame( (uchar *)&cb, sizeof(cb) );
}

void
tplink_ontout( void )
{
	printf(("RCV Tout !!!\n"));
	printf("-------------\n");
}

void
tplink_onchkerr( void )
{
	printf(("RCV Check Error !!!\n"));
	printf("-------------\n");
}

#if __USE_TPLINK_ONXMIT_CMP_CBF__

void
tplink_onxmit_cmp( void )
{
	printf(("XMIT Complete\n"));
	printf("-------------------\n");
}

#endif

FILE *f;
FILE *fdbg;

int
main(int argc, char **argv)
{
	char c;

	init_options( argc, argv );

	if( !options.instream_file.empty() )
	{
		if( ( f = fopen( options.instream_file.c_str(), "rt" ) ) == NULL )
			fatal_error( no_trace_file, options.instream_file.c_str() );
	}


	if( ( fdbg = fopen( "tzlog.txt", "w+" ) ) == NULL )
	{
		perror( "Can't open file tzlog.txt\n" );
		exit( EXIT_FAILURE );
	}

	trazer_init();

//	tplink_init();
	while( (c = fgetc(f)) != EOF )
		trazer_parse( c );

	fflush( fdbg );
	fclose( fdbg );

	for(;;);
	return 0;
}


