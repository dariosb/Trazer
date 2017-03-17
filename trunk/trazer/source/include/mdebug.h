/*
 * mdebug.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MDEBUG_H__
#define __MDEBUG_H__


#if ACTIVE_DEBUG

void dprintf( const char* fmt, ... );

#else

#define dprintf(...);

#endif

#endif

#ifdef __cplusplus
}
#endif
