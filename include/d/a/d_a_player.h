#ifndef D_A_PLAYER_H
#define D_A_PLAYER_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_itembase.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/file_manager.h"

class dAcPy_c : public dAcObjBase_c {
    // See Below for some info
    // https://github.com/lepelog/skywardsword-tools/wiki/Player-Actor
public:
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

    enum dAcPy_RidingActor {
        RIDING_NONE = 0,
        RIDING_TRUCK_MINECART = 1,
        RIDING_LOFTWING = 2,
        RIDING_BOAT = 3,
    };

    dAcPy_c();
    virtual ~dAcPy_c();

    void setTransform(const mMtx_c &mtx, UNKWORD, UNKWORD, UNKWORD);
    void setPosYRot(const mVec3_c &pos, mAng rot, UNKWORD, UNKWORD, UNKWORD);
    // argument is always false in existing code, true doesn't seem to make a difference
    void bonk(bool unk = false);
    bool fn_80202D90(bool);

    /* vt 0x080 */ virtual UNKWORD IfCurrentActionToActor(dAcBase_c *ac, UNKWORD);
    /* vt 0x084 */ virtual void vt_0x084(dAcBase_c *ac, UNKWORD);
    /* vt 0x088 */ virtual void set0x439F(dAcBase_c *ac, UNKWORD);
    /* vt 0x08C */ virtual void set0x43DE();
    /* vt 0x090 */ virtual f32 vt_0x090();
    /* vt 0x094 */ virtual void relatedToUsingItem0x11();
    /* vt 0x098 */ virtual void setPosRot(const mVec3_c &pos, const mAng3_c &ang, UNKWORD, UNKWORD, UNKWORD);
    /* vt 0x09C */ virtual void isLiftingObject();
    /* vt 0x0A0 */ virtual void isThrowingOrRollingItem();
    /* vt 0x0A4 */ virtual void canThrowObject();
    /* vt 0x0A8 */ virtual void canRollObject();
    /* vt 0x0AC */ virtual void vt_0x0AC();
    /* vt 0x0B0 */ virtual void isCarryingBomb();
    /* vt 0x0B4 */ virtual void isPullingOutBomb();
    /* vt 0x0B8 */ virtual void isUsingHarp();
    /* vt 0x0BC */ virtual void isFinishedPlayingHarp();
    /* vt 0x0C0 */ virtual void isUsingWhip();
    /* vt 0x0C4 */ virtual void anotherThingWithWhip();
    /* vt 0x0C8 */ virtual void somethingWithWHip();
    /* vt 0x0CC */ virtual void isWhipOnObject();
    /* vt 0x0D0 */ virtual void tryGrabItemWithWhip();
    /* vt 0x0D4 */ virtual void someFloatWithWhip();
    /* vt 0x0D8 */ virtual void getItemFromClawshotsMaybe();
    /* vt 0x0DC */ virtual void vt_0x0DC(dAcBase_c *, mVec3_c &);
    /* vt 0x0E0 */ virtual void isUsingBomb();
    /* vt 0x0E4 */ virtual void vt_0x0E4();
    /* vt 0x0E8 */ virtual void vt_0x0E8();
    /* vt 0x0EC */ virtual void vt_0x0EC();
    /* vt 0x0F0 */ virtual void vt_0x0F0();
    /* vt 0x0F4 */ virtual void vt_0x0F4();
    /* vt 0x0F8 */ virtual void vt_0x0F8();
    /* vt 0x0FC */ virtual void vt_0x0FC();
    /* vt 0x100 */ virtual void isCrawling();
    /* vt 0x104 */ virtual void isUsingSword();
    /* vt 0x108 */ virtual void isUsingMitts();
    /* vt 0x10C */ virtual void isUsingShield();
    /* vt 0x110 */ virtual void getTargetedActor1();
    /* vt 0x114 */ virtual void somethingWithCarriedActorFlags();
    /* vt 0x118 */ virtual dAcObjBase_c *getCurrentCarriedActor();
    /* vt 0x11C */ virtual void checkBeetleActorReferences();
    /* vt 0x120 */ virtual void getBeetleActorReferencesSize();
    /* vt 0x124 */ virtual void vt_0x124();
    /* vt 0x128 */ virtual void vt_0x128();
    /* vt 0x12C */ virtual void vt_0x12C();
    /* vt 0x130 */ virtual void isBurning();
    /* vt 0x134 */ virtual void isShocked();
    /* vt 0x138 */ virtual void isCursed();
    /* vt 0x13C */ virtual void isSmelly();
    /* vt 0x140 */ virtual void store2Floats0x4500();
    /* vt 0x144 */ virtual void setActorLinkToSomething();
    /* vt 0x148 */ virtual void applyDamageWithIFrames();
    /* vt 0x14C */ virtual void vt_0x14C();
    /* vt 0x150 */ virtual bool hasLessThanQuarterHealth(bool);
    /* vt 0x154 */ virtual void vt_0x154();
    /* vt 0x158 */ virtual void vt_0x158();
    /* vt 0x15C */ virtual void vt_0x15C();
    /* vt 0x160 */ virtual void vt_0x160();
    /* vt 0x164 */ virtual void vt_0x164();
    /* vt 0x168 */ virtual f32 getCurrentAnimFrame() const;
    /* vt 0x16C */ virtual void getFloatIfStruggling();
    /* vt 0x170 */ virtual void isSwimming();
    /* vt 0x174 */ virtual void isGustBellowsBeingBlown();
    /* vt 0x178 */ virtual void getPointedAngle();
    /* vt 0x17C */ virtual void getBellowsPosOrOtherVec3F();
    /* vt 0x180 */ virtual void setWindMillPos();
    /* vt 0x184 */ virtual void isOffeset0x435eEqual0x20();
    /* vt 0x188 */ virtual void getVec3F_Z();
    /* vt 0x18C */ virtual void getBodyMtx(mMtx_c *out_mtx, int boneIdx);
    /* vt 0x190 */ virtual void getSheathModelMatrix();
    /* vt 0x194 */ virtual void getSwordModelMatrix();
    /* vt 0x198 */ virtual void vt_0x198();
    /* vt 0x19C */ virtual void vt_0x19C();
    /* vt 0x1A0 */ virtual void vt_0x1A0();
    /* vt 0x1A4 */ virtual void vt_0x1A4();
    /* vt 0x1A8 */ virtual void vt_0x1A8();
    /* vt 0x1AC */ virtual void vt_0x1AC();
    /* vt 0x1B0 */ virtual void isItemFairyFromBugnet();
    /* vt 0x1B4 */ virtual void isByte0x434eEqual7();
    /* vt 0x1B8 */ virtual void canHandleGameOver();
    /* vt 0x1BC */ virtual void vt_0x1BC();
    /* vt 0x1C0 */ virtual void *vt_0x1C0() const;
    /* vt 0x1C4 */ virtual void getActorInActorRef1();
    /* vt 0x1C8 */ virtual void doesActorRef1Exist();
    /* vt 0x1CC */ virtual void unlinkActorRef1();
    /* vt 0x1D0 */ virtual void ifHasHealthAndSomethingElse();
    /* vt 0x1D4 */ virtual void isChargingWeapon();
    /* vt 0x1D8 */ virtual void isWeaponChargedFully();
    /* vt 0x1DC */ virtual void didShootBow();
    /* vt 0x1E0 */ virtual void vt_0x1E0();
    /* vt 0x1E4 */ virtual void checkRefArrayAtIndex();
    /* vt 0x1E8 */ virtual void sizeOfActorRefs_16();
    /* vt 0x1EC */ virtual void vt_0x1EC();
    /* vt 0x1F0 */ virtual void vt_0x1F0();
    /* vt 0x1F4 */ virtual void didDiveSky_and_other();
    /* vt 0x1F8 */ virtual void didDiveSky_andNotOther();
    /* vt 0x1FC */ virtual void isLandingFromSailcloth();
    /* vt 0x200 */ virtual void isBeingPulledFromRightClawshot();
    /* vt 0x204 */ virtual void isBeingPulledByLeftClawshot();
    /* vt 0x208 */ virtual void isDashing();
    /* vt 0x20C */ virtual void isSliding();
    /* vt 0x210 */ virtual void isSlding2();
    /* vt 0x214 */ virtual void vt_0x214();
    /* vt 0x218 */ virtual void isinMpPosePower();
    /* vt 0x21C */ virtual void isPushingBlock();
    /* vt 0x220 */ virtual void getRidingActor();
    /* vt 0x224 */ virtual void isRidingBall();
    /* vt 0x228 */ virtual void getRidingMinecartActor();
    /* vt 0x22C */ virtual void getTurnableSwitch();
    /* vt 0x230 */ virtual void vt_0x230();
    /* vt 0x234 */ virtual void vt_0x234();
    /* vt 0x238 */ virtual void setVecIfInFreeFall();
    /* vt 0x23C */ virtual void setActorRef10();
    /* vt 0x240 */ virtual void vt_0x240();
    /* vt 0x244 */ virtual void vt_0x244();
    /* vt 0x248 */ virtual void vt_0x248();
    /* vt 0x24C */ virtual void vt_0x24C();
    /* vt 0x250 */ virtual void vt_0x250();
    /* vt 0x254 */ virtual void isAttackingUnderground();
    /* vt 0x258 */ virtual void vt_0x258();
    /* vt 0x25C */ virtual void vt_0x25C();
    /* vt 0x260 */ virtual void vt_0x260();
    /* vt 0x264 */ virtual void getGroosenatorIfIn();
    /* vt 0x268 */ virtual void setActorRef11();
    /* vt 0x26C */ virtual void doesGameOver();
    /* vt 0x270 */ virtual void triggerExitRelated();
    /* vt 0x274 */ virtual void vt_0x274();
    /* vt 0x278 */ virtual void vt_0x278();
    /* vt 0x27C */ virtual void getSwordPos();
    /* vt 0x280 */ virtual void vt_0x280();
    /* vt 0x284 */ virtual bool isUsingBugnet() const;
    /* vt 0x288 */ virtual const mVec3_c &getBugNetPos() const;
    /* vt 0x28C */ virtual bool isUsingBugnet1();
    /* vt 0x290 */ virtual void bugNetCollectTreasure(u32 itemId);
    /* vt 0x294 */ virtual void somethingSwitchDials();
    /* vt 0x298 */ virtual void vt_0x298();
    /* vt 0x29C */ virtual void getDieLargeDamageDir();
    /* vt 0x2A0 */ virtual void doGameOver();
    /* vt 0x2A4 */ virtual void isRunningUpWall();
    /* vt 0x2A8 */ virtual void lookTowardItem();
    /* vt 0x2AC */ virtual void vt_0x2AC();
    /* vt 0x2B0 */ virtual void vt_0x2B0();
    /* vt 0x2B4 */ virtual void triggerMoveEventMaybe();
    /* vt 0x2B8 */ virtual void setActorRef9();
    /* vt 0x2BC */ virtual void unlinkActorRef9();
    /* vt 0x2C0 */ virtual void vt_0x2C0();
    /* vt 0x2C4 */ virtual void vt_0x2C4();
    /* vt 0x2C8 */ virtual void isRecovering();
    /* vt 0x2CC */ virtual void isDrinkingPotion();
    /* vt 0x2D0 */ virtual void isHealingShield();
    /* vt 0x2D4 */ virtual void isBottleOut();
    /* vt 0x2D8 */ virtual void isUsingBottle();
    /* vt 0x2DC */ virtual void getShieldRegenProgressMaybe();
    /* vt 0x2E0 */ virtual void vt_0x2E0();
    /* vt 0x2E4 */ virtual void someDivingHrabbingStuff();
    /* vt 0x2E8 */ virtual void vt_0x2E8();
    /* vt 0x2EC */ virtual void isOnClawTargetMaybe();
    /* vt 0x2F0 */ virtual void isMPPose();
    /* vt 0x2F4 */ virtual void isOnTightRope();
    /* vt 0x2F8 */ virtual void isOnVines();
    /* vt 0x2FC */ virtual void alwaysRet0();
    /* vt 0x300 */ virtual void canBlockAttack();
    /* vt 0x304 */ virtual void vt_0x304();
    /* vt 0x308 */ virtual void vt_0x308();
    /* vt 0x30C */ virtual void vt_0x30C();
    /* vt 0x310 */ virtual void vt_0x310();
    /* vt 0x314 */ virtual void vt_0x314();
    /* vt 0x318 */ virtual void vt_0x318();

