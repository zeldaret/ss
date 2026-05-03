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

class dItemMdl_c;
class dAcItem_c;

class dAcItem_0xB34 {
public:
    virtual ~dAcItem_0xB34() {}
    virtual void vt_0x0C() = 0;
    virtual void vt_0x10() = 0;
};

class dAcItem_0xB34_1 : public dAcItem_0xB34 {
public:
    virtual ~dAcItem_0xB34_1() {}
    virtual void vt_0x0C() override;
    virtual void vt_0x10() override;
};

class dAcItem_0xB34_2 : public dAcItem_0xB34 {
public:
    virtual ~dAcItem_0xB34_2() {}
    virtual void vt_0x0C() override;
    virtual void vt_0x10() override;

private:
    /* 0x04 */ u8 _0x04[0x0C - 0x04];
};

class dAcItem_0xB3C {
public:
    virtual ~dAcItem_0xB3C() {}
    virtual void vt_0x0C(u16) = 0;
    virtual void vt_0x10() = 0;
    virtual void vt_0x14() = 0;
    virtual void vt_0x18() = 0;

    void fn_802579D0();
    bool fn_80257A10(dAcItem_c*);
    bool fn_80257A80();
    void fn_80257AC0();
    bool fn_80257B10();

protected:
    /* 0x04 */ u8 _0x04[0x08 - 0x04];
};

class dAcItem_0xB3C_1 : public dAcItem_0xB3C {
public:
    dAcItem_0xB3C_1(u16, u16) {}
    virtual ~dAcItem_0xB3C_1();
    virtual void vt_0x0C(u16) override;
    virtual void vt_0x10() override;
    virtual void vt_0x14() override;
    virtual void vt_0x18() override;
};

class dAcItem_0xB3C_2 : public dAcItem_0xB3C {
public:
    dAcItem_0xB3C_2(u16 a1, u16 a2) {
        mCb1 = sCb1s[a1];
        mCb2 = sCb2s[a2];
    }
    virtual ~dAcItem_0xB3C_2();
    virtual void vt_0x0C(u16) override;
    virtual void vt_0x10() override;
    virtual void vt_0x14() override;
    virtual void vt_0x18() override;

public:
    typedef void (dAcItem_0xB3C_2::*Callback1)();
    typedef bool (dAcItem_0xB3C_2::*Callback2)();

    static Callback1 sCb1s[2];
    static Callback2 sCb2s[3];

    /* 0x08 */ Callback1 mCb1;
    /* 0x14 */ Callback2 mCb2;
    /* 0x20 */ u8 _0x20[0x24 - 0x20];
};

class dAcItem_c : public dAcItemBase_c {
public:
    dAcItem_c();
    virtual ~dAcItem_c();

    virtual bool createHeap() override;
    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    virtual u16 getItemIdFromParams() override;
    virtual void setItemId(u16 id);
    virtual bool shouldDespawn();
    virtual bool isItemSmallKeyOrHeartPieceOrStaminaFruit();

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
    // TODO should these be u16?
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
    static s32 getBowArrowCount();
    static s32 getTotalSeedCount();
    static s32 getSlingshotSeedCount();

    static s32 getTotalBombCapacity();
    static s32 getBowArrowCapacity();
    static s32 getTotalArrowCapacity();
    static s32 getTotalSeedCapacity();
    static s32 getSlingshotSeedCapacity();

    static s32 getRupeeCounter();
    static u32 getCurrentWalletCapacity();
    static u32 getExtraWalletCount();

    static u32 getHeartContainerHealthCount();
    static u32 getGratitudeCrystalCount();

    static u32 getTotalRequiredKeyPieceCount();
    static u32 getKeyPieceCount();
    static u32 getSmallKeyCount();

