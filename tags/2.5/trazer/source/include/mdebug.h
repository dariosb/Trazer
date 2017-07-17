/*
 * mdebug.h
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MDEBUG_H__
#define __MDEBUG_H__


#if ACTIVE_DEBUG

void mdprintf( const char* fmt, ... );

#else

#define mdprintf(...);

#endif

#endif

#ifdef __cplusplus
}
#endif
