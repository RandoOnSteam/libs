/**
*	@file test.h
*
*	#     #  #####  ####### #######  #####  #######
*	#  #  # #     #    #    #       #     #    #
*	#  #  # #          #    #       #          #
*	#  #  #  #####     #    #####    #####     #
*	#  #  #       #    #    #             #    #
*	#  #  # #     #    #    #       #     #    #
*	 ## ##   #####     #    #######  #####     #
*
*	WS test suite.
*
*	###Contained Classes###
*
*	WSTESTCASE, WSTESTGROUP, WSTESTSUITE
*
*	###Contained Functions###
*
*	WSTEST_SUITE_BEGIN/END, WSTEST_GROUP_BEGIN/END, WSTEST_CASE,
*	WSTEST_SUITE_DECLARE, WSTEST_SUITE_RUN
*
*	@author Ryan Norton
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
#ifndef __TEST_H__
#define __TEST_H__
#ifdef __cplusplus
extern "C" {
#endif

#if !defined(WIKISERVER_VERSION)
typedef struct WSCONSOLE
{
	void* handle;
} WSCONSOLE;
#endif
int WSConsole_ConstructOutput(WSCONSOLE* pThis);
int WSConsole_Write(WSCONSOLE* pThis, const char* buf);
int WSConsole_WriteUInt(WSCONSOLE* pThis, size_t ui);
void WSConsole_Destruct(WSCONSOLE* pThis);
void WSConsoleSetColorRed(void);
void WSConsoleSetColorGreen(void);
void WSConsoleSetColorBlue(void);
void WSConsoleSetColorBold(void);
void WSConsoleSetColorReset(void);
void WSConsoleSetColorWhite(void);
void WSConsoleSetColorYellow(void);
void WSConsoleSetColorCyan(void);
void WSConsoleSetColorMagenta(void);
int WSDecimalPlaces (size_t n);

typedef struct WSTESTCASE
{
	const char* name;
	const char* groupname;
	int printedHeader;
	const char* filename;
	unsigned int fileline;
	unsigned int assertions;
	unsigned int failedAssertions;
	void (*lpfnCaseProc) (struct WSTESTCASE*);
	struct WSTESTCASE* next;
} WSTESTCASE;

typedef struct WSTESTGROUP
{
	const char* name;
	struct WSTESTCASE* cases;
	struct WSTESTGROUP* next;
} WSTESTGROUP;
typedef struct WSTESTSUITE
{
	const char* name;
	struct WSTESTGROUP* groups;
} WSTESTSUITE;

#define WSTEST_SUITE_BEGIN(suitename) \
	void WSTEST_SUITE_IMPL_##suitename(struct WSTESTSUITE* suite) \
	{ \
		WSTESTCASE** curcase; \
		WSTESTGROUP** curgroup = &suite->groups; \
		suite->name = #suitename;
#define WSTEST_GROUP_BEGIN(groupname) \
		*curgroup = WSALLOC1(WSTESTGROUP); (*curgroup)->name = #groupname; \
		curcase = &(*curgroup)->cases;
#define WSTEST_CASE(casename) \
		*curcase = WSALLOC1(WSTESTCASE); \
		(*curcase)->name = #casename; \
		(*curcase)->groupname = (*curgroup)->name; \
		(*curcase)->filename = __FILE__; (*curcase)->fileline = __LINE__; \
		(*curcase)->lpfnCaseProc = &WSTESTFUNCTION_##casename; \
		curcase = &(*curcase)->next;
#define WSTEST_GROUP_END(groupname) \
		*curcase = NULL; curgroup = &(*curgroup)->next;
#define WSTEST_SUITE_END(name) \
		*curgroup = NULL; \
	}
#define WSTEST_FUNCTION(name) \
	void WSTESTFUNCTION_##name(struct WSTESTCASE* pThis)
#ifndef WSTEST_SUITE_DECLARE
#define WSTEST_SUITE_DECLARE(suitename) \
	void WSTEST_SUITE_IMPL_##suitename(struct WSTESTSUITE* suite);
#endif

#ifndef WSALLOC1
#define WSALLOC1(x) (x*)malloc(sizeof(x))
#define	WSFREE(x) free((void*)(x))
#endif

#define WSTEST_ASSERT(exp) do { \
	++pThis->assertions; \
	if (!(exp)) \
	{ \
		WSCONSOLE conout; WSConsole_ConstructOutput(&conout); \
		if(pThis->printedHeader == 0) \
		{ \
			WSConsole_Write(&conout, \
				"-------------------------------------------------------------------------------\n"); \
			WSConsole_Write(&conout, pThis->groupname); \
			WSConsole_Write(&conout, "\n  "); \
			WSConsole_Write(&conout, pThis->name); \
			WSConsole_Write(&conout, "\n" \
				"-------------------------------------------------------------------------------\n"); \
			WSConsole_Write(&conout, pThis->filename); \
			WSConsole_Write(&conout, "("); \
			WSConsole_WriteUInt(&conout, pThis->fileline); \
			WSConsole_Write(&conout, ")\n" \
				"...............................................................................\n\n"); \
			pThis->printedHeader = 1; \
		} \
		/*WSConsoleSetColorYellow();*/ \
		WSConsole_Write(&conout, __FILE__); \
		WSConsole_Write(&conout, "("); \
		WSConsole_WriteUInt(&conout, __LINE__); \
		WSConsole_Write(&conout, "): FAILED:\n  " #exp "\n\n"); \
		/*WSConsoleSetColorReset();*/ \
		WSConsole_Destruct(&conout); \
		++pThis->failedAssertions; \
	} \
} while (0)

