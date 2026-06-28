#ifndef D_A_NPC_BEE_H
#define D_A_NPC_BEE_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/e/d_a_en_base.h"
#include "d/a/npc/d_a_npc_honeycomb.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_cc.h"
#include "m/m3d/m_proc.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"

class dAcOSpore_c;

class dAcNpcBeeSingleBee {
public:
    enum State {
        BEE_STATE_DEAD = 0,
        BEE_STATE_CHASE = 1,
        BEE_STATE_FLY_TO_HIVE = 2,
        BEE_STATE_FLY_UP = 3,
        BEE_STATE_BLOWN_AWAY = 4,
        BEE_STATE_RISE = 5,
        BEE_STATE_CRAWL = 0xA,
    };
    /* 0x000 */ u8 mIndex;
    /* 0x001 */ u8 mBeeState; // see State enum
    /* 0x002 */ u8 mActionTimer;
    /* 0x003 */ u8 mIsFlying;
    /* 0x004 */ u8 field_0x004; // unused
    /* 0x005 */ u8 mSubState;
    /* 0x006 */ u8 mDisableAttackTimer;
    /* 0x007 */ u8 mSwordDodgeTimer;
    /* 0x008 */ u8 mHide;
    /* 0x009 */ u8 mAttackTimer;
    /* 0x00A */ u8 mIsAttacking;
    /* 0x00B */ u8 mWallReflectTimer;
    /* 0x00C */ s16 mTimer; // only counts up, used for kinda random calculations
    /* 0x00E */ s16 mFlyingStatesTimer; // used for vanishing after flying up and something else
    /* 0x010 */ f32 mCrawlingSpeed;
    /* 0x014 */ f32 mSpeed;
    /* 0x018 */ mVec3_c mPos;
    /* 0x024 */ mVec3_c field_0x024;
    /* 0x030 */ mVec3_c mSwordDodgeOffset;
    /* 0x03C */ mVec3_c mOldPos;
    /* 0x048 */ mVec3_c field_0x048;
    /* 0x054 */ mVec3_c mTargetPos; // Either an absolute target or relative to the attacked actor
    /* 0x060 */ mAng3_c mCrawlingRotation;
    /* 0x066 */ mAng3_c mModelRotation;
    /* 0x06C */ dCcD_Linked_Sph mCollider;
};


class dAcNpcBee_c : public dAcEnBase_c {
public:
    dAcNpcBee_c(): mRes(nullptr) {}
    virtual ~dAcNpcBee_c() {}
    virtual bool createHeap() override;
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;
    u32 updateSwarmBeeColliders();
    void actuallyUpdateSwarmBees();
    void handleBeeCrawlingOnHive(dAcNpcBeeSingleBee* bee);
    void handleBeeFlyingStates(dAcNpcBeeSingleBee* bee);
    void handleBeeRising(dAcNpcBeeSingleBee* bee);
    s32 beeCheckWallFloorCollision(dAcNpcBeeSingleBee* bee);
    void handleBeeBlownAway(dAcNpcBeeSingleBee* bee);

private:
    /* 0x0378 */ u8 mSwarmBeeCount;
    /* 0x0379 */ u8 mRumbleTimer; // rumble timer
    /* 0x037A */ u8 mSceneflag;
    /* 0x037B */ u8 field_0x037B; // padding
    /* 0x037C */ s16 mFrameCounter; // used to only search for new targets every 16 frames
    /* 0x037E */ u8 field_0x037E[2]; // padding
    /* 0x0380 */ f32 mAttackActorDistFromHome;
    /* 0x0384 */ u8 field_0x0384[4]; // padding?
    /* 0x0388 */ nw4r::g3d::ResFile mRes;
    /* 0x038C */ dShpProc1_c mBeeModels[4];
    /* 0x045C */ dAcNpcBeeSingleBee mBees[100];
    /* 0xB80C */ dAcRef_c<dAcNpcHc_c> mHiveRef;
    /* 0xB818 */ dAcRef_c<dAcObjBase_c> mAttackRef;
    /* 0xB824 */ dColliderLinkedList mColliderList;
    /* 0xB830 */ mVec3_c mAliveBeePos; // overwritten by all alive bees, so it's the last on in the list
    /* 0xB83C */ dAcObjBase_c* mAttackActor;
    /* 0xB840 */ dAcOSpore_c* mSporeActor;
};

#endif
