cmake_minimum_required (VERSION 2.8)
project (ZEmu)

set (ZEMU_VERSION_MAJOR 0)
set (ZEMU_VERSION_MINOR 99944)

include (TestBigEndian)
test_big_endian(BIG_ENDIAN)

configure_file (
	"${PROJECT_SOURCE_DIR}/ZEmuConfig.h.in"
	"${PROJECT_BINARY_DIR}/ZEmuConfig.h"
)

option (USE_Z80EX "Use external Z80EX library for z80 emulation" Off)
set (ZAME_Z80_PATH "${PROJECT_SOURCE_DIR}/zame_z80"
	CACHE PATH "Path to zame z80 library")
set (Z80EX_PATH "${PROJECT_SOURCE_DIR}/../z80ex"
	CACHE PATH "Path to Z80Ex library")

if (USE_Z80EX)
	include_directories("${Z80EX_PATH}/include")
	set (Z80EX_BINARY_DIR "${PROJECT_BINARY_DIR}/lib_z80ex")
	make_directory (${Z80EX_BINARY_DIR})
	set (Z80EX_STATIC_ONLY true)
	add_subdirectory (${Z80EX_PATH} ${Z80EX_BINARY_DIR})
	link_directories (${Z80EX_BINARY_DIR})
else ()
	include_directories("${ZAME_Z80_PATH}/processed")
	add_subdirectory (${ZAME_Z80_PATH} ${Z80EX_BINARY_DIR})
	link_directories (${ZAME_Z80_BINARY_DIR})
endif ()

if (WIN32 AND NOT CMAKE_COMPILER_IS_GNUCC)
	if (NOT ENV{SDLDIR})
		set (ENV{SDLDIR} "${PROJECT_BINARY_DIR}/SDL")
		message ("ENV{SDLDIR}=$ENV{SDLDIR}")
	endif ()
endif ()

# message ("ENV{SDLDIR}=$ENV{SDLDIR}")

find_package (SDL REQUIRED)
if (SDL_FOUND)
	message ("SDL found")
	include_directories (${SDL_INCLUDE_DIR})
else ()
	message (FATAL_ERROR "SDL not found")
endif (SDL_FOUND)

include_directories("${PROJECT_SOURCE_DIR}/src")

file (GLOB_RECURSE SOURCES
	src/*.cpp
	src/*.h
)
add_executable (zemu WIN32 ${SOURCES})

if (USE_Z80EX)
	target_link_libraries (zemu z80ex-static z80ex_dasm-static)
else ()
	target_link_libraries (zemu z80ex_wrapper)
endif ()

target_link_libraries (zemu ${SDL_LIBRARY})

if (WIN32)
	target_link_libraries (zemu winmm shlwapi)
endif ()

# Visual Studio file grouping
source_group (zemu src/*)
source_group (devices src/devices/*)
source_group (devices/border src/devices/border/*)
source_group (devices/covox src/devices/covox/*)
source_group (devices/extport src/devices/extport/*)
source_group (devices/joystick src/devices/joystick/*)
source_group (devices/keyboard src/devices/keyboard/*)
source_group (devices/mmanager src/devices/mmanager/*)
source_group (devices/mouse src/devices/mouse/*)
source_group (devices/trdos src/devices/trdos/*)
source_group (devices/tsfm src/devices/tsfm/*)
source_group (images src/images/*)
source_group (lib_ay src/lib_ay/*)
source_group (lib_wd1793 src/lib_wd1793/*)
source_group (lib_ym2203 src/lib_ym2203/*)
source_group (renderer src/renderer/*)
source_group (sound src/sound/*)
source_group (tape src/tape/*)
