#ifndef __LANG_H__
#define __LANG_H__

/*
 *	lang.h
 */
#ifdef __cplusplus
extern "C" {
#endif

enum
{
	ENGLISH, SPANISH,
	NUM_LANG
};

extern unsigned lang;

void set_language( unsigned language );

#ifdef __cplusplus
}
#endif

#endif
