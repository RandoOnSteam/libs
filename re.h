/**
*	@file re.h
*
*	######  #######
*	#     # #
*	#     # #
*	######  #####
*	#   #   #
*	#    #  #
*	#     # #######
*
*	PHP-compatible regular expressions that can be compiled on legacy compilers
*	including old System 6 compilers. Supports almost everything except inline
*	modifiers (?opt) and named subexpressions. See re.c for more info.
*
*	###Instructions###
*
*	Define RE_IMPLEMENTATION in one source file for the implementation
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

#ifndef __RE_H__
#define __RE_H__

#if defined __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
	#include <sys/types.h>
#endif
/*	Required on MSW platforms for size_t	*/
#include <stddef.h>

/*	error codes */
/*#include "regexerror.h"*/

#if 1 /* compat with FreeBSD's AddressSanitizer */
#define regcomp wsregcomp
#define regfree wsregfree
#define regexec wsregexec
#define regerror wsregerror
#endif

typedef size_t regoff_t;
typedef char regchar;
typedef unsigned char uregchar;

typedef struct regex_t		/**< Contains info on a compiled regular expression */
{
	size_t re_nsub;			/**< Number of parenthesized subexpressions */
	regchar* nfa;			/**< not spec - NFA */
	size_t nfaSize;			/**< not spec	- NFA characters */
	const regchar* pat;		/**< not spec - Pattern or search string */
	size_t patPos;			/**< not spec - Position in pattern or search string */
#if 0 /* for future builds */
	struct regclo_t* clos;	/* not spec - Closure data for execute */
	struct regbot_t* bots;	/* not spec - BOT data for execute */
	regchar numClo;			/* not spec - Closure amount in NFA */
#endif
	unsigned char numBot;	/**< not spec - BOT amount in NFA */
} regex_t;

typedef struct regmatch_t	/** Contains expression match indexes */
{
	/**	Byte offset from start of string to start of substring. */
	regoff_t rm_so;

	/**	Byte offset from start of string to the first character	after the end of substring. */
	regoff_t rm_eo;
} regmatch_t;

/* regcomp() flags */
#define REG_EXTENDED	64	/*	Use Extended Regular Expressions.			*/
#define REG_ICASE		1	/*	Ignores case in matches.					*/
#define REG_NOSUB		32	/*	Doesn't set re_nsub of regex_t in regcomp.	*/
#define REG_NEWLINE		2	/*	Uses '^' and '$' regardless of settings if
								used after one or the other.  Also \n won't
								be matched with a '.' or non-matching list.	*/
#define REG_FREESPACE	1024/*	# are line comments and space (' '), \r, and
								\n are ignored (escape to literal match).	*/
#define REG_UNGREEDY	512	/*	Closures are non-greedy by default.			*/
#define REG_ENHANCED	2048/*	Use Perl/GNU extensions.					*/

/* regexec() flags */
#define REG_NOTBOL		4	/*	'^' no longer matches line beginnning.		*/
#define REG_NOTEOL		8	/*	'$' no longer matches line end.				*/
#define REG_STARTEND	128	/*	pmatches[0].rm_so/rm_eo are start/end pos.	*/
#define REG_NOTEMPTY	256	/*	Empty string matches aren't included.		*/

int regclone(regex_t* pregclone,
			 const regex_t* preg);	/* not spec	*/

int regcomp(regex_t* preg,
			const regchar* pattern, int cflags);

size_t regerror(int errcode, const regex_t* preg,
				regchar* errbuf, size_t errbuf_size);

int regexec(const regex_t* preg,
			const regchar* string, size_t nmatch,
			regmatch_t pmatch[], int eflags);

void regfree(regex_t* preg);

size_t regsubs(const regchar* string, const regchar* expr,
			   regchar* outstring, size_t outstring_size,
			   size_t numMatches, regmatch_t pmatch[]);		/* not spec	*/

void regsymbolic(const regex_t* preg, regchar** outbuf,
				 size_t* outbuflen, size_t* outbufsize);	/* not spec	*/

/* success from all public functions */
#define REG_OK	0

/*				regclone() errors						*/
#define REG_SRCNINIT	10	/*	not spec	*/
#define REG_DSTINIT		11	/*	not spec	*/

/*				regexec() errors						*/
#define REG_NOMATCH		20
#define REG_NOTCOMP		21	/*	not spec	*/
#define REG_IECLOSE		22	/*	not spec	*/
#define REG_IENFA		23	/*	not spec	*/

/*				regcompile() errors						*/
#define REG_BADPAT		31
#define REG_ECOLLATE	32
#define REG_ECTYPE		33
#define REG_EESCAPE		34
#define REG_ESUBREG		35
#define REG_EBRACK		36
#define REG_EPAREN		37
#define REG_EBRACE		38
#define REG_BADBR		39
#define REG_ERANGE		40
#define REG_ESPACE		41
#define REG_BADRPT		42
#define REG_NOSTRING	43	/*	not spec	*/
#define REG_EMPTYRPT	44	/*	not spec	*/
#define REG_EMPTYPRN	45	/*	not spec	*/
#define REG_EMPTYSA		46	/*	not spec	*/
#define REG_REQFIXED	47	/*	not spec	*/
#define REG_LOOKINSUB	48	/*	not spec	*/
#define REG_UTFINBRACK	49	/*	not spec	*/
#define REG_BADCON		50	/*	not spec	*/

/*		Error descriptions		*/
#define REG_OKSTR _T("NO ERROR: Operation Successful")
#define REG_SRCNINITSTR	_T("Source for clone not ininitialized")
#define REG_DSTINITSTR	_T("Destination for clone already initialized")

#define REG_NOMATCHSTR	_T("Failed to match expression")
#define REG_NOTCOMPSTR	_T("Pattern is not compiled yet or the compile failed")
#define REG_IECLOSESTR	_T("[INTERNAL ERROR] closure: bad nfa op:")
#define REG_IENFASTR	_T("[INTERNAL ERROR] re_exec: bad nfa")

#define REG_BADPATSTR	_T("Invalid regular expression")
#define REG_ECOLLATESTR	_T("Invalid collating element referenced")
#define REG_ECTYPESTR	_T("Invalid character class type referenced")
#define REG_EESCAPESTR	_T("Trailing \\ in pattern")
#define REG_ESUBREGSTR	_T("Number in \\digit invalid or in error")
#define REG_EBRACKSTR	_T("[ ] imbalance or too many [")
#define REG_EPARENSTR	_T("\\( \\) or ( ) imbalance or too many \\( or (")
#define REG_EBRACESTR	_T("\\{ \\} imbalance")
#define REG_BADBRSTR	\
	_T("Values of \\{ \\} or { } not a #, #>9, >2 #'s, 1# > 2#")
#define REG_ERANGESTR	_T("Invalid endpoint in range expression ([x-x])")
#define REG_ESPACESTR	_T("Out of memory")
#define REG_BADRPTSTR	\
	_T("?, *, or + not preceded by valid regular expression")
#define REG_NOSTRINGSTR	_T("Empty input string (NULL or length 0)")
#define REG_EMPTYRPTSTR	_T("Empty closure (? * +)")
#define REG_ESTRINGSTR	_T("Invalid input string")
#define REG_EMPTYPRNSTR	_T("Nothing inside \\( \\) or ( )")
#define REG_EMPTYSASTR	_T("Null pattern inside \\<\\> or < >")
#define REG_REQFIXEDSTR	\
	_T("Pattern inside lookbehinds must be fixed length")
#define REG_LOOKINSUBSTR	\
	_T("Lookbehinds and lookaheads cannot be in a subexpression")
#define REG_UTFINBRACKSTR _T("Unicode characters not allowed in [ ] brackets")
#define REG_BADCONSTR _T("Malformed (?(?IF)THEN|ELSE) conditional")
#define REG_UNKNOWNSTR	_T("[INTERNAL ERROR] *****Unknown Error*****")

/*		State		  [Statement translated from] [Parameter]				*/
#define REG_CHR 2	/* Char						Character					*/
#define REG_ANY 3	/* .						REG_NEWLINE					*/
#define REG_CCL 4	/* [						16 byte bitmask				*/
#define REG_BND 5	/* \b/\B					1 if word boundry, 0 if not	*/
#define REG_ICR 6	/* Case-independent char	Character					*/
#define REG_BOL 7	/* ^						REG_NEWLINE					*/
#define REG_EOL 8	/* $						REG_NEWLINE					*/
#define REG_BOT 9	/* (						Next Alt (|) 2 Bytes		*/
#define REG_EOT 10	/* )						Capture # (0 for none)		*/
#define REG_BOW 11	/* <						(no parameter)				*/
#define REG_EOW 12	/* >						(no parameter)				*/
#define REG_REF 13	/* Backreference			Capture #					*/
#define REG_AND 14	/* &						Next Alt (|) 2 Bytes		*/
#define REG_ALT 15	/* | (Greedy/POSIX)			1st+2nd Bytes=Next Alt		*/
	 /*        3rd Byte: Depth to inval */
#define REG_CLO 16	/* Closure					2 characters: min and max	*/
#define REG_HUM 17	/* Non-greedy Closure		2 characters: min and max	*/
#define REG_GRE 18	/* Greedy Closure			2 characters: min and max	*/
#define REG_POS 19	/* Possessive Closure		2 characters: min and max	*/
#define REG_KPO 20	/* Keep Out; drop matches	(no parameter)				*/
#define REG_NCL 21	/* [^						16 byte bitmask				*/
#define REG_NLB 22	/* (?<! (Neg Lookbehind)	Next Alt (|) 2 Bytes		*/
#define REG_PLB 23	/* (?<= (Pos Lookbehind)	Next Alt (|) 2 Bytes		*/
#define REG_NLA 24	/* (?! (Neg Lookahead)		Next Alt (|) 2 Bytes		*/
#define REG_PLA 25	/* (?= (Pos Lookahead)		Next Alt (|) 2 Bytes		*/
#define REG_8CC 26	/* UTF8 Char				UTF8 Char (1-4 bytes)		*/
#define REG_8IC 27	/* UTF8 Case-independent	UTF8 Char (1-4 bytes)		*/
#define REG_CON 28	/* (?(If)Then|Else)			If then else conditional	*/

/* Unit Test Suite (system.h required) */
#if defined(WIKISERVER_VERSION) && defined(WSTESTOPT) && !defined(WSNTVREOPT)
WSTEST_SUITE_DECLARE(WSREGEX)
#endif

#if defined __cplusplus
}
#endif
#endif /* __RE_H__ */
#if defined(RE_IMPLEMENTATION)
/**
*	@file re.c
*
*	###Purpose###
*	Attempt at making a PHP-compatable regular expression engine
*	in the public domain that has POSIX compatibility as well.
*
*	Created because I was working on a public domain project that needed
*	to run on really old systems such as Mac OS System 6. PCRE -
*	the closest - was not public domain nor close to compiling on those
*	archaic systems.
*
*	@todo 1) Use an array and ditch regexec() recursion.
*	@todo
*	@todo 2) Add unicode support for bracket expressions and character classes
*	@todo
*	@todo LOOK INTO - Inline modifiers
*	@todo
*	@todo LOOK INTO - {,y} should print characters on REG_ENHANCED?? - testing various
*	@todo
*	@todo system regex libs reveals they all behave differents on this
*	@todo
*	@todo CONFIRM - Backref - $1-9, ${XXX}
*	@todo
*	@todo CONFIRM - \< \> GNU word boundary
*	@todo
*	@todo [[:<:]] [[:>]] POSIX word boundary
*	@todo
*	@todo CONFIRM - illegal to end RE with '\\'
*	@todo
*	@todo CONFIRM - REG_UNGREEDY, REG_ICASE, REG_NOSUB, REG_NOTEMPTY,
*	@todo
*	@todo CONFIRM - REG_STARTEND ([0]rm_so/rm_eo are start/end positions)
*	@todo
*	@todo CONFIRM - REG_NOSUB IGNORES_ nmatch and pmatch - doesn't return
*	main match either
*	@todo
*	@todo matches newline    yes (no for perl) REG_NEWLINE
*	@todo newline matches [^a]    yes REG_NEWLINE
*	@todo $ matches \\n at end  no  (yes for perl)   REG_NEWLINE
*	@todo $ matches \\n in middle  no  REG_NEWLINE
*	@todo ^ matches \\n in middle  no  REG_NEWLINE
*	@todo
*	@todo LOOK INTO - REG_DOTALL   0x0010 - REG_NEWLINE currently
*	lets dots match as newlines
*	@todo
*	@todo LOOK INTO - REG_UTF8   0x0040 - Everything outside of
*	brackets is treated as
*	@todo UTF8 already
*	@todo
*	@todo LOOK INTO - REG_UCP   0x0400
*	@todo
*	@todo CONFIRM - [[.col.]] [[=eqiuv=]]
*	@todo
*	@todo CONFIRM - \x{XXXX} - up to 32 bit value
*	@todo
*	@todo CONFIRM - (? - i - icase, n - newline, U - ungreedy;
*	@todo
*	@todo if flag is in does reverse
*	@todo
*	@todo CONFIRM - (?EXTRA#COMMENT) - EXTRA is ignored
*
*	@author Ryan Norton
*	@date 09/01/2004-04/06/2024
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

/*=============================================================================
								Headers
=============================================================================*/
/*#include "system.h"*/
#if defined(WSREGEXOPT) && !defined(WSNTVREOPT)
#ifndef PRECOMPILEDHEADERS
#include <string.h>	/* memXXX, strXXX */
#endif
/*#include "re.h"*/
#ifndef _WIN32
	#define _tcsncmp strncmp
#else
	#include <tchar.h> /* _tcsXXX */
#endif
/*=============================================================================
								Definitions
=============================================================================*/
/*	debug macro for LOTS of output	*/
#if 0
#include <stdio.h>
#define REGEXDEBUG(x) printf(x)
#define REGEXDEBUG2(x, y) printf(x, y)
#else
#define REGEXDEBUG(x)
#define REGEXDEBUG2(x, y)
#endif
/*	system function wrappers	*/
#ifdef REG_UNICODE
	/* native c libaries are unpredictable with wchar_t strlen */
	size_t regstrlen(register const regchar* s)
	{
		register size_t ret = 0;
		while (*s++) {++ret;}
		return ret;
	}
#ifdef _T
#undef _T
#endif
#define _T(x) L##x
#define regstrncmp(x, y, z) _tcsncmp(x, y, z)
#else /* !defined(REG_UNICODE) */
#define regstrlen(s) strlen(s)
#ifdef _T
#undef _T
#endif
#define _T(x) x
#define regstrncmp(x, y, z) strncmp(x, y, z)
#endif /* defined(REG_UNICODE) */
#define regmemcpy WSMemoryCopy
#define regmemmove WSMemoryMove
#define regmemset WSMemoryFill
#if defined(__clang__)
#define REGFALLTHROUGH __attribute__((fallthrough));
#else
#define REGFALLTHROUGH
#endif


#ifndef strcountof
#define countof(x) (sizeof(x) / sizeof(*x))
#define strcountof(x) (countof(x) - (1 * sizeof(*x))
#endif

/* #defines and common #define functions */
#define REG_TRUE	1
#define REG_FALSE	0
#define REG_LOOKAHEAD 1
#define REG_LOOKBEHIND 2
#define REG_LOOKNEG (1 << 4)
#define REG_MAXCHR	128
#define REG_CHRBIT	(8 * sizeof(regchar))
#define REG_BITBLK	REG_MAXCHR/REG_CHRBIT
#define REG_BLKIND	120
#define REG_BITIND	7
#define REG_MAXTAG 10
#define REG_CCL_MASK 255
#define REG_MAX_CAPTURES 255
#define REG_CAPTUREALL 4096
#define REG_FIXEDLEN 8192
#define REG_COMPCON 16384
#define REG_ISDIGIT(c) c >= '0' && c <= '9'
#define REG_ISRECHAR(x) ((x >= 'A' && x <= 'Z') \
	||	(x >= 'a' && x <= 'z') \
	|| (REG_ISDIGIT(x)) || x == '_')
#define reginascii(x)		(127&(x))
#define regiswordc(x)		REG_ISRECHAR(reginascii(x))
static
int regisinset(const regchar* x, regchar y) /* func avoiding multiple y's */
{ return ((x)[((y) & REG_BLKIND)  >> 3] & (1 << ((y) & REG_BITIND))); }
static
void regaddtoset(regchar* bittab, regchar c) /* func avoiding multiple c's */
{ bittab[((c) & REG_BLKIND) >> 3] |= (1 << ((c) & REG_BITIND)); }

/* ANSII tolower() */
regchar regtolower(regchar x)
{
	if (x >= 'A' && x <= 'Z')
		x += ('a' - 'A');
	return x;
}

