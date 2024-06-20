#
# CMake minimum version
#
cmake_minimum_required (VERSION 3.18)

#
#
# Compile options for which features you want, command line is -D FEATURE=OFF
#
#
macro(wsoption ARGV1 ARGV2 ARGV3)
	option(${ARGV1} ${ARGV2} ${ARGV3})
	if(${ARGV1})
		add_definitions(-DWS${ARGV1})
		message(STATUS "======OPTION [${ARGV1}] is ++ENABLED++=======")
	else()
		message(STATUS "======OPTION [${ARGV1}] is --DISABLED--=======")
	endif()
endmacro()
wsoption (SSLOPT "Enable SSL with OpenSSL for https handling, about 2k\n
requires the dynamic library installed" OFF)
wsoption (WOLFSSLOPT "Enable SSL with WolfSSL for https handling\n
	if WolfSSL isn't loaded OpenSSL is used instead, SSL option required'"  OFF)
wsoption (NTVSSLOPT "Enable SSL with SChannel on Windows, about 4k\n
	SSL option required, takes priority over other options\n
	and removes any third party library dependencies" OFF)
wsoption (EPOLLOPT "Linux: Use epoll for socket polling" ON)
wsoption (KQUEUEOPT "Unux: Use kqueue for socket polling (FreeBSD/OSX 10.3+)" ON)
wsoption (IOCPOPT "Windows: Use IOCP (NT 3.5+) to poll sockets, "
"takes priority over normal sockets, EVENTSELECTOPT and ASYNCSELECTOPT if available" OFF)
wsoption (ACCEPTEXOPT "Windows: Use AcceptEx to preallocate sockets if available, "
"requires IOCP or EVENTSELECT" OFF)
wsoption (EVENTSELECTOPT "Windows: Use WSAEventSelect to poll sockets, "
"takes priority over normal sockets and ASYNCSELECTOPT if available" OFF)
wsoption (CONNECTEXOPT "Windows: Use ConnectEx for socket connection to connect a socket and send data in the same operation" ON)
wsoption (DISCONNECTEXOPT "Windows: Use DisconnectEx for socket disconnection that allows reuse of sockets" ON)
wsoption (ASYNCSELECTOPT "Windows: Use WSAAsyncSelect to poll sockets, takes priority over normal sockets if available" ON)
wsoption (MEMORYDEBUGOPT "Adds compile flags for Address Sanitizer memory debugging" OFF)
wsoption (MUTURIOPT "Make the MutURI class available. A changable URI variation" ON)
wsoption (GTK4OPT "Use GTK4 for *nux. While a couple years old, support for most distros is still sketchy in 2023." OFF)
wsoption (MSIXOPT "Use WinRT for calls banned for MSIX apps on Windows." ON)
wsoption (KDEOPT "Use KDE instead of GTK." OFF)
wsoption (AMIGAGCCOPT "Cross compile using GCC Amiga." OFF)
wsoption (AUDIOWMMEOPT "Enable WMME (Win32) WSAudio Backend." OFF)
wsoption (AUDIODSOPT "Enable DirectSound WSAudio Backend." ON)
wsoption (AUDIOKSOPT "Enable Windows Kernel Streaming WSAudio Backend." ON)
wsoption (AUDIOWASOPT "Enable WASAPI (Vista+) WSAudio Backend." ON)
wsoption (AUDIOSMOPT "Enable Sound Manager (Mac/QTML) WSAudio Backend." ON)
wsoption (AUDIOOSSOPT "Enable OSS WSAudio Backend." ON)
wsoption (AUDIOALSAOPT "Enable ALSA WSAudio Backend." ON)
wsoption (X68000OPT "Cross compile using GCC for the Sharp x68000." OFF)
if (${AMIGAGCCOPT})
	set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/amiga-gcc-toolchain.cmake)
elseif (${X68000OPT})
	set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/x68000-gcc-toolchain.cmake)
endif()
#
#
# End compile options
#
#

#
# Project will accept asm, c, and c++ files
#
project ("glcontextc" ASM_MASM C CXX)
project ("glcontextcpp" ASM_MASM C CXX)
project ("displayc" ASM_MASM C CXX)
project ("displaycpp" ASM_MASM C CXX)
project ("difffilec" ASM_MASM C CXX)
project ("difffilecpp" ASM_MASM C CXX)
project ("diffmemc" ASM_MASM C CXX)
project ("diffmemcpp" ASM_MASM C CXX)
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
	project ("guihelloworldc" ASM_MASM C CXX OBJC OBJCXX)
	project ("guihelloworldcpp" ASM_MASM C CXX OBJC OBJCXX)
else ()
	project ("guihelloworldc" ASM_MASM C CXX)
	project ("guihelloworldcpp" ASM_MASM C CXX)
endif ()
project ("joystickc" ASM_MASM C CXX)
project ("joystickcpp" ASM_MASM C CXX)
add_executable(glcontextc "examples/glcontextc.c")
add_executable(glcontextcpp "examples/glcontextcpp.cpp")
add_executable(displayc "examples/displayc.c")
add_executable(displaycpp "examples/displaycpp.cpp")
add_executable(difffilec "examples/difffilec.c")
add_executable(difffilecpp "examples/difffilecpp.cpp")
add_executable(diffmemc "examples/diffmemc.c")
add_executable(diffmemcpp "examples/diffmemcpp.cpp")
add_executable(guihelloworldc "examples/guihelloworldc.c")
add_executable(guihelloworldcpp "examples/guihelloworldcpp.cpp")
add_executable(joystickc "examples/joystickc.c")
add_executable(joystickcpp "examples/joystickcpp.cpp")

#
# Includes, needs to be included after project
#
include(GNUInstallDirs) # Standard UNIX installation directories
include(CheckIncludeFile) # Check for header existance

#
#
# CMAKE doesn't supply a way to get target architecture
# https://github.com/axr/solar-cmake/blob/master/TargetArch.cmake
#
#
set(archdetect_c_code "
#if defined(__arm__) || defined(__TARGET_ARCH_ARM)
	#if defined(__ARM_ARCH_7__) \\
		|| defined(__ARM_ARCH_7A__) \\
		|| defined(__ARM_ARCH_7R__) \\
		|| defined(__ARM_ARCH_7M__) \\
		|| (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 7)
		#error cmake_ARCH armv7
	#elif defined(__ARM_ARCH_6__) \\
		|| defined(__ARM_ARCH_6J__) \\
		|| defined(__ARM_ARCH_6T2__) \\
		|| defined(__ARM_ARCH_6Z__) \\
		|| defined(__ARM_ARCH_6K__) \\
		|| defined(__ARM_ARCH_6ZK__) \\
		|| defined(__ARM_ARCH_6M__) \\
		|| (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 6)
		#error cmake_ARCH armv6
	#elif defined(__ARM_ARCH_5TEJ__) \\
		|| (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 5)
		#error cmake_ARCH armv5
	#else
		#error cmake_ARCH arm
	#endif
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
	#error cmake_ARCH i386
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
	#error cmake_ARCH x86_64
#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
	#error cmake_ARCH ia64
#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) \\
	  || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC)  \\
	  || defined(_M_MPPC) || defined(_M_PPC)
	#if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
		#error cmake_ARCH ppc64
	#else
		#error cmake_ARCH ppc
	#endif
#endif
#error cmake_ARCH unknown
")
# Set ppc_support to TRUE before including this file or ppc and ppc64
# will be treated as invalid architectures since they are no longer supported by Apple
set(ppc_support TRUE)
if(APPLE AND CMAKE_OSX_ARCHITECTURES)
	# On OS X we use CMAKE_OSX_ARCHITECTURES *if* it was set
	# First let's normalize the order of the values

	# Note that it's not possible to compile PowerPC applications if you are using
	# the OS X SDK version 10.6 or later - you'll need 10.4/10.5 for that, so we
	# disable it by default
	# See this page for more information:
	# http://stackoverflow.com/questions/5333490/how-can-we-restore-ppc-ppc64-as-well-as-full-10-4-10-5-sdk-support-to-xcode-4

	# Architecture defaults to i386 or ppc on OS X 10.5 and earlier, depending on the CPU type detected at runtime.
	# On OS X 10.6+ the default is x86_64 if the CPU supports it, i386 otherwise.

	foreach(osx_arch ${CMAKE_OSX_ARCHITECTURES})
		if("${osx_arch}" STREQUAL "ppc" AND ppc_support)
			set(osx_arch_ppc TRUE)
		elseif("${osx_arch}" STREQUAL "i386")
			set(osx_arch_i386 TRUE)
		elseif("${osx_arch}" STREQUAL "x86_64")
			set(osx_arch_x86_64 TRUE)
		elseif("${osx_arch}" STREQUAL "ppc64" AND ppc_support)
			set(osx_arch_ppc64 TRUE)
		else()
			message(FATAL_ERROR "Invalid OS X arch name: ${osx_arch}")
		endif()
	endforeach()

	# Now add all the architectures in our normalized order
	if(osx_arch_ppc)
		list(APPEND ARCH ppc)
	endif()

	if(osx_arch_i386)
		list(APPEND ARCH i386)
	endif()

	if(osx_arch_x86_64)
		list(APPEND ARCH x86_64)
	endif()

	if(osx_arch_ppc64)
		list(APPEND ARCH ppc64)
	endif()
else()
	file(WRITE "${CMAKE_BINARY_DIR}/arch.c" "${archdetect_c_code}")

	enable_language(C)

	# Detect the architecture in a rather creative way...
	# This compiles a small C program which is a series of ifdefs that selects a
	# particular #error preprocessor directive whose message string contains the
	# target architecture. The program will always fail to compile (both because
	# file is not a valid C program, and obviously because of the presence of the
	# #error preprocessor directives... but by exploiting the preprocessor in this
	# way, we can detect the correct target architecture even when cross-compiling,
	# since the program itself never needs to be run (only the compiler/preprocessor)
	try_run(
		run_result_unused
		compile_result_unused
		"${CMAKE_BINARY_DIR}"
		"${CMAKE_BINARY_DIR}/arch.c"
		COMPILE_OUTPUT_VARIABLE ARCH
		CMAKE_FLAGS CMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
	)

	# Parse the architecture name from the compiler output
	string(REGEX MATCH "cmake_ARCH ([a-zA-Z0-9_]+)" ARCH "${ARCH}")

	# Get rid of the value marker leaving just the architecture name
	string(REPLACE "cmake_ARCH " "" ARCH "${ARCH}")

	# If we are compiling with an unknown architecture this variable should
	# already be set to "unknown" but in the case that it's empty (i.e. due
	# to a typo in the code), then set it to unknown
	if (NOT ARCH)
		set(ARCH unknown)
	endif()
endif()
#
#
# End Architecture scanning
#
#

# The built-in functionality for compiler flag support in CMake
# is garbage. clang just outputs a warning, so check for both w/errors
function(check_compiler_flag_withwarnings flag var)
	file(WRITE "${CMAKE_BINARY_DIR}/arch.c" "int main() { return 0; }")
	enable_language(C)
	#file(REMOVE "${CMAKE_BINARY_DIR}/arch.c" "${compiler_output_file}")
	try_run(
		run_result_unused
		compile_result_unused
		"${CMAKE_BINARY_DIR}"
		"${CMAKE_BINARY_DIR}/arch.c"
		COMPILE_OUTPUT_VARIABLE FLAGCHECK
		COMPILE_DEFINITIONS "${flag}"
	)

	if(NOT ${FLAGCHECK} MATCHES "error" AND NOT ${FLAGCHECK} MATCHES "warning")
		#message(STATUS "--------------------FLAG:${flag} SUCCESS: ${FLAGCHECK}----------------")
		set(${var} TRUE PARENT_SCOPE)
	else()
		#message(STATUS "Flag Check Failed: ${FLAGCHECK}")
	endif()
endfunction()

add_definitions(-DWSBUILDINGFROMCMAKE)
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ferror-limit=200")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=200")
endif()
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_definitions(-D_DEBUG)
else()
	if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
		message(STATUS "----Enabling inlining and optimizing for size in GCC----")
		# Enable inlining and optimize for size in GCC
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finline-functions -finline-small-functions -finline-limit=100")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finline-functions -finline-small-functions -finline-limit=100")

		check_compiler_flag_withwarnings("-Oz" SUPPORTS_OZ_FLAG)
		if (SUPPORTS_OZ_FLAG)
			message(STATUS "----Enabling -Oz (GCC is high enough)----")
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Oz")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Oz")
		else()
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
		endif()
	elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
		message(STATUS "----Enabling inlining and optimizing for size in Clang----")
		# CMake won't let you change optimization levels unless you clear them first
		set(CMAKE_C_FLAGS "" CACHE STRING "" FORCE)
		set(CMAKE_CXX_FLAGS "" CACHE STRING "" FORCE)
		set(CMAKE_C_FLAGS_RELWITHDEBINFO "" CACHE STRING "" FORCE)
		set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE STRING "" FORCE)
		if("${CMAKE_BUILD_TYPE}" MATCHES "RelWithDebInfo")
			# CMake doesn't correctly put -g for clang
			set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g")
			set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g")
			set(CMAKE_C_FLAGS "-g")
			set(CMAKE_CXX_FLAGS "-g")
			set(CMAKE_EXE_LINKER_FLAGS "-g")
		endif()
		# Enable inlining and optimize for size in Clang
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mllvm -inline-threshold=100")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -inline-threshold=100")
		check_compiler_flag_withwarnings("-Oz" SUPPORTS_OZ_FLAG)
		if (SUPPORTS_OZ_FLAG)
			message(STATUS "--============--Enabling -Oz (Clang is high enough)--============--")
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Oz")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Oz")
		else()
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
		endif()
	endif()
