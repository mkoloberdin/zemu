#include "z80ex_common.h"
#include "lib_z80_dasm/dasm.h"

/* callback that returns byte for a given adress */
typedef Z80EX_BYTE (* z80ex_dasm_readbyte_cb)(Z80EX_WORD addr, void* user_data);

/* flags */
enum Z80EX_DASM_FLAGS {
    WORDS_DEC = 1,
    BYTES_DEC = 2
};

#ifndef Z80EX_SELF_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

extern int z80ex_dasm(
    char* output,
    int output_size,
    unsigned flags,
    int* t_states,
    int* t_states2,
    z80ex_dasm_readbyte_cb readbyte_cb,
    Z80EX_WORD addr,
    void* user_data
);

#ifdef __cplusplus
}
#endif

#endif /* Z80EX_SELF_INCLUDE */
