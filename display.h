/**
*	@file display.h
*
*	#     #  #####  ######  ###  #####  ######  #          #    #     #
*	#  #  # #     # #     #  #  #     # #     # #         # #    #   #
*	#  #  # #       #     #  #  #       #     # #        #   #    # #
*	#  #  #  #####  #     #  #   #####  ######  #       #     #    #
*	#  #  #       # #     #  #        # #       #       #######    #
*	#  #  # #     # #     #  #  #     # #       #       #     #    #
*	 ## ##   #####  ######  ###  #####  #       ####### #     #    #
*
*	Single header file for cross-platform monitor API abstraction.
*
*	###Instructions###
*
*	Define WSDISPLAY_IMPLEMENTATION in one source file for the implementation
*
*	###Example###
*	WSDISPLAYITERATOR it;
*	WSDisplayBegin(&it);
*	for(; !WSDisplayIsEnd(&it); WSDisplayIterator_Iterate(&it))
*	{
*		WSVIDEOMODE curmode;
*		WSDISPLAY dis;
*		WSDisplayIterator_Value(&dis);
*		WSDisplay_GetCurrentMode(&disp, &curmode);
*		WSDisplay_Destroy(&dis);
*	}
*	WSDisplayIterator_Destroy(&it);
*
*	###Options###
*	- X11 uses XF86VidMode and Xinerama by default for compatability.
*	-- To use the newer Xrandr instead, specify HAVE_XRANDR
*
*	###Supported targets###
*	- Win32
*	- Mac Classic/Carbon (Display Manager)
*	- iOS (UIKit)
*	- Mac OSX (Quartz)
*	- X11 (Xinerama, XF86 for video mode setting)
*	- X11 (Xrandr)
*	- AMIGA (GfxBase)
*	- X68000 (_iocs_crtmod)
*
*	###Supports###
*	- Pretty much everything I can think of
*
*	###TODO###
*	- There's a bit of confusion between Depth and Bits Per Pixel
*	-- They are slightly different things but often interchanged
*	-- I don't handle them correctly outside of XXXGetDepth()
*	- WSDisplayGetFromWindow is currently Win32 only
*
*	###NOTES###
*	- ASCII art, mostly "Banner", from
*	-- budavariam.github.io/asciiart-text/multi
*	- Wayland doesn't have a way to change video modes yet, and
*	may never have a way directly do so for a while.
*
*	@version 0.2
*
*	Version History
*
*	0.2		WSMSIXOPT for MSIX apps.
*				- Don't load shcore.dll functions if using WSMSIXOPT only as
*					MSIX supports Windows 7 and shcore.dll is 8.1
*				- Rename XXX_Destruct to XXX_Destroy
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
#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#ifdef __cplusplus
	extern "C" {
#endif
#if !defined(WSBOOL_DEFINED)
	typedef int wsbool;
	#if !defined(TRUE)
		#define TRUE 1
		#define FALSE 0
	#endif
	#define WSBOOL_DEFINED
#endif
#if !defined(WSALLOC)
	#define WSALLOC(type, count) (type*)malloc(sizeof(type) * (count))
	#define WSALLOC1(x) (x*)malloc(sizeof(x))
	#define WSREALLOC(old, type, count) \
		(type*)realloc(old, sizeof(type) * (count))
	#define	WSFREE(x) free((void*)(x))
#endif /* WSALLOC */
/*
	Ideal floating point type. Matches CoreFoundation.
*/
#ifndef wsfloat
	#if defined(__LP64__) && __LP64__
	#define wsfloat double /* DBL_MIN/MAX */
	#else
	#define wsfloat float /* FLT_MIN/MAX */
	#endif
#endif /* wsfloat */
#if defined(_WIN32)
	typedef struct HMONITOR__* WSDISPLAYHANDLE;
	#if defined(WSMSIXOPT)
		#ifndef WSDISPLAY_NOWINXP
			#define WSDISPLAY_NOWINXP
		#endif
		#ifndef WSDISPLAY_STATICGDI32
			#define WSDISPLAY_STATICGDI32
		#endif
		#ifndef WSDISPLAY_STATICSHCORE
			#define WSDISPLAY_STATICSHCORE
		#endif
		#ifndef WSDISPLAY_STATICADVAPI32
			#define WSDISPLAY_STATICADVAPI32
		#endif
	#endif
	#if defined(WSDISPLAY_NOWIN8)
		#ifndef WSDISPLAY_NOWINXP
			#define WSDISPLAY_NOWINXP
		#endif
	#endif
	#if defined(WSDISPLAY_NOWINXP)
		#ifndef WSDISPLAY_NOWIN95
			#define WSDISPLAY_NOWIN95
		#endif
	#endif
	#if defined(WSDISPLAY_NOWIN95)
		#ifndef WSDISPLAY_NOWINNT31
			#define WSDISPLAY_NOWINNT31
		#endif
	#endif
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
#elif defined(__human68k__)
	typedef int WSDISPLAYHANDLE;
#elif defined(__amigaos__)
	typedef struct Screen* WSDISPLAYHANDLE; /* APTR->DisplayInfoHandle */
#elif defined(macintosh)
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
	typedef struct GDevice** WSDISPLAYHANDLE;
#elif defined(__MACH__)
	#if defined(__has_builtin) && __has_builtin(__is_target_arch) \
		&& __has_builtin(__is_target_vendor) && __has_builtin(__is_target_os) \
		&& __has_builtin(__is_target_environment) && __is_target_arch(x86_64) \
		&& __is_target_vendor(apple) && __is_target_os(ios)
		#define WSDISPLAYIOS
	#endif
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
	typedef unsigned int WSDISPLAYHANDLE;
#else
	typedef struct _XDisplay* WSDISPLAYHANDLE;
#endif

#if !defined(guipos_t)
	#define guipos_t int
	typedef struct WSRECT
	{
		int left;
		int top;
		int right;
		int bottom;
	} WSRECT;
#endif
#if !defined(WSRect_GetX)
	#if defined(__MACH__)
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
		#define WSRect_GetWidth(pThis) (pThis)->right
		#define WSRect_GetHeight(pThis) (pThis)->bottom
		#define WSRect_SetX(pThis, x) (pThis)->left = x
		#define WSRect_SetY(pThis, x) (pThis)->top = x
		#define WSRect_SetWidth(pThis, x) (pThis)->right = x
		#define WSRect_SetHeight(pThis, x) (pThis)->bottom = x
	#endif
#endif
#include <stddef.h> /* size_t */

#define WSVideoMode_ConstructDefault(pThis) \
{ \
	(pThis)->width = (pThis)->height = 0; \
}
#define WSVideoMode_Construct(pThis, inwidth, inheight, indepth, infreq) \
{ \
	(pThis)->width = inwidth; (pThis)->height = inheight; \
	(pThis)->depth = indepth; (pThis)->refreshrate = infreq; \
}
#define WSVideoMode_GetWidth(pThis) (pThis)->width
#define WSVideoMode_GetHeight(pThis) (pThis)->height
#define WSVideoMode_GetDepth(pThis) (pThis)->depth
#define WSVideoMode_GetRefreshRate(pThis) (pThis)->refreshrate
#define WSVideoMode_IsEqual(pThis, pOther) \
	((pThis)->width == (pOther)->width && (pThis)->height == (pOther)->height \
		&& (pThis)->depth == (pOther)->depth \
		&& (pThis)->refreshrate == (pOther)->refreshrate)
#define WSVideoMode_IsDefaultNonSafe(pThis) \
	((pThis)->width == 0 && (pThis)->height == 0)
#define WSVideoMode_IsDefault(pThis) \
	(!(pThis) || WSVideoMode_IsDefaultNonSafe(pThis))
#define WSVideoMode_MatchesNonSafe(pThis, pOther) \
	((!(pOther)->width || (pThis)->width == (pOther)->width) \
		&& (!(pOther)->height || (pThis)->height == (pOther)->height) \
		&& (!(pOther)->depth || (pThis)->depth == (pOther)->depth) \
		&& (!(pOther)->refreshrate \
				|| (pThis)->refreshrate == (pOther)->refreshrate))
#define WSVideoMode_Matches(pThis, pOther) \
	(!(pOther) || WSVideoMode_MatchesNonSafe(pThis, pOther))
typedef struct WSVIDEOMODE
{ /* 0 on any member means unspecified/"normal" depending on OS */
#if defined(__cplusplus)
	WSVIDEOMODE(){}
	WSVIDEOMODE(int width, int height = 0, int depth = 0, int freq = 0)
	{ WSVideoMode_Construct(this, width, height, depth, freq); }
	bool operator==(const WSVIDEOMODE& other) const
	{ return WSVideoMode_IsEqual(this, &other); }
	bool operator!=(const WSVIDEOMODE& mode) const
	{ return !operator==(mode); }
	bool Matches(const WSVIDEOMODE& other) const
	{ return WSVideoMode_MatchesNonSafe(this, &other); }
	int GetWidth() const { return WSVideoMode_GetWidth(this); }
	int GetHeight() const { return WSVideoMode_GetHeight(this); }
	int GetDepth() const { return WSVideoMode_GetDepth(this); }
	int GetRefreshRate() const { return WSVideoMode_GetRefreshRate(this); }
#endif
	int width, height;
	int depth; /* bits per pixel */
	int refreshrate; /* refresh rate in hertz (Hz) */
} WSVIDEOMODE;

typedef struct WSDISPLAY WSDISPLAY;
typedef struct WSDISPLAYITERATOR WSDISPLAYITERATOR;
void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis);
void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay);
void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis);

void WSDisplayGetPrimary(WSDISPLAY* pDisplay);
void WSDisplayBegin(WSDISPLAYITERATOR* pIt);
wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt);
wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay);
wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay);
/* WSDisplayGetFromWindow is currently Win32 only */
wsbool WSDisplayGetFromWindow(void* windowhandle, WSDISPLAY* pDisplay);
void WSDisplayGetGeometry(WSRECT* r);
void WSDisplayGetClientArea(WSRECT* r);
void WSDisplayGetDepth(guipos_t* depth);
void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r);
void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r);
wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis);
void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode);
wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode);
void WSDisplay_ResetMode(const WSDISPLAY* pThis); /* default video mode */
void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode,
	WSVIDEOMODE** modes, unsigned int* modescount);
void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y);
void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale);
void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth);
size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize);
int WSDisplay_MakePrimary(const WSDISPLAY* pThis);
void WSDisplay_Destroy(WSDISPLAY* pThis);
#if defined(macintosh) || defined(__MACH__)
	#define WSDisplayGetStdPPIValue(val) (*val = 72)
#else
	#define WSDisplayGetStdPPIValue(val) (*val = 96)
#endif
#if !defined(_WIN32)
	#define WSDisplaySetDPIAware()
#else
	void WSDisplaySetDPIAware();
#endif
#if defined(_WIN32)
typedef struct HDC__ HDC__;
typedef struct HMONITOR__ HMONITOR__;
typedef struct HWND__ HWND__;
typedef struct tagMONITORINFO tagMONITORINFO;
typedef struct tagRECT tagRECT;
typedef struct tagPOINT tagPOINT;
typedef struct _devicemodeA _devicemodeA;
#if defined(_WIN64)
	typedef wsbool (__stdcall* EDMEnumProc)(HMONITOR__*,
		HDC__*, tagRECT*, long long);
	typedef wsbool (__stdcall*
		EnumDisplayMonitorsProc)(HDC__* hdc,
		const WSRECT* r, EDMEnumProc enumproc, long long lParam);
#else
	typedef wsbool (__stdcall* EDMEnumProc)(HMONITOR__*,
		HDC__*, tagRECT*, long);
	typedef wsbool (__stdcall*
		EnumDisplayMonitorsProc)(HDC__* hdc,
		const WSRECT* r, EDMEnumProc enumproc, long lParam);
#endif
typedef HMONITOR__* (__stdcall*
	MonitorFromPointProc)(tagPOINT pt, unsigned long dwFlags);
typedef HMONITOR__* (__stdcall*
	MonitorFromRectProc)(const WSRECT* r, unsigned long dwFlags);
typedef HMONITOR__* (__stdcall*
	MonitorFromWindowProc)(HWND__* hwnd, unsigned long dwFlags);
typedef wsbool (__stdcall*
	GetMonitorInfoAProc)(HMONITOR__* hMonitor, tagMONITORINFO* lpmi);
typedef long (__stdcall* ChangeDisplaySettingsAProc)(
	_devicemodeA* lpDevMode, unsigned long dwflags);
typedef long (__stdcall*
	ChangeDisplaySettingsExAProc)(const char* lpszDeviceName,
	_devicemodeA* lpDevMode, HWND__* hwnd,
	unsigned long dwFlags, void* lParam);
typedef wsbool (__stdcall* EnumDisplaySettingsAProc)(const char* devname,
	unsigned long modenum, _devicemodeA* lpDevMode);
typedef int (__stdcall* GetDeviceCapsProc)(HDC__* hdc, int index);
typedef long (__stdcall* GetDpiForMonitorProc)(
	struct HMONITOR__*, int, unsigned int*, unsigned int*);
typedef struct HDC__* (__stdcall* CreateDCAProc)(
	const char* lpszDriver, const char* lpszDevice,
	const char* lpszOutput, const _devicemodeA* lpInitData);
typedef int (__stdcall* DeleteDCProc)(HDC__* hdc);
#endif

typedef struct WSDISPLAYITERATOR
{
#if defined(__cplusplus)
	void Iterate()
	{ WSDisplayIterator_Iterate(this); }
	void Value(WSDISPLAY* pDisplay) const
	{ WSDisplayIterator_Value(this, pDisplay); }
	void Destroy()
	{ WSDisplayIterator_Destroy(this); }
#endif
#if defined(_WIN32)
	WSDISPLAYHANDLE* displays;
	unsigned int index;
	unsigned int count;
	#if !defined(WSDISPLAY_NOWIN95)
		struct HINSTANCE__* mDisplayDLL;
	#endif /* !WSDISPLAY_NOWIN95 */
#elif defined(WSDISPLAYIOS)
	unsigned int index;
#elif defined(__amigaos__) || defined(__human68k__)
	unsigned int id;
#elif defined(macintosh)
	WSDISPLAYHANDLE display;
#elif defined(__MACH__)
	WSDISPLAYHANDLE* displays;
	unsigned int index;
	unsigned int count;
#elif defined(HAVE_XRANDR)
	WSDISPLAYHANDLE mDisplay;
	int mNumScreens;
	int mIndex;
	struct _XRRScreenResources* mScreenResources;
	struct _XRROutputInfo** mOutputInfos;
	struct _XRRCrtcInfo** mCrtcInfos;
#else
	WSDISPLAYHANDLE mDisplay;
	struct _XineramaScreenInfo* mScreenInfos;
	int mNumScreens;
	int mIndex;
#endif
} WSDISPLAYITERATOR;

typedef struct WSDISPLAY
{
#if defined(__cplusplus)
	static void GetPrimary(WSDISPLAY* pDisplay)
	{ WSDisplayGetPrimary(pDisplay); }
	static void Begin(WSDISPLAYITERATOR* pIt) { WSDisplayBegin(pIt); }
	static wsbool IsEnd(WSDISPLAYITERATOR* pIt)
	{ return WSDisplayIsEnd(pIt); }
	static wsbool GetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
	{ return WSDisplayGetFromPoint(x, y, pDisplay); }
	static wsbool GetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
	{ return WSDisplayGetFromRect(r, pDisplay); }
	static wsbool GetFromWindow(void* windowhandle, WSDISPLAY* pDisplay)
	{ return WSDisplayGetFromWindow(windowhandle, pDisplay); }
	static void GetPrimaryGeometry(WSRECT* r)
	{ WSDisplayGetGeometry(r); }
	static void GetPrimaryClientArea(WSRECT* r)
	{ WSDisplayGetClientArea(r); }
	static void GetPrimaryDepth(guipos_t* depth)
	{ WSDisplayGetDepth(depth); }
	void GetGeometry(WSRECT* r) const
	{ WSDisplay_GetGeometry(this, r); }
	void GetClientArea(WSRECT* r) const
	{ WSDisplay_GetClientArea(this, r); }
	wsbool IsPrimary() const
	{ return WSDisplay_IsPrimary(this); }
	void GetCurrentMode(WSVIDEOMODE* mode) const
	{ WSDisplay_GetCurrentMode(this, mode); }
	wsbool ChangeMode(WSVIDEOMODE* mode) const
	{ return WSDisplay_ChangeMode(this, mode); }
	void ResetMode() const { WSDisplay_ResetMode(this); }
	void GetModes(const WSVIDEOMODE* matchingmode,
		WSVIDEOMODE** modes, unsigned int* modescount) const
	{ WSDisplay_GetModes(this, matchingmode, modes, modescount); }
	void GetPPI(guipos_t* x, guipos_t* y)
	{ WSDisplay_GetPPI(this, x, y); }
	void GetScaleFactor(float* scale)
	{ WSDisplay_GetScaleFactor(this, scale); }
	void GetDepth(guipos_t* depth) const
	{ WSDisplay_GetDepth(this, depth); }
	size_t GetName(char* buffer, size_t buffersize) const
	{ return WSDisplay_GetName(this, buffer, buffersize); }
	int MakePrimary() const
	{ return WSDisplay_MakePrimary(this); }
	void Destroy()
	{ WSDisplay_Destroy(this); }
	static void SetDPIAware()
	{ WSDisplaySetDPIAware(); }
#endif
	WSDISPLAYHANDLE mHandle;
#if defined(_WIN32)
	#if !defined(WSDISPLAY_NOWIN95)
		struct HINSTANCE__* mDisplayDLL;
		GetMonitorInfoAProc mpfnGetMonitorInfoA;
		#if !defined(WSDISPLAY_NOWINNT31)
			ChangeDisplaySettingsAProc mpfnChangeDisplaySettingsA;
		#endif
		ChangeDisplaySettingsExAProc mpfnChangeDisplaySettingsExA;
		EnumDisplayMonitorsProc mpfnEnumDisplayMonitors;
		EnumDisplaySettingsAProc mpfnEnumDisplaySettingsA;
	#endif /* !WSDISPLAY_NOWIN95 */
#elif defined(__human68k__)
	WSVIDEOMODE mModes[28];
#elif defined(__amigaos__)
	wsbool mIsOpen;
#elif !defined(macintosh) && !defined(__MACH__)
	#if defined(HAVE_XRANDR)
		unsigned long mCrtc;
		unsigned long mOutput;
	#endif
	int mScreenNum;
#endif
} WSDISPLAY;
#define WSDisplay_IsCreated(pThis) ((pThis)->mHandle != NULL)
#ifdef __cplusplus
	} /* end extern "C" */
