#ifndef D_A_ITEM_H
#define D_A_ITEM_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_itembase.h"
#include "d/a/obj/d_a_obj_scattersand.h"
#include "d/col/bg/d_bg_s_acch.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/d_shadow.h"
#include "f/f_list_mg.h"
#include "f/f_list_nd.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/ut/ut_Color.h"
#include "s/s_State.hpp"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/dowsing_target.h"

class dAcItem_0xB34 {
public:
    virtual ~dAcItem_0xB34();
};

class dAcItem_c : public dAcItemBase_c {
public:
    dAcItem_c();
    virtual ~dAcItem_c();

    STATE_FUNC_DECLARE(dAcItem_c, Wait);
    STATE_FUNC_DECLARE(dAcItem_c, Carry);
    STATE_FUNC_DECLARE(dAcItem_c, GetBeetle);
    STATE_FUNC_DECLARE(dAcItem_c, WaitGet);
    STATE_FUNC_DECLARE(dAcItem_c, Get);
    STATE_FUNC_DECLARE(dAcItem_c, WaitGetDemo);
    STATE_FUNC_DECLARE(dAcItem_c, WaitForcedGetDemo);
    STATE_FUNC_DECLARE(dAcItem_c, GetDemo);
    STATE_FUNC_DECLARE(dAcItem_c, WaitTBoxGetDemo);
    STATE_FUNC_DECLARE(dAcItem_c, ResurgeWait);
    STATE_FUNC_DECLARE(dAcItem_c, WaitTurnOff);
    STATE_FUNC_DECLARE(dAcItem_c, WaitSacredDewGetEffect);

    static u32 createItemParams(u16 itemId, u32 subType, u32 unk, u8 unk2, u8, u8);

    static void spawnItem(u16 item, u32 room, const mVec3_c &pos, const mAng3_c &rot, u32 params, u32 arg);
    static void spawnDrop(u16 item, u32 room, const mVec3_c &pos, const mAng3_c &rot);
    static void giveItem22(ITEM_ID, s32, s32);
    static void giveItem(ITEM_ID, s32, s32);
    static u32 checkFlag(u32 flag);
    static bool hasItem(u32 flag) {
        return checkFlag(flag);
    }
    static void setFlag(s32 id);
    static bool checkTreasureTempCollect(u16 itemId);

    void setItemPosition(const mVec3_c &);
    void getItemFromBWheelItem();
    bool isStateWait();

    static s32 getTotalBombCount();
    static s32 getTotalArrowCount();
    static s32 getTotalSeedCount();

    static s32 getTotalBombCapacity();
    static s32 getTotalSeedCapacity();
    static s32 getTotalArrowCapacity();

    static s32 getRupeeCounter();
    static u32 getCurrentWalletCapacity();
    static u32 getExtraWalletCount();

    static u32 getHeartContainerHealthCount();
    static u32 getGratitudeCrystalCount();

    static u32 getKeyPieceCount();
    static u32 getSmallKeyCount();

    static bool isRupee(ITEM_ID item);
    static bool isKeyPiece(ITEM_ID item);
    static bool isTreasure(ITEM_ID item);
    bool isTriforce() const;

    static bool getItemGetEventName(u16 item, const char **outName);
    static void itemGetEventStart(dAcBase_c *);
    static void itemGetEventEnd(dAcBase_c *);

    static void addRupees(s32 amount);

    static bool hasAnyFairy();

    enum Trial_e {
        TRIAL_SKYLOFT,
        TRIAL_FARON,
        TRIAL_ELDIN,
        TRIAL_LANAYRU,
        TRIAL_NONE,
    };

    static Trial_e getCurrentTrial();

    static void healLink(u32 amount, bool); // move to dAcPy_c

    static bool isPerformingInitialCollection() {
        return sIsPerformingInitialCollection;
    }

    static s32 getCollectionCurrentCount() {
        return sCollectionCurrentCount;
    }

    s32 getQuantity() const {
        return mItemQuantity;
    }

private:
    static bool sIsPerformingInitialCollection;
    static s32 sCollectionCurrentCount;

