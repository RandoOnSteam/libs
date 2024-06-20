/**
*	@file audio.h
*
*	#     #  #####   #####  #     # ###
*	#  #  # #     # #     # #     #  #
*	#  #  # #       #       #     #  #
*	#  #  #  #####  #  #### #     #  #
*	#  #  #       # #     # #     #  #
*	#  #  # #     # #     # #     #  #
*	 ## ##   #####   #####   #####  ###
*
*	Experimental basic GUI/main() framework
*
*	budavariam.github.io/asciiart-text/multi
*	Big/Slant/Banner
*
*	@todo The menu event handler API is bad. There are a lot of issues:
*
*	- Win32 uses messages with a switch
*
*	- KDE uses
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
#ifndef __GUI_H__
#define __GUI_H__
#if !defined(WIKISERVER_VERSION) || defined(TRYGUIH)
/*#############################################################################
							Type Declarations
#############################################################################*/
#if !defined(WSBOOL_DEFINED)
	typedef int wsbool;
	#if !defined(TRUE)
		#define TRUE 1
		#define FALSE 0
	#endif
	#define WSBOOL_DEFINED
#endif
#ifndef wsfloat /* CoreFoundation equiv */
	#if defined(__LP64__) && __LP64__
		#define wsfloat double /* DBL_MIN/MAX */
	#else
		#define wsfloat float /* FLT_MIN/MAX */
	#endif
#endif /* wsfloat */
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
#if !defined(WIKISERVER_VERSION)
	#if defined(__amigaos__) || defined(__AMIGA__) || defined(__AMIGA__) \
		|| defined(WSAMIGAGCCOPT)
		#define WSAMIGA
	#endif
	#if (!defined(__MACH__) || defined(WSMACCARBONSYSTEM)) && \
		(defined(__SC__) || defined(__MRC__) \
		|| (defined(__MACH__) && !defined(__i386__) && !defined(__INTEL__)) \
		|| (defined(__MWERKS__) && defined(macintosh) && !defined(__INTEL__)) \
		|| (defined(__GNUC__) \
			&& (defined(__m68k__) || defined(__ppc__)) && !defined(__unix__))) \
			&& !defined(WSAMIGA)
		#define WSMACCLASSICORCARBON
		#if (defined(TARGET_API_MAC_CARBON) && TARGET_API_MAC_CARBON) \
		|| (defined(TARGET_CARBON) && TARGET_CARBON) || defined(__MACH__)
			#define WSMACCARBON
		#else
			#define WSMACCLASSIC
		#endif
		#define WSMAC
	#elif defined(__MACH__)
		#define WSMAC
		#if defined(__has_builtin) && __has_builtin(__is_target_arch) \
			&& __has_builtin(__is_target_vendor) \
			&& __has_builtin(__is_target_os) \
			&& __has_builtin(__is_target_environment) \
			&& (__is_target_arch(x86_64) \
				&& __is_target_vendor(apple) && __is_target_os(ios)) \
				|| (__is_target_arch(x86_64) && __is_target_vendor(apple) \
					&& __is_target_os(ios) \
					&& __is_target_environment(simulator))
			#define WSMACUIKIT
		#else
			#define WSMACCOCOA
		#endif
	#endif
#endif
#if !defined(loword)
	#define loword(l) ((uint16_t)((size_t)(l) & 0xffff))
	#define hiword(l) ((uint16_t)((size_t)(l) >> 16))
#endif
#ifdef __cplusplus
	extern "C" {
#endif
/*-----------------------------------------------------------------------------
								DLL
-----------------------------------------------------------------------------*/
#if !defined(WSDLL_IsLoaded)
	typedef struct WSDLL
	{
	#if defined(_WIN32)
		struct HINSTANCE__* mHandle;
	#else
		void* mHandle;
	#endif
	} WSDLL;
	#define LPWSDLLPROC void*
	int WSDLL_Create(WSDLL* pThis, const char* libraryname);
	#define WSDLL_IsLoaded(pThis) ((pThis)->mHandle != NULL)
	LPWSDLLPROC WSDLL_GetProc(const WSDLL* pThis, const char* procname);
	void WSDLL_Destroy(WSDLL* pThis);
#endif
#if defined(_WIN32) && !defined(WSDLL_CreateKernel32)
	/*  Get the base module (kernel32/user32.dll) for passing to GetProc */
	struct HINSTANCE__* WSWinGetFunctionBase(void* func);
	#if !defined(WSMSIXOPT) && (1 || (defined(_MSC_VER) \
					&& _MSC_VER <= 1400 && defined(_WIN64)))
		#define WSWinGetExeHMODULE() \
			(GetModuleHandleA(NULL))
		#define WSDLL_CreateKernel32(pThis) \
			((pThis)->mHandle = GetModuleHandleA("kernel32.dll"))
		#define WSDLL_DestroyKernel32(pThis)
		#define WSDLL_CreateUser32(pThis) \
			((pThis)->mHandle = GetModuleHandleA("user32.dll"))
		#define WSDLL_DestroyUser32(pThis)
	#else
		#define WSWinGetExeHMODULE() \
			(HMODULE)(WSWinGetFunctionBase((void*)WSWinGetFunctionBase))
		#define WSDLL_CreateKernel32(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)VirtualQuery))
		#define WSDLL_DestroyKernel32(pThis)
		#define WSDLL_CreateUser32(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)CreateMenu))
		#define WSDLL_DestroyUser32(pThis)
	#endif
	#if defined(WSMSIXOPT)
		#define WSDLL_CreateMswsock(pThis) \
				((pThis)->mHandle = WSWinGetFunctionBase((void*)AcceptEx))
		#define WSDLL_DestroyMswsock(pThis)
		#define WSDLL_CreateWs2_32(pThis) \
				((pThis)->mHandle = WSWinGetFunctionBase((void*)WSAEventSelect))
		#define WSDLL_DestroyWs2_32(pThis)
		#define WSDLL_CreateShell32(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)ShellExecuteA))
		#define WSDLL_DestroyShell32(pThis)
		#define WSDLL_CreateGdi32(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)GetDeviceCaps))
		#define WSDLL_DestroyGdi32(pThis)
		#define WSDLL_CreateAdvapi32(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)RegCloseKey))
		#define WSDLL_DestroyAdvapi32(pThis)
		#define WSDLL_CreateNtdll(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)RtlGetVersion))
		#define WSDLL_DestroyNtdll(pThis)
		#define WSDLL_CreateUxtheme(pThis) \
			((pThis)->mHandle = \
				WSWinGetFunctionBase((void*)DrawThemeBackground))
		#define WSDLL_DestroyUxtheme(pThis)
		#define WSDLL_CreateOle32(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)CoInitialize))
		#define WSDLL_DestroyOle32(pThis)
		#define WSDLL_CreateOleaut32(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)SysFreeString))
		#define WSDLL_DestroyOleaut32(pThis)
		#define WSDLL_CreateComctl32(pThis) \
			((pThis)->mHandle = \
				WSWinGetFunctionBase((void*)InitCommonControlsEx))
		#define WSDLL_DestroyComctl32(pThis)
		#define WSDLL_CreateSecur32(pThis) \
			((pThis)->mHandle = \
				WSWinGetFunctionBase((void*)AcceptSecurityContext))
		#define WSDLL_DestroySecur32(pThis)
		#define WSDLL_CreateCrypt32(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)CertCloseStore))
		#define WSDLL_DestroyCrypt32(pThis)
		#define WSDLL_CreateCryptui(pThis) \
			((pThis)->mHandle = \
				WSWinGetFunctionBase((void*)CryptUIDlgSelectCertificateA))
		#define WSDLL_DestroyCryptui(pThis)
		#define WSDLL_CreateIphlpapi(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)GetNetworkParams))
		#define WSDLL_DestroyIphlpapi(pThis)
		#define WSDLL_CreateDsound(pThis) \
			((pThis)->mHandle = WSWinGetFunctionBase((void*)DirectSoundCreate))
		#define WSDLL_DestroyDsound(pThis)
	#else
		#define WSDLL_CreateMswsock(pThis) \
				WSDLL_Create(pThis, "mswsock.dll")
		#define WSDLL_DestroyMswsock(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateWs2_32(pThis) \
					WSDLL_Create(pThis, "ws2_32.dll")
		#define WSDLL_DestroyWs2_32(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateShell32(pThis) \
					WSDLL_Create(pThis, "shell32.dll")
		#define WSDLL_DestroyShell32(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateGdi32(pThis) \
					WSDLL_Create(pThis, "gdi32.dll")
		#define WSDLL_DestroyGdi32(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateAdvapi32(pThis) \
							WSDLL_Create(pThis, "advapi32.dll")
		#define WSDLL_DestroyAdvapi32(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateNtdll(pThis) \
					((pThis)->mHandle = GetModuleHandleA("ntdll.dll"))
		#define WSDLL_DestroyNtdll(pThis)
		#define WSDLL_CreateUxtheme(pThis) \
									WSDLL_Create(pThis, "uxtheme.dll")
		#define WSDLL_DestroyUxtheme(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateOle32(pThis) \
									WSDLL_Create(pThis, "ole32.dll")
		#define WSDLL_DestroyOle32(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateOleaut32(pThis) \
									WSDLL_Create(pThis, "oleaut32.dll")
		#define WSDLL_DestroyOleaut32(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateComctl32(pThis) \
									WSDLL_Create(pThis, "comctl32.dll")
		#define WSDLL_DestroyComctl32(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateSecur32(pThis) \
									WSDLL_Create(pThis, "secur32.dll")
		#define WSDLL_DestroySecur32(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateCrypt32(pThis) \
									WSDLL_Create(pThis, "crypt32.dll")
		#define WSDLL_DestroyCrypt32(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateCryptui(pThis) \
									WSDLL_Create(pThis, "cryptui32.dll")
		#define WSDLL_DestroyCryptui(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateIphlpapi(pThis) \
									WSDLL_Create(pThis, "iphlpapi.dll")
		#define WSDLL_DestroyIphlpapi(pThis) WSDLL_Destroy(pThis)
		#define WSDLL_CreateDsound(pThis) WSDLL_Create(pThis, "dsound.dll")
		#define WSDLL_DestroyDsound(pThis) WSDLL_Destroy(pThis)
	#endif
#endif
/*-----------------------------------------------------------------------------
									WSEVENTLOOP
-----------------------------------------------------------------------------*/
#if defined(_WIN32)
	typedef struct WSEVENTLOOP
	{
		void*        hwnd;
		unsigned int        message;
		size_t      wParam;
		size_t      lParam;
		uint32_t       time;
		struct { long x; long y; } pt;
	#ifdef _MAC
		uint32_t       lPrivate;
	#endif
	} WSEVENTLOOP;
#elif defined(WSMACCLASSICORCARBON)
	typedef struct WSEVENTRECORD
	{
	  uint16_t           what;
	  unsigned long       message;
	  uint32_t              when;
	  struct { short v; short h; } where;
	  uint16_t      modifiers;
	} WSEVENTRECORD;
	typedef struct WSEVENTLOOP
	{
		WSEVENTRECORD mEr;
		#if !defined(WSMACCARBON)
			wsbool mHasWaitNextEvent;
			struct MacRegion** mMouseRgn;
		#else
			wsbool mIsQuitting;
			struct OpaqueRgnHandle* mMouseRgn;
		#endif
	} WSEVENTLOOP;
#elif defined(WSMACCARBON) && defined(__MACH__)
	typedef struct WSEVENTLOOP
	{
		struct OpaqueEventRef* mEvent;
		struct OpaqueEventTargetRef* mDispatcher;
	} WSEVENTLOOP;
#else
	typedef struct WSEVENTLOOP_* WSEVENTLOOP;
#endif
wsbool WSEventLoop_Create(WSEVENTLOOP* pThis);
void WSEventLoop_Destroy(WSEVENTLOOP* pThis);
wsbool WSEventLoop_RunSingle(WSEVENTLOOP* pThis);
void WSEventLoop_Run(WSEVENTLOOP* pThis);
wsbool WSEventLoop_IsQuitMessage(WSEVENTLOOP* pThis);

/*-----------------------------------------------------------------------------
									GUI Types
-----------------------------------------------------------------------------*/

typedef struct WSWINDOW_* WSWINDOW;
typedef struct WSSCROLLEDWINDOW_* WSSCROLLEDWINDOW;
typedef struct WSDC_* WSDC;
typedef struct WSICON_* WSICON;
typedef struct WSMENUACTIONS_* WSMENUACTIONS;
typedef struct WSMENU_* WSMENU;
typedef struct WSMENUBAR_* WSMENUBAR;
typedef struct WSMENUSECTION_* WSMENUSECTION;

/*-----------------------------------------------------------------------------
									WSAPP
-----------------------------------------------------------------------------*/

typedef struct WSAPP
{
#if defined(_WIN32)
	struct HINSTANCE__* mhInstance;
#endif
#if !defined(NOCRUNTIME)
	#if defined(_WIN32) && (!defined(_CONSOLE))
		char* mArgv;
	#else
		char** mArgv;
		int mArgc;
	#endif
#endif
#if !defined(_WIN32) && !defined(__MACH__) && !defined(WSMACCLASSICORCARBON)
	#if defined(WSKDEOPT)
		wsclass WSQApplication* mApplication;
	#elif defined(WSGTK4OPT)
		struct _GtkApplication* mApplication;
	#else
		wsbool mHasGUI;
	#endif
#endif
#if defined(WSMACCLASSICORCARBON)
	void* mCarbonEventUPP;
	void* mCarbonEventHandler;
	void* mSetEventParameterProc;
	void* mGetEventParameterProc;
	void* mGetEventClassProc;
	void* mGetEventKindProc;
	void* mRemoveEventHandlerProc;
	void* mDisposeEventHandlerUPPProc;
#elif defined (WSMACCOCOA)
	void* mDelegate;
#endif
} WSAPP;

#if defined(NOCRUNTIME)
	#define WSAPP_BASIC_MAIN_ARGS struct HINSTANCE__* hInstance
	#define WSAPP_BASIC_MAIN_FILLAPP \
		((WSAPP*)&app)->mhInstance = hInstance;
#elif defined(_WIN32)
	#if !defined(_CONSOLE)
		#define WSAPP_BASIC_MAIN_ARGS struct HINSTANCE__* hInstance, char* argv
		#define WSAPP_BASIC_MAIN_FILLAPP \
			((WSAPP*)&app)->mhInstance = hInstance; \
			((WSAPP*)&app)->mArgv = argv;
	#else
		#define WSAPP_BASIC_MAIN_ARGS \
			struct HINSTANCE__* hInstance, int argc, char** argv
		#define WSAPP_BASIC_MAIN_FILLAPP \
			((WSAPP*)&app)->mhInstance = hInstance; \
			((WSAPP*)&app)->mArgc = argc; ((WSAPP*)&app)->mArgv = argv;
	#endif
#else
	#define WSAPP_BASIC_MAIN_ARGS int argc, char** argv
	#define WSAPP_BASIC_MAIN_FILLAPP \
		((WSAPP*)&app)->mArgc = argc; ((WSAPP*)&app)->mArgv = argv;
#endif

WSAPP* WSAppCreate(void);
wsbool WSApp_OnInitGUI(WSAPP* pThis); /* return == do idle/eventloop */
wsbool WSApp_OnIdle(WSAPP* pThis); /* return == continue eventloop */
wsbool WSApp_OnEventLoopBegin(WSAPP* pThis); /* return == continue eventloop */
wsbool WSApp_OnEventLoopEnd(WSAPP* pThis); /* return == continue eventloop */
int WSApp_OnExitGUI(WSAPP* pThis); /* return == exit code */
wsbool WSApp_CreateGUI(WSAPP* pThis);
void WSApp_Destroy(WSAPP* pThis);
void WSAppDestroy(WSAPP* pThis);
int WSApp_BasicMain(WSAPP_BASIC_MAIN_ARGS);

#if defined(_WIN32) && (defined(NOCRUNTIME) || !defined(_CONSOLE))
	#define WSApp_GetArgc(pThis) (1)
	#define WSApp_GetArgv(pThis) (&(pThis)->mArgv)
	#if !defined(NOCRUNTIME) /* WinMain lpCmdLine does not contain exe path */
		#define WSAPP_HASARGV
	#else
		void WSAppWinSplitCmdLine(WSMUTSTR* firstArg,
			const char** argv, int* argc);
	#endif
#else
	#define WSApp_GetArgc(pThis) ((pThis)->mArgc)
	#define WSApp_GetArgv(pThis) ((pThis)->mArgv)
	#define WSAPP_HASSPLITARGV
	#define WSAPP_HASARGV
#endif
#if !defined(_WIN32) && !defined(__MACH__) && !defined(WSMACCLASSICORCARBON)
	#if defined(WSGTK4OPT) || defined(WSKDEOPT)
		#define WSApp_HasGUI(pThis) ((pThis)->mApplication != NULL)
	#else
		#define WSApp_HasGUI(pThis) ((pThis)->mHasGUI)
	#endif
#else
	#define WSApp_HasGUI(pThis) (TRUE)
#endif

#if 1
#define WSAPP_BASIC_MAIN(appname) \
int WSApp_BasicMain(WSAPP_BASIC_MAIN_ARGS) \
{ \
	appname app; WSEVENTLOOP ev; \
	WSAPP_BASIC_MAIN_FILLAPP \
	if(WSApp_OnInitGUI((WSAPP*)&app)) \
	{ \
		WSEventLoop_Create(&ev); \
		do \
		{ \
			if(!WSApp_OnEventLoopBegin((WSAPP*)&app)) \
				break; \
			if(WSApp_HasGUI((WSAPP*)&app)) \
				do \
				{ \
					while(WSEventLoop_RunSingle(&ev)){} \
				} \
				while(!WSEventLoop_IsQuitMessage(&ev) \
					  && WSApp_OnIdle((WSAPP*)&app)); \
			else \
				while(WSApp_OnIdle((WSAPP*)&app)){} \
		} while(WSApp_OnEventLoopEnd((WSAPP*)&app)); \
		WSEventLoop_Destroy(&ev); \
	} \
	return WSApp_OnExitGUI((WSAPP*)&app); \
}
#else
int WSApp_BasicMain_(WSAPP* pThis, WSEVENTLOOP* ev);
#define WSAPP_BASIC_MAIN(appname) \
int WSApp_BasicMain(WSAPP_BASIC_MAIN_ARGS) \
{ \
	appname app; WSEVENTLOOP ev; \
	WSAPP_BASIC_MAIN_FILLAPP \
	return WSApp_BasicMain_((WSAPP*)&app, &ev); \
}
#endif

#if defined(__cplusplus)
	#define IMPLEMENT_WSAPP(name, init, idle, lbeg, lend, exit) \
	extern "C" { \
		WSAPP_BASIC_MAIN(name) \
		wsbool WSApp_OnInitGUI(WSAPP* pThis) { return init((name*)pThis); } \
		wsbool WSApp_OnIdle(WSAPP* pThis) { return idle((name*)pThis); } \
		wsbool WSApp_OnEventLoopBegin(WSAPP* pThis) \
		{ return lbeg((name*)pThis); } \
		wsbool WSApp_OnEventLoopEnd(WSAPP* pThis) \
		{ return lend((name*)pThis); } \
		int WSApp_OnExitGUI(WSAPP* pThis) { return exit((name*)pThis); } \
	}
#else
	#define IMPLEMENT_WSAPP(name, init, idle, lbeg, lend, exit) \
	WSAPP_BASIC_MAIN(name) \
	wsbool WSApp_OnInitGUI(WSAPP* pThis) { return init((name*)pThis); } \
	wsbool WSApp_OnIdle(WSAPP* pThis) { return idle((name*)pThis); } \
	wsbool WSApp_OnEventLoopBegin(WSAPP* pThis) \
	{ return lbeg((name*)pThis); } \
	wsbool WSApp_OnEventLoopEnd(WSAPP* pThis) \
	{ return lend((name*)pThis); } \
	int WSApp_OnExitGUI(WSAPP* pThis) { return exit((name*)pThis); }
#endif
#define IMPLEMENT_WSAPP_CONSOLE(name, console) \
wsbool WSApp_Console_OnInitGUI(name* pThis) { return FALSE; } \
wsbool WSApp_Console_OnIdle(name* pThis) { return FALSE; } \
wsbool WSApp_Console_OnEventLoopBegin(name* pThis) { return FALSE; } \
wsbool WSApp_Console_OnEventLoopEnd(name* pThis) { return FALSE; } \
IMPLEMENT_WSAPP(name, WSApp_Console_OnInitGUI, WSApp_Console_OnIdle, \
				WSApp_Console_OnEventLoopBegin, WSApp_Console_OnEventLoopEnd, \
				console)


/*-----------------------------------------------------------------------------
								GUI
-----------------------------------------------------------------------------*/
#define WM_WSNOTIFYICON 0x0400 + 1000
#define WM_WSMENU 0x400 + 2000
#define WM_WSCOMMAND 0x0111
#define WM_WSMOVE 0x0003
#define WM_WSSIZE 0x0005
#define WM_WSDESTROY 0x0002
#define WM_WSCLOSE 0x0010
#define WM_WSPAINT 0x000F /* 15 */
#define WM_WSKEYDOWN 0x0100 /* 256 */
#define WM_WSRBUTTONDOWN 0x0204
#define WM_WSLBUTTONDOWN 0x0202
#define WM_WSMOUSEWHEEL 0x020A
#define WM_WSQUIT 109
#define WM_WSACTIVATE	0x0006
#define WM_WSSETFOCUS	0x0007
#define WM_WSKILLFOCUS	0x0008
#define WM_WSHANDLECONTENTEVENT 100
#define WM_WSHANDLEGROWEVENT 101
#define WM_WSHANDLEZOOMEVENT 102
#define WM_WSHANDLEIDLEEVENT 104
#define WM_WSHANDLEUPDATEEVENT 105
#define WM_WSHANDLEACTIVATEEVENT 106
#define WM_WSHANDLEDEACTIVATEEVENT 107
#define WM_WSHANDLEIBEAMEVENT 108
#define WM_WSHANDLEMOUSEWHEELEVENT 109

/*=============================================================================
		 _       ___      ________        ______                 __
		| |     / (_)___ |__  /__ \      / ____/   _____  ____  / /______
		| | /| / / / __ \ /_ <__/ /     / __/ | | / / _ \/ __ \/ __/ ___/
		| |/ |/ / / / / /__/ / __/     / /___ | |/ /  __/ / / / /_(__  )
		|__/|__/_/_/ /_/____/____/    /_____/ |___/\___/_/ /_/\__/____/
=============================================================================*/
#if defined(_WIN32)
/*
	Win32 GUI+structure definitions
*/
#if !defined(guipos_t)
	#define guipos_t long
	typedef struct WSRECT
	{
		long left;
		long top;
		long right;
		long bottom;
	} WSRECT;
#endif
typedef struct HWND__* WSHWND;

#if defined(_WIN64)
	#define longptr_t __int64
	typedef unsigned __int64 uintptr_t;
#else /* MWERKS uintptr_t is unsigned long which conflicts with Win32 */
	#define longptr_t long
	#if defined(__MWERKS__) || (defined(_MSC_VER) && _MSC_VER <= 1400)
		#define uintptr_t unsigned int
	#endif
#endif
#define WSWINDOWMESSAGEARGSDECLARE \
	WSHWND hWnd, unsigned int msg, uintptr_t wParam, longptr_t lParam
#define WSWINDOWMESSAGEARGSPASS hWnd, msg, wParam, lParam
typedef longptr_t (__stdcall *LPWSWindowMessages)(WSWINDOWMESSAGEARGSDECLARE);
#if defined(UNICODE)
	#define WSWinDefWindowProc DefWindowProcW
#else
	#define WSWinDefWindowProc DefWindowProcA
#endif
void WSWindowProcessAccels(WSWINDOWMESSAGEARGSDECLARE);
__declspec(dllimport) longptr_t __stdcall WSWinDefWindowProc(
	WSWINDOWMESSAGEARGSDECLARE);
typedef struct WSEVENT
{
	WSHWND mhWnd;
	uintptr_t mwParam;
	longptr_t mlParam;
} WSEVENT;
typedef WSEVENT WSSIZEEVENT;
typedef WSEVENT WSPAINTEVENT;
typedef WSEVENT WSKEYEVENT;
typedef WSEVENT WSCLOSEEVENT;
typedef WSEVENT WSNOTIFYICONEVENT;
typedef WSEVENT WSMENUEVENT;
typedef WSEVENT WSDESTROYEVENT;
typedef WSEVENT WSMOUSEWHEELEVENT;
typedef WSEVENT WSIDLEEVENT;
typedef WSEVENT WSCONTENTEVENT;
typedef WSEVENT WSGROWEVENT;
typedef WSEVENT WSZOOMEVENT;
typedef WSEVENT WSACTIVATEEVENT;
typedef WSEVENT WSDEACTIVATEEVENT;
typedef WSEVENT WSIBEAMEVENT;

#define WSEVENT_CALLBACK(x) longptr_t (x)(WSWINDOWMESSAGEARGSDECLARE)
#define WSEVENT_HANDLED FALSE
#define WSEVENT_UNHANDLED TRUE
#define WSEvent_Construct(pThis) { (pThis)->mhWnd = hWnd; \
	(pThis)->mwParam = wParam; (pThis)->mlParam = lParam; }

#define WSSIZEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSSIZEEVENT_HANDLED WSEVENT_HANDLED
#define WSSIZEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSSizeEvent_Construct WSEvent_Construct

#define WSPAINTEVENT_CALLBACK WSEVENT_CALLBACK
#define WSPAINTEVENT_HANDLED WSEVENT_HANDLED
#define WSPAINTEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSPaintEvent_Construct WSEvent_Construct

#define WSKEYEVENT_CALLBACK WSEVENT_CALLBACK
#define WSKEYEVENT_HANDLED WSEVENT_HANDLED
#define WSKEYEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSKeyEvent_Construct WSEvent_Construct

#define WSCLOSEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSCLOSEEVENT_HANDLED WSEVENT_HANDLED
#define WSCLOSEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSCloseEvent_Construct WSEvent_Construct

#define WSNOTIFYICONEVENT_CALLBACK WSEVENT_CALLBACK
#define WSNOTIFYICONEVENT_HANDLED WSEVENT_HANDLED
#define WSNOTIFYICONEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSNotifyIconEvent_Construct WSEvent_Construct

#define WSMENUEVENT_CALLBACK WSEVENT_CALLBACK
#define WSMENUEVENT_HANDLED WSEVENT_HANDLED
#define WSMENUEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSMenuEvent_Construct WSEvent_Construct

#define WSDESTROYEVENT_CALLBACK WSEVENT_CALLBACK
#define WSDESTROYEVENT_HANDLED WSEVENT_HANDLED
#define WSDESTROYEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSDestroyEvent_Construct WSEvent_Construct

#define WSMOUSEWHEELEVENT_CALLBACK WSEVENT_CALLBACK
#define WSMOUSEWHEELEVENT_HANDLED WSEVENT_HANDLED
#define WSMOUSEWHEELEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSMouseWheelEvent_Construct WSEvent_Construct

#define WSCONTENTEVENT_CALLBACK WSEVENT_CALLBACK
#define WSCONTENTEVENT_HANDLED WSEVENT_HANDLED
#define WSCONTENTEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSContentEvent_Construct WSEvent_Construct

#define WSGROWEVENT_CALLBACK WSEVENT_CALLBACK
#define WSGROWEVENT_HANDLED WSEVENT_HANDLED
#define WSGROWEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSGrowEvent_Construct WSEvent_Construct

#define WSZOOMEVENT_CALLBACK WSEVENT_CALLBACK
#define WSZOOMEVENT_HANDLED WSEVENT_HANDLED
#define WSZOOMEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSZoomEvent_Construct WSEvent_Construct

#define WSIDLEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSIDLEEVENT_HANDLED WSEVENT_HANDLED
#define WSIDLEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSIdleEvent_Construct WSEvent_Construct

#define WSACTIVATEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSACTIVATEEVENT_HANDLED WSEVENT_HANDLED
#define WSACTIVATEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSActivateEvent_Construct WSEvent_Construct

#define WSDEACTIVATEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSDEACTIVATEEVENT_HANDLED WSEVENT_HANDLED
#define WSDEACTIVATEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSDeactivateEvent_Construct WSEvent_Construct

#define WSIBEAMEVENT_CALLBACK WSEVENT_CALLBACK
#define WSIBEAMEVENT_HANDLED WSEVENT_HANDLED
#define WSIBEAMEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSIBeamEvent_Construct WSEvent_Construct

/*
	Message map decls for Win32
*/
#define BEGIN_ALL_MENU_MESSAGES
#define BEGIN_MENU_MESSAGES(name) longptr_t name(WSWINDOWMESSAGEARGSDECLARE) \
{ \
	switch (loword(wParam)) \
	{
#define DECLARE_MENU_MESSAGE(command, proc) \
		case command: \
			return proc(WSWINDOWMESSAGEARGSPASS);
#define END_MENU_MESSAGES(name) \
	} \
	return WSEVENT_UNHANDLED; \
}
#define END_ALL_MENU_MESSAGES

#define BEGIN_ALL_WINDOW_MESSAGES
#define BEGIN_WINDOW_MESSAGES(name) longptr_t __stdcall name( \
	WSWINDOWMESSAGEARGSDECLARE) \
{ \
	if(0) {}
#define IMPLEMENT_MENU_WINDOW_MESSAGES(name) \
	else if(msg == WM_WSCOMMAND && hiword(wParam) == 0) \
		{ if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0; }
#define IMPLEMENT_SIZE_CHANGED_MESSAGE(name) \
	else if(msg == WM_WSSIZE) \
		{	if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0; }
#define IMPLEMENT_PAINT_MESSAGE(name) \
	else if(msg == WM_WSPAINT) \
		{ if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0; }
#define IMPLEMENT_KEYDOWN_MESSAGE(name) \
	else if(msg == WM_WSKEYDOWN) \
		{ if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0;  \
			WSWindowProcessAccels(WSWINDOWMESSAGEARGSPASS); \
		}
#define IMPLEMENT_CLOSE_MESSAGE(name) \
	else if(msg == WM_WSCLOSE) \
		{ if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0; }
#define IMPLEMENT_DESTROYED_MESSAGE(name) \
	else if(msg == WM_WSDESTROY) \
		{ if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0; }
#define IMPLEMENT_NOTIFYICON_MESSAGE(name, command) \
	else if(msg == command + WM_WSNOTIFYICON && lParam == WM_WSRBUTTONDOWN) \
		{ if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0; }
#define IMPLEMENT_HANDLEMOUSEWHEEL_MESSAGE(name) \
	else if(msg == WM_WSMOUSEWHEEL) \
		{ if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0; }
#define IMPLEMENT_HANDLEIDLE_MESSAGE(name)
#define IMPLEMENT_HANDLECONTENT_MESSAGE(name)
#define IMPLEMENT_HANDLEGROW_MESSAGE(name)
#define IMPLEMENT_HANDLEZOOM_MESSAGE(name)
#define IMPLEMENT_HANDLEACTIVATE_MESSAGE(name) \
	else if(msg == WM_WSACTIVATE && loword(wParam) != 0) \
		{ if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0; }
#define IMPLEMENT_HANDLEDEACTIVATE_MESSAGE(name) \
	else if(msg == WM_WSACTIVATE && loword(wParam) == 0) \
		{ if(!name(WSWINDOWMESSAGEARGSPASS))\
				return 0; }
#define IMPLEMENT_HANDLEUPDATE_MESSAGE(name)
#define IMPLEMENT_HANDLEIBEAM_MESSAGE(name)
#define IMPLEMENT_FALLBACK_MESSAGE(name) \
	else if(!name(WSWINDOWMESSAGEARGSPASS)) \
		return 0;
#define END_WINDOW_MESSAGES(name) \
	return WSWinDefWindowProc(WSWINDOWMESSAGEARGSPASS); \
}
#define END_ALL_WINDOW_MESSAGES

/*
	Win32 Key Codes
*/
#define WSKEY_LBUTTON           0x01 /* VK_LBUTTON */
#define WSKEY_RBUTTON           0x02 /* VK_RBUTTON */
#define WSKEY_CANCEL            0x03 /* VK_CANCEL */
#define WSKEY_MBUTTON           0x04 /* VK_MBUTTON */
#define WSKEY_XBUTTON1          0x05 /* VK_XBUTTON1 */
#define WSKEY_XBUTTON2          0x06 /* VK_XBUTTON2 */
#define WSKEY_BACK              0x08 /* VK_BACK */
#define WSKEY_TAB               0x09 /* VK_TAB */
#define WSKEY_CLEAR             0x0C /* VK_CLEAR */
#define WSKEY_RETURN            0x0D /* VK_RETURN */
#define WSKEY_SHIFT             0x10 /* VK_SHIFT */
#define WSKEY_CONTROL           0x11 /* VK_CONTROL */
#define WSKEY_MENU              0x12 /* VK_MENU */
#define WSKEY_PAUSE             0x13 /* VK_PAUSE */
#define WSKEY_CAPITAL           0x14 /* VK_CAPITAL */
#define WSKEY_KANA              0x15 /* VK_KANA */
#define WSKEY_HANGEUL           0x15 /* VK_HANGEUL */
#define WSKEY_HANGUL            0x15 /* VK_HANGUL */
#define WSKEY_JUNJA             0x17 /* VK_JUNJA */
#define WSKEY_FINAL             0x18 /* VK_FINAL */
#define WSKEY_HANJA             0x19 /* VK_HANJA */
#define WSKEY_KANJI             0x19 /* VK_KANJI */
#define WSKEY_ESCAPE            0x1B /* VK_ESCAPE */
#define WSKEY_CONVERT           0x1C /* VK_CONVERT */
#define WSKEY_NONCONVERT        0x1D /* VK_NONCONVERT */
#define WSKEY_ACCEPT            0x1E /* VK_ACCEPT */
#define WSKEY_MODECHANGE        0x1F /* VK_MODECHANGE */
#define WSKEY_SPACE             0x20 /* VK_SPACE */
#define WSKEY_PRIOR             0x21 /* VK_PRIOR */
#define WSKEY_NEXT              0x22 /* VK_NEXT */
#define WSKEY_END               0x23 /* VK_END */
#define WSKEY_HOME              0x24 /* VK_HOME */
#define WSKEY_LEFT              0x25 /* VK_LEFT */
#define WSKEY_UP                0x26 /* VK_UP */
#define WSKEY_RIGHT             0x27 /* VK_RIGHT */
#define WSKEY_DOWN              0x28 /* VK_DOWN */
#define WSKEY_SELECT            0x29 /* VK_SELECT */
#define WSKEY_PRINT             0x2A /* VK_PRINT */
#define WSKEY_EXECUTE           0x2B /* VK_EXECUTE */
#define WSKEY_SNAPSHOT          0x2C /* VK_SNAPSHOT */
#define WSKEY_INSERT            0x2D /* VK_INSERT */
#define WSKEY_DELETE            0x2E /* VK_DELETE */
#define WSKEY_HELP              0x2F /* VK_HELP */
#define WSKEY_0                 0x30 /* VK_0 */
#define WSKEY_1                 0x31 /* VK_1 */
#define WSKEY_2                 0x32 /* VK_2 */
#define WSKEY_3                 0x33 /* VK_3 */
#define WSKEY_4                 0x34 /* VK_4 */
#define WSKEY_5                 0x35 /* VK_5 */
#define WSKEY_6                 0x36 /* VK_6 */
#define WSKEY_7                 0x37 /* VK_7 */
#define WSKEY_8                 0x38 /* VK_8 */
#define WSKEY_9                 0x39 /* VK_9 */
#define WSKEY_A                 0x41 /* VK_A */
#define WSKEY_B                 0x42 /* VK_B */
#define WSKEY_C                 0x43 /* VK_C */
#define WSKEY_D                 0x44 /* VK_D */
#define WSKEY_E                 0x45 /* VK_E */
#define WSKEY_F                 0x46 /* VK_F */
#define WSKEY_G                 0x47 /* VK_G */
#define WSKEY_H                 0x48 /* VK_H */
#define WSKEY_I                 0x49 /* VK_I */
#define WSKEY_J                 0x4A /* VK_J */
#define WSKEY_K                 0x4B /* VK_K */
#define WSKEY_L                 0x4C /* VK_L */
#define WSKEY_M                 0x4D /* VK_M */
#define WSKEY_N                 0x4E /* VK_N */
#define WSKEY_O                 0x4F /* VK_O */
#define WSKEY_P                 0x50 /* VK_P */
#define WSKEY_Q                 0x51 /* VK_Q */
#define WSKEY_R                 0x52 /* VK_R */
#define WSKEY_S                 0x53 /* VK_S */
#define WSKEY_T                 0x54 /* VK_T */
#define WSKEY_U                 0x55 /* VK_U */
#define WSKEY_V                 0x56 /* VK_V */
#define WSKEY_W                 0x57 /* VK_W */
#define WSKEY_X                 0x58 /* VK_X */
#define WSKEY_Y                 0x59 /* VK_Y */
#define WSKEY_Z                 0x5A /* VK_Z */
#define WSKEY_LWIN              0x5B /* VK_LWIN */
#define WSKEY_RWIN              0x5C /* VK_RWIN */
#define WSKEY_APPS              0x5D /* VK_APPS */
#define WSKEY_SLEEP             0x5F /* VK_SLEEP */
#define WSKEY_NUMPAD0           0x60 /* VK_NUMPAD0 */
#define WSKEY_NUMPAD1           0x61 /* VK_NUMPAD1 */
#define WSKEY_NUMPAD2           0x62 /* VK_NUMPAD2 */
#define WSKEY_NUMPAD3           0x63 /* VK_NUMPAD3 */
#define WSKEY_NUMPAD4           0x64 /* VK_NUMPAD4 */
#define WSKEY_NUMPAD5           0x65 /* VK_NUMPAD5 */
#define WSKEY_NUMPAD6           0x66 /* VK_NUMPAD6 */
#define WSKEY_NUMPAD7           0x67 /* VK_NUMPAD7 */
#define WSKEY_NUMPAD8           0x68 /* VK_NUMPAD8 */
#define WSKEY_NUMPAD9           0x69 /* VK_NUMPAD9 */
#define WSKEY_MULTIPLY          0x6A /* VK_MULTIPLY */
#define WSKEY_ADD               0x6B /* VK_ADD */
#define WSKEY_SEPARATOR         0x6C /* VK_SEPARATOR */
#define WSKEY_SUBTRACT          0x6D /* VK_SUBTRACT */
#define WSKEY_DECIMAL           0x6E /* VK_DECIMAL */
#define WSKEY_DIVIDE            0x6F /* VK_DIVIDE */
#define WSKEY_F1                0x70 /* VK_F1 */
#define WSKEY_F2                0x71 /* VK_F2 */
#define WSKEY_F3                0x72 /* VK_F3 */
#define WSKEY_F4                0x73 /* VK_F4 */
#define WSKEY_F5                0x74 /* VK_F5 */
#define WSKEY_F6                0x75 /* VK_F6 */
#define WSKEY_F7                0x76 /* VK_F7 */
#define WSKEY_F8                0x77 /* VK_F8 */
#define WSKEY_F9                0x78 /* VK_F9 */
#define WSKEY_F10               0x79 /* VK_F10 */
#define WSKEY_F11               0x7A /* VK_F11 */
#define WSKEY_F12               0x7B /* VK_F12 */
#define WSKEY_F13               0x7C /* VK_F13 */
#define WSKEY_F14               0x7D /* VK_F14 */
#define WSKEY_F15               0x7E /* VK_F15 */
#define WSKEY_F16               0x7F /* VK_F16 */
#define WSKEY_F17               0x80 /* VK_F17 */
#define WSKEY_F18               0x81 /* VK_F18 */
#define WSKEY_F19               0x82 /* VK_F19 */
#define WSKEY_F20               0x83 /* VK_F20 */
#define WSKEY_F21               0x84 /* VK_F21 */
#define WSKEY_F22               0x85 /* VK_F22 */
#define WSKEY_F23               0x86 /* VK_F23 */
#define WSKEY_F24               0x87 /* VK_F24 */
#define WSKEY_NUMLOCK           0x90 /* VK_NUMLOCK */
#define WSKEY_SCROLL            0x91 /* VK_SCROLL */
#define WSKEY_OEM_NEC_EQUAL     0x92 /* VK_OEM_NEC_EQUAL */
#define WSKEY_OEM_FJ_JISHO      0x92 /* VK_OEM_FJ_JISHO */
#define WSKEY_OEM_FJ_MASSHOU    0x93 /* VK_OEM_FJ_MASSHOU */
#define WSKEY_OEM_FJ_TOUROKU    0x94 /* VK_OEM_FJ_TOUROKU */
#define WSKEY_OEM_FJ_LOYA       0x95 /* VK_OEM_FJ_LOYA */
#define WSKEY_OEM_FJ_ROYA       0x96 /* VK_OEM_FJ_ROYA */
#define WSKEY_LSHIFT            0xA0 /* VK_LSHIFT */
#define WSKEY_RSHIFT            0xA1 /* VK_RSHIFT */
#define WSKEY_LCONTROL          0xA2 /* VK_LCONTROL */
#define WSKEY_RCONTROL          0xA3 /* VK_RCONTROL */
#define WSKEY_LMENU             0xA4 /* VK_LMENU */
#define WSKEY_RMENU             0xA5 /* VK_RMENU */
#define WSKEY_BROWSER_BACK      0xA6 /* VK_BROWSER_BACK */
#define WSKEY_BROWSER_FORWARD   0xA7 /* VK_BROWSER_FORWARD */
#define WSKEY_BROWSER_REFRESH   0xA8 /* VK_BROWSER_REFRESH */
#define WSKEY_BROWSER_STOP      0xA9 /* VK_BROWSER_STOP */
#define WSKEY_BROWSER_SEARCH    0xAA /* VK_BROWSER_SEARCH */
#define WSKEY_BROWSER_FAVORITES 0xAB /* VK_BROWSER_FAVORITES */
#define WSKEY_BROWSER_HOME      0xAC /* VK_BROWSER_HOME */
#define WSKEY_VOLUME_MUTE       0xAD /* VK_VOLUME_MUTE */
#define WSKEY_VOLUME_DOWN       0xAE /* VK_VOLUME_DOWN */
#define WSKEY_VOLUME_UP         0xAF /* VK_VOLUME_UP */
#define WSKEY_MEDIA_NEXT_TRACK  0xB0 /* VK_MEDIA_NEXT_TRACK */
#define WSKEY_MEDIA_PREV_TRACK  0xB1 /* VK_MEDIA_PREV_TRACK */
#define WSKEY_MEDIA_STOP        0xB2 /* VK_MEDIA_STOP */
#define WSKEY_MEDIA_PLAY_PAUSE  0xB3 /* VK_MEDIA_PLAY_PAUSE */
#define WSKEY_LAUNCH_MAIL       0xB4 /* VK_LAUNCH_MAIL */
#define WSKEY_LAUNCH_MEDIA_SELECT 0xB5 /* VK_LAUNCH_MEDIA_SELECT */
#define WSKEY_LAUNCH_APP1       0xB6 /* VK_LAUNCH_APP1 */
#define WSKEY_LAUNCH_APP2       0xB7 /* VK_LAUNCH_APP2 */
#define WSKEY_OEM_1             0xBA /* VK_OEM_1 */
#define WSKEY_OEM_PLUS          0xBB /* VK_OEM_PLUS */
#define WSKEY_OEM_COMMA         0xBC /* VK_OEM_COMMA */
#define WSKEY_OEM_MINUS         0xBD /* VK_OEM_MINUS */
#define WSKEY_OEM_PERIOD        0xBE /* VK_OEM_PERIOD */
#define WSKEY_OEM_2             0xBF /* VK_OEM_2 */
#define WSKEY_OEM_3             0xC0 /* VK_OEM_3 */
#define WSKEY_OEM_4             0xDB /* VK_OEM_4 */
#define WSKEY_OEM_5             0xDC /* VK_OEM_5 */
#define WSKEY_OEM_6             0xDD /* VK_OEM_6 */
#define WSKEY_OEM_7             0xDE /* VK_OEM_7 */
#define WSKEY_OEM_8             0xDF /* VK_OEM_8 */
#define WSKEY_OEM_AX            0xE1 /* VK_OEM_AX */
#define WSKEY_OEM_102           0xE2 /* VK_OEM_102 */
#define WSKEY_ICO_HELP          0xE3 /* VK_ICO_HELP */
#define WSKEY_ICO_00            0xE4 /* VK_ICO_00 */
#define WSKEY_PROCESSKEY        0xE5 /* VK_PROCESSKEY */
#define WSKEY_ICO_CLEAR         0xE6 /* VK_ICO_CLEAR */
#define WSKEY_PACKET            0xE7 /* VK_PACKET */
#define WSKEY_OEM_RESET         0xE9 /* VK_OEM_RESET */
#define WSKEY_OEM_JUMP          0xEA /* VK_OEM_JUMP */
#define WSKEY_OEM_PA1           0xEB /* VK_OEM_PA1 */
#define WSKEY_OEM_PA2           0xEC /* VK_OEM_PA2 */
#define WSKEY_OEM_PA3           0xED /* VK_OEM_PA3 */
#define WSKEY_OEM_WSCTRL        0xEE /* VK_OEM_WSCTRL */
#define WSKEY_OEM_CUSEL         0xEF /* VK_OEM_CUSEL */
#define WSKEY_OEM_ATTN          0xF0 /* VK_OEM_ATTN */
#define WSKEY_OEM_FINISH        0xF1 /* VK_OEM_FINISH */
#define WSKEY_OEM_COPY          0xF2 /* VK_OEM_COPY */
#define WSKEY_OEM_AUTO          0xF3 /* VK_OEM_AUTO */
#define WSKEY_OEM_ENLW          0xF4 /* VK_OEM_ENLW */
#define WSKEY_OEM_BACKTAB       0xF5 /* VK_OEM_BACKTAB */
#define WSKEY_ATTN              0xF6 /* VK_ATTN */
#define WSKEY_CRSEL             0xF7 /* VK_CRSEL */
#define WSKEY_EXSEL             0xF8 /* VK_EXSEL */
#define WSKEY_EREOF             0xF9 /* VK_EREOF */
#define WSKEY_PLAY              0xFA /* VK_PLAY */
#define WSKEY_ZOOM              0xFB /* VK_ZOOM */
#define WSKEY_NONAME            0xFC /* VK_NONAME */
#define WSKEY_PA1               0xFD /* VK_PA1 */
#define WSKEY_OEM_CLEAR         0xFE /* VK_OEM_CLEAR */
#define WSKEY_GRAVE              0xC0 /* VK_OEM_3 */
#define WSKEY_APOSTROPHE         0xDE /* VK_OEM_7 */
#define WSKEY_BACKSLASH          0xDC /* VK_OEM_5 */
#define WSKEY_BRACKETLEFT        0xDB /* VK_OEM_4 */
#define WSKEY_BRACKETRIGHT       0xDD /* VK_OEM_6 */
#define WSKEY_COMMA              0xBC /* VK_OEM_COMMA */
#define WSKEY_EQUAL              0xBB /* VK_OEM_PLUS */
#define WSKEY_MINUS              0xBD /* VK_OEM_MINUS */
#define WSKEY_PERIOD             0xBE /* VK_OEM_PERIOD */
#define WSKEY_SEMICOLON          0xBA /* VK_OEM_1 */
#define WSKEY_SLASH              0xBF /* VK_OEM_2 */
#define WSKEY_SPACE              0x20 /* VK_SPACE */
#define WSKEY_QUOTEDBL           0xDE /* VK_OEM_7 */
#define WSKEY_HASH               0xDE /* VK_OEM_7 */
#define WSKEY_DOLLAR             0x24 /* VK_4 (Shifted) */
#define WSKEY_PERCENT            0x25 /* VK_5 (Shifted) */
#define WSKEY_AMPERSAND          0x26 /* VK_7 (Shifted) */
#define WSKEY_PARENLEFT          0x28 /* VK_9 (Shifted) */
#define WSKEY_PARENRIGHT         0x29 /* VK_0 (Shifted) */
#define WSKEY_ASTERISK           0x2A /* VK_8 (Shifted) */
#define WSKEY_PLUS               0xBB /* VK_OEM_PLUS */
#define WSKEY_LESS               0xBC /* VK_OEM_COMMA */
#define WSKEY_GREATER            0xBE /* VK_OEM_PERIOD */
#define WSKEY_QUESTION           0xBF /* VK_OEM_2 */
#define WSKEY_AT                 0x40 /* VK_2 (Shifted) */
#define WSKEY_CARET              0x5E /* VK_6 (Shifted) */
#define WSKEY_UNDERSCORE         0xBD /* VK_OEM_MINUS */
#define WSKEY_EXCLAMATION        0x21 /* VK_1 (Shifted) */
#define WSKEY_TILDE              0xC0 /* VK_OEM_3 */
/*=============================================================================
			__  ___               ______                 __
		   /  |/  /___ ______    / ____/   _____  ____  / /______
		  / /|_/ / __ `/ ___/   / __/ | | / / _ \/ __ \/ __/ ___/
		 / /  / / /_/ / /__    / /___ | |/ /  __/ / / / /_(__  )
		/_/  /_/\__,_/\___/   /_____/ |___/\___/_/ /_/\__/____/
=============================================================================*/
#elif defined(WSMACCLASSICORCARBON)
typedef struct EventRecord EventRecord;

/* Mac WSRECT */
#if !defined(guipos_t)
	#define guipos_t short
	typedef struct WSRECT
	{
	  short top;
	  short left;
	  short bottom;
	  short right;
	} WSRECT;
#endif
typedef struct WSEVENT
{
	WSWINDOW* pWindow;
	const EventRecord* er;
	uint32_t part;
} WSEVENT;
typedef WSEVENT WSSIZEEVENT;
typedef WSEVENT WSPAINTEVENT;
typedef WSEVENT WSKEYEVENT;
typedef WSEVENT WSCLOSEEVENT;
typedef WSEVENT WSNOTIFYICONEVENT;
typedef struct WSMENUEVENT
{
	WSWINDOW* pWindow;
} WSMENUEVENT;
typedef WSEVENT WSDESTROYEVENT;
typedef WSEVENT WSMOUSEWHEELEVENT;
typedef WSEVENT WSIDLEEVENT;
typedef WSEVENT WSCONTENTEVENT;
typedef WSEVENT WSGROWEVENT;
typedef WSEVENT WSZOOMEVENT;
typedef WSEVENT WSACTIVATEEVENT;
typedef WSEVENT WSDEACTIVATEEVENT;
typedef WSEVENT WSIBEAMEVENT;

#define WSEVENT_CALLBACK(x) wsbool (x)(WSWINDOW* cd, \
	const EventRecord* er, uint32_t inpart)
#define WSEVENT_HANDLED FALSE
#define WSEVENT_UNHANDLED TRUE
#define WSEvent_Construct(pThis) { (pThis)->pWindow = cd; \
	(pThis)->er = er; (pThis)->part = inpart; }

#define WSSIZEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSSIZEEVENT_HANDLED WSEVENT_HANDLED
#define WSSIZEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSSizeEvent_Construct WSEvent_Construct

#define WSPAINTEVENT_CALLBACK WSEVENT_CALLBACK
#define WSPAINTEVENT_HANDLED WSEVENT_HANDLED
#define WSPAINTEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSPaintEvent_Construct WSEvent_Construct

#define WSKEYEVENT_CALLBACK WSEVENT_CALLBACK
#define WSKEYEVENT_HANDLED WSEVENT_HANDLED
#define WSKEYEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSKeyEvent_Construct WSEvent_Construct

#define WSCLOSEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSCLOSEEVENT_HANDLED WSEVENT_HANDLED
#define WSCLOSEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSCloseEvent_Construct WSEvent_Construct

#define WSNOTIFYICONEVENT_CALLBACK WSEVENT_CALLBACK
#define WSNOTIFYICONEVENT_HANDLED WSEVENT_HANDLED
#define WSNOTIFYICONEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSNotifyIconEvent_Construct WSEvent_Construct

#define WSMENUEVENT_CALLBACK(x) void (x)(WSWINDOW* cd)
#define WSMENUEVENT_HANDLED
#define WSMENUEVENT_UNHANDLED
#define WSMenuEvent_Construct(pThis) (pThis)->pWindow = cd

#define WSDESTROYEVENT_CALLBACK WSEVENT_CALLBACK
#define WSDESTROYEVENT_HANDLED WSEVENT_HANDLED
#define WSDESTROYEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSDestroyEvent_Construct WSEvent_Construct

#define WSMOUSEWHEELEVENT_CALLBACK WSEVENT_CALLBACK
#define WSMOUSEWHEELEVENT_HANDLED WSEVENT_HANDLED
#define WSMOUSEWHEELEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSMouseWheelEvent_Construct WSEvent_Construct

#define WSCONTENTEVENT_CALLBACK WSEVENT_CALLBACK
#define WSCONTENTEVENT_HANDLED WSEVENT_HANDLED
#define WSCONTENTEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSContentEvent_Construct WSEvent_Construct

#define WSGROWEVENT_CALLBACK WSEVENT_CALLBACK
#define WSGROWEVENT_HANDLED WSEVENT_HANDLED
#define WSGROWEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSGrowEvent_Construct WSEvent_Construct

#define WSZOOMEVENT_CALLBACK WSEVENT_CALLBACK
#define WSZOOMEVENT_HANDLED WSEVENT_HANDLED
#define WSZOOMEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSZoomEvent_Construct WSEvent_Construct

#define WSIDLEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSIDLEEVENT_HANDLED WSEVENT_HANDLED
#define WSIDLEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSIdleEvent_Construct WSEvent_Construct

#define WSACTIVATEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSACTIVATEEVENT_HANDLED WSEVENT_HANDLED
#define WSACTIVATEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSActivateEvent_Construct WSEvent_Construct

#define WSDEACTIVATEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSDEACTIVATEEVENT_HANDLED WSEVENT_HANDLED
#define WSDEACTIVATEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSDeactivateEvent_Construct WSEvent_Construct

#define WSIBEAMEVENT_CALLBACK WSEVENT_CALLBACK
#define WSIBEAMEVENT_HANDLED WSEVENT_HANDLED
#define WSIBEAMEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSIBeamEvent_Construct WSEvent_Construct

/*
	Mac Menu Messsages
*/
void WSMenuMessages(WSWINDOW* pWindow, uint32_t menuresult);
#define BEGIN_ALL_MENU_MESSAGES void WSMenuMessages( \
	WSWINDOW* pWindow, uint32_t menuresult) \
{ \
	switch (loword(menuresult)) \
	{
#define BEGIN_MENU_MESSAGES(name)
#define DECLARE_MENU_MESSAGE(command, proc) \
		case command: \
		{ \
			proc(pWindow); \
			break; \
		}
#define END_MENU_MESSAGES(name)
#define END_ALL_MENU_MESSAGES \
		default: \
			break; \
	} \
}

/*
	Mac Window Messages
*/
typedef wsbool (*LPWSWindowMessages)(WSWINDOW* pWindow,
	unsigned int message, const EventRecord* er, uint32_t part);
wsbool WSWindowMessages(
	WSWINDOW* pWindow, unsigned int message, const EventRecord* er, uint32_t part);
#define BEGIN_ALL_WINDOW_MESSAGES wsbool WSWindowMessages( \
	WSWINDOW* pWindow, unsigned int message, const EventRecord* er, uint32_t part) \
{ if(0) {}
#define BEGIN_WINDOW_MESSAGES(name)
#define IMPLEMENT_MENU_WINDOW_MESSAGES(name)
#define IMPLEMENT_SIZE_CHANGED_MESSAGE(name)
#define IMPLEMENT_CLOSE_MESSAGE(name)
#define IMPLEMENT_DESTROYED_MESSAGE(name)
#define IMPLEMENT_PAINT_MESSAGE(name) \
	else if(message == WM_WSHANDLEUPDATEEVENT) \
		return name(pWindow, er, part);
#define IMPLEMENT_NOTIFYICON_MESSAGE(name, command)

#define IMPLEMENT_HANDLEIDLE_MESSAGE(name) \
	else if(message == WM_WSHANDLEIDLEEVENT) \
		return name(pWindow, er, part);
#define IMPLEMENT_HANDLECONTENT_MESSAGE(name) \
	else if(message == WM_WSHANDLECONTENTEVENT) \
		return name(pWindow, er, part);
#define IMPLEMENT_HANDLEGROW_MESSAGE(name) \
	else if(message == WM_WSHANDLEGROWEVENT) \
		return name(pWindow, er, part);
#define IMPLEMENT_HANDLEZOOM_MESSAGE(name) \
	else if(message == WM_WSHANDLEZOOMEVENT) \
		return name(pWindow, er, part);
#define IMPLEMENT_KEYDOWN_MESSAGE(name) \
	else if(message == WM_WSKEYDOWN) \
		return name(pWindow, er, part);
#define IMPLEMENT_HANDLEACTIVATE_MESSAGE(name) \
	else if(message == WM_WSHANDLEACTIVATEEVENT) \
		return name(pWindow, er, part);
#define IMPLEMENT_HANDLEDEACTIVATE_MESSAGE(name) \
	else if(message == WM_WSHANDLEDEACTIVATEEVENT) \
		return name(pWindow, er, part);
#define IMPLEMENT_HANDLEIBEAM_MESSAGE(name) \
	else if(message == WM_WSHANDLEIBEAMEVENT) \
		return name(pWindow, er, part);
#define IMPLEMENT_HANDLEMOUSEWHEEL_MESSAGE(name) \
	else if(message == WM_WSHANDLEMOUSEWHEELEVENT) \
		return name(pWindow, er, part);
#define IMPLEMENT_FALLBACK_MESSAGE(name)
#define END_WINDOW_MESSAGES(name)
#define END_ALL_WINDOW_MESSAGES \
	return WSEVENT_UNHANDLED; \
}

/*
	Mac key codes
*/
#define WSKEY_HOME                0x01 /* kHomeCharCode */
#define WSKEY_ENTER              0x03 /* kEnterCharCode */
#define WSKEY_END                 0x04 /* kEndCharCode */
#define WSKEY_HELP                0x05 /* kHelpCharCode */
#define WSKEY_BACK                0x08 /* kBackspaceCharCode */
#define WSKEY_TAB                 0x09 /* kTabCharCode */
#define WSKEY_PAGEUP              0x0B /* kPageUpCharCode */
#define WSKEY_PAGEDOWN            0x0C /* kPageDownCharCode */
#define WSKEY_RETURN              0x0D /* kReturnCharCode */
#define WSKEY_ESCAPE              0x1B /* kEscapeCharCode */
#define WSKEY_LEFT                0x1C /* kLeftArrowCharCode */
#define WSKEY_RIGHT               0x1D /* kRightArrowCharCode */
#define WSKEY_UP                  0x1E /* kUpArrowCharCode */
#define WSKEY_DOWN                0x1F /* kDownArrowCharCode */
#define WSKEY_DELETE              0x7F /* kDeleteCharCode */
#define WSKEY_F1                  0x7A /* kFunctionKeyCharCode (0x7A) */
#define WSKEY_F2                  0x78 /* kFunctionKeyCharCode (0x78) */
#define WSKEY_F3                  0x63 /* kFunctionKeyCharCode (0x63) */
#define WSKEY_F4                  0x76 /* kFunctionKeyCharCode (0x76) */
#define WSKEY_F5                  0x60 /* kFunctionKeyCharCode (0x60) */
#define WSKEY_F6                  0x61 /* kFunctionKeyCharCode (0x61) */
#define WSKEY_F7                  0x62 /* kFunctionKeyCharCode (0x62) */
#define WSKEY_F8                  0x64 /* kFunctionKeyCharCode (0x64) */
#define WSKEY_F9                  0x65 /* kFunctionKeyCharCode (0x65) */
#define WSKEY_F10                 0x6D /* kFunctionKeyCharCode (0x6D) */
#define WSKEY_F11                 0x67 /* kFunctionKeyCharCode (0x67) */
#define WSKEY_F12                 0x6F /* kFunctionKeyCharCode (0x6F) */
#define WSKEY_F13                 0x69 /* kFunctionKeyCharCode (0x69) */
#define WSKEY_F14                 0x6B /* kFunctionKeyCharCode (0x6B) */
#define WSKEY_F15                 0x71 /* kFunctionKeyCharCode (0x71) */
#define WSKEY_0                   0x30 /* '0' */
#define WSKEY_1                   0x31 /* '1' */
#define WSKEY_2                   0x32 /* '2' */
#define WSKEY_3                   0x33 /* '3' */
#define WSKEY_4                   0x34 /* '4' */
#define WSKEY_5                   0x35 /* '5' */
#define WSKEY_6                   0x36 /* '6' */
#define WSKEY_7                   0x37 /* '7' */
#define WSKEY_8                   0x38 /* '8' */
#define WSKEY_9                   0x39 /* '9' */
#define WSKEY_A                   0x41 /* 'A' */
#define WSKEY_B                   0x42 /* 'B' */
#define WSKEY_C                   0x43 /* 'C' */
#define WSKEY_D                   0x44 /* 'D' */
#define WSKEY_E                   0x45 /* 'E' */
#define WSKEY_F                   0x46 /* 'F' */
#define WSKEY_G                   0x47 /* 'G' */
#define WSKEY_H                   0x48 /* 'H' */
#define WSKEY_I                   0x49 /* 'I' */
#define WSKEY_J                   0x4A /* 'J' */
#define WSKEY_K                   0x4B /* 'K' */
#define WSKEY_L                   0x4C /* 'L' */
#define WSKEY_M                   0x4D /* 'M' */
#define WSKEY_N                   0x4E /* 'N' */
#define WSKEY_O                   0x4F /* 'O' */
#define WSKEY_P                   0x50 /* 'P' */
#define WSKEY_Q                   0x51 /* 'Q' */
#define WSKEY_R                   0x52 /* 'R' */
#define WSKEY_S                   0x53 /* 'S' */
#define WSKEY_T                   0x54 /* 'T' */
#define WSKEY_U                   0x55 /* 'U' */
#define WSKEY_V                   0x56 /* 'V' */
#define WSKEY_W                   0x57 /* 'W' */
#define WSKEY_X                   0x58 /* 'X' */
#define WSKEY_Y                   0x59 /* 'Y' */
#define WSKEY_Z                   0x5A /* 'Z' */
#define WSKEY_a                   0x61 /* 'a' */
#define WSKEY_b                   0x62 /* 'b' */
#define WSKEY_c                   0x63 /* 'c' */
#define WSKEY_d                   0x64 /* 'd' */
#define WSKEY_e                   0x65 /* 'e' */
#define WSKEY_f                   0x66 /* 'f' */
#define WSKEY_g                   0x67 /* 'g' */
#define WSKEY_h                   0x68 /* 'h' */
#define WSKEY_i                   0x69 /* 'i' */
#define WSKEY_j                   0x6A /* 'j' */
#define WSKEY_k                   0x6B /* 'k' */
#define WSKEY_l                   0x6C /* 'l' */
#define WSKEY_m                   0x6D /* 'm' */
#define WSKEY_n                   0x6E /* 'n' */
#define WSKEY_o                   0x6F /* 'o' */
#define WSKEY_p                   0x70 /* 'p' */
#define WSKEY_q                   0x71 /* 'q' */
#define WSKEY_r                   0x72 /* 'r' */
#define WSKEY_s                   0x73 /* 's' */
#define WSKEY_t                   0x74 /* 't' */
#define WSKEY_u                   0x75 /* 'u' */
#define WSKEY_v                   0x76 /* 'v' */
#define WSKEY_w                   0x77 /* 'w' */
#define WSKEY_x                   0x78 /* 'x' */
#define WSKEY_y                   0x79 /* 'y' */
#define WSKEY_z                   0x7A /* 'z' */
#define WSKEY_SPACE               0x20 /* ' ' */
#define WSKEY_EXCLAMATION         0x21 /* '!' */
#define WSKEY_QUOTEDBL            0x22 /* '"' */
#define WSKEY_HASH                0x23 /* '#' */
#define WSKEY_DOLLAR              0x24 /* '$' */
#define WSKEY_PERCENT             0x25 /* '%' */
#define WSKEY_AMPERSAND           0x26 /* '&' */
#define WSKEY_APOSTROPHE          0x27 /* '\'' */
#define WSKEY_PARENLEFT           0x28 /* '(' */
#define WSKEY_PARENRIGHT          0x29 /* ')' */
#define WSKEY_ASTERISK            0x2A /* '*' */
#define WSKEY_PLUS                0x2B /* '+' */
#define WSKEY_COMMA               0x2C /* ',' */
#define WSKEY_MINUS               0x2D /* '-' */
#define WSKEY_PERIOD              0x2E /* '.' */
#define WSKEY_SLASH               0x2F /* '/' */
#define WSKEY_COLON               0x3A /* ':' */
#define WSKEY_SEMICOLON           0x3B /* ';' */
#define WSKEY_LESS                0x3C /* '<' */
#define WSKEY_EQUAL               0x3D /* '=' */
#define WSKEY_GREATER             0x3E /* '>' */
#define WSKEY_QUESTION            0x3F /* '?' */
#define WSKEY_AT                  0x40 /* '@' */
#define WSKEY_BRACKETLEFT         0x5B /* '[' */
#define WSKEY_BACKSLASH           0x5C /* '\\' */
#define WSKEY_BRACKETRIGHT        0x5D /* ']' */
#define WSKEY_CARET               0x5E /* '^' */
#define WSKEY_UNDERSCORE          0x5F /* '_' */
#define WSKEY_GRAVE               0x60 /* '`' */
#define WSKEY_a                   0x61 /* 'a' */
#define WSKEY_b                   0x62 /* 'b' */
#define WSKEY_c                   0x63 /* 'c' */
#define WSKEY_d                   0x64 /* 'd' */
#define WSKEY_e                   0x65 /* 'e' */
#define WSKEY_f                   0x66 /* 'f' */
#define WSKEY_g                   0x67 /* 'g' */
#define WSKEY_h                   0x68 /* 'h' */
#define WSKEY_i                   0x69 /* 'i' */
#define WSKEY_j                   0x6A /* 'j' */
#define WSKEY_k                   0x6B /* 'k' */
#define WSKEY_l                   0x6C /* 'l' */
#define WSKEY_m                   0x6D /* 'm' */
#define WSKEY_n                   0x6E /* 'n' */
#define WSKEY_o                   0x6F /* 'o' */
#define WSKEY_p                   0x70 /* 'p' */
#define WSKEY_q                   0x71 /* 'q' */
#define WSKEY_r                   0x72 /* 'r' */
#define WSKEY_s                   0x73 /* 's' */
#define WSKEY_t                   0x74 /* 't' */
#define WSKEY_u                   0x75 /* 'u' */
#define WSKEY_v                   0x76 /* 'v' */
#define WSKEY_w                   0x77 /* 'w' */
#define WSKEY_x                   0x78 /* 'x' */
#define WSKEY_y                   0x79 /* 'y' */
#define WSKEY_z                   0x7A /* 'z' */
#define WSKEY_TILDE               0x7E /* '~' */
/*=============================================================================
   ____  ______  __    ___ ____  _____     ______                 __
  / __ \/ ___/ |/ /  _/_(_) __ \/ ___/    / ____/   _____  ____  / /______
 / / / /\__ \|   / _/_// / / / /\__ \    / __/ | | / / _ \/ __ \/ __/ ___/
/ /_/ /___/ /   |_/_/ / / /_/ /___/ /   / /___ | |/ /  __/ / / / /_(__  )
\____//____/_/|_/_/  /_/\____//____/   /_____/ |___/\___/_/ /_/\__/____/
=============================================================================*/
#elif defined(__MACH__)
/*
	OSX/iOS types
*/
#if !defined(guipos_t)
	#define guipos_t wsfloat
	typedef struct WSRECT
	{
		struct
		{
			wsfloat x;
			wsfloat y;
		} origin;
		struct
		{
			wsfloat width;
			wsfloat height;
		} size;
	} WSRECT;
#endif

typedef struct WSEVENT
{
	void* mWindow;
	void* mEvent;
} WSEVENT;
typedef WSEVENT WSSIZEEVENT;
typedef WSEVENT WSPAINTEVENT;
typedef WSEVENT WSKEYEVENT;
typedef WSEVENT WSCLOSEEVENT;
typedef WSEVENT WSNOTIFYICONEVENT;
typedef struct WSMENUEVENT
{
	void* mWindow;
} WSMENUEVENT;
typedef WSEVENT WSDESTROYEVENT;
typedef WSEVENT WSMOUSEWHEELEVENT;
typedef WSEVENT WSIDLEEVENT;
typedef WSEVENT WSCONTENTEVENT;
typedef WSEVENT WSGROWEVENT;
typedef WSEVENT WSZOOMEVENT;
typedef WSEVENT WSACTIVATEEVENT;
typedef WSEVENT WSDEACTIVATEEVENT;
typedef WSEVENT WSIBEAMEVENT;

#define WSEVENT_CALLBACK(x) wsbool (x)(void* qwin, void* qev)
#define WSEVENT_HANDLED FALSE
#define WSEVENT_UNHANDLED TRUE
#define WSEvent_Construct(pThis) { (pThis)->mWindow = qwin; \
	(pThis)->mEvent = qev; }

#define WSSIZEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSSIZEEVENT_HANDLED WSEVENT_HANDLED
#define WSSIZEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSSizeEvent_Construct WSEvent_Construct

#define WSPAINTEVENT_CALLBACK WSEVENT_CALLBACK
#define WSPAINTEVENT_HANDLED WSEVENT_HANDLED
#define WSPAINTEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSPaintEvent_Construct WSEvent_Construct

#define WSKEYEVENT_CALLBACK WSEVENT_CALLBACK
#define WSKEYEVENT_HANDLED WSEVENT_HANDLED
#define WSKEYEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSKeyEvent_Construct WSEvent_Construct

#define WSCLOSEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSCLOSEEVENT_HANDLED WSEVENT_HANDLED
#define WSCLOSEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSCloseEvent_Construct WSEvent_Construct

#define WSNOTIFYICONEVENT_CALLBACK WSEVENT_CALLBACK
#define WSNOTIFYICONEVENT_HANDLED WSEVENT_HANDLED
#define WSNOTIFYICONEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSNotifyIconEvent_Construct WSEvent_Construct

#define WSMENUEVENT_CALLBACK(x) wsbool (x)(void* qwin)
#define WSMENUEVENT_HANDLED WSEVENT_HANDLED
#define WSMENUEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSMenuEvent_Construct(pThis) (pThis)->mWindow = qwin

#define WSDESTROYEVENT_CALLBACK WSEVENT_CALLBACK
#define WSDESTROYEVENT_HANDLED WSEVENT_HANDLED
#define WSDESTROYEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSDestroyEvent_Construct WSEvent_Construct

#define WSMOUSEWHEELEVENT_CALLBACK WSEVENT_CALLBACK
#define WSMOUSEWHEELEVENT_HANDLED WSEVENT_HANDLED
#define WSMOUSEWHEELEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSMouseWheelEvent_Construct WSEvent_Construct

#define WSCONTENTEVENT_CALLBACK WSEVENT_CALLBACK
#define WSCONTENTEVENT_HANDLED WSEVENT_HANDLED
#define WSCONTENTEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSContentEvent_Construct WSEvent_Construct

#define WSGROWEVENT_CALLBACK WSEVENT_CALLBACK
#define WSGROWEVENT_HANDLED WSEVENT_HANDLED
#define WSGROWEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSGrowEvent_Construct WSEvent_Construct

#define WSZOOMEVENT_CALLBACK WSEVENT_CALLBACK
#define WSZOOMEVENT_HANDLED WSEVENT_HANDLED
#define WSZOOMEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSZoomEvent_Construct WSEvent_Construct

#define WSIDLEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSIDLEEVENT_HANDLED WSEVENT_HANDLED
#define WSIDLEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSIdleEvent_Construct WSEvent_Construct

#define WSACTIVATEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSACTIVATEEVENT_HANDLED WSEVENT_HANDLED
#define WSACTIVATEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSActivateEvent_Construct WSEvent_Construct

#define WSDEACTIVATEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSDEACTIVATEEVENT_HANDLED WSEVENT_HANDLED
#define WSDEACTIVATEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSDeactivateEvent_Construct WSEvent_Construct

#define WSIBEAMEVENT_CALLBACK WSEVENT_CALLBACK
#define WSIBEAMEVENT_HANDLED WSEVENT_HANDLED
#define WSIBEAMEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSIBeamEvent_Construct WSEvent_Construct

/*
	OSX/iOS event map
*/
typedef wsbool (*LPWSMenuCallback)(void* pWindow);
LPWSMenuCallback WSMenuMessages(unsigned int desiredcommand);
#define BEGIN_ALL_MENU_MESSAGES LPWSMenuCallback WSMenuMessages(\
	unsigned int desiredcommand) \
{ \
	switch (desiredcommand) \
	{
#define BEGIN_MENU_MESSAGES(name)
#define DECLARE_MENU_MESSAGE(command, proc) \
		case command: \
		{ \
			return proc; \
		}
#define END_MENU_MESSAGES(name)
#define END_ALL_MENU_MESSAGES \
	} \
	return WSMenuSectionDefaultCallback; \
}

#define WSWINDOWMESSAGEARGSDECLARE \
	void* qwin, unsigned int msg, void* qev
#define WSWINDOWMESSAGEARGSPASS qwin, msg, qev

typedef wsbool (*LPWSWindowMessages)(WSWINDOWMESSAGEARGSDECLARE);
wsbool WSWindowMessages(WSWINDOWMESSAGEARGSDECLARE);
#define BEGIN_ALL_WINDOW_MESSAGES \
wsbool WSWindowMessages(WSWINDOWMESSAGEARGSDECLARE) \
{ \
	if(0) {}
#define BEGIN_WINDOW_MESSAGES(name)
#define IMPLEMENT_MENU_WINDOW_MESSAGES(name)
#define IMPLEMENT_SIZE_CHANGED_MESSAGE(name) \
	else if(msg == WM_WSSIZE) \
		return name(qwin, qev);
#define IMPLEMENT_CLOSE_MESSAGE(name) \
	else if(msg == WM_WSCLOSE) \
		return name(qwin, qev);
#define IMPLEMENT_DESTROYED_MESSAGE(name) \
	else if(msg == WM_WSDESTROY) \
		return name(qwin, qev);
#define IMPLEMENT_NOTIFYICON_MESSAGE(name, command) \
	else if(msg == command + WM_WSNOTIFYICON) \
		return name(qwin, qev);
#define IMPLEMENT_HANDLEIDLE_MESSAGE(name)
#define IMPLEMENT_HANDLECONTENT_MESSAGE(name)
#define IMPLEMENT_HANDLEGROW_MESSAGE(name)
#define IMPLEMENT_HANDLEZOOM_MESSAGE(name)
#define IMPLEMENT_KEYDOWN_MESSAGE(name) \
	else if(msg == WM_WSKEYDOWN) \
		return name(qwin, qev);
#define IMPLEMENT_HANDLEACTIVATE_MESSAGE(name)
#define IMPLEMENT_HANDLEDEACTIVATE_MESSAGE(name)
#define IMPLEMENT_PAINT_MESSAGE(name)
#define IMPLEMENT_HANDLEIBEAM_MESSAGE(name)
#define IMPLEMENT_HANDLEMOUSEWHEEL_MESSAGE(name)
#define IMPLEMENT_FALLBACK_MESSAGE(name)
#define END_WINDOW_MESSAGES(name)
#define END_ALL_WINDOW_MESSAGES \
	return WSEVENT_UNHANDLED; \
}

/*
	OSX/iOS key codes
*/
#if defined(WSMACCOCOA)
	#define WSKEY_LEFT                0x7B /* NSLeftArrowFunctionKey */
	#define WSKEY_RIGHT               0x7C /* NSRightArrowFunctionKey */
	#define WSKEY_UP                  0x7E /* NSUpArrowFunctionKey */
	#define WSKEY_DOWN                0x7D /* NSDownArrowFunctionKey */
	#define WSKEY_RETURN              0x24 /* NSReturnFunctionKey */
	#define WSKEY_ESCAPE              0x35 /* NSEscapeFunctionKey */
	#define WSKEY_SPACE               0x31 /* NSSpaceFunctionKey */
	#define WSKEY_BACK                0x33 /* NSBackspaceFunctionKey */
	#define WSKEY_TAB                 0x30 /* NSTabFunctionKey */
	#define WSKEY_SHIFT               0x38 /* NSShiftFunctionKey */
	#define WSKEY_CONTROL             0x3B /* NSControlFunctionKey */
	#define WSKEY_MENU                0x3A /* NSAlternateFunctionKey */
	#define WSKEY_PAUSE               0x71 /* NSPauseFunctionKey */
	#define WSKEY_CAPITAL             0x39 /* NSCapsLockFunctionKey */
	#define WSKEY_PRIOR               0x74 /* NSPageUpFunctionKey */
	#define WSKEY_NEXT                0x79 /* NSPageDownFunctionKey */
	#define WSKEY_END                 0x77 /* NSEndFunctionKey */
	#define WSKEY_HOME                0x73 /* NSHomeFunctionKey */
	#define WSKEY_SELECT              0x77 /* NSSelectFunctionKey */
	#define WSKEY_PRINT               0x69 /* NSPrintScreenFunctionKey */
	#define WSKEY_EXECUTE             0x75 /* NSExecuteFunctionKey */
	#define WSKEY_INSERT              0x72 /* NSInsertFunctionKey */
	#define WSKEY_DELETE              0x75 /* NSDeleteFunctionKey */
	#define WSKEY_HELP                0x72 /* NSHelpFunctionKey */
	#define WSKEY_0                   0x1D /* 0 */
	#define WSKEY_1                   0x12 /* 1 */
	#define WSKEY_2                   0x13 /* 2 */
	#define WSKEY_3                   0x14 /* 3 */
	#define WSKEY_4                   0x15 /* 4 */
	#define WSKEY_5                   0x17 /* 5 */
	#define WSKEY_6                   0x16 /* 6 */
	#define WSKEY_7                   0x1A /* 7 */
	#define WSKEY_8                   0x1C /* 8 */
	#define WSKEY_9                   0x19 /* 9 */
	#define WSKEY_A                   0x00 /* A */
	#define WSKEY_B                   0x0B /* B */
	#define WSKEY_C                   0x08 /* C */
	#define WSKEY_D                   0x02 /* D */
	#define WSKEY_E                   0x0E /* E */
	#define WSKEY_F                   0x03 /* F */
	#define WSKEY_G                   0x05 /* G */
	#define WSKEY_H                   0x04 /* H */
	#define WSKEY_I                   0x22 /* I */
	#define WSKEY_J                   0x26 /* J */
	#define WSKEY_K                   0x28 /* K */
	#define WSKEY_L                   0x25 /* L */
	#define WSKEY_M                   0x2E /* M */
	#define WSKEY_N                   0x2D /* N */
	#define WSKEY_O                   0x1F /* O */
	#define WSKEY_P                   0x23 /* P */
	#define WSKEY_Q                   0x0C /* Q */
	#define WSKEY_R                   0x0F /* R */
	#define WSKEY_S                   0x01 /* S */
	#define WSKEY_T                   0x11 /* T */
	#define WSKEY_U                   0x20 /* U */
	#define WSKEY_V                   0x09 /* V */
	#define WSKEY_W                   0x0D /* W */
	#define WSKEY_X                   0x07 /* X */
	#define WSKEY_Y                   0x10 /* Y */
	#define WSKEY_Z                   0x06 /* Z */
	#define WSKEY_LWIN                0x37 /* NSCommandFunctionKey (Left) */
	#define WSKEY_RWIN                0x36 /* NSCommandFunctionKey (Right) */
	#define WSKEY_NUMPAD0             0x52 /* 0 (NumPad) */
	#define WSKEY_NUMPAD1             0x53 /* 1 (NumPad) */
	#define WSKEY_NUMPAD2             0x54 /* 2 (NumPad) */
	#define WSKEY_NUMPAD3             0x55 /* 3 (NumPad) */
	#define WSKEY_NUMPAD4             0x56 /* 4 (NumPad) */
	#define WSKEY_NUMPAD5             0x57 /* 5 (NumPad) */
	#define WSKEY_NUMPAD6             0x58 /* 6 (NumPad) */
	#define WSKEY_NUMPAD7             0x59 /* 7 (NumPad) */
	#define WSKEY_NUMPAD8             0x5B /* 8 (NumPad) */
	#define WSKEY_NUMPAD9             0x5C /* 9 (NumPad) */
	#define WSKEY_MULTIPLY            0x43 /* * (NumPad) */
	#define WSKEY_ADD                 0x45 /* + (NumPad) */
	#define WSKEY_SEPARATOR           0x41 /* Clear (NumPad) */
	#define WSKEY_SUBTRACT            0x4E /* - (NumPad) */
	#define WSKEY_DECIMAL             0x41 /* . (NumPad) */
	#define WSKEY_DIVIDE              0x4B /* / (NumPad) */
	#define WSKEY_F1                  0x7A /* F1 */
	#define WSKEY_F2                  0x78 /* F2 */
	#define WSKEY_F3                  0x63 /* F3 */
	#define WSKEY_F4                  0x76 /* F4 */
	#define WSKEY_F5                  0x60 /* F5 */
	#define WSKEY_F6                  0x61 /* F6 */
	#define WSKEY_F7                  0x62 /* F7 */
	#define WSKEY_F8                  0x64 /* F8 */
	#define WSKEY_F9                  0x65 /* F9 */
	#define WSKEY_F10                 0x6D /* F10 */
	#define WSKEY_F11                 0x67 /* F11 */
	#define WSKEY_F12                 0x6F /* F12 */
	#define WSKEY_F13                 0x69 /* F13 */
	#define WSKEY_F14                 0x6B /* F14 */
	#define WSKEY_F15                 0x71 /* F15 */
	#define WSKEY_F16                 0x6A /* F16 */
	#define WSKEY_F17                 0x40 /* F17 */
	#define WSKEY_F18                 0x4F /* F18 */
	#define WSKEY_F19                 0x50 /* F19 */
	#define WSKEY_F20                 0x5A /* F20 */
	#define WSKEY_NUMLOCK             0x47 /* NSNumLockFunctionKey */
	#define WSKEY_SCROLL              0x6E /* NSScrollLockFunctionKey */
	#define WSKEY_LSHIFT              0x38 /* NSShiftFunctionKey (Left) */
	#define WSKEY_RSHIFT              0x3C /* NSShiftFunctionKey (Right) */
	#define WSKEY_LCONTROL            0x3B /* NSControlFunctionKey (Left) */
	#define WSKEY_RCONTROL            0x3E /* NSControlFunctionKey (Right) */
	#define WSKEY_LMENU               0x3A /* NSAlternateFunctionKey (Left) */
	#define WSKEY_RMENU               0x3D /* NSAlternateFunctionKey (Right) */
	#define WSKEY_VOLUME_MUTE         0x4A /* NSAudioVolumeMuteFunctionKey */
	#define WSKEY_VOLUME_DOWN         0x49 /* NSAudioVolumeDownFunctionKey */
	#define WSKEY_VOLUME_UP           0x48 /* NSAudioVolumeUpFunctionKey */
	#define WSKEY_MEDIA_NEXT_TRACK    0x4D /* NSAudioNextFunctionKey */
	#define WSKEY_MEDIA_PREV_TRACK    0x4B /* NSAudioPrevFunctionKey */
	#define WSKEY_MEDIA_STOP          0x4E /* NSAudioStopFunctionKey */
	#define WSKEY_MEDIA_PLAY_PAUSE    0x4C /* NSAudioPlayPauseFunctionKey */
	#define WSKEY_LAUNCH_MAIL         0x60 /* NSMailFunctionKey */
	#define WSKEY_LAUNCH_APP1         0x65 /* NSLaunch0FunctionKey */
	#define WSKEY_LAUNCH_APP2         0x6D /* NSLaunch1FunctionKey */
	#define WSKEY_OEM_1               0x29 /* ; */
	#define WSKEY_OEM_PLUS            0x18 /* = */
	#define WSKEY_OEM_COMMA           0x2B /* , */
	#define WSKEY_OEM_MINUS           0x1B /* - */
	#define WSKEY_OEM_PERIOD          0x2F /* . */
	#define WSKEY_OEM_2               0x2C /* / */
	#define WSKEY_OEM_3               0x32 /* ` */
	#define WSKEY_OEM_4               0x21 /* [ */
	#define WSKEY_OEM_5               0x2A /* \ */
	#define WSKEY_OEM_6               0x1E /* ] */
	#define WSKEY_OEM_7               0x27 /* ' */
	#define WSKEY_PLAY                0x76 /* NSPlayFunctionKey */
	#define WSKEY_ZOOM                0x77 /* NSZoomFunctionKey */
	#define WSKEY_GRAVE               0x32 /* ` */
	#define WSKEY_APOSTROPHE          0x27 /* ' */
	#define WSKEY_BACKSLASH           0x2A /* \ */
	#define WSKEY_BRACKETLEFT         0x21 /* [ */
	#define WSKEY_BRACKETRIGHT        0x1E /* ] */
	#define WSKEY_COMMA               0x2B /* , */
	#define WSKEY_EQUAL               0x18 /* = */
	#define WSKEY_MINUS               0x1B /* - */
	#define WSKEY_PERIOD              0x2F /* . */
	#define WSKEY_SEMICOLON           0x29 /* ; */
	#define WSKEY_SLASH               0x2C /* / */
	#define WSKEY_SPACE               0x31 /*   */
	#define WSKEY_QUOTEDBL            0x27 /* " */
	#define WSKEY_HASH                0x23 /* # */
	#define WSKEY_DOLLAR              0x24 /* $ */
	#define WSKEY_PERCENT             0x25 /* % */
	#define WSKEY_AMPERSAND           0x26 /* & */
	#define WSKEY_PARENLEFT           0x28 /* ( */
	#define WSKEY_PARENRIGHT          0x29 /* ) */
	#define WSKEY_ASTERISK            0x2A /* * */
	#define WSKEY_PLUS                0x2B /* + */
	#define WSKEY_LESS                0x2C /* < */
	#define WSKEY_GREATER             0x2E /* > */
	#define WSKEY_QUESTION            0x3F /* ? */
	#define WSKEY_AT                  0x40 /* @ */
	#define WSKEY_CARET               0x5E /* ^ */
	#define WSKEY_UNDERSCORE          0x5F /* _ */
	#define WSKEY_EXCLAMATION         0x21 /* ! */
	#define WSKEY_TILDE               0x7E /* ~ */
#else /* WSMACUIKIT */
	#define WSKEY_LEFT              0xF704 /* UIKeyInputLeftArrow */
	#define WSKEY_RIGHT             0xF705 /* UIKeyInputRightArrow */
	#define WSKEY_UP                0xF700 /* UIKeyInputUpArrow */
	#define WSKEY_DOWN              0xF701 /* UIKeyInputDownArrow */
	#define WSKEY_RETURN            0x000D /* UIKeyInputReturn */
	#define WSKEY_ESCAPE            0x001B /* UIKeyInputEscape */
	#define WSKEY_SPACE             0x0020 /* UIKeyInputSpace */
	#define WSKEY_BACK              0x0008 /* UIKeyInputBackspace */
	#define WSKEY_TAB               0x0009 /* UIKeyInputTab */
	#define WSKEY_SHIFT             0x0010 /* UIKeyInputShift */
	#define WSKEY_CONTROL           0x0011 /* UIKeyInputControl */
	#define WSKEY_MENU              0x0012 /* UIKeyInputAlt */
	#define WSKEY_PAUSE             0x0013 /* UIKeyInputPause */
	#define WSKEY_CAPITAL           0x0014 /* UIKeyInputCapsLock */
	#define WSKEY_PRIOR             0xF72C /* UIKeyInputPageUp */
	#define WSKEY_NEXT              0xF72D /* UIKeyInputPageDown */
	#define WSKEY_END               0xF72B /* UIKeyInputEnd */
	#define WSKEY_HOME              0xF729 /* UIKeyInputHome */
	#define WSKEY_SELECT            0xF737 /* UIKeyInputSelect */
	#define WSKEY_PRINT             0xF738 /* UIKeyInputPrintScreen */
	#define WSKEY_EXECUTE           0xF739 /* UIKeyInputExecute */
	#define WSKEY_INSERT            0xF727 /* UIKeyInputInsert */
	#define WSKEY_DELETE            0xF728 /* UIKeyInputDelete */
	#define WSKEY_HELP              0xF746 /* UIKeyInputHelp */
	#define WSKEY_0                 0x0030 /* '0' */
	#define WSKEY_1                 0x0031 /* '1' */
	#define WSKEY_2                 0x0032 /* '2' */
	#define WSKEY_3                 0x0033 /* '3' */
	#define WSKEY_4                 0x0034 /* '4' */
	#define WSKEY_5                 0x0035 /* '5' */
	#define WSKEY_6                 0x0036 /* '6' */
	#define WSKEY_7                 0x0037 /* '7' */
	#define WSKEY_8                 0x0038 /* '8' */
	#define WSKEY_9                 0x0039 /* '9' */
	#define WSKEY_A                 0x0041 /* 'A' */
	#define WSKEY_B                 0x0042 /* 'B' */
	#define WSKEY_C                 0x0043 /* 'C' */
	#define WSKEY_D                 0x0044 /* 'D' */
	#define WSKEY_E                 0x0045 /* 'E' */
	#define WSKEY_F                 0x0046 /* 'F' */
	#define WSKEY_G                 0x0047 /* 'G' */
	#define WSKEY_H                 0x0048 /* 'H' */
	#define WSKEY_I                 0x0049 /* 'I' */
	#define WSKEY_J                 0x004A /* 'J' */
	#define WSKEY_K                 0x004B /* 'K' */
	#define WSKEY_L                 0x004C /* 'L' */
	#define WSKEY_M                 0x004D /* 'M' */
	#define WSKEY_N                 0x004E /* 'N' */
	#define WSKEY_O                 0x004F /* 'O' */
	#define WSKEY_P                 0x0050 /* 'P' */
	#define WSKEY_Q                 0x0051 /* 'Q' */
	#define WSKEY_R                 0x0052 /* 'R' */
	#define WSKEY_S                 0x0053 /* 'S' */
	#define WSKEY_T                 0x0054 /* 'T' */
	#define WSKEY_U                 0x0055 /* 'U' */
	#define WSKEY_V                 0x0056 /* 'V' */
	#define WSKEY_W                 0x0057 /* 'W' */
	#define WSKEY_X                 0x0058 /* 'X' */
	#define WSKEY_Y                 0x0059 /* 'Y' */
	#define WSKEY_Z                 0x005A /* 'Z' */
	#define WSKEY_LWIN              0xF73D /* UIKeyInputMeta (Left) */
	#define WSKEY_RWIN              0xF73E /* UIKeyInputMeta (Right) */
	#define WSKEY_NUMPAD0           0x0030 /* '0' */
	#define WSKEY_NUMPAD1           0x0031 /* '1' */
	#define WSKEY_NUMPAD2           0x0032 /* '2' */
	#define WSKEY_NUMPAD3           0x0033 /* '3' */
	#define WSKEY_NUMPAD4           0x0034 /* '4' */
	#define WSKEY_NUMPAD5           0x0035 /* '5' */
	#define WSKEY_NUMPAD6           0x0036 /* '6' */
	#define WSKEY_NUMPAD7           0x0037 /* '7' */
	#define WSKEY_NUMPAD8           0x0038 /* '8' */
	#define WSKEY_NUMPAD9           0x0039 /* '9' */
	#define WSKEY_MULTIPLY          0x002A /* '*' */
	#define WSKEY_ADD               0x002B /* '+' */
	#define WSKEY_SEPARATOR         0x002C /* ',' */
	#define WSKEY_SUBTRACT          0x002D /* '-' */
	#define WSKEY_DECIMAL           0x002E /* '.' */
	#define WSKEY_DIVIDE            0x002F /* '/' */
	#define WSKEY_F1                0xF704 /* UIKeyInputF1 */
	#define WSKEY_F2                0xF705 /* UIKeyInputF2 */
	#define WSKEY_F3                0xF706 /* UIKeyInputF3 */
	#define WSKEY_F4                0xF707 /* UIKeyInputF4 */
	#define WSKEY_F5                0xF708 /* UIKeyInputF5 */
	#define WSKEY_F6                0xF709 /* UIKeyInputF6 */
	#define WSKEY_F7                0xF70A /* UIKeyInputF7 */
	#define WSKEY_F8                0xF70B /* UIKeyInputF8 */
	#define WSKEY_F9                0xF70C /* UIKeyInputF9 */
	#define WSKEY_F10               0xF70D /* UIKeyInputF10 */
	#define WSKEY_F11               0xF70E /* UIKeyInputF11 */
	#define WSKEY_F12               0xF70F /* UIKeyInputF12 */
	#define WSKEY_F13               0xF710 /* UIKeyInputF13 */
	#define WSKEY_F14               0xF711 /* UIKeyInputF14 */
	#define WSKEY_F15               0xF712 /* UIKeyInputF15 */
	#define WSKEY_F16               0xF713 /* UIKeyInputF16 */
	#define WSKEY_F17               0xF714 /* UIKeyInputF17 */
	#define WSKEY_F18               0xF715 /* UIKeyInputF18 */
	#define WSKEY_F19               0xF716 /* UIKeyInputF19 */
	#define WSKEY_F20               0xF717 /* UIKeyInputF20 */
	#define WSKEY_F21               0xF718 /* UIKeyInputF21 */
	#define WSKEY_F22               0xF719 /* UIKeyInputF22 */
	#define WSKEY_F23               0xF71A /* UIKeyInputF23 */
	#define WSKEY_F24               0xF71B /* UIKeyInputF24 */
	#define WSKEY_NUMLOCK           0xF710 /* UIKeyInputNumLock */
	#define WSKEY_SCROLL            0xF71C /* UIKeyInputScrollLock */
	#define WSKEY_LSHIFT            0xF738 /* UIKeyInputShift (Left) */
	#define WSKEY_RSHIFT            0xF739 /* UIKeyInputShift (Right) */
	#define WSKEY_LCONTROL          0xF71D /* UIKeyInputControl (Left) */
	#define WSKEY_RCONTROL          0xF71E /* UIKeyInputControl (Right) */
	#define WSKEY_LMENU             0xF73A /* UIKeyInputAlt (Left) */
	#define WSKEY_RMENU             0xF73B /* UIKeyInputAlt (Right) */
	#define WSKEY_VOLUME_MUTE       0xF728 /* UIKeyInputVolumeMute */
	#define WSKEY_VOLUME_DOWN       0xF729 /* UIKeyInputVolumeDown */
	#define WSKEY_VOLUME_UP         0xF72A /* UIKeyInputVolumeUp */
	#define WSKEY_MEDIA_NEXT_TRACK	0xF7A3 /* UIKeyInputNext */
	#define WSKEY_MEDIA_PREV_TRACK  0xF7A4 /* UIKeyInputPrevious */
	#define WSKEY_MEDIA_STOP        0xF7A2 /* UIKeyInputStop */
	#define WSKEY_MEDIA_PLAY_PAUSE  0xF7A1 /* UIKeyInputPlayPause */
	#define WSKEY_LAUNCH_MAIL       0xF73C /* UIKeyInputLaunchMail */
	#define WSKEY_LAUNCH_APP1       0xF740 /* UIKeyInputLaunchApp1 */
	#define WSKEY_LAUNCH_APP2       0xF741 /* UIKeyInputLaunchApp2 */
	#define WSKEY_OEM_1             0x003B /* ';' */
	#define WSKEY_OEM_PLUS          0x003D /* '=' */
	#define WSKEY_OEM_COMMA         0x002C /* ',' */
	#define WSKEY_OEM_MINUS         0x002D /* '-' */
	#define WSKEY_OEM_PERIOD        0x002E /* '.' */
	#define WSKEY_OEM_2             0x002F /* '/' */
	#define WSKEY_OEM_3             0x0060 /* '`' */
	#define WSKEY_OEM_4             0x005B /* '[' */
	#define WSKEY_OEM_5             0x005C /* '\' */
	#define WSKEY_OEM_6             0x005D /* ']' */
	#define WSKEY_OEM_7             0x0027 /* '\'' */
	#define WSKEY_PLAY              0xF728 /* UIKeyInputPlay */
	#define WSKEY_ZOOM              0xF73F /* UIKeyInputZoom */
	#define WSKEY_GRAVE				0x0060 /* '`' */
	#define WSKEY_APOSTROPHE		0x0027 /* '\'' */
	#define WSKEY_BACKSLASH			0x005C /* '\\' */
	#define WSKEY_BRACKETLEFT		0x005B /* '[' */
	#define WSKEY_BRACKETRIGHT		0x005D /* ']' */
	#define WSKEY_COMMA				0x002C /* ',' */
	#define WSKEY_EQUAL				0x003D /* '=' */
	#define WSKEY_MINUS				0x002D /* '-' */
	#define WSKEY_PERIOD			0x002E /* '.' */
	#define WSKEY_SEMICOLON			0x003B /* ';' */
	#define WSKEY_SLASH				0x002F /* '/' */
	#define WSKEY_SPACE				0x0020 /* ' ' */
	#define WSKEY_QUOTEDBL			0x0022 /* '"' */
	#define WSKEY_HASH				0x0023 /* '#' */
	#define WSKEY_DOLLAR			0x0024 /* '$' */
	#define WSKEY_PERCENT			0x0025 /* '%' */
	#define WSKEY_AMPERSAND			0x0026 /* '&' */
	#define WSKEY_PARENLEFT			0x0028 /* '(' */
	#define WSKEY_PARENRIGHT		0x0029 /* ')' */
	#define WSKEY_ASTERISK			0x002A /* '*' */
	#define WSKEY_PLUS				0x002B /* '+' */
	#define WSKEY_LESS				0x003C /* '<' */
	#define WSKEY_GREATER			0x003E /* '>' */
	#define WSKEY_QUESTION			0x003F /* '?' */
	#define WSKEY_AT				0x0040 /* '@' */
	#define WSKEY_CARET				0x005E /* '^' */
	#define WSKEY_UNDERSCORE		0x005F /* '_' */
	#define WSKEY_EXCLAMATION		0x0021 /* '!' */
	#define WSKEY_TILDE				0x007E /* '~' */
#endif /* OSX/iOS switch */

/*=============================================================================
			__ __ ____  ______   ______                 __
		   / //_// __ \/ ____/  / ____/   _____  ____  / /______
		  / ,<  / / / / __/    / __/ | | / / _ \/ __ \/ __/ ___/
		 / /| |/ /_/ / /___   / /___ | |/ /  __/ / / / /_(__  )
		/_/ |_/_____/_____/  /_____/ |___/\___/_/ /_/\__/____/
=============================================================================*/
#elif defined(__human68k__) || defined(WSAMIGA)
/*
	X68000/Amiga GUI+structure definitions
*/
#if !defined(guipos_t)
	#define guipos_t int
	typedef struct WSRECT
	{ /* Match QRect */
		int left;
		int top;
		int right;
		int bottom;
	} WSRECT;
#endif
typedef struct WSEVENT
{
	void* mhWnd;
	void* mEvent;
} WSEVENT;
typedef WSEVENT WSSIZEEVENT;
typedef WSEVENT WSPAINTEVENT;
typedef WSEVENT WSKEYEVENT;
typedef WSEVENT WSCLOSEEVENT;
typedef WSEVENT WSNOTIFYICONEVENT;
typedef struct WSMENUEVENT
{
	void* mhWnd;
} WSMENUEVENT;
typedef WSEVENT WSDESTROYEVENT;

/*
	X68000/Amiga event map
*/
void WSMenuMessages(void* pWindow, unsigned int desiredcommand);
#define BEGIN_ALL_MENU_MESSAGES void WSMenuMessages(\
	void* pWindow, unsigned int desiredcommand) \
{ \
	WSMENUEVENT ev; ev.mhWnd = pWindow; \
	switch (desiredcommand) \
	{
#define BEGIN_MENU_MESSAGES(name)
#define DECLARE_MENU_MESSAGE(command, proc) \
		case command: \
		{ \
			return proc(&ev); \
		}
#define END_MENU_MESSAGES(name)
#define END_ALL_MENU_MESSAGES \
	} \
}

wsbool WSWindowMessages(void* pWindow, unsigned int msg, void* qev);
#define BEGIN_ALL_WINDOW_MESSAGES \
wsbool WSWindowMessages(void* pWindow, unsigned int msg, void* qev) \
{ \
	WSEVENT ev; ev.mhWnd = pWindow; ev.mEvent = qev; \
	if(0) {}
#define BEGIN_WINDOW_MESSAGES(name)
#define IMPLEMENT_MENU_WINDOW_MESSAGES(name)
#define IMPLEMENT_SIZE_CHANGED_MESSAGE(name) \
	else if(msg == WM_WSSIZE) \
		return name(&ev);
#define IMPLEMENT_CLOSE_MESSAGE(name) \
	else if(msg == WM_WSCLOSE) \
		return name(&ev);
#define IMPLEMENT_DESTROYED_MESSAGE(name) \
	else if(msg == WM_WSDESTROY) \
		return name(&ev);
#define IMPLEMENT_NOTIFYICON_MESSAGE(name, command) \
	else if(msg == command + WM_WSNOTIFYICON) \
		return name(&ev);
#define IMPLEMENT_HANDLEIDLE_MESSAGE(name)
#define IMPLEMENT_HANDLECONTENT_MESSAGE(name)
#define IMPLEMENT_HANDLEGROW_MESSAGE(name)
#define IMPLEMENT_HANDLEZOOM_MESSAGE(name)
#define IMPLEMENT_KEYDOWN_MESSAGE(name)
	else if(msg == WM_WSKEYDOWN) \
		return name(&ev);
#define IMPLEMENT_HANDLEACTIVATE_MESSAGE(name)
#define IMPLEMENT_HANDLEDEACTIVATE_MESSAGE(name)
#define IMPLEMENT_HANDLEUPDATE_MESSAGE(name)
#define IMPLEMENT_HANDLEIBEAM_MESSAGE(name)
#define IMPLEMENT_HANDLEMOUSEWHEEL_MESSAGE(name)
#define IMPLEMENT_FALLBACK_MESSAGE(name)
#define END_WINDOW_MESSAGES(name)
#define END_ALL_WINDOW_MESSAGES \
}

/*=============================================================================
			__ __ ____  ______   ______                 __
		   / //_// __ \/ ____/  / ____/   _____  ____  / /______
		  / ,<  / / / / __/    / __/ | | / / _ \/ __ \/ __/ ___/
		 / /| |/ /_/ / /___   / /___ | |/ /  __/ / / / /_(__  )
		/_/ |_/_____/_____/  /_____/ |___/\___/_/ /_/\__/____/
=============================================================================*/
#elif defined(WSKDEOPT)
/*
	KDE GUI+structure definitions
*/
#if !defined(guipos_t)
	#define guipos_t int
	typedef struct WSRECT
	{ /* Match QRect */
		int left;
		int top;
		int right;
		int bottom;
	} WSRECT;
#endif

typedef struct WSEVENT
{
	void* mhWnd;
	void* mEvent;
} WSEVENT;
typedef WSEVENT WSSIZEEVENT;
typedef WSEVENT WSPAINTEVENT;
typedef WSEVENT WSKEYEVENT;
typedef WSEVENT WSCLOSEEVENT;
typedef WSEVENT WSNOTIFYICONEVENT;
typedef struct WSMENUEVENT
{
	void* mhWnd;
} WSMENUEVENT;
typedef WSEVENT WSDESTROYEVENT;
/*
	KDE Message map
*/
	LPWSMenuCallback WSMenuMessages(unsigned int desiredcommand);
	#define BEGIN_ALL_MENU_MESSAGES
	#define BEGIN_MENU_MESSAGES(name) LPWSMenuCallback name(\
		unsigned int desiredcommand) \
	{ \
		switch (desiredcommand) \
		{
	#define DECLARE_MENU_MESSAGE(command, proc) \
			case command: \
			{ \
				return proc; \
			}
	#define END_MENU_MESSAGES(name) \
		} \
		WSASSERT(0); return WSMenuSectionDefaultCallback; \
	}
	#define END_ALL_MENU_MESSAGES

	typedef void* (*LPWSWindowMessages)(unsigned int msg);
	#define WSWINDOWMESSAGEARGSDECLARE unsigned int msg
	#define BEGIN_ALL_WINDOW_MESSAGES
	#define BEGIN_WINDOW_MESSAGES(name) \
	extern "C" void* name(unsigned int msg) \
	{
	#define IMPLEMENT_MENU_WINDOW_MESSAGES(name) \
		if(msg >= WM_WSMENU) return (void*)name(msg - WM_WSMENU);
	#define IMPLEMENT_SIZE_CHANGED_MESSAGE(name) \
		if(msg == WM_WSSIZE) return (void*)name;
	#define IMPLEMENT_CLOSE_MESSAGE(name) \
		if(msg == WM_WSCLOSE) return (void*)name;
	#define IMPLEMENT_DESTROYED_MESSAGE(name) \
		if(msg == WM_WSDESTROY) return (void*)name;
	#define IMPLEMENT_NOTIFYICON_MESSAGE(name, command) \
		if(msg == WM_WSNOTIFYICON + command) return (void*)name;
	#define IMPLEMENT_HANDLEIDLE_MESSAGE(name)
	#define IMPLEMENT_HANDLECONTENT_MESSAGE(name)
	#define IMPLEMENT_HANDLEGROW_MESSAGE(name)
	#define IMPLEMENT_HANDLEZOOM_MESSAGE(name)
	#define IMPLEMENT_KEYDOWN_MESSAGE(name)
	#define IMPLEMENT_HANDLEACTIVATE_MESSAGE(name)
	#define IMPLEMENT_HANDLEDEACTIVATE_MESSAGE(name)
	#define IMPLEMENT_HANDLEUPDATE_MESSAGE(name)
	#define IMPLEMENT_HANDLEIBEAM_MESSAGE(name)
	#define IMPLEMENT_HANDLEMOUSEWHEEL_MESSAGE(name)
	#define IMPLEMENT_FALLBACK_MESSAGE(name)
	#define END_WINDOW_MESSAGES(name) \
		return (void*)WSWindowDefaultCallback; \
	}
	#define END_ALL_WINDOW_MESSAGES

/*
	KDE(Qt) Key Codes
*/
#define WSKEY_LEFT                0x01000012 /* Qt::Key_Left */
#define WSKEY_RIGHT               0x01000014 /* Qt::Key_Right */
#define WSKEY_UP                  0x01000013 /* Qt::Key_Up */
#define WSKEY_DOWN                0x01000015 /* Qt::Key_Down */
#define WSKEY_RETURN              0x01000004 /* Qt::Key_Return */
#define WSKEY_ESCAPE              0x01000000 /* Qt::Key_Escape */
#define WSKEY_SPACE               0x20       /* Qt::Key_Space */
#define WSKEY_BACK                0x01000003 /* Qt::Key_Backspace */
#define WSKEY_TAB                 0x01000001 /* Qt::Key_Tab */
#define WSKEY_SHIFT               0x01000020 /* Qt::Key_Shift */
#define WSKEY_CONTROL             0x01000021 /* Qt::Key_Control */
#define WSKEY_MENU                0x01000023 /* Qt::Key_Alt */
#define WSKEY_PAUSE               0x01000008 /* Qt::Key_Pause */
#define WSKEY_CAPITAL             0x01000024 /* Qt::Key_CapsLock */
#define WSKEY_PRIOR               0x01000016 /* Qt::Key_PageUp */
#define WSKEY_NEXT                0x01000017 /* Qt::Key_PageDown */
#define WSKEY_END                 0x01000011 /* Qt::Key_End */
#define WSKEY_HOME                0x01000010 /* Qt::Key_Home */
#define WSKEY_SELECT              0x01000062 /* Qt::Key_Select */
#define WSKEY_PRINT               0x01000018 /* Qt::Key_Print */
#define WSKEY_EXECUTE             0x01000063 /* Qt::Key_Execute */
#define WSKEY_INSERT              0x01000006 /* Qt::Key_Insert */
#define WSKEY_DELETE              0x01000007 /* Qt::Key_Delete */
#define WSKEY_HELP                0x01000061 /* Qt::Key_Help */
#define WSKEY_0                   0x30       /* Qt::Key_0 */
#define WSKEY_1                   0x31       /* Qt::Key_1 */
#define WSKEY_2                   0x32       /* Qt::Key_2 */
#define WSKEY_3                   0x33       /* Qt::Key_3 */
#define WSKEY_4                   0x34       /* Qt::Key_4 */
#define WSKEY_5                   0x35       /* Qt::Key_5 */
#define WSKEY_6                   0x36       /* Qt::Key_6 */
#define WSKEY_7                   0x37       /* Qt::Key_7 */
#define WSKEY_8                   0x38       /* Qt::Key_8 */
#define WSKEY_9                   0x39       /* Qt::Key_9 */
#define WSKEY_A                   0x41       /* Qt::Key_A */
#define WSKEY_B                   0x42       /* Qt::Key_B */
#define WSKEY_C                   0x43       /* Qt::Key_C */
#define WSKEY_D                   0x44       /* Qt::Key_D */
#define WSKEY_E                   0x45       /* Qt::Key_E */
#define WSKEY_F                   0x46       /* Qt::Key_F */
#define WSKEY_G                   0x47       /* Qt::Key_G */
#define WSKEY_H                   0x48       /* Qt::Key_H */
#define WSKEY_I                   0x49       /* Qt::Key_I */
#define WSKEY_J                   0x4A       /* Qt::Key_J */
#define WSKEY_K                   0x4B       /* Qt::Key_K */
#define WSKEY_L                   0x4C       /* Qt::Key_L */
#define WSKEY_M                   0x4D       /* Qt::Key_M */
#define WSKEY_N                   0x4E       /* Qt::Key_N */
#define WSKEY_O                   0x4F       /* Qt::Key_O */
#define WSKEY_P                   0x50       /* Qt::Key_P */
#define WSKEY_Q                   0x51       /* Qt::Key_Q */
#define WSKEY_R                   0x52       /* Qt::Key_R */
#define WSKEY_S                   0x53       /* Qt::Key_S */
#define WSKEY_T                   0x54       /* Qt::Key_T */
#define WSKEY_U                   0x55       /* Qt::Key_U */
#define WSKEY_V                   0x56       /* Qt::Key_V */
#define WSKEY_W                   0x57       /* Qt::Key_W */
#define WSKEY_X                   0x58       /* Qt::Key_X */
#define WSKEY_Y                   0x59       /* Qt::Key_Y */
#define WSKEY_Z                   0x5A       /* Qt::Key_Z */
#define WSKEY_LWIN                0x01000083 /* Qt::Key_Meta */
#define WSKEY_RWIN                0x01001103 /* Qt::Key_Meta */
#define WSKEY_NUMPAD0             0x01000030 /* Qt::Key_0 */
#define WSKEY_NUMPAD1             0x01000031 /* Qt::Key_1 */
#define WSKEY_NUMPAD2             0x01000032 /* Qt::Key_2 */
#define WSKEY_NUMPAD3             0x01000033 /* Qt::Key_3 */
#define WSKEY_NUMPAD4             0x01000034 /* Qt::Key_4 */
#define WSKEY_NUMPAD5             0x01000035 /* Qt::Key_5 */
#define WSKEY_NUMPAD6             0x01000036 /* Qt::Key_6 */
#define WSKEY_NUMPAD7             0x01000037 /* Qt::Key_7 */
#define WSKEY_NUMPAD8             0x01000038 /* Qt::Key_8 */
#define WSKEY_NUMPAD9             0x01000039 /* Qt::Key_9 */
#define WSKEY_MULTIPLY            0x0100002A /* Qt::Key_Asterisk */
#define WSKEY_ADD                 0x0100002B /* Qt::Key_Plus */
#define WSKEY_SEPARATOR           0x0100002C /* Qt::Key_Comma */
#define WSKEY_SUBTRACT            0x0100002D /* Qt::Key_Minus */
#define WSKEY_DECIMAL             0x0100002E /* Qt::Key_Period */
#define WSKEY_DIVIDE              0x0100002F /* Qt::Key_Slash */
#define WSKEY_F1                  0x01000030 /* Qt::Key_F1 */
#define WSKEY_F2                  0x01000031 /* Qt::Key_F2 */
#define WSKEY_F3                  0x01000032 /* Qt::Key_F3 */
#define WSKEY_F4                  0x01000033 /* Qt::Key_F4 */
#define WSKEY_F5                  0x01000034 /* Qt::Key_F5 */
#define WSKEY_F6                  0x01000035 /* Qt::Key_F6 */
#define WSKEY_F7                  0x01000036 /* Qt::Key_F7 */
#define WSKEY_F8                  0x01000037 /* Qt::Key_F8 */
#define WSKEY_F9                  0x01000038 /* Qt::Key_F9 */
#define WSKEY_F10                 0x01000039 /* Qt::Key_F10 */
#define WSKEY_F11                 0x0100003A /* Qt::Key_F11 */
#define WSKEY_F12                 0x0100003B /* Qt::Key_F12 */
#define WSKEY_F13                 0x0100003C /* Qt::Key_F13 */
#define WSKEY_F14                 0x0100003D /* Qt::Key_F14 */
#define WSKEY_F15                 0x0100003E /* Qt::Key_F15 */
#define WSKEY_F16                 0x0100003F /* Qt::Key_F16 */
#define WSKEY_F17                 0x01000040 /* Qt::Key_F17 */
#define WSKEY_F18                 0x01000041 /* Qt::Key_F18 */
#define WSKEY_F19                 0x01000042 /* Qt::Key_F19 */
#define WSKEY_F20                 0x01000043 /* Qt::Key_F20 */
#define WSKEY_F21                 0x01000044 /* Qt::Key_F21 */
#define WSKEY_F22                 0x01000045 /* Qt::Key_F22 */
#define WSKEY_F23                 0x01000046 /* Qt::Key_F23 */
#define WSKEY_F24                 0x01000047 /* Qt::Key_F24 */
#define WSKEY_NUMLOCK             0x01000025 /* Qt::Key_NumLock */
#define WSKEY_SCROLL              0x01000026 /* Qt::Key_ScrollLock */
#define WSKEY_LSHIFT              0x01000020 /* Qt::Key_Shift */
#define WSKEY_RSHIFT              0x01000020 /* Qt::Key_Shift */
#define WSKEY_LCONTROL            0x01000021 /* Qt::Key_Control */
#define WSKEY_RCONTROL            0x01000021 /* Qt::Key_Control */
#define WSKEY_LMENU               0x01000023 /* Qt::Key_Alt */
#define WSKEY_RMENU               0x01000023 /* Qt::Key_Alt */
#define WSKEY_VOLUME_MUTE         0x01000099 /* Qt::Key_VolumeMute */
#define WSKEY_VOLUME_DOWN         0x01000097 /* Qt::Key_VolumeDown */
#define WSKEY_VOLUME_UP           0x01000098 /* Qt::Key_VolumeUp */
#define WSKEY_MEDIA_NEXT_TRACK    0x0100009B /* Qt::Key_MediaNext */
#define WSKEY_MEDIA_PREV_TRACK    0x0100009A /* Qt::Key_MediaPrevious */
#define WSKEY_MEDIA_STOP          0x0100009E /* Qt::Key_MediaStop */
#define WSKEY_MEDIA_PLAY_PAUSE    0x0100009D /* Qt::Key_MediaPlay */
#define WSKEY_LAUNCH_MAIL         0x010000A4 /* Qt::Key_LaunchMail */
#define WSKEY_LAUNCH_APP1         0x010000A5 /* Qt::Key_Launch1 */
#define WSKEY_LAUNCH_APP2         0x010000A6 /* Qt::Key_Launch2 */
#define WSKEY_OEM_1               0x003B /* Qt::Key_Semicolon */
#define WSKEY_OEM_PLUS            0x003D /* Qt::Key_Equal */
#define WSKEY_OEM_COMMA           0x002C /* Qt::Key_Comma */
#define WSKEY_OEM_MINUS           0x002D /* Qt::Key_Minus */
#define WSKEY_OEM_PERIOD          0x002E /* Qt::Key_Period */
#define WSKEY_OEM_2               0x002F /* Qt::Key_Slash */
#define WSKEY_OEM_3               0x0060 /* Qt::Key_QuoteLeft */
#define WSKEY_OEM_4               0x005B /* Qt::Key_BracketLeft */
#define WSKEY_OEM_5               0x005C /* Qt::Key_Backslash */
#define WSKEY_OEM_6               0x005D /* Qt::Key_BracketRight */
#define WSKEY_OEM_7               0x0027 /* Qt::Key_Apostrophe */
#define WSKEY_PLAY                0x010000C8 /* Qt::Key_Play */
#define WSKEY_ZOOM                0x0100004A /* Qt::Key_Zoom */
#define WSKEY_GRAVE              0x0060 /* Qt::Key_QuoteLeft */
#define WSKEY_APOSTROPHE         0x0027 /* Qt::Key_Apostrophe */
#define WSKEY_BACKSLASH          0x005C /* Qt::Key_Backslash */
#define WSKEY_BRACKETLEFT        0x005B /* Qt::Key_BracketLeft */
#define WSKEY_BRACKETRIGHT       0x005D /* Qt::Key_BracketRight */
#define WSKEY_COMMA              0x002C /* Qt::Key_Comma */
#define WSKEY_EQUAL              0x003D /* Qt::Key_Equal */
#define WSKEY_MINUS              0x002D /* Qt::Key_Minus */
#define WSKEY_PERIOD             0x002E /* Qt::Key_Period */
#define WSKEY_SEMICOLON          0x003B /* Qt::Key_Semicolon */
#define WSKEY_SLASH              0x002F /* Qt::Key_Slash */
#define WSKEY_SPACE              0x0020 /* Qt::Key_Space */
#define WSKEY_QUOTEDBL           0x0022 /* Qt::Key_QuoteDbl */
#define WSKEY_HASH               0x0023 /* Qt::Key_NumberSign */
#define WSKEY_DOLLAR             0x0024 /* Qt::Key_Dollar */
#define WSKEY_PERCENT            0x0025 /* Qt::Key_Percent */
#define WSKEY_AMPERSAND          0x0026 /* Qt::Key_Ampersand */
#define WSKEY_PARENLEFT          0x0028 /* Qt::Key_ParenLeft */
#define WSKEY_PARENRIGHT         0x0029 /* Qt::Key_ParenRight */
#define WSKEY_ASTERISK           0x002A /* Qt::Key_Asterisk */
#define WSKEY_PLUS               0x002B /* Qt::Key_Plus */
#define WSKEY_LESS               0x003C /* Qt::Key_Less */
#define WSKEY_GREATER            0x003E /* Qt::Key_Greater */
#define WSKEY_QUESTION           0x003F /* Qt::Key_Question */
#define WSKEY_AT                 0x0040 /* Qt::Key_At */
#define WSKEY_CARET              0x005E /* Qt::Key_AsciiCircum */
#define WSKEY_UNDERSCORE         0x005F /* Qt::Key_Underscore */
#define WSKEY_EXCLAMATION        0x0021 /* Qt::Key_Exclam */
#define WSKEY_TILDE              0x007E /* Qt::Key_AsciiTilde */

/*=============================================================================
			   ________  __       ______                 __
			  / ____/ /_/ /__    / ____/   _____  ____  / /______
			 / / __/ __/ //_/   / __/ | | / / _ \/ __ \/ __/ ___/
			/ /_/ / /_/ ,<     / /___ | |/ /  __/ / / / /_(__  )
			\____/\__/_/|_|   /_____/ |___/\___/_/ /_/\__/____/
=============================================================================*/
#else
/*
	GTK GUI+Event types
*/
#if !defined(guipos_t)
	#define guipos_t int
	typedef struct WSRECT
	{ /* Match GdkRect - x,y,width,height */
		guipos_t left;
		guipos_t top;
		guipos_t width;
		guipos_t height;
	} WSRECT;
	#define WSRECT_HASWIDTHHEIGHT
#endif

#if defined(WSGTK4OPT) && !defined(__MACH__)
	typedef struct _GtkWidget GtkWidget;
	typedef struct _GSimpleAction GSimpleAction;
	typedef struct _GVariant GVariant;
	typedef struct _GdkEvent GdkEvent;
	#define WSMENUEVENT_CALLBACK(x) void (x)(GSimpleAction* action, \
				GVariant* acparam, WSWINDOW* cd)
	#define WSEVENT_CALLBACK(x) void (x)(GtkWidget* widget,\
		GdkEvent* gdkevent, WSWINDOW* cd)
	#define WSCLOSEEVENT_CALLBACK(x) wsbool (x)(GtkWidget* widget,\
		GdkEvent* gdkevent, WSWINDOW* cd)
	#define WSMenuEvent_Construct(pThis) { \
		(pThis)->mpWindow = cd; (pThis)->action = action; \
		(pThis)->acparam = acparam; }
	typedef struct WSMENUEVENT
	{
		WSWINDOW* mpWindow;
		GSimpleAction* action;
		GVariant* acparam;
	} WSMENUEVENT;

	typedef struct WSSIZEEVENT
	{
		GtkWidget* widget;
		int width, height, baseline;
	} WSSIZEEVENT;
	#define WSSIZEEVENT_CALLBACK(x) wsbool (x)(\
		GtkWidget *widget, int width, int height, int baseline)
	#define WSSizeEvent_Construct(pThis) { \
		(pThis)->widget = widget; (pThis)->width = width; \
		(pThis)->height = height; (pThis)->baseline = baseline; }
#elif !defined(__MACH__)
	typedef struct _GtkMenu GtkMenu;
	typedef struct _GtkMenuBar GtkMenuBar;
	typedef struct _GtkWidget GtkWidget;
	typedef union _GdkEvent GdkEvent;
	typedef struct _GtkAccelGroup GtkAccelGroup;
	#define WSMENUEVENT_CALLBACK(x) \
		void (x)(GtkWidget* widget, WSWINDOW* cd)
	#define WSEVENT_CALLBACK(x) void (x)(GtkWidget* widget,\
		GdkEvent* gdkevent, WSWINDOW* cd)
	#define WSCLOSEEVENT_CALLBACK(x) wsbool (x)(GtkWidget* widget,\
		GdkEvent* gdkevent, WSWINDOW* cd)
	#define WSMenuEvent_Construct(pThis) { \
		(pThis)->mpWindow = cd; (pThis)->widget = widget; }

	typedef struct WSMENUEVENT
	{
		WSWINDOW* mpWindow;
		GtkWidget* widget;
	} WSMENUEVENT;

	#define WSSIZEEVENT_CALLBACK(x) wsbool (x)(\
		GtkWidget* widget, WSRECT* allocation)
	#define WSSizeEvent_Construct(pThis) { \
		(pThis)->widget = widget; (pThis)->rect = allocation; }
	typedef struct WSSIZEEVENT
	{
		GtkWidget* widget;
		WSRECT* rect;
	} WSSIZEEVENT;

#endif

typedef struct WSEVENT
{
	GtkWidget* mhWnd;
	GdkEvent* mEvent;
} WSEVENT;
typedef WSEVENT WSPAINTEVENT;
typedef WSEVENT WSKEYEVENT;
typedef WSEVENT WSCLOSEEVENT;
typedef struct WSNOTIFYICONEVENT
{
	GtkWidget* widget;
} WSNOTIFYICONEVENT;
typedef WSEVENT WSDESTROYEVENT;
typedef WSEVENT WSPAINTEVENT;
typedef WSEVENT WSKEYEVENT;
typedef WSEVENT WSCLOSEEVENT;
typedef WSEVENT WSDESTROYEVENT;
typedef WSEVENT WSMOUSEWHEELEVENT;
typedef WSEVENT WSIDLEEVENT;
typedef WSEVENT WSCONTENTEVENT;
typedef WSEVENT WSGROWEVENT;
typedef WSEVENT WSZOOMEVENT;
typedef WSEVENT WSACTIVATEEVENT;
typedef WSEVENT WSDEACTIVATEEVENT;
typedef WSEVENT WSIBEAMEVENT;

#define WSEVENT_HANDLED
#define WSEVENT_UNHANDLED
#define WSEvent_Construct(pThis) { (pThis)->mhWnd = widget; \
	(pThis)->mEvent = gdkevent; }

#define WSSIZEEVENT_HANDLED TRUE
#define WSSIZEEVENT_UNHANDLED FALSE

#define WSPAINTEVENT_CALLBACK WSEVENT_CALLBACK
#define WSPAINTEVENT_HANDLED WSEVENT_HANDLED
#define WSPAINTEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSPaintEvent_Construct WSEvent_Construct

#define WSKEYEVENT_CALLBACK WSEVENT_CALLBACK
#define WSKEYEVENT_HANDLED WSEVENT_HANDLED
#define WSKEYEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSKeyEvent_Construct WSEvent_Construct

#define WSCLOSEEVENT_HANDLED TRUE
#define WSCLOSEEVENT_UNHANDLED FALSE
#define WSCloseEvent_Construct WSEvent_Construct

#define WSNOTIFYICONEVENT_CALLBACK(x) void (x)(GtkWidget* widget)
#define WSNOTIFYICONEVENT_HANDLED WSEVENT_HANDLED
#define WSNOTIFYICONEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSNotifyIconEvent_Construct(pThis) { (pThis)->widget = widget; }

#define WSMENUEVENT_HANDLED WSEVENT_HANDLED
#define WSMENUEVENT_UNHANDLED WSEVENT_UNHANDLED

#define WSDESTROYEVENT_CALLBACK WSEVENT_CALLBACK
#define WSDESTROYEVENT_HANDLED WSEVENT_HANDLED
#define WSDESTROYEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSDestroyEvent_Construct WSEvent_Construct

#define WSMOUSEWHEELEVENT_CALLBACK WSEVENT_CALLBACK
#define WSMOUSEWHEELEVENT_HANDLED WSEVENT_HANDLED
#define WSMOUSEWHEELEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSMouseWheelEvent_Construct WSEvent_Construct

#define WSCONTENTEVENT_CALLBACK WSEVENT_CALLBACK
#define WSCONTENTEVENT_HANDLED WSEVENT_HANDLED
#define WSCONTENTEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSContentEvent_Construct WSEvent_Construct

#define WSGROWEVENT_CALLBACK WSEVENT_CALLBACK
#define WSGROWEVENT_HANDLED WSEVENT_HANDLED
#define WSGROWEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSGrowEvent_Construct WSEvent_Construct

#define WSZOOMEVENT_CALLBACK WSEVENT_CALLBACK
#define WSZOOMEVENT_HANDLED WSEVENT_HANDLED
#define WSZOOMEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSZoomEvent_Construct WSEvent_Construct

#define WSIDLEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSIDLEEVENT_HANDLED WSEVENT_HANDLED
#define WSIDLEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSIdleEvent_Construct WSEvent_Construct

#define WSACTIVATEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSACTIVATEEVENT_HANDLED WSEVENT_HANDLED
#define WSACTIVATEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSActivateEvent_Construct WSEvent_Construct

#define WSDEACTIVATEEVENT_CALLBACK WSEVENT_CALLBACK
#define WSDEACTIVATEEVENT_HANDLED WSEVENT_HANDLED
#define WSDEACTIVATEEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSDeactivateEvent_Construct WSEvent_Construct

#define WSIBEAMEVENT_CALLBACK WSEVENT_CALLBACK
#define WSIBEAMEVENT_HANDLED WSEVENT_HANDLED
#define WSIBEAMEVENT_UNHANDLED WSEVENT_UNHANDLED
#define WSIBeamEvent_Construct WSEvent_Construct

/*
	GTK Message map
*/
#define BEGIN_ALL_MENU_MESSAGES
#define BEGIN_MENU_MESSAGES(name)
#define DECLARE_MENU_MESSAGE(command, proc)
#define END_MENU_MESSAGES(name)
#define END_ALL_MENU_MESSAGES

typedef void* (*LPWSWindowMessages)(unsigned int msg);
#define WSWINDOWMESSAGEARGSDECLARE unsigned int msg
#define BEGIN_ALL_WINDOW_MESSAGES
#define BEGIN_WINDOW_MESSAGES(name) \
void* name(unsigned int msg) \
{
#define IMPLEMENT_MENU_WINDOW_MESSAGES(name)
#define IMPLEMENT_SIZE_CHANGED_MESSAGE(name) \
	if(msg == WM_WSSIZE) return (void*)name;
#define IMPLEMENT_CLOSE_MESSAGE(name) \
	if(msg == WM_WSCLOSE) return (void*)name;
#define IMPLEMENT_DESTROYED_MESSAGE(name) \
	if(msg == WM_WSDESTROY) return (void*)name;
#define IMPLEMENT_NOTIFYICON_MESSAGE(name, command) \
	if(msg == WM_WSNOTIFYICON + command) return (void*)name;
#define IMPLEMENT_HANDLEIDLE_MESSAGE(name)
#define IMPLEMENT_HANDLECONTENT_MESSAGE(name)
#define IMPLEMENT_HANDLEGROW_MESSAGE(name)
#define IMPLEMENT_HANDLEZOOM_MESSAGE(name)
#define IMPLEMENT_KEYDOWN_MESSAGE(name) \
	if(msg == WM_WSKEYDOWN) return (void*)name;
#define IMPLEMENT_HANDLEACTIVATE_MESSAGE(name)
#define IMPLEMENT_HANDLEDEACTIVATE_MESSAGE(name)
#define IMPLEMENT_HANDLEUPDATE_MESSAGE(name)
#define IMPLEMENT_HANDLEIBEAM_MESSAGE(name)
#define IMPLEMENT_HANDLEMOUSEWHEEL_MESSAGE(name)
#define IMPLEMENT_PAINT_MESSAGE(name)
#define IMPLEMENT_FALLBACK_MESSAGE(name)
#define END_WINDOW_MESSAGES(name) \
	return (void*)WSWindowDefaultCallback; \
}
#define END_ALL_WINDOW_MESSAGES

/*
	GTK Key Codes
*/
#define WSKEY_LEFT                0xFF51 /* GDK_KEY_Left */
#define WSKEY_RIGHT               0xFF53 /* GDK_KEY_Right */
#define WSKEY_UP                  0xFF52 /* GDK_KEY_Up */
#define WSKEY_DOWN                0xFF54 /* GDK_KEY_Down */
#define WSKEY_RETURN              0xFF0D /* GDK_KEY_Return */
#define WSKEY_ESCAPE              0xFF1B /* GDK_KEY_Escape */
#define WSKEY_BACK                0xFF08 /* GDK_KEY_BackSpace */
#define WSKEY_TAB                 0xFF09 /* GDK_KEY_Tab */
#define WSKEY_SHIFT               0xFFE1 /* GDK_KEY_Shift_L */
#define WSKEY_CONTROL             0xFFE3 /* GDK_KEY_Control_L */
#define WSKEY_MENU                0xFFE9 /* GDK_KEY_Alt_L */
#define WSKEY_PAUSE               0xFF13 /* GDK_KEY_Pause */
#define WSKEY_CAPITAL             0xFFE5 /* GDK_KEY_Caps_Lock */
#define WSKEY_PRIOR               0xFF55 /* GDK_KEY_Page_Up */
#define WSKEY_NEXT                0xFF56 /* GDK_KEY_Page_Down */
#define WSKEY_END                 0xFF57 /* GDK_KEY_End */
#define WSKEY_HOME                0xFF50 /* GDK_KEY_Home */
#define WSKEY_SELECT              0xFF60 /* GDK_KEY_Select */
#define WSKEY_PRINT               0xFF61 /* GDK_KEY_Print */
#define WSKEY_EXECUTE             0xFF62 /* GDK_KEY_Execute */
#define WSKEY_INSERT              0xFF63 /* GDK_KEY_Insert */
#define WSKEY_DELETE              0xFFFF /* GDK_KEY_Delete */
#define WSKEY_HELP                0xFF6A /* GDK_KEY_Help */
#define WSKEY_0                   0x030 /* GDK_KEY_0 */
#define WSKEY_1                   0x031 /* GDK_KEY_1 */
#define WSKEY_2                   0x032 /* GDK_KEY_2 */
#define WSKEY_3                   0x033 /* GDK_KEY_3 */
#define WSKEY_4                   0x034 /* GDK_KEY_4 */
#define WSKEY_5                   0x035 /* GDK_KEY_5 */
#define WSKEY_6                   0x036 /* GDK_KEY_6 */
#define WSKEY_7                   0x037 /* GDK_KEY_7 */
#define WSKEY_8                   0x038 /* GDK_KEY_8 */
#define WSKEY_9                   0x039 /* GDK_KEY_9 */
#define WSKEY_A                   0x041 /* GDK_KEY_A */
#define WSKEY_B                   0x042 /* GDK_KEY_B */
#define WSKEY_C                   0x043 /* GDK_KEY_C */
#define WSKEY_D                   0x044 /* GDK_KEY_D */
#define WSKEY_E                   0x045 /* GDK_KEY_E */
#define WSKEY_F                   0x046 /* GDK_KEY_F */
#define WSKEY_G                   0x047 /* GDK_KEY_G */
#define WSKEY_H                   0x048 /* GDK_KEY_H */
#define WSKEY_I                   0x049 /* GDK_KEY_I */
#define WSKEY_J                   0x04A /* GDK_KEY_J */
#define WSKEY_K                   0x04B /* GDK_KEY_K */
#define WSKEY_L                   0x04C /* GDK_KEY_L */
#define WSKEY_M                   0x04D /* GDK_KEY_M */
#define WSKEY_N                   0x04E /* GDK_KEY_N */
#define WSKEY_O                   0x04F /* GDK_KEY_O */
#define WSKEY_P                   0x050 /* GDK_KEY_P */
#define WSKEY_Q                   0x051 /* GDK_KEY_Q */
#define WSKEY_R                   0x052 /* GDK_KEY_R */
#define WSKEY_S                   0x053 /* GDK_KEY_S */
#define WSKEY_T                   0x054 /* GDK_KEY_T */
#define WSKEY_U                   0x055 /* GDK_KEY_U */
#define WSKEY_V                   0x056 /* GDK_KEY_V */
#define WSKEY_W                   0x057 /* GDK_KEY_W */
#define WSKEY_X                   0x058 /* GDK_KEY_X */
#define WSKEY_Y                   0x059 /* GDK_KEY_Y */
#define WSKEY_Z                   0x05A /* GDK_KEY_Z */
#define WSKEY_LWIN                0xFFEB /* GDK_KEY_Super_L */
#define WSKEY_RWIN                0xFFEC /* GDK_KEY_Super_R */
#define WSKEY_NUMPAD0             0xFFB0 /* GDK_KEY_KP_0 */
#define WSKEY_NUMPAD1             0xFFB1 /* GDK_KEY_KP_1 */
#define WSKEY_NUMPAD2             0xFFB2 /* GDK_KEY_KP_2 */
#define WSKEY_NUMPAD3             0xFFB3 /* GDK_KEY_KP_3 */
#define WSKEY_NUMPAD4             0xFFB4 /* GDK_KEY_KP_4 */
#define WSKEY_NUMPAD5             0xFFB5 /* GDK_KEY_KP_5 */
#define WSKEY_NUMPAD6             0xFFB6 /* GDK_KEY_KP_6 */
#define WSKEY_NUMPAD7             0xFFB7 /* GDK_KEY_KP_7 */
#define WSKEY_NUMPAD8             0xFFB8 /* GDK_KEY_KP_8 */
#define WSKEY_NUMPAD9             0xFFB9 /* GDK_KEY_KP_9 */
#define WSKEY_MULTIPLY            0xFFAA /* GDK_KEY_KP_Multiply */
#define WSKEY_ADD                 0xFFAB /* GDK_KEY_KP_Add */
#define WSKEY_SEPARATOR           0xFFAC /* GDK_KEY_KP_Separator */
#define WSKEY_SUBTRACT            0xFFAD /* GDK_KEY_KP_Subtract */
#define WSKEY_DECIMAL             0xFFAE /* GDK_KEY_KP_Decimal */
#define WSKEY_DIVIDE              0xFFAF /* GDK_KEY_KP_Divide */
#define WSKEY_F1                  0xFFBE /* GDK_KEY_F1 */
#define WSKEY_F2                  0xFFBF /* GDK_KEY_F2 */
#define WSKEY_F3                  0xFFC0 /* GDK_KEY_F3 */
#define WSKEY_F4                  0xFFC1 /* GDK_KEY_F4 */
#define WSKEY_F5                  0xFFC2 /* GDK_KEY_F5 */
#define WSKEY_F6                  0xFFC3 /* GDK_KEY_F6 */
#define WSKEY_F7                  0xFFC4 /* GDK_KEY_F7 */
#define WSKEY_F8                  0xFFC5 /* GDK_KEY_F8 */
#define WSKEY_F9                  0xFFC6 /* GDK_KEY_F9 */
#define WSKEY_F10                 0xFFC7 /* GDK_KEY_F10 */
#define WSKEY_F11                 0xFFC8 /* GDK_KEY_F11 */
#define WSKEY_F12                 0xFFC9 /* GDK_KEY_F12 */
#define WSKEY_F13                 0xFFCA /* GDK_KEY_F13 */
#define WSKEY_F14                 0xFFCB /* GDK_KEY_F14 */
#define WSKEY_F15                 0xFFCC /* GDK_KEY_F15 */
#define WSKEY_F16                 0xFFCD /* GDK_KEY_F16 */
#define WSKEY_F17                 0xFFCE /* GDK_KEY_F17 */
#define WSKEY_F18                 0xFFCF /* GDK_KEY_F18 */
#define WSKEY_F19                 0xFFD0 /* GDK_KEY_F19 */
#define WSKEY_F20                 0xFFD1 /* GDK_KEY_F20 */
#define WSKEY_F21                 0xFFD2 /* GDK_KEY_F21 */
#define WSKEY_F22                 0xFFD3 /* GDK_KEY_F22 */
#define WSKEY_F23                 0xFFD4 /* GDK_KEY_F23 */
#define WSKEY_F24                 0xFFD5 /* GDK_KEY_F24 */
#define WSKEY_NUMLOCK             0xFF7F /* GDK_KEY_Num_Lock */
#define WSKEY_SCROLL              0xFF14 /* GDK_KEY_Scroll_Lock */
#define WSKEY_LSHIFT              0xFFE1 /* GDK_KEY_Shift_L */
#define WSKEY_RSHIFT              0xFFE2 /* GDK_KEY_Shift_R */
#define WSKEY_LCONTROL            0xFFE3 /* GDK_KEY_Control_L */
#define WSKEY_RCONTROL            0xFFE4 /* GDK_KEY_Control_R */
#define WSKEY_LMENU               0xFFE9 /* GDK_KEY_Alt_L */
#define WSKEY_RMENU               0xFFEA /* GDK_KEY_Alt_R */
#define WSKEY_VOLUME_MUTE         0x1008FF12 /* GDK_KEY_AudioMute */
#define WSKEY_VOLUME_DOWN         0x1008FF11 /* GDK_KEY_AudioLowerVolume */
#define WSKEY_VOLUME_UP           0x1008FF13 /* GDK_KEY_AudioRaiseVolume */
#define WSKEY_MEDIA_NEXT_TRACK    0x1008FF17 /* GDK_KEY_AudioNext */
#define WSKEY_MEDIA_PREV_TRACK    0x1008FF16 /* GDK_KEY_AudioPrev */
#define WSKEY_MEDIA_STOP          0x1008FF15 /* GDK_KEY_AudioStop */
#define WSKEY_MEDIA_PLAY_PAUSE    0x1008FF14 /* GDK_KEY_AudioPlay */
#define WSKEY_LAUNCH_MAIL         0x1008FF19 /* GDK_KEY_Mail */
#define WSKEY_LAUNCH_APP1         0x1008FF81 /* GDK_KEY_Launch1 */
#define WSKEY_LAUNCH_APP2         0x1008FF82 /* GDK_KEY_Launch2 */
#define WSKEY_OEM_1               0x003B /* GDK_KEY_semicolon */
#define WSKEY_OEM_PLUS            0x003D /* GDK_KEY_equal */
#define WSKEY_OEM_COMMA           0x002C /* GDK_KEY_comma */
#define WSKEY_OEM_MINUS           0x002D /* GDK_KEY_minus */
#define WSKEY_OEM_PERIOD          0x002E /* GDK_KEY_period */
#define WSKEY_OEM_2               0x002F /* GDK_KEY_slash */
#define WSKEY_OEM_3               0x0060 /* GDK_KEY_grave */
#define WSKEY_OEM_4               0x005B /* GDK_KEY_bracketleft */
#define WSKEY_OEM_5               0x005C /* GDK_KEY_backslash */
#define WSKEY_OEM_6               0x005D /* GDK_KEY_bracketright */
#define WSKEY_OEM_7               0x0027 /* GDK_KEY_apostrophe */
#define WSKEY_PLAY                0x1008FF14 /* GDK_KEY_XF86AudioPlay */
#define WSKEY_ZOOM                0x1008FF2D /* GDK_KEY_ZoomIn */
#define WSKEY_GRAVE              0x0060 /* GDK_KEY_grave */
#define WSKEY_APOSTROPHE         0x0027 /* GDK_KEY_apostrophe */
#define WSKEY_BACKSLASH          0x005C /* GDK_KEY_backslash */
#define WSKEY_BRACKETLEFT        0x005B /* GDK_KEY_bracketleft */
#define WSKEY_BRACKETRIGHT       0x005D /* GDK_KEY_bracketright */
#define WSKEY_COMMA              0x002C /* GDK_KEY_comma */
#define WSKEY_EQUAL              0x003D /* GDK_KEY_equal */
#define WSKEY_MINUS              0x002D /* GDK_KEY_minus */
#define WSKEY_PERIOD             0x002E /* GDK_KEY_period */
#define WSKEY_SEMICOLON          0x003B /* GDK_KEY_semicolon */
#define WSKEY_SLASH              0x002F /* GDK_KEY_slash */
#define WSKEY_SPACE              0x0020 /* GDK_KEY_space */
#define WSKEY_QUOTEDBL           0x0022 /* GDK_KEY_quotedbl */
#define WSKEY_HASH               0x0023 /* GDK_KEY_numbersign */
#define WSKEY_DOLLAR             0x0024 /* GDK_KEY_dollar */
#define WSKEY_PERCENT            0x0025 /* GDK_KEY_percent */
#define WSKEY_AMPERSAND          0x0026 /* GDK_KEY_ampersand */
#define WSKEY_PARENLEFT          0x0028 /* GDK_KEY_parenleft */
#define WSKEY_PARENRIGHT         0x0029 /* GDK_KEY_parenright */
#define WSKEY_ASTERISK           0x002A /* GDK_KEY_asterisk */
#define WSKEY_PLUS               0x002B /* GDK_KEY_plus */
#define WSKEY_LESS               0x003C /* GDK_KEY_less */
#define WSKEY_GREATER            0x003E /* GDK_KEY_greater */
#define WSKEY_QUESTION           0x003F /* GDK_KEY_question */
#define WSKEY_AT                 0x0040 /* GDK_KEY_at */
#define WSKEY_CARET              0x005E /* GDK_KEY_asciicircum */
#define WSKEY_UNDERSCORE         0x005F /* GDK_KEY_underscore */
#define WSKEY_EXCLAMATION        0x0021 /* GDK_KEY_exclam */
#define WSKEY_TILDE              0x007E /* GDK_KEY_asciitilde */

#endif /* End events OS switch */

#if defined(__MACH__) /*&& defined(__OBJC__)*/
	#define WSRect_GetX(pThis) (pThis)->origin.x
	#define WSRect_GetY(pThis) (pThis)->origin.y
	#define WSRect_GetWidth(pThis) (pThis)->size.width
	#define WSRect_GetHeight(pThis) (pThis)->size.height
	#define WSRect_SetX(pThis, xin) (pThis)->origin.x = xin
	#define WSRect_SetY(pThis, x) (pThis)->origin.y = x
	#define WSRect_SetWidth(pThis, x) (pThis)->size.width = x
	#define WSRect_SetHeight(pThis, x) (pThis)->size.height = x
#else
	#define WSRect_GetX(pThis) (pThis)->left
	#define WSRect_GetY(pThis) (pThis)->top
	#define WSRect_SetX(pThis, x) (pThis)->left = x
	#define WSRect_SetY(pThis, x) (pThis)->top = x
	#if defined(WSRECT_HASWIDTHHEIGHT)
		#define WSRect_GetWidth(pThis) (pThis)->width
		#define WSRect_GetHeight(pThis) (pThis)->height
		#define WSRect_GetRight(pThis) (pThis)->width + (pThis)->left
		#define WSRect_GetBottom(pThis) (pThis)->height - (pThis)->top
		#define WSRect_SetWidth(pThis, x) (pThis)->width = x
		#define WSRect_SetHeight(pThis, x) (pThis)->height = x
		#define WSRect_SetBottom(pThis, x) (pThis)->height = x - (pThis)->top
		#define WSRect_SetRight(pThis, x) (pThis)->width = x - (pThis)->left
	#else
		#define WSRect_GetWidth(pThis) (pThis)->right - (pThis)->left
		#define WSRect_GetHeight(pThis) (pThis)->bottom - (pThis)->top
		#define WSRect_GetRight(pThis) (pThis)->right
		#define WSRect_GetBottom(pThis) (pThis)->bottom
		#define WSRect_SetWidth(pThis, x) (pThis)->right = x + (pThis)->left
		#define WSRect_SetHeight(pThis, x) (pThis)->bottom = x + (pThis)->top
		#define WSRect_SetRight(pThis, x) (pThis)->right = x
		#define WSRect_SetBottom(pThis, x) (pThis)->bottom = x
	#endif
#endif
#define WSRect_GetLeft(pThis) WSRect_GetX(pThis)
#define WSRect_GetTop(pThis) WSRect_GetY(pThis)

#if defined(_WIN32)
typedef struct WSNOTIFYICONDATAA {
	uint32_t cbSize;
	struct HWND__* hWnd;
	unsigned int uID;
	unsigned int uFlags;
	unsigned int uCallbackMessage;
	struct HICON__* hIcon;
	char szTip[64];
} WSNOTIFYICONDATAA;
typedef wsbool(__stdcall* SHELL_NOTIFYICONAPROC)(
	uint32_t dwMessage, WSNOTIFYICONDATAA* lpData);
typedef struct WSNOTIFYICON {
	WSNOTIFYICONDATAA mNid;
	WSDLL mShell32;
	SHELL_NOTIFYICONAPROC mlpShell_NotifyIconA;
} WSNOTIFYICON;
typedef uint16_t WSWNDCLASS;
#else
typedef struct WSNOTIFYICON_* WSNOTIFYICON;
typedef struct WSWNDCLASS
{
	WSICON* mWindowIcon;
	#if !defined(WSKDEOPT)
		LPWSWindowMessages mWindowMessages;
	#endif
} WSWNDCLASS;
#endif
#if defined(_WIN32)
typedef struct WSTEXTCTRL
{
	struct HWND__* mHandle;
	size_t mTextLength;
	WSDLL mRiched;
} WSTEXTCTRL;
#define WSDEFAULTPOSITION ((int)0x80000000)
#else
typedef struct WSTEXTCTRL_* WSTEXTCTRL;
#define WSDEFAULTPOSITION 0
#endif
/* WS[XXX]Event methods */
WSWINDOW* WSEvent_GetWindow(WSEVENT* ev);
void* WSEvent_GetWindowCon(WSEVENT* ev);
WSWINDOW* WSPaintEvent_GetWindow(WSPAINTEVENT* ev);
void* WSPaintEvent_GetWindowCon(WSPAINTEVENT* ev);
WSWINDOW* WSKeyEvent_GetWindow(WSKEYEVENT* ev);
void* WSKeyEvent_GetWindowCon(WSKEYEVENT* ev);
size_t WSKeyEvent_GetKey(WSKEYEVENT* ev);
WSWINDOW* WSCloseEvent_GetWindow(WSCLOSEEVENT* ev);
void* WSCloseEvent_GetWindowCon(WSCLOSEEVENT* ev);
WSWINDOW* WSNotifyIconEvent_GetWindow(WSNOTIFYICONEVENT* ev);
void* WSNotifyIconEvent_GetWindowCon(WSNOTIFYICONEVENT* ev);
WSWINDOW* WSMenuEvent_GetWindow(WSMENUEVENT* ev);
void* WSMenuEvent_GetWindowCon(WSMENUEVENT* ev);
WSWINDOW* WSSizeEvent_GetWindow(WSSIZEEVENT* ev);
void* WSSizeEvent_GetWindowCon(WSSIZEEVENT* ev);
guipos_t WSSizeEvent_GetWidth(WSSIZEEVENT* ev);
guipos_t WSSizeEvent_GetHeight(WSSIZEEVENT* ev);
WSWINDOW* WSDestroyEvent_GetWindow(WSDESTROYEVENT* ev);
void* WSDestroyEvent_GetWindowCon(WSDESTROYEVENT* ev);

/* WSICON */
void WSIcon_CreateFromResource(WSICON* pThis, WSAPP* pApp, const char* path);
void WSIcon_CreateFromFile(WSICON* pThis, const char* path, size_t pathlen);
void WSIcon_Destroy(WSICON* pThis);

#if defined(WSMACCLASSICORCARBON) \
	|| defined(__MACH__) \
	|| (!defined(__MACH__) && !defined(macintosh) \
			&& !defined(_WIN32) && defined(WSKDEOPT))
wsbool WSWndClass_Create(WSWNDCLASS* pThis, WSAPP* pApp, WSICON* pWindowIcon,
	const char* wndclassname);
#define WSWndClass_Create(pThis, pApp, pWindowIcon, messages, wndclassname) \
	WSWndClass_Create(pThis, pApp, pWindowIcon, wndclassname)
#else
wsbool WSWndClass_Create(WSWNDCLASS* pThis, WSAPP* pApp, WSICON* pWindowIcon,
	LPWSWindowMessages messages, const char* wndclassname);
#endif
void WSWndClass_Destroy(WSWNDCLASS* pThis, WSAPP* pApp);

		
wsbool WSNotifyIcon_Create(WSNOTIFYICON* pThis, WSWINDOW* pWindow,
	const char* tip, size_t tiplen, WSICON* pIcon, const char* iconpath,
	uint16_t command);
void WSNotifyIcon_SetTooltip(WSNOTIFYICON* pThis, const char* title);
void WSNotifyIcon_SetIcon(WSNOTIFYICON* pThis, const char* path);
void WSNotifyIcon_SetMenu(WSNOTIFYICON* pThis, WSMENU* pMenu);
void WSNotifyIcon_Destroy(WSNOTIFYICON* pThis);

void WSWindow_Create(WSWINDOW* pThis, WSAPP* pApp, WSRECT* bounds,
	const char* title, WSMENUBAR* pMenuBar, WSWNDCLASS* pClass);
void WSWindow_Destroy(WSWINDOW* pThis);
void WSWindow_SetMenuBar(WSWINDOW* pThis, WSMENUBAR* pMenuBar,
	WSMENUACTIONS* ma);
wsbool WSWindow_IsVisible(WSWINDOW* pThis);
wsbool WSWindow_SetIconFromFilePath(WSWINDOW* pThis,
	const char* abspath, size_t abspathlen);
void WSWindow_Hide(WSWINDOW* pThis);
void WSWindow_Show(WSWINDOW* pThis);
void* WSWindow_GetCon(WSWINDOW* pThis);
void WSWindow_SetCon(WSWINDOW* pThis, void* con);
void* WSWindow_GetHandle(WSWINDOW* pThis);
WSEVENT_CALLBACK(WSWindowDefaultCallback);
#if defined(__MACH__) || defined(_WIN32) || defined(macintosh)
	#define WSWINDOW_CLOSEKEEPALIVE FALSE
	#define WSWINDOW_CLOSEKEEPDEAD TRUE
#else
	#define WSWINDOW_CLOSEKEEPALIVE TRUE
	#define WSWINDOW_CLOSEKEEPDEAD FALSE
#endif

void WSScrolledWindow_Destroy(WSSCROLLEDWINDOW* pThis);

void WSDC_Create(WSDC* pThis, WSWINDOW* pWindow);
void WSDC_Destroy(WSDC* pThis, WSWINDOW* pWindow);
wsbool WSDC_IsWindowReady(WSDC* pThis, WSWINDOW* pWindow);
void WSDC_GetTextWidthAndHeight(WSDC* pThis,
	guipos_t* outwidth, guipos_t* outheight);

#if defined(_WIN32)
typedef struct WSPAINTDC
{
	WSDC         hdc;
	wsbool        fErase;
	WSRECT        rcPaint;
	wsbool        fRestore;
	wsbool        fIncUpdate;
	uint8_t        rgbReserved[32];
} WSPAINTDC;
#else
typedef struct WSPAINTDC_* WSPAINTDC;
#endif
void WSPaintDC_Create(WSPAINTDC* pThis, WSWINDOW* pWindow);
void WSPaintDC_Destroy(WSPAINTDC* pThis, WSWINDOW* pWindow);
WSDC* WSPaintDC_GetDC(WSPAINTDC* pThis);


#if defined(_WIN32)
wsbool WSTextCtrl_CreateScrolled(WSTEXTCTRL* pThis, WSSCROLLEDWINDOW* pParent,
	WSWINDOW* pTopParent, WSRECT* rect, WSWNDCLASS* pClass);
#else
wsbool WSTextCtrl_CreateScrolled(WSTEXTCTRL* pThis, WSSCROLLEDWINDOW* pParent,
	WSWINDOW* pTopParent, WSRECT* rect);
	#define WSTextCtrl_CreateScrolled(pThis, pParent, pTopParent, rect, wc) \
		WSTextCtrl_CreateScrolled(pThis, pParent, pTopParent, rect)
#endif
void WSTextCtrl_Append(WSTEXTCTRL* pThis, const char* text, size_t textLen);
void WSTextCtrl_Activate(WSTEXTCTRL* pThis);
void WSTextCtrl_Deactivate(WSTEXTCTRL* pThis);
void WSTextCtrl_Update(WSTEXTCTRL* pThis, WSWINDOW* pParent, WSRECT* rect);
void WSTextCtrl_SetText(WSTEXTCTRL* pThis, const char* text, size_t textLen);
void WSTextCtrl_Destroy(WSTEXTCTRL* pThis);
void WSTextCtrl_Idle(WSTEXTCTRL* pThis);
void WSTextCtrl_SetSize(WSTEXTCTRL* pThis, guipos_t width, guipos_t height);
void* WSTextCtrl_GetHandle(WSTEXTCTRL* pThis);
/*void WSTextCtrl_GetFrameRect(WSTEXTCTRL* pThis, WSRECT* r);*/
void WSTextCtrl_HandleKeyDown(WSTEXTCTRL* pThis, WSKEYEVENT* ev);
void WSTextCtrl_HandlePaint(WSTEXTCTRL* pThis, WSPAINTEVENT* ev, WSPAINTDC* dc);
void WSTextCtrl_HandleContentEvent(WSTEXTCTRL* pThis,
	WSCONTENTEVENT* ev);
void WSTextCtrl_HandleGrowEvent(WSTEXTCTRL* pThis,
	WSGROWEVENT* ev);
void WSTextCtrl_HandleZoomEvent(WSTEXTCTRL* pThis,
	WSZOOMEVENT* ev);
void WSTextCtrl_HandleKeyEvent(WSTEXTCTRL* pThis, WSKEYEVENT* ev);
void WSTextCtrl_HandleIdle(WSTEXTCTRL* pThis);
void WSTextCtrl_HandleActivate(WSTEXTCTRL* pThis);
void WSTextCtrl_HandleDeactivate(WSTEXTCTRL* pThis);
void WSTextCtrl_HandleIBeamCursor(WSTEXTCTRL* pThis,
	WSIBEAMEVENT* er);
void WSTextCtrl_HandleMouseWheel(WSTEXTCTRL* pThis,
	WSMOUSEWHEELEVENT* er);

void WSMenuActions_Create(WSMENUACTIONS* pThis);
void WSMenuActions_Destroy(WSMENUACTIONS* pThis);

#if !defined(WSMACCLASSICORCARBON)
	#define WSMENUINDEX(x) (x)
	void WSMenu_Create(WSMENU* pThis, WSMENUSECTION* pSection);
	#define WSMenu_Create(pThis, pSection, title) WSMenu_Create(pThis, pSection)
#else
	#define WSMENUINDEX(x) (x + 1)
	void WSMenu_Create(WSMENU* pThis, WSMENUSECTION* pSection, const char* title);
#endif
void WSMenu_Destroy(WSMENU* pThis);
void WSMenu_Finalize(WSMENU* pThis, WSMENUSECTION* pSection);
void WSMenu_EndSection(WSMENU* pThis, WSMENUSECTION* pSection);
void WSMenu_SetItemTitle(WSMENU* pThis, size_t index, const char* title);
/*
	Windows: "&Shutdown\tCtrl+Q"
	Mac: "\pShutdown/Q"
*/
#if defined(_WIN32)
	#define WSMenuSection_AppendWithAccelLit(pThis, title, cid, cb, \
			cd, acc, ag) \
		WSMenuSection_Append(pThis, title "\tCtrl+" acc, cid, cb, cd, ag)
	#if !defined(WSMENUALT)
		#define WSMENUALT "&"
	#endif
#elif defined(WSMACCLASSICORCARBON)
	#define WSMenuSection_AppendWithAccelLit(pThis, title, cid, cb, \
			cd, acc, ag) \
		WSMenuSection_Append(pThis, title "/" acc, cid, cb, cd, ag)
	#if !defined(WSMENUALT)
		#define WSMENUALT
	#endif
#else
	#define WSMenuSection_AppendWithAccelLit(pThis, title, cid, cb, \
			cd, acc, ag) \
		WSMenuSection_AppendWithAccel(pThis, title, cid, cb, cd, acc, ag)
	#if !defined(WSMENUALT)
		#define WSMENUALT
	#endif
#endif
wsbool WSMenuSection_Append(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*mmes), WSWINDOW* window,
	WSMENUACTIONS* ma);
wsbool WSMenuSection_AppendWithAccel(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*mmes), WSWINDOW* window,
	const char* accel, WSMENUACTIONS* ma);
void WSMenu_Popup(WSMENU* pThis, WSWINDOW* pWindow);
WSMENUEVENT_CALLBACK(WSMenuSectionDefaultCallback);

void WSMenuBar_Create(WSMENUBAR* pThis);
void WSMenuBar_Append(WSMENUBAR* pThis, WSMENU* pMenu, const char* title);
void WSMenuBar_Draw(WSMENUBAR* pThis);
void WSMenuBar_Destroy(WSMENUBAR* pThis);

void WSDisplayGetDPI(guipos_t* x, guipos_t* y);
void WSDisplayGetScaleFactor(guipos_t* x, guipos_t* y);
void WSApp_SetNotifyIconMenu(WSAPP* pThis, WSMENU* pMenu);
void WSApp_SetNotifyIcon(WSAPP* pThis, const char* abspath, size_t abspathlen);
void WSApp_SetMenuBar(WSAPP* pThis, WSMENUBAR* pMenuBar, WSMENUACTIONS* ma);

wsbool WSLaunchURL(const char* url, size_t urllen);

#define WSMESSAGEBOX_OK                       0x00000000L
#define WSMESSAGEBOX_OKCANCEL                 0x00000001L
#define WSMESSAGEBOX_ABORTRETRYIGNORE         0x00000002L
#define WSMESSAGEBOX_YESNOCANCEL              0x00000003L
#define WSMESSAGEBOX_YESNO                    0x00000004L
#define WSMESSAGEBOX_RETRYCANCEL              0x00000005L
#define WSMESSAGEBOX_ICONHAND                 0x00000010L
#define WSMESSAGEBOX_ICONQUESTION             0x00000020L
#define WSMESSAGEBOX_ICONEXCLAMATION          0x00000030L
#define WSMESSAGEBOX_ICONASTERISK             0x00000040L
#define WSMESSAGEBOX_ICONSTOP		WSMESSAGEBOX_ICONHAND
#define WSMESSAGEBOX_ICONCAUTION	WSMESSAGEBOX_ICONEXCLAMATION
#define WSMESSAGEBOX_IDOK						0x00000001L
#define WSMESSAGEBOX_IDCANCEL                   0x00000002L
#define WSMESSAGEBOX_IDABORT                    0x00000003L
#define WSMESSAGEBOX_IDRETRY                    0x00000004L
#define WSMESSAGEBOX_IDIGNORE                   0x00000005L
#define WSMESSAGEBOX_IDYES						0x00000006L
#define WSMESSAGEBOX_IDNO						0x00000007L
#define WSMESSAGEBOX_IDTRYAGAIN                 0x0000000AL
#define WSMESSAGEBOX_IDCONTINUE					0x0000000BL
int WSMessageBox(WSWINDOW* pParent,
	const char* title, const char* message, int ops);
#define WSMessageBoxSimple(message) WSMessageBox(NULL, "Message", message, 0)

void WSDebugMemoryBegin(void);
void WSDebugMemoryEnd(void);
#ifdef __cplusplus
} /* end extern "C" */
#endif
#endif /* WIKISERVER_VERSION */
#endif /* __GUI_H__ */
#if defined(GUI_IMPLEMENTATION)
/*=============================================================================
		 #####  #####                                     #####
		 #     #     #  ####  #    # #    #  ####  #    #     #
		 #     #       #    # ##  ## ##  ## #    # ##   #     #
		 #     #       #    # # ## # # ## # #    # # #  #     #
		 #     #       #    # #    # #    # #    # #  # #     #
		 #     #     # #    # #    # #    # #    # #   ##     #
		 #####  #####   ####  #    # #    #  ####  #    # #####
=============================================================================*/
#if !defined(WSPATHSEPARATOR)
	#ifdef _WIN32
		#define WSPATHSEPARATOR "\\"
		#define WSPATHSEPARATORCHAR '\\'
	#else
		#define WSPATHSEPARATOR "/"
		#define WSPATHSEPARATORCHAR '/'
	#endif
#endif /* WSPATHSEPARATOR */
#if !defined(WSALLOC)
	#define WSALLOC(type, count) (type*)malloc(sizeof(type) * (count))
	#define WSALLOC1(x) (x*)malloc(sizeof(x))
	#define WSREALLOC(old, type, count) \
		(type*)realloc(old, sizeof(type) * (count))
	#define	WSFREE(x) free((void*)(x))
#endif /* WSALLOC */
#if !defined(WSMemoryCopy)
	#define WSMemoryCopy memcpy
	#define WSMemoryMove memmove
	#define WSMemoryFill memset
	#define WSMemoryCompare memcmp
#endif /* WSMemoryXXX */
#if !defined(WSASSERT)
	#include <assert.h>
	#define WSASSERT(x) assert(x)
#endif /* WSASSERT */
#if !defined(WSMAXLENGTH)
	#define WSMAXLENGTH ((size_t)-1)
	#define WSMAXSIZE	((size_t)(~0))
#endif /* WSMAXLENGTH */
#if !defined(__MUTSTR_H__) && !defined(WIKISERVER_VERSION)
#include <string.h>
#define CStrLength strlen
#define CStrLocateChar strchr
#define CStrFind GUICStrFind
size_t CStrFind(const char* x, size_t xlen, const char* y, size_t ylen,
	size_t pos)
{
	const char* res = strstr(&(x)[pos], y);
	if(res)
		return res - &(x)[pos];
	return WSMAXLENGTH;
}
#define CStrrfind GUICStrrfind
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
#endif
/*=============================================================================
					 #     # ### #     #  #####   #####
					 #  #  #  #  ##    # #     # #     #
					 #  #  #  #  # #   #       #       #
					 #  #  #  #  #  #  #  #####   #####
					 #  #  #  #  #   # #       # #
					 #  #  #  #  #    ## #     # #
					  ## ##  ### #     #  #####  #######
=============================================================================*/
#if defined(_WIN32)
#ifndef PRECOMPILEDHEADERS
	#define WIN32_LEAN_AND_MEAN
	#define	  NOSERVICE
	#define	  NOMCX
	#define	  NOIME
	#define	  NOSOUND
	#define	  NOCOMM
	#define	  NOKANJI
	#define	  NORPC
	#define	  NOPROXYSTUB
	#define	  NOIMAGE
	#define	  NOTAPE
	#include <windows.h>
	#ifdef int64
		#undef int64
		#undef uint64
	#endif
	#include <objbase.h>
#endif
#include <Shlobj.h>
/* UWP can't load system DLLs directly, so pull in func memory addresses */
#if defined(WSMSIXOPT)
	#include <Uxtheme.h>
	#include <shellapi.h>
	#ifndef NTAPI
		#define NTAPI WINAPI
	#endif
	#ifdef __cplusplus
		extern "C" {
	#endif
	NTSYSAPI LONG NTAPI RtlGetVersion(
		PRTL_OSVERSIONINFOW lpVersionInformation
	);
	#ifdef __cplusplus
		}
	#endif
	#pragma comment (lib, "comctl32.lib")
	#pragma comment (lib, "uxtheme.lib")
	#pragma comment (lib, "ntdll.lib")
#endif
#if (!defined(_MSC_VER) || (_MSC_VER < 1300)) && !defined(_WIN64)
#define UINT_PTR unsigned int
#define LONG_PTR long
#define ULONG_PTR unsigned long
#define DWORD_PTR unsigned long
#ifdef SetWindowLongPtr
#	undef SetWindowLongPtr
#endif
#ifdef GetWindowLongPtr
#	undef GetWindowLongPtr
#endif
#ifdef GetClassLongPtr
#	undef GetClassLongPtr
#endif
#ifdef GWLP_USERDATA
#undef GWLP_USERDATA
#endif
#ifdef GWLP_WNDPROC
#undef GWLP_WNDPROC
#endif
#define SetWindowLongPtr SetWindowLong
#define GetWindowLongPtr GetWindowLong
#define GetClassLongPtr GetClassLong
#define GWLP_USERDATA GWL_USERDATA
#define GWLP_WNDPROC GWL_WNDPROC
#endif /* !defined(_MSC_VER) || _MSC_VER < 1300 */
/* Debugging in VC2017: remove NOCRUNTIME;, add _DEBUG;_CRTDBG_MAP_ALLOC;, */
/* turn optimizations off, link ;libucrtd.lib */
#if defined(_DEBUG) && defined(_MSC_VER) && !defined(NOCRUNTIME)
#include <crtdbg.h>
#endif
#define USE_WINNOTIFICATIONS 0 /*TODO: command line switch for this?*/
#define WSMAXDWORD (DWORD)(~0)
#ifndef NOCRUNTIME
#include <process.h> /* _beginthreadex */
#endif
#if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0501)
#define WM_THEMECHANGED 0x031A
#endif
#if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0500)
#define ODS_NOACCEL   0x0100
#define DT_HIDEPREFIX      0x00100000
#endif
#ifndef DT_HIDEPREFIX /* mingw probably - define them ourselves */
	#define DT_HIDEPREFIX 0x00100000
	#define ODS_HOTLIGHT 0x0040
	#define ODS_INACTIVE 0x0080
	#define ODS_NOACCEL 0x0100
	#define ODS_NOFOCUSRECT 0x0200
#endif
/* In C++ Win32 IID Params are ref (&) instead of p (*)  */
#ifndef WSIIDPARAM
	#ifdef __cplusplus
		#define WSIIDPARAM(x) x
	#else
		#define WSIIDPARAM(x) &(x)
	#endif
#endif
/* SetWindowLongPtr==SetWindowLong on 32-bit targets (32 vs 64 bit warnings) */
#if defined(_X86_) || defined(_M_IX86)
#define WSSetWindowLongPtr(hw, i, p) SetWindowLongPtr(hw, i, (LONG)(LONG_PTR)p)
#else
#define WSSetWindowLongPtr(hw, i, p) SetWindowLongPtr(hw, i, (LONG_PTR)p)
#endif

#ifdef __cplusplus
	extern "C" { /* Everything in this file is C linkage */
#endif

/*=============================================================================*/
/* [Utils] */
/*=============================================================================*/
size_t CopyWSTRToSTR(char* s, size_t slen,
	const WCHAR* w, size_t wlen)
{
	WSASSERT(slen <= INT_MAX);
	WSASSERT(wlen <= INT_MAX);
	slen = WideCharToMultiByte(CP_UTF8, 0, w, (int)wlen,
		s, (int)slen, NULL, NULL);
	s[slen] = '\0';
	return slen;
}

LPWSTR STRToWSTRWithLength(const char* s, size_t* len)
{
	size_t alen;
	int wlen;
	LPWSTR w;
	if (len == NULL || *len == WSMAXLENGTH)
		alen = CStrLength(s);
	else
		alen = *len;
	wlen = MultiByteToWideChar( /* get length */
		CP_UTF8, 0, s, (int)alen, NULL, 0);
	w = WSALLOC(WCHAR, wlen + 1); /* new buffer */
	if(w)
	{ /* convert the incoming string to OLECHARs */
		MultiByteToWideChar(CP_UTF8, 0, s, (int)alen, w, wlen);
		w[wlen] = '\0';
		if(len)
			*len = wlen;
	}
	return w;
}
LPWSTR STRToWSTR(const char* s)
{
	return STRToWSTRWithLength(s, NULL);
}

char* WSTRToSTRWithLength(LPCWSTR w, size_t* len)
{
	size_t wlen;
	int alen;
	char* s;
	if (len == NULL || *len == WSMAXLENGTH)
	{
		wlen = 0;
		while (w[wlen]) { ++wlen; }
	}
	else
		wlen = *len;
	alen = WideCharToMultiByte(CP_UTF8, 0, w, (int)wlen,
		NULL, 0, NULL, NULL);
	s = WSALLOC(char, alen + 1);
	if(s)
	{
		CopyWSTRToSTR(s, alen, w, wlen);
		s[alen] = '\0';
		if(len)
			*len = alen;
	}
	return s;
}
char* WSTRToSTR(LPCWSTR w)
{
	return WSTRToSTRWithLength(w, NULL);
}
/* GetModuleHandle is banned in Windows Store; get HMODULE by func address */
struct HINSTANCE__* WSWinGetFunctionBase(void* func)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	VirtualQuery(func, &mbi, sizeof(mbi));
	return (struct HINSTANCE__*)(mbi.AllocationBase);
}

/*=============================================================================*/
/* Windows XP Common Controls & Visual Styling Without Manifest */
/*=============================================================================*/
#ifndef ACTCTX_FLAG_PROCESSOR_ARCHITECTURE_VALID
#define ACTCTX_FLAG_PROCESSOR_ARCHITECTURE_VALID    0x00000001
#define ACTCTX_FLAG_LANGID_VALID                    0x00000002
#define ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID        0x00000004
#define ACTCTX_FLAG_RESOURCE_NAME_VALID             0x00000008
#define ACTCTX_FLAG_SET_PROCESS_DEFAULT             0x00000010
#define ACTCTX_FLAG_APPLICATION_NAME_VALID          0x00000020
#define ACTCTX_FLAG_SOURCE_IS_ASSEMBLYREF           0x00000040
#define ACTCTX_FLAG_HMODULE_VALID                   0x00000080
typedef struct tagACTCTXW
{
	ULONG       cbSize;
	DWORD       dwFlags;
	LPCWSTR     lpSource;
	USHORT      wProcessorArchitecture;
	LANGID      wLangId;
	LPCWSTR     lpAssemblyDirectory;
	LPCWSTR     lpResourceName;
	LPCWSTR     lpApplicationName;
	HMODULE     hModule;
} ACTCTXW, *PACTCTXW;
typedef const PACTCTXW PCACTCTXW;
#endif
#ifndef ICC_TAB_CLASSES
typedef struct tagINITCOMMONCONTROLSEX
{
	DWORD dwSize;
	DWORD dwICC;
} INITCOMMONCONTROLSEX, *LPINITCOMMONCONTROLSEX;
#define ICC_TAB_CLASSES        0x00000008
#define ICC_PROGRESS_CLASS     0x00000020
#endif
#ifndef ICC_STANDARD_CLASSES
#define ICC_STANDARD_CLASSES   0x00004000
#endif
typedef BOOL (WINAPI *LPActivateActCtx)(HANDLE hActCtx, ULONG_PTR *lpCookie);
typedef BOOL (WINAPI *LPInitCommonControlsEx)(const INITCOMMONCONTROLSEX *picce);
typedef UINT (WINAPI *LPGetSystemDirectoryW)(LPWSTR lpBuffer, UINT uSize);
typedef HANDLE (WINAPI *LPCreateActCtxW)(PCACTCTXW pActCtx);
BOOL WSEnableVisualStyles(void)
{
	LPActivateActCtx lpActivateActCtx;
	LPInitCommonControlsEx lpInitCommonControlsEx;
	LPGetSystemDirectoryW lpGetSystemDirectoryW;
	LPCreateActCtxW lpCreateActCtxW;
	WCHAR dir[MAX_PATH];
	ULONG_PTR cookie = FALSE;
	HANDLE hActCtx;
	BOOL success;
	WSDLL comCtl32, kernel32;
	INITCOMMONCONTROLSEX icex =
	{
		sizeof(INITCOMMONCONTROLSEX),
		ICC_TAB_CLASSES | ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES
	};
	ACTCTXW actCtx;
	actCtx.cbSize = sizeof(ACTCTXW);
	actCtx.dwFlags =
		ACTCTX_FLAG_RESOURCE_NAME_VALID
			| ACTCTX_FLAG_SET_PROCESS_DEFAULT
			| ACTCTX_FLAG_ASSEMBLY_DIRECTORY_VALID;
	actCtx.lpSource = L"shell32.dll";
	actCtx.wProcessorArchitecture = 0;
	actCtx.wLangId = 0;
	actCtx.lpAssemblyDirectory = dir;
	actCtx.lpResourceName = (LPCWSTR)124;
	WSDLL_CreateKernel32(&kernel32);
	lpActivateActCtx = (LPActivateActCtx)
		WSDLL_GetProc(&kernel32, "ActivateActCtx");
	if(!lpActivateActCtx)
	{
		WSDLL_DestroyKernel32(&kernel32);
		return FALSE;
	}
	WSDLL_CreateComctl32(&comCtl32);
	lpInitCommonControlsEx = (LPInitCommonControlsEx) WSDLL_GetProc(
		&comCtl32, "InitCommonControlsEx");
	lpGetSystemDirectoryW = (LPGetSystemDirectoryW) WSDLL_GetProc(
		&kernel32, "GetSystemDirectoryW");
	lpCreateActCtxW = (LPCreateActCtxW)WSDLL_GetProc(
		&kernel32, "CreateActCtxW");

	(*lpInitCommonControlsEx)(&icex);
	dir[(*lpGetSystemDirectoryW)(dir, sizeof(dir) / sizeof(*dir))] = '\0';
	hActCtx = (*lpCreateActCtxW)(&actCtx);
	success = hActCtx != INVALID_HANDLE_VALUE;
	if(success)
		success = (*lpActivateActCtx)(hActCtx, &cookie);
	WSDLL_DestroyComctl32(&comCtl32);
	WSDLL_DestroyKernel32(&kernel32);
	return success;
}
/*===========================================================================*/
/*	DLL*/
/*===========================================================================*/
int WSDLL_Create(WSDLL* pThis, const char* libraryname)
{
#if defined(WSMSIXOPT)
	WCHAR* w = STRToWSTR(libraryname);
	pThis->mHandle = LoadPackagedLibrary(w, 0);
	WSFREE(w);
#else
	pThis->mHandle = LoadLibraryA(libraryname);
#endif
	return pThis->mHandle != NULL;
}
void* WSDLL_GetProc(const WSDLL* pThis, const char* procname)
{
	return (void*)GetProcAddress((HMODULE)pThis->mHandle, procname);
}
void WSDLL_Destroy(WSDLL* pThis)
{
	FreeLibrary((HMODULE)pThis->mHandle);
}
/*-----------------------------------------------------------------------------*/
/*						WSEVENTLOOP*/
/*-----------------------------------------------------------------------------*/
wsbool WSEventLoop_Create(WSEVENTLOOP* pThis)
{
	(void)(pThis);
	return TRUE;
}
void WSEventLoop_Destroy(WSEVENTLOOP* pThis)
{
	(void)(pThis);
}
void WSEventLoop_Dispatch(WSEVENTLOOP* pThis)
{
	TranslateMessage((MSG*)pThis);
	DispatchMessage((MSG*)pThis);
}
wsbool WSEventLoop_RunSingle(WSEVENTLOOP* pThis)
{
	if (PeekMessage((MSG*)pThis, NULL, 0, 0, PM_REMOVE)
		&& pThis->message != WM_QUIT)
	{
		WSEventLoop_Dispatch(pThis);
		return TRUE;
	}
	return FALSE;
}
void WSEventLoop_Run(WSEVENTLOOP* pThis)
{
	while (GetMessage((MSG*)pThis, 0, 0, 0)
		&& pThis->message != WM_QUIT)
	{
		WSEventLoop_Dispatch(pThis);
	}
}
wsbool WSEventLoop_IsQuitMessage(WSEVENTLOOP* pThis)
{
	return pThis->message == WM_QUIT;
}
/*=============================================================================*/
/* WINAPI definitions for dynamic function loading */
/*=============================================================================*/

#ifndef NIM_ADD
#define NIM_ADD			0x00000000
#define NIM_MODIFY		0x00000001
#define NIM_DELETE		0x00000002
#define NIF_MESSAGE		0x00000001
#define NIF_ICON		0x00000002
#define NIF_TIP			0x00000004
#endif

/*-----------------------------------------------------------------------------*/
/*						GUI*/
/*-----------------------------------------------------------------------------*/
#define WSWINDOW_HANDLE(pThis) (*(HWND*)(pThis))
#define WSSCROLLEDWINDOW_HANDLE(pThis) (*(HWND*)(pThis))
#define WSMENU_HANDLE(pThis) (*(HMENU*)(pThis))
#define WSMENUBAR_HANDLE(pThis) (*(HMENU*)(pThis))
#define WSMENUSECTION_HANDLE(pThis) (*(HMENU*)(pThis))
#define WSDC_HANDLE(pThis) (*(HDC*)(pThis))
#define WSPAINTDC_HANDLE(pThis) (*(PAINTSTRUCT**)&(pThis))
#define WSICON_HANDLE(pThis) (*(HICON*)(pThis))
#define WSNOTIFYICON_HANDLE(pThis) ((PNOTIFYICONDATAA)&(pThis->mNid))
#define WSWNDCLASS_HANDLE(pThis) (*(uint16_t*)(pThis))
#define WSTEXTCTRL_HANDLE(pThis) ((pThis)->mHandle)

/*-----------------------------------------------------------------------------*/
/*						WS[XXX]Event methods								   */
/*-----------------------------------------------------------------------------*/
#define WSWindowFromHandle(hWnd) ((struct WSWINDOW_**)&hWnd)
WSWINDOW* WSEvent_GetWindow(WSEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSEvent_GetWindowCon(WSEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
WSWINDOW* WSPaintEvent_GetWindow(WSPAINTEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSPaintEvent_GetWindowCon(WSPAINTEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
WSWINDOW* WSKeyEvent_GetWindow(WSKEYEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSKeyEvent_GetWindowCon(WSKEYEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
size_t WSKeyEvent_GetKey(WSKEYEVENT* ev)
{ return ev->mwParam; }
WSWINDOW* WSCloseEvent_GetWindow(WSCLOSEEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSCloseEvent_GetWindowCon(WSCLOSEEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
WSWINDOW* WSNotifyIconEvent_GetWindow(WSNOTIFYICONEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSNotifyIconEvent_GetWindowCon(WSNOTIFYICONEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
WSWINDOW* WSMenuEvent_GetWindow(WSMENUEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSMenuEvent_GetWindowCon(WSMENUEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
WSWINDOW* WSSizeEvent_GetWindow(WSSIZEEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSSizeEvent_GetWindowCon(WSSIZEEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
guipos_t WSSizeEvent_GetWidth(WSSIZEEVENT* ev)
{ return loword(ev->mlParam); }
guipos_t WSSizeEvent_GetHeight(WSSIZEEVENT* ev)
{ return hiword(ev->mlParam); }
WSWINDOW* WSDestroyEvent_GetWindow(WSDESTROYEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSDestroyEvent_GetWindowCon(WSDESTROYEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }


void WSIcon_CreateFromResource(WSICON* pThis, WSAPP* pApp, const char* path)
{
	WSICON_HANDLE(pThis) = LoadIconA(pApp->mhInstance, path);
}

void WSIcon_CreateFromFile(WSICON* pThis, const char* path, size_t pathlen)
{
	LPWSTR w = STRToWSTRWithLength(path, &pathlen);
	WSICON_HANDLE(pThis) = (HICON)LoadImageW(NULL, w, IMAGE_ICON,
			0, 0,
			LR_LOADFROMFILE | LR_DEFAULTSIZE);
	WSFREE(w);
}

void WSIcon_Destroy(WSICON* pThis)
{
	DestroyIcon(WSICON_HANDLE(pThis));
}


wsbool WSWndClass_Create(WSWNDCLASS* pThis, WSAPP* pApp, WSICON* pWindowIcon,
	LPWSWindowMessages messages, const char* wndclassname)
{
	/*
		GetDC[Ex] have 5 common DCs available in Windows 95 when not called
		on a window with a CS_OWNDC style. CS_OWNDC windows require 800 bytes
		each from the 64KB GDI local heap, so it isn't advisable to use
		the flag.

		However, if one of the common DCs is held on for too long,
		Windows 95 will reassign and cause any future calls to fail,
		including Windows OpenGL's wglMakeCurrent. This is prevented via
		CS_OWNDC as surmised, but the better resolution is just not to
		hold onto common DCs for too long. This also means in OpenGL the context
		needs to be recreated every now and then most likely.
	*/
	WNDCLASSA wc;
	wc.style = 0;
	wc.lpfnWndProc = messages;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = pApp->mhInstance;
	wc.hIcon = WSICON_HANDLE(pWindowIcon);
	wc.hCursor = NULL; /* A.K.A. LoadCursor((HINSTANCE)NULL, IDC_ARROW); */
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = wndclassname;
	WSWNDCLASS_HANDLE(pThis) = RegisterClassA(&wc); /* !atom == error */
	/*DWORD gle = GetLastError();
	int breakhere = 5;*/
	return WSWNDCLASS_HANDLE(pThis) != 0;
}
void WSWndClass_Destroy(WSWNDCLASS* pThis, WSAPP* pApp)
{
	/*DWORD res = */
		UnregisterClassA((LPCSTR)MAKEINTATOM(WSWNDCLASS_HANDLE(pThis)),
		pApp->mhInstance);
	/*DWORD gle = GetLastError();
	int breakhere = 5;*/
}


wsbool WSNotifyIcon_Create(WSNOTIFYICON* pThis, WSWINDOW* pWindow,
	const char* tip, size_t tiplen, WSICON* pIcon, const char* iconpath,
	uint16_t command)
{
	if (tiplen > strcountof(pThis->mNid.szTip))
		tiplen = strcountof(pThis->mNid.szTip);
	WSMemoryCopy(pThis->mNid.szTip, tip, (tiplen) * sizeof(CHAR));
#if !defined(WSMSIXOPT)
	WSDLL_CreateShell32(&pThis->mShell32);
	pThis->mlpShell_NotifyIconA =
		EXPLICITPROCCAST(SHELL_NOTIFYICONAPROC)
			WSDLL_GetProc(&pThis->mShell32, "Shell_NotifyIconA");
#else
	pThis->mlpShell_NotifyIconA = (SHELL_NOTIFYICONAPROC)Shell_NotifyIconA;
#endif
	pThis->mNid.cbSize = sizeof(WSNOTIFYICON);
	pThis->mNid.hWnd = WSWINDOW_HANDLE(pWindow);
	pThis->mNid.uCallbackMessage = WM_WSNOTIFYICON + command;
	pThis->mNid.szTip[tiplen] = '\0';
	pThis->mNid.hIcon = WSICON_HANDLE(pIcon);
	pThis->mNid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	pThis->mNid.uID = 0;
	return (*pThis->mlpShell_NotifyIconA)(NIM_ADD, &pThis->mNid);
}
void WSNotifyIcon_SetTooltip(WSNOTIFYICON* pThis, const char* title)
{
	(void)(pThis); (void)(title);
}
void WSNotifyIcon_SetIcon(WSNOTIFYICON* pThis, const char* path)
{
	(void)(pThis); (void)(path);
}
void WSNotifyIcon_SetMenu(WSNOTIFYICON* pThis, WSMENU* pMenu)
{
	(void)(pThis); (void)(pMenu);
}
void WSNotifyIcon_Destroy(WSNOTIFYICON* pThis)
{
	(*pThis->mlpShell_NotifyIconA)(NIM_DELETE, &pThis->mNid);
	WSDLL_DestroyShell32(&pThis->mShell32);
}

void WSNotifyIcon_ShowBalloon(WSNOTIFYICON* pThis, const char* message)
{
#if USE_WINNOTIFICATIONS
	pThis->mNid.uFlags = NIF_INFO;
	lstrcpyn(pThis->mNid.szInfoTitle, TEXT("WikiServer Notification"), 24);
	lstrcpyn(pThis->mNid.szInfo, message, 256);
	pThis->mNid.dwInfoFlags = NIIF_INFO; /*icon #*/
	pThis->mNid.u.uTimeout = 1000;
	(*pThis->mlpShell_NotifyIconA)(NIM_MODIFY, &pThis->mNid);
#else
	(void)(pThis); (void)(message);
#endif
}

int WSWindowProcessAccelsRec(WSWINDOWMESSAGEARGSDECLARE, HMENU hM,
	WCHAR** inbuffer, size_t* inbuffersize)
{
	int submenuindex = 0;
	HMENU hMenu;
	while((hMenu = GetSubMenu(hM, submenuindex)) != NULL)
	{
		int itemCount = GetMenuItemCount(hMenu);
		int i;
		WCHAR expected[8];
		MENUITEMINFOW mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_STRING | MIIM_ID | MIIM_SUBMENU;
		expected[0] = '\t';
		expected[1] = 'C';
		expected[2] = 't';
		expected[3] = 'r';
		expected[4] = 'l';
		expected[5] = '+';
		expected[6] = (WCHAR)wParam;
		expected[7] = '\0';
		for(i = 0; i < itemCount; ++i)
		{
			mii.dwTypeData = NULL;
			mii.cch = 0;
			if (GetMenuItemInfoW(hMenu, i, TRUE, &mii))
			{
				if(*inbuffersize < mii.cch + 1)
				{
					WCHAR* newbuffer = WSREALLOC(
						*inbuffer, WCHAR, mii.cch + 1);
					if(!newbuffer)
						return 0;
					*inbuffer = newbuffer;
					*inbuffersize = mii.cch + 1;
				}
				++mii.cch;
				mii.dwTypeData = *inbuffer;
				if (GetMenuItemInfoW(hMenu, i, TRUE, &mii))
				{
					if(mii.hSubMenu)
					{
						if(WSWindowProcessAccelsRec(WSWINDOWMESSAGEARGSPASS,
							mii.hSubMenu, inbuffer, inbuffersize))
							return 1;
					}
					else
					{
						if(mii.cch >= 7 && WSMemoryCompare(
							&mii.dwTypeData[mii.cch - 7], expected,
							7 * sizeof(WCHAR)) == 0)
						{
							SendMessageA(hWnd, WM_COMMAND, mii.wID, 0);
							return 1;
						}
					}
				}
			}
		}
		++submenuindex;
	}
	return 0;
}
void WSWindowProcessAccels(WSWINDOWMESSAGEARGSDECLARE)
{
	if(!(GetKeyState(VK_CONTROL) & 0x8000)
		|| wParam < 'A' || wParam > 'Z')
		return;
	WCHAR* buffer = NULL;
	size_t buffersize = 0;
	WSWindowProcessAccelsRec(WSWINDOWMESSAGEARGSPASS, GetMenu(hWnd),
		&buffer, &buffersize);
	WSFREE(buffer);
}
void WSWindow_Create(WSWINDOW* pThis, WSAPP* pApp, WSRECT* r,
	const char* title, WSMENUBAR* pMenuBar, WSWNDCLASS* pClass)
{
	(void)(pMenuBar);
	WSWINDOW_HANDLE(pThis) = CreateWindowA(
		(LPCSTR)MAKEINTATOM(WSWNDCLASS_HANDLE(pClass)), title,
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_CLIPCHILDREN,
		r->left, r->top,
		/*CW_USEDEFAULT, CW_USEDEFAULT,*/
		r->right, r->bottom, NULL, WSMENUBAR_HANDLE(pMenuBar),
		pApp->mhInstance, NULL);
	if(!WSWINDOW_HANDLE(pThis))
	{
		/*DWORD gle = GetLastError();
		int breakhere = 5;*/
	}
}
void WSWindow_Destroy(WSWINDOW* pThis)
{
	DestroyWindow(WSWINDOW_HANDLE(pThis));
}
void WSWindow_SetMenuBar(WSWINDOW* pThis, WSMENUBAR* pMenuBar,
	WSMENUACTIONS* ma)
{
	(void)(pThis); (void)(pMenuBar); (void)(ma);
}
wsbool WSWindow_IsVisible(WSWINDOW* pThis)
{
	return IsWindowVisible(WSWINDOW_HANDLE(pThis));
}
wsbool WSWindow_SetIconFromFilePath(WSWINDOW* pThis,
	const char* abspath, size_t abspathlen)
{
	(void)(pThis); (void)(abspath); (void)(abspathlen);
	return FALSE;
}
void WSWindow_Hide(WSWINDOW* pThis)
{
	ShowWindow(WSWINDOW_HANDLE(pThis), SW_HIDE);
}
void WSWindow_Show(WSWINDOW* pThis)
{
	ShowWindow(WSWINDOW_HANDLE(pThis), SW_SHOW);
}
void WSWindow_GetFrame(WSWINDOW* pThis, WSRECT* r)
{
	GetClientRect(WSWINDOW_HANDLE(pThis), (RECT*)&r);
}

void* WSWindow_GetCon(WSWINDOW* pThis)
{
	return (void*)GetWindowLongPtr(WSWINDOW_HANDLE(pThis), GWLP_USERDATA);
}
void WSWindow_SetCon(WSWINDOW* pThis, void* con)
{
	WSSetWindowLongPtr(WSWINDOW_HANDLE(pThis), GWLP_USERDATA, con);
}
void* WSWindow_GetHandle(WSWINDOW* pThis)
{
	return WSWINDOW_HANDLE(pThis);
}


void WSScrolledWindow_Destroy(WSSCROLLEDWINDOW* pThis)
{ (void)(pThis); }

void WSDC_Create(WSDC* pThis, WSWINDOW* pWindow)
{
	WSDC_HANDLE(pThis) = GetDC(WSWINDOW_HANDLE(pWindow));
}
void WSDC_Destroy(WSDC* pThis, WSWINDOW* pWindow)
{
	ReleaseDC(WSWINDOW_HANDLE(pWindow), WSDC_HANDLE(pThis));
}
wsbool WSDC_IsWindowReady(WSDC* pThis, WSWINDOW* pWindow)
{
	(void)(pThis); (void)(pWindow);
	return FALSE;
}

void WSPaintDC_Create(WSPAINTDC* pThis, WSWINDOW* pWindow)
{
	BeginPaint(WSWINDOW_HANDLE(pWindow), WSPAINTDC_HANDLE(pThis));
}
void WSPaintDC_Destroy(WSPAINTDC* pThis, WSWINDOW* pWindow)
{
	EndPaint(WSWINDOW_HANDLE(pWindow), WSPAINTDC_HANDLE(pThis));
}
#if defined(_MSC_VER)
#pragma comment(lib, "gdi32.lib")
#endif
void WSDC_GetTextWidthAndHeight(WSDC* pThis,
	guipos_t* outwidth, guipos_t* outheight)
{
#if defined(STBTEXTCTRL)
	TEXTMETRICA tm;
	GetTextMetricsA(WSDC_HANDLE(pThis), &tm);
	*outwidth = tm.tmAveCharWidth;
	*outheight = tm.tmHeight;
#else
	(void)(pThis);
	(void)(outwidth);
	(void)(outheight);
#endif
}
void WSPaintDC_DrawText(WSPAINTDC* pThis,
	const char* text, size_t len, WSRECT* r)
{
#if defined(STBTEXTCTRL)
	int res;
	r->right = 5000;
	res = DrawTextA(WSDC_HANDLE(&pThis->hdc), text, len, (LPRECT)r, 0);
	/*res = 5;*/
	/*BOOL res = TextOutA(WSDC_HANDLE(&pThis->hdc), r->left, r->top, text, len);*/
	/*res =50;*/
	/*TextOutA(WSDC_HANDLE(&pThis->hdc), 0, 0, "HELLO", 5);*/
#else
	(void)(pThis);
	(void)(text);
	(void)(len);
	(void)(r);
#endif
}
WSDC* WSPaintDC_GetDC(WSPAINTDC* pThis)
{
	return &pThis->hdc;
}

/*#define STBTEXTCTRL*/
#if defined(STBTEXTCTRL)
struct StbTexteditRowStruct;

#define STB_TEXTEDIT_KEYTYPE size_t
#define STB_TEXTEDIT_CHARTYPE char
#define STB_TEXTEDIT_POSITIONTYPE size_t
#define STB_TEXTEDIT_WIDTHTYPE size_t

STB_TEXTEDIT_WIDTHTYPE getwidth( WSMUTSTR* str, STB_TEXTEDIT_POSITIONTYPE n, STB_TEXTEDIT_POSITIONTYPE i );
void layoutrow( struct StbTexteditRowStruct* row, WSMUTSTR* str, STB_TEXTEDIT_POSITIONTYPE n );

#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT
#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_PRIOR VK_PRIOR /*pgup*/
#define KEY_NEXT VK_NEXT /*pgdwn*/
#define KEY_HOME VK_HOME
#define KEY_END VK_END
#define KEY_DELETE VK_DELETE
#define KEY_BACK VK_BACK
#define KEY_SHIFT VK_SHIFT
#define KEY_CONTROL VK_CONTROL
#define KEY_INSERT VK_INSERT

#define STB_TEXTEDIT_memmove WSMemoryMove
#define STB_TEXTEDIT_IMPLEMENTATION
#define STB_TEXTEDIT_UNDOSTATECOUNT (4096)
#define STB_TEXTEDIT_UNDOCHARCOUNT (256*1024)
#define STB_TEXTEDIT_STRING WSMUTSTR
#define STB_TEXTEDIT_STRINGLEN(obj) WSMutStr_GetLength(obj)
#define STB_TEXTEDIT_LAYOUTROW(r,obj,n) layoutrow(r,obj,n)
#define STB_TEXTEDIT_GETWIDTH(obj,n,i) getwidth(obj,n,i)
#define STB_TEXTEDIT_GETWIDTH_NEWLINE 0.0f
#define STB_TEXTEDIT_KEYTOTEXT(k) (CharToLower(k))
#define STB_TEXTEDIT_GETCHAR(obj,i) WSMutStr_At(obj, i)
#define STB_TEXTEDIT_NEWLINE '\n'
#define STB_TEXTEDIT_DELETECHARS(obj,i,n) WSMutStr_EraseRange(obj,i,n)
#define STB_TEXTEDIT_INSERTCHARS(obj,i,c,n) WSMutStr_InsertCStringWithLength(obj,i,c,n), 1
#define STB_TEXTEDIT_K_SHIFT  256
#define STB_TEXTEDIT_K_CONTROL 512
#define STB_TEXTEDIT_K_LEFT KEY_LEFT
#define STB_TEXTEDIT_K_RIGHT KEY_RIGHT
#define STB_TEXTEDIT_K_UP KEY_UP
#define STB_TEXTEDIT_K_DOWN KEY_DOWN
#define STB_TEXTEDIT_K_PGUP KEY_PRIOR
#define STB_TEXTEDIT_K_PGDOWN KEY_NEXT
#define STB_TEXTEDIT_K_LINESTART KEY_HOME
#define STB_TEXTEDIT_K_LINEEND KEY_END
#define STB_TEXTEDIT_K_TEXTSTART (STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_TEXTEND (STB_TEXTEDIT_K_LINEEND | STB_TEXTEDIT_K_CONTROL)
#define STB_TEXTEDIT_K_DELETE KEY_DELETE
#define STB_TEXTEDIT_K_BACKSPACE KEY_BACK
#define STB_TEXTEDIT_K_UNDO ( 'Z' | STB_TEXTEDIT_K_CONTROL )
#define STB_TEXTEDIT_K_REDO ( 'Y' | STB_TEXTEDIT_K_CONTROL )
#define STB_TEXTEDIT_K_INSERT KEY_INSERT
#define STB_TEXTEDIT_IS_SPACE(ch) ( CharIsSpace(ch) && ch != '\t' )
#define STB_TEXTEDIT_K_WORDLEFT ( KEY_LEFT | STB_TEXTEDIT_K_CONTROL )
#define STB_TEXTEDIT_K_WORDRIGHT ( KEY_RIGHT | STB_TEXTEDIT_K_CONTROL )
#include "stb_textedit.h"

void GetDefFont()
{
	HFONT hFont;
	NONCLIENTMETRICSA ncMetrics;
	ncMetrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfoA(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncMetrics, 0 );
	hFont= CreateFontIndirectA(&ncMetrics.lfMessageFont);
}



#define tab_width 4
STB_TEXTEDIT_WIDTHTYPE getwidth( WSMUTSTR* str,
	STB_TEXTEDIT_POSITIONTYPE n, STB_TEXTEDIT_POSITIONTYPE i )
{
	const char* s = WSMutStr_GetCString(str) + n;
	const char* end = s + i;
	size_t x = 0;
	while(s < end)
	{
		if(*s == '\t')
		{
			size_t ofs = tab_width - ( x % tab_width );
			x += ( ofs - 1 );
		}
		++s;
		++x;
	}
	if( *s == '\t' )
	{
		size_t ofs = tab_width - ( x % tab_width );
		x += ofs;
		return (STB_TEXTEDIT_WIDTHTYPE)x;
	}
	else if(*s < ' ')
		return 0;
	else
		return 1;
}


void layoutrow( struct StbTexteditRowStruct* row, WSMUTSTR* str,
	STB_TEXTEDIT_POSITIONTYPE n )
{
	char const* s = WSMutStr_GetCString(str) + n;
	size_t len = 0;
	size_t x = 0;
	while( *s ) {
		if(*s == '\t')
		{
			size_t ofs = tab_width - ( x % tab_width );
			x += ( ofs - 1 );
		}
		++x;
		if(*s++ == '\n')
			break;
		++len;
	}
	row->x0 = 0;
	row->x1 = (STB_TEXTEDIT_WIDTHTYPE)x;
	row->baseline_y_delta = 1;
	row->ymin = 0;
	row->ymax = 1;
	row->num_chars = (size_t)( s - WSMutStr_GetCString(str) ) - n;
}
STB_TexteditState state;
WSMUTSTR statestr;
#endif
void WSTextCtrl_HandlePaint(WSTEXTCTRL* pThis, WSPAINTEVENT* ev, WSPAINTDC* dc)
{
#if defined(STBTEXTCTRL)
	WSRECT rect;
	RECT rc;
	HBRUSH hBsh = (HBRUSH)(GetClassLongPtr(WSTEXTCTRL_HANDLE(pThis), GCL_HBRBACKGROUND));
	GetClientRect(WSTEXTCTRL_HANDLE(pThis), &rc);
	FillRect(WSDC_HANDLE(&dc->hdc), &rc, hBsh);
	 rect.left = rect.top = 0;
	rect.bottom = rect.right = 500;
	WSPaintDC_DrawText(dc, WSMutStr_GetCString(&statestr),
		WSMutStr_GetLength(&statestr), &rect);
#endif
}
void WSTextCtrl_HandleKeyDown(WSTEXTCTRL* pThis, WSKEYEVENT* ev)
{
#if defined(STBTEXTCTRL)
	size_t key = WSKeyEvent_GetKey(ev);
	if(key == KEY_SHIFT)
		key = 256;
	else if(key == KEY_CONTROL)
		key = 512;
	stb_textedit_key( &statestr, &state, key );
	InvalidateRect(WSTEXTCTRL_HANDLE(pThis), NULL, FALSE);
#endif
}
void WSTextCtrl_HandleContentEvent(WSTEXTCTRL* pThis,
	WSCONTENTEVENT* ev)
{
}

void WSTextCtrl_HandleGrowEvent(WSTEXTCTRL* pThis,
	WSGROWEVENT* ev)
{
}

void WSTextCtrl_HandleZoomEvent(WSTEXTCTRL* pThis,
	WSZOOMEVENT* ev)
{
}

void WSTextCtrl_HandleKeyEvent(WSTEXTCTRL* pThis, WSKEYEVENT* ev)
{
}

void WSTextCtrl_HandleIdle(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_HandleActivate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_HandleDeactivate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_HandleIBeamCursor(WSTEXTCTRL* pThis,
	WSIBEAMEVENT* er)
{
}

void WSTextCtrl_HandleMouseWheel(WSTEXTCTRL* pThis,
	WSMOUSEWHEELEVENT* er)
{
}

wsbool WSTextCtrl_CreateScrolled(WSTEXTCTRL* pThis, WSSCROLLEDWINDOW* pParent,
	WSWINDOW* pTopParent, WSRECT* r, WSWNDCLASS* pClass)
{
	int flags =
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | WS_HSCROLL
#if !defined(STBTEXTCTRL)
		| ES_MULTILINE
#endif
		;
	LPCSTR classname =
#if defined(STBTEXTCTRL)
		(LPCSTR)MAKEINTATOM(WSWNDCLASS_HANDLE(pClass));
#else
		"RICHEDIT";
#endif
		/*"RichEdit20A",*/
	(void)(pParent);
#if !defined(STBTEXTCTRL)
	/*		WSDLL_Create(&pThis->mRiched, "riched20.dll");*/
	WSDLL_Create(&pThis->mRiched, "riched32.dll");
#endif
	WSTEXTCTRL_HANDLE(pThis) = CreateWindowA(classname,
		NULL, flags,
		r->left, r->top, r->right, r->bottom, WSWINDOW_HANDLE(pTopParent),
		NULL, NULL, NULL);
#if !defined(STBTEXTCTRL)
	if (!WSTEXTCTRL_HANDLE(pThis))
	{
		/*Try normal edit control - no DLL loading required for it*/
		WSTEXTCTRL_HANDLE(pThis) = CreateWindowA("EDIT",
			NULL, WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL,
			r->left, r->top, r->right, r->bottom, WSWINDOW_HANDLE(pTopParent),
			NULL, NULL, NULL);
	}
#endif
	if (!WSTEXTCTRL_HANDLE(pThis))
	{
#if !defined(STBTEXTCTRL)
		WSDLL_Destroy(&pThis->mRiched);
#endif
		return FALSE;
	}
#if defined(STBTEXTCTRL)
	WSMutStr_Create(&statestr);
	stb_textedit_initialize_state( &state, 0 );
#else
	pThis->mTextLength = 0;
#endif
	return WSTEXTCTRL_HANDLE(pThis) != NULL;
}
void WSTextCtrl_Append(WSTEXTCTRL* pThis, const char* text, size_t textLen)
{
#if defined(STBTEXTCTRL)
	WSMutStr_AppendCStringWithLength(&statestr, text, textLen);
#else
	SendMessageA(WSTEXTCTRL_HANDLE(pThis), EM_SETSEL,
		(WPARAM)pThis->mTextLength, (LPARAM)pThis->mTextLength);
	SendMessageA(WSTEXTCTRL_HANDLE(pThis), EM_REPLACESEL,
		(WPARAM)FALSE, (LPARAM)text);
	pThis->mTextLength += textLen;
#endif
}
void WSTextCtrl_Activate(WSTEXTCTRL* pThis)
{
	(void)(pThis);
}
void WSTextCtrl_Deactivate(WSTEXTCTRL* pThis)
{
	(void)(pThis);
}
void WSTextCtrl_Update(WSTEXTCTRL* pThis, WSWINDOW* pParent, WSRECT* rect)
{
	(void)(pThis); (void)(pParent); (void)(rect);
}
void WSTextCtrl_SetSize(WSTEXTCTRL* pThis, long width, long height)
{
	RECT rclog;
	GetClientRect(WSTEXTCTRL_HANDLE(pThis), &rclog);
	SetWindowPos(WSTEXTCTRL_HANDLE(pThis), 0, 0, 0, width, height, 0);
	rclog.left = rclog.right;
	rclog.top = rclog.bottom;
	if (width > rclog.right)
		rclog.right = width;
	if (height > rclog.bottom)
		rclog.bottom = height;
	InvalidateRect(WSTEXTCTRL_HANDLE(pThis), &rclog, TRUE);
	/*MoveWindow(WSTEXTCTRL_HANDLE(pThis), 0, 0, width, height, TRUE);*/
	/*InvalidateRect(WSTEXTCTRL_HANDLE(pThis), &rclog, TRUE);*/
}
void* WSTextCtrl_GetHandle(WSTEXTCTRL* pThis)
{
	return WSTEXTCTRL_HANDLE(pThis);
}
void WSTextCtrl_SetText(WSTEXTCTRL* pThis, const char* text, size_t textLen)
{
	#if defined(STBTEXTCTRL)
		WSMutStr_AssignCStringWithLength(&statestr, text, textLen);
	#else
		#if 0 /* Increadibly slow for filled windows */
			nLength = GetWindowTextLengthA(mhLogText); */
		#else
			SendMessageA(WSTEXTCTRL_HANDLE(pThis), EM_SETSEL,
				(WPARAM)0, (LPARAM)pThis->mTextLength);
			SendMessageA(WSTEXTCTRL_HANDLE(pThis), EM_REPLACESEL,
				(WPARAM)FALSE, (LPARAM)text);
			pThis->mTextLength = textLen;
		#endif
	#endif
}

void WSTextCtrl_Destroy(WSTEXTCTRL* pThis)
{
	DestroyWindow(WSTEXTCTRL_HANDLE(pThis));
#if defined(STBTEXTCTRL)
	WSMutStr_Destroy(&statestr);
#else
	WSDLL_Destroy(&pThis->mRiched);
#endif
}

void WSTextCtrl_Idle(WSTEXTCTRL* pThis)
{
	(void)(pThis);
}


void WSMenuActions_Create(WSMENUACTIONS* pThis)
{	(void)(pThis); }
void WSMenuActions_Destroy(WSMENUACTIONS* pThis)
{	(void)(pThis); }

#undef WSMenu_Create
void WSMenu_Create(WSMENU* pThis, WSMENUSECTION* pSection)
{
	WSMENUSECTION_HANDLE(pSection) =
		WSMENU_HANDLE(pThis) = CreatePopupMenu();
}
void WSMenu_Destroy(WSMENU* pThis)
{
	DestroyMenu(WSMENU_HANDLE(pThis));
}
void WSMenu_Finalize(WSMENU* pThis, WSMENUSECTION* pSection)
{
	(void)(pThis); (void)(pSection);
}
void WSMenu_EndSection(WSMENU* pThis, WSMENUSECTION* pSection)
{
	(void)(pThis);
	AppendMenuA(WSMENUSECTION_HANDLE(pSection), MF_SEPARATOR, 0, NULL);
}
void WSMenu_SetItemTitle(WSMENU* pThis, size_t index, const char* title)
{
	ModifyMenuA(WSMENU_HANDLE(pThis), (UINT)index, MF_BYPOSITION | MF_STRING,
				index, title);
}
void WSMenu_Popup(WSMENU* pThis, WSWINDOW* pWindow)
{
	POINT pt;
	GetCursorPos(&pt);
	/* MSDN, PRB: */
	/* "Menus for Notification Icons Do Not Work Correctly" */
	/*1) Menu doesn't vanish if user clicks outside the menu*/
	/*	  SetForegroundWindow(hWnd)*/
	/*		before TrackPopupMenu is called*/
	/*2) If (1), then menu reappears instantly*/
	/*	  SendMessage(hWnd, WM_NULL, 0, 0)*/
	/* (These also fix a third bug where when */
	/*	the application quits it would switch*/
	/*	to the last application chosen)*/
	SetForegroundWindow(WSWINDOW_HANDLE(pWindow));
	TrackPopupMenu(WSMENU_HANDLE(pThis), 0, pt.x, pt.y, 0,
		WSWINDOW_HANDLE(pWindow), NULL);
	SendMessageA(WSWINDOW_HANDLE(pWindow), WM_NULL, 0, 0);
}
wsbool WSMenuSection_Append(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*mmes), WSWINDOW* window,
	WSMENUACTIONS* ag)
{
	(void)(mmes); (void)(window); (void)(ag);
	return AppendMenuA(WSMENU_HANDLE(pThis), MF_STRING, commandid, title);
}

wsbool WSMenuSection_AppendWithAccel(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*mmes), WSWINDOW* window,
	const char* accel, WSMENUACTIONS* ag)
{
	(void)(pThis);(void)(title);(void)(commandid);(void)(mmes);
	(void)(window);(void)(accel);(void)(ag);
	return FALSE;
}

void WSMenuBar_Create(WSMENUBAR* pThis)
{
	WSMENUBAR_HANDLE(pThis) = CreateMenu();
}
void WSMenuBar_Append(WSMENUBAR* pThis, WSMENU* pMenu, const char* title)
{
	AppendMenuA(WSMENUBAR_HANDLE(pThis), MF_POPUP,
		(LPARAM)WSMENU_HANDLE(pMenu), title);
}
void WSMenuBar_Draw(WSMENUBAR* pThis)
{
	(void)(pThis);
}
void WSMenuBar_Destroy(WSMENUBAR* pThis)
{
	DestroyMenu(WSMENUBAR_HANDLE(pThis));
}

wsbool WSApp_CreateGUI(WSAPP* pThis)
{
	(void)(pThis);
	return TRUE;
}
void WSApp_SetNotifyIconMenu(WSAPP* pThis, WSMENU* pMenu)
{
	(void)(pThis); (void)(pMenu);
}
void WSApp_SetNotifyIcon(WSAPP* pThis, const char* abspath, size_t abspathlen)
{
	(void)(pThis); (void)(abspath); (void)(abspathlen);
}
void WSApp_Destroy(WSAPP* pThis)
{
	(void)(pThis);
}
void WSApp_SetMenuBar(WSAPP* pThis, WSMENUBAR* pMenuBar, WSMENUACTIONS* ma)
{
	(void)(pThis); (void)(pMenuBar); (void)(ma);
}

#if defined(WIKISERVER_VERSION)
void WSAppWinSplitCmdLine(WSMUTSTR* firstArg,
	const char** argv, int* argc)
{
	const char* arg = GetCommandLineA();
	const char* e;
	if (arg[0] == '\"') /* Quotations surround when GUI launched */
	{
		e = CStrLocateChar(&arg[1], '\"');
		if (e)
			WSMutStr_AssignCStringWithLength(firstArg, &arg[1], e - &arg[1]);
	}
	else
	{ /* <exe><space><args> */
		e = CStrLocateChar(&arg[1], ' ');
		if(e)
			WSMutStr_AssignCStringWithLength(firstArg, &arg[0], e - &arg[0]);
#if 0 /*WinCE doesn't provide exe path in argv*/
		WCHAR wszPath[MAX_PATH]; /* 128 is MAX_PATH on CE */
		size_t realSize =
			GetModuleFileNameW(NULL, wszPath, MAX_PATH);
		if(realSize == 0)
		{
			ShowCreateError("wikiserver: could not get executable path");
			return FALSE;
		}
		else
		{
			CHAR* szPath = WSTRToSTRWithLength(wszPath, &realSize);
			if(!szPath)
				return FALSE;
			firstArg.Own(szPath, realSize, realSize + 1);
		}
#endif
	}
	/* e points to ending quote, set it to next arg if there is one */
	argv[0] = WSMutStr_GetCString(firstArg);
	if(e && *(e + 1))
	{
		argv[1] = e + 2;
		*argc = 2;
	}
	else
		*argc = 1;
}
#endif


/*=============================================================================*/
/* WSLaunchURL() */
/**/
/* UWP apps can't call cmd.exe/launch other programs, so use WinRT in UWP */
/*=============================================================================*/
/* https://stackoverflow.com/questions/7436144/using-winrt-from-c */
/* The following is the equivelent of  */
#if 0
	#if defined(WSMSIXOPT)
	#include <winrt/Windows.Foundation.h>
	#include <winrt/Windows.System.h>
	#endif
	using namespace winrt;
	using namespace Windows::Foundation;
	using namespace Windows::System;
	wsbool result;
	size_t wlen = urllen;
	WCHAR* w = STRToWSTRWithLength(url, &wlen);
	result = Launcher::LaunchUriAsync(Uri(w)); /* .get(); would block here */
	WSFREE(w);
#endif
#if defined(WSMSIXOPT)
	#ifdef __cplusplus /* roapi.h has templates which can't be C linkage */
		}
	#else
	#endif
	#include <roapi.h> /* RoInitialize/RoGetActivationFactory/RoUninitialize */
	#include <winstring.h> /* WinCreateString */
	#ifdef __cplusplus
		extern "C" {
	#endif
#pragma comment(lib, "runtimeobject.lib") /* RoXXX */
typedef interface __x_ABI_CWindows_CFoundation_CIUriRuntimeClass __x_ABI_CWindows_CFoundation_CIUriRuntimeClass;
typedef interface __x_ABI_CWindows_CSystem_CILauncherStatics __x_ABI_CWindows_CSystem_CILauncherStatics;

typedef struct __x_ABI_CWindows_CFoundation_CIWwwFormUrlDecoderRuntimeClass __x_ABI_CWindows_CFoundation_CIWwwFormUrlDecoderRuntimeClass;
interface __x_ABI_CWindows_CFoundation_CIUriRuntimeClass
{
	CONST_VTBL struct __x_ABI_CWindows_CFoundation_CIUriRuntimeClassVtbl* lpVtbl;
};
typedef struct __x_ABI_CWindows_CFoundation_CIUriRuntimeClassVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE* QueryInterface)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		REFIID riid,
		void** ppvObject);
	ULONG(STDMETHODCALLTYPE* AddRef)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This);
	ULONG(STDMETHODCALLTYPE* Release)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This);
	HRESULT(STDMETHODCALLTYPE* GetIids)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		ULONG* iidCount,
		IID** iids);
	HRESULT(STDMETHODCALLTYPE* GetRuntimeClassName)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* className);
	HRESULT(STDMETHODCALLTYPE* GetTrustLevel)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		TrustLevel* trustLevel);
	HRESULT(STDMETHODCALLTYPE* get_AbsoluteUri)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_DisplayUri)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_Domain)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_Extension)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_Fragment)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_Host)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_Password)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_Path)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_Query)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_QueryParsed)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		__x_ABI_CWindows_CFoundation_CIWwwFormUrlDecoderRuntimeClass** ppWwwFormUrlDecoder);
	HRESULT(STDMETHODCALLTYPE* get_RawUri)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_SchemeName)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_UserName)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING* value);
	HRESULT(STDMETHODCALLTYPE* get_Port)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		INT32* value);
	HRESULT(STDMETHODCALLTYPE* get_Suspicious)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		boolean* value);
	HRESULT(STDMETHODCALLTYPE* Equals)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* pUri,
		boolean* value);
	HRESULT(STDMETHODCALLTYPE* CombineUri)(__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* This,
		HSTRING relativeUri,
		__x_ABI_CWindows_CFoundation_CIUriRuntimeClass** instance);

	END_INTERFACE
} __x_ABI_CWindows_CFoundation_CIUriRuntimeClassVtbl;

typedef interface __FIAsyncOperation_1_boolean __FIAsyncOperation_1_boolean;
typedef interface __FIAsyncOperationCompletedHandler_1_boolean __FIAsyncOperationCompletedHandler_1_boolean;
typedef struct __FIAsyncOperation_1_booleanVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE* QueryInterface)(__FIAsyncOperation_1_boolean* This,
		REFIID riid,
		void** ppvObject);
	ULONG(STDMETHODCALLTYPE* AddRef)(__FIAsyncOperation_1_boolean* This);
	ULONG(STDMETHODCALLTYPE* Release)(__FIAsyncOperation_1_boolean* This);
	HRESULT(STDMETHODCALLTYPE* GetIids)(__FIAsyncOperation_1_boolean* This,
		ULONG* iidCount,
		IID** iids);
	HRESULT(STDMETHODCALLTYPE* GetRuntimeClassName)(__FIAsyncOperation_1_boolean* This,
		HSTRING* className);
	HRESULT(STDMETHODCALLTYPE* GetTrustLevel)(__FIAsyncOperation_1_boolean* This,
		TrustLevel* trustLevel);
	HRESULT(STDMETHODCALLTYPE* put_Completed)(__FIAsyncOperation_1_boolean* This,
		__FIAsyncOperationCompletedHandler_1_boolean* handler);
	HRESULT(STDMETHODCALLTYPE* get_Completed)(__FIAsyncOperation_1_boolean* This,
		__FIAsyncOperationCompletedHandler_1_boolean** result);
	HRESULT(STDMETHODCALLTYPE* GetResults)(__FIAsyncOperation_1_boolean* This,
		boolean* result);

	END_INTERFACE
} __FIAsyncOperation_1_booleanVtbl;

interface __FIAsyncOperation_1_boolean
{
	CONST_VTBL struct __FIAsyncOperation_1_booleanVtbl* lpVtbl;
};

typedef struct __x_ABI_CWindows_CStorage_CIStorageFile __x_ABI_CWindows_CStorage_CIStorageFile;
typedef struct __x_ABI_CWindows_CSystem_CILauncherOptions __x_ABI_CWindows_CSystem_CILauncherOptions;
interface __x_ABI_CWindows_CSystem_CILauncherStatics
{
	CONST_VTBL struct __x_ABI_CWindows_CSystem_CILauncherStaticsVtbl* lpVtbl;
};
typedef struct __x_ABI_CWindows_CSystem_CILauncherStaticsVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE* QueryInterface)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This,
		REFIID riid,
		void** ppvObject);
	ULONG(STDMETHODCALLTYPE* AddRef)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This);
	ULONG(STDMETHODCALLTYPE* Release)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This);
	HRESULT(STDMETHODCALLTYPE* GetIids)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This,
		ULONG* iidCount,
		IID** iids);
	HRESULT(STDMETHODCALLTYPE* GetRuntimeClassName)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This,
		HSTRING* className);
	HRESULT(STDMETHODCALLTYPE* GetTrustLevel)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This,
		TrustLevel* trustLevel);
	HRESULT(STDMETHODCALLTYPE* LaunchFileAsync)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This,
		__x_ABI_CWindows_CStorage_CIStorageFile* file,
		__FIAsyncOperation_1_boolean** operation);
	HRESULT(STDMETHODCALLTYPE* LaunchFileWithOptionsAsync)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This,
		__x_ABI_CWindows_CStorage_CIStorageFile* file,
		__x_ABI_CWindows_CSystem_CILauncherOptions* options,
		__FIAsyncOperation_1_boolean** operation);
	HRESULT(STDMETHODCALLTYPE* LaunchUriAsync)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This,
		__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* uri,
		__FIAsyncOperation_1_boolean** operation);
	HRESULT(STDMETHODCALLTYPE* LaunchUriWithOptionsAsync)
		(__x_ABI_CWindows_CSystem_CILauncherStatics* This,
		__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* uri,
		__x_ABI_CWindows_CSystem_CILauncherOptions* options,
		__FIAsyncOperation_1_boolean** operation);

	END_INTERFACE
} __x_ABI_CWindows_CSystem_CILauncherStaticsVtbl;


typedef interface __x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory __x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory;
typedef struct __x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactoryVtbl
{
	BEGIN_INTERFACE

	HRESULT(STDMETHODCALLTYPE* QueryInterface)
		(__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory* This,
		REFIID riid,
		void** ppvObject);
	ULONG(STDMETHODCALLTYPE* AddRef)
		(__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory* This);
	ULONG(STDMETHODCALLTYPE* Release)
		(__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory* This);
	HRESULT(STDMETHODCALLTYPE* GetIids)
		(__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory* This,
		ULONG* iidCount,
		IID** iids);
	HRESULT(STDMETHODCALLTYPE* GetRuntimeClassName)
		(__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory* This,
		HSTRING* className);
	HRESULT(STDMETHODCALLTYPE* GetTrustLevel)
		(__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory* This,
		TrustLevel* trustLevel);
	HRESULT(STDMETHODCALLTYPE* CreateUri)
		(__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory* This,
		HSTRING uri,
		__x_ABI_CWindows_CFoundation_CIUriRuntimeClass** instance);
	HRESULT(STDMETHODCALLTYPE* CreateWithRelativeUri)
		(__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory* This,
		HSTRING baseUri,
		HSTRING relativeUri,
		__x_ABI_CWindows_CFoundation_CIUriRuntimeClass** instance);

	END_INTERFACE
} __x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactoryVtbl;

interface __x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory
{
	CONST_VTBL struct
		__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactoryVtbl* lpVtbl;
};
#ifdef __cplusplus
	#define IActivationFactory_Release(x) (x)->Release()
	#define IActivationFactory_QueryInterface(x, y, z) \
		(x)->QueryInterface(y, z)
#else
	#define IActivationFactory_Release(x) (x)->lpVtbl->Release(x)
	#define IActivationFactory_QueryInterface(x, y, z) \
		(x)->lpVtbl->QueryInterface(x, y, z)
#endif
#endif
wsbool WSLaunchURL(const char* url, size_t urllen)
{
#if defined(WSMSIXOPT)
	/* ShellExecute is banned from UWP, use WinRT instead. */
	HRESULT hr;
	HSTRING hstrLauncherClass = NULL, hstrUriClass = NULL;
	HSTRING hstrUri = NULL;
	IActivationFactory* pLauncherActivationFactory = NULL;
	IActivationFactory* pUriActivationFactory = NULL;
	__x_ABI_CWindows_CSystem_CILauncherStatics* pLauncherStatics = NULL;
	__x_ABI_CWindows_CFoundation_CIUriRuntimeClass* pUri = NULL;
	__x_ABI_CWindows_CFoundation_CIUriRuntimeClassFactory* pUriFactory = NULL;
	__FIAsyncOperation_1_boolean* pResult = NULL;
	WCHAR* wUrl;
	IID IID_ILauncherStatics = { 0x277151C3, 0x9E3E, 0x42F6,
		{0x91, 0xA4, 0x5D, 0xFD, 0xEB, 0x23, 0x24, 0x51} };
	/*IID IID_IUriRuntimeClass = { 0x9e365e57, 0x48b2, 0x4160,
		{0x95, 0x6f, 0xc7, 0x38, 0x51, 0x20, 0xbb, 0xfc} };*/
	IID IID_IUriRuntimeClassFactory = { 0x44a9796f, 0x723e, 0x4fdf,
		{0xa2, 0x18, 0x03, 0x3e, 0x75, 0xb0, 0xc0, 0x84} };
	hr = RoInitialize(RO_INIT_MULTITHREADED);
	if (FAILED(hr))
		return FALSE;
	/*
		WinRT is inited. Create fully qualified instance names to create
		an IActivationFactory, which we then QueryInterface of for the WinRT
		object. Uri isn't created directly, instead it is created via
		IUriFactory, a private interface. Then we pass the Uri to LaunchAsync
	*/
	hr = WindowsCreateString(L"Windows.System.Launcher",
		strcountof(L"Windows.System.Launcher"), &hstrLauncherClass);
	if(SUCCEEDED(hr))
		hr = WindowsCreateString(L"Windows.Foundation.Uri",
			strcountof(L"Windows.Foundation.Uri"), &hstrUriClass);
	if(SUCCEEDED(hr))
		hr = RoGetActivationFactory(hstrLauncherClass,
			WSIIDPARAM(IID_IActivationFactory),
			(void**)&pLauncherActivationFactory);
	if(SUCCEEDED(hr))
		hr = RoGetActivationFactory(hstrUriClass,
			WSIIDPARAM(IID_IActivationFactory),
			(void**)&pUriActivationFactory);
	if(SUCCEEDED(hr))
		hr = IActivationFactory_QueryInterface(pLauncherActivationFactory,
				WSIIDPARAM(IID_ILauncherStatics), (void**)&pLauncherStatics);
	if(SUCCEEDED(hr))
		hr = IActivationFactory_QueryInterface(pUriActivationFactory,
				WSIIDPARAM(IID_IUriRuntimeClassFactory), (void**)&pUriFactory);
	if (SUCCEEDED(hr))
	{ /* Get the windows HSTRING for the Uri creation */
		wUrl = STRToWSTRWithLength(url, &urllen);
		if (wUrl)
		{
			hr = WindowsCreateString(wUrl, urllen, &hstrUri);
			WSFREE(wUrl);
		}
		else
			hr = E_ABORT;
	}
	if(SUCCEEDED(hr))
		hr = pUriFactory->lpVtbl->CreateUri(pUriFactory, hstrUri, &pUri);
	if(SUCCEEDED(hr))
		hr = pLauncherStatics->lpVtbl->LaunchUriAsync(
			pLauncherStatics, pUri, &pResult);
	if (hstrLauncherClass) WindowsDeleteString(hstrLauncherClass);
	if (hstrUri) WindowsDeleteString(hstrUri);
	if (hstrUriClass) WindowsDeleteString(hstrUriClass);
	if (pLauncherStatics) pLauncherStatics->lpVtbl->Release(pLauncherStatics);
	if (pLauncherActivationFactory)
		IActivationFactory_Release(pLauncherActivationFactory);
	if (pUriActivationFactory)
		IActivationFactory_Release(pUriActivationFactory);
	if (pUriFactory) pUriFactory->lpVtbl->Release(pUriFactory);
	if (pUri) pUri->lpVtbl->Release(pUri);
	if (pResult) pResult->lpVtbl->Release(pResult);
	RoUninitialize();
	return SUCCEEDED(hr);
#else
	typedef HINSTANCE(WINAPI* SHELLEXECUTEAPROC)(HWND hwnd,
		LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters,
		LPCSTR lpDirectory, INT nShowCmd);
	WSDLL shell32;
	longptr_t nResult;
	SHELLEXECUTEAPROC lpShellExecuteA;
	(void)(urllen);
	WSDLL_CreateShell32(&shell32);
	if (!WSDLL_IsLoaded(&shell32))
		return FALSE;
	lpShellExecuteA =
		EXPLICITPROCCAST(SHELLEXECUTEAPROC)
		WSDLL_GetProc(&shell32, "ShellExecuteA");
	if (lpShellExecuteA)
	{
		/* Windows sometimes doesn't open the browser correctly */
		/* when using mime types, so do ShellExecute - i.e. start <url> */
		nResult = (longptr_t)
			(*lpShellExecuteA)(NULL, NULL, url, NULL, "", SW_SHOWNORMAL);
	}
	else
		nResult = HINSTANCE_ERROR;
	WSDLL_DestroyShell32(&shell32); /* done with shell32.dll, unload it */
	/* Hack for Firefox (returns file not found for some reason) */
	/* HINSTANCE_ERROR == 32 */
	if (nResult <= HINSTANCE_ERROR && nResult != 2) /*2==SE_ERR_FNF*/
		return FALSE;
	if (nResult == 2) /* Log something if SE_ERR_FNF(==2) happens */
	{
		WSLogDebug("Got SE_ERR_FNF from ShellExecute - maybe FireFox");
	}
	return TRUE;
#endif /* WSMSIXOPT */
}

/*===========================================================================*/
/* WSMessageBox() */
/*===========================================================================*/

int WSMessageBox(WSWINDOW* pParent,
	const char* title, const char* message, int ops)
{
	HWND hWndParent;
	if(pParent)
		hWndParent = WSWINDOW_HANDLE(pParent);
	else
		hWndParent = NULL;
	return MessageBoxA(hWndParent, title, message, ops);
}

/*===========================================================================*/
/* WSDebugMemoryXXX() */
/*===========================================================================*/
void WSDebugMemoryBegin()
{
#if defined(_DEBUG) && defined(_MSC_VER) \
	&& !defined(NOCRUNTIME) && !defined(WSMEMORYDEBUGOPT)
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)
			| _CRTDBG_DELAY_FREE_MEM_DF
			| _CRTDBG_LEAK_CHECK_DF); /* _CRTDBG_CHECK_ALWAYS_DF; */
		/* disable retry dialog */
		SetProcessShutdownParameters(0x100, SHUTDOWN_NORETRY);
#endif
}
void WSDebugMemoryEnd()
{
#if defined(_DEBUG) && defined(_MSC_VER) \
	&& !defined(NOCRUNTIME) && !defined(WSMEMORYDEBUGOPT)
		_CrtDumpMemoryLeaks();
#endif
}

/*===========================================================================*/
/*						WSGetDPI()*/
/*===========================================================================*/
void WSDisplayGetScaleFactor(guipos_t* x, guipos_t* y)
{
	WSDisplayGetDPI(x, y);
	*x /= 96;
	*y /= 96;
}

void WSDisplayGetDPI(guipos_t* x, guipos_t* y)
{/* MS Recommends (DPI * SIZE / 96.f). 96 is the default */
	typedef BOOL(WINAPI* SETPROCESSWSDPIAWAREOPTPROC) ();
	typedef int (WINAPI* GETDEVICECAPSPROC)(HDC hdc, int index);
	HDC hdc;
	WSDLL user32;
	SETPROCESSWSDPIAWAREOPTPROC lpSetProcessDPIAware;
	GETDEVICECAPSPROC lpGetDeviceCaps;
	WSDLL gdi32;


	WSDLL_CreateUser32(&user32);
	lpSetProcessDPIAware =
		(SETPROCESSWSDPIAWAREOPTPROC)WSDLL_GetProc(
			&user32, "SetProcessDPIAware");
	if (lpSetProcessDPIAware)
		(*lpSetProcessDPIAware)();
	WSDLL_DestroyUser32(&user32);

	/* Without calling SetProcessDPIAware LOGPIXELSX/Y will always be 96 */
	WSDLL_CreateGdi32(&gdi32);
	lpGetDeviceCaps = (GETDEVICECAPSPROC)
		WSDLL_GetProc(&gdi32, "GetDeviceCaps");
	hdc = GetDC(NULL);
	*x = (*lpGetDeviceCaps)(hdc, LOGPIXELSX);
	*y = (*lpGetDeviceCaps)(hdc, LOGPIXELSY);
	ReleaseDC(NULL, hdc);
	WSDLL_DestroyGdi32(&gdi32);
}
/*===========================================================================*/
/*	WinMain*/
/*===========================================================================*/
#if (defined(_WINDOWS) && !defined(_CONSOLE)) \
	|| (!defined(_WINDOWS) && !defined(_CONSOLE))
	#ifndef NOCRUNTIME
		int __stdcall WinMain(
			struct HINSTANCE__* hInstance, /* GetModuleHandle(NULL) */
			struct HINSTANCE__*	hPrevInstance, /* NULL */
			char*				lpCmdLine, /* GetCommandLine */
		/* GetStartupInfoW dwFlags & STARTF_USESHOWWINDOW ?
							wShowWindow : SW_SHOWDEFAULT */
			int		  nCmdShow)
	#else
		#ifdef __GNUC__
			WSEXTERNC int mainCRTStartup()
		#else
			WSEXTERNC int WinMainCRTStartup()
		#endif
	#endif /* NOCRUNTIME */
	{
	#ifdef NOCRUNTIME
		HINSTANCE hInstance = WSWinGetExeHMODULE();
		int exitCode = /*WikiServerWindows::WinMainStartup(hInstance);*/
						WSApp_BasicMain(hInstance);
	#else
		int exitCode = WSApp_BasicMain(hInstance, lpCmdLine);
		(void)(hPrevInstance); (void)(nCmdShow); /* req for VC6 */
	#endif /* NOCRUNTIME */
	#ifdef NOCRUNTIME
		ExitProcess(exitCode);
	#	if (defined(_MSC_VER) && (_MSC_VER < 1300)) \
			|| (defined(__MWERKS__))
		return exitCode;
	#	endif
	#else
		return exitCode;
	#endif /* NOCRUNTIME */
	}
#else /* Console entry */
	#ifdef NOCRUNTIME
		WSEXTERNC int mainCRTStartup()
	#else
		int main(int argc, char** argv)
	#endif /* NOCRUNTIME */
	{
		HINSTANCE hInstance = WSWinGetExeHMODULE();
	#ifdef NOCRUNTIME
		int exitCode = WSApp_BasicMain(hInstance);
		ExitProcess(exitCode);
	#	if defined(_MSC_VER) && (_MSC_VER < 1300)
		return exitCode;
	#	endif
	#else
		return WSApp_BasicMain(hInstance, argc, argv);
	#endif /* NOCRUNTIME */
	}
#endif /* _WINDOWS/_CONSOLE switch */
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
			 #     #                  #######  #####  #     #
			 ##   ##   ##    ####     #     # #     #  #   #
			 # # # #  #  #  #    #    #     # #         # #
			 #  #  # #    # #         #     #  #####     #
			 #     # ###### #         #     #       #   # #
			 #     # #    # #    #    #     # #     #  #   #
			 #     # #    #  ####     #######  #####  #     #

				  #####  #######  #####  #######    #
				 #     # #     # #     # #     #   # #
				 #       #     # #       #     #  #   #
				 #       #     # #       #     # #     #
				 #       #     # #       #     # #######
				 #     # #     # #     # #     # #     #
				  #####  #######  #####  ####### #     #
=============================================================================*/
#elif defined(__MACH__)
#include <CoreFoundation/CoreFoundation.h>
#if (!defined(TARGET_OS_IOS) || !TARGET_OS_IOS)
#import <Cocoa/Cocoa.h>
#include <mach-o/dyld.h>
#include <string.h>

/*__has_feature(objc_arc)*/
#if (defined(TARGET_OS_IOS) && TARGET_OS_IOS)
#define WSBRIDGERETAINED __bridge_retained
#define WSBRIDGE __bridge
#else
#define WSBRIDGERETAINED
#define WSBRIDGE
#endif

/* We aren't concerned with deprecated calls as they are necessary on old OSX */
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#define WSWINDOW_HANDLE(pThis) (*((WSNSWindow**)pThis))
#define WSWINDOW_NSWINDOW_HANDLE(pThis) (*((NSWindow**)pThis))
#define WSSCROLLEDWINDOW_HANDLE(pThis) (*((NSScrollView**)pThis))
#define WSTEXTCTRL_HANDLE(pThis) (*((WSNSTextView**)pThis))
#define WSMENU_HANDLE(pThis) (*((NSMenu**)pThis))
#define WSMENUBAR_HANDLE(pThis) (*((NSMenu**)pThis))
#define WSMENUSECTION_HANDLE(pThis) (*((NSMenu**)pThis))
#define WSAPP_HANDLE(x) (*((WSNSApplicationDelegate**)&(x)->mDelegate))
#define WSDC_HANDLE(x)
#define WSICON_HANDLE(x) (*((NSImage**)pThis))

#if !defined(WIKISERVER_VERSION)
	#define StringDup strdup
#endif

NSString* WSCFStrToNSString(CFStringRef cfstring)
{
  return (WSBRIDGE NSString*)cfstring;
}

/*
CFStringRef WSCStrToCFString(const char* cstr, size_t cstrlen)
{
	return CFStringCreateWithBytes(kCFAllocatorDefault,
								 (const UInt8*)cstr, cstrlen,
									  kCFStringEncodingUTF8, FALSE);
}
*/

#if 0 /* not in c++, and not needed really */
CFStringRef WSMutStrToCFString(const MutStr* mutstr)
{
  return WSCStrToCFString(mutstr->c_str(), mutstr->length());
}
void WSCFStringToMutStr(MutStr* mutstr, CFStringRef cfstring)
{CFStringGetSystemEncoding();
  CFStringEncoding cfenc = kCFStringEncodingUTF8;
  CFIndex cfstrfinallen;
  Size cfstrlen = CFStringGetLength(cfstring);
  CFStringGetBytes(cfstring, CFRangeMake(9, cfstringlen),
				   cfenc, '?', FALSE, NULL, 0, &cfstrfinallen);
  mutstr->reserve(cfstrfinallen);
  mutstr->SetLength(cfstrfinallen);
  CFStringGetBytes(cfstring, CFRangeMake(9, cfstringlen),
				   cfenc, '?', FALSE,
				   (unsigned char*)mutstr->c_str(),
				   0, &cfstrfinallen);
}
#endif

NSString* WSCStrToNSString(const char* cstr)
{
//  return WSCFStrToNSString(WSCStrToCFString(cstr, cstrlen));
	return [NSString stringWithUTF8String:cstr];

}

static NSScreen* WSGetMainScreen()
{
  if([NSScreen screens] == NULL)
	return [NSScreen mainScreen];
  else
	return [[NSScreen screens] objectAtIndex:0];
}
void WSAdjustNSRectToCenter(NSRect* rect)
{
	NSSize screensize = [WSGetMainScreen() frame].size;
	rect->origin.x = (screensize.width-rect->size.width)/2;
	rect->origin.y = (screensize.height-rect->size.height)/2;
}

void WSNSRectChildToParent(NSView* parent, NSRect* rect)
{
  NSRect parentFrame;
  if(parent)
	parentFrame = [parent bounds];
  else
	parentFrame = [WSGetMainScreen() frame];

  rect->origin.x = parentFrame.origin.x;
  rect->origin.y = parentFrame.origin.y;
  if(parent == NULL || [parent isFlipped] == FALSE)
	rect->origin.y = (parentFrame.size.height -
					  (parentFrame.origin.y + rect->size.height));
}

void WSNSPointChildtoParent(NSView* parent, NSPoint* point)
{
  NSRect rect;
  WSNSRectChildToParent(parent, &rect);
  point->x = rect.origin.x;
  point->y = rect.origin.y;
}

void WSNSRectToRect(WSRECT* rect, NSRect* nsrect)
{
	*rect = *(WSRECT*)nsrect;
}
void WSRectToNSRect(NSRect* nsrect, WSRECT* rect)
{
	*nsrect = *(NSRect*)rect;
}

void WSRectChildToParent(NSView* parent, WSRECT* rect)
{
  NSRect nsrect;
  /*Could mess around with
	GetThemeMetric(kThemeMetricListBoxFrameOutset , &SInt32); */
  WSRectToNSRect(&nsrect, rect);
  WSNSRectChildToParent(parent, &nsrect);
  WSNSRectToRect(rect, &nsrect);
}

int WSMessageBox(WSWINDOW* pParent,
	const char* ctitle, const char* cmessage, int flags)
{
	#if 1 /* 10.0+ */
		long ret; int defret, altret, otherret;
		NSString* title;
		NSString* message;
		NSString* def;
		NSString* alt;
		NSString* other;
		NSWindow* nsparent = pParent ?
			WSWINDOW_NSWINDOW_HANDLE(pParent) : NULL;
		title = WSCStrToNSString(ctitle);
		message = WSCStrToNSString(cmessage);

		if (flags & WSMESSAGEBOX_OKCANCEL)
		{
			def = @"OK";
			alt = @"Cancel"; other = NULL;
			defret = WSMESSAGEBOX_IDOK; altret = WSMESSAGEBOX_IDCANCEL;
			otherret = 0;
		}
		else if (flags & WSMESSAGEBOX_ABORTRETRYIGNORE)
		{
			def = @"Abort";
			alt = @"Retry";
			other = @"Ignore";
			defret = WSMESSAGEBOX_IDABORT; altret = WSMESSAGEBOX_IDRETRY;
			otherret = WSMESSAGEBOX_IDIGNORE;
		}
		else if (flags & WSMESSAGEBOX_YESNOCANCEL)
		{
			def = @"Yes";
			alt = @"No";
			other = @"Cancel";
			defret = WSMESSAGEBOX_IDYES; altret = WSMESSAGEBOX_IDNO;
			otherret = WSMESSAGEBOX_IDCANCEL;
		}
		else if (flags & WSMESSAGEBOX_YESNO)
		{
			def = @"Yes";
			alt = @"No"; other = NULL;
			defret = WSMESSAGEBOX_IDYES; altret = WSMESSAGEBOX_IDNO;
			otherret = 0;
		}
		else if (flags & WSMESSAGEBOX_RETRYCANCEL)
		{
			def = @"Retry";
			alt = @"Cancel"; other = NULL;
			defret = WSMESSAGEBOX_IDRETRY; altret = WSMESSAGEBOX_IDCANCEL;
			otherret = 0;
		}
		else /* (flags & WSMESSAGEBOX_OK) */
		{
			def = @"OK"; alt = NULL; other = NULL;
			defret = WSMESSAGEBOX_IDOK; altret = 0; otherret = 0;
		}

		if (flags & WSMESSAGEBOX_ICONEXCLAMATION)
			ret = NSRunAlertPanelRelativeToWindow(title, @"%@",
				def, alt, other, nsparent, message);
		else if (flags & WSMESSAGEBOX_ICONHAND)
			ret = NSRunCriticalAlertPanelRelativeToWindow(title, @"%@",
				def, alt, other, nsparent, message);
		else
			ret = NSRunInformationalAlertPanelRelativeToWindow(title, @"%@",
				def, alt, other, nsparent, message);
		switch(ret)
		{
			case NSAlertDefaultReturn:
				return defret;
			case NSAlertAlternateReturn:
				return altret;
			case NSAlertOtherReturn:
				return otherret;
			default: /*NSAlertErrorReturn*/
				return 0;
		}
	#else /* 10.3+ */
		NSAlert *alert = [NSAlert alloc];
		if(!alert)
			return 0;
		alert = [alert init];
		if(!alert)
			return 0;
		[alert setMessageText:WSCStrToNSString(title)];
		[alert setInformativeText:WSCStrToNSString(message)];
		if(flags & MB_YESNO)
		{
			[alert addButtonWithTitle:@"Yes"];
			[alert addButtonWithTitle:@"No"];
		}
		else if (flags & MB_OKCANCEL)
		{
			[alert addButtonWithTitle:@"OK"];
			[alert addButtonWithTitle:@"Cancel"];
		}
		else
			[alert addButtonWithTitle:@"OK"];

		return [alert runModal];
	#endif
}

/*========================= WSEVENTLOOP =================*/
wsbool WSEventLoop_Create(WSEVENTLOOP* pThis)
{
	return TRUE;
}
void WSEventLoop_Destroy(WSEVENTLOOP* pThis)
{
}
wsbool WSEventLoop_RunSingle(WSEVENTLOOP* pThis)
{ /* CFRunLoop can disable input apparently, go with NSApplication */
	NSEvent *event = [NSApp
		  nextEventMatchingMask:NSAnyEventMask
		  untilDate:NULL /* [NSDate distantFuture] blocks forever */
		  inMode:NSDefaultRunLoopMode
		  dequeue: YES];
	if(event)
	{
		*pThis = (WSEVENTLOOP)event;
		[NSApp sendEvent: event];
		return TRUE;
	}

  /*Says it is not called in NSEventTrackingRunLoopMode,
  which we don't do
  [NSApp updateWindows];*/
  /*
  [NSApp sendAction:applicationShouldTerminate
				target:NSApp sender:NSApp] == FALSE;
	*/
	return FALSE;
}
wsbool WSEventLoop_IsQuitMessage(WSEVENTLOOP* pThis)
{
	return FALSE;
}

/*=============================================================================*/
/*=================================GUI=========================================*/
/*=============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------*/
/*						WS[XXX]Event methods								   */
/*-----------------------------------------------------------------------------*/
#define WSWindowFromHandle(hWnd) ((struct WSWINDOW_**)hWnd)
WSWINDOW* WSEvent_GetWindow(WSEVENT* ev)
{ return WSWindowFromHandle(ev->mWindow); }
void* WSEvent_GetWindowCon(WSEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mWindow)); }
WSWINDOW* WSPaintEvent_GetWindow(WSPAINTEVENT* ev)
{ return WSWindowFromHandle(ev->mWindow); }
void* WSPaintEvent_GetWindowCon(WSPAINTEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mWindow)); }
WSWINDOW* WSKeyEvent_GetWindow(WSKEYEVENT* ev)
{ return WSWindowFromHandle(ev->mWindow); }
void* WSKeyEvent_GetWindowCon(WSKEYEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mWindow)); }
size_t WSKeyEvent_GetKey(WSKEYEVENT* ev)
{ return 0; }
WSWINDOW* WSCloseEvent_GetWindow(WSCLOSEEVENT* ev)
{ return WSWindowFromHandle(ev->mWindow); }
void* WSCloseEvent_GetWindowCon(WSCLOSEEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mWindow)); }
WSWINDOW* WSNotifyIconEvent_GetWindow(WSNOTIFYICONEVENT* ev)
{ return WSWindowFromHandle(ev->mWindow); }
void* WSNotifyIconEvent_GetWindowCon(WSNOTIFYICONEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mWindow)); }
WSWINDOW* WSMenuEvent_GetWindow(WSMENUEVENT* ev)
{ return WSWindowFromHandle(ev->mWindow); }
void* WSMenuEvent_GetWindowCon(WSMENUEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mWindow)); }
WSWINDOW* WSSizeEvent_GetWindow(WSSIZEEVENT* ev)
{ return WSWindowFromHandle(ev->mWindow); }
void* WSSizeEvent_GetWindowCon(WSSIZEEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mWindow)); }
guipos_t WSSizeEvent_GetWidth(WSSIZEEVENT* ev)
{ return 0; }
guipos_t WSSizeEvent_GetHeight(WSSIZEEVENT* ev)
{ return 0; }
WSWINDOW* WSDestroyEvent_GetWindow(WSDESTROYEVENT* ev)
{ return WSWindowFromHandle(ev->mWindow); }
void* WSDestroyEvent_GetWindowCon(WSDESTROYEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mWindow)); }

/*=================================WSICON====================================*/
void WSIcon_CreateFromResource(WSICON* pThis, WSAPP* pApp, const char* path)
{
}
void WSIcon_CreateFromFile(WSICON* pThis, const char* path, size_t pathlen)
{
	WSICON_HANDLE(pThis) = [[NSImage alloc] initWithContentsOfFile:
						  WSCStrToNSString(path)];
}
void WSIcon_Destroy(WSICON* pThis)
{
	[WSICON_HANDLE(pThis) release];
}

/*=================================WSWNDCLASS====================================*/
#undef WSWndClass_Create
wsbool WSWndClass_Create(WSWNDCLASS* pThis, WSAPP* pApp, WSICON* pWindowIcon,
	const char* wndclassname)
{
	pThis->mWindowIcon = pWindowIcon;
	return TRUE;
}
void WSWndClass_Destroy(WSWNDCLASS* pThis, WSAPP* pApp)
{
}

/*=================================WSWINDOW====================================*/
@interface WSNSWindow : NSWindow<NSWindowDelegate>
{
	NSMenu* mMenuBar;
	NSMenu* mWindowsMenu;
	void* mCon;
	wsbool mIsVisible;
}
-(void)setMenuBar:(NSMenu*)menuBar;
-(void*)getCon;
-(void)setCon:(void*)con;
-(wsbool)getIsReallyVisible;
@end
@implementation WSNSWindow
+(void)populateWindowMenu:(NSMenu *)aMenu
{
	NSMenuItem *menuItem;

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			NSLocalizedString(@"Minimize", nil)
		action:@selector(miniaturize:)
		keyEquivalent:@"m"];
	[menuItem setKeyEquivalentModifierMask:NSCommandKeyMask];

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			NSLocalizedString(@"Zoom", nil)
		action:@selector(zoom:)
		keyEquivalent:@""];

	/*
		Full screen for NSView is generally only in the menu in 10.5 as
		enter/exitFullScreenMode is only available then.
		However fullscreen is available for NSWindow is some from in 10.2+
		floor(NSAppKitVersionNumber) >= NSAppKitVersionNumber10_5
		620 == OSX 10.1
	*/
	//if([NSView respondsToSelector:@selector(exitFullScreenModeWithOptions:)])
	//if([NSWindow collectionBehavior] & NSWindowCollectionBehaviorFullScreen])
	//SetSystemUIMode(0,0);
	if(NSIsSymbolNameDefined("_SetSystemUIMode"))
	{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundeclared-selector"
		menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
				NSLocalizedString(@"Fullscreen", nil)
			action:@selector(fullscreen:)
			keyEquivalent:@"F"];
#pragma clang diagnostic pop
		[menuItem setKeyEquivalentModifierMask:NSCommandKeyMask];
		[menuItem setTarget:[NSApp delegate]];
	}

/*	[aMenu addItem:[NSMenuItem separatorItem]];

	item = [menu addItemWithTitle:NSLocalizedString(@"Bring All to Front", nil)
						   action:@selector(arrangeInFront:)
					keyEquivalent:@""];*/
}
+(void)populateApplicationMenu:(NSMenu *)aMenu
{
	NSString *applicationName = [[NSProcessInfo processInfo] processName];
	NSMenuItem *menuItem;

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			[NSString stringWithFormat:@"%@ %@",
						NSLocalizedString(@"About", nil),
						applicationName]
		action:@selector(orderFrontStandardAboutPanel:)
		keyEquivalent:@""];
	[menuItem setTarget:NSApp];

	[aMenu addItem:[NSMenuItem separatorItem]];

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			NSLocalizedString(@"Preferences...", nil)
		action:NULL
		keyEquivalent:@","];

	[aMenu addItem:[NSMenuItem separatorItem]];

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			NSLocalizedString(@"Services", nil)
		action:NULL
		keyEquivalent:@""];
	NSMenu * servicesMenu = [[NSMenu alloc] initWithTitle:@"Services"];
	[aMenu setSubmenu:servicesMenu forItem:menuItem];
	[NSApp setServicesMenu:servicesMenu];

	[aMenu addItem:[NSMenuItem separatorItem]];

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			[NSString stringWithFormat:@"%@ %@",
									NSLocalizedString(@"Hide", nil),
									applicationName]
		action:@selector(hide:)
		keyEquivalent:@"h"];
	[menuItem setTarget:NSApp];

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			NSLocalizedString(@"Hide Others", nil)
		action:@selector(hideOtherApplications:)
		keyEquivalent:@"h"];
	[menuItem setKeyEquivalentModifierMask:NSCommandKeyMask | NSAlternateKeyMask];
	[menuItem setTarget:NSApp];

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			NSLocalizedString(@"Hide Others", nil)
		action:@selector(hideOtherApplications:)
		keyEquivalent:@"h"];
	[menuItem setKeyEquivalentModifierMask:NSCommandKeyMask | NSAlternateKeyMask];
	[menuItem setTarget:NSApp];

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			NSLocalizedString(@"Show All", nil)
		action:@selector(unhideAllApplications:)
		keyEquivalent:@""];
	[menuItem setTarget:NSApp];

	[aMenu addItem:[NSMenuItem separatorItem]];

	menuItem = (NSMenuItem*)[aMenu addItemWithTitle:
			[NSString stringWithFormat:@"%@ %@",
				NSLocalizedString(@"Quit", nil),
				applicationName]
		action:@selector(terminate:)
		keyEquivalent:@"q"];
	[menuItem setTarget:NSApp];
}
- (id)initWithContentRect:(NSRect)contentRect styleMask:(unsigned int)style
	backing:(NSBackingStoreType)bst defer:(BOOL)flag
{ /* NSWindowStyleMask wasn't available in earlier <= 10.4 SDKs */
	self = [super initWithContentRect:contentRect styleMask:style backing:bst defer:flag];
	if(self)
	{
		self->mMenuBar = NULL;
		self->mWindowsMenu = NULL;
	}
	return self;
}
-(void) dealloc
/*- (oneway void)release;*/
{
	/*NSLog(@"WSNSWindow dealloc");*/
	[self->mWindowsMenu release];
	[self->mMenuBar release];
	[super dealloc];
}/*
-(id)retain
{
	if(self->mMenuBar)
		self->mMenuBar = [self->mMenuBar retain];
	return [super retain];
}*/
-(void*)getCon
{
	return self->mCon;
}
-(void)setCon:(void*)con
{
	self->mCon = con;
}
-(void)setMenuBar:(NSMenu*)menuBar
{
	NSMenuItem* menuItem;
	NSMenuItem* appleMenuItem;
	NSMenu* appleMenu;
	[self->mMenuBar release];
	self->mMenuBar = [menuBar retain];
	/* Window menu */
	menuItem = (NSMenuItem*)[self->mMenuBar addItemWithTitle:@"Window"
						action:NULL keyEquivalent:@""];
	appleMenuItem = (NSMenuItem*)[self->mMenuBar insertItemWithTitle:@"Apple"
						 action:NULL keyEquivalent:@"" atIndex:0];
	if(menuItem)
	{
		self->mWindowsMenu = [[NSMenu alloc] initWithTitle:
						NSLocalizedString(@"Window", @"The Window menu")];
		if(self->mWindowsMenu)
		{
			[WSNSWindow populateWindowMenu:self->mWindowsMenu];
			[self->mMenuBar setSubmenu:self->mWindowsMenu forItem:menuItem];
		}
	}
	if(appleMenuItem)
	{
		appleMenu = [[NSMenu alloc] initWithTitle:@"Apple"];
		if(appleMenu)
		{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundeclared-selector"
			[NSApp performSelector:@selector(setAppleMenu:) withObject:appleMenu];
#pragma clang diagnostic pop
			[WSNSWindow populateApplicationMenu:appleMenu];
			[appleMenuItem setSubmenu:appleMenu];
		}
	}

	if([self isEqual:[NSApp keyWindow]])
	{
		[NSApp setWindowsMenu:self->mWindowsMenu];
		[NSApp setMainMenu:self->mMenuBar];
	}
}
-(void)windowDidBecomeKey:(NSNotification*)notification
{
	if(self->mMenuBar)
	{
		[NSApp setWindowsMenu:self->mWindowsMenu];
		[NSApp setMainMenu:self->mMenuBar];
	}
}

- (void)windowDidBecomeVisible:(NSNotification*)notification
{
	self->mIsVisible = true;
}

- (void)windowDidBecomeHidden:(NSNotification*)notification
{
	self->mIsVisible = false;
}

- (wsbool)getIsReallyVisible
{
	return self->mIsVisible;
}
@end
void WSWindow_Create(WSWINDOW* pThis, WSAPP* pApp, WSRECT* bounds,
	const char* title, WSMENUBAR* pMenuBar, WSWNDCLASS* pClass)
{
  NSRect frame;
  WSNSWindow* window;
  WSRectToNSRect(&frame, bounds);
  WSAdjustNSRectToCenter(&frame);
  window = [[WSNSWindow alloc] initWithContentRect:frame
					styleMask:NSTitledWindowMask
							| NSResizableWindowMask
							| NSMiniaturizableWindowMask
							//| NSMiniaturizableWindowMask
					backing:NSBackingStoreBuffered
					defer:NO];
/*[window setBackgroundColor:[NSColor whiteColor]];*/
  [window setTitle:WSCStrToNSString(title)];
	[window setDelegate:window];
  /*[window center];*/
//  [window makeKeyAndOrderFront:NSApp];
//    dispatch_async(dispatch_get_main_queue(), ^{[window setIsVisible:false];});
  //  [window setIsVisible:false];
  WSWINDOW_HANDLE(pThis) = window;
}

wsbool WSWindow_IsCreated(WSWINDOW* pThis)
{
	return *pThis != NULL;
}

void WSWindow_SetMenuBar(WSWINDOW* pThis, WSMENUBAR* pMenuBar,
	WSMENUACTIONS* ma)
{
	(void)(ma);
	[WSWINDOW_HANDLE(pThis) setMenuBar:WSMENUBAR_HANDLE(pMenuBar)];
}

wsbool WSWindow_SetIconFromFilePath(WSWINDOW* pThis,
	const char* abspath, size_t abspathlen)
{
	NSWindow* window = WSWINDOW_HANDLE(pThis);
	NSButton* documentIconButton;
	/*
		Normally on OSX the guidelines specify it should
		only have an icon when the window represents a file,
		in which case it is set automatically from
		setRepresentedFilename/setTitleWithRepresentedFilename
	*/
	NSImage* theImage;
	NSImage* resizedImage;
	NSBitmapImageRep* rep;
	NSSize resizedSize;
	resizedImage = [NSImage alloc];
	rep = [NSBitmapImageRep alloc];
	theImage = [[NSImage alloc] initWithContentsOfFile:
						  WSCStrToNSString(abspath)];
	if(!theImage || !resizedImage || !rep)
	{
		[theImage release];
		[resizedImage release];
		[rep release];
		return FALSE;
	}

	/*
		Could also possibly override and do it this way?
		- (void)setRepresentedURL:(NSURL *)url
		{
			[NSURL URLWithString:]
			[super setRepresentedURL:url];
			[[self standardWindowButton:NSWindowDocumentIconButton]
				setImage:theImage];
		}
	*/
	/*
		Hack to force it to show a custom icon
		See shouldPopUpDocumentPathMenu in WSNSApplicationDelegate
		for the other workaround part of this
	*/
	/* force NSWindowDocumentIconButton to be created */
	[window setRepresentedFilename:@"WindowTitle"];
	documentIconButton = [window standardWindowButton:
		NSWindowDocumentIconButton];

	/* resize image first */
	resizedSize = [documentIconButton bounds].size;
	rep = [rep initWithBitmapDataPlanes:NULL
			pixelsWide:resizedSize.width
			pixelsHigh:resizedSize.height
			bitsPerSample:8
			samplesPerPixel:4
			hasAlpha:TRUE
			isPlanar:FALSE
			colorSpaceName:NSCalibratedRGBColorSpace
			bytesPerRow:0
			bitsPerPixel:0];
	resizedImage = [resizedImage initWithSize:resizedSize];
	if(!rep || !resizedImage)
	{ /* This will also fail on pre-10.2 as button will be NULL */
		[rep release];
		[resizedImage release];
		[theImage release];
		return FALSE;
	}

	/* all allocations and init done, continue resizing the image */
	/*rep.size = resizedSize;*/
	/*
		10.4+ and retina-compatible; untested but should work
		https://stackoverflow.com/questions/11949250/how-to-resize-nsimage/38442746#38442746
	*/
	if([NSGraphicsContext respondsToSelector:
		@selector(graphicsContextWithBitmapImageRep:)])
	{
		[NSGraphicsContext saveGraphicsState];
		[NSGraphicsContext setCurrentContext:
			/*[NSGraphicsContext graphicsContextWithBitmapImageRep:rep]*/
			[NSGraphicsContext performSelector:
				@selector(graphicsContextWithBitmapImageRep:)
				withObject:rep]
		];
		[theImage drawInRect:
				NSMakeRect(0, 0, resizedSize.width, resizedSize.height)
			fromRect:NSZeroRect operation:NSCompositeCopy fraction:1.0];
		[NSGraphicsContext restoreGraphicsState];
		[resizedImage addRepresentation:rep];
	}
	else
	{
		/* Pre-10.4*/
		[resizedImage lockFocus];
		[theImage setSize:resizedSize];
		[[NSGraphicsContext currentContext]
			setImageInterpolation:NSImageInterpolationHigh];
		[theImage drawAtPoint:NSZeroPoint
			fromRect:NSMakeRect(0, 0, resizedSize.width, resizedSize.height)
			operation:NSCompositeCopy fraction:1.0];
		[resizedImage unlockFocus];
	}
	/* Now set the image finally and clean up */
	[documentIconButton setImage:resizedImage];

	[rep release];
	[resizedImage release];
	[theImage release];
	return TRUE;
}

	wsbool WSWindow_IsVisible(WSWINDOW* pThis)
	{
		return [WSWINDOW_HANDLE(pThis) isVisible];
	}


void WSWindow_Hide(WSWINDOW* pThis)
{
	[WSWINDOW_HANDLE(pThis) orderOut:NULL];
}

void WSWindow_Show(WSWINDOW* pThis)
{
  [WSWINDOW_HANDLE(pThis) orderFront:NULL];
}

void WSWindow_Destroy(WSWINDOW* pThis)
{
  [WSWINDOW_HANDLE(pThis) release];
}
void WSScrolledWindow_Destroy(WSSCROLLEDWINDOW* pThis)
{
  [WSSCROLLEDWINDOW_HANDLE(pThis) release];
}

void* WSWindow_GetCon(WSWINDOW* pThis)
{
	return [WSWINDOW_HANDLE(pThis) getCon];
}
void WSWindow_SetCon(WSWINDOW* pThis, void* con)
{
	[WSWINDOW_HANDLE(pThis) setCon:con];
}

void* WSWindow_GetHandle(WSWINDOW* pThis)
{
    return WSWINDOW_HANDLE(pThis);
}
WSEVENT_CALLBACK(WSWindowDefaultCallback)
{
	return WSEVENT_UNHANDLED;
}


/*========================= WSNSTextView =================*/
@interface WSNSTextView : NSTextView
{
	NSUndoManager* undoManager;
}
- (void)textDidChange:(NSNotification*)aNotification;
//- (void)changeColor:(id)sender;
@end
@implementation WSNSTextView

+ (void)initialize
{
}

- (id)initWithFrame:(NSRect)frameRect
{
	if ([super initWithFrame:frameRect])
	{
		self->undoManager = [[NSUndoManager alloc] init];
		if(self->undoManager)
			return self;
		[self release];
	}
	return NULL;
}

-(void) dealloc
{
	[self->undoManager release];
	[super dealloc];
}

- (void)textDidChange:(NSNotification*)aNotification
{
  (void)(aNotification);
}

- (NSUndoManager*)undoManagerForTextView:(NSTextView*)view
{
	return self->undoManager;
}

//- (void)changeColor:(id)sender
//{
 // /* Stop color change messages sent from shared views */
//	(void)(sender);
//}


- (void) setEnabled:(BOOL) flag
{
	/* Apple QA1461 */
	if (flag)
		[self setTextColor: [NSColor controlTextColor]];
	else
		[self setTextColor: [NSColor disabledControlTextColor]];

	[self setSelectable: flag];
	[self setEditable: flag];
}

- (BOOL) isEnabled
{
	return [self isEditable];
}

- (void)textDidEndEditing:(NSNotification*)aNotification
{
  (void)(aNotification);
  /* TODO: Don't process if being edited */
}

-(BOOL)textView:(NSTextView*)aTextView
	clickedOnLink:(id)link atIndex:(unsigned)charIndex
{
  [super clickedOnLink:link atIndex:charIndex];
  return TRUE;
}

- (void)copy:(id)sender
{
  [super copy:sender];
}

- (void)cut:(id)sender
{
  [super cut:sender];
}

- (void)paste:(id)sender
{
  [super paste:sender];
}
@end


/*========================= WSTEXTCTRL =================*/
#undef WSTextCtrl_CreateScrolled
wsbool WSTextCtrl_CreateScrolled(WSTEXTCTRL* pThis, WSSCROLLEDWINDOW* pScroll,
	WSWINDOW* pParent, WSRECT* rect)
{
  NSRect frame;
  WSNSTextView* textView;

  WSRectToNSRect(&frame, rect);

  WSSCROLLEDWINDOW_HANDLE(pScroll) =
	[[NSScrollView alloc] init];
  if(!WSSCROLLEDWINDOW_HANDLE(pScroll))
	return FALSE;
  [WSWINDOW_HANDLE(pParent) setContentView:WSSCROLLEDWINDOW_HANDLE(pScroll)];

  WSNSRectChildToParent(
	  WSSCROLLEDWINDOW_HANDLE(pScroll), &frame);
  textView = [[WSNSTextView alloc] initWithFrame:frame];
  if(!textView)
  {
	[WSSCROLLEDWINDOW_HANDLE(pScroll) release];
	return FALSE;
  }

	[WSSCROLLEDWINDOW_HANDLE(pScroll) setHasVerticalScroller:TRUE];
	[WSSCROLLEDWINDOW_HANDLE(pScroll) setHasHorizontalScroller:TRUE];
	[WSSCROLLEDWINDOW_HANDLE(pScroll) setAutoresizingMask:
		NSViewWidthSizable | NSViewHeightSizable];
//	[[textView enclosingScrollView] setHasHorizontalScroller:TRUE];
  [textView setVerticallyResizable:TRUE];
  [textView setHorizontallyResizable:TRUE];
  //[textView setMinSize:NSMakeSize(0.0, frame.size.height)];
  [textView setMaxSize:NSMakeSize(FLT_MAX, FLT_MAX)];
  [textView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
  [[textView textContainer] setContainerSize:NSMakeSize(FLT_MAX, FLT_MAX)];
  [[textView textContainer] setWidthTracksTextView:FALSE];
  [textView setDelegate:(id)textView];
  [textView setAllowsUndo:YES];
  [textView setRichText:NO];
  [textView setUsesFontPanel:NO];
  [textView setEditable:YES];
  [textView setSelectable:YES];
  [WSSCROLLEDWINDOW_HANDLE(pScroll) setDocumentView:textView];
  WSTEXTCTRL_HANDLE(pThis) = textView;
	return TRUE;
}

void WSTextCtrl_Activate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_Deactivate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_Update(WSTEXTCTRL* pThis, WSWINDOW* pParent, WSRECT* rect)
{
}

void WSTextCtrl_Append(WSTEXTCTRL* pThis,
					  const char* text, size_t textLen)
{
	NSString* theString = WSCStrToNSString(text);
	NSMutableAttributedString* theAttributedString =
		[[NSMutableAttributedString alloc] initWithString:theString];
	if(!theAttributedString)
		return;
	[theAttributedString addAttribute:NSForegroundColorAttributeName
		value:[NSColor controlTextColor]
								range:NSMakeRange(0, textLen)];
	[[WSTEXTCTRL_HANDLE(pThis) textStorage] appendAttributedString:
		theAttributedString];
	[theAttributedString release];
}

void WSTextCtrl_SetText(WSTEXTCTRL* pThis, const char* text, size_t textLen)
{
	NSString* theString = WSCStrToNSString(text);
	NSMutableAttributedString* theAttributedString =
		[[NSMutableAttributedString alloc] initWithString:theString];
	if(!theAttributedString)
		return;
	[theAttributedString addAttribute:NSForegroundColorAttributeName
		value:[NSColor controlTextColor]
								range:NSMakeRange(0, textLen)];
	[[WSTEXTCTRL_HANDLE(pThis) textStorage] setAttributedString:
		theAttributedString];
	[theAttributedString release];
}

void WSTextCtrl_SetSize(WSTEXTCTRL* pThis, guipos_t width, guipos_t height)
{
	[WSTEXTCTRL_HANDLE(pThis) setFrameSize:NSMakeSize(width, height)];
}

void WSTextCtrl_Destroy(WSTEXTCTRL* pThis)
{
  [WSTEXTCTRL_HANDLE(pThis) release];
}

void WSTextCtrl_Idle(WSTEXTCTRL* pThis)
{
}


void WSTextCtrl_HandlePaint(WSTEXTCTRL* pThis, WSPAINTEVENT* ev, WSPAINTDC* dc)
{
}
void WSTextCtrl_HandleKeyDown(WSTEXTCTRL* pThis, WSKEYEVENT* ev)
{
}
void WSTextCtrl_HandleContentEvent(WSTEXTCTRL* pThis,
	WSCONTENTEVENT* ev)
{
}

void WSTextCtrl_HandleGrowEvent(WSTEXTCTRL* pThis,
	WSGROWEVENT* ev)
{
}

void WSTextCtrl_HandleZoomEvent(WSTEXTCTRL* pThis,
	WSZOOMEVENT* ev)
{
}

void WSTextCtrl_HandleKeyEvent(WSTEXTCTRL* pThis, WSKEYEVENT* ev)
{
}

void WSTextCtrl_HandleIdle(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_HandleActivate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_HandleDeactivate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_HandleIBeamCursor(WSTEXTCTRL* pThis,
	WSIBEAMEVENT* er)
{
}

void WSTextCtrl_HandleMouseWheel(WSTEXTCTRL* pThis,
	WSMOUSEWHEELEVENT* er)
{
}

void* WSTextCtrl_GetHandle(WSTEXTCTRL* pThis)
{
	return WSTEXTCTRL_HANDLE(pThis);
}
/*========================= WSDC =================*/

void WSDC_Create(WSDC* pThis, WSWINDOW* pWindow)
{
}
void WSDC_Destroy(WSDC* pThis, WSWINDOW* pWindow)
{
}
wsbool WSDC_IsWindowReady(WSDC* pThis, WSWINDOW* pWindow)
{
	return FALSE;
}

/*========================= WSPAINTDC =================*/
void WSPaintDC_Create(WSPAINTDC* pThis, WSWINDOW* pWindow)
{
	*(WSPAINTDC*)pThis = *(WSPAINTDC*)pWindow;
}
void WSPaintDC_Destroy(WSPAINTDC* pThis, WSWINDOW* pWindow)
{
}

/*========================= WSNSSubMenuItem =================*/

@interface WSNSSubMenuItem : NSMenuItem
{
	LPWSMenuCallback mProc;
	void* mUserData;
}
- (void)setProc:(LPWSMenuCallback)proc;
- (void)setUserData:(void*)userData;
@end
@implementation WSNSSubMenuItem
- (void)menuAction: (id)sender
{
  (*self->mProc)(mUserData);
}
- (void)setProc:(LPWSMenuCallback)proc
{
	self->mProc = proc;
}
- (void)setUserData:(void*)userData
{
	self->mUserData = userData;
}
@end
/*========================= WSMENUACTIONS =================*/
void WSMenuActions_Create(WSMENUACTIONS* pThis)
{
	(void)(pThis);
}
void WSMenuActions_Destroy(WSMENUACTIONS* pThis)
{
	(void)(pThis);
}
/*========================= WSMENU =================*/
#undef WSMenu_Create
void WSMenu_Create(WSMENU* pThis, WSMENUSECTION* pSection)
{
  NSMenu* theMenu = [[NSMenu alloc] init];
  WSMENU_HANDLE(pThis) = theMenu;
  WSMENUSECTION_HANDLE(pSection) = theMenu;
//  WSMENU_HANDLE(pThis) = [NSMenuItem new];
//  if(*pThis)
//    [WSMENU_HANDLE(pThis) setSubmenu:theMenu];
//  [theMenu release];
}

void WSMenu_Destroy(WSMENU* pThis)
{
	[WSMENU_HANDLE(pThis) release];
}

wsbool WSMenuSection_AppendWithAccel(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*mmes), WSWINDOW* pWindow,
	const char* accel, WSMENUACTIONS* ma)
{
  WSNSSubMenuItem* theItem = [WSNSSubMenuItem alloc];
  if(!theItem)
	return FALSE;
  size_t titlelen = strlen(title);
  unichar shortcutChar;
  if(accel)
	  shortcutChar = accel[0];
  else
  {
	  /* "TITLE/X" where X is the shortcut key */
	  if(titlelen > 2 &&
		*(title + titlelen - 1) == '/')
	  {
		shortcutChar = *(title + titlelen - 1);
		titlelen -= 2;
	  }
	  else
		shortcutChar = 0;
  }
  theItem = [theItem initWithTitle: WSCStrToNSString(title)
	action:@selector(menuAction:)
	keyEquivalent:[NSString stringWithCharacters:&shortcutChar length:1]];
  if(!theItem)
	return FALSE;
  [theItem setKeyEquivalentModifierMask:NSCommandKeyMask];
  [theItem setProc:WSMenuMessages(commandid)];
  [theItem setUserData:pWindow];
  [theItem setTarget:theItem];
  [WSMENUSECTION_HANDLE(pThis) addItem:theItem];
  [theItem release];
  return TRUE;
}

wsbool WSMenuSection_Append(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*mmes), WSWINDOW* window,
	WSMENUACTIONS* ma)
{
	return WSMenuSection_AppendWithAccel(pThis, title,
		commandid, mmes, window, NULL, ma);
}

void WSMenu_AppendSeparator(WSMENU* pThis)
{
	[WSMENU_HANDLE(pThis) addItem:[NSMenuItem separatorItem]];
}

void WSMenu_EndSection(WSMENU* pThis, WSMENUSECTION* pSection)
{
	WSMenu_AppendSeparator(pThis);
}

void WSMenu_Finalize(WSMENU* pThis, WSMENUSECTION* pSection)
{
}
void WSMenu_SetItemTitle(WSMENU* pThis, size_t index, const char* title)
{
	 [[WSMENU_HANDLE(pThis) itemAtIndex:index] setTitle:
		WSCStrToNSString(title)];
}

WSMENUEVENT_CALLBACK(WSMenuSectionDefaultCallback)
{
	return WSMENUEVENT_UNHANDLED;
}

/*=====================================WSMENUBAR==================*/
void WSMenuBar_Create(WSMENUBAR* pThis)
{
	WSMENUBAR_HANDLE(pThis) = [[NSMenu alloc] init];
}

void WSMenuBar_Append(WSMENUBAR* pThis, WSMENU* pMenu, const char* title)
{
  NSMenuItem* item;// = [NSMenuItem alloc];
//  if(!item)
//	return;
  item = (NSMenuItem*)[WSMENUBAR_HANDLE(pThis) addItemWithTitle:
				WSCStrToNSString(title)
			action:NULL keyEquivalent:@""];
  if(!item)
	return;
  [WSMENU_HANDLE(pMenu) setTitle:WSCStrToNSString(title)];
  [WSMENUBAR_HANDLE(pThis) setSubmenu:WSMENU_HANDLE(pMenu) forItem:item];
}

void WSMenuBar_Draw(WSMENUBAR* pThis)
{
}

void WSMenuBar_Destroy(WSMENUBAR* pThis)
{
  [WSMENUBAR_HANDLE(pThis) release];
}
/*==============================================================*/



/*==============================================================*/
/*==============================================================*/
/*==============================================================*/
/*==============================================================*/

@interface WSNSApplicationDelegate : NSObject
{
	NSMenu* dockMenu;
	NSAutoreleasePool* pool;
}
-(void)setDockMenu:(NSMenu*)menu;
@end
@implementation WSNSApplicationDelegate
- (id)init
{
	self = [super init];
	if(self)
	{
		self->pool = [[NSAutoreleasePool alloc] init];
		if(self->pool)
			return self;
		[self release];
	}
	return NULL;
}
- (void)dealloc
{
	[self->pool release];
  [super dealloc];
}

- (NSMenu*)applicationDockMenu:(NSApplication*)sender
{
	return self->dockMenu;
}
- (BOOL)window:(NSWindow*)window shouldPopUpDocumentPathMenu:(NSMenu*)menu
{
	/*[[window standardWindowButton:NSWindowDocumentIconButton] image]*/
	return FALSE;
}
- (void)setDockMenu:(NSMenu*)menu
{
	self->dockMenu = menu;
}

- (void)fullscreen:(NSNotification *)notification
{
	typedef OSStatus (*LPSetSystemUIMode)(int mode, int ops);
	typedef void (*LPGetSystemUIMode)(int* mode, int* ops);

	LPSetSystemUIMode lpSetSystemUIMode;
	LPGetSystemUIMode lpGetSystemUIMode;
	int prevUIMode;
	NSWindow* window = [NSApp keyWindow];


	if([window respondsToSelector:@selector(toggleFullScreen:)])
	{ /* 10.7+ */
		[window performSelector:@selector(toggleFullScreen:) withObject:NULL];
		return;
	}

#if 0 /* The proper way is to call setCollectionBehavior on window creation
			with NSWindowCollectionBehaviorFullScreenPrimary/128,
		 then check that here and only fullscreen if that is so. */
	if([window respondsToSelector:@selector(toggleFullScreen:)])
	{
		NSUInteger collectionMode =
			(NSUInteger)[[window valueForKey:@"collectionBehavior"] intValue];
		/* toggleFullScreen is 10.7+; 128 ==
				NSWindowCollectionBehaviorFullScreenPrimary
			256 == NSWindowCollectionBehaviorFullScreenAuxiliary */
		if(collectionMode & 128)
		{ /* 16384 == NSFullScreenWindowMask */
			if(!([window styleMask] & 16384))
				[window performSelector:@selector(toggleFullScreen:) withObject:NULL];
			return;
		}
	}
#endif

	lpSetSystemUIMode =
		(LPSetSystemUIMode) NSAddressOfSymbol(
			NSLookupAndBindSymbol("_SetSystemUIMode"));
	lpGetSystemUIMode =
		(LPGetSystemUIMode) NSAddressOfSymbol(
			NSLookupAndBindSymbol("_GetSystemUIMode"));

	(*lpGetSystemUIMode)(&prevUIMode, NULL);

	if(prevUIMode == 0/*kUIModeNormal*/)
	{
		/*
			"Animating a window to fullscreen on the Mac"
			August 14, 2009 by Matt Gallagher
			https://www.cocoawithlove.com/2009/08/animating-window-to-fullscreen-on-mac.html
		*/
		NSRect tempframe = NSMakeRect(0, 0, 100, 100);
		NSRect screenframe = [[window screen] frame];
		NSRect contentframe;
		int uimode;
		/* TODO: Save level/frame/styleMask */

		/* setStyleMask is 10.6+ */
		if([window respondsToSelector:@selector(setStyleMask:)])
			[window performSelector:@selector(setStyleMask:)
				withObject:(NSObject*)([window styleMask] & ~NSResizableWindowMask)];
		contentframe = [NSWindow contentRectForFrameRect:tempframe
			styleMask:[window styleMask]];
		screenframe.origin.y += (tempframe.origin.y - contentframe.origin.y);
		screenframe.size.height +=
			(tempframe.size.height - contentframe.size.height);
		[window setFrame:screenframe display:TRUE];

		/* only hide the menu bar if the window is on the monitor with the menu bar */
		if([[window screen] isEqual:[[NSScreen screens] objectAtIndex:0]])
			uimode = 1/*kUIOptionAutoShowMenuBar*/;
		else
			uimode = 0;
		/* Cocoa has [NSMenu setMenuBarVisible:FALSE] which also hides the dock
			but doesn't re-show it when the mouse is over it */
		(*lpSetSystemUIMode)(3/*kUIModeAllHidden*/, uimode
							/*4*//*kUIOptionDisableAppleMenu*/
							);
	}
	else
	{
		/* TODO: Restore frame/styleMask */
		(*lpSetSystemUIMode)(0/*kUIModeNormal*/, 0);
	}
}

#if 0
+(void) populateDebugMenu:(NSMenu *)menu
{
}

+(void) populateEditMenu:(NSMenu *)menu
{
	NSMenuItem * item;

	item = [menu addItemWithTitle:NSLocalizedString(@"Undo", nil)
						   action:@selector(undo:)
					keyEquivalent:@"z"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Redo", nil)
						   action:@selector(redo:)
					keyEquivalent:@"Z"];

	[menu addItem:[NSMenuItem separatorItem]];

	item = [menu addItemWithTitle:NSLocalizedString(@"Cut", nil)
						   action:@selector(cut:)
					keyEquivalent:@"x"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Copy", nil)
						   action:@selector(copy:)
					keyEquivalent:@"c"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Paste", nil)
						   action:@selector(paste:)
					keyEquivalent:@"v"];

	item = [menu addItemWithTitle:
					NSLocalizedString(@"Paste and Match Style", nil)
				action:@selector(pasteAsPlainText:)
				keyEquivalent:@"V"];
	[item setKeyEquivalentModifierMask:NSCommandKeyMask | NSAlternateKeyMask];

	item = [menu addItemWithTitle:NSLocalizedString(@"Delete", nil)
						   action:@selector(delete:)
					keyEquivalent:@""];

	item = [menu addItemWithTitle:NSLocalizedString(@"Select All", nil)
						   action:@selector(selectAll:)
					keyEquivalent:@"a"];

	[menu addItem:[NSMenuItem separatorItem]];

	item = [menu addItemWithTitle:NSLocalizedString(@"Find", nil)
						   action:NULL
					keyEquivalent:@""];
	NSMenu * findMenu = [[[NSMenu alloc] initWithTitle:@"Find"] autorelease];
	[self populateFindMenu:findMenu];
	[menu setSubmenu:findMenu forItem:item];

	item = [menu addItemWithTitle:NSLocalizedString(@"Spelling", nil)
						   action:NULL
					keyEquivalent:@""];
	NSMenu * spellingMenu =
		[[[NSMenu alloc] initWithTitle:@"Spelling"] autorelease];
	[self populateSpellingMenu:spellingMenu];
	[menu setSubmenu:spellingMenu forItem:item];
}

+(void) populateFileMenu:(NSMenu *)menu
{
	NSMenuItem * item;

	item = [menu addItemWithTitle:NSLocalizedString(@"New", nil)
						   action:NULL
					keyEquivalent:@"n"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Open...", nil)
						   action:NULL
					keyEquivalent:@"o"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Open Recent", nil)
						   action:NULL
					keyEquivalent:@""];
	NSMenu * openRecentMenu =
		[[[NSMenu alloc] initWithTitle:@"Open Recent"] autorelease];
	[openRecentMenu performSelector:@selector(_setMenuName:)
					withObject:@"NSRecentDocumentsMenu"];
	[menu setSubmenu:openRecentMenu forItem:item];

	item = [openRecentMenu addItemWithTitle:NSLocalizedString(@"Clear Menu", nil)
									 action:@selector(clearRecentDocuments:)
							  keyEquivalent:@""];

	[menu addItem:[NSMenuItem separatorItem]];

	item = [menu addItemWithTitle:NSLocalizedString(@"Close", nil)
						   action:@selector(performClose:)
					keyEquivalent:@"w"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Save", nil)
						   action:NULL
					keyEquivalent:@"s"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Save As...", nil)
						   action:NULL
					keyEquivalent:@"S"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Revert", nil)
						   action:NULL
					keyEquivalent:@""];

	[menu addItem:[NSMenuItem separatorItem]];

	item = [menu addItemWithTitle:NSLocalizedString(@"Page Setup...", nil)
						   action:@selector(runPageLayout:)
					keyEquivalent:@"P"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Print...", nil)
						   action:@selector(print:)
					keyEquivalent:@"p"];
}

+(void) populateFindMenu:(NSMenu *)menu
{
	NSMenuItem * item;

	item = [menu addItemWithTitle:NSLocalizedString(@"Find...", nil)
						   action:@selector(performFindPanelAction:)
					keyEquivalent:@"f"];
	[item setTag:NSFindPanelActionShowFindPanel];

	item = [menu addItemWithTitle:NSLocalizedString(@"Find Next", nil)
						   action:@selector(performFindPanelAction:)
					keyEquivalent:@"g"];
	[item setTag:NSFindPanelActionNext];

	item = [menu addItemWithTitle:NSLocalizedString(@"Find Previous", nil)
						   action:@selector(performFindPanelAction:)
					keyEquivalent:@"G"];
	[item setTag:NSFindPanelActionPrevious];

	item = [menu addItemWithTitle:
						NSLocalizedString(@"Use Selection for Find", nil)
				action:@selector(performFindPanelAction:)
				keyEquivalent:@"e"];
	[item setTag:NSFindPanelActionSetFindString];

	item = [menu addItemWithTitle:NSLocalizedString(@"Jump to Selection", nil)
						   action:@selector(centerSelectionInVisibleArea:)
					keyEquivalent:@"j"];
}

+(void) populateHelpMenu:(NSMenu *)menu
{
	NSMenuItem * item;

	item = [menu addItemWithTitle:[NSString stringWithFormat:@"%@ %@",
										[[NSApp delegate] applicationName],
										NSLocalizedString(@"Help", nil)]
						   action:@selector(showHelp:)
					keyEquivalent:@"?"];
	[item setTarget:NSApp];
}

+(void) populateSpellingMenu:(NSMenu *)menu
{
	NSMenuItem * item;

	item = [menu addItemWithTitle:NSLocalizedString(@"Spelling...", nil)
					action:@selector(showGuessPanel:)
					keyEquivalent:@":"];

	item = [menu addItemWithTitle:NSLocalizedString(@"Check Spelling", nil)
				action:@selector(checkSpelling:)
				keyEquivalent:@";"];

	item = [menu addItemWithTitle:
					NSLocalizedString(@"Check Spelling as You Type", nil)
				action:@selector(toggleContinuousSpellChecking:)
				keyEquivalent:@""];
}
+(void) populateViewMenu:(NSMenu *)menu
{
}
#endif

- (void)applicationWillFinishLaunching: (NSNotification*)notification
{
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
	/*[WSNSApplicationDelegate populateMainMenu];*/
}
#if 0
- (NSApplicationTerminateReply)applicationShouldTerminate:
	(NSApplication*)sender
{
}
#endif
@end
/*==============================================================*/
/*==============================================================*/
/*==============================================================*/
/*==============================================================*/
/*==============================================================*/
/*NS [un]hideOtherApplications@"" */
wsbool WSApp_CreateGUI(WSAPP* pThis)
{
	/*
		See the following for making a nibless app:
		http://lapcatsoftware.com/blog/2007/05/16/working-without-a-nib-part-1/
		https://github.com/ccxvii/snippets/blob/master/nibless.m
		https://github.com/hammackj/niblesscocoa
	*/
	ProcessSerialNumber psn = { 0, kCurrentProcess };
	NSApplication* theApp;

	/* Allocate the delegate */
	WSAPP_HANDLE(pThis) = [WSNSApplicationDelegate alloc];
	if(!WSAPP_HANDLE(pThis))
		return FALSE;
	WSAPP_HANDLE(pThis) = [WSAPP_HANDLE(pThis) init];
	if(!WSAPP_HANDLE(pThis))
		return FALSE;

	/* Create the shared NSApplication if it hasn't been created yet */
	theApp = [NSApplication sharedApplication];
	if(!theApp)
	{
		[WSAPP_HANDLE(pThis) release];
		return FALSE;
	}

	/* Allows for the OS to get the app icon without a bundle */
	TransformProcessType(&psn,
		kProcessTransformToForegroundApplication);
	SetFrontProcess(&psn);
	[theApp setDelegate:WSAPP_HANDLE(pThis)];

	/* Calls applicationDidFinishLaunching of the delegate,
	allows drawing the menu bar (otherwise menus modifications
	won't work outside of the dock). */
	[theApp finishLaunching];
	return TRUE;
}

void WSApp_SetNotifyIconMenu(WSAPP* pThis, WSMENU* pMenu)
{
	NSMenu* theMenu = pMenu ? WSMENU_HANDLE(pMenu) : NULL;
	[WSAPP_HANDLE(pThis) setDockMenu:theMenu];
}

void WSApp_SetNotifyIcon(WSAPP* pThis,
						  const char* abspath, size_t abspathlen)
{
	NSImage* theImage = [[NSImage alloc] initWithContentsOfFile:
						  WSCStrToNSString(abspath)];
	if(!theImage)
		return;

	[[NSApplication sharedApplication] setApplicationIconImage:theImage];
}

void WSApp_SetMenuBar(WSAPP* pThis, WSMENUBAR* pMenuBar, WSMENUACTIONS* ma)
{
	(void)(pThis);
	(void)(pMenuBar);
	(void)(ma);
}

void WSApp_Destroy(WSAPP* pThis)
{
	[[NSApplication sharedApplication] setDelegate:NULL];
	[WSAPP_HANDLE(pThis) release];
}


wsbool WSLaunchURL(const char* urlstring, size_t urllen)
{
	wsbool success;
	NSURL* theUrl = [NSURL URLWithString:
						WSCStrToNSString(urlstring)];
	if(!theUrl)
		return FALSE;
	success = [[NSWorkspace sharedWorkspace] openURL:theUrl];
	return success;
}

wsbool WSNotifyIcon_Create(WSNOTIFYICON* pThis, WSWINDOW* pWindow,
	const char* tip, size_t tiplen, WSICON* pIcon, const char* iconpath,
	uint16_t command)
{
	return FALSE;
}
void WSNotifyIcon_SetTooltip(WSNOTIFYICON* pThis, const char* title)
{
}
void WSNotifyIcon_SetIcon(WSNOTIFYICON* pThis, const char* path)
{
}
void WSNotifyIcon_SetMenu(WSNOTIFYICON* pThis, WSMENU* pMenu)
{
}
void WSNotifyIcon_Destroy(WSNOTIFYICON* pThis)
{
}

#ifdef __cplusplus
}
#endif

//=============================================================================
// WSPathGetXXX()
//
// One of the very few times where a raw OS version check is necessary.
// There is no way to tell which values are valid for
// NSSearchPathForDirectoriesInDomains as they are raw integer values. If a
// integer value is passed in that isn't supported the behaviour is strange
// (NSApplicationSupportDirectory on 10.3 == @"//network")
// or it just crashes the app.
//=============================================================================
#ifdef __cplusplus
extern "C" {
#endif
#ifndef NSAppKitVersionNumber10_2
#define NSAppKitVersionNumber10_2 663
#endif
#ifndef NSAppKitVersionNumber10_4
#define NSAppKitVersionNumber10_4 824
#endif
#ifndef NSAppKitVersionNumber10_6
#define NSAppKitVersionNumber10_6 1038
#endif

wsbool WSPathFormulate(char** outpath, NSSearchPathDirectory type,
	NSSearchPathDomainMask domainMask, wsbool appendPreferences)
{
	NSString* path;
	NSArray* paths;
	paths = NSSearchPathForDirectoriesInDomains(type, domainMask, YES);
	if(paths == NULL || [paths count] == 0)
		return FALSE;
	path = [paths objectAtIndex:0];

	if(appendPreferences)
	{
		NSString* finalpath;
		finalpath = [path stringByAppendingString:@"/Preferences"];
		*outpath = StringDup([finalpath UTF8String]);
	}
	else
		*outpath = StringDup([path UTF8String]);
	return TRUE;
}

void WSPathGetHome(char** path)
{
	*path = StringDup([NSHomeDirectory() UTF8String]);
}
void WSPathGetConfig(char** path)
{
	WSPathFormulate(path, NSLibraryDirectory, NSLocalDomainMask, TRUE);
}
void WSPathGetExe(char** path)
{
	*path = StringDup([[[NSBundle mainBundle] executablePath] UTF8String]);
}
void WSPathGetUserConfig(char** path)
{
	WSPathFormulate(path, NSLibraryDirectory, NSUserDomainMask, TRUE);
}
void WSPathGetData(char** path)
{
	*path = StringDup([[[NSBundle mainBundle] sharedSupportPath] UTF8String]);
}
void WSPathGetLocalData(char** path)
{
	if(NSAppKitVersionNumber < NSAppKitVersionNumber10_4
		|| !WSPathFormulate(path, 14/*NSApplicationSupportDirectory*/,
						NSLocalDomainMask, FALSE)) /* 10.4+ */
		*path = StringDup("/Library/Application Support");
}
void WSPathGetUserData(char** path)
{
	if(NSAppKitVersionNumber < NSAppKitVersionNumber10_4
		|| !WSPathFormulate(path, 14/*NSApplicationSupportDirectory*/,
						NSUserDomainMask, FALSE)) /* 10.4+ */
		*path = StringDup("/Library/Application Support");
}
void WSPathGetUserLocalData(char** path)
{
	WSPathGetUserData(path);
}
void WSPathGetPlugins(char** path)
{
	*path = StringDup([[[NSBundle mainBundle] builtInPlugInsPath] UTF8String]);
}
void WSPathGetResources(char** path)
{
	*path = StringDup([[[NSBundle mainBundle] resourcePath] UTF8String]);
}
void WSPathGetLocalizedResources(char** path)
{
//	GetResources/LANGUAGE.lproj
}
void WSPathGetUserCache(char** path)
{
	if(NSAppKitVersionNumber < NSAppKitVersionNumber10_4
		|| !WSPathFormulate(path, 13/*NSCachesDirectory*/,
						NSUserDomainMask, FALSE)) /* 10.4+ */
		*path = StringDup("/Library/Caches");
}
void WSPathGetUserDesktop(char** path)
{
	if(NSAppKitVersionNumber < NSAppKitVersionNumber10_4
		|| !WSPathFormulate(path, 12/*NSDesktopDirectory*/,
						NSUserDomainMask, FALSE)) /* 10.4+ */
		*path = StringDup("/Library/Caches");
}
void WSPathGetUserDownloads(char** path)
{
	if(NSAppKitVersionNumber < NSAppKitVersionNumber10_4
		|| !WSPathFormulate(path, 15/*NSDownloadsDirectory*/,
						NSUserDomainMask, FALSE)) /* 10.4+ */
		*path = StringDup("/Library/Caches");
}
void WSPathGetUserMusic(char** path)
{
	if(NSAppKitVersionNumber < NSAppKitVersionNumber10_4
		|| !WSPathFormulate(path, 18/*NSMusicDirectory*/,
						NSUserDomainMask, FALSE)) /* 10.4+ */
		*path = StringDup("/Library/Caches");
}
void WSPathGetUserPictures(char** path)
{
	if(NSAppKitVersionNumber < NSAppKitVersionNumber10_6
		|| !WSPathFormulate(path, 19/*NSPicturesDirectory*/,
						NSUserDomainMask, FALSE)) /* 10.6+ */
		*path = StringDup("/Library/Caches");
}
void WSPathGetUserVideos(char** path)
{
	if(NSAppKitVersionNumber < NSAppKitVersionNumber10_4
		|| !WSPathFormulate(path, 17/*NSMoviesDirectory*/,
						NSUserDomainMask, FALSE)) /* 10.4+ */
		*path = StringDup("/Library/Caches");
}
void WSPathGetUserDocuments(char** path)
{
	if(NSAppKitVersionNumber < NSAppKitVersionNumber10_2
		|| !WSPathFormulate(path, NSDocumentDirectory,
						NSUserDomainMask, FALSE)) /* 10.2+ */
		*path = StringDup("/Library/Caches");
}

/*========================WSDISPLAY==========================================*/
void WSDisplayGetDPI(guipos_t* x, guipos_t* y)
{
#if defined(__MACH__)
	/* CGMainDisplayID() is 10.2+ so just use kCGDirectMainDisplay */
	CGDirectDisplayID mainDisplay = kCGDirectMainDisplay;

	// Get the physical pixel dimensions of the display
	size_t pixelsWide = CGDisplayPixelsWide(mainDisplay);
	size_t pixelsHigh = CGDisplayPixelsHigh(mainDisplay);

	// Get the display size in points
	// Note that CGFloat did not exist in early headers
	CGRect mainScreenBounds = CGDisplayBounds(mainDisplay);
	wsfloat pointsWide = CGRectGetWidth(mainScreenBounds);
	wsfloat pointsHigh = CGRectGetHeight(mainScreenBounds);

	// Calculate the DPI for each axis (72 points per inch)
	*x = (guipos_t)((pixelsWide / pointsWide) * 72);
	*y = (guipos_t)((pixelsHigh / pointsHigh) * 72);
#else
	*x = *y = 72; // standard on classic mac and up to 10.3
#endif
}

void WSDisplayGetScaleFactor(guipos_t* x, guipos_t* y)
{
	WSDisplayGetDPI(x, y);
	*x /= 72;
	*y /= 72;
}
#if 0
	int WSApp_BasicMain_(WSAPP* app, WSEVENTLOOP* ev)
	{
		if(WSApp_OnInitGUI((app)))
		{
			WSEventLoop_Create(ev);
			do
			{
				if(!WSApp_OnEventLoopBegin(app))
					break;
				if(WSApp_HasGUI(app))
					do
					{
						while(WSEventLoop_RunSingle(ev)){}
					}
					while(!WSEventLoop_IsQuitMessage(ev)
						  && WSApp_OnIdle(app));
				else
					while(WSApp_OnIdle(app)){}
			} while(WSApp_OnEventLoopEnd(app));
			WSEventLoop_Destroy(ev);
		}
		return WSApp_OnExitGUI(app);
	}
#endif
/*-----------------------------------------------------------------------------*/
/*	main()*/
/*-----------------------------------------------------------------------------*/
#if !defined(__MACH__) || (!defined(TARGET_OS_IOS) || !TARGET_OS_IOS)
 int main(int argc, char* argv[])
{ return WSApp_BasicMain(argc, argv); }
#endif
#ifdef __cplusplus
}
#endif
#endif /* !IOS */
/*=============================================================================
	 ##### #         #####       # ##### #     # #####
	 #     #       #     #      #  #     #     #     # #    # # #    #
	 #     #       #     #     #   #     #     #     # ##   # #  #  #
	 #     #       #     #    #    #     #     #     # # #  # #   ##
	 #     #       #     #   #     #     #     #     # #  # # #   ##
	 #     #       #     #  #      #     #     #     # #   ## #  #  #
	 ##### ####### # ##### #       #####  #####  ##### #    # # #    #

			  #####  ####### #    #     #####        # #
			 #     #    #    #   #     #     #      #  #    #
			 #          #    #  #            #     #   #    #
			 #  ####    #    ###        #####     #    #    #
			 #     #    #    #  #            #   #     #######
			 #     #    #    #   #     #     #  #           #
			  #####     #    #    #     #####  #            #
=============================================================================*/
#elif (defined(__FreeBSD__) \
	|| defined(__DragonFly__) || defined(__OpenBSD__) || defined(__linux__))
#include <sys/types.h>	/* basic types */
#include <sys/file.h>	/* flock */
#include <sys/stat.h>	/* stat */
#include <sys/time.h>	/* gettimeofday */
#include <stdio.h>		/* remove */
#include <stdlib.h>		/* getenv */
#include <pthread.h>	/* pthread_self etc. */
#include <time.h>		/* time routines incl clock_gettime() */
#include <dirent.h>		/* opendir,closedir, dirent */
#include <unistd.h>		/* sleep, write, read, etc. */
#include <fcntl.h>		/* open, fstat */
#include <unistd.h>		/* for read/write and close */
#include <sys/mman.h>	/* mmap, munmap */
#include <errno.h>		/* for errno */
#include <signal.h>		/* signal/SIGPIPEs */
#include <ctype.h>		/* tolower */
#include <syslog.h>		/* openlog/syslog/closelog/vsyslog */
#include <termios.h>	/* tcgetattr/tcsetattr */
#include <semaphore.h>	/* POSIX semaphores (sem_XXX) */
#ifdef __linux__
	#include <sys/random.h> /* getrandom */
#elif !defined(__MACH__)
	#include <sys/sysctl.h> /* sysctl */
#endif

#include <glib.h>
#include <gtk/gtk.h>
#if 0 /* Old WSLaunchURL method */
#include <gconf/gconf-client.h>
#endif
#include <dlfcn.h>		/* dlopen (link with -ldl) */
#if defined(WSHASAPPINDICATOR) /* Newer Ubuntu-originated AppIndicator */
	#if defined(WSHASAYATANAAPPINDICATOR)
		#include <libayatana-appindicator/app-indicator.h> /* AppIndicator/Status icon */
	#else
		#include <libappindicator/app-indicator.h> /* AppIndicator/Status icon */
	#endif
#endif
#define DOERROR WSLogError
#define ERROR(s) DOERROR(#s);
#define VERIFYEQ(s, e) if (s != e) ERROR(s)


/*-----------------------------------------------------------------------------*/
/*	Path Handling*/
/*-----------------------------------------------------------------------------*/
void MakePathAbsolute(char* abspath, const char* path)
{
	if (!realpath(path, abspath))
	{
		perror("realpath() failed. That's interesting.");
		abspath[0] = '\0';
	}
}

wsbool GdkThemePathPositions(const char* path, size_t pathlen,
	size_t* outslashpos, size_t* outextpos, size_t* outextendpos)
{
	char pathsepchar = WSPATHSEPARATORCHAR, dotchar = '.';
	size_t slashpos, extpos;
	slashpos = CStrrfind(path, pathlen,
		&pathsepchar, WSMAXLENGTH, 1);
	if (slashpos == WSMAXLENGTH || pathlen <= slashpos)
		*outslashpos = 0;
	else
		*outslashpos = slashpos + 1;
	extpos = CStrrfind(path, pathlen,
		&dotchar, WSMAXLENGTH, 1);
	if (extpos == WSMAXLENGTH ||
		(*outslashpos && extpos < slashpos))
		return FALSE;
	if (*outslashpos)
		*outextpos = slashpos + 1;
	else
		*outextpos = 0;
	*outextendpos = extpos;
	return TRUE;
}


/*=============================================================================*/
/**/
/* OS-Dependent GUI */
/**/
/*=============================================================================*/

/*-----------------------------------------------------------------------------*/
/*	WSGetDPI()*/
/*-----------------------------------------------------------------------------*/

void WSDisplayGetScaleFactor(guipos_t* x, guipos_t* y)
{
#if GTK_CHECK_VERSION (3, 2, 2)
	#if GTK_CHECK_VERSION (4, 0, 0)
		GdkDisplay *display = gdk_display_get_default();
		GListModel* list = gdk_display_get_monitors(display);
		*x = *y = gdk_monitor_get_scale_factor(
						GDK_MONITOR(g_list_model_get_item(list, 0)));
	#else
		GdkScreen *screen = gdk_screen_get_default();
		GdkDisplay* display = gdk_screen_get_display(screen);
		*x = *y = gdk_screen_get_monitor_scale_factor(screen, 0);
	#endif
#else
	WSDisplayGetDPI(x, y);
	*x /= 96;
	*y /= 96;
#endif
}

void WSDisplayGetDPI(guipos_t* x, guipos_t* y)
{
#if GTK_CHECK_VERSION (3, 2, 2)
	gint width_mm;
	gint height_mm;
	gint width_px;
	gint height_px;

#if GTK_CHECK_VERSION (4, 0, 0) /* Version on GTK4 is slightly different */
	GdkDisplay *display = gdk_display_get_default();
	GListModel* list = gdk_display_get_monitors(display);
	GdkMonitor* monitor = GDK_MONITOR(g_list_model_get_item(list, 0));
	GdkRectangle geometry;
	width_mm = gdk_monitor_get_width_mm(monitor);
	height_mm = gdk_monitor_get_height_mm(monitor);
	gdk_monitor_get_geometry(monitor, &geometry);
	width_px = geometry.width;
	height_px = geometry.height;
#else /* GTK3 */
	GdkScreen *screen = gdk_screen_get_default();
	GdkDisplay* display = gdk_screen_get_display(screen);
	GdkMonitor *monitor = gdk_display_get_primary_monitor(display);
	if(!monitor)
		monitor = gdk_display_get_monitor(display, 0);
	width_mm = gdk_monitor_get_width_mm(monitor);
	height_mm = gdk_monitor_get_height_mm(monitor);
	width_px = gdk_screen_get_width(screen);
	height_px = gdk_screen_get_height(screen);
#endif
	if(width_mm != 0 && height_mm != 0)
	{
		*x = (width_px / (width_mm / 25.4));
		*y = (height_px / (height_mm / 25.4));
		return;
	}
#endif
#if 1 /* https://www.purebasic.fr/english/viewtopic.php?f=15&t=63467 (GDK) */
	/*gtk_settings_get_for_screen is an option too eventually*/
	GValue dpi = G_VALUE_INIT;
	GtkSettings* defsettings = gtk_settings_get_default();
	WSASSERT(defsettings); /* should be a default screen... */
	g_value_init(&dpi, G_TYPE_UINT);
	g_object_get_property(G_OBJECT(defsettings), "gtk-xft-dpi", &dpi);
	*x = *y = (int)(g_value_get_uint(&dpi) / 1024);
	g_value_unset(&dpi);
	/*printf("The DPI is %d x %d\n", *x, *y);*/
#else /* XWindows (#include <X11/Xlib.h>) */
	double xres, yres;
	Display *dpy;
	char *displayname = NULL;
	int scr = 0; /* Screen number */

	if(x == NULL || y == NULL)
		return;

	dpy = XOpenDisplay (displayname);

	/*
	* there are 2.54 centimeters to an inch; so there are 25.4 millimeters.
	*
	*     dpi = N pixels / (M millimeters / (25.4 millimeters / 1 inch))
	*         = N pixels / (M inch / 25.4)
	*         = N * 25.4 pixels / M inch
	*/
	xres = ((((double) DisplayWidth(dpy, scr)) * 25.4) /
		((double) DisplayWidthMM(dpy, scr)));
	yres = ((((double) DisplayHeight(dpy, scr)) * 25.4) /
		((double) DisplayHeightMM(dpy, scr)));

	*x = (int) (xres + 0.5);
	*y = (int) (yres + 0.5);

	XCloseDisplay (dpy);
#endif
}

/*=============================================================================*/
/**/
/*	GUI (GTK)*/
/**/
/*=============================================================================*/

/*-----------------------------------------------------------------------------*/
/*	GUI Prototypes*/
/*-----------------------------------------------------------------------------*/

#define WSWINDOW_WIDGET_HANDLE(pThis) (*((GtkWidget**)pThis))
#define WSWINDOW_WINDOW_HANDLE(pThis) (*((GtkWindow**)pThis))
#define WSSCROLLEDWINDOW_HANDLE(pThis) (*((GtkScrolledWindow**)pThis))
#define WSSCROLLEDWINDOW_WIDGET_HANDLE(pThis) (*((GtkWidget**)pThis))
#define WSMENUBAR_WIDGET_HANDLE(pThis) (*((GtkWidget**)pThis))
#define WSTEXTCTRL_HANDLE(pThis) (*((GtkTextView**)pThis))
#define WSTEXTCTRL_WIDGET_HANDLE(pThis) (*((GtkWidget**)pThis))
#define WSMENU_WIDGET_HANDLE(pThis) (*((GtkWidget**)pThis))
#define WSAPP_HANDLE(x) ((x)->mApplication)

/*-----------------------------------------------------------------------------*/
/*						WS[XXX]Event methods								   */
/*-----------------------------------------------------------------------------*/
#define WSWindowFromHandle(hWnd) ((struct WSWINDOW_**)&hWnd)
WSWINDOW* WSEvent_GetWindow(WSEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSEvent_GetWindowCon(WSEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
WSWINDOW* WSPaintEvent_GetWindow(WSPAINTEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSPaintEvent_GetWindowCon(WSPAINTEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
WSWINDOW* WSKeyEvent_GetWindow(WSKEYEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSKeyEvent_GetWindowCon(WSKEYEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
WSWINDOW* WSCloseEvent_GetWindow(WSCLOSEEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSCloseEvent_GetWindowCon(WSCLOSEEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }
WSWINDOW* WSNotifyIconEvent_GetWindow(WSNOTIFYICONEVENT* ev)
{ return WSWindowFromHandle(ev->widget); }
void* WSNotifyIconEvent_GetWindowCon(WSNOTIFYICONEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->widget)); }
WSWINDOW* WSSizeEvent_GetWindow(WSSIZEEVENT* ev)
{ return WSWindowFromHandle(ev->widget); }
void* WSSizeEvent_GetWindowCon(WSSIZEEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->widget)); }
WSWINDOW* WSDestroyEvent_GetWindow(WSDESTROYEVENT* ev)
{ return WSWindowFromHandle(ev->mhWnd); }
void* WSDestroyEvent_GetWindowCon(WSDESTROYEVENT* ev)
{ return WSWindow_GetCon(WSWindowFromHandle(ev->mhWnd)); }

#if GTK_CHECK_VERSION (4, 0, 0)
size_t WSKeyEvent_GetKey(WSKEYEVENT* ev)
{
	return gdk_key_event_get_keycode(ev->mEvent);
}
guipos_t WSSizeEvent_GetWidth(WSSIZEEVENT* ev)
{ return ev->width; }
guipos_t WSSizeEvent_GetHeight(WSSIZEEVENT* ev)
{ return ev->height; }
WSWINDOW* WSMenuEvent_GetWindow(WSMENUEVENT* ev)
{ return ev->mpWindow; }
void* WSMenuEvent_GetWindowCon(WSMENUEVENT* ev)
{ return WSWindow_GetCon(ev->mpWindow); }
#else
size_t WSKeyEvent_GetKey(WSKEYEVENT* ev)
{
	GdkEventKey* keyevent = (GdkEventKey*)ev->mEvent;
	return keyevent->keyval;
}
guipos_t WSSizeEvent_GetWidth(WSSIZEEVENT* ev)
{ return WSRect_GetWidth(ev->rect); }
guipos_t WSSizeEvent_GetHeight(WSSIZEEVENT* ev)
{ return WSRect_GetHeight(ev->rect); }
WSWINDOW* WSMenuEvent_GetWindow(WSMENUEVENT* ev)
{ return ev->mpWindow; }
void* WSMenuEvent_GetWindowCon(WSMENUEVENT* ev)
{ return WSWindow_GetCon(ev->mpWindow); }
#endif

/******************************************************************************
*
*					GTK1/2/3 vs 4 Switch
*
******************************************************************************/
#if GTK_CHECK_VERSION (4, 0, 0)
#define WSMENU_HANDLE(pThis) (*((GMenu**)pThis))
#define WSMENUSECTION_HANDLE(pThis) WSMENU_HANDLE(pThis)
#define WSMENUBAR_HANDLE(pThis) WSMENU_HANDLE(pThis)
#define WSWINDOW_HANDLE(pThis) (*((WSGtkApplicationWindow**)pThis))
#define WSMENUACTIONS_HANDLE(pThis) (*((GSimpleActionGroup**)pThis))

void WSAppendActionMap(GActionMap* dst, GActionMap* src)
{
	gchar** actionnames;
	actionnames = g_action_group_list_actions(
		G_ACTION_GROUP(src)
	);
	for(;*actionnames;++actionnames)
	{
		GAction* action = g_action_map_lookup_action(
			src, *actionnames
		);
		g_action_map_add_action(dst, action);
	}
}

typedef struct GMenuModelList
{
	struct GMenuModelList* mNext;
	struct GMenuModelList* mPrev;
	GMenuModel* mMenu;
	guint mIndex;
	guint mCurIndex;
	GMenuLinkIter* mLinkIter;
} GMenuModelList;
typedef struct GMenuItemIterator
{
	GMenuModelList* mMenuList;
	GMenuModelList* mCurrent;
	GVariant* mAccelVar;
	GVariant* mLabelVar;
	GVariant* mActionVar;
} GMenuItemIterator;
void g_menu_model_iterate_items_finish(GMenuModel* topmenu, GMenuItemIterator* it)
{
	(void)(topmenu);
	if(it->mAccelVar)
		g_variant_unref(it->mAccelVar);
	if(it->mLabelVar)
	{
		g_variant_unref(it->mLabelVar);
		g_variant_unref(it->mActionVar);
	}
	while(it->mMenuList)
	{
		GMenuModelList* nextList = it->mMenuList->mNext;
		if(it->mMenuList->mLinkIter)
			g_object_unref(G_OBJECT(it->mMenuList->mLinkIter));
		g_free(it->mMenuList);
		it->mMenuList = nextList;
	}
}
gint g_menu_model_iterate_items(GMenuModel* topmenu, GMenuItemIterator* it)
{
	guint itemCount;
	GMenuModelList* curList;
	if(!it->mMenuList)
	{
		it->mMenuList = (GMenuModelList*)g_malloc(sizeof(GMenuModelList));
		if(!it->mMenuList)
			return 0;
		it->mMenuList->mMenu = topmenu;
		it->mMenuList->mNext = NULL;
		it->mMenuList->mPrev = NULL;
		it->mCurrent = it->mMenuList;
		it->mCurrent->mIndex = 0;
		it->mCurrent->mLinkIter = NULL;
	}
	else
	{
		/* clean up ... */
		if(it->mAccelVar)
			g_variant_unref(it->mAccelVar);
		if(it->mLabelVar)
		{
			g_variant_unref(it->mLabelVar);
			g_variant_unref(it->mActionVar);
		}
	}
	it->mAccelVar = it->mActionVar = it->mLabelVar = NULL;
	itemCount = g_menu_model_get_n_items(it->mCurrent->mMenu);
	for(guint i = it->mCurrent->mIndex; i < itemCount; ++i)
	{
		gint isSubMenu = 0;
		GMenuAttributeIter* atiter;
		GVariant* varaccel;
		GVariant* varaction;
		GVariant* varlabel;
		if(it->mCurrent->mLinkIter)
			isSubMenu = 1;
		else
			it->mCurrent->mLinkIter =
			g_menu_model_iterate_item_links(it->mCurrent->mMenu, i);
		while(g_menu_link_iter_next(it->mCurrent->mLinkIter))
		{
			const char* name =
				g_menu_link_iter_get_name(it->mCurrent->mLinkIter);
			GMenuModel* item =
				g_menu_link_iter_get_value(it->mCurrent->mLinkIter);
			GMenuModelList* curList = it->mCurrent;
			guint result;
			it->mCurrent->mIndex = i;
			it->mCurrent->mNext =
				(GMenuModelList*)g_malloc(sizeof(GMenuModelList));
			if(!it->mMenuList)
			{
				g_menu_model_iterate_items_finish(topmenu, it);
				return 0;
			}
			it->mCurrent->mNext->mPrev = it->mCurrent;
			it->mCurrent = it->mCurrent->mNext;
			it->mCurrent->mNext = NULL;
			it->mCurrent->mMenu = item;
			it->mCurrent->mIndex = 0;
			it->mCurrent->mLinkIter = NULL;
			/*isSubMenu = 1;*/
			return g_menu_model_iterate_items(topmenu, it);
		}
		g_object_unref(G_OBJECT(it->mCurrent->mLinkIter));
		it->mCurrent->mLinkIter = NULL;
		if(isSubMenu)
			continue;
		WSASSERT(it->mCurrent->mMenu != topmenu);

		atiter = g_menu_model_iterate_item_attributes(it->mCurrent->mMenu, i);
		varaccel = NULL;
		varaction = NULL;
		varlabel = NULL;
		while(g_menu_attribute_iter_next (atiter))
		{
			const char* name = g_menu_attribute_iter_get_name(atiter);
			if(strcmp(name, G_MENU_ATTRIBUTE_ACTION) == 0)
				varaction =
				g_menu_attribute_iter_get_value(atiter);
			else if(strcmp(name, G_MENU_ATTRIBUTE_LABEL) == 0)
				varlabel =
				g_menu_attribute_iter_get_value(atiter);
			else if(strcmp(name, "accel") == 0)
				varaccel =
				g_menu_attribute_iter_get_value(atiter);
		}/* end iter */
		g_object_unref(G_OBJECT(atiter));

		if(varlabel && varaction)
		{
			gsize labellength;
			const gchar* label;
			label = g_variant_get_string (
				varlabel, &labellength);
			it->mAccelVar = varaccel;
			it->mActionVar = varaction;
			it->mLabelVar = varlabel;
			it->mCurrent->mIndex = i + 1; /* next entry is next index */
			it->mCurrent->mCurIndex = i;
			return 1;
		}
	} /* end items */
	if(it->mCurrent->mPrev != NULL)
	{
		GMenuModelList* prevList = it->mCurrent->mPrev;
		g_free(it->mCurrent);
		it->mCurrent = prevList;
		it->mCurrent->mNext = NULL;
		return g_menu_model_iterate_items(topmenu, it);
	}
	else
	{
		g_menu_model_iterate_items_finish(topmenu, it);
		return 0;
	}
}
void gtk_application_set_accels_for_menu(GtkApplication* pThis, GMenuModel* menu)
{
	GMenuItemIterator it;
	guint curIndex = 0;
	memset(&it, 0, sizeof(it));
	while(g_menu_model_iterate_items(menu, &it))
	{
		if(it.mAccelVar)
		{
			gsize actionlength, accellength;
			const gchar* action;
			const gchar* accel;
			const gchar* accels[2];
			action  = g_variant_get_string (
				it.mActionVar, &actionlength);
			accel = g_variant_get_string (
				it.mAccelVar, &accellength);
			accels[0] = accel;
			accels[1] = NULL;
			gtk_application_set_accels_for_action(
				pThis, action, accels);
		}
	}
}
/*-----------------------------------------------------------------------------*/
/*	GTK4 WSWINDOW															   */
/*-----------------------------------------------------------------------------*/
#define WSGTKAPPLICATIONWINDOWPARENT GtkApplicationWindow
#define WSGTKAPPLICATIONWINDOWCLASSPARENT GtkApplicationWindowClass

#define WSGTK_APPLICATION_WINDOW_TYPE                  (wsgtk_application_window_get_type ())
#define WSGTK_APPLICATION_WINDOW(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), WSGTK_APPLICATION_WINDOW_TYPE, WSGtkApplicationWindow))
#define WSGTK_APPLICATION_WINDOW_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST  ((klass), WSGTK_APPLICATION_WINDOW_TYPE, WSGtkApplicationWindowClass))
#define WSGTK_APPLICATION_WINDOW_IS(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WSGTK_APPLICATION_WINDOW_TYPE))
#define WSGTK_APPLICATION_WINDOW_IS_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE  ((klass), WSGTK_APPLICATION_WINDOW_TYPE))
#define WSGTK_APPLICATION_WINDOW_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS  ((obj), WSGTK_APPLICATION_WINDOW_TYPE, WSGtkApplicationWindowClass))

typedef struct _WSGtkApplicationWindow      WSGtkApplicationWindow;
typedef struct _WSGtkApplicationWindowClass WSGtkApplicationWindowClass;

typedef wsbool (*LPWSGtkApplcationWindowOnSize)(GtkWidget* pWindow, guipos_t x, guipos_t y, guipos_t baseline);
struct _WSGtkApplicationWindow
{
	GtkApplicationWindow parent_instance;
	void* mCon;
	LPWSGtkApplcationWindowOnSize OnSize;
};

struct _WSGtkApplicationWindowClass
{
	GtkApplicationWindowClass parent_class;
};

GType wsgtk_application_window_get_type(void);

gpointer wsgtk_application_window_new(GtkApplication *app)
{
	return g_object_new(WSGTK_APPLICATION_WINDOW_TYPE, "application", app, NULL);
}

G_DEFINE_TYPE(WSGtkApplicationWindow, wsgtk_application_window, GTK_TYPE_APPLICATION_WINDOW)

static void
wsgtk_application_window_size_allocate(GtkWidget *widget, int width, int height, int baseline)
{
	GtkWidgetClass *parent_class;
	WSGtkApplicationWindow* pThis = WSGTK_APPLICATION_WINDOW(widget);
	(*pThis->OnSize)(widget, width, height, baseline);
	parent_class = GTK_WIDGET_CLASS(g_type_class_peek_parent(G_OBJECT_GET_CLASS(pThis)));
	parent_class->size_allocate(widget, width, height, baseline);
}

static void wsgtk_application_window_class_init(WSGtkApplicationWindowClass* klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
	widget_class->size_allocate = wsgtk_application_window_size_allocate;
}

static void wsgtk_application_window_init(WSGtkApplicationWindow* self)
{}


void WSWindow_Create(WSWINDOW* pThis, WSAPP* pApp, WSRECT* bounds,
	const char* title, WSMENUBAR* pMenuBar, WSWNDCLASS* pClass)
{
	GtkWidget* widget;
	GtkWindow* window;
	(void)(pApp);
	WSWINDOW_HANDLE(pThis) =
		WSGTK_APPLICATION_WINDOW(wsgtk_application_window_new(WSAPP_HANDLE(pApp)));
	widget = GTK_WIDGET(WSWINDOW_HANDLE(pThis));
	window = GTK_WINDOW(WSWINDOW_HANDLE(pThis));

	/*equiv of:*/
	/*gtk_container_set_border_width(GTK_CONTAINER(WSWINDOW_HANDLE(pThis)), 5)*/
	/*gtk_widget_set_margin_start (widget, 5);*/
	/*gtk_widget_set_margin_end (widget, 5);*/
	/*gtk_widget_set_margin_top (widget, 5);*/
	/*gtk_widget_set_margin_bottom (widget, 5);*/

	/*gtk_window_move(WSWINDOW_HANDLE(pThis), bounds->left, bounds->top);*/
	gtk_window_set_default_size(window,
		WSRect_GetWidth(bounds), WSRect_GetHeight(bounds));
	gtk_window_set_title(window, title);
	/* GTK widgets can only contain one widget without a box or other */
	/* layout mechanism. Since we have a menubar + more it needs at least 2. */
	gtk_window_set_child(window, gtk_fixed_new());
	g_signal_connect(G_OBJECT(WSWINDOW_HANDLE(pThis)), "close_request",
						G_CALLBACK((*pClass->mWindowMessages)(WM_WSCLOSE)), (gpointer) pThis);
	gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(WSWINDOW_HANDLE(pThis)), TRUE);
	gtk_window_set_default_icon_name((const char*)*pClass->mWindowIcon);
	gtk_window_set_icon_name(GTK_WINDOW(WSWINDOW_HANDLE(pThis)), (const char*)*pClass->mWindowIcon);
	gtk_window_set_resizable(GTK_WINDOW(WSWINDOW_HANDLE(pThis)), TRUE);
	/*gtk_window_present(GTK_WINDOW(WSWINDOW_HANDLE(pThis)));*/
	WSWINDOW_HANDLE(pThis)->OnSize =
		(LPWSGtkApplcationWindowOnSize)(*pClass->mWindowMessages)(WM_WSSIZE);
}


wsbool WSWindow_IsCreated(WSWINDOW* pThis)
{
	return *pThis != NULL;
}

void WSWindow_SetMenuBar(WSWINDOW* pThis, WSMENUBAR* pMenuBar,
	WSMENUACTIONS* ma)
{
	(void)(pThis); (void)(pMenuBar); (void)(ma);
}
wsbool WSWindow_IsVisible(WSWINDOW* pThis)
{ /* gtk_widget_is_visible returns TRUE only if parent is also visible */
	return gtk_widget_get_visible(WSWINDOW_WIDGET_HANDLE(pThis));
}

void WSIcon_CreateFromFile(WSICON* pThis, const char* path, size_t pathlen)
{
	char pathstr[PATH_MAX];
	const char* extstr;
	size_t pathstrlen;
	size_t slashpos, extpos, extendpos;
	*pThis = NULL;
	if(path[0] != WSPATHSEPARATORCHAR)
	{
		MakePathAbsolute(pathstr, path);
		pathstrlen = CStrLength(pathstr);
	}
	else
	{
		memcpy(pathstr, path, sizeof(char) * (pathlen + 1));
		pathstrlen = pathlen;
	}
	if(pathstr[0])
	{
		if(GdkThemePathPositions(path, pathlen, &slashpos, &extpos, &extendpos))
		{
			size_t extstrlen;
#if defined(WIKISERVER_VERSION)
			extstr = StringDupWithEnd(&path[extpos], &path[extendpos]);
#else
			extstr = g_strndup(&path[extpos], &path[extendpos] - &path[extpos]);
#endif
			extstrlen = extendpos - extpos;
			/*https://stackoverflow.com/questions/71847278/gtk-4-and-applications-icons-how-to-include-an-application-icon-in-a-portable */
			GtkIconTheme* icon_theme = gtk_icon_theme_get_for_display(
				gdk_display_get_default());
			size_t findpos = CStrFind(pathstr,
				pathstrlen, extstr, extstrlen
				, 0);
			if(findpos != WSMAXLENGTH)
			{
				pathstr[findpos] = '\0';
				gtk_icon_theme_add_search_path(icon_theme, pathstr);
				if(gtk_icon_theme_has_icon(icon_theme, extstr) == 1)
				{
					/*gtk_window_set_default_icon_name(extstr);*/
					/*gtk_window_set_icon_name(GTK_WINDOW(WSWINDOW_HANDLE(pThis)),*/
					/*							extstr);*/
					*pThis = (struct WSICON_*)extstr;
					extstr = NULL;
				}
			}
			WSFREE(extstr);
		}
	}
}
void WSIcon_Destroy(WSICON* pThis)
{
	WSFREE(*pThis);
}

wsbool WSWindow_SetIconFromFilePath(WSWINDOW* pThis, const char* path, size_t pathlen)
{
	/*WSICON icon;
	WSIcon_CreateFromFile(&icon, path, pathlen);
	gtk_window_set_default_icon_name((const char*)icon);
	gtk_window_set_icon_name(GTK_WINDOW(WSWINDOW_HANDLE(pThis)), (const char*)icon);
	WSIcon_Destroy(&icon);*/
	return TRUE;
}

void WSWindow_Hide(WSWINDOW* pThis)
{
	gtk_widget_hide(WSWINDOW_WIDGET_HANDLE(pThis));
}

void WSWindow_Show(WSWINDOW* pThis)
{
	/*
		gtk_window_present brings it to the front,
		gtk_widget_show often displays in the background
		in gtk4, which is awkward.
	*/
	gtk_window_present(GTK_WINDOW(WSWINDOW_HANDLE(pThis)));
}

void WSWindow_Destroy(WSWINDOW* pThis)
{
	gtk_window_destroy(GTK_WINDOW(WSWINDOW_HANDLE(pThis)));
}

/*-----------------------------------------------------------------------------
					GTK4 WSTEXTCTRL
-----------------------------------------------------------------------------*/
#undef WSTextCtrl_CreateScrolled
wsbool WSTextCtrl_CreateScrolled(WSTEXTCTRL* pThis, WSSCROLLEDWINDOW* pParent,
	WSWINDOW* pTopParent, WSRECT* bounds)
{
	WSTEXTCTRL_WIDGET_HANDLE(pThis) = gtk_text_view_new();
	WSSCROLLEDWINDOW_WIDGET_HANDLE(pParent) =
	gtk_scrolled_window_new();
	gtk_scrolled_window_set_policy(WSSCROLLEDWINDOW_HANDLE(pParent),
									GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_child(WSSCROLLEDWINDOW_HANDLE(pParent),
		WSTEXTCTRL_WIDGET_HANDLE(pThis));
	gtk_fixed_put(
		GTK_FIXED(gtk_window_get_child(GTK_WINDOW(WSWINDOW_HANDLE(pTopParent)))),
		WSSCROLLEDWINDOW_WIDGET_HANDLE(pParent), bounds->left, bounds->top);
	gtk_widget_set_vexpand(WSSCROLLEDWINDOW_WIDGET_HANDLE(pParent), TRUE);
	gtk_widget_set_size_request(WSSCROLLEDWINDOW_WIDGET_HANDLE(pParent),
		WSRect_GetWidth(bounds), WSRect_GetHeight(bounds));
	return TRUE;
}

void WSTextCtrl_Destroy(WSTEXTCTRL* pThis)
{
}

/*-----------------------------------------------------------------------------
					GTK4 WSMENU[BAR/SECTION]
-----------------------------------------------------------------------------*/
/* https://github.com/ToshioCP/Gtk4-tutorial/blob/main/gfm/sec17.md */
void WSMenuBar_Create(WSMENUBAR* pThis)
{
	WSMENU_HANDLE(pThis) = g_menu_new();
}

void WSMenuBar_Append(WSMENUBAR* pThis, WSMENU* pMenu, const char* title)
{/*g_menu_item_new_submenu() and g_menu_insert_item()*/
	g_menu_append_submenu(WSMENUBAR_HANDLE(pThis), title,
		G_MENU_MODEL(WSMENU_HANDLE(pMenu)));
}

void WSMenuBar_Draw(WSMENUBAR* pThis)
{
}

void WSMenuBar_Destroy(WSMENUBAR* pThis)
{
	g_object_unref(G_OBJECT(WSMENU_HANDLE(pThis)));
}


guint g_menu_model_set_item_label(GMenuModel* pThis,
	guint index, const gchar* newlabel)
{
	GMenuItemIterator it;
	guint curIndex = 0;
	memset(&it, 0, sizeof(it));
	while(g_menu_model_iterate_items(pThis, &it))
	{
		if(++curIndex == index)
		{
			gsize actionlength, labellength, accellength;
			const gchar* action = g_variant_get_string (
				it.mActionVar, &actionlength);
			const gchar* label = g_variant_get_string (
				it.mLabelVar, &labellength);
			GMenuItem* newitem = g_menu_item_new(newlabel, action);
			if(it.mAccelVar)
				g_menu_item_set_attribute_value(newitem, "accel", it.mAccelVar);
			g_menu_remove(G_MENU(it.mCurrent->mMenu), it.mCurrent->mCurIndex);
			g_menu_insert_item(G_MENU(it.mCurrent->mMenu),
				it.mCurrent->mCurIndex, newitem);
			return 1;
		}
	}
	return 0;
}
void WSMenu_SetItemTitle(WSMENU* pThis, size_t index, const char* title)
{
	/*
		All I could find online is:
		https://discourse.gnome.org/t/gtkapplicationwindow-with-dynamic-menu-labels/977/2

		However it is very possible, if ... very ... convoluted.
		We know our menu is always a section->item structure which simplifies it.
		We first iterate the links of our sections, then the
		attributes of the submenus of the sections which are the items.

		It isn't possible to set a title of a GMenuItem directly as when
		a GMenuItem is addded to a GMenu it copies over the label and action
		and other attributes as GVariants in a GHashMap, which can never
		be directly modified.

		So, we iterate through them, finding the one we want to replace
		and remembering it's action. Then we g_menu_remove->g_menu_insert
		with the old action and the new title.
	*/
	g_menu_model_set_item_label(G_MENU_MODEL(WSMENU_HANDLE(pThis)), index, title);
}

void WSMenu_BeginSection_(WSMENU* pThis, WSMENUSECTION* pSection)
{
	WSMENUSECTION_HANDLE(pSection) = g_menu_new();
}

void WSMenuActions_Create(WSMENUACTIONS* pThis)
{
	WSMENUACTIONS_HANDLE(pThis) = g_simple_action_group_new();
}
void WSMenuActions_Destroy(WSMENUACTIONS* pThis)
{
	g_object_unref(G_OBJECT(WSMENUACTIONS_HANDLE(pThis)));
}

#undef WSMenu_Create
void WSMenu_Create(WSMENU* pThis, WSMENUSECTION* pSection)
{
	WSMENU_HANDLE(pThis) = g_menu_new();
	WSMenu_BeginSection_(pThis, pSection);
}

void WSMenu_Destroy(WSMENU* pThis)
{
	g_object_unref(G_OBJECT(WSMENU_HANDLE(pThis)));
}

void WSMenu_Finalize(WSMENU* pThis, WSMENUSECTION* pSection)
{
	g_menu_append_section(WSMENU_HANDLE(pThis), NULL,
		G_MENU_MODEL(WSMENUSECTION_HANDLE(pSection)));
	g_object_unref(G_OBJECT(WSMENUSECTION_HANDLE(pSection)));
}
void WSMenu_EndSection(WSMENU* pThis, WSMENUSECTION* pSection)
{
	WSMenu_Finalize(pThis, pSection);
	WSMenu_BeginSection_(pThis, pSection);
}

#define WSMENUSECTION_ACCEL_CONTROL 1
/*GVariant* varaccel = g_variant_new_string("<Control>S");*/
/*g_menu_item_set_attribute_value(newitem, "accel", varaccel);*/
wsbool WSMenuSection_AppendWithAccel(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*cb), WSWINDOW* cd,
	const char* accel, WSMENUACTIONS* ma)
{
	typedef void (*GActionEntryCBProc)(GSimpleAction*,GVariant*,gpointer);
	char* actionfullname;
	size_t actionfullnamelen;
	GActionEntry action = { title, (GActionEntryCBProc)cb, NULL, NULL, NULL,{0,0,0} };
#if !defined(WIKISERVER_VERSION)
	char* actionname;
	size_t actionnamelen;
	GString* s = g_string_new("");
	g_string_assign(s, g_utf8_strdown(title, -1));
	g_string_replace(s, " ", "", 0);
	actionnamelen = s->len;
	actionname = g_string_free(s, FALSE);
#else
	size_t actionnamelen = CStrLength(title);
	char* actionname = StringDupWithEnd(title, title + actionnamelen);
#endif
	(void)(commandid);
	if(!*pThis)
		return FALSE;
#if defined(WIKISERVER_VERSION)
	for(size_t i = 0; i < actionnamelen; ++i)
	{
		if(actionname[i] == ' ')
		{
			memmove(&actionname[i], &actionname[i + 1],
				sizeof(char) * (actionnamelen - i + 1));
			--actionnamelen;
			--i;
		}
		else
		{
			size_t charlen = UTF8CharLen(&actionname[i]);
			uint32_t ucschar = UTF8CharToUCS(&actionname[i]);
			uint32_t ucslowerchar = UCSCharToLower(ucschar);
			size_t lowercharlen = UCSCharUTF8Size(ucslowerchar);
			if(lowercharlen > charlen)
			{
				char* newactionname;
				size_t lendiff = lowercharlen - charlen;
				actionnamelen += lendiff;
				newactionname = WSREALLOC(actionname, char, actionnamelen + 1);
				if(!newactionname)
				{
					WSFREE(actionname);
					return FALSE;
				}
				actionname = newactionname;
				memmove(&actionname[i + lendiff], &actionname[i],
					sizeof(char) * (actionnamelen - lendiff + 1));
				for(size_t j = 0; j < lowercharlen; ++j)
					actionname[i + j] = ' ';
			}
			else if(lowercharlen < charlen)
			{
				size_t lendiff = charlen - lowercharlen;
				memmove(&actionname[i], &actionname[i + lendiff],
					sizeof(char) * (actionnamelen - lendiff + 1));
				actionnamelen -= lendiff;
			}
			UCSCharToUTF8(ucslowerchar, &actionname[i]);
			i += lowercharlen - 1;
		}
	}
#endif
	actionfullname = WSALLOC(char, strcountof("app.") + actionnamelen + 1);
	if(!actionfullname)
	{
		WSFREE(actionname);
		return FALSE;
	}
	memcpy(actionfullname, "app.", sizeof(char) * strcountof("app."));
	memcpy(&actionfullname[strcountof("app.")], actionname, sizeof(char) * (actionnamelen + 1));
	action.name = actionname;
/*	printf("name[%s] fullname[%s] namelen[%d]\n",*/
/*		actionname.c_str(), actionfullname.c_str(), actionname.length());*/

	if(accel)
	{
		char accelstr[strcountof("<Control>") + 1 + 1];
		GVariant* varaccel;
		GMenuItem* newitem =
			g_menu_item_new(title, actionfullname);
		memcpy(accelstr, "<Control>", sizeof(char) * strcountof("<Control>"));
		accelstr[strcountof("<Control>")] = accel[0];
		accelstr[strcountof("<Control>") + 1] = '\0';
		varaccel = g_variant_new_string(accelstr);
		g_menu_item_set_attribute_value(newitem, "accel", varaccel);
		g_menu_append_item(WSMENU_HANDLE(pThis), newitem);
	}
	else
		g_menu_append(WSMENU_HANDLE(pThis), title, actionfullname);

	g_action_map_add_action_entries(G_ACTION_MAP(WSMENUACTIONS_HANDLE(ma)),
		&action, 1, cd);
	WSFREE(actionname);
	WSFREE(actionfullname);
	return TRUE;
}

wsbool WSMenuSection_Append(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*cb), WSWINDOW* cd,
	WSMENUACTIONS* ma)
{
	return WSMenuSection_AppendWithAccel(pThis, title, commandid,
		cb, cd, NULL, ma);
}

/*-----------------------------------------------------------------------------
					GTK4 WSNOTIFYICON
-----------------------------------------------------------------------------*/
wsbool WSNotifyIcon_Create(WSNOTIFYICON* pThis, WSWINDOW* pWindow,
	const char* tip, size_t tiplen, WSICON* pIcon, const char* iconpath,
	uint16_t command)
{
	return FALSE;
}
void WSNotifyIcon_SetTooltip(WSNOTIFYICON* pThis, const char* title)
{
}
void WSNotifyIcon_SetIcon(WSNOTIFYICON* pThis, const char* path)
{
}
void WSNotifyIcon_SetMenu(WSNOTIFYICON* pThis, WSMENU* pMenu)
{
}
void WSNotifyIcon_Destroy(WSNOTIFYICON* pThis)
{
}

/*-----------------------------------------------------------------------------
					GTK4 WSMessageBox()
-----------------------------------------------------------------------------*/
#define MB_OK			0
#define MB_YESNO		1
#define MB_OKCANCEL		2
#define MB_ICONSTOP		4
#define MB_ICONCAUTION	8
void message_response(GtkDialog* dialog, gint response, gpointer data)
{
	/*
		GTK_RESPONSE_NONE
		GTK_RESPONSE_REJECT
		GTK_RESPONSE_ACCEPT
		GTK_RESPONSE_DELETE_EVENT
		GTK_RESPONSE_OK
		GTK_RESPONSE_CANCEL
		GTK_RESPONSE_CLOSE
		GTK_RESPONSE_YES
		GTK_RESPONSE_NO
		GTK_RESPONSE_APPLY
		GTK_RESPONSE_HELP
	*/
	int* retres = (int*)data;
	*retres = response;
	gtk_window_destroy(GTK_WINDOW(dialog));
}
int WSMessageBox(WSWINDOW* pWindow, const char* title, const char* message, int type)
{ /* https://gist.github.com/cjxgm/4a5d9006a6f9f19ee6daf4974abfa9cf */
	guint prevNumTopLevel = g_list_model_get_n_items(gtk_window_get_toplevels());

#if GTK_CHECK_VERSION (4, 10, 0)
	GtkAlertDialog* dialog = gtk_alert_dialog_new(
		"%s", message);
	gtk_alert_dialog_set_detail(dialog, title);
	gtk_alert_dialog_show(dialog, NULL);
#else
	int ret;
	GtkWidget* dialog = gtk_message_dialog_new(
		pWindow ? (GtkWindow*)WSWINDOW_HANDLE(pWindow) : NULL, GTK_DIALOG_MODAL,
		(type & MB_ICONSTOP) ? GTK_MESSAGE_ERROR :
			((type & MB_ICONCAUTION) ? GTK_MESSAGE_WARNING :
									   GTK_MESSAGE_OTHER),
		(type & MB_YESNO) ? GTK_BUTTONS_YES_NO :
			((type & MB_OKCANCEL) ? GTK_BUTTONS_OK_CANCEL :
									   GTK_BUTTONS_OK),
		"%s", title);
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
		"%s", message);
		/*gtk_window_set_title(GTK_WINDOW(dialog), title);*/
	gtk_widget_show(GTK_WIDGET(dialog));
	g_signal_connect (dialog, "response",
							  G_CALLBACK(message_response), &ret);
#endif
	while(g_list_model_get_n_items(gtk_window_get_toplevels()) > prevNumTopLevel)
	{
		/*printf("NUM LEFT:[%d], Prev TopLevel:[%d]\n",*/
		/*	g_list_model_get_n_items(gtk_window_get_toplevels()),*/
		/*	prevNumTopLevel);*/
		g_main_context_iteration(NULL, TRUE);
	}

#if GTK_CHECK_VERSION (4, 10, 0)
/*	g_object_unref(G_OBJECT(dialog));*/
	return 0;
#else
	return ret;
#endif
}

/*-----------------------------------------------------------------------------
					GTK4 WSEVENTLOOP
-----------------------------------------------------------------------------*/
wsbool WSEventLoop_Create(WSEVENTLOOP* pThis)
{
	/*pThis->mContext = g_main_context_new();*/
	/*pThis->mLoop = g_main_loop_new(pThis->mContext, FALSE);*/
	/*return pThis->mContext && pThis->mLoop;*/
	return TRUE;
}
wsbool WSEventLoop_HasEvent(WSEVENTLOOP* pThis)
{
	return g_main_context_pending(NULL);
}
wsbool WSEventLoop_RunSingle(WSEVENTLOOP* pThis)
{ /* gtk_main_quit(); to quit */
	if(!WSEventLoop_HasEvent(pThis))
		return FALSE;
	/*g_main_context_iteration(pThis->mContext, FALSE);*/
	g_main_context_iteration(NULL, FALSE);
	return TRUE;
}
wsbool WSEventLoop_IsQuitMessage(WSEVENTLOOP* pThis)
{
	return FALSE;
}
void WSEventLoop_Destroy(WSEVENTLOOP* pThis)
{
	/*g_free(pThis->mContext);*/
	/*g_free(pThis->mLoop);*/
}
#else /* not GTK 4.0+ */
#define WSMENU_HANDLE(pThis) (*((GtkMenu**)pThis))
#define WSMENUSECTION_HANDLE(pThis) WSMENU_HANDLE(pThis)
#define WSMENUBAR_HANDLE(pThis) (*((GtkMenuBar**)pThis))
#define WSWINDOW_HANDLE(pThis) (*((WSGtkWindow**)pThis))
#define WSMENUACTIONS_HANDLE(pThis) (*((GtkAccelGroup**)pThis))

/*-----------------------------------------------------------------------------*/
/*	GTK2/3 WSWINDOW															   */
/*-----------------------------------------------------------------------------*/
#define WSGTKWINDOWPARENT GtkWindow
#define WSGTKWINDOWCLASSPARENT GtkWindowClass

#define WSGTK_WINDOW_TYPE                  (wsgtk_window_get_type ())
#define WSGTK_WINDOW(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), WSGTK_WINDOW_TYPE, WSGtkWindow))
#define WSGTK_WINDOW_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST  ((klass), WSGTK_WINDOW_TYPE, WSGtkWindowClass))
#define WSGTK_WINDOW_IS(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WSGTK_WINDOW_TYPE))
#define WSGTK_WINDOW_IS_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE  ((klass), WSGTK_WINDOW_TYPE))
#define WSGTK_WINDOW_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS  ((obj), WSGTK_WINDOW_TYPE, WSGtkWindowClass))

typedef struct _WSGtkWindow      WSGtkWindow;
typedef struct _WSGtkWindowClass WSGtkWindowClass;

typedef wsbool (*LPWSGtkWindowOnSize)(GtkWidget *widget, GtkAllocation *allocation);
struct _WSGtkWindow
{
	GtkWindow parent_instance;
	void* mCon;
	LPWSGtkWindowOnSize OnSize;
};

struct _WSGtkWindowClass
{
	GtkWindowClass parent_class;
};

GType wsgtk_window_get_type(void);

gpointer wsgtk_window_new()
{
	/* You may need to adjust the parameters for window creation as GtkWindow does not take GtkApplication as a parameter */
	return g_object_new(WSGTK_WINDOW_TYPE, NULL);
}

G_DEFINE_TYPE(WSGtkWindow, wsgtk_window, GTK_TYPE_WINDOW)

static void
wsgtk_window_size_allocate(GtkWidget *widget, GtkAllocation* allocation)
{
	GtkWidgetClass *parent_class;
	GdkWindow* gdk_window;
	WSGtkWindow* pThis = WSGTK_WINDOW(widget);
	GdkRectangle frame_extents;
	int window_x, window_y;
	int window_width, window_height;
	int client_x, client_y, client_width, client_height;
	gdk_window = gtk_widget_get_window(widget);
	if(gdk_window)
	{	/* GTK2 allocation gives us the non-client size */
		GtkAllocation adjallocation;
#if GTK_CHECK_VERSION (3, 0, 0)
		gdk_window_get_geometry(gdk_window, &window_x, &window_y, &window_width, &window_height);
#else
		int depth;
		gdk_window_get_geometry(gdk_window, &window_x, &window_y, &window_width, &window_height, &depth);
#endif /* GTK3 switch */
		gdk_window_get_frame_extents(gdk_window, &frame_extents);
		adjallocation.x = client_x = frame_extents.x - window_x;
		adjallocation.y = client_y = frame_extents.y - window_y;
		adjallocation.width = client_width = window_width - (frame_extents.width - allocation->width);
		adjallocation.height = client_height = window_height - (frame_extents.height - allocation->height);
		(*pThis->OnSize)(widget, &adjallocation);
	}
	parent_class = GTK_WIDGET_CLASS(g_type_class_peek_parent(G_OBJECT_GET_CLASS(pThis)));
	parent_class->size_allocate(widget, allocation);
}


static void wsgtk_window_class_init(WSGtkWindowClass* klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(klass);
	widget_class->size_allocate = wsgtk_window_size_allocate;
}
static void wsgtk_window_init(WSGtkWindow* self)
{ }

#define WSICON_HANDLE(pThis) (*((GdkPixbuf**)(pThis)))
void WSIcon_CreateFromFile(WSICON* pThis, const char* path, size_t pathlen)
{
	GError* error = NULL;
	WSICON_HANDLE(pThis) = gdk_pixbuf_new_from_file(path, &error);
	if (!WSICON_HANDLE(pThis))
	{
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
}
void WSIcon_Destroy(WSICON* pThis)
{
	if(WSICON_HANDLE(pThis))
		g_object_unref(WSICON_HANDLE(pThis));
}

wsbool WSWindow_SetIcon(WSWINDOW* pThis, WSICON* pIcon)
{
	if (WSICON_HANDLE(pThis))
	{
		gtk_window_set_icon(WSWINDOW_WINDOW_HANDLE(pThis), WSICON_HANDLE(pIcon));
		gtk_window_set_default_icon(WSICON_HANDLE(pIcon));
	}
	return TRUE;
}

void WSWindow_Create(WSWINDOW* pThis, WSAPP* pApp, WSRECT* bounds,
	const char* title, WSMENUBAR* pMenuBar, WSWNDCLASS* pClass)
{
	(void)(pApp);
	WSWINDOW_HANDLE(pThis) = WSGTK_WINDOW(wsgtk_window_new());
	gtk_container_set_border_width(GTK_CONTAINER(WSWINDOW_HANDLE(pThis)), 5);
	/*gtk_window_move(WSWINDOW_HANDLE(pThis), bounds->left, bounds->top);*/
	gtk_window_set_default_size(WSWINDOW_WINDOW_HANDLE(pThis), /*resize?*/
		WSRect_GetWidth(bounds), WSRect_GetHeight(bounds));
	gtk_window_set_title(WSWINDOW_WINDOW_HANDLE(pThis), title);
	/* GTK widgets can only contain one widget without a box or other */
	/* layout mechanism. Since we have a menubar + more it needs at least 2. */
#if GTK_CHECK_VERSION (3, 0, 0)
	gtk_container_add(GTK_CONTAINER(WSWINDOW_HANDLE(pThis)),
		gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
#else
	gtk_container_add(GTK_CONTAINER(WSWINDOW_HANDLE(pThis)),
		gtk_vbox_new(FALSE, 0));
#endif
	g_signal_connect(G_OBJECT(WSWINDOW_HANDLE(pThis)), "delete_event",
		G_CALLBACK((*pClass->mWindowMessages)(WM_WSCLOSE)), (gpointer) pThis);
	WSWindow_SetIcon(pThis, pClass->mWindowIcon);
	WSWINDOW_HANDLE(pThis)->OnSize =
				(LPWSGtkWindowOnSize)(*pClass->mWindowMessages)(WM_WSSIZE);
}

wsbool WSWindow_IsCreated(WSWINDOW* pThis)
{
	return *pThis != NULL;
}

void WSWindow_SetMenuBar(WSWINDOW* pThis, WSMENUBAR* pMenuBar,
	WSMENUACTIONS* ma)
{
	size_t iaccel;
	gtk_box_pack_start(GTK_BOX(gtk_bin_get_child(GTK_BIN(WSWINDOW_HANDLE(pThis)))),
		 WSMENUBAR_WIDGET_HANDLE(pMenuBar), FALSE, FALSE, 0);
	gtk_box_reorder_child(
		 GTK_BOX(gtk_bin_get_child(GTK_BIN(WSWINDOW_HANDLE(pThis)))),
		 WSMENUBAR_WIDGET_HANDLE(pMenuBar),
		 0);
	gtk_widget_show(WSMENUBAR_WIDGET_HANDLE(pMenuBar));
	gtk_window_add_accel_group(GTK_WINDOW(WSWINDOW_HANDLE(pThis)),
		WSMENUACTIONS_HANDLE(ma));
}
wsbool WSWindow_IsVisible(WSWINDOW* pThis)
{ /* gtk_widget_is_visible returns TRUE only if parent is also visible */
	return gtk_widget_get_visible(WSWINDOW_WIDGET_HANDLE(pThis));
}

wsbool WSWindow_SetIconFromFilePath(WSWINDOW* pThis, const char* path, size_t pathlen)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(path, &error);

	if (!pixbuf)
	{
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
		return FALSE;
	}

	gtk_window_set_icon(WSWINDOW_WINDOW_HANDLE(pThis), pixbuf);
	gtk_window_set_default_icon(pixbuf);
	g_object_unref(pixbuf);
	return TRUE;
}

void WSWindow_Hide(WSWINDOW* pThis)
{
	gtk_widget_hide(WSWINDOW_WIDGET_HANDLE(pThis));
}

void WSWindow_Show(WSWINDOW* pThis)
{
	gtk_widget_show_all(WSWINDOW_WIDGET_HANDLE(pThis));
}

void WSWindow_Destroy(WSWINDOW* pThis)
{
	/*puts("Destroying Main Window...");*/
	gtk_widget_destroy(WSWINDOW_WIDGET_HANDLE(pThis));
}


/*-----------------------------------------------------------------------------
					GTK2/3 WSTEXTCTRL
-----------------------------------------------------------------------------*/
#undef WSTextCtrl_CreateScrolled
wsbool WSTextCtrl_CreateScrolled(WSTEXTCTRL* pThis, WSSCROLLEDWINDOW* pParent,
	WSWINDOW* pTopParent, WSRECT* bounds)
{
	WSTEXTCTRL_WIDGET_HANDLE(pThis) = gtk_text_view_new();
	WSSCROLLEDWINDOW_WIDGET_HANDLE(pParent) =
	gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(WSSCROLLEDWINDOW_HANDLE(pParent),
									GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(WSSCROLLEDWINDOW_HANDLE(pParent)),
		WSTEXTCTRL_WIDGET_HANDLE(pThis));
	gtk_box_pack_start(
		GTK_BOX(gtk_bin_get_child(GTK_BIN(WSWINDOW_HANDLE(pTopParent)))),
		WSSCROLLEDWINDOW_WIDGET_HANDLE(pParent), TRUE, TRUE, 0);
/*
gtk_fixed_put(
		GTK_FIXED(gtk_bin_get_child(GTK_BIN(WSWINDOW_HANDLE(pTopParent)))),
		WSSCROLLEDWINDOW_WIDGET_HANDLE(pParent), bounds->left, bounds->top);
		gtk_widget_set_size_request(WSSCROLLEDWINDOW_WIDGET_HANDLE(pParent),
		bounds->right - bounds->left, bounds->bottom - bounds->top);
		*/
	return TRUE;
}

void WSTextCtrl_Destroy(WSTEXTCTRL* pThis)
{
	/*puts("Destroying Text Window...");*/
	gtk_widget_destroy(WSTEXTCTRL_WIDGET_HANDLE(pThis));
}


/*-----------------------------------------------------------------------------
					GTK2/3 WSMENU[BAR/SECTION]

					From the GTK 1.2 menu item tutorial
					http://www.gtk.org/tutorial1.2/gtk_tut-13.html
-----------------------------------------------------------------------------*/
void WSMenuBar_Create(WSMENUBAR* pThis)
{
	WSMENUBAR_HANDLE(pThis) = (GtkMenuBar*)gtk_menu_bar_new();
}

void WSMenuBar_Append(WSMENUBAR* pThis, WSMENU* pMenu, const char* title)
{
	GtkAccelGroup** newaccelgroups;
	GtkMenuItem* item = (GtkMenuItem*)gtk_menu_item_new_with_label(title);
	gtk_menu_item_set_submenu(item, (GtkWidget*)WSMENU_HANDLE(pMenu));
	gtk_menu_shell_append((GtkMenuShell*)WSMENUBAR_HANDLE(pThis), (GtkWidget*)item);
}

void WSMenuBar_Draw(WSMENUBAR* pThis)
{
	/*	gtk_widget_show_all(mainwindow);*/
}

void WSMenuBar_Destroy(WSMENUBAR* pThis)
{
	/*gtk_widget_destroy(WSMENUBAR_WIDGET_HANDLE(pThis));*/
}

void WSMenuActions_Create(WSMENUACTIONS* pThis)
{
	WSMENUACTIONS_HANDLE(pThis) = gtk_accel_group_new();
}
void WSMenuActions_Destroy(WSMENUACTIONS* pThis)
{
#if GTK_CHECK_VERSION (2, 0, 0)
	g_object_unref(G_OBJECT(WSMENUACTIONS_HANDLE(pThis)));
#else
	gtk_accel_group_unref(WSMENUACTIONS_HANDLE(pThis));
#endif
}


#undef WSMenu_Create
void WSMenu_Create(WSMENU* pThis, WSMENUSECTION* pSection)
{
	WSMENUSECTION_HANDLE(pSection) =
		WSMENU_HANDLE(pThis) = (GtkMenu*)gtk_menu_new();
}

void WSMenu_AppendSeparator(WSMENU* pThis)
{
	GtkMenuItem* item = (GtkMenuItem*)gtk_separator_menu_item_new();
	gtk_menu_shell_append((GtkMenuShell*)WSMENU_HANDLE(pThis),
						  (GtkWidget*)item);
	gtk_widget_show((GtkWidget*)item);
}

void WSMenu_SetItemTitle(WSMENU* pThis, size_t index, const char* title)
{
	GList* shmenu;
	GList* children =
		gtk_container_get_children(GTK_CONTAINER(WSMENU_HANDLE(pThis)));
	WSASSERT(children);
	shmenu = g_list_nth(children, index);
	WSASSERT(shmenu);
	gtk_menu_item_set_label(GTK_MENU_ITEM(shmenu->data), title);
}

void WSMenu_Destroy(WSMENU* pThis)
{
	/*puts("Destroying A Menu...");*/
	/*gtk_widget_destroy(WSMENU_WIDGET_HANDLE(pThis));*/
}

void WSMenu_EndSection(WSMENU* pThis, WSMENUSECTION* pSection)
{
	WSMenu_AppendSeparator(pThis);
}

wsbool WSMenuSection_AppendWithAccel(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*mmes), WSWINDOW* window,
	const char* accel, WSMENUACTIONS* ma)
{
	GtkMenuItem* item = (GtkMenuItem*)gtk_menu_item_new_with_label(title);

	if (accel)
	{
		gtk_widget_add_accelerator(GTK_WIDGET(item), "activate",
			WSMENUACTIONS_HANDLE(ma),
			gdk_keyval_from_name(accel), GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	}

	g_signal_connect(G_OBJECT(item), "activate",
		G_CALLBACK(mmes), window);
	gtk_menu_shell_append((GtkMenuShell*)WSMENU_HANDLE(pThis),
						  (GtkWidget*)item);
	gtk_widget_show((GtkWidget*)item);
	return TRUE;
}

wsbool WSMenuSection_Append(WSMENUSECTION* pThis, const char* title,
	uint16_t commandid, WSMENUEVENT_CALLBACK(*mmes), WSWINDOW* window,
	WSMENUACTIONS* ma)
{
	return WSMenuSection_AppendWithAccel(pThis, title,
		commandid, mmes, window, NULL, ma);
}

void WSMenu_Finalize(WSMENU* pThis, WSMENUSECTION* pSection)
{
}

/*-----------------------------------------------------------------------------
						GTK2/3 WSNOTIFYICON
-----------------------------------------------------------------------------*/
#if defined(WSHASAPPINDICATOR)
/*
	The docs for libappindicator are all dead links as of this writing.
	Not only that but their samples and tests on github pass all asserts
	but have buggy displays; all due to the following:

	the app_indicator_new() params are based off of the .desktop file
	if available. Sans that the icon can, and should - since
	calls like set_title will cause the status icon not to be
	shown at all if there is no valid icon, be set via
	1) passing the abolute path of the icon minus the filename,
	including the ending slash to
	app_indicator_set_icon_theme_path()
	2) passing the filename WITHOUT the path or extension to
	app_indicator_set_icon_full()

	Once, and ONLY once you have the icon set can you set the title
	with app_indicator_set_title(). The "title" in this is the
	tooltip that appears when hovering over the status icon.

	app_indicator_set_menu() sets the menu, and while it
	appears to work with or without the icon set I wouldn't risk it.

	app_indicator_set_label() puts text next to the icon on the bar,
	the first param, and the second param is another string as a "guide"
	for the max visible size of the string. Neither function of set_label
	works under KDE on FreeBSD as of this writing.

	g_object_unref() appears to be called by the gtk stack.
*/
#define WSNOTIFYICON_HANDLE(x) (*((AppIndicator**)x))
wsbool WSNotifyIcon_Create(WSNOTIFYICON* pThis, WSWINDOW* pWindow,
	const char* tip, size_t tiplen, WSICON* pIcon, const char* iconpath,
	uint16_t command)
{
	WSNOTIFYICON_HANDLE(pThis) = app_indicator_new("WikiServer",
								"wikiserver",
								APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
	app_indicator_set_status(WSNOTIFYICON_HANDLE(pThis),
								APP_INDICATOR_STATUS_ACTIVE); /* show it */
	WSNotifyIcon_SetTooltip(pThis, tip);
	WSNotifyIcon_SetIcon(pThis, iconpath);
	return TRUE;
}
void WSNotifyIcon_SetTooltip(WSNOTIFYICON* pThis, const char* title)
{
	app_indicator_set_title(WSNOTIFYICON_HANDLE(pThis), title);
	/*app_indicator_set_label(WSNOTIFYICON_HANDLE(pThis), "WHAT", "WHAT");*/
}
void WSNotifyIcon_SetIcon(WSNOTIFYICON* pThis, const char* path)
{
	/* app_indicator_set_icon_theme_path has to include the absolute */
	/* directory path but NOT the filename */
	/* app_indicator_set_icon_full has to have the filename WITHOUT the path */
	/* OR extension */
	size_t slashpos, extpos;
	char oldcharafterslash;
	char fullpath[PATH_MAX];
	size_t fullpathlen;
	/*printf("Entering WSNotifyIcon_SetIcon with abspath:[%s]\n", path);*/
	/* Make the path absolute if necessary */
	/* Make the path absolute if necessary */
	if(path[0] != '/')
	{
		if(!realpath(path, fullpath))
		{
			perror("realpath() failed. That's interesting.");
			return;
		}
		/*printf("Make path an abspath:[%s]\n", pathstr);*/
		fullpathlen = CStrLength(fullpath);
	}
	else
	{
		fullpathlen = CStrLength(path);
		memcpy(fullpath, path, sizeof(char) * (fullpathlen + 1));
	}
	/* Find the final directory (/) and cut off the char after it */
	{
		char pathsepchar = WSPATHSEPARATORCHAR;
		slashpos = CStrrfind(fullpath, fullpathlen, &pathsepchar,
			WSMAXLENGTH, 1);
	}
	if(slashpos == WSMAXLENGTH
		|| fullpathlen <= slashpos)
		return;
	/*printf("Slash rfind at [%ju]\n", slashpos);*/
	oldcharafterslash = fullpath[slashpos + 1];
	fullpath[slashpos + 1] = '\0';
	/*printf("Abs Path Minus File:[%s]\n",*/
	/* WSMutStr_GetCString(&pathstr)); */
	app_indicator_set_icon_theme_path(WSNOTIFYICON_HANDLE(pThis), fullpath);

	/* Restore the char after the directory char */
	fullpath[slashpos + 1] = oldcharafterslash;

	/* cut off the extension, if there is one */
	{
		char dotchar = '.';
		extpos = CStrrfind(fullpath, fullpathlen, &dotchar,
			WSMAXLENGTH, 1);
	}
	if(extpos != WSMAXLENGTH && extpos > slashpos)
		fullpath[extpos] = '\0';
	/*printf("File minus extension:[%s]\n",*/
	/*	&WSMutStr_GetCString(&pathstr)[slashpos + 1]);*/
	app_indicator_set_icon_full(WSNOTIFYICON_HANDLE(pThis),
		&fullpath[slashpos + 1], "");
}
void WSNotifyIcon_SetMenu(WSNOTIFYICON* pThis, WSMENU* pMenu)
{
	app_indicator_set_menu(WSNOTIFYICON_HANDLE(pThis), WSMENU_HANDLE(pMenu));
}

void WSNotifyIcon_Destroy(WSNOTIFYICON* pThis)
{
	/* freed automatically */
}
#else
typedef struct WSNOTIFYICON_
{
	GtkStatusIcon* mStatusIcon;
	WSMENU* mMenu;
}WSNOTIFYICON_;
/*#define WSNOTIFYICON_HANDLE(x) (*((GtkStatusIcon**)x))*/
#define WSNOTIFYICON_HANDLE(x) ((*x)->mStatusIcon)
void show_menu(GtkStatusIcon* status_icon, guint button,
	guint activate_time, gpointer user_data)
{
	WSNOTIFYICON* pThis = (WSNOTIFYICON*)user_data;
	if((*pThis)->mMenu)
		gtk_menu_popup(WSMENU_HANDLE((*pThis)->mMenu), 0, 0,
						gtk_status_icon_position_menu,
						status_icon, button, activate_time);
}

static void tray_icon_on_click(GtkStatusIcon *status_icon,
						gpointer data)
{ show_menu(status_icon, 1, gtk_get_current_event_time(), data); }
static void tray_icon_on_menu(GtkStatusIcon *status_icon, guint button,
						guint activate_time, gpointer data)
{ show_menu(status_icon, button, activate_time, data); }
void WSNotifyIconLogFilter(const gchar *log_domain, GLogLevelFlags log_level,
	const gchar *message, gpointer user_data)
{
	*((wsbool*)user_data) = FALSE;
}

wsbool WSNotifyIcon_Create(WSNOTIFYICON* pThis, WSWINDOW* pWindow,
	const char* tip, size_t tiplen, WSICON* pIcon, const char* iconpath,
	uint16_t command)
{
	/* Inspired by the "GTK system tray icon example" */
	/* http://yettocome.blogspot.com/2007/08/gtk-system-tray-icon-example.html */
	wsbool success;
	GtkStatusIcon* tray_icon = gtk_status_icon_new();
	*pThis = WSALLOC1(WSNOTIFYICON_);
	success = tray_icon != NULL && *pThis != NULL;
	if(success)
	{
		(*pThis)->mStatusIcon = tray_icon;
		(*pThis)->mMenu = NULL;
		g_signal_connect(G_OBJECT(tray_icon), "activate",
							G_CALLBACK(tray_icon_on_click), (gpointer)pThis);
		g_signal_connect(G_OBJECT(tray_icon),
							"popup-menu",
							G_CALLBACK(tray_icon_on_menu), (gpointer)pThis);
		WSNotifyIcon_SetTooltip(pThis, tip);
		/*
			this may trigger a
			"gtk_widget_get_scale_factor: assertion 'GTK_IS_WIDGET (widget)' failed"
			meaning legacy addindicator support isn't enabled
			https://extensions.gnome.org/extension/615/appindicator-support/
		*/
		if(WSICON_HANDLE(pIcon))
		{
			guint handler = /* set up handler to quiet the message */
				g_log_set_handler("Gtk", G_LOG_LEVEL_CRITICAL, WSNotifyIconLogFilter, &success);
			gtk_status_icon_set_from_pixbuf(WSNOTIFYICON_HANDLE(pThis), WSICON_HANDLE(pIcon));
			g_log_remove_handler("Gtk", handler);
		}
		gtk_status_icon_set_visible(tray_icon, TRUE);
	}
	if(!success)
	{
		if(tray_icon)
			g_object_unref(G_OBJECT(tray_icon));
		if(*pThis)
		{
			WSFREE(*pThis);
			*pThis = NULL;
		}
		return FALSE;
	}
	return TRUE;
}
void WSNotifyIcon_SetTooltip(WSNOTIFYICON* pThis, const char* title)
{
	gtk_status_icon_set_tooltip_text(WSNOTIFYICON_HANDLE(pThis), title);
}
void WSNotifyIcon_SetIcon(WSNOTIFYICON* pThis, const char* path)
{
	gtk_status_icon_set_from_file(WSNOTIFYICON_HANDLE(pThis), path);
}
void WSNotifyIcon_SetMenu(WSNOTIFYICON* pThis, WSMENU* pMenu)
{
	if((*pThis))
		(*pThis)->mMenu = pMenu;
}
void WSNotifyIcon_Destroy(WSNOTIFYICON* pThis)
{
	if(*pThis)
	{
		if((*pThis)->mMenu)
			WSMenu_Destroy((*pThis)->mMenu);
		g_object_unref(G_OBJECT(WSNOTIFYICON_HANDLE(pThis)));
		WSFREE(*pThis);
	}
}
#endif

/*-----------------------------------------------------------------------------
						GTK2/3 WSMessageBox()
-----------------------------------------------------------------------------*/
int WSMessageBox(WSWINDOW* pWindow,
	const char* title, const char* message, int ops)
{ /* GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO */
	GtkWidget* dialog = gtk_message_dialog_new(
		pWindow ? WSWINDOW_WINDOW_HANDLE(pWindow) : NULL,
		GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", title);
	gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
		"%s", message);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(GTK_WIDGET(dialog));
	return 0;
}

/*-----------------------------------------------------------------------------
						GTK2/3 WSEVENTLOOP
-----------------------------------------------------------------------------*/
wsbool WSEventLoop_Create(WSEVENTLOOP* pThis)
{
	return TRUE;
}
wsbool WSEventLoop_HasEvent(WSEVENTLOOP* pThis)
{
	return gtk_events_pending();
}
wsbool WSEventLoop_RunSingle(WSEVENTLOOP* pThis)
{ /* gtk_main_quit(); to quit */
	if(!WSEventLoop_HasEvent(pThis))
		return FALSE;
	*((size_t*)pThis) = gtk_main_iteration_do(FALSE);
	return TRUE;
}
wsbool WSEventLoop_IsQuitMessage(WSEVENTLOOP* pThis)
{
	return !*((size_t*)pThis);
}
void WSEventLoop_Destroy(WSEVENTLOOP* pThis)
{
}
#endif /* GTK 4.0 check */
/******************************************************************************
*
*					GTK1/2/3/4 Common
*
******************************************************************************/
/*-----------------------------------------------------------------------------
						GTK1/2/3/4 WSAPP
-----------------------------------------------------------------------------*/
/**/
/* SetupGUIThreadProc is due to gtk_init_check/gtk_parse_args */
/* blocking if there isn't an X display available. So we call */
/* them in another thread and fall back to a console event loop */
/* if they block. */
/**/
typedef struct SetupGUIThreadData
{
	char** argv;
	int argc;
	pthread_mutex_t* initGUIMutex;
	pthread_cond_t* initGUICond;
	int didSucceed;
} SetupGUIThreadData;
static void* SetupGUIThreadProc(void* indata)
{
	SetupGUIThreadData* data = (SetupGUIThreadData*)indata;
	pthread_mutex_lock(data->initGUIMutex);
	int oldtype;
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
	pthread_mutex_unlock(data->initGUIMutex);
	data->didSucceed = gdk_display_open(NULL) != NULL
#if !GTK_CHECK_VERSION (4, 0, 0)
		&& gtk_init_check(&data->argc, &data->argv)
#endif
		;
	/*gtk_parse_args(&data->argc, &data->argv) is not in GTK4*/
	pthread_cond_signal(data->initGUICond);
	return NULL;
}

static void gtkapp_startup(GApplication* application, gpointer userdata)
{
	(*((wsbool *)(userdata))) = TRUE;
}
wsbool WSApp_CreateGUI(WSAPP* pThis)
{
/* gtk_init handling */
#if 0
	gtk_init(&argc, &argv);
#else
	pthread_t t;
	pthread_mutex_t mt;
	pthread_cond_t ct;
	SetupGUIThreadData tdata;
	struct timespec ts = { 0, 0 };
	tdata.argc = WSApp_GetArgc(pThis);
	tdata.argv = WSApp_GetArgv(pThis);
	tdata.initGUICond = &ct;
	tdata.initGUIMutex = &mt;
	pthread_mutex_init(&mt, NULL);
	pthread_cond_init(&ct, NULL);
	pthread_mutex_lock(&mt);
	pthread_create(&t, NULL, SetupGUIThreadProc, (void*)&tdata);
	pthread_detach(t);
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 1;
	if(pthread_cond_timedwait(&ct, &mt, &ts) == ETIMEDOUT)
	{
		pthread_cancel(t);
		tdata.didSucceed = 0;
	}
	pthread_mutex_unlock(&mt);
	pthread_cond_destroy(&ct);
	pthread_mutex_destroy(&mt);

	if(!tdata.didSucceed)
	{
		/*WSLogDebug("gtk_init_check failed");*/
#if GTK_CHECK_VERSION (4, 0, 0)
		WSAPP_HANDLE(pThis) = NULL;
#else
		pThis->mHasGUI = FALSE;
#endif
		return FALSE;
	}
#endif
#if GTK_CHECK_VERSION (4, 0, 0)
	WSAPP_HANDLE(pThis) =
		gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE);
	GError* error = NULL;
	if(!g_application_register(G_APPLICATION(WSAPP_HANDLE(pThis)),
			NULL, &error))
	{
		fprintf(stderr, "Unable To Initalize GtkApplcation:%s\n", error->message);
		g_error_free(error);
		return FALSE;
	}
#else
	pThis->mHasGUI = TRUE;
#endif
	/* done with gtk_init handling */
	return TRUE;
}
void WSApp_SetNotifyIconMenu(WSAPP* pThis, WSMENU* pMenu)
{
}
void WSApp_SetNotifyIcon(WSAPP* pThis, const char* abspath, size_t abspathlen)
{
}
void WSApp_SetMenuBar(WSAPP* pThis, WSMENUBAR* pMenuBar, WSMENUACTIONS* ma)
{
#if GTK_CHECK_VERSION (4, 0, 0)
	gtk_application_set_accels_for_menu(
		WSAPP_HANDLE(pThis), G_MENU_MODEL(WSMENUBAR_HANDLE(pMenuBar)));
	gtk_application_set_menubar(WSAPP_HANDLE(pThis),
		G_MENU_MODEL(WSMENUBAR_HANDLE(pMenuBar)));
	WSAppendActionMap(G_ACTION_MAP(WSAPP_HANDLE(pThis)),
		G_ACTION_MAP(WSMENUACTIONS_HANDLE(ma)));
#endif
}
void WSApp_Destroy(WSAPP* pThis)
{
#if GTK_CHECK_VERSION (4, 0, 0)
	 g_object_unref(G_OBJECT(WSAPP_HANDLE(pThis)));
#endif
}

/*-----------------------------------------------------------------------------
						GTK1/2/3/4 WSWINDOW common
-----------------------------------------------------------------------------*/
void* WSWindow_GetHandle(WSWINDOW* pThis)
{
	return WSWINDOW_HANDLE(pThis);
}
void WSWindow_SetCon(WSWINDOW* pThis, void* con)
{
	WSWINDOW_HANDLE(pThis)->mCon = con;
}
void* WSWindow_GetCon(WSWINDOW* pThis)
{
	return WSWINDOW_HANDLE(pThis)->mCon;
}

/*-----------------------------------------------------------------------------
						GTK1/2/3/4 WSSCROLLEDWINDOW common
-----------------------------------------------------------------------------*/
void WSScrolledWindow_Destroy(WSSCROLLEDWINDOW* pThis)
{
#if !GTK_CHECK_VERSION (4, 0, 0)
	/*puts("Destroying Scrolling Window...");*/
	gtk_widget_destroy(WSSCROLLEDWINDOW_WIDGET_HANDLE(pThis));
#endif
}
WSEVENT_CALLBACK(WSWindowDefaultCallback)
{
}


WSMENUEVENT_CALLBACK(WSMenuSectionDefaultCallback)
{
}

/*-----------------------------------------------------------------------------
						GTK1/2/3/4 WSWNDCLASS
-----------------------------------------------------------------------------*/
wsbool WSWndClass_Create(WSWNDCLASS* pThis, WSAPP* pApp, WSICON* pWindowIcon,
	LPWSWindowMessages messages, const char* wndclassname)
{
	pThis->mWindowIcon = pWindowIcon;
	pThis->mWindowMessages = messages;
	return TRUE;
}
void WSWndClass_Destroy(WSWNDCLASS* pThis, WSAPP* pApp)
{
}

/*-----------------------------------------------------------------------------
						GTK1/2/3/4 WSDC
-----------------------------------------------------------------------------*/
typedef struct WSDC_* WSDC;

#define WSDC_HANDLE(x)

void WSDC_Create(WSDC* pThis, WSWINDOW* pWindow)
{
	*(WSDC*)pThis = *(WSDC*)pWindow;
}
void WSDC_Destroy(WSDC* pThis, WSWINDOW* pWindow)
{
}
wsbool WSDC_IsWindowReady(WSDC* pThis, WSWINDOW* pWindow)
{
	return FALSE;
}
void WSDC_GetTextWidthAndHeight(WSDC* pThis,
	guipos_t* outwidth, guipos_t* outheight)
{
	GtkWidget* widget = WSWINDOW_WIDGET_HANDLE(pThis);
#if !GTK_CHECK_VERSION (4, 0, 0)
	GtkStyle* style =
		gtk_widget_get_style(widget);
#endif
	const char g = 'g'; /*H and G are used a lot too*/
	PangoContext* pc = gtk_widget_get_pango_context(widget);
	int width;
	guipos_t height;
	PangoFontMetrics* pm = pango_context_get_metrics(
		pc,
#if GTK_CHECK_VERSION (4, 0, 0)
		pango_context_get_font_description(pc),
#else
		style->font_desc,
#endif
		pango_context_get_language(pc));
	/* gtk_widget_create_pango_layout also... */
	PangoLayout* pl = pango_layout_new(pc);

	pango_layout_set_text(pl, &g, 1);
	pango_layout_get_pixel_size(pl, &width, NULL);
	g_object_unref(pl);

	height = PANGO_PIXELS(pango_font_metrics_get_descent (pm)
		+ pango_font_metrics_get_ascent (pm));
	pango_font_metrics_unref(pm);
	/*pango_font_metrics_get_approximate_char_width ??*/
	*outwidth = width;/*gdk_string_width(font, g);*/
	*outheight = height;/*gtk_style_get_font(style)->ascent+descent*/
}
void WSDC_DrawText(WSDC* pThis,
	const char* text, size_t len, WSRECT* r)
{
	/*
		In GTK4 you'd create a GtkDrawingArea and call
		gtk_drawing_area_set_draw_func
		void (*GtkDrawingAreaDrawFunc) (GtkDrawingArea* drawing_area,
			cairo_t* cr, int width, int height, gpointer user_data)

		GTK3 you'd
		gboolean draw_cb(GtkWidget* widget, cairo_t* cr, gpointer data)
		g_signal_connect( da, "draw", G_CALLBACK(draw_cb), NULL );
		gdk_drawing_context_get_cairo_context

		In GTK4 you can override GtkWidget's
		void snapshot(GtkWidget* widget, GtkSnapshot* snapshot)
		In GTK2/3
		gboolean draw(GtkWidget* widget, cairo_t* cr)
	*/
/*	gdk_window_create_similar_surface();*/
	PangoContext* pc
		= gtk_widget_get_pango_context(WSWINDOW_WIDGET_HANDLE(pThis));
	PangoFontDescription* pfd
		= pango_context_get_font_description(pc);
	PangoLayout* pl;
	#if GTK_CHECK_VERSION (4, 0, 0)
		GdkSurface* surface =
			gtk_native_get_surface(
				gtk_widget_get_native(WSWINDOW_WIDGET_HANDLE(pThis)));
		GdkCairoContext* gdkcairo =
			gdk_surface_create_cairo_context(surface);
		cairo_region_t* region = (cairo_region_t*)
			gdk_draw_context_get_frame_region(GDK_DRAW_CONTEXT(gdkcairo));
		cairo_t* cairo;
		gdk_draw_context_begin_frame(GDK_DRAW_CONTEXT(gdkcairo),
			region);
		cairo =
			gdk_cairo_context_cairo_create(gdkcairo);
		cairo_reset_clip(cairo);


	#elif GTK_CHECK_VERSION (3, 22, 0)
		cairo_region_t* region
			= gdk_window_get_visible_region(
				GDK_WINDOW(WSWINDOW_WINDOW_HANDLE(pThis)));
		GdkDrawingContext* context
			= gdk_window_begin_draw_frame(
				GDK_WINDOW(WSWINDOW_WINDOW_HANDLE(pThis)),
				region);
		cairo_t* cairo
			= gdk_drawing_context_get_cairo_context(context);
	#else
		cairo_t* cairo =
			gdk_cairo_create(GDK_WINDOW(WSWINDOW_WINDOW_HANDLE(pThis)));
	#endif
	#if GTK_CHECK_VERSION (4, 0, 0)
	#else
		if(cairo_version() < CAIRO_VERSION_ENCODE(1, 12, 0))
			gdk_cairo_set_source_window(cairo,
				GDK_WINDOW(WSWINDOW_WINDOW_HANDLE(pThis)),
				0, 0);
		else
		{
			cairo_push_group(cairo);
			gdk_cairo_set_source_window(cairo,
				GDK_WINDOW(WSWINDOW_WINDOW_HANDLE(pThis)),
				0, 0);
			cairo_paint(cairo);
			cairo_pop_group_to_source(cairo);
		}
	#endif
	pl = pango_cairo_create_layout (cairo);
	pango_layout_set_font_description(pl, pfd);
	pango_layout_set_text(pl, text, len);


 /*   if (isunderlined)
	{
		PangoAttrList* attrs = pango_attr_list_new();
		PangoAttribute* attr = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
		pango_attr_list_insert(attrs, attr);
		pango_layout_set_attributes(pl, attrs);
		pango_attr_list_unref(attrs);
	}*/

	cairo_move_to(cairo, r->left, r->top);
	pango_cairo_show_layout (cairo, pl);

	g_object_unref(pl);

	#if GTK_CHECK_VERSION (4, 0, 0)
		gdk_draw_context_end_frame(GDK_DRAW_CONTEXT(gdkcairo));
	#elif GTK_CHECK_VERSION (3, 22, 0)
		gdk_window_invalidate_region(GDK_WINDOW(WSWINDOW_WINDOW_HANDLE(pThis)),
				region, TRUE);
	#endif
	#if GTK_CHECK_VERSION (4, 0, 0)
		cairo_destroy(cairo);
		g_object_unref(gdkcairo);
	#elif GTK_CHECK_VERSION (3, 22, 0)
		if (context != 0)
			gdk_window_end_draw_frame(
				GDK_WINDOW(WSWINDOW_WINDOW_HANDLE(pThis)), context);
	#else
		cairo_destroy(cairo);
	#endif
}
void WSPaintDC_Create(WSPAINTDC* pThis, WSWINDOW* pWindow)
{
	*(WSPAINTDC*)pThis = *(WSPAINTDC*)pWindow;
}
void WSPaintDC_Destroy(WSPAINTDC* pThis, WSWINDOW* pWindow)
{
}

/*-----------------------------------------------------------------------------
						GTK1/2/3/4 TextCtrl Common
-----------------------------------------------------------------------------*/

void WSTextCtrl_Activate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_Deactivate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_Update(WSTEXTCTRL* pThis, WSWINDOW* pParent, WSRECT* rect)
{
}

void WSTextCtrl_SetSize(WSTEXTCTRL* pThis, guipos_t x, guipos_t y)
{
	gtk_widget_set_size_request(WSTEXTCTRL_WIDGET_HANDLE(pThis), x, y);
}

void WSTextCtrl_SetText(WSTEXTCTRL* pThis, const char* text, size_t textLen)
{
	GtkTextBuffer* buffer = gtk_text_view_get_buffer(WSTEXTCTRL_HANDLE(pThis));
	gtk_text_buffer_set_text(buffer, text, textLen);
}

void WSTextCtrl_Idle(WSTEXTCTRL* pThis)
{
}
void WSTextCtrl_Append(WSTEXTCTRL* pThis, const char* text, size_t textLen)
{
	GtkTextBuffer* buffer = gtk_text_view_get_buffer(WSTEXTCTRL_HANDLE(pThis));
	GtkTextIter it;
	gtk_text_buffer_get_end_iter(buffer, &it);
	gtk_text_buffer_insert(buffer, &it, text, textLen);
	/*gtk_text_view_scroll_to_mark(h, gtk_text_buffer_get_insert(buffer), 0);*/
}

void WSTextCtrl_HandlePaint(WSTEXTCTRL* pThis, WSPAINTEVENT* ev, WSPAINTDC* dc)
{
}
void WSTextCtrl_HandleKeyDown(WSTEXTCTRL* pThis, WSKEYEVENT* ev)
{
}
void WSTextCtrl_HandleContentEvent(WSTEXTCTRL* pThis,
	WSCONTENTEVENT* ev)
{
}

void WSTextCtrl_HandleGrowEvent(WSTEXTCTRL* pThis,
	WSGROWEVENT* ev)
{
}

void WSTextCtrl_HandleZoomEvent(WSTEXTCTRL* pThis,
	WSZOOMEVENT* ev)
{
}

void WSTextCtrl_HandleKeyEvent(WSTEXTCTRL* pThis, WSKEYEVENT* ev)
{
}

void WSTextCtrl_HandleIdle(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_HandleActivate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_HandleDeactivate(WSTEXTCTRL* pThis)
{
}

void WSTextCtrl_HandleIBeamCursor(WSTEXTCTRL* pThis,
	WSIBEAMEVENT* er)
{
}

void WSTextCtrl_HandleMouseWheel(WSTEXTCTRL* pThis,
	WSMOUSEWHEELEVENT* er)
{
}

void* WSTextCtrl_GetHandle(WSTEXTCTRL* pThis)
{
	return WSTEXTCTRL_HANDLE(pThis);
}

/*-----------------------------------------------------------------------------
								WSLaunchURL()
-----------------------------------------------------------------------------*/
wsbool WSLaunchURL(const char* url, size_t urllen)
{
	if(!g_app_info_launch_default_for_uri(url, NULL, NULL))
	{
		int res;
		size_t realurllen = urllen == WSMAXLENGTH ? strlen(url) : urllen;
		char* s = WSALLOC(char, strcountof("x-www-browser ") + realurllen + 1);
		if(!s)
			return FALSE;
		WSMemoryCopy(&s[0], "x-www-browser ",
			strcountof("x-www-browser ") * sizeof(char));
		WSMemoryCopy(&s[strcountof("x-www-browser ")], url,
			realurllen * sizeof(char));
		s[strcountof("x-www-browser ") + realurllen] = '\0';
		res = system(s);
		if(res == 32512 || res == -1)
		{
			WSMemoryCopy(&s[0], "xdg-open ",
				strcountof("xdg-open ") * sizeof(char));
			WSMemoryCopy(&s[strcountof("xdg-open ")], url,
				realurllen * sizeof(char));
			s[strcountof("xdg-open ") + realurllen] = '\0';
			system(s);
		}
		WSFREE(s);
		#if 0
		WSMUTSTR s;
		WSMutStr_Create(&s);
		WSMutStr_AppendCStringLiteral(&s, "x-www-browser ");
		WSMutStr_AppendCStringWithLength(&s, url, urllen);
		int res = system(WSMutStr_GetCString(&s));
		fprintf(stderr, "system res [%d]\n", res);
		if(res == 32512 || res == -1)
		{
			WSMutStr_AssignCStringWithLength(&s,
				"xdg-open ", strcountof("xdg-open "));
			WSMutStr_AppendCStringWithLength(&s, url, urllen);
			system(WSMutStr_GetCString(&s));
		}
		WSMutStr_Destroy(&s);
		#endif
#if 0
		/* Ancient method */
		/* https://ubuntuforums.org/archive/index.php/t-640657.html */
		/* gconftool-2 -g /desktop/gnome/url-handlers/http/command */
		GConfClient* client = gconf_client_get_default();
		gchar* browser =
				gconf_client_get_string(client,
					"/desktop/gnome/url-handlers/http/command", NULL);
		/* Launch the browser */
		if (browser)
		{
			gchar cmdline[5000];
			g_snprintf(cmdline, 5000, browser, url);
			GError* error;
			g_spawn_command_line_async(cmdline, &error);
		}
#endif
	}
	return TRUE;
}

/*-----------------------------------------------------------------------------*/
/*	main()*/
/*-----------------------------------------------------------------------------*/
#if !defined(__MACH__) || (!defined(TARGET_OS_IOS) || !TARGET_OS_IOS)
 int main(int argc, char* argv[])
{ return WSApp_BasicMain(argc, argv); }
#endif

#endif /* OS switch */
#if !defined(__MUTSTR_H__) && !defined(WIKISERVER_VERSION)
	#undef CStrLength
	#undef CStrLocateChar
	#undef CStrFind
	#undef CStrrfind
#endif
#if !defined(WSMACCLASSICORCARBON)
	#define WSMenu_Create(pThis, pSection, title) WSMenu_Create(pThis, pSection)
#endif
#if !defined(_WIN32)
	#define WSTextCtrl_CreateScrolled(pThis, pParent, pTopParent, rect, wc) \
		WSTextCtrl_CreateScrolled(pThis, pParent, pTopParent, rect)
#endif
#if defined(WSMACCLASSICORCARBON) \
	|| defined(__MACH__) \
	|| (!defined(__MACH__) && !defined(macintosh) \
			&& !defined(_WIN32) && defined(WSKDEOPT))
#define WSWndClass_Create(pThis, pApp, pWindowIcon, messages, wndclassname) \
	WSWndClass_Create(pThis, pApp, pWindowIcon, wndclassname)
#endif
#endif /* GUI_IMPLEMENTATION */
