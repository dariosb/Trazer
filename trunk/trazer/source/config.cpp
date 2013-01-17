/*
 * 	config.c
 * 		Management of config options
 * 		This is dependent on application definitions of size types
 */
#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <conio.h>
#endif

#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "config.h"
#include "error.h"
#include "mdebug.h"
#include "utils.h"

using namespace std;

/*
 * 	Macros and definitions
 */

#define COMMENT	'#'

#define sep		"=\n"

enum
{
	TRAZER_SIZEOF_SIG_CFG,
	TRAZER_SIZEOF_TSTAMP_CFG,
	TRAZER_SIZEOF_POINTER_CFG,
	TRAZER_SIZEOF_NTIMER_CFG,
	TRAZER_SIZEOF_NBLOCK_CFG,
	TRAZER_SIZEOF_NELEM_CFG,
	TRAZER_SIZEOF_ESIZE_CFG,
	TRAZER_EN_NSEQ_CFG,
	TRAZER_EN_CHK_CFG,
	TRAZER_EN_TSTAMP_CFG,

	MAX_CONFIGS
};

/*
 * 	Available commands for trazer.cfg
 */

static const char *config[ MAX_CONFIGS ] =
{
	"TRAZER_SIZEOF_SIG",
	"TRAZER_SIZEOF_TSTAMP",
	"TRAZER_SIZEOF_POINTER",
	"TRAZER_SIZEOF_NTIMER",
	"TRAZER_SIZEOF_NBLOCK",
	"TRAZER_SIZEOF_NELEM",
	"TRAZER_SIZEOF_ESIZE",
	"TRAZER_EN_NSEQ",
	"TRAZER_EN_CHK",
	"TRAZER_EN_TSTAMP"
};

/*
 * 	Initialized static 'not constant' tables
 */

/*
 * 	Config structure
 */

CONFIG_T configs;

/*
 * 	Static uninitialized variables
 */

static char buffer[ 100 ];			/*	buffer: used for field separation	*/	

/*
 * 		validate_config:
 * 			Verifies that the config name is 
 * 			correct; returns index or
 * 			negative number in case of error
 */

static
int
validate_config( const char *pcomm )
{
	static const char **p;

	for( p = config; p < config + MAX_CONFIGS ; ++p )
		if( strcmp( *p, pcomm ) == 0 )
			return p - config;
	return -1;
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
num_config_cpy( char *ptail )
{
	if( ( ptail = strtok( NULL, sep ) ) == NULL )
		return -1;

	return atoi(ptail);
}
/*
 * 		process_config:
 * 			Process an option from the config file
 * 			If bad option, returns negative, else returns 0
 */
static
int
process_config( FILE *f )
{
	char *p;
	int line;

	for( line = 1; fgets( buffer, sizeof( buffer ), f ) != NULL; ++line )
	{
		if( (strlen( buffer ) == 0) || *buffer == COMMENT 
									|| ( p = strtok( buffer, sep ) ) == NULL )
			continue;

		switch( validate_config( p ) )
		{
			case TRAZER_SIZEOF_SIG_CFG:
				if( (configs.trazer_sizeof_sig = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			case TRAZER_SIZEOF_TSTAMP_CFG:
				if( (configs.trazer_sizeof_tstamp = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			case TRAZER_SIZEOF_POINTER_CFG:
				if( (configs.trazer_sizeof_pointer = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			case TRAZER_SIZEOF_NTIMER_CFG:
				if( (configs.trazer_sizeof_ntimer = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			case TRAZER_SIZEOF_NBLOCK_CFG:
				if( (configs.trazer_sizeof_nblock = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			case TRAZER_SIZEOF_NELEM_CFG:
				if( (configs.trazer_sizeof_nelem = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			case TRAZER_SIZEOF_ESIZE_CFG:
				if( (configs.trazer_sizeof_esize = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			case TRAZER_EN_NSEQ_CFG:
				if( (configs.trazer_en_nseq = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			case TRAZER_EN_CHK_CFG:
				if( (configs.trazer_en_chk = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			case TRAZER_EN_TSTAMP_CFG:
				if( (configs.trazer_en_tstamp = num_config_cpy( p ) ) < 0 )
					return -line;
				break;
			default:
				return -line;
		}
	}
	return 0;
}

/*
 * 		read_conig_file:
 * 			Opens and read configs from the configs
 * 			file
 */
void
read_config_file( char *cfg_file )
{
	FILE *f;
	int line;

	if( ( f = fopen( cfg_file, "rt" ) ) == NULL )
	{
		fatal_error( no_config_file, cfg_file );
	}
	else
	{
		if( ( line = process_config( f ) ) < 0 )
		{
			fclose( f );
			fatal_error( error_in_config_file, cfg_file, -line );
		}
		fclose( f );
	}
}


