/*
 * error.h
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ERROR_H__
#define __ERROR_H__

#include "messages.h"

void fatal_error( const char *fmt, ... );

#endif

#ifdef __cplusplus
}
#endif