#ifndef D_A_PLAYER_H
#define D_A_PLAYER_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_itembase.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "d/a/e/d_a_en_base.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmchrblend.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_mdl.h"
#include "m/m3d/m_smdl.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_calcworld.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/stage_render_stuff.h"

// See Below for some info
// https://github.com/lepelog/skywardsword-tools/wiki/Player-Actor

/*
 * The inheritance chain goes something like:
 *     dAcObjBase_c          straightfoward
 *          v
 *     daPlBase_c            only has a dtor, vtable (80533128) pooled into d_a_player at the end
 *          v
 *     daPlayerActBase_c     has a few strong and many weak functions, vtable stripped
 *          v
 *     daPlayerModelBase_c   has the actual implementation of many functions, vtable at 8050da00, size 0x310
 *          v
 *     dAcPy_c               "ActorLink", vtable at 80532cb0
 */

// Vtable at 80533128, no differences to dAcObjBase_c?
class daPlBase_c : public dAcObjBase_c {
public:
    virtual ~daPlBase_c() {}
};

class daPlBaseCallback1_c : m3d::callback_c {
    u8 _0x__[4];
};
class daPlBaseCallback2_c : m3d::callback_c {
    u8 _0x__[4];
};

class daPlBaseMdlCallback_c : m3d::mdl_c::mdlCallback_c {
    u8 _0x__[8];
};

class daPlBaseAnmChr_c : m3d::anmChr_c {
public:
    daPlBaseAnmChr_c() : field_0x35(0), field_0x36(0) {}

private:
    /* 0x35 */ u8 field_0x35;
    /* 0x36 */ u8 field_0x36;
};

class daPlBaseMdl_c : m3d::mdl_c {
public:
    daPlBaseMdl_c() : field_0x5C(0) {}

private:
    /* 0x24 */ daPlBaseMdlCallback_c mCallback;
    /* 0x5C */ UNKWORD field_0x5C;
};

class daPlBaseCalcWorldCallback_c : public nw4r::g3d::ICalcWorldCallback {
public:
    virtual void ExecCallbackC(
        nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl,
        nw4r::g3d::FuncObjCalcWorld *pFuncObj
    ) override; // at 0x14
};
class daPlBaseScnObjCallback_c : public nw4r::g3d::IScnObjCallback {
    u8 _0x__[0x10];
};

class UnkPlayerClass {
public:
    UnkPlayerClass() {}
    ~UnkPlayerClass() {}

private:
    /* 0x00 */ u8 _0x00[0x10 - 0x00];
};

// Does this one have a vtable?
class daPlayerActBase_c : public daPlBase_c {
public:
    daPlayerActBase_c() : field_0x334(120.0f), mCurrentAction(187) {}
    virtual ~daPlayerActBase_c() {}

    int preExecute() override;
    void postExecute(MAIN_STATE_e state) override;

