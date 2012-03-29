/*
 * debug.h
 */
#ifndef __DEBUG_H__
#define __DEBUG_H__

#define ACTIVE_DEBUG 1

#if ACTIVE_DEBUG

void dprintf( const char* fmt, ... );

#else

#define ldebug()

#endif

#endif
