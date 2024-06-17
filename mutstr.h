/**
*	@file mutstr.h
*
*	#     #  #####  #     # #     # #######  #####  ####### ######
*	#  #  # #     # ##   ## #     #    #    #     #    #    #     #
*	#  #  # #       # # # # #     #    #    #          #    #     #
*	#  #  #  #####  #  #  # #     #    #     #####     #    ######
*	#  #  #       # #     # #     #    #          #    #    #   #
*	#  #  # #     # #     # #     #    #    #     #    #    #    #
*	 ## ##   #####  #     #  #####     #     #####     #    #     #
*
*	Single header file for basic string manipulation
*
*	###Instructions###
*
*	Define WSMUTSTR_IMPLEMENTATION in one source file for the implementation
*
*	###Example###
*	WSMUTSTR str;
*	WSMutStr_Create(&str);
*	WSMutStr_AssignCString(&str, "MyString");
*	WSMutStr_Destroy(&str);
*
*	###Notes###
*	Always call BeforeWrite() and AfterWrite[XXX]() between manipulating
*	the internal buffer of WSMUTSTR.
*
*	WSMUTSTR explicitly does not handle UTFXXX and only raw data
*
*	###BeforeWrite() et al.###
*
*	StrBuf/Vector etc. have a function called BeforeWrite which is called
*	before passing its data buffer to a function. This is due to the STL
*	and the way reserve, erase, and resize work. Reserve allocs memory
*	for the internal buffer; if you just reserve() with a empty container
*	before a function, like GetModuleFileNameA() or Socket::Read(), then
*	use erase or resize to set the length of the container (std::string etc.),
*	then all the data you read into the data buffer up to that length is wiped
*	out with the element's default value such as 0 for char in std::string.
*
*	std::string pathName;
*	pathName.reserve(128);
*	pathName.erase(GetModuleFileName(NULL, (char*)pathName.c_str(), 128));
*
*	Above data is read to pathName, but because a length hasn't been set yet,
*	the erase() call wipes out all elements up to the number you set.
*
*	You can't call erase() on an object without an allocated buffer (via
*	resize() or reserve()), and resize() BOTH allocates the container and
*	sets its length. The STL provides no optimal way to just reserve data then
*	set a container's length later without wiping out its data.
*
*	Thus BeforeWrite() which in STL mode calls resize() as it can't be avoided,
*	but without STL just allocates the buffer without setting the length.
*
*	SetLength() is another similar situation where I want to set the length of a
*	container without allocating its buffer, this does that.
*
*	@todo List
*	- Better API for manipulating internal data
*	- Construct/Destruct?
*
*	@version 0.1
*
*	Version History
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
#ifndef __MUTSTR_H__
#define __MUTSTR_H__

#include <stddef.h> /* size_t */
#if !defined(WSBOOL_DEFINED)
	typedef int wsbool;
	#if !defined(TRUE)
		#define TRUE 1
		#define FALSE 0
	#endif
	#define WSBOOL_DEFINED
#endif /* WSBOOL_DEFINED */
#ifndef WSMAXLENGTH
	#define WSMAXLENGTH (size_t)(-1)
#endif /* WSMAXLENGTH */
#ifndef WSASSERT
	#ifdef _DEBUG
		#include <assert.h>
		#define WSASSERT(x) assert(x)
		#define WSRELEASEUNUSED(x)
	#else
		#define WSASSERT(x)
		#define WSRELEASEUNUSED(x) WSUNUSED(x)
	#endif
#endif /* WSASSERT */

#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------------------------------------------------------
						Digit->String
-----------------------------------------------------------------------------*/
size_t UDigitToString(char* dest, size_t digit);
size_t DigitToString(char* dest, ptrdiff_t digit);
#define DigitToStringMaxChars() (21) /* -9223372036854775807 '\0' */
size_t HexToString(char* dest, size_t digit);
#define HexToStringMaxChars() (17) /* 7FFFFFFFFFFFFFFF '\0' */
/*-----------------------------------------------------------------------------
						C Characters
-----------------------------------------------------------------------------*/
char CharToLower(char c);

/*-----------------------------------------------------------------------------
						C String
-----------------------------------------------------------------------------*/
#define CStrSwapChar(x, y) { char tx = x; x = y; y = tx; }
void CStrCopy(char*, const char*);
void CStrCopyWithLength(char*, const char*, size_t);
void CStrAppend(char*, const char*);
size_t CStrLength(const char* sz);
size_t CStrFind(const char* szData, size_t nLen,
	const char* Value, size_t nValueLen, size_t nStart);
size_t CStrFindICase(const char* szData, size_t nLen,
	const char* Value, size_t nValueLen, size_t nStart);
const char* CStrLocateChar(const char* s, char c);
const char* CStrLocateCharWithEnd(const char* s, const char* send, char c);
const char* CStrLocateCharWithLength(const char* s, char c, size_t len);
size_t CStrrfind(const char* search, size_t searchlen,
	const char* what, size_t pos, size_t len);
/* SubString==VBScript Mid */
char* CStrSubString(const char* s, size_t dwPos, size_t dwCount);
void CStrRemove(char* s, size_t len, size_t where, size_t amount);

/*-----------------------------------------------------------------------------
					C String Comparison
-----------------------------------------------------------------------------*/
#define COMPARE_CASE	0
#define COMPARE_NOCASE	1
int AnsiiCompare(const char* s1, const char* s2, size_t s1len, size_t s2len);
int AnsiiCompareICase(const char* s1, const char* s2,
	size_t s1len, size_t s2len);
wsbool AnsiiStartsWith(const char* s1, const char* s2, size_t s2len);
wsbool AnsiiStartsWithICase(const char* s1, const char* s2, size_t s2len);
#define ANSIICOMPARELITERAL(s1, s1end, s2) \
	((size_t)(s1end - s1) == strcountof(s2) \
		&& !AnsiiCompare(s1, s2, strcountof(s2), strcountof(s2)))
#define ANSIICOMPARELITERALPARTIAL(s1, s1end, s2) \
	((size_t)(s1end - s1) >= strcountof(s2) \
		&& !AnsiiCompare(s1, s2, strcountof(s2), strcountof(s2)))
#define ANSIICOMPARELITERALICASE(s1, s1end, s2) \
	((size_t)(s1end - s1) == strcountof(s2) \
		&& !AnsiiCompareICase(s1, s2, strcountof(s2), strcountof(s2)))
#define ANSIICOMPARELITERALPARTIALICASE(s1, s1end, s2) \
	((size_t)(s1end - s1) >= strcountof(s2) \
		&& !AnsiiCompareICase(s1, s2, strcountof(s2), strcountof(s2)))
#define AnsiiCmp(s1, s2) AnsiiCompare(s1, s2, WSMAXLENGTH, WSMAXLENGTH)
#define AnsiiNCmp(s1, s2, l) AnsiiCompare(s1, s2, l, l)

/*-----------------------------------------------------------------------------
					WSMUTSTR - C String Buffer
-----------------------------------------------------------------------------*/
#ifndef WSMutStr_AsStr
#define WSEMPTYCSTRING ""
#if !defined(WSSTR_DEFINED)
	typedef struct WSSTR
	{
		char* mData;
		size_t mLength;
	} WSSTR;
	#define WSSTR_DEFINED
#endif
#if defined(__cplusplus)
typedef struct WSMUTSTR : public WSSTR
{
	size_t mSize;
} WSMUTSTR;
#else
typedef struct WSMUTSTR
{
	char* mData;
	size_t mLength;
	size_t mSize;
} WSMUTSTR;
#endif
#define WSStr_ConstructWithLiteral(pThis, data) \
	(pThis)->mData = (char*)data; (pThis)->mLength = strcountof(data);
#define WSStr_Construct(pThis, data, length) \
	(pThis)->mData = (char*)data; (pThis)->mLength = length;
#define WSStr_IsEqual(pThis, pOther) ((pThis)->mLength == (pOther)->mLength \
	&& WSMemoryCompare((pThis)->mData, (pOther)->mData, (pThis)->mLength) == 0)
#define WSStr_GetData(pThis) (pThis)->mData
#define WSStr_GetLength(pThis) (pThis)->mLength
#define WSStr_FindCStringWithPos(pThis, toFind, toFindLen, startPos) \
	CStrFind(WSStr_GetData(pThis), WSStr_GetLength(pThis), \
		toFind, toFindLen, startPos)
#define WSStr_FindCStringWithPosICase(pThis, toFind, toFindLen, startPos) \
	CStrFindICase(WSStr_GetData(pThis), WSStr_GetLength(pThis), \
		toFind, toFindLen, startPos)
#define WSStr_Find(pThis, pToFind) \
	WSStr_FindCStringWithPos(pThis, WSStr_GetData(pToFind),\
		WSStr_GetLength(pToFind), 0)
#define WSStr_FindWithPos(pThis, pToFind, startPos) \
	WSStr_FindCStringWithPos(pThis, WSStr_GetData(pToFind),\
		WSStr_GetLength(pToFind), startPos)
#define WSStr_FindICase(pThis, pToFind) \
	WSStr_FindCStringWithPosICase(pThis, WSStr_GetData(pToFind),\
		WSStr_GetLength(pToFind), 0)
#define WSStr_FindICaseWithPos(pThis, pToFind, startPos) \
	WSStr_FindCStringWithPosICase(pThis, WSStr_GetData(pToFind),\
		WSStr_GetLength(pToFind), startPos)

#define WSMutStr_AsStr(pThis) ((WSSTR*)(pThis))
#define WSMutStr_AppendStr(pThis, pOther) \
	WSMutStr_AppendCStringWithLength(pThis, \
		pOther->mData, pOther->mLength);
