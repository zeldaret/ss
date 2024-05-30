#include <egg/core/eggAllocator.h>
#include <m/m_allocator.h>
#include <m/m_heap.h>

// TODO this include is weird
#include <egg/core/eggAssertHeap.h>
#include <egg/core/eggHeap.h>

mAllocator_c::mAllocator_c() : EGG::Allocator(mHeap::g_assertHeap, 0x04) {}

mAllocator_c::~mAllocator_c() {}

bool mAllocator_c::attach(EGG::Heap *heap, int align) {
    if (heap == nullptr) {
        heap = EGG::Heap::sCurrentHeap;
    }
    this->align = align;
    this->mHeap = heap;
    heap->initAllocator(this, align);
    return true;
}

void *mAllocator_c::alloc(u32 size) {
    return EGG::Allocator::alloc(size);
}

void mAllocator_c::free(void *block) {
    EGG::Allocator::free(block);
}

mHeapAllocator_c::mHeapAllocator_c() {}
mHeapAllocator_c::~mHeapAllocator_c() {
    destroyHeap();
}

bool mHeapAllocator_c::replaceWithNewFrmHeap(s32 size, EGG::Heap *newHeap, char *heapName, s32 align, u32 attrs) {
    destroyHeap();
    EGG::Heap *heap = mHeap::createFrmHeap(size, newHeap, heapName, align, attrs);
    if (heap == nullptr) {
        return false;
    }
    attach(heap, align);
    return true;
}

bool mHeapAllocator_c::replaceWithNewExpHeap(s32 size, EGG::Heap *newHeap, char *heapName, s32 align, u32 attrs) {
    destroyHeap();
    EGG::Heap *heap = mHeap::createExpHeap(size, newHeap, heapName, align, attrs);
    if (heap == nullptr) {
        return false;
    }
    attach(heap, align);
    return true;
}

void mHeapAllocator_c::destroyHeap() {
    EGG::Heap *assertHeap = mHeap::g_assertHeap;
    if (mHeap != assertHeap) {
        mHeap->destroy();
        mHeap = assertHeap;
    }
}

inline EGG::Heap *getHeapOfKind(EGG::Heap *heap, EGG::Heap::eHeapKind kind) {
    if (heap != nullptr && heap->getHeapKind() == kind) {
        return heap;
    }
    return nullptr;
}

s32 mHeapAllocator_c::adjustFrmHeap() {
    EGG::Heap *heap = mHeap;
    if (heap == mHeap::g_assertHeap) {
        return 0;
    }
    return mHeap::adjustFrmHeap(static_cast<EGG::FrmHeap *>(getHeapOfKind(heap, EGG::Heap::HEAP_KIND_FRAME)));
}

s32 mHeapAllocator_c::adjustExpHeap() {
    EGG::Heap *heap = mHeap;
    if (heap == mHeap::g_assertHeap) {
        return 0;
    }
    return mHeap::adjustExpHeap(static_cast<EGG::ExpHeap *>(getHeapOfKind(heap, EGG::Heap::HEAP_KIND_EXPANDED)));
}

s32 mHeapAllocator_c::createNewTempFrmHeap(s32 size, EGG::Heap *newHeap, char *heapName, s32 align, u32 attrs) {
    if (!replaceWithNewFrmHeap(size, newHeap, heapName, align, attrs)) {
        return false;
    }
    mHeap::saveCurrentHeap();
    mHeap::setCurrentHeap(mHeap);
    return true;
}

void mHeapAllocator_c::adjustFrmHeapRestoreCurrent() {
    mHeap::restoreCurrentHeap();
    EGG::Heap *heap = mHeap;
    mHeap::adjustFrmHeap(static_cast<EGG::FrmHeap *>(getHeapOfKind(heap, EGG::Heap::HEAP_KIND_FRAME)));
}

void *mHeapAllocator_c::allocOnHeap(u32 size, mHeapAllocator_c *allocator) {
    return allocator->alloc(size);
}
