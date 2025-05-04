#ifndef EFFECTS_STRUCT_H
#define EFFECTS_STRUCT_H

#include "JSystem/JParticle/JPADrawInfo.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAParticle.h"
#include "common.h"
#include "d/d_base.h"
#include "m/m2d.h"
#include "m/m3d/m_proc.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/d_unk_mdl_stuff_2.h"
#include "toBeSorted/tlist.h"

#include <cstddef>

class dEmitterBase_c;
class dEmitterCallback_c;
class dParticleCallback_c;

class dEmitterBase_c {
    friend class dEmitterCallback_c;
    friend class dParticleCallback_c;

public:
    dEmitterBase_c() : mpEmitterHead(nullptr), mpEmitterCallback(nullptr), mpParticleCallback(nullptr) {}

protected:
    void deactivateEmitters();
    void stopCalcEmitters();
    void playCalcEmitters();
    bool createEmitters(
        u16 effectResourceId, const mVec3_c &position, const mAng3_c *rot, const mVec3_c *scale, const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2
    );
    static u16 fn_80028900(u16);
    static void loadColors(JPABaseEmitter *emitter, const GXColor *c1, const GXColor *c2, s32 idx1, s32 idx2);

    void setEmitterCallback(dEmitterCallback_c *cb);
    void setParticleCallback(dParticleCallback_c *cb);

    void setImmortal();

    static JPABaseEmitter *GetNextEmitter(JPABaseEmitter *head);

    /* 0x00 */ JPABaseEmitter *mpEmitterHead;
    /* 0x04 */ dEmitterCallback_c *mpEmitterCallback;
    /* 0x08 */ dParticleCallback_c *mpParticleCallback;
    /* 0x0C */ TListNode<dEmitterBase_c> mEmitterCallbackNode;
    /* 0x14 */ TListNode<dEmitterBase_c> mParticleCallbackNode;

public:
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
    void attachEmitterCallbackId(s32 id);               // corresponds to setup at 80028a80
    void bindShpEmitter(s32 id, bool unused); // corresponds to setup at 8002b6b0
};

// Suggested name: dEmitter_c
class EffectsStruct : public dEmitterBase_c {
public:
    // vt at 0x1C
    EffectsStruct();
    EffectsStruct(dBase_c *);
    virtual ~EffectsStruct();

    inline void init(dBase_c *owner) {
        mpOwner = owner;
    }

    void addToActiveEmittersList(u16 resourceId, bool bFlags);
    void removeFromActiveEmittersList();

    void remove(bool bForceDeleteEmitters);
    void fn_80029920(u16 effect, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, void *, void *);
    void fn_80029980(u16 effect, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, void *, void *);
    void fn_800299F0(u16 effect, mMtx_c *mtx, void *, void *);
    void fn_80029A10(u16 effect, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, void *, void *);
    void fn_80029A70(u16 effect, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, void *, void *);

    static void fn_800298C0(u16 effect, mVec3_c *pos, void *, void *, void *, void *);

    bool hasEmitters() const {
        return mpEmitterHead != 0;
    }

protected:
    bool areAllEmittersDone();

public:
    /* 0x24 */ TListNode<EffectsStruct> mNode;

protected:
    /* 0x28 */ dBase_c *mpOwner;
    /* 0x2C */ u8 field_0x2C;
    /* 0x2D */ u8 _0x2D[0x30 - 0x2D];
    /* 0x30 */ u16 mFlags;
    /* 0x32 */ u16 mEffect;
};

// Not sure if these belongs here, but it uses EffectsStruct
// Related Functions in the 8002B100 area
class EffectsStruct_Ext {
public:
    EffectsStruct_Ext() : mField_0x00(1), mField_0x01(0) {}

    void init(dBase_c *, f32, f32, f32);

    void setField_0x00(u8 val) {
        mField_0x00 = val;
    }
    void setField_0x01(u8 val) {
        mField_0x01 = val;
    }

    void fn_8002B120(f32, f32);