#define WSMutStr_Append(pThis, pOther) \
	WSMutStr_AppendCStringWithLength(pThis, \
		WSMutStr_GetCString(pOther), WSMutStr_GetLength(pOther));
#define WSMutStr_AppendCStringLiteral(pThis, cstrlit) \
	WSMutStr_AppendCStringWithLength(pThis, cstrlit, strcountof(cstrlit));
#define WSMutStr_At(pThis, i) (pThis)->mData[i]
#define WSMutStr_SetChar(pThis, i, c) (pThis)->mData[i] = c
void WSMutStr_Create(WSMUTSTR* pThis);
void WSMutStr_CreateCopy(WSMUTSTR* pThis, WSMUTSTR* pOther);
void WSMutStr_CreateFromCString(WSMUTSTR* pThis, const char* s);
void WSMutStr_CreateFromCStringWithLength(WSMUTSTR* pThis,
	const char* s, size_t len);
#define WSMutStr_CreateFromCStringLiteral(pThis, cstrlit) \
	WSMutStr_CreateFromCStringWithLength(pThis, cstrlit, strcountof(cstrlit));
void WSMutStr_CreateFromBuffer(WSMUTSTR* pThis,
	char* buf, size_t len, size_t size);
void WSMutStr_CreateWithSize(WSMUTSTR* pThis, size_t dwSize);
void WSMutStr_Destroy(WSMUTSTR* pThis);
char* WSMutStr_Disown(WSMUTSTR* pThis);
void WSMutStr_Erase(WSMUTSTR* pThis, size_t pos);
void WSMutStr_EraseRange(WSMUTSTR* pThis, size_t pos, size_t amount);
size_t WSMutStr_GetLength(const WSMUTSTR* pThis);
size_t WSMutStr_GetCapacity(const WSMUTSTR* pThis);
const char* WSMutStr_GetCString(const WSMUTSTR* pThis);
wsbool WSMutStr_IsCreated(WSMUTSTR* pThis);
void WSMutStr_Take(WSMUTSTR* pThis, char* buf, size_t len, size_t size);
void WSMutStr_Assign(WSMUTSTR* pThis, const WSMUTSTR* pOp);
void WSMutStr_AssignCString(WSMUTSTR* pThis, const char* cstr);
void WSMutStr_AssignCStringWithLength(WSMUTSTR* pThis,
	const char* cstr, size_t cstrlen);
#define WSMutStr_AssignCStringLiteral(pThis, cstrlit) \
	WSMutStr_AssignCStringWithLength(pThis, cstrlit, strcountof(cstrlit));
void WSMutStr_AppendChar(WSMUTSTR* pThis, char c);
void WSMutStr_AppendCString(WSMUTSTR* pThis, const char* cstr);
void WSMutStr_AppendCStringWithLength(WSMUTSTR* pThis,
	const char* cstr, size_t cstrlen);
void WSMutStr_AppendDigit(WSMUTSTR* pThis, ptrdiff_t digit);
void WSMutStr_AppendUDigit(WSMUTSTR* pThis, size_t digit);
int WSMutStr_CompareCString(WSMUTSTR* pThis, const char* s);
void WSMutStr_InsertCStringWithLength(WSMUTSTR* pThis,
	size_t where, const char* s, size_t slen);
void WSMutStr_BeforeWrite(WSMUTSTR* pThis, size_t bufsize);
void WSMutStr_AfterWrite(WSMUTSTR* pThis, size_t newlength);
void WSMutStr_AfterWriteAndNullTerminate(WSMUTSTR* pThis, size_t newlength);
void WSMutStr_ReplaceSingle(WSMUTSTR* pThis, size_t where, size_t remove,
	const char* s, size_t nStringLength);
void WSMutStr_Replace(WSMUTSTR* pThis, const char* what, size_t whatlen,
	const char* where, size_t wherelen,
	size_t startpos, size_t amount);
void WSMutStr_Reserve(WSMUTSTR* pThis, size_t bufsize);
void WSMutStr_Resize(WSMUTSTR* pThis, size_t bufsize);
void WSMutStr_SetLength(WSMUTSTR* pThis, size_t newlength);
#endif /* WSMutStr_AsStr */
#ifdef __cplusplus
} /* end extern "C" */
#endif
#endif /* __MUTSTR_H__ */
#if defined(WSMUTSTR_IMPLEMENTATION)
#if !defined(WSMemoryCompare) && !defined(WIKISERVER_VERSION)
	#include <string.h>
	#define WSMemoryCompare memcmp
	#define WSMemoryFind memchr
	#define WSMemoryFill memset
	#define WSMemoryMove memmove
	#define WSMemoryCopy memcpy
#endif
#if !defined(WSALLOC)
	#include <stdlib.h>
	#define WSALLOC(type, count) (type*)malloc(sizeof(type) * (count))
	#define WSALLOC1(x) (x*)malloc(sizeof(x))
	#define WSREALLOC(old, type, count) \
		(type*)realloc(old, sizeof(type) * (count))
	#define	WSFREE(x) free((void*)(x))
#endif /* WSALLOC */
#if !defined(STRCHRRET)
	#if defined(_MSC_VER) && ((_MSC_VER < 1300) || !defined(__cplusplus))
		#define MEMCHRRET void*
		#define STRCHRRET char*
	#else
		#define MEMCHRRET const void*
		#define STRCHRRET const char*
	#endif
#endif /* STRCHRRET */
/*-----------------------------------------------------------------------------
					C Characters
-----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**
* @defgroup StringCompare String Comparison
* @{
*/
/*
* Compares two strings up to a specified number of characters, optionally
* ignoring case sensitivity.
*
* This function compares the strings `s1` and `s2` up to the first `nNum1`
* and `nNum2` characters, respectively. It supports case-sensitive or
* case-insensitive comparison based on the `ignore_case` flag. The comparison
* stops at the first differing character or when the end of either string
* segment is reached. Strings are considered equal if they are identical up to
* the shorter of the two lengths specified. If one string is a prefix of the
* other, the shorter string is considered to be "less" than the longer one.
*
* If `nNum1` or `nNum2` is set to `WSMAXLENGTH`, the comparison does not
*       stop at `nNum1` or `nNum2` characters and continues until a null
*       terminator or a difference is found.
*
* `WSASSERT` is used to ensure `s1` and `s2` are not NULL unless their
*       respective lengths (`nNum1`, `nNum2`) are 0, preventing undefined
*       behavior.
*
* @param s1 First string to compare.
* @param s2 Second string to compare.
* @param ignore_case If TRUE, comparison is case-insensitive. If FALSE,
*        comparison is case-sensitive.
* @param nNum1 Maximum number of characters to compare from `s1`.
* @param nNum2 Maximum number of characters to compare from `s2`.
* @return 0 if the string segments are equal, -1 if `s1` is less than `s2`,
*         or 1 if `s1` is greater than `s2`.
*/
int AnsiiCompare_(const char* s1, const char* s2, wsbool ignore_case,
	size_t nNum1, size_t nNum2)
{
	size_t nPos = 0;
	wsbool e1, e2;

	WSASSERT(s1 || nNum1 == 0);
	WSASSERT(s2 || nNum2 == 0);

	while (nPos < nNum1 && nPos < nNum2) {
		char c1 = s1[nPos];
		char c2 = s2[nPos];

		if (c1 == '\0' && nNum1 == WSMAXLENGTH)
			break;
		if (c2 == '\0' && nNum2 == WSMAXLENGTH)
			break;

		if (ignore_case)
		{
			c1 = CharToLower(c1);
			c2 = CharToLower(c2);
		}

		if (c1 != c2)
			return (c1 < c2) ? -1 : 1;

		++nPos;
	}

	e1 = (nPos == nNum1 || (nNum1 == WSMAXLENGTH && s1[nPos] == '\0'));
	e2 = (nPos == nNum2 || (nNum2 == WSMAXLENGTH && s2[nPos] == '\0'));

	if (e1 && e2)
		return 0;
	else if (e1)
		return -1;
	else if (e2)
		return 1;

	return 0;
}
/**
* Performs a case-sensitive comparison of two ANSI strings up to specified
* lengths.
*
* This function compares the strings `s1` and `s2` up to the first `nNum1`
* and `nNum2` characters, respectively. The comparison
* stops at the first differing character or when the end of either string
* segment is reached. Strings are considered equal if they are identical up to
* the shorter of the two lengths specified. If one string is a prefix of the
* other, the shorter string is considered to be "less" than the longer one.
*
* If `nNum1` or `nNum2` is set to `WSMAXLENGTH`, the comparison does not
* stop at `nNum1` or `nNum2` characters and continues until a null
* terminator or a difference is found.
*
* `WSASSERT` is used to ensure `s1` and `s2` are not NULL unless their
* respective lengths (`nNum1`, `nNum2`) are 0, preventing undefined
* behavior.
*
* @param s1 First string to compare.
* @param s2 Second string to compare.
* @param nNum1 Maximum number of characters to compare from `s1`.
* @param nNum2 Maximum number of characters to compare from `s2`.
* @return 0 if the string segments are equal, -1 if `s1` is less than `s2`,
*         or 1 if `s1` is greater than `s2`.
*/
int AnsiiCompare(const char* s1, const char* s2, size_t s1len, size_t s2len)
{	return AnsiiCompare_(s1, s2, FALSE, s1len, s2len);	}

