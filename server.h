/**
*	@file server.h
*
*	#     #  #####   #####  #######  #####  #    # ####### #######
*	#  #  # #     # #     # #     # #     # #   #  #          #
*	#  #  # #       #       #     # #       #  #   #          #
*	#  #  #  #####   #####  #     # #       ###    #####      #
*	#  #  #       #       # #     # #       #  #   #          #
*	#  #  # #     # #     # #     # #     # #   #  #          #
*	 ## ##   #####   #####  #######  #####  #    # #######    #
*
*	###Abstract###
*
*	Contains WSSOCKETLIBRARY (global initialization for Mac/Windows),
*	Socket and Server interfaces that OS-specific implementations.
*
*	@todo SSL backends can be buggy. DOCUMENTATION.
*
*	@version 0.2
*
*	0.2		Add some missing definitions and add wndclass registering
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
#ifndef __SERVER_H__
#define __SERVER_H__
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
#if defined(__MUTSTR_H__) || defined(WIKISERVER_VERSION)
	#define HAVE_MUTSTR
#else
	typedef struct WSMUTSTR WSMUTSTR;
#endif
/*
	PTRCASTTYPE for integer<->pointer cast warnings
*/
#ifndef PTRCASTTYPE
	#define PTRCASTTYPE(type) (type)(size_t)
#endif
#if !defined(countof)
	#define countof(x) (sizeof(x) / sizeof(*x))
	#define strcountof(x) (countof(x) - 1)
#endif
/*
	Check for the inline keyword support
*/
#ifndef wsinline
	#if defined(_MSC_VER) && (_MSC_VER <= 1600)
		#define wsinline _inline
		#define wsrestrict __restrict
	#elif defined(__cplusplus) || (defined(__STDC_VERSION__) \
								 && __STDC_VERSION__ >= 199901L)
		#define wsinline inline
		#define wsrestrict __restrict
	#else
		#define wsinline
		#define wsrestrict
	#endif
#endif
#ifndef WSMAXSIZE
	#define WSMAXSIZE	(size_t)(~0)
	#define WSUNUSED(x) (void)(x)
#endif
#if !defined(WIKISERVER_VERSION) && defined(macintosh)
	#if defined(TARGET_API_MAC_CARBON) && TARGET_API_MAC_CARBON
		#define WSMACCARBON
	#else
		#define WSMACCLASSIC
	#endif
	#define WSMACCLASSICORCARBON
#endif
/*#############################################################################
				Check for kqueue() presence (FreeBSD or OS X 10.3+)
				Also other helper defines
#############################################################################*/
#if !defined(_WIN32) && defined(WSKQUEUEOPT)
	#if (defined(__MACH__) \
	&& (__GNUC__ > 3 \
		|| (__GNUC__ == 3 && __GNUC_MINOR__ >= 2) \
		|| (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)))
		#define WSKQUEUEMACH
		#define WSKQUEUEGCC3P1
	#elif !defined(__FreeBSD__)
		/*#warning kqueue not supported*/
		#undef WSKQUEUEOPT
	#endif
#endif
#if defined(WSACCEPTEXOPT) || defined(WSCONNECTEXOPT) \
	|| defined(WSDISCONNECTEXOPT) || defined(WSEVENTSELECTOPT)
	#define WSUSEWINSOCK2
#endif
#if !defined(macintosh) \
	&& ((defined(_WIN32) && !defined(WSASYNCSELECTOPT)) \
		|| (!defined(_WIN32) && !defined(WSEPOLLOPT) && !defined(WSKQUEUEOPT)))
	#define WSUSERAWSELECT
#endif
#if defined(WSMACCARBON) || defined(WSMACCARBONSOCKETS) \
	|| (defined(WSMACCLASSIC) && defined(WSUSEOTINCLASSIC))
	#define WSMACUSEOPENTRANSPORT
#endif
#if !defined(__linux__) && defined(WSEPOLLOPT)
	#undef WSEPOLLOPT
#endif

/*#############################################################################
				Everything until Server is C
#############################################################################*/
#if defined(__cplusplus)
extern "C" {
#endif
/*#############################################################################
						WSDLL if needed
#############################################################################*/
#if !defined(WIKISERVER_VERSION)
	typedef struct WSSOCKETDLL
	{
	#if defined(_WIN32)
		struct HINSTANCE__* mHandle;
	#else
		void* mHandle;
	#endif
	} WSSOCKETDLL;
	#define WSDLL WSSOCKETDLL
#endif
/*#############################################################################
				Forward Declares: WSSOCKADDR
#############################################################################*/
typedef struct WSSOCKADDRINADDR
{
	union
	{
		struct { uint8_t s_b1, s_b2, s_b3, s_b4; } S_un_b;
		struct { uint16_t s_w1, s_w2; } S_un_w;
		uint32_t S_addr;
	} S_un;
} WSSOCKADDRINADDR;
typedef struct WSSOCKADDR
{
	uint16_t family;
	uint16_t port;
	WSSOCKADDRINADDR ipv4addr;
	int8_t padding[120];
} WSSOCKADDR;
typedef struct WSSOCKADDRv4
{
	uint16_t family;
	uint16_t port;
	WSSOCKADDRINADDR ipv4addr;
	int8_t padding[8];
} WSSOCKADDRv4;

/*#############################################################################
				Win32 Dynamic Loading Definitions
#############################################################################*/
#if defined(_WIN32)
typedef void* WINSOCKET;
typedef struct _OVERLAPPED OVERLAPPED;
typedef struct sockaddr sockaddr;
typedef struct WSWSANETWORKEVENTS
{
	long lNetworkEvents;
	int  iErrorCode[10]; /* FD_MAX_EVENTS */
} WSWSANETWORKEVENTS;
typedef struct WSOVERLAPPED
{
	void* Internal; void* InternalHigh;
	union {
		struct { uint32_t Offset, OffsetHigh; };
		void* Pointer;
	};
	void* hEvent;
} WSOVERLAPPED;
/* Older headers don't have the Pointer member */
#define OVERLAPPED_DATA(ov) (*((void**)&((ov)->Offset)))
typedef struct _TRANSMIT_FILE_BUFFERS TRANSMIT_FILE_BUFFERS;
typedef struct _TRANSMIT_PACKETS_ELEMENT TRANSMIT_PACKETS_ELEMENT;
typedef wsbool(__stdcall* WSAEVENTSELECTPROC)(WINSOCKET sSocket,
	void* hEventObject, long lNetworkEvents);
typedef wsbool(__stdcall* WSAENUMNETWORKEVENTSPROC)(WINSOCKET sSocket,
	void* hEventObject, WSWSANETWORKEVENTS* lpNetworkEvents);
typedef void* (__stdcall* CREATEIOCOMPLETIONPORTPROC)(
	void* FileHandle, void* ExistingCompletionPort,
	void* CompletionKey, uint32_t NumberOfConcurrentThreads);
typedef wsbool (__stdcall* GETQUEUEDCOMPLETIONSTATUSPROC)(
	void* CompletionPort, uint32_t* lpNumberOfBytes,
	void** lpCompletionKey, OVERLAPPED** lpOverlapped,
	uint32_t dwMilliseconds);
typedef wsbool(__stdcall* CONNECTEXPROC) (WINSOCKET sSocket,
	const sockaddr* name, int namelen,
	void* lpSendBuffer, uint32_t dwSendDataLength,
	uint32_t* lpdwBytesSent, OVERLAPPED* lpOverlapped);
typedef wsbool(__stdcall* TRANSMITPACKETSPROC) (WINSOCKET sSocket,
	TRANSMIT_PACKETS_ELEMENT* lpPacketArray, uint32_t dwElementCount,
	uint32_t dwSendSize, OVERLAPPED* lpOverlapped, uint32_t dwFlags);
typedef void(__stdcall* GETACCEPTEXSOCKADDRSPROC)(
	void* lpOutputBuffer, uint32_t dwReceiveDataLength,
	uint32_t dwLocalAddressLength, uint32_t dwRemoteAddressLength,
	sockaddr** LocalSockaddr, int* pnLocalSockaddrLength,
	sockaddr** RemoteSockaddr, int* pnRemoteSockaddrLength);
typedef wsbool(__stdcall* TRANSMITFILEPROC)(WINSOCKET sSocket,
	void* hFile, uint32_t nNumberOfBytesToWrite,
	uint32_t nNumberOfBytesPerSend, OVERLAPPED* lpOverlapped,
	TRANSMIT_FILE_BUFFERS* lpTransmitBuffers, uint32_t dwReserved);
typedef void(__stdcall* MYOVERLAPPED_COMPLETION_ROUTINEPROC)(
	uint32_t dwError, uint32_t cbTransferred,
	OVERLAPPED* lpOverlapped, uint32_t dwFlags);
typedef int(__stdcall* WSAIOCTLPROC)(void* hSocket,
	uint32_t dwIoControlCode, void* lpvInBuffer,
	uint32_t cbInBuffer, void* lpvOutBuffer,
	uint32_t cbOutBuffer, uint32_t* lpcbBytesReturned,
	OVERLAPPED* lpOverlapped,
	MYOVERLAPPED_COMPLETION_ROUTINEPROC lpCompletionRoutine);
typedef wsbool(__stdcall* ACCEPTEXPROC)(WINSOCKET sListenSocket,
	WINSOCKET sAcceptSocket, void* lpOutputBuffer,
	uint32_t dwReceiveDataLength, uint32_t dwLocalAddressLength,
	uint32_t dwRemoteAddressLength, uint32_t* lpdwBytesReceived,
	struct _OVERLAPPED* lpOverlapped);
typedef wsbool(__stdcall* DISCONNECTEXPROC) (WINSOCKET sSocket,
	struct _OVERLAPPED* lpOverlapped, uint32_t dwFlags, uint32_t dwReserved);
typedef wsbool (__stdcall* CONNECTEXPROC)(WINSOCKET s, const sockaddr* name,
	int namelen, void* lpSendBuffer, uint32_t dwSendDataLength,
	uint32_t* lpdwBytesSent, OVERLAPPED* lpOverlapped);
#endif

/*=============================================================================
			WSSOCKETLIBRARY (Global Initialization)
=============================================================================*/
typedef struct WSSOCKETLIBRARY
{
#if defined(WSMACCLASSIC) && !defined(WSUSEOTINCLASSIC)
	int16_t mIORefNum;
	uint16_t mCreateBufferSize;
#endif
#if defined(_WIN32)
	uint16_t mWinsockVersion;
	#if defined(WSUSEWINSOCK2)
		WSDLL mWs2_32DLL;
	#endif
	#if !defined(WSMSIXOPT) && defined(WSACCEPTEXOPT)
		WSDLL mMswsockDLL;
	#endif
	#if defined(WSACCEPTEXOPT)
		ACCEPTEXPROC mAcceptEx;
		GETACCEPTEXSOCKADDRSPROC mGetAcceptExSockaddrs;
	#endif
	#if defined(WSCONNECTEXOPT) || defined(WSDISCONNECTEXOPT)
		WSAIOCTLPROC mWSAIoctl;
	#endif
	#if defined(WSIOCPOPT)
		CREATEIOCOMPLETIONPORTPROC mCreateIoCompletionPort;
		GETQUEUEDCOMPLETIONSTATUSPROC mGetQueuedCompletionStatus;
	#endif
	#if defined(WSEVENTSELECTOPT)
		WSAEVENTSELECTPROC mWSAEventSelect;
		WSAENUMNETWORKEVENTSPROC mWSAEnumNetworkEvents;
	#endif
#endif
#if defined(WSSSLOPT)
	struct WSSSLLOADER* mSSLLoader;
#endif
#if defined(WSMACUSEOPENTRANSPORT)
	long mOTVersion;
#endif
#if defined(WSAMIGA)
	struct Library* mSocketBase;
#endif
#if !defined(WSMACUSEOPENTRANSPORT) \
	&& !(defined(WSMACCLASSIC) && !defined(WSUSEOTINCLASSIC)) \
	&& !defined(_WIN32) \
	&& !defined(WSSSLOPT)
	void* requiredpadding;
#endif
} WSSOCKETLIBRARY;
wsbool WSSocketLibrary_Create(WSSOCKETLIBRARY* pThis);
int16_t WSSocketLibrary_GetMacTCPRefNum(WSSOCKETLIBRARY* pThis);
uint16_t WSSocketLibrary_GetMacTCPStreamSize(WSSOCKETLIBRARY* pThis);
uint16_t WSSocketLibrary_GetWinsockVersion(WSSOCKETLIBRARY* pThis);
WSDLL* WSSocketLibrary_GetWs2_32DLL(WSSOCKETLIBRARY* pThis);
#if defined(WSSSLOPT)
struct WSSSLLOADER* WSSocketLibrary_GetSSLLoader(WSSOCKETLIBRARY* pThis);
#endif
void WSSocketLibrary_Destroy(WSSOCKETLIBRARY* pThis);

/*=============================================================================
									WSSOCKET
=============================================================================*/
typedef struct WSSOCKET
{
	void* mhSocket;
} WSSOCKET;
#define WSSOCKET_BLOCKING 1
#define WSSOCKET_NONBLOCKING 0
wsbool WSSocketGetDNSServer(WSMUTSTR* dns);
void WSSocket_Construct(WSSOCKET* pThis);
void WSSocket_Destruct(WSSOCKET* pThis);
wsbool WSSocket_Accept(WSSOCKET* pThis, const WSSOCKET* pListener,
	WSSOCKETLIBRARY* socklib);
wsbool WSSocket_CreateTCPv4(WSSOCKET* pThis);
wsbool WSSocket_CreateClient(WSSOCKET* pThis,
	const char* szHostname, size_t port, WSSOCKETLIBRARY* socklib);
wsbool WSSocket_CreateServer(WSSOCKET* pThis, size_t port,
	WSSOCKETLIBRARY* socklib);
wsbool WSSocket_CreateEndpoint(WSSOCKET* pThis, const char* szHostname,
	WSSOCKETLIBRARY* socklib);
void WSSocket_Destroy(WSSOCKET* pThis);
wsbool WSSocket_IsCreated(const WSSOCKET* pThis);
#define WSSocket_IsSSL(pThis) (FALSE)
size_t WSSocket_Read(const WSSOCKET* pThis,
	void* data, size_t num, wsbool* block);
size_t WSSocket_Write(const WSSOCKET* pThis,
	const void* data, size_t num, wsbool* block);

void* WSSocket_Disown(WSSOCKET* pThis);
#define WSSocket_Own(pThis, other) \
	WSASSERT(!WSSocket_IsCreated(pThis)); (pThis)->mhSocket = WSSocket_Disown(other)
#define WSSocket_OwnHandle(pThis, handle) \
	WSASSERT(!WSSocket_IsCreated(pThis)); (pThis)->mhSocket = handle
wsbool WSSocket_SetNonBlocking(WSSOCKET* pThis, wsbool nonBlock);
wsbool WSSocket_SetReuseAddr(WSSOCKET* pThis, wsbool reuseAddr);
wsbool WSSocket_SetIPv6Only(WSSOCKET* pThis, wsbool ipv6only);
void WSSocket_Shutdown(WSSOCKET* pThis);
wsbool WSSocket_ClearReadBuffer(WSSOCKET* pThis, wsbool* inblock);
size_t WSSocket_ReadEndpoint(const WSSOCKET* pThis,
	void* data, size_t num, wsbool* block);
size_t WSSocket_WriteEndpoint(const WSSOCKET* pThis,
	const void* data, size_t num, const char* dest, wsbool* block);
wsbool WSSocket_WaitForRead(const WSSOCKET* pThis,
	size_t microsecs, wsbool* block);
wsbool WSSocket_WaitForWrite(const WSSOCKET* pThis,
	size_t microsecs, wsbool* block);
wsbool WSSocket_WaitForReadWrite(const WSSOCKET* pThis,
	size_t microsecs, wsbool* block);
wsbool WSSocket_GetLocalIPAddress(const WSSOCKET* pThis, WSMUTSTR* ip);
wsbool WSSocket_GetLocalHostname(const WSSOCKET* pThis, WSMUTSTR* hostname);
wsbool WSSocket_GetIPAddress(const WSSOCKET* pThis, WSMUTSTR* ip);
wsbool WSSocket_GetHostname(const WSSOCKET* pThis, WSMUTSTR* hostname);
int WSSocket_Error(const WSSOCKET* pThis);
#define WSSocket_GetHandle(pThis) ((pThis)->mhSocket)
size_t WSSocket_Peek(const WSSOCKET* pThis,
	void* data, size_t num, wsbool* inblock);
#if defined(_WIN32)
	#if defined(WSACCEPTEXOPT)
		void WSSocketAddrToMutStr(WSSOCKADDR* addr, WSMUTSTR* s);
		wsbool WSSocket_AcceptEx(WSSOCKET* pThis, const WSSOCKET* pListener,
			void* data, uint32_t* firstreadlen, uint32_t sockaddrlen,
			OVERLAPPED* lpOverlapped, WSSOCKETLIBRARY* socklib);
		void WSSocketFinishAcceptEx(
			WSSOCKET* pThis, const WSSOCKET* pListener,
			void* data, uint32_t firstreadlen,
			uint32_t sockaddrlen, WSSOCKADDR** localaddr,
			WSSOCKADDR** remoteaddr, WSSOCKETLIBRARY* socklib);
	#endif
	#if defined(WSCONNECTEXOPT)
		wsbool WSSocket_CreateClientEx(WSSOCKET* pThis,
			const char* szHostname, size_t port,
			void* data, size_t datalen,
			OVERLAPPED* lpOverlapped, WSSOCKETLIBRARY* socklib);
	#endif
	#if defined(WSDISCONNECTEXOPT)
		wsbool WSSocket_DisconnectEx(WSSOCKET* pThis,
			OVERLAPPED* lpOverlapped, WSSOCKETLIBRARY* socklib);
	#endif
#endif
/*=============================================================================
									WSSOCKETSSL
=============================================================================*/
#if defined(WSSSLOPT) && !defined(macintosh)
#if defined(_WIN32) && defined(WSNTVSSLOPT)
typedef struct WSSOCKETSSLSECHANDLE
{
	size_t dwLower;
	size_t dwUpper;
} WSSOCKETSSLSECHANDLE;
#endif
typedef struct WSSOCKETSSL
{
	WSSOCKET mSuper;
#if defined(_WIN32) && defined(WSNTVSSLOPT)
	WSSOCKETSSLSECHANDLE mSSL;
	WSSOCKETSSLSECHANDLE mCtx;
	class BYTEVector* mReadBuf;
	class BYTEVector* mReadBackBuf;
	unsigned long mHeaderSize;
	unsigned long mMaxMessageSize;
#else
	struct ssl_st* mSSL;
	struct ssl_ctx_st* mCtx;
#endif
	struct WSSSLLOADER* mSSLLoader;
} WSSOCKETSSL;
void WSSocketSSL_Construct(WSSOCKETSSL* pThis);
void WSSocketSSL_Destruct(WSSOCKETSSL* pThis);
wsbool WSSocketSSL_Accept(WSSOCKETSSL* pThis, const WSSOCKETSSL* pListener,
	WSSOCKETLIBRARY* socklib);
wsbool WSSocketSSL_CreateClient(WSSOCKETSSL* pThis, WSSOCKET* pSocket,
		WSSOCKETLIBRARY* socklib, const char* certPath,
		const char* keyPath);
wsbool WSSocketSSL_CreateServer(WSSOCKETSSL* pThis, size_t port,
		const char* certPath, const char* keyPath, WSSOCKETLIBRARY* socklib
);
void WSSocketSSL_Destroy(WSSOCKETSSL* pThis);
wsbool WSSocketSSL_IsCreated(const WSSOCKETSSL* pThis);
wsbool WSSocketSSL_IsSSL(const WSSOCKETSSL* pThis);
size_t WSSocketSSL_Read(const WSSOCKETSSL* pThis,
	void* data, size_t num, wsbool* block);
size_t WSSocketSSL_Write(const WSSOCKETSSL* pThis,
	const void* data, size_t num, wsbool* block);
void WSSocketSSL_Own(WSSOCKETSSL* pThis, WSSOCKETSSL* pOther);
#define WSSocketSSL_CreateEndpoint(pThis, szHostname, socklib) \
	WSSocket_CreateEndpoint(&(pThis)->mSuper, szHostname, socklib)
/* #define WSSocketSSL_Disown WSSocket_Disown */
#define WSSocketSSL_OwnHandle(pThis, handle) \
	WSSocket_OwnHandle(&(pThis)->mSuper, handle)
#define WSSocketSSL_SetNonBlocking(pThis, nonblock) \
	WSSocket_SetNonBlocking(&(pThis)->mSuper, nonblock)
#define WSSocketSSL_SetReuseAddr(pThis, reuseaddr) \
	WSSocket_SetReuseAddr(&(pThis)->mSuper, reuseaddr)
#define WSSocketSSL_SetIPv6Only(pThis, ipv6only) \
	WSSocket_SetIPv6Only(&(pThis)->mSuper, ipv6only)
#define WSSocketSSL_Shutdown(pThis) \
	WSSocket_Shutdown(&(pThis)->mSuper)
#define WSSocketSSL_ClearReadBuffer(pThis, block) \
	WSSocket_ClearReadBuffer(&(pThis)->mSuper, block)
#define WSSocketSSL_Peek(pThis, data, num, block) \
	WSSocket_Peek(&(pThis)->mSuper, data, num, block)
#define WSSocketSSL_ReadEndpoint(pThis, data, num, block) \
	WSSocket_ReadEndpoint(&(pThis)->mSuper, data, num, block)
#define WSSocketSSL_WriteEndpoint(pThis, data, num, block) \
	WSSocket_WriteEndpoint(&(pThis)->mSuper, data, num, block)
#define WSSocketSSL_WaitForRead(pThis, microsecs, block) \
	WSSocket_WaitForRead(&(pThis)->mSuper, microsecs, block)
#define WSSocketSSL_WaitForWrite(pThis, microsecs, block) \
	WSSocket_WaitForWrite(&(pThis)->mSuper, microsecs, block)
#define WSSocketSSL_WaitForReadWrite(pThis, microsecs, block) \
	WSSocket_WaitForReadWrite(&(pThis)->mSuper, microsecs, block)
#define WSSocketSSL_GetIPAddress(pThis, ip) \
	WSSocket_GetIPAddress(&(pThis)->mSuper, ip)
#define WSSocketSSL_GetHostname(pThis, hostname) \
	WSSocket_GetHostname(&(pThis)->mSuper, hostname)
#define WSSocketSSL_Error(pThis) WSSocket_Error(&(pThis)->mSuper)
#define WSSocketSSL_GetHandle(pThis) WSSocket_GetHandle(&(pThis)->mSuper)
#else
#define WSSOCKETSSL WSSOCKET
#define WSSocketSSL_Construct WSSocket_Construct
#define WSSocketSSL_Destruct WSSocket_Destruct
#define WSSocketSSL_Accept WSSocket_Accept
#define WSSocketSSL_CreateClient(pThis, szHostname, port, socklib, un1, un2) \
	WSSocket_CreateClient(pThis, szHostname, port, socklib)
#define WSSocketSSL_CreateServer WSSocket_CreateServer
#define WSSocketSSL_Destroy WSSocket_Destroy
#define WSSocketSSL_Read WSSocket_Read
#define WSSocketSSL_Write WSSocket_Write
#define WSSocketSSL_IsCreated WSSocket_IsCreated
#define WSSocketSSL_IsSSL WSSocket_IsSSL
#define WSSocketSSL_CreateEndpoint WSSocket_CreateEndpoint
#define WSSocketSSL_Disown WSSocket_Disown
#define WSSocketSSL_Own WSSocket_Own
#define WSSocketSSL_OwnHandle WSSocket_OwnHandle
#define WSSocketSSL_SetNonBlocking WSSocket_SetNonBlocking
#define WSSocketSSL_SetReuseAddr WSSocket_SetReuseAddr
#define WSSocketSSL_SetIPv6Only WSSocket_SetIPv6Only
#define WSSocketSSL_Shutdown WSSocket_Shutdown
#define WSSocketSSL_ClearReadBuffer WSSocket_ClearReadBuffer
#define WSSocketSSL_ReadEndpoint WSSocket_ReadEndpoint
#define WSSocketSSL_WriteEndpoint WSSocket_WriteEndpoint
#define WSSocketSSL_WaitForRead WSSocket_WaitForRead
#define WSSocketSSL_WaitForWrite WSSocket_WaitForWrite
#define WSSocketSSL_WaitForReadWrite WSSocket_WaitForReadWrite
#define WSSocketSSL_GetIPAddress WSSocket_GetIPAddress
#define WSSocketSSL_GetHostname WSSocket_GetHostname
#define WSSocketSSL_Error WSSocket_Error
#define WSSocketSSL_GetHandle WSSocket_GetHandle
#define WSSocketSSL_Peek WSSocket_Peek
#endif /* !defined(macintosh) */
#define WSSocketSSLGetDNSServer WSSocketGetDNSServer


/*=============================================================================
						WSSOCKETNOTIFIER
=============================================================================*/
#define SERVER_DEFAULTSOCKETNUM 512
#define SERVER_NUM_EVENTS 62
typedef struct WSSOCKETNOTIFIERPOST
{
#if defined(_WIN32)
	WSOVERLAPPED mOverlapped;
#endif
	void* mBuffer;
	size_t mPos;
	size_t mSize;
	wsbool mShouldFreeBuffer;
	wsbool mIsOperationComplete;
	wsbool mIsRead;
	wsbool mIsWrite;
	wsbool mIsAccept;
	struct WSSOCKETNOTIFIERPOST* mNext;
	struct WSSOCKETNOTIFIERPOST* mPrev;
} WSSOCKETNOTIFIERPOST;
typedef struct WSSOCKETNOTIFIERINFO
{
	WSSOCKET mSocket;
	size_t mLastActiveTime;
	wsbool mShouldDestroySocket;
	wsbool mCanRead;
	wsbool mCanWrite;
	wsbool mCanAccept;
	wsbool mIsListener;
	wsbool mIsListenerClient;
	wsbool mIsDisconnected;
#if defined(WSMACUSEOPENTRANSPORT)
	wsbool mNeedsOrderlyDisconnect;
#endif
#if defined(WSMACCLASSICORCARBON) || defined(WSMACCARBONSOCKETS)
	wsbool mHasEvents;
	size_t mNumRequests;
#endif
#if defined(_WIN32) && defined(WSIOCPOPT)
	wsbool mIsDead;
#endif
#if (defined(_WIN32) \
		&& (defined(WSEVENTSELECTOPT) || defined(WSASYNCSELECTOPT))) \
	|| defined(WSMACCLASSICORCARBON) || defined(WSMACCARBONSOCKETS)
	#if defined(_WIN32) && defined(WSEVENTSELECTOPT)
		uint32_t mEventIndex;
	#endif
	struct WSSOCKETNOTIFIERINFO* mNextForEvent;
	struct WSSOCKETNOTIFIERINFO* mPrevForEvent;
#endif
#if defined(WSMACCLASSICORCARBON) || defined(WSMACCARBONSOCKETS)
	struct WSSOCKETNOTIFIER* mNotifier;
#endif
	struct WSSOCKETNOTIFIERINFO* mNext;
	struct WSSOCKETNOTIFIERINFO* mPrev;
	struct WSSOCKETNOTIFIERPOST* mPending;
} WSSOCKETNOTIFIERINFO;
typedef struct WSSOCKETNOTIFIER
{
#if defined(_WIN32)
	#if defined(WSASYNCSELECTOPT) || defined(WSEVENTSELECTOPT)
		union
		{
			struct WSSOCKETNOTIFIERINFO** mSocketsForEvents;
			struct WSSOCKETNOTIFIERINFO* mSocketsForAsyncSelect;
		};
		union
		{
			struct
			{
				size_t mNumSockets;
				void** mEvents;
				uint32_t mNumEvents;
				#if defined(WSEVENTSELECTOPT)
					uint32_t mEventIndex;
				#endif
			};
			struct
			{
				struct HWND__* mWnd;
				#if defined(WIKISERVER_VERSION)
					WSEVENTLOOP mLoop;
				#else
					unsigned short mAtom;
				#endif
			};
		};
	#endif
	#if defined(WSIOCPOPT)
		WSSOCKETNOTIFIERINFO* mDeadSockets;
		void* mhIOCP;
	#endif
#elif defined(WSKQUEUEOPT) && !defined(__linux__)
	struct kevent* mKqueueEvents;
	int mNumEvents;
	int mCurEvent;
	int mKQueue;
#elif defined(WSEPOLLOPT) && (!defined(__MACH__) && !defined(__FreeBSD__))
	struct epoll_event* mEpollEvents;
	int mNumFDReady;
	int mCurFD;
	int mEpollFD;
#endif
#if defined(WSUSERAWSELECT)
	void* mMaxFD;
	struct WSNOTIFIERFDSETS* mFDSets;
	int mNumSelectedFDs;
	int mCurFD;
#endif
#if defined(WSMACCLASSICORCARBON) || defined(WSMACCARBONSOCKETS)
	size_t mNumAcceptReqs;
	WSSOCKETNOTIFIERINFO* mNextInfoForEvent;
#endif /* Mac (Open Transport/MacTCP) only */
	WSSOCKETNOTIFIERINFO* mSockets;
	WSSOCKETNOTIFIERPOST* mLastPost;
	WSSOCKETLIBRARY* mSocketLibRef;
} WSSOCKETNOTIFIER;

void WSSocketNotifier_Construct(WSSOCKETNOTIFIER* pThis);
wsbool WSSocketNotifier_Create(WSSOCKETNOTIFIER* pThis,
	WSSOCKETLIBRARY* socklib);
#if defined(_WIN32)
#define WSSOCKETNOTIFIER_BACKEND_RAWSELECT		1
#define WSSOCKETNOTIFIER_BACKEND_EVENTSELECT	2
#define WSSOCKETNOTIFIER_BACKEND_IOCP			4
#define WSSOCKETNOTIFIER_BACKEND_ASYNCSELECT	8
#define WSSOCKETNOTIFIER_BACKEND_ANY			(1|2|4|8)
wsbool WSSocketNotifier_CreateWithBackend(WSSOCKETNOTIFIER* pThis,
	WSSOCKETLIBRARY* socklib, int backend);
#endif
void WSSocketNotifier_Destroy(WSSOCKETNOTIFIER* pThis);
void WSSocketNotifier_Destruct(WSSOCKETNOTIFIER* pThis);
void WSSocketNotifier_InitListenerClientInfo(
	WSSOCKETNOTIFIER* pThis, WSSOCKETNOTIFIERINFO* info);
void WSSocketNotifier_InitClientInfo(
	WSSOCKETNOTIFIER* pThis, WSSOCKETNOTIFIERINFO* info);
void WSSocketNotifier_InitListenerInfo(
	WSSOCKETNOTIFIER* pThis, WSSOCKETNOTIFIERINFO* info);
WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddListenerClientSocket(
	WSSOCKETNOTIFIER* pThis, WSSOCKET* sock);
WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddClientSocket(
	WSSOCKETNOTIFIER* pThis, WSSOCKET* sock);
WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddListenerSocket(
	WSSOCKETNOTIFIER* pThis, WSSOCKET* sock);
void WSSocketNotifier_AddInfo(WSSOCKETNOTIFIER* pThis,
	WSSOCKETNOTIFIERINFO* pInfo);
wsbool WSSocketNotifier_TrackInfo(WSSOCKETNOTIFIER* pThis,
	WSSOCKETNOTIFIERINFO* pInfo);
#if defined(_WIN32) && defined(WSACCEPTEXOPT)
void WSSocketNotifier_UntrackInfo(WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO* entry);
#endif
void WSSocketNotifier_UpdateInfo(WSSOCKETNOTIFIER* pThis,
	WSSOCKETNOTIFIERINFO* pInfo);
void WSSocketNotifier_RemoveInfo(WSSOCKETNOTIFIER* pThis,
	WSSOCKETNOTIFIERINFO* pInfo);
void WSSocketNotifier_DestroyInfo(WSSOCKETNOTIFIER* pThis,
	WSSOCKETNOTIFIERINFO* pInfo);
void WSSocketNotifier_RemoveWatch(WSSOCKETNOTIFIER* pThis,
	WSSOCKETNOTIFIERINFO* sock);
void WSSocketNotifier_Begin(WSSOCKETNOTIFIER* pThis);
wsbool WSSocketNotifier_Next(WSSOCKETNOTIFIER* pThis,
	WSSOCKETNOTIFIERINFO** ppEvent, WSSOCKETNOTIFIERPOST** ppPost);
wsbool WSSocketNotifier_IsCreated(WSSOCKETNOTIFIER* pThis);
WSSOCKETNOTIFIERPOST* WSSocketNotifierAllocPost(
	WSSOCKETNOTIFIERINFO* pInfo, void* pvData,
	size_t len, size_t pos, wsbool shouldFreeData, char isWrite);
void WSSocketNotifierAddPost(
	WSSOCKETNOTIFIERINFO* pInfo, WSSOCKETNOTIFIERPOST* pPost);
wsbool WSSocketNotifierPostRead(WSSOCKETNOTIFIERINFO* pThis,
	void* pvData, size_t len, size_t pos, wsbool shouldFreeData);
wsbool WSSocketNotifierPostWrite(WSSOCKETNOTIFIERINFO* pThis,
	const void* pvData, size_t len, size_t pos, wsbool shouldFreeData);
#define WSSocketNotifier_Root(pThis) ((pThis)->mSockets)

/*=============================================================================
						WSSERVER
=============================================================================*/
typedef struct SERVER SERVER;
typedef wsbool (*SERVERCLIENTCONNECTEDPROC)(SERVER*,
	WSSOCKETNOTIFIERINFO*);
typedef void (*SERVERCLIENTDISCONNECTEDPROC)(SERVER*,
	WSSOCKETNOTIFIERINFO*);
typedef wsbool (*SERVERCLIENTIOCOMPLETEPROC)(SERVER*,
	WSSOCKETNOTIFIERINFO*, WSSOCKETNOTIFIERPOST*);

typedef struct SERVER
{
	SERVERCLIENTCONNECTEDPROC mOnClientConnect;
	SERVERCLIENTDISCONNECTEDPROC mOnClientDisconnect;
	SERVERCLIENTIOCOMPLETEPROC mOnClientIOComplete;
	wsbool mWillShutdown;
	wsbool mWillRestart;
#if defined(WSTHREADEDSERVEROPT)
	/*void OnWorkerThreadEntry(WSSOCKETNOTIFIERINFO* entry);*/
	/*static bool ClientConnectProc(ThreadedServer*, WSSOCKETNOTIFIERINFO* entry);*/
	/*bool DoThreadedClientConnect(WSSOCKETNOTIFIERINFO* entry);*/
	struct WSSERVERWORKERDATA* mWorkers;
	size_t mNumWorkers;
	WSCRITICALSECTION mDistributionLock;
	size_t mCurrentWorker;
	WSRWLOCK mConfigLock;
	/*CLIENTCONNECTEDPROC mWorkerConnectedProc;*/
#else
	WSSOCKETNOTIFIER mPoller;
#endif
	size_t mMaxClients;
	WSSOCKETLIBRARY mSocketLib;
} SERVER;

size_t Server_GetTimeout(const SERVER* pThis);
void Server_OOM(SERVER* pThis);
wsbool Server_Process(SERVER* pThis);
#define Server_ShutdownAfterTransaction(pThis) ((pThis)->mWillShutdown = TRUE)
#define Server_WillRestart(pThis) (pThis)->mWillRestart
#define Server_WillShutdown(pThis) ((pThis)->mWillShutdown == TRUE)
#define Server_RestartAfterTransaction(pThis) \
	(Server_ShutdownAfterTransaction(pThis), (pThis)->mWillRestart = TRUE)
#define Server_GetSocketLibrary(pThis) (&(pThis)->mSocketLib)
void Server_Construct(SERVER* pThis, SERVERCLIENTCONNECTEDPROC conproc,
	SERVERCLIENTDISCONNECTEDPROC dcproc, SERVERCLIENTIOCOMPLETEPROC ioproc,
	size_t numClients);
wsbool Server_Create(SERVER* pThis, size_t port);
void Server_Destroy(SERVER* pThis);
void Server_ProcessNotifier(SERVER* pThis, WSSOCKETNOTIFIER* pNotifier);
WSSOCKETNOTIFIERINFO* Server_DistributeSocket(SERVER* pThis, WSSOCKET* socket);
wsbool Server_AddAcceptExSockets(SERVER* pThis, WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO* listenerinfo, size_t amount);


/*void ServerEmptyDisconnectProc(WSSOCKETNOTIFIERINFO* WSUNUSED(newclient)) {};*/
WSSOCKETSSL* ServerGetQueueEntrySocket(WSSOCKETNOTIFIERINFO* qe);
wsbool ServerIsIODataWrite(WSSOCKETNOTIFIERPOST* data);
void ServerGetIODataBuffer(WSSOCKETNOTIFIERPOST* data, void** buffer,
	size_t* pos, size_t* size);
void ServerSetIODataAutoFree(WSSOCKETNOTIFIERPOST* data, wsbool autofree);
#if defined(WSDLL)
	#undef WSDLL
#endif
#if defined(__cplusplus)
} /* end extern "C" */
#endif
#endif /* __SERVER_H__ */
#if defined(WSSOCKET_IMPLEMENTATION)
/******************************************************************************
			 #####  #####                                     #####
			 #     #     #  ####  #    # #    #  ####  #    #     #
			 #     #       #    # ##  ## ##  ## #    # ##   #     #
			 #     #       #    # # ## # # ## # #    # # #  #     #
			 #     #       #    # #    # #    # #    # #  # #     #
			 #     #     # #    # #    # #    # #    # #   ##     #
			 #####  #####   ####  #    # #    #  ####  #    # #####
******************************************************************************/
#if !defined(WIKISERVER_VERSION)
	#include <stdlib.h> /* malloc */
	#include <string.h> /* memXXX */
	#ifdef _DEBUG
		#include <assert.h> /* assert */
		#define WSASSERT(x) assert(x)
		#define WSRELEASEUNUSED(x)
	#else
		#define WSASSERT(x)
		#define WSRELEASEUNUSED(x) WSUNUSED(x)
	#endif
	#define WSMemoryCopy memcpy
	#define WSMemoryFill memset
	#define WSMemoryMove memmove
	/* SetWindowLongPtr==SetWindowLong on 32-bit targets (64 bit warnings) */
	#if defined(_WIN32)
		#if defined(_X86_) || defined(_M_IX86)
			#define longptr_t long
			#define WSSetWindowLongPtr(hw, i, p) \
				SetWindowLongPtr(hw, i, (LONG)(LONG_PTR)p)
		#else
			#define longptr_t __int64
			#define WSSetWindowLongPtr(hw, i, p) \
				SetWindowLongPtr(hw, i, (LONG_PTR)p)
		#endif
	#endif
#endif
/******************************************************************************
			 #     # ### #     #  #####  #######  #####  #    #
			 #  #  #  #  ##    # #     # #     # #     # #   #
			 #  #  #  #  # #   # #       #     # #       #  #
			 #  #  #  #  #  #  #  #####  #     # #       ###
			 #  #  #  #  #   # #       # #     # #       #  #
			 #  #  #  #  #    ## #     # #     # #     # #   #
			  ## ##  ### #     #  #####  #######  #####  #    #

			 ######   #####  ######   #####  #######  #####  #    #
			 #     # #     # #     # #     # #     # #     # #   #
			 #     # #       #     # #       #     # #       #  #
			 ######   #####  #     #  #####  #     # #       ###
			 #     #       # #     #       # #     # #       #  #
			 #     # #     # #     # #     # #     # #     # #   #
			 ######   #####  ######   #####  #######  #####  #    #

	@file socket.c

	###Abstract###

	Windows and Unix implementation for Socket/SocketNotifier.

	Supports: Windows/OSX/Unix/AMIGA

	###Notes####

	For compatability we do NOT use WinSock2 directly, we link to
	WinSock 1 (winsock32.lib - ws2_32 is WinSock2!), then dynload the rest!

	@todo  ifdef SO_NOSIGPIPE setsockopt SO_NOSIGPIPE after socket().
******************************************************************************/
#if !defined(macintosh)
/*=============================================================================
								Headers
=============================================================================*/
/*#include "system.h"*/
#ifndef PRECOMPILEDHEADERS
	#if defined(WSSSLOPT) && defined(_WIN32) && defined(WSNTVSSLOPT)
		/* Pull in Vector for the SChannel backend */
		#include "vector.h"
	#endif
	/*#include "server.h"*/
	/* Windows Headers */
	#ifdef _WIN32
		#define _WINSOCK_DEPRECATED_NO_WARNINGS /*gethostbyname/inet_addr*/
		#define WIN32_LEAN_AND_MEAN
		#define NOSERVICE
		#define NOMCX
		#define NOIME
		#define NOSOUND
		#define NOCOMM
		#define NOKANJI
		#define NORPC
		#define NOPROXYSTUB
		#define NOIMAGE
		#define NOTAPE
		/* Note that mingw goes nuts including other win headers before */
		#if defined(WSMSIXOPT)
			#include <winsock2.h>
			#pragma comment (lib, "ws2_32.lib")
		#else
			#include <winsock.h>
		#endif
		#include <limits.h>
	/* Unix/Linux Headers */
	#elif defined(WSAMIGA)
		#include <sys/types.h>
		#include <exec/types.h>
		#include <proto/exec.h>
		#include <proto/dos.h>
		#include <proto/socket.h>
		#include <sys/filio.h>
	#else
		#include <sys/types.h>		/* uchar & other reqs for apple headers */
		#ifdef __MWERKS__
			#include <strings.h>	/* bzero for FDZERO */
		#endif
		#include <sys/time.h>		/* timeval */
		#include <sys/socket.h> 	/* socket() */
		#include <netinet/in.h>
		#include <arpa/inet.h>
		#include <sys/ioctl.h>
		#include <fcntl.h>
		#include <unistd.h>
		#include <netdb.h>			/* gethostbyaddr */
		#include <errno.h>			/* errno global */
		#if defined(WSKQUEUEGCC3P1)
			#include <sys/event.h>
		#elif defined(WSKQUEUEOPT)
			#include <sys/event.h>
			#include <sys/time.h> 	/* timeval */
		#elif defined(WSEPOLLOPT)
			#include <sys/epoll.h>
		#endif /* kqueue/epoll */
		#include <signal.h>
	#endif
#endif /* NOPRECOMPILEDHEADERS*/
/*=============================================================================
							WIN32 Definitions
=============================================================================*/
#ifdef _WIN32
	#if defined(WSMSIXOPT) /* As no dynloading, pull in all the winsock defs */
		#include <iphlpapi.h>
		#pragma comment (lib, "iphlpapi.lib")
	#endif
	#define SOCKETERROR(x) WSLogError(x)
	#define WSSOCKETTYPE void*
	#define WSsocklen int
	typedef int AcceptLengthType;
	typedef const char* INetArg;
	typedef const char* SendArg;
	#if !defined(_MSC_VER) || (_MSC_VER < 1300) /* MSVC <= 6 */
		#define UINT_PTR unsigned int
		#define LONG_PTR long
		#define ULONG_PTR unsigned long
		#define DWORD_PTR unsigned long
		#define PDWORD_PTR unsigned long*
		#ifdef SetWindowLongPtr
		#	undef SetWindowLongPtr
		#endif
		#ifdef GetWindowLongPtr
		#	undef GetWindowLongPtr
		#endif
		#ifdef GWLP_USERDATA
			#undef GWLP_USERDATA
		#endif
		#ifdef GWLP_WNDPROC
			#undef GWLP_WNDPROC
		#endif
		#define SetWindowLongPtr SetWindowLong
		#define GetWindowLongPtr GetWindowLong
		#define GWLP_USERDATA GWL_USERDATA
		#define GWLP_WNDPROC GWL_WNDPROC
	#endif /* !defined(_MSC_VER) || _MSC_VER < 1300 */
/*=============================================================================
							Unix Definitions
=============================================================================*/
#else /* !_WIN32 */
	#define WSAEINTR		EINTR
	#define WSAEWOULDBLOCK	EAGAIN
	#define WSAEADDRINUSE	EADDRINUSE
	#define INVALID_SOCKET			-1
	#define SOCKET_ERROR			-1
	#define WSAEINPROGRESS EINPROGRESS
	#define WSAENETDOWN ENETDOWN
	#define WSAENOTSOCK ENOTSOCK
	#define WSSOCKETTYPE void*
	/*#if !defined(WSKQUEUEOPT) || defined(WSKQUEUEGCC3P1)*/
		#define WSsocklen socklen_t
	/*#else*/
	/*	#define WSsocklen int*/
	/*#endif*/ /* Project Builder/OS X 10.2 doesn't have socklen_t*/
	#define SOCKETERROR(x)
	#define Sleep usleep
	#if defined(WSAMIGA)
		#define SOCKET					LONG
		LONG WSAGetLastError()
		{ return Errno(); }
		#define closesocket				CloseSocket
		#define ioctlsocket				IoctlSocket
		#ifndef select
			#define select(nfds,rfds,wfds,efds,timeout) \
				WaitSelect(nfds,rfds,wfds,efds,timeout,NULL)
		#endif
		typedef char* INetArg;
		typedef void* SendArg;
		struct Library* SocketBase;
	#else
		#define SOCKET					int
		int WSAGetLastError() { return errno; }
		#define closesocket				close
		#define ioctlsocket				ioctl
		typedef const char* INetArg;
		typedef const char* SendArg;
	#endif
		typedef unsigned int AcceptLengthType;
#endif /*_WIN32*/

/*#############################################################################
				C++ Guard
#############################################################################*/
#ifdef __cplusplus
	extern "C" {
#endif

#if !defined(WIKISERVER_VERSION)
/*#############################################################################
				WSDLL if needed
#############################################################################*/
#define LPWSDLLPROC void*
#define WSDLL_Create WSSocketDLL_Create
#define WSDLL_IsLoaded(pThis) ((pThis)->mHandle != NULL)
#define WSDLL_GetProc WSSocketDLL_GetProc
#define WSDLL_Destroy WSSocketDLL_Destroy
#define SckWinFunctionBase SckWinFunctionBase
#define WSDLL WSSOCKETDLL
#if defined(_WIN32)
/* GetModuleHandle is banned in Windows Store; get HMODULE by func address */
struct HINSTANCE__* SckWinFunctionBase(void* func)
{ /*  Get the base module (kernel32/user32.dll) for passing to GetProc */
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	VirtualQuery(func, &mbi, sizeof(mbi));
	return (struct HINSTANCE__*)(mbi.AllocationBase);
}
int WSDLL_Create(WSDLL* pThis, const char* libraryname)
{
	pThis->mHandle = LoadLibraryA(libraryname);
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
#elif defined(__MACH__)
#if defined(__has_builtin) && __has_builtin(__is_target_arch) \
		&& __has_builtin(__is_target_vendor) && __has_builtin(__is_target_os) \
		&& __has_builtin(__is_target_environment) && __is_target_arch(x86_64) \
		&& __is_target_vendor(apple) && __is_target_os(ios)
/* iOS, at least in the past, can't dynload properly */
int WSDLL_Create(WSDLL* pThis, const char* libraryname)
{
	(void)(pThis); (void)(libraryname);
	return FALSE;
}
void WSDLL_Destroy(WSDLL* pThis)
{
	(void)(pThis);
}
void* WSDLL_GetProc(const WSDLL* pThis, const char* procname)
{
	(void)(pThis); (void)(procname);
	return NULL;
}
#else
/*
	Note that OS X did not have dlopen() until 10.3. Its version of dlopen()
	et al. just call NSSymbol methods the exact same way we do.
*/
#include <mach-o/dyld.h>
typedef void (*initProcPtr)(void);
typedef void (*finiProcPtr)(void);

int WSDLL_Create(WSDLL* pThis, const char* libraryname)
{
	NSObjectFileImage image;
	NSSymbol initSymbol;

	/* Create the file image that will contian the module we load procs from */
	if (NSCreateObjectFileImageFromFile(libraryname, &image)
		!= NSObjectFileImageSuccess)
		return FALSE;

	/* Now get the module from the file image which we load procs from */
	pThis->mHandle = (NSModule)
		NSLinkModule(image, libraryname,
				 NSLINKMODULE_OPTION_BINDNOW
					 | NSLINKMODULE_OPTION_RETURN_ON_ERROR);
	NSDestroyObjectFileImage(image); /* yes, it's safe to free the image */
	if (!pThis->mHandle)
		return FALSE;

	/* Call __init (_init in C) if it exists, required when loading Mach-O */
	initSymbol = NSLookupSymbolInModule((NSModule)pThis->mHandle, "__init");
	if(initSymbol)
	{
		initProcPtr initProc = (initProcPtr)NSAddressOfSymbol(initSymbol);
		(*initProc)();
	}
	return TRUE;
}
void WSDLL_Destroy(WSDLL* pThis)
{
	/* Call __fini (_fini in C) if it exists, req unloading Mach-O */
	NSSymbol finiSymbol =
		NSLookupSymbolInModule((NSModule)pThis->mHandle, "__fini");
	if(finiSymbol)
	{
		finiProcPtr finiProc = (finiProcPtr)NSAddressOfSymbol(finiSymbol);
		(*finiProc)();
	}
	NSUnLinkModule((NSModule)pThis->mHandle, NSUNLINKMODULE_OPTION_NONE);
}
void* WSDLL_GetProc(const WSDLL* pThis, const char* procname)
{
	void* retProc;
	NSSymbol symbol;
	char* symbolname;
	size_t symbolnamelength = CStrLength(procname);

	/* Mach-O names in C are prefixed with an underscore that
	NSLookupSymbol[XXX] doesn't adjust for. dlopen() does exactly the same. */
	symbolname = WSALLOC(char, symbolnamelength + 2);
	if(!symbolname)
		return NULL;
	symbolname[0] = '_'; /* +1 in size to incl '\0' */
	WSMemoryCopy(&symbolname[1], procname, symbolnamelength + 1);

	/*
		Now that that's over with, actually load the symbol. Apple's Macho-O
		interface loads it into a NSSymbol structure before loading the actual
		address (in case you wanted detailed info about the symbol itself).

		It should be noted that if NSLookupSymbolInModule does not return
		NULL then NSAddressOfSymbol WILL return a valid proc.
	*/
	symbol = NSLookupSymbolInModule((NSModule)pThis->mHandle, symbolname);
	if(symbol)
		retProc = NSAddressOfSymbol(symbol);
	else
		retProc = NULL;
	WSFREE(symbolname);
	return retProc;
}
#endif /* __MACH__ */
#else
#include <dlfcn.h>		/* dlopen (link with -ldl) */
int WSDLL_Create(WSDLL* pThis, const char* libraryname)
{
	pThis->mHandle = dlopen(libraryname, RTLD_LAZY);
	return pThis->mHandle != NULL;
}
void WSDLL_Destroy(WSDLL* pThis)
{
	dlclose(pThis->mHandle);
}
void* WSDLL_GetProc(const WSDLL* pThis, const char* procname)
{ return dlsym(pThis->mHandle, procname); }
#endif
#if defined(_WIN32) && !defined(WSDLL_CreateKernel32)
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
			(HMODULE)(SckWinFunctionBase((void*)SckWinFunctionBase))
		#define WSDLL_CreateKernel32(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)VirtualQuery))
		#define WSDLL_DestroyKernel32(pThis)
		#define WSDLL_CreateUser32(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)CreateMenu))
		#define WSDLL_DestroyUser32(pThis)
	#endif
	#if defined(WSMSIXOPT)
		#define WSDLL_CreateMswsock(pThis) \
				((pThis)->mHandle = SckWinFunctionBase((void*)AcceptEx))
		#define WSDLL_DestroyMswsock(pThis)
		#define WSDLL_CreateWs2_32(pThis) \
				((pThis)->mHandle = SckWinFunctionBase((void*)WSAEventSelect))
		#define WSDLL_DestroyWs2_32(pThis)
		#define WSDLL_CreateShell32(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)ShellExecuteA))
		#define WSDLL_DestroyShell32(pThis)
		#define WSDLL_CreateGdi32(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)GetDeviceCaps))
		#define WSDLL_DestroyGdi32(pThis)
		#define WSDLL_CreateAdvapi32(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)RegCloseKey))
		#define WSDLL_DestroyAdvapi32(pThis)
		#define WSDLL_CreateNtdll(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)RtlGetVersion))
		#define WSDLL_DestroyNtdll(pThis)
		#define WSDLL_CreateUxtheme(pThis) \
			((pThis)->mHandle = \
				SckWinFunctionBase((void*)DrawThemeBackground))
		#define WSDLL_DestroyUxtheme(pThis)
		#define WSDLL_CreateOle32(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)CoInitialize))
		#define WSDLL_DestroyOle32(pThis)
		#define WSDLL_CreateOleaut32(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)SysFreeString))
		#define WSDLL_DestroyOleaut32(pThis)
		#define WSDLL_CreateComctl32(pThis) \
			((pThis)->mHandle = \
				SckWinFunctionBase((void*)InitCommonControlsEx))
		#define WSDLL_DestroyComctl32(pThis)
		#define WSDLL_CreateSecur32(pThis) \
			((pThis)->mHandle = \
				SckWinFunctionBase((void*)AcceptSecurityContext))
		#define WSDLL_DestroySecur32(pThis)
		#define WSDLL_CreateCrypt32(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)CertCloseStore))
		#define WSDLL_DestroyCrypt32(pThis)
		#define WSDLL_CreateCryptui(pThis) \
			((pThis)->mHandle = \
				SckWinFunctionBase((void*)CryptUIDlgSelectCertificateA))
		#define WSDLL_DestroyCryptui(pThis)
		#define WSDLL_CreateIphlpapi(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)GetNetworkParams))
		#define WSDLL_DestroyIphlpapi(pThis)
		#define WSDLL_CreateDsound(pThis) \
			((pThis)->mHandle = SckWinFunctionBase((void*)DirectSoundCreate))
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
/*#############################################################################
				Doubly-linked list macros
#############################################################################*/
/* Adds a node to a doubly-linked list without a dedicated root node */
#define DList_AddWithoutRoot(top, s, next, prev)\
	{ \
		/* Link the new node (s) after the last node (*top)->prev */ \
		(*top)->prev->next = s; \
		/* Set the new node's previous link to the last node */ \
		s->prev = (*top)->prev; \
		/* Update the last node's previous link to the new node */ \
		(*top)->prev = s; \
		/* Set the new node's next link to the first node */ \
		s->next = (*top); \
	}

/* Removes a node from a doubly-linked list without a dedicated root node */
#define DList_RemoveWithoutRoot(top, s, next, prev)\
	{ \
		/* Link the previous node to the next node*/ \
		/* bypassing the current node (s) */ \
		s->prev->next = s->next; \
		/* Link the next node's previous to the previous node, */ \
		/* removing current node (s) from the list */ \
		s->next->prev = s->prev; \
	}

/* Initializes a rootless doubly-linked list */
#define DList_InitRootless(top, next, prev)\
	{ \
		/* Set the next and previous pointers of the */ \
		/* first node to point to itself */ \
		(*top)->next = (*top)->prev = (*top); \
	}

/* Base macro for adding a node to the list,
used by both DList_Update and DList_Add */
#define DList_AddBase(top, s, next, prev, adding)\
	{ \
		/* Check if the list is not empty */ \
		if((*top))\
		{ \
			/* If not empty, use the DList_AddWithoutRoot to add the node */ \
			DList_AddWithoutRoot(top, s, next, prev);\
		} \
		else \
		{ \
			/* If empty, set the top to the new node */ \
			(*top) = s; \
			/* If adding, initialize the node to point to itself */ \
			if(adding)\
				s->next = s->prev = s; \
		} \
	}

/* Updates a doubly-linked list by adding a node */
#define DList_Update(top, s, next, prev) \
	DList_AddBase(top, s, next, prev, FALSE)

/* Adds a new node to the doubly-linked list */
#define DList_Add(top, s, next, prev) DList_AddBase(top, s, next, prev, TRUE)

/* Removes a node from the doubly-linked list */
#define DList_Remove(top, s, next, prev) \
	{ \
		/* If the node to remove is the first node */ \
		if(s == *top)\
		{ \
			/* If it's the only node, set the list to NULL */ \
			if((*top)->next == *top)\
				*top = NULL;\
			else \
				/* Otherwise, set the top to the next node */ \
				*top = (*top)->next;\
		}\
		/* Use the DList_RemoveWithoutRoot to remove the node */ \
		DList_RemoveWithoutRoot(top, s, next, prev); \
	}
/*#############################################################################
				WSLog/EXPLICITPROCCAST/WSCLAMP
#############################################################################*/
#if !defined(WSLogDebug) && !defined(WIKISERVER_VERSION)
	#define WSLogDebug(x)
	#define WSLogError(x)
#endif
#ifndef EXPLICITPROCCAST
	#ifdef _MSC_VER
		#define EXPLICITPROCCAST(type) (type)(void*)
	#else
		#define EXPLICITPROCCAST(type) (type)
	#endif
#endif
#ifndef WSTYPECEILING
	#define WSTYPECEILING(type) (~((type)0))
	#define WSSTYPECEILING(type) (((type)1 << (sizeof(type) * 8 - 1)) - 1)
	#define WSSTYPEFLOOR(type) (-(type)(((type)1 << (sizeof(type) * 8 - 1))))
	#define WSCLAMP(x, l, h) (((x) > (h)) ? (h) : (((x) < (l)) ? (l) : (x)))
	#define WSUPPERCLAMP(x, y) (((x) > (y)) ? (y) : (x))
	#define WSUPPERCLAMPTYPE(var, type) \
		((type)WSUPPERCLAMP(var, (type)WSTYPECEILING(type)))
	#define WSSCLAMPTYPE(var, type) \
		((type)WSCLAMP(var, (type)WSSTYPECEILING(type), \
			(type)WSSTYPEFLOOR(type)))
	#define WSSUPPERCLAMPTYPE(var, type) \
		((type)WSUPPERCLAMP(var, (type)WSSTYPECEILING(type)))
#endif
/*#############################################################################
				WSTimeGetLocalValue/WSThreadYield
#############################################################################*/

#define WSTimeGetLocalValue WSSocketTimeGetLocalValue
#define WSThreadYield WSSocketThreadYield
#if defined(_WIN32)
	#define SOCKETFILETIME_SECOND ((size_t)10000000)
	#define SOCKETTIMET_BASE 11644473600
	size_t SocketFileTimeToCTime(FILETIME* pFT, size_t* fracsecs)
	{
		UINT64 t64 = *((UINT64*)pFT);
		if(fracsecs)
			*fracsecs = t64 % SOCKETFILETIME_SECOND;
		t64 /= SOCKETFILETIME_SECOND;
		t64 -= SOCKETTIMET_BASE;
		return t64;
	}
	size_t WSTimeGetLocalValue()
	{
		SYSTEMTIME st;
		FILETIME ft;
		GetLocalTime(&st);
		SystemTimeToFileTime(&st, &ft);
		return SocketFileTimeToCTime(&ft, NULL);
	}
	void WSThreadYield()
	{
		MSG msg; /* has to be while, msgwfmo only returns for NEW messages */
		while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				PostQuitMessage((int)msg.wParam);
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
			}
		}
		Sleep(10);
		/*Must be above Sleep(0) context switch to avoid 100% CPU usage
			(at least 10)*/
		/*MsgWaitForMultipleObjects(0, NULL, 0, 10, QS_ALLINPUT);*/
	}
#else
	#include <time.h> /* time() */
	void WSThreadYield()
	{
		#if (defined(__FreeBSD__) || defined(__DragonFly__) \
				|| defined(__OpenBSD__) || defined(__linux__))
			usleep(10);
		#elif defined(__human68k__)
			extern void	_dos_change_pr (void);
			_dos_change_pr();
		#endif /* Delay(2); on AMIGA */
	}

	size_t WSTimeGetLocalValue()
	{
		time_t theT = time(NULL);
		WSASSERT(theT != (time_t)-1);
		return theT;
	}
#endif /* _WIN32 */
/*#############################################################################
				WSFile
#############################################################################*/
#if (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__) \
			|| defined(__linux__)) || defined (_WIN32)
	#if defined(_MSC_VER)
		#include <io.h>		/* _open et al. */
		#include <fcntl.h>	/* _O_RDWR and other _O */
		#include <sys/stat.h>	/* fstat for both */
	#else
		#include <unistd.h>
		#include <time.h>
		#ifndef __MWERKS__
			#include <sys/stat.h>	/* fstat for both */
		#endif
	#endif
	#ifdef __MWERKS__
		#include <fcntl.h>	/* _O_RDWR and other _O */
	#endif
	#include <stdio.h>
	#define WSFILE WSSOCKETFILE
	#define WSFile_Create WSSocketFile_Create
	#define WSFile_Destroy WSSocketFile_Destroy
	#define WSFile_Read WSSocketFile_Read
	#define FILE_READ			  1
	#define FILE_WRITE			  2
	#define FILE_READWRITE		  3
	#define FILE_CREATEALWAYS	  4
	#define FILE_BINARY			  8
	#define FILE_FROMCURRENT	  1 /* SEEK_CUR */
	#define FILE_FROMSTART		  0 /* SEEK_SET */
	#define FILE_FROMEND		  2 /* SEEK_END */
	#if defined(_MSC_VER)
		#define open _open
		#define lseek _lseek
		#define close _close
		#define read _read
		#define fsync _commit
		#define fstat _fstat
		#define stat _stat
		#define O_RDWR _O_RDWR
		#define O_WRONLY _O_WRONLY
		#define O_TRUNC _O_TRUNC
		#define O_CREAT _O_CREAT
	#endif
	typedef struct WSFILE
	{
		int mhFile;
	} WSFILE;
	wsbool WSFile_Create(WSFILE* pThis, const char* szPath, long lFlags)
	{
		#if defined(_MSC_VER)
			#pragma warning(push)
			#pragma warning(disable:4996) /* open "unsafe" */
		#endif
		pThis->mhFile = open(szPath,
							  (lFlags & FILE_WRITE ?
								  (lFlags & FILE_READ ? O_RDWR : O_WRONLY) :
								  O_RDONLY) | //O_BINARY |
								 (lFlags & FILE_CREATEALWAYS ?
									  O_TRUNC | O_CREAT : 0),
							  0777);
		#if defined(_MSC_VER)
			#pragma warning(pop)
		#endif
		if (pThis->mhFile == -1)
			return FALSE;
		return TRUE;
	}
	void WSFile_Destroy(WSFILE* pThis)
	{
		if (pThis->mhFile != -1 && pThis->mhFile > 2)
		{
			close(PTRCASTTYPE(int)pThis->mhFile);
			pThis->mhFile = -1;
		}
	}
	size_t WSFile_Read(const WSFILE* pThis, void* p, size_t nin)
	{
		#if defined(_MSC_VER)
			unsigned n = WSUPPERCLAMPTYPE(nin, unsigned);
		#else
			size_t n = nin;
		#endif
		ptrdiff_t ret = read(PTRCASTTYPE(int)pThis->mhFile, p, n);
		if (ret == -1)
			return 0;
		return ret;
	}
	#if defined(_MSC_VER)
		#undef open
		#undef lseek
		#undef close
		#undef read
		#undef fsync
		#undef fstat
		#undef stat
		#undef O_RDWR
		#undef O_WRONLY
		#undef O_TRUNC
		#undef O_CREAT
	#endif
#endif /* _WIN32/UNIX */
#endif /* WIKISERVER_VERSION */
/*#############################################################################
				URIParseIPv4address (for GetDNSServer on Unix)
#############################################################################*/
#if (!defined(__URI_H__) || !defined(URI_IMPLEMENTATION)) \
		&& (defined(__FreeBSD__) || defined(__DragonFly__) \
			|| defined(__OpenBSD__) || defined(__linux__))
#define SocketURIIsDigit(c) (c >= '0' && c <= '9')
#define URIIsDigit SocketURIIsDigit
#define URIParseIPv4address SocketURIParseIPv4address
wsbool URIParseIPv4address(const char** uri)
{
	/*
		IPv4address	= dec-octet "." dec-octet "." dec-octet "." dec-octet

		dec-octet	=	   DIGIT					; 0-9
					  / %x31-39 DIGIT				; 10-99
					  / "1" 2DIGIT					; 100-199
					  / "2" %x30-34 DIGIT			; 200-249
					  / "25" %x30-35				; 250-255
	*/
	size_t iIPv4 = 0;
	const char* uriOrig = (*uri);
	if (URIIsDigit(*(*uri)))
	{
		++iIPv4;
		/* each ip part must be between 0-255 (dupe of version in for loop)*/
		if ((URIIsDigit(*++(*uri)) && URIIsDigit(*++(*uri)) &&
		   /* 100 or less  (note !)*/
		   !( (*((*uri)-2) == '1') ||
		   /* 240 or less*/
			 (*((*uri)-2) == '2' &&
			   (*((*uri)-1) < '5' || (*((*uri)-1) == '5' && *(*uri) <= '5'))
			 )
			)
		   ) || (((*uri) - uriOrig >= 2) && (*uriOrig == '0')) /* leading 0*/
		  ) { return FALSE; }
		if (URIIsDigit(*(*uri)))
			++(*uri);
		for (; iIPv4 < 4; ++iIPv4) /*compilers should unroll this loop*/
		{
			if (*(*uri) != '.' || !URIIsDigit(*++(*uri)))
				break;
			/*each ip part must be between 0-255*/
			uriOrig = (*uri);
			if ((URIIsDigit(*++(*uri)) && URIIsDigit(*++(*uri)) &&
			   /* 100 or less  (note !)*/
			   !( (*((*uri)-2) == '1') ||
			   /* 240 or less*/
				 (*((*uri)-2) == '2' &&
				   (*((*uri)-1) < '5'
						|| (*((*uri)-1) == '5' && *(*uri) <= '5'))
				 )
				) /* *uriOrig == '0' == leading 0*/
			   ) || (((*uri) - uriOrig >= 2) && (*uriOrig == '0'))
			  ) { return FALSE; }
			if (URIIsDigit(*(*uri)))
				++(*uri);
		}
	}
	return iIPv4 == 4;
}
#endif
/*#############################################################################
				WSFD_SET (FD_SET wrapper for broken compilers)
#############################################################################*/
#ifdef _MSC_VER
	wsinline void WSFD_SET(SOCKET fd, fd_set* fds)
	{ /* MVC generates constant expression warning */
	#pragma warning(push)
	#pragma warning(disable:4127)
		FD_SET(fd, fds);
	#pragma warning(pop)
	}
#else
	#define WSFD_SET(x, y) FD_SET(x, y)
#endif /* _MSC_VER */

/*#############################################################################
				WSNOTIFIERFDSETS
#############################################################################*/
typedef struct WSNOTIFIERFDSETS
{
	fd_set read, write, lost;
	fd_set readcur, writecur, lostcur;
	struct timeval tv;
	WSSOCKETNOTIFIERINFO* nextinfo;
} WSNOTIFIERFDSETS;

/*#############################################################################
				Generic IPv6 structures for platform independence
#############################################################################*/
#ifndef SOMAXCONN
#define SOMAXCONN 5
#endif
typedef struct WSSOCKADDR6IN6ADDR
{
	union
	{
		uint8_t	__u6_addr8[16];
		uint16_t __u6_addr16[8];
		uint32_t __u6_addr32[4];
	} __in6_u;
} WSSOCKADDR6IN6ADDR;

typedef struct WSSOCKADDR6
{
	uint16_t sin6_family;
	uint16_t sin6_port;
	uint32_t sin6_flowinfo;
	WSSOCKADDR6IN6ADDR sin6_addr;
	uint32_t sin6_scope_id;
} WSSOCKADDR6;

#if !defined(AF_INET6)
	#ifdef _WIN32
		#define AF_INET6 23
	#else
		#define AF_INET6 10
	#endif
#endif
#if !defined(IPPROTO_IPV6)
	#define IPPROTO_IPV6 41
	#define IPV6_V6ONLY 27
#else
#define WSHASIPV6INHEADERS
#endif

/*#############################################################################
						LogEntries() for debugging
#############################################################################*/
#if defined(WIKISERVER_VERSION)
void LogEntries(WSSOCKETNOTIFIERINFO* root)
{
	WSCONSOLE con;
	char hexbuf[HexToStringMaxChars()];
	WSSOCKETNOTIFIERINFO* cur = root;
	WSSOCKETNOTIFIERINFO* next;
	WSMUTSTR logstring;
	WSMutStr_Create(&logstring);
	HexToString(hexbuf, (size_t)cur);
	WSMutStr_AppendCString(&logstring, "STATUS: ROOT [");
	WSMutStr_AppendCString(&logstring, hexbuf);
	WSMutStr_AppendCString(&logstring, "]");
	if(root)
	{
		next = cur->mNext;
		while(next != root)
		{
			cur = next;
			HexToString(hexbuf, (size_t)cur);
			WSMutStr_AppendCString(&logstring, " --> [");
			WSMutStr_AppendCString(&logstring, hexbuf);
			WSMutStr_AppendCString(&logstring, "]");
			next = cur->mNext;
		}
	}
	WSConsole_ConstructOutput(&con);
	WSConsole_Write(&con, WSMutStr_GetCString(&logstring));
	WSConsole_Write(&con, "\n");
	WSConsole_Destruct(&con);
	WSMutStr_Destroy(&logstring);
}
#endif
/*=============================================================================

						WSSOCKETLIBRARY

=============================================================================*/
wsbool WSSocketLibrary_Create(WSSOCKETLIBRARY* pThis)
{
#if defined(WSSSLOPT)
	/* Load OpenSSL/SChannel */
	pThis->mSSLLoader = new NOTHROW WSSSLLOADER;
	if (!pThis->mSSLLoader
		|| !WSSSLLoader_Create(pThis->mSSLLoader))
	{
		delete pThis->mSSLLoader;
		pThis->mSSLLoader = NULL;
	}
#endif

#ifdef _WIN32
	#if defined(WSIOCPOPT)
		WSDLL kernel32;
	#endif
	/* Initialize Winsock */
	/* We try version 2 first for AcceptEx and others. */
	/* There are some catches here: */
	/* 1a) On Windows 95, it does not correctly detect the 2 */
	/*	   in the requested version, so we either need to*/
	/*	   specify 2.1 or check for the 2 in the version.*/
	/*	   (we currently do the latter)*/
	/* 1b) After this if the 2.0 succeeded thus far we need */
	/*	   to make sure the platform has WSAIoctl and*/
	/*	   AcceptEx available - if not we fall back to 1.1*/
	WORD wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;
	/* For reference:
	GUID GuidAcceptEx =
	{ 0xb5367df1,0xcbac,0x11cf,{0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92} };
	GUID GuidTransmitFile =
	{ 0xb5367df0,0xcbac,0x11cf,{0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92} };
	GUID GetAcceptExSockaddrs =
	{ 0xb5367df2,0xcbac,0x11cf,{0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92} };
	GUID GuidTransmitPackets =
	{ 0xd9689da0,0x1f90,0x11d3,{0x99,0x71,0x00,0xc0,0x4f,0x68,0xc8,0x76} };*/

	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		WSLogDebug("Initial init of Winsock 2.0 failed, falling back to 1.1");
		wVersionRequested = MAKEWORD(1, 1);
		if (WSAStartup(wVersionRequested, &wsaData) != 0)
		{
			WSLogError("FATAL: Cannot initialize WinSock");
			return FALSE;
		}
		#if defined(WSUSEWINSOCK2)
			pThis->mWs2_32DLL.mHandle = NULL;
		#endif
		#if !defined(WSMSIXOPT) && defined(WSACCEPTEXOPT)
			pThis->mMswsockDLL.mHandle = NULL;
		#endif
	}
	else
	{ /* Load AcceptEx (Winsock 1.1) if needed */
		#if !defined(WSMSIXOPT) && defined(WSACCEPTEXOPT)
			WSDLL_CreateMswsock(&pThis->mMswsockDLL);
			pThis->mAcceptEx = (ACCEPTEXPROC)
				WSDLL_GetProc(&pThis->mMswsockDLL, "AcceptEx");
			pThis->mGetAcceptExSockaddrs = (GETACCEPTEXSOCKADDRSPROC)
				WSDLL_GetProc(&pThis->mMswsockDLL, "GetAcceptExSockaddrs");
		#endif
		#if defined(WSUSEWINSOCK2)
			if ((wsaData.wHighVersion & 2))
			{
				WSDLL_CreateWs2_32(&pThis->mWs2_32DLL);
			#if defined(WSEVENTSELECTOPT)
				pThis->mWSAEventSelect =
					EXPLICITPROCCAST(WSAEVENTSELECTPROC)
					WSDLL_GetProc(&pThis->mWs2_32DLL, "WSAEventSelect");
				pThis->mWSAEnumNetworkEvents =
					EXPLICITPROCCAST(WSAENUMNETWORKEVENTSPROC)
					WSDLL_GetProc(&pThis->mWs2_32DLL, "WSAEnumNetworkEvents");
				if (!pThis->mWSAEnumNetworkEvents)
					pThis->mWSAEventSelect = NULL;
			#endif
			}
			else
			{
				pThis->mWs2_32DLL.mHandle = NULL;
				#if defined(WSEVENTSELECTOPT)
					pThis->mWSAEventSelect = NULL;
					pThis->mWSAEnumNetworkEvents = NULL;
				#endif
			}
		#endif
		#if defined(WSMSIXOPT) && defined(WSACCEPTEXOPT)
			WSASSERT((wsaData.wHighVersion & 2));
			/* AcceptEx/GetAcceptExSockAddrs have spastic linkage and need to be
				hard linked rather dynamically linked. */
			BOOL __stdcall AcceptEx(SOCKET sListenSocket, SOCKET sAcceptSocket,
				PVOID lpOutputBuffer, DWORD dwReceiveDataLength,
				DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength,
				LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped);
			void __stdcall GetAcceptExSockaddrs(
				void* lpOutputBuffer, uint32_t dwReceiveDataLength,
				uint32_t dwLocalAddressLength, uint32_t dwRemoteAddressLength,
				struct sockaddr** LocalSockaddr, int* pnLocalSockaddrLength,
				struct sockaddr** RemoteSockaddr, int* pnRemoteSockaddrLength);
			pThis->mAcceptEx = (ACCEPTEXPROC) AcceptEx;
			pThis->mGetAcceptExSockaddrs =
				(GETACCEPTEXSOCKADDRSPROC) GetAcceptExSockaddrs;
		#endif
	}
	pThis->mWinsockVersion = wsaData.wHighVersion;
	#if defined(WSIOCPOPT)
		WSDLL_CreateKernel32(&kernel32);
		pThis->mCreateIoCompletionPort = (CREATEIOCOMPLETIONPORTPROC)
			WSDLL_GetProc(&kernel32, "CreateIoCompletionPort");
		pThis->mGetQueuedCompletionStatus = (GETQUEUEDCOMPLETIONSTATUSPROC)
			WSDLL_GetProc(&kernel32, "GetQueuedCompletionStatus");
		WSDLL_DestroyKernel32(&kernel32);
		if (!pThis->mGetQueuedCompletionStatus)
			pThis->mCreateIoCompletionPort = NULL;
	#endif
#else
	#if defined(WSAMIGA)
		SocketBase =
			pThis->mSocketBase = OpenLibrary("bsdsocket.library", 0);
		if(!pThis->mSocketBase)
			return FALSE;
	#else
		(void)(pThis);
	#endif
#endif
	return TRUE;
}

#ifdef _WIN32
uint16_t WSSocketLibrary_GetWinsockVersion(WSSOCKETLIBRARY* pThis)
{
	return pThis->mWinsockVersion;
}

WSDLL* WSSocketLibrary_GetWs2_32DLL(WSSOCKETLIBRARY* pThis)
{
	#if defined(WSUSEWINSOCK2)
		return &pThis->mWs2_32DLL;
	#else
		return NULL;
	#endif
}
#endif

#if defined(WSSSLOPT)
struct WSSSLLOADER* WSSocketLibrary_GetSSLLoader(WSSOCKETLIBRARY* pThis)
{
	return pThis->mSSLLoader;
}
#endif

void WSSocketLibrary_Destroy(WSSOCKETLIBRARY* pThis)
{
#if defined(WSSSLOPT)
	if (pThis->mSSLLoader)
	{
		WSSSLLoader_Destroy(pThis->mSSLLoader);
		delete pThis->mSSLLoader;
		pThis->mSSLLoader = NULL;
	}
#endif
#if defined(WSAMIGA)
	CloseLibrary(pThis->mSocketBase);
#elif _WIN32
	#if defined(WSUSEWINSOCK2)
		WSDLL_DestroyWs2_32(&pThis->mWs2_32DLL);
	#endif
	#if !defined(WSMSIXOPT) && defined(WSACCEPTEXOPT)
	WSDLL_DestroyMswsock(&pThis->mMswsockDLL);
	#endif
	WSACleanup();
#else
	(void)(pThis);
#endif
}

/*=============================================================================

								WSSOCKET

=============================================================================*/
void WSSocket_Construct(WSSOCKET* pThis)
{ pThis->mhSocket = ((WSSOCKETTYPE)INVALID_SOCKET); }
void WSSocket_Destruct(WSSOCKET* pThis)
{
	if(WSSocket_IsCreated(pThis))
		WSSocket_Destroy(pThis);
}
void WSSocket_Shutdown(WSSOCKET* pThis)
{
	/* Socket shutdown is.... complicated */
	/* https://stackoverflow.com/questions/48208236/tcp-close-vs-shutdown-in-linux-os */
	/* https://docs.microsoft.com/en-us/windows/win32/winsock/graceful-shutdown-linger-options-and-socket-closure-2 */
	/* ideal is this, but this blocks so we need to rework some things */
	/* 1) shutdown(PTRCASTTYPE(SOCKET)pThis->mhSocket, SHUT_WR) */
	/* 2) shutdown(PTRCASTTYPE(SOCKET)pThis->mhSocket, SHUT_RD) */
	/* 3) closesocket(PTRCASTTYPE(SOCKET)pThis->mhSocket) */
	shutdown(PTRCASTTYPE(SOCKET)pThis->mhSocket, 1);
	shutdown(PTRCASTTYPE(SOCKET)pThis->mhSocket, 0);
}
wsbool WSSocket_ClearReadBuffer(WSSOCKET* pThis, wsbool* inblock)
{
	char* buffer = WSALLOC(char, 1024);
	size_t numRead = (size_t)-1;
	wsbool block = inblock ? *inblock : WSSOCKET_NONBLOCKING;
	if(WSSocket_WaitForRead(pThis, 0, inblock))
	{
		do
		{ numRead = WSSocket_Read(pThis, buffer, 1024, &block); }
		while (numRead != (size_t)-1 && numRead != 0);
		if(inblock) *inblock = block;
	}
	WSFREE(buffer);
	return numRead != (size_t)-1;
}
void WSSocket_Destroy(WSSOCKET* pThis)
{
	int res;
	struct linger linger;
	linger.l_onoff = 1;
	linger.l_linger = 0;
	res = setsockopt(PTRCASTTYPE(SOCKET)pThis->mhSocket, SOL_SOCKET, SO_LINGER,
						(char*)&linger, sizeof(linger));
	WSASSERT(res == 0);
	res = closesocket(PTRCASTTYPE(SOCKET)pThis->mhSocket);
	WSASSERT(res == 0);
	WSRELEASEUNUSED(res);
#if 0
	/* shutdown() is called implicitly by closesocket() */
	/*int sockerr = shutdown(PTRCASTTYPE(SOCKET)pThis->mhSocket, 2);*/
	sockerr = sockerr == SOCKET_ERROR
		|| closesocket(PTRCASTTYPE(SOCKET)pThis->mhSocket) == SOCKET_ERROR;
	if (sockerr)
	{
#if 0 /* print out errors from shutdown() and close() for debugging */
		switch (WSAGetLastError())
		{
#define SSEE(x) case x: { WSLogError(#x); break;}
			SSEE(WSAENETDOWN)
				SSEE(WSAENOTSOCK)
				SSEE(WSAEINPROGRESS)
				SSEE(WSAEINTR)
				SSEE(WSAEWOULDBLOCK)
#ifdef _WIN32 /* WinSock-specific */
				SSEE(WSAEPROCLIM)
				SSEE(WSANOTINITIALISED)
				SSEE(WSASYSNOTREADY)
				SSEE(WSAVERNOTSUPPORTED)
#endif
		default:
			{
				WSLogError("UNKNOWN");
				break;
			}
		}
#endif
		closesocket(PTRCASTTYPE(SOCKET)pThis->mhSocket);
	} /* end if shutdown or close failed */
#endif
	pThis->mhSocket = (WSSOCKETTYPE)INVALID_SOCKET;
}
wsbool WSSocket_IsCreated(const WSSOCKET* pThis)
{
	return pThis->mhSocket != (WSSOCKETTYPE)INVALID_SOCKET;
}
void* WSSocket_Disown(WSSOCKET* pThis)
{
	WSSOCKETTYPE rets = pThis->mhSocket;
	pThis->mhSocket = ((WSSOCKETTYPE)INVALID_SOCKET);
	return rets;
}
wsbool WSSocket_SetNonBlocking(WSSOCKET* pThis, wsbool nonBlock)
{
	u_long option = (u_long)nonBlock;
	return ioctlsocket(PTRCASTTYPE(SOCKET)pThis->mhSocket, FIONBIO, &option) != SOCKET_ERROR;
}

wsbool WSSocket_SetReuseAddr(WSSOCKET* pThis, wsbool reuseAddr)
{
	u_long option = (u_long)reuseAddr;
	return setsockopt(PTRCASTTYPE(SOCKET)pThis->mhSocket, SOL_SOCKET, SO_REUSEADDR,
						(char *)&option, sizeof(option)) != SOCKET_ERROR;
}

wsbool WSSocket_SetIPv6Only(WSSOCKET* pThis, wsbool ipv6only)
{
	u_long option = (u_long)ipv6only;
	return setsockopt(PTRCASTTYPE(SOCKET)pThis->mhSocket, IPPROTO_IPV6, IPV6_V6ONLY,
						(char *)&option, sizeof(option)) != SOCKET_ERROR;
}

wsbool WSSocket_Accept(WSSOCKET* pThis, const WSSOCKET* listenSocket,
	WSSOCKETLIBRARY* socklib)
{
	struct sockaddr_in inaddr;
	WSsocklen inaddrsize = sizeof(struct sockaddr_in);
	(void)(socklib);
	pThis->mhSocket = PTRCASTTYPE(WSSOCKETTYPE)
		accept(PTRCASTTYPE(SOCKET)WSSocket_GetHandle(listenSocket),
			(struct sockaddr*)&inaddr, &inaddrsize);
	if (pThis->mhSocket == (WSSOCKETTYPE)INVALID_SOCKET)
	{
		/*WSLogError("accept() failed");*/
		return FALSE;
	}
	if(!WSSocket_SetNonBlocking(pThis, TRUE))
	{
		WSLogDebug("Could not set accepted socket to non-blocking");
	}

	return TRUE;
}

wsbool WSSocket_CreateServerIPV6(WSSOCKET* pThis,
	size_t port, WSSOCKETLIBRARY* socklib)
{
	int bindres;
	WSSOCKADDR6 inaddr6;
	(void)(socklib);
	pThis->mhSocket =
		PTRCASTTYPE(WSSOCKETTYPE)socket(AF_INET6, SOCK_STREAM, 0);
	if (pThis->mhSocket != (WSSOCKETTYPE)INVALID_SOCKET)
	{
		if (!WSSocket_SetIPv6Only(pThis, FALSE))
		{
			WSLogDebug("Could not set ipv6 socket to dual stack");
		}
		if (!WSSocket_SetNonBlocking(pThis, TRUE))
		{
			WSLogDebug("Could not set accepted socket to non-blocking");
		}
		if (!WSSocket_SetReuseAddr(pThis, TRUE))
		{
			WSLogDebug("Could not set SO_REUSEADDR");
		}
		WSMemoryFill(&inaddr6, 0, sizeof(inaddr6));
		inaddr6.sin6_family = AF_INET6; /*AF_INET6*/
	#ifdef WSHASIPV6INHEADERS /* generally in6addr_any is all 0s */
		inaddr6.sin6_addr = *(WSSOCKADDR6IN6ADDR*)&in6addr_any; /*in6addr_any;*/
	#endif
		inaddr6.sin6_port = htons(((unsigned short)port)); /*IPPORT_RESERVED*/
		bindres =
			bind(PTRCASTTYPE(SOCKET)pThis->mhSocket,
				(struct sockaddr*)&inaddr6, sizeof(inaddr6));
		if(bindres != SOCKET_ERROR)
			bindres = listen(PTRCASTTYPE(SOCKET)pThis->mhSocket, 5);
		else
		{
			WSLogDebug("IPV6 bind failed");
		}
		if(bindres != SOCKET_ERROR)
			return TRUE;
		else
		{
			WSLogDebug("IPV6 listen failed");
			closesocket(PTRCASTTYPE(SOCKET)pThis->mhSocket);
		}
	}
	return FALSE;
}

wsbool WSSocket_CreateServer(WSSOCKET* pThis,
	size_t port, WSSOCKETLIBRARY* socklib)
{
	/* create socket: socket()->bind()->listen() */
	/*struct sockaddr_in6*/
	struct sockaddr_in inaddr4;
#if 0
	WSSOCKADDR6 inaddr6;
#endif
	(void)(socklib);
	WSASSERT(!WSSocket_IsCreated(pThis));

	/* sockets are overlapped by default in Win32 */
	pThis->mhSocket = PTRCASTTYPE(WSSOCKETTYPE)socket(AF_INET, SOCK_STREAM, 0);

	if (pThis->mhSocket == (WSSOCKETTYPE)INVALID_SOCKET)
	{
		WSLogError("Could not create socket!");
		return FALSE;
	}
	if (!WSSocket_SetNonBlocking(pThis, TRUE))
	{
		WSLogDebug("Could not set accepted socket to blocking");
	}
	if (!WSSocket_SetReuseAddr(pThis, TRUE))
	{
		WSLogDebug("Could not set SO_REUSEADDR");
	}
	WSMemoryFill(&inaddr4, 0, sizeof(inaddr4));
	inaddr4.sin_family = AF_INET;
	inaddr4.sin_addr.s_addr = INADDR_ANY;
	inaddr4.sin_port = htons(((unsigned short)port)); /*IPPORT_RESERVED*/
	if (bind(PTRCASTTYPE(SOCKET)pThis->mhSocket,
		(struct sockaddr*)&inaddr4, sizeof(inaddr4)) == SOCKET_ERROR)
	{
		closesocket(PTRCASTTYPE(SOCKET)pThis->mhSocket);
		if (WSAGetLastError() == WSAEADDRINUSE)
		{
			WSLogError("Port in use!");
		}
		else
		{
			#if defined(HAVE_MUTSTR)
				WSMUTSTR errormessage;
				WSMutStr_Create(&errormessage);
				WSMutStr_AppendCStringLiteral(&errormessage,
					"Could not bind to port [");
				WSMutStr_AppendUDigit(&errormessage, port);
				WSMutStr_AppendCStringLiteral(&errormessage,
					"] (probably need to be root)!\n"
					"Error code:");
				WSMutStr_AppendUDigit(&errormessage, WSAGetLastError());
				WSLogError(WSMutStr_GetCString(&errormessage));
				WSMutStr_Destroy(&errormessage);
			#endif /* HAVE_MUTSTR */
		}
		WSSocket_Destroy(pThis);
		return FALSE;
	}
	while (listen(PTRCASTTYPE(SOCKET)pThis->mhSocket, 5) == SOCKET_ERROR) /* SOMAXCONN == 5 */
	{
		if (WSAGetLastError() != WSAEINTR)
		{
			WSLogError("Error listening on port!");
			WSSocket_Destroy(pThis);
			return FALSE;
		}
	}
	return TRUE;
}
wsbool WSSocket_CreateEndpoint(WSSOCKET* pThis,
	const char* szHostname, WSSOCKETLIBRARY* socklib)
{
	struct sockaddr_in inaddr;
	(void)(socklib);
	WSASSERT(!WSSocket_IsCreated(pThis));
	pThis->mhSocket = PTRCASTTYPE(WSSOCKETTYPE)
		socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (pThis->mhSocket == (WSSOCKETTYPE)INVALID_SOCKET)
		return FALSE;
	WSMemoryFill((char*)&inaddr, 0, sizeof(inaddr));
	/*Host*/
	if (szHostname)
	{
		struct hostent* pHostEntry = gethostbyname((INetArg)szHostname);
		if (pHostEntry)
			WSMemoryCopy(&inaddr.sin_addr,
				pHostEntry->h_addr, pHostEntry->h_length);
		else
		{
			inaddr.sin_addr.s_addr = inet_addr((INetArg)szHostname);
			if (inaddr.sin_addr.s_addr == INADDR_NONE)
			{
				WSSocket_Destroy(pThis);
				return FALSE;
			}
		}
	}
	else
		inaddr.sin_addr.s_addr = INADDR_ANY;
	inaddr.sin_family = AF_INET;

#if 0
	if (bind((SOCKET)pThis->mhSocket,
		(struct sockaddr*)&inaddr, sizeof(inaddr)) == SOCKET_ERROR)
	{ /* WSAEISCONN WSAEWOULDBLOCK loop around with WSThreadYield() */
		MutStr sError;
		sError << Error();
		SOCKETERROR(sError);
		Destroy();
		return FALSE;
	}
#endif
	WSSocket_SetNonBlocking(pThis, TRUE);
	return TRUE;
}

size_t WSSocket_ReadEndpoint(const WSSOCKET* pThis,
	void* data, size_t num, wsbool* block)
{
	size_t res;
	struct sockaddr_in addr;
	struct sockaddr* saddr = (struct sockaddr*)&addr;
	WSsocklen addrlen = sizeof(addr);
	res = (size_t)
		recvfrom(PTRCASTTYPE(SOCKET)pThis->mhSocket, (char*)data, (int)num, 0, saddr, &addrlen);
	if(res == (size_t)SOCKET_ERROR && block)
		*block = WSAGetLastError() == WSAEWOULDBLOCK;
	return res;
}

size_t WSSocket_WriteEndpoint(const WSSOCKET* pThis,
	const void* data, size_t num, const char* dest, wsbool* block)
{
	size_t res;
	struct sockaddr_in addr;
	WSsocklen saddrlen = sizeof(addr);
	WSMemoryFill(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
#ifdef __MACH__
	addr.sin_len = sizeof(addr);
#endif
	addr.sin_addr.s_addr = inet_addr((INetArg)dest);
	addr.sin_port = htons((unsigned short)53);
	saddrlen = sizeof(addr);
	res = (size_t)
		sendto(PTRCASTTYPE(SOCKET)pThis->mhSocket, (SendArg)data, (int)num, 0,
				 (struct sockaddr*)&addr, (int)saddrlen);
	if(res == (size_t)SOCKET_ERROR && block)
		*block = WSAGetLastError() == WSAEWOULDBLOCK;
	return res;
}

wsbool WSSocket_CreateTCPv4(WSSOCKET* pThis)
{
	pThis->mhSocket = PTRCASTTYPE(WSSOCKETTYPE)
		socket(AF_INET, SOCK_STREAM, 0);
	return pThis->mhSocket != (WSSOCKETTYPE)INVALID_SOCKET;
}
wsbool WSSocket_CreateClient(WSSOCKET* pThis, const char* szHostname,
	size_t port, WSSOCKETLIBRARY* socklib)
{
	struct hostent* pHostEntry;
	struct sockaddr_in inaddr;
	(void)(socklib);
	WSASSERT(!WSSocket_IsCreated(pThis));
	/*Create client: socket()->connect(szHostname, port)*/
	/*SOCK_DGRAM, IPPROTO_UDP - datagram using getprotobyname*/
	pThis->mhSocket = PTRCASTTYPE(WSSOCKETTYPE)
		socket(AF_INET, SOCK_STREAM, 0);
	if (pThis->mhSocket == (WSSOCKETTYPE)INVALID_SOCKET)
		return FALSE;
	if (!WSSocket_SetNonBlocking(pThis, TRUE))
	{
		WSLogDebug("Could not set accepted socket to blocking");
	}

	WSMemoryFill((char*)&inaddr, 0, sizeof(inaddr));
	/*Host*/
	pHostEntry = gethostbyname((INetArg)szHostname);
	if (pHostEntry)
		WSMemoryCopy(&inaddr.sin_addr,
			pHostEntry->h_addr, pHostEntry->h_length);
	else
	{
		inaddr.sin_addr.s_addr = inet_addr((INetArg)szHostname);
		if (inaddr.sin_addr.s_addr == INADDR_NONE)
		{
			WSSocket_Destroy(pThis);
			return FALSE;
		}
	}
	/*Port (don't prepend  to this, linux has funky macro)*/
	inaddr.sin_port = htons(((unsigned short)port));
	if (inaddr.sin_port == 0)
	{
		WSSocket_Destroy(pThis);
		return FALSE;
	}
	/*Family*/
	inaddr.sin_family = AF_INET;
	/*connect (call bind() with localaddr before if connecting two endpoints)*/
	if (connect(PTRCASTTYPE(SOCKET)pThis->mhSocket,
		(struct sockaddr*)&inaddr, sizeof(inaddr)) == SOCKET_ERROR)
	{ /* WSAEISCONN WSAEWOULDBLOCK loop around with WSThreadYield() */
		#if defined(HAVE_MUTSTR)
			WSMUTSTR errormessage;
			WSMutStr_Create(&errormessage);
			WSMutStr_AppendUDigit(&errormessage, WSSocket_Error(pThis));
			SOCKETERROR(WSMutStr_GetCString(&errormessage));
			WSMutStr_Destroy(&errormessage);
		#endif
		return FALSE;
	}
	return TRUE;
}

wsbool WSSocket_WaitForReadWrite(const WSSOCKET* pThis, size_t microsecs, wsbool* block)
{
	struct timeval* tvreal;
	struct timeval tv;
	fd_set readfds; fd_set writefds;
	FD_ZERO(&readfds); FD_ZERO(&writefds);
	WSFD_SET((int)(size_t)pThis->mhSocket, &readfds);
	if (microsecs != WSMAXSIZE)
	{
		tv.tv_sec = (int)(microsecs / 1000000);
		tv.tv_usec = (int)(microsecs % 1000000);
		tvreal = &tv;
	}
	else
		tvreal = NULL;
	if(select((int)(((size_t)pThis->mhSocket)+1), &readfds, &writefds, NULL, tvreal) > 0)
		return TRUE;
	if(block)
		*block = WSAGetLastError() == WSAEWOULDBLOCK;
	return FALSE;
}
wsbool WSSocket_WaitForRead(const WSSOCKET* pThis, size_t microsecs, wsbool* block)
{
	struct timeval* tvreal;
	struct timeval tv;
	fd_set readfds; FD_ZERO(&readfds);
	WSFD_SET((int)(size_t)pThis->mhSocket, &readfds);
	if (microsecs != WSMAXSIZE)
	{
		tv.tv_sec = (int)(microsecs / 1000000);
		tv.tv_usec = (int)(microsecs % 1000000);
		tvreal = &tv;
	}
	else
		tvreal = NULL;
	if(select((int)(((size_t)pThis->mhSocket)+1), &readfds, NULL, NULL, tvreal) > 0)
		return TRUE;
	if(block)
		*block = WSAGetLastError() == WSAEWOULDBLOCK;
	return FALSE;
}
wsbool WSSocket_WaitForWrite(const WSSOCKET* pThis, size_t microsecs, wsbool* block)
{
	struct timeval* tvreal;
	struct timeval tv;
	fd_set writefds; FD_ZERO(&writefds);
	WSFD_SET((int)(size_t)pThis->mhSocket, &writefds);
	if (microsecs != WSMAXSIZE)
	{
		tv.tv_sec = (int)(microsecs / 1000000);
		tv.tv_usec = (int)(microsecs % 1000000);
		tvreal = &tv;
	}
	else
		tvreal = NULL;
	if(select((int)(((size_t)pThis->mhSocket)+1), NULL, &writefds, NULL, tvreal) > 0)
		return TRUE;
	if(block)
		*block = WSAGetLastError() == WSAEWOULDBLOCK;
	return FALSE;
}
#if defined(HAVE_MUTSTR)
/*
	GetHostName() is simple enough, but GetIPAddress() has loads upon loads of
	compatability issues with older versions of Windows and Unix, so we
	implement it from scratch ourselves.
*/
wsbool WSSocket_GetHostname(const WSSOCKET* pThis, WSMUTSTR* s)
{
	WSSOCKADDR addr;
	WSsocklen addrsize = sizeof(addr);
	if (getpeername(PTRCASTTYPE(SOCKET)pThis->mhSocket,
		(struct sockaddr*)&addr, &addrsize) != SOCKET_ERROR)
	{
		struct hostent* host = gethostbyaddr((INetArg)&addr.ipv4addr,
			sizeof(addr.ipv4addr), PF_INET);
		if (host)
		{
			WSMutStr_AssignCString(s, host->h_name);
			return TRUE;
		}
		else
		{
			WSLogError("Could not get host name for GetHostName");
			return FALSE;
		}
	}
	else
	{
		WSLogError("Could not get peer name for GetHostName");
		return FALSE;
	}
}
void WSSocketAddrToMutStr(WSSOCKADDR* addr, WSMUTSTR* s)
{
	/* There are so many legacy compatability issues that it's */
	/* best we roll our own. I.E. Windows XP+ has IPv6 available, */
	/* but the standard inet_ntop wasn't available until Vista */
	/* XP = have to use WSAAddressToString->RtlIpv6AddressToString */
	WSMutStr_Reserve(s, 46); /*INET6_ADDRSTRLEN*/
	WSMutStr_SetLength(s, 0);
	if (addr->family == AF_INET)
	{
		WSSOCKADDRINADDR* IPv4addr = (WSSOCKADDRINADDR*)addr;
		WSMutStr_AppendUDigit(s, IPv4addr->S_un.S_un_b.s_b1);
		WSMutStr_AppendChar(s, '.');
		WSMutStr_AppendUDigit(s, IPv4addr->S_un.S_un_b.s_b2);
		WSMutStr_AppendChar(s, '.');
		WSMutStr_AppendUDigit(s, IPv4addr->S_un.S_un_b.s_b3);
		WSMutStr_AppendChar(s, '.');
		WSMutStr_AppendUDigit(s, IPv4addr->S_un.S_un_b.s_b4);
	}
	else if (addr->family == AF_INET6)
	{
		const uint16_t* as_shorts = (const uint16_t*)(addr);
		int runpos[8];
		int current = 1;
		int max = 0;
		int maxpos = -1;
		int run_array_size = countof(runpos);
		int i;

		for (i = 0; i < run_array_size; ++i)
		{
			if (as_shorts[i] == 0)
			{
				runpos[i] = current;
				if (current > max)
				{
					maxpos = i;
					max = current;
				}
				++current;
			}
			else
			{
				runpos[i] = -1;
				current = 1;
			}
		}

		if (max > 0)
		{
			int tmpmax = maxpos;
			for (i = run_array_size - 1; i >= 0; i--)
			{
				if (i > tmpmax)
					runpos[i] = -1;
				else if (runpos[i] == -1)
					tmpmax = -1;
			}
		}

		if (runpos[0] == 1 &&
			(maxpos == 5 || (maxpos == 4 && as_shorts[5] == 0xFFFF)))
		{
			WSSOCKADDRINADDR* IPv4addr;
			WSMutStr_AppendChar(s, ':');
			WSMutStr_AppendChar(s, ':');
			if (maxpos == 4)
				WSMutStr_AppendCStringWithLength(s,
					"ffff:", strcountof("ffff:"));
			IPv4addr = (WSSOCKADDRINADDR*)&(as_shorts[6]);
			WSMutStr_AppendUDigit(s, IPv4addr->S_un.S_un_b.s_b1);
			WSMutStr_AppendChar(s, '.');
			WSMutStr_AppendUDigit(s, IPv4addr->S_un.S_un_b.s_b2);
			WSMutStr_AppendChar(s, '.');
			WSMutStr_AppendUDigit(s, IPv4addr->S_un.S_un_b.s_b3);
			WSMutStr_AppendChar(s, '.');
			WSMutStr_AppendUDigit(s, IPv4addr->S_un.S_un_b.s_b4);
		}
		else
		{
			for (i = 0; i < run_array_size; ++i)
			{
				if (runpos[i] == -1)
				{
					size_t len = WSMutStr_GetLength(s);
					WSMutStr_BeforeWrite(s, 46);
					len += HexToString((char*)&WSMutStr_GetCString(s)[len],
						ntohs(as_shorts[i]));
					WSMutStr_AfterWriteAndNullTerminate(s, len);
					if (i != 7 && runpos[i + 1] != 1)
						WSMutStr_AppendChar(s, ':');
				}
				else if (runpos[i] == 1)
				{
					WSMutStr_AppendChar(s, ':');
					WSMutStr_AppendChar(s, ':');
					i += (max - 1);
				}
			}
		}
	} /* end if ipv6 */
}
wsbool WSSocket_GetIPAddress(const WSSOCKET* pThis, WSMUTSTR* s)
{
	WSSOCKADDR addr;
	WSsocklen addrsize = sizeof(addr);
	if (getpeername(PTRCASTTYPE(SOCKET)pThis->mhSocket,
		(struct sockaddr*)&addr, &addrsize) != SOCKET_ERROR)
	{
		WSSocketAddrToMutStr(&addr, s);
		return TRUE;
	}
	else
	{
		WSLogError("Could not get peer name for get IP of user");
		return FALSE;
	}
}
wsbool WSSocket_GetLocalIPAddress(const WSSOCKET* pThis, WSMUTSTR* s)
{
	WSSOCKADDR addr;
	WSsocklen addrsize = sizeof(addr);
	if (getsockname(PTRCASTTYPE(SOCKET)pThis->mhSocket,
		(struct sockaddr*)&addr, &addrsize) != SOCKET_ERROR)
	{
		WSSocketAddrToMutStr(&addr, s);
		return TRUE;
	}
	else
	{
		WSLogError("Could not get peer name for get IP of user");
		return FALSE;
	}
}
wsbool WSSocket_GetLocalHostname(const WSSOCKET* pThis, WSMUTSTR* s)
{
	WSSOCKADDR addr;
	WSsocklen addrsize = sizeof(addr);
	if (getsockname(PTRCASTTYPE(SOCKET)pThis->mhSocket,
		(struct sockaddr*)&addr, &addrsize) != SOCKET_ERROR)
	{
		struct hostent* host = gethostbyaddr((INetArg)&addr.ipv4addr,
			sizeof(addr.ipv4addr), PF_INET);
		if (host)
		{
			WSMutStr_AssignCString(s, host->h_name);
			return TRUE;
		}
		else
		{
			WSLogError("Could not get host name for GetHostName");
			return FALSE;
		}
	}
	else
	{
		WSLogError("Could not get peer name for GetHostName");
		return FALSE;
	}
}
/*
	GetDNSServer() is fairly involved for Windows NT versions before 2K
	and DOS versions before 98 as we have to poke around in the registry.
*/
wsbool WSSocketGetDNSServer(WSMUTSTR* dns)
{/* Python Cookbook, Java */
#if defined(_WIN32)
	/*
		Windows: First try to get the info from GetNetworkParams() which works
		on newer Windows versions, but if not we have to do a lot of involved
		registry  key reading with lots of fallbacks for various versions.
	*/
	typedef struct {
		char String[4 * 4];
	} IP_ADDRESS_STRING, IP_MASK_STRING;/**PIP_ADDRESS_STRING,*PIP_MASK_STRING;*/
	typedef struct _IP_ADDR_STRING {
		struct _IP_ADDR_STRING *Next;
		IP_ADDRESS_STRING   IpAddress;
		IP_MASK_STRING   IpMask;
		DWORD      Context;
	} IP_ADDR_STRING, *PIP_ADDR_STRING;
	#define MAX_DOMAIN_NAME_LEN 128
	#define MAX_HOSTNAME_LEN 128
	#define MAX_SCOPE_ID_LEN 256
	typedef struct {
		char   HostName[MAX_HOSTNAME_LEN + 4];
		char   DomainName[MAX_DOMAIN_NAME_LEN + 4];
		PIP_ADDR_STRING CurrentDnsServer;
		IP_ADDR_STRING  DnsServerList;
		UINT   NodeType;
		char   ScopeId[MAX_SCOPE_ID_LEN + 4];
		UINT   EnableRouting;
		UINT   EnableProxy;
		UINT   EnableDns;
	} FIXED_INFO, *PFIXED_INFO;
	typedef DWORD(WINAPI* GETNETWORKPARAMSPROC)(PFIXED_INFO pFixedInfo, PULONG pOutBufLen);
	typedef LONG (WINAPI* REGQUERYVALUEEXAPROC)(HKEY hKey, LPCSTR lpValueName,
		LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	typedef LONG (WINAPI* REGOPENKEYAPROC)(HKEY hKey, LPCSTR lpSubKey,  PHKEY phkResult);
	typedef LONG (WINAPI* REGENUMKEYAPROC)(HKEY hKey, DWORD dwIndex, LPSTR lpName, DWORD cchName);
	typedef LONG (WINAPI* REGCLOSEKEYPROC)(HKEY hKey);

	WSDLL advapi32;
	WSDLL iph;
	REGQUERYVALUEEXAPROC RegQueryValueExAProc;
	REGENUMKEYAPROC RegEnumKeyAProc;
	REGOPENKEYAPROC RegOpenKeyAProc;
	REGCLOSEKEYPROC RegCloseKeyProc;

	HKEY hSubKey, hLinkageKey;
	HKEY hKey;
	DWORD dwError;
	DWORD dwDnsSize;
	char* enumbuffer;
	char* endofenumbuffer;
	DWORD dwIndex;
	DWORD endofenumbuffersize;

	WSDLL_CreateIphlpapi(&iph);
	if(WSDLL_IsLoaded(&iph))
	{
		GETNETWORKPARAMSPROC lpGetNetworkParams =
			(GETNETWORKPARAMSPROC)WSDLL_GetProc(&iph, "GetNetworkParams");
		FIXED_INFO* FixedInfo;
		ULONG dwBufferSize;
		DWORD dwRet;
		IP_ADDR_STRING* pIPAddr;
		struct in_addr itAddr;

		/* GetNetworkParams - 2K/98+ */
		if(lpGetNetworkParams)
		{
			FixedInfo = (FIXED_INFO*)WSALLOC(BYTE, sizeof(FIXED_INFO));
			dwBufferSize = sizeof(FIXED_INFO);
			dwRet = (*lpGetNetworkParams)(FixedInfo, &dwBufferSize);
			if(dwBufferSize == ERROR_BUFFER_OVERFLOW)
			{
				WSFREE((BYTE*)FixedInfo);
				FixedInfo = (FIXED_INFO*)WSALLOC(BYTE, dwBufferSize);
				dwRet = (*lpGetNetworkParams)(FixedInfo, &dwBufferSize);
			}
		}
		else
		{
			dwRet = ERROR_NOT_SUPPORTED;
			FixedInfo = NULL;
		}

		if(dwRet == ERROR_SUCCESS)
		{
			pIPAddr = &(FixedInfo->DnsServerList);
			do
			{
				/*inet_aton (pIPAddr->IpAddress.String, [&]tmp_addr)*/
				itAddr.s_addr = inet_addr(pIPAddr->IpAddress.String);
				/*return (tmp_addr.s_addr == INADDR_NONE) ? 0 : 1;*/
				/*end inet_aton...*/
				if(itAddr.s_addr != INADDR_NONE
					&& itAddr.s_addr != INADDR_ANY
					&& itAddr.s_addr != INADDR_LOOPBACK)
				{
					WSMutStr_AssignCString(dns, pIPAddr->IpAddress.String);
					WSFREE((BYTE*)FixedInfo);
					WSDLL_DestroyIphlpapi(&iph);
					return TRUE;
				}
				/*		printf("DNS Addr:%s\n", pIPAddr->IpAddress.String);*/
				pIPAddr = pIPAddr->Next;
			} while(pIPAddr);
		}
		WSFREE((BYTE*)FixedInfo);
		WSDLL_DestroyIphlpapi(&iph);
	}

	WSDLL_CreateAdvapi32(&advapi32);

	RegQueryValueExAProc = (REGQUERYVALUEEXAPROC)
		WSDLL_GetProc(&advapi32, "RegQueryValueExA");
	RegEnumKeyAProc = (REGENUMKEYAPROC)
		WSDLL_GetProc(&advapi32, "RegEnumKeyA");
	RegOpenKeyAProc = (REGOPENKEYAPROC)
		WSDLL_GetProc(&advapi32, "RegOpenKeyA");
	RegCloseKeyProc = (REGCLOSEKEYPROC)
		WSDLL_GetProc(&advapi32, "RegCloseKey");

	/* Registry method; Python cookbook is a good one */
	/*NameServer  - 95, DhcpNameServer then NameServer  - NT*/
	/**/
	/*DhcpNameServer DhcpIPAddress EnableDHCP==1*/
	/*Transient has dailup info*/
	WSMutStr_BeforeWrite(dns, 1024); /* Windows uses this value as max */
	if(!WSMutStr_IsCreated(dns))
		return FALSE;

	dwDnsSize = (DWORD)WSMutStr_GetCapacity(dns);
	if ((*RegOpenKeyAProc)(HKEY_LOCAL_MACHINE,
			"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters", &hKey)
		!= ERROR_SUCCESS)
	{ /* Failed, so Non-NT */
		(*RegCloseKeyProc)(hKey);
		if ((*RegOpenKeyAProc)(HKEY_LOCAL_MACHINE,
				"SYSTEM\\CurrentControlSet\\Services\\VxD\\MSTCP",
				&hKey)  != ERROR_SUCCESS)
		{
			(*RegCloseKeyProc)(hKey); /* WinSock not installed properly most likely */
			WSMutStr_AfterWriteAndNullTerminate(dns, 0);
			WSDLL_DestroyAdvapi32(&advapi32);
			return FALSE;
		}

		dwError = (*RegQueryValueExAProc)(hKey, "Transient", NULL, NULL,
				(LPBYTE)WSMutStr_GetCString(dns), &dwDnsSize);
		if((dwError == ERROR_SUCCESS) && dwDnsSize > 1)
		{
			WSMutStr_AfterWrite(dns, dwDnsSize - 1);
			WSDLL_DestroyAdvapi32(&advapi32);
			return TRUE;
		}

		dwError = (*RegQueryValueExAProc)(hKey, "NameServer", NULL, NULL,
				(LPBYTE)WSMutStr_GetCString(dns), &dwDnsSize);
		if ((dwError == ERROR_SUCCESS) && dwDnsSize > 1)
		{
			WSMutStr_AfterWrite(dns, dwDnsSize - 1);
			WSDLL_DestroyAdvapi32(&advapi32);
			return TRUE;
		}
		WSMutStr_AfterWriteAndNullTerminate(dns, 0);
		WSDLL_DestroyAdvapi32(&advapi32);
		return FALSE; /* Won't be anywhere else on 95/98/ME */
			/*maybe SYSTEM.INI -> DNS.DNSServers...*/
	}
	(*RegCloseKeyProc)(hKey);

	enumbuffer = (char*) WSMutStr_GetCString(dns); /* allocated earlier */


	if ((*RegOpenKeyAProc)(HKEY_LOCAL_MACHINE,
			"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces",
			&hKey)
		!= ERROR_SUCCESS)
	{ /* DNSRegisteredAdapters is Win2K/NT */
		if ((*RegOpenKeyAProc)(HKEY_LOCAL_MACHINE,
				"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\DNSRegisteredAdapters",
				&hKey)
			!= ERROR_SUCCESS)
		{
			WSMutStr_AfterWriteAndNullTerminate(dns, 0);
			WSDLL_DestroyAdvapi32(&advapi32);
			return FALSE;
		}

		lstrcpyA(enumbuffer, "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\DNSRegisteredAdapters");
		enumbuffer[strcountof("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\DNSRegisteredAdapters")] = '\\';
		endofenumbuffer = enumbuffer + strcountof("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\DNSRegisteredAdapters\\");
		endofenumbuffersize = (DWORD)(sizeof(enumbuffer) - (endofenumbuffer - enumbuffer));
		for(dwIndex = 0;
			(*RegEnumKeyAProc)(hKey, dwIndex, endofenumbuffer, endofenumbuffersize) == ERROR_SUCCESS;
			++dwIndex)
		{
			if((*RegOpenKeyAProc)(HKEY_LOCAL_MACHINE, enumbuffer, &hSubKey) == ERROR_SUCCESS)
			{
				dwError = (*RegQueryValueExAProc)(hSubKey, "DNSServerAddresses", NULL, NULL,
					(LPBYTE)WSMutStr_GetCString(dns), &dwDnsSize);
				if((dwError == ERROR_SUCCESS) && dwDnsSize > 1)
				{ /* it's a chain of IPv4 strings seperated by a null char \0 */
					/*DNSServerAddressCount gives the count, though it isn't really needed*/
					(*RegCloseKeyProc)(hSubKey);
					(*RegCloseKeyProc)(hKey);
					WSMutStr_AfterWrite(dns, dwDnsSize - 1);
					WSDLL_DestroyAdvapi32(&advapi32);
					return TRUE;
				}
				dwDnsSize = (DWORD)WSMutStr_GetCapacity(dns);
				(*RegCloseKeyProc)(hSubKey);
			}
		}

		(*RegCloseKeyProc)(hKey);
		WSMutStr_AfterWriteAndNullTerminate(dns, 0);
		WSDLL_DestroyAdvapi32(&advapi32);
		return FALSE;
	}

	if ((*RegOpenKeyAProc)(HKEY_LOCAL_MACHINE,
			"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Linkage",
			&hLinkageKey) == ERROR_SUCCESS)
	{
		DWORD dwBindSize;
		dwError = (*RegQueryValueExAProc)(hLinkageKey, "Bind", NULL, NULL,
				(LPBYTE)WSMutStr_GetCString(dns), &dwBindSize);
		(*RegCloseKeyProc)(hLinkageKey);

		if(dwError == ERROR_SUCCESS && dwBindSize > 1
			&& WSMutStr_GetLength(dns) == strcountof("\\Device\\")
			&& WSMemoryCompare(WSMutStr_GetCString(dns), "\\Device\\",
							   strcountof("\\Device\\") * sizeof(char)) == 0)
		{/*null char seperated list of "\Device\{X-X-X-X}"*/
			WSMUTSTR interfaces;
			WSMutStr_Erase(dns, dwBindSize);
			WSMutStr_Create(&interfaces);
			WSMutStr_AppendCStringWithLength(&interfaces,
				WSMutStr_GetCString(dns), WSMutStr_GetLength(dns));
			if(WSMutStr_IsCreated(&interfaces))
			{
				const char* currentinterface;
				for(currentinterface = WSMutStr_GetCString(&interfaces);
					(DWORD)(currentinterface - WSMutStr_GetCString(&interfaces)) < dwBindSize;
					currentinterface += CStrLength(currentinterface) + 1)
				{
					const char* szDeviceSerial;
					WSMUTSTR sSubKey;
					WSMutStr_Create(&sSubKey);
					WSMutStr_AppendCStringWithLength(&sSubKey,
						"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\",
						strcountof("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\"));
					szDeviceSerial = currentinterface + strcountof("\\Device\\");
					if(WSMutStr_IsCreated(&sSubKey))
						WSMutStr_AppendCString(&sSubKey, szDeviceSerial);
					if(WSMutStr_IsCreated(&sSubKey))
					{
						/* Fun fact: in old source of a "certain" operating system they named the variable */
						/* that pointed to the part after "Dhcp" 'p' and I had trouble figuring it out */
						if((*RegOpenKeyAProc)(HKEY_LOCAL_MACHINE, WSMutStr_GetCString(&sSubKey), &hSubKey) == ERROR_SUCCESS)
						{
							wsbool success = ((*RegQueryValueExAProc)(hSubKey, "Transient", NULL, NULL,
									(LPBYTE)WSMutStr_GetCString(dns), &dwDnsSize) == ERROR_SUCCESS && dwDnsSize > 1);
							if(!success)
							{
								dwDnsSize = (DWORD)WSMutStr_GetCapacity(dns);
								success = ((*RegQueryValueExAProc)(hSubKey, "NameServer", NULL, NULL,
										(LPBYTE)WSMutStr_GetCString(dns), &dwDnsSize) == ERROR_SUCCESS && dwDnsSize > 1);
								if(!success)
								{
									dwDnsSize = (DWORD)WSMutStr_GetCapacity(dns);
									success = ((*RegQueryValueExAProc)(hSubKey, "DhcpNameServer", NULL, NULL,
											(LPBYTE)WSMutStr_GetCString(dns), &dwDnsSize) == ERROR_SUCCESS && dwDnsSize > 1);
								}
							}
							if(success)
							{
								(*RegCloseKeyProc)(hSubKey);
								(*RegCloseKeyProc)(hKey);
								WSDLL_DestroyAdvapi32(&advapi32);
								WSMutStr_AfterWrite(dns, dwDnsSize - 1);
								return TRUE;
							}
							dwDnsSize = (DWORD)WSMutStr_GetCapacity(dns);
							(*RegCloseKeyProc)(hSubKey);
						}
						WSMutStr_Destroy(&sSubKey);
					}
				} /* end going through binded interfaces */
				WSMutStr_Destroy(&interfaces);
			} /* end if interfaces created */
		} /* end if bind key checks out */
	} /* end if Linkage key exists */

	lstrcpyA(enumbuffer, "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces");
	enumbuffer[strcountof("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces")] = '\\';
	endofenumbuffer = enumbuffer + strcountof("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\");



	endofenumbuffersize = (DWORD) (sizeof(enumbuffer) - (endofenumbuffer - enumbuffer));


	for(dwIndex = 0;
		(*RegEnumKeyAProc)(hKey, dwIndex, endofenumbuffer, endofenumbuffersize) == ERROR_SUCCESS;
		++dwIndex)
	{
		if((*RegOpenKeyAProc)(HKEY_LOCAL_MACHINE, enumbuffer, &hSubKey) == ERROR_SUCCESS)
		{
			wsbool success = ((*RegQueryValueExAProc)(hSubKey, "Transient", NULL, NULL,
					(LPBYTE)WSMutStr_GetCString(dns), &dwDnsSize) == ERROR_SUCCESS && dwDnsSize > 1);
			if(!success)
			{
				dwDnsSize = (DWORD)WSMutStr_GetCapacity(dns);
				success = ((*RegQueryValueExAProc)(hSubKey, "NameServer", NULL, NULL,
						(LPBYTE)WSMutStr_GetCString(dns), &dwDnsSize) == ERROR_SUCCESS && dwDnsSize > 1);
				if(!success)
				{
					dwDnsSize = (DWORD)WSMutStr_GetCapacity(dns);
					success = ((*RegQueryValueExAProc)(hSubKey, "DhcpNameServer", NULL, NULL,
							(LPBYTE)WSMutStr_GetCString(dns), &dwDnsSize) == ERROR_SUCCESS && dwDnsSize > 1);
				}
			}
			if(success)
			{
				(*RegCloseKeyProc)(hSubKey);
				(*RegCloseKeyProc)(hKey);
				WSDLL_DestroyAdvapi32(&advapi32);
				WSMutStr_AfterWrite(dns, dwDnsSize - 1);
				return TRUE;
			}
			dwDnsSize = (DWORD)WSMutStr_GetCapacity(dns);
			(*RegCloseKeyProc)(hSubKey);
		}
	}
	(*RegCloseKeyProc)(hKey);
	WSMutStr_AfterWriteAndNullTerminate(dns, 0);
	WSDLL_DestroyAdvapi32(&advapi32);
	return FALSE;
#elif (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__) \
			|| defined(__linux__)) /*!_WIN32*/
	/*
		Unix/Linux: Grab the DNS server from the /etc/resolv.conf file.
	*/
	WSMUTSTR buff;
	WSFILE resolvConf;
	int found = 0;
	struct in_addr loopbackAddr;
	struct in_addr localAddr;
	struct in_addr itAddr;
	WSMutStr_CreateWithSize(&buff, 512);
	WSMutStr_BeforeWrite(&buff, 512);
	if(!WSMutStr_IsCreated(&buff))
		return FALSE;
	inet_aton("127.0.0.1", &loopbackAddr);
	if (gethostname((char*)WSMutStr_GetCString(&buff),
							WSMutStr_GetCapacity(&buff)) == 0)
	{
		struct addrinfo hints;
		WSMemoryFill(&hints, 0, sizeof(hints));
		hints.ai_flags = AI_NUMERICHOST;
		hints.ai_family = AF_INET;
		struct addrinfo* ai;
		if (getaddrinfo(WSMutStr_GetCString(&buff), NULL, &hints, &ai) == 0)
		{
			localAddr = *(struct in_addr*)ai->ai_addr->sa_data;
			freeaddrinfo(ai);
		}
		else
			localAddr.s_addr = 0;
		if (localAddr.s_addr == 0)
			localAddr.s_addr = loopbackAddr.s_addr;
	}
	else
		localAddr.s_addr = 0;

	/* /var/lib/dhcp3/dhclient.leases has the dhcp info if needed... */
	if(WSFile_Create(&resolvConf, "/etc/resolv.conf", FILE_READ))
	{
		size_t numRead;
		while ((numRead = WSFile_Read(&resolvConf,
							(void*)WSMutStr_GetCString(&buff),
							WSMutStr_GetCapacity(&buff) - 1)) != WSMAXSIZE)
		{
			WSMutStr_Erase(&buff, numRead);
			size_t nameserverPos =
				CStrFind(WSMutStr_GetCString(&buff),
					WSMutStr_GetLength(&buff),
					"nameserver", strcountof("nameserver"), 0);
			const char* ipv4loc = WSMutStr_GetCString(&buff)
				+ nameserverPos + strcountof("nameserver");
			const char* ipv4end;
			while(*ipv4loc == ' ' || *ipv4loc == '\t')
				++ipv4loc;
			ipv4end = ipv4loc;
			if(URIParseIPv4address(&ipv4end))
			{
				if (!inet_aton((INetArg)ipv4loc, &itAddr))
					continue;
				if (itAddr.s_addr == loopbackAddr.s_addr)
					itAddr = localAddr;
				if (found == 0)
				{
					WSMutStr_BeforeWrite(dns, 46); /* INET6_ADDRSTRLEN == 46 in POSIX */
					if(WSMutStr_IsCreated(dns))
					{
						if (inet_ntop(AF_INET, &itAddr,
							(char*)WSMutStr_GetCString(dns), 46) != NULL)
						{
							WSMutStr_AfterWrite(dns,
								CStrLength(WSMutStr_GetCString(dns)));
							WSMutStr_Destroy(&buff);
							return TRUE;
						}
						else
							WSMutStr_AfterWriteAndNullTerminate(dns, 0);
					}
				} /* got it? */
			} /* good ipv4 address? */
		} /* while reading */
		WSFile_Destroy(&resolvConf);
	} /* if "/etc/resolv.conf" exists */
	WSMutStr_Destroy(&buff);
	return FALSE;
#else
	return FALSE;
#endif /*_WIN32*/
}
#endif /* HAVE_MUTSTR */
#if !defined(_WIN32) && !defined(WSKQUEUEOPT) && !defined(WSAMIGA)
typedef void (*sighandlercompat_t) (int);
typedef struct WSSIGIGNORE
{
	sighandlercompat_t mPrevProc; /*sighandler_t is non-standard*/
	int mSig;
} WSSIGIGNORE;
/* You are sent a SIGPIPE if a socket closes before READ/WRITE and then you do */
/* another READ/WRITE afterwards. signal(x, SIG_IGN) overrides this behaviour */
/* to do nothing. */
/* Another less portable solution is to pass the MSG_NOSIGNAL to A4 of send(). */
void WSSigIgnore_Create(WSSIGIGNORE* pThis, int sig)
{
	pThis->mPrevProc = (signal(sig, SIG_IGN));
	pThis->mSig = (sig);
}
void WSSigIgnore_Destroy(WSSIGIGNORE* pThis)
{	signal(pThis->mSig, pThis->mPrevProc); }
#else
typedef struct WSSIGIGNORE { void* h; } WSSIGIGNORE;
void WSSigIgnore_Create(WSSIGIGNORE* pThis, int sig)
{ (void)(pThis); (void)(sig); }
void WSSigIgnore_Destroy(WSSIGIGNORE* pThis) { (void)(pThis); }
#endif

#define RECVBLOCKSIZEVALUE 8192
#define RECVBLOCKSIZE(x) (x > RECVBLOCKSIZEVALUE ? RECVBLOCKSIZEVALUE : x)

size_t WSSocket_Read_(void* mhSocket,
	void* bufin, size_t lenin, wsbool* inblock, int flags)
{
	char* buf = (char*)bufin;
	wsbool block = inblock ? *inblock : WSSOCKET_BLOCKING;
	size_t dlen = lenin;
	size_t slen;
	size_t pos = 0;
	for (;;)
	{
		slen = recv(PTRCASTTYPE(SOCKET)mhSocket,
					  buf, (int)RECVBLOCKSIZE(dlen), flags);
		if (slen == 0)
			break; /* connection closed */
		else if (slen != (size_t)SOCKET_ERROR)
			return slen;
		else if (!block
			|| WSAGetLastError() != WSAEWOULDBLOCK)
		{
			if(inblock)
				*inblock = WSAGetLastError() == WSAEWOULDBLOCK;
			return pos == 0 ? (size_t)SOCKET_ERROR : pos;
		}
		WSThreadYield();
	}
	return (size_t)SOCKET_ERROR;
}

size_t WSSocket_Read(const WSSOCKET* pThis,
	void* data, size_t num, wsbool* inblock)
{
	return WSSocket_Read_(pThis->mhSocket, data, num, inblock, 0);
}
size_t WSSocket_Peek(const WSSOCKET* pThis,
	void* data, size_t num, wsbool* inblock)
{
	return WSSocket_Read_(pThis->mhSocket, data, num, inblock, MSG_PEEK);
}
size_t WSSocket_Write(const WSSOCKET* pThis, const void* dbufin, size_t dlenin, wsbool* inblock)
{
	const char* buf = (const char*)dbufin;
	wsbool block = inblock ? *inblock : WSSOCKET_BLOCKING;
	size_t dlen = dlenin;
	size_t slen;
	size_t pos = 0;
#if !defined(_WIN32) && !defined(WSAMIGA)
	WSSIGIGNORE si;
	WSSigIgnore_Create(&si, SIGPIPE);
#endif
	for (; ;)
	{ /* use MSG_NOSIGNAL for A4 of send() when debugging when GDB, it ignores signal() */
#define SENDBLOCKSIZEVALUE 65535
#define SENDBLOCKSIZE(x) (x > SENDBLOCKSIZEVALUE ? SENDBLOCKSIZEVALUE : x)
		slen = send(PTRCASTTYPE(SOCKET)pThis->mhSocket,
					  (SendArg)buf, (int)SENDBLOCKSIZE(dlen), 0);
		if (slen != (size_t)SOCKET_ERROR)
		{
			pos += slen;
			if (pos >= dlenin)
			{
				WSASSERT(pos == dlenin);
				return pos;
			}
			buf += slen;
			dlen -= slen;
			continue;
		}
		else if (!block
			|| WSAGetLastError() != WSAEWOULDBLOCK)
		{
			if(inblock)
				*inblock = WSAGetLastError() == WSAEWOULDBLOCK;
			break;
		}
		WSThreadYield();
	}
#if !defined(_WIN32) && !defined(WSAMIGA)
	WSSigIgnore_Destroy(&si);
#endif
	return pos == 0 ? (size_t)SOCKET_ERROR : pos;
}
int WSSocket_Error(const WSSOCKET* pThis)
{
	(void)(pThis);
	return WSAGetLastError();
}

/*=============================================================================*/
/* Win32 Options: AcceptEx/ConnectEx/DisconnectEx */
/*=============================================================================*/
#if defined(_WIN32)
#if defined(WSACCEPTEXOPT)
/* WSAIoctl requires ws2_32.dll (aka ws2_32.lib) - the problem */
/* is that it is not on generic Windows 95 installs. One can */
/* install it with the winsock2 update for win95 (W95ws2setup.exe) */
/* WSIOCPOPT) Obtains AcceptEx availability through */
/* GetQueuedCompletionStatus with the server socket and the */
/* OVERLAPPED structure for the individual client socket */
/* returned. */
/* WSAEventSelect) Obtains AcceptEx availability through */
/* the hEvent member of the OVERLAPPED structure passed */
/* to AcceptEx. */
/* WSAAsyncSelect) Has no way of obtaining AcceptEx availability */
/* select) Has no way of obtaining AcceptEx availability */

wsbool WSSocket_AcceptEx(WSSOCKET* pThis, const WSSOCKET* pListener,
	void* data, uint32_t* firstreadlen, uint32_t sockaddrlen,
	OVERLAPPED* lpOverlapped, WSSOCKETLIBRARY* socklib)
{
	wsbool res = (*socklib->mAcceptEx)(
		WSSocketSSL_GetHandle(pListener),
		WSSocketSSL_GetHandle(pThis),
		data, *firstreadlen,
		sockaddrlen, sockaddrlen,
		firstreadlen,
		lpOverlapped);
	return res || GetLastError() == ERROR_IO_PENDING;
}
void WSSocketFinishAcceptEx(WSSOCKET* pThis, const WSSOCKET* pListener,
	void* data, uint32_t firstreadlen,
	uint32_t sockaddrlen, WSSOCKADDR** localaddr,
	WSSOCKADDR** remoteaddr, WSSOCKETLIBRARY* socklib)
{
	int addrlengths;
	 /* Less compatible */
	/*wsbool res =*/
#if 1
		setsockopt((SOCKET)WSSocketSSL_GetHandle(pThis),
		SOL_SOCKET, 0x700B, /*SO_UPDATE_ACCEPT_CONTEXT*/
		(LPSTR)&WSSocketSSL_GetHandle(pListener),
		sizeof(SOCKET))
			;/*!= SOCKET_ERROR;*/
#endif

	(*socklib->mGetAcceptExSockaddrs)(
		data, firstreadlen, sockaddrlen, sockaddrlen,
		(struct sockaddr**)localaddr, &addrlengths,
		(struct sockaddr**)remoteaddr, &addrlengths);
}

#endif
#if defined(WSCONNECTEXOPT)
wsbool WSSocket_CreateClientEx(WSSOCKET* pThis,
	const char* szHostname, size_t port,
	void* data, size_t datalen,
	OVERLAPPED* lpOverlapped, WSSOCKETLIBRARY* socklib)
{
	uint32_t dwBytes;
	WSAIOCTLPROC lpWSAIoctl;
	CONNECTEXPROC lpConnectEx;
	GUID GuidConnectEx =
	{ 0x25a207b9,0xddf3,0x4660,{0x8e,0xe9,0x76,0xe5,0x8c,0x74,0x06,0x3e} };
	struct sockaddr_in inaddr;
	(void)(socklib);
	WSASSERT(!WSSocket_IsCreated(pThis));
	/*Create client: socket()->connect(szHostname, port)*/
	/*SOCK_DGRAM, IPPROTO_UDP - datagram using getprotobyname*/
	pThis->mhSocket = PTRCASTTYPE(WSSOCKETTYPE)
		socket(AF_INET, SOCK_STREAM, 0);
	if (pThis->mhSocket == (WSSOCKETTYPE)INVALID_SOCKET)
		return FALSE;
	if (!WSSocket_SetNonBlocking(pThis, TRUE))
	{
		WSLogDebug("Could not set accepted socket to blocking");
	}

	WSMemoryFill((char*)&inaddr, 0, sizeof(inaddr));
	/*Host*/
	struct hostent* pHostEntry = gethostbyname(szHostname);
	if (pHostEntry)
		WSMemoryCopy(&inaddr.sin_addr,
			pHostEntry->h_addr, pHostEntry->h_length);
	else
	{
		inaddr.sin_addr.s_addr = inet_addr(szHostname);
		if (inaddr.sin_addr.s_addr == INADDR_NONE)
		{
			WSSocket_Destroy(pThis);
			return FALSE;
		}
	}
	/*Port (don't prepend  to this, linux has funky macro)*/
	inaddr.sin_port = htons(((unsigned short)port));
	lpWSAIoctl =
		EXPLICITPROCCAST(WSAIOCTLPROC) WSDLL_GetProc(
			WSSocketLibrary_GetWs2_32DLL(socklib), "WSAIoctl");
	if (inaddr.sin_port == 0 || !lpWSAIoctl
		|| (*lpWSAIoctl)(WSSocket_GetHandle(pThis),
			(0x80000000 | 0x40000000) | 0x08000000 | 6,
			&GuidConnectEx, sizeof(GuidConnectEx),
			&lpConnectEx, sizeof(lpConnectEx),
			&dwBytes, NULL, NULL) != 0)
	{
		WSSocket_Destroy(pThis);
		return FALSE;
	}
	/*Family*/
	inaddr.sin_family = AF_INET;

	if((*lpConnectEx)(WSSocket_GetHandle(pThis),
		(struct sockaddr*)&inaddr, sizeof(inaddr), data,
		WSUPPERCLAMPTYPE(datalen, uint32_t),
		&dwBytes, lpOverlapped) != TRUE && GetLastError() != ERROR_IO_PENDING)
	{
		WSSocket_Destroy(pThis);
		return FALSE;
	}

	return TRUE;
}
#endif
#if defined(WSDISCONNECTEXOPT)
wsbool WSSocket_DisonnectEx(WSSOCKET* pThis,
	OVERLAPPED* lpOverlapped, WSSOCKETLIBRARY* socklib)
{
	DISCONNECTEXPROC lpDisconnectEx;
	uint32_t dwBytes;
	GUID GuidDisconnectEx =
	{ 0x7fda2e11,0x8630,0x436f,{0xa0,0x31,0xf5,0x36,0xa6,0xee,0xc1,0x57} };
	if ((*socklib->mWSAIoctl)(WSSocket_GetHandle(pThis),
		(0x80000000 | 0x40000000) | 0x08000000 | 6,
		&GuidDisconnectEx, sizeof(GuidDisconnectEx),
		&lpDisconnectEx, sizeof(lpDisconnectEx),
		&dwBytes, NULL, NULL) != 0)
		return FALSE;

	return (*lpDisconnectEx)(
		WSSocketSSL_GetHandle(pThis),
		lpOverlapped, 0x02, /*TF_REUSE_SOCKET*/
		0) == TRUE || GetLastError() == ERROR_IO_PENDING;
}
#endif
#endif

/*=============================================================================*/
/**/
/*						SocketNotifier*/
/**/
/*	Synchronous = socket()->bind()->listen()->accept() = client socket for r/w.*/
/*	Asynchronous = Above but more involved. WinSock 2/AcceptEx actually has*/
/*				   a seperate socket for accepting which does the dirty work.*/
/*=============================================================================*/


/*#############################################################################
						WSSOCKETNOTIFIERPOST/WSSOCKETNOTIFIERINFO
#############################################################################*/

typedef WSSOCKETNOTIFIERINFO WSSOCKETNOTIFIERINFO;
WSSOCKETNOTIFIERINFO* WSSocketNotifierInfo_FindSocket(
	WSSOCKETNOTIFIERINFO* root, void* hSocket)
{
	WSSOCKETNOTIFIERINFO* cur = root;
	if(root)
	{
		do
		{
			WSASSERT(cur != NULL);
			if(WSSocketSSL_GetHandle(&cur->mSocket) == hSocket)
				return cur;
			cur = (WSSOCKETNOTIFIERINFO*)cur->mNext;
		} while(cur != root);
	}
	return NULL;
}
static void AssureEntryIsGone(WSSOCKETNOTIFIERINFO* mSockets,
	WSSOCKETNOTIFIERINFO* entry)
{
	WSSOCKETNOTIFIERINFO* seeking = mSockets;
	if (seeking)
	{
		while (seeking->mNext != mSockets)
		{
			if (seeking == entry)
				break;
			seeking = seeking->mNext;
		}
	}
	WSASSERT(seeking != entry);
}
/*=============================================================================*/
/**/
/*						SocketNotifier*/
/**/
/*	The Windows side is a bit of a labyrinth due to the different backends.*/
/*	IOCP (NT 3.5+) will take care of clients always, and listeners if AcceptEx*/
/*  is enabled. WSAEventSelect (Winsock 2+) is next in line and handles */
/*	anything. If WSAEventSelect fails it falls back to either WSAAsyncSelect*/
/*	or raw select() depending on compilation settings.*/
/*=============================================================================*/

void WSSocketNotifierPost_Destroy(WSSOCKETNOTIFIERPOST* post)
{
	if(post->mShouldFreeBuffer)
		WSFREE(post->mBuffer);
	WSFREE(post);
}
void WSSocketNotifierInfo_Destroy(WSSOCKETNOTIFIERINFO* entry)
{
	if(entry->mShouldDestroySocket)
		WSSocketSSL_Destruct(&entry->mSocket);
	WSFREE(entry);
}

/*===========================================================================*/
/*==============================Info List Handling===========================*/
/*===========================================================================*/

void WSSocketNotifierRemoveEntry(
	WSSOCKETNOTIFIERINFO** top, WSSOCKETNOTIFIERINFO* entry)
{
	DList_Remove(top, entry, mNext, mPrev);
	AssureEntryIsGone(*top, entry);
}
void WSSocketNotifierUpdateEntry(
	WSSOCKETNOTIFIERINFO** top, WSSOCKETNOTIFIERINFO* entry)
{
	AssureEntryIsGone(*top, entry);
	entry->mLastActiveTime = WSTimeGetLocalValue();
	DList_Update(top, entry, mNext, mPrev);
}

void WSSocketNotifier_ReshuffleEntries(WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO* entry)
{
#if defined(_WIN32)
	#if defined(WSIOCPOPT)
		if(!pNotifier->mhIOCP)
	#endif
		{
		#if defined(WSEVENTSELECTOPT)
			if(pNotifier->mEvents)
			{
				entry->mPrevForEvent->mNextForEvent = entry->mNextForEvent;
				entry->mNextForEvent->mPrevForEvent = entry->mPrevForEvent;
				if(pNotifier->mNumSockets < pNotifier->mNumEvents)
					pNotifier->mSocketsForEvents[entry->mEventIndex] = NULL;
				--pNotifier->mNumSockets;
			}
			else
		#endif
			{
		#if defined(WSASYNCSELECTOPT)
		/*		WSAAsyncSelect((SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),*/
		/*			 this->mWnd, 0, 0);*/
		#endif
			}
		}
#endif /*_WIN32*/
#if defined(WSUSERAWSELECT)
	if(pNotifier->mFDSets)
	{
		FD_CLR((SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),
				&pNotifier->mFDSets->read);
		FD_CLR((SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),
				&pNotifier->mFDSets->write);
		FD_CLR((SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),
				&pNotifier->mFDSets->lost);
		if((SOCKET)WSSocketSSL_GetHandle(&entry->mSocket)
				== (SOCKET)pNotifier->mMaxFD)
		{
			WSSOCKETNOTIFIERINFO* qe = pNotifier->mSockets;
			pNotifier->mMaxFD = 0;
			if(qe)
			{
				do
				{
					if(qe != entry
						&& (SOCKET)WSSocketSSL_GetHandle(&entry->mSocket)
							> (SOCKET)pNotifier->mMaxFD)
						pNotifier->mMaxFD = (void*)
							WSSocketSSL_GetHandle(&entry->mSocket);
					qe = qe->mNext;
				} while(qe != pNotifier->mSockets);
			}
		}
	}
#endif
}

void WSSocketNotifier_UntrackInfo(WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO* entry)
{
#if defined(_WIN32)
#if defined(WSEVENTSELECTOPT)
	if(pNotifier->mEvents)
	{
		(*pNotifier->mSocketLibRef->mWSAEventSelect)(
				WSSocketSSL_GetHandle(&entry->mSocket),
				pNotifier->mEvents[entry->mEventIndex], 0);
	}
#endif
	#if defined(WSASYNCSELECTOPT)
		#if defined(WSEVENTSELECTOPT)
	else
		#endif
	{
		WSAAsyncSelect(PTRCASTTYPE(SOCKET)
				WSSocketSSL_GetHandle(&entry->mSocket), pNotifier->mWnd,
				WM_USER, 0);
	}
	#endif
#endif
	WSSocketNotifier_ReshuffleEntries(pNotifier, entry);
}

void WSSocketNotifier_DeleteEntry(
	 WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO** top, WSSOCKETNOTIFIERINFO* entry,
	wsbool force)
{
	(void)(top);
	AssureEntryIsGone(pNotifier->mSockets, entry);

	WSSocketNotifier_ReshuffleEntries(pNotifier, entry);

#if defined(_WIN32) && defined(WSIOCPOPT)
	if (pNotifier->mhIOCP && entry->mPending && !force)
	{
		entry->mIsDead = TRUE;
		DList_Add(&pNotifier->mDeadSockets, entry, mNext, mPrev);
	}
	else
#endif
	{
		/* clear up data used for pending IO */
		while(entry->mPending)
		{
			WSSOCKETNOTIFIERPOST* todel = entry->mPending;
			DList_Remove(&entry->mPending, todel, mNext, mPrev);
			WSSocketNotifierPost_Destroy(todel);
		}
		/*WSLogDebug(MutStr("FINALIZING --DELETE-- OF [")*/
		/*			<< (size_t)WSSocketSSL_GetHandle(&entry->mSocket) << "]");*/
		/* delete it finally (if not IOCP with pending) */
		WSSocketNotifierInfo_Destroy(entry);

		/*WSLogDebug(MutStr("DeleteEntry; Thread [") << WSThreadGetCurrentID()*/
	/*	<< "] Current State mSockets [" << (size_t)mSockets << "]");*/
	/*LogEntries(pNotifier->mSockets);*/
	/*if(pNotifier->mSockets)*/
	{
		/*WSLogDebug(MutStr("mSockets->mNext [") << (size_t)mSockets->mNext << "]");*/
	}
	/*if(pThis->mhIOCP && entry->mNumDeadPending)*/
	{
		/*DList_Add(&pNotifier->mDeadSockets, entry, mNext, mPrev);*/
	#if 0/* 61 == FileReplaceCompletionInformation */
		WSDLL ntdll;
		WSDLL_CreateNtdll(&ntdll);
		typedef LONG (NTAPI* NtSetInformationFileProcPtr)(HANDLE hFile,
			ULONG_PTR* statusBlock, void* fileInfo, ULONG fileInfoLength,
			ULONG fileInfoClass);
		ULONG_PTR iosb[2] { 0, 0 };
		void* info[2] = { 0, 0 };
		NtSetInformationFileProcPtr NtSetInformationFileProc =
			(NtSetInformationFileProcPtr)
			WSDLL_GetProc(&ntdll, "NtSetInformationFile");
		LONG status = /* doesn't work; NTSIFP returns NOT_SUCCESS */
		(*NtSetInformationFileProc)(WSSocketSSL_GetHandle(&entry->mSocket), iosb,
			&info, sizeof(info), 61);
		if(status)
			WSLogError("NTINFO");
		status =
			CancelIo(WSSocketSSL_GetHandle(&entry->mSocket));
		WSDLL_DestroyNtdll(&ntdll);
	#endif
	}
	/*WSLogDebug(MutStr("DeleteEntry FIN; Thread [") << WSThreadGetCurrentID()*/
	/*	<< "] Current State mSockets [" << (size_t)mSockets << "]");*/
	}
}

void WSSocketNotifier_AddEntry( WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO** top, WSSOCKETNOTIFIERINFO* entry)
{
	(void)(pNotifier);
	AssureEntryIsGone(*top, entry);
	entry->mLastActiveTime = WSTimeGetLocalValue();
	DList_Add(top, entry, mNext, mPrev);
}

/*===========================================================================*/
/*===========================WSAsyncSelect WndProc===========================*/
/*===========================================================================*/

#if defined(_WIN32) && defined(WSASYNCSELECTOPT)
LRESULT CALLBACK WSSocketNotifierAsyncWndProc(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{/*WndProc for the case where AcceptEx()/WinSock 2 isn't available*/
	/*wParam == SOCKET LOWORD(lParam) == event HIWORD(lParam) == error*/
	if (msg >= WM_USER)
	{
		WSSOCKETNOTIFIER* poller =
			(WSSOCKETNOTIFIER*)(longptr_t)
				GetWindowLongPtr(hWnd, GWLP_USERDATA);
		WSSOCKETNOTIFIERINFO* qe = /* ASyncSEL is limited to 64 */
			WSSocketNotifierInfo_FindSocket(
				(WSSOCKETNOTIFIERINFO*)poller->mSockets, (void*)wParam);
		if(!qe)
		{
			/* This can happen after a socket is shutdown due to */
			/* queued windows messages from queued FD_READs */
			return 0;
		}
		WSASSERT(wParam == (SOCKET)WSSocketSSL_GetHandle(&qe->mSocket));
		if (LOWORD(lParam) == FD_ACCEPT)
			qe->mCanAccept = TRUE;
		else
			#if defined(WSIOCPOPT)
				if (!poller->mhIOCP)
			#endif
		{
			if (LOWORD(lParam) == FD_READ)
				qe->mCanRead = TRUE;
			else if (LOWORD(lParam) == FD_WRITE)
				qe->mCanWrite = TRUE;
			else if (LOWORD(lParam) == FD_CLOSE
				|| (LOWORD(lParam) == FD_CONNECT && HIWORD(lParam)))
				qe->mIsDisconnected = TRUE;
			else if (LOWORD(lParam) == FD_CONNECT)
				qe->mCanRead = TRUE;
			else
				qe = NULL;
		}
			#if defined(WSIOCPOPT)
			else
				qe = NULL;
			#endif
		if(qe)
			DList_Add(&poller->mSocketsForAsyncSelect, qe,
					mNextForEvent, mPrevForEvent);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif /* _WIN32 */

/*===========================================================================*/
/*=============================Main Implementation===========================*/
/*===========================================================================*/


void WSSocketNotifier_Construct(WSSOCKETNOTIFIER* pThis)
{
	pThis->mSocketLibRef = NULL;
}
void WSSocketNotifier_Destruct(WSSOCKETNOTIFIER* pThis)
{
	if(WSSocketNotifier_IsCreated(pThis))
	{
		WSSocketNotifier_Destroy(pThis);
		pThis->mSocketLibRef = NULL;
	}
}

#if defined(_WIN32)
wsbool WSSocketNotifier_CreateWithBackend(WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETLIBRARY* socklib, int backend)
#else
wsbool WSSocketNotifier_Create(WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETLIBRARY* socklib)
#endif
{
#if defined(_WIN32)
	#if defined(WSIOCPOPT)
		if ((backend & WSSOCKETNOTIFIER_BACKEND_IOCP)
			&& socklib->mCreateIoCompletionPort)
		{
			pNotifier->mhIOCP =
				(*socklib->mCreateIoCompletionPort)(
					INVALID_HANDLE_VALUE, NULL, 0, 0);
			if (!pNotifier->mhIOCP)
				return FALSE;
			pNotifier->mDeadSockets = NULL;
			WSLogDebug("Server: Using IOCP for Win32");
		}
		else
			pNotifier->mhIOCP = NULL;
	#endif
	#if defined(WSEVENTSELECTOPT)
		if(
		#if defined(WSIOCPOPT)
			(
			!pNotifier->mhIOCP
			#if defined(WSACCEPTEXOPT)
					|| !socklib->mAcceptEx
			#endif
			) &&
		#endif
			 (backend & WSSOCKETNOTIFIER_BACKEND_EVENTSELECT)
			&& socklib->mWSAEventSelect)
		{
			size_t i;
		#if defined(WSIOCPOPT)
			if(pNotifier->mhIOCP)
				pNotifier->mNumEvents = 1;
			else
		#endif
				pNotifier->mNumEvents = SERVER_NUM_EVENTS;
			pNotifier->mEvents = WSALLOC(void*, pNotifier->mNumEvents);
			pNotifier->mSocketsForEvents =
				WSALLOC(WSSOCKETNOTIFIERINFO*, pNotifier->mNumEvents);
			if(!pNotifier->mSocketsForEvents || !pNotifier->mEvents)
			{
				WSFREE(pNotifier->mSocketsForEvents);
				WSFREE(pNotifier->mEvents);
				return FALSE;
			}
			for(i = 0; i < pNotifier->mNumEvents; ++i)
			{
				pNotifier->mEvents[i] = CreateEventA(NULL, FALSE, FALSE, NULL);
				pNotifier->mSocketsForEvents[i] = NULL;
			}
			pNotifier->mNumSockets = 0;
			WSLogDebug("Server: Using WSAEventSelect() for Win32");
		}
		else
	#endif
		{
			#if defined(WSEVENTSELECTOPT)
				pNotifier->mEvents = NULL;
			#endif
			#if defined(WSASYNCSELECTOPT)
			if((backend & WSSOCKETNOTIFIER_BACKEND_ASYNCSELECT))
			{
				HINSTANCE hInstance = WSWinGetExeHMODULE();
				#if !defined(WIKISERVER_VERSION)
					WNDCLASSA wc =
						{ 0, WSSocketNotifierAsyncWndProc, 0, 0,
						hInstance, NULL, NULL,
						NULL, NULL, "WS" };
					pNotifier->mAtom = RegisterClassA(&wc); /*!atom == error*/
					if(!pNotifier->mAtom
							&& GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
						return FALSE;
				#endif
				pNotifier->mSocketsForAsyncSelect = NULL;
				pNotifier->mWnd = CreateWindowA("WS", NULL, 0, 0, 0, 0, 0,
					(HWND)NULL, (HMENU)NULL,
					hInstance, (LPVOID)NULL);
				WSSetWindowLongPtr(pNotifier->mWnd, GWLP_WNDPROC,
					WSSocketNotifierAsyncWndProc);
				WSSetWindowLongPtr(pNotifier->mWnd, GWLP_USERDATA, pNotifier);
				#if defined(WIKISERVER_VERSION)
					WSEventLoop_Create(&pNotifier->mLoop);
				#endif
			}
			#endif
		}
#elif defined(WSKQUEUEOPT)
	pNotifier->mKQueue = kqueue();
	if (pNotifier->mKQueue == -1)
	{
		WSLogError("Could not create kqueue");
		return FALSE;
	}
	pNotifier->mKqueueEvents = WSALLOC(struct kevent, SERVER_NUM_EVENTS);
	if (!pNotifier->mKqueueEvents)
		return FALSE;
#elif defined(WSEPOLLOPT)
	pNotifier->mEpollFD = epoll_create(SERVER_DEFAULTSOCKETNUM); /*num fds*/
	if (pNotifier->mEpollFD == -1)
	{
		WSLogError("Could not create epoll FD");
		return FALSE;
	}
	pNotifier->mEpollEvents = WSALLOC(struct epoll_event, SERVER_NUM_EVENTS);
	if (!pNotifier->mEpollEvents)
		return FALSE;
#endif
#if defined(WSUSERAWSELECT)
	#if defined(_WIN32)
		if(
			#if defined(WSEVENTSELECTOPT)
				!pNotifier->mEvents
					&&
			#endif
			#if defined(WSIOCPOPT) && defined(WSACCEPTEXOPT)
				(!pNotifier->mhIOCP || !socklib->mAcceptEx)
					&&
			#endif


			(backend & WSSOCKETNOTIFIER_BACKEND_RAWSELECT)
			)
	#endif
	{
		pNotifier->mFDSets = WSALLOC1(WSNOTIFIERFDSETS);
		if(!pNotifier->mFDSets)
			return FALSE;
		FD_ZERO(&pNotifier->mFDSets->read);
		FD_ZERO(&pNotifier->mFDSets->write);
		FD_ZERO(&pNotifier->mFDSets->lost);
		pNotifier->mFDSets->tv.tv_sec = 0; pNotifier->mFDSets->tv.tv_usec = 0;
		pNotifier->mMaxFD = 0;
		WSLogDebug("Server: Using raw select()");
	}
	#if defined(_WIN32)
		else
			pNotifier->mFDSets = NULL;
	#endif
#endif /*_WIN32*/
	pNotifier->mSockets = NULL;
	pNotifier->mSocketLibRef = socklib;
	return TRUE;
}

#if defined(_WIN32)
wsbool WSSocketNotifier_Create(WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETLIBRARY* socklib)
{
	return WSSocketNotifier_CreateWithBackend(pNotifier,
		socklib, WSSOCKETNOTIFIER_BACKEND_ANY);
}
#endif

void WSSocketNotifierDestroyEntries(
	 WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO** pInfo, wsbool clientsonly)
{
	if (*pInfo)
	{
		WSSOCKETNOTIFIERINFO* oldentry;
		WSSOCKETNOTIFIERINFO* nextentry = *pInfo;
		do
		{
			oldentry = nextentry;
			nextentry = oldentry->mNext;
			if (!clientsonly || !oldentry->mIsListener)
			{
				WSSocketNotifierRemoveEntry(pInfo, oldentry);
				WSSocketNotifier_DeleteEntry(pNotifier, pInfo, oldentry, TRUE);
			}
		} while (nextentry != oldentry);
		*pInfo = NULL;
	}
}

void WSSocketNotifier_Destroy(
	WSSOCKETNOTIFIER* pNotifier)
{
	WSSocketNotifierDestroyEntries(pNotifier, &pNotifier->mSockets, FALSE);
#if defined(_WIN32)
	#if defined(WSIOCPOPT)
		if (pNotifier->mhIOCP)
		{
			CloseHandle(pNotifier->mhIOCP);
			if (pNotifier->mDeadSockets)
			{
				WSSOCKETNOTIFIERINFO* oldentry = pNotifier->mDeadSockets;
				WSSOCKETNOTIFIERINFO* root = pNotifier->mDeadSockets;
				do
				{
					pNotifier->mDeadSockets = pNotifier->mDeadSockets->mNext;
					WSFREE(oldentry);
					oldentry = pNotifier->mDeadSockets;
				} while (pNotifier->mDeadSockets != root);
			}
		}
	#endif
	#if defined(WSEVENTSELECTOPT)
			if (
		#if defined(WSIOCPOPT)
				(
				!pNotifier->mSocketLibRef->mCreateIoCompletionPort
		#if defined(WSACCEPTEXOPT)
				|| !pNotifier->mSocketLibRef->mAcceptEx
		#endif
				) &&
		#endif
				 pNotifier->mSocketLibRef->mWSAEventSelect)
		{
			size_t i;
			for(i = 0; i < pNotifier->mNumEvents; ++i)
				CloseHandle(pNotifier->mEvents[i]);
			WSFREE(pNotifier->mEvents);
			WSFREE(pNotifier->mSocketsForEvents);
		}
		#if defined(WSASYNCSELECTOPT)
			else
		#endif
	#endif
	#if defined(WSASYNCSELECTOPT)
		{
			#if defined(WIKISERVER_VERSION)
				WSEventLoop_Destroy(&pNotifier->mLoop);
			#endif
			DestroyWindow(pNotifier->mWnd);
			#if !defined(WIKISERVER_VERSION)
				if(pNotifier->mAtom)
					UnregisterClassA(MAKEINTATOM(pNotifier->mAtom),
						WSWinGetExeHMODULE());
			#endif
		}
	#endif
#elif defined(WSKQUEUEOPT)
	close(pNotifier->mKQueue);
	WSFREE(pNotifier->mKqueueEvents);
#elif defined(WSEPOLLOPT)
	close(pNotifier->mEpollFD);
	WSFREE(pNotifier->mEpollEvents);
#endif
#if defined(WSUSERAWSELECT)
	WSFREE(pNotifier->mFDSets);
#endif
}

wsbool WSSocketNotifier_IsCreated(WSSOCKETNOTIFIER* pThis)
{ return pThis->mSocketLibRef != NULL; }

void WSSocketNotifier_AddInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* entry)
{
	WSSocketNotifier_AddEntry(pNotifier, &pNotifier->mSockets, entry);
}

wsbool WSSocketNotifier_TrackInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* entry)
{
	/*WSLogDebug(MutStr("AddSocket(entry) THREAD [")*/
	/*	<< WSThreadGetCurrentID() << "] ENTRY [" << (size_t)entry*/
	/*	<< "] SOCKET [" << (size_t)WSSocketSSL_GetHandle(&entry->mSocket) << "]");*/
#if defined(_WIN32)
	#if defined(WSIOCPOPT)
		if (pNotifier->mhIOCP && (!entry->mIsListener
			#if defined(WSACCEPTEXOPT)
				|| pNotifier->mSocketLibRef->mAcceptEx
			#endif
				)
			)
		{
			if (!(*pNotifier->mSocketLibRef->mCreateIoCompletionPort)(
				WSSocketSSL_GetHandle(&entry->mSocket),
				pNotifier->mhIOCP, entry, 0))
			{
				WSLogError("CreateIOCompletionPort failed");
				return FALSE;
			}
		}
		#ifdef WSEVENTSELECTOPT
		else
		#endif
	#endif
	#ifdef WSEVENTSELECTOPT
		if(pNotifier->mEvents
		#if defined(WSIOCPOPT)
			&& (!pNotifier->mhIOCP || (entry->mIsListener
			#if defined(WSACCEPTEXOPT)
				&& !pNotifier->mSocketLibRef->mAcceptEx
			#endif
				))
		#endif
			) /* EventSelect */
		{ /* NOTE: If WSAEventSelect gives an invalid param error you are likely mixing 32+64 bit */
			size_t eventIndex;
			if(pNotifier->mNumSockets < pNotifier->mNumEvents)
				eventIndex = pNotifier->mNumSockets;
			else
				eventIndex =
					(pNotifier->mNumSockets) & (pNotifier->mNumEvents-1);
			entry->mEventIndex = eventIndex;
			if(entry->mIsListener)
			{
				if((*pNotifier->mSocketLibRef->mWSAEventSelect)(
						WSSocketSSL_GetHandle(&entry->mSocket),
						pNotifier->mEvents[eventIndex], FD_ACCEPT)
							== SOCKET_ERROR)
				{
					WSLogError("WSAEventSelect failed");
					return FALSE;
				}
			}
			else /* client */
			{
				if((*pNotifier->mSocketLibRef->mWSAEventSelect)(
						WSSocketSSL_GetHandle(&entry->mSocket),
						pNotifier->mEvents[eventIndex],
						FD_READ|FD_WRITE) == SOCKET_ERROR)
				{
					WSLogError("WSAEventSelect failed");
					return FALSE;
				}
			}

			if(pNotifier->mNumSockets < pNotifier->mNumEvents)
			{
				pNotifier->mSocketsForEvents[pNotifier->mNumSockets] = entry;
				entry->mPrevForEvent = entry->mNextForEvent = entry;
				entry->mEventIndex = pNotifier->mNumSockets;
			}
			else
			{
				size_t index
					= (pNotifier->mNumSockets) & (pNotifier->mNumEvents-1);
				pNotifier->mSocketsForEvents[index]->mPrevForEvent->mNextForEvent
					= entry;
				entry->mPrevForEvent =
					pNotifier->mSocketsForEvents[index]->mPrevForEvent;
				pNotifier->mSocketsForEvents[index]->mPrevForEvent = entry;
				entry->mNextForEvent = pNotifier->mSocketsForEvents[index];
				entry->mEventIndex = index;
			}
			++pNotifier->mNumSockets;
		} /* end if no IOCP or acceptex */
	#endif
	#if defined(WSASYNCSELECTOPT)
		/*!pNotifier->mhIOCP - need ASync for listen socket if AcceptEx is not available*/
		/*as IOCP can not do normal listening.*/
		/* !mlpAcceptEx - need ASync for r/w */
		#if defined(WSEVENTSELECTOPT) || defined(WSIOCPOPT) || defined(WSACCEPTEXOPT)
		if(
			#if defined(WSEVENTSELECTOPT)
				(!pNotifier->mSocketLibRef->mWSAEventSelect)
				#if defined(WSIOCPOPT)
					&&
				#endif
			#endif
				(
			#if defined(WSIOCPOPT)
			(!entry->mIsListener && !pNotifier->mhIOCP)
				#if defined(WSACCEPTEXOPT)
					||
				#endif
			#endif
			#if defined(WSACCEPTEXOPT)
				(entry->mIsListener && !pNotifier->mSocketLibRef->mAcceptEx)
			#endif
				)
		)
		#endif
		{ /* No WinSock 2, fallback to async event select */
			if (WSAAsyncSelect(PTRCASTTYPE(SOCKET)
				WSSocketSSL_GetHandle(&entry->mSocket), pNotifier->mWnd,
				WM_USER,
				entry->mIsListener ? FD_ACCEPT :
						   FD_READ|FD_WRITE|FD_CLOSE|FD_CONNECT) == SOCKET_ERROR)
			{
				WSLogError("Error setting up windows event handler for Winsock 1.1!");
				return FALSE;
			}
			WSLogDebug("Server: Using WSAAsyncSelect() for Win32");
		}
	#endif
#elif defined(WSKQUEUEOPT)
	struct kevent ke;
	EV_SET(&ke, PTRCASTTYPE(SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),
		(entry->mIsListener ? EVFILT_READ : EVFILT_READ|EVFILT_WRITE),
		EV_ADD, 0, 5, entry);
	if (kevent(pNotifier->mKQueue, &ke, 1, NULL, 0, NULL) == -1)
	{
		WSLogError("Could not set initial kevent");
		return FALSE;
	}
#elif defined(WSEPOLLOPT)
	struct epoll_event ev;
	ev.events = entry->mIsListener ? EPOLLIN : EPOLLIN|EPOLLOUT;
	ev.data.ptr = entry;
	if (epoll_ctl(pNotifier->mEpollFD, EPOLL_CTL_ADD,
		PTRCASTTYPE(SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),
		&ev) == -1) /*0 success*/
	{
		WSLogError("Could not add event to epoll");
		return FALSE;
	}
#endif
#if defined(WSUSERAWSELECT)
	#if defined(_WIN32) \
		&& (defined(WSEVENTSELECTOPT) \
			|| (!defined(WSEVENTSELECTOPT) && !defined(WSASYNCSELECTOPT) \
				&& (defined(WSIOCPOPT) || defined(WSASYNCSELECTOPT))))
	else
	#endif
	{
		if(entry->mIsListener)
		{
			WSFD_SET(PTRCASTTYPE(SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),
				&pNotifier->mFDSets->read);
		}
		else
		{
			WSFD_SET(PTRCASTTYPE(SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),
				&pNotifier->mFDSets->read);
			WSFD_SET(PTRCASTTYPE(SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),
				&pNotifier->mFDSets->write);
			WSFD_SET(PTRCASTTYPE(SOCKET)WSSocketSSL_GetHandle(&entry->mSocket),
				&pNotifier->mFDSets->lost);
		}
		if((SOCKET)WSSocketSSL_GetHandle(&entry->mSocket)
				> (SOCKET)pNotifier->mMaxFD)
			pNotifier->mMaxFD = (void*)WSSocketSSL_GetHandle(&entry->mSocket);
	}
#endif /*_WIN32*/
	/*WSLogDebug(MutStr("AddSocket(entry) FIN ENTRY [") << (size_t)entry*/
	/*	<< "] SOCKET [" << (size_t)WSSocketSSL_GetHandle(&entry->mSocket) << "]");*/
	return TRUE;
}
void WSSocketNotifier_UpdateInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* pInfo)
{
	WSSocketNotifierRemoveEntry(&pNotifier->mSockets, pInfo);
	WSSocketNotifierUpdateEntry(&pNotifier->mSockets, pInfo);
}

void WSSocketNotifier_RemoveInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* pInfo)
{
	WSSocketNotifierRemoveEntry(&pNotifier->mSockets, pInfo);
}
void WSSocketNotifier_DestroyInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* pInfo)
{
	WSSocketNotifier_DeleteEntry(
		pNotifier, &pNotifier->mSockets, pInfo, FALSE);
}


void WSSocketNotifier_InitInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* entry,
	wsbool mIsListener, wsbool mIsListenerClient)
{
	(void)(pNotifier);
	entry->mIsListener = mIsListener;
	entry->mPending = NULL;
	entry->mIsListenerClient = mIsListenerClient;
	entry->mCanAccept = entry->mCanRead = entry->mCanWrite =
#if defined(_WIN32) && defined(WSIOCPOPT)
		entry->mIsDead =
#endif
		entry->mIsDisconnected = FALSE;
	entry->mShouldDestroySocket = TRUE;
#if defined(_WIN32) && defined(WSASYNCSELECTOPT)
	entry->mNextForEvent = NULL;
#endif
}
void WSSocketNotifier_InitListenerClientInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* info)
{
	WSSocketNotifier_InitInfo(pNotifier, info, FALSE, TRUE);
}
void WSSocketNotifier_InitClientInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* info)
{
	WSSocketNotifier_InitInfo(pNotifier, info, FALSE, FALSE);
}
void WSSocketNotifier_InitListenerInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* info)
{
	WSSocketNotifier_InitInfo(pNotifier, info, TRUE, FALSE);
}

WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddSocket(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKET* sock,
	wsbool mIsListener, wsbool mIsListenerClient)
{
	WSSOCKETNOTIFIERINFO* info = WSALLOC1(WSSOCKETNOTIFIERINFO);
	if(!info)
		return NULL;
	WSSocket_Construct(&info->mSocket);
	WSSocket_Own(&info->mSocket, sock);
	WSSocketNotifier_InitInfo(
			pNotifier, info, mIsListener, mIsListenerClient);
	if(!info)
		return NULL;
	if(!WSSocketNotifier_TrackInfo(pNotifier, info))
	{
		WSSocketNotifierInfo_Destroy(info);
		return NULL;
	}
	WSSocketNotifier_AddInfo(pNotifier, info);
	return info;
}

WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddListenerClientSocket(

	WSSOCKETNOTIFIER* pNotifier, WSSOCKET* sock)
{
	return WSSocketNotifier_AddSocket(
		pNotifier, sock, FALSE, TRUE);
}
WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddClientSocket(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKET* sock)
{
	return WSSocketNotifier_AddSocket(
		pNotifier, sock, FALSE, FALSE);
}
WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddListenerSocket(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKET* sock)
{
	return WSSocketNotifier_AddSocket(
		pNotifier, sock, TRUE, FALSE);
}


void WSSocketNotifier_Begin(WSSOCKETNOTIFIER* pNotifier)
{
#if defined(_WIN32)
	#if defined(WSIOCPOPT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ IOCP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (pNotifier->mhIOCP)
	{
	}
	#endif
	#if defined(WSEVENTSELECTOPT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ WSAEventSelect ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (pNotifier->mEvents)
	{
		pNotifier->mEventIndex = WaitForMultipleObjects(
			pNotifier->mNumEvents, pNotifier->mEvents, FALSE, 0);
	} /* end if */
	#endif
#elif defined(WSKQUEUEOPT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ kevent()/kqueue ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	struct timespec tv;
	tv.tv_sec = 0;
	tv.tv_nsec = 0;
	pNotifier->mNumEvents = kevent(pNotifier->mKQueue,
		NULL, 0, pNotifier->mKqueueEvents, SERVER_NUM_EVENTS, &tv);
	pNotifier->mCurEvent = 0;
#elif defined(WSEPOLLOPT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ epoll() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	pNotifier->mNumFDReady = epoll_wait(pNotifier->mEpollFD,
		pNotifier->mEpollEvents, SERVER_NUM_EVENTS, 0);
	pNotifier->mCurFD = 0;
#endif
#if defined(WSUSERAWSELECT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ RAW select() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	#if	defined(_WIN32)
	if(pNotifier->mFDSets)
	#endif
	{ /* select() empties the fds upon function exit thus needing the copy */
		pNotifier->mFDSets->readcur = pNotifier->mFDSets->read;
		pNotifier->mFDSets->writecur = pNotifier->mFDSets->write;
		pNotifier->mFDSets->lostcur = pNotifier->mFDSets->lost;
		pNotifier->mNumSelectedFDs = select(
			(int)(((SOCKET)pNotifier->mMaxFD)+1),
			&pNotifier->mFDSets->readcur, /* &writefds */ 0,
			&pNotifier->mFDSets->lostcur, &pNotifier->mFDSets->tv);
		/*if(pNotifier->mNumSelectedFDs == -1)
		{
			int err;
			perror("");
			err = WSAGetLastError();
		}*/
		pNotifier->mCurFD = 0;
		pNotifier->mFDSets->nextinfo = pNotifier->mSockets;
	}
#endif /*_WIN32*/
	pNotifier->mLastPost = NULL;
}

wsbool WSSocketNotifier_Next(
	WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO** ppInfo, WSSOCKETNOTIFIERPOST** ppPost)
{
	WSSOCKETNOTIFIERPOST* post;
	if(pNotifier->mLastPost)
		WSSocketNotifierPost_Destroy(pNotifier->mLastPost);
#if defined(_WIN32)
	#if defined(WSIOCPOPT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ IOCP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (pNotifier->mhIOCP)
	{
		uint32_t dwIOSize;
		WSSOCKETNOTIFIERINFO* entry;
		BOOL lgqcRet;
		OVERLAPPED* data;
		while (TRUE)
		{
			lgqcRet =
#if 0
				GetQueuedCompletionStatus(
					pNotifier->mhIOCP, &dwIOSize,
					(PULONG_PTR)&entry,
					&data, 0);
#else
				(*pNotifier->mSocketLibRef->mGetQueuedCompletionStatus)(
					pNotifier->mhIOCP, &dwIOSize,
					(void**)&entry,
					&data,
					0);
#endif
			if (!lgqcRet)
			{
				DWORD dwLastError = GetLastError();
				/*if(dwLastError != WAIT_TIMEOUT)*/
				/*{*/
				/*	int breakhere = 5;*/
				/*}*/
				if(dwLastError != ERROR_NETNAME_DELETED
					&& dwLastError != ERROR_CONNECTION_ABORTED)
					break; /* Error or finished */
			}

			/* This happens in the case of an AcceptEx() socket */
			/* ready to be accepted. It gets passed through the server */
			/* socket and overlapped.Pointer contains the WSSOCKETNOTIFIERINFO */
			/* set in PrepareIODataForAcceptEx() */

			/* CONTAINING_RECORD is an involved macro that expands to: */
			/* (WSSOCKETNOTIFIERPOST*)(data - &((WSSOCKETNOTIFIERPOST*)0)->overlapped) */
			/* It takes the offset of overlapped, the 3rd parameter, */
			/* within WSSOCKETNOTIFIERPOST, the structure type passed to the 2nd */
			/* parameter, and subtracts THAT from the variable data, */
			/* the 1st parameter. This is passible because overlapped */
			/* is allocated as part of the variable data, all part of */
			/* one memory block so we can subtract overlapped to get */
			/* the address of the structure containing it. */
			*ppPost = CONTAINING_RECORD(
				data, WSSOCKETNOTIFIERPOST, mOverlapped);
			/*entry = (WSSOCKETNOTIFIERINFO*)*/
			/*	OVERLAPPED_DATA(&(*ppPost)->mOverlapped);*/
			WSASSERT(entry);
			WSASSERT(*ppPost);

			/* dead socket handling, not part of if(entry) */
			/* closedsocket() only closes the user handle, not the kernel side */
			/* so GetQueuedCompletionStatus() will still return for */
			/* sockets already closed with closedsocket() - we */
			/* need to keep track and not handle those dead sockets */
			if (entry->mIsDead)
			{
				WSSOCKETNOTIFIERPOST* pending = entry->mPending;
				if (pending->mNext == pending)
				{ /* Now, FINALLY we can delete it */
					DList_Remove(&pNotifier->mDeadSockets,
						entry, mNext, mPrev);
					WSSocketNotifierInfo_Destroy(entry);
				}
				else
				{
					DList_Remove(&entry->mPending, pending,
						mNext, mPrev);
				}
				WSSocketNotifierPost_Destroy(pending);
				continue;
			}

			*ppInfo = entry;

			/*dwIOSize == 0 is an AcceptEx*/
			if (!lgqcRet)/* || dwIOSize == 0) */
			{ /* Disconnected/Closed */
				pNotifier->mLastPost = NULL;
				*ppPost = NULL;
				entry->mIsDisconnected = TRUE;
				return TRUE;
			}
			/* Success */
			pNotifier->mLastPost = *ppPost;
			DList_Remove(&entry->mPending, pNotifier->mLastPost, mNext, mPrev);
			return TRUE;
		}
		#if defined(WSACCEPTEXOPT)
		if(pNotifier->mSocketLibRef->mAcceptEx)
			return FALSE;
		#endif
	}
	#endif
	#if defined(WSEVENTSELECTOPT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ WSAEventSelect ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if (pNotifier->mEvents)
	{
		WSWSANETWORKEVENTS ne;
		*ppInfo = NULL;
		while (pNotifier->mEventIndex >= WAIT_OBJECT_0 &&
			pNotifier->mEventIndex <= WAIT_OBJECT_0
				+ pNotifier->mNumEvents - 1)
		{
			WSSOCKETNOTIFIERINFO* rootForEvent;

			pNotifier->mEventIndex -= WAIT_OBJECT_0;
			rootForEvent =
				pNotifier->mSocketsForEvents[pNotifier->mEventIndex];
			if (rootForEvent)
			{
				WSSOCKETNOTIFIERINFO* entry = rootForEvent;
				do
				{
					WSSOCKETNOTIFIERINFO* nextForEvent = entry->mNextForEvent;
					if (entry->mIsListener)
					{
						if ((*pNotifier->mSocketLibRef->mWSAEnumNetworkEvents)(
							WSSocketSSL_GetHandle(&entry->mSocket),
							NULL, &ne) != SOCKET_ERROR
							&& (ne.lNetworkEvents & FD_ACCEPT))
						{
							*ppInfo = entry;
							entry->mCanAccept = TRUE;
						}
					}
					else
					{
						if ((*pNotifier->mSocketLibRef->mWSAEnumNetworkEvents)(
								WSSocketSSL_GetHandle(&entry->mSocket),
								NULL, &ne) != SOCKET_ERROR
							&& (ne.lNetworkEvents & (FD_READ | FD_WRITE)))
						{
							*ppInfo = entry;
							entry->mCanRead = (ne.lNetworkEvents & FD_READ);
							entry->mCanWrite = (ne.lNetworkEvents & FD_WRITE);
						}
					}
					if(*ppInfo)
						break;
					entry = nextForEvent;
				} while (entry != rootForEvent);
			} /* end if events[i] exists */
			if(*ppInfo)
				break;
			pNotifier->mEventIndex = WaitForMultipleObjects(
				pNotifier->mNumEvents, pNotifier->mEvents, FALSE, 0);
		} /* end if event was waited upon */
		if(!*ppInfo)
			return FALSE;
	} /* end if */
		#if defined(WSASYNCSELECTOPT)
			else
		#endif
	#endif
	#if defined(WSASYNCSELECTOPT)
	{
		if (!pNotifier->mSocketsForAsyncSelect)
		{
	#if defined(WIKISERVER_VERSION)
			WSEventLoop_RunSingle(&pNotifier->mLoop);
	#else
			MSG msg;
			while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					PostQuitMessage((int)msg.wParam);
					break;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessageA(&msg);
				}
			}
	#endif
		}
		if (pNotifier->mSocketsForAsyncSelect)
		{
			*ppInfo = pNotifier->mSocketsForAsyncSelect;
			DList_Remove(&pNotifier->mSocketsForAsyncSelect,
				(*ppInfo), mNextForEvent, mPrevForEvent);
		}
		else
			return FALSE;
	}
	#endif
#elif defined(WSKQUEUEOPT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ kevent()/kqueue ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if(pNotifier->mCurEvent >= pNotifier->mNumEvents)
		return FALSE;
	(*ppInfo) = (WSSOCKETNOTIFIERINFO*)
		pNotifier->mKqueueEvents[pNotifier->mCurEvent].udata;
	if((*ppInfo)->mIsListener)
		(*ppInfo)->mCanAccept = TRUE;
	else
	{
		(*ppInfo)->mCanRead =
			(pNotifier->mKqueueEvents[pNotifier->mCurEvent].filter
				& EVFILT_READ);
		(*ppInfo)->mCanWrite =
			(pNotifier->mKqueueEvents[pNotifier->mCurEvent].filter
				& EVFILT_WRITE);
	}
	++pNotifier->mCurEvent;
#elif defined(WSEPOLLOPT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ epoll() ~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	if(pNotifier->mCurFD >= pNotifier->mNumFDReady)
		return FALSE;
	(*ppInfo) = (WSSOCKETNOTIFIERINFO*)
		pNotifier->mEpollEvents[pNotifier->mCurFD].data.ptr;
	if((*ppInfo)->mIsListener)
		(*ppInfo)->mCanAccept = TRUE;
	else
	{
		(*ppInfo)->mCanRead =
			(pNotifier->mEpollEvents[pNotifier->mCurFD].events & EPOLLIN);
		(*ppInfo)->mCanWrite =
			(pNotifier->mEpollEvents[pNotifier->mCurFD].events & EPOLLOUT);
	}
	++pNotifier->mCurFD;
#endif

#if defined(WSUSERAWSELECT)
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~ RAW select() ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	#if defined(_WIN32)
		#if defined(WSEVENTSELECTOPT)
		else
		#endif
			if(pNotifier->mFDSets)
	#endif
	{
		if (pNotifier->mNumSelectedFDs <= 0)
			return FALSE;
		{
			WSSOCKETNOTIFIERINFO* entry = pNotifier->mFDSets->nextinfo;
			*ppInfo = NULL;
			/*if(pNotifier->mNumSelectedFDs > 1)
			{
				int breakhere = 5;
			}*/
			do
			{
				wsbool processed = FALSE;
				pNotifier->mFDSets->nextinfo = entry->mNext;
				if (FD_ISSET((SOCKET)WSSocket_GetHandle(&entry->mSocket),
							&pNotifier->mFDSets->readcur))
				{
					if (entry->mIsListener)
						entry->mCanAccept = TRUE;
					else
						entry->mCanRead = TRUE;
					processed = TRUE;
				}
				/*if (isAlive && FD_ISSET((SOCKET)*/
				/* entry->mSocket.GetHandle(), &writefds)) */
				/*{*/
				/*	isAlive = HandleWrite(qe);*/
				/*	processed = TRUE;*/
				/*}*/
				if (FD_ISSET((SOCKET)WSSocket_GetHandle(&entry->mSocket),
								&pNotifier->mFDSets->lostcur))
				{
					entry->mIsDisconnected = TRUE;
					processed = TRUE;
				}
				if (processed)
				{
					*ppInfo = entry;
					--pNotifier->mNumSelectedFDs;
					break;
				}
				entry = pNotifier->mFDSets->nextinfo;
			} while (entry != pNotifier->mSockets
					&& pNotifier->mNumSelectedFDs != 0);
			if(!*ppInfo)
				return FALSE;
		}
	}
#endif
	WSASSERT((*ppInfo));
	post = (*ppInfo)->mPending;
	pNotifier->mLastPost = *ppPost = NULL;
	while (post)
	{
		if ((post->mIsRead && (*ppInfo)->mCanRead)
			|| (post->mIsWrite && (*ppInfo)->mCanWrite))
		{
			*ppPost = pNotifier->mLastPost = post;
			DList_Remove(&(*ppInfo)->mPending,
				pNotifier->mLastPost, mNext, mPrev);
			break;
		}
	}
	return TRUE;
}

WSSOCKETNOTIFIERPOST* WSSocketNotifierAllocPost(
	WSSOCKETNOTIFIERINFO* pInfo, void* pvData,
	size_t len, size_t pos, wsbool shouldFreeData, char isWrite)
{
	/*OVERLAPPED* lp;*/
	WSSOCKETNOTIFIERPOST* data = WSALLOC1(WSSOCKETNOTIFIERPOST);
	if (!data)
		return FALSE;

	data->mBuffer = pvData;
	data->mNext = pInfo->mPending;
	data->mPos = pos;
	data->mSize = len;
	data->mShouldFreeBuffer = shouldFreeData;
	data->mIsWrite = isWrite;
#ifdef _WIN32
	WSMemoryFill(&data->mOverlapped, 0, sizeof(data->mOverlapped));
	OVERLAPPED_DATA(&data->mOverlapped) = pInfo;
	/*lp = (OVERLAPPED*)&data->mOverlapped;*/
#endif
	return data;
}
void WSSocketNotifierAddPost(
	WSSOCKETNOTIFIERINFO* pInfo, WSSOCKETNOTIFIERPOST* pPost)
{
	DList_Add(&pInfo->mPending, pPost, mNext, mPrev);
}
wsbool WSSocketNotifierPost_(WSSOCKETNOTIFIERINFO* pInfo, void* pvData,
	size_t len, size_t pos, wsbool shouldFreeData, char isWrite)
{
	/*	AssureEntryIsGone(mAcceptExClients, &entry);*/
#ifdef _WIN32
	DWORD dwDataCount;
	BOOL res;
#endif
	WSSOCKETNOTIFIERPOST* data;
	data = WSSocketNotifierAllocPost(pInfo,
		pvData, len, pos, shouldFreeData, isWrite);
	if(!data)
		return FALSE;
#ifdef _WIN32
	if (isWrite)
		res = WriteFile((HANDLE)WSSocketSSL_GetHandle(&pInfo->mSocket),
			(LPCVOID)&((uint8_t*)data->mBuffer)[pos], (DWORD)len, (LPDWORD)&dwDataCount,
			(LPOVERLAPPED)&data->mOverlapped);
	else
		res = ReadFile((HANDLE)WSSocketSSL_GetHandle(&pInfo->mSocket),
			&((uint8_t*)data->mBuffer)[pos], (DWORD)len, (LPDWORD)&dwDataCount,
			(LPOVERLAPPED)&data->mOverlapped);
	if (!res && GetLastError() != ERROR_IO_PENDING)
	{
		WSFREE(data);
		return FALSE;
	}
	else
#endif
		WSSocketNotifierAddPost(pInfo, data);
	return TRUE;
}
wsbool WSSocketNotifierPostRead(WSSOCKETNOTIFIERINFO* pInfo,
	void* pvData, size_t len, size_t pos, wsbool shouldFreeData)
{
	return WSSocketNotifierPost_(pInfo, pvData, len, pos,
								shouldFreeData, FALSE);
}
wsbool WSSocketNotifierPostWrite(WSSOCKETNOTIFIERINFO* pInfo,
	const void* pvData, size_t len, size_t pos, wsbool shouldFreeData)
{
	return WSSocketNotifierPost_(pInfo, (void*)pvData, len, pos,
								shouldFreeData, TRUE);
}
/*=============================================================================
		 ####### ######  ####### #     #  #####   #####  #
		 #     # #     # #       ##    # #     # #     # #
		 #     # #     # #       # #   # #       #       #
		 #     # ######  #####   #  #  #  #####   #####  #
		 #     # #       #       #   # #       #       # #
		 #     # #       #       #    ## #     # #     # #
		 ####### #       ####### #     #  #####   #####  #######
=============================================================================*/
#if defined(WSSSLOPT) && (!defined(_WIN32) || !defined(WSNTVSSLOPT))
#define NID_commonName 13
#ifdef X509_NAME /* _WIN32 */
#undef X509_NAME
#endif
	enum { /* macros with no prefix */
		X509_V_OK = 0,
		X509_FILETYPE_PEM = 1,
		X509_FILETYPE_ASN1 = 2,
	};
	enum { /* OPENSSL_ prefix macros */
		INIT_LOAD_CRYPTO_STRINGS = 0x00000002L,
		INIT_LOAD_SSL_STRINGS = 0x00200000L,
	};
	enum { /* SSL_ prefix macros */
		NOCLOSE = 0,
		CTRL_RESET = 1,
		CTRL_SET_CLOSE = 9,
		CTRL_SET_SESS_CACHE_MODE = 44,
		ERROR_SSL = 1,
		ERROR_WANT_READ = 2,
		ERROR_WANT_WRITE = 3,
		ERROR_SYSCALL = 5,
		SESS_CACHE_CLIENT = 1,
		VERIFY_PEER = 1,
		VERIFY_FAIL_IF_NO_PEER_CERT = 2,
	};
	typedef struct ssl_st SSL;
	typedef struct ssl_ctx_st SSL_CTX;
	typedef struct ssl_method_st SSL_METHOD;
	typedef struct ossl_init_settings_st OPENSSL_INIT_SETTINGS;
	typedef struct evp_pkey_st EVP_PKEY;
	typedef struct bio_st BIO;
	typedef struct x509_st X509;
	typedef struct x509_store_ctx_st X509_STORE_CTX;
	typedef struct x509_store_st X509_STORE;
	typedef struct X509_name_st X509_NAME;
	typedef int (*SSL_verify_cb)(int preverify_ok, X509_STORE_CTX* x509_ctx);
	typedef int (*SSL_CTX_LOAD_VERIFY_LOCATIONSPROC)(SSL_CTX* ctx,
		const char* CAfile, const char* CApath);
	typedef void (*SSL_CTX_FREEPROC)(SSL_CTX* ctx);
	typedef SSL_CTX* (*SSL_CTX_NEWPROC)(const SSL_METHOD* method);
	typedef long (*SSL_CTX_CTRLPROC)(SSL_CTX *ctx, int cmd, long larg, void *parg);
	typedef int (*SSL_CTX_USE_CERTIFICATE_FILEPROC)(SSL_CTX* ctx, const char* file, int type);
	typedef int (*SSL_CTX_USE_CERTIFICATE_CHAIN_FILEPROC)(SSL_CTX* ctx, const char* file);
	typedef int (*SSL_CTX_USE_PRIVATEKEY_FILEPROC)(SSL_CTX* ctx, const char* file,
		int type);
	typedef void (*SSL_CTX_SET_VERIFYPROC)(SSL_CTX* ctx, int mode, SSL_verify_cb callback);
	typedef void (*SSL_CTX_SET_VERIFY_DEPTHPROC)(SSL_CTX* ctx, int depth);
	typedef long (*SSL_CTX_SET_TIMEOUTPROC)(SSL_CTX* ctx, long t);
	typedef SSL* (*SSL_NEWPROC)(SSL_CTX* ctx);
	typedef int (*SSL_SHUTDOWNPROC)(SSL* s);
	typedef void (*SSL_FREEPROC)(SSL* ssl);
	typedef int (*SSL_ACCEPTPROC)(SSL* ssl);
	typedef int (*SSL_CONNECTPROC)(SSL* ssl);
	typedef int (*SSL_SET_FDPROC)(SSL* s, int fd);
	typedef int (*OPENSSL_INIT_SSLPROC)(uint64_t opts, const OPENSSL_INIT_SETTINGS* settings);
	typedef int (*SSL_WRITEPROC)(SSL* ssl, const void* buf, int num);
	typedef int (*SSL_READPROC)(SSL* ssl, void* buf, int num);
	typedef int (*SSL_WRITE_EXPROC)(SSL* ssl, const void* buf, size_t num, size_t* written);
	typedef int (*SSL_READ_EXPROC)(SSL* ssl, void* buf, size_t num, size_t* readbytes);
	typedef int (*SSL_PENDINGPROC)(const SSL *ssl);
	typedef long (*SSL_GET_VERIFY_RESULTPROC)(const SSL* ssl);
	typedef X509* (*SSL_GET_PEER_CERTIFICATEPROC)(const SSL* s);
	typedef int (*X509_NAME_GET_TEXT_BY_NIDPROC)(X509_NAME* name, int nid, char* buf, int len);
	typedef X509_NAME* (*X509_GET_SUBJECT_NAMEPROC)(const X509* a);
	typedef long (*SSL_CTRLPROC)(SSL* ssl, int cmd, long larg, void* parg);
	typedef int (*SSL_GET_ERRORPROC)(const SSL* ssl, int ret);
	typedef void (*ERR_ERROR_STRING_NPROC)(unsigned long e, char* buf, size_t len);
	typedef unsigned long (*ERR_GET_ERRORPROC)();
	typedef const SSL_METHOD* (*SSL_METHODPROC)();
	typedef int pem_password_cb(char *buf, int size, int rwflag, void *u);
	typedef EVP_PKEY * (*B2I_PVK_BIOPROC)(BIO *in, pem_password_cb *cb, void *u);
	typedef int (*BIO_FREEPROC)(BIO *a);
	typedef BIO * (*BIO_NEW_FILEPROC)(const char *filename, const char *mode);
	/*typedef void (*SSL_CTX_SET_DEFAULT_PASSWD_CBPROC)(SSL_CTX *ctx, pem_passwd_cb cb);*/
	/*typedef void (*SSL_CTX_SET_DEFAULT_PASSWD_CB_USERDATAPROC)(SSL_CTX *ctx, void *u);*/

	/*SSL_CTX_SET_DEFAULT_PASSWD_CB_USERDATAPROC SSL_CTX_set_default_passwd_cb_userdataProc;*/
	/*SSL_CTX_SET_DEFAULT_PASSWD_CBPROC SSL_CTX_set_default_passwd_cbProc;*/
	typedef int (*SSL_CTX_USE_PRIVATEKEYPROC)(SSL_CTX *ctx, EVP_PKEY *pkey);
	typedef X509_STORE * (*SSL_CTX_GET_CERT_STOREPROC)(const SSL_CTX *ctx);
	typedef int (*X509_STORE_ADD_CERTPROC)(X509_STORE *ctx, X509 *x);
	typedef X509 * (*D2I_X509_BIOPROC)(BIO *bp, X509 **x);

typedef struct WSSSLLOADER
{
	SSL_CTX_FREEPROC SSL_CTX_freeProc;
	SSL_CTX_NEWPROC SSL_CTX_newProc;
	SSL_CTX_CTRLPROC SSL_CTX_ctrlProc;
	SSL_CTX_USE_CERTIFICATE_FILEPROC SSL_CTX_use_certificate_fileProc;
	SSL_CTX_USE_CERTIFICATE_CHAIN_FILEPROC SSL_CTX_use_certificate_chain_fileProc;
	SSL_CTX_USE_PRIVATEKEYPROC SSL_CTX_use_PrivateKeyProc;
	SSL_CTX_USE_PRIVATEKEY_FILEPROC SSL_CTX_use_PrivateKey_fileProc;
	SSL_CTX_LOAD_VERIFY_LOCATIONSPROC SSL_CTX_load_verify_locationsProc;
	SSL_CTX_SET_VERIFYPROC SSL_CTX_set_verifyProc;
	SSL_CTX_SET_VERIFY_DEPTHPROC SSL_CTX_set_verify_depthProc;
	SSL_CTX_SET_TIMEOUTPROC SSL_CTX_set_timeoutProc;
	SSL_NEWPROC SSL_newProc;
	SSL_FREEPROC SSL_freeProc;
	SSL_ACCEPTPROC SSL_acceptProc;
	SSL_CONNECTPROC SSL_connectProc;
	SSL_SHUTDOWNPROC SSL_shutdownProc;
	SSL_SET_FDPROC SSL_set_fdProc;
	OPENSSL_INIT_SSLPROC OPENSSL_init_sslProc;
	SSL_WRITEPROC SSL_writeProc;
	SSL_READPROC SSL_readProc;
	SSL_WRITE_EXPROC SSL_write_exProc;
	SSL_READ_EXPROC SSL_read_exProc;
	SSL_PENDINGPROC SSL_pendingProc;
	SSL_GET_VERIFY_RESULTPROC SSL_get_verify_resultProc;
	SSL_GET_PEER_CERTIFICATEPROC SSL_get_peer_certificateProc;
	X509_NAME_GET_TEXT_BY_NIDPROC X509_NAME_get_text_by_NIDProc;
	X509_GET_SUBJECT_NAMEPROC X509_get_subject_nameProc;
	SSL_METHODPROC SSL_server_methodProc;
	SSL_METHODPROC SSL_client_methodProc;
	SSL_CTRLPROC SSL_ctrlProc;
	SSL_GET_ERRORPROC SSL_get_errorProc;
	ERR_ERROR_STRING_NPROC ERR_error_string_nProc;
	ERR_GET_ERRORPROC ERR_get_errorProc;

	BIO_NEW_FILEPROC BIO_new_fileProc;
	BIO_FREEPROC BIO_freeProc;
	B2I_PVK_BIOPROC b2i_PVK_bioProc;
	SSL_CTX_GET_CERT_STOREPROC SSL_CTX_get_cert_storeProc;
	X509_STORE_ADD_CERTPROC X509_STORE_add_certProc;
	D2I_X509_BIOPROC d2i_X509_bioProc;

	WSDLL sslDLL;
	WSDLL cryptoDLL;
} WSSSLLOADER;
	wsbool WSSSLLoader_Create(WSSSLLOADER* pThis)
	{
		wsbool loaded;
#ifdef _WIN32
#ifdef WSWOLFSSLOPT
		if(!WSDLL_Create(&pThis->sslDLL, "wolfssl.dll"))
			loaded = FALSE;
		else if(!WSDLL_Create(&pThis->cryptoDLL, "wolfssl.dll"))
		{
			WSDLL_Destroy(&pThis->sslDLL);
			loaded = FALSE;
		}
		else
			loaded = TRUE;
		char* prefix1; char* prefix2;
		if (!loaded)
		{
#endif
			if(!WSDLL_Create(&pThis->sslDLL, "libssl-1_1-x64.dll"))
				loaded = FALSE;
			if(!WSDLL_Create(&pThis->cryptoDLL, "libcrypto-1_1-x64.dll"))
			{
				WSDLL_Destroy(&pThis->sslDLL);
				loaded = FALSE;
			}
			else
				loaded = TRUE;
			if (!loaded)
				return false;
#ifdef WSWOLFSSLOPT
			prefix1 = prefix2 = "";
		}
		else
		{
			prefix1 = "wolf";
			prefix2 = "wolfSSL_";
		}
#define OPENSSLPREFIX(x) (MutStr(prefix1) << x).c_str()
#define OPENSSLPREFIX2(x) (MutStr(prefix2) << x).c_str()
#else
#define OPENSSLPREFIX(x) x
#define OPENSSLPREFIX2(x) x
#endif
#else
#define OPENSSLPREFIX(x) x
#define OPENSSLPREFIX2(x) x
		if (!WSDLL_Create(&pThis->sslDLL, "libssl.so.1.1"))
			return false;
		if (!WSDLL_Create(&pThis->cryptoDLL, "libcrypto.so.1.1"))
		{
			WSDLL_Destroy(&pThis->sslDLL);
			return false;
		}
#endif /* _WIN32 */
		/*wsbool is11;*/
		pThis->SSL_server_methodProc = (SSL_METHODPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("TLS_server_method"));
		pThis->SSL_client_methodProc = (SSL_METHODPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("TLS_client_method"));
		if (!pThis->SSL_server_methodProc)
		{
			/*is11 = false;*/
			pThis->SSL_server_methodProc = (SSL_METHODPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSLv23_server_method"));
			pThis->SSL_client_methodProc = (SSL_METHODPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSLv23_client_method"));
		}
		/*else
			is11 = true;*/

		/* SSL */
		pThis->SSL_CTX_freeProc = (SSL_CTX_FREEPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_free"));
		pThis->SSL_CTX_newProc = (SSL_CTX_NEWPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_new"));
		pThis->SSL_CTX_ctrlProc = (SSL_CTX_CTRLPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_ctrl")); /* not in wolf */
		pThis->SSL_CTX_use_certificate_fileProc = (SSL_CTX_USE_CERTIFICATE_FILEPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_use_certificate_file"));
		pThis->SSL_CTX_use_certificate_chain_fileProc = (SSL_CTX_USE_CERTIFICATE_CHAIN_FILEPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_use_certificate_chain_file"));
		pThis->SSL_CTX_use_PrivateKeyProc = (SSL_CTX_USE_PRIVATEKEYPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_use_PrivateKey"));
		pThis->SSL_CTX_use_PrivateKey_fileProc = (SSL_CTX_USE_PRIVATEKEY_FILEPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_use_PrivateKey_file"));
		pThis->SSL_CTX_load_verify_locationsProc = (SSL_CTX_LOAD_VERIFY_LOCATIONSPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_load_verify_locations"));
		pThis->SSL_CTX_set_verifyProc = (SSL_CTX_SET_VERIFYPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_set_verify"));
		pThis->SSL_CTX_set_verify_depthProc = (SSL_CTX_SET_VERIFY_DEPTHPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_set_verify_depth"));
		pThis->SSL_CTX_set_timeoutProc = (SSL_CTX_SET_TIMEOUTPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_set_timeout"));
		/*SSL_CTX_set_default_passwd_cbProc = (SSL_CTX_SET_DEFAULT_PASSWD_CBPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX/("SSL_CTX_set_default_passwd_cb"));*/
		/*SSL_CTX_set_default_passwd_cb_userdataProc = (SSL_CTX_SET_DEFAULT_PASSWD_CB_USERDATAPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_set_default_passwd_cb_userdata"));*/
		pThis->SSL_newProc = (SSL_NEWPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_new"));
		pThis->SSL_freeProc = (SSL_FREEPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_free"));
		pThis->SSL_acceptProc = (SSL_ACCEPTPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_accept"));
		pThis->SSL_connectProc = (SSL_CONNECTPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_connect"));
		pThis->SSL_shutdownProc = (SSL_SHUTDOWNPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_shutdown"));
		pThis->SSL_set_fdProc = (SSL_SET_FDPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_set_fd"));
		pThis->OPENSSL_init_sslProc = (OPENSSL_INIT_SSLPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("OPENSSL_init_ssl")); /* not in wolf */
		pThis->SSL_write_exProc = (SSL_WRITE_EXPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_write_ex"));
		pThis->SSL_read_exProc = (SSL_READ_EXPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_read_ex"));
		pThis->SSL_writeProc = (SSL_WRITEPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_write"));
		pThis->SSL_readProc = (SSL_READPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_read"));
		pThis->SSL_pendingProc = (SSL_PENDINGPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_pending"));
		pThis->SSL_get_verify_resultProc = (SSL_GET_VERIFY_RESULTPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_get_verify_result"));
		pThis->SSL_get_peer_certificateProc = (SSL_GET_PEER_CERTIFICATEPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_get_peer_certificate"));
		pThis->SSL_ctrlProc = (SSL_CTRLPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_ctrl")); /* not in wolf */
		pThis->SSL_get_errorProc = (SSL_GET_ERRORPROC)WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_get_error"));
		pThis->SSL_CTX_get_cert_storeProc = (SSL_CTX_GET_CERT_STOREPROC)
			WSDLL_GetProc(&pThis->sslDLL, OPENSSLPREFIX("SSL_CTX_get_cert_store"));

		/* cryto */
		pThis->X509_NAME_get_text_by_NIDProc = (X509_NAME_GET_TEXT_BY_NIDPROC)WSDLL_GetProc(&pThis->cryptoDLL, OPENSSLPREFIX2("X509_NAME_get_text_by_NID"));
		pThis->X509_get_subject_nameProc = (X509_GET_SUBJECT_NAMEPROC)WSDLL_GetProc(&pThis->cryptoDLL, OPENSSLPREFIX2("X509_get_subject_name"));
		pThis->ERR_error_string_nProc = (ERR_ERROR_STRING_NPROC)WSDLL_GetProc(&pThis->cryptoDLL, OPENSSLPREFIX2("ERR_error_string_n"));
		pThis->ERR_get_errorProc = (ERR_GET_ERRORPROC)WSDLL_GetProc(&pThis->cryptoDLL, OPENSSLPREFIX2("ERR_get_error"));
		pThis->BIO_new_fileProc = (BIO_NEW_FILEPROC)WSDLL_GetProc(&pThis->cryptoDLL, OPENSSLPREFIX2("BIO_new_file"));
		pThis->BIO_freeProc = (BIO_FREEPROC)WSDLL_GetProc(&pThis->cryptoDLL, OPENSSLPREFIX2("BIO_free"));
		pThis->b2i_PVK_bioProc = (B2I_PVK_BIOPROC)WSDLL_GetProc(&pThis->cryptoDLL, OPENSSLPREFIX2("b2i_PVK_bio"));
		pThis->X509_STORE_add_certProc = (X509_STORE_ADD_CERTPROC)WSDLL_GetProc(&pThis->cryptoDLL, OPENSSLPREFIX2("X509_STORE_add_cert"));;
		pThis->d2i_X509_bioProc = (D2I_X509_BIOPROC)WSDLL_GetProc(&pThis->cryptoDLL, OPENSSLPREFIX2("d2i_X509_bio"));;

		/*SSL_load_error_strings() | SSL_library_init() (OPENSSL_VERSION_NUMBER < 0x1010001fL)*/
		/* ~() -> ERR_free_strings() < 0x1010001fL */
		if (pThis->OPENSSL_init_sslProc)
			return (*pThis->OPENSSL_init_sslProc)(
				INIT_LOAD_CRYPTO_STRINGS | INIT_LOAD_SSL_STRINGS, NULL) == 1;
		return true;
	}
	void WSSSLLoader_Destroy(WSSSLLOADER* pThis)
	{
		WSDLL_Destroy(&pThis->cryptoDLL);
		WSDLL_Destroy(&pThis->sslDLL);
	}

void OpenSSLOutputError(WSSSLLOADER* mSSLLoader, SSL* WSUNUSED(mSSL), int res)
{
#if 0
	int realError = (*mSSLLoader->SSL_get_errorProc)(mSSL, res);
	char sslError[120];
	unsigned long oldError;
	do
	{
		oldError = (*mSSLLoader->ERR_get_errorProc)();
		if (oldError != 0)
			(*mSSLLoader->ERR_error_string_nProc)(oldError, sslError, countof(sslError));
	} while (oldError != 0);
	WSLogDebug(MutStr("OpenSSL ERROR; ret:[") << res
		<< "] real:[" << realError << "] message:[" << sslError << "]");
#else
	unsigned long oldError;
	oldError = (*mSSLLoader->ERR_get_errorProc)();
	char sslError[120];
		(*mSSLLoader->ERR_error_string_nProc)(
			oldError, sslError, countof(sslError));
	WSLogDebug(MutStr("OpenSSL ERROR; ret:[") << res
		<< "] message:[" << sslError << "]");
#endif
}
void WSSocketSSL_Construct(WSSOCKETSSL* pThis)
{
	WSSocket_Construct(&pThis->mSuper);
	pThis->mSSL = (NULL);
}
void WSSocketSSL_Destruct(WSSOCKETSSL* pThis)
{
	WSSocketSSL_Destroy(pThis);
}
void WSSocketSSL_Own(WSSOCKETSSL* pThis, WSSOCKETSSL* pOther)
{
	WSMemoryCopy(pThis, pOther, sizeof(WSSOCKETSSL));
	WSSocketSSL_Construct(pOther);
}
void WSSocketSSL_Destroy(WSSOCKETSSL* pThis)
{
	/*There isnt really much of a difference between closing the socket before or after calling those functions.*/
	/*However, it is generally considered good practice to close the socket before releasing the memory used by the*/
	/*SSL objects because doing so ensures that all data sent over the network was properly encrypted and decrypted.*/
	if (pThis->mSSL)
	{
		(*pThis->mSSLLoader->SSL_shutdownProc)(pThis->mSSL);
		(*pThis->mSSLLoader->SSL_freeProc)(pThis->mSSL);
		(*pThis->mSSLLoader->SSL_CTX_freeProc)(pThis->mCtx);
		pThis->mCtx = NULL;
		pThis->mSSL = NULL;
	}
	WSSocket_Destroy(&pThis->mSuper);
}

wsbool WSSocketSSL_DoConnectOrAccept(WSSOCKETSSL* pThis,
	int (*connectacceptProc)(struct ssl_st*))
{
	int realError;
	char c;
	wsbool blocking = WSSOCKET_NONBLOCKING;
	(*pThis->mSSLLoader->SSL_set_fdProc)(pThis->mSSL,
		PTRCASTTYPE(int)WSSocketSSL_GetHandle(pThis));
	if (!WSSocket_WaitForRead(&pThis->mSuper, 1000000, NULL)
		|| WSSocket_Peek(&pThis->mSuper, &c, 1, &blocking) == (size_t)SOCKET_ERROR)
	{
		WSSocketSSL_Destroy(pThis);
		return false;
	}
	if (CStrLocateCharWithLength("DGHOPT", c, strcountof("DGHOPT"))) /* CUPS-like HTTP check */
	{
		(*pThis->mSSLLoader->SSL_freeProc)(pThis->mSSL);
		(*pThis->mSSLLoader->SSL_CTX_freeProc)(pThis->mCtx);
		pThis->mSSL = NULL;
		pThis->mCtx = NULL;
		return true; /* regular HTTP instead of HTTPS */
	}
	do
	{
		int acceptRes = (*connectacceptProc)(pThis->mSSL);
		if (acceptRes != 1)
		{
			realError = (*pThis->mSSLLoader->SSL_get_errorProc)(
				pThis->mSSL, acceptRes);
			if (realError != ERROR_WANT_READ
				&& realError != ERROR_WANT_WRITE
#ifndef _WIN32
				&& !(realError == ERROR_SYSCALL && WSSocket_Error(pThis) == EINTR)
#endif
				)
			{ /* assume ERROR_SSL == regular HTTP */
				if (realError != ERROR_SSL)
				{
					OpenSSLOutputError(pThis->mSSLLoader, pThis->mSSL, acceptRes);
					(*pThis->mSSLLoader->SSL_shutdownProc)(pThis->mSSL);
				}
				(*pThis->mSSLLoader->SSL_freeProc)(pThis->mSSL);
				(*pThis->mSSLLoader->SSL_CTX_freeProc)(pThis->mCtx);
				pThis->mSSL = NULL;
				pThis->mCtx = NULL;
				if (realError != ERROR_SSL)
					return false;
			}
		}
		else
			realError = 0;
	} while ((realError == ERROR_WANT_READ
				&& WSSocket_WaitForRead(&pThis->mSuper, 1000000, NULL))
		|| (realError == ERROR_WANT_WRITE
			&& WSSocket_WaitForWrite(&pThis->mSuper, 1000000, NULL)));
	/* Check that the common name matches the host name */
	if (pThis->mSSL)
	{
		X509* peer;
		char peer_CN[256];
		if ((*pThis->mSSLLoader->SSL_get_verify_resultProc)(pThis->mSSL) != X509_V_OK)
			return false; /* certificate doesn't verify */
		peer = (*pThis->mSSLLoader->SSL_get_peer_certificateProc)(pThis->mSSL);
		if (peer)
		{
			(*pThis->mSSLLoader->X509_NAME_get_text_by_NIDProc)(
				(*pThis->mSSLLoader->X509_get_subject_nameProc)(peer),
				NID_commonName, peer_CN, 256);
			if (AnsiiCompare(peer_CN, "localhost", WSMAXLENGTH, strcountof("localhost")))
				return false; /* Common name doesn't match host name */
		}
	}
	return realError != ERROR_WANT_READ
		&& realError != ERROR_WANT_WRITE;
}


wsbool WSSocketSSL_Accept(WSSOCKETSSL* pThis, const WSSOCKETSSL* listen,
	WSSOCKETLIBRARY* socklib)
{
	if (!WSSocket_Accept(&pThis->mSuper, &listen->mSuper, socklib))
		return false;
	pThis->mSSLLoader = WSSocketLibrary_GetSSLLoader(socklib);
	if (!pThis->mSSLLoader)
		return true;
	pThis->mCtx = NULL;
	pThis->mSSL = (*pThis->mSSLLoader->SSL_newProc)(listen->mCtx);
	if (!pThis->mSSL)
		return false;
	return WSSocketSSL_DoConnectOrAccept(pThis, pThis->mSSLLoader->SSL_acceptProc);
}

int OpenSSLPasswordProc(char *buf, int size, int rwflag, void *userdata)
{
	const char* inPassword = (const char*)userdata;
	size_t inPasswordLen = CStrLength(inPassword);
	int outLen;
	WSASSERT(rwflag == 0); /* reading */
	if(inPasswordLen > size - 1)
		outLen = size - 1;
	else
		outLen = (int)inPasswordLen;
	WSMemoryCopy(buf, inPassword, outLen);
	buf[outLen] = '\0';
	return (int)(inPasswordLen); /* OpenSSL wants the null char in the count */
}

wsbool WSSocketSSL_CreateClient(WSSOCKETSSL* pThis, const char* szHostname,
	size_t port, WSSOCKETLIBRARY* socklib,
	const char* certPath, const char* keyPath)
{
	if (!WSSocket_CreateClient(&pThis->mSuper, szHostname, port, socklib))
		return false;
	pThis->mSSLLoader = WSSocketLibrary_GetSSLLoader(socklib);
	if (!pThis->mSSLLoader)
		return true;
	pThis->mCtx = (*pThis->mSSLLoader->SSL_CTX_newProc)(
		(*pThis->mSSLLoader->SSL_client_methodProc)());
	if (pThis->mCtx)
	{
		(*pThis->mSSLLoader->SSL_CTX_set_timeoutProc)(
			pThis->mCtx, 600); /* libcrypto preference */
		if (pThis->mSSLLoader->SSL_CTX_ctrlProc) /* not in wolfssl so need check */
			(*pThis->mSSLLoader->SSL_CTX_ctrlProc)(pThis->mCtx,
				CTRL_SET_SESS_CACHE_MODE,
				SESS_CACHE_CLIENT, NULL); /* enable client caching */
		if (certPath && keyPath)
		{
			if ((*pThis->mSSLLoader->SSL_CTX_use_certificate_fileProc)(
				pThis->mCtx, certPath, X509_FILETYPE_PEM) != 1
				|| (*pThis->mSSLLoader->SSL_CTX_use_PrivateKey_fileProc)(
					pThis->mCtx, keyPath, X509_FILETYPE_PEM) != 1)
			{
				(*pThis->mSSLLoader->SSL_CTX_freeProc)(pThis->mCtx);
				pThis->mCtx = NULL;
				return false;
			}
		}
	}
	pThis->mSSL = (*pThis->mSSLLoader->SSL_newProc)(pThis->mCtx);
	if (!pThis->mSSL)
	{
		(*pThis->mSSLLoader->SSL_CTX_freeProc)(pThis->mCtx);
		pThis->mCtx = NULL;
		return false;
	}
	if (pThis->mSSLLoader->SSL_ctrlProc) /* not in wolfssl so need check */
		(*pThis->mSSLLoader->SSL_ctrlProc)(
			pThis->mSSL, 55, 0, (void*)szHostname);
	return WSSocketSSL_DoConnectOrAccept(pThis, pThis->mSSLLoader->SSL_connectProc);
}

wsbool WSSocketSSL_CreateServer(WSSOCKETSSL* pThis, size_t port,
	const char* certPath, const char* keyPath, WSSOCKETLIBRARY* socklib,
	const char* CAFilePath, const char* CADirPath)
{
	if (!WSSocket_CreateServer(&pThis->mSuper, port, socklib))
		return false;
	pThis->mSSLLoader = WSSocketLibrary_GetSSLLoader(socklib);
/*	WSASSERT(pThis->mSSLLoader);*/
	if(!pThis->mSSLLoader)
		return true;
	pThis->mCtx = (*pThis->mSSLLoader->SSL_CTX_newProc)(
		(*pThis->mSSLLoader->SSL_server_methodProc)());
	if (pThis->mCtx)
	{
/*		(*pThis->mSSLLoader->SSL_CTX_set_default_passwd_cb_userdataProc)(pThis->mCtx, (void*)"Test123");*/
/*		(*pThis->mSSLLoader->SSL_CTX_set_default_passwd_cbProc)(pThis->mCtx, OpenSSLPasswordProc);*/
		int publicCertSuccess =
			(*pThis->mSSLLoader->SSL_CTX_use_certificate_chain_fileProc)(
				pThis->mCtx, certPath);
		if(!publicCertSuccess)
			publicCertSuccess =
				(*pThis->mSSLLoader->SSL_CTX_use_certificate_fileProc)(
					pThis->mCtx, certPath, X509_FILETYPE_PEM);
		if(!publicCertSuccess)
			publicCertSuccess =
				(*pThis->mSSLLoader->SSL_CTX_use_certificate_fileProc)(
					pThis->mCtx, certPath, X509_FILETYPE_ASN1);
		int privateKeySuccess =
				(*pThis->mSSLLoader->SSL_CTX_use_PrivateKey_fileProc)(
					pThis->mCtx, keyPath, X509_FILETYPE_PEM);
		if(!privateKeySuccess)
			privateKeySuccess =
				(*pThis->mSSLLoader->SSL_CTX_use_PrivateKey_fileProc)(
					pThis->mCtx, keyPath, X509_FILETYPE_ASN1);
		if(!privateKeySuccess && pThis->mSSLLoader->b2i_PVK_bioProc)
		{
			BIO* pvkBio = (*pThis->mSSLLoader->BIO_new_fileProc)(keyPath, "rb");
			if(pvkBio)
			{
				EVP_PKEY* pvkEvp =
					(*pThis->mSSLLoader->b2i_PVK_bioProc)(
						pvkBio, OpenSSLPasswordProc, (void*)"Test123");
				if(pvkEvp)
					privateKeySuccess =
						(*pThis->mSSLLoader->SSL_CTX_use_PrivateKeyProc)(
							pThis->mCtx, pvkEvp);
				(*pThis->mSSLLoader->BIO_freeProc)(pvkBio);
			}
		}
		if(!publicCertSuccess || !privateKeySuccess)
		{
			OpenSSLOutputError(pThis->mSSLLoader, NULL, privateKeySuccess);
			WSSocketSSL_Destroy(pThis);
			return false;
		}
		else if (CAFilePath || CADirPath)
		{
			if(!(*pThis->mSSLLoader->SSL_CTX_load_verify_locationsProc)(
					pThis->mCtx, CAFilePath, CADirPath))
			{
				BIO* caBio = (*pThis->mSSLLoader->BIO_new_fileProc)(CAFilePath, "rb");
				if(caBio)
				{ /* https://stackoverflow.com/questions/5052563/c-openssl-use-root-ca-from-buffer-rather-than-file-ssl-ctx-load-verify-locat */
					X509_STORE* store = (*pThis->mSSLLoader->SSL_CTX_get_cert_storeProc)(pThis->mCtx);
					X509* ca509ret = (*pThis->mSSLLoader->d2i_X509_bioProc)(caBio, NULL);
					(*pThis->mSSLLoader->X509_STORE_add_certProc)(store, ca509ret);
					(*pThis->mSSLLoader->BIO_freeProc)(caBio);
				}
				else
				{
					OpenSSLOutputError(pThis->mSSLLoader, NULL, privateKeySuccess);
					/*(*pThis->mSSLLoader->SSL_CTX_freeProc)(pThis->mCtx);*/
					/*pThis->mCtx = NULL;*/
				}
			}
			/*(*pThis->mSSLLoader->SSL_CTX_set_verifyProc)(pThis->mCtx,*/
			/*	VERIFY_PEER | VERIFY_FAIL_IF_NO_PEER_CERT, NULL);*/
		}
		else
		{
			if (!(*pThis->mSSLLoader->SSL_CTX_load_verify_locationsProc)(
				pThis->mCtx, certPath, NULL))
			{
				(*pThis->mSSLLoader->SSL_CTX_freeProc)(pThis->mCtx);
				pThis->mCtx = NULL;
			}
			else
				(*pThis->mSSLLoader->SSL_CTX_set_verify_depthProc)(pThis->mCtx, 1);
		}
	}
	else
	{
		WSSocketSSL_Destroy(pThis);
		return false;
	}
	pThis->mSSL = (*pThis->mSSLLoader->SSL_newProc)(pThis->mCtx);
	if (!pThis->mSSL)
	{
		WSSocketSSL_Destroy(pThis);
		return false;
	}
	return true;
}

size_t WSSocketSSL_Read(const WSSOCKETSSL* pThis,
	void* data, size_t num, wsbool* inblock)
{
	if (!WSSocketSSL_IsSSL(pThis))
		return WSSocket_Read(&pThis->mSuper, data, num, inblock);
	size_t pos = 0, uNumRead = 0;
	int waited = 0;
	int res, realError;
	do
	{
		if (pThis->mSSLLoader->SSL_read_exProc)
			res = (*pThis->mSSLLoader->SSL_read_exProc)(pThis->mSSL, &((char*)data)[pos], num - pos, &uNumRead);
		else
			res = (*pThis->mSSLLoader->SSL_readProc)(pThis->mSSL, &((char*)data)[pos], (int)(num - pos));
		if (res <= 0)
		{
			realError = (*pThis->mSSLLoader->SSL_get_errorProc)(pThis->mSSL, res);
			if (!(realError == ERROR_WANT_READ
				|| realError == ERROR_WANT_WRITE))
			{
				OpenSSLOutputError(pThis->mSSLLoader, pThis->mSSL, res);
				return WSMAXSIZE;
			}
			else
			{
				if ((inblock && *inblock == WSSOCKET_NONBLOCKING)
					|| waited > 10000)
				{
					if (pos == 0)
					{
						if(inblock)
						{
							int lastError = (*pThis->mSSLLoader->SSL_get_errorProc)(
								pThis->mSSL, 0);
							if (lastError == ERROR_WANT_READ
								|| lastError == ERROR_WANT_WRITE)
								*inblock = true;
						}
						return WSMAXSIZE;
					}
					break;
				}
			}
			waited++;
			uNumRead = 0;
		}
		else
		{
			if (!pThis->mSSLLoader->SSL_read_exProc)
				uNumRead = res;
			realError = 0;
		}
		pos += uNumRead;
	} while (realError == ERROR_WANT_READ
		|| realError == ERROR_WANT_WRITE);
	return pos;
}

size_t WSSocketSSL_Write(const WSSOCKETSSL* pThis,
	const void* data, size_t num, wsbool* inblock)
{
	if (!WSSocketSSL_IsSSL(pThis))
		return WSSocket_Write(&pThis->mSuper, data, num, inblock);
	size_t pos = 0, uNumRead = 0;
	wsbool waited = false;
	int res, realError;
	do
	{
		if (pThis->mSSLLoader->SSL_write_exProc)
			res = (*pThis->mSSLLoader->SSL_write_exProc)(pThis->mSSL, &((char*)data)[pos], num - pos, &uNumRead);
		else
			res = (*pThis->mSSLLoader->SSL_writeProc)(pThis->mSSL, &((char*)data)[pos], (int)(num - pos));
		if (res <= 0)
		{
			realError = (*pThis->mSSLLoader->SSL_get_errorProc)(pThis->mSSL, res);
			if (!(realError == ERROR_WANT_READ
				|| realError == ERROR_WANT_WRITE))
			{
				OpenSSLOutputError(pThis->mSSLLoader, pThis->mSSL, res);
				return WSMAXSIZE;
			}
			else if ((inblock && *inblock == WSSOCKET_NONBLOCKING)
					 || waited)
			{
				if (pos == 0)
				{
					if(inblock)
					{
						int lastError = (*pThis->mSSLLoader->SSL_get_errorProc)(pThis->mSSL, 0);
						if (lastError == ERROR_WANT_READ
							|| lastError == ERROR_WANT_WRITE)
							*inblock = true;
					}
					return WSMAXSIZE;
				}
				realError = 0; /* error */
			}
			waited = true;
			uNumRead = 0;
		}
		else
		{
			if (!pThis->mSSLLoader->SSL_write_exProc)
				uNumRead = res;
			realError = 0;
		}
		pos += uNumRead;
	} while (realError == ERROR_WANT_READ
		|| realError == ERROR_WANT_WRITE);
	return pos;
}
wsbool WSSocketSSL_IsCreated(const WSSOCKETSSL* pThis)
{
	return WSSocketSSL_IsSSL(pThis) || (!WSSocketSSL_IsSSL(pThis) && WSSocket_IsCreated(&pThis->mSuper));
}
wsbool WSSocketSSL_IsSSL(const WSSOCKETSSL* pThis)
{
	return pThis->mSSL != NULL;
}

#endif /* defined(WSSSLOPT) && (!defined(_WIN32) || !defined(WSNTVSSLOPT)) */
/*=============================================================================
	  #####   #####  #     #    #    #     # #     # ####### #
	 #     # #     # #     #   # #   ##    # ##    # #       #
	 #       #       #     #  #   #  # #   # # #   # #       #
	  #####  #       ####### #     # #  #  # #  #  # #####   #
		   # #       #     # ####### #   # # #   # # #       #
	 #     # #     # #     # #     # #    ## #    ## #       #
	  #####   #####  #     # #     # #     # #     # ####### #######

=============================================================================*/
#if defined(WSSSLOPT) && defined(_WIN32) && defined(WSNTVSSLOPT)
#if defined(__VECTOR_H__)
DECLARE_RAWVECTOR(BYTEVector, unsigned char)
#endif
#define DECLARE_SCOPED_POINTER(NAME, TYPE) \
class NAME \
{ \
public: \
	NAME(){} NAME(TYPE* data) : mData(data) {} \
	NAME(size_t size) : mData(WSALLOC(TYPE, size)) {} \
	~NAME() { WSFREE(mData); } bool IsCreated() { return mData != NULL; } \
	void Reserve(size_t size) { \
		TYPE* newData = WSREALLOC(mData, TYPE, size); \
		if(!newData) { WSFREE(mData); } mData = newData; } \
	void operator = (TYPE* data) { mData = data; } \
	operator TYPE*() { return mData; } void Free() {} \
	TYPE** operator&() { return &mData; } \
protected: \
	TYPE* mData; \
};

DECLARE_SCOPED_POINTER(ScopedBytes, uint8_t)

#define SECURITY_WIN32
#include <sspi.h>
#include <wincrypt.h>
#include <wchar.h>
#if defined(WSMSIXOPT)
	#pragma comment (lib, "secur32.lib")
	#pragma comment (lib, "crypt32.lib")
#endif
#ifndef SEC_I_CONTEXT_EXPIRED /* MW CodeWarrior doesn't have */
	#define SEC_I_CONTEXT_EXPIRED			0x00090317L
#endif
#ifndef SP_PROT_SSL2_SERVER
	#define SP_PROT_SSL2_SERVER    0x00000004
	#define SP_PROT_SSL2_CLIENT    0x00000008
	#define SP_PROT_SSL3_SERVER    0x00000010
	#define SP_PROT_SSL3_CLIENT    0x00000020
	#define SP_PROT_TLS1_SERVER    0x00000040
	#define SP_PROT_TLS1_CLIENT    0x00000080
	#define SP_PROT_TLS1_1_SERVER     0x00000100
	#define SP_PROT_TLS1_1_CLIENT     0x00000200
	#define SP_PROT_TLS1_2_SERVER     0x00000400
	#define SP_PROT_TLS1_2_CLIENT     0x00000800
	typedef struct _SCHANNEL_CRED
	{
		DWORD     dwVersion;
		DWORD     cCreds;
		PCCERT_CONTEXT * paCred;
		HCERTSTORE   hRootStore;
		DWORD     cMappers;
		struct _HMAPPER **  aphMappers;
		DWORD     cSupportedAlgs;
		ALG_ID *  palgSupportedAlgs;
		DWORD     grbitEnabledProtocols;
		DWORD     dwMinimumCipherStrength;
		DWORD     dwMaximumCipherStrength;
		DWORD     dwSessionLifespan;
		DWORD     dwFlags;
		DWORD     reserved;
	} SCHANNEL_CRED, *PSCHANNEL_CRED;
	#define SCHANNEL_CRED_VERSION 0x00000004
	#define SCH_CRED_NO_DEFAULT_CREDS     0x00000010
	#define SCH_CRED_NO_SYSTEM_MAPPER     0x00000002
	#define SCH_CRED_REVOCATION_CHECK_CHAIN     0x00000200
	#define SP_PROT_SSL2_SERVER    0x00000004
	#define SP_PROT_SSL2_CLIENT    0x00000008
	#define SP_PROT_SSL2     (SP_PROT_SSL2_SERVER | SP_PROT_SSL2_CLIENT)
	#define SP_PROT_SSL3_SERVER    0x00000010
	#define SP_PROT_SSL3_CLIENT    0x00000020
	#define SP_PROT_SSL3     (SP_PROT_SSL3_SERVER | SP_PROT_SSL3_CLIENT)
	#define SP_PROT_TLS1_SERVER    0x00000040
	#define SP_PROT_TLS1_CLIENT    0x00000080
	#define SP_PROT_TLS1     (SP_PROT_TLS1_SERVER | SP_PROT_TLS1_CLIENT)
	#define SP_PROT_TLS1_1_SERVER     0x00000100
	#define SP_PROT_TLS1_1_CLIENT     0x00000200
	#define SP_PROT_TLS1_1      (SP_PROT_TLS1_1_SERVER | \
			   SP_PROT_TLS1_1_CLIENT)
	#define SP_PROT_TLS1_2_SERVER     0x00000400
	#define SP_PROT_TLS1_2_CLIENT     0x00000800
	#define SP_PROT_TLS1_2      (SP_PROT_TLS1_2_SERVER | \
			   SP_PROT_TLS1_2_CLIENT)
	#if 0
	typedef struct _SECURITY_FUNCTION_TABLE_A {
		unsigned long      dwVersion;
		ENUMERATE_SECURITY_PACKAGES_FN_A  EnumerateSecurityPackagesA;
		QUERY_CREDENTIALS_ATTRIBUTES_FN_A QueryCredentialsAttributesA;
		ACQUIRE_CREDENTIALS_HANDLE_FN_A   AcquireCredentialsHandleA;
		FREE_CREDENTIALS_HANDLE_FN     FreeCredentialHandle;
		void         *Reserved2;
		INITIALIZE_SECURITY_CONTEXT_FN_A  InitializeSecurityContextA;
		ACCEPT_SECURITY_CONTEXT_FN     AcceptSecurityContext;
		COMPLETE_AUTH_TOKEN_FN      CompleteAuthToken;
		DELETE_SECURITY_CONTEXT_FN     DeleteSecurityContext;
		APPLY_CONTROL_TOKEN_FN      ApplyControlToken;
		QUERY_CONTEXT_ATTRIBUTES_FN_A  QueryContextAttributesA;
		IMPERSONATE_SECURITY_CONTEXT_FN   ImpersonateSecurityContext;
		REVERT_SECURITY_CONTEXT_FN     RevertSecurityContext;
		MAKE_SIGNATURE_FN     MakeSignature;
		VERIFY_SIGNATURE_FN      VerifySignature;
		FREE_CONTEXT_BUFFER_FN      FreeContextBuffer;
		QUERY_SECURITY_PACKAGE_INFO_FN_A  QuerySecurityPackageInfoA;
		void         *Reserved3;
		void         *Reserved4;
		EXPORT_SECURITY_CONTEXT_FN     ExportSecurityContext;
		IMPORT_SECURITY_CONTEXT_FN_A   ImportSecurityContextA;
		ADD_CREDENTIALS_FN_A     AddCredentialsA;
		void         *Reserved8;
		QUERY_SECURITY_CONTEXT_TOKEN_FN   QuerySecurityContextToken;
		ENCRYPT_MESSAGE_FN       EncryptMessage;
		DECRYPT_MESSAGE_FN       DecryptMessage;
		SET_CONTEXT_ATTRIBUTES_FN_A    SetContextAttributesA;
		SET_CREDENTIALS_ATTRIBUTES_FN_A   SetCredentialsAttributesA;
		CHANGE_PASSWORD_FN_A     ChangeAccountPasswordA;
		void         *Reserved9;
		QUERY_CONTEXT_ATTRIBUTES_EX_FN_A  QueryContextAttributesExA;
		QUERY_CREDENTIALS_ATTRIBUTES_EX_FN_A QueryCredentialsAttributesExA;
	} SecurityFunctionTableA, *PSecurityFunctionTableA;
	#endif /* sectable */
#endif
typedef struct _CRYPTUI_SELECTCERTIFICATE_STRUCT {
	DWORD      dwSize;
	HWND    hwndParent;
	DWORD      dwFlags;
	LPCSTR    szTitle;
	DWORD      dwDontUseColumn;
	LPCSTR    szDisplayString;
	LPWSDLLPROC			pFilterCallback;
	LPWSDLLPROC			pDisplayCallback;
	void    *pvCallbackData;
	DWORD      cDisplayStores;
	HCERTSTORE    *rghDisplayStores;
	DWORD      cStores;
	HCERTSTORE    *rghStores;
	DWORD      cPropSheetPages;
	void* rgPropSheetPages;
	HCERTSTORE    hSelectedCertStore;
} CRYPTUI_SELECTCERTIFICATE_STRUCT, *PCRYPTUI_SELECTCERTIFICATE_STRUCT;
typedef PCCERT_CONTEXT(WINAPI* CRYPTUIDLGSELECTCERTIFICATEPROC)(
	PCRYPTUI_SELECTCERTIFICATE_STRUCT pcsc);


typedef SECURITY_STATUS(SEC_ENTRY *ACCEPTSECURITYCONTEXTPROC)(
	PCredHandle phCredential,
	PCtxtHandle phContext,
	PSecBufferDesc pInput,
	unsigned long  fContextReq,
	unsigned long  TargetDataRep,
	PCtxtHandle phNewContext,
	PSecBufferDesc pOutput,
	unsigned long  *pfContextAttr,
	PTimeStamp  ptsExpiry
	);
typedef SECURITY_STATUS(SEC_ENTRY *ACQUIRECREDENTIALSHANDLEAPROC)(
	LPSTR    pszPrincipal,
	LPSTR    pszPackage,
	unsigned long  fCredentialUse,
	void     *pvLogonId,
	void     *pAuthData,
	SEC_GET_KEY_FN pGetKeyFn,
	void     *pvGetKeyArgument,
	PCredHandle phCredential,
	PTimeStamp  ptsExpiry
	);
typedef SECURITY_STATUS(SEC_ENTRY *COMPLETEAUTHTOKENPROC)(
	PCtxtHandle phContext,
	PSecBufferDesc pToken
	);
typedef SECURITY_STATUS(SEC_ENTRY *FREECREDENTIALSHANDLEPROC)(
	PCredHandle phCredential
	);
typedef SECURITY_STATUS(SEC_ENTRY *QUERYCONTEXTATTRIBUTESAPROC)(
	PCtxtHandle   phContext,
	unsigned long ulAttribute,
	void    *pBuffer
	);
typedef SECURITY_STATUS(SEC_ENTRY *INITIALIZESECURITYCONTEXTAPROC)(
	PCredHandle phCredential,
	PCtxtHandle phContext,
	SEC_CHAR    *pszTargetName,
	unsigned long  fContextReq,
	unsigned long  Reserved1,
	unsigned long  TargetDataRep,
	PSecBufferDesc pInput,
	unsigned long  Reserved2,
	PCtxtHandle phNewContext,
	PSecBufferDesc pOutput,
	unsigned long  *pfContextAttr,
	PTimeStamp  ptsExpiry
	);
typedef SECURITY_STATUS(SEC_ENTRY *FREECONTEXTBUFFERPROC)(PVOID pvContextBuffer);
typedef SECURITY_STATUS(SEC_ENTRY *DELETESECURITYCONTEXTPROC)(PCtxtHandle phContext);
typedef SECURITY_STATUS(SEC_ENTRY *DECRYPTMESSAGEPROC)(
	PCtxtHandle phContext,
	PSecBufferDesc pMessage,
	unsigned long  MessageSeqNo,
	unsigned long  *pfQOP
	);
typedef SECURITY_STATUS(SEC_ENTRY *ENCRYPTMESSAGEPROC)(
	PCtxtHandle phContext,
	unsigned long  fQOP,
	PSecBufferDesc pMessage,
	unsigned long  MessageSeqNo
	);
typedef BOOL(WINAPI* CERTCLOSESTOREPROC)(
	HCERTSTORE hCertStore,
	DWORD   dwFlags
	);
typedef PCCERT_CONTEXT(WINAPI* CERTFINDCERTIFICATEINSTOREPROC)(
	HCERTSTORE  hCertStore,
	DWORD    dwCertEncodingType,
	DWORD    dwFindFlags,
	DWORD    dwFindType,
	const void  *pvFindPara,
	PCCERT_CONTEXT pPrevCertContext
	);
typedef BOOL(WINAPI* CERTFREECERTIFICATECONTEXTPROC)(
	PCCERT_CONTEXT pCertContext
	);
typedef HCERTSTORE(WINAPI* CERTOPENSTOREPROC)(
	LPCSTR   lpszStoreProvider,
	DWORD    dwEncodingType,
	ULONG_PTR			hCryptProv,
	DWORD    dwFlags,
	const void  *pvPara
	);
typedef HCERTSTORE(WINAPI* CERTOPENSYSTEMSTOREPROC)(
	HCRYPTPROV hProv,
	LPCSTR szSubsystemProtocol
	);
typedef LONG (WINAPI* CERTVERIFYTIMEVALIDITYPROC)(
	LPFILETIME pTimeToVerify,
	PCERT_INFO pCertInfo
	);
typedef VOID (WINAPI* CERTFREECERTIFICATECHAINPROC)(
	PCCERT_CHAIN_CONTEXT pChainContext
	);
typedef BOOL (WINAPI* CERTGETCERTIFICATECHAINPROC)(
		HCERTCHAINENGINE hChainEngine,
		PCCERT_CONTEXT pCertContext,
		LPFILETIME pTime,
		HCERTSTORE hAdditionalStore,
		PCERT_CHAIN_PARA pChainPara,
		DWORD dwFlags,
		LPVOID pvReserved,
		PCCERT_CHAIN_CONTEXT* ppChainContext
	);
typedef BOOL(WINAPI* CERTVERIFYCERTIFICATECHAINPOLICYPROC)(
		LPCSTR pszPolicyOID,
		PCCERT_CHAIN_CONTEXT pChainContext,
		PCERT_CHAIN_POLICY_PARA pPolicyPara,
		PCERT_CHAIN_POLICY_STATUS pPolicyStatus
	);
typedef BOOL (WINAPI* CERTVERIFYREVOCATIONPROC)(
		DWORD dwEncodingType,
		DWORD dwRevType,
		DWORD cContext,
		PVOID rgpvContext[],
		DWORD dwFlags,
		PCERT_REVOCATION_PARA pRevPara,
		PCERT_REVOCATION_STATUS pRevStatus
	);
typedef SECURITY_STATUS (WINAPI* APPLYCONTROLTOKENPROC)(
		PCtxtHandle phContext,
		PSecBufferDesc pInput
	);
typedef BOOL (WINAPI* CRYPTSTRINGTOBINARYAPROC)(
	LPCSTR pszString,DWORD cchString,DWORD dwFlags,
	BYTE *pbBinary,DWORD *pcbBinary,DWORD *pdwSkip,DWORD *pdwFlags);
typedef BOOL (WINAPI* CRYPTDECODEOBJECTPROC)(
	DWORD dwCertEncodingType,LPCSTR lpszStructType,
	const BYTE *pbEncoded,DWORD cbEncoded,DWORD dwFlags,
	void *pvStructInfo,DWORD *pcbStructInfo);
typedef BOOL (WINAPI* CERTSETCERTIFICATECONTEXTPROPERTYPROC)(
	PCCERT_CONTEXT pCertContext, DWORD dwPropId,DWORD dwFlags,const void *pvData);
typedef PCCERT_CONTEXT (WINAPI* CERTCREATECERTIFICATECONTEXTPROC)(
	DWORD dwCertEncodingType,const BYTE *pbCertEncoded,DWORD cbCertEncoded);


typedef BOOL (WINAPI* CRYPTACQUIRECONTEXTAPROC)(
	HCRYPTPROV *phProv,LPCSTR szContainer,LPCSTR szProvider,
	DWORD dwProvType,DWORD dwFlags);
typedef BOOL (WINAPI* CRYPTIMPORTKEYPROC)(
	HCRYPTPROV hProv,CONST BYTE *pbData,DWORD dwDataLen,
	HCRYPTKEY hPubKey,DWORD dwFlags,HCRYPTKEY *phKey);
typedef BOOL (WINAPI* CRYPTDERIVEKEYPROC)(
	HCRYPTPROV hProv,ALG_ID Algid,HCRYPTHASH hBaseData,
	DWORD dwFlags,HCRYPTKEY *phKey);
typedef BOOL (WINAPI* CRYPTGETKEYPARAMPROC)(
	HCRYPTKEY hKey,DWORD dwParam,BYTE *pbData,DWORD *pdwDataLen,DWORD dwFlags);
typedef BOOL (WINAPI* CRYPTGETPROVPARAMPROC)(
	HCRYPTPROV hProv,DWORD dwParam,BYTE *pbData,DWORD *pdwDataLen,DWORD dwFlags);
typedef BOOL (WINAPI* CRYPTCREATEHASHPROC)(
	HCRYPTPROV hProv,ALG_ID Algid,HCRYPTKEY hKey,DWORD dwFlags,HCRYPTHASH *phHash);
typedef BOOL (WINAPI* CRYPTDESTROYHASHPROC)(HCRYPTHASH hHash);
typedef BOOL (WINAPI* CRYPTDESTROYKEYPROC)(HCRYPTKEY hKey);
typedef BOOL (WINAPI* CRYPTHASHDATAPROC)(
	HCRYPTHASH hHash,CONST BYTE *pbData,DWORD dwDataLen,DWORD dwFlags);

typedef struct WSSSLLOADER
{
	CRYPTHASHDATAPROC CryptHashDataProc;
	CRYPTDESTROYKEYPROC CryptDestroyKeyProc;
	CRYPTDESTROYHASHPROC CryptDestroyHashProc;
	CRYPTCREATEHASHPROC CryptCreateHashProc;
	CRYPTGETPROVPARAMPROC CryptGetProvParamProc;
	CRYPTGETKEYPARAMPROC CryptGetKeyParamProc;
	CRYPTDERIVEKEYPROC CryptDeriveKeyProc;
	CRYPTIMPORTKEYPROC CryptImportKeyProc;
	CRYPTACQUIRECONTEXTAPROC CryptAcquireContextAProc;
	CRYPTDECODEOBJECTPROC CryptDecodeObjectProc;
	CRYPTSTRINGTOBINARYAPROC CryptStringToBinaryAProc;
	ACCEPTSECURITYCONTEXTPROC AcceptSecurityContextProc;
	ACQUIRECREDENTIALSHANDLEAPROC AcquireCredentialsHandleAProc;
	COMPLETEAUTHTOKENPROC CompleteAuthTokenProc;
	FREECREDENTIALSHANDLEPROC FreeCredentialsHandleProc;
	QUERYCONTEXTATTRIBUTESAPROC QueryContextAttributesAProc;
	INITIALIZESECURITYCONTEXTAPROC InitializeSecurityContextAProc;
	FREECONTEXTBUFFERPROC FreeContextBufferProc;
	DELETESECURITYCONTEXTPROC DeleteSecurityContextProc;
	DECRYPTMESSAGEPROC DecryptMessageProc;
	ENCRYPTMESSAGEPROC EncryptMessageProc;
	CERTCLOSESTOREPROC CertCloseStoreProc;
	CERTFINDCERTIFICATEINSTOREPROC CertFindCertificateInStoreProc;
	CERTCREATECERTIFICATECONTEXTPROC CertCreateCertificateContextProc;
	CERTFREECERTIFICATECONTEXTPROC CertFreeCertificateContextProc;
	CERTSETCERTIFICATECONTEXTPROPERTYPROC CertSetCertificateContextPropertyProc;
	CERTOPENSTOREPROC CertOpenStoreProc;
	CERTOPENSYSTEMSTOREPROC CertOpenSystemStoreProc;
	CERTVERIFYTIMEVALIDITYPROC CertVerifyTimeValidityProc;
	CERTFREECERTIFICATECHAINPROC CertFreeCertificateChainProc;
	CERTGETCERTIFICATECHAINPROC CertGetCertificateChainProc;
	CERTVERIFYCERTIFICATECHAINPOLICYPROC CertVerifyCertificateChainPolicyProc;
	CERTVERIFYREVOCATIONPROC CertVerifyRevocationProc;
	APPLYCONTROLTOKENPROC ApplyControlTokenProc;

	WSDLL secur32DLL;
	WSDLL crypt32DLL;
	WSDLL advapi32DLL;
	PCCERT_CONTEXT mpCertContext;
} WSSSLLOADER;
	wsbool WSSSLLoader_Create(WSSSLLOADER* pThis)
	{
		if (!WSDLL_CreateSecur32(&pThis->secur32DLL))
			return false;
		if (!WSDLL_CreateCrypt32(&pThis->crypt32DLL))
		{
			WSDLL_DestroySecur32(&pThis->secur32DLL);
			return false;
		}
		if (!WSDLL_CreateAdvapi32(&pThis->advapi32DLL))
		{
			WSDLL_DestroyCrypt32(&pThis->crypt32DLL);
			WSDLL_DestroySecur32(&pThis->secur32DLL);
			return false;
		}

		/*secur32.dll*/
		pThis->AcceptSecurityContextProc = (ACCEPTSECURITYCONTEXTPROC)WSDLL_GetProc(&pThis->secur32DLL, "AcceptSecurityContext");
		pThis->AcquireCredentialsHandleAProc = (ACQUIRECREDENTIALSHANDLEAPROC)WSDLL_GetProc(&pThis->secur32DLL, "AcquireCredentialsHandleA");
		pThis->CompleteAuthTokenProc = (COMPLETEAUTHTOKENPROC)WSDLL_GetProc(&pThis->secur32DLL, "CompleteAuthToken");
		pThis->FreeCredentialsHandleProc = (FREECREDENTIALSHANDLEPROC)WSDLL_GetProc(&pThis->secur32DLL, "FreeCredentialsHandle");
		pThis->QueryContextAttributesAProc = (QUERYCONTEXTATTRIBUTESAPROC)WSDLL_GetProc(&pThis->secur32DLL, "QueryContextAttributesA");
		pThis->InitializeSecurityContextAProc = (INITIALIZESECURITYCONTEXTAPROC)WSDLL_GetProc(&pThis->secur32DLL, "InitializeSecurityContextA");
		pThis->FreeContextBufferProc = (FREECONTEXTBUFFERPROC)WSDLL_GetProc(&pThis->secur32DLL, "FreeContextBuffer");
		pThis->DeleteSecurityContextProc = (DELETESECURITYCONTEXTPROC)WSDLL_GetProc(&pThis->secur32DLL, "DeleteSecurityContext");
		pThis->DecryptMessageProc = (DECRYPTMESSAGEPROC)WSDLL_GetProc(&pThis->secur32DLL, "DecryptMessage");
		pThis->EncryptMessageProc = (ENCRYPTMESSAGEPROC)WSDLL_GetProc(&pThis->secur32DLL, "EncryptMessage");
		pThis->ApplyControlTokenProc = (APPLYCONTROLTOKENPROC)WSDLL_GetProc(&pThis->secur32DLL, "ApplyControlToken");

		/*crypt32.dll*/
		pThis->CertCloseStoreProc = (CERTCLOSESTOREPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertCloseStore");
		pThis->CertFindCertificateInStoreProc = (CERTFINDCERTIFICATEINSTOREPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertFindCertificateInStore");
		pThis->CertCreateCertificateContextProc = (CERTCREATECERTIFICATECONTEXTPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertCreateCertificateContext");
		pThis->CertFreeCertificateContextProc = (CERTFREECERTIFICATECONTEXTPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertFreeCertificateContext");
		pThis->CertSetCertificateContextPropertyProc = (CERTSETCERTIFICATECONTEXTPROPERTYPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertSetCertificateContextProperty");
		pThis->CertOpenStoreProc = (CERTOPENSTOREPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertOpenStore");
		pThis->CertOpenSystemStoreProc = (CERTOPENSYSTEMSTOREPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertOpenSystemStoreA");
		pThis->CryptStringToBinaryAProc = (CRYPTSTRINGTOBINARYAPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CryptStringToBinaryA");
		pThis->CryptDecodeObjectProc = (CRYPTDECODEOBJECTPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CryptDecodeObject");

		/*advapi32.dll*/
		pThis->CryptAcquireContextAProc = (CRYPTACQUIRECONTEXTAPROC)WSDLL_GetProc(&pThis->advapi32DLL, "CryptAcquireContextA");
		pThis->CryptImportKeyProc = (CRYPTIMPORTKEYPROC)WSDLL_GetProc(&pThis->advapi32DLL, "CryptImportKey");
		pThis->CryptHashDataProc = (CRYPTHASHDATAPROC)WSDLL_GetProc(&pThis->advapi32DLL, "CryptHashData");
		pThis->CryptDestroyKeyProc = (CRYPTDESTROYKEYPROC)WSDLL_GetProc(&pThis->advapi32DLL, "CryptDestroyKey");
		pThis->CryptDestroyHashProc = (CRYPTDESTROYHASHPROC)WSDLL_GetProc(&pThis->advapi32DLL, "CryptDestroyHash");
		pThis->CryptCreateHashProc = (CRYPTCREATEHASHPROC)WSDLL_GetProc(&pThis->advapi32DLL, "CryptCreateHash");
		pThis->CryptGetProvParamProc = (CRYPTGETPROVPARAMPROC)WSDLL_GetProc(&pThis->advapi32DLL, "CryptGetProvParam");
		pThis->CryptGetKeyParamProc = (CRYPTGETKEYPARAMPROC)WSDLL_GetProc(&pThis->advapi32DLL, "CryptGetKeyParam");
		pThis->CryptDeriveKeyProc = (CRYPTDERIVEKEYPROC)WSDLL_GetProc(&pThis->advapi32DLL, "CryptDeriveKey");

		/*crypt32.dll - cert verification*/
		pThis->CertVerifyTimeValidityProc = (CERTVERIFYTIMEVALIDITYPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertVerifyTimeValidity");
		pThis->CertFreeCertificateChainProc = (CERTFREECERTIFICATECHAINPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertFreeCertificateChain");
		pThis->CertGetCertificateChainProc = (CERTGETCERTIFICATECHAINPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertGetCertificateChain");
		pThis->CertVerifyCertificateChainPolicyProc = (CERTVERIFYCERTIFICATECHAINPOLICYPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertVerifyCertificateChainPolicy");
		pThis->CertVerifyRevocationProc = (CERTVERIFYREVOCATIONPROC)WSDLL_GetProc(&pThis->crypt32DLL, "CertVerifyRevocation");

		return true;
	}
	void WSSSLLoader_Destroy(WSSSLLOADER* pThis)
	{
		WSDLL_DestroyAdvapi32(&pThis->advapi32DLL);
		WSDLL_DestroyCrypt32(&pThis->crypt32DLL);
		WSDLL_DestroySecur32(&pThis->secur32DLL);
	}

BOOL SChannelDisconnect(WSSOCKETSSL* pThis, PCredHandle phCreds, CtxtHandle *phContext,
						BOOL isClient, WSSSLLOADER* loader)
{
	DWORD     dwType;
	PBYTE     pbMessage;
	DWORD     cbMessage;
	size_t    cbData;

	SecBufferDesc   OutBuffer;
	SecBuffer    OutBuffers[1];
	DWORD     dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT   |
				ISC_REQ_REPLAY_DETECT  |
				ISC_REQ_CONFIDENTIALITY   |
				ISC_RET_EXTENDED_ERROR |
				ISC_REQ_ALLOCATE_MEMORY   |
				ISC_REQ_STREAM;
	DWORD     dwSSPIOutFlags;
	TimeStamp    tsExpiry;
	SECURITY_STATUS scRet;

	dwType = 1; /* SCHANNEL_SHUTDOWN; */

	OutBuffers[0].pvBuffer = &dwType;
	OutBuffers[0].BufferType = SECBUFFER_TOKEN;
	OutBuffers[0].cbBuffer = sizeof(dwType);

	OutBuffer.cBuffers = 1;
	OutBuffer.pBuffers = OutBuffers;
	OutBuffer.ulVersion = SECBUFFER_VERSION;

	do
	{
		scRet = (*loader->ApplyControlTokenProc)(phContext, &OutBuffer);

		if (FAILED(scRet)) {
			SetLastError(scRet);
			break;
		}

		OutBuffers[0].pvBuffer = NULL;
		OutBuffers[0].BufferType = SECBUFFER_TOKEN;
		OutBuffers[0].cbBuffer = 0;

		OutBuffer.cBuffers = 1;
		OutBuffer.pBuffers = OutBuffers;
		OutBuffer.ulVersion = SECBUFFER_VERSION;

		if (isClient)
		{
			scRet = (*loader->InitializeSecurityContextAProc)(
				phCreds,
				phContext,
				NULL,
				dwSSPIFlags,
				0,
				SECURITY_NATIVE_DREP,
				NULL,
				0,
				phContext,
				&OutBuffer,
				&dwSSPIOutFlags,
				&tsExpiry);
		}
		else
		{
			scRet = (*loader->AcceptSecurityContextProc)(
				phCreds,
				phContext,
				NULL,
				dwSSPIFlags,
				SECURITY_NATIVE_DREP,
				NULL,
				&OutBuffer,
				&dwSSPIOutFlags,
				&tsExpiry);
		}

		if (FAILED(scRet)) {
			SetLastError(scRet);
			break;
		}

		pbMessage = (BYTE *)(OutBuffers[0].pvBuffer);
		cbMessage = OutBuffers[0].cbBuffer;

		if (pbMessage != NULL && cbMessage != 0) {
			cbData = WSSocket_Write(&pThis->mSuper, pbMessage, cbMessage, NULL);
			if (cbData == SOCKET_ERROR || cbData == 0) {
				scRet = WSAGetLastError();
				SetLastError(scRet);
				break;
			}

			(*loader->FreeContextBufferProc)(pbMessage);
		}
	} while (FALSE);

	(*loader->DeleteSecurityContextProc)(phContext);

	return scRet;
}

void WSSocketSSL_Construct(WSSOCKETSSL* pThis)
{
	WSSocket_Construct(&pThis->mSuper);
	pThis->mHeaderSize = (0);
	pThis->mSSL.dwLower = pThis->mSSL.dwUpper =
		pThis->mCtx.dwUpper = pThis->mCtx.dwLower = 0;
}
void WSSocketSSL_Destruct(WSSOCKETSSL* pThis)
{
	WSSocketSSL_Destroy(pThis);
}
void WSSocketSSL_Own(WSSOCKETSSL* pThis, WSSOCKETSSL* pOther)
{
	WSMemoryCopy(pThis, pOther, sizeof(WSSOCKETSSL));
	WSSocketSSL_Construct(pOther);
}
void WSSocketSSL_Destroy(WSSOCKETSSL* pThis)
{
	if (pThis->mCtx.dwUpper != 0 && pThis->mCtx.dwLower != 0)
	{
		/*SChannelDisconnect(pThis, (_SecHandle*)&pThis->mSSL, pThis->mCtx,*/
		/*	pThis->mHeaderSize ? TRUE : FALSE, pThis->mSSLLoader);*/
		(*pThis->mSSLLoader->DeleteSecurityContextProc)((_SecHandle*)&pThis->mCtx);
		pThis->mCtx.dwUpper = pThis->mCtx.dwLower = 0;
	}
	if (pThis->mHeaderSize)
	{
		delete pThis->mReadBuf;
		delete pThis->mReadBackBuf;
		pThis->mHeaderSize = 0;
	}
	else if (pThis->mSSL.dwUpper != 0 && pThis->mSSL.dwLower != 0)
	{
		(*pThis->mSSLLoader->FreeCredentialsHandleProc)((_SecHandle*)&pThis->mSSL);
		pThis->mSSL.dwUpper = pThis->mSSL.dwLower = 0;
	}
	WSSocket_Destroy(&pThis->mSuper);
}

#ifdef _DEBUG
MutStr SSPIRetToString(SECURITY_STATUS sc)
{
	switch(sc)
	{
	case SEC_E_OK:
		return "SEC_E_OK";
	case SEC_I_CONTINUE_NEEDED:
		return "SEC_I_CONTINUE_NEEDED";
	case SEC_E_INCOMPLETE_MESSAGE:
		return "SEC_E_INCOMPLETE_MESSAGE";
	case SEC_I_INCOMPLETE_CREDENTIALS:
		return "SEC_I_INCOMPLETE_CREDENTIALS";
	case SEC_E_INCOMPLETE_CREDENTIALS:
		return "SEC_E_INCOMPLETE_CREDENTIALS";
	case SEC_I_COMPLETE_NEEDED:
		return "SEC_I_COMPLETE_NEEDED";
	case SEC_I_COMPLETE_AND_CONTINUE:
		return "SEC_I_COMPLETE_AND_CONTINUE";
	case SEC_E_INVALID_HANDLE:
		return "SEC_E_INVALID_HANDLE";
	case SEC_E_INVALID_TOKEN:
		return "SEC_E_INVALID_TOKEN";
	}
	return MutStr("Unhandled - numerical:") << sc;
}
#endif /* _DEBUG */

int SSPIconnectProc(_SecHandle*) { return 0; }
int SSPIacceptProc(_SecHandle*) { return 0; }

wsbool WSSocketSSL_DoConnectOrAccept(WSSOCKETSSL* pThis,
	int (*connectacceptProc)(_SecHandle*),
	const char* szHostname, SecBuffer* extraData)
{ /* sni hostname->strip trainling period->lowercase everything */
	SecBufferDesc  OutBuffDesc;
	SecBuffer   OutSecBuff[3];
	SecBufferDesc  InBuffDesc;
	SecBuffer   InSecBuff[2];
	/*ASC_REQ_MUTUAL_AUTH if client needs it*/
	ULONG    ulInFlags = ASC_REQ_SEQUENCE_DETECT | ASC_REQ_REPLAY_DETECT
		| ASC_REQ_CONFIDENTIALITY | ASC_REQ_EXTENDED_ERROR
		| ASC_REQ_ALLOCATE_MEMORY | ASC_REQ_STREAM,
		ulOutFlags;
	TimeStamp		  liExpiration;
	SECURITY_STATUS   scRet = SEC_E_OK, scLastStatus = SEC_E_OK;

	/* Prepare buffers */
	OutBuffDesc.ulVersion = InBuffDesc.ulVersion = SECBUFFER_VERSION;
	OutBuffDesc.cBuffers = 3;
	OutBuffDesc.pBuffers = OutSecBuff;
	InBuffDesc.cBuffers = 2;
	InBuffDesc.pBuffers = InSecBuff;
	BYTEVector pbReadBuffer;
	size_t uReadableBytes = 0, uGoingToRead, uOldSize;
	wsbool bDidLastReadBlock = false;
	unsigned long ulAllegedReadableBytes;
	wsbool block;

	/* while(SEC_I_CONTINUE_NEEDED||SEC_E_INCOMPLETE_MESSAGE */
	/*||SEC_I_INCOMPLETE_CREDENTIALS)*/
	do /* Read (Size4+Data)->Get Server SecurityContext->Write (Size4+Data) */
	{ /* don't read first on first loop (pThis->mCtx.dwUpper != 0 on 2+ iteration) */
		if ((pThis->mCtx.dwUpper || connectacceptProc == SSPIacceptProc)
			&& (uReadableBytes == 0 || scRet == SEC_E_INCOMPLETE_MESSAGE))
		{ /* only do socket read on connect, accept, and 2+ iterations */
		  /*do*/
		  /*{*/
			WSSocket_WaitForRead(&pThis->mSuper, 1000000, NULL);

			/*} while (uReadableBytes == 0);*/
			if(ioctlsocket((SOCKET)pThis->mSuper.mhSocket, FIONREAD, &ulAllegedReadableBytes) == 0)
				uReadableBytes = ulAllegedReadableBytes;
			else
				uReadableBytes = 0;

			uGoingToRead = uReadableBytes ?
				uReadableBytes : (1000);
			uOldSize = pbReadBuffer.size();
			pbReadBuffer.BeforeWrite(uOldSize + uGoingToRead);

			if (pThis->mCtx.dwUpper == 0 && connectacceptProc == SSPIacceptProc
				&& uOldSize == 0)
			{
				/* Read data without moving input location in case not ssl */
				block = WSSOCKET_NONBLOCKING;
				size_t uPeekableBytes = WSSocket_Peek(&pThis->mSuper,
					Vector_Data(pbReadBuffer), uReadableBytes, &block);
				if(uPeekableBytes == SOCKET_ERROR || uPeekableBytes == 0)
				{
					WSLogDebug(MutStr("SocketSCHANNEL: Couldn't peek initial data in handshake, should've read [") << uReadableBytes << "] bytes!");
				}
				/* CUPS-like HTTP check, do on first byte only */
				if (CStrLocateCharWithLength("DGHOPT", (char)*((BYTE*)Vector_Data(pbReadBuffer)), strcountof("DGHOPT")))
				{
					WSLogDebug("SocketSCHANNEL: Connection was regular HTTP, not HTTPS, passing on...");
					return true; /* regular HTTP instead of HTTPS */
				}
			}

			block = WSSOCKET_NONBLOCKING;
			uReadableBytes = WSSocket_Read(&pThis->mSuper,
				&pbReadBuffer[uOldSize], uGoingToRead, &block);
			if(uReadableBytes == (size_t)SOCKET_ERROR
				&& uOldSize == 0)
			{
				if(bDidLastReadBlock && scLastStatus != SEC_E_OK)
				{
					WSLogError("SocketSCHANNEL: Could not read back buffer during handshake");
					WSSocketSSL_Destroy(pThis);
					return false;
				}
				WSLogDebug("SocketSCHANNEL: Got a zero or blocked read in the handshake, trying one more time");;
				bDidLastReadBlock = true;
				scLastStatus = scRet;
				uReadableBytes = 0;
			}
			else
				bDidLastReadBlock = false;
			WSLogDebug(MutStr("SocketSCHANNEL: Read [") << uReadableBytes << "] bytes of data in handshake");
			pbReadBuffer.SetSize(uOldSize + uReadableBytes);
		} /* end if do socket read */
		OutSecBuff[0].BufferType = SECBUFFER_TOKEN;
		OutSecBuff[0].cbBuffer = 0;
		OutSecBuff[0].pvBuffer = NULL;
		OutSecBuff[1].BufferType = SECBUFFER_ALERT;
		OutSecBuff[1].cbBuffer = 0;
		OutSecBuff[1].pvBuffer = NULL;
		OutSecBuff[2].BufferType = SECBUFFER_EMPTY;
		OutSecBuff[2].cbBuffer = 0;
		OutSecBuff[2].pvBuffer = NULL;
		InSecBuff[0].BufferType = SECBUFFER_TOKEN;
		InSecBuff[0].pvBuffer = pbReadBuffer.size() ? Vector_Data(pbReadBuffer) : NULL;
		InSecBuff[0].cbBuffer = (unsigned long)pbReadBuffer.size();
		InSecBuff[1].BufferType = SECBUFFER_EMPTY;
		InSecBuff[1].cbBuffer = 0;
		InSecBuff[1].pvBuffer = NULL;

		/* AcceptSecurityContext() */
		if (connectacceptProc == SSPIacceptProc)
		{ /* Server-side */
			scRet = (*pThis->mSSLLoader->AcceptSecurityContextProc)(
				(_SecHandle*)&pThis->mSSL,
				pThis->mCtx.dwUpper ? (_SecHandle*)&pThis->mCtx : NULL,
				&InBuffDesc,
				ulInFlags,
				SECURITY_NATIVE_DREP,
				pThis->mCtx.dwUpper ? NULL : (_SecHandle*)&pThis->mCtx,
				&OutBuffDesc,
				&ulOutFlags,
				&liExpiration);
		}  /* end Server-side */
		else /* connectacceptProc == SSPIconnectProc */
		{  /* Client-side */
			scRet = (*pThis->mSSLLoader->InitializeSecurityContextAProc)(
				(_SecHandle*)&pThis->mSSL,
				pThis->mCtx.dwUpper ? (_SecHandle*)&pThis->mCtx : NULL,
				pThis->mCtx.dwUpper ? NULL : (char*)szHostname,
				ulInFlags,
				0,
				0,
				pThis->mCtx.dwUpper ? &InBuffDesc : NULL,
				0,
				pThis->mCtx.dwUpper ? NULL : (_SecHandle*)&pThis->mCtx,
				&OutBuffDesc,
				&ulOutFlags,
				&liExpiration);
		}
		WSLogDebug(MutStr("SocketSCHANNEL: Got return of [") << SSPIRetToString(scRet) << "] from init/acceptSecurityContext");
		if (scRet == SEC_I_COMPLETE_NEEDED
			|| scRet == SEC_I_COMPLETE_AND_CONTINUE)
		{
			WSLogDebug("SocketSCHANNEL: Got completion notice in handshake");
			if ((*pThis->mSSLLoader->CompleteAuthTokenProc)(
				(_SecHandle*)&pThis->mCtx, &OutBuffDesc) < SEC_E_OK)
			{
				WSLogError("SocketSCHANNEL: CompleteAuthToken on handshake premature completion failed");
				WSSocketSSL_Destroy(pThis);
				return false;
			}

			if (scRet == SEC_I_COMPLETE_AND_CONTINUE)
				scRet = SEC_I_CONTINUE_NEEDED;
			else
				scRet = SEC_E_OK;
		}
		/* Write back data */
		if ((scRet == SEC_E_OK || scRet == SEC_I_CONTINUE_NEEDED
			|| (scRet < SEC_E_OK && ((ulOutFlags & ASC_RET_EXTENDED_ERROR) != 0))
			|| (scRet == SEC_I_INCOMPLETE_CREDENTIALS
				&& !(ulInFlags & ISC_REQ_USE_SUPPLIED_CREDS))
			))
		{
			/* Should only be sent by the client version - */
			/* server is requesting a certificate instead of just */
			/* verifying theirs, attempt to do so */
			if(scRet == SEC_I_INCOMPLETE_CREDENTIALS)
			{
				WSLogDebug("SocketSCHANNEL: Server requested client credentials in handshake");
				ulInFlags |= ISC_REQ_USE_SUPPLIED_CREDS;
			}
			for(size_t i = 0; i < 3; ++i)
			{
				WSLogDebug("SocketSCHANNEL: Iterating through output buffers");
				if(OutSecBuff[i].cbBuffer > 0
					&& OutSecBuff[i].BufferType == SECBUFFER_TOKEN)
				{
					WSLogDebug(MutStr("SocketSCHANNEL: Writing back [")
						<< OutSecBuff[i].cbBuffer
						<< "] bytes of data in handshake");

					/* Write back the data XXXSecurityContext says we should */
					size_t numWrote =
						WSSocket_Write(&pThis->mSuper,
							OutSecBuff[i].pvBuffer, OutSecBuff[i].cbBuffer, NULL);
					(*pThis->mSSLLoader->FreeContextBufferProc)(OutSecBuff[i].pvBuffer);
					if (numWrote != OutSecBuff[i].cbBuffer)
					{
						(*pThis->mSSLLoader->DeleteSecurityContextProc)(
							(_SecHandle*)&pThis->mCtx);
						WSLogError("SocketSCHANNEL: Could not write back buffer");
						WSSocketSSL_Destroy(pThis);
						return false;
					}
				} /* end if valid output buffer */
			} /* end iterating through output buffers */
		}

		if (scRet == SEC_E_INCOMPLETE_MESSAGE)
			WSLogDebug("SocketSCHANNEL: Message incomplete, SSAPI wants more to be read");
		else if (scRet == SEC_E_INCOMPLETE_CREDENTIALS)
		{
			WSLogDebug("SocketSCHANNEL: Credentials incomplete, trying to read more to fix");
			scRet = SEC_I_INCOMPLETE_CREDENTIALS;
		}
		else if(scRet >= SEC_E_OK)
		{
			if (scRet == SEC_E_OK)
			{
				if (extraData)
				{
					if (InSecBuff[1].BufferType == SECBUFFER_EXTRA)
					{
						extraData->pvBuffer =
							WSALLOC(BYTE, InSecBuff[1].cbBuffer);

						if (extraData->pvBuffer == NULL)
						{
							SetLastError(ERROR_OUTOFMEMORY);
							return false;
						}

						WSMemoryCopy(extraData->pvBuffer,
							((BYTE*)Vector_Data(pbReadBuffer)) + (pbReadBuffer.size() - InSecBuff[1].cbBuffer),
							InSecBuff[1].cbBuffer);

						extraData->cbBuffer = InSecBuff[1].cbBuffer;
						extraData->BufferType = SECBUFFER_TOKEN;
					}
					else
					{
						extraData->pvBuffer = NULL;
						extraData->cbBuffer = 0;
						extraData->BufferType = SECBUFFER_EMPTY;
					}
				}
				return true;
			}
			if (InSecBuff[1].BufferType == SECBUFFER_EXTRA)
			{
				WSLogDebug(MutStr("SocketSCHANNEL: Got an extra buffer of [") << InSecBuff[1].cbBuffer << "] bytes in the handshake");
				pbReadBuffer.resize(InSecBuff[1].cbBuffer);
				WSMemoryCopy(Vector_Data(pbReadBuffer),
					(LPBYTE)(Vector_Data(pbReadBuffer) + (pbReadBuffer.size() - InSecBuff[1].cbBuffer)),
					InSecBuff[1].cbBuffer);
			}
			else
				pbReadBuffer.SetSize(uReadableBytes = 0);
		}
		else
		{
			WSLogError("SocketSCHANNEL: Initialize/AcceptSecurityContext failed");
			break;
		}
	} while (scRet == SEC_I_CONTINUE_NEEDED
		|| scRet == SEC_E_INCOMPLETE_MESSAGE
		/*|| scRet == SEC_E_INCOMPLETE_CREDENTIALS*/
		|| scRet == SEC_I_INCOMPLETE_CREDENTIALS);
	return false;
}

wsbool WSSocketSSL_Accept(WSSOCKETSSL* pThis, const WSSOCKETSSL* pListener,
	WSSOCKETLIBRARY* socklib)
{
	SecPkgContext_StreamSizes	sizes;
	if (!WSSocket_Accept(&pThis->mSuper, &pListener->mSuper, socklib))
		return false;
	pThis->mSSLLoader = WSSocketLibrary_GetSSLLoader(socklib);
	WSMemoryCopy(&pThis->mSSL, &pListener->mSSL, sizeof(pListener->mSSL));
	if (!WSSocketSSL_DoConnectOrAccept(pThis, SSPIacceptProc, NULL, NULL))
		return false;

	if (pThis->mCtx.dwUpper)
	{
		SECURITY_STATUS scRet;
		if ((scRet = (*pThis->mSSLLoader->QueryContextAttributesAProc)(
			(_SecHandle*)&pThis->mCtx, SECPKG_ATTR_STREAM_SIZES, &sizes)) < SEC_E_OK)
		{
			return false;
		}
		pThis->mReadBuf = new NOTHROW BYTEVector();
		pThis->mReadBackBuf = new NOTHROW BYTEVector();
		if(!pThis->mReadBuf || !pThis->mReadBackBuf)
			return false;
		pThis->mReadBuf->reserve(sizes.cbHeader +
			sizes.cbMaximumMessage + sizes.cbTrailer);
		pThis->mReadBackBuf->reserve(sizes.cbHeader +
			sizes.cbMaximumMessage + sizes.cbTrailer);
		if(!pThis->mReadBuf->capacity() || !pThis->mReadBackBuf->capacity())
			return false;
		pThis->mHeaderSize = sizes.cbHeader;
		pThis->mMaxMessageSize = sizes.cbMaximumMessage;
	}
	return true;
}


#ifndef UNISP_NAME_A
#define UNISP_NAME_A "Microsoft Unified Security Protocol Provider"
#endif

HRESULT VerifySSLCertificate(PCCERT_CONTEXT pContext, PCTSTR pszServerName,
	DWORD dwCertFlags, DWORD dwAuthType, WSSSLLOADER* loader)
{
	HTTPSPolicyCallbackData		polHttps;
	CERT_CHAIN_POLICY_PARA		PolicyPara;
	CERT_CHAIN_POLICY_STATUS	PolicyStatus;
	CERT_CHAIN_PARA				ChainPara;
	PCCERT_CHAIN_CONTEXT		pChainContext = NULL;
	PWSTR   pwszServerName;
#ifndef _UNICODE
	DWORD   cchServerName;
#endif
	SECURITY_STATUS scRet;
	do {
		int iRc = (*loader->CertVerifyTimeValidityProc)(NULL, pContext->pCertInfo);
		if (iRc != 0)
		{
			pwszServerName = NULL;
			scRet = SEC_E_CERT_EXPIRED;
			SetLastError(scRet);
			break;
		}

#ifndef _UNICODE
		if (dwAuthType == AUTHTYPE_CLIENT
				&& (pszServerName == NULL || lstrlen(pszServerName) == 0))
			return SEC_E_WRONG_PRINCIPAL;

		cchServerName = MultiByteToWideChar(CP_ACP, 0, pszServerName, -1, NULL, 0);
		pwszServerName = WSALLOC(WCHAR, cchServerName);
		if (pwszServerName == NULL)
			return SEC_E_INSUFFICIENT_MEMORY;
		cchServerName = MultiByteToWideChar(CP_ACP, 0, pszServerName, -1, pwszServerName, cchServerName);
		if (dwAuthType == AUTHTYPE_CLIENT && cchServerName == 0)
			return SEC_E_WRONG_PRINCIPAL;
#else
		pwszServerName = (WCHAR*)pszServerName;
#endif

		WSMemoryFill(&ChainPara, 0, sizeof(ChainPara));
		ChainPara.cbSize = sizeof(ChainPara);

		if (!(*loader->CertGetCertificateChainProc)(
			NULL, pContext, NULL, NULL, &ChainPara,
			dwAuthType == AUTHTYPE_SERVER ? CERT_CHAIN_REVOCATION_CHECK_CHAIN : 0,
			NULL, &pChainContext))
		{
			scRet = GetLastError();
			SetLastError(scRet);
			break;
		}

		WSMemoryFill(&polHttps, 0, sizeof(HTTPSPolicyCallbackData));
		polHttps.cbStruct = sizeof(HTTPSPolicyCallbackData);
		polHttps.dwAuthType = dwAuthType;
		polHttps.fdwChecks = dwCertFlags;
		polHttps.pwszServerName = pwszServerName;

		WSMemoryFill(&PolicyPara, 0, sizeof(PolicyPara));
		PolicyPara.cbSize = sizeof(PolicyPara);
		PolicyPara.pvExtraPolicyPara = &polHttps;

		WSMemoryFill(&PolicyStatus, 0, sizeof(PolicyStatus));
		PolicyStatus.cbSize = sizeof(PolicyStatus);

		if (!(*loader->CertVerifyCertificateChainPolicyProc)(
			CERT_CHAIN_POLICY_SSL, pChainContext, &PolicyPara, &PolicyStatus))
		{
			scRet = GetLastError();
			SetLastError(scRet);
			break;
		}

		if (PolicyStatus.dwError != 0x80092012) /* 0x80092012 == unable to check for revocation */
		{
			scRet = PolicyStatus.dwError;
			SetLastError(scRet);
			break;
		}

		PCERT_CONTEXT *pCerts = WSALLOC(PCERT_CONTEXT, pChainContext->cChain);
		if(!pCerts)
		{
			scRet = SEC_E_INSUFFICIENT_MEMORY;
			break;
		}

		for (DWORD i = 0; i < pChainContext->cChain; i++)
			pCerts[i] = (PCERT_CONTEXT)(pChainContext->rgpChain[i]->rgpElement[0]->pCertContext);

		CERT_REVOCATION_STATUS revStat;
		revStat.cbSize = sizeof(CERT_REVOCATION_STATUS);

		BOOL bRc = (*loader->CertVerifyRevocationProc)(
			X509_ASN_ENCODING, CERT_CONTEXT_REVOCATION_TYPE,
			pChainContext->cChain, (void **)pCerts,
			CERT_VERIFY_REV_CHAIN_FLAG, NULL, &revStat);
		if (!bRc)/* && revStat.dwError != 0x80092012) */
		{
			scRet = revStat.dwError;
			SetLastError(revStat.dwError);
			break;
		}

		WSFREE(pCerts);

		scRet = SEC_E_OK;

	} while (FALSE);

	if (pChainContext)
		(*loader->CertFreeCertificateChainProc)(pChainContext);

#ifndef _UNICODE
	if (pwszServerName) WSFREE(pwszServerName);
#endif

	return scRet;
}

HRESULT VerifyRemoteSSLCertificate(PCtxtHandle hContext, PCTSTR pszServerName,
	DWORD dwCertFlags, DWORD dwAuthType, WSSSLLOADER* loader)
{
	SECURITY_STATUS scRet;
	PCCERT_CONTEXT pContext;
	if ((*loader->QueryContextAttributesAProc)(hContext,
		0x53, /*SECPKG_ATTR_REMOTE_CERT_CONTEXT,*/
		(PVOID)&pContext) != SEC_E_OK)
	{
		scRet = SEC_E_WRONG_PRINCIPAL;
		SetLastError(scRet);
		return scRet;
	}
	scRet = VerifySSLCertificate(pContext, pszServerName,
								 dwCertFlags, dwAuthType, loader);
	if (scRet == SEC_E_OK)
		(*loader->CertFreeCertificateContextProc)(pContext);
	return scRet;
}

HRESULT VerifyServerSSLCertificate(PCCERT_CONTEXT pContext, WSSSLLOADER* loader)
{
	TCHAR szComputerName[1024];
	DWORD dwComputerNameSize = sizeof(szComputerName);
	if(!GetComputerName(szComputerName, &dwComputerNameSize))
		 return SEC_E_INSUFFICIENT_MEMORY;
	return VerifySSLCertificate(pContext,
				TEXT("localhost"), 0, AUTHTYPE_SERVER, loader);
}

HRESULT VerifyClientSSLCertificate(PCtxtHandle hContext,
	PCTSTR szHostName, WSSSLLOADER* loader)
{
	return VerifyRemoteSSLCertificate(hContext,
				szHostName, 0, AUTHTYPE_CLIENT, loader);
}

wsbool WSSocketSSL_CreateClient(WSSOCKETSSL* pThis,
	const char* szHostname, size_t port,
	WSSOCKETLIBRARY* socklib, const char* WSUNUSED(certPath),
	const char* WSUNUSED(keyPath))
{
	TimeStamp liExpiration;
	SecPkgContext_StreamSizes	sizes;
	if (!WSSocket_CreateClient(&pThis->mSuper, szHostname, port, socklib))
		return false;
	pThis->mSSLLoader = WSSocketLibrary_GetSSLLoader(socklib);
	/* Create credentials */
	if ((*pThis->mSSLLoader->AcquireCredentialsHandleAProc)( /* 2 alt == "Negotiate" */
		NULL, (SEC_CHAR*)UNISP_NAME_A, SECPKG_CRED_OUTBOUND,
		NULL, NULL,
		NULL, NULL, (_SecHandle*)&pThis->mSSL, &liExpiration) < SEC_E_OK)
	{
		WSSocketSSL_Destroy(pThis);
		return false;
	}
	if(!WSSocketSSL_DoConnectOrAccept(pThis, SSPIconnectProc, szHostname, NULL)
		|| !pThis->mCtx.dwUpper)
	{
		WSSocketSSL_Destroy(pThis);
		return false;
	}

	SECURITY_STATUS scRet;
	if ((scRet = (*pThis->mSSLLoader->QueryContextAttributesAProc)(
		(_SecHandle*)&pThis->mCtx, SECPKG_ATTR_STREAM_SIZES, &sizes)) < SEC_E_OK)
	{
		WSSocketSSL_Destroy(pThis);
		return false;
	}
	pThis->mReadBuf = new NOTHROW BYTEVector(sizes.cbHeader +
		sizes.cbMaximumMessage + sizes.cbTrailer);
	pThis->mReadBackBuf = new NOTHROW BYTEVector(sizes.cbHeader +
		sizes.cbMaximumMessage + sizes.cbTrailer);
	if(!pThis->mReadBuf || !pThis->mReadBackBuf)
		return false;
	pThis->mHeaderSize = sizes.cbHeader;
	pThis->mMaxMessageSize = sizes.cbMaximumMessage;
	return true;
}

DECLARE_SCOPED_POINTER(ScopedPSTR, CHAR)
DECLARE_SCOPED_POINTER(ScopedPWSTR, WCHAR)

BOOL CryptDeriveEncryptedKey(HCRYPTPROV hPrivateProv, PBYTE pbSaltData,
	DWORD dwSaltDataSize, DWORD dwKeySize, HCRYPTKEY* phDerivedKey,
	PSTR szPassword, DWORD dwPasswordLen, WSSSLLOADER* mSSLLoader)
{
	wsbool success;
	HCRYPTHASH hHash;
	if(!(*mSSLLoader->CryptCreateHashProc)(hPrivateProv, CALG_SHA1, NULL, 0, &hHash))
		return FALSE;
	success =
		(*mSSLLoader->CryptHashDataProc)(hHash, pbSaltData, dwSaltDataSize, 0)
		&& (*mSSLLoader->CryptHashDataProc)(hHash, (const BYTE*)szPassword, dwPasswordLen, 0)
		&& (*mSSLLoader->CryptDeriveKeyProc)(hPrivateProv, CALG_RC4, hHash, dwKeySize << 16, phDerivedKey);
	(*mSSLLoader->CryptDestroyHashProc)(hHash);
	return success;
}

LPWSTR STRToWSTR(const char* s, size_t* len = NULL);
char* WSTRToSTR(LPCWSTR w, size_t* len = NULL);

wsbool WSSocketSSL_CreateServer(WSSOCKETSSL* pThis, size_t port,
	const char* certPath, const char* keyPath, WSSOCKETLIBRARY* socklib,
	const char* WSUNUSED(CAFilePath), const char* WSUNUSED(CADirPath))
{
	if (!WSSocket_CreateServer(&pThis->mSuper, port, socklib))
		return false;
	pThis->mSSLLoader = WSSocketLibrary_GetSSLLoader(socklib);
	/* Create credentials */
	SECURITY_STATUS scRet;
	SCHANNEL_CRED credData;
	TimeStamp expiration;
	PCCERT_CONTEXT pCertContext;

#if 0
	HCERTSTORE certStore;
	/* Client mode with auth (SECPKG_CRED_OUTBOUND) only opens system store, */
	/* only tries to get a context with username input, */
	/* and 0 for cCreads/paCreds if no name */
	certStore = (*pThis->mSSLLoader->CertOpenStoreProc)(
		CERT_STORE_PROV_SYSTEM_A, X509_ASN_ENCODING,
		0, CERT_SYSTEM_STORE_LOCAL_MACHINE
		/*|CERT_STORE_READONLY_FLAG*/
		, "MY");
	if (!certStore)
	{
		certStore = (*pThis->mSSLLoader->CertOpenSystemStoreProc)(NULL, "MY");
		if (!certStore)
		{
			WSLogError("Could not open certificate store");
			WSSocketSSL_Destroy(pThis);
			return false;
		}
	}

#if 0
	FindCertificate(certStore, "ererer");
	CERT_RDN cert_rdn;
	CERT_RDN_ATTR cert_rdn_attr;
	cert_rdn.cRDNAttr = 1;
	cert_rdn.rgRDNAttr = &cert_rdn_attr;

	cert_rdn_attr.pszObjId = szOID_COMMON_NAME;
	cert_rdn_attr.dwValueType = CERT_RDN_ANY_TYPE;
	cert_rdn_attr.Value.cbData = lstrlenA("Ryan");
	cert_rdn_attr.Value.pbData = (BYTE *)"Ryan";
	PCCERT_CONTEXT certContext = CertFindCertificateInStore(
		certStore,
		X509_ASN_ENCODING,
		0,
		CERT_FIND_SUBJECT_ATTR,
		&cert_rdn,
		NULL);
#else
	WSDLL cryptui;
	WSDLL_Create(&cryptui, "cryptui.dll");
	CRYPTUIDLGSELECTCERTIFICATEPROC lpCryptUIDlgSelectCertificate =
		(CRYPTUIDLGSELECTCERTIFICATEPROC)WSDLL_GetProc(&cryptui, "CryptUIDlgSelectCertificateA");

	CRYPTUI_SELECTCERTIFICATE_STRUCT csc;
	WSMemoryFill(&csc, 0, sizeof(csc));
	csc.dwSize = sizeof(csc);
	csc.szTitle = "Select a Server Certificate";
	csc.cDisplayStores = 1;
	csc.rghDisplayStores = &certStore;
	PCCERT_CONTEXT certContext = (*lpCryptUIDlgSelectCertificate)(&csc);
	WSDLL_Destroy(&cryptui);
#endif
	if (!certContext)
	{
		(*pThis->mSSLLoader->CertCloseStoreProc)(certStore, 0);
		WSLogError("Could not find certificate in store");
		WSSocketSSL_Destroy(pThis);
		return false;
	}
#endif
	File certFile;
	size_t uCertFileSize, uB64Len;
	LPCSTR szStartPos, szEndPos;
	if(!certFile.Create(certPath, FILE_READ | FILE_BINARY))
	{
		WSLogError("Could not find open given public key certificate");
		WSSocketSSL_Destroy(pThis);
		return false;
	}

	{
		uCertFileSize = certFile.Size();
		ScopedBytes pbCertBuffer(uCertFileSize);
		if(!pbCertBuffer.IsCreated()
			|| certFile.Read(pbCertBuffer, uCertFileSize) != uCertFileSize)
		{
			WSLogError("Could not find open given public key certificate");
			WSSocketSSL_Destroy(pThis);
			return false;
		}

		szStartPos = (LPCSTR)(PBYTE)pbCertBuffer;
		szEndPos = szStartPos + uCertFileSize - 1;
		while(*szEndPos != '-')
			--szEndPos;
		szEndPos -= 24;
		wsbool bIsPEMIsh = AnsiiCompare(szStartPos, "-----BEGIN CERTIFICATE-----", 27, 27) == 0
			&& AnsiiCompare(szEndPos, "-----END CERTIFICATE-----", 25, 25) == 0;
		wsbool bIscertreqStyle;
		if(!bIsPEMIsh)
		{
			szEndPos -= 12;
			bIscertreqStyle = true;
			bIsPEMIsh = AnsiiCompare(szStartPos, "-----BEGIN NEW CERTIFICATE REQUEST-----", 39, 39) == 0
				&& AnsiiCompare(szEndPos, "-----END NEW CERTIFICATE REQUEST-----", 37, 37) == 0;
		}
		else
			bIscertreqStyle = false;

		if(bIsPEMIsh)
		{
			while(szEndPos[-1] == '\x0D' || szEndPos[-1] == '\x0A')
				--szEndPos;
			szStartPos += bIscertreqStyle ? 39 : 27; GetLine(&szStartPos);
			ScopedBytes pbEncodedCertBuffer(pbCertBuffer);

			uB64Len = szEndPos - szStartPos;
			pbCertBuffer = (PBYTE) Base64Decode(szStartPos, &uB64Len);
			if(!pbCertBuffer.IsCreated())
			{
				WSSocketSSL_Destroy(pThis);
				return false;
			}
			uCertFileSize = uB64Len;
		}

		pCertContext = (*pThis->mSSLLoader->CertCreateCertificateContextProc)(
			X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, pbCertBuffer, (DWORD)uCertFileSize);
		if (!(pCertContext))
		{
			WSLogError("Could not create certificate for public key certificate");
			WSSocketSSL_Destroy(pThis);
			return false;
		}
	}

	if(!certFile.Create(keyPath, FILE_READ | FILE_BINARY))
	{
		WSLogError("Could not find open given private key certificate");
		WSSocketSSL_Destroy(pThis);
		return false;
	}

	{
		DWORD dwSaltDataSize, dwPvkDataSize, dwInfoSize;
		PBYTE pbSaltData, pbPvkData;
		HCRYPTPROV hPrivateProv;
		HCRYPTKEY hPrivateKey, hDerivedPrivateKey;
		CRYPT_KEY_PROV_INFO PvkProviderInfo;
		uCertFileSize = certFile.Size();
		ScopedBytes pbPvkBuffer(uCertFileSize);
		if(!pbPvkBuffer.IsCreated()
			|| uCertFileSize < 100
			|| certFile.Read(pbPvkBuffer, uCertFileSize) != uCertFileSize)
		{
			WSLogError("Could not find open given public key certificate");
			WSSocketSSL_Destroy(pThis);
			return false;
		}
#if 0 /* for reference of the file format */
		typedef struct PVK_ELF_HEADER__
		{ /* http://www.drh-consultancy.demon.co.uk/pvk.html */
			DWORD dwMagic;
			DWORD dwReserved;
			DWORD dwKeySpec;
			DWORD dwIsEncrypted;
			DWORD dwSaltLen;
			DWORD dwKeyLen;
		} PVK_ELF_HEADER, *PPVK_ELF_HEADER;
#endif
		szStartPos = (LPCSTR)(PBYTE)pbPvkBuffer;
		szEndPos = szStartPos + uCertFileSize - 1;
		while(*szEndPos != '-')
			--szEndPos;
		szEndPos -= 24;
		wsbool bIsPvk, bIsPEM, bIsRSA;
		bIsPEM = AnsiiCompare(szStartPos, "-----BEGIN PRIVATE KEY-----", 27, 27) == 0
			&& AnsiiCompare(szEndPos, "-----END PRIVATE KEY-----", 25, 25) == 0;
		if(!bIsPEM)
		{
			szEndPos -= 4;
			bIsRSA = true;
			bIsPEM = AnsiiCompare(szStartPos, "-----BEGIN RSA PRIVATE KEY-----", 31, 31) == 0
				&& AnsiiCompare(szEndPos, "-----END RSA PRIVATE KEY-----", 29, 29) == 0;
		}
		else
			bIsRSA = false;

		if(bIsPEM)
		{
			while(szEndPos[-1] == '\x0D' || szEndPos[-1] == '\x0A')
				--szEndPos;
			szStartPos += bIsRSA ? 31 : 27; GetLine(&szStartPos);
			ScopedBytes pbEncodedPvkBuffer(pbPvkBuffer);
			uB64Len = szEndPos - szStartPos;
			pbPvkBuffer = (PBYTE) Base64Decode(szStartPos, &uB64Len);
			if(!pbPvkBuffer.IsCreated())
			{
				WSSocketSSL_Destroy(pThis);
				return false;
			}
			uCertFileSize = uB64Len;
			bIsPvk = false;

			PCRYPT_PRIVATE_KEY_INFO PrivateKeyInfo;
			BYTE* pbRSAData;
			DWORD dwRSADataSize;
			if(!bIsRSA)
			{ /* If it isn't RSA we must parse it into a PrivateKeyInfo struct first */
				if(!(*pThis->mSSLLoader->CryptDecodeObjectProc)(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
					PKCS_PRIVATE_KEY_INFO, pbPvkBuffer, (DWORD)uB64Len,
					CRYPT_DECODE_ALLOC_FLAG|CRYPT_DECODE_NOCOPY_FLAG,
					(void**)&PrivateKeyInfo, &dwInfoSize))
				{
					WSLogError("CryptDecodeObject failed");
					WSSocketSSL_Destroy(pThis);
					return false;
				}
				pbRSAData = PrivateKeyInfo->PrivateKey.pbData;
				dwRSADataSize = PrivateKeyInfo->PrivateKey.cbData;
			}
			else
			{
				pbRSAData = pbPvkBuffer;
				dwRSADataSize = (DWORD)uB64Len;
			}

			if (!(*pThis->mSSLLoader->CryptDecodeObjectProc)(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
				PKCS_RSA_PRIVATE_KEY, pbRSAData, dwRSADataSize, 0, NULL, &dwInfoSize))
			{
				WSLogError("CryptDecodeObject2 failed");
				WSSocketSSL_Destroy(pThis);
				return false;
			}

			ScopedBytes pbPartEncodePvkBuffer(pbPvkBuffer);
			pbPvkBuffer = WSALLOC(BYTE, dwInfoSize);
			if(!pbPvkBuffer.IsCreated())
			{
				WSSocketSSL_Destroy(pThis);
				return false;
			}
			if (!(*pThis->mSSLLoader->CryptDecodeObjectProc)(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
				PKCS_RSA_PRIVATE_KEY, pbRSAData, dwRSADataSize, 0, pbPvkBuffer, &dwInfoSize))
			{
				WSLogError("CryptDecodeObject3 failed");
				WSSocketSSL_Destroy(pThis);
				return false;
			}

			uCertFileSize = dwInfoSize;
			if(!bIsRSA)
				::LocalFree(PrivateKeyInfo);
		}
		else if(((DWORD*)(BYTE*)pbPvkBuffer)[0] != 0xB0B5F11E)
		{
			WSLogError("Private key PVK magic value is not correct");
			WSSocketSSL_Destroy(pThis);
			return false;
		}
		else
			bIsPvk = true;
		/* MS_STRONG_PROV/PROV_RSA_FULL is pvk default, not PROV_RSA_SCHANNEL */
		/* If last param is CRYPT_VERIFYCONTEXT, AcquireCredentialsHandle will */
		/* fail unless app runner has root access and dwFlags = CRYPT_MACHINE_KEYSET */
		/* of the provider info structure, which is responsible for */
		/* unknown certificate errors from AcquireCredentialsHandle */
		if(!(*pThis->mSSLLoader->CryptAcquireContextAProc)(
			&hPrivateProv, NULL, MS_STRONG_PROV_A, PROV_RSA_FULL, CRYPT_SILENT))
		{
			WSLogError("Could not acquire Win32Crypt context");
			WSSocketSSL_Destroy(pThis);
			return false;
		}

		wsbool bImportRaw;
		/*((DWORD*)(BYTE*)pbPvkBuffer)[1];*/ /* REserved */
		/*((DWORD*)(BYTE*)pbPvkBuffer)[2];*/ /* keytype(CRYPT_KEY_PROV_INFO->dwKeySpec) */
		dwSaltDataSize = ((DWORD*)(BYTE*)pbPvkBuffer)[4];
		dwPvkDataSize = ((DWORD*)(BYTE*)pbPvkBuffer)[5];
		if(bIsPvk
			&& ((DWORD*)(BYTE*)pbPvkBuffer)[3] /* is encrypted ? 1 == yes */
			&& (dwSaltDataSize))
		{
			pbSaltData = (BYTE*)&(((DWORD*)(BYTE*)pbPvkBuffer)[6]);
			pbPvkData = pbSaltData + dwSaltDataSize;
			if(!CryptDeriveEncryptedKey(hPrivateProv, pbSaltData, dwSaltDataSize,
									0x0080, &hDerivedPrivateKey, (SEC_CHAR*)"Test123",
									strcountof("Test123"), pThis->mSSLLoader))
			{
				WSLogError("Could not derive encrypted Win32Crypt key");
				WSSocketSSL_Destroy(pThis);
				return false;
			}

			if(!(*pThis->mSSLLoader->CryptImportKeyProc)(hPrivateProv, pbPvkData, dwPvkDataSize,
					hDerivedPrivateKey, 1, &hPrivateKey))
			{ /* Strong auth failed, try weaker auth */
				(*pThis->mSSLLoader->CryptDestroyKeyProc)(hDerivedPrivateKey);
				CryptDeriveEncryptedKey(hPrivateProv, pbSaltData, dwSaltDataSize,
						0x0028, &hDerivedPrivateKey, (SEC_CHAR*)"Test123", strcountof("Test123"),
						pThis->mSSLLoader);
				bImportRaw = (*pThis->mSSLLoader->CryptImportKeyProc)(hPrivateProv, pbPvkData, dwPvkDataSize,
					hDerivedPrivateKey, 1, &hPrivateKey) == 0;
			}
			else
				bImportRaw = false;
		}
		else
			bImportRaw = true;

		if(bImportRaw)
		{
			if(!bIsPvk)
				pbPvkData = pbPvkBuffer;
			else
			{
				pbPvkData = (BYTE*)&(((DWORD*)(BYTE*)pbPvkBuffer)[6]);
				uCertFileSize = dwPvkDataSize;
			}
			bImportRaw = (*pThis->mSSLLoader->CryptImportKeyProc)(
				hPrivateProv, pbPvkData, (DWORD)uCertFileSize, NULL, 1, &hPrivateKey);
		}

#if 1
		ScopedPSTR pszContainerName, pszProviderName;
		ScopedPWSTR pwszContainerName, pwszProviderName;
		/* Container - NULL probably, param of CryptAcquireContext */
		if((*pThis->mSSLLoader->CryptGetProvParamProc)(hPrivateProv, PP_CONTAINER,
				NULL, &dwInfoSize, 0))
		{
			pszContainerName = WSALLOC(CHAR, dwInfoSize);
			if(!pszContainerName.IsCreated())
			{
				WSSocketSSL_Destroy(pThis);
				return false;
			}
			(*pThis->mSSLLoader->CryptGetProvParamProc)(hPrivateProv, PP_CONTAINER,
					(PBYTE)(PSTR)pszContainerName, &dwInfoSize, 0);
			pwszContainerName = STRToWSTR(pszContainerName);
			if(!pwszContainerName.IsCreated())
			{
				WSSocketSSL_Destroy(pThis);
				return false;
			}
			PvkProviderInfo.pwszContainerName = pwszContainerName;
		}
		else
		{
			pszContainerName = NULL;
			PvkProviderInfo.pwszContainerName = NULL;
			pwszContainerName = NULL;
		}
		/**/
		if((*pThis->mSSLLoader->CryptGetProvParamProc)(hPrivateProv, PP_NAME,
				NULL, &dwInfoSize, 0))
		{
			pszProviderName = WSALLOC(CHAR, dwInfoSize);
			if(!pszProviderName.IsCreated())
			{
				WSSocketSSL_Destroy(pThis);
				return false;
			}

			(*pThis->mSSLLoader->CryptGetProvParamProc)(hPrivateProv, PP_NAME,
					(PBYTE)(PSTR)pszProviderName, &dwInfoSize, 0);
			pwszProviderName = STRToWSTR(pszProviderName);
			if(!pwszProviderName.IsCreated())
			{
				WSSocketSSL_Destroy(pThis);
				return false;
			}
			PvkProviderInfo.pwszProvName = pwszProviderName;
		}
		else
		{
			pszProviderName = NULL;
			PvkProviderInfo.pwszProvName = NULL;
			pwszProviderName = NULL;
		}
#else
		PvkProviderInfo.pwszContainerName = NULL;
		PvkProviderInfo.pwszProvName = MS_STRONG_PROV_W;
#endif
		if(!(*pThis->mSSLLoader->CryptGetProvParamProc)(hPrivateProv, PP_PROVTYPE,
				(LPBYTE) &PvkProviderInfo.dwProvType, &dwInfoSize, 0))
		{
		}
		PvkProviderInfo.dwFlags = 0;/*CRYPT_MACHINE_KEYSET;*//*0x20*/
		PvkProviderInfo.cProvParam = 0;
		PvkProviderInfo.rgProvParam = NULL;
		if(!(*pThis->mSSLLoader->CryptGetKeyParamProc)(hPrivateKey, KP_ALGID,
				(LPBYTE) &PvkProviderInfo.dwKeySpec, &dwInfoSize, 0))
		{
		}
		switch (PvkProviderInfo.dwKeySpec)
		{ //https://stackoverflow.com/questions/45211765/cryptoapi-rsa-schannel-provider-error
		case CALG_RSA_KEYX:
		case CALG_DH_SF:
			PvkProviderInfo.dwKeySpec = AT_KEYEXCHANGE;
		break;
		case CALG_RSA_SIGN:
		case CALG_DSS_SIGN:
			PvkProviderInfo.dwKeySpec = AT_SIGNATURE;
		break;
		}
#if 0
		ScopedPWSTR pwszPvkPath(STRToWSTR(keyPath, CStrLength(keyPath)));
		if(!pwszPvkPath.IsCreated())
		{
			WSSocketSSL_Destroy(pThis);
			return false;
		}
		CRYPT_DATA_BLOB PvkFileNameBlob;
		PvkFileNameBlob.pbData = (BYTE*)(PWSTR)pwszPvkPath;
		PvkFileNameBlob.cbData = lstrlenW(pwszPvkPath);
		if(!(*pThis->mSSLLoader->CertSetCertificateContextPropertyProc)(pCertContext,
			CERT_PVK_FILE_PROP_ID, 0, (const void*)(LPCWSTR)&PvkFileNameBlob))
		{
			DWORD dw = GetLastError();
			dw = 0;
		}
#endif

		DWORD dw = (*pThis->mSSLLoader->CertSetCertificateContextPropertyProc)(pCertContext,
			CERT_KEY_PROV_INFO_PROP_ID, 0, &PvkProviderInfo);
		if(!dw)
		{
			pThis->mSSLLoader->CertFreeCertificateContextProc(pCertContext);
			return false;
		}
	}

	WSMemoryFill(&credData, 0, sizeof(credData));
	credData.dwVersion = SCHANNEL_CRED_VERSION;
	credData.grbitEnabledProtocols =
		SP_PROT_TLS1_2 | SP_PROT_SSL2 | SP_PROT_SSL3
		| SP_PROT_TLS1 | SP_PROT_TLS1_1;/* | SP_PROT_TLS1_3 = beyond XP */
	credData.cCreds = 1;
	credData.paCred = &pCertContext;
	credData.dwFlags = 0x00400000; /* SCH_USE_STRONG_CRYPTO */
	/*credData.hRootStore = certStore;*/
	/*credData.dwMinimumCipherStrength = 80;*/
	/*credData.dwFlags = SCH_CRED_NO_DEFAULT_CREDS | SCH_CRED_NO_SYSTEM_MAPPER | SCH_CRED_REVOCATION_CHECK_CHAIN;*/
	if ((scRet = pThis->mSSLLoader->AcquireCredentialsHandleAProc(
		NULL, (SEC_CHAR*)UNISP_NAME_A, SECPKG_CRED_INBOUND,
		NULL, &credData,
		NULL, NULL, (_SecHandle*)&pThis->mSSL, &expiration)) < SEC_E_OK)
	{
		WSLogError("Could not acquire credentials handle - \
					probably couldn't access private key");
#ifdef _DEBUG
		::MessageBox(0, TEXT("NOOO"), TEXT("dgdg"), 0);
#endif
		ExitProcess(0);
	}
	/*VerifyServerSSLCertificate(pCertContext, pThis->mSSLLoader);*/
	/*pThis->mSSLLoader->CertCloseStoreProc(certStore, 0);*/
	pThis->mSSLLoader->CertFreeCertificateContextProc(pCertContext);
	if (scRet >= SEC_E_OK)
		return true;
	WSSocketSSL_Destroy(pThis);
	return false;
}

size_t WSSocketSSL_Read(const WSSOCKETSSL* pThis,
	void* out, size_t numOut, wsbool* inblock)
{
	SecBufferDesc  BuffDesc;
	SecBuffer   SecBuff[4];
	SecBuffer *  pDataBuffer;
	SecBuffer *  pExtraBuffer;
	SecBuffer		HandshakeBuffer;
	BYTEVector* pReadBuffer;
	BYTEVector* pReserveReadBuffer;
	size_t uNumRead, uDataLen, uDataBufLen, uRawNumRead;
	BYTE* dataBuf;
	SECURITY_STATUS scRet;
	BOOL shouldContinue;
	size_t uReadBufferPos;
	const size_t uShutdownHeaderSize = 5,
					uShutdownMessageSize = 26;

	if (!WSSocketSSL_IsSSL(pThis))
		return WSSocket_Read(&pThis->mSuper, out, numOut, inblock);

	pReadBuffer = pThis->mReadBuf;
	pReserveReadBuffer = pThis->mReadBackBuf;
	if (pReserveReadBuffer->size())
	{
		if (pReserveReadBuffer->size() >= numOut)
		{
			Vector_Transfer((*pReserveReadBuffer),((BYTE*)out), numOut);
			return numOut;
		}
		uNumRead = pReserveReadBuffer->size();
		Vector_Transfer((*pReserveReadBuffer), ((BYTE*)out), pReserveReadBuffer->size());
		((BYTE*&)out) += uNumRead;
		numOut -= uNumRead;
	}
	else
		uNumRead = 0;

	uDataLen = uDataBufLen = 0;
	dataBuf = NULL;
	BuffDesc.ulVersion = SECBUFFER_VERSION;
	BuffDesc.cBuffers = 4;
	BuffDesc.pBuffers = SecBuff;
	uReadBufferPos = pReadBuffer->size();
	do
	{
		pReadBuffer->BeforeWrite(pReadBuffer->capacity());
		uRawNumRead = WSSocket_Read(&pThis->mSuper,
			&Vector_Data(*pReadBuffer)[uReadBufferPos],
			pReadBuffer->capacity() - uReadBufferPos, inblock);

		if (uRawNumRead == (size_t)SOCKET_ERROR)
		{
			if (uNumRead)
			{
				pReadBuffer->SetSize(uReadBufferPos);
				break;
			}
			return (size_t)SOCKET_ERROR;
		}
		if (uRawNumRead == 0)
		{
			if (uReadBufferPos)
			{
				pReadBuffer->SetSize(uReadBufferPos);
				return (size_t)SOCKET_ERROR;
			}
			break;
		}

		pReadBuffer->SetSize(uReadBufferPos += uRawNumRead);

		/* DATA->(EMPTY)*3 */
		SecBuff[0].pvBuffer = Vector_Data(*pReadBuffer);
		SecBuff[0].cbBuffer = (unsigned long)pReadBuffer->size();
		SecBuff[0].BufferType = SECBUFFER_DATA;
		SecBuff[1].BufferType = SecBuff[2].BufferType =
			SecBuff[3].BufferType = SECBUFFER_EMPTY;

		/* DecryptMessage can't handle bundled shutdown messages on old Windows */
		if (Vector_Data(*pReadBuffer)[0] == 21 /* 21 == SSL Alert */
			&& pReadBuffer->size() > (uShutdownMessageSize + uShutdownHeaderSize)
			&& Vector_Data(*pReadBuffer)[3] == 0
			&& Vector_Data(*pReadBuffer)[4] == uShutdownMessageSize
			&& Vector_Data(*pReadBuffer)[5] == 0)
		{
			WSLogDebug("GOT A BUNDLED SHUTDOWN MESSAGE!");
			SecBuff[0].cbBuffer = uShutdownMessageSize + uShutdownHeaderSize;
			scRet = (*pThis->mSSLLoader->DecryptMessageProc)(
				(_SecHandle*)&pThis->mCtx, &BuffDesc, 0, NULL);
			if (scRet == SEC_I_CONTEXT_EXPIRED)
			{
				SecBuff[1].pvBuffer =
					Vector_Data(*pReadBuffer) + uShutdownMessageSize + uShutdownHeaderSize;
				SecBuff[1].cbBuffer = (unsigned long)(size_t)
					(Vector_Data(*pReadBuffer) - uShutdownMessageSize - uShutdownHeaderSize);
				SecBuff[1].BufferType = SECBUFFER_EXTRA;
			}
		}
		else
			scRet = (*pThis->mSSLLoader->DecryptMessageProc)(
				(_SecHandle*)&pThis->mCtx, &BuffDesc, 0, NULL);

		if (scRet == SEC_E_INCOMPLETE_MESSAGE)
		{
			shouldContinue = TRUE;
			continue;
		}
		if (scRet != SEC_E_OK && scRet != SEC_I_RENEGOTIATE && scRet != SEC_I_CONTEXT_EXPIRED)
		{
			pReadBuffer->resize(0);
			return (size_t)SOCKET_ERROR;
		}

		pDataBuffer = NULL;
		pExtraBuffer = NULL;
		for (size_t i = 1; i < 4; i++)
		{
			if (pDataBuffer == NULL && SecBuff[i].BufferType == SECBUFFER_DATA)
				pDataBuffer = &SecBuff[i];
			if (pExtraBuffer == NULL && SecBuff[i].BufferType == SECBUFFER_EXTRA)
				pExtraBuffer = &SecBuff[i];
		}

		if (pDataBuffer) /* unencrypted data */
		{
			if ((uDataLen + (pDataBuffer->cbBuffer)) > uDataBufLen)
			{
				BYTE* newDataBuf =
					WSALLOC(BYTE, uDataBufLen += pDataBuffer->cbBuffer);
				if(!newDataBuf)
					return (size_t)SOCKET_ERROR;
				WSMemoryCopy(newDataBuf, dataBuf, uDataLen);
				WSFREE(dataBuf);
				dataBuf = newDataBuf;
			}
			WSMemoryCopy(dataBuf + uDataLen, pDataBuffer->pvBuffer, pDataBuffer->cbBuffer);
			uDataLen += pDataBuffer->cbBuffer;
		}

		if (pExtraBuffer)
		{
			pReadBuffer->resize(pExtraBuffer->cbBuffer);
			WSMemoryMove(Vector_Data(*pReadBuffer), pExtraBuffer->pvBuffer, pExtraBuffer->cbBuffer);
			shouldContinue = TRUE;
			continue;
		}

		shouldContinue = FALSE;

		if (scRet == SEC_I_RENEGOTIATE)
		{
			if (WSSocketSSL_DoConnectOrAccept((WSSOCKETSSL*)pThis, SSPIconnectProc,
					NULL, &HandshakeBuffer) != true)
			{
				pReadBuffer->SetSize();
				return (size_t)SOCKET_ERROR;
			}

			if (HandshakeBuffer.pvBuffer)
			{
				pReadBuffer->resize(HandshakeBuffer.cbBuffer);
				WSMemoryCopy(Vector_Data(*pReadBuffer), HandshakeBuffer.pvBuffer, HandshakeBuffer.cbBuffer);
			}
			else
				pReadBuffer->SetSize();

			if (HandshakeBuffer.pvBuffer)
				WSFREE(HandshakeBuffer.pvBuffer);
		}
		else
			pReadBuffer->SetSize();
	} while (shouldContinue);

	if (uDataLen)
	{
		if (uDataLen > numOut)
		{
			pReserveReadBuffer->resize(uDataLen - numOut);
			WSMemoryCopy(out, dataBuf, numOut);
			WSMemoryCopy(Vector_Data(*pReserveReadBuffer), &dataBuf[numOut],
						 pReserveReadBuffer->size());
			uNumRead += numOut;
		}
		else
		{
			WSMemoryCopy(out, dataBuf, uDataLen);
			uNumRead += uDataLen;
		}

		WSFREE(dataBuf);
	}

	return uNumRead;
}

size_t WSSocketSSL_Write(const WSSOCKETSSL* pThis,
	const void* pWriteBuffer, size_t uToWrite, wsbool* inblock)
{
	SecBufferDesc				BuffDesc;
	SecBuffer					SecBuff[4];
	DWORD dwDataLeft;
	DWORD dwCurrentPart, dwSendPos;
	size_t uTrailerSize;
	if (!WSSocketSSL_IsSSL(pThis))
		return WSSocket_Write(&pThis->mSuper,
			pWriteBuffer, uToWrite, inblock);
	uTrailerSize =
		pThis->mReadBuf->capacity() - (pThis->mMaxMessageSize + pThis->mHeaderSize);
	ScopedBytes pbOutBuffer(pThis->mReadBuf->capacity()); /* Head + Message + Trailing */
	dwDataLeft = (DWORD)uToWrite;
	dwSendPos = 0;

	BuffDesc.ulVersion = SECBUFFER_VERSION;
	BuffDesc.cBuffers = 4;
	BuffDesc.pBuffers = SecBuff;

	do
	{
		dwCurrentPart = ((DWORD)uToWrite)-dwSendPos > pThis->mMaxMessageSize ?
			pThis->mMaxMessageSize : (((DWORD)uToWrite)-dwSendPos);
		WSMemoryFill(((BYTE*)pbOutBuffer), 0, pThis->mHeaderSize);
		WSMemoryCopy(((BYTE*)pbOutBuffer) + pThis->mHeaderSize, ((BYTE*)pWriteBuffer) + dwSendPos,
			dwCurrentPart);
		WSMemoryFill(((BYTE*)pbOutBuffer) + pThis->mHeaderSize + dwCurrentPart, 0, uTrailerSize);
		dwSendPos += dwCurrentPart;
		dwDataLeft -= dwCurrentPart;
		/*  Prepare buffers STREAM_HEADER->DATA->STREAM_TRAILER->EMPTY */
		SecBuff[0].BufferType = SECBUFFER_STREAM_HEADER;
		SecBuff[0].pvBuffer = ((BYTE*)pbOutBuffer);
		SecBuff[0].cbBuffer = pThis->mHeaderSize;

		SecBuff[1].BufferType = SECBUFFER_DATA;
		SecBuff[1].pvBuffer = ((BYTE*)pbOutBuffer) + pThis->mHeaderSize;
		SecBuff[1].cbBuffer = dwCurrentPart;

		SecBuff[2].BufferType = SECBUFFER_STREAM_TRAILER;
		SecBuff[2].pvBuffer = ((BYTE*)pbOutBuffer) + pThis->mHeaderSize + dwCurrentPart;
		SecBuff[2].cbBuffer = (DWORD)uTrailerSize;

		SecBuff[3].BufferType = SECBUFFER_EMPTY;
		if ((*pThis->mSSLLoader->EncryptMessageProc)(
			(_SecHandle*)&pThis->mCtx, 0, &BuffDesc, 0) < SEC_E_OK)
		{
			WSLogError("EncryptMessage failed");
			return (size_t)SOCKET_ERROR;
		}

		if (WSSocket_Write(&pThis->mSuper, pbOutBuffer,
				SecBuff[0].cbBuffer + SecBuff[1].cbBuffer + SecBuff[2].cbBuffer,
				inblock)
			!= SecBuff[0].cbBuffer + SecBuff[1].cbBuffer + SecBuff[2].cbBuffer)
			return (size_t)SOCKET_ERROR;
	} while (dwDataLeft);
	return uToWrite;
}

wsbool WSSocketSSL_IsCreated(const WSSOCKETSSL* pThis)
{
	return WSSocketSSL_IsSSL(pThis)
		|| (!WSSocketSSL_IsSSL(pThis) && WSSocket_IsCreated(&pThis->mSuper));
}
wsbool WSSocketSSL_IsSSL(const WSSOCKETSSL* pThis)
{
	return pThis->mCtx.dwUpper != 0;
}

#endif /*defined(WSSSLOPT) && defined(_WIN32) && defined(WSNTVSSLOPT)*/
/*#############################################################################
				Cleanup
#############################################################################*/
#if !defined(WIKISERVER_VERSION)
	#undef LPWSDLLPROC
	#undef WSDLL_Create
	#undef WSDLL_IsLoaded
	#undef WSDLL_GetProc
	#undef WSDLL_Destroy
	#undef SckWinFunctionBase
	#undef WSDLL
	#ifdef WSFILE
		#undef WSFILE
		#undef WSFile_Create
		#undef WSFile_Destroy
		#undef WSFile_Read
	#endif
	#ifdef URIParseIPv4address
		#define URIIsDigit SocketURIIsDigit
		#define URIParseIPv4address SocketURIParseIPv4address
	#endif
#endif
/*#############################################################################
				C++ Guard
#############################################################################*/
#ifdef __cplusplus
}
#endif
/******************************************************************************
	 #     #    #     #####  ### #     # ####### #######  #####  #     #
	 ##   ##   # #   #     #  #  ##    #    #    #     # #     # #     #
	 # # # #  #   #  #        #  # #   #    #    #     # #       #     #
	 #  #  # #     # #        #  #  #  #    #    #     #  #####  #######
	 #     # ####### #        #  #   # #    #    #     #       # #     #
	 #     # #     # #     #  #  #    ##    #    #     # #     # #     #
	 #     # #     #  #####  ### #     #    #    #######  #####  #     #
******************************************************************************/
#else /* defined(macintosh) */
/**
	@file socket.c

	###Abstract###

	Mac Classic/Carbon implementation for Socket/SocketNotifier.

	###Notes####

	Mac Classic sockets function quite differently from Winsock/Berkeley
	sockets.

	1) Rather than polling a group of socket descriptors Mac sockets
	are entirely callback-based at interrupt time. I.E. you can't _really_
	"select" a Mac socket for the most part.

	2) While WSAAsyncSelect is also callback-based in a sense,
	anything can be done in that callback as it is a normal callback
	at application time. Since Mac socket callbacks are at interrupt time
	you cannot allocate memory so are mostly limited to moving around
	addresses and minor math operations on variables. System calls and/or
	memory allocation at interrupt time can crash the entire system.

	For more information about interrupt time see the following articles:

	"Technical Note TN1104 Interrupt-Safe Routines"
	https://developer.apple.com/library/archive/technotes/tn/tn1104.html

	"Technical Note TN1137 Disabling Interrupts on the Traditional Mac OS"
	https://developer.apple.com/library/archive/technotes/tn/tn1137.html

	"Atomicity Concurrent Data Access Without Blowing Up. jon@redshed.net"
	https://web.archive.org/web/20010221131009/
	http://redshed.net/macHack/99/atomicity.html
*/
/*#include "system.h"
#ifndef PRECOMPILEDHEADERS
	#include "server.h"
#endif*/
#if defined(__cplusplus)
extern "C" {
#endif

/*#############################################################################
						Definitions
#############################################################################*/
#if !defined(WSLogDebug) && !defined(WIKISERVER_VERSION)
	#define WSLogDebug(x)
	#define WSLogError(x)
#endif
#if !defined(WIKISERVER_VERSION)
#ifdef __MACH__
	#include <Carbon/Carbon.h>
#else
	#include <Gestalt.h>
	#include <DateTimeUtils.h>
	#include <Traps.h>
	#include <Patches.h>
	#include <Threads.h>
#endif
#define WIKIDEBUGVERIFY(x, y, z) if((y) != (z)) \
	{printf("%s failed with result [%zu]]", (x), (z));}
#if defined(__MWERKS__) || !defined(__MACH__)
	#define wspascal pascal /*  PB/XCode have no support for pascal */
#else
	#define wspascal /*  FUNCTION_PASCAL is the metric UH uses */
#endif

#if defined(macintosh) || defined(__MACH__)
	/*	Mac toolbox datetime epoch is
		2082844800 milliseconds from Unix->Mac epoch
		I.E. 1904 vs. 1970: 66 years of 356 days + 17 leap days */
	#define WSMAC_TO_UNIX_TIME ((unsigned long)2082844800L)
	#define WSMacToUnixTime(x) ((x) - WSMAC_TO_UNIX_TIME)
	#define WSUnixToMacTime(x) ((x) + WSMAC_TO_UNIX_TIME)
#else
	#define WSMAC_TO_UNIX_TIME 0
	#define WSMacToUnixTime(x) (x)
	#define WSUnixToMacTime(x) (x)
#endif
#define WSTimeGetLocalValue WSSocketTimeGetLocalValue
size_t WSSocketTimeGetLocalValue()
{
	/*
		3 ways:
		1) Microseconds - returns local time, conv. to seconds
			and take that off of the GetDateTime. However,
			OS7(?)+ only and doesn't work in sleep mode on OSX
			http://lists.apple.com/archives/Carbon-development/
			2002/Nov/msg00819.html
		2) ROM/ReadLocation - "Time" global variable
		3) GetLocalDateTime - OSX(?) only
	*/
	unsigned long datetime;
	GetDateTime(&datetime);
	return WSMacToUnixTime(datetime);
}
#define WSThreadYield WSSocketThreadYield
#if !defined(TARGET_API_MAC_CARBON) || !TARGET_API_MAC_CARBON
	static short SocketNumToolboxTraps(void)
	{
		if (NGetTrapAddress(0xA86E, ToolTrap) == NGetTrapAddress(0xAA6E, ToolTrap))
			return 0x200;
		else
			return 0x400;
	}
	static TrapType SocketGetTrapType(short theTrap)
	{
		/* OS traps start with A0, Tool with A8 or AA. */
		if ((theTrap & 0x0800) == 0)
			return OSTrap;
		else
			return ToolTrap;
	}
	static Boolean SocketTrapExists(short theTrap)
	{
		TrapType theTrapType;
		theTrapType = SocketGetTrapType(theTrap);
		if ((theTrapType == ToolTrap)
			&& ((theTrap &= 0x07FF) >= SocketNumToolboxTraps()))
			theTrap = _Unimplemented; /* 0xA89F */
		return(NGetTrapAddress(_Unimplemented, ToolTrap)
			   != NGetTrapAddress(theTrap, theTrapType));
	}
#endif
void WSThreadYield()
{
#if !defined(TARGET_API_MAC_CARBON) || !TARGET_API_MAC_CARBON
	long response;
	if (
		!SocketTrapExists(0xA1AD) /* 0xA1AD == _Gestalt */ ||
		Gestalt(gestaltThreadMgrAttr, &response) != noErr)
		response = 0;
	/*SystemTask();*/ /* Docs say never to call if using WaitNextEvent() */
	if((response & (1 << gestaltThreadMgrPresent)) != 0)
#endif
		/*WindowPtr idleWindow = FrontWindow();
		if (idleWindow)
			IdleControls(idleWindow); *//* AppareanceLib 1 (7+Lib or 8) */
		YieldToAnyThread(); // TODO: check OSErr?
}
/*#############################################################################
				Doubly-linked list macros
#############################################################################*/
/* Adds a node to a doubly-linked list without a dedicated root node */
#define DList_AddWithoutRoot(top, s, next, prev)\
	{ \
		/* Link the new node (s) after the last node (*top)->prev */ \
		(*top)->prev->next = s; \
		/* Set the new node's previous link to the last node */ \
		s->prev = (*top)->prev; \
		/* Update the last node's previous link to the new node */ \
		(*top)->prev = s; \
		/* Set the new node's next link to the first node */ \
		s->next = (*top); \
	}

/* Removes a node from a doubly-linked list without a dedicated root node */
#define DList_RemoveWithoutRoot(top, s, next, prev)\
	{ \
		/* Link the previous node to the next node*/ \
		/* bypassing the current node (s) */ \
		s->prev->next = s->next; \
		/* Link the next node's previous to the previous node, */ \
		/* removing current node (s) from the list */ \
		s->next->prev = s->prev; \
	}

/* Initializes a rootless doubly-linked list */
#define DList_InitRootless(top, next, prev)\
	{ \
		/* Set the next and previous pointers of the */ \
		/* first node to point to itself */ \
		(*top)->next = (*top)->prev = (*top); \
	}

/* Base macro for adding a node to the list,
used by both DList_Update and DList_Add */
#define DList_AddBase(top, s, next, prev, adding)\
	{ \
		/* Check if the list is not empty */ \
		if((*top))\
		{ \
			/* If not empty, use the DList_AddWithoutRoot to add the node */ \
			DList_AddWithoutRoot(top, s, next, prev);\
		} \
		else \
		{ \
			/* If empty, set the top to the new node */ \
			(*top) = s; \
			/* If adding, initialize the node to point to itself */ \
			if(adding)\
				s->next = s->prev = s; \
		} \
	}

/* Updates a doubly-linked list by adding a node */
#define DList_Update(top, s, next, prev) \
	DList_AddBase(top, s, next, prev, FALSE)

/* Adds a new node to the doubly-linked list */
#define DList_Add(top, s, next, prev) DList_AddBase(top, s, next, prev, TRUE)

/* Removes a node from the doubly-linked list */
#define DList_Remove(top, s, next, prev) \
	{ \
		/* If the node to remove is the first node */ \
		if(s == *top)\
		{ \
			/* If it's the only node, set the list to NULL */ \
			if((*top)->next == *top)\
				*top = NULL;\
			else \
				/* Otherwise, set the top to the next node */ \
				*top = (*top)->next;\
		}\
		/* Use the DList_RemoveWithoutRoot to remove the node */ \
		DList_RemoveWithoutRoot(top, s, next, prev); \
	}
#endif /* !WIKISERVER_VERSION */
/*=============================================================================
			ServerIOData/WSSOCKETNOTIFIERINFO(Open Transport + MacTCP)
=============================================================================*/

void WSSocketAddEntryToIOReq(WSSOCKETNOTIFIERINFO* entry)
{
	if (entry->mNextForEvent == entry) /* Make sure it isn't already queued */
	{
		DList_Add(&entry->mNotifier->mSockets,
					entry, mNextForEvent, mPrevForEvent);
	}
}
/*=============================================================================
					 ####### ######  ####### #     #
					 #     # #     # #       ##    #
					 #     # #     # #       # #   #
					 #     # ######  #####   #  #  #
					 #     # #       #       #   # #
					 #     # #       #       #    ##
					 ####### #       ####### #     #

	 ####### ######     #    #     #  #####  ######  ####### ######  #######
		#    #     #   # #   ##    # #     # #     # #     # #     #    #
		#    #     #  #   #  # #   # #       #     # #     # #     #    #
		#    ######  #     # #  #  #  #####  ######  #     # ######     #
		#    #   #   ####### #   # #       # #       #     # #   #      #
		#    #    #  #     # #    ## #     # #       #     # #    #     #
		#    #     # #     # #     #  #####  #       ####### #     #    #
=============================================================================*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

						Open Transport

	Refs:
	http://lists.apple.com/archives/mac-games-dev/2002/Jul/msg00063.html
	http://lists.apple.com/archives/Opentransportdev/2002/Jan/msg00101.html
	http://developer.apple.com/samplecode/OT_Virtual_Server/listing2.html
	Some good examples include the Putty Open Transport backend

	#1 thing with Open Transport is that you MUST read all data from
	OTRcv() up to kOTNoDataErr, otherwise the recipient gets NOTHING.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#if defined(WSMACUSEOPENTRANSPORT)

/* Vanilla to [XXX]InContext macros */
#if defined(WSMACCARBON) && !defined(OTCARBONAPPLICATION)
	#define OTCARBONAPPLICATION 1
#endif

/* Headers */
#ifdef __MACH__
	#include <Carbon/Carbon.h>
#else
	#include <Files.h>
	#include <OpenTransport.h>
	#include <OpenTptInternet.h> /* InetAddress and friends */
	#include <Gestalt.h>
#endif

/* SOCKET and types */
#define SOCKET EndpointRef
#define SOCKET_ERROR ((size_t)-1)
#define WSSOCKETTYPE void*

#define WSOTDEBUG(x) WSLogDebug(x)

/* MWERKS MACH fixup */
#if defined(WSMACCARBON) && defined(OTCARBONAPPLICATION) \
&& !defined(InitOpenTransport)
	#define InitOpenTransport() \
		InitOpenTransportInContext(kInitOTForApplicationMask, NULL)
	#define CloseOpenTransport() CloseOpenTransportInContext(NULL)
	#define OTOpenEndpoint(config, oflag, info, err) \
		OTOpenEndpointInContext(config, oflag, info, err, NULL)
	#define OTAsyncOpenEndpoint(config, oflag, info, proc, userData) \
		OTAsyncOpenEndpointInContext(config, oflag, info, proc, userData, NULL)
	#define OTOpenInternetServices(cfig, oflags, err) \
		OTOpenInternetServicesInContext(cfig, oflags, err, NULL)
	#define OTAsyncOpenInternetServices(cfig, oflags, proc, userData) \
		OTAsyncOpenInternetServicesInContext(cfig, oflags, \
											 proc, userData, NULL)
	#define OTAlloc(ref, structType, fields, err) \
		OTAllocInContext(ref, structType, fields, err, NULL)
#endif

/*=============================================================================

						WSSOCKETLIBRARY (Open Transport)

=============================================================================*/
wsbool WSSocketLibrary_Create(WSSOCKETLIBRARY* pThis)
{
	long val;
	/*long lOTVersion;*/
	OSErr err = InitOpenTransport();
	if (err != noErr)
	{
		WSLogError("Couldn't INIT Open Transport!");
		return false;
	}
	val = 0;
	Gestalt('otan', &val);

	/*WSLogDebug(MutStr("OT Gestalt:[") << val << "]");*/

	err = Gestalt('otvr', &pThis->mOTVersion);
	if (err)
	{
		WSLogError("Couldn't Gestalt Open Transport!");
		return false;
	}
	WSLogDebug("Open Transport INITED");
	return TRUE;
}

void WSSocketLibrary_Destroy(WSSOCKETLIBRARY* pThis)
{
	CloseOpenTransport();
}

/*-----------------------------------------------------------------------------
			Get/SetFourByteOption (IM: Networking With Open Transport)
-----------------------------------------------------------------------------*/

static OTResult GetFourByteOption(EndpointRef ep,
	OTXTILevel level, OTXTIName name, UInt32* value)
{
	OTResult err;
	TOption option;
	TOptMgmt request;
	TOptMgmt result;
	/* Set up the option buffer */
	option.len = kOTFourByteOptionSize;
	option.level = level;
	option.name = name;
	option.status = 0;
	option.value[0] = 0;/* Ignored because we're getting the value. */
	/* Set up the request parameter for OTOptionManagement to point
	 to the option buffer we just filled out */
	request.opt.buf = (UInt8*)&option;
	request.opt.len = sizeof(option);
	request.flags = T_CURRENT;
	/* Set up the reply parameter for OTOptionManagement. */
	result.opt.buf = (UInt8*)&option;
	result.opt.maxlen = sizeof(option);
	err = OTOptionManagement(ep, &request, &result);
	if(err == noErr)
	{
		switch(option.status)
		{
		case T_SUCCESS:
		case T_READONLY:
			*value = option.value[0];
			break;
		default:
			err = option.status;
			break;
		}
	}
	return (err);
}

static OTResult SetFourByteOption(EndpointRef ep,
	OTXTILevel level, OTXTIName name, UInt32 value)
{
	OTResult err;
	TOption option;
	TOptMgmt request;
	TOptMgmt result;
	/* Set up the option buffer to specify the option and value to set. */
	option.len = kOTFourByteOptionSize;
	option.level = level;
	option.name = name;
	option.status = 0;
	option.value[0] = value;
	/* Set up request parameter for OTOptionManagement */
	request.opt.buf = (UInt8*)&option;
	request.opt.len = sizeof(option);
	request.flags = T_NEGOTIATE;
	/* Set up reply parameter for OTOptionManagement. */
	result.opt.buf = (UInt8*)&option;
	result.opt.maxlen = sizeof(option);
	err = OTOptionManagement(ep, &request, &result);
	if(err == noErr)
	{
		if(option.status != T_SUCCESS)
			err = option.status;
	}
	return (err);
}

/******************************************************************************
						OTSocketCreate
******************************************************************************/

WSSOCKETTYPE OTSocketCreate(const char* path)
{
	/* Create configuration for endpoint */
	EndpointRef ref;
	OSStatus err;
	struct OTConfiguration* cfg = OTCreateConfiguration(path);
	if (!cfg)
	{
		WSLogError("Couldn't create open transport tcp configuration");
		return kOTInvalidEndpointRef;
	}

	/* Open endpoint - it takes ownership of cfg on success */
	ref = OTOpenEndpoint(cfg, 0, NULL, &err);
	if (!ref)
	{
		WSLogError("Couldn't create EndPoint.");
		/*Error [") << (size_t)err << "]");*/
		OTDestroyConfiguration(cfg);
		return kOTInvalidEndpointRef;
	}
	return (WSSOCKETTYPE) ref;
}

/*****************************************************************************
						Socket_RcvDisconnect
******************************************************************************/
wsbool Socket_RcvDisconnect(WSSOCKETTYPE s)
{
	OSStatus err = OTRcvDisconnect((SOCKET)s, NULL);
	WIKIDEBUGVERIFY("OTRcvDisconnect", err, noErr);
	return err == noErr;
}

/******************************************************************************
						Socket_RcvOrderlyDisconnect
******************************************************************************/
wsbool Socket_RcvOrderlyDisconnect(WSSOCKETTYPE s)
{
	OSStatus err = OTRcvOrderlyDisconnect((SOCKET)s);
	WIKIDEBUGVERIFY("OTRcvOrderlyDisconnect", err, noErr);
	return err == noErr;
}

/*=============================================================================
						 Socket (Open Transport)
=============================================================================*/

void WSSocket_Construct(WSSOCKET* pThis)
{ pThis->mhSocket = (kOTInvalidEndpointRef); }
wsbool WSSocket_IsCreated(const WSSOCKET* pThis)
{ return pThis->mhSocket != kOTInvalidEndpointRef; }
wsbool WSSocket_SetNonBlocking(WSSOCKET* pThis, wsbool nonBlock)
{
	OSStatus err;
	if(nonBlock)
		err = OTSetNonBlocking((SOCKET)pThis->mhSocket);
	else
		err = OTSetBlocking((SOCKET)pThis->mhSocket);

	WIKIDEBUGVERIFY((nonBlock ? "OTSetNonBlocking" : "OTSetBlocking"), err, noErr);
	return err == noErr;
}
wsbool WSSocket_SetReuseAddr(WSSOCKET* pThis, wsbool reuseAddr)
{
	return SetFourByteOption((SOCKET)pThis->mhSocket, INET_IP, kIP_REUSEADDR,
		reuseAddr) == noErr;
}

void* WSSocket_Disown(WSSOCKET* pThis)
{
	WSSOCKETTYPE rets = pThis->mhSocket;
	pThis->mhSocket = kOTInvalidEndpointRef;
	return rets;
}

wsbool WSSocket_Accept(WSSOCKET* pThis, const WSSOCKET* server, WSSOCKETLIBRARY* socklib)
{ /* http://developer.apple.com/samplecode/OTMP/listing11.html */
	OSStatus res;
	TCall call;
	WSASSERT(server.IsCreated());
	WSASSERT(!IsCreated());

	/* Create endpoint */
	pThis->mhSocket = OTSocketCreate("tcp");
	if (pThis->mhSocket == kOTInvalidEndpointRef)
		return false;

	/* Listen + Accept incoming socket */
	/* Open Transport is far more involved than other APIs in */
	/* this case as it can */
	OTMemzero(&call, sizeof(TCall));
	res = OTListen((SOCKET)(WSSOCKETTYPE)server->mhSocket, &call);
	WIKIDEBUGVERIFY("OTListen", res, noErr);
	if (res == noErr)
	{
		res = OTAccept((SOCKET)(WSSOCKETTYPE)server->mhSocket, (SOCKET)pThis->mhSocket, &call);
		WIKIDEBUGVERIFY("OTAccept", res, noErr);
		if(res == noErr)
		{
			WSSocket_SetNonBlocking(pThis, TRUE);
			return true;
		}
	}

	/* In some cases with Open Transport accept will */
	/* fail and expect us to do a OTRcvDisconnect */
	/* in order to use the endpoint again */
	/* However, this only makes sense if you preallocate */
	/* sockets similar to Win32 AcceptEx() */
	if (res != kOTNoDataErr)
	{
		WIKIDEBUGVERIFY("OTListen", res, kOTLookErr);
		if (res == kOTLookErr)
		{
			OTResult lookRes = OTLook((SOCKET)server->mhSocket);
			WIKIDEBUGVERIFY("OTLook", lookRes, T_DISCONNECT);
			if (lookRes == T_DISCONNECT)
			{
				res = OTRcvDisconnect((SOCKET)server->mhSocket, NULL);
				WIKIDEBUGVERIFY("OTRcvDisconnect", res, noErr);
				if(res == noErr)
				{
					/* If we were pre-allocating sockets */
					/* we would put it on the stack here... */
				}
			}
		}
	}

	/* Failure/Cleanup */
	OTCloseProvider((SOCKET)pThis->mhSocket);
	pThis->mhSocket = kOTInvalidEndpointRef;
	return false;
}
wsbool WSSocket_CreateServer(WSSOCKET* pThis,
	size_t port, WSSOCKETLIBRARY* socklib)
{
	OSStatus res;
	TBind binder;
	InetAddress addr;
	WSASSERT(!WSSocket_IsCreated(pThis));

	/* Create endpoint. tilisten, 1.1.1+, makes it so that */
	/* OTAccept won't fail due to a pending T_LISTEN event - */
	/* only due to a pending T_DISCONNECT */
	pThis->mhSocket = OTSocketCreate("tilisten,tcp"); /* OT >=  1.1.1 */
	if(pThis->mhSocket == kOTInvalidEndpointRef)
		pThis->mhSocket = OTSocketCreate("tcp");
	if(pThis->mhSocket == kOTInvalidEndpointRef)
		return false;

	/* Now bind it to the port we want */
	OTInitInetAddress(&addr, port, kOTAnyInetAddress);
	binder.addr.len = sizeof(addr);
	binder.addr.buf = (UInt8*) &addr;
	binder.qlen = 100;
	res = OTBind((SOCKET)pThis->mhSocket, &binder, NULL); /*3rd returned address*/
	if (res != noErr)
	{
		OTCloseProvider((SOCKET)pThis->mhSocket);
		pThis->mhSocket = kOTInvalidEndpointRef;
		WIKIDEBUGVERIFY("OTBind", res, noErr);
		return false;
	}
	return true;
}
wsbool WSSocket_CreateClient(WSSOCKET* pThis,
	const char* szHostname, size_t port, WSSOCKETLIBRARY* socklib)
{ /* http://developer.apple.com/documentation/
  mac/NetworkingOT/NetworkingWOT-22.html */
	OSStatus res;
	InetHostInfo hi;
	InetSvcRef srvRef;
	WSASSERT(!WSSocket_IsCreated(pThis));

	/* Create the endpoint */
	pThis->mhSocket = OTSocketCreate("tcp");
	if (pThis->mhSocket == kOTInvalidEndpointRef)
		return false;

	/* Convert the hostname to an InetHostInfo */
	srvRef = OTOpenInternetServices(
		kDefaultInternetServicesPath, 0, &res);
	WIKIDEBUGVERIFY("OTOpenInternetServices", res, noErr);
	if (srvRef)
	{
		res = OTInetStringToAddress(srvRef, (char*)szHostname, &hi);
		WIKIDEBUGVERIFY("OTInetStringToAddress", res, noErr);
		OTCloseProvider(srvRef);
	}

	/* Bind and connect the socket */
	if (res == noErr)
	{
		res = OTBind((SOCKET)pThis->mhSocket, NULL, NULL);
		WIKIDEBUGVERIFY("OTBind", res, noErr);
		if (res == noErr)
		{
			TCall call;
			InetAddress addr;
			OTMemzero(&call, sizeof(TCall));
			OTInitInetAddress(&addr, port, hi.addrs[0]);
			call.addr.buf = (UInt8*)&addr;
			call.addr.len = sizeof(InetAddress);
			res = OTConnect((SOCKET)pThis->mhSocket, &call, NULL);
			WIKIDEBUGVERIFY("OTConnect", res, noErr);
		}

	}

	/* Cleanup */
	if (res != noErr)
	{
		OTCloseProvider((SOCKET)pThis->mhSocket);
		pThis->mhSocket = kOTInvalidEndpointRef;
		return false;
	}
	return true;
}
void WSSocket_Destruct(WSSOCKET* pThis)
{
	if(WSSocket_IsCreated(pThis))
		WSSocket_Destroy(pThis);
}
void WSSocket_Shutdown(WSSOCKET* pThis)
{
	WSASSERT(WSSocket_IsCreated(pThis));
	/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)pThis->mhSocket << "] Shutdown()");*/
	if (OTGetEndpointState((SOCKET)pThis->mhSocket) != T_IDLE)
	{
		/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)pThis->mhSocket << "] Shutdown() Sending Orderly");*/
		OTSndOrderlyDisconnect((SOCKET)pThis->mhSocket);
	}
	else
	{
		/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)pThis->mhSocket << "] Shutdown() NOT Sending Orderly");*/
	}
}
void WSSocket_Destroy(WSSOCKET* pThis)
{
	OSErr err;
	WSASSERT(WSSocket_IsCreated(pThis));
	/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)pThis->mhSocket << "] Destroy()");*/
	if (OTGetEndpointState((SOCKET)pThis->mhSocket) != T_IDLE)
		err = OTSndDisconnect((SOCKET)pThis->mhSocket, NULL);
	err = OTUnbind((SOCKET)pThis->mhSocket);
	if (OTCloseProvider((SOCKET)pThis->mhSocket) != noErr)
		{ WSLogDebug("Could not close socket"); }
	pThis->mhSocket = kOTInvalidEndpointRef;
}
#if defined(HAVE_MUTSTR)
wsbool SocketGetHostnameOrIPAddress(const void* mhSocket,
	WSMUTSTR* s, wsbool getlocal, wsbool gethostname)
{
	TBind localAddr, peerAddr;
	InetAddress localInetAddr, peerInetAddr;
	InetDomainName hostname;
	OSStatus err;

	/* Get local and peer addresses of the socket in net form */
	localAddr.addr.maxlen = sizeof(localInetAddr);
	localAddr.addr.buf = (unsigned char*) &localInetAddr;
	peerAddr.addr.maxlen = sizeof(peerInetAddr);
	peerAddr.addr.buf = (unsigned char*) &peerInetAddr;
	err = OTGetProtAddress((SOCKET)mhSocket, &localAddr, &peerAddr);
	if (err != noErr)
	{
		WIKIDEBUGVERIFY("OTGetProtAddress", err, noErr);
		return false;
	}

	/* If we are getting a hostname, open the internet services */
	/* and convert it to string form, otherwise just get the IP */
	if(gethostname)
	{
		InetSvcRef srvRef = OTOpenInternetServices(
			kDefaultInternetServicesPath, 0, &err);
		if (err != noErr)
		{
			WIKIDEBUGVERIFY("OTOpenInternetServices", err, noErr);
			return false;
		}
		err = OTInetAddressToName(srvRef,
			getlocal ? localInetAddr.fHost : peerInetAddr.fHost, hostname);
		if (err != noErr)
		{
			WIKIDEBUGVERIFY("OTInetAddressToName", err, noErr);
			return false;
		}
		OTCloseProvider(srvRef);
	}
	else
		OTInetHostToString(
			getlocal ? localInetAddr.fHost : peerInetAddr.fHost, hostname);

	/* Done */
	WSMutStr_AssignCString(s, hostname);
	return true;
}
wsbool WSSocket_GetHostname(const WSSOCKET* pThis, WSMUTSTR* s)
{  return SocketGetHostnameOrIPAddress(pThis->mhSocket, s, false, true); }
wsbool WSSocket_GetIPAddress(const WSSOCKET* pThis, WSMUTSTR* s)
{  return SocketGetHostnameOrIPAddress(pThis->mhSocket, s, false, false); }
wsbool WSSocket_GetLocalHostname(const WSSOCKET* pThis, WSMUTSTR* s)
{  return SocketGetHostnameOrIPAddress(pThis->mhSocket, s, true, true); }
wsbool WSSocket_GetLocalIPAddress(const WSSOCKET* pThis, WSMUTSTR* s)
{  return SocketGetHostnameOrIPAddress(pThis->mhSocket, s, true, false); }
#endif /* HAVE_MUTSTR */
size_t WSSocket_Read(const WSSOCKET* pThis, void* data, size_t num, wsbool* inblock)
{
	OTFlags retFlags;
	OTResult rlen;
	wsbool block = inblock ? *inblock : TRUE;
	/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] Entering Read");*/
	do
	{
		/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] OTRcv");*/
		rlen = OTRcv((SOCKET)pThis->mhSocket, (char*)data, (int)num, &retFlags);
		/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] Done with OTRcv");*/
		if (!rlen)
			return SOCKET_ERROR; /* connection is closed */
		if (rlen > 0)
		{
			/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis)*/
			/*	<< "] WSSocket_Read Success Of [" << (size_t)rlen << "] Bytes");*/
			return (size_t)rlen;
		}
		if (retFlags & T_MORE)
			continue;
		if (!block && (rlen == kEWOULDBLOCKErr || rlen == kOTNoDataErr))
		{
			/*if(rlen == kEWOULDBLOCKErr)*/
			/*	WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] OTRcv kEWOULDBLOCKErr");*/
			/*else*/
			/*	WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] OTRcv kOTNoDataErr");*/

			*inblock = TRUE;
			return SOCKET_ERROR;
		}
		if (rlen != kOTLookErr
			  && rlen != kOTNoDataErr && rlen != kEWOULDBLOCKErr)
			break; /* real error */
		/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] OTRcv->OTLook");*/
		/* We passed the common cases, figure out what's */
		/* going on from OTLook() */
		switch(OTLook((SOCKET)pThis->mhSocket))
		{
			case T_ORDREL:
				/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] OTRcv T_ORDREL");*/
				OTRcvOrderlyDisconnect((SOCKET)pThis->mhSocket);
				rlen = 0;
				break;
			case T_DISCONNECT:
				/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] OTRcv T_DISCONNECT");*/
				OTRcvDisconnect((SOCKET)pThis->mhSocket, NULL);
				rlen = 0;
				break;
			default:
				/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] OTRcv UNKNOWN OTLOOK()");*/
				if (rlen == kOTLookErr)
					rlen = 0;
				break;
		}
		WSOTDEBUG("[OT] OTRcv->OTLook DONE -> Yielding");
		if (rlen)
			WSThreadYield();
	} while (rlen == kOTNoDataErr || rlen == kEWOULDBLOCKErr);
	if (rlen)
	{
		/*if (rlen != kOTNoDataErr)*/
		/*	WSLogError(MutStr("OTRcv Error:[") << rlen << "]");*/
		/*else*/
		/*	WSLogError(MutStr("OTRcv met END OF STREAM, LOOK:[") << OTLook((SOCKET)pThis->mhSocket) << "] -> [" << retFlags << "]");*/
	}
	/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)WSSocket_GetHandle(pThis) << "] OTRcv done with ERROR");*/
	return SOCKET_ERROR;
}
wsbool WSSocket_WaitForRead(const WSSOCKET* pThis, size_t microsecs, wsbool* block)
{
	OTByteCount countBytes;
	OTResult state;
	size_t sleepTime;
	do
	{
		state = OTGetEndpointState((SOCKET)pThis->mhSocket);
		if(state == T_INCON)
			return true;
		state = OTCountDataBytes((SOCKET)pThis->mhSocket, &countBytes);
		if(countBytes > 0)
			return true;
		switch(state)
		{
		case kOTNoDataErr:
			/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)pThis->mhSocket
				<< "] kOTNoDataErr");*/
			break;
		case kEWOULDBLOCKErr:
			/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)pThis->mhSocket
				<< "] kEWOULDBLOCKErr");*/
			break;
		case noErr:
			/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)pThis->mhSocket
				<< "] WaitForRead: Success but 0 data");*/
			break;
		default:
			/*WSOTDEBUG(MutStr("[OT]: [") << (size_t)pThis->mhSocket*/
			/*	<< "] WaitForRead: Unknown Error [" << state << "]");*/
			break;
		}

		sleepTime = microsecs > 1000 ? 1000 : microsecs;
		if(sleepTime)
			WSThreadYield();
		microsecs -= sleepTime;
	} while(microsecs);

	if(state == kOTNoDataErr || state == kEWOULDBLOCKErr)
	{
		if(block)
			*block = true;
	}
	return false;
}
wsbool WSSocket_WaitForWrite(const WSSOCKET* pThis, size_t microsecs, wsbool* block)
{
	OTResult state;
	size_t sleepTime;
	do
	{
		state = OTGetEndpointState((SOCKET)pThis->mhSocket);
		if(state == T_DATAXFER || state == T_INREL)
			return true;
		sleepTime = microsecs > 1000 ? 1000 : microsecs;
		WSThreadYield();
		microsecs -= sleepTime;
	} while(microsecs);

	return false;
}
wsbool WSSocket_WaitForReadWrite(const WSSOCKET* pThis, size_t microsecs, wsbool* block)
{
	OTByteCount countBytes;
	OTResult state;
	size_t sleepTime;
	do
	{
		state = OTGetEndpointState((SOCKET)pThis->mhSocket);
		OTCountDataBytes((SOCKET)pThis->mhSocket, &countBytes);
		if(state == T_DATAXFER || state == T_INREL
				|| state == T_INCON || countBytes > 0)
			return true;
		sleepTime = microsecs > 1000 ? 1000 : microsecs;
		WSThreadYield();
		microsecs -= sleepTime;
	} while(microsecs);

	return false;
}
#if 1
#define SENDBLOCKSIZEVALUE 65535
#define SENDBLOCKSIZE(x) (x > SENDBLOCKSIZEVALUE ? SENDBLOCKSIZEVALUE : x)
#else
#define SENDBLOCKSIZE(x) (x)
#endif
size_t WSSocket_Write(const WSSOCKET* pThis, const void* dbufin, size_t dlenin, wsbool* inblock)
{
	const char* dbuf = (const char*)dbufin;
	OTResult dlen = (int)dlenin;
	OTResult olen = dlen;
	OTResult slen;
	wsbool block = inblock ? *inblock : TRUE;
	for (; ;)
	{
		slen = OTSnd((SOCKET)pThis->mhSocket, (void*)dbuf, SENDBLOCKSIZE(dlen), 0);
		if (slen >= 0)
		{
			if (slen == dlen)
			{/*I_FLUSH, FLUSHRW*/
				/*OTIoctl((SOCKET)pThis->mhSocket, ((MIOC_STREAMIO << 8) | 5), (void*)(0x01 | 0x02));*/
				/*OTSndDisconnect((SOCKET)pThis->mhSocket, NULL);*/
				return olen;
			}
			dbuf += slen;
			dlen -= slen;
		}
		else if (slen != kEWOULDBLOCKErr) /* && slen != kOTNoDataErr ?? */
			break;	/* proper error */
		else if (!block)
		{
			*inblock = TRUE;
			return SOCKET_ERROR;
		}
		WSThreadYield();
	}
	/*WSLogError(MutStr("OTSnd Error:[") << slen << "]");*/
	return SOCKET_ERROR;
}
int WSSocket_Error(const WSSOCKET* pThis)
{ return 0; }
#if defined(__cplusplus)
	#define DECLARE_HEAP_OTT(NAME, type, flagtype) \
	class NAME \
	{ \
	public: \
		NAME(EndpointRef ref, int fields) \
		{ \
			OSStatus err; \
			mData = (type*) OTAlloc(ref, flagtype, fields, &err); \
		} \
		~NAME() \
		{ \
			OTFree(mData, flagtype); \
		} \
	\
		operator type* () { return mData; } \
		type* operator -> () {  return mData; } \
		type* mData; \
	};
	DECLARE_HEAP_OTT(HeapTUnitData, TUnitData, T_UNITDATA)
#endif

wsbool WSSocket_CreateEndpoint(WSSOCKET* pThis,
	const char* szHostname, WSSOCKETLIBRARY* socklib)
{
	OSErr err;
	WSASSERT(!IsCreated());

	/* Create the endpoint */
	pThis->mhSocket = OTSocketCreate("udp");
	if(pThis->mhSocket == kOTInvalidEndpointRef)
		return false;

	/* Bind it to nothing, i.e. the local interface */
	/* for connecting to something else later */
	err = OTBind((SOCKET)pThis->mhSocket, NULL, NULL);
	WIKIDEBUGVERIFY("OTBind", err, noErr);
	if(err != noErr)
	{
		OTCloseProvider((SOCKET)pThis->mhSocket);
		pThis->mhSocket = kOTInvalidEndpointRef;
		return false;
	}
	return true;
}

size_t WSSocket_ReadEndpoint(const WSSOCKET* pThis,
	void* data, size_t num, wsbool* block)
{
	InetAddress sender;
	OTFlags flags;
	OTResult res;
	size_t ret;
	TUnitData* tdata = (TUnitData*)OTAlloc((SOCKET)pThis->mhSocket,
		T_UNITDATA, T_ADDR|T_OPT, &res);
	if(!tdata)
		return SOCKET_ERROR;
	tdata->udata.len = tdata->udata.maxlen = num;
	tdata->udata.buf = (UInt8*)(data);

	res = OTRcvUData((SOCKET)pThis->mhSocket, tdata, &flags);
	if(res < 0)
	{
		if(block)
			*block = res == kEWOULDBLOCKErr;
		ret = (size_t)-1;
	}
	else
		ret = tdata->udata.len;
	OTFree(tdata, T_UNITDATA);
	return ret;
}

size_t WSSocket_WriteEndpoint(const WSSOCKET* pThis,
	const void* data, size_t num, const char* szHostname, wsbool* block)
{
	InetAddress addr;
	InetAddress sender;
	OSStatus res;
	InetHostInfo hi;
	InetSvcRef srvRef;
	size_t ret;
	TUnitData* tdata = (TUnitData*)OTAlloc((SOCKET)pThis->mhSocket,
		T_UNITDATA, szHostname ? T_ADDR|T_OPT : T_OPT, &res);
	if(!tdata)
		return SOCKET_ERROR;
	tdata->udata.len = tdata->udata.maxlen = num;
	tdata->udata.buf = (UInt8*)(data);
	srvRef = OTOpenInternetServices(
		kDefaultInternetServicesPath, 0, &res);
	if (srvRef)
	{
		res = OTInetStringToAddress(srvRef, (char*)szHostname, &hi);
		OTCloseProvider(srvRef);
	}
	OTInitInetAddress(&addr, kOTAnyInetAddress, hi.addrs[0]);
	tdata->addr.buf = (UInt8*)&addr;
	tdata->addr.len = sizeof(InetAddress);

	res = OTSndUData((SOCKET)pThis->mhSocket, tdata);
	if(res < 0)
	{
		if(block)
			*block = res == kEWOULDBLOCKErr;
		ret =  (size_t)-1;
	}
	else
		ret = tdata->udata.len;
	OTFree(tdata, T_UNITDATA);
	return ret;
}
wsbool WSSocket_ClearReadBuffer(WSSOCKET* pThis, wsbool* inblock)
{
	char* buffer = WSALLOC(char, 1024);
	size_t numRead = (size_t)-1;
	wsbool block = inblock ? *inblock : WSSOCKET_NONBLOCKING;
	if(WSSocket_WaitForRead(pThis, 0, inblock))
	{
		do
		{ numRead = WSSocket_Read(pThis, buffer, 1024, &block); }
		while (numRead != (size_t)-1 && numRead != 0);
		if(inblock) *inblock = block;
	}
	WSFREE(buffer);
	return numRead != (size_t)-1;
}
#if defined(HAVE_MUTSTR)
wsbool WSSocketGetDNSServer(WSMUTSTR* dns)
{
	OSStatus err;
	InetInterfaceInfo info;
	err = OTInetGetInterfaceInfo(&info, kDefaultInetInterface);
	WIKIDEBUGVERIFY("OTInetGetInterfaceInfo", err, noErr);
	if(err != noErr)
		return false;
	WSMutStr_BeforeWrite(dns, sizeof(InetDomainName));
	OTInetHostToString(info.fDNSAddr, (char*)WSMutStr_GetCString(dns));
	WSMutStr_AfterWrite(dns, CStrLength(WSMutStr_GetCString(dns)));
	return true;
}
#endif /* HAVE_MUTSTR */
/*=============================================================================
				 #     #               #######  #####  ######
				 ##   ##   ##    ####     #    #     # #     #
				 # # # #  #  #  #    #    #    #       #     #
				 #  #  # #    # #         #    #       ######
				 #     # ###### #         #    #       #
				 #     # #    # #    #    #    #     # #
				 #     # #    #  ####     #     #####  #
=============================================================================*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

									MacTCP

	Similar to Open Transport you must read all data from TCPRcv up the
	MacTCP equivelent of kOTNoDataErr or the recipient gets NOTHING.

	NOTE: IO Completion routines use the address of the parameter block passed
	to PBControlAsync. So, if you use put the parameter block on the stack
	and pass that to PBControlAsync and it completes later, the system
	will dereference that stack parameter block that no longer exists
	and the system will freeze!

	This does have a side benefit though - since PBControlAsync always uses
	the address passed in, you can modify the parameter block (in this case
	TCPiopb or UDPiopb) after the PBControlAsync, for example updating the
	the IO Completion routine, and it updates as those variables updates rather
	than continuing to use the values passed in to PBControlAsync.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#else /* MacTCP */

#include <OSUtils.h>
#include <Errors.h>
#include <Files.h>
#include <Resources.h>
#include <Memory.h>
#include <Traps.h>
#include <Gestalt.h>
#include <Folders.h>
#include <ToolUtils.h>
#include <MacTCP.h>
#include <Devices.h>

#ifndef	_GestaltDispatch
#define	_GestaltDispatch _Gestalt
#endif

#ifdef SOCKET
#undef SOCKET
#endif
#define SOCKET TCPiopb*
#define SOCKET_ERROR ((size_t)-1)
#define WSSOCKETTYPE void*
#define STREAM_BUFFER_SIZE SHRT_MAX /* memory for MacTCP streams */
#define kDefTCPCreateRcvBufferSize 4096 /* MacTCP minimum */

/*#############################################################################
						WSMacTCPNotifyProc Forward Declare
#############################################################################*/
wspascal void WSMacTCPNotifyProc(StreamPtr tcpStream,
	unsigned short eventCode, Ptr userDataPtr,
	unsigned short terminReason, ICMPReport* icmpMsg);

/*=============================================================================

						WSSOCKETLIBRARY (MacTCP)

=============================================================================*/

wsbool WSSocketLibrary_Create(WSSOCKETLIBRARY* pThis)
{
	ip_addr localAddr;
	OSErr err;
	/* Init MacTCP */
	{
		ParamBlockRec pb;
		pb.ioParam.ioCompletion = NULL;
		pb.ioParam.ioNamePtr = (StringPtr)"\p.IPP";
		pb.ioParam.ioPermssn = fsCurPerm;
		if (PBOpenAsync(&pb) != noErr)
		{/*OpenDriver("\p.IPP", &mIORefNum) equiv*/
			/*WSLogError(MutStr("Server: Could not open MacTCP driver! Error:[")*/
			/*			<< pb.ioParam.ioResult << "]");*/
			return FALSE;
		}
		pThis->mIORefNum = pb.ioParam.ioRefNum;
	}
#if 1
	/* Get local address */
	{
		GetAddrParamBlock gapb;
		gapb.ioCompletion = NULL;
		gapb.csCode = ipctlGetAddr;
		gapb.ioCRefNum = pThis->mIORefNum;
		gapb.ioResult = 1;
		if ((err = PBControlSync((ParmBlkPtr)&gapb)) != noErr)
		{
			/*WSLogError(MutStr("Server: Could not get local address! Error:[")*/
			/*			<< err << "]");*/
			return FALSE;
		}
		localAddr = gapb.ourAddress;
	}
	/* Get the Network MTU (maximum transfer unit) size of the local address */
	/* 548 is by default for remote. 1024 is a good stab for the default. */
	{
		UDPiopb udppb;
		/* compute MTU (maximum transfer unit) size */
		udppb.ioCompletion = NULL;
		udppb.ioCRefNum = pThis->mIORefNum;
		udppb.csCode = UDPMaxMTUSize;
		udppb.csParam.mtu.remoteHost = localAddr;
		if ((err = PBControlSync((ParmBlkPtr)&udppb)) != noErr)
		{
			/*WSLogError(MutStr("Server: Could not get MTU size! Error:[")*/
			/*			<< err << "]");*/
			return FALSE;
		}
		pThis->mCreateBufferSize = (udppb.csParam.mtu.mtuSize * 4) + 1024;
	}
#else
	pThis->mCreateBufferSize = STREAM_BUFFER_SIZE;
#endif
	return TRUE;
}
int16_t WSSocketLibrary_GetMacTCPRefNum(WSSOCKETLIBRARY* pThis)
{
	return pThis->mIORefNum;
}
uint16_t WSSocketLibrary_GetMacTCPStreamSize(WSSOCKETLIBRARY* pThis)
{
	return pThis->mCreateBufferSize;
}
void WSSocketLibrary_Destroy(WSSOCKETLIBRARY* pThis)
{
	ParamBlockRec pb;
	pb.ioParam.ioCompletion = NULL;
	pb.ioParam.ioNamePtr = (StringPtr)"\p.IPP";
	pb.ioParam.ioPermssn = fsCurPerm;
	pb.ioParam.ioRefNum = pThis->mIORefNum;
	PBCloseAsync(&pb);
}

/*=============================================================================
								MacTCPDNR
	This loads the domain name resolver from the MacTCP driver and
	abstracts its functionality. A subset of this was in AddressXlation.h/dnr.c

	This includes a whole bunch of needed helper functions and definitions.
=============================================================================*/

/*#############################################################################
						DNR Driver Exported Functions
	Callbacks are pascal based, functions C based.
	These are extra elaborate for maximum capatability.
#############################################################################*/
enum
{
	OPENRESOLVER = 1,
	CLOSERESOLVER,
	STRTOADDR,
	ADDRTOSTR,
	ENUMCACHE,
	ADDRTONAME,
	HXINFO, /* deliberate - HINFO has symbol conflicts */
	MXINFO
};

#define NUM_ALT_ADDRS 4

typedef struct hostInfo
{
	int rtnCode;
	char cname[255];
	unsigned long addr[NUM_ALT_ADDRS];
};

/* EnumResultProcPtr callback for EnumCache */
typedef CALLBACK_API(void, EnumResultProcPtr)(struct cacheEntryRecord*, char*);
typedef STACK_UPP_TYPE(EnumResultProcPtr) EnumResultUPP;
enum { uppEnumResultProcInfo = kPascalStackBased
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(struct cacheEntryRecord*)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char*))) };
#define NewEnumResultUPP(userRoutine)					\
	(ResultUPP)NewRoutineDescriptor((ProcPtr)(userRoutine),		\
					uppEnumResultProcInfo,			\
					GetCurrentArchitecture())
#define DisposeEnumResultUPP(userUPP) DisposeRoutineDescriptor(userUPP)

/* ResultProcPtr callback for StrToAddr and AddrToName */
typedef CALLBACK_API(void, ResultProcPtr)(struct hostInfo*, char*);
typedef STACK_UPP_TYPE(ResultProcPtr) ResultUPP;
enum { uppResultProcInfo = kPascalStackBased
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(struct hostInfo*)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char*))) };
#define NewResultUPP(userRoutine)					\
	(ResultUPP)NewRoutineDescriptor((ProcPtr)(userRoutine),		\
					uppResultProcInfo,			\
					GetCurrentArchitecture())
#define DisposeResultUPP(userUPP) DisposeRoutineDescriptor(userUPP)

/* ResultProc2ProcPtr callback for HInfo and MXInfo */
typedef CALLBACK_API(void, ResultProc2ProcPtr)(struct returnRec*, char*);
typedef STACK_UPP_TYPE(ResultProc2ProcPtr) ResultProc2UPP;
enum { uppResultProc2ProcInfo = kPascalStackBased
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(struct returnRec*)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char*))) };
#define NewResultProc2UPP(userRoutine)					\
	(ResultUPP)NewRoutineDescriptor((ProcPtr)(userRoutine),		\
					uppResultProc2ProcInfo,			\
					GetCurrentArchitecture())
#define DisposeResultProc2UPP(userUPP) DisposeRoutineDescriptor(userUPP)

/* extern OSErr OpenResolver(char* fileName); */
typedef CALLBACK_API_C(OSErr, OpenResolverProcPtr)(UInt32, char *);
typedef STACK_UPP_TYPE(OpenResolverProcPtr) OpenResolverUPP;
enum { uppOpenResolverProcInfo = kCStackBased
	   | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UInt32)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char*))) };
#define InvokeOpenResolverUPP(selector, fileName, userUPP)		\
	CALL_TWO_PARAMETER_UPP((userUPP), uppOpenResolverProcInfo,		\
			   (selector), (fileName))

/* extern OSErr CloseResolver(); */
typedef CALLBACK_API_C(OSErr, CloseResolverProcPtr)(UInt32);
typedef STACK_UPP_TYPE(CloseResolverProcPtr) CloseResolverUPP;
enum { uppCloseResolverProcInfo = kCStackBased
	   | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UInt32))) };
#define InvokeCloseResolverUPP(selector, userUPP)			\
	CALL_ONE_PARAMETER_UPP((userUPP), uppCloseResolverProcInfo, (selector))

/* extern OSErr StrToAddr(char* hostName,
	struct hostInfo* hostInfoPtr, ResultUPP ResultProc, char* userDataPtr); */
typedef CALLBACK_API_C(OSErr, StrToAddrProcPtr)(UInt32, char*,
						struct hostInfo*, ResultUPP, char*);
typedef STACK_UPP_TYPE(StrToAddrProcPtr) StrToAddrUPP;
enum { uppStrToAddrProcInfo = kCStackBased
	   | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UInt32)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char*)))
	   | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(struct hostInfo*)))
	   | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ResultUPP)))
	   | STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(char*))) };
#define InvokeStrToAddrUPP(selector, hostName, hostInfoPtr, ResultProc,	\
			   userDataPtr, userUPP)			\
	CALL_FIVE_PARAMETER_UPP((userUPP), uppStrToAddrProcInfo, (selector),\
				(hostName), (hostInfoPtr), (ResultProc),	\
				(userDataPtr))

/* extern OSErr AddrToStr(unsigned long addr, char* addrStr); */
typedef CALLBACK_API_C(OSErr, AddrToStrProcPtr)(UInt32, unsigned long, char *);
typedef STACK_UPP_TYPE(AddrToStrProcPtr) AddrToStrUPP;
enum { uppAddrToStrProcInfo = kCStackBased
	   | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UInt32)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(unsigned long)))
	   | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(char*))) };
#define InvokeAddrToStrUPP(selector, addr, addrStr, userUPP)		\
	CALL_THREE_PARAMETER_UPP((userUPP), uppAddrToStrProcInfo, (selector),\
				 (addr), (addrStr))

/* extern OSErr EnumCache(EnumResultUPP enumResultProc, Ptr userDataPtr);  */
typedef CALLBACK_API_C(OSErr, EnumCacheProcPtr)(UInt32, EnumResultUPP, char*);
typedef STACK_UPP_TYPE(EnumCacheProcPtr) EnumCacheUPP;
enum { uppEnumCacheProcInfo = kCStackBased
	   | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UInt32)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(EnumResultUPP)))
	   | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(char*))) };
#define InvokeEnumCacheUPP(selector, enumResultProc, userDataPtr, userUPP) \
	CALL_THREE_PARAMETER_UPP((userUPP), uppEnumCacheProcInfo, (selector), \
				 (enumResultProc), (userDataPtr))

/* extern OSErr AddrToName(ip_addr addr, struct hostInfo* hostInfoPtr,
						   ResultUPP ResultProc, Ptr userDataPtr); */
typedef CALLBACK_API_C(OSErr, AddrToNameProcPtr)(UInt32, unsigned long,
	struct hostInfo*, ResultProcPtr, char*);
typedef STACK_UPP_TYPE(AddrToNameProcPtr) AddrToNameUPP;
enum { uppAddrToNameProcInfo = kCStackBased
	   | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UInt32)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(unsigned long)))
	   | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(struct hostInfo*)))
	   | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ResultUPP)))
	   | STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(char*))) };
#define InvokeAddrToNameUPP(selector, addr, hostInfoPtr, \
							resultProc, userDataPtr, userUPP) \
	CALL_FIVE_PARAMETER_UPP((userUPP), uppAddrToNameProcInfo, (selector),\
				 (addr), (hostInfoPtr), (resultProc), (userDataPtr))

/* extern OSErr HInfo(char* hostName, struct returnRec* returnRecPtr,
					  ResultProc2UPP resultProc, char* userDataPtr); */
typedef CALLBACK_API_C(OSErr, HInfoProcPtr)(UInt32, char*,
	struct returnRec*, ResultProc2ProcPtr, char*);
typedef STACK_UPP_TYPE(HInfoProcPtr) HInfoUPP;
enum { uppHInfoProcInfo = kCStackBased
	   | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UInt32)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char*)))
	   | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(struct returnRec*)))
	   | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ResultProc2UPP)))
	   | STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(char*))) };
#define InvokeHInfoUPP(selector, hostName, returnRecPtr, \
					   resultProc, userDataPtr, userUPP) \
	CALL_FIVE_PARAMETER_UPP((userUPP), uppHInfoProcInfo, (selector),\
				 (hostName), (returnRecPtr), (resultProc), (userDataPtr))

/* extern OSErr MXInfo(char* hostName, struct returnRec* returnRecPtr,
					   ResultProc2UPP resultProc, char* userDataPtr); */
typedef CALLBACK_API_C(OSErr, MXInfoProcPtr)(UInt32, char*,
	struct returnRec*, ResultProc2ProcPtr, char*);
typedef STACK_UPP_TYPE(MXInfoProcPtr) MXInfoUPP;
enum { uppMXInfoProcInfo = kCStackBased
	   | RESULT_SIZE(SIZE_CODE(sizeof(OSErr)))
	   | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(UInt32)))
	   | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(char*)))
	   | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(struct returnRec*)))
	   | STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(ResultProc2UPP)))
	   | STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(char*))) };
#define InvokeMXInfoUPP(selector, hostName, returnRecPtr, \
					   resultProc, userDataPtr, userUPP) \
	CALL_FIVE_PARAMETER_UPP((userUPP), uppMXInfoProcInfo, (selector),\
				(hostName), (returnRecPtr), (resultProc), (userDataPtr))

/******************************************************************************
	NumToolboxTraps/GetTrapType/TrapExists - Trap determination utilities
******************************************************************************/
static short NumToolboxTraps(void)
{
	if (NGetTrapAddress(0xA86E, ToolTrap) == NGetTrapAddress(0xAA6E, ToolTrap))
		return 0x200;
	else
		return 0x400;
}
static TrapType GetTrapType(short theTrap)
{
	/* OS traps start with A0, Tool with A8 or AA. */
	if ((theTrap & 0x0800) == 0)
		return OSTrap;
	else
		return ToolTrap;
}
static Boolean TrapExists(short theTrap)
{
	TrapType theTrapType;
	theTrapType = GetTrapType(theTrap);
	if ((theTrapType == ToolTrap)
		&& ((theTrap &= 0x07FF) >= NumToolboxTraps()))
		theTrap = _Unimplemented; /* 0xA89F */
	return(NGetTrapAddress(_Unimplemented, ToolTrap)
		   != NGetTrapAddress(theTrap, theTrapType));
}
/******************************************************************************
		GetSystemFolder->GetCPanelFolder->SearchFolderForDNRP->OpenMacTCPRF

	SearchFolderForDNRP is called to search a folder for files that might
	contain the 'dnrp' resource. Ultimately this leads to
	OpenMacTCPRF, which is called to open the MacTCP driver resources
******************************************************************************/
void GetSystemFolder(short *vRefNumP, long *dirIDP)
{
	SysEnvRec info;
	long wdProcID;

	SysEnvirons(1, &info);
	if (GetWDInfo(info.sysVRefNum, vRefNumP, dirIDP, &wdProcID) != noErr)
	{
		*vRefNumP = 0;
		*dirIDP = 0;
	}
}
void GetCPanelFolder(short *vRefNumP, long *dirIDP)
{
	Boolean hasFolderMgr = false;
	long feature;

	if (TrapExists(_GestaltDispatch))
		if (Gestalt(gestaltFindFolderAttr, &feature) == noErr)
			hasFolderMgr = true;
	if (!hasFolderMgr)
	{
		GetSystemFolder(vRefNumP, dirIDP);
		return;
	}
	else
	{
		if (FindFolder(kOnSystemDisk, kControlPanelFolderType,
					   kDontCreateFolder, vRefNumP, dirIDP) != noErr)
		{
			*vRefNumP = 0;
			*dirIDP = 0;
		}
	}
}
short SearchFolderForDNRP(long targetType, long targetCreator,
	short vRefNum, long dirID)
{
	HParamBlockRec fi;
	Str255 filename;
	short refnum;

	fi.fileParam.ioCompletion = nil;
	fi.fileParam.ioNamePtr = filename;
	fi.fileParam.ioVRefNum = vRefNum;
	fi.fileParam.ioDirID = dirID;
	fi.fileParam.ioFDirIndex = 1;

	while (PBHGetFInfo(&fi, false) == noErr)
	{
		/* scan system folder for driver resource files
		   of specific type & creator */
		if (fi.fileParam.ioFlFndrInfo.fdType == targetType &&
			fi.fileParam.ioFlFndrInfo.fdCreator == targetCreator)
		{
			/* found the MacTCP driver file? */
			refnum = HOpenResFile(vRefNum, dirID, filename, fsRdPerm);
			if (GetIndResource('dnrp', 1) == NULL)
				CloseResFile(refnum);
			else
				return refnum;
		}
		/* check mNext file in system folder */
		fi.fileParam.ioFDirIndex++;
		fi.fileParam.ioDirID = dirID; /* PBHGetFInfo() clobbers ioDirID */
	}
	return(-1);
}
short OpenMacTCPRF(void)
{
	short refnum;
	short vRefNum;
	long dirID;

	/* first search Control Panels for MacTCP 1.1 */
	GetCPanelFolder(&vRefNum, &dirID);
	refnum = SearchFolderForDNRP('cdev', 'ztcp', vRefNum, dirID);
	if (refnum != -1) return(refnum);

	/* mNext search the System Folder for MacTCP 1.0.x */
	GetSystemFolder(&vRefNum, &dirID);
	refnum = SearchFolderForDNRP('cdev', 'mtcp', vRefNum, dirID);
	if (refnum != -1) return(refnum);

	/* finally, search Control Panels for MacTCP 1.0.x */
	GetCPanelFolder(&vRefNum, &dirID);
	refnum = SearchFolderForDNRP('cdev', 'mtcp', vRefNum, dirID);
	if (refnum != -1) return(refnum);

	return -1;
}

/*=============================================================================
									MacTCPDNR
=============================================================================*/
typedef struct MacTCPDNR
{
	Handle mhDNR;
} MacTCPDNR;

	static OSErr MacTCPDNROpenResolver(Handle* hDNRIn, char *fileName)
	{
		short 			refnum;
		OSErr 			rc;
		Handle			hDNR;

		/* open the MacTCP driver to get DNR resources. Search for it based on
		   creator & type rather than simply file name */
		refnum = OpenMacTCPRF();

		/* ignore failures since the resource may have been installed in the
		   System file if running on a Mac 512Ke */

		/* load in the DNR resource package */
		hDNR = GetIndResource('dnrp', 1);
		if (hDNR == nil)
			/* can't open DNR */
			return(ResError());

		DetachResource(hDNR);
		if (refnum != -1)
			CloseResFile(refnum);

		/* lock the DNR resource since it cannot be relocated while opened */
		MoveHHi(hDNR);
		HLock(hDNR);

		/* call open resolver */
		rc = InvokeOpenResolverUPP(OPENRESOLVER, fileName, (OpenResolverUPP)*hDNR);
		if (rc != noErr)
		{
			/* problem with open resolver, flush it */
			HUnlock(hDNR);
			DisposeHandle(hDNR);
		}
		*hDNRIn = hDNR;
		return(rc);
	}

	static OSErr MacTCPDNRCloseResolver(Handle hDNR)
	{
		/* call close resolver */
		InvokeCloseResolverUPP(CLOSERESOLVER, (CloseResolverUPP)*hDNR);

		/* release the DNR resource package */
		HUnlock(hDNR);
		DisposeHandle(hDNR);
		return(noErr);
	}


	void MacTCPDNR_Construct(MacTCPDNR* pThis)
	{
		if(MacTCPDNROpenResolver(&pThis->mhDNR, NULL) != noErr)
			pThis->mhDNR = NULL;
	}
	void MacTCPDNR_Destruct(MacTCPDNR* pThis)
	{
		if(pThis->mhDNR)
			MacTCPDNRCloseResolver(pThis->mhDNR);
	}
	wsbool MacTCPDNR_IsCreated(MacTCPDNR* pThis)
	{
		return pThis->mhDNR != NULL;
	}

	OSErr MacTCPDNR_StrToAddr(MacTCPDNR* pThis,
		char* hostName, struct hostInfo* hostInfoPtr,
					 ResultUPP ResultProc, char* userDataPtr)
	{
		return InvokeStrToAddrUPP(STRTOADDR, hostName, hostInfoPtr, ResultProc,
								  userDataPtr, (StrToAddrUPP)*pThis->mhDNR);
	}
	OSErr MacTCPDNR_AddrToStr(MacTCPDNR* pThis,
		unsigned long addr, char* addrStr)
	{
		return InvokeAddrToStrUPP(ADDRTOSTR, addr, addrStr,
								  (AddrToStrUPP)*pThis->mhDNR);
	}

	OSErr MacTCPDNR_AddrToName(MacTCPDNR* pThis,
		unsigned long addr, struct hostInfo* hostInfoPtr,
					 ResultUPP resultProc, char* userDataPtr)
	{
		return InvokeAddrToNameUPP(ADDRTONAME, addr, hostInfoPtr, resultProc,
								   userDataPtr, (AddrToNameUPP)*pThis->mhDNR);
	}

	OSErr MacTCPDNR_EnumCache(MacTCPDNR* pThis,
		EnumResultUPP enumResultProc, char* userDataPtr)
	{
		return InvokeEnumCacheUPP(ENUMCACHE, enumResultProc, userDataPtr,
								  (EnumCacheUPP)*pThis->mhDNR);
	}

	OSErr MacTCPDNR_HInfo(MacTCPDNR* pThis,
		char* hostName, struct returnRec* returnRecPtr,
				ResultProc2UPP resultProc, char* userDataPtr)
	{
		return InvokeHInfoUPP(HXINFO, hostName, returnRecPtr, resultProc,
							  userDataPtr, (HInfoUPP)*pThis->mhDNR);
	}

	OSErr MacTCPDNR_MXInfo(MacTCPDNR* pThis,
		char* hostName, struct returnRec* returnRecPtr,
				 ResultProc2UPP resultProc, char* userDataPtr)
	{
		return InvokeMXInfoUPP(MXINFO, hostName, returnRecPtr, resultProc,
							  userDataPtr, (MXInfoUPP)*pThis->mhDNR);
	}



/*#############################################################################
		SingleWdsEntry/DNRCB are used with sending/endpoints/datastreams
#############################################################################*/
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
	#pragma pack(2)
#endif
	typedef struct SingleWdsEntry
	{
		unsigned short length;
		Ptr ptr;
		unsigned short end;
	} SingleWDSEntry;
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
	#pragma pack()
#endif
static wspascal void DNRCB(struct hostInfo *hi, char *cookie)
{
	volatile int *donep = (int *)cookie;
	*donep = TRUE;
}

/******************************************************************************
						Socket_Create
******************************************************************************/
StreamPtr Socket_CreateTCP(SOCKET pSocket, short ioRefNumTCP,
	UInt32 rcvBufferSize, TCPNotifyUPP notifyProc, Ptr userDataPtr)
{
	/*	IN csCode 26 word csCode = TCPCreate
		OUT tcpStream 28 long stream pointer
		IN csParam.create.rcvBuff 32 long pointer to receive buffer area
		IN csParam.create.rcvBuffLen 36 long length of receive buffer area
		IN csParam.create.notifyProc 40 long pointer to ASR
		IN csParam.create.userDataPtr 44 long user data pointer */
	pSocket->ioCRefNum = ioRefNumTCP;
	pSocket->csCode = TCPCreate;
	/* FIXME: The NewPtr call here scrambles memory in PPC */
	pSocket->csParam.create.rcvBuff = NewPtr(rcvBufferSize);
	pSocket->csParam.create.rcvBuffLen = rcvBufferSize;
	pSocket->csParam.create.notifyProc = notifyProc;
	pSocket->csParam.create.userDataPtr = (Ptr)pSocket;
	if (!pSocket->csParam.create.rcvBuff)
	{
		/*WSLogError(MutStr("Socket: [") << (size_t)pSocket << "] "*/
		/*	"Socket_Create could not allocate ["*/
		/*	<< rcvBufferSize << "]" "bytes for rcvBuff");*/
		return NULL;
	}
	if (PBControlSync((ParmBlkPtr)pSocket) != noErr)
	{
		/*WSLogError(MutStr("Socket: [") << (size_t)pSocket << "] "*/
		/*	"TCPCreate failed with error [" << pSocket->ioResult << "]");*/
		DisposePtr(pSocket->csParam.create.rcvBuff);
		return NULL;
	}
	/*WSLogDebug(MutStr("Socket: [") << (size_t)pSocket << "] "*/
	/*	"Socket_Create completed with ["*/
	/*	<< rcvBufferSize << "]" "bytes for rcvBuff at addr ["*/
	/*	<< (size_t)pSocket->csParam.create.rcvBuff << "]");*/
	/*pSocket->csParam.create.rcvBuff = 0;*//* Buffer is owned by MacTCP now */
	return pSocket->tcpStream;
}

/******************************************************************************
						Socket_CreateServer
	Creates a listening socket. Used to clone a socket after one gets
	a connection.
******************************************************************************/
wsbool Socket_CreateTCPServer(SOCKET pSocket, short ioRefNumTCP,
						size_t port, UInt32 rcvBufferSize,
						TCPNotifyUPP notifyProc, Ptr userDataPtr)
{
	if(!Socket_CreateTCP(pSocket, ioRefNumTCP, rcvBufferSize,
						 notifyProc, userDataPtr))
		return false;
	pSocket->csCode = TCPPassiveOpen;
	pSocket->ioCompletion = NULL;
	pSocket->csParam.open.validityFlags = timeoutValue | timeoutAction;
	pSocket->csParam.open.ulpTimeoutValue = 0; /* seconds */
	pSocket->csParam.open.ulpTimeoutAction = 0; /* 1:abort 0:report */
	pSocket->csParam.open.commandTimeoutValue = 0; /* infinity */
	pSocket->csParam.open.remoteHost = 0;
	pSocket->csParam.open.remotePort = 0;
	pSocket->csParam.open.localHost = 0;
	pSocket->csParam.open.localPort = port;
	pSocket->csParam.open.dontFrag = 0;
	pSocket->csParam.open.timeToLive = 0;
	pSocket->csParam.open.security = 0;
	pSocket->csParam.open.optionCnt = 0;
	pSocket->csParam.open.userDataPtr = NULL;
	if(PBControlAsync((ParmBlkPtr)pSocket) != noErr)
	{
		/*WSLogError(MutStr("Socket: [") << (size_t)pSocket << "] "*/
		/*	"TCPPassiveOpen failed with error [" << pSocket->ioResult << "]");*/
		return false;
	}
	return true;
}


/******************************************************************************
						Socket_GetTCPState
******************************************************************************/
#define TCPS_CLOSED			0	/* TCPSTATE_CLOSED */
#define TCPS_LISTEN			2	/* TCPSTATE_LISTENING */
#define TCPS_SYN_RECEIVED	4	/* TCPSTATE_ESTINCOMING */
#define TCPS_SYN_SENT		6	/* TCPSTATE_ESTOUTGOING */
#define TCPS_ESTABLISHED	8	/* TCPSTATE_UP */
#define TCPS_FIN_WAIT_1		10	/* TCPSTATE_UPNCLOSEISSUED */
#define TCPS_FIN_WAIT_2		12	/* TCPSTATE_UPNCLOSECOMPLETE */
#define TCPS_CLOSE_WAIT		14	/* TCPSTATE_UPNCLOSERECV */
#define TCPS_CLOSING		16	/* TCPSTATE_UPNCLOSING */
#define TCPS_LAST_ACK		18	/* TCPSTATE_UPNALMOSTCLOSED */
#define TCPS_TIME_WAIT		20	/* TCPSTATE_CONNBREAKING */
#define TCPS_INVALID		-1
OSErr Socket_GetTCPStatus(TCPiopb* pSocket, StreamPtr streamPtr)
{
	short oldCsCode = pSocket->csCode;
	OSErr err;
	WSASSERT(pTCPState != NULL);
	pSocket->csCode = TCPStatus;
	pSocket->tcpStream = streamPtr;
	pSocket->ioCompletion = NULL;
	err = PBControlAsync((ParmBlkPtr)pSocket);
	pSocket->csCode = oldCsCode;
	if(err != noErr)
	{
		PBKillIOSync((ParmBlkPtr)pSocket);
		return err;
	}
	return noErr;
}

OSErr Socket_GetTCPStateRaw(TCPiopb* pSocket, SInt8* pTCPState)
{
	TCPiopb pbStatus;
	OSErr err = Socket_GetTCPStatus(&pbStatus, pSocket->tcpStream);
	if(err != noErr)
		return err;
	*pTCPState = pbStatus.csParam.status.connectionState;
	return noErr;
}
SInt8 Socket_GetTCPStateSimple(TCPiopb* pSocket)
{
	SInt8 ret;
	if(Socket_GetTCPStateRaw(pSocket, &ret) == noErr)
		return ret;
	return TCPS_INVALID;
}

/******************************************************************************
						Socket_GetIPAddress
******************************************************************************/
#if defined(HAVE_MUTSTR)
wsbool Socket_GetIPAddress(SOCKET pSocket, MacTCPDNR* dnr,
	WSMUTSTR* sString, wsbool getlocal)
{
	WSMutStr_BeforeWrite(sString, 16);
	if(MacTCPDNR_AddrToStr(dnr,
		getlocal ? pSocket->csParam.open.localHost
					: pSocket->csParam.open.remoteHost,
					 (char*)WSMutStr_GetCString(sString)) != noErr)
		return false;
	WSMutStr_AfterWrite(sString, CStrLength(WSMutStr_GetCString(sString)));
	return true;
}
#endif /* HAVE_MUTSTR */
/******************************************************************************
						Socket_IsIOComplete
******************************************************************************/
wsbool Socket_IsIOComplete(SOCKET pSocket)
{
	return pSocket->ioResult <= 0;
}

/******************************************************************************
						Socket_IsListener
******************************************************************************/
wsbool Socket_IsListener(SOCKET pSocket)
{
	return pSocket->csCode == TCPPassiveOpen;
}

/******************************************************************************
						Socket_IsUDP
******************************************************************************/
wsbool Socket_IsUDP(SOCKET pSocket)
{
	return pSocket->csCode == UDPCreate
			|| pSocket->csCode == UDPRead
			|| pSocket->csCode == UDPWrite;
}

/******************************************************************************
						Socket_IOSync
******************************************************************************/
void Socket_IOSync(SOCKET pSocket)
{ /* This yields to other threads until the last IO operation is finished */
	while (!Socket_IsIOComplete(pSocket))
		WSThreadYield();
}

/******************************************************************************
						Socket_Close

	TCPS_CLOSED/LISTEN are the soft close states
******************************************************************************/
OSErr Socket_Close(WSSOCKETTYPE s, wsbool force)
{
	if (s)
	{
		OSErr res;
		SOCKET pSocket = (SOCKET) s;
		wsbool isUDP = Socket_IsUDP(pSocket);
		/*WSLogDebug(MutStr("Socket: [") << (size_t)s << "] CLOSING"*/
		/*		  << " TCPState [" << Socket_GetTCPStateSimple(pSocket) << "]");*/
		if(isUDP)
		{
			pSocket->csCode = UDPRelease;
		}
		else
		{
			/* If no TCPClose then they don't seem to actually get any data */
			if(force)
			{
				pSocket->csCode = TCPAbort;
			}
			else
			{
				pSocket->csCode = TCPClose;
				pSocket->csParam.close.validityFlags = timeoutValue | timeoutAction;
				pSocket->csParam.close.ulpTimeoutValue = 1 /* seconds */;
				pSocket->csParam.close.ulpTimeoutAction = 1 /* 1:abort 0:report */;
			}
		}
		res = PBControlAsync((ParmBlkPtr)pSocket);
		if(res == noErr)
			Socket_IOSync(pSocket);
		/*WSLogDebug(MutStr("Socket: [") << (size_t)s
					<< "] CLOSE RESULT:[" << res
					<<"] ASYNCRESULT:[" << pSocket->ioResult << "]"
					<< " TCPState [" << Socket_GetTCPStateSimple(pSocket)
					<< "]");
		*/
		return noErr;
	}
	return -1;
}

/******************************************************************************
						Socket_Send
******************************************************************************/
unsigned short Socket_Send(SOCKET pSocket,
	const void* dbufin, unsigned short dlenin, wsbool* inblock)
{
	OSErr err;
	wdsEntry theSendWDS[2];
	ptrdiff_t state = Socket_GetTCPStateSimple(pSocket);
	short ioCRefNum = pSocket->ioCRefNum;
	StreamPtr tcpStream = pSocket->tcpStream;
	WSMemoryFill(pSocket, 0, sizeof(TCPiopb));
	pSocket->ioCRefNum = ioCRefNum;
	pSocket->tcpStream = tcpStream;
	/*
	WSLogDebug(MutStr("Socket: [") << (size_t)pSocket
		<< "] SENDING [" << dlenin << "] bytes, state ["
		<< state*Socket_GetTCPStateSimple(pSocket)* << "]");
	WSLogDebug(MutStr("Socket: [") << (size_t)pSocket
		<< "] theSendWDS size:[" << sizeof(theSendWDS) << "]");
	*/
		WSASSERT(!Socket_IsListener(pSocket));
	if((pSocket->csCode == TCPSend
	   && pSocket->csParam.send.userDataPtr == (Ptr)inProgress)
	   == FALSE)
	{
		theSendWDS[0].length = dlenin;
		theSendWDS[0].ptr = (Ptr)dbufin;
		theSendWDS[1].length = 0;
		pSocket->csCode = TCPSend;
		pSocket->csParam.send.ulpTimeoutValue = 30;
		pSocket->csParam.send.ulpTimeoutAction = TRUE; /* abort */
		pSocket->csParam.send.validityFlags =
			(unsigned char) (timeoutValue | timeoutAction);
		pSocket->csParam.send.pushFlag = TRUE;
		pSocket->csParam.send.urgentFlag = FALSE;
		pSocket->csParam.send.wdsPtr = (Ptr)theSendWDS;
		if(inblock && *inblock == false)
			err = PBControlAsync((ParmBlkPtr)pSocket);
		else
			err = PBControlSync((ParmBlkPtr)pSocket);
	}
	else /* previous send in progress or finished, fallthrough */
	{
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pSocket*/
		/*	<< "] READ found previous pending read, falling through");*/
		err = noErr;
	}

	if (err == noErr)
	{
		if(!inblock || *inblock == true || pSocket->ioResult == noErr)
		{
			/*WSLogDebug(MutStr("Socket: [") << (size_t)pSocket*/
			 /*  << "] done with WRITE of [" */
			 /*  << theSendWDS[0].length << "] bytes"); */
			pSocket->csParam.send.userDataPtr = (Ptr)0;
			return theSendWDS[0].length;
		}
		else if (pSocket->ioResult == inProgress)
		{
			/*WSLogDebug(MutStr("Socket: [") << (size_t)pSocket*/
			 /*  << "] WRITE in progress [" << dlenin << "] bytes"); */
			if(inblock)
				*inblock = true;
			pSocket->csParam.send.userDataPtr = (Ptr)inProgress;
			return SOCKET_ERROR;
		}
	}
	/*else*/
	/*	WSLogError(MutStr("Socket: Write Error:[") << err << "]");*/
	pSocket->csParam.send.userDataPtr = (Ptr)0;
	return SOCKET_ERROR;
}

/******************************************************************************
						Socket_RcvDisconnect
******************************************************************************/
wsbool Socket_RcvDisconnect(SOCKET s)
{ /* TCPTerminate */
	return TRUE;
}

/******************************************************************************
						Socket_RcvOrderlyDisconnect
******************************************************************************/
wsbool Socket_RcvOrderlyDisconnect(SOCKET s)
{ /* TCPClosing */
	SInt8 state = Socket_GetTCPStateSimple(s);
	return state > 10 || state == TCPS_INVALID;
}

/*=============================================================================

								Socket (MacTCP)

=============================================================================*/
void WSSocket_Construct(WSSOCKET* pThis)
{ pThis->mhSocket = (WSALLOC1(SOCKET)); }
void WSSocket_Destruct(WSSOCKET* pThis)
{
	if(pThis->mhSocket)
	{
		if(WSSocket_IsCreated(pThis))
			WSSocket_Destroy(pThis);
		WSFREE(pThis->mhSocket);
	}
}
void WSSocket_Shutdown(WSSOCKET* pThis)
{
	/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket << "] Shutdown()");*/
	OSErr err = Socket_Close(pThis->mhSocket, false);
	if(err != noErr)
	{/*
		WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket
					<< "] Socket_Close in Shutdown() FAILED with code ["
					<< err << "]");
	*/}
}
void WSSocket_Destroy(WSSOCKET* pThis)
{
	/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket << "] Destroy()");*/
	SOCKET pSocket = (SOCKET)pThis->mhSocket;
	WSASSERT(pSocket);
	if(pSocket->csCode != 0)
	{
		OSErr err;
		if(pSocket->csCode == TCPPassiveOpen)
		{
			/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket << "] ================Destroy() killing a LISTENER!!================");*/
		}
		err = Socket_Close(pThis->mhSocket, true);
		if(err != noErr)
		{/*
			WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket
						<< "] Socket_Close in Destroy() FAILED with code ["
						<< err << "]");
		*/}
		pSocket->csCode = TCPRelease;
		err = PBControlAsync((ParmBlkPtr)pThis->mhSocket);
		if(err != noErr)
		{
			/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket
						<< "] TCPRelease ASync FAILED with code ["
						<< err << "]");
			*/
			err = PBControlSync((ParmBlkPtr)pThis->mhSocket);
			if(err != noErr)
			{/*
				WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket
						<< "] TCPRelease SYNC FAILED with code ["
						<< err << "] MEMORY WILL LEAK");
			*/}
			else
			{/*
				WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket
						<< "] TCPRelease freeing addr ["
						<< (size_t)((SOCKET)pThis->mhSocket)->csParam.create.rcvBuff << "]");
				DisposePtr(((SOCKET)pThis->mhSocket)->csParam.create.rcvBuff);
				((SOCKET)pThis->mhSocket)->csCode = 0;
			*/}
		}
		else
		{
			/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket
						<< "] TCPRelease freeing addr ["
						<< (size_t)((SOCKET)pThis->mhSocket)->csParam.create.rcvBuff << "]");
			*/
			Socket_IOSync((SOCKET)pThis->mhSocket);
			DisposePtr(((SOCKET)pThis->mhSocket)->csParam.create.rcvBuff);
			((SOCKET)pThis->mhSocket)->csCode = 0;
		}
	}
	/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket
					<< "] Destroy() is DONE");*/
}
wsbool WSSocket_IsCreated(const WSSOCKET* pThis)
{ return pThis->mhSocket && ((SOCKET)pThis->mhSocket)->csCode != 0; }
void* WSSocket_Disown(WSSOCKET* pThis)
{ WSSOCKETTYPE rets = pThis->mhSocket; pThis->mhSocket = NULL; return rets; }
wsbool WSSocket_Accept(WSSOCKET* pThis, const WSSOCKET* listen,
	WSSOCKETLIBRARY* socklib)
{
	SOCKET pSocket;
	wsbool res;
	/**/
	/* Unlike other socket APIs, MacTCP transitions listeners */
	/* to clients. That means the listening socket will become */
	/* the new client, however we need to keep listen a listener */
	/* socket. So, we make this socket the new server socket */
	/* then swap the two. */
	/**/
	WSASSERT(pThis->mhSocket);
	/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
	/*		  << "] Accept()");*/
	pSocket = (SOCKET) listen->mhSocket;
	if (!Socket_IsListener(pSocket))
	{
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
		/*	  << "] Accept() called on a non-listening socket");*/
		return false;
	}
	if (!Socket_IsIOComplete(pSocket))
	{
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
		/*	  << "] Accept() called on a listening socket "*/
		/*		 "that is not ready to accept");*/
		return false;
	}
	/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
	/*		<< "] listener sync successful. Transitioning listener");*/
	res = WSSocket_CreateServer(pThis,
		((SOCKET)listen->mhSocket)->csParam.open.localPort,
		socklib);
	if(res)
	{
		/**/
		/* Everything checks out. Swap the listener socket */
		/* with this socket so this socket becomes the new client */
		/* and the listener socket becomes a new listener/server socket. */
		/**/
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
		/*		<< "] listener transitioned to client, this socket"*/
		/*			" is now a listener");*/
		SOCKET newserver = (SOCKET)pThis->mhSocket;
		pThis->mhSocket = (SOCKET)listen->mhSocket;
		(*(SOCKET*)listen->mhSocket) = newserver;
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
		/*		<< "] Accept() successful and now a listener");*/
		return true;
	}
	else
	{
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
		/*		<< "] listener transition but this socket's listen"*/
		/*			" FAILED");*/
	}
	return false;
}
wsbool WSSocket_CreateServer(WSSOCKET* pThis,
	size_t port, WSSOCKETLIBRARY* socklib)
{
	wsbool res = Socket_CreateTCPServer((SOCKET)pThis->mhSocket,
		WSSocketLibrary_GetMacTCPRefNum(socklib),
			port, WSSocketLibrary_GetMacTCPStreamSize(socklib),
			NewTCPNotifyUPP(WSMacTCPNotifyProc),
			(Ptr)pThis->mhSocket); /*kDefTCPCreateRcvBufferSize*/
	if(!res)
	{
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket << "] "*/
		/*			"Socket_CreateServerTCP failed in CreateServer()");*/
		return false;
	}

	return true;
}
wsbool WSSocket_CreateClient(WSSOCKET* pThis,
	const char* szHostname, size_t port, WSSOCKETLIBRARY* socklib)
{
	ResultUPP s2acb;
	struct hostInfo hostinfo;
	OSErr err;
	volatile int done;
	SOCKET pSocket = (SOCKET) pThis->mhSocket;
	MacTCPDNR dnr;

	/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
	/*		<< "] CreateClient()");*/
	if(!Socket_CreateTCP((SOCKET)pThis->mhSocket,
					WSSocketLibrary_GetMacTCPRefNum(socklib),
					WSSocketLibrary_GetMacTCPStreamSize(socklib),
					NewTCPNotifyUPP(WSMacTCPNotifyProc), (Ptr)pThis->mhSocket))
	{
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
		/*	<< "] CreateClient() - DNR isn't alive or Socket_Create failed");*/
		return false;
	}

	MacTCPDNR_Construct(&dnr);
	if(!MacTCPDNR_IsCreated(&dnr))
		return false;

	done = FALSE;
	s2acb = NewResultUPP(&DNRCB);
	err = MacTCPDNR_StrToAddr(&dnr,
		(char*)szHostname, &hostinfo, s2acb, (char*)&done);
	if (err == cacheFault)
		while (!done)
			continue;
	if (hostinfo.rtnCode != noErr)
	{
		MacTCPDNR_Destruct(&dnr);
		return FALSE;
	}

	pSocket->csCode = TCPActiveOpen;
	pSocket->csParam.open.validityFlags = timeoutValue | timeoutAction;
	pSocket->csParam.open.ulpTimeoutValue = 60 /* seconds */;
	pSocket->csParam.open.ulpTimeoutAction = 1 /* 1:abort 0:report */;
	pSocket->csParam.open.commandTimeoutValue = 0;
	pSocket->csParam.open.remoteHost = hostinfo.addr[0];
	pSocket->csParam.open.remotePort = port;
	pSocket->csParam.open.localHost = 0;
	pSocket->csParam.open.localPort = 0/*??? needs valid ????*/;
	pSocket->csParam.open.dontFrag = 0;
	pSocket->csParam.open.timeToLive = 0;
	pSocket->csParam.open.security = 0;
	pSocket->csParam.open.optionCnt = 0;
	PBControlAsync((ParmBlkPtr)pSocket);
	Socket_IOSync(pSocket);
	MacTCPDNR_Destruct(&dnr);
	return pSocket->ioResult == noErr;
}
#if defined(HAVE_MUTSTR)
wsbool WSSocket_GetHostname_(const WSSOCKET* pThis, WSMUTSTR* s,
	wsbool getlocal)
{
/*	csParam.open.localHost; csParam.open.localPort; csParam.open.remotePort;*/
	SOCKET pSocket;
	ResultUPP s2acb;
	struct hostInfo hostinfo;
	OSErr err;
	volatile int done;
	wsbool ret;
	MacTCPDNR dnr;
	MacTCPDNR_Construct(&dnr);
	if(!MacTCPDNR_IsCreated(&dnr))
		return false;

	pSocket = (SOCKET) pThis->mhSocket;
	WSASSERT(pSocket->csCode != TCPPassiveOpen
			 && pSocket->csCode != TCPCreate);
	done = FALSE;
	s2acb = NewResultUPP(&DNRCB);
	err = MacTCPDNR_AddrToName(&dnr,
		getlocal ?
			pSocket->csParam.open.localHost
			: pSocket->csParam.open.remoteHost, &hostinfo,
					s2acb, (char*)&done);
	if (err == cacheFault)
		while (!done)
			continue;
	if (hostinfo.rtnCode != noErr)
		ret = Socket_GetIPAddress(pSocket, &dnr, s, getlocal);
	else
	{
		WSMutStr_Resize(s, CStrLength(hostinfo.cname));
		CStrCopy((char*)WSMutStr_GetCString(s), hostinfo.cname);
		ret = true;
	}

	MacTCPDNR_Destruct(&dnr);
	return ret;
}
wsbool WSSocket_GetIPAddress_(const WSSOCKET* pThis, WSMUTSTR* s,
	wsbool getlocal)
{
	SOCKET pSocket;
	MacTCPDNR dnr;
	MacTCPDNR_Construct(&dnr);
	if(!MacTCPDNR_IsCreated(&dnr))
		return false;
	pSocket = (SOCKET) pThis->mhSocket;
	WSASSERT(pSocket->csCode != TCPPassiveOpen
			 && pSocket->csCode != TCPCreate);
	MacTCPDNR_Destruct(&dnr);
	return Socket_GetIPAddress(pSocket, &dnr, s, getlocal);
}

wsbool WSSocket_GetIPAddress(const WSSOCKET* pThis, WSMUTSTR* s)
{
	return WSSocket_GetIPAddress_(pThis, s, FALSE);
}
wsbool WSSocket_GetLocalIPAddress(const WSSOCKET* pThis, WSMUTSTR* s)
{
	return WSSocket_GetIPAddress_(pThis, s, TRUE);
}
wsbool WSSocket_GetHostname(const WSSOCKET* pThis, WSMUTSTR* s)
{
	return WSSocket_GetHostname_(pThis, s, FALSE);
}
wsbool WSSocket_GetLocalHostname(const WSSOCKET* pThis, WSMUTSTR* s)
{
	return WSSocket_GetHostname_(pThis, s, TRUE);
}
#endif /* HAVE_MUTSTR */
size_t WSSocket_Read(const WSSOCKET* pThis, void* data, size_t num, wsbool* inblock)
{
	OSErr err;
	SOCKET pSocket = (SOCKET)pThis->mhSocket;
	/*WSLogDebug(MutStr("Socket: [") << (size_t)pSocket*/
	/*		   << "] READING [" << num << "] bytes");*/
	WSASSERT(pSocket->csCode != TCPPassiveOpen
			 && pSocket->csCode != TCPCreate);
	if((pSocket->csCode == TCPRcv
	   && pSocket->csParam.receive.userDataPtr == (Ptr)inProgress)
	   == FALSE)
	{
		/*(0 timeout == infinite wait)*/
		pSocket->csCode = TCPRcv;
		pSocket->csParam.receive.commandTimeoutValue = 0;
		pSocket->csParam.receive.rcvBuff = (char*)data;
		pSocket->csParam.receive.rcvBuffLen = num;
		if(inblock && *inblock == false)
			err = PBControlAsync((ParmBlkPtr)pSocket);
		else
			err = PBControlSync((ParmBlkPtr)pSocket);
	}
	else /* previous read in progress or finished, fallthrough */
	{
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pSocket*/
		/*	<< "] READ found previous pending read, falling through");*/
		err = noErr;
	}

	if (err == noErr)
	{
		if(!inblock || *inblock == true || pSocket->ioResult == noErr)
		{
			/*WSLogDebug(MutStr("Socket: [") << (size_t)pSocket*/
			/*   << "] done with READ of [" << num << "] bytes"); */
			pSocket->csParam.receive.userDataPtr = (Ptr)0;
			return pSocket->csParam.receive.rcvBuffLen;
		}
		else if (pSocket->ioResult == inProgress)
		{
			/*WSLogDebug(MutStr("Socket: [") << (size_t)pSocket*/
			/*   << "] READ in progress [" << num << "] bytes"); */
			if(inblock)
				*inblock = true;
			pSocket->csParam.receive.userDataPtr = (Ptr)inProgress;
			return SOCKET_ERROR;
		}
	}
	/*else*/
	/*	WSLogError(MutStr("WSSocket_Read Error:[") << err << "]");*/
	pSocket->csParam.receive.userDataPtr = (Ptr)0;
	return SOCKET_ERROR;
}
wsbool WSSocket_WaitForRead(const WSSOCKET* pThis, size_t microsecs, wsbool* block)
{
	SOCKET pSocket = (SOCKET)pThis->mhSocket;
	OSErr err;
	size_t startTime;
	size_t seconds;
	TCPiopb pbStatus;
	/* WSTimeGetLocalValue is costly on earlier macs, so */
	/* there's a tiny bit of extra code to avoid calling it */
	/* unless absolutely necessary */
	WSASSERT(pSocket->csCode != TCPPassiveOpen
			 && pSocket->csCode != TCPCreate);
	err = Socket_GetTCPStatus(&pbStatus, pSocket->tcpStream);
	if(err == noErr
		&& pSocket->csParam.status.amtUnreadData)
		return true;
	if(err == connectionDoesntExist)
	{
		/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
		/*	<< "] WaitForRead() called on nonexistant connection!");*/
		return false;
	}
	if(microsecs == 0)
	{
		if(block)
			*block = true;
		return false;
	}
	/* OK, we have to call it */
	startTime = WSTimeGetLocalValue();
	seconds = microsecs / 1000;
	WSThreadYield();
	while(WSTimeGetLocalValue() - startTime < seconds)
	{
		err = Socket_GetTCPStatus(&pbStatus, pSocket->tcpStream);
		if(err == noErr
		   && pSocket->csParam.status.amtUnreadData) /* TCPS_CLOSE_WAIT == readable too */
			return true;
		if(err == connectionDoesntExist)
		{
			/*WSLogDebug(MutStr("Socket: [") << (size_t)pThis->mhSocket*/
			/*	<< "] WaitForRead() socket disconnected during wait!");*/
			return false;
		}
		WSThreadYield();
	}

	if(block)
		*block = true;
	return false;
}
wsbool WSSocket_WaitForWrite(const WSSOCKET* pThis, size_t microsecs, wsbool* block)
{
	return true;
}
wsbool WSSocket_WaitForReadWrite(const WSSOCKET* pThis, size_t microsecs, wsbool* block)
{
	return WSSocket_WaitForRead(pThis, microsecs, block);
}

#if 1
#define SENDBLOCKSIZEVALUE STREAM_BUFFER_SIZE
#define SENDBLOCKSIZE(x) (x > SENDBLOCKSIZEVALUE ? SENDBLOCKSIZEVALUE : x)
#else
#define SENDBLOCKSIZE(x) (x)
#endif
size_t WSSocket_Write(const WSSOCKET* pThis, const void* dbufin, size_t dlenin, wsbool* inblock)
{
	const char* dbuf = (const char*)dbufin;
	size_t dlen = dlenin;
	size_t olen = dlen;
	size_t slen;
	for (; ;)
	{
		slen = Socket_Send((SOCKET)pThis->mhSocket,
						   dbuf, SENDBLOCKSIZE(dlen), inblock);
		if (slen >= 0)
		{
			if (slen == dlen)
				return olen;
			dbuf += slen;
			dlen -= slen;
		}
		else
			break;
	}
	return SOCKET_ERROR;
}
int WSSocket_Error(const WSSOCKET* pThis)
{ return 0; }
wsbool WSSocket_CreateEndpoint(WSSOCKET* pThis,
	const char* szHostname, WSSOCKETLIBRARY* socklib)
{
	UDPiopb* pSocket = (UDPiopb*)pThis->mhSocket;
	pSocket->ioCRefNum = WSSocketLibrary_GetMacTCPRefNum(socklib);
	pSocket->csCode = UDPCreate;
	pSocket->csParam.create.rcvBuff = NewPtr(STREAM_BUFFER_SIZE);
	pSocket->csParam.create.rcvBuffLen = STREAM_BUFFER_SIZE;
	pSocket->csParam.create.notifyProc = NULL;
	pSocket->csParam.create.userDataPtr = NULL;
	pSocket->csParam.create.localPort = 0;
	if (PBControlSync((ParmBlkPtr)pSocket) != noErr)
	{
		pSocket->csCode = 0;
		return false;
	}
	return true;
}
size_t WSSocket_ReadEndpoint(const WSSOCKET* pThis,
	void* data, size_t num, wsbool* block)
{
	OSErr err;
	UDPiopb* pSocket = (UDPiopb*)pThis->mhSocket;
	pSocket->csCode								= UDPRead;
	pSocket->ioCompletion							= NULL;
	pSocket->csParam.receive.rcvBuff				= (char*)data;
	pSocket->csParam.receive.rcvBuffLen			= num;
	pSocket->csParam.receive.secondTimeStamp		= 0;
	if (!block || *block == TRUE)
		return PBControlSync((ParmBlkPtr)pSocket) == noErr;
	err = PBControlAsync((ParmBlkPtr)pSocket);
	if(err == noErr)
	{
		if(pSocket->ioResult == noErr)
			return (pSocket->csParam.receive.rcvBuffLen);
		if(block)
			*block = pSocket->ioResult == inProgress;
	}
	else if (*block)
		*block = FALSE;
	/*WSLogError(MutStr("WSSocket_ReadEndPoint Error:[") << pSocket->ioResult << "]");*/
	return (size_t) -1;
}

size_t WSSocket_WriteEndpoint(const WSSOCKET* pThis,
	const void* data, size_t num, const char* dest, wsbool* block)
{
	wsbool ret;
	UDPiopb* pSocket;
	ResultUPP s2acb;
	struct hostInfo hostinfo;
	OSErr err;
	SingleWDSEntry theSendWDS;
	volatile int done;
	MacTCPDNR dnr;
	MacTCPDNR_Construct(&dnr);
	if(!MacTCPDNR_IsCreated(&dnr))
		return false;

	done = FALSE;
	s2acb = NewResultUPP(&DNRCB);
	err = MacTCPDNR_StrToAddr(&dnr,
		(char*)dest, &hostinfo, s2acb, (char*)&done);
	if (err == cacheFault)
		while (!done)
			continue;
	if (hostinfo.rtnCode != noErr)
	{
		MacTCPDNR_Destruct(&dnr);
		return FALSE;
	}

	/*WSLogDebug(MutStr("theSendWDS size:[") << sizeof(theSendWDS) << "]");*/
	theSendWDS.length = num;
	theSendWDS.ptr = (Ptr)data;
	theSendWDS.end = 0;
	pSocket = (UDPiopb*)pThis->mhSocket;
	pSocket->csCode							= UDPWrite;
	pSocket->csParam.send.remoteHost			= hostinfo.addr[0];
	pSocket->csParam.send.remotePort			= 0; /* FIXME maybe? */
	pSocket->csParam.send.wdsPtr				= (Ptr)&theSendWDS;
	pSocket->csParam.send.checkSum			= TRUE;
	pSocket->csParam.send.sendLength			= 0;
	if (!block || *block == TRUE)
		ret = PBControlSync((ParmBlkPtr)pSocket) == noErr;
	else
	{
		err = PBControlAsync((ParmBlkPtr)pSocket);
		if(err == noErr)
		{
			if(pSocket->ioResult == noErr)
			{
				/*WSLogDebug(MutStr("``SENT [") << theSendWDS.length << "] bytes``");*/
				MacTCPDNR_Destruct(&dnr);
				return theSendWDS.length;
			}
			if(block)
				*block = pSocket->ioResult == inProgress;
		}
		else if (*block)
			*block = FALSE;
		/*WSLogError(MutStr("WSSocket_ReadEndPoint Error:[")*/
		/* << pSocket->ioResult << "]"); */
		ret = (size_t) -1;
	}

	MacTCPDNR_Destruct(&dnr);
	return ret;
}

/******************************************************************************
								WSSocketGetDNSServer

	What follows is quite a bit of code involving sniffing into MacTCP
	binary preferences for the DNS server used the OS and chosen by the user.
	Originally from NCBI's Entrez source then reworked entirely for compat.
******************************************************************************/
typedef unsigned char uchar;
extern Boolean ResLoad;
#if TARGET_CPU_PPC
#pragma options align=mac68k
#endif
/*
 * Holds IP and LAP-specific configuration information.  This is held in the
 * 'ipln' configuration resource in either the MacTCP driver or the "MacTCP
 * Prep" files.
 */
typedef struct IPConfig {
	long            version;
	long            flags;
	long            dfl_ip_addr;
	long            dfl_net_mask;
	long            dfl_broadcast_mask;
	long            dfl_gateway_addr;
	b_8             server_lap_address[8];
	long            configIPAddr;
	long            configNetMask;
	long            dfl_dyn_low;
	long            dfl_dyn_high;
	char            dfl_zone[34];           /* ### */
	Boolean         load;
	Boolean         admin;
	Boolean         netLock;
	Boolean         subnetLock;
	Boolean         nodeLock;
	Boolean         filler1;
	long            activeLap;
	long            slot;
	char            filename[33];           /* ### */
} IPConfig;

#if 0
/*
 * LAPInfo is allocated during system start-up and holds all of the
LAP-specific information.
 */
typedef struct LAPInfo
{
	b_32            our_ip_addr;		/* LAP's IP address = */
	b_32            our_net_mask;		/* LAP's IP net-mask= */
	b_32            ip_broadcast_addr;	/* IP's broadcast address */
	IPConfig		lc;					/* copy of IP LAP configuration resource */
	OSErrProcPtr    lapInit;			/* pointer to once-only LAP init routine */
	OSErrProcPtr    lapOpen;			/* LAP open routine = */
	OSErrProcPtr    lapClose;			/* LAP close routine=  */
	voidProcPtr     lapUnload;          /* LAP unload routine, undoes LapInit */
	OSErrProcPtr    lapAttach;			/* LAP attach PH routine */
	OSErrProcPtr    lapDetach;			/* LAP detach= routine */
	OSErrProcPtr    lapOutput;			/* LAP output= routine */
	OSErrProcPtr    lapControl;         /* LAP control routine */
	voidProcPtr     lapFault;			/* LAP fault isolation routine */
	OSErrProcPtr    lapStatistics;      /* LAP statistic reading routine */
	voidProcPtr     lapConfigure;       /* LAP-specific configuration routines */
	BooleanProcPtr  lapProbe;			/* send a LAP-specific address probe packet */
	BooleanProcPtr  lapRegister;        /* register our IP address on the network */
	voidProcPtr     lapFindGateway;     /* LAP-specific means of finding a gateway */
	BooleanProcPtr  lapGwyCheck;        /* LAP-specific means of verifying gateway up */
	/* IP parameters */
	ip_addr         dfl_dns_addr;       /* address of DNS from config protocol */
	Handle          dnslHndl;			/* handle to DNS configuration resource */
	Ptr             dnsCache;			/* pointer to space allocated for dns cache */
	long            dnsCacheSize;       /* size of cache allocated, in bytes */
	/* LAP parameters */
	long            headerSize;			/* LAP header space requirements */
	long            trailerSize;		/* LAP trailer space requirements */
	long            outMaxPacketSize;	/* size of maximum output packet */
	long            inMaxPacketSize;	/* size of maximum input packet */
	long            maxDataSize;		/* size of maximum data packet */
	long			numConnections;		/* number of separate network connections */
	unsigned long   versionFlags;		/* version number flags */
	voidProcPtr     ip_ph;			    /* pointer to IP's protocol handler */
	Ptr             ipGlobals;			/* pointer to IP's A5 */
	short           link_unit;			/* unit number of link driver */
	Boolean         addressConflict;	/* TRUE if address conflict discovered */
	long            lapType;			/* IP LAP hardware type number */
	long            lapAddrLength;		/* size of LAP address field */
	unsigned char   *lapAddrPtr;		/* pointer to LAP address field */
	unsigned long   reserved;			/* MacTCP reserved field */
} LAPInfo;
#endif
#if TARGET_CPU_PPC
#pragma options align=reset
#endif

Boolean UsingOpenTransport(void)
{
	OSErr  err;
	long   gval;
	err = Gestalt ('otan'/*gestaltOpenTpt*/, &gval);
	if (err == noErr && gval != 0)
			return TRUE;
	return FALSE;
}

/*
* Search a folder for a file of a specific type and creator (specify 0L for
* any) that contains a resource of a given type.
* Returns a FSpec to that file.
*/
static OSErr SearchFolderForRsrc(FSSpec *target, short vRefNum, long dirID,
								 OSType ftype, OSType fcrea, OSType rsrcType)
{
	FSSpec              fspec;
	HParamBlockRec      fi;
	Handle              rsrcH;
	short               refNum;
	OSErr               err;
	Boolean             oldResLoad;

	err = fnfErr;
#ifdef NEW_UNIVERSAL_HEADERS /* JAE */
	oldResLoad = LMGetResLoad();
#else
	oldResLoad = /* ResLoad */ true;
#endif /* NEW_UNIVERSAL_HEADERS */
	SetResLoad(false);

	fspec.vRefNum = vRefNum;
	fspec.parID = dirID;

	fi.fileParam.ioCompletion = NULL;
	fi.fileParam.ioNamePtr = fspec.name;
	fi.fileParam.ioVRefNum = vRefNum;
	fi.fileParam.ioDirID = dirID;
	fi.fileParam.ioFDirIndex = 1;

	while(PBHGetFInfoSync(&fi) == noErr)
	{
		/* scan folder for resource files of specific type & creator */
		if((fcrea == 0L || fi.fileParam.ioFlFndrInfo.fdCreator == fcrea)
			&& (ftype == 0L || fi.fileParam.ioFlFndrInfo.fdType == ftype))
		{
			/* found the conforming file, try to open it */
			if((refNum=FSpOpenResFile(&fspec,fsRdPerm)) != -1)
			{
				/* see if a resource is there */
				rsrcH = GetIndResource(rsrcType, 1);
				CloseResFile(refNum);

				/* if there, return noErr */
				if(rsrcH != NULL)
				{
					err = noErr;
					*target = fspec;
					break;
				}
			}
		}
		/* check mNext file in folder */
		fi.fileParam.ioFDirIndex++;
		fi.fileParam.ioDirID = dirID;  /* PBHGetFInfo() clobbers ioDirID */
	}
	SetResLoad(oldResLoad);
	return err;
}
#if defined(HAVE_MUTSTR)
/*
* Takes a 'ipln' resource and tries to figure out which LAP driver we are
* using.
*/
wsbool ParseLAPType(Handle configH, WSMUTSTR* laptype)
{
	const IPConfig *ipc;
	uchar   hs;

	hs = HGetState(configH);
	HLock(configH);
	ipc = (IPConfig*)*configH;

	/* Check if MacTCP is using LocalTalk or Ethernet */
	if( ipc->activeLap == 128 || ipc->activeLap == 129 )
	{
		if (ipc->activeLap == 128)
			WSMutStr_AssignCStringWithLength(laptype,
				"LocalTalk", strcountof("LocalTalk"));
		else
			WSMutStr_AssignCStringWithLength(laptype,
				"Ethernet", strcountof("Ethernet"));
	}
	else
	{
		WSMutStr_BeforeWrite(laptype, ipc->filename[0] + 1);
		if(!WSMutStr_IsCreated(laptype))
			return FALSE;
		WSMemoryCopy((char*)WSMutStr_GetCString(laptype),
					 &ipc->filename[1], ipc->filename[0]);
		WSMutStr_AfterWriteAndNullTerminate(laptype, ipc->filename[0]);
	}
	HSetState(configH, hs);
	return TRUE;
}

/*
* Tries to get the default DNS server from the "dnsl" resource
* The "dnsl" resource is in either the MacTCP Prep file, or
* in the MacTCP control panel file itself
*/
wsbool ParseDNSL(Handle resource, MacTCPDNR* dnr, WSMUTSTR* dns)
{
	uint16_t numEntries;
	uchar hs;
	char* raw;
	wsbool filled = false;

	hs = HGetState(resource);
	HLock(resource);
	raw = *resource;

	/*
		It has Entry * numEntries data. "title" is zero terminated.
		------------dnsl format:------------

		unsigned short numEntries;
		struct Entry
		{
			uint32_t ip;
			char isDefault;
			char* title;
		};
		char endingZero;
	*/
	numEntries = *((uint16_t*)raw);
	raw += sizeof(uint16_t);
	for(;numEntries != 0; --numEntries)
	{
		uint32_t ip = *((uint32_t*)raw);
		raw += sizeof(ip);
		if(*raw)
		{
			WSMutStr_BeforeWrite(dns, 16);
			if(MacTCPDNR_AddrToStr(dnr, ip,
								  (char*)WSMutStr_GetCString(dns)) != noErr)
				return false;
			WSMutStr_AfterWrite(dns, CStrLength(WSMutStr_GetCString(dns)));
			filled = true;
			break;
		}
		++raw;
		raw += CStrLength(raw) + 1; /* skip past dns title */
	}
	HSetState(resource, hs);
	return filled;
}
wsbool MacTCPGetPreference(short prefcode, WSMUTSTR* dns)
{
	wsbool ret;
	long responce;
	FSSpec fspec;
	short refNum = -1;
	char** dnslH;
	int filled = 0;
	MacTCPDNR dnr;
	MacTCPDNR_Construct(&dnr);
	if(!MacTCPDNR_IsCreated(&dnr))
		return false;

	if(Gestalt('mtcp', &responce) != noErr)
	{
		if(prefcode == 'ipln')
		{
			WSMutStr_AssignCStringWithLength(dns,
				"OpenTransport", strcountof("OpenTransport"));
			ret = true;
		}
		else
			ret = false;
		MacTCPDNR_Destruct(&dnr);
		return ret;
	}

	/*
		First check the MacTCP Prep file
		The MacTCP Prep file contains 2 resources
		ipln, which has a IPConfig struct,
		and dnsl, which contains the dns server list
	*/
	if(FindFolder(kOnSystemDisk, kPreferencesFolderType, false,
		&fspec.vRefNum, &fspec.parID) == noErr)
	{
		if(SearchFolderForRsrc( &fspec, fspec.vRefNum, fspec.parID,
			'mtpp', 'mtcp', prefcode) == noErr)
		{
			refNum = FSpOpenResFile(&fspec, fsRdPerm);
			if(refNum != -1)
			{
				dnslH = Get1IndResource(prefcode, 1);
				if(dnslH != NULL && *dnslH != NULL)
				{
					if(prefcode == 'ipln')
						filled = ParseLAPType(dnslH, dns);
					else
						filled = ParseDNSL(dnslH, &dnr, dns);
				}
				CloseResFile(refNum);

				if(filled != NULL)
				{
					MacTCPDNR_Destruct(&dnr);
					return filled;
				}
			}
		}
	}


	/*
		Otherwise check the MacTCP driver/control panel
	*/
	if(FindFolder(kOnSystemDisk, 'cntl', false,
				  &fspec.vRefNum, &fspec.parID ) == noErr)
	{
		if(SearchFolderForRsrc(&fspec, fspec.vRefNum, fspec.parID,
								'cdev', 'ztcp', prefcode) == noErr)
		{
			refNum = FSpOpenResFile(&fspec, fsRdPerm);
			if(refNum != -1)
			{
				dnslH = Get1IndResource(prefcode, 1);
				if(dnslH != NULL && *dnslH != NULL)
				{
					if(prefcode == 'ipln')
						filled = ParseLAPType(dnslH, dns);
					else
						filled = ParseDNSL(dnslH, &dnr, dns);
				}
				CloseResFile(refNum);
			}
		}
	}

	/*
		Clean up
	*/
	if(!filled && prefcode == 'ipln' && UsingOpenTransport())
	{
		WSMutStr_AssignCStringWithLength(dns,
			"OpenTransport", strcountof("OpenTransport"));
		ret = true;
	}
	else
		ret = filled;

	MacTCPDNR_Destruct(&dnr);
	return ret;
}
wsbool WSSocketGetDNSServer(WSMUTSTR* dns)
{
	return MacTCPGetPreference('dnsl', dns);
}
#endif /* HAVE_MUTSTR */
wsbool WSSocket_ClearReadBuffer(WSSOCKET* pThis, wsbool* inblock)
{
	char* buffer = WSALLOC(char, 1024);
	size_t numRead = (size_t)-1;
	wsbool block = inblock ? *inblock : WSSOCKET_NONBLOCKING;
	if(WSSocket_WaitForRead(pThis, 0, inblock))
	{
		do
		{ numRead = WSSocket_Read(pThis, buffer, 1024, &block); }
		while (numRead != (size_t)-1 && numRead != 0);
		if(inblock) *inblock = block;
	}
	WSFREE(buffer);
	return numRead != (size_t)-1;
}


#endif /* CARBON */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


						 SocketNotifier


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*#############################################################################
						Notification Callbacks
	The key thing here is that these are called at interrupt time. At
	interrupt time some system calls are prohibited and will crash the app.
	Therefore we just add it to a list to handled at another time.
#############################################################################*/
#ifdef WSMACUSEOPENTRANSPORT

#define PRINTTIME(x)
#if defined(WSMACCARBON)
#define WSOTINTERRUPTDEBUG(x)
#else
#define WSOTINTERRUPTDEBUG(x)
#endif
wspascal void WSServerOTNotify(
	void* contextPtr, OTEventCode code, OTResult result, void* cookie)
{
	OSErr err;
	WSSOCKETNOTIFIERINFO* entry = (WSSOCKETNOTIFIERINFO*)contextPtr;
	if (code == T_LISTEN)
	{ /* Client can be accepted */
		entry->mCanAccept = TRUE;
		++entry->mNumRequests;
		/* We have an IO request we can handle, add it to the queue */
		if (entry->mNextForEvent == entry) /* Make sure it isn't already queued */
		{
			DList_Add(&entry->mNotifier->mSockets,
					entry, mNextForEvent, mPrevForEvent);
		}
		WSOTINTERRUPTDEBUG(MutStr("[OT]: [")
			<< (size_t)WSSocket_GetHandle(&entry->mSocket)
			<< "] CLIENT Got");
	}
	else
		WSOTINTERRUPTDEBUG(MutStr("[OT]: [")
			<< (size_t)WSSocket_GetHandle(&entry->mSocket)
			<< "] UNKNOWN SERVER [" << code << "] request");
}

wspascal void WSClientOTNotify(
	void* contextPtr, OTEventCode code, OTResult result, void* cookie)
{
	OSErr err;
	WSSOCKETNOTIFIERINFO* entry = (WSSOCKETNOTIFIERINFO*)contextPtr;

	/* Unlike the server, the client callback is quite involved. */
	/* We can't due much here due to thing being ran at */
	/* interrupt time */
	switch (code)
	{
		/* T_DATA/T_EXDATA: When data can be read; */
		/* Can be only be cleared when by kOTNoDataErr from OTRcv[UData] */
		/* OR during OTRcv when going to return kOTNoDataErr */
		/* T_PASSCON: means Accept() is completed and data can be read, */
		/* and it is possible to get T_DATA before T_PASSCON out of order */
		case T_DATA:
		case T_EXDATA:
		case T_PASSCON:
		{
			entry->mCanRead = TRUE;
			WSOTINTERRUPTDEBUG(MutStr("[OT]: [")
				<< (size_t)WSSocket_GetHandle(&entry->mSocket)
				<< "] READ request");
			break;
		}
		/* T_GODATA/T_GOEXDATA: happens when OTSnd() returns a kOTFlowErr */
		/* or sends less bytes than the input amount; can happen otherwise */
		/* when data can be written as well */
		/* T_CONNECT: OTConnect accepted by peer, data can now be written */
		case T_CONNECT:
		case T_GODATA:
		case T_GOEXDATA:
		{
			entry->mCanWrite = TRUE;
			WSOTINTERRUPTDEBUG(MutStr("[OT]: [")
				<< (size_t)WSSocket_GetHandle(&entry->mSocket)
				<< "] WRITE request");
			break;
		}
		/* T_DISCONNECT: Peer disconnected, usually forcefully. */
		/* OTRcvDisconnect clears */
		/* T_DISCONNECTCOMPLETE: OTSndDisconnect completed */
		case T_DISCONNECT:
		case T_DISCONNECTCOMPLETE:
		{
			entry->mIsDisconnected = TRUE;
			WSOTINTERRUPTDEBUG(MutStr("[OT]: [")
				<< (size_t)WSSocket_GetHandle(&entry->mSocket)
				<< "] LOST request");
			break;
		}
		case T_ORDREL:
		{
			entry->mIsDisconnected = TRUE;
			entry->mNeedsOrderlyDisconnect = TRUE;
			WSOTINTERRUPTDEBUG(MutStr("[OT]: [")
				<< (size_t)WSSocket_GetHandle(&entry->mSocket)
				<< "] ORDERLYDISCONNECT request");
			break;
		}
		/* Unknown response, don't handle it - note return not break here */
		default:
		{
			WSOTINTERRUPTDEBUG(MutStr("[OT]: [")
				<< (size_t)WSSocket_GetHandle(&entry->mSocket)
				<< "] UNKNOWN [" << code << "] request");
			return;
		}
	}

	/* We have an IO request we can handle, add it to the queue */
	WSSocketAddEntryToIOReq(entry);
}
#else /* MacTCP */
#if 1
#define PRINTTIME(x)
#else
#define PRINTTIME(x) {\
			WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&x->mSockets->mSocket) \
					<< "] {" << __FILE__ << "--" << __LINE__ << "}#########PRINTING######");\
			WSSOCKETNOTIFIERINFO* qe = x->mSockets;\
			size_t pos = 0;\
			do\
			{\
				WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&x->mSockets->mSocket) \
					<< "] POS[" << pos << "] == [" << (size_t)qe->mPrevForEvent \
					<< "]<---[" << (size_t)qe \
					<< "]--->[" << (size_t)qe->mNextForEvent << "]");\
				++pos;\
				qe = qe->mNextForEvent;\
				if(pos == 5)\
					break;\
			}\
			while(qe && qe != x->mSockets); \
			pos = 0;WSLogDebug("-----------------------");\
			qe = x->mSockets;\
			do\
			{\
				WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&x->mSockets->mSocket) \
					<< "] POS[" << pos << "] == PREVIO[" << (size_t)qe->mPrevForEvent \
					<< "]<---PREV[" << (size_t)qe->mPrev \
					<< "]<---[" << (size_t)qe \
					<< "]--->NEXT[" << (size_t)qe->mNext << "]--->NEXTIO[" << (size_t)qe->mNextForEvent << "]");\
				++pos;\
				qe = qe->mNext;\
				if(pos == 5)\
					break;\
			}\
			while(qe && qe != x->mSockets); \
			WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&x->mSockets->mSocket) \
					<< "] #####DONE PRINTING#####"); }
#endif
/*
	TCPiopb->ioCompletion is set once the SocketNotifier has the object.
	So at that point we know the TCPiopb is contained within a
	WSSOCKETNOTIFIERINFO. Thus, we can adjust the address of the TCPiopb/socket
	handle back to the containing WSSOCKETNOTIFIERINFO.
*/
void WSMacTCPCompletionProc(TCPiopb* pbPtr)
{
	WSSOCKETNOTIFIERINFO* entry;
	if(!pbPtr->ioCompletion)
		return;
	entry = (WSSOCKETNOTIFIERINFO*)
		(pbPtr - offsetof(WSSOCKETNOTIFIERINFO, mSocket));
}
wspascal void WSMacTCPNotifyProc(StreamPtr tcpStream,
	unsigned short eventCode, Ptr userDataPtr,
	unsigned short terminReason, ICMPReport* icmpMsg)
{
	WSSOCKETNOTIFIERINFO* entry;
	TCPiopb* pbPtr = (TCPiopb*)userDataPtr;
	if(!pbPtr->ioCompletion)
		return;
	entry = (WSSOCKETNOTIFIERINFO*)
		(pbPtr - offsetof(WSSOCKETNOTIFIERINFO, mSocket));
	switch(eventCode)
	{
	case TCPClosing:
		/*WSLogDebug(MutStr("ServerCallbacks: [")*/
		/*		<< (size_t)WSSocket_GetHandle(&entry->mSocket)*/
		/*		<< "] TCPClosing");*/
		break;
	case TCPULPTimeout:
		/*WSLogDebug(MutStr("ServerCallbacks: [")*/
		/*		<< (size_t)WSSocket_GetHandle(&entry->mSocket)*/
		/*		<< "] TCPULPTimeout");*/
		break;
	case TCPTerminate:
		/*WSLogDebug(MutStr("ServerCallbacks: [")*/
		/*		<< (size_t)WSSocket_GetHandle(&entry->mSocket)*/
		/*		<< "] TCPTerminate");*/
		entry->mIsDisconnected = TRUE;
		if (((SOCKET)WSSocket_GetHandle(&entry->mSocket))->csCode != TCPRelease
			&& ((SOCKET)WSSocket_GetHandle(&entry->mSocket))->csCode != TCPAbort)
			WSSocketAddEntryToIOReq(entry);
		break;
	case TCPDataArrival:
		/*WSLogDebug(MutStr("ServerCallbacks: [")*/
		/*		<< (size_t)WSSocket_GetHandle(&entry->mSocket)*/
		/*		<< "] TCPDataArrival; sending to HandleRead()");*/
		/* We have an IO request we can handle, add it to the queue */
		if(entry->mIsListener)
		{
			entry->mCanAccept = TRUE;
			++entry->mNumRequests;
		}
		else
			entry->mCanRead = TRUE;
		WSSocketAddEntryToIOReq(entry);
		break;
	case TCPUrgent:
		/*WSLogDebug(MutStr("ServerCallbacks: [")*/
		/*		<< (size_t)WSSocket_GetHandle(&entry->mSocket)*/
		/*		<< "] TCPUrgent");*/
		break;
	case TCPICMPReceived:
		/*WSLogDebug(MutStr("ServerCallbacks: [")*/
		/*		<< (size_t)WSSocket_GetHandle(&entry->mSocket)*/
		/*		<< "] TCPICMPReceived");*/
		break;
	default:
		/*WSLogDebug(MutStr("ServerCallbacks: [")*/
		/*		<< (size_t)WSSocket_GetHandle(&entry->mSocket)*/
		/*		<< "] UNKNOWN event code of [" << eventCode << "]");*/
		break;
	}
}
#endif /* WSMACUSEOPENTRANSPORT */

/*=============================================================================

						 SocketNotifier

=============================================================================*/

/*===========================================================================*/
/*=============================Utilities=====================================*/
/*===========================================================================*/

static void AssureEntryIsGone(WSSOCKETNOTIFIERINFO* mSockets,
	WSSOCKETNOTIFIERINFO* entry)
{
	WSSOCKETNOTIFIERINFO* seeking = mSockets;
	if (seeking)
	{
		while (seeking->mNext != mSockets)
		{
			if (seeking == entry)
				break;
			seeking = seeking->mNext;
		}
	}
	WSASSERT(seeking != entry);
}

void WSSocketNotifierPost_Destroy(WSSOCKETNOTIFIERPOST* post)
{
	if(post->mShouldFreeBuffer)
		WSFREE(post->mBuffer);
	WSFREE(post);
}
void WSSocketNotifierInfo_Destroy(WSSOCKETNOTIFIERINFO* entry)
{
	if(entry->mShouldDestroySocket)
		WSSocketSSL_Destruct(&entry->mSocket);
	WSFREE(entry);
}

/*===========================================================================*/
/*==============================Info List Handling===========================*/
/*===========================================================================*/

void WSSocketNotifierRemoveEntry(
	WSSOCKETNOTIFIERINFO** top, WSSOCKETNOTIFIERINFO* entry)
{
	/*
	WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&entry->mSocket)
			<< "] REMOVE BASE BEFORE:[" << (size_t)entry->mPrevForEvent
			<< "] CUR:[" << (size_t)entry
			<< "] NEXT:[" << (size_t)entry->mNextForEvent << "]");
	WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&entry->mSocket)
			<< "] REMOVE BASE Server BEFORE:[" << (size_t)mSockets->mPrevForEvent
			<< "] CUR:[" << (size_t)mSockets
			<< "] NEXT:[" << (size_t)mSockets->mNextForEvent << "]");
	*/
	PRINTTIME(this);
	DList_Remove(top, entry, mNext, mPrev);
	/*WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&entry->mSocket)
					<< "] REMOVE IOREQ BEFORE:[" << (size_t)entry->mPrevForEvent
					<< "] CUR:[" << (size_t)entry
					<< "] NEXT:[" << (size_t)entry->mNextForEvent << "]");
	*/
	PRINTTIME(this);
	DList_RemoveWithoutRoot(top, entry, mNextForEvent, mPrevForEvent);
	PRINTTIME(this);
	if((entry->mPrevForEvent->mNextForEvent == entry))
	{ WSASSERT((entry->mPrevForEvent == entry) && entry->mNextForEvent == entry); }
/*	if(entry->mPrevForEvent->mNextForEvent == entry)*/
/*		entry->mPrevForEvent = entry->mNextForEvent = entry;*/
/*	entry->mPrevForEvent->mNextForEvent = entry->mNextForEvent;*/
/*	entry->mNextForEvent->mPrevForEvent = entry->mPrevForEvent;*/
	/*WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&entry->mSocket)
			<< "] REMOVE MID:[" << (size_t)entry->mPrevForEvent
			<< "] CUR:[" << (size_t)entry
			<< "] NEXT:[" << (size_t)entry->mNextForEvent << "]");
	PRINTTIME(this);
	WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&entry->mSocket)
			<< "] REMOVE CORRECTED:[" << (size_t)entry->mPrevForEvent
			<< "] CUR:[" << (size_t)entry
			<< "] NEXT:[" << (size_t)entry->mNextForEvent << "]");*/
	/*PRINTTIME(this);*/
	/*entry->mNextForEvent = entry->mPrevForEvent = entry;*/
	PRINTTIME(this);
}
void WSSocketNotifierUpdateEntry(
	WSSOCKETNOTIFIERINFO** top, WSSOCKETNOTIFIERINFO* entry)
{
	entry->mLastActiveTime = WSTimeGetLocalValue();
	/*WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&entry->mSocket) << "] UPDATE");*/
	PRINTTIME(this);
/*			WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&entry->mSocket)*/
/*					<< "] UPDATE BEFORE:[" << (size_t)entry->mPrevForEvent*/
/*					<< "] CUR:[" << (size_t)entry*/
/*					<< "] NEXT:[" << (size_t)entry->mNextForEvent << "]");*/
	DList_Update(top, entry, mNext, mPrev);
/*			WSLogDebug(MutStr("Server: [") << (size_t)WSSocket_GetHandle(&entry->mSocket)*/
/*					<< "] UPDATE AFTER:[" << (size_t)entry->mPrevForEvent*/
/*					<< "] CUR:[" << (size_t)entry*/
/*					<< "] NEXT:[" << (size_t)entry->mNextForEvent << "]");*/
}

void WSSocketNotifier_DeleteEntry(
	 WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO** top, WSSOCKETNOTIFIERINFO* entry,
	wsbool force)
{
	AssureEntryIsGone(pNotifier->mSockets, entry);

	while(entry->mPending)
	{
		WSSOCKETNOTIFIERPOST* nextdata = entry->mPending->mNext;
		WSSocketNotifierPost_Destroy(entry->mPending);
		entry->mPending = nextdata;
	}
	/* delete it finally */
	PRINTTIME(this);
	WSSocketNotifierInfo_Destroy(entry);
/*	WSLogDebug(MutStr("----------------~~~~~~~~~~~~") << "[" << (size_t)mSockets << "]C[" << (size_t)entry << "]");*/
	PRINTTIME(this);
}

void WSSocketNotifier_AddEntry( WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO** top, WSSOCKETNOTIFIERINFO* entry)
{
	AssureEntryIsGone(*top, entry);
	entry->mLastActiveTime = WSTimeGetLocalValue();
	entry->mNotifier = pNotifier;
	DList_Add(top, entry, mNext, mPrev);
	PRINTTIME(this);
}

/*===========================================================================*/
/*=============================Main Implementation===========================*/
/*===========================================================================*/

void WSSocketNotifier_Construct(WSSOCKETNOTIFIER* pThis)
{
	pThis->mSocketLibRef = NULL;
}
void WSSocketNotifier_Destruct(WSSOCKETNOTIFIER* pThis)
{
	if(WSSocketNotifier_IsCreated(pThis))
	{
		WSSocketNotifier_Destroy(pThis);
		pThis->mSocketLibRef = NULL;
	}
}
wsbool WSSocketNotifier_Create(WSSOCKETNOTIFIER* pNotifier, WSSOCKETLIBRARY* socklib)
{
	pNotifier->mSockets = NULL;
	pNotifier->mSocketLibRef = socklib;
	return TRUE;
}

void WSSocketNotifierDestroyEntries(
	 WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO** pInfo, wsbool clientsonly)
{
	if (*pInfo)
	{
		WSSOCKETNOTIFIERINFO* oldentry;
		WSSOCKETNOTIFIERINFO* nextentry = *pInfo;
		do
		{
			oldentry = nextentry;
			nextentry = oldentry->mNext;
			if (!clientsonly || !oldentry->mIsListener)
			{
				WSSocketNotifierRemoveEntry(pInfo, oldentry);
				WSSocketNotifier_DeleteEntry(pNotifier, pInfo, oldentry, TRUE);
			}
		} while (nextentry != oldentry);
		*pInfo = NULL;
	}
}

void WSSocketNotifier_Destroy(
	WSSOCKETNOTIFIER* pNotifier)
{
	WSSocketNotifierDestroyEntries(pNotifier, &pNotifier->mSockets, FALSE);
}

wsbool WSSocketNotifier_IsCreated(WSSOCKETNOTIFIER* pThis)
{ return pThis->mSocketLibRef != NULL; }

void WSSocketNotifier_AddInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* entry)
{
	WSSocketNotifier_AddEntry(pNotifier, &pNotifier->mSockets, entry);
}

wsbool WSSocketNotifier_TrackInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* entry)
{
#ifndef WSMACUSEOPENTRANSPORT
	TCPiopb* tcppb;
#else
	OTNotifyProcPtr notifyProc;
	OSErr err;
#endif
	entry->mPending = NULL;
	entry->mNextForEvent = entry->mPrevForEvent = entry;
#ifdef WSMACUSEOPENTRANSPORT
	if(entry->mIsListener)
		notifyProc = WSServerOTNotify;
	else
	{
		/* Data can arrive during the accept process,
			when we didn't have a callback installed.
			So, do a quick check first. */
		if(WSSocket_WaitForRead(&entry->mSocket, 0, NULL))
		{
			entry->mCanRead = TRUE;
			WSSocketAddEntryToIOReq(entry);
		}
		notifyProc = WSClientOTNotify;
	}
	err = OTInstallNotifier((SOCKET)WSSocket_GetHandle(&entry->mSocket),
		NewOTNotifyUPP(notifyProc), (void*)entry);
	if (err != noErr)
		return FALSE;
#else
	/* Due to listeners transitioning to clients the latter
		should have the callback installed already. */
	tcppb = (TCPiopb*)WSSocket_GetHandle(&entry->mSocket);
	tcppb->ioCompletion = NewTCPIOCompletionUPP(WSMacTCPCompletionProc);
#endif
	return TRUE;
}
void WSSocketNotifier_UpdateInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* pInfo)
{
	WSSocketNotifierRemoveEntry(&pNotifier->mSockets, pInfo);
	WSSocketNotifierUpdateEntry(&pNotifier->mSockets, pInfo);
}
void WSSocketNotifier_RemoveInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* pInfo)
{
	WSSocketNotifierRemoveEntry(&pNotifier->mSockets, pInfo);
}
void WSSocketNotifier_DestroyInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* pInfo)
{
	WSSocketNotifier_DeleteEntry(
		pNotifier, &pNotifier->mSockets, pInfo, FALSE);
}


void WSSocketNotifier_InitInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* entry,
	wsbool mIsListener, wsbool mIsListenerClient)
{
	entry->mIsListener = mIsListener;
	entry->mPending = NULL;
	entry->mIsListenerClient = mIsListenerClient;
	entry->mCanAccept = entry->mCanRead = entry->mCanWrite =
		entry->mIsDisconnected = FALSE;
	entry->mShouldDestroySocket = TRUE;
	entry->mNumRequests = 0;
}
void WSSocketNotifier_InitListenerClientInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* info)
{
	WSSocketNotifier_InitInfo(pNotifier, info, FALSE, TRUE);
}
void WSSocketNotifier_InitClientInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* info)
{
	WSSocketNotifier_InitInfo(pNotifier, info, FALSE, FALSE);
}
void WSSocketNotifier_InitListenerInfo(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKETNOTIFIERINFO* info)
{
	WSSocketNotifier_InitInfo(pNotifier, info, TRUE, FALSE);
}

WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddSocket(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKET* sock,
	wsbool mIsListener, wsbool mIsListenerClient)
{
	WSSOCKETNOTIFIERINFO* info = WSALLOC1(WSSOCKETNOTIFIERINFO);
	if(!info)
		return NULL;
	WSSocket_Construct(&info->mSocket);
	WSSocket_Own(&info->mSocket, sock);
	WSSocketNotifier_InitInfo(
			pNotifier, info, mIsListener, mIsListenerClient);
	if(!info)
		return NULL;
	if(!WSSocketNotifier_TrackInfo(pNotifier, info))
	{
		WSSocketNotifierInfo_Destroy(info);
		return NULL;
	}
	WSSocketNotifier_AddInfo(pNotifier, info);
	return info;
}

WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddListenerClientSocket(

	WSSOCKETNOTIFIER* pNotifier, WSSOCKET* sock)
{
	return WSSocketNotifier_AddSocket(
		pNotifier, sock, FALSE, TRUE);
}
WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddClientSocket(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKET* sock)
{
	return WSSocketNotifier_AddSocket(
		pNotifier, sock, FALSE, FALSE);
}
WSSOCKETNOTIFIERINFO* WSSocketNotifier_AddListenerSocket(
	WSSOCKETNOTIFIER* pNotifier, WSSOCKET* sock)
{
	return WSSocketNotifier_AddSocket(
		pNotifier, sock, TRUE, FALSE);
}


void WSSocketNotifier_Begin(WSSOCKETNOTIFIER* pNotifier)
{
	pNotifier->mLastPost = NULL;
	if(pNotifier->mSockets
		&& (pNotifier->mSockets->mNextForEvent->mCanRead
			|| pNotifier->mSockets->mNextForEvent->mCanAccept
			|| pNotifier->mSockets->mNextForEvent->mCanWrite
			|| pNotifier->mSockets->mNextForEvent->mIsDisconnected))
		pNotifier->mNextInfoForEvent = pNotifier->mSockets->mNextForEvent;
	else
		pNotifier->mNextInfoForEvent = NULL;
}

wsbool WSSocketNotifier_Next(
	WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO** ppInfo, WSSOCKETNOTIFIERPOST** ppPost)
{
	WSSOCKETNOTIFIERPOST* post;
	WSSOCKETNOTIFIERINFO* cur;
	if(pNotifier->mLastPost)
		WSSocketNotifierPost_Destroy(pNotifier->mLastPost);
	if(!pNotifier->mNextInfoForEvent)
		return FALSE; /* done */


	cur = *ppInfo = pNotifier->mNextInfoForEvent;

#if defined(WSMACUSEOPENTRANSPORT)
	if(cur->mNeedsOrderlyDisconnect)
		Socket_RcvOrderlyDisconnect((SOCKET)WSSocket_GetHandle(&cur->mSocket));
	else if(cur->mIsDisconnected)
		Socket_RcvDisconnect((SOCKET)WSSocket_GetHandle(&cur->mSocket));
#endif
	if(cur->mIsListener && cur->mNumRequests)
	{
		--cur->mNumRequests;
		cur->mCanAccept = TRUE;
	}
	if(!(cur->mIsListener && cur->mNumRequests))
	{
		if((cur->mNextForEvent->mCanRead
				|| cur->mNextForEvent->mCanAccept
				|| cur->mNextForEvent->mCanWrite
				|| cur->mNextForEvent->mIsDisconnected))
			pNotifier->mNextInfoForEvent = cur->mNextForEvent;
		else
			pNotifier->mNextInfoForEvent = NULL;
	}

	post = (*ppInfo)->mPending;
	pNotifier->mLastPost = *ppPost = NULL;
	while (post)
	{
		if ((post->mIsRead && (*ppInfo)->mCanRead)
			|| (post->mIsWrite && (*ppInfo)->mCanWrite))
		{
			*ppPost = pNotifier->mLastPost = post;
			DList_Remove(&(*ppInfo)->mPending,
				pNotifier->mLastPost, mNext, mPrev);
			break;
		}
	}
	return TRUE;
}

WSSOCKETNOTIFIERPOST* WSSocketNotifierAllocPost(
	WSSOCKETNOTIFIERINFO* pInfo, void* pvData,
	size_t len, size_t pos, wsbool shouldFreeData, char isWrite)
{
	WSSOCKETNOTIFIERPOST* data = WSALLOC1(WSSOCKETNOTIFIERPOST);
	if (!data)
		return FALSE;

	data->mBuffer = pvData;
	data->mNext = pInfo->mPending;
	data->mPos = pos;
	data->mSize = len;
	data->mShouldFreeBuffer = shouldFreeData;
	data->mIsWrite = isWrite;
	return data;
}
void WSSocketNotifierAddPost(
	WSSOCKETNOTIFIERINFO* pInfo, WSSOCKETNOTIFIERPOST* pPost)
{
	DList_Add(&pInfo->mPending, pPost, mNext, mPrev);
}
wsbool WSSocketNotifierPost_(WSSOCKETNOTIFIERINFO* pInfo, void* pvData,
	size_t len, size_t pos, wsbool shouldFreeData, char isWrite)
{
	/*	AssureEntryIsGone(mAcceptExClients, &entry); */
	wsbool res;
	WSSOCKETNOTIFIERPOST* data;
	data = WSSocketNotifierAllocPost(pInfo,
		pvData, len, pos, shouldFreeData, isWrite);
	if(!data)
		return FALSE;
	WSSocketNotifierAddPost(pInfo, data);
	return TRUE;
}
wsbool WSSocketNotifierPostRead(WSSOCKETNOTIFIERINFO* pInfo,
	void* pvData, size_t len, size_t pos, wsbool shouldFreeData)
{
	return WSSocketNotifierPost_(pInfo, pvData, len, pos,
								shouldFreeData, FALSE);
}
wsbool WSSocketNotifierPostWrite(WSSOCKETNOTIFIERINFO* pInfo,
	const void* pvData, size_t len, size_t pos, wsbool shouldFreeData)
{
	return WSSocketNotifierPost_(pInfo, (void*)pvData, len, pos,
								shouldFreeData, TRUE);
}
#if defined(__cplusplus)
} /* end extern "C" */
#endif
#endif /* defined(macintosh) */
/******************************************************************************
			  #####  ####### ######  #     # ####### ######
			 #     # #       #     # #     # #       #     #
			 #       #       #     # #     # #       #     #
			  #####  #####   ######  #     # #####   ######
				   # #       #   #    #   #  #       #   #
			 #     # #       #    #    # #   #       #    #
			  #####  ####### #     #    #    ####### #     #
******************************************************************************/
/**
*	@file server.c
*
*	Server implementation.
*
*	Written in ANSI C (C89) for compiler compatability.
*/
/*=============================================================================
								Headers
=============================================================================*/
#if 0
	#include "system.h"
	#ifndef PRECOMPILEDHEADERS
		#include "server.h"
	#endif /* NOPRECOMPILEDHEADERS*/
#endif
#if defined(__cplusplus)
extern "C" {
#endif

/*===========================================================================*/
/*===========================================================================*/
typedef struct WSSERVERWORKERDATA
{
	WSSOCKETNOTIFIER mPoller;
	#ifdef WSTHREADEDSERVEROPT
		WSCRITICALSECTION mLock;
		WSTHREAD mThread;
		WSRWLOCK* mConfigLockRef;
	#endif
	wsbool mIsDone;
	wsbool mShouldShutdown;
	SERVER* mServer;
} WSSERVERWORKERDATA;


typedef struct WSSERVERACCEPTEXINFO
{
	WSSOCKETNOTIFIERINFO mSuper;
	WSSOCKETNOTIFIERPOST* mAcceptExPost;
	WSSOCKETNOTIFIERINFO* mAcceptExListener;
} WSSERVERACCEPTEXINFO;
/*============================================================================

						Server

	Synchronous = socket()->bind()->listen()->accept() = client socket for r/w.
	Asynchronous = Above but more involved. WinSock 2/AcceptEx actually has
				   a seperate socket for accepting which does the dirty work.
=============================================================================*/
#ifdef WSTHREADEDSERVEROPT
	static WSTHREADENTRYPROCRET WSTHREADENTRYPROCCT
		ServerWorkerThreadEntry(void*);
#endif
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4355) /* this in constructor initializer */
#endif
void Server_Construct(SERVER* pThis,
	SERVERCLIENTCONNECTEDPROC conProc, SERVERCLIENTDISCONNECTEDPROC dcProc,
	SERVERCLIENTIOCOMPLETEPROC ioProc, size_t numClients)
{
	pThis->mOnClientConnect = (conProc);
	pThis->mOnClientDisconnect = (dcProc);
	pThis->mOnClientIOComplete = (ioProc);
	pThis->mWillShutdown = (FALSE); pThis->mWillRestart = (FALSE);
#ifdef WSTHREADEDSERVEROPT
	pThis->mWorkers = (NULL);
#endif
	pThis->mMaxClients = (numClients);

#if !defined(WSTHREADEDSERVEROPT)
	WSSocketNotifier_Construct(&pThis->mPoller);
#endif
}
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
wsbool Server_AddAcceptExSockets(SERVER* pThis, WSSOCKETNOTIFIER* pNotifier,
	WSSOCKETNOTIFIERINFO* listenerinfo, size_t amount)
{
#if defined(_WIN32) && defined(WSACCEPTEXOPT)
	if(pThis->mSocketLib.mAcceptEx)
	{
		size_t iClient;
		for(iClient = 0; iClient < amount; ++iClient)
		{
			WSSERVERACCEPTEXINFO* clientrealinfo;
			WSSOCKETNOTIFIERINFO* clientinfo;
			WSSOCKETNOTIFIERPOST* post;
			uint32_t firstReadLen = 0;
			wsbool res;
			clientrealinfo = WSALLOC1(WSSERVERACCEPTEXINFO);
			clientinfo = &clientrealinfo->mSuper;

			res = WSSocket_CreateTCPv4(&clientinfo->mSocket);
			WSSocketNotifier_InitListenerClientInfo(
				pNotifier, clientinfo);
			WSSocketNotifier_AddInfo(
				pNotifier, clientinfo);
			post = WSSocketNotifierAllocPost(clientinfo,
				WSALLOC(char, 64), 64, 0, TRUE, FALSE);
			#if defined(WSIOCPOPT)
				if(!pNotifier->mhIOCP)
			#endif
					WSSocketNotifier_TrackInfo(pNotifier, clientinfo);
			clientrealinfo->mAcceptExPost = post;
			clientrealinfo->mAcceptExListener = listenerinfo;
			WSSocketNotifierAddPost(listenerinfo, post);
			res = WSSocket_AcceptEx(&clientinfo->mSocket,
				&listenerinfo->mSocket,
				post->mBuffer, &firstReadLen, 32,
				(OVERLAPPED*)&post->mOverlapped, &pThis->mSocketLib);
			res = TRUE;
			if(res){}
		}
	}
#endif
	return TRUE;
}
wsbool Server_Create(SERVER* pThis, size_t port)
{
	WSSOCKETNOTIFIERINFO* info;
#ifdef WSTHREADEDSERVEROPT
	wsbool succeeded;
	size_t iWorker;
	if(pThis->mWorkers)
		Server_Destroy(pThis);
#endif

	info = WSALLOC1(WSSOCKETNOTIFIERINFO);
	if(!info)
	{
		return FALSE;
	}

	if(!WSSocketLibrary_Create(&pThis->mSocketLib))
	{
		WSFREE(info);
		return FALSE;
	}

	/* Listen socket setup: socket()->bind()->listen(). accept()/Hard part comes next.*/
	WSSocketSSL_Construct(&info->mSocket);
#if defined(WSSSLOPT)
#if 0 && !defined(WSNTVSSLOPT)
	if (!WSSocketSSL_CreateServer(&info->mSocket, port,
		"localhost.crt", "localhost.key", &pThis->mSocketLib))
#else
	if (!WSSocketSSL_CreateServer(&info->mSocket, port,
		"local.cer", "local.pvk", &pThis->mSocketLib, "CARoot.cer", NULL))
#endif
#else
	if (!WSSocketSSL_CreateServer(&info->mSocket, port, &pThis->mSocketLib))
#endif
	{
		WSFREE(info);
		Server_Destroy(pThis);
		return FALSE;
	}



#ifdef WSTHREADEDSERVEROPT
	pThis->mNumWorkers = WSThreadPoolGetRecommendedThreadCount();
	pThis->mWorkers = WSALLOC(WSSERVERWORKERDATA, pThis->mNumWorkers);
	pThis->mCurrentWorker = 1;
	if(!pThis->mWorkers)
	{
		WSFREE(info);
		WSSocketLibrary_Destroy(&pThis->mSocketLib);
		return FALSE;
	}
	WSRWLock_Construct(&pThis->mConfigLock);
	succeeded = TRUE;
	for(iWorker = 0; iWorker < pThis->mNumWorkers; ++iWorker)
	{
		pThis->mWorkers[iWorker].mIsDone = FALSE;
		pThis->mWorkers[iWorker].mShouldShutdown = FALSE;
		pThis->mWorkers[iWorker].mServer = pThis;
		if(!WSSocketNotifier_Create(&pThis->mWorkers[iWorker].mPoller,
				&pThis->mSocketLib))
		{
			succeeded = FALSE;
			break;
		}
		if(iWorker == 0)
		{
			WSSocketNotifier_InitListenerInfo(
				&pThis->mWorkers[iWorker].mPoller, info);
			WSSocketNotifier_AddInfo(&pThis->mWorkers[iWorker].mPoller, info);
			if(!WSSocketNotifier_TrackInfo(&pThis->mWorkers[iWorker].mPoller, info))
			{
				succeeded = FALSE;
				break;
			}
		}
#if defined(_WIN32)
		else
		{
			/* Flush events for WSTextCtrl SendMessage */
			WSEVENTLOOP el;
			WSEventLoop_Create(&el);
			while(WSEventLoop_RunSingle(&el)) {}
			WSEventLoop_Destroy(&el);
		}
#endif
		if(!Server_AddAcceptExSockets(pThis, &pThis->mWorkers[iWorker].mPoller,
				info, pThis->mMaxClients / pThis->mNumWorkers))
		{
			succeeded = FALSE;
			break;
		}

		WSCriticalSection_Construct(&pThis->mWorkers[iWorker].mLock);
		pThis->mWorkers[iWorker].mConfigLockRef = &pThis->mConfigLock;
		if(!WSThread_Create(&pThis->mWorkers[iWorker].mThread,
							ServerWorkerThreadEntry, &pThis->mWorkers[iWorker]))
		{
			WSCriticalSection_Destruct(&pThis->mWorkers[iWorker].mLock);
			succeeded = FALSE;
			break;
		}
	}
	if(!succeeded)
	{
		size_t iWorkerBT;
		if(!iWorker)
			WSFREE(info);
		for(iWorkerBT = 0; iWorkerBT < iWorker; ++iWorkerBT)
		{
			WSThread_Destroy(&pThis->mWorkers[iWorkerBT].mThread);
			WSCriticalSection_Destruct(&pThis->mWorkers[iWorkerBT].mLock);
			WSSocketNotifier_Destroy(&pThis->mWorkers[iWorkerBT].mPoller);
		}
		pThis->mWorkers = NULL;
		return FALSE;
	}
	WSCriticalSection_Construct(&pThis->mDistributionLock);
#else
	if(!WSSocketNotifier_Create(&pThis->mPoller, &pThis->mSocketLib))
	{
		WSFREE(info);
		Server_Destroy(pThis);
		return FALSE;
	}

	WSSocketNotifier_InitListenerInfo(&pThis->mPoller, info);
	WSSocketNotifier_AddInfo(&pThis->mPoller, info);
	if(!WSSocketNotifier_TrackInfo(&pThis->mPoller, info))
	{
		Server_Destroy(pThis);
		return FALSE;
	}

	if(!Server_AddAcceptExSockets(pThis, &pThis->mPoller,
		info, pThis->mMaxClients))
	{
		Server_Destroy(pThis);
		return FALSE;
	}
#endif
	pThis->mWillShutdown = pThis->mWillRestart = FALSE;
	return TRUE;
}

void Server_Destroy(SERVER* pThis)
{
#ifdef WSTHREADEDSERVEROPT
	if(pThis->mWorkers)
	{
		size_t iWorker;
		for(iWorker = 0; iWorker < pThis->mNumWorkers; ++iWorker)
		{
			WSThread_Cancel(&pThis->mWorkers[iWorker].mThread);
			WSThread_Destroy(&pThis->mWorkers[iWorker].mThread);
			WSCriticalSection_Destruct(&pThis->mWorkers[iWorker].mLock);
			WSSocketNotifier_Destroy(&pThis->mWorkers[iWorker].mPoller);
		}
		WSFREE(pThis->mWorkers);
		pThis->mWorkers = NULL;
		WSCriticalSection_Destruct(&pThis->mDistributionLock);
		WSRWLock_Destruct(&pThis->mConfigLock);
	}
#else
	WSSocketNotifier_Destruct(&pThis->mPoller);
#endif
	WSSocketLibrary_Destroy(&pThis->mSocketLib);
}

void Server_ProcessNotifier(SERVER* pThis, WSSOCKETNOTIFIER* pNotifier)
{
	WSSOCKETNOTIFIERINFO* info; WSSOCKETNOTIFIERPOST* post;
	WSSOCKETNOTIFIERINFO* entry;
	WSSocketNotifier_Begin(pNotifier);
	while(WSSocketNotifier_Next(pNotifier,
		&info, &post))
	{
		if(info->mIsListener)
		{
			WSSOCKETNOTIFIERINFO* clientinfo;
#if defined(_WIN32) && defined(WSACCEPTEXOPT)
			clientinfo = NULL;

			if(post)
			{
				WSSOCKADDR *localaddr, *remoteaddr;
				/*WSMUTSTR local, remote;*/
				WSSERVERACCEPTEXINFO* realclientinfo;
				clientinfo = (WSSOCKETNOTIFIERINFO*)
					OVERLAPPED_DATA(&(post)->mOverlapped);
				realclientinfo =
					(WSSERVERACCEPTEXINFO*) clientinfo;
				/*WSMutStr_Create(&local);*/
				/*WSMutStr_Create(&remote);*/
				WSSocketFinishAcceptEx(
					&clientinfo->mSocket, &info->mSocket, post->mBuffer,
					0, 32, &localaddr, &remoteaddr, &pThis->mSocketLib);
				/*WSSocketAddrToMutStr(localaddr, &local);*/
				/*WSSocketAddrToMutStr(remoteaddr, &remote);*/
				/*WSMutStr_Destroy(&local);*/
				/*WSMutStr_Destroy(&remote);*/
/*				while((*pThis->mOnClientIOComplete)
						(pThis, clientinfo,*post))*/
				realclientinfo->mAcceptExPost = NULL;
				while ((*pThis->mOnClientConnect)(pThis, clientinfo))
				{
					if(clientinfo->mPending != NULL)
						break;
					post = clientinfo->mPending;
				}
				WSSocketNotifier_TrackInfo(
					pNotifier, clientinfo);
				WSSocketNotifier_UpdateInfo(pNotifier, clientinfo);
			}
			else
#endif
			{
				WSSOCKETSSL client;
				if(WSSocketSSL_Accept(&client, &info->mSocket, &pThis->mSocketLib))
				{
					clientinfo = Server_DistributeSocket(pThis, &client);
					if(clientinfo)
					{
						#if defined(_WIN32) && defined(WSIOCPOPT)
						if(pNotifier->mhIOCP)
							while ((*pThis->mOnClientConnect)(pThis, clientinfo))
							{
								if(clientinfo->mPending != NULL)
									break;
								post = clientinfo->mPending;
							}
						#endif
					}
				}
			}
		}
		else
		{
			wsbool keepconnected = FALSE;
			if(info->mCanRead)
			{
				#if defined(_WIN32) && defined(WSACCEPTEXOPT)
					if(pThis->mSocketLib.mAcceptEx
						#if defined(WSIOCPOPT)
							&& !pNotifier->mhIOCP
						#endif
							&& ((WSSERVERACCEPTEXINFO*)info)->mAcceptExPost
						)
					{
						WSSOCKADDR *localaddr, *remoteaddr;
						/*WSMUTSTR local, remote;*/
						WSSERVERACCEPTEXINFO* realinfo =
							((WSSERVERACCEPTEXINFO*)info);
						post = realinfo->mAcceptExPost;
						/*WSMutStr_Create(&local);*/
						/*WSMutStr_Create(&remote);*/
						/* SO_UPDATE_ACCEPT_CONTEXT (Winsock 2.0,
							AcceptEx is 1.1) changes the state
							of the socket so we need to reselect it */
						WSSocketNotifier_UntrackInfo(pNotifier,
							info);
						WSSocketFinishAcceptEx(&info->mSocket,
							&realinfo->mAcceptExListener->mSocket,
							post->mBuffer, 0, 32,
							&localaddr, &remoteaddr, &pThis->mSocketLib);
						WSSocketNotifier_TrackInfo(pNotifier,
							info);
						/*WSSocketAddrToMutStr(localaddr, &local);*/
						/*WSSocketAddrToMutStr(remoteaddr, &remote);*/
						/*WSMutStr_Destroy(&local);*/
						/*WSMutStr_Destroy(&remote);*/
						DList_Remove(&realinfo->mAcceptExListener->mPending,
							post, mNext, mPrev);
						WSFREE(realinfo->mAcceptExPost->mBuffer);
						WSFREE(realinfo->mAcceptExPost);
						realinfo->mAcceptExPost = NULL;
					}
				#endif
				if(post)
					keepconnected = (*pThis->mOnClientIOComplete)(pThis,
						info, post);
				else
					keepconnected = (*pThis->mOnClientConnect)(pThis, info);
			}
			else if(info->mIsDisconnected)
				(*pThis->mOnClientDisconnect)(pThis, info);
			if(!keepconnected)
			{
				#if defined(_WIN32) && defined(WSACCEPTEXOPT)
				WSSOCKETNOTIFIERINFO* listenerinfo;
				if(pThis->mSocketLib.mAcceptEx
						&& info->mIsListenerClient
						)
				{
					listenerinfo = ((WSSERVERACCEPTEXINFO*)info)->mAcceptExListener;
				}
				else
					listenerinfo = NULL;
				#endif
				WSSocket_ClearReadBuffer(&info->mSocket, NULL);
				WSSocketNotifier_RemoveInfo(pNotifier, info);
				WSSocketNotifier_DestroyInfo(pNotifier, info);
				#if defined(_WIN32) && defined(WSACCEPTEXOPT)
					if(listenerinfo)
					{
						Server_AddAcceptExSockets(pThis,
							pNotifier, listenerinfo, 1);
					}
				#endif
			}
			else
			{
				WSSocketNotifier_UpdateInfo(pNotifier, info);
			}
		}
	}
	/*

		Take care of timeouts

	*/
	entry = WSSocketNotifier_Root(pNotifier);
	if(entry)
	{
		size_t ctimestamp = WSTimeGetLocalValue();
		WSSOCKETNOTIFIERINFO* root = entry;
		do
		{
			WSSOCKETNOTIFIERINFO* toDel = entry;
			#if defined(_WIN32) && defined(WSACCEPTEXOPT)
				WSSOCKETNOTIFIERINFO* listenerinfo;
			#endif
			entry = entry->mNext;
			#if defined(_WIN32) && defined(WSACCEPTEXOPT)
				if(pThis->mSocketLib.mAcceptEx
					&& toDel->mIsListenerClient)
				{
					if(((WSSERVERACCEPTEXINFO*)toDel)->mAcceptExPost)
						continue;
					listenerinfo =
						((WSSERVERACCEPTEXINFO*)toDel)->mAcceptExListener;
				}
				else
					listenerinfo = NULL;
			#endif
			/*
				Check if the socket is past the timeout.
				Sockets are ordered by time so if this isn't past the timeout
				then the rest are not as well. entry->time > ctimestamp
				can happen due to linger adjustment
			*/
			if(entry->mLastActiveTime > ctimestamp
				|| ctimestamp - entry->mLastActiveTime
					< Server_GetTimeout(pThis)
			)
				break;
			if(toDel->mIsListener)
				continue;
			/*WSLogDebug(MutStr("Socket [")
						<< (size_t)WSSocketSSL_GetHandle(&toDel->mSocket)
						<<  "] --TIMEOUT--");*/
			WSSocketNotifier_RemoveInfo(pNotifier, toDel);
			(*pThis->mOnClientDisconnect)(pThis, toDel);
			WSSocketNotifier_DestroyInfo(pNotifier, toDel);
			#if defined(_WIN32) && defined(WSACCEPTEXOPT)
				if(listenerinfo)
				{
					Server_AddAcceptExSockets(pThis, pNotifier, listenerinfo, 1);
				}
			#endif
		} while(entry != root);
	}
}
wsbool Server_Process(SERVER* pThis)
{
#if !defined(WSTHREADEDSERVEROPT)
	Server_ProcessNotifier(pThis, &pThis->mPoller);
#endif
	return Server_WillShutdown(pThis) == FALSE;
}

void Server_OOM(SERVER* pThis)
{
	/*WSLog(MKSTR("Out of memory, purging pending requests"));*/
	/* Purge all pending sockets*/
#if 0 && defined(WSTHREADEDSERVEROPT)
	for(size_t iWorker = 0; iWorker < pThis->mNumWorkers; ++iWorker)
	{
		pThis->mWorkers[iWorker].mPoller.PurgeAllClients();
	}
#else
	/*....*/
	(void)(pThis);
#endif
}

/*
	Timeout handling
*/
size_t Server_GetTimeout(const SERVER* pThis)
{
	(void)(pThis);
	return 15;/*300;*/
} /* 300 == default apache timeout*/


/*
	WSSOCKETNOTIFIERINFO/WSSOCKETNOTIFIERPOST handling
*/
WSSOCKETSSL* ServerGetQueueEntrySocket(WSSOCKETNOTIFIERINFO* qe)
{ return &qe->mSocket; }
wsbool ServerIsIODataWrite(WSSOCKETNOTIFIERPOST* pIOData)
{ return pIOData->mIsWrite == TRUE; }
void ServerGetIODataBuffer(WSSOCKETNOTIFIERPOST* pIOData,
	void** buffer, size_t* pos, size_t* size)
{
	*buffer = pIOData->mBuffer;
	*pos = pIOData->mPos;
	*size = pIOData->mSize;
}
void ServerSetIODataAutoFree(
	WSSOCKETNOTIFIERPOST* pIOData, wsbool autofree)
{ pIOData->mShouldFreeBuffer = autofree; }

WSSOCKETNOTIFIERINFO* Server_DistributeSocket(SERVER* pThis, WSSOCKET* socket)
{
#ifdef WSTHREADEDSERVEROPT
	WSSOCKETNOTIFIERINFO* clientinfo;

	WSCriticalSection_Lock(&pThis->mDistributionLock);
	clientinfo = WSSocketNotifier_AddListenerClientSocket(
		&pThis->mWorkers[pThis->mCurrentWorker].mPoller, socket);
	if(clientinfo)
	{
		/*WSLogDebug(MutStr("~~~~Distribution: Thread [") << WSThreadGetCurrentID()
					<< "] DISTRIBUTED to Worker #[" << pThis->mCurrentWorker
			<< "]~~~~~");*/
		pThis->mCurrentWorker =
			(pThis->mCurrentWorker + 1) & (pThis->mNumWorkers - 1);
	}
	WSCriticalSection_Unlock(&pThis->mDistributionLock);
	return clientinfo;
#else
	return WSSocketNotifier_AddListenerClientSocket(&pThis->mPoller, socket);
#endif
}

/*
	Threaded
*/
#ifdef WSTHREADEDSERVEROPT
WSTHREADENTRYPROCRET WSTHREADENTRYPROCCT ServerWorkerThreadEntry(void* param)
{
	WSSERVERWORKERDATA* data = (WSSERVERWORKERDATA*)param;
	/*WSLogDebug(MutStr("++++++Thread [") << WSThreadGetCurrentID()*/
	/*					<< "] Beginning+++++\n");*/
	while(!data->mShouldShutdown)
	{
		WSRWLock_ReadLock(data->mConfigLockRef);
		WSCriticalSection_Lock(&data->mLock);
			Server_ProcessNotifier(data->mServer, &data->mPoller);
		WSCriticalSection_Unlock(&data->mLock);
		WSRWLock_ReadUnlock(data->mConfigLockRef);
		WSThreadYield();
	}
	data->mIsDone = TRUE;
	/*WSLogDebug(MutStr("------Thread [") << WSThreadGetCurrentID()
				<< "] Ending--------\n");*/
	return 0;
}
#endif

#if defined(__cplusplus)
} /* end extern "C" */
#endif
#if !defined(WIKISERVER_VERSION)
	#undef WSLogDebug
	#undef WSLogError
	#undef WSTimeGetLocalValue
	#undef WSThreadYield
	#undef WSASSERT
	#undef WSMemoryCopy
	#undef WSMemoryFill
	#undef WSMemoryMove
	#undef WSRELEASEUNUSED
	#if defined(_WIN32)
		#undef WSSetWindowLongPtr
		#undef longptr_t
	#endif
#endif
#if defined(SocketURIIsDigit)
	#undef SocketURIIsDigit
	#undef URIIsDigit
	#undef URIParseIPv4address
#endif
#endif /* WSSOCKET_IMPLEMENTATION */
