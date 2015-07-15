/*
 * 	rerror.h
 * 		Run errors
 */

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif
extern int debugging;

int debug( const char *fmt, ... );
int fatal( const char *fmt, ... );
int inform_error( const char *fmt, ... );

#ifdef __cplusplus
}
#endif
