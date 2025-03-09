#ifndef SIZED_STRING_H
#define SIZED_STRING_H

#include "__va_arg.h"
#include "common.h"
#include "string.h"

extern "C" bool strequals(const char *a, const char *b);

/**
 * A statically sized string buffer used for resource
 * identification where strings are guaranteed to be short.
 *
 * Note: We aren't aware of any other projects that use a similar
 * class and given that SS has no debugging info anywhere it's hard
 * to be certain about anything.
 */
template <size_t Size>
struct SizedString {
    SizedString() {
        mChars[0] = '\0';
    }

    SizedString(const char *src) {
        operator=(src);
    }

    char mChars[Size];

    operator char *() {
        return mChars;
    }

    operator const char *() const {
        return mChars;
    }

    void operator=(const char *src) {
        if (src != mChars) {
            mChars[0] = '\0';
            operator+=(src);
        }
    }

    void operator+=(const char *src) {
        if (src != nullptr) {
            size_t destLen = strlen(mChars);
            size_t copyLen = strlen(src);

            // Make sure copy length isnt more than destination length
            if (destLen + copyLen + 1 >= Size) {
                size_t tmpLen = Size - destLen;
                copyLen = tmpLen - 1;
            }

            strncpy(mChars + destLen, src, copyLen);

            // make sure string is null terminated
            size_t offset = destLen + copyLen;
            mChars[offset] = '\0';
        }
    }

    bool operator==(const char *other) const {
        return strequals(mChars, other);
    }

    int sprintf(const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);

        int printed = vsnprintf(this->mChars, Size, fmt, args);
        if (printed != strlen(this->mChars)) {
            this->mChars[0] = '\0';
        }
        va_end(list);
        return printed;
    }

    size_t len() const {
        return strlen(mChars);
    }
};

// TODO this might be a shared template with SizedString but I'm
// not sure how to write the inline functions
template <size_t Size>
struct SizedWString {
    SizedWString() {
        mChars[0] = '\0';
    }

    wchar_t mChars[Size];

    operator wchar_t *() {
        return mChars;
    }

    operator const wchar_t *() const {
        return mChars;
    }

    int sprintf(const wchar_t *fmt, ...) {
        va_list args;
        va_start(args, fmt);

        int printed = vswprintf(this->mChars, Size, fmt, args);
        if (printed != wcslen(this->mChars)) {
            this->mChars[0] = '\0';
        }
        va_end(list);
        return printed;
    }
};

#endif