/*		forwards C++ compatability	*/
#ifdef __cplusplus
extern "C" {
#if __cplusplus >= 199711L
#define register
#endif
#	define REG_CPP_CAST(c) (c)
#else
#	define REG_CPP_CAST(c) WSUNUSEDPARAM(c)
/* Some compilers give depreciation warnings for register, clang doesn't */
#	if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L \
&& !defined(__clang__)
#		define register
#	endif
#endif

/* Signals a closure with no upper match limit */
#define REG_INF 0

/*	skip values for CLO XXX to skip past the closure */
/* [CLO] ANY nln ...	  */
#define REG_ANYSKIP 2
/* [CLO] CHR chr ...	  */
#define REG_CHRSKIP 2
/* [CLO] CCL 16bytes ... */
#define REG_CCLSKIP 17
/* CLO min max */
#define REG_CLOSKIP 3
#define REG_CLODATA 2
/* REG_ALT/BOT nextalt2bytes toinval */
#define REG_ALTBOTSKIP 5
#define REG_ALTBOTDATA 4
#define REG_ALTBOTNEXTALTDATA 2

/* bit table character classes, must match ctypes array in storeset() */
#define REG_CCLASS_W		13	/*[:w:]*/
#define REG_CCLASS_WORD		15	/*[:word:] -> [A-Za-z0-9_]*/
#define REG_CCLASS_ALNUM	0	/*[:alnum:] -> [A-Za-z0-9]*/
#define REG_CCLASS_ALPHA	1	/*[:alpha:] -> [A-Za-z]*/
#define REG_CCLASS_BLANK	2	/*[:blank:] -> [ \t]*/
#define REG_CCLASS_CNTRL	3	/*[:cntrl:] -> [\x00-\x1F\x7F]*/
#define REG_CCLASS_D		12	/*[:d:]*/
#define REG_CCLASS_DIGIT	4	/*[:digit:] -> [0-9]*/
#define REG_CCLASS_LOWER	6	/*[:lower:] -> [a-z]*/
#define REG_CCLASS_PRINT	7	/*[:print:] -> [[:graph:] ]*/
#define REG_CCLASS_GRAPH	5	/*[:graph:] -> [^ [:cntrl:]]*/
#define REG_CCLASS_PUNCT	8	/*[:punct:] -> [-!"#$%&'()*+,./:;<=>?@[]^_`{|}~]*/
#define REG_CCLASS_S		14	/*[:s:]*/
#define REG_CCLASS_SPACE	9	/*[:space:] -> [ \t\n\r\f\v]*/
#define REG_CCLASS_UPPER	10	/*[:upper:] -> [A-Z]*/
#define REG_CCLASS_XDIGIT	11	/*[:xdigit:] -> [0-9A-Fa-f]*/
#define REG_CCLASS_ASCII	16	/*[:ascii:] -> [\x00-\x7F] */
#define REG_CCLASS_NODIGIT	17	/*[:nodigit:]*/
#define REG_CCLASS_NOSPACE	18	/*[:nospace:]*/
#define REG_CCLASS_NOWORD	19	/*[:noword:]*/

/* matching direction */
#define REG_FORWARD		1
#define REG_BACKWARD	0

/* Used in asserts to test if a subexpression has the proper state */
#define regisbottype(x) ((x) == REG_BOT || (x) == REG_NLB \
|| (x) == REG_PLB || (x) == REG_NLA || (x) == REG_PLA)

/* Use recursion or allocation in regexec when encountering BOT and LOOKXXXS */
#define REGEXECMETHOD_RECURSE	1
#define REGEXECMETHOD_ALLOC		0
#define REGEXECMETHOD REGEXECMETHOD_RECURSE  /*REGEXECMETHOD_ALLOC*/
/*=============================================================================
								Implementation
=============================================================================*/
/**
* [Private] Contains compilation info about a expression, one is created by
* regcomp() for each expression and subexpression.
*/
typedef struct regsubexpr_t
{
	regchar* patloc;	/**< pattern location for ( used for closure */
	regchar* altloc;	/**< pattern location for (/| used for | */
	regchar regchrtype;	/**< REG_CHR or REG_ICR to store normal chars */
	regchar* fixlenloc;	/**< location to store fixlen */
	uregchar fixlen;	/**< length used for lookbehind */
	uregchar capidx;	/**< capture index for ) */
	int cflags;			/**< flags for this subexpression */
} regsubexpr_t;

/**
* [Private] Puts a character class instruction ([:EXPRESSION:]) into a
* 16-byte bit table
*
* @param bittab 16 byte bit table
* @param n REG_CCLASS_XXX macro type
*/
static void regaddctypetoset(regchar bittab[], regchar n)
{ /* https://github.com/micromatch/posix-character-classes */
	switch(n)
	{
	case REG_CCLASS_W: /*[:w:]*/
	case REG_CCLASS_WORD: /*[:word:] -> [A-Za-z0-9_]*/
		regaddtoset(bittab, '_');
		REGFALLTHROUGH /* fallthrough */
	case REG_CCLASS_ALNUM: /*[:alnum:] -> [A-Za-z0-9]*/
		for (n = '0'; n <= '9'; ++n) /*0-9*/
			regaddtoset(bittab, n);
		REGFALLTHROUGH /* fallthrough */
	case REG_CCLASS_ALPHA: /*[:alpha:] -> [A-Za-z]*/
		for (n = 'A'; n <= 'Z'; ++n) /*A-Z*/
			regaddtoset(bittab, n);
		for (n = 'a'; n <= 'z'; ++n) /*a-z*/
			regaddtoset(bittab, n);
		break;
	case REG_CCLASS_BLANK: /*[:blank:] -> [ \t]*/
		regaddtoset(bittab, ' ');
		regaddtoset(bittab, '\t');
		break;
	case REG_CCLASS_CNTRL: /*[:cntrl:] -> [\x00-\x1F\x7F]*/
		for (n = 0; n <= 31; ++n)
			regaddtoset(bittab, n);
		regaddtoset(bittab, 127);
		break;
	case REG_CCLASS_D: /*[:d:]*/
	case REG_CCLASS_DIGIT: /*[:digit:] -> [0-9]*/
		for (n = '0'; n <= '9'; ++n) /*0-9*/
			regaddtoset(bittab, n);
		break;
	case REG_CCLASS_LOWER: /*[:lower:] -> [a-z]*/
		for (n = 'a'; n <= 'z'; ++n) /*a-z*/
			regaddtoset(bittab, n);
		break;
	case REG_CCLASS_PRINT: /*[:print:] -> [[:graph:] ]*/
		regaddtoset(bittab, ' ');
		REGFALLTHROUGH /* fallthrough */
	case REG_CCLASS_GRAPH: /*[:graph:] -> [^ [:cntrl:]]*/
		for (n = '0'; n <= '9'; ++n) /*0-9*/
			regaddtoset(bittab, n);
		for (n = 'A'; n <= 'Z'; ++n) /*A-Z*/
			regaddtoset(bittab, n);
		for (n = 'a'; n <= 'z'; ++n) /*a-z*/
			regaddtoset(bittab, n);
		REGFALLTHROUGH /* fallthrough */
	case REG_CCLASS_PUNCT: /*[:punct:] -> [-!"#$%&'()*+,./:;<=>?@[]^_`{|}~]*/
		for (n = '!'; n <= '/'; ++n) /* !-/ (slash confuses doxygen) */
			regaddtoset(bittab, n);
		for (n = ':'; n <= '@'; ++n) /*:-@*/
			regaddtoset(bittab, n);
		for (n = '['; n <= '`'; ++n) /*[-`*/
			regaddtoset(bittab, n);
		for (n = '{'; n <= '~'; ++n) /*{-~*/
			regaddtoset(bittab, n);
		break;
	case REG_CCLASS_S: /*[:s:]*/
	case REG_CCLASS_SPACE: /*[:space:] -> [ \t\n\r\f\v]*/
		regaddtoset(bittab, '\t');
		regaddtoset(bittab, '\f');
		regaddtoset(bittab, '\v');
		regaddtoset(bittab, '\n');
		regaddtoset(bittab, '\r');
		regaddtoset(bittab, ' ');
		break;
	case REG_CCLASS_UPPER: /*[:upper:] -> [A-Z]*/
		for (n = 'A'; n <= 'Z'; ++n) /*A-Z*/
			regaddtoset(bittab, n);
		break;
	case REG_CCLASS_XDIGIT: /*[:xdigit:] -> [0-9A-Fa-f]*/
		for (n = '0'; n <= '9'; ++n) /*0-9*/
			regaddtoset(bittab, n);
		for (n = 'A'; n <= 'F'; ++n) /*A-F*/
			regaddtoset(bittab, n);
		for (n = 'a'; n <= 'f'; ++n) /*a-f*/
			regaddtoset(bittab, n);
		break;
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4310) /* cast truncates constant value */
#endif
	case REG_CCLASS_ASCII: /*[:ascii:] -> [\x00-\x7F] */
		for (n = 0x00; n != (regchar)(0x80); ++n) /*\x00-\x7F*/
			regaddtoset(bittab, n);
		break;
	case REG_CCLASS_NODIGIT: /*[:nodigit:]*/
		for (n = 0x00; n != (regchar)(0x80); ++n)
			if (n < '0' || n > '9')
				regaddtoset(bittab, n);
		break;
	case REG_CCLASS_NOSPACE: /*[:nospace:]*/
		for (n = 0x00; n != (regchar)(0x80); ++n)
			if (n != '\t' && n != '\f' && n != '\v'
				&& n != '\n' && n != '\r' && n != ' ')
				regaddtoset(bittab, n);
		break;
	case REG_CCLASS_NOWORD: /*[:noword:]*/
		for (n = 0x00; n != (regchar)(0x80); ++n)
			if (!(n >= '0' && n <= '9') && !(n >= 'A' && n <= 'F')
				&& !(n >= 'a' && n <= 'f') && n != '_')
				regaddtoset(bittab, n);
		break;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
	}
}

/**
* [Private] Puts a backslash (\\) escaped character from pointer to string p
* into 16 byte bit table bittab.
*
* @param bittab 16 byte bit table
* @param p pattern containing the slash and following characters,
* starting at index 1, not 0
* @return raw escaped character if not an escape expression, 0 otherwise
 */
regchar regaddescapetoset(char* bittab, const regchar** p)
{ /* REG_ENHANCED-specific escapes */
	regchar mask, n;
	switch (*++*p)
	{
	case 'a':
		regaddtoset(bittab, '\a');
		break;
	case 'e':
		regaddtoset(bittab, 27); /* \e is non-standard C */
		break;
	case 'n':
		regaddtoset(bittab, '\n');
		break;
	case 'f':
		regaddtoset(bittab, '\f');
		break;
	case 'r':
		regaddtoset(bittab, '\r');
		break;
	case 't':
		regaddtoset(bittab, '\t');
		break;
	case 'v':
		regaddtoset(bittab, '\v');
		break;
	case 'd': /* decimal: [0-9] */
		regaddctypetoset(bittab, REG_CCLASS_DIGIT);
		break;
	case 'D': /* non-decimal: [^0-9] */
		regaddctypetoset(bittab, REG_CCLASS_NODIGIT);
		break;
	case 's': /* white-space: [ \f\n\r\t\v] */
		regaddctypetoset(bittab, REG_CCLASS_SPACE);
		break;
	case 'S': /* non-white-space: [^ \f\n\r\t\v] */
		regaddctypetoset(bittab, REG_CCLASS_NOSPACE);
		break;
	case 'w': /* word character: [a-zA-Z_0-9] */
		regaddctypetoset(bittab, REG_CCLASS_WORD);
		break;
	case 'W': /* non-word character: [^a-zA-Z_0-9] */
		regaddctypetoset(bittab, REG_CCLASS_NOWORD);
		break;
	case 'x': /* \xXX XX = hex character value */
		mask = 0;
		for (n = 0; n < 2; ++n)
		{
			if (*++*p >= '0' && **p <= '9')
				mask += (**p - '0') << (4 * (1-n));
			else if (**p >= 'A' && **p <= 'F')
				mask += ((**p - 'A') + 10) << (4 * (1-n));
			else if (**p >= 'a' && **p <= 'f')
				mask += ((**p - 'a') + 10) << (4 * (1-n));
			else
			{
				--*p;
				return 0;
			}
		}
		regaddtoset(bittab, mask);
		return mask;
	default:
		regaddtoset(bittab, **p);
		return **p;
	} /* end ENHANCED switch */
	return 0;
}

/**	[Private] Stores a value in the NFA, here more for programmer reference */
#define store(x)	*mp++ = x

/** [Private] stores p in a 16 byte bit table */
#define storeset(p) \
{\
	c1 = 0;\
	regmemset(bittab, 0, REG_BITBLK * sizeof(regchar));\
	if (*++p == '^')\
	{\
		mask = (regchar)(size_t)REG_CCL_MASK;\
		p++;\
		store(REG_NCL);\
	}\
	else\
	{\
		mask = 0;\
		store(REG_CCL);\
	}\
	if (*p == '-')		/* real dash */\
		regaddtoset(bittab, *p++);\
	if (*p == ']')		/* real brac */\
		regaddtoset(bittab, *p++);\
	while (*p && *p != ']')\
	{\
		if (*p == '-' && *(p+1) && *(p+1) != ']')\
		{\
			if (*++p == '\\')\
			{\
				if (se[tagi].cflags & REG_ENHANCED)\
				{\
					c2 = regaddescapetoset(bittab, (const regchar**)&p);\
					c2 -= c2 ? 1 : 0;\
				}\
				else\
					c2 = *++p;\
			}\
			else\
				c2 = *p;\
			++p;\
			while (++c1 <= c2)\
				regaddtoset(bittab, (regchar)c1);\
		}\
		else if (*p == '\\' && *(p+1))\
		{\
			if (se[tagi].cflags & REG_ENHANCED)\
				c1 = regaddescapetoset(bittab, (const regchar**)&p);\
			else\
				regaddtoset(bittab, (regchar)(c1 = *++p));\
			++p;\
		}\
		else if (*p == '[' && *(p+1) == ':')\
		{\
			const regchar* ctypes[] = { _T("alnum"), _T("alpha"), _T("blank"),\
				_T("cntrl"), _T("digit"), _T("graph"), _T("lower"),\
				_T("print"), _T("punct"), _T("space"), _T("upper"),\
				_T("xdigit"), _T("d"), _T("w"), _T("s"), _T("word"),\
				_T("ascii"), _T("nodigit"), _T("nospace"), _T("noword") };\
			for (sp = (regchar*)p+2; *sp && *sp != ':'; ++sp) {}\
			for (n = 0; n < (regchar)(sizeof(ctypes) / sizeof(*ctypes)); ++n)\
			{\
				if (regstrncmp(p+2, ctypes[(int)n], sp-p+2) == 0)\
					break;\
			}\
			if (n == (sizeof(ctypes) / sizeof(*ctypes)))\
			{ ret = REG_ECTYPE; break; }\
			if ((se[tagi].cflags & REG_ICASE) && (n == 6 || n == 10))\
				n = 1; /* lower/upper -> alpha on REG_ICASE */\
			regaddctypetoset(bittab, n);\
			c1 = 0;\
		}\
		else\
		{\
			if ((se[tagi].cflags & REG_ICASE) && *p >= 'a' && *p <= 'z') \
				regaddtoset(bittab, (regchar)(*p - ('a' - 'A')));\
			c1 = *p;\
			regaddtoset(bittab, *p++);\
		}\
	}\
		/*already checked at beginning*/\
	/*if (!*p)*/\
		/*return EBRACK;*/\
	if(ret == REG_OK)\
		for (n = 0; n < (regchar)(REG_BITBLK);)/*bittab[n++] = (regchar) 0)*/\
			store(mask ^ bittab[(size_t)n++]);\
}

/**
* Duplicates regex_t rsrc into rdest.
*
* @param rdest destination for the clone
* @param rsrc the source to clone into rdest
* @return REG_OK on success, REG_SRCNINIT if the source wasn't
* compiled, REG_DESTINIT if the dest WAS compiled, REG_ESPACE
* if the allocation for the clone fails
*/
int regclone(register regex_t* rdest, register const regex_t* rsrc)
{
	if (rsrc->nfaSize == 0)
		return REG_SRCNINIT;
	if (rdest->nfaSize != 0)
		return REG_DSTINIT;

	rdest->nfa = REG_CPP_CAST(regchar*)
		malloc(rdest->nfaSize = rsrc->nfaSize);
	if (!rdest->nfa)
		return REG_ESPACE;
	regmemcpy(rdest->nfa, rsrc->nfa, rdest->nfaSize);
	rdest->re_nsub = rsrc->re_nsub;
	return REG_OK;
}

