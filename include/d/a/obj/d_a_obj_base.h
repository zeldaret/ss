#ifndef D_A_OBJ_BASE_H
#define D_A_OBJ_BASE_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/d_jnt_col.h"
#include "d/d_light_env.h"
#include "d/d_linkage.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "m/types_m.h"

class dBgS_Acch;
class dShadowCircle_c;
class dSalvageIf_c;

class dAcObjBase_c : public dAcBase_c {
public:
    enum ObjProperties_e {
        OBJ_PROP_0x1 = (1 << 0),
        OBJ_PROP_0x2 = (1 << 1),
        OBJ_PROP_0x4 = (1 << 2),
        OBJ_PROP_0x8 = (1 << 3),
        OBJ_PROP_0x10 = (1 << 4),
        OBJ_PROP_0x20 = (1 << 5),
        OBJ_PROP_0x40 = (1 << 6),
        OBJ_PROP_0x80 = (1 << 7),
        OBJ_PROP_0x100 = (1 << 8),
        OBJ_PROP_0x200 = (1 << 9),
        OBJ_PROP_0x400 = (1 << 10),
        OBJ_PROP_0x800 = (1 << 11),
        OBJ_PROP_0x1000 = (1 << 12),
        OBJ_PROP_0x2000 = (1 << 13),
        OBJ_PROP_0x4000 = (1 << 14),
        OBJ_PROP_0x8000 = (1 << 15),
        OBJ_PROP_0x10000 = (1 << 16),
        OBJ_PROP_0x20000 = (1 << 17),
        OBJ_PROP_0x40000 = (1 << 18),
        OBJ_PROP_0x80000 = (1 << 19),
        OBJ_PROP_0x100000 = (1 << 20),
        OBJ_PROP_0x200000 = (1 << 21),
        OBJ_PROP_0x400000 = (1 << 22),
        OBJ_PROP_0x800000 = (1 << 23),
        OBJ_PROP_0x1000000 = (1 << 24),
        OBJ_PROP_0x2000000 = (1 << 25),
        OBJ_PROP_0x4000000 = (1 << 26),
        OBJ_PROP_0x8000000 = (1 << 27),
        OBJ_PROP_0x10000000 = (1 << 28),
        OBJ_PROP_0x20000000 = (1 << 29),
        OBJ_PROP_0x40000000 = (1 << 30),
        OBJ_PROP_0x80000000 = (1 << 31),
    };

public:
    /* 0x0FC */ f32 mSinkDepth;
    /* 0x100 */ f32 mDeepSinkTarget;
    /* 0x104 */ f32 mShallowSinkTarget;
    /* 0x104 */ f32 mDeepSinkRate;
    /* 0x104 */ f32 mShallowSinkRate;
    /* 0x104 */ f32 mDefaultSinkRate;
    /* 0x114 */ u16 mTargetFiTextID;
    /* 0x116 */ u8 mTargetFiRelated;
    /* 0x118 */ mVec3_c mOldPosition;
    /* 0x124 */ mVec3_c mPositionCopy2;
    /* 0x130 */ mVec3_c mPositionCopy3;
    /* 0x13C */ mAng3_c mAngle;
    /* 0x144 */ f32 mSpeed;
    /* 0x148 */ f32 mGravity;
    /* 0x14C */ f32 mMaxFallSpeed;
    /* 0x150 */ mVec3_c mVelocity;
    /* 0x15C */ mMtx_c mWorldMtx;
    /* 0x18C */ cM3dGAab mBoundingBox;
    /* 0x1A4 */ f32 mCullingDistance;
    /* 0x1A8 */ f32 field_0x1A8;
    /* 0x1AC */ u32 mObjectActorFlags;
    /* 0x1B0 */ f32 field_0x1B0;
    /* 0x1B4 */ mVec3_c mGndNormal;
    /* 0x1C0 */ cCcD_Stts mStts;
    /* 0x1FC */ mVec3_c mStartingPos;
    /* 0x208 */ mAng3_c mStartingRot;
    /* 0x210 */ dLinkage_c mLinkage;
    /* 0x2B8 */ u32 field_0x2B8;
    /* 0x2BC */ ActorLighting mLightingInfo;
    /* 0x32C */ dSalvageIf_c *mpSalvageIf;

public:
    mVec3_c &getOldPosition() {
        return mOldPosition;
    }
    void setOldPosition(const mVec3_c &v) {
        mOldPosition = v;
    }
    mVec3_c &getStartingPosition() {
        return mStartingPos;
    }
    const mVec3_c &getStartingPos() const {
        return mStartingPos;
    }
    void setStartingPosition(const mVec3_c &v) {
        mStartingPos = v;
    }
    mVec3_c &getVelocity() {
        return mVelocity;
    }
    mAng3_c &getAngle() {
        return mAngle;
    }