#define WSTEST_WARNING(msg) do { \
		WSCONSOLE conout; WSConsole_ConstructOutput(&conout); \
		if(pThis->printedHeader == 0) \
		{ \
			WSConsole_Write(&conout, \
				"-------------------------------------------------------------------------------\n"); \
			WSConsole_Write(&conout, pThis->groupname); \
			WSConsole_Write(&conout, "\n  "); \
			WSConsole_Write(&conout, pThis->name); \
			WSConsole_Write(&conout, "\n" \
				"-------------------------------------------------------------------------------\n"); \
			WSConsole_Write(&conout, pThis->filename); \
			WSConsole_Write(&conout, "("); \
			WSConsole_WriteUInt(&conout, pThis->fileline); \
			WSConsole_Write(&conout, ")\n" \
				"...............................................................................\n\n"); \
			pThis->printedHeader = 1; \
		} \
		WSConsole_Write(&conout, __FILE__); \
		WSConsole_Write(&conout, "("); \
		WSConsole_WriteUInt(&conout, __LINE__); \
		WSConsole_Write(&conout, "):\nwarning:\n  "); \
		WSConsole_Write(&conout, msg); \
		WSConsole_Write(&conout, "\n\n"); \
		WSConsole_Destruct(&conout); \
} while (0)

void WSTestSuiteRun(void (*lpRunFunc)(WSTESTSUITE* runner));

#define WSTEST_SUITE_RUN(suitename) \
	WSTestSuiteRun(WSTEST_SUITE_IMPL_##suitename);
#ifdef __cplusplus
}
#endif
#endif /* __TEST_H__ */

