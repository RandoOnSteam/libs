/**
*	@file diff.h
*
*	######  ### ####### #######
*	#     #  #  #       #
*	#     #  #  #       #
*	#     #  #  #####   #####
*	#     #  #  #       #
*	#     #  #  #       #
*	######  ### #       #
*
*	Modern diff engine that can be compiled on legacy compilers
*	including old System 6 compilers.
*
*	###Instructions###
*
*	Define DIFF_IMPLEMENTATION in one source file for the implementation
*
*	###Options###
*	- DIFF_DISABLE_OUTPUT disables Diff_Output.
*	-- Diff_Output relies on libc for win32/unix/osx, limiting compatability.
*	- #define the MemoryStream_XXX methods yourself for custom input
*
*	@version 0.4
*
*	Version History
*
*	0.4		Made reading/writing from memory the default
*
*	0.3		Changes to make it usable outside of personal projects
*
*	0.2		Rename Diff_Construct/Destruct to Diff_Create/Destroy
*
*   0.1		Initial
*
*	@copyright ALTERNATIVE A - 0BSD (www.opensource.org/license/0BSD)
*
*	Copyright (c) 2024 Ryan Norton
*
*	Permission to use, copy, modify, and/or distribute this software for
*	any purpose with or without fee is hereby granted.
*
*	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
*	WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
*	OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE
*	FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY
*	DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
*	AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
*	OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*
*	@copyright ALTERNATIVE B - Public Domain (www.unlicense.org)
*
*	This is free and unencumbered software released into the public domain.
*
*	Anyone is free to copy, modify, publish, use, compile, sell, or distribute
*	this software, either in source code form or as a compiled binary, for any
*	purpose, commercial or non-commercial, and by any means.
*
*	In jurisdictions that recognize copyright laws, the author or authors of
*	this software dedicate any and all copyright interest in the software to
*	the public domain. We make this dedication for the benefit of the public
*	at large and to the detriment of our heirs and successors. We intend this
*	dedication to be an overt act of relinquishment in perpetuity of all
*	present and future rights to this software under copyright law.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
*	ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
*	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef __DIFF_H__
#define __DIFF_H__
#include <stddef.h> /* size_t */
#ifdef __cplusplus
extern "C" {
#endif
#define DIFF_COMPARE_IGNORECASE			1
#define DIFF_COMPARE_IGNOREWHITESPACE	2
#define DIFF_COMPARE_BYWORD				4
#define DIFF_COMPARE_BYCHAR				8
#define DIFF_COMPARE_NOUTF8				512
#define DIFF_OUTPUT_CONTEXT				16
#define DIFF_OUTPUT_ED					32
#define DIFF_OUTPUT_UNIFIED				64
#define DIFF_OUTPUT_WINDOWSNEWLINES		128
#define DIFF_OUTPUT_MACNEWLINES			256
typedef size_t linenum_t;
typedef size_t filesize_t;
#define LINENUM_MAX (size_t)(~0)
#define LINELENGTH_MAX (size_t)(~0)
typedef int diffbool;

#define STREAM_FROMCURRENT	0
#define STREAM_FROMSTART	1
#define STREAM_FROMEND		2


typedef struct MEMORYSTREAM
{
	unsigned char* mData;
	size_t mSize;
	size_t mPosition;
} MEMORYSTREAM;
void MemoryStream_Construct(MEMORYSTREAM* pThis, void* data, size_t size);
size_t MemoryStream_Read(MEMORYSTREAM* pThis, void* p, size_t n);
size_t MemoryStream_Write(MEMORYSTREAM* pThis, const void* p, size_t n);
size_t MemoryStream_Seek(MEMORYSTREAM* pThis, ptrdiff_t ulWhere, long lMode);
size_t MemoryStream_Tell(MEMORYSTREAM* pThis);

#define DIFFBLOCK_ADD 1
#define DIFFBLOCK_REMOVE 2
#define DIFFBLOCK_CHANGE 4

typedef struct DIFFLINE
{
	size_t val;
	size_t len;
	size_t pos;
} DIFFLINE;
typedef struct DIFFLINESPAN
{
	DIFFLINE* data;
	size_t length;
} DIFFLINESPAN;

typedef struct DIFFBLOCK
{
	linenum_t leftStart;		/* left stream start position */
	linenum_t leftEnd;			/* left stream end position */
	linenum_t rightStart;		/* right stream start position */
	linenum_t rightEnd;			/* right stream end position */
	size_t type;				/* removed/added type (DIFFBLOCK_XXX) */
} DIFFBLOCK;

typedef struct DIFFPOINT {
	size_t y;			/* furthest y axis point */
	size_t score;		/* insert/remove/change influencing score */
	size_t diag;		/* array index of nearest DIFFDIAG */
	diffbool hasY;		/* whether y DIFFPOINT member is valid */
	diffbool hasDiag;	/* whether diag DIFFPOINT member is valid */
} DIFFPOINT;
typedef struct DIFFPOINTSPAN
{
	DIFFPOINT* data;
	size_t length;
} DIFFPOINTSPAN;

typedef struct DIFF
{
	MEMORYSTREAM* streams[2];		/* input streams */
	int flags;						/* input/output flags (DIFF_XXX) */
	linenum_t contextlinecount;		/* number of lines for CONTEXT etc. diffs */
	DIFFLINESPAN lines[2];			/* Hash/line data for total file  */
	diffbool reverse;				/* Compare() lines[] were reversed */
	DIFFPOINTSPAN fpN, fpM;			/* DIFFPOINTs for each file */
	struct DIFFDIAGVECTOR* diags;	/* DIFFDIAG vector */
} DIFF;
void Diff_Create(DIFF* pThis, MEMORYSTREAM* leftStream, MEMORYSTREAM* rightStream);
void Diff_Destroy(DIFF* pThis);
diffbool Diff_BuildSeekVectors(DIFF* pThis);
diffbool Diff_Compare(DIFF* pThis, int flags);
void Diff_FillBlocks(DIFF* pThis, DIFFBLOCK* blocks, size_t* outCount);
diffbool Diff_GetLine(DIFF* pThis, int nStream,
					  char** buffer, size_t* bufsize, size_t* outLen);
void Diff_Output(DIFF* pThis, MEMORYSTREAM* outStream, int flags,
				 size_t contextlinecount,
				 const char* fileAname, const char* fileBname,
				 DIFFBLOCK* blocks, size_t numBlocks);
void Diff_SeekToLine(DIFF* pThis, int nStream, linenum_t nLine);

unsigned short CRC16(const char* buffer, size_t len);
#if defined(WIKISERVER_VERSION)
	size_t DiffFormatDate(WSDATE* pThis, const char* f, char* s);
#endif
#ifdef __cplusplus
}
class Diff : public DIFF
{
public:
	Diff()
	{ Diff_Create(this, NULL, NULL); }
	Diff(MEMORYSTREAM* leftStream, MEMORYSTREAM* rightStream)
	{ Diff_Create(this, leftStream, rightStream); }
	~Diff()
	{ Diff_Destroy(this); }
	void Create(MEMORYSTREAM* leftStream, MEMORYSTREAM* rightStream)
	{ this->streams[0] = leftStream; this->streams[1] = rightStream; }
	void Destroy()
	{ Diff_Destroy(this); Diff_Create(this, NULL, NULL); }
	void BuildSeekVectors()
	{ Diff_BuildSeekVectors(this); }
	void Compare(int nOps)
	{ Diff_Compare(this, nOps); }
	void FillBlocks(DIFFBLOCK* blocks, size_t* outCount)
	{ Diff_FillBlocks(this, blocks, outCount); }
	int GetLine(int nStream, char** buffer, size_t* bufsize, size_t* outlen)
	{ return Diff_GetLine(this, nStream, buffer, bufsize, outlen); }
	void Output(MEMORYSTREAM* rOutStream, int nOps,
				DIFFBLOCK* blocks, size_t numBlocks,
				const char* szFileName1, const char* szFileName2,
				linenum_t incontextlinecount = 0)
	{ Diff_Output(this, rOutStream, nOps, incontextlinecount,
				  szFileName1, szFileName2, blocks, numBlocks); }
	void SeekToLine(int nStream, linenum_t nLine)
	{ Diff_SeekToLine(this, nStream, nLine); }
};
#endif /* __cplusplus */
#endif /* __DIFF_H__ */
#if defined(DIFF_IMPLEMENTATION)
/**
*	@file diff.c
*
*	Implementation of the diff engine.
*
*	Written in ANSI C (C89) for compiler compatability.
*	@defgroup diff Diff
*	Outputs the difference between two streams.
*
*	Diff obtains the differences between two streams (referred to as "files"
*	in some comments here) using the 198X O(NP) algorithm by Myers. Typical
*	usage is:
*
*	Diff_Create()->Diff_Compare()->Diff_FillBlocks()->Diff_Destroy()
*
*	This allows you to fill in an array of DIFFBLOCK which contain line
*	number positions where there were differnces along with a type that
*	specifies whether it was an addition, removal, or change. Generally
*	you'll want to call Diff_FillBlocks twice, the first with a NULL size
*	so that you'll get the complete size of blocks you'll need.
*
*	If you want to output the diff changes to a stream typically you'll
*	want to do the following:
*
*	Diff_Create()->Diff_Compare()->Diff_Output()->Diff_Destroy()
*
*	Once Diff_BuildSeekVectors() is called the diff object contains a
*	vector that has file positions for each line. Sometimes you might also
*	want to seek to lines using these vectors rather than reading in
*	each file yourself, and Diff_SeekToLine() provides an efficient
*	way to do so.
*
*	::More Technical Information::
*
*	"An Algorithm for Differential File Comparison" by Hunt & Mcilory
*	https://www.cs.dartmouth.edu/~doug/diff.pdf
*
*	"An O(ND) Difference Algorithm and Its Variations" by Eugene W. Myers
*	http://www.xmailserver.org/diff2.pdf
*
*	"An O(NP) Sequence Comparison Algorithm" by Gene Myers et al.
*	https://publications.mpi-cbg.de/Wu_1990_6334.pdf
*
*	"A Formal Investigation of Diff3" by Benjamin C. Pierce et al.
*	https://www.cis.upenn.edu/~bcpierce/papers/diff3-short.pdf
*
*	IOCCC 2015 Winner by SirWumpus (public domain)
*	https://github.com/SirWumpus/ioccc-npdif/blob/master/spoiler/npdif.c
*
*	cdiff from the comp.sources.misc usenet group archive (public domain)
*
*	@author Ryan Norton
*	@date 2/23/2024
*	@{
**/
#if !defined(WIKISERVER_VERSION)
	#define DIFFSELFCONTAINED
#endif
/*#include "system.h"
#ifndef PRECOMPILEDHEADERS
	#include "diff.h"
#endif*/
#include <ctype.h>
#include <limits.h> /* [U]INT_MAX, for CodeWarrior/10.2 project builder */
#if defined(_DEBUG) && !defined(NOCRUNTIME)
	#define DIFFDEBUG /* for compatability with cdiff source */
#endif
#define diffuint16 unsigned short
#define diffint16 short

#ifdef DIFFSELFCONTAINED
	#include <stdlib.h> /* malloc, realloc, free, size_t */
	#include <stdio.h>
	#include <string.h>
	#include <assert.h> /* assert */
	/*#include "diff.h"*/
	#undef STREAM_FROMSTART
	#undef STREAM_FROMCURRENT
	#define STREAM_FROMSTART	SEEK_SET
	#define STREAM_FROMCURRENT	SEEK_CUR

	#if defined(_MSC_VER)
		#pragma warning(disable:4996) /* "unsafe" crt functions */
	#endif
#else /* not self contained */
	#if defined(DIFFDEBUG)
	#include <stdio.h>
	#endif
#endif /* DIFFSELFCONTAINED */
#if !defined(Stream_Seek)
	#define Stream_Seek(stream, pos, flag) \
		MemoryStream_Seek(stream, (long)pos, flag)
	#define Stream_Read(stream, data, count) \
		MemoryStream_Read(stream, data, (long)count)
	#define Stream_Write(stream, data, count) \
		MemoryStream_Write(stream, data, (long)count)
	#define Stream_Tell(stream) MemoryStream_Tell(stream)
