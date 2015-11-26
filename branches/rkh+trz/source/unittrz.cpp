/*
 *	file: unittrz.cpp
 */

#include <vector>
#include <list>
#include "unittrz.h"
#include "tzlog.h"

using namespace std;

int expect;
UTRZ_EXPECT_EVT expected_evt;
list <UTRZ_EXPECT_EVT> utrz_expected_lst;

static RKH_TG_T utrz_ign_group[ RKH_TG_NGROUP ]; 
static rui32_t utrz_ign_evt[ RKH_TE_NEVENT ]; 

bool
is_ignored( rui32_t e  )
{
    if( utrz_ign_group[GETGRP(e)] == GRP_IGNORED )
        return true;

    if( utrz_ign_evt[e] == EVT_IGNORED )
        return true;

    return false;
}


void
utrz_clean( void )
{
    utrz_expected_lst.clear();
    memset( utrz_ign_group, GRP_EXPECTED, sizeof(utrz_ign_group) );
    memset( utrz_ign_evt, EVT_EXPECTED, sizeof(utrz_ign_evt) );
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
utrz_chk_expect( rui8_t id, rui8_t nargs, ... )
{
	va_list args;
	UTRZ_ARG_T va;
    UTRZ_EXPECT_EVT rcv_evt;
    UTRZ_EXPECT_EVT exp_evt;
   	vector<rui32_t>::iterator arg_ix;

    if( utrz_expected_lst.empty() )
        return; 

    if( is_ignored( id ) )
        return;

    rcv_evt.id = id;

    /** process incoming trace event args **/
	va_start( args, nargs );
	while( nargs-- )
	{
		va.ignored = EVT_EXPECTED;
		va.value = va_arg( args, rui32_t );        
        rcv_evt.va.args.push_back( va );
	}
	va_end(args);

    /** get expected trace event from list **/
    exp_evt = utrz_expected_lst.front();
    utrz_expected_lst.pop_front();

    /** compare trace events id´s  **/
    if( exp_evt.id != rcv_evt.id )
    {
        lprintf("FAIL: Trace Events diferents\n");
        return;
    }

    /*** TODO: compare Trace events args expected and received */


}

void
utrz_ignore_group( RKH_TG_T grp )
{
    if( grp >= RKH_TG_NGROUP )
        return;

    utrz_ign_group[ grp ] = GRP_IGNORED;
}

void
utrz_ignore_evt( rui32_t e )
{
    if(  e >= RKH_TE_NEVENT )
        return;

    utrz_ign_evt[ e ] = EVT_IGNORED;
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
