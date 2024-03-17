#ifndef EGG_GRAPHICS_FIFO_H
#define EGG_GRAPHICS_FIFO_H

#include "egg/core/eggHeap.h"
#include <common.h>

namespace EGG {

class GraphicsFifo {
public:
    // vtable 0x00 | 8056eb80
    /* vt 0x08 | 80498e90*/ virtual ~GraphicsFifo();

public:
    /* 0x04 */ void *mGxInitData;
    /* 0x08 */ void *mBuffBase;
    /* 0x0C */ u32 mBufSize;

public:
    /* 80498e20 */ static void create(u32 size, Heap *heap);
    /* 80498f30 */ GraphicsFifo(u32 size, Heap *heap);

public:
    /* 80576798 */ static GraphicsFifo *sGraphicsFifo;
    /* 8057679c */ static bool sGpStatus[4];
};

} // namespace EGG

#endif
