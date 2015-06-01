/*
 *	file: unittrz.cpp
 */

#include <vector>
#include "rkh.h"
#include "unittrz.h"

using namespace std;

typedef struct v_u32args_t
{
	vector <rui32_t> args;
}V_U32ARGS_T;

typedef struct utrz_evt
{
	unsigned char id;
	V_U32ARGS_T va;
}UTRZ_EVT_ST;

vector <UTRZ_EVT_ST> utrz_tbl;

void
utrz_clean( void )
{
	utrz_tbl.clear();
}


void
utrz_insert( rui8_t id, rui8_t nargs, ... )
{
	va_list args;
	UTRZ_EVT_ST uevt;

	uevt.id = id;
	va_start( args, nargs );
	while( nargs-- )
		uevt.va.args.push_back( va_arg( args, rui32_t ) );
	utrz_tbl.push_back( uevt );
	va_end(args);
}


ri8_t
utrz_check( rui8_t id, rui8_t nargs, ... )
{
	vector<UTRZ_EVT_ST>::iterator i;
	vector<rui32_t>::iterator j;
	va_list args;
	rui32_t arg;

	va_start( args, nargs );

	for( i = utrz_tbl.begin(); i < utrz_tbl.end(); ++i )
	{
		if( i->id == id )
		{
			if( i->va.args.size() != nargs )
				return -1;

			for( j = i->va.args.begin(); j < i->va.args.end(); ++j )
			{
				arg = va_arg( args, rui32_t );
				if( *j != arg )
				{
					va_end( args );
					return -1;
				}
			}

			va_end( args );
			return 0;
		}
	}

	va_end( args );
	return -1;
}

