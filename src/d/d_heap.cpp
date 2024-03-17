#include <d/d_heap.h>

dHeap dHeap::work1Heap;
dHeap dHeap::work2Heap;
dHeap dHeap::workExHeap;
dHeap dHeap::layoutHeap;
dHeap dHeap::layoutExHeap;
dHeap dHeap::layoutEx2Heap;
dHeap dHeap::layoutResHeap;
dHeap dHeap::fontHeap;
dHeap dHeap::HBMHeap;

EGG::ExpHeap *dHeap::init(const char *name, size_t size, EGG::Heap *parent) {
    heap = EGG::ExpHeap::create(size, parent, 4);
    if (heap != nullptr) {
        heap->setGroupID(0);
        heap->mName = name;
    }
    return heap;
}
void dHeap::createWork1Heap(size_t size, EGG::Heap *parent) {
    work1Heap.init("ゲーム用作業用ヒープ(dHeap::work1Heap)", size, parent);
}
void dHeap::createWork2Heap(size_t size, EGG::Heap *parent) {
    work2Heap.init("ゲーム用作業用ヒープ(dHeap::work2Heap)", size, parent);
}
void dHeap::createWorkExHeap(size_t size, EGG::Heap *parent) {
    workExHeap.init("ゲーム用拡張作業用ヒープ(dHeap::workExHeap)", size, parent);
}
void dHeap::createLayoutHeap(size_t size, EGG::Heap *parent) {
    layoutHeap.init("レイアウト作業用ヒープ(dHeap::layoutHeap)", size, parent);
}
void dHeap::createLayoutExHeap(size_t size, EGG::Heap *parent) {
    layoutExHeap.init("レイアウト拡張作業用ヒープ(dHeap::layoutExHeap)", size, parent);
}
void dHeap::createLayoutEx2Heap(size_t size, EGG::Heap *parent) {
    layoutEx2Heap.init("レイアウト拡張作業用ヒープ2(dHeap::layoutEx2Heap)", size, parent);
}
void dHeap::createLayoutResHeap(size_t size, EGG::Heap *parent) {
    layoutResHeap.init("レイアウトリソース用ヒープ(dHeap::layoutResHeap)", size, parent);
}
void dHeap::createFontHeap(size_t size, EGG::Heap *parent) {
    fontHeap.init("フォント用ヒープ(dHeap::fontHeap)", size, parent);
}
void dHeap::createHBMHeap(size_t size, EGG::Heap *parent) {
    HBMHeap.init("HBM用ヒープ(dHeap::HBMHeap)", size, parent);
}
