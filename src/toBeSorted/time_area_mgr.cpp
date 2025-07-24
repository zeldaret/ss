#include "toBeSorted/time_area_mgr.h"

#include "d/d_heap.h"
#include "d/d_sc_game.h"
#include "m/m_heap.h"
#include "nw4r/g3d/g3d_obj.h"

dTimeAreaMgr_c::dTimeAreaMgr_c() {
    sInstance = this;
}

dTimeAreaMgr_c::~dTimeAreaMgr_c() {
    sInstance = nullptr;
}

bool dTimeAreaMgr_c::init() {
    mAllocator.createFrmHeap(-1, dHeap::work1Heap.heap, "dTimeAreaMgr_c::m_allocator", 0x20, mHeap::OPT_NONE);
    mProcA.create(&mAllocator);
    mProcB.create(&mAllocator);
    mAllocator.adjustFrmHeap();
    mIsInLanayruMiningFacility = dScGame_c::isCurrentStage("D300") || dScGame_c::isCurrentStage("D300_1");
    return true;
}

void dTimeAreaMgr_c::draw() {
    if (field_0x64 != 0 && !dTimeAreaMgr_c::GetInstance()->isField0x78()) {
        mProcA.entry();
        mProcB.entry();
    }
}

void dTimeAreaMgrProcA_c::drawOpa() {
    // TODO
}

bool dTimeAreaMgrProcA_c::create(mHeapAllocator_c *alloc) {
    if (!m3d::proc_c::create(alloc, nullptr)) {
        return false;
    }

    setPriorityDraw(0x0C, 0);
    setOption(/* DISABLE_DRAW_XLU */ 0x07, 1);
    return true;
}

void dTimeAreaMgrProcB_c::drawOpa() {
    // TODO
}

bool dTimeAreaMgrProcB_c::create(mHeapAllocator_c *alloc) {
    if (!m3d::proc_c::create(alloc, nullptr)) {
        return false;
    }

    setPriorityDraw(0x13, 0);
    setOption(/* DISABLE_DRAW_XLU */ 0x07, 1);
    field_0x18 = 0x8C;
    field_0x19 = 0x64;
    field_0x1A = 0x3C;
    field_0x1B = 0x64;
    return true;
}
