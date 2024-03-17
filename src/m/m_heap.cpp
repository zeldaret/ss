#include <egg/core/eggAssertHeap.h>
#include <egg/core/eggExpHeap.h>
#include <egg/core/eggFrmHeap.h>

namespace mHeap {

u8 g_DefaultGameHeapId = 1;
static const char *s_GameHeapNames[4] = {
        0,
        "ゲーム用汎用ヒープ1(mHeap::gameHeaps[1])",
        "ゲーム用汎用ヒープ2(mHeap::gameHeaps[2])",
        0,
};

EGG::ExpHeap *g_gameHeaps[4];
EGG::ExpHeap *s_SavedCurrentHeap;
EGG::ExpHeap *g_archiveHeap;
EGG::ExpHeap *g_commandHeap;
EGG::ExpHeap *g_dylinkHeap;
EGG::AssertHeap *g_assertHeap;

u32 getHeapAttribute(u32) {}

EGG::Heap *setCurrentHeap(EGG::Heap *heap) {
    return heap->becomeCurrentHeap();
}
EGG::ExpHeap *createExpHeap(size_t, EGG::Heap *parent, const char *name, unsigned long, unsigned long) {}
void adjustExpHeap(EGG::ExpHeap *) {}
size_t expHeapCost(unsigned long, unsigned long) {}

EGG::FrmHeap *createFrmHeap(unsigned long, EGG::Heap *, const char *, unsigned long, unsigned long) {}
void destroyFrmHeap(EGG::FrmHeap *) {}
size_t adjustFrmHeap(EGG::FrmHeap *) {}
size_t frmHeapCost(unsigned long, unsigned long) {}

EGG::ExpHeap *createHeap(unsigned long, EGG::Heap *, const char *) {}
void saveCurrentHeap() {}
void restoreCurrentHeap() {}

int getDefaultGameHeapId() {
    return g_DefaultGameHeapId;
}

inline bool isValidHeapId(u32 id) {
    return id >= 1 && id <= 2;
}

EGG::ExpHeap *createGameHeap(int heapId, size_t size, EGG::Heap *parent) {
    if (!isValidHeapId(heapId)) {
        return nullptr;
    }

    g_gameHeaps[heapId] = createHeap(size, parent, s_GameHeapNames[heapId]);
    if (heapId == g_DefaultGameHeapId) {
        g_gameHeaps[0] = g_gameHeaps[heapId];
    }
    return g_gameHeaps[heapId];
}
EGG::ExpHeap *createGameHeap1(size_t size, EGG::Heap *parent) {
    return createGameHeap(getDefaultGameHeapId(), size, parent);
}
EGG::ExpHeap *createArchiveHeap(size_t size, EGG::Heap *parent) {
    g_archiveHeap = createHeap(size, parent, "");
}
EGG::ExpHeap *createCommandHeap(size_t size, EGG::Heap *parent) {
    g_commandHeap = createHeap(size, parent, "DVD読み込みコマンド用ヒープ(mHeap::commandHeap)");
}
EGG::ExpHeap *createDylinkHeap(size_t size, EGG::Heap *parent) {
    g_dylinkHeap = createHeap(size, parent, "ダイナミックリンク用ヒープ(mHeap::dylinkHeap)");
}
EGG::ExpHeap *createAssertHeap(EGG::Heap *parent) {
    g_assertHeap = EGG::AssertHeap::create(EGG::AssertHeap::getSize(), parent);
    g_assertHeap->mName = "アサートヒープ(mHeap::assertHeap)";
}
} // namespace mHeap
