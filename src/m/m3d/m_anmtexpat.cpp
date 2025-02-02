#include "m/m3d/m_anmtexpat.h"

#include "egg/core/eggFrmHeap.h"
#include "m/m3d/m3d.h"
#include "m/m_heap.h"

namespace m3d {

int anmTexPat_c::child_c::getType() const {
    return nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_TEXPAT;
}

anmTexPat_c::child_c::~child_c() {}

u32 anmTexPat_c::child_c::heapCost(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexPat pat, bool b) {
    u32 size = 0;
    nw4r::g3d::AnmObjTexPatRes::Construct(nullptr, &size, pat, mdl, false);
    if (b) {
        size = ROUND_UP(mHeap::frmHeapCost(size, 0x20), 0x20);
    }
    return size;
}

bool anmTexPat_c::child_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexPat pat, mAllocator_c *alloc, u32 *pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size;
    if (pSize == nullptr) {
        pSize = &size;
    }

    *pSize = child_c::heapCost(mdl, pat, false);
    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjTexPatRes::Construct(&mAllocator, nullptr, pat, mdl, false);

    if (!mpAnmObj->Bind(mdl)) {
        remove();
        return false;
    }
    setFrmCtrlDefault(pat, PLAY_MODE_4);
    return true;
}

void anmTexPat_c::child_c::setAnm(m3d::bmdl_c &mdl, nw4r::g3d::ResAnmTexPat pat, m3d::playMode_e mode) {
    releaseAnm();
    mpAnmObj = nw4r::g3d::AnmObjTexPatRes::Construct(&mAllocator, nullptr, pat, mdl.getResMdl(), false);
    mpAnmObj->Bind(mdl.getResMdl());
    setFrmCtrlDefault(pat, mode);
}

void anmTexPat_c::child_c::releaseAnm() {
    if (mpAnmObj != nullptr) {
        mpAnmObj->Release();
        mpFrameHeap->free(3);
        mpAnmObj = nullptr;
    }
}

void anmTexPat_c::child_c::setFrmCtrlDefault(nw4r::g3d::ResAnmTexPat &pat, playMode_e mode) {
    if (mode == PLAY_MODE_4) {
        mode = pat.GetAnmPolicy() == nw4r::g3d::ANM_POLICY_ONETIME ? PLAY_MODE_1 : PLAY_MODE_0;
    }
    set(pat.GetNumFrame(), mode, 1.0f, -1.0f);
}

u32 anmTexPat_c::heapCost(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexPat pat, s32 num, bool b) {
    u32 size = 0;
    nw4r::g3d::AnmObjTexPatOverride::Construct(nullptr, &size, mdl, num);
    size += ROUND_UP(num * sizeof(child_c), 0x20);
    u32 childCost = child_c::heapCost(mdl, pat, true);
    size += num * ROUND_UP(childCost, 0x20);

    if (b) {
        size = ROUND_UP(mHeap::frmHeapCost(size, 0x20), 0x20);
    }
    return size;
}

bool anmTexPat_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmTexPat pat, mAllocator_c *alloc, u32 *pSize, s32 num) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size = 0;
    if (pSize == nullptr) {
        pSize = &size;
    }

    *pSize = heapCost(mdl, pat, num, false);
    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjTexPatOverride::Construct(&mAllocator, nullptr, mdl, num);

    // TODO inline?
    mpChildren = (child_c *)MEMAllocFromAllocator(&mAllocator, ROUND_UP(num * sizeof(child_c), 0x20));

    nw4r::g3d::AnmObjTexPatOverride *obj = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexPatOverride>(mpAnmObj);
    child_c *child = mpChildren;
    for (int i = 0; i < num; i++) {
        new (child) child_c();
        if (!child->create(mdl, pat, &mAllocator, nullptr)) {
            mHeap::destroyFrmHeap(mpFrameHeap);
            return false;
        }

        if (i == 0) {
            obj->Attach(i, nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexPatRes>(child->getAnimObj()));
        } else {
            child->releaseAnm();
        }
        child++;
    }

    return true;
}

anmTexPat_c::~anmTexPat_c() {
    anmTexPat_c::remove();
}

int anmTexPat_c::getType() const {
    return nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_TEXPAT;
}

void anmTexPat_c::remove() {
    nw4r::g3d::AnmObjTexPatOverride *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexPatOverride>(mpAnmObj);
    if (o != nullptr && mpChildren != 0) {
        int size = o->Size();
        for (int i = 0; i < size; i++) {
            mpChildren[i].remove();
        }
        mpChildren = nullptr;
    }
    banm_c::remove();
}

void anmTexPat_c::setAnm(bmdl_c &mdl, nw4r::g3d::ResAnmTexPat pat, s32 idx, playMode_e mode) {
    nw4r::g3d::AnmObjTexPatOverride *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexPatOverride>(mpAnmObj);
    o->Detach(idx);
    mpChildren[idx].setAnm(mdl, pat, mode);

    nw4r::g3d::AnmObjTexPatRes *res =
        nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexPatRes>(mpChildren[idx].getAnimObj());
    o->Attach(idx, res);
}

void anmTexPat_c::play() {
    nw4r::g3d::AnmObjTexPatOverride *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjTexPatOverride>(mpAnmObj);
    int size = o->Size();
    for (int i = 0; i < size; i++) {
        play(i);
    }
}

void anmTexPat_c::play(s32 idx) {
    if (mpChildren[idx].IsBound()) {
        mpChildren[idx].play();
    }
}

f32 anmTexPat_c::getFrame(s32 idx) const {
    return mpChildren[idx].getFrame();
}

void anmTexPat_c::setFrame(f32 f, s32 idx) {
    mpChildren[idx].setFrameOnly(f);
}

void anmTexPat_c::setRate(f32 f, s32 idx) {
    mpChildren[idx].setRate(f);
}

bool anmTexPat_c::isStop(s32 idx) const {
    return mpChildren[idx].isStop();
}

f32 anmTexPat_c::getFrameMax(s32 idx) const {
    return mpChildren[idx].getEndFrame();
}

void anmTexPat_c::setFrameStart(f32 f, s32 idx) {
    mpChildren[idx].setStartFrame(f);
}

} // namespace m3d
