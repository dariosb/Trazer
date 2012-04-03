/*
 * 	options.h
 */
#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <string>
using namespace std;

enum
{
	DISABLE_OPT,
	ENABLE_OPT
};

/* 
 * HSMINFO Masks
 */

typedef struct
{
	unsigned state_props;
	unsigned state_stimu;
	unsigned pstate_props;
	unsigned pstate_stimu;
}HSM_INFO_MSK_ST;

typedef struct
{
	unsigned enable_debug;
	string instream_file;
	string instream_comport;
	string baudrate;
	string parity;
} OPTIONS_T;

#define CONSOLE_OPTIONS_DEFINITION	"df:c:::hv"

#define OPTIONS_FILE				"trazer.opt"

#define SAME_EXEC_PATH				"."

extern OPTIONS_T options;

void read_option_file( char *opt_file );
void usage( char *name );
void evaluate_args( int argc, char **argv );
void show_help( void );

void init_options( int argc, char **argv );

#endif
