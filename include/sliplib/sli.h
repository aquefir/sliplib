/****************************************************************************\
 *                                 sliplib.                                 *
 *                                                                          *
 *                     Copyright (C) 2021-2022 Aquefir.                     *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#ifndef INC_API__SLIPLIB_SLI_H
#define INC_API__SLIPLIB_SLI_H

#include <uni/types/int.h>

enum /* sli_record_flags */
{
	SLI_RECORD_FLAG_PURE,
	SLI_RECORD_FLAG_NOTHROW,
	SLI_RECORD_FLAG_NORETURN,
	SLI_RECORD_FLAG_NOALLOC,
	SLI_RECORD_FLAG_NEW,
	SLI_RECORD_FLAG_THREADSAFE,
	SLI_RECORD_FLAG_PREPROC,
	SLI_RECORD_FLAG_VALIDUSE,
	SLI_RECORD_FLAG_PACKED = SLI_RECORD_FLAG_PURE,
	SLI_RECORD_FLAG_ZEROFILL = SLI_RECORD_FLAG_NOTHROW,
};

enum /* sli_record_masks */
{
	SLI_RECORD_MASK_PURE = 1 << SLI_RECORD_FLAG_PURE,
	SLI_RECORD_MASK_NOTHROW = 1 << SLI_RECORD_FLAG_NOTHROW,
	SLI_RECORD_MASK_NORETURN = 1 << SLI_RECORD_FLAG_NORETURN,
	SLI_RECORD_MASK_NOALLOC = 1 << SLI_RECORD_FLAG_NOALLOC,
	SLI_RECORD_MASK_NEW = 1 << SLI_RECORD_FLAG_NEW,
	SLI_RECORD_MASK_THREADSAFE = 1 << SLI_RECORD_FLAG_THREADSAFE,
	SLI_RECORD_MASK_PREPROC = 1 << SLI_RECORD_FLAG_PREPROC,
	SLI_RECORD_MASK_VALIDUSE = 1 << SLI_RECORD_FLAG_VALIDUSE,
	SLI_RECORD_MASK_PACKED = 1 << SLI_RECORD_FLAG_PACKED,
	SLI_RECORD_MASK_ZEROFILL = 1 << SLI_RECORD_FLAG_ZEROFILL
};

enum /* sli_filetypes */
{
	SLI_FILETYPE_ENUM,
	SLI_FILETYPE_STRUCT,
	SLI_FILETYPE_SUB,
	SLI_FILETYPE_VAR,
	SLI_FILETYPE_CONST,
	SLI_FILETYPE_TYPE,
	SLI_FILETYPE_MEMBER,
	SLI_FILETYPE_LAW
};

enum
{
	SLI_FILETYPE_MASK_ENUM = 1 << SLI_FILETYPE_ENUM,
	SLI_FILETYPE_MASK_STRUCT = 1 << SLI_FILETYPE_STRUCT,
	SLI_FILETYPE_MASK_SUB = 1 << SLI_FILETYPE_SUB,
	SLI_FILETYPE_MASK_VAR = 1 << SLI_FILETYPE_VAR,
	SLI_FILETYPE_MASK_CONST = 1 << SLI_FILETYPE_CONST,
	SLI_FILETYPE_MASK_TYPE = 1 << SLI_FILETYPE_TYPE,
	SLI_FILETYPE_MASK_MEMBER = 1 << SLI_FILETYPE_MEMBER,
	SLI_FILETYPE_MASK_LAW = 1 << SLI_FILETYPE_LAW
};

struct sli_record;

struct sli_record
{
	const char * fname;
	const char * text;
	/* NULL if this is the root */
	struct sli_record * parent;
	/* NULL terminated array */
	struct sli_record ** children;
	u16 flags;
	u16 ftype;
	/* $0 == 0, $1 == 1, ... # (i.e. return value) == 0xFFFFFFFF */
	u32 param_n;
};

struct sli_record * sli_fromftext( const char * );

#endif /* INC_API__SLIPLIB_SLI_H */
