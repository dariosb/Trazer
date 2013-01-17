/*
 * 		serial.c
 * 			Hardware dependent
 * 			serial port functions
 */

#include "options.h"
//#include "errors.h"
#include "messages.h"

void
init_serial( void )
{
#if 0
	int br;
	parbuff[3];

	strncpy( serials.com_name, options.instream_comport, sizeof(serials.com_name));
	serials.baud = atoi( options.baudrate );
	strncpy( parbuff, options.parity.c_str(), sizeof(parbuff) );

	serials.bit_num = parbuff[0] - '0';
	
	switch( parbuff[1] )
	{
		case 'N':
		case 'n':
			serials.parity = PAR_NONE;
			break;
		case 'O':
		case 'o':
			serials.parity = PAR_ODD;
			break;
		case 'E':
		case 'e':
			serials.parity = PAR_EVEN;
			break;
		case 'M':
		case 'm':
			serials.parity = PAR_MARK;
			break;
		case 'S':
		case 's':
			serials.parity = PAR_SPACE;
			break;
		default
			fatal_error( invalid_com_config );

	}

	if( parbuff[2] == '0' )
		fatal_error( invalid_com_config );
	else if( parbuff[2] == '1' )
		serials.stop_num = STOP_1;
	else
		serials.serials = STOP2;

	serials.is_modem = 0;

#endif
}
