#pragma once

#include "egg/core/eggFrmHeap.h"
#include "egg/core/eggHeap.h"

// #include "egg/core/eggExpHeap.h"
// #include "egg/core/eggAssertHeap.h"

namespace EGG {
class ExpHeap;
class AssertHeap;
} // namespace EGG
// TODO: Doc symbols and func locations
namespace mHeap {
/* 802f0f00 */ u8 copyAttribute(u32);
/* 802f0f40 */ EGG::Heap *setCurrentHeap(EGG::Heap *);
/* 802f0f50 */ EGG::ExpHeap *createExpHeap(size_t size, EGG::Heap *parentHeap, char *name, s32 align, u32 unk);
/* 802f1060 */ void adjustExpHeap(EGG::Heap *heap);
/* 802f10d0 */ s32 expHeapCost(size_t start, size_t size);
/* 802f10f0 */ EGG::FrmHeap *createFrmHeap(size_t size, EGG::Heap *parentHeap, char *name, s32 align, u32 unk);
/* 802f1200 */ void destroyFrmHeap(EGG::FrmHeap *heap);
/* 802f1220 */ size_t adjustFrmHeap(EGG::FrmHeap *heap);
/* 802f1290 */ s32 frmHeapCost(size_t start, size_t size);
/* 802f12b0 */ void setTempHeap(EGG::Heap **prevHeap, EGG::Heap *tempNewHeap);
/* 802f12f0 */ void restoreTempHeap(EGG::Heap **prevHeap);
/* 802f1350 */ void createHeap(size_t size, EGG::Heap *, char *name);
/* 802f13d0 */ void saveCurrentHeap();
/* 802f13e0 */ void restoreCurrentHeap();
/* 802f1410 */ EGG::FrmHeap *makeFrmHeapAndUpdate(size_t size, EGG::Heap *parentHeap, const char *name, s32 align,
        u32 unk);
/* 802f1450 */ int getGameHeapNum();
/* 802f1460 */ EGG::Heap *createGameHeap(s32 heapNum, size_t size, EGG::Heap *parentHeap);
/* 802f1510 */ EGG::Heap *createGameHeap1(s32 size, EGG::Heap *parentHeap);
/* 802f1560 */ void createArchiveHeap(size_t size, EGG::Heap *parentHeap);
/* 802f1590 */ void createCommandHeap(size_t size, EGG::Heap *parentHeap);
/* 802f15c0 */ void createDylinkHeap(size_t size, EGG::Heap *parentHeap);
/* 802f15f0 */ void createAssertHeap(EGG::Heap *parentHeap);
/* 802f1640 */ EGG::Heap *makeHeapOnCurrentGameHeap(size_t size, char *name, s32 align, u32 unk);
extern EGG::ExpHeap *g_gameHeaps[4];
extern EGG::ExpHeap *s_SavedCurrentHeap;
extern EGG::ExpHeap *g_archiveHeap;
extern EGG::ExpHeap *g_commandHeap;
extern EGG::ExpHeap *g_dylinkHeap;
extern EGG::AssertHeap *g_assertHeap;
}; // namespace mHeap
