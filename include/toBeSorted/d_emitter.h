#ifndef EFFECTS_STRUCT_H
#define EFFECTS_STRUCT_H

#include "JSystem/JParticle/JPADrawInfo.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_base.h"
#include "m/m2d.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "rvl/GX/GXTypes.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_unk_mdl_stuff_2.h"
#include "toBeSorted/tlist.h"

#include <cstddef>

class dEmitterBase_c;
class dEmitterCallback_c;
class dParticleCallback_c;

#include "toBeSorted/d_emmitter_resource_map.inc"

class dEmitterBase_c {
    friend class dEmitterCallback_c;
    friend class dParticleCallback_c;

public:
    dEmitterBase_c() : mpEmitterHead(nullptr), mpEmitterCallback(nullptr), mpParticleCallback(nullptr) {}

    bool createEmitters(
        u16 effectResourceId, const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
        const GXColor *c2, s32 idx1, s32 idx2
    );

    dEmitterCallback_c *getEmitterCallback() const {
        return mpEmitterCallback;
    }

    void setEmitterCallback(dEmitterCallback_c *cb);
    void setParticleCallback(dParticleCallback_c *cb);

protected:
    void deactivateEmitters();
    void stopCalcEmitters();
    void playCalcEmitters();
    static void loadColors(JPABaseEmitter *emitter, const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2);

    void setImmortal();

    static JPABaseEmitter *GetNextEmitter(JPABaseEmitter *head);

    /* 0x00 */ JPABaseEmitter *mpEmitterHead;
    /* 0x04 */ dEmitterCallback_c *mpEmitterCallback;
    /* 0x08 */ dParticleCallback_c *mpParticleCallback;
    /* 0x0C */ TListNode<dEmitterBase_c> mEmitterCallbackNode;
    /* 0x14 */ TListNode<dEmitterBase_c> mParticleCallbackNode;

public:
    // vtable at 0x1C
    virtual ~dEmitterBase_c();

    void stopDrawParticles();
    void playDrawParticles();
    void setPosRotScale(const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale);
    void setPosRotScaleWithScreenScale(const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale);
    void setTransform(const mMtx_c &mtx);
    void loadColors(const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2);
    void setGlobalAlpha(u8 alpha);
    void setRate(f32 rate);
    void setDynamicsScale(const mVec3_c &scale);
    void setParticleScale(const mVec3_c &scale);
    void setAwayFromCenterSpeed(f32 speed);
    void setVolumeSize(u16 size);
    void setLifeTime(s16 lifetime);
    void attachEmitterCallbackId(s32 id);     // corresponds to setup at 80028a80
    void bindShpEmitter(s32 id, bool unused); // corresponds to setup at 8002b6b0
};

// Suggested name: dEmitter_c
class EffectsStruct : public dEmitterBase_c {
public:
    enum Flags_e {
        EMITTER_0x1 = 0x1,
        EMITTER_0x2 = 0x2,
        EMITTER_0x4 = 0x4,
        EMITTER_Fading = 0x8,
        EMITTER_0x10 = 0x10,
        EMITTER_0x20 = 0x20,
    };

    // vt at 0x1C
    EffectsStruct();
    EffectsStruct(dBase_c *);
    virtual ~EffectsStruct();

    inline void init(dBase_c *owner) {
        mpOwner = owner;
    }

    inline dBase_c *getOwner() const {
        return mpOwner;
    }

    void addToActiveEmittersList(u16 resourceId, bool bFlags);
    void removeFromActiveEmittersList();
    void execute();
    void setFading(u8 lifetime);

    void remove(bool bForceDeleteEmitters);

    // TODO maybe reconsider the naming here - the observation here is that a "continous" effect
    // is typically called every frame, while the others are one-shot calls
    bool createContinuousEffect(
        u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
        const GXColor *c2
    );
    bool createContinuousUIEffect(
        u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
        const GXColor *c2
    );
    bool createContinuousEffect(u16 resourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2);

