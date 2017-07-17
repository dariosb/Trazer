/*
 * TCP/IP communication for Win32
 */

#include "tcp.h"
#include "trkhtrc.h"
#include "mydefs.h"
#include "utrzhal.h"
#include "tzlog.h"
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>

/*..........................................................................*/
SOCKET l_serverSock = INVALID_SOCKET;
SOCKET l_clentSock  = INVALID_SOCKET;

/*..........................................................................*/
int tcpOpen(int portNum)
{
    struct sockaddr_in local;
    ULONG ioctl_opt = 1;

    /* initialize Windows sockets */
    static WSADATA wsaData;
    int wsaErr = WSAStartup(MAKEWORD(2,0), &wsaData);

    if (wsaErr == SOCKET_ERROR)
	{
        printf("Windows Sockets cannot be initialized.\n"
               "The library reported error 0x%08X.", wsaErr);
        return 0;                                                  /* false */
    }

    l_serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); /* TCP socket */
    if (l_serverSock == INVALID_SOCKET)
	{
        printf("Server socket cannot be created.\n"
               "Windows socket error 0x%08X.",
               WSAGetLastError());
        return 0;                                                  /* false */
    }

    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons((unsigned short)portNum);

    /* bind() associates a local address and port combination with the
    * socket just created. This is most useful when the application is a
    * server that has a well-known port that clients know about in advance.
    */
    if (bind(l_serverSock, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
    {
        printf("Error by binding server socket.\n");
        return 0;                                                  /* false */
    }

    if (listen(l_serverSock, 1) == SOCKET_ERROR)
	{
        printf("Server socket listen failed.\n"
               "Windows socket error 0x%08X.",
               WSAGetLastError());
        return 0;                                                  /* false */
    }
    /* Set the socket to non-blocking mode. */
    if (ioctlsocket(l_serverSock, FIONBIO, &ioctl_opt) == SOCKET_ERROR)
	{
        printf("Socket configuration failed.\n"
               "Windows socket error 0x%08X.",
               WSAGetLastError());
        return 0;                                                  /* false */
    }
    return 1;                                                    /* success */
}


void
tcpClose(void)
{
    if (l_serverSock != INVALID_SOCKET)
        closesocket(l_serverSock);

    WSACleanup();
}


int
tcpRead(unsigned char *buf, int size)
{
    fd_set readSet;
    int nfound;
    int n;
    struct timeval delay;

    if ( _kbhit() )
	{
		if( _getch() == ESC )
          	return -1;                                         /* terminate */
    }

    delay.tv_sec = 0;
    delay.tv_usec = 200000;

    if (l_clentSock == INVALID_SOCKET)	   /* still waiting for the client? */
	{

        n = 0;                                   /* no data from the client */

        FD_ZERO(&readSet);
        FD_SET(l_serverSock, &readSet);

        nfound = select(0, &readSet, 0, 0, &delay);
        if (nfound == SOCKET_ERROR)
		{
            printf("Server socket select failed.\n"
                   "Windows socket error 0x%08X.",
                   WSAGetLastError());
            return -1;                                         /* terminate */
        }

        if (FD_ISSET(l_serverSock, &readSet))
		{
            struct sockaddr_in fromAddr;
            int fromLen = (int)sizeof(fromAddr);
            l_clentSock = accept(l_serverSock,
                                 (struct sockaddr *)&fromAddr, &fromLen);
            if (l_clentSock == INVALID_SOCKET)
			{
                printf("Server socket accept failed.\n"
                       "Windows socket error 0x%08X.",
                       WSAGetLastError());
                return -1;                                     /* terminate */
            }
            printf("Accepted connection from %s, port %d\n",
                   inet_ntoa(fromAddr.sin_addr),
                   (int)ntohs(fromAddr.sin_port));
        }
    }
    else
	{
        FD_ZERO(&readSet);
        FD_SET(l_clentSock, &readSet);

        nfound = select(0, &readSet, 0, 0, &delay);        /* selective blocking */
        if (nfound == SOCKET_ERROR)
		{
            printf("Client socket select failed.\n"
                   "Windows socket error 0x%08X.",
                   WSAGetLastError());
            return -1;                                         /* terminate */
        }
        if (FD_ISSET(l_clentSock, &readSet))
		{
            n = recv(l_clentSock, (char *)buf, (int)size, 0);
            if (n == SOCKET_ERROR)
			{
                printf("Client socket error.\n"
                       "Windows socket error 0x%08X.",
                       WSAGetLastError());
            }
            else if (n <= 0)                         /* the client hang up */
			{
                closesocket(l_clentSock);

                /* go back to waiting for a client, or a keypress
                * to terminate
                */
                l_clentSock = INVALID_SOCKET;
                return 0;                            /* no data from client */
            }
        }
        else
		{
            n = 0;                               /* no data from the client */
        }
    }

    return n;
}


int
tcpSend(unsigned char *buf, int size)
{
#if 0
    fd_set sendSet;
    int nfound;
    int n;
    struct timeval delay;
    if (l_clentSock == INVALID_SOCKET)	   /* still waiting for the client? */
	{

        n = 0;                                   /* no data from the client */

        FD_ZERO(&sendSet);
        FD_SET(l_serverSock, &sendSet);

        nfound = select(0, &sendSet, 0, 0, &delay);
        if (nfound == SOCKET_ERROR)
		{
            printf("Server socket select failed.\n"
                   "Windows socket error 0x%08X.",
                   WSAGetLastError());
            return -1;                                         /* terminate */
        }

        if (FD_ISSET(l_serverSock, &sendSet))
		{
            struct sockaddr_in fromAddr;
            int fromLen = (int)sizeof(fromAddr);
            l_clentSock = accept(l_serverSock,
                                 (struct sockaddr *)&fromAddr, &fromLen);
            if (l_clentSock == INVALID_SOCKET)
			{
                printf("Server socket accept failed.\n"
                       "Windows socket error 0x%08X.",
                       WSAGetLastError());
                return -1;                                     /* terminate */
            }
            printf("Accepted connection from %s, port %d\n",
                   inet_ntoa(fromAddr.sin_addr),
                   (int)ntohs(fromAddr.sin_port));
        }
    }
    else
	{
        FD_ZERO(&sendSet);
        FD_SET(l_clentSock, &sendSet);

        nfound = select(0, &sendSet, 0, 0, &delay);        /* selective blocking */
        if (nfound == SOCKET_ERROR)
		{
            printf("Client socket select failed.\n"
                   "Windows socket error 0x%08X.",
                   WSAGetLastError());
            return -1;                                         /* terminate */
        }
        if (FD_ISSET(l_clentSock, &sendSet))
		{
            n = send(l_clentSock, (char *)buf, (int)size, 0);
            if (n == SOCKET_ERROR)
			{
                printf("Client socket error.\n"
                       "Windows socket error 0x%08X.",
                       WSAGetLastError());
            }
            else if (n <= 0)                         /* the client hang up */
			{
                closesocket(l_clentSock);

                /* go back to waiting for a client, or a keypress
                * to terminate
                */
                l_clentSock = INVALID_SOCKET;
                return 0;                            /* no data from client */
            }
        }
        else
		{
            n = 0;                               /* no data from the client */
        }
    }

    return n;
#else
	int n;
	n = send(l_clentSock, (char *)buf, (int)size, 0);
	return n;
#endif
}


static char send_buff[2048];
static char pre_buff[2048];

void
tcp_printf( const char *fmt, ... )
{
	va_list args;

	va_start(args,fmt);
	vsprintf( send_buff, fmt, args );
    va_end(args);
	sprintf( pre_buff, UTRZACK_HEADER "%06d" UTRZACK_TAIL, strlen(send_buff)+1 );
    tcpSend( (unsigned char *)pre_buff, 15 );
    tcpSend( (unsigned char *)send_buff, strlen(send_buff)+1 );
}




