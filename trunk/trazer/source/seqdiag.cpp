/*
 * evtbl.cpp
 */

#include "error.h"
#include "options.h"
#include "seqdiag.h"
#include "symbtbl.h"
#include "sigtbl.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>
//#include <conio.h>

using namespace std;

FILE *fseq;

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

static const char* search = "// INSERT SEQUENCE HERE"; // search pattern

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
		if ( line.find(search, 0) != string::npos)
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
fcpy_from_line( ofstream *ft, const char *sname, int nl )
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
	}


	fs.close();
	return -1;
}

static char trbuff[ 100 ];

static vector <string> seqdiag;

void
seqdiag_insert( ofstream *ft, TRN_ST *p )
{
	string cs;

	vector<string>::iterator i;

	sprintf( trbuff, "%s -> %s [label=\"%s\"];\n", 
				map_obj(p->sobj), map_obj(p->tobj), map_sig(p->e) );

	cs.assign( trbuff );

	seqdiag.push_back( cs );

	for( i = seqdiag.begin(); i < seqdiag.end(); ++i )
		ft->write( i->c_str(), strlen(i->c_str()) );
}


#define SEQDIAG_TEMPLATE_FILE		"seqdiag_tmp.html"
#define SEQDIAG_OUT_FILE			"seqdiag.html"

void
add_to_trntbl( TRN_ST *p )
{
	int loff;
	ofstream fout;

	fout.open( SEQDIAG_OUT_FILE );

	loff = find_infile( SEQDIAG_TEMPLATE_FILE, search );

	fcpy_until_line( &fout, SEQDIAG_TEMPLATE_FILE, loff-1 );

	seqdiag_insert( &fout, p );

	fcpy_from_line( &fout, SEQDIAG_TEMPLATE_FILE, loff+1 );

	fout.close();
}



void
start_seqdiag( const char *fname )
{
}