#endif
#endif /* __DISPLAY_H__ */
#if defined(WSDISPLAY_IMPLEMENTATION)
/*=============================================================================
		 #     # ### #     # ######  ####### #     #  #####
		 #  #  #  #  ##    # #     # #     # #  #  # #     #
		 #  #  #  #  # #   # #     # #     # #  #  # #
		 #  #  #  #  #  #  # #     # #     # #  #  #  #####
		 #  #  #  #  #   # # #     # #     # #  #  #       #
		 #  #  #  #  #    ## #     # #     # #  #  # #     #
		  ## ##  ### #     # ######  #######  ## ##   #####

			 #     #  #####  ####### ######   #####   #####
			 #     # #     # #       #     # #     # #     #
			 #     # #       #       #     #       #       #
			 #     #  #####  #####   ######   #####   #####
			 #     #       # #       #   #         # #
			 #     # #     # #       #    #  #     # #
			  #####   #####  ####### #     #  #####  #######

	Unless WSDISPLAY_NOWIN95 is specified a lot of stuff is dynloaded.
	If you want Win95 but don't want NT 3.1 define WSDISPLAY_NOWINNT31.
	Define WSDISPLAY_NOWIN8 to remove compat for all pre 8.1 OSes.
	Define WSDISPLAY_NOWINXP to remove compat for all pre XP OSes.
	Define WSDISPLAY_STATICGDI32 to remove dyncalls to GDI32.
	Define WSDISPLAY_STATICSHCORE to remove dyncalls to SHCORE.
	Calling GetPPI with return 96/96 and any geometry call will give
		((RECT) / ppi) as dimensions if the process is not set as
		DPI aware on Windows!
=============================================================================*/
#if defined(_WIN32)
#include <windows.h>
#ifdef __cplusplus
	extern "C" {
#endif
#if defined(WSDISPLAY_NOWIN95)
	#define WSDISPLAYWIN95CALL(x) x
#else
	#define WSDISPLAYWIN95CALL(x) (*(pThis)->mpfn##x)
#endif /* WSDISPLAY_NOWIN95 */
#if defined(WSDISPLAY_NOWINNT31)
	#define WSDISPLAYWINNT31CALL(x) x
#else
	#define WSDISPLAYWINNT31CALL(x) (*(pThis)->mpfn##x)
#endif /* WSDISPLAY_NOWINNT31 */
#if defined(_WIN32_WCE)
	#define WIN32DISPLAYDLL "coredll.dll"
#else
	#define WIN32DISPLAYDLL "user32.dll"
	/* Indirectly check for WINVER >= 0x0500 as HMONITOR and monitor info
		defines are not present in earlier WinSDK headers */
	#if !defined(HMONITOR_DECLARED) && !defined(MNS_NOCHECK)
		DECLARE_HANDLE(HMONITOR);
		typedef BOOL(CALLBACK * MONITORENUMPROC )(
			HMONITOR, HDC, LPRECT, LPARAM);
		typedef struct tagMONITORINFO
		{
			DWORD   cbSize;
			RECT    rcMonitor;
			RECT    rcWork;
			DWORD   dwFlags;
		} MONITORINFO, *LPMONITORINFO;
		#if defined(__cplusplus)
			typedef struct tagMONITORINFOEXA : public tagMONITORINFO
			{
				char       szDevice[CCHDEVICENAME];
			} MONITORINFOEXA, *LPMONITORINFOEXA;
		#else
			typedef struct tagMONITORINFOEXA
			{
				MONITORINFO;
				char       szDevice[CCHDEVICENAME];
			} MONITORINFOEXA, *LPMONITORINFOEXA;
		#endif
		#define MONITOR_DEFAULTTONULL       0x00000000
		#define MONITOR_DEFAULTTOPRIMARY    0x00000001
		#define MONITOR_DEFAULTTONEAREST    0x00000002
		#define MONITORINFOF_PRIMARY        0x00000001
		#define HMONITOR_DECLARED
	#endif
#endif
#if !defined(CDS_FULLSCREEN)
	#define CDS_FULLSCREEN 4
#endif
#if !defined(DM_POSITION)
	#define DM_POSITION             0x00000020
#endif

void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis)
{
	++pThis->index;
}
void WSDisplay_FillFields_(WSDISPLAY* pDisplay)
{
#if !defined(WSDISPLAY_NOWIN95)
	pDisplay->mDisplayDLL = LoadLibraryA(WIN32DISPLAYDLL);
	if(!pDisplay->mDisplayDLL)
	{
		#if !defined(WSDISPLAY_NOWINNT31)
			pDisplay->mpfnChangeDisplaySettingsA = NULL;
		#endif /* !WSDISPLAY_NOWINNT31 */
		pDisplay->mpfnGetMonitorInfoA = NULL;
		pDisplay->mpfnEnumDisplayMonitors = NULL;
		pDisplay->mpfnChangeDisplaySettingsExA = NULL;
		pDisplay->mpfnEnumDisplaySettingsA = NULL;
		return;
	}
	pDisplay->mpfnGetMonitorInfoA = (GetMonitorInfoAProc)
			GetProcAddress(pDisplay->mDisplayDLL, "GetMonitorInfoA");
	pDisplay->mpfnEnumDisplayMonitors = (EnumDisplayMonitorsProc)
			GetProcAddress(pDisplay->mDisplayDLL, "EnumDisplayMonitors");
	pDisplay->mpfnEnumDisplaySettingsA = (EnumDisplaySettingsAProc)
			GetProcAddress(pDisplay->mDisplayDLL, "EnumDisplaySettingsA");
	pDisplay->mpfnChangeDisplaySettingsExA = (ChangeDisplaySettingsExAProc)
			GetProcAddress(pDisplay->mDisplayDLL, "ChangeDisplaySettingsExA");
	#if !defined(WSDISPLAY_NOWINNT31)
		if(!pDisplay->mpfnChangeDisplaySettingsExA)
			pDisplay->mpfnChangeDisplaySettingsA = (ChangeDisplaySettingsAProc)
				GetProcAddress(pDisplay->mDisplayDLL,
						"ChangeDisplaySettingsA");
	#endif /* !WSDISPLAY_NOWINNT31 */
#endif /* !WSDISPLAY_NOWIN95 */
}
void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay)
{
	pDisplay->mHandle = pThis->displays[pThis->index];
	WSDisplay_FillFields_(pDisplay);
}
void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis)
{
	HeapFree(GetProcessHeap(), 0, pThis->displays);
	#if !defined(WSDISPLAY_NOWIN95)
		FreeLibrary(pThis->mDisplayDLL);
	#endif /* !WSDISPLAY_NOWIN95 */
}
void WSDisplay_Destroy(WSDISPLAY* pThis)
{
	#if !defined(WSDISPLAY_NOWIN95)
		FreeLibrary(pThis->mDisplayDLL);
	#endif /* !WSDISPLAY_NOWIN95 */
}

wsbool __stdcall WSDisplayIteratorEnumMonitors(
  struct HMONITOR__* hMonitor,
  struct HDC__* hdcMonitor,
  RECT* lprcMonitor,
  LONG_PTR lparamData
)
{
	WSDISPLAYITERATOR* pThis = (WSDISPLAYITERATOR*)lparamData;
	HMONITOR* newmonitors;
	(void)(hdcMonitor);

	if(pThis->displays) /* HeapReAlloc fails if NULL is passed in */
		newmonitors = (HMONITOR*)HeapReAlloc(GetProcessHeap(), 0,
			pThis->displays, sizeof(HMONITOR) * (pThis->count + 1));
	else
		newmonitors = (HMONITOR*)HeapAlloc(GetProcessHeap(), 0,
			sizeof(HMONITOR));
	if(newmonitors)
	{
		pThis->displays = newmonitors;
		pThis->displays[pThis->count] = hMonitor;
		pThis->count += 1;
	}
	return TRUE; /* continue */
}
wsbool WSDisplayGetFromWindowWin32(void* hWindow, WSDISPLAY* pDisplay, int flg)
{
	#if !defined(WSDISPLAY_NOWIN95)
		MonitorFromWindowProc pfnMonitorFromWindow;
		HINSTANCE hDisplayDLL;
		hDisplayDLL = LoadLibraryA(WIN32DISPLAYDLL);
		pfnMonitorFromWindow = (MonitorFromWindowProc)
			GetProcAddress(hDisplayDLL, "MonitorFromWindow");
		if(!pfnMonitorFromWindow)
			pDisplay->mHandle = NULL;
		else
			pDisplay->mHandle = (*pfnMonitorFromWindow)((HWND)hWindow, flg);
		FreeLibrary(hDisplayDLL);
	#else /* !WSDISPLAY_NOWIN95 */
		pDisplay->mHandle = MonitorFromWindow((HWND)hWindow, flg);
	#endif /* !WSDISPLAY_NOWIN95 */
	if(pDisplay->mHandle)
	{
		WSDisplay_FillFields_(pDisplay);
		return TRUE;
	}
	return FALSE;
}
void WSDisplayGetPrimary(WSDISPLAY* pDisplay)
{
	if(!WSDisplayGetFromWindowWin32(GetDesktopWindow(),
			pDisplay, MONITOR_DEFAULTTOPRIMARY))
		pDisplay->mHandle = NULL;
}
void WSDisplayBegin(WSDISPLAYITERATOR* pIt)
{
	wsbool success;
	#if !defined(WSDISPLAY_NOWIN95)
		EnumDisplayMonitorsProc pfnEnumDisplayMonitors;
	#endif
	pIt->index = 0;
	pIt->count = 0;
	#if !defined(WSDISPLAY_NOWIN95)
		pIt->mDisplayDLL = LoadLibraryA(WIN32DISPLAYDLL);
		if(!pIt->mDisplayDLL)
		{
			pIt->displays = NULL;
			return;
		}
		pfnEnumDisplayMonitors = (EnumDisplayMonitorsProc)
			GetProcAddress(pIt->mDisplayDLL, "EnumDisplayMonitors");
		success = pfnEnumDisplayMonitors
			&& (*pfnEnumDisplayMonitors)(NULL, NULL,
					WSDisplayIteratorEnumMonitors, (LONG_PTR)pIt);
	#else /* !WSDISPLAY_NOWIN95 */
		success = EnumDisplayMonitors(NULL, NULL,
					WSDisplayIteratorEnumMonitors, (LONG_PTR)pIt);
	#endif /* !WSDISPLAY_NOWIN95 */
	if(!success)
	{
		pIt->count = 0;
		pIt->displays = NULL;
		return;
	}
}
wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt)
{
	return pIt->index >= pIt->count;
}
wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
{
	POINT point;
	#if !defined(WSDISPLAY_NOWIN95)
		MonitorFromPointProc pfnMonitorFromPoint;
		HINSTANCE hDisplayDLL;
		point.x = x;
		point.y = y;
		hDisplayDLL = LoadLibraryA(WIN32DISPLAYDLL);
		pfnMonitorFromPoint = (MonitorFromPointProc)
			GetProcAddress(hDisplayDLL, "MonitorFromPoint");
		if(!pfnMonitorFromPoint)
			pDisplay->mHandle = NULL;
		else
			pDisplay->mHandle = (*pfnMonitorFromPoint)(point,
				MONITOR_DEFAULTTONULL);
		FreeLibrary(hDisplayDLL);
	#else /* !WSDISPLAY_NOWIN95 */
		point.x = x;
		point.y = y;
		pDisplay->mHandle = MonitorFromPoint(point,
			MONITOR_DEFAULTTONULL);
	#endif /* !WSDISPLAY_NOWIN95 */
	if(pDisplay->mHandle)
	{
		WSDisplay_FillFields_(pDisplay);
		return TRUE;
	}
	return FALSE;
}
wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
{
	#if !defined(WSDISPLAY_NOWIN95)
		MonitorFromRectProc pfnMonitorFromRect;
		HINSTANCE hDisplayDLL;
		hDisplayDLL = LoadLibraryA(WIN32DISPLAYDLL);
		pfnMonitorFromRect = (MonitorFromRectProc)
			GetProcAddress(hDisplayDLL, "MonitorFromRect");
		if(!pfnMonitorFromRect)
			pDisplay->mHandle = NULL;
		else
			pDisplay->mHandle = (*pfnMonitorFromRect)(r,
				MONITOR_DEFAULTTONULL);
		FreeLibrary(hDisplayDLL);
	#else /* !WSDISPLAY_NOWIN95 */
		pDisplay->mHandle = MonitorFromRect((RECT*)r,
			MONITOR_DEFAULTTONULL);
	#endif /* !WSDISPLAY_NOWIN95 */
	if(pDisplay->mHandle)
	{
		WSDisplay_FillFields_(pDisplay);
		return TRUE;
	}
	return FALSE;
}
wsbool WSDisplayGetFromWindow(void* hWindow, WSDISPLAY* pDisplay)
{
	return WSDisplayGetFromWindowWin32(hWindow, pDisplay, MONITOR_DEFAULTTONULL);
}

void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r)
{
	MONITORINFO mi;
	#if !defined(WSDISPLAY_NOWIN95)
		if(!pThis->mpfnGetMonitorInfoA)
		{
			r->top = r->left = r->bottom = r->right = 0;
			return;
		}
	#endif /* !WSDISPLAY_NOWIN95 */
	mi.cbSize = sizeof(MONITORINFO);
	if(!WSDISPLAYWIN95CALL(GetMonitorInfoA)(pThis->mHandle, &mi))
		return;
	r->top = mi.rcMonitor.top;
	r->bottom = mi.rcMonitor.bottom;
	r->left = mi.rcMonitor.left;
	r->right = mi.rcMonitor.right;
}
void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r)
{
	MONITORINFO mi;
	#if !defined(WSDISPLAY_NOWIN95)
		if(!pThis->mpfnGetMonitorInfoA)
		{
			r->top = r->left = r->bottom = r->right = 0;
			return;
		}
	#endif /* !WSDISPLAY_NOWIN95 */
	mi.cbSize = sizeof(MONITORINFO);
	if(!WSDISPLAYWIN95CALL(GetMonitorInfoA)(pThis->mHandle, &mi))
		return;
	r->top = mi.rcWork.top;
	r->bottom = mi.rcWork.bottom;
	r->left = mi.rcWork.left;
	r->right = mi.rcWork.right;
}
wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis)
{
	MONITORINFO mi;
	#if !defined(WSDISPLAY_NOWIN95)
		if(!pThis->mpfnGetMonitorInfoA)
			return TRUE;
	#endif /* !WSDISPLAY_NOWIN95 */
	mi.cbSize = sizeof(MONITORINFO);
	if(!WSDISPLAYWIN95CALL(GetMonitorInfoA)(pThis->mHandle, &mi))
		return TRUE;
	return mi.dwFlags & MONITORINFOF_PRIMARY;
}
void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	DEVMODEA dm;
	MONITORINFOEXA mi;
	#if !defined(WSDISPLAY_NOWIN95)
		if(!pThis->mpfnEnumDisplaySettingsA || !pThis->mpfnGetMonitorInfoA)
			return;
	#endif /* !WSDISPLAY_NOWIN95 */
	mi.cbSize = sizeof(mi);
	if(!WSDISPLAYWIN95CALL(GetMonitorInfoA)(pThis->mHandle, (MONITORINFO*)&mi))
		return;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	if (!WSDISPLAYWIN95CALL(EnumDisplaySettingsA)(
			mi.szDevice, ENUM_CURRENT_SETTINGS, &dm))
		return;
	WSVideoMode_Construct(mode,
		dm.dmPelsWidth, dm.dmPelsHeight, dm.dmBitsPerPel,
		dm.dmDisplayFrequency > 1 ? dm.dmDisplayFrequency : 0);
}
wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	DEVMODEA dm;
	DEVMODEA* pDm;
	MONITORINFOEXA mi;
	int flags;
	LONG res;
	#if !defined(WSDISPLAY_NOWIN95)
		if(
		#if !defined(WSDISPLAY_NOWINNT31)
			!pThis->mpfnChangeDisplaySettingsA
				&&
		#endif /* !WSDISPLAY_NOWINNT31 */
					!pThis->mpfnChangeDisplaySettingsExA)
			return FALSE;
	#endif /* !WSDISPLAY_NOWIN95 */
	mi.cbSize = sizeof(mi);
	if(
		#if !defined(WSDISPLAY_NOWIN95)
			pThis->mpfnGetMonitorInfoA
			&&
		#endif
			!WSDISPLAYWIN95CALL(GetMonitorInfoA)(
					pThis->mHandle, (MONITORINFO*)&mi))
		return FALSE;

	if (WSVideoMode_IsDefault(mode))
	{ /* reset mode to default */
		pDm = NULL;
		flags = 0;
	}
	else
	{
		dm.dmSize = sizeof(dm);
		dm.dmDriverExtra = 0;
		dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
		dm.dmPelsWidth = mode->width;
		dm.dmPelsHeight = mode->height;

		if (mode->depth)
		{
			dm.dmFields |= DM_BITSPERPEL;
			dm.dmBitsPerPel = mode->depth;
		}

		if (mode->refreshrate)
		{
			dm.dmFields |= DM_DISPLAYFREQUENCY;
			dm.dmDisplayFrequency = mode->refreshrate;
		}

		pDm = &dm;

#if defined(_WIN32_WCE)
		flags = 0;
#else
		flags = CDS_FULLSCREEN;
#endif
	}
	#if !defined(WSDISPLAY_NOWIN95)
		if(pThis->mpfnChangeDisplaySettingsExA)
	#endif /* !WSDISPLAY_NOWIN95 */
			res = WSDISPLAYWIN95CALL(ChangeDisplaySettingsExA)(mi.szDevice,
				pDm, NULL, flags, NULL);
	#if !defined(WSDISPLAY_NOWIN95)
		else
			res = WSDISPLAYWINNT31CALL(ChangeDisplaySettingsA)(pDm, flags);
	#endif /* !WSDISPLAY_NOWIN95 */
	return res == DISP_CHANGE_SUCCESSFUL;
}
void WSDisplay_ResetMode(const WSDISPLAY* pThis)
{
	WSVIDEOMODE vm;
	WSVideoMode_ConstructDefault(&vm);
	WSDisplay_ChangeMode(pThis, &vm);
}
void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode,
	WSVIDEOMODE** modes, unsigned int* modescount)
{
	DEVMODEA dm;
	MONITORINFOEXA mi;
	int iMode;
	#if !defined(WSDISPLAY_NOWIN95)
		if(!pThis->mpfnEnumDisplaySettingsA || !pThis->mpfnGetMonitorInfoA)
			return;
	#endif /* !WSDISPLAY_NOWIN95 */
	mi.cbSize = sizeof(mi);
	if(!WSDISPLAYWIN95CALL(GetMonitorInfoA)(pThis->mHandle, (MONITORINFO*)&mi))
		return;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	for(iMode = 0;
		WSDISPLAYWIN95CALL(EnumDisplaySettingsA)(
			mi.szDevice, iMode, &dm);
		++iMode)
	{
		WSVIDEOMODE* newmodes;
		WSVIDEOMODE curmode;
		WSVideoMode_Construct(&curmode,
			dm.dmPelsWidth, dm.dmPelsHeight, dm.dmBitsPerPel,
			dm.dmDisplayFrequency > 1 ? dm.dmDisplayFrequency : 0);
		if(WSVideoMode_Matches(&curmode, matchingmode))
		{
			if(iMode)
				newmodes = (WSVIDEOMODE*)HeapReAlloc(GetProcessHeap(), 0,
					(*modes), sizeof(WSVIDEOMODE) * (iMode + 1));
			else
				newmodes = (WSVIDEOMODE*)HeapAlloc(GetProcessHeap(),
					0, sizeof(WSVIDEOMODE));
			if(!newmodes)
				break;
			(*modes) = newmodes;
			(*modes)[iMode] = curmode;
		}
	}
	*modescount = iMode;
}
void WSDisplayGetGeometry(WSRECT* r)
{
#if defined(WSDISPLAY_STATICGDI32)
	HDC hScreenDC = GetDC(NULL);
	r->top = r->left = 0;
	r->right = GetDeviceCaps(hScreenDC, HORZRES);
	r->bottom = GetDeviceCaps(hScreenDC, VERTRES);
	ReleaseDC(NULL, hScreenDC);
#else
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	WSDisplay_GetGeometry(&d, r);
	WSDisplay_Destroy(&d);
#endif
}
void WSDisplayGetClientArea(WSRECT* r)
{
	SystemParametersInfoA(SPI_GETWORKAREA, 0, (RECT*)r, 0);
}
void WSDisplayGetDepth(guipos_t* depth)
{
	#if !defined(WSDISPLAY_STATICGDI32)
		HINSTANCE hGdi32;
		GetDeviceCapsProc pfnGetDeviceCaps;
	#endif /* !WSDISPLAY_STATICGDI32 */
	HDC hScreenDC = GetDC(NULL);
	#if !defined(WSDISPLAY_STATICGDI32)
		hGdi32 = LoadLibraryA("gdi32.dll");
		pfnGetDeviceCaps = (GetDeviceCapsProc)
			GetProcAddress(hGdi32, "GetDeviceCaps");
		*depth = (guipos_t)((*pfnGetDeviceCaps)(hScreenDC, PLANES)
				* (*pfnGetDeviceCaps)(hScreenDC, BITSPIXEL));
		FreeLibrary(hGdi32);
	#else /* !WSDISPLAY_STATICGDI32 */
		*depth = (guipos_t)(GetDeviceCaps(hScreenDC, PLANES)
			* GetDeviceCaps(hScreenDC, BITSPIXEL));
	#endif /* !WSDISPLAY_STATICGDI32 */
	ReleaseDC(NULL, hScreenDC);
}
void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth)
{
	HDC hScreenDC;
	MONITORINFOEXA mi;
	#if !defined(WSDISPLAY_STATICGDI32)
		HINSTANCE hGdi32;
		GetDeviceCapsProc pfnGetDeviceCaps;
		CreateDCAProc pfnCreateDCA;
		DeleteDCProc pfnDeleteDC;
	#endif /* !WSDISPLAY_STATICGDI32 */
	#if !defined(WSDISPLAY_NOWIN95)
		if(!pThis->mpfnGetMonitorInfoA)
		{
			*depth = 0;
			return;
		}
	#endif /* !WSDISPLAY_NOWIN95 */
	mi.cbSize = sizeof(mi);
	if(!WSDISPLAYWIN95CALL(GetMonitorInfoA)(
			pThis->mHandle, (MONITORINFO*)&mi))
	{
		*depth = 0;
		return;
	}
	#if !defined(WSDISPLAY_STATICGDI32)
		hGdi32 = LoadLibraryA("gdi32.dll");
		pfnGetDeviceCaps = (GetDeviceCapsProc)
			GetProcAddress(hGdi32, "GetDeviceCaps");
		pfnCreateDCA = (CreateDCAProc)
			GetProcAddress(hGdi32, "CreateDCA");
		pfnDeleteDC = (DeleteDCProc)
			GetProcAddress(hGdi32, "DeleteDC");
		hScreenDC = (*pfnCreateDCA)(NULL, mi.szDevice, NULL, NULL);
		*depth = (guipos_t)((*pfnGetDeviceCaps)(hScreenDC, PLANES)
				* (*pfnGetDeviceCaps)(hScreenDC, BITSPIXEL));
		(*pfnDeleteDC)(hScreenDC);
		FreeLibrary(hGdi32);
	#else /* !WSDISPLAY_STATICGDI32 */
		hScreenDC = CreateDCA(NULL, mi.szDevice, NULL, NULL);
		*depth = (guipos_t)(GetDeviceCaps(hScreenDC, PLANES)
			* GetDeviceCaps(hScreenDC, BITSPIXEL));
		DeleteDC(hScreenDC);
	#endif /* !WSDISPLAY_STATICGDI32 */
}

