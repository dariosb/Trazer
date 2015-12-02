/**
 * \cond
 *  --------------------------------------------------------------------------
 *
 *                                Framework RKH
 *                                -------------
 *
 * 	          State-machine framework for reactive embedded systems            
 * 	        
 * 	                    Copyright (C) 2010 Leandro Francucci.
 * 	        All rights reserved. Protected by international copyright laws.
 *
 *
 * 	RKH is free software: you can redistribute it and/or modify it under the 
 * 	terms of the GNU General Public License as published by the Free Software 
 * 	Foundation, either version 3 of the License, or (at your option) any 
 * 	later version.
 *
 *  RKH is distributed in the hope that it will be useful, but WITHOUT ANY 
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for 
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along 
 *  with RKH, see copying.txt file.
 *
 * 	Contact information:
 * 	RKH web site:	http://sourceforge.net/projects/rkh-reactivesys/
 * 	e-mail:			francuccilea@gmail.com
 *
 *  --------------------------------------------------------------------------
 *  File                     : tcptrc.c
 *	Last updated for version : v2.4.04
 *	By                       : DB
 *  --------------------------------------------------------------------------
 *  \endcond
 *
 * 	\file
 * 	\ingroup 	prt
 *
 * 	\brief 		Socket TCP/IP support for 80x86 OS win32
 */


#include "tcptrc.h"
#include "rkh.h"
#include <stdio.h>


#if RKH_CFG_TRC_EN == 1

int
tcp_trace_open( unsigned short port, char *srv_ip, SOCKET *ps )
{
	WORD wVersionRequested;
	WSADATA wsaData;
	SOCKADDR_IN target; /* Socket address information */
	SOCKET s;
	int err;

	/* --- INITIALIZATION ----------------------------------- */
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	*ps = INVALID_SOCKET;

	if( err != 0 )
	{
		printf("WSAStartup error %ld", WSAGetLastError() );
		WSACleanup();
		return -1;
	}
	/* ------------------------------------------------------ */
	
	/* ---- build address structure to bind to socket.-------- */
	target.sin_family = AF_INET; /* address family Internet */
	target.sin_port = htons( port ); /* Port to connect on */
	target.sin_addr.s_addr = inet_addr (srv_ip); /* Server IP */
	/* ------------------------------------------------------ */
	
	/* ---- create SOCKET-------------------------------------- */
	s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); /* Create socket */
	if( s == INVALID_SOCKET )
	{
		printf("socket error %ld" , WSAGetLastError() );
		WSACleanup();
		return -1; /* Couldn't create the socket */
	}  
	/* ------------------------------------------------------ */

	/* ---- try CONNECT ----------------------------------------- */
	if( connect( s, (SOCKADDR*)&target, sizeof(target) ) == SOCKET_ERROR )
	{
		printf("connect error %ld", WSAGetLastError() );
		WSACleanup();
		return -1; /* Couldn't connect */
	}
	*ps = s;
	return 0;
}


void
tcp_trace_send( SOCKET s, const char *buf, int len )
{
	send( s, buf, len, 0 ); 
}

#define UTRZACK_HEADER "UTRZACK:"
#define UTRZANSW_LENGTH_WIDTH	6
#define UTRZACK_TAIL ":"

#define UTRZACK_LENGTH	(strlen(UTRZACK_HEADER) + UTRZANSW_LENGTH_WIDTH + strlen(UTRZACK_TAIL))


int
tcp_trace_recv( SOCKET s, const char *buf, int len )
{
	int q, l;
	fd_set readSet;
       int nfound;
	       struct timeval delay;

#if 0
    while(!FD_ISSET(s, &readSet))
        n=0;
#endif
	
	 delay.tv_sec = 0;
     delay.tv_usec = 200000;

	 FD_ZERO(&readSet);
     FD_SET(s, &readSet);

     nfound = select(0, &readSet, 0, 0, &delay);        /* selective blocking */
     if (nfound == SOCKET_ERROR)
	 {
          printf("Client socket select failed.\n"
                 "Windows socket error 0x%08X.",
                 WSAGetLastError());
          return -1;                                         /* terminate */
      }
	
	while(!FD_ISSET(s, &readSet))
        q=0;
	l = UTRZACK_LENGTH;
	if( q = recv(s, (char *)buf, l, 0) != UTRZACK_LENGTH )
		return -1;

	l = atoi( buf+strlen(UTRZACK_HEADER) );
	
	if( q = recv(s, (char *)buf, (int)l, 0) < 0)
		return -1;

	return q;

	//return recv(s, (char *)buf, (int)len, 0);
#if 0
    if (n == SOCKET_ERROR)
	{
        printf("Client socket error.\n"
               "Windows socket error 0x%08X.",
                WSAGetLastError());
    }
    else if (n <= 0)                         /* the client hang up */
    {
         closesocket(s);
         return 0;                            /* no data from client */
     }
#endif
}

#include "unitrazer.h"
#include "tzparse.h"
int
utrz_recv( void *s, UtrzProcessOut *p )
{
	char c;
	int n;
	
	p->status = UT_PROC_BUSY;
	/* Blocking call with timeout */
	while( (n = recv((SOCKET)s, &c, sizeof(c), 0)) > 0 )
	{
		trazer_parse(c, p);
		if( p->status != UT_PROC_BUSY )
			return 1;
	}
	return -1;
}

void
tcp_trace_close( SOCKET s )
{
	closesocket( s );
	WSACleanup();
}

#endif
