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
#include <conio.h>

#include "options.h"
#include "error.h"
#include "getopt.h"
#include "debug.h"
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
	ID_OPT,
	NAME_OPT,
	ARGS_OPT,
	COMMENT_OPT,

	MAX_OPTS
};

/*
 * 	Available commands for optdef.txt
 */

static const char *commands[ MAX_OPTS ] =
{
	"Group",
	"Event",
	"ID",
	"Name",
	"Args",
	"Comment"
};

/*
 * 	Command line letter options
 */

static char *opts = CONSOLE_OPTIONS_DEFINITION; 

/*
 * 	Initialized static 'not constant' tables
 */

/*
 * 	Option structure
 */

OPTIONS_T options =
{
	{ DISABLE_OPT }
};

/*
 * 	Static uninitialized variables
 */

static char buffer[ 100 ];			/*	buffer: used for field separation	*/	

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
 * 		validate_command:
 * 			Verifies that the command name is 
 * 			correct; returns index or
 * 			negative number in case of error
 */

static
int
validate_command( const char *pcomm )
{
	static const char **p;

	for( p = commands; p < commands + MAX_OPTS ; ++p )
		if( strcmp( *p, pcomm ) == 0 )
			return p - commands;
	return -1;
}

static
int
options_enable( unsigned *opt, char *ptail )
{
	char *p;

	if( ( ptail = strtok( NULL, sep ) ) == NULL )
		return -1;

	*opt = (strtol( ptail, &p, 0 ) != 0) ? ENABLE_OPT : DISABLE_OPT;
	return 0;
}

static
int
str_options_cpy( string *dest, char *ptail )
{
	if( ( ptail = strtok( NULL, sep ) ) == NULL )
		return -1;

	while( *ptail == ' ' )
		++ptail;

	if( !dest->empty() )
		return -1;

	dest->assign( ptail );
	return 0;
}

static
int
num_options_cpy( char *ptail )
{
	if( ( ptail = strtok( NULL, sep ) ) == NULL )
		return -1;

	return atoi(ptail);
}
/*
 * 		process_opt:
 * 			Process an option from the option file
 * 			If bad option, returns negative, else returns 0
 */
static
int
process_event( FILE *f )
{
	char *p;
	EVENT_INFO_ST evt;
	int inscan = 0;
	int line;

	evt.id = -1;
	for( line = 1; fgets( buffer, sizeof( buffer ), f ) != NULL; ++line )
	{
		if( (strlen( buffer ) == 0) || *buffer == COMMENT 
									|| ( p = strtok( buffer, sep ) ) == NULL )
			continue;

		if( !inscan &&  ( validate_command( p ) == EVENT_OPT ) )
		{
			if( str_options_cpy( &evt.event, p ) < 0 )
				return -line;

			inscan = 1;
			continue;
		}

		switch( validate_command( p ) )
		{
			case EVENT_OPT:
				if( add_to_evtbl( &evt ) < 0 )
					return -line;
				if( str_options_cpy( &evt.event, p ) < 0 )
					return -line;
				break;
			case ID_OPT:
				if( (evt.id = num_options_cpy( p )) < 0 )
					return -line;
				break;
			case GROUP_OPT:
				if( str_options_cpy( &evt.group, p ) < 0 )
					return -line;
				break;
			case NAME_OPT:
				if( str_options_cpy( &evt.name, p ) < 0 )
					return -line;
				break;
			case ARGS_OPT:
				str_options_cpy( &evt.args, p ); 
				break;
			case COMMENT_OPT:
				str_options_cpy( &evt.comment, p ); 
				break;
			default:
				return -line;
		}
	}
	return 0;
}

/*
 * 		read_option_file:
 * 			Opens and read options from the options
 * 			file
 */
void
read_option_file( char *option_file )
{
	FILE *f;
	int line;

	if( ( f = fopen( option_file, "rt" ) ) == NULL )
		printf( no_options_file, option_file );
	else
	{
		dprintf( getting_options_from, option_file );
		if( ( line = process_event( f ) ) < 0 )
		{
			fclose( f );
			fatal_error( error_in_options_file, option_file, -line );
		}
	}
}


/*
 * 	show_help:
 */

void
show_help( void )
{
	printf( help_message );
}

/*
 * 	evaluate_args
 */
void
evaluate_args( int argc, char **argv )
{
	int c;
//	char *p;

	if( argc < 2 )
		usage( argv[0] );

	while( ( c = getopt( argc, argv, opts ) ) != EOF )
		switch( c )
		{
			case 'd':
				options.enable_debug = ENABLE_OPT;
				break;
			case 't':
			//	options.tree_info = ENABLE_OPT;
				break;
			case 's':
			//	options.hsm_summary = ENABLE_OPT;
				break;
			case 'f':
			//	strncpy( options.source_file, optarg, sizeof( options.source_file ) );
				break;
			case 'o':
			//	strncpy( options.target_dir, optarg, sizeof( options.target_dir ) );
				break;
			case 'c':
			//	strncpy( options.rkh_cfg_file, optarg, sizeof( options.rkh_cfg_file ) );
				break;
			case 'v':
				show_version();
				break;				
			case 'h':
				show_help();
				exit( EXIT_SUCCESS );
			case '?':
				usage( argv[0] );
				break;
		}
}

void
init_options( int argc, char **argv )
{
	evaluate_args( argc, argv );
	read_option_file( OPTIONS_FILE );
	//set_rkhcfg_opts();
}
