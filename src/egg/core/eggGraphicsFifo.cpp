#include <rvl/GX.h>
#include <egg/core/eggGraphicsFifo.h>
#include <egg/core/eggHeap.h>


extern "C" void *GXInit(void *buf, u32 bufSize);
extern "C" void GXGetGPStatus(GXBool *overhi, GXBool *underlow, GXBool *readIdle, GXBool *cmdIdle, GXBool *brkpt);

namespace EGG {


GraphicsFifo *GraphicsFifo::sGraphicsFifo;
u8 GraphicsFifo::sGpStatus[];

void GraphicsFifo::create(u32 size, Heap *heap) {
    if (heap == nullptr) {
        heap = Heap::sCurrentHeap;
    }
    sGraphicsFifo = new(heap, 0x04) GraphicsFifo(size, heap);
}

GraphicsFifo::~GraphicsFifo() {
    do {
        GXGetGPStatus(&sGpStatus[0], &sGpStatus[1], &sGpStatus[2], &sGpStatus[3], &sGpStatus[4]);
    } while (sGpStatus[2] == false);
    Heap::free(mBuffBase, nullptr);
}

GraphicsFifo::GraphicsFifo(u32 size, Heap *heap) {
    mBufSize = ROUND_UP(size, 0x20);
    mBuffBase = Heap::alloc(mBufSize, 0x20, heap);
    mGxInitData = GXInit(mBuffBase, mBufSize);
}

} // namespace EGG