    /* vt 0x080 */ virtual UNKWORD IfCurrentActionToActor(dAcBase_c *ac, UNKWORD) {
        return 0;
    }
    /* vt 0x084 */ virtual UNKWORD vt_0x084(dAcBase_c *ac, UNKWORD) {
        return 0;
    }
    /* vt 0x088 */ virtual void set0x439F(dAcBase_c *ac, UNKWORD) {}
    /* vt 0x08C */ virtual void set0x43DE() {}
    /* vt 0x090 */ virtual f32 vt_0x090() {
        return 0.0f;
    }
    /* vt 0x094 */ virtual void relatedToUsingItem0x11() {
    }
    /* vt 0x098 */ virtual void setPosRot(const mVec3_c *pos, const mAng3_c *ang, bool force, UNKWORD, UNKWORD);
    /* vt 0x09C */ virtual bool isLiftingObject() {
        return false;
    }
    /* vt 0x0A0 */ virtual bool isThrowingOrRollingItem() {
        return false;
    }
    /* vt 0x0A4 */ virtual bool canThrowObject() {
        return false;
    }
    /* vt 0x0A8 */ virtual bool canRollObject() {
        return false;
    }
    /* vt 0x0AC */ virtual UNKWORD vt_0x0AC() {
        return 0;
    }
    /* vt 0x0B0 */ virtual bool isCarryingBomb() {
        return false;
    }
    /* vt 0x0B4 */ virtual bool isPullingOutBomb() {
        return false;
    }
    /* vt 0x0B8 */ virtual bool isUsingHarp() {
        return false;
    }
    /* vt 0x0BC */ virtual void finishPlayingHarp() {}
    /* vt 0x0C0 */ virtual bool isUsingWhip() {
        return false;
    }
    /* vt 0x0C4 */ virtual bool anotherThingWithWhip() {
        return false;
    }
    /* vt 0x0C8 */ virtual bool somethingWithWHip() {
        return false;
    }
    /* vt 0x0CC */ virtual bool isWhipOnObject() {
        return false;
    }
    /* vt 0x0D0 */ virtual bool tryGrabItemWithWhip() {
        return false;
    }
    /* vt 0x0D4 */ virtual f32 someFloatWithWhip() {
        return 0.0f;
    }
    /* vt 0x0D8 */ virtual UNKWORD getItemFromClawshotsMaybe() {
        return 0;
    }
    /* vt 0x0DC */ virtual UNKWORD vt_0x0DC(dAcBase_c *, mVec3_c &) {
        return 0;
    }
    /* vt 0x0E0 */ virtual bool isUsingBomb() {
        return false;
    }
    /* vt 0x0E4 */ virtual const mVec3_c &getCenterTranslation() const {
        // unused; overridden by daPlayerModelBase_c
        return mVec3_c::Zero;
    }
    /* vt 0x0E8 */ virtual const mVec3_c &vt_0x0E8() const {
        // unused; overridden by daPlayerModelBase_c
        return mVec3_c::Zero;
    }
    /* vt 0x0EC */ virtual const mVec3_c &vt_0x0EC() const {
        // unused; overridden by daPlayerModelBase_c
        return mVec3_c::Zero;
    }
    /* vt 0x0F0 */ virtual const mVec3_c &vt_0x0F0() const {
        // unused; overridden by daPlayerModelBase_c
        return mVec3_c::Zero;
    }
    /* vt 0x0F4 */ virtual const mVec3_c &vt_0x0F4() const {
        // unused; overridden by daPlayerModelBase_c
        return mVec3_c::Zero;
    }
    /* vt 0x0F8 */ virtual mAng vt_0x0F8() {
        return 0;
    }
    /* vt 0x0FC */ virtual mAng vt_0x0FC() {
        return 0;
    }
    /* vt 0x100 */ virtual bool isCrawling() {
        return false;
    }
    /* vt 0x104 */ virtual bool isUsingSword() {
        return false;
    }
    /* vt 0x108 */ virtual bool isUsingMitts() {
        return false;
    }
    /* vt 0x10C */ virtual bool isUsingShield() {
        return false;
    }
    /* vt 0x110 */ virtual dAcEnBase_c *getTargetedActor1() const {
        return nullptr;
    }
    /* vt 0x114 */ virtual void somethingWithCarriedActorFlags() {
        // unused; overridden by daPlayerModelBase_c
    }
    /* vt 0x118 */ virtual dAcObjBase_c *getCurrentCarriedActor() {
        // unused; overridden by daPlayerModelBase_c
        return nullptr;
    }
    /* vt 0x11C */ virtual UNKWORD checkBeetleActorReferences() {
        return 0;
    }
    /* vt 0x120 */ virtual UNKWORD getBeetleActorReferencesSize() {
        return 0;
    }
    /* vt 0x124 */ virtual UNKWORD vt_0x124() {
        return 0;
    }
    /* vt 0x128 */ virtual UNKWORD vt_0x128() {
        return 0;
    }
    /* vt 0x12C */ virtual UNKWORD vt_0x12C() {
        return 0;
    }
    /* vt 0x130 */ virtual bool isBurning() {
        return 0;
    }
    /* vt 0x134 */ virtual bool isShocked() {
        return 0;
    }
    /* vt 0x138 */ virtual bool isCursed() {
        return 0;
    }
    /* vt 0x13C */ virtual bool isSmelly() {
        return 0;
    }
    /* vt 0x140 */ virtual void store2Floats0x4500() {}
    /* vt 0x144 */ virtual bool setActorLinkToSomething() {
        return true;
    }
    /* vt 0x148 */ virtual void applyDamageWithIFrames() {}
    /* vt 0x14C */ virtual void vt_0x14C() {}
    /* vt 0x150 */ virtual bool hasLessThanQuarterHealth(bool) {
        return false;
    }
    /* vt 0x154 */ virtual UNKWORD vt_0x154() {
        return 0;
    }
    /* vt 0x158 */ virtual UNKWORD vt_0x158() {
        return 0;
    }
    /* vt 0x15C */ virtual UNKWORD vt_0x15C() {
        return 0;
    }
    /* vt 0x160 */ virtual f32 getYPos() {
        return position.y;
    }
    /* vt 0x164 */ virtual f32 vt_0x164() {
        return position.y;
    }
    /* vt 0x168 */ virtual f32 getCurrentAnimFrame() const {
        return 0.0f;
    }
    /* vt 0x16C */ virtual f32 getFloatIfStruggling() {
        return 0.0f;
    }
    /* vt 0x170 */ virtual bool isSwimming() {
        return false;
    }
    /* vt 0x174 */ virtual bool isGustBellowsBeingBlown() {
        return false;
    }
    /* vt 0x178 */ virtual const mAng3_c &getPointedAngle() const {
        return rotation;
    }
    /* vt 0x17C */ virtual const mVec3_c &getBellowsPosOrOtherVec3F() const {
        return position;
    }
    /* vt 0x180 */ virtual void setWindMillPos() {}
    /* vt 0x184 */ virtual bool isOffeset0x435eEqual0x20() {
        return 0;
    }
    /* vt 0x188 */ virtual const mVec3_c &getVec3F_Z() const {
        return mVec3_c::Ez;
    }
    /* vt 0x18C */ virtual void getBodyMtx(mMtx_c *out_mtx, int boneIdx) {
        // unused; overridden by daPlayerModelBase_c
    }
    /* vt 0x190 */ virtual void getSheathModelMatrix() {
        // unused; overridden by daPlayerModelBase_c
    }
    /* vt 0x194 */ virtual void getSwordModelMatrix() {
        // unused; overridden by daPlayerModelBase_c
    }
    /* vt 0x198 */ virtual void vt_0x198() {
        // unused; overridden by daPlayerModelBase_c
    }
    /* vt 0x19C */ virtual void vt_0x19C() {
        // unused; overridden by daPlayerModelBase_c
    }
    /* vt 0x1A0 */ virtual f32 vt_0x1A0() {
        return 0.0f;
    }
    /* vt 0x1A4 */ virtual const mVec3_c &vt_0x1A4() {
        return poscopy3;
    }
    /* vt 0x1A8 */ virtual f32 vt_0x1A8() {
        return yoffset;
    }
    /* vt 0x1AC */ virtual f32 vt_0x1AC() {
        return 0.0f;
    }
    /* vt 0x1B0 */ virtual bool isItemFairyFromBugnet() {
        return false;
    }
    /* vt 0x1B4 */ virtual bool isByte0x434eEqual7() {
        return false;
    }
    /* vt 0x1B8 */ virtual void canHandleGameOver() {}
    /* vt 0x1BC */ virtual UNKWORD vt_0x1BC() {
        return 0;
    }
    /* vt 0x1C0 */ virtual void *vt_0x1C0() const {
        return nullptr;
    }
    /* vt 0x1C4 */ virtual UNKWORD getActorInActorRef1() {
        return 0;
    }
    /* vt 0x1C8 */ virtual bool doesActorRef1Exist() {
        return false;
    }
    /* vt 0x1CC */ virtual void unlinkActorRef1() {}
    /* vt 0x1D0 */ virtual bool ifHasHealthAndSomethingElse() {
        return false;
    }
    /* vt 0x1D4 */ virtual bool isChargingWeapon() {
        return false;
    }
    /* vt 0x1D8 */ virtual bool isWeaponChargedFully() {
        return false;
    }
    /* vt 0x1DC */ virtual bool didShootBow() {
        return true;
    }
    /* vt 0x1E0 */ virtual UNKWORD vt_0x1E0() {
        return 0;
    }
    /* vt 0x1E4 */ virtual dAcObjBase_c *checkRefArrayAtIndex() {
        return nullptr;
    }
    /* vt 0x1E8 */ virtual s32 sizeOfActorRefs_16() {
        return 0;
    }
    /* vt 0x1EC */ virtual bool vt_0x1EC() {
        return true;
    }
    /* vt 0x1F0 */ virtual UNKWORD vt_0x1F0() {
        return 0;
    }
    /* vt 0x1F4 */ virtual UNKWORD didDiveSky_and_other() {
        return 0;
    }
    /* vt 0x1F8 */ virtual UNKWORD didDiveSky_andNotOther() {
        return 0;
    }
    /* vt 0x1FC */ virtual UNKWORD isLandingFromSailcloth() {
        return 0;
    }
    /* vt 0x200 */ virtual bool isBeingPulledFromRightClawshot() {
        return false;
    }
    /* vt 0x204 */ virtual bool isBeingPulledByLeftClawshot() {
        return false;
    }
    /* vt 0x208 */ virtual bool isDashing() {
        return false;
    }
    /* vt 0x20C */ virtual bool isSliding() {
        return false;
    }
    /* vt 0x210 */ virtual bool isSlding2() {
        return false;
    }
    /* vt 0x214 */ virtual UNKWORD vt_0x214() {
        return 0;
    }
    /* vt 0x218 */ virtual bool isinMpPosePower() {
        return false;
    }
    /* vt 0x21C */ virtual bool isPushingBlock() {
        return false;
    }
    /* vt 0x220 */ virtual dAcObjBase_c *getRidingActor() {
        return nullptr;
    }
    /* vt 0x224 */ virtual bool isRidingBall() const {
        return false;
    }
    /* vt 0x228 */ virtual dAcObjBase_c *getRidingMinecartActor() const {
        return 0;
    }
    /* vt 0x22C */ virtual UNKWORD getTurnableSwitch() {
        return 0;
    }
    /* vt 0x230 */ virtual UNKWORD vt_0x230() {
        return 0;
    }
    /* vt 0x234 */ virtual UNKWORD vt_0x234() {
        return 0;
    }
    /* vt 0x238 */ virtual void setVecIfInFreeFall() {}
    /* vt 0x23C */ virtual void setActorRef10() {
        
    }
    /* vt 0x240 */ virtual f32 vt_0x240() {
        return 0.0f;
    }
    /* vt 0x244 */ virtual UNKWORD vt_0x244() {
        return 0;
    }
    /* vt 0x248 */ virtual UNKWORD vt_0x248() {
        return 0;
    }
    /* vt 0x24C */ virtual UNKWORD vt_0x24C() {
        return 0;
    }
    /* vt 0x250 */ virtual f32 vt_0x250() {
        return 1.0f;
    }
    /* vt 0x254 */ virtual bool isAttackingUnderground() {
        return false;
    }
    /* vt 0x258 */ virtual mAng vt_0x258() const {
        return 0;
    }
    /* vt 0x25C */ virtual UNKWORD vt_0x25C() {
        return 0;
    }
    /* vt 0x260 */ virtual f32 vt_0x260() const {
        return 1.0f;
    }
    /* vt 0x264 */ virtual UNKWORD getGroosenatorIfIn() {
        return 0;
    }
    /* vt 0x268 */ virtual void setActorRef11() {}
    /* vt 0x26C */ virtual bool doesGameOver() {
        return true;
    }
    /* vt 0x270 */ virtual void triggerExitRelated() {}
    /* vt 0x274 */ virtual mAng vt_0x274() {
        return rotation.y;
    }
    /* vt 0x278 */ virtual const mVec3_c &vt_0x278() const {
        return position;
    }
    /* vt 0x27C */ virtual const mVec3_c &getSwordPos() const {
        return position;
    }
    /* vt 0x280 */ virtual UNKWORD vt_0x280() {
        return 0;
    }
    /* vt 0x284 */ virtual bool isUsingBugnet() const {
        return false;
    }
    /* vt 0x288 */ virtual const mVec3_c &getBugNetPos() const {
        return position;
    }
    /* vt 0x28C */ virtual bool isUsingBugnet1() {
        return false;
    }
    /* vt 0x290 */ virtual UNKWORD bugNetCollectTreasure(u32 itemId) {
        return 0;
    }
    /* vt 0x294 */ virtual mAng somethingSwitchDials() {
        return 0;
    }
    /* vt 0x298 */ virtual UNKWORD vt_0x298() {
        return 0;
    }
    /* vt 0x29C */ virtual UNKWORD getDieLargeDamageDir() {
        return 0;
    }
    /* vt 0x2A0 */ virtual void doGameOver() {}
    /* vt 0x2A4 */ virtual bool isRunningUpWall() {
        return false;
    }
    /* vt 0x2A8 */ virtual void lookTowardItem() {}
    /* vt 0x2AC */ virtual void vt_0x2AC() {}
    /* vt 0x2B0 */ virtual void vt_0x2B0() {}
    /* vt 0x2B4 */ virtual void triggerMoveEventMaybe() {}
    /* vt 0x2B8 */ virtual void setActorRef9() {}
    /* vt 0x2BC */ virtual void unlinkActorRef9() {}
    /* vt 0x2C0 */ virtual bool vt_0x2C0() {
        return true;
    }
    /* vt 0x2C4 */ virtual bool vt_0x2C4() {
        return true;
    }
    /* vt 0x2C8 */ virtual bool isRecovering() const {
        return false;
    }
    /* vt 0x2CC */ virtual bool isDrinkingPotion() {
        return false;
    }
    /* vt 0x2D0 */ virtual bool isHealingShield() {
        return false;
    }
    /* vt 0x2D4 */ virtual bool isBottleOut() {
        return false;
    }
    /* vt 0x2D8 */ virtual bool isUsingBottle() {
        return false;
    }
    /* vt 0x2DC */ virtual f32 getShieldRegenProgressMaybe() {
        return 0.0f;
    }

