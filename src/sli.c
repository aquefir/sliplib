/****************************************************************************\
 *                                 sliplib.                                 *
 *                                                                          *
 *                     Copyright (C) 2021-2022 Aquefir.                     *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#include "sli.h"

#include <uni/err.h>
#include <uni/memory.h>
#include <uni/str.h>

#define ISSPACE( X ) ( ( ( X ) >= 9 && ( X ) <= 13 ) || ( X ) == 32 )

struct record_unfini
{
	struct sli_record rec;
	char * path;
	char * suffix;
	char * text;
	u32 attribs;
};

ptri countrecs( char ** lines )
{
	ptri i, ret;

	for( i = 0, ret = 0; lines[i] != NULL; ++i )
	{
		ret += lines[i][0] == '\t' ? 0 : 1;
	}

	return ret;
}

int allwspace( char * line )
{
	ptri i;

	for( i = 0; line[i] != '\0'; ++i )
	{
		if( !ISSPACE( line[i] ) )
		{
			return 0;
		}
	}

	return 1;
}

char ** lines2recs( char ** lines, ptri recs )
{
	struct uni_str ** ret = uni_alloc( sizeof( struct uni_str * ) * recs );
	ptri i;
	ptri rec_i  = 0;
	int started = 0;

	for( i = 0; lines[i] != NULL; ++i )
	{
		if( lines[i][0] == '\0' || allwspace( lines[i] ) )
		{
			continue;
		}

		if( lines[i][0] == '\t' )
		{
			if( !started )
			{
				uni_free( ret );
				uni_die( );

				/* UNREACHABLE */
				return NULL;
			}

			uni_str_app( ret[rec_i], lines[i] );
			uni_str_appch( ret[rec_i], '\n' );
		}
		else
		{
			struct uni_str * nu;

			started = 1;
			nu      = uni_str_init( (const char *)lines[i] );
			uni_str_appch( nu, '\n' );

			rec_i++;

			if( rec_i >= recs )
			{
				/* oh no, we ran into more records than we
				 * expected */
				ptri i2;

				/* clean up and die */
				for( i2 = 0; i2 < recs; ++i2 )
				{
					uni_str_fini( ret[i2] );
				}

				uni_free( ret );
				uni_die( );

				/* UNREACHABLE */
				return NULL;
			}

			ret[rec_i] = nu;
		}
	}

	/* make the dynamic strings into regular ones to return to caller */
	{
		char ** r = uni_alloc( sizeof( char * ) * ( recs + 1 ) );

		for( i = 0; i < recs; ++i )
		{
			/* this allocates new C string */
			r[i] = uni_str_make( ret[i] );
			/* no sense in keeping these, free them as we go */
			uni_str_fini( ret[i] );
		}

		/* the array is NULL terminated */
		r[recs] = NULL;
		/* free the old array of dynamic strings */
		uni_free( ret );

		return r;
	}
}

enum
{
	ATTRIB_PURE,
	ATTRIB_NOTHROW,
	ATTRIB_NORETURN,
	ATTRIB_NOALLOC,
	ATTRIB_NEW,
	ATTRIB_THREADSAFE,
	ATTRIB_PACKED,
	ATTRIB_ZEROFILL,
	MAX_ATTRIB
};

enum
{
	ATTRIB_MASK_PURE       = 1 << ATTRIB_PURE,
	ATTRIB_MASK_NOTHROW    = 1 << ATTRIB_NOTHROW,
	ATTRIB_MASK_NORETURN   = 1 << ATTRIB_NORETURN,
	ATTRIB_MASK_NOALLOC    = 1 << ATTRIB_NOALLOC,
	ATTRIB_MASK_NEW        = 1 << ATTRIB_NEW,
	ATTRIB_MASK_THREADSAFE = 1 << ATTRIB_THREADSAFE,
	ATTRIB_MASK_PACKED     = 1 << ATTRIB_PACKED,
	ATTRIB_MASK_ZEROFILL   = 1 << ATTRIB_ZEROFILL
};

const u8 attrib_masks[MAX_ATTRIB] = { ATTRIB_MASK_PURE,
	ATTRIB_MASK_NOTHROW,
	ATTRIB_MASK_NORETURN,
	ATTRIB_MASK_NOALLOC,
	ATTRIB_MASK_NEW,
	ATTRIB_MASK_THREADSAFE,
	ATTRIB_MASK_PACKED,
	ATTRIB_MASK_ZEROFILL };

const u32 attrib2flag[MAX_ATTRIB] = { SLI_RECORD_FLAG_PURE,
	SLI_RECORD_FLAG_NOTHROW,
	SLI_RECORD_FLAG_NORETURN,
	SLI_RECORD_FLAG_NOALLOC,
	SLI_RECORD_FLAG_NEW,
	SLI_RECORD_FLAG_THREADSAFE,
	SLI_RECORD_FLAG_PACKED,
	SLI_RECORD_FLAG_ZEROFILL };

