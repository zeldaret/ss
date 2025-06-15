#ifndef EGG_BUFFER_H
#define EGG_BUFFER_H

#include "common.h"
#include "egg/core/eggHeap.h"

namespace EGG {

template <typename T>
class TBuffer {
public:
    class iterator {
    public:
        iterator(TBuffer *pBuffer, int index) : mBuffer(pBuffer), mIndex(index) {}

        int get_index() const {
            return mIndex;
        }

        iterator &operator++() {
            mIndex++;
            return *this;
        }

        T &operator*() {
            return (*mBuffer)(mIndex);
        }

        friend bool operator==(const iterator &rLhs, const iterator &rRhs) {
            return rLhs.mBuffer == rRhs.mBuffer && rLhs.mIndex == rRhs.mIndex;
        }
        friend bool operator!=(const iterator &rLhs, const iterator &rRhs) {
            return !(rLhs == rRhs);
        }

    private:
        TBuffer *mBuffer; // at 0x0
        int mIndex;       // at 0x4
    };

public:
    TBuffer() : mSize(0), mBuffer(NULL) {}

    virtual ~TBuffer() {
        if (mBuffer == NULL) {
            return;
        }

        delete[] mBuffer;
        mBuffer = NULL;
    } // at 0x8

    virtual void allocate(int size, int align = 0); // at 0xC

    virtual void allocate(int size, Heap *pHeap, int align = 0); // at 0x10

    virtual void onAllocate(Heap * /* pHeap */) {} // at 0x14
    virtual void errRangeOver() const {}           // at 0x18

    T &operator()(int i) {
        checkRange(i);
        return mBuffer[i];
    }
    const T &operator()(int i) const {
        checkRange(i);
        return mBuffer[i];
    }

    T &get(int i) {
        checkRange(i);
        return mBuffer[i];
    }
    const T &get(int i) const {
        checkRange(i);
        return mBuffer[i];
    }

    int getSize() const {
        return mSize;
    }
    int size() const {
        return mSize;
    }

    iterator begin() {
        return iterator(this, 0);
    }
    iterator end() {
        return iterator(this, getSize());
    }

private:
    void flush() {}

    void checkRange(int i) const {
        if (!isRangeValid(i)) {
            errRangeOver();
        }
    }

    bool isRangeValid(int i) const {
        return i >= 0 && i < mSize;
    }

private:
    int mSize;  // at 0x4
    T *mBuffer; // at 0x8
};

template <typename T>
void TBuffer<T>::allocate(int size, int) {
    mSize = size;
    mBuffer = new T[size];
    onAllocate(nullptr);
}
template <typename T>
void TBuffer<T>::allocate(int size, Heap *pHeap, int) {
    mSize = size;

    if (pHeap == NULL) {
        pHeap = Heap::getCurrentHeap();
    }

    mBuffer = new (pHeap) T[mSize];

    flush();
    onAllocate(pHeap);
}

} // namespace EGG

#endif
