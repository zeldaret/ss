#include "m/m3d/m_banm.h"

#include "m/m3d/m3d.h"
#include "m/m_heap.h"


namespace m3d {
banm_c::~banm_c() {
    banm_c::remove();
}

void banm_c::remove() {
    if (mpAnmObj != nullptr) {
        mpAnmObj->DetachFromParent();
        mpAnmObj->Destroy();
        mpAnmObj = nullptr;
    }

    if (mpFrameHeap != nullptr) {
        mHeap::destroyFrmHeap(mpFrameHeap);
        mpFrameHeap = nullptr;
    }
}

bool banm_c::createAllocator(mAllocator_c *alloc, u32 *pStart) {
    // TODO understand this maybe?
    int i1 = mHeap::frmHeapCost(*pStart, 0x20);
    int i2 = mHeap::frmHeapCost(0, 0x20);
    i2 = ROUND_UP(i1, 0x20) - i2;
    i1 = mHeap::frmHeapCost(i2, 0x20);
    *pStart = ROUND_UP(i1, 0x20);
    mpFrameHeap = mHeap::createFrmHeap(
        i2, (EGG::Heap *)alloc->mHeap, "アニメ切り替え用アロケータ(m3d::banm_c::m_heap)", internal::l_alignment, 0
    );
    mAllocator.attach(mpFrameHeap, 0x20);
    return true;
}

bool banm_c::IsBound() const {
    if (mpAnmObj == nullptr) {
        return false;
    }
    return mpAnmObj->TestAnmFlag(nw4r::g3d::AnmObj::ANMFLAG_ISBOUND);
}

void banm_c::play() {}

f32 banm_c::getFrame() const {
    return mpAnmObj->GetFrame();
}

void banm_c::setFrameOnly(f32 frame) {
    mpAnmObj->SetFrame(frame);
}

f32 banm_c::getRate() const {
    return mpAnmObj->GetUpdateRate();
}

void banm_c::setRate(f32 f) {
    mpAnmObj->SetUpdateRate(f);
}

} // namespace m3d
