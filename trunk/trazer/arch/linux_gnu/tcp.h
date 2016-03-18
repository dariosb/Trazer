/*
 * TCP/IP communication for Win32
 */

#ifdef __cplusplus
extern "C" {
#endif

int tcpOpen( int portNum );
void tcpClose( void );
int tcpRead( unsigned char *buf, int size );

#ifdef __cplusplus
}
#endif