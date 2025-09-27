// clang-format off
// vtable order
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "d/d_gfx.h"
#include "m/m_heap.h"
#include "rvl/GX/GXTypes.h"
#include "toBeSorted/d_d3d.h"
// clang-format on

#include "toBeSorted/d_emitter.h"

#include "JSystem/JParticle/JPADrawInfo.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_base.h"
#include "d/d_heap.h"
#include "d/d_light_env.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "egg/core/eggHeap.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m3d.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_camera.h"
#include "nw4r/g3d/g3d_state.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_arithmetic.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_particle.h"
#include "toBeSorted/event_manager.h"

#include "rvl/GX.h"
#include "rvl/MTX.h"

void float_ordering_1(s32 a) {
    (f32) a;
}

void float_ordering_2() {
    255.0f;
}

void float_ordering_3(u32 a) {
    (f32) a;
}

void float_ordering_4() {
    0.0f;
}

dJEffManager_c::EffectsList dJEffManager_c::sPlayingEffectsList;
CommonEmitterCallback dJEffManager_c::sCommonEmitterCallbacks[2];
dShpEmitter_c dJEffManager_c::sShpEmitters[47];
dEmitterBase_c dJEffManager_c::sEmitter;
dParticleFogProc_c dJEffManager_c::sFogProcs[12];
dEffect2D_c dJEffManager_c::s2DEffects[3];

// broken by -ipa file - probably TList nonsense?
dEmitterCallback_c::~dEmitterCallback_c() {
    for (EmitterCallbackList::Iterator it = mEmitterList.GetBeginIter(); it != mEmitterList.GetEndIter();) {
        EmitterCallbackList::Iterator itCopy = it;
        ++itCopy;
        it->setEmitterCallback(nullptr);
        it = itCopy;
    }
}

void dEmitterCallback_c::remove(dEmitterBase_c *emitter) {
    if (mEmitterList.GetPosition(emitter) == mEmitterList.GetEndIter()) {
        return;
    }
    mEmitterList.remove(emitter);
}

// broken by -ipa file - probably TList nonsense?
dParticleCallback_c::~dParticleCallback_c() {
    for (ParticleCallbackList::Iterator it = mEmitterList.GetBeginIter(); it != mEmitterList.GetEndIter();) {
        ParticleCallbackList::Iterator itCopy = it;
        ++itCopy;
        it->setParticleCallback(nullptr);
        it = itCopy;
    }
}

void dParticleCallback_c::remove(dEmitterBase_c *emitter) {
    if (mEmitterList.GetPosition(emitter) == mEmitterList.GetEndIter()) {
        return;
    }
    mEmitterList.remove(emitter);
}

dEmitterBase_c::~dEmitterBase_c() {
    deactivateEmitters();
    if (mpEmitterCallback != nullptr) {
        mpEmitterCallback->remove(this);
        mpEmitterCallback = nullptr;
    }

    if (mpParticleCallback != nullptr) {
        mpParticleCallback->remove(this);
        mpParticleCallback = nullptr;
    }
}

JPABaseEmitter *dEmitterBase_c::GetNextEmitter(JPABaseEmitter *head) {
    // Portability hazard: u32->pointer cast
    return reinterpret_cast<JPABaseEmitter *>(head->getUserWork());
}

bool dEmitterBase_c::createEmitters(
    u16 effectResourceId, const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
    const GXColor *c2, s32 idx1, s32 idx2
) {
    u16 iter = effectResourceId;
    JPABaseEmitter *head = nullptr;
    JPABaseEmitter *last;
    for (; iter != 0xFFFF; iter = dParticle::mgr_c::GetInstance()->getJpnData(iter)) {
        last = dParticle::mgr_c::GetInstance()->createEmitter(
            iter, dJEffManager_c::getGroupId(iter), position, rot, scale
        );
        if (last != nullptr) {
            if (head != nullptr) {
                // Portability hazard: pointer->u32 cast
                head->setUserWork(reinterpret_cast<u32>(last));
            } else {
                mpEmitterHead = last;
            }
            head = last;
            last->setEmitterCallBackPtr(mpEmitterCallback);
            last->setParticleCallBackPtr(mpParticleCallback);
            loadColors(last, c1, c2, idx1, idx2);
        }
    }

    if (mpEmitterHead != nullptr) {
        if (mpEmitterCallback != nullptr) {
            // why last and not head?
            mpEmitterCallback->create(last);
        }
        if (mpParticleCallback != nullptr) {
            // why last and not head?
            mpParticleCallback->create(last);
        }
    }
    return mpEmitterHead != nullptr;
}

void dEmitterBase_c::setEmitterCallback(dEmitterCallback_c *cb) {
    if (mpEmitterCallback != cb) {
        if (mpEmitterCallback != nullptr) {
            mpEmitterCallback->remove(this);
            for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
                emitter->setEmitterCallBackPtr(nullptr);
            }
        }
        mpEmitterCallback = cb;
        if (cb != nullptr) {
            cb->mEmitterList.append(this);
        }
    }
}

void dEmitterBase_c::setParticleCallback(dParticleCallback_c *cb) {
    if (mpParticleCallback != cb) {
        if (mpParticleCallback != nullptr) {
            mpParticleCallback->remove(this);
            for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
                emitter->setParticleCallBackPtr(nullptr);
            }
        }
        mpParticleCallback = cb;
        if (cb != nullptr) {
            cb->mEmitterList.append(this);
        }
    }
}

void dEmitterBase_c::setImmortal() {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->becomeImmortalEmitter();
    }
}

void dEmitterBase_c::deactivateEmitters() {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setEmitterCallBackPtr(nullptr);
        emitter->setParticleCallBackPtr(nullptr);
        emitter->quitImmortalEmitter();
        if (emitter->mMaxFrame == 0) {
            emitter->becomeInvalidEmitter();
        }
        emitter->playCalcEmitter();
    }
    mpEmitterHead = nullptr;
}

void dEmitterBase_c::stopCalcEmitters() {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->stopCalcEmitter();
    }
}

void dEmitterBase_c::playCalcEmitters() {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->playCalcEmitter();
    }
}

