/*
 * seqdiag.h
 */

#ifndef __SEQDIAG_H__
#define __SEQDIAG_H__

#include "mytypes.h"
#include "tzparse.h"


typedef struct
{
	ulong sobj;
	ulong tobj;
	TRZE_T e;
}TRN_ST;

void add_to_trntbl( TRN_ST *p );

void start_seqdiag( const char *fname );

#endif
