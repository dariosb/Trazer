/*
 * sigtbl.h
 */

#ifndef __SIGTBL_H__
#define __SIGTBL_H__

#include "tzparse.h"

void add_to_sigtbl( TRZE_T sig, const char *name );
const char * map_sig( TRZE_T sig );

#endif

