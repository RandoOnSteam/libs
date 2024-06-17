/**
*	@file joystick.h
*
*	#     #  #####        # ####### #     #  #####  ####### ###  #####  #    #
*	#  #  # #     #       # #     #  #   #  #     #    #     #  #     # #   #
*	#  #  # #             # #     #   # #   #          #     #  #       #  #
*	#  #  #  #####        # #     #    #     #####     #     #  #       ###
*	#  #  #       # #     # #     #    #          #    #     #  #       #  #
*	#  #  # #     # #     # #     #    #    #     #    #     #  #     # #   #
*	 ## ##   #####   #####  #######    #     #####     #    ###  #####  #    #
*
*	Single header file library for OS-dependant joystick API abstraction.
*
*	###Instructions###
*
*	Define WSJOYSTICK_IMPLEMENTATION in one source file for the implementation
*
*	###Example###
*	WSJOYSTICKITERATOR it;
*	WSJoystickBegin(&it);
*	for(; !WSJoystickIsEnd(&it); WSJoystickIterator_Iterate(&it))
*	{
*		WSJOYSTICK joy;
*		WSJOYSTICKCAPS joycaps;
*		WSJOYSTICKSTATE joystate;
*		WSJoystickIterator_Value(&joy);
*		WSJoystick_GetCaps(&joy, &joycaps);
*		WSJoystick_PollState(&joy, &joystate);
*		WSJoystick_Destroy(&joy);
*	}
*	WSJoystickIterator_Destroy(&it);
*
*	###XBOX 360 ON WINDOWS###
*	XBOX 360/ONE/SERIES and knockoff controllers are laid out as
*	follows using the Windows MMSYSTEM API:
*
*	Button indexes:
*
*	A == 0, B == 1, X == 2, Y == 3, LSHOULDER == 4, RSHOULDER == 5,
*	SELECT == 6, START == 7
*
*	POV Axis is the DPAD:
*
*	0 == up, 9000 == right, 18000 == down, 27000 == left
*
*	Zaxis is LT/RT:
*	128-32766 == right, 32767 == none, 32768-65408 == left
*
*	X/Y axes are the left stick
*
*	U/Rudder axes are the right stick
*
*	Common settings are 7000 sensitivity, 260 initial delay, 80 repeat delay.
*
*	if(curval >= 260 && curval - lastjoyinterval >= 120)
*	{
*		WSJOYSTICKSTATE js;
*		WSJoystick_PollState(&joy, &js);
*		WSJoystickState_GetXPosition(&js);
*		WSJoystickState_GetYPosition(&js);
*		for(int ibutton = 0; ibutton <
*				WSJoystickCaps_GetNumberButtons(&jc); ++ibutton)
*		{
*			if(WSJoystickState_GetButtonState(&js, ibutton))
*			{
*				WSConsole_Write(&con, "Joystick button #[");
*				WSConsole_WriteUInt(&con, ibutton);
*				WSConsole_Write(&con, "] is pressed\n");
*				lastjoyinterval = curval;
*			}
*		}
*		int xpos = WSJoystickState_GetXPosition(&js);
*		int ypos = WSJoystickState_GetYPosition(&js);
*		int zpos = WSJoystickState_GetZPosition(&js);
*		int upos = WSJoystickState_GetUPosition(&js);
*		int rpos = WSJoystickState_GetRudderPosition(&js);
*		int pov = WSJoystickState_GetPOVPosition(&js);
*		if(xpos < (WSJoystickCaps_GetXMax(&jc) / 2) - 7000
*			|| xpos > (WSJoystickCaps_GetXMax(&jc) / 2) + 7000)
*		{
*			WSConsole_Write(&con, "Joystick Left Stick X [");
*			WSConsole_WriteUInt(&con, xpos);
*			WSConsole_Write(&con, "]\n");
*			lastjoyinterval = curval;
*		}
*		if(ypos < (WSJoystickCaps_GetYMax(&jc) / 2) - 7000
*			|| ypos > (WSJoystickCaps_GetYMax(&jc) / 2) + 7000)
*		{
*			WSConsole_Write(&con, "Joystick Left Stick Y [");
*			WSConsole_WriteUInt(&con, ypos);
*			WSConsole_Write(&con, "]\n");
*			lastjoyinterval = curval;
*		}
*		if(zpos < (WSJoystickCaps_GetZMax(&jc) / 2) - 7000
*			|| zpos > (WSJoystickCaps_GetZMax(&jc) / 2) + 7000)
*		{
*			if(zpos < (WSJoystickCaps_GetZMax(&jc) / 2) - 7000)
*				WSConsole_Write(&con, "Joystick RT [");
*			else
*				WSConsole_Write(&con, "Joystick LT [");
*			WSConsole_WriteUInt(&con, zpos);
*			WSConsole_Write(&con, "]\n");
*			lastjoyinterval = curval;
*		}
*		if(upos < (WSJoystickCaps_GetUMax(&jc) / 2) - 7000
*			|| upos > (WSJoystickCaps_GetUMax(&jc) / 2) + 7000)
*		{
*			WSConsole_Write(&con, "Joystick Right Stick X [");
*			WSConsole_WriteUInt(&con, upos);
*			WSConsole_Write(&con, "]\n");
*			lastjoyinterval = curval;
*		}
*		if(rpos < (WSJoystickCaps_GetRudderMax(&jc) / 2) - 7000
*			|| rpos > (WSJoystickCaps_GetRudderMax(&jc) / 2) + 7000)
*		{
*			WSConsole_Write(&con, "Joystick Right Stick Y [");
*			WSConsole_WriteUInt(&con, rpos);
*			WSConsole_Write(&con, "]\n");
*			lastjoyinterval = curval;
*		}
*		switch(pov)
*		{
*		case 0:
*			WSConsole_Write(&con, "DPAD [UP]\n");
*			lastjoyinterval = curval;
*			break;
*		case 9000:
*			WSConsole_Write(&con, "DPAD [RIGHT]\n");
*			lastjoyinterval = curval;
*			break;
*		case 18000:
*			WSConsole_Write(&con, "DPAD [DOWN]\n");
*			lastjoyinterval = curval;
*			break;
*		case 27000:
*			WSConsole_Write(&con, "DPAD [LEFT]\n");
*			lastjoyinterval = curval;
*			break;
*		}
*	}
*
*	###Supported targets###
*	- Win32 (mmsystem, XInput with WSJOYSTICKXINPUTOPT)
*	- Mac Classic/Carbon (InputSprocket)
*	- Mac OSX (Darwin HID)
*	- Linux (/dev/input/js#, /dev/js#)
*	- BSD (/dev/ujoy/#, /dev/joy#, /dev/uhid#)
*	- X68000 (_iocs_joyget)
*	- AMIGA (ReadJoyPort)
*	- IOS 7+ (GCController)
*
*	###Supports###
*	- Arbitrary number of buttons
*	- X/Y/Z/U/V axes
*	- Rudder
*	- POV
*
*	###TODO###
*	- Force feedback.
*	-- Would require the Raw Input+DirectInput+XInput Trio on Win32
*	- More meaningful joystick naming based on product+vendor ids
*	-- Libraries like SDL have large source files devoted purely to this
*	- Predefined mapping of buttons based on controller type
*	-- Similar to XInput
*	- Linux/BSD opens on iterate and value of iterator
*	-- Rework iterator API or cache the value
*
*	###Options###
*	- For Windows:
*	-- WSJOYSTICK_STATICADVAPI32 removes dyncalls to advapi32.dll
*	-- WSJOYSTICK_STATICNOWINNT31 removes Windows NT 3.1 compatibility
*
*	###NOTES###
*	- ASCII art, mostly "Banner", from
*	-- budavariam.github.io/asciiart-text/multi
*
*	@version 0.4
*
*	Version History
*
*	0.4		Cleaned up iteration. WSJoystick_IsCreated()
*
*	0.3		XInput option
*
*	0.2		WSMSIXOPT for MSIX apps.
*				- Rename XXX_Destruct to XXX_Destroy
*				- Some define name fixes related to dyncalls
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
#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__
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
#if defined(WSMSIXOPT)
	#if !defined(WSJOYSTICK_NOWINXP)
		#define WSJOYSTICK_NOWINXP
	#endif
	#if !defined(WSJOYSTICK_STATICADVAPI32)
		#define WSJOYSTICK_STATICADVAPI32
	#endif
#endif
#if defined(WSJOYSTICK_NOWINXP)
	#if !defined(WSJOYSTICK_NOWINNT31)
		#define WSJOYSTICK_NOWINNT31
	#endif
#endif
#if defined(_WIN32)
	typedef struct WSJOYSTICKSTATE WSJOYSTICKSTATE;
	typedef struct _XINPUT_STATE _XINPUT_STATE;
	typedef unsigned int (__stdcall *joyGetPosExProc)(
		unsigned int joyid, WSJOYSTICKSTATE* state);
	typedef int WSJOYSTICKHANDLE;
	typedef uint32_t (__stdcall *XInputGetStateProc)(
		uint32_t dwUserIndex, _XINPUT_STATE* pState);
#elif defined(__amigaos__)
	#define WSJOYSTICK_MAX_AXES       2
	#define WSJOYSTICK_MAX_BUTTONS    7
	typedef int WSJOYSTICKHANDLE;
#elif defined(macintosh)
	#define WSJOYSTICK_MAX_JOYSTICKS 16 /* bungie uses 10 */
	#define WSJOYSTICK_MAX_BUTTONS 32
	#define WSJOYSTICK_MAX_AXES 16
	typedef struct ISpDeviceDefinition ISpDeviceDefinition;
	typedef struct OpaqueISpDeviceReference* ISpDeviceReference;
	typedef struct OpaqueISpElementReference* ISpElementReference;
	typedef struct OpaqueISpElementListReference* ISpElementListReference;
	typedef ISpDeviceReference WSJOYSTICKHANDLE;
#elif defined(__MACH__)
	#if defined(__has_builtin) && __has_builtin(__is_target_arch) \
		&& __has_builtin(__is_target_vendor) && __has_builtin(__is_target_os) \
		&& __has_builtin(__is_target_environment) \
		&& (__is_target_arch(x86_64) \
			&& __is_target_vendor(apple) && __is_target_os(ios)) \
			|| (__is_target_arch(x86_64) && __is_target_vendor(apple) \
				&& __is_target_os(ios) && __is_target_environment(simulator))
		#define WSJOYSTICKMACIOS

		#if (defined(__GNUC__) && defined(__APPLE__))
			#if defined(__OBJC__)
				#define DECLARE_OBJC_CLASS(klass, name) \
					@class klass; \
					typedef klass* name;
			#else /*  not defined(__OBJC__) */
				#define DECLARE_OBJC_CLASS(klass, name) \
					typedef struct klass* name;
			#endif /*  defined(__OBJC__) */
		#else /*  not Apple's gcc */
			#define DECLARE_OBJC_CLASS(klass, name) \
				typedef struct objc_object* name;
		#endif /*  (defined(__GNUC__) && defined(__APPLE__)) */
		DECLARE_OBJC_CLASS(GCController, JoyGCController)
		DECLARE_OBJC_CLASS(GCControllerButtonInput,
						   JoyGCControllerButtonInput)
		DECLARE_OBJC_CLASS(GCControllerAxisInput,
						   JoyGCControllerAxisInput)
		DECLARE_OBJC_CLASS(GCControllerDirectionPad,
						   JoyGCControllerDirectionPad)
	#endif
	typedef struct IOHIDDeviceInterface IOHIDDeviceInterface;
	typedef struct IOHIDQueueInterface IOHIDQueueInterface;
	typedef uint32_t IOHIDElementCookie;
	typedef unsigned int mach_port_t; /*struct ipc_port* */
	typedef mach_port_t io_iterator_t;
	typedef mach_port_t io_object_t;
	typedef struct __CFDictionary* CFMutableDictionaryRef;
	typedef struct __CFArray* CFMutableArrayRef;
	#define WSJOYSTICK_MAX_AXES       10 /*max number of axes*/
	#define WSJOYSTICK_MAX_BUTTONS    40 /*max number of buttons*/
#elif defined(__linux__)
	typedef int WSJOYSTICKHANDLE;
	#define WSJOYSTICK_MAX_AXES 15
#elif defined(__human68k__) /* X68000 */
	typedef int WSJOYSTICKHANDLE;
#else
	#define WSJOYSTICK_MAX_AXES 22
	typedef int WSJOYSTICKHANDLE;
	#if defined(__FreeBSD__) || HAVE_MACHINE_JOYSTICK_H \
		 || defined(__FreeBSD_kernel__) || defined(__DragonFly_)
		#define HAVE_GAMEPORT
	#endif
#endif

typedef struct WSJOYSTICK WSJOYSTICK;
typedef struct WSJOYSTICKSTATE WSJOYSTICKSTATE;
typedef struct WSJOYSTICKCAPS WSJOYSTICKCAPS;
typedef struct WSJOYSTICKITERATOR WSJOYSTICKITERATOR;
void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis);
void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick);
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis);

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt);
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt);
wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis);
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state);
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps);
void WSJoystick_Destroy(WSJOYSTICK* pThis);

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis);
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis);
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis);
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button);
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis);
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis);
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis);
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis);

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis);
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick);
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis);
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis);
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis);
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis);
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis);
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis);
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis);
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis);
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis);

typedef struct WSJOYSTICKITERATOR
{
#if defined(__cplusplus)
	void Iterate()
	{ WSJoystickIterator_Iterate(this); }
	void Value(WSJOYSTICK* pJoystick) const
	{ WSJoystickIterator_Value(this, pJoystick); }
	void Destroy()
	{ WSJoystickIterator_Destroy(this); }
#endif
#if defined(_WIN32)
	WSJOYSTICKHANDLE joystick;
	#if defined(WSJOYSTICKXINPUTOPT)
	#else
		WSJOYSTICKHANDLE maxjoysticks;
	#endif /* WSJOTSTICKXINPUTOPT */
#elif defined(__amigaos__)
	WSJOYSTICKHANDLE joystick;
#elif defined(macintosh)
	WSJOYSTICKHANDLE joysticks[WSJOYSTICK_MAX_JOYSTICKS];
	int maxjoysticks;
	int index;
#elif defined(WSJOYSTICKMACIOS)
	unsigned int index;
#elif defined(__MACH__)
	mach_port_t mMasterPort;
	io_iterator_t mIterator;
	io_object_t mCurObject;
	CFMutableDictionaryRef mDevDict;
	int mDidGamepadsAlso;
#else
	int index;
	#if defined(__linux__)
		int type;
	#endif
#endif
} WSJOYSTICKITERATOR;