/**
* [Private] Called to perform the grunt work of compiling a closure once it is
* known that the syntax for a closure is good and the min/max is determined.
*
* @param se regsubexpr_t containing compilation info for the expression
* and subexpressions
* @param tagi tag - or expression - index we are currently at, 0-based
* @param p beginning point of closer, the '{' in "{1,3}"
* @param pat pointer after the closure
* @param lp current opcode
* @param sp previous opcode
* @param mp NFA
* @return Various including REG_EMPTYRPT is closure is empty
*  - REG_BADBR on misplaced brackets
*  - REG_BADRPT if qualifier syntax is wrong
*  - REG_OK on success
*/
int regcompclosure(regsubexpr_t* se, int tagi,
				   const regchar* p, const regchar* pat,
				   regchar** lp, regchar* sp, regchar** mp,
				   regchar* min, regchar* max)
{
	size_t tomove;
	if (p == pat)		/* Nothing before closure == empty closure */
		return REG_EMPTYRPT;
	/* determine the min/max for the closure type */
	if (*p == '{')
	{ /* initial \ for BASIC/EXTENDED distinction is already checked outside */
		/* outer if - left side, inner - right */
		if (*++p == ',') /* {,[x]} == 0-([x] or INF)*/
		{
			++p; /* skip past ',' */
			if ((!(se[tagi].cflags & (REG_EXTENDED)) && *p == '\\')
				|| ((se[tagi].cflags & (REG_EXTENDED))
					&& *p == '}'))
			{ /* {,} */
				*min = 0; *max = REG_INF;
				if (!(se[tagi].cflags & (REG_EXTENDED))
					&& *++p != '}')
					return REG_BADBR;
			}
			else if (*p >= '0' && *p <= '9') /* {,x} */
			{
				*min = 0; *max = *p - '0';
				if ((!(se[tagi].cflags & (REG_EXTENDED))
						&& (*++p != '\\' || *++p != '}'))
					|| ((se[tagi].cflags & (REG_EXTENDED))
						&& *++p != '}'))
					return REG_BADBR;
			}
			else
				return REG_BADBR;
		}
		else if (*p >= '0' && *p <= '9') /* {x[,y]} */
		{
			if ((se[tagi].cflags & (REG_EXTENDED))
				&& *++p == '}')
			{ /* {x} == exactly x matches */
				*min = *max = *(p - 1) - '0';
			}
			else if (!(se[tagi].cflags & (REG_EXTENDED))
					&& *++p == '\\')
			{
				*min = *(p - 2) - '0'; *max = REG_INF;
				if (*++p != '}')
					return REG_BADBR;
			}

			if (*p++ == ',') /* {x,[y]} == x-([y] or INF) */
			{
				if (*p >= '0' && *p <= '9') /* {x,y} == x-y */
				{
					*min = *(p - 2) - '0'; *max = *p - '0';
					if ((!(se[tagi].cflags & (REG_EXTENDED))
							&& (*p != '\\' || *++p != '}'))
						|| ((se[tagi].cflags & (REG_EXTENDED))
							&& *++p != '}')
						|| *(p - 2) > *p) /* x > y is invalid */
						return REG_BADBR;
				}
				else /* {x,} == x-INF */
				{
					*min = *(p - 2) - '0'; *max = REG_INF;
					if ((!(se[tagi].cflags & (REG_EXTENDED))
							&& (*p != '\\' || *++p != '}'))
						|| ((se[tagi].cflags & (REG_EXTENDED))
							&& *p != '}'))
						return REG_BADBR;
				}
			}
		} /* end {x,y} check for x */
		else
			return REG_BADBR;
	}
	else if (*p == '*')
	{
		*min = 0; *max = REG_INF;
	}
	else if (*p == '+')
	{
		*min = 1; *max = REG_INF;
	}
	else /* '?' */
	{
		*min = 0; *max = 1;
	}
	/* find out if the previous char conflicted or is a closure itself */
	switch(*sp)
	{
	case REG_HUM:
	case REG_GRE:
		*lp = sp; /* keep lp at closure opcode */
		if (*p == '+')
			*sp = REG_POS;
		else if (*p == '?')
		{
			if (se[tagi].cflags & REG_UNGREEDY)
			{ /* switch to REG_GRE (Greedy) */
				if (*sp == REG_GRE)
					return REG_BADRPT;
				*sp = REG_GRE;
			}
			else
			{ /* switch to REG_HUM (Non-greedy) */
				if (*sp == REG_HUM)
					return REG_BADRPT;
				*sp = REG_HUM;
			}
		}
		else /* only ? and + can be used to change closures */
			return REG_BADRPT;
		return REG_OK; /* quantifier handled and changed, return */
	case REG_EOT:
		*lp = sp = se[tagi + 1].patloc; /* move to BOT location */
		break;
	case REG_BOL:
	case REG_BOT:
	case REG_BOW:
	case REG_EOW:
	case REG_REF:
	case REG_POS:  /* Already possessive, can't modify */
		return REG_BADRPT;
	default:
		*lp = sp; /* keep lp at previous opcode */
		break;
	}
	/* insert the REG_GRE/POS/HUM before the last entry,
	   and move the last entry up */
	tomove = (*mp) - sp;
	regmemmove((*mp)-tomove+REG_CLOSKIP, (*mp)-tomove, tomove);
	(*mp) = sp;
	if (se[tagi].cflags & REG_UNGREEDY)
		*(*mp)++ = REG_HUM;
	else
		*(*mp)++ = REG_GRE;
	*(*mp)++ = *min;
	*(*mp)++ = *max;
	(*mp) += tomove;
	return REG_OK;
}
/**
* Compiles regex pattern pat into a NFA (Nondeterministic finite automaton)
* See the header for the states and thier bytes. ALT is the only one that
* needs an explanation. It's 1 Byte ALT (15) followed by 2 bytes, an
* unsigned short with the location of the next ALT's 2 bytes or the byte
* after an REG_EOT ( ")" closing parenthases ). This includes a count of
* 2 for the 2 bytes needed for the unsigned short, so it's 2 + NFA bytes
* between it and the next ALT/EOT, then 1 byte to skip over the ALT/EOT byte.
*
* @param r regex_t that will contain the compiled expression
*
* @param pat pattern/expression to compile
*
* @param cflags compilation flags
*
* @return REG_OK on success, see re.h for the 20-ish failure returns
* that can occur. Use regerror() to get extended error information.
*/
int regcomp(register regex_t* r,
			register const regchar *pat, register int cflags)
{
	const regchar* p;								/* pattern pointer		*/
	regchar* mp = (regchar*) pat;					/* nfa pointer			*/
	regchar* lp;									/* saved pointer..		*/
	regchar* sp;									/* another one..		*/
	register uregchar tagi = 0;						/* last unmatched (		*/
	register uregchar tagc = 1;				/* number of captures (1+#of('s)*/
	regchar n = 0;							/* bittab byte count			*/
	regchar mask;							/* xor mask -CCL/NCL			*/
	regchar bittab[REG_BITBLK];				/* bit table for CCL			*/
	uint32_t c1 = 0, c2 = 0, ret;			/* temporary/return variables	*/
	regsubexpr_t* se;						/* holds temporories for each (	*/

	/*
		Intialize re_nsub of the handle, which will contain the number
		of captured subexpressions
	*/
	r->re_nsub = 0;

	/* Empty pattern?  If so then according to posix
	   specifications an empty pattern matches anything.
	   Return REG_NOSTRING for advanced. */
	if (!pat || !*pat)
	{
		if (cflags & REG_ENHANCED)
		{
			r->nfa = NULL;
			return REG_NOSTRING;
		}
		r->nfa = _T("");
		return REG_OK;
	}

	/* Loop through and find out how many CCLs and parenthases we have
	to allocate memory for NFA (c1 == closures+alts, c2 == cloquantifiers,
	tagi == brack, n == brackcontentsize, tagc == captures,
	mp == position in pattern, lp == start of bracket expression) */
	while (*mp)
	{
		if (*mp == '[')
		{
			++tagi;
			lp = mp;
			while (*++mp && *mp != ']') {}
			if (!*mp || (mp - lp - 1 > 255))
			{
				r->nfa = NULL;
				r->pat = pat;
				r->patPos = mp - pat;
				return REG_EBRACK;
			}
			/* +1 for the beginning [, then 1 for all incl ] */
			n += (regchar)(mp - lp) + 1;
			c1++; /* for the REG_CCL byte */
		}
		else if (*mp == '(')
		{
			if (++tagc == 255)
			{
				r->nfa = NULL;
				r->pat = pat;
				r->patPos = mp - pat;
				/* ending EOT is tagc+1, so tagc+1==255 is max */
				return REG_EPAREN;
			}
			c1 += 4;
		}
		else if (*mp == '|')
			c1 += 3;
		else if (*mp == '+' || *mp == '?' || *mp == '*')
		{
			if (*(mp+1) != '+' && *(mp+1) != '?' && *(mp+1) != '*')
				++c1;
			else
			{
				++mp;
				++c2;
			}
		}
		else if (*mp == '\\')
		{
			if ((*(mp+1) == 'd' || *(mp+1) == 'D'
					|| *(mp+1) == 's' || *(mp+1) == 'S'
					|| *(mp+1) == 'w' || *(mp+1) == 'W'))
			{
				++tagi;
				++c2;
			}
			++mp;
			++n;
		}
		if (UTF8CharIsLeading(*mp))
			while (UTF8CharIsTrailing(*(mp + 1)))
			{ ++mp; ++c2; }
		else
			++mp;
	}

	/* 255 (256 bytes) is the max number of characters supported
	   by POSIX regular expressions */
	if (!(cflags & REG_ENHANCED) && (mp - pat) > 255)
		return REG_BADPAT;

	r->nfaSize = ( /* the size here is just approximate over the amount we need */
		((mp - pat) * 2) /* General status take up 2 bytes (STATE STATEVAL) */
		+ c1 /* extra bytes in closures, BOTs and ALTs */
		- c2  /* bytes not needed in closures in and UTF8 characters */
		+
		((tagi * (REG_BITBLK)) - (n * 2)) /* bytes needed for bit tables ( [] ) */
		+ 8 /* 8 for beginning and ending BOT+EOT */
		) * sizeof(regchar);
	sp = mp = r->nfa = REG_CPP_CAST(regchar*) malloc(r->nfaSize * sizeof(regchar));
	se = REG_CPP_CAST(regsubexpr_t*) malloc(tagc * sizeof(regsubexpr_t));
	r->numBot = (size_t)tagc;

	/* Out of memory? */
	if (!r->nfa)
	{
		free(se);
		return REG_ESPACE;
	}
	if (!se)
	{
		free(r->nfa);
		r->nfa = NULL;
		return REG_ESPACE;
	}
	tagc = 1;
	tagi = 0;
	se[0].cflags = cflags;
	if (se[0].cflags & REG_ICASE)
		se[0].regchrtype = REG_ICR;
	else
		se[0].regchrtype = REG_CHR;
	se[0].patloc = r->nfa;
	se[0].altloc = r->nfa + 1;
	se[0].fixlenloc = r->nfa + 4;
	se[0].fixlen = 0;

	/* Set up beginning state */
	store(REG_BOT);
	store(0); /* 1st of 2 bytes containing next ALT location */
	store(0); /* 2nd of 2 bytes containing next ALT location */
	store(0); /* byte containing capture number to invalidate on failure */
	store(0); /* byte containing length if fixed */

	/*
		Enter loop which parses the input pattern
		character by character

		p == current pattern position
		lp == last position in compiled NFA
		mp == current position in compiled NFA
		ap == will point to alt if one is found
		capidx = capture # for end parenthases
	*/
	for (ret = REG_OK, p = REG_CPP_CAST(regchar*) pat; *p; ++p, sp = lp)
	{
		/* Get start position in NFA */
		lp = mp;

		/* Parse pattern character */
		switch(*p)
		{
		/*
			'.'
			POSIX - match any character,
					or any except '\n' if REG_NEWLINE is specified
			VBSCRIPT - many any character except '\n'

			Rather than using a specific macro,
			we simulate the '.' using a negated character class.
		*/
		case '.':
			store(REG_ANY);
			store(se[tagi].cflags & REG_NEWLINE);
			++se[tagi].fixlen;
			break;
		/*
			'^'
			Match beginning of the search string.

			If REG_NEWLINE is specified it matches if the last
			character in the search string was a '\n' also.
		*/
		case '^':
			/* End of pattern or REG_NEWLINE specified?	 Add meta version */
			if ((p == pat || (se[tagi].cflags & REG_NEWLINE)) && !(se[tagi].cflags & REG_NOTBOL))
			{
				store(REG_BOL);
				store(se[tagi].cflags & REG_NEWLINE);
			}
			else /* Neither specified - add '$' character to pattern */
			{
				store(REG_CHR);
				store(*p);
				++se[tagi].fixlen;
			}
			break;
		/*
			'$'
			Match the end of the search string ('\0').

			If REG_NEWLINE is specified it also matches if the next
			character in the search string is a '\n'.
		*/
		case '$':
			/* End of pattern or REG_NEWLINE specified?	 Add meta version */
			if ((!*(p+1) || (se[tagi].cflags & REG_NEWLINE)) && !(se[tagi].cflags & REG_NOTEOL))
			{
				store(REG_EOL);
				store(se[tagi].cflags & REG_NEWLINE);
			}
			else /* Neither specified - add '$' character to pattern */
			{
				store(REG_CHR);
				store(*p);
				++se[tagi].fixlen;
			}
			break;
		/*
			'['
			Match a character class.

			Syntax types:
			[:x:] - match a named character class ("digit")
			x1-x2 - match from x1 to x2 (x1 <= input <= x2)
			[[ - simply adds more to the existing character class
		*/
		case '[':
			storeset(p);
			++se[tagi].fixlen;
			break;
		/*
			'*'
			Match zero or more of an expression.
		*/
		case '*':				/* match 0 or more.. */
			if((se[tagi].cflags & (REG_FIXEDLEN)))
			{
				ret = REG_REQFIXED;
				break;
			}
			if(*sp != REG_EOT)
				--se[tagi].fixlen;
			ret = regcompclosure(se, tagi, p, pat, &lp, sp, &mp, &n, &mask);
			break;
		/*
			'?'
			EXTENDED - Match zero or one of an expression.
		*/
		case '?':
			if((se[tagi].cflags & (REG_FIXEDLEN)))
			{
				ret = REG_REQFIXED;
				break;
			}
			if (!(se[tagi].cflags & (REG_EXTENDED)))
			{
				store(REG_CHR);
				store(*p);
				++se[tagi].fixlen;
				break;
			}
			if(*sp != REG_EOT)
				--se[tagi].fixlen;
			ret = regcompclosure(se, tagi, p, pat, &lp, sp, &mp, &n, &mask);
			break;
		/*
			'+'
			EXTENDED - Match one or more of an expression.
		*/
		case '+':
			if((se[tagi].cflags & (REG_FIXEDLEN)))
			{
				ret = REG_REQFIXED;
				break;
			}
			if (!(se[tagi].cflags & (REG_EXTENDED)))
			{
				store(REG_CHR);
				store(*p);
				++se[tagi].fixlen;
				break;
			}
			ret = regcompclosure(se, tagi, p, pat, &lp, sp, &mp, &n, &mask);
			break;
		/*
			'|'
			EXTENDED - Match last expression OR next one.
		*/
		case '|':
			if (!(se[tagi].cflags & (REG_EXTENDED)))
			{
				store(REG_CHR);
				store(*p);
				++se[tagi].fixlen;
				break;
			}
			if((se[tagi].cflags & (REG_COMPCON))
				&& (*(se[tagi].altloc - 1)) != REG_CON)
			{
				ret = REG_BADCON;
				break;
			}
			store(REG_ALT);
			*((unsigned short*)se[tagi].altloc) =
				(unsigned short)(mp - se[tagi].altloc);
			se[tagi].altloc = (regchar*)mp;
			store(0); /* 1st of 2 bytes containing next ALT location */
			store(0); /* 2nd of 2 bytes containing next ALT location */
			/* byte containing capture number to invalidate on failure */
			store(tagc);
			store(0); /* fixed length amount */
			*se[tagi].fixlenloc = se[tagi].fixlen;
			se[tagi].fixlenloc = mp - 1;
			se[tagi].fixlen = 0;
			break;
#if 0 /* SADLY breaks compat with too many other regex expressions */
		/*
			'&'
			ENHANCED - Match last expression AND next one.
		*/
		case '&':
			if (se[tagi].cflags & REG_ENHANCED)
			{
				store(REG_AND);
				store(0); /* fixed length amount */
				*se[tagi].fixlenloc = n;
				se[tagi].fixlenloc = mp - 1;
				se[tagi].fixlen = 0;
			}
			else
			{
				store(REG_CHR);
				store(*p);
				++se[tagi].fixlen;
			}
			break;
#endif
		/*
			'('
			Start subexpression
			"(?[lookaround]THEN|ELSE) == conditional (non-capturing)
			"(?:" == subexpression (non-capturing)
			"(?!" == negative lookahead (non-capturing)
			"(?=" == positive lookahead (non-capturing)
			"(?<!" == negative lookbehind (non-capturing)
			"(?<=" == positive lookbehind (non-capturing)

		*/
		case '(':
#if 0
			if((se[tagi].cflags & (REG_FIXEDLEN)))
			{
				ret = REG_LOOKINSUB;
				break;
			}
			cflags &= ~REG_FIXEDLEN; /* turn it off unless we get a LB again */
#endif
			if (!(se[tagi].cflags & (REG_EXTENDED))
				&& (p == pat || *(p-1) != '\\'))
			{ /* BASIC - Must be preceded by \ */
				ret = REG_EPAREN;
				break;
			}
			if ((se[tagi].cflags & REG_ENHANCED) && p[1] == '?')
			{
				se[++tagi].capidx = 0;
				if(p[2] == '(' && p[3] == '?'
						&& (p[4] == '!' || p[4] == '='
							|| (p[4] == '<' && p[5] == '!')
							|| (p[4] == '<' && p[5] == '='))
					)
				{/* if then else conditional */
					store(REG_CON);
					se[tagi].cflags |= REG_COMPCON;
					p += 1;
				}
				else if (p[2] == '<' && p[3] == '!')
				{
					p += 3;
					se[tagi].cflags |= REG_FIXEDLEN;
					store(REG_NLB);
				}
				else if (p[2] == '<' && p[3] == '=')
				{
					p += 3;
					se[tagi].cflags |= REG_FIXEDLEN;
					store(REG_PLB);
				}
				else if (p[2] == '!')
				{
					p += 2;
					store(REG_NLA);
				}
				else if (p[2] == '=')
				{
					p += 2;
					store(REG_PLA);
				}
				else if (p[2] == ':') /* (?: == no saved match */
				{
					p += 2;
					store(REG_BOT);
				}
				else
				{
					ret = REG_EPAREN;
					break;
				}
			}
			else
			{
				if ((tagc - 1) == REG_MAX_CAPTURES)
				{
					ret = REG_EPAREN;
					break;
				}
				se[++tagi].capidx = ++tagc;
				store(REG_BOT); /* positioning important since mp is moved by this */
			}
			se[tagi].cflags = cflags;
			se[tagi].patloc = mp - 1;
			se[tagi].altloc = mp;
			se[tagi].regchrtype = se[tagi - 1].regchrtype;
			store(0); /* 1st of 2 bytes containing next ALT location */
			store(0); /* 2nd of 2 bytes containing next ALT location */
			/* byte containing capture number to invalidate on failure */
			store(tagc);
			store(0); /* fixed length amount */
			se[tagi].fixlenloc = mp - 1;
			se[tagi].fixlen = 0;
			break;
		/*
			')'
			End subexpression
		*/
		case ')':
			if (tagi <= 0
				|| (!(se[tagi].cflags & (REG_EXTENDED))
					&& (p == pat || *(p-1) != '\\')))
			{ /* BASIC - Must be preceded by \ */
				ret = REG_EPAREN;
				break;
			}
			if((se[tagi].cflags & (REG_COMPCON))
				&& (*(se[tagi].altloc - 1)) != REG_ALT)
			{
				ret = REG_BADCON;
				break;
			}
			store(REG_EOT);
			*((unsigned short*)se[tagi].altloc) =
				(unsigned short)(mp - se[tagi].altloc);
			*se[tagi].fixlenloc = se[tagi].fixlen;
			store(se[tagi--].capidx);
			break;
		/*
			'{'
			EXTENDED
			Closure in form {[x][,][y]} == x-y, x-INF, 0-y, or 0-INF
			Note that modern REs treat as literal chars for {,} and {,x}
		*/
		case '{':
			if (se[tagi].cflags & REG_EXTENDED)
			{
				c1 = *sp != REG_EOT;
				ret = regcompclosure(se, tagi, p, pat, &lp, sp, &mp, &n, &mask);
				if (ret == REG_OK)
				{
					if((n != mask || mask == REG_INF) && (se[tagi].cflags & (REG_FIXEDLEN)))
					{
						ret = REG_FIXEDLEN;
						break;
					}
					if(c1)
						se[tagi].fixlen += n - 1;
					while (*++p != '}') {} /* skip past closure */
				}
			}
			else
			{
				store(REG_CHR);
				store(*p);
				++se[tagi].fixlen;
			}
			break;
		/*
			'\'
			Escape character or:
			BASIC - \[X] is a backref is X is 1-9 and an existing capture
			ENHANCED - Character class shorthand, \b is a word boundary,
				\B is not/anti-word-boundary, \Q-\E is a literal quote
		*/
		case '\\':
			++p;
			c2 = 1;
			if (se[tagi].cflags & REG_ENHANCED)
			{ /* REG_ENHANCED-specific escapes */
				switch (*p)
				{
				case 'a':
					store(REG_CHR);
					store('\a');
					++se[tagi].fixlen;
					break;
				case 'e':
					store(REG_CHR);
					store(27); /* \e is non-standard C */
					++se[tagi].fixlen;
					break;
				case 'n':
					store(REG_CHR);
					store('\n');
					++se[tagi].fixlen;
					break;
				case 'f':
					store(REG_CHR);
					store('\f');
					++se[tagi].fixlen;
					break;
				case 'r':
					store(REG_CHR);
					store('\r');
					++se[tagi].fixlen;
					break;
				case 't':
					store(REG_CHR);
					store('\t');
					++se[tagi].fixlen;
					break;
				case 'v':
					store(REG_CHR);
					store('\v');
					++se[tagi].fixlen;
					break;
				case 'b':
					store(REG_BND);
					store(1);
					break;
				case 'B':
					store(REG_BND);
					store(0);
					break;
				case 'c': /* control code */
					++p;
					n = *p;
					if ((n >= 'a' && n <= 'z')
						|| (n >= 'A' && n <= '_')
						|| (n == 127))
					{
						if (n != 127) /* delete control code */
							n %= 32;
						store(REG_CHR);
					}
					else
					{
						store(se[tagi].regchrtype);
						n = 'c';
					}
					store(n);
					++se[tagi].fixlen;
					break;
				case 'd': /* decimal: [0-9] */
					sp = _T("[0-9]");
					storeset(sp);
					++se[tagi].fixlen;
					break;
				case 'D': /* non-decimal: [^0-9] */
					sp = _T("[^0-9]");
					storeset(sp);
					++se[tagi].fixlen;
					break;
				case 's': /* white-space: [ \f\n\r\t\v] */
					sp = _T("[ \f\n\r\t\v]");
					storeset(sp);
					++se[tagi].fixlen;
					break;
				case 'S': /* non-white-space: [^ \f\n\r\t\v] */
					sp = _T("[^ \f\n\r\t\v]");
					storeset(sp);
					++se[tagi].fixlen;
					break;
				case 'w': /* word character: [a-zA-Z_0-9] */
					sp = _T("[a-zA-Z_0-9]");
					storeset(sp);
					++se[tagi].fixlen;
					break;
				case 'W': /* non-word character: [^a-zA-Z_0-9] */
					sp = _T("[^a-zA-Z_0-9]");
					storeset(sp);
					++se[tagi].fixlen;
					break;
				case 'x': /* \xXX XX = hex character value */
					mask = 0;
					for (n = 0; n < 2; ++n)
					{
						if (*++p >= '0' && *p <= '9')
							mask += (*p - '0') << (4 * (1-n));
						else if (*p >= 'A' && *p <= 'F')
							mask += ((*p - 'A') + 10) << (4 * (1-n));
						else if (*p >= 'a' && *p <= 'f')
							mask += ((*p - 'a') + 10) << (4 * (1-n));
						else
						{
							ret = REG_EESCAPE;
							break;
						}
					}
					store(se[tagi].regchrtype);
					store(mask);
					++se[tagi].fixlen;
					break;
				case 'u': /* \uXXXX XXXX = 4-digit unicode hex char value */
					store(se[tagi].regchrtype == REG_ICR ? REG_8IC : REG_8CC);
					if (se[tagi].cflags & REG_ENHANCED)
					{
						c2 = 0;
						for (n = 0; n < 4; ++n)
						{
							if (*++p >= '0' && *p <= '9')
								c1 = *p - '0';
							else if (*p >= 'A' && *p <= 'F')
								c1 = *p - 'A';
							else if (*p >= 'a' && *p <= 'f')
								c1 = *p - 'a';
							else
							{
								ret = REG_EESCAPE;
								break;
							}
							for (mask = 3 - n; mask > 0; --mask)
								c1 *= 16;
							c2 += c1;
						}
						UCSCharToUTF8(c2, mp);
						mp += UTF8CharLen(mp);
						se[tagi].fixlen += (uregchar)UTF8CharLen(mp);
					}
					else
					{
						store(*p);
						++se[tagi].fixlen;
					}
					break;
				case 'K':
					store(REG_KPO);
					break;
				case 'Q':
					while (*++p != '\\' || p[1] != 'E')
					{
						if (!*p)
							return REG_EESCAPE;
						store(se[tagi].regchrtype);
						store(*p);
						++se[tagi].fixlen;
					}
					++p; /* up to E, loop will iterate over that */
					break;
				case '<':
					store(REG_BOW);
					break;
				case '>':
					store(REG_EOW);
					break;
				case 'A':
					store(REG_BOL);
					store(0);
					break;
				case 'Z':
					store(REG_EOL);
					store(0);
					break;
				default:
					c2 = 0;
					break;
				} /* end ENHANCED switch */
			}
			if (!c2 || !(se[tagi].cflags & REG_ENHANCED))
			{
				/*	'{' BASIC slash-prepended veriation */
				if (*p == '{' && !(se[tagi].cflags & REG_EXTENDED))
				{
					c1 = *sp != REG_EOT;
					ret = regcompclosure(se, tagi, p, pat, &lp, sp, &mp, &n, &mask);
					if (ret == REG_OK)
					{
						if((n != mask || mask == REG_INF) && (se[tagi].cflags & (REG_FIXEDLEN)))
						{
							ret = REG_FIXEDLEN;
							break;
						}
						if(c1)
							se[tagi].fixlen += n - 1;
						p += 2;
						while (*++p != '}') {} /* skip past closure */
					}
					break;
				}
				/* \0 = NULL char, \OOO or \0OOO = 1-3 octal (<=377) */
				/* \o{OOO} 1-3 octal values, malformed Os == '\0' */
				/* \1-\255 = backreference */
				if (*p >= '1' && *p <= '9')
				{
					c1 = 0; c2 = 0; /* c1 = integer value, c2 = isoctal */
					if (se[tagi].cflags & REG_ENHANCED)
					{
						for (n = 0; n < 3; ++n)
							if (p[(int)n] < '0' || p[(int)n] > '9')
								break;
					}
					else
						n = 1; /* POSIX == 1 digit only */
					for (mask = n; n > 0; --n)
					{
						if (n == 3)
						{
							if (p[n-1] <= '3')
								++c2;
							c1 += (p[n-1] - '0') * 100;
						}
						else if (n == 2)
						{
							if (p[n-1] <= '7')
								++c2;
							c1 += (p[n-1] - '0') * 10;
						}
						else
						{
							if (p[n-1] <= '7')
								++c2;
							c1 += (p[n-1] - '0');
						}
					}
					if (!(se[tagi].cflags & REG_ENHANCED)
						|| (tagi == c1 && tagi != tagc))
					{
						ret = REG_ESUBREG;/* Cyclical reference */
						break;
					}
					if (tagc >= c1) /* if not a real capture, treat as octal */
					{ /* it's a real capture, mark as a REF */
						store(REG_REF);
						store((regchar)c1);
						break;
					}
					else if (mask != (regchar)c2) /* not octal? */
					{
						ret = REG_ESUBREG;/* Undetermined reference */
						break;
					}
				}
				if (!(se[tagi].cflags & REG_ENHANCED)
					|| ((*p < '0' || *p > '3') && *p != 'o'))
				{ /* an ordinary char	 */
					if(UTF8CharIsLeading(*p) == 0)
					{
						store(se[tagi].regchrtype);
						store(*p);
						++se[tagi].fixlen;
					}
					else
					{
						store(se[tagi].regchrtype == REG_ICR ? REG_8IC : REG_8CC);
						*mp++ = *p;
						++se[tagi].fixlen;
						while (UTF8CharIsTrailing(*(p + 1)))
						{
							*mp++ = *++p;
							++se[tagi].fixlen;
						}
					}
					break;
				}
				sp = (regchar*)p; /* we know it is an octal from this point */
				if (*p == 'o' || *p == '0')
				{
					++sp;
					if (*p == 'o' && p[1] != '{')
					{
						ret = REG_EESCAPE;
						break;
					}
				}
				for (n = 0; n < 3; ++n)
				{
					if (sp[(int)n] < '0' || sp[(int)n] > '7')
						break;
				}
				if (*p == 'o' && p[(int)n] != '}')
				{
					ret = REG_EESCAPE;
					break;
				}
				mask = 0;
				for (; n > 0; --n)
				{
					if (n == 3)
						mask += (*sp++ - '0') * 64;
					else if (n == 2)
						mask += (*sp++ - '0') * 8;
					else
						mask += (*sp++ - '0');
				}
				store(se[tagi].regchrtype);
				store(mask);
				++se[tagi].fixlen;
				break;
			}
			break; /* don't fallthrough and print extra char */
		/*
			'#'
			ENHANCED+REG_FREESPACE
			Single-line comment
		*/
		case '#':
			if ((se[tagi].cflags & (REG_ENHANCED|REG_FREESPACE)) == (REG_ENHANCED|REG_FREESPACE))
			{
				while (*++p && *p != '\n' && *p != '\r') {}
				--p; /* back up as loop will iterate one */
				break;
			}
			REGFALLTHROUGH /* fallthrough */
		case ' ':
		case '\r':
		case '\n':
			if ((se[tagi].cflags & (REG_ENHANCED|REG_FREESPACE)) == (REG_ENHANCED|REG_FREESPACE))
				break; /* ignored in freespace mode */
			REGFALLTHROUGH /* fallthrough */
		default:
			/* an ordinary char	 */
			if(UTF8CharIsLeading(*p) == 0)
			{
				store(se[tagi].regchrtype);
				store(*p);
				++se[tagi].fixlen;
			}
			else
			{
				store(se[tagi].regchrtype == REG_ICR ? REG_8IC : REG_8CC);
				*mp++ = *p;
				++se[tagi].fixlen;
				while (UTF8CharIsTrailing(*(p + 1)))
				{
					*mp++ = *++p;
					++se[tagi].fixlen;
				}
			}
			break;
		} /* end switch for current pattern character */
		if (ret != REG_OK) /* error? */
		{
			free(se);
			free(r->nfa);
			r->nfa = NULL;
			r->pat = pat;
			r->patPos = p - pat;
			return ret;
		}
		WSASSERT((size_t)(mp - r->nfa) <= r->nfaSize);
	}/* end while searching through pattern */

	/* Not enough )'s */
	if (tagi > 0)
		return REG_EPAREN;

	/* Store ending state and set up re_nsub */
	store(REG_EOT);
	*((unsigned short*)se[tagi].altloc) =
		(unsigned short)(mp - se[tagi].altloc);
	*se[tagi].fixlenloc = se[tagi].fixlen;
	if (!(cflags & REG_NOSUB))
	{
		store(1);
		r->re_nsub = tagc - 1;
	}
	else
		store(0); /* REG_NOSUB == not even main match is recorded */

	/* shrink the buffer down to the real size of the NFA */
	mp = REG_CPP_CAST(regchar*)
		realloc(r->nfa, r->nfaSize = (size_t)(mp - r->nfa));
	if(mp) /* if realloc fails that's fine, we were just trying to shrink */
		r->nfa = mp;

	/* Clean up se and return success */
	free(se);
	if(!r->nfa) /* might happen due to a race condition after realloc frees */
		return REG_ESPACE;
	return REG_OK;
}

