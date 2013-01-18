#ifndef __SERIAL_H__
#define __SERIAL_H__

void init_serial( void (*prcv)(unsigned char byte) );
void close_mserial( void );

#endif