    bool createEffect(
        u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
        const GXColor *c2
    );
    bool createUIEffect(
        u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
        const GXColor *c2
    );
    bool createEffect(u16 resourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2);

    bool hasEmitters() const {
        return mpEmitterHead != 0;
    }

    u8 getGroupId() const {
        return mpEmitterHead->getGroupID();
    }

    bool checkFlag(u32 flag) const {
        return mFlags & flag;
    }

    void onFlag(u32 flag) {
        mFlags |= flag;
    }

    void offFlag(u32 flag) {
        mFlags &= ~flag;
    }

    void realizeAlpha();
    bool areAllEmittersDone();

protected:
    bool canReuse(u16 resourceId) const {
        return hasEmitters() && !checkFlag(EMITTER_0x2) && mEffect == resourceId;
    }

    bool createEffect(
        bool bFlags, u16 resourceId, const mVec3_c &pos, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
        const GXColor *c2
    );
    bool createEffect(bool bFlags, u16 resourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2);
    bool getOwnerPolyAttrs(s32 *pOut1, s32 *pOut2);

public:
    /* 0x24 */ TListNode<EffectsStruct> mNode;

protected:
    /* 0x28 */ dBase_c *mpOwner;
    /* 0x2C */ u8 mFadeTimer;
    /* 0x2D */ u8 mFadeDuration;
    /* 0x2E */ u8 _0x2D[0x30 - 0x2E];
    /* 0x30 */ u16 mFlags;
    /* 0x32 */ u16 mEffect;
};

class dWaterEffect_c {
private:
    /* 0x00 */ bool mIsInWater;
    /* 0x01 */ bool mIsSmall;
    /* 0x04 */ f32 mHeight;
    /* 0x08 */ f32 mDepth;
    /* 0x0C */ f32 mScale;
    /* 0x10 */ EffectsStruct mEff;

    dAcObjBase_c *getActor() const {
        return static_cast<dAcObjBase_c *>(mEff.getOwner());
    }

    f32 getActorGroundPos(dAcObjBase_c *obj) const {
        return obj->mPosition.y + mDepth;
    }

    f32 getActorCeilPos(dAcObjBase_c *obj) const {
        return obj->mPosition.y + mHeight;
    }

public:
    dWaterEffect_c() : mIsInWater(1), mIsSmall(false) {}
    virtual ~dWaterEffect_c() {}

    void init(dAcObjBase_c *, f32 height, f32 scale, f32 depth);
    void execute(f32 water, f32 ground);

    void setIsSmall(bool val) {
        mIsSmall = val;
    }
};

/** An emitter callback can be part of multiple emitters. */
class dEmitterCallback_c : public JPAEmitterCallBack {
    friend class dEmitterBase_c;

    typedef TList<dEmitterBase_c, offsetof(dEmitterBase_c, mEmitterCallbackNode)> EmitterCallbackList;

public:
    virtual ~dEmitterCallback_c();
    virtual void create(JPABaseEmitter *) {}
    virtual void vt_0x20(f32, f32) {}

    void remove(dEmitterBase_c *emitter);

    /* 0x04 */ EmitterCallbackList mEmitterList;
};

/** A particle callback can be part of multiple emitters. */
class dParticleCallback_c : public JPAParticleCallBack {
    friend class dEmitterBase_c;

    typedef TList<dEmitterBase_c, offsetof(dEmitterBase_c, mParticleCallbackNode)> ParticleCallbackList;

public:
    virtual ~dParticleCallback_c();
    virtual void create(JPABaseEmitter *) {}

    void remove(dEmitterBase_c *emitter);

    /* 0x04 */ ParticleCallbackList mEmitterList;
};

class dParticleFogProc_c : public d3d::UnkProc {
public:
    dParticleFogProc_c() {}
    virtual ~dParticleFogProc_c() {}
    virtual void drawOpa() override {
        doDraw();
    }
    virtual void drawXlu() override {
        doDraw();
    }

