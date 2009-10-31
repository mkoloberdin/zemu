ifdef PREFIX
	INSTALL_PARAMS = PREFIX=${PREFIX}
endif

ifdef BIG_ENDIAN
	Z80EX_PARAMS = ENDIANNESS=WORDS_LITTLE_ENDIAN
endif

ifndef Z80EX_PATH
	Z80EX_PATH = `pwd`/../z80ex
endif

.PHONY: zemu clean install

zemu:
	make -C ${Z80EX_PATH} static ${Z80EX_PARAMS}
	# cp -f ${Z80EX_PATH}/lib/libz80ex.a zemu/lib_z80ex/libz80ex.a
	# cp -f ${Z80EX_PATH}/lib/libz80ex_dasm.a zemu/lib_z80ex/libz80ex_dasm.a
	make -C src Z80EX_PATH=${Z80EX_PATH}

clean:
	make -C ${Z80EX_PATH} clean
	make -C src clean

install:
	make -C ${Z80EX_PATH} static ${Z80EX_PARAMS}
	# cp -f ${Z80EX_PATH}/lib/libz80ex.a zemu/lib_z80ex/libz80ex.a
	# cp -f ${Z80EX_PATH}/lib/libz80ex_dasm.a zemu/lib_z80ex/libz80ex_dasm.a
	make -C src clean
	make -C src FOR_INSTALL=1 ${INSTALL_PARAMS}
	make -C src install ${INSTALL_PARAMS}
