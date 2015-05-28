/*
 * seqdiag.h
 */

#ifndef __SEQDIAG_H__
#define __SEQDIAG_H__

#include "mytypes.h"
#include "tzparse.h"

#define SEQDIAG_SEPARATOR_TEXT	"... RKH START ..."
#define SEQDIAG_TEMPLATE_FILE	"sdiag_html.sk"
#define SEQDIAG_OUT_FNAME 		"sd%s.html"
#define SEQDIAG_FNAME 			"%s.diag"
#define SEQDIAG_FNAME_LENGTH	25
#define SEQDIAG_CLOSE_MARK		"\n}\n"
#define SEQDIAG_TEMPLATE		"seqdiag\n{\n"	\
								SEQDIAG_CLOSE_MARK

#define SEQDIAG_TMP_INSERTION 	"// INSERT SEQUENCE HERE"
#define SEQDIAG_TMP_HEIGHT	  	"svg.setAttribute(\"height\", \""
#define SEQDIAG_TMP_HEIGHT_BASE	150


typedef struct
{
	ulong sobj;
	ulong tobj;
	TRZE_T e;
}TRN_ST;

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void add_to_trntbl( TRN_ST *p );
EXTERNC void add_seqdiag_text( const char *s );

EXTERNC void seqdiag_init( void );



#endif
