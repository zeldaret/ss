#ifndef D_A_PLAYER_H
#define D_A_PLAYER_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_itembase.h"
#include "d/a/e/d_a_en_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_d.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmchrblend.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_banm.h"
#include "m/m3d/m_bmdl.h"
#include "m/m3d/m_mdl.h"
#include "m/m3d/m_scnleaf.h"
#include "m/m3d/m_smdl.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_calcworld.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/stage_render_stuff.h"

// See Below for some info
// https://github.com/lepelog/skywardsword-tools/wiki/Player-Actor

struct PlayerAnimation {
    /* 0x00 */ const char *animName;
    /* 0x04 */ u8 field_0x04;
    /* 0x05 */ u8 field_0x05;
    /* 0x06 */ u16 mFaceTexPatIdx;
    /* 0x08 */ u16 mFaceTexSrtIdx;
    /* 0x0A */ u16 mFaceAnmChrIdx;
};

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

class daPlayerModelBase_c;

class daPlBaseScnObjCallback_c : public dScnCallback_c {
public:
    daPlBaseScnObjCallback_c(dAcObjBase_c *b) : dScnCallback_c(b), field_0x0C(b) {}

    /* vt 0x10 */ virtual void
    ExecCallback_CALC_MAT(nw4r::g3d::ScnObj::Timing, nw4r::g3d::ScnObj *, u32, void *) override;

    /* 0x18 */ virtual void ExecCallback_DRAW_OPA(nw4r::g3d::ScnObj::Timing, nw4r::g3d::ScnObj *, u32, void *) override;

private:
    /* 0x0C */ dAcObjBase_c *field_0x0C;
};

class daPlBaseMdlCallback_c : public m3d::mdl_c::mdlCallback_c {
public:
    virtual void ExecCallbackA(nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl, nw4r::g3d::FuncObjCalcWorld *) override;

    void setNodeIds(u16 min, u16 max) {
        mNodeIdMin = min;
        mNodeIdMax = max;
    }

private:
    /* 0x30 */ u16 mNodeIdMin;
    /* 0x32 */ u16 mNodeIdMax;
};

class daPlBaseMdl_c : public m3d::mdl_c {
public:
    daPlBaseMdl_c() : mpSoundData(nullptr) {}

    virtual bool setAnm(m3d::banm_c &anm) override;
    virtual void remove() override;

    bool create(
        daPlayerModelBase_c *player, nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption, u32 nView, u32 *pSize
    );
    void setBlendNodeRange(u16, u16, f32);

    nw4r::g3d::ChrAnmResult *getNodeResult(u16 node) {
        return mCallback.getNodeResult(node);
    }

    void setSoundRelated(u16 id, void *data) {
        field_0x5A = id;
        mpSoundData = data;
    }

private:
    /* 0x24 */ daPlBaseMdlCallback_c mCallback;
    /* 0x58 */ u8 field_0x58;
    /* 0x5A */ u16 field_0x5A;
    /* 0x5C */ void *mpSoundData;
};

class daPlBaseMainCallback_c : public m3d::callback_c {
public:
    virtual void timingA(u32, nw4r::g3d::ChrAnmResult *, nw4r::g3d::ResMdl) override;
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;
    virtual void timingC(nw4r::math::MTX34 *, nw4r::g3d::ResMdl) override;

    void setPlayer(daPlayerModelBase_c *p) {
        mpPlayer = p;
    }

private:
    daPlayerModelBase_c *mpPlayer;
};

class daPlBaseAnmChr_c : public m3d::anmChr_c {
public:
    daPlBaseAnmChr_c() : field_0x35(0), field_0x36(0) {}

    void play() override;
    bool isFinished();

private:
    /* 0x35 */ u8 field_0x35;
    /* 0x36 */ u8 field_0x36;
};

class daPlBaseHandsCallback_c : public nw4r::g3d::ICalcWorldCallback {
public:
    virtual void ExecCallbackC(
        nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl,
        nw4r::g3d::FuncObjCalcWorld *pFuncObj
    ) override; // at 0x14