typedef struct WSJOYSTICK
{
#if defined(__cplusplus)
	static void Begin(WSJOYSTICKITERATOR* pIt) { WSJoystickBegin(pIt); }
	static wsbool IsEnd(WSJOYSTICKITERATOR* pIt)
	{ return WSJoystickIsEnd(pIt); }
	void PollState(WSJOYSTICKSTATE* state)
	{ WSJoystick_PollState(this, state); }
	void GetCaps(WSJOYSTICKCAPS* caps) const
	{ WSJoystick_GetCaps(this, caps); }
	void Destroy()
	{ WSJoystick_Destroy(this); }
#endif /* __cplusplus */
#if defined(_WIN32)
	WSJOYSTICKHANDLE mHandle;
	#if defined(WSJOYSTICKXINPUTOPT)
		#if !defined(WSJOYSTICK_NOWINXP)
			struct HINSTANCE__* mhXInput;
			XInputGetStateProc mpfnXInputGetState;
		#endif
	#elif !defined(WSJOYSTICK_NOWINNT31)
		joyGetPosExProc mpfnJoyGetPosEx;
		struct HINSTANCE__* mhWinMM;
	#endif
#elif defined(__amigaos__)
	WSJOYSTICKHANDLE mHandle;
	unsigned long mButtonCache[WSJOYSTICK_MAX_BUTTONS];
#elif defined(macintosh)
	WSJOYSTICKHANDLE mHandle;
	ISpElementReference mXAxis, mYAxis, mZAxis,
		mUAxis, mVAxis, mRudder, mPOV;
	ISpElementReference mButtons[WSJOYSTICK_MAX_BUTTONS];
	int mNumButtons;
#elif defined(WSJOYSTICKMACIOS)
	JoyGCController mHandle;
	JoyGCControllerDirectionPad mAxes[WSJOYSTICK_MAX_AXES];
	JoyGCControllerButtonInput mButtons[WSJOYSTICK_MAX_BUTTONS];
	unsigned int mNumButtons, mNumAxes;
#elif defined(__MACH__)
	IOHIDDeviceInterface** mppDevice;
	CFMutableDictionaryRef mPropDict;
	CFMutableArrayRef mCookies;
	CFMutableDictionaryRef mXAxis, mYAxis, mZAxis, mUAxis, mVAxis,
		mRudder, mPOV;
	CFMutableDictionaryRef mButtons[WSJOYSTICK_MAX_BUTTONS];
	int mNumButtons;
#elif defined(__linux__)
	WSJOYSTICKHANDLE mHandle;
	int  axes[WSJOYSTICK_MAX_AXES];
	uint32_t buttonsstate;
	int mVersion;
#elif defined(__human68k__)
	WSJOYSTICKHANDLE mHandle;
#else
	union
	{
		struct WSJOYSTICKUHIDDATA
		{
			void* descs;
			size_t descssize;
			struct report_desc* uhidrepdesc;
			size_t numhidrepdesc;
			int uhidrepid;
		} uhid;
		struct WSJOYSTICKGAMEPORTDATA
		{
			int x, y;
			int xmin, ymin;
			int xmax, ymax;
		} gameport;
	} u;
	int index;
	int axes[WSJOYSTICK_MAX_AXES];
	uint32_t buttonsstate;
	WSJOYSTICKHANDLE mHandle;
#endif
} WSJOYSTICK;

typedef struct WSJOYSTICKSTATE
{
#if defined(__cplusplus)
  int GetXPosition(void) const
  { return WSJoystickState_GetXPosition(this); }
  int GetYPosition(void) const
  { return WSJoystickState_GetYPosition(this); }
  int GetZPosition(void) const
  { return WSJoystickState_GetZPosition(this); }
  bool GetButtonState(int button) const
  { return WSJoystickState_GetButtonState(this, button) == TRUE; }
  int GetPOVPosition(void) const
  { return WSJoystickState_GetPOVPosition(this); }
  int GetRudderPosition(void) const
  { return WSJoystickState_GetRudderPosition(this); }
  int GetUPosition(void) const
  { return WSJoystickState_GetUPosition(this); }
  int GetVPosition(void) const
  { return WSJoystickState_GetVPosition(this); }
#endif /* __cplusplus */
#if defined(_WIN32)
	#if defined(WSJOYSTICKXINPUTOPT)
		uint32_t packetnum;
		struct
		{
			uint16_t wButtons;
			uint8_t leftTrigger, rightTrigger;
			short thumbLX, thumbLY, thumbRX, thumbRY;
		};
	#else
		uint32_t dwSize;
		uint32_t dwFlags;
		uint32_t dwXpos;
		uint32_t dwYpos;
		uint32_t dwZpos;
		uint32_t dwRpos;
		uint32_t dwUpos;
		uint32_t dwVpos;
		uint32_t dwButtons;
		uint32_t dwButtonNumber;
		uint32_t dwPOV;
		uint32_t dwReserved1;
		uint32_t dwReserved2;
	#endif /* WSJOYSTICKXINPUTOPT */
#elif defined(__amigaos__)
	unsigned long state;
	const WSJOYSTICK* joystick;
#elif defined(__human68k__)
	int state;
#else
	WSJOYSTICK* joystick;
#endif
} WSJOYSTICKSTATE;
typedef struct WSJOYSTICKCAPS
{
#if defined(__cplusplus)
  int GetManufacturerId(void) const
  { return WSJoystickCaps_GetManufacturerId(this); }
  int GetProductId(void) const
  { return WSJoystickCaps_GetProductId(this); }
  const char* GetProductName(const WSJOYSTICK* joystick)
  { return WSJoystickCaps_GetProductName(this, joystick); }
  int GetXMin(void) const
  { return WSJoystickCaps_GetXMin(this); }
  int GetYMin(void) const
  { return WSJoystickCaps_GetYMin(this); }
  int GetZMin(void) const
  { return WSJoystickCaps_GetZMin(this); }
  int GetXMax(void) const
  { return WSJoystickCaps_GetXMax(this); }
  int GetYMax(void) const
  { return WSJoystickCaps_GetYMax(this); }
  int GetZMax(void) const
  { return WSJoystickCaps_GetZMax(this); }
  int GetPOVMin(void) const
  { return WSJoystickCaps_GetPOVMin(this); }
  int GetPOVMax(void) const
  { return WSJoystickCaps_GetPOVMax(this); }
  int GetPOVCenter(void) const
  { return WSJoystickCaps_GetPOVCenter(this); }
  int GetNumberButtons(void) const
  { return WSJoystickCaps_GetNumberButtons(this); }
  int GetNumberAxes(void) const
  { return WSJoystickCaps_GetNumberAxes(this); }
  int GetMaxButtons(void) const
  { return WSJoystickCaps_GetMaxButtons(this); }
  int GetMaxAxes(void) const
  { return WSJoystickCaps_GetMaxAxes(this); }
  int GetRudderMin(void) const
  { return WSJoystickCaps_GetRudderMin(this); }
  int GetRudderMax(void) const
  { return WSJoystickCaps_GetRudderMax(this); }
  int GetUMin(void) const
  { return WSJoystickCaps_GetUMin(this); }
  int GetUMax(void) const
  { return WSJoystickCaps_GetUMax(this); }
  int GetVMin(void) const
  { return WSJoystickCaps_GetVMin(this); }
  int GetVMax(void) const
  { return WSJoystickCaps_GetVMax(this); }
  bool HasRudder(void) const
  { return WSJoystickCaps_HasRudder(this) == TRUE; }
  bool HasZ(void) const
  { return WSJoystickCaps_HasZ(this) == TRUE; }
  bool HasU(void) const
  { return WSJoystickCaps_HasU(this) == TRUE; }
  bool HasV(void) const
  { return WSJoystickCaps_HasV(this) == TRUE; }
  bool HasPOV(void) const
  { return WSJoystickCaps_HasPOV(this) == TRUE; }
  bool HasPOV4Dir(void) const
  { return WSJoystickCaps_HasPOV4Dir(this) == TRUE; }
  bool HasPOVCTS(void) const
  { return WSJoystickCaps_HasPOVCTS(this) == TRUE; }
#endif /* __cplusplus */
#if defined(_WIN32)
	#if defined(WSJOYSTICKXINPUTOPT)
		int filler;
	#else
		uint16_t wMid;
		uint16_t wPid;
		char szPname[32]; /* MAXPNAMELEN */
		unsigned int wXmin;
		unsigned int wXmax;
		unsigned int wYmin;
		unsigned int wYmax;
		unsigned int wZmin;
		unsigned int wZmax;
		unsigned int wNumButtons;
		unsigned int wPeriodMin;
		unsigned int wPeriodMax;
		unsigned int wRmin;
		unsigned int wRmax;
		unsigned int wUmin;
		unsigned int wUmax;
		unsigned int wVmin;
		unsigned int wVmax;
		unsigned int wCaps;
		unsigned int wMaxAxes;
		unsigned int wNumAxes;
		unsigned int wMaxButtons;
		char szRegKey[32]; /* MAXPNAMELEN */
		char szOEMVxD[260]; /* MAX_JOYSTICKOEMVXDNAME */
	#endif /* WSJOYSTICKXINPUTOPT */
#elif defined(__amigaos__)
	unsigned long state;
#elif defined(__linux__)
	char productname[128];
	char numbuttons;
	char numaxes;
#elif defined(macintosh)
	const WSJOYSTICK* joystick;
	char name[64];
#elif defined(__human68k__)
  /* nothing */
#elif defined(__MACH__)
	const WSJOYSTICK* joystick;
#else
	char productname[128]; /* 128 matches usb_device_info */
	int axes[WSJOYSTICK_MAX_AXES];
	int numbuttons;
	int numaxes;
	uint16_t productid;
	uint16_t vendorid;
#endif
} WSJOYSTICKCAPS;

#define WSJoystick_HandleButton(pThis, val, bit) \
	{ \
		if(val) \
			pThis->buttonsstate |= (1 << bit); \
		else \
			pThis->buttonsstate &= ~(1 << bit); \
	}
#ifdef __cplusplus
	} /* end extern "C" */
#endif
#endif /* __JOYSTICK_H__ */
#if defined(WSJOYSTICK_IMPLEMENTATION)
/*=============================================================================
	One of the issues with Joysticks on Win32 is that XBox 360/One/Series
	controllers have separate drivers for USB HID and XInput. The
	HID drivers combine and the left and right triggers into a single axis
	(in mmsystem the z axis), so when they are both fully pressed the
	value is the same as if neither were pressed.

	Brief overview of Win32 Gamepad/Joystick APIs:

	MMSYSTEM: Most basic and compatible. Support for x/y/z/u/v/r/pov axes
	and 32 buttons. XBox 360/One/Series show everything in neutral state
	unless a window of the app has focus and is in the foreground, and
	since it uses the HID driver the LT/RT issue is present.

	DirectInput: 95+/NT 4+. Supports 6+2 normal and 4 pov axes and 32 buttons
	(in the 95+/NT 4+ form). Same issues as with MMSYSTEM with XBox 360+, and
	force feeback (rumble) does not work without 3rd party hooks with them.

	RawInput: XP+ (though controllers are usually Vista+). Full button and axe
	support. Still has the LT/RT and rumble issues but it doesn't need a
	focused foreground window to get accurate polling.

	XInput: XP+. Only works with XInput controllers, and limited to 4
	controllers. Limited to the XBox 360 controller layout.

	Windows.Gaming.Input (UWP): 10+. Requires a focused foreground window
	to get polling for XBox 360+ controllers, but has full axe and button
	support and can get accurate rumble and LT/RT states.
=============================================================================*/
#if defined(_WIN32)
/*=============================================================================
				 #     # ###
				  #   #   #  #    # #####  #    # #####
				   # #    #  ##   # #    # #    #   #
					#     #  # #  # #    # #    #   #
				   # #    #  #  # # #####  #    #   #
				  #   #   #  #   ## #      #    #   #
				 #     # ### #    # #       ####    #
=============================================================================*/
#if defined(WSJOYSTICKXINPUTOPT)
#include <windows.h>
#ifdef __cplusplus
	extern "C" {
#endif
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30
#if defined(WSJOYSTICK_NOWINXP) && defined(_MSC_VER)
	#pragma comment(lib, "xinput9_1_0.lib")
#endif
typedef struct _XINPUT_GAMEPAD
{
	WORD                                wButtons;
	BYTE                                bLeftTrigger;
	BYTE                                bRightTrigger;
	SHORT                               sThumbLX;
	SHORT                               sThumbLY;
	SHORT                               sThumbRX;
	SHORT                               sThumbRY;
} XINPUT_GAMEPAD, *PXINPUT_GAMEPAD;
typedef struct _XINPUT_STATE
{
	DWORD                               dwPacketNumber;
	XINPUT_GAMEPAD                      Gamepad;
} XINPUT_STATE, *PXINPUT_STATE;

DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState);
void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{
	++pThis->joystick;
}

void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{
	pJoystick->mHandle = pThis->joystick;
#if !defined(WSJOYSTICK_NOWINXP)
	pJoystick->mhXInput = LoadLibraryA("xinput9_1_0.dll");
	if(pJoystick->mhXInput)
	{
		pJoystick->mpfnXInputGetState = (XInputGetStateProc)
			GetProcAddress(pJoystick->mhXInput, "XInputGetState");
		if(pJoystick->mpfnXInputGetState)
		{
			XINPUT_STATE joyinfo;
			if((*pThis->mpfnXInputGetState)(pThis->joystick, &joyinfo)
				!= ERROR_SUCCESS)
			{
				FreeLibrary(pJoystick->mhXInput);
				pJoystick->mhXInput = NULL;
			}
		}
		else
		{
			FreeLibrary(pJoystick->mhXInput);
			pJoystick->mhXInput = NULL;
		}
	}
#endif
}
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{
}

wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{
	#if !defined(WSJOYSTICK_NOWINXP)
		return pThis->mhXInput != NULL;
	#else
		XINPUT_STATE joyinfo;
		return XInputGetState(pThis->mHandle, &joyinfo) == ERROR_SUCCESS;
	#endif
}

