#
# Amiga SDL Test
#
CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(test C)

# Set on if you need to see build output
SET(CMAKE_VERBOSE_MAKEFILE OFF)
#SET(CMAKE_DEBUG_TARGET_PROPERTIES INCLUDE_DIRECTORIES)
#SET(CMAKE_DEBUG_TARGET_PROPERTIES LINK_DIRECTORIES)

############## Cross Compile Setup ##############
SET(AMIGA_ROOT /opt/amiga)
SET(CMAKE_SYSTEM_NAME generic)
SET(AMIGA_OS "${AMIGA_ROOT}/m68k-amigaos")
SET(GCCM68K "${AMIGA_ROOT}/bin")
SET(CMAKE_C_COMPILER ${GCCM68K}/m68k-amigaos-gcc)
SET(CMAKE_C_COMPILER_AR "${GCCM68K}/m68k-amigaos-ar rcu")
SET(CMAKE_C_COMPILER_RANLIB ${GCCM68K}/m68k-amigaos-ranlib)
#SET(CMAKE_CXX_COMPILER ${gccm68k}/m68k-amigaos-g++)
SET(CMAKE_C_FLAGS "-DNDEBUG -Wfatal-errors -noixemul -fomit-frame-pointer -m68020-60 -mhard-float -std=c99 -L$AMIGA_OS/lib")
SET(CMAKE_EXE_LINKER_FLAGS "-noixemul")
SET(CMAKE_C_STANDARD 99)
SET(LIBM_LIBRARIES m)

INCLUDE_DIRECTORIES(
	"${AMIGA_OS}/include/**"
)

############## General Setup ##############
if(NOT DEFINED TARGET)
	set(TARGET ${PROJECT_NAME})
endif()
SET(SEP "/")
ADD_DEFINITIONS( -lm )

FILE(GLOB_RECURSE SOURCES
	"src/*.h"
	"src/*.c"
)

SET(SOURCES ${SOURCES})

if(DEBUG)
	#add_definitions(-ggdb3 -O0)
	ADD_DEFINITIONS(-O0)
else()
	ADD_DEFINITIONS(-O3)
endif()

INCLUDE_DIRECTORIES(
	src/**
)

LINK_DIRECTORIES(
	${AMIGA_OS}/lib
)

ADD_EXECUTABLE(${TARGET} ${SOURCES})

TARGET_LINK_LIBRARIES(${TARGET}
	# Novacoder SDL
	#SDL_RTG_040
	# Amigaports SDL
	SDL
	m
	libdebug.a
)

# This will ensure the output is put in the right place to run regardless
# of where you build from. This will include clean. Be careful that different
# build types will all write to the same place so the executable will overwrite
set_target_properties(${TARGET} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${PROJECT_SOURCE_DIR}
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}
)
