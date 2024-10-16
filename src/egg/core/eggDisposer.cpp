#include "egg/core/eggDisposer.h"

#include "egg/core/eggHeap.h"


namespace EGG {

Disposer::Disposer() {
    mContainHeap = Heap::findContainHeap(this);
    if (mContainHeap) {
        mContainHeap->appendDisposer(this);
    }
}

Disposer::~Disposer() {
    if (mContainHeap) {
        mContainHeap->removeDisposer(this);
    }
}

} // namespace EGG
