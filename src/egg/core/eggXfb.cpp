#include <egg/core/eggSystem.h>
#include <egg/core/eggVideo.h>
#include <egg/core/eggXfb.h>


namespace EGG {

/* 804989e0 */ void Xfb::init(u16 width, u16 height, Heap *heap) {
    mWidth = width;
    mHeight = height;
    u32 size = calcBufferSize(width, height);
    if (heap == nullptr) {
        heap = Heap::sCurrentHeap;
    }

    u8 *buf = new (heap, 32) u8[size];
    mBuffer = buf;
    mPrev = nullptr;
    mNext = nullptr;
    mState = XFB_UNPROCESSED;
}

/* 80498a60 */ Xfb::Xfb(Heap *heap) {
    Video *video = BaseSystem::mConfigData->getVideo();
    init(video->pRenderMode->fbWidth, video->pRenderMode->xfbHeight, heap);
}

/* 80498ad0 */ u32 Xfb::calcBufferSize(u16 width, u16 height) {
    return ((width + 0xf) & 0xfff0) * height * 2;
}

} // namespace EGG
