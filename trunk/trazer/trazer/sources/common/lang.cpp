/*
 * 	lang.cpp
 */

#include "lang.h"

extern unsigned lang = ENGLISH;

void
set_language( unsigned language )
{
	lang = language >= NUM_LANG ? ENGLISH : language;
}



