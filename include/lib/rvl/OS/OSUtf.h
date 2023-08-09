#ifndef RVL_SDK_OS_UTF_H
#define RVL_SDK_OS_UTF_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

const u8* OSUTF8to32(const u8* utf8, u32* utf32);
const wchar_t* OSUTF16to32(const wchar_t* utf16, u32* utf32);
u8 OSUTF32toANSI(u32 utf32);
wchar_t OSUTF32toSJIS(u32 utf32);

#ifdef __cplusplus
}
#endif
#endif