    static bool isJellyBlob(ITEM_ID item);
    static bool isStarryFirefly(ITEM_ID item);
    static bool isSlingshot(ITEM_ID item);
    static bool isRupee(ITEM_ID item);
    static bool isKeyPiece(ITEM_ID item);
    static bool isTreasure2(ITEM_ID item);
    static bool isTreasure(ITEM_ID item);
    static bool isTear(ITEM_ID item);
    static bool isAnyPouchItem(ITEM_ID item);
    static bool isUpgradedPotion(ITEM_ID item);

    bool isLightFruit() const;
    bool isSingleArrow() const;
    bool isArrowBundle() const;
    bool isAnyArrow() const;
    bool isTriforce() const;
    bool isGratitudeCrystal() const;
    bool is5GratitudeCrystals() const;
    bool isHeart() const;
    bool isSmallKey() const;
    bool isGreenRupee() const;
    bool isBlueRupee() const;
    bool isRedRupee() const;
    bool isRupoor() const;
    bool isSilverRupee() const;
    bool isGoldRupee() const;
    bool isSlingshot() const;
    bool isEldinOre() const;
    bool isJellyBlob() const;
    bool isUnkTreasureGroup1() const;
    bool isHeartPiece() const;
    bool isTreasure3() const;
    bool isInsect() const;
    bool isAnyRupee() const;
    bool isAnyTear() const;
    bool isBabyRattle() const;
    bool is10DekuSeeds() const;
    bool isAnyBombs() const;
    bool isLifeTreeFruit() const;
    bool isBirdStatuette() const;

    // These go through the vtable to retrieve the item id. Can't think of a better name for now
    bool isKeyPieceV() const;
    bool isHeartV() const;
    bool isTreasureV() const;
    bool isGratitudeCrystalV() const;
    bool is5GratitudeCrystalsV() const;
    bool isFairyV() const;
    bool isBottleFairyV() const;
    bool isStarryFireflyV() const;
    bool isAnyRupeeV() const;

    static void increaseRupeeCounter(s32 by);

    u32 getParams2Lower_shift1_0x7() const;

    static bool getItemGetEventName(u16 item, const char **outName);
    static void itemGetEventStart(dAcBase_c *);
    static void itemGetEventEnd(dAcBase_c *);

    static bool hasAnyFairy();

    enum Trial_e {
        TRIAL_SKYLOFT,
        TRIAL_FARON,
        TRIAL_ELDIN,
        TRIAL_LANAYRU,
        TRIAL_NONE,
    };

    static Trial_e getCurrentTrial();

    enum Tear_e {
        TEAR_FARON,
        TEAR_ELDIN,
        TEAR_LANAYRU,
        TEAR_GODDESS,
        TEAR_MAX
    };

    static Tear_e getTearSubtype(ITEM_ID item);

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

    static s32 sGetItemPouchSlot;
    static s32 sGetItemQuantity;

    static const mVec3_c sFreestandingDowsingOffset;
    static const mVec3_c sScale1Maybe;
    static const mVec3_c sScale2Maybe;

    typedef bool (dAcItem_c::*sStaticPtmf)();
    static const sStaticPtmf sStaticPtmfs[];

    static dAcRef_c<dAcObjBase_c> sItemListHead;
    static dAcRef_c<dAcObjBase_c> sItemListTail;

    static const ItemInitStruct *getItemInitStructForId(u16 itemId);
    const ItemInitStruct *getItemInitStruct() const;

    static void setTreasureTempCollect(u16 id);
    static void setSpawnQuantity(s32 quantity);
    static void unsetSpawnQuantity();

    void setObtainedItemId(u32, bool);
    bool checkItemFlagV() const;
    void setItemFlagV();
    bool checkFlagV() const;
    void setFlagV();
    void setSceneFlag();
    void setDungeonFlag();