/*===========================================================================*/
/*							regexec()										 */
/*===========================================================================*/

/* structure used for each depth count when doing closures */
typedef struct regexprm_t
{
	const regchar* lp;
	const regchar* ap;
	regmatch_t* matches;
} regexprm_t;

typedef struct regexecfallback_t
{
	struct regexecfallback_t* prev;
	struct regexecfallback_t* next;
} regexecfallback_t;
typedef struct regexecbot_t
{
	struct regexecbot_t* prev;
	struct regexecbot_t* next;
	const regex_t* r;
	const regchar** lp;
	const regchar** ap;
	const regchar* bol;
	const regchar* eol;
	int eflags;
	size_t numMatches;
	regmatch_t* matches;
	size_t depth;
	const regchar** lpstart;
	regchar direction;
	regchar desres;
	const regchar* bol2;			/* lp to reset to after each regexecrec() */
	const regchar* lplongest;		/* longest lp match found so far */
	regoff_t kpolongest;			/* /K keepout from the longest match */
	regoff_t lastkpolongest;		/* so \K doesn't apply on failed matches */
	size_t imatch, imatchend;		/* iterators for invalidating failed matches */
	int execRes;					/* result from regexecrec() */
	uregchar fixlen;				/* fixed length amount */
	const regchar* altloc;			/* location of ALT we are working on */
	const regchar* sucaltloc;		/* ALT of last successful match */
} regexecbot_t;

/* forward declare for regexecrec() */
int regexecbot(register const regex_t* r, const regchar** lp,
			   const regchar** ap, const regchar* bol, const regchar* eol,
			   int eflags, size_t numMatches, regmatch_t matches[],
			   size_t depth, register const regchar** lpstart,
			   regchar direction, regchar desres);