void WSJoystick_Destroy(WSJOYSTICK* pThis)
{
#if !defined(WSJOYSTICK_NOWINXP)
	FreeLibrary(pThis->mhXInput);
#endif
}

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{
	XINPUT_STATE joyinfo;
	pIt->joystick = 0;
#if !defined(WSJOYSTICK_NOWINXP)
	pIt->mhXInput = LoadLibraryA("xinput9_1_0.dll");
	pIt->mpfnXInputGetState = (XInputGetStateProc)
		GetProcAddress(pIt->mhXInput, "XInputGetState");
#endif
#if !defined(WSJOYSTICK_NOWINXP)
		if((*pIt->mpfnXInputGetState)(pIt->joystick, &joyinfo)
				!= ERROR_SUCCESS)
#else
		if(XInputGetState(pIt->joystick, &joyinfo) != ERROR_SUCCESS)
#endif
			WSJoystickIterator_Iterate(pIt);
}
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{
	return pIt->joystick >= 4;
}
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{
	DWORD err;
#if !defined(WSJOYSTICK_NOWINXP)
	err = (*pThis->mpfnXInputGetState)(pThis->mHandle, (XINPUT_STATE*)state);
#else
	err = XInputGetState(pThis->mHandle, (XINPUT_STATE*)state);
#endif
	if(err != ERROR_SUCCESS)
		memset(state, 0, sizeof(WSJOYSTICKSTATE));
}
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{ }
int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->thumbLX; }
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->thumbLY; }
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->leftTrigger; }
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{
	switch(button)
	{
	case 0:
		return pThis->wButtons & XINPUT_GAMEPAD_A;
	case 1:
		return pThis->wButtons & XINPUT_GAMEPAD_B;
	case 2:
		return pThis->wButtons & XINPUT_GAMEPAD_X;
	case 3:
		return pThis->wButtons & XINPUT_GAMEPAD_Y;
	case 4:
		return pThis->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
	case 5:
		return pThis->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
	case 6:
		return pThis->wButtons & XINPUT_GAMEPAD_BACK;
	default:
		return pThis->wButtons & XINPUT_GAMEPAD_START;
	}

}
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{
	if(pThis->wButtons & XINPUT_GAMEPAD_DPAD_UP)
		return 0;
	if(pThis->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		return 9000;
	if(pThis->wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		return 18000;
	if(pThis->wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		return 27000;
	return (WORD)-1;
}
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->thumbRY; }
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->thumbRX; }
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->rightTrigger; }

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{ return ""; }
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return -32768; }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return -32768; }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return 32767; }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return 32767; }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return 255; }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return 35900; }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return (WORD)-1; /* AKA: JOY_POVCENTERED */ }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return 8; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{ return 6; }
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return 8; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return 6; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return -32768; }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return 32767; }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return -32768; }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return 32767; }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return 255; }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
					#     # ### #     #  #####   #####
					#  #  #  #  ##    # #     # #     #
					#  #  #  #  # #   #       #       #
					#  #  #  #  #  #  #  #####   #####
					#  #  #  #  #   # #       # #
					#  #  #  #  #    ## #     # #
					 ## ##  ### #     #  #####  #######

	 #     # #     #  #####  #     #  #####  ####### ####### #     #
	 ##   ## ##   ## #     #  #   #  #     #    #    #       ##   ##
	 # # # # # # # # #         # #   #          #    #       # # # #
	 #  #  # #  #  #  #####     #     #####     #    #####   #  #  #
	 #     # #     #       #    #          #    #    #       #     #
	 #     # #     # #     #    #    #     #    #    #       #     #
	 #     # #     #  #####     #     #####     #    ####### #     #

	joyGetPosEx is not in Windows NT 3.1 despite what the MSDN 2000 docs say.
	Windows headers define it as WINVER >= 0x400 which is NT 4.0+.
=============================================================================*/
#else /* MMSYSTEM */
#include <windows.h>
#include <mmsystem.h>
#include <RegStr.h> /* REGSTR_XXX */
#pragma comment(lib, "winmm.lib") /* joyXXX */
#ifdef __cplusplus
	extern "C" {
#endif
void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{
	++pThis->joystick;
}

void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{
#if !defined(WSJOYSTICK_NOWINNT31)
	JOYINFO joyinfo;
	pJoystick->mHandle = pThis->joystick;
	pJoystick->mhWinMM = LoadLibraryA("winmm.dll");
	pJoystick->mpfnJoyGetPosEx = (joyGetPosExProc)
		GetProcAddress(pJoystick->mhWinMM, "joyGetPosEx");
	if(joyGetPos(pJoystick->mHandle, &joyinfo) != JOYERR_NOERROR)
	{
		FreeLibrary(pJoystick->mhWinMM);
		pJoystick->mhWinMM = NULL;
	}
#else
	pJoystick->mHandle = pThis->joystick;
#endif
}
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{
}

wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{
#if !defined(WSJOYSTICK_NOWINNT31)
	return pThis->mhWinMM != NULL;
#else
	JOYINFO joyinfo;
	return joyGetPos(pThis->mHandle, &joyinfo) == JOYERR_NOERROR;
#endif
}
void WSJoystick_Destroy(WSJOYSTICK* pThis)
{
#if !defined(WSJOYSTICK_NOWINNT31)
	FreeLibrary(pThis->mhWinMM);
#endif
}

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{
	pIt->maxjoysticks = joyGetNumDevs();
	pIt->joystick = 0;
}
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{
	return pIt->joystick >= pIt->maxjoysticks;
}
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{
	int err;
#if !defined(WSJOYSTICK_NOWINNT31)
	if(pThis->mpfnJoyGetPosEx)
	{
		state->dwSize = sizeof(WSJOYSTICKSTATE);
		state->dwFlags = JOY_RETURNALL|JOY_RETURNPOVCTS;
		err =(*pThis->mpfnJoyGetPosEx)(pThis->mHandle, state);
	}
	else
	{
		err = joyGetPos(pThis->mHandle, (JOYINFO*)&state->dwXpos);
		state->dwButtons = state->dwRpos;
	}
#else
	state->dwSize = sizeof(WSJOYSTICKSTATE);
	state->dwFlags = JOY_RETURNALL|JOY_RETURNPOVCTS;
	err = joyGetPosEx(pThis->mHandle, (JOYINFOEX*)state);
#endif
	if(err != JOYERR_NOERROR)
	{
		memset(state, 0, sizeof(WSJOYSTICKSTATE));
	}
}
typedef struct WSJOYSTICKCAPSNT31
{
	uint16_t wMid;
	uint16_t wPid;
	char szPname[32]; /* MAXPNAMELEN */
	unsigned int wXmin;
	unsigned int wXmax;
	unsigned int wYmin;
	unsigned int wYmax;
	unsigned int wZmin;
	unsigned int wZmax;
	unsigned int wNumButtons;
	unsigned int wPeriodMin;
	unsigned int wPeriodMax;
} WSJOYSTICKCAPSNT31;
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{
	UINT uSize;
#if !defined(WSJOYSTICK_NOWINNT31)
	if(!pThis->mpfnJoyGetPosEx)
	{
		uSize = sizeof(WSJOYSTICKCAPSNT31);
		caps->wRmin = caps->wRmax = caps->wUmin = caps->wUmax
			= caps->wVmin = caps->wVmax = caps->wCaps = caps->wMaxAxes
			= caps->wNumAxes = caps->wMaxButtons = 0;
		caps->szRegKey[0] = caps->szOEMVxD[0] = '\0';
	}
	else
#endif
		uSize = sizeof(WSJOYSTICKCAPS);
	joyGetDevCapsA(pThis->mHandle, (JOYCAPS*)caps, uSize);
}

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->dwXpos; }
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->dwYpos; }
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->dwZpos; }
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{ return pThis->dwButtons & (1 << button); }
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->dwPOV; }
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->dwRpos; }
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->dwUpos; }
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->dwVpos; }

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{ return pThis->wMid; }
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{ return pThis->wPid; }
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{
	HKEY hTopKey;
	HKEY hKey;
	DWORD regsize;
	LONG regresult;
	char regkey[256];
	char regvalue[256];
	char regname[256];
	size_t joyregkeylen;

	/* IF NOT WINE */
	joyregkeylen = strlen(pThis->szRegKey);

	CopyMemory(&regkey[0], REGSTR_PATH_JOYCONFIG,
		strcountof(REGSTR_PATH_JOYCONFIG) * sizeof(char));
	regkey[strcountof(REGSTR_PATH_JOYCONFIG)] = '\\';
	CopyMemory(&regkey[strcountof(REGSTR_PATH_JOYCONFIG) + 1], pThis->szRegKey,
		joyregkeylen * sizeof(char));
	regkey[strcountof(REGSTR_PATH_JOYCONFIG) + 1
		+ joyregkeylen] = '\\';
	CopyMemory(&regkey[strcountof(REGSTR_PATH_JOYCONFIG) + 1
		+ joyregkeylen + 1], REGSTR_KEY_JOYCURR,
		strcountof(REGSTR_KEY_JOYCURR) * sizeof(char));
	regkey[strcountof(REGSTR_PATH_JOYCONFIG) + 1
		+ joyregkeylen + 1 + strcountof(REGSTR_KEY_JOYCURR)] = '\0';
	hTopKey = HKEY_LOCAL_MACHINE;
	{
		typedef LONG (WINAPI* REGQUERYVALUEEXAPROC)(HKEY hKey, LPCSTR lpValueName,
			LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
		typedef LONG (WINAPI* REGOPENKEYAPROC)(HKEY hKey, LPCSTR lpSubKey,  PHKEY phkResult);
		typedef LONG (WINAPI* REGCLOSEKEYPROC)(HKEY hKey);
		REGQUERYVALUEEXAPROC RegQueryValueExAProc;
		REGOPENKEYAPROC RegOpenKeyAProc;
		REGCLOSEKEYPROC RegCloseKeyProc;
		#if !defined(WSJOYSTICK_STATICADVAPI32)
			HINSTANCE hAdvApi32 = LoadLibraryA("advapi32.dll");
			RegQueryValueExAProc = (REGQUERYVALUEEXAPROC)
				GetProcAddress(hAdvApi32, "RegQueryValueExA");
			RegOpenKeyAProc = (REGOPENKEYAPROC)
				GetProcAddress(hAdvApi32, "RegOpenKeyA");
			RegCloseKeyProc = (REGCLOSEKEYPROC)
				GetProcAddress(hAdvApi32, "RegCloseKey");
		#else
			RegQueryValueExAProc = RegQueryValueExA;
			RegOpenKeyAProc = RegOpenKeyA;
			RegCloseKeyProc = RegCloseKey;
		#endif
		regresult = (*RegOpenKeyAProc)(hTopKey, regkey, &hKey);
		if (regresult != ERROR_SUCCESS)
		{
			hTopKey = HKEY_CURRENT_USER;
			regresult = (*RegOpenKeyAProc)(hTopKey, regkey, &hKey);
		}
		if (regresult != ERROR_SUCCESS)
			return "";

		regsize = sizeof(regname);
		/*CopyMemory(&regvalue[0], "Joystick", strcountof("Joystick"));
		CopyMemory(&regvalue[strcountof("Joystick")],
			pThis->szRegKey, joyregkeylen * sizeof(char));
		CopyMemory(&regvalue[strcountof("Joystick")
			+ joyregkeylen], REGSTR_VAL_JOYOEMNAME,
			strcountof(REGSTR_VAL_JOYOEMNAME) * sizeof(char));
		regkey[strcountof("Joystick")
			+ joyregkeylen + strcountof(REGSTR_VAL_JOYOEMNAME)] = '\0';*/
		wsprintfA(regvalue, "Joystick%d%s", joystick->mHandle+1,
			REGSTR_VAL_JOYOEMNAME);
		regresult = (*RegQueryValueExAProc)(hKey,
			regvalue, 0, 0, (LPBYTE)regname, &regsize);
		(*RegCloseKeyProc)(hKey);
		if (regresult != ERROR_SUCCESS)
			return "";

		wsprintfA(regkey, "%s\\%s", REGSTR_PATH_JOYOEM, regname);
		/* 3/4 for EX==0, KEY_READ*/
		regresult = (*RegOpenKeyAProc)(hTopKey, regkey, &hKey);
		if (regresult != ERROR_SUCCESS)
			return "";

		regsize = sizeof(regvalue);
		regresult = (*RegQueryValueExAProc)(hKey,
			REGSTR_VAL_JOYOEMNAME, 0, 0, NULL, &regsize);
		if (regresult == ERROR_SUCCESS)
		{
			regsize = sizeof(pThis->szOEMVxD);
			regresult = (*RegQueryValueExAProc)(hKey,
				REGSTR_VAL_JOYOEMNAME, 0, 0,
				(LPBYTE) pThis->szOEMVxD, &regsize);
		}
		else
			((char*)(pThis->szOEMVxD))[0] = '\0';
		(*RegCloseKeyProc)(hKey);
		#if !defined(WSJOYSTICK_STATICADVAPI32)
			FreeLibrary(hAdvApi32);
		#endif
	}
	return pThis->szOEMVxD;
}
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return pThis->wXmin; }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return pThis->wYmin; }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return pThis->wZmin; }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return pThis->wXmax; }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return pThis->wYmax; }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return pThis->wZmax; }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return 35900; }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return (WORD)-1; /* AKA: JOY_POVCENTERED */ }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return pThis->wNumButtons; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{ return pThis->wNumAxes; }
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return pThis->wMaxButtons; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return pThis->wMaxAxes; }
int WSJoystickCaps_GetPollingMin(const WSJOYSTICKCAPS* pThis)
{ return pThis->wPeriodMin; }
int WSJoystickCaps_GetPollingMax(const WSJOYSTICKCAPS* pThis)
{ return pThis->wPeriodMax; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return pThis->wRmin; }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return pThis->wRmax; }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return pThis->wUmin; }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return pThis->wUmax; }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return pThis->wVmin; }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return pThis->wVmax; }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return (pThis->wCaps & JOYCAPS_HASR) == JOYCAPS_HASR; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return (pThis->wCaps & JOYCAPS_HASZ) == JOYCAPS_HASZ; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return (pThis->wCaps & JOYCAPS_HASU) == JOYCAPS_HASU; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return (pThis->wCaps & JOYCAPS_HASV) == JOYCAPS_HASV; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return (pThis->wCaps & JOYCAPS_HASPOV) == JOYCAPS_HASPOV; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return (pThis->wCaps & JOYCAPS_POV4DIR) == JOYCAPS_POV4DIR; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return (pThis->wCaps & JOYCAPS_POVCTS) == JOYCAPS_POVCTS; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
#endif /* XINPUT switch */
/*=============================================================================
					#    #     # ###  #####     #
				   # #   ##   ##  #  #     #   # #
				  #   #  # # # #  #  #        #   #
				 #     # #  #  #  #  #  #### #     #
				 ####### #     #  #  #     # #######
				 #     # #     #  #  #     # #     #
				 #     # #     # ###  #####  #     #

		 #                     #
		 #        ####  #    # #       ###### #    # ###### #
		 #       #    # #    # #       #      #    # #      #
		 #       #    # #    # #       #####  #    # #####  #
		 #       #    # # ## # #       #      #    # #      #
		 #       #    # ##  ## #       #       #  #  #      #
		 #######  ####  #    # ####### ######   ##   ###### ######
=============================================================================*/
#elif defined(__amigaos__)
#include <stdio.h> /* NULL */
#include <libraries/lowlevel.h>
#if defined(__SASC) || defined(WARPOS) || defined(AROS)
	#include <proto/exec.h>
	#include <proto/lowlevel.h>
	#include <proto/graphics.h>
