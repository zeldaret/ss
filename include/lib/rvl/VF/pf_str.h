#ifndef RVL_SDK_VF_PF_STRING_H
#define RVL_SDK_VF_PF_STRING_H
#include <lib/rvl/VF/pf.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PFSTR_CODEMODE_NONE,
    PFSTR_CODEMODE_OEM,
    PFSTR_CODEMODE_UNICODE,
    PFSTR_CODEMODE_LOCAL,
} PFSTR_CodeMode;

typedef struct PF_STR {
    const char* head;  // at 0x0
    const char* tail;  // at 0x4
    const char* local; // at 0x8
    u32 mode;          // at 0xC
} PF_STR;

void VFiPFSTR_SetCodeMode(PF_STR* str, u32 mode);
u32 VFiPFSTR_GetCodeMode(PF_STR* str);
void VFiPFSTR_SetLocalStr(PF_STR* str, char* local);
char* VFiPFSTR_GetStrPos(PF_STR* str, u32 target);
void VFiPFSTR_MoveStrPos(PF_STR* str, s16 n);
s32 VFiPFSTR_InitStr(PF_STR* str, const char* s, u32 mode);
u16 VFiPFSTR_StrLen(PF_STR* str);
u16 VFiPFSTR_StrNumChar(PF_STR* str, u32 target);
s32 VFiPFSTR_StrCmp(const PF_STR* str, const char* cmp);
int VFiPFSTR_StrNCmp(PF_STR* str, const char* cmp, u32 target, s16 offset,
                     u16 n);
void VFiPFSTR_ToUpperNStr(PF_STR* str, u16 n, char* out);

#ifdef __cplusplus
}
#endif
#endif
