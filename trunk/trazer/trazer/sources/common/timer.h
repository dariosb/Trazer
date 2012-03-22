/*
 * 	timer.h
 * 		Hardware dependant level
 * 		for timer interrupts	
 */

/*
 * 	init_hard:
 * 		Performs all hardware initialization
 */

#ifdef __cplusplus
extern "C" {
#endif

#define TIME_INTER	20	

void init_timer_hard( void (*timer_callback)( void ) );

/*
 * 	enable_timer_interrupts:
 * 		Enable timer interrupts
 */

void enable_timer_interrupts( void );

/*
 * 	disable_timer_interrupts:
 * 		Disable timer interrupts
 */

void disable_timer_interrupts( void );
#ifdef __cplusplus
}
#endif
