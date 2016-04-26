/*
 * sigtbl.h
 */

#ifndef __SIGTBL_H__
#define __SIGTBL_H__

#include "tzparse.h"
#include "seqdiag.h"

void add_to_sigtbl( TRZE_T sig, const char *name );
const char * map_sig( TRZE_T sig );
const char * search_in_sigtbl(  TRZE_T sig  );


void tmrtbl_init( ulong tobj, ulong sobj );
void tmrtbl_start( ulong tobj, ulong smobj );
uint tmrtbl_find( ulong tobj, TMREVT_T *p );
uint tmrtbl_find_sig( ulong sobj );
	
#endif