#if defined(WSDISPLAY_STATICGDI32)
	#if defined(_MSC_VER)
		#pragma comment(lib, "gdi32.lib")
	#endif /* _MSC_VER */
#endif /* WSDISPLAY_STATICGDI32 */
#if defined(WSDISPLAY_NOWIN8) && defined(WSDISPLAY_STATICSHCORE)
	#include <shellscalingapi.h>
	#if defined(_MSC_VER)
		#pragma comment(lib, "shcore.lib")
	#endif /* _MSC_VER */
#endif /* defined(WSDISPLAY_NOWIN8) && defined(WSDISPLAY_STATICSHCORE) */
#ifndef MDT_EFFECTIVE_DPI
	#define MDT_EFFECTIVE_DPI 0 /* DWM's DPI that incorporates accessibility */
	#define MDT_ANGULAR_DPI 1 /* Compliant angular DPI without accessibility */
	#define MDT_RAW_DPI 2 /* Linear DPI of monitor */
#endif
void WSDisplaySetDPIAware()
{
#if defined(WSDISPLAY_NOWINXP)
	SetProcessDPIAware();
#else
	typedef BOOL(WINAPI* SETPROCESSWSDPIAWAREOPTPROC) ();
	HINSTANCE user32 = LoadLibraryA("user32.dll");
	SETPROCESSWSDPIAWAREOPTPROC lpSetProcessDPIAware;
	lpSetProcessDPIAware =
		(SETPROCESSWSDPIAWAREOPTPROC)GetProcAddress(
			user32, "SetProcessDPIAware");
	if (lpSetProcessDPIAware)
		(*lpSetProcessDPIAware)();
	FreeLibrary(user32);
#endif
}
void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y)
{ /* WITHOUT CALLING SetProcessDPIAware THESE WILL ALWAYS GIVE 96! */
	#if defined(WSDISPLAY_NOWIN8) && defined(WSDISPLAY_STATICSHCORE)
		unsigned int uiX, uiY;
		if(SUCCEEDED(GetDpiForMonitor(
			pThis->mHandle, MDT_EFFECTIVE_DPI, &uiX, &uiY)))
		{
			*x = (guipos_t)uiX;
			*y = (guipos_t)uiY;
		}
		else
	#elif !defined(WSDISPLAY_STATICSHCORE)
		unsigned int uiX, uiY;
		HINSTANCE hShcore = LoadLibraryA("shcore.dll");
		GetDpiForMonitorProc pfnGetDpiForMonitor = (GetDpiForMonitorProc)
			GetProcAddress(hShcore, "GetDpiForMonitor");
		FreeLibrary(hShcore);
		if(pfnGetDpiForMonitor && SUCCEEDED((*pfnGetDpiForMonitor)(
			pThis->mHandle, MDT_EFFECTIVE_DPI, &uiX, &uiY)) && 0)
		{
			*x = (guipos_t)uiX;
			*y = (guipos_t)uiY;
		}
		else
	#endif /* WSDISPLAY_NOWIN8 && WSDISPLAY_STATICSHCORE */
	{
		#if !defined(WSDISPLAY_STATICGDI32)
			HINSTANCE hGdi32;
			GetDeviceCapsProc pfnGetDeviceCaps;
			CreateDCAProc pfnCreateDCA;
			DeleteDCProc pfnDeleteDC;
		#endif /* !WSDISPLAY_STATICGDI32 */
		MONITORINFOEXA mi;
		HDC hScreenDC;
		#if !defined(WSDISPLAY_NOWIN95)
			if(!pThis->mpfnGetMonitorInfoA)
			{
				*x = *y = 0;
				return;
			}
		#endif /* !WSDISPLAY_NOWIN95 */
		mi.cbSize = sizeof(mi);
		if(!WSDISPLAYWIN95CALL(GetMonitorInfoA)(
				pThis->mHandle, (MONITORINFO*)&mi))
		{
			*x = *y = 0;
			return;
		}
		#if !defined(WSDISPLAY_STATICGDI32)
			hGdi32 = LoadLibraryA("gdi32.dll");
			pfnGetDeviceCaps = (GetDeviceCapsProc)
				GetProcAddress(hGdi32, "GetDeviceCaps");
			pfnCreateDCA = (CreateDCAProc)
				GetProcAddress(hGdi32, "CreateDCA");
			pfnDeleteDC = (DeleteDCProc)
				GetProcAddress(hGdi32, "DeleteDC");
			hScreenDC = (*pfnCreateDCA)(NULL, mi.szDevice, NULL, NULL);
			*x = (*pfnGetDeviceCaps)(hScreenDC, LOGPIXELSX);
			*y = (*pfnGetDeviceCaps)(hScreenDC, LOGPIXELSY);
			(*pfnDeleteDC)(hScreenDC);
			FreeLibrary(hGdi32);
		#else /* !WSDISPLAY_STATICGDI32 */
			hScreenDC = CreateDCA(NULL, mi.szDevice, NULL, NULL);
			*x = GetDeviceCaps(hScreenDC, LOGPIXELSX);
			*y = GetDeviceCaps(hScreenDC, LOGPIXELSY);
			DeleteDC(hScreenDC);
		#endif /* !WSDISPLAY_STATICGDI32 */
	}
	#if !defined(WSDISPLAY_NOWIN8) && !defined(WSDISPLAY_STATICSHCORE)
		FreeLibrary(hShcore);
	#endif /* !defined(WSDISPLAY_NOWIN8) && !defined(WSDISPLAY_STATICSHCORE) */
}
void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale)
{
	guipos_t x, y, stdppi;
	WSDisplay_GetPPI(pThis, &x, &y);
	WSDisplayGetStdPPIValue(&stdppi);
	*scale = y ? y / (float)(stdppi) : (float)(1.0);
}
typedef struct HKEYLIST
{
	struct HKEYLIST* mNext;
	HKEY mhKey;
} HKEYLIST;
size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize)
{
	/*
	*	A rather involved process despite monitor names being stored in
	*	simple registry keys. The problem is that Windows refers to monitor
	*	names as "device names", which means it's either a string representing
	*	a device id or a generic name like "Generic PnP Monitor". To obtain
	*	the real ("friendly") name there are several completely different
	*	methods:
	*
	*	QueryDisplayConfig (7+)
	*
	*	The most straightforward, and of course least compatible method
	*	requiring just 3 functions:
	*	GetDisplayConfigBufferSizes->QueryDisplayConfig->
	*	DisplayConfigGetDeviceInfo(DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME)
	*
	*	WMI (Vista+)
	*
	*	https://docs.microsoft.com/en-us/windows/win32/wmicoreprov/wmimonitorid
	*	CoCreateInstance an IWbemLocator, enumerate and query a couple
	*	interfaces to get an IWbemClassObject corresponding to the
	*	monitor, then call Get methods to get EDID info.
	*
	*	Most compatible: EnumDisplayDevices (98/2K+) and registry functions
	*
	*	Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\DISPLAY\
	*	Ignore Default_Monitor
	*	Enum until Address value is not 0
	*	EDID value of "Device Parameters" subkey
	*/
	typedef LONG (WINAPI* REGQUERYVALUEEXAPROC)(HKEY hKey, LPCSTR lpValueName,
		LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	typedef LONG (WINAPI* REGOPENKEYAPROC)(HKEY hKey, LPCSTR lpSubKey,  PHKEY phkResult);
	typedef LONG (WINAPI* REGENUMKEYAPROC)(HKEY hKey, DWORD dwIndex, LPSTR lpName, DWORD cchName);
	typedef LONG (WINAPI* REGCLOSEKEYPROC)(HKEY hKey);
	#if !defined(WSDISPLAY_NOWIN95)
		typedef BOOL (WINAPI* EnumDisplayDevicesProc)(LPCSTR lpDevice, DWORD iDevNum,
			PDISPLAY_DEVICEA lpDisplayDevice, DWORD dwFlags);
	#endif
	DISPLAY_DEVICEA dd;
	MONITORINFOEXA mi;
	const char* devidstart; const char* devidend;
	char* regpath;
	size_t devidstartpos, devidendpos, devidlen, regstartlen;
	HKEYLIST* curkeylist; HKEYLIST* lastkeylist;
	BYTE EDIDdata[1024];
	DWORD edidsize;
	BOOL success;
	REGQUERYVALUEEXAPROC pfnRegQueryValueExA;
	REGENUMKEYAPROC pfnRegEnumKeyA;
	REGOPENKEYAPROC pfnRegOpenKeyA;
	REGCLOSEKEYPROC pfnRegCloseKey;
	HKEY hKey;
	#if !defined(WSDISPLAY_STATICADVAPI32)
		HINSTANCE hAdvApi32;
	#endif
	/* First, get the monitor's root in
	SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\ */
	#if !defined(WSDISPLAY_NOWIN95)
		EnumDisplayDevicesProc lpEnumDisplayDevicesA = (EnumDisplayDevicesProc)
			GetProcAddress(pThis->mDisplayDLL, "EnumDisplayDevicesA");
		if (!lpEnumDisplayDevicesA)
			return 0;
	#endif
	mi.cbSize = sizeof(mi);
	if(!WSDISPLAYWIN95CALL(GetMonitorInfoA)(pThis->mHandle, (MONITORINFO*)&mi))
		return 0;
	dd.cb = sizeof(dd);
	#if !defined(WSDISPLAY_NOWIN95)
		if(!(*lpEnumDisplayDevicesA)(mi.szDevice, 0, &dd, 0))
	#else
		if(!EnumDisplayDevicesA(mi.szDevice, 0, &dd, 0))
	#endif
			return 0;
	/* Extract monitor ID from the device ID string */
	devidstart = strstr(dd.DeviceID, "DISPLAY\\");
	if(!devidstart)
	{
		devidstart = strstr(dd.DeviceID, "MONITOR\\");
		if(!devidstart)
			return 0;
	}
	devidend = strstr(devidstart + 8, "\\");
	if(!devidend)
		return 0;
	devidstartpos = devidstart - dd.DeviceID;
	devidendpos = devidend - dd.DeviceID;
	devidlen = devidendpos - devidstartpos - 8;
	regstartlen =
		(sizeof("SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\") / sizeof(char))
			- 1;
	regpath = (char*)HeapAlloc(GetProcessHeap(), 0, sizeof(char)
				* (devidlen + regstartlen + 1));
	if(!regpath)
		return 0;
	memcpy(regpath, "SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\", regstartlen);
	memcpy(&regpath[regstartlen], &devidstart[8], devidlen * sizeof(char));
	regpath[regstartlen + devidlen] = '\0';

	/* Now open the root key in SYSTEM\\CurrentControlSet\\Enum\\DISPLAY\\ */
	#if !defined(WSDISPLAY_STATICADVAPI32)
		hAdvApi32 = LoadLibraryA("advapi32.dll");
		pfnRegQueryValueExA = (REGQUERYVALUEEXAPROC)
			GetProcAddress(hAdvApi32, "RegQueryValueExA");
		pfnRegEnumKeyA = (REGENUMKEYAPROC)
			GetProcAddress(hAdvApi32, "RegEnumKeyA");
		pfnRegOpenKeyA = (REGOPENKEYAPROC)
			GetProcAddress(hAdvApi32, "RegOpenKeyA");
		pfnRegCloseKey = (REGCLOSEKEYPROC)
			GetProcAddress(hAdvApi32, "RegCloseKey");
	#else
		pfnRegQueryValueExA = RegQueryValueExA;
		pfnRegEnumKeyA = RegEnumKeyA;
		pfnRegOpenKeyA = RegOpenKeyA;
		pfnRegCloseKey = RegCloseKey;
	#endif


	success = (*pfnRegOpenKeyA)(HKEY_LOCAL_MACHINE, regpath, &hKey);
	HeapFree(GetProcessHeap(), 0, regpath);
	if (success != ERROR_SUCCESS)
	{
		DWORD dwNameLength = (DWORD)strlen(dd.DeviceString);
		if(buffer && buffersize)
		{
			if(buffersize <= dwNameLength)
				--buffersize;
			else
				buffersize = dwNameLength;
			memcpy(buffer, dd.DeviceString, sizeof(char) * buffersize);
			buffer[buffersize] = '\0';
		}
		#if !defined(WSDISPLAY_STATICADVAPI32)
			FreeLibrary(hAdvApi32);
		#endif
		return dwNameLength;
	}

	/* Now look for the EDID value in any of it or its subkeys */
	edidsize = sizeof(EDIDdata);
	curkeylist = (HKEYLIST*)HeapAlloc(GetProcessHeap(), 0, sizeof(HKEYLIST));
	if(!curkeylist)
	{
		DWORD dwNameLength = (DWORD)strlen(dd.DeviceString);
		if(buffer && buffersize)
		{
			if(buffersize <= dwNameLength)
				--buffersize;
			else
				buffersize = dwNameLength;
			memcpy(buffer, dd.DeviceString, sizeof(char) * buffersize);
			buffer[buffersize] = '\0';
		}
		(*pfnRegCloseKey)(hKey);
		#if !defined(WSDISPLAY_STATICADVAPI32)
			FreeLibrary(hAdvApi32);
		#endif
		return dwNameLength;
	}
	lastkeylist = curkeylist;
	curkeylist->mNext = NULL;
	curkeylist->mhKey = hKey;
	success = FALSE;
	do
	{
		DWORD dwIndex;
		char subKeyName[256];
		DWORD subKeyNameSize;
		if (curkeylist != lastkeylist)
		{
			HKEYLIST* temp = curkeylist->mNext;
			(*pfnRegCloseKey)(curkeylist->mhKey);
			HeapFree(GetProcessHeap(), 0, curkeylist);
			curkeylist = temp;
		}
		if ((*pfnRegQueryValueExA)(curkeylist->mhKey, "EDID",
				NULL, NULL, EDIDdata, &edidsize) == ERROR_SUCCESS)
		{
			success = TRUE;
			break;
		}

		subKeyNameSize = sizeof(subKeyName) / sizeof(subKeyName[0]);
		dwIndex = 0;
		while ((*pfnRegEnumKeyA)(curkeylist->mhKey, dwIndex, subKeyName,
				subKeyNameSize) == ERROR_SUCCESS)
		{
			HKEY hSubKey;
			if ((*pfnRegOpenKeyA)(curkeylist->mhKey,
					subKeyName, &hSubKey) == ERROR_SUCCESS)
			{
				HKEYLIST* temp = lastkeylist;
				lastkeylist = (HKEYLIST*)
					HeapAlloc(GetProcessHeap(), 0, sizeof(HKEYLIST));
				if(!lastkeylist)
					break;
				lastkeylist->mNext = NULL;
				lastkeylist->mhKey = hSubKey;
				temp->mNext = lastkeylist;
			}
			subKeyNameSize = sizeof(subKeyName) / sizeof(subKeyName[0]);
			dwIndex++;
		}
		if(!lastkeylist)
			break;
	} while(curkeylist->mNext);

	while(curkeylist)
	{
		HKEYLIST* temp = curkeylist->mNext;
		(*pfnRegCloseKey)(curkeylist->mhKey);
		HeapFree(GetProcessHeap(), 0, curkeylist);
		curkeylist = temp;
	}

	#if !defined(WSDISPLAY_STATICADVAPI32)
		FreeLibrary(hAdvApi32);
	#endif
	if (success)
	{
		DWORD dwCount = 0;
		BYTE* beg = NULL;
		if (EDIDdata[72] == 0 && EDIDdata[72 + 3] == 0xFC)
			beg = &EDIDdata[72 + 5];
		else if (EDIDdata[90] == 0 && EDIDdata[90 + 3] == 0xFC)
			beg = &EDIDdata[90 + 5];
		else
			beg = &EDIDdata[108 + 5];

		for (; dwCount < 13; ++dwCount)
			if (beg[dwCount] == '\n')
				break;

		if(buffer && buffersize)
		{
			if(buffersize <= dwCount)
				--buffersize;
			else
				buffersize = dwCount;
			memcpy(buffer, beg, sizeof(char) * buffersize);
			buffer[buffersize] = '\0';
		}
		return dwCount;
	}
	else
	{
		DWORD dwNameLength = (DWORD)strlen(dd.DeviceString);
		if(buffer && buffersize)
		{
			if(buffersize <= dwNameLength)
				--buffersize;
			else
				buffersize = dwNameLength;
			memcpy(buffer, dd.DeviceString, sizeof(char) * buffersize);
			buffer[buffersize] = '\0';
		}
		return dwNameLength;
	}
}
int WSDisplay_MakePrimary(const WSDISPLAY* pThis)
{
	MONITORINFOEXA mi;
	DEVMODE dm;
	#if !defined(WSDISPLAY_NOWIN95)
		if(!pThis->mpfnGetMonitorInfoA
				|| !pThis->mpfnChangeDisplaySettingsExA)
			return FALSE;
	#endif /* !WSDISPLAY_NOWIN95 */

	mi.cbSize = sizeof(mi);
	if(!WSDISPLAYWIN95CALL(GetMonitorInfoA)(
					pThis->mHandle, (MONITORINFO*)&mi))
		return FALSE;

	/* Get current settings of the specified display device */
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);
	dm.dmFields = DM_POSITION;
	if (!WSDISPLAYWIN95CALL(EnumDisplaySettingsA)(
			mi.szDevice, ENUM_CURRENT_SETTINGS, &dm))
		return FALSE;

	/* Make the device with the name of szDevice the primary display */
	dm.dmPosition.x = 0;
	dm.dmPosition.y = 0;
	if (WSDISPLAYWIN95CALL(ChangeDisplaySettingsExA)(mi.szDevice,
			&dm, NULL, CDS_SET_PRIMARY | CDS_UPDATEREGISTRY | CDS_NORESET,
			NULL) != DISP_CHANGE_SUCCESSFUL)
		return FALSE;

	/* Apply the changes to all displays */
	WSDISPLAYWIN95CALL(ChangeDisplaySettingsExA)(NULL, NULL, NULL, 0, NULL);
	return TRUE;
}
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
			 #     #  #####   #####    ###     ###     ###
			  #   #  #     # #     #  #   #   #   #   #   #
			   # #   #       #     # #     # #     # #     #
				#    ######   #####  #     # #     # #     #
			   # #   #     # #     # #     # #     # #     #
			  #   #  #     # #     #  #   #   #   #   #   #
			 #     #  #####   #####    ###     ###     ###
=============================================================================*/
#elif defined(__human68k__) /* _WIN32 */
#ifdef __MARIKO_CC__
	#include <iocslib.h>
#else
	#include <iocs.h>
#endif
#define WSDISPLAY_MAX_MODES 28
#ifdef __cplusplus
	extern "C" {
#endif
void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis)
{ ++pThis->id; }
void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay)
{ WSDisplayGetPrimary(pDisplay); }
void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis)
{}

void WSDisplayGetPrimary(WSDISPLAY* pDisplay)
{
	pDisplay->mBaseMode = _iocs_crtmod(-1); /* save original mode */
	/*
			Mode List [Blank values mean same as previous mode]
		Mode	Actual	Resolution	#Colors	Khz
		0		1024	512 x 512	16		31
		1									15
		2				256 x 256			31
		3									15
		4		512		512 x 512			31
		5									15
		6				256 x 256			31
		7									15
		8				512 x 512	256		31
		9									15
		10				256 x 256			31
		11									15
		12				512 x 512	65536	31
		13									15
		14				256 x 256			31
		15									15
		16		1024	768 x 512	16		31
		17				1024 x 424			24
		18				1024 x 848			24
		19				640 x 480			24
		20				768 x 512	256		31
		21				1024 x 848			24
		22				1024 x 424			24
		23				640 x 480			24
		24				768 x 512	65536	31
		25				1024 x 848			24
		26				1024 x 424			24
		27				640 x 480			24
	*/
	WSVideoMode_Construct(&pDisplay->mModes[0], 512, 512, 4, 31);
	WSVideoMode_Construct(&pDisplay->mModes[1], 512, 512, 4, 15);
	WSVideoMode_Construct(&pDisplay->mModes[2], 256, 256, 4, 31);
	WSVideoMode_Construct(&pDisplay->mModes[3], 256, 256, 4, 15);
	WSVideoMode_Construct(&pDisplay->mModes[4], 512, 512, 4, 31);
	WSVideoMode_Construct(&pDisplay->mModes[5], 512, 512, 4, 15);
	WSVideoMode_Construct(&pDisplay->mModes[6], 256, 256, 4, 31);
	WSVideoMode_Construct(&pDisplay->mModes[7], 256, 256, 4, 15);
	WSVideoMode_Construct(&pDisplay->mModes[8], 512, 512, 8, 31);
	WSVideoMode_Construct(&pDisplay->mModes[9], 512, 512, 8, 15);
	WSVideoMode_Construct(&pDisplay->mModes[10], 256, 256, 8, 31);
	WSVideoMode_Construct(&pDisplay->mModes[11], 256, 256, 8, 15);
	WSVideoMode_Construct(&pDisplay->mModes[12], 512, 512, 16, 31);
	WSVideoMode_Construct(&pDisplay->mModes[13], 512, 512, 16, 15);
	WSVideoMode_Construct(&pDisplay->mModes[14], 256, 256, 16, 31);
	WSVideoMode_Construct(&pDisplay->mModes[15], 256, 256, 16, 15);
	WSVideoMode_Construct(&pDisplay->mModes[16], 768, 512, 4, 31);
	WSVideoMode_Construct(&pDisplay->mModes[17], 1024, 424, 4, 24);
	WSVideoMode_Construct(&pDisplay->mModes[18], 1024, 848, 4, 24);
	WSVideoMode_Construct(&pDisplay->mModes[19], 640, 480, 4, 24);
	WSVideoMode_Construct(&pDisplay->mModes[20], 768, 512, 8, 31);
	WSVideoMode_Construct(&pDisplay->mModes[21], 1024, 848, 8, 24);
	WSVideoMode_Construct(&pDisplay->mModes[22], 1024, 424, 8, 24);
	WSVideoMode_Construct(&pDisplay->mModes[23], 640, 480, 8, 24);
	WSVideoMode_Construct(&pDisplay->mModes[24], 768, 512, 16, 31);
	WSVideoMode_Construct(&pDisplay->mModes[25], 1024, 848, 16, 24);
	WSVideoMode_Construct(&pDisplay->mModes[26], 1024, 424, 16, 24);
	WSVideoMode_Construct(&pDisplay->mModes[27], 640, 480, 16, 24);
}
void WSDisplayBegin(WSDISPLAYITERATOR* pIt)
{ pIt->id = 0; }
wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt)
{ return pIt->id > 0; } /* X68000 only supports a single display */
wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
{ return FALSE; }
wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
{ return FALSE; }
wsbool WSDisplayGetFromWindow(void* windowhandle, WSDISPLAY* pDisplay)
{ return FALSE; }
void WSDisplayGetGeometry(WSRECT* r)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	if(WSDisplay_IsCreated(&d))
		WSDisplay_GetGeometry(&d, r);
	WSDisplay_Destroy(&d);
}
void WSDisplayGetClientArea(WSRECT* r)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	if(WSDisplay_IsCreated(&d))
		WSDisplay_GetClientArea(&d, r);
	WSDisplay_Destroy(&d);
}
void WSDisplayGetDepth(guipos_t* depth)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	if(WSDisplay_IsCreated(&d))
		WSDisplay_GetDepth(&d, depth);
	WSDisplay_Destroy(&d);
}
void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r)
{
	WSVIDEOMODE* curmode = &pThis->mModes[_iocs_crtmod(-1)];
	r->left = r->top = 0;
	r->right = curmode->width;
	r->bottom = curmode->height;
}
void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r)
{ WSDisplay_GetGeometry(pThis, r); } /* Don't think X68000 distinguishes? */
wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis)
{ return TRUE; }
void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	*mode = pThis->mModes[_iocs_crtmod(-1)];
}
wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	size_t i;
	for(i = 0; i < WSDISPLAY_MAX_MODES; ++i)
	{
		if (WSVideoMode_Matches(&pThis->mModes[i], mode))
		{
			_iocs_crtmod(i);
			return TRUE;
		}
	}
	return FALSE;
}
void WSDisplay_ResetMode(const WSDISPLAY* pThis)
{
	WSVIDEOMODE vm;
	WSVideoMode_ConstructDefault(&vm);
	WSDisplay_ChangeMode(pThis, &vm);
}
void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode,
	WSVIDEOMODE** modes, unsigned int* modescount)
{
	size_t i;
	*modescount = 0;
	if(modes)
		*modes = NULL;
	for(i = 0; i < WSDISPLAY_MAX_MODES; ++i)
	{
		if (WSVideoMode_Matches(&pThis->mModes[i], matchingmode))
		{
			++*modescount;
			if(modes)
			{
				WSVIDEOMODE* newmodes = WSREALLOC(*modes,
					WSVIDEOMODE, *modescount);
				if(newmodes)
				{
					newmodes[*modescount - 1] = pThis->mModes[i];
					*modes = newmodes;
				}
			}
		}
	}
}
void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y)
{ WSDisplayGetStdPPIValue(x); WSDisplayGetStdPPIValue(y); }
void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale)
{ *scale = 1.0; }
void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth)
{ *depth = pThis->mModes[_iocs_crtmod(-1)].depth; }
void WSDisplay_Destroy(WSDISPLAY* pThis)
{ }
size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize)
{ return 0; }
int WSDisplay_MakePrimary(const WSDISPLAY* pThis)
{ return FALSE; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
		#    #     # ###  #####     #        #####  ####### #     #
	   # #   ##   ##  #  #     #   # #      #     # #        #   #
	  #   #  # # # #  #  #        #   #     #       #         # #
	 #     # #  #  #  #  #  #### #     #    #  #### #####      #
	 ####### #     #  #  #     # #######    #     # #         # #
	 #     # #     #  #  #     # #     #    #     # #        #   #
	 #     # #     # ###  #####  #     #     #####  #       #     #
=============================================================================*/
#elif defined(__amigaos__) /* _WIN32 */
#include <exec/exec.h>
#include <cybergraphx/cybergraphics.h>
#include <graphics/scale.h>
#include <graphics/gfx.h>
#include <intuition/intuition.h>
#if defined(__SASC) || defined(WARPOS) || defined(AROS)
	#include <proto/exec.h>
	#include <proto/cybergraphics.h>
	#include <proto/graphics.h>
	#include <proto/intuition.h>
	#include <proto/console.h>
#else /* defined(__SASC) || defined(WARPOS) || defined(AROS) */
	#ifdef MORPHOS
		#include <ppcinline/exec.h>
		#include <ppcinline/cybergraphics.h>
		#include <ppcinline/graphics.h>
		#include <ppcinline/intuition.h>
		#include <ppcinline/console.h>
	#else /* MORPHOS */
		#include <inline/exec.h>
		#include <inline/cybergraphics.h>
		#include <inline/graphics.h>
		#include <inline/intuition.h>
		#include <inline/console.h>
	#endif /* MORPHOS */
#endif /* defined(__SASC) || defined(WARPOS) || defined(AROS) */
#ifdef __cplusplus
	extern "C" {
#endif
/*extern struct Library* CyberGfxBase;*/
extern struct IntuitionBase* IntuitionBase;
extern struct GfxBase* GfxBase;
extern struct ExecBase* SysBase;
extern struct DosLibrary* DOSBase;
wsbool WSDisplayInit()
{
	if (!GfxBase)
		if (!(GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 39L)))
			return FALSE;
	/* No longer needed as BestCModeIDTags isn't used ATM */
	/*if (!CyberGfxBase)
		if (!(CyberGfxBase = OpenLibrary("cybergraphics.library", 40L)))
		{
			CloseLibrary((struct Library*)GfxBase);
			return FALSE;
		}*/
	return TRUE;
}
void WSDisplayIterator_SkipNonCyberModeIDs(WSDISPLAYITERATOR* pThis)
{
	while(WSDisplayIsEnd(pThis) == FALSE && IsCyberModeID(pThis->id) == 0)
		WSDisplayIterator_Iterate(pThis);
}
void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis)
{ ++pThis->id; }
void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay)
{ WSDisplayGetPrimary(pDisplay); }
void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis)
{}

