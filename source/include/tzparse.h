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

#ifdef __TRAZER__
#include "rkhtype.h"
#else
#include "rkh.h"
#endif
#include "mytypes.h"
#include <string>
#include <vector>
#include <deque>

using namespace std;


typedef unsigned long TRZTS_T;
typedef unsigned long TRZNB_T;
typedef unsigned long TRZNE_T;
typedef unsigned long TRZNT_T;
typedef unsigned long TRZES_T;

/*
 * 	Application dependent macros and typedefs
 */
#if 0
#define TRAZER_SIZEOF_SIG			RKH_SIZEOF_EVENT/8
#define TRAZER_SIZEOF_TSTAMP		RKH_TRC_SIZEOF_TSTAMP/8
#define TRAZER_SIZEOF_POINTER		RKH_TRC_SIZEOF_POINTER/8
#define TRAZER_SIZEOF_NTIMER		RKH_TIM_SIZEOF_NTIMER/8
#define TRAZER_SIZEOF_NBLOCK		RKH_MP_SIZEOF_NBLOCK/8
#define TRAZER_SIZEOF_NELEM			RKH_RQ_SIZEOF_NELEM/8
#define TRAZER_SIZEOF_ESIZE			RKH_SIZEOF_ESIZE/8
#define TRAZER_EN_NSEQ				RKH_TRC_EN_NSEQ
#define TRAZER_EN_CHK				RKH_TRC_EN_CHK
#define TRAZER_EN_TSTAMP			RKH_TRC_EN_TSTAMP

#endif

#if 0

#if TRAZER_SIZEOF_SIG == 1
	typedef unsigned char TRZE_T;
#elif TRAZER_SIZEOF_SIG == 2
	typedef unsigned short TRZE_T;
#elif TRAZER_SIZEOF_SIG == 4
	typedef unsigned long TRZE_T;
#else
	typedef unsigned char TRZE_T;
#endif

#else

typedef	unsigned long TRZE_T;

#endif


typedef char *(*HDLR_T)( const void *tre );

typedef struct vargs_t
{
	vector<char *>args;
}VARGS_T;

typedef struct tre_t
{
	unsigned char id;
	string group;
	string name;
	VARGS_T va;
	HDLR_T fmt_args;
} TRE_T;

typedef struct sym_evt_q
{
	unsigned long tstamp;
	unsigned long id;	
}SYM_EVT_Q;

typedef struct symobj_t
{
	unsigned long adr;
	string name;
	deque <SYM_EVT_Q> se_q;
} SYMOBJ_T;

typedef struct symsig_t
{
	TRZE_T sig;
	string name;
} SYMSIG_T;

typedef struct usrevt_t
{
	uchar uevt;
	string name;
} SYMUEVT_T;


#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void trazer_parse( rui8_t d );
EXTERNC void trazer_init( void );

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
char * h_sig2u8( const void *tre );
char * h_symevt( const void *tre );
char * h_sma_get( const void *tre );
char * h_sma_ff( const void *tre );
char * h_sma_lf( const void *tre );
char * h_sma_dch( const void *tre );
char * h_symnblk( const void *tre );
char * h_mp_init( const void *tre );
char * h_mp_get( const void *tre );
char * h_2symnused( const void *tre );
char * h_symnused( const void *tre );
char * h_rq_ffll( const void *tre );
char * h_symobj( const void *tre );
char * h_symst( const void *tre );
char * h_symsig( const void *tre );
char * h_symuevt( const void *tre );
char * h_tinit( const void *tre );
char * h_tstart( const void *tre );
char * h_tstop( const void *tre );
char * h_tout( const void *tre );
char * h_exact( const void *tre );
char * h_sync( const void *tre );
char * h_assert( const void *tre );
char * h_tcfg( const void *tre );
char * usr_fmt( const void *tre );


#endif