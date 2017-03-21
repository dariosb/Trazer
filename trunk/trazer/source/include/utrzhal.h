/*
 * TCP/IP communication for Win32
 */

#ifndef __UTRZHAL_H__
#define __UTRZHAL_H__

#include "trkhtrc.h"
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

void rkh_trc_flush(void);

#ifdef __cplusplus
}
#endif

#endif
