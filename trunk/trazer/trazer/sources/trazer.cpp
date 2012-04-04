#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <stdarg.h>
#include <conio.h>

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
#include "tzparse.h"
#include "tzlog.h"
#include "utils.h"

FILE *f;

enum
{
	FILE_STREAM,
	SERIAL_STREAM,

	NONE_STREAM
};

static int instr = NONE_STREAM;

static
void
close_all( void )
{
	if( instr == FILE_STREAM )
		_fcloseall();
	else if( instr == SERIAL_STREAM )
		close_mserial();
}

void
sig_handler( int sig )
{
	exit( EXIT_SUCCESS );
}

int
main(int argc, char **argv)
{
	char c;
	size_t r;

	atexit( close_all );
	signal( SIGINT, sig_handler );
	signal( SIGABRT, sig_handler );
	signal( SIGTERM, sig_handler );
	signal( SIGBREAK, sig_handler );

	init_options( argc, argv );

	read_config_file( CONFIG_FILE );

	start_log( "tzlog.txt" );

	trazer_init();

	if( !options.instream_file.empty() )
	{
		lprintf( "\n-------- Parsing trace stream from file %s --------\n\n", 
				options.instream_file.c_str() );

		if( ( f = fopen( options.instream_file.c_str(), "rb" ) ) == NULL )
			fatal_error( no_trace_file, options.instream_file.c_str() );

		instr = FILE_STREAM;

		while( ( r = fread( &c, sizeof(c), sizeof(c), f ) ) == sizeof( c ) )
		{
			trazer_parse( c );
		}
	}

	else if( !options.instream_comport.empty() )
	{
		lprintf( "\n-------- Parsing trace stream from %s at %s %s --------\n\n", 
				options.instream_comport.c_str(), 
				options.baudrate.c_str(), 
				options.parity.c_str() );

		init_serial( &trazer_parse );

		instr = SERIAL_STREAM;
	}

	while( test_key() != ESC );

	return EXIT_SUCCESS;
}


