#ifndef REVOSDK_KPAD_H
#define REVOSDK_KPAD_H

#include "common.h"
#include "rvl/WPAD/WPAD.h"

#include "rvl/MTX.h"

#ifdef __cplusplus
extern "C" {
#endif

#define KPAD_MAX_SAMPLES 16

typedef union KPADEXStatus {
    struct {
        Vec2 stick;    // at 0x0
        Vec acc;       // at 0x8
        f32 acc_value; // at 0x14
        f32 acc_speed; // at 0x18
    } fs;              // at 0x0

    struct {
        u32 hold;     // at 0x0
        u32 trig;     // at 0x4
        u32 release;  // at 0x8
        Vec2 lstick;  // at 0xC
        Vec2 rstick;  // at 0x14
        f32 ltrigger; // at 0x1C
        f32 rtrigger; // at 0x20
    } cl;             // at 0x0

    struct {
        f64 tgc_weight;     // at 0x0
        f64 weight[4];      // at 0x8
        f64 weight_ave[4];  // at 0x28
        s32 weight_err;     // at 0x48
        s32 tgc_weight_err; // at 0x4C
    } bl;                   // at 0x0
} KPADEXStatus;

typedef struct KPADStatus {
    u32 hold;               // at 0x0
    u32 trig;               // at 0x4
    u32 release;            // at 0x8
    Vec acc;                // at 0xC
    f32 acc_value;          // at 0x18
    f32 acc_speed;          // at 0x1C
    Vec2 pos;               // at 0x20
    Vec2 vec;               // at 0x28
    f32 speed;              // at 0x30
    Vec2 horizon;           // at 0x34
    Vec2 hori_vec;          // at 0x3C
    f32 hori_speed;         // at 0x44
    f32 dist;               // at 0x48
    f32 dist_vec;           // at 0x4C
    f32 dist_speed;         // at 0x50
    Vec2 acc_vertical;      // at 0x54
    u8 dev_type;            // at 0x5C
    s8 wpad_err;            // at 0x5D
    s8 dpd_valid_fg;        // at 0x5E
    u8 data_format;         // at 0x5F
    KPADEXStatus ex_status; // at 0x60
    u8 _0xB0[0xF0 - 0xB0];  // at 0xB0
} KPADStatus;

typedef struct KPADUnifiedWpadStatus {
    union {
        WPADStatus core;
        WPADFSStatus fs;
        WPADCLStatus cl;
        WPADTRStatus tr;
        WPADBLStatus bl;
    } u;        // at 0x0
    u8 fmt;     // 0x36;
    u8 padding; // 0x37;

} KPADUnifiedWpadStatus;

typedef void (*KPADConnectCallback)(s32, s32);

void KPADSetConnectCallback(u32, KPADConnectCallback);

void KPADSetPosParam(s32 chan, f32 playRadius, f32 sensitivity);
void KPADSetHoriParam(s32 chan, f32 playRadius, f32 sensitivity);
void KPADSetDistParam(s32 chan, f32 playRadius, f32 sensitivity);
void KPADSetAccParam(s32 chan, f32 playRadius, f32 sensitivity);

s32 KPADRead(s32 chan, KPADStatus *pSamples, s32 numSamples);
s32 KPADReadEx(s32 chan, UNKTYPE *status, s32, s32 *);

void KPADGetUnifiedWpadStatus(s32 chan, KPADUnifiedWpadStatus *pStatus, int);

void KPADInit(void);
void KPADInitEx(void * /* UNKTYPE */, u32);

void KPADSetBtnRepeat(s32 chan, f32, f32);
void KPADSetReviseMode(s32 chan, s32);

void KPADEnableMpls(s32 chan, s32);
void KPADDisableMpls(s32 chan);

s32 KPADGetMplsWorkSize();
void KPADSetMplsWorkarea(void *);
void KPADSetControlMplsCallback(s32 chan, KPADConnectCallback cb);
void KPADResetMpls(s32 chan);
void KPADDisableMplsZeroPlay(s32 chan);
void KPADSetMplsZeroPlayParam(s32 chan, f32);
void KPADSetMplsZeroDriftMode(s32 chan, s32);
void KPADEnableMplsDirRevise(s32 chan);
void KPADDisableMplsDirRevise(s32 chan);
void KPADSetMplsDirReviseParam(s32 chan, f32);
void KPADEnableMplsAccRevise(s32 chan);
void KPADSetMplsAccReviseParam(s32 chan, f32, f32);
void KPADEnableMplsDpdRevise(s32 chan);
void KPADSetMplsDpdReviseParam(s32 chan, f32);

#ifdef __cplusplus
}
#endif
#endif