    /* 0x00 */ u8 mField_0x00;
    /* 0x01 */ u8 mField_0x01;
    /* 0x04 */ f32 mField_0x04;
    /* 0x08 */ f32 mField_0x08;
    /* 0x0C */ f32 mField_0x0C;
    /* 0x10 */ EffectsStruct mEff;
    virtual ~EffectsStruct_Ext() {}
};

/** An emitter callback can be part of multiple emitters. */
class dEmitterCallback_c : public JPAEmitterCallBack {
    friend class dEmitterBase_c;

    typedef TList<dEmitterBase_c, offsetof(dEmitterBase_c, mEmitterCallbackNode)> EmitterCallbackList;

public:
    virtual ~dEmitterCallback_c();
    virtual void create(JPABaseEmitter *) {}
    virtual void vt_0x20(JPABaseEmitter *) {}

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

class dParticleFogProc_c : public d3d::EggTextureProc {
public:
    virtual ~dParticleFogProc_c() {}
    virtual void drawOpa() override {
        doDraw();
    }
    virtual void drawXlu() override {
        doDraw();
    }

    void create(s32 idx, s32 unk, s32 unk2, mHeapAllocator_c *alloc);

private:
    void doDraw();
    /* 0x18 */ u8 _0x18[0x1C - 0x18];
    /* 0x1C */ bool field_0x1C;
};

class dEffect2D_c : m2d::Base_c {
public:
    dEffect2D_c() : m2d::Base_c(0x80) {}
    /* vt 0x08 */ virtual ~dEffect2D_c() {}
    /* vt 0x0C */ virtual void draw() override;

    void create(u32 groupId, u8);

private:
    /* 0x10 */ u32 mGroupId;
};

class dJEffEmitterCallback_c : public dEmitterCallback_c {
public:
    virtual ~dJEffEmitterCallback_c() {}
    virtual void executeAfter(JPABaseParticle *);

    /* 0x010 */ mVec3_c field_0x010[0x32];
    /* 0x268 */ mVec3_c field_0x268[0x32];
    /* 0x4C0 */ u8 _0x4C0[0x650 - 0x4C0];
    // TODO unclear structure boundary
};

class CommonEmitterCallback : public JPAEmitterCallBack {
public:
    CommonEmitterCallback() : field_0x04(0) {}
    virtual ~CommonEmitterCallback() {}
    virtual void executeAfter(JPABaseEmitter *) override;
    void init(s32 v) {
        field_0x04 = v;
    }

private:
    /* 0x04 */ s32 field_0x04;
};

class dShpEmitter_c;

class dShpEmitterProc : public dShpProc1_c {
public:
    virtual ~dShpEmitterProc() {
        mpTransforms = nullptr;
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
    /* 0x3C */ u8 mAlpha[100];
};

class dShpEmitter_c : public JPAParticleCallBack {
public:
    virtual ~dShpEmitter_c() {}
    virtual void draw(JPABaseEmitter *, JPABaseParticle *) override;

    bool create(mHeapAllocator_c *alloc);
    void init(const char *mdlName, const char *arcName, bool priority);
    void clear();
    void remove();

private:
    /* 0x04 */ dShpEmitterProc mProc;
    /* 0xA0 */ s32 field_0xA0;
};

class dJEffManager_c : public dEmitterBase_c {
public:
    dJEffManager_c() : field_0x670(-1), field_0x67C(-1) {}
    virtual ~dJEffManager_c() {
        remove();
    }

    static bool createEffManagers();
    static void removeEffManagers();
    static void draw(const JPADrawInfo *info, u32 groupId);
    static void setupEffects();

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

private:
    void create(u16 resourceId);
    void remove();

    static mHeapAllocator_c *ms_allocator;
    static dJEffManager_c *sManagers;

    /* 0x01C */ dJEffEmitterCallback_c mCallback;
    /* 0x670 */ s32 field_0x670;
    /* 0x674 */ u8 _0x674[0x67C - 0x674];
    /* 0x674 */ s32 field_0x67C;
};

#endif
