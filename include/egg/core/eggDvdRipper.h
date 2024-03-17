#ifndef EGG_DVD_RIPPER_H
#define EGG_DVD_RIPPER_H

#include "egg/core/eggDecomp.h"
#include "egg/core/eggDvdFile.h"
#include "egg/core/eggHeap.h"
#include <common.h>

namespace EGG {

class DvdRipper {
public:
    enum EAllocDirection { ALLOC_CIR_PAD, ALLOC_DIR_TOP, ALLOC_DIR_BOTTOM };

public:
    /* 80494680 */ static u8 *loadToMainRAM(s32 entryNum, u8 *dst, Heap *heap, EAllocDirection allocDir, u32 offset,
            u32 *amountRead, u32 *fileSize);
    /* 80494730 */ static u8 *loadToMainRAM(const char *path, u8 *dst, Heap *heap, EAllocDirection allocDir, u32 offset,
            u32 *amountRead, u32 *fileSize);
    /* 804947e0 */ static u8 *loadToMainRAM(DvdFile *file, u8 *dst, Heap *heap, EAllocDirection allocDir, u32 offset,
            u32 *amountRead, u32 *fileSize);
    /* 804949b0 */ static void *loadToMainRAMDecomp(DvdFile *file, StreamDecomp *decompressor, u8 *dst, Heap *heap,
            EAllocDirection allocDir, u32 offset, s32 size, u32 maxChunkSize, u32 *amountRead, u32 *fileSize);

public:
    /* 80574ed0 */ static bool sErrorRetry;
    /*  */ // static sCallback;
};

} // namespace EGG

#endif
