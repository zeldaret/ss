#include "JSystem/JParticle/JPADrawInfo.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/d_base.h"
#include "d/d_heap.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "egg/core/eggHeap.h"
#include "f/f_base.h"
#include "m/m_allocator.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtxvec.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/blur_and_palette_manager.h"
#include "toBeSorted/d_particle.h"
#include "toBeSorted/effects_struct.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/lyt_related_floats.h"

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

typedef TList<EffectsStruct, offsetof(EffectsStruct, mNode)> EffectsList;
EffectsList sPlayingEffectsList;
CommonEmitterCallback sCommonEmitterCallbacks[2];
dShpEmitter_c sShpEmitters[47];
dEmitterBase_c sEmitter;
dParticleFogProc_c sFogProcs[12];
dEffect2D_c s2DEffects[3];

// broken by -ipa file - reconsider splits?
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

// broken by -ipa file - reconsider splits?
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
        last = dParticle::mgr_c::GetInstance()->createEmitter(iter, getGroupId(iter), position, rot, scale);
        if (last != nullptr) {
            if (head != nullptr) {
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

extern "C" f32 lbl_80571C50;
inline f32 getlbl_80571C50() {
    return lbl_80571C50;
}
void dEmitterBase_c::setPosRotScaleWithScreenScale(const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale) {
    mVec3_c adjustedPosition(position.x * getlbl_80571C50(), position.y, position.z);
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
        emitter->setEmitterCallBackPtr(&sCommonEmitterCallbacks[id]);
    }
}

void dEmitterBase_c::bindShpEmitter(s32 id, bool unused) {
    for (JPABaseEmitter *emitter = mpEmitterHead; emitter != nullptr; emitter = GetNextEmitter(emitter)) {
        emitter->setParticleCallBackPtr(&sShpEmitters[id]);
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
    sPlayingEffectsList.append(this);
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
    if (mpOwner != nullptr && (mpOwner->delete_request || mpOwner->lifecycle_state == fBase_c::TO_BE_DELETED)) {
        mpOwner = nullptr;
    }
    if (shouldBePaused(mpOwner)) {
        if (!checkFlag(EMITTER_0x10)) {
            stopCalcEmitters();
            if (mpOwner != nullptr && !mpOwner->isBasePropertyFlag(0x100)) {
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
    if (mpOwner != nullptr && mpOwner->group_type == fBase_c::ACTOR) {
        dAcBase_c *actor = static_cast<dAcBase_c *>(mpOwner);
        *pOut1 = actor->polyAttr0;
        *pOut2 = actor->polyAttr1;
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

extern "C" bool fn_80054AD0();
void dEffect2D_c::draw() {
    f32 proj[GX_PROJECTION_SZ];
    GXGetProjectionv(proj);
    f32 base = proj[1];
    proj[1] = base / get_805751A4();
    GXSetProjectionv(proj);
    f32 f = get_80575184() / 2;
    mMtx_c mtx1, mtx2;
    JPADrawInfo info;

    C_MTXLightOrtho(mtx1, -get_80575150(), get_80575150(), -f, f, 0.5f, 0.5f, 0.5f, 0.5f);

    MTXScale(mtx2, fn_80054AD0() ? get_805751A4() : 1.0f, 1.0f, 1.0f);
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

void dMassObjEmitterCallback_c::executeAfter(JPABaseParticle *) {}

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

void dMassObjEmitter_c::remove() {
    setEmitterCallback(nullptr);
    mpEmitterHead = nullptr;
}

extern "C" u32 sNumberOfMgrs; // should be const?
extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_119_;
extern "C" dMassObjEmitter_c *CURRENT_EFFECT_MANAGER_INIT;

// various grasses being cut
extern const u16 sEffectResourceIds[];

void dJEffManager_c::setupEffects() {
    for (s32 idx = 0; idx < sNumberOfMgrs; idx++) {
        sManagers[idx].create(sEffectResourceIds[idx]);
    }

    sShpEmitters[TsuboA].init("FX_TsuboA", "Tubo", false);
    sShpEmitters[TsuboB].init("FX_TsuboB", "Tubo", false);
    sShpEmitters[BRockA].init("FX_BRockA", "FXBRockA", false);
    sShpEmitters[FruitA].init("FX_FruitA", "FruitA", true);
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

void dJEffManager_c::removeEffManagers() {
    for (s32 i = 0; i < sNumberOfMgrs; i++) {
        sManagers[i].remove();
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

// TODO explain this
static u32 sInts[] = {0x28, 0x29, 0x87, 0x88, 0x89, 0x8A, 0x8C, 0x8D, 0x91, 0x86, 0x1, 0x2};
static u32 sInts2[] = {0x2, 0x87, 0x8B};

s32 dEmitterBase_c::getGroupId(u16 resourceId) {
    u32 mask = dParticle::mgr_c::GetInstance()->getResUserWork(resourceId);
    s32 bit = 1;
    for (int i = 0; i < 10; i++) {
        if ((mask & (1 << i)) != 0) {
            bit = i;
            break;
        }
    }

    int i = sInts[bit];
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

    for (s32 i = 0; i < sNumberOfMgrs; i++) {
        mColor c = sManagers[i].getField_0x67C();
        sManagers[i].loadColors(&c, nullptr, 0, 0);
        sManagers[i].execute();
    }

    if ((dBase_c::s_ExecuteControlFlags & 0x6F9) == 0) {
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

bool EffectsStruct::shouldBePaused(dBase_c *owner) {
    return owner != nullptr && !owner->isBasePropertyFlag(dBase_c::BASE_PROP_0x4) &&
           (EventManager::isInEvent() || owner->isProcControlFlag(fBase_c::DISABLE_EXECUTE) ||
            // TODO execute control flags
            (owner->s_ExecuteControlFlags & 0x6fb));
}

bool dJEffManager_c::createEffManagers() {
    EGG::Heap *heap = dHeap::work1Heap.heap;
    ms_allocator = new (heap, 4) mHeapAllocator_c();
    sManagers = new (heap, 4) dMassObjEmitter_c[sNumberOfMgrs];

    for (s32 idx = 0; idx < sNumberOfMgrs; idx++) {
        // TODO explain this
        if (sEffectResourceIds[idx] == PARTICLE_RESOURCE_ID_MAPPING_119_) {
            CURRENT_EFFECT_MANAGER_INIT = &sManagers[idx];
            break;
        }
    }

    ms_allocator->createNewTempFrmHeap(-1, dHeap::work1Heap.heap, "dJEffManager_c::ms_allocator", 0x20, 0);
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
    mVec3_c adjustedPosition(position.x * getlbl_80571C50(), position.y, position.z);
    return spawnEffectInternal(effectResourceId, adjustedPosition, rot, scale, c1, c2, 0, 0);
}

dEmitterBase_c *dJEffManager_c::spawnEffect(
    u16 effectResourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2
) {
    return spawnEffectInternal(effectResourceId, transform, c1, c2, idx1, idx2);
}

void dEmitterBase_c::loadColors(
    JPABaseEmitter *emitter, const GXColor *color1, const GXColor *color2, s32 plltIdx1, s32 plltIdx2
) {
    BlurAndPaletteManager &mgr = BlurAndPaletteManager::GetInstance();
    u8 r1 = 0xFF;
    u8 g1 = 0xFF;
    u8 b1 = 0xFF;
    u8 r2 = 0xFF;
    u8 g2 = 0xFF;
    u8 b2 = 0xFF;
    u8 r, g, b;

    mColor c1 = mgr.GetCurrentSph().black3_000000;
    mColor c2 = mgr.GetCurrentSph().black4_000000;

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
        const mColor &c = mgr.GetCurrentSph().black4_000000;
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
            const PaletteEAF_smol_entry &entry = mgr.getSmallEAF(plltIdx1 - 1, plltIdx2);
            r1 = entry.field_0x00.r;
            g1 = entry.field_0x00.g;
            b1 = entry.field_0x00.b;
        } else if ((flags & 0x2000) != 0) {
            const PaletteEAF_smol_entry &entry = mgr.getSmallEAF(plltIdx1 - 1, plltIdx2);
            r1 = entry.field_0x08.r;
            g1 = entry.field_0x08.g;
            b1 = entry.field_0x08.b;
        }
    }

    if (factor != 0) {
        r = mgr.getfield_0x2F14() * r1 * scaleR;
        g = mgr.getfield_0x2F18() * g1 * scaleG;
        b = mgr.getfield_0x2F1C() * b1 * scaleB;
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
            const PaletteEAF_smol_entry &entry = mgr.getSmallEAF(plltIdx1 - 1, plltIdx2);
            r2 = entry.field_0x04.r;
            g2 = entry.field_0x04.g;
            b2 = entry.field_0x04.b;
        } else if ((flags & 0x2000) != 0) {
            const PaletteEAF_smol_entry &entry = mgr.getSmallEAF(plltIdx1 - 1, plltIdx2);
            r2 = entry.field_0x0C.r;
            g2 = entry.field_0x0C.g;
            b2 = entry.field_0x0C.b;
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
    if (sPlayingEffectsList.GetPosition(this) != sPlayingEffectsList.GetEndIter()) {
        sPlayingEffectsList.remove(this);
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