    /* 0x330 */ u8 unk_0x330[0x339 - 0x330];
    /* 0x339 */ u8 mRidingActorType;
    /* 0x33A */ u8 unk_0x33A[0x340 - 0x33A];
    /* 0x340 */ u32 someFlags_0x340;
    /* 0x344 */ u8 unk_0x344[0x348 - 0x344];
    /* 0x348 */ u32 mSwordAndMoreStates;
    /* 0x34C */ u8 unk_0x34C[0x350 - 0x34C];
    /* 0x350 */ u32 someFlags_0x350;
    u8 UNK_0x354[0x35C - 0x354];
    /* 0x35C */ u32 mForceOrPreventActionFlags;
    /* 0x360 */ UNKWORD field_0x360;
    /* 0x364 */ u32 mActionFlags;
    /* 0x368 */ u32 mActionFlagsCont;
    /* 0x36C */ int mCurrentAction; // TODO (Document Enum)
    /* 0x370 */ u8 _370[0x3C8 - 0x370];
    /* 0x3C8 */ nw4r::g3d::ResFile mSwordRes;
    /* 0x3CC */ u8 _0x3CC[0x5B4 - 0x3CC];
    /* 0x5B4 */ m3d::anmMatClr_c mAnmMatClr;
    /* 0x5E0 */ u8 _0x5E0[0x4564 - 0x5E0];
    /* 0x4564 */ f32 field_0x4564;

    f32 getField_0x4564() const {
        return field_0x4564;
    }

    f32 getAnmMatClrFrame() const {
        return mAnmMatClr.getFrame(0);
    }

    inline bool checkFlags0x340(u32 mask) const {
        return (someFlags_0x340 & mask) != 0;
    }

    inline bool checkFlags0x350(u32 mask) const {
        return (someFlags_0x350 & mask) != 0;
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

    static bool isItemRestrictedByBokoBase(ITEM_ID item);

    static u32 getCurrentHealthCapacity();
};

#endif