/**
* Performs a case-insensitive comparison of two ANSI strings up to specified
* lengths.
*
* This function compares the strings `s1` and `s2` up to the first `nNum1`
* and `nNum2` characters, respectively. The comparison
* stops at the first differing character or when the end of either string
* segment is reached. Strings are considered equal if they are identical up to
* the shorter of the two lengths specified. If one string is a prefix of the
* other, the shorter string is considered to be "less" than the longer one.
*
* If `nNum1` or `nNum2` is set to `WSMAXLENGTH`, the comparison does not
* stop at `nNum1` or `nNum2` characters and continues until a null
* terminator or a difference is found.
*
* `WSASSERT` is used to ensure `s1` and `s2` are not NULL unless their
* respective lengths (`nNum1`, `nNum2`) are 0, preventing undefined behavior.
*
* @param s1 First string to compare.
* @param s2 Second string to compare.
* @param nNum1 Maximum number of characters to compare from `s1`.
* @param nNum2 Maximum number of characters to compare from `s2`.
* @return 0 if the string segments are equal, -1 if `s1` is less than `s2`,
*         or 1 if `s1` is greater than `s2`.
*/
int AnsiiCompareICase(const char* s1, const char* s2,
	size_t s1len, size_t s2len)
{	return AnsiiCompare_(s1, s2, TRUE, s1len, s2len);	}

/**
* Checks if the first ANSI string starts with the second string,
* case-sensitive.
* @param s1 String to check for prefix.
* @param s2 Prefix to compare against.
* @param s2len Length of the prefix string to consider.
* @return TRUE if `s1` starts with `s2`, FALSE otherwise.
*/
wsbool AnsiiStartsWith(const char* s1, const char* s2, size_t s2len)
{	return AnsiiCompare_(s1, s2, FALSE, s2len, s2len) == 0;	}

/**
* Checks if the first ANSI string starts with the second string, ignoring case.
* @param s1 String to check for prefix.
* @param s2 Prefix to compare against.
* @param s2len Length of the prefix string to consider.
* @return TRUE if `s1` starts with `s2` regardless of case, FALSE otherwise.
*/
wsbool AnsiiStartsWithICase(const char* s1, const char* s2, size_t s2len)
{	return AnsiiCompare_(s1, s2, TRUE, s2len, s2len) == 0;	}
/**
* @}
*/
#ifdef __cplusplus
} /* end extern "C" */
#endif
/*-----------------------------------------------------------------------------
					C Characters
-----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**
* Converts an uppercase character to lowercase.
*
* If the input character `c` is an uppercase letter (A-Z), it is converted to
* its lowercase equivalent (a-z). If `c` is not an uppercase letter, it is
* returned unchanged.
*
* @param c The character to convert.
* @return The lowercase version of `c` if it is an uppercase letter; otherwise,
*         `c` itself.
*/
char CharToLower(char c)
{ return ((c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c); }
#ifdef __cplusplus
} /* end extern "C" */
#endif

/*-----------------------------------------------------------------------------
					Digit->String
-----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**
* @defgroup ANSIIConvert ANSII Manipulation Functions
* @{
*/

/**
* Converts an unsigned integer to its string representation and stores it in
* a provided buffer.
*
* This function takes an unsigned integer `digit` and converts it to its
* decimal string representation, storing the result in the buffer pointed to
* by `dest`. The conversion is done in reverse order; the digits are extracted
* from least significant to most significant, and then the resulting string
* is reversed to reflect the correct order. If `digit` is 0, the function
* simply sets the first character of `dest` to '0' and null-terminates the
* string. The function ensures that the resulting string is null-terminated
* before returning.
*
* The function implements an efficient in-place algorithm to convert the
* number to a string and reverse it without needing an additional buffer.
*
* `CStrSwapChar` is used to swap characters during the reversal of the string.
*
* @param dest Pointer to the buffer where the resulting null-terminated
* string will be stored. The buffer must be large enough to hold the resultant
* string that is never over DigitToStringMaxChars() in length (thus never over
* (DigitToStringMaxChars() + 1) in size).
* @param digit The unsigned integer to convert to a string.
*
* @return The length of the string representation of `digit` (not including
* the null terminator).
*
* @see DigitToString
*/
size_t UDigitToString(char* dest, size_t digit)
{
	char* odest; char* edest; char* indest;
	if (!digit)
	{
		*dest++ = (char)('0');
		*dest = '\0';
		return 1;
	}
	odest = indest = dest;
	do
	{
		*dest++ = ((char)('0' + digit % 10));
		digit /= 10;
	} while (digit);
	edest = --dest;
	while (odest < edest) /* reverse string */
	{
		CStrSwapChar(*odest, *edest);
		++odest; --edest;
	}
	*++dest = '\0';
	return dest - indest;
}

/**
* Converts a signed integer to its string representation and stores it in a
* provided buffer.
*
* This function handles both positive and negative integers by utilizing the
* `UDigitToString` function for the conversion of the absolute value of the
* integer to its decimal string representation. If the input integer `digit`
* is negative, the function prepends a '-' character to the string
* representation and then converts the absolute value of `digit` using
* `UDigitToString`. For positive numbers, it directly calls `UDigitToString`
* without any modifications. The conversion ensures that the resulting string
* is null-terminated.
*
* This function seamlessly handles the conversion of negative numbers
* by converting them to their positive counterpart and then invoking
* `UDigitToString` for the conversion process.
*
* @param dest Pointer to the buffer where the resulting null-terminated
* string will be stored. The buffer must be large enough to hold the resultant
* string that is never over DigitToStringMaxChars() in length (thus never over
* (DigitToStringMaxChars() + 1) in size).
* @param digit The signed integer to convert to a string.
*
* @return The length of the string representation of `digit` (not including
* the null terminator). If `digit` is negative, the length includes the '-'
* character.
*
* @see UDigitToString
*/
size_t DigitToString(char* dest, ptrdiff_t digit)
{
	if(digit < 0)
	{
		*dest++ = '-';
		digit = -digit;
		return UDigitToString(dest, digit) + 1;
	}
	return UDigitToString(dest, digit);
}
/**
* Converts an unsigned integer to its hexadecimal string representation and
* stores it in a provided buffer.
*
* This function takes an unsigned integer `digit` and converts it to its
* hexadecimal string representation, storing the result in the buffer pointed
* to by `dest`. The conversion is performed by repeatedly dividing the number
* by 16 and using the remainder as an index into a string of hexadecimal
* digits. The process continues until `digit` becomes 0. If `digit` is
* initially 0, the function simply sets the first character of `dest` to '0'
* and null-terminates the string. After the conversion, the function reverses
* the string in place to ensure the digits are in the correct order. The
* resulting string is null-terminated.
*
* The function implements an efficient in-place algorithm for the
* conversion and reversal of the string without needing an additional buffer.
*
* @param dest Pointer to the buffer where the resulting null-terminated
* string will be stored. The buffer must be large enough to hold the resultant
* string that is never over HexToStringMaxChars() in length (thus never over
* (HexToStringMaxChars() + 1) in size).
* @param digit The unsigned integer to convert to a hexadecimal string.
*
* @return The length of the string representation of `digit`
* (not including the null terminator).
*/
size_t HexToString(char* dest, size_t digit)
{
	const char* hdig = "0123456789abcdef";
	char* odest; char* edest; char* indest;
	if (!digit)
	{
		*dest++ = (char)('0');
		*dest = '\0';
		return 1;
	}
	odest = indest = dest;
	*dest++ = (hdig[digit % 16]);
	digit /= 16;
	while (digit)
	{
		*dest++ = (hdig[digit % 16]);
		digit /= 16;
	}
	edest = --dest;
	while (odest < edest) /* reverse string */
	{
		CStrSwapChar(*odest, *edest);
		++odest; --edest;
	}
	*++dest = '\0';
	return dest - indest;
}
/** @} */
#ifdef __cplusplus
} /* end extern "C" */
#endif
/*-----------------------------------------------------------------------------
					CStr (C)
-----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/**
* Appends a source string to the end of a destination string.
*
* This function copies the characters from `src` including the null terminator
* to the end of `dest`, effectively concatenating the two strings. The caller
* must ensure that `dest` has enough space to accommodate the additional
* characters.
*
* This function calculates the length of `dest` to determine the starting
* point for the copy operation, then uses `WSMemoryCopy` to perform the
* actual copying, including the null terminator from `src`.
*
* @param dest Pointer to the destination string to which `src` is appended.
* Must have enough space to hold the concatenated result.
* @param src Pointer to the source string that is appended to `dest`.
*/
void CStrAppend(char* dest, const char* src)
{ WSMemoryCopy(&dest[CStrLength(dest)], src, CStrLength(src) + 1); }
/**
* Copies a source string to a destination buffer.
*
* This function copies the null-terminated string pointed to by `in`, including
* the null terminator, to the buffer pointed to by `out`. The caller must
* ensure that the destination buffer is large enough to store the copied
* string.
*
* The function uses `WSMemoryCopy` to perform the copy operation. It
* calculates the length of the source string `in` to ensure the null
* terminator is also copied.
*
* @param out Pointer to the destination buffer where the string is copied.
* @param in Pointer to the null-terminated source string to be copied.
*/
void CStrCopy(char* out, const char* in)
{ WSMemoryCopy(out, in, CStrLength(in) + 1); }
/**
* Copies a specified number of characters from a source string to a
* destination.
*
* This function copies exactly `size` characters from the string pointed to by
* `in`, to the buffer pointed to by `out`. Unlike `CStrCopy`, it does not
* necessarily stop at a null terminator and does not append a null terminator
* unless included in the specified `size`.
*
* This function is useful for copying substrings or fixed-size string
* blocks. The caller is responsible for ensuring that `out` has enough
* space to accommodate `size` characters.
*
* @param out Pointer to the destination buffer where the characters are copied.
* @param in Pointer to the source string from which characters are copied.
* @param size The number of characters to copy from `in` to `out`.
*/
void CStrCopyWithLength(char* out, const char* in, size_t size)
{ WSMemoryCopy(out, in, size); }
/**
* Finds the first occurrence of a character in a string.
*
* This function searches for the first occurrence of the character `c` in the
* string `s` and returns a pointer to it. If `c` is not found, the function
* returns NULL.
*
* @param s Pointer to the string to be searched.
* @param c The character to search for.
* @return A pointer to the first occurrence of `c` in `s`, or NULL if `c` is
* not found.
*/
const char* CStrLocateChar(const char* s, char c)
{
#if defined(__MWERKS__) || defined(NOCRUNTIME)
	STRCHRRET strchr(const char* s, int c);
#endif
	return strchr(s, c);
}
/**
* Calculates the length of a null-terminated string.
*
* This function returns the number of characters in the string pointed to by
* `sz`, not including the null terminator. It wraps the standard `strlen`
* function and provides a consistent interface for obtaining the length of a
* string.
*
* @param sz Pointer to the null-terminated string whose length is calculated.
* @return The length of the string, in characters, without counting the null
*         terminator.
*/
size_t CStrLength(const char* sz)
{
#if defined(NOCRUNTIME) && defined(_MSC_VER)
	size_t strlen(const char* s);
#endif
	return strlen(sz);
}
const char* CStrLocateCharWithLength(const char* s, char c, size_t len)
{
	return (const char*)WSMemoryFind(s, c, len);
}
const char* CStrLocateCharWithEnd(const char* s, const char* send, char c)
{
	return (const char*)WSMemoryFind(s, c, send - s);
}

