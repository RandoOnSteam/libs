/**
*	@file utf.h
*
*	 #####  #     # ######  #       ### #     # #######
*	#     # ##   ## #     # #        #  ##    # #
*	#       # # # # #     # #        #  # #   # #
*	#       #  #  # #     # #        #  #  #  # #####
*	#       #     # #     # #        #  #   # # #
*	#     # #     # #     # #        #  #    ## #
*	 #####  #     # ######  ####### ### #     # #######
*
*	Single header file for command line parsing.
*
*	###Instructions###
*
*	Define CMDLINE_IMPLEMENTATION in one source file for the implementation
*
*	###NOTES###
*	- ASCII art, mostly "Banner", from
*	-- budavariam.github.io/asciiart-text/multi
*
*	@version 0.2
*
*	Version History
*
*	0.2		Rename CmdLine_Construct/Destruct to CmdLine_Create/Destroy
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
#ifndef __CMDLINE_H__
#define __CMDLINE_H__
#include <stddef.h> /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

/*  Command line parsing */
typedef struct CMDLINEOPTION
{
	const char* longoption;
	size_t longoptionlen;
	char shortoption;
	char hasargument;
} CMDLINEOPTION;

typedef struct CMDLINE
{
	int argc;
	char** argv;
	char* arg;
	CMDLINEOPTION* options;
	size_t optioncount;
} CMDLINE;

void CmdLine_Create(CMDLINE* pThis, int argc, char** argv,
						 CMDLINEOPTION* options, size_t optioncount);
