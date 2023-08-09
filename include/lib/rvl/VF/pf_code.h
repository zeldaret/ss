#ifndef RVL_SDK_VF_PF_CODE_H
#define RVL_SDK_VF_PF_CODE_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define VF_FN_MIN_CHAR 0x20 // (Space)
#define VF_FN_MAX_CHAR 0x80 // (DEL)

typedef enum {
    PFCODE_FN_INVALID = 0,
    PFCODE_FN_VALID_LONG = (1 << 0),
    PFCODE_FN_VALID_SHORT = (1 << 1),
} PFCODE_FnValidity;

typedef s32 (*PF_OEM_2_UNICODE)(char* src, u16* dst);
typedef s32 (*PF_UNICODE_2_OEM)(u16* src, char* dst);
typedef s32 (*PF_OEM_CHAR_WIDTH)(char* ch);
typedef s32 (*PF_UNICODE_CHAR_WIDH)(wchar_t* ch);
typedef u32 (*PF_IS_OEM_MB_CHAR)(char ch, u32 target);
typedef u32 (*PF_IS_UNICODE_MB_CHAR)(wchar_t ch, u32 target);

typedef struct {
    PF_OEM_2_UNICODE oem2unicode;             // at 0x0
    PF_UNICODE_2_OEM unicode2oem;             // at 0x4
    PF_OEM_CHAR_WIDTH oem_char_width;         // at 0x8
    PF_IS_OEM_MB_CHAR is_oem_mb_char;         // at 0xC
    PF_UNICODE_CHAR_WIDH unicode_char_width;  // at 0x10
    PF_IS_UNICODE_MB_CHAR is_unicode_mb_char; // at 0x14
} PF_CHARCODE;

extern const u8 VFipf_valid_fn_char[VF_FN_MAX_CHAR - VF_FN_MIN_CHAR];

int VFiPFCODE_Combine_Width(s16 oem_width, s16 uni_width);
void VFiPFCODE_Divide_Width(u32 width, s16* oem_width, s16* uni_width);

#ifdef __cplusplus
}
#endif
#endif
