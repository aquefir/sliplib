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
	const char * path;
};

ptri countrecs( char ** lines )
{
	ptri i, ret;

	for(i = 0, ret = 0; lines[i] != NULL; ++i)
	{
		ret += lines[i][0] == '\t' ? 0 : 1;
	}

	return ret;
}

int allwspace( char * line )
{
	ptri i;

	for(i = 0; line[i] != '\0'; ++i)
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
	ptri rec_i = 0;
	int started = 0;

	for(i = 0; lines[i] != NULL; ++i)
	{
		if(lines[i][0] == '\0' || allwspace( lines[i] ))
		{
			continue;
		}

		if(lines[i][0] == '\t')
		{
			if(!started)
			{
				uni_free( ret );
				uni_die( );

				/* UNREACHABLE */
				return NULL;
			}

			uni_str_app( ret[rec_i], lines[i] );
			uni_str_appch( ret[rec_i], '\n');
		}
		else
		{
			struct uni_str * nu;

			started = 1;
			nu = uni_str_init( (const char *)lines[i] );
			uni_str_appch( nu, '\n' );

			rec_i++;

			if( rec_i >= recs )
			{
				/* oh no, we ran into more records than we expected */
				ptri i2;

				/* clean up and die */
				for(i2 = 0; i2 < recs; ++i2)
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
		char ** r = uni_alloc( sizeof( char *) * (recs + 1 ) );

		for(i = 0; i < recs; ++i)
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

struct record_unfini mkrecord( char * in )
{
	struct record_unfini ret;

	if( in == NULL || in[0] == '\t')
	{
		uni_die( );

		/* UNREACHABLE */
		return ret;
	}

	/* WIP */
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
