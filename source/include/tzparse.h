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
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TZPARSE_H__
#define __TZPARSE_H__

#ifdef __TRAZER__
#include "trkhtype.h"
#else
#include "rkh.h"
#endif

typedef unsigned long TRZTS_T;
typedef unsigned long TRZNB_T;
typedef unsigned long TRZNE_T;
typedef unsigned long TRZNT_T;
typedef unsigned long TRZES_T;

typedef	unsigned long TRZE_T;

extern void tzparser_init( void );
extern void tzparser_exec( rui8_t d );

#endif

#ifdef __cplusplus
}
#endif
