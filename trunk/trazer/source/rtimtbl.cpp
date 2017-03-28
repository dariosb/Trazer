/*
 * symbtbl.cpp
 */

#include <string>
#include <vector>
#include <cstdio>
#include <string.h>
#include "rtimtbl.h"
#include "trkhtrc.h"
#include "tzpdef.h"
#include "messages.h"

using namespace std;

vector <RTIME_T> rt_tbl;

void
clear_rt_tbl( void )
{
	rt_tbl.clear();
}


void
post_fifo_symevt( unsigned long ao, TRZE_T e, unsigned long ts, 
							unsigned long *nseq )
{
	vector<RTIME_T>::iterator i;
    RTIME_T rtime;
	SYM_EVT_Q evt;

	for( i = rt_tbl.begin(); i < rt_tbl.end(); ++i )
	{
		if( i->ao == ao )
		{
            *nseq = evt.nseq = i->seq_counter;
			evt.tstamp = ts;
			evt.id = e;
			i->se_q.push_back( evt );
            i->seq_counter += 1;
			return;
		}
	}

    rtime.ao = ao;
    *nseq = evt.nseq = rtime.seq_counter = 1;
	evt.tstamp = ts;
	evt.id = e;
    rtime.se_q.push_back(evt);
    rtime.seq_counter += 1;
    rt_tbl.push_back(rtime);
}

void
post_lifo_symevt( unsigned long ao, TRZE_T e, unsigned long ts, 
                        unsigned long *nseq )
{
	vector<RTIME_T>::iterator i;
    RTIME_T rtime;
	SYM_EVT_Q evt;

	for( i = rt_tbl.begin(); i < rt_tbl.end(); ++i )
	{
		if( i->ao == ao )
		{
            *nseq = evt.nseq = i->seq_counter;
			evt.tstamp = ts;
			evt.id = e;
			i->se_q.push_front( evt );
            i->seq_counter += 1;
			return;
		}
	}

    rtime.ao = ao;
    *nseq = evt.nseq = rtime.seq_counter = 1;
	evt.tstamp = ts;
	evt.id = e;
    rtime.se_q.push_back(evt);
    i->seq_counter += 1;
    rt_tbl.push_back(rtime);
}


int
remove_symevt_tstamp( unsigned long ao, TRZE_T e, unsigned long *pt, 
                      unsigned long *nseq )
{
	vector<RTIME_T>::iterator i;
	SYM_EVT_Q evt;
	
	for( i = rt_tbl.begin(); i < rt_tbl.end(); ++i )
	{
		if( i->ao == ao )
		{
			if( i->se_q.size() == 0 )
				break;

			evt = i->se_q.front();
			i->se_q.pop_front();

			if( evt.id != e )
				break;
			
			*pt = evt.tstamp;
            *nseq = evt.nseq;

			return 0;
		}
	}
	return -1;
}