    void setPlayer(daPlayerModelBase_c *p) {
        mpPlayer = p;
    }

private:
    /* 0x04 */ daPlayerModelBase_c *mpPlayer;
};

class daPlBaseHeadCallback_c : public m3d::callback_c {
public:
    virtual void timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) override;

    void setPlayer(daPlayerModelBase_c *p) {
        mpPlayer = p;
    }

private:
    daPlayerModelBase_c *mpPlayer;
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

    enum ModelUpdateFlags_e {
        UPDATE_MODEL_SWORD = 0x20,
        UPDATE_MODEL_SHIELD = 0x40,
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

    inline bool checkActionFlags(u32 mask) const {
        return (mActionFlags & mask) != 0;
    }
    inline bool checkActionFlagsCont(u32 mask) const {
        return (mActionFlagsCont & mask) != 0;
    }

    inline u8 getRidingActorType() const {
        return mRidingActorType;
    }

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
    static void updateCurrentSword();

    static s32 getCurrentSwordType();
    static const char *getSwordName(s32);
    static s32 getCurrentlyEquippedShieldType();
    static s32 getCurrentlyEquippedShieldId();
    static u32 getCurrentShieldPouchSlot();

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
    /* 0x33A */ u8 unk_0x33A[0x340 - 0x33A];
    /* 0x340 */ u32 someFlags_0x340;
    /* 0x344 */ u8 unk_0x344[0x348 - 0x344];
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
          mScnObjCallback(this),
          mScnCallback1(this),
          mScnCallback2(this),
          mScnCallback3(this),
          mScnCallback4(this),
          mScnCallback5(this) {}
    virtual ~daPlayerModelBase_c() {}

    enum TunicType_e {
        TUNIC_0,
        TUNIC_1,
        TUNIC_SILENT,
    };

    // Alink.arc > g3d > model.brres > 3DModels(NW4R) > al > Bones > ...
    enum PlayerMainModelNode_e {
        PLAYER_MAIN_NODE_CENTER = 0,
        PLAYER_MAIN_NODE_BACKBONE_1 = 1,
        PLAYER_MAIN_NODE_BACKBONE_2 = 2,
        PLAYER_MAIN_NODE_HEAD = 4,
        PLAYER_MAIN_NODE_POD = 5,

        PLAYER_MAIN_NODE_SHOULDER_L = 6,
        PLAYER_MAIN_NODE_ARM_L1 = 7,
        PLAYER_MAIN_NODE_ARM_L2 = 8,
        PLAYER_MAIN_NODE_HAND_L = 9,
        PLAYER_MAIN_NODE_WEAPON_L = 10,
        
        PLAYER_MAIN_NODE_SHOULDER_R = 11,
        PLAYER_MAIN_NODE_ARM_R1 = 12,
        PLAYER_MAIN_NODE_ARM_R2 = 13,
        PLAYER_MAIN_NODE_HAND_R = 14,
        PLAYER_MAIN_NODE_WEAPON_R = 15,
        PLAYER_MAIN_NODE_WAIST = 16,

        PLAYER_MAIN_NODE_CLOTCH_L = 17,
        PLAYER_MAIN_NODE_LEG_L1 = 18,
        PLAYER_MAIN_NODE_LEG_L2 = 19,
        PLAYER_MAIN_NODE_FOOT_L = 20,
        PLAYER_MAIN_NODE_TOE_L = 21,

        PLAYER_MAIN_NODE_CLOTCH_R = 22,
        PLAYER_MAIN_NODE_LEG_R1 = 23,
        PLAYER_MAIN_NODE_LEG_R2 = 24,
        PLAYER_MAIN_NODE_FOOT_R = 25,
        PLAYER_MAIN_NODE_TOE_R = 26,

        PLAYER_MAIN_NODE_FSKIRT_L1 = 27,
        PLAYER_MAIN_NODE_FSKIRT_L2 = 28,

        PLAYER_MAIN_NODE_FSKIRT_R1 = 29,
        PLAYER_MAIN_NODE_FSKIRT_R2 = 30,

        PLAYER_MAIN_NODE_RSKIRT_L1 = 31,
        PLAYER_MAIN_NODE_RSKIRT_L2 = 32,
        PLAYER_MAIN_NODE_RSKIRT_R1 = 33,
        PLAYER_MAIN_NODE_RSKIRT_R2 = 34,
    };

    // Alink.arc > g3d > model.brres > 3DModels(NW4R) > al_head > Bones > ...
    enum PlayerHeadModelNode_e {
        PLAYER_HEAD_NODE_HAIR_L = 1,
        PLAYER_HEAD_NODE_HAIR_R1 = 2,
        PLAYER_HEAD_NODE_HAIR_R2 = 3,
        PLAYER_HEAD_NODE_MOMI_L = 4,
        PLAYER_HEAD_NODE_MOMI_R = 5,
        PLAYER_HEAD_NODE_MOMI_Z_CAP_1 = 6,
    };

    // Alink.arc > g3d > model.brres > 3DModels(NW4R) > al_hands > Bones > ...
    enum PlayerHandsModelNode_e {
        PLAYER_HANDS_NODE_HAND_L = 1,
        PLAYER_HANDS_NODE_HAND_R = 2,
    };

    void fn_8005F890(nw4r::math::MTX34 *);
    void fn_80061410();

    static void freeFrmHeap(mHeapAllocator_c *allocator);
    static void allocFrmHeap(mHeapAllocator_c *allocator, u32 size, const char *name);
    void allocExternalDataBuffers();
    void initModelHeaps();
    void updateSwordShieldModelsIfNeeded();
    m3d::anmTexSrt_c *createAnmTexSrt(const char *resName, m3d::bmdl_c &mdl);

    static bool createGenericMdl(nw4r::g3d::ResMdl resMdl, m3d::mdl_c &mdl, mAllocator_c *alloc, u32 bufferOption);
    static bool createGenericSmdl(nw4r::g3d::ResMdl resMdl, m3d::smdl_c &mdl, mAllocator_c *alloc, u32 bufferOption);
    static void setTransformAndCalc(m3d::scnLeaf_c &lf, const mMtx_c *mtx);
    void setShieldTransform(bool inHand);

    static nw4r::g3d::ResFile getItemResFile(const char *name, mAllocator_c &allocator);
    nw4r::g3d::ResFile getExternalCompressedFile(const char *name, const char *extension, void *dest, u32 maxSize);
    nw4r::g3d::ResAnmChr getExternalAnmChr(const char *name, void *dest, u32 maxSize);
    nw4r::g3d::ResAnmTexPat getExternalAnmTexPat(const char *name, void *dest, u32 maxSize);
    nw4r::g3d::ResAnmTexSrt getExternalAnmTexSrt(const char *name, void *dest, u32 maxSize);

    static bool isBodyAnmPart_0_2_4(s32 part);

    void updateEarringsColor();
    void loadBodyModels();
    void loadBody();
    void loadHandsModels();
    void setSwordAnm(const char *name);
    void initSwordModel();
    void updateSwordModel();
    void initShieldModel();
    void updateShieldModel();

    // Own virtual methods

    /* vt 0x2E0 */ virtual void transformBackbone1(nw4r::g3d::WorldMtxManip *) {}
    /* vt 0x2E4 */ virtual void transformModelCenter(mMtx_c *) {}
    /* vt 0x2E8 */ virtual bool vt_0x2E8(nw4r::math::MTX34 *, const u16 *, bool) {
        return true;
    }
    /* vt 0x2EC */ virtual void isOnClawTargetMaybe(s32 arm, mAng &a1, mAng &a2) {
        a1 = 0;
        a2 = 0;
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
    // r = 1, l = 0, backbone = 2
    /* vt 0x308 */ virtual mAng getArmZRot(s32 which) {
        return 0;
    }
    /* vt 0x30C */ virtual void vt_0x30C(mVec3_c &res) {
        res = mVec3_c::Ex;
    }

    // Overrides

    /* vt 0x0E4 */ virtual const mVec3_c &getCenterTranslation() const override {
        return mCenterTranslation;
    }
    /* vt 0x0E8 */ virtual const mVec3_c &vt_0x0E8() const override {
        return field_0x12F4;
    }
    /* vt 0x0EC */ virtual const mVec3_c &vt_0x0EC() const override {
        return mHeadTranslation;
    }
    /* vt 0x0F0 */ virtual const mVec3_c &vt_0x0F0() const override {
        return mTranslationHand[1];
    }
    /* vt 0x0F4 */ virtual const mVec3_c &vt_0x0F4() const override {
        return mTranslationHand[0];
    }

    /* vt 0x114 */ virtual void somethingWithCarriedActorFlags() override;
    /* vt 0x118 */ virtual dAcObjBase_c *getCurrentCarriedActor() override {
        return mRef.get();
    }

    /* vt 0x18C */ virtual void getBodyMtx(mMtx_c *out_mtx, int boneIdx) override {
        mMainMdl.getNodeWorldMtx(boneIdx, *out_mtx);
    }
    /* vt 0x190 */ virtual void getSheathModelMatrix(mMtx_c *out_mtx) override {
        mSheathMdl.getLocalMtx(*out_mtx);
    }
    /* vt 0x194 */ virtual void getSwordModelMatrix(mMtx_c *out_mtx) override {
        mSwordMdl.getLocalMtx(*out_mtx);
    }
    /* vt 0x198 */ virtual mAng vt_0x198() override {
        return field_0x1268;
    }
    /* vt 0x19C */ virtual mAng vt_0x19C() override {
        return field_0x126A;
    }

    u8 getHandMat(s32 idx) {
        return field_0x1208[idx];
    }

    inline bool checkSwordAndMoreStates(u32 mask) const {
        return (mSwordAndMoreStates & mask) != 0;
    }

    inline void offSwordAndMoreStates(u32 mask) {
        mSwordAndMoreStates &= ~mask;
    }

    void loadSound(nw4r::g3d::ResFile file, const char *name, s32 animIdx);

    void syncSoundWithAnim();
    static void registMassObj(cCcD_Obj* obj, u8 priority);
    void updateModelColliders();
    void updateCachedPositions();

    // Model callbacks - dropping the mdl argument, apparently
    void mainModelTimingA(u32 nodeId, nw4r::g3d::ChrAnmResult *result);
    void adjustMainModelChrAnm(PlayerMainModelNode_e nodeId, nw4r::g3d::ChrAnmResult *result);

    void mainModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result);
    void adjustMainModelWorldMtx(PlayerMainModelNode_e nodeId, nw4r::g3d::WorldMtxManip *result);

    void mainModelTimingC(nw4r::math::MTX34 *result);
    void headModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result);

    void handsCallbackC(nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *pFuncObj);

    void updateBlendWeights(PlayerMainModelNode_e nodeId);
    void updateMainBlend1(f32 blend);
    void updateMainBlend2(f32 blend, bool save);
    void applyWorldRotationMaybe(nw4r::math::MTX34 *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order);
    void applyWorldRotationMaybe(nw4r::g3d::WorldMtxManip *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order);

    static const PlayerAnimation sAnimations[443];
    static const u8 sShieldDurabilities[10];
    static const u8 sShieldRegenTimes[10];
    static mColor sGuideColor1;
    static mColor sGuideColor2;
    static mColor sGuideColor3;

