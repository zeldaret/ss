#ifndef D_A_PLAYER_ACT_H
#define D_A_PLAYER_ACT_H

#include "d/d_player_base.h"
#include "m/m_vec.h"

class dAcEnBase_c;
class dAcOFairy_c;

// Does this one have a vtable?
// Name unknown
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
        // TODO obvious hack, not sure
        delete (daPlBase_c *)0;
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
    /* vt 0x0C0 */ virtual bool isUsingWhip() const {
        return false;
    }
    /* vt 0x0C4 */ virtual const mVec3_c *anotherThingWithWhip() {
        return nullptr;
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
    /* vt 0x11C */ virtual dAcObjBase_c *checkBeetleActorReferences(s32) {
        return nullptr;
    }
    /* vt 0x120 */ virtual s32 getBeetleActorReferencesSize() {
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
        return mPosition.y;
    }
    /* vt 0x164 */ virtual f32 vt_0x164() {
        return mPosition.y;
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
        return mRotation;
    }
    /* vt 0x17C */ virtual const mVec3_c &getBellowsPosOrOtherVec3F() const {
        return mPosition;
    }
    /* vt 0x180 */ virtual void setWindMillPos(mVec3_c *position) const {}
    /* vt 0x184 */ virtual bool isOffeset0x435eEqual0x20() {
        return 0;
    }
    /* vt 0x188 */ virtual const mVec3_c &getVec3F_Z() const {
        return mVec3_c::Ez;
    }
    /* vt 0x18C */ virtual void getBodyMtx(mMtx_c *out_mtx, int boneIdx) {
        // unused; overridden by daPlayerModelBase_c
    }
    /* vt 0x190 */ virtual void getSheathModelMatrix(mMtx_c *out_mtx) {
        // unused; overridden by daPlayerModelBase_c
    }
    /* vt 0x194 */ virtual void getSwordModelMatrix(mMtx_c *out_mtx) {
        // unused; overridden by daPlayerModelBase_c
    }
    /* vt 0x198 */ virtual mAng vt_0x198() {
        // unused; overridden by daPlayerModelBase_c
        return 0;
    }
    /* vt 0x19C */ virtual mAng vt_0x19C() {
        // unused; overridden by daPlayerModelBase_c
        return 0;
    }
    /* vt 0x1A0 */ virtual f32 vt_0x1A0() {
        return 0.0f;
    }
    /* vt 0x1A4 */ virtual const mVec3_c &vt_0x1A4() {
        return mPositionCopy3;
    }
    /* vt 0x1A8 */ virtual f32 vt_0x1A8() {
        return mYOffset;
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
    /* vt 0x23C */ virtual void setActorRef10() {}
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
        return mRotation.y;
    }
    /* vt 0x278 */ virtual const mVec3_c &vt_0x278() const {
        return mPosition;
    }
    /* vt 0x27C */ virtual const mVec3_c &getSwordPos() const {
        return mPosition;
    }
    /* vt 0x280 */ virtual UNKWORD vt_0x280() {
        return 0;
    }
    /* vt 0x284 */ virtual bool isUsingBugnet() const {
        return false;
    }
    /* vt 0x288 */ virtual const mVec3_c &getBugNetPos() const {
        return mPosition;
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
    /* vt 0x2B4 */ virtual void triggerMoveEventMaybe(u32, u32, u32, const mVec3_c *, const mAng&, u32, u32) {}
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
    /* vt 0x2D8 */ virtual bool isUsingBottle() const {
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

    enum SpecificPlayerAttackDirection_e {
        /* 0x000 */ ATTACK_DIRECTION_NONE = 0,
        /* 0x001 */ ATTACK_DIRECTION_DOWN = 1 << 0,
        /* 0x002 */ ATTACK_DIRECTION_DOWNRIGHT = 1 << 1,
        /* 0x004 */ ATTACK_DIRECTION_RIGHT = 1 << 2,
        /* 0x008 */ ATTACK_DIRECTION_UPRIGHT = 1 << 3,
        /* 0x010 */ ATTACK_DIRECTION_UP = 1 << 4,
        /* 0x020 */ ATTACK_DIRECTION_UPLEFT = 1 << 5,
        /* 0x040 */ ATTACK_DIRECTION_LEFT = 1 << 6,
        /* 0x080 */ ATTACK_DIRECTION_DOWNLEFT = 1 << 7,
        /* 0x100 */ ATTACK_DIRECTION_STAB = 1 << 8,
    };

    enum ModelUpdateFlags_e {
        UPDATE_MODEL_SWORD = 0x20,
        UPDATE_MODEL_SHIELD = 0x40,

        UPDATE_MODEL_BEETLE = 0x10000000,
    };

    enum SwordAndMoreStates_e {
        NO_SHIETH = (1 << 0),
        POUCH = (1 << 1),
        SWORD_AWAY = (1 << 2),
        FLASH_RED = (1 << 4),
        DEPLETE_STAMINA = (1 << 5),
        SPRINTING = (1 << 6),
        B_WHEEL_OPEN = (1 << 7),
        POUCH_OPEN = (1 << 8),
        TRYING_TO_USE_STAMINA_NOT_ABLE = (1 << 9),
        PUTTING_AWAY_SAILCLOTH = (1 << 10),
        GRAB_LEDGE_FROM_UNDER = (1 << 13),
        HOLDING_A_NO_STAMINA_DROP = (1 << 14),
        FIRST_PERSON = (1 << 18),
        SKYWARD_STRIKE_ACTIVE = (1 << 20),
        LIGHTNING_STRIKE_ACTIVE = (1 << 22),
        C_MENU_OPEN = (1 << 23),
        SHIELD_OUT = (1 << 27),
        HOLDING_TARGET = (1 << 28),
        ITEM_NEAR_LINK = (1 << 29),
        USING_ITEM = (1 << 31),
    };

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

    inline bool checkFlags0x340(u32 mask) const {
        return (someFlags_0x340 & mask) != 0;
    }

    inline void offFlags_0x340(u32 mask) {
        someFlags_0x340 &= ~mask;
    }

    inline bool checkFlags0x350(u32 mask) const {
        return (someFlags_0x350 & mask) != 0;
    }

    inline bool checkFlags0x354(u32 mask) const {
        return (someFlags_0x354 & mask) != 0;
    }

    inline void onFlags_0x358(u32 mask) {
        someFlags_0x358 |= mask;
    }
    inline void offFlags_0x358(u32 mask) {
        someFlags_0x358 &= ~mask;
    }

    inline void onFlags_0x360(u32 mask) {
        field_0x360 |= mask;
    }

    inline void offFlags_0x360(u32 mask) {
        field_0x360 &= ~mask;
    }

    inline void onForceOrPreventActionFlags(u32 mask) {
        mForceOrPreventActionFlags |= mask;
    }

    inline void onFaceUpdateFlags(u32 mask) {
        mFaceUpdateFlags |= mask;
    }

    inline void offFaceUpdateFlags(u32 mask) {
        mFaceUpdateFlags &= ~mask;
    }

    inline bool checkFaceUpdateFlags(u32 mask) const {
        return (mFaceUpdateFlags & mask) != 0;
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

    bool checkSwordAndMoreStates(u32 mask) {
        return mSwordAndMoreStates & mask;
    }

    void onModelUpdateFlag(u32 mask) {
        mModelUpdateFlags |= mask;
    }
    void offModelUpdateFlag(u32 mask) {
        mModelUpdateFlags &= ~mask;
    }
    bool checkModelUpdateFlag(u32 mask) const {
        return mModelUpdateFlags & mask;
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

    bool isAttacking() const {
        return mAttackDirection != ATTACK_NONE;
    }

    bool isAttackingJumpSlash() const {
        return mAttackDirection == ATTACK_JUMP_SLASH;
    }

    u16 getSpecificAttackDirection() const {
        return mSpecificAttackDirection;
    }

    inline bool hasvt_0x1C0() const {
        return vt_0x1C0() != nullptr;
    }

    static s32 getCurrentSwordTypeInline() {
        return sCurrentSword;
    }

    static bool isInEvent();
    bool isAttackingLeft() const;
    bool isAttackingRight() const;
    bool isAttackingDown() const;
    bool isAttackingUp() const;
    bool isAttackingStab() const;
    bool isAttackingSpinHorizontal() const;
    bool isAttackingSpinVertical() const;
    bool isAttackingSpin() const {
        return isAttackingSpinHorizontal() || isAttackingSpinVertical();
    }
    void setBonkRelatedAnimFlag(bool b);
    void setPosYRot(const mVec3_c &pos, mAng rot, bool force = false, UNKWORD = 0, UNKWORD = 0);
    void setTransform(const mMtx_c &mtx, bool force, UNKWORD, UNKWORD);
    bool someTargetedActorCheck() const;
    static mAng fn_8005BA90();
    static mAng fn_8005BAA0();
    static void fairyHeal(dAcOFairy_c *fairy);
    static void updateCurrentSword();

    static bool isOutOfStamina();
    static f32 getStamina();

    static s32 getCurrentTunicType();
    static s32 getCurrentSwordType();
    static const char *getSwordName(s32);
    static s32 getCurrentlyEquippedShieldType();

    static const mColor &getEarringsColor();

    static mVec3_c sPos1;
    static mVec3_c sPos2;
    static mAng sRot1;
    static mAng sRot2;
    static const f32 sPushRelatedConstant;
    static u8 sCurrentSword;

protected:
    /* 0x330 */ u8 unk_0x330[0x334 - 0x330];
    /* 0x334 */ f32 field_0x334;
    /* 0x338 */ u8 mAttackDirection;
    /* 0x339 */ u8 mRidingActorType;
    /* 0x33A */ u16 mSpecificAttackDirection;
    /* 0x33C */ u8 unk_0x33C[0x340 - 0x33C];
    /* 0x340 */ u32 someFlags_0x340;
    /* 0x344 */ u32 mFaceUpdateFlags;
    /* 0x348 */ u32 mSwordAndMoreStates;
    /* 0x34C */ u32 mModelUpdateFlags;
    /* 0x350 */ u32 someFlags_0x350;
    /* 0x354 */ u32 someFlags_0x354;
    /* 0x358 */ u32 someFlags_0x358;
    /* 0x35C */ u32 mForceOrPreventActionFlags;
    /* 0x360 */ u32 field_0x360;
    /* 0x364 */ u32 mActionFlags;
    /* 0x368 */ u32 mActionFlagsCont;
    /* 0x36C */ int mCurrentAction; // TODO (Document Enum)
};

#endif
