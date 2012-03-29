/*
 *	console.cpp
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <windows.h>

#include "console.h"

using namespace std;

#define FG_INT		FOREGROUND_INTENSITY
#define FG_RED		FOREGROUND_RED
#define FG_GREEN	FOREGROUND_GREEN
#define FG_BLUE		FOREGROUND_BLUE
#define BG_INT		BACKGROUND_INTENSITY
#define BG_RED		BACKGROUND_RED
#define BG_GREEN	BACKGROUND_GREEN
#define BG_BLUE		BACKGROUND_BLUE

static const WORD attrib[ NUM_ATTRIBS ] =
{
	FG_INT | FG_RED | FG_GREEN | FG_BLUE,											/*	00:	White on Black		*/
	FG_INT | FG_RED,																/*	01: Red on Black		*/
	FG_INT | FG_GREEN,																/*	02:	Green on Black		*/
	FG_INT | FG_RED | FG_GREEN,														/*	03:	Yellow on Black		*/
	FG_INT | FG_BLUE,																/*	04:	Blue on Black		*/
	FG_INT | FG_RED | FG_BLUE,														/*	05:	Magenta on Black	*/
	FG_INT | FG_GREEN | FG_BLUE,													/*	06:	Cyan on Black		*/
	BG_INT | BG_INT,																/*	07:	Black on Gray		*/
	BG_INT | FG_INT | BG_RED | BG_GREEN | BG_BLUE,									/*	08:	Black on White		*/
	BG_INT | FG_INT | BG_RED | BG_GREEN | BG_BLUE | FG_RED,							/*	09:	Red on White		*/
	BG_INT | FG_INT | BG_RED | BG_GREEN | BG_BLUE | FG_GREEN,						/*	10: Green on White		*/
	BG_INT | FG_INT | BG_RED | BG_GREEN | BG_BLUE | FG_RED | FG_GREEN,				/*	11: Yellow on White		*/
	BG_INT | FG_INT | BG_RED | BG_GREEN | BG_BLUE | FG_BLUE,						/*	12: Blue on White		*/
	BG_INT | FG_INT | BG_RED | BG_GREEN | BG_BLUE | FG_RED | FG_BLUE,				/*	13: Magenta on White	*/
	BG_INT | FG_INT | BG_RED | BG_GREEN | BG_BLUE | FG_GREEN | FG_BLUE,				/*	14: Cyan on White		*/
	BG_INT | FG_INT | BG_RED | BG_GREEN | BG_BLUE | FG_RED | FG_GREEN | FG_BLUE		/*	15: White on White		*/
};

POINT screensize;

/*
 *	Clears the screen
 */
void
clrscr( void )
{
	COORD coordScreen = { 0, 0 }; 
	DWORD cCharsWritten; 
	CONSOLE_SCREEN_BUFFER_INFO csbi; 
	DWORD dwConSize; 
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
  
	GetConsoleScreenBufferInfo(hConsole, &csbi); 
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y; 
	screensize.x = csbi.dwSize.X;
	screensize.y = csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten); 
	GetConsoleScreenBufferInfo(hConsole, &csbi); 
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten); 
	SetConsoleCursorPosition(hConsole, coordScreen); 
}

/*
 *	Moves the cursor to x, y in console window
 *	ie x=left\right y=top\bottom
 */

void
gotoxy(int x, int y)
{
	COORD point;
	if((x < 0 || x > screensize.x) || (y < 0 || y > screensize.y))
		return;
	point.X = (SHORT)x; point.Y = (SHORT)y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}


/*
 * Set text and background colors
 */

void
setrgb(int color)
{
	if( color >= NUM_ATTRIBS )
		color = WHITE_ON_BLACK;
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), attrib[ color ] );
}

/*
 *	Clear screen to determined color
 */

void
clr2rgb( int color )
{
	setrgb( color );
	clrscr();
}


