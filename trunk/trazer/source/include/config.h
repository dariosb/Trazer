/*
 * 	config.h
 */
#ifndef __CONFIG_H__
#define __CONFIG_H__


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

/*
 * RKH default configuration
 */

#define	TRAZER_SIZEOF_SIG_DFT		1
#define	TRAZER_SIZEOF_TSTAMP_DFT	4
#define	TRAZER_SIZEOF_POINTER_DFT	4
#define	TRAZER_SIZEOF_NTIMER_DFT	2
#define	TRAZER_SIZEOF_NBLOCK_DFT	1
#define	TRAZER_SIZEOF_NELEM_DFT		1
#define	TRAZER_SIZEOF_ESIZE_DFT		2
#define	TRAZER_EN_NSEQ_DFT			1
#define	TRAZER_EN_CHK_DFT			1
#define	TRAZER_EN_TSTAMP_DFT		1

#define CONFIG_FILE				"trazer.cfg"

extern CONFIG_T configs;

void read_config_file( char *cfg_file );

#endif