    void setStartingRotation(const mAng3_c &v) {
        mStartingRot = v;
    }

    f32 getVelocityMag() const {
        return fabsf(nw4r::math::VEC3LenSq(mVelocity));
    }

    f32 getSpeed() const {
        return mSpeed;
    }

    f32 getYOffset() const {
        return mSinkDepth;
    }

    bool isFullyStopped(f32 speed = EGG::Math<f32>::epsilon()) const {
        return getVelocityMag() <= speed;
    }

    bool isSpeedStopped(f32 speed = EGG::Math<f32>::epsilon()) const {
        return std::fabsf(mSpeed) <= speed;
    }

    bool isVerticalSpeedStopped(f32 speed = EGG::Math<f32>::epsilon()) const {
        return std::fabsf(mVelocity.y) <= speed;
    }

    bool checkAtDeepSinkDepth() const {
        return mSinkDepth <= mDeepSinkTarget;
    }

    void unsetObjectProperty(u32 property) {
        mObjectActorFlags &= ~property;
    }
    void setObjectProperty(u32 property) {
        mObjectActorFlags |= property;
    }
    bool checkObjectProperty(u32 property) const {
        return mObjectActorFlags & property;
    }

    dLinkage_c &getLinkage() {
        return mLinkage;
    }
    const dLinkage_c &getLinkage() const {
        return mLinkage;
    }

    void setJntCol(dJntCol_c *pCol) {
        mLinkage.field_0xA0 = pCol;
    }

    dSalvageIf_c *getSalvageIf() const {
        return mpSalvageIf;
    }

    // could be their own thing?
    static void *getOarcFile(const char *oarcName, const char *fileName);
    static void *getOarcSubEntry(const char *oarcName, const char *fileName);
    static void *getOarcResFile(const char *oarcName);
    static void *getOarcModelFile(const char *oarcName);
    static void *getOarcZev(const char *oarcName);
    static void *getOarcDZB(const char *oarcName, const char *dzbName);
    static void *getOarcPLC(const char *oarcName, const char *plcName);

public:
    dAcObjBase_c();
    virtual ~dAcObjBase_c();
    virtual int preCreate() override;
    virtual int preExecute() override;
    virtual void postExecute(MAIN_STATE_e state) override;
    virtual int preDraw() override;
    virtual fLiNdBa_c *getObjectListEntry();
    virtual bool restorePosRotFromCopy() override;
    virtual bool canBeLinkedToWoodTag();
    virtual bool drop();

public:
    static dAcObjBase_c *create(
        ProfileName actorId, u32 roomId, u32 params1, const mVec3_c *pos, const mAng3_c *rot, const mVec3_c *scale,
        u32 params2
    );
    static dAcObjBase_c *create(
        const char *name, u32 roomId, u32 params1, const mVec3_c *pos, const mAng3_c *rot, const mVec3_c *scale,
        u32 params2, u16 id, s8 viewclipId
    );

