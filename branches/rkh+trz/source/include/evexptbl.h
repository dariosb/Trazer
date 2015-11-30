
#ifndef __EVEXPTBL_H__
#define __EVEXPTBL_H__

#include <string>
#include <vector>
#include "evtbl.h"
#include "tzparse.h"
#include "unittrz.h"

using namespace std;

typedef void(*CHEK_ARG_T)( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args );

typedef struct fmt_exp_st
{
    FMT_ID_T *p;
	CHEK_ARG_T chk;
} FMT_EXP_ST;


CHEK_ARG_T find_exp_chk_fun( unsigned int id );

void c_none( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args );
void c_sm_no_ao( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args ); 


#define  c_sminit       c_none
#define  c_smclrh       c_none
#define  c_smtrn        c_sm_no_ao
#define  c_smst         c_none
#define  c_smenst       c_none
#define  c_smexst       c_none
#define  c_smnenex      c_none
#define  c_smntrna      c_none
#define  c_smtsst       c_none  
#define  c_smevpr       c_none
#define  c_smnfnd       c_none
#define  c_smgf         c_none    
#define  c_smnf         c_none    
#define  c_smukst       c_none  
#define  c_smexhl       c_none  
#define  c_smextseg     c_none
#define  c_smexeact     c_none
#define  c_none         c_none

#endif
