/*
 *	file: trazer.h
 *	Last updated for version: 2.0
 *	Date of the last update:  Mar 19, 2012
 *
 * 	Copyright (C) 2010 Leandro Francucci. All rights reserved.
 *
 * 	RKH is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 * 	(at your option) any later version.
 *
 *  RKH is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RKH, see copying.txt file.
 *
 * Contact information:
 * RKH web site:	http://
 * e-mail:			francuccilea@gmail.com
 */
#ifndef __TZPARSE_H__
#define __TZPARSE_H__

#include "rkhtype.h"

typedef char *(*HDLR_T)( const void *tre );

void trazer_parse( rkhui8_t d );
void trazer_init( void );

char * h_none( const void *tre );
char * h_epreg( const void *tre );
char * h_ae( const void *tre );
char * h_evt( const void *tre );
char * h_1sym( const void *tre );
char * h_2sym( const void *tre );
char * h_symtrn( const void *tre );
char * h_symrc( const void *tre );
char * h_symu8( const void *tre );
char * h_sym2u8( const void *tre );
char * h_symevt( const void *tre );
char * h_symnblk( const void *tre );
char * h_2symnused( const void *tre );
char * h_symnused( const void *tre );
char * h_symobj( const void *tre );
char * h_symsig( const void *tre );
char * h_2symntick( const void *tre );
char * h_symntick( const void *tre );


#endif
