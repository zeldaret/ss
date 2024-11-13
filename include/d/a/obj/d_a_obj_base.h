#ifndef D_A_OBJ_BASE_H
#define D_A_OBJ_BASE_H

#include "c/c_math.h"
#include "d/a/d_a_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "egg/math/eggMath.h"
#include "m/m3d/m_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "m/types_m.h"

class dAcObjBase_c;
class dBgS_Acch;

// Size: 0xA8
struct ActorCarryStruct {
    /* 0x00 */ fLiNdBa_c actorLink;
    /* 0x0C */ fLiNdBa_c *carriedActor;
    /* 0x10 */ u32 carryFlags;
    /* 0x14 */ int carryType;
    /* 0x18 */ u16 field_0x18;
    /* 0x1A */ u16 field_0x1A;
    /* 0x1C */ mVec3_c field_0x1C;
    /* 0x28 */ mMtx_c carryTransMtx;
    /* 0x58 */ mMtx_c field_0x58;
    /* 0x88 */ s32 isCarried;
    /* 0x8C */ f32 field_0x8C;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ f32 field_0x94;
    /* 0x98 */ f32 field_0x98;
    /* 0x9C */ void *dtor; // ???
    /* 0xA0 */ u32 field_0xA0;
    /* 0xA4 */ u32 field_0xA4;

    void set(u32 flags, f32 x, f32 y, f32 z, void *unk);

    // not real name, but sure
    void bushTpFunc(dBgS_Acch &);

    void fn_80050EA0(dAcObjBase_c *);
    void fn_800511E0(dAcObjBase_c *);

    bool testCarryFlag(u32 flag) {
        return (carryFlags & flag) != 0;
    }

    bool checkCarryType(int type) const {
        return (isCarried == 1 && carryType == type);
    }
};

// Ghidra has it as `unk_ActorObjectBase`
struct LightingInfo {
    /* 0x00 */ u8 mField_0x00[0x60 - 0x00];
    /* 0x60 */ u8 mLightingCode;
    /* 0x61 */ u8 mField_0x61[0x70 - 0x61];
};

// Ghidra: ActorObjectBase
//   size: 0x330
// vtable: 0x805015b8
// non-official name
class dAcObjBase_c : public dAcBase_c {
public:
    /* 0x0FC */ f32 yoffset;
    /* 0x100 */ f32 field_0x100;
    /* 0x104 */ f32 unkfloat;
    /* 0x108 */ char _1[12];
    /* 0x114 */ u16 targetFiTextId;
    /* 0x116 */ u8 unkByteTargetFiRelated;
    /* 0x118 */ mVec3_c mOldPosition;
    /* 0x124 */ mVec3_c poscopy2;
    /* 0x130 */ mVec3_c poscopy3;
    /* 0x13C */ mAng3_c angle;
    /* 0x144 */ f32 forwardSpeed;
    /* 0x148 */ f32 forwardAccel;
    /* 0x14C */ f32 forwardMaxSpeed;
    /* 0x150 */ mVec3_c velocity;
    /* 0x15C */ mMtx_c mWorldMtx;
    /* 0x18C */ cM3dGAab boundingBox;
    /* 0x1A4 */ f32 mCullingDistance;
    /* 0x1A8 */ f32 field_0x1A8;
    /* 0x1AC */ u32 mObjectActorFlags;
    /* 0x1B0 */ f32 mField_0x1B0;
    /* 0x1B4 */ mVec3_c mField_0x1B4;
    /* 0x1C0 */ cCcD_Stts mStts;
    /* 0x1FC */ mVec3_c mStartingPos;
    /* 0x208 */ mAng3_c mStartingRot;
    /* 0x210 */ ActorCarryStruct mActorCarryInfo;
    /* 0x2B8 */ u32 mField_0x2B8;
    /* 0x2BC */ LightingInfo mLightingInfo;
    /* 0x32C */ u32 mField_0x32C;

public:
    mVec3_c &GetOldPosition() {
        return mOldPosition;
    }
    mVec3_c &GetVelocity() {
        return velocity;
    }
    mAng3_c &GetAngle() {
        return angle;
    }

    f32 getVelocityMag() const {
        return fabsf(nw4r::math::VEC3LenSq(velocity));
    }

    bool isStopped() const {
        return getVelocityMag() <= EGG::Math<f32>::epsilon();
    }

    bool isSlowerThan(f32 speed) const {
        return fabsf(forwardSpeed) <= speed;
    }

    bool checkYOffsetField_0x100() const {
        return yoffset <= field_0x100;
    }

    void clearObjectProperty(u32 property) {
        mObjectActorFlags &= ~property;
    }
    void setObjectProperty(u32 property) {
        mObjectActorFlags |= property;
    }
    bool checkObjectProperty(u32 property) const {
        return mObjectActorFlags & property;
    }