size_t CmdLine_Parse(CMDLINE* pThis, char** argument);
#define CmdLine_Destroy(pThis)
#ifdef __cplusplus
}
#endif
#endif /* __CMDLINE_H__ */
/******************************************************************************
	 ##### ###                                                        #####
	 #      #  #    # #####  #      ###### #    # ###### #    # #####     #
	 #      #  ##  ## #    # #      #      ##  ## #      ##   #   #       #
	 #      #  # ## # #    # #      #####  # ## # #####  # #  #   #       #
	 #      #  #    # #####  #      #      #    # #      #  # #   #       #
	 #      #  #    # #      #      #      #    # #      #   ##   #       #
	 ##### ### #    # #      ###### ###### #    # ###### #    #   #   #####
******************************************************************************/
#if defined(CMDLINE_IMPLEMENTATION)
/* Do a quick implementation of AnsiiStartsWithICase if needed */
#ifndef __MUTSTR_H__
char CmdLineCharToLower(char c)
{ return ((c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c); }

int CmdLineAnsiiCompare_(const char* s1, const char* s2, int ignore_case,
	size_t nNum1, size_t nNum2)
{
	size_t nPos = 0;
	int e1, e2;

	while (nPos < nNum1 && nPos < nNum2) {
		char c1 = s1[nPos];
		char c2 = s2[nPos];

		if (c1 == '\0' && nNum1 == (size_t)-1)
			break;
		if (c2 == '\0' && nNum2 == (size_t)-1)
			break;

		if (ignore_case)
		{
			c1 = CmdLineCharToLower(c1);
			c2 = CmdLineCharToLower(c2);
		}

		if (c1 != c2)
			return (c1 < c2) ? -1 : 1;

		++nPos;
	}

	e1 = (nPos == nNum1 || (nNum1 == (size_t)-1 && s1[nPos] == '\0'));
	e2 = (nPos == nNum2 || (nNum2 == (size_t)-1 && s2[nPos] == '\0'));

	if (e1 && e2)
		return 0;
	else if (e1)
		return -1;
	else if (e2)
		return 1;

	return 0;
}

int CmdLineAnsiiStartsWithICase(const char* s1, const char* s2, size_t s2len)
{ return CmdLineAnsiiCompare_(s1, s2, 1, s2len, s2len) == 0; }
#else
int CmdLineAnsiiStartsWithICase(const char* s1, const char* s2, size_t s2len)
{ return AnsiiStartsWithICase(s1, s2, s2len); }
#endif
/**
* @}
* @defgroup CMDLINE CmdLine
* @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
* Initializes a command line parser object with command line arguments.
*
* This function sets up a `CMDLINE` object with the command line arguments
* passed to the application, along with a set of expected options. It processes
* the `argc` and `argv` parameters typically provided by the `main` function
* and stores them in the `CMDLINE` object for further parsing. The function
* also adjusts `argc` and `argv` to skip the executable name if it's present
* as the first argument (indicated by not starting with a '-'). This adjustment
* makes further processing of arguments and options simpler by directly
* starting with actual command line arguments or options.
*
* This function assumes `pThis` and `argv` are valid pointers and that
* `argc`, `argv`, and `optioncount` correctly represent the command line
* arguments and options. Care should be taken to ensure that `options`
* points to a valid array of `CMDLINEOPTION` structures with at least
* `optioncount` elements.
*
* @param pThis Pointer to the `CMDLINE` object to initialize.
* @param argc The number of command line arguments, as provided to `main`.
* @param argv The array of command line arguments, as provided to `main`.
*             If the first argument (argv[0]) is not an option (does not start
*             with '-'), it is assumed to be the executable name and is
*			  skipped.
*             This is reflected by decrementing `argc` and incrementing `argv`
*             to bypass the executable name in the command line arguments
*			  array.
* @param options Pointer to an array of `CMDLINEOPTION` structures that
*                describe the expected options.
* @param optioncount The number of elements in the `options` array.
*/
void CmdLine_Create(CMDLINE* pThis, int argc, char** argv,
	CMDLINEOPTION* options, size_t optioncount)
{
	pThis->argc = argc; /* Handle argc being 0 */
	pThis->argv = argv;
	pThis->options = options;
	pThis->optioncount = optioncount;

	if(argc)
	{
		if(**pThis->argv != '-')
		{
			/* assume first argument is the exe */
			--pThis->argc;
			++pThis->argv;
		}
		if(argc)
			pThis->arg = *pThis->argv;
	}
}

/**
* Parses the next command line argument or option.
*
* This function processes the next argument or option from the command line
* arguments stored in the `CMDLINE` object. It distinguishes between options
* (prefixed with '-' or '--') and standalone arguments, handling options with
* or without additional arguments (e.g., `-o value` or `--option=value`). The
* function identifies matched options based on the `options` array provided
* during initialization and retrieves any associated value if specified.
*
* If a standalone argument is encountered (not prefixed with '-'), or if an
* option does not require an additional argument, `argument` is set to point
* to the argument or the option's value. The function then prepares for the
* next call by advancing to the subsequent command line element.
*
* The function updates `pThis->argc` and `pThis->argv` to reflect the
* consumption of command line elements. If all arguments have been
* processed, `argument` is set to NULL, and 0 is returned.
*
* Options are expected to be prefixed with '-' for short options or '--'
* for long options. Short options that require an additional argument can
* have the argument directly concatenated (e.g., `-oValue`) or separated
* by space (e.g., `-o Value`). Long options can have their arguments
* separated by an '=' sign or a space (e.g., `--option=value` or
* `--option value`). If an option is expected to have an argument but
* none is provided, `argument` is set to NULL.
*
* @param pThis Pointer to the `CMDLINE` object being parsed.
* @param argument Pointer to a char pointer, set to point to the current
* argument or the value of the current option. If the current element is
* an option without an additional argument, `argument` is set to NULL.
* @return The index of the matched option in the `options` array, or 0 if the
*         current element is a standalone argument or an unrecognized option.
*/
size_t CmdLine_Parse(CMDLINE* pThis, char** argument)
{
	int isDouble; size_t i; int gotonextarg = 1;
	if (pThis->argc == 0)
	{
		*argument = NULL;
		return 0; /* Handle no more arguments */
	}

	if (pThis->arg == *pThis->argv && *pThis->arg != '-')
	{
		*argument = pThis->arg;
		--pThis->argc;
		pThis->arg = *++pThis->argv;
		return 0;
	}

	isDouble = pThis->arg == *pThis->argv
		&& pThis->arg[0] && pThis->arg[1] == '-';
	if(!isDouble && pThis->arg == *pThis->argv)
		++pThis->arg;
	for (i = 0; i < pThis->optioncount; ++i)
	{
		CMDLINEOPTION* opt = &pThis->options[i];

		if ((isDouble && opt->longoption
				&& CmdLineAnsiiStartsWithICase(pThis->arg + 2,
						opt->longoption, opt->longoptionlen))
			|| (!isDouble && pThis->arg[0] == opt->shortoption))
		{
			if (opt->hasargument)
			{
				if (isDouble)
				{
					char* posequalSign = pThis->arg + 2 + opt->longoptionlen;
					if (*posequalSign == '=' || *posequalSign == ' ')
					{
						*argument = posequalSign + 1;
					}
					else if (pThis->argc > 1)
					{
						*argument = *++pThis->argv;
						pThis->argc--;
					} else
					{
						*argument = NULL;
					}
				}
				else
				{
					if (pThis->arg[1] != '\0')
					{
						*argument = &pThis->arg[1];
						pThis->arg += 1;
						gotonextarg = 0;
					}
					else if (pThis->argc > 1)
					{
						*argument = *++pThis->argv;
						pThis->argc--;
					}
					else
					{
						*argument = NULL;
					}
				}
			}
			else
			{
				*argument = NULL;
				if(!isDouble)
				{
					++pThis->arg;
					if(*pThis->arg)
						gotonextarg = 0;
				}
			}
			break;
		}
	}

	if(i == pThis->optioncount)
	{
		*argument = pThis->arg;
		i = 0;
	}
	else
		++i;

	if(gotonextarg && pThis->argc > 0)
	{
		--pThis->argc;
		pThis->arg = *++pThis->argv;
	}

	return i;
}

#ifdef __cplusplus
}
#endif

/**
* @}
*/
#endif /* CMDLINE_IMPLEMENTATION */
