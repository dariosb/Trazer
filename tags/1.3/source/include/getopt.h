/*
 * 		getopt.h
 */

#ifdef __cplusplus
extern "C" {
#endif

extern int optind;
extern char *optarg;

int getopt( int argc, char **argv, char *opts );
char * get_next( char **argv );

#ifdef __cplusplus
}
#endif
