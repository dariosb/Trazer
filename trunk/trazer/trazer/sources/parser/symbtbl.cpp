/*
 * symbtbl.cpp
 */
#include <string>
#include <vector>
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

	symbol.adr = obj;
	symbol.name.assign( sym );

	symb_tbl.push_back( symbol );
}


const char * 
map_obj( unsigned long adr )
{
	vector<SYMOBJ_T>::iterator i;

	for( i = symb_tbl.begin(); i < symb_tbl.end(); ++i )
		if( i->adr == adr )
			return i->name.c_str();

	return NULL;
}
