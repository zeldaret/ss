#ifndef D_A_OBJ_BASE_H
#define D_A_OBJ_BASE_H

#include "d/a/d_a_base.h"
#include "d/cc/d_cc_d.h"
#include "m/m3d/m_shadow.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "m/types_m.h"
#include "nw4r/g3d/g3d_resfile.h"


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
    /* 0x8C */ u8 field_0x8C[0x10]; // mQuat_c
    /* 0x9C */ void *dtor;          // ???
    /* 0xA0 */ u32 field_0xA0;
    /* 0xA4 */ u32 field_0xA4;

    bool testCarryFlag(u32 flag) {
        return (carryFlags & flag) != 0;
    }
};

// Ghidra: ActorObjectBase
//   size: 0x330
// vtable: 0x805015b8
// non-official name
class dAcObjBase_c : public dAcBase_c {
public:
    // Data Members
    f32 yoffset;
    char _0[4];
    f32 unkfloat;
    char _1[12];
    u16 targetFiTextId;
    u8 unkByteTargetFiRelated;
    mVec3_c poscopy1;
    mVec3_c poscopy2;
    mVec3_c poscopy3;
    mAng3_c rotcopy1;
    f32 forwardSpeed;
    f32 forwardAccel;
    f32 forwardMaxSpeed;
    mVec3_c velocity;
    /* 0x15C */ mMtx_c worldMatrix;
    nw4r::math::AABB boundingBox;
    /* 0x1A4 */ f32 mCullingDistance;
    /* 0x1A8 */ f32 field_0x1A8;
    /* 0x1AC */ u32 mObjectActorFlags;

    u8 unk_0x1B0[0x1C0 - 0x1B0];

    /* 0x1C0 */ UnkCCDStruct mCCdStruct;

    mVec3_c mStartingPos;
    mAng3_c mStartingRot;

    /* 0x210 */ ActorCarryStruct mActorCarryInfo;

    u8 unk_0x1EC[0x330 - 0x2B8];

public:
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
        fProfile::PROFILE_NAME_e actorId, s8 roomId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale,
        u32 params2
    );
    /* 8002e6d0 */ static dAcObjBase_c *create(
        char *name, u32 roomId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, u16 id,
        s8 viewclipId
    );
    /* 8002e770 */ static dAcObjBase_c *
    findObject(fProfile::PROFILE_NAME_e actorId, fLiNdBa_c *refList, dAcObjBase_c *parent);
    /* 8002e7f0 */ static dAcObjBase_c *getNextObject(fLiNdBa_c *ref, dAcObjBase_c *parent);
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

protected:
    inline void setBoundingBox(mVec3_c min, mVec3_c max) {
        boundingBox.min = min;
        boundingBox.max = max;
    }
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

#endif