const u32 attrib2flagmask[MAX_ATTRIB] = { SLI_RECORD_MASK_PURE,
	SLI_RECORD_MASK_NOTHROW,
	SLI_RECORD_MASK_NORETURN,
	SLI_RECORD_MASK_NOALLOC,
	SLI_RECORD_MASK_NEW,
	SLI_RECORD_MASK_THREADSAFE,
	SLI_RECORD_MASK_PACKED,
	SLI_RECORD_MASK_ZEROFILL };

const char * const flagstrs[MAX_ATTRIB + 1] = { "pure",
	"nothrow",
	"noreturn",
	"noalloc",
	"new",
	"threadsafe",
	"packed",
	"zerofill",
	NULL };

enum
{
	STATE_BEGIN,
	STATE_ATTRIB,
	STATE_BETWEEN,
	STATE_END,
	STATE_ERROR,
	MAX_STATE
};

void parse_begin( char c, ptri * i, unsigned * state, struct uni_str * out )
{
	switch( c )
	{
	case '[':
		*state = STATE_ATTRIB;
		/* fall through */
	case ' ':
	case '\t':
	case '\v':
	case '\r':
		*i += 1;
		break;
	default:
		*state = STATE_ERROR;
		break;
	}
}

void parse_attrib( char c, ptri * i, unsigned * state, struct uni_str * out )
{
	switch( c )
	{
	case ' ':
	case '\t':
	case '\v':
	case '\r':
		*state = STATE_ERROR;
		break;
	case ']':
		*state = STATE_BETWEEN;
		/* internal representation is comma-separated */
		c = ',';
		/* fall through */
	default:
		uni_str_appch( out, c );
		*i += 1;
		break;
	}
}

void parse_between( char c, ptri * i, unsigned * state, struct uni_str * out )
{
	switch( c )
	{
	case '[':
		*state = STATE_ATTRIB;
		*i += 1;
		break;
	case ' ':
	case '\t':
	case '\v':
	case '\r':
		*state = STATE_END;
		*i += 1;
		break;
	default:
		*state = STATE_ERROR;
		break;
	}
}

void parse_end( char c, ptri * i, unsigned * state, struct uni_str * out )
{
	switch( c )
	{
	case ' ':
	case '\t':
	case '\v':
	case '\r':
		*i += 1;
		break;
	default:
		*state = STATE_ERROR;
		break;
	}
}

void parse_error( char c, ptri * i, unsigned * state, struct uni_str * out )
{
	*i += 1;
}

typedef void ( *PFN_parse_step )( char, ptri *, unsigned *, struct uni_str * );

const PFN_parse_step steps[MAX_STATE] = {
	parse_begin, parse_attrib, parse_between, parse_end, parse_error };

char ** split_attribs( char * line )
{
	struct uni_str * out;
	ptri i;
	const ptri linesz = uni_strlen( line );
	unsigned state    = STATE_BEGIN;

	out = uni_str_init( NULL );

	for( i = 0; i < linesz; ++i )
	{
		const char c = line[i];

		steps[state]( c, &i, &state, out );
	}

	{
		const char * const tmp = (const char *)uni_str_make( out );
		char ** const ret      = uni_strsplit( tmp, ",", 0 );

		uni_free( tmp );
		uni_str_fini( out );

		return ret;
	}
}

u32 getflags( char * line ) { u32 ret = 0; }

struct record_unfini mkrecord( char * in )
{
	struct record_unfini ret;
	char ** lines;
	char ** tmp;
	ptri lines_ct, i;

	if( in == NULL || in[0] == '\t' )
	{
		uni_die( );

		/* UNREACHABLE */
		return ret;
	}

	lines = uni_strsplit( in, "\n", 0 );

	for( lines_ct = 0; lines[lines_ct] != NULL; ++lines_ct )
		;

	tmp = lines_ct <= 1 ? NULL : uni_strsplit( lines[0], ":", 2 );

	if( tmp == NULL )
	{
		uni_die( );

		/* UNREACHABLE */
		return ret;
	}

	ret.path   = uni_strdup( tmp[0] );
	ret.suffix = uni_strdup( tmp[1] );

	/* get rid of the split string */
	for( i = 0; tmp[i] != NULL; ++i )
	{
		uni_free( tmp[i] );
	}

	uni_free( tmp );

	ret.rec.fname    = NULL;
	ret.rec.parent   = NULL;
	ret.rec.children = NULL;
}

struct sli_record * sli_fromftext( const char * in )
{
	struct sli_record * ret;
	char ** lines;

	if( in == NULL )
	{
		uni_die( );

		/* UNREACHABLE */
		return ret;
	}

	lines = uni_strsplit( in, "\n", 0 );
}
