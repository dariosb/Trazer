/*
 *	file: unittrz.cpp
 */

#include <vector>
#include "unittrz.h"

using namespace std;

int expect;
UTRZ_EXPECT_EVT expected_evt;

vector <UTRZ_EXPECT_EVT> utrz_expected_lst;
vector <rui8_t> utrz_ignore_lst;


void
utrz_clean( void )
{
    utrz_expected_lst.clear();
}


void
utrz_chk_expect( rui8_t nargs, ... )
{
    
}

void
utrz_add_expect( rui8_t nargs, ... )
{
	va_list args;
	UTRZ_ARG_T va;

	va_start( args, nargs );
	while( nargs-- )
	{
		va.ignored = EVT_EXPECTED;
		va.value = va_arg( args, rui32_t );
		expected_evt.va.args.push_back( va );
	}
	utrz_expected_lst.push_back( expected_evt );
	va_end(args);
    END_EXPECT();
}


void
sm_init_ignore( rui8_t id )
{

}


void
sm_init_ignoreArg_initState( unsigned int line, rui8_t id, rui8_t arg_ix )
{

}

#if 0
vector <UTRZ_EVT_ST> utrz_tbl;

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
