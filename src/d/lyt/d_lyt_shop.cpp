#include "d/lyt/d_lyt_shop.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/d_cs_base.h"
#include "d/d_cs_game.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_message.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/d_rumble.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "d/lyt/d_lyt_common_icon_material.h"
#include "d/lyt/d_lyt_util_items.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_profile.h"
#include "m/m_pad.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/types_nw4r.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/counters/counter.h"

#include <cwchar>

struct ShopUpgradeMaterialCost {
    /* 0x0 */ s16 itemId;
    /* 0x2 */ s16 amount;
};

struct ShopUpgradeCosts {
    /* 0x0 */ s16 newItemId;
    /* 0x2 */ s16 oldItemId;
    /* 0x4 */ s16 price;
    /* 0x6 */ ShopUpgradeMaterialCost matCosts[4];
};

struct ShopUpgradeItemData {
    /* 0x0 */ s32 itemId;
    /* 0x4 */ bool canBeUpgraded;
};

static const ShopUpgradeCosts sGearUpgrades[] = {
    {      ITEM_BANDED_SHIELD,
     ITEM_WOODEN_SHIELD,  30,
     {
     {ITEM_AMBER_RELIC, 2},
     {ITEM_MONSTER_CLAW, 1},
     {ITEM_JELLY_BLOB, 1},
     {-1, -1},
     }},
    {      ITEM_BRACED_SHIELD,
     ITEM_BANDED_SHIELD,  50,
     {
     {ITEM_AMBER_RELIC, 3},
     {ITEM_MONSTER_CLAW, 2},
     {ITEM_TUMBLE_WEED, 2},
     {ITEM_ORNAMENTAL_SKULL, 1},
     }},
    {  ITEM_REINFORCED_SHIELD,
     ITEM_IRON_SHIELD,  50,
     {
     {ITEM_ELDIN_ORE, 2},
     {ITEM_MONSTER_CLAW, 2},
     {ITEM_ORNAMENTAL_SKULL, 2},
     {-1, -1},
     }},
    {   ITEM_FORTIFIED_SHIELD,
     ITEM_REINFORCED_SHIELD, 100,
     {
     {ITEM_ELDIN_ORE, 3},
     {ITEM_MONSTER_CLAW, 3},
     {ITEM_TUMBLE_WEED, 3},
     {ITEM_BLUE_BIRD_FEATHER, 1},
     }},
    {      ITEM_DIVINE_SHIELD,
     ITEM_SACRED_SHIELD, 100,
     {
     {ITEM_DUSK_RELIC, 1},
     {ITEM_BIRD_FEATHER, 2},
     {ITEM_ORNAMENTAL_SKULL, 3},
     {-1, -1},
     }},
    {     ITEM_GODDESS_SHIELD,
     ITEM_DIVINE_SHIELD, 150,
     {
     {ITEM_DUSK_RELIC, 4},
     {ITEM_BIRD_FEATHER, 3},
     {ITEM_MONSTER_HORN, 3},
     {ITEM_BLUE_BIRD_FEATHER, 1},
     }},
    { ITEM_MIGHTY_SCATTERSHOT,
     ITEM_SLINGSHOT,  50,
     {
     {ITEM_DUSK_RELIC, 2},
     {ITEM_AMBER_RELIC, 2},
     {ITEM_JELLY_BLOB, 3},
     {-1, -1},
     }},
    {       ITEM_QUICK_BEETLE,
     ITEM_HOOK_BEETLE,  50,
     {
     {ITEM_ANCIENT_FLOWER, 2},
     {ITEM_HORNET_LARVAE, 2},
     {ITEM_GOLDEN_SKULL, 1},
     {-1, -1},
     }},
    {       ITEM_TOUGH_BEETLE,
     ITEM_QUICK_BEETLE,  50,
     {
     {ITEM_ANCIENT_FLOWER, 3},
     {ITEM_AMBER_RELIC, 4},
     {ITEM_BLUE_BIRD_FEATHER, 1},
     {ITEM_GODDESS_PLUME, 1},
     }},
    {           ITEM_IRON_BOW,
     ITEM_BOW,  50,
     {
     {ITEM_TUMBLE_WEED, 3},
     {ITEM_EVIL_CRYSTAL, 1},
     {ITEM_MONSTER_CLAW, 3},
     {ITEM_ELDIN_ORE, 2},
     }},
    {         ITEM_SACRED_BOW,
     ITEM_IRON_BOW, 100,
     {
     {ITEM_TUMBLE_WEED, 5},
     {ITEM_EVIL_CRYSTAL, 2},
     {ITEM_LIZARD_TAIL, 3},
     {ITEM_GODDESS_PLUME, 1},
     }},
    {ITEM_MEDIUM_SEED_SATCHEL,
     ITEM_SMALL_SEED_SATCHEL,  50,
     {
     {ITEM_AMBER_RELIC, 4},
     {ITEM_MONSTER_CLAW, 3},
     {ITEM_ORNAMENTAL_SKULL, 3},
     {-1, -1},
     }},
    { ITEM_LARGE_SEED_SATCHEL,
     ITEM_MEDIUM_SEED_SATCHEL, 100,
     {
     {ITEM_AMBER_RELIC, 5},
     {ITEM_MONSTER_CLAW, 3},
     {ITEM_GOLDEN_SKULL, 1},
     {ITEM_BLUE_BIRD_FEATHER, 1},
     }},
    {      ITEM_MEDIUM_QUIVER,
     ITEM_SMALL_QUIVER,  50,
     {
     {ITEM_MONSTER_HORN, 3},
     {ITEM_DUSK_RELIC, 3},
     {ITEM_AMBER_RELIC, 5},
     {-1, -1},
     }},
    {       ITEM_LARGE_QUIVER,
     ITEM_MEDIUM_QUIVER, 100,
     {
     {ITEM_MONSTER_HORN, 2},
     {ITEM_DUSK_RELIC, 3},
     {ITEM_GOLDEN_SKULL, 1},
     {ITEM_GODDESS_PLUME, 1},
     }},
    {    ITEM_MEDIUM_BOMB_BAG,
     ITEM_SMALL_BOMB_BAG,  50,
     {
     {ITEM_LIZARD_TAIL, 1},
     {ITEM_ORNAMENTAL_SKULL, 3},
     {ITEM_BLUE_BIRD_FEATHER, 1},
     {-1, -1},
     }},
    {     ITEM_LARGE_BOMB_BAG,
     ITEM_MEDIUM_BOMB_BAG, 100,
     {
     {ITEM_LIZARD_TAIL, 3},
     {ITEM_JELLY_BLOB, 4},
     {ITEM_HORNET_LARVAE, 2},
     {ITEM_GOLDEN_SKULL, 1},
     }},
    {        ITEM_BIG_BUG_NET,
     ITEM_BUG_NET, 100,
     {
     {ITEM_TUMBLE_WEED, 3},
     {ITEM_ANCIENT_FLOWER, 2},
     {ITEM_EVIL_CRYSTAL, 1},
     {-1, -1},
     }},
};

static const ShopUpgradeCosts sPotionUpgrades[] = {
    {            ITEM_HEART_POTION_PLUS,
     ITEM_HEART_POTION, 20,
     {
     {ITEM_BLESSED_BUTTERFLY, 3},
     {ITEM_WOODLAND_RHINO_BEETLE, 1},
     {ITEM_VOLCANIC_LADYBUG, 1},
     {-1, -1},
     }},
    {       ITEM_HEART_POTION_PLUS_PLUS,
     ITEM_HEART_POTION_PLUS, 30,
     {
     {ITEM_BLESSED_BUTTERFLY, 3},
     {ITEM_DEKU_HORNET, 3},
     {ITEM_SAND_CICADA, 1},
     {ITEM_ELDIN_ROLLER, 1},
     }},
    {              ITEM_AIR_POTION_PLUS,
     ITEM_AIR_POTION, 20,
     {
     {ITEM_SKY_STAG_BEETLE, 2},
     {ITEM_SKYLOFT_MANTIS, 2},
     {ITEM_LANAYRU_ANT, 2},
     {-1, -1},
     }},
    {          ITEM_STAMINA_POTION_PLUS,
     ITEM_STAMINA_POTION, 20,
     {
     {ITEM_VOLCANIC_LADYBUG, 3},
     {ITEM_SKY_STAG_BEETLE, 2},
     {ITEM_GERUDO_DRAGONFLY, 2},
     {ITEM_FARON_GRASSHOPPER, 1},
     }},
    {     ITEM_REVITALIZING_POTION_PLUS,
     ITEM_REVITALIZING_POTION, 20,
     {
     {ITEM_WOODLAND_RHINO_BEETLE, 2},
     {ITEM_SKYLOFT_MANTIS, 2},
     {ITEM_DEKU_HORNET, 3},
     {-1, -1},
     }},
    {ITEM_REVITALIZING_POTION_PLUS_PLUS,
     ITEM_REVITALIZING_POTION_PLUS, 30,
     {
     {ITEM_WOODLAND_RHINO_BEETLE, 2},
     {ITEM_LANAYRU_ANT, 3},
     {ITEM_GERUDO_DRAGONFLY, 1},
     {ITEM_SAND_CICADA, 1},
     }},
    {         ITEM_GUARDIAN_POTION_PLUS,
     ITEM_GUARDIAN_POTION, 40,
     {
     {ITEM_FARON_GRASSHOPPER, 1},
     {ITEM_ELDIN_ROLLER, 2},
     {ITEM_BLESSED_BUTTERFLY, 3},
     {ITEM_STARRY_FIREFLY, 3},
     }},
};