    // could be their own thing?
    /* 8002de40 */ static void *getOarcFile(const char *oarcName, const char *fileName);
    /* 8002de60 */ static void *getOarcSubEntry(const char *oarcName, const char *fileName);
    /* 8002de80 */ static void *getOarcResFile(const char *oarcName);
    /* 8002de90 */ static void *getOarcModelFile(const char *oarcName);
    /* 8002dea0 */ static void *getOarcZev(const char *oarcName);
    /* 8002deb0 */ static void *getOarcDZB(const char *dzbName, const char *dzbName2);
    /* 8002ded0 */ static void *getOarcPLC(const char *plcName, const char *plcName2);

public:
    /* 8002def0 */ dAcObjBase_c();
    /* 8002e050 */ virtual ~dAcObjBase_c();
    /* 8002e0e0 */ virtual int preCreate() override;
    /* 8002e150 */ virtual int preExecute() override;
    /* 8002e2a0 */ virtual void postExecute(MAIN_STATE_e state) override;
    /* 8002e330 */ virtual int preDraw() override;
    /* 8002e5b0 */ virtual void *getObjectListEntry();
    /* 8002e5c0 */ virtual bool restorePosRotFromCopy() override;
    /* 8002ea10 */ virtual bool canBeLinkedToWoodTag();
    /* 8002ea20 */ virtual bool drop();

public:
    /* 8002e630 */ static dAcObjBase_c *create(
        fProfile::PROFILE_NAME_e actorId, u32 roomId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale,
        u32 params2
    );
    /* 8002e6d0 */ static dAcObjBase_c *create(
        char *name, u32 roomId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, u16 id,
        s8 viewclipId
    );
    /* 8002e770 */ static dAcObjBase_c *
    findObject(fProfile::PROFILE_NAME_e actorId, fLiNdBa_c *refList, dAcObjBase_c *parent);
    /* 8002e7f0 */ static dAcObjBase_c *getNextObject(fLiMgBa_c *ref, dAcObjBase_c *parent);
    /* 8002e850 */ bool isPlayer();
    /* 8002e870 */ void calcVelocity(mVec3_c &pos, f32 speed);
    /* 8002e8e0 */ void calcVelocity();
    /* 8002e9a0 */ void updateMatrix();
    /* 8002ea00 */ void setDefaultCullDistance();
    /* 8002ea30 */ void fn_8002EA30(f32, f32, f32, f32, f32, void *);
    /* 8002ea60 */ void fn_8002EA60(void *);
    /* 8002eb30 */ void fn_8002EB30(void *);
    /* 8002eb90 */ void putInODesert(f32 depth, mVec3_c *position);
    // Disabling makes Items and Link Disappear
    /* 8002ec70 */ void drawModelType1(m3d::smdl_c *smdl);
    // Disabling make Lava and other objects Disappear
    /* 8002ecd0 */ void drawModel2();
    /* 8002ed20 */ void fn_8002ed20();
    /* 8002edb0 */ void fn_8002edb0(m3d::mShadowCircle_c &shadow, m3d::smdl_c &smdl, const mQuat_c *rot, s32, s32, f32);
    /* 8002ee40 */ void drawShadow(
        m3d::mShadowCircle_c &shadow, void *, const mMtx_c &mtx, const mQuat_c *rot, s32, s32, s32, s32, s32, f32
    );
    /* 8002ef40 */ void createChildAttached(
        fProfile::PROFILE_NAME_e actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2,
        u32 roomId
    );
    /* 8002ef40 */ void createChildNonAttached(
        fProfile::PROFILE_NAME_e actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2,
        u32 roomId
    );
    /* 8002efa0 */ void fn_8002efa0();
    /* 8002eff0 */ bool fn_8002eff0(f32 *, s16 *, UnkCollider *, f32 *, f32 *);
    /* 8002f190 */ void createChildAttached2(
        fProfile::PROFILE_NAME_e actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2,
        u32 roomId
    );
    // These may not belong here
    /* 8002f1c0 */ static dAcBase_c *createActorUnkGroup3(
        fProfile::PROFILE_NAME_e actorId, u32 roomId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale,
        u32 params2
    );
    /* 8002f260 */ static dAcBase_c *createActorUnkGroup3(
        char *name, u32 roomId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, u16 id,
        u8 viewclipId
    );
};

// Actors' createHeap functions often have patterns that can be matched with this macro
#define TRY_CREATE(thing)                                                                                              \
    do {                                                                                                               \
        bool result = (thing);                                                                                         \
        if (!result)                                                                                                   \
            return result;                                                                                             \
    } while (0)
// Use this in actors' createHeap functions
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
