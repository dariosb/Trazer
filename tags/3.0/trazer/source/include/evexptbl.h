
#ifndef __EVEXPTBL_H__
#define __EVEXPTBL_H__

#include <string>
#include <vector>
#include "evtbl.h"
#include "unittrz.h"

typedef void(*CHEK_ARG_T)( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args );

typedef struct fmt_exp_st
{
    FMT_ID_T *p;
	CHEK_ARG_T chk;
} FMT_EXP_ST;


CHEK_ARG_T find_exp_chk_fun( unsigned int id );

void c_none( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args );
void c_sm_no_ao( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args ); 
void c_sm_exe_act( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args ); 
void c_sm_no_num_args ( UTRZ_EXPECT_EVT *pex, rui8_t nargs, va_list args ); 

#define  c_sminit       c_sm_no_ao
#define  c_smclrh       c_sm_no_ao
#define  c_smtrn        c_sm_no_ao
#define  c_smst         c_sm_no_ao
#define  c_smenst       c_sm_no_ao
#define  c_smexst       c_sm_no_ao
#define  c_smnenex      c_sm_no_num_args
#define  c_smntrna      c_sm_no_num_args
#define  c_smtsst       c_sm_no_ao  
#define  c_smevpr       c_sm_no_num_args
#define  c_smnfnd       c_sm_no_ao
#define  c_smgf         c_sm_no_ao    
#define  c_smnf         c_sm_no_ao    
#define  c_smukst       c_sm_no_ao  
#define  c_smexhl       c_sm_no_ao  
#define  c_smextseg     c_sm_no_ao
#define  c_smexeact     c_sm_exe_act
#define  c_smdch        c_sm_no_ao

#endif