    enum dAcPy_RidingActor {
        RIDING_NONE = 0,
        RIDING_TRUCK_MINECART = 1,
        RIDING_LOFTWING = 2,
        RIDING_BOAT = 3,
    };

    enum PlayerAttackDirection_e {
        ATTACK_NONE = 0,
        ATTACK_SWING_LEFT = 1,
        ATTACK_SWING_RIGHT = 2,
        ATTACK_SWING_DOWN = 3,
        ATTACK_SWING_UP = 4,
        ATTACK_STAB = 5,
        ATTACK_SPIN_RIGHT = 6,
        ATTACK_SPIN_LEFT = 7,
        ATTACK_SPIN_DOWN = 8,
        ATTACK_SPIN_UP = 9,
        ATTACK_JUMP_SLASH = 10,
        ATTACK_FINAL_BLOW = 11,
    };

    static bool isInEvent();
    bool isAttackingLeft() const;
    bool isAttackingRight() const;
    bool isAttackingDown() const;
    bool isAttackingUp() const;
    bool isAttackingStab() const;
    bool isAttackingSpinHorizontal() const;
    bool isAttackingSpinVertical() const;
    void setBonkRelatedAnimFlag(bool b);
    void setPosYRot(const mVec3_c &pos, mAng rot, bool force, UNKWORD, UNKWORD);
    void setTransform(const mMtx_c &mtx, bool force, UNKWORD, UNKWORD);
    bool someTargetedActorCheck() const;
    static mAng fn_8005BA90();
    static mAng fn_8005BAA0();