#else
	#ifdef MORPHOS
		#include <ppcinline/exec.h>
		#include <ppcinline/lowlevel.h>
		#include <ppcinline/graphics.h>
	#else
		#include <proto/dos.h>
		#include <inline/dos.h>
		#include <inline/exec.h>
		#include <inline/lowlevel.h>
		#include <inline/graphics.h>
	#endif
#endif
#define WSJOYSTICK_AXIS_MAX 1
#define WSJOYSTICK_AXIS_MIN -1
#define WSJOYSTICK_AXIS_CENTER 0
#ifdef __cplusplus
	extern "C" {
#endif
#define WSJOYSTICK_MAX_JOYSTICKS 2 /* AMIGA has 2 ports */
extern struct ExecBase* SysBase;
extern struct Library* LowLevelBase;
wsbool WSJoystickInit()
{
	if(!LowLevelBase)
		if((LowLevelBase = OpenLibrary("lowlevel.library", 37)) == NULL)
			return FALSE;
	return TRUE;
}

void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{
	++pThis->joystick;
}

void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{
	pJoystick->mHandle = pThis->joystick;
	pJoystick->mButtonCache[0] = JPF_BUTTON_RED;
	pJoystick->mButtonCache[1] = JPF_BUTTON_BLUE;
	pJoystick->mButtonCache[2] = JPF_BUTTON_PLAY;
	pJoystick->mButtonCache[3] = JPF_BUTTON_YELLOW;
	pJoystick->mButtonCache[4] = JPF_BUTTON_GREEN;
	pJoystick->mButtonCache[5] = JPF_BUTTON_FORWARD;
	pJoystick->mButtonCache[6] = JPF_BUTTON_REVERSE;
}
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{
}

wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{
	return pThis->mHandle < WSJOYSTICK_MAX_JOYSTICKS;
}
void WSJoystick_Destroy(WSJOYSTICK* pThis)
{
}

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{
	pIt->joystick = 0;
}
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{
	return pIt->joystick >= WSJOYSTICK_MAX_JOYSTICKS;
}
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{
	state->state = ReadJoyPort(pThis->mHandle);
	state->joystick = pThis;
}
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{ caps->state = ReadJoyPort(pThis->mHandle); }

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{
	int res;
	if(pThis->state & JP_DIRECTION_MASK)
	{
		if(pThis->state & JPF_JOY_LEFT)
		 res = WSJOYSTICK_AXIS_MIN;
		else if(pThis->state & JPF_JOY_RIGHT)
			 res = WSJOYSTICK_AXIS_MAX;
		else
			 res = WSJOYSTICK_AXIS_CENTER;
	}
	else
		res = WSJOYSTICK_AXIS_CENTER;
	return res;
}
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{
	int res;
	if(pThis->state & JP_DIRECTION_MASK)
	{
		if(pThis->state & JPF_JOY_UP)
		 res = WSJOYSTICK_AXIS_MIN;
		else if(pThis->state & JPF_JOY_DOWN)
			 res = WSJOYSTICK_AXIS_MAX;
		else
			 res = WSJOYSTICK_AXIS_CENTER;
	}
	else
		res = WSJOYSTICK_AXIS_CENTER;
	return res;
}
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{ return (pThis->state & pThis->joystick->mButtonCache[button])
	== pThis->joystick->mButtonCache[button]; }
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{ return ""; }
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_CENTER; }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return (pThis->state & JP_TYPE_MASK) == JP_TYPE_GAMECTLR ? 7 : 3; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_AXES; }
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_BUTTONS; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_AXES; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
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

		  #####
		 #     # #####  #####   ####  #    # ###### #####  ####
		 #       #    # #    # #    # #   #  #        #   #
		  #####  #    # #    # #    # ####   #####    #    ####
			   # #####  #####  #    # #  #   #        #        #
		 #     # #      #   #  #    # #   #  #        #   #    #
		  #####  #      #    #  ####  #    # ######   #    ####
=============================================================================*/
#elif defined(macintosh)
#include <InputSprocket.h>
#include <string.h> /* memcpy */
#ifdef __cplusplus
	extern "C" {
#endif
#if !defined(WIKISERVER_VERSION)
	#define WSMemoryCopy memcpy
#endif
#define	ISpSymmetricAxisToFloat(axis) \
	((((float) axis) - kISpAxisMiddle) / (kISpAxisMaximum-kISpAxisMiddle))
#define	ISpAsymmetricAxisToFloat(axis) \
	(((float) axis) / (kISpAxisMaximum))
void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{
	++pThis->index;
}

void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{
	OSStatus err;
	ISpElementListReference elelist;
	uint32_t numeles, iele;
	ISpElementReference eles[64];
	ISpStartup();
	pJoystick->mHandle = pThis->joysticks[pThis->index];
	ISpDevices_Activate(1, &pJoystick->mHandle);
	err = ISpDevice_GetElementList(pJoystick->mHandle, &elelist);
	assert(err == noErr && elelist != NULL);
	err = ISpElementList_Extract(elelist, 64, &numeles, eles);
	assert(err == noErr && numeles != 0);
	pJoystick->mXAxis = pJoystick->mYAxis = pJoystick->mZAxis =
		pJoystick->mUAxis = pJoystick->mVAxis = pJoystick->mRudder =
		pJoystick->mPOV = NULL;
	for(iele = 0; iele < numeles; ++iele)
	{
		ISpElementInfo eleinfo;
		err = ISpElement_GetInfo(eles[iele], &eleinfo);
		assert(err == noErr);
		switch(eleinfo.theKind)
		{
		case kISpElementKind_Button:
			if (pJoystick->mNumButtons < WSJOYSTICK_MAX_BUTTONS)
			{
				pJoystick->mButtons[pJoystick->mNumButtons] = eles[iele];
				++pJoystick->mNumButtons;
			}
		break;

		case kISpElementKind_Delta:
			switch (eleinfo.theLabel)
			{
			case kISpElementLabel_Delta_X:
				pJoystick->mXAxis = eles[iele];
				break;

			case kISpElementLabel_Delta_Y:
				pJoystick->mYAxis = eles[iele];
				break;

			case kISpElementLabel_Delta_Z:
				pJoystick->mZAxis = eles[iele];
				break;

			case kISpElementLabel_Delta_Rx:
				pJoystick->mUAxis = eles[iele];
				break;
			case kISpElementLabel_Delta_Ry:
				pJoystick->mVAxis = eles[iele];
				break;
			}
		break;
		default:
			switch (eleinfo.theLabel)
			{
			case kISpElementLabel_Axis_Rudder:
				pJoystick->mRudder = eles[iele];
				break;
			case kISpElementLabel_Pad_POV:
				pJoystick->mPOV = eles[iele];
				break;
			}
			break;
		}
	}
}
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{
	ISpShutdown();
}

wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{
	(void)(pThis);
	return TRUE;
}

void WSJoystick_Destroy(WSJOYSTICK* pThis)
{
	ISpDevices_Deactivate(1, &pThis->mHandle);
	ISpShutdown();
}

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{
	UInt32 count = 0;
	OSStatus err;
	/* Present, ISpStartup == 1.1 */
	if(
		#if !defined(TARGET_CARBON) || !TARGET_CARBON
			ISpGetVersion == NULL ||
		#endif
		ISpStartup == NULL)
		return;
	ISpStartup();
	pIt->maxjoysticks = 0;
	/* ISpDeviceClass */
	err = ISpDevices_ExtractByClass(kISpDeviceClass_Joystick,
		WSJOYSTICK_MAX_JOYSTICKS - pIt->maxjoysticks, &count,
		&pIt->joysticks[pIt->maxjoysticks]);
	pIt->maxjoysticks += count;
	err = ISpDevices_ExtractByClass(kISpDeviceClass_Wheel,
		WSJOYSTICK_MAX_JOYSTICKS - pIt->maxjoysticks, &count,
		&pIt->joysticks[pIt->maxjoysticks]);
	pIt->maxjoysticks += count;
	#if defined(kISpDeviceClass_Gamepad)
		err = ISpDevices_ExtractByClass(kISpDeviceClass_Gamepad,
			WSJOYSTICK_MAX_JOYSTICKS - pIt->maxjoysticks, &count,
			&pIt->joysticks[pIt->maxjoysticks]);
		pIt->maxjoysticks += count;
	#endif
}
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{
	return pIt->index == pIt->maxjoysticks;
}
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{
	state->joystick = pThis;
}
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{
	caps->joystick = pThis;
}

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{
	ISpAxisData a;
	ISpElement_GetSimpleState(pThis->joystick->mXAxis, &a);
	return a;
}
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{
	ISpAxisData a;
	ISpElement_GetSimpleState(pThis->joystick->mYAxis, &a);
	return a;
}
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{
	ISpAxisData a;
	ISpElement_GetSimpleState(pThis->joystick->mZAxis, &a);
	return a;
}
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{
	ISpAxisData a;
	ISpElement_GetSimpleState(
		pThis->joystick->mButtons[pThis->joystick->mNumButtons], &a);
	return a;
}
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{
	ISpAxisData a;
	ISpElement_GetSimpleState(pThis->joystick->mPOV, &a);
	return a;
}
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{
	ISpAxisData a;
	ISpElement_GetSimpleState(pThis->joystick->mRudder, &a);
	return a;
}
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{
	ISpAxisData a;
	ISpElement_GetSimpleState(pThis->joystick->mUAxis, &a);
	return a;
}
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{
	ISpAxisData a;
	ISpElement_GetSimpleState(pThis->joystick->mVAxis, &a);
	return a;
}

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{
	ISpDeviceDefinition devdef;
	ISpDevice_GetDefinition(joystick->mHandle,
		sizeof(ISpDeviceDefinition), &devdef);
	WSMemoryCopy(pThis->name, &devdef.deviceName[1], devdef.deviceName[0]);
	pThis->name[devdef.deviceName[0]] = '\0';
	return pThis->name;
}
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMinimum; }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMinimum; }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMinimum; }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMaximum; }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMaximum; }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMaximum; }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMinimum; }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMaximum; }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMiddle; }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mNumButtons; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{
	int numaxes = 0;
	if(pThis->joystick->mXAxis)
		++numaxes;
	if(pThis->joystick->mYAxis)
		++numaxes;
	if(pThis->joystick->mZAxis)
		++numaxes;
	if(pThis->joystick->mUAxis)
		++numaxes;
	if(pThis->joystick->mVAxis)
		++numaxes;
	if(pThis->joystick->mRudder)
		++numaxes;
	if(pThis->joystick->mPOV)
		++numaxes;
	return numaxes;
}
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_BUTTONS; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_AXES; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMinimum; }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMaximum; }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMinimum; }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMaximum; }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMinimum; }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return kISpAxisMaximum; }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mRudder != NULL; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mZAxis != NULL; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mUAxis != NULL; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mVAxis != NULL; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mPOV != NULL; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
				   #######  #####     #     # ####### ###
				 # #     # #     #    ##   ## #        #
				 # #     # #          # # # # #        #
				 # #     #  #####     #  #  # #####    #
				 # #     #       #    #     # #        #
				 # #     # #     #    #     # #        #
				 # #######  #####     #     # #       ###

							[M]ade [F]or [i]OS (7+)
	https://stackoverflow.com/questions/55226373/
	how-do-i-use-apples-gamecontroller-framework-from-a-macos-command-line-tool

	Note that GCController devices WILL also enumerate as IOKit devices.
	GCController has supportsHIDDevice which is OSX 11+ only, as well as
	deviceRef though the latter is private and thus banned from the App Store.
=============================================================================*/
#elif defined(WSJOYSTICKMACIOS)
#include <GameController/GameController.h>
#ifdef __cplusplus
	extern "C"
#endif
#define WSJOYSTICK_AXIS_MIN -32768
#define WSJOYSTICK_AXIS_MAX 32767
#define WSJOYSTICK_AXIS_CENTER 0
void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{
	++pThis->index;
}

void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{
	GCGamepad* gg;
	GCMicroGamepad* mg;
	GCExtendedGamepad* eg;
	if(@available(iOS 7.0, *))
	{}
	else
	{
		pJoystick->mHandle = NULL;
		return;
	}
	pJoystick->mHandle = [GCController controllers][pThis->index];
	eg = [pJoystick->mHandle extendedGamepad];
	if(eg)
	{
		pJoystick->mAxes[0] = [eg dpad];
		pJoystick->mAxes[1] = [eg leftThumbstick];
		pJoystick->mAxes[2] = [eg rightThumbstick];
		pJoystick->mButtons[0] = [eg buttonA];
		pJoystick->mButtons[1] = [eg buttonB];
		pJoystick->mButtons[2] = [eg buttonX];
		pJoystick->mButtons[3] = [eg buttonY];
		pJoystick->mButtons[4] = [eg buttonOptions];
		pJoystick->mButtons[5] = [eg buttonHome];
		pJoystick->mButtons[6] = [eg buttonMenu];
		pJoystick->mButtons[7] = [eg leftShoulder];
		pJoystick->mButtons[8] = [eg rightShoulder];
		pJoystick->mButtons[9] = [eg leftTrigger];
		pJoystick->mButtons[10] = [eg rightTrigger];
		pJoystick->mButtons[11] = [eg leftThumbstickButton];
		pJoystick->mButtons[12] = [eg rightThumbstickButton];
		pJoystick->mNumAxes = 2;
		pJoystick->mNumButtons = 13;
		return;
	}
	gg = [pJoystick->mHandle gamepad];
	if(gg)
	{
		pJoystick->mAxes[0] = [gg dpad];
		pJoystick->mButtons[0] = [gg buttonA];
		pJoystick->mButtons[1] = [gg buttonB];
		pJoystick->mButtons[2] = [gg buttonX];
		pJoystick->mButtons[3] = [gg buttonY];
		pJoystick->mButtons[4] = [gg leftShoulder];
		pJoystick->mButtons[5] = [gg rightShoulder];
		pJoystick->mNumAxes = 2;
		pJoystick->mNumButtons = 6;
		return;
	}
	if(@available(iOS 9.0, *))
		mg = [pJoystick->mHandle microGamepad];
	else /* its a GCControllerDirectionPad in 7/8 */
	{
		mg = NULL;
		GCControllerDirectionPad* dp =
			[pJoystick->mHandle microGamepad];
		if(dp)
		{
			pJoystick->mAxes[0] = dp;
			pJoystick->mNumAxes = 2;
			pJoystick->mNumButtons = 0;
			return;
		}
	}
	if(mg)
	{
		pJoystick->mAxes[0] = [mg dpad];
		pJoystick->mButtons[0] = [mg buttonA];
		pJoystick->mButtons[2] = [mg buttonX];
		pJoystick->mNumAxes = 2;
		pJoystick->mNumButtons = 2;
		return;
	}
	pJoystick->mNumAxes = pJoystick->mNumButtons = 0;
}
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{
}

wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{
	return pThis->mHandle != NULL;
}
void WSJoystick_Destroy(WSJOYSTICK* pThis)
{
}

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{
	pIt->index = 0;
}
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{
	return pIt->index >= [[GCController controllers] count];
}
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{
	state->joystick = pThis;
}
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{ caps->joystick = pThis; }

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{
	if([[pThis->joystick->mAxes[0] xAxis] value])
		return [[pThis->joystick->mAxes[0] xAxis] value];
	if(pThis->joystick->mNumButtons == 13
	   && [[pThis->joystick->mAxes[1] xAxis] value])
		return [[pThis->joystick->mAxes[1] xAxis] value];
	return WSJOYSTICK_AXIS_CENTER;
}
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{
	if([[pThis->joystick->mAxes[0] yAxis] value])
		return [[pThis->joystick->mAxes[0] yAxis] value];
	if(pThis->joystick->mNumButtons == 13
	   && [[pThis->joystick->mAxes[1] yAxis] value])
		return [[pThis->joystick->mAxes[1] yAxis] value];
	return WSJOYSTICK_AXIS_CENTER;
}
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{ return [pThis->joystick->mButtons[button] isPressed]; }
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{ return ""; }
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_CENTER; }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mNumButtons; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mNumAxes; }
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_BUTTONS; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_AXES; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
	 ######     #    ######  #     # ### #     #    #     # ### ######
	 #     #   # #   #     # #  #  #  #  ##    #    #     #  #  #     #
	 #     #  #   #  #     # #  #  #  #  # #   #    #     #  #  #     #
	 #     # #     # ######  #  #  #  #  #  #  #    #######  #  #     #
	 #     # ####### #   #   #  #  #  #  #   # #    #     #  #  #     #
	 #     # #     # #    #  #  #  #  #  #    ##    #     #  #  #     #
	 ######  #     # #     #  ## ##  ### #     #    #     # ### ######

							Also supported on iOS 16+
				https://developer.apple.com/documentation/iokit
=============================================================================*/
#elif defined(__MACH__)
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <Kernel/IOKit/hidsystem/IOHIDUsageTables.h>
/*
	Needed for codewarrior link error with mach_port_deallocate()
	http://www.macosx.com/forums/archive/index.php/t-68069.html
	mach_port.h itself isn't guarded with extern "C"
*/
#if defined(__cplusplus)
	extern "C" {
#endif
#include <mach/mach_port.h>
#if defined(__cplusplus)
	}
#endif
#include <mach/mach.h> /* normally includes mach_port.h */

#ifdef __cplusplus
	extern "C" {
#endif
#define WSJoystickLogError printf
#define WSJoystickLogDebug printf

int GetIntFromCFDictionary(CFDictionaryRef cfDict, CFStringRef key)
{
	int res;
	if(CFNumberGetValue(
		(CFNumberRef) CFDictionaryGetValue(cfDict, key),
					kCFNumberIntType, &res))
	   return res;
	return 0;
}
void GetCookie(CFDictionaryRef Data, IOHIDElementCookie* cookie)
{
	CFNumberGetValue(
		(CFNumberRef) CFDictionaryGetValue(Data,
			CFSTR(kIOHIDElementCookieKey)),
		kCFNumberIntType, cookie);
}
#if 0
void AddCookieToQueue(IOHIDQueueInterface** mppQueue, CFTypeRef Data, int i)
{ /* 3rd Param of addElement is flags (none yet) */
	if ( (*mppQueue)->addElement(mppQueue, m_pCookies[i], 0) != S_OK )
		WSJoystickLogError(("HID device: adding element failed"));
}
void InitCookiesAndQueue(IOHIDElementCookie** cookies,
	size_t dwSize, wsbool bQueue, IOHIDQueueInterface*** mppQueue)
{
	*cookies = (IOHIDElementCookie*)
		malloc(sizeof(IOHIDElementCookie) * dwSize);
	if (bQueue)
	{
		assert(*mppQueue == NULL);
		mppQueue = (*(*mppDevice)->allocQueue)(mppDevice);
		if (!mppQueue)
			WSJoystickLogError(("HID device: allocQueue failed"));
		/* Param 2, flags, none yet */
		else if ((*(*mppQueue)->create)(mppQueue, 0, 512) != S_OK)
			WSJoystickLogError(("HID device: create failed"));
	}
	memset(*cookies, 0, sizeof(**cookies) * dwSize);
}
#endif

typedef struct WSHIDDEVICE
{
	IOHIDDeviceInterface** mppDevice;
	CFMutableDictionaryRef mPropDict;
	CFMutableArrayRef mCookies;
} WSHIDDEVICE;
typedef struct WSHIDITERATOR
{
	mach_port_t mMasterPort;
	io_iterator_t mIterator;
	io_object_t mCurObject;
	CFMutableDictionaryRef mDevDict;
} WSHIDITERATOR;
void WSHIDIterator_Construct(WSHIDITERATOR* pThis)
{
	/* Create the mach port needed for getting the device list */
	if(IOMasterPort(bootstrap_port, &pThis->mMasterPort) != kIOReturnSuccess)
		WSJoystickLogError("Could not create mach port");
}

wsbool WSHIDIterator_Create(WSHIDITERATOR* pThis, int hidpage, int hidusage)
{
	/* Get the list of devices in the machine in a dictionary */
	pThis->mDevDict = IOServiceMatching(kIOHIDDeviceKey);
	if(pThis->mDevDict == NULL)
	{
		WSJoystickLogError( ("IOServiceMatching(kIOHIDDeviceKey) failed") );
		pThis->mIterator = 0; /*NULL;*/
		pThis->mCurObject = 0; /*NULL;*/
		return FALSE;
	}

	/* filter down the services to what we want */
	if (hidpage != -1)
	{
		CFNumberRef pType = CFNumberCreate(kCFAllocatorDefault,
									kCFNumberIntType, &hidpage);
		CFDictionarySetValue(pThis->mDevDict,
			CFSTR(kIOHIDPrimaryUsageKey), pType);
		CFRelease(pType);
	}
	if (hidusage != -1)
	{
		CFNumberRef pClass = CFNumberCreate(kCFAllocatorDefault,
									kCFNumberIntType, &hidusage);
		CFDictionarySetValue(pThis->mDevDict,
			CFSTR(kIOHIDPrimaryUsagePageKey), pClass);
		CFRelease(pClass);
	}

	/* get the maching services */
	if(IOServiceGetMatchingServices(pThis->mMasterPort,
			pThis->mDevDict, &pThis->mIterator) != kIOReturnSuccess)
	{
		WSJoystickLogError(("No Matching HID Services"));
		pThis->mIterator = 0; /*NULL;*/
		pThis->mCurObject = 0; /*NULL;*/
		return FALSE;
	}

	/* Were there any devices matched? */
	if(pThis->mIterator == 0)
	{
		pThis->mCurObject = 0;/*NULL;*/
		return FALSE; /* No devices found */
	}

	/* Go to the first device */
	pThis->mCurObject = IOIteratorNext(pThis->mIterator);
	return TRUE;
}
wsbool WSHIDIterator_ReCreate(WSHIDITERATOR* pThis, int hidpage, int hidusage)
{
	IOObjectRelease(pThis->mCurObject);
	IOObjectRelease(pThis->mIterator);
	CFRelease(pThis->mDevDict);
	return WSHIDIterator_Create(pThis, hidpage, hidusage);
}
void WSHIDIterator_Destruct(WSHIDITERATOR* pThis)
{
	IOObjectRelease(pThis->mCurObject);
	IOObjectRelease(pThis->mIterator);
	CFRelease(pThis->mDevDict);
	mach_port_deallocate(mach_task_self(), pThis->mMasterPort);
}

/*
	Returns true if a cookie of the device is active - for example if a key is
	held down, joystick button pressed, caps lock active, etc..
*/
int32_t WSHIDDevice_GetCookieIndexValue(WSHIDDEVICE* pThis,
										CFMutableDictionaryRef index)
{
	IOHIDEventStruct hidevent;
	IOHIDElementCookie cookie;
	GetCookie(index, &cookie);
	(*(*pThis->mppDevice)->getElementValue)(
		pThis->mppDevice, cookie, &hidevent);
	return hidevent.value;
}

wsbool WSHIDDevice_IsCreated(const WSHIDDEVICE* pThis)
{	return pThis->mppDevice != NULL; }
void WSHIDDevice_Destruct(WSHIDDEVICE* pThis)
{
	CFRelease(pThis->mPropDict);
	if (pThis->mppDevice != NULL)
	{
		(*(*pThis->mppDevice)->close)(pThis->mppDevice);
		(*(*pThis->mppDevice)->Release)(pThis->mppDevice);
	}
}

void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{
	IOObjectRelease(pThis->mCurObject);
	pThis->mCurObject = IOIteratorNext(pThis->mIterator);

	if(!pThis->mCurObject)
	{
		if(!pThis->mDidGamepadsAlso)
		{
			WSHIDIterator_ReCreate((WSHIDITERATOR*)pThis,
				kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
			pThis->mDidGamepadsAlso = 1;
		}
	}
}

void WSJoystick_BuildCookies(WSJOYSTICK* pThis, CFArrayRef cookies)
{
	int i, nUsage, nPage;
	CFIndex count = CFArrayGetCount(cookies);
	for (i = 0; i < count; ++i)
	{
		CFArrayRef subcookies = (CFArrayRef)CFDictionaryGetValue(
				(CFDictionaryRef)CFArrayGetValueAtIndex(cookies, i),
				CFSTR(kIOHIDElementKey)
											  );
		if(subcookies)
			WSJoystick_BuildCookies(pThis, subcookies); /* recurse */
		/* usage/page */
		CFNumberGetValue(
			(CFNumberRef) CFDictionaryGetValue(
					(CFDictionaryRef) CFArrayGetValueAtIndex(cookies, i),
					CFSTR(kIOHIDElementUsageKey)
									),
			kCFNumberIntType, &nUsage);
		CFNumberGetValue(
			(CFNumberRef) CFDictionaryGetValue(
					(CFDictionaryRef) CFArrayGetValueAtIndex(cookies, i),
					CFSTR(kIOHIDElementUsagePageKey)
									),
			kCFNumberIntType, &nPage);

#if 0
			WSJoystickLogDebug(("[%i][%i]"), nUsage, nPage);
#endif
		if (nPage == kHIDPage_Button && nUsage <= WSJOYSTICK_MAX_BUTTONS)
		{
			pThis->mButtons[nUsage - 1] = (CFMutableDictionaryRef)
				CFArrayGetValueAtIndex(cookies, i);
			++pThis->mNumButtons;
		}
		else if (nPage == kHIDPage_GenericDesktop)
		{
			switch(nUsage)
			{
				case kHIDUsage_GD_X:
					pThis->mXAxis = (CFMutableDictionaryRef)
						CFArrayGetValueAtIndex(cookies, i);
					break;
				case kHIDUsage_GD_Y:
					pThis->mYAxis = (CFMutableDictionaryRef)
						CFArrayGetValueAtIndex(cookies, i);
					break;
				case kHIDUsage_GD_Z:
					pThis->mZAxis = (CFMutableDictionaryRef)
						CFArrayGetValueAtIndex(cookies, i);
					break;
				case kHIDUsage_GD_Rx:
					pThis->mUAxis = (CFMutableDictionaryRef)
						CFArrayGetValueAtIndex(cookies, i);
					break;
				case kHIDUsage_GD_Ry:
					pThis->mVAxis = (CFMutableDictionaryRef)
						CFArrayGetValueAtIndex(cookies, i);
					break;
				case kHIDUsage_GD_Hatswitch:
					pThis->mPOV = (CFMutableDictionaryRef)
						CFArrayGetValueAtIndex(cookies, i);
					break;
				default:
					break;
			}
		}
		else if (nPage == kHIDPage_Simulation
				&& nUsage == kHIDUsage_Sim_Rudder)
			pThis->mRudder = (CFMutableDictionaryRef)
				CFArrayGetValueAtIndex(cookies, i);
	}
}
void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{
	/* Create the IOCFPlugInInterface */
	SInt32 nScore;
	IOCFPlugInInterface** ppPlugin;
	if(IOCreatePlugInInterfaceForService(pThis->mCurObject,
			kIOHIDDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &ppPlugin,
			&nScore) !=  kIOReturnSuccess)
	{
		WSJoystickLogError(("Could not create HID Interface for product"));
		memset(pJoystick, 0, sizeof(WSJOYSTICK));
		return;
	}

	/* Query the IOCFPlugInInterface for the IOHIDDeviceInterface */
	if((*(*ppPlugin)->QueryInterface)(ppPlugin,
			CFUUIDGetUUIDBytes(kIOHIDDeviceInterfaceID),
			(void**) &pJoystick->mppDevice) != S_OK)
	{
		WSJoystickLogError(("Could not get device interface "
			"from HID interface"));
		memset(pJoystick, 0, sizeof(WSJOYSTICK));
		return;
	}

	/* release the plugin */
	(*(*ppPlugin)->Release)(ppPlugin);

	/* open the HID interface */
	if ((*(*pJoystick->mppDevice)->open)(pJoystick->mppDevice, 0) != S_OK)
	{
		(*(*pJoystick->mppDevice)->Release)(pJoystick->mppDevice);
		WSJoystickLogError(("HID device: open failed"));
		memset(pJoystick, 0, sizeof(WSJOYSTICK));
		return;
	}

	if (IORegistryEntryCreateCFProperties(pThis->mCurObject,
			&pJoystick->mPropDict,
			kCFAllocatorDefault, kNilOptions) != KERN_SUCCESS)
	{
		(*(*pJoystick->mppDevice)->Release)(pJoystick->mppDevice);
		WSJoystickLogError(("IORegistryEntryCreateCFProperties failed"));
		memset(pJoystick, 0, sizeof(WSJOYSTICK));
		return;
	}
	pJoystick->mCookies = (CFMutableArrayRef)
		CFDictionaryGetValue(pJoystick->mPropDict,
								CFSTR(kIOHIDElementKey));
	assert(pJoystick->mCookies);
	WSJoystick_BuildCookies(pJoystick, pJoystick->mCookies);
}
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{
	WSHIDIterator_Destruct((WSHIDITERATOR*)pThis);
}

wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{
	return WSHIDDevice_IsCreated((WSHIDDEVICE*)pThis);
}
void WSJoystick_Destroy(WSJOYSTICK* pThis)
{
	WSHIDDevice_Destruct((WSHIDDEVICE*)pThis);
}

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{
	WSHIDIterator_Create((WSHIDITERATOR*)pIt, kHIDPage_GenericDesktop,
								   kHIDUsage_GD_Joystick);
	pIt->mDidGamepadsAlso = 0;
}
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{
	return pIt->mCurObject == 0;
}
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{
	state->joystick = pThis;
}
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{
	caps->joystick = pThis;
}

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{
	return WSHIDDevice_GetCookieIndexValue((WSHIDDEVICE*)pThis->joystick,
		pThis->joystick->mXAxis);
}
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{
	return WSHIDDevice_GetCookieIndexValue((WSHIDDEVICE*)pThis->joystick,
		pThis->joystick->mYAxis);
}
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{
	return WSHIDDevice_GetCookieIndexValue((WSHIDDEVICE*)pThis->joystick,
		pThis->joystick->mZAxis);
}
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{
	return WSHIDDevice_GetCookieIndexValue((WSHIDDEVICE*)pThis->joystick,
		pThis->joystick->mButtons[button]) != 0;
}
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{
	return WSHIDDevice_GetCookieIndexValue((WSHIDDEVICE*)pThis->joystick,
		pThis->joystick->mPOV);
}
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{
	return WSHIDDevice_GetCookieIndexValue((WSHIDDEVICE*)pThis->joystick,
		pThis->joystick->mRudder);
}
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{
	return WSHIDDevice_GetCookieIndexValue((WSHIDDEVICE*)pThis->joystick,
		pThis->joystick->mUAxis);
}
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{
	return WSHIDDevice_GetCookieIndexValue((WSHIDDEVICE*)pThis->joystick,
		pThis->joystick->mVAxis);
}

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{
	int mid;
	CFNumberRef cfnVendorId = (CFNumberRef)
		CFDictionaryGetValue(pThis->joystick->mPropDict,
			CFSTR(kIOHIDVendorIDKey));
	if (cfnVendorId
		&& CFNumberGetValue(cfnVendorId, kCFNumberIntType, &mid))
		return mid;
	else
		return 0;
}
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{
	int pid;
	CFNumberRef cfnProductId = (CFNumberRef)
		CFDictionaryGetValue(pThis->joystick->mPropDict,
							 CFSTR(kIOHIDProductIDKey));
	if (cfnProductId
		&& CFNumberGetValue(cfnProductId, kCFNumberIntType, &pid))
		return pid;
	else
		return 0;
}
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{
	CFStringRef cfsProduct = (CFStringRef)
		CFDictionaryGetValue(pThis->joystick->mPropDict,
							 CFSTR(kIOHIDProductKey));
	return CFStringGetCStringPtr(cfsProduct, kCFStringEncodingUTF8);
}
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mXAxis,
	CFSTR(kIOHIDElementMinKey)); }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mYAxis,
	CFSTR(kIOHIDElementMinKey)); }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mZAxis,
	CFSTR(kIOHIDElementMinKey)); }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mXAxis,
	CFSTR(kIOHIDElementMaxKey)); }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mYAxis,
	CFSTR(kIOHIDElementMaxKey)); }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mZAxis,
	CFSTR(kIOHIDElementMaxKey)); }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mPOV,
	CFSTR(kIOHIDElementMinKey)); }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mPOV,
	CFSTR(kIOHIDElementMaxKey)); }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return (WSJoystickCaps_GetPOVMax(pThis)
		+ WSJoystickCaps_GetPOVMin(pThis)) / 2; }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mNumButtons; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{
	int numaxes = 0;
	if(pThis->joystick->mXAxis)
		++numaxes;
	if(pThis->joystick->mYAxis)
		++numaxes;
	if(pThis->joystick->mZAxis)
		++numaxes;
	if(pThis->joystick->mUAxis)
		++numaxes;
	if(pThis->joystick->mVAxis)
		++numaxes;
	if(pThis->joystick->mRudder)
		++numaxes;
	if(pThis->joystick->mPOV)
		++numaxes;
	return numaxes;
}
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_BUTTONS; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_AXES; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mRudder,
	CFSTR(kIOHIDElementMinKey)); }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mRudder,
	CFSTR(kIOHIDElementMaxKey)); }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mUAxis,
	CFSTR(kIOHIDElementMinKey)); }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mUAxis,
	CFSTR(kIOHIDElementMaxKey)); }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mVAxis,
	CFSTR(kIOHIDElementMinKey)); }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return GetIntFromCFDictionary(pThis->joystick->mVAxis,
	CFSTR(kIOHIDElementMaxKey)); }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mRudder != NULL; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mZAxis != NULL; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mUAxis != NULL; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mVAxis != NULL; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return pThis->joystick->mPOV != NULL; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
					 #       ### #     # #     # #     #
					 #        #  ##    # #     #  #   #
					 #        #  # #   # #     #   # #
					 #        #  #  #  # #     #    #
					 #        #  #   # # #     #   # #
					 #        #  #    ## #     #  #   #
					 ####### ### #     #  #####  #     #

		https://www.kernel.org/doc/Documentation/input/joystick-api.txt
