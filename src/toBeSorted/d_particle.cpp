#include "toBeSorted/d_particle.h"

#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAEmitterManager.h"
#include "JSystem/JParticle/JPAResourceManager.h"
#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggTexture.h"
#include "m/m_angle.h"
#include "m/m_heap.h"
#include "m/m_vec.h"

extern "C" const u16 lbl_8057AB72;
extern "C" const u16 lbl_8057AB74;

static u32 extractResMgrIdx(u16 particleResId) {
    return (particleResId & lbl_8057AB72) != 0 ? 1 : 0;
}

static u16 extractResUserIdx(u16 particleResId) {
    return (particleResId & ~(lbl_8057AB72 | lbl_8057AB74));
}

namespace dParticle {

JPAResourceManager *res_c::init(const void *jpc, const void *jpn, EGG::Heap *heap) {
    mpHeap = mHeap::createFrmHeap(-1, heap, "dParticle::res_c", 0x20, 0);
    if (mpHeap == nullptr) {
        return nullptr;
    }
    mpResourceManager = new (mpHeap, 4) JPAResourceManager(jpc, mpHeap);
    if (mpResourceManager != nullptr) {
        mpJpnData = static_cast<const JPN *>(jpn);
    }
    mHeap::adjustFrmHeap(mpHeap);
    return mpResourceManager;
}

void res_c::dispose() {
    if (mpHeap != nullptr) {
        if (mpResourceManager != nullptr) {
            mpResourceManager = nullptr;
            mpJpnData = nullptr;
        }
        mpHeap->destroy();
        mpHeap = nullptr;
    }
}

void tex_c::set(EGG::CpuTexture *tex) {
    mImg.format = tex->getFormat();
    mImg.alphaEnabled = true;
    mImg.width = tex->getWidth();
    mImg.height = tex->getHeight();
    mImg.wrapS = tex->getWrapS();
    mImg.wrapT = tex->getWrapT();
    mImg.indexTexture = 0;
    mImg.colorFormat = GX_TL_IA8;
    mImg.numColors = 0;
    mImg.paletteOffset = 0;
    mImg.mipmapEnabled = 0;
    mImg.doEdgeLOD = 0;
    mImg.biasClamp = 0;
    mImg.maxAnisotropy = 0;
    mImg.minFilter = tex->getMinFilt();
    mImg.magFilter = tex->getMagFilt();
    mImg.minLOD = 0;
    mImg.maxLOD = 0;
    mImg.mipmapCount = 1;
    mImg.LODBias = 0;
    mImg.imageOffset = (u32)tex->getBuffer() - (u32)&mImg;

    for (int i = 0; i < D_PARTICLE_NUM_RES; i++) {
        JPAResourceManager *mgr = dParticle::mgr_c::GetInstance()->getResourceManager(i);
        if (mgr != nullptr) {
            mgr->swapTexture(&mImg, "dummy");
        }
    }
}

mgr_c *mgr_c::sInstance;

mgr_c::mgr_c(EGG::Heap *heap, u32 ptclMax, u32 emtrMax, u8 grpMax) : mpEmitterMgr(nullptr) {
    sInstance = this;
    mpEmitterMgr = new (heap, 4) JPAEmitterManager(ptclMax, emtrMax, heap, grpMax, D_PARTICLE_NUM_RES);
}

void mgr_c::createResource(EGG::Heap *heap, s32 mgrIndex, void *jpc, void *jpn) {
    JPAResourceManager *mgr = mRes[mgrIndex].init(jpc, jpn, heap);
    if (mgr != nullptr) {
        mpEmitterMgr->entryResourceManager(mgr, mgrIndex);
    }
}

void mgr_c::disposeResource(s32 mgrIndex) {
    res_c &res = mRes[mgrIndex];
    if (res.getResourceManager() != nullptr) {
        mpEmitterMgr->clearResourceManager(mgrIndex);
    }
    res.dispose();
}

s32 mgr_c::getResUserWork(u16 particleResId) const {
    u8 idx = extractResMgrIdx(particleResId);
    JPAResourceManager *mgr = mpEmitterMgr->getResourceManager(idx);
    if (mgr == nullptr) {
        return -1;
    }
    return mgr->getResUserWork(extractResUserIdx(particleResId));
}

u16 mgr_c::getJpnData(u16 particleResId) const {
    const JPN *jpn = mRes[extractResMgrIdx(particleResId)].getJpn();
    u16 idx = extractResUserIdx(particleResId);
    if (idx >= jpn->numJpn) {
        return 0xFFFF;
    } else {
        return jpn->mData[idx];
    }
}

void mgr_c::doCustomSkywardSwordThing(u8 groupId, f32 x, f32 y) {
    mpEmitterMgr->fn_80320E20(groupId, x, y);
}

void mgr_c::calc(u8 groupId) {
    mpEmitterMgr->calc(groupId);
}

void mgr_c::draw(const JPADrawInfo *drawInfo, u8 groupId) {
    mpEmitterMgr->draw(drawInfo, groupId);
}

JPABaseEmitter *
mgr_c::createEmitter(u16 particleResId, u8 groupId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale) {
    u32 resMgrId = extractResMgrIdx(particleResId);
    u16 resId = extractResUserIdx(particleResId);
    JPABaseEmitter *emitter = mpEmitterMgr->createSimpleEmitterID(pos, resId, groupId, resMgrId, nullptr, nullptr);
    if (emitter == nullptr) {
        return nullptr;
    }
    if (rot != nullptr) {
        emitter->setGlobalRotation(*rot);
    }
    if (scale != nullptr) {
        emitter->setGlobalScale(*scale);
    }
    return emitter;
}

void mgr_c::forceDeleteAllEmitter() {
    mpEmitterMgr->forceDeleteAllEmitter();
}

void mgr_c::forceDeleteEmitter(JPABaseEmitter *emitter) {
    mpEmitterMgr->forceDeleteEmitter(emitter);
}

JPAResourceManager *mgr_c::getResourceManager(s32 idx) {
    return mRes[idx].getResourceManager();
}

mgr_c *mgr_c::create(EGG::Heap *heap, u32 ptclMax, u32 emtrMax, u8 grpMax) {
    return new (heap, 4) mgr_c(heap, ptclMax, emtrMax, grpMax);
}

} // namespace dParticle