    static const mColor &getEarringsColor();

    static mVec3_c sPos1;
    static mVec3_c sPos2;
    static mAng sRot1;
    static mAng sRot2;
    static const f32 sPushRelatedConstant;

protected:
    /* 0x330 */ u8 unk_0x330[0x334 - 0x330];
    /* 0x334 */ f32 field_0x334;
    /* 0x338 */ u8 mAttackDirection;
    /* 0x339 */ u8 mRidingActorType;
    /* 0x33A */ u8 unk_0x33A[0x340 - 0x33A];
    /* 0x340 */ u32 someFlags_0x340;
    /* 0x344 */ u8 unk_0x344[0x348 - 0x344];
    /* 0x348 */ u32 mSwordAndMoreStates;
    /* 0x34C */ u8 unk_0x34C[0x350 - 0x34C];
    /* 0x350 */ u32 someFlags_0x350;
    /* 0x354 */ u32 someFlags_0x354;
    /* 0x358 */ u32 someFlags_0x358;
    /* 0x35C */ u32 mForceOrPreventActionFlags;
    /* 0x360 */ u32 field_0x360;
    /* 0x364 */ u32 mActionFlags;
    /* 0x368 */ u32 mActionFlagsCont;
    /* 0x36C */ int mCurrentAction; // TODO (Document Enum)
};