protected:
    /* 0x370 */ mHeapAllocator_c mModelAllocator;
    /* 0x38C */ mHeapAllocator_c mSwordAllocator;
    /* 0x3A8 */ mHeapAllocator_c mShieldAllocator;
    /* 0x3C4 */ void *mpExternalAnmCharBuffer;
    /* 0x3C8 */ nw4r::g3d::ResFile mSwordRes;
    /* 0x3CC */ nw4r::g3d::ResFile mAlinkRes;
    /* 0x3D0 */ nw4r::g3d::ResFile mPlCommonAnimeRes;
    /* 0x3D4 */ nw4r::g3d::ResFile mPlCommonSoundRes;
    /* 0x3D8 */ nw4r::g3d::ResFile mCurrentRes;
    /* 0x3DC */ nw4r::g3d::ResFile mAlink2Res;
    /* 0x3E0 */ daPlBaseMdl_c mMainMdl;
    /* 0x440 */ daPlBaseMainCallback_c mMainBodyCallback;
    /* 0x448 */ m3d::mdl_c mHeadMdl;
    /* 0x46C */ daPlBaseHeadCallback_c mHeadCallback;
    /* 0x474 */ m3d::smdl_c mFaceMdl;
    /* 0x490 */ m3d::anmChr_c mFaceAnmChr;
    /* 0x4C8 */ void *mpAnmCharBuffer;
    /* 0x4CC */ m3d::anmTexPat_c mFaceTexPat;
    /* 0x4F8 */ void *mpTexPatBuffer;
    /* 0x4FC */ m3d::anmTexSrt_c mFaceTexSrt;
    /* 0x528 */ void *mpTexSrtBuffer;
    /* 0x52C */ m3d::anmChr_c mHeadAnmChr;
    /* 0x564 */ m3d::smdl_c mHandsMdl;
    /* 0x580 */ daPlBaseHandsCallback_c mCalcWorldCallback;
    /* 0x588 */ daPlBaseScnObjCallback_c mScnObjCallback;
    /* 0x598 */ m3d::smdl_c mSwordMdl;
    /* 0x5B4 */ m3d::anmMatClr_c mSwordAnmMatClr;
    /* 0x5E0 */ m3d::smdl_c mShieldMdl;
    /* 0x5FC */ m3d::anmTexPat_c mShieldAnmTexPat1;
    /* 0x628 */ m3d::anmTexPat_c mShieldAnmTexPat2;
    /* 0x654 */ m3d::smdl_c mSheathMdl;
    /* 0x670 */ m3d::smdl_c mUnkMdl1;
    /* 0x68C */ m3d::anmChr_c mAnmChr;
    /* 0x6C4 */ m3d::anmChrBlend_c mAnmChrBlend;
    /* 0x6EC */ dScnCallback_c mScnCallback1;
    /* 0x6F8 */ dScnCallback_c mScnCallback2;
    /* 0x704 */ dScnCallback_c mScnCallback3;
    /* 0x710 */ dScnCallback_c mScnCallback4;
    /* 0x71C */ dScnCallback_c mScnCallback5;
    /* 0x728 */ daPlBaseAnmChr_c mAnmChrs[6];
    /* 0x878 */ dCcD_Cyl mCcCyls[3];
    /* 0xC68 */ dCcD_Cps mCcCpss[3];
    /* 0x10B8 */ dCcD_Cyl mCcCyl;
    /* 0x1208 */ u8 field_0x1208[2];
    /* 0x120A */ u8 field_0x120A;
    /* 0x120B */ u8 mTunicType;
    /* 0x120C */ u8 mCurrentAnmChrIdx;
    /* 0x120D */ u8 mShieldPouchSlot;
    /* 0x120E */ s8 mPouchMatId;
    /* 0x120F */ s8 mBeltMatId;
    /* 0x1210 */ s8 mLowbodyMapId;
    /* 0x1211 */ s8 mLowotherMapId;
    /* 0x1212 */ s8 mPouch2MatId;
    /* 0x1213 */ s8 mUpbodyMatId;
    /* 0x1214 */ s8 mUpotherMatId;
    /* 0x1215 */ s8 mBelt2MatId;
    /* 0x1216 */ s8 mCapMatId;
    /* 0x1217 */ s8 mSkinMatId;
    /* 0x1218 */ u8 mSkywardStrikeChargeDuration;
    /* 0x1219 */ u8 field_0x1219;
    /* 0x121A */ u16 mAnimations[6];
    /* 0x1226 */ u16 mFaceAnmChrIdx1;
    /* 0x1228 */ u16 mFaceAnmChrIdx2;
    /* 0x122A */ u16 mFaceAnmTexPatIdx1;
    /* 0x122C */ u16 mFaceAnmTexPatIdx2;
    /* 0x122E */ u16 mFaceAnmTexSrtIdx1;
    /* 0x1230 */ u16 mFaceAnmTexSrtIdx2;
    /* 0x1232 */ u16 mBreakingEffect;
    /* 0x1234 */ mAng mWaistZRot;
    /* 0x1236 */ mAng mWaistYRot;
    /* 0x1238 */ mAng field_0x1238[5];
    /* 0x1242 */ mAng field_0x1242[5];
    /* 0x124C */ u8 _0x124C[0x1256 - 0x124C];
    /* 0x1256 */ mAng field_0x1256;
    /* 0x1258 */ mAng field_0x1258;
    /* 0x125A */ mAng field_0x125A;
    /* 0x125C */ mAng mRSkirtR1Rot;
    /* 0x125E */ mAng mRSkirtL1Rot;
    /* 0x1260 */ mAng mRightHandRotation;
    /* 0x1262 */ mAng mHeadZRot;
    /* 0x1264 */ mAng mHeadYRot;
    /* 0x1266 */ mAng mHeadXRot;
    /* 0x1268 */ mAng field_0x1268;
    /* 0x126A */ mAng field_0x126A;
    /* 0x126C */ mAng field_0x126C;
    /* 0x1270 */ f32 field_0x1270;
    /* 0x1274 */ f32 field_0x1274;
    /* 0x1278 */ f32 field_0x1278;
    /* 0x127C */ mVec3_c field_0x127C;
    /* 0x1288 */ mVec3_c mCenterTranslation;
    /* 0x1294 */ mVec3_c mHeadTranslation;
    /* 0x12A0 */ mVec3_c mPositionAboveLink;
    /* 0x12AC */ mVec3_c mTranslationHand[2];
    /* 0x12C4 */ mVec3_c mTranslationWeapon[2];
    /* 0x12DC */ mVec3_c mToeTranslation[2];
    /* 0x12F4 */ mVec3_c field_0x12F4;
    /* 0x1300 */ mQuat_c field_0x1300[4];
    /* 0x1340 */ mQuat_c mQuat1;
    /* 0x1350 */ mQuat_c mQuat2;
    /* 0x1360 */ dAcRef_c<dAcObjBase_c> mRef; // not sure about the class
    /* 0x136C */ mAng *field_0x136C;
    /* 0x1370 */ mAng *field_0x1370;
    /* 0x1374 */ mAng *field_0x1374; // x3
    /* 0x1378 */ mAng *field_0x1378; // x3
};

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
    /* 0x137C */ u8 _0x137C[0x4564 - 0x137C];
    /* 0x4564 */ f32 field_0x4564;

public:
    f32 getField_0x4564() const {
        return field_0x4564;
    }

    f32 getSwordAnmFrame() const {
        return mSwordAnmMatClr.getFrame(0);
    }

    nw4r::g3d::ResFile getSwordResFile() const {
        return mSwordRes;
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

    static const dAcPy_c *GetLink() {
        return LINK;
    }

    static dAcPy_c *GetLink2() {
        return LINK2;
    }

    void onShieldUpdate();

    static dAcPy_c *LINK;
    static dAcPy_c *LINK2;

    static s32 getCurrentBowType();
    static s32 getCurrentSlingshotType();
    static s32 getCurrentBeetleType();
    static s32 getCurrentBugNetType();

    static bool isInBambooCuttingMinigame();

    static bool isItemRestrictedByBokoBase(ITEM_ID item);

    static u32 getCurrentHealthCapacity();

    bool canDowseProbably() const;

    mAng fn_8023B520() const;
    mAng fn_8023B590() const;
};

#endif
