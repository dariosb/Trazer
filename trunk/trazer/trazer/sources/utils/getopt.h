/*
 * 		getopt.h
 */

#ifdef __cplusplus
extern "C" {
#endif

extern int optind;
extern char *optarg;

int getopt( int argc, char **argv, char *opts );

#ifdef __cplusplus
}
#endif