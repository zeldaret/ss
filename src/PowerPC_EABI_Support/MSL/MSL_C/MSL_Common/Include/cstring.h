#ifndef STD_CMATH_H_
#define STD_CMATH_H_

#include "string.h"

namespace std {
inline size_t strlen(const char *str) {
    return ::strlen(str);
}

using ::strncpy;
using ::strncat;

inline char *strcpy(char *dest, const char *src) {
    return ::strcpy(dest, src);
}
} // namespace std

#endif
