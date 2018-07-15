#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "version.h"
#include "rerror.h"
#include "getopt.h"
#include "lang.h"
#include "serial.h"
#include "mydefs.h"
#include "timer.h"
#include "options.h"
#include "config.h"
#include "messages.h"
#include "error.h"
#include "tzpdef.h"
#include "tzparse.h"
#include "unittrz.h"
#include "tzlog.h"
#include "utils.h"
#include "seqdiag.h"
#include "tcp.h"
#include "cfgdep.h"

FILE *f;

enum
{
	FILE_STREAM,
	SERIAL_STREAM,
	TCP_STREAM,

	NONE_STREAM
};

static int instr = NONE_STREAM;

static
void
trazer_init( void )
{
	lprintf( VERSION_STRING_TXT );
	lprintf( "\nUsing local RKH configuration\n\n" );
	lprintf( "   RKH_CFGPORT_TRC_SIZEOF_TSTAMP = %d\n", TRZ_RKH_CFGPORT_TRC_SIZEOF_TSTAMP*8 );
	lprintf( "   RKH_CFGPORT_TRC_SIZEOF_PTR    = %d\n", TRZ_RKH_CFGPORT_TRC_SIZEOF_PTR*8 );
	lprintf( "   RKH_CFG_FWK_SIZEOF_EVT        = %d\n", TRZ_RKH_CFG_FWK_SIZEOF_EVT*8 );
	lprintf( "   RKH_CFG_FWK_SIZEOF_EVT_SIZE   = %d\n", TRZ_RKH_CFG_FWK_SIZEOF_EVT_SIZE*8 );
	lprintf( "   RKH_CFG_FWK_MAX_EVT_POOL      = %d\n", TRZ_RKH_CFG_FWK_MAX_EVT_POOL );
	lprintf( "   RKH_CFG_RQ_GET_LWMARK_EN      = %d\n", TRZ_RKH_CFG_RQ_GET_LWMARK_EN );
	lprintf( "   RKH_CFG_RQ_SIZEOF_NELEM       = %d\n", TRZ_RKH_CFG_RQ_SIZEOF_NELEM*8 );
	lprintf( "   RKH_CFG_MP_GET_LWM_EN         = %d\n", TRZ_RKH_CFG_MP_GET_LWM_EN );
	lprintf( "   RKH_CFG_MP_SIZEOF_NBLOCK      = %d\n", TRZ_RKH_CFG_MP_SIZEOF_NBLOCK*8 );
	lprintf( "   RKH_CFG_MP_SIZEOF_BSIZE       = %d\n", TRZ_RKH_CFG_MP_SIZEOF_BSIZE*8 );
	lprintf( "   RKH_CFG_SMA_TRC_SNDR_EN       = %d\n", TRZ_RKH_CFG_SMA_TRC_SNDR_EN );
	lprintf( "   RKH_CFG_TMR_SIZEOF_NTIMER     = %d\n", TRZ_RKH_CFG_TMR_SIZEOF_NTIMER*8 );
	lprintf( "   RKH_CFG_TRC_RTFIL_EN          = %d\n", TRZ_RKH_CFG_TRC_RTFIL_EN );
	lprintf( "   RKH_CFG_TRC_USER_TRACE_EN     = %d\n", TRZ_RKH_CFG_TRC_USER_TRACE_EN );
	lprintf( "   RKH_CFG_TRC_ALL_EN            = %d\n", TRZ_RKH_CFG_TRC_ALL_EN );
	lprintf( "   RKH_CFG_TRC_MP_EN             = %d\n", TRZ_RKH_CFG_TRC_MP_EN );
	lprintf( "   RKH_CFG_TRC_RQ_EN             = %d\n", TRZ_RKH_CFG_TRC_RQ_EN );
	lprintf( "   RKH_CFG_TRC_SMA_EN            = %d\n", TRZ_RKH_CFG_TRC_SMA_EN );
	lprintf( "   RKH_CFG_TRC_TMR_EN            = %d\n", TRZ_RKH_CFG_TRC_TMR_EN );
	lprintf( "   RKH_CFG_TRC_SM_EN             = %d\n", TRZ_RKH_CFG_TRC_SM_EN );
	lprintf( "   RKH_CFG_TRC_FWK_EN            = %d\n", TRZ_RKH_CFG_TRC_FWK_EN );
	lprintf( "   RKH_CFG_TRC_ASSERT_EN         = %d\n", TRZ_RKH_CFG_TRC_ASSERT_EN );
	lprintf( "   RKH_CFG_TRC_RTFIL_SMA_EN      = %d\n", TRZ_RKH_CFG_TRC_RTFIL_SMA_EN );
	lprintf( "   RKH_CFG_TRC_RTFIL_SIGNAL_EN   = %d\n", TRZ_RKH_CFG_TRC_RTFIL_SIGNAL_EN );
	lprintf( "   RKH_CFG_TRC_NSEQ_EN           = %d\n", TRZ_RKH_CFG_TRC_NSEQ_EN );
	lprintf( "   RKH_CFG_TRC_TSTAMP_EN         = %d\n", TRZ_RKH_CFG_TRC_TSTAMP_EN );
	lprintf( "   RKH_CFG_TRC_CHK_EN            = %d\n", TRZ_RKH_CFG_TRC_CHK_EN );
	lprintf( "\n" );

}