#if defined(WSTEST_IMPLEMENTATION)
#include <stdio.h> /* printf */
#ifdef __cplusplus
extern "C" {
#endif
/**
* Calculates the number of decimal places in a non-negative integer.
*
* This function determines the number of decimal places required to represent
* the non-negative integer `n`. It does so by comparing `n` against fixed
* thresholds to count the digits. The approach is straightforward and relies on
* sequential comparison against powers of 10.
*
* This function assumes `n` is a size_t, which is typically a 64-bit
*       unsigned integer on modern systems, but the function currently handles
*       values up to 10 digits, covering up to 9999999999.
*
* @param n The non-negative integer whose decimal places are to be counted.
* @return The number of decimal places in `n`.
*/
int WSDecimalPlaces (size_t n)
{
	if (n < 10) return 1;
	if (n < 100) return 2;
	if (n < 1000) return 3;
	if (n < 10000) return 4;
	if (n < 100000) return 5;
	if (n < 1000000) return 6;
	if (n < 10000000) return 7;
	if (n < 100000000) return 8;
	if (n < 1000000000) return 9;
	return 10;
}
/**
* Executes a test suite and reports results, including assertions and test
* cases passed/failed.
*
* This function runs a given test suite by invoking the provided `lpRunFunc`
* function, which initializes a `WSTESTSUITE` structure. It iterates through
* all test groups and cases, executing each one and tracking the number of
* passed and failed assertions and test cases. After running all tests, it
* outputs a summary report including the total number of test cases, the
* number passed/failed, and the total number of assertions made along with
* the number passed/failed.
*
* Test cases and groups are dynamically allocated and freed after execution
* to manage memory efficiently during the test run.
*
* The function constructs an output console `WSCONSOLE` for reporting and
* adjusts console colors based on test outcomes (cyan for all tests passed,
* red for any failures) to enhance readability of the test results.
*
* The detailed report format includes padding to align numerical values for
* a tidy and consistent presentation in the console output.
*
* This function also demonstrates the proper cleanup of dynamically allocated
* test structures (`WSTESTGROUP` and `WSTESTCASE`) to prevent memory leaks.
*
* Users should ensure that the `lpRunFunc` properly initializes the test
* suite structure (`WSTESTSUITE`) with relevant test groups and cases before
* invocation.
*
* The console color is reset to its default after reporting to avoid
* unintended color changes in subsequent console output.
*
* The dynamic calculation of padding spaces for report formatting ensures
* readability regardless of the number of tests or assertions.
*
* @param lpRunFunc Pointer to the function that initializes and runs the
*                  test suite.
*/
void WSTestSuiteRun(void (*lpRunFunc)(WSTESTSUITE* runner))
{
	size_t assertionsPlaces;
	size_t casesPlaces;
	size_t passedPlaces;
	size_t passedAssertionsPlaces;
	size_t maxPlaces;
	size_t assertionsFailedPlaces;
	size_t casesFailedPlaces;
	size_t maxFailedPlaces;
	size_t passed = 0;
	size_t total = 0;
	size_t passedAssertions = 0;
	size_t totalAssertions = 0;
	WSTESTSUITE tests;
	WSTESTGROUP* curgroup;
	WSTESTGROUP* oldgroup;
	WSTESTCASE* curcase;
	WSTESTCASE* oldcase;
	WSCONSOLE conout;
	(*lpRunFunc)(&tests);

	WSConsole_ConstructOutput(&conout);
	WSConsole_Write(&conout, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
		"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
		"Test Suite ");
	WSConsole_Write(&conout, tests.name);
	WSConsole_Write(&conout, "\n");

	for (curgroup = tests.groups; curgroup != NULL;)
	{
		size_t passedingroup = 0;
		size_t numcases = 0;
		for (curcase = curgroup->cases; curcase != NULL;)
		{
			curcase->assertions = curcase->failedAssertions = 0;
			curcase->printedHeader = 0;
			(*curcase->lpfnCaseProc)(curcase);
			if(curcase->failedAssertions == 0)
				++passedingroup;
			++numcases;
			totalAssertions += curcase->assertions;
			passedAssertions +=
				curcase->assertions - curcase->failedAssertions;
			oldcase = curcase;
			curcase = curcase->next;
			WSFREE(oldcase);
		}
		total += numcases;
		passed += passedingroup;
		oldgroup = curgroup;
		curgroup = curgroup->next;
		WSFREE(oldgroup);
	}

	WSConsole_Write(&conout, "==========================================="
		"====================================\n");
	assertionsPlaces = WSDecimalPlaces(totalAssertions);
	casesPlaces = WSDecimalPlaces(total);
	passedPlaces = WSDecimalPlaces(passed);
	passedAssertionsPlaces = WSDecimalPlaces(passedAssertions);
	if(assertionsPlaces > casesPlaces)
		maxPlaces = assertionsPlaces;
	else
		maxPlaces = casesPlaces;
	assertionsFailedPlaces =
		WSDecimalPlaces(totalAssertions - passedAssertions);
	casesFailedPlaces = WSDecimalPlaces(total - passed);
	if(assertionsFailedPlaces > casesFailedPlaces)
		maxFailedPlaces = assertionsFailedPlaces;
	else
		maxFailedPlaces = casesFailedPlaces;
	/* "test cases: %%%dd | %%%dd passed | %%%dd failed\n" */
	if (passed == total) {
		WSConsoleSetColorCyan();
	} else {
		WSConsoleSetColorRed();
	}
	WSConsole_Write(&conout, "test cases: ");
	while(casesPlaces < maxPlaces)
	{
		WSConsole_Write(&conout, " ");
		++casesPlaces;
	}
	WSConsole_WriteUInt(&conout, total);
	WSConsole_Write(&conout, " | ");
	while(passedPlaces < maxPlaces)
	{
		WSConsole_Write(&conout, " ");
		++passedPlaces;
	}
	WSConsole_WriteUInt(&conout, passed);
	WSConsole_Write(&conout, " passed | ");
	while(casesFailedPlaces < maxFailedPlaces)
	{
		WSConsole_Write(&conout, " ");
		++casesFailedPlaces;
	}
	WSConsole_WriteUInt(&conout, total - passed);
	WSConsole_Write(&conout, " failed\n");
	/* "assertions: %%%dd | %%%dd passed | %%%dd failed\n" */
	WSConsole_Write(&conout, "assertions: ");
	while(assertionsPlaces < maxPlaces)
	{
		WSConsole_Write(&conout, " ");
		++assertionsPlaces;
	}
	WSConsole_WriteUInt(&conout, totalAssertions);
	WSConsole_Write(&conout, " | ");
	while(passedAssertionsPlaces < maxPlaces)
	{
		WSConsole_Write(&conout, " ");
		++passedAssertionsPlaces;
	}
	WSConsole_WriteUInt(&conout, passedAssertions);
	WSConsole_Write(&conout, " passed | ");
	while(assertionsFailedPlaces < maxFailedPlaces)
	{
		WSConsole_Write(&conout, " ");
		++assertionsFailedPlaces;
	}
	WSConsole_WriteUInt(&conout, totalAssertions - passedAssertions);
	WSConsole_Write(&conout, " failed\n");

	WSConsole_Destruct(&conout);
	WSConsoleSetColorReset();
}
#if !defined(WIKISERVER_VERSION)
int WSConsole_ConstructOutput(WSCONSOLE* pThis) { return 1; }
int WSConsole_Write(WSCONSOLE* pThis, const char* buf)
{ return printf("%s", buf); }
int WSConsole_WriteUInt(WSCONSOLE* pThis, size_t ui)
{ return printf("%zu", ui); }
void WSConsole_Destruct(WSCONSOLE* pThis) {}

#if defined(_WIN32)
	#ifndef WINAPI
		void* __stdcall GetStdHandle(unsigned long);
		int __stdcall SetConsoleTextAttribute(
			void* hConsoleOutput, unsigned short wAttributes);
		#define WSTEST_STD_OUTPUT_HANDLE ((unsigned long)-11)
	#endif

	void WSConsoleSetColorBlue()
	{
		SetConsoleTextAttribute(GetStdHandle(WSTEST_STD_OUTPUT_HANDLE), 1);
	}

	void WSConsoleSetColorGreen()
	{
		SetConsoleTextAttribute(GetStdHandle(WSTEST_STD_OUTPUT_HANDLE), 2);
	}

	void WSConsoleSetColorRed()
	{
		SetConsoleTextAttribute(GetStdHandle(WSTEST_STD_OUTPUT_HANDLE), 4);
	}

	void WSConsoleSetColorBold()
	{
		SetConsoleTextAttribute(GetStdHandle(WSTEST_STD_OUTPUT_HANDLE), 8);
	}

	void WSConsoleSetColorReset()
	{
		SetConsoleTextAttribute(GetStdHandle(WSTEST_STD_OUTPUT_HANDLE), 1|2|4);
	}

	void WSConsoleSetColorYellow()
	{
		SetConsoleTextAttribute(GetStdHandle(WSTEST_STD_OUTPUT_HANDLE), 2|4);
	}

	void WSConsoleSetColorCyan()
	{
		SetConsoleTextAttribute(GetStdHandle(WSTEST_STD_OUTPUT_HANDLE), 1|2);
	}

	void WSConsoleSetColorMagenta()
	{
		SetConsoleTextAttribute(GetStdHandle(WSTEST_STD_OUTPUT_HANDLE), 1|4);
	}

	void WSConsoleSetColorWhite()
	{
		SetConsoleTextAttribute(GetStdHandle(WSTEST_STD_OUTPUT_HANDLE), 1|2|4|8);
	}
#else /* non-WIN32 */

#define WSConsoleColorChange(x) \
	printf("%s", (x));

void WSConsoleSetColorRed()
{
	WSConsoleColorChange("\x1B[31m");
}

void WSConsoleSetColorGreen()
{
	WSConsoleColorChange("\x1B[32m");
}

void WSConsoleSetColorBlue()
{
	WSConsoleColorChange("\x1B[34m");
}

void WSConsoleSetColorYellow()
{
	WSConsoleColorChange("\x1B[33m");
}

void WSConsoleSetColorCyan()
{
	WSConsoleColorChange("\x1B[36m");
}

void WSConsoleSetColorMagenta()
{
	WSConsoleColorChange("\x1B[35m");
}

void WSConsoleSetColorReset()
{
	WSConsoleColorChange("\x1B[0m");
}

void WSConsoleSetColorWhite()
{
	WSConsoleColorChange("\x1B[34m");
}

void WSConsoleSetColorBold()
{
	WSConsoleColorChange("\x1B[1m");
}
#endif /* WIN32 */
#endif /* WIKISERVER_VERSION */
#ifdef __cplusplus
}
#endif
#endif /* WSTEST_IMPLEMENTATION */
