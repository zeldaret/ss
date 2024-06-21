#include <egg/core/eggFrmHeap.h>
#include <m/m3d/m3d.h>
#include <m/m3d/m_anmtexsrt.h>
#include <m/m_heap.h>

namespace m3d {

int anmTexSrt_c::child_c::getType() {
    return 0x04;
}

anmTexSrt_c::child_c::~child_c() {}

u32 anmTexSrt_c::child_c::heapCost(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexSrt srt, bool b) {
    u32 size = 0;
    nw4r::g3d::AnmObjTexSrtRes::Construct(nullptr, &size, srt, mdl, false);
    if (b) {
        size = ROUND_UP(mHeap::frmHeapCost(size, 0x20), 0x20);
    }
    return size;
}

bool anmTexSrt_c::child_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexSrt srt, mAllocator_c* alloc, u32* pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size;
    if (pSize == nullptr) {
        pSize = &size;
    }

    *pSize = child_c::heapCost(mdl, srt, false);
    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjTexSrtRes::Construct(&mAllocator, nullptr, srt, mdl, false);
    if (!mpAnmObj) {
        return false;
    }

    if (!mpAnmObj->Bind(mdl)) {
        remove();
        return false;
    }
    setFrmCtrlDefault(srt, PLAY_MODE_4);
    return true;
}

void anmTexSrt_c::child_c::setAnm(m3d::bmdl_c &mdl, nw4r::g3d::ResAnmTexSrt srt, m3d::playMode_e mode) {
    releaseAnm();
    mpAnmObj = nw4r::g3d::AnmObjTexSrtRes::Construct(&mAllocator, nullptr, srt, mdl.getResMdl(), false);
    mpAnmObj->Bind(mdl.getResMdl());
    setFrmCtrlDefault(srt, mode);
}

void anmTexSrt_c::child_c::releaseAnm() {
    if (mpAnmObj != nullptr) {
        mpAnmObj->Release();
        mpFrameHeap->free(3);
        mpAnmObj = nullptr;
    }
}

void anmTexSrt_c::child_c::setFrmCtrlDefault(nw4r::g3d::ResAnmTexSrt &srt, playMode_e mode) {
    if (mode == PLAY_MODE_4) {
        mode = srt.GetAnmPolicy() == nw4r::g3d::ANM_POLICY_ONETIME ? PLAY_MODE_1 : PLAY_MODE_0;
    }
    set(srt.GetNumFrame(), mode, 1.0f, -1.0f);
}

u32 anmTexSrt_c::heapCost(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexSrt srt, s32 num, bool b) {
    u32 size = 0;
    nw4r::g3d::AnmObjTexSrtOverride::Construct(nullptr, &size, mdl, num);
    size += ROUND_UP(num * sizeof(child_c), 0x20);
    u32 childCost = child_c::heapCost(mdl, srt, true);
    size += num * ROUND_UP(childCost, 0x20);

    if (b) {
        size = ROUND_UP(mHeap::frmHeapCost(size, 0x20), 0x20);
    }
    return size;
}

bool anmTexSrt_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexSrt srt, mAllocator_c* alloc, u32* pSize, s32 num) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size = 0;
    if (pSize == nullptr) {
        pSize = &size;
    }

    *pSize = heapCost(mdl, srt, num, false);
    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjTexSrtOverride::Construct(&mAllocator, nullptr, mdl, num);
    if (!mpAnmObj) {
        return false;
    }

    // TODO inline?
    mpChildren = (child_c*)MEMAllocFromAllocator(&mAllocator, ROUND_UP(num * sizeof(child_c), 0x20));
    if (!mpChildren) {
        remove();
        return false;
    }

    nw4r::g3d::AnmObjTexSrtOverride *obj = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtOverride>(mpAnmObj);
    child_c *child = mpChildren;
    for (int i = 0; i < num; i++) {
        new (child) child_c();
        if (!child->create(mdl, srt, &mAllocator, nullptr)) {
            mHeap::destroyFrmHeap(mpFrameHeap);
            EGG::Heap::free(mpFrameHeap, nullptr);
            return false;
        }

        if (i == 0) {
            obj->Attach(i, nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtRes>(child->getAnimObj()));
        } else {
            child->releaseAnm();
        }
        child++;
    }

    return true;
}

anmTexSrt_c::~anmTexSrt_c() {
    anmTexSrt_c::remove();
}

int anmTexSrt_c::getType() {
    return 0x04;
}

void anmTexSrt_c::remove() {
    nw4r::g3d::AnmObjTexSrtOverride *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtOverride>(mpAnmObj);
    if (o != nullptr && mpChildren != 0) {
        int size = o->Size();
        for (int i = 0; i < size; i++) {
            mpChildren[i].remove();
        }
        mpChildren = nullptr;
    }
    banm_c::remove();
}

void anmTexSrt_c::setAnm(bmdl_c &mdl, nw4r::g3d::ResAnmTexSrt srt, s32 idx, playMode_e mode) {
    nw4r::g3d::AnmObjTexSrtOverride *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtOverride>(mpAnmObj);
    o->Detach(idx);
    mpChildren[idx].setAnm(mdl, srt, mode);

    nw4r::g3d::AnmObjTexSrtRes *res =
            nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtRes>(mpChildren[idx].getAnimObj());
    o->Attach(idx, res);
}

void anmTexSrt_c::play() {
    nw4r::g3d::AnmObjTexSrtOverride *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexSrtOverride>(mpAnmObj);
    int size = o->Size();
    for (int i = 0; i < size; i++) {
        play(i);
    }
}

void anmTexSrt_c::play(s32 idx) {
    if (mpChildren[idx].IsBound()) {
        mpChildren[idx].play();
    }
}

f32 anmTexSrt_c::getFrame(s32 idx) const {
    return mpChildren[idx].getFrame();
}

void anmTexSrt_c::setFrame(f32 f, s32 idx) {
    mpChildren[idx].setFrameOnly(f);
}

void anmTexSrt_c::setRate(f32 f, s32 idx) {
    mpChildren[idx].setRate(f);
}

bool anmTexSrt_c::isStop(s32 idx) const {
    return mpChildren[idx].isStop();
}

bool anmTexSrt_c::checkFrame(f32 f, s32 idx) const {
    return mpChildren[idx].checkFrame(f);
}

void anmTexSrt_c::setPlayMode(playMode_e mode, s32 idx) {
    mpChildren[idx].setPlayState(mode);
}

f32 anmTexSrt_c::getFrameMax(s32 idx) const {
    return mpChildren[idx].getEndFrame();
}

void anmTexSrt_c::setFrameStart(f32 f, s32 idx) {
    mpChildren[idx].setStartFrame(f);
}

f32 anmTexSrt_c::getFrameStart(s32 idx) const {
    return mpChildren[idx].getStartFrame();
}

} // namespace m3d