static
void
close_all( void )
{
	if( instr == FILE_STREAM )
	{
		fclose( f );
	}
	else if( instr == SERIAL_STREAM )
		close_mserial();
	else if( instr == TCP_STREAM )
		tcpClose();
}

void
sig_handler( int sig )
{
	exit( EXIT_SUCCESS );
}

FILE *fdbg = NULL;

void
start_stream_dbg( char *fname )
{
	if( ( fdbg = fopen( fname, "w+b" ) ) == NULL )
	{
		fatal_error( "Can't open file %s\n", fname );
	}
}

void
serialhook(unsigned char c)
{
	fwrite ( &c, 1, 1, fdbg );
	fflush(fdbg);
    tzparser_exec(c);
} 

int
main(int argc, char **argv)
{
	char c;
	int n;
	size_t r;
	int tcpPort;

	start_stream_dbg("instream.bin");

	atexit( close_all );
	signal( SIGINT, sig_handler );
	signal( SIGABRT, sig_handler );
	signal( SIGTERM, sig_handler );
//	signal( SIGBREAK, sig_handler );

	init_options( argc, argv );

	read_config_file( (char *)CONFIG_FILE );

	start_log( options.outfile.c_str() );

	trazer_init();

    tzparser_init();

    unitrazer_init();

	seqdiag_init();

	if( !options.symbols_file.empty() )
	{
		lprintf( "\n-------- Parsing symbols from file %s --------\n\n", 
				options.symbols_file.c_str() );

		if( ( f = fopen( options.symbols_file.c_str(), "rb" ) ) == NULL )
			fatal_error( no_trace_file, options.symbols_file.c_str() );

		while( ( r = fread( &c, sizeof(c), sizeof(c), f ) ) == sizeof( c ) )
		{
		    fwrite ( &c, 1, 1, fdbg );
			fflush(fdbg);
			tzparser_exec( c );
		}

		fclose( f );
	}

	if( !options.instream_tcpsock.empty() )
	{
		tcpPort = atoi(options.instream_tcpsock.c_str());

		if (!tcpOpen(tcpPort))
			return EXIT_FAILURE;
		else
			lprintf( "\n-------- Parsing trace stream from TCP Port %u --------\n\n", tcpPort ); 

		instr = TCP_STREAM;

		while ((n = tcpRead((unsigned char *)&c, sizeof(c))) != -1)
		{
			if (n > 0)
			{
				fwrite ( &c, 1, 1, fdbg );
				fflush(fdbg);
				tzparser_exec(c);
			}
		}
	}

	else if( !options.instream_file.empty() )
	{
		lprintf( "\n-------- Parsing trace stream from file %s --------\n\n", 
				options.instream_file.c_str() );

		if( ( f = fopen( options.instream_file.c_str(), "rb" ) ) == NULL )
			fatal_error( no_trace_file, options.instream_file.c_str() );

		instr = FILE_STREAM;

		while( ( r = fread( &c, sizeof(c), sizeof(c), f ) ) == sizeof( c ) )
		{
		    fwrite ( &c, 1, 1, fdbg );
			fflush(fdbg);
			tzparser_exec( c );
		}
	}

	else if( !options.instream_comport.empty() )
	{
		lprintf( "\n-------- Parsing trace stream from %s at %s %s --------\n\n", 
				options.instream_comport.c_str(), 
				options.baudrate.c_str(), 
				options.parity.c_str() );

		init_serial( &serialhook );

		instr = SERIAL_STREAM;

		while( test_key() != ESC );
	}
	return EXIT_SUCCESS;
}


