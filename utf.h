/**
*	@file utf.h
*
*	#     # ####### #######
*	#     #    #    #
*	#     #    #    #
*	#     #    #    #####
*	#     #    #    #
*	#     #    #    #
*	 #####     #    #
*
*	Single header file for UTF8<->UTF16<->UCS conversion.
*
*	###Instructions###
*
*	Define UTF_IMPLEMENTATION in one source file for the implementation
*
*	###NOTES###
*	- ASCII art, mostly "Banner", from
*	-- budavariam.github.io/asciiart-text/multi
*
*	@version 0.1
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
#ifndef __UTF_H__
#define __UTF_H__

#if !defined(WSBOOL_DEFINED)
	typedef int wsbool;
	#if !defined(TRUE)
		#define TRUE 1
		#define FALSE 0
	#endif
	#define WSBOOL_DEFINED
#endif
#if !defined(countof)
	#define countof(x) (sizeof(x) / sizeof(*x))
	#define strcountof(x) (countof(x) - 1)
#endif
/* [u]int8_t, [u]int16_t, [u]int32_t */
#if !defined(__clang__) /* For Metrowerks and others, clang complains */
	#define __STDC_LIMIT_MACROS
	#define __STDC_CONSTANT_MACROS
#endif
#include <stddef.h>
#if !defined(_MSC_VER) || _MSC_VER >= 1600 /* 2010+ */
	#ifndef UINTMAX_MAX
		#include <stdint.h> /* for NULL, size_t, other_ts */
	#endif
	#if !defined(ULONG_MAX)
		#include <limits.h>
	#endif
#elif defined(_MSC_VER) && !defined(INT_MAX) /* VC6 */
	#include <limits.h>
#endif
#ifndef INT32_MAX
	#ifdef __alpha
		typedef unsigned int uint32_t;
		typedef int int32_t;
	#else
		typedef unsigned long uint32_t;
		#if !defined(__MWERKS__) || !defined(__MACH__)
			typedef long int32_t;
		#endif /*  MWERKS MACH defs int32_t but not uint32_t */
	#endif
	#if !defined(__MWERKS__) || !defined(__MACH__)
		typedef char int8_t;
	#endif /*  MWERKS MACH defs int8_t */
	typedef unsigned char uint8_t;
	typedef short int16_t;
	typedef unsigned short uint16_t;
	#define INT32_MAX (uint32_t)(~0)
#endif
/*=============================================================================
			UCS/UTF[8/16]CharXXX - Unicode UTF8/16/UCS characters
=============================================================================*/
#ifdef __cplusplus
	extern "C" {
#endif
#define UTF8CharToUpper(s) UCSCharToUpper(UTF8CharToUCS(s))
#define UTF8CharToLower(s) UCSCharToLower(UTF8CharToUCS(s))
#define UTF8CharIsLeading(c)	((c & 0xC0) == 0xC0)
#define UTF8CharIsTrailing(c)	((c & 0xC0) == 0x80)
#define UTF8CharIsAnsii(c)		((c & 0x80) == 0x00)
size_t UTF8CharTrailing(const char* s);
void UCSCharToUTF8(uint32_t ch, char *dest);
size_t UTF8CharLen(const char* s);
uint32_t UTF8CharToUCS(const char *src);
uint32_t UCSCharToLower(uint32_t cp);
uint32_t UCSCharToUpper(uint32_t cp);
size_t UCSCharUTF8Size(uint32_t ch);
size_t UTF8CharUCSSize(const char* s);
uint32_t UTF16CharToUCS(const uint16_t* utf16);
void UCSCharToUTF16(uint32_t ucs4, uint16_t* utf16);
uint16_t* UTF8ToUTF16(const char* utf8);
char* UTF16ToUTF8(const uint16_t* utf16);
uint32_t UTF16DecodeSurrogatePair(uint16_t high, uint16_t low);
int UTF16IsLowSurrogate(uint16_t wc);
int UTF16IsHighSurrogate(uint16_t wc);
#ifdef __cplusplus
	} /* end extern "C" */