/**
* [Private] Performs the execution of each NFA state. Currently execution
* goes as regexec()->regexecrec()<->regexecbot().
*
* ##Time for the BIG explanation##
*
* 1) regexec() is called initially, lp represents the current position in
* the string we are matching against. regexec() than calls regexecrec() for
* each character of lp, or the search string.
*
* 2) regexecrec() then goes through the NFA one state at a time until a
* closure (multi-match like "c*" which matches 0 or more "c"s) or a
* subexpression (or REG_BOT/"("). When it reaches a subexpression it
* calls regexecbot()
*
* 3) regexecbot() sets up a few variables than calls regexecrec() again
*
* 4) regexecrec() returns on full success - REG_TRUE, failure - REG_FALSE,
* if the end of the subexpression has been reached - REG_EOT, or if an
* alternation has been reached - REG_ALT.
*
* 5) Closure matching in regexecrec() can get really involved because,
* attempting be POSIX compatible, it takes the "leftmost longmost" match,
* i.e.:
*
*	pattern "(kkk|kk+)kiii"
*
*	must match "kkkiii"
*
* Normally if you just did left to right matching it would not match because
* the first three k's in the first subexpression, then exit that
* subexpression. So you have to keep track of each stack of match locations
* for each match of each alternation in each subexpression.
*
* 6) Closures have to match the entire match, and call regexecrec() with
* the private REG_CAPTUREALL flag which causes REG_EOT to return until it
* reaches the end of the entire NFA, or depth 0.
*
* 7) depth is the number of subexpressions deep regexecrec() is, i.e.
*
* aa(bbb(c*c|c)bb)aaa
*
* in the above, the a's at are depth 1, the b's at 2, c's 3. Normally
* REG_EOT returns right away when the end of the subexpression is reached.
* However, being as closure have to match the entire match, for example
* the first c in depth 3 in the above will match the ENTIRE NFA before it
* continues the loop in regexecrec() to see how many c's would net the
* longest match, the loop just keeps matching past REG_EOT until the end
* is reached.
*
* 8) The entire NFA is surrounded by a REG_BOT and REG_EOT that signifies
* where the beginning and end of the NFA is.

* 9) Another involved thing is how the matches array is filled - whenever
* a REG_EOT ( ")" ) is reached, or end of the current subexpression. With
* alternations (ALT) involved it gets involved fast
*
* @param r regex structure
*
* @param lp current position in string to match against (search string)
*
* @param ap current position in NFA
*
* @param bol beginning of string to match against
*
* @param eol end of string to match against
*
* @param eflags execute specific flags
*
* @param numMatches size of passed in matches array (1 == index 0)
*
* @param matches match array to fill, of numMatches size
*
* @param depth Number of (/BOL passed
*
* @param lpstart array[depth] containing lp for each time entering
* into regexecbot for keeping track of which lp is the longest match to keep
*
* @return REG_OK on success, REG_FALSE on failure, REG_ALT if alternation
* reached, REG_EOT if end of entire expression is reached
*/
int regexecrec(register const regex_t* r, const regchar** lp,
			   const regchar** ap, const regchar* bol, const regchar* eol,
			   int eflags, size_t numMatches, regmatch_t matches[],
			   size_t depth, register const regchar** lpstart)
{
	int unexpectedState = 0; /* break and return on unexpected NFA state */

	for (;;)
	{
		WSASSERT(*ap >= r->nfa && *ap < (r->nfa + r->nfaSize));
		switch(*(*ap)++)
		{
		case REG_CHR:
			if (*lp == eol || *(*lp)++ != *(*ap)++)
				return REG_FALSE;
			break;
		case REG_ICR:
			if (*lp == eol || regtolower(*(*lp)) != regtolower(*(*ap)))
				return REG_FALSE;
			(*ap)++; (*lp)++;
			break;
		case REG_8CC:
		case REG_8IC:
			{
				size_t utf8Len = UTF8CharLen((*lp));
				REGEXDEBUG("REG_8IC/REG_8CC\n");
				if ((*lp) == eol || utf8Len != UTF8CharLen((*ap)))
					return REG_FALSE;
				if(*((*ap) - 1) == REG_8IC)
				{
					if(UTF8CharToLower((*lp)) != UTF8CharToLower((*ap)))
						return REG_FALSE;
				}
				else if (UTF8CharToUCS((*lp)) != UTF8CharToUCS((*ap)))
					return REG_FALSE;
				(*lp) += utf8Len;
				(*ap) += utf8Len;
				break;

			}
		case REG_ANY:
			REGEXDEBUG("REG_ANY\n");
			{
				int fail = ((*lp) == eol &&
					(!*(*ap) || (*(*lp) != '\n' && *(*lp) != '\r')));
				(*lp) += UTF8CharLen((*lp));
				++(*ap);
				if (fail)
					return REG_FALSE;
				break;
			}
		case REG_NCL:
		case REG_CCL:
			REGEXDEBUG("REG_CCL/NCL\n");
			{
				int fail;
				if (!UTF8CharIsAnsii(*(*lp)))
					fail = (*lp) == eol || *((*ap) - 1) == REG_CCL;
				else
					fail = (*lp) == eol || !regisinset((*ap),*(*lp));
				(*ap) += REG_BITBLK;
				(*lp) += UTF8CharLen((*lp));
				if (fail)
					return REG_FALSE;
				break;
			}
		case REG_BOL:
			REGEXDEBUG("REG_BOL\n");
			if (*(*ap))
			{
				if (!((*lp) == bol || (*((*lp)-1) != '\n' && *((*lp)-1) != '\r')))
					return REG_FALSE;
			}
			else if ((*lp) != bol || (eflags & REG_NOTBOL))
				return REG_FALSE;
			++(*ap);
			break;
		case REG_EOL:
			REGEXDEBUG("REG_EOL\n");
			if (*(*ap))
			{
				if (!((*lp) == eol || (*((*lp)+1) != '\n' && *((*lp)+1) != '\r')))
					return REG_FALSE;
			}
			else if ((*lp) != eol || (eflags & REG_NOTBOL))
				return REG_FALSE;
			++(*ap);
			break;
		case REG_BOT:
		case REG_NLB:
		case REG_PLB:
		case REG_NLA:
		case REG_PLA:
		case REG_CON:
			REGEXDEBUG("REG_BOT\n");
			{
				int res; int desiredres;
				regchar type = *((*ap) - 1), iscon = type == REG_CON;
				const regchar* tempbol; const regchar* tempeol; const regchar* templp;
				const regchar* oldap = *ap; const regchar* conap;
				regchar direction;
				if(iscon)
				{
					conap = *ap;
					(*ap) += REG_ALTBOTSKIP;
					type = *((*ap) - 1);
					oldap = *ap;
				}
				#ifdef _DEBUG
				else
					conap = NULL; /* for MSVC checking */
				#endif

				if(type == REG_NLB || type == REG_PLB)
				{
					desiredres = type == REG_PLB ? REG_TRUE : REG_FALSE;
					templp = *lp; /* will be adjusted by fixlen by regexecbot() */
					tempeol = *lp;
					tempbol = bol;
					direction = REG_BACKWARD;
				}
				else if(type == REG_NLA || type == REG_PLA)
				{ /* lookaheads had the comment
				  "*lp + 1 as we must match/not match first pos"
				  I.E. (*lp) + 1 for tempeol, however if fixlen is greater
				  than 1 it always messes up*/
					desiredres = type == REG_PLA ? REG_TRUE : REG_FALSE;
					tempbol = bol;
					tempeol = eol;
					templp = *lp;
					direction = REG_FORWARD;
				}
				else
				{
					desiredres = REG_TRUE;
					tempbol = bol;
					tempeol = eol;
					templp = *lp;
					direction = REG_FORWARD;
				}

				do
				{
					(*ap) = oldap; /* for the lookarounds, won't affect normals */

					res = regexecbot(r, &templp, ap, tempbol, tempeol, eflags,
							numMatches, matches, depth, lpstart, direction,
							(regchar)desiredres);

					if(type == REG_NLA || type == REG_PLA)
					{
						/* break without touching the main bol/eol/lp
						since its a lookaround */
						break;
					}
					else if (type == REG_BOT)
					{
						bol = tempbol;
						eol = tempeol;
						*lp = templp;
						break;
					}

					if(type == REG_NLB || type == REG_PLB)
					{
#if 0 /* non-fixed length lookbehinds, killer of the fastest supercomputers */
						/* test if we need to try to match again
						with one character further up in the search
						string. In a lookbehind the end of the match
						MUST meet up to the point where we are */
						if(templp != *lp && tempbol != *lp)
						{
							++tempbol;
							templp = tempbol;
							res = !desiredres;
							continue;
						}
#endif
						break;
					}
				} while (res != REG_TRUE); /* basically while(1) */
				if(iscon)
				{
					if(res != REG_TRUE)
						(*ap) = conap + *((unsigned short*)conap) - 1 + REG_ALTBOTSKIP;
					else
						++(*ap); /* move over the EOT capture # */
					res = regexecrec(r, lp, ap, bol, eol, eflags,
							   numMatches, matches, depth, lpstart);
					/* Skip over ending REG_EOT so the pattern continues matching */
					if(res == REG_ALT)
					{
						(*ap) = (*ap) + *((unsigned short*)((*ap) + 1)) + 1;
						res = REG_TRUE;
					}
				}
				if(res != REG_TRUE)
				{
					return res;
				}
				++(*ap); /* regexecbot() puts it one before the next state */
			}
			break;
		case REG_EOT:
			REGEXDEBUG("REG_EOT\n");
			if (*(*ap) && numMatches >= (size_t)*(*ap))
			{ /* 1st: if capture # is 0 "(?:",  3rd if \K set it won't be -1 */
				if ((*(*ap)) - 1 != 0 || matches[(*(*ap)) - 1].rm_so == (regoff_t)-1)
					matches[(*(*ap)) - 1].rm_so = (regoff_t)(lpstart[depth - 1] - bol);
				matches[(*(*ap)) - 1].rm_eo = (regoff_t)((*lp) - bol);
				/*WSASSERT(matches[(*(*ap)) - 1].rm_eo >= matches[(*(*ap)) - 1].rm_so);*/
			}
			if (!(eflags & REG_CAPTUREALL))
				return REG_TRUE;
			else if (!--depth)
				return REG_EOT;
			++*ap; /* skip past EOT cap # as we are continuing */
			break;
		case REG_HUM: /* closure */
		case REG_GRE:
		case REG_POS:
			REGEXDEBUG("REG_CLOSURE\n");
			{
				/** to save the line ptr. */
				regchar* oldap; regchar* oldlp;
				const regchar* are = (*lp);
				int n;
				regexprm_t* clomatches = NULL;
				size_t numCloMatches = 0, iClo, iCloEnd, i;
				size_t min = *(*ap), max = *((*ap) + 1);
				int isPossessive = *((*ap) - 1) == REG_POS;
				int isLazy = *((*ap) - 1) == REG_HUM;
				if (max == REG_INF)
					max = (size_t)-1;
				switch(*((*ap) + REG_CLODATA))
				{
				case REG_CHR:
					{
						while ((*lp) != eol
								&& *((*ap) + (REG_CLODATA + 1)) == *(*lp)
								&& numCloMatches < max)
						{
							++numCloMatches;
							(*lp) += 1;
						}
						n = REG_CHRSKIP;
						break;
					}
				case REG_ICR:
					{
						char lowerchar = regtolower(*((*ap) + (REG_CLODATA + 1)));
						while ((*lp) != eol
								&& numCloMatches < max
								&& lowerchar == regtolower(*(*lp)))
						{
							++numCloMatches;
							(*lp) += 1;
						}
						n = REG_CHRSKIP;
						break;
					}
				case REG_8CC:
					{
						uint32_t ucschar = UTF8CharToUCS(((*ap) + (REG_CLODATA + 1)));
						while ((*lp) != eol
								&& numCloMatches < max
								&& ucschar == UTF8CharToUCS((*lp)))
						{
							++numCloMatches;
							(*lp) += UTF8CharLen((*lp));
						}
						n = REG_CHRSKIP;
						break;
					}
				case REG_8IC:
					{
						uint32_t lowerchar = UTF8CharToLower(((*ap) + (REG_CLODATA + 1)));
						while ((*lp) != eol
								&& numCloMatches < max
								&& lowerchar == UTF8CharToLower((*lp)))
						{
							++numCloMatches;
							(*lp) += UTF8CharLen((*lp));
						}
						n = REG_CHRSKIP;
						break;
					}
				case REG_NCL:
				case REG_CCL:
					{
						while ((*lp) != eol
								&& ((!UTF8CharIsAnsii(*(*lp)) && (*((*ap) + REG_CLODATA) == REG_NCL))
									|| (UTF8CharIsAnsii(*(*lp)) &&
										regisinset((*ap) + (REG_CLODATA + 1), *(*lp))))
								&& numCloMatches < max)
						{
							++numCloMatches;
							(*lp) += UTF8CharLen((*lp));
						}
						n = REG_CCLSKIP;
						break;
					}
				case REG_ANY:
					{
						#define REGMIN(x, y) (x < y ? x : y)
						size_t charsToMatch;
						charsToMatch = REGMIN((size_t)(eol - (*lp)), max - numCloMatches);
						for (i = 0; i < charsToMatch; ++i)
							(*lp) += UTF8CharLen((*lp));
						numCloMatches += charsToMatch;
						n = REG_ANYSKIP;
						break;
					}
				case REG_BOT:
					{
						const regchar* api = *ap;
						const regchar* lpi = *lp;
						regexprm_t* newclomatches;
						size_t iterations = 0;
						int execBotRes;
						n = 0;
						while (iterations++ < max)
						{ /* ap is 1 over CLODATA as regexecbot expects 1 over REG_BOT */
							/*static its = 0;
							++its;
							printf("It #%d\n", its);
							if(its == 8)
							{
								int breakhere = 5;
							}*/
							*ap = api + REG_CLODATA + 1;
							WSASSERT(*((*ap)-1) == REG_BOT);
							execBotRes = regexecbot(r, lp, ap, bol, eol, eflags,
													numMatches, matches, depth, lpstart,
													REG_FORWARD, REG_TRUE);
							if (execBotRes == REG_FALSE
								|| execBotRes == REG_ESPACE)
								break;
							if (execBotRes == REG_EOT)
							{
								if (!(eflags & REG_NOMATCH))
									for (i = 0; i < numCloMatches; ++i)
										free(clomatches[i].matches);
								free(clomatches);
								return REG_EOT;
							}
							++numCloMatches;
							newclomatches =
								(regexprm_t*)realloc(clomatches, numCloMatches * sizeof(regexprm_t));
							if (!newclomatches)
							{
								for (i = 0; i < numCloMatches - 1; ++i)
									free(clomatches[i].matches);
								free(clomatches);
								return REG_ESPACE;
							}
							clomatches = newclomatches;
							clomatches[numCloMatches - 1].lp = lpi;
							clomatches[numCloMatches - 1].ap = api;
							if (!(eflags & REG_NOMATCH))
							{
								clomatches[numCloMatches - 1].matches =
									(regmatch_t*)malloc(numMatches * sizeof(regmatch_t));
								if (!clomatches[numCloMatches - 1].matches)
								{
									for (i = 0; i < numCloMatches - 1; ++i)
										free(clomatches[i].matches);
									free(clomatches);
									return REG_ESPACE;
								}
								regmemcpy(clomatches[numCloMatches - 1].matches, matches,
											numMatches * sizeof(regmatch_t));
							}
							lpi = *lp;
						}
						/* -REG_CLODATA as it is added later
						* +1 to skip the byte after REG_EOT */
						*ap += -REG_CLODATA + 1;
						*lp = lpi;
						break;
					}
				default:
					WSASSERT(0);
					return REG_FALSE;
				}

				(*ap) += n + REG_CLODATA;
				oldap = (regchar*)(*ap);
				if (numCloMatches < min) /* min not met? */
					return REG_FALSE;
				if (isLazy) /* Lazy tries from least to most, else opposite */
				{
					iClo = min + 1; iCloEnd = numCloMatches + 1;
					*lp = are + min;
					oldlp = (regchar*)(*lp);
				}
				else
				{
					iClo = numCloMatches; iCloEnd = 0;
					oldlp = (regchar*)(*lp);
				}
				for (; iClo != iCloEnd; iClo += isLazy ? 1 : -1)
				{ /* try the rest of the match past the closure */
					n = regexecrec(r, lp, ap, bol, eol, eflags | REG_CAPTUREALL,
									numMatches, matches, depth, lpstart);
					if ((!isLazy && (n || isPossessive || iClo < min))
						|| (isLazy && n && (iClo-1) >= min))
					{
						if (clomatches)
						{
							if (!(eflags & REG_NOMATCH))
								for (i = 0; i < numCloMatches; ++i)
									free(clomatches[i].matches);
							free(clomatches);
						}
						if (n == REG_FALSE || iClo < min)
							return REG_FALSE;
						return n;
					}
					if (clomatches)
					{
						(*lp) = clomatches[iClo - 1].lp;
						(*ap) = clomatches[iClo - 1].ap;
					}
					else
					{
						(*ap) = oldap;
						if (isLazy)
							oldlp += UTF8CharLen(oldlp);
						else
						{
							--oldlp;
							if (!UTF8CharIsAnsii(*oldlp))
								while (UTF8CharIsTrailing(*oldlp))
									--oldlp;
						}
						(*lp) = oldlp;
					}
					if (!(eflags & REG_NOSUB) && numMatches > 0) /* clear rm_so if set by \K */
						matches[0].rm_so = (regoff_t)-1;
				}
				if (clomatches)
				{
					if (!(eflags & REG_NOMATCH))
						for (i = 0; i < numCloMatches; ++i)
							free(clomatches[i].matches);
					free(clomatches);
					clomatches = NULL;
				}
				if((!isLazy && (iClo < min))
						|| (isLazy && (iClo-1) >= min))
				/* Failed to complete rest of match */
					return REG_FALSE;
				break;
			}
		case REG_AND:
			if(*(ap))
			{ /* fixed length, if it goes before bol fail */
				if((*lp) - *(*ap) < bol)
					return REG_FALSE;
				(*lp) -= *(*ap);
			}
			else
				(*lp) = bol;
			++(*ap);
			break;
		case REG_ALT:
			REGEXDEBUG("REG_ALT\n");
			--(*ap); /* move back to ALT */
			return REG_ALT;
		case REG_BND: /* \b == (^\w|\w$|\W\w|\w\W) */
			if (((((*lp)!=bol || !regiswordc((*lp)[-1])))
				== (!regiswordc((*lp)[0])))
					!= *(*ap)++)
				return REG_FALSE;
			++(*lp); /* skip over the word boundry char */
			break;
		case REG_BOW:
			if (((*lp) != bol && regiswordc((*lp)[-1])) || !regiswordc(*(*lp)))
				return REG_FALSE;
			break;
		case REG_EOW:
			if ((*lp) == bol || !regiswordc((*lp)[-1]) || regiswordc(*(*lp)))
				return REG_FALSE;
			break;
		case REG_REF:
			REGEXDEBUG("REG_REF\n");
			{
				if (numMatches > (size_t)*(*ap))
				{
					/* beginning of subpat.. */
					register regchar * bp =
						(regchar*)(matches[(int)(*(*ap))].rm_so + bol);
					/* ending of subpat..	 */
					register regchar * ep =
						(regchar*)(matches[(int)(*(*ap)++)].rm_eo + bol);
					while (bp < ep) /* compare the backreference */
						if (*bp++ != *(*lp)++) /* direct compare, no flags */
							return REG_FALSE; /* doesn't match */
				}
				else
					return REG_FALSE;
				break;
			}
		case REG_KPO:
			/* drop everything matched so far from primary match 0,
			does NOT apply to submatches. Note that /K in a non-matching
			ALT isn't applied: i.e. "r(da(t)\K^t|datz)s" matches "rdatzs"
			in "srdatzs". */
			if (!(eflags & REG_NOSUB) && numMatches > 0)
				matches[0].rm_so = (regoff_t)((*lp) - bol);
			break;
		default:
			WSASSERT(0);
			unexpectedState = 1;
			break;
		}
		if (unexpectedState) /* reached default case - bad NFA state */
			break;
	}
	return REG_FALSE;
}