void dEmitterBase_c::stopDrawParticles() {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->stopDrawParticle();
    }
}

void dEmitterBase_c::playDrawParticles() {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->playDrawParticle();
    }
}

void dEmitterBase_c::setPosRotScale(const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setGlobalTranslation(position);
        if (rot != nullptr) {
            emitter->setGlobalRotation(*rot);
        }
        if (scale != nullptr) {
            emitter->setGlobalScale(*scale);
        }
    }
}

void dEmitterBase_c::setPosRotScaleWithScreenScale(const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale) {
    mVec3_c adjustedPosition(position.x * dGfx_c::getCurrentScreenTo4x3WidthScaleF(), position.y, position.z);
    setPosRotScale(adjustedPosition, rot, scale);
}

void dEmitterBase_c::setTransform(const mMtx_c &mtx) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setGlobalSRTMatrix(mtx);
    }
}

void dEmitterBase_c::loadColors(const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        loadColors(emitter, c1, c2, idx1, idx2);
    }
}

void dEmitterBase_c::setGlobalAlpha(u8 alpha) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setGlobalAlpha(alpha);
    }
}

void dEmitterBase_c::setRate(f32 rate) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setRate(rate);
    }
}

void dEmitterBase_c::setDynamicsScale(const mVec3_c &scale) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setGlobalDynamicsScale(scale);
    }
}

void dEmitterBase_c::setParticleScale(const mVec3_c &scale) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setGlobalParticleScale(scale);
    }
}

void dEmitterBase_c::setAwayFromCenterSpeed(f32 speed) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setAwayFromCenterSpeed(speed);
    }
}

void dEmitterBase_c::setVolumeSize(u16 size) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setVolumeSize(size);
    }
}

void dEmitterBase_c::setLifeTime(s16 lifetime) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setLifeTime(lifetime);
    }
}

void dEmitterBase_c::attachEmitterCallbackId(s32 id) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setEmitterCallBackPtr(&dJEffManager_c::sCommonEmitterCallbacks[id]);
    }
}

void dEmitterBase_c::bindShpEmitter(s32 id, bool unused) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setParticleCallBackPtr(&dJEffManager_c::sShpEmitters[id]);
        emitter->setGlobalAlpha(0);
    }
}

EffectsStruct::EffectsStruct() : mpOwner(nullptr), mFlags(0), mEffect(0) {}

EffectsStruct::EffectsStruct(dBase_c *base) : mpOwner(base), mFlags(0), mEffect(0) {}

EffectsStruct::~EffectsStruct() {
    mpOwner = nullptr;
    remove(false);
}

void EffectsStruct::remove(bool bForceDeleteEmitters) {
    offFlag(EMITTER_0x1);
    JPABaseEmitter *emitter = bForceDeleteEmitters ? mpEmitterHead : nullptr;
    deactivateEmitters();
    for (; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        dParticle::mgr_c::GetInstance()->forceDeleteEmitter(emitter);
    }
    removeFromActiveEmittersList();
}

void EffectsStruct::addToActiveEmittersList(u16 resourceId, bool bFlags) {
    mEffect = resourceId;
    mFlags = 0;
    setImmortal();
    dJEffManager_c::sPlayingEffectsList.append(this);
    onFlag(EMITTER_0x1);
    if (bFlags) {
        onFlag(EMITTER_0x4);
    }
    if (dParticle::mgr_c::GetInstance()->getResUserWork(resourceId) & 0x4000) {
        onFlag(EMITTER_0x10);
    }
}

bool EffectsStruct::areAllEmittersDone() {
    bool allDone = true;

    if (mpEmitterHead != nullptr) {
        if (checkFlag(EMITTER_Fading) && mFadeTimer != 0) {
            return false;
        }
        for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
            bool canDelete = emitter->isEnableDeleteEmitter();
            if (!canDelete) {
                if (emitter->mMaxFrame == 0) {
                    emitter->becomeInvalidEmitter();
                }
                allDone = false;
            }
        }
    }

    return allDone;
}

void EffectsStruct::execute() {
    if (mpOwner != nullptr && (mpOwner->mDeleteRequest || mpOwner->mLifecycleState == fBase_c::TO_BE_DELETED)) {
        mpOwner = nullptr;
    }
    if (dJEffManager_c::shouldBePaused(mpOwner)) {
        if (!checkFlag(EMITTER_0x10)) {
            stopCalcEmitters();
            if (mpOwner != nullptr && !mpOwner->checkBaseProperty(0x100)) {
                if (!mpEmitterHead->checkStatus(JPAEmtrStts_StopDraw)) {
                    onFlag(EMITTER_0x20);
                    stopDrawParticles();
                }
            } else {
                if (checkFlag(EMITTER_0x20)) {
                    offFlag(EMITTER_0x20);
                    playDrawParticles();
                }
            }
        }
        onFlag(EMITTER_0x1);
    } else {
        playCalcEmitters();
        if (checkFlag(EMITTER_0x20)) {
            offFlag(EMITTER_0x20);
            playDrawParticles();
        }
        if (checkFlag(EMITTER_Fading) && mFadeTimer != 0) {
            mFadeTimer--;
        }
    }
}

bool EffectsStruct::getOwnerPolyAttrs(s32 *pOut1, s32 *pOut2) {
    if (mpOwner != nullptr && mpOwner->mGroupType == fBase_c::ACTOR) {
        dAcBase_c *actor = static_cast<dAcBase_c *>(mpOwner);
        *pOut1 = actor->mPolyAttr0;
        *pOut2 = actor->mPolyAttr1;
        return true;
    } else {
        *pOut1 = 0;
        *pOut2 = 0;
        return false;
    }
}

void EffectsStruct::realizeAlpha() {
    setGlobalAlpha(mFadeTimer * (255.0f / mFadeDuration));
}

void EffectsStruct::setFading(u8 lifetime) {
    if (!checkFlag(EMITTER_Fading)) {
        mFadeTimer = lifetime;
        onFlag(EMITTER_Fading);
        mFadeDuration = lifetime;
    }
}

