#ifndef INLINE_STRING_H
#define INLINE_STRING_H

#include <MSL_C/string.h>

inline void inline_strncat(char *dest, const char *src, size_t destSize) {
    if (src != nullptr) {
        size_t destLen = strlen(dest);
        size_t copyLen = strlen(src);

        // Make sure copy length isnt more than destination length
        if (destLen + copyLen + 1 >= destSize) {
            copyLen = destSize - destLen - 1;
        }

        strncpy(dest + destLen, src, copyLen);

        // make sure string is null terminated
        size_t offset = destLen + copyLen;
        dest[offset] = '\0';
    }
}

inline void inline_strncpy(char *dest, const char *src, size_t destSize) {
    if (src != dest) {
        dest[0] = '\0';
        inline_strncat(dest, src, destSize);
    }
}

#endif
