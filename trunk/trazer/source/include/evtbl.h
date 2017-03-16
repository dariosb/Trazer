
#ifndef __EVTBL_H__
#define __EVTBL_H__

#include <string>
#include "tzparse.h"

typedef struct fmt_id_t
{
	TRE_T tre;
	string idstr;
}FMT_ID_T;

const TRE_T *find_trevt( unsigned int id );
const TRE_T *find_exp_trevt( unsigned int id );

string * get_evt_comment( unsigned int id );

#define TR_EVT_PRN_ARG_SEP	", "
#define TR_EVT_PRNBUFF_SIZE	100

const TRG_T * find_trgroup( RKH_TG_T grp );

#endif
