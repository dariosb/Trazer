/*
 * seqdiag.h
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SEQDIAG_H__
#define __SEQDIAG_H__

#include "mytypes.h"
#include "tzparse.h"

#define SEQDIAG_TEMPLATE_FILE	"sdiag_html.sk"
#define SEQDIAG_OUT_FNAME 		"sd%s.html"
#define SEQDIAG_FNAME 			"%s.sig"
#define SEQDIAG_FNAME_LENGTH	25
#define SEQDIAG_CLOSE_MARK		"\n}\n"
#define SEQDIAG_

#define MSC_TARGET_START		"---: TARGET START;\n"
#define MSC_OPTIONS_FILE		"msc.cfg"

#define SEQDIAG_TMP_INSERTION 	"// INSERT SEQUENCE HERE"
#define SEQDIAG_TMP_HEIGHT	  	"svg.setAttribute(\"height\", \""
#define SEQDIAG_TMP_HEIGHT_BASE	150


typedef struct
{
	ulong sobj;
	ulong tobj;
	TRZE_T e;
}EVENT_ST;

typedef struct
{
	ulong tobj;
	ulong smobj;
	ulong sobj;
}TMREVT_T;



void sdiag_async_evt( EVENT_ST *p );
void sdiag_state( ulong smobj, ulong stobj );
void sdiag_tmrevt( ulong t );
void sdiag_text( const char *s );
void sdiag_exec_act( ulong ao, ulong act );
void sdiag_sync( ulong f, ulong snr, ulong dest );

void seqdiag_init( void );

#endif

#ifdef __cplusplus
}
#endif

