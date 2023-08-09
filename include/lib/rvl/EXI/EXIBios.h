#ifndef RVL_SDK_EXI_BIOS_H
#define RVL_SDK_EXI_BIOS_H
#include <lib/rvl/EXI/EXICommon.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EXI_FREQ_1MHZ,
    EXI_FREQ_2MHZ,
    EXI_FREQ_4MHZ,
    EXI_FREQ_8MHZ,
    EXI_FREQ_16MHZ,
    EXI_FREQ_32HZ,
} EXIFreq;

typedef struct EXIItem {
    u32 dev;              // at 0x0
    EXICallback callback; // at 0x4
} EXIItem;

typedef struct EXIData {
    EXICallback exiCallback; // at 0x0
    EXICallback tcCallback;  // at 0x4
    EXICallback extCallback; // at 0x8
    volatile s32 state;      // at 0xC
    s32 bytesRead;           // at 0x10
    void* buffer;            // at 0x14
    u32 dev;                 // at 0x18
    u32 id;                  // at 0x1C
    UNKWORD WORD_0x20;
    s32 numItems;     // at 0x24
    EXIItem items[3]; // at 0x28
} EXIData;

BOOL EXIImm(EXIChannel chan, void* buf, s32 len, u32 type,
            EXICallback callback);
BOOL EXIImmEx(EXIChannel chan, void* buf, s32 len, u32 type);
BOOL EXIDma(EXIChannel chan, void* buf, s32 len, u32 type,
            EXICallback callback);
BOOL EXISync(EXIChannel chan);
void EXIClearInterrupts(EXIChannel chan, BOOL exi, BOOL tc, BOOL ext);
EXICallback EXISetExiCallback(EXIChannel chan, EXICallback callback);
void EXIProbeReset(void);
BOOL EXIProbe(EXIChannel chan);
BOOL EXIAttach(EXIChannel chan, EXICallback callback);
BOOL EXIDetach(EXIChannel chan);
BOOL EXISelect(EXIChannel chan, u32 dev, u32 freq);
BOOL EXIDeselect(EXIChannel chan);
void EXIInit(void);
BOOL EXILock(EXIChannel chan, u32 dev, EXICallback callback);
BOOL EXIUnlock(EXIChannel chan);
s32 EXIGetID(EXIChannel chan, u32 dev, u32* out);

#ifdef __cplusplus
}
#endif
#endif
