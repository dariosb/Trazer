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
#include "tzparse.h"
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