endif()

#
# print out status
#
if(DEFINED CMAKE_BUILD_TYPE)
	message(STATUS "Building for ${ARCH} architecture on config ${CMAKE_BUILD_TYPE} for ${CMAKE_SYSTEM_NAME}")
else ()
	message(STATUS "Building for ${ARCH} architecture with an empty/release config for ${CMAKE_SYSTEM_NAME}")
endif()
message(STATUS "The host OS is ${CMAKE_HOST_SYSTEM_NAME} and the processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "CMake main directory is [${CMAKE_SOURCE_DIR}]")
message(STATUS "######GENERATOR IS [${CMAKE_GENERATOR}]########")

#
# Older versions of CMake don't have target_link_directories
#
if(CMAKE_VERSION VERSION_LESS 3.13.0)
	macro(target_link_directories _TARGET _SCOPE)
		link_directories(${ARGN})
	endmacro()
endif()

if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
	add_definitions(-D_CONSOLE)
 endif()

#
# Set Linux dependencies
#
if (NOT AMIGAGCCOPT AND NOT X68000OPT AND NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	# Package and header checks
	if (NOT ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
		#add_executable(threadlocktest "tests/rwlocktest.cpp")
		#target_link_libraries(threadlocktest pthread)
		find_package(PkgConfig REQUIRED)
		#Only needed for an old method for WSLaunchURI
		#pkg_check_modules(GCONF2 REQUIRED gconf-2.0)
		CHECK_INCLUDE_FILE("sys/resource.h" HAVE_SYS_RESOURCE_H)
		if (${KDEOPT})
			find_package(KF5I18n REQUIRED)
			find_package(KF5CoreAddons REQUIRED)
			find_package(KF5ConfigWidgets REQUIRED)
			find_package(KF5DBusAddons REQUIRED)
			find_package(KF5GlobalAccel REQUIRED)
			find_package(KF5IconThemes REQUIRED)
			find_package(KF5Service REQUIRED)
			find_package(KF5Notifications REQUIRED)
			find_package(KF5TextWidgets REQUIRED)
			find_package(KF5WindowSystem REQUIRED)
			find_package(KF5WidgetsAddons REQUIRED)
			find_package(KF5XmlGui REQUIRED)
			find_package(Qt5 COMPONENTS Widgets REQUIRED)
			target_link_libraries(displayc
				KF5::I18n
				KF5::ConfigWidgets
				KF5::CoreAddons
				KF5::Service
				KF5::TextWidgets
				KF5::Notifications
				KF5::WidgetsAddons
				KF5::XmlGui
				Qt5::Widgets
				dl pthread)
			target_compile_options(displayc PUBLIC
			-DGLIB_DISABLE_DEPRECATION_WARNINGS -DGTK_DISABLE_DEPRECATION_WARNINGS -DGDK_DISABLE_DEPRECATION_WARNINGS -ggdb)
			target_link_libraries(displaycpp
				KF5::I18n
				KF5::ConfigWidgets
				KF5::CoreAddons
				KF5::Service
				KF5::TextWidgets
				KF5::Notifications
				KF5::WidgetsAddons
				KF5::XmlGui
				Qt5::Widgets
				dl pthread)
			target_compile_options(displaycpp PUBLIC
			-DGLIB_DISABLE_DEPRECATION_WARNINGS -DGTK_DISABLE_DEPRECATION_WARNINGS -DGDK_DISABLE_DEPRECATION_WARNINGS -ggdb)
		else () # GTK
			if (${GTK4OPT})
				pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk4)
			else ()
				pkg_check_modules(APPIND ayatana-appindicator3-0.1)
				if(NOT APPIND_FOUND)
					pkg_check_modules (APPIND appindicator3-0.1)
				else ()
					add_definitions(-DWSHASAYATANAAPPINDICATOR)
					set(HASAYATANA 1)
				endif()
				if(NOT APPIND_FOUND)
					pkg_check_modules (APPIND appindicator-0.1)
				endif()
				if(APPIND_FOUND)
					add_definitions(-DWSHASAPPINDICATOR)
				endif()
				if(NOT HASAYATANA)
					pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk+-3.0)
				else()
					#set(GTK "") # set by ayatana's pkg-config
				endif()
			endif()
			pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-egl) # GLCONTEXT
			pkg_check_modules(X11 REQUIRED x11) # GLCONTEXT
			pkg_check_modules(GLX REQUIRED glx) # GLCONTEXT
			pkg_check_modules(EGL REQUIRED egl) # GLCONTEXT
			pkg_check_modules(GL REQUIRED gl) # GLCONTEXT
			pkg_check_modules(GLU REQUIRED glu) # GLCONTEXT
			pkg_check_modules(WAYLANDSERVER REQUIRED wayland-server) # GLCONTEXT
			pkg_check_modules(XF86VM REQUIRED xxf86vm) # WSDISPLAY
			pkg_check_modules(XINERAMA REQUIRED xinerama) # WSDISPLAY
			pkg_check_modules(XRANDR REQUIRED xrandr) # WSDISPLAY
			set(EXTRAINCLUDES ${GTK_INCLUDE_DIRS} ${GCONF2_INCLUDE_DIRS} ${APPIND_INCLUDE_DIRS} ${WAYLAND_INCLUDE_DIRS} ${X11_INCLUDE_DIRS} ${GLX_INCLUDE_DIRS} ${EGL_INCLUDE_DIRS} ${GL_INCLUDE_DIRS} ${GLU_INCLUDE_DIRS} ${WAYLANDSERVER_INCLUDE_DIRS} ${XF86VM_INCLUDE_DIRS} ${XINERAMA_INCLUDE_DIRS} ${XRANDR_INCLUDE_DIRS})
			set(EXTRALIBS ${GTK_LIBRARIES} ${GCONF2_LIBRARIES} ${APPIND_LIBRARIES} ${WAYLAND_LIBRARIES} ${X11_LIBRARIES} ${GLX_LIBRARIES} ${EGL_LIBRARIES} ${GL_LIBRARIES} ${GLU_LIBRARIES} ${WAYLANDSERVER_LIBRARIES} ${XF86VM_LIBRARIES} ${XINERAMA_LIBRARIES} ${XRANDR_LIBRARIES} dl pthread)
			set(EXTRACFLAGS ${GTK_CFLAGS_OTHER} ${GCONF2_CFLAGS_OTHER} ${APPIND_CFLAGS_OTHER}
			-DGLIB_DISABLE_DEPRECATION_WARNINGS -DGTK_DISABLE_DEPRECATION_WARNINGS -DGDK_DISABLE_DEPRECATION_WARNINGS -ggdb)
			target_include_directories(glcontextc PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(glcontextc ${EXTRALIBS})
			target_compile_options(glcontextc PUBLIC ${EXTRACFLAGS})
			target_include_directories(glcontextcpp PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(glcontextcpp ${EXTRALIBS})
			target_compile_options(glcontextcpp PUBLIC ${EXTRACFLAGS})

			target_include_directories(displayc PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(displayc ${EXTRALIBS})
			target_compile_options(displayc PUBLIC ${EXTRACFLAGS})
			target_include_directories(displaycpp PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(displaycpp ${EXTRALIBS})
			target_compile_options(displaycpp PUBLIC ${EXTRACFLAGS})

			target_include_directories(difffilec PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(difffilec ${EXTRALIBS})
			target_compile_options(difffilec PUBLIC ${EXTRACFLAGS})
			target_include_directories(difffilecpp PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(difffilecpp ${EXTRALIBS})
			target_compile_options(difffilecpp PUBLIC ${EXTRACFLAGS})

			target_include_directories(diffmemc PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(diffmemc ${EXTRALIBS})
			target_compile_options(diffmemc PUBLIC ${EXTRACFLAGS})
			target_include_directories(diffmemcpp PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(diffmemcpp ${EXTRALIBS})
			target_compile_options(diffmemcpp PUBLIC ${EXTRACFLAGS})

			target_include_directories(guihelloworldc PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(guihelloworldc ${EXTRALIBS})
			target_compile_options(guihelloworldc PUBLIC ${EXTRACFLAGS})
			target_include_directories(guihelloworldcpp PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(guihelloworldcpp ${EXTRALIBS})
			target_compile_options(guihelloworldcpp PUBLIC ${EXTRACFLAGS})

			target_include_directories(joystickc PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(joystickc ${EXTRALIBS})
			target_compile_options(joystickc PUBLIC ${EXTRACFLAGS})
			target_include_directories(joystickcpp PUBLIC ${EXTRAINCLUDES})
			target_link_libraries(joystickcpp ${EXTRALIBS})
			target_compile_options(joystickcpp PUBLIC ${EXTRACFLAGS})
		endif() # End GTK
		pkg_check_modules(LIBPCRE libpcre)
		if(LIBPCRE_FOUND)
			add_definitions(-DWSHASPCRE)
		endif()
		# Link library paths are not setup by default with CMake on FreeBSD
		if (NOT ${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
			target_link_libraries(glcontextc "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(glcontextcpp "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(displayc "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(displaycpp "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(difffilec "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(difffilecpp "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(diffmemc "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(diffmemcpp "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(guihelloworldc "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(guihelloworldcpp "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(joystickc "-L/usr/local/lib -R/usr/local/lib")
			target_link_libraries(joystickcpp "-L/usr/local/lib -R/usr/local/lib")
			if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "FreeBSD")
				target_link_libraries(joystickc "-lusbhid")
				target_link_libraries(joystickcpp "-lusbhid")
			endif()
		endif()
		if (${AUDIOALSAOPT})
			pkg_check_modules(ALSA REQUIRED alsa)
			CHECK_INCLUDE_FILE("alsa/asoundlib.h" HAVE_ALSA_ASOUNDLIB_H)
			if (${HAVE_ALSA_ASOUNDLIB_H})
			endif()
		endif()
		if (${AUDIOOSSOPT})
		endif()
		if (${HAVE_SYS_RESOURCE_H})
		endif()
	else () # OSX
		find_library(OPENGL_LIBRARY OpenGL)
		find_library(AGL_LIBRARY AGL)
		find_library(IOKIT_LIBRARY IOKit)
		find_library(COREGRAPHICS_LIBRARY CoreGraphics)
		find_library(COREFOUNDATION_LIBRARY CoreFoundation)
		find_library(COCOA_LIBRARY Cocoa)
		if(OPENGL_LIBRARY)
			target_link_libraries(glcontextc ${AGL_LIBRARY} ${OPENGL_LIBRARY})
			target_link_libraries(glcontextcpp ${AGL_LIBRARY} ${OPENGL_LIBRARY})
			target_link_libraries(displayc ${IOKIT_LIBRARY} ${COREGRAPHICS_LIBRARY} ${COREFOUNDATION_LIBRARY})
			target_link_libraries(displaycpp ${IOKIT_LIBRARY} ${COREGRAPHICS_LIBRARY} ${COREFOUNDATION_LIBRARY})
			target_link_libraries(guihelloworldc ${COCOA_LIBRARY} ${COREGRAPHICS_LIBRARY} ${COREFOUNDATION_LIBRARY})
			target_link_libraries(guihelloworldcpp ${COCOA_LIBRARY} ${COREGRAPHICS_LIBRARY} ${COREFOUNDATION_LIBRARY})
			target_link_libraries(joystickc ${IOKIT_LIBRARY} ${COREGRAPHICS_LIBRARY} ${COREFOUNDATION_LIBRARY})
			target_link_libraries(joystickcpp ${IOKIT_LIBRARY} ${COREGRAPHICS_LIBRARY} ${COREFOUNDATION_LIBRARY})

		else()
			message(STATUS "OpenGL library on OSX not found")
		endif()
	endif()
	# GCC specific flags
	if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
		set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -w")
		set(CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
	endif()
endif()
#
# End platform-specific stuff
#
#

#
# Set output directories
#
set_target_properties(glcontextc PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(glcontextcpp PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(displayc PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(displaycpp PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
	set_target_properties(difffilec PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(difffilecpp PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
	set_target_properties(diffmemc PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(diffmemcpp PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(guihelloworldc PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(guihelloworldcpp PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(joystickc PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_target_properties(joystickcpp PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

#
# Global include directories
#
set (EXTRAINCLUDES
		${CMAKE_SOURCE_DIR}
)
target_include_directories(glcontextc PRIVATE ${EXTRAINCLUDES})
target_include_directories(glcontextcpp PRIVATE ${EXTRAINCLUDES})
target_include_directories(displayc PRIVATE ${EXTRAINCLUDES})
target_include_directories(displaycpp PRIVATE ${EXTRAINCLUDES})
target_include_directories(difffilec PRIVATE ${EXTRAINCLUDES})
target_include_directories(difffilecpp PRIVATE ${EXTRAINCLUDES})
target_include_directories(diffmemc PRIVATE ${EXTRAINCLUDES})
target_include_directories(diffmemcpp PRIVATE ${EXTRAINCLUDES})
target_include_directories(guihelloworldc PRIVATE ${EXTRAINCLUDES})
target_include_directories(guihelloworldcpp PRIVATE ${EXTRAINCLUDES})
target_include_directories(joystickc PRIVATE ${EXTRAINCLUDES})
target_include_directories(joystickcpp PRIVATE ${EXTRAINCLUDES})

#
# Compile GUI example as objective C/C++ on OS X
#
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
	set_source_files_properties("examples/guihelloworldc.c"
		PROPERTIES LANGUAGE OBJC)
	set_source_files_properties("examples/guihelloworldcpp.cpp"
		PROPERTIES LANGUAGE OBJCXX)
endif()

set(CMAKE_SUPPRESS_REGENERATION ON)