void WSDisplayGetPrimary(WSDISPLAY* pDisplay)
{
	if(WSDisplayInit())
		pDisplay->mHandle = LockPubScreen(NULL);
	else
		pDisplay->mHandle = NULL;
	pDisplay->mIsOpen = FALSE;
}
void WSDisplayBegin(WSDISPLAYITERATOR* pIt)
{ pIt->id = 0; }
wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt)
{ return pIt->id > 0; } /* Amiga only supports a single display */
wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
{ return FALSE; }
wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
{ return FALSE; }
wsbool WSDisplayGetFromWindow(void* windowhandle, WSDISPLAY* pDisplay)
{ return FALSE; }
void WSDisplayGetGeometry(WSRECT* r)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	if(WSDisplay_IsCreated(&d))
		WSDisplay_GetGeometry(&d, r);
	WSDisplay_Destroy(&d);
}
void WSDisplayGetClientArea(WSRECT* r)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	if(WSDisplay_IsCreated(&d))
		WSDisplay_GetClientArea(&d, r);
	WSDisplay_Destroy(&d);
}
void WSDisplayGetDepth(guipos_t* depth)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	if(WSDisplay_IsCreated(&d))
		WSDisplay_GetDepth(&d, depth);
	WSDisplay_Destroy(&d);
}
void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r)
{
	r->left = r->top = 0;
	r->right = pThis->mHandle->Width;
	r->bottom = pThis->mHandle->Height;
}
void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r)
{ WSDisplay_GetGeometry(pThis, r); } /* Don't think Amiga distinguishes? */
wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis)
{ return pThis->mHandle == FindDisplayInfo(DTAG_DISP); }
void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	DisplayInfoHandle hDi = FindDisplayInfo(DTAG_DISP);
	struct DimensionInfo diminfo;
	if(!hDi
		|| !GetDisplayInfoData(hDi, (char*)&diminfo,
				sizeof(struct DimensionInfo), DTAG_DIMS, 0))
	{
		WSVideoMode_ConstructDefault(mode);
		return;
	}
	WSVideoMode_Construct(mode, diminfo.Nominal.MaxX + 1,
		diminfo.Nominal.MaxY + 1, diminfo.MaxDepth, 60);
}
wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	struct Screen* newscreen;
	WSVIDEOMODE rm;
	long id = INVALID_ID;
	DisplayInfoHandle hDi;
	struct DimensionInfo diminfo;
	WSVIDEOMODE curmode;
	if(WSVideoMode_IsDefault(mode))
	{	WSVideoMode_Construct(&rm, 640, 480, 8, 60); }
	else
		rm = *mode;
	/* find the matching mode id */
	while((id = NextDisplayInfo(id)) != INVALID_ID)
	{
		hDi = FindDisplayInfo(id);
		if(!hDi)
			continue;
		if(!GetDisplayInfoData(hDi, (char*)&diminfo,
					sizeof(struct DimensionInfo), DTAG_DIMS, 0))
			continue;
		WSVideoMode_Construct(&curmode, diminfo.Nominal.MaxX + 1,
			diminfo.Nominal.MaxY + 1, diminfo.MaxDepth, 60);
		if (WSVideoMode_MatchesNonSafe(&curmode, &rm))
			break;
	}
	/*Alt to the above, req CyberGfx */
	/*id = BestCModeIDTags(CYBRBIDTG_NominalWidth, rm.width,
		CYBRBIDTG_NominalHeight, rm.height, CYBRBIDTG_Depth,
		rm.depth, TAG_DONE);*/
	if(id == INVALID_ID)
		return FALSE;
	WSDisplay_Destroy((WSDISPLAY*)pThis);
	newscreen = OpenScreenTags(NULL, SA_Width, rm.width,
		SA_Height, rm.height, SA_Depth, rm.depth,
		SA_DisplayID, id,
		SA_ShowTitle, FALSE, TAG_DONE);
	if(newscreen)
	{
		((WSDISPLAY*)pThis)->mHandle = newscreen;
		((WSDISPLAY*)pThis)->mIsOpen = TRUE;
		return TRUE;
	}
	else
		WSDisplayGetPrimary((WSDISPLAY*)pThis);
	return FALSE;
}
void WSDisplay_ResetMode(const WSDISPLAY* pThis)
{
	WSVIDEOMODE vm;
	WSVideoMode_ConstructDefault(&vm);
	WSDisplay_ChangeMode(pThis, &vm);
}
void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode,
	WSVIDEOMODE** modes, unsigned int* modescount)
{
	long id = INVALID_ID;
	DisplayInfoHandle hDi;
	struct DimensionInfo diminfo;
	WSVIDEOMODE curmode;
	*modescount = 0;
	if(modes)
		*modes = NULL;
	while((id = NextDisplayInfo(id)) != INVALID_ID)
	{
		hDi = FindDisplayInfo(id);
		if(!hDi)
			continue;
		if(!GetDisplayInfoData(hDi, (char*)&diminfo,
					sizeof(struct DimensionInfo), DTAG_DIMS, 0))
			continue;
		WSVideoMode_Construct(&curmode, diminfo.Nominal.MaxX + 1,
			diminfo.Nominal.MaxY + 1, diminfo.MaxDepth, 60);
		if (WSVideoMode_Matches(&curmode, matchingmode))
		{
			++*modescount;
			if(modes)
			{
				WSVIDEOMODE* newmodes = WSREALLOC(*modes,
					WSVIDEOMODE, *modescount);
				if(newmodes)
				{
					newmodes[*modescount - 1] = curmode;
					*modes = newmodes;
				}
			}
		}
	}
}
void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y)
{ WSDisplayGetStdPPIValue(x); WSDisplayGetStdPPIValue(y); }
void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale)
{ *scale = 1.0; }
void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth)
{ *depth = pThis->mHandle->RastPort.BitMap->Depth; }
void WSDisplay_Destroy(WSDISPLAY* pThis)
{
	if(pThis->mIsOpen)
		CloseScreen(pThis->mHandle);
	else
		UnlockPubScreen(NULL, pThis->mHandle);
}
size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize)
{ return 0; }
int WSDisplay_MakePrimary(const WSDISPLAY* pThis)
{ return 0; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
	 #     #    #     #####  ### #     # ####### #######  #####  #     #
	 ##   ##   # #   #     #  #  ##    #    #    #     # #     # #     #
	 # # # #  #   #  #        #  # #   #    #    #     # #       #     #
	 #  #  # #     # #        #  #  #  #    #    #     #  #####  #######
	 #     # ####### #        #  #   # #    #    #     #       # #     #
	 #     # #     # #     #  #  #    ##    #    #     # #     # #     #
	 #     # #     #  #####  ### #     #    #    #######  #####  #     #

			 ######  ###  #####  ######  #          #    #     #
			 #     #  #  #     # #     # #         # #    #   #
			 #     #  #  #       #     # #        #   #    # #
			 #     #  #   #####  ######  #       #     #    #
			 #     #  #        # #       #       #######    #
			 #     #  #  #     # #       #       #     #    #
			 ######  ###  #####  #       ####### #     #    #

		 #     #    #    #     #    #     #####  ####### ######
		 ##   ##   # #   ##    #   # #   #     # #       #     #
		 # # # #  #   #  # #   #  #   #  #       #       #     #
		 #  #  # #     # #  #  # #     # #  #### #####   ######
		 #     # ####### #   # # ####### #     # #       #   #
		 #     # #     # #    ## #     # #     # #       #    #
		 #     # #     # #     # #     #  #####  ####### #     #
=============================================================================*/
#elif defined(macintosh) /* _WIN32 */
#ifdef __MACH__
	#include <Carbon/Carbon.h>
#else
	#include <Gestalt.h>
	#include <Displays.h>
	#include <Quickdraw.h>
	#include <Video.h>  /* for VDSwitchInfoRec */
	#include <FixMath.h>
	#include <Debugging.h>
#endif
#include <stdlib.h> /* malloc et al. */
#include <string.h> /* memXXX */
#ifdef __cplusplus
	extern "C" {
#endif
#if 0
	/* Here's how you'd write GetClientArea for the main
	screen. However, GetAvailableWindowPositioningBounds isn't
	Carbon 1.0 */
#if TARGET_CARBON
	GetAvailableWindowPositioningBounds(GetMainDevice(), (Rect*)r) ;
#else
	BitMap screenBits;
	GetQDGlobalsScreenBits(&screenBits);
	SInt16 menuheight;
	r->left = 0;
	r->top = 0;
	r->right = screenBits.bounds.right - screenBits.bounds.left  ;
	r->bottom = screenBits.bounds.bottom - screenBits.bounds.top ;
#if TARGET_CARBON
	GetThemeMenuBarHeight(&menuheight);
#else
	menuheight = LMGetMBarHeight();
#endif
	r->top = menuheight;
#endif
#endif
SInt16 WSDisplayGetMenuBarHeight()
{
	SInt16 menuheight;
#if TARGET_CARBON
	GetThemeMenuBarHeight(&menuheight);
#else
	menuheight = LMGetMBarHeight();
#endif
	return menuheight;
}

void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis)
{
	pThis->display = DMGetNextScreenDevice(pThis->display, true);
}
void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay)
{
	pDisplay->mHandle = pThis->display;
}
void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis)
{
}
void WSDisplay_Destroy(WSDISPLAY* pThis)
{
}

