#include <egg/core/eggFrmHeap.h>
#include <m/m3d/m3d.h>
#include <m/m3d/m_anmmatclr.h>
#include <m/m_heap.h>

namespace m3d {

anmMatClr_c::child_c::~child_c() {}

int anmMatClr_c::child_c::getType() const {
    return nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_MATCLR;
}

u32 anmMatClr_c::child_c::heapCost(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmClr clr, bool b) {
    u32 size = 0;
    nw4r::g3d::AnmObjMatClrRes::Construct(nullptr, &size, clr, mdl, false);
    if (b) {
        size = ROUND_UP(mHeap::frmHeapCost(size, 0x20), 0x20);
    }
    return size;
}

bool anmMatClr_c::child_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmClr clr, mAllocator_c *alloc, u32 *pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size;
    if (pSize == nullptr) {
        pSize = &size;
    }

    *pSize = child_c::heapCost(mdl, clr, false);
    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjMatClrRes::Construct(&mAllocator, nullptr, clr, mdl, false);

    if (!mpAnmObj->Bind(mdl)) {
        remove();
        return false;
    }
    setFrmCtrlDefault(clr, PLAY_MODE_4);
    return true;
}

void anmMatClr_c::child_c::setAnm(m3d::bmdl_c &mdl, nw4r::g3d::ResAnmClr clr, m3d::playMode_e mode) {
    releaseAnm();
    mpAnmObj = nw4r::g3d::AnmObjMatClrRes::Construct(&mAllocator, nullptr, clr, mdl.getResMdl(), false);
    mpAnmObj->Bind(mdl.getResMdl());
    setFrmCtrlDefault(clr, mode);
}

void anmMatClr_c::child_c::releaseAnm() {
    if (mpAnmObj != nullptr) {
        mpAnmObj->Release();
        mpFrameHeap->free(3);
        mpAnmObj = nullptr;
    }
}

void anmMatClr_c::child_c::setFrmCtrlDefault(nw4r::g3d::ResAnmClr &clr, playMode_e mode) {
    if (mode == PLAY_MODE_4) {
        mode = clr.GetAnmPolicy() == nw4r::g3d::ANM_POLICY_ONETIME ? PLAY_MODE_1 : PLAY_MODE_0;
    }
    set(clr.GetNumFrame(), mode, 1.0f, -1.0f);
}

u32 anmMatClr_c::heapCost(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmClr clr, s32 num, bool b) {
    u32 size = 0;
    nw4r::g3d::AnmObjMatClrOverride::Construct(nullptr, &size, mdl, num);
    size += ROUND_UP(num * sizeof(child_c), 0x20);
    u32 childCost = child_c::heapCost(mdl, clr, true);
    size += num * ROUND_UP(childCost, 0x20);

    if (b) {
        size = ROUND_UP(mHeap::frmHeapCost(size, 0x20), 0x20);
    }
    return size;
}

bool anmMatClr_c::create(nw4r::g3d::ResMdl mdl, nw4r::g3d::ResAnmClr clr, mAllocator_c *alloc, u32 *pSize, s32 num) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size = 0;
    if (pSize == nullptr) {
        pSize = &size;
    }

    *pSize = heapCost(mdl, clr, num, false);
    if (!createAllocator(alloc, pSize)) {
        return false;
    }

    mpAnmObj = nw4r::g3d::AnmObjMatClrOverride::Construct(&mAllocator, nullptr, mdl, num);

    // TODO inline?
    mpChildren = (child_c *)MEMAllocFromAllocator(&mAllocator, ROUND_UP(num * sizeof(child_c), 0x20));

    nw4r::g3d::AnmObjMatClrOverride *obj = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjMatClrOverride>(mpAnmObj);
    child_c *child = mpChildren;
    for (int i = 0; i < num; i++) {
        new (child) child_c();
        if (!child->create(mdl, clr, &mAllocator, nullptr)) {
            mHeap::destroyFrmHeap(mpFrameHeap);
            return false;
        }

        if (i == 0) {
            obj->Attach(i, nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjMatClrRes>(child->getAnimObj()));
        } else {
            child->releaseAnm();
        }
        child++;
    }

    return true;
}

anmMatClr_c::~anmMatClr_c() {
    anmMatClr_c::remove();
}

int anmMatClr_c::getType() const {
    return nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_MATCLR;
}

void anmMatClr_c::remove() {
    nw4r::g3d::AnmObjMatClrOverride *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjMatClrOverride>(mpAnmObj);
    if (o != nullptr && mpChildren != 0) {
        int size = o->Size();
        for (int i = 0; i < size; i++) {
            mpChildren[i].remove();
        }
        mpChildren = nullptr;
    }
    banm_c::remove();
}

void anmMatClr_c::setAnm(bmdl_c &mdl, nw4r::g3d::ResAnmClr clr, s32 idx, playMode_e mode) {
    nw4r::g3d::AnmObjMatClrOverride *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjMatClrOverride>(mpAnmObj);
    o->Detach(idx);
    mpChildren[idx].setAnm(mdl, clr, mode);

    nw4r::g3d::AnmObjMatClrRes *res =
            nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjMatClrRes>(mpChildren[idx].getAnimObj());
    o->Attach(idx, res);
}

void anmMatClr_c::play() {
    nw4r::g3d::AnmObjMatClrOverride *o = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::AnmObjMatClrOverride>(mpAnmObj);
    int size = o->Size();
    for (int i = 0; i < size; i++) {
        play(i);
    }
}

void anmMatClr_c::play(s32 idx) {
    if (mpChildren[idx].IsBound()) {
        mpChildren[idx].play();
    }
}

f32 anmMatClr_c::getFrame(s32 idx) const {
    return mpChildren[idx].getFrame();
}

void anmMatClr_c::setFrame(f32 f, s32 idx) {
    mpChildren[idx].setFrameOnly(f);
}

f32 anmMatClr_c::getRate(s32 idx) const {
    return mpChildren[idx].getRate();
}

void anmMatClr_c::setRate(f32 f, s32 idx) {
    mpChildren[idx].setRate(f);
}

bool anmMatClr_c::isStop(s32 idx) const {
    return mpChildren[idx].isStop();
}

bool anmMatClr_c::checkFrame(f32 f, s32 idx) const {
    return mpChildren[idx].checkFrame(f);
}

void anmMatClr_c::setPlayMode(playMode_e mode, s32 idx) {
    mpChildren[idx].setPlayState(mode);
}

f32 anmMatClr_c::getFrameMax(s32 idx) const {
    return mpChildren[idx].getEndFrame();
}

f32 anmMatClr_c::getFrameStart(s32 idx) const {
    return mpChildren[idx].getStartFrame();
}

} // namespace m3d
