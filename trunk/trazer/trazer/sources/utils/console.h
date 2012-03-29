/* 
 *		console.h
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	WHITE_ON_BLACK,		RED_ON_BLACK,		GREEN_ON_BLACK, 	YELLOW_ON_BLACK,
	BLUE_ON_BLACK,		MAGENTA_ON_BLACK,	CYAN_ON_BLACK,		BLACK_ON_GRAY,
	BLACK_ON_WHITE,		RED_ON_WHITE,		GREEN_ON_WHITE,		YELLOW_ON_WHITE,
	BLUE_ON_WHITE,		MAGENTA_ON_WHITE,	CYAN_ON_WHITE,		WHITE_ON_WHITE,
	NUM_ATTRIBS
};

void clrscr( void );
void gotoxy( int x, int y );
void setrgb( int color );
void clr2rgb( int color );

#ifdef __cplusplus
}
#endif

#endif