void WSDisplayBegin(WSDISPLAYITERATOR* pIt)
{
	pIt->display = DMGetFirstScreenDevice(true);
}
wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt)
{
	return pIt->display == NULL;
}
wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
{
	unsigned num = 0;
	GDHandle hndl = DMGetFirstScreenDevice(true);
	while(hndl)
	{
		Rect screenrect = (*hndl)->gdRect;
		if (x >= screenrect.left
			&& x <= screenrect.right
			&& y >= screenrect.top
			&& y <= screenrect.bottom)
		{
			pDisplay->mHandle = hndl;
			return TRUE;
		}
		num++;
		hndl = DMGetNextScreenDevice(hndl, true);
	}

	return FALSE;
}
wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
{
	guipos_t x = r->left + ((r->right - r->left) / 2);
	guipos_t y = r->top + ((r->bottom - r->top) / 2);
	return WSDisplayGetFromPoint(x, y, pDisplay);
}
wsbool WSDisplayGetFromWindow(void* windowhandle, WSDISPLAY* pDisplay)
{
	/* find point r.x + (r.right - r.left)/2, r.y + (r.bottom - r.top)/2 */
	return FALSE;
}
void WSRectFromRect(WSRECT* r, Rect* mr)
{
	r->left = mr->left;
	r->top = mr->top;
	r->right = mr->right;
	r->bottom = mr->bottom;
}
void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r)
{
	WSRectFromRect(r, &(*pThis->mHandle)->gdRect);
}
void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r)
{
	WSRectFromRect(r, &(*pThis->mHandle)->gdRect);
	r->top += WSDisplayGetMenuBarHeight();
}
wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis)
{
	return pThis->mHandle == DMGetFirstScreenDevice(true);
}


typedef struct DMModeIteratorRec
{
	WSVIDEOMODE* pModes;
	size_t count;
	const WSVIDEOMODE* pMatchMode;
} DMModeIteratorRec;

pascal void DMModeListIteratorProc(
	void* pData,
	DMListIndexType nIndex,
	DMDisplayModeListEntryPtr pInfo)
{
	unsigned long i;
	DMModeIteratorRec* pInfoData = (DMModeIteratorRec*) pData;
	DMDepthInfoPtr pDI = pInfo->displayModeDepthBlockInfo->depthVPBlock;
	int refresh =
		(int)Fix2Long(pInfo->displayModeResolutionInfo->csRefreshRate);

	for (i = 0; i < pInfo->displayModeDepthBlockInfo->depthBlockCount; ++i)
	{
		WSVIDEOMODE vm;
		WSVideoMode_Construct(&vm,
			(int) pInfo->displayModeResolutionInfo->csHorizontalPixels,
			(int) pInfo->displayModeResolutionInfo->csVerticalLines,
			(int) pDI[i].depthVPBlock->vpPixelSize, refresh);
		if (WSVideoMode_Matches(&vm, pInfoData->pMatchMode))
		{
			WSVIDEOMODE* newmodes = WSREALLOC(pInfoData->pModes, WSVIDEOMODE,
				(pInfoData->count + 1));
			if(newmodes)
			{
				WSVideoMode_Construct(&newmodes[pInfoData->count],
					(int) pInfo->displayModeResolutionInfo->csHorizontalPixels,
					(int) pInfo->displayModeResolutionInfo->csVerticalLines,
					(int) pDI[i].depthVPBlock->vpPixelSize,
					refresh);
				pInfoData->pModes = newmodes;
				++pInfoData->count;
			}
		}
	}
}
typedef struct DMModeInfoRec
{
	const WSVIDEOMODE* pMode;
	VDSwitchInfoRec sMode;
	wsbool bMatched;
} DMModeInfoRec;

pascal void DMModeInfoProc(
	void* pData,
	DMListIndexType nIndex,
	DMDisplayModeListEntryPtr pInfo )
{
	unsigned long i;
	DMModeInfoRec* pInfoData = (DMModeInfoRec*) pData;
	DMDepthInfoPtr pDI = pInfo->displayModeDepthBlockInfo->depthVPBlock;
	Fixed refresh = Long2Fix(pInfoData->pMode->refreshrate);

	for (i = 0;
		i < pInfo->displayModeDepthBlockInfo->depthBlockCount; ++i)
	{
		VPBlockPtr pDBI =
			pInfo->displayModeDepthBlockInfo->depthVPBlock[i].depthVPBlock;
		if (pInfoData->pMode->width ==
				(int)pInfo->displayModeResolutionInfo->csHorizontalPixels
			&& pInfoData->pMode->height ==
				(int)pInfo->displayModeResolutionInfo->csVerticalLines
			&& pInfoData->pMode->depth == (int)pDBI->vpPixelSize
			&& refresh == pInfo->displayModeResolutionInfo->csRefreshRate)
		{
			memcpy(
				&pInfoData->sMode,
				pDI[i].depthSwitchInfo,
				sizeof(VDSwitchInfoRec));
			pInfoData->sMode.csMode = pDI[i].depthVPBlock->vpPixelSize;
			pInfoData->bMatched = true;
			break;
		}
	}
}
typedef struct DMModeTransRec
{
	WSVIDEOMODE Mode;
	const VDSwitchInfoRec* psMode;
	wsbool bMatched;
} DMModeTransRec;

pascal void DMModeTransProc(
	void* pData,
	DMListIndexType nIndex,
	DMDisplayModeListEntryPtr pInfo)
{
	unsigned long i;
	DMModeTransRec* pInfoData = (DMModeTransRec*) pData;
	DMDepthInfoPtr pDI = pInfo->displayModeDepthBlockInfo->depthVPBlock;

	for (i = 0;
		i < pInfo->displayModeDepthBlockInfo->depthBlockCount; ++i)
	{
		if (pInfoData->psMode->csData == pDI[i].depthSwitchInfo->csData)
		{
			WSVideoMode_Construct(&pInfoData->Mode,
				(int)pInfo->displayModeResolutionInfo->csHorizontalPixels,
				(int)pInfo->displayModeResolutionInfo->csVerticalLines,
				(int)pDI[i].depthVPBlock->vpPixelSize,
				(int)Fix2Long(pInfo->displayModeResolutionInfo->csRefreshRate)
			);
			pInfoData->bMatched = true;
			break;
		}
	}
}
void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	unsigned long dwDMVer;
	OSErr err;
	VDSwitchInfoRec sMode; /* csMode member also contains the bit depth */

	/*
		Check DM version == 2
		(for backward compatibility only - 7.5.3+ use 2.0)
	*/
	Gestalt(gestaltDisplayMgrVers, (long*) &dwDMVer);
	if (dwDMVer >= 0x020000)
	{
		err = DMGetDisplayMode(pThis->mHandle, &sMode);
		if (err == noErr)
		{
			DMListIndexType i;
			DMListIndexType nNumModes;
			DMListType pModes;
			DMDisplayModeListIteratorUPP uppMLI;
			DisplayIDType nDisplayID;
			DMModeTransRec sModeInfo;
			err = DMGetDisplayIDByGDevice(pThis->mHandle, &nDisplayID, false);
			err = DMNewDisplayModeList(nDisplayID,
				NULL, NULL, &nNumModes, &pModes);
			uppMLI = NewDMDisplayModeListIteratorUPP(DMModeTransProc);

			sModeInfo.bMatched = false;
			sModeInfo.psMode = &sMode;
			for (i = 0; i < nNumModes; ++i)
			{
				err = DMGetIndexedDisplayModeFromList(pModes,
					i, NULL, uppMLI, &sModeInfo);
				if (sModeInfo.bMatched)
				{
					*mode = sModeInfo.Mode;
					break;
				}
			}

			DisposeDMDisplayModeListIteratorUPP(uppMLI);
			err = DMDisposeList(pModes);
		}
		else
		{
			/*printf("Couldn't obtain current display mode!!!\n"
				"dwDMVer:%u\n",
				(unsigned int) dwDMVer);*/
		}
	} else
	{
		/*printf("Display Manager Version %u Not Supported!"
				" Present? %s\n",
			   (unsigned int) dwDMVer / 0x10000,
			   (dwDMVer & (1 << gestaltDisplayMgrPresent) ? "Yes" : "No"));*/
	}
}

wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	unsigned long dwDMVer;

	Gestalt(gestaltDisplayMgrVers, (long*)&dwDMVer);
	if (dwDMVer >= 0x020000)
	{
		VDSwitchInfoRec sMode;
		DMListIndexType nNumModes;
		DMListType pModes;
		DMDisplayModeListIteratorUPP uppMLI;
		DisplayIDType nDisplayID;
		OSErr err;
		unsigned long dwBPP;
		Handle hDisplayState;
		DMModeInfoRec sModeInfo;
		unsigned int i;

		if (WSVideoMode_IsDefault(mode))
		{
			#if 0
				#ifndef __MACH__
					Handle hDisplayState;
					if (DMBeginConfigureDisplays(&hDisplayState) != noErr)
					{
						/*printf("Could not lock display"
							"for display mode changing!");*/
						return false;
					}
					err = DMUseScreenPrefs(true, hDisplayState);
					DMEndConfigureDisplays(hDisplayState);
					return true;
				#else
					return true;
				#endif
			#endif
			return true;
		}

		/*
			0 & NULL for params 2 & 3 of DMSetDisplayMode signal
			it to use defaults (current mode)

			DM 2.0+ doesn't use params 2 & 3 of DMSetDisplayMode
		*/
		err = DMGetDisplayIDByGDevice(pThis->mHandle, &nDisplayID, false);
		assert(err == noErr);

		/* Create a new list... */
		err = DMNewDisplayModeList(nDisplayID, NULL, NULL,
			&nNumModes, &pModes);
		assert(err == noErr);

		uppMLI = NewDMDisplayModeListIteratorUPP(DMModeInfoProc);
		assert(uppMLI);
		sModeInfo.bMatched = false;
		sModeInfo.pMode = mode;
		for (i = 0; i < nNumModes; ++i)
		{
			err = DMGetIndexedDisplayModeFromList(pModes,
				i, NULL, uppMLI, &sModeInfo);
			assert(err == noErr);
			if (sModeInfo.bMatched)
				break;
		}
		if (i == nNumModes)
			return false;

		DisposeDMDisplayModeListIteratorUPP(uppMLI);
		err = DMDisposeList(pModes);
		assert(err == noErr);

		/*
			For the really paranoid -
			 unsigned long flags;
			  Boolean bok;
			 assert(noErr == DMCheckDisplayMode(pThis->mHandle, sMode.csData,
				   sMode.csMode, &flags, NULL, &bok));
			 assert(bok);
		*/
		if (DMBeginConfigureDisplays(&hDisplayState) != noErr)
		{
			/*printf("Could not lock display for display mode changing!");*/
			return false;
		}

		dwBPP = (unsigned long) mode->depth;
		err = DMSetDisplayMode(
			pThis->mHandle, sModeInfo.sMode.csData,
			&dwBPP, NULL, hDisplayState);
		DMEndConfigureDisplays(hDisplayState);
		if (err != noErr)
		{
			/*printf("Could not set the display mode");*/
			return false;
		}
	}
	else  /* DM 1.0, 1.2, 1.x */
	{
		/*printf("Display Manager version %lu not supported or"
			" does not support gravitation! Present? %s\n",
				dwDMVer / 0x10000,
				(dwDMVer & (1 << gestaltDisplayMgrPresent) ? "Yes" : "No"));*/
		return false;
	}

	return true;
}
void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode,
	WSVIDEOMODE** modes, unsigned int* modescount)
{
	unsigned long dwDMVer;

	/*
		Check DM version == 2
		(for backward compatibility only - 7.5.3+ use 2.0)
	*/
	Gestalt(gestaltDisplayMgrVers, (long*)&dwDMVer);
	if (dwDMVer >= 0x020000)
	{
		DMListIndexType nNumModes;
		DMListType pModes;
		DMDisplayModeListIteratorUPP uppMLI;
		DisplayIDType nDisplayID;
		OSErr err;
		DMModeIteratorRec sModeInfo;
		DMListIndexType i;

		err = DMGetDisplayIDByGDevice(pThis->mHandle, &nDisplayID, false);
		assert(err == noErr);

		/* Create a new list... */
		err = DMNewDisplayModeList(nDisplayID, NULL, NULL,
			&nNumModes, &pModes);
		assert(err == noErr);

		uppMLI = NewDMDisplayModeListIteratorUPP(DMModeListIteratorProc);
		assert(uppMLI);

		sModeInfo.pModes = NULL;
		sModeInfo.count = 0;
		sModeInfo.pMatchMode = matchingmode;

		for (i = 0; i < nNumModes; ++i)
		{
			err = DMGetIndexedDisplayModeFromList(pModes,
				i, NULL, uppMLI, &sModeInfo);
			assert(err == noErr);
		}

		DisposeDMDisplayModeListIteratorUPP(uppMLI);
		err = DMDisposeList(pModes);
		assert(err == noErr);

		*modes = sModeInfo.pModes;
		*modescount = sModeInfo.count;
	}
	else /* DM 1.0, 1.2, 1.x */
	{
		/*printf("Display Manager Version %lu Not Supported! Present? %s\n",
			   dwDMVer / 0x10000,
			   (dwDMVer & (1 << gestaltDisplayMgrPresent) ? "Yes" : "No"));*/
		*modes = NULL;
		*modescount = 0;
	}
}
void WSDisplay_ResetMode(const WSDISPLAY* pThis)
{
	WSVIDEOMODE vm;
	WSVideoMode_ConstructDefault(&vm);
	WSDisplay_ChangeMode(pThis, &vm);
}
void WSDisplayGetGeometry(WSRECT* r)
{
	BitMap screenBits;
	GetQDGlobalsScreenBits(&screenBits);
	r->left = 0;
	r->top = 0;
	r->right = screenBits.bounds.right - screenBits.bounds.left;
	r->bottom = screenBits.bounds.bottom - screenBits.bounds.top;
}
void WSDisplayGetClientArea(WSRECT* r)
{
	WSDisplayGetGeometry(r);
	r->top += WSDisplayGetMenuBarHeight();
}
void WSDisplayGetPrimary(WSDISPLAY* pDisplay)
{ pDisplay->mHandle = GetMainDevice(); }
void WSDisplayGetDepth(guipos_t* depth)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	WSDisplay_GetDepth(&d, depth);
	WSDisplay_Destroy(&d);
}
void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth)
{
	GDHandle dev = pThis->mHandle;
	PixMapHandle pm = (*dev)->gdPMap;
	*depth = (*pm)->pixelSize;
}

void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y)
{
	*x = *y = 72;
}
void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale)
{
	*scale = 1.0;
}
size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize)
{
	DisplayIDType did;
	OSErr err;
	err = DMGetDisplayIDByGDevice(pThis->mHandle, &did, false);
	if (err == noErr)
	{
		Str255 name= "\p";
		err = DMGetNameByAVID(did, 0, name);
		if (err == noErr)
		{
			if(buffer && buffersize)
			{
				if(buffersize <= *name)
					--buffersize;
				else
					buffersize = *name;
				memcpy(buffer, name + 1, sizeof(*name) * buffersize);
				buffer[buffersize] = '\0';
			}
			return *name;
		}
	}
	return 0;
}
int WSDisplay_MakePrimary(const WSDISPLAY* pThis)
{
	Handle hDisplayState;
	OSErr err;
	if (DMBeginConfigureDisplays(&hDisplayState) != noErr)
		return false;

	err = DMSetMainDisplay(pThis->mHandle, hDisplayState);
	DMEndConfigureDisplays(hDisplayState);
	return err == noErr;
}
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
			   #######  #####     #     # ### #    # ### #######
			 # #     # #     #    #     #  #  #   #   #     #
			 # #     # #          #     #  #  #  #    #     #
			 # #     #  #####     #     #  #  ###     #     #
			 # #     #       #    #     #  #  #  #    #     #
			 # #     # #     #    #     #  #  #   #   #     #
			 # #######  #####      #####  ### #    # ###    #
=============================================================================*/
#elif defined(WSDISPLAYIOS) && defined(__OBJC__)
#include <UIKit/UIKit.h>
#ifdef __cplusplus
	extern "C" {
#endif
void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis)
{ ++pThis->index;}
void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay)
{}
void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis)
{}