#endif
#ifndef WIKISERVER_VERSION
	#ifdef _WIN32
		#define WSPATHSEPARATOR "\\"
		#define WSPATHSEPARATORCHAR '\\'
	#else
		#define WSPATHSEPARATOR "/"
		#define WSPATHSEPARATORCHAR '/'
	#endif
	#define WSASSERT(x) assert(x)
	#ifndef WSMemoryCopy
		#define WSMemoryCopy memcpy
		#define WSMemoryMove memmove
	#endif
	#include <assert.h>
	#if !defined(TRUE)
		#define TRUE 1
		#define FALSE 0
	#endif
	#if !defined(DIFF_DISABLE_OUTPUT)
		#if !defined(TARGET_API_MAC_CARBON) \
			|| !defined(__MWERKS__) || __MWERKS__ > 0x2400
		#define WSBITFIELD(x) : x
		#else
		#define WSBITFIELD(x)
		#endif
		typedef struct WSDATE
		{
			unsigned short year WSBITFIELD(14);		/* 1900 + year */
			unsigned char month WSBITFIELD(4);		/* 0-11 */
			unsigned char dayOfWeek WSBITFIELD(3);	/* 0-6 */
			unsigned char dayOfMonth WSBITFIELD(5);	/* 1-31 */
			unsigned char hour WSBITFIELD(5);		/* 0-23 */
			unsigned char minute WSBITFIELD(6);		/* 0-59 */
			unsigned char second WSBITFIELD(6);		/* 0-59 */
		} WSDATE;
		#if !defined(WSALLOC)
			#define WSALLOC(type, count) (type*)malloc(sizeof(type) * (count))
			#define	WSFREE(x) free((void*)(x))
		#endif
		#include <sys/stat.h> /* stat */
		#include <time.h> /* localtime et al. */
		#if defined(_WIN32)
			#include <Windows.h> /* for micro/nanosec filemodtime precision */
		#endif
		size_t WSFileModificationTimeWithFracSecs(
			const char* s, size_t* fracsecs)
		{
			struct stat rec;
			stat(s, &rec);
			if(fracsecs)
			{
			#if defined(STAT_HAVE_NSEC) && STAT_HAVE_NSEC
				*fracsecs = rec.st_mtime_nsec;
			#elif defined(__MACH__)
				*fracsecs = rec.st_mtimespec.tv_nsec;
			#elif !defined(_WIN32)
				*fracsecs = rec.st_mtim.tv_nsec;
			#else
				HANDLE hFile = CreateFileA(s, GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				ULARGE_INTEGER li;
				FILETIME ft;
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetFileTime(hFile, NULL, NULL, &ft);
					li.LowPart = ft.dwLowDateTime;
					li.HighPart = ft.dwHighDateTime;
					*fracsecs = li.QuadPart % 10000000;
					CloseHandle(hFile);
				}
				else
					*fracsecs = 0;
			#endif
			}
			return rec.st_mtime;
		}
		time_t WSTimeGetLocalValue()
		{ return time(NULL); }
		time_t WSTimeGetGMTValue()
		{ time_t nowtime = time(NULL); return mktime(gmtime(&nowtime)); }
		void WSTime_GetDate(time_t time, WSDATE* date)
		{
			struct tm* t = localtime(&time);
			date->second = t->tm_sec;
			date->minute = t->tm_min;
			date->hour = t->tm_hour;
			date->dayOfMonth = t->tm_mday;
			date->dayOfWeek = t->tm_wday;
			date->month = t->tm_mon;
			date->year = t->tm_year;
		}
		int UDigitToString(char* buf, size_t udigit)
		{ return sprintf(buf, "%zu", udigit); }

		char* DiffStringDupWithEnd(const char* start, const char* end)
		{
			char* text = (char*) malloc(sizeof(char) * (end - start + 1));
			if(text)
			{
				WSMemoryCopy(text, start, (end - start) * sizeof(char));
				text[end - start] = 0;
			}
			return text;
		}
		int DiffDigitToString(char* sz, size_t n)
		{
			return sprintf(sz, "%zu", n);
		}
	#endif /* !DIFF_DISABLE_OUTPUT */
#else /* not WIKISERVER_VERSION; Below is WIKISERVER_VERSION */
	#define DiffStringDupWithEnd StringDupWithEnd
	#define DiffDigitToString DigitToString
	#ifdef strlen
		#undef strlen
	#endif
	#define strlen CStrLength

	#if defined(NOCRUNTIME)
		#define memset WSMemoryFill
	#endif
#endif /* end if not WIKISERVER_VERSION */
#if __cplusplus >= 199711L
	#define register /* deprecated */
#endif

/*****************************************************************************/
/**********************************[Utils]************************************/
/*****************************************************************************/
#ifndef WSSWAP
	#define WSSWAP(DATATYPE, x, y) { DATATYPE tx = x; x = y; y = tx; }
	#define WSMIN(x, y) (x < y ? x : y)
	#define WSREVERSE(DATATYPE, SIZETYPE, x, first, last)\
	{\
		SIZETYPE iCur, iLast, iEnd;\
		for (iCur = first, iLast = last, iEnd = (iLast + 1) / 2; \
				iCur != iEnd; ++iCur)\
			WSSWAP(DATATYPE, x[iCur], x[iLast - iCur]);\
	}
#endif /* #ifdef WSSWAP */
/*****************************************************************************/
/**********************************Vector (c)*********************************/
/*****************************************************************************/
#ifndef DECLARE_SIMPLEVECTORFUNCS_C
#define DECLARE_SIMPLEVECTORFUNCS_C(NAME, FUNCNAME, DATATYPE)\
void FUNCNAME##_OnCopyItem(NAME* pThis, size_t loc, const DATATYPE* item)\
{ pThis->mData[loc] = *item; }\
void FUNCNAME##_OnCopyItemMultiple(NAME* pThis, size_t loc, const DATATYPE* item, size_t amount)\
{\
	size_t i;\
	for (i = loc; i < loc + amount; ++i)\
		FUNCNAME##_OnCopyItem(pThis, i, item);\
}\
void FUNCNAME##_OnCopyItems(NAME* pThis, size_t loc, const DATATYPE** items, size_t amount)\
{\
	size_t i, j;\
	for (i = loc, j = 0; i < loc + amount; ++i, ++j)\
		FUNCNAME##_OnCopyItem(pThis, i, &(*items)[j]);\
}\
void FUNCNAME##_OnDestroyItems(NAME* pThis, size_t loc, size_t amount)\
{ (void)(pThis); (void)(loc); (void)(amount); }