bool dParticleFogProc_c::create(u32 idx, s32 prioOpa, s32 prioXlu, mHeapAllocator_c *alloc) {
    if (!d3d::UnkProc::create(prioOpa, prioXlu, alloc)) {
        return false;
    }
    mIdx = idx;
    field_0x1C = idx == 0 || idx == 2 || idx == 4 || idx == 6 || idx == 10;
    return true;
}

void dParticleFogProc_c::doDraw() {
    nw4r::g3d::Camera c = m3d::getCurrentCamera();
    mMtx_c camMtx, texProjMtx;
    c.GetCameraMtx(camMtx);
    c.GetProjectionTexMtx(texProjMtx);
    JPADrawInfo info;
    MTXCopy(camMtx, info.mCamMtx);
    MTXCopy(texProjMtx, info.mPrjMtx);
    if (field_0x1C) {
        m3d::getFogMgr(0); // ignored, maybe an inline on FogMgr?
        nw4r::g3d::G3DState::LoadFog(0);
    } else {
        mColor c(0);
        GXSetFog(GX_FOG_NONE, c, 0.0f, 0.0f, 0.0f, 0.0f);
        GXSetFogRangeAdj(GX_FALSE, 0, nullptr);
    }
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    dJEffManager_c::draw(&info, mIdx);
}

extern "C" bool fn_80054AD0();
void dEffect2D_c::draw() {
    f32 proj[GX_PROJECTION_SZ];
    GXGetProjectionv(proj);
    f32 base = proj[1];
    proj[1] = base / dGfx_c::get16x9to4x3WidthScaleF();
    GXSetProjectionv(proj);
    f32 f = dGfx_c::getWidth16x9() / 2;
    f32 h = dGfx_c::getCurrentScreenHeightLimitF();
    mMtx_c mtx1, mtx2;
    JPADrawInfo info;

    C_MTXLightOrtho(mtx1, -h, h, -f, f, 0.5f, 0.5f, 0.5f, 0.5f);

    MTXScale(mtx2, fn_80054AD0() ? dGfx_c::get16x9to4x3WidthScaleF() : 1.0f, 1.0f, 1.0f);
    MTXCopy(mtx2, info.mCamMtx);
    MTXCopy(mtx1, info.mPrjMtx);
    dJEffManager_c::draw(&info, mGroupId);
    proj[1] = base;
    GXSetProjectionv(proj);
}

void dEffect2D_c::create(u32 groupId, u8 prio) {
    mGroupId = groupId;
    setPriority(prio);
}

bool dMassObjEmitterCallback_c::start(const mVec3_c &v1, dAcObjBase_c *owner) {
    if (field_0x658 >= 50) {
        return false;
    }

    field_0x010[field_0x658] = v1;
    field_0x4C0[field_0x658] = owner;

    field_0x658++;
    return true;
}

void dMassObjEmitterCallback_c::executeAfter(JPABaseEmitter *emitter) {
    s32 createNumber = emitter->getCurrentCreateNumber();
    if (createNumber > 0) {
        s32 i = 0;
        mVec3_c translate;
        emitter->getLocalTranslation(translate);
        if (field_0x654 != 0) {
            for (; i < field_0x654; i++) {
                mVec3_c newTranslate = mVec3_c(field_0x268[i] + translate);
                emitter->setGlobalTranslation(newTranslate);
                for (s32 j = 0; j < createNumber; j++) {
                    JPABaseParticle *p = emitter->createParticle();
                    if (p != nullptr) {
                        p->setOffsetPosition(newTranslate);
                    }
                }
            }
        }
    }
}

void dMassObjEmitterCallback_c::execute() {
    for (s32 i = field_0x654 - 1; i >= 0; i--) {
        if (field_0x588[i] != nullptr) {
            if (!dJEffManager_c::shouldBePaused(field_0x588[i]) || field_0x588[i]->mDeleteRequest ||
                field_0x588[i]->mLifecycleState == fBase_c::TO_BE_DELETED) {
                if (field_0x654 != 0 && i != field_0x654 - 1) {
                    field_0x588[i] = field_0x588[field_0x654 - 1];
                    field_0x268[i] = field_0x268[field_0x654 - 1];
                } else {
                    field_0x588[i] = nullptr;
                }
                field_0x654--;
            }
        } else {
            field_0x654--;
        }
    }
    if (field_0x658 + field_0x654 > 50) {
        field_0x658 = 50 - field_0x654;
    }

    for (u32 i = 0; i < field_0x658; i++) {
        field_0x588[field_0x654] = field_0x4C0[i];
        field_0x268[field_0x654] = field_0x010[i];
        field_0x654++;
    }
    field_0x658 = 0;
}

#pragma opt_unroll_loops off
void dMassObjEmitterCallback_c::clear() {
    // TODO: this unrolls the entire loops,
    // but the original code uses an unroll
    // factor of 10 (5 iterations). 2D arrays
    // work here but are probably fake.
    field_0x654 = 0;
    for (u32 i = 0; i < 50; i++) {
        field_0x588[i] = nullptr;
    }
    field_0x658 = 0;
    for (u32 i = 0; i < 50; i++) {
        field_0x4C0[i] = nullptr;
    }
}
#pragma opt_unroll_loops reset

void dMassObjEmitter_c::create(u16 resourceId) {
    setEmitterCallback(&mCallback);
    if (createEmitters(resourceId, mVec3_c::Zero, nullptr, nullptr, nullptr, nullptr, 0, 0)) {
        for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
            emitter->setStatus(JPAEmtrStts_Immortal);
            emitter->setStatus(JPAEmtrStts_StopEmit);
        }
        mCallback.clear();
    }
}