    static void increaseKeyPieceCounter(s32 by);
    static void increaseHeartPieceCounter(s32 by);
    static void increaseTotalHeartPieceCounter(s32 by);
    static void increaseFaronGrasshopperCounter(s32 by);
    static s32 getFaronGrasshooperCounter();
    static void increaseWoodlandRhinoBeetleCounter(s32 by);
    static s32 getWoodlandRhinoBeetleCounter();
    static void increaseDekuHornetCounter(s32 by);
    static s32 getDekuHornetCounter();
    static void increaseSkyloftMantisCounter(s32 by);
    static s32 getSkyloftMantisCounter();
    static void increaseVolcanicLadybugCounter(s32 by);
    static s32 getVolcanicLadybugCounter();
    static void increaseBlessedButterflyCounter(s32 by);
    static s32 getBlessedButterflyCounter();
    static void increaseLanayruAntCounter(s32 by);
    static s32 getLanayruAntCounter();
    static void increaseSandCicadaCounter(s32 by);
    static s32 getSandCicadaCounter();
    static void increaseGerudoDragonflyCounter(s32 by);
    static s32 getGerudoDragonflyCounter();
    static void increaseEldinRollerCounter(s32 by);
    static s32 getEldinRollerCounter();
    static void increaseSkyStagBeetleCounter(s32 by);
    static s32 getSkyStagBeetleCounter();
    static void increaseStarryFireflyCounter(s32 by);
    static s32 getStarryFireflyCounter();
    static void increaseHornetLarvaeCounter(s32 by);
    static s32 getHornetLarvaeCounter();
    static void increaseBirdFeatherCounter(s32 by);
    static s32 getBirdFeatherCounter();
    static void increaseTumbleWeedCounter(s32 by);
    static s32 getTumbleWeedCounter();
    static void increaseLizardTailCounter(s32 by);
    static s32 getLizardTailCounter();
    static void increaseEldinOreCounter(s32 by);
    static s32 getEldinOreCounter();
    static void increaseAncientFlowerCounter(s32 by);
    static s32 getAncientFlowerCounter();
    static void increaseAmberRelicCounter(s32 by);
    static s32 getAmberRelicCounter();
    static void increaseDuskRelicCounter(s32 by);
    static s32 getDuskRelicCounter();
    static void increaseJellyBlobCounter(s32 by);
    static s32 getJellyBlobCounter();
    static void increaseMonsterClawCounter(s32 by);
    static s32 getMonsterClawCounter();
    static void increaseMonsterHornCounter(s32 by);
    static s32 getMonsterHornCounter();
    static void increaseSkullCounter(s32 by);
    static s32 getSkullCounter();
    static void increaseEvilCrystalCounter(s32 by);
    static s32 getEvilCrystalCounter();
    static void increaseBlueBirdFeatherCounter(s32 by);
    static s32 getBlueBirdFeatherCounter();
    static void increaseGoldenSkullCounter(s32 by);
    static s32 getGoldenSkullCounter();
    static void increaseGoddessPlumeCounter(s32 by);
    static s32 getGoddessPlumeCounter();
    static void increaseGratitudeCrystalCounter(s32 by);
    static s32 getGratitudeCrystalCounter();
    static void increaseExtraWalletCounter(s32 by);
    static s32 getExtraWalletCounter();
    static void increaseSmallKeyCounter(s32 by);
    static void increaseArrowCounter(s32 by);
    static void increaseDekuSeedCounter(s32 by);
    static void increaseArrowAndPouchCounter(s32 by);
    static void increaseBombAndPouchCounter(s32 by);
    static void increaseDekuSeedAndPouchCounter(s32 by);
    static void increaseExtraPouchCounter(s32 by);

    static void increaseHealthCapacity(s32 by);
    static void setLifeTreeSeedlingFlag(s32 by);

    static s32 getLightFruitTime();
    static s32 getNumRemainingHeartPiecesForNextHeart();
    static void doFullHeal();
    static void restoreStamina();