char* CStrSubString(const char* s, size_t dwPos, size_t dwCount)
{
	char* ret = WSALLOC(char, dwCount + 1);
	if(ret)
	{
		WSMemoryCopy(&ret[0], &s[dwPos], dwCount*sizeof(char));
		ret[dwCount] = '\0';
	}
	return ret;
}

size_t StringFindSingle(const char* mData, size_t mLength,
	const char* Value, size_t nStart, wsbool bICase)
{
	if (bICase)
	{
		size_t i;
		char targetchar = CharToLower(*Value);
		for (i = nStart; i < mLength; ++i)
		{
			if (CharToLower(mData[i]) == targetchar)
				return i;
		}
	}
	else
	{
		const char* res = (const char*)
			WSMemoryFind(&mData[nStart], *Value, mLength - nStart);
		if(res)
			return res - mData;
	}
	return WSMAXLENGTH;
}


size_t StringFind(const char* mData, size_t mLength,
	const char* Value, size_t nValLen,
	size_t nStart, wsbool bICase)
{
	size_t nPos;
	WSASSERT(mLength != WSMAXLENGTH);
	WSASSERT(nValLen != WSMAXLENGTH);
	if (nValLen)
		nPos = StringFindSingle(mData, mLength, Value, nStart, bICase);
	else
		nPos = WSMAXLENGTH;
	if (nPos == WSMAXLENGTH)
		return WSMAXLENGTH;
	while (nPos < mLength &&
		(nPos + nValLen > mLength || AnsiiCompare_(&mData[nPos], Value,
			bICase, nValLen, nValLen) != 0)) /* reanchor until found */
		nPos = StringFindSingle(mData, mLength, Value, ++nPos, bICase);
	if (nPos >= mLength)
		return WSMAXLENGTH;
	else
		return nPos;
}
size_t CStrFind(const char* mData, size_t mLength,
	const char* Value, size_t nValLen,
	size_t nStart)
{ return StringFind(mData, mLength, Value, nValLen, nStart, FALSE); }
size_t CStrFindICase(const char* mData, size_t mLength,
	const char* Value, size_t nValLen,
	size_t nStart)
{ return StringFind(mData, mLength, Value, nValLen, nStart, TRUE); }

void CStrRemove(char* mData, size_t mLength, size_t nWhere, size_t nAmount)
{
	size_t nLength;
	WSASSERT(mLength + nWhere >= nAmount);
	nLength = mLength - nAmount;
	WSMemoryMove(&mData[nWhere], &mData[nWhere+nAmount],
		(nLength - nWhere) * sizeof(char));
	mData[nLength] = '\0';
}

