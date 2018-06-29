/*
 * cfgdep.cpp
 */


#include "config.h"
#include "cfgdep.h"
#include "tzlog.h"

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
get_trheader( int en_tstamp, int sz_tstamp, int en_nseq )
{
	(void)(en_nseq);
	(void)(en_tstamp);
	(void)(sz_tstamp);

	switch( TRZ_RKH_CFGPORT_TRC_SIZEOF_TSTAMP )
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

void
trazer_output( TRAZER_DATA_T* tz )
{
    int flg = PREPARE_TZOUT_FLGS( TRZ_RKH_CFG_TRC_TSTAMP_EN, 
										TRZ_RKH_CFGPORT_TRC_SIZEOF_TSTAMP,
										TRZ_RKH_CFG_TRC_NSEQ_EN );

	if( flg & TRAZER_TCFG_EN_TSTAMP_MSK )
	{
		switch( flg & TRAZER_TCFG_SIZEOF_TSTAMP_MSK )
		{
			case 1:		// RKH_TRC_SIZEOF_TSTAMP 8bit
				lprintf( TRAZER_FMT_TSTAMP_8BIT, tz->ts );
				break;

			case 2:		// RKH_TRC_SIZEOF_TSTAMP 16bit
				lprintf( TRAZER_FMT_TSTAMP_16BIT, tz->ts );
				break;

			case 4:		// RKH_TRC_SIZEOF_TSTAMP 32bit
			default: 
				lprintf( TRAZER_FMT_TSTAMP_32BIT, tz->ts );
				break;
		}
	}

	if( flg & TRAZER_TCFG_EN_NSEQ_MSK )
	{
		lprintf( TRAZER_FMT_NSEQ, tz->nseq );
	}

	lprintf( TRAZER_FMT_TRACE_EVT, tz->group, tz->name );
}

