/*
 * cfgdep.cpp
 */


#include "config.h"
#include "cfgdep.h"

int
sizeof_type( int type )
{
	if( type == 1 )
		return 1;
	else if( type == 2 )
		return 2;
	else if( type == 4 )
		return 4;
	else
		return 1;
}

const char*
get_trheader( void )
{
	if( RKH_TRC_SIZEOF_TSTAMP == 2 )
		return "%5u [%3d] %-4s| %-10s : ";
	else
		return "%10u [%3d] %-4s| %-10s : ";
}
