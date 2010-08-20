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

file (GLOB SOURCES
    src/*.cpp
    src/lib_ay/*.cpp
    src/lib_ym2203/*.cpp
    src/lib_wd1793/*.cpp
    src/devices/*/*.cpp
    src/renderer/*.cpp
    src/tape/*.cpp
    src/sound/*.cpp
)
add_executable (zemu WIN32 ${SOURCES})

target_link_libraries(zemu z80ex-static z80ex_dasm-static ${SDL_LIBRARY})
if (WIN32)
    target_link_libraries(zemu winmm shlwapi)
endif ()

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

################## from src/Makefile ##########################
#CXX = g++
#CXXFLAGS += ${shell sdl-config --cflags} -O2 -fomit-frame-pointer -Wall -pipe -I${Z80EX_PATH}/include
#LDFLAGS += -L${Z80EX_PATH}/lib
#LIBS = ${shell sdl-config --libs} -lz80ex -lz80ex_dasm

#ifeq ("${OSTYPE}","msys")
#	ADDITIONAL_PARAMS = -DPLATFORM_WIN=1 -Dmain=SDL_main -mwindows
#	LIBS += -lwinmm -lmingw32
#	EXT = .exe
#endif

# ------------------------------------------------------------

#ifndef PREFIX
#	PREFIX = /opt/zemu
#endif

#BIN_PATH = ${PREFIX}/bin

#ifeq ("${PREFIX}","/opt/zemu")
#	SHARE_PATH = ${PREFIX}/share
#else
#	SHARE_PATH = ${PREFIX}/share/zemu
#endif

#ROMS_PATH = ${SHARE_PATH}/roms
#CONFIG_PATH = ${SHARE_PATH}
#ARC_PATH = ${SHARE_PATH}/arc
#BOOT_PATH = ${SHARE_PATH}/boot

# ------------------------------------------------------------

#ifdef FOR_INSTALL
#	ADDITIONAL_PARAMS += -DSHARE_PATH=\"${SHARE_PATH}\" -DFOR_INSTALL=\"${FOR_INSTALL}\"
#endif

# ------------------------------------------------------------

#SRCS =		$(wildcard *.cpp) \
#			$(wildcard lib_ay/*.cpp) \
#			$(wildcard lib_ym2203/*.cpp) \
#			$(wildcard lib_wd1793/*.cpp) \
#			$(wildcard devices/*/*.cpp) \
#			$(wildcard renderer/*.cpp) \
#			$(wildcard tape/*.cpp) \
#			$(wildcard sound/*.cpp)

#HEADERS =	$(wildcard *.h) \
#			$(wildcard lib_ay/*.h) \
#			$(wildcard lib_ym2203/*.h) \
#			$(wildcard lib_wd1793/*.h) \
#			$(wildcard devices/*/*.h) \
#			$(wildcard renderer/*.h) \
#			$(wildcard tape/*.h) \
#			$(wildcard sound/*.h)

#OBJS = $(SRCS:.cpp=.o)

#zemu: $(OBJS)
#	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
#	strip --strip-all $@$(EXT)

#$(OBJS): $(HEADERS)

#%.o: %.cpp
#	$(CXX) $(CXXFLAGS) $(ADDITIONAL_PARAMS) -c $< -o $*.o

#.PHONY: clean install

#clean:
#	rm -f $(OBJS)

#install: zemu
#	install -d ${BIN_PATH}
#	install -d ${ROMS_PATH}
#	install -d ${CONFIG_PATH}
#	install -d ${ARC_PATH}
#	install -d ${BOOT_PATH}

#	install -s ./zemu ${BIN_PATH}
#	install ./roms/* ${ROMS_PATH}
#	install ./config-install.xml ${CONFIG_PATH}
#	install ./keys.config ${CONFIG_PATH}
#	install ./arc/* ${ARC_PATH}
#	install ./boot/* ${BOOT_PATH}

#	mv ${CONFIG_PATH}/config-install.xml ${CONFIG_PATH}/config.xml