    static dAcObjBase_c *findObject(ProfileName actorId, fLiMgBa_c *refList, dAcObjBase_c *parent);
    static dAcObjBase_c *getNextObject(fLiMgBa_c *ref, dAcObjBase_c *parent);
    bool isPlayer();
    void calcVelocity(const mVec3_c &pos, f32 speed);
    void calcVelocity();
    void updateMatrix();
    void setDefaultCullDistance();
    void setSinkDepths(f32, f32, f32, f32, f32, dBgS_Acch *);
    void calcSinkDepth(const cBgS_PolyInfo &);
    void setGndNormal(const cBgS_PolyInfo &);

    bool putInODesert(f32 depth, mVec3_c *position);
    void drawModel(m3d::smdl_c *smdl);
    void drawModel(m3d::smdl_c *smdl, int lightSetIdx);
    bool drawShadowAlone(m3d::smdl_c &mdl);
    bool drawShadow(dShadowCircle_c &shadow, m3d::smdl_c &smdl, const mSphere_c &sph, s32, s32, f32);
    bool drawShadow(
        dShadowCircle_c &shadow, const GXTexObj *pTex, const mMtx_c &mtx, const mSphere_c &sph, s32 priority, s32 alpha,
        s32 red, s32 green, s32 blue, f32 height
    );
    void fn_8002efa0();
    bool fn_8002eff0(const f32 &, const s16 &, cCcD_Obj *, const f32 &, const f32 &);

    dAcObjBase_c *createChildAttached(
        ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, u32 roomId
    );
    dAcObjBase_c *createChildDetached(
        ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, u32 roomId
    );

    static dAcObjBase_c *createInsectActor(
        dAcBase_c *parent, ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2,
        s32 roomId
    );
    static dAcObjBase_c *createStageActor(
        ProfileName actorId, u32 roomId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2
    );
    static dAcObjBase_c *createStageActor(
        const char *name, u32 roomId, u32 params1, const mVec3_c *pos, const mAng3_c *rot, const mVec3_c *scale,
        u32 params2, u16 id, s8 viewclipId
    );
};

// Actors' createHeap functions often have patterns that can be matched with this macro
#define TRY_CREATE(thing)                                                                                              \
    do {                                                                                                               \
        bool result = (thing);                                                                                         \
        if (!result)                                                                                                   \
            return result;                                                                                             \
    } while (0)

// Actors' createHeap functions often have patterns that can be matched with this macro
// `bool result;` needs to be defined in scope prior to use
#define TRY_CREATE2(thing)                                                                                             \
    do {                                                                                                               \
        result = (thing);                                                                                              \
        if (!result)                                                                                                   \
            return result;                                                                                             \
    } while (0)

#define CREATE_ALLOCATOR_SIZE(className, size)                                                                         \
    do {                                                                                                               \
        if (!initAllocatorWork1Heap(size, #className "::m_allocator", 0x20)) {                                         \
            return FAILED;                                                                                             \
        }                                                                                                              \
    } while (0)

// Use this in actors' create functions
#define CREATE_ALLOCATOR(className) CREATE_ALLOCATOR_SIZE(className, -1)
#define CREATE_ALLOCATOR_UNCHECKED(className) initAllocatorWork1Heap(-1, #className "::m_allocator", 0x20)

class dAcObjRef_unk {
public:
    dAcObjRef_unk(dAcObjBase_c *ref) : mObj(nullptr), refOwner(ref) {}
    ~dAcObjRef_unk() {
        refOwner = nullptr;
    }

    bool fn_80051780(const cCcD_Obj &);
    void fn_800051630();

    void modifyMtx();

    /* 0x00 */ dAcRef_c<dAcObjBase_c> mObj;
    /* 0x0C */ u8 _0C[4];
    /* 0x10 */ dAcObjBase_c *refOwner;
    /* 0x14 */ u8 _14[0x2C - 0x14];
};

typedef dAcRef_c<dAcObjBase_c> dAcObjRef_c;

#endif