void dShpEmitterProc::doDraw() {
    mMtx_c viewMtx;
    getViewMtx(viewMtx);
    s32 ctrl = 0;
    mLightSetId = mAlpha[0];
    mMtx_c *pTransforms = mpTransforms;
    bool *pFlags = mpFlags;
    for (s32 idx = 0; idx < mCount;) {
        if (*pFlags == false) {
            mMtx_c mtx;
            mtx.copyFrom(*pTransforms);
            MTXConcat(viewMtx, mtx, mtx);
            mMtx_c mtx2;
            mtx2.copyFrom(mtx);
            mtx2.fn_802F1C40(0, 1);
            nw4r::math::MTX33 mtx3;
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    mtx3.m[a][b] = mtx2.m[a][b];
                }
            }
            if (mLightSetId != mAlpha[idx]) {
                mLightSetId = mAlpha[idx];
                setupLight();
            }
            mVec3_c v;
            mVec3_c zero(0.0f, 0.0f, 0.0f);
            MTXMultVec(*pTransforms, zero, v);
            dShpProcBase_c::draw(&v);
            nw4r::g3d::G3DState::SetViewPosNrmMtxArray(mtx, &mtx3, nullptr);
            nw4r::g3d::Draw1Mat1ShpDirectly(nw4r::g3d::ResMat(nullptr), mShp, mtx, mtx2, ctrl, nullptr, nullptr);
            if (ctrl == 0) {
                ctrl = 2;
            }
        }
        idx++;
        pTransforms++;
        pFlags++;
    }
    mpOwner->clear();
}

bool dShpEmitter_c::create(mHeapAllocator_c *alloc) {
    if (!mProc.create(nw4r::g3d::ResMat(nullptr), nw4r::g3d::ResShp(nullptr), 100, alloc, false, nullptr)) {
        return false;
    }
    mProc.setOwner(this);
    return true;
}

void dShpEmitter_c::init(const char *mdlName, const char *arcName, bool priority) {
    clear();
    void *ptr = OarcManager::GetInstance()->getSubEntryData(arcName, "g3d/model.brres");
    if (ptr == nullptr) {
        mProc.setResMat(nw4r::g3d::ResMat(nullptr));
        mProc.setResShp(nw4r::g3d::ResShp(nullptr));
        return;
    }

    nw4r::g3d::ResFile f(ptr);

    if (strequals(arcName, "F300BrokenRockWall")) {
        dStage_c::bindStageResToFile(&f);
    }

    nw4r::g3d::ResMdl mdl = f.GetResMdl(mdlName);
    nw4r::g3d::ResMat mat = mdl.GetResMat(0);
    mProc.setResMat(mat);
    mProc.setResShp(mdl.GetResShp(0));
    if (priority) {
        mProc.setPriorityDraw(0x1C, 4);
    }
}

void dShpEmitter_c::clear() {
    field_0xA0 = 0;
    mProc.setAllFlags();
}

void dShpEmitter_c::draw(JPABaseEmitter *emitter, JPABaseParticle *particle) {
    // TODO
    for (s32 i = 0; i < 100; i++) {
        if (mProc.isDrawDisabled(i)) {
            mProc.setDrawDisabled(i, false);
            mVec3_c pos;
            mVec3_c velocity;
            particle->getGlobalPosition(pos);
            particle->getVelVec(velocity);
            mVec3_c v;
            if (cM::isZero(velocity.squareMagXZ())) {
                v = mVec3_c::Ex;
            } else {
                v = mVec3_c::Ey.cross(velocity);
                v.normalize();
            }
            mMtx_c m;
            m.setAxisRotation(v, mAng((u16)particle->mRotateAngle).radian2());
            m.setBase(3, pos);
            mMtx_c mtx2;
            mVec3_c scale(
                particle->getParticleScaleX() * emitter->getGlobalParticleScaleX(),
                particle->getParticleScaleY() * emitter->getGlobalParticleScaleY(),
                particle->getParticleScaleX() * emitter->getGlobalParticleScaleX()
            );
            MTXScale(mtx2, scale.x, scale.y, scale.z);
            MTXConcat(m, mtx2, m);
            mProc.setTransform(i, m);
            mProc.setAlpha(i, emitter->getGlobalAlpha());
            field_0xA0++;
            break;
        }
    }
    // Hide the particle - we're doing all the drawing ourselves
    particle->setInvisibleParticleFlag();
}

void CommonEmitterCallback::draw(JPABaseEmitter *emitter) {
    emitter->loadTexture(field_0x04, GX_TEXMAP0);
}

void dMassObjEmitter_c::remove() {
    setEmitterCallback(nullptr);
    mpEmitterHead = nullptr;
}

extern "C" u32 sNumMassObjEmitters; // should be const?
extern "C" dMassObjEmitter_c *CURRENT_EFFECT_MANAGER_INIT;

const u32 dJEffManager_c::moreInts[] = {3, 5, 7, 8, 12, 13, 14, 1, 9, 11};

s32 dJEffManager_c::getGroupId(u16 resourceId) {
    u32 mask = dParticle::mgr_c::GetInstance()->getResUserWork(resourceId);
    s32 bit = 1;
    for (int i = 0; i < 10; i++) {
        if ((mask & (1 << i)) != 0) {
            bit = i;
            break;
        }
    }

    int i = moreInts[bit];
    // TODO explain this
    if ((mask & 0x8000) != 0) {
        if (i == 3) {
            i = 2;
        } else if (i == 5) {
            i = 4;
        } else if (i == 7) {
            i = 6;
        } else if (i == 1) {
            i = 0;
        } else if (i == 11) {
            i = 10;
        }
    }

    return i;
}

// various grasses being cut
extern const u16 sEffectResourceIds[];

