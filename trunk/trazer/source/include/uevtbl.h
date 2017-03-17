/*
 * uevttbl.h
 */

#ifndef __UEVTTBL_H__
#define __UEVTTBL_H__

#include "tzpdef.h"
#include "mytypes.h"

void add_to_uevttbl( uchar uevt, const char *name );
const char * map_uevt( uchar uevt );

#endif

