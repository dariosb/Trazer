/*
 * symbtbl.h
 */

#ifndef __SYMBTBL_H__
#define __SYMBTBL_H__

#include "tzpdef.h"

void clear_symtbl( void );
void add_to_symtbl( unsigned long obj, const char *sym );
const char * map_obj( unsigned long adr );
const char * search_in_objtbl( unsigned long obj  );
void post_fifo_symevt( unsigned long adr, TRZE_T e, unsigned long ts );
void post_lifo_symevt( unsigned long adr, TRZE_T e, unsigned long ts );
int remove_symevt_tstamp( unsigned long adr, TRZE_T e, unsigned long *pt );

#endif

