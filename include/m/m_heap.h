#pragma once

#include "egg/core/eggHeap.h"
#include "egg/core/eggFrmHeap.h"
// #include "egg/core/eggExpHeap.h"
// #include "egg/core/eggAssertHeap.h"

namespace EGG {
    class ExpHeap;
    class AssertHeap;
}
// TODO: Doc symbols and func locations
namespace mHeap {
    /* 802f0f00 */ u8 copyAttribute(u32);
    /* 802f0f40 */ EGG::Heap* setCurrentHeap(EGG::Heap*);
    /* 802f0f50 */ EGG::ExpHeap* createExpHeap(s32 size, EGG::Heap* parentHeap, char* name, s32 align, u32 unk);
    /* 802f1060 */ void adjustExpHeap(EGG::Heap* heap);
    /* 802f10d0 */ s32 expHeapCost(s32 start, s32 size);
    /* 802f10f0 */ EGG::FrmHeap* createFrmHeap(s32 size, EGG::Heap* parentHeap, char* name, s32 align, u32 unk);
    /* 802f1200 */ void destroyFrmHeap(EGG::Heap* heap);
    /* 802f1220 */ void adjustFrmHeap(EGG::Heap* heap);
    /* 802f1290 */ s32 frmHeapCost(s32 start, s32 size);
    /* 802f12b0 */ void setTempHeap(EGG::Heap** prevHeap, EGG::Heap* tempNewHeap);
    /* 802f12f0 */ void restoreTempHeap(EGG::Heap** prevHeap);
    /* 802f1350 */ void createHeap(s32 size, EGG::Heap*, char* name);
    /* 802f13d0 */ void saveCurrentHeap();
    /* 802f13e0 */ void restoreCurrentHeap();
    /* 802f1410 */ EGG::Heap* makeFrmHeapAndSet(s32 size, EGG::Heap* parentHeap, char* name, s32 align, u32 unk);
    /* 802f1450 */ int getGameHeapNum();
    /* 802f1460 */ EGG::Heap* createGameHeap(s32 heapNum, s32 size, EGG::Heap* parentHeap);
    /* 802f1510 */ EGG::Heap* createGameHeap1(s32 size, EGG::Heap* parentHeap);
    /* 802f1560 */ void createArchiveHeap(s32 size, EGG::Heap* parentHeap);
    /* 802f1590 */ void createCommandHeap(s32 size, EGG::Heap* parentHeap);
    /* 802f15c0 */ void createDylinkHeap(s32 size, EGG::Heap* parentHeap);
    /* 802f15f0 */ void createAssertHeap(EGG::Heap* parentHeap);
    /* 802f1640 */ EGG::Heap* makeHeapOnCurrentGameHeap(s32 size, char* name, s32 align, u32 unk);
};