// Vtable at 8050da00, dtor reveals two superclasses
// until dAcObjBase_c
class daPlayerModelBase_c : public daPlayerActBase_c {
public:
    daPlayerModelBase_c()
        : mSwordRes(nullptr),
          mAlinkRes(nullptr),
          mPlCommonAnimeRes(nullptr),
          mPlCommonSoundRes(nullptr),
          mCurrentRes(nullptr),
          mAlink2Res(nullptr),
          mScnCallback1(this),
          mScnCallback2(this),
          mScnCallback3(this),
          mScnCallback4(this),
          mScnCallback5(this) {}
    virtual ~daPlayerModelBase_c() {}

    // These Flags are named under general guesses. Very likely may need some changes
    // Correspond to mActionFlags
    enum dAcPy_FLG0 {
        FLG0_ON_FOOT = 0x80000000,
        FLG0_USE_ITEM_0 = 0x40000000,
        FLG0_USE_ITEM_1 = 0x20000000,
        FLG0_UNK_GEN_MOVE = 0x10000000,
        FLG0_ATTACK_MOVE = 0x8000000,
        FLG0_USE_ITEM_2 = 0x4000000,
        FLG0_RECOVER_ELEC = 0x2000000,
        FLG0_CRAWLING = 0x1000000,
        FLG0_TIGHT_ROPE = 0x800000,
        FLG0_SWING_ROPE = 0x400000,
        FLG0_PUSHPULL = 0x200000,
        FLG0_UNK_100000 = 0x100000,
        FLG0_UNK_80000 = 0x80000,
        FLG0_IN_WATER = 0x40000,
        FLG0_ON_LADDDER = 0x20000,
        FLG0_ON_VINES = 0x10000,
        FLG0_UNK_0x8000 = 0x8000,
        FLG0_UNK_0x4000 = 0x4000,
        FLG0_INTERATION = 0x2000,
        FLG0_BASIC_ACTION = 0x1000,
        FLG0_HANGING_ITEM = 0x800,
        FLG0_EXTERN_CONTROL = 0x400,
        FLG0_ATTACK_RELATED = 0x200,
        FLG0_UNK_0x100 = 0x100,
        FLG0_UNK_0x80 = 0x80,
        FLG0_HANGING_LEDGE = 0x40,
        FLG0_UNK_0x20 = 0x20,
        FLG0_UNK_0x10 = 0x10,
        FLG0_DAMAGED = 0x8,
        FLG0_UNK_0x4 = 0x4,
        FLG0_IN_AIR = 0x2,
        FLG0_STATIONARY = 0x1,

