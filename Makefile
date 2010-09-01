ifdef PREFIX
	INSTALL_PARAMS = PREFIX=${PREFIX}
endif

ifdef BIG_ENDIAN
	Z80EX_PARAMS = ENDIANNESS=WORDS_LITTLE_ENDIAN
endif

ifndef Z80EX_PATH
	Z80EX_PATH = `pwd`/../z80ex
endif

OS = ${shell uname -s}

ifeq (${OS},FreeBSD)
	CALLMAKE = gmake
else
	CALLMAKE = make
endif

.PHONY: zemu clean install

zemu:
	${CALLMAKE} -C ${Z80EX_PATH} static ${Z80EX_PARAMS}
	# cp -f ${Z80EX_PATH}/lib/libz80ex.a zemu/lib_z80ex/libz80ex.a
	# cp -f ${Z80EX_PATH}/lib/libz80ex_dasm.a zemu/lib_z80ex/libz80ex_dasm.a
	${CALLMAKE} -C src Z80EX_PATH=${Z80EX_PATH}

clean:
	${CALLMAKE} -C ${Z80EX_PATH} clean
	${CALLMAKE} -C src clean

install:
	${CALLMAKE} -C ${Z80EX_PATH} static ${Z80EX_PARAMS}
	# cp -f ${Z80EX_PATH}/lib/libz80ex.a zemu/lib_z80ex/libz80ex.a
	# cp -f ${Z80EX_PATH}/lib/libz80ex_dasm.a zemu/lib_z80ex/libz80ex_dasm.a
	${CALLMAKE} -C src clean
	${CALLMAKE} -C src FOR_INSTALL=1 ${INSTALL_PARAMS}
	${CALLMAKE} -C src install ${INSTALL_PARAMS}
