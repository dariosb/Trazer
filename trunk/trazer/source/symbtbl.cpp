/*
 * symbtbl.cpp
 */
#include <string>
#include <vector>
#include <cstdio>
#include "symbtbl.h"
#include "rkhtrc.h"
#include "tzparse.h"
#include "messages.h"

using namespace std;

vector <SYMOBJ_T> symb_tbl;

void
add_to_symtbl( unsigned long obj, const char *sym )
{
	SYMOBJ_T symbol;
	vector<SYMOBJ_T>::iterator i;

	for( i = symb_tbl.begin(); i < symb_tbl.end(); ++i )
	{
		if( i->adr == obj )
		{
			i->name.assign( sym );
			return;
		}
		if( strcmp(i->name.c_str(), sym ) == 0 )
		{
			i->adr = obj;
			return;
		}
	}

	symbol.adr = obj;
	symbol.name.assign( sym );

	symb_tbl.push_back( symbol );
}


const char * 
map_obj( unsigned long adr )
{
	vector<SYMOBJ_T>::iterator i;
	char buff [2+sizeof(long)*8+1];

	for( i = symb_tbl.begin(); i < symb_tbl.end(); ++i )
		if( i->adr == adr )
			return i->name.c_str();

	if( adr == 0 )
		strcpy( buff, "?" );
	else
		sprintf( buff, "0X%X", (unsigned int)adr );

	add_to_symtbl( adr, buff );
	return symb_tbl.back().name.c_str();
}


void
post_fifo_symevt( unsigned long adr, TRZE_T e, unsigned long ts )
{
	vector<SYMOBJ_T>::iterator i;
	SYM_EVT_Q evt;

	for( i = symb_tbl.begin(); i < symb_tbl.end(); ++i )
	{
		if( i->adr == adr )
		{
			evt.tstamp = ts;
			evt.id = e;
			i->se_q.push_back( evt );
			return;
		}
	}
}

void
post_lifo_symevt( unsigned long adr, TRZE_T e, unsigned long ts )
{
	vector<SYMOBJ_T>::iterator i;
	SYM_EVT_Q evt;

	for( i = symb_tbl.begin(); i < symb_tbl.end(); ++i )
	{
		if( i->adr == adr )
		{
			evt.tstamp = ts;
			evt.id = e;
			i->se_q.push_front( evt );
			return;
		}
	}
}


int
remove_symevt_tstamp( unsigned long adr, TRZE_T e, unsigned long *pt )
{
	static vector<SYMOBJ_T>::iterator i;
	static SYM_EVT_Q evt;

	for( i = symb_tbl.begin(); i < symb_tbl.end(); ++i )
	{
		if( i->adr == adr )
		{
			if( i->se_q.size() == 0 )
				return 0xFF00;

			evt = i->se_q.front();
			i->se_q.pop_front();

			if( evt.id != e )
				break;
			
			*pt = evt.tstamp;

			return 0;
		}
	}
	return -1;
}
