/*
 *	file: unittrz.cpp
 */

#include <vector>
#include <list>
#include <string.h>
#include "unittrz.h"
#include "tzlog.h"
#include "tzparse.h"
#include "evexptbl.h"
#include "rkhtrc.h"
#include "utrzhal.h"

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
	rkh_trc_init();
    utrz_expected_lst.clear();
    memset( utrz_ign_group, GRP_EXPECTED, sizeof(utrz_ign_group) );
    memset( utrz_ign_evt, EVT_EXPECTED, sizeof(utrz_ign_evt) );
    utrz_success();
}


void
utrz_verify( char *ptext )
{
    if( utrz_expected_lst.empty() )
    {
        utrz_success();
    }
    else
    {
        utrzVerify_fail();
    }
}


void
utrz_add_expect_any_args( rui32_t line, rui32_t e )
{
	UTRZ_ARG_T va;
    const TRE_T *p;
    rui8_t nargs;

    NEW_EXPECT_EVT( line, e );

    /** removes ignore attribute **/
    utrz_ign_evt[ expected_evt.id ] = EVT_EXPECTED;

    p = find_exp_trevt( e );
    
    nargs = p->va.args.size();

	while( nargs-- )
	{
		va.ignored = EVT_IGNORED;
        va.value = 0;
		expected_evt.va.args.push_back( va );
	}
	utrz_expected_lst.push_back( expected_evt );

    END_EXPECT();
    utrz_success();
}


void
utrz_add_expect( rui8_t nargs, ... )
{
	va_list args;
	UTRZ_ARG_T va;

    /** removes ignore attribute **/
    utrz_ign_evt[ expected_evt.id ] = EVT_EXPECTED;

	va_start( args, nargs );
	while( nargs-- )
	{
		va.ignored = ARG_EXPECTED;
		va.value = va_arg( args, rui32_t );
		expected_evt.va.args.push_back( va );
	}
	utrz_expected_lst.push_back( expected_evt );
	va_end(args);
    END_EXPECT();

    utrz_success();
}

void
utrz_chk_expect( rui8_t id, rui8_t nargs, ... )
{
	va_list args;
    UTRZ_EXPECT_EVT rcv_evt;
    UTRZ_EXPECT_EVT exp_evt;
	CHEK_ARG_T p;
   	vector<rui32_t>::iterator arg_ix;

    if( utrz_expected_lst.empty() || is_ignored( id ) )
    {
        utrz_success();
        return;
    }

    /** get expected trace event from list **/
    exp_evt = utrz_expected_lst.front();
    utrz_expected_lst.pop_front();

    /** compare trace events id´s  **/
    if( exp_evt.id != id )
    {
        utrzEvtExpect_fail( exp_evt.line, 
                find_trevt(id)->name.c_str(), 
                find_trevt(exp_evt.id)->name.c_str() );

        return;
    }

    /*** compare Trace events args expected and received */

    rcv_evt.id = id;

    p = find_exp_chk_fun( id );
	va_start( args, nargs );
    p( &exp_evt, nargs, args );
	va_end(args);
}


void
utrz_ignore_arg( rui32_t line, rui32_t e, rui8_t ix )
{
    UTRZ_EXPECT_EVT last_expect; 
    
    last_expect = utrz_expected_lst.back();

    /** check if event refered is equal to last expected **/
    if( e == last_expect.id )
    {
        last_expect.va.args[ix].ignored = ARG_IGNORED;
		utrz_expected_lst.pop_back();
		utrz_expected_lst.push_back( last_expect );
		utrz_success();
    }
    else
	{
        utrzIgnArg_fail( line, find_trevt(e)->name.c_str() );
	}
}


void
utrz_ignore_group( RKH_TG_T grp )
{
    if( grp < RKH_TG_NGROUP )
        utrz_ign_group[ grp ] = GRP_IGNORED;

    utrz_success();
}

void
utrz_ignore_evt( rui32_t e )
{
    if(  e < RKH_TE_NEVENT )
        utrz_ign_evt[ e ] = EVT_IGNORED;

    utrz_success();
}