static const ShopUpgradeItemData sItemData[] = {
    {                      ITEM_BUG_NET,  true},
    {                  ITEM_BIG_BUG_NET, false},
    {                    ITEM_SLINGSHOT,  true},
    {           ITEM_MIGHTY_SCATTERSHOT, false},
    {                  ITEM_HOOK_BEETLE,  true},
    {                 ITEM_QUICK_BEETLE,  true},
    {                 ITEM_TOUGH_BEETLE, false},
    {                          ITEM_BOW,  true},
    {                     ITEM_IRON_BOW,  true},
    {                   ITEM_SACRED_BOW, false},
    {                ITEM_WOODEN_SHIELD,  true},
    {                ITEM_BANDED_SHIELD,  true},
    {                ITEM_BRACED_SHIELD, false},
    {                  ITEM_IRON_SHIELD,  true},
    {            ITEM_REINFORCED_SHIELD,  true},
    {             ITEM_FORTIFIED_SHIELD, false},
    {                ITEM_SACRED_SHIELD,  true},
    {                ITEM_DIVINE_SHIELD,  true},
    {               ITEM_GODDESS_SHIELD, false},
    {           ITEM_SMALL_SEED_SATCHEL,  true},
    {          ITEM_MEDIUM_SEED_SATCHEL,  true},
    {           ITEM_LARGE_SEED_SATCHEL, false},
    {               ITEM_SMALL_BOMB_BAG,  true},
    {              ITEM_MEDIUM_BOMB_BAG,  true},
    {               ITEM_LARGE_BOMB_BAG, false},
    {                 ITEM_SMALL_QUIVER,  true},
    {                ITEM_MEDIUM_QUIVER,  true},
    {                 ITEM_LARGE_QUIVER, false},
    {                 ITEM_HEART_POTION,  true},
    {            ITEM_HEART_POTION_PLUS,  true},
    {       ITEM_HEART_POTION_PLUS_PLUS, false},
    {                   ITEM_AIR_POTION,  true},
    {              ITEM_AIR_POTION_PLUS, false},
    {               ITEM_STAMINA_POTION,  true},
    {          ITEM_STAMINA_POTION_PLUS, false},
    {          ITEM_REVITALIZING_POTION,  true},
    {     ITEM_REVITALIZING_POTION_PLUS,  true},
    {ITEM_REVITALIZING_POTION_PLUS_PLUS, false},
    {              ITEM_GUARDIAN_POTION,  true},
    {         ITEM_GUARDIAN_POTION_PLUS, false},
    {            ITEM_FARON_GRASSHOPPER, false},
    {        ITEM_WOODLAND_RHINO_BEETLE, false},
    {                  ITEM_DEKU_HORNET, false},
    {               ITEM_SKYLOFT_MANTIS, false},
    {             ITEM_VOLCANIC_LADYBUG, false},
    {            ITEM_BLESSED_BUTTERFLY, false},
    {                  ITEM_LANAYRU_ANT, false},
    {                  ITEM_SAND_CICADA, false},
    {             ITEM_GERUDO_DRAGONFLY, false},
    {                 ITEM_ELDIN_ROLLER, false},
    {              ITEM_SKY_STAG_BEETLE, false},
    {               ITEM_STARRY_FIREFLY, false},
    {                ITEM_HORNET_LARVAE, false},
    {                 ITEM_BIRD_FEATHER, false},
    {                  ITEM_TUMBLE_WEED, false},
    {                  ITEM_LIZARD_TAIL, false},
    {                    ITEM_ELDIN_ORE, false},
    {               ITEM_ANCIENT_FLOWER, false},
    {                  ITEM_AMBER_RELIC, false},
    {                   ITEM_DUSK_RELIC, false},
    {                   ITEM_JELLY_BLOB, false},
    {                 ITEM_MONSTER_CLAW, false},
    {                 ITEM_MONSTER_HORN, false},
    {             ITEM_ORNAMENTAL_SKULL, false},
    {                 ITEM_EVIL_CRYSTAL, false},
    {            ITEM_BLUE_BIRD_FEATHER, false},
    {                 ITEM_GOLDEN_SKULL, false},
    {                ITEM_GODDESS_PLUME, false},
};

// clang-format off
static const s32 sLytIconMapping[68] = {
    LYT_CMN_ItemBugnet,
    LYT_CMN_ItemBigBugnet,
    LYT_CMN_ItemSlingshot,
    LYT_CMN_ItemScattershot,
    LYT_CMN_ItemHookBeetle,
    LYT_CMN_ItemQuickBeetle,
    LYT_CMN_ItemToughBeetle,
    LYT_CMN_ItemBow,
    LYT_CMN_ItemIronBow,
    LYT_CMN_ItemSacredBow,
    LYT_CMN_PouchShieldWoodA,
    LYT_CMN_PouchShieldWoodB,
    LYT_CMN_PouchShieldWoodC,
    LYT_CMN_PouchShieldIronA,
    LYT_CMN_PouchShieldIronB,
    LYT_CMN_PouchShieldIronC,
    LYT_CMN_PouchShieldHolyA,
    LYT_CMN_PouchShieldHolyB,
    LYT_CMN_PouchShieldHolyC,
    LYT_CMN_PouchSeedSatchelSmall,
    LYT_CMN_PouchSeedSatchelMed,
    LYT_CMN_PouchSeedSatchelBig,
    LYT_CMN_PouchBombBagSmall,
    LYT_CMN_PouchBombBagMed,
    LYT_CMN_PouchBombBagBig,
    LYT_CMN_PouchQuiverSmall,
    LYT_CMN_PouchQuiverMed,
    LYT_CMN_PouchQuiverBig,
    LYT_CMN_PouchPotionHealth,
    LYT_CMN_PouchPotionHealthPlus,
    LYT_CMN_PouchPotionHealthPlusPlusFull,
    LYT_CMN_PouchPotionAir,
    LYT_CMN_PouchPotionAirPlus,
    LYT_CMN_PouchPotionStamina,
    LYT_CMN_PouchPotionStaminaPlus,
    LYT_CMN_PouchPotionRepair,
    LYT_CMN_PouchPotionRepairPlus,
    LYT_CMN_PouchPotionRepairPlusPlusFull,
    LYT_CMN_PouchPotionGuardian,
    LYT_CMN_PouchPotionGuardianPlus,
    LYT_CMN_BugFaronGrasshopper,
    LYT_CMN_BugWoodlandRhinoBetle,
    LYT_CMN_BugDekuHornet,
    LYT_CMN_BugSkyloftMantis,
    LYT_CMN_BugVolcanicLadyBug,
    LYT_CMN_BugBlessedButterfly,
    LYT_CMN_BugLanayruAnt,
    LYT_CMN_BugSandCicada,
    LYT_CMN_BugGerudoDragonfly,
    LYT_CMN_BugEldinRoller,
    LYT_CMN_BugSkyStagBeetle,
    LYT_CMN_BugStarryFirefly,
    LYT_CMN_HORNET_LARVAE,
    LYT_CMN_BIRD_FEATHER,
    LYT_CMN_TUMBLEWEED,
    LYT_CMN_LIZALFOS_TAIL,
    LYT_CMN_ELDIN_ORE,
    LYT_CMN_ANCIENT_FLOWER,
    LYT_CMN_AMBER_RELIC,
    LYT_CMN_DUSK_RELOC,
    LYT_CMN_JELLY_BLOB,
    LYT_CMN_MONSTER_CLAW,
    LYT_CMN_MONSTER_HORN,
    LYT_CMN_ORNAMENTAL_SKULL,
    LYT_CMN_EVIL_CRYSTAL,
    LYT_CMN_BLUE_BIRD_FEATHER,
    LYT_CMN_GOLDEN_SKULL,
    LYT_CMN_GODDESS_PLUME,
};
// clang-format on

STATE_DEFINE(dLytShopMain_c, ModeNone);
STATE_DEFINE(dLytShopMain_c, ModeSelectIn);
STATE_DEFINE(dLytShopMain_c, ModeSelectCheck);
STATE_DEFINE(dLytShopMain_c, ModeSelectOut);
STATE_DEFINE(dLytShopMain_c, ModeOutIn);
STATE_DEFINE(dLytShopMain_c, ModeMaterialCheck);
STATE_DEFINE(dLytShopMain_c, ModeOut);
STATE_DEFINE(dLytShopMain_c, ModeInOut);

static const char *sLytNames[] = {
    "itemSelect_00.brlyt",
    "materialCheck_00.brlyt",
    "shopInfo_00.brlyt",
};

static const d2d::LytBrlanMapping brlanMapItemSelect[] = {
    {        "itemSelect_00_in.brlan",     "G_inOut_00"},
    {  "itemSelect_00_btnOnOff.brlan",       "G_btn_00"},
    {  "itemSelect_00_btnOnOff.brlan",    "G_onOffB_00"},
    {   "itemSelect_00_btnLoop.brlan",   "G_btnLoop_00"},
    { "itemSelect_00_btnDecide.brlan",       "G_btn_00"},
    {       "itemSelect_00_out.brlan",     "G_inOut_00"},
    {      "itemSelect_00_loop.brlan",      "G_loop_00"},
    { "itemSelect_00_btnCancel.brlan", "G_btnCancel_00"},
    {"itemSelect_00_shopChange.brlan",    "G_change_00"},
};