    bool create(u32 idx, s32 prioOpa, s32 prioXlu, mHeapAllocator_c *alloc);

private:
    void doDraw();
    /* 0x18 */ u32 mIdx;
    /* 0x1C */ bool field_0x1C;
};

class dEffect2D_c : public m2d::Base_c {
public:
    dEffect2D_c() : m2d::Base_c(0x80) {}
    /* vt 0x08 */ virtual ~dEffect2D_c() {}
    /* vt 0x0C */ virtual void draw() override;

    void create(u32 groupId, u8);

private:
    /* 0x10 */ u32 mGroupId;
};

class dShpEmitter_c;

class dShpEmitterProc : public dShpProc1_c {
public:
    dShpEmitterProc() : mpOwner(nullptr) {}
    virtual ~dShpEmitterProc() {
        mpOwner = nullptr;
    }
    virtual void drawOpa() override {
        doDraw();
    }
    virtual void drawXlu() override {
        doDraw();
    }

    void setOwner(dShpEmitter_c *cb) {
        mpOwner = cb;
    }

    void setAlpha(s32 idx, u8 val) {
        mAlpha[idx] = val;
    }

private:
    void doDraw();
    /* 0x34 */ dShpEmitter_c *mpOwner;
    // TODO: This is read from JParticle as alpha, but used as the nw4r::g3d lightSetId.
    // Is this a creative use of the particle color value, given that particles themselves
    // are never drawn?
    /* 0x38 */ u8 mAlpha[100];
};

class dShpEmitter_c : public JPAParticleCallBack {
public:
    virtual ~dShpEmitter_c() {}
    virtual void draw(JPABaseEmitter *, JPABaseParticle *) override;

    bool create(mHeapAllocator_c *alloc);
    void init(const char *mdlName, const char *arcName, bool priority);
    void clear();
    void remove();
    void draw() {
        mProc.entry();
    }

private:
    /* 0x04 */ dShpEmitterProc mProc;
    /* 0xA0 */ s32 field_0xA0;
};

class CommonEmitterCallback : public JPAEmitterCallBack {
public:
    CommonEmitterCallback() : field_0x04(0) {}
    virtual ~CommonEmitterCallback() {}
    virtual void draw(JPABaseEmitter *) override;
    void init(s32 v) {
        field_0x04 = v;
    }

private:
    /* 0x04 */ s32 field_0x04;
};

class dMassObjEmitterCallback_c : public dEmitterCallback_c {
public:
    virtual ~dMassObjEmitterCallback_c() {}
    virtual void executeAfter(JPABaseEmitter *) override;

    void execute();
    void clear();
    bool start(const mVec3_c &v1, dAcObjBase_c *owner);

private:
    /* 0x010 */ mVec3_c field_0x010[50];
    /* 0x268 */ mVec3_c field_0x268[50];
    /* 0x4C0 */ dAcObjBase_c *field_0x4C0[50];
    /* 0x588 */ dAcObjBase_c *field_0x588[50];
    /* 0x650 */ mColor field_0x650;
    /* 0x654 */ u32 field_0x654;
    /* 0x658 */ u32 field_0x658;
    // TODO unclear structure boundary
};

class dMassObjEmitter_c : public dEmitterBase_c {
public:
    dMassObjEmitter_c() {}
    virtual ~dMassObjEmitter_c() {
        remove();
    }

    void create(u16 resourceId);
    bool start(const mVec3_c &v1, dAcObjBase_c *owner) {
        return mCallback.start(v1, owner);
    }
    void remove();

    mColor getField_0x67C() const {
        return field_0x67C;
    }

    void setField_0x67C(GXColor clr) {
        field_0x67C.r = clr.r;
        field_0x67C.g = clr.g;
        field_0x67C.b = clr.b;
        field_0x67C.a = clr.a;
    }

    void setField_0x67C(mColor clr) {
        field_0x67C.r = clr.r;
        field_0x67C.g = clr.g;
        field_0x67C.b = clr.b;
        field_0x67C.a = clr.a;
    }