/**
* [Private] Called each time a REG_BOT or lookaround is reached
*
* @see regexecrec()
*
* @param r regex structure
*
* @param lp current position in string to match against
*
* @param ap current position in NFA
*
* @param bol beginning of string to match against
*
* @param eol end of string to match against
*
* @param eflags execute specific flags
*
* @param numMatches size of passed in matches array (1 == index 0)
*
* @param matches match array to fill, of numMatches size
*
* @param depth Number of (/BOL passed
*
* @param lpstart array[depth] containing lp for each time entering into
* regexecbot for keeping track of which lp is the longest match to keep
*
* @param direction matching direction, 1 for forward, 0 for backward
*
* @param desres disired result, REG_TRUE success REG_FALSE failure.
* Negative lookarounds will be REG_FALSE for example
*
* @return REG_TRUE, REG_FALSE, REG_EOT if end of the entire base level
* is reached, REG_ALT if alternation met
*/
int regexecbot(register const regex_t* r, const regchar** lp,
			   const regchar** ap, const regchar* bol, const regchar* eol,
			   int eflags, size_t numMatches, regmatch_t matches[],
			   size_t depth, register const regchar** lpstart,
			   regchar direction, regchar desres)
{
	const regchar* bol2 = (*lp);			/* lp to reset to after
												each regexecrec() */
	const regchar* lplongest = NULL;		/* longest lp match found so far */
	regoff_t kpolongest = (regoff_t) -1,	/* /K keepout from
												the longest match */
		 lastkpolongest = kpolongest;		/* so \K doesn't apply on
												failed matches */
	size_t imatch, imatchend;				/* iterators for invalidating
												failed matches */
	int execRes;							/* result from regexecrec() */
	uregchar fixlen;						/* fixed length amount */
	const regchar* altloc = (*ap);			/* location of ALT
												we are working on */
	const regchar* sucaltloc = NULL;		/* ALT of last successful match */

	WSASSERT( regisbottype(*((*ap) - 1)) );
	(*ap) += REG_ALTBOTDATA; /* move past REG_BOT params */
	lpstart[depth] = *lp;
	for (;;)
	{
		WSASSERT(*ap >= r->nfa && *ap < (r->nfa + r->nfaSize));

		/* adjust lp based off direction and fixed length */
		fixlen = (uregchar)*(*ap - 1);
		execRes = REG_TRUE;
		if(direction == REG_FORWARD)
		{
			if(bol2 + fixlen > eol)
				execRes = REG_FALSE;
		}
		else
		{
			*lp = bol2 - fixlen;
			if(*lp < bol)
				execRes = REG_FALSE;
		}

		/* do the match */
		if(execRes)
			execRes = regexecrec(r, lp, ap, bol, eol, eflags & ~REG_CAPTUREALL,
								numMatches, matches, depth + 1, lpstart);
		if (execRes && *lp > lplongest)
		{ /* got a new match longer than previous ones */
			lplongest = *lp;
			if (!(eflags & REG_NOSUB) && numMatches > 0
				&& lastkpolongest != kpolongest) /* handle \K */
				lastkpolongest = kpolongest = matches[0].rm_so;
			else
				kpolongest = (regoff_t) -1;
		}

		/* Invalidate failed captures. Only time we don't if on the first
		part of the expression and it was a successful match */
		/* +2 is to skip past first REG_ALT param to 2nd */
		if (execRes && lplongest == *lp)
		{ /* first invalidate subexpression matches from the last
		  successful ALT location if we found a new successful match */
			if(desres != REG_TRUE) /* got match but didn't want one */
				return REG_FALSE;
			if(sucaltloc)
			{
				imatch = *(sucaltloc + REG_ALTBOTNEXTALTDATA);
				imatchend = *(altloc + REG_ALTBOTNEXTALTDATA);
				for (; imatch < imatchend; ++imatch)
					matches[imatch].rm_so =
						matches[imatch].rm_eo = (regoff_t) -1;
			}
			sucaltloc = altloc;
		}
		else
		{ /* match failure, clear subexpression matches from this
			ALT forward */
			imatch = *(altloc + REG_ALTBOTNEXTALTDATA);
			imatchend = numMatches;
			for (; imatch < imatchend; ++imatch)
				matches[imatch].rm_so =
					matches[imatch].rm_eo = (regoff_t) -1;
		}

		altloc = (*ap) = altloc + *((unsigned short*)altloc);
		WSASSERT(*((*ap)-1) == REG_ALT || *((*ap)-1) == REG_EOT);
		if (execRes == REG_EOT || *((*ap) - 1) == REG_EOT)
			break;
		(*ap) += REG_ALTBOTDATA; /* skip over the 3 bytes of REG_ALT params */
		(*lp) = bol2; /* reset to pointer before regexecrec() */
	}
	if (execRes == REG_FALSE && lplongest)
	{ /* quick cleanup the case where the last ALT doesn't match */
		(*ap) -= 1;
		WSASSERT(*(*ap) == REG_EOT);
		regexecrec(r, &lplongest, ap, bol, eol, eflags & ~REG_CAPTUREALL,
				   numMatches, matches, depth + 1, lpstart);
	}
	if (!lplongest)
		return desres == REG_FALSE ? REG_TRUE : REG_FALSE;
	*lp = lplongest;
	if (!(eflags & REG_NOSUB) && numMatches > 0
		&& kpolongest != (regoff_t)-1)
		matches[0].rm_so = kpolongest;
	if(execRes == REG_EOT)
		return REG_EOT;
	return desres == REG_TRUE ? REG_TRUE : REG_FALSE;
}
/**
* Attempts to match a compiled regular expression against a search string
*
* @param r compiled regular expression from regcomp()
*
* @param lp search to match again (search string)
*
* @param numMatches number of indexes in the matches array
*
* @param matches array of regmatch_t representing the match positions,
* must be of numMatches size
*
* @param eflags flags controlling how the compiled expression matches
*
* @return NOMATCH if compiled expression doesn't match the search string.
*/
int regexec(register const regex_t* r,
			const regchar* lp, register size_t numMatches,
			register regmatch_t matches[], register int eflags)
{
	const regchar* ap = r->nfa;			/* position the NFA from regcomp() */
	register const regchar* bol = lp;	/* beginning of the search string */
	register const regchar* eol;		/* end of the search string */
	const regchar* lplongest = NULL;	/* leftmost longmost == POSIX */
	const regchar* lplongeststart;		/* start of the longest lpstart */
	const regchar** lpstart;			/* start of lp for the longest match */
	register size_t imatch, imatchend;	/* match indexes when invalidating captures */
	register int execRes = REG_FALSE;	/* result from regexecrec() */
	const regchar* altloc = ap + 1;		/* Current ALT (|) position in the NFA */
	const regchar* sucaltloc = NULL;	/* last successful ALT location */
	uregchar fixlen;					/* fixed length if applicable */
	int i;								/* generic loop counter */

	if (!r->nfa)
		return REG_NOTCOMP;
	if (eflags & REG_STARTEND)
	{
		bol += matches[0].rm_so;
		eol = bol + matches[0].rm_eo;
	}
	else
		eol = bol + regstrlen(bol);
	if (*(ap+1) == 0) /* main match recorded? If not nosub was passed */
		eflags |= REG_NOSUB;
	else
	{
		eflags &= ~REG_NOSUB;
		if (numMatches) /* set up keepout */
			matches[0].rm_so = (regoff_t)-1;
	}

	lpstart = REG_CPP_CAST(const regchar**) malloc(r->numBot * sizeof(regchar*));
	if (!lpstart)
		return REG_ESPACE;
	WSASSERT(*ap == REG_BOT);
	ap += 1; /* skip past the raw REG_BOT byte, loop will skip past params */
	for (i = 0; *lp; lp = bol + ++i)
	{
		lpstart[0] = lp;
		ap += REG_ALTBOTDATA; /* skip past REG_BOT/REG_ALT params */

		/* do the match, or fail if fixed length and would never match */
		fixlen = (uregchar)*(ap - 1);
		if(bol + fixlen > eol)
			execRes = REG_FALSE;
		else
			execRes = regexecrec(r, &lp, &ap, bol, eol,
								 eflags & ~REG_CAPTUREALL,
								 numMatches, matches, 1, lpstart);
		/* lp - lpstart[0] == 0 means an empty match, which doesn't count */
		if (execRes && lp > lplongest && (lp - lpstart[0]) != 0)
		{ /* new match longer than previous */
			lplongeststart = lpstart[0];
			WSASSERT(lp - lpstart[0] > 0);
			lplongest = lp;
		}

		/* Invalidate failed captures. Only time we don't if on the first
		part of the expression and it was a successful match */
		/* +2 is to skip past first REG_ALT param to 2nd */
		if (execRes && lplongest == lp)
		{ /* first invalidate subexpression matches from the last
		  successful ALT location if we found a new successful match */
			if(sucaltloc)
			{
				imatch = *(sucaltloc + REG_ALTBOTNEXTALTDATA);
				imatchend = *(altloc + REG_ALTBOTNEXTALTDATA);
				for (; imatch < imatchend; ++imatch)
					matches[imatch].rm_so =
						matches[imatch].rm_eo = (regoff_t) -1;
			}
			sucaltloc = altloc;
		}
		else
		{ /* match failure, clear subexpression matches from this
			ALT forward */
			imatch = *(altloc + REG_ALTBOTNEXTALTDATA);
			imatchend = numMatches;
			for (; imatch < imatchend; ++imatch)
				matches[imatch].rm_so =
					matches[imatch].rm_eo = (regoff_t) -1;
		}

		ap = altloc + *((unsigned short*)altloc);
		WSASSERT(*(ap-1) == REG_ALT || *(ap-1) == REG_EOT);
		if (execRes != REG_TRUE && execRes != REG_EOT && *(ap - 1) == REG_ALT)
		{
			altloc = ap;
			if (numMatches > 0)
				matches[0].rm_so = (regoff_t)-1; /* clear if set by \K */
			--i; /* reset position to same as entered loop */
		}
		else if (lplongest >= (bol + i + 1))
			break; /* longest possible found */
		else
		{ /* increment lp, our position in the search string
			and reset our position in the NFA to the beginning */
			altloc = ap = r->nfa + 1;
			WSASSERT(*(altloc-1) == REG_BOT && *(ap-1) == REG_BOT);
		}
	}

	free((void*)lpstart);

	if (!lplongest)
	{
		/* clear matches and return nomatch */
		for (imatch = 0; imatch < numMatches; ++imatch)
			matches[imatch].rm_so = matches[imatch].rm_eo = (regoff_t)-1;
		return REG_NOMATCH;
	}
	else if (execRes == REG_FALSE)
	{ /* quick cleanup to skip to the last REG_EOT so main match gets
	  recorded in the case where the last ALT doesn't match */
		while(*(ap - 1) != REG_EOT)
			altloc = ap = altloc + *((unsigned short*)(altloc));
		--ap;
		WSASSERT(*ap == REG_EOT);
		regexecrec(r, &lplongest, &ap, bol, eol, eflags & ~REG_CAPTUREALL,
					numMatches, matches, 1, &lplongeststart);
	}
	WSASSERT(!numMatches || (matches[0].rm_so != (regoff_t) -1
			&& matches[0].rm_eo != (regoff_t) -1));
	return REG_OK;
}

/**
* Releases resources allocated by a compiled regular expression after regcomp()
*
* @param r regex_t object to free
*/
void regfree(register regex_t* r)
{
	if (r->nfa != NULL && *r->nfa != '\0')
	{
		free(r->nfa);
		r->nfa = NULL;
	}
}

/**
* Substitutes based on match indexes. Typically you'd pass in the search
* string you passed to regexec() as src, an expression such as "$1"
* for expr which would replace the area where the entire match occured with
* that of the first expression.
*
* Ex:
*
*	regsubs(repl, &subj[isubj], (char*)subs.c_str(),
*
*	sublen, ((regex_t*)mpRE)->re_nsub + 1, ((regmatch_t*)mpMatch));
*
* @param src Typically the search string you passed to regexec(),
* string used as the source for the substition
*
* @param expr Expression to substitute with, \\# or $# with # being
* the expression number to substitute in
*
* @param dst buffer to receive the substitution result
*
* @param nm size of dst
*
* @param numMatches size of matches array
*
* @param matches match array from a successful regexec()
*
* @return size needed for the full buffer
*/
size_t regsubs(register const regchar *src, register const regchar* expr,
			   register regchar *dst, register size_t nm,
			   register size_t numMatches, register regmatch_t matches[])
{
	register regchar c, cr;
	register int pin;
	register regchar *bp;
	register regchar *ep;
	register regchar *edst = dst + nm;

	if (!*src)
		return 0;

	nm = 0;
	while (*src)
	{
		c = *src++;
		switch(c)
		{
		/* & conflicts with other regexs */
/*		case '&':
			pin = 0;
			break;
*/		case '\\':
		case '$':
			cr = *src;
			if (cr >= '0' && cr <= '9' && (cr - '0') < (regchar)numMatches)
			{
				++src;
				pin = cr - '0';
				break;
			}
		default:
			if (dst < edst)
				*dst++ = c;
			++nm;
			if (UTF8CharIsLeading(c))
				while (UTF8CharIsTrailing(*src))
				{
					if (dst < edst)
						*dst++ = *src;
					++src;
					++nm;
				}
			continue;
		}
		if (matches[pin].rm_so == (regoff_t)-1)
			return nm; /* unused matches filled with -1 per POSIX spec */
		bp = (regchar*)(matches[pin].rm_so + expr);
		ep = (regchar*)(matches[pin].rm_eo + expr);
		if (bp && ep)
		{
			while (*bp && bp < ep)
			{
				if (dst < edst)
					*dst++ = *bp;
				++nm;
				if (UTF8CharIsLeading(*bp++))
					while (UTF8CharIsTrailing(*bp))
					{
						if (dst < edst)
							*dst++ = *bp;
						++bp;
						++nm;
					}
			}
			if (bp < ep)
				return nm;
		}
	}
	if (dst < edst)
		*dst = '\0';
	return ++nm;
}

/*
						regsymbolic()
append a string and an int or char to a buffer and move buffer to end
*/
#define regsymadd(buf, s, send, chr, ischr) \
	regsymaddfull(buf, s, strcountof(s), send, strcountof(send), chr, ischr)
