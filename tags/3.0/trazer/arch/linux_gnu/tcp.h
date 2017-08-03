/*
 * TCP/IP communication for Win32
 */

#ifdef __cplusplus
extern "C" {
#endif

int tcpOpen( int portNum );
int tcpSend(unsigned char *buf, int size);
void tcpClose( void );
int tcpRead( unsigned char *buf, int size );

#ifdef __cplusplus
}
#endif