    bool isItemDropFromEnemy() const;
    static u16 determineActualItemIdFromItem(u16);
    u16 getItemId2() const;
    static s32 getSubtypeFromParam(u32 params);
    bool isFirstBitParams2NotSet();
    void setupUnkColliderFlags2();
    static bool hitCallback(dAcObjBase_c *i_actorA, cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB);
    void fn_80253F90(bool);
    void fn_80256790();
    void fn_802567D0();
    void onTouchLink();
    void fn_80255DF0();
    void fn_80254BB0();
    void setDespawnTimer(u16 timer);
    void applyBoundingBox();
    s32 getTearIdx();
    void setFreestandingYOffset(f32);
    void setFramesInAir(u32 numFrames);
    void unsetFramesInAir();
    void incrementFramesInAir();
    void setItemFlags(u32 flags);

    bool fn_80255CF0();
    bool fn_802574A0();
    void addToGetQueue();
    void removeFromGetQueue();
    static bool fn_8024A230();
    void unsetHaveNoGravity();
    void fn_80252A80();
    void tickDespawnTimer();
    void fn_80253D50();
    bool fn_802577A0();
    void fn_80256F20();
    bool fn_80256E80();
    bool fn_80255C50(u32 specialCode);
    bool fn_80255CA0(u32 specialCode);
    void getCurrentModelScale(f32 *scale);
    f32 getCurrentScale();
    void fn_802518C0(mVec3_c *out);
    void makeLinkLookTowardItem();
    void fn_80254CA0();

    void fn_802546A0();
    void fn_80254710();
    void fn_80254790();

    bool checkAbovePosition();

    bool isMdlHidden() const;
    static bool isBlinkBeforeDespawnShown(s32 timer);

    void performCollection();
    void performCollectionPart1();
    void performCollectionPart2();

    void fn_80254BC0();
    void fn_80255B10();
    void updateLightingMaybe();

    void fn_80253E20();
    void fn_80254BA0();

    void setNotWaiting();
    void fn_802548A0();
    void decideOnGetOrForcedGet();

    void fn_80256710(mVec3_c &);
    void fn_80257560(const mVec3_c &);
    void fn_80254D10();
    bool fn_80254D70();
    void fn_802542E0();

    static bool fn_80247BB0();
    bool checkShouldDemo();
    bool fn_80254EC0();
    void addGetEvent();
    void fn_80254590(mVec3_c &);
    void fn_80254680();
    void fn_80254810();
    static s16 getItemRotateAngle();

    static void getItemGetEventName(u16 id, char *const *name);

    // Could also return vector
    static void fn_80247540(mVec3_c &);
    void fn_80247560(const mVec3_c &);

    // static PTMF callbacks
    bool fn_80248020();
    bool fn_80248010();
    bool fn_80255B30();
    bool fn_80255BA0();
    bool fn_80248040();
    bool fn_80248030();
    bool fn_80255BD0();
    bool fn_80255C40();

    // PTMF callbacks
    f32 fn_80254DE0();
    f32 fn_80254DF0();

    f32 getSmallKeyFreestandingScale();
    f32 getLowValueRupeeFreestandingScale();
    f32 getHighValueRupeeFreestandingScale();
    f32 getSlingshotFreestandingScale();
    f32 getEldinOreFreestandingScale();
    f32 getJellyBlobFreestandingScale();
    f32 getUnkTreasureGroup1FreestandingScale();
    f32 getHeartPieceFreestandingScale();
    f32 getGratitudeCrystalFreestandingScale();
    f32 getDefaultFreestandingScale();

    f32 callGetFreestandingModelScale();

    f32 getTreasure3Scale();
    f32 getInsectScale();
    f32 getDefaultScale();

    f32 getHighValueRupeePickupScale();
    f32 getDefaultPickupScale();

    void moveNormal0();
    void moveNormal1();
    void moveSpecial();

    void fn_802555D0();
    void fn_802555F0();

    void bounceNone();
    void bounceNormal();
    void bounceRupee();

    void fn_802558F0();
    void fn_80255AE0();
    void fn_802558E0();

