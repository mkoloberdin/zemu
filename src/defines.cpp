// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "defines.h"
#include <string.h>

char hex[17] = "0123456789ABCDEF";
int addLogFirst = 1;

double sqq(double n) {
    return (n * n);
}

int sgn(int a) {
    return ((a == 0) ? 0 : ((a > 0) ? 1 : -1));
}

int unhex(char c) {
    switch (c) {
        case '0':
            return 0;

        case '1':
            return 1;

        case '2':
            return 2;

        case '3':
            return 3;

        case '4':
            return 4;

        case '5':
            return 5;

        case '6':
            return 6;

        case '7':
            return 7;

        case '8':
            return 8;

        case '9':
            return 9;

        case 'a': // fallthrough
        case 'A':
            return 10;

        case 'b': // fallthrough
        case 'B':
            return 11;

        case 'c': // fallthrough
        case 'C':
            return 12;

        case 'd': // fallthrough
        case 'D':
            return 13;

        case 'e': // fallthrough
        case 'E':
            return 14;

        case 'f': // fallthrough
        case 'F':
            return 15;

        default:
            return 0;
    }
}

bool ishex(char c) {
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

void AddLog(const char* fmt, ...) {
    va_list argptr;
    char buf[0x800];
    FILE* fp;

    va_start(argptr, fmt);
    vsprintf(buf, fmt, argptr);
    va_end(argptr);

    if (addLogFirst) {
        fp = fopen("debuglog.txt", "wb");
        addLogFirst = 0;
    } else {
        fp = fopen("debuglog.txt", "ab");
    }

    if (fp != nullptr) {
        fprintf(fp, "%s\n", buf);
        fclose(fp);
    }
}

void AddLogN(const char* fmt, ...) {
    va_list argptr;
    char buf[0x800];
    FILE* fp;

    va_start(argptr, fmt);
    vsprintf(buf, fmt, argptr);
    va_end(argptr);

    if (addLogFirst) {
        fp = fopen("debuglog.txt", "wb");
        addLogFirst = 0;
    } else {
        fp = fopen("debuglog.txt", "ab");
    }

    if (fp != nullptr) {
        fprintf(fp, "%s", buf);
        fclose(fp);
    }
}

[[noreturn]] void StrikeError(const char* fmt, ...) {
    va_list argptr;
    char buf[0x800];

    va_start(argptr, fmt);
    vsprintf(buf, fmt, argptr);
    va_end(argptr);

    #ifdef _WIN32
        fprintf(stderr, "[Error] %s\n", buf);
        MessageBox(HWND_DESKTOP, buf, "Error", 0);
        exit(1);
    #else
        fprintf(stderr, "[Error] %s\n", buf);
        exit(1);
    #endif
}

void StrikeMessage(const char* fmt, ...) {
    va_list argptr;
    char buf[0x800];

    va_start(argptr, fmt);
    vsprintf(buf, fmt, argptr);
    va_end(argptr);

    #ifdef _WIN32
        MessageBox(HWND_DESKTOP, buf, "Message", 0);
    #else
        printf("Message: %s\n", buf);
    #endif
}

char* AllocNstrcpy(const char* str) {
    char* s = new char[strlen(str) + 1];
    strcpy(s, str);

    return s;
}
