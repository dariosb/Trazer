/*
 * 	messages.c
 * 		Here the message table for
 * 		each recognized language
 */


#include "messages.h"

/*
 * In Shapes parsing
 */

extern const char * err_syntax	=
{	
	"\nSyntax Error in %s: %s, %s\n"
};

extern const char * err_trans =
{
	"\nError in %s: %s, %s -> %s, %s\n"
};

extern const char * err_undef =
{
	"\nError in %s: %s, %s property not definition\n"
};

extern const char * err_redef =
{
	"\nError in %s: %s, %s property redefinition\n"
};

extern const char * err_fopen =
{
	"\nCan't open file %s\n"
};


/*
 * 	In file options.c
 */

const char * help_message =
{
		"\nOption usage:\n"
		"\tf: Parse from binary stream file\n"
		"\tc: Parse from COM Port\n"
		"\t      COM Port setup must be specified ex:\n"
		"\t      trazer -c COM1 115200 8N1.\n"
		"\td: Enable Debugg info\n"
		"\tv: Version info\n"
		"\th: This help message\n"
};

const char * usage_msg =
{
		"\nUsage: %s\n"
		"Option usage:\n"
		"\t -f Parse from binary stream file\n"
		"\t -c Parse from COM Port\n"
		"\t      COM Port setup must be specified ex:\n"
		"\t      trazer -c COM1 115200 8N1.\n"
		"\t -d Enable Debugg info\n"
		"\t -v Version info\n"
		"\t -h (help)\n"
};

const char * no_options_file =
{
		"\nWARNING: Event definition file %s doesn't exists...\n"
};

const char * getting_options_from =
{
		"\nReading Event definition from file %s\n"
};

const char * error_in_options_file =
{
	"\nError in options file %s on line %d\n"
};

const char * error_incomplete_trace_evt_data =
{
	"\nIncomplete Trace Event data in config file\n"
};

const char * error_invalid_trace_id =
{
	"\nInvalid Trace Event ID %d\n"
};

const char * error_event_id_unknown =
{
	"\nEvent ID not defined for: %s\n"
};

const char * no_trace_file =
{
		"\nERROR: Trace file %s doesn't exists...\n"
};

const char * no_config_file =
{
		"\nERROR: Config file %s doesn't exists...\n"
};

const char * error_in_config_file =
{
	"\nError in config file %s on line %d\n"
};


const char * invalid_com_config =
{
	"\nInvalid Serial Port configuration\n"
};