    bool fn_80248060();
    bool fn_80248050();
    bool fn_80255D50();
    bool fn_80255DB0();

    void setLowValueRupeeBoundingBox();
    void setHighValueRupeeBoundingBox();
    void setArrowBundleBoundingBox();
    void setDefaultBoundingBox();

    f32 getSmallKeyGravity1();
    f32 getGreenRupeeGravity1();
    f32 getBlueRupeeGravity1();
    f32 getRedRupeeGravity1();
    f32 getHeartGravity1();
    f32 getSingleArrowGravity1();
    f32 getArrowBundleGravity1();
    f32 getSilverRupeeGravity1();
    f32 getGoldRupeeGravity1();
    f32 getRupoorGravity1();
    f32 get5BombsGravity1();
    f32 get10BombsGravity1();
    f32 getFaroreTearGravity1();
    f32 get5SeedsGravity1();
    f32 getDefaultGravity1();

    f32 getHeartGravity2();

    f32 getGreenRupeeGravity3();
    f32 getBlueRupeeGravity3();
    f32 getRedRupeeGravity3();
    f32 getRupoorGravity3();
    f32 getSilverRupeeGravity3();
    f32 getGoldRupeeGravity3();

    f32 getSmallKeyGravity4();
    f32 getGreenRupeeGravity4();
    f32 getBlueRupeeGravity4();
    f32 getRedRupeeGravity4();
    f32 getHeartGravity4();
    f32 getSingleArrowGravity4();
    f32 getArrowBundleGravity4();
    f32 getSilverRupeeGravity4();
    f32 getGoldRupeeGravity4();
    f32 getRupoorGravity4();
    f32 get5BombsGravity4();
    f32 get10BombsGravity4();
    f32 getFaroreTearGravity4();
    f32 get5SeedsGravity4();
    f32 getDefaultGravit4();

    f32 getGreenRupeeDirHitKnockback();
    f32 getBlueRupeeDirHitKnockback();
    f32 getRedRupeeDirHitKnockback();
    f32 getSilverRupeeDirHitKnockback();
    f32 getGoldRupeeDirHitKnockback();
    f32 getRupoorDirHitKnockback();

    f32 getGreenRupeeDirHitKnockbackRand();
    f32 getBlueRupeeDirHitKnockbackRand();
    f32 getRedRupeeDirHitKnockbackRand();
    f32 getSilverRupeeDirHitKnockbackRand();
    f32 getGoldRupeeDirHitKnockbackRand();
    f32 getRupoorDirHitKnockbackRand();

    f32 getGreenRupeeHitKnockback();
    f32 getBlueRupeeHitKnockback();
    f32 getRedRupeeHitKnockback();
    f32 getSilverRupeeHitKnockback();
    f32 getGoldRupeeHitKnockback();
    f32 getRupoorHitKnockback();

    f32 getGreenRupeeHitKnockbackRand();
    f32 getBlueRupeeHitKnockbackRand();
    f32 getRedRupeeHitKnockbackRand();
    f32 getSilverRupeeHitKnockbackRand();
    f32 getGoldRupeeHitKnockbackRand();
    f32 getRupoorHitKnockbackRand();

    f32 getGreenRupeeHitSquareDistance2();
    f32 getBlueRupeeHitSquareDistance2();
    f32 getRedRupeeHitSquareDistance2();
    f32 getSilverRupeeHitSquareDistance2();
    f32 getGoldRupeeHitSquareDistance2();
    f32 getRupoorHitSquareDistance2();
    f32 getHeartHitSquareDistance2();

    f32 getGreenRupeeDirHitKnockback2();
    f32 getBlueRupeeDirHitKnockback2();
    f32 getRedRupeeDirHitKnockback2();
    f32 getSilverRupeeDirHitKnockback2();
    f32 getGoldRupeeDirHitKnockback2();
    f32 getRupoorDirHitKnockback2();
    f32 getHeartDirHitKnockback2();

