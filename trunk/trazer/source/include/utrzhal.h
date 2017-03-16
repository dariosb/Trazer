/*
 * TCP/IP communication for Win32
 */

#ifndef __UTRZHAL_H__
#define __UTRZHAL_H__

#include "rkhtrc.h"
#ifdef __cplusplus
extern "C" {
#endif

enum
{
    UTRZ_OUT_OF_ORDER_MSG,
    UTRZ_IGN_CALL_WO_EXP,
    UTRZ_VERIFY_FAIL,
    UTRZ_ARG_VAL
};


#define UTRZACK_HEADER "UTRZACK:"
#define UTRZANSW_LENGTH_WIDTH	6
#define UTRZACK_TAIL ":"


extern char *utrz_msgs[];

void utrz_hal_start( void );
void utrz_hal_stop( void );

void utrz_resp( RKH_TE_ID_T e, rui32_t line, const char *msg, int nargs, ... );

#ifdef __cplusplus
}
#endif

#endif
