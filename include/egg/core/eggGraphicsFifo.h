#ifndef EGG_GRAPHICS_FIFO_H
#define EGG_GRAPHICS_FIFO_H

#include "common.h"
#include "egg/core/eggHeap.h"


namespace EGG {

class GraphicsFifo {
public:
    // vtable 0x00
    /* vt 0x08 */ virtual ~GraphicsFifo();

public:
    /* 0x04 */ void *mGxInitData;
    /* 0x08 */ void *mBuffBase;
    /* 0x0C */ u32 mBufSize;

public:
    static void create(u32 size, Heap *heap);
    GraphicsFifo(u32 size, Heap *heap);

public:
    static GraphicsFifo *sGraphicsFifo;
    static u8 sGpStatus[5];
};

} // namespace EGG

#endif
