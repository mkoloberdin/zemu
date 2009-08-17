ifdef PREFIX
	INSTALL_PARAMS = PREFIX=${PREFIX}
endif

ifdef BIG_ENDIAN
	Z80EX_PARAMS = ENDIANNESS=WORDS_LITTLE_ENDIAN
endif

.PHONY: zemu clean install

zemu:
	make -C lib_z80ex static ${Z80EX_PARAMS}
	cp -f lib_z80ex/lib/libz80ex.a zemu/lib_z80ex/libz80ex.a
	cp -f lib_z80ex/lib/libz80ex_dasm.a zemu/lib_z80ex/libz80ex_dasm.a
	make -C zemu

clean:
	make -C lib_z80ex clean
	make -C zemu clean

install:
	make -C lib_z80ex static ${Z80EX_PARAMS}
	cp -f lib_z80ex/lib/libz80ex.a zemu/lib_z80ex/libz80ex.a
	cp -f lib_z80ex/lib/libz80ex_dasm.a zemu/lib_z80ex/libz80ex_dasm.a
	make -C zemu clean
	make -C zemu FOR_INSTALL=1 ${INSTALL_PARAMS}
	make -C zemu install ${INSTALL_PARAMS}