#endif
#endif /* __UTF_H__ */
/******************************************************************************
	 ##### ###                                                        #####
	 #      #  #    # #####  #      ###### #    # ###### #    # #####     #
	 #      #  ##  ## #    # #      #      ##  ## #      ##   #   #       #
	 #      #  # ## # #    # #      #####  # ## # #####  # #  #   #       #
	 #      #  #    # #####  #      #      #    # #      #  # #   #       #
	 #      #  #    # #      #      #      #    # #      #   ##   #       #
	 ##### ### #    # #      ###### ###### #    # ###### #    #   #   #####
******************************************************************************/
#if defined(UTF_IMPLEMENTATION)
#include <stdlib.h> /* malloc */
#ifdef __cplusplus
	extern "C" {
#endif
/**
* @defgroup UTF8UCS UTF-8 and Unicode Conversion Functions
*
* UTF8 and UCS conversion functions - some replacements in case you'd like
*
* https://github.com/sheredom/utf8.h
* - UTF8CharUCSSize -> utf8len
* - UCSCharToLower -> utf8lwrcodepoint
* - UCSCharToUpper -> utf8uprcodepoint
*
* https://github.com/JeffBezanson/cutef8/blob/master/utf8.c
* - UCSCharToUTF8 -> u8_toutf8
* - UTF8CharLen -> u8_seqlen
* - UTF8CharToUCS ->  u8_toucs
*
* https://github.com/RoelofBerg/Utf8Ucs2Converter/blob/master/Utf8Ucs2Converter.cpp
* - (No replacements though it has some UCS2<->UTF8 converters)
*  @{
*/

/**
* Determines the number of trailing bytes in a UTF-8 character.
*
* Given a pointer to the start of a UTF-8 encoded character, this function
* calculates the number of bytes that follow the initial byte, based on the
* leading byte's value. This is used to determine the total length of the
* character in UTF-8 encoding, facilitating correct parsing and handling of
* multi-byte characters.
*
* UTF-8 characters can vary in length from 1 to 4 bytes:
* - 1 byte for standard ASCII (0xxxxxxx).
* - 2 bytes for characters from the Latin-1 Supplement and other alphabets
*   (110xxxxx).
* - 3 bytes for characters from various other scripts and symbols (1110xxxx).
* - 4 bytes for rare characters and symbols, including some emoji (11110xxx).
*
* The input pointer should reference the beginning byte of a UTF-8
* character for accurate results.
*
* @param s Pointer to the character from which to determine the trailing byte
*          count.
* @return Number of trailing bytes following the initial byte of the character.
*
*/
size_t UTF8CharTrailing(const char* s)
{ /* https://sites.google.com/site/markusicu/unicode/utf-8-bytes */
	unsigned char c = (unsigned char)*s;
	if (c < 0xC0) /* 0xxxxxxx */
		return 0;
	if (c < 0xE0) /* 110xxxxx */
		return 1;
	if (c < 0xF0) /* 1110xxxx */
		return 2;
	return 3; /* 11110xxx */
}

/**
* Calculates the length of a UTF-8 character, including handling of partial
* characters.
*
* This function computes the total length of a UTF-8 encoded character pointed
* to by `s`. It first determines the number of trailing bytes expected for the
* character by calling `UTF8CharTrailing`. It then iterates through the string,
* checking for the presence of these trailing bytes, but also ensures that it
* does not go past the end of a string prematurely, allowing for the safe
* handling of partial UTF-8 characters. This is particularly useful for
* processing text streams or buffers where a character may be split across
* data chunks.
*
* For complete characters, the length will be between 1 and 4 bytes.
*       For partial characters (e.g., when the expected trailing bytes are not
*       present because of string truncation), the length returned accounts for
*       the bytes that are present.
*
* @param s Pointer to the start of a UTF-8 character.
* @return The length of the UTF-8 character starting at `s`, accounting for
*         both complete and partial characters.
*/
size_t UTF8CharLen(const char* s)
{ /* The extra logic is for partial UTF8 characters like "\xC3" */
	size_t numTrailing = UTF8CharTrailing(s);
	const char* sptr = s + 1;
	while(numTrailing-- && *sptr) { ++sptr; }
	return sptr - s;
}

/**
* Converts a Unicode scalar value to its UTF-8 encoded form and stores it in a
* buffer.
*
* This function encodes a single Unicode character (scalar value) `ch` into its
* UTF-8 representation and writes the encoded bytes into the buffer pointed to
* by `dest`. The UTF-8 encoding is variable length, ranging from 1 to 4 bytes,
* depending on the scalar value:
* - Values below 0x80 are encoded in a single byte.
* - Values between 0x80 and 0x7FF are encoded in two bytes.
* - Values between 0x800 and 0xFFFF are encoded in three bytes.
* - Values between 0x10000 and 0x10FFFF are encoded in four bytes.
*
* The function handles the conversion by applying bit manipulation and masking
* techniques to format the scalar value according to the UTF-8 encoding
* standard. All bytes, except for single-byte characters, are prefixed with a
* leading byte indicating the length of the sequence and one or more
* continuation bytes, each beginning with '10' in binary.
*
* Casting to `char` within the function is necessary to ensure correct handling
* of the bit manipulation operations across different compilers, such as GCC.
*
* @param ch The Unicode scalar value to be encoded.
* @param dest Pointer to the buffer where the resulting UTF-8 encoded bytes
* will be stored.
*
* @note This function directly modifies the buffer pointed to by `dest`, and it
* does not null-terminate the encoded string; it only writes the bytes
* representing the Unicode character.
* @note The buffer `dest` must have sufficient space to hold the encoded
* character (up to 4 bytes).
*/
void UCSCharToUTF8(uint32_t ch, char *dest)
{/* we have to cast these all to chars because GCC isn't smart */
	if (ch < 0x80)
		*dest++ = (char)ch;
	else if (ch < 0x800)
	{
		*dest++ = (char)((ch>>6) | 0xC0);
		*dest++ = (char)((ch & 0x3F) | 0x80);
	}
	else if (ch < 0x10000)
	{
		*dest++ = (char)((ch>>12) | 0xE0);
		*dest++ = (char)(((ch>>6) & 0x3F) | 0x80);
		*dest++ = (char)((ch & 0x3F) | 0x80);
	}
	else if (ch < 0x110000)
	{
		*dest++ = (char)((ch>>18) | 0xF0);
		*dest++ = (char)(((ch>>12) & 0x3F) | 0x80);
		*dest++ = (char)(((ch>>6) & 0x3F) | 0x80);
		*dest++ = (char)((ch & 0x3F) | 0x80);
	}
}

/**
* Decodes a single UTF-8 encoded character to its Unicode scalar value.
*
* This function decodes the first character in a UTF-8 encoded string `src` and
* returns its Unicode scalar value. It uses an array `offsetsFromUTF8` to
* adjust the decoded value based on the number of bytes that make up the UTF-8
* encoded character. The function calculates the number of trailing bytes for
* the UTF-8 character with `UTF8CharTrailing` and then decodes the character
* by combining these bytes into a single 32-bit integer (`uint32_t`). The
* decoding process involves bit shifting and addition operations to
* reconstruct the scalar value from the UTF-8 sequence.
*
* Within the function the switch statement uses deliberate fall-through for
* cases 5 to 0 to accumulate the character value, then adjusts it with an
* offset to get the correct Unicode scalar value. `offsetsFromUTF8` contains
* adjustment values to be subtracted from the accumulated character value to
* correct for the multi-byte encoding overhead introduced by UTF-8.
*
* This function assumes `src` points to the start of a UTF-8 encoded
* character. Additionally, this function processes only the first character
* in `src` and does not handle string iteration or multiple characters.
*
* @param src Pointer to the UTF-8 encoded character string.
* @return The Unicode scalar value of the first character in the `src` string.
*/
uint32_t UTF8CharToUCS(const char* src)
{
	const uint32_t offsetsFromUTF8[6] =
	{
		0x00000000UL, 0x00003080UL, 0x000E2080UL,
		0x03C82080UL, 0xFA082080UL, 0x82082080UL
	};
	uint32_t ch = 0;
	size_t nb = UTF8CharTrailing(src);
	switch (nb)
	{
		/* these fall through deliberately */
		case 5: ch += (unsigned char)*src++; ch <<= 6;
		case 4: ch += (unsigned char)*src++; ch <<= 6;
		case 3: ch += (unsigned char)*src++; ch <<= 6;
		case 2: ch += (unsigned char)*src++; ch <<= 6;
		case 1: ch += (unsigned char)*src++; ch <<= 6;
		case 0: ch += (unsigned char)*src++;
	}
 ch -= offsetsFromUTF8[nb];
 return ch;
}

/**
* Converts a Unicode code point to its lowercase equivalent.
*
* This function converts a single Unicode code point `cp` to its lowercase
* version, if it exists. The conversion covers a wide range of Unicode
* characters, including:
* - Basic Latin uppercase letters (U+0041 to U+005A) and extended ranges,
*   which are directly converted by adding 32 to their code point value.
* - Cyrillic (U+0410 to U+042F) and Greek (U+0391 to U+03A1, U+03A3 to U+03AB)
*   characters, also converted by adding 32.
* - Specific ranges within the Latin Extended-A block and other areas of
*   Unicode, which are converted by setting the least significant bit, or
*   adding 1 and clearing the least significant bit, to handle alternating
*   patterns of uppercase and lowercase characters.
* - Individual exceptions and special cases, such as the Latin Extended-B
*   block and other isolated characters, are handled explicitly.
*
* This function uses a combination of range checks, bitwise operations,
*       and specific case mappings to accommodate the complex rules for
*       lowercase conversion in Unicode. It covers a significant portion of
*       Unicode characters but may not include all possible characters,
*       especially those added in newer Unicode versions.
*
* @param cp The Unicode code point to be converted.
* @return The lowercase equivalent of the code point, if it exists; otherwise,
*         returns the original code point.
*/
uint32_t UCSCharToLower(uint32_t cp)
{
	if (((0x0041 <= cp) && (0x005a >= cp)) ||
		((0x00c0 <= cp) && (0x00d6 >= cp)) ||
		((0x00d8 <= cp) && (0x00de >= cp)) ||
		((0x0391 <= cp) && (0x03a1 >= cp)) ||
		((0x03a3 <= cp) && (0x03ab >= cp)) ||
		((0x0410 <= cp) && (0x042f >= cp)))
		cp += 32;
	else if ((0x0400 <= cp) && (0x040f >= cp))
		cp += 80;
	else if (((0x0100 <= cp) && (0x012f >= cp)) ||
				((0x0132 <= cp) && (0x0137 >= cp)) ||
				((0x014a <= cp) && (0x0177 >= cp)) ||
				((0x0182 <= cp) && (0x0185 >= cp)) ||
				((0x01a0 <= cp) && (0x01a5 >= cp)) ||
				((0x01de <= cp) && (0x01ef >= cp)) ||
				((0x01f8 <= cp) && (0x021f >= cp)) ||
				((0x0222 <= cp) && (0x0233 >= cp)) ||
				((0x0246 <= cp) && (0x024f >= cp)) ||
				((0x03d8 <= cp) && (0x03ef >= cp)) ||
				((0x0460 <= cp) && (0x0481 >= cp)) ||
				((0x048a <= cp) && (0x04ff >= cp)))
		cp |= 0x1;
	else if (((0x0139 <= cp) && (0x0148 >= cp)) ||
				((0x0179 <= cp) && (0x017e >= cp)) ||
				((0x01af <= cp) && (0x01b0 >= cp)) ||
				((0x01b3 <= cp) && (0x01b6 >= cp)) ||
				((0x01cd <= cp) && (0x01dc >= cp)))
	{
			cp += 1;
			cp &= ~0x1;
	}
	else
	{
		switch (cp)
		{
			default: break;
			case 0x0178: cp = 0x00ff; break;
			case 0x0243: cp = 0x0180; break;
			case 0x018e: cp = 0x01dd; break;
			case 0x023d: cp = 0x019a; break;
			case 0x0220: cp = 0x019e; break;
			case 0x01b7: cp = 0x0292; break;
			case 0x01c4: cp = 0x01c6; break;
			case 0x01c7: cp = 0x01c9; break;
			case 0x01ca: cp = 0x01cc; break;
			case 0x01f1: cp = 0x01f3; break;
			case 0x01f7: cp = 0x01bf; break;
			case 0x0187: cp = 0x0188; break;
			case 0x018b: cp = 0x018c; break;
			case 0x0191: cp = 0x0192; break;
			case 0x0198: cp = 0x0199; break;
			case 0x01a7: cp = 0x01a8; break;
			case 0x01ac: cp = 0x01ad; break;
			case 0x01af: cp = 0x01b0; break;
			case 0x01b8: cp = 0x01b9; break;
			case 0x01bc: cp = 0x01bd; break;
			case 0x01f4: cp = 0x01f5; break;
			case 0x023b: cp = 0x023c; break;
			case 0x0241: cp = 0x0242; break;
			case 0x03fd: cp = 0x037b; break;
			case 0x03fe: cp = 0x037c; break;
			case 0x03ff: cp = 0x037d; break;
			case 0x037f: cp = 0x03f3; break;
			case 0x0386: cp = 0x03ac; break;
			case 0x0388: cp = 0x03ad; break;
			case 0x0389: cp = 0x03ae; break;
			case 0x038a: cp = 0x03af; break;
			case 0x038c: cp = 0x03cc; break;
			case 0x038e: cp = 0x03cd; break;
			case 0x038f: cp = 0x03ce; break;
			case 0x0370: cp = 0x0371; break;
			case 0x0372: cp = 0x0373; break;
			case 0x0376: cp = 0x0377; break;
			case 0x03f4: cp = 0x03d1; break;
			case 0x03cf: cp = 0x03d7; break;
			case 0x03f9: cp = 0x03f2; break;
			case 0x03f7: cp = 0x03f8; break;
			case 0x03fa: cp = 0x03fb; break;
		}
	}
	return cp;
}

/**
* Converts a Unicode code point to its uppercase equivalent.
*
* Similar to `UCSCharToLower`, this function converts a Unicode code point `cp`
* to its uppercase version, if available. It handles a broad spectrum of
* Unicode characters, including:
* - Basic Latin lowercase letters (U+0061 to U+007A) and extended ranges, which
*   are directly converted by subtracting 32.
* - Cyrillic (U+0430 to U+044F) and Greek (U+03B1 to U+03C1, U+03C3 to U+03CB)
*   characters, converted by subtracting 32.
* - Other specific ranges within Latin Extended-A and other areas, converted by
*   clearing the least significant bit, indicating a switch to uppercase.
* - Certain ranges where alternating characters are uppercase followed by
*   lowercase, which adjust by subtracting 1 and then setting the least
*   significant bit.
* - Individual exceptions and special cases are handled explicitly, such as
*   characters in the Latin Extended-B block and other isolated characters.
*
* This function uses a mix of range checks, bitwise operations, and
*       explicit case mappings to accommodate the complex rules for uppercase
*       conversion in Unicode, covering a significant portion of characters but
*       may not include newer additions to Unicode.
*
* @param cp The Unicode code point to be converted.
* @return The uppercase equivalent of the code point, if it exists; otherwise,
*         the original code point is returned.
*/
uint32_t UCSCharToUpper(uint32_t cp)
{
	if (((0x0061 <= cp) && (0x007a >= cp)) ||
		((0x00e0 <= cp) && (0x00f6 >= cp)) ||
		((0x00f8 <= cp) && (0x00fe >= cp)) ||
		((0x03b1 <= cp) && (0x03c1 >= cp)) ||
		((0x03c3 <= cp) && (0x03cb >= cp)) ||
		((0x0430 <= cp) && (0x044f >= cp)))
		cp -= 32;
	else if ((0x0450 <= cp) && (0x045f >= cp))
		cp -= 80;
	else if (((0x0100 <= cp) && (0x012f >= cp)) ||
				((0x0132 <= cp) && (0x0137 >= cp)) ||
				((0x014a <= cp) && (0x0177 >= cp)) ||
				((0x0182 <= cp) && (0x0185 >= cp)) ||
				((0x01a0 <= cp) && (0x01a5 >= cp)) ||
				((0x01de <= cp) && (0x01ef >= cp)) ||
				((0x01f8 <= cp) && (0x021f >= cp)) ||
				((0x0222 <= cp) && (0x0233 >= cp)) ||
				((0x0246 <= cp) && (0x024f >= cp)) ||
				((0x03d8 <= cp) && (0x03ef >= cp)) ||
				((0x0460 <= cp) && (0x0481 >= cp)) ||
				((0x048a <= cp) && (0x04ff >= cp)))
		cp &= ~0x1;
	else if (((0x0139 <= cp) && (0x0148 >= cp)) ||
				((0x0179 <= cp) && (0x017e >= cp)) ||
				((0x01af <= cp) && (0x01b0 >= cp)) ||
				((0x01b3 <= cp) && (0x01b6 >= cp)) ||
				((0x01cd <= cp) && (0x01dc >= cp)))
	{
		cp -= 1;
		cp |= 0x1;
	}
	else
	{
		switch (cp)
		{
			default: break;
			case 0x00ff: cp = 0x0178; break;
			case 0x0180: cp = 0x0243; break;
			case 0x01dd: cp = 0x018e; break;
			case 0x019a: cp = 0x023d; break;
			case 0x019e: cp = 0x0220; break;
			case 0x0292: cp = 0x01b7; break;
			case 0x01c6: cp = 0x01c4; break;
			case 0x01c9: cp = 0x01c7; break;
			case 0x01cc: cp = 0x01ca; break;
			case 0x01f3: cp = 0x01f1; break;
			case 0x01bf: cp = 0x01f7; break;
			case 0x0188: cp = 0x0187; break;
			case 0x018c: cp = 0x018b; break;
			case 0x0192: cp = 0x0191; break;
			case 0x0199: cp = 0x0198; break;
			case 0x01a8: cp = 0x01a7; break;
			case 0x01ad: cp = 0x01ac; break;
			case 0x01b0: cp = 0x01af; break;
			case 0x01b9: cp = 0x01b8; break;
			case 0x01bd: cp = 0x01bc; break;
			case 0x01f5: cp = 0x01f4; break;
			case 0x023c: cp = 0x023b; break;
			case 0x0242: cp = 0x0241; break;
			case 0x037b: cp = 0x03fd; break;
			case 0x037c: cp = 0x03fe; break;
			case 0x037d: cp = 0x03ff; break;
			case 0x03f3: cp = 0x037f; break;
			case 0x03ac: cp = 0x0386; break;
			case 0x03ad: cp = 0x0388; break;
			case 0x03ae: cp = 0x0389; break;
			case 0x03af: cp = 0x038a; break;
			case 0x03cc: cp = 0x038c; break;
			case 0x03cd: cp = 0x038e; break;
			case 0x03ce: cp = 0x038f; break;
			case 0x0371: cp = 0x0370; break;
			case 0x0373: cp = 0x0372; break;
			case 0x0377: cp = 0x0376; break;
			case 0x03d1: cp = 0x03f4; break;
			case 0x03d7: cp = 0x03cf; break;
			case 0x03f2: cp = 0x03f9; break;
			case 0x03f8: cp = 0x03f7; break;
			case 0x03fb: cp = 0x03fa; break;
		}
	}
	return cp;
}

/**
* Determines the number of bytes required to represent a Unicode code point in
* UTF-8 encoding.
*
* This function calculates the size, in bytes, needed to encode a given Unicode
* code point `ch` in UTF-8. The size is determined based on the value of `ch`:
* - Code points U+0000 to U+007F are encoded in 1 byte.
* - Code points U+0080 to U+07FF are encoded in 2 bytes.
* - Code points U+0800 to U+FFFF are encoded in 3 bytes.
* - Code points U+10000 to U+10FFFF are encoded in 4 bytes.
* If the code point falls outside of the valid Unicode range (above U+10FFFF),
* the function returns 0, indicating an error or invalid code point.
*
* This function does not perform the encoding but only calculates the
*       required size for encoding. It is useful for buffer allocation or
*       pre-validation of Unicode characters before encoding them into UTF-8.
*
* @param ch The Unicode code point to be evaluated.
* @return The number of bytes required for UTF-8 encoding of `ch`, or 0 if `ch`
*         is not a valid Unicode code point.
*/
size_t UCSCharUTF8Size(uint32_t ch)
{
	if (ch < 0x80)
		return 1;
	else if (ch < 0x800)
		return 2;
	else if (ch < 0x10000)
		return 3;
	else if (ch < 0x110000)
		return 4;
	return 0;
}

/**
* Calculates the size in bytes of a UTF-8 encoded character based on its first
* byte.
*
* This function examines the first byte of a UTF-8 encoded character pointed to
* by `s` and determines the total number of bytes that make up the UTF-8
* character. The size is determined by the leading bits in the first byte:
* - If the first byte starts with `0xf0` (after masking with `0xf8`), the
*   character is encoded in 4 bytes.
* - If the first byte starts with `0xe0` (after masking with `0xf0`), the
*   character is encoded in 3 bytes.
* - If the first byte starts with `0xc0` (after masking with `0xe0`), the
*   character is encoded in 2 bytes.
* - Otherwise, the character is encoded in 1 byte (standard ASCII).
*
* This function only inspects the leading byte of a character and
*       assumes `s` points to the start of a UTF-8 encoded character. It is
*       useful for determining the length of a character in a UTF-8 encoded
*       string before decoding it.
*
* @param s Pointer to the first byte of a UTF-8 encoded character.
* @return The total number of bytes in the UTF-8 encoded character.
*/
size_t UTF8CharUCSSize(const char* s)
{
	if (0xf0 == (0xf8 & s[0]))
		return 4;
	else if (0xe0 == (0xf0 & s[0]))
		return 3;
	else if (0xc0 == (0xe0 & s[0]))
		return 2;
	return 1;
}

/**
* Checks if a UTF-16 character is a high surrogate.
*
* This function determines if the given UTF-16 character falls within
* the high surrogate range, which is used to encode supplementary
* characters in UTF-16.
*
* @param wc A UTF-16 character.
* @return 1 if the character is a high surrogate (0xD800-0xDBFF),
*         0 otherwise.
*/
int UTF16IsHighSurrogate(uint16_t wc)
{ return (wc >= 0xD800 && wc <= 0xDBFF); }

/**
* Checks if a UTF-16 character is a low surrogate.
*
* This function determines if the given UTF-16 character falls within
* the low surrogate range, which is used to encode supplementary
* characters in UTF-16.
*
* @param wc A UTF-16 character.
* @return 1 if the character is a low surrogate (0xDC00-0xDFFF),
*         0 otherwise.
*/
int UTF16IsLowSurrogate(uint16_t wc)
{ return (wc >= 0xDC00 && wc <= 0xDFFF); }

/**
* Decodes a UTF-16 surrogate pair into a UCS-4 code point.
*
* This function takes a high surrogate and a low surrogate UTF-16 character,
* and decodes them into a single UCS-4 code point. The input characters must
* be valid surrogates.
*
* @param high The high surrogate UTF-16 character (0xD800-0xDBFF).
* @param low The low surrogate UTF-16 character (0xDC00-0xDFFF).
* @return The decoded UCS-4 code point.
*/
uint32_t UTF16DecodeSurrogatePair(uint16_t high, uint16_t low)
{ return 0x10000 + ((high - 0xD800) << 10) + (low - 0xDC00); }

/**
* Converts a UTF-16 encoded string to a UTF-8 encoded string.
*
* The function calculates the required length for the UTF-8 string,
* allocates memory for it, and then performs the conversion. It handles
* single UTF-16 units, pairs of surrogate units, and ensures proper
* UTF-8 encoding.
* @param utf16 A pointer to a null-terminated UTF-16 encoded string.
*              The input string must be properly encoded and should
*              not be NULL.
* @return A pointer to a newly allocated null-terminated UTF-8 encoded
*         string. The caller is responsible for freeing the allocated
*         memory using free(). If memory allocation fails or the input
*         is NULL, returns NULL.
*/
char* UTF16ToUTF8(const uint16_t* utf16)
{
	const uint16_t* p;
	size_t utf8_length;
	char* utf8;
	char* out;
	if (utf16 == NULL) return NULL;

	/* Calculate the length of the resulting UTF-8 string */
	utf8_length = 0;
	for (p = utf16; *p; ++p)
	{
		if (*p < 0x80)
			utf8_length += 1;
		else if (*p < 0x800)
			utf8_length += 2;
		else if (UTF16IsHighSurrogate(*p) && UTF16IsLowSurrogate(*(p + 1)))
		{
			utf8_length += 4;
			p++; /* Skip the next value, as it is part of the surrogate pair */
		}
		else
			utf8_length += 3;
	}

	/* Allocate memory for the UTF-8 string */
	utf8 = (char*)malloc(utf8_length + 1);
	if (utf8 == NULL)
		return NULL;

	/* Convert UTF-16 to UTF-8 */
	out = utf8;
	for (p = utf16; *p; ++p)
	{
		if (*p < 0x80)
			*out++ = (char)*p;
		else if (*p < 0x800)
		{
			*out++ = (char)(0xC0 | (*p >> 6));
			*out++ = (char)(0x80 | (*p & 0x3F));
		}
		else if (UTF16IsHighSurrogate(*p) && UTF16IsLowSurrogate(*(p + 1)))
		{
			uint32_t codepoint = UTF16DecodeSurrogatePair(*p, *(p + 1));
			*out++ = (char)(0xF0 | ((codepoint >> 18) & 0x07));
			*out++ = (char)(0x80 | ((codepoint >> 12) & 0x3F));
			*out++ = (char)(0x80 | ((codepoint >> 6) & 0x3F));
			*out++ = (char)(0x80 | (codepoint & 0x3F));
			p++; /* Skip the next value, as it is part of the surrogate pair */
		}
		else
		{
			*out++ = (char)(0xE0 | (*p >> 12));
			*out++ = (char)(0x80 | ((*p >> 6) & 0x3F));
			*out++ = (char)(0x80 | (*p & 0x3F));
		}
	}

	/* Null-terminate the UTF-8 string */
	*out = '\0';
	return utf8;
}

/**
* Converts a UTF-8 encoded string to a UTF-16 encoded string.
*
* The function calculates the required length for the UTF-16 string,
* allocates memory for it, and then performs the conversion. It handles
* both BMP characters and supplementary characters (surrogate pairs)
* ensuring proper UTF-16 encoding.
*
* @param utf8 A pointer to a null-terminated UTF-8 encoded string.
*             The input string must be properly encoded and should
*             not be NULL.
* @return A pointer to a newly allocated null-terminated UTF-16 encoded
*         string. The caller is responsible for freeing the allocated
*         memory using free(). If memory allocation fails or the input
*         is NULL, returns NULL.
*/
uint16_t* UTF8ToUTF16(const char* utf8)
{
	size_t utf16_length;
	const char* p;
	uint16_t* utf16;
	uint16_t* out;
	if (utf8 == NULL) return NULL;

	/* Calculate the length of the resulting UTF-16 string */
	utf16_length = 0;
	p = utf8;
	while (*p)
	{
		uint32_t codepoint = UTF8CharToUCS(p);
		p += UTF8CharLen(p);
		utf16_length += (codepoint <= 0xFFFF) ? 1 : 2;
	}

	/* Allocate memory for the UTF-16 string */
	utf16 = (uint16_t*)malloc((utf16_length + 1) * sizeof(uint16_t));
	if (utf16 == NULL) return NULL;

	/* Convert UTF-8 to UTF-16 */
	out = utf16;
	p = utf8;
	while (*p)
	{
		uint32_t codepoint = UTF8CharToUCS(p);
		p += UTF8CharLen(p);

		if (codepoint <= 0xFFFF)
			/* BMP character */
			*out++ = (uint16_t)codepoint;
		else
		{
			/* Supplementary character */
			codepoint -= 0x10000;
			*out++ = (uint16_t)(0xD800 + (codepoint >> 10));
			*out++ = (uint16_t)(0xDC00 + (codepoint & 0x03FF));
		}
	}

	/* Null-terminate the UTF-16 string */
	*out = 0;
	return utf16;
}

/**
* Converts a UCS-4 character to a UTF-16 encoded character or surrogate pair.
*
* The function converts a UCS-4 character to its equivalent UTF-16
* representation. For BMP characters (code points less than 0x10000), a
* single UTF-16 unit is used. For supplementary characters (code points
* 0x10000 and above), a surrogate pair is used. The result is
* null-terminated to ensure it can be safely used as a C-style string.
*
* @param ucs4 The UCS-4 character to be converted. Must be a valid Unicode
*             code point.
* @param utf16 A pointer to an array of at least 3 uint16_t elements. The
*              converted UTF-16 character(s) will be stored here. The array
*              will be null-terminated.
*/
void UCSCharToUTF16(uint32_t ucs4, uint16_t* utf16)
{
	if (ucs4 < 0x10000)
	{
		/* BMP character (not a surrogate pair) */
		utf16[0] = (uint16_t)ucs4;
		utf16[1] = 0; /* Null terminator for single UTF-16 character */
	}
	else
	{
		/* Supplementary character (requires a surrogate pair) */
		ucs4 -= 0x10000;
		utf16[0] = (uint16_t)((ucs4 >> 10) + 0xD800); /* High surrogate */
		utf16[1] = (uint16_t)((ucs4 & 0x3FF) + 0xDC00); /* Low surrogate */
		utf16[2] = 0; /* Null terminator for surrogate pair */
	}
}

/**
* Converts the first UTF-16 value of a string to a UCS-4 character.
*
* The function checks if the first UTF-16 unit is a high surrogate and the
* second is a low surrogate, indicating a surrogate pair. If so, it converts
* the pair to a single UCS-4 character. If not, it converts the single UTF-16
* unit to UCS-4. This function supports both BMP and supplementary characters.
*
* @param utf16 A pointer to a UTF-16 encoded string. The string must be
*              properly encoded and should not be NULL. It must contain
*              at least one UTF-16 unit.
* @return The UCS-4 character equivalent of the first UTF-16 character or
*         surrogate pair in the input string.
*/
uint32_t UTF16CharToUCS(const uint16_t* utf16)
{
	if (utf16[0] >= 0xD800 && utf16[0] <= 0xDBFF
			&& utf16[1] >= 0xDC00 && utf16[1] <= 0xDFFF) /* Surrogate pair */
		return (((uint32_t)utf16[0] - 0xD800) << 10)
			+ (utf16[1] - 0xDC00) + 0x10000;
	else /* Single UTF-16 character */
		return (uint32_t)utf16[0];
}

/**
* @}
*/
#ifdef __cplusplus
	} /* end extern "C" */
#endif
#endif /* UTF_IMPLEMENTATION */