void dJEffManager_c::setupEffects() {
    for (s32 idx = 0; idx < sNumMassObjEmitters; idx++) {
        sMassObjEmitters[idx].create(sEffectResourceIds[idx]);
    }

    sShpEmitters[TsuboA].init("FX_TsuboA", "Tubo", false);
    sShpEmitters[TsuboB].init("FX_TsuboB", "Tubo", false);
    sShpEmitters[BRockA].init("FX_BRockA", "FXBRockA", true);
    sShpEmitters[FruitA].init("FX_FruitA", "FruitA", false);
    sShpEmitters[GrassCoil].init("FX_GrassCoil", "GrassCoilPiece", false);
    sShpEmitters[BRockB].init("FX_BRockB", "RockFace", true);
    sShpEmitters[Beehive].init("FX_Beehive", "Bee", false);
    sShpEmitters[VeraRock].init("FX_VeraRock", "Bvd", false);
    sShpEmitters[Pumpkin].init("FX_Pumpkin", "Pumpkin", false);
    sShpEmitters[BRockC].init("FX_BRockC", "FX_BRockC", true);
    sShpEmitters[F300BrokenRockWall_00].init("FX_F300BrokenRockWall_00", "F300BrokenRockWall", true);
    sShpEmitters[F300BrokenRockWall_01].init("FX_F300BrokenRockWall_01", "F300BrokenRockWall", true);
    sShpEmitters[F300BrokenRockWall_01T].init("FX_F300BrokenRockWall_01T", "F300BrokenRockWall", true);
    sShpEmitters[LotusSeed].init("FX_LotusSeed", "LotusSeed", false);
    sShpEmitters[Beamoss].init("FX_Beamoss", "Beamos", false);
    sShpEmitters[Cakram].init("FX_Cakram", "Asura", false);
    sShpEmitters[Dodai].init("FX_Dodai", "Asura", false);
    sShpEmitters[LegParts].init("FX_LegParts", "Asura", false);
    sShpEmitters[AsuraFloorBrk].init("FX_AsuraFloorBrk", "Asura", false);
    sShpEmitters[BreakPillar].init("FX_BreakPillar", "BreakPillar", false);
    sShpEmitters[Barrel].init("FX_Barrel", "Barrel", false);
    sShpEmitters[BarrelBomb].init("FX_BarrelBomb", "BarrelBomb", false);
    sShpEmitters[RockMiddle].init("FX_RockMiddle", "RockCarryMiddle", false);
    sShpEmitters[BocoBone].init("FX_BocoBone", "Skull", false);
    sShpEmitters[BRockStopA].init("FX_BRockStopA", "BRockStopA", true);
    sShpEmitters[BWallD201].init("FX_BWallD201", "BWallD201", true);
    sShpEmitters[Gunho].init("FX_Gunho", "Gunho", false);
    sShpEmitters[TuboBig].init("FX_TuboBig", "TuboBig", false);
    sShpEmitters[Amos].init("FX_Amos", "Amos", false);
    sShpEmitters[HidokariBoneA].init("FX_HidokariBoneA", "HidokariS", false);
    sShpEmitters[HidokariBoneB].init("FX_HidokariBoneB", "HidokariS", false);
    sShpEmitters[Hydra].init("FX_Hydra", "Hydra", false);
    sShpEmitters[Captain].init("FX_Captain", "Captain", false);
    sShpEmitters[KibakoHang].init("FX_KibakoHang", "KibakoHang", true);
    sShpEmitters[IslTreIRock].init("FX_IslTreIRock", "IslTreIRock", false);
    sShpEmitters[BirdraceTarget].init("FX_BirdraceTarget", "BirdraceTarget", false);
    sShpEmitters[FlowerA00F].init("FX_FlowerA00F", "FlowerA00", false);
    sShpEmitters[FlowerA00L].init("FX_FlowerA00L", "FlowerA00", false);
    sShpEmitters[FlowerB00].init("FX_FlowerB00", "FlowerB00", false);
    sShpEmitters[FlowerB01].init("FX_FlowerB01", "FlowerB01", false);
    sShpEmitters[GirahimClaymore].init("FX_GirahimClaymore", "GirahimBt03", false);
    sShpEmitters[BirdKobunAEgg].init("FX_BirdKobunAEgg", "BirdEgg", false);
    sShpEmitters[BirdKobunBEgg].init("FX_BirdKobunBEgg", "BirdEgg", false);
    sShpEmitters[BirdRivalEgg].init("FX_BirdRivalEgg", "BirdEgg", false);
    sShpEmitters[BWallAF200].init("FX_BWallAF200", "BWallAF200", true);
    sShpEmitters[BWallBF200].init("FX_BWallBF200", "BWallBF200", true);
    sShpEmitters[BWallF210].init("FX_BWallF210", "BWallF210", true);
}

u32 dJEffManager_c::sInts[] = {0x28, 0x29, 0x87, 0x88, 0x89, 0x8A, 0x8C, 0x8D, 0x91, 0x86, 0x1, 0x2};
u32 dJEffManager_c::sInts2[] = {0x2, 0x87, 0x8B};

void dJEffManager_c::removeEffManagers() {
    for (s32 i = 0; i < sNumMassObjEmitters; i++) {
        sMassObjEmitters[i].remove();
    }

    // TODO
    EffectsList::Iterator it = sPlayingEffectsList.GetBeginIter();
    while (it != sPlayingEffectsList.GetEndIter()) {
        EffectsList::Iterator itCopy;
        ++itCopy;
        it->remove(true);
        it = itCopy;
    }

    dParticle::mgr_c::GetInstance()->forceDeleteAllEmitter();
    for (s32 i = 0; i < 47; i++) {
        sShpEmitters[i].clear();
    }
}

void dJEffManager_c::doCustomSkywardSwordThing(f32 x, f32 y) {
    EffectsList::Iterator it = sPlayingEffectsList.GetBeginIter();
    while (it != sPlayingEffectsList.GetEndIter()) {
        if (it->getEmitterCallback() != nullptr && it->hasEmitters() && it->getGroupId() < 12) {
            it->getEmitterCallback()->vt_0x20(x, y);
        }
        ++it;
    }

    for (s32 i = 0; i < 12; i++) {
        dParticle::mgr_c::GetInstance()->doCustomSkywardSwordThing(i, x, y);
    }
}