    /* 0x334 */ UNKTYPE *mpMdl; // Model has its own handling system
    /* 0x338 */ dShadowCircle_c mShdw;
    /* 0x340 */ mVec3_c field_0x340;
    /* 0x34C */ nw4r::ut::Color mColorFilter;
    /* 0x350 */ f32 mLightLevelMaybe;
    /* 0x354 */ u8 _354[0x35C - 0x354];
    /* 0x35C */ dBgS_AcchCir mAcchCir;
    /* 0x3B8 */ dBgS_ObjAcch mObjAcch;
    /* 0x768 */ dCcD_Cyl mCyl;
    /* 0x8B8 */ STATE_MGR_DECLARE(dAcItem_c);
    /* 0x8F4 */ dEmitter_c mEff_0x8F4;
    /* 0x928 */ dEmitter_c mEff_0x928;
    /* 0x95C */ dEmitter_c mEff_0x95C;
    /* 0x990 */ dEmitter_c mEff_0x990;
    /* 0x9C4 */ dEmitter_c mEff_0x9C4;
    /* 0x9F8 */ dEmitter_c mEff_0x9F8;
    /* 0xA2C */ dEmitter_c mEff_0xA2C;
    /* 0xA60 */ dEmitter_c mEff_0xA60;
    /* 0xA94 */ dAcRef_c<dAcOScatterSand> mCoveredSand;
    /* 0xAA0 */ dAcRef_c<dAcObjBase_c> mUnkRef;
    /* 0xAAC */ mVec3_c posCopy;
    /* 0xAB8 */ fLiNdBa_c mNode;
    /* 0xAC4 */ ActorEventRelated mEventRelated;
    /* 0xB14 */ DowsingTarget mDowsingTarget;
    /* 0xB34 */ dAcItem_0xB34 *field_0xB34[2];
    /* 0xB34 */ dAcItem_0xB34 *field_0xB3C;
    /* 0xB40 */ UNKTYPE (dAcItem_c::*mFunc_0xB40)(UNKTYPE);
    /* 0xB4C */ UNKTYPE (dAcItem_c::*mFunc_0xB4C)(UNKTYPE);
    /* 0xB58 */ UNKTYPE (dAcItem_c::*mFunc_0xB58)(UNKTYPE);
    /* 0xB64 */ UNKTYPE (dAcItem_c::*mFunc_0xB64)(UNKTYPE);
    /* 0xB70 */ UNKTYPE (dAcItem_c::*mFunc_0xB70)(UNKTYPE);
    /* 0xB7C */ UNKTYPE (dAcItem_c::*mFunc_0xB7C)(UNKTYPE);
    /* 0xB88 */ UNKTYPE (dAcItem_c::*mFunc_0xB88)(UNKTYPE);
    /* 0xB94 */ UNKTYPE (dAcItem_c::*mFunc_0xB94)(UNKTYPE);
    /* 0xBA0 */ UNKTYPE (dAcItem_c::*mFunc_0xBA0)(UNKTYPE);
    /* 0xBAC */ UNKTYPE (dAcItem_c::*mFunc_0xBAC)(UNKTYPE);
    /* 0xBB8 */ UNKTYPE (dAcItem_c::*mFunc_0xBB8)(UNKTYPE);
    /* 0xBC4 */ UNKTYPE (dAcItem_c::*mFunc_0xBC4)(UNKTYPE);
    /* 0xBD0 */ UNKTYPE (dAcItem_c::*mFunc_0xBD0)(UNKTYPE);
    /* 0xBDC */ UNKTYPE (dAcItem_c::*mFunc_0xBDC)(UNKTYPE);
    /* 0xBE8 */ UNKTYPE (dAcItem_c::*mFunc_0xBE8)(UNKTYPE);
    /* 0xBF4 */ UNKTYPE (dAcItem_c::*mFunc_0xBF4)(UNKTYPE);
    /* 0xC00 */ UNKTYPE (dAcItem_c::*mFunc_0xC00)(UNKTYPE);
    /* 0xC0C */ UNKTYPE (dAcItem_c::*mFunc_0xC0C)(UNKTYPE);
    /* 0xC18 */ UNKTYPE (dAcItem_c::*mFunc_0xC18)(UNKTYPE);
    /* 0xC24 */ UNKTYPE (dAcItem_c::*mFunc_0xC24)(UNKTYPE);
    /* 0xC30 */ UNKTYPE (dAcItem_c::*mFunc_0xC30)(UNKTYPE);
    /* 0xC3C */ UNKTYPE (dAcItem_c::*mFunc_0xC3C)(UNKTYPE);
    /* 0xC48 */ UNKTYPE (dAcItem_c::*mFunc_0xC48)(UNKTYPE);
    /* 0xC54 */ UNKTYPE (dAcItem_c::*mFunc_0xC54)(UNKTYPE);
    /* 0xC60 */ UNKTYPE (dAcItem_c::*mFunc_0xC60)(UNKTYPE);
    /* 0xC6C */ UNKTYPE (dAcItem_c::*mFunc_0xC6C)(UNKTYPE);
    /* 0xC78 */ UNKTYPE (dAcItem_c::*mFunc_0xC78)(UNKTYPE);
    /* 0xC84 */ UNKTYPE (dAcItem_c::*mFunc_0xC84)(UNKTYPE);
    /* 0xC90 */ UNKTYPE (dAcItem_c::*mFunc_0xC90)(UNKTYPE);
    /* 0xC9C */ UNKTYPE (dAcItem_c::*mFunc_0xC9C)(UNKTYPE);
    /* 0xCA8 */ UNKTYPE (dAcItem_c::*mFunc_0xCA8)(UNKTYPE);
    /* 0xCB4 */ UNKTYPE (dAcItem_c::*mFunc_0xCB4)(UNKTYPE);
    /* 0xCC0 */ dAcRef_c<dAcItem_c> mItemQueuePrev;
    /* 0xCCC */ dAcRef_c<dAcItem_c> mItemQueueNext;
    /* 0xCD8 */ f32 field_0xCD8;
    /* 0xCDC */ f32 mBaseScale;
    /* 0xCE0 */ f32 mCurrentScale;
    /* 0xCE4 */ f32 field_0xCE4;
    /* 0xCE8 */ f32 field_0xCE8;
    /* 0xCEC */ f32 field_0xCEC;
    /* 0xCF0 */ u8 _CF0[0xD00 - 0xCF0];
    /* 0xD00 */ f32 field_0xD00;
    /* 0xD04 */ f32 field_0xD04;
    /* 0xD08 */ u8 _D08[0xD14 - 0xD08];
    /* 0xD14 */ f32 mFreestandingOffsetH;
    /* 0xD18 */ f32 field_0xD18;
    /* 0xD1C */ int mGetItemPouchSlot;
    /* 0xD20 */ int field_0xD20;
    /* 0xD24 */ u8 _D24[0xD2C - 0xD24];
    /* 0xD2C */ u32 mFramesInAir;
    /* 0xD30 */ int mItemQuantity;
    /* 0xD34 */ u32 mItemFlags;
    /* 0xD38 */ mAng3_c field_0xD38;
    /* 0xD3E */ u16 field_0xD3E;
    /* 0xD40 */ mAng mMdlRotY;
    /* 0xD42 */ s16 field_0xD42;
    /* 0xD44 */ u16 mRealItemId;
    /* 0xD46 */ s16 mDespawnTimer;
    /* 0xD48 */ u8 field_0xD48;
    /* 0xD49 */ u8 field_0xD49;
    /* 0xD4A */ u8 field_0xD4A;
    /* 0xD4B */ u8 field_0xD4B;
    /* 0xD4C */ u8 field_0xD4C;
    /* 0xD4D */ u8 field_0xD4D;
    /* 0xD4E */ u8 mMdlScaleType;
    /* 0xD4F */ u8 mbNoDespawn;
    /* 0xD50 */ u8 mbNoGravity;
    /* 0xD51 */ u8 field_0xD51;
    /* 0xD52 */ u8 field_0xD52;
    /* 0xD53 */ u8 field_0xD53;
    /* 0xD54 */ u8 mbIsWaiting;
    /* 0xD55 */ u8 field_0xD55;
    /* 0xD56 */ u8 field_0xD56;
    /* 0xD57 */ u8 field_0xD57;
    /* 0xD58 */ u8 field_0xD58;
    /* 0xD59 */ u8 field_0xD59;
    /* 0xD5A */ u8 field_0xD5A;
    /* 0xD5B */ u8 field_0xD5B;
    /* 0xD5C */ u8 field_0xD5C;
    /* 0xD5D */ bool mbShowItemLighting;
    /* 0xD5E */ u8 field_0xD5E;
    /* 0xD5F */ u8 field_0xD5F;
    /* 0xD60 */ u8 field_0xD60;
    /* 0xD61 */ u8 field_0xD61;
    /* 0xD62 */ u8 field_0xD62;
    /* 0xD63 */ u8 field_0xD63;
    /* 0xD64 */ u8 field_0xD64;
    /* 0xD65 */ u8 field_0xD65;
    /* 0xD66 */ u8 field_0xD66;
    /* 0xD67 */ u8 field_0xD67;

public:
    static fLiMgBa_c sItemList;
};

#endif
