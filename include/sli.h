/****************************************************************************\
 *                                 sli-tool                                 *
 *                                                                          *
 *                         Copyright © 2021 Aquefir                         *
 *                       Released under BSD-2-Clause.                       *
\****************************************************************************/

#ifndef INC_API__SLITOOL_SLI_H
#define INC_API__SLITOOL_SLI_H

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
	SLI_RECORD_MASK_PACKED = 1 << SLI_RECORD_FLAG_PACKED,
	SLI_RECORD_MASK_ZEROFILL = 1 << SLI_RECORD_FLAG_ZEROFILL,
	SLI_RECORD_MASK_VALIDUSE = 1 << SLI_RECORD_FLAG_VALIDUSE
};

enum /* sli_filetypes */
{
	SLI_FILETYPE_ENUM,
	SLI_FILETYPE_STRUCT,
	SLI_FILETYPE_SUB,
	SLI_FILETYPE_VAR,
	SLI_FILETYPE_CONST
};

enum
{
	SLI_FILETYPE_MASK_ENUM = 1 << SLI_FILETYPE_ENUM,
	SLI_FILETYPE_MASK_STRUCT = 1 << SLI_FILETYPE_STRUCT,
	SLI_FILETYPE_MASK_SUB = 1 << SLI_FILETYPE_SUB,
	SLI_FILETYPE_MASK_VAR = 1 << SLI_FILETYPE_VAR,
	SLI_FILETYPE_MASK_CONST = 1 << SLI_FILETYPE_CONST,
};

struct sli_record
{
	/* NULL terminated array */
	const char ** dirs;
	const char * file;
	const char * text;
	/* NOTE: for return val, param_n == 0xFFFF */
	u16 param_n;
	u8 type;
	u8 flags;
};

struct sli_file
{
	struct sli_record * records;
	ptri records_sz;
};

#endif /* INC_API__SLITOOL_SLI_H */
