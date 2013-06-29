/*
 * evtbl.cpp
 */


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

//#define SEQDIAG_OUT_FILE		"seqdiag.html"
static char foname[ SEQDIAG_FNAME_LENGTH ];
static char sdfname[ SEQDIAG_FNAME_LENGTH ];
static long seqdiag_len = 0;


using namespace std;


bool
fexists(const char *filename)
{
	ifstream ifile(filename);
	return ifile;
}

#if 0
void
seq_printf( const char *fmt, ... )
{
    va_list args;

    va_start(args,fmt);

	if( fseq != NULL )
	    vfprintf( fseq, fmt, args);

	fflush(fseq);

    va_end(args);
}
#endif


void
start_rawsdiag( const char *fname )
{
	FILE *fseq;

	++seqdiag_len = 0;

	if( ( fseq = fopen( fname, "w+" ) ) == NULL )
		fatal_error( "Can't open file %s\n", fname );

	fprintf( fseq, SEQDIAG_TEMPLATE );

	fclose( fseq );
}


void
insert_rawsdiag( const char *fname, const char *s )
{
	FILE *fseq;
	ofstream ft;
	int i, nl;
	static long p;


	if( !fexists( fname ) )
		return;

	++seqdiag_len;
	fseq = fopen( fname, "r+" );
	fseek( fseq, 0, SEEK_END );
	p = ftell( fseq );
	fseek( fseq, p-strlen(SEQDIAG_CLOSE_MARK), SEEK_SET );
	
	fprintf( fseq, "%s", s );
	fprintf( fseq, "%s", SEQDIAG_CLOSE_MARK );

	fclose( fseq );
}

#if 0
void
end_seqdiag( void )
{
	if( fseq != NULL ) 
	{
		//seq_printf("\n}\n");
		fclose( fseq );
	}
}
#endif



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


int
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
	return -1;
}


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


#if 0
static char seqlbuff[100]; 

static vector <string> seqdiag;

void
seqdiag_text_insert( ofstream *ft, const char *s )
{
	string cs;

	vector<string>::iterator i;

	sprintf( seqlbuff, "%s\n", s );

	cs.assign( seqlbuff );

	seqdiag.push_back( cs );

	//start_seqdiag( "trazer.diag" );

	for( i = seqdiag.begin(); i < seqdiag.end(); ++i )
	{
		ft->write( i->c_str(), strlen(i->c_str()) );
		//seq_printf( "\t%s", i->c_str() );
	}
	//end_seqdiag();
}


void
seqdiag_trn_insert( ofstream *ft, TRN_ST *p )
{
	char trbuff[ 100 ];

	sprintf( trbuff, "%s -> %s [label=\"%s\"];\n", 
				map_obj(p->sobj), map_obj(p->tobj), map_sig(p->e) );

	insert_rawsdiag( sdfname, trbuff );
	seqdiag_text_insert( ft, trbuff );
}

#endif

static char factor[20];
static int hfact;
static	int offs;

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
	sprintf( hbuff, "%d\");", (seqdiag_len*hfact) + SEQDIAG_TMP_HEIGHT_BASE );

	fout.write( hbuff, strlen(hbuff) );
	
	fcpy_from_line( &fout, SEQDIAG_TEMPLATE_FILE, loffh+1, 0 );

	fout.close();
}


void
add_to_trntbl( TRN_ST *p )
{
	char trbuff[ 100 ];

	sprintf( trbuff, "%s -> %s [label=\"%s\"];\n", 
				map_obj(p->sobj), map_obj(p->tobj), map_sig(p->e) );

	insert_rawsdiag( sdfname, trbuff );

	update_fout_html();
}



void
add_seqdiag_text( const char *s )
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
