#ifndef RVL_SDK_VF_PF_VOLUME_H
#define RVL_SDK_VF_PF_VOLUME_H
#include <lib/rvl/VF/pf_code.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// TODO: 0x37b91e
typedef struct PF_VOLUME {
    u8 dummy[0x1898];
} PF_VOLUME;

typedef struct PF_CUR_VOLUME {
    u32 stat;         // at 0x0
    s32 context_id;   // at 0x4
    PF_VOLUME* p_vol; // at 0x8
} PF_CUR_VOLUME;

typedef struct PF_CONTEXT {
    u32 stat;       // at 0x0
    s32 context_id; // at 0x4
} PF_CONTEXT;

typedef struct PF_VOLUME_SET {
    PF_CUR_VOLUME current_vol[1]; // at 0x0
    s32 num_attached_drives;      // at 0xC
    s32 num_mounted_volumes;      // at 0x10
    u32 config;                   // at 0x14
    void* param;                  // at 0x18
    s32 last_error;               // at 0x1C
    s32 last_driver_error;        // at 0x20
    PF_CHARCODE codeset;          // at 0x24
    u32 setting;                  // at 0x3C
    PF_CONTEXT context[1];        // at 0x40
    PF_VOLUME volumes[26];        // at 0x48
} PF_VOLUME_SET;

extern PF_VOLUME_SET VFipf_vol_set;

#ifdef __cplusplus
}
#endif
#endif
