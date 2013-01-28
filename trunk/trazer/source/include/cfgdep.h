/*
 * cfgdep.h
 */

#ifndef __CFGDEP_H__
#define __CFGDEP_H__

#include "config.h"

extern CONFIG_T configs;

#define TRAZER_SIZEOF_SIG			configs.trazer_sizeof_sig
#define TRAZER_SIZEOF_TSTAMP		configs.trazer_sizeof_tstamp
#define TRAZER_SIZEOF_POINTER		configs.trazer_sizeof_pointer
#define TRAZER_SIZEOF_NTIMER		configs.trazer_sizeof_ntimer
#define TRAZER_SIZEOF_NBLOCK		configs.trazer_sizeof_nblock
#define TRAZER_SIZEOF_NELEM			configs.trazer_sizeof_nelem
#define TRAZER_SIZEOF_ESIZE			configs.trazer_sizeof_esize
#define TRAZER_EN_NSEQ				configs.trazer_en_nseq
#define TRAZER_EN_CHK				configs.trazer_en_chk
#define TRAZER_EN_TSTAMP			configs.trazer_en_tstamp

int sizeof_type( int type );

#define sizeof_trze()		sizeof_type( TRAZER_SIZEOF_SIG )
#define sizeof_trzts()		sizeof_type( TRAZER_SIZEOF_TSTAMP )
#define sizeof_trznt()		sizeof_type( TRAZER_SIZEOF_NTIMER )
#define sizeof_trznb()		sizeof_type( TRAZER_SIZEOF_NBLOCK )
#define sizeof_trzne()		sizeof_type( TRAZER_SIZEOF_NELEM )
#define sizeof_trzes()		sizeof_type( TRAZER_SIZEOF_ESIZE )

const char *get_trheader( void );
const char *get_usr_trheader( void );
#define trheader	get_trheader()
#define usr_trheader	get_usr_trheader()


#endif
