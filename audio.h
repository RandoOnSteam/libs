/**
*	@file audio.h
*
*	#     #  #####     #    #     # ######  ### #######
*	#  #  # #     #   # #   #     # #     #  #  #     #
*	#  #  # #        #   #  #     # #     #  #  #     #
*	#  #  #  #####  #     # #     # #     #  #  #     #
*	#  #  #       # ####### #     # #     #  #  #     #
*	#  #  # #     # #     # #     # #     #  #  #     #
*	 ## ##   #####  #     #  #####  ######  ### #######
*
*	Audio streaming API
*
*	@todo I've got a list:
*	- Mixing needs to be implemented
*	- ALSA is a little unstable... only rarely though
*	- OSX Core Audio compiles but hasn't been tested
*	- Recording isn't tested
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
*	@version 0.2
*
*	Version History
*
*	0.2		Added the missing WSIIDPARAM definition for WAS
*
*   0.1		Initial
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
#ifndef __AUDIO_H__
#define __AUDIO_H__
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
#ifndef WSASSERT
	#include <assert.h>
	#define WSASSERT(x) assert(x)
#endif
#ifdef __cplusplus
	extern "C" {
#endif
/**	Silence value	*/
#define WSAUDIO_SILENCE(bits) (bits == 8 ? 0x80 : 0x00)
/** Recording/Capture. If not specified it is output. */
#define WSAUDIO_INPUT (1 << 0)
/** OR with WSAUDIO_INPUT to open both directions, omit it for output only. */
#define WSAUDIO_OUTPUT (1 << 1)
/** Allows sample rate at creation to not fit exactly */
#define WSAUDIO_ALLOWFREQUENCYCHANGE (1 << 2)
/** Allows bits per sample at creation to not fit exactly */
#define WSAUDIO_ALLOWFORMATCHANGE (1 << 3)
/** Allows number of channels at creation to not fit exactly */
#define WSAUDIO_ALLOWCHANNELSCHANGE (1 << 4)
/** Allows buffer size at creation to not fit exactly */
#define WSAUDIO_ALLOWSAMPLESCHANGE (1 << 5)
/** Allows all WSAUDIOSTREAMFORMAT elements to not fit exactly at creation */
#define WSAUDIO_ALLOWANYCHANGE (WSAUDIO_ALLOWFREQUENCYCHANGE \
	| WSAUDIO_ALLOWFORMATCHANGE | WSAUDIO_ALLOWCHANNELSCHANGE \
	| WSAUDIO_ALLOWSAMPLESCHANGE)
/** WSAudio_Create(): device field is valid will be opened */
#define WSAUDIO_CREATEFROMDEVICE (1 << 6)

/** Contains info about the type of format in audio/files. */
typedef struct WSAUDIOFORMAT
{
	/** Number of channels in sample data */
	uint16_t channels;
	/** Number of bits in each sample (8, 16 usually) */
	uint16_t bitsPerSample;
	/** Samplerate...  Commonly 44100 */
	uint32_t sampleRate;
} WSAUDIOFORMAT;
#define WSAudioFormat_ConstructDefault(pThis) \
		WSAudioFormat_Construct(pThis, 0, 0, 0)
#define WSAudioFormat_Construct(pThis, wInChannels, wInBitsPerSample, dwInSampleRate) \
		(pThis)->channels = wInChannels; \
		(pThis)->bitsPerSample = wInBitsPerSample; \
		(pThis)->sampleRate = dwInSampleRate
#define WSAudioFormat_ConstructCopy(pThis, pOther) \
		(pThis)->channels = (pOther)->channels; \
		(pThis)->bitsPerSample = (pOther)->bitsPerSample; \
		(pThis)->sampleRate = (pOther)->sampleRate
#define WSAudioFormat_GetBlockAlign(pThis) \
	((((uint16_t)(pThis)->channels) * (pThis)->bitsPerSample) / 8) /* >> 3 */
#define WSAudioFormat_BytesToSamples(pThis, bytes) \
	(bytes) / (pThis)->channels / ((pThis)->bitsPerSample / 8)
#define WSAudioFormat_SamplesToBytes(pThis, samples) \
	(pThis)->channels * ((pThis)->bitsPerSample / 8) * (samples)
/** Contains info about the type of format in audio streams */
typedef struct WSAUDIOSTREAMFORMAT
{
	/** @see WSAUDIOFORMAT */
	WSAUDIOFORMAT mSuper;
	/** DMA buffer size. General purpose: 1024*(bitspersamp/8)*nchan */
	size_t dwBufferSize;
} WSAUDIOSTREAMFORMAT;
#define WSAudioStreamFormat_ConstructDefault(pThis) \
		WSAudioFormat_ConstructDefault(&(pThis)->mSuper); \
		(pThis)->dwBufferSize = 0;
#define WSAudioStreamFormat_ConstructWithFormat(pThis, pFormat, dwInBufferSize) \
		*(pThis)->mSuper = *(pFormat); \
		(pThis)->dwBufferSize = dwInBufferSize
#define WSAudioStreamFormat_Construct(pThis, wInChannels, wInBitsPerSample, dwInSampleRate, dwInBufferSize) \
		WSAudioFormat_Construct(&(pThis)->mSuper, wInChannels, wInBitsPerSample, dwInSampleRate); \
		(pThis)->dwBufferSize = dwInBufferSize

/** Contains information about this device */
typedef struct WSAUDIODEVICE
{
	char* name;
	WSAUDIOFORMAT maxformat;
	size_t id;
	uint8_t flags;
} WSAUDIODEVICE;
/** Contains information about all devices */
typedef struct WSAUDIOENUM
{
	WSAUDIODEVICE* devices;
	size_t numdevices;
} WSAUDIOENUM;
#define WSAudioDevice_Destruct(pThis) \
	WSFREE((pThis)->name)
#define WSAudioEnum_Destruct(pThis) \
	for(; (pThis)->numdevices != 0; --(pThis)->numdevices) \
		{ WSAudioDevice_Destruct(&(pThis)->devices[\
									(pThis)->numdevices - 1]); } \
	WSFREE((pThis)->devices)
/** Callback to fill data in out mode and has data in in mode */
typedef void (*WSAudioFillProc)(
	void* userData, uint8_t* buffer, size_t bufferSize);
/** Specification to determinate which to device to open and how */
typedef struct WSAUDIOSPEC
{
	WSAUDIOSTREAMFORMAT format;
	WSAudioFillProc proc;
	void* procuserdata;
	WSAUDIODEVICE* device;
	int flags;
} WSAUDIOSPEC;
/** Internal audio data */
typedef struct WSAUDIO WSAUDIO;

/** @return TRUE if simultaneous i/o is supported */
wsbool WSAudioHasFullDuplex();

/** @param enumparams structure filled with enumeration info */
void WSAudioEnumerate(WSAUDIOENUM* enumparams);

/**
* Constructs/Creates the device based on the format passed in spec.
* @param spec format in it contains the requirements for the device opened
* @return TRUE on successs
*/
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec);

/** Closes the device and frees any data */
void WSAudio_Destroy(WSAUDIO* pThis);

/** @param panleft  left pan up to GetMaxVolume()
* @param panright right pan up to GetMaxVolume()
* WSAudio_GetMaxVolume() = all right. Output only. */
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright);

/** @param volume 0 == silent. WSAudio_GetMaxVolume() = full volume.
* Output only. */
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume);

/** Obtains the maximum volume value for WSAudio_SetVolume() */
int WSAudio_GetMaxVolume(WSAUDIO* pThis);

/** Obtains the minimum volume value for WSAudio_SetVolume() */
int WSAudio_GetMinVolume(WSAUDIO* pThis);

/** Sends the current buffer for output/recording.*/
wsbool WSAudio_Play(WSAUDIO* pThis);

/** @return TRUE if playing, FALSE if paused.  */
wsbool WSAudio_IsPlaying(WSAUDIO* pThis);

/** Pauses. Call WSAudio_Resume(), NOT play, to resume. */
wsbool WSAudio_Pause(WSAUDIO* pThis);

/** Resume from WSAudio_Pause(). */
wsbool WSAudio_Resume(WSAUDIO* pThis);

/** Gets the byte position in the internal buffer. May not be applicable. */
size_t WSAudio_GetPosition(WSAUDIO* pThis);

/** MUST be called in your idle event handler. */
void WSAudio_Process(WSAUDIO* pThis);

/*=============================================================================
						Forward Declarations
 ============================================================================*/
//#define WSAUDIOSM_USEDOUBLEBUFFER
#if defined(__MACH__)
	#if defined(__has_builtin) && __has_builtin(__is_target_arch) \
		&& __has_builtin(__is_target_vendor) && __has_builtin(__is_target_os) \
		&& __has_builtin(__is_target_environment) \
		&& (__is_target_arch(x86_64) \
			&& __is_target_vendor(apple) && __is_target_os(ios)) \
			|| (__is_target_arch(x86_64) && __is_target_vendor(apple) \
				&& __is_target_os(ios) && __is_target_environment(simulator))
		&& __is_target_vendor(apple) && __is_target_os(ios)) \
			|| (__is_target_arch(x86_64) && __is_target_vendor(apple) \
				&& __is_target_os(ios) && __is_target_environment(simulator))
		#define WSAUDIOMACIOS
		#define WSAUDIOAVNUMBUFFERS 3
	#endif
#endif
#if defined(macintosh) || (defined(_WIN32) && defined(WSAUDIOSMOPT))
	#define WSAUDIOSM_NUMBUFFERS 2
	#if defined(_WIN64)
		#undef WSAUDIOSMOPT /* 32-bit only */
	#endif
	#if defined(TARGET_API_MAC_CARBON) && TARGET_API_MAC_CARBON
		#define WSAUDIO_ISCARBON
	#endif
	#if defined(__MACH__)
		#include <CoreServices/../Frameworks/CarbonCore.framework/Headers/ConditionalMacros.h>
	#else
		#include <ConditionalMacros.h>
	#endif
	#if PRAGMA_STRUCT_ALIGN
		#pragma options align=mac68k
	#elif PRAGMA_STRUCT_PACKPUSH
		#pragma pack(push, 2)
	#elif PRAGMA_STRUCT_PACK
		#pragma pack(2)
	#endif
	typedef struct WSAUDIOExtSoundHeader
	{
		char* samplePtr; /* Ptr */
		unsigned long numChannels;
		unsigned long sampleRate; /* UnsignedFixed */
		unsigned long loopStart;
		unsigned long loopEnd;
		uint8_t encode;
		uint8_t baseFrequency;
		unsigned long numFrames;
		#if TARGET_CPU_68K && !TARGET_RT_MAC_68881
			long double AIFFSampleRate;
		#else
			struct
			{
				int16_t exp;
				uint16_t man[4];
			} AIFFSampleRate;
		#endif
		char* markerChunk; /* Ptr */
		char* instrumentChunks; /* Ptr */
		char* AESRecording; /* Ptr */
		unsigned short sampleSize;
		unsigned short futureUse1;
		unsigned long futureUse2;
		unsigned long futureUse3;
		unsigned long futureUse4;
		uint8_t sampleArea[1];
	} WSAUDIOExtSoundHeader;
	typedef struct WSAUDIOSndDoubleBufferHeader
	{
		short dbhNumChannels;
		short dbhSampleSize;
		short dbhCompressionID;
		short dbhPacketSize;
		unsigned long dbhSampleRate; /* UnsignedFixed */
		/* SndDoubleBufferPtr */
		struct SndDoubleBuffer* dbhBufferPtr[2];
		void* dbhDoubleBack; /* SndDoubleBackUPP */
	} WSAUDIOSndDoubleBufferHeader;
	typedef struct WSAUDIOSPB
	{
		long inRefNum;
		unsigned long count;
		unsigned long milliseconds;
		unsigned long bufferLength;
		char* bufferPtr; /* Ptr */
		void* completionRoutine; /* SICompletionUPP */
		void* interruptRoutine; /* SIInterruptUPP */
		long userLong;
		int16_t error; /* OSErr */
		long unused1;
	} WSAUDIOSPB;
	#if PRAGMA_STRUCT_ALIGN
		#pragma options align=reset
	#elif PRAGMA_STRUCT_PACKPUSH
		#pragma pack(pop)
	#elif PRAGMA_STRUCT_PACK
		#pragma pack()
	#endif
#endif
#if defined(_WIN32) && defined(WSAUDIOXAUDIOOPT) && !defined(WSAUDIOSMOPT)
#define WSAUDIOX2_NUMBUFFERS 2
typedef struct XAUDIO2_BUFFER
{
	uint32_t Flags;
	uint32_t AudioBytes;
	const uint8_t* pAudioData;
	uint32_t PlayBegin;
	uint32_t PlayLength;
	uint32_t LoopBegin;
	uint32_t LoopLength;
	uint32_t LoopCount;
	void* pContext;
} XAUDIO2_BUFFER;
typedef struct IXAudio2VoiceCallback IXAudio2VoiceCallback;
typedef struct IXAudio2VoiceCallbackVtbl
{
	void (__stdcall *OnVoiceProcessingPassStart)(IXAudio2VoiceCallback* This,
		uint32_t BytesRequired);
	void (__stdcall *OnVoiceProcessingPassEnd)(IXAudio2VoiceCallback* This);
	void (__stdcall *OnStreamEnd)(IXAudio2VoiceCallback* This);
	void (__stdcall *OnBufferStart)(IXAudio2VoiceCallback* This,
		void* pBufferContext);
	void (__stdcall *OnBufferEnd)(IXAudio2VoiceCallback* This,
		void* pBufferContext);
	void (__stdcall *OnLoopEnd)(IXAudio2VoiceCallback* This,
		void* pBufferContext);
	void (__stdcall *OnVoiceError)(IXAudio2VoiceCallback* This,
		void* pBufferContext, long Error);
} IXAudio2VoiceCallbackVtbl;
typedef struct IXAudio2VoiceCallback
{
	IXAudio2VoiceCallbackVtbl* lpVtbl;
} IXAudio2VoiceCallback;
#endif

/*=============================================================================
						WSAUDIO declaration
 ============================================================================*/
 typedef struct WSAUDIO
{
	WSAudioFillProc mFillProc;
	void* mUserData;
	int mFlags;
#if defined(_WIN32) && !defined(WSAUDIOSMOPT)
	#if !defined(WSAUDIODSOPT) && !defined(WSAUDIOKSOPT) \
		&& !defined(WSAUDIOWASOPT) && !defined(WSAUDIOXAUDIOOPT)
		#if defined(WSAUDIOWMMEUSECIRCULAR)
			#define WSNUMWAVEBUFFERS 1
		#else
			#define WSNUMWAVEBUFFERS 2
		#endif
		struct WSAUDIOWAVEHDR
		{
			char*       lpData;
			uint32_t       dwBufferLength;
			uint32_t       dwBytesRecorded;
			size_t   dwUser;
			uint32_t       dwFlags;
			uint32_t       dwLoops;
			struct WSAUDIOWAVEHDR* lpNext;
			size_t   reserved;
		} mHeaders[WSNUMWAVEBUFFERS];
		#if defined(WSAUDIOWMMEUSECIRCULAR)
			int mCurDiv;
		#else
			int mNextHeader;
		#endif
			uint8_t* mBuffer;
			union
			{
				struct HWAVEIN__* mhWaveIn;
				struct HWAVEOUT__* mhWaveOut;
			};
	#elif defined(_WIN32) && !defined(WSAUDIOKSOPT) \
		&& !defined(WSAUDIOWASOPT) && !defined(WSAUDIOXAUDIOOPT)
		/** Last chunk in buffer played (buffer is div into WSNUMDXCHUNKS) */
		uint32_t mPrevChunk;
		/** Size of a chunk in the buffer */
		uint32_t mBufferSize;
		union
		{
			struct IDirectSoundBuffer* mSoundBuffer;
			struct IDirectSoundCaptureBuffer* mCaptureBuffer;
		};
		union
		{
			struct IDirectSound* mSound;
			struct IDirectSoundCapture* mCapture;
		};
		struct HINSTANCE__* mhDSound;
	#elif !defined(WSAUDIOWASOPT) && !defined(WSAUDIOXAUDIOOPT)
		uint8_t* mBuffer;
		unsigned long mBufferSize;
		unsigned long mPrevChunk;
		void* mhPin;
		void* mhDevice;
		struct HINSTANCE__* mhKSUserDll;
		unsigned long mVolumeNodeId;
		uint16_t mNumChannels;
		int mMinVolume;
		int mMaxVolume;
	#elif !defined(WSAUDIOXAUDIOOPT)
		struct HINSTANCE__* mOle32;
		struct IAudioClient* client;
		struct IAudioRenderClient* render;
		struct IAudioCaptureClient* capture;
		uint32_t mReqFrames;
		uint32_t mBlockAlign;
		size_t mReqSize;
	#else
		struct IXAudio2* engine;
		struct IXAudio2MasteringVoice* masteringVoice;
		struct IXAudio2SourceVoice* sourceVoice;
		XAUDIO2_BUFFER buffers[WSAUDIOX2_NUMBUFFERS];
		uint32_t bufsize;
		IXAudio2VoiceCallback voicecbs;
		IXAudio2VoiceCallbackVtbl voicecbsvtbl;
		struct HINSTANCE__* mOle32;
		#if !defined(WSMSIXOPT)
			struct HINSTANCE__* mXAudioDLL;
		#endif
	#endif
#elif defined(__amigaos__)
	struct IOAudio* mAio[NUMAIO];
	struct MsgPort* mPort[NUMAIO / 2];
	size_t mNumAIO;
	int mIsPlaying;
	int mAioIndex;
#elif defined(macintosh) || (defined(_WIN32) && defined(WSAUDIOSMOPT))
	char* mBuffer; /* Ptr */
	size_t mBufferSize;
	union
	{
		struct
		{
			struct SndChannel* mChannel; /* SndChannelPtr */
			union
			{
				struct
				{
					void* mCallBack; /* SndCallBackUPP */
					WSAUDIOExtSoundHeader mHeader;
					size_t mNextChunk;
					wsbool mBufferReady[WSAUDIOSM_NUMBUFFERS];
				};
				struct
				{
					WSAUDIOSndDoubleBufferHeader mDblHeader;
					size_t mBufferFrames;
				};
			};
		};
		WSAUDIOSPB mSPB;
	};
	#if defined(WSAUDIOSM_USEDOUBLEBUFFER) \
				&& defined(macintosh) && !defined(WSAUDIO_ISCARBON)
		wsbool mIsDoubleBufferAvail;
	#endif
	#if defined(macintosh) && !defined(WSAUDIO_ISCARBON)
		wsbool mIsInputAvail;
	#endif
#elif defined(__MACH__)
	#if defined(WSAUDIOMACIOS)
		struct OpaqueAudioQueue* mAudioQueue;
		struct AudioQueueBuffer* mAudioBuffers[WSAUDIOAVNUMBUFFERS];
	#else
		struct OpaqueAUGraph* kGraph; /* AUGraph */
		struct ComponentInstanceRecord* kUnit; /* AudioUnit */
		struct ComponentInstanceRecord* kInputUnit; /* AudioUnit */
		struct ComponentInstanceRecord* kMixer; /* AudioUnit */
		WSAUDIOFORMAT format;
		uint8_t* inputbuffer;
	#endif
#elif (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__) \
		|| defined(__linux__))
	#if defined(WSAUDIOALSAOPT)
		char* mBuffer;
		size_t mBufferSize;
		size_t mBufferFrames;
		unsigned int mBlockAlign;
		long mMinVolume, mMaxVolume;
		struct _snd_pcm* mPcm; /* snd_pcm_t* */
		struct _snd_mixer* mMixer; /* snd_mixer_t* */
		struct _snd_mixer_elem* mMixerElem; /* snd_mixer_elem_t* */
		struct _snd_pcm_hw_params* mHwparams; /* snd_pcm_hw_params_t* */
		struct _snd_pcm_sw_params* mSwparams; /* snd_pcm_sw_params_t* */
		int mPollFDCount;
		struct pollfd* mPollFDs;
		wsbool mGotAgainI, mGotAgainO;
	#else
		int mFd;
		char* mBuffer;
		int mBufferSize;
	#endif
#endif
} WSAUDIO;

#ifdef __cplusplus
	}
#endif
#endif /* __AUDIO_H__ */
#if defined(WSAUDIO_IMPLEMENTATION)
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
=============================================================================*/
#if defined(_WIN32) && !defined(WSAUDIODSOPT) && !defined(WSAUDIOKSOPT) \
		&& !defined(WSAUDIOWASOPT) && !defined(WSAUDIOXAUDIOOPT) \
		&& !defined(WSAUDIOSMOPT)
#include <windows.h>
#include <mmreg.h>
#include <crtdbg.h>
#pragma comment (lib, "winmm.lib")

/*
	WSAUDIOWMMEUSECIRCULAR is allegro's singular circular buffer
	method.

	PROS: Less overhead, only 1 WMME buffer needed. Works well
	in a single thread.
	CONS: Terrible in a second thread

	!WSAUDIOWMMEUSECIRCULAR is the bog-standard multiple buffer
	method.

	PROS: Works well in a secondary thread.
	CONS: Needs a minumum buffer size, high overhead.
*/
#define WSAUDIOWMMENUMDIVS 32


#ifndef DWORD_PTR /* For MSVC6 */
	#define DWORD_PTR size_t
#endif
#ifndef WAVE_FORMAT_44M08 /* For MSVC6 */
	#define WAVE_FORMAT_44M08 0x00000100
	#define WAVE_FORMAT_44S08 0x00000200
	#define WAVE_FORMAT_44M16 0x00000400
	#define WAVE_FORMAT_44S16 0x00000800
	#define WAVE_FORMAT_48M08 0x00001000
	#define WAVE_FORMAT_48S08 0x00002000
	#define WAVE_FORMAT_48M16 0x00004000
	#define WAVE_FORMAT_48S16 0x00008000
	#define WAVE_FORMAT_96M08 0x00010000
	#define WAVE_FORMAT_96S08 0x00020000
	#define WAVE_FORMAT_96M16 0x00040000
	#define WAVE_FORMAT_96S16 0x00080000
#endif

/*---------------------------------------------------------------------------
						UTILITIES
---------------------------------------------------------------------------*/
#if defined(__cplusplus)
extern "C" {
#endif
const char* WSManufacturerMMIDToString(WORD wId)
{
	switch (wId)
	{
	case MM_GRAVIS:
		return "Advanced Gravis Computer Technology, Ltd.";

	case MM_ANTEX:
		return "Antex Electronics Corporation";

	case MM_APPS:
		return "APPS Software";

	case MM_ARTISOFT:
		return "Artisoft, Inc.";

	case MM_AST:
		return "AST Research, Inc.";

	case MM_ATI:
		return "ATI Technologies, Inc.";

	case MM_AUDIOFILE:
		return "Audio, Inc.";

	case MM_APT:
	case MM_AUDIOPT:
		return "Audio Processing Technology";

	case MM_AURAVISION:
		return "Auravision Corporation";

	case MM_AZTECH:
		return "Aztech Labs, Inc.";

	case MM_CANOPUS:
		return "Canopus, Co., Ltd.";

	case MM_COMPUSIC:
		return "Compusic";

	case MM_CAT:
		return "Computer Aided Technology, Inc.";

	case MM_COMPUTER_FRIENDS:
		return "Computer Friends, Inc.";

	case MM_CONTROLRES:
		return "Control Resources Corporation";

	case MM_CREATIVE:
		return "Creative Labs, Inc.";

	case MM_DIALOGIC:
		return "Dialogic Corporation";

	case MM_DOLBY:
		return "Dolby Laboratories, Inc.";

	case MM_DSP_GROUP:
		return "DSP Group, Inc.";

	case MM_DSP_SOLUTIONS:
		return "DSP Solutions, Inc.";

	case MM_ECHO:
		return "Echo Speech Corporation";

	case MM_ESS:
		return "ESS Technology, Inc.";

	case MM_EVEREX:
		return "Everex Systems, Inc.";

	case MM_EXAN:
		return "EXAN, Ltd.";

	case MM_FUJITSU:
		return "Fujitsu, Ltd.";

	case MM_IOMAGIC:
		return "I/O Magic Corporation";

	case MM_ICL_PS:
		return "ICL Personal Systems";

	case MM_OLIVETTI:
		return "Ing. C. Olivetti & C., S.p.A.";

	case MM_ICS:
		return "Integrated Circuit Systems, Inc.";

	case MM_INTEL:
		return "Intel Corporation";

	case MM_INTERACTIVE:
		return "InterActive, Inc.";

	case MM_IBM:
		return "International Business Machines ";

	case MM_ITERATEDSYS:
		return "Iterated Systems, Inc.";

	case MM_LOGITECH:
		return "Logitech, Inc.";

	case MM_LYRRUS:
		return "Lyrrus, Inc.";

	case MM_MATSUSHITA:
		return "Matsushita Electric Corporation of America";

	case MM_MEDIAVISION:
		return "Media Vision, Inc.";

	case MM_METHEUS:
		return "Metheus Corporation";

	case MM_MELABS:
		return "microEngineering Labs";

	case MM_MICROSOFT:
		return "Microsoft Corporation";

	case MM_MOSCOM:
		return "MOSCOM Corporation";

	case MM_MOTOROLA:
		return "Motorola, Inc.";

	case MM_NMS:
		return "Natural MicroSystems Corporation";

	case MM_NCR:
		return "NCR Corporation";

	case MM_NEC:
		return "NEC Corporation";

	case MM_NEWMEDIA:
		return "New Media Corporation";

	case MM_OKI:
		return "OKI";

	case MM_OPTI:
		return "OPTi, Inc.";

	case MM_ROLAND:
		return "Roland Corporation";

	case MM_SCALACS:
		return "SCALACS";

	case MM_EPSON:
		return "Seiko Epson Corporation, Inc.";

	case MM_SIERRA:
		return "Sierra Semiconductor Corporation";

	case MM_SILICONSOFT:
		return "Silicon Software, Inc.";

	case MM_SONICFOUNDRY:
		return "Sonic Foundry";

	case MM_SPEECHCOMP:
		return "Speech Compression";

	case MM_SUPERMAC:
		return "Supermac Technology, Inc.";

	case MM_TANDY:
		return "Tandy Corporation";

	case MM_KORG:
		return "Toshihiko Okuhura, Korg, Inc.";

	case MM_TRUEVISION:
		return "Truevision, Inc.";

	case MM_TURTLE_BEACH:
		return "Turtle Beach Systems";

	case MM_VAL:
		return "Video Associates Labs, Inc.";

	case MM_VIDEOLOGIC:
		return "VideoLogic, Inc.";

	case MM_VITEC:
		return "Visual Information Technologies, Inc.";

	case MM_VOCALTEC:
		return "VocalTec, Inc.";

	case MM_VOYETRA:
		return "Voyetra Technologies";

	case MM_WANGLABS:
		return "Wang Laboratories";

	case MM_WILLOWPOND:
		return "Willow Pond Corporation";

	case MM_WINNOV:
		return "Winnov, LP";

	case MM_XEBEC:
		return "Xebec Multimedia Solutions Limited";

	case MM_YAMAHA:
		return "Yamaha Corporation of America";
	}
	return "Unknown";
}

char* WMMEErrorToString(MMRESULT mr)
{
	char* szBuffer = WSALLOC(char, MAX_PATH);
	waveOutGetErrorTextA(mr, szBuffer, MAX_PATH);
	return szBuffer;
}

void WSAudioFormatToWaveFormatEx(
	const WSAUDIOFORMAT* Format, WAVEFORMATEX* wfx)
{
	wfx->cbSize = 0;
	wfx->wFormatTag = WAVE_FORMAT_PCM;
	wfx->wBitsPerSample = Format->bitsPerSample;
	wfx->nSamplesPerSec = Format->sampleRate;
	wfx->nChannels = Format->channels;
	wfx->nBlockAlign = WSAudioFormat_GetBlockAlign(Format);
	wfx->nAvgBytesPerSec = Format->sampleRate * wfx->nBlockAlign;
}

void WSWaveFormatExToAudioFormat(
	const WAVEFORMATEX* wfx, WSAUDIOFORMAT* Format)
{
	Format->bitsPerSample = wfx->wBitsPerSample;
	Format->sampleRate = wfx->nSamplesPerSec;
	Format->channels = wfx->nChannels;
}


void WSDevCapsFormatToAudioFormat(DWORD dwFormat, WSAUDIOFORMAT* af)
{
	if (dwFormat & (WAVE_FORMAT_96S16 | WAVE_FORMAT_96M16
					| WAVE_FORMAT_96S08 | WAVE_FORMAT_96M08))
	{
		af->sampleRate = 96000;
		dwFormat &= (WAVE_FORMAT_96S16 | WAVE_FORMAT_96M16
					| WAVE_FORMAT_96S08 | WAVE_FORMAT_96M08);
	}
	else if (dwFormat & (WAVE_FORMAT_48S16 | WAVE_FORMAT_48M16
						| WAVE_FORMAT_48S08 | WAVE_FORMAT_48M08))
	{
		af->sampleRate = 48000;
		dwFormat &= (WAVE_FORMAT_48S16 | WAVE_FORMAT_48M16
						| WAVE_FORMAT_48S08 | WAVE_FORMAT_48M08);
	}
	else if (dwFormat & (WAVE_FORMAT_44S16 | WAVE_FORMAT_44M16
						| WAVE_FORMAT_44S08 | WAVE_FORMAT_44M08))
	{
		af->sampleRate = 44100;
		dwFormat &= (WAVE_FORMAT_44S16 | WAVE_FORMAT_44M16
						| WAVE_FORMAT_44S08 | WAVE_FORMAT_44M08);
	}
	else if (dwFormat & (WAVE_FORMAT_2S16 |  WAVE_FORMAT_2M16
						| WAVE_FORMAT_2S08 | WAVE_FORMAT_2M08))
	{
		af->sampleRate = 22050;
		dwFormat &= (WAVE_FORMAT_2S16 |  WAVE_FORMAT_2M16
						| WAVE_FORMAT_2S08 | WAVE_FORMAT_2M08);
	}
	else /* WAVE_FORMAT_1S16 | WAVE_FORMAT_1M16 |
			WAVE_FORMAT_1S08 | WAVE_FORMAT_1M08 */
	{
		af->sampleRate = 11025;
		dwFormat &= WAVE_FORMAT_1S16 | WAVE_FORMAT_1M16 |
			WAVE_FORMAT_1S08 | WAVE_FORMAT_1M08;
	}

	af->bitsPerSample = (dwFormat & (WAVE_FORMAT_1M16 | WAVE_FORMAT_1S16
		| WAVE_FORMAT_2M16 | WAVE_FORMAT_2S16 | WAVE_FORMAT_4M16
		| WAVE_FORMAT_4S16 | WAVE_FORMAT_48M16 | WAVE_FORMAT_48S16
		| WAVE_FORMAT_96M16 | WAVE_FORMAT_96S16)) ? 16 : 8;

	af->channels = (dwFormat & (WAVE_FORMAT_1S08 | WAVE_FORMAT_1S16
		| WAVE_FORMAT_2S08 | WAVE_FORMAT_2S16 | WAVE_FORMAT_4S08
		| WAVE_FORMAT_4S16 | WAVE_FORMAT_48S08 | WAVE_FORMAT_48S16
		| WAVE_FORMAT_96S08 | WAVE_FORMAT_96S16)) ? 2 : 1;
}

#define WSDevCapsToAudioDevice(dc, ad) \
{ \
	(ad)->name = StringDup((dc)->szPname); \
	WSDevCapsFormatToAudioFormat((dc)->dwFormats, &(ad)->maxformat); \
	/*EnumParams.szDesc = WSManufacturerMMIDToString(WAVECAPS.wMid);*/ \
}


/*---------------------------------------------------------------------------
						WSAUDIO WMME
---------------------------------------------------------------------------*/


static void CALLBACK WaveInProc(HWAVEIN hwi, UINT uMsg,
	DWORD_PTR dwInstance,
	DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	(void)(hwi);
	(void)(dwParam1); (void)(dwParam2);
	if (uMsg == MM_WIM_DATA)
	{
		/*WSAUDIO* pThis = (WSAUDIO*)dwInstance;*/
		/*WAVEHDR* curhead = &pThis->mHeaders[pThis->mNextHeader];*/

		WSAudio_Play((WSAUDIO*)dwInstance);
	}
}


static void CALLBACK WaveOutProc(HWAVEOUT hwo, UINT uMsg,
	DWORD_PTR dwInstance,
	DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	(void)(hwo);
	(void)(dwParam1); (void)(dwParam2);
	if (uMsg == MM_WOM_DONE)
		WSAudio_Play((WSAUDIO*)dwInstance);
}

void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	UINT i;
	UINT incount = waveInGetNumDevs();
	UINT outcount = waveOutGetNumDevs();
	enumparams->numdevices = 0;
	enumparams->devices = WSALLOC(WSAUDIODEVICE, incount + outcount);
	for(i = 0; i < incount; ++i)
	{
		WAVEINCAPS wic;
		if(waveInGetDevCapsA(i, &wic, sizeof(WAVEINCAPS)) != MMSYSERR_NOERROR)
			continue;
		WSDevCapsToAudioDevice(&wic,
			&enumparams->devices[enumparams->numdevices]);
		enumparams->devices[enumparams->numdevices].flags = WSAUDIO_INPUT;
		enumparams->devices[enumparams->numdevices].id = i;
		++enumparams->numdevices;
	}
	for(i = 0; i < outcount; ++i)
	{
		WAVEOUTCAPS woc;
		if(waveOutGetDevCapsA(i, &woc, sizeof(WAVEOUTCAPS))
				!= MMSYSERR_NOERROR)
			continue;
		WSDevCapsToAudioDevice(&woc,
			&enumparams->devices[enumparams->numdevices]);
		enumparams->devices[enumparams->numdevices].flags = 0;
		enumparams->devices[enumparams->numdevices].id = i;
		++enumparams->numdevices;
	}
}
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	size_t i;
	WAVEFORMATEX wfx;
	DWORD dwOpenFlags;
	size_t adjbuffersize;

#if 1 || defined(WSAUDIOWMMEUSECIRCULAR)
	dwOpenFlags = CALLBACK_NULL;
#else
	dwOpenFlags = CALLBACK_FUNCTION;
#endif
	WSAudioFormatToWaveFormatEx(&spec->format.mSuper, &wfx);
	if(spec->flags & WSAUDIO_INPUT)
	{
		if(waveInOpen(&pThis->mhWaveIn,
			(spec->flags & WSAUDIO_CREATEFROMDEVICE) ?
				spec->device->id : WAVE_MAPPER, &wfx, (DWORD_PTR)WaveInProc,
			(DWORD_PTR)pThis, dwOpenFlags) != MMSYSERR_NOERROR)
		{
			return FALSE;
		}
	}
	else
	{
		if(waveOutOpen(&pThis->mhWaveOut,
			(spec->flags & WSAUDIO_CREATEFROMDEVICE) ?
				spec->device->id : WAVE_MAPPER, &wfx, (DWORD_PTR)WaveOutProc,
			(DWORD_PTR)pThis, dwOpenFlags) != MMSYSERR_NOERROR)
		{
			return FALSE;
		}
	}

#if !defined(WSAUDIOWMMEUSECIRCULAR)
	/* WMME needs a big buffer when not using a singular circular buffer
		- adjust it to at least .25 of a second
		and align that to the nearest word boundary or WMME screeches */
	if (spec->format.dwBufferSize < spec->format.mSuper.sampleRate )
		adjbuffersize = ((spec->format.mSuper.sampleRate) + 3) & ~3;
	else
#endif
		adjbuffersize = spec->format.dwBufferSize;

#if defined(WSAUDIOWMMEUSECIRCULAR)
	pThis->mBuffer = WSALLOC(uint8_t,
		adjbuffersize * WSAUDIOWMMENUMDIVS);
#else
	pThis->mBuffer = WSALLOC(uint8_t,
		adjbuffersize * WSNUMWAVEBUFFERS);
#endif

	if(!pThis->mBuffer)
	{
		if(spec->flags & WSAUDIO_INPUT)
			waveInClose(pThis->mhWaveIn);
		else
			waveOutClose(pThis->mhWaveOut);
		WSFREE(pThis->mBuffer);
		return FALSE;
	}

#if !defined(WSAUDIOWMMEUSECIRCULAR)
	for(i = 0; i < WSNUMWAVEBUFFERS; ++i)
#else
	for(i = 0; i < 1; ++i)
#endif
	{
		pThis->mHeaders[i].dwBufferLength =
			(adjbuffersize
#if defined(WSAUDIOWMMEUSECIRCULAR)
				* WSAUDIOWMMENUMDIVS
#endif
				)
			;
		pThis->mHeaders[i].lpData = (char*)
			&pThis->mBuffer[
				(adjbuffersize) * i];
		#if defined(WSAUDIOWMMEUSECIRCULAR)
		pThis->mHeaders[i].dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
		/*pThis->mHeaders[i].dwUser = 0xFFFF;*/
		pThis->mHeaders[i].dwLoops = 0x7FFFFFFFL;
		#else
		pThis->mHeaders[i].dwFlags = 0;
		/*pThis->mHeaders[i].dwUser = 0xFFFF;*/
		/*pThis->mHeaders[i].dwLoops = 0;*/
		#endif

		if(spec->flags & WSAUDIO_INPUT)
		{
			if(waveInPrepareHeader(pThis->mhWaveIn,
				(LPWAVEHDR)&pThis->mHeaders[i],
				sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
			{
				for(; i != 0; --i)
					waveInUnprepareHeader(pThis->mhWaveIn,
						(LPWAVEHDR)&pThis->mHeaders[i - 1], sizeof(WAVEHDR));
				waveInClose(pThis->mhWaveIn);
				WSFREE(pThis->mBuffer);
				return FALSE;
			}
		}
		else
		{
			if(waveOutPrepareHeader(pThis->mhWaveOut,
				(LPWAVEHDR)&pThis->mHeaders[i],
				sizeof(WAVEHDR)) != MMSYSERR_NOERROR)
			{
				for(; i != 0; --i)
					waveOutUnprepareHeader(pThis->mhWaveOut,
						(LPWAVEHDR)&pThis->mHeaders[i - 1], sizeof(WAVEHDR));
				waveOutClose(pThis->mhWaveOut);
				WSFREE(pThis->mBuffer);
				return FALSE;
			}
		}
	}
	pThis->mFillProc = spec->proc;
	pThis->mUserData = spec->procuserdata;
	pThis->mFlags = spec->flags;
#if defined(WSAUDIOWMMEUSECIRCULAR)
	pThis->mCurDiv = 0;
#else
	pThis->mNextHeader = 0;
#endif
	return TRUE;
}

void WSAudio_Destroy(WSAUDIO* pThis)
{
	size_t i;
	if(pThis->mFlags & WSAUDIO_INPUT)
	{
		waveInReset(pThis->mhWaveIn);
		for(i = 0; i < WSNUMWAVEBUFFERS; ++i)
			waveInUnprepareHeader(pThis->mhWaveIn,
				(LPWAVEHDR)&pThis->mHeaders[i], sizeof(WAVEHDR));
		waveInClose(pThis->mhWaveIn);
	}
	else
	{
		waveOutReset(pThis->mhWaveOut);
		for(i = 0; i < WSNUMWAVEBUFFERS; ++i)
			waveOutUnprepareHeader(pThis->mhWaveOut,
				(LPWAVEHDR)&pThis->mHeaders[i], sizeof(WAVEHDR));
		waveOutClose(pThis->mhWaveOut);
	}
	WSFREE(pThis->mBuffer);
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright)
{ /* Bytes 1-4 == left channel, 5-8 == right channel */
	WSASSERT(!(pThis->mFlags & WSAUDIO_INPUT));
	return waveOutSetVolume(pThis->mhWaveOut, panleft + (panright << 16))
		== MMSYSERR_NOERROR;
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{ /* Bytes 1-4 == left channel, 5-8 == right channel */
	WSASSERT(!(pThis->mFlags & WSAUDIO_INPUT));
	return waveOutSetVolume(pThis->mhWaveOut, volume + (volume << 16))
		== MMSYSERR_NOERROR;
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	(void)(pThis);
	return 0xFFFF;
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	(void)(pThis);
	return 0;
}
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	MMRESULT res;
#if defined(WSAUDIOWMMEUSECIRCULAR)
	WAVEHDR* curhead = (LPWAVEHDR)&pThis->mHeaders[0];
#else
	WAVEHDR* curhead = (LPWAVEHDR)&pThis->mHeaders[pThis->mNextHeader];
#endif
	if(pThis->mFlags & WSAUDIO_INPUT)
		res = waveInAddBuffer(pThis->mhWaveIn, curhead, sizeof(WAVEHDR));
	else
	{
#if defined(WSAUDIOWMMEUSECIRCULAR)
		WSAudio_Process(pThis);
		res = waveOutWrite(pThis->mhWaveOut, curhead, sizeof(WAVEHDR));
#else
		size_t i;
		for(i = 0; i < WSNUMWAVEBUFFERS; ++i)
		{
			WAVEHDR* ihead = (LPWAVEHDR)&pThis->mHeaders[i];
			(*pThis->mFillProc)(pThis->mUserData,
				(uint8_t*)ihead->lpData, ihead->dwBufferLength);
			res = waveOutWrite(pThis->mhWaveOut, ihead, sizeof(WAVEHDR));
			if(res != MMSYSERR_NOERROR)
				break;
		}
#endif
	}
	if(res != MMSYSERR_NOERROR)
		return FALSE;
#if !defined(WSAUDIOWMMEUSECIRCULAR) && WSNUMWAVEBUFFERS > 1
	pThis->mNextHeader = (pThis->mNextHeader + 1) & (WSNUMWAVEBUFFERS - 1);
#endif
	return TRUE;
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{
	(void)(pThis);
	return FALSE;
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	if(pThis->mFlags & WSAUDIO_INPUT)
		return waveInStop(pThis->mhWaveIn) == MMSYSERR_NOERROR;
	else
		return waveOutPause(pThis->mhWaveOut) == MMSYSERR_NOERROR;
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	if(pThis->mFlags & WSAUDIO_INPUT)
		return waveInStart(pThis->mhWaveIn) == MMSYSERR_NOERROR;
	else
		return waveOutRestart(pThis->mhWaveOut) == MMSYSERR_NOERROR;
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	MMTIME time;
	MMRESULT res;
	time.wType = TIME_BYTES;
	if(pThis->mFlags & WSAUDIO_INPUT)
		res = waveInGetPosition(pThis->mhWaveIn, &time, sizeof(MMTIME));
	else
		res = waveOutGetPosition(pThis->mhWaveOut, &time, sizeof(MMTIME));
	if(res != MMSYSERR_NOERROR
		|| time.wType != TIME_BYTES)
		return 0;
	return time.u.cb;
}
wsbool WSAudioHasFullDuplex() { return FALSE; }
void WSAudio_Process(WSAUDIO* pThis)
{
#if defined(WSAUDIOWMMEUSECIRCULAR)
	WAVEHDR* curhead = (LPWAVEHDR)&pThis->mHeaders[0];
	size_t writepos = WSAudio_GetPosition(pThis);
	size_t divsize = curhead->dwBufferLength/WSAUDIOWMMENUMDIVS;
	writepos /= (divsize);
	writepos += 8;
	while (writepos > (WSAUDIOWMMENUMDIVS-1))
		writepos -= WSAUDIOWMMENUMDIVS;
	while (writepos != pThis->mCurDiv)
	{
		if (++pThis->mCurDiv > (WSAUDIOWMMENUMDIVS-1))
			pThis->mCurDiv = 0;

		(*pThis->mFillProc)(pThis->mUserData,
			(uint8_t*)curhead->lpData+((divsize)*pThis->mCurDiv), divsize);
	}
#else
	size_t i;
	MMRESULT res;
	for(i = 0; i < WSNUMWAVEBUFFERS; ++i)
	{
		WAVEHDR* ihead = (LPWAVEHDR)&pThis->mHeaders[i];
		if(!(ihead->dwFlags & WHDR_DONE))
			continue;
		(*pThis->mFillProc)(pThis->mUserData,
			(uint8_t*)ihead->lpData, ihead->dwBufferLength);
		res = waveOutWrite(pThis->mhWaveOut, ihead, sizeof(WAVEHDR));
		if(res != MMSYSERR_NOERROR)
		{
		}
	}
#endif
}
#if defined(__cplusplus)
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

	 ######                                #####
	 #     # # #####  ######  ####  ##### #     #  ####  #    # #    # #####
	 #     # # #    # #      #    #   #   #       #    # #    # ##   # #    #
	 #     # # #    # #####  #        #    #####  #    # #    # # #  # #    #
	 #     # # #####  #      #        #         # #    # #    # #  # # #    #
	 #     # # #   #  #      #    #   #   #     # #    # #    # #   ## #    #
	 ######  # #    # ######  ####    #    #####   ####   ####  #    # #####
=============================================================================*/
#elif defined(_WIN32) && !defined(WSAUDIOKSOPT) && !defined(WSAUDIOWASOPT) \
	&& !defined(WSAUDIOXAUDIOOPT) && !defined(WSAUDIOSMOPT)
#define DIRECTSOUND_VERSION 0x0300
#include <dsound.h>
#if defined(WSMSIXOPT)
	#pragma comment (lib, "dsound.lib")
#endif

#define WSNUMDXCHUNKS 2 /* From my knowledge SDL uses 8 while allegro uses 2 */
#if defined(__cplusplus)
extern "C" {
#endif

typedef HRESULT(WINAPI* LPDirectSoundCaptureCreate) (
	LPGUID lpcGUID,
	LPDIRECTSOUNDCAPTURE* lpDSC,
	LPUNKNOWN pUnkOuter
	);

typedef HRESULT(WINAPI* LPDirectSoundCaptureEnumerateA) (
	LPDSENUMCALLBACKA  lpDSEnumCallback,
	LPVOID  lpContext
	);

typedef HRESULT(WINAPI* LPDirectSoundCreate)(
	LPGUID  lpcGuidDevice,
	LPDIRECTSOUND* ppDS,
	LPUNKNOWN  pUnkOuter
	);
typedef HRESULT(WINAPI* LPDirectSoundEnumerateA)(
	LPDSENUMCALLBACKA  lpDSEnumCallback,
	LPVOID  lpContext
	);

/**
*   [Private] Converts an integer DirectSound error message to a string
*/
const char* DXErrorToString(HRESULT hResult)
{
#define DXEH(m) \
	if (hResult == m) return #m;
	DXEH(DSERR_ALLOCATED);
	DXEH(DSERR_ALREADYINITIALIZED);
	DXEH(DSERR_BUFFERLOST);
	DXEH(DSERR_CONTROLUNAVAIL);
	DXEH(DSERR_GENERIC);
	DXEH(DSERR_NOAGGREGATION);
	DXEH(DSERR_NODRIVER);
	DXEH(DSERR_NOINTERFACE);
	DXEH(DSERR_OTHERAPPHASPRIO);
	DXEH(DSERR_UNINITIALIZED);
	DXEH(DSERR_OUTOFMEMORY);
	DXEH(DSERR_UNSUPPORTED);
	DXEH(DSERR_PRIOLEVELNEEDED);
	DXEH(DSERR_INVALIDPARAM);
	DXEH(DSERR_INVALIDCALL);
	DXEH(DSERR_BADFORMAT);
	return "UNKNOWN ERROR";
}

void WSAudioFormatToWaveFormatEx(
	const WSAUDIOFORMAT* Format, WAVEFORMATEX* wfx)
{
	wfx->cbSize = 0;
	wfx->wFormatTag = WAVE_FORMAT_PCM;
	wfx->wBitsPerSample = Format->bitsPerSample;
	wfx->nSamplesPerSec = Format->sampleRate;
	wfx->nChannels = Format->channels;
	wfx->nBlockAlign = WSAudioFormat_GetBlockAlign(Format);
	wfx->nAvgBytesPerSec = Format->sampleRate * wfx->nBlockAlign;
}

void WSWaveFormatExToAudioFormat(
	const WAVEFORMATEX* wfx, WSAUDIOFORMAT* Format)
{
	Format->bitsPerSample = wfx->wBitsPerSample;
	Format->sampleRate = wfx->nSamplesPerSec;
	Format->channels = wfx->nChannels;
}


void WSDevCapsFormatToAudioFormat(DWORD dwFormat, WSAUDIOFORMAT* af)
{
	if (dwFormat & (WAVE_FORMAT_96S16 | WAVE_FORMAT_96M16
					| WAVE_FORMAT_96S08 | WAVE_FORMAT_96M08))
	{
		af->sampleRate = 96000;
		dwFormat &= (WAVE_FORMAT_96S16 | WAVE_FORMAT_96M16
					| WAVE_FORMAT_96S08 | WAVE_FORMAT_96M08);
	}
	else if (dwFormat & (WAVE_FORMAT_48S16 | WAVE_FORMAT_48M16
						| WAVE_FORMAT_48S08 | WAVE_FORMAT_48M08))
	{
		af->sampleRate = 48000;
		dwFormat &= (WAVE_FORMAT_48S16 | WAVE_FORMAT_48M16
						| WAVE_FORMAT_48S08 | WAVE_FORMAT_48M08);
	}
	else if (dwFormat & (WAVE_FORMAT_44S16 | WAVE_FORMAT_44M16
						| WAVE_FORMAT_44S08 | WAVE_FORMAT_44M08))
	{
		af->sampleRate = 44100;
		dwFormat &= (WAVE_FORMAT_44S16 | WAVE_FORMAT_44M16
						| WAVE_FORMAT_44S08 | WAVE_FORMAT_44M08);
	}
	else if (dwFormat & (WAVE_FORMAT_2S16 |  WAVE_FORMAT_2M16
						| WAVE_FORMAT_2S08 | WAVE_FORMAT_2M08))
	{
		af->sampleRate = 22050;
		dwFormat &= (WAVE_FORMAT_2S16 |  WAVE_FORMAT_2M16
						| WAVE_FORMAT_2S08 | WAVE_FORMAT_2M08);
	}
	else /* WAVE_FORMAT_1S16 | WAVE_FORMAT_1M16 |
			WAVE_FORMAT_1S08 | WAVE_FORMAT_1M08 */
	{
		af->sampleRate = 11025;
		dwFormat &= WAVE_FORMAT_1S16 | WAVE_FORMAT_1M16 |
			WAVE_FORMAT_1S08 | WAVE_FORMAT_1M08;
	}

	af->bitsPerSample = (dwFormat & (WAVE_FORMAT_1M16 | WAVE_FORMAT_1S16
		| WAVE_FORMAT_2M16 | WAVE_FORMAT_2S16 | WAVE_FORMAT_4M16
		| WAVE_FORMAT_4S16 | WAVE_FORMAT_48M16 | WAVE_FORMAT_48S16
		| WAVE_FORMAT_96M16 | WAVE_FORMAT_96S16)) ? 16 : 8;

	af->channels = (dwFormat & (WAVE_FORMAT_1S08 | WAVE_FORMAT_1S16
		| WAVE_FORMAT_2S08 | WAVE_FORMAT_2S16 | WAVE_FORMAT_4S08
		| WAVE_FORMAT_4S16 | WAVE_FORMAT_48S08 | WAVE_FORMAT_48S16
		| WAVE_FORMAT_96S08 | WAVE_FORMAT_96S16)) ? 2 : 1;
}

#define WSDevCapsToAudioDevice(dc, ad) \
{ \
	(ad)->name = StringDup((dc)->szPname); \
	WSDevCapsFormatToAudioFormat((dc)->dwFormats, &(ad)->maxformat); \
	/*EnumParams.szDesc = WSManufacturerMMIDToString(WAVECAPS.wMid);*/ \
}
/*---------------------------------------------------------------------------
					DIRECTSOUND ENUMERATION
---------------------------------------------------------------------------*/

typedef struct WSDSENUM
{
	WSAUDIOENUM* audioenum;
#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
	union
	{
		LPDirectSoundCaptureCreate pfDirectSoundCaptureCreate;
		LPDirectSoundCreate pfDirectSoundCreate;
	};
#endif /* #if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT) */
} WSDSENUM;


BOOL CALLBACK WSDirectSoundInputEnumCallback(
	LPGUID  lpGuid, LPCSTR  lpcstrDescription,
	LPCSTR  lpcstrModule, LPVOID  lpContext)
{
	DSCCAPS caps;
	LPDIRECTSOUNDCAPTURE capture;
	WSDSENUM* dsenum = (WSDSENUM*)lpContext;
#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
	if (FAILED((*dsenum->pfDirectSoundCaptureCreate)(lpGuid, &capture, NULL)))
#else
	if (FAILED(DirectSoundCaptureCreate(lpGuid, &capture, NULL)))
#endif
		return FALSE;
	caps.dwSize = sizeof(DSCCAPS);
	if(SUCCEEDED(IDirectSoundCapture_GetCaps(capture, &caps)))
	{
		/*
			if (lpGuid == NULL)
				nFlags |= WSAUDIODEVICE_PRIMARYDEVICE;
			if ((caps.dwFlags & DSCCAPS_EMULDRIVER) ||
				!(caps.dwFlags & DSCCAPS_CERTIFIED))
				nFlags |= WSAUDIODEVICE_SLOWDEVICE;
			EnumParams.szDesc = lpcstrDescription;
		*/
		WSAUDIODEVICE* newdevs =
			WSREALLOC(dsenum->audioenum->devices, WSAUDIODEVICE,
					dsenum->audioenum->numdevices + 1);
		if(newdevs)
		{
			WSAUDIODEVICE* curdev = &newdevs[dsenum->audioenum->numdevices];
			dsenum->audioenum->devices = newdevs;
			curdev->id = (size_t)lpGuid;
			curdev->name = StringDup(lpcstrModule);
			WSDevCapsFormatToAudioFormat(caps.dwFormats, &curdev->maxformat);
			curdev->flags = WSAUDIO_INPUT;
			++dsenum->audioenum->numdevices;
		}
	}

	IDirectSoundCapture_Release(capture);
	return TRUE; /* TRUE == continue */
}

BOOL CALLBACK WSDirectSoundOutputEnumCallback(
	LPGUID  lpGuid, LPCSTR  lpcstrDescription,
	LPCSTR  lpcstrModule, LPVOID  lpContext)
{
	DSCAPS caps;
	LPDIRECTSOUND capture;
	WSDSENUM* dsenum = (WSDSENUM*)lpContext;
#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
	if (FAILED((*dsenum->pfDirectSoundCreate)(lpGuid, &capture, NULL)))
#else
	if (FAILED(DirectSoundCreate(lpGuid, &capture, NULL)))
#endif
	return FALSE;
	caps.dwSize = sizeof(DSCAPS);
	if(SUCCEEDED(IDirectSound_GetCaps(capture, &caps)))
	{
		/*
			if (lpGuid == NULL)
				nFlags |= WSAUDIODEVICE_PRIMARYDEVICE;
			if ((caps.dwFlags & DSCAPS_EMULDRIVER) ||
				!(caps.dwFlags & DSCAPS_CERTIFIED))
				nFlags |= WSAUDIODEVICE_SLOWDEVICE;
			EnumParams.szDesc = lpcstrDescription;
			if (caps.dwFlags & DSCAPS_CONTINUOUSRATE)
				nFlags |= WSAUDIO_CONTINUOUSRATE;
			if (caps.dwFreeHwMemBytes < caps.dwMaxContigFreeHwMemBytes)
				Formats[0].dwBufferSize = caps.dwFreeHwMemBytes;
			else
				Formats[0].dwBufferSize = caps.dwMaxContigFreeHwMemBytes;
			Formats[1].dwBufferSize = caps.dwTotalHwMemBytes;
		*/
		WSAUDIODEVICE* newdevs =
			WSREALLOC(dsenum->audioenum->devices, WSAUDIODEVICE,
					dsenum->audioenum->numdevices + 1);
		if(newdevs)
		{
			WSAUDIODEVICE* curdev = &newdevs[dsenum->audioenum->numdevices];
			dsenum->audioenum->devices = newdevs;
			curdev->id = (size_t)lpGuid;
			curdev->name = StringDup(lpcstrModule);
			curdev->maxformat.sampleRate = caps.dwMaxSecondarySampleRate;
			if (caps.dwFlags & DSCAPS_PRIMARY16BIT)
				curdev->maxformat.bitsPerSample = 16;
			else
				curdev->maxformat.bitsPerSample = 8;

			if (caps.dwFlags & DSCAPS_PRIMARYSTEREO)
				curdev->maxformat.channels = 2;
			else
				curdev->maxformat.channels = 1;
			curdev->flags = 0;
			++dsenum->audioenum->numdevices;
		}
	}

	IDirectSound_Release(capture);
	return TRUE; /* TRUE == continue */
}

/*===========================================================================
					 DIRECTSOUND IMPLEMENTATION
===========================================================================*/

void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	LPDirectSoundEnumerateA pfDirectSoundEnumerate;
	LPDirectSoundCaptureEnumerateA pfDirectSoundCaptureEnumerate;
	WSDSENUM dsenum;
#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
	HINSTANCE hDSound = LoadLibraryA("dsound.dll");
	if (!hDSound)
		return;
#endif
	enumparams->devices = NULL;
	enumparams->numdevices = 0;
	dsenum.audioenum = enumparams;
#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
	pfDirectSoundEnumerate = (LPDirectSoundEnumerateA)
		GetProcAddress(hDSound, "DirectSoundEnumerateA");
	dsenum.pfDirectSoundCreate =
		(LPDirectSoundCreate)GetProcAddress(hDSound, "DirectSoundCreate");
	if (pfDirectSoundEnumerate != NULL && dsenum.pfDirectSoundCreate != NULL)
		(*pfDirectSoundEnumerate)(WSDirectSoundOutputEnumCallback, &dsenum);
	pfDirectSoundCaptureEnumerate =
		(LPDirectSoundCaptureEnumerateA)
			GetProcAddress(hDSound, "DirectSoundCaptureEnumerateA");
	dsenum.pfDirectSoundCaptureCreate =
		(LPDirectSoundCaptureCreate)
			GetProcAddress(hDSound, "DirectSoundCaptureCreate");
	if (pfDirectSoundCaptureEnumerate != NULL
		&& dsenum.pfDirectSoundCaptureCreate != NULL)
		(*pfDirectSoundCaptureEnumerate)(
			WSDirectSoundInputEnumCallback, &dsenum);

	FreeLibrary(hDSound);
#else
	DirectSoundEnumerate(WSDirectSoundOutputEnumCallback, &dsenum);
	DirectSoundCaptureEnumerate(WSDirectSoundInputEnumCallback, &dsenum);
#endif
}
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	HRESULT hr;
	/*const char* errorstring;*/
	LPGUID lpGuid;
#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
	LPDirectSoundCreate pfDirectSoundCreate;
	LPDirectSoundCaptureCreate pfDirectSoundCaptureCreate;
	pThis->mhDSound = LoadLibraryA("dsound.dll");
	if (!pThis->mhDSound)
		return FALSE;
#endif

	if(spec->flags & WSAUDIO_CREATEFROMDEVICE)
		lpGuid = (LPGUID)spec->device->id;
	else
		lpGuid = NULL;

	spec->format.dwBufferSize = WSUPPERCLAMP(spec->format.dwBufferSize,
		WSTYPECEILING(DWORD) / WSNUMDXCHUNKS);
	pThis->mBufferSize = (DWORD)spec->format.dwBufferSize;

	if(spec->flags & WSAUDIO_INPUT)
	{
		DSCBUFFERDESC bufferdesc;
		WAVEFORMATEX waveformat;
		#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
			pfDirectSoundCaptureCreate = (LPDirectSoundCaptureCreate)
				GetProcAddress(pThis->mhDSound, "DirectSoundCaptureCreate");
			if (FAILED((*pfDirectSoundCaptureCreate)(
					lpGuid, &pThis->mCapture, NULL)))
		#else
			if (FAILED(DirectSoundCaptureCreate(
					lpGuid, &pThis->mCapture, NULL)))
		#endif
		{
			#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
				FreeLibrary(pThis->mhDSound);
			#endif
			return FALSE;
		}

		bufferdesc.dwSize = sizeof(bufferdesc);
		bufferdesc.dwFlags = 0;
		bufferdesc.dwBufferBytes = pThis->mBufferSize * WSNUMDXCHUNKS;
		bufferdesc.lpwfxFormat = &waveformat;
		bufferdesc.dwReserved = 0;
		WSAudioFormatToWaveFormatEx(&spec->format.mSuper, &waveformat);
		if(FAILED(IDirectSoundCapture_CreateCaptureBuffer(pThis->mCapture,
			&bufferdesc, &pThis->mCaptureBuffer, NULL)))
		{
			IDirectSoundCapture_Release(pThis->mCapture);
			#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
				FreeLibrary(pThis->mhDSound);
			#endif
			return FALSE;
		}
	}
	else
	{
		/*
			HWND hWnd;
			if (CreationParams.pPriorityHandler == NULL)
			{
				hWnd = GetForegroundWindow();
				if (hWnd == NULL)
					hWnd = GetDesktopWindow();
			}
			else
				hWnd = (HWND)((WSWindow*&)CreationParams.pPriorityHandler)->GetHWND();

			WSDSCHECK(((LPDIRECTSOUND&)EnumParams.lParam)->SetCooperativeLevel(hWnd,
				CreationParams.nPriority == WSAUDIO_APPEXCLUSIVE ? DSSCL_EXCLUSIVE :
				DSSCL_PRIORITY
				DSSCL_NORMAL
			), "Could not set cooperative level of DirectSound object");
		*/
		DSBUFFERDESC bufferdesc;
		WAVEFORMATEX waveformat;
		#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
			pfDirectSoundCreate = (LPDirectSoundCreate)
				GetProcAddress(pThis->mhDSound, "DirectSoundCreate");
			if (FAILED((*pfDirectSoundCreate)(
					lpGuid, &pThis->mSound, NULL)))
		#else
			if (FAILED(DirectSoundCreate(
					lpGuid, &pThis->mSound, NULL)))
		#endif
		{
			#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
				FreeLibrary(pThis->mhDSound);
			#endif
			return FALSE;
		}

		bufferdesc.dwSize = sizeof(bufferdesc);
		if(0) /* Primary */
			bufferdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER
				| DSBCAPS_CTRLVOLUME;
		else /* Secondary */
			bufferdesc.dwFlags =
			DSBCAPS_CTRLVOLUME /*| DSBCAPS_CTRL3D*/
				| DSBCAPS_CTRLPAN
				| /*DSBCAPS_LOCSOFTWARE
				| DSBCAPS_CTRLPOSITIONNOTIFY
				| DSBCAPS_GETCURRENTPOSITION2
				| */
			DSBCAPS_GLOBALFOCUS;
		bufferdesc.dwReserved = 0;

		if (bufferdesc.dwFlags & DSBCAPS_PRIMARYBUFFER)
		{
			bufferdesc.dwBufferBytes = 0;
			bufferdesc.lpwfxFormat = NULL;
		}
		else
		{
			WSAudioFormatToWaveFormatEx(&spec->format.mSuper, &waveformat);
			bufferdesc.dwBufferBytes = pThis->mBufferSize * WSNUMDXCHUNKS;
			bufferdesc.lpwfxFormat = &waveformat;
		}

		hr = IDirectSound_CreateSoundBuffer(pThis->mSound,
			&bufferdesc, &pThis->mSoundBuffer, NULL);
		if(FAILED(hr)
			|| FAILED(IDirectSound_SetCooperativeLevel(pThis->mSound,
					GetDesktopWindow(), DSSCL_NORMAL)))
		{
			/*errorstring = DXErrorToString(hr);*/
			IDirectSound_Release(pThis->mSound);
			#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
				FreeLibrary(pThis->mhDSound);
			#endif
			return FALSE;
		}
		/* Some APIs including miniaudio set the format explicitly afterwards
		IDirectSoundBuffer_SetFormat(pThis->mSoundBuffer, &waveformat);
		*/
	}
	/* Optionally fill the buffer with silence...
	{
		void* pData;
		DWORD dwData;
			hr = IDirectSoundBuffer_Lock(pThis->mSoundBuffer, 0,
				spec->format.dwBufferSize * WSNUMDXCHUNKS, &pData, &dwData, NULL, 0, DSBLOCK_ENTIREBUFFER);
			WSMemoryFill(pData, 0, spec->format.dwBufferSize * WSNUMDXCHUNKS);
			hr = IDirectSoundBuffer_Unlock(pThis->mSoundBuffer,
				pData, dwData, NULL, 0);
	}
	*/
	pThis->mFlags = spec->flags;
	pThis->mFillProc = spec->proc;
	pThis->mUserData = spec->procuserdata;
	pThis->mPrevChunk = WSNUMDXCHUNKS;
	return TRUE;
}
void WSAudio_Destroy(WSAUDIO* pThis)
{
	if(pThis->mFlags & WSAUDIO_INPUT)
	{
		IDirectSoundCaptureBuffer_Release(pThis->mCaptureBuffer);
		IDirectSoundCapture_Release(pThis->mCapture);
	}
	else
	{
		IDirectSoundBuffer_Release(pThis->mSoundBuffer);
		IDirectSound_Release(pThis->mSound);
	}
	#if !defined(WSAUDIO_STATICDSOUND) && !defined(WSMSIXOPT)
		FreeLibrary(pThis->mhDSound);
	#endif
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright)
{
	/*
		-(panleft) + panright

		Your calculation: -(panleft) + panright = -(-10000) + 0 = 10000
		Desired outcome: 10000 (full right)
		Result: 10000
		This correctly represents a full right pan, as desired.
		When panleft = 0 and panright = -10000:

		Your calculation: -(panleft) + panright = -(0) + (-10000) = -10000
		Desired outcome: -10000 (full left)
		Result: -10000
		This correctly represents a full left pan, as desired.
		When panleft = 0 and panright = 0:

		Your calculation: -(panleft) + panright = -(0) + 0 = 0
		Desired outcome: 0 (centered)
		Result: 0
		This correctly represents a centered pan, as desired.
	*/
	int pan = -(panleft) + panright;
	HRESULT hr;
	WSASSERT(!(pThis->mFlags & WSAUDIO_INPUT));
	hr = IDirectSoundBuffer_SetPan(pThis->mSoundBuffer, pan);
	return SUCCEEDED(hr);
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{
	HRESULT hr;
	WSASSERT(!(pThis->mFlags & WSAUDIO_INPUT));
	hr = IDirectSoundBuffer_SetVolume(pThis->mSoundBuffer, volume);
	return SUCCEEDED(hr);
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	return DSBVOLUME_MAX;
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	return DSBVOLUME_MIN;
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{
	DWORD status;
	if(pThis->mFlags & WSAUDIO_INPUT)
	{
		IDirectSoundCaptureBuffer_GetStatus(pThis->mCaptureBuffer, &status);
		return status & DSCBSTATUS_CAPTURING;
	}
	else
	{
		IDirectSoundBuffer_GetStatus(pThis->mSoundBuffer, &status);
		if(status & DSBSTATUS_BUFFERLOST)
		{
			IDirectSoundBuffer_Restore(pThis->mSoundBuffer);
			IDirectSoundBuffer_GetStatus(pThis->mSoundBuffer, &status);
			if(status & DSBSTATUS_BUFFERLOST)
				return FALSE;
		}
		return status & DSBSTATUS_PLAYING;
	}
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	if(pThis->mFlags & WSAUDIO_INPUT)
		return SUCCEEDED(IDirectSoundCaptureBuffer_Stop(
			pThis->mCaptureBuffer));
	else
		return SUCCEEDED(IDirectSoundBuffer_Stop(pThis->mSoundBuffer));
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	if(pThis->mFlags & WSAUDIO_INPUT)
		return SUCCEEDED(IDirectSoundCaptureBuffer_Start(
			pThis->mCaptureBuffer, DSCBSTART_LOOPING));
	else
		return SUCCEEDED(IDirectSoundBuffer_Play(
			pThis->mSoundBuffer, 0, 0, DSBPLAY_LOOPING));
}
void WSAudio_Process(WSAUDIO* pThis);
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	if(!WSAudio_IsPlaying(pThis))
	{
		if(!(pThis->mFlags & WSAUDIO_INPUT))
			WSAudio_Process(pThis);
		return WSAudio_Resume(pThis);
	}
	return TRUE;
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	DWORD dwPos;
	if(pThis->mFlags & WSAUDIO_INPUT)
	{
		if(FAILED(IDirectSoundCaptureBuffer_GetCurrentPosition(
				pThis->mCaptureBuffer, NULL, &dwPos)))
			return 0;
	}
	else
	{
		if(FAILED(IDirectSoundBuffer_GetCurrentPosition(
				pThis->mSoundBuffer, NULL, &dwPos)))
			return 0;
	}
	return dwPos;
}
wsbool WSAudioHasFullDuplex() { return FALSE; }
/*#include <stdio.h>
#include <time.h>
time_t start = 0;
*/
void WSAudio_Process(WSAUDIO* pThis)
{
	/* we know it won't return anything greater than a DWORD */
	DWORD dspos = (DWORD)WSAudio_GetPosition(pThis);
	/*if(!start)
		start = time(NULL);*/
	/*
		The logic here is to only the part of the buffer DX
		is playing next.

		(dspos / pThis->mBufferSize) is the current chunk
		DX is playing within. (dspos / pThis->mBufferSize) + 1
		is the chunk afterwards we need to fill.

		In the case of capture we read from the last chunk
		(not the current or next chunk).
	*/
	if((dspos / pThis->mBufferSize) != pThis->mPrevChunk)
	{
		HRESULT hr;
		DWORD dwData = 0
			/*, dwData2 = 0*/;
		void* pData = NULL;
		/*void* pData2 = NULL;*/


		/*printf("DSPOS:[%zu] PREVCHUNK:[%zu] CURCHUNK:[%zu] "
			"NEXTCHUNK:[%zu] NEXTPOS:[%zu] TIME:[%zu]\n",
			dspos, pThis->mPrevChunk, (dspos / pThis->mBufferSize),
			nextchunk, nextpos, (size_t)time(NULL) - start);*/
		if(pThis->mFlags & WSAUDIO_INPUT)
		{
			if(SUCCEEDED(IDirectSoundCaptureBuffer_Lock(
				pThis->mCaptureBuffer,
				pThis->mPrevChunk * pThis->mBufferSize,
				pThis->mBufferSize, &pData, &dwData, NULL, 0, 0)))
			{
				(*pThis->mFillProc)(pThis->mUserData, (uint8_t*)pData, dwData);
				hr = IDirectSoundCaptureBuffer_Unlock(pThis->mCaptureBuffer,
					pData, dwData, NULL, 0);
			}
		}
		else
		{
			DWORD nextchunk = ((dspos / pThis->mBufferSize) + 1)
				% WSNUMDXCHUNKS;
			DWORD nextpos = nextchunk * pThis->mBufferSize;
			hr = IDirectSoundBuffer_Lock(pThis->mSoundBuffer, nextpos,
				pThis->mBufferSize, &pData, &dwData, NULL, 0, 0);
			if(hr == DSERR_BUFFERLOST)
			{
				IDirectSoundBuffer_Restore(pThis->mSoundBuffer);
				hr = IDirectSoundBuffer_Lock(pThis->mSoundBuffer, nextpos,
					pThis->mBufferSize, &pData, &dwData, NULL, 0, 0);
			}
			if(hr == DS_OK)
			{
				(*pThis->mFillProc)(pThis->mUserData, (uint8_t*)pData, dwData);
				hr = IDirectSoundBuffer_Unlock(pThis->mSoundBuffer,
					pData, dwData, NULL, 0);
			}
		}
		pThis->mPrevChunk = (dspos / pThis->mBufferSize);
	}
}

#if 0
	virtual bool SetFormat(const WSAudioStreamFormat& Format)
	{
		WAVEFORMATEX wf;
		WSAudioFormatToWaveFormatEx(Format, wf);

		if (m_dwFlags & DSBCAPS_PRIMARYBUFFER)
		{
			/* required on write primary (stop/play) */
			WSDSASSERT(m_pBuffer->Play(0, 0, DSBPLAY_LOOPING));
			WSDSCHECK(m_pBuffer->SetFormat(&wf), "Could Not Set Format");
			WSDSASSERT(m_pBuffer->Stop());
		}
		else /* !DSBCAPS_PRIMARYBUFFER */
		{
			m_pBuffer->Release();

			/* buffer info params */
			DSBUFFERDESC dbd;
			memset(&dbd, 0, sizeof(DSBUFFERDESC));
			dbd.dwSize = sizeof(DSBUFFERDESC);
			dbd.dwFlags = m_dwFlags;
			dbd.dwBufferBytes = m_Format.dwBufferSize;
			dbd.lpwfxFormat = &wf;

			WSDSCHECK(m_pDirectSound->CreateSoundBuffer(
				&dbd, &m_pBuffer, NULL),
				"Could not create a new directsound buffer");
		}
		m_Format = Format;
		return true;
	}
	virtual bool SetPlaybackRate(const WSAudioValueType& nPlaybackRate)
	{
		if (FAILED(m_pBuffer->SetFrequency(
			nPlaybackRate == 0 ?  DSBFREQUENCY_ORIGINAL :
		nPlaybackRate < 0 ? m_Format.dwSampleRate / ((-nPlaybackRate) / 10) :
		m_Format.dwSampleRate * (nPlaybackRate / 10)
													)
					)
			)
			return false;
		m_nPlaybackRate = nPlaybackRate;
		return true;
	}
	virtual off_t OnSysSeek(off_t dwPos, WSSeekMode mode)
	{
		switch (mode)
		{
			/* case WSFromStart */
		case WSFromCurrent:
			dwPos += TellO();
			break;
		case WSFromEnd:
			dwPos = m_Format.dwBufferSize - dwPos;
			break;
		}

		if (FAILED(m_pBuffer->SetCurrentPosition(dwPos)))
			return WSInvalidOffset;
		return m_currentPos = dwPos;
	}
#endif
#if defined(__cplusplus)
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

					#    #
					#   #  ###### #####  #    # ###### #
					#  #   #      #    # ##   # #      #
					###    #####  #    # # #  # #####  #
					#  #   #      #####  #  # # #      #
					#   #  #      #   #  #   ## #      #
					#    # ###### #    # #    # ###### ######

		  #####
		 #     # ##### #####  ######   ##   #    # # #    #  ####
		 #         #   #    # #       #  #  ##  ## # ##   # #    #
		  #####    #   #    # #####  #    # # ## # # # #  # #
			   #   #   #####  #      ###### #    # # #  # # #  ###
		 #     #   #   #   #  #      #    # #    # # #   ## #    #
		  #####    #   #    # ###### #    # #    # # #    #  ####

	Kernel Streaming (WDMKS) is Win98+. I tested some third party libraries
	like PortAudio and they consistantly failed at KsCreatePin() on
	Windows 10. I think StepMania was the only one that got it correct. This
	is not based on PortAudio/StepMania because I wanted to investigate it
	for myself (probably a mistake).

	Tehre is no real reason to use Kernel Streaming if WASAPI is available.
	Kernel Streaming also has some drawbacks as the volume of the device
	does not follow the volume the user sets in Windows, and will
	play at full blast unless you set it manually. Additionally,
	modern audio drivers that are not stack such as those from
	third party headsets may not support WDMKS.

	In addition, you often have to adjust your bits per sample
	and sample rate to that of the device as it is not guaranteed to
	support more than one sample rate/bits per sample.

	Kernel streaming is organized as such:

	DEVICES

	Devices can be input, output, or bidirectional. They are enumerated by
	the Setup API.

	PINS

	Devices have an arbitrary number of pins. A pin can be EITHER
	KSPIN_DATAFLOW_IN - rendering (audio output), or
	KSPIN_DATAFLOW_OUT - capture (audio input). WSAUDIO devices
	are pins. Pins are enumerated via DeviceIoControl() with the
	KSPROPERTY_PIN_CTYPES property passed in.

	NODES

	Pins have an arbitrary of nodes. Nodes control features of pins. In
	this case we use the volume pin to get the volume ranges and set
	the volume of the pin/device. Nodes are enumerated via
	DeviceIoControl() with the KSPROPERTY_TOPOLOGY_NODES property.
===========================================================================*/
#elif defined(_WIN32) && !defined(WSAUDIOWASOPT) \
	&& !defined(WSAUDIOXAUDIOOPT) && !defined(WSAUDIOSMOPT)
#include <windows.h>
#include <winioctl.h>
#include <ks.h>
/*#include <ksmedia.h>*/
#include <SetupAPI.h>

/* Framing structs for MSVC6 */
#define WSKSPROPERTY_CONNECTION_ALLOCATORFRAMING 3
#define WSKSPROPERTY_CONNECTION_ALLOCATORFRAMING_EX 6
typedef struct WSKSALLOCATOR_FRAMING
{
	union
	{
		ULONG       OptionsFlags;
		ULONG       RequirementsFlags;
	};
#if defined(_NTDDK_)
	POOL_TYPE   PoolType;
#else  /* User SDK */
	ULONG       PoolType;
#endif /* _NTDDK_ */
	ULONG       Frames;
	ULONG       FrameSize;
	union
	{
		ULONG       FileAlignment;
		LONG        FramePitch;
	};
	ULONG       Reserved;
} WSKSALLOCATOR_FRAMING, *PWSKSALLOCATOR_FRAMING;
typedef struct WSKS_FRAMING_RANGE
{
	ULONG   MinFrameSize;
	ULONG   MaxFrameSize;
	ULONG   Stepping;
} WSKS_FRAMING_RANGE, *PWSKS_FRAMING_RANGE;
typedef struct WSKS_FRAMING_RANGE_WEIGHTED
{
	WSKS_FRAMING_RANGE  Range;
	ULONG             InPlaceWeight;
	ULONG             NotInPlaceWeight;
} WSKS_FRAMING_RANGE_WEIGHTED, *PWSKS_FRAMING_RANGE_WEIGHTED;
typedef struct WSKS_COMPRESSION
{
	ULONG   RatioNumerator;
	ULONG   RatioDenominator;
	ULONG   RatioConstantMargin;
} WSKS_COMPRESSION, *PWSKS_COMPRESSION;
typedef struct WSKS_FRAMING_ITEM
{
	GUID                        MemoryType;
	GUID                        BusType;
	ULONG                       MemoryFlags;
	ULONG                       BusFlags;
	ULONG                       Flags;
	ULONG                       Frames;
	union
	{
		ULONG                   FileAlignment;
		LONG                    FramePitch;
	};
	ULONG                       MemoryTypeWeight;
	WSKS_FRAMING_RANGE            PhysicalRange;
	WSKS_FRAMING_RANGE_WEIGHTED   FramingRange;
} WSKS_FRAMING_ITEM, *PWSKS_FRAMING_ITEM;
typedef struct WSKSALLOCATOR_FRAMING_EX
{
	ULONG               CountItems;
	ULONG               PinFlags;
	WSKS_COMPRESSION      OutputCompression;
	ULONG               PinWeight;
	WSKS_FRAMING_ITEM     FramingItem[1];
} WSKSALLOCATOR_FRAMING_EX, *PWSKSALLOCATOR_FRAMING_EX;

/* MSVC6 ksmedia.h */
#if !defined(KSINTERFACESETID_Media)
	typedef struct KSAUDIO_POSITION
	{
	#if defined(_NTDDK_)
		ULONGLONG	PlayOffset;
		ULONGLONG	WriteOffset;
	#else
		DWORDLONG	PlayOffset;
		DWORDLONG	WriteOffset;
	#endif
	} KSAUDIO_POSITION, *PKSAUDIO_POSITION;
	typedef struct KSDATARANGE_AUDIO
	{
		KSDATARANGE	DataRange;
		ULONG		MaximumChannels;
		ULONG		MinimumBitsPerSample;
		ULONG		MaximumBitsPerSample;
		ULONG		MinimumSampleFrequency;
		ULONG		MaximumSampleFrequency;
	} KSDATARANGE_AUDIO, *PKSDATARANGE_AUDIO;
	typedef struct KSNODEPROPERTY
	{
		KSPROPERTY	Property;
		ULONG		NodeId;
		ULONG		Reserved;
	} KSNODEPROPERTY, *PKSNODEPROPERTY;
	typedef struct KSNODEPROPERTY_AUDIO_CHANNEL
	{
		KSNODEPROPERTY	NodeProperty;
		LONG			Channel;
		ULONG			Reserved;
	} KSNODEPROPERTY_AUDIO_CHANNEL, *PKSNODEPROPERTY_AUDIO_CHANNEL;
	#define KSPROPERTY_AUDIO_VOLUMELEVEL 4
	#define KSPROPERTY_AUDIO_POSITION 5
	/* struct align of 1 */
	#if ! (defined(lint) || defined(RC_INVOKED))
		#if ( _MSC_VER >= 800 && !defined(_M_I86)) \
				|| defined(_PUSHPOP_SUPPORTED)
			#pragma warning(disable:4103)
			#if !(defined( MIDL_PASS )) || defined( __midl )
				#pragma pack(push,1)
			#else
				#pragma pack(1)
			#endif
		#else
			#pragma pack(1)
		#endif
	#endif /* !(defined(lint) || defined(RC_INVOKED)) */
	typedef struct KSDATAFORMAT_WAVEFORMATEX
	{
		KSDATAFORMAT    DataFormat;
		WAVEFORMATEX    WaveFormatEx;
	} KSDATAFORMAT_WAVEFORMATEX, *PKSDATAFORMAT_WAVEFORMATEX;
	#if ! (defined(lint) || defined(RC_INVOKED))
		#if ( _MSC_VER >= 800 && !defined(_M_I86)) \
				|| defined(_PUSHPOP_SUPPORTED)
			#pragma warning(disable:4103)
			#if !(defined( MIDL_PASS )) || defined( __midl )
				#pragma pack(pop)
			#else
				#pragma pack()
			#endif
		#else
			#pragma pack()
		#endif
	#endif /* !(defined(lint) || defined(RC_INVOKED)) */
#endif /* !defined(KSINTERFACESETID_Media) */
/* for KSDATAFORMAT_SUBTYPE_PCM et al. if needed */
/*#pragma comment (lib, "ksuser.lib")
#pragma comment (lib, "setupapi.lib")*/

#ifndef DWORD_PTR
	#define DWORD_PTR size_t
#endif

/* KSDATAFORMAT_SPECIFIER_WILDCARD is defined as
	{0x0F6417D6L, 0xC318, 0x11D0,
		{0xA4, 0x3F, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96}}
	in the VC++ 6 SDK instead of GUID_NULL */
#define WSKSDATAFORMAT_SPECIFIER_WILDCARD WSGUID_NULL
/* Not really needed just for consistancy */
#define WSKSDATAFORMAT_TYPE_WILDCARD WSGUID_NULL
#define WSKSDATAFORMAT_SUBTYPE_WILDCARD WSGUID_NULL

typedef DWORD (WINAPI* LPKsCreatePin)(
	HANDLE FilterHandle, PKSPIN_CONNECT Connect,
	ACCESS_MASK DesiredAccess, PHANDLE ConnectionHandle);
typedef BOOL (WINAPI* LPSetupDiGetDeviceInterfaceAlias)(
	HDEVINFO DeviceInfoSet,
	PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData,
	CONST GUID* AliasInterfaceClassGuid,
	PSP_DEVICE_INTERFACE_DATA  AliasDeviceInterfaceData);
typedef BOOL (WINAPI* LPSetupDiGetDeviceInterfaceDetail)(
	HDEVINFO DeviceInfoSet,
	PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData,
	PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData,
	DWORD DeviceInterfaceDetailDataSize, PDWORD RequiredSize,
	PSP_DEVINFO_DATA DeviceInfoData);
typedef BOOL (WINAPI* LPSetupDiEnumDeviceInterfaces)(
	HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData,
	CONST GUID* InterfaceClassGuid, DWORD MemberIndex,
	PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData);
typedef BOOL (WINAPI* LPSetupDiGetDeviceRegistryProperty)(
	HDEVINFO DeviceInfoSet, PSP_DEVINFO_DATA DeviceInfoData,
	DWORD Property, PDWORD PropertyRegDataType,
	PBYTE PropertyBuffer, DWORD PropertyBufferSize, PDWORD RequiredSize);
typedef BOOL (WINAPI* LPSetupDiDestroyDeviceInfoList)(
	HDEVINFO DeviceInfoSet);
typedef HDEVINFO (WINAPI* LPSetupDiGetClassDevsA)(
	CONST GUID* ClassGuid, PCSTR Enumerator, HWND hwndParent,
	DWORD Flags);

typedef struct WSKSSTREAM_HEADER
{
	ULONG    Size;
	ULONG    TypeSpecificFlags;
	KSTIME   PresentationTime;
	LONGLONG Duration;
	ULONG    FrameExtent;
	ULONG    DataUsed;
	PVOID    Data;
	ULONG    OptionsFlags;
#if _WIN64
	ULONG    Reserved;
#endif
} WSKSSTREAM_HEADER;

/*===========================================================================
					 KS HELPERS
===========================================================================*/
#if defined(__cplusplus)
extern "C" {
#endif
	void WSAudioFormatToWaveFormatEx(
		const WSAUDIOFORMAT* Format, WAVEFORMATEX* wfx)
	{
		wfx->cbSize = 0;
		wfx->wFormatTag = WAVE_FORMAT_PCM;
		wfx->wBitsPerSample = Format->bitsPerSample;
		wfx->nSamplesPerSec = Format->sampleRate;
		wfx->nChannels = Format->channels;
		wfx->nBlockAlign = WSAudioFormat_GetBlockAlign(Format);
		wfx->nAvgBytesPerSec = Format->sampleRate * wfx->nBlockAlign;
	}

	void WSWaveFormatExToAudioFormat(
		const WAVEFORMATEX* wfx, WSAUDIOFORMAT* Format)
	{
		Format->bitsPerSample = wfx->wBitsPerSample;
		Format->sampleRate = wfx->nSamplesPerSec;
		Format->channels = wfx->nChannels;
	}

#define IsGUIDSame(guid1, guid2) \
	(WSMemoryCompare(guid1, guid2, sizeof(GUID)) == 0)

HRESULT SyncIoctl(HANDLE handle, ULONG ulIoctl,
	PVOID pvInBuffer, ULONG cbInBuffer, PVOID pvOutBuffer, ULONG cbOutBuffer,
	PULONG pulBytesReturned)
{
	HRESULT hr = S_OK;
	OVERLAPPED overlapped;
	BOOL fRes = TRUE;
	ULONG ulBytesReturned;

	WSASSERT(handle != NULL && handle != INVALID_HANDLE_VALUE);

	if (!pulBytesReturned)
		pulBytesReturned = &ulBytesReturned;

	ZeroMemory(&overlapped, sizeof(overlapped));
	overlapped.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	/*
		 Flag the event by setting the low-order bit so we
		 don't get completion port notifications.
		 Really! - see the description of the lpOverlapped parameter in
		 the docs for GetQueuedCompletionStatus
	 */
	overlapped.hEvent = (HANDLE)((DWORD_PTR)overlapped.hEvent | 0x1);

	if (SUCCEEDED(hr))
	{
		fRes = DeviceIoControl(handle, ulIoctl, pvInBuffer, cbInBuffer,
			pvOutBuffer, cbOutBuffer, pulBytesReturned, &overlapped);
		if (!fRes)
		{

			DWORD dwError;
			dwError = GetLastError();
			if (ERROR_IO_PENDING == dwError)
			{
				DWORD dwWait;
				/* Wait for completion */
				dwWait = WaitForSingleObject(overlapped.hEvent, INFINITE);
				WSASSERT(WAIT_OBJECT_0 == dwWait);
				WSRELEASEUNUSED(dwWait);
			}
			else if (((ERROR_INSUFFICIENT_BUFFER == dwError)
						|| (ERROR_MORE_DATA == dwError)) &&
				(IOCTL_KS_PROPERTY == ulIoctl) &&
				(cbOutBuffer == 0))
			{
				hr = S_OK;
				fRes = TRUE;
			}
			else
				hr = E_FAIL;
		}
		if (!fRes) *pulBytesReturned = 0;
		CloseHandle(overlapped.hEvent);
	}

	return hr;
}

ULONG WSAudioEnumDeviceFormats(
	const char* szDevice, WSAUDIOFORMAT* format)
{
	ULONG iPin;
	ULONG nNumPins;
	HANDLE hDevice;
	KSP_PIN PropPin;
	ULONG uRet;
	/* These GUIDs normally require pulling in ksuser.lib */
	GUID WSKSDATAFORMAT_SUBTYPE_PCM = {0x00000001, 0x0000, 0x0010,
		{0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71} };
	/* 0x00000000 == wFormatTag of WAVEFORMATEX */
	/*GUID WSKSDATAFORMAT_SUBTYPE_WAVEFORMATEX = { 0x00000000, 0x0000, 0x0010,
		{0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71} };*/
	GUID WSKSINTERFACESETID_Standard = { 0x1A8766A0, 0x62CE, 0x11CF,
		{0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00} };
	GUID WSKSMEDIUMSETID_Standard = { 0x4747B320, 0x62CE, 0x11CF,
		{0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00} };
	GUID WSKSPROPSETID_Pin = { 0x8C134960, 0x51AD, 0x11CF,
		{0x87, 0x8A, 0x94, 0xF8, 0x01, 0xC1, 0x00, 0x00} };
	GUID WSKSDATAFORMAT_TYPE_AUDIO = { 0x73647561, 0x0000, 0x0010,
		{0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71} };
	GUID WSKSDATAFORMAT_SPECIFIER_WAVEFORMATEX = { 0x05589F81, 0xC356, 0x11CE,
		{0xBF, 0x01, 0x00, 0xAA, 0x00, 0x55, 0x59, 0x5A} };
	GUID WSKSDATAFORMAT_SUBTYPE_IEEE_FLOAT = { 0x00000003, 0x0000, 0x0010,
		{0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71} };
	GUID WSGUID_NULL = { 0x00000000, 0x0000, 0x0000,
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };

	hDevice = CreateFileA(szDevice, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if(hDevice == NULL)
		return FALSE;

	uRet = 0;
	PropPin.Property.Set = WSKSPROPSETID_Pin;
	PropPin.Property.Flags = KSPROPERTY_TYPE_GET;
	PropPin.Reserved = 0;
	PropPin.PinId = 0;
	PropPin.Property.Id = KSPROPERTY_PIN_CTYPES;

	if(FAILED(SyncIoctl((void*)hDevice, IOCTL_KS_PROPERTY, &PropPin,
		sizeof(PropPin), &nNumPins, sizeof(nNumPins), &uRet)))
		return FALSE;

	/* go through the pins */
	for (iPin = 0; iPin < nNumPins; ++iPin)
	{
		KSPIN_DATAFLOW PinData;
		PKSMULTIPLE_ITEM pDataRangeStuff;
		PKSMULTIPLE_ITEM pMediumStuff;
		PKSMULTIPLE_ITEM pInterfaceStuff;
		PKSPIN_INTERFACE pInterfaces;
		PKSDATARANGE pDataRange;
		ULONG iInterface;
		ULONG iDataRange;
		PKSPIN_MEDIUM pMediums;
		KSPIN_COMMUNICATION PinComm = KSPIN_COMMUNICATION_NONE;
		PropPin.PinId = iPin;

		PropPin.Property.Id = KSPROPERTY_PIN_COMMUNICATION;
		if(FAILED(SyncIoctl((void*)hDevice, IOCTL_KS_PROPERTY, &PropPin,
			sizeof(PropPin), &PinComm, sizeof(PinComm), &uRet)))
			continue; /* some error, next pin */

		/* Valid communication? */
		if (!(
			/*(PinComm == KSPIN_COMMUNICATION_SOURCE) ||*/
			(PinComm == KSPIN_COMMUNICATION_SINK) ||
			(PinComm == KSPIN_COMMUNICATION_BOTH)
			))
			continue; /* nope :( */

		PinData = (KSPIN_DATAFLOW)0;

		PropPin.Property.Id = KSPROPERTY_PIN_DATAFLOW;
		if(FAILED(SyncIoctl((void*)hDevice, IOCTL_KS_PROPERTY, &PropPin,
			sizeof(PropPin), &PinData, sizeof(PinData), &uRet)))
			continue; /* need the dataflow and didn't get it */

		if (((KSPIN_DATAFLOW_IN == PinData)
				/*|| (KSPIN_DATAFLOW_OUT == PinData)*/))
		{
			ULONG uSize;
			PropPin.Property.Id = KSPROPERTY_PIN_INTERFACES;
			if(FAILED(SyncIoctl((void*)hDevice, IOCTL_KS_PROPERTY, &PropPin,
				sizeof(PropPin), NULL, 0, &uRet)))
				continue; /* couldn't get pin interfaces */

			uSize = uRet;
			pInterfaceStuff =
				(PKSMULTIPLE_ITEM) WSALLOC(BYTE, uSize);
			if(FAILED(SyncIoctl((void*)hDevice, IOCTL_KS_PROPERTY, &PropPin,
				sizeof(PropPin), pInterfaceStuff, uSize, &uRet)))
			{
				WSFREE((BYTE*)pInterfaceStuff);
				continue;
			}
			WSASSERT(uSize == uRet);

			pInterfaces =
				(PKSPIN_INTERFACE)(pInterfaceStuff + 1);
			/* interface & medium standard streaming? */
			for (iInterface = 0; iInterface < pInterfaceStuff->Count; ++iInterface)
			{
				if (IsGUIDSame(&pInterfaces[iInterface].Set, &WSKSINTERFACESETID_Standard) &&
					(pInterfaces[iInterface].Id ==
							KSINTERFACE_STANDARD_STREAMING
						/* We don't do looped streaming (like DirectSound etc.) */
						/*|| pInterfaces[iInterface].Id ==
								KSINTERFACE_STANDARD_LOOPED_STREAMING*/))
					break;
			}
			if(iInterface == pInterfaceStuff->Count)
			{
				WSFREE((BYTE*)pInterfaceStuff);
				continue;
			}

			PropPin.Property.Id = KSPROPERTY_PIN_MEDIUMS;
			if(FAILED(SyncIoctl((void*)hDevice, IOCTL_KS_PROPERTY, &PropPin,
				sizeof(PropPin), NULL, 0, &uRet)))
			{
				WSFREE((BYTE*)pInterfaceStuff);
				continue;
			}

			uSize = uRet;
			pMediumStuff = (PKSMULTIPLE_ITEM) WSALLOC(BYTE, uSize);
			if(FAILED(SyncIoctl((void*)hDevice, IOCTL_KS_PROPERTY, &PropPin,
				sizeof(PropPin), pMediumStuff, uSize, &uRet)))
			{
				WSFREE((BYTE*)pMediumStuff);
				WSFREE((BYTE*)pInterfaceStuff);
				continue;
			}
			WSASSERT(uSize == uRet);
			/*pMediumStuff->Count ? */
			pMediums = (PKSPIN_MEDIUM)(pMediumStuff + 1);
			for (iInterface = 0; iInterface < pInterfaceStuff->Count; ++iInterface)
			{
				if (IsGUIDSame(&pMediums[iInterface].Set, &WSKSMEDIUMSETID_Standard) &&
					pMediums[iInterface].Id == KSMEDIUM_STANDARD_DEVIO)
					break;
			}
			WSASSERT(iInterface != (pInterfaceStuff->Count));

			/* datarange support audio? */
			PropPin.Property.Id = KSPROPERTY_PIN_DATARANGES;
			if(FAILED(SyncIoctl((void*)hDevice, IOCTL_KS_PROPERTY, &PropPin,
				sizeof(PropPin), NULL, 0, &uRet)))
			{
				WSFREE((BYTE*)pMediumStuff);
				WSFREE((BYTE*)pInterfaceStuff);
				continue;
			}
			uSize = uRet;
			pDataRangeStuff = (PKSMULTIPLE_ITEM) WSALLOC(BYTE, uSize);
			if(FAILED(SyncIoctl((void*)hDevice, IOCTL_KS_PROPERTY, &PropPin,
				sizeof(PropPin), pDataRangeStuff, uSize, &uRet)))
			{
				WSFREE((BYTE*)pMediumStuff);
				WSFREE((BYTE*)pInterfaceStuff);
				continue;
			}
			WSASSERT(uSize == uRet);

			pDataRange = (PKSDATARANGE)(pDataRangeStuff + 1);
			WSASSERT(pDataRange->FormatSize);

			for (iDataRange = 0; iDataRange < pDataRangeStuff->Count; iDataRange++)
			{
				if(IsGUIDSame(&pDataRange->MajorFormat, &WSKSDATAFORMAT_TYPE_AUDIO)
					|| IsGUIDSame(&pDataRange->MajorFormat, &WSKSDATAFORMAT_TYPE_WILDCARD))
				{
					if (IsGUIDSame(&pDataRange->SubFormat, &WSKSDATAFORMAT_SUBTYPE_PCM)
							|| IsGUIDSame(&pDataRange->SubFormat, &WSKSDATAFORMAT_SUBTYPE_IEEE_FLOAT)
							|| IsGUIDSame(&pDataRange->SubFormat, &WSKSDATAFORMAT_SUBTYPE_WILDCARD))
					{
						if (IsGUIDSame(&pDataRange->Specifier, &WSKSDATAFORMAT_SPECIFIER_WILDCARD)
							|| IsGUIDSame(&pDataRange->Specifier, &WSKSDATAFORMAT_SPECIFIER_WAVEFORMATEX))
						{
							PKSDATARANGE_AUDIO pDataRangeAudio =
								(PKSDATARANGE_AUDIO)pDataRange;
							/* FINALLY!  A valid pin - fill out

								MinimumBitsPerSample/MinimumSampleFrequency
								are largely useless as they are always
								the same as the maximum in practice.
							*/
							/*WSAudioStreamFormat MinFormat =
								WSAudioStreamFormat(1,
									pDataRangeAudio->MinimumBitsPerSample,
									pDataRangeAudio->MinimumSampleFrequency);*/
							format->sampleRate =
								pDataRangeAudio->MaximumSampleFrequency;
							format->channels = (uint16_t)
								pDataRangeAudio->MaximumChannels;
							format->bitsPerSample = (uint16_t)
								pDataRangeAudio->MaximumBitsPerSample;
							CloseHandle(hDevice);
							WSFREE((BYTE*)pInterfaceStuff);
							WSFREE((BYTE*)pMediumStuff);
							WSFREE((BYTE*)pDataRangeStuff);
							return iPin;
						}
					}
				}
				pDataRange = (PKSDATARANGE)
					(((PBYTE)pDataRange) + pDataRange->FormatSize);
			}/* end datarange loop */

			WSFREE((BYTE*)pInterfaceStuff);
			WSFREE((BYTE*)pMediumStuff);
			WSFREE((BYTE*)pDataRangeStuff);
		}/* end pin == meet outer condition */
	}/* end pin loop */
	CloseHandle(hDevice);
	return (ULONG)-1;
}

/*===========================================================================
					 KS IMPLEMENTATION
===========================================================================*/

HRESULT WSAudio_State_(WSAUDIO* pThis, KSSTATE* pState, ULONG flags)
{
	ULONG ulReturn;
	HRESULT hr;
	GUID WSKSPROPSETID_Connection = { 0x1D58C920, 0xAC9B, 0x11CF,
		{0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00} };
	KSP_PIN PropPin;
	PropPin.Property.Set = WSKSPROPSETID_Connection;
	PropPin.Property.Flags = flags;
	PropPin.Reserved = 0;
	PropPin.PinId = 0;
	PropPin.Property.Id = KSPROPERTY_CONNECTION_STATE;
	hr = SyncIoctl(pThis->mhPin, IOCTL_KS_PROPERTY, &PropPin,
		sizeof(PropPin), pState, sizeof(KSSTATE), &ulReturn);
	return hr;
}
HRESULT WSAudio_SetState_(WSAUDIO* pThis, KSSTATE state)
{
	return WSAudio_State_(pThis, &state, KSPROPERTY_TYPE_SET);
}
/*#include <stdio.h>*/

void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	HMODULE m_hSetupDll;
	LPSetupDiGetDeviceInterfaceAlias pSetupDiGetDeviceInterfaceAlias;
	LPSetupDiGetDeviceInterfaceDetail pSetupDiGetDeviceInterfaceDetail;
	LPSetupDiEnumDeviceInterfaces pSetupDiEnumDeviceInterfaces;
	LPSetupDiDestroyDeviceInfoList pSetupDiDestroyDeviceInfoList;
	LPSetupDiGetClassDevsA pSetupDiGetClassDevs;
	LPSetupDiGetDeviceRegistryProperty pSetupDiGetDeviceRegistryProperty;
	HDEVINFO hDevInfo;
	int iDevice;
	SP_DEVICE_INTERFACE_DETAIL_DATA* pDevInfo;
	SP_DEVICE_INTERFACE_DATA            DID;
	SP_DEVICE_INTERFACE_DATA            DIDAlias;
	SP_DEVINFO_DATA                     DevInfoData;
	GUID WSKSCATEGORY_AUDIO = { 0x6994AD04L, 0x93EF, 0x11D0,
		{0xA3, 0xCC, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96} };
	GUID WSKSCATEGORY_CAPTURE = { 0x65E8773DL, 0x8F56, 0x11D0,
		{0xA3, 0xB9, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96} };
	GUID WSKSCATEGORY_RENDER = { 0x65E8773EL, 0x8F56, 0x11D0,
		{0xA3, 0xB9, 0x00, 0xA0, 0xC9, 0x22, 0x31, 0x96} };
	m_hSetupDll = LoadLibraryA("SETUPAPI.DLL");
	if(m_hSetupDll == NULL)
		return;
	pSetupDiGetDeviceInterfaceAlias = (LPSetupDiGetDeviceInterfaceAlias)
		GetProcAddress(m_hSetupDll, "SetupDiGetDeviceInterfaceAlias");
	pSetupDiGetDeviceInterfaceDetail = (LPSetupDiGetDeviceInterfaceDetail)
		GetProcAddress(m_hSetupDll, "SetupDiGetDeviceInterfaceDetailA");
	pSetupDiEnumDeviceInterfaces = (LPSetupDiEnumDeviceInterfaces)
		GetProcAddress(m_hSetupDll, "SetupDiEnumDeviceInterfaces");
	pSetupDiDestroyDeviceInfoList = (LPSetupDiDestroyDeviceInfoList)
		GetProcAddress(m_hSetupDll, "SetupDiDestroyDeviceInfoList");
	pSetupDiGetClassDevs = (LPSetupDiGetClassDevsA)
		GetProcAddress(m_hSetupDll, "SetupDiGetClassDevsA");
	pSetupDiGetDeviceRegistryProperty = (LPSetupDiGetDeviceRegistryProperty)
		GetProcAddress(m_hSetupDll, "SetupDiGetDeviceRegistryPropertyA");

	enumparams->devices = NULL;
	enumparams->numdevices = 0;
	hDevInfo = (*pSetupDiGetClassDevs)(&WSKSCATEGORY_AUDIO, NULL, NULL,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	if(hDevInfo == INVALID_HANDLE_VALUE)
	{
		FreeLibrary(m_hSetupDll);
		return;
	}

	for (iDevice = 0; ; ++iDevice)
	{
		int nDevInfoSize;
		int devtype;
		WSAUDIODEVICE* newdevices;
		DWORD DataT;
		CHAR* szDeviceName;
		DWORD RequiredSize;
		DID.cbSize = sizeof(DID);
		DID.Reserved = 0;
		DIDAlias.cbSize = sizeof(DIDAlias);
		DIDAlias.Reserved = 0;
		DevInfoData.cbSize = sizeof(DevInfoData);
		DevInfoData.Reserved = 0;
		/* Enumerate the device index */
		if ((*pSetupDiEnumDeviceInterfaces)(hDevInfo, NULL, &WSKSCATEGORY_AUDIO,
			iDevice, &DID) == 0)
			break; /* done */

		/* Obtain the device path */
		nDevInfoSize =
			sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + MAX_PATH * sizeof(WCHAR);
		pDevInfo =
			(SP_DEVICE_INTERFACE_DETAIL_DATA*) WSALLOC(BYTE, nDevInfoSize);
		pDevInfo->cbSize = sizeof(*pDevInfo);
		if((*pSetupDiGetDeviceInterfaceDetail)(hDevInfo, &DID, pDevInfo,
			nDevInfoSize, NULL, &DevInfoData) == 0)
		{
			WSFREE((BYTE*)pDevInfo);
			continue; /* error */
		}

		/* Obtain the device audio direction(s) */
		devtype =
			((*pSetupDiGetDeviceInterfaceAlias)(hDevInfo, &DID, &WSKSCATEGORY_CAPTURE,
				&DIDAlias) * WSAUDIO_INPUT);
		devtype +=
			((*pSetupDiGetDeviceInterfaceAlias)(hDevInfo, &DID, &WSKSCATEGORY_RENDER,
					&DIDAlias) * WSAUDIO_OUTPUT);
		/*printf("%s\n", pDevInfo->DevicePath);*/

		if(!devtype)
		{ /* could be topology device which isn't useful to us */
			WSFREE((BYTE*)pDevInfo);
			continue;
		}

		/* realloc device array */
		newdevices = WSREALLOC(enumparams->devices,
			WSAUDIODEVICE, enumparams->numdevices + 1);
		if(!newdevices)
		{
			WSFREE((BYTE*)pDevInfo);
			break; /* alloc error */
		}
		enumparams->devices = newdevices;

		/*
			get the device name

			Investigate why everyone else uses SetupDiOpenDeviceInterfaceRegKey
			instead:
			HKEY hKey = (*pSetupDiOpenDeviceInterfaceRegKey)(
				hDevInfo, &DevInfoData, 0, KEY_QUERY_VALUE);
			if(hKey != INVALID_HANDLE_VALUE)
			{
				char szFriendlyName[MAX_PATH] = "[none]";
				DWORD sizeFriendlyName = sizeof(szFriendlyName);
				DWORD type;
				if(RegQueryValueEx(hKey, "FriendlyName", 0, &type,
						(BYTE*)szFriendlyName, &sizeFriendlyName) != ERROR_SUCCESS)
					WSMemoryCopy(szFriendlyName, "[none]", countof("[none]"));
				RegCloseKey(hKey);
			}
		*/
		szDeviceName = WSALLOC(CHAR, MAX_PATH);
		if ((*pSetupDiGetDeviceRegistryProperty)(
				hDevInfo, &DevInfoData, SPDRP_FRIENDLYNAME,
				&DataT, (PBYTE)szDeviceName, MAX_PATH,
				&RequiredSize))
		{
			enumparams->devices[enumparams->numdevices].name = szDeviceName;
		}
		else if ((*pSetupDiGetDeviceRegistryProperty)(
				hDevInfo, &DevInfoData, SPDRP_DEVICEDESC,
				&DataT, (PBYTE)szDeviceName, MAX_PATH,
				&RequiredSize))
		{
			enumparams->devices[enumparams->numdevices].name = szDeviceName;
		}
		else
		{
			WSFREE(szDeviceName);
			enumparams->devices[enumparams->numdevices].name = NULL;
		}

		/* enumerate the pins of the audio details */
		if ((DIDAlias.Flags && !(DIDAlias.Flags & SPINT_REMOVED)))
		{
			ULONG iPin = WSAudioEnumDeviceFormats(pDevInfo->DevicePath,
				&enumparams->devices[enumparams->numdevices].maxformat);
			if(iPin != (ULONG)-1)
			{ /* consider the device valid */
				enumparams->devices[enumparams->numdevices].flags
					= (uint8_t)devtype;
				enumparams->devices[enumparams->numdevices].id
					= iPin;
				WSFREE(enumparams->devices[enumparams->numdevices].name);
				enumparams->devices[enumparams->numdevices].name
					= StringDup(pDevInfo->DevicePath);
				++enumparams->numdevices;
			}
		}

		WSFREE((BYTE*)pDevInfo);
	}
	(*pSetupDiDestroyDeviceInfoList)(hDevInfo);
	FreeLibrary(m_hSetupDll);
}

wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	LONG err;
	KSP_PIN PropPin;
	PKSMULTIPLE_ITEM nodeTypes;
	GUID* nodes;
	ULONG nodeId;
	DWORD dwBytesReturned;
	PKSDATAFORMAT_WAVEFORMATEX pdfwfx;
	PKSPIN_CONNECT pPinConnect;
	HANDLE hDevice, hPin;
	HMODULE hKSUserDll;
	LPKsCreatePin pKsCreatePin;
	WSAUDIOFORMAT* audioformat = &spec->format.mSuper;
	uint8_t* buffer;
	ULONG bufferSize;
	DWORD dwOpenMode;
	WSKSALLOCATOR_FRAMING ksalframing;
	WSKSALLOCATOR_FRAMING_EX ksalframingex;
	GUID WSKSDATAFORMAT_SUBTYPE_PCM = {0x00000001, 0x0000, 0x0010,
		{0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71} };
	GUID WSKSDATAFORMAT_SPECIFIER_WAVEFORMATEX = {0x05589F81, 0xC356, 0x11CE,
		{0xBF, 0x01, 0x00, 0xAA, 0x00, 0x55, 0x59, 0x5A} };
	GUID WSKSINTERFACESETID_Standard = {0x1A8766A0, 0x62CE, 0x11CF,
		{0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00} };
	GUID WSKSMEDIUMSETID_Standard = {0x4747B320, 0x62CE, 0x11CF,
		{0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00} };
	GUID WSKSDATAFORMAT_TYPE_AUDIO = {0x73647561, 0x0000, 0x0010,
		{0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71} };
	GUID WSKSPROPSETID_Topology = {0x720D4AC0L, 0x7533, 0x11D0,
		{0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00}};
	GUID WSKSNODETYPE_VOLUME = {0x3A5ACC00L, 0xC557, 0x11D0,
		{0x8A, 0x2B, 0x00, 0xA0, 0xC9, 0x25, 0x5A, 0xC1}};
	/*GUID WSKSPROPSETID_Pin = { 0x8C134960, 0x51AD, 0x11CF,
		{0x87, 0x8A, 0x94, 0xF8, 0x01, 0xC1, 0x00, 0x00} };*/
	GUID WSKSPROPSETID_Connection = { 0x1D58C920, 0xAC9B, 0x11CF,
		{0xA5, 0xD6, 0x28, 0xDB, 0x04, 0xC1, 0x00, 0x00} };
	WSASSERT(!(spec->flags & WSAUDIO_INPUT));
	if(!(spec->flags & WSAUDIO_CREATEFROMDEVICE))
		return FALSE;
	hKSUserDll = LoadLibraryA("ksuser.dll");
	if (hKSUserDll == NULL)
		return FALSE;
	/*if(spec->flags & WSAUDIO_INPUT)
	{
		if(spec->flags & WSAUDIO_OUTPUT)
			dwOpenMode = GENERIC_READ | GENERIC_WRITE;
		else
			dwOpenMode = GENERIC_READ;
	}
	else
		dwOpenMode = GENERIC_WRITE;*/
	dwOpenMode = GENERIC_READ | GENERIC_WRITE;
	hDevice =
		CreateFileA(spec->device->name, dwOpenMode,
			0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		FreeLibrary(hKSUserDll);
		return FALSE;
	}

	pPinConnect = (PKSPIN_CONNECT)
		WSALLOC(BYTE, sizeof(KSPIN_CONNECT)
						+ sizeof(KSDATAFORMAT_WAVEFORMATEX));
	/*(PKSPIN_CONNECT)HeapAlloc(GetProcessHeap(), 0,
		sizeof(KSPIN_CONNECT) + sizeof(KSDATAFORMAT_WAVEFORMATEX));
	*/
	/*
		KSINTERFACE_STANDARD_STREAMING
		KSINTERFACE_STANDARD_LOOPED_STREAMING
	*/
	pPinConnect->PinId = (ULONG)spec->device->id;
	pPinConnect->Interface.Set = WSKSINTERFACESETID_Standard;
	pPinConnect->Interface.Id = KSINTERFACE_STANDARD_STREAMING;
	pPinConnect->Interface.Flags = 0;
	pPinConnect->Medium.Set = WSKSMEDIUMSETID_Standard;
	pPinConnect->Medium.Id = KSMEDIUM_TYPE_ANYINSTANCE;
	pPinConnect->Medium.Flags = 0;
	pPinConnect->PinToHandle = NULL;
	pPinConnect->Priority.PriorityClass = KSPRIORITY_NORMAL;
	pPinConnect->Priority.PrioritySubClass = 1;

	/* fill out data format */
	pdfwfx = (PKSDATAFORMAT_WAVEFORMATEX)(pPinConnect + 1);
	WSAudioFormatToWaveFormatEx(audioformat, &pdfwfx->WaveFormatEx);
	pdfwfx->DataFormat.FormatSize = /*sizeof(KSDATAFORMAT_WAVEFORMATEX);*/
		sizeof(KSDATAFORMAT) + sizeof(WAVEFORMATEX);
	pdfwfx->DataFormat.Flags = 0;/*KSDATAFORMAT_ATTRIBUTES;*/
	pdfwfx->DataFormat.Reserved = 0;
	pdfwfx->DataFormat.MajorFormat = WSKSDATAFORMAT_TYPE_AUDIO;
	pdfwfx->DataFormat.SubFormat = WSKSDATAFORMAT_SUBTYPE_PCM;
	pdfwfx->DataFormat.Specifier = WSKSDATAFORMAT_SPECIFIER_WAVEFORMATEX;
	pdfwfx->DataFormat.SampleSize =
		(unsigned short)(pdfwfx->WaveFormatEx.nChannels
			* (pdfwfx->WaveFormatEx.wBitsPerSample / 8));

	/*
		Create the pin, always problematic.

		1169 ERROR_NO_MATCH
		WAVEFORMATEX passed in did not match that those of the device

		1450 ERROR_NO_SYSTEM_RESOURCES
		Basically doesn't support WDMKS in user mode, i.e.
		some newer Windows 10 device drivers
	*/
	pKsCreatePin = (LPKsCreatePin)
		GetProcAddress(hKSUserDll, "KsCreatePin");
	if (pKsCreatePin == NULL
		|| (err = (*pKsCreatePin)(hDevice,
			pPinConnect,
				GENERIC_READ | GENERIC_WRITE,
			&hPin)) != ERROR_SUCCESS)
	{
		/*wsbool isSuccess =
			err >= 0 && err <= 0x7FFFFFFF;
		wsbool isWarning =
			err >= 0x80000000 && err <= 0xBFFFFFFF;
		wsbool isError =
			err >= 0xC0000000 && err <= 0xFFFFFFFF;*/

		/*WSLogError("CreatePin Failed");*/
		WSFREE((BYTE*)pPinConnect);
		CloseHandle(hDevice);
		FreeLibrary(hKSUserDll);
		return FALSE;
	}

	WSFREE((BYTE*)pPinConnect);

	/* Obtain the device buffer size - prefer the device's */
	PropPin.Property.Set = WSKSPROPSETID_Connection;
	PropPin.Property.Flags = KSPROPERTY_TYPE_GET;
	PropPin.Reserved = 0;
	PropPin.PinId = 0;
	PropPin.Property.Id = WSKSPROPERTY_CONNECTION_ALLOCATORFRAMING;
	if(SUCCEEDED(SyncIoctl(hPin, IOCTL_KS_PROPERTY,
			&PropPin, sizeof(PropPin),
			&ksalframing, sizeof(ksalframing), &dwBytesReturned)))
		bufferSize = ksalframing.FrameSize;
	else
	{
		PropPin.Property.Id = WSKSPROPERTY_CONNECTION_ALLOCATORFRAMING_EX;
		if(SUCCEEDED(SyncIoctl(hPin, IOCTL_KS_PROPERTY,
			&PropPin, sizeof(PropPin),
			&ksalframingex, sizeof(ksalframingex), &dwBytesReturned)))
			bufferSize = /* RealTek uses an EX of 1764 */
				ksalframingex.FramingItem[0].FramingRange.Range.MinFrameSize;
		else
			bufferSize = spec->format.dwBufferSize == 0 ?
				(1024 * /* Number of samples per buffer */
				(audioformat->bitsPerSample / 8) * /* Convert bits to bytes */
				audioformat->channels) /* Samples * BytesPerSample * #chan = Bytes */
					: WSUPPERCLAMPTYPE(spec->format.dwBufferSize, ULONG);
	}
	/*
		Allocate the buffer. We could avoid any alignment issues by

		VirtualAlloc(NULL, bufferSize,
				MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

		but it really isn't necessary as we are allocating from the heap.
	*/
	buffer = (uint8_t*)
		WSALLOC(uint8_t, bufferSize);
	if(!buffer)
	{
		WSFREE(buffer);
		CloseHandle(hDevice);
		FreeLibrary(hKSUserDll);
		return FALSE;
	}

	pThis->mFlags = spec->flags;
	if(dwOpenMode & GENERIC_WRITE)
		pThis->mFlags |= WSAUDIO_OUTPUT;
	pThis->mFillProc = spec->proc;
	pThis->mUserData = spec->procuserdata;
	pThis->mNumChannels = spec->format.mSuper.channels;
	pThis->mPrevChunk = WSTYPECEILING(ULONG);
	pThis->mhPin = hPin;
	pThis->mhDevice = hDevice;
	pThis->mBuffer = buffer;
	pThis->mBufferSize = bufferSize;
	pThis->mhKSUserDll = hKSUserDll;
	/* Win32 uses -0x7FFFFFFF by default internally for minimum */
	pThis->mMinVolume = -2147483647; /* -2147483648 (nonexistant) == mute */
	pThis->mMaxVolume = 0;

	/*
		Not done yet! Find the volume node if it exists
	*/
	pThis->mVolumeNodeId = (ULONG)-1;
	PropPin.Property.Set = WSKSPROPSETID_Topology;
	PropPin.Property.Flags = KSPROPERTY_TYPE_GET;
	PropPin.Reserved = 0;
	PropPin.PinId = 0;
	PropPin.Property.Id = KSPROPERTY_TOPOLOGY_NODES;
	PropPin.Reserved = 0;
	if(DeviceIoControl(hDevice, IOCTL_KS_PROPERTY,
		&PropPin, sizeof(PropPin),
		NULL, 0,
		&dwBytesReturned, NULL) == FALSE
		&& GetLastError() != ERROR_MORE_DATA/*234*/)
	{
		return TRUE;
	}
	nodeTypes = (PKSMULTIPLE_ITEM)WSALLOC(BYTE, dwBytesReturned);
	if(!nodeTypes)
	{
		return TRUE;
	}
	if(DeviceIoControl(hDevice, IOCTL_KS_PROPERTY,
		&PropPin, sizeof(PropPin),
		nodeTypes, dwBytesReturned, &dwBytesReturned, NULL) == FALSE)
	{
		WSFREE(nodeTypes);
		return TRUE;
	}
	nodes = (GUID*)(nodeTypes+1);
	for (nodeId = 0; nodeId < nodeTypes->Count; nodeId++)
	{
		if(IsGUIDSame(&nodes[nodeId], &WSKSNODETYPE_VOLUME))
		{
			pThis->mVolumeNodeId = nodeId;
			break;
		}
	}

	if(pThis->mVolumeNodeId != (ULONG)-1)
	{ /* obtain the min and max volumes */
		KSNODEPROPERTY_AUDIO_CHANNEL ksNode;
		DWORD dwBytesReturned;
		KSPROPERTY_DESCRIPTION* pKsdesc;
		KSPROPERTY_STEPPING_LONG* pVolumeRange;
		BOOL Ret;
		GUID WSKSPROPSETID_Audio = { 0x45FFAAA0, 0x6E1B, 0x11D0,
			{0xBC, 0xF2, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00} };
		ksNode.NodeProperty.Property.Set = WSKSPROPSETID_Audio;
		ksNode.NodeProperty.Property.Id = KSPROPERTY_AUDIO_VOLUMELEVEL;
		ksNode.NodeProperty.Property.Flags =
			KSPROPERTY_TYPE_BASICSUPPORT | KSPROPERTY_TYPE_TOPOLOGY;
		ksNode.NodeProperty.NodeId = pThis->mVolumeNodeId;
		ksNode.Reserved = 0;
		/*ksNode.NodeProperty.Reserved = 0;*/
		pKsdesc
			= (KSPROPERTY_DESCRIPTION*)
			WSALLOC(char, sizeof(KSPROPERTY_DESCRIPTION));
		Ret = DeviceIoControl(pThis->mhPin,
			IOCTL_KS_PROPERTY, &ksNode, sizeof(ksNode),
			pKsdesc, sizeof(KSPROPERTY_DESCRIPTION), &dwBytesReturned, NULL);
		if(Ret)
		{
			KSPROPERTY_DESCRIPTION* pNewKsdesc = (KSPROPERTY_DESCRIPTION*)
				WSREALLOC(pKsdesc, char,
						pKsdesc->DescriptionSize);
			if(pNewKsdesc)
			{
				pKsdesc = pNewKsdesc;
				Ret = DeviceIoControl(pThis->mhPin,
					IOCTL_KS_PROPERTY, &ksNode, sizeof(ksNode),
					pKsdesc, pKsdesc->DescriptionSize, &dwBytesReturned, NULL);
				if(Ret)
				{
					pVolumeRange = (KSPROPERTY_STEPPING_LONG*)
						(((PKSPROPERTY_MEMBERSHEADER)(pKsdesc + 1)) + 1);
					pThis->mMinVolume = pVolumeRange->Bounds.SignedMinimum;
					pThis->mMaxVolume = pVolumeRange->Bounds.SignedMaximum;
				}
			}
		}
		WSFREE(pKsdesc);
	}

	WSFREE(nodeTypes);
	return TRUE;
}

void WSAudio_Destroy(WSAUDIO* pThis)
{
	WSAudio_SetState_(pThis, KSSTATE_PAUSE);
	WSAudio_SetState_(pThis, KSSTATE_STOP);
	CloseHandle(pThis->mhPin);
	FreeLibrary(pThis->mhKSUserDll);
	CloseHandle(pThis->mhDevice);
	/*VirtualFree(pThis->mBuffer, 0, MEM_RELEASE);*/
	WSFREE(pThis->mBuffer);
}
wsbool WSAudio_SetVolume_(WSAUDIO* pThis,
	int leftVolume, int rightVolume)
{
	KSNODEPROPERTY_AUDIO_CHANNEL ksNode;
	DWORD dwBytesReturned;
	LONG volumeLevel;
	GUID WSKSPROPSETID_Audio = { 0x45FFAAA0, 0x6E1B, 0x11D0,
		{0xBC, 0xF2, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00} };
	if(pThis->mVolumeNodeId == (ULONG)-1)
		return FALSE;

	ksNode.NodeProperty.Property.Set = WSKSPROPSETID_Audio;
	ksNode.NodeProperty.Property.Id = KSPROPERTY_AUDIO_VOLUMELEVEL;
	ksNode.NodeProperty.Property.Flags =
		KSPROPERTY_TYPE_SET | KSPROPERTY_TYPE_TOPOLOGY;
	ksNode.NodeProperty.NodeId = pThis->mVolumeNodeId;
	ksNode.Reserved = 0;
	ksNode.Channel = 0; /* 0 == left, 1 == right */
	volumeLevel = leftVolume; /*WSAudio_GetMinVolume(pThis) * 0.3;*/
	if(DeviceIoControl(pThis->mhPin,
		IOCTL_KS_PROPERTY, &ksNode, sizeof(ksNode),
		&volumeLevel, sizeof(volumeLevel), &dwBytesReturned, NULL) == FALSE)
	{
		return FALSE;
	}
	if(pThis->mNumChannels > 1)
	{
		ksNode.Channel = 1; /* 0 == left, 1 == right */
		volumeLevel = rightVolume; /*WSAudio_GetMinVolume(pThis) * 0.3;*/
		if(DeviceIoControl(pThis->mhPin,
			IOCTL_KS_PROPERTY, &ksNode, sizeof(ksNode),
			&volumeLevel, sizeof(volumeLevel),
			&dwBytesReturned, NULL) == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright)
{
	/*int fullvol = WSAudio_GetMaxVolume(pThis);
	int halfvol = fullvol / 2;
	*/int volLeft = panleft - panright;
	int volRight = panright - panleft;
	return WSAudio_SetVolume_(pThis, volLeft, volRight);
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{
	return WSAudio_SetVolume_(pThis, volume, volume);
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	return pThis->mMinVolume;
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	return pThis->mMaxVolume;
}
wsbool WSAudio_ReadWrite(WSAUDIO* pThis, uint8_t* buffer, ULONG bufferSize)
{
	WSKSSTREAM_HEADER Header;
	OVERLAPPED Overlapped;
	WSMemoryFill(&Overlapped, 0, sizeof(Overlapped));
	Header.Size = sizeof(Header);
	Header.TypeSpecificFlags = 0;
	Header.PresentationTime.Time = 0;
	Header.PresentationTime.Numerator = 1;
	Header.PresentationTime.Denominator = 1;
	Header.Duration = 0;
	Header.FrameExtent = WSUPPERCLAMPTYPE(bufferSize, ULONG);
	Header.Data = (void*)buffer;
	Header.OptionsFlags = 0;
#if _WIN64
	Header.Reserved = 0;
#endif
	if(pThis->mFlags & WSAUDIO_INPUT)
	{
		Header.DataUsed = 0;
		if (DeviceIoControl(pThis->mhPin, IOCTL_KS_READ_STREAM, NULL, 0,
			&Header, Header.Size, NULL, &Overlapped) == FALSE)
			return FALSE;
	}
	else
	{
		Header.DataUsed = bufferSize;
		if (DeviceIoControl(pThis->mhPin, IOCTL_KS_WRITE_STREAM, NULL, 0,
			&Header, Header.Size, NULL, &Overlapped) == FALSE)
		{
			DWORD gle = GetLastError();
			if(gle != ERROR_IO_PENDING)
				return FALSE;
		}
	}
	/*WSASSERT(uRet == Header.Size);*/
	return TRUE;
}
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	/*
		Source of a lot of bugs and too many hours debugging

		1) A device has to be in KSSTATE_PAUSE or KSSTATE_RUN
		to write to it, otherwise DeviceIoControl will fail
		with a GetLastError() of 1 (ERROR_INVALID_FUNCTION).

		2) If you go from KSSTATE_STOP to KSSTATE_RUN
		various stock Windows drivers choke and return
		random errors. Always go KSSTATE_STOP->KSSTATE_PAUSE->KSSTATE_RUN.
	*/
	HRESULT hr = WSAudio_SetState_(pThis, KSSTATE_PAUSE);
	if(FAILED(hr))
		return FALSE;
	hr = WSAudio_SetState_(pThis, KSSTATE_RUN);
	if(FAILED(hr))
		return FALSE;
	return TRUE;
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{
	KSSTATE state;
	if(FAILED(WSAudio_State_(pThis, &state, KSPROPERTY_TYPE_GET)))
		return FALSE;
	return state == KSSTATE_RUN;
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	return SUCCEEDED(WSAudio_SetState_(pThis, KSSTATE_PAUSE));
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	return SUCCEEDED(WSAudio_SetState_(pThis, KSSTATE_RUN));
}
wsbool WSAudio_GetKSPosition(WSAUDIO* pThis, KSAUDIO_POSITION* pPos)
{
	KSPROPERTY Property;
	ULONG uRet;
	GUID WSKSPROPSETID_Audio = { 0x45FFAAA0, 0x6E1B, 0x11D0,
		{0xBC, 0xF2, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00} };
	Property.Alignment = 0;
	Property.Set = WSKSPROPSETID_Audio;
	Property.Id = KSPROPERTY_AUDIO_POSITION;
	Property.Flags = KSPROPERTY_TYPE_GET;

	if(FAILED(SyncIoctl(pThis->mhPin, IOCTL_KS_PROPERTY, &Property,
		sizeof(Property), pPos, sizeof(KSAUDIO_POSITION), &uRet)))
		return FALSE;
	return TRUE;
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	KSAUDIO_POSITION Pos;
	if(!WSAudio_GetKSPosition(pThis, &Pos))
		return 0;
	if(pThis->mFlags & WSAUDIO_INPUT)
		return (size_t)Pos.PlayOffset;
	return (size_t)Pos.WriteOffset;
}
wsbool WSAudioHasFullDuplex() { return FALSE; } /* Pins are one direction */
void WSAudio_Process(WSAUDIO* pThis)
{
	KSAUDIO_POSITION Pos;
	ULONG dspos;
	if(!WSAudio_GetKSPosition(pThis, &Pos))
		return;

	if(pThis->mFlags & WSAUDIO_INPUT)
		dspos = (ULONG)Pos.PlayOffset;
	else
		dspos = (ULONG)Pos.WriteOffset;

	if((dspos / pThis->mBufferSize) != pThis->mPrevChunk)
	{ /* WDMKS handles all the buffer management itself */
		ULONG prevPos = (dspos / pThis->mBufferSize);
		if(pThis->mFlags & WSAUDIO_INPUT)
		{
			WSAudio_ReadWrite(pThis, pThis->mBuffer, pThis->mBufferSize);
			(*pThis->mFillProc)(pThis->mUserData,
				pThis->mBuffer, pThis->mBufferSize);
		}
		if(!(pThis->mFlags & WSAUDIO_INPUT)
			|| (pThis->mFlags & WSAUDIO_OUTPUT))
		{
			(*pThis->mFillProc)(pThis->mUserData,
				pThis->mBuffer, pThis->mBufferSize);
			WSAudio_ReadWrite(pThis, pThis->mBuffer, pThis->mBufferSize);
		}
		pThis->mPrevChunk = prevPos;
	}
}
#if defined(__cplusplus)
} /* end extern "C" */
#endif
/*===========================================================================
				 #     # ### #     #  #####   #####
				 #  #  #  #  ##    # #     # #     #
				 #  #  #  #  # #   #       #       #
				 #  #  #  #  #  #  #  #####   #####
				 #  #  #  #  #   # #       # #
				 #  #  #  #  #    ## #     # #
				  ## ##  ### #     #  #####  #######

			 #     #    #     #####     #    ######  ###
			 #  #  #   # #   #     #   # #   #     #  #
			 #  #  #  #   #  #        #   #  #     #  #
			 #  #  # #     #  #####  #     # ######   #
			 #  #  # #######       # ####### #        #
			 #  #  # #     # #     # #     # #        #
			  ## ##  #     #  #####  #     # #       ###

	WASAPI ("Windows Audio Session API") (Vista+) is quite straightforward
	despite the official samples being giant C++ classes. Every little
	feature generally requires a new COM interface from

	IAudioClient_GetService

	90% of the stuff here is definitions for MSVC6 and old Metrowerks.

	TODO:

	IMMDeviceEnumerator_GetDevice(enumerator, name, &dev);
	for custom devices

	Wait 8000 milliseconds on E_NOTFOUND for bad drivers on GetDevice

	Make a custom IMMNotificationClient for hotplug support
===========================================================================*/
#elif defined(_WIN32) && !defined(WSAUDIOXAUDIOOPT) \
			&& !defined(WSAUDIOSMOPT)
	#include <windows.h>
	#include <objbase.h>
	typedef LONGLONG REFERENCE_TIME;
	void WSAudioFormatToWaveFormatEx(
		const WSAUDIOFORMAT* Format, WAVEFORMATEX* wfx)
	{
		wfx->cbSize = 0;
		wfx->wFormatTag = WAVE_FORMAT_PCM;
		wfx->wBitsPerSample = Format->bitsPerSample;
		wfx->nSamplesPerSec = Format->sampleRate;
		wfx->nChannels = Format->channels;
		wfx->nBlockAlign = WSAudioFormat_GetBlockAlign(Format);
		wfx->nAvgBytesPerSec = Format->sampleRate * wfx->nBlockAlign;
	}

	void WSWaveFormatExToAudioFormat(
		const WAVEFORMATEX* wfx, WSAUDIOFORMAT* Format)
	{
		Format->bitsPerSample = wfx->wBitsPerSample;
		Format->sampleRate = wfx->nSamplesPerSec;
		Format->channels = wfx->nChannels;
	}

#define DEVICE_STATE_ACTIVE      0x00000001
#define DEVICE_STATE_DISABLED    0x00000002
#define DEVICE_STATE_NOTPRESENT  0x00000004
#define DEVICE_STATE_UNPLUGGED   0x00000008

#ifndef PROPERTYKEY_DEFINED
#define PROPERTYKEY_DEFINED
typedef struct _tagpropertykey
{
	GUID fmtid;
	DWORD pid;
} PROPERTYKEY;
#endif
#ifndef _REFPROPVARIANT_DEFINED
#define _REFPROPVARIANT_DEFINED
#ifdef __cplusplus
#define REFPROPVARIANT const PROPVARIANT &
#else
#define REFPROPVARIANT const PROPVARIANT * const
#endif
#endif

#define WSAUDCLNT_SHAREMODE_SHARED 0
#define WSAUDCLNT_SHAREMODE_EXCLUSIVE 1
typedef int WSAUDCLNT_SHAREMODE;

#define eRender	0
#define eCapture ( eRender + 1 )
#define eAll ( eCapture + 1 )
typedef int EDataFlow;

#define eConsole 0
#define eMultimedia	( eConsole + 1 )
#define eCommunications	( eMultimedia + 1 )
typedef int ERole;

#define WSAudioSessionStateInactive 0
#define WSAudioSessionStateActive 1
#define WSAudioSessionStateExpired 2
typedef int WSAudioSessionState;

#ifndef REFPROPERTYKEY
#ifdef __cplusplus
#define REFPROPERTYKEY const PROPERTYKEY &
#else /* !__cplusplus*/
#define REFPROPERTYKEY const PROPERTYKEY * const
#endif /* __cplusplus*/
#endif /*REFPROPERTYKEY*/

#ifdef PropVariantInit
#undef PropVariantInit
#endif
#define PropVariantInit(x) WSMemoryFill(x, 0, sizeof(*x))

#define AUDCLNT_STREAMFLAGS_CROSSPROCESS             0x00010000
#define AUDCLNT_STREAMFLAGS_LOOPBACK                 0x00020000
#define AUDCLNT_STREAMFLAGS_EVENTCALLBACK            0x00040000
#define AUDCLNT_STREAMFLAGS_NOPERSIST                0x00080000
#define AUDCLNT_STREAMFLAGS_RATEADJUST               0x00100000
#define AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY      0x08000000
#define AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM           0x80000000
#define AUDCLNT_SESSIONFLAGS_EXPIREWHENUNOWNED       0x10000000
#define AUDCLNT_SESSIONFLAGS_DISPLAY_HIDE            0x20000000
#define AUDCLNT_SESSIONFLAGS_DISPLAY_HIDEWHENEXPIRED 0x40000000

typedef struct IAudioClient
{
	CONST_VTBL struct IAudioClientVtbl *lpVtbl;
} IAudioClient;
typedef struct IAudioClientVtbl
{
	BEGIN_INTERFACE
	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IAudioClient * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IAudioClient * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IAudioClient * This);
	HRESULT ( STDMETHODCALLTYPE *Initialize )(
		IAudioClient * This,
		/* [annotation][in] */
		WSAUDCLNT_SHAREMODE ShareMode,
		/* [annotation][in] */
		DWORD StreamFlags,
		/* [annotation][in] */
		REFERENCE_TIME hnsBufferDuration,
		/* [annotation][in] */
		REFERENCE_TIME hnsPeriodicity,
		/* [annotation][in] */
		const WAVEFORMATEX *pFormat,
		/* [annotation][in] */
		const LPGUID AudioSessionGuid);
	HRESULT ( STDMETHODCALLTYPE *GetBufferSize )(
		IAudioClient * This,
		/* [annotation][out] */
		UINT32 *pNumBufferFrames);
	HRESULT ( STDMETHODCALLTYPE *GetStreamLatency )(
		IAudioClient * This,
		/* [annotation][out] */
		REFERENCE_TIME *phnsLatency);
	HRESULT ( STDMETHODCALLTYPE *GetCurrentPadding )(
		IAudioClient * This,
		/* [annotation][out] */
		UINT32 *pNumPaddingFrames);
	HRESULT ( STDMETHODCALLTYPE *IsFormatSupported )(
		IAudioClient * This,
		/* [annotation][in] */
		WSAUDCLNT_SHAREMODE ShareMode,
		/* [annotation][in] */
		const WAVEFORMATEX *pFormat,
		/* [unique][annotation][out] */
		WAVEFORMATEX **ppClosestMatch);
	HRESULT ( STDMETHODCALLTYPE *GetMixFormat )(
		IAudioClient * This,
		/* [annotation][out] */
		WAVEFORMATEX **ppDeviceFormat);
	HRESULT ( STDMETHODCALLTYPE *GetDevicePeriod )(
		IAudioClient * This,
		/* [annotation][out] */
		REFERENCE_TIME *phnsDefaultDevicePeriod,
		/* [annotation][out] */
		REFERENCE_TIME *phnsMinimumDevicePeriod);
	HRESULT ( STDMETHODCALLTYPE *Start )(
		IAudioClient * This);
	HRESULT ( STDMETHODCALLTYPE *Stop )(
		IAudioClient * This);
	HRESULT ( STDMETHODCALLTYPE *Reset )(
		IAudioClient * This);
	HRESULT ( STDMETHODCALLTYPE *SetEventHandle )(
		IAudioClient * This,
		/* [in] */ HANDLE eventHandle);
	HRESULT ( STDMETHODCALLTYPE *GetService )(
		IAudioClient * This,
		/* [annotation][in] */
		REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppv);

	END_INTERFACE
} IAudioClientVtbl;
#define IAudioClient_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IAudioClient_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IAudioClient_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IAudioClient_Initialize(This,ShareMode,StreamFlags,hnsBufferDuration,hnsPeriodicity,pFormat,AudioSessionGuid)	\
	( (This)->lpVtbl -> Initialize(This,ShareMode,StreamFlags,hnsBufferDuration,hnsPeriodicity,pFormat,AudioSessionGuid) )
#define IAudioClient_GetBufferSize(This,pNumBufferFrames)	\
	( (This)->lpVtbl -> GetBufferSize(This,pNumBufferFrames) )
#define IAudioClient_GetStreamLatency(This,phnsLatency)	\
	( (This)->lpVtbl -> GetStreamLatency(This,phnsLatency) )
#define IAudioClient_GetCurrentPadding(This,pNumPaddingFrames)	\
	( (This)->lpVtbl -> GetCurrentPadding(This,pNumPaddingFrames) )
#define IAudioClient_IsFormatSupported(This,ShareMode,pFormat,ppClosestMatch)	\
	( (This)->lpVtbl -> IsFormatSupported(This,ShareMode,pFormat,ppClosestMatch) )
#define IAudioClient_GetMixFormat(This,ppDeviceFormat)	\
	( (This)->lpVtbl -> GetMixFormat(This,ppDeviceFormat) )
#define IAudioClient_GetDevicePeriod(This,phnsDefaultDevicePeriod,phnsMinimumDevicePeriod)	\
	( (This)->lpVtbl -> GetDevicePeriod(This,phnsDefaultDevicePeriod,phnsMinimumDevicePeriod) )
#define IAudioClient_Start(This)	\
	( (This)->lpVtbl -> Start(This) )
#define IAudioClient_Stop(This)	\
	( (This)->lpVtbl -> Stop(This) )
#define IAudioClient_Reset(This)	\
	( (This)->lpVtbl -> Reset(This) )
#define IAudioClient_SetEventHandle(This,eventHandle)	\
	( (This)->lpVtbl -> SetEventHandle(This,eventHandle) )
#define IAudioClient_GetService(This,riid,ppv)	\
	( (This)->lpVtbl -> GetService(This,riid,ppv) )

typedef struct IAudioCaptureClient
{
	CONST_VTBL struct IAudioCaptureClientVtbl *lpVtbl;
} IAudioCaptureClient;
typedef struct IAudioCaptureClientVtbl
{
	BEGIN_INTERFACE

	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IAudioCaptureClient * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IAudioCaptureClient * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IAudioCaptureClient * This);
	HRESULT ( STDMETHODCALLTYPE *GetBuffer )(
		IAudioCaptureClient * This,
		/* [annotation][out] */
		BYTE **ppData,
		/* [annotation][out] */
		UINT32 *pNumFramesToRead,
		/* [annotation][out] */
		DWORD *pdwFlags,
		/* [annotation][unique][out] */
		UINT64 *pu64DevicePosition,
		/* [annotation][unique][out] */
		UINT64 *pu64QPCPosition);
	HRESULT ( STDMETHODCALLTYPE *ReleaseBuffer )(
		IAudioCaptureClient * This,
		/* [annotation][in] */
		UINT32 NumFramesRead);
	HRESULT ( STDMETHODCALLTYPE *GetNextPacketSize )(
		IAudioCaptureClient * This,
		/* [annotation][out] */
		UINT32 *pNumFramesInNextPacket);

	END_INTERFACE
} IAudioCaptureClientVtbl;
#define IAudioCaptureClient_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IAudioCaptureClient_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IAudioCaptureClient_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IAudioCaptureClient_GetBuffer(This,ppData,pNumFramesToRead,pdwFlags,pu64DevicePosition,pu64QPCPosition)	\
	( (This)->lpVtbl -> GetBuffer(This,ppData,pNumFramesToRead,pdwFlags,pu64DevicePosition,pu64QPCPosition) )
#define IAudioCaptureClient_ReleaseBuffer(This,NumFramesRead)	\
	( (This)->lpVtbl -> ReleaseBuffer(This,NumFramesRead) )
#define IAudioCaptureClient_GetNextPacketSize(This,pNumFramesInNextPacket)	\
	( (This)->lpVtbl -> GetNextPacketSize(This,pNumFramesInNextPacket) )

typedef struct IAudioRenderClient
{
	CONST_VTBL struct IAudioRenderClientVtbl *lpVtbl;
} IAudioRenderClient;
typedef struct IAudioRenderClientVtbl
{
	BEGIN_INTERFACE

	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IAudioRenderClient * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IAudioRenderClient * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IAudioRenderClient * This);
	HRESULT ( STDMETHODCALLTYPE *GetBuffer )(
		IAudioRenderClient * This,
		/* [annotation][in] */
		UINT32 NumFramesRequested,
		/* [annotation][out] */
		BYTE **ppData);
	HRESULT ( STDMETHODCALLTYPE *ReleaseBuffer )(
		IAudioRenderClient * This,
		/* [annotation][in] */
		UINT32 NumFramesWritten,
		/* [annotation][in] */
		DWORD dwFlags);

	END_INTERFACE
} IAudioRenderClientVtbl;
#define IAudioRenderClient_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IAudioRenderClient_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IAudioRenderClient_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IAudioRenderClient_GetBuffer(This,NumFramesRequested,ppData)	\
	( (This)->lpVtbl -> GetBuffer(This,NumFramesRequested,ppData) )
#define IAudioRenderClient_ReleaseBuffer(This,NumFramesWritten,dwFlags)	\
	( (This)->lpVtbl -> ReleaseBuffer(This,NumFramesWritten,dwFlags) )

typedef struct IPropertyStore
{
	CONST_VTBL struct IPropertyStoreVtbl *lpVtbl;
} IPropertyStore;
typedef struct IPropertyStoreVtbl
{
	BEGIN_INTERFACE

	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IPropertyStore * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IPropertyStore * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IPropertyStore * This);
	HRESULT ( STDMETHODCALLTYPE *GetCount )(
		IPropertyStore * This,
		/* [out] */ DWORD *cProps);
	HRESULT ( STDMETHODCALLTYPE *GetAt )(
		IPropertyStore * This,
		/* [in] */ DWORD iProp,
		/* [out] */ PROPERTYKEY *pkey);
	HRESULT ( STDMETHODCALLTYPE *GetValue )(
		IPropertyStore * This,
		/* [in] */ REFPROPERTYKEY key,
		/* [out] */ PROPVARIANT *pv);
	HRESULT ( STDMETHODCALLTYPE *SetValue )(
		IPropertyStore * This,
		/* [in] */ REFPROPERTYKEY key,
		/* [in] */ REFPROPVARIANT propvar);
	HRESULT ( STDMETHODCALLTYPE *Commit )(
		IPropertyStore * This);

	END_INTERFACE
} IPropertyStoreVtbl;
#define IPropertyStore_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IPropertyStore_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IPropertyStore_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IPropertyStore_GetCount(This,cProps)	\
	( (This)->lpVtbl -> GetCount(This,cProps) )
#define IPropertyStore_GetAt(This,iProp,pkey)	\
	( (This)->lpVtbl -> GetAt(This,iProp,pkey) )
#define IPropertyStore_GetValue(This,key,pv)	\
	( (This)->lpVtbl -> GetValue(This,key,pv) )
#define IPropertyStore_SetValue(This,key,propvar)	\
	( (This)->lpVtbl -> SetValue(This,key,propvar) )
#define IPropertyStore_Commit(This)	\
	( (This)->lpVtbl -> Commit(This) )

typedef struct IAudioStreamVolume
{
	CONST_VTBL struct IAudioStreamVolumeVtbl *lpVtbl;
} IAudioStreamVolume;
typedef struct IAudioStreamVolumeVtbl
{
	BEGIN_INTERFACE

	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IAudioStreamVolume * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IAudioStreamVolume * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IAudioStreamVolume * This);
	HRESULT ( STDMETHODCALLTYPE *GetChannelCount )(
		IAudioStreamVolume * This,
		/* [annotation][out] */
		UINT32 *pdwCount);
	HRESULT ( STDMETHODCALLTYPE *SetChannelVolume )(
		IAudioStreamVolume * This,
		/* [annotation][in] */
		UINT32 dwIndex,
		/* [annotation][in] */
		const float fLevel);
	HRESULT ( STDMETHODCALLTYPE *GetChannelVolume )(
		IAudioStreamVolume * This,
		/* [annotation][in] */
		UINT32 dwIndex,
		/* [annotation][out] */
		float *pfLevel);
	HRESULT ( STDMETHODCALLTYPE *SetAllVolumes )(
		IAudioStreamVolume * This,
		/* [annotation][in] */
		UINT32 dwCount,
		/* [annotation][size_is][in] */
		const float *pfVolumes);
	HRESULT ( STDMETHODCALLTYPE *GetAllVolumes )(
		IAudioStreamVolume * This,
		/* [annotation][in] */
		UINT32 dwCount,
		/* [annotation][size_is][out] */
		float *pfVolumes);

	END_INTERFACE
} IAudioStreamVolumeVtbl;
#define IAudioStreamVolume_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IAudioStreamVolume_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IAudioStreamVolume_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IAudioStreamVolume_GetChannelCount(This,pdwCount)	\
	( (This)->lpVtbl -> GetChannelCount(This,pdwCount) )
#define IAudioStreamVolume_SetChannelVolume(This,dwIndex,fLevel)	\
	( (This)->lpVtbl -> SetChannelVolume(This,dwIndex,fLevel) )
#define IAudioStreamVolume_GetChannelVolume(This,dwIndex,pfLevel)	\
	( (This)->lpVtbl -> GetChannelVolume(This,dwIndex,pfLevel) )
#define IAudioStreamVolume_SetAllVolumes(This,dwCount,pfVolumes)	\
	( (This)->lpVtbl -> SetAllVolumes(This,dwCount,pfVolumes) )
#define IAudioStreamVolume_GetAllVolumes(This,dwCount,pfVolumes)	\
	( (This)->lpVtbl -> GetAllVolumes(This,dwCount,pfVolumes) )

typedef struct IAudioSessionEvents IAudioSessionEvents;
typedef struct IAudioSessionControl
{
	CONST_VTBL struct IAudioSessionControlVtbl *lpVtbl;
} IAudioSessionControl;
typedef struct IAudioSessionControlVtbl
{
	BEGIN_INTERFACE

	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IAudioSessionControl * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IAudioSessionControl * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IAudioSessionControl * This);
	HRESULT ( STDMETHODCALLTYPE *GetState )(
		IAudioSessionControl * This,
		/* [annotation][out] */
		WSAudioSessionState *pRetVal);
	HRESULT ( STDMETHODCALLTYPE *GetDisplayName )(
		IAudioSessionControl * This,
		/* [annotation][string][out] */
		LPWSTR *pRetVal);
	HRESULT ( STDMETHODCALLTYPE *SetDisplayName )(
		IAudioSessionControl * This,
		/* [annotation][string][in] */
		LPCWSTR Value,
		/* [unique][in] */ const LPGUID EventContext);
	HRESULT ( STDMETHODCALLTYPE *GetIconPath )(
		IAudioSessionControl * This,
		/* [annotation][string][out] */
		LPWSTR *pRetVal);
	HRESULT ( STDMETHODCALLTYPE *SetIconPath )(
		IAudioSessionControl * This,
		/* [annotation][string][in] */
		LPCWSTR Value,
		/* [unique][in] */ const LPGUID EventContext);
	HRESULT ( STDMETHODCALLTYPE *GetGroupingParam )(
		IAudioSessionControl * This,
		/* [annotation][out] */
		GUID *pRetVal);
	HRESULT ( STDMETHODCALLTYPE *SetGroupingParam )(
		IAudioSessionControl * This,
		/* [annotation][in] */
		const LPGUID Override,
		/* [unique][in] */ const LPGUID EventContext);
	HRESULT ( STDMETHODCALLTYPE *RegisterAudioSessionNotification )(
		IAudioSessionControl * This,
		/* [annotation][in] */
		IAudioSessionEvents *NewNotifications);
	HRESULT ( STDMETHODCALLTYPE *UnregisterAudioSessionNotification )(
		IAudioSessionControl * This,
		/* [annotation][in] */
		IAudioSessionEvents *NewNotifications);

	END_INTERFACE
} IAudioSessionControlVtbl;
#define IAudioSessionControl_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IAudioSessionControl_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IAudioSessionControl_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IAudioSessionControl_GetState(This,pRetVal)	\
	( (This)->lpVtbl -> GetState(This,pRetVal) )
#define IAudioSessionControl_GetDisplayName(This,pRetVal)	\
	( (This)->lpVtbl -> GetDisplayName(This,pRetVal) )
#define IAudioSessionControl_SetDisplayName(This,Value,EventContext)	\
	( (This)->lpVtbl -> SetDisplayName(This,Value,EventContext) )
#define IAudioSessionControl_GetIconPath(This,pRetVal)	\
	( (This)->lpVtbl -> GetIconPath(This,pRetVal) )
#define IAudioSessionControl_SetIconPath(This,Value,EventContext)	\
	( (This)->lpVtbl -> SetIconPath(This,Value,EventContext) )
#define IAudioSessionControl_GetGroupingParam(This,pRetVal)	\
	( (This)->lpVtbl -> GetGroupingParam(This,pRetVal) )
#define IAudioSessionControl_SetGroupingParam(This,Override,EventContext)	\
	( (This)->lpVtbl -> SetGroupingParam(This,Override,EventContext) )
#define IAudioSessionControl_RegisterAudioSessionNotification(This,NewNotifications)	\
	( (This)->lpVtbl -> RegisterAudioSessionNotification(This,NewNotifications) )
#define IAudioSessionControl_UnregisterAudioSessionNotification(This,NewNotifications)	\
	( (This)->lpVtbl -> UnregisterAudioSessionNotification(This,NewNotifications) )

typedef struct IMMNotificationClient
{
	CONST_VTBL struct IMMNotificationClientVtbl *lpVtbl;
} IMMNotificationClient;
typedef struct IMMNotificationClientVtbl
{
	BEGIN_INTERFACE
	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IMMNotificationClient * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IMMNotificationClient * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IMMNotificationClient * This);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnDeviceStateChanged )(
		IMMNotificationClient * This,
		/* [annotation][in] */
		LPCWSTR pwstrDeviceId,
		/* [annotation][in] */
		DWORD dwNewState);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnDeviceAdded )(
		IMMNotificationClient * This,
		/* [annotation][in] */
		LPCWSTR pwstrDeviceId);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnDeviceRemoved )(
		IMMNotificationClient * This,
		/* [annotation][in] */
		LPCWSTR pwstrDeviceId);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnDefaultDeviceChanged )(
		IMMNotificationClient * This,
		/* [annotation][in] */
		EDataFlow flow,
		/* [annotation][in] */
		ERole role,
		/* [annotation][in] */
		LPCWSTR pwstrDefaultDeviceId);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnPropertyValueChanged )(
		IMMNotificationClient * This,
		/* [annotation][in] */
		LPCWSTR pwstrDeviceId,
		/* [annotation][in] */
		const PROPERTYKEY key);

	END_INTERFACE
} IMMNotificationClientVtbl;
#define IMMNotificationClient_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IMMNotificationClient_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IMMNotificationClient_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IMMNotificationClient_OnDeviceStateChanged(This,pwstrDeviceId,dwNewState)	\
	( (This)->lpVtbl -> OnDeviceStateChanged(This,pwstrDeviceId,dwNewState) )
#define IMMNotificationClient_OnDeviceAdded(This,pwstrDeviceId)	\
	( (This)->lpVtbl -> OnDeviceAdded(This,pwstrDeviceId) )
#define IMMNotificationClient_OnDeviceRemoved(This,pwstrDeviceId)	\
	( (This)->lpVtbl -> OnDeviceRemoved(This,pwstrDeviceId) )
#define IMMNotificationClient_OnDefaultDeviceChanged(This,flow,role,pwstrDefaultDeviceId)	\
	( (This)->lpVtbl -> OnDefaultDeviceChanged(This,flow,role,pwstrDefaultDeviceId) )
#define IMMNotificationClient_OnPropertyValueChanged(This,pwstrDeviceId,key)	\
	( (This)->lpVtbl -> OnPropertyValueChanged(This,pwstrDeviceId,key) )

typedef struct IMMDevice
{
	CONST_VTBL struct IMMDeviceVtbl *lpVtbl;
} IMMDevice;
typedef struct IMMDeviceVtbl
{
	BEGIN_INTERFACE

	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IMMDevice * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IMMDevice * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IMMDevice * This);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Activate )(
		IMMDevice * This,
		/* [annotation][in] */
		REFIID iid,
		/* [annotation][in] */
		DWORD dwClsCtx,
		/* [annotation][unique][in] */
		PROPVARIANT *pActivationParams,
		/* [annotation][iid_is][out] */
		void **ppInterface);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenPropertyStore )(
		IMMDevice * This,
		/* [annotation][in] */
		DWORD stgmAccess,
		/* [annotation][out] */
		IPropertyStore **ppProperties);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetId )(
		IMMDevice * This,
		/* [annotation][out] */
		LPWSTR *ppstrId);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetState )(
		IMMDevice * This,
		/* [annotation][out] */
		DWORD *pdwState);

	END_INTERFACE
} IMMDeviceVtbl;
#define IMMDevice_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IMMDevice_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IMMDevice_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IMMDevice_Activate(This,iid,dwClsCtx,pActivationParams,ppInterface)	\
	( (This)->lpVtbl -> Activate(This,iid,dwClsCtx,pActivationParams,ppInterface) )
#define IMMDevice_OpenPropertyStore(This,stgmAccess,ppProperties)	\
	( (This)->lpVtbl -> OpenPropertyStore(This,stgmAccess,ppProperties) )
#define IMMDevice_GetId(This,ppstrId)	\
	( (This)->lpVtbl -> GetId(This,ppstrId) )
#define IMMDevice_GetState(This,pdwState)	\
	( (This)->lpVtbl -> GetState(This,pdwState) )

typedef struct IMMDeviceCollection
{
	CONST_VTBL struct IMMDeviceCollectionVtbl *lpVtbl;
} IMMDeviceCollection;
typedef struct IMMDeviceCollectionVtbl
{
	BEGIN_INTERFACE

	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IMMDeviceCollection * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IMMDeviceCollection * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IMMDeviceCollection * This);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCount )(
		IMMDeviceCollection * This,
		/* [annotation][out] */
		UINT *pcDevices);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Item )(
		IMMDeviceCollection * This,
		/* [annotation][in] */
		UINT nDevice,
		/* [annotation][out] */
		IMMDevice **ppDevice);

	END_INTERFACE
} IMMDeviceCollectionVtbl;
#define IMMDeviceCollection_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IMMDeviceCollection_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IMMDeviceCollection_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IMMDeviceCollection_GetCount(This,pcDevices)	\
	( (This)->lpVtbl -> GetCount(This,pcDevices) )
#define IMMDeviceCollection_Item(This,nDevice,ppDevice)	\
	( (This)->lpVtbl -> Item(This,nDevice,ppDevice) )

typedef struct IMMDeviceEnumerator
{
	CONST_VTBL struct IMMDeviceEnumeratorVtbl *lpVtbl;
} IMMDeviceEnumerator;
typedef struct IMMDeviceEnumeratorVtbl
{
	BEGIN_INTERFACE

	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IMMDeviceEnumerator * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IMMDeviceEnumerator * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IMMDeviceEnumerator * This);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumAudioEndpoints )(
		IMMDeviceEnumerator * This,
		/* [annotation][in] */
		EDataFlow dataFlow,
		/* [annotation][in] */
		DWORD dwStateMask,
		/* [annotation][out] */
		IMMDeviceCollection **ppDevices);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDefaultAudioEndpoint )(
		IMMDeviceEnumerator * This,
		/* [annotation][in] */
		EDataFlow dataFlow,
		/* [annotation][in] */
		ERole role,
		/* [annotation][out] */
		IMMDevice **ppEndpoint);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDevice )(
		IMMDeviceEnumerator * This,
		/* [annotation][in] */
		LPCWSTR pwstrId,
		/* [annotation][out] */
		IMMDevice **ppDevice);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RegisterEndpointNotificationCallback )(
		IMMDeviceEnumerator * This,
		/* [annotation][in] */
		IMMNotificationClient *pClient);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UnregisterEndpointNotificationCallback )(
		IMMDeviceEnumerator * This,
		/* [annotation][in] */
		IMMNotificationClient *pClient);

	END_INTERFACE
} IMMDeviceEnumeratorVtbl;
#define IMMDeviceEnumerator_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IMMDeviceEnumerator_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IMMDeviceEnumerator_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IMMDeviceEnumerator_EnumAudioEndpoints(This,dataFlow,dwStateMask,ppDevices)	\
	( (This)->lpVtbl -> EnumAudioEndpoints(This,dataFlow,dwStateMask,ppDevices) )
#define IMMDeviceEnumerator_GetDefaultAudioEndpoint(This,dataFlow,role,ppEndpoint)	\
	( (This)->lpVtbl -> GetDefaultAudioEndpoint(This,dataFlow,role,ppEndpoint) )
#define IMMDeviceEnumerator_GetDevice(This,pwstrId,ppDevice)	\
	( (This)->lpVtbl -> GetDevice(This,pwstrId,ppDevice) )
#define IMMDeviceEnumerator_RegisterEndpointNotificationCallback(This,pClient)	\
	( (This)->lpVtbl -> RegisterEndpointNotificationCallback(This,pClient) )
#define IMMDeviceEnumerator_UnregisterEndpointNotificationCallback(This,pClient)	\
	( (This)->lpVtbl -> UnregisterEndpointNotificationCallback(This,pClient) )

typedef struct IMMEndpoint
{
	CONST_VTBL struct IMMEndpointVtbl *lpVtbl;
} IMMEndpoint;
typedef struct IMMEndpointVtbl
{
	BEGIN_INTERFACE

	HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
		IMMEndpoint * This,
		/* [in] */ REFIID riid,
		/* [annotation][iid_is][out] */
		void **ppvObject);
	ULONG ( STDMETHODCALLTYPE *AddRef )(
		IMMEndpoint * This);
	ULONG ( STDMETHODCALLTYPE *Release )(
		IMMEndpoint * This);
	/* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDataFlow )(
		IMMEndpoint * This,
		/* [annotation][out] */
		EDataFlow *pDataFlow);

	END_INTERFACE
} IMMEndpointVtbl;
#define IMMEndpoint_QueryInterface(This,riid,ppvObject)	\
	( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )
#define IMMEndpoint_AddRef(This)	\
	( (This)->lpVtbl -> AddRef(This) )
#define IMMEndpoint_Release(This)	\
	( (This)->lpVtbl -> Release(This) )
#define IMMEndpoint_GetDataFlow(This,pDataFlow)	\
	( (This)->lpVtbl -> GetDataFlow(This,pDataFlow) )

#ifndef AUDCLNT_STREAMFLAGS_RATEADJUST
#define AUDCLNT_STREAMFLAGS_RATEADJUST 0x00100000
#define AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY 0x08000000
#define AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM 0x80000000
#endif

static const IID IID_IAudioClient = { 0x1cb9ad4c, 0xdbfa, 0x4c32,
	{ 0xb1, 0x78, 0xc2, 0xf5, 0x68, 0xa7, 0x03, 0xb2 } };
static const IID IID_IAudioRenderClient = { 0xf294acfc, 0x3146, 0x4483,
	{ 0xa7, 0xbf, 0xad, 0xdc, 0xa7, 0xc2, 0x60, 0xe2 } };
static const IID IID_IAudioCaptureClient = { 0xc8adbd64, 0xe71e, 0x48a0,
	{ 0xa4, 0xde, 0x18, 0x5c, 0x39, 0x5c, 0xd3, 0x17 } };
static const CLSID CLSID_MMDeviceEnumerator = { 0xbcde0395, 0xe52f, 0x467c,
	{ 0x8e, 0x3d, 0xc4, 0x57, 0x92, 0x91, 0x69, 0x2e } };
static const IID IID_IMMDeviceEnumerator = { 0xa95664d2, 0x9614, 0x4f35,
	{ 0xa7, 0x46, 0xde, 0x8d, 0xb6, 0x36, 0x17, 0xe6 } };
static const IID IID_IMMEndpoint = { 0x1be09788, 0x6894, 0x4089,
	{ 0x85, 0x86, 0x9a, 0x2a, 0x6c, 0x26, 0x5a, 0xc5 } };
static const PROPERTYKEY PKEY_Device_FriendlyName = { { 0xa45c254e, 0xdf1c, 0x4efd,
	{ 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, } }, 14 };
static const PROPERTYKEY PKEY_AudioEngine_DeviceFormat = { { 0xf19f064d, 0x82c, 0x4e27,
	{ 0xbc, 0x73, 0x68, 0x82, 0xa1, 0xbb, 0x8e, 0x4c, } }, 0 };
/*
static const IID IID_IMMNotificationClient = { 0x7991eec9, 0x7e89, 0x4d85,
	{ 0x83, 0x90, 0x6c, 0x70, 0x3c, 0xec, 0x60, 0xc0 } };
static const PROPERTYKEY PKEY_AudioEndpoint_GUID = { { 0x1da5d803, 0xd492, 0x4edd,
	{ 0x8c, 0x23, 0xe0, 0xc0, 0xff, 0xee, 0x7f, 0x0e, } }, 4 };
static const GUID KSDATAFORMAT_SUBTYPE_PCM = { 0x00000001, 0x0000, 0x0010,
	{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID KSDATAFORMAT_SUBTYPE_IEEE_FLOAT = { 0x00000003, 0x0000, 0x0010,
	{ 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
*/
static const IID IID_IAudioSessionControl = { 0xF4B1A599, 0x7266, 0x4319,
	{ 0xA8, 0xCA, 0xE7, 0x0A, 0xCB, 0x11, 0xE8, 0xCD } };

#define FACILITY_AUDCLNT                 2185
#define AUDCLNT_ERR(n) MAKE_HRESULT(SEVERITY_ERROR, FACILITY_AUDCLNT, n)
#define AUDCLNT_SUCCESS(n) MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_AUDCLNT, n)
#define AUDCLNT_E_NOT_INITIALIZED              AUDCLNT_ERR(0x001)
#define AUDCLNT_E_ALREADY_INITIALIZED          AUDCLNT_ERR(0x002)
#define AUDCLNT_E_WRONG_ENDPOINT_TYPE          AUDCLNT_ERR(0x003)
#define AUDCLNT_E_DEVICE_INVALIDATED           AUDCLNT_ERR(0x004)
#define AUDCLNT_E_NOT_STOPPED                  AUDCLNT_ERR(0x005)
#define AUDCLNT_E_BUFFER_TOO_LARGE             AUDCLNT_ERR(0x006)
#define AUDCLNT_E_OUT_OF_ORDER                 AUDCLNT_ERR(0x007)
#define AUDCLNT_E_UNSUPPORTED_FORMAT           AUDCLNT_ERR(0x008)
#define AUDCLNT_E_INVALID_SIZE                 AUDCLNT_ERR(0x009)
#define AUDCLNT_E_DEVICE_IN_USE                AUDCLNT_ERR(0x00a)
#define AUDCLNT_E_BUFFER_OPERATION_PENDING     AUDCLNT_ERR(0x00b)
#define AUDCLNT_E_THREAD_NOT_REGISTERED        AUDCLNT_ERR(0x00c)
#define AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED   AUDCLNT_ERR(0x00e)
#define AUDCLNT_E_ENDPOINT_CREATE_FAILED       AUDCLNT_ERR(0x00f)
#define AUDCLNT_E_SERVICE_NOT_RUNNING          AUDCLNT_ERR(0x010)
#define AUDCLNT_E_EVENTHANDLE_NOT_EXPECTED     AUDCLNT_ERR(0x011)
#define AUDCLNT_E_EXCLUSIVE_MODE_ONLY          AUDCLNT_ERR(0x012)
#define AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL AUDCLNT_ERR(0x013)
#define AUDCLNT_E_EVENTHANDLE_NOT_SET          AUDCLNT_ERR(0x014)
#define AUDCLNT_E_INCORRECT_BUFFER_SIZE        AUDCLNT_ERR(0x015)
#define AUDCLNT_E_BUFFER_SIZE_ERROR            AUDCLNT_ERR(0x016)
#define AUDCLNT_E_CPUUSAGE_EXCEEDED            AUDCLNT_ERR(0x017)
#define AUDCLNT_E_BUFFER_ERROR                 AUDCLNT_ERR(0x018)
#define AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED      AUDCLNT_ERR(0x019)
#define AUDCLNT_E_INVALID_DEVICE_PERIOD        AUDCLNT_ERR(0x020)
#define AUDCLNT_E_INVALID_STREAM_FLAG          AUDCLNT_ERR(0x021)
#define AUDCLNT_E_ENDPOINT_OFFLOAD_NOT_CAPABLE AUDCLNT_ERR(0x022)
#define AUDCLNT_E_OUT_OF_OFFLOAD_RESOURCES     AUDCLNT_ERR(0x023)
#define AUDCLNT_E_OFFLOAD_MODE_ONLY            AUDCLNT_ERR(0x024)
#define AUDCLNT_E_NONOFFLOAD_MODE_ONLY         AUDCLNT_ERR(0x025)
#define AUDCLNT_E_RESOURCES_INVALIDATED        AUDCLNT_ERR(0x026)
#define AUDCLNT_E_RAW_MODE_UNSUPPORTED         AUDCLNT_ERR(0x027)
#define AUDCLNT_E_ENGINE_PERIODICITY_LOCKED    AUDCLNT_ERR(0x028)
#define AUDCLNT_E_ENGINE_FORMAT_LOCKED         AUDCLNT_ERR(0x029)
#define AUDCLNT_E_HEADTRACKING_ENABLED         AUDCLNT_ERR(0x030)
#define AUDCLNT_E_HEADTRACKING_UNSUPPORTED     AUDCLNT_ERR(0x040)
#define AUDCLNT_E_EFFECT_NOT_AVAILABLE         AUDCLNT_ERR(0x041)
#define AUDCLNT_E_EFFECT_STATE_READ_ONLY       AUDCLNT_ERR(0x042)
#define AUDCLNT_S_BUFFER_EMPTY                 AUDCLNT_SUCCESS(0x001)
#define AUDCLNT_S_THREAD_ALREADY_REGISTERED    AUDCLNT_SUCCESS(0x002)
#define AUDCLNT_S_POSITION_STALLED             AUDCLNT_SUCCESS(0x003)

#define WSAUDIOWAS_NUMSEGMENTS 8

/*===========================================================================
					 WAS HELPERS
===========================================================================*/
/*===========================================================================
					 WAS IMPLEMENTATION
===========================================================================*/
#if defined(__cplusplus)
extern "C" {
#endif
/* In C++ Win32 IID Params are ref (&) instead of p (*)  */
#ifndef WSIIDPARAM
	#ifdef __cplusplus
		#define WSIIDPARAM(x) x
	#else
		#define WSIIDPARAM(x) &(x)
	#endif
#endif
typedef HRESULT (WINAPI* LPCoCreateInstance)(REFCLSID rclsid,
	LPUNKNOWN pUnkOuter, DWORD dwClsContext,
	REFIID riid, LPVOID FAR* ppv);
typedef HRESULT (WINAPI* LPCoInitialize)(LPVOID pvReserved);
typedef void (WINAPI* LPCoUninitialize)(void);
typedef void(__stdcall* LPCoTaskMemFree)(LPVOID pv);
typedef HRESULT (WINAPI* LPPropVariantClear) (PROPVARIANT * pvar);

char* WSTRToSTR(LPCWSTR w);

void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	IMMDevice* dev;
	IMMDeviceCollection* collection;
	IMMDeviceEnumerator* enumerator;
	HRESULT hr;
	UINT devcount;
	UINT i;
	#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
		HINSTANCE ole32;
		LPCoCreateInstance pfCoCreateInstance;
		LPCoInitialize pfCoInitialize;
		LPCoUninitialize pfCoUninitialize;
		LPCoTaskMemFree pfCoTaskMemFree;
		LPPropVariantClear pfPropVariantClear;
	#endif
	enumparams->numdevices = 0;
	#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
		ole32 = LoadLibraryA("ole32.dll");
		pfCoInitialize =
			(LPCoInitialize) GetProcAddress(ole32, "CoInitialize");
		pfCoUninitialize =
			(LPCoUninitialize) GetProcAddress(ole32, "CoUninitialize");
		pfCoCreateInstance =
			(LPCoCreateInstance) GetProcAddress(ole32, "CoCreateInstance");
		pfCoTaskMemFree =
			(LPCoTaskMemFree) GetProcAddress(ole32, "CoTaskMemFree");
		pfPropVariantClear =
			(LPPropVariantClear) GetProcAddress(ole32, "PropVariantClear");
		hr = (*pfCoInitialize)(NULL);
	#else
		hr = CoInitialize(NULL);
	#endif
	if(FAILED(hr))
	{
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			FreeLibrary(ole32);
		#endif
		return;
	}
	#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
		hr = (*pfCoCreateInstance)(WSIIDPARAM(CLSID_MMDeviceEnumerator), NULL,
			 CLSCTX_INPROC_SERVER, WSIIDPARAM(IID_IMMDeviceEnumerator),
			 (LPVOID*)&enumerator);
	#else
		hr = CoCreateInstance(WSIIDPARAM(CLSID_MMDeviceEnumerator), NULL,
			 CLSCTX_INPROC_SERVER, WSIIDPARAM(IID_IMMDeviceEnumerator),
			 (LPVOID*)&enumerator);
	#endif
	if(FAILED(hr)) /* WASAPI is Vista+ so bound to happen */
	{
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(ole32);
		#else
			CoUninitialize();
		#endif
		return;
	}

	hr = IMMDeviceEnumerator_EnumAudioEndpoints(enumerator, eAll,
		DEVICE_STATE_ACTIVE, &collection);
	if(FAILED(hr))
	{
		IMMDeviceEnumerator_Release(enumerator);
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(ole32);
		#else
			CoUninitialize();
		#endif
		return;
	}

	hr = IMMDeviceCollection_GetCount(collection, &devcount);
	if(FAILED(hr))
	{
		IMMDeviceCollection_Release(collection);
		IMMDeviceEnumerator_Release(enumerator);
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(ole32);
		#else
			CoUninitialize();
		#endif
		return;
	}

	enumparams->devices = WSALLOC(WSAUDIODEVICE, devcount);
	if(!enumparams->devices)
	{
		IMMDeviceCollection_Release(collection);
		IMMDeviceEnumerator_Release(enumerator);
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(ole32);
		#else
			CoUninitialize();
		#endif
		return;
	}

	for(i = 0; i < devcount; ++i)
	{
		LPWSTR wname;
		IPropertyStore* propstore;
		IMMEndpoint* endpoint;
		hr = IMMDeviceCollection_Item(collection, i, &dev);
		if(FAILED(hr))
			continue;
		hr = IMMDevice_GetId(dev, &wname);
		if(FAILED(hr))
			continue;
		enumparams->devices[i].name = WSTRToSTR(wname);
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoTaskMemFree)(wname);
		#else
			CoTaskMemFree(wname);
		#endif
		hr = IMMDevice_OpenPropertyStore(dev, STGM_READ, &propstore);
		if(SUCCEEDED(hr))
		{
			PROPVARIANT v;
			PropVariantInit(&v);
			hr = IPropertyStore_GetValue(propstore, WSIIDPARAM(PKEY_Device_FriendlyName), &v);
			if (SUCCEEDED(hr))
			{
				WSFREE(enumparams->devices[i].name);
				enumparams->devices[i].name = WSTRToSTR(v.pwszVal);
			}
			#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
				(*pfPropVariantClear)(&v);
			#else
				PropVariantClear(&v);
			#endif
			if (SUCCEEDED(IPropertyStore_GetValue(propstore,
				WSIIDPARAM(PKEY_AudioEngine_DeviceFormat), &v)))
			{
				WSWaveFormatExToAudioFormat( /* == WAVEFORMATEXTENSIBLE */
					(WAVEFORMATEX*)v.blob.pBlobData,
					&enumparams->devices[i].maxformat);
			}
			#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
				(*pfPropVariantClear)(&v);
			#else
				PropVariantClear(&v);
			#endif
			/*if (SUCCEEDED(IPropertyStore_GetValue(propstore, &PKEY_AudioEndpoint_GUID, &v)))
			{
				CLSIDFromString(v.pwszVal, pThis->guid);
			}
			(*pfPropVariantClear)(&var);*/
			IPropertyStore_Release(propstore);
		}

		hr = IMMDevice_QueryInterface(dev,
			WSIIDPARAM(IID_IMMEndpoint), (void**)&endpoint);
		if(SUCCEEDED(hr))
		{
			EDataFlow dataflow = 0;
			hr = IMMEndpoint_GetDataFlow(endpoint, &dataflow);
			WSASSERT(SUCCEEDED(hr));
			/*if(SUCCEEDED(hr))*/
			/**/
			/*{*/
			if(dataflow == eAll)
				enumparams->devices[i].flags = WSAUDIO_INPUT | WSAUDIO_OUTPUT;
			else if(dataflow == eCapture)
				enumparams->devices[i].flags = WSAUDIO_INPUT;
			else
				enumparams->devices[i].flags = 0;
			/*}*/
			/*else*/
			/*{}*/
			IMMEndpoint_Release(endpoint);
		}
		IMMDevice_Release(dev);
		++enumparams->numdevices;
	}

	IMMDeviceCollection_Release(collection);
	IMMDeviceEnumerator_Release(enumerator);
	#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
		(*pfCoUninitialize)();
		FreeLibrary(ole32);
	#else
		CoUninitialize();
	#endif
}

wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	IMMDevice* dev;
	IMMDeviceEnumerator* enumerator;
	HRESULT hr;
	EDataFlow dataflow;
	WSAUDCLNT_SHAREMODE sharemode;
	WAVEFORMATEX wfx;
	DWORD streamflags;
	#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
		LPCoCreateInstance pfCoCreateInstance;
		LPCoInitialize pfCoInitialize;
		LPCoUninitialize pfCoUninitialize;
		/*LPPropVariantClear pfPropVariantClear;*/

		pThis->mOle32 = LoadLibraryA("ole32.dll");
		pfCoInitialize = (LPCoInitialize)
			GetProcAddress(pThis->mOle32, "CoInitialize");
		pfCoUninitialize = (LPCoUninitialize)
			GetProcAddress(pThis->mOle32, "CoUninitialize");
		pfCoCreateInstance = (LPCoCreateInstance)
			GetProcAddress(pThis->mOle32, "CoCreateInstance");
		/*pfPropVariantClear = (LPPropVariantClear)
			GetProcAddress(pThis->mOle32, "PropVariantClear");*/
		hr = (*pfCoInitialize)(NULL);
	#else
		hr = CoInitialize(NULL);
	#endif
	if(FAILED(hr))
	{
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(pThis->mOle32);
		#else
			CoUninitialize();
		#endif
		return FALSE;
	}
	#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
		hr = (*pfCoCreateInstance)(WSIIDPARAM(CLSID_MMDeviceEnumerator), NULL,
			 CLSCTX_INPROC_SERVER, WSIIDPARAM(IID_IMMDeviceEnumerator),
			 (LPVOID*)&enumerator);
	#else
		hr = CoCreateInstance(WSIIDPARAM(CLSID_MMDeviceEnumerator), NULL,
			 CLSCTX_INPROC_SERVER, WSIIDPARAM(IID_IMMDeviceEnumerator),
			 (LPVOID*)&enumerator);
	#endif
	if(FAILED(hr)) /* WASAPI is Vista+ so bound to happen */
	{
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(pThis->mOle32);
		#else
			CoUninitialize();
		#endif
		return FALSE;
	}

	if(spec->flags & (WSAUDIO_INPUT|WSAUDIO_OUTPUT))
		dataflow = eAll;
	else if(spec->flags & (WSAUDIO_INPUT))
		dataflow = eCapture;
	else
		dataflow = eRender;
	/*
		IMMDeviceEnumerator_GetDevice(enumerator, name, &dev);
		Wait 8000 milliseconds on E_NOTFOUND
	*/
	hr = IMMDeviceEnumerator_GetDefaultAudioEndpoint(enumerator, dataflow,
		eConsole, &dev);
	if(SUCCEEDED(hr))
		hr = IMMDevice_Activate(dev, WSIIDPARAM(IID_IAudioClient),
			CLSCTX_ALL, NULL, (void**)&pThis->client);
	else
		IMMDevice_Release(dev);
	/* The following commented out code gets the device format if needed */
	/*
	if(SUCCEEDED(hr))
	{
		IPropertyStore* propstore;
		hr = IMMDevice_OpenPropertyStore(dev, STGM_READ, &propstore);
		if(SUCCEEDED(hr))
		{
			PROPVARIANT v;
			PropVariantInit(&v);
			hr = IPropertyStore_GetValue(propstore,
				&PKEY_AudioEngine_DeviceFormat, &v);
			if (SUCCEEDED(hr))
			{
				WSWaveFormatExToAudioFormat(
					(WAVEFORMATEX*)v.blob.pBlobData,
					&devformat);
			}
			(*pfPropVariantClear)(&v);
			IPropertyStore_Release(propstore);
		}
	}
	*/
	IMMDeviceEnumerator_Release(enumerator);
	if(SUCCEEDED(hr))
	{
		sharemode = WSAUDCLNT_SHAREMODE_SHARED;
		streamflags = (AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY);
		/*streamflags |= AUDCLNT_STREAMFLAGS_EVENTCALLBACK;*/
		WSAudioFormatToWaveFormatEx(&spec->format.mSuper, &wfx);
		hr = IAudioClient_Initialize(pThis->client, sharemode,
			streamflags, 0, 0, &wfx, NULL);
	}
	if(FAILED(hr))
	{
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(pThis->mOle32);
		#else
			CoUninitialize();
		#endif
		return FALSE;
	}

	hr = IAudioClient_GetBufferSize(pThis->client,
		(UINT32*)&pThis->mReqFrames); /* uint32_t is ulong on MWERKS */

	/*if (SUCCEEDED(hr))
		hr = IAudioClient_SetEventHandle(pThis->client, hEvent);
	*/

	/*GetDevicePeriod / 10000.0 == milliseconds*/
	if(SUCCEEDED(hr))
	{
		if(spec->flags & WSAUDIO_INPUT)
			hr = IAudioClient_GetService(pThis->client,
				WSIIDPARAM(IID_IAudioCaptureClient),
				(void**)&pThis->capture);
		if(SUCCEEDED(hr)
			&& (!(pThis->mFlags & WSAUDIO_INPUT)
				|| (pThis->mFlags & WSAUDIO_OUTPUT)))
		{
			hr = IAudioClient_GetService(pThis->client,
				WSIIDPARAM(IID_IAudioRenderClient), (void**)&pThis->render);
			if(FAILED(hr))
			{
				if((spec->flags & WSAUDIO_INPUT) && pThis->capture)
					IAudioCaptureClient_Release(pThis->capture);
			}
		}
	}

	if(FAILED(hr))
	{
		IAudioClient_Release(pThis->client);
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(pThis->mOle32);
		#else
			CoUninitialize();
		#endif
		return FALSE;
	}
	/*IMMDeviceEnumerator_RegisterEndpointNotificationCallback(enumerator,
		(IMMNotificationClient*)&pThis->mNotifyclient);*/
	/* divide the total frames into a segment */
	pThis->mBlockAlign =
		WSAudioFormat_GetBlockAlign(&spec->format.mSuper);
	pThis->mReqFrames /= WSAUDIOWAS_NUMSEGMENTS;
	pThis->mReqSize = pThis->mReqFrames
		* pThis->mBlockAlign;
	pThis->mFillProc = spec->proc;
	pThis->mUserData = spec->procuserdata;
	pThis->mFlags = spec->flags;
	return TRUE;
}

void WSAudio_Destroy(WSAUDIO* pThis)
{
	#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
		LPCoUninitialize pfCoUninitialize;
		pfCoUninitialize =
			(LPCoUninitialize) GetProcAddress(pThis->mOle32, "CoUninitialize");
	#endif
	/*IMMDeviceEnumerator_UnregisterEndpointNotificationCallback(enumerator,
		(IMMNotificationClient*)&pThis->mNotifyclient);*/
	IAudioClient_Stop(pThis->client);
	IAudioClient_Release(pThis->client);
	if(!(pThis->mFlags & WSAUDIO_INPUT) || (pThis->mFlags & WSAUDIO_OUTPUT))
		IAudioRenderClient_Release(pThis->render);
	if(pThis->mFlags & WSAUDIO_INPUT)
		IAudioCaptureClient_Release(pThis->capture);
	#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
		(*pfCoUninitialize)();
		FreeLibrary(pThis->mOle32);
	#else
		CoUninitialize();
	#endif
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright)
{
	const IID IID_IAudioStreamVolume = { 0x93014887, 0x242D, 0x4068,
	{ 0x8A, 0x15, 0xCF, 0x5E, 0x93, 0xB9, 0x0F, 0xE3 } };
	IAudioStreamVolume* audiostreamvolume;
	HRESULT hr;
	UINT32 chcount;
	hr = IAudioClient_GetService(pThis->client,
		WSIIDPARAM(IID_IAudioStreamVolume), (void**)&audiostreamvolume);
	if(SUCCEEDED(hr))
		hr = IAudioStreamVolume_GetChannelCount(audiostreamvolume, &chcount);
	if(SUCCEEDED(hr) && chcount > 1)
	{
		hr = IAudioStreamVolume_SetChannelVolume(audiostreamvolume,
			0, panleft / ((float)INT_MAX));
		if(SUCCEEDED(hr))
			hr = IAudioStreamVolume_SetChannelVolume(audiostreamvolume,
					1, panleft / ((float)INT_MAX));
	}
	else
		hr = E_FAIL;
	return SUCCEEDED(hr);
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{
	const IID IID_IAudioStreamVolume = { 0x93014887, 0x242D, 0x4068,
	{ 0x8A, 0x15, 0xCF, 0x5E, 0x93, 0xB9, 0x0F, 0xE3 } };
	IAudioStreamVolume* audiostreamvolume;
	HRESULT hr;
	UINT32 chcount;
	float* volumes;
	hr = IAudioClient_GetService(pThis->client,
		WSIIDPARAM(IID_IAudioStreamVolume), (void**)&audiostreamvolume);
	if(SUCCEEDED(hr))
		hr = IAudioStreamVolume_GetChannelCount(audiostreamvolume, &chcount);
	if(SUCCEEDED(hr))
	{
		volumes = WSALLOC(float, chcount);
		if(volumes)
		{
			DWORD i;
			for(i = 0; i < chcount; ++i)
				volumes[i] = volume / ((float)INT_MAX);
			hr = IAudioStreamVolume_SetAllVolumes(audiostreamvolume,
				chcount, volumes);
			WSFREE(volumes);
		}
		else
			hr = E_OUTOFMEMORY;
	}
	return SUCCEEDED(hr);
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	return 0;
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	return INT_MAX;
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	HRESULT hr = IAudioClient_Stop(pThis->client);
	return SUCCEEDED(hr);
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	HRESULT hr = IAudioClient_Start(pThis->client);
	return SUCCEEDED(hr);
}
void WSAudio_Process(WSAUDIO* pThis); /* forward declare */
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	/* deal with AUDCLNT_S_BUFFER_EMPTY? */
	if(!(pThis->mFlags & WSAUDIO_INPUT)
		|| (pThis->mFlags & WSAUDIO_OUTPUT))
		WSAudio_Process(pThis); /* fill output buffer */
	return WSAudio_Resume(pThis);
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{
	IAudioSessionControl* audiosessioncontrol;
	HRESULT hr;
	WSAudioSessionState state = WSAudioSessionStateInactive;
	hr = IAudioClient_GetService(pThis->client,
		WSIIDPARAM(IID_IAudioSessionControl), (void**)&audiosessioncontrol);
	if(SUCCEEDED(hr))
		hr = IAudioSessionControl_GetState(audiosessioncontrol, &state);
	return state == WSAudioSessionStateActive;
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	UINT32 padding;
	HRESULT hr;
	hr = IAudioClient_GetCurrentPadding(pThis->client, &padding);
	if(FAILED(hr))
		return 0;
	return padding * pThis->mBlockAlign;
}
wsbool WSAudioHasFullDuplex() { return TRUE; }
void WSAudio_Process(WSAUDIO* pThis)
{
	BYTE* buffer;
	HRESULT hr;
	if(pThis->mFlags & WSAUDIO_INPUT)
	{
		DWORD flags;
		UINT32 numframes;
		hr = IAudioCaptureClient_GetBuffer(pThis->capture,
			&buffer, &numframes, &flags, NULL, NULL);
	   /*AUDCLNT_S_BUFFER_EMPTY*/
		if(SUCCEEDED(hr))
		{
			(*pThis->mFillProc)(pThis->mUserData, buffer,
				numframes * pThis->mBlockAlign);
			/*AUDCLNT_BUFFERFLAGS_SILENT*/
			hr = IAudioCaptureClient_ReleaseBuffer(pThis->capture,
				numframes);
		}
	}
	if(!(pThis->mFlags & WSAUDIO_INPUT) || (pThis->mFlags & WSAUDIO_OUTPUT))
	{
		hr = IAudioRenderClient_GetBuffer(pThis->render,
			pThis->mReqFrames, &buffer);
	   /*AUDCLNT_E_BUFFER_TOO_LARGE*/
		if(SUCCEEDED(hr))
		{
			(*pThis->mFillProc)(pThis->mUserData, buffer, pThis->mReqSize);
			/*AUDCLNT_BUFFERFLAGS_SILENT*/
			hr = IAudioRenderClient_ReleaseBuffer(pThis->render,
				pThis->mReqFrames, 0);
		}
	}
}
#if defined(__cplusplus)
} /* end extern "C" */
#endif
/*---------------------------------------------------------------------------
					 #     # ### #     #  #####   #####
					 #  #  #  #  ##    # #     # #     #
					 #  #  #  #  # #   #       #       #
					 #  #  #  #  #  #  #  #####   #####
					 #  #  #  #  #   # #       # #
					 #  #  #  #  #    ## #     # #
					  ## ##  ### #     #  #####  #######

			 #     #    #    #     # ######  ### #######  #####
			  #   #    # #   #     # #     #  #  #     # #     #
			   # #    #   #  #     # #     #  #  #     #       #
				#    #     # #     # #     #  #  #     #  #####
			   # #   ####### #     # #     #  #  #     # #
			  #   #  #     # #     # #     #  #  #     # #
			 #     # #     #  #####  ######  ### ####### #######

https://github.com/tsherif/xaudio2-c-demo/blob/master/xaudio2-c-demo.c#L93
---------------------------------------------------------------------------*/
/* 1910 == MSVC 2017+ */
#elif defined(_WIN32) && !defined(WSAUDIOSMOPT)

#if 0
	#include <xaudio2.h>
#else
#include <windows.h> /* Win32 functions/types */
#ifndef SPEAKER_FRONT_LEFT
	#define SPEAKER_FRONT_LEFT				0x1
	#define SPEAKER_FRONT_RIGHT				0x2
	#define SPEAKER_FRONT_CENTER			0x4
	#define SPEAKER_LOW_FREQUENCY			0x8
	#define SPEAKER_BACK_LEFT				0x10
	#define SPEAKER_BACK_RIGHT				0x20
	#define SPEAKER_FRONT_LEFT_OF_CENTER	0x40
	#define SPEAKER_FRONT_RIGHT_OF_CENTER	0x80
	#define SPEAKER_BACK_CENTER				0x100
	#define SPEAKER_SIDE_LEFT				0x200
	#define SPEAKER_SIDE_RIGHT				0x400
	#define SPEAKER_TOP_CENTER				0x800
	#define SPEAKER_TOP_FRONT_LEFT			0x1000
	#define SPEAKER_TOP_FRONT_CENTER		0x2000
	#define SPEAKER_TOP_FRONT_RIGHT			0x4000
	#define SPEAKER_TOP_BACK_LEFT			0x8000
	#define SPEAKER_TOP_BACK_CENTER			0x10000
	#define SPEAKER_TOP_BACK_RIGHT			0x20000
#endif


#define XAUDIO2_ANY_PROCESSOR 0XFFFFFFFF


#define AudioCategory_Other 0
#define AudioCategory_ForegroundOnlyMedia 1
#if NTDDI_VERSION < NTDDI_WINTHRESHOLD
	#define AudioCategory_BackgroundCapableMedia 2
#endif
#define AudioCategory_Communications 3
#define AudioCategory_Alerts 4
#define AudioCategory_SoundEffects 5
#define AudioCategory_GameEffects 6
#define AudioCategory_GameMedia 7
#define AudioCategory_GameChat 8
#define AudioCategory_Speech 9
#define AudioCategory_Movie 10
#define AudioCategory_Media 11
#if NTDDI_VERSION >= NTDDI_WIN10_FE
	#define AudioCategory_FarFieldSpeech 12
	#define AudioCategory_UniformSpeech 13
	#define AudioCategory_VoiceTyping 14
#endif
typedef int AUDIO_STREAM_CATEGORY;
typedef UINT32 XAUDIO2_PROCESSOR;
#define XAUDIO2_DEBUG_ENGINE		0x0001
#define XAUDIO2_COMMIT_NOW			0
#define XAUDIO2_COMMIT_ALL			0
#define XAUDIO2_NO_LOOP_REGION		0
#define XAUDIO2_LOOP_INFINITE		255
#define XAUDIO2_DEFAULT_CHANNELS	0
#define XAUDIO2_DEFAULT_SAMPLERATE	0
#define XAUDIO2_USE_DEFAULT_PROCESSOR 0x00000000
#define XAUDIO2_DEFAULT_PROCESSOR 0x00000001

#pragma pack(push, 1)
typedef struct XAUDIO2_VOICE_DETAILS
{
	UINT32 CreationFlags;
	UINT32 ActiveFlags;
	UINT32 InputChannels;
	UINT32 InputSampleRate;
} XAUDIO2_VOICE_DETAILS;
/*typedef struct XAUDIO2_BUFFER
{
	UINT32 Flags;
	UINT32 AudioBytes;
	const BYTE* pAudioData;
	UINT32 PlayBegin;
	UINT32 PlayLength;
	UINT32 LoopBegin;
	UINT32 LoopLength;
	UINT32 LoopCount;
	void* pContext;
} XAUDIO2_BUFFER;*/
typedef struct XAUDIO2_VOICE_STATE
{
	void* pCurrentBufferContext;
	UINT32 BuffersQueued;
	UINT64 SamplesPlayed;
} XAUDIO2_VOICE_STATE;
typedef struct XAUDIO2_DEBUG_CONFIGURATION
{
	UINT32 TraceMask;
	UINT32 BreakMask;
	BOOL LogThreadID;
	BOOL LogFileline;
	BOOL LogFunctionName;
	BOOL LogTiming;
} XAUDIO2_DEBUG_CONFIGURATION;
#pragma pack(pop)

/* Unused types */
typedef struct XAUDIO2_VOICE_SENDS XAUDIO2_VOICE_SENDS;
typedef struct XAUDIO2_EFFECT_CHAIN XAUDIO2_EFFECT_CHAIN;
typedef struct XAUDIO2_FILTER_PARAMETERS XAUDIO2_FILTER_PARAMETERS;
typedef struct XAUDIO2_BUFFER_WMA XAUDIO2_BUFFER_WMA;
typedef struct XAUDIO2_PERFORMANCE_DATA XAUDIO2_PERFORMANCE_DATA;
typedef struct IXAudio2SubmixVoice IXAudio2SubmixVoice;
/* End Unused types */
#define XAUDIO2_LOG_ERRORS     0x0001
#define XAUDIO2_LOG_WARNINGS   0x0002
#define XAUDIO2_LOG_INFO       0x0004
#define XAUDIO2_LOG_DETAIL     0x0008
#define XAUDIO2_LOG_API_CALLS  0x0010
#define XAUDIO2_LOG_FUNC_CALLS 0x0020
#define XAUDIO2_LOG_TIMING     0x0040
#define XAUDIO2_LOG_LOCKS      0x0080
#define XAUDIO2_LOG_MEMORY     0x0100
#define XAUDIO2_LOG_STREAMING  0x1000

#define DECLARE_INTERFACE_C(iface) typedef interface iface { \
	struct iface##Vtbl FAR* lpVtbl; \
	} iface; \
	typedef struct iface##Vtbl iface##Vtbl; \
	struct iface##Vtbl
#define STDMETHOD_C_(ret, name) ret (__stdcall *name)
#define STDMETHOD_C(name) HRESULT (__stdcall *name)


/*DECLARE_INTERFACE_C(IXAudio2VoiceCallback)
{
	BEGIN_INTERFACE
	STDMETHOD_C_(void, OnVoiceProcessingPassStart) (IXAudio2VoiceCallback* This,
		UINT32 BytesRequired);
	STDMETHOD_C_(void, OnVoiceProcessingPassEnd) (IXAudio2VoiceCallback* This);
	STDMETHOD_C_(void, OnStreamEnd) (IXAudio2VoiceCallback* This);
	STDMETHOD_C_(void, OnBufferStart) (IXAudio2VoiceCallback* This,
		void* pBufferContext);
	STDMETHOD_C_(void, OnBufferEnd) (IXAudio2VoiceCallback* This,
		void* pBufferContext);
	STDMETHOD_C_(void, OnLoopEnd) (IXAudio2VoiceCallback* This,
		void* pBufferContext);
	STDMETHOD_C_(void, OnVoiceError) (IXAudio2VoiceCallback* This,
		void* pBufferContext, HRESULT Error);
	END_INTERFACE
};*/
DECLARE_INTERFACE_C(IXAudio2EngineCallback)
{
	BEGIN_INTERFACE
	STDMETHOD_C_(void, OnProcessingPassStart) (IXAudio2EngineCallback* This) ;
	STDMETHOD_C_(void, OnProcessingPassEnd) (IXAudio2EngineCallback* This);
	STDMETHOD_C_(void, OnCriticalError) (IXAudio2EngineCallback* This, HRESULT Error);
	END_INTERFACE
};
DECLARE_INTERFACE_C(IXAudio2Voice)
{
	BEGIN_INTERFACE
	STDMETHOD_C_(void, GetVoiceDetails) (IXAudio2Voice* This,
		XAUDIO2_VOICE_DETAILS* pVoiceDetails);
	STDMETHOD_C(SetOutputVoices) (IXAudio2Voice* This,
		const XAUDIO2_VOICE_SENDS* pSendList);
	STDMETHOD_C(SetEffectChain) (IXAudio2Voice* This,
		const XAUDIO2_EFFECT_CHAIN* pEffectChain);
	STDMETHOD_C(EnableEffect) (IXAudio2Voice* This, UINT32 EffectIndex,
							 UINT32 OperationSet);
	STDMETHOD_C(DisableEffect) (IXAudio2Voice* This, UINT32 EffectIndex,
							  UINT32 OperationSet);
	STDMETHOD_C_(void, GetEffectState) (IXAudio2Voice* This,
		UINT32 EffectIndex, BOOL* pEnabled);
	STDMETHOD_C(SetEffectParameters) (IXAudio2Voice* This, UINT32 EffectIndex,
									const void* pParameters,
									UINT32 ParametersByteSize,
									UINT32 OperationSet);
	STDMETHOD_C(GetEffectParameters) (IXAudio2Voice* This, UINT32 EffectIndex,
									void* pParameters,
									UINT32 ParametersByteSize);
	STDMETHOD_C(SetFilterParameters) (IXAudio2Voice* This,
		const XAUDIO2_FILTER_PARAMETERS* pParameters,
									UINT32 OperationSet);
	STDMETHOD_C_(void, GetFilterParameters) (IXAudio2Voice* This,
		XAUDIO2_FILTER_PARAMETERS* pParameters);
	STDMETHOD_C(SetOutputFilterParameters) (IXAudio2Voice* This,
		IXAudio2Voice* pDestinationVoice,
		const XAUDIO2_FILTER_PARAMETERS* pParameters, UINT32 OperationSet);
	STDMETHOD_C_(void, GetOutputFilterParameters) (IXAudio2Voice* This,
		IXAudio2Voice* pDestinationVoice,
		XAUDIO2_FILTER_PARAMETERS* pParameters);
	STDMETHOD_C(SetVolume) (IXAudio2Voice* This, float Volume,
						  UINT32 OperationSet);
	STDMETHOD_C_(void, GetVolume) (IXAudio2Voice* This, float* pVolume);
	STDMETHOD_C(SetChannelVolumes) (IXAudio2Voice* This, UINT32 Channels,
		const float* pVolumes,
								  UINT32 OperationSet);
	STDMETHOD_C_(void, GetChannelVolumes) (IXAudio2Voice* This,
		UINT32 Channels, float* pVolumes);
	STDMETHOD_C(SetOutputMatrix) (IXAudio2Voice* This,
		IXAudio2Voice* pDestinationVoice,
		UINT32 SourceChannels, UINT32 DestinationChannels,
		const float* pLevelMatrix,
		UINT32 OperationSet);
	STDMETHOD_C_(void, GetOutputMatrix) (IXAudio2Voice* This,
		IXAudio2Voice* pDestinationVoice,
		UINT32 SourceChannels, UINT32 DestinationChannels,
		float* pLevelMatrix);
	STDMETHOD_C_(void, DestroyVoice) (IXAudio2Voice* This);
	END_INTERFACE
};
#define IXAudio2Voice_GetVoiceDetails(This,pVoiceDetails) \
	((This)->lpVtbl->GetVoiceDetails(This,pVoiceDetails))
#define IXAudio2Voice_SetOutputVoices(This,pSendList) \
	((This)->lpVtbl->SetOutputVoices(This,pSendList))
#define IXAudio2Voice_SetEffectChain(This,pEffectChain) \
	((This)->lpVtbl->SetEffectChain(This,pEffectChain))
#define IXAudio2Voice_EnableEffect(This,EffectIndex,OperationSet) \
	((This)->lpVtbl->EnableEffect(This,EffectIndex,OperationSet))
#define IXAudio2Voice_DisableEffect(This,EffectIndex,OperationSet) \
	((This)->lpVtbl->DisableEffect(This,EffectIndex,OperationSet))
#define IXAudio2Voice_GetEffectState(This,EffectIndex,pEnabled) \
	((This)->lpVtbl->GetEffectState(This,EffectIndex,pEnabled))
#define IXAudio2Voice_SetEffectParameters(This,EffectIndex,pParameters,\
	ParametersByteSize, OperationSet) \
	((This)->lpVtbl->SetEffectParameters(This,EffectIndex,pParameters,\
		ParametersByteSize,OperationSet))
#define IXAudio2Voice_GetEffectParameters(This,EffectIndex,pParameters,\
	ParametersByteSize) \
	((This)->lpVtbl->GetEffectParameters(This,EffectIndex,pParameters,\
		ParametersByteSize))
#define IXAudio2Voice_SetFilterParameters(This,pParameters,OperationSet) \
	((This)->lpVtbl->SetFilterParameters(This,pParameters,OperationSet))
#define IXAudio2Voice_GetFilterParameters(This,pParameters) \
	((This)->lpVtbl->GetFilterParameters(This,pParameters))
#define IXAudio2Voice_SetOutputFilterParameters(This,pDestinationVoice,\
	pParameters,OperationSet) \
	((This)->lpVtbl->SetOutputFilterParameters(This,pDestinationVoice,\
		pParameters,OperationSet))
#define IXAudio2Voice_GetOutputFilterParameters(This,pDestinationVoice,\
	pParameters) \
	((This)->lpVtbl->GetOutputFilterParameters(This,pDestinationVoice,\
		pParameters))
#define IXAudio2Voice_SetVolume(This,Volume,OperationSet) \
	((This)->lpVtbl->SetVolume(This,Volume,OperationSet))
#define IXAudio2Voice_GetVolume(This,pVolume) \
	((This)->lpVtbl->GetVolume(This,pVolume))
#define IXAudio2Voice_SetChannelVolumes(This,Channels,pVolumes,OperationSet) \
	((This)->lpVtbl->SetChannelVolumes(This,Channels,pVolumes,OperationSet))
#define IXAudio2Voice_GetChannelVolumes(This,Channels,pVolumes) \
	((This)->lpVtbl->GetChannelVolumes(This,Channels,pVolumes))
#define IXAudio2Voice_SetOutputMatrix(This,pDestinationVoice,SourceChannels,\
	DestinationChannels,pLevelMatrix,OperationSet) \
	((This)->lpVtbl->SetOutputMatrix(This,pDestinationVoice,\
		SourceChannels,DestinationChannels,pLevelMatrix,OperationSet))
#define IXAudio2Voice_GetOutputMatrix(This,pDestinationVoice,SourceChannels,\
	DestinationChannels,pLevelMatrix) \
	((This)->lpVtbl->GetOutputMatrix(This,pDestinationVoice,SourceChannels,\
		DestinationChannels,pLevelMatrix))
#define IXAudio2Voice_DestroyVoice(This) ((This)->lpVtbl->DestroyVoice(This))

DECLARE_INTERFACE_C(IXAudio2SourceVoice)
{
	BEGIN_INTERFACE
	STDMETHOD_C_(void, GetVoiceDetails) (IXAudio2SourceVoice* This,
		XAUDIO2_VOICE_DETAILS* pVoiceDetails);
	STDMETHOD_C(SetOutputVoices) (IXAudio2SourceVoice* This,
		const XAUDIO2_VOICE_SENDS* pSendList);
	STDMETHOD_C(SetEffectChain) (IXAudio2SourceVoice* This,
		const XAUDIO2_EFFECT_CHAIN* pEffectChain);
	STDMETHOD_C(EnableEffect) (IXAudio2SourceVoice* This, UINT32 EffectIndex,
							 UINT32 OperationSet);
	STDMETHOD_C(DisableEffect) (IXAudio2SourceVoice* This, UINT32 EffectIndex,
							  UINT32 OperationSet);
	STDMETHOD_C_(void, GetEffectState) (IXAudio2SourceVoice* This,
		UINT32 EffectIndex, BOOL* pEnabled);
	STDMETHOD_C(SetEffectParameters) (IXAudio2SourceVoice* This,
		UINT32 EffectIndex,
		const void* pParameters,
		UINT32 ParametersByteSize,
		UINT32 OperationSet);
	STDMETHOD_C(GetEffectParameters) (IXAudio2SourceVoice* This,
		UINT32 EffectIndex,
		void* pParameters,
		UINT32 ParametersByteSize);
	STDMETHOD_C(SetFilterParameters) (IXAudio2SourceVoice* This,
		const XAUDIO2_FILTER_PARAMETERS* pParameters,
		UINT32 OperationSet);
	STDMETHOD_C_(void, GetFilterParameters) (IXAudio2SourceVoice* This,
		XAUDIO2_FILTER_PARAMETERS* pParameters);
	STDMETHOD_C(SetOutputFilterParameters) (IXAudio2SourceVoice* This,
		IXAudio2Voice* pDestinationVoice,
		const XAUDIO2_FILTER_PARAMETERS* pParameters,
		UINT32 OperationSet);
	STDMETHOD_C_(void, GetOutputFilterParameters) (IXAudio2SourceVoice* This,
		IXAudio2Voice* pDestinationVoice,
		XAUDIO2_FILTER_PARAMETERS* pParameters);
	STDMETHOD_C(SetVolume) (IXAudio2SourceVoice* This, float Volume,
						  UINT32 OperationSet);
	STDMETHOD_C_(void, GetVolume) (IXAudio2SourceVoice* This, float* pVolume);
	STDMETHOD_C(SetChannelVolumes) (IXAudio2SourceVoice* This,
		UINT32 Channels, const float* pVolumes,
		UINT32 OperationSet);
	STDMETHOD_C_(void, GetChannelVolumes) (IXAudio2SourceVoice* This,
		UINT32 Channels, float* pVolumes);
	STDMETHOD_C(SetOutputMatrix) (IXAudio2SourceVoice* This,
		IXAudio2Voice* pDestinationVoice,
		UINT32 SourceChannels, UINT32 DestinationChannels,
		const float* pLevelMatrix,
		UINT32 OperationSet);
	STDMETHOD_C_(void, GetOutputMatrix) (IXAudio2SourceVoice* This,
		IXAudio2Voice* pDestinationVoice,
		UINT32 SourceChannels, UINT32 DestinationChannels,
		float* pLevelMatrix);
	STDMETHOD_C_(void, DestroyVoice) (IXAudio2SourceVoice* This);
	STDMETHOD_C(Start) (IXAudio2SourceVoice* This,
		UINT32 Flags, UINT32 OperationSet);
	STDMETHOD_C(Stop) (IXAudio2SourceVoice* This,
		UINT32 Flags, UINT32 OperationSet);
	STDMETHOD_C(SubmitSourceBuffer) (IXAudio2SourceVoice* This,
		const XAUDIO2_BUFFER* pBuffer, const XAUDIO2_BUFFER_WMA* pBufferWMA);
	STDMETHOD_C(FlushSourceBuffers) (IXAudio2SourceVoice* This);
	STDMETHOD_C(Discontinuity) (IXAudio2SourceVoice* This);
	STDMETHOD_C(ExitLoop) (IXAudio2SourceVoice* This, UINT32 OperationSet);
	STDMETHOD_C_(void, GetState) (IXAudio2SourceVoice* This,
		XAUDIO2_VOICE_STATE* pVoiceState, UINT32 Flags);
	STDMETHOD_C(SetFrequencyRatio) (IXAudio2SourceVoice* This, float Ratio,
								  UINT32 OperationSet);
	STDMETHOD_C_(void, GetFrequencyRatio) (IXAudio2SourceVoice* This, float* pRatio);
	STDMETHOD_C(SetSourceSampleRate) (IXAudio2SourceVoice* This,
		UINT32 NewSourceSampleRate);
	END_INTERFACE
};
#define IXAudio2SourceVoice_GetVoiceDetails IXAudio2Voice_GetVoiceDetails
#define IXAudio2SourceVoice_SetOutputVoices IXAudio2Voice_SetOutputVoices
#define IXAudio2SourceVoice_SetEffectChain IXAudio2Voice_SetEffectChain
#define IXAudio2SourceVoice_EnableEffect IXAudio2Voice_EnableEffect
#define IXAudio2SourceVoice_DisableEffect IXAudio2Voice_DisableEffect
#define IXAudio2SourceVoice_GetEffectState IXAudio2Voice_GetEffectState
#define IXAudio2SourceVoice_SetEffectParameters \
	IXAudio2Voice_SetEffectParameters
#define IXAudio2SourceVoice_GetEffectParameters \
	IXAudio2Voice_GetEffectParameters
#define IXAudio2SourceVoice_SetFilterParameters \
	IXAudio2Voice_SetFilterParameters
#define IXAudio2SourceVoice_GetFilterParameters \
	IXAudio2Voice_GetFilterParameters
#define IXAudio2SourceVoice_SetOutputFilterParameters \
	IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2SourceVoice_GetOutputFilterParameters \
	IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2SourceVoice_SetVolume IXAudio2Voice_SetVolume
#define IXAudio2SourceVoice_GetVolume IXAudio2Voice_GetVolume
#define IXAudio2SourceVoice_SetChannelVolumes IXAudio2Voice_SetChannelVolumes
#define IXAudio2SourceVoice_GetChannelVolumes IXAudio2Voice_GetChannelVolumes
#define IXAudio2SourceVoice_SetOutputMatrix IXAudio2Voice_SetOutputMatrix
#define IXAudio2SourceVoice_GetOutputMatrix IXAudio2Voice_GetOutputMatrix
#define IXAudio2SourceVoice_DestroyVoice IXAudio2Voice_DestroyVoice
#define IXAudio2SourceVoice_Start(This,Flags,OperationSet) \
	((This)->lpVtbl->Start(This,Flags,OperationSet))
#define IXAudio2SourceVoice_Stop(This,Flags,OperationSet) \
	((This)->lpVtbl->Stop(This,Flags,OperationSet))
#define IXAudio2SourceVoice_SubmitSourceBuffer(This,pBuffer,pBufferWMA) \
	((This)->lpVtbl->SubmitSourceBuffer(This,pBuffer,pBufferWMA))
#define IXAudio2SourceVoice_FlushSourceBuffers(This) \
	((This)->lpVtbl->FlushSourceBuffers(This))
#define IXAudio2SourceVoice_Discontinuity(This) \
	((This)->lpVtbl->Discontinuity(This))
#define IXAudio2SourceVoice_ExitLoop(This,OperationSet) \
	((This)->lpVtbl->ExitLoop(This,OperationSet))
#define IXAudio2SourceVoice_GetState(This,pVoiceState,Flags) \
	((This)->lpVtbl->GetState(This,pVoiceState,Flags))
#define IXAudio2SourceVoice_SetFrequencyRatio(This,Ratio,OperationSet) \
	((This)->lpVtbl->SetFrequencyRatio(This,Ratio,OperationSet))
#define IXAudio2SourceVoice_GetFrequencyRatio(This,pRatio) \
	((This)->lpVtbl->GetFrequencyRatio(This,pRatio))
#define IXAudio2SourceVoice_SetSourceSampleRate(This,NewSourceSampleRate) \
	((This)->lpVtbl->SetSourceSampleRate(This,NewSourceSampleRate))

DECLARE_INTERFACE_C(IXAudio2MasteringVoice)
{
	BEGIN_INTERFACE
	STDMETHOD_C_(void, GetVoiceDetails) (IXAudio2MasteringVoice* This,
		XAUDIO2_VOICE_DETAILS* pVoiceDetails);
	STDMETHOD_C(SetOutputVoices) (IXAudio2MasteringVoice* This,
		const XAUDIO2_VOICE_SENDS* pSendList);
	STDMETHOD_C(SetEffectChain) (IXAudio2MasteringVoice* This,
		const XAUDIO2_EFFECT_CHAIN* pEffectChain);
	STDMETHOD_C(EnableEffect) (IXAudio2MasteringVoice* This,
		UINT32 EffectIndex,
		UINT32 OperationSet);
	STDMETHOD_C(DisableEffect) (IXAudio2MasteringVoice* This,
		UINT32 EffectIndex,
		UINT32 OperationSet);
	STDMETHOD_C_(void, GetEffectState) (IXAudio2MasteringVoice* This,
		UINT32 EffectIndex, BOOL* pEnabled);
	STDMETHOD_C(SetEffectParameters) (IXAudio2MasteringVoice* This,
		UINT32 EffectIndex,
		const void* pParameters,
		UINT32 ParametersByteSize,
		UINT32 OperationSet);
	STDMETHOD_C(GetEffectParameters) (IXAudio2MasteringVoice* This,
		UINT32 EffectIndex,
		void* pParameters,
		UINT32 ParametersByteSize);
	STDMETHOD_C(SetFilterParameters) (IXAudio2MasteringVoice* This,
		const XAUDIO2_FILTER_PARAMETERS* pParameters,
									UINT32 OperationSet);
	STDMETHOD_C_(void, GetFilterParameters) (IXAudio2MasteringVoice* This,
		XAUDIO2_FILTER_PARAMETERS* pParameters);
	STDMETHOD_C(SetOutputFilterParameters) (IXAudio2MasteringVoice* This,
		IXAudio2Voice* pDestinationVoice,
		const XAUDIO2_FILTER_PARAMETERS* pParameters,
		UINT32 OperationSet);
	STDMETHOD_C_(void, GetOutputFilterParameters) (IXAudio2MasteringVoice* This,
		IXAudio2Voice* pDestinationVoice,
		XAUDIO2_FILTER_PARAMETERS* pParameters);
	STDMETHOD_C(SetVolume) (IXAudio2MasteringVoice* This, float Volume,
						  UINT32 OperationSet);
	STDMETHOD_C_(void, GetVolume) (IXAudio2MasteringVoice* This, float* pVolume);
	STDMETHOD_C(SetChannelVolumes) (IXAudio2MasteringVoice* This,
		UINT32 Channels, const float* pVolumes,
								  UINT32 OperationSet);
	STDMETHOD_C_(void, GetChannelVolumes) (IXAudio2MasteringVoice* This,
		UINT32 Channels, float* pVolumes);
	STDMETHOD_C(SetOutputMatrix) (IXAudio2MasteringVoice* This,
		IXAudio2Voice* pDestinationVoice,
		UINT32 SourceChannels, UINT32 DestinationChannels,
		const float* pLevelMatrix,
		UINT32 OperationSet);
	STDMETHOD_C_(void, GetOutputMatrix) (IXAudio2MasteringVoice* This,
		IXAudio2Voice* pDestinationVoice,
		UINT32 SourceChannels, UINT32 DestinationChannels,
		float* pLevelMatrix);
	STDMETHOD_C_(void, DestroyVoice) (IXAudio2MasteringVoice* This);
	STDMETHOD_C(GetChannelMask) (IXAudio2MasteringVoice* This, DWORD* pChannelmask);
	END_INTERFACE
};
#define IXAudio2MasteringVoice_GetVoiceDetails IXAudio2Voice_GetVoiceDetails
#define IXAudio2MasteringVoice_SetOutputVoices IXAudio2Voice_SetOutputVoices
#define IXAudio2MasteringVoice_SetEffectChain IXAudio2Voice_SetEffectChain
#define IXAudio2MasteringVoice_EnableEffect IXAudio2Voice_EnableEffect
#define IXAudio2MasteringVoice_DisableEffect IXAudio2Voice_DisableEffect
#define IXAudio2MasteringVoice_GetEffectState IXAudio2Voice_GetEffectState
#define IXAudio2MasteringVoice_SetEffectParameters \
	IXAudio2Voice_SetEffectParameters
#define IXAudio2MasteringVoice_GetEffectParameters \
	IXAudio2Voice_GetEffectParameters
#define IXAudio2MasteringVoice_SetFilterParameters \
	IXAudio2Voice_SetFilterParameters
#define IXAudio2MasteringVoice_GetFilterParameters \
	IXAudio2Voice_GetFilterParameters
#define IXAudio2MasteringVoice_SetOutputFilterParameters \
	IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2MasteringVoice_GetOutputFilterParameters \
	IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2MasteringVoice_SetVolume IXAudio2Voice_SetVolume
#define IXAudio2MasteringVoice_GetVolume IXAudio2Voice_GetVolume
#define IXAudio2MasteringVoice_SetChannelVolumes \
	IXAudio2Voice_SetChannelVolumes
#define IXAudio2MasteringVoice_GetChannelVolumes \
	IXAudio2Voice_GetChannelVolumes
#define IXAudio2MasteringVoice_SetOutputMatrix IXAudio2Voice_SetOutputMatrix
#define IXAudio2MasteringVoice_GetOutputMatrix IXAudio2Voice_GetOutputMatrix
#define IXAudio2MasteringVoice_DestroyVoice IXAudio2Voice_DestroyVoice
#define IXAudio2MasteringVoice_GetChannelMask(This,pChannelMask) \
	((This)->lpVtbl->GetChannelMask(This,pChannelMask))

DECLARE_INTERFACE_C(IXAudio2)
{
	BEGIN_INTERFACE
	STDMETHOD_C(QueryInterface)(IXAudio2* This, REFIID riid, void **ppvObject);
	STDMETHOD_C_(ULONG, AddRef)(IXAudio2* This);
	STDMETHOD_C_(ULONG, Release)(IXAudio2* This);
	STDMETHOD_C(RegisterForCallbacks) (
		IXAudio2* This, IXAudio2EngineCallback* pCallback);
	STDMETHOD_C_(void, UnregisterForCallbacks) (IXAudio2* This,
		IXAudio2EngineCallback* pCallback);
	STDMETHOD_C(CreateSourceVoice) (IXAudio2* This,
		IXAudio2SourceVoice** ppSourceVoice,
		const WAVEFORMATEX* pSourceFormat,
	   UINT32 Flags,
	   float MaxFrequencyRatio,
		IXAudio2VoiceCallback* pCallback,
		const XAUDIO2_VOICE_SENDS* pSendList,
		const XAUDIO2_EFFECT_CHAIN* pEffectChain);
	STDMETHOD_C(CreateSubmixVoice) (IXAudio2* This,
		IXAudio2SubmixVoice** ppSubmixVoice,
	   UINT32 InputChannels, UINT32 InputSampleRate,
	   UINT32 Flags, UINT32 ProcessingStage,
		const XAUDIO2_VOICE_SENDS* pSendList,
		const XAUDIO2_EFFECT_CHAIN* pEffectChain);
	STDMETHOD_C(CreateMasteringVoice) (IXAudio2* This,
		IXAudio2MasteringVoice** ppMasteringVoice,
		UINT32 InputChannels,
		UINT32 InputSampleRate,
		  UINT32 Flags, LPCWSTR szDeviceId,
		   const XAUDIO2_EFFECT_CHAIN* pEffectChain,
		  AUDIO_STREAM_CATEGORY StreamCategory);
	STDMETHOD_C(StartEngine) (IXAudio2* This);
	STDMETHOD_C_(void, StopEngine) (IXAudio2* This);
	STDMETHOD_C(CommitChanges) (IXAudio2* This, UINT32 OperationSet);
	STDMETHOD_C_(void, GetPerformanceData) (IXAudio2* This,
		XAUDIO2_PERFORMANCE_DATA* pPerfData);
	STDMETHOD_C_(void, SetDebugConfiguration) (IXAudio2* This,
		const XAUDIO2_DEBUG_CONFIGURATION* pDebugConfiguration,
				  void* pReserved);
	END_INTERFACE
};
#define IXAudio2_QueryInterface(This,riid,ppvInterface) \
	((This)->lpVtbl->QueryInterface(This,riid,ppvInterface))
#define IXAudio2_AddRef(This) ((This)->lpVtbl->AddRef(This))
#define IXAudio2_Release(This) ((This)->lpVtbl->Release(This))
#define IXAudio2_RegisterForCallbacks(This,pCallback) \
	((This)->lpVtbl->RegisterForCallbacks(This,pCallback))
#define IXAudio2_UnregisterForCallbacks(This,pCallback) \
	((This)->lpVtbl->UnregisterForCallbacks(This,pCallback))
#define IXAudio2_CreateSourceVoice(This,ppSourceVoice,pSourceFormat,Flags,\
	MaxFrequencyRatio,pCallback,pSendList,pEffectChain) \
	((This)->lpVtbl->CreateSourceVoice(This,ppSourceVoice,pSourceFormat,\
		Flags,MaxFrequencyRatio,pCallback,pSendList,pEffectChain))
#define IXAudio2_CreateSubmixVoice(This,ppSubmixVoice,InputChannels,\
	InputSampleRate,Flags,ProcessingStage,pSendList,pEffectChain) \
	((This)->lpVtbl->CreateSubmixVoice(This,ppSubmixVoice,InputChannels,\
		InputSampleRate,Flags,ProcessingStage,pSendList,pEffectChain))
#define IXAudio2_CreateMasteringVoice(This,ppMasteringVoice,InputChannels,\
	InputSampleRate,Flags,DeviceId,pEffectChain,StreamCategory) \
	((This)->lpVtbl->CreateMasteringVoice(This,ppMasteringVoice,InputChannels,\
		InputSampleRate,Flags,DeviceId,pEffectChain,StreamCategory))
#define IXAudio2_StartEngine(This) ((This)->lpVtbl->StartEngine(This))
#define IXAudio2_StopEngine(This) ((This)->lpVtbl->StopEngine(This))
#define IXAudio2_CommitChanges(This,OperationSet) \
	((This)->lpVtbl->CommitChanges(This,OperationSet))
#define IXAudio2_GetPerformanceData(This,pPerfData) \
	((This)->lpVtbl->GetPerformanceData(This,pPerfData))
#define IXAudio2_SetDebugConfiguration(This,pDebugConfiguration,pReserved) \
	((This)->lpVtbl->SetDebugConfiguration(This,pDebugConfiguration,pReserved))
#endif /* include xaudio2.h */


#define WSAUDIOX2_MONO             SPEAKER_FRONT_CENTER
#define WSAUDIOX2_STEREO           (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT)
#define WSAUDIOX2_2POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT \
	| SPEAKER_LOW_FREQUENCY)
#define WSAUDIOX2_SURROUND         (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT \
	| SPEAKER_FRONT_CENTER | SPEAKER_BACK_CENTER)
#define WSAUDIOX2_QUAD             (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT \
	| SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
#define WSAUDIOX2_4POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT \
	| SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
#define WSAUDIOX2_5POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT \
	| SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT \
	| SPEAKER_BACK_RIGHT)
#define WSAUDIOX2_7POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT \
	| SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT \
	| SPEAKER_BACK_RIGHT | SPEAKER_FRONT_LEFT_OF_CENTER \
	| SPEAKER_FRONT_RIGHT_OF_CENTER)
#define WSAUDIOX2_5POINT1SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT \
	| SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_SIDE_LEFT  \
	| SPEAKER_SIDE_RIGHT)
#define WSAUDIOX2_7POINT1SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT \
	| SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT \
	| SPEAKER_BACK_RIGHT | SPEAKER_SIDE_LEFT  | SPEAKER_SIDE_RIGHT)

/*=============================================================================
					WSAUDIO XAUDIO IMPLEMENTATION
=============================================================================*/
#if defined(__cplusplus)
extern "C" {
#endif
void WSAudioFormatToWaveFormatEx(
	const WSAUDIOFORMAT* Format, WAVEFORMATEX* wfx)
{
	wfx->cbSize = 0;
	wfx->wFormatTag = WAVE_FORMAT_PCM;
	wfx->wBitsPerSample = Format->bitsPerSample;
	wfx->nSamplesPerSec = Format->sampleRate;
	wfx->nChannels = Format->channels;
	wfx->nBlockAlign = WSAudioFormat_GetBlockAlign(Format);
	wfx->nAvgBytesPerSec = Format->sampleRate * wfx->nBlockAlign;
}

void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	enumparams->numdevices = 0;
	enumparams->devices = NULL;
}

void WSAudio_FillAndSubmitX2Buffer(WSAUDIO* pThis, XAUDIO2_BUFFER* xbuffer)
{
	HRESULT hr;
	(*pThis->mFillProc)(pThis->mUserData,
		(BYTE*)xbuffer->pAudioData,
		pThis->bufsize);
	hr = IXAudio2SourceVoice_SubmitSourceBuffer(pThis->sourceVoice,
		xbuffer, NULL);
	if(!SUCCEEDED(hr))
		return; /* does nothing, for debugging */
}

void __stdcall WSAudioOnBufferEnd(IXAudio2VoiceCallback* pThis, void* pBufferContext)
{
	WSAUDIO* pAudio =
		CONTAINING_RECORD(pThis, WSAUDIO, voicecbs);
	XAUDIO2_BUFFER* xbuffer = (XAUDIO2_BUFFER*)pBufferContext;
	WSAudio_FillAndSubmitX2Buffer(pAudio, xbuffer);
}

void __stdcall WSAudioOnStreamEnd(IXAudio2VoiceCallback* pThis){}
void __stdcall WSAudioOnVoiceProcessingPassEnd(IXAudio2VoiceCallback* pThis){}
void __stdcall WSAudioOnVoiceProcessingPassStart(IXAudio2VoiceCallback* pThis,
	UINT32 SamplesRequired){}
void __stdcall WSAudioOnBufferStart(IXAudio2VoiceCallback* pThis,
	void* pBufferContext){}
void __stdcall WSAudioOnLoopEnd(IXAudio2VoiceCallback* pThis, void* pBufferContext){}
void __stdcall WSAudioOnVoiceError(IXAudio2VoiceCallback* pThis,
	void* pBufferContext, HRESULT Error){}

#if defined(WSMSIXOPT)
	HRESULT __stdcall XAudio2Create(IXAudio2**, UINT32, XAUDIO2_PROCESSOR);
	#pragma comment(lib, "xaudio2_8.lib")
#endif
typedef HRESULT(__stdcall *XAudio2CreateFunc)(IXAudio2**, UINT32, XAUDIO2_PROCESSOR);

typedef HRESULT (WINAPI* LPCoInitialize)(LPVOID pvReserved);
typedef void (WINAPI* LPCoUninitialize)(void);
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	size_t i;
	WAVEFORMATEX wfx;
	XAudio2CreateFunc pfnXAudio2Create;
	XAUDIO2_DEBUG_CONFIGURATION debug = {0};
	HRESULT hr;
	#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
		LPCoInitialize pfCoInitialize;
		LPCoUninitialize pfCoUninitialize;

		pThis->mOle32 = LoadLibraryA("ole32.dll");
		pfCoInitialize = (LPCoInitialize)
			GetProcAddress(pThis->mOle32, "CoInitialize");
		pfCoUninitialize = (LPCoUninitialize)
			GetProcAddress(pThis->mOle32, "CoUninitialize");
		hr = (*pfCoInitialize)(NULL);
	#else
		hr = CoInitialize(NULL);
	#endif
	if(FAILED(hr))
	{
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			FreeLibrary(pThis->mOle32);
		#endif
		return FALSE;
	}


#if !defined(WSMSIXOPT)
	pThis->mXAudioDLL = LoadLibraryA("xaudio2_8.dll");
	if(!pThis->mXAudioDLL)
	{
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(pThis->mOle32);
		#else
			CoUninitialize();
		#endif
		return FALSE;
	}
	pfnXAudio2Create = (XAudio2CreateFunc)
		GetProcAddress(pThis->mXAudioDLL, "XAudio2Create");
	if(!pfnXAudio2Create)
	{
		#if !defined(WSMSIXOPT)
			FreeLibrary(pThis->mXAudioDLL);
		#endif
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(pThis->mOle32);
		#else
			CoUninitialize();
		#endif
		return FALSE;
	}
#else
	pfnXAudio2Create = XAudio2Create;
#endif


	hr = (*pfnXAudio2Create)(&pThis->engine,
		XAUDIO2_DEBUG_ENGINE, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
	{
		#if !defined(WSMSIXOPT)
			FreeLibrary(pThis->mXAudioDLL);
		#endif
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(pThis->mOle32);
		#else
			CoUninitialize();
		#endif
		return FALSE;
	}

	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	IXAudio2_SetDebugConfiguration(pThis->engine, &debug, 0);

	hr = IXAudio2_CreateMasteringVoice(pThis->engine,
		&pThis->masteringVoice,
		/*spec->format.mSuper.channels, spec->format.mSuper.sampleRate,*/
		XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE,
		0, NULL, NULL, AudioCategory_GameEffects);
	if (FAILED(hr))
	{
		IXAudio2_Release(pThis->engine);
		#if !defined(WSMSIXOPT)
			FreeLibrary(pThis->mXAudioDLL);
		#endif
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(pThis->mOle32);
		#else
			CoUninitialize();
		#endif
		return FALSE;
	}

	pThis->voicecbsvtbl.OnBufferStart = WSAudioOnBufferStart;
	pThis->voicecbsvtbl.OnBufferEnd = WSAudioOnBufferEnd;
	pThis->voicecbsvtbl.OnLoopEnd = WSAudioOnLoopEnd;
	pThis->voicecbsvtbl.OnStreamEnd = WSAudioOnStreamEnd;
	pThis->voicecbsvtbl.OnVoiceError = WSAudioOnVoiceError;
	pThis->voicecbsvtbl.OnVoiceProcessingPassEnd =
		WSAudioOnVoiceProcessingPassEnd;
	pThis->voicecbsvtbl.OnVoiceProcessingPassStart =
		WSAudioOnVoiceProcessingPassStart;
	pThis->voicecbs.lpVtbl = &pThis->voicecbsvtbl;
	WSAudioFormatToWaveFormatEx(&spec->format.mSuper, &wfx);
	hr = IXAudio2_CreateSourceVoice(pThis->engine, &pThis->sourceVoice,
		&wfx, 0, 1.0/*XAUDIO2_DEFAULT_FREQ_RATIO*/, &pThis->voicecbs,
		NULL, NULL);
	if (FAILED(hr))
	{
		/*IXAudio2MasteringVoice_DestroyVoice(pThis->masteringVoice);*/
		IXAudio2_Release(pThis->engine);
		#if !defined(WSMSIXOPT)
			FreeLibrary(pThis->mXAudioDLL);
		#endif
		#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
			(*pfCoUninitialize)();
			FreeLibrary(pThis->mOle32);
		#else
			CoUninitialize();
		#endif
		return FALSE;
	}

	pThis->bufsize = WSUPPERCLAMPTYPE(spec->format.dwBufferSize, UINT32);
	for(i = 0; i < WSAUDIOX2_NUMBUFFERS; ++i)
	{
		WSMemoryFill(&pThis->buffers[i], 0, sizeof(XAUDIO2_BUFFER));
		pThis->buffers[i].pAudioData =
			WSALLOC(uint8_t, pThis->bufsize);
		pThis->buffers[i].AudioBytes = pThis->bufsize;
		pThis->buffers[i].pContext = &pThis->buffers[i];
	}

	pThis->mFlags = spec->flags;
	pThis->mFillProc = spec->proc;
	pThis->mUserData = spec->procuserdata;
	return TRUE;
}

void WSAudio_Destroy(WSAUDIO* pThis)
{
	size_t i;
#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
	LPCoUninitialize pfCoUninitialize;
	pfCoUninitialize =
		(LPCoUninitialize) GetProcAddress(pThis->mOle32, "CoUninitialize");
#endif
	for(i = 0; i < WSAUDIOX2_NUMBUFFERS; ++i)
		WSFREE(pThis->buffers[i].pAudioData);
	IXAudio2SourceVoice_Stop(pThis->sourceVoice,
		0, XAUDIO2_COMMIT_NOW);
	/* not needed, IXAudio2_Release frees all */
	/*IXAudio2SourceVoice_DestroyVoice(pThis->sourceVoice);
	IXAudio2MasteringVoice_DestroyVoice(pThis->masteringVoice);*/
	IXAudio2_Release(pThis->engine);
#if !defined(WSMSIXOPT)
	FreeLibrary(pThis->mXAudioDLL);
#endif
	(*pfCoUninitialize)();
#if !defined(WSAUDIO_STATICOLE32) && !defined(WSMSIXOPT)
	(*pfCoUninitialize)();
	FreeLibrary(pThis->mOle32);
#else
	CoUninitialize();
#endif
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	return 10000; /* arbitrary - it uses 0 to 1.0 */
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	return 0;
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int leftpan, int rightpan)
{
	float left = (((float)leftpan) / WSAudio_GetMaxVolume(pThis));
	float right = (((float)rightpan) / WSAudio_GetMaxVolume(pThis));
	float pan = -(((float)leftpan) / WSAudio_GetMaxVolume(pThis))
		+ (((float)rightpan) / WSAudio_GetMaxVolume(pThis));
	float matrix[4];
	DWORD dwChannelMask;
	XAUDIO2_VOICE_DETAILS MasterVoiceDetails;
	XAUDIO2_VOICE_DETAILS SourceVoiceDetails;
	HRESULT hr = IXAudio2MasteringVoice_GetChannelMask(
		pThis->masteringVoice, &dwChannelMask);
	if(FAILED(hr))
		return FALSE;
	switch (dwChannelMask)
	{
	case WSAUDIOX2_MONO:
		left = 1.0f - pan;
		if(left > 1.0f)
			left = 1.0f;
		else if(left < 0.0f)
			left = 0.0f;
		matrix[0] = left;
		right = pan + 1.0f;
		if(right > 1.0f)
			right = 1.0f;
		else if(right < 0.0f)
			right = 0.0f;
		matrix[1] = right;
		break;
	case WSAUDIOX2_STEREO:
	case WSAUDIOX2_2POINT1:
	case WSAUDIOX2_SURROUND:
		if (-1.0 <= pan && pan <= 0.0)
		{
			matrix[0] = 0.5f * pan + 1.0f;	/* 0.5<->1.0 */
			matrix[1] = 0.5f * -pan;		/* 0.5<->0.0 */
			matrix[2] = 0.0f;				/* 0.0<->0.0 */
			matrix[3] = pan + 1.0f;			/* 0.0<->1.0 */
		}
		else
		{
			matrix[0] = -pan + 1.0f;		/* 1.0<->0.0 */
			matrix[1] = 0.0f;				/* 0.0<->0.0 */
			matrix[2] = 0.5f * pan;			/* 0.0<->0.5 */
			matrix[3] = 0.5f * -pan + 1.0f;	/* 1.0<->0.5 */
		}
		break;
	case WSAUDIOX2_QUAD:
	case WSAUDIOX2_4POINT1:
	case WSAUDIOX2_5POINT1:
	case WSAUDIOX2_7POINT1:
	case WSAUDIOX2_5POINT1SURROUND:
	case WSAUDIOX2_7POINT1SURROUND:
		return FALSE; /* Would require a lot of math */
	}
	IXAudio2MasteringVoice_GetVoiceDetails(
		pThis->masteringVoice, &MasterVoiceDetails);
	IXAudio2SourceVoice_GetVoiceDetails(
		pThis->sourceVoice, &SourceVoiceDetails);

	/*
		p2) NULL indicates the mastering voice
		p3) is the number of source channels
		p4) is the number of destination channels
		p5) matrix is our volume matrix
	*/
	hr = IXAudio2SourceVoice_SetOutputMatrix(
		pThis->sourceVoice, NULL, SourceVoiceDetails.InputChannels,
		MasterVoiceDetails.InputChannels, matrix, XAUDIO2_COMMIT_NOW);
	return SUCCEEDED(hr);
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{
	return SUCCEEDED(IXAudio2SourceVoice_SetVolume(
		pThis->sourceVoice,
		((float)volume) / WSAudio_GetMaxVolume(pThis),
		XAUDIO2_COMMIT_NOW));
}

void WSAudio_Process(WSAUDIO* pThis);
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	HRESULT hr;
	size_t i;
	for(i = 0; i < WSAUDIOX2_NUMBUFFERS; ++i)
		WSAudio_FillAndSubmitX2Buffer(pThis, &pThis->buffers[i]);
	hr = IXAudio2SourceVoice_Start(pThis->sourceVoice, 0,
		XAUDIO2_COMMIT_NOW);
	return SUCCEEDED(hr);
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{
	XAUDIO2_VOICE_STATE state;
	IXAudio2SourceVoice_GetState(pThis->sourceVoice, &state, 0);
	return state.BuffersQueued > 0;
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	return SUCCEEDED(IXAudio2SourceVoice_Stop(pThis->sourceVoice, 0,
		XAUDIO2_COMMIT_NOW));
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	return SUCCEEDED(IXAudio2SourceVoice_Start(pThis->sourceVoice, 0,
		XAUDIO2_COMMIT_NOW));
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	return 0;
}
wsbool WSAudioHasFullDuplex() { return FALSE; }
void WSAudio_Process(WSAUDIO* pThis)
{
	/* Handled in IXAudio2VoiceCallback */
	/*
	WSAUDIOX2HANDLE* pThis = (WSAUDIOX2HANDLE*)pThis->mHandle;
	XAUDIO2_VOICE_STATE state;
	HRESULT hr;
	IXAudio2SourceVoice_GetState(pThis->sourceVoice,	&state, 0);
	while(state.BuffersQueued < WSAUDIOX2_NUMBUFFERS)
	{
		(*pThis->mFillProc)(pThis->mUserData,
			(BYTE*)pThis->buffers[state.BuffersQueued].pAudioData,
			pThis->bufsize);
		hr = IXAudio2SourceVoice_SubmitSourceBuffer(pThis->sourceVoice,
			&pThis->buffers[state.BuffersQueued], NULL);
		++state.BuffersQueued;
	}*/
}
#if defined(__cplusplus)
} /* end extern "C" */
#endif
/*=============================================================================
					#    #     # ###  #####     #
				   # #   ##   ##  #  #     #   # #
				  #   #  # # # #  #  #        #   #
				 #     # #  #  #  #  #  #### #     #
				 ####### #     #  #  #     # #######
				 #     # #     #  #  #     # #     #
				 #     # #     # ###  #####  #     #
=============================================================================*/
#elif defined(__amigaos__)
#include <exec/exec.h>
#include <proto/exec.h>
#include <clib/alib_protos.h>
#include <devices/audio.h>
#if defined(__cplusplus)
extern "C" {
#endif
/*=============================================================================
						AMIGA IMPLEMENTATION
=============================================================================*/
#define NUMAIO 4
void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	enumparams->devices = NULL;
	enumparams->numdevices = 0;
	/* There's no formal enumeration - it's just the PAULA chip */
	enumparams->devices = WSALLOC(WSAUDIODEVICE, 1);
	if(!enumparams->devices)
		return;
	enumparams->devices[0].id = 0;
	enumparams->devices[0].flags = 0;
	enumparams->devices[0].name = StringDup("PAULA");
	enumparams->devices[0].maxformat.bitsPerSample = 8;
	enumparams->devices[0].maxformat.channels = 2;
	enumparams->devices[0].maxformat.sampleRate = 48000;
}
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	size_t iIO, jIO, iPort, uiIO, ujIO, uiPort;

	/* Amiga is 8 bits per sample, and is limited to 1 or 2 channels */
	if(spec->format.mSuper.channels > 2
		|| spec->format.mSuper.bitsPerSample > 8)
		return FALSE;

	/* 2 chunks per channels as is the commonly accepted standard */
	pThis->mNumAIO =
		spec->format.mSuper.channels == 2 ? 4 : 2;

	for (iIO = 0, iPort = 0; iIO < pThis->mNumAIO; ++iIO, ++iPort)
	{
		UBYTE channelbytes[4];
		pThis->mPort[iPort] = CreatePort(0, 0);
		pThis->mAio[iIO] = (struct IOAudio*)
			CreateExtIO(pThis->mPort[iPort], sizeof(struct IOAudio));
		if(!pThis->mPort[iPort]
			|| !pThis->mAio[iIO])
			break;

		pThis->mAio[iIO]->ioa_Request.io_Command = ADCMD_ALLOCATE;
		pThis->mAio[iIO]->ioa_Request.io_Flags = ADIOF_NOWAIT;
		pThis->mAio[iIO]->ioa_AllocKey = 0;

		/*
		UBYTE rightchannel[]={4,2,1,8};
		UBYTE leftchannel[]={1,8,2,4};
		UBYTE eitherchannel[]={1,2,4,8};
		UBYTE bothchannels[]={8,3,5,10};
		*/

		if (iIO == 0 && pThis->mNumAIO == 2)
		{ /* mono */
			channelbytes[0] = 1;
			channelbytes[1] = 2;
			channelbytes[2] = 4;
			channelbytes[3] = 8;
		}
		else if (iIO == 2)
		{ /* right channel */
			channelbytes[0] = 4;
			channelbytes[1] = 2;
			channelbytes[2] = 1;
			channelbytes[3] = 8;
		}
		else
		{ /* left channel */
			channelbytes[0] = 1;
			channelbytes[1] = 8;
			channelbytes[2] = 2;
			channelbytes[3] = 4;
		}


		pThis->mAio[iIO]->ioa_Data = channelbytes;
		pThis->mAio[iIO]->ioa_Length = sizeof(channelbytes);


		if(OpenDevice("audio.device", 0L,
					(struct IORequest*)pThis->mAio[iIO], 0L) != 0)
			break;

		/* Create a spec->format.dwBufferSize size buffer
		* for each channel, splitting it between two IOAudios for
		* each channel */
		pThis->mAio[iIO]->ioa_Request.io_Command = CMD_WRITE;
		pThis->mAio[iIO]->ioa_Request.io_Flags =ADIOF_PERVOL;
		pThis->mAio[iIO]->ioa_Period = 1000000000
			/ (spec->format.mSuper.sampleRate * 279);
		pThis->mAio[iIO]->ioa_Data = WSALLOC(UBYTE, spec->format.dwBufferSize);
		pThis->mAio[iIO]->ioa_Length = spec->format.dwBufferSize / 2;
		pThis->mAio[iIO]->ioa_Cycles = 1;
		/* create secondary buffer */
		++iIO;
		pThis->mAio[iIO] = (struct IOAudio*)
			CreateExtIO(pThis->mPort[iPort], sizeof(struct IOAudio));
		if(!pThis->mAio[iIO])
			break;
		WSMemoryCopy(pThis->mAio[iIO], pThis->mAio[iIO - 1],
			sizeof(struct IOAudio));
		/* Move the secondary buffer to the second half of the buffer */
		pThis->mAio[iIO]->ioa_Data +=
			pThis->mAio[iIO]->ioa_Length;
	}
	if(iIO != pThis->mNumAIO)
	{ /* had an error, clean up and return FALSE */
		for (uiIO = 0, uiPort = 0; uiIO < iIO; ++uiIO, ++uiPort)
		{
			if(pThis->mAio[uiIO])
				CloseDevice((struct IORequest*)pThis->mAio[uiIO]);
			if(pThis->mPort[uiPort])
				DeletePort(pThis->mPort[uiPort]);
			if(pThis->mAio[uiIO])
			{
				WSFREE(pThis->mAio[uiIO]->ioa_Data);
				DeleteExtIO((struct IORequest*)pThis->mAio[uiIO]);
			}
			++uiIO;
			if(uiIO < iIO && pThis->mAio[uiIO])
				DeleteExtIO((struct IORequest*)pThis->mAio[uiIO]);
		}
		return FALSE;
	}

	pThis->mFlags = spec->flags;
	pThis->mUserData = spec->procuserdata;
	pThis->mFillProc = spec->proc;
	pThis->mIsPlaying = FALSE;
	pThis->mAioIndex = 0;
	return TRUE;
}
void WSAudio_Destroy(WSAUDIO* pThis)
{
	size_t i, j;

	for (i = 0; i < pThis->mNumAIO; ++i)
	{
		CloseDevice((struct IORequest*)pThis->mAio[i]);
		DeletePort(pThis->mPort[i / 2]);
		WSFREE(pThis->mAio[i]->ioa_Data);
		for (j = 0; j < 2; ++i, ++j)
		{
			DeleteExtIO((struct IORequest*)pThis->mAio[i]);
		}
	}
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright)
{
	return FALSE;
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{
	return FALSE;
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	return 64;
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	return 0;
}
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	(*pThis->mFillProc)(pThis->mUserData,
				pThis->mAio[0]->ioa_Data, pThis->mAio[0]->ioa_Length * 2);
	BeginIO((struct IORequest *)pThis->mAio[0]);
	BeginIO((struct IORequest *)pThis->mAio[1]);
	if(pThis->mNumAIO > 2)
	{ /* Play the same sound on the other channel */
		WSMemoryCopy(pThis->mAio[2]->ioa_Data,
			pThis->mAio[0]->ioa_Data,
			pThis->mAio[0]->ioa_Length * 2);
		BeginIO((struct IORequest *)pThis->mAio[2]);
		BeginIO((struct IORequest *)pThis->mAio[3]);
	}
	pThis->mIsPlaying = TRUE;
	return TRUE;
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{
	return pThis->mIsPlaying;
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{ /* bits 0-3 of io_Unit are channels: 1, 2, 4, 8 are R1, L1, L2, R2 */
	struct Unit* oldiounit = pThis->mAio[0]->ioa_Request.io_Unit;
	int oldcommand = pThis->mAio[0]->ioa_Request.io_Command;
	pThis->mAio[0]->ioa_Request.io_Command = CMD_STOP;
	pThis->mAio[0]->ioa_Request.io_Unit = (struct Unit*)0xF;
	BeginIO((struct IORequest*)pThis->mAio[0]);
	pThis->mAio[0]->ioa_Request.io_Unit = oldiounit;
	pThis->mAio[0]->ioa_Request.io_Command = oldcommand;
	pThis->mIsPlaying = FALSE;
	return TRUE;
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	struct Unit* oldiounit = pThis->mAio[0]->ioa_Request.io_Unit;
	int oldcommand = pThis->mAio[0]->ioa_Request.io_Command;
	pThis->mAio[0]->ioa_Request.io_Command = CMD_START;
	pThis->mAio[0]->ioa_Request.io_Unit = (struct Unit*)0xF;
	BeginIO((struct IORequest*)pThis->mAio[0]);
	pThis->mAio[0]->ioa_Request.io_Unit = oldiounit;
	pThis->mAio[0]->ioa_Request.io_Command = oldcommand;
	pThis->mIsPlaying = TRUE;
	return TRUE;
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	return 0;
}
wsbool WSAudioHasFullDuplex() { return FALSE; }
void WSAudio_Process(WSAUDIO* pThis)
{
	struct Message* msgLeft = GetMsg(pThis->mPort[0]);

	if(msgLeft)
	{ /* Here we only fill half the buffer at a time (double buffering) */
		(*pThis->mFillProc)(pThis->mUserData,
					pThis->mAio[pThis->mAioIndex]->ioa_Data,
			pThis->mAio[pThis->mAioIndex]->ioa_Length);
		BeginIO((struct IORequest *)pThis->mAio[pThis->mAioIndex]);
		if(pThis->mNumAIO > 2 && GetMsg(pThis->mPort[1]))
		{ /* Play the same sound on the other channel */
			WSMemoryCopy(pThis->mAio[pThis->mAioIndex + 2]->ioa_Data,
				pThis->mAio[pThis->mAioIndex]->ioa_Data,
				pThis->mAio[pThis->mAioIndex]->ioa_Length);
			BeginIO((struct IORequest *)pThis->mAio[pThis->mAioIndex + 2]);
		}
		++pThis->mAioIndex;
	}
}
#if defined(__cplusplus)
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
				 #     #  ####  #    # #    # #####
				 #       #    # #    # ##   # #    #
				  #####  #    # #    # # #  # #    #
					   # #    # #    # #  # # #    #
				 #     # #    # #    # #   ## #    #
				  #####   ####   ####  #    # #####

			#     #
			##   ##   ##   #    #   ##    ####  ###### #####
			# # # #  #  #  ##   #  #  #  #    # #      #    #
			#  #  # #    # # #  # #    # #      #####  #    #
			#     # ###### #  # # ###### #  ### #      #####
			#     # #    # #   ## #    # #    # #      #   #
			#     # #    # #    # #    #  ####  ###### #    #

			Can be compiled and function on Windows.
=============================================================================*/
#elif defined(macintosh) || (defined(_WIN32) && defined(WSAUDIOSMOPT))
/* QuickTime Media Layer headers and libraries for Windows */
#ifdef _WIN32
	#ifndef	_M_IX86 /* Force X86 so TARGET_OS_WIN32 gets defined */
		#define _M_IX86
	#endif
	#define _STDINT_H /* Prevent uintXXX_t clashing with QuickTime SDK */
	#define __STDBOOL_H__ /* Includes an empty enum (bool) == compile error */
	#define CStrCopy CStrCopyMac /* Our conflict */
	#define float_t float2_t /* conflicts with corecrt_math.h */
	#define GetProcessInformation MacGetProcessInformation /* Win32 conflict */
	#include <qtml.h>
	#undef GetProcessInformation
	#undef float_t
	#ifdef _MSC_VER
		#pragma comment(lib, "QTMLClient.lib")
	#endif
	#undef CStrCopy
#endif
/* Sound Manager headers */
#if defined(__MACH__)
	#ifdef __APPLE_CC__
		#include <Carbon/Carbon.h>
	#else
		#include <Carbon.h>
	#endif
#else
	#include <Sound.h>
	#if defined(macintosh) || defined(WSAUDIO_ISCARBON)
		#include <Gestalt.h>
	#endif
#endif

/*=============================================================================
						WSAUDIO SOUND MANAGER

	Carbon doesn't support double buffering, and neither does QTML
=============================================================================*/
#if defined(__cplusplus)
extern "C" {
#endif
#if defined(WSAUDIOSM_USEDOUBLEBUFFER)
	#if defined(macintosh) && !defined(WSAUDIO_ISCARBON)
		#define WSAUDIOSM_CHECKDOUBLEBUFFER
	#else
		#define WSAUDIOSM_NOCHECKDOUBLEBUFFER
	#endif
#endif
#if !defined(WIKISERVER_VERSION) && !defined(WSAUDIO_ISCARBON)
#if !defined(TARGET_API_MAC_CARBON) || !TARGET_API_MAC_CARBON
	static short AudioNumToolboxTraps(void)
	{
		if (NGetTrapAddress(0xA86E, ToolTrap) == NGetTrapAddress(0xAA6E, ToolTrap))
			return 0x200;
		else
			return 0x400;
	}
	static TrapType AudioGetTrapType(short theTrap)
	{
		/* OS traps start with A0, Tool with A8 or AA. */
		if ((theTrap & 0x0800) == 0)
			return OSTrap;
		else
			return ToolTrap;
	}
	static Boolean AudioTrapExists(short theTrap)
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
	wsbool WSAudioOneGestalt(OSType selector, long test)
{
	long response;
	if (
#if !defined(TARGET_API_MAC_CARBON) || !TARGET_API_MAC_CARBON
		!AudioTrapExists(0xA1AD) /* 0xA1AD == _Gestalt */ ||
#endif
		Gestalt(selector, &response) != noErr)
		response = 0;
	return (response & (1 << test)) != 0;
}
#else
	wsbool WSOneGestalt(OSType selector, long test);
#define WSAudioOneGestalt WSOneGestalt
#endif

#define WSAUDIOSM_NUMDOUBLEBUFFERS 2
#define WSAUDIOSM_USECALLBACKCMD
/*#include <stdio.h>*/
/*#include <time.h>*/
/*time_t start = 0;*/
static pascal void SMOutProc(SndChannelPtr pChannel,
							SndCommand* pCommand)
{
	/*Metrowerks can't handle inline struct initializations */
	/*SndCommand callback = { callBackCmd, 0, pCommand->param2 };*/
	/*pCommand->param1 = !pCommand->param1;*/
#if defined(WSAUDIOSM_USECALLBACKCMD)
	SndCommand PlayCommand
		/*, CallbackCommand*/
	;
	WSAUDIO* pThis = (WSAUDIO*)pCommand->param2;
	/*if(!start) start = time(NULL);*/
	/*printf("NEXTCHUNK:[%zu] TIME:[%zu]\n",
			pThis->mNextChunk, (size_t)(time(NULL) - start));*/


	pThis->mHeader.samplePtr =
		&pThis->mBuffer[pThis->mNextChunk * pThis->mBufferSize];
	PlayCommand.cmd = bufferCmd;
	PlayCommand.param2 = (long)&pThis->mHeader;
	(*pThis->mFillProc)(pThis->mUserData,
		(uint8_t*)pThis->mHeader.samplePtr, pThis->mBufferSize);
	SndDoCommand(pChannel, &PlayCommand, FALSE);
	if(pThis->mNextChunk == WSAUDIOSM_NUMBUFFERS - 1)
		pThis->mNextChunk = 0;
	else
		++pThis->mNextChunk;
	/* requeue callBackCmd */
	SndDoCommand(pChannel, pCommand, FALSE);

	pCommand->cmd = callBackCmd;
#else
	*((wsbool*)pCommand->param2) = TRUE;
#endif
}

#if defined(WSAUDIOSM_USEDOUBLEBUFFER)
static void pascal SMDBOutProc(SndChannelPtr channel,
	SndDoubleBufferPtr dbuffer)
{
	WSAUDIO* pThis = (WSAUDIO*)dbuffer->dbUserInfo[0];
	/* on close down... */
	/*
	dbuffer->dbFlags |= dbBufferReady;
	dbuffer->dbFlags |= dbLastBuffer;
	*/
	(*pThis->mFillProc)(pThis->mUserData,
		(uint8_t*)(&(dbuffer->dbSoundData[0])), pThis->mBufferSize);
	dbuffer->dbFlags |= dbBufferReady;
}
#endif

static pascal void SMInProc(SPBPtr spb)
{
	WSAUDIO* pThis = (WSAUDIO*)spb->userLong;
	Ptr pBuffer = pThis->mBuffer;
	(*pThis->mFillProc)(pThis->mUserData,
		(uint8_t*)pBuffer, pThis->mBufferSize);
	SPBRecord(spb, TRUE);
}


OSErr WSAudioInit()
{
	#ifdef _WIN32
	/*
		Why kInitializeQTMLDisableDirectSound instead of 0?

		Long2Fix has a bug in QTML where it is capped at
		2147483647, and it won't handle negative Fixed values
		it is supposed to, thus a 32000 or below sample rate is OK
		but the standard 44100 gets capped and either goes
		in slow motion, or we do it manually (<< 16/rate44khz) and
		it has unplayable stutter.

		With kInitializeQTMLDisableDirectSound specified
		Long2Fix is still bugged, but if we do it manually
		(sample rate << 16 or through a constant like
		rate44khz) QTML plays the audio correctly and doesn't
		stutter.
	*/
		return InitializeQTML(kInitializeQTMLDisableDirectSound);
	#else
		return noErr;
	#endif
}
void WSAudioUninit()
{
	#ifdef _WIN32
		TerminateQTML();
	#endif
}
void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	Str255 devname;
	WSAUDIODEVICE* newdevs;
	SoundInfoList SampleRatesInfo, SampleSizesInfo;
	short wChannels;
	Component component;
	ComponentDescription looking;
	int numComponents;
	int i;
#if defined(macintosh) && !defined(WSAUDIO_ISCARBON)
	wsbool isInputAvail
		= WSOneGestalt(gestaltSoundAttr, gestaltSoundIOMgrPresent);
#endif
	WSAudioInit();
	enumparams->devices = NULL;
	enumparams->numdevices = 0;
	/*
			INPUT
	*/
#if defined(macintosh) && !defined(WSAUDIO_ISCARBON)
	if(isInputAvail)
#endif
	{
		for(i = 1; ; ++i)
		{
			long refNum;
			short devParam;
			int index = i - 1;
			UnsignedFixed sampleRate;
			char* devnamec = (char*)devname;
			if(SPBGetIndexedDevice(i, devname, NULL) != noErr
				|| SPBOpenDevice(devname, siWritePermission, &refNum) != noErr)
				break; /* done */
			newdevs = WSREALLOC(enumparams->devices, WSAUDIODEVICE,
				enumparams->numdevices + 1);
			if(!newdevs)
			{
				WSFREE(enumparams->devices);
				enumparams->numdevices = 0;
				WSAudioUninit();
				return;
			}
			enumparams->devices = newdevs;
			enumparams->numdevices += 1;
			SPBGetDeviceInfo(refNum, siNumberChannels, &devParam);
			enumparams->devices[index].maxformat.channels = devParam;
			SPBGetDeviceInfo(refNum, siSampleSize, &devParam);
			enumparams->devices[index].maxformat.bitsPerSample = devParam;
			SPBGetDeviceInfo(refNum, siSampleRate, &sampleRate);
			enumparams->devices[index].maxformat.sampleRate = sampleRate >> 16;
			enumparams->devices[index].flags = WSAUDIO_INPUT;
			enumparams->devices[index].id = i;
			enumparams->devices[index].name = StringDupWithEnd(
					&devnamec[1],
					&devnamec[1] + devnamec[0]);
			SPBCloseDevice(refNum);
		}
	}
	/*
			OUTPUT
	*/
	component                      = 0;
	looking.componentType           = kSoundOutputDeviceType; /* 'sdev' */
	looking.componentSubType        = 0;
	looking.componentManufacturer   = 0;
	looking.componentFlags          = 0;
	looking.componentFlagsMask      = 0;
	numComponents = CountComponents (&looking);

	newdevs = WSREALLOC(enumparams->devices, WSAUDIODEVICE,
		enumparams->numdevices + numComponents);
	if(!newdevs)
	{
		WSFREE(enumparams->devices);
		enumparams->numdevices = 0;
		WSAudioUninit();
		return;
	}
	enumparams->devices = newdevs;
	enumparams->numdevices = numComponents;

	for(i = 0; i < numComponents; ++i)
	{
		ComponentDescription found;
		int y;
		Handle hName = NewHandle(0);
		Handle hDesc = NewHandle(0);
		component = FindNextComponent (component, &looking);
		GetComponentInfo(component, &found, hName, hDesc, NULL);
		GetSoundOutputInfo(component,
			siSampleRateAvailable, &SampleRatesInfo);
		GetSoundOutputInfo(component,
			siSampleSizeAvailable, &SampleSizesInfo);
		GetSoundOutputInfo(component, siNumberChannels, &wChannels);

		enumparams->devices[i].flags = 0;
		enumparams->devices[i].id = (size_t)component;
		enumparams->devices[i].maxformat.channels = wChannels;
		enumparams->devices[i].maxformat.bitsPerSample = 0;
		enumparams->devices[i].maxformat.sampleRate = 0;
		for (y = 0; y < SampleRatesInfo.count; ++y)
		{
			uint32_t adjsamplerate = /* infoHandle is a UnsignedFixed** */
				((UnsignedFixed*)*(SampleRatesInfo.infoHandle))[y] >> 16;
			if(adjsamplerate > enumparams->devices[i].maxformat.sampleRate)
				enumparams->devices[i].maxformat.sampleRate = adjsamplerate;
		}
		for (y = 0; y < SampleSizesInfo.count; ++y)
		{
			uint16_t adjsamplesize = /* infoHandle is a uint16_t** */
				((uint16_t*)*(SampleSizesInfo.infoHandle))[y];
			if(adjsamplesize > enumparams->devices[i].maxformat.bitsPerSample)
				enumparams->devices[i].maxformat.bitsPerSample = adjsamplesize;
		}

		if(hName)
		{
			HLock(hName);
			enumparams->devices[i].name = StringDupWithEnd(
				&(*hName)[1], &(*hName)[1] + (*hName)[0]);
			HUnlock(hName);
			DisposeHandle(hName);
		}
		else
			enumparams->devices[i].name = NULL;
		if(hDesc)
		{
			/*enumparams->devices[i].desc = StringDupWithEnd(
				&(*hDesc)[1], &(*hDesc)[1] + (*hDesc)[0]);*/
			DisposeHandle(hDesc);
		}
		/*else
			enumparams->devices[i].desc = NULL;
		*/
	}

	WSAudioUninit();
}
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	WSAudioInit();
#if defined(macintosh) && !defined(WSAUDIO_ISCARBON)
	pThis->mIsInputAvail =
		/*((spec->flags & WSAUDIO_CREATEFROMDEVICE)
			&& (spec->device->flags & WSAUDIO_INPUT)) ||*/
		WSOneGestalt(gestaltSoundAttr, gestaltSoundIOMgrPresent);
#endif
#if defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
	/* Could do SM version if needed */
	/*NumVersion smver = SndSoundManagerVersion();
	 if(smver.majorRev >= 3)*/
	pThis->mIsDoubleBufferAvail =
		WSOneGestalt(gestaltSoundAttr, gestaltSndPlayDoubleBuffer);
	if(pThis->mIsDoubleBufferAvail)
#endif
#if defined(WSAUDIOSM_USEDOUBLEBUFFER)
		pThis->mBuffer =
			NewPtr((sizeof(SndDoubleBuffer) + spec->format.dwBufferSize)
				* WSAUDIOSM_NUMDOUBLEBUFFERS);
#endif
#if defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
	else
#endif
	#if !defined(WSAUDIOSM_USEDOUBLEBUFFER) \
			|| defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
		pThis->mBuffer =
			NewPtr(spec->format.dwBufferSize * WSAUDIOSM_NUMBUFFERS);
	#endif
	if(!pThis->mBuffer
#if defined(macintosh) && !defined(WSAUDIO_ISCARBON)
		/* Sound Input Manager needs to be present for SPB*() */
		|| ((spec->flags & WSAUDIO_INPUT) && !pThis->mIsInputAvail)
#endif
		)
	{
		WSAudioUninit();
		return FALSE;
	}
	pThis->mBufferSize = spec->format.dwBufferSize;
#if defined(macintosh) || defined(WSAUDIO_ISCARBON)
		HoldMemory(pThis->mBuffer, spec->format.dwBufferSize);
#endif
	if(spec->flags & WSAUDIO_INPUT)
	{
		long refNum;
		Str255 devname;
		if(spec->flags & WSAUDIO_CREATEFROMDEVICE)
		{
			if(SPBGetIndexedDevice((short)spec->device->id, devname, NULL) != noErr)
			{
				WSAudioUninit();
				return FALSE;
			}
		}
		else
			devname[0] = 0;

		if(SPBOpenDevice(devname, siWritePermission, &refNum) != noErr)
		{
			WSAudioUninit();
			return FALSE;
		}
		pThis->mSPB.inRefNum = refNum;
		pThis->mSPB.bufferPtr = pThis->mBuffer;
		pThis->mSPB.bufferLength =
			pThis->mSPB.count = spec->format.dwBufferSize;
		pThis->mSPB.milliseconds = 0;
		pThis->mSPB.completionRoutine = NewSICompletionUPP(&SMInProc);
		pThis->mSPB.interruptRoutine = NULL;
		pThis->mSPB.userLong = (long)pThis;
		pThis->mSPB.error = noErr;
		pThis->mSPB.unused1 = 0;
	}
	else /* output */
	{
		OSErr err;
		SndCallBackUPP channelcb;
		int sizeofesh = sizeof(ExtSoundHeader);
		int sizeofwaesh =  sizeof(WSAUDIOExtSoundHeader);
		sizeofwaesh = sizeof(pThis->mHeader);
		pThis->mChannel = NULL;
		#if defined(WSAUDIOSM_USEDOUBLEBUFFER) \
				&& defined(macintosh) && !defined(WSAUDIO_ISCARBON)
		if(pThis->mIsDoubleBufferAvail)
			channelcb = NULL;
		else
		#endif
			channelcb = NewSndCallBackUPP(&SMOutProc);
			pThis->mCallBack = (void*)channelcb;
		/* kUseOptionalOutputDevice opens a non-default device,
		* with a ComponentInstance (the return from FindNextComponent())
		* as the 3rd parameter. Use reInitCmd to change from
		* mono<->stereo when creating from a non-default device */
		/* CHECK: Alloc SndChannelPtr set userInfo and qLength
		* (most seem to use 128)?*/
		/*pThis->mChannel->userInfo = (ptrdiff_t)pThis;
		pThis->mChannel->qLength = 128;*/
		if(spec->flags & WSAUDIO_CREATEFROMDEVICE)
			err = SndNewChannel(&pThis->mChannel, kUseOptionalOutputDevice,
						spec->device->id, channelcb);
		else
			err = SndNewChannel(&pThis->mChannel, sampledSynth,
				spec->format.mSuper.channels == 1 ? initMono : initStereo,
				channelcb);
		if(err != noErr)
		{
			DisposePtr(pThis->mBuffer);
			WSAudioUninit();
			return FALSE;
		}

		#if defined(WSAUDIOSM_USEDOUBLEBUFFER)
		#if defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
			if(pThis->mIsDoubleBufferAvail)
		#endif
		{
			size_t i;
			pThis->mBufferFrames =
				WSAudioFormat_BytesToSamples(&spec->format.mSuper,
					pThis->mBufferSize);
			pThis->mDblHeader.dbhNumChannels =
				spec->format.mSuper.channels;
			pThis->mDblHeader.dbhSampleSize =
				spec->format.mSuper.bitsPerSample;
			pThis->mDblHeader.dbhCompressionID = 0;
			pThis->mDblHeader.dbhPacketSize = 0;
			/* aka Long2Fix(spec->format.mSuper.sampleRate) */
			pThis->mDblHeader.dbhSampleRate =
				spec->format.mSuper.sampleRate << 16;
			pThis->mDblHeader.dbhDoubleBack =
				NewSndDoubleBackUPP(SMDBOutProc);
			for(i = 0; i < WSAUDIOSM_NUMDOUBLEBUFFERS; ++i)
			{
				pThis->mDblHeader.dbhBufferPtr[i]
					= (SndDoubleBufferPtr) (pThis->mBuffer
						+ (i * (sizeof(SndDoubleBuffer)
								+ spec->format.dwBufferSize)));
				pThis->mDblHeader.dbhBufferPtr[i]->dbUserInfo[0]
					= (long)pThis;
				pThis->mDblHeader.dbhBufferPtr[i]->dbNumFrames
					= pThis->mBufferFrames;
				pThis->mDblHeader.dbhBufferPtr[i]->dbFlags = 0;
			}
		}
		#endif
		#if !defined(WSAUDIOSM_USEDOUBLEBUFFER) \
				|| defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
		else
		{
			/*WSMemoryFill(&pThis->mHeader, 0, sizeof(pThis->mHeader));
			Fixed fixsr = Long2Fix(spec->format.mSuper.sampleRate),
				fix44Khz = rate44khz;*/
			pThis->mHeader.samplePtr = pThis->mBuffer;
			pThis->mHeader.numChannels    = spec->format.mSuper.channels;
			pThis->mHeader.sampleRate     = /* fixsr; */
				(spec->format.mSuper.sampleRate) << 16;
			pThis->mHeader.loopStart = 0;
			pThis->mHeader.loopEnd = 0;
			pThis->mHeader.encode         = extSH;/*cmpSH;*/
			pThis->mHeader.baseFrequency = kMiddleC;
			pThis->mHeader.numFrames      = WSAudioFormat_BytesToSamples(
				&spec->format.mSuper, spec->format.dwBufferSize);
#if TARGET_CPU_68K && !TARGET_RT_MAC_68881
			pThis->mHeader.AIFFSampleRate = 0;
#else
			pThis->mHeader.AIFFSampleRate.exp = 0;
			pThis->mHeader.AIFFSampleRate.man[0] = 0;
			pThis->mHeader.AIFFSampleRate.man[1] = 0;
			pThis->mHeader.AIFFSampleRate.man[2] = 0;
			pThis->mHeader.AIFFSampleRate.man[3] = 0;
#endif
			pThis->mHeader.markerChunk = 0;
			pThis->mHeader.instrumentChunks = 0;
			pThis->mHeader.AESRecording = 0;
			pThis->mHeader.sampleSize     =
				spec->format.mSuper.bitsPerSample;
			pThis->mHeader.futureUse1 = 0;
			pThis->mHeader.futureUse2 = 0;
			pThis->mHeader.futureUse3 = 0;
			pThis->mHeader.futureUse4 = 0;
			pThis->mNextChunk = 0;

			#if 0
			pThis->mHeader.compressionID = fixedCompression;
			pThis->mHeader.format = k16BitLittleEndianFormat;
			#endif
		}
		#endif /* USE/CHECK DOUBLE BUFFER */
	}

	pThis->mFlags = spec->flags;
	pThis->mFillProc = spec->proc;
	pThis->mUserData = spec->procuserdata;
	return TRUE;
}

void WSAudio_Destroy(WSAUDIO* pThis)
{
#if defined(macintosh) || defined(WSAUDIO_ISCARBON)
	UnholdMemory(pThis->mBuffer, pThis->mBufferSize);
#endif
	if (pThis->mFlags & WSAUDIO_INPUT)
	{
		SPBCloseDevice(pThis->mSPB.inRefNum);
		DisposeSICompletionUPP((SICompletionUPP)pThis->mSPB.completionRoutine);
	}
	else
	{
		SndDisposeChannel(pThis->mChannel, TRUE);
#if defined(WSAUDIOSM_USEDOUBLEBUFFER)
	#if defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
		if(pThis->mIsDoubleBufferAvail)
	#endif
			DisposeSndDoubleBackUPP(
				(SndDoubleBackUPP)pThis->mDblHeader.dbhDoubleBack);
	#if defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
		else
	#endif
#endif
			DisposeSndCallBackUPP((SndCallBackUPP)pThis->mCallBack);
	}
	DisposePtr(pThis->mBuffer);
	WSAudioUninit();
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int leftpan, int rightpan)
{ /* Bytes 1-4 == left channel, 5-8 == right channel */
	SndCommand cmd;
	WSASSERT(!(pThis->mFlags & WSAUDIO_INPUT));
	cmd.cmd = volumeCmd;
	cmd.param2 = (rightpan << 16) | leftpan;
	return (SndDoCommand(pThis->mChannel, &cmd, true) == noErr);
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{ /* Bytes 1-4 == left channel, 5-8 == right channel */
	SndCommand cmd;
	WSASSERT(!(pThis->mFlags & WSAUDIO_INPUT));
	cmd.cmd = volumeCmd;
	/*	Sound Manager's volumeCmd expects a fixed-point number with the
	high-order word specifying the left channel volume and the
	low-order word the right channel volume, both in the range of 0 to 255. */
	/* Set both left and right channels to the same volume */
	cmd.param2 = (volume << 16) | volume;
	return SndDoCommand(pThis->mChannel, &cmd, TRUE) == noErr;
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	return 255; /* == kFullVolume */
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	return 0;
}
wsbool WSAudio_QueueBuffer(WSAUDIO* pThis)
{
	SndCommand command;
	OSErr err;
	pThis->mHeader.samplePtr =
		&pThis->mBuffer[pThis->mNextChunk * pThis->mBufferSize];

	(*pThis->mFillProc)(pThis->mUserData,
		(uint8_t*)pThis->mHeader.samplePtr,
		pThis->mBufferSize);
	command.cmd = bufferCmd;
	command.param2 = (long) &pThis->mHeader;
	err = SndDoCommand(pThis->mChannel, &command, FALSE);
	if(err != noErr)
		return FALSE;
	command.cmd = callBackCmd;
	command.param1 = 0;
#if defined(WSAUDIOSM_USECALLBACKCMD)
	command.param2 = (long)pThis;
#else
	command.param2 = (long)&pThis->mBufferReady[pThis->mNextChunk];
#endif
	err = SndDoCommand(pThis->mChannel, &command, FALSE);
	if(err != noErr)
		return FALSE;

	if(pThis->mNextChunk == WSAUDIOSM_NUMBUFFERS - 1)
		pThis->mNextChunk = 0;
	else
		++pThis->mNextChunk;
	return TRUE;
}
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	if(pThis->mFlags & WSAUDIO_INPUT)
		return SPBRecord((SPBPtr)&pThis->mSPB, TRUE);
	else
	{
#if defined(WSAUDIOSM_USEDOUBLEBUFFER)
		OSErr err;
		#if defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
			if(pThis->mIsDoubleBufferAvail)
		#endif
		{
			if(pThis->mDblHeader.dbhBufferPtr[0]->dbFlags == 0)
			{ /* get the double buffers ready */
				size_t i;
				for(i = 0; i < WSAUDIOSM_NUMDOUBLEBUFFERS; ++i)
				{
					InvokeSndDoubleBackUPP(pThis->mChannel,
						pThis->mDblHeader.dbhBufferPtr[i],
						(SndDoubleBackUPP)pThis->mDblHeader.dbhDoubleBack);
				}
			}
			err = SndPlayDoubleBuffer(pThis->mChannel,
				(SndDoubleBufferHeaderPtr)&pThis->mDblHeader);
			if(err != noErr)
				return FALSE;
		}
		#if defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
			else
		#endif
#endif
		#if !defined(WSAUDIOSM_USEDOUBLEBUFFER) \
				|| defined(WSAUDIOSM_CHECKDOUBLEBUFFER)
		{
			#if defined(WSAUDIOSM_USECALLBACKCMD)
				size_t i;
				for(i = 0; i < WSAUDIOSM_NUMBUFFERS; ++i)
			#endif
					if(!WSAudio_QueueBuffer(pThis))
						return FALSE;
		}
		#endif
		return TRUE;
	}
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{ /* SndChannelStatus -> SCStatus->scChannelPaused - 7+ */
#if defined(macintosh) && !defined(WSAUDIO_ISCARBON)
	if(pThis->mIsInputAvail) /* Sound Input Manager guarantees System 7 */
#endif
	{
		if(pThis->mFlags & WSAUDIO_INPUT)
		{/* 	SPBGetRecordingStatus */
			short recordingStatus, meterLevel;
			unsigned long totalSamplesToRecord, numberOfSamplesRecorded,
					totalMillisToRecord, numberOfMillisRecorded;
			return SPBGetRecordingStatus(pThis->mSPB.inRefNum,
					&recordingStatus, &meterLevel,
					&totalSamplesToRecord, &numberOfSamplesRecorded,
					&totalMillisToRecord, &numberOfMillisRecorded) == noErr
				&& recordingStatus > 0;
		}
		else
		{
			SCStatus theStatus;
			return SndChannelStatus(pThis->mChannel,
					sizeof(theStatus), &theStatus) == noErr
				&& !theStatus.scChannelPaused;
		}
	}
	return FALSE;
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	if(pThis->mFlags & WSAUDIO_INPUT)
		return SPBPauseRecording(pThis->mSPB.inRefNum) == noErr;
	else
	{
		SndCommand command;
		command.cmd = pauseCmd;
		return SndDoCommand(pThis->mChannel, &command, TRUE) == noErr;
	}
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	if(pThis->mFlags & WSAUDIO_INPUT)
		return SPBResumeRecording(pThis->mSPB.inRefNum) == noErr;
	else
	{
		SndCommand command;
		command.cmd = resumeCmd;
		return SndDoCommand(pThis->mChannel, &command, TRUE) == noErr;
	}
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	/* Not possible in Sound Manager */
	return 0;
}
wsbool WSAudioHasFullDuplex() { return FALSE; }
void WSAudio_Process(WSAUDIO* pThis)
{ /* handled in callback */
#if !defined(WSAUDIOSM_USECALLBACKCMD)
	if(pThis->mBufferReady[pThis->mNextChunk])
		WSAudio_QueueBuffer(pThis);
#endif
}
#if defined(__cplusplus)
} /* end extern "C" */
#endif
/*=============================================================================
								  #######  #####
								# #     # #     #
								# #     # #
								# #     #  #####
								# #     #       #
								# #     # #     #
								# #######  #####

	  #####  ####### ######  #######       #    #     # ######  ### #######
	 #     # #     # #     # #            # #   #     # #     #  #  #     #
	 #       #     # #     # #           #   #  #     # #     #  #  #     #
	 #       #     # ######  #####      #     # #     # #     #  #  #     #
	 #       #     # #   #   #          ####### #     # #     #  #  #     #
	 #     # #     # #    #  #          #     # #     # #     #  #  #     #
	  #####  ####### #     # #######    #     #  #####  ######  ### #######
=============================================================================*/
#elif defined(__MACH__) && defined(WSAUDIOMACIOS)
#if defined(__OBJC__)
	#import <AVFoundation/AVFoundation.h>
#endif
#include <AudioToolbox/AudioToolbox.h>
#include <AudioUnit/AudioUnit.h>
#if defined(__cplusplus)
extern "C" {
#endif
void WSAudioAVInputCallback(void* userData, AudioQueueRef aq,
	AudioQueueBufferRef aqb, const AudioTimeStamp* inStartTime,
	UInt32 numpacketdescs, const AudioStreamPacketDescription* packetdescs)
{
	WSAUDIO* pAudio = (WSAUDIO*)userData;
	if(numpacketdescs > 0)
		(*pAudio->mFillProc)(pAudio->mUserData,
			aqb->mAudioData, aqb->mAudioDataByteSize);
	AudioQueueEnqueueBuffer(aq, aqb, 0, NULL);
}
void WSAudioAVOutputCallback(void* userData, AudioQueueRef aq,
	AudioQueueBufferRef aqb)
{
	WSAUDIO* pAudio = (WSAUDIO*)userData;
	(*pAudio->mFillProc)(pAudio->mUserData,
		aqb->mAudioData, aqb->mAudioDataByteSize);
	AudioQueueEnqueueBuffer(aq, aqb, 0, NULL);
}
wsbool WSAudioHasFullDuplex(void) { return 0; }
void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
#if defined(__OBJC__)
	enumparams->devices = WSALLOC(WSAUDIODEVICE, 2);
	if(enumparams->devices)
	{
		AVAudioSession* shared = [AVAudioSession sharedInstance];
		enumparams->devices[0].flags = 0;
		enumparams->devices[1].flags = WSAUDIO_INPUT;
		enumparams->devices[0].id = enumparams->devices[1].id = 0;
		enumparams->devices[0].name = enumparams->devices[1].name = NULL;
		WSAudioFormat_Construct(&enumparams->devices[0].maxformat,
			[shared outputNumberOfChannels], 16, [shared sampleRate]);
		enumparams->devices[1].maxformat = enumparams->devices[0].maxformat;
		return;
	}
#endif
	enumparams->devices = NULL;
	enumparams->numdevices = 0;
}
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	AudioStreamBasicDescription aqformat;
	OSStatus err;
	size_t ibuffer;
	aqformat.mBitsPerChannel = spec->format.mSuper.bitsPerSample;
	aqformat.mChannelsPerFrame = spec->format.mSuper.channels;
	aqformat.mSampleRate = spec->format.mSuper.sampleRate;
	aqformat.mFormatID = kAudioFormatLinearPCM;
	aqformat.mFormatFlags = kAudioFormatFlagIsSignedInteger
		| kAudioFormatFlagIsPacked;
	aqformat.mFramesPerPacket = 1;
	aqformat.mBytesPerFrame = aqformat.mChannelsPerFrame
		* (aqformat.mBitsPerChannel / 8);
	aqformat.mBytesPerPacket = aqformat.mBytesPerFrame
		* aqformat.mFramesPerPacket;
	if(spec->flags & WSAUDIO_INPUT)
		err = AudioQueueNewInput(&aqformat, WSAudioAVInputCallback, pThis,
			CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 0,
			&pThis->mAudioQueue);
	else
		err = AudioQueueNewOutput(&aqformat, WSAudioAVOutputCallback, pThis,
			CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 0,
			&pThis->mAudioQueue);
	if(err != noErr)
		return FALSE;
	for(ibuffer = 0; ibuffer < WSAUDIOAVNUMBUFFERS; ++ibuffer)
	{
		wsbool failed =
			AudioQueueAllocateBuffer(pThis->mAudioQueue,
				(UInt32)(spec->format.dwBufferSize / WSAUDIOAVNUMBUFFERS),
				&pThis->mAudioBuffers[ibuffer]) != noErr;
		if(!failed)
		{
			failed = AudioQueueEnqueueBuffer(
				pThis->mAudioQueue, pThis->mAudioBuffers[ibuffer], 0, NULL) != noErr;
			if(failed)
				AudioQueueFreeBuffer(pThis->mAudioQueue, pThis->mAudioBuffers[ibuffer]);
		}
		if(failed)
		{
			size_t ibuffercleanup;
			for(ibuffercleanup = 0; ibuffercleanup < ibuffer; ++ibuffercleanup)
				AudioQueueFreeBuffer(pThis->mAudioQueue,
					pThis->mAudioBuffers[ibuffercleanup]);
			AudioQueueDispose(pThis->mAudioQueue, false);
			return FALSE;
		}
	}
	pThis->mFillProc = spec->proc;
	pThis->mUserData = spec->procuserdata;
	pThis->mFlags = spec->flags;
	return TRUE;
}
void WSAudio_Destroy(WSAUDIO* pThis)
{
	size_t ibuffercleanup;
	#if defined(__OBJC__)
		NSError* nserr;
	#endif
	AudioQueueFlush(pThis->mAudioQueue);
	AudioQueueStop(pThis->mAudioQueue, false);
	for(ibuffercleanup = 0; ibuffercleanup < WSAUDIOAVNUMBUFFERS; ++ibuffercleanup)
		AudioQueueFreeBuffer(pThis->mAudioQueue,
			pThis->mAudioBuffers[ibuffercleanup]);
	AudioQueueDispose(pThis->mAudioQueue, false);
	#if defined(__OBJC__)
		[[AVAudioSession sharedInstance] setActive:NO error:&nserr];
	#endif
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright) { return 0; }
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume) { return 0; }
int WSAudio_GetMaxVolume(WSAUDIO* pThis) { return 0; }
int WSAudio_GetMinVolume(WSAUDIO* pThis) { return 0; }
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	#if defined(__OBJC__)
		NSError* nserr;
		if(pThis->mFlags & WSAUDIO_INPUT)
		{
			[[AVAudioSession sharedInstance]
				setCategory:AVAudioSessionCategoryRecord error:&nserr];
			if(nserr)
			{
				NSLog(@"Error setting AVAudioSession to record: %@", [nserr localizedDescription]);
				return FALSE;
			}
			[[AVAudioSession sharedInstance] setActive:YES error:&nserr];
			if(nserr)
			{
				NSLog(@"Error setting AVAudioSession to active for record: %@",
					[nserr localizedDescription]);
				return FALSE;
			}
		}
	#endif

	return AudioQueueStart(pThis->mAudioQueue, NULL) == noErr;
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis) { return 0; }
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	return AudioQueuePause(pThis->mAudioQueue) == noErr;
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	return AudioQueueStart(pThis->mAudioQueue, NULL) == noErr;
}
size_t WSAudio_GetPosition(WSAUDIO* pThis) { return 0; }
void WSAudio_Process(WSAUDIO* pThis) {}
#if defined(__cplusplus)
} /* end extern "C" */
#endif

/*=============================================================================
							 #######  #####  #     #
							 #     # #     #  #   #
							 #     # #         # #
							 #     #  #####     #
							 #     #       #   # #
							 #     # #     #  #   #
							 #######  #####  #     #

	  #####  ####### ######  #######       #    #     # ######  ### #######
	 #     # #     # #     # #            # #   #     # #     #  #  #     #
	 #       #     # #     # #           #   #  #     # #     #  #  #     #
	 #       #     # ######  #####      #     # #     # #     #  #  #     #
	 #       #     # #   #   #          ####### #     # #     #  #  #     #
	 #     # #     # #    #  #          #     # #     # #     #  #  #     #
	  #####  ####### #     # #######    #     #  #####  ######  ### #######
=============================================================================*/
#elif defined(__MACH__)
#include <CoreAudio/CoreAudio.h>
#include <AudioToolbox/AUGraph.h>
#include <AudioUnit/AudioUnitProperties.h>
#include <stdlib.h>
#include <string.h>
#if defined(__cplusplus)
extern "C" {
#endif
OSStatus RecordCallback(void* pParam, AudioUnitRenderActionFlags* pFlags,
						 const AudioTimeStamp* pTime, UInt32 dwBusNumber,
						 UInt32 dwNumFrames, AudioBufferList* pBufferList)
{
	WSAUDIO* pAudio = (WSAUDIO*)pParam;
	AudioBufferList realBufferList;
	(void)(pBufferList);
	realBufferList.mBuffers[0].mNumberChannels = pAudio->format.channels;
	realBufferList.mBuffers[0].mDataByteSize = WSAudioFormat_SamplesToBytes(
		&pAudio->format, dwNumFrames);
	realBufferList.mBuffers[0].mData = pAudio->inputbuffer;
	realBufferList.mNumberBuffers = 1;

	if(AudioUnitRender(pAudio->kUnit, pFlags,
		pTime, dwBusNumber, dwNumFrames, &realBufferList) == noErr)
		(*pAudio->mFillProc)(pAudio->mUserData,
			pAudio->inputbuffer, realBufferList.mBuffers[0].mDataByteSize);
	return noErr;
}
OSStatus RenderCallback(void* pParam, AudioUnitRenderActionFlags* pFlags,
						 const AudioTimeStamp* pTime, UInt32 dwBusNumber,
						 UInt32 dwNumFrames, AudioBufferList* pBufferList)
{
	WSAUDIO* pAudio = (WSAUDIO*)pParam;
	uint32_t i;
	for (i = 0; i < pBufferList->mNumberBuffers; i++)
		(*pAudio->mFillProc)(pAudio->mUserData,
			(uint8_t*)pBufferList->mBuffers[i].mData,
			pBufferList->mBuffers[i].mDataByteSize);
	return noErr;
}
const char* WSAudioCAErrorToString(OSStatus e)
{
#define WSAudioCAErrorToStringLiteral(e, error) \
	if(e == error) return #error;
	/* HAL errors*/
	WSAudioCAErrorToStringLiteral(e, kAudioHardwareNotRunningError);
	WSAudioCAErrorToStringLiteral(e, kAudioHardwareUnspecifiedError);
	WSAudioCAErrorToStringLiteral(e, kAudioHardwareUnknownPropertyError);
	WSAudioCAErrorToStringLiteral(e, kAudioHardwareBadPropertySizeError);
	WSAudioCAErrorToStringLiteral(e, kAudioHardwareIllegalOperationError);
	WSAudioCAErrorToStringLiteral(e, kAudioHardwareBadDeviceError);
	WSAudioCAErrorToStringLiteral(e, kAudioHardwareBadStreamError);
	/* AUgraph errors*/
	WSAudioCAErrorToStringLiteral(e, kAUGraphErr_NodeNotFound);
	WSAudioCAErrorToStringLiteral(e, kAUGraphErr_InvalidConnection);
	WSAudioCAErrorToStringLiteral(e, kAUGraphErr_OutputNodeErr);
	WSAudioCAErrorToStringLiteral(e, kAUGraphErr_CannotDoInCurrentContext);
	WSAudioCAErrorToStringLiteral(e, kAUGraphErr_InvalidAudioUnit);
	/* AudioUnit errors */
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_InvalidProperty);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_InvalidParameter);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_InvalidElement);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_NoConnection);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_FailedInitialization);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_TooManyFramesToProcess);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_IllegalInstrument);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_InstrumentTypeNotFound);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_InvalidFile);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_UnknownFileType);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_FileNotSpecified);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_FormatNotSupported);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_Uninitialized);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_InvalidScope);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_PropertyNotWritable);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_InvalidPropertyValue);
	WSAudioCAErrorToStringLiteral(e, kAudioUnitErr_CannotDoInCurrentContext);
	return "Unknown";
}


wsbool WSAudioHasFullDuplex()
{ return TRUE; }
void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	AudioDeviceID* pIDs;
	int32_t i, k, nIDs;
	UInt32 nSize;
	WSAUDIODEVICE* newdevs;
	Boolean bWritable;
	enumparams->devices = NULL;
	enumparams->numdevices = 0;
	if(AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices,
		&nSize, &bWritable) != noErr)
		return; /* error */
	nIDs = nSize / sizeof(AudioDeviceID);
	pIDs = WSALLOC(AudioDeviceID, nIDs);
	if(!pIDs)
		return;
	if(AudioHardwareGetProperty(kAudioHardwarePropertyDevices,
		&nSize, pIDs) != noErr)
	{
		WSFREE(pIDs);
		return;
	}

	for (i = 0; i < nIDs; ++i)
	{
		int32_t nDescs;
		AudioStreamBasicDescription* pStreamDescs;
		char* szName;
		CFStringRef cfsname;
		WSAUDIOFORMAT* maxformat;
		UInt32 size;
		OSStatus status;
		int devflags = 0;
		/* device has input streams - i.e. supports input (Recording)? */
		status = AudioDeviceGetPropertyInfo(pIDs[i], 0, false, kAudioDevicePropertyStreamConfiguration, &size, NULL);
		if (status == noErr && size > 0)
			devflags = WSAUDIO_INPUT;
		/* device has output streams - i.e. supports output (Render)? */
		status = AudioDeviceGetPropertyInfo(pIDs[i], 0, true, kAudioDevicePropertyStreamConfiguration, &size, NULL);
		if (status == noErr && size > 0)
			devflags |= WSAUDIO_OUTPUT;
		else if(!(devflags & WSAUDIO_INPUT))
			continue; /* supports neither input or output */

		if(AudioDeviceGetPropertyInfo(pIDs[i], 0, 0,
			/*kAudioDevicePropertyStreamFormats*/
			kAudioStreamPropertyPhysicalFormats, &nSize, &bWritable) != noErr)
			continue;
		nDescs = nSize / sizeof(AudioStreamBasicDescription);
		pStreamDescs = WSALLOC(AudioStreamBasicDescription, nDescs);
		if(!pStreamDescs)
			continue;
		if(AudioDeviceGetProperty( pIDs[i], 0, 0,
				kAudioStreamPropertyPhysicalFormats,
				&nSize, pStreamDescs) != noErr)
			continue;
		/*kAudioDevicePropertyBufferFrameSize
		kAudioDevicePropertyBufferFrameSize*/
		/* grow/realloc devices */
		newdevs = WSREALLOC(enumparams->devices, WSAUDIODEVICE,
				enumparams->numdevices + 1);
		if(!newdevs)
		{
			WSFREE(pStreamDescs);
			WSFREE(pIDs);
			return; /* can't realloc devices... done */
		}
		enumparams->devices = newdevs;
		maxformat =
			&enumparams->devices[enumparams->numdevices].maxformat;
		enumparams->devices[enumparams->numdevices].id = pIDs[i];
		enumparams->devices[enumparams->numdevices].flags = devflags;
		++enumparams->numdevices;
		WSAudioFormat_ConstructDefault(maxformat);

		/* get name/manufacturer */
		nSize = sizeof(CFStringRef);
		if(AudioDeviceGetProperty(pIDs[i], 0, 0,
				kAudioDevicePropertyDeviceName, &nSize, &cfsname) == noErr)
		{
			nSize = CFStringGetMaximumSizeForEncoding(
				CFStringGetLength(cfsname), kCFStringEncodingUTF8);
			szName = WSALLOC(char, nSize + 1);
			if(szName)
			{
				if(!CFStringGetCString(cfsname, szName,
						nSize + 1, kCFStringEncodingUTF8))
				{
					WSFREE(szName);
					szName = NULL;
				}
			}
		}
		enumparams->devices[enumparams->numdevices].name = szName;
		for (k = 0; k < nDescs; ++k)
		{
			if(pStreamDescs[k].mSampleRate > maxformat->sampleRate)
			{
				maxformat->sampleRate = pStreamDescs[k].mSampleRate;
				maxformat->bitsPerSample = pStreamDescs[k].mBitsPerChannel;
				maxformat->channels = pStreamDescs[k].mChannelsPerFrame;
			}
		}
		/* szName is handled by WSAUDIODEVICE */
		WSFREE(pStreamDescs);
	}
	WSFREE(pIDs);
}
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	ComponentDescription kDesc;
	AudioStreamBasicDescription destDesc;
	AURenderCallbackStruct kInRenderCallbackStruct, kOutRenderCallbackStruct;
	UInt32 size, val;
	AUNode kUnitNode, kMixerNode;
	UInt32 disableoutput = (spec->flags & WSAUDIO_INPUT)
		&& !(spec->flags & WSAUDIO_OUTPUT);
	UInt32 enableinput = (spec->flags & WSAUDIO_INPUT) == WSAUDIO_INPUT;

	kDesc.componentType = kAudioUnitType_Output;
	kDesc.componentSubType = kAudioUnitSubType_HALOutput;
	kDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
	kDesc.componentFlags          = 0;
	kDesc.componentFlagsMask      = 0;

	if(NewAUGraph(&pThis->kGraph) != noErr)
		return FALSE;
	destDesc.mSampleRate = spec->format.mSuper.sampleRate;
	destDesc.mFormatID = kAudioFormatLinearPCM;
	destDesc.mFormatFlags = kAudioFormatFlagIsSignedInteger
		| kAudioFormatFlagIsPacked;
	destDesc.mBytesPerPacket = spec->format.mSuper.channels
		* (spec->format.mSuper.bitsPerSample / 8);
	destDesc.mFramesPerPacket = 1;
	destDesc.mBytesPerFrame = destDesc.mBytesPerPacket;
	destDesc.mChannelsPerFrame = spec->format.mSuper.channels;
	destDesc.mBitsPerChannel = spec->format.mSuper.bitsPerSample;
	size = sizeof(destDesc);
	if(AUGraphAddNode(pThis->kGraph, &kDesc, &kUnitNode) != noErr
		|| AUGraphOpen(pThis->kGraph) != noErr)
	{
		DisposeAUGraph(pThis->kGraph);
		return FALSE;
	}
	/* Create audio unit for full duplex I/O */
	kDesc.componentType = kAudioUnitType_FormatConverter;
	kDesc.componentSubType = kAudioUnitSubType_AUConverter;
	kDesc.componentManufacturer = kAudioUnitManufacturer_Apple;
	kDesc.componentFlags = 0;
	kDesc.componentFlagsMask = 0;
	if(AUGraphAddNode(pThis->kGraph, &kDesc, &kMixerNode) != noErr
		|| AUGraphInitialize(pThis->kGraph) != noErr)
	{
		AUGraphClose(pThis->kGraph);
		DisposeAUGraph(pThis->kGraph);
		return FALSE;
	}
	kOutRenderCallbackStruct.inputProc = RenderCallback;
	kOutRenderCallbackStruct.inputProcRefCon = pThis;
	kInRenderCallbackStruct.inputProc = RecordCallback;
	kInRenderCallbackStruct.inputProcRefCon = pThis;
	AUGraphGetNodeInfo(pThis->kGraph, kUnitNode,
				NULL, NULL, NULL, &pThis->kUnit);
	AUGraphGetNodeInfo(pThis->kGraph, kMixerNode,
				NULL, NULL, NULL, &pThis->kMixer);

	if(spec->device)
	{ /* Set the device if the user had a specific one chosen */
		if(AudioUnitSetProperty(pThis->kUnit,
			kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global,
			0, &spec->device->id, sizeof(AudioDeviceID)) != noErr)
		{
			AUGraphUninitialize(pThis->kGraph);
			AUGraphClose(pThis->kGraph);
			DisposeAUGraph(pThis->kGraph);
			return FALSE;
		}
	}

	if(/* set input audio input format (i.e. whats its mixing from) */
		AudioUnitSetProperty(pThis->kUnit,
				kAudioUnitProperty_StreamFormat,
				kAudioUnitScope_Input, 0, &destDesc, size) != noErr
		/* connect the mixer node that will convert input->output format */
		|| AUGraphConnectNodeInput(pThis->kGraph, kMixerNode, 0,
				kUnitNode, 0) != noErr
		/* disable output for input only and set output callback
		note that output is enabled by default */
		|| (!disableoutput && (AudioUnitSetProperty(pThis->kUnit,
				kAudioOutputUnitProperty_EnableIO,
				kAudioUnitScope_Output, 1,
				&disableoutput, sizeof(disableoutput)) != noErr
			|| AudioUnitSetProperty(pThis->kMixer,
				kAudioUnitProperty_SetRenderCallback,
				kAudioUnitScope_Input, 0, &kInRenderCallbackStruct,
				sizeof(AURenderCallbackStruct)) != noErr)
		/* enable input if supported and set input callback
		note that input is disabled by default */
		|| (enableinput && (AudioUnitSetProperty(pThis->kUnit,
				kAudioOutputUnitProperty_EnableIO,
				kAudioUnitScope_Input, 1,
				&enableinput, sizeof(enableinput)) != noErr
			|| AudioUnitSetProperty(pThis->kMixer,
				kAudioUnitProperty_SetRenderCallback,
				kAudioUnitScope_Output, 0, &kOutRenderCallbackStruct,
				sizeof(AURenderCallbackStruct)) != noErr))
		))
	{
		AUGraphUninitialize(pThis->kGraph);
		AUGraphClose(pThis->kGraph);
		DisposeAUGraph(pThis->kGraph);
		return FALSE;
	}
	if(enableinput)
	{
		size = sizeof(val);
		if(AudioUnitGetProperty(pThis->kInputUnit,
				kAudioUnitProperty_MaximumFramesPerSlice,
				kAudioUnitScope_Global, 0, &val, &size) != noErr)
			pThis->inputbuffer = NULL;
		else
			pThis->inputbuffer = WSALLOC(uint8_t, val * destDesc.mBytesPerFrame);
		if(!pThis->inputbuffer)
		{
			AUGraphUninitialize(pThis->kGraph);
			AUGraphClose(pThis->kGraph);
			DisposeAUGraph(pThis->kGraph);
			return FALSE;
		}
	}
	else
		pThis->inputbuffer = NULL;
	pThis->mFlags = spec->flags;
	pThis->mFillProc = spec->proc;
	pThis->mUserData = spec->procuserdata;
	pThis->format = spec->format.mSuper;
	return TRUE;
}
void WSAudio_Destroy(WSAUDIO* pThis)
{
	AUGraphStop(pThis->kGraph);
	AUGraphUninitialize(pThis->kGraph);
	AUGraphClose(pThis->kGraph);
	DisposeAUGraph(pThis->kGraph);
	WSFREE(pThis->inputbuffer);
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright)
{
	float maxVolume = (float)WSAudio_GetMaxVolume(pThis);
	float value = -((float)panleft / maxVolume)
		+ ((float)panright / maxVolume);
	return AudioUnitSetParameter(pThis->kMixer,
		kAudioUnitParameterUnit_Pan, kAudioUnitScope_Output,
		0, value, 0) == noErr;
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{
	float maxVolume = (float)WSAudio_GetMaxVolume(pThis);
	float value = (float)volume / maxVolume;
	return AudioUnitSetParameter(pThis->kMixer,
		kAudioUnitParameterUnit_LinearGain, kAudioUnitScope_Output,
		0, value, 0) == noErr;
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	(void)(pThis);
	return INT_MAX;
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	(void)(pThis);
	return 0;
}
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	return AUGraphStart(pThis->kGraph) == noErr;
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{
	Boolean isrunning;
	if(AUGraphIsRunning(pThis->kGraph, &isrunning) != noErr)
		return FALSE;
	return isrunning;
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	return AUGraphStop(pThis->kGraph) == noErr;
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	return AUGraphStart(pThis->kGraph) == noErr;
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	AudioTimeStamp curtimestamp;
	UInt32 dataSize = sizeof(curtimestamp);
	if(AudioUnitGetProperty(pThis->kUnit, kAudioUnitProperty_CurrentPlayTime,
		kAudioUnitScope_Global, 0, &curtimestamp, &dataSize) != noErr)
		return 0;
	if (curtimestamp.mSampleTime == -1.0) /* means graph is stopped */
		curtimestamp.mSampleTime = 0;
	return WSAudioFormat_SamplesToBytes(
		&pThis->format, curtimestamp.mSampleTime);
}
void WSAudio_Process(WSAUDIO* pThis)
{ (void)(pThis); return; /* Handled by callbacks */ }
#if defined(__cplusplus)
} /* end extern "C" */
#endif
/*=============================================================================
	 #     # #     # ### #     #       #    #        #####     #
	 #     # ##    #  #   #   #       # #   #       #     #   # #
	 #     # # #   #  #    # #       #   #  #       #        #   #
	 #     # #  #  #  #     #       #     # #        #####  #     #
	 #     # #   # #  #    # #      ####### #             # #######
	 #     # #    ##  #   #   #     #     # #       #     # #     #
	  #####  #     # ### #     #    #     # #######  #####  #     #
=============================================================================*/
#elif defined(WSAUDIOALSAOPT) && (defined(__FreeBSD__) \
	|| defined(__DragonFly__) || defined(__OpenBSD__) || defined(__linux__))
#ifdef HAVE_ALSA_ASOUNDLIB_H
	#include <alsa/asoundlib.h>
#else
	#include <sys/asoundlib.h>
#endif
/*=============================================================================
						ALSA IMPLEMENTATION
		https://www.alsa-project.org/alsa-doc/alsa-lib/pcm.html
=============================================================================*/
#if defined(__cplusplus)
extern "C" {
#endif
void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	int card = -1;
	snd_ctl_t* handle;
	snd_ctl_card_info_t* info;
	snd_pcm_hw_params_t* params;
	snd_pcm_format_mask_t* format;
	snd_pcm_t* pcm;
	int dir;
	unsigned int minrate, maxrate, rate;
	unsigned int minchannels, maxchannels;
	int bitsPerSample;
	WSAUDIODEVICE* newdevs;
	enumparams->numdevices = 0;
	enumparams->devices = NULL;
	snd_ctl_card_info_alloca(&info);
	snd_pcm_hw_params_alloca(&params);
	snd_pcm_format_mask_alloca(&format);
	do /* open "default" (card == -1) first, then all physical cards */
	{
		char cardname[DigitToStringMaxChars() + 4];
		if(card == -1)
			WSMemoryCopy(cardname, "default", sizeof("default"));
		else
		{
			WSMemoryCopy(cardname, "hw:", strcountof("hw:"));
			cardname[DigitToString(cardname, card)] = '\0';
		}

		if(snd_ctl_open(&handle, cardname, 0) < 0)
			continue;
		if(snd_ctl_card_info(handle, info) < 0)
		{
			snd_ctl_close(handle);
			continue;
		}

		if (snd_pcm_open(&pcm,
				cardname,
				SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) < 0)
		{
			snd_ctl_close(handle);
			continue;
		}
		snd_pcm_hw_params_any(pcm, params);
		snd_pcm_hw_params_get_format_mask(params, format);

#ifdef IS_BIG_ENDIAN
		if(snd_pcm_format_mask_test(format, SND_PCM_FORMAT_S32_BE)
			|| snd_pcm_format_mask_test(format,SND_PCM_FORMAT_U32_BE))
			bitsPerSample = 32;
		else if(snd_pcm_format_mask_test(format, SND_PCM_FORMAT_S24_BE)
			|| snd_pcm_format_mask_test(format, SND_PCM_FORMAT_U24_BE))
			bitsPerSample = 24;
		else if(snd_pcm_format_mask_test(format, SND_PCM_FORMAT_S16_BE)
			|| snd_pcm_format_mask_test(format, SND_PCM_FORMAT_U16_BE))
			bitsPerSample = 16;
#else
		if(snd_pcm_format_mask_test(format, SND_PCM_FORMAT_S32_LE)
			|| snd_pcm_format_mask_test(format,SND_PCM_FORMAT_U32_LE))
			bitsPerSample = 32;
		else if(snd_pcm_format_mask_test(format, SND_PCM_FORMAT_S24_LE)
			|| snd_pcm_format_mask_test(format, SND_PCM_FORMAT_U24_LE))
			bitsPerSample = 24;
		else if(snd_pcm_format_mask_test(format, SND_PCM_FORMAT_S16_LE)
			|| snd_pcm_format_mask_test(format, SND_PCM_FORMAT_U16_LE))
			bitsPerSample = 16;
#endif
		else if(snd_pcm_format_mask_test(format, SND_PCM_FORMAT_S8)
			|| snd_pcm_format_mask_test(format, SND_PCM_FORMAT_U8))
			bitsPerSample = 8;
		else
		{
			snd_pcm_close(pcm);
			snd_ctl_close(handle);
			continue;
		}

		newdevs = WSREALLOC(enumparams->devices, WSAUDIODEVICE,
			enumparams->numdevices + 1);
		if(!newdevs)
		{
			snd_pcm_close(pcm);
			snd_ctl_close(handle);
			break;
		}
		enumparams->devices = newdevs;
		snd_pcm_hw_params_get_rate_max(params, &maxrate, &dir);
		snd_pcm_hw_params_get_channels_max(params, &maxchannels);

		enumparams->devices[enumparams->numdevices].maxformat.bitsPerSample
			= bitsPerSample;
		enumparams->devices[enumparams->numdevices].maxformat.channels
			 = maxchannels;
		enumparams->devices[enumparams->numdevices].maxformat.sampleRate
			= maxrate;
		enumparams->devices[enumparams->numdevices].id
			= UDigitVal(snd_ctl_card_info_get_id(info));
		enumparams->devices[enumparams->numdevices].name
			= StringDup(cardname);
		/*snd_ctl_card_info_get_name(info)*/
		snd_pcm_close(pcm);
		snd_ctl_close(handle);
		++enumparams->numdevices;
	} while(snd_card_next(&card) >= 0 && card >= 0);
}

wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	snd_mixer_selem_id_t *sid;
	unsigned int setting;
	int dir;
	int ret;
	unsigned int periods;
	snd_pcm_uframes_t periodsize;
	const char* cardname =
		(spec->flags & WSAUDIO_CREATEFROMDEVICE) ?
			spec->device->name : "default";
	/* open the pcm device */
	if (snd_pcm_open(&pThis->mPcm,
				cardname,
				SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK) < 0)
		return FALSE;
	/* convert the bits per sample to the ALSA format */
	switch(spec->format.mSuper.bitsPerSample)
	{
#ifdef IS_BIG_ENDIAN
	case 32:
		setting = SND_PCM_FORMAT_S32_BE;
		break;
	case 24:
		setting = SND_PCM_FORMAT_S24_BE;
		break;
	case 16:
		setting = SND_PCM_FORMAT_S16_BE;
		break;
#else
	case 32:
		setting = SND_PCM_FORMAT_S32_LE;
		break;
	case 24:
		setting = SND_PCM_FORMAT_S24_LE;
		break;
	case 16:
		setting = SND_PCM_FORMAT_S16_LE;
		break;
#endif
	default:
		setting = SND_PCM_FORMAT_S8;
		break;
	}

	/* set the pThis->mHwparams including access permissions */
	ret = snd_pcm_hw_params_malloc(&pThis->mHwparams);
	ret = snd_pcm_hw_params_any(pThis->mPcm,
		pThis->mHwparams); /* clear with default values */
	ret = snd_pcm_hw_params_set_access(pThis->mPcm, pThis->mHwparams,
		SND_PCM_ACCESS_RW_INTERLEAVED);
	ret = snd_pcm_hw_params_set_format(pThis->mPcm, pThis->mHwparams,
		(snd_pcm_format_t)setting);
	setting = (unsigned int)spec->format.mSuper.channels;
	ret = snd_pcm_hw_params_set_channels(pThis->mPcm, pThis->mHwparams,
		setting);
	setting = (unsigned int)spec->format.mSuper.sampleRate;
	ret = snd_pcm_hw_params_set_rate_near(pThis->mPcm, pThis->mHwparams,
		&setting, &dir);
	periodsize = spec->format.dwBufferSize;
	ret = snd_pcm_hw_params_set_period_size_near(pThis->mPcm,
		pThis->mHwparams, &periodsize, NULL);
	setting = 5;
	ret = snd_pcm_hw_params_set_periods_near(pThis->mPcm,
		pThis->mHwparams, &setting, NULL);
	/* Allocate our buffer and commit the audio format changes to the device */
	ret = snd_pcm_hw_params(pThis->mPcm, pThis->mHwparams);
	ret = snd_pcm_hw_params_get_period_size(pThis->mHwparams,
		&periodsize, NULL);
	ret = snd_pcm_hw_params_get_periods(pThis->mHwparams, &periods, NULL);

	ret = snd_pcm_sw_params_malloc(&pThis->mSwparams);
	ret = snd_pcm_sw_params_set_start_threshold(pThis->mPcm,
		pThis->mSwparams, periodsize);
	ret = snd_pcm_sw_params_set_avail_min(pThis->mPcm,
		pThis->mSwparams, spec->format.dwBufferSize);
	ret = snd_pcm_sw_params(pThis->mPcm, pThis->mSwparams);
	if(ret >= 0)
	{
		pThis->mBufferSize = WSAudioFormat_SamplesToBytes(
			&spec->format.mSuper, periodsize);
		pThis->mBufferFrames = periodsize;/*WSAudioFormat_BytesToSamples(
			&spec->format.mSuper, pThis->mBufferSize);*/
		pThis->mBuffer = WSALLOC(char, pThis->mBufferSize);
		pThis->mPollFDCount = snd_pcm_poll_descriptors_count(pThis->mPcm);
		if (pThis->mPollFDCount <= 0)
			ret = -1;
		else
		{
			pThis->mPollFDs = WSALLOC(struct pollfd, pThis->mPollFDCount);
			if(!pThis->mPollFDs)
				ret = -2;
			else
				ret = snd_pcm_poll_descriptors(pThis->mPcm,
						pThis->mPollFDs, pThis->mPollFDCount);
		}
	}
	if(ret < 0 || !pThis->mBuffer)
	{
		snd_pcm_close(pThis->mPcm);
		snd_pcm_hw_params_free(pThis->mHwparams);
		WSFREE(pThis->mBuffer);
		return FALSE;
	}
	if(!(snd_mixer_open(&pThis->mMixer, 0) < 0))
	{
		pThis->mMixerElem = 0;
		snd_mixer_selem_id_alloca(&sid);
		snd_mixer_selem_id_set_index(sid, 0);
		snd_mixer_selem_id_set_name(sid, "Master"); /* Input is "Capture" */
		if(snd_mixer_attach(pThis->mMixer, cardname) < 0
			|| snd_mixer_selem_register(pThis->mMixer, NULL, NULL) < 0
			|| snd_mixer_load(pThis->mMixer) < 0
			|| (pThis->mMixerElem = snd_mixer_find_selem(pThis->mMixer, sid)) == NULL
			|| snd_mixer_selem_get_playback_volume_range(pThis->mMixerElem,
				&pThis->mMinVolume, &pThis->mMaxVolume) < 0
			)
		{
			pThis->mMinVolume = pThis->mMaxVolume = 0;
			snd_mixer_close(pThis->mMixer);
			pThis->mMixer = NULL;
		}
	}
	else
	{
		pThis->mMinVolume = pThis->mMaxVolume = 0;
		pThis->mMixer = NULL;
	}
	pThis->mBlockAlign = WSAudioFormat_GetBlockAlign(
		&spec->format.mSuper);
	pThis->mGotAgainI = pThis->mGotAgainO = FALSE;
	pThis->mFlags = spec->flags;
	pThis->mUserData = spec->procuserdata;
	pThis->mFillProc = spec->proc;
	return TRUE;
}
void WSAudio_Destroy(WSAUDIO* pThis)
{
	if(pThis->mMixer)
		snd_mixer_close(pThis->mMixer);
	snd_pcm_close(pThis->mPcm);
	snd_pcm_hw_params_free(pThis->mHwparams);
	snd_pcm_sw_params_free(pThis->mSwparams);
	WSFREE(pThis->mBuffer);
	WSFREE(pThis->mPollFDs);
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright)
{
	return snd_mixer_selem_set_playback_volume(pThis->mMixerElem,
			SND_MIXER_SCHN_FRONT_LEFT, panleft) == 0
		&& snd_mixer_selem_set_playback_volume(pThis->mMixerElem,
			SND_MIXER_SCHN_FRONT_LEFT, panright) == 0;
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{
	return snd_mixer_selem_set_playback_volume_all(
		pThis->mMixerElem, volume) == 0;
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	return (int)pThis->mMaxVolume;
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	return (int)pThis->mMinVolume;
}
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	return snd_pcm_prepare(pThis->mPcm) == 0
		&& snd_pcm_start(pThis->mPcm) == 0; /* start isn't needed for output */
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{
	return snd_pcm_state(pThis->mPcm) == SND_PCM_STATE_RUNNING;
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	return snd_pcm_pause(pThis->mPcm, 1);
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	return snd_pcm_pause(pThis->mPcm, 0);
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	snd_pcm_sframes_t numframes = snd_pcm_avail(pThis->mPcm);
	if(numframes < 0)
		return 0;
	return numframes * pThis->mBlockAlign;
}
int WSAudio_HandleALSAProcess(WSAUDIO* pThis, long err)
{
	int badfd = -EBADFD;
	int pipe = -EPIPE;
	int strpipe = -ESTRPIPE;
	int intr = -EINTR;
	if (err == -EPIPE) /* underrun */
		err = snd_pcm_prepare(pThis->mPcm);
	else if (err != 0)
		err = snd_pcm_recover(pThis->mPcm, err, 0);
	return err;
}
wsbool WSAudioHasFullDuplex() { return TRUE; }
void WSAudio_Process(WSAUDIO* pThis)
{
	long numframes;
	if(pThis->mFlags & WSAUDIO_INPUT)
	{
		snd_pcm_sframes_t numread = snd_pcm_readi(pThis->mPcm,
			pThis->mBuffer, pThis->mBufferFrames);
		pThis->mGotAgainI = numread == -EAGAIN;
		if(!pThis->mGotAgainI)
		{
			if(numread < 0)
				WSAudio_HandleALSAProcess(pThis, numread);
			(*pThis->mFillProc)(pThis->mUserData,
				(uint8_t*)pThis->mBuffer, pThis->mBufferSize);
		}
	}
	if(!(pThis->mFlags & WSAUDIO_INPUT) || (pThis->mFlags & WSAUDIO_OUTPUT))
	{
		snd_pcm_sframes_t numwritten;
		if(!pThis->mGotAgainO)
			(*pThis->mFillProc)(pThis->mUserData,
				(uint8_t*)pThis->mBuffer, pThis->mBufferSize);
		numwritten = snd_pcm_writei(pThis->mPcm,
			pThis->mBuffer, pThis->mBufferFrames);
		pThis->mGotAgainO = numwritten == -EAGAIN;
		if(!pThis->mGotAgainO)
		{
			if(numwritten < 0)
				WSAudio_HandleALSAProcess(pThis, numwritten);
		}
	}
}
#if defined(__cplusplus)
} /* end extern "C" */
#endif
/*=============================================================================
		 #     # #     # ### #     #    #######  #####   #####
		 #     # ##    #  #   #   #     #     # #     # #     #
		 #     # # #   #  #    # #      #     # #       #
		 #     # #  #  #  #     #       #     #  #####   #####
		 #     # #   # #  #    # #      #     #       #       #
		 #     # #    ##  #   #   #     #     # #     # #     #
		  #####  #     # ### #     #    #######  #####   #####
=============================================================================*/
#elif (defined(__FreeBSD__) \
	|| defined(__DragonFly__) || defined(__OpenBSD__) || defined(__linux__))
#include <unistd.h> /* close() */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <math.h> /* sqrt() */
#include <errno.h>

/*=============================================================================
						OSS IMPLEMENTATION
						http://www.opensound.com/pguide/oss.pdf
=============================================================================*/
#if defined(__cplusplus)
extern "C" {
#endif

void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	WSMUTSTR dsploc;
	char* dspbuf;
	size_t i, j;
	enumparams->devices = NULL;
	enumparams->numdevices = 0;

	/*
		/dev/dsp#\0
		/dev/dsp == 8
		# == max itoa chars
		\0 == 1
	*/
	WSMutStr_CreateWithSize(&dsploc, 8 + DigitToStringMaxChars() + 1);
	WSMutStr_BeforeWrite(&dsploc, WSMutStr_GetCapacity(&dsploc));
	dspbuf = (char*)WSMutStr_GetCString(&dsploc);
	WSMemoryCopy(dspbuf, "/dev/dsp", 8);

	for(i = 0; i < INT_MAX; ++i)
	{
		int oss;
		int setting = 1;
		int flags[3] = { WSAUDIO_INPUT | WSAUDIO_OUTPUT, WSAUDIO_INPUT, 0 };
		int resflags;
		int openmodes[3] = { O_RDWR, O_WRONLY, O_RDONLY };
		int samplerates[5] = { 96000, 48000, 44100, 22050, 11025 };
		size_t numdspnumchars;
		wsbool isDone = FALSE;
		WSAUDIOFORMAT format;
		if(i) /* /dev/dsp->/dev/dsp0->/dev/dsp1 */
			numdspnumchars = UDigitToString(&dspbuf[8], i - 1);
		else
			numdspnumchars = 0;
		dspbuf[8 +  numdspnumchars] = '\0';
		for(j = 0; j < countof(flags); ++j)
		{
			oss = open(dspbuf, openmodes[j]);
			if(oss != -1)
			{
				resflags = flags[j];
				break;
			}
			if(errno != EACCES)
			{
				isDone = TRUE;
				break;
			}
		}
		if(j == countof(flags))
			continue;
		if(isDone)
			break;

		format.channels =
			ioctl(oss, SNDCTL_DSP_STEREO, &setting) < 0 ? 1 : 2;

		if(ioctl(oss, SNDCTL_DSP_GETFMTS, &setting) < 0)
			continue;
		format.bitsPerSample = setting &
	#ifdef IS_BIG_ENDIAN
			AFMT_S16_BE
	#else
			AFMT_S16_LE
	#endif
			? 16 : 8;

		for(j = 0; j < countof(samplerates); ++j)
		{
			setting = samplerates[j];
			if(ioctl(oss, SNDCTL_DSP_SPEED, &setting) >= 0)
			{
				format.sampleRate = setting;
				break;
			}
		}
		close(oss);
		if(j == countof(samplerates))
			continue;
		enumparams->devices = WSREALLOC(enumparams->devices,
			WSAUDIODEVICE, enumparams->numdevices + 1);
		enumparams->devices[enumparams->numdevices].id
			= i;
		enumparams->devices[enumparams->numdevices].flags
			= resflags;
		enumparams->devices[enumparams->numdevices].maxformat
			= format;
		enumparams->devices[enumparams->numdevices].name
			= StringDup(dspbuf);
		++enumparams->numdevices;
	}

	WSMutStr_AfterWrite(&dsploc, 0);
	WSMutStr_Destroy(&dsploc);
}
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec)
{
	int openmode;
	int fd;
	int setting;
	int ret;
	audio_buf_info bi;
	if(spec->flags & WSAUDIO_INPUT)
	{
		if(spec->flags & WSAUDIO_OUTPUT)
			openmode = O_RDWR;
		else
			openmode = O_RDONLY;
	}
	else
		openmode = O_WRONLY;

	if((spec->flags & WSAUDIO_CREATEFROMDEVICE)
		&& spec->device->id)
	{
		WSMUTSTR dsploc;
		char* dspbuf;
		WSMutStr_CreateWithSize(&dsploc, 8 + DigitToStringMaxChars() + 1);
		WSMutStr_BeforeWrite(&dsploc, WSMutStr_GetCapacity(&dsploc));
		dspbuf = (char*)WSMutStr_GetCString(&dsploc);
		WSMemoryCopy(dspbuf, "/dev/dsp", 8);
		WSMutStr_AfterWriteAndNullTerminate(&dsploc,
			8 + UDigitToString(&dspbuf[8], spec->device->id - 1));
		fd = open(WSMutStr_GetCString(&dsploc), openmode);
		WSMutStr_Destroy(&dsploc);
	}
	else
		fd = open("/dev/dsp", openmode);
	if(fd == -1)
		return FALSE;

	/* low 16 - sqrt(buffersize) high 16 - # fragments */
	for ( setting = 0; (0x01 << setting) < spec->format.dwBufferSize;
		++setting ) {}
	if ((0x01 << setting) != spec->format.dwBufferSize)
		return FALSE; /* not a power of 2, otherwise buffer size is
						some obscure low value */
	setting |= (8 << 16); /* set to 8 fragments */
	ret = ioctl(fd, SNDCTL_DSP_SETFRAGMENT, &setting);

	setting = spec->format.mSuper.channels - 1; /* 0 - mono 1 - stereo */
	ret = ioctl(fd, SNDCTL_DSP_STEREO, &setting);

	setting = spec->format.mSuper.bitsPerSample == 8 ? AFMT_S8 : AFMT_S16_NE;
	ret = ioctl(fd, SNDCTL_DSP_SETFMT, &setting);

	setting = spec->format.mSuper.sampleRate;
	ret = ioctl(fd, SNDCTL_DSP_SPEED, &setting);

	if(ioctl(fd, SNDCTL_DSP_GETOSPACE, &bi) < 0)
	{
		close(fd);
		return FALSE;
	}

	pThis->mBufferSize = bi.bytes;
	pThis->mBuffer = WSALLOC(char, pThis->mBufferSize);
	if(!pThis->mBuffer)
	{
		close(fd);
		return FALSE;
	}
	pThis->mFd = fd;
	pThis->mFlags = spec->flags;
	pThis->mUserData = spec->procuserdata;
	pThis->mFillProc = spec->proc;
	return TRUE;
}
void WSAudio_Destroy(WSAUDIO* pThis)
{
	WSFREE(pThis->mBuffer);
	close(pThis->mFd);
}
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright)
{
	return FALSE;
}
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume)
{
#ifdef SNDCTL_DSP_SETPLAYVOL /* Solaris only */
	return ioctl(pThis->mFd, SNDCTL_DSP_SETPLAYVOL, volume) != -1;
#else
	return FALSE;
#endif
}
int WSAudio_GetMaxVolume(WSAUDIO* pThis)
{
	return 100; /* Solaris only */
}
int WSAudio_GetMinVolume(WSAUDIO* pThis)
{
	return 0; /* Solaris only */
}
wsbool WSAudio_Play(WSAUDIO* pThis)
{
	return TRUE; /* No need to do anything, read/write always plays */
}
wsbool WSAudio_IsPlaying(WSAUDIO* pThis)
{
	return TRUE; /* Not available in ioctl OSS */
}
wsbool WSAudio_Pause(WSAUDIO* pThis)
{
	return FALSE; /* Not available in ioctl OSS */
}
wsbool WSAudio_Resume(WSAUDIO* pThis)
{
	return FALSE; /* Not available in ioctl OSS */
}
size_t WSAudio_GetPosition(WSAUDIO* pThis)
{
	int nPos = 0;
	ioctl(pThis->mFd, SNDCTL_DSP_GETODELAY, &nPos);
	return nPos;
}
wsbool WSAudioHasFullDuplex() { return TRUE; }
void WSAudio_Process(WSAUDIO* pThis)
{
	audio_buf_info info;
	ssize_t rw;
	if(pThis->mFlags & WSAUDIO_INPUT)
	{
		if(ioctl(pThis->mFd, SNDCTL_DSP_GETISPACE, &info) == 0)
		{
			rw = read(pThis->mFd, pThis->mBuffer, info.bytes);
			(*pThis->mFillProc)(pThis->mUserData,
				(uint8_t*)pThis->mBuffer, info.bytes);
		}
	}
	if(!(pThis->mFlags & WSAUDIO_INPUT) || (pThis->mFlags & WSAUDIO_OUTPUT))
	{
		if(ioctl(pThis->mFd, SNDCTL_DSP_GETOSPACE, &info) == 0)
		{
			(*pThis->mFillProc)(pThis->mUserData,
				(uint8_t*)pThis->mBuffer, info.bytes);
			rw = write(pThis->mFd, pThis->mBuffer, info.bytes);
		}
	}
	/*        WSOSSASSERT(ioctl(handle, SNDCTL_DSP_SYNC));*/
}
#if defined(__cplusplus)
} /* end extern "C" */
#endif
/******************************************************************************
				 ##### #     #       #    #    #####
				 #     ##    #      #    # #       #
				 #     # #   #     #    #   #      #
				 #     #  #  #    #    #     #     #
				 #     #   # #   #     #######     #
				 #     #    ##  #      #     #     #
				 ##### #     # #       #     # #####
******************************************************************************/
#else
#if defined(__cplusplus)
extern "C" {
#endif
wsbool WSAudioHasFullDuplex() { return 0; }
void WSAudioEnumerate(WSAUDIOENUM* enumparams)
{
	enumparams->devices = NULL;
	enumparams->numdevices = 0;
}
wsbool WSAudio_Create(WSAUDIO* pThis, WSAUDIOSPEC* spec) { return 0; }
void WSAudio_Destroy(WSAUDIO* pThis) { }
wsbool WSAudio_SetPan(WSAUDIO* pThis, int panleft, int panright) { return 0; }
wsbool WSAudio_SetVolume(WSAUDIO* pThis, int volume) { return 0; }
int WSAudio_GetMaxVolume(WSAUDIO* pThis) { return 0; }
int WSAudio_GetMinVolume(WSAUDIO* pThis) { return 0; }
wsbool WSAudio_Play(WSAUDIO* pThis) { return 0; }
wsbool WSAudio_IsPlaying(WSAUDIO* pThis) { return 0; }
wsbool WSAudio_Pause(WSAUDIO* pThis) { return 0; }
wsbool WSAudio_Resume(WSAUDIO* pThis) { return 0; }
size_t WSAudio_GetPosition(WSAUDIO* pThis) { return 0; }
void WSAudio_Process(WSAUDIO* pThis) {}
#if defined(__cplusplus)
} /* end extern "C" */
#endif
#endif /* OS SWITCH */
#endif /* WSAUDIO_IMPLEMENTATION */
