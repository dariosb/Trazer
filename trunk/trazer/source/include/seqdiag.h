/*
 * seqdiag.h
 */

#ifndef __SEQDIAG_H__
#define __SEQDIAG_H__

#include "mytypes.h"
#include "tzparse.h"

#define SEQDIAG_SEPARATOR_TEXT	"=== SESSION START ==="
#define SEQDIAG_TEMPLATE_FILE	"sdiag_html.sk"
#define SEQDIAG_OUT_FILE		"seqdiag.html"

#define SEQDIAG_TMP_INSERTION 	"// INSERT SEQUENCE HERE"
#define SEQDIAG_TMP_HEIGHT	  	"svg.setAttribute(\"height\", \""
#define SEQDIAG_TMP_HEIGHT_BASE	150


typedef struct
{
	ulong sobj;
	ulong tobj;
	TRZE_T e;
}TRN_ST;

void add_to_trntbl( TRN_ST *p );
void add_seqdiag_text( char *s );

void start_seqdiag( const char *fname );

#endif