void dJEffManager_c::execute() {
    for (EffectsList::Iterator it = sPlayingEffectsList.GetBeginIter(); it != sPlayingEffectsList.GetEndIter();) {
        EffectsList::Iterator itNext = it;
        ++itNext;
        it->execute();
        if (it->checkFlag(EffectsStruct::EMITTER_Fading)) {
            it->realizeAlpha();
        }
        if (it->checkFlag(EffectsStruct::EMITTER_0x1)) {
            it->offFlag(EffectsStruct::EMITTER_0x1);
        } else {
            if (it->areAllEmittersDone()) {
                it->remove(false);
            } else {
                it->onFlag(EffectsStruct::EMITTER_0x2);
            }
        }
        it = itNext;
    }

    if (dStageMgr_c::GetInstance() != nullptr) {
        CURRENT_EFFECT_MANAGER_INIT->setGlobalAlpha(dStageMgr_c::GetInstance()->getGlobalAlpha());
    }

    for (s32 i = 0; i < sNumMassObjEmitters; i++) {
        mColor c = sMassObjEmitters[i].getField_0x67C();
        sMassObjEmitters[i].loadColors(&c, nullptr, 0, 0);
        sMassObjEmitters[i].execute();
    }

    if ((dBase_c::s_ExecuteControlFlags & (dBase_c::BASE_PROP_UNK_PARTICLE_1)) == 0) {
        for (int i = 0; i < 12; i++) {
            dParticle::mgr_c::GetInstance()->calc(i);
        }
    } else {
        dParticle::mgr_c::GetInstance()->calc(9);
    }

    dParticle::mgr_c::GetInstance()->calc(12);
    dParticle::mgr_c::GetInstance()->calc(13);
    dParticle::mgr_c::GetInstance()->calc(14);
}

void dJEffManager_c::draw() {
    for (s32 i = 0; i < 12; i++) {
        sFogProcs[i].entry();
    }

    s2DEffects[0].addToDrawList();
    s2DEffects[1].addToDrawList();
    s2DEffects[2].addToDrawList();

    for (s32 i = 0; i < 47; i++) {
        sShpEmitters[i].draw();
    }
}

bool dJEffManager_c::shouldBePaused(dBase_c *owner) {
    return owner != nullptr && !owner->checkBaseProperty(dBase_c::BASE_PROP_0x4) &&
           (EventManager::isInEvent() || owner->checkProcControl(fBase_c::DISABLE_EXECUTE) ||
            (owner->s_ExecuteControlFlags & dBase_c::BASE_PROP_UNK_PARTICLE_2));
}

void dJEffManager_c::draw(const JPADrawInfo *info, u32 groupId) {
    dParticle::mgr_c::GetInstance()->draw(info, groupId);
}

bool dJEffManager_c::createEffManagers() {
    EGG::Heap *heap = dHeap::work1Heap.heap;
    ms_allocator = new (heap, 4) mHeapAllocator_c();
    sMassObjEmitters = new (heap, 4) dMassObjEmitter_c[sNumMassObjEmitters];

    for (s32 idx = 0; idx < sNumMassObjEmitters; idx++) {
        // TODO explain this
        if (sEffectResourceIds[idx] == PARTICLE_RESOURCE_ID_MAPPING_119_) {
            CURRENT_EFFECT_MANAGER_INIT = &sMassObjEmitters[idx];
            break;
        }
    }

    ms_allocator->createFrmHeapToCurrent(
        -1, dHeap::work1Heap.heap, "dJEffManager_c::ms_allocator", 0x20, mHeap::OPT_NONE
    );
    for (s32 idx = 0; idx < 2; idx++) {
        sFogProcs[idx].create(idx, sInts[idx], -1, ms_allocator);
    }
    for (s32 idx = 2; idx < 12; idx++) {
        sFogProcs[idx].create(idx, -1, sInts[idx], ms_allocator);
    }

    for (s32 idx = 0; idx < 3; idx++) {
        s2DEffects[idx].create(idx + 12, sInts2[idx]);
    }

    for (s32 idx = 0; idx < 47; idx++) {
        sShpEmitters[idx].create(ms_allocator);
    }

    ms_allocator->adjustFrmHeapRestoreCurrent();
    for (int i = 0; i < 2; i++) {
        sCommonEmitterCallbacks[i].init(i);
    }
    return true;
}

dEmitterBase_c *dJEffManager_c::spawnEffectInternal(
    u16 effectResourceId, const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
    const GXColor *c2, s32 idx1, s32 idx2
) {
    if (!sEmitter.createEmitters(effectResourceId, position, rot, scale, c1, c2, idx1, idx2)) {
        return nullptr;
    }
    return &sEmitter;
}

dEmitterBase_c *dJEffManager_c::spawnEffectInternal(
    u16 effectResourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2
) {
    dEmitterBase_c *e = spawnEffectInternal(effectResourceId, mVec3_c::Zero, nullptr, nullptr, c1, c2, idx1, idx2);
    if (e != nullptr) {
        e->setTransform(transform);
    }
    return e;
}

dEmitterBase_c *dJEffManager_c::spawnEffect(
    u16 effectResourceId, const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
    const GXColor *c2, s32 idx1, s32 idx2
) {
    return spawnEffectInternal(effectResourceId, position, rot, scale, c1, c2, idx1, idx2);
}

dEmitterBase_c *dJEffManager_c::spawnUIEffect(
    u16 effectResourceId, const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
    const GXColor *c2
) {
    mVec3_c adjustedPosition(position.x * dGfx_c::getCurrentScreenTo4x3WidthScaleF(), position.y, position.z);
    return spawnEffectInternal(effectResourceId, adjustedPosition, rot, scale, c1, c2, 0, 0);
}

dEmitterBase_c *dJEffManager_c::spawnEffect(
    u16 effectResourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2
) {
    return spawnEffectInternal(effectResourceId, transform, c1, c2, idx1, idx2);
}

bool EffectsStruct::createEffect(
    u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1, const GXColor *c2
) {
    return createEffect(true, resourceId, pos, rot, scale, c1, c2);
}

bool EffectsStruct::createUIEffect(
    u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1, const GXColor *c2
) {
    mVec3_c adjustedPosition(pos.x * dGfx_c::getCurrentScreenTo4x3WidthScaleF(), pos.y, pos.z);
    return createEffect(true, resourceId, adjustedPosition, rot, scale, c1, c2);
}

bool EffectsStruct::createEffect(u16 resourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2) {
    return createEffect(true, resourceId, transform, c1, c2);
}

bool EffectsStruct::createContinuousEffect(
    u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1, const GXColor *c2
) {
    return createEffect(false, resourceId, pos, rot, scale, c1, c2);
}

