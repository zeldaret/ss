#ifndef EGG_BUFFER_H
#define EGG_BUFFER_H

#include "egg/core/eggHeap.h"

#include <common.h>


namespace EGG {

template <typename T>
class TBuffer {
public:
    // vtable 0x00
    /* vt 0x08 */ virtual ~TBuffer() {
        if (mBuffer != nullptr) {
            delete[] mBuffer;
            mBuffer = nullptr;
        }
    }
    /* vt 0x0C */ virtual void allocate(int n, int);
    /* vt 0x10 */ virtual void allocate(int n, Heap *heap, int);
    /* vt 0x14 */ virtual void onAllocate(Heap *) { return; }
    /* vt 0x18 */ virtual void errRangeOver() { return; }

public:
    /* 0x08 */ s32 mSize;
    /* 0x0C */ T *mBuffer;

public:
    inline TBuffer() : mSize(0), mBuffer(nullptr) {}

    inline bool isRangeValid(int i) { return (i >= 0 && i < mSize); }
    inline void checkRange(int i) {
        if (!isRangeValid(i)) {
            errRangeOver();
        }
    }
    inline T &operator()(int i) {
        checkRange(i);
        return mBuffer[i];
    }
    inline s32 getSize() { return mSize; }
};

template <typename T>
void TBuffer<T>::allocate(int n, int) {
    mSize = n;
    mBuffer = new T[n];
    onAllocate(nullptr);
}

template <typename T>
void TBuffer<T>::allocate(int n, Heap *heap, int) {
    mSize = n;
    if (heap == nullptr) {
        heap = Heap::sCurrentHeap;
    }
    mBuffer = new (heap, 4) T[mSize];
    onAllocate(heap);
}

} // namespace EGG

#endif
