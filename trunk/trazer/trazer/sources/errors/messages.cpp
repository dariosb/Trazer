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
		"\tf: Source File\n"
		"\tt: Enable HSM Tree info in log file\n"
		"\ts: Enable HSM Sumary info in log file\n"
		"\ti: Log file detailed info mask:\n"
		"\t\t It is a 8bit number, assigned each bit as:\n"
		"\t\t x x x x x x x x\n"
		"\t\t | | | | | | | |___ States Properties\n"
		"\t\t | | | | | | |_____ States Stimulus\n"
		"\t\t | | | | | |_______ Pseudo States Properties\n"
		"\t\t | | | | |_________ Pseudo States Stimulus\n"
		"\t\t | |_|_|___________ Reserved\n"
		"\t\t |_________________ All\n"
		"\to: Target Directory\n"
		"\tc: RKSYS configuartion file (default rksys.h)\n"
		"\td: Enable Debugg info\n"
		"\tv: Version info\n"
		"\th: This help message\n"
};

const char * usage_msg =
{
		"\nUsage: %s\n"
		"Option usage:\n"
		"\t -f Source File\n"
		"\t -t Enable HSM Tree info in log file\n"
		"\t -s Enable HSM Sumary info in log file\n"
		"\t -i Log file detailed info mask\n"
		"\t -o Target Directory\n"
		"\t -c RKSYS configuartion file (default rksys.h)\n"
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
	"\nIncomplete Trace Event data in config file"
};

const char * error_invalid_trace_id =
{
	"\nInvalid Trace Event ID %d"
};

const char * error_event_id_unknown =
{
	"\nEvent ID not defined for: %s"
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