void WSDisplayGetPrimary(WSDISPLAY* pDisplay)
{}
void WSDisplayBegin(WSDISPLAYITERATOR* pIt)
{ pIt->index = 0; }
wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt)
{ return pIt->index > 0; }
wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
{ return FALSE; }
wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
{ return FALSE; }
wsbool WSDisplayGetFromWindow(void* windowhandle, WSDISPLAY* pDisplay)
{ return FALSE; }
void WSDisplayGetGeometry(WSRECT* r)
{
	CGRect bounds = [[UIScreen mainScreen] bounds];
	r->origin.x = r->origin.y = 0;
	if(UIInterfaceOrientationIsPortrait([[UIApplication
		sharedApplication] statusBarOrientation]))
	{
		r->size.width = bounds.size.width;
		r->size.height = bounds.size.height;
	}
	else
	{
		r->size.width = bounds.size.height;
		r->size.height = bounds.size.width;
	}
}
void WSDisplayGetClientArea(WSRECT* r)
{ WSDisplayGetGeometry(r); }
void WSDisplayGetDepth(guipos_t* depth)
{ *depth = 32; /* All iOS devices run at 32bpp per spec */ }
void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r)
{ WSDisplayGetGeometry(r); }
void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r)
{ WSDisplayGetGeometry(r); }
wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis)
{ return FALSE; }
void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	if(@available(iOS 3.2, *))
	{
		int depth;
		long refreshrate;
		if(@available(iOS 10.3, *))
			refreshrate = [[UIScreen mainScreen] maximumFramesPerSecond];
		else
			refreshrate = 60;
		WSDisplayGetDepth(&depth);
		WSVideoMode_Construct(mode,
			[[UIScreen mainScreen] currentMode].size.width,
			[[UIScreen mainScreen] currentMode].size.height,
			depth, (int)refreshrate);
	}
	else
		WSVideoMode_ConstructDefault(mode);
}
wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	NSArray<UIScreenMode*>* availmodes;
	CFIndex i;
	int depth;
	long refreshrate;
	if(@available(iOS 10.3, *))
		refreshrate = [[UIScreen mainScreen] maximumFramesPerSecond];
	else
		refreshrate = 60;
	WSDisplayGetDepth(&depth);
	if(@available(iOS 3.2, *)) {}
	else
		return FALSE;
	availmodes = [[UIScreen mainScreen] availableModes];
	for(i = 0; i < [availmodes count]; ++i)
	{
		WSVIDEOMODE curmode;
		WSVideoMode_Construct(&curmode,
			availmodes[i].size.width,
			availmodes[i].size.height, depth, (int)refreshrate);
		if (WSVideoMode_Matches(&curmode, mode))
		{
			[[UIScreen mainScreen] setCurrentMode:availmodes[i]];
			return TRUE;
		}
	}
	return FALSE;

}
void WSDisplay_ResetMode(const WSDISPLAY* pThis)
{
	[[UIScreen mainScreen] setCurrentMode:
			[[UIScreen mainScreen] preferredMode]
	];
}
void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode,
	WSVIDEOMODE** modes, unsigned int* modescount)
{
	NSArray<UIScreenMode*>* availmodes;
	CFIndex i;
	int depth;
	long refreshrate;
	if(@available(iOS 10.3, *))
		refreshrate = [[UIScreen mainScreen] maximumFramesPerSecond];
	else
		refreshrate = 60;
	*modescount = 0;
	if(modes)
		*modes = NULL;
	WSDisplayGetDepth(&depth);
	if(@available(iOS 3.2, *)) {}
	else
		return;
	availmodes = [[UIScreen mainScreen] availableModes];
	for(i = 0; i < [availmodes count]; ++i)
	{
		WSVIDEOMODE curmode;
		WSVideoMode_Construct(&curmode,
			availmodes[i].size.width,
			availmodes[i].size.height, depth, (int)refreshrate);
		if (WSVideoMode_Matches(&curmode, matchingmode))
		{
			++*modescount;
			if(modes)
			{
				WSVIDEOMODE* newmodes = WSREALLOC(*modes,
					WSVIDEOMODE, *modescount);
				if(newmodes)
				{
					newmodes[*modescount - 1] = curmode;
					*modes = newmodes;
				}
			}
		}
	}
}
void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y)
{
	float scale;
	WSDisplay_GetScaleFactor(pThis, &scale);
	*x = 72 * scale;
	*y = 72 * scale;
}
void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale)
{
	if(@available(iOS 4.0, *))
		*scale = [[UIScreen mainScreen] scale];
	else
		*scale = 1.0;
}
void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth)
{ (void)(pThis); WSDisplayGetDepth(depth); }
void WSDisplay_Destroy(WSDISPLAY* pThis)
{}
size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize)
{ return 0; }
int WSDisplay_MakePrimary(const WSDISPLAY* pThis)
{ return 0; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
 #######  #####  #     #     #####  #     #    #    ######  ####### #######
 #     # #     #  #   #     #     # #     #   # #   #     #    #         #
 #     # #         # #      #     # #     #  #   #  #     #    #        #
 #     #  #####     #       #     # #     # #     # ######     #       #
 #     #       #   # #      #   # # #     # ####### #   #      #      #
 #     # #     #  #   #     #    #  #     # #     # #    #     #     #
 #######  #####  #     #     #### #  #####  #     # #     #    #    #######
=============================================================================*/
#elif defined(__MACH__) && !defined(WSDISPLAYIOS) /* _WIN32 */
#include <mach-o/dyld.h> /* NSIsSymbolNameDefined et al. */
#include <CoreGraphics/CoreGraphics.h>
#include <IOKit/graphics/IOGraphicsLib.h> /* IODisplayCreateInfoDictionary */
#ifdef __cplusplus
	extern "C" {
#endif
/* CGDisplayBitsPerPixel isn't in newer headers, it will link though */
size_t CGDisplayBitsPerPixel(CGDirectDisplayID display);
#if defined(__OBJC__)
	#import <Cocoa/Cocoa.h>
	NSScreen* WSDisplayScreenFromDisplay(CGDirectDisplayID desid)
	{
		for (NSScreen* screen in [NSScreen screens])
		{
			CGDirectDisplayID curid = [[[screen deviceDescription]
				objectForKey:@"NSScreenNumber"] intValue];
			if (desid == curid)
				return screen;
		}
		return NULL;
	}
#endif
io_service_t IOServicePortFromCGDisplayID(CGDirectDisplayID displayID)
{
	io_iterator_t iter;
	io_service_t serv, servicePort;
	kern_return_t err;
	CFMutableDictionaryRef matching;
	matching = IOServiceMatching("IODisplayConnect");
	if(!matching)
		return 0;
	err = IOServiceGetMatchingServices(kIOMasterPortDefault,
		matching /* WARNING: call releases! */, &iter);
	if (err)
		return 0;

	servicePort = 0;
	while ((serv = IOIteratorNext(iter)) != 0)
	{
		CFDictionaryRef info;
		CFIndex vendorID, productID, serialNumber;
		CFNumberRef vendorIDRef, productIDRef, serialNumberRef;
		Boolean success;

		info = IODisplayCreateInfoDictionary(serv,
											 kIODisplayOnlyPreferredName);
		vendorIDRef = (CFNumberRef)CFDictionaryGetValue(info,
										   CFSTR(kDisplayVendorID));
		productIDRef = (CFNumberRef)CFDictionaryGetValue(info,
											CFSTR(kDisplayProductID));
		serialNumberRef = (CFNumberRef)CFDictionaryGetValue(info,
											   CFSTR(kDisplaySerialNumber));
		success = CFNumberGetValue(vendorIDRef, kCFNumberCFIndexType,
								   &vendorID);
		success &= CFNumberGetValue(productIDRef, kCFNumberCFIndexType,
									&productID);
		success &= CFNumberGetValue(serialNumberRef, kCFNumberCFIndexType,
									&serialNumber);
		if (!success)
		{
			CFRelease(info);
			continue;
		}
		if (CGDisplayVendorNumber(displayID) != vendorID  ||
			CGDisplayModelNumber(displayID) != productID  ||
			CGDisplaySerialNumber(displayID) != serialNumber)
		{
			CFRelease(info);
			continue;
		}

		servicePort = serv;
		CFRelease(info);
		break;
	}
	IOObjectRelease(iter);
	return servicePort;
}
int WSDisplayCFDictKeyToInt(CFDictionaryRef desc, CFStringRef key)
{
	CFNumberRef value = (CFNumberRef) CFDictionaryGetValue(desc, key);
	if (value == NULL)
		return 0;
	int num = 0;
	CFNumberGetValue(value, kCFNumberIntType, &num);
	return num;
}

void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis)
{ ++pThis->index; }
void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay)
{ pDisplay->mHandle = pThis->displays[pThis->index]; }
void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis)
{ WSFREE(pThis->displays); }

void WSDisplayBegin(WSDISPLAYITERATOR* pIt)
{
	CGDisplayErr error;
	CGDisplayCount onlineCount;

	pIt->index = 0; pIt->count = 0;
	if(NSIsSymbolNameDefined("_CGGetOnlineDisplayList")) /* 10.2+ */
	{
		typedef CGDirectDisplayID (*CGDisplayMirrorsDisplayProc)(
			CGDirectDisplayID);
		typedef CGDisplayErr (*CGGetOnlineDisplayListProc)(
			CGDisplayCount, CGDirectDisplayID*, CGDisplayCount*);

		CGGetOnlineDisplayListProc pfnCGGetOnlineDisplayList =
			(CGGetOnlineDisplayListProc) NSAddressOfSymbol(
				NSLookupAndBindSymbol("_CGGetOnlineDisplayList"));
		CGDisplayMirrorsDisplayProc pfnCGDisplayMirrorsDisplay =
			(CGDisplayMirrorsDisplayProc) NSAddressOfSymbol(
				NSLookupAndBindSymbol("_CGDisplayMirrorsDisplay"));

		error = (*pfnCGGetOnlineDisplayList)(0,NULL,&onlineCount);
		if (error == kCGErrorSuccess && onlineCount > 0)
		{
			pIt->displays = WSALLOC(CGDirectDisplayID, onlineCount);
			if(!pIt->displays)
				return;
			error = (*pfnCGGetOnlineDisplayList)(onlineCount,
				pIt->displays, &onlineCount);
			if (error == kCGErrorSuccess)
			{
				CGDisplayCount i;
				for (i = 0; i < onlineCount; ++i)
				{
					if ((*pfnCGDisplayMirrorsDisplay)(pIt->displays[i]) !=
								kCGNullDirectDisplay)
						continue;
					pIt->displays[pIt->count] = pIt->displays[i];
					pIt->count += 1;
				}
			}
		}
	}
	else
	{
		error = CGGetActiveDisplayList(0,NULL,&pIt->count);
		if (error == kCGErrorSuccess && pIt->count > 0)
		{
			pIt->displays = WSALLOC(CGDirectDisplayID, pIt->count);
			if(!pIt->displays)
			{
				pIt->count = 0;
				return;
			}
			if(CGGetActiveDisplayList(pIt->count,
					pIt->displays, &pIt->count) != kCGErrorSuccess)
				pIt->count = 0;
		}
	}
}
wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt)
{ return pIt->index >= pIt->count; }

void WSDisplayGetPrimary(WSDISPLAY* pDisplay)
{ pDisplay->mHandle = kCGDirectMainDisplay; }
wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
{
	guipos_t x = r->origin.x + ((r->size.width) / 2);
	guipos_t y = r->origin.y + ((r->size.height) / 2);
	return WSDisplayGetFromPoint(x, y, pDisplay);
}
wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
{
	CGPoint thePoint = {(float)x, (float)y};
	CGDirectDisplayID theID;
	CGDisplayCount theCount;
	CGDisplayErr err = CGGetDisplaysWithPoint(thePoint, 1, &theID, &theCount);
	if(err != CGDisplayNoErr)
		return FALSE;
	pDisplay->mHandle = theID;
	return TRUE;
}
wsbool WSDisplayGetFromWindow(void* windowhandle, WSDISPLAY* pDisplay)
{ return FALSE; }
void WSDisplayGetGeometry(WSRECT* r)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	WSDisplay_GetGeometry(&d, r);
	WSDisplay_Destroy(&d);
}
void WSDisplayGetClientArea(WSRECT* r)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	WSDisplay_GetClientArea(&d, r);
	WSDisplay_Destroy(&d);
}
void WSDisplayGetDepth(guipos_t* depth)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	WSDisplay_GetDepth(&d, depth);
	WSDisplay_Destroy(&d);
}
void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth)
{
	if(NSIsSymbolNameDefined("_CGDisplayCopyDisplayMode")) /* 10.6+ */
	{
		typedef struct CGDisplayMode* CGDisplayModeRef;
		typedef CGDisplayModeRef (*CGDisplayCopyDisplayModeProc)(
			CGDirectDisplayID);
		typedef CFStringRef (*CGDisplayModeCopyPixelEncodingProc)(
			CGDisplayModeRef);
		typedef void (*CGDisplayModeReleaseProc)(
			CGDisplayModeRef);

		CGDisplayCopyDisplayModeProc pfnCGDisplayCopyDisplayMode =
			(CGDisplayCopyDisplayModeProc) NSAddressOfSymbol(
				NSLookupAndBindSymbol("_CGDisplayCopyDisplayMode"));
		CGDisplayModeCopyPixelEncodingProc pfnCGDisplayModeCopyPixelEncoding =
			(CGDisplayModeCopyPixelEncodingProc) NSAddressOfSymbol(
				NSLookupAndBindSymbol("_CGDisplayModeCopyPixelEncoding"));
		CGDisplayModeReleaseProc pfnCGDisplayModeRelease =
			(CGDisplayModeReleaseProc) NSAddressOfSymbol(
				NSLookupAndBindSymbol("_CGDisplayModeRelease"));

		CGDisplayModeRef dm =
			(*pfnCGDisplayCopyDisplayMode)(pThis->mHandle);
		CFStringRef de = (*pfnCGDisplayModeCopyPixelEncoding)(dm);
		if(de)
		{
			if(CFStringCompare(de, CFSTR(IO32BitDirectPixels),
					kCFCompareCaseInsensitive) == kCFCompareEqualTo)
				*depth = 32;
			else if(CFStringCompare(de, CFSTR(IO16BitDirectPixels),
					kCFCompareCaseInsensitive) == kCFCompareEqualTo)
				*depth = 16;
			else if(CFStringCompare(de, CFSTR(IO8BitIndexedPixels),
					kCFCompareCaseInsensitive) == kCFCompareEqualTo)
				*depth = 8;
			else
				*depth = CGDisplayBitsPerPixel(pThis->mHandle);
			CFRelease(de);
		}
		else
			*depth = CGDisplayBitsPerPixel(pThis->mHandle);

		(*pfnCGDisplayModeRelease)(dm);
	}
	else
		*depth = CGDisplayBitsPerPixel(pThis->mHandle);
}
void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r)
{
	CGRect theRect = CGDisplayBounds(pThis->mHandle);
	r->origin.x = theRect.origin.x;
	r->origin.y = theRect.origin.y;
	r->size.width = theRect.size.width;
	r->size.height = theRect.size.height;
}
void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r)
{
#if defined(__OBJC__)
	NSRect displayRect =
		[WSDisplayScreenFromDisplay(pThis->mHandle) visibleFrame];
	r->size.width = displayRect.size.width;
	r->size.height = displayRect.size.height;
	r->origin.x = displayRect.origin.x;
	r->origin.y = displayRect.origin.y;
#else
	WSDisplay_GetGeometry(pThis, r);
#endif
}
wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis)
{ return pThis->mHandle == kCGDirectMainDisplay; }
void WSDisplay_ResetMode(const WSDISPLAY* pThis)
{
	WSVIDEOMODE vm;
	WSVideoMode_ConstructDefault(&vm);
	WSDisplay_ChangeMode(pThis, &vm);
}
void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	CFDictionaryRef theValue = CGDisplayCurrentMode(pThis->mHandle);
	WSVideoMode_Construct(mode,
		WSDisplayCFDictKeyToInt(theValue, kCGDisplayWidth),
		WSDisplayCFDictKeyToInt(theValue, kCGDisplayHeight),
		WSDisplayCFDictKeyToInt(theValue, kCGDisplayBitsPerPixel),
		WSDisplayCFDictKeyToInt(theValue, kCGDisplayRefreshRate));
}
wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	boolean_t bExactMatch;
	CFDictionaryRef dm;
	if(WSVideoMode_IsDefault(mode) /* 10.2+ */
		&& NSIsSymbolNameDefined("_CGRestorePermanentDisplayConfiguration"))
	{
		typedef void (*CGRestorePermanentDisplayConfigurationProc)();

		CGRestorePermanentDisplayConfigurationProc
			pfnCGRestorePermanentDisplayConfiguration =
			(CGRestorePermanentDisplayConfigurationProc) NSAddressOfSymbol(
				NSLookupAndBindSymbol(
					"_CGRestorePermanentDisplayConfiguration"));
		(*pfnCGRestorePermanentDisplayConfiguration)();
		return TRUE;
	}
	dm = CGDisplayBestModeForParametersAndRefreshRate(
		pThis->mHandle, (size_t)mode->depth, (size_t)mode->width,
		(size_t)mode->height, (double)mode->refreshrate, &bExactMatch);
	if (mode && bExactMatch)
		return CGDisplaySwitchToMode(pThis->mHandle, dm) == CGDisplayNoErr;
	return FALSE;
}
void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode,
	WSVIDEOMODE** modes, unsigned int* modescount)
{
	CFArrayRef theArray = CGDisplayAvailableModes(pThis->mHandle);
	CFIndex i, count = CFArrayGetCount(theArray);
	*modescount = 0;
	if(modes)
		*modes = NULL;
	for (i = 0; i < count; ++i)
	{
		CFDictionaryRef theValue = (CFDictionaryRef)
			CFArrayGetValueAtIndex(theArray, i);
		WSVIDEOMODE curmode;
		WSVideoMode_Construct(&curmode,
			WSDisplayCFDictKeyToInt(theValue, kCGDisplayWidth),
			WSDisplayCFDictKeyToInt(theValue, kCGDisplayHeight),
			WSDisplayCFDictKeyToInt(theValue, kCGDisplayBitsPerPixel),
			WSDisplayCFDictKeyToInt(theValue, kCGDisplayRefreshRate));
		if (WSVideoMode_Matches(&curmode, matchingmode))
		{
			++*modescount;
			if(modes)
			{
				WSVIDEOMODE* newmodes = WSREALLOC(*modes,
					WSVIDEOMODE, *modescount);
				if(newmodes)
				{
					newmodes[*modescount - 1] = curmode;
					*modes = newmodes;
				}
			}
		}
	}
	CFRelease(theArray);
}
void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y)
{
	size_t pixelsWide, pixelsHigh;
	CGRect mainScreenBounds;
	CGFloat pointsWide, pointsHigh;
	/* Get the physical pixel dimensions of the display */
	pixelsWide = CGDisplayPixelsWide(pThis->mHandle);
	pixelsHigh = CGDisplayPixelsHigh(pThis->mHandle);
	/* Get the display size in points */
	/* Note that CGFloat did not exist in early headers */
	mainScreenBounds = CGDisplayBounds(pThis->mHandle);
	pointsWide = CGRectGetWidth(mainScreenBounds);
	pointsHigh = CGRectGetHeight(mainScreenBounds);
	/* Calculate the DPI for each axis (72 points per inch) */
	*x = (guipos_t)((pixelsWide / pointsWide) * 72);
	*y = (guipos_t)((pixelsHigh / pointsHigh) * 72);
}
void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale)
{
	/*
		10.6+
		CGDisplayModeRef dm = CGDisplayCopyDisplayMode(pThis->mHandle);
		size_t w = CGDisplayModeGetWidth(dm);
		size_t pixw = CGDisplayModeGetPixelWidth(dm);
		CGDisplayModeRelease(dm);
		return ((double)pixw) / w;
	*/
	guipos_t x, y;
	WSDisplay_GetPPI(pThis, &x, &y);
	x /= 72;
	y /= 72;
	*scale = (x + y) / 2;
}
void WSDisplay_Destroy(WSDISPLAY* pThis)
{}

