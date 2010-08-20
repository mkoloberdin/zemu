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

if (NOT DEFINED Z80EX_PATH)
	set (Z80EX_PATH "${PROJECT_SOURCE_DIR}/../z80ex"
        CACHE PATH "Path to Z80Ex library")
endif ()

include_directories("${Z80EX_PATH}/include")
set (Z80EX_BINARY_DIR "${PROJECT_BINARY_DIR}/lib_z80ex")
make_directory (${Z80EX_BINARY_DIR})
add_subdirectory (${Z80EX_PATH} ${Z80EX_BINARY_DIR})
link_directories(${z80EX_BINARY_DIR})
#add_custom_target (z80ex "cmake --build ${Z80EX_BINARY_DIR} --target z80ex-static ${Z80EX_PATH}")

if (WIN32)
    set (ENV{SDLDIR} "${PROJECT_BINARY_DIR}/SDL")
    message ("ENV{SDLDIR}=$ENV{SDLDIR}")
endif ()
#message ("ENV{SDLDIR}=$ENV{SDLDIR}")
find_package (SDL REQUIRED)
if (SDL_FOUND)
    message ("SDL found")
    include_directories (${SDL_INCLUDE_DIR})
else ()
    message (FATAL_ERROR "SDL not found")
endif (SDL_FOUND)

add_subdirectory (src)

#.PHONY: zemu clean install

#zemu:
#	make -C ${Z80EX_PATH} static ${Z80EX_PARAMS}
	# cp -f ${Z80EX_PATH}/lib/libz80ex.a zemu/lib_z80ex/libz80ex.a
	# cp -f ${Z80EX_PATH}/lib/libz80ex_dasm.a zemu/lib_z80ex/libz80ex_dasm.a
#	make -C src Z80EX_PATH=${Z80EX_PATH}

#clean:
#	make -C ${Z80EX_PATH} clean
#	make -C src clean

#install:
#	make -C ${Z80EX_PATH} static ${Z80EX_PARAMS}
	# cp -f ${Z80EX_PATH}/lib/libz80ex.a zemu/lib_z80ex/libz80ex.a
	# cp -f ${Z80EX_PATH}/lib/libz80ex_dasm.a zemu/lib_z80ex/libz80ex_dasm.a
#	make -C src clean
#	make -C src FOR_INSTALL=1 ${INSTALL_PARAMS}
#	make -C src install ${INSTALL_PARAMS}
