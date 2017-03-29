/*
 * rtimbtbl.h
 */

#ifndef __RTIMTBL_H__
#define __RTIMTBL_H__

#include "tzpdef.h"

void clear_rt_tbl( void );
void disable_rtime( void );

void post_fifo_symevt( unsigned long ao, TRZE_T e, unsigned long ts,
                                unsigned long *nseq );
void post_lifo_symevt( unsigned long ao, TRZE_T e, unsigned long ts,
                                unsigned long *nseq );
int remove_symevt_tstamp( unsigned long ao, TRZE_T e, 
                                unsigned long *pt, unsigned long *nseq );

#endif
