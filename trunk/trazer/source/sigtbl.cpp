/*
 * sigtbl.cpp
 */
#include <string>
#include <vector>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "sigtbl.h"
#include "rkhtrc.h"
#include "tzpdef.h"
#include "messages.h"
#include "seqdiag.h"

using namespace std;

vector <SYMSIG_T> sig_tbl;

void
add_to_sigtbl( TRZE_T sig, const char * name)
{
	SYMSIG_T signal;
	vector<SYMSIG_T>::iterator i;

	for( i = sig_tbl.begin(); i < sig_tbl.end(); ++i )
	{
		if( i->sig == sig )
		{
			i->name.assign( name );
			return;
		}
		if( strcmp( i->name.c_str(), name ) == 0 )
		{
			i->sig = sig;
			return;
		}

	}

	signal.sig = sig;
	signal.name.assign( name );

	sig_tbl.push_back( signal );
}


const char * 
map_sig( TRZE_T sig )
{
	vector<SYMSIG_T>::iterator i;
	char buff [2+sizeof(long)*8+1];	

	for( i = sig_tbl.begin(); i < sig_tbl.end(); ++i )
		if( i->sig == sig )
			return i->name.c_str();

	sprintf( buff, "0X%X", (unsigned int)sig );
	add_to_sigtbl( sig, buff );
	return sig_tbl.back().name.c_str();	
}


const char *
search_in_sigtbl(  TRZE_T sig  )
{
	vector<SYMSIG_T>::iterator i;

	for( i = sig_tbl.begin(); i < sig_tbl.end(); ++i )
		if( i->sig == sig )
			return i->name.c_str();

    return NULL;
}



vector <TMREVT_T> tmr_tbl;

void
tmrtbl_init( ulong tobj, ulong sobj )
{
	TMREVT_T tmr;

	tmr.tobj = tobj;
	tmr.sobj = sobj;
	tmr.smobj = NULL;

	tmr_tbl.push_back( tmr );
}


void
tmrtbl_start( ulong tobj, ulong smobj )
{
	vector<TMREVT_T>::iterator i;

	for( i = tmr_tbl.begin(); i < tmr_tbl.end(); ++i )
	{
		if( i->tobj == tobj )
			i->smobj = smobj;
	}
}


uint
tmrtbl_find( ulong tobj, TMREVT_T *p )
{
	vector<TMREVT_T>::iterator i;

	for( i = tmr_tbl.begin(); i < tmr_tbl.end(); ++i )
	{
		if( i->tobj == tobj )
		{
			*p = *i;
			return 1;
		}
	}
	return 0;
}

uint
tmrtbl_find_sig( ulong sobj )
{
	vector<TMREVT_T>::iterator i;

	for( i = tmr_tbl.begin(); i < tmr_tbl.end(); ++i )
	{
		if( i->sobj == sobj )
			return 1;
	}
	return 0;	
}