size_t CStrrfind(const char* mData, size_t mLength,
	const char* what, size_t pos, size_t len)
{
	const char* haystack;
	if(pos == WSMAXLENGTH)
		pos = mLength;
	if(len == WSMAXLENGTH)
		len = CStrLength(what);
	if(len > mLength)
		return WSMAXLENGTH;
	haystack = mData + mLength - len;
	do
	{
		if(WSMemoryCompare(haystack, what, len) == 0)
			return haystack - mData;
		--haystack;
	} while(haystack != mData - 1);
	return WSMAXLENGTH;
}
#ifdef __cplusplus
} /* end extern "C" */
#endif
/**
* @defgroup WSMUTSTR WSMutStr
* @{
*/
#ifdef __cplusplus
extern "C" {
#endif

/** Extra padding WSMUTSTR uses every allocation */
#define WSMUTSTR_EXTRASIZE    10

/* Internal function to null terminate string */
void WSMutStr_NullTerminate(WSMUTSTR* pThis, size_t pos)
{
	pThis->mData[pos] = '\0';
}

/**
* Allocates or reallocates memory for a mutable string to a specified size.
*
* This function adjusts the memory allocated for a mutable string (`WSMUTSTR`)
* object to accommodate a new size. It attempts to resize the memory buffer
* to hold `newsize` characters plus additional space defined by
* `WSMUTSTR_EXTRASIZE`, allowing for efficient string manipulation by reducing
* the frequency of memory reallocations. If the current size (`mSize`) is zero,
* indicating that no memory is allocated, it sets the data pointer (`mData`)
* to NULL before attempting reallocation.
*
* The function uses `WSREALLOC` to resize the memory buffer, which may
*       move the buffer to a new memory location if necessary. If reallocation
*       fails, it frees the current buffer using `WSFREE` and resets the length
*       and size of the string object to zero, effectively rendering the object
*       empty.
*
* `WSMUTSTR_EXTRASIZE` is a predefined macro that specifies additional
*       space allocated beyond `newsize` to minimize the need for further
*       memory operations during subsequent modifications to the string.
*
* This function directly modifies the mutable string object's internal
*       state, including its data buffer, length, and size attributes. It's
*       designed to be used as part of the implementation details of mutable
*       string handling and should be used with caution to maintain data
*       integrity.
*
* @param pThis Pointer to the mutable string object.
* @param newsize The new size to which the string should be resized, excluding
*        any extra space for operational efficiency.
*/
void WSMutStr_Alloc(WSMUTSTR* pThis, size_t newsize)
{
	char* newData; char* oldData;
	if (!pThis->mSize)
	{
		oldData = pThis->mData; /* data wasn't alloced but is still copied */
		pThis->mData = NULL;
	}

	newData = (char*)
		WSREALLOC(pThis->mData, char, WSMUTSTR_EXTRASIZE + newsize);
	if(!newData)
	{
		WSFREE(pThis->mData);
		pThis->mLength = pThis->mSize = 0;
	}
	else
	{
		if(!pThis->mSize) /* copy over old non-allocated data */
			WSMemoryCopy(newData, oldData,
				sizeof(char) * (pThis->mLength + 1));
		pThis->mSize = WSMUTSTR_EXTRASIZE + newsize;
	}
	pThis->mData = newData;
}

/**
* Initializes a mutable string object to an empty state.
*
* This function sets up a `WSMUTSTR` object, preparing it for use by assigning
* its data buffer to point to a predefined empty string constant and setting
* both its length and allocated size to zero. The operation ensures the mutable
* string is in a valid state, ready to store text data.
*
* The use of `WSEMPTYCSTRING` allows the `mData` field to point to a
*       read-only empty string, avoiding unnecessary memory allocation for
*       an empty mutable string. This approach is efficient for scenarios
*       where a `WSMUTSTR` object may not immediately receive content.
*
* Initializing a `WSMUTSTR` object with this function is essential before
*       performing any operations on the object, as it ensures that the
*		internal state of the object is correctly set up to prevent undefined
*		behavior.
*
* The function explicitly sets `mLength` and `mSize` to 0, indicating
*       that the string currently contains no characters and no memory has been
*       dynamically allocated for character data.
*
* @param pThis Pointer to the mutable string object to be initialized.
*/
void WSMutStr_Create(WSMUTSTR* pThis)
{
	pThis->mData = (char*)WSEMPTYCSTRING;
	pThis->mLength = pThis->mSize = 0;
}

/**
* Initializes a mutable string object as a copy of another mutable string.
*
* This function creates a new mutable string (`WSMUTSTR`) object `pThis` and
* initializes it with the content of another mutable string `pOther`. It
* ensures that `pThis` is an exact copy of `pOther` in terms of the string
* content.
*
* This operation involves creating a new mutable string for `pThis`
*       and then assigning it the content of `pOther`. The original content
*       and state of `pOther` are not modified.
*
* @param pThis Pointer to the mutable string object to be initialized.
* @param pOther Pointer to the mutable string object to copy from.
*/
void WSMutStr_CreateCopy(WSMUTSTR* pThis, WSMUTSTR* pOther)
{
	WSMutStr_Create(pThis);
	WSMutStr_AssignCStringWithLength(pThis, pOther->mData, pOther->mLength);
}

/**
* Initializes a mutable string object with a C-style string.
*
* This function creates a new mutable string (`WSMUTSTR`) object `pThis` and
* initializes it with the content of a null-terminated C-style string `s`.
* The content of `s` is copied into the mutable string.
*
* The function ensures that `pThis` contains a copy of the string `s`.
*       The mutable string object is initialized, and its content is set
*       to the content of `s`.
*
* @param pThis Pointer to the mutable string object to be initialized.
* @param s Pointer to the null-terminated C-style string to copy from.
*/
void WSMutStr_CreateFromCString(WSMUTSTR* pThis, const char* s)
{
	WSMutStr_Create(pThis);
	WSMutStr_AssignCString(pThis, s);
}

/**
* Initializes a mutable string object with a segment of a C-style string.
*
* This function creates a new mutable string (`WSMUTSTR`) object `pThis` and
* initializes it with a specific segment of a C-style string `s`, defined by
* `len` characters starting from `s`. The specified segment is copied into
* the mutable string.
*
* The function initializes `pThis` and assigns it the specified segment
*       of `s`. It is useful for creating a mutable string from a substring
*       or a specific portion of a larger string.
*
* @param pThis Pointer to the mutable string object to be initialized.
* @param s Pointer to the C-style string segment to copy from.
* @param len Number of characters to copy from `s`.
*/
void WSMutStr_CreateFromCStringWithLength(WSMUTSTR* pThis,
	const char* s, size_t len)
{
	WSMutStr_Create(pThis);
	WSMutStr_AssignCStringWithLength(pThis, s, len);
}
/**
* Initializes a mutable string object with a pre-allocated buffer, length,
* and size.
*
* This function sets up a `WSMUTSTR` object using an existing buffer `s`,
* directly assigning it as the mutable string's data storage. It allows the
* specification of the buffer's current content length (`len`) and its total
* allocated size (`size`), facilitating the management of strings that may
* have been created or manipulated outside the standard mutable string
* functions.
*
* This initialization method is particularly useful when integrating
* with code that manipulates strings directly, allowing for efficient
* reuse of existing buffers without requiring data copying.
*
* If `len` is set to `WSMAXLENGTH`, the function calculates the length
* of the string in `s` up to the first null terminator, making it easier
* to initialize the mutable string with null-terminated strings without
* pre-calculating their length.
*
* Care must be taken when using this function to avoid buffer overflows
* or underflows, as the mutable string functions may modify the content
* based on the specified `len` and `size`.
*
* @param pThis Pointer to the mutable string object to be initialized.
* @param s Pointer to the pre-allocated character buffer to be used as the
* mutable string's data. It is the caller's responsibility to ensure that the
* buffer `s` is appropriately sized and allocated for the intended use. The
* mutable string object does not attempt to resize or reallocate this buffer.
* @param len The length of the content currently in the buffer. If set to
* `WSMAXLENGTH`, the length is determined automatically using `CStrLength`.
* @param size The total allocated size of the buffer `s`.
*/
void WSMutStr_CreateFromBuffer(WSMUTSTR* pThis,
	char* s, size_t len, size_t size)
{
	pThis->mData = s;
	pThis->mLength = len == WSMAXLENGTH ? CStrLength(s) : len;
	pThis->mSize = size;
}

/**
* Initializes a mutable string with a specified size.
*
* This function prepares a mutable string object for use, setting its initial
* size as specified by `dwSize`. It allocates memory to hold a string of length
* `dwSize`, but initializes the string to be empty. The function ensures that
* the string data is non-NULL by setting it to point to a constant empty string
* before allocation. This is a safety measure to prevent undefined behavior in
* case `WSMutStr_Alloc` fails or `dwSize` is zero. After allocating memory, the
* function explicitly sets the first character of `mData` to the
* null terminator to mark the string as empty.
*
* The `WSMutStr_CreateWithSize` function is typically used to initialize a
* `WSMUTSTR` object before using it to store or manipulate strings. By
* allocating memory upfront, it helps manage memory more efficiently for
* scenarios where the size of the string data is known in advance or can be
* estimated.
*
* The function does not return a value. It assumes `pThis` is a valid
* pointer to a `WSMUTSTR` structure. The behavior of the function is
* dependent on the successful allocation of memory; if `WSMutStr_Alloc`
* fails to allocate memory, the `mData` field will remain pointing to an
* empty string constant, and `mLength` and `mSize` will be zero.
*
* @param pThis Pointer to the `WSMUTSTR` object to be initialized.
* @param dwSize The desired initial size (capacity) for the string, not
* including the null terminator. The actual allocated size may be larger to
* accommodate the null terminator and any implementation-specific overhead.
*/
void WSMutStr_CreateWithSize(WSMUTSTR* pThis, size_t dwSize)
{
	pThis->mData = (char*)WSEMPTYCSTRING;
	pThis->mLength = pThis->mSize = 0;
	WSMutStr_Alloc(pThis, dwSize);
	WSMutStr_NullTerminate(pThis, 0);
}
/**
* Frees the memory allocated for the data buffer of a mutable string object.
*
* This function releases the dynamically allocated memory used by the data
* buffer of a `WSMUTSTR` object, if any. It is designed to be called when a
* mutable string is no longer needed, to prevent memory leaks. The function
* checks if the mutable string has an allocated data buffer (`mSize` is
* non-zero) before attempting to free it, ensuring safe operation even if
* called multiple times or on uninitialized objects.
*
* After calling this function, the data buffer of the mutable string
*       object is deallocated, but the `WSMUTSTR` structure itself is not
*       freed. If the `WSMUTSTR` was dynamically allocated, it is the caller's
*       responsibility to free the structure itself if necessary.
*
* This function does not reset the internal state of the `WSMUTSTR`
*       object (e.g., setting `mData` to NULL or `mSize` to 0), so the object
*       should not be used after calling this function without reinitializing.*
*
* It is safe to call this function on a `WSMUTSTR` object that has not
*       been allocated or has already been destroyed, making it robust against
*       multiple invocations.
*
* @param pThis Pointer to the mutable string object to be destroyed.
*/
void WSMutStr_Destroy(WSMUTSTR* pThis)
{
	if(pThis->mSize)
		WSFREE(pThis->mData);
}
/**
* Detaches and returns the internal data buffer of a mutable string object,
* leaving the object in an empty state.
*
* This function allows the caller to take ownership of the internal data buffer
* of a `WSMUTSTR` object without copying, effectively transferring the buffer's
* ownership outside the mutable string management. After the call, the mutable
* string object is reset to an empty state, with its internal data pointer set
* to point to a predefined empty string constant (`WSEMPTYCSTRING`). This
* operation is useful for efficiently retrieving the string data for use
* elsewhere while avoiding memory duplication.
*
* This approach provides an efficient mechanism for mutable string
*       objects to relinquish their data buffers, facilitating scenarios where
*       mutable strings are used to build or process string data that is then
*       passed on for further use.
*
* @param pThis Pointer to the mutable string object.
* @return Pointer to the detached data buffer previously managed by the mutable
*         string object. The caller is responsible for managing this memory,
*         including freeing it with WSFREE() when no longer needed.
*/
char* WSMutStr_Disown(WSMUTSTR* pThis)
{
	char* data = pThis->mData;
	pThis->mData = (char*)WSEMPTYCSTRING;
	pThis->mSize = pThis->mLength = 0;
	return data;
}
/**
* Erases the content of a mutable string object from a specified position to
* the end.
*
* This function truncates the mutable string represented by `WSMUTSTR` at the
* specified position `pos`, effectively removing all characters from `pos`
* onwards. The operation adjusts the string's length to `pos` and ensures the
* string is properly null-terminated afterwards. It is a convenient way to
* shorten a string or remove unwanted trailing content without reallocating
* or modifying the existing data buffer up to `pos`.
*
* The function leverages `WSMutStr_SetLength` to adjust the string's length,
* which internally ensures that the `mLength` attribute of the `WSMUTSTR`
* object is updated correctly and does not exceed the allocated buffer size.
*
* Care should be taken to ensure `pos` is within the valid range of the
*       string's current length to avoid unintended behavior.
*
* This operation does not reduce the allocated memory size of the mutable
*       string; it only adjusts the perceived length of the string content.
*
* @param pThis Pointer to the mutable string object to be modified.
* @param pos The position at which to truncate the string. Characters at and
*        beyond this position are removed. If `pos` is beyond the end of the
*        string, this function has no effect.
* @see WSMutStr_EraseRange() Variation to efficiently erase more than once
*/
void WSMutStr_Erase(WSMUTSTR* pThis, size_t pos)
{
	WSMutStr_SetLength(pThis, pos);
	WSMutStr_NullTerminate(pThis, pThis->mLength);
}

/**
* Erases a specified range of characters from a mutable string object
* similar to std::string::erase().
*
* This function removes a sequence of characters of length `amount` from the
* mutable string `pThis`, starting at index `pos`. It moves the subsequent part
* of the string forward to fill the gap created by the removal, effectively
* reducing the string's length by `amount`. The operation ensures that the
* string remains properly null-terminated after the modification.
*
* The function first checks if the mutable string object has been
*       properly initialized (`WSMutStr_IsCreated`) and that the operation
*       does not attempt to remove more characters than exist beyond `pos`.
*
* `WSMemoryMove` is used to shift the remaining part of the string
*       to the left, starting from the index `pos + amount` to the end of
*       the string, to the position specified by `pos`, effectively erasing
*       the specified range of characters.
*
* The `mLength` attribute of the `WSMUTSTR` object is updated to reflect
*       the new length of the string after the range has been erased.
*
* This operation modifies the string in place and can be used to remove
*       unwanted sections of text, such as specific words or characters, from
*       a mutable string.
*
* @param pThis Pointer to the mutable string object to be modified.
* @param pos The starting index from which characters will be removed.
* @param amount The number of characters to remove starting from `pos`.
*
* @see WSMutStr_Erase() Variation for a single character
*/
void WSMutStr_EraseRange(WSMUTSTR* pThis, size_t pos, size_t amount)
{
	WSASSERT(WSMutStr_IsCreated(pThis)
			 && pThis->mLength + pos >= amount);
	if(WSMutStr_IsCreated(pThis))
	{
		size_t newlength = pThis->mLength - amount;
		WSMemoryMove(&pThis->mData[pos],
			&pThis->mData[pos+amount], (newlength - pos) * sizeof(char));
		pThis->mLength = newlength;
		WSMutStr_NullTerminate(pThis, pThis->mLength);
	}
}

/**
* Retrieves the current length of a mutable string.
*
* This function returns the length of the string stored in the `WSMUTSTR`
* structure, which is the number of characters in the string excluding the null
* terminator. The length is stored in the `mLength` member of the `WSMUTSTR`
* structure. This function provides a way to access the length directly,
* allowing for efficient string length checks without needing to compute the
* length manually or access the structure's fields directly.
*
* This function assumes `pThis` is a valid pointer to an initialized
* `WSMUTSTR` structure. It does not perform any null-pointer checks.
*
* @param pThis Pointer to the `WSMUTSTR` object whose length is to be
* retrieved.
* @return The length of the string contained in the `WSMUTSTR` object, measured
*         in characters without including the null terminator.
*/
size_t WSMutStr_GetLength(const WSMUTSTR* pThis)
{
	return pThis->mLength;
}
/**
* Retrieves the current capacity of a mutable string.
*
* This function returns the capacity of the mutable string stored in the
* `WSMUTSTR` structure, which represents the total number of characters the
* string can hold without requiring reallocation. The capacity is stored in the
* `mSize` member of the `WSMUTSTR` structure and includes the space for the
* null terminator. This function facilitates understanding how much more data
* can be added to the string before it needs to be resized, allowing for
* optimized memory usage and potentially reducing the need for frequent
* reallocations.
*
* This function assumes `pThis` is a valid pointer to an initialized
* `WSMUTSTR` structure. It does not perform any null-pointer checks.
*
* @param pThis Pointer to the `WSMUTSTR` object whose capacity is to be
*              retrieved.
* @return The capacity of the `WSMUTSTR` object, measured in characters,
*         including the space required for the null terminator.
*/
size_t WSMutStr_GetCapacity(const WSMUTSTR* pThis)
{
	return pThis->mSize;
}

/**
* Retrieves the C-string representation of a mutable string object.
*
* This function provides access to the internal buffer of a `WSMUTSTR` object
* as a C-style null-terminated string. It allows the content of the mutable
* string to be used with standard C string functions that expect a
* null-terminated char array.
*
* @param pThis Pointer to the mutable string object.
* @return A pointer to the null-terminated string representing the content
* of the mutable string object. The lifetime of the returned string is tied
* to the mutable string object it came from. It remains valid as long as
* the mutable string is not destroyed or modified to change its buffer.
*
* @note The returned string should not be modified directly unless
* WSMutStr_BeforeWrite() is called beforehand, followed by either
* WSMutStr_AfterWrite() or WSMutStr_AfterWriteAndNullTerminate() after the
* caller is done modifying the returned string.
*/
const char* WSMutStr_GetCString(const WSMUTSTR* pThis)
{
	return pThis->mData;
}

/**
* Checks if a mutable string has been successfully initialized.
*
* This function determines whether a `WSMUTSTR` object has been initialized by
* checking if its `mData` pointer is not NULL. A non-NULL `mData` indicates that
* memory has been allocated for the string data, suggesting that the `WSMUTSTR`
* object is ready for use. This check is crucial for preventing operations on
* uninitialized or improperly initialized string objects, which can lead to
* undefined behavior or crashes.
*
* This function assumes `pThis` is a valid pointer to a `WSMUTSTR`
* structure. It does not perform any null-pointer checks on `pThis` itself,
* and thus the caller must ensure that `pThis` points to a valid object.
*
* @param pThis Pointer to the `WSMUTSTR` object to be checked.
* @return Returns `true` if the `WSMUTSTR` object has been initialized (i.e.,
*         `mData` is not NULL), otherwise returns `false`.
*/
wsbool WSMutStr_IsCreated(WSMUTSTR* pThis)
{
	return pThis->mData != NULL;
}

/**
* Replaces a mutable string object's buffer with a new buffer, safely disposing
* of the old buffer.
*
* This function first destroys the existing buffer of the `WSMUTSTR` object
* to prevent memory leaks, then initializes the mutable string with a new
* buffer `buf`. It effectively transfers ownership of the new buffer to the
* mutable string object, setting its content length to `len` and its allocated
* size to `size`. This operation allows for efficient reuse or replacement of
* the mutable string's data with an externally managed buffer.
*
* This function is useful for cases where a mutable string needs to be quickly
* reset or replaced with new content without the overhead of copying data from
* one buffer to another.
*
* @param pThis Pointer to the mutable string object whose buffer is being
* replaced.
* @param buf Pointer to the new character buffer to be used by the mutable
* string object. It is the caller's responsibility to ensure that `buf` is
* dynamically allocated with WSALLOC() and can safely be managed (freed or
* reallocated) by the mutable string functions. The mutable string object
* will assume ownership of `buf`, meaning it will manage the memory for `buf`
* after this call.
* @param len The length of the content currently in `buf`. If the actual
* content is unknown, `len` can be set to `WSMAXLENGTH` to automatically
* determine the length using `CStrLength`.
* @param size The total allocated size of the new buffer `buf`.
*/
void WSMutStr_Take(WSMUTSTR* pThis, char* buf, size_t len, size_t size)
{
	WSMutStr_Destroy(pThis);
	WSMutStr_CreateFromBuffer(pThis, buf, len, size);
}

/**
* Assigns the value of one mutable string to another.
*
* This function copies the string content from the source `WSMUTSTR` object
* (`pOp`) to the destination `WSMUTSTR` object (`pThis`). It first resets the
* length of the destination string to 0, effectively clearing its current
* content. Then, it appends the entire content of the source string to the
* destination string using `WSMutStr_AppendCStringWithLength`. This approach
* ensures that the destination string exactly matches the source string after
* the assignment.
*
* The function assumes both `pThis` and `pOp` are valid pointers to
* initialized `WSMUTSTR` objects. The function does not perform any
* null-pointer checks on its arguments.
*
* @param pThis Pointer to the destination `WSMUTSTR` object.
* @param pOp Pointer to the source `WSMUTSTR` object.
*/
void WSMutStr_Assign(WSMUTSTR* pThis, const WSMUTSTR* pOp)
{
	pThis->mLength = 0;
	WSMutStr_AppendCStringWithLength(pThis,
		WSMutStr_GetCString(pOp),
		WSMutStr_GetLength(pOp)
	);
}
/**
* Assigns a C-string to a mutable string object.
*
* This function sets the content of the `WSMUTSTR` object (`pThis`) to the
* specified C-string (`cstr`). It first clears the current content of the
* mutable string by setting its length to 0. Then, it appends the new C-string
* content to the mutable string using `WSMutStr_AppendCString`. This ensures
* that the mutable string contains exactly the same characters as the provided
* C-string after the assignment.
*
* This function assumes `pThis` is a valid pointer to an initialized
* `WSMUTSTR` object and that `cstr` is a null-terminated string. The
* function does not perform any null-pointer checks on its arguments.
*
* @param pThis Pointer to the `WSMUTSTR` object to be modified.
* @param cstr Pointer to the C-string that will be assigned to `pThis`.
*/
void WSMutStr_AssignCString(WSMUTSTR* pThis, const char* cstr)
{
	pThis->mLength = 0;
	WSMutStr_AppendCString(pThis, cstr);
}
/**
* Assigns a C-string with a specified length to a mutable string object.
*
* Similar to `WSMutStr_AssignCString`, this function sets the content of the
* `WSMUTSTR` object (`pThis`) to the specified C-string (`cstr`). However, it
* allows for specifying the length (`len`) of the string to be assigned, which
* is useful for strings that may contain null characters or when the exact
* length is known in advance. The function clears the current content of the
* mutable string by setting its length to 0, then appends the specified portion
* of the C-string to the mutable string using
* `WSMutStr_AppendCStringWithLength`.
*
* This function assumes `pThis` is a valid pointer to an initialized
* `WSMUTSTR` object and that `cstr` points to a string of at least `len`
* characters. The function does not perform any null-pointer checks on
* its arguments.
*
* @param pThis Pointer to the `WSMUTSTR` object to be modified.
* @param cstr Pointer to the C-string that will be assigned to `pThis`.
* @param len The number of characters from `cstr` to be assigned.
*/
void WSMutStr_AssignCStringWithLength(WSMUTSTR* pThis,
	const char* cstr, size_t len)
{
	pThis->mLength = 0;
	WSMutStr_AppendCStringWithLength(pThis, cstr, len);
}
/**
* Appends a single character to a mutable string.
*
* This function appends the given character `c` to the end of the mutable
* string represented by `pThis`. If the current capacity (`mSize`) of the
* mutable string is insufficient to accommodate the new character, the function
* attempts to  allocate additional memory by doubling the current capacity. The
* appended character is followed by a null terminator to ensure the string
* remains properly null-terminated.
*
* This function checks if the mutable string has been successfully
* initialized (i.e., `mData` is not NULL) before attempting to append. It
* relies on `WSMutStr_Alloc` for memory allocation, which may change the
* `mData` pointer if reallocation is necessary.
*
* @param pThis Pointer to the `WSMUTSTR` object being modified.
* @param c The character to append to the mutable string.
*/
void WSMutStr_AppendChar(WSMUTSTR* pThis, char c)
{
	if (pThis->mLength + 1 >= pThis->mSize)
		WSMutStr_Alloc(pThis, (pThis->mLength + 1) << 1);
	if(WSMutStr_IsCreated(pThis))
	{
		pThis->mData[pThis->mLength] = c;
		WSMutStr_NullTerminate(pThis, ++pThis->mLength);
	}
}
/**
* Appends a null-terminated C-string to a mutable string.
*
* This function appends the entire content of the null-terminated C-string `s`
* to the mutable string represented by `pThis`. The function calculates the
* length of `s` using `CStrLength` and then delegates the actual appending
* operation to `WSMutStr_AppendCStringWithLength`, which handles memory
* allocation and string copying.
*
* The function assumes `s` is a valid, null-terminated string. It relies
* on the correctness of `CStrLength` for determining the length of `s`.
*
* @param pThis Pointer to the `WSMUTSTR` object being modified.
* @param s Pointer to the null-terminated C-string to append.
*/
void WSMutStr_AppendCString(WSMUTSTR* pThis, const char* s)
{
	WSMutStr_AppendCStringWithLength(pThis, s, CStrLength(s));
}
/**
* Appends a C-string with specified length to a mutable string.
*
* This function appends a portion of the C-string `s`, specified by `applen`,
* to the mutable string represented by `pThis`. If the current capacity
* (`mSize`) is insufficient to accommodate the new characters, it attempts
* to allocate additional memory by doubling the required capacity. The appended
* string segment is followed by a null terminator to maintain proper string
* termination.
*
* This function checks if the mutable string has been successfully
* initialized before appending. It relies on `WSMemoryCopy` for efficient
* memory copying and `WSMutStr_Alloc` for memory allocation. The function
* ensures the mutable string remains null-terminated after appending.
*
* @param pThis Pointer to the `WSMUTSTR` object being modified.
* @param s Pointer to the C-string to append.
* @param applen The number of characters from `s` to append.
*/
void WSMutStr_AppendCStringWithLength(WSMUTSTR* pThis,
	const char* s, size_t applen)
{
	if (pThis->mLength + applen >= pThis->mSize)
		WSMutStr_Alloc(pThis, (pThis->mLength + applen) << 1);
	if (WSMutStr_IsCreated(pThis))
	{
		WSMemoryCopy(&pThis->mData[pThis->mLength], s,
			(applen) * sizeof(char));
		pThis->mLength += applen;
		WSMutStr_NullTerminate(pThis, pThis->mLength);
	}
}

/**
* Appends a signed digit to the mutable string.
*
* This function converts a signed digit (`digit`), represented by
* a `ptrdiff_t`, into its string representation and appends it to the end
* of the mutable string (`pThis`). Before appending, it prepares the
* mutable string for modification by ensuring there is enough capacity,
* based on the maximum number of characters a digit could be converted
* into (`DigitToStringMaxChars`). If the mutable string is successfully
* initialized, it then calls `DigitToString` to perform the conversion and
* appends the result. The actual length of the mutable string is updated
* accordingly.
*
* Before writing, `WSMutStr_BeforeWrite` is called to ensure sufficient
* capacity. The function checks if the mutable string is initialized using
* `WSMutStr_IsCreated`. It relies on `DigitToString` for conversion, which
* should handle the sign of `digit` appropriately.
*
* @param pThis Pointer to the `WSMUTSTR` object being modified.
* @param digit The signed digit to append to the mutable string.
*/
void WSMutStr_AppendDigit(WSMUTSTR* pThis, ptrdiff_t digit)
{
	size_t len = pThis->mLength;
	WSMutStr_BeforeWrite(pThis, len + DigitToStringMaxChars());
	if(WSMutStr_IsCreated(pThis))
		WSMutStr_AfterWrite(pThis, len
			+ DigitToString((char*)&WSMutStr_GetCString(pThis)[len], digit));
}

/**
* Appends an unsigned digit to the mutable string.
*
* Similar to `WSMutStr_AppendDigit`, this function handles an unsigned digit
* (`digit`), converting it into its string representation and appending it to
* the mutable string (`pThis`). It ensures there is enough space for the
* conversion by considering the maximum number of characters required, as
* returned by `DigitToStringMaxChars`. Upon successful preparation, it uses
* `UDigitToString` for the conversion and updates the mutable string length
* based on the number of characters appended.
*
* This function ensures the mutable string can accommodate the new
* characters by calling `WSMutStr_BeforeWrite` before the operation.
* It verifies the mutable string's readiness with `WSMutStr_IsCreated`.
* The conversion is performed by `UDigitToString`, which should efficiently
* handle the conversion of `digit` to its string equivalent.
*
* @param pThis Pointer to the `WSMUTSTR` object being modified.
* @param digit The unsigned digit to append to the mutable string.
*/
void WSMutStr_AppendUDigit(WSMUTSTR* pThis, size_t digit)
{
	size_t len = pThis->mLength;
	WSMutStr_BeforeWrite(pThis, len + DigitToStringMaxChars());
	if(WSMutStr_IsCreated(pThis))
		WSMutStr_AfterWrite(pThis, len
			+ UDigitToString((char*)&WSMutStr_GetCString(pThis)[len], digit));
}

/**
* Compares the content of a mutable string with a C-string.
*
* This function compares the string content of a `WSMUTSTR` object (`pThis`)
* with a specified C-string (`s`). The comparison is performed using the
* `AnsiiCompare` function, which compares the two strings up to the length of
* the mutable string or until a null terminator is encountered in the C-string.
* This allows for a flexible comparison that accommodates strings of different
* lengths and content.
*
* The comparison is sensitive to the actual length of the mutable string
* (`pThis->mLength`) and does not necessarily extend to `WSMAXLENGTH` for
* the C-string unless required by the comparison logic. This design choice
* ensures that the function can correctly handle strings with embedded null
* characters.
*
* @param pThis Pointer to the `WSMUTSTR` object whose content is being
* compared.
* @param s Pointer to the C-string to compare against the content of `pThis`.
* @return The result of the comparison: a negative value if `pThis` is
* lexicographically less than `s`, zero if they are equal, and a positive
* value if `pThis` is greater. The specific comparison logic and return
* values are determined by the `AnsiiCompare` function.
*/
int WSMutStr_CompareCString(WSMUTSTR* pThis, const char* s)
{
	return AnsiiCompare(pThis->mData, s, pThis->mLength, WSMAXLENGTH);
}

/**
* Inserts a substring into a mutable string at a specified position.
*
* This function inserts a given number of characters from a C-string (`s`) into
* the mutable string (`pThis`) at the specified index (`where`). If `where` is
* set to `WSMAXSIZE`, the substring is inserted at the end of the mutable
* string. If `nStringLength` is `WSMAXLENGTH`, the function calculates the
* length of `s` using `CStrLength` to determine how many characters to insert.
* It ensures there is enough space in the mutable string to accommodate the new
* characters by reserving additional space. The existing characters starting
* from `where` are moved to make space for the new substring. The function
* handles memory allocation and ensures the string remains null-terminated
* after insertion.
*
* The function adjusts `pThis->mLength` to reflect the new length of the
* mutable string after insertion. It checks if the mutable string is
* initialized using `WSMutStr_IsCreated` before proceeding with the
* operation. The function ensures that the mutable string's capacity is
* sufficient to hold the new characters plus the null terminator.
*
* @param pThis Pointer to the `WSMUTSTR` object being modified.
* @param where The position at which to insert the substring. If `where` is
* beyond the current length of the string, the substring is appended
* at the end.
* @param s Pointer to the C-string from which characters are to be inserted.
* @param nStringLength The number of characters from `s` to insert. If set to
* `WSMAXLENGTH`, the function uses the length of `s`.
*/
void WSMutStr_InsertCStringWithLength(WSMUTSTR* pThis,
	size_t where, const char* s, size_t nStringLength)
{
	if (where == WSMAXLENGTH) where = pThis->mLength;
	if (nStringLength == WSMAXLENGTH) nStringLength = CStrLength(s);
	WSMutStr_Reserve(pThis, pThis->mLength + nStringLength + 1);
	if(WSMutStr_IsCreated(pThis))
	{
		if (pThis->mLength - where > 0)
			WSMemoryMove(&pThis->mData[where + nStringLength],
				&pThis->mData[where],
				(pThis->mLength - where) * sizeof(char));
		/*if (nStringLength > 0)*/ /*implicit*/
			WSMemoryCopy(&pThis->mData[where],
				s,
				(nStringLength) * sizeof(char));
		WSMutStr_NullTerminate(pThis, pThis->mLength + nStringLength);
		pThis->mLength += nStringLength;
	}
}
/**
* Prepares a mutable string for writing by ensuring enough allocated memory.
*
* Before modifying a `WSMUTSTR` object, this function checks and adjusts the
* allocated memory to accommodate a specified size. It's crucial when buffer
* sharing or reference counting might be affected by modifications, ensuring
* exclusive access to the buffer or sufficient space for new content.
*
* This is a wrapper for `WSMutStr_Reserve`, targeting memory management
* ahead of content changes. It prevents unintended effects on shared
* buffers by allocating additional space if needed.
*
* The function itself does not modify the string's content but prepares
* the buffer for safe modifications by ensuring it can hold the new data.
*
* @param pThis Pointer to the mutable string object.
* @param size Expected size of data post-modification.
*/
void WSMutStr_BeforeWrite(WSMUTSTR* pThis, size_t size)
{ WSMutStr_Reserve(pThis, size); }

/**
* Finalizes modifications to a mutable string by setting its new length
* after calling WSMutStr_BeforeWrite().
*
* This function is called after writing to or modifying the internal buffer of
* a `WSMUTSTR` object to update its length to reflect the new content size.
* It's particularly useful in scenarios where direct buffer manipulation
* occurs, ensuring the mutable string's metadata accurately represents its
* state post-operation.
*
* Essential for maintaining the integrity of the mutable string's length
* attribute, especially after operations that alter the string size without
* automatic length management.
*
* @param pThis Pointer to the mutable string object being updated.
* @param newlength The updated length of the string after modifications.
*
* @see WSMutStr_BeforeWrite()
*/
void WSMutStr_AfterWrite(WSMUTSTR* pThis, size_t newlength)
{ WSMutStr_SetLength(pThis, newlength); }

/**
* Updates the length of a mutable string and ensures it is null-terminated
* after calling WSMutStr_BeforeWrite().
*
* After performing write operations or modifications to the internal buffer of
* a `WSMUTSTR` object, this function is used to set the new length of the
* string and to ensure that the string is properly null-terminated at this new
* length. It effectively truncates the string at `newlength`, removing any
* characters beyond this point and adding a null terminator.
*
* Essential for maintaining the integrity of the mutable string's length
* attribute, especially after operations that alter the string size without
* automatic length management.
*
* @param pThis Pointer to the mutable string object being updated.
* @param newlength The new length of the string, where it should be
* null-terminated.
*
* @see WSMutStr_BeforeWrite()
*/
void WSMutStr_AfterWriteAndNullTerminate(WSMUTSTR* pThis, size_t newlength)
{ WSMutStr_Erase(pThis, newlength); }


/**
* Replaces a specified substring within a mutable string object with another
* string.
*
* This function replaces a portion of the mutable string specified by
* `patternpos` and `patternlen` with the string `replace` of length
* `replacelen`. If `patternpos` is equal to `WSMAXSIZE`, it is set to the
* current length of the mutable string, allowing for appending. If
* `replacelen` is set to `WSMAXLENGTH`, the length of `replace` is calculated
* using `CStrLength`. The function handles resizing of the mutable string if
* the replacement string is longer than the substring being replaced. It also
* ensures that the mutable string remains null-terminated after the
* operation. If the mutable string object is not properly initialized, the
* function does nothing.
*
* The function ensures that the final string is properly null-terminated. If
* the new length of the string after replacement is longer than the original,
* the function resizes the mutable string to accommodate the new length. It
* safely shifts the contents of the string to accommodate the difference in
* lengths between the pattern being replaced and the replacement string.
*
* @param pThis Pointer to the mutable string object (`WSMUTSTR*`).
* @param patternpos The starting position of the substring to be replaced
* within the mutable string.
* @param patternlen The length of the substring to be replaced.
* @param replace Pointer to the replacement string (`const char*`).
* @param replacelen The length of the replacement string; if set to
* `WSMAXLENGTH`, the actual length is calculated.
*/
void WSMutStr_ReplaceSingle(WSMUTSTR* pThis,
	size_t patternpos, size_t patternlen,
	const char* replace, size_t replacelen)
{
	size_t oldlength = WSMutStr_GetLength(pThis);
	if (patternpos == WSMAXLENGTH) patternpos = oldlength;
	if (replacelen == WSMAXLENGTH) replacelen = CStrLength(replace);
	if (replacelen > patternlen)
		WSMutStr_BeforeWrite(pThis, oldlength + replacelen - patternlen);
	if(WSMutStr_IsCreated(pThis))
	{
		if (oldlength > patternpos + patternlen)
			WSMemoryMove((void*)&(WSMutStr_GetCString(pThis)[
							patternpos + replacelen]),
				(void*)&(WSMutStr_GetCString(pThis)[patternpos + patternlen]),
				(oldlength - patternpos - patternlen) * sizeof(char));
		if (replacelen > 0)
			WSMemoryCopy((void*)&(WSMutStr_GetCString(pThis)[patternpos]),
				replace,
				(replacelen) * sizeof(char));
		/* note that resize will null-terminate the string */
		WSMutStr_Resize(pThis,
			(patternlen > oldlength ? replacelen :
									  oldlength - patternlen + replacelen));
	}
}

/**
* Replaces all occurrences of a specified substring within a mutable string
* object with another string, starting from a given position and up to a
* specified number of times.
*
* This function searches for all occurrences of the substring `exp` of length
* `explen` within the mutable string starting from `startpos`. Each found
* occurrence is replaced with the string `what` of length `whatlen`. The
* replacement process continues until either all instances of the substring
* have been replaced or a specified `amount` of replacements have been made.
* If `startpos` is equal to `WSMAXLENGTH`, the search begins from the start
* of the mutable string. If `amount` is set to `WSMAXLENGTH`, there is no
* limit on the number of replacements.
*
* The function uses `CStrFind` to locate each occurrence of the specified
* substring within the mutable string. `WSMutStr_ReplaceSingle` is used for
* performing the actual replacement of each found substring. This function
* can potentially alter the length of the mutable string multiple times,
* depending on the number of replacements and the difference in length
* between the original and replacement substrings.
*
* @param pThis Pointer to the mutable string object (`WSMUTSTR*`).
* @param exp Pointer to the substring to be replaced (`const char*`).
* @param explen The length of the substring to be replaced.
* @param what Pointer to the replacement string (`const char*`).
* @param whatlen The length of the replacement string.
* @param startpos The starting position within the mutable string to begin
* searching for the substring. If `WSMAXLENGTH`, the search starts from the
* beginning.
* @param amount The maximum number of substring replacements to perform. If
* `WSMAXLENGTH`, there is no limit.
*/
void WSMutStr_Replace(WSMUTSTR* pThis, const char* exp, size_t explen,
	const char* what, size_t whatlen,
	size_t startpos, size_t amount)
{
	size_t pos = startpos == WSMAXLENGTH ? 0 : startpos;

	while ((pos = WSStr_FindCStringWithPos(WSMutStr_AsStr(pThis),
					exp, explen, pos)) != WSMAXLENGTH)
	{
		WSMutStr_ReplaceSingle(pThis, pos, explen, what, whatlen);
		pos += whatlen - 1;
		if (amount != WSMAXLENGTH && --amount == 0) break;
		++pos;
	}
}
/**
* Ensures that a mutable string object has enough allocated memory for a
* specified size similar to std::string::reserve().
*
* This function checks if the current allocated size (`mSize`) of a `WSMUTSTR`
* object is less than or equal to the requested `newsize`. If so, it attempts
* to allocate (or reallocate) memory for the mutable string to meet the
* requested size. This pre-allocation is beneficial for reducing the number
* of allocations needed when the size of the data that the mutable string will
* hold is known in advance, improving performance by minimizing memory
* reallocation.
*
* The function only increases the allocated memory size if `newsize` is greater
* than the current allocated size (`mSize`). It does not decrease the allocated
* memory if `newsize` is less than `mSize`.
*
* The actual allocation of memory is delegated to `WSMutStr_Alloc`, which
* handles the details of memory management, including any necessary
* reallocation and the preservation of existing data within the buffer.
*
* After calling this function, the length of the string (`mLength`) remains
* unchanged, and the function solely affects the capacity (`mSize`) of the
* allocated buffer.
*
* @param pThis Pointer to the mutable string object to reserve memory for.
* @param newsize The desired minimum allocated size for the mutable string
* object.
*/
void WSMutStr_Reserve(WSMUTSTR* pThis, size_t newsize)
{
	if (pThis->mSize <= newsize) /* C++ spec doesn't allow to reserve more */
		WSMutStr_Alloc(pThis, newsize);
}

/**
* Resizes the internal buffer of a mutable string object and updates its
* length similar to std::string::resize().
*
* This function adjusts the size of the mutable string's buffer to at least
* `newsize` characters, allowing for additional space to avoid frequent
* reallocations. It then sets the string's length to `newsize` and ensures
* the data is null-terminated at this new length. The allocation strategy
* aims to balance memory efficiency with performance by pre-allocating extra
* space beyond the immediate requirement.
*
* If the current buffer size (`mSize`) is not sufficient to accommodate
* `newsize + 1` characters (accounting for the null terminator), the function
* allocates a larger buffer, typically larger than requested to minimize
* future allocations.
*
* The resizing operation ensures the buffer is allocated with additional
* space beyond `newsize` (specifically, `(newsize + 16) * 2`), providing a
* buffer for further modifications without immediate need for reallocation.
*
* After resizing, the function explicitly sets the string's length to
* `newsize` and places a null terminator at the end of the new content,
* maintaining compatibility with C string functions and ensuring the string
* is properly terminated.
*
* The function checks if the mutable string object has been correctly
* initialized (`WSMutStr_IsCreated`) before proceeding with the resize
* operation, ensuring the integrity of the operation.
*
* @param pThis Pointer to the mutable string object to be resized.
* @param newsize The new desired length of the string content.
*/
void WSMutStr_Resize(WSMUTSTR* pThis, size_t newsize)
{ /* Allocs newsize + 1 minimum, sets length to newsize, and nullchar at len */
	if (pThis->mSize <= newsize + 1)
		WSMutStr_Alloc(pThis, (newsize + 16) * 2);
	WSASSERT(WSMutStr_IsCreated(pThis));
	if(WSMutStr_IsCreated(pThis))
	{
		pThis->mLength = newsize;
		WSMutStr_NullTerminate(pThis, pThis->mLength);
	}
}

/**
* Sets the length of a mutable string.
*
* This function directly sets the length of the `WSMUTSTR` object (`pThis`) to
* a specified value (`l`). This operation can be used to manually adjust the
* length of the string, for instance, after directly modifying the string data
* or when truncating the string. It is the caller's responsibility to ensure
* that this operation does not lead to invalid string states, such as
* specifying a length longer than the actual allocated memory or the current
* capacity of the string.
*
* This function does not modify the string's content or capacity; it only
* updates the length metadata. After calling this function, it is essential
* to ensure that the string data is consistent with the new length, including
* proper null termination if applicable.
*
* Should generally not be called unless absolutely necessary.
*
* @param pThis Pointer to the `WSMUTSTR` object being modified.
* @param l The new length to set for the mutable string, not including the null
*          terminator.
*/
void WSMutStr_SetLength(WSMUTSTR* pThis, size_t l)
{ pThis->mLength = l; }

#ifdef __cplusplus
}
#endif

/**
* @}
*/
#endif /* WSMUTSTR_IMPLEMENTATION */
