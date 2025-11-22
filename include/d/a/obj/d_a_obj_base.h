#ifndef D_A_OBJ_BASE_H
#define D_A_OBJ_BASE_H

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_salbage_if.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/d_jnt_col.h"
#include "d/d_light_env.h"
#include "d/d_linkage.h"
#include "egg/math/eggMath.h"
#include "m/m3d/m_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "m/types_m.h"
#include "nw4r/math/math_arithmetic.h"

// Ghidra: ActorObjectBase
//   size: 0x330
// vtable: 0x805015b8
// non-official name
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
    /* 0x0FC */ f32 mYOffset;
    /* 0x100 */ f32 field_0x100;
    /* 0x104 */ f32 unkfloat;
    /* 0x108 */ char field_0x108[12];
    /* 0x114 */ u16 mTargetFiTextID;
    /* 0x116 */ u8 mTargetFiRelated;
    /* 0x118 */ mVec3_c mOldPosition;
    /* 0x124 */ mVec3_c mPositionCopy2;
    /* 0x130 */ mVec3_c mPositionCopy3;
    /* 0x13C */ mAng3_c mAngle;
    /* 0x144 */ f32 mSpeed;
    /* 0x148 */ f32 mAcceleration;
    /* 0x14C */ f32 mMaxSpeed;
    /* 0x150 */ mVec3_c mVelocity;
    /* 0x15C */ mMtx_c mWorldMtx;
    /* 0x18C */ cM3dGAab mBoundingBox;
    /* 0x1A4 */ f32 mCullingDistance;
    /* 0x1A8 */ f32 field_0x1A8;
    /* 0x1AC */ u32 mObjectActorFlags;
    /* 0x1B0 */ f32 field_0x1B0;
    /* 0x1B4 */ mVec3_c field_0x1B4;
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
    mVec3_c &getVelocity() {
        return mVelocity;
    }
    mAng3_c &getAngle() {
        return mAngle;
    }

    f32 getVelocityMag() const {
        return fabsf(nw4r::math::VEC3LenSq(mVelocity));
    }

    f32 getSpeed() const {
        return mSpeed;
    }

    f32 getYOffset() const {
        return mYOffset;
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

    bool checkYOffsetField_0x100() const {
        return mYOffset <= field_0x100;
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

    // could be their own thing?
    static void *getOarcFile(const char *oarcName, const char *fileName);
    static void *getOarcSubEntry(const char *oarcName, const char *fileName);
    static void *getOarcResFile(const char *oarcName);
    static void *getOarcModelFile(const char *oarcName);
    static void *getOarcZev(const char *oarcName);
    static void *getOarcDZB(const char *dzbName, const char *dzbName2);
    static void *getOarcPLC(const char *plcName, const char *plcName2);

public:
    dAcObjBase_c();
    virtual ~dAcObjBase_c();
    virtual int preCreate() override;
    virtual int preExecute() override;
    virtual void postExecute(MAIN_STATE_e state) override;
    virtual int preDraw() override;
    virtual void *getObjectListEntry();
    virtual bool restorePosRotFromCopy() override;
    virtual bool canBeLinkedToWoodTag();
    virtual bool drop();

public:
    static dAcObjBase_c *create(
        fProfile::PROFILE_NAME_e actorId, u32 roomId, u32 params1, const mVec3_c *pos, const mAng3_c *rot,
        const mVec3_c *scale, u32 params2
    );
    static dAcObjBase_c *create(
        const char *name, u32 roomId, u32 params1, const mVec3_c *pos, const mAng3_c *rot, const mVec3_c *scale,
        u32 params2, u16 id, s8 viewclipId
    );
    static dAcObjBase_c *findObject(fProfile::PROFILE_NAME_e actorId, fLiNdBa_c *refList, dAcObjBase_c *parent);
    static dAcObjBase_c *getNextObject(fLiMgBa_c *ref, dAcObjBase_c *parent);
    bool isPlayer();
    void calcVelocity(mVec3_c &pos, f32 speed);
    void calcVelocity();
    void updateMatrix();
    void setDefaultCullDistance();
    void fn_8002EA30(f32, f32, f32, f32, f32, void *);
    void fn_8002EA60(void *);
    void fn_8002EB30(void *);
    void putInODesert(f32 depth, mVec3_c *position);
    // Disabling makes Items and Link Disappear
    void drawModelType1(m3d::smdl_c *smdl);
    // Disabling make Lava and other objects Disappear
    void drawModel2();
    void fn_8002ed20();
    void fn_8002edb0(m3d::mShadowCircle_c &shadow, m3d::smdl_c &smdl, const mQuat_c *rot, s32, s32, f32);
    void drawShadow(
        m3d::mShadowCircle_c &shadow, void *, const mMtx_c &mtx, const mQuat_c *rot, s32, s32, s32, s32, s32, f32
    );
    void createChildAttached(
        fProfile::PROFILE_NAME_e actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2,
        u32 roomId
    );
    void createChildNonAttached(
        fProfile::PROFILE_NAME_e actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2,
        u32 roomId
    );
    void fn_8002efa0();
    bool fn_8002eff0(const f32 &, const s16 &, cCcD_Obj *, const f32 &, const f32 &);
    void createChildAttached2(
        fProfile::PROFILE_NAME_e actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2,
        u32 roomId
    );
    // These may not belong here
    static dAcBase_c *createActorUnkGroup3(
        fProfile::PROFILE_NAME_e actorId, u32 roomId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale,
        u32 params2
    );
    static dAcBase_c *createActorUnkGroup3(
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
// Use this in actors' create functions
#define CREATE_ALLOCATOR(className)                                                                                    \
    do {                                                                                                               \
        if (!initAllocatorWork1Heap(-1, #className "::m_allocator", 0x20)) {                                           \
            return FAILED;                                                                                             \
        }                                                                                                              \
    } while (0)

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

#endif
