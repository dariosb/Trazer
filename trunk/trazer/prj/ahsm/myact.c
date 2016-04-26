/*
 *	myact.c
 */


#include <stdio.h>
#include "rkh.h"
#include "my.h"
#include "unittrz.h"


#define CMY( s )		(( MYSM_T* )(s))


/*
 *	Defines HSM init function
 */

void 
my_init( const void *sma )
{
	CMY( sma )->x = CMY( sma )->y = 0;
	RKH_TRC_CLEANUP();
}


/*
 *	Defines state entry functions
 */


void
set_x_1( const struct RKH_SMA_T *sma )
{
	CMY( sma )->x = 1;
}


void
set_x_2( const struct RKH_SMA_T *sma )
{
	CMY( sma )->x = 2;
}


void
set_x_3( const struct RKH_SMA_T *sma )
{
	CMY( sma )->x = 3;
}


void
set_y_0( const struct RKH_SMA_T *sma )
{
	CMY( sma )->y = 0;
}


/*
 *	Defines state exit functions
 */

void 
dummy_exit( const struct RKH_SMA_T *sma )
{
	(void)sma;
}


/*
 *	Defines state preprocessor functions
 */



/*
 *	Defines transition action functions
 */


void
set_x1( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)pe;
	CMY( sma )->x = 1;
}


void
set_y_2( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)pe;
	CMY( sma )->y = 2;
}


void
set_y_1( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)pe;
	CMY( sma )->y = 1;
}


void
dummy_act( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)sma;
	(void)pe;
}


void
show_data( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)sma;
	(void)pe;
}


void 
terminate( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)sma;
	(void)pe;
	rkh_fwk_exit();
}


void
unit_clean( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)sma;
	(void)pe;
	RKH_TRC_CLEANUP();
	printf("\n\tunit_clean\n\n");
}


void
unit_test( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	ri8_t r;

	(void)sma;
	(void)pe;

	r = TEST_ASSERT_TRUE(RKH_TRC_IS_FINAL_STATE( my, &S111 ));

	printf("\n\tunit_test: %s\n\n", r < 0 ? "FAIL":"OK");
}


/*
 *	Defines branch's guards
 */


rbool_t
y_0( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)pe;
	return CMY( sma )->y == 0 ? RKH_GTRUE : RKH_GFALSE;
}


rbool_t
y_1( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)pe;
	return CMY( sma )->y == 1 ? RKH_GTRUE : RKH_GFALSE;
}


rbool_t
y_2( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)pe;
	return CMY( sma )->y == 2 ? RKH_GTRUE : RKH_GFALSE;
}


rbool_t
x1( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)pe;
	return CMY( sma )->x == 1 ? RKH_GTRUE : RKH_GFALSE;
}


rbool_t
x2_or_x3( const struct RKH_SMA_T *sma, RKH_EVT_T *pe )
{
	(void)pe;
	return CMY( sma )->x == 2 || CMY( sma )->x == 3 ? RKH_GTRUE : RKH_GFALSE;
}