    f32 getGreenRupeeDirHitKnockbackRand2();
    f32 getBlueRupeeDirHitKnockbackRand2();
    f32 getRedRupeeDirHitKnockbackRand2();
    f32 getSilverRupeeDirHitKnockbackRand2();
    f32 getGoldRupeeDirHitKnockbackRand2();
    f32 getRupoorDirHitKnockbackRand2();
    f32 getHeartDirHitKnockbackRand2();

    f32 getGreenRupeeHitKnockback2();
    f32 getBlueRupeeHitKnockback2();
    f32 getRedRupeeHitKnockback2();
    f32 getSilverRupeeHitKnockback2();
    f32 getGoldRupeeHitKnockback2();
    f32 getRupoorHitKnockback2();
    f32 getHeartHitKnockback2();

    f32 getGreenRupeeHitKnockbackRand2();
    f32 getBlueRupeeHitKnockbackRand2();
    f32 getRedRupeeHitKnockbackRand2();
    f32 getSilverRupeeHitKnockbackRand2();
    f32 getGoldRupeeHitKnockbackRand2();
    f32 getRupoorHitKnockbackRand2();
    f32 getHeartHitKnockbackRand2();

    f32 getGreenRupeeDirHitKnockback3();
    f32 getBlueRupeeDirHitKnockback3();
    f32 getRedRupeeDirHitKnockback3();
    f32 getSilverRupeeDirHitKnockback3();
    f32 getGoldRupeeDirHitKnockback3();
    f32 getRupoorDirHitKnockback3();
    f32 getHeartDirHitKnockback3();

    f32 getGreenRupeeDirHitKnockbackRand3();
    f32 getBlueRupeeDirHitKnockbackRand3();
    f32 getRedRupeeDirHitKnockbackRand3();
    f32 getSilverRupeeDirHitKnockbackRand3();
    f32 getGoldRupeeDirHitKnockbackRand3();
    f32 getRupoorDirHitKnockbackRand3();
    f32 getHeartDirHitKnockbackRand3();

    f32 getGreenRupeeHitKnockback3();
    f32 getBlueRupeeHitKnockback3();
    f32 getRedRupeeHitKnockback3();
    f32 getSilverRupeeHitKnockback3();
    f32 getGoldRupeeHitKnockback3();
    f32 getRupoorHitKnockback3();
    f32 getHeartHitKnockback3();

    f32 getGreenRupeeHitKnockbackRand3();
    f32 getBlueRupeeHitKnockbackRand3();
    f32 getRedRupeeHitKnockbackRand3();
    f32 getSilverRupeeHitKnockbackRand3();
    f32 getGoldRupeeHitKnockbackRand3();
    f32 getRupoorHitKnockbackRand3();
    f32 getHeartHitKnockbackRand3();

    f32 fn_802577C0();
    f32 fn_802577D0();

    void fn_80255E80();
    void fn_80255F40();

