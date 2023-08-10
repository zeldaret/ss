#pragma once

#include <lib/egg/core/eggHeap.h>
#include <lib/egg/core/eggFrmHeap.h>
// #include <lib/egg/core/eggExpHeap.h>
// #include <lib/egg/core/eggAssertHeap.h>

namespace EGG {
    class ExpHeap;
    class AssertHeap;
}
// TODO: Doc symbols and func locations
namespace mHeap {
    EGG::ExpHeap* createExpHeap(s32 size, EGG::Heap* parentHeap, char* name, s32 align, u32 unk);
    void adjustExpHeap(EGG::Heap* heap);
    s32 expHeapCost(s32 start, s32 size);
    EGG::FrmHeap* createFrmHeap(s32 size, EGG::Heap* parentHeap, char* name, s32 align, u32 unk);
    void destroyFrmHeap(EGG::Heap* heap);
    void adjustFrmHeap(EGG::Heap* heap);
    s32 frmHeapCost(s32 start, s32 size);
    void setTempHeap(EGG::Heap** prevHeap, EGG::Heap* tempNewHeap);
    void restoreTempHeap(EGG::Heap** prevHeap);
    void createHeap(s32 size, EGG::Heap*, char* name);
    void saveCurrentHeap();
    void restoreCurrentHeap();
    EGG::Heap* makeFrmHeapAndSet(s32 size, EGG::Heap* parentHeap, char* name, s32 align, u32 unk);
    int getGameHeapNum();
    EGG::Heap* createGameHeap(s32 heapNum, s32 size, EGG::Heap* parentHeap);
    EGG::Heap* createGameHeap1(s32 size, EGG::Heap* parentHeap);
    void createArchiveHeap(s32 size, EGG::Heap* parentHeap);
    void createCommandHeap(s32 size, EGG::Heap* parentHeap);
    void createDylinkHeap(s32 size, EGG::Heap* parentHeap);
    void createAssertHeap(EGG::Heap* parentHeap);
    EGG::Heap* makeHeapOnCurrentGameHeap(s32 size, char* name, s32 align, u32 unk);
};