static const d2d::LytBrlanMapping brlanMapMaterialCheck[] = {
    {        "materialCheck_00_in.brlan",      "G_inOut_00"},
    {       "materialCheck_00_out.brlan",      "G_inOut_00"},
    {  "materialCheck_00_btnOnOff.brlan",        "G_btn_00"},
    {  "materialCheck_00_btnOnOff.brlan",        "G_btn_01"},
    {  "materialCheck_00_btnOnOff.brlan",     "G_onOffB_00"},
    { "materialCheck_00_btnDecide.brlan",        "G_btn_00"},
    { "materialCheck_00_btnDecide.brlan",        "G_btn_01"},
    {   "materialCheck_00_btnLoop.brlan",    "G_btnLoop_00"},
    {   "materialCheck_00_btnLoop.brlan",    "G_btnLoop_01"},
    {      "materialCheck_00_loop.brlan",       "G_loop_00"},
    {      "materialCheck_00_keta.brlan",       "G_keta_00"},
    {  "materialCheck_00_itemFlag.brlan",   "G_itemFlag_00"},
    {  "materialCheck_00_itemFlag.brlan",   "G_itemFlag_01"},
    {  "materialCheck_00_itemFlag.brlan",   "G_itemFlag_02"},
    {  "materialCheck_00_itemFlag.brlan",   "G_itemFlag_03"},
    {"materialCheck_00_itemNumber.brlan", "G_itemNumber_00"},
    { "materialCheck_00_btnCancel.brlan",  "G_btnCancel_00"},
    {"materialCheck_00_shopChange.brlan",     "G_change_00"},
};

static const d2d::LytBrlanMapping brlanMapShopInfo[] = {
    { "shopInfo_00_in.brlan", "G_inOut_00"},
    {"shopInfo_00_out.brlan", "G_inOut_00"},
};

static const char *sItemSelectBoundings[] = {
    "B_item_00", "B_item_01", "B_item_02", "B_item_03", "B_item_04", "B_item_05",    "B_item_06",
    "B_item_07", "B_item_08", "B_item_09", "B_item_10", "B_item_11", "B_choices_00",
};

static const char *sMaterialCheckChoiceBoundings[] = {
    "B_choices_00",
    "B_choices_01",
};

#define SHOP_MATERIAL_CHECK_CHOICE_NUM_BOUNDINGS 2

#define SHOP_LYT_ITEM_SELECT 0
#define SHOP_LYT_MATERIAL_CHECK 1
#define SHOP_LYT_SHOP_INFO 2

#define SHOP_NUM_LYTS 3

#define SHOP_ITEM_SELECT_NUM_ANIMS 9
#define SHOP_MATERIAL_NUM_ANIMS 18
#define SHOP_SHOP_INFO_NUM_ANIMS 2

#define SHOP_ITEM_SELECT_BOUNDING_ITEM_OFFSET 0
#define SHOP_ITEM_SELECT_BOUNDING_CHOICES 12

#define SHOP_ITEM_SELECT_NUM_BOUNDINGS 13

void dLytShopMain_c::initializeState_ModeNone() {}
void dLytShopMain_c::executeState_ModeNone() {
    mIsIdle = true;
}
void dLytShopMain_c::finalizeState_ModeNone() {}

void dLytShopMain_c::initializeState_ModeSelectIn() {}
void dLytShopMain_c::executeState_ModeSelectIn() {
    if (mAnmItemSelect[0].isEndReached() && mAnmShopInfo[0].isEndReached()) {
        mAnmItemSelect[0].setAnimEnable(false);
        mAnmShopInfo[0].setAnimEnable(false);

        for (int i = 0; i < NUM_ITEMS; i++) {
            mpItemBoundings[i + SHOP_ITEM_SELECT_BOUNDING_ITEM_OFFSET]->SetVisible(false);
        }
        dCsBase_c::GetInstance()->setVisible(true);
        mPhase = PHASE_SELECT_ITEM;
        mIsIdle = true;
    }
}
void dLytShopMain_c::finalizeState_ModeSelectIn() {}

void dLytShopMain_c::initializeState_ModeSelectCheck() {
    mCurrentNavTarget = -12;
    field_0x116DD = false;
    field_0x116DE = false;
    field_0x11700 = false;
}
void dLytShopMain_c::executeState_ModeSelectCheck() {
    if (dPad::getDownTrigB()) {
        field_0x116DD = true;
        fn_802A5CA0(false);
        mIsIdle = true;
    } else if (dPad::getDownTrigA() && mCurrentNavTarget >= 0 &&
               (mCurrentNavTarget < mItemCount || mCurrentNavTarget == 12)) {
        if (mCurrentNavTarget == 12) {
            field_0x116DD = true;
        }
        mIsIdle = true;
    } else {
        handleNavigation();
    }

    if (mIsIdle) {
        dPadNav::setNavEnabled(false, false);
    }
}
void dLytShopMain_c::finalizeState_ModeSelectCheck() {}

void dLytShopMain_c::initializeState_ModeSelectOut() {}
void dLytShopMain_c::executeState_ModeSelectOut() {
    if (field_0x116DD) {
        if (mAnmItemSelect[4].isEndReached()) {
            mUpgradeIdx = 0;
            mItemCount = 0;
            if (mCurrentNavTarget == 12) {
                mAnmItemSelect[1].setAnimEnable(false);
            }
            mCurrentNavTarget = -13;
            mIsIdle = true;
        }
    } else {
        if (mBWheelOrPouchItemIcons[mCurrentNavTarget + 12].isDoneDeciding()) {
            mUpgradeIdx = mItemUpgradeIdxes[mCurrentNavTarget];
            mIsIdle = true;
        }
    }
}
void dLytShopMain_c::finalizeState_ModeSelectOut() {}

void dLytShopMain_c::initializeState_ModeOutIn() {}
void dLytShopMain_c::executeState_ModeOutIn() {
    if (mStateStep == 0) {
        bool b = false;
        if (field_0x116DD) {
            if (mAnmItemSelect[4].isEndReached()) {
                mAnmItemSelect[4].setAnimEnable(false);
                b = true;
            }
        } else {
            s32 offset = mItemKinds[mCurrentNavTarget] == dLytCommonIconItem_c::B_WHEEL ? 0 : 12;
            if (mBWheelOrPouchItemIcons[mCurrentNavTarget + offset].isDoneDeciding()) {
                b = true;
                mUpgradeIdx = mItemUpgradeIdxes[mCurrentNavTarget];
            }
        }

        if (b) {
            mAnmItemSelect[5].setAnimEnable(true);
            mAnmItemSelect[5].setFrame(0.0f);
            mAnmItemSelect[1].setAnimEnable(true);
            mAnmItemSelect[1].setFrame(0.0f);
            mAnmItemSelect[3].setAnimEnable(true);
            mAnmItemSelect[3].setFrame(0.0f);
            mLyts[SHOP_LYT_ITEM_SELECT].calc();
            mAnmItemSelect[1].setAnimEnable(false);
            mAnmItemSelect[3].setAnimEnable(false);

            mAnmMaterialCheck[0].setAnimEnable(true);
            mAnmMaterialCheck[0].setFrame(0.0f);
            mAnmMaterialCheck[16].setAnimEnable(true);
            mAnmMaterialCheck[16].setFrame(1.0f);

            updateShopStuffMaybe();

            f32 f;
            if (mService != SERVICE_POTION_INFUSE) {
                f = 0.0f;
                if (mService == SERVICE_GEAR_UPGRADE) {
                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P2_APPEAR);
                }
            } else {
                f = 1.0f;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P2_APPEAR);
            }

            mAnmMaterialCheck[17].setAnimEnable(true);
            mAnmMaterialCheck[17].setFrame(f);
            mLyts[SHOP_LYT_MATERIAL_CHECK].calc();
            mAnmMaterialCheck[17].setAnimEnable(false);
            mStateStep = 1;
        }
    } else {
        if (mAnmItemSelect[5].isEndReached() && mAnmMaterialCheck[0].isEndReached()) {
            mAnmItemSelect[5].setAnimEnable(false);
            mAnmMaterialCheck[0].setAnimEnable(false);

            s32 offset = mItemKinds[mCurrentNavTarget] == dLytCommonIconItem_c::B_WHEEL ? 0 : 12;
            mBWheelOrPouchItemIcons[mCurrentNavTarget + offset].setOff();

            mPhase = PHASE_MATERIAL_CHECK;
            mCurrentNavTarget = mCurrentNavTarget - 12;
            dPadNav::setNavEnabled(true, false);
            mIsIdle = true;
        }
    }
}
void dLytShopMain_c::finalizeState_ModeOutIn() {}

void dLytShopMain_c::initializeState_ModeMaterialCheck() {
    mCurrentNavTarget = -2;
    field_0x116DD = false;
    field_0x116DE = false;
}
void dLytShopMain_c::executeState_ModeMaterialCheck() {
    if (dPad::getDownTrigB()) {
        if (field_0x116DE) {
            mAnmMaterialCheck[3].setBackwardsOnce();
        }
        field_0x116DD = true;
        fn_802A5CA0(false);
        mIsIdle = true;
    } else if (dPad::getDownTrigA() && mCurrentNavTarget >= 0) {
        if (mCurrentNavTarget != 0) {
            dCsBase_c::GetInstance()->setVisible(false);
        }
        mIsIdle = true;
    } else {
        handleNavigation();
    }

    if (mIsIdle) {
        dPadNav::setNavEnabled(false, false);
    }
}
void dLytShopMain_c::finalizeState_ModeMaterialCheck() {}

void dLytShopMain_c::initializeState_ModeOut() {}
void dLytShopMain_c::executeState_ModeOut() {
    if (mStateStep == 0) {
        if (mAnmMaterialCheck[6].isEndReached()) {
            mAnmMaterialCheck[6].setAnimEnable(false);
            mAnmMaterialCheck[1].setAnimEnable(true);
            mAnmMaterialCheck[1].setFrame(0.0f);
            mAnmMaterialCheck[3].setAnimEnable(true);
            mAnmMaterialCheck[3].setFrame(0.0f);
            mLyts[SHOP_LYT_MATERIAL_CHECK].calc();
            mAnmMaterialCheck[3].setAnimEnable(false);

            mAnmShopInfo[1].setAnimEnable(true);
            mAnmShopInfo[1].setFrame(0.0f);
            mLyts[SHOP_LYT_SHOP_INFO].calc();
            dCsBase_c::GetInstance()->setVisible(false);
            mStateStep = 1;
        }
    } else {
        if (mAnmMaterialCheck[1].isEndReached()) {
            mAnmMaterialCheck[1].setAnimEnable(false);
            mAnmShopInfo[1].setAnimEnable(false);
            fn_802A5BD0();
            mIsIdle = true;
        }
    }
}
void dLytShopMain_c::finalizeState_ModeOut() {}

