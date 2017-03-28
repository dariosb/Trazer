/*
 * symbtbl.cpp
 */

#include <string>
#include <vector>
#include <cstdio>
#include <string.h>
#include "symbtbl.h"
#include "trkhtrc.h"
#include "tzpdef.h"
#include "messages.h"

using namespace std;

vector <SYMOBJ_T> symb_tbl;

void
clear_symtbl( void )
{
	symb_tbl.clear();
}


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


const char *
search_in_objtbl(  unsigned long  adr  )
{
	vector<SYMOBJ_T>::iterator i;

	for( i = symb_tbl.begin(); i < symb_tbl.end(); ++i )
		if( i->adr == adr )
			return i->name.c_str();
    
    return NULL;
}