    void execute() {
        mCallback.execute();
    }

private:
    /* 0x020 */ dMassObjEmitterCallback_c mCallback;
    /* 0x67C */ mColor field_0x67C;
};

class dJEffManager_c {
public:
    static bool createEffManagers();
    static void removeEffManagers();
    static void draw(const JPADrawInfo *info, u32 groupId);
    static void draw();
    static void execute();
    static void doCustomSkywardSwordThing(f32 x, f32 y);
    static void setupEffects();
    static dEmitterBase_c *spawnEffect(
        u16 effectResourceId, const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
        const GXColor *c2, s32 idx1, s32 idx2
    );
    static dEmitterBase_c *spawnUIEffect(
        u16 effectResourceId, const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
        const GXColor *c2
    );
    static dEmitterBase_c *spawnEffect(
        u16 effectResourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2
    );
    static dEmitterBase_c *spawnGroundEffect(
        const mVec3_c &pos, u8 polyAttr0, u8 polyAttr1, const mVec3_c &, s32 unk, f32 scale, f32 groundHeightMaybe
    );

    static void spawnHitMarkEffect(int type, cCcD_Obj &collider, const mVec3_c *pVec, bool);
    // "mass obj" = grass, fire
    static bool createMassObjEffect(u16 effectResourceId, const mVec3_c &v1, dAcObjBase_c *owner, const mColor *color);

    static s32 getGroupId(u16);
    static bool shouldBePaused(dBase_c *owner);

    enum GroundEffect_e {
        GROUND_0,
        GROUND_1,
        GROUND_2,
        GROUND_3,
        GROUND_4,
        GROUND_5,
        GROUND_6,
    };

    enum Fx_e {
        TsuboA,
        TsuboB,
        BRockA,
        FruitA,
        GrassCoil,
        BRockB,
        Beehive,
        VeraRock,
        Pumpkin,
        BRockC,
        F300BrokenRockWall_00,
        F300BrokenRockWall_01,
        F300BrokenRockWall_01T,
        LotusSeed,
        Beamoss,
        Cakram,
        Dodai,
        LegParts,
        AsuraFloorBrk,
        BreakPillar,
        Barrel,
        BarrelBomb,
        RockMiddle,
        BocoBone,
        BRockStopA,
        BWallD201,
        Gunho,
        TuboBig,
        Amos,
        HidokariBoneA,
        HidokariBoneB,
        Hydra,
        Captain,
        KibakoHang,
        IslTreIRock,
        BirdraceTarget,
        FlowerA00F,
        FlowerA00L,
        FlowerB00,
        FlowerB01,
        GirahimClaymore,
        BirdKobunAEgg,
        BirdKobunBEgg,
        BirdRivalEgg,
        BWallAF200,
        BWallBF200,
        BWallF210,
    };

    typedef TList<EffectsStruct, offsetof(EffectsStruct, mNode)> EffectsList;
    static EffectsList sPlayingEffectsList;
    static CommonEmitterCallback sCommonEmitterCallbacks[2];
    static dShpEmitter_c sShpEmitters[47];
    static dEmitterBase_c sEmitter;
    static dParticleFogProc_c sFogProcs[12];
    static dEffect2D_c s2DEffects[3];

private:
    static dEmitterBase_c *spawnEffectInternal(
        u16 effectResourceId, const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1,
        const GXColor *c2, s32 idx1, s32 idx2
    );
    static dEmitterBase_c *spawnEffectInternal(
        u16 effectResourceId, const mMtx_c &transform, const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2
    );

    static GroundEffect_e polyAttrsToGroundEffectIdx(s32 polyAttr0, s32 polyAttr1);

    static mHeapAllocator_c *ms_allocator;
    static dMassObjEmitter_c *sMassObjEmitters;
    static u32 sInts[];
    static const u32 moreInts[];
    static u32 sInts2[];
};

#endif