        FLG0_FLY = FLG0_TIGHT_ROPE | FLG0_SWING_ROPE | FLG0_IN_WATER | FLG0_ON_LADDDER | FLG0_ON_VINES |
                   FLG0_HANGING_ITEM | FLG0_HANGING_LEDGE | FLG0_UNK_0x10 | FLG0_IN_AIR,
    };

    void fn_8005ED60();
    void fn_8005EAC0();
    void fn_8005F890();
    void fn_80061410();

    /* vt 0x114 */ virtual void somethingWithCarriedActorFlags() override {
        // TODO
    }
    /* vt 0x118 */ virtual dAcObjBase_c *getCurrentCarriedActor() override {
        // TODO
        return nullptr;
    }

    /* vt 0x0E4 */ virtual const mVec3_c &getCenterTranslation() const override {
        // TODO
        return mVec3_c::Zero;
    }
    /* vt 0x0E8 */ virtual const mVec3_c &vt_0x0E8() const override {
        return field_0x12F4;
    }
    /* vt 0x0EC */ virtual const mVec3_c &vt_0x0EC() const override {
        // TODO
        return mVec3_c::Zero;
    }
    /* vt 0x0F0 */ virtual const mVec3_c &vt_0x0F0() const override {
        // TODO
        return mVec3_c::Zero;
    }
    /* vt 0x0F4 */ virtual const mVec3_c &vt_0x0F4() const override {
        // TODO
        return mVec3_c::Zero;
    }