    /* 0x334 */ dItemMdl_c *mpMdl;
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
    /* 0xAA0 */ dAcRef_c<dAcObjBase_c> mForceSignRef;
    /* 0xAAC */ mVec3_c posCopy;
    /* 0xAB8 */ fLiNdBa_c mNode;
    /* 0xAC4 */ ActorEventRelated mEventRelated;
    /* 0xB14 */ DowsingTarget mDowsingTarget;
    /* 0xB34 */ dAcItem_0xB34 *field_0xB34[2];
    /* 0xB34 */ dAcItem_0xB3C *field_0xB3C;
    /* 0xB40 */ void (dAcItem_c::*mFnAction)();
    /* 0xB4C */ void (dAcItem_c::*mFnBounce)();
    /* 0xB58 */ void (dAcItem_c::*mFunc_0xB58)();
    /* 0xB64 */ void (dAcItem_c::*mFnAction3)();
    /* 0xB70 */ bool (dAcItem_c::*mFnAction4)();
    /* 0xB7C */ f32 (dAcItem_c::*mFnGetGravity1)();
    /* 0xB88 */ f32 (dAcItem_c::*mFnGetHeartGravity)();
    /* 0xB94 */ f32 (dAcItem_c::*mFnGetRupeeGravity)();
    /* 0xBA0 */ f32 (dAcItem_c::*mFnGetGravity4)();
    /* 0xBAC */ UNKTYPE (dAcItem_c::*mFunc_0xBAC)(UNKTYPE);
    /* 0xBB8 */ UNKTYPE (dAcItem_c::*mFunc_0xBB8)(UNKTYPE);
    /* 0xBC4 */ f32 (dAcItem_c::*mFnGetFreestandingScale)();
    /* 0xBD0 */ f32 (dAcItem_c::*mFnGetScale)();
    /* 0xBDC */ f32 (dAcItem_c::*mFnGetPickupDemoScale)();
    /* 0xBE8 */ f32 (dAcItem_c::*mFnCallGetFreestandingScale)();
    /* 0xBF4 */ f32 (dAcItem_c::*mFnGetDirHitKnockback)();
    /* 0xC00 */ f32 (dAcItem_c::*mFnGetDirHitKnockbackRand)();
    /* 0xC0C */ f32 (dAcItem_c::*mFnGetHitKnockback)();
    /* 0xC18 */ f32 (dAcItem_c::*mFnGetHitKnockbackRand)();
    /* 0xC24 */ f32 (dAcItem_c::*mFnGetHitSquareDistance2)();
    /* 0xC30 */ f32 (dAcItem_c::*mFnGetDirHitKnockback2)();
    /* 0xC3C */ f32 (dAcItem_c::*mFnGetDirHitKnockbackRand2)();
    /* 0xC48 */ f32 (dAcItem_c::*mFnGetHitKnockback2)();
    /* 0xC54 */ f32 (dAcItem_c::*mFnGetHitKnockbackRand2)();
    /* 0xC60 */ f32 (dAcItem_c::*mFnGetDirHitKnockback3)();
    /* 0xC6C */ f32 (dAcItem_c::*mFnGetDirHitKnockbackRand3)();
    /* 0xC78 */ f32 (dAcItem_c::*mFnGetHitKnockback3)();
    /* 0xC84 */ f32 (dAcItem_c::*mFnGetHitKnockbackRand3)();
    /* 0xC90 */ f32 (dAcItem_c::*mFn_0xC90)();
    /* 0xC9C */ void (dAcItem_c::*mFn_0xC9C)();
    /* 0xCA8 */ void (dAcItem_c::*mFnSetBoundingBox)();
    /* 0xCB4 */ f32 (dAcItem_c::*mFn_0xCB4)(void);
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
    /* 0xD08 */ u8 _D08[0xD0C - 0xD08];
    /* 0xD0C */ f32 field_0xD0C;
    /* 0xD10 */ u8 _D10[0xD14 - 0xD10];
    /* 0xD14 */ f32 mFreestandingOffsetH;
    /* 0xD18 */ f32 field_0xD18;
    /* 0xD1C */ s32 mGetItemPouchSlot;
    /* 0xD20 */ s32 field_0xD20;
    /* 0xD24 */ u8 _D24[0xD2C - 0xD24];
    /* 0xD2C */ u32 mFramesInAir;
    /* 0xD30 */ u32 mItemQuantity;
    /* 0xD34 */ u32 mItemFlags;
    /* 0xD38 */ s16 field_0xD38;
    /* 0xD3A */ s16 field_0xD3A;
    /* 0xD3C */ s16 field_0xD3C;
    /* 0xD3E */ u16 field_0xD3E;
    /* 0xD40 */ s16 mMdlRotY;
    /* 0xD42 */ s16 field_0xD42;
    /* 0xD44 */ u16 mRealItemId;
    /* 0xD46 */ u16 mDespawnTimer;
    /* 0xD48 */ mAng field_0xD48;
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
