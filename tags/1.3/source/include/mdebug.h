/*
 * debug.h
 */
#ifndef __MDEBUG_H__
#define __MDEBUG_H__

#define ACTIVE_DEBUG 1

#if ACTIVE_DEBUG

void dprintf( const char* fmt, ... );

#else

#define dprintf()

#endif

#endif