void dLytShopMain_c::initializeState_ModeInOut() {}
void dLytShopMain_c::executeState_ModeInOut() {
    if (mStateStep == 0) {
        if (mAnmMaterialCheck[5].isEndReached()) {
            mAnmMaterialCheck[5].setAnimEnable(false);

            mAnmItemSelect[0].setAnimEnable(true);
            mAnmItemSelect[0].setFrame(0.0f);
            mLyts[SHOP_LYT_ITEM_SELECT].calc();

            mAnmMaterialCheck[1].setAnimEnable(true);
            mAnmMaterialCheck[1].setFrame(0.0f);
            mAnmMaterialCheck[2].setAnimEnable(true);
            mAnmMaterialCheck[2].setFrame(0.0f);
            mAnmMaterialCheck[3].setAnimEnable(true);
            mAnmMaterialCheck[3].setFrame(0.0f);
            mAnmMaterialCheck[7].setAnimEnable(true);
            mAnmMaterialCheck[7].setFrame(0.0f);
            mAnmMaterialCheck[8].setAnimEnable(true);
            mAnmMaterialCheck[8].setFrame(0.0f);
            mLyts[SHOP_LYT_MATERIAL_CHECK].calc();
            mAnmMaterialCheck[2].setAnimEnable(false);
            mAnmMaterialCheck[3].setAnimEnable(false);
            mAnmMaterialCheck[7].setAnimEnable(false);
            mAnmMaterialCheck[8].setAnimEnable(false);

            if (mService == SERVICE_POTION_INFUSE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P1_APPEAR);
            } else if (mService == SERVICE_GEAR_UPGRADE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P1_APPEAR);
            }

            fn_802A5CA0(true);
            mStateStep = 1;
        }
    } else {
        if (mAnmItemSelect[0].isEndReached() && mAnmMaterialCheck[1].isEndReached()) {
            mAnmItemSelect[0].setAnimEnable(false);
            mAnmMaterialCheck[1].setAnimEnable(false);
            fn_802A5BD0();
            mPhase = PHASE_SELECT_ITEM;
            dPadNav::setNavEnabled(true, false);
            mIsIdle = true;
        }
    }
}
void dLytShopMain_c::finalizeState_ModeInOut() {}

dLytShopMain_c::dLytShopMain_c() : mStateMgr(*this) {}

bool dLytShopMain_c::build(d2d::ResAccIf_c *resAcc) {
    for (int i = 0; i < SHOP_NUM_LYTS; i++) {
        // TODO instruction swap, maybe will resolve itself?
        mLyts[i].setResAcc(resAcc);
        mLyts[i].build(sLytNames[i], nullptr);
    }

    for (int i = 0; i < SHOP_ITEM_SELECT_NUM_ANIMS; i++) {
        mAnmItemSelect[i].init(
            brlanMapItemSelect[i].mFile, resAcc, mLyts[SHOP_LYT_ITEM_SELECT].getLayout(), brlanMapItemSelect[i].mName
        );
        mAnmItemSelect[i].bind(false);
        mAnmItemSelect[i].setAnimEnable(false);
    }

    for (int i = 0; i < SHOP_MATERIAL_NUM_ANIMS; i++) {
        mAnmMaterialCheck[i].init(
            brlanMapMaterialCheck[i].mFile, resAcc, mLyts[SHOP_LYT_MATERIAL_CHECK].getLayout(),
            brlanMapMaterialCheck[i].mName
        );
        mAnmMaterialCheck[i].bind(false);
        mAnmMaterialCheck[i].setAnimEnable(false);
    }

    for (int i = 0; i < SHOP_SHOP_INFO_NUM_ANIMS; i++) {
        mAnmShopInfo[i].init(
            brlanMapShopInfo[i].mFile, resAcc, mLyts[SHOP_LYT_SHOP_INFO].getLayout(), brlanMapShopInfo[i].mName
        );
        mAnmShopInfo[i].bind(false);
        mAnmShopInfo[i].setAnimEnable(false);
    }

    for (int i = 0; i < SHOP_ITEM_SELECT_NUM_BOUNDINGS; i++) {
        mpItemBoundings[i] = mLyts[SHOP_LYT_ITEM_SELECT].findBounding(sItemSelectBoundings[i]);
    }

    for (int i = 0; i < SHOP_MATERIAL_CHECK_CHOICE_NUM_BOUNDINGS; i++) {
        mpMaterialCheckChoiceBoundings[i] =
            mLyts[SHOP_LYT_MATERIAL_CHECK].findBounding(sMaterialCheckChoiceBoundings[i]);
    }

    for (int i = 0; i < NUM_ITEMS; i++) {
        mItemPouchSlots[i] = -1;
    }

    buildSubpanes();

    mAnmItemSelect[0].setAnimEnable(true);
    mAnmItemSelect[0].setFrame(mAnmItemSelect[0].getLastFrame());
    mAnmItemSelect[6].setAnimEnable(true);
    mAnmItemSelect[6].setFrame(0.0f);
    mLyts[SHOP_LYT_ITEM_SELECT].calc();
    mAnmItemSelect[0].setAnimEnable(false);

    mAnmItemSelect[3].setForwardLoop();
    mAnmMaterialCheck[7].setForwardLoop();
    mAnmMaterialCheck[8].setForwardLoop();

    mAnmMaterialCheck[0].setAnimEnable(true);
    mAnmMaterialCheck[0].setFrame(0.0f);
    mAnmMaterialCheck[7].setAnimEnable(true);
    mAnmMaterialCheck[7].setFrame(0.0f);
    mAnmMaterialCheck[8].setAnimEnable(true);
    mAnmMaterialCheck[8].setFrame(0.0f);
    mAnmMaterialCheck[9].setAnimEnable(true);
    mAnmMaterialCheck[9].setFrame(0.0f);
    mLyts[SHOP_LYT_MATERIAL_CHECK].calc();
    mAnmMaterialCheck[0].setAnimEnable(false);

    mAnmShopInfo[0].setAnimEnable(true);
    mAnmShopInfo[0].setFrame(0.0f);
    mLyts[SHOP_LYT_SHOP_INFO].calc();
    mAnmShopInfo[0].setAnimEnable(false);

    mCsHitChecks[0].init(mLyts[SHOP_LYT_ITEM_SELECT].getLayout()->GetRootPane(), 1, 0, 0);
    mCsHitChecks[1].init(mLyts[SHOP_LYT_MATERIAL_CHECK].getLayout()->GetRootPane(), 1, 0, 0);

    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitChecks[0]);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitChecks[1]);

    dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[0]);

    setModeNone();
    field_0x116DD = false;
    field_0x116DE = false;
    mUpgradeLoaded = false;
    mPrevPointerVisible = true;

    fn_802A5CA0(true);

    mPhase = PHASE_SELECT_ITEM;
    mCurrentNavTarget = -12;
    mStateStep = 0;
    mUpgradeIdx = 0;
    mItemCount = 0;
    mUpgradeCostCount = 0;
    field_0x11700 = 0;

    return true;
}