    /* vt 0x2E0 */ virtual void vt_0x2E0() {}
    /* vt 0x2E4 */ virtual void someDivingHrabbingStuff() {}
    /* vt 0x2E8 */ virtual bool vt_0x2E8() {
        return true;
    }
    /* vt 0x2EC */ virtual void isOnClawTargetMaybe(UNKWORD, mAng &a1, mAng &a2) {
        a1.setR(0);
        a2.setR(0);
    }
    /* vt 0x2F0 */ virtual bool isMPPose() {
        return false;
    }
    /* vt 0x2F4 */ virtual bool isOnTightRope() {
        return false;
    }
    /* vt 0x2F8 */ virtual bool isOnVines() {
        return false;
    }
    /* vt 0x2FC */ virtual bool alwaysRet0() {
        return 0;
    }
    /* vt 0x300 */ virtual bool canBlockAttack() {
        return false;
    }
    /* vt 0x304 */ virtual const mVec3_c &vt_0x304() const {
        return mVec3_c::Zero;
    }
    /* vt 0x308 */ virtual mAng vt_0x308() {
        return 0;
    }
    /* vt 0x30C */ virtual void vt_0x30C(mVec3_c &res) {
        res = mVec3_c::Ex;
    }

protected:
    /* 0x370 */ mHeapAllocator_c mModelAllocator;
    /* 0x38C */ mHeapAllocator_c mSwordAllocator;
    /* 0x3A8 */ mHeapAllocator_c mShieldAllocator;
    /* 0x3C4 */ u32 mModelUpdateFlags;
    /* 0x3C8 */ nw4r::g3d::ResFile mSwordRes;
    /* 0x3CC */ nw4r::g3d::ResFile mAlinkRes;
    /* 0x3D0 */ nw4r::g3d::ResFile mPlCommonAnimeRes;
    /* 0x3D4 */ nw4r::g3d::ResFile mPlCommonSoundRes;
    /* 0x3D8 */ nw4r::g3d::ResFile mCurrentRes;
    /* 0x3DC */ nw4r::g3d::ResFile mAlink2Res;
    /* 0x3E0 */ daPlBaseMdl_c mMainMdl;
    /* 0x440 */ daPlBaseCallback1_c mCallback1;
    /* 0x448 */ m3d::mdl_c mHeadMdl;
    /* 0x46C */ daPlBaseCallback2_c mCallback2;
    /* 0x474 */ m3d::smdl_c mFaceMdl;
    /* 0x490 */ m3d::anmChr_c mFaceAnmChr;
    /* 0x4C8 */ u8 _0x4C8[4];
    /* 0x4CC */ m3d::anmTexPat_c mFaceTexPat;
    /* 0x4F8 */ u8 _0x4F8[4];
    /* 0x4FC */ m3d::anmTexSrt_c mFaceTexSrt;
    /* 0x528 */ u8 _0x528[4];
    /* 0x52C */ m3d::anmChr_c mHeadAnmChr;
    /* 0x564 */ m3d::smdl_c mHandsMdl;
    /* 0x580 */ daPlBaseCalcWorldCallback_c mCalcWorldCallback;
    /* 0x588 */ daPlBaseScnObjCallback_c mScnObjCallback;
    /* 0x598 */ m3d::smdl_c mSwordMdl;
    /* 0x5B4 */ m3d::anmMatClr_c mAnmMatClr;
    /* 0x5E0 */ m3d::smdl_c mShieldMdl;
    /* 0x5FC */ m3d::anmTexPat_c mUnkAnmTextPat1;
    /* 0x628 */ m3d::anmTexPat_c mUnkAnmTextPat2;
    /* 0x654 */ m3d::smdl_c mSheathMdl;
    /* 0x670 */ m3d::smdl_c mUnkMdl1;
    /* 0x68C */ m3d::anmChr_c mAnmChr;
    /* 0x6C4 */ m3d::anmChrBlend_c mAnmChrBlend;
    /* 0x6EC */ dScnCallback_c mScnCallback1;
    /* 0x6F8 */ dScnCallback_c mScnCallback2;
    /* 0x704 */ dScnCallback_c mScnCallback3;
    /* 0x610 */ dScnCallback_c mScnCallback4;
    /* 0x61C */ dScnCallback_c mScnCallback5;
    /* 0x728 */ daPlBaseAnmChr_c mAnmChrs[6];
    /* 0x878 */ dCcD_Cyl mCcCyls[3];
    /* 0xC68 */ dCcD_Cps mCcCpss[3];
    /* 0x10B8 */ dCcD_Cyl mCcCyl;
    /* 0x1208 */ u8 _0x1208[0xA4];
    /* 0x12AC */ mVec3_c mTranslationHand[2];
    /* 0x12C4 */ mVec3_c mTranslationWeapon[2];
    /* 0x12DC */ mVec3_c mToeTranslation[2];
    /* 0x12F4 */ mVec3_c field_0x12F4;
    /* 0x1300 */ UnkPlayerClass mUnk_0x1300[4];
    /* 0x1340 */ mQuat_c mQuat1;
    /* 0x1350 */ mQuat_c mQuat2;
    /* 0x1360 */ dAcRef_c<dAcObjBase_c> mRef; // not sure about the class
};

