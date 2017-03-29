/*
 * 	messages.h
 * 		Here the message table 
 */

#ifndef __MESSAGES_H__
#define __MESSAGES_H__
/*
 * In Shapes parsing
 */

extern const char * err_syntax;
extern const char * err_trans;
extern const char * err_undef;
extern const char * err_redef;
extern const char * err_fopen;

/*
 * 	In file options.c
 */

extern const char   * help_message,
					* usage_msg,
					* option_line_command,
					* no_options_file,
					* getting_options_from,
					* error_in_options_file,
					* error_incomplete_trace_evt_data,
					* error_invalid_trace_id,
					* msg_comm_channel,
					* error_event_id_unknown,
					* no_trace_file,
					* no_config_file,
					* error_in_config_file,
					* invalid_com_config,
					* lost_trace_info,
					* unknown_te,
					* stream_chkerr;

#endif