bool dLytShopMain_c::execute() {
    mCsHitChecks[mPhase].resetCachedHitboxes();
    mCsHitChecks[mPhase].execute();
    mStateMgr.executeState();

    for (int i = 0; i < mItemCount; i++) {
        if (mItemKinds[i] == dLytCommonIconItem_c::B_WHEEL) {
            mBWheelOrPouchNodes[i].mpLytPane->execute();
        } else {
            mBWheelOrPouchNodes[i + 12].mpLytPane->execute();
        }
    }

    if (mUpgradeLoaded) {
        for (int i = 0; i < mUpgradeCostCount; i++) {
            if (i < 2) {
                if (field_0x11770[i] == 0) {
                    mUpgradeNodes[i].mpLytPane->execute();
                } else {
                    mUpgradeNodes[i + 6].mpLytPane->execute();
                }
            } else if (field_0x11770[i] == 0) {
                mUpgradeNodes[i + 6].mpLytPane->execute();
            } else {
                mUpgradeNodes[i].mpLytPane->execute();
            }
        }
    }

    for (int i = 0; i < 7; i++) {
        if (mAnmItemSelect[i].isEnabled()) {
            mAnmItemSelect[i].play();
            if (i == 1) {
                f32 frame;
                if (!field_0x116E1 || mAnmItemSelect[i].getFrame() == mAnmItemSelect[i].getLastFrame()) {
                    frame = 0.0f;
                } else {
                    frame = 1.0f;
                }
                mAnmItemSelect[7].setAnimEnable(true);
                mAnmItemSelect[7].setFrame(frame);
                mLyts[SHOP_LYT_ITEM_SELECT].calc();
                mAnmItemSelect[7].setAnimEnable(false);
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        if (mAnmMaterialCheck[i].isEnabled()) {
            mAnmMaterialCheck[i].play();
            if (i == 2) {
                f32 frame;
                if (!field_0x116E1 || mAnmMaterialCheck[i].getFrame() == mAnmMaterialCheck[i].getLastFrame()) {
                    frame = 0.0f;
                } else {
                    frame = 1.0f;
                }
                mAnmMaterialCheck[16].setAnimEnable(true);
                mAnmMaterialCheck[16].setFrame(frame);
                mLyts[SHOP_LYT_MATERIAL_CHECK].calc();
                mAnmMaterialCheck[16].setAnimEnable(false);
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        if (mAnmShopInfo[i].isEnabled()) {
            mAnmShopInfo[i].play();
        }
    }

    for (int i = 0; i < SHOP_NUM_LYTS; i++) {
        mLyts[i].calc();
    }

    mPrevPointerVisible = dPadNav::isPointerVisible();

    return true;
}

bool dLytShopMain_c::draw() {
    for (int i = 0; i < SHOP_NUM_LYTS; i++) {
        mLyts[i].addToDrawList();
    }
    return true;
}

bool dLytShopMain_c::remove() {
    dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitChecks[0]);
    dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitChecks[1]);

    // Okay this one does need temps to avoid regswaps, the others probably should use temps too for consistency
    d2d::SubPaneList::Iterator it = mItemList.GetBeginIter();
    d2d::SubPaneList::Iterator end = mItemList.GetEndIter();
    for (; it != end; ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < 24; i++) {
        mBWheelOrPouchNodes[i].mpLytPane->remove();
    }

    it = mUpgradeList.GetBeginIter();
    end = mUpgradeList.GetEndIter();
    for (; it != end; ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }

    for (int i = 0; i < 12; i++) {
        mUpgradeNodes[i].mpLytPane->remove();
    }

    mUpgradeLoaded = false;

    for (int i = 0; i < SHOP_NUM_LYTS; i++) {
        mLyts[i].unbindAnims();
    }

    for (int i = 0; i < SHOP_ITEM_SELECT_NUM_ANIMS; i++) {
        mAnmItemSelect[i].remove();
    }

    for (int i = 0; i < SHOP_MATERIAL_NUM_ANIMS; i++) {
        mAnmMaterialCheck[i].remove();
    }

    for (int i = 0; i < SHOP_SHOP_INFO_NUM_ANIMS; i++) {
        mAnmShopInfo[i].remove();
    }

    return true;
}

void dLytShopMain_c::setModeNone() {
    mStateMgr.changeState(StateID_ModeNone);
    mIsIdle = false;
}

void dLytShopMain_c::setModeSelectIn() {
    // TODO: wat
    if (dCsBase_c::GetInstance()->isVisible()) {
        dCsBase_c::GetInstance()->setVisible(true);
    }

    f32 frame = mService != SERVICE_POTION_INFUSE ? 0.0f : 1.0f;

    mAnmItemSelect[8].setAnimEnable(true);
    mAnmItemSelect[8].setFrame(frame);
    mAnmItemSelect[0].setAnimEnable(true);
    mAnmItemSelect[7].setAnimEnable(true);
    mAnmItemSelect[7].setFrame(1.0f);
    mAnmItemSelect[3].setAnimEnable(true);
    mAnmItemSelect[3].setFrame(0.0f);
    mLyts[SHOP_LYT_ITEM_SELECT].calc();
    mAnmItemSelect[8].setAnimEnable(false);

    mAnmShopInfo[0].setAnimEnable(true);
    mAnmShopInfo[0].setFrame(0.0f);
    mLyts[SHOP_LYT_SHOP_INFO].calc();

    mStateMgr.changeState(StateID_ModeSelectIn);
    mIsIdle = false;
}

void dLytShopMain_c::setModeSelectCheck() {
    mStateMgr.changeState(StateID_ModeSelectCheck);
    mIsIdle = false;
}

void dLytShopMain_c::setModeSelectOut() {
    mStateStep = 0;
    if (field_0x116DD) {
        mAnmItemSelect[4].setAnimEnable(true);
        mAnmItemSelect[4].setFrame(0.0f);
        mLyts[SHOP_LYT_ITEM_SELECT].calc();
    } else {
        mBWheelOrPouchItemIcons[mCurrentNavTarget + 12].startConfirm();
        field_0x11700 = mCurrentNavTarget;
    }
    dCsBase_c::GetInstance()->setVisible(false);
    dPadNav::setNavEnabled(false, false);
    mStateMgr.changeState(StateID_ModeSelectOut);
    mIsIdle = false;
}

void dLytShopMain_c::setModeOutIn() {
    if (field_0x116DD) {
        mStateStep = 0;
        mAnmItemSelect[4].setAnimEnable(true);
        mAnmItemSelect[4].setFrame(0.0f);
        mLyts[SHOP_LYT_ITEM_SELECT].calc();
    } else {
        s32 offset = mItemKinds[mCurrentNavTarget] == dLytCommonIconItem_c::B_WHEEL ? 0 : 12;
        mStateStep = 0;
        mBWheelOrPouchItemIcons[mCurrentNavTarget + offset].startConfirm();
        if (mCurrentNavTarget < 12) {
            field_0x11700 = mCurrentNavTarget;
        } else {
            field_0x11700 = mCurrentNavTarget - 12;
        }
    }
    mStateMgr.changeState(StateID_ModeSelectOut);
    mIsIdle = false;
}

void dLytShopMain_c::setModeMaterialCheck() {
    dMessage_c::getInstance()->setField_0x2FC(-1);
    mStateMgr.changeState(StateID_ModeMaterialCheck);
    mIsIdle = false;
}

void dLytShopMain_c::setModeOut() {
    mStateStep = 0;
    mAnmMaterialCheck[6].setAnimEnable(true);
    mAnmMaterialCheck[6].setFrame(0.0f);
    mLyts[SHOP_LYT_MATERIAL_CHECK].calc();
    mStateMgr.changeState(StateID_ModeMaterialCheck);
    mIsIdle = false;
}

void dLytShopMain_c::setModeInOut() {
    mStateStep = 0;
    mAnmMaterialCheck[5].setAnimEnable(true);
    mAnmMaterialCheck[5].setFrame(0.0f);
    mLyts[SHOP_LYT_MATERIAL_CHECK].calc();
    dMessage_c::getInstance()->setField_0x2FC(-2);
    dLytMeter_c::setRupyField_0x8AC(0);
    mStateMgr.changeState(StateID_ModeInOut);
    mIsIdle = false;
}

static const char *sGroupName = "G_ref_00";

void dLytShopMain_c::buildSubpanes() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("DoButton");
    d2d::ResAccIf_c resAcc;
    resAcc.attach(data, "");

    for (int i = 0; i < (int)ARRAY_LENGTH(mBWheelOrPouchItemIcons); i++) {
        mBWheelOrPouchNodes[i].mpLytPane = &mBWheelOrPouchItemIcons[i];
        if (i < 12) {
            mBWheelOrPouchItemIcons[i].build(&resAcc, dLytCommonIconItem_c::B_WHEEL);
        } else {
            mBWheelOrPouchItemIcons[i].build(&resAcc, dLytCommonIconItem_c::POUCH);
        }
        mItemList.PushBack(&mBWheelOrPouchNodes[i]);
    }

    nw4r::lyt::GroupContainer *container = mLyts[SHOP_LYT_ITEM_SELECT].getLayout()->GetGroupContainer();
    if (container != nullptr) {
        nw4r::lyt::Group *g = container->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mItemList);
        }
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(mBWheelOrPouchNodes); i++) {
        mBWheelOrPouchNodes[i].mpPane->SetVisible(false);
    }

    buildMaterialCheckIcons(&resAcc, dLytCommonIconItem_c::B_WHEEL);
    buildMaterialCheckIcons(&resAcc, dLytCommonIconItem_c::POUCH);

    container = mLyts[SHOP_LYT_MATERIAL_CHECK].getLayout()->GetGroupContainer();
    if (container != nullptr) {
        nw4r::lyt::Group *g = container->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mUpgradeList);
        }
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(mUpgradeNodes); i++) {
        mUpgradeNodes[i].mpPane->SetVisible(false);
    }
}

void dLytShopMain_c::prepareRemodelStoreStuff(s32 service) {
    mService = service;
    mItemCount = 0;

    s32 textVariant;
    dTextBox_c *box1, *box2;
    box1 = mLyts[SHOP_LYT_MATERIAL_CHECK].getTextBox("T_text_03");
    box2 = mLyts[SHOP_LYT_MATERIAL_CHECK].getTextBox("T_textS_03");

    s32 offset1;
    s32 offset2;
    if (mService != SERVICE_POTION_INFUSE) {
        // "Upgrade"
        offset1 = 0;
        offset2 = 28;
        textVariant = 0;
    } else {
        // "Infuse"
        offset1 = 28;
        offset2 = 68;
        textVariant = 1;
    }
    mLyts[SHOP_LYT_MATERIAL_CHECK].loadTextVariant(box1, textVariant);
    mLyts[SHOP_LYT_MATERIAL_CHECK].loadTextVariant(box2, textVariant);

    for (int i = 0; i < (int)ARRAY_LENGTH(mItemUpgradeIdxes); i++) {
        mItemUpgradeIdxes[i] = -1;
    }

    s32 idx = 0;
    if (mService == SERVICE_GEAR_UPGRADE && offset1 < 10) {
        // Upgrade-able B-Wheel items (4)
        static const s32 skips[] = {2, 2, 3, 3};
        for (int i = 0; i < 4; i++) {
            bool show;
            s32 itemTier = getItemTier(i);
            if (itemTier == 0) {
                show = false;
            } else {
                idx = offset1 + itemTier - 1;
                show = sItemData[idx].canBeUpgraded;
                if (show) {
                    // TODO change arg type to u16
                    show = dAcItem_c::checkFlag((u16)sItemData[idx].itemId);
                }
            }
            if (show) {
                addItemToDisplayed(idx, dLytCommonIconItem_c::B_WHEEL, -1);
            }
            offset1 += skips[i];
        }
    }

    if (mService == SERVICE_SHIELD_REPAIR) {
        for (int i = 0; i < 8; i++) {
            s32 pouchItemId = getPouchItemIdForIndex(i, false);
            s32 val = getPouchShieldDurability2(i, false);
            if (val >= 0 && getEvenMoreShieldStuff(i, false) > val) {
                for (int i = 10; i < 19; i++) {
                    if (sItemData[i].itemId == pouchItemId) {
                        idx = i;
                        break;
                    }
                }
                addItemToDisplayed(idx, dLytCommonIconItem_c::POUCH, i);
            }
        }
    } else {
        // infusable or upgradeable pouch items
        for (int i = 0; i < 8; i++) {
            s32 pouchItemId = getPouchItemIdForIndex(i, false);
            bool show = false;
            for (int j = offset1; j < offset2; j++) {
                if (sItemData[j].itemId == pouchItemId) {
                    show = sItemData[j].canBeUpgraded;
                    idx = j;
                    break;
                }
            }

            if (show) {
                addItemToDisplayed(idx, dLytCommonIconItem_c::POUCH, i);
            }
        }
    }

    for (int i = 0; i < 12; i++) {
        if (i < mItemCount) {
            if (mItemKinds[i] == dLytCommonIconItem_c::B_WHEEL) {
                mBWheelOrPouchNodes[i].mpPane->SetVisible(true);
                mBWheelOrPouchNodes[i + 12].mpPane->SetVisible(false);
                mBWheelOrPouchItemIcons[i + 12].setVisible(false);
            } else {
                mBWheelOrPouchNodes[i].mpPane->SetVisible(false);
                mBWheelOrPouchItemIcons[i].setVisible(false);
                mBWheelOrPouchNodes[i + 12].mpPane->SetVisible(true);
            }
        } else {
            mBWheelOrPouchNodes[i].mpPane->SetVisible(false);
            mBWheelOrPouchItemIcons[i].setVisible(false);
            mBWheelOrPouchNodes[i + 12].mpPane->SetVisible(false);
            mBWheelOrPouchItemIcons[i + 12].setVisible(false);
        }
    }
}

