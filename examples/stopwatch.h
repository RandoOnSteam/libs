#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
/*  WSStopWatch - Time a portion of code */
typedef struct WSSTOPWATCH
{
	size_t liSnapshot[2], liHead[2], liFreq[2];
} WSSTOPWATCH;
void WSStopWatch_Construct(WSSTOPWATCH* pThis);
void WSStopWatch_Destruct(WSSTOPWATCH* pThis);
void WSStopWatch_Begin(WSSTOPWATCH* pThis);
void WSStopWatch_End(WSSTOPWATCH* pThis);
size_t WSStopWatch_Value(WSSTOPWATCH* pThis);
/*-----------------------------------------------------------------------------


	WIN32 WSSTOPWATCH


-----------------------------------------------------------------------------*/
#if defined(_WIN32)
#include <Windows.h>
/*=============================================================================*/
/* WSSTOPWATCH */
/**/
/* All _WIN32 compilers, even as early as MSVC5, have 64-bit support */
/* Division here doesn't pull in C Std Lib for whatever reason on MWerks. */
/*=============================================================================*/

void WSStopWatch_Construct(WSSTOPWATCH* pThis)
{
	LARGE_INTEGER li1, li2, liFreq;
	QueryPerformanceFrequency(&liFreq);
	QueryPerformanceCounter(&li1);
	QueryPerformanceCounter(&li2);
	(*((LONGLONG*)pThis->liHead)) = li2.QuadPart - li1.QuadPart;
	(*((LONGLONG*)pThis->liFreq)) = liFreq.QuadPart;
	(*((LONGLONG*)pThis->liSnapshot)) = li2.QuadPart;
}
void WSStopWatch_Destruct(WSSTOPWATCH* pThis)
{
	(void)(pThis);
}

void WSStopWatch_Begin(WSSTOPWATCH* pThis)
{
	LARGE_INTEGER li1;
	QueryPerformanceCounter(&li1);
	(*((LONGLONG*)pThis->liSnapshot)) = li1.QuadPart;
}

void WSStopWatch_End(WSSTOPWATCH* pThis)
{

}

size_t WSStopWatch_Value(WSSTOPWATCH* pThis)
{
	LARGE_INTEGER li2;
	size_t liCur[2];
	QueryPerformanceCounter(&li2);
	(*((LONGLONG*)liCur)) =
		li2.QuadPart - (*((LONGLONG*)pThis->liSnapshot));
	return (size_t) ((
		(*((LONGLONG*)liCur))
			- (*((LONGLONG*)pThis->liHead))) /
		  (
			  (*((LONGLONG*)pThis->liFreq))/1000
		  )
		);
}
/*-----------------------------------------------------------------------------


	MACINTOSH WSSTOPWATCH


-----------------------------------------------------------------------------*/
#elif defined(macintosh)
#ifdef __MACH__
	#include <Carbon/Carbon.h>
#else
	#include <Limits.h>
	#include <Types.h>
	#include <Resources.h>
	#include <QuickDraw.h>
	#include <Fonts.h>
	#include <Events.h>
	#include <Windows.h>
	#include <Menus.h>
	#include <TextEdit.h>
	#include <Dialogs.h>
	#include <Menus.h>
	#include <Devices.h>
	#include <ToolUtils.h>
	#include <Memory.h>
	#include <Processes.h>
	#include <SegLoad.h>
	#include <OSUtils.h>
	#include <DiskInit.h>
	#include <Packages.h>
	#include <Traps.h>
	#include <InternetConfig.h> /* IC[XXX] in WSLaunchBrowser */
	#include <Sound.h> /* SysBeep() */
	#include <Timer.h> /* Microseconds() */
	#include <Threads.h>
	#include <Files.h>
	#include <Folders.h> /* FindFolder */
	#include <TextUtils.h>
	#include <Aliases.h>
	#include <Gestalt.h>
#endif
/*-----------------------------------------------------------------------------
	WSSTOPWATCH
-----------------------------------------------------------------------------*/

/* http://preserve.mactech.com/articles/develop/issue_26/minow.html */
void WSStopWatch_Construct(WSSTOPWATCH* pThis)
{ WSStopWatch_Begin(pThis); }
void WSStopWatch_Destruct(WSSTOPWATCH* pThis)
{ }

