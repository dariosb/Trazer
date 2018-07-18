/*
 * 		serial.c
 * 			Hardware dependent
 * 			serial port functions
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <termios.h>

#include "tzparse.h"
#include "options.h"
#include "messages.h"
#include "serial.h"
#include "serdefs.h"
#include "error.h"

//extern SERIAL_T serials[NUM_SERIALS];
//static SERIAL_CBACK_T serial_callback;


static int tty_fd;
static int running;
static struct termios tio;
static struct termios old_tio;

static (void)(*serHook)(unsigned char c);

static 
void *
isr_ser_thread( void *d )	/* thread to emulate timer ISR */
{
	char buff[64];
	int count;
	char *p;

	(void)d;
    while( running ) 
	{
		if ( ( count = read(tty_fd, buff, 64) ) > 0 )
		{
			p = buff;
			while( count-- )
                serHook(*p++);
		}
    }
	pthread_exit(NULL);
	return NULL;
}

void
init_serial( void (*prcv)(unsigned char byte) )
{
	char parbuff[10];
	char fname[20];
	int baud, sbaud;
	pthread_t thser_id;
 	pthread_attr_t threadAttr;

	strncpy( parbuff, options.parity.c_str(), sizeof(parbuff) );

	memset(&tio, 0, sizeof(tio));

	tio.c_iflag = 0;
	tio.c_oflag = 0;

	switch( parbuff[0] )
	{
		case '8':
			tio.c_cflag = CS8|CREAD|CLOCAL;
			break;
		case '7':
			tio.c_cflag = CS7|CREAD|CLOCAL;
			break;
		case '6':
			tio.c_cflag = CS6|CREAD|CLOCAL;
			break;
		case '5':
			tio.c_cflag = CS5|CREAD|CLOCAL;
			break;
		default:
			fatal_error( invalid_com_config );
			break;
	}

	switch( parbuff[1] )
	{
		case 'N':
		case 'n':
			break;
		case 'O':
		case 'o':
			tio.c_cflag |= PARENB|PARODD;
			break;
		case 'E':
		case 'e':
			tio.c_cflag |= PARENB;
			break;
		default:
			fatal_error( invalid_com_config );

	}

	if( parbuff[2] == '0' )
		fatal_error( invalid_com_config );

	strcpy( fname, options.instream_comport.c_str() );
	
	baud = atoi( options.baudrate.c_str() );


	switch( baud )
	{
		case 2400:
			sbaud = B2400;
			break;
		case 4800:
			sbaud = B4800;
			break;
		case 9600:
			sbaud = B9600;
			break;
		case 19200:
			sbaud = B19200;
			break;
		case 38400:
			sbaud = B38400;
			break;
		case 57600:
			sbaud = B57600;
			break;
		case 115200:
			sbaud = B115200;
			break;
		default:
			break;
	}

	cfsetospeed(&tio, sbaud);
	cfsetispeed(&tio, sbaud); 

	tty_fd = open( fname, O_RDWR | O_NOCTTY | O_SYNC);

	if( tty_fd < 0 )
		fatal_error( invalid_com_config );

	tcgetattr(tty_fd, &old_tio);

    tio.c_cc[VMIN]  = 0;			// non blocking
    tio.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tcsetattr(tty_fd, TCSANOW, &tio);

	running = 1;

	/* initialize the thread attribute */
	pthread_attr_init(&threadAttr);

	/* Set the stack size of the thread */
	pthread_attr_setstacksize(&threadAttr, 1024);

	/* Create the threads */
	pthread_create(&thser_id, &threadAttr, isr_ser_thread, NULL);

	/* Destroy the thread attributes */
	pthread_attr_destroy(&threadAttr);

    serHook = prcv;
}


void
close_mserial( void )
{
	tcsetattr(tty_fd, TCSANOW, &old_tio);
	close(tty_fd);
	running = 0;
}

