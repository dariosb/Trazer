/*
 * 	config.h
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>

using namespace std;

typedef struct
{
	int trazer_sizeof_sig;
	int trazer_sizeof_tstamp;
	int trazer_sizeof_pointer;	
	int trazer_sizeof_ntimer;
	int trazer_sizeof_nblock;
	int trazer_sizeof_nelem;
	int trazer_sizeof_esize;
	int trazer_en_nseq;
	int trazer_en_chk;
	int trazer_en_tstamp;
} CONFIG_T;

#define CONFIG_FILE				"trazer.cfg"

extern CONFIG_T configs;

void read_config_file( char *cfg_file );

#endif
