/*
 * 	sertbl.c
 */

#include "serdefs.h"
#include "serial.h"

SERIAL_T serials[ NUM_SERIALS ] =
{
	{	"COM1",	9600, 8, PAR_NONE,	STOP_1, 0 },
};

