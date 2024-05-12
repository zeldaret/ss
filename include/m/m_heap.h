#ifndef M_HEAP_H
#define M_HEAP_H

#include "egg/core/eggExpHeap.h"
#include "egg/core/eggFrmHeap.h"

// #include "egg/core/eggExpHeap.h"
// #include "egg/core/eggAssertHeap.h"

// TODO: REWRITE THIS

namespace EGG {
class ExpHeap;
class AssertHeap;
} // namespace EGG
// TODO: Doc symbols and func locations
class mHeap {
public:
    /* 802f0f00 */ static u16 copyAttribute(u32);
    /* 802f0f40 */ static EGG::Heap *setCurrentHeap(EGG::Heap *);
    /* 802f0f50 */ static EGG::Heap *createExpHeap(size_t size, EGG::Heap *parentHeap, const char *name, u32 align,
            u32 attrs);
    /* 802f1060 */ static size_t adjustExpHeap(EGG::ExpHeap *heap);
    /* 802f10d0 */ static size_t expHeapCost(size_t start, u32 size);
    /* 802f10f0 */ static EGG::FrmHeap *createFrmHeap(size_t size, EGG::Heap *parentHeap, const char *name,
            size_t align, size_t attrs);
    /* 802f1200 */ static void destroyFrmHeap(EGG::FrmHeap *heap);
    /* 802f1220 */ static size_t adjustFrmHeap(EGG::FrmHeap *heap);
    /* 802f1290 */ static size_t frmHeapCost(size_t start, u32 size);
    mHeap() {
        heap = nullptr;
    }
    /* 802f12b0 */ mHeap(EGG::Heap *heap);
    /* 802f12f0 */ ~mHeap();
    /* 802f1350 */ static EGG::Heap *createHeap(size_t size, EGG::Heap *, const char *name);
    /* 802f13d0 */ static void saveCurrentHeap();
    /* 802f13e0 */ static void restoreCurrentHeap();
    /* 802f1410 */ static EGG::FrmHeap *makeFrmHeapAndUpdate(size_t size, EGG::Heap *parentHeap, const char *name,
            u32 align, u32 attrs);
    /* 802f1450 */ static int getGameHeapNum();
    /* 802f1460 */ static EGG::Heap *createGameHeap(int heapNum, size_t size, EGG::Heap *parentHeap);
    /* 802f1510 */ static void createGameHeap1(size_t size, EGG::Heap *parentHeap);
    /* 802f1560 */ static void createArchiveHeap(size_t size, EGG::Heap *parentHeap);
    /* 802f1590 */ static void createCommandHeap(size_t size, EGG::Heap *parentHeap);
    /* 802f15c0 */ static void createDylinkHeap(size_t size, EGG::Heap *parentHeap);
    /* 802f15f0 */ static EGG::AssertHeap *createAssertHeap(EGG::Heap *parentHeap);
    /* 802f1640 */ static EGG::Heap *makeHeapOnCurrentGameHeap(size_t size, const char *name, u32 align, u32 flags);

    static int getDefaultGameHeapId();

    static bool isValidHeapId(u32 id) {
        return id >= 1 && id <= 2;
    }

    static EGG::Heap *g_gameHeaps[4];
    static EGG::Heap *s_SavedCurrentHeap;
    static EGG::Heap *g_archiveHeap;
    static EGG::Heap *g_commandHeap;
    static EGG::Heap *g_dylinkHeap;
    static EGG::AssertHeap *g_assertHeap;
    static u8 g_DefaultGameHeapId;
    static const char *const s_GameHeapNames[4];

    EGG::Heap *heap;
}; // namespace mHeap

#endif
