#include "m/m3d/m_mdl.h"

#include "m/m3d/m3d.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"
#include "nw4r/g3d/g3d_scnobj.h"

namespace m3d {

mdl_c::mdlCallback_c::mdlCallback_c() {
    mNumNode = 0;
    mpNodes = nullptr;
    mpBaseCallback = 0;
    mpAlloc = nullptr;
}
mdl_c::mdlCallback_c::~mdlCallback_c() {}

void mdl_c::mdlCallback_c::ExecCallbackA(
    nw4r::g3d::ChrAnmResult *result, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *o
) {
    u16 nodeId = o->GetCallbackNodeID();
    nw4r::g3d::ChrAnmResult *resPtr = &mpNodes[nodeId];
    if (mCalcRatio.is0x18() && !mCalcRatio.isEnd()) {
        if (!mCalcRatio.is0x19()) {
            *result = *resPtr;
        } else {
            u32 flags = result->flags;
            f32 f2 = mCalcRatio.get0x10();
            f32 f1 = mCalcRatio.get0x14();

            // TODO clean up this code, what does it even do, why do operators
            // break
            if ((flags & 8) == 0) {
                result->s.x = (result->s.x * f1 + resPtr->s.x * f2);
                result->s.y = (result->s.y * f1 + resPtr->s.y * f2);
                result->s.z = (result->s.z * f1 + resPtr->s.z * f2);
            } else {
                result->s.x = (f1 + resPtr->s.x * f2);
                result->s.y = (f1 + resPtr->s.y * f2);
                result->s.z = (f1 + resPtr->s.z * f2);
            }

            nw4r::math::QUAT q1;
            nw4r::math::QUAT q2;

            C_QUATMtx(q1, resPtr->rt);
            if ((flags & 0x20) == 0) {
                C_QUATMtx(q2, result->rt);
            } else {
                q2.x = 0.0f;
                q2.y = 0.0f;
                q2.z = 0.0f;
                q2.w = 1.0f;
            }

            C_QUATSlerp(q1, q2, q1, f1);
            nw4r::math::VEC3 tmp;
            tmp.x = result->rt._03;
            tmp.y = result->rt._13;
            tmp.z = result->rt._23;
            PSMTXQuat(result->rt, q1);
            result->rt._03 = tmp.x;
            result->rt._13 = tmp.y;
            result->rt._23 = tmp.z;
            if ((flags & 0x40) == 0) {
                result->rt._03 = tmp.x * f1;
                result->rt._13 = tmp.y * f1;
                result->rt._23 = tmp.z * f1;
            }
            result->rt._03 += resPtr->rt._03 * f2;
            result->rt._13 += resPtr->rt._13 * f2;
            result->rt._23 += resPtr->rt._23 * f2;
            result->flags = result->flags & ~(0x80000000 | 0x00000040 | 0x00000020 | 0x00000008);
            *resPtr = *result;
        }
    } else {
        *resPtr = *result;
    }

    if (mpBaseCallback != nullptr) {
        mpBaseCallback->timingA(nodeId, result, mdl);
    }
}

void mdl_c::mdlCallback_c::ExecCallbackB(
    nw4r::g3d::WorldMtxManip *m, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *o
) {
    u16 nodeId = o->GetCallbackNodeID();
    if (mpBaseCallback != nullptr) {
        mpBaseCallback->timingB(nodeId, m, mdl);
    }
    o->SetCallbackNodeID((nodeId + 1) % mdl.GetResNodeNumEntries());
}

void mdl_c::mdlCallback_c::ExecCallbackC(nw4r::math::MTX34 *mat, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *) {
    if (mpBaseCallback != nullptr) {
        mpBaseCallback->timingC(mat, mdl);
    }
    mCalcRatio.offUpdate();
}

bool mdl_c::mdlCallback_c::create(nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 *pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 size = 0;
    if (pSize == nullptr) {
        pSize = &size;
    }

    mNumNode = mdl.GetResNodeNumEntries();
    size_t bufSize = mNumNode * sizeof(nw4r::g3d::ChrAnmResult);
    mpNodes = (nw4r::g3d::ChrAnmResult *)MEMAllocFromAllocator(alloc, bufSize);
    if (mpNodes == nullptr) {
        return false;
    }

    *pSize = ROUND_UP(bufSize + ROUND_UP(*pSize, 0x04), 0x04);
    nw4r::g3d::ChrAnmResult *node = mpNodes;
    for (int i = 0; i < mNumNode; i++) {
        node->s.x = 1.0f;
        node->s.y = 1.0f;
        node->s.z = 1.0f;
        PSMTXIdentity(node->rt);
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

bool mdl_c::create(
    nw4r::g3d::ResMdl mdl, mdl_c::mdlCallback_c *cb, mAllocator_c *alloc, u32 bufferOption, int nView, u32 *pSize
) {
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

    if (!smdl_c::create(mdl, alloc, bufferOption, nView, pSize1)) {
        return false;
    }

    if (cb == nullptr) {
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
    sMdl->SetScnMdlCallback(mpCallback);
    sMdl->EnableScnMdlCallbackTiming(nw4r::g3d::ScnObj::Timing(0x7));
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

bool mdl_c::setAnm(m3d::banm_c &anm) {
    return setAnm(anm, 0.0f);
}

void mdl_c::play() {
    bmdl_c::play();
    mpCallback->calcBlend();
}

bool mdl_c::setAnm(m3d::banm_c &anm, f32 f) {
    if (anm.getType() == nw4r::g3d::ScnMdlSimple::ANMOBJTYPE_CHR) {
        mpCallback->setBlendFrame(f);
    }
    return bmdl_c::setAnm(anm);
}

void mdl_c::setCallback(callback_c *cb) {
    mpCallback->setBaseCallback(cb);
}

callback_c::~callback_c() {}
void callback_c::timingA(u32, nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl) {}
void callback_c::timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) {}
void callback_c::timingC(nw4r::math::MTX34 *, nw4r::g3d::ResMdl) {}

} // namespace m3d