void regsymaddfull(register regchar** buf,
			   regchar* s,
			   size_t slen,
			   register regchar* send,
			   size_t sendlen,
			   register int chr,
			   register int ischr)
{
	register regchar t;					/* temp for integer conversion		*/
	register regchar* cp;				/* low position in buffer			*/
	register regchar* ep;				/* high position in buffer			*/

	/* copy message string in */
	regmemcpy(*buf, s, slen * sizeof(regchar));

	/* increment buffer to end */
	*buf += slen;

	/* appending a char? */
	if (ischr == 1)
	{
		/* append char and increment buffer by 1  */
		**buf = (regchar) chr;
		++*buf;
	}
	else if (ischr == 0 || ischr == 2)
	{
		/* appending an int - save low position for later reversing */
		cp = *buf - 1;

		/* convert int to string backwards */
		*(*buf)++ = (regchar) '0' + chr % 10;
		chr /= 10;

		while (chr)
		{
			*(*buf)++ = (regchar) '0' + chr % 10;
			chr /= 10;
		}

		/* save high position for reversing */
		ep = *buf;

		/* reverse string to normal viewing */
		while (++cp < --ep)
		{
			t = *cp;
			*cp = *ep;
			*ep = t;
		}
	}

	/* add a newline to the end */
	if (send)
	{
		/* copy newline string in */
		regmemcpy(*buf, send, sendlen * sizeof(regchar));

		/* increment buffer to end */
		*buf += sendlen;
	}
}

/** [Private] callback for each NFA state to be represented	*/
static void regsymbolicrec(register regchar** ap, regchar** s, int* numEOT)
{
	switch(*(*ap)++)
	{
	case REG_CHR:
		regsymadd(s, _T("\tCHR "), _T("\n"), *(*ap), 1);
		(*ap) += 1;
		break;
	case REG_ICR:
		regsymadd(s, _T("\tICR "), _T("\n"), *(*ap), 1);
		(*ap) += 1;
		break;
	case REG_8CC:
		regsymadd(s, _T("\tUTF8 CHR "), _T("\n"), *(*ap), 1);
		(*ap) += UTF8CharLen((*ap));
		break;
	case REG_8IC:
		regsymadd(s, _T("\tUTF8 ICR "), _T("\n"), *(*ap), 1);
		(*ap) += UTF8CharLen((*ap));
		break;
	case REG_ANY:
		regsymadd(s, _T("\tANY .\n"), _T("\n"), *(*ap) ? 'F' : ' ', -1);
		(*ap) += UTF8CharLen((*ap));
		break;
	case REG_BOL:
		regsymadd(s, _T("\tBOL "), _T("\n"), *(*ap)++ ? 'F' : ' ', -1);
		break;
	case REG_EOL:
		regsymadd(s, _T("\tEOL "), _T("\n"), *(*ap)++ ? 'F' : ' ', -1);
		break;
	case REG_BOT:
		++*numEOT;
		regsymadd(s, _T("(BOT( next:"), _T(""), *((unsigned short*)(*ap)), 0);
		(*ap) += 2;
		regsymadd(s, _T("\tbotstoinval:"), _T(""), *(*ap), 0);
		(*ap) += 1;
		regsymadd(s, _T("\tfixlen:"), _T("\n"), *(*ap), 0);
		(*ap) += 1;
		break;
	case REG_EOT:
		--*numEOT;
		regsymadd(s, _T(")EOT) #"), _T("\n"), *(*ap)++, 0);
		break;
	case REG_REF:
		regsymadd(s, _T("\tREF #"), _T("\n"), *(*ap)++, 0);
		break;
	case REG_NCL:
	case REG_CCL:
		{
			regchar n = 0;
			regmemcpy((*s), _T("\tCCL ["), strcountof(_T("\tCCL [")) * sizeof(regchar));
			(*s) += strcountof(_T("\tCCL ["));
			while (n++ != (REG_MAXCHR - 1)) /* we want to include the char at REG_MAXCHR without overflow */
			{
				if (regisinset((*ap),n))
				{
					switch(n)
					{
					case '\n':
						*(*s)++ = '\\';
						*(*s)++ = 'n';
						break;
					case '\r':
						*(*s)++ = '\\';
						*(*s)++ = 'r';
						break;
					case '\t':
						*(*s)++ = '\\';
						*(*s)++ = 't';
						break;
					default:
						*(*s)++ = n;
						break;
					}
				}
			};
			*(*s)++ = ']';
			*(*s)++ = '\n';
			(*ap) += REG_BITBLK;
			break;
		}
	case REG_HUM:
	case REG_GRE:
	case REG_POS:
	{
		int min = *(*ap)++, max = *(*ap)++;
		regsymadd(s, _T("\tCLOSURE"), _T("\n"), ' ', -1);
		regsymbolicrec(ap, s, numEOT);
		regsymadd(s, _T("{"), _T(""), min, 0);
		regsymadd(s, _T(","), _T("}\n"), max, 0);
		break;
	}
	case REG_ALT:
		regsymadd(s, _T("\t|ALT| next:"), _T(""), *((unsigned short*)(*ap)), 0);
		(*ap) += 2;
		regsymadd(s, _T("\tbotstoinval:"), _T(""), *(*ap), 0);
		(*ap) += 1;
		regsymadd(s, _T("\tfixlen:"), _T("\n"), *(*ap), 0);
		(*ap) += 1;
		break;
	case REG_AND:
		regsymadd(s, _T("\t&AND& fixlen:"), _T("\n"), *(*ap), 0);
		(*ap) += 1;
		break;
	case REG_BND:
		regsymadd(s, _T("\tWORD BOUNDARY: "), _T("\n"), *(*ap)++, 0);
		break;
	case REG_BOW:
		regsymadd(s, _T("\tBOW "), _T("\n"), ' ', -1);
		break;
	case REG_EOW:
		regsymadd(s, _T("\tEOW "), _T("\n"), ' ', -1);
		break;
	case REG_KPO:
		regsymadd(s, _T("\tKEEP OUT "), _T("\n"), ' ', -1);
		break;
	case REG_NLB:
		++*numEOT;
		regsymadd(s, _T("(?<!NEGATIVE LOOKBEHIND(?<! next:"), _T(""), *((unsigned short*)(*ap)), 0);
		((*ap) += 2);
		regsymadd(s, _T("\tbotstoinval:"), _T(""), *(*ap), 0);
		(*ap) += 1;
		regsymadd(s, _T("\tfixlen:"), _T("\n"), *(*ap), 0);
		(*ap) += 1;
		break;
	case REG_PLB:
		++*numEOT;
		regsymadd(s, _T("(?<=POSITIVE LOOKBEHIND(?<= next:"), _T(""), *((unsigned short*)(*ap)), 0);
		((*ap) += 2);
		regsymadd(s, _T("\tbotstoinval:"), _T(""), *(*ap), 0);
		(*ap) += 1;
		regsymadd(s, _T("\tfixlen:"), _T("\n"), *(*ap), 0);
		(*ap) += 1;
		break;
	case REG_NLA:
		++*numEOT;
		regsymadd(s, _T("(?!NEGATIVE LOOKAHEAD(?! next:"), _T(""), *((unsigned short*)(*ap)), 0);
		((*ap) += 2);
		regsymadd(s, _T("\tbotstoinval:"), _T(""), *(*ap), 0);
		(*ap) += 1;
		regsymadd(s, _T("\tfixlen:"), _T("\n"), *(*ap), 0);
		(*ap) += 1;
		break;
	case REG_PLA:
		++*numEOT;
		regsymadd(s, _T("(?=POSITIVE LOOKAHEAD(?= next:"), _T(""), *((unsigned short*)(*ap)), 0);
		((*ap) += 2);
		regsymadd(s, _T("\tbotstoinval:"), _T(""), *(*ap), 0);
		(*ap) += 1;
		regsymadd(s, _T("\tfixlen:"), _T("\n"), *(*ap), 0);
		(*ap) += 1;
		break;
	case REG_CON:
		++*numEOT;
		regsymadd(s, _T("(?IF->THEN|ELSE(? conditional next:"), _T(""), *((unsigned short*)(*ap)), 0);
		((*ap) += 2);
		regsymadd(s, _T("\tbotstoinval:"), _T(""), *(*ap), 0);
		(*ap) += 1;
		regsymadd(s, _T("\tfixlen:"), _T("\n"), *(*ap), 0);
		(*ap) += 1;
		break;
	default:
		regsymadd(s, _T("!!!BAD NFA!!!"), _T("\n"), ' ', -1);
		return;
	}
}

/**
* Obtains a string representation of a compiled NFA from regcomp()
* Useful for debugging
*
* @param r compiled regex from regcomp()
*
* @param outbuf buffer to receive the text representation,
* buffer must be passed to free() when done
*/
void regsymbolic(register const regex_t* r, register regchar** outbuf,
				 size_t* outbuflen, size_t* outbufsize)
{
	regchar* ap = r->nfa;		/* current state */
	/* -1 for EOT/), + for BOT/). RE opens with (, so ending ) will be 0 */
	int numEOT = 0;				/* ) number, if 0 at end */
	size_t bufsize;

	/* Allocate buffer - We need up to 150 characters for each state	  */
	register regchar* bufstart;
	bufsize = (r->nfaSize * 150) * sizeof(regchar);
	bufstart = *outbuf =
		REG_CPP_CAST(regchar*) malloc(bufsize);
	if (!bufstart)
		return;

	/* Loop through NFA and generate a representation for each state */
	do
	{
		/* Parse NFA state and generate representation */
		regsymbolicrec(&ap, outbuf, &numEOT);
	} while (numEOT > 0);

	/* Move the outbuf back to the beginning and finish */
	**outbuf = '\0';
	if(outbuflen)
		*outbuflen = *outbuf - bufstart;
	if(outbufsize)
		*outbufsize = bufsize;
	*outbuf = bufstart;
}

/**
* Obtains the string representation of an error code (errcode)
*
* @param errcode err code returned from regcomp(), regexec(), or regsubs()
*
* @param r compiled regular expression
*
* @param errbuf [Optional] buffer to recieve the string representation
*
* @param errbuf_size [Optional] errbuf size in bytes
*
* @return Size needed for a full representation of the string including
* the null termination character.
*/
size_t regerror(register int errcode, register const regex_t* r,
				register regchar* errbuf, register size_t errbuf_size)
{
	regchar* s;
	size_t outsize;
	size_t fulloutsize;
	size_t patlen;
	size_t outamount;

	/* errors we can print out extra info for first */
	switch (errcode)
	{
	/* regexec () */
	case REG_NOMATCH:
		s = REG_NOMATCHSTR; outsize = sizeof(REG_NOMATCHSTR); break;
	case REG_IECLOSE:
		s = REG_IECLOSESTR; outsize = sizeof(REG_IECLOSESTR); break;
	case REG_IENFA:
		s = REG_IENFASTR; outsize = sizeof(REG_IENFASTR); break;
	/* regcomp () */
	case REG_ECOLLATE:
		s = REG_ECOLLATESTR; outsize = sizeof(REG_ECOLLATESTR); break;
	case REG_ECTYPE:
		s = REG_ECTYPESTR; outsize = sizeof(REG_ECTYPESTR); break;
	case REG_EESCAPE:
		s = REG_EESCAPESTR; outsize = sizeof(REG_EESCAPESTR); break;
	case REG_ESUBREG:
		s = REG_ESUBREGSTR; outsize = sizeof(REG_ESUBREGSTR); break;
	case REG_EBRACK:
		s = REG_EBRACKSTR; outsize = sizeof(REG_EBRACKSTR); break;
	case REG_EPAREN:
		s = REG_EPARENSTR; outsize = sizeof(REG_EPARENSTR); break;
	case REG_EBRACE:
		s = REG_EBRACESTR; outsize = sizeof(REG_EBRACESTR); break;
	case REG_BADBR:
		s = REG_BADBRSTR; outsize = sizeof(REG_BADBRSTR); break;
	case REG_ERANGE:
		s = REG_ERANGESTR; outsize = sizeof(REG_ERANGESTR); break;
	case REG_BADRPT:
		s = REG_BADRPTSTR; outsize = sizeof(REG_BADRPTSTR); break;
	case REG_EMPTYRPT:
		s = REG_EMPTYRPTSTR; outsize = sizeof(REG_EMPTYRPTSTR); break;
	case REG_EMPTYPRN:
		s = REG_EMPTYPRNSTR; outsize = sizeof(REG_EMPTYPRNSTR); break;
	case REG_EMPTYSA:
		s = REG_EMPTYSASTR; outsize = sizeof(REG_EMPTYSASTR); break;
	case REG_REQFIXED:
		s = REG_REQFIXEDSTR; outsize = sizeof(REG_REQFIXEDSTR); break;
	case REG_LOOKINSUB:
		s = REG_LOOKINSUBSTR; outsize = sizeof(REG_LOOKINSUBSTR); break;
	case REG_UTFINBRACK:
		s = REG_UTFINBRACKSTR; outsize = sizeof(REG_UTFINBRACKSTR); break;
	case REG_BADCON:
		s = REG_BADCONSTR; outsize = sizeof(REG_BADCONSTR); break;
	default:
		s = NULL; outsize = 0; break;
	}

	if(outsize)
	{
		patlen = regstrlen(r->pat);
		fulloutsize = outsize + 1 /*\n*/
			+ 4 /*---\n*/
			+ patlen + 1 /*\n*/
			+ r->patPos + 1 /*[patPos blank]^*/
			+ 1; /*\0*/
		if (errbuf_size && errbuf)
		{
			if(errbuf_size > outsize)
				outamount = outsize;
			else
				outamount = errbuf_size;
			regmemcpy(errbuf, s, (outamount - 1) * sizeof(regchar));
			if(errbuf_size >= fulloutsize)
			{
				errbuf[outamount-1] = '\n';
				errbuf[outamount] = errbuf[outamount+1] = errbuf[outamount+2] = '-';
				errbuf[outamount+3] = '\n';
				regmemcpy(&errbuf[outamount+4], r->pat, patlen * sizeof(regchar));
				errbuf[outamount+4+patlen] = '\n';
				regmemset(&errbuf[outamount+4+patlen+1], ' ', r->patPos * sizeof(regchar));
				errbuf[outamount+4+patlen+1+r->patPos] = '^';
				errbuf[outamount+4+patlen+1+r->patPos+1] = '\0';
			}
			else
				errbuf[outamount-1] = '\0';
		}
		return fulloutsize;
	}

	/* errors we can't print out extra info for */
	switch (errcode)
	{
	case REG_OK:
		s = REG_OKSTR; outsize = sizeof(REG_OKSTR); break;
	case REG_BADPAT:
		s = REG_BADPATSTR; outsize = sizeof(REG_BADPATSTR); break;
	case REG_NOSTRING:
		s = REG_NOSTRINGSTR; outsize = sizeof(REG_NOSTRINGSTR); break;
	case REG_ESPACE:
		s = REG_ESPACESTR; outsize = sizeof(REG_ESPACESTR); break;
	case REG_SRCNINIT:
		s = REG_SRCNINITSTR;  outsize = sizeof(REG_SRCNINITSTR); break;
	case REG_DSTINIT:
		s = REG_DSTINITSTR; outsize = sizeof(REG_DSTINITSTR); break;
	case REG_NOTCOMP:
		s = REG_NOTCOMPSTR; outsize = sizeof(REG_NOTCOMPSTR); break;
	default:
		s = REG_UNKNOWNSTR; outsize = sizeof(REG_UNKNOWNSTR); break;
	}

	if (errbuf_size && errbuf)
	{
		if(errbuf_size > outsize)
			outamount = outsize;
		else
			outamount = errbuf_size;
		regmemcpy(errbuf, s, (outamount - 1) * sizeof(regchar));
		errbuf[outamount-1] = '\0';
	}
	return outsize;
}

