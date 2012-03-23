/*
 * tpldefs.h
 *  		TPLINK internal definitions
 */

#ifndef __TPLDEFS_H__
#define __TPLDEFS_H__

#include "tplcfg.h"
#include "mytypes.h"

/*
 * TPLINK escape codes. 
 */
enum
{
	TPLINK_ESC_SOF = 1,
	TPLINK_ESC_ESC,
	TPLINK_NUM_ESC_CODES
};

#define TPLINK_ESC_MASK	(uchar)(0x0F)

/*
 * Macros to Checksum calculation and verifing.
 */
#define CHECK_MASKH		(uchar)(0xF0)
#define CHECK_MASKL		(uchar)(0x0F)
#define CHECK_OFFSET	(uchar)(0xA0)

#define calc_check(s)  (ushort)( ((((s & CHECK_MASKH)<<4)+(CHECK_OFFSET<<8))) \
			           		   | (((s & CHECK_MASKL)+CHECK_OFFSET)) )

#define TPL_SOF			(uchar)(TPLINK_SOF)
#define TPL_ESC			(uchar)(TPLINK_ESC)


/*
 * Device Types
 */
#define TPLINK_DEV_MASTER		0
#define TPLINK_DEV_SLAVE		1

#define TPLINK_COMFLOW_MSK		0x80					/* Com flow mask, Master to Slave */
#define TPLINK_BROADCAST_ADDR	0x7F					/* Broad Cast Address */

#if ( TPLINK_BROADCAST_ADDR == TPLINK_SLAVE_ADDRESS )
#error 	"Device and Broadcast Addresses must be diferents"
#endif

#if ( TPLINK_SLAVE_ADDRESS > 0x80 )
#error "Device Address definition out of range [0x01 .. 0x7F]"
#endif

#if ( TPLINK_SOF == TPLINK_ESC )
#error "TPLINK_SOF and TPLINK_ESC definition must be different"
#endif

#if ( (TPLINK_SLAVE_ADDRESS == TPLINK_SOF) || (TPLINK_SLAVE_ADDRESS == TPLINK_ESC) )
#error "Device Address must be different than TPLINK_SOF and TPLINK_ESC"
#endif

#if ( TPLINK_DEV_TYPE == TPLINK_DEV_SLAVE )
#ifndef TPLINK_SLAVE_ADDRESS
#error "TPLINK_SLAVE_ADDERESS must be defined for Slave Device"
#endif
#define get_slave_addr()	(uchar)(TPLINK_SLAVE_ADDRESS)
#endif

#endif
