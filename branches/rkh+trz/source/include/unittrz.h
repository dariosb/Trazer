/*
 *	file: unittrz.h
 */

#ifndef __UNITTRZ_H__
#define __UNITTRZ_H__

#include "rkh.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void unit_trace_clean( void );
EXTERNC void unit_trace_insert( UNIT_TRACE_EVT *tre );
EXTERNC rui8_t unit_trace_exist( UNIT_TRACE_EVT *tre );

#endif