void dLytShopMain_c::addItemToDisplayed(s32 itemIdx, s32 itemKind, s32 pouchSlot) {
    mItemKinds[mItemCount] = itemKind;
    s32 iconIdx;
    if (itemKind == dLytCommonIconItem_c::B_WHEEL) {
        iconIdx = mItemCount;
    } else {
        iconIdx = mItemCount + 12;
    }

    mBWheelOrPouchItemIcons[iconIdx].setUnk(true);
    mBWheelOrPouchItemIcons[iconIdx].setHasNumber(false);
    mBWheelOrPouchItemIcons[iconIdx].setItem(sLytIconMapping[itemIdx]);

    s32 id = sItemData[itemIdx].itemId;
    bool found = false;
    int i = 0;
    if (mService != SERVICE_POTION_INFUSE) {
        for (; i < (int)ARRAY_LENGTH(sGearUpgrades); i++) {
            if (sGearUpgrades[i].oldItemId == id) {
                found = true;
                break;
            }
        }
    } else {
        for (; i < (int)ARRAY_LENGTH(sPotionUpgrades); i++) {
            if (sPotionUpgrades[i].oldItemId == id) {
                found = true;
                break;
            }
        }
    }

    if (found) {
        mItemUpgradeIdxes[mItemCount] = i;
    }

    f32 durability = getPouchShieldDurability(pouchSlot, false);
    if (durability >= 0.0f) {
        mBWheelOrPouchItemIcons[iconIdx].setShieldDurability(durability);
    }
    mItemPouchSlots[mItemCount] = pouchSlot;
    mItemCount++;
}

void dLytShopMain_c::buildMaterialCheckIcons(d2d::ResAccIf_c *resAcc, u8 variant) {
    u8 costVariant;
    s32 upgradeItemIconOffset, upgradeMatIconOffset, nodeOffset;
    if (variant == dLytCommonIconItem_c::B_WHEEL) {
        costVariant = dLytCommonIconMaterial_c::TREASURE;
        upgradeItemIconOffset = 0;
        upgradeMatIconOffset = 0;
        nodeOffset = 0;
    } else {
        costVariant = dLytCommonIconMaterial_c::BUG;
        upgradeItemIconOffset = 2;
        upgradeMatIconOffset = 4;
        nodeOffset = 6;
    }

    s32 run = nodeOffset;

    // base item
    mUpgradeNodes[run].mpLytPane = &mUpgradePreviewIcons[upgradeItemIconOffset];
    mUpgradePreviewIcons[upgradeItemIconOffset].build(resAcc, variant);
    mUpgradeList.PushBack(&mUpgradeNodes[run]);

    upgradeItemIconOffset++;
    run++;
    // new item
    mUpgradeNodes[run].mpLytPane = &mUpgradePreviewIcons[upgradeItemIconOffset];
    mUpgradePreviewIcons[upgradeItemIconOffset].build(resAcc, variant);
    mUpgradeList.PushBack(&mUpgradeNodes[run]);

    // upgrade costs
    for (int i = 0; i < 4; i++) {
        mUpgradeNodes[nodeOffset + i + 2].mpLytPane = &mUpgradeCostIcons[upgradeMatIconOffset + i];
        mUpgradeCostIcons[upgradeMatIconOffset + i].build(resAcc, costVariant);
        mUpgradeList.PushBack(&mUpgradeNodes[nodeOffset + i + 2]);
    }
}

s32 dLytShopMain_c::getItemTier(s32 i) {
    if (i == 0) {
        return dAcPy_c::GetLink()->getCurrentBugNetType();
    } else if (i == 1) {
        return dAcPy_c::GetLink()->getCurrentSlingshotType();
    } else if (i == 2) {
        s32 ty = dAcPy_c::GetLink()->getCurrentBeetleType() - 1;
        if (ty < 0) {
            ty = 0;
        }
        return ty;
    } else {
        return dAcPy_c::GetLink()->getCurrentBowType();
    }
}

