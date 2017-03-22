/*
 * evtbl.cpp
 */


#if (__UNITRAZER_LIB__ == 0)    

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdio>

#include "error.h"
#include "options.h"
#include "seqdiag.h"
#include "symbtbl.h"
#include "sigtbl.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

static char foname[ SEQDIAG_FNAME_LENGTH ];
static char sdfname[ SEQDIAG_FNAME_LENGTH ];
static long seqdiag_len = 0;


using namespace std;


static
bool
fexists(const char *filename)
{
	ifstream ifile(filename);
	return (ifile != NULL);
}


static
int
find_infile( const char *fname, const char *ps )
{
	ifstream ft;
	string line;
	int nl = 0; 

	ft.open( fname );

	while( !ft.eof() )
	{
		getline( ft, line );
		if ( line.find(ps, 0) != string::npos)
		{	
			ft.close();
			return nl;
		}
		++nl;
	}
	ft.close();
	return -1;
}

static
void
fcpy_file( ofstream *ft, const char *sname )
{
	ifstream fs;
	string line;
	int i; 

	fs.open( sname );

	for( i = 0; !fs.eof(); ++i )
	{
		getline( fs, line );
		ft->write( line.c_str(), line.size()  );
		ft->write( "\n", 1 );
	}

	fs.close();
}

static
void
fcpy_until_line( ofstream *ft, const char *sname, int nl )
{
	ifstream fs;
	string line;
	int i; 

	fs.open( sname );

	for( i = 0; !fs.eof() && i <= nl; ++i )
	{
		getline( fs, line );
		ft->write( line.c_str(), line.size()  );
	}

	fs.close();
}

static
int
fcpy_from_line( ofstream *ft, const char *sname, int nl, int qty )
{
	ifstream fs;
	string line;
	int i; 

	fs.open( sname );

	for( i = 0; !fs.eof() && i < nl; ++i )
	{
		getline( fs, line );
	}

	while( !fs.eof() )
	{
		getline( fs, line );
		ft->write( line.c_str(), line.size()  );

		if( qty && (--qty == 0) )
			break;
	}


	fs.close();
	return -1;
}


static char factor[20];
static int hfact;
static uint offs;

static
int
get_seqdiag_high_factor( const char *fname )
{
	ifstream ft;
	string line;
	int nl = 0; 


	ft.open( fname );

	while( !ft.eof() )
	{
		getline( ft, line );
		if ( (offs = line.find( SEQDIAG_TMP_HEIGHT, 0)) != string::npos)
		{	
			line.copy( factor, sizeof(factor), offs+strlen(SEQDIAG_TMP_HEIGHT));
			hfact = atoi( factor ) / 5;
			ft.close();
			return nl;
		}
		++nl;
	}
	ft.close();
	return -1;
}

static
void
update_fout_html( void )
{
#if 0
	ofstream fout;
	int loff, loffh;
	char hbuff[50];

	if( !fexists(SEQDIAG_TEMPLATE_FILE) )
		return;

	fout.open( foname );

	loff = find_infile( SEQDIAG_TEMPLATE_FILE, SEQDIAG_TMP_INSERTION );

	fcpy_until_line( &fout, SEQDIAG_TEMPLATE_FILE, loff-1 );

	fcpy_from_line( &fout, sdfname, 0, 0 );

	loffh = get_seqdiag_high_factor( SEQDIAG_TEMPLATE_FILE );

	fcpy_from_line( &fout, SEQDIAG_TEMPLATE_FILE, loff+1, (loffh-loff-1) );

	fout.write( SEQDIAG_TMP_HEIGHT, strlen(SEQDIAG_TMP_HEIGHT) );
	sprintf( hbuff, "%d\");", (int)((seqdiag_len*hfact) + SEQDIAG_TMP_HEIGHT_BASE) );

	fout.write( hbuff, strlen(hbuff) );
	
	fcpy_from_line( &fout, SEQDIAG_TEMPLATE_FILE, loffh+1, 0 );

	fout.close();
#endif
/*	char cmd[100];

	sprintf(cmd, "msc-gen -T svg %s", sdfname );
	system(cmd);*/
}

static
void
start_rawsdiag( const char *fname )
{
	ofstream fseq;

	++seqdiag_len = 0;
	
	if( !fexists(MSC_OPTIONS_FILE))
		return;

	fseq.open( fname );

	fcpy_file( &fseq, MSC_OPTIONS_FILE );

	fseq.close();
}

static
void
insert_rawsdiag( const char *fname, const char *s )
{
	FILE *fseq;
	ofstream ft;

	if( !fexists( fname ) )
		return;

	++seqdiag_len;
	fseq = fopen( fname, "r+" );
	fseek( fseq, 0, SEEK_END );
	fprintf( fseq, "%s", s );

	fclose( fseq );
}


void
sdiag_async_evt( EVENT_ST *p )
{
	char trbuff[ 100 ];

	if( tmrtbl_find_sig( p->e) )
		return;

	sprintf( trbuff, "%s->%s: %s[arrow.type=line];\n",
				map_obj(p->sobj), map_obj(p->tobj), map_sig(p->e) );

	insert_rawsdiag( sdfname, trbuff );

	update_fout_html();
}


void
sdiag_state( ulong smobj, ulong stobj )
{
	char trbuff[ 100 ];

	sprintf( trbuff, "%s--%s: %s;\n", 
				map_obj(smobj), map_obj(smobj), map_obj(stobj) );

	insert_rawsdiag( sdfname, trbuff );

	update_fout_html();
}


void
sdiag_tmrevt( ulong t )
{
	char trbuff[ 100 ];
	TMREVT_T tmr;

	if( !tmrtbl_find( t, &tmr ) )
		return;

	sprintf( trbuff, "%s->%s: \\-tm(%s)"
				"[arrow.type=solid,arrow.starttype=empty_diamond];\n",
				map_obj(tmr.smobj), map_obj(tmr.smobj), map_sig(tmr.sobj) );

	insert_rawsdiag( sdfname, trbuff );

	update_fout_html();
}


void
sdiag_exec_act( ulong ao, ulong act )
{
	char trbuff[ 100 ];

	sprintf( trbuff, "%s->%s: \\-%s()[arrow.type=solid];\n",
				map_obj(ao), map_obj(ao), map_obj(act));

	insert_rawsdiag( sdfname, trbuff );

	update_fout_html();	
	
}


void
sdiag_sync( ulong f, ulong snr, ulong dest )
{
	char trbuff[ 100 ];

	sprintf( trbuff, "%s->%s: \\-%s()[arrow.type=solid];\n",
				map_obj(snr), map_obj(dest), map_obj(f));

	insert_rawsdiag( sdfname, trbuff );

	update_fout_html();	
	
}

void
sdiag_text( const char *s )
{
	insert_rawsdiag( sdfname, s );

	update_fout_html();
}


void
seqdiag_init( void )
{
	char tstamp[20];
	time_t t = time(0);
	struct tm *tm;

	tm = gmtime(&t);
	strftime(tstamp, sizeof(tstamp), "%y%m%d%H%M%S", tm);
	sprintf(foname, SEQDIAG_OUT_FNAME, tstamp);
	sprintf( sdfname, SEQDIAG_FNAME, tstamp );

	start_rawsdiag( sdfname );
}

#endif
