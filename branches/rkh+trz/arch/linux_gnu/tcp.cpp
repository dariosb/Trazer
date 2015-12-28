/*
 * TCP/IP communication for Win32
 */

#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h>

#include "tcp.h"
#include "mydefs.h"
#include "utils.h"
#include "tzlog.h"
#include <stdio.h>
#include <stdlib.h>   
#include <string.h>

/*..........................................................................*/

#define INVALID_SOCKET -1
int l_clentSock = INVALID_SOCKET;
int l_serverSock = INVALID_SOCKET;

/*..........................................................................*/
int tcpOpen(int portNum)
{
    int sock;                        /* socket to create */
    struct sockaddr_in echoServAddr; /* Local address */
	int yes=1;
	/* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
        printf("Socket cannot be initialized.\n");
        return 0;                                                  /* false */
    }
      
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(portNum);              /* Local port */
	
	/*reuse port */
	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) < 0) 
	{
        printf("Socket port %d busy.\n", portNum );
        return 0;                                                  /* false */
    }

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
    {
        printf("Error by binding server socket.\n");
        return 0;                                                  /* false */
    }

    /* Mark the socket so it will listen for incoming connections */
    if (listen(sock, 1) < 0)
	{
        printf("Server socket listen failed.\n");
        return 0;                                                  /* false */
    }

	l_serverSock = sock;

    return 1;
}


int
tcpSend(unsigned char *buf, int size)
{
    write( l_clentSock, buf, size );
    return size;
}


void
tcpClose(void)
{
    if (l_serverSock != INVALID_SOCKET)
        close(l_serverSock);

}


int
tcpRead(unsigned char *buf, int size)
{
    fd_set readSet;
    int n;
    struct timeval delay;

    if ( test_key() == ESC )
       	return -1;                          /* terminate */

    delay.tv_sec = 0;
    delay.tv_usec = 200;

    if (l_clentSock == INVALID_SOCKET)	   /* still waiting for the client? */
	{

        n = 0;                                   /* no data from the client */

        FD_ZERO(&readSet);
        FD_SET(l_serverSock, &readSet);

        if (select(0, &readSet, 0, 0, &delay) < 0)
		{
            printf("Server socket select failed.\n\n");
            return -1;                                         /* terminate */
        }

        if (FD_ISSET(l_serverSock, &readSet))
		{
            struct sockaddr_in fromAddr;
            socklen_t fromLen = (socklen_t)sizeof(fromAddr);
            l_clentSock = accept(l_serverSock,
                                 (struct sockaddr *)&fromAddr, 
                                 &fromLen);
            if (l_clentSock == INVALID_SOCKET)
			{
                printf("Server socket accept failed.\n\n");
                return -1;                                     /* terminate */
            }
            printf("Accepted connection from %s, port %d\n\n",
                   inet_ntoa(fromAddr.sin_addr),
                   (int)ntohs(fromAddr.sin_port));
        }
    }
    else
	{
        FD_ZERO(&readSet);
        FD_SET(l_clentSock, &readSet);

        
        if (select(0, &readSet, 0, 0, &delay) < 0)			/* selective blocking */
		{
            printf("Client socket select failed.\n\n" );
            return -1;                                         /* terminate */
        }
        if (FD_ISSET(l_clentSock, &readSet))
		{
            n = recv(l_clentSock, (char *)buf, (int)size, 0);

            if (n <= 0)                         /* the client hang up */
			{
                close(l_clentSock);

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
