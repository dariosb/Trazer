
#ifndef __EVTBL_H__
#define __EVTBL_H__


#include <string>
#include <vector>
#include "tzparse.h"

using namespace std;

typedef struct fmt_id_t
{
	TRE_T tre;
	string idstr;
//	struct event_info_st *evinfo;
}FMT_ID_T;

/*
typedef struct event_info_st
{
	string event;
	string group;
	string name;
	string args;
	string comment;
	struct fmt_id_t *fmtid;
}EVENT_INFO_ST;*/

//int add_to_evtbl( EVENT_INFO_ST *p );

const TRE_T *find_trevt( unsigned char id );

string * get_evt_comment( unsigned char id );

//extern vector <EVENT_INFO_ST> event_tbl;

const TRE_T *point_2_trevt( unsigned char id );
string trevt_name( unsigned int ix );

#define TR_EVT_PRN_ARG_SEP	", "
#define TR_EVT_PRNBUFF_SIZE	100

#endif