size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize)
{
	char* name;
	CFDictionaryRef info, names;
	CFStringRef value;
	CFIndex size;
	io_service_t serv;
	serv = IOServicePortFromCGDisplayID(pThis->mHandle);
	if (serv == 0)
		return 0;

	info = IODisplayCreateInfoDictionary(serv, kIODisplayOnlyPreferredName);
	IOObjectRelease(serv);

	names = (CFDictionaryRef)
		CFDictionaryGetValue(info, CFSTR(kDisplayProductName));

	if (!names || !CFDictionaryGetValueIfPresent(names, CFSTR("en_US"),
												 (const void**) &value))
		return 0;

	size = CFStringGetMaximumSizeForEncoding(CFStringGetLength(value),
											 kCFStringEncodingUTF8);

	if(buffer && buffersize)
	{
		if(buffersize > size)
			buffersize = size;
		CFStringGetCString(value, name, buffersize, kCFStringEncodingUTF8);
	}

	CFRelease(info);
	return size;
}

int WSDisplay_MakePrimary(const WSDISPLAY* pThis)
{
	CGDisplayConfigRef config;
	CGBeginDisplayConfiguration(&config);
	CGConfigureDisplayOrigin(config, pThis->mHandle, 0, 0);
	CGCompleteDisplayConfiguration(config, kCGConfigurePermanently);
	return TRUE;
}

#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
		 #     #   #     #      #     # #######  #####   #####
		  #   #   ##    ##       #   #  #       #     # #     #
		   # #   # #   # #        # #   #       #     # #
			#      #     #         #    #####    #####  ######
		   # #     #     #        # #   #       #     # #     #
		  #   #    #     #       #   #  #       #     # #     #
		 #     # ##### #####    #     # #        #####   #####
=============================================================================*/
#elif !defined(HAVE_XRANDR) && (defined(__FreeBSD__) \
	|| defined(__DragonFly__) || defined(__OpenBSD__) || defined(__linux__))
#ifdef __cplusplus
	extern "C" {
#endif
#if defined(QT_VERSION)
	#include <QX11Info>
#endif
#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>
#include <X11/Xatom.h> /* XA_CARDINAL */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
/* XineramaScreenInfo is an anonymous struct and cannot be forward declared
	i.e. typedef struct { ... } XineramaScreenInfo;
	Nothing after struct == anonymous */
#define WSDisplayIterator_ScreenInfos(pThis) \
	(*((XineramaScreenInfo**)(&(pThis)->mScreenInfos)))
#if !defined(__cplusplus)
	#define c_private private
	#define c_class class
#endif
WSDISPLAYHANDLE WSX11MainDisplay()
{
#if defined(QT_VERSION)
	return QX11Info::display();
#elif defined(GDK_MAJOR_VERSION)
	return GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
#else
	return XOpenDisplay(NULL);
#endif
}
void WSX11CloseMainDisplay(WSDISPLAYHANDLE display)
{
#if !defined(QT_VERSION) && !defined(GDK_MAJOR_VERSION)
	XCloseDisplay(display);
#endif
}
void WSDisplayGetPrimary(WSDISPLAY* pDisplay)
{
	pDisplay->mHandle = WSX11MainDisplay();
	pDisplay->mScreenNum = DefaultScreen(pDisplay->mHandle);
}
void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis)
{
	assert(pThis->mIndex != pThis->mNumScreens);
	++pThis->mIndex;
}
void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay)
{
	pDisplay->mHandle = pThis->mDisplay;
	pDisplay->mScreenNum =
		WSDisplayIterator_ScreenInfos(pThis)[pThis->mIndex].screen_number;
}
void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis)
{
	XFree(WSDisplayIterator_ScreenInfos(pThis));
	WSX11CloseMainDisplay(pThis->mDisplay);
}

void WSDisplayBegin(WSDISPLAYITERATOR* pIt)
{
	pIt->mDisplay = WSX11MainDisplay();
	assert(XineramaIsActive(pIt->mDisplay));
	WSDisplayIterator_ScreenInfos(pIt) = XineramaQueryScreens(
		pIt->mDisplay, &pIt->mNumScreens);
	pIt->mIndex = 0;
}
wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt)
{
	return pIt->mIndex == pIt->mNumScreens;
}

wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
{
	wsbool res = FALSE;
	WSDISPLAYITERATOR it;
	WSDisplayBegin(&it);
	for (; !WSDisplayIsEnd(&it); WSDisplayIterator_Iterate(&it))
	{
		const XineramaScreenInfo* si =
			&WSDisplayIterator_ScreenInfos(&it)[it.mIndex];
		if (x >= si->x_org && x < si->x_org + si->width
				&& y >= si->y_org && y < si->y_org + si->height)
		{
			WSDisplayIterator_Value(&it, pDisplay);
			res = TRUE;
		}
	}
	WSDisplayIterator_Destroy(&it);
	return res;
}
wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
{
	guipos_t x = r->left + ((r->right - r->left) / 2);
	guipos_t y = r->top + ((r->bottom - r->top) / 2);
	return WSDisplayGetFromPoint(x, y, pDisplay);
}
wsbool WSDisplayGetFromWindow(void* windowhandle, WSDISPLAY* pDisplay);
void WSDisplayX11GetGeometry_(WSDISPLAYHANDLE maindisplay,
	int mainscreenidx, WSRECT* r)
{
	r->top = 0; r->left = 0;
	r->right = DisplayWidth(maindisplay, mainscreenidx);
	r->bottom = DisplayHeight(maindisplay, mainscreenidx);
}
void WSDisplayGetGeometry(WSRECT* r)
{
	WSDISPLAYHANDLE maindisplay = WSX11MainDisplay();
	int mainscreenidx = DefaultScreen(maindisplay);
	WSDisplayX11GetGeometry_(maindisplay, mainscreenidx, r);
	WSX11CloseMainDisplay(maindisplay);
}
void WSDisplayX11GetNetWorkArea(WSDISPLAYHANDLE display,
	int screennum, WSRECT* r)
{
	Atom prop = XInternAtom(display, "_NET_WORKAREA", TRUE);
	if (prop)
	{
		Atom actual_type;
		int actual_format;
		unsigned long nitems;
		unsigned long bytes_after;
		unsigned char* data = NULL;
		Status status = XGetWindowProperty(
			display, RootWindow(display, screennum), prop,
			0, 4, FALSE, XA_CARDINAL,
			&actual_type, &actual_format, &nitems, &bytes_after, &data);
		if (status == Success && actual_type == XA_CARDINAL
			&& actual_format == 32 && nitems == 4)
		{
			const long* p = (const long*)data;
			r->left = p[0];
			r->top = p[1];
			r->right = p[2] - r->left;
			r->bottom = p[3] - r->top;
		}
		else
		{
			r->left = r->top = r->bottom = r->right = 0;
		}
		if (data)
			XFree(data);
	}
	else
	{
		r->left = r->top = r->bottom = r->right = 0;
	}
}
void WSDisplayX11GetClientArea_(WSDISPLAYHANDLE maindisplay,
	int screennum, WSRECT* r)
{
	WSRECT rgeo;
	WSDisplayGetGeometry(&rgeo);
	WSDisplayX11GetNetWorkArea(maindisplay, screennum, r);

	if ( !r->right || !r->bottom )
		*r = rgeo; /* WSDisplayX11GetNetWorkArea failed, use full area */
	else
	{ /* Force the client area into the full area */
		if(r->right > rgeo.right)
			r->right = rgeo.right;
		if(r->left < rgeo.left)
			r->left = rgeo.left;
		if(r->bottom > rgeo.bottom)
			r->bottom = rgeo.bottom;
		if(r->top < rgeo.top)
			r->top = rgeo.top;
	}
}
void WSDisplayGetClientArea(WSRECT* r)
{
	WSDISPLAYHANDLE maindisplay = WSX11MainDisplay();
	WSDisplayX11GetClientArea_(maindisplay, DefaultScreen(maindisplay), r);
	WSX11CloseMainDisplay(maindisplay);
}
guipos_t WSDisplayCountSetBits_(unsigned long n)
{
	guipos_t count = 0;
	while (n)
	{
		count += n & 1;
		n >>= 1;
	}
	return count;
}
guipos_t WSDisplayX11GetDepth_(WSDISPLAYHANDLE display, int screen)
{
	guipos_t depth = DefaultDepth(display, screen);
	Visual *visual = DefaultVisual(display, screen);
	if (visual->c_class == TrueColor || visual->c_class == DirectColor)
	{ /* Count bits in each color mask to get the bits per pixel */
		depth = WSDisplayCountSetBits_(visual->red_mask)
					   + WSDisplayCountSetBits_(visual->green_mask)
					   + WSDisplayCountSetBits_(visual->blue_mask);
	}
	else
	{ /* For other visual classes, calculate bits per pixel by
	  counting all set bits in combined color masks */
		unsigned long combinedMask = visual->red_mask
			| visual->green_mask | visual->blue_mask;
		depth = WSDisplayCountSetBits_(combinedMask);
	}
	return depth;
}
void WSDisplayGetDepth(guipos_t* depth)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	WSDisplay_GetDepth(&d, depth);
	WSDisplay_Destroy(&d);
}
void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth)
{
	*depth = WSDisplayX11GetDepth_(pThis->mHandle, pThis->mScreenNum);
}
void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r)
{
	WSDisplayX11GetGeometry_(pThis->mHandle, pThis->mScreenNum, r);
}
void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r)
{
	WSDisplayX11GetClientArea_(pThis->mHandle, pThis->mScreenNum, r);
}
wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis)
{
	return DefaultScreen(pThis->mHandle) == pThis->mScreenNum;
}
/*
			XF86
			Get/Set Resolutions available from 0.6
			(XF86VidModeQueryVersion(dpy, &maj, &min))
*/
#include <X11/extensions/xf86vmode.h>
int XF86InfoConvertRefreshRate(int htotal, int vtotal, int dotclock)
{
	float pixelspersecond, pixelsperframe;
	pixelspersecond = (1000.0f * (float)dotclock);
	pixelsperframe = ((float)(htotal * vtotal));
	return (int)((pixelspersecond / pixelsperframe) + 0.5f);
}
void WSVideoMode_ConstructWithXF86Info(WSVIDEOMODE* pThis,
	int hdisplay, int vdisplay, int depth,
	int htotal, int vtotal, int dotclock)
{
	pThis->width = hdisplay;
	pThis->height = vdisplay;
	pThis->depth = depth;
	pThis->refreshrate = XF86InfoConvertRefreshRate(htotal, vtotal, dotclock);
}
void XF86VidModeModeLine_Destruct(XF86VidModeModeLine* pThis)
{
	if(pThis->privsize)
		XFree(pThis->c_private);
}
void XF86VidModeModeInfo_Destruct(XF86VidModeModeInfo* pThis)
{
	if(pThis->privsize)
		XFree(pThis->c_private);
}

void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
  XF86VidModeModeLine ml;
  int dotclock;
  if (!XF86VidModeGetModeLine(pThis->mHandle,
	  pThis->mScreenNum, &dotclock, &ml))
  {
	  WSVideoMode_ConstructDefault(mode);
	  return;
  }
  XF86VidModeModeLine_Destruct(&ml);
  return WSVideoMode_ConstructWithXF86Info(mode, ml.hdisplay,
	  ml.vdisplay,
	  WSDisplayX11GetDepth_(pThis->mHandle, pThis->mScreenNum),
	  ml.htotal, ml.vtotal, dotclock);
}
wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	XF86VidModeModeInfo** ppmi;
	int imode, nummodes;
	wsbool ret;
	if (!XF86VidModeGetAllModeLines(pThis->mHandle, pThis->mScreenNum,
			&nummodes, &ppmi))
		return FALSE;

	ret = FALSE;
	if (WSVideoMode_IsDefault(mode))
	{
		ret = XF86VidModeSwitchToMode(pThis->mHandle, pThis->mScreenNum,
			ppmi[0]) != 0;
		for (imode = 0; imode < nummodes; ++imode)
			XF86VidModeModeInfo_Destruct(ppmi[imode]);
	}
	else
	{
		for (imode = 0; imode < nummodes; ++imode)
		{
			if (!ret &&
					ppmi[imode]->hdisplay == mode->width &&
					ppmi[imode]->vdisplay == mode->height &&
					XF86InfoConvertRefreshRate(ppmi[imode]->htotal,
						ppmi[imode]->vtotal, ppmi[imode]->dotclock)
						== mode->refreshrate)
				ret = XF86VidModeSwitchToMode(
					pThis->mHandle, pThis->mScreenNum, ppmi[imode]) != 0;
			XF86VidModeModeInfo_Destruct(ppmi[imode]);
		}
	}

	XFree(ppmi);
	return ret;
}
void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode,
	WSVIDEOMODE** modes, unsigned int* modescount)
{
	XF86VidModeModeInfo** ppmi;
	int nummodes;
	if (XF86VidModeGetAllModeLines(pThis->mHandle, pThis->mScreenNum,
		&nummodes, &ppmi))
	{
		int imode;
		int depth = WSDisplayX11GetDepth_(pThis->mHandle, pThis->mScreenNum);
		*modescount = 0;
		if(modes)
			*modes = NULL;
		for(imode = 0; imode < nummodes; ++imode)
		{
			XF86VidModeModeInfo* pmi = ppmi[imode];
			WSVIDEOMODE curmode;
			WSVideoMode_ConstructWithXF86Info(&curmode, pmi->hdisplay,
				pmi->vdisplay, depth,
				pmi->htotal, pmi->vtotal, pmi->dotclock);
			if (WSVideoMode_Matches(&curmode, matchingmode))
			{
				++*modescount;
				if(modes)
				{
					WSVIDEOMODE* newmodes = WSREALLOC(*modes,
						WSVIDEOMODE, *modescount);
					if(newmodes)
					{
						newmodes[*modescount - 1] = curmode;
						*modes = newmodes;
					}
				}
			}
			XF86VidModeModeInfo_Destruct(pmi);
		}
		XFree(ppmi);
	}
	else
	{
		*modes = NULL;
		*modescount = 0;
	}
}

size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize)
{
	size_t size = 0;
	XF86VidModeMonitor monitor;
	if (XF86VidModeGetMonitor(pThis->mHandle, pThis->mScreenNum, &monitor))
	{
		if(monitor.model)
		{
			size = strlen(monitor.model);
			if(buffer && buffersize)
			{
				if(buffersize <= size)
					--buffersize;
				else
					buffersize = size;
				memcpy(buffer, monitor.model, sizeof(char) * buffersize);
				buffer[buffersize] = '\0';
			}
		}
		XFree(monitor.vendor);
		XFree(monitor.model);
		XFree(monitor.hsync);
		XFree(monitor.vsync);
	}
	return size;
 }
int WSDisplay_MakePrimary(const WSDISPLAY* pThis)
{
	return 0;
}
/*
			END XF86
*/
void WSDisplay_ResetMode(const WSDISPLAY* pThis) /* default video mode */
{
	WSVIDEOMODE vm;
	WSVideoMode_ConstructDefault(&vm);
	WSDisplay_ChangeMode(pThis, &vm);
}
void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y)
{
	WSDISPLAYHANDLE dpy = pThis->mHandle;
	int scr = pThis->mScreenNum;
	double xres = ((((double) DisplayWidth(dpy, scr)) * 25.4) /
		((double) DisplayWidthMM(dpy, scr)));
	double yres = ((((double) DisplayHeight(dpy, scr)) * 25.4) /
		((double) DisplayHeightMM(dpy, scr)));
	*x = (int)(xres + 0.5);
	*y = (int)(yres + 0.5);
}
void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale)
{
	WSDISPLAYHANDLE dpy = pThis->mHandle;
	int scr = pThis->mScreenNum;
	double xres = ((((double) DisplayWidth(dpy, scr)) * 25.4) /
		((double) DisplayWidthMM(dpy, scr)));
	double yres = ((((double) DisplayHeight(dpy, scr)) * 25.4) /
		((double) DisplayHeightMM(dpy, scr)));
	*scale = (float)(((xres + yres) / 2) / 96.0);
}
void WSDisplay_Destroy(WSDISPLAY* pThis)
{
	WSX11CloseMainDisplay(pThis->mHandle);
}
#if !defined(__cplusplus)
	#undef c_private
	#undef c_class
#endif
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
	 #     #   #     #      #     #
	  #   #   ##    ##       #   #  #####    ##   #    # #####  #####
	   # #   # #   # #        # #   #    #  #  #  ##   # #    # #    #
		#      #     #         #    #    # #    # # #  # #    # #    #
	   # #     #     #        # #   #####  ###### #  # # #    # #####
	  #   #    #     #       #   #  #   #  #    # #   ## #    # #   #
	 #     # ##### #####    #     # #    # #    # #    # #####  #    #
=============================================================================*/
#elif defined(HAVE_XRANDR) && (defined(__FreeBSD__) \
	|| defined(__DragonFly__) || defined(__OpenBSD__) || defined(__linux__))
#ifdef __cplusplus
	extern "C" {
#endif
#if defined(QT_VERSION)
	#include <QX11Info>
#endif
#include <X11/Xlib.h>
#include <X11/Xatom.h> /* XA_CARDINAL */
#include <X11/extensions/Xrandr.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#if !defined(__cplusplus)
	#define c_private private
	#define c_class class
#endif

/*------------------------------UTILITIES------------------------------------*/
Display* WSX11MainDisplay()
{
#if defined(QT_VERSION)
	return QX11Info::display();
#elif defined(GDK_MAJOR_VERSION)
	return GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
#else
	return XOpenDisplay(NULL);
#endif
}

void WSX11CloseMainDisplay(Display* display)
{
#if !defined(QT_VERSION) && !defined(GDK_MAJOR_VERSION)
	XCloseDisplay(display);
#endif
}

int X11ConvertRefreshRate(int dotclock, int htotal, int vtotal)
{
	float pixelspersecond, pixelsperframe;
	pixelspersecond = (float)dotclock;
	pixelsperframe = ((float)(htotal * vtotal));
	return (int)((pixelspersecond / pixelsperframe) + 0.5f);
}

int X11ScreenNumFromRROutput(Display* dpy, RROutput output)
{
	int screennum = 0; /* default to 0 */
	/* Find the X11 screen number associated with the Xrandr output */
	int numx11screens = ScreenCount(dpy);
	for (int i = 0; i < numx11screens; ++i)
	{
		Window root = RootWindow(dpy, i);
		XRRScreenResources *resources = XRRGetScreenResources(
			dpy, root);
		XRROutputInfo *outputinfo = XRRGetOutputInfo(dpy,
			resources, output);
		if (outputinfo)
		{
			screennum = i;
			XRRFreeOutputInfo(outputinfo);
			break;
		}
		XRRFreeScreenResources(resources);
	}
	return screennum;
}

/*------------------------------IMPLEMENTATION-------------------------------*/
void WSDisplayGetPrimary(WSDISPLAY* pDisplay)
{
	Window root;
	int i;
	pDisplay->mHandle = WSX11MainDisplay();
	root = DefaultRootWindow(pDisplay->mHandle);
	XRRScreenResources* screenResources =
		XRRGetScreenResources(pDisplay->mHandle, root);

	RROutput primaryOutput = XRRGetOutputPrimary(pDisplay->mHandle, root);
	if(!primaryOutput && screenResources->noutput)
		primaryOutput = screenResources->outputs[0];
	for (i = 0; i < screenResources->noutput; ++i)
	{
		if (screenResources->outputs[i] == primaryOutput)
		{
			XRROutputInfo* outputInfo = XRRGetOutputInfo(pDisplay->mHandle,
				screenResources, primaryOutput);
			pDisplay->mOutput = primaryOutput;
			pDisplay->mCrtc = outputInfo->crtc;
			XRRFreeOutputInfo(outputInfo);
			break;
		}
	}

	XRRFreeScreenResources(screenResources);

	pDisplay->mScreenNum =
		X11ScreenNumFromRROutput(pDisplay->mHandle, pDisplay->mOutput);
}

void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis)
{
	assert(pThis->mIndex != pThis->mNumScreens);
	++pThis->mIndex;
}

