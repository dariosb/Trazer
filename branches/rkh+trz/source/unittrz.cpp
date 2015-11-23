/*
 *	file: unittrz.cpp
 */

#ifndef __TRAZER__

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

typedef struct utrz_arg_t
{
	rui8_t	ignored;
	rui32_t	value;
}UTRZ_ARG_T;

typedef struct v_utrz_arg_t
{
	vector <utrz_arg_t> args;
}V_UTRZ_ARG_T;

typedef struct utrz_expect_evt
{
	unsigned int line;
	rui8_t id;
	V_UTRZ_ARG_T va;
}UTRZ_EXPECT_EVT;

vector <UTRZ_EXPECT_EVT> utrz_expect_evttbl;
vector <rui8_t> utrz_ignore_evttbl;


void
utrz_clean( void )
{
	utrz_tbl.clear();
}


void
utrz_expect( unsigned int line, rui8_t id, rui8_t nargs, ... )
{

}


void
sm_init_ignore( rui8_t id )
{

}


void
sm_init_ignoreArg_initState( unsigned int line, rui8_t id, rui8_t arg_ix )
{

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



#endif
