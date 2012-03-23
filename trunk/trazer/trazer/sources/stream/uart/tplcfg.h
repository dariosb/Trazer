/*
 * tplcfg.h
 *
 *	<SOF> <addr> <nDataH> <nDataL> <Data-n> …. <Data-0> <CHK-H> <CHK-L>
 *
 *	SOF: 0xF1
 *
 *	ESC: 0xF2
 *	ESC_MASK: 0x0F
 *		Escape codes:
 *			0x01 – SOF
 *			0x02 – ESC 
 *
 *	Checksum:
 *		mod-256, from nData..Data-n splitted each nibble in 2 bytes, 
 *		right justified plus CHECK_OFFSET (0xA0), then these are bounded to 
 *		0xA0 - 0xAF. 
 *		Then checksum bytes never has to be escaped.
 */	

#ifndef __TPLCFG_H__
#define __TPLCFG_H__

#include "mytypes.h"

#define __TPLINK_DBG__	1

#define TPLINK_DEV_TYPE			TPLINK_DEV_SLAVE	/* Device type */


#define TPLINK_ADDRESSING_ON	1					/* 1/0 Enable/Disable Protocol Slaves Addressing */
#define TPLINK_SLAVE_ADDRESS	0x00				/* Device Address [0x01 .. 0x7F] */

#define TPLINK_SOF				0xF1				/* Start of Frame "MARK" */
#define TPLINK_ESC				0xF2				/* Escape character */

#define TPLINK_MAX_PLOAD_SIZE	512					/* Max Payload Size */

#define TPLINK_INTER_BYTE_TIME	200					/* Inter Byte timeout in ms */
#define TPLINK_INTER_FRAME_TIME	20000				/* Inter Frame timeout in ms */

#define __USE_TPLINK_ONXMIT_CMP_CBF__	0			/* Enable/Disable transmit complete call back function */
#define __USE_TPLINK_ONCHERR_CBF__		1			/* Enable/Disable check sum error call back function */

#endif