// The split between daPlBase_c and dAcPy_c is somewhere after
// member offset 0x1368 and before 0x137C

class dAcPy_c : public daPlayerModelBase_c {
public:
    dAcPy_c();
    virtual ~dAcPy_c();

    // argument is always false in existing code, true doesn't seem to make a difference
    void bonk(bool unk = false);
    bool fn_80202D90(bool);

    /* overrides omitted */
    /* vt 0x310 */ virtual void vt_0x310();
    /* vt 0x314 */ virtual void vt_0x314();
    /* vt 0x318 */ virtual void vt_0x318();

protected:
    /* 0x136C */ u8 _0x136C[0x4564 - 0x136C];
    /* 0x4564 */ f32 field_0x4564;

public:
    f32 getField_0x4564() const {
        return field_0x4564;
    }

    f32 getAnmMatClrFrame() const {
        return mAnmMatClr.getFrame(0);
    }

    nw4r::g3d::ResFile getSwordResFile() const {
        return mSwordRes;
    }

    inline bool checkFlags0x340(u32 mask) const {
        return (someFlags_0x340 & mask) != 0;
    }

    inline bool checkFlags0x350(u32 mask) const {
        return (someFlags_0x350 & mask) != 0;
    }

    inline void onFlags_0x360(u32 mask) {
        field_0x360 |= mask;
    }

    inline void onForceOrPreventActionFlags(u32 mask) {
        mForceOrPreventActionFlags |= mask;
    }

    bool CheckPlayerFly() const {
        return mActionFlags & FLG0_FLY;
    }

    int getCurrentAction() const {
        return mCurrentAction;
    }

    bool checkCurrentAction(int action) const {
        return mCurrentAction == action;
    }

    bool isSittingOrUnk0xAE() const {
        return mCurrentAction == 0xAD || mCurrentAction == 0xAE;
    }

    inline bool checkSwordAndMoreStates(u32 mask) const {
        return (mSwordAndMoreStates & mask) != 0;
    }

    inline bool checkActionFlags(u32 mask) const {
        return (mActionFlags & mask) != 0;
    }
    inline bool checkActionFlagsCont(u32 mask) const {
        return (mActionFlagsCont & mask) != 0;
    }

    inline u8 getRidingActorType() const {
        return mRidingActorType;
    }

    inline bool hasvt_0x1C0() const {
        return vt_0x1C0() != nullptr;
    }

    inline bool isAffectedByStaminaPotion() const {
        return FileManager::GetInstance()->hasStaminaPotionNormal() && getRidingActorType() != RIDING_BOAT;
    }

    inline bool isAffectedByStaminaPotionPlus() const {
        return FileManager::GetInstance()->hasStaminaPotionPlus() && getRidingActorType() != RIDING_BOAT;
    }

    static nw4r::g3d::ResFile getItemResFile(const char *name, mAllocator_c &allocator);

    static const dAcPy_c *GetLink() {
        return LINK;
    }

    static dAcPy_c *GetLink2() {
        return LINK2;
    }

    static dAcPy_c *LINK;
    static dAcPy_c *LINK2;

    static s32 getCurrentBowType();
    static s32 getCurrentSlingshotType();
    static s32 getCurrentBeetleType();
    static s32 getCurrentBugNetType();
    static s32 getCurrentlyEquippedShieldType();

    static bool isInBambooCuttingMinigame();

    static bool isItemRestrictedByBokoBase(ITEM_ID item);

    static u32 getCurrentHealthCapacity();

    bool canDowseProbably() const;

    mAng fn_8023B520() const;
    mAng fn_8023B590() const;
};

#endif
