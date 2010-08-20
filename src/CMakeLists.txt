file (GLOB SOURCES
    *.cpp
    lib_ay/*.cpp
    lib_ym2203/*.cpp
    lib_wd1793/*.cpp
    devices/*/*.cpp
    renderer/*.cpp
    tape/*.cpp
    sound/*.cpp
)
add_executable (zemu WIN32 ${SOURCES})

target_link_libraries(zemu z80ex-static z80ex_dasm-static ${SDL_LIBRARY})
if (WIN32)
    target_link_libraries(zemu winmm shlwapi)
endif ()

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
