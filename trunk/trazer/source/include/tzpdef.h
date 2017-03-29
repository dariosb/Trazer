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
#ifndef __TZPDEF_H__
#define __TZPDEF_H__

#ifdef __TRAZER__
#include "trkhtype.h"
#include "trkhtrc.h"
#else
#include "rkh.h"
#endif
#include "mytypes.h"
#include "tzparse.h"
#include <string>
#include <vector>
#include <deque>

using namespace std;


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

typedef struct trg_t
{
	RKH_TG_T grp;
	const char * name;
} TRG_T;

typedef struct sym_evt_q
{
	unsigned long tstamp;
	unsigned long id;	
	unsigned long nseq;
}SYM_EVT_Q;

typedef struct rtime_t
{
	unsigned long ao;
	unsigned long seq_counter;
	deque <SYM_EVT_Q> se_q;
} RTIME_T;

typedef struct symobj_t
{
	unsigned long adr;
	string name;
} SYMOBJ_T;

typedef struct epid_t
{
	unsigned long id;
	string name;
} EPID_T;

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


extern const char * atype_str[];

const TRE_T * find_exp_trevt( unsigned int id );

char * h_none( const void *tre );
char * h_epreg( const void *tre );
char * h_ae( const void *tre );
char * h_gc( const void *tre );
char * h_gcr( const void *tre );
char * h_1sym( const void *tre );
char * h_2sym( const void *tre );
char * h_symtrn( const void *tre );
char * h_symu8( const void *tre );
char * h_2u8( const void *tre );
char * h_sym2u8( const void *tre );
char * h_sig2u8( const void *tre );
char * h_1sig( const void *tre );
char * h_symevt( const void *tre );
char * h_sma_act( const void *tre );
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
char * h_symepid( const void *tre );
char * h_symst( const void *tre );
char * h_symsig( const void *tre );
char * h_symuevt( const void *tre );
char * h_tinit( const void *tre );
char * h_tstart( const void *tre );
char * h_tstop( const void *tre );
char * h_tout( const void *tre );
char * h_exact( const void *tre );
char * h_exact_no_ao( const void *tre );
char * h_sync( const void *tre );
char * h_assert( const void *tre );
char * h_tcfg( const void *tre );
char * usr_fmt( const void *tre );
char * h_ExpAnyArg( const void *tre );
char * h_Expect( const void *tre );
char * h_IgnGroup( const void *tre );
char * h_IgnEvt( const void *tre );
char * h_IgnArg( const void *tre );
char * h_utInit( const void *tre );
char * h_utCleanup( const void *tre );
char * h_utVerify( const void *tre );

#endif
