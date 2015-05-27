/*
 * cfgdep.cpp
 */


#include "config.h"
#include "cfgdep.h"
#include "tzlog.h"

int
sizeof_type( int type )
{
#ifdef __TRAZER__
	if( type == 1 )
		return 1;
	else if( type == 2 )
		return 2;
	else if( type == 4 )
		return 4;
	else
		return 1;
#else
	if( type == 8 )
		return 1;
	else if( type == 16 )
		return 2;
	else if( type == 32 )
		return 4;
	else
		return 1;
#endif
}

const char*
get_trheader( int en_tstamp, int sz_tstamp, int en_nseq )
{
	switch( RKH_CFGPORT_TRC_SIZEOF_TSTAMP )
	{
#if __TRAZER__
		case 1:		// RKH_TRC_SIZEOF_TSTAMP 8bit
#else
		case 8:
#endif
			return "%3u [%3d] %-4s| %-10s : ";

#if __TRAZER__
		case 2:		// RKH_TRC_SIZEOF_TSTAMP 16bit
#else
		case 16:
#endif
			return "%5u [%3d] %-4s| %-10s : ";

#if __TRAZER__
		case 4:		// RKH_TRC_SIZEOF_TSTAMP 32bit
#else
		case 32:
#endif
		default: 
			return "%10u [%3d] %-4s| %-10s : ";

	}
}

void
trazer_output( int flg, TRAZER_DATA_T* tz )
{
	if( flg & TRAZER_TCFG_EN_TSTAMP_MSK )
	{
		switch( flg & TRAZER_TCFG_SIZEOF_TSTAMP_MSK )
		{
#if __TRAZER__
			case 1:		// RKH_TRC_SIZEOF_TSTAMP 8bit
#else
			case 8:
#endif
				lprintf( " %3u|", tz->ts );
				break;

#if __TRAZER__
			case 2:		// RKH_TRC_SIZEOF_TSTAMP 16bit
#else
			case 16:
#endif
				lprintf( " %5u|", tz->ts );
				break;

#if __TRAZER__
			case 4:		// RKH_TRC_SIZEOF_TSTAMP 32bit
#else
			case 32:
#endif
			default: 
				lprintf( " %10u|", tz->ts );
				break;
		}
	}

	if( flg & TRAZER_TCFG_EN_NSEQ_MSK )
	{
		lprintf( " [%3d] |", tz->nseq );
	}

	lprintf( " %-4s| %-10s : ", tz->group, tz->name );
}

