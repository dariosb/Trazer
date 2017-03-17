/*
 * 	options.c
 * 		Management of test options
 * 		This is dependent on test
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fstream>
#include <iostream>

#include "messages.h"
#include "options.h"
#include "error.h"
#include "getopt.h"
#include "mdebug.h"
#include "utils.h"
#include "version.h"
#include "evtbl.h"

using namespace std;

/*
 * 	Macros and definitions
 */

#define COMMENT	'#'

#define sep		":\n"

enum
{
	GROUP_OPT,
	EVENT_OPT,
	NAME_OPT,
	ARGS_OPT,
	COMMENT_OPT,
	ENDOFTBL,

	MAX_OPTS
};


/*
 * 	Command line letter options
 */

static char *opts = (char *)CONSOLE_OPTIONS_DEFINITION; 

/*
 * 	Initialized static 'not constant' tables
 */

/*
 * 	Option structure
 */
#ifdef __TRAZER__
OPTIONS_T options;
#else
OPTIONS_T options =
{
	0,
	0,
	"",
	"",
	"",
	"",
	"",
	""
};
#endif

/*
 * 	Static uninitialized variables
 */

static
void
show_version( void )
{
	printf(VERSION_STRING_TXT);
}

/*
 * 	usage:
 */

void
usage( char *name )
{
	fatal_error( usage_msg, name );
}


/*
 * 	show_help:
 */

void
show_help( void )
{
	printf( "%s", help_message );
}

#ifdef __TRAZER__
/*
 * 	evaluate_args
 */
void
evaluate_args( int argc, char **argv )
{
	int c;

	if( argc < 2 )
		usage( argv[0] );

	while( ( c = getopt( argc, argv, opts ) ) != EOF )
		switch( c )
		{
			case 'f':
				options.instream_file.assign(optarg);
				break;
			case 'c':
				options.instream_comport.assign(optarg);
				options.baudrate.assign(get_next(argv));
				options.parity.assign(get_next(argv));
				break;
			case 't':
				options.instream_tcpsock.assign(optarg);
				break;
			case 'o':
				options.outfile.assign( optarg );
				break;
			case 'q':
				options.quiet = 1;
				break;
			case 'v':
				show_version();
				while( !test_key() );
				exit( EXIT_SUCCESS );
				break;				
			case 'd':
				options.enable_debug = 1;
				break;
			case 'h':
				show_help();
				while( !test_key() );
				exit( EXIT_SUCCESS );
				break;
			case '?':
				usage( argv[0] );
				break;
		}
}

void
init_options( int argc, char **argv )
{
	evaluate_args( argc, argv );
//	read_option_file( (char *)OPTIONS_FILE );
}
#endif