void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay)
{
	int i;
	int numx11screens;
	XRROutputInfo* outputInfo;
	pDisplay->mHandle = pThis->mDisplay;
	pDisplay->mOutput = pThis->mScreenResources->outputs[pThis->mIndex];
	outputInfo = pThis->mOutputInfos[pThis->mIndex];
	pDisplay->mCrtc = outputInfo->crtc;
	pDisplay->mScreenNum =
		X11ScreenNumFromRROutput(pDisplay->mHandle, pDisplay->mOutput);
}

void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis)
{
	for (int i = 0; i < pThis->mNumScreens; ++i)
	{
		XRRFreeOutputInfo(pThis->mOutputInfos[i]);
		XRRFreeCrtcInfo(pThis->mCrtcInfos[i]);
	}
	WSFREE(pThis->mOutputInfos);
	WSFREE(pThis->mCrtcInfos);
	XRRFreeScreenResources(pThis->mScreenResources);
	WSX11CloseMainDisplay(pThis->mDisplay);
}

void WSDisplayBegin(WSDISPLAYITERATOR* pIt)
{
	pIt->mDisplay = WSX11MainDisplay();
	Window root = RootWindow(pIt->mDisplay, DefaultScreen(pIt->mDisplay));
	pIt->mScreenResources = XRRGetScreenResources(pIt->mDisplay, root);
	pIt->mNumScreens = pIt->mScreenResources->noutput;
	pIt->mIndex = 0;

	pIt->mOutputInfos = WSALLOC(XRROutputInfo*, pIt->mNumScreens);
	pIt->mCrtcInfos = WSALLOC(XRRCrtcInfo*, pIt->mNumScreens);

	for (int i = 0; i < pIt->mNumScreens; ++i)
	{
		pIt->mOutputInfos[i] = XRRGetOutputInfo(pIt->mDisplay,
			pIt->mScreenResources, pIt->mScreenResources->outputs[i]);
		pIt->mCrtcInfos[i] = XRRGetCrtcInfo(pIt->mDisplay,
			pIt->mScreenResources, pIt->mOutputInfos[i]->crtc);
	}
}

wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt)
{
	return pIt->mIndex == pIt->mNumScreens;
}

wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
{
	wsbool res = FALSE;
	WSDISPLAYITERATOR it;
	WSDisplayBegin(&it);
	for (; !WSDisplayIsEnd(&it); WSDisplayIterator_Iterate(&it))
	{
		XRROutputInfo* outputInfo = it.mOutputInfos[it.mIndex];
		if (outputInfo->connection == RR_Connected)
		{
			XRRCrtcInfo* crtcInfo = it.mCrtcInfos[it.mIndex];
			if (x >= crtcInfo->x && x < crtcInfo->x + crtcInfo->width
				&& y >= crtcInfo->y && y < crtcInfo->y + crtcInfo->height)
			{
				WSDisplayIterator_Value(&it, pDisplay);
				res = TRUE;
			}
		}
	}
	WSDisplayIterator_Destroy(&it);
	return res;
}

wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
{
	guipos_t x = r->left + ((r->right - r->left) / 2);
	guipos_t y = r->top + ((r->bottom - r->top) / 2);
	return WSDisplayGetFromPoint(x, y, pDisplay);
}

void WSDisplayX11GetGeometry_(Display* maindisplay, RRCrtc crtc,
	int screennum, WSRECT* r)
{
	XRRScreenResources* screenResources = XRRGetScreenResources(
		maindisplay, RootWindow(maindisplay, screennum));
	XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(maindisplay, screenResources, crtc);

	r->left = crtcInfo->x;
	r->top = crtcInfo->y;
	r->right = crtcInfo->x + crtcInfo->width;
	r->bottom = crtcInfo->y + crtcInfo->height;

	XRRFreeCrtcInfo(crtcInfo);
	XRRFreeScreenResources(screenResources);
}

void WSDisplayGetGeometry(WSRECT* r)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	WSDisplayX11GetGeometry_(d.mHandle, d.mCrtc, d.mScreenNum, r);
	WSX11CloseMainDisplay(d.mHandle);
}

void WSDisplayX11GetNetWorkArea(Display* maindisplay, int screennum, WSRECT* r)
{
	Atom prop = XInternAtom(maindisplay, "_NET_WORKAREA", TRUE);
	if (prop)
	{
		Atom actual_type;
		int actual_format;
		unsigned long nitems;
		unsigned long bytes_after;
		unsigned char* data = NULL;
		Status status = XGetWindowProperty(
			maindisplay, RootWindow(maindisplay, screennum),
			prop, 0, 4, FALSE, XA_CARDINAL,
			&actual_type, &actual_format, &nitems, &bytes_after, &data);
		if (status == Success && actual_type == XA_CARDINAL
			&& actual_format == 32 && nitems == 4)
		{
			const long* p = (const long*)data;
			r->left = p[0];
			r->top = p[1];
			r->right = p[2] - r->left;
			r->bottom = p[3] - r->top;
		}
		else
		{
			r->left = r->top = r->bottom = r->right = 0;
		}
		if (data)
			XFree(data);
	}
	else
	{
		r->left = r->top = r->bottom = r->right = 0;
	}
}

void WSDisplayX11GetClientArea_(Display* maindisplay, RRCrtc crtc,
	int screennum, WSRECT* r)
{
	WSRECT rgeo;
	WSDisplayX11GetGeometry_(maindisplay, crtc, screennum, &rgeo);
	WSDisplayX11GetNetWorkArea(maindisplay, screennum, r);

	if ( !r->right || !r->bottom )
		*r = rgeo; /* WSDisplayX11GetNetWorkArea failed, use full area */
	else
	{ /* Force the client area into the full area */
		if(r->right > rgeo.right)
			r->right = rgeo.right;
		if(r->left < rgeo.left)
			r->left = rgeo.left;
		if(r->bottom > rgeo.bottom)
			r->bottom = rgeo.bottom;
		if(r->top < rgeo.top)
			r->top = rgeo.top;
	}
}

void WSDisplayGetClientArea(WSRECT* r)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	WSDisplayX11GetClientArea_(d.mHandle, d.mCrtc, d.mScreenNum, r);
	WSX11CloseMainDisplay(d.mHandle);
}

guipos_t WSDisplayCountSetBits_(unsigned long n)
{
	guipos_t count = 0;
	while (n)
	{
		count += n & 1;
		n >>= 1;
	}
	return count;
}

guipos_t WSDisplayX11GetDepth_(Display* display, int screen)
{
	guipos_t depth = DefaultDepth(display, screen);
	Visual* visual = DefaultVisual(display, screen);
	if (visual->c_class == TrueColor || visual->c_class == DirectColor)
	{ /* Count bits in each color mask to get the bits per pixel */
		depth = WSDisplayCountSetBits_(visual->red_mask)
			+ WSDisplayCountSetBits_(visual->green_mask)
			+ WSDisplayCountSetBits_(visual->blue_mask);
	}
	else
	{ /* For other visual classes, calculate bits per pixel by
		counting all set bits in combined color masks */
		unsigned long combinedMask = visual->red_mask
			| visual->green_mask | visual->blue_mask;
		depth = WSDisplayCountSetBits_(combinedMask);
	}
	return depth;
}

void WSDisplayGetDepth(guipos_t* depth)
{
	WSDISPLAY d;
	WSDisplayGetPrimary(&d);
	WSDisplay_GetDepth(&d, depth);
	WSDisplay_Destroy(&d);
}

void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth)
{
	*depth = WSDisplayX11GetDepth_(pThis->mHandle, pThis->mScreenNum);
}

void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r)
{
	WSDisplayX11GetGeometry_(pThis->mHandle, pThis->mCrtc,
		pThis->mScreenNum, r);
}

void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r)
{
	WSDisplayX11GetClientArea_(pThis->mHandle, pThis->mCrtc,
		pThis->mScreenNum, r);
}

wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis)
{
	return XRRGetOutputPrimary(pThis->mHandle,
		RootWindow(pThis->mHandle, pThis->mScreenNum)) == pThis->mOutput;
}

void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	int i;
	XRRModeInfo* curx11mode;
	XRRScreenResources* screenResources = XRRGetScreenResources(
		pThis->mHandle, RootWindow(pThis->mHandle, pThis->mScreenNum));
	XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(pThis->mHandle,
		screenResources, pThis->mCrtc);

	for (i = 0; i < screenResources->nmode; i++)
	{
		if (screenResources->modes[i].id == crtcInfo->mode)
		{
			curx11mode = &screenResources->modes[i];
			break;
		}
	}
	if(i == screenResources->nmode)
	{
		WSVideoMode_ConstructDefault(mode);
		return;
	}

	mode->width = crtcInfo->width;
	mode->height = crtcInfo->height;
	WSDisplay_GetDepth(pThis, &mode->depth);
	mode->refreshrate =
		X11ConvertRefreshRate(curx11mode->dotClock,
				curx11mode->hTotal,
				curx11mode->vTotal);

	XRRFreeCrtcInfo(crtcInfo);
	XRRFreeScreenResources(screenResources);
}

wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{
	XRRScreenResources* screenResources = XRRGetScreenResources(
		pThis->mHandle, RootWindow(pThis->mHandle, pThis->mScreenNum));
	XRROutputInfo* outputInfo = XRRGetOutputInfo(pThis->mHandle,
		screenResources, pThis->mOutput);
	XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(pThis->mHandle,
		screenResources, pThis->mCrtc);
	int depth, curdepth;
	WSDisplay_GetDepth(pThis, &depth);

	wsbool ret = FALSE;
	for (int i = 0; i < screenResources->nmode; i++)
	{
		WSVIDEOMODE curmode;
		curmode.width = screenResources->modes[i].width;
		curmode.height = screenResources->modes[i].height;
		curmode.refreshrate =
			X11ConvertRefreshRate(screenResources->modes[i].dotClock,
				screenResources->modes[i].hTotal,
				screenResources->modes[i].vTotal);
		curdepth = depth;
		do
		{
			curmode.depth = curdepth;
			if (WSVideoMode_Matches(&curmode, mode))
			{
				Rotation rotation = (crtcInfo->rotation & 0xf) ?
						crtcInfo->rotation : RR_Rotate_0;
				Status status = XRRSetCrtcConfig(pThis->mHandle,
					screenResources, pThis->mCrtc, CurrentTime,
					crtcInfo->x, crtcInfo->y,
					screenResources->modes[i].id,
					rotation,
					(RROutput*)&pThis->mOutput, 1); /* const->nonconst */
				if(status == Success)
					ret = TRUE;
				break;
			}
			if(curdepth >= 8)
				curdepth -= 8;
		} while(curdepth >= 8);
		if(ret)
			break;
	}

	XRRFreeCrtcInfo(crtcInfo);
	XRRFreeOutputInfo(outputInfo);
	XRRFreeScreenResources(screenResources);
	return ret;
}

void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode, WSVIDEOMODE** modes,
	unsigned int* modescount)
{
	int depth, curdepth;
	XRRScreenResources* screenResources = XRRGetScreenResources(
		pThis->mHandle, RootWindow(pThis->mHandle, pThis->mScreenNum));
	*modescount = 0;
	*modes = NULL;

	WSDisplay_GetDepth(pThis, &depth);
	for (int i = 0; i < screenResources->nmode; i++)
	{
		WSVIDEOMODE curmode;
		curmode.width = screenResources->modes[i].width;
		curmode.height = screenResources->modes[i].height;
		curmode.refreshrate = X11ConvertRefreshRate(
			screenResources->modes[i].dotClock,
				screenResources->modes[i].hTotal,
				screenResources->modes[i].vTotal);

		curdepth = depth;
		do
		{
			curmode.depth = curdepth;
			if (WSVideoMode_Matches(&curmode, matchingmode))
			{
				++*modescount;
				if(modes)
				{
					WSVIDEOMODE* newmodes = WSREALLOC(*modes,
						WSVIDEOMODE, *modescount);
					if(newmodes)
					{
						newmodes[*modescount - 1] = curmode;
						*modes = newmodes;
					}
				}
			}
			if(curdepth >= 8)
				curdepth -= 8;
		} while(curdepth >= 8);
	}

	XRRFreeScreenResources(screenResources);
}

size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize)
{
	Window window = RootWindow(pThis->mHandle, pThis->mScreenNum);
	XRRScreenResources* screenResources = XRRGetScreenResources(
		pThis->mHandle, window);
	XRROutputInfo* outputInfo = XRRGetOutputInfo(pThis->mHandle,
		screenResources, pThis->mOutput);

	size_t size = strlen(outputInfo->name);
	if (buffer && buffersize)
	{
		if (buffersize <= size)
			--buffersize;
		else
			buffersize = size;
		memcpy(buffer, outputInfo->name, buffersize);
		buffer[buffersize] = '\0';
	}

	XRRFreeOutputInfo(outputInfo);
	XRRFreeScreenResources(screenResources);
	return size;
}

int WSDisplay_MakePrimary(const WSDISPLAY* pThis)
{
	XRRSetOutputPrimary(pThis->mHandle,
		RootWindow(pThis->mHandle, pThis->mScreenNum),
		pThis->mOutput);
	return 0;
}

void WSDisplay_ResetMode(const WSDISPLAY* pThis)
{ /* default video mode */
	WSVIDEOMODE vm;
	WSVideoMode_ConstructDefault(&vm);
	WSDisplay_ChangeMode(pThis, &vm);
}

void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y)
{
	XRRScreenResources* screenResources = XRRGetScreenResources(
		pThis->mHandle, RootWindow(pThis->mHandle, pThis->mScreenNum));
	XRROutputInfo* outputInfo = XRRGetOutputInfo(pThis->mHandle,
		screenResources, pThis->mOutput);
	XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(pThis->mHandle,
		screenResources, pThis->mCrtc);

	if (outputInfo->mm_width > 0 && outputInfo->mm_height > 0)
	{
		double xres = ((((double)crtcInfo->width) * 25.4)
			/ ((double)outputInfo->mm_width));
		double yres = ((((double)crtcInfo->height) * 25.4)
			/ ((double)outputInfo->mm_height));
		*x = (int)(xres + 0.5);
		*y = (int)(yres + 0.5);
	}
	else
	{
		WSDisplayGetStdPPIValue(x);
		WSDisplayGetStdPPIValue(y);
	}

	XRRFreeCrtcInfo(crtcInfo);
	XRRFreeOutputInfo(outputInfo);
	XRRFreeScreenResources(screenResources);
}

void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale)
{
	XRRScreenResources* screenResources = XRRGetScreenResources(
		pThis->mHandle, RootWindow(pThis->mHandle, pThis->mScreenNum));
	XRROutputInfo* outputInfo = XRRGetOutputInfo(pThis->mHandle,
		screenResources, pThis->mOutput);
	XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(pThis->mHandle,
		screenResources, pThis->mCrtc);

	if (outputInfo->mm_width > 0 && outputInfo->mm_height > 0)
	{
		double xres = ((((double)crtcInfo->width) * 25.4)
			/ ((double)outputInfo->mm_width));
		double yres = ((((double)crtcInfo->height) * 25.4)
			/ ((double)outputInfo->mm_height));
		*scale = (float)(((xres + yres) / 2) / 96.0);
	}
	else
		*scale = 1.0;

	XRRFreeCrtcInfo(crtcInfo);
	XRRFreeOutputInfo(outputInfo);
	XRRFreeScreenResources(screenResources);
}
void WSDisplay_Destroy(WSDISPLAY* pThis)
{
	WSX11CloseMainDisplay(pThis->mHandle);
}
#if !defined(__cplusplus)
	#undef c_private
	#undef c_class
#endif
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================


						N/A Not Implemented OSes


=============================================================================*/
#else
#ifdef __cplusplus
	extern "C" {
#endif
void WSDisplayIterator_Iterate(WSDISPLAYITERATOR* pThis)
{}
void WSDisplayIterator_Value(const WSDISPLAYITERATOR* pThis,
	WSDISPLAY* pDisplay)
{}
void WSDisplayIterator_Destroy(WSDISPLAYITERATOR* pThis)
{}

void WSDisplayGetPrimary(WSDISPLAY* pDisplay)
{}
void WSDisplayBegin(WSDISPLAYITERATOR* pIt)
{}
wsbool WSDisplayIsEnd(WSDISPLAYITERATOR* pIt)
{ return TRUE; }
wsbool WSDisplayGetFromPoint(guipos_t x, guipos_t y, WSDISPLAY* pDisplay)
{ return FALSE; }
wsbool WSDisplayGetFromRect(WSRECT* r, WSDISPLAY* pDisplay)
{ return FALSE; }
wsbool WSDisplayGetFromWindow(void* windowhandle, WSDISPLAY* pDisplay)
{ return FALSE; }
void WSDisplayGetGeometry(WSRECT* r)
{ r->left = r->top = r->right = r->bottom = 0; }
void WSDisplayGetClientArea(WSRECT* r)
{ r->left = r->top = r->right = r->bottom = 0; }
void WSDisplayGetDepth(guipos_t* depth)
{ *depth = 0; }
void WSDisplay_GetGeometry(const WSDISPLAY* pThis, WSRECT* r)
{ r->left = r->top = r->right = r->bottom = 0; }
void WSDisplay_GetClientArea(const WSDISPLAY* pThis, WSRECT* r)
{ r->left = r->top = r->right = r->bottom = 0; }
wsbool WSDisplay_IsPrimary(const WSDISPLAY* pThis)
{ return FALSE; }
void WSDisplay_GetCurrentMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{}
wsbool WSDisplay_ChangeMode(const WSDISPLAY* pThis, WSVIDEOMODE* mode)
{ return FALSE; }
void WSDisplay_ResetMode(const WSDISPLAY* pThis)
{}
void WSDisplay_GetModes(const WSDISPLAY* pThis,
	const WSVIDEOMODE* matchingmode,
	WSVIDEOMODE** modes, unsigned int* modescount)
{}
void WSDisplay_GetPPI(WSDISPLAY* pThis, guipos_t* x, guipos_t* y)
{ *x = *y = 0; }
void WSDisplay_GetScaleFactor(WSDISPLAY* pThis, float* scale)
{}
void WSDisplay_GetDepth(const WSDISPLAY* pThis, guipos_t* depth)
{ *depth = 0; }
void WSDisplay_Destroy(WSDISPLAY* pThis)
{}
size_t WSDisplay_GetName(const WSDISPLAY* pThis,
	char* buffer, size_t buffersize)
{ return 0; }
int WSDisplay_MakePrimary(const WSDISPLAY* pThis)
{ return 0; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
#endif /* OS if/else */
#endif /* WSDISPLAY_IMPLEMENTATION */
