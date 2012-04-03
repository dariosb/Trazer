/*
 * 		serial.c
 * 			Hardware dependent
 * 			serial port functions
 */

#include "options.h"
#include "messages.h"
#include "serial.h"
#include "serdefs.h"
#include "error.h"

//extern SERIAL_T serials[NUM_SERIALS];
static SERIAL_CBACK_T serial_callback;

#include "tzparse.h"

static
void
ser_rx_isr( unsigned char byte )
{
	trazer_parse( byte );
}

void
init_serial( void )
{
	char parbuff[3];

	strncpy( serials[TRAZZER_SERIAL].com_name, options.instream_comport.c_str(), sizeof(serials[TRAZZER_SERIAL].com_name));
	serials[TRAZZER_SERIAL].baud = atoi( options.baudrate.c_str() );
	strncpy( parbuff, options.parity.c_str(), sizeof(parbuff) );

	serials[TRAZZER_SERIAL].bit_num = parbuff[0] - '0';
	
	switch( parbuff[1] )
	{
		case 'N':
		case 'n':
			serials[TRAZZER_SERIAL].parity = PAR_NONE;
			break;
		case 'O':
		case 'o':
			serials[TRAZZER_SERIAL].parity = PAR_ODD;
			break;
		case 'E':
		case 'e':
			serials[TRAZZER_SERIAL].parity = PAR_EVEN;
			break;
		case 'M':
		case 'm':
			serials[TRAZZER_SERIAL].parity = PAR_MARK;
			break;
		case 'S':
		case 's':
			serials[TRAZZER_SERIAL].parity = PAR_SPACE;
			break;
		default:
			fatal_error( invalid_com_config );

	}

	if( parbuff[2] == '0' )
		fatal_error( invalid_com_config );
	else if( parbuff[2] == '1' )
		serials[TRAZZER_SERIAL].stop_num = STOP_1;
	else
		serials[TRAZZER_SERIAL].stop_num = STOP2;

	serials[TRAZZER_SERIAL].is_modem = 0;
	
	memset( &serial_callback, 0, sizeof( serial_callback ) );
	serial_callback.rx = &ser_rx_isr;
	init_serial_hard( TRAZZER_SERIAL, &serial_callback );
	connect_serial( TRAZZER_SERIAL );

}


