/*
 *	file: unittrz.h
 */

#ifndef __UNITTRZ_H__
#define __UNITTRZ_H__

#ifndef __TRAZER__

#ifdef __cplusplus
extern "C"{
#endif

#include "rkh.h"

void utrz_clean( void );
void utrz_insert( rui8_t id, rui8_t nargs, ... );
ri8_t utrz_check( rui8_t id, rui8_t nargs, ... );


#define RKH_TRC_CLEANUP()			utrz_clean()
#define UTRZEVT_INSERT( q, ... )	utrz_insert( CTE(tre)->id, q, __VA_ARGS__ )

#define RKH_TRC_IS_FINAL_STATE(ao,state)	RKH_TE_SM_STATE,2,(ao),(state)
#define TEST_ASSERT_TRUE(x)					utrz_check(x)

#ifdef __cplusplus
}
#endif

#else

#define RKH_TRC_CLEANUP();			
#define UTRZEVT_INSERT( q, ... );

#define RKH_TRC_IS_FINAL_STATE(ao,state);
#define TEST_ASSERT_TRUE(x);

#endif

#endif
