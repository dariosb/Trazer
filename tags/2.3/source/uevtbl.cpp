/*
 * uevttbl.cpp
 */
#include <string>
#include <vector>
#include "uevtbl.h"
#include "rkhtrc.h"
#include "tzparse.h"
#include "messages.h"

using namespace std;

vector <SYMUEVT_T> uevt_tbl;

void
add_to_uevttbl( uchar uevt, const char * name)
{
	SYMUEVT_T uevtnal;
	vector<SYMUEVT_T>::iterator i;

	for( i = uevt_tbl.begin(); i < uevt_tbl.end(); ++i )
	{
		if( i->uevt == uevt )
		{
			i->name.assign( name );
			return;
		}
		if( strcmp( i->name.c_str(), name ) == 0 )
		{
			i->uevt = uevt;
			return;
		}

	}

	uevtnal.uevt = uevt;
	uevtnal.name.assign( name );

	uevt_tbl.push_back( uevtnal );
}


const char * 
map_uevt( uchar uevt )
{
	vector<SYMUEVT_T>::iterator i;

	for( i = uevt_tbl.begin(); i < uevt_tbl.end(); ++i )
		if( i->uevt == uevt )
			return i->name.c_str();

	return NULL;
}