bool EffectsStruct::createContinuousUIEffect(
    u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1, const GXColor *c2
) {
    mVec3_c adjustedPosition(pos.x * dGfx_c::getCurrentScreenTo4x3WidthScaleF(), pos.y, pos.z);
    return createEffect(false, resourceId, adjustedPosition, rot, scale, c1, c2);
}

bool EffectsStruct::createContinuousEffect(
    u16 resourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2
) {
    return createEffect(false, resourceId, transform, c1, c2);
}

bool dJEffManager_c::createMassObjEffect(
    u16 effectResourceId, const mVec3_c &v1, dAcObjBase_c *owner, const mColor *color
) {
    for (s32 i = 0; i < sNumMassObjEmitters; i++) {
        if (effectResourceId == sEffectResourceIds[i]) {
            if (color != nullptr) {
                sMassObjEmitters[i].setField_0x67C(*color);
            } else {
                sMassObjEmitters[i].setField_0x67C(mColor(0xFF, 0xFF, 0xFF, 0xFF));
            }
            return sMassObjEmitters[i].start(v1, owner);
        }
    }

    return false;
}

void dEmitterBase_c::loadColors(
    JPABaseEmitter *emitter, const GXColor *color1, const GXColor *color2, s32 plltIdx1, s32 plltIdx2
) {
    u8 r1 = 0xFF;
    u8 g1 = 0xFF;
    u8 b1 = 0xFF;
    u8 r2 = 0xFF;
    u8 g2 = 0xFF;
    u8 b2 = 0xFF;
    u8 r, g, b;

    dLightEnv_c &mgr = dLightEnv_c::GetInstance();
    const mColor c1 = mgr.GetCurrentSpf().mParticleTransparentClr;
    const mColor c2 = mgr.GetCurrentSpf().mParticleSolidClr;

    u32 flags = emitter->getDynResUserWork();
    u32 factor = flags >> 24;
    bool useBlack4 = false;
    if (factor > 100) {
        useBlack4 = true;
        factor = factor - 100;
    }
    f32 fFactor = factor / 100.0f;
    if (fFactor > 1.0f) {
        fFactor = 1.0f;
    }

    f32 scaleR, scaleG, scaleB;
    if (!useBlack4) {
        scaleR = 1.0f - (1.0f - (c1.r / 255.0f)) * fFactor;
        scaleG = 1.0f - (1.0f - (c1.g / 255.0f)) * fFactor;
        scaleB = 1.0f - (1.0f - (c1.b / 255.0f)) * fFactor;
    } else {
        scaleR = 1.0f - (1.0f - (c2.r / 255.0f)) * fFactor;
        scaleG = 1.0f - (1.0f - (c2.g / 255.0f)) * fFactor;
        scaleB = 1.0f - (1.0f - (c2.b / 255.0f)) * fFactor;
    }

    if (color1 != nullptr) {
        r1 = color1->r;
        g1 = color1->g;
        b1 = color1->b;
    }

    if (plltIdx1 > 0) {
        if ((flags & 0x1000) != 0) {
            const mColor &entry = mgr.getSmallEAF(plltIdx1 - 1, plltIdx2).field_0x00;
            r1 = entry.r;
            g1 = entry.g;
            b1 = entry.b;
        } else if ((flags & 0x2000) != 0) {
            const mColor &entry = mgr.getSmallEAF(plltIdx1 - 1, plltIdx2).field_0x08;
            r1 = entry.r;
            g1 = entry.g;
            b1 = entry.b;
        }
    }

    if (factor != 0) {
        r = r1 * scaleR * mgr.getfield_0x2F14();
        g = g1 * scaleG * mgr.getfield_0x2F18();
        b = b1 * scaleB * mgr.getfield_0x2F1C();
    } else {
        r = r1 * scaleR;
        g = g1 * scaleG;
        b = b1 * scaleB;
    }

    emitter->setGlobalPrmColor(r, g, b);

    if (color2 != nullptr) {
        r2 = color2->r;
        g2 = color2->g;
        b2 = color2->b;
    }

    if (plltIdx1 > 0) {
        if ((flags & 0x1000) != 0) {
            const mColor &entry = mgr.getSmallEAF(plltIdx1 - 1, plltIdx2).field_0x04;
            r2 = entry.r;
            g2 = entry.g;
            b2 = entry.b;
        } else if ((flags & 0x2000) != 0) {
            const mColor &entry = mgr.getSmallEAF(plltIdx1 - 1, plltIdx2).field_0x0C;
            r2 = entry.r;
            g2 = entry.g;
            b2 = entry.b;
        }
    }

    if (factor != 0) {
        r = mgr.getfield_0x2F14() * r2 * scaleR;
        g = mgr.getfield_0x2F18() * g2 * scaleG;
        b = mgr.getfield_0x2F1C() * b2 * scaleB;
    } else {
        r = r2 * scaleR;
        g = g2 * scaleG;
        b = b2 * scaleB;
    }

    emitter->setGlobalEnvColor(r, g, b);
}

void EffectsStruct::removeFromActiveEmittersList() {
    if (dJEffManager_c::sPlayingEffectsList.GetPosition(this) != dJEffManager_c::sPlayingEffectsList.GetEndIter()) {
        dJEffManager_c::sPlayingEffectsList.remove(this);
    }
}

bool EffectsStruct::createEffect(
    bool bFlags, u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
    const GXColor *c2
) {
    if (!bFlags && canReuse(resourceId)) {
        setPosRotScale(pos, rot, scale);
        s32 idx1 = 0;
        s32 idx2 = 0;
        getOwnerPolyAttrs(&idx1, &idx2);
        loadColors(c1, c2, idx1, idx2);
        onFlag(EMITTER_0x1);
    } else {
        remove(false);
        s32 idx1 = 0;
        s32 idx2 = 0;
        getOwnerPolyAttrs(&idx1, &idx2);
        if (createEmitters(resourceId, pos, rot, scale, c1, c2, idx1, idx2)) {
            addToActiveEmittersList(resourceId, bFlags);
        }
    }

    return hasEmitters();
}

