#ifndef D_A_OBJ_BASE_H
#define D_A_OBJ_BASE_H

#include "d/a/d_a_base.h"
#include "m/types_m.h"

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
    // TODO: add the rest

public:
    // could be their own thing?
    /* 8002de40 */ static void *getOarcFile(char *oarcName, char *fileName);
    /* 8002de60 */ static void *getOarcSubEntry(char *oarcName, char *fileName);
    /* 8002de80 */ static void *getOarcResFile(char *oarcName);
    /* 8002de90 */ static void *getOarcModelFile(char *oarcName);
    /* 8002dea0 */ static void *getOarcZev(char *oarcName);
    /* 8002deb0 */ static void *getOarcDZB(char *dzbName);
    /* 8002ded0 */ static void *getOarcPLC(char *plcName);

public:
    /* 8002def0 */ dAcObjBase_c();
    /* 8002e050 */ virtual ~dAcObjBase_c();
    /* 8002e0e0 */ virtual int preCreate();
    /* 8002e150 */ virtual int preExecute();
    /* 8002e2a0 */ virtual void postExecute();
    /* 8002e330 */ virtual int preDraw();
    /* 8002e5b0 */ virtual void *getObjectListEntry();
    /* 8002e5c0 */ virtual bool restorePosRotFromCopy();
    /* 8002ea10 */ virtual bool canBeLinkedToWoodTag();
    /* 8002ea20 */ virtual bool drop();

public:
    /* 8002e630 */ static dAcObjBase_c create(fProfile::PROFILE_NAME_e actorId, u32 roomId, u32 params1, mVec3_c *pos,
            mAng3_c *rot, mVec3_c *scale, u32 params2);
    /* 8002e6d0 */ static dAcObjBase_c create(char *name, u32 roomId, u32 params1, mVec3_c *pos, mAng3_c *rot,
            mVec3_c *scale, u32 params2, u16 id, u8 viewclipId);
    /* 8002e770 */ static dAcObjBase_c *findObject(fProfile::PROFILE_NAME_e actorId, fLiNdBa_c *refList,
            dAcObjBase_c *parent);
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
    /* 8002ec70 */ void drawModelType1();
    // Disabling make Lava and other objects Disappear
    /* 8002ecd0 */ void drawModel2();
    /* 8002ed20 */ void fn_8002ed20();
    /* 8002edb0 */ void fn_8002edb0();
    /* 8002ee40 */ void drawShadow();
    /* 8002ef40 */ void createChildAttached(fProfile::PROFILE_NAME_e actorId, u32 params1, mVec3_c *pos, mAng3_c *rot,
            mVec3_c *scale, u32 params2, u32 roomId);
    /* 8002ef40 */ void createChildNonAttached(fProfile::PROFILE_NAME_e actorId, u32 params1, mVec3_c *pos,
            mAng3_c *rot, mVec3_c *scale, u32 params2, u32 roomId);
    /* 8002efa0 */ void fn_8002efa0();
    /* 8002eff0 */ bool fn_8002eff0(f32 *, s16 *, UnkCollider *, f32 *, f32 *);
    /* 8002f190 */ void createChildAttached2(fProfile::PROFILE_NAME_e actorId, u32 params1, mVec3_c *pos, mAng3_c *rot,
            mVec3_c *scale, u32 params2, u32 roomId);
    // These may not belong here
    /* 8002f1c0 */ static dAcBase_c *createActorUnkGroup3(fProfile::PROFILE_NAME_e actorId, u32 roomId, u32 params1,
            mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2);
    /* 8002f260 */ static dAcBase_c *createActorUnkGroup3(char *name, u32 roomId, u32 params1, mVec3_c *pos,
            mAng3_c *rot, mVec3_c *scale, u32 params2, u16 id, u8 viewclipId);
};

#endif
