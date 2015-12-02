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

enum
{
    UTRZ_OUT_OF_ORDER_MSG,
    UTRZ_IGN_CALL_WO_EXP,
    UTRZ_VERIFY_FAIL,
    UTRZ_ARG_VAL
};

extern char *utrz_msgs[];

void utrz_resp( RKH_TE_ID_T e, rui32_t line, char *msg, int nargs, ... );
#define utrz_success()  				utrz_resp( RKH_TE_UT_SUCCESS, 0, "", 0 )

#define utrz_fail( l, m, n, ... )       utrz_resp( RKH_TE_UT_FAIL, (l), utrz_msgs[(m)], n, __VA_ARGS__ )


#ifdef __cplusplus
}
#endif
