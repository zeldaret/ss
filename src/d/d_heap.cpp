#include "d/d_heap.h"

#include "d/d_heap_alloc.h"
#include "s/s_Assert.h"

dHeap dHeap::work1Heap;
dHeap dHeap::work2Heap;
dHeap dHeap::workExHeap;
dHeap dHeap::layoutHeap;
dHeap dHeap::layoutExHeap;
dHeap dHeap::layoutEx2Heap;
dHeap dHeap::layoutResHeap;
dHeap dHeap::fontHeap;
dHeap dHeap::HBMHeap;
dHeapAllocator dHeapAllocator::sAllocator;

static bool sAssertOnAllocFailure = true;

// TODO TU splits?

void dHeapAllocator::onAlloc(EGG::HeapAllocArg *arg) {
    if (arg->ptr != nullptr) {
        return;
    }
    if (!sAssertOnAllocFailure) {
        return;
    }
    sAssert::assert();
}

EGG::ExpHeap *dHeap::init(const char *name, size_t size, EGG::Heap *parent) {
    heap = EGG::ExpHeap::create(size, parent, 4);
    if (heap != nullptr) {
        heap->setAllocMode(0);
        heap->mName = name;
    }
    return heap;
}

void dHeap::createWork1Heap(size_t size, EGG::Heap *parent) {
    static const char name[] = "ゲーム用作業用ヒープ(dHeap::work1Heap)";
    work1Heap.init(name, size, parent);
}
void dHeap::createWork2Heap(size_t size, EGG::Heap *parent) {
    static const char name[] = "ゲーム用作業用ヒープ(dHeap::work2Heap)";
    work2Heap.init(name, size, parent);
}
void dHeap::createWorkExHeap(size_t size, EGG::Heap *parent) {
    static const char name[] = "ゲーム用拡張作業用ヒープ(dHeap::workExHeap)";
    workExHeap.init(name, size, parent);
}
void dHeap::createLayoutHeap(size_t size, EGG::Heap *parent) {
    static const char name[] = "レイアウト作業用ヒープ(dHeap::layoutHeap)";
    layoutHeap.init(name, size, parent);
}
void dHeap::createLayoutExHeap(size_t size, EGG::Heap *parent) {
    static const char name[] = "レイアウト拡張作業用ヒープ(dHeap::layoutExHeap)";
    layoutExHeap.init(name, size, parent);
}
void dHeap::createLayoutEx2Heap(size_t size, EGG::Heap *parent) {
    static const char name[] = "レイアウト拡張作業用ヒープ2(dHeap::layoutEx2Heap)";
    layoutEx2Heap.init(name, size, parent);
}
void dHeap::createLayoutResHeap(size_t size, EGG::Heap *parent) {
    static const char name[] = "レイアウトリソース用ヒープ(dHeap::layoutResHeap)";
    layoutResHeap.init(name, size, parent);
}
void dHeap::createFontHeap(size_t size, EGG::Heap *parent) {
    static const char name[] = "フォント用ヒープ(dHeap::fontHeap)";
    fontHeap.init(name, size, parent);
}
void dHeap::createHBMHeap(size_t size, EGG::Heap *parent) {
    static const char name[] = "HBM用ヒープ(dHeap::HBMHeap)";
    HBMHeap.init(name, size, parent);
}

void dHeapAllocator::initCallbacks() {
    sAllocator.doInitCallbacks();
}

void *operator new(size_t size) {
    return EGG::Heap::alloc(size, 0x04, nullptr);
}

void *operator new[](size_t size) {
    return EGG::Heap::alloc(size, 0x04, nullptr);
}

void operator delete(void *ptr) {
    return EGG::Heap::free(ptr, nullptr);
}

void operator delete[](void *ptr) {
    return EGG::Heap::free(ptr, nullptr);
}
