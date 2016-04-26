/*
 * rkhutrz.h
 */


#ifndef __RKHUTRZ_H__
#define __RKHUTRZ_H__

#ifndef __TRAZER__

#ifdef __cplusplus
extern "C"{
#endif

#include "rkh.h"

#define sm_init_expect(initState)		utrz_expect( __LINE__, 			\
											RKH_TE_SM_INIT, 1, initState )
#define sm_init_ignore()				utrz_ignore( RKH_TE_SM_INIT )
#define sm_init_ignoreArg_initState()	utrz_ignoreArg( __LINE__,		\
											RKH_TE_SM_INIT, 1 )


#ifdef __cplusplus
}
#endif

#else

#endif

#endif
