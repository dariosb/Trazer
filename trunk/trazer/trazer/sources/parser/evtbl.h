
#ifndef __EVTBL_H__
#define __EVTBL_H__


#include <string>
#include <vector>
#include "tzparse.h"

using namespace std;

typedef struct fmt_id_t
{
	int id;
	string idstr;
	HDLR_T fmt;
	struct event_info_st *evinfo;
}FMT_ID_T;

typedef struct event_info_st
{
	string event;
	int id;
	string group;
	string name;
	string args;
	string comment;
	struct fmt_id_t *fmtid;
}EVENT_INFO_ST;

int add_to_evtbl( EVENT_INFO_ST *p );

#endif
