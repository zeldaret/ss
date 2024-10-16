#ifndef RVL_SDK_AX_VPB_H
#define RVL_SDK_AX_VPB_H
#include "common.h"
#include "rvl/AX/AXPB.h"


#ifdef __cplusplus
extern "C" {
#endif

#define AX_SAMPLE_RATE 32000
#define AX_VOICE_MAX 96

enum {
    AX_PBSYNC_SELECT = (1 << 0),
    AX_PBSYNC_MIXER_CTRL = (1 << 1),
    AX_PBSYNC_STATE = (1 << 2),
    AX_PBSYNC_TYPE = (1 << 3),
    AX_PBSYNC_MIX = (1 << 4),
    AX_PBSYNC_ITD = (1 << 5),
    AX_PBSYNC_ITD_SHIFT = (1 << 6),
    AX_PBSYNC_DPOP = (1 << 7),
    AX_PBSYNC_VE = (1 << 8),
    AX_PBSYNC_VE_DELTA = (1 << 9),
    AX_PBSYNC_ADDR = (1 << 10),
    AX_PBSYNC_LOOP_FLAG = (1 << 11),
    AX_PBSYNC_LOOP_ADDR = (1 << 12),
    AX_PBSYNC_END_ADDR = (1 << 13),
    AX_PBSYNC_CURR_ADDR = (1 << 14),
    AX_PBSYNC_ADPCM = (1 << 15),
    AX_PBSYNC_SRC = (1 << 16),
    AX_PBSYNC_SRC_RATIO = (1 << 17),
    AX_PBSYNC_ADPCM_LOOP = (1 << 18),
    AX_PBSYNC_LPF = (1 << 19),
    AX_PBSYNC_LPF_COEFS = (1 << 20),
    AX_PBSYNC_BIQUAD = (1 << 21),
    AX_PBSYNC_BIQUAD_COEFS = (1 << 22),
    AX_PBSYNC_REMOTE = (1 << 23),
    AX_PBSYNC_RMT_MIXER_CTRL = (1 << 24),
    AX_PBSYNC_RMTMIX = (1 << 25),
    AX_PBSYNC_RMTDPOP = (1 << 26),
    AX_PBSYNC_RMTSRC = (1 << 27),
    AX_PBSYNC_RMTIIR = (1 << 28),
    AX_PBSYNC_RMTIIR_LPF_COEFS = (1 << 29),
    AX_PBSYNC_RMTIIR_BIQUAD_COEFS = (1 << 30),
    AX_PBSYNC_ALL = (1 << 31),
};

typedef void (*AXVoiceCallback)(void *voice);

typedef struct _AXVPB {
    void *next;               // at 0x0
    void *prev;               // at 0x4
    void *next1;              // at 0x8
    u32 priority;             // at 0xC
    AXVoiceCallback callback; // at 0x10
    u32 userContext;          // at 0x14
    u32 index;                // at 0x18
    u32 sync;                 // at 0x1C
    u32 depop;                // at 0x20
    void *itdBuffer;          // at 0x24
    AXPB pb;                  // at 0x28
} AXVPB;

s32 __AXGetNumVoices(void);
void __AXServiceVPB(AXVPB *vpb);
void __AXDumpVPB(AXVPB *vpb);
void __AXSyncPBs(u32 baseCycles);
AXPB *__AXGetPBs(void);
void __AXSetPBDefault(AXVPB *vpb);
void __AXVPBInit(void);
void AXSetVoiceState(AXVPB *vpb, u16 state);
void AXSetVoiceAddr(AXVPB *vpb, AXPBADDR *addr);
void AXGetLpfCoefs(u16 freq, u16 *a, u16 *b);
void AXSetMaxDspCycles(u32 num);
s32 AXGetMaxVoices(void);

#ifdef __cplusplus
}
#endif
#endif