void dLytShopMain_c::handleNavigation() {
    dCursorHitCheck_c *d = dCsBase_c::GetInstance()->getHitCheck();
    s32 nextTarget;
    bool selectIcon = false;
    s32 i1, i2;
    if (mPhase == PHASE_SELECT_ITEM) {
        i1 = 13;
        i2 = 1;
        for (int i = 0; i < NUM_ITEMS; i++) {
            mpItemBoundings[i + SHOP_ITEM_SELECT_BOUNDING_ITEM_OFFSET]->SetVisible(false);
        }
    } else {
        i1 = 2;
        i2 = 2;
    }

    bool pointerVisible = dPadNav::isPointerVisible();
    if (mPhase == PHASE_SELECT_ITEM) {
        if (pointerVisible) {
            for (nextTarget = 0; nextTarget < mItemCount; nextTarget++) {
                s32 iconIdx;
                if (mItemKinds[nextTarget] == dLytCommonIconItem_c::B_WHEEL) {
                    iconIdx = nextTarget;
                } else {
                    iconIdx = nextTarget + 12;
                }
                if (mBWheelOrPouchItemIcons[iconIdx].isCursorOver()) {
                    selectIcon = true;
                    mUpgradeIdx = mItemUpgradeIdxes[nextTarget];
                    break;
                }
            }
        } else {
            if (dPadNav::getFSStickNavDirection() != dPadNav::FS_STICK_NONE) {
                nextTarget = mCurrentNavTarget;
                if (nextTarget < 0 || mPrevPointerVisible) {
                    if (nextTarget == -13) {
                        nextTarget = 12;
                    } else if (nextTarget < 0) {
                        nextTarget += 12;
                    }
                    dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[nextTarget]);
                    selectIcon = true;
                } else {
                    switch (checkNav()) {
                        case dPadNav::FS_STICK_UP:
                            selectIcon = true;
                            nextTarget = mCurrentNavTarget - 6;
                            break;
                        case dPadNav::FS_STICK_UP_RIGHT:
                            selectIcon = true;
                            nextTarget = mCurrentNavTarget - 5;
                            break;
                        case dPadNav::FS_STICK_RIGHT:
                            selectIcon = true;
                            nextTarget = mCurrentNavTarget + 1;
                            break;
                        case dPadNav::FS_STICK_DOWN_RIGHT:
                            selectIcon = true;
                            nextTarget = mCurrentNavTarget + 7;
                            break;
                        case dPadNav::FS_STICK_DOWN:
                            selectIcon = true;
                            nextTarget = mCurrentNavTarget + 6;
                            break;
                        case dPadNav::FS_STICK_DOWN_LEFT:
                            selectIcon = true;
                            nextTarget = mCurrentNavTarget + 5;
                            break;
                        case dPadNav::FS_STICK_LEFT:
                            selectIcon = true;
                            nextTarget = mCurrentNavTarget - 1;
                            break;
                        case dPadNav::FS_STICK_UP_LEFT:
                            selectIcon = true;
                            nextTarget = mCurrentNavTarget - 7;
                            break;
                        default:
                            selectIcon = true;
                            nextTarget = mCurrentNavTarget;
                            break;
                    }
                    if (nextTarget >= 12) {
                        nextTarget = 12;
                    }
                    dCsBase_c::GetInstance()->setCursorStickTargetPane(mpItemBoundings[nextTarget]);
                }
            } else if (mCurrentNavTarget >= 0) {
                selectIcon = true;
                nextTarget = mCurrentNavTarget;
            }
        }
    } else {
        if (!pointerVisible) {
            if (mPrevPointerVisible) {
                // Sorry what? Why is dPadNav not good enough here?
                if (dPad::ex_c::getInstance()->getFSStickTrig(0x1) || dPad::ex_c::getInstance()->getFSStickTrig(0x10)) {
                    // up/down
                    dPadNav::setNavEnabled(true, false);
                    if (mCurrentNavTarget >= 0) {
                        nextTarget = mCurrentNavTarget;
                        selectIcon = true;
                    }
                } else {
                    if (dPad::ex_c::getInstance()->getFSStickTrig(0x80) ||
                        dPad::ex_c::getInstance()->getFSStickTrig(0x40) ||
                        dPad::ex_c::getInstance()->getFSStickTrig(0x20)) {
                        // right
                        nextTarget = 1;
                    } else {
                        // left
                        nextTarget = 0;
                    }
                    dCsBase_c::GetInstance()->setCursorStickTargetPane(mpMaterialCheckChoiceBoundings[nextTarget]);
                    selectIcon = true;
                }
            } else {
                switch (dPadNav::getFSStickNavDirection()) {
                    case 2:
                    case 3:
                    case 4:
                        nextTarget = 1;
                        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpMaterialCheckChoiceBoundings[1]);
                        selectIcon = true;
                        break;
                    case 6:
                    case 7:
                    case 8:
                        nextTarget = 0;
                        dCsBase_c::GetInstance()->setCursorStickTargetPane(mpMaterialCheckChoiceBoundings[0]);
                        selectIcon = true;
                        break;
                    default: nextTarget = mCurrentNavTarget; selectIcon = true;
                }
            }
        }
    }

    if (!selectIcon && pointerVisible) {
        if (d != nullptr && d->getType() == 'lyt ') {
            for (nextTarget = 0; nextTarget < i1; nextTarget++) {
                nw4r::lyt::Bounding *b;
                if (mPhase == 0) {
                    b = mpItemBoundings[12];
                    nextTarget = 12;
                } else {
                    b = mpMaterialCheckChoiceBoundings[nextTarget];
                }
                if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == b) {
                    selectIcon = true;
                    break;
                }
            }
        }
    }

    if (selectIcon) {
        if (mCurrentNavTarget != nextTarget) {
            if (mCurrentNavTarget >= 0) {
                if (mPhase == PHASE_SELECT_ITEM) {
                    if (mCurrentNavTarget == 12) {
                        mAnmItemSelect[i2].setBackwardsOnce();
                        mAnmItemSelect[2].setBackwardsOnce();
                        field_0x116DD = false;
                    } else if (mItemKinds[mCurrentNavTarget] == dLytCommonIconItem_c::B_WHEEL) {
                        mBWheelOrPouchItemIcons[mCurrentNavTarget].setOff();
                    } else {
                        mBWheelOrPouchItemIcons[mCurrentNavTarget + 12].setOff();
                    }
                } else {
                    mAnmMaterialCheck[i2 + mCurrentNavTarget].setBackwardsOnce();
                    if (mCurrentNavTarget == 0) {
                        mAnmMaterialCheck[4].setBackwardsOnce();
                    }
                }
            }

            if (mPhase == PHASE_SELECT_ITEM) {
                if (nextTarget == 12) {
                    mAnmItemSelect[i2].setAnimEnable(true);
                    mAnmItemSelect[i2].setFrame(0.0f);
                    mAnmItemSelect[i2].setForwardOnce();
                    mAnmItemSelect[2].setForwardOnce();
                    field_0x116DD = true;
                    if (mService == SERVICE_POTION_INFUSE) {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P1_POINT_BACK);
                    } else if (mService == SERVICE_GEAR_UPGRADE) {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P1_POINT_BACK);
                    } else {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RP_POINT_BACK);
                    }
                } else {
                    if (mItemKinds[nextTarget] == dLytCommonIconItem_c::B_WHEEL) {
                        mBWheelOrPouchItemIcons[nextTarget].setOn();
                    } else {
                        mBWheelOrPouchItemIcons[nextTarget + 12].setOn();
                    }
                    if (mService == SERVICE_POTION_INFUSE) {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P1_POINT_ITEM);
                    } else if (mService == SERVICE_GEAR_UPGRADE) {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P1_POINT_ITEM);
                    } else {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RP_POINT_ITEM);
                    }
                }
            } else {
                mAnmMaterialCheck[i2 + nextTarget].setAnimEnable(true);
                mAnmMaterialCheck[i2 + nextTarget].setFrame(0.0f);
                mAnmMaterialCheck[i2 + nextTarget].setForwardOnce();
                if (nextTarget == 0) {
                    mAnmMaterialCheck[4].setForwardOnce();
                }

                if (nextTarget == 0) {
                    field_0x116DD = true;
                    field_0x116DE = false;
                    if (mService == SERVICE_POTION_INFUSE) {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P2_POINT_BACK);
                    } else if (mService == SERVICE_GEAR_UPGRADE) {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P2_POINT_BACK);
                    }
                } else if (nextTarget == 1) {
                    field_0x116DE = true;
                    field_0x116DD = false;
                    if (mService == SERVICE_POTION_INFUSE) {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P2_POINT_BLEND);
                    } else if (mService == SERVICE_GEAR_UPGRADE) {
                        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P2_POINT_RECONST);
                    }
                }
            }
            mCurrentNavTarget = nextTarget;
            dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
        }
    } else {
        if (mCurrentNavTarget >= 0) {
            if (mPhase == PHASE_SELECT_ITEM) {
                if (mCurrentNavTarget == 12) {
                    mAnmItemSelect[i2].setBackwardsOnce();
                    mCurrentNavTarget = -13;
                } else {
                    if (mItemKinds[mCurrentNavTarget] == dLytCommonIconItem_c::B_WHEEL) {
                        mBWheelOrPouchItemIcons[mCurrentNavTarget].setOff();
                    } else {
                        mBWheelOrPouchItemIcons[mCurrentNavTarget + 12].setOff();
                    }
                    mCurrentNavTarget -= 12;
                }
            } else {
                mAnmMaterialCheck[i2 + mCurrentNavTarget].setBackwardsOnce();
                mCurrentNavTarget -= 2;
            }
            field_0x116DD = false;
            field_0x116DE = false;
        }
    }
}

void dLytShopMain_c::updateShopStuffMaybe() {
    // TODO - Nonmatching. Probably equivalent.

    SizedString<64> buf;
    ShopUpgradeCosts costs;
    if (mService != SERVICE_POTION_INFUSE) {
        costs = sGearUpgrades[mUpgradeIdx];
    } else {
        costs = sPotionUpgrades[mUpgradeIdx];
    }

    showShopLytMaybe();
    showPriceMaybe(costs.price);

    s32 someFrame = 0;

    s32 a1 = fn_802A4490(costs.oldItemId);
    s32 a2 = fn_802A4500(a1);
    s32 a3 = fn_802A5760(a2);
    field_0x11770[0] = a2;
    mUpgradePreviewIcons[a3].setUnk(false);
    mUpgradePreviewIcons[a3].setHasNumber(false);
    mUpgradePreviewIcons[a3].setItem(sLytIconMapping[a1]);
    mUpgradePreviewIcons[a3].setShieldOnOff(false);

    if (field_0x11770[0] == 0) {
        mUpgradeNodes[0].mpPane->SetVisible(true);
    } else {
        mUpgradeNodes[6].mpPane->SetVisible(true);
    }

    a1 = fn_802A4490(costs.newItemId);
    a2 = fn_802A4500(a1);
    a3 = fn_802A5760(a2);
    field_0x11770[1] = a2;
    mUpgradePreviewIcons[a3 + 1].setUnk(false);
    mUpgradePreviewIcons[a3 + 1].setHasNumber(false);
    mUpgradePreviewIcons[a3 + 1].setItem(sLytIconMapping[a1]);
    mUpgradePreviewIcons[a3 + 1].setShieldOnOff(false);

    if (field_0x11770[1] == 0) {
        mUpgradeNodes[1].mpPane->SetVisible(true);
    } else {
        mUpgradeNodes[7].mpPane->SetVisible(true);
    }

    mUpgradeCostCount = 2;

    // REGSWAPS start here.
    // Actual code: loop variables r18-r23, text boxes r14,r24-r28
    // Generated code: loop variables r23-r28, text boxes r14,r18-r22

    dTextBox_c *b1, *b2, *b3, *b4, *b5, *b6;

    for (int i = 0; i < 4; i++) {
        s32 i1 = i * 3 + 2;
        s32 i2 = i * 2;
        s32 i3 = i2 + 1;

        buf.sprintf("T_message_%02d", i1);
        b1 = mLyts[SHOP_LYT_MATERIAL_CHECK].getTextBox(buf);
        buf.sprintf("T_materialNum_%02d", i2);
        b2 = mLyts[SHOP_LYT_MATERIAL_CHECK].getTextBox(buf);
        buf.sprintf("T_materialNum_%02d", i3);
        b3 = mLyts[SHOP_LYT_MATERIAL_CHECK].getTextBox(buf);
        buf.sprintf("T_messageS_%02d", i1);
        b4 = mLyts[SHOP_LYT_MATERIAL_CHECK].getTextBox(buf);
        buf.sprintf("T_matNumS_%02d", i2);
        b5 = mLyts[SHOP_LYT_MATERIAL_CHECK].getTextBox(buf);
        buf.sprintf("T_matNumS_%02d", i3);
        b6 = mLyts[SHOP_LYT_MATERIAL_CHECK].getTextBox(buf);

        mAnmMaterialCheck[i + 11].setAnimEnable(true);

        static wchar_t sBuf[5];
        for (int j = 0; j < (int)ARRAY_LENGTH(sBuf); j++) {
            sBuf[j] = L'\0';
        }
        if (costs.matCosts[i].itemId < 0) {
            b1->setTextWithGlobalTextProcessor(sBuf, nullptr);
            b2->setTextWithGlobalTextProcessor(sBuf, nullptr);
            b3->setTextWithGlobalTextProcessor(sBuf, nullptr);
            b4->setTextWithGlobalTextProcessor(sBuf, nullptr);
            b5->setTextWithGlobalTextProcessor(sBuf, nullptr);
            b6->setTextWithGlobalTextProcessor(sBuf, nullptr);

            someFrame = 1;
            mUpgradeNodes[i + 2].mpPane->SetVisible(false);
            mUpgradeNodes[i + 8].mpPane->SetVisible(false);
        } else {
            // TODO check if we can avoid this cast...
            // getItemText has a bunch of other users as well
            const wchar_t *text = getItemText((u16)costs.matCosts[i].itemId);
            b1->setTextWithGlobalTextProcessor(text);
            b4->setTextWithGlobalTextProcessor(text);

            s32 amt = costs.matCosts[i].amount;
            (void)calcNumDigits(amt);
            swprintf(sBuf, ARRAY_LENGTH(sBuf), L"%d", amt);
            b2->setTextWithGlobalTextProcessor(sBuf, nullptr);
            b5->setTextWithGlobalTextProcessor(sBuf, nullptr);

            for (int j = 0; j < (int)ARRAY_LENGTH(sBuf); j++) {
                sBuf[j] = L'\0';
            }

            // TODO ??????????
            s32 counterIdx = costs.matCosts[i].itemId >= ITEM_HORNET_LARVAE ?
                                 costs.matCosts[i].itemId - ITEM_UNK_139 :
                                 costs.matCosts[i].itemId - ITEM_SMALL_QUIVER;

            // Probably some register weirdness around here
            bool shadow = !dAcItem_c::checkFlag((u16)costs.matCosts[i].itemId);
            s32 counter = getCounterByIndex(counterIdx);
            (void)calcNumDigits(counter);
            swprintf(sBuf, ARRAY_LENGTH(sBuf), L"%d", amt);
            b3->setTextWithGlobalTextProcessor(sBuf, nullptr);
            b6->setTextWithGlobalTextProcessor(sBuf, nullptr);

            if (counter != 0 && counter >= costs.matCosts[i].amount) {
                mAnmMaterialCheck[i + 11].setFrame(0.0f);
            } else {
                mAnmMaterialCheck[i + 11].setFrame(1.0f);
            }

            a1 = fn_802A4490(costs.matCosts[i].itemId);
            a2 = fn_802A4500(a1);
            a3 = fn_802A5780(a2);
            field_0x11770[mUpgradeCostCount] = a2;
            mUpgradeCostIcons[a3 + i].setBg(false);
            mUpgradeCostIcons[a3 + i].setHasNumber(false);
            mUpgradeCostIcons[a3 + i].setShadow(shadow);
            mUpgradeCostIcons[a3 + i].setItem(sLytIconMapping[a1]);

            if (field_0x11770[mUpgradeCostCount] == 0) {
                mUpgradeNodes[i + 2].mpPane->SetVisible(false);
                mUpgradeNodes[i + 8].mpPane->SetVisible(true);
            } else {
                mUpgradeNodes[i + 2].mpPane->SetVisible(true);
                mUpgradeNodes[i + 8].mpPane->SetVisible(false);
            }
            mUpgradeCostCount++;
        }
    }

    mAnmMaterialCheck[15].setAnimEnable(true);
    mAnmMaterialCheck[15].setFrame(someFrame);
    mUpgradeLoaded = true;
}

