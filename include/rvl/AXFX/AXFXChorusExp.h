#ifndef RVL_SDK_AXFX_CHORUS_EXP_H
#define RVL_SDK_AXFX_CHORUS_EXP_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct AXFX_BUS;
typedef struct AXFX_BUFFERUPDATE;

typedef struct AXFX_CHORUS_EXP_DELAY {
    f32 *line[3]; // at 0x0
    u32 inPos;    // at 0xC
    u32 outPos;   // at 0x10
    u32 lastPos;  // at 0x14
    u32 sizeFP;   // at 0x18
    u32 size;     // at 0x1C
} AXFX_CHORUS_EXP_DELAY;

typedef struct AXFX_CHORUS_EXP_LFO {
    s32 *table;     // at 0x0
    s32 phaseAdd;   // at 0x4
    s32 stepSamp;   // at 0x8
    s32 depthSamp;  // at 0xC
    u32 phase;      // at 0x10
    u32 sign;       // at 0x14
    u32 lastNum;    // at 0x18
    s32 lastValue;  // at 0x1C
    s32 grad;       // at 0x20
    s32 gradFactor; // at 0x24
} AXFX_CHORUS_EXP_LFO;

typedef struct AXFX_CHORUS_EXP {
    AXFX_CHORUS_EXP_DELAY delay; // at 0x0
    AXFX_CHORUS_EXP_LFO lfo;     // at 0x20
    f32 history[3][4];           // at 0x48
    u32 histIndex;               // at 0x78
    u32 active;                  // at 0x7C
    f32 delayTime;               // at 0x80
    f32 depth;                   // at 0x84
    f32 rate;                    // at 0x88
    f32 feedback;                // at 0x8C
    struct AXFX_BUS *busIn;      // at 0x90
    struct AXFX_BUS *busOut;     // at 0x94
    f32 outGain;                 // at 0x98
    f32 sendGain;                // at 0x9C
} AXFX_CHORUS_EXP;

u32 AXFXChorusExpGetMemSize(const AXFX_CHORUS_EXP *fx);
BOOL AXFXChorusExpInit(AXFX_CHORUS_EXP *fx);
void AXFXChorusExpShutdown(AXFX_CHORUS_EXP *fx);
BOOL AXFXChorusExpSettings(AXFX_CHORUS_EXP *fx);
void AXFXChorusExpCallback(struct AXFX_BUFFERUPDATE *update, AXFX_CHORUS_EXP *fx);

#ifdef __cplusplus
}
#endif
#endif