/* Microseconds() is Time Manager/System 7 */
#if defined(TARGET_API_MAC_CARBON) && TARGET_API_MAC_CARBON
	#if !defined(__MACH__)
		#include <Math64.h>
	#endif
	void WSStopWatch_Begin(WSSTOPWATCH* pThis)
	{
		Microseconds((UnsignedWide*)pThis->liSnapshot);
	}
	void WSStopWatch_End(WSSTOPWATCH* pThis)
	{
	}
	size_t WSStopWatch_Value(WSSTOPWATCH* pThis)
	{
		UInt64 uwRes;
		Microseconds((UnsignedWide*)pThis->liFreq);
		(*((UInt64*)pThis->liFreq)) =
			U64Subtract((*((UInt64*)pThis->liFreq)),
				(*((UInt64*)pThis->liSnapshot)));
		uwRes =
			U64Div((*((UInt64*)pThis->liFreq)), 1000);
		return (*((UnsignedWide*)&uwRes)).lo;
	}
#else /* TickCount() from IM: 1 */
	void WSStopWatch_Begin(WSSTOPWATCH* pThis)
	{
		pThis->liSnapshot[0] = TickCount();
	}
	void WSStopWatch_End(WSSTOPWATCH* pThis)
	{
	}
	size_t WSStopWatch_Value(WSSTOPWATCH* pThis)
	{ /* 1/60 of a second (0.01666666666 == 16.6666666667 milliseconds) */
		size_t curval = TickCount() - pThis->liSnapshot[0];
		return curval * 17; /* 16.6666666667 */
	}
#endif
/*-----------------------------------------------------------------------------


	UNIX WSSTOPWATCH


-----------------------------------------------------------------------------*/
#else /* UNIX etc. */
#if defined(__MACH__)
	#include <mach/clock.h>		/* WSSTOPWATCH */
	#include <mach/mach.h>		/* WSSTOPWATCH */
#else
	#include <time.h>		/* time routines incl clock_gettime() */
#endif
/* Unix Time Service for dealing with timespecs */
/* MACH: CALENDAR_CLOCK == UNIX: CLOCK_REALTIME */
/* CLOCK_REALTIME: Wall Clock time affected by NTP/DST? */
/* CLOCK_MONOTONIC: Time since arbitrary point in past */
/* inotify/fsevents/FindFirstChangeNotification/ReadDirectoryChangesW */
#ifdef __MACH__
	typedef clock_serv_t UTS;
	typedef struct mach_timespec WSnativetimespec;
#else
	typedef void* UTS;
	typedef struct timespec WSnativetimespec;
#endif
void UTS_Construct(UTS* uts)
{
#ifdef __MACH__
	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, uts);
#endif
}
void UTS_Destruct(UTS* uts)
{
#ifdef __MACH__
	mach_port_deallocate(mach_task_self(), *uts);
#endif
}
void UTS_GetNativeTimeSpec(UTS* uts, WSnativetimespec* nts)
{
#ifdef __MACH__
	clock_get_time(*uts, nts);
#else
	clock_gettime(CLOCK_MONOTONIC, nts);
#endif
}
void UTS_MillisAdjustedTimeSpec(UTS* uts, struct timespec* ts, size_t msecs)
{
#ifdef __MACH__
	WSnativetimespec mts;
	WSnativetimespec* nts = &mts;
#else
	WSnativetimespec* nts = ts;
#endif
	UTS_GetNativeTimeSpec(uts, nts);
	nts->tv_sec = (unsigned int)(msecs / 1000);
	nts->tv_nsec = (msecs % 1000) * 1000000;
#ifdef __MACH__
	ts->tv_sec = nts->tv_sec;
	ts->tv_nsec = nts->tv_nsec;
#endif
}
/*-----------------------------------------------------------------------------*/
/*	WSSTOPWATCH*/
/*-----------------------------------------------------------------------------*/

void WSStopWatch_Construct(WSSTOPWATCH* pThis)
{
	UTS_Construct((UTS*)&pThis->liHead[0]);
	WSStopWatch_Begin(pThis);
}

void WSStopWatch_Destruct(WSSTOPWATCH* pThis)
{
	UTS_Destruct((UTS*)&pThis->liHead[0]);
}
void WSStopWatch_Begin(WSSTOPWATCH* pThis)
{
	WSnativetimespec ts;
	UTS_GetNativeTimeSpec((UTS*)&pThis->liHead[0], &ts);
	pThis->liSnapshot[0] = (((size_t)ts.tv_nsec) / 1000000)
		+ (((size_t)ts.tv_sec) * 1000);
}
void WSStopWatch_End(WSSTOPWATCH* pThis)
{
}

size_t WSStopWatch_Value(WSSTOPWATCH* pThis)
{
	WSnativetimespec ts;
	UTS_GetNativeTimeSpec((UTS*)&pThis->liHead[0], &ts);
	pThis->liFreq[0] = (((size_t)ts.tv_nsec) / 1000000)
		+ (((size_t)ts.tv_sec) * 1000) - ((size_t)pThis->liSnapshot[0]);
	return pThis->liFreq[0];
}
#endif /* OS Switch */
#ifdef __cplusplus
} /* end extern "C" */
#endif