/*-----------------------------------------------------------------------------
								[Unit Tests]
-----------------------------------------------------------------------------*/
#if defined(WIKISERVER_VERSION) && defined(WSTESTOPT)
#include "test.h"
#include <stdio.h>
void PrintMatches(regmatch_t* matches, size_t i, size_t re_nsub, const char* search)
{
#if 0
	for(i = 0; i < re_nsub + 1; ++i)
		if (matches[i].rm_so != -1 && matches[i].rm_eo >= matches[i].rm_so)
		{
			char* thesub =
				CStrSubString(search, matches[i].rm_so,
							matches[i].rm_eo - matches[i].rm_so);
			printf("%d: %s\n",
				i, thesub);
			WSFREE(thesub);
		}
#endif
}
#define WSREGEX_ASSERT_BASE(pattern, search, compflags, desres) \
{ \
	regmatch_t* matches; \
	regex_t re; \
	size_t i;\
	wsbool success = regcomp(&re, pattern, compflags) == REG_OK; \
	if(!success) \
		WSTEST_WARNING("Compilation of "#pattern" FAILED."); \
	else \
	{ \
		matches = WSALLOC(regmatch_t, re.re_nsub + 1); \
		if(!desres) \
			WSTEST_ASSERT(regexec(&re, search, re.re_nsub + 1, matches, 0) == REG_OK); \
		else \
			WSTEST_ASSERT(regexec(&re, search, re.re_nsub + 1, matches, 0) != REG_OK); \
		if(success) \
		{ \
			PrintMatches(matches, i = 0, re.re_nsub, search); \
			regfree(&re); \
		} \
		WSFREE(matches); \
	} \
}

#define WSREGEX_ASSERT_BASIC_SUCCESS(pattern, search) \
	WSREGEX_ASSERT_BASE(pattern, search, 0, REG_OK)
#define WSREGEX_ASSERT_BASIC_FAILURE(pattern, search) \
	WSREGEX_ASSERT_BASE(pattern, search, 0, ~REG_OK)
#define WSREGEX_ASSERT_EXTENDED_SUCCESS(pattern, search) \
	WSREGEX_ASSERT_BASE(pattern, search, REG_EXTENDED, REG_OK)
#define WSREGEX_ASSERT_EXTENDED_FAILURE(pattern, search) \
	WSREGEX_ASSERT_BASE(pattern, search, REG_EXTENDED, ~REG_OK)
#define WSREGEX_ASSERT_ENHANCED_SUCCESS(pattern, search) \
	WSREGEX_ASSERT_BASE(pattern, search, REG_ENHANCED | REG_EXTENDED, REG_OK)
#define WSREGEX_ASSERT_ENHANCED_FAILURE(pattern, search) \
	WSREGEX_ASSERT_BASE(pattern, search, REG_ENHANCED | REG_EXTENDED, ~REG_OK)

WSTEST_FUNCTION(regdottest)
{
	WSREGEX_ASSERT_BASIC_SUCCESS("r.na", "rdna")
	WSREGEX_ASSERT_BASIC_FAILURE("r.na", "rna")
}

WSTEST_FUNCTION(regstartest)
{
	WSREGEX_ASSERT_BASIC_SUCCESS("rd*na", "rddddna")
	WSREGEX_ASSERT_BASIC_SUCCESS("rd*na", "rna")
}

WSTEST_FUNCTION(regboltest)
{
	WSREGEX_ASSERT_BASIC_SUCCESS("^b", "b")
	WSREGEX_ASSERT_BASIC_SUCCESS("^bol", "bol")
	WSREGEX_ASSERT_BASIC_FAILURE("^bol", "abol")
}

WSTEST_FUNCTION(regeoltest)
{
	WSREGEX_ASSERT_BASIC_SUCCESS("b$", "b")
	WSREGEX_ASSERT_BASIC_SUCCESS("bol$", "bol")
	WSREGEX_ASSERT_BASIC_FAILURE("bol$", "bola")
	WSREGEX_ASSERT_BASIC_FAILURE("bol$", "abola")
}

WSTEST_FUNCTION(regquestionmarktest)
{
	WSREGEX_ASSERT_EXTENDED_SUCCESS("rd?na", "rdna")
	WSREGEX_ASSERT_EXTENDED_SUCCESS("rd?na", "rna")
	WSREGEX_ASSERT_EXTENDED_FAILURE("rd?na", "rddna")
}

WSTEST_FUNCTION(regplusparenthasestest)
{
	WSREGEX_ASSERT_EXTENDED_SUCCESS("a(bb)+c", "abbc");
	WSREGEX_ASSERT_EXTENDED_SUCCESS("a(bb)+c", "abbbbc");
	WSREGEX_ASSERT_EXTENDED_FAILURE("a(bb)+c", "abc");
	WSREGEX_ASSERT_EXTENDED_FAILURE("a(bb)+c", "ac");
}

WSTEST_FUNCTION(regclosurecompliancetest)
{
	WSREGEX_ASSERT_EXTENDED_SUCCESS("(kkk|kk+)kiii", "kkkiii");
}

WSTEST_FUNCTION(regpositivelookaheadtest)
{ /* X(?=Y), matches X, but match only if followed by Y. */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("X(?=Y)", "XY");
	WSREGEX_ASSERT_ENHANCED_FAILURE("X(?=Y)", "X");
	WSREGEX_ASSERT_ENHANCED_FAILURE("X(?=Y)", "XX");
}

WSTEST_FUNCTION(regnegativelookaheadtest)
{ /* X(?!Y), matches X, but only if not followed by Y. */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("X(?!Y)", "XX");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("X(?!Y)", "X");
	WSREGEX_ASSERT_ENHANCED_FAILURE("X(?!Y)", "XY");
}

WSTEST_FUNCTION(regpositivelookbehindtest)
{ /* (?<=Y)X, matches X, but only if there?s Y before it. */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(?<=Y)X", "YX");
	WSREGEX_ASSERT_ENHANCED_FAILURE("(?<=Y)X", "X");
	WSREGEX_ASSERT_ENHANCED_FAILURE("(?<=Y)X", "XX");
}

WSTEST_FUNCTION(regnegativelookbehindtest)
{ /* (?<!Y)X, matches X, but only if there?s no Y before it. */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(?<!Y)X", "X");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(?<!Y)X", "XX");
	/*WSTEST_WARNING("SANITY CHECK");*/
	WSREGEX_ASSERT_ENHANCED_FAILURE("(?<!Y)X", "YX");
	WSREGEX_ASSERT_ENHANCED_FAILURE("(?<!Y)X", "YYX");
}

WSTEST_FUNCTION(regurltest)
{
	/* https://stackoverflow.com/questions/27745/getting-parts-of-a-url-regex */
	const char* pattern = "^(([^:\\/?#]+):)?(\\/\\/([^\\/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?";
	regmatch_t matches[12];
	regex_t re;
	wsbool success = regcomp(&re, pattern, REG_ENHANCED | REG_EXTENDED) == REG_OK;
	WSTEST_ASSERT(success);
	if(success)
	{
		const char* search;
		search = "mailto:joe@someone.com";
		success = regexec(&re, search, countof(matches), matches, 0) == REG_OK;
		WSTEST_ASSERT(success);
		if(success)
		{
			WSTEST_ASSERT(strncmp("mailto", &search[matches[2].rm_so], strcountof("mailto")) == 0);
			WSTEST_ASSERT(strncmp("joe@someone.com", &search[matches[5].rm_so], strcountof("joe@someone.com")) == 0);
		}
		search = "jim:bob@somewhere.com:8080";
		success = regexec(&re, search, countof(matches), matches, 0) == REG_OK;
		WSTEST_ASSERT(success);
		if(success)
		{
			WSTEST_ASSERT(strncmp("jim", &search[matches[2].rm_so], strcountof("jim")) == 0);
			WSTEST_ASSERT(strncmp("bob@somewhere.com:8080", &search[matches[5].rm_so], strcountof("bob@somewhere.com:8080")) == 0);
		}
		search = "user:pass@example.com";
		success = regexec(&re, search, countof(matches), matches, 0) == REG_OK;
		WSTEST_ASSERT(success);
		if(success)
		{
			WSTEST_ASSERT(strncmp("user", &search[matches[2].rm_so], strcountof("user")) == 0);
			WSTEST_ASSERT(strncmp("pass@example.com", &search[matches[5].rm_so], strcountof("pass@example.com")) == 0);
		}
	}
	pattern = "^(([^@:\\/\\s]+):\\/?)?\\/?(([^@:\\/\\s]+)(:([^@:\\/\\s]+))?@)?([^@:\\/\\s]+)(:(\\d+))?(((\\/\\w+)*\\/)([\\w\\-\\.]+[^#?\\s]*)?(.*)?(#[\\w\\-]+)?)?$";
	success = regcomp(&re, pattern, REG_ENHANCED | REG_EXTENDED) == REG_OK;
	WSTEST_ASSERT(success);
	if(success)
	{
		const char* search;
		search = "mailto:joe@someone.com";
		success = regexec(&re, search, countof(matches), matches, 0) == REG_OK;
		WSTEST_ASSERT(success);
		if(success)
		{
			WSTEST_ASSERT(strncmp("mailto", &search[matches[2].rm_so], strcountof("mailto")) == 0);
			WSTEST_ASSERT(strncmp("joe", &search[matches[4].rm_so], strcountof("joe")) == 0);
			WSTEST_ASSERT(strncmp("someone.com", &search[matches[7].rm_so], strcountof("someone.com")) == 0);
		}
		search = "jim:bob@somewhere.com:8080";
		success = regexec(&re, search, countof(matches), matches, 0) == REG_OK;
		WSTEST_ASSERT(success);
		if(success)
		{
			WSTEST_ASSERT(strncmp("jim", &search[matches[2].rm_so], strcountof("jim")) == 0);
			WSTEST_ASSERT(strncmp("bob", &search[matches[4].rm_so], strcountof("bob")) == 0);
			WSTEST_ASSERT(strncmp("somewhere.com", &search[matches[7].rm_so], strcountof("somewhere.com")) == 0);
			WSTEST_ASSERT(strncmp("somewhere.com", &search[matches[7].rm_so],
				matches[7].rm_eo - matches[7].rm_so) == 0);
			WSTEST_ASSERT(strncmp("8080", &search[matches[9].rm_so], strcountof("8080")) == 0);
		}
		search = "user:pass@example.com";
		success = regexec(&re, search, countof(matches), matches, 0) == REG_OK;
		WSTEST_ASSERT(success);
		if(success)
		{
			WSTEST_ASSERT(strncmp("user", &search[matches[2].rm_so], strcountof("user")) == 0);
			WSTEST_ASSERT(strncmp("pass", &search[matches[4].rm_so], strcountof("pass")) == 0);
			WSTEST_ASSERT(strncmp("example.com", &search[matches[7].rm_so], strcountof("someone.com")) == 0);
		}
	}
}

WSTEST_FUNCTION(regextratest)
{
	/* #1: Positive lookbehind */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(?<=Y)X", "YX");
	WSREGEX_ASSERT_ENHANCED_FAILURE("(?<=Y)X", "X");

	/* #2: Negative lookbehind */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(?<!Y)X", "AX");
	WSREGEX_ASSERT_ENHANCED_FAILURE("(?<!Y)X", "YX");

	/* #3: Positive lookahead */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("X(?=Y)", "XY");
	WSREGEX_ASSERT_ENHANCED_FAILURE("X(?=Y)", "XZ");

	/* #4: Negative lookahead */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("X(?!Y)", "XZ");
	WSREGEX_ASSERT_ENHANCED_FAILURE("X(?!Y)", "XY");

	/* #5: Word boundary */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("\\bword\\b", "a word is here");
	WSREGEX_ASSERT_ENHANCED_FAILURE("\\bword\\b", "wordless");

	/* #6: Non-word boundary */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("\\Bend", "weekend");
	WSREGEX_ASSERT_ENHANCED_FAILURE("\\Bend", "the end");

	/* #7: Character classes */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("\\d+", "123");
	WSREGEX_ASSERT_ENHANCED_FAILURE("\\d+", "abc");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("\\w+", "word");
	WSREGEX_ASSERT_ENHANCED_FAILURE("\\w+", "%%!");

	/* #8: Alternation */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("cat|dog", "I have a cat");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("cat|dog", "I have a dog");
	WSREGEX_ASSERT_ENHANCED_FAILURE("cat|dog", "I have a bird");

	/* #9: Quantifiers */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho+", "hoo");
	WSREGEX_ASSERT_ENHANCED_FAILURE("ho+", "h");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho?", "ho");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho?", "h");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho?", "hoo");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho{2}", "hoo");
	WSREGEX_ASSERT_ENHANCED_FAILURE("ho{2}", "ho");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho{1,}", "hoo");
	WSREGEX_ASSERT_ENHANCED_FAILURE("ho{1,}", "hh");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho{1,2}", "ho");
	WSREGEX_ASSERT_ENHANCED_FAILURE("ho{1,2}", "h");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho{,2}d", "hod");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho{,2}d", "hood");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho{,2}d", "hd");
	WSREGEX_ASSERT_ENHANCED_FAILURE("ho{,2}d", "hoood");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho{0}", "h");
	WSREGEX_ASSERT_ENHANCED_FAILURE("ho{0}", "o");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho{0}", "ho");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("ho{0}", "hh");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(ho){2}", "hoho");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(ho){2}", "hohoh");
	WSREGEX_ASSERT_ENHANCED_FAILURE("(ho){2}", "hoh");

	/* #10: Grouping and capturing */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(foo)(bar)", "foobar");
	WSREGEX_ASSERT_ENHANCED_FAILURE("(foo)(bar)", "foobaz");

	/* #11: Anchors */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("^start", "start of the line");
	WSREGEX_ASSERT_ENHANCED_FAILURE("^start", "middle start");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("end$", "at the end");
	WSREGEX_ASSERT_ENHANCED_FAILURE("end$", "end in the middle");

	/* #12: Dot character */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("a.b", "acb");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("a.b", "aXb");
	WSREGEX_ASSERT_ENHANCED_FAILURE("a.b", "ab");
	/* NB: Dot matches newline without flags */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("a.b", "a\nb");

	/* #13: Escaped special characters */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("\\.\\+\\*", ".+*");
	WSREGEX_ASSERT_ENHANCED_FAILURE("\\.\\+\\*", "abc");

	/* #14: Character sets and ranges */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("[abc]", "a");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("[a-c]", "b");
	WSREGEX_ASSERT_ENHANCED_FAILURE("[abc]", "d");
	WSREGEX_ASSERT_ENHANCED_FAILURE("[a-c]", "z");

	/* #15: Negated character sets */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("[^abc]", "d");
	WSREGEX_ASSERT_ENHANCED_FAILURE("[^abc]", "b");

	/* #16: Greedy vs lazy quantifiers */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("a.*b", "axxxb");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("a.*?b", "axxxb");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("a.*b", "ab");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("a.+?b", "axxxb");
	WSREGEX_ASSERT_ENHANCED_FAILURE("a.+?b", "ab");

	/* #17: Non-capturing groups */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(?:foo)bar", "foobar");
	WSREGEX_ASSERT_ENHANCED_FAILURE("(?:foo)bar", "foobaz");

	/* #18: Named capturing groups (Not supported yet) */
	/*WSREGEX_ASSERT_ENHANCED_SUCCESS("(?<word>\\w+)\\s+\\k<word>", "repeat repeat");*/
	/*WSREGEX_ASSERT_ENHANCED_FAILURE("(?<word>\\w+)\\s+\\k<word>", "repeat test");*/

	/* #19: Backreferences */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(\\w+) \\1", "word word");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("(\\w+) \\1", "word words");
	WSREGEX_ASSERT_ENHANCED_FAILURE("(\\w+) \\1", "word wors");

	/* #20: Assertions */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("\\bword\\b", "a word is here");
	WSREGEX_ASSERT_ENHANCED_FAILURE("\\bword\\b", "wordless");

	/* #21: Unicode property (Not supported yet) */
	/*WSREGEX_ASSERT_ENHANCED_SUCCESS("\\p{L}", "a");*/
	/*WSREGEX_ASSERT_ENHANCED_FAILURE("\\p{L}", "1");*/

	/* #22: Complex patterns */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("^(\\w+).+\\1$", "start and end with start");
	WSREGEX_ASSERT_ENHANCED_FAILURE("^(\\w+).+\\1$", "start and end with finish");
	WSREGEX_ASSERT_ENHANCED_FAILURE("^(d{2}).+\\1$", "ddddr");
	WSREGEX_ASSERT_ENHANCED_FAILURE("^(d+).+\\1$", "ddr");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("^(\\w+)\\1$", "ss");
	WSREGEX_ASSERT_ENHANCED_FAILURE("^(\\w+)\\1$", "ssr");

	/* #23: Possessive - doesn't give back what it matches */
	WSREGEX_ASSERT_ENHANCED_SUCCESS("s++", "ss");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("s++d", "ssd");
	WSREGEX_ASSERT_ENHANCED_FAILURE("s++s", "ssd");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("s{1,}+", "ss");
	WSREGEX_ASSERT_ENHANCED_SUCCESS("s{1,}+d", "ssd");
	WSREGEX_ASSERT_ENHANCED_FAILURE("s{1,}+s", "ssd");
}

WSTEST_SUITE_BEGIN(WSREGEX)
	WSTEST_GROUP_BEGIN(Basic)
		WSTEST_CASE(regdottest)
		WSTEST_CASE(regstartest)
		WSTEST_CASE(regboltest)
		WSTEST_CASE(regeoltest)
	WSTEST_GROUP_END(Basic)
	WSTEST_GROUP_BEGIN(Extended)
		WSTEST_CASE(regquestionmarktest)
		WSTEST_CASE(regplusparenthasestest)
		WSTEST_CASE(regclosurecompliancetest)
	WSTEST_GROUP_END(Extended)
	WSTEST_GROUP_BEGIN(Enhanced)
		WSTEST_CASE(regpositivelookaheadtest)
		WSTEST_CASE(regnegativelookaheadtest)
		WSTEST_CASE(regpositivelookbehindtest)
		WSTEST_CASE(regnegativelookbehindtest)
		WSTEST_CASE(regurltest)
		WSTEST_CASE(regextratest)
	WSTEST_GROUP_END(Enhanced)
WSTEST_SUITE_END(WSREGEX)

#if defined(REGEXSELFCONTAINED)
int main(void)
{
	WSTEST_SUITE_RUN(WSREGEX);
	return 0;
}
#endif /* REGEXSELFCONTAINED */
#endif /* defined(WIKISERVER_VERSION) && defined(WSTESTOPT) */

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif /* EOF: defined(WSREGEXOPT) && !defined(WSNTVREOPT) */
#endif /* RE_IMPLEMENTATION */
