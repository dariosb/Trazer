/*
 * 	mytypes.h
 */

#ifndef __MYTYPES_H__
#define __MYTYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char	uchar;
typedef signed char		schar;
typedef unsigned int	uint;
typedef unsigned short	ushort;
typedef unsigned long	ulong;

#ifdef __TINY_PROC__

typedef uchar			MUInt;
typedef schar			MInt;
typedef uint			bitf;

#else

typedef uint			MUInt;
typedef int				MInt;
typedef ushort			bitf;

#endif

#ifdef __cplusplus
}
#endif

#endif
