CXX = g++
CXXFLAGS += ${shell sdl-config --cflags} -O2 -fomit-frame-pointer -Wall -g -pipe
LDFLAGS += -Llib_z80ex
LIBS = ${shell sdl-config --libs} -lz80ex -lz80ex_dasm

ifeq ("${OSTYPE}","msys")
	ADDITIONAL_PARAMS = -DPLATFORM_WIN=1 -Dmain=SDL_main -mwindows
	LIBS += -lwinmm -lmingw32
	EXT = .exe
endif

# ------------------------------------------------------------

ifndef PREFIX
	PREFIX = /opt/zemu
endif

BIN_PATH = ${PREFIX}/bin

ifeq ("${PREFIX}","/opt/zemu")
	SHARE_PATH = ${PREFIX}/share
else
	SHARE_PATH = ${PREFIX}/share/zemu
endif

ROMS_PATH = ${SHARE_PATH}/roms
CONFIG_PATH = ${SHARE_PATH}/config
ARC_PATH = ${SHARE_PATH}/arc
BOOT_PATH = ${SHARE_PATH}/boot

# ------------------------------------------------------------

ifdef FOR_INSTALL
	ADDITIONAL_PARAMS += -DCONFIG_PATH=\"${CONFIG_PATH}\"
else
	ADDITIONAL_PARAMS += -DCONFIG_PATH=\".\"
endif

# ------------------------------------------------------------

SRCS =		$(wildcard *.cpp) \
			$(wildcard lib_ay/*.cpp) \
			$(wildcard lib_ym2203/*.cpp) \
			$(wildcard lib_wd1793/*.cpp) \
			$(wildcard devices/*/*.cpp) \
			$(wildcard renderer/*.cpp) \
			$(wildcard tape/*.cpp) \
			$(wildcard sound/*.cpp)

HEADERS =	$(wildcard *.h) \
			$(wildcard lib_ay/*.h) \
			$(wildcard lib_ym2203/*.h) \
			$(wildcard lib_wd1793/*.h) \
			$(wildcard devices/*/*.h) \
			$(wildcard renderer/*.h) \
			$(wildcard tape/*.h) \
			$(wildcard sound/*.h)

OBJS = $(SRCS:.cpp=.o)

zemu: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	strip --strip-all $@$(EXT)

$(OBJS): $(HEADERS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(ADDITIONAL_PARAMS) -c $< -o $*.o

.PHONY: clean install

clean:
	rm -f $(OBJS)

install: zemu
	install -d ${BIN_PATH}
	install -d ${ROMS_PATH}
	install -d ${CONFIG_PATH}
	install -d ${ARC_PATH}
	install -d ${BOOT_PATH}

	install -s ./zemu ${BIN_PATH}
	install ./roms/* ${ROMS_PATH}
	install ./config-install.xml ${CONFIG_PATH}
	install ./keys.config ${CONFIG_PATH}
	install ./arc/* ${ARC_PATH}
	install ./boot/* ${BOOT_PATH}

	mv ${CONFIG_PATH}/config-install.xml ${CONFIG_PATH}/config.xml
