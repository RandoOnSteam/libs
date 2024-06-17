# amiga-gcc-toolchain.cmake

# Specify the cross-compiler
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR m68k)

# Path to the cross-compiler
set(CMAKE_C_COMPILER x68000-gcc)
set(CMAKE_CXX_COMPILER x68000-gcc)

# You might need to specify additional flags or settings depending on your setup
# Disable unsupported features/flags
set(CMAKE_C_COMPILER_TARGET "")
set(CMAKE_CXX_COMPILER_TARGET "")
set(CMAKE_C_FLAGS "" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "" CACHE STRING "" FORCE)