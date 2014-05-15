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
	switch( RKH_TRC_SIZEOF_TSTAMP )
	{
		case 1:		// RKH_TRC_SIZEOF_TSTAMP 8bit
			return "%3u [%3d] %-4s| %-10s : ";

		case 2:		// RKH_TRC_SIZEOF_TSTAMP 16bit
			return "%5u [%3d] %-4s| %-10s : ";

		case 4:		// RKH_TRC_SIZEOF_TSTAMP 32bit
		default: 
			return "%10u [%3d] %-4s| %-10s : ";

	}
}

