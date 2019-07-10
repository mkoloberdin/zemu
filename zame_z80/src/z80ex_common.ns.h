/* For detection that it is not genuine z80ex */
#define Z80EX_ZAME_WRAPPER

#ifdef _MSC_VER
    #define LIB_EXPORT __declspec(dllexport)

    #ifndef __cplusplus
        typedef int bool;
        #define true 1
        #define false 0
    #endif
#else
    #define LIB_EXPORT
    #include <stdbool.h>
#endif

#if defined(__SYMBIAN32__)
    typedef unsigned char Z80EX_BYTE;
    typedef signed char Z80EX_SIGNED_BYTE;
    typedef unsigned short Z80EX_WORD;
    typedef unsigned int Z80EX_DWORD;
#elif defined(__GNUC__)
    #include <stdint.h>
    typedef uint8_t Z80EX_BYTE;
    typedef int8_t Z80EX_SIGNED_BYTE;
    typedef uint16_t Z80EX_WORD;
    typedef uint32_t Z80EX_DWORD;
#elif defined(_MSC_VER)
    typedef unsigned __int8 Z80EX_BYTE;
    typedef signed __int8 Z80EX_SIGNED_BYTE;
    typedef unsigned __int16 Z80EX_WORD;
    typedef unsigned __int32 Z80EX_DWORD;
#else
    typedef unsigned char Z80EX_BYTE;
    typedef signed char Z80EX_SIGNED_BYTE;
    typedef unsigned short Z80EX_WORD;
    typedef unsigned int Z80EX_DWORD;
#endif