#define DECLARE_VECTOR_C(NAME, FUNCNAME, DATATYPE, ITEMCOPYFUNCS) \
typedef struct NAME\
{\
	DATATYPE*	mData;\
	size_t	mLength;\
	size_t	mSize;\
} NAME;\
ITEMCOPYFUNCS \
void FUNCNAME##_RemoveAll(NAME* pThis)\
{\
	FUNCNAME##_OnDestroyItems(pThis, 0, pThis->mLength);\
	pThis->mLength = 0;\
}\
void FUNCNAME##_Destroy(NAME* pThis)\
{\
	if (pThis->mSize)\
	{\
		FUNCNAME##_RemoveAll(pThis);\
		free(pThis->mData);\
	}\
}\
diffbool FUNCNAME##_DoResize(NAME* pThis, size_t newSize)\
{\
	diffbool success; \
	DATATYPE* newData = \
		(DATATYPE*)realloc(pThis->mData, (size_t)(newSize) * sizeof(DATATYPE));\
	success = newData != NULL; \
	if(!success)\
	{\
		FUNCNAME##_Destroy(pThis);\
		pThis->mSize = pThis->mLength = 0;\
	}\
	else\
		pThis->mSize = newSize;\
	pThis->mData = newData;\
	return success;\
}\
diffbool FUNCNAME##_CopyBeforeWrite(NAME* pThis, size_t minimumSize)\
{\
	if (minimumSize > pThis->mSize)\
		return FUNCNAME##_DoResize(pThis, minimumSize * 2);\
	return TRUE;\
}\
void FUNCNAME##_reserve(NAME* pThis, size_t newsize)\
{\
	if (pThis->mSize < newsize)\
		FUNCNAME##_DoResize(pThis, newsize);\
}\
void FUNCNAME##_resize(NAME* pThis, size_t inLength)\
{ FUNCNAME##_reserve(pThis, inLength); pThis->mLength = inLength; }\
void FUNCNAME##_append(NAME* pThis, const DATATYPE* item)\
{\
	if (!FUNCNAME##_CopyBeforeWrite(pThis, pThis->mLength + 1))\
		return;\
	FUNCNAME##_OnCopyItem(pThis, pThis->mLength, item);\
	pThis->mLength += 1;\
}\
void FUNCNAME##_AppendEmpty(NAME* pThis)\
{\
	if (!FUNCNAME##_CopyBeforeWrite(pThis, pThis->mLength + 1))\
		return;\
	pThis->mLength += 1;\
}\
void FUNCNAME##_AppendWithItem(NAME* pThis, const DATATYPE* item, size_t amount)\
{\
	if (!FUNCNAME##_CopyBeforeWrite(pThis, pThis->mLength + amount))\
		return;\
	FUNCNAME##_OnCopyItemMultiple(pThis, pThis->mLength, item, amount);\
	pThis->mLength += amount;\
}\
void FUNCNAME##_AppendWithItems(NAME* pThis, const DATATYPE** data, size_t amount)\
{\
	if (!FUNCNAME##_CopyBeforeWrite(pThis, pThis->mLength + amount))\
		return;\
	FUNCNAME##_OnCopyItems(pThis, pThis->mLength, data, amount);\
	pThis->mLength += amount;\
}\
void FUNCNAME##_assign(NAME* pThis, const NAME* otherArray)\
{\
	FUNCNAME##_Destroy(pThis);\
	pThis->mData = otherArray->mData;\
	pThis->mLength = otherArray->mLength;\
	pThis->mSize = otherArray->mSize;\
	((NAME*)otherArray)->mSize = 0;\
}\
void FUNCNAME##_BeforeWrite(NAME* pThis, size_t len) { FUNCNAME##_reserve(pThis, len); }\
size_t FUNCNAME##_capacity(const NAME* pThis)\
{ return pThis->mSize; }\
void FUNCNAME##_erase(NAME* pThis, size_t loc, size_t amount)\
{\
	WSASSERT(pThis->mLength >= loc + amount);\
	FUNCNAME##_OnDestroyItems(pThis, loc, amount);\
	pThis->mLength -= amount;\
	if (loc < pThis->mLength)\
		WSMemoryCopy(&pThis->mData[loc], &pThis->mData[loc + amount],\
						(size_t)(pThis->mLength - loc) * sizeof(DATATYPE));\
}\
void FUNCNAME##_clear(NAME* pThis)\
{ FUNCNAME##_erase(pThis, 0, pThis->mLength); }\
DATATYPE* FUNCNAME##_data(NAME* pThis)\
{ return pThis->mData; }\
void FUNCNAME##_insert(NAME* pThis, const DATATYPE** data, size_t amount,\
			size_t loc)\
{\
	WSASSERT(data != NULL && *data != NULL);\
	WSASSERT(amount != 0);\
	if (!FUNCNAME##_CopyBeforeWrite(pThis, pThis->mLength + amount))\
		return;\
	WSMemoryMove(&pThis->mData[loc + amount], &pThis->mData[loc],\
					(size_t)(pThis->mLength - loc) * sizeof(DATATYPE));\
	FUNCNAME##_OnCopyItems(pThis, loc, data, amount);\
	pThis->mLength += amount;\
}\
void FUNCNAME##_InsertItem(NAME* pThis, const DATATYPE* item, size_t loc)\
{ FUNCNAME##_insert(pThis, &item, 1, loc); }\
void FUNCNAME##_push_back(NAME* pThis, const DATATYPE* item)\
{ FUNCNAME##_append(pThis, item); }\
void FUNCNAME##_push_back_nonconst(NAME* pThis, DATATYPE* item)\
{ FUNCNAME##_append(pThis, (const DATATYPE*)item); }\
void FUNCNAME##_SetSize(NAME* pThis, size_t len) { pThis->mLength = len; } \
size_t FUNCNAME##_size(const NAME* pThis)\
{ return pThis->mLength; }\
void FUNCNAME##_Transfer(NAME* pThis, DATATYPE** data, size_t amount)\
{\
	WSASSERT(amount <= pThis->mLength);\
	WSMemoryCopy(*data, pThis->mData, (size_t)(amount) * sizeof(DATATYPE));\
	WSMemoryMove(pThis->mData, pThis->mData+amount, (size_t)(pThis->mLength -= amount) * sizeof(DATATYPE));\
}\
void FUNCNAME##_Construct(NAME* pThis)\
{\
	pThis->mData = NULL;\
	pThis->mLength = pThis->mSize = 0;\
}\
void FUNCNAME##_ConstructWithSize(NAME* pThis, size_t size) \
{\
	pThis->mLength = 0; \
	pThis->mData = (DATATYPE*)\
		malloc((size_t)(size) * sizeof(DATATYPE));\
	if(pThis->mData)					\
		pThis->mSize = size;		\
	else \
		pThis->mSize = 0;	\
}\
void FUNCNAME##_ConstructCopy(NAME* pThis, const NAME* otherArray) \
{\
	pThis->mData = otherArray->mData; \
	pThis->mLength = otherArray->mLength;\
	pThis->mSize = otherArray->mSize;\
	((NAME*)otherArray)->mSize = 0;\
}\
void FUNCNAME##_Destruct(NAME* pThis) { FUNCNAME##_Destroy(pThis); }\
DATATYPE* FUNCNAME##_At(NAME* pThis, size_t index) { return &pThis->mData[index]; }

#define DECLARE_SIMPLEVECTOR_C(NAME, FUNCNAME, DATATYPE) \
DECLARE_VECTOR_C(NAME, FUNCNAME, DATATYPE, DECLARE_SIMPLEVECTORFUNCS_C(NAME, FUNCNAME, DATATYPE))
#endif /* #ifndef DECLARE_SIMPLEVECTORFUNCS_C */
/*****************************************************************************/
/**********************************MemoryStream*******************************/
/*****************************************************************************/
void MemoryStream_Construct(MEMORYSTREAM* pThis, void* data, size_t size)
{
	pThis->mData = ((unsigned char*)data);
	pThis->mSize = (size);
	pThis->mPosition = (0);
}
size_t MemoryStream_ReadWrite(MEMORYSTREAM* pThis,
	void* p1, const void* p2, size_t n)
{
	size_t remaining = pThis->mSize - pThis->mPosition;
	if (n > remaining)
		n = remaining;
	WSMemoryCopy(p1, p2, n);
	pThis->mPosition += n;
	return n;
}
size_t MemoryStream_Read(MEMORYSTREAM* pThis, void* p, size_t n)
{ return MemoryStream_ReadWrite(pThis, p, &pThis->mData[pThis->mPosition], n); }
size_t MemoryStream_Write(MEMORYSTREAM* pThis, const void* p, size_t n)
{
	size_t remaining = pThis->mSize - pThis->mPosition;
	if (n > remaining)
	{
		unsigned char* newdata = (unsigned char*)realloc(pThis->mData,
			(pThis->mSize + (n - remaining)) * sizeof(unsigned char));
		if(newdata)
		{
			pThis->mData = newdata;
			pThis->mSize += (n - remaining);
		}
	}

	return MemoryStream_ReadWrite(pThis, &pThis->mData[pThis->mPosition], p, n);
}
size_t MemoryStream_Seek(MEMORYSTREAM* pThis, ptrdiff_t ulWhere, long lMode)
{
	if (lMode == STREAM_FROMSTART)
	{
		if (ulWhere > (ptrdiff_t)pThis->mSize)
			return (size_t)-1;
		return pThis->mPosition = ulWhere;
	}
	else if (lMode == STREAM_FROMEND)
	{
		if (ulWhere > (ptrdiff_t)pThis->mSize)
			return (size_t)-1;
		return pThis->mPosition = pThis->mSize - ulWhere;
	}
	else
	{
		if ((ptrdiff_t)(pThis->mPosition + ulWhere) > (ptrdiff_t)pThis->mSize
			|| (ptrdiff_t)(pThis->mPosition + ulWhere) < 0)
			return (size_t)-1;
		return pThis->mPosition += ulWhere;
	}
}
size_t MemoryStream_Tell(MEMORYSTREAM* pThis)
{ return pThis->mPosition; }

/*****************************************************************************/
/**********************************Diff***************************************/
/*****************************************************************************/

typedef struct DIFFDIAG {
	size_t x;			/* x axis position of aline start */
	size_t y;			/* y axis position of line start */
	size_t length;		/* length of diagonal line */
	size_t next;		/* next diagonal line towards start position */
	diffbool hasNext;	/* whether next DIFFDIAG member is valid */
} DIFFDIAG;
DECLARE_SIMPLEVECTOR_C(DIFFDIAGVECTOR, DiffDiagVector, DIFFDIAG)
/* Private */
diffbool Diff_input(DIFF* pThis, int which);
void Diff_change(DIFF* pThis, MEMORYSTREAM* outStream, size_t type,
				 linenum_t astart, linenum_t  aend,
				 linenum_t bstart, linenum_t  bend);
void Diff_range(DIFF* pThis, MEMORYSTREAM* outStream,
				linenum_t from, linenum_t to, linenum_t w);
diffbool Diff_fetch(DIFF* pThis, MEMORYSTREAM* outStream,
				char** buffer, size_t* bufferSize,
				linenum_t start, linenum_t end, linenum_t trueend,
				int nStream, const char* pfx);
void Diff_PutString(DIFF* pThis, MEMORYSTREAM* outStream, const char* s);
void Diff_PutStringWithLen(DIFF* pThis, MEMORYSTREAM* outStream,
						   const char* s, size_t len);
void Diff_PutNewline(DIFF* pThis, MEMORYSTREAM* outStream);
void Diff_PutChar(DIFF* pThis, MEMORYSTREAM* outStream, char c);
void Diff_PutInt(DIFF* pThis, MEMORYSTREAM* outStream, linenum_t n);
/* utilities */
char DiffAnsiiToLower(char x)
{ /* ANSII tolower() */
	if (x >= 'A' && x <= 'Z')
		x += ('a' - 'A');
	return x;
}
diffuint16 CRC16(const char* buffer, size_t bufferlen);
#define CSIZE_INC 50 /* How many to allocate each time we have to */
/*
#if defined(_WIN32)
#	define WS_WIN
#elif defined(applec) || defined(THINK_C) || \
	 (defined(__MWERKS__) && defined(macintosh) && !defined(__MACH__))
#	define WS_MAC
#else //Unix-like
#	define WS_UNIX
#endif
*/
#	define WS_UNIX
/**
	Initailizes the diff object.
	@param pThis The diff object to initialize
	@param leftStream Stream being used as the base, or
		what is on the left side of the output.
	@param rightStream Stream being compared to leftStream, or
		what is on the right side of the output.
**/
void Diff_Create(DIFF* pThis, MEMORYSTREAM* leftStream, MEMORYSTREAM* rightStream)
{
	pThis->streams[0] = leftStream;
	pThis->streams[1] = rightStream;
	pThis->contextlinecount = 0;
	pThis->diags = NULL;
	pThis->lines[0].data = pThis->lines[1].data = NULL;
}
/**
	Uninitializes the diff object.
	@param pThis The diff object to uninitialize.
**/
void Diff_Destroy(DIFF* pThis)
{
	free(pThis->lines[0].data); free(pThis->lines[1].data);
	if(pThis->diags)
	{
		free(pThis->fpM.data); free(pThis->fpN.data);
		DiffDiagVector_Destruct(pThis->diags); free(pThis->diags);
	}
}

void Diff_PutStringWithLen(DIFF* pThis, MEMORYSTREAM* outStream, const char* s, size_t len)
{ /* Write the string */
#if 0 /* check to make sure we didn't get any newlines in output buffer */
	for (size_t i = 0; i < strlen(s); ++i)
		WSASSERT(s[i] != '\n' && s[i] != '\r');
#endif
	Stream_Write(outStream, s, len);
	(void)(pThis);
}

void Diff_PutString(DIFF* pThis, MEMORYSTREAM* outStream, const char* s)
{ /* Write the string */
#if 0 /* check to make sure we didn't get any newlines in output buffer */
	for (size_t i = 0; i < strlen(s); ++i)
		WSASSERT(s[i] != '\n' && s[i] != '\r');
#endif
	Stream_Write(outStream, s, strlen(s));
	(void)(pThis);
}

/** put a line of text plus a platform-specific
	newline set to the output stream */
void Diff_PutNewline(DIFF* pThis, MEMORYSTREAM* outStream)
{
	if(pThis->flags & DIFF_OUTPUT_WINDOWSNEWLINES)
		Stream_Write(outStream, "\r\n", sizeof(char) * 2);
	else if(pThis->flags & DIFF_OUTPUT_MACNEWLINES)
		Stream_Write(outStream, "\r", sizeof(char));
	else
		Stream_Write(outStream, "\n", sizeof(char));
}

void Diff_PutChar(DIFF* pThis, MEMORYSTREAM* outStream, char c)
{
	Diff_PutStringWithLen(pThis, outStream, &c, 1);
}

void Diff_PutInt(DIFF* pThis, MEMORYSTREAM* outStream, linenum_t n)
{
	char sz[21];
	DiffDigitToString(sz, n);
	Diff_PutString(pThis, outStream, sz);
}

/**
* Reads one line (or character/word) from a specified input stream and
* processes it according to diff options.
*
* This function reads a line from one of the input streams associated with a
* `DIFF` object, removing the terminating newline character. It also handles
* special processing based on diff options such as ignoring case
* (`DIFF_COMPARE_IGNORECASE`), ignoring whitespace
* (`DIFF_COMPARE_IGNOREWHITESPACE`), and handling input by character or word
* (`DIFF_COMPARE_BYCHAR`, `DIFF_COMPARE_BYWORD`) instead of by line.
*
* The function dynamically allocates or reallocates a buffer to store the read
* line, adjusting the buffer size as necessary to fit the line. The function
* automatically removes the newline character from the end of the line. If the
* `-b` option was specified, it also removes trailing whitespace and replaces
* multiple whitespace characters with a single space.
*
* Special processing for UTF-8 input and case-insensitive comparisons
* may require additional considerations not fully detailed here.
*
* @param pThis Pointer to the `DIFF` object associated with the input
*              streams.
* @param n Specifies which stream to read from (0 for left, 1 for right).
* @param in_buffer Pointer to a buffer that receives the line content. If
*                  `bufsize` is non-zero after the function call, this buffer
*                  is dynamically allocated with malloc() and can be reused. It
*                  must be freed after all calls to `Diff_GetLine` if `bufsize`
*                  is non-zero with free().  `in_buffer` must either point to a
*                  null pointer or a valid dynamically allocated buffer when
*                  passed to this function.
* @param bufsize Pointer to a variable holding the current size of
*                `in_buffer`. Must be 0 when the function is first called with
*                a new buffer. If still 0 after the function call, no dynamic
*                allocation was performed.
* @param outlen Pointer to a variable that receives the character length of
*               the output in `in_buffer`. The terminating null character is
*               not counted in `outlen`.
* @return `FALSE` if the end of the specified stream is reached or memory
*         allocation fails, otherwise `TRUE`.
*/
diffbool Diff_GetLine(DIFF* pThis, int n, char** in_buffer, size_t* bufsize, size_t* outlen)
{
	register char* top;
	register char* fromp;
	register char* buffer = *in_buffer;
	register size_t i;

	WSASSERT(in_buffer);
	WSASSERT(bufsize);

	/* Read in a line or word */
	for (i = 0; i < LINELENGTH_MAX; ++i)
	{
		/* reallocate buffer if neccesary */
		if (i >= *bufsize)
		{ /* check for overflow */
			/* 257 was the PD diff's buffer size, so use
				that as an increment */
			if (*bufsize + 257 < *bufsize)
				*bufsize = LINELENGTH_MAX;
			else
				*bufsize += 257;

			if (*bufsize == 257)
				buffer = (char*) malloc(*bufsize);
			else
				buffer = (char*) realloc(*in_buffer, *bufsize);
			if(!buffer)
			{
				if(*bufsize != 257)
					free(*in_buffer);
				return FALSE;
			}

			*in_buffer = buffer;
		}

		/* read in a character */
		if (Stream_Read(pThis->streams[n],
						&buffer[i], sizeof(*buffer)) != sizeof(*buffer))
		{
			if (i == 0)
				return FALSE;
			else
				break;
		}

		/* "[new]line" found? */
		if (pThis->flags & DIFF_COMPARE_BYCHAR)
			break;
		else if (pThis->flags & DIFF_COMPARE_BYWORD)
		{
			/* read in character break chars */
			if (buffer[i] == '\n' || buffer[i] == '\r'
			   || buffer[i] == ' ' || buffer[i] == '\t')
			{
				if (i)
					break;
			}
		}
		else /* BYLINE */
		{
			if (buffer[i] == '\n' || buffer[i] == '\r')
			{
				/* skip past <LF> in <CR><LF> combo */
				if (buffer[i] == '\r')
				{
					char tempchar;
					if (Stream_Read(pThis->streams[n],
									&tempchar,
									sizeof(*buffer)) == sizeof(*buffer))
					{
						if (tempchar != '\n')
							Stream_Seek(pThis->streams[n], -1, STREAM_FROMCURRENT);
					}
				}

				/* found newline - done reading line */
				break;
			} /* end if \r or \n */
		} /* end switch/if (flags & COMPARE_X) */
	} /* end for (i<LINELENGTH_MAX) */

	/* mark the string length */
	*outlen = i;

	/* Deal with special cases (options) */
	if (pThis->flags & (DIFF_COMPARE_IGNORECASE |
						DIFF_COMPARE_IGNOREWHITESPACE))
	{
		top = buffer;
		fromp = buffer;

		while ((size_t)(fromp - buffer) < i)
		{
			if ((pThis->flags & DIFF_COMPARE_IGNOREWHITESPACE)
				&& (*fromp == ' ' || *fromp == '\t'))
			{
				*top++ = ' ';
				if ((size_t)(top - buffer) == i)
					break;
				do
				{
					fromp++;
				} while (*fromp == ' ' || *fromp == '\t');
			}
#ifdef WIKISERVER_VERSION /* UTF8 is a lot of code to bring in */
			else if(!(pThis->flags & DIFF_COMPARE_NOUTF8))
			{
				size_t clen;
				if (pThis->flags & DIFF_COMPARE_IGNORECASE)
				{
					uint32_t clower = UTF8CharToLower(fromp);
					clen = UCSCharUTF8Size(clower);
					if ((top - buffer + clen) >= i)
						break;
					UCSCharToUTF8(clower, top);
					fromp += UTF8CharLen(fromp);
					top += clen;
				}
				else
				{
					clen = UTF8CharLen(fromp);
					if ((top - buffer + clen) >= i)
						break;
					for (; clen != 0; --clen)
						*top++ = *fromp++;
				}
			}
#endif
			else
			{
				if (pThis->flags & DIFF_COMPARE_IGNORECASE)
					*top++ = (char) DiffAnsiiToLower(*fromp);
				else
					*top++ = *fromp;
				++fromp;
			}
		}
		if ((pThis->flags & DIFF_COMPARE_IGNOREWHITESPACE)
			 && top[-1] == ' ')
			top--;
		*top = '\0';
	}

	return TRUE;
}

/**
* [Private] Outputs a block of text lines with optional prefix from specified
* streams.
*
* This function is designed to output a specified range of lines from one of
* the text streams being compared in a diff operation. It handles context lines
* by adjusting the start and end of the range to include additional lines
* surrounding the specified range. The function can prepend a prefix to each
* line, typically used to indicate the nature of the change (e.g., '+', '-',
* or a context indicator). It manages buffer allocation for reading lines and
* ensures that output respects the configured diff output format (context or
* unified).
*
* This function adjusts `start` and `end` to include context lines as
* specified by `pThis->contextlinecount`. It ensures that the adjusted
* range does not exceed the bounds of the file.
*
* Line numbers are zero-based internally but are adjusted for one-based
* indexing when interacting with the diff's line data structures.
*
* The function seeks to the correct position within the input stream
* before reading lines. If seeking fails or the end of the file is
* reached unexpectedly, it outputs an error message to `outStream`.
*
* The buffer for line reading is managed dynamically; its size is
* adjusted as needed to accommodate the length of lines being read.
*
* @param pThis Pointer to the `DIFF` object containing configuration and state
*              for the diff operation.
* @param outStream Pointer to the `MEMORYSTREAM` where the fetched lines will
*                  be written.
* @param buffer Pointer to a character buffer for storing line data. May be
*               reallocated if the buffer is too small.
* @param bufferSize Pointer to the size of `buffer`. Updated if `buffer` is
*                   reallocated to accommodate longer lines.
* @param start The starting line number of the range to output.
* @param end The ending line number of the range to output.
* @param trueend The actual end line number of the file, used to prevent
*                context lines from extending beyond the file's bounds.
* @param nStream The index of the stream (0 or 1) from which lines are fetched.
* @param pfx The prefix string to prepend to each line, indicating the nature
*            of the change (e.g., '+', '-', or context).
* @return A boolean indicating success (`TRUE`) or failure (`FALSE`) of the
*         operation.
*/
diffbool Diff_fetch(DIFF* pThis, MEMORYSTREAM* outStream,
				char** buffer, size_t* bufferSize,
				linenum_t start, linenum_t end, linenum_t trueend,
				int nStream, const char* pfx)
{
	register linenum_t i;
	register linenum_t first;
	register linenum_t last;
	size_t bufferLen;

	if (pThis->contextlinecount)
	{
		if ((start - pThis->contextlinecount > start)
			|| ((first = start - pThis->contextlinecount) <= 0))
			first = 1;
		if ((end + pThis->contextlinecount < end)
			|| ((last = end + pThis->contextlinecount) > trueend))
			last = trueend;
	}
	else
	{
		first = start;
		last = end;
	}

	--first;
	--last;
	--start;
	--end;

	if ((filesize_t)Stream_Seek(pThis->streams[nStream],
					pThis->lines[nStream].data[first].pos,
					STREAM_FROMSTART) != pThis->lines[nStream].data[first].pos)
	{
#ifdef DIFFDEBUG
		printf("?Can't read line %u at %08zx (hex) in file%c\n",
			(unsigned int)start, pThis->lines[nStream].data[first].pos,
			   (nStream == 0) ? 'A' : 'B');
#endif /* DIFFDEBUG */
	}
	else  /* successfully seeked */
	{
		for (i = first; i <= last; i++)
		{
			if (!Diff_GetLine(pThis, nStream, buffer, bufferSize, &bufferLen))
			{
				Diff_PutString(pThis, outStream, "** Unexpected end of file");
				Diff_PutNewline(pThis, outStream);
				return FALSE;
			}
			Diff_PutString(pThis, outStream,
						   ((pThis->flags & (DIFF_OUTPUT_CONTEXT
											| DIFF_OUTPUT_UNIFIED))
								&& (i < start || i > end)) ? "	" : pfx
						  );
			Diff_PutStringWithLen(pThis, outStream, *buffer, bufferLen);
			Diff_PutNewline(pThis, outStream);
		}
	}

	return TRUE;
}

/**
* [Private] Outputs a formatted line number range to the specified output
* stream.
*
* This function prints a range of line numbers, adjusting for context if
* specified within the `DIFF` object. The context adjustment adds a predefined
* number of lines before and after the change to provide more information
* around each diff block. The function formats the range based on the `from`
* and `to` line numbers, including adjustments for context, and outputs this to
* the provided `MEMORYSTREAM`. The function supports output for both sides of
* the diff (e.g., `w = 0` for the first file, `w = 1` for the second file) and
* adjusts the range to stay within the bounds of the file's line numbers.
*
* The range is inclusive and formatted in a manner suitable for diff
* outputs, e.g., "from,to" for ranges spanning multiple lines or just
* "from" for a single line. The function ensures the range does not
* exceed the file's line count and adjusts ranges based on the
* `contextlinecount` specified in `pThis`.
*
* If the context line count causes the range to extend beyond the start or
* end of the file, the function adjusts the range to fit within the file's
* actual line numbers.
*
* The function handles reverse ranges (where `to < from`) by swapping the
* values to ensure a logical, forward range is always output.
*
* @param pThis Pointer to the `DIFF` object, containing state and configuration
*              for the diff operation, including the context line count.
* @param outStream Pointer to the `MEMORYSTREAM` where the formatted range will
*                  be written.
* @param from The starting line number of the range.
* @param to The ending line number of the range.
* @param w The index of the file (0 or 1) from which the line numbers are
*          derived, used to ensure range adjustments respect file boundaries.
*/
void Diff_range(DIFF* pThis, MEMORYSTREAM* outStream,
	linenum_t from, linenum_t to, linenum_t w)
{
	if (pThis->contextlinecount)
	{
		if ((from - pThis->contextlinecount > from)
			|| ((from -= pThis->contextlinecount) <= 0))
			from = 1;
		if ((to + pThis->contextlinecount < to)
			|| ((to += pThis->contextlinecount) > pThis->lines[w].length))
			to = pThis->lines[w].length;
	}

	if (to > from)
	{
		Diff_PutInt(pThis, outStream, from);
		Diff_PutStringWithLen(pThis, outStream, ",", 1);
		Diff_PutInt(pThis, outStream, to);
	}
	else if (to < from)
	{
		Diff_PutInt(pThis, outStream, to);
		Diff_PutStringWithLen(pThis, outStream, ",", 1);
		Diff_PutInt(pThis, outStream, from);
	}
	else
	{
		Diff_PutInt(pThis, outStream, from);
	}
}

/**
* [Private] Outputs a change entry indicating differences between file
* segments.
*
* This function formats and outputs a description of a change detected by the
* diff algorithm (file[0][astart..aend] changed to file[1][bstart..bend]). It
* specifies the segments of the two files (or streams) that differ from each
* other and how they are transformed (added, removed, changed). The output
* format can vary based on the flags provided in the `DIFF` object, supporting
* unified, context, and traditional diff formats.
*
* The function handles "dummy" entries where no actual change occurred (e.g.,
* where start and end line numbers would indicate a non-existent segment) by
* simply returning without outputting anything. It prepares and outputs the
* header for the change block, according to the diff output style specified in
* `pThis->flags`, and then outputs the actual text of the changes, using
* prefixes like '+', '-', or '!' to indicate additions, deletions, or changes,
* respectively.
*
* This function performs its output in a manner that respects the diff
* output format specified by `pThis->flags`. It may output in unified,
* context, or traditional (ED) diff format.
*
* Line numbers are zero-based and inclusive. The function correctly
* handles cases where the start line number is greater than the end line
* number, indicating an empty segment.
*
* The function allocates a buffer for fetching and formatting lines of
* text to be output. This buffer is dynamically managed and freed before
* the function returns to avoid memory leaks.*
* @param pThis Pointer to the `DIFF` object containing configuration and state
*              for the diff operation, including output flags and stream data.
* @param outStream Pointer to a `MEMORYSTREAM` object where the change entry
*                  will be written.
* @param type The type of change (DIFFBLOCK_ADD, DIFFBLOCK_REMOVE, or other)
*             indicating how the file segments differ.
* @param astart Starting line number of the segment in the first file/stream.
* @param aend Ending line number of the segment in the first file/stream.
* @param bstart Starting line number of the segment in the second file/stream.
* @param bend Ending line number of the segment in the second file/stream.
*/
void Diff_change(DIFF* pThis, MEMORYSTREAM* outStream, size_t type,
				 linenum_t astart, linenum_t aend,
				 linenum_t bstart, linenum_t bend)
{
	char c;
	const char* szHL;
	char* buffer;
	size_t bufferSize;

	/*
	 * This catches a "dummy" last entry
	 */
	if (astart > aend && bstart > bend)
		return;
	/* init */
	buffer = NULL;
	bufferSize = 0;
	c = (type == DIFFBLOCK_ADD) ? 'a' : (type == DIFFBLOCK_REMOVE) ? 'd' : 'c';

	/*
	 *	Print the block header
	 */
	if (pThis->flags & DIFF_OUTPUT_UNIFIED)
	{
		Diff_PutString(pThis, outStream, "@@ -");
	}
	else if (pThis->flags & DIFF_OUTPUT_CONTEXT)
	{
		Diff_PutString(pThis, outStream, "**************");
		Diff_PutNewline(pThis, outStream);
		Diff_PutString(pThis, outStream, "*** ");
	}

	if (type == DIFFBLOCK_ADD && !(pThis->flags & DIFF_OUTPUT_CONTEXT))
	{
		/* Addition: just print one odd # */
		Diff_range(pThis, outStream, astart, astart, 0);
	}
	else
	{
		/* Print both, if different */
		Diff_range(pThis, outStream, astart, aend, 0);
	}

	if (!(pThis->flags & DIFF_OUTPUT_CONTEXT))
	{
		if (pThis->flags & DIFF_OUTPUT_UNIFIED)
		{
			Diff_PutString(pThis, outStream, " +");
		}
		else
		{
			Diff_PutChar(pThis, outStream, c);
		}

		if (!(pThis->flags & DIFF_OUTPUT_ED))
		{
			if (c == 'd')
			{
				/* Deletion: just print one odd # */
				Diff_range(pThis, outStream, bstart, bstart, 1);
			}
			else
			{
				/* Print both, if different */
				Diff_range(pThis, outStream, bstart, bend, 1);
			}
		}

		if (pThis->flags & DIFF_OUTPUT_UNIFIED)
		{
			Diff_PutString(pThis, outStream, " @@");
		}
	}

	Diff_PutString(pThis, outStream, "");
	Diff_PutNewline(pThis, outStream);

	if(!((pThis->flags & DIFF_OUTPUT_ED) && type == DIFFBLOCK_REMOVE))
	{
		/*
		 *	Now print the first block of text with the prefix szHL
		 */
		/********************** left side **************************/
		if ((type != DIFFBLOCK_ADD) || (pThis->flags & DIFF_OUTPUT_CONTEXT))
		{
			if((type != DIFFBLOCK_ADD))
			{
				if (pThis->flags & DIFF_OUTPUT_UNIFIED)
					szHL = "- ";
				else if (pThis->flags & DIFF_OUTPUT_CONTEXT)
					szHL = (c == 'd' ? "- " : "! ");
				else if (!(pThis->flags & DIFF_OUTPUT_ED))
					szHL = "< ";
				else
					szHL = "";

				Diff_fetch(pThis, outStream, &buffer, &bufferSize,
					astart, aend, pThis->lines[0].length, 0, szHL);
			}

			if (pThis->flags & (DIFF_OUTPUT_CONTEXT))
			{
				Diff_PutString(pThis, outStream, "--- ");
				Diff_range(pThis, outStream, bstart, bend, 1);
				Diff_PutString(pThis, outStream, " -----");
				Diff_PutNewline(pThis, outStream);
			}
			else if (!(pThis->flags & (DIFF_OUTPUT_UNIFIED|DIFF_OUTPUT_ED))
					 && (astart <= aend && bstart <= bend)
					 && (type != DIFFBLOCK_ADD)
					 && (type != DIFFBLOCK_REMOVE))
			{
				Diff_PutString(pThis, outStream, "---");
				Diff_PutNewline(pThis, outStream);
			}
		}
		/********************** right side **************************/
		if ((type != DIFFBLOCK_REMOVE))
		{
			/*
			 *	Now print the second block of text with the prefix szHL
			 */
			if (pThis->flags & DIFF_OUTPUT_UNIFIED)
				szHL = "+ ";
			else if (pThis->flags & DIFF_OUTPUT_CONTEXT)
				szHL = (c == 'a' ? "+ " : "! ");
			else if (!(pThis->flags & DIFF_OUTPUT_ED))
				szHL = "> ";
			else
				szHL = "";

			Diff_fetch(pThis, outStream, &buffer, &bufferSize,
					   bstart, bend, pThis->lines[1].length, 1, szHL);

			if ((pThis->flags & DIFF_OUTPUT_ED)
				&& bstart <= bend
				&& (type != DIFFBLOCK_REMOVE))
			{
				Diff_PutString(pThis, outStream, ".");
				Diff_PutNewline(pThis, outStream);
			}
		}
	} /* end if !(ed && remove) */
	/* clean up */
	if(bufferSize)
		free(buffer);
}

/**
* Calculates the CRC16 checksum for a given buffer using a specific algorithm
* variant.
*
* This implementation of the CRC16 checksum calculation is tailored for speed
* and simplicity. It processes the buffer byte by byte, applying a series of
* bitwise operations to compute the checksum. The algorithm uses a bit-shifting
* technique to iterate through each bit of the input data and combines this
* with a series of XOR operations based on shifted versions of the intermediate
* result. This approach avoids the need for a lookup table, thus reducing
* memory usage while still providing a reliable checksum calculation suitable
* for many applications where data integrity needs to be verified.
*
* The specific steps for each byte include:
* - XORing the current byte of data with the high byte of the current CRC.
* - Shifting the CRC value left by 8 bits.
* - Applying three successive XOR operations with shifted versions of an
*   intermediate value derived from the XOR operation in the first step.
*   These operations incorporate bits from the entire 8-bit breadth of the
*   data, effectively spreading their influence throughout the resulting CRC.
*
* This implementation does not specify the polynomial used, which is a
* critical component of CRC calculations. Different polynomials will
* produce different results for the same input data. The lack of a
* specified polynomial means this implementation may not be directly
* compatible with other CRC16 calculations expected by some protocols
* or standards.
*
* @param buffer Pointer to the data buffer for which the CRC16 checksum is
*               to be calculated.
* @param len The number of bytes in the buffer.
* @return The calculated 16-bit CRC checksum. To ensure a non-zero value is
*         always returned, a calculated checksum of 0 is replaced with 1.
*         This adjustment is made because a checksum of 0 might be interpreted
*         as an indication of an error or an uninitialized state in some
*         contexts.
*/
diffuint16 CRC16(const char* buffer, size_t len)
{
	register diffuint16 crc, quick, temp;
	register const char* tp, *tpend;

	crc = 0;
	for (tp = buffer, tpend = tp + len; tp != tpend; ++tp)
	{
		temp = (diffuint16) ((crc >> 8) ^ *tp);
		crc <<= 8;
		quick = (diffuint16) (temp ^ (temp >> 4));
		crc ^= quick;
		quick <<= 5;
		crc ^= quick;
		quick <<= 7;
		crc ^= quick;
	}

	return ((crc == 0) ? ((diffuint16)1) : crc);
}

/**
	[Private] Read the file, building hash table
	@param pThis DIFF object that contains the streams
	@param which file number to get (0 = left, 1 = right)
*/
diffbool Diff_input(DIFF* pThis, int which)
{
	register DIFFLINE* lentry;
	register linenum_t linect = 0;
	register void* alloceddata;
	register size_t pos = 0;
	char* buffer = NULL;
	size_t buffersize = 0, bufferlen;

#define LSIZE_INC 200		/* # of line entries to alloc at once */
	linenum_t lsize = LSIZE_INC;

	lentry = (DIFFLINE*)malloc(sizeof(DIFFLINE) * (lsize));
	if(!lentry)
		return FALSE;

	while (Diff_GetLine(pThis, which, &buffer, &buffersize, &bufferlen)
			/* check for linect overflow */
			&& linect != LINENUM_MAX)
	{
		if (linect + 1 >= lsize)
		{
			/* check for lsize overflow */
			if (lsize + LSIZE_INC < lsize)
				lsize = LINENUM_MAX;
			else
				lsize += LSIZE_INC;

			alloceddata = realloc(lentry, (lsize) * sizeof(DIFFLINE));
			if(!alloceddata)
			{
				free(lentry);
				return FALSE;
			}
			lentry = (DIFFLINE*)alloceddata;
		}

		lentry[linect].val = (linenum_t)CRC16(buffer, bufferlen);
		lentry[linect].len = bufferlen;
		lentry[linect].pos = pos;
		pos = Stream_Tell(pThis->streams[which]);
		++linect;
	}

	pThis->lines[which].length = linect;
	pThis->lines[which].data = lentry;
	free(buffer);
	return TRUE;
}

/**
	Seeks to a line in a specified stream efficiently.
	@param pThis DIFF object that contains the streams
	@param nStream Stream number to seek within. 0 == left, 1 == right
	@param nLine Line number to seek to
*/
void Diff_SeekToLine(DIFF* pThis, int nStream, linenum_t nLine)
{
	Stream_Seek(pThis->streams[nStream],
				pThis->lines[nStream].data[nLine - 1].pos, STREAM_FROMSTART);
}

/**
* [Private] Subroutine for finding the furthest reaching point on a diagonal.
*
* The short version is this is the Diff_Compare() subroutine for finding the y
* coordinate of the furthest point on diagonal k that can be reached from
* (y ? k, y) by traversing diagonal edges.
*
* This function is a key component of the Myers diff algorithm, specifically
* adapted for the linear space refinement known as the "snake" function. It
* finds the furthest point that can be reached on diagonal `k` from a given
* starting point by advancing on equal elements (matching characters) in both
* input sequences. The function updates the given `fp` (furthest reaching point
* span) with the new furthest reaching point on diagonal `k`, including the
* score based on insertions/removals, changes, and matches.
*
* The function considers previous points (diagonals) and their scores to
* determine the best path (score) to the current point, effectively tracing
* the path of the diff through the edit graph. Diagonal moves (matches)
* increase the score more than vertical or horizontal moves
* (insertions/deletions), reflecting the alignment of the two sequences being
* compared.
*
* The "snake" function is crucial for efficiently computing the edit
* script between two sequences. It leverages the observation that
* matching sequences (diagonals in the edit graph) can be "jumped" over
* quickly, as opposed to step-by-step comparison.
*
* The function updates `fp` with the furthest reaching point found for
* diagonal `k`, along with the accumulated score and a reference to any
* discovered diagonal segment in `diags`. This information is used to
* reconstruct the edit path after the completion of the diff algorithm.
*
* The scoring system (`insRemScore`, `changeScore`, and `matchScore`)
* reflects the cost of insertions/deletions, substitutions, and matches,
* respectively. It is used to guide the algorithm towards the lowest
* cost (highest score) path through the edit graph.
*
* @param pThis Pointer to the `DIFF` object containing the sequences being
*              compared.
* @param k The diagonal being processed (k from the np algorithm).
* @param isKNegative Indicates if `k` is considered negative in the context of
*                    the algorithm. Negative `k` values are used for indexing
*                    into the `fp` array from the middle towards the start.
* @param fp Pointer to the `DIFFPOINTSPAN` representing the furthest reaching
*           points in the current phase of the algorithm.
* @param kindex The index in `fp` corresponding to diagonal `k`.
* @param diags Pointer to the `DIFFDIAGVECTOR` where newly discovered diagonal
*              segments are to be appended.
*/
void Diff_Snake(DIFF* pThis, const size_t k, diffbool isKNegative,
	DIFFPOINTSPAN* fp, const size_t kindex, DIFFDIAGVECTOR* diags)
{
	diffbool hasPrevDiag;
	size_t prevDiag, x, y, score;
	size_t length = 0;
	const size_t N = pThis->lines[0].length;
	const size_t M = pThis->lines[1].length;
	size_t insRemScore = 1;
	size_t changeScore = 4;
	size_t matchScore = 2;
	const DIFFPOINT* prevPt = &fp->data[kindex];
	DIFFPOINT rightPt;
	DIFFPOINT belowPt;
	if(kindex != fp->length - 1)
		rightPt = fp->data[kindex + 1];
	else
		memset(&rightPt, 0, sizeof(rightPt));
	if(kindex != 0)
		belowPt = fp->data[kindex - 1];
	else
		memset(&belowPt, 0, sizeof(belowPt));

	hasPrevDiag = prevPt->hasDiag;
	prevDiag = prevPt->diag;
	y = prevPt->hasY ? prevPt->y + 1 : 0;
	score = prevPt->score + changeScore;

	if ((rightPt.score + insRemScore) > score)
	{
		hasPrevDiag = rightPt.hasDiag;
		prevDiag = rightPt.diag;
		y = rightPt.y;
		score = rightPt.score + insRemScore;
	}

	if ((belowPt.score + insRemScore) > score)
	{
		hasPrevDiag = belowPt.hasDiag;
		prevDiag = belowPt.diag;
		if(belowPt.hasY)
			y = belowPt.y + 1;
		else
			y = 0;
		score = belowPt.score + insRemScore;
	}

	/* x := y ? k; */
	if(!isKNegative)
		x = y - k;
	else
		x = y + k;

	/* While x < M and y < N and A[x+1] = B[y+1] do */
	while (x < N && y < M
		&& pThis->lines[0].data[x].val == pThis->lines[1].data[y].val)
	{ /* x := x + 1; y := y + 1; */
		x++; y++; length++; score += matchScore;
	}

	if (length > 0)
	{
		DIFFDIAG newDiag;
		newDiag.x = x - length;
		newDiag.y = y - length;
		newDiag.length = length;
		newDiag.next = prevDiag;
		newDiag.hasNext = TRUE;
		DiffDiagVector_append(diags, &newDiag);
		fp->data[kindex].y = y;
		fp->data[kindex].score = score;
		fp->data[kindex].diag = DiffDiagVector_size(diags) - 1;
		fp->data[kindex].hasDiag = fp->data[kindex].hasY = TRUE;
	}
	else
	{
		fp->data[kindex].y = y;
		fp->data[kindex].score = score;
		fp->data[kindex].diag = prevDiag;
		fp->data[kindex].hasDiag = hasPrevDiag;
		fp->data[kindex].hasY = TRUE;
	}
}

/**
* Compares two text streams and builds a match vector with line number
* differences.
*
* This function performs a comparison between two sets of text streams
* represented within the `DIFF` object and identifies the differences between
* them. It builds a match vector detailing these differences, which is
* essential for subsequent operations like `Diff_BuildSeekVectors()` or
* `Diff_FillBlocks()`. The comparison uses the Myers diff algorithm, optimized with the
* linear space refinement for calculating the edit distance and generating the match
* vector.
*
* The function initializes necessary structures for the comparison, reads in
* the input streams, and prepares them for the diff algorithm. It ensures the
* input streams are in the correct order for the algorithm to work efficiently.
* After the preparation, it executes the diff algorithm, capturing the
* essential diagonal paths that represent the optimal edit script. The match
* vector, along with other metadata, is stored in the `DIFF` object for use in
* later stages.
*
* Before calling `Diff_Compare`, the text streams should be loaded into
* `pThis`. On success, `pThis` will contain the necessary information for
* generating the edit script and understanding the differences between
* the streams.
*
* The function may modify the order of the text streams within `pThis`
* for the purpose of the comparison but restores the original order before
* returning if necessary.
*
* Memory allocated during the function (for the match vector and other
* structures) is managed internally and should be freed by calling
* `Diff_Destroy` on the `DIFF` object when it is no longer needed.
*
* @param pThis Pointer to the `DIFF` object containing the text streams and
* where the comparison results will be stored.
* @param ops Bitwise ORed `DIFF_[XXX]` flags to tailor the comparison process.
* These flags might control aspects such as case sensitivity,
* whitespace handling, etc.
* @return A boolean value indicating the success (`TRUE`) or failure (`FALSE`)
* of the comparison. Failure can result from memory allocation issues or
* errors in reading the input streams.
*/
diffbool Diff_Compare(DIFF* pThis, int ops)
{
	size_t N, M, delta;
	size_t p, k; /* loop vars */
	DIFFDIAG emptydiag;
	if(pThis->diags)
		Diff_Destroy(pThis);
	pThis->flags = ops;
	/* read in the streams */
	if(!Diff_input(pThis, 0) || !Diff_input(pThis, 1))
	{
		free(pThis->lines[0].data); free(pThis->lines[1].data);
		pThis->lines[0].data = pThis->lines[1].data = NULL;
		return FALSE;
	}
	if (pThis->lines[0].length > pThis->lines[1].length)
	{ /* right vector has to be larger for the NP algorithm */
		WSSWAP(DIFFLINESPAN, pThis->lines[0], pThis->lines[1]);
		pThis->reverse = TRUE;
	}
	else
		pThis->reverse = FALSE;
	pThis->fpN.length = N = pThis->lines[0].length;
	pThis->fpM.length = M = pThis->lines[1].length;
	delta = M - N;

	/* set up vectors */
	pThis->fpN.data = (DIFFPOINT*)malloc(pThis->fpN.length * sizeof(DIFFPOINT));
	pThis->fpM.data = (DIFFPOINT*)malloc(pThis->fpM.length * sizeof(DIFFPOINT));
	pThis->diags = (DIFFDIAGVECTOR*)malloc(sizeof(DIFFDIAGVECTOR));
	if(!pThis->fpN.data || !pThis->fpM.data || !pThis->diags)
	{
		free(pThis->fpN.data); free(pThis->fpM.data);
		free(pThis->diags); pThis->diags = NULL;
		return FALSE;
	}
	memset(pThis->fpN.data, 0, pThis->fpN.length * sizeof(DIFFPOINT));
	memset(pThis->fpM.data, 0, pThis->fpM.length * sizeof(DIFFPOINT));
	memset(&emptydiag, 0, sizeof(emptydiag));
	DiffDiagVector_Construct(pThis->diags);
	DiffDiagVector_append(pThis->diags, &emptydiag);

	/*
		Calculate common diagonals with all leading to empty diags at zero.

		Note that there is some convoluted logic here as the NP alg uses
		negative numbers, which we can't have as outside code assumes we use
		the full range of values of the max size unsigned integer type,
		so we need logic to count what have been negative values of p and k.

		p := p + 1;
		For k := ?p to ??1 do
			fp[k] := snake( k, max (fp[k?1] + 1, fp[k+1]) );
		For k := ? + p downto ? + 1 by ?1 do
			fp[k] := snake( k, max (fp[k?1] + 1, fp[k+1]) );
		fp[?] := snake( ?, max (fp[??1] + 1, fp[?+1]) );
		Until fp[?] = N;
	*/
	for (p = 0; pThis->fpM.data[delta].y != M; p++)
	{
		/* Loop for negative k values */
		for (k = 0; k < p; k++)
			Diff_Snake(pThis, k, TRUE, &pThis->fpN, k, pThis->diags);

		/* Loop for non-negative k values */
		for (k = 0; k <= delta + p; k++)
			Diff_Snake(pThis, k, FALSE, &pThis->fpM, k, pThis->diags);
	}

	/* clean - note that the edit distance is (delta + (2 * p)) */
	if(pThis->reverse)
	{
		WSSWAP(DIFFLINESPAN, pThis->lines[0], pThis->lines[1]);
	}
	return TRUE;
}

/**
	Fills an array of DIFFBLOCK with the left and right line numbers and
	whether or not the lines were added/removed/changed.
	@param pThis DIFF object to calculate the DIFFBLOCK array from
	@param blocks DIFFBLOCK array to fill
	@param outCount pointer to the size of the DIFFBLOCK array, or
	if NULL is set to the size of all the line differences
*/
void Diff_FillBlocks(DIFF* pThis, DIFFBLOCK* blocks, size_t* outCount)
{
	size_t i, numBlocks = 0, numBlocksFilled = 0, x, y;
	DIFFBLOCK newBlock;
	const DIFFDIAG* diag;
	size_t numDel, numIns, numSubs;
	DIFFDIAG prevDiag;
	prevDiag.x = pThis->fpN.length; prevDiag.y = pThis->fpM.length;
	prevDiag.length = 0; prevDiag.hasNext = FALSE;
	/*	Find shortest difference, or edit script.
		Requires backtracing and swapping */
	i = pThis->fpM.data[pThis->fpM.length - pThis->fpN.length].diag;
	do
	{
		diag = DiffDiagVector_At(pThis->diags, i);
		numDel = prevDiag.x - (diag->x + diag->length);
		numIns = prevDiag.y - (diag->y + diag->length);
		numSubs = WSMIN(numDel, numIns);
		/*size_t numMatch = diag->length;*/
		x = prevDiag.x; y = prevDiag.y;
		if (pThis->reverse)
		{
			WSSWAP(size_t, numDel, numIns);
			WSSWAP(size_t, x, y);
		}

		prevDiag = *diag;
		i = DiffDiagVector_At(pThis->diags, i)->next;

		if(numSubs)
		{
			newBlock.rightEnd = y;
			newBlock.rightStart = y - numIns + 1;
			newBlock.leftEnd = x;
			newBlock.leftStart = x - numDel + 1;
			newBlock.type = DIFFBLOCK_CHANGE;
		}
		else if(numIns)
		{
			newBlock.leftStart = newBlock.leftEnd = x;
			newBlock.rightEnd = y;
			newBlock.rightStart = y - numIns + 1;
			newBlock.type = DIFFBLOCK_ADD;
		}
		else if(numDel)
		{
			newBlock.rightEnd = newBlock.rightStart = y;
			newBlock.leftEnd = x;
			newBlock.leftStart = x - numDel + 1;
			newBlock.type = DIFFBLOCK_REMOVE;
		}
		else
			continue;
		if(blocks && numBlocksFilled != *outCount)
		{
			blocks[numBlocksFilled] = newBlock;
			++numBlocksFilled;
		}
		++numBlocks;
	} while(diag->hasNext);
	/* NP alg does back to front, flip it */
	if(numBlocksFilled)
		WSREVERSE(DIFFBLOCK, size_t, blocks, 0, numBlocksFilled - 1);
	*outCount = numBlocks;
}

/**
	Builds the vector with file positions for each line. Does
	not perform any stream input.
	@param pThis DIFF object
*/
diffbool Diff_BuildSeekVectors(DIFF* pThis)
{
	return pThis->diags != NULL;
}
#ifdef DIFFDEBUG
void Diff_Print(DIFF* pThis, DIFFBLOCK* blocks, size_t numBlocks)
{
	char* buffer = NULL; char* text = NULL;
	size_t buffersize = 0;
	size_t bufferlen;
	size_t iLeft, iRight;
	printf("  Base:   ");
	for (iLeft = 0; iLeft < numBlocks; ++iLeft)
	{
		DIFFBLOCK* block = &blocks[iLeft];
		if(block->type != DIFFBLOCK_ADD)
		{
			Stream_Seek(pThis->streams[0],
				pThis->lines[0].data[block->leftStart - 1].pos,
				STREAM_FROMSTART);
			Diff_GetLine(pThis, 0, &buffer, &buffersize, &bufferlen);
			text = DiffStringDupWithEnd(buffer, buffer + bufferlen);
		}
		switch (block->type)
		{
		case DIFFBLOCK_CHANGE:
			printf("\033[0m");
			printf("%u: %s", (unsigned int)block->leftStart, text);
			free(text);
			break;
		case DIFFBLOCK_ADD:
			printf("\033[0m");
			printf(" ");
			break;
		case DIFFBLOCK_REMOVE:
			printf("\033[41m");
			printf("%u: %s", (unsigned int)block->leftStart, text);
			free(text);
			break;
		default:
			break;
		}
	}
	printf("\033[0m\n");
	printf("  Change: ");
	for (iRight = 0; iRight < numBlocks; ++iRight)
	{
		DIFFBLOCK* block = &blocks[iRight];
		if(block->type != DIFFBLOCK_REMOVE)
		{
			Stream_Seek(pThis->streams[1],
				pThis->lines[1].data[block->rightStart - 1].pos,
				STREAM_FROMSTART);
			Diff_GetLine(pThis, 1, &buffer, &buffersize, &bufferlen);
			text = DiffStringDupWithEnd(buffer, buffer + bufferlen);
		}
		switch (block->type)
		{
		case DIFFBLOCK_CHANGE:
			printf("\033[43m");
			printf("%u: %s", (unsigned int)block->rightStart, text);
			free(text);
			break;
		case DIFFBLOCK_ADD:
			printf("\033[42m");
			printf("%u: %s", (unsigned int)block->rightStart, text);
			free(text);
			break;
		case DIFFBLOCK_REMOVE:
			printf("\033[0m");
			printf(" ");
			break;
		default:
			break;
		}
	}
	printf("\033[0m\n");
	if(buffersize)
		free(buffer);
}
#endif
/**
* Formats a date based on a custom format string, similar to strftime.
*
* This function takes a `WSDATE` structure representing a date and time and
* formats it into a string according to the format specifiers provided in `f`.
* The result is stored in the buffer `s`. The function supports a subset of the
* standard strftime format specifiers, allowing for flexible date formatting
* within the limitations of the `WSDATE` structure's fields.
*
* Supported format specifiers include:
* - `%%a`: Abbreviated weekday name (Sun through Sat).
* - `%%A`: Full weekday name (Sunday through Saturday).
* - `%%b`: Abbreviated month name (Jan through Dec).
* - `%%B`: Full month name (January through December).
* - `%%d`: Day of the month, zero-padded (01 through 31).
* - `%%p`: AM/PM designation for the given time.
* - `%%H`: Hour in 24-hour format, zero-padded (00 through 23).
* - `%%k`: Hour in 24-hour format, space-padded ( 0 through 23).
* - `%%I`: Hour in 12-hour format, zero-padded (01 through 12).
* - `%%l`: Hour in 12-hour format, space-padded ( 1 through 12).
* - `%%m`: Month as a decimal number, zero-padded (01 through 12).
* - `%%M`: Minute, zero-padded (00 through 59).
* - `%%S`: Second, zero-padded (00 through 59).
* - `%%Y`: Year with century.
* - `%%`: A literal `%` character.
*
* This function provides an extremely portable and cross-platform way to format
* dates in environments where `strftime` may not be available or when working
* with a custom date structure like `WSDATE`.
*
* @param pThis Pointer to a `WSDATE` structure containing the date to format.
* @param f The format string containing one or more of the specifiers listed
*          above.
* @param s The buffer to receive the formatted date string. It is not
*          allocated by this function, thus the size of this
*          buffer must be large enough to hold the resulting string.
* @return The length of the formatted string stored in `s`.
*/
size_t DiffFormatDate(WSDATE* pThis, const char* f, char* s)
{
	const char* szDays[] =
	{
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday"
	};
	const char* szMonths[] =
	{
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};
	char* origs = s;
	while(*f)
	{
		if (*f != '%')
			*s++ = *f++;
		else
		{
			switch(*++f)
			{
			case 'a':
				{
					const char* wday = szDays[pThis->dayOfWeek];
					size_t wdaylen = 3;
					WSMemoryCopy(s, wday, wdaylen);
					s += wdaylen;
					break;
				}
			case 'A':
				{
					const char* wday = szDays[pThis->dayOfWeek];
					size_t wdaylen = strlen(wday);
					WSMemoryCopy(s, wday, wdaylen);
					s += wdaylen;
					break;
				}

			case 'b':
				{
					const char* month = szMonths[pThis->month];
					size_t monthlen = 3;
					WSMemoryCopy(s, month, monthlen);
					s += monthlen;
					break;
				}
			case 'B':
				{
					const char* month = szMonths[pThis->month];
					size_t monthlen = strlen(month);
					WSMemoryCopy(s, month, monthlen);
					s += monthlen;
					break;
				}
			case 'd':
				{
					if(pThis->dayOfMonth < 10)
						*s++ = '0';
					s += DiffDigitToString(s, pThis->dayOfMonth);
					break;
				}
			case 'p':
				{
					if(pThis->hour > 11)
						WSMemoryCopy(s, "pm", 2);
					else
						WSMemoryCopy(s, "am", 2);
					s += 2;
					break;
				}
			case 'k':
			case 'H':
				{
					if(pThis->hour < 10)
					{
						if(*f == 'H')
							*s++ = '0';
						else
							*s++ = ' ';
					}
					s += DiffDigitToString(s, pThis->hour);
					break;
				}
			case 'l':
			case 'I':
				{
					unsigned char hour;
					if(pThis->hour > 12)
						hour = pThis->hour - 12;
					else if(pThis->hour == 0)
						hour = 12;
					else
						hour = pThis->hour;
					if(hour < 10)
					{
						if(*f == 'I')
							*s++ = '0';
						else
							*s++ = ' ';
					}
					s += DiffDigitToString(s, hour);
					break;
				}
			case 'm':
				{
					if(pThis->month + 1 < 10)
						*s++ = '0';
					s += DiffDigitToString(s, pThis->month + 1);
					break;
				}
			case 'M':
				{
					if(pThis->minute < 10)
						*s++ = '0';
					s += DiffDigitToString(s, pThis->minute);
					break;
				}
			case 'S':
				{
					if(pThis->second < 10)
						*s++ = '0';
					s += DiffDigitToString(s, pThis->second);
					break;
				}
			case 'Y':
				{
					s += DiffDigitToString(s, pThis->year + 1900);
					break;
				}
			case '%':
				*s++ = *f++;
				break;
			default:
				break;

			}
			++f;
		}
	}
	return s - origs;
}

#ifdef _MSC_VER
#pragma warning(push, 3)
#pragma warning(disable:4701)
#endif

/**
* Outputs the differences between two files.
*
* This function processes two files (or text streams) for differences and
* outputs the result to a specified output stream. The differences are
* determined by the `blocks` parameter, which contains information on differing
* segments identified by a previous comparison process. The output can be
* formatted according to various styles controlled by the `flags` parameter,
* such as unified, context, or traditional diff formats. Additionally, the
* function can output differences with a specified number of context lines
* around each change.
*
* The function resets and seeks to the start of the input streams
* associated with the `DIFF` object before generating output to ensure
* accurate line positions.
*
* File modification times and names are included in the output headers
* for context and unified diff formats to provide additional information
* about the compared files.
*
* This function is capable of generating different styles of diff output,
* including the traditional line-oriented format (`DIFF_OUTPUT_ED`),
* context format (`DIFF_OUTPUT_CONTEXT`), and unified format
* (`DIFF_OUTPUT_UNIFIED`). The specific format used is determined by the
* `flags` parameter.
*
* In the context and unified formats, modification times of the files
* (including timezone information) are formatted and included in the
* output to comply with standard diff output conventions.
*
* @param pThis Pointer to the `DIFF` object that contains configuration and
*              state for the diff operation, including input streams and
*              comparison options.
* @param outStream Stream to which the difference output will be written.
* @param flags Flags controlling the output format (`DIFF_OUTPUT_[XXX]` flags).
* @param contextlinecount Number of context lines to include around changes,
*                         for context or unified diff formats.
* @param fileAname Name of the first (left) file, used in headers of the diff
*                  output.
* @param fileBname Name of the second (right) file, used in headers of the diff
*                  output.
* @param blocks Array of `DIFFBLOCK` structures representing the differences
*               between the files.
* @param numBlocks The number of elements in the `blocks` array.
*/
#if !defined(DIFF_DISABLE_OUTPUT)
void Diff_Output(DIFF* pThis, MEMORYSTREAM* outStream, int flags,
	size_t contextlinecount,
	const char* fileAname, const char* fileBname,
	DIFFBLOCK* blocks, size_t numBlocks)
{
	register linenum_t astart;
	const char* fileAStartString;
	const char* fileBStartString;
	size_t modAtime, modBtime, nowtime, nowgmttime, gmtdeltapos, fracsecs;
	char* lineheader;
	WSDATE modAdate, modBdate;

	pThis->contextlinecount = contextlinecount;
	pThis->flags = (pThis->flags & (DIFF_COMPARE_IGNORECASE
						|DIFF_COMPARE_IGNOREWHITESPACE
						|DIFF_COMPARE_BYWORD
						|DIFF_COMPARE_NOUTF8)) | flags;

	/*
	 *	Tidies before processing
	 */
	Stream_Seek(pThis->streams[0], 0, STREAM_FROMSTART);
	Stream_Seek(pThis->streams[1], 0, STREAM_FROMSTART);

	/*
	 *	Now print it out
	 */
	if (!(pThis->flags & DIFF_OUTPUT_ED))
	{
		if (pThis->flags & (DIFF_OUTPUT_CONTEXT | DIFF_OUTPUT_UNIFIED))
		{
			if (pThis->flags & DIFF_OUTPUT_UNIFIED)
			{
				/* See "Detailed Description of Unified Format" in the diff
				 * manual
				 *
				 * which is kind of incomplete - here is a better
				 * description:
				 * http://www.artima.com/weblogs/viewpost.jsp?thread=164293
				 *
				 * --- [from-file] [from-file-mod-time]
				 * +++ [to-file] [to-file-mod-time]
				 */

				fileAStartString = "--- ";
				fileBStartString = "+++ ";
			}
			else
			{
				fileAStartString = "*** ";
				fileBStartString = "--- ";
			}


			/*
			 *	Should include ctime style dates after the file names, but
			 *	this would be non-trivial on OSK.  Perhaps there should be a
			 *	special case for stdin.
			 */
			lineheader = WSALLOC(char, 100);
			Diff_PutString(pThis, outStream, fileAStartString);
			Diff_PutString(pThis, outStream, fileAname);
			if(lineheader)
			{
				modAtime = WSFileModificationTimeWithFracSecs(fileAname, &fracsecs);
				nowtime = WSTimeGetLocalValue();
				nowgmttime = WSTimeGetGMTValue();
				WSTime_GetDate(modAtime, &modAdate);
				gmtdeltapos =  /* .%N after %H:%M:%S for nanoseconds */
					DiffFormatDate(&modAdate, " %Y-%m-%d %H:%M:%S.", lineheader);
				gmtdeltapos += UDigitToString(&lineheader[gmtdeltapos], fracsecs);
				lineheader[gmtdeltapos++] = ' ';
				if (nowtime > nowgmttime)
					lineheader[gmtdeltapos+0] = '+';
				else
					lineheader[gmtdeltapos+0] = '-';
				lineheader[gmtdeltapos+1] = '0';
				if (nowtime > nowgmttime)
					lineheader[gmtdeltapos+2] =
						(char)(((nowtime - nowgmttime) / (60*60)) + '0');
				else
					lineheader[gmtdeltapos+2] =
						(char)(((nowgmttime - nowtime) / (60*60)) + '0');
				lineheader[gmtdeltapos+3] = '0';
				lineheader[gmtdeltapos+4] = '0';
				lineheader[gmtdeltapos+5] = '\0';
				Diff_PutString(pThis, outStream, lineheader);
			}
			Diff_PutNewline(pThis, outStream);
			Diff_PutString(pThis, outStream, fileBStartString);
			Diff_PutString(pThis, outStream, fileBname);
			if(lineheader)
			{
				modBtime = WSFileModificationTimeWithFracSecs(
					fileBname,  &fracsecs);
				WSTime_GetDate(modBtime, &modBdate);
				gmtdeltapos =  /* .%N after %H:%M:%S for nanoseconds */
					DiffFormatDate(&modBdate, " %Y-%m-%d %H:%M:%S.", lineheader);
				gmtdeltapos += UDigitToString(&lineheader[gmtdeltapos], fracsecs);
				lineheader[gmtdeltapos++] = ' ';
				if (nowtime > nowgmttime)
					lineheader[gmtdeltapos+0] = '+';
				else
					lineheader[gmtdeltapos+0] = '-';
				lineheader[gmtdeltapos+1] = '0';
				if (nowtime > nowgmttime)
					lineheader[gmtdeltapos+2] =
						(char)(((nowtime - nowgmttime) / (60*60)) + '0');
				else
					lineheader[gmtdeltapos+2] =
						(char)(((nowgmttime - nowtime) / (60*60)) + '0');
				lineheader[gmtdeltapos+3] = '0';
				lineheader[gmtdeltapos+4] = '0';
				lineheader[gmtdeltapos+5] = '\0';
				Diff_PutString(pThis, outStream, lineheader);
				WSFREE(lineheader);
			}
			Diff_PutNewline(pThis, outStream);
		}

		/*
		 * Normal printout
		 */
		for(astart = 0; astart < numBlocks; ++astart)
		{
			DIFFBLOCK* theBlock = &blocks[astart];
			Diff_change(pThis, outStream, theBlock->type,
				theBlock->leftStart, theBlock->leftEnd,
				theBlock->rightStart, theBlock->rightEnd);
		}
	}
	else
	{
		/*
		 * Edit script output -- differences are output "backwards" for the
		 * benefit of a line-oriented editor.
		 */
		for(astart = numBlocks; astart != 0; --astart)
		{
			DIFFBLOCK* theBlock = &blocks[astart - 1];
			Diff_change(pThis, outStream, theBlock->type,
				theBlock->leftStart, theBlock->leftEnd,
				theBlock->rightStart, theBlock->rightEnd);
		}
	}
	/*if (pThis->lines[0].length == 0) TODO: Don't think this is needed
		Diff_change(pThis, outStream, DIFFBLOCK_ADD,
					1, 0, 1, pThis->lines[1].length);*/
}
#endif /* !DIFF_DISABLE_OUTPUT */
#ifdef _MSC_VER
#pragma warning(pop)
#endif

/******************************************************************************
*						Self-Contained Test App								  *
******************************************************************************/
#ifdef DIFFSELFCONTAINEDEXAMPLE
void cant(
	char			*filename,
	const char		*what,
	int				fatalflag)
{
	fprintf(stderr, "Can't open %s file \"%s\": ", what, filename);
	perror((char*) NULL);
	if (fatalflag) {
		exit(fatalflag);
	}
}

#ifdef vms
opendir(which, arg, okfd)
	int             which;		/* Which file to open (0 or 1)  	 */
	char          **arg;		/* File name argument, &argv[which]  */
	FILE           *okfd;		/* File name (already open)  	 */
{
	register char  *tp;
	register int    c;
	register char  *newname;

	fgetname(okfd, text);

	/*
	 * Skip over device name
	 */
	for (tp = text; (c = *tp) && c != ':'; tp++);
	if (c)
		tp++;
	else
		tp = text;

	/*
	 * Skip over [UIC] or [PPN] if present
	 */
	if (*tp == '[' || *tp == '(') {
		while ((c = *tp++) && c != ']' && c != ')');
		if (c == 0) {
			fprintf(stderr, "?Bug: bad file name \"%s\"\n",
					text);
			tp--;
		}
	}
	strcpy(text, tp);

	/*
	 * Don't include version
	 */
	for (tp = text; (c = *tp) && c != ';'; tp++);
	*tp = 0;

	/*
	 * Now, text has the file name, tp - text, its length, and *arg the
	 * (possible) directory name.  Create a new file name for opening.
	 */
#ifndef	OSK
	if ((newname = malloc(tp - text + strlen(*arg) + 1)) == NULL)
		error("Out of space at start");
#ifdef	AMIGA
	savsiz = tp - text + strlen(*arg) + 1;
	savptr = newname;
#endif /* AMIGA */
#else /* OSK */
	newname = myalloc(tp - text + strlen(*arg) + 1, "Out of space at start");
#endif /* OSK */
	concat(newname, *arg, text, NULL);
	if ((infd[which] = fopen(newname, "r")) == NULL)
		cant(*arg, "constructed input", 1);
	else
		*arg = newname;
}
/* Amiga C doesn't have all these extensions for directory... */
#endif /* vms */

/**
* Entry point for the diff program.
*
* Man page from the original public domain diff with the old
* Hunt/McIlroy algortihm follows (note that none of the code is used,
* however the demonstration program functions the same and the comments
* are extremely useful for understanding diff development and algorithms):
*
* diff
*
* ---------------------------
* SYNOPSIS
* ---------------------------
* diff [-b/-c/-i/-e] file1 file2
*
* ---------------------------
* DESCRIPTION
* ---------------------------
* diff compares two files, showing what must be changed to make them
* identical. Either file1 or file2 (but not both) may refer to
* directories. If that is the case, a file in the directory whose
* name is the same as the other file argument will be used. The
* standard input may be used for one of the files by replacing the
* argument by "-". Except for the standard input, both files must
* be on disk devices.
*
* ---------------------------
* OPTIONS
* ---------------------------
* -b
*
* Remove trailing whitespace (blanks and tabs)
* and compress all other strings of whitespace to a single blank.
*
* -c
*
* Print some context -- matching lines before
* and after the non-match section.  Mark non-matched sections
* with "|".
*
* -i
*
* Ignore lower/upper case distinctions.
*
* -e
*
* Output is in an "editor script" format which
* is compatible with the Unix 'ed' editor.
*
* ---------------------------
* OPERATION
* ---------------------------
* All information needed to compare the files is maintained in
* main memory. This means that very large files (or fairly
* large files with many differences) will cause the program to
* abort with an "out of space" message. Main memory
* requirements (in words) are approximately:
*
* 2 * (length of file1 + length of file2) + 3 * (number of changes)
*
* (Where "length" is the number of lines of data in each file.)
*
* The algorithm reads each file twice, once to build hash
* tables and once to check for fortuitous matches (two lines
* that are in fact different, but which have the same hash
* value). CPU time requirements include sorting the hash
* tables and randomly searching memory tables for equivalence
* classes. For example, on a time-shared VAX-11/780, comparing
* two 1000 line files required about 30 seconds (elapsed clock
* time) and about 10,000 bytes of working storage. About 90
* per-cent of the time was taken up by file I/O.
*
* ---------------------------
* DIAGNOSTICS
* ---------------------------
* Warning, bad option 'x'
*
* The option is ignored.
*
* Usage ...
*
* Two input files were not specified.
*
* Can't open input file "filename".
*
* Can't continue.
*
* Out of space
* The program ran out of memory while comparing the two files.
*
* Can't read line nnn at xxx in file[A/B]
*
* This indicates an I/O error when seeking to the specific line. It should not
* happen.
*
* Spurious match, output is not optimal.
*
* Two lines that were different yielded the same hash value. This is harmless
* except that the difference output is not the minimum set of differences
* between the two files. For example, instead of the output:
*
* lines 1 to 5 were changed to ...
*
* the program will print
*
* lines 1 to 3 were changed to ...
*
* lines 4 to 5 were changed to ...
*
* The program uses a CRC16 hash code. The likelihood of this error is quite
* small.
*
* ---------------------------
* AUTHOR
* ---------------------------
* The diff algorithm was developed by J. W. Hunt and M. D. McIlroy, using a
* central algorithm defined by H. S. Stone. It was published in:
*
* Hunt, J. W., and McIlroy, M. D.,
*
* An Algorithm for Differential File Comparison,
*
* Computing Science Technical Report #41,
*
* Bell Laboratories, Murray Hill, NJ  07974
*
* ---------------------------
* BUGS
* ---------------------------
* On RSX and DECUS C on VMS systems, diff may fail if the both
* files are not "variable-length, implied carriage control"
* format.  The scopy program can be used to convert files to
* this format if problems arise.
*
* When compiled under VAX C, diff handles STREAM_LF files
* properly (in addition to the canonical variable-length
* implied carriage control files). Other variations should
* work, but have not been tested.
*
* When compiled under VAX C, diff is quite slow for unknown
* reasons which ought to be investigated. On the other hand,
* it has access to effectively unlimited memory.
*
* Output in a form suitable for ed - the -e option - seems
* rather pointless; the analogue on DEC systems is SLP (SUMSLP
* on VMS). It would be simple to provide SLP-compatible
* output. The question is, why bother - since the various DEC
* file comparison utilities already produce it.
*
* ---------------------------
* ALGORITHM
* ---------------------------
	@verbatim
	Diff maintains all information needed to compare the two files in
	main memory. This means that very large files (or fairly large
	files with many differences) will cause the program to abort with
	an "out of space" error. Main memory requirements (in words) are
	approximately:

		2 * (length of file1 + length of file2) + (3 * number of changes)

	The diff algorithm reads each file twice (once to build hash
	tables and a second time to check for fortuitous matches), then
	reads the differences by seeking randomly within the files. CPU
	time requirements include sorting the two hash vectors and
	randomly searching memory tables for equivalence classes. For
	example, running in Vax compatibility mode, two 1000 line files
	with a fair number of differences took about 25 seconds (elapsed
	wall clock time) for processing. Most of this time was spent in
	the file read routines. This test required slightly more than
	6000 words of memory for internal tables.

	The diff algorithm was developed by J. W. Hunt and M. D. McIlroy,
	using a central algorithm defined by H. S. Stone. The algorithm
	was described in:

		Hunt, J. W., and McIlroy, M. D.,
		An Algorithm for Differential File Comparison,
		Computing Science Technical Report #41,
		Bell Laboratories, Murray Hill, NJ  07974

	The following description is summarized from that document. While
	it has been slightly modified to correspond to the program
	source, the algorithm is essentially identical.

	1. Read the input files, building two vectors containing the line
	number (serial) and hash value (hash) of each line. Data for
	fileA will be in a vector pointed to by fileA[], while data for
	fileB will be pointed to by fileB[]. The lengths (number of
	lines) of the files will be represented by lenA and lenB
	respectively. [This is slightly different from the published
	algorithm.]

	2. Note initial and final sequences that have identical hash
	values to shorten subsequent processing. Note that the "jackpot"
	phase (step 9.) will examine all lines in the file. Next, sort
	each file using hash as the primary key and serial as the
	secondary key.

	3. Construct an array of equivalence classes (member[1..lenB])
	where each element contains the line number in fileB and a flag
	which is True if the indicated line is the first member of an
	equivalence class. (An equivalence class is a set of lines which
	all hash to the same value. The lines themselves are not
	necessarily identical.) Note that in wikiserver the flag is the
	"serial" field of LINE.

	4. Construct an array, class[1..lenA], where each element, I, is
	set to the index of a line, J, in fileB if member[J] is the first
	element in an equivalence class and the hash code of line[I] in
	fileA is the same as the hash code of line[J] in fileB. Class[I]
	is set to zero if no such line exists.

	If non-zero, class[I] now points in member[] to the beginning of
	the class of lines in fileB equivalent to line[I] in fileA.

	The next two steps implement the longest common subsequence
	algorithm.

	5. Structure CANDIDATE { a, b, previous }, where a and b are line
	numbers and previous a reference to a candidate, will store
	candidate lists as they are constructed.

	Vector clist[] stores references to candidates. It is dimensioned
	(0..min(lenA, lenB) + 1)
		Initialize
			clist[0] = CANDIDATE {   0,   0, -1 };
			clist[1] = CANDIDATE { A+1, B+1, -1 };
			ktop = 1;
	clist[1] is a fence beyond the last usefully filled element and
	-1 is an out-of-range clist index. Ktop is the index of the
	fence. Note, because of the memory allocation used, clist[] is
	actually composed of two vectors, clist[] containing the
	candidate reference, and klist[] containing pointers to clist.

	6.  For (A = 1 to lenA) {
	I = class[A];  -- the index in member[]:  beginning of
			-- the class of lines in fileB equivalent
			-- to this line in fileA.
	if (I is non-zero) {
		Merge each member into the candidate list
		as discussed below.
	}

	Unravel the chain of candidates, getting a vector of common
	subsequences:

	7.  Set all elements of match[0..lenA] to zero.

	8. clist[ktop-1] points to the subsequence chain head. For each
	element in the chain, let A and B be the line number entries.
	Then, set

		match[A] = B;

	The non-zero elements of match[] now pick out a longest common
	subsequence chain, possibly including spurious matches due to
	hash coincidences. The pairings between the two files are:

	if (match[A] is non-zero) {
		line A in fileA matches line match[A] in fileB;
	}

	Now, read each line of fileA and fileB to check for jackpots:

	9.  for (A = 1 to lenA) {
		if (match[A] is nonzero) {
			if (fileA[A] is not identical to fileB[match[A]])
				match[A] = 0;  -- Hash congruence
		}
	}

	Ignoring "squish" complications, the merge step may be defined as
	follows:

	Entry:
		clist[]		Candidate pointer array
		ktop		Fence beyond last filled index
		A			Current index in fileA
		member[]	Equivalence vector
		I			The index in member[] of the first element
					of the class of lines in fileB that are
					equivalent to line[A] in fileA.

	1. Let clist[R] be "an r-candidate" and C a reference to the last
	candidate found, which will always be an r-candidate. clist[R]
	will be updated with this reference once the previous value of
	clist[R] is no longer needed. Initialize:

		R = 0; C = clist[0];

	2.  Do steps 3 through 6 repeatedly:

	3. set B to the line number in member[I]; search clist[R..ktop]
	for an element, clist[S], such that

		clist[S-1].b < B and clist[S].b >= B

	Note that clist[] is ordered on clist[].b so that binary search
	will work. The search algorithm used requires the two "fence"
	entries described above.

	If such an element is found, perform steps 4. and 5.

	4. Extend the longest common subsequence chain:

		If (clist[S].b > B) {
			clist[R] = C;
			R = S;
			C = candidate(A, B, clist[S - 1]);
		}

	5. Extend the number of subsequences, moving the fence:

		If (S == ktop) {
			clist[ktop + 1] = clist[ktop]
			ktop = ktop + 1;
			break out of step 2's loop;
		}

	6.  I = I + 1;
		if (member[I] is the first element in another class) {
			break out of step 2's loop;
		}
		else {
			continue at step 2.
		}

	7. clist[R] = C; exit merge subroutine.

	To illustrate vector contents, here is a sample run:

	File A:
		line 1
		line 2
		line 3
		line 4
		line 5 gets deleted
		line 6

	File B:
		line 1
		line 1.5 inserted
		line 2
		line 3 changed
		line 4
		line 6

	(For clarity, the "squish" step is omitted from the following)

	On entry to equiv() (after readin and sorting), the file[] vector
	is as follows (the first entry in each pair is the line number,
	the second is the hash value. Entries are sorted on hash value):

	FileA[] (1..lines in fileA):
		line   hash
		3 042400  6 043300  5 050026  1 102201  2 102701  4 103501
	FileB[] (1..lines in fileB):
		6 043300  2 045600  1 102201  3 102701  5 103501  4 147166


	After Equiv has processed file[]:

	FileA[] (1..lines in fileA):
		line value
		3 0  6 1  5 0  1 3  2 4  4 5
	Member[] (0..lines in fileB):
		0  -6  -2  -1  -3  -5  -4

	After unsort() has unwound fileB:

	Class[] (1 .. lines in fileA):
		3   4  0  5  0  1

	Within unravel(), match is built in the following order:
		match[6] := 6
		match[4] := 5
		match[2] := 3
		match[1] := 1

	Match[] (0 .. lines in fileA):
		0  1  3  0  5  0  6

	Output is as follows:
		1a2
		> line 1.5 inserted
		3c4
		< line 3
		---
		> line 3 changed
		5d5
		< line 5 gets deleted
	@endverbatim
*/
int main(int argc, char** argv)
{
	char* ap;
	int flags = 0;
	linenum_t contextlinecount = 0;
	int i;
	FILE* infd[2];
	DIFF diff;
	DIFFBLOCK* blocks;
	size_t numBlocks;
	/*char* myfiles[2] = {
		(char*)".." WSPATHSEPARATOR "tests" WSPATHSEPARATOR "diff" WSPATHSEPARATOR "URICPP.wik1",
		(char*)".." WSPATHSEPARATOR "tests" WSPATHSEPARATOR "diff" WSPATHSEPARATOR "URICPP.wik2"
	};*/
	while (argc > 1 && *(ap = argv[1]) == '-' && *++ap != '\0')
	{
		while (*ap != '\0')
		{
			switch ((*ap++))
			{
			case 'b':
				flags |= DIFF_COMPARE_IGNOREWHITESPACE;
				break;
			case 'u':
				flags |= DIFF_OUTPUT_UNIFIED;
				break;
			case 'c':
				flags |= DIFF_OUTPUT_CONTEXT;
				if (*ap > '0' && *ap <= '9')
					contextlinecount = *ap++ - '0';
				else
					contextlinecount = 3;
				break;
			case 'e':
				flags |= DIFF_OUTPUT_ED;
				break;
			case 'i':
				flags |= DIFF_COMPARE_IGNORECASE;
				break;
			case 'w':
				flags |= DIFF_COMPARE_BYWORD;
				break;
			case 'a':
				flags |= DIFF_COMPARE_BYCHAR;
				break;
			default:
				fprintf(stderr,
						"Warning, bad option '%c'\n",
						ap[-1]);
				break;
			}
		}

		argc--;
		argv++;
	}

/*#if 0*/
	if (argc != 3)
	{
		fprintf(stderr, "%s", "Usage: diff [-options] file1 file2");
		putc('\n', stderr);
		exit(1);
	}

/*#endif*/
	if ((flags & (DIFF_OUTPUT_UNIFIED | DIFF_OUTPUT_CONTEXT))
			== (DIFF_OUTPUT_UNIFIED | DIFF_OUTPUT_CONTEXT))
	{
		fprintf(stderr,
				"Warning, -u and -c are incompatible, -u supressed.\n");
		flags = DIFF_OUTPUT_CONTEXT;
	}

	if ((flags & (DIFF_OUTPUT_ED | DIFF_OUTPUT_CONTEXT))
			== (DIFF_OUTPUT_ED | DIFF_OUTPUT_CONTEXT))
	{
		fprintf(stderr,
				"Warning, -c and -e are incompatible, -c suppressed.\n");
		flags = DIFF_OUTPUT_ED;
		contextlinecount = 0;
	}
	argv++;
	#if 0 /* testing */
	argv = myfiles;
	#endif
	for (i = 0; i <= 1; i++)
	{
		infd[i] = fopen(argv[i], "rb");
		if (!infd[i])
			cant(argv[i], "input", 2);		/* Fatal error */
	}

	if (infd[0] == NULL && infd[1] == NULL)
	{
		cant(argv[0], "input", 0);
		cant(argv[1], "input", 1);
	}
#ifdef vms
	else if (infd[1] == NULL)
		opendir(1, &argv[1], infd[0]);
	else if (infd[0] == NULL)
		opendir(0, &argv[0], infd[1]);
#endif /* vms */

#if defined(_WIN32)
	flags |= DIFF_OUTPUT_WINDOWSNEWLINES;
#elif defined(macintosh)
	flags |= DIFF_OUTPUT_MACNEWLINES;
#endif
	Diff_Create(&diff, (MEMORYSTREAM*) infd[0], (MEMORYSTREAM*) infd[1]);
	Diff_Compare(&diff, flags);
	blocks = NULL; numBlocks = 0;
	Diff_FillBlocks(&diff, blocks, &numBlocks);
	blocks = (DIFFBLOCK*)malloc(sizeof(DIFFBLOCK) * numBlocks);
	Diff_FillBlocks(&diff, blocks, &numBlocks);
	Diff_Output(&diff, (MEMORYSTREAM*)stdout,
				flags,/*|DIFF_OUTPUT_UNIFIED,*/
				contextlinecount,
				argv[0], argv[1], blocks, numBlocks);
	free(blocks);
	Diff_Destroy(&diff);

	fclose(infd[0]);
	fclose(infd[1]);

	return 0;
}
#endif /* DIFFSELFCONTAINEDEXAMPLE */
/** @} */
#endif /* DIFF_IMPLEMENTATION */