=============================================================================*/
#elif defined(__linux__)
#include <linux/joystick.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> /* memset */
#ifdef HAVE_SYS_SELECT_H
#   include <sys/select.h>
#endif
#include <sys/stat.h>	/* stat */
#define WSJOYSTICK_AXIS_X 0
#define WSJOYSTICK_AXIS_Y 1
#define WSJOYSTICK_AXIS_Z 2
#define WSJOYSTICK_AXIS_RUDDER 3
#define WSJOYSTICK_AXIS_U 4
#define WSJOYSTICK_AXIS_V 5
#define WSJOYSTICK_AXIS_MAX 32767
#define WSJOYSTICK_AXIS_MIN -32767
#define WSJOYSTICK_AXIS_CENTER 0
#define WSJOYSTICKTYPE_DEVINPUTJS	0 /* newer /dev/input structure */
#define WSJOYSTICKTYPE_DEVJS		1
#define WSJOYSTICKTYPE_NONE		2

#ifdef __cplusplus
	extern "C" {
#endif
#if !defined(WIKISERVER_VERSION)
	#define countof(x) (sizeof(x) / sizeof(*x))
	#define strcountof(x) (countof(x) - 1)
	#define CStrSwapChar(x, y) { char tx = x; x = y; y = tx; }
	#ifndef DigitToStringMaxChars
		#define DigitToStringMaxChars() (21)
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
	#endif
#endif
#if !defined(ABS_X)
	#define ABS_X			0x00
	#define ABS_Y			0x01
	#define ABS_Z			0x02
	#define ABS_RX			0x03
	#define ABS_RY			0x04
	#define ABS_RZ			0x05
	#define ABS_THROTTLE	0x06
	#define ABS_RUDDER		0x07
	#define ABS_WHEEL		0x08
	#define ABS_GAS			0x09
	#define ABS_BRAKE		0x0a
	#define ABS_HAT0X		0x10
	#define ABS_HAT0Y		0x11
	#define ABS_HAT1X		0x12
	#define ABS_HAT1Y		0x13
	#define ABS_HAT2X		0x14
	#define ABS_HAT2Y		0x15
	#define ABS_HAT3X		0x16
	#define ABS_HAT3Y		0x17
	#define ABS_PRESSURE	0x18
	#define ABS_DISTANCE	0x19
	#define ABS_TILT_X		0x1a
	#define ABS_TILT_Y		0x1b
	#define ABS_TOOL_WIDTH	0x1c
	#define ABS_VOLUME		0x20
	#define ABS_MISC		0x28
#endif /* !defined(ABS_X) */
wsbool WSJoystickIterator_Try(WSJOYSTICKITERATOR* pThis)
{
	char pathbuffer[strcountof("/dev/input/js")
		+ DigitToStringMaxChars() + 1];
	struct stat theStat;
	if(pThis->type == WSJOYSTICKTYPE_DEVINPUTJS)
	{
		memcpy(pathbuffer, "/dev/input/js",
			sizeof(char) * strcountof("/dev/input/js"));
		pathbuffer[strcountof("/dev/input/js")
			+ UDigitToString(&pathbuffer[strcountof("/dev/input/js")],
				pThis->index)] = '\0';
	}
	else
	{
		memcpy(pathbuffer, "/dev/js", sizeof(char) * strcountof("/dev/js"));
		pathbuffer[strcountof("/dev/js")
			+ UDigitToString(&pathbuffer[strcountof("/dev/js")],
				pThis->index)] = '\0';
	}
	return stat(pathbuffer, &theStat) == 0;
}

void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{
	++pThis->index;
	while(!WSJoystickIsEnd(pThis))
	{
		if(WSJoystickIterator_Try(pThis))
			break;

		pThis->index = 0;
		++pThis->type;
	};
}

void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{
	char pathbuffer[strcountof("/dev/input/js")
		+ DigitToStringMaxChars() + 1];
	memset(pJoystick, 0, sizeof(WSJOYSTICK));
	if(pThis->type == WSJOYSTICKTYPE_DEVINPUTJS)
	{
		memcpy(pathbuffer, "/dev/input/js",
			sizeof(char) * strcountof("/dev/input/js"));
		pathbuffer[strcountof("/dev/input/js")
			+ UDigitToString(&pathbuffer[strcountof("/dev/input/js")],
				pThis->index)] = '\0';
	}
	else
	{
		memcpy(pathbuffer, "/dev/js", sizeof(char) * strcountof("/dev/js"));
		pathbuffer[strcountof("/dev/js")
			+ UDigitToString(&pathbuffer[strcountof("/dev/js")],
				pThis->index)] = '\0';
	}
	pJoystick->mHandle = open(pathbuffer, O_RDONLY | O_NONBLOCK);
	if(pJoystick->mHandle != -1)
	{
		pJoystick->mVersion = 0x000800;
		/*
		*	Version is of the form (major.minor.micro):
		*	pJoystick->mVersion >> 16, (pJoystick->mVersion >> 8) & 0xFF,
		*	pJoystick->mVersion & 0xFF
		*/
		ioctl(pJoystick->mHandle, JSIOCGVERSION, &pJoystick->mVersion);
	}
	/*
	if(pJoystick->mHandle != -1)
		fcntl(pJoystick->mHandle, F_SETFL, O_NONBLOCK);
	*/
}
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{
}

wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{
	return pThis->mHandle != -1;
}
void WSJoystick_Destroy(WSJOYSTICK* pThis)
{
	close(pThis->mHandle);
}

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{
	pIt->index = 0;
	pIt->type = WSJOYSTICKTYPE_DEVINPUTJS;
	if(!WSJoystickIterator_Try(pIt))
		WSJoystickIterator_Iterate(pIt);
}
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{
	return pIt->type == WSJOYSTICKTYPE_NONE;
}
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{
	int numread;
	fd_set readfds;
	struct timeval timeout;
	state->joystick = pThis;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(pThis->mHandle, &readfds);
	select(pThis->mHandle+1, &readfds, NULL, NULL, &timeout);
	if (!FD_ISSET(pThis->mHandle, &readfds))
		return;
	if(pThis->mVersion >= 0x010000)
	{
		struct js_event events[32];
		while ((numread = read(pThis->mHandle, events, sizeof(events))) > 0)
		{
			int i;
			numread /= sizeof(events[0]);
			for (i = 0; i < numread; ++i)
			{
				if (events[i].type & JS_EVENT_AXIS)
				{
					state->joystick->axes[events[i].number] = events[i].value;
					/*printf("AXIS EVENT[%d]=%d\n",
						events[i].number, events[i].value);*/
					if(events[i].number == ABS_THROTTLE)
						state->joystick->axes[ABS_Z] = events[i].value;
					else if(events[i].number == ABS_RUDDER)
						state->joystick->axes[ABS_RZ] = events[i].value;
					else if(events[i].number == ABS_HAT0Y)
						state->joystick->axes[ABS_HAT0X] = events[i].value;
				}
				if (events[i].type & JS_EVENT_BUTTON)
				{
					/*printf("BUTTON EVENT[%d]=%d\n",
						events[i].number, events[i].value);*/
					if (events[i].value)
						state->joystick->buttonsstate |=
							(1 << events[i].number);
					else
						state->joystick->buttonsstate &=
							~(1 << events[i].number);
				}
			}
		}
	}
	else
	{
		struct JS_DATA_TYPE events;
		if(read(pThis->mHandle, &events, JS_RETURN) == JS_RETURN)
		{
			state->joystick->buttonsstate = events.buttons;
			state->joystick->axes[0] = events.x;
			state->joystick->axes[1] = events.y;
		}
	}
}
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{
	if(pThis->mVersion >= 0x010000)
	{
		if (ioctl(pThis->mHandle, JSIOCGNAME(sizeof(caps->productname)),
				caps->productname) < 0)
			memcpy(caps->productname, "Unknown",
				sizeof(char) * strcountof("Unknown"));
		if(ioctl(pThis->mHandle, JSIOCGBUTTONS, &caps->numbuttons) < 0)
			caps->numbuttons = 0;
		if(ioctl(pThis->mHandle, JSIOCGAXES, &caps->numaxes) < 0)
			caps->numaxes = 0;
	}
	else
	{
		caps->productname[0] = '\0';
		caps->numbuttons = caps->numaxes = 2;
	}
}

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[ABS_X]; }
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[ABS_Y]; }
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[ABS_Z]; }
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{ return pThis->joystick->buttonsstate & (1 << button); }
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[ABS_HAT0X]; }
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[ABS_RZ]; }
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[ABS_RX]; }
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[ABS_RY]; }

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{ return pThis->productname; }
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_CENTER; }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return pThis->numbuttons; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{ return pThis->numaxes; }
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return 31; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return 15; }
int WSJoystickCaps_GetPollingMin(const WSJOYSTICKCAPS* pThis)
{ return 10; }
int WSJoystickCaps_GetPollingMax(const WSJOYSTICKCAPS* pThis)
{ return 1000; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return pThis->numaxes >= ABS_RZ; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return pThis->numaxes >= ABS_Z; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return pThis->numaxes >= ABS_RX; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return pThis->numaxes >= ABS_RY; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return pThis->numaxes >= ABS_HAT0X; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
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

	A.K.A. _iocs_joyget(joynum)
	_iocs_bitsns is for the keyboard (0x09, 0x06, 0x00 == enter/space/cancel)
	Refs:
	https://github.com/FedericoTech/X68KTutorials/
	https://github.com/pirota-pirozou/SHOT68K/
	https://nfggames.com/forum2/index.php?topic=5317.0
=============================================================================*/
#elif defined(__human68k__)
#include <iocs.h>
#define JOY_BUTTON1	(1 << 6)
#define JOY_BUTTON2	(1 << 5)
#define JOY_RIGHT	(1 << 3)
#define JOY_LEFT	(1 << 2)
#define JOY_DOWN	(1 << 1)
#define JOY_UP		(1 << 0)
#define WSJOYSTICK_AXIS_MAX 1
#define WSJOYSTICK_AXIS_MIN -1
#define WSJOYSTICK_AXIS_CENTER 0
#define WSJOYSTICK_MAX_BUTTONS 2
#define WSJOYSTICK_MAX_AXES 2
#ifdef __cplusplus
	extern "C" {
#endif
#if !defined(WIKISERVER_VERSION)
	#define countof(x) (sizeof(x) / sizeof(*x))
	#define strcountof(x) (countof(x) - 1)
	#define CStrSwapChar(x, y) { char tx = x; x = y; y = tx; }
	#ifndef DigitToStringMaxChars
		#define DigitToStringMaxChars() (21)
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
	#endif
#endif
void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{ ++pThis->index; }
void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{ pJoystick->mHandle = pThis->index; }
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{}

wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{ return pThis->mHandle < 2; }
void WSJoystick_Destroy(WSJOYSTICK* pThis)
{}
void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{ pIt->index = 0; }
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{ return pIt->index > 1; /* only 2 joysticks supported by X68000 */ }
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{ state->state = _iocs_joyget(pThis->mHandle /* JOY_PORT1/JOY_PORT2 */);}
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{ }

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{
	int res;
	/*
		If both JOY_LEFT and JOY_RIGHT
		are set, that means that neither may be set and the SELECT
		button could be pressed (or MENU on a Xbox360-compatable controller)
	if((pThis->state & JOY_LEFT) && (pThis->state & JOY_RIGHT))
		res = WSJOYSTICK_AXIS_CENTER;
	else */if(pThis->state & JOY_LEFT)
		 res = WSJOYSTICK_AXIS_MIN;
	else if(pThis->state & JOY_RIGHT)
		 res = WSJOYSTICK_AXIS_MAX;
	else
		 res = WSJOYSTICK_AXIS_CENTER;
	return res;
}
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{
	int res;
	/*
		If both JOY_UP and JOY_DOWN
		are set, that means that neither may be set and the START
		button could be pressed, presumably on a
		SEGA Mega Drive Mini controller
	if((pThis->state & JOY_UP) && (pThis->state & JOY_DOWN))
		res = WSJOYSTICK_AXIS_CENTER;
	else */ if(pThis->state & JOY_UP)
		 res = WSJOYSTICK_AXIS_MIN;
	else if(pThis->state & JOY_DOWN)
		 res = WSJOYSTICK_AXIS_MAX;
	else
		 res = WSJOYSTICK_AXIS_CENTER;

	return res;
}
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{ /* Caveat: On a Xbox360 controller pressing X will set both buttons */
  return pThis->state & (1 << (5 + button)); }
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{ return 0]; }
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{ return ""; }
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_CENTER; }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_BUTTONS; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_AXES; }
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_BUTTONS; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_AXES; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
/*=============================================================================
						 ######   #####  ######
						 #     # #     # #     #
						 #     # #       #     #
						 ######   #####  #     #
						 #     #       # #     #
						 #     # #     # #     #
						 ######   #####  ######

	https://man.freebsd.org/cgi/man.cgi?
	query=joy&sektion=4&apropos=0&manpath=FreeBSD+12.0-RELEASE+and+Ports
	https://man.freebsd.org/cgi/man.cgi?
	query=uhid&sektion=4&apropos=0&manpath=FreeBSD+12.0-RELEASE+and+Ports
	(uhid(4)/joy(4))
	- joy is the gameport api at /dev/joy#
	-- The api is a simple 2 button 2 axis API based around read()
	- uhid is at /dev/uhid# or in the case of newer OpenBSD /dev/ujoy/#
	-- Revolves around keeping a uhid id and buffer
=============================================================================*/
#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__)
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h> /* memset */
#include <stdlib.h> /* malloc/free */
#include <sys/stat.h> /* stat */
#ifndef __FreeBSD_kernel_version
#define __FreeBSD_kernel_version __FreeBSD_version
#endif
#if defined(HAVE_USB_H)
	#include <usb.h>
