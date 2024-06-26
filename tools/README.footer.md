Goals
---
Goals of these libraries:
* Cross-platform, particularly the Macintosh classic and eventually X68000
* Extreme compatibility. That means options for dynloading everything on Windows
  that would break compatibility with Windows 95.

	Some things can't be helped. I.E. Macintosh did not support joysticks
	before System 7.
 
* Compile down to hand-written code. That means no virtual methods/OOP.
* No global variables unless required.

  	Currently there is one in the GLX backend of GLCONTEXT

* Be completely self-contained.
* Compile on old compilers such as MSVC6 and CodeWarrior Pro 6 and MPW. This means:
	* No C++ comments in C code
  	* Explicit function pointer invocation (  (*func)(param) )
  	* No templates
  	* No local array initialization
  	* All variables must be declared at the beginning of a block

Notes
---
Note that all files have 4 space tabs.

* [audio.h](audio.h) - WSAudioEnumerate()->WSAudio_Create()->WSAudio_Play()

	NOTE: Audio input (recording) is experimental currently.

	WSAudioEnumerate allocates the passed in array, you don't pass in your own.
	
		That part of the API might need a rethink though it is simple...
	
	Enumeration is optional - you can just pass in a NULL audio device and 0 or WSAUDIO_INPUT for the flags and let it automatically select.
	
	After that your callback passed to WSAudio_Create will feed it data
	
	By default on Windows WMME is used for maximum compatibility. 
	
	WSAUDIOWASOPT, WSAUDIODSOPT, WSAUDIOKSOPT, WSAUDIOXAUDIOOPT are options 
	on Windows for Windows Audio Streaming (the most preferred and modern API), 
	DirectSound, Kernel Streaming (won't work on a lot of modern headphones), 
	and XAudio2. WSAUDIOSMOPT is also an option if you want to use the old QTML 		
	(QuickTime Media Layer) on Windows.
		
	The priority order for which is used on Windows goes:
	WSAUDIOSMOPT->WSAUDIOXAUDIOOPT->WSAUDIOWASOPT->WSAUDIOKSOPT->WSAUDIODSOPT
		
	Again, if none are specified WMME is used on Windows. 
		
	WSAUDIOALSAOPT is the Unix option, otherwise OSS is used.
	
	When done call WSAudio_Destroy

	#define WSAUDIO_IMPLEMENTATION before including the header in the one source file you want the implementation in.

  	Designed for older systems than the more feature complete miniaudio.

  	Officially supports: Win32 (MMSYSTEM, DSound, KS, XAudio2, WAS), Mac (SM), Unix (OSS/ALSA)

  	Compiles on but low/no testing: OSX(CA)/iOS(CA)/AMIGA

* [diff.h](diff.h) - Diff_Create()->Diff_Compare()

	After that you can do Diff_FillBlocks() to get the difference. You need to provide your own array to Diff_FillBlocks().
	You can call it once beforehand to get the size of the array of DIFFBLOCK you need.
		
		Despite the member variable names the "lines" can be characters or words
		
		Pass DIFF_COMPARE_BYCHAR/DIFF_COMPARE_BYWORD to get this behavior
	
	DIFF_COMPARE_NOUTF8 is currently non-functional and only ANSI is used publicly.

	Note that by default Diff_Create will take two MEMORYSTREAM structure pointers. If you want different behavior you can override the Stream_XXX defines. I.E. if you want to use FILE* and routines you'd do:

  	```c
	#define Stream_Seek(stream, pos, flag) \
		(fseek((FILE*)stream, (long)pos, flag) == 0 ? pos : -1)
	#define Stream_Read(stream, data, count) \
		fread(data, 1, (long)count, (FILE*)stream)
	#define Stream_Write(stream, data, count) \
		fwrite(data, 1, (long)count, (FILE*)stream)
	#define Stream_Tell(stream) ftell((FILE*)stream)
	```

	See [examples/difffilec.c](examples/difffilec.c) for an example of this.
 
	When done call Diff_Destroy()
	
	#define DIFF_IMPLEMENTATION before including the header in the one source file you want the implementation in.
	

* [display.h](display.h) - Call WSDisplayBegin() to begin iteration, calling until WSDisplayIsEnd(), call WSDisplayIterator_Iterate() and WSDisplayIterator_Value() in-between them.

	You can just call WSDisplayGetPrimary() if you don't want to iterate through them
	
	WSDisplayGetFromWindow is only on Windows, just use WSDisplayGetFromRect with your window's window rect (not client rect) on other platforms.
	
	When done call enumerating call, WSDisplayIterator_Destroy() and when done with each display from WSDisplayIterator_Value() or WSDiplayGetDefault() call WSDisplay_Destroy()

	#define WSDISPLAY_IMPLEMENTATION before including the header in the one source file you want the implementation in.

  	Officially supports: Win32, X11 (XF86+Xinerama or Xrandr)

  	Compiles on but low/no testing: iOS/OSX/AMIGA/X68000

* [glcontext.h](glcontext.h) - GLContext_Create()->GLContext_SetCurrent()

	If you don't pass in pixel attributes you get a context with empty attributes, which often isn't desirable. Call:
	
	```c
	GLPixelAttributes_Create(&pa);
	GLPixelAttributes_PlatformDefaults(&pa);
	GLPixelAttributes_Depth(&pa, 24);
	GLPixelAttributes_EndList(&pa);
	```
 
	For a more sensible default.
	
	If you want EGL instead of WGL #define HAVE_GTK before including the header.
		
	When done call GLContext_Destroy()

	#define GLCONTEXT_IMPLEMENTATION before including the header in the one source file you want the implementation in.

	Officially supports: Win32, EGL, X11

	Compiles on but low/no testing: Mac classic/carbon, AMIGA, OSX, iOS
 
* [joystick.h](joystick.h) - Call WSJoystickBegin() to begin iteration, calling until WSJoystickIsEnd(), call WSJoystickIterator_Iterate() and WSJoystickIterator_Value() in-between them.

	Call WSJoystick_GetCaps() to get info on the joystick. You only really need to do this once
	
	Call WSJoystick_PollState() to get the state of the joystick. The results aren't necessarily cached, so save the results from the calls.

	When done call enumerating call, WSJoystickIterator_Destroy() and when done with each display from WSJoystickIterator_Value() call WSJoystick_Destroy()

	On Windows #define WSJOYSTICKXINPUTOPT before the header to use XInput instead of MMSYSTEM.
	
	#define WSJOYSTICK_IMPLEMENTATION before including the header in the one source file you want the implementation in.

	Officially supports: Win32(mmsystem/XInput), Linux(/dev/input/js#, /dev/js#)

	Compiles on but low/no testing: Mac, OSX, BSD, X68000, AMIGA, iOS 7+

* [mutstr.h](mutstr.h) - WSMutStr_Create[XXX]

	If you want to manipulate the internal data yourself from WSMutStr_GetCString(), currently call WSMutStr_BeforeWrite then WSMutStr_AfterWrite() when done. Yes, the API for manipulating the internal data isn't great and requires a cast...

	When done call WSMutStr_Destroy

	#define WSMUTSTR_IMPLEMENTATION before including the header in the one source file you want the implementation in.

* [re.h](re.h) - regcomp()->regexec()

	It's the 30+ year old POSIX api. If you don't pass REG_EXTENDED|REG_ENHANCED to regcomp() you get those 30+ year old features only.
	It supports everything I know of from PHP regular expressions including lookbehinds except inline modifiers and named subexpressions.
	
	regfree() when done.
	
	#define RE_IMPLEMENTATION before including the header in the one source file you want the implementation in.	

* [server.h](server.h) - There are 3 levels of abstraction from lowest to highest: WSSOCKET, WSSOCKETNOTIFIER, and SERVER.

	Before you do anything you need to create a WSSOCKETLIBRARY via WSSocketLibrary_Create() (closing it with WSSocketLibrary_Destroy() when done).

  	WSSOCKET is the basic socket interface. If you really want to you could call WSSocket_Create[Server/Client]()->WSSocket_WaitFor[Read/Write]() then read/write/accept. This isn't recommeded though

  	WSSOCKETNOTIFIER wraps around native waiting interfaces such as Epoll or Completion Ports and probably the lowest level interface you should use to create objects. Call WSSocketNotifier_Create()->WSSocketNotifier_Add[Listener/Client]Socket to get started. After this during idle call WSSocketNotifier_Begin()->WSSocketNotifier_Next() to iterate through the events. These take two objects: WSSOCKETNOTIFIERINFO and WSSOCKETNOTIFIERPOST. Check the Post first - it can be NULL - it will contain completed/partial read/writes. The Info contains the socket and will tell you if the socket can be read/write/accept from the socket.

  	SERVER is high level interface that invokes 3 callbacks for connection, disconnection, and read/write done passed to Server_Construct. Server_Create will start the server up at a specified port, and you can call Server_DistributeSocket to add a socket for it to handle if you really want to. Call Server_Process() on your idle time to have it process the callbacks. Call Server_Destroy() when done.

  	Compiling with SSL functionality is possible, though it doesn't work well. SChannel is also an option on Windows, though it only compiles in C++ mode currently due to using new/delete still.

  	Extra care was taken to make sure all methods work on old systems. I.E. there's some code in there to get the DNS server from the MacTCP resource files for that backend.

	#define WSSERVER_IMPLEMENTATION before including the header in the one source file you want the implementation in.

  	Abstracts APIs like select, Epoll, Kqueue and the 3 WIN32 APIs such as Completion Ports
  	
  	Obtaining the IP/Hostname in string form requires the WSMUTSTR library for now

	Include uri.h with URI_IMPLEMENTATION before WSSOCKET_IMPLEMENTATION to avoid code duplication

	Supports: Win32/Unix/Macintosh (OT+MACTCP)
	
	Compiles on but low/no testing: AMIGA/X68000

* [test.h](test.h) - Declare functions with WSTEST_FUNCTION(NAME) that call WSTEST_ASSERT(x) within them. After that make a map of your functions - the "GROUPS" are just for categorization. The WSTEST_FUNCTION ones are the WSTEST_CASE ones:

	```c
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
	WSTEST_SUITE_END(WSREGEX)
	```
	
After that's done all you need to do is call 

	WSTEST_SUITE_RUN(WSREGEX);

Currently it only outputs to the console.

   #define WSTEST_IMPLEMENTATION before including the header in the one source file you want the implementation in.
	
* [uri.h](uri.h) - URI_Create()

	Once that is called the URI is parsed and you can call Get/Has methods, Unescape etc. Uri is a C++ wrapper that encapsulates all of this.
	
	MutUri is a C++ class that offers a lot more features but also performs a handful of allocations to provide these features (converting it to C is a TODO)

	When done call URI_Destroy()

	#define URI_IMPLEMENTATION before including the header in the one source file you want the implementation in.

WIP
---

examples/gui.h (examples/guihelloworldc[pp].c[pp])

Experimental weightless GUI framework


FAQ
---

#### What's the license?

These libraries are in the public domain. You can do anything you
want with them. You have no legal obligation
to do anything else, although I appreciate attribution.

They are also licensed under the 0BSD open source license, if you have lawyers
who are unhappy with public domain. Every source file includes an explicit
dual-license for you to choose from.

#### How do I use these libraries?

The idea behind single-header file libraries is that they're easy to distribute and deploy
because all the code is contained in a single file. By default, the .h files in here act as
their own header files, i.e. they declare the functions contained in the file but don't
actually result in any code getting compiled.

So in addition, you should select _exactly one_ C/C++ source file that actually instantiates
the code, preferably a file you're not editing frequently. This file should define a
specific macro (this is documented per-library) to actually enable the function definitions.
For example, to use stb_image, you should have exactly one C/C++ file that doesn't
include stb_image.h regularly, but instead does

    #define WSAUDIO_IMPLEMENTATION
    #include "audio.h"

The right macro to define is pointed out in the notes above.

#### <a name="other_libs"></a> Are there other single-file public-domain/open source libraries with minimal dependencies out there?

[Yes.](https://github.com/nothings/single_file_libs)

#### If I wrap an stb library in a new library, does the new library have to be public domain/0BSD?

No, because it's public domain you can freely relicense it to whatever license your new
library wants to be.

#### Why do you list "lines of code"? It's a terrible metric.

Just to give you some idea of the internal complexity of the library,
to help you manage your expectations, or to let you know what you're
getting into. While not all the libraries are written in the same
style, they're certainly similar styles, and so comparisons between
the libraries are probably still meaningful.

Note though that the lines do include both the implementation, the
part that corresponds to a header file, and the documentation.

#### Why single-file headers?

Windows doesn't have standard directories where libraries
live. That makes deploying libraries in Windows a lot more
painful than open source developers on Unix-derivates generally
realize. (It also makes library dependencies a lot worse in Windows.)

There's also a common problem in Windows where a library was built
against a different version of the runtime library, which causes
link conflicts and confusion. Shipping the libs as headers means
you normally just compile them straight into your project without
making libraries, thus sidestepping that problem.

Making them a single file makes it very easy to just
drop them into a project that needs them. (Of course you can
still put them in a proper shared library tree if you want.)

Why not two files, one a header and one an implementation?
The difference between 10 files and 9 files is not a big deal,
but the difference between 2 files and 1 file is a big deal.
You don't need to zip or tar the files up, you don't have to
remember to attach *two* files, etc.

#### Do you have any advice on how to create my own single-file library?

Yes. https://github.com/nothings/stb/blob/master/docs/stb_howto.txt

#### Why public domain?

I prefer it over GPL, LGPL, BSD, zlib, etc. for many reasons.
Some of them are listed here:
https://github.com/nothings/stb/blob/master/docs/why_public_domain.md

#### Why C?

Primarily, because I use C, not C++. But it does also make it easier
for other people to use them from other languages.

#### Why not C99? stdint.h, declare-anywhere, etc.

Older systems don't support C++ well as all, and I use 
CodeWarrior Pro 6 (2000) for cross-compilation.
