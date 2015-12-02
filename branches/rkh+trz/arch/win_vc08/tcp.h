/*
 * TCP/IP communication for Win32
 */

#include "rkhtrc.h"
#ifdef __cplusplus
extern "C" {
#endif


int tcpOpen( int portNum );
void tcpClose( void );
int tcpRead( unsigned char *buf, int size );

void tcp_printf( const char *fmt, ... );
void utrz_tcp_resp( RKH_TE_ID_T e, rui32_t line, char *msg );
#define utrz_tcp_success()  		utrz_tcp_resp( RKH_TE_UT_SUCCESS, 0, NULL )

#ifdef __cplusplus
}
#endif
