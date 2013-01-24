/*
 * utils.cpp
 */
#include "utils.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#define AUXILIAR_BUFFER_SIZE	1024
static char aux_buff[AUXILIAR_BUFFER_SIZE];
#define ENDOF_AUX_BUFF	(aux_buff+AUXILIAR_BUFFER_SIZE)

char *
str2lower( const char *src )
{
	char *dest;

	dest = aux_buff;

	memset( aux_buff, '\0', sizeof(aux_buff) );

	while( *src != '\0' && (dest < ENDOF_AUX_BUFF) )
		*dest++ = tolower(*src++);

	return aux_buff;
}

char *
str2upper( const char *src )
{
	char *dest;

	dest = aux_buff;

	memset( aux_buff, '\0', sizeof(aux_buff) );

	while( *src != '\0' && (dest < ENDOF_AUX_BUFF) )
		*dest++ = toupper(*src++);

	return aux_buff;
}


long
find_str_into_file(FILE* pFile, const char* str)
{
    //make sure we were passed a valid, if it isn't return -1
    if ((!pFile)||(!str))
        return -1;

    unsigned long ulFileSize=0;

    //get the size of the file
    fseek(pFile,0,SEEK_END);

    ulFileSize=ftell(pFile);

    fseek(pFile,0,SEEK_SET);

    //if the file is empty return -1
    if (!ulFileSize)
        return -1;

    //get the length of the string we're looking for, this is
    //the size the buffer will need to be
    unsigned long ulBufferSize=strlen(str);

    if (ulBufferSize>ulFileSize)
        return -1;

    //allocate the memory for the buffer
    char* lpBuffer=(char*)malloc(ulBufferSize);

    //if malloc() returned a null pointer (which probably means
    //there is not enough memory) then return -1
    if (!lpBuffer)
        return -1;

    unsigned long ulCurrentPosition=0;

    //this is where the actual searching will happen, what happens
    //here is we set the file pointer to the current position
    //is incrimented by one each pass, then we read the size of
    //the buffer into the buffer and compare it with the string
    //we're searching for, if the string is found we return the
    //position at which it is found
    while (ulCurrentPosition<ulFileSize-ulBufferSize)
    {
        //set the pointer to the current position
        fseek(pFile,ulCurrentPosition,SEEK_SET);

        //read ulBufferSize bytes from the file
        fread(lpBuffer,1,ulBufferSize,pFile);

        //if the data read matches the string we're looking for
        if (!memcmp(lpBuffer,str,ulBufferSize))
        {
            //free the buffer
            free(lpBuffer);

            //return the position the string was found at
            return ulCurrentPosition;
        }
        
        //incriment the current position by one
        ulCurrentPosition++;
    }

    //if we made it this far the string was not found in the file
    //so we free the buffer
    free(lpBuffer);

    //and return -1
    return -1;
}

static
void
chg_stdin(int dir)
{
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
 

static
int
_kbhit (void)
{
  struct timeval tv;
  fd_set rdfs;
 

  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}


int
test_key( void )
{
	int c;

  	chg_stdin( 1 );

	if( !_kbhit() )
	{
		chg_stdin( 0 );
		return 0;
	}

	c = getchar();

	chg_stdin( 0 );

	if( c == ESC )
		exit( EXIT_SUCCESS );

	return c;
}

