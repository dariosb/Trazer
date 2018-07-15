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
	int enable_debug;
	int quiet;
	string outfile;
	string instream_tcpsock;
	string symbols_file;
	string instream_file;
	string instream_comport;
	string baudrate;
	string parity;
} OPTIONS_T;

#define CONSOLE_OPTIONS_DEFINITION	"s:t:dqo:f:c:::hv"

#define OPTIONS_FILE				"trazer.evt"

extern OPTIONS_T options;

void read_option_file( char *opt_file );
void usage( char *name );
void evaluate_args( int argc, char **argv );
void show_help( void );

void init_options( int argc, char **argv );

#endif