bool EffectsStruct::createEffect(
    bool bFlags, u16 resourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2
) {
    if (!bFlags && canReuse(resourceId)) {
        s32 idx1 = 0;
        s32 idx2 = 0;
        getOwnerPolyAttrs(&idx1, &idx2);
        loadColors(c1, c2, idx1, idx2);
        onFlag(EMITTER_0x1);
    } else {
        remove(false);
        s32 idx1 = 0;
        s32 idx2 = 0;
        getOwnerPolyAttrs(&idx1, &idx2);
        if (createEmitters(resourceId, mVec3_c::Zero, nullptr, nullptr, c1, c2, idx1, idx2)) {
            addToActiveEmittersList(resourceId, bFlags);
        }
    }
    setTransform(transform);
    return hasEmitters();
}

// TODO: Document polyAttr1 and GROUND enum
dJEffManager_c::GroundEffect_e dJEffManager_c::polyAttrsToGroundEffectIdx(s32 polyAttr0, s32 polyAttr1) {
    if (polyAttr0 == POLY_ATT_0_NONE || (polyAttr0 == POLY_ATT_0_GRASS && polyAttr1 == 1) ||
        (polyAttr0 == POLY_ATT_0_LOTUS) || (polyAttr0 == POLY_ATT_0_METAL && polyAttr1 == 1) ||
        (polyAttr0 == POLY_ATT_0_TUTA) || (polyAttr0 == POLY_ATT_0_DEATH && polyAttr1 != 1) ||
        (polyAttr0 == POLY_ATT_0_LIFE && (polyAttr1 == 1 || polyAttr1 == 3)) || polyAttr0 == POLY_ATT_0_MAX) {
        return GROUND_6;
    } else if (polyAttr0 == POLY_ATT_0_DEATH) {
        return GROUND_5;
    } else if (polyAttr0 == POLY_ATT_0_LAVA) {
        return GROUND_2;
    } else if (polyAttr0 == POLY_ATT_0_GRASS) {
        return GROUND_1;
    } else if (polyAttr0 == POLY_ATT_0_SAND || polyAttr0 == POLY_ATT_0_QSAND) {
        return GROUND_3;
    } else if (polyAttr0 == POLY_ATT_0_NUMA) {
        return GROUND_4;
    } else {
        return GROUND_0;
    }
}

dEmitterBase_c *dJEffManager_c::spawnGroundEffect(
    const mVec3_c &pos, u8 polyAttr0, u8 polyAttr1, const mVec3_c &v1, s32 unk, f32 scale, f32 groundHeightMaybe
) {
    static const u16 sEffArray[6][2] = {
        { PARTICLE_RESOURCE_ID_MAPPING_89_,  PARTICLE_RESOURCE_ID_MAPPING_90_},
        { PARTICLE_RESOURCE_ID_MAPPING_87_,  PARTICLE_RESOURCE_ID_MAPPING_88_},
        {PARTICLE_RESOURCE_ID_MAPPING_429_, PARTICLE_RESOURCE_ID_MAPPING_429_},
        {PARTICLE_RESOURCE_ID_MAPPING_416_, PARTICLE_RESOURCE_ID_MAPPING_417_},
        {PARTICLE_RESOURCE_ID_MAPPING_418_, PARTICLE_RESOURCE_ID_MAPPING_419_},
        {PARTICLE_RESOURCE_ID_MAPPING_893_, PARTICLE_RESOURCE_ID_MAPPING_894_},
    };

    if (pos.y < groundHeightMaybe) {
        return nullptr;
    }

    GroundEffect_e idx = polyAttrsToGroundEffectIdx(polyAttr0, polyAttr1);
    if (idx == GROUND_6) {
        return nullptr;
    }
    if (idx == GROUND_2 && unk == 0) {
        scale *= 1.5f;
    }
    mMtx_c mtx;
    mtx.makeRotationFromVecs(mVec3_c::Ey, v1, 1.0f);
    mAng rot(cM::rndF(65536.0f));
    mtx.YrotM(rot);
    mtx.setTranslation(pos);
    mMtx_c scaleMtx;
    MTXScale(scaleMtx, scale, scale, scale);
    MTXConcat(mtx, scaleMtx, mtx);
    return spawnEffect(sEffArray[idx][unk], mtx, nullptr, nullptr, polyAttr0, polyAttr1);
}

void dWaterEffect_c::init(dAcObjBase_c *base, f32 height, f32 scale, f32 f3) {
    mEff.init(base);
    mHeight = height;
    mScale = scale;
    mDepth = f3;
}

void dWaterEffect_c::execute(f32 water, f32 ground) {
    dAcObjBase_c *ac = getActor();
    bool b = getActorGroundPos(ac) < water && ground < water;
    if (b) {
        if (!mIsInWater) {
            mIsInWater = true;
            mVec3_c pos(ac->mPosition.x, water, ac->mPosition.z);
            mVec3_c scale(mScale, mScale, mScale);
            if (mIsSmall || water - ground < 50.0f) {
                // For small objects or shallow water, create a
                // particle FX upon entering water
                mAng3_c rot(0, cM::rndF(65536.0f), 0);
                dJEffManager_c::spawnEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_91_, pos, &rot, &scale, nullptr, nullptr, 0, 0
                );
            } else {
                // Otherwise spawn a "water spout" with a 3d model
                dAcObjBase_c::create(fProfile::OBJ_WATER_SPOUT, -1, 0, &pos, nullptr, &scale, -1);
            }
        }
    } else if (getActorGroundPos(ac) - 5.0f > water) {
        mIsInWater = false;
    }

    if (mIsInWater && getActorCeilPos(ac) > water) {
        // Spawn effect while in water
        mVec3_c pos(ac->mPosition.x, water, ac->mPosition.z);
        mVec3_c scale(mScale, mScale, mScale);
        mEff.createContinuousEffect(PARTICLE_RESOURCE_ID_MAPPING_127_, pos, nullptr, &scale, nullptr, nullptr);
        f32 rate = nw4r::math::FAbs(ac->mSpeed) * 0.02f;
        rate = rate > 0.95f ? 0.95f : rate;
        mEff.setRate(rate + 0.05f);
    }
}
