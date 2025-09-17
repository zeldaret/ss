#ifndef M_HEAP_H
#define M_HEAP_H

#include "common.h"
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
    // TODO - fix when importing RVL MEM
    enum AllocOptBit_t {
        OPT_NONE = 0x0,
        OPT_1 = 0x1,
        OPT_2 = 0x2,
        OPT_4 = 0x4,
    };

    static u16 GetOptFlag(AllocOptBit_t);
    static EGG::Heap *setCurrentHeap(EGG::Heap *);
    static EGG::Heap *
    createExpHeap(size_t size, EGG::Heap *parentHeap, const char *name, u32 align, AllocOptBit_t attrs);
    static size_t adjustExpHeap(EGG::ExpHeap *heap);
    static size_t expHeapCost(size_t start, u32 size);
    static EGG::FrmHeap *
    createFrmHeap(size_t size, EGG::Heap *parentHeap, const char *name, size_t align, AllocOptBit_t attrs);
    static void destroyFrmHeap(EGG::FrmHeap *heap);
    static size_t adjustFrmHeap(EGG::FrmHeap *heap);
    static size_t frmHeapCost(size_t start, u32 size);
    mHeap() : heap(nullptr) {}
    mHeap(EGG::Heap *heap);
    ~mHeap();
    static EGG::ExpHeap *createHeap(size_t size, EGG::Heap *, const char *name);
    static void saveCurrentHeap();
    static void restoreCurrentHeap();
    static EGG::FrmHeap *
    createFrmHeapToCurrent(size_t size, EGG::Heap *parentHeap, const char *name, u32 align, AllocOptBit_t attrs);
    static int getGameHeapNum();
    static EGG::Heap *createGameHeap(int heapNum, size_t size, EGG::Heap *parentHeap);
    static void createGameHeap1(size_t size, EGG::Heap *parentHeap);
    static void createArchiveHeap(size_t size, EGG::Heap *parentHeap);
    static void createCommandHeap(size_t size, EGG::Heap *parentHeap);
    static void createDylinkHeap(size_t size, EGG::Heap *parentHeap);
    static EGG::AssertHeap *createAssertHeap(EGG::Heap *parentHeap);
    static EGG::FrmHeap *makeHeapOnCurrentGameHeap(size_t size, const char *name, u32 align, AllocOptBit_t flags);

    static int getDefaultGameHeapId();

    static bool isValidHeapId(u32 id) {
        return id >= 1 && id <= 2;
    }

    static EGG::Heap *g_gameHeaps[4];
    static EGG::Heap *s_SavedCurrentHeap;
    static EGG::Heap *g_archiveHeap;
    static EGG::Heap *g_commandHeap;
    static EGG::ExpHeap *g_dylinkHeap;
    static EGG::AssertHeap *g_assertHeap;
    static u8 g_DefaultGameHeapId;
    static const char *const s_GameHeapNames[4];

    EGG::Heap *heap;
}; // namespace mHeap

#endif
