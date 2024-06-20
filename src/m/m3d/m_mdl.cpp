#include <m/m3d/m3d.h>
#include <m/m3d/m_mdl.h>
#include <nw4r/g3d/g3d_scnmdlsmpl.h>

namespace m3d {

mdl_c::mdlCallback_c::mdlCallback_c() {
    mNumNode = 0;
    mpNodes = nullptr;
    mpBaseCallback = 0;
    mpAlloc = nullptr;
}
mdl_c::mdlCallback_c::~mdlCallback_c() {}

bool mdl_c::mdlCallback_c::create(nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 *pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }
    0.0f;

    u32 size = 0;
    if (pSize == nullptr) {
        pSize = &size;
    }

    mNumNode = mdl.GetResNodeNumEntries();
    size_t bufSize = mNumNode * sizeof(UnkNode);
    mpNodes = (UnkNode*)MEMAllocFromAllocator(alloc, bufSize);
    if (mpNodes == nullptr) {
        return false;
    }

    *pSize = ROUND_UP(bufSize + ROUND_UP(*pSize, 0x04), 0x04);
    UnkNode *node = mpNodes;
    for (int i = 0; i < mNumNode; i++) {
        node->field_0x04 = 1.0f;
        node->field_0x08 = 1.0f;
        node->field_0x0C = 1.0f;
        PSMTXIdentity(node->mtx.m);
        node++;
    }

    mpAlloc = alloc;
    return true;
}

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
        mpOwnedCallback = (mdlCallback_c *)alloc->alloc(sizeof(mdlCallback_c));
        if (mpOwnedCallback == nullptr) {
            remove();
            return false;
        }

        new (mpOwnedCallback) mdlCallback_c();
        mpCallback = mpOwnedCallback;
    } else {
        mpCallback = cb;
    }
    if (!mpCallback->create(mdl, alloc, pSize2)) {
        remove();
        return false;
    }

    if (pSize != nullptr) {
        *pSize = tmp1 + tmp2;
    }
    nw4r::g3d::ScnMdlSimple *sMdl;
    sMdl = nw4r::g3d::G3dObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mpScnLeaf);
    sMdl->SetCallback(mpCallback);
    sMdl->EnableScnObjCallbackTiming(nw4r::g3d::ScnObj::TIMING_ALL);
    setCallback(nullptr);
    return true;
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

void mdl_c::setCallback(callback_c *cb) {
    mpCallback->setBaseCallback(cb);
}

} // namespace m3d
