/*
 * TCP/IP communication for Win32
 */

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

void utrz_resp( RKH_TE_ID_T e, rui32_t line, const char *msg, int nargs, ... );
#define utrz_success()  				                \
            utrz_resp( RKH_TE_UT_SUCCESS, 0, "", 0, 0 )

#define utrz_fail( l, m, n, ... )                       \
            utrz_resp( RKH_TE_UT_FAIL, (l), m, n, __VA_ARGS__ )

#define utrzIgnArg_fail(l,e)                                    \
            utrz_fail( (l), "IgnoreArg called before "          \
                            "Expect on event ", 3,              \
                            "'", (e), "'." )

#define utrzVerify_fail(l,e)                                            \
            utrz_fail( (l), "Event '",  2,                              \
                            (e),                                        \
                            "' ocurred less times than expected."       \
                    )

#define utrzEvtExpect_fail(l,r,e)                               \
            utrz_fail( (l), "Out of order Trace event.", 5,     \
                            " ocurred: '", (r),                 \
                            "' expected: '", (e),               \
                            "'." )

#define utrz_ArgExpect_fail(l,e,at,x )                          \
            utrz_fail( (l), "Event '",   6,                              \
                       (e),                                              \
                       "' ocurred with unexpected value for argument '", \
                       (at), "' expected value='", (x), "'." )

#define utrzMoreEvtThanExpect(l,e)                              \
            utrz_fail( (l), "Event '",   2,                     \
                       (e),                                     \
                       "' ocurred more times than expected." )

#ifdef __cplusplus
}
#endif