#endif
#ifdef __DragonFly__
	#include <bus/u4b/usb.h>
	#include <bus/u4b/usbhid.h>
#else
	#include <dev/usb/usb.h>
	#include <dev/usb/usbhid.h>
#endif
#if defined(HAVE_LIBUSB_H)
	#include <libusb.h>
#elif defined(HAVE_LIBUSBHID_H)
	#include <libusbhid.h>
#else /*defined(HAVE_USBHID_H)*/
	#include <usbhid.h>
#endif
#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	#if __FreeBSD_kernel_version > 800063
		#include <dev/usb/usb_ioctl.h>
	#elif defined(__DragonFly__)
		#include <bus/u4b/usb_ioctl.h>
		/*#include <osreldate.h>*/
	#endif
	#include <sys/joystick.h>
#endif
#if defined(HAVE_MACHINE_JOYSTICK_H)
	#include <machine/joystick.h>
#endif

#define WSJOYSTICK_MAX_UHID 64
#define WSJOYSTICK_MAX_GAMEPAD  32 /* SDL uses 2 IIRC */
int WSJoystickIsGamepad_(int index)
{
	return index >= WSJOYSTICK_MAX_UHID;
}
#if defined(__FreeBSD__) && (__FreeBSD_kernel_version > 900000) || \
	defined(__DragonFly__)
	#define WSJoystick_UHIDDescs(pThis) (pThis)->u.uhid.descs
#elif defined(__FreeBSD__) && (__FreeBSD_kernel_version > 800063)
	#define WSJoystick_UHIDDescs(pThis) \
		(*((struct usb_gen_descriptor**)&(pThis)->u.uhid.descs))
#else
	#define WSJoystick_UHIDDescs(pThis) \
		(*((struct usb_ctl_report**)&(pThis)->u.uhid.descs))
#endif

#ifdef __OpenBSD__
#define HUG_DPAD_UP    0x90
#define HUG_DPAD_DOWN  0x91
#define HUG_DPAD_RIGHT 0x92
#define HUG_DPAD_LEFT  0x93
#define HAT_UP        0x01
#define HAT_RIGHT     0x02
#define HAT_DOWN      0x04
#define HAT_LEFT      0x08
#endif /*__OpenBSD__*/

#if 0
	#define WSJOYSTICK_AXIS_MAX 32767
	#define WSJOYSTICK_AXIS_MIN -32767
	#define WSJOYSTICK_AXIS_CENTER 0
#else /* the API is from 0-255 without calibration */
	#define WSJOYSTICK_AXIS_MAX 255
	#define WSJOYSTICK_AXIS_MIN 0
	#define WSJOYSTICK_AXIS_CENTER 127
#endif
#if defined(__OpenBSD__) && (OpenBSD >= 202105)
	#define WSJOYSTICK_UHIDPATH "/dev/ujoy/"
#else
	#define WSJOYSTICK_UHIDPATH "/dev/uhid"
#endif
#define WSJOYSTICK_MAX_BUTTONS 32

#if defined(USBHID_UCR_DATA) || (defined(__FreeBSD_kernel__) \
	&& __FreeBSD_kernel_version <= 800063)
	#define WSJOYSTICK_DESC_DATA(pThis) \
		(WSJoystick_UHIDDescs(pThis)->ucr_data)
#elif (defined(__FreeBSD__) && (__FreeBSD_kernel_version > 900000)) || \
	defined(__DragonFly__)
#define WSJOYSTICK_DESC_DATA(pThis) WSJoystick_UHIDDescs(pThis)
#elif (defined(__FreeBSD__) && (__FreeBSD_kernel_version > 800063))
#define WSJOYSTICK_DESC_DATA(pThis) \
	(WSJoystick_UHIDDescs(pThis)->ugd_data)
#else
#define WSJOYSTICK_DESC_DATA(pThis) \
	(WSJoystick_UHIDDescs(pThis)->data)
#endif