s32 dLytShopMain_c::calcNumDigits(s32 value) {
    int cmp = 1000;
    int i = 4;
    for (; i > 1; i--) {
        if (value >= cmp) {
            break;
        }
        cmp /= 10;
    }
    return i;
}

STATE_DEFINE(dLytShop_c, None);
STATE_DEFINE(dLytShop_c, SelectIn);
STATE_DEFINE(dLytShop_c, SelectCheck);
STATE_DEFINE(dLytShop_c, SelectOut);
STATE_DEFINE(dLytShop_c, OutIn);
STATE_DEFINE(dLytShop_c, MaterialCheck);
STATE_DEFINE(dLytShop_c, Out);
STATE_DEFINE(dLytShop_c, InOut);
STATE_DEFINE(dLytShop_c, End);

void dLytShop_c::initializeState_None() {}
void dLytShop_c::executeState_None() {
    if (mInRequest) {
        mMain.setModeSelectIn();
        mStateMgr.changeState(StateID_SelectIn);
        mInRequest = false;
    }
}
void dLytShop_c::finalizeState_None() {}

void dLytShop_c::initializeState_SelectIn() {}
void dLytShop_c::executeState_SelectIn() {
    if (mMain.isIdle()) {
        dPadNav::setNavEnabled(true, false);
        mMain.setModeSelectCheck();
        mStateMgr.changeState(StateID_SelectCheck);
    }
}
void dLytShop_c::finalizeState_SelectIn() {}

void dLytShop_c::initializeState_SelectCheck() {}
void dLytShop_c::executeState_SelectCheck() {
    if (mMain.isIdle()) {
        s32 service = mMain.getService();
        if (mMain.getField_0x116DD()) {
            mMain.setModeSelectOut();
            mStateMgr.changeState(StateID_SelectOut);

            if (service == dLytShopMain_c::SERVICE_POTION_INFUSE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P1_SELECT_BACK);
            } else if (service == dLytShopMain_c::SERVICE_GEAR_UPGRADE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P1_SELECT_BACK);
            } else {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RP_SELECT_BACK);
            }
        } else if (service == dLytShopMain_c::SERVICE_SHIELD_REPAIR) {
            mMain.setModeSelectOut();
            mStateMgr.changeState(StateID_SelectOut);
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RP_SELECT_ITEM);
        } else {
            mMain.setModeOutIn();
            mStateMgr.changeState(StateID_OutIn);
            if (service == dLytShopMain_c::SERVICE_POTION_INFUSE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P1_SELECT_ITEM);
            } else if (service == dLytShopMain_c::SERVICE_GEAR_UPGRADE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P1_SELECT_ITEM);
            }
        }
    }
}
void dLytShop_c::finalizeState_SelectCheck() {}

void dLytShop_c::initializeState_SelectOut() {}
void dLytShop_c::executeState_SelectOut() {
    if (mMain.isIdle()) {
        mIsMovingOut = true;
        mMain.setModeNone();
        mStateMgr.changeState(StateID_End);
    }
}
void dLytShop_c::finalizeState_SelectOut() {}

void dLytShop_c::initializeState_OutIn() {}
void dLytShop_c::executeState_OutIn() {
    if (mMain.isIdle()) {
        mMain.setModeMaterialCheck();
        mStateMgr.changeState(StateID_MaterialCheck);
    }
}
void dLytShop_c::finalizeState_OutIn() {}

void dLytShop_c::initializeState_MaterialCheck() {}
void dLytShop_c::executeState_MaterialCheck() {
    if (mMain.isIdle()) {
        s32 service = mMain.getService();
        if (mMain.getField_0x116DD()) {
            mMain.setModeInOut();
            mStateMgr.changeState(StateID_InOut);

            if (service == dLytShopMain_c::SERVICE_POTION_INFUSE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P2_SELECT_BACK);
            } else if (service == dLytShopMain_c::SERVICE_GEAR_UPGRADE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P2_SELECT_BACK);
            }
        } else if (mMain.getField_0x116DE()) {
            mMain.setModeOut();
            mStateMgr.changeState(StateID_Out);
            if (service == dLytShopMain_c::SERVICE_POTION_INFUSE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_MDS_P2_SELECT_BLEND);
            } else if (service == dLytShopMain_c::SERVICE_GEAR_UPGRADE) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SHOP_JNK_RC_P2_SELECT_RECONST);
            }
        } else {
            mMain.retry();
        }
    }
}
void dLytShop_c::finalizeState_MaterialCheck() {}

void dLytShop_c::initializeState_Out() {}
void dLytShop_c::executeState_Out() {
    if (mMain.isIdle()) {
        mMain.setModeNone();
        mStateMgr.changeState(StateID_End);
    }
}
void dLytShop_c::finalizeState_Out() {}

void dLytShop_c::initializeState_InOut() {}
void dLytShop_c::executeState_InOut() {
    if (mMain.isIdle()) {
        mMain.setModeSelectCheck();
        mStateMgr.changeState(StateID_SelectCheck);
    }
}
void dLytShop_c::finalizeState_InOut() {}

void dLytShop_c::initializeState_End() {}
void dLytShop_c::executeState_End() {}
void dLytShop_c::finalizeState_End() {}

SPECIAL_BASE_PROFILE(LYT_SHOP, dLytShop_c, fProfile::LYT_SHOP, 0x2B8, 0);

bool dLytShop_c::build() {
    void *data = LayoutArcManager::GetInstance()->getLoadedData("Shop");
    mResAcc.attach(data, "");
    mMain.build(&mResAcc);
    return true;
}

int dLytShop_c::create() {
    sInstance = this;
    allocateLytWork1Heap("dLytShop_c::m_allocator", false);
    mStateMgr.changeState(StateID_None);
    mInRequest = false;
    mIsMovingOut = false;
    return SUCCEEDED;
}

int dLytShop_c::execute() {
    if (*mStateMgr.getStateID() != StateID_None) {
        if (dPad::getDownTrigDown()) {
            dPad::ex_c::getInstance()->centerCursor(mPad::getCurrentCoreID(), true);
        }
        if (dCsBase_c::GetInstance()->isVisible()) {
            dCsGame_c::GetInstance()->pointerDefSet();
        }
        mMain.execute();
    }
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dLytShop_c::draw() {
    if (*mStateMgr.getStateID() != StateID_None || mIsMovingOut) {
        mMain.draw();
    }
    return SUCCEEDED;
}

int dLytShop_c::doDelete() {
    mMain.remove();
    mResAcc.detach();
    sInstance = nullptr;
    return SUCCEEDED;
}
