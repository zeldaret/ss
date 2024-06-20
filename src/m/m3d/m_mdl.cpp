#include <m/m3d/m3d.h>
#include <m/m3d/m_mdl.h>
#include <nw4r/g3d/g3d_scnmdlsmpl.h>

namespace m3d {

mdl_c::mdlCallback_c::mdlCallback_c() {
    mNumNode = 0;
    mpNodes = nullptr;
    mCallbackNum = 0;
    mpAlloc = nullptr;
}
mdl_c::mdlCallback_c::~mdlCallback_c() {}

bool mdl_c::mdlCallback_c::create(nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 *pSize) {}

void mdl_c::mdlCallback_c::remove() {
    mCalcRatio.remove();
    if (mpNodes != nullptr) {
        // Probably an m_allocator inline
        MEMFreeToAllocator(mpAlloc, mpNodes);
    }
    mpNodes = nullptr;
    mpAlloc = nullptr;
}

void mdl_c::mdlCallback_c::setBlendFrame(f32 frame) {
    mCalcRatio.set(frame);
}
void mdl_c::mdlCallback_c::calcBlend() {
    if (!mCalcRatio.isEnd()) {
        mCalcRatio.calc();
    }
}

mdl_c::mdl_c() : mpOwnedCallback(nullptr), mpCallback(nullptr) {}
mdl_c::~mdl_c() {
    remove();
}

bool mdl_c::create(nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption, int nView, u32 *pSize) {
    return create(mdl, nullptr, alloc, bufferOption, nView, pSize);
}

bool mdl_c::create(nw4r::g3d::ResMdl mdl, mdl_c::mdlCallback_c *cb, mAllocator_c *alloc, u32 bufferOption, int nView,
        u32 *pSize) {
    nw4r::g3d::ScnMdlSimple *sMdl;
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 tmp1;
    u32 tmp2;
    u32 *pSize1 = nullptr;
    u32 *pSize2 = nullptr;
    if (pSize != nullptr) {
        pSize1 = &tmp1;
        pSize2 = &tmp2;
    }

    bool success;

    if (!smdl_c::create(mdl, alloc, bufferOption, nView, pSize1)) {
        return false;
    }
    if (cb == nullptr) {
        // TODO sizeof
        mpOwnedCallback = (mdlCallback_c *)alloc->alloc(0x30);
        if (mpOwnedCallback == nullptr) {
            remove();
            return false;
        }

        new (mpOwnedCallback) mdlCallback_c();
        mpCallback = (mdlCallback_c *)mpOwnedCallback;
    } else {
        mpCallback = cb;
    }
    if (!mpCallback->create(mdl, alloc, pSize2)) {
        remove();
        return false;
    } else {
        if (pSize != nullptr) {
            *pSize = tmp1 + tmp2;
        }

        sMdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf);
        sMdl->SetCallback(mpCallback);
        sMdl->EnableScnObjCallbackTiming(nw4r::g3d::ScnObj::TIMING_ALL);
        setCallback(nullptr);
        return true;
    }
}

void mdl_c::setCallback(mdlCallback_c *cb) {
    mpCallback = cb;
}

void mdl_c::remove() {
    if (mpOwnedCallback != nullptr) {
        mAllocator_c *alloc = mpOwnedCallback->getAllocator();
        mpOwnedCallback->remove();
        mpOwnedCallback->~mdlCallback_c();
        alloc->free(mpOwnedCallback);
        mpOwnedCallback = nullptr;
        mpCallback = nullptr;
    }
    bmdl_c::remove();
}

void mdl_c::setAnm(m3d::banm_c &anm) {
    setAnm(anm, 0.0f);
}

void mdl_c::play() {
    bmdl_c::play();
    mpCallback->calcBlend();
}

void mdl_c::setAnm(m3d::banm_c &anm, f32 f) {
    if (anm.getType() == 0) {
        mpCallback->setBlendFrame(f);
    }
    bmdl_c::setAnm(anm);
}

} // namespace m3d