#ifdef __cplusplus
	extern "C" {
#endif
#if !defined(WIKISERVER_VERSION)
	#define countof(x) (sizeof(x) / sizeof(*x))
	#define strcountof(x) (countof(x) - 1)
	#define CStrSwapChar(x, y) { char tx = x; x = y; y = tx; }
	#ifndef DigitToStringMaxChars
		#define DigitToStringMaxChars() (21)
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
	#endif
#endif
wsbool WSJoystickIterator_Try(const WSJOYSTICKITERATOR* pThis)
{
	char pathbuffer[strcountof(WSJOYSTICK_UHIDPATH)
		+ DigitToStringMaxChars() + 1];
	struct stat theStat;
#if defined(HAVE_GAMEPORT)
	if(!WSJoystickIsGamepad_(pThis->index))
	{
#endif
		memcpy(pathbuffer, WSJOYSTICK_UHIDPATH,
			sizeof(char) * strcountof(WSJOYSTICK_UHIDPATH));
		pathbuffer[strcountof(WSJOYSTICK_UHIDPATH)
			+ UDigitToString(&pathbuffer[strcountof(WSJOYSTICK_UHIDPATH)],
				pThis->index)] = '\0';
#if defined(HAVE_GAMEPORT)
	}
	else
	{
		memcpy(pathbuffer, "/dev/joy",
			sizeof(char) * strcountof("/dev/joy"));
		pathbuffer[strcountof("/dev/joy")
			+ UDigitToString(&pathbuffer[strcountof("/dev/joy")],
				pThis->index)] = '\0';
	}
#endif
	return stat(pathbuffer, &theStat) == 0;
}

void WSJoystickIterator_Validate(WSJOYSTICKITERATOR* pThis)
{ /* makes sure the path exists with stat(), and if not tries gamepads */
	wsbool success;
	success = WSJoystickIterator_Try(pThis);
	#if defined(HAVE_GAMEPORT)
		if(!success && !WSJoystickIsGamepad_(pThis->index))
		{
			/* make it a gamepad */
			pThis->index = WSJOYSTICK_MAX_UHID;
			success = WSJoystickIterator_Try(pThis);
		}
	#endif /* HAVE_GAMEPORT */
	if(!success)
		pThis->index = -1;
}
void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{
	if(pThis->index != -1)
	{
		++pThis->index;
		WSJoystickIterator_Validate(pThis);
	}
}

void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{

	char pathbuffer[strcountof(WSJOYSTICK_UHIDPATH)
		+ DigitToStringMaxChars() + 1];
	memset(pJoystick, 0, sizeof(WSJOYSTICK));
	pJoystick->index = pThis->index;
#if defined(HAVE_GAMEPORT)
	if(!WSJoystickIsGamepad_(pJoystick->index))
	{
#endif
		memcpy(pathbuffer, WSJOYSTICK_UHIDPATH,
			sizeof(char) * strcountof(WSJOYSTICK_UHIDPATH));
		pathbuffer[strcountof(WSJOYSTICK_UHIDPATH)
			+ UDigitToString(&pathbuffer[strcountof(WSJOYSTICK_UHIDPATH)],
				pThis->index)] = '\0';
		pJoystick->mHandle = open(pathbuffer, O_RDONLY | O_CLOEXEC);
		if(pJoystick->mHandle != -1)
		{
			int uhidrepsizelen;
			pJoystick->u.uhid.uhidrepdesc = hid_get_report_desc(
				pJoystick->mHandle);
			if(!pJoystick->u.uhid.uhidrepdesc)
				return;
			#if defined(__FreeBSD__) \
				&& (__FreeBSD_kernel_version > 800063) \
				|| defined(__FreeBSD_kernel__) || defined(__DragonFly__)
				pJoystick->u.uhid.uhidrepid = hid_get_report_id(
					pJoystick->mHandle);
				if (pJoystick->u.uhid.uhidrepid < 0)
			#else
				if (ioctl(pJoystick->mHandle, USB_GET_REPORT_ID,
							&pJoystick->u.uhid.uhidrepid) < 0)
			#endif
					pJoystick->u.uhid.uhidrepid = -1;

			#ifdef __DragonFly__
				uhidrepsizelen = hid_report_size(pJoystick->u.uhid.uhidrepdesc,
					hid_input, pJoystick->u.uhid.uhidrepid);
			#elif defined(__FreeBSD__)
			#if (__FreeBSD_kernel_version >= 460000) \
					|| defined(__FreeBSD_kernel__)
				#if (__FreeBSD_kernel_version <= 500111)
					uhidrepsizelen = hid_report_size(
						pJoystick->u.uhid.uhidrepdesc,
						pJoystick->u.uhid.uhidrepid, hid_input);
				#else
					uhidrepsizelen = hid_report_size(
						pJoystick->u.uhid.uhidrepdesc,
						hid_input, pJoystick->u.uhid.uhidrepid);
				#endif
			#else
				uhidrepsizelen = hid_report_size(pJoystick->u.uhid.uhidrepdesc,
					hid_input, &pJoystick->u.uhid.uhidrepid);
			#endif
			#else
				#ifdef USBHID_NEW
					uhidrepsizelen = hid_report_size(
						pJoystick->u.uhid.uhidrepdesc,
						hid_input, pJoystick->u.uhid.uhidrepid);
				#else
					uhidrepsizelen = hid_report_size(
						pJoystick->u.uhid.uhidrepdesc,
						hid_input, &pJoystick->u.uhid.uhidrepid);
				#endif
			#endif

			if (uhidrepsizelen > 0)
			{
			#if defined(__FreeBSD__) \
					&& (__FreeBSD_kernel_version > 900000) \
					|| defined(__DragonFly__)
					pJoystick->u.uhid.descs = malloc(uhidrepsizelen);
			#else
					pJoystick->u.uhid.descs = malloc(
						sizeof(*WSJoystick_UHIDDescs(pJoystick))
						- sizeof(WSJOYSTICK_DESC_DATA(pJoystick))
						+ uhidrepsizelen);
			#endif
				pJoystick->u.uhid.descssize = uhidrepsizelen;
				#ifdef __NetBSD__
					while (read(pJoystick->mHandle,
						WSJOYSTICK_DESC_DATA(pJoystick),
						pJoystick->u.uhid.descssize) == pJoystick->u.uhid.descssize)
						;
				#endif
			}
			if (uhidrepsizelen <= 0 || !pJoystick->u.uhid.descs)
			{
				close(pJoystick->mHandle);
#if defined(HAVE_GAMEPORT)
				pJoystick->index = WSJOYSTICK_MAX_UHID; /* make it a gamepad */
#else
				pJoystick->mHandle = -1;
#endif
			}
		} /* if open succeeded */
#if defined(HAVE_GAMEPORT)
	}
	else
	{ /* older /dev/joy structure */
		pJoystick->u.gameport.x = 0;
		pJoystick->u.gameport.y = 0;
		pJoystick->u.gameport.xmin = 0xFFFF;
		pJoystick->u.gameport.ymin = 0xFFFF;
		pJoystick->u.gameport.xmax = 0;
		pJoystick->u.gameport.ymax = 0;
		memcpy(pathbuffer, "/dev/joy",
			sizeof(char) * strcountof("/dev/joy"));
		pathbuffer[strcountof("/dev/joy")
			+ UDigitToString(&pathbuffer[strcountof("/dev/joy")],
				pThis->index)] = '\0';
		pJoystick->mHandle = open(pathbuffer, O_RDONLY | O_CLOEXEC);
	}
#endif

	if(pJoystick->mHandle != -1)
		fcntl(pJoystick->mHandle, F_SETFL, O_NONBLOCK);
}
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{
}

wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{
	return pThis->mHandle != -1;
}
void WSJoystick_Destroy(WSJOYSTICK* pThis)
{
	close(pThis->mHandle);
#if defined(HAVE_GAMEPORT)
	if(!WSJoystickIsGamepad_(pThis->index))
#endif
		free(pThis->u.uhid.descs);
}

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{
	pIt->index = 0;
	WSJoystickIterator_Validate(pIt);
}
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{
	return pIt->index == -1;
}
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{
	struct hid_item uhiditem;
	struct hid_data *uhiddata;
	int nbutton;
	int32_t v;

#if defined(HAVE_GAMEPORT)
	if (WSJoystickIsGamepad_(pThis->index))
	{
		struct joystick events[4];
		int numread;
		int i;
		while ((numread = read(pThis->mHandle, events, sizeof(events)))
				>= sizeof(events))
		{
			numread /= sizeof(events[0]);
			for(i = 0; i < numread; ++i)
			{
				/*
					https://man.openbsd.org/joy.4
					axe values are 0-255 but due to the
					RC curcuit hardware hack, values may be above
					or below that so we have to calibrate the values
				*/
				if (abs(pThis->u.gameport.x - events[i].x)
						> 8)
				{
					pThis->u.gameport.x = events[i].x;
					if (pThis->u.gameport.x < pThis->u.gameport.xmin)
						pThis->u.gameport.xmin = pThis->u.gameport.x;
					if (pThis->u.gameport.x > pThis->u.gameport.xmax)
						pThis->u.gameport.xmax = pThis->u.gameport.x;
					if (pThis->u.gameport.xmin == pThis->u.gameport.xmax)
					{
						pThis->u.gameport.xmin--;
						pThis->u.gameport.xmax++;
					}
					v = (((WSJOYSTICK_AXIS_MAX - WSJOYSTICK_AXIS_MIN)
								* ((int32_t)pThis->u.gameport.x
									- pThis->u.gameport.xmin))
							/ (pThis->u.gameport.xmax
								- pThis->u.gameport.xmin))
						+ WSJOYSTICK_AXIS_MIN;
					pThis->axes[0] = v;
				}
				if (abs(pThis->u.gameport.y - events[i].y)
						> 8)
				{
					pThis->u.gameport.y = events[i].y;
					if (pThis->u.gameport.y < pThis->u.gameport.ymin)
						pThis->u.gameport.ymin = pThis->u.gameport.y;
					if (pThis->u.gameport.y > pThis->u.gameport.ymax)
						pThis->u.gameport.ymax = pThis->u.gameport.y;
					if (pThis->u.gameport.ymin == pThis->u.gameport.ymax)
					{
						pThis->u.gameport.ymin--;
						pThis->u.gameport.ymax++;
					}
					v = (((WSJOYSTICK_AXIS_MAX - WSJOYSTICK_AXIS_MIN)
								* ((int32_t)pThis->u.gameport.y
									- pThis->u.gameport.ymin))
							/ (pThis->u.gameport.ymax
								- pThis->u.gameport.ymin))
						+ WSJOYSTICK_AXIS_MIN;
					pThis->axes[1] = v;
				}
				WSJoystick_HandleButton(pThis, 0, events[i].b1);
				WSJoystick_HandleButton(pThis, 1, events[i].b2);
			} /* end for -> num events read */
		}
		return; /* it's the old gameport and we are done */
	}
#endif

	while (read(pThis->mHandle, WSJOYSTICK_DESC_DATA(pThis),
				pThis->u.uhid.descssize) == pThis->u.uhid.descssize)
	{
		#if defined(USBHID_NEW) \
			|| (defined(__FreeBSD__) && __FreeBSD_kernel_version >= 500111) \
			|| defined(__FreeBSD_kernel__) || defined(__DragonFly__)
			uhiddata = hid_start_parse(pThis->u.uhid.uhidrepdesc,
				1 << hid_input, pThis->u.uhid.uhidrepid);
		#else
			uhiddata = hid_start_parse(pThis->u.uhid.uhidrepdesc,
						1 << hid_input);
		#endif
		if (!uhiddata)
			continue;

		while (hid_get_item(uhiddata, &uhiditem) > 0)
		{
			switch (uhiditem.kind)
			{
			case hid_input:
				switch (HID_PAGE(uhiditem.usage))
				{
				case HUP_GENERIC_DESKTOP:
				{
					int usage = HID_USAGE(uhiditem.usage);
					if(usage - HUG_X >= WSJOYSTICK_MAX_AXES)
						break;
					v = (int32_t)hid_get_data(WSJOYSTICK_DESC_DATA(pThis),
						&uhiditem);
					v = (((WSJOYSTICK_AXIS_MAX - WSJOYSTICK_AXIS_MIN)
						* (v - uhiditem.logical_minimum)) /
							(uhiditem.logical_maximum
								- uhiditem.logical_minimum))
						+ WSJOYSTICK_AXIS_MIN;
#ifdef __OpenBSD__
					if (usage == HUG_DPAD_UP)
						v = (int32_t)hid_get_data(
							WSJOYSTICK_DESC_DATA(pThis), &uhiditem);
					else if (usage == HUG_DPAD_DOWN)
						v = (int32_t)hid_get_data(
							WSJOYSTICK_DESC_DATA(pThis), &uhiditem);
					else if (usage == HUG_DPAD_RIGHT)
						v = (int32_t)hid_get_data(
							WSJOYSTICK_DESC_DATA(pThis), &uhiditem);
				   else if (usage == HUG_DPAD_LEFT)
						v = (int32_t)hid_get_data(
							WSJOYSTICK_DESC_DATA(pThis), &uhiditem);
#endif
					pThis->axes[usage - HUG_X] = v;
					break;
				}
				case HUP_BUTTON:
					v = (int32_t)hid_get_data(WSJOYSTICK_DESC_DATA(pThis),
						&uhiditem);
					/* HID_USAGE is 1 based */
					nbutton = HID_USAGE(uhiditem.usage) - 1;
					WSJoystick_HandleButton(pThis, v, nbutton);
					break;
				default:
					continue;
				}
				break;
			default:
				break;
			} /* end switch HID_PAGE */
		} /* end switch item kind */
		hid_end_parse(uhiddata);
	} /* end read loop */
}
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{
	struct usb_device_info uhidinfo;
#if defined(HAVE_GAMEPORT)
	if (WSJoystickIsGamepad_(pThis->index))
	{
		memcpy(caps->productname, "/dev/joy",
			sizeof(char) * strcountof("/dev/joy"));
		caps->productname[strcountof("/dev/joy")
			+ UDigitToString(&caps->productname[strcountof("/dev/joy")],
					pThis->index)] = '\0';
		caps->numaxes = 2;
		caps->numbuttons = 2;
		caps->productid = 0;
		caps->vendorid = 0;
	}
	else
#endif
	{
		struct hid_data* uhiddata;
		struct hid_item uhiditem;
		if(ioctl(pThis->mHandle, USB_GET_DEVICEINFO, &uhidinfo) != -1)
		{
			/*usb_device_info:
				uint16_t udi_productNo;
				uint16_t udi_vendorNo;
				uint16_t udi_releaseNo;
				uint16_t udi_power; mA power consumption, 0 if selfpowered
				uint8_t	udi_bus;
				uint8_t	udi_addr;
				uint8_t	udi_index;
				uint8_t	udi_class;
				uint8_t	udi_subclass;
				uint8_t	udi_protocol;
				uint8_t	udi_config_no;
				uint8_t	udi_config_index;
				uint8_t	udi_speed; USB_SPEED_XXX
				uint8_t	udi_mode; USB_MODE_XXX
				uint8_t	udi_nports;
				uint8_t	udi_hubaddr; parent
				uint8_t	udi_hubindex; parent
				uint8_t	udi_hubport; parent
				uint8_t	udi_power_mode; USB_POWER_MODE_XXX
				uint8_t	udi_suspended; set if device is suspended
				uint8_t	udi_reserved[16];
				char	udi_product[128];
				char	udi_vendor[128];
				char	udi_serial[64];
				char	udi_release[8];
			*/
			size_t productlen = strlen(uhidinfo.udi_product);
			if(productlen > 127)
				productlen = 127;
			memcpy(caps->productname, uhidinfo.udi_product, productlen);
			caps->productname[productlen] = '\0';
			caps->productid = uhidinfo.udi_productNo;
			caps->vendorid = uhidinfo.udi_vendorNo;
		}
		else
		{
			memcpy(caps->productname, WSJOYSTICK_UHIDPATH,
				sizeof(char) * strcountof(WSJOYSTICK_UHIDPATH));
			caps->productname[strcountof(WSJOYSTICK_UHIDPATH)
				+ UDigitToString(&caps->productname[
					strcountof(WSJOYSTICK_UHIDPATH)], pThis->index)] = '\0';
			caps->productid = 0;
			caps->vendorid = 0;
		}

#if defined(USBHID_NEW) \
	|| (defined(__FreeBSD__) && __FreeBSD_kernel_version >= 500111) \
	|| defined(__FreeBSD_kernel__) || defined(__DragonFly__)
		uhiddata = hid_start_parse(pThis->u.uhid.uhidrepdesc,
			1 << hid_input, pThis->u.uhid.uhidrepid);
#else
		uhiddata = hid_start_parse(pThis->u.uhid.uhidrepdesc, 1 << hid_input);
#endif
		if(!uhiddata)
			return;
		memset(caps->axes, 0, sizeof(caps->axes));
		caps->numbuttons = 0;
		caps->numaxes = 0;
		while (hid_get_item(uhiddata, &uhiditem) > 0)
		{
			switch (uhiditem.kind)
			{
			case hid_input:
				switch (HID_PAGE(uhiditem.usage))
				{
				case HUP_GENERIC_DESKTOP:
				{
					int usage = HID_USAGE(uhiditem.usage);
					int axe = usage - HUG_X;
					if(usage - HUG_X >= WSJOYSTICK_MAX_AXES)
						break;
					caps->axes[axe] = 1;
					++caps->numaxes;
					break;
				}
				case HUP_BUTTON:
				{
					int usage = HID_USAGE(uhiditem.usage);
					if (usage > caps->numbuttons)
						caps->numbuttons = usage;
					break;
				}
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
		hid_end_parse(uhiddata);
	}
}

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[HUG_X - HUG_X]; }
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[HUG_Y - HUG_X]; }
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[HUG_Z - HUG_X]; }
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{ return pThis->joystick->buttonsstate & (1 << button); }
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[HUG_HAT_SWITCH - HUG_X]; }
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[HUG_RZ - HUG_X]; }
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[HUG_RX - HUG_X]; }
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{ return pThis->joystick->axes[HUG_RY - HUG_X]; }

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{ return pThis->vendorid; }
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{ return pThis->productid; }
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{ return pThis->productname; }
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_CENTER; }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return pThis->numbuttons; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{ return pThis->numaxes; }
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_BUTTONS; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_MAX_AXES; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MIN; }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return WSJOYSTICK_AXIS_MAX; }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return pThis->axes[HUG_RZ - HUG_X] != 0; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return pThis->axes[HUG_Z - HUG_X] != 0; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return pThis->axes[HUG_RX - HUG_X] != 0; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return pThis->axes[HUG_RY - HUG_X] != 0; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return pThis->axes[HUG_HAT_SWITCH - HUG_X] != 0; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return TRUE; }
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
void WSJoystickIterator_Iterate(WSJOYSTICKITERATOR* pThis)
{}
void WSJoystickIterator_Value(const WSJOYSTICKITERATOR* pThis,
	WSJOYSTICK* pJoystick)
{}
void WSJoystickIterator_Destroy(WSJOYSTICKITERATOR* pThis)
{}

void WSJoystickBegin(WSJOYSTICKITERATOR* pIt)
{}
wsbool WSJoystickIsEnd(WSJOYSTICKITERATOR* pIt)
{ return TRUE; }
void WSJoystick_PollState(WSJOYSTICK* pThis, WSJOYSTICKSTATE* state)
{}
void WSJoystick_GetCaps(const WSJOYSTICK* pThis,
	WSJOYSTICKCAPS* caps)
{}
wsbool WSJoystick_IsCreated(const WSJOYSTICK* pThis)
{ return FALSE; }
void WSJoystick_Destroy(WSJOYSTICK* pThis)
{}

int WSJoystickState_GetXPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetYPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetZPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
wsbool WSJoystickState_GetButtonState(const WSJOYSTICKSTATE* pThis,
	int button)
{ return FALSE; }
int WSJoystickState_GetPOVPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetRudderPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetUPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }
int WSJoystickState_GetVPosition(const WSJOYSTICKSTATE* pThis)
{ return 0; }

int WSJoystickCaps_GetManufacturerId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetProductId(const WSJOYSTICKCAPS* pThis)
{ return 0; }
const char* WSJoystickCaps_GetProductName(WSJOYSTICKCAPS* pThis,
	const WSJOYSTICK* joystick)
{ return ""; }
int WSJoystickCaps_GetXMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetYMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetZMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetXMax(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetYMax(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetZMax(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetPOVMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetPOVMax(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetPOVCenter(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetNumberButtons(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetNumberAxes(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetMaxButtons(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetMaxAxes(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetRudderMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetRudderMax(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetUMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetUMax(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetVMin(const WSJOYSTICKCAPS* pThis)
{ return 0; }
int WSJoystickCaps_GetVMax(const WSJOYSTICKCAPS* pThis)
{ return 0; }
wsbool WSJoystickCaps_HasRudder(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasZ(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasU(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasV(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOV(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOV4Dir(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
wsbool WSJoystickCaps_HasPOVCTS(const WSJOYSTICKCAPS* pThis)
{ return FALSE; }
#ifdef __cplusplus
	} /* end extern "C" */
#endif
#endif /* __linux__/_WIN32 */
#endif /* WSJOYSTICK_IMPLEMENTATION */
