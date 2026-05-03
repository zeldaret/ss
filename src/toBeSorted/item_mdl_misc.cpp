#include "toBeSorted/item_mdl_misc.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "m/m3d/m_fanm.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/item_mdl_name.h"

enum MdlIdx {
    MDL_PUT_ARROW,
    MDL_PUT_ARROW_BUNDLE,
    MDL_PUT_HEART,
    MDL_PUT_KEY_SMALL,
    MDL_PUT_BOMB_SET,
    MDL_PUT_SEED,
    MDL_PUT_HEART_UTUWA,
    MDL_PUT_HEART_KAKERA,
    MDL_PUT_TRI_FORCE,
    MDL_PUT_GARAGARA,
    MDL_PUT_MAX,
};

const u8 dItemMdlPut_c::sMdlIdx[] = {
    MDL_PUT_KEY_SMALL,    /* ITEMMDL_SMALL_KEY */
    MDL_PUT_MAX,          /* ITEMMDL_GREEN_RUPEE */
    MDL_PUT_MAX,          /* ITEMMDL_BLUE_RUPEE */
    MDL_PUT_MAX,          /* ITEMMDL_RED_RUPEE */
    MDL_PUT_MAX,          /* ITEMMDL_COMPLETE_TRIFORCE */
    MDL_PUT_HEART,        /* ITEMMDL_HEART */
    MDL_PUT_ARROW,        /* ITEMMDL_SINGLE_ARROW */
    MDL_PUT_ARROW_BUNDLE, /* ITEMMDL_BUNDLE_ARROWS */
    MDL_PUT_MAX,          /* ITEMMDL_GODDESS_WHITESWORD */
    MDL_PUT_MAX,          /* ITEMMDL_PRACTICE_SWORD */
    MDL_PUT_MAX,          /* ITEMMDL_GODDESS_SWORD */
    MDL_PUT_MAX,          /* ITEMMDL_GODDESS_LONGSWORD */
    MDL_PUT_MAX,          /* ITEMMDL_MASTER_SWORD */
    MDL_PUT_MAX,          /* ITEMMDL_TRUE_MASTER_SWORD */
    MDL_PUT_MAX,          /* ITEMMDL_SAILCLOTH */
    MDL_PUT_MAX,          /* ITEMMDL_GODDESS_HARP */
    MDL_PUT_MAX,          /* ITEMMDL_SPIRIT_VESSEL */
    MDL_PUT_MAX,          /* ITEMMDL_BOW */
    MDL_PUT_MAX,          /* ITEMMDL_CLAWSHOTS */
    MDL_PUT_MAX,          /* ITEMMDL_BIRD_STATUETTE */
    MDL_PUT_MAX,          /* ITEMMDL_AC_BK */
    MDL_PUT_MAX,          /* ITEMMDL_FS_BK */
    MDL_PUT_MAX,          /* ITEMMDL_SSH_BK */
    MDL_PUT_MAX,          /* ITEMMDL_KEY_PIECE */
    MDL_PUT_MAX,          /* ITEMMDL_SV_BK */
    MDL_PUT_MAX,          /* ITEMMDL_ET_BK */
    MDL_PUT_MAX,          /* ITEMMDL_LMF_BK */
    MDL_PUT_MAX,          /* ITEMMDL_SILVER_RUPEE */
    MDL_PUT_MAX,          /* ITEMMDL_GOLD_RUPEE */
    MDL_PUT_MAX,          /* ITEMMDL_RUPOOR */
    MDL_PUT_MAX,          /* ITEMMDL_5_CRYSTALS */
    MDL_PUT_MAX,          /* ITEMMDL_GLITTERING_SPORES */
    MDL_PUT_MAX,          /* ITEMMDL_5_BOMBS */
    MDL_PUT_BOMB_SET,     /* ITEMMDL_10_BOMBS */
    MDL_PUT_MAX,          /* ITEMMDL_STAMINA_FRUIT */
    MDL_PUT_MAX,          /* ITEMMDL_FARORE_TEAR */
    MDL_PUT_MAX,          /* ITEMMDL_DIN_TEAR */
    MDL_PUT_MAX,          /* ITEMMDL_NAYRU_TEAR */
    MDL_PUT_MAX,          /* ITEMMDL_SACRED_TEAR */
    MDL_PUT_MAX,          /* ITEMMDL_LIGHT_FRUIT */
    MDL_PUT_MAX,          /* ITEMMDL_1_CRYSTAL */
    MDL_PUT_MAX,          /* ITEMMDL_GUST_BELLOWS */
    MDL_PUT_MAX,          /* ITEMMDL_DUNGEON_MAP_FI */
    MDL_PUT_MAX,          /* ITEMMDL_DUNGEON_MAP */
    MDL_PUT_MAX,          /* ITEMMDL_SLINGSHOT */
    MDL_PUT_MAX,          /* ITEMMDL_BEETLE */
    MDL_PUT_MAX,          /* ITEMMDL_WATER */
    MDL_PUT_MAX,          /* ITEMMDL_MUSHROOM_SPORES */
    MDL_PUT_MAX,          /* ITEMMDL_DIGGING_MITTS */
    MDL_PUT_SEED,         /* ITEMMDL_5_DEKU_SEEDS */
    MDL_PUT_MAX,          /* ITEMMDL_10_DEKU_SEEDS */
    MDL_PUT_MAX,          /* ITEMMDL_GUARDIAN_POTION */
    MDL_PUT_MAX,          /* ITEMMDL_GUARDIAN_POTION_PLUS */
    MDL_PUT_MAX,          /* ITEMMDL_WATER_SCALE */
    MDL_PUT_MAX,          /* ITEMMDL_BUG_MEDAL */
    MDL_PUT_MAX,          /* ITEMMDL_BUG_NET */
    MDL_PUT_MAX,          /* ITEMMDL_FAIRY */
    MDL_PUT_MAX,          /* ITEMMDL_SACRED_WATER */
    MDL_PUT_MAX,          /* ITEMMDL_HOOK_BEETLE */
    MDL_PUT_MAX,          /* ITEMMDL_QUICK_BEETLE */
    MDL_PUT_MAX,          /* ITEMMDL_TOUGH_BEETLE */
    MDL_PUT_MAX,          /* ITEMMDL_HEART_POTION */
    MDL_PUT_MAX,          /* ITEMMDL_HEART_POTION_PLUS */
    MDL_PUT_MAX,          /* ITEMMDL_HEART_POTION_ALSO_PLUS */
    MDL_PUT_MAX,          /* ITEMMDL_STAMINA_POTION */
    MDL_PUT_MAX,          /* ITEMMDL_STAMINA_POTION_PLUS */
    MDL_PUT_MAX,          /* ITEMMDL_AIR_POTION */
    MDL_PUT_MAX,          /* ITEMMDL_AIR_POTION_PLUS */
    MDL_PUT_MAX,          /* ITEMMDL_BOTTLED_FAIRY */
    MDL_PUT_MAX,          /* ITEMMDL_IRON_BOW */
    MDL_PUT_MAX,          /* ITEMMDL_SACRED_BOW */
    MDL_PUT_MAX,          /* ITEMMDL_BOMB_BAG */
    MDL_PUT_HEART_UTUWA,  /* ITEMMDL_HEART_CONTAINER */
    MDL_PUT_HEART_KAKERA, /* ITEMMDL_HEART_PIECE */
    MDL_PUT_TRI_FORCE,    /* ITEMMDL_TRI_COURAGE */
    MDL_PUT_TRI_FORCE,    /* ITEMMDL_TRI_POWER */
    MDL_PUT_TRI_FORCE,    /* ITEMMDL_TRI_WISDOWM */
    MDL_PUT_MAX,          /* ITEMMDL_SEA_CHART */
    MDL_PUT_MAX,          /* ITEMMDL_MOGMA_MITTS */
    MDL_PUT_MAX,          /* ITEMMDL_HEART_MEDAL */
    MDL_PUT_MAX,          /* ITEMMDL_RUPEE_MEDAL */
    MDL_PUT_MAX,          /* ITEMMDL_TREASURE_MEDAL */
    MDL_PUT_MAX,          /* ITEMMDL_POTION_MEDAL */
    MDL_PUT_MAX,          /* ITEMMDL_CURSED_MEDAL */
    MDL_PUT_MAX,          /* ITEMMDL_MIGHTY_SCATTERSHOT */
    MDL_PUT_MAX,          /* ITEMMDL_85 */
    MDL_PUT_MAX,          /* ITEMMDL_MEDIUM_WALLET */
    MDL_PUT_MAX,          /* ITEMMDL_BIG_WALLET */
    MDL_PUT_MAX,          /* ITEMMDL_GIANT_WALLET */
    MDL_PUT_MAX,          /* ITEMMDL_TYCOON_WALLET */
    MDL_PUT_MAX,          /* ITEMMDL_ADVENTURE_POUCH */
    MDL_PUT_MAX,          /* ITEMMDL_POUCH_EXPANSION */
    MDL_PUT_MAX,          /* ITEMMDL_LIFE_MEDAL */
    MDL_PUT_MAX,          /* ITEMMDL_WOODEN_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_BANDED_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_BRACED_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_IRON_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_REINFORCED_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_FORTIFIED_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_SACRED_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_DIVINE_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_GODDESS_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_HYLIAN_SHIELD */
    MDL_PUT_MAX,          /* ITEMMDL_REVITALIZING_POTION */
    MDL_PUT_MAX,          /* ITEMMDL_REVITALIZING_POTION_PLUS */
    MDL_PUT_MAX,          /* ITEMMDL_SMALL_SEED_SATCHEL */
    MDL_PUT_MAX,          /* ITEMMDL_MEDIUM_SEED_SATCHEL */
    MDL_PUT_MAX,          /* ITEMMDL_LARGE_SEED_SATCHEL */
    MDL_PUT_MAX,          /* ITEMMDL_SMALL_QUIVER */
    MDL_PUT_MAX,          /* ITEMMDL_MEDIUM_QUIVER */
    MDL_PUT_MAX,          /* ITEMMDL_LARGE_QUIVER */
    MDL_PUT_MAX,          /* ITEMMDL_SMALL_BOMB_BAG */
    MDL_PUT_MAX,          /* ITEMMDL_MEDIUM_BOMB_BAG */
    MDL_PUT_MAX,          /* ITEMMDL_LARGE_BOMB_BAG */
    MDL_PUT_MAX,          /* ITEMMDL_WHIP */
    MDL_PUT_MAX,          /* ITEMMDL_FIRESHIELD_EARRINGS */
    MDL_PUT_MAX,          /* ITEMMDL_BIG_BUG_NET */
    MDL_PUT_MAX,          /* ITEMMDL_FARON_GRASSHOPPER */
    MDL_PUT_MAX,          /* ITEMMDL_WOODLAND_RHINO_BEETLE */
    MDL_PUT_MAX,          /* ITEMMDL_DEKU_HORNET */
    MDL_PUT_MAX,          /* ITEMMDL_SKYLOFT_MANTIS */
    MDL_PUT_MAX,          /* ITEMMDL_VOLCANIC_LADYBUG */
    MDL_PUT_MAX,          /* ITEMMDL_BLESSED_BUTTERFLY */
    MDL_PUT_MAX,          /* ITEMMDL_LANAYRU_ANT */
    MDL_PUT_MAX,          /* ITEMMDL_SAND_CICADA */
    MDL_PUT_MAX,          /* ITEMMDL_GERUDO_DRAGONFLY */
    MDL_PUT_MAX,          /* ITEMMDL_ELDIN_ROLLER */
    MDL_PUT_MAX,          /* ITEMMDL_SKY_STAG_BEETLE */
    MDL_PUT_MAX,          /* ITEMMDL_STARRY_FIREFLY */
    MDL_PUT_MAX,          /* ITEMMDL_BOTTLE */
    MDL_PUT_MAX,          /* ITEMMDL_CAWLIN_LETTER */
    MDL_PUT_MAX,          /* ITEMMDL_BEEDLE_INSECT_CAGE */
    MDL_PUT_GARAGARA,     /* ITEMMDL_RATTLE */
    MDL_PUT_MAX,          /* ITEMMDL_HORNET_LARVAE */
    MDL_PUT_MAX,          /* ITEMMDL_BIRD_FEATHER */
    MDL_PUT_MAX,          /* ITEMMDL_TUMBLE_WEED */
    MDL_PUT_MAX,          /* ITEMMDL_LIZARD_TAIL */
    MDL_PUT_MAX,          /* ITEMMDL_ELDIN_ORE */
    MDL_PUT_MAX,          /* ITEMMDL_ANCIENT_FLOWER */
    MDL_PUT_MAX,          /* ITEMMDL_AMBER_RELIC */
    MDL_PUT_MAX,          /* ITEMMDL_DUSK_RELIC */
    MDL_PUT_MAX,          /* ITEMMDL_JELLY_BLOB */
    MDL_PUT_MAX,          /* ITEMMDL_MONSTER_CLAW */
    MDL_PUT_MAX,          /* ITEMMDL_MONSTER_HORN */
    MDL_PUT_MAX,          /* ITEMMDL_ORNAMENTAL_SKULL */
    MDL_PUT_MAX,          /* ITEMMDL_EVIL_CRYSTAL */
    MDL_PUT_MAX,          /* ITEMMDL_BLUE_BIRD_FEATHER */
    MDL_PUT_MAX,          /* ITEMMDL_GOLDEN_SKULL */
    MDL_PUT_MAX,          /* ITEMMDL_GODDESS_PLUME */
    MDL_PUT_MAX,          /* ITEMMDL_EMERALD_TABLET */
    MDL_PUT_MAX,          /* ITEMMDL_RUBY_TABLET */
    MDL_PUT_MAX,          /* ITEMMDL_AMBER_TABLET */
    MDL_PUT_MAX,          /* ITEMMDL_STONE_OF_TRIALS */
    MDL_PUT_MAX,          /* ITEMMDL_BALLAD_OF_THE_GODDESS */
    MDL_PUT_MAX,          /* ITEMMDL_FARORES_COURAGE */
    MDL_PUT_MAX,          /* ITEMMDL_NAYRUS_WISDOM */
    MDL_PUT_MAX,          /* ITEMMDL_DINS_POWER */
    MDL_PUT_MAX,          /* ITEMMDL_FARON_SOTH */
    MDL_PUT_MAX,          /* ITEMMDL_ELDIN_SOTH */
    MDL_PUT_MAX,          /* ITEMMDL_LANAYRU_SOTH */
    MDL_PUT_MAX,          /* ITEMMDL_ITEM_SOTH */
    MDL_PUT_MAX,          /* ITEMMDL_REVITALIZING_POTION_PLUS_PLUS */
    MDL_PUT_MAX,          /* ITEMMDL_HOT_SOUP */
    MDL_PUT_MAX,          /* ITEMMDL_COLD_SOUP */
    MDL_PUT_MAX,          /* ITEMMDL_LIFE_TREE_SEED */
    MDL_PUT_MAX,          /* ITEMMDL_LIFE_TREE_FRUIT */
    MDL_PUT_MAX,          /* ITEMMDL_EXTRA_WALLET */
};

const dItemMdlPut_c::MdlConfig dItemMdlPut_c::sMdlConfig[] = {
    {         "PutArrow",          "PutArrow",          "GetArrow",          "GetArrow"},
    {         "PutArrow",    "PutArrowBundle",          "GetArrow",    "GetArrowBundle"},
    {         "PutHeart",          "PutHeart",          "GetHeart",          "GetHeart"},
    {      "PutKeySmall", "PutKeySmallNormal",       "GetKeySmall", "GetKeySmallNormal"},
    {       "PutBombSet",        "PutBombSet",        "GetBombSet",        "GetBombSet"},
    {          "PutSeed",           "PutSeed",           "GetSeed",           "GetSeed"},
    {    "PutHeartUtuwa",     "PutHeartUtuwa",     "GetHeartUtuwa",     "GetHeartUtuwa"},
    {   "PutHeartKakera",    "PutHeartKakera",    "GetHeartKakera",    "GetHeartKakera"},
    {"PutTriForceSingle", "PutTriForceSingle", "GetTriForceSingle", "GetTriForceSingle"},
    {      "PutGaragara",       "PutGaragara",       "GetGaragara",       "GetGaragara"},
};

bool dItemMdlPut_c::isMdlForItemId(u16 itemId) {
    if (hasItemMdl(itemId) == false) {
        return false;
    } else {
        return sMdlIdx[getItemMdlIdx(itemId)] <= MDL_PUT_MAX - 1;
    }
}

bool dItemMdlPut_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    s32 mdlIdx = getItemMdlIdx(itemId);
    u8 mdlConfigIdx = sMdlIdx[mdlIdx];

    const MdlConfig *fileAndMdl = &sMdlConfig[mdlConfigIdx];

    void *resData0 = OarcManager::GetInstance()->getMdlFromArc2(fileAndMdl->putResFile);
    if (resData0 == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile res0(resData0);
    if (!res0.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl0(res0.GetResMdl(fileAndMdl->putResMdl));
    if (!mdl0.IsValid()) {
        return false;
    }

    if (!mPutMdl.create(mdl0, allocator, 0x120)) {
        return false;
    }

    bool isPickupArrow = item->isSingleArrow() && item->getParams2Lower_shift1_0x7() == 0;
    void *resData1 = OarcManager::GetInstance()->getMdlFromArc2(fileAndMdl->getResFile);
    if (resData1 == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile res1(resData1);
    if (!res1.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl1(res1.GetResMdl(isPickupArrow ? "GetArrowEnemy" : fileAndMdl->getResMdl));
    if (!mdl1.IsValid()) {
        return false;
    }

    if (!mGetMdl.create(mdl1, allocator, 0x120)) {
        return false;
    }

    mpItem = item;
    return true;
}

void dItemMdlPut_c::setDrawMode(u8 arg) {
    if (arg == DrawMode_Max) {
        mDrawMode = DrawMode_Put;
    } else {
        mDrawMode = arg;
    }
}

void dItemMdlPut_c::setScale(const mVec3_c &scale) {
    mPutMdl.setScale(scale);
    mGetMdl.setScale(scale);
}

void dItemMdlPut_c::setLocalMtx(const mMtx_c &mtx) {
    mPutMdl.setLocalMtx(mtx);
    mGetMdl.setLocalMtx(mtx);
}

void dItemMdlPut_c::draw() {
    switch (mDrawMode) {
        case DrawMode_Put: mpItem->fn_8002ECD0(&mPutMdl, 7); break;
        case DrawMode_Get: mpItem->fn_8002ECD0(&mGetMdl, 6); break;
    }
}

void dItemMdlPut_c::changeItemId(u16 itemId) {
    // no-op
}

void dItemMdlPut_c::setPriorityDraw() {
    mPutMdl.setPriorityDraw(0x82, 0x7F);
    mGetMdl.setPriorityDraw(0x82, 0x7F);
}

void dItemMdlPut_c::unsetPriorityDraw() {
    mPutMdl.setPriorityDraw(0x7F, 0x7F);
    mGetMdl.setPriorityDraw(0x7F, 0x7F);
}

enum ShieldIdx {
    MDL_SHIELD_WOOD_0,
    MDL_SHIELD_WOOD_1,
    MDL_SHIELD_WOOD_2,
    MDL_SHIELD_IRON_0,
    MDL_SHIELD_IRON_1,
    MDL_SHIELD_IRON_2,
    MDL_SHIELD_HOLY_0,
    MDL_SHIELD_HOLY_1,
    MDL_SHIELD_HOLY_2,
    MDL_SHIELD_MAX
};

const u8 dItemMdlShield_c::sMdlIdx[] = {
    MDL_SHIELD_MAX,    /* ITEMMDL_SMALL_KEY */
    MDL_SHIELD_MAX,    /* ITEMMDL_GREEN_RUPEE */
    MDL_SHIELD_MAX,    /* ITEMMDL_BLUE_RUPEE */
    MDL_SHIELD_MAX,    /* ITEMMDL_RED_RUPEE */
    MDL_SHIELD_MAX,    /* ITEMMDL_COMPLETE_TRIFORCE */
    MDL_SHIELD_MAX,    /* ITEMMDL_HEART */
    MDL_SHIELD_MAX,    /* ITEMMDL_SINGLE_ARROW */
    MDL_SHIELD_MAX,    /* ITEMMDL_BUNDLE_ARROWS */
    MDL_SHIELD_MAX,    /* ITEMMDL_GODDESS_WHITESWORD */
    MDL_SHIELD_MAX,    /* ITEMMDL_PRACTICE_SWORD */
    MDL_SHIELD_MAX,    /* ITEMMDL_GODDESS_SWORD */
    MDL_SHIELD_MAX,    /* ITEMMDL_GODDESS_LONGSWORD */
    MDL_SHIELD_MAX,    /* ITEMMDL_MASTER_SWORD */
    MDL_SHIELD_MAX,    /* ITEMMDL_TRUE_MASTER_SWORD */
    MDL_SHIELD_MAX,    /* ITEMMDL_SAILCLOTH */
    MDL_SHIELD_MAX,    /* ITEMMDL_GODDESS_HARP */
    MDL_SHIELD_MAX,    /* ITEMMDL_SPIRIT_VESSEL */
    MDL_SHIELD_MAX,    /* ITEMMDL_BOW */
    MDL_SHIELD_MAX,    /* ITEMMDL_CLAWSHOTS */
    MDL_SHIELD_MAX,    /* ITEMMDL_BIRD_STATUETTE */
    MDL_SHIELD_MAX,    /* ITEMMDL_AC_BK */
    MDL_SHIELD_MAX,    /* ITEMMDL_FS_BK */
    MDL_SHIELD_MAX,    /* ITEMMDL_SSH_BK */
    MDL_SHIELD_MAX,    /* ITEMMDL_KEY_PIECE */
    MDL_SHIELD_MAX,    /* ITEMMDL_SV_BK */
    MDL_SHIELD_MAX,    /* ITEMMDL_ET_BK */
    MDL_SHIELD_MAX,    /* ITEMMDL_LMF_BK */
    MDL_SHIELD_MAX,    /* ITEMMDL_SILVER_RUPEE */
    MDL_SHIELD_MAX,    /* ITEMMDL_GOLD_RUPEE */
    MDL_SHIELD_MAX,    /* ITEMMDL_RUPOOR */
    MDL_SHIELD_MAX,    /* ITEMMDL_5_CRYSTALS */
    MDL_SHIELD_MAX,    /* ITEMMDL_GLITTERING_SPORES */
    MDL_SHIELD_MAX,    /* ITEMMDL_5_BOMBS */
    MDL_SHIELD_MAX,    /* ITEMMDL_10_BOMBS */
    MDL_SHIELD_MAX,    /* ITEMMDL_STAMINA_FRUIT */
    MDL_SHIELD_MAX,    /* ITEMMDL_FARORE_TEAR */
    MDL_SHIELD_MAX,    /* ITEMMDL_DIN_TEAR */
    MDL_SHIELD_MAX,    /* ITEMMDL_NAYRU_TEAR */
    MDL_SHIELD_MAX,    /* ITEMMDL_SACRED_TEAR */
    MDL_SHIELD_MAX,    /* ITEMMDL_LIGHT_FRUIT */
    MDL_SHIELD_MAX,    /* ITEMMDL_1_CRYSTAL */
    MDL_SHIELD_MAX,    /* ITEMMDL_GUST_BELLOWS */
    MDL_SHIELD_MAX,    /* ITEMMDL_DUNGEON_MAP_FI */
    MDL_SHIELD_MAX,    /* ITEMMDL_DUNGEON_MAP */
    MDL_SHIELD_MAX,    /* ITEMMDL_SLINGSHOT */
    MDL_SHIELD_MAX,    /* ITEMMDL_BEETLE */
    MDL_SHIELD_MAX,    /* ITEMMDL_WATER */
    MDL_SHIELD_MAX,    /* ITEMMDL_MUSHROOM_SPORES */
    MDL_SHIELD_MAX,    /* ITEMMDL_DIGGING_MITTS */
    MDL_SHIELD_MAX,    /* ITEMMDL_5_DEKU_SEEDS */
    MDL_SHIELD_MAX,    /* ITEMMDL_10_DEKU_SEEDS */
    MDL_SHIELD_MAX,    /* ITEMMDL_GUARDIAN_POTION */
    MDL_SHIELD_MAX,    /* ITEMMDL_GUARDIAN_POTION_PLUS */
    MDL_SHIELD_MAX,    /* ITEMMDL_WATER_SCALE */
    MDL_SHIELD_MAX,    /* ITEMMDL_BUG_MEDAL */
    MDL_SHIELD_MAX,    /* ITEMMDL_BUG_NET */
    MDL_SHIELD_MAX,    /* ITEMMDL_FAIRY */
    MDL_SHIELD_MAX,    /* ITEMMDL_SACRED_WATER */
    MDL_SHIELD_MAX,    /* ITEMMDL_HOOK_BEETLE */
    MDL_SHIELD_MAX,    /* ITEMMDL_QUICK_BEETLE */
    MDL_SHIELD_MAX,    /* ITEMMDL_TOUGH_BEETLE */
    MDL_SHIELD_MAX,    /* ITEMMDL_HEART_POTION */
    MDL_SHIELD_MAX,    /* ITEMMDL_HEART_POTION_PLUS */
    MDL_SHIELD_MAX,    /* ITEMMDL_HEART_POTION_ALSO_PLUS */
    MDL_SHIELD_MAX,    /* ITEMMDL_STAMINA_POTION */
    MDL_SHIELD_MAX,    /* ITEMMDL_STAMINA_POTION_PLUS */
    MDL_SHIELD_MAX,    /* ITEMMDL_AIR_POTION */
    MDL_SHIELD_MAX,    /* ITEMMDL_AIR_POTION_PLUS */
    MDL_SHIELD_MAX,    /* ITEMMDL_BOTTLED_FAIRY */
    MDL_SHIELD_MAX,    /* ITEMMDL_IRON_BOW */
    MDL_SHIELD_MAX,    /* ITEMMDL_SACRED_BOW */
    MDL_SHIELD_MAX,    /* ITEMMDL_BOMB_BAG */
    MDL_SHIELD_MAX,    /* ITEMMDL_HEART_CONTAINER */
    MDL_SHIELD_MAX,    /* ITEMMDL_HEART_PIECE */
    MDL_SHIELD_MAX,    /* ITEMMDL_TRI_COURAGE */
    MDL_SHIELD_MAX,    /* ITEMMDL_TRI_POWER */
    MDL_SHIELD_MAX,    /* ITEMMDL_TRI_WISDOWM */
    MDL_SHIELD_MAX,    /* ITEMMDL_SEA_CHART */
    MDL_SHIELD_MAX,    /* ITEMMDL_MOGMA_MITTS */
    MDL_SHIELD_MAX,    /* ITEMMDL_HEART_MEDAL */
    MDL_SHIELD_MAX,    /* ITEMMDL_RUPEE_MEDAL */
    MDL_SHIELD_MAX,    /* ITEMMDL_TREASURE_MEDAL */
    MDL_SHIELD_MAX,    /* ITEMMDL_POTION_MEDAL */
    MDL_SHIELD_MAX,    /* ITEMMDL_CURSED_MEDAL */
    MDL_SHIELD_MAX,    /* ITEMMDL_MIGHTY_SCATTERSHOT */
    MDL_SHIELD_MAX,    /* ITEMMDL_85 */
    MDL_SHIELD_MAX,    /* ITEMMDL_MEDIUM_WALLET */
    MDL_SHIELD_MAX,    /* ITEMMDL_BIG_WALLET */
    MDL_SHIELD_MAX,    /* ITEMMDL_GIANT_WALLET */
    MDL_SHIELD_MAX,    /* ITEMMDL_TYCOON_WALLET */
    MDL_SHIELD_MAX,    /* ITEMMDL_ADVENTURE_POUCH */
    MDL_SHIELD_MAX,    /* ITEMMDL_POUCH_EXPANSION */
    MDL_SHIELD_MAX,    /* ITEMMDL_LIFE_MEDAL */
    MDL_SHIELD_WOOD_0, /* ITEMMDL_WOODEN_SHIELD */
    MDL_SHIELD_WOOD_1, /* ITEMMDL_BANDED_SHIELD */
    MDL_SHIELD_WOOD_2, /* ITEMMDL_BRACED_SHIELD */
    MDL_SHIELD_IRON_0, /* ITEMMDL_IRON_SHIELD */
    MDL_SHIELD_IRON_1, /* ITEMMDL_REINFORCED_SHIELD */
    MDL_SHIELD_IRON_2, /* ITEMMDL_FORTIFIED_SHIELD */
    MDL_SHIELD_HOLY_0, /* ITEMMDL_SACRED_SHIELD */
    MDL_SHIELD_HOLY_1, /* ITEMMDL_DIVINE_SHIELD */
    MDL_SHIELD_HOLY_2, /* ITEMMDL_GODDESS_SHIELD */
    MDL_SHIELD_MAX,    /* ITEMMDL_HYLIAN_SHIELD */
    MDL_SHIELD_MAX,    /* ITEMMDL_REVITALIZING_POTION */
    MDL_SHIELD_MAX,    /* ITEMMDL_REVITALIZING_POTION_PLUS */
    MDL_SHIELD_MAX,    /* ITEMMDL_SMALL_SEED_SATCHEL */
    MDL_SHIELD_MAX,    /* ITEMMDL_MEDIUM_SEED_SATCHEL */
    MDL_SHIELD_MAX,    /* ITEMMDL_LARGE_SEED_SATCHEL */
    MDL_SHIELD_MAX,    /* ITEMMDL_SMALL_QUIVER */
    MDL_SHIELD_MAX,    /* ITEMMDL_MEDIUM_QUIVER */
    MDL_SHIELD_MAX,    /* ITEMMDL_LARGE_QUIVER */
    MDL_SHIELD_MAX,    /* ITEMMDL_SMALL_BOMB_BAG */
    MDL_SHIELD_MAX,    /* ITEMMDL_MEDIUM_BOMB_BAG */
    MDL_SHIELD_MAX,    /* ITEMMDL_LARGE_BOMB_BAG */
    MDL_SHIELD_MAX,    /* ITEMMDL_WHIP */
    MDL_SHIELD_MAX,    /* ITEMMDL_FIRESHIELD_EARRINGS */
    MDL_SHIELD_MAX,    /* ITEMMDL_BIG_BUG_NET */
    MDL_SHIELD_MAX,    /* ITEMMDL_FARON_GRASSHOPPER */
    MDL_SHIELD_MAX,    /* ITEMMDL_WOODLAND_RHINO_BEETLE */
    MDL_SHIELD_MAX,    /* ITEMMDL_DEKU_HORNET */
    MDL_SHIELD_MAX,    /* ITEMMDL_SKYLOFT_MANTIS */
    MDL_SHIELD_MAX,    /* ITEMMDL_VOLCANIC_LADYBUG */
    MDL_SHIELD_MAX,    /* ITEMMDL_BLESSED_BUTTERFLY */
    MDL_SHIELD_MAX,    /* ITEMMDL_LANAYRU_ANT */
    MDL_SHIELD_MAX,    /* ITEMMDL_SAND_CICADA */
    MDL_SHIELD_MAX,    /* ITEMMDL_GERUDO_DRAGONFLY */
    MDL_SHIELD_MAX,    /* ITEMMDL_ELDIN_ROLLER */
    MDL_SHIELD_MAX,    /* ITEMMDL_SKY_STAG_BEETLE */
    MDL_SHIELD_MAX,    /* ITEMMDL_STARRY_FIREFLY */
    MDL_SHIELD_MAX,    /* ITEMMDL_BOTTLE */
    MDL_SHIELD_MAX,    /* ITEMMDL_CAWLIN_LETTER */
    MDL_SHIELD_MAX,    /* ITEMMDL_BEEDLE_INSECT_CAGE */
    MDL_SHIELD_MAX,    /* ITEMMDL_RATTLE */
    MDL_SHIELD_MAX,    /* ITEMMDL_HORNET_LARVAE */
    MDL_SHIELD_MAX,    /* ITEMMDL_BIRD_FEATHER */
    MDL_SHIELD_MAX,    /* ITEMMDL_TUMBLE_WEED */
    MDL_SHIELD_MAX,    /* ITEMMDL_LIZARD_TAIL */
    MDL_SHIELD_MAX,    /* ITEMMDL_ELDIN_ORE */
    MDL_SHIELD_MAX,    /* ITEMMDL_ANCIENT_FLOWER */
    MDL_SHIELD_MAX,    /* ITEMMDL_AMBER_RELIC */
    MDL_SHIELD_MAX,    /* ITEMMDL_DUSK_RELIC */
    MDL_SHIELD_MAX,    /* ITEMMDL_JELLY_BLOB */
    MDL_SHIELD_MAX,    /* ITEMMDL_MONSTER_CLAW */
    MDL_SHIELD_MAX,    /* ITEMMDL_MONSTER_HORN */
    MDL_SHIELD_MAX,    /* ITEMMDL_ORNAMENTAL_SKULL */
    MDL_SHIELD_MAX,    /* ITEMMDL_EVIL_CRYSTAL */
    MDL_SHIELD_MAX,    /* ITEMMDL_BLUE_BIRD_FEATHER */
    MDL_SHIELD_MAX,    /* ITEMMDL_GOLDEN_SKULL */
    MDL_SHIELD_MAX,    /* ITEMMDL_GODDESS_PLUME */
    MDL_SHIELD_MAX,    /* ITEMMDL_EMERALD_TABLET */
    MDL_SHIELD_MAX,    /* ITEMMDL_RUBY_TABLET */
    MDL_SHIELD_MAX,    /* ITEMMDL_AMBER_TABLET */
    MDL_SHIELD_MAX,    /* ITEMMDL_STONE_OF_TRIALS */
    MDL_SHIELD_MAX,    /* ITEMMDL_BALLAD_OF_THE_GODDESS */
    MDL_SHIELD_MAX,    /* ITEMMDL_FARORES_COURAGE */
    MDL_SHIELD_MAX,    /* ITEMMDL_NAYRUS_WISDOM */
    MDL_SHIELD_MAX,    /* ITEMMDL_DINS_POWER */
    MDL_SHIELD_MAX,    /* ITEMMDL_FARON_SOTH */
    MDL_SHIELD_MAX,    /* ITEMMDL_ELDIN_SOTH */
    MDL_SHIELD_MAX,    /* ITEMMDL_LANAYRU_SOTH */
    MDL_SHIELD_MAX,    /* ITEMMDL_ITEM_SOTH */
    MDL_SHIELD_MAX,    /* ITEMMDL_REVITALIZING_POTION_PLUS_PLUS */
    MDL_SHIELD_MAX,    /* ITEMMDL_HOT_SOUP */
    MDL_SHIELD_MAX,    /* ITEMMDL_COLD_SOUP */
    MDL_SHIELD_MAX,    /* ITEMMDL_LIFE_TREE_SEED */
    MDL_SHIELD_MAX,    /* ITEMMDL_LIFE_TREE_FRUIT */
    MDL_SHIELD_MAX,    /* ITEMMDL_EXTRA_WALLET */
};

const dItemMdlShield_c::MdlConfig dItemMdlShield_c::sMdlConfig[] = {
    {0.0f, "GetShieldWood", "GetShieldWood", "EquipShieldWood"},
    {1.0f, "GetShieldWood", "GetShieldWood", "EquipShieldWood"},
    {2.0f, "GetShieldWood", "GetShieldWood", "EquipShieldWood"},
    {0.0f, "GetShieldIron", "GetShieldIron", "EquipShieldIron"},
    {1.0f, "GetShieldIron", "GetShieldIron", "EquipShieldIron"},
    {2.0f, "GetShieldIron", "GetShieldIron", "EquipShieldIron"},
    {0.0f, "GetShieldHoly", "GetShieldHoly", "EquipShieldHoly"},
    {1.0f, "GetShieldHoly", "GetShieldHoly", "EquipShieldHoly"},
    {2.0f, "GetShieldHoly", "GetShieldHoly", "EquipShieldHoly"},
};

bool dItemMdlShield_c::isMdlForItemId(u16 itemId) {
    if (hasItemMdl(itemId) == false) {
        return false;
    } else {
        return sMdlIdx[getItemMdlIdx(itemId)] <= MDL_SHIELD_MAX - 1;
    }
}

bool dItemMdlShield_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    s32 mdlIdx = getItemMdlIdx(itemId);
    u8 mdlConfigIdx = sMdlIdx[mdlIdx];

    const MdlConfig *fileAndMdl = &sMdlConfig[mdlConfigIdx];

    void *resData0 = OarcManager::GetInstance()->getMdlFromArc2(fileAndMdl->resFile);
    if (resData0 == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile res0(resData0);
    if (!res0.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl0(res0.GetResMdl(fileAndMdl->mdlName));
    if (!mdl0.IsValid()) {
        return false;
    }

    if (!mMdl.create(mdl0, allocator, 0x123)) {
        return false;
    }

    nw4r::g3d::ResAnmTexPat pat0(res0.GetResAnmTexPat(fileAndMdl->anmName));
    if (!pat0.IsValid()) {
        return false;
    }

    if (!mAnmTexPat.create(mdl0, pat0, allocator, NULL, 1)) {
        return false;
    }

    mMdl.setAnm(mAnmTexPat);
    mAnmTexPat.setFrame(fileAndMdl->frame, 0);

    mpItem = item;
    return true;
}

void dItemMdlShield_c::setDrawMode(u8 arg) {
    mDrawMode = arg;
}

void dItemMdlShield_c::setScale(const mVec3_c &scale) {
    mMdl.setScale(scale);
}

void dItemMdlShield_c::setLocalMtx(const mMtx_c &mtx) {
    mMdl.setLocalMtx(mtx);
}

void dItemMdlShield_c::draw() {
    if (mDrawMode == DrawMode_Get) {
        mpItem->fn_8002ECD0(&mMdl, 6);
    } else {
        mpItem->fn_8002ECD0(&mMdl, 7);
    }
}

void dItemMdlShield_c::changeItemId(u16 itemId) {
    // no-op
}

void dItemMdlShield_c::setPriorityDraw() {
    mMdl.setPriorityDraw(0x82, 0x7F);
}

void dItemMdlShield_c::unsetPriorityDraw() {
    mMdl.setPriorityDraw(0x7F, 0x7F);
}

enum BottleIdx {
    MDL_BOTTLE_WATER,
    MDL_BOTTLE_FAIRY,
    MDL_BOTTLE_HOLY,
    MDL_BOTTLE_KUSURI,
    MDL_BOTTLE_KUSURI_S,
    MDL_BOTTLE_B_FAIRY,
    MDL_BOTTLE_SOZAI_I,
    MDL_BOTTLE_PUMPKIN,
    MDL_BOTTLE_MAX
};

const u8 dItemMdlBottle_c::sMdlIdx[] = {
    MDL_BOTTLE_MAX,      /* ITEMMDL_SMALL_KEY */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GREEN_RUPEE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BLUE_RUPEE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_RED_RUPEE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_COMPLETE_TRIFORCE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_HEART */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SINGLE_ARROW */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BUNDLE_ARROWS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GODDESS_WHITESWORD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_PRACTICE_SWORD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GODDESS_SWORD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GODDESS_LONGSWORD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MASTER_SWORD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_TRUE_MASTER_SWORD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SAILCLOTH */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GODDESS_HARP */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SPIRIT_VESSEL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BOW */
    MDL_BOTTLE_MAX,      /* ITEMMDL_CLAWSHOTS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BIRD_STATUETTE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_AC_BK */
    MDL_BOTTLE_MAX,      /* ITEMMDL_FS_BK */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SSH_BK */
    MDL_BOTTLE_MAX,      /* ITEMMDL_KEY_PIECE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SV_BK */
    MDL_BOTTLE_MAX,      /* ITEMMDL_ET_BK */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LMF_BK */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SILVER_RUPEE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GOLD_RUPEE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_RUPOOR */
    MDL_BOTTLE_MAX,      /* ITEMMDL_5_CRYSTALS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GLITTERING_SPORES */
    MDL_BOTTLE_MAX,      /* ITEMMDL_5_BOMBS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_10_BOMBS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_STAMINA_FRUIT */
    MDL_BOTTLE_MAX,      /* ITEMMDL_FARORE_TEAR */
    MDL_BOTTLE_MAX,      /* ITEMMDL_DIN_TEAR */
    MDL_BOTTLE_MAX,      /* ITEMMDL_NAYRU_TEAR */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SACRED_TEAR */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LIGHT_FRUIT */
    MDL_BOTTLE_MAX,      /* ITEMMDL_1_CRYSTAL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GUST_BELLOWS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_DUNGEON_MAP_FI */
    MDL_BOTTLE_MAX,      /* ITEMMDL_DUNGEON_MAP */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SLINGSHOT */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BEETLE */
    MDL_BOTTLE_WATER,    /* ITEMMDL_WATER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MUSHROOM_SPORES */
    MDL_BOTTLE_MAX,      /* ITEMMDL_DIGGING_MITTS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_5_DEKU_SEEDS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_10_DEKU_SEEDS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GUARDIAN_POTION */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GUARDIAN_POTION_PLUS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_WATER_SCALE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BUG_MEDAL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BUG_NET */
    MDL_BOTTLE_FAIRY,    /* ITEMMDL_FAIRY */
    MDL_BOTTLE_HOLY,     /* ITEMMDL_SACRED_WATER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_HOOK_BEETLE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_QUICK_BEETLE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_TOUGH_BEETLE */
    MDL_BOTTLE_KUSURI,   /* ITEMMDL_HEART_POTION */
    MDL_BOTTLE_KUSURI,   /* ITEMMDL_HEART_POTION_PLUS */
    MDL_BOTTLE_KUSURI_S, /* ITEMMDL_HEART_POTION_ALSO_PLUS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_STAMINA_POTION */
    MDL_BOTTLE_MAX,      /* ITEMMDL_STAMINA_POTION_PLUS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_AIR_POTION */
    MDL_BOTTLE_MAX,      /* ITEMMDL_AIR_POTION_PLUS */
    MDL_BOTTLE_B_FAIRY,  /* ITEMMDL_BOTTLED_FAIRY */
    MDL_BOTTLE_MAX,      /* ITEMMDL_IRON_BOW */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SACRED_BOW */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BOMB_BAG */
    MDL_BOTTLE_MAX,      /* ITEMMDL_HEART_CONTAINER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_HEART_PIECE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_TRI_COURAGE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_TRI_POWER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_TRI_WISDOWM */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SEA_CHART */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MOGMA_MITTS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_HEART_MEDAL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_RUPEE_MEDAL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_TREASURE_MEDAL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_POTION_MEDAL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_CURSED_MEDAL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MIGHTY_SCATTERSHOT */
    MDL_BOTTLE_MAX,      /* ITEMMDL_85 */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MEDIUM_WALLET */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BIG_WALLET */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GIANT_WALLET */
    MDL_BOTTLE_MAX,      /* ITEMMDL_TYCOON_WALLET */
    MDL_BOTTLE_MAX,      /* ITEMMDL_ADVENTURE_POUCH */
    MDL_BOTTLE_MAX,      /* ITEMMDL_POUCH_EXPANSION */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LIFE_MEDAL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_WOODEN_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BANDED_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BRACED_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_IRON_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_REINFORCED_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_FORTIFIED_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SACRED_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_DIVINE_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GODDESS_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_HYLIAN_SHIELD */
    MDL_BOTTLE_MAX,      /* ITEMMDL_REVITALIZING_POTION */
    MDL_BOTTLE_MAX,      /* ITEMMDL_REVITALIZING_POTION_PLUS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SMALL_SEED_SATCHEL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MEDIUM_SEED_SATCHEL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LARGE_SEED_SATCHEL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SMALL_QUIVER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MEDIUM_QUIVER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LARGE_QUIVER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SMALL_BOMB_BAG */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MEDIUM_BOMB_BAG */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LARGE_BOMB_BAG */
    MDL_BOTTLE_MAX,      /* ITEMMDL_WHIP */
    MDL_BOTTLE_MAX,      /* ITEMMDL_FIRESHIELD_EARRINGS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BIG_BUG_NET */
    MDL_BOTTLE_MAX,      /* ITEMMDL_FARON_GRASSHOPPER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_WOODLAND_RHINO_BEETLE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_DEKU_HORNET */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SKYLOFT_MANTIS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_VOLCANIC_LADYBUG */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BLESSED_BUTTERFLY */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LANAYRU_ANT */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SAND_CICADA */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GERUDO_DRAGONFLY */
    MDL_BOTTLE_MAX,      /* ITEMMDL_ELDIN_ROLLER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_SKY_STAG_BEETLE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_STARRY_FIREFLY */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BOTTLE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_CAWLIN_LETTER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BEEDLE_INSECT_CAGE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_RATTLE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_HORNET_LARVAE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BIRD_FEATHER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_TUMBLE_WEED */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LIZARD_TAIL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_ELDIN_ORE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_ANCIENT_FLOWER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_AMBER_RELIC */
    MDL_BOTTLE_MAX,      /* ITEMMDL_DUSK_RELIC */
    MDL_BOTTLE_SOZAI_I,  /* ITEMMDL_JELLY_BLOB */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MONSTER_CLAW */
    MDL_BOTTLE_MAX,      /* ITEMMDL_MONSTER_HORN */
    MDL_BOTTLE_MAX,      /* ITEMMDL_ORNAMENTAL_SKULL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_EVIL_CRYSTAL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BLUE_BIRD_FEATHER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GOLDEN_SKULL */
    MDL_BOTTLE_MAX,      /* ITEMMDL_GODDESS_PLUME */
    MDL_BOTTLE_MAX,      /* ITEMMDL_EMERALD_TABLET */
    MDL_BOTTLE_MAX,      /* ITEMMDL_RUBY_TABLET */
    MDL_BOTTLE_MAX,      /* ITEMMDL_AMBER_TABLET */
    MDL_BOTTLE_MAX,      /* ITEMMDL_STONE_OF_TRIALS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_BALLAD_OF_THE_GODDESS */
    MDL_BOTTLE_MAX,      /* ITEMMDL_FARORES_COURAGE */
    MDL_BOTTLE_MAX,      /* ITEMMDL_NAYRUS_WISDOM */
    MDL_BOTTLE_MAX,      /* ITEMMDL_DINS_POWER */
    MDL_BOTTLE_MAX,      /* ITEMMDL_FARON_SOTH */
    MDL_BOTTLE_MAX,      /* ITEMMDL_ELDIN_SOTH */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LANAYRU_SOTH */
    MDL_BOTTLE_MAX,      /* ITEMMDL_ITEM_SOTH */
    MDL_BOTTLE_MAX,      /* ITEMMDL_REVITALIZING_POTION_PLUS_PLUS */
    MDL_BOTTLE_PUMPKIN,  /* ITEMMDL_HOT_SOUP */
    MDL_BOTTLE_PUMPKIN,  /* ITEMMDL_COLD_SOUP */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LIFE_TREE_SEED */
    MDL_BOTTLE_MAX,      /* ITEMMDL_LIFE_TREE_FRUIT */
    MDL_BOTTLE_MAX,      /* ITEMMDL_EXTRA_WALLET */
};

const dItemMdlBottle_c::MdlConfig dItemMdlBottle_c::sMdlConfig[] = {
    {  "GetBottleWater",   "GetBottleWater",   "GetBottleWater"},
    {        "GetFairy",         "GetFairy",         "GetFairy"},
    {   "GetBottleHoly",    "GetBottleHoly",    "GetBottleHoly"},
    { "GetBottleKusuri",  "GetBottleKusuri",  "GetBottleKusuri"},
    {"GetBottleKusuriS", "GetBottleKusuriS", "GetBottleKusuriS"},
    {  "GetBottleFairy",   "GetBottleFairy",   "GetBottleFairy"},
    {       "GetSozaiI",        "GetSozaiI",        "GetSozaiI"},
    {"GetBottlePumpkin", "GetBottlePumpkin", "GetBottlePumpkin"},
};

bool dItemMdlBottle_c::isMdlForItemId(u16 itemId) {
    if (hasItemMdl(itemId) == false) {
        return false;
    } else {
        return sMdlIdx[getItemMdlIdx(itemId)] <= MDL_BOTTLE_MAX - 1;
    }
}

bool dItemMdlBottle_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    s32 mdlIdx = getItemMdlIdx(itemId);
    u8 mdlConfigIdx = sMdlIdx[mdlIdx];

    const MdlConfig *fileAndMdl = &sMdlConfig[mdlConfigIdx];

    void *resData0 = OarcManager::GetInstance()->getMdlFromArc2(fileAndMdl->resFile);
    if (resData0 == nullptr) {
        return false;
    }

    if (!mMdl.create(resData0, fileAndMdl->mdlName, fileAndMdl->anmName, allocator, 0x120)) {
        return false;
    }

    if (!mMdl.setAnm(fileAndMdl->anmName, m3d::PLAY_MODE_4)) {
        return false;
    }

    mpItem = item;
    if (dAcItem_c::isJellyBlob((ITEM_ID)itemId)) {
        mFlags |= 0x1;
    }

    return true;
}

void dItemMdlBottle_c::setDrawMode(u8 arg) {
    mDrawMode = arg;
}

void dItemMdlBottle_c::setScale(const mVec3_c &scale) {
    mMdl.getModel().setScale(scale);
}

void dItemMdlBottle_c::setLocalMtx(const mMtx_c &mtx) {
    mMdl.getModel().setLocalMtx(mtx);
}

void dItemMdlBottle_c::draw() {
    if (mpItem == nullptr) {
        return;
    }

    if (mDrawMode != DrawMode_Put || (mFlags & 0x1) == 0) {
        mMdl.play();
    }

    if (mDrawMode == DrawMode_Get) {
        mpItem->fn_8002ECD0(&mMdl.getModel(), 6);
    } else {
        mpItem->fn_8002ECD0(&mMdl.getModel(), 7);
    }
}

void dItemMdlBottle_c::changeItemId(u16 itemId) {
    // no-op
}

void dItemMdlBottle_c::setPriorityDraw() {
    mMdl.getModel().setPriorityDraw(0x82, 0x7F);
}

void dItemMdlBottle_c::unsetPriorityDraw() {
    mMdl.getModel().setPriorityDraw(0x7F, 0x7F);
}

const dItemMdlTear_c::MdlConfig dItemMdlTear_c::sMdlConfig[] = {
    {0.0f},
    {1.0f},
    {2.0f},
    {3.0f},
};

bool dItemMdlTear_c::isMdlForItemId(u16 itemId) {
    return dAcItem_c::isTear((ITEM_ID)itemId);
}

bool dItemMdlTear_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    // TODO - why is TRY_CREATE used so inconsistently here?

    dAcItem_c::Tear_e tear = dAcItem_c::getTearSubtype((ITEM_ID)itemId);
    if (tear == dAcItem_c::TEAR_MAX) {
        return false;
    }

    const MdlConfig *fileAndMdl = &sMdlConfig[tear];
    f32 frame = fileAndMdl->frame;

    {
        void *resData1 = OarcManager::GetInstance()->getMdlFromArc2("PutSizuku");
        if (resData1 == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile res1(resData1);
        if (!res1.IsValid()) {
            return false;
        }

        nw4r::g3d::ResMdl mdl1 = res1.GetResMdl("PutShizuku");
        if (!mdl1.IsValid()) {
            return false;
        }
        if (!mPutMdl.create(mdl1, allocator, 0x327)) {
            return false;
        }

        nw4r::g3d::ResAnmTexSrt anmTexSrt1 = res1.GetResAnmTexSrt("Shizuku");
        if (!anmTexSrt1.IsValid()) {
            return false;
        }
        TRY_CREATE(mPutAnmTexSrt.create(mdl1, anmTexSrt1, allocator, nullptr, 1));
        TRY_CREATE(mPutMdl.setAnm(mPutAnmTexSrt));

        nw4r::g3d::ResAnmTexPat anmTexPat1 = res1.GetResAnmTexPat("Shizuku");
        if (!anmTexPat1.IsValid()) {
            return false;
        }
        if (!mPutAnmTexPat.create(mdl1, anmTexPat1, allocator, nullptr, 1)) {
            return false;
        }
        TRY_CREATE(mPutMdl.setAnm(mPutAnmTexPat));
        mPutAnmTexPat.setFrame(frame, 0);
    }

    {
        void *resData2 = OarcManager::GetInstance()->getMdlFromArc2("GetSizuku");
        if (resData2 == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile res2(resData2);
        if (!res2.IsValid()) {
            return false;
        }

        nw4r::g3d::ResMdl mdl2 = res2.GetResMdl("GetShizuku");
        if (!mdl2.IsValid()) {
            return false;
        }
        if (!mGetMdl.create(mdl2, allocator, 0x327)) {
            return false;
        }

        nw4r::g3d::ResAnmTexSrt anmTexSrt2 = res2.GetResAnmTexSrt("Shizuku");
        if (!anmTexSrt2.IsValid()) {
            return false;
        }
        TRY_CREATE(mGetAnmTexSrt.create(mdl2, anmTexSrt2, allocator, nullptr, 1));
        TRY_CREATE(mGetMdl.setAnm(mGetAnmTexSrt));

        nw4r::g3d::ResAnmTexPat anmTexPat2 = res2.GetResAnmTexPat("Shizuku");
        if (!anmTexPat2.IsValid()) {
            return false;
        }
        if (!mGetAnmTexPat.create(mdl2, anmTexPat2, allocator, nullptr, 1)) {
            return false;
        }
        TRY_CREATE(mGetMdl.setAnm(mGetAnmTexPat));
        mGetAnmTexPat.setFrame(frame, 0);
    }

    mpItem = item;

    return true;
}

void dItemMdlTear_c::setDrawMode(u8 arg) {
    if (arg == DrawMode_Max) {
        mDrawMode = DrawMode_Put;
    } else {
        mDrawMode = arg;
    }
}

void dItemMdlTear_c::setScale(const mVec3_c &scale) {
    mPutMdl.setScale(scale);
    mGetMdl.setScale(scale);
}

void dItemMdlTear_c::setLocalMtx(const mMtx_c &mtx) {
    mPutMdl.setLocalMtx(mtx);
    mGetMdl.setLocalMtx(mtx);
}

void dItemMdlTear_c::draw() {
    switch (mDrawMode) {
        case DrawMode_Put:
            mPutAnmTexSrt.play();
            mpItem->fn_8002ECD0(&mPutMdl, 7);
            break;
        case DrawMode_Get:
            mGetAnmTexSrt.play();
            mpItem->fn_8002ECD0(&mGetMdl, 6);
            break;
    }
}

void dItemMdlTear_c::changeItemId(u16 itemId) {
    // no-op
}

void dItemMdlTear_c::setPriorityDraw() {
    mPutMdl.setPriorityDraw(0x82, 0x7F);
    mGetMdl.setPriorityDraw(0x82, 0x7F);
}

void dItemMdlTear_c::unsetPriorityDraw() {
    mPutMdl.setPriorityDraw(0x7F, 0x7F);
    mGetMdl.setPriorityDraw(0x7F, 0x7F);
}

bool dItemMdlFirefly_c::isMdlForItemId(u16 itemId) {
    return dAcItem_c::isStarryFirefly((ITEM_ID)itemId);
}

bool dItemMdlFirefly_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    void *resData = OarcManager::GetInstance()->getMdlFromArc2("GetInsectFirefly");
    if (resData == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile res(resData);
    if (!res.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl = res.GetResMdl("GetInsectFirefly");
    if (!mdl.IsValid()) {
        return false;
    }
    if (!mMdl.create(mdl, allocator, 0x128)) {
        return false;
    }

    nw4r::g3d::ResAnmClr anmClr = res.GetResAnmClr("GetInsectFirefly");
    if (!anmClr.IsValid()) {
        return false;
    }
    TRY_CREATE(mAnmMatClr.create(mdl, anmClr, allocator, nullptr, 1));
    mAnmMatClr.setFrameOnly(0.0f);
    TRY_CREATE(mMdl.setAnm(mAnmMatClr));

    mpItem = item;
    return true;
}

void dItemMdlFirefly_c::setDrawMode(u8 arg) {
    mDrawMode = arg;
}

void dItemMdlFirefly_c::setScale(const mVec3_c &scale) {
    mMdl.setScale(scale);
}

void dItemMdlFirefly_c::setLocalMtx(const mMtx_c &mtx) {
    mMdl.setLocalMtx(mtx);
}

void dItemMdlFirefly_c::draw() {
    switch (mDrawMode) {
        case DrawMode_Put: mpItem->fn_8002ECD0(&mMdl, 7); break;
        case DrawMode_Get: mpItem->fn_8002ECD0(&mMdl, 6); break;
    }
    mAnmMatClr.play();
}

void dItemMdlFirefly_c::changeItemId(u16 itemId) {
    // no-op
}

void dItemMdlFirefly_c::setPriorityDraw() {
    mMdl.setPriorityDraw(0x82, 0x7F);
}

void dItemMdlFirefly_c::unsetPriorityDraw() {
    mMdl.setPriorityDraw(0x7F, 0x7F);
}

m3d::smdl_c *dItemMdlFirefly_c::getMdl() {
    return &mMdl;
}

enum PotionIdx {
    MDL_POTION_MUTEKI,
    MDL_POTION_GUTS,
    MDL_POTION_AIR,
    MDL_POTION_GENKI,
    MDL_POTION_MAX,
};

const u8 dItemMdlPotion_c::sMdlIdx[] = {
    MDL_POTION_MAX,    /* ITEMMDL_SMALL_KEY */
    MDL_POTION_MAX,    /* ITEMMDL_GREEN_RUPEE */
    MDL_POTION_MAX,    /* ITEMMDL_BLUE_RUPEE */
    MDL_POTION_MAX,    /* ITEMMDL_RED_RUPEE */
    MDL_POTION_MAX,    /* ITEMMDL_COMPLETE_TRIFORCE */
    MDL_POTION_MAX,    /* ITEMMDL_HEART */
    MDL_POTION_MAX,    /* ITEMMDL_SINGLE_ARROW */
    MDL_POTION_MAX,    /* ITEMMDL_BUNDLE_ARROWS */
    MDL_POTION_MAX,    /* ITEMMDL_GODDESS_WHITESWORD */
    MDL_POTION_MAX,    /* ITEMMDL_PRACTICE_SWORD */
    MDL_POTION_MAX,    /* ITEMMDL_GODDESS_SWORD */
    MDL_POTION_MAX,    /* ITEMMDL_GODDESS_LONGSWORD */
    MDL_POTION_MAX,    /* ITEMMDL_MASTER_SWORD */
    MDL_POTION_MAX,    /* ITEMMDL_TRUE_MASTER_SWORD */
    MDL_POTION_MAX,    /* ITEMMDL_SAILCLOTH */
    MDL_POTION_MAX,    /* ITEMMDL_GODDESS_HARP */
    MDL_POTION_MAX,    /* ITEMMDL_SPIRIT_VESSEL */
    MDL_POTION_MAX,    /* ITEMMDL_BOW */
    MDL_POTION_MAX,    /* ITEMMDL_CLAWSHOTS */
    MDL_POTION_MAX,    /* ITEMMDL_BIRD_STATUETTE */
    MDL_POTION_MAX,    /* ITEMMDL_AC_BK */
    MDL_POTION_MAX,    /* ITEMMDL_FS_BK */
    MDL_POTION_MAX,    /* ITEMMDL_SSH_BK */
    MDL_POTION_MAX,    /* ITEMMDL_KEY_PIECE */
    MDL_POTION_MAX,    /* ITEMMDL_SV_BK */
    MDL_POTION_MAX,    /* ITEMMDL_ET_BK */
    MDL_POTION_MAX,    /* ITEMMDL_LMF_BK */
    MDL_POTION_MAX,    /* ITEMMDL_SILVER_RUPEE */
    MDL_POTION_MAX,    /* ITEMMDL_GOLD_RUPEE */
    MDL_POTION_MAX,    /* ITEMMDL_RUPOOR */
    MDL_POTION_GENKI,  /* ITEMMDL_5_CRYSTALS */
    MDL_POTION_MAX,    /* ITEMMDL_GLITTERING_SPORES */
    MDL_POTION_MAX,    /* ITEMMDL_5_BOMBS */
    MDL_POTION_MAX,    /* ITEMMDL_10_BOMBS */
    MDL_POTION_MAX,    /* ITEMMDL_STAMINA_FRUIT */
    MDL_POTION_MAX,    /* ITEMMDL_FARORE_TEAR */
    MDL_POTION_MAX,    /* ITEMMDL_DIN_TEAR */
    MDL_POTION_MAX,    /* ITEMMDL_NAYRU_TEAR */
    MDL_POTION_MAX,    /* ITEMMDL_SACRED_TEAR */
    MDL_POTION_MAX,    /* ITEMMDL_LIGHT_FRUIT */
    MDL_POTION_GENKI,  /* ITEMMDL_1_CRYSTAL */
    MDL_POTION_MAX,    /* ITEMMDL_GUST_BELLOWS */
    MDL_POTION_MAX,    /* ITEMMDL_DUNGEON_MAP_FI */
    MDL_POTION_MAX,    /* ITEMMDL_DUNGEON_MAP */
    MDL_POTION_MAX,    /* ITEMMDL_SLINGSHOT */
    MDL_POTION_MAX,    /* ITEMMDL_BEETLE */
    MDL_POTION_MAX,    /* ITEMMDL_WATER */
    MDL_POTION_MAX,    /* ITEMMDL_MUSHROOM_SPORES */
    MDL_POTION_MAX,    /* ITEMMDL_DIGGING_MITTS */
    MDL_POTION_MAX,    /* ITEMMDL_5_DEKU_SEEDS */
    MDL_POTION_MAX,    /* ITEMMDL_10_DEKU_SEEDS */
    MDL_POTION_MUTEKI, /* ITEMMDL_GUARDIAN_POTION */
    MDL_POTION_MUTEKI, /* ITEMMDL_GUARDIAN_POTION_PLUS */
    MDL_POTION_MAX,    /* ITEMMDL_WATER_SCALE */
    MDL_POTION_MAX,    /* ITEMMDL_BUG_MEDAL */
    MDL_POTION_MAX,    /* ITEMMDL_BUG_NET */
    MDL_POTION_MAX,    /* ITEMMDL_FAIRY */
    MDL_POTION_MAX,    /* ITEMMDL_SACRED_WATER */
    MDL_POTION_MAX,    /* ITEMMDL_HOOK_BEETLE */
    MDL_POTION_MAX,    /* ITEMMDL_QUICK_BEETLE */
    MDL_POTION_MAX,    /* ITEMMDL_TOUGH_BEETLE */
    MDL_POTION_MAX,    /* ITEMMDL_HEART_POTION */
    MDL_POTION_MAX,    /* ITEMMDL_HEART_POTION_PLUS */
    MDL_POTION_MAX,    /* ITEMMDL_HEART_POTION_ALSO_PLUS */
    MDL_POTION_GUTS,   /* ITEMMDL_STAMINA_POTION */
    MDL_POTION_GUTS,   /* ITEMMDL_STAMINA_POTION_PLUS */
    MDL_POTION_AIR,    /* ITEMMDL_AIR_POTION */
    MDL_POTION_AIR,    /* ITEMMDL_AIR_POTION_PLUS */
    MDL_POTION_MAX,    /* ITEMMDL_BOTTLED_FAIRY */
    MDL_POTION_MAX,    /* ITEMMDL_IRON_BOW */
    MDL_POTION_MAX,    /* ITEMMDL_SACRED_BOW */
    MDL_POTION_MAX,    /* ITEMMDL_BOMB_BAG */
    MDL_POTION_MAX,    /* ITEMMDL_HEART_CONTAINER */
    MDL_POTION_MAX,    /* ITEMMDL_HEART_PIECE */
    MDL_POTION_MAX,    /* ITEMMDL_TRI_COURAGE */
    MDL_POTION_MAX,    /* ITEMMDL_TRI_POWER */
    MDL_POTION_MAX,    /* ITEMMDL_TRI_WISDOWM */
    MDL_POTION_MAX,    /* ITEMMDL_SEA_CHART */
    MDL_POTION_MAX,    /* ITEMMDL_MOGMA_MITTS */
    MDL_POTION_MAX,    /* ITEMMDL_HEART_MEDAL */
    MDL_POTION_MAX,    /* ITEMMDL_RUPEE_MEDAL */
    MDL_POTION_MAX,    /* ITEMMDL_TREASURE_MEDAL */
    MDL_POTION_MAX,    /* ITEMMDL_POTION_MEDAL */
    MDL_POTION_MAX,    /* ITEMMDL_CURSED_MEDAL */
    MDL_POTION_MAX,    /* ITEMMDL_MIGHTY_SCATTERSHOT */
    MDL_POTION_MAX,    /* ITEMMDL_85 */
    MDL_POTION_MAX,    /* ITEMMDL_MEDIUM_WALLET */
    MDL_POTION_MAX,    /* ITEMMDL_BIG_WALLET */
    MDL_POTION_MAX,    /* ITEMMDL_GIANT_WALLET */
    MDL_POTION_MAX,    /* ITEMMDL_TYCOON_WALLET */
    MDL_POTION_MAX,    /* ITEMMDL_ADVENTURE_POUCH */
    MDL_POTION_MAX,    /* ITEMMDL_POUCH_EXPANSION */
    MDL_POTION_MAX,    /* ITEMMDL_LIFE_MEDAL */
    MDL_POTION_MAX,    /* ITEMMDL_WOODEN_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_BANDED_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_BRACED_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_IRON_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_REINFORCED_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_FORTIFIED_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_SACRED_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_DIVINE_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_GODDESS_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_HYLIAN_SHIELD */
    MDL_POTION_MAX,    /* ITEMMDL_REVITALIZING_POTION */
    MDL_POTION_MAX,    /* ITEMMDL_REVITALIZING_POTION_PLUS */
    MDL_POTION_MAX,    /* ITEMMDL_SMALL_SEED_SATCHEL */
    MDL_POTION_MAX,    /* ITEMMDL_MEDIUM_SEED_SATCHEL */
    MDL_POTION_MAX,    /* ITEMMDL_LARGE_SEED_SATCHEL */
    MDL_POTION_MAX,    /* ITEMMDL_SMALL_QUIVER */
    MDL_POTION_MAX,    /* ITEMMDL_MEDIUM_QUIVER */
    MDL_POTION_MAX,    /* ITEMMDL_LARGE_QUIVER */
    MDL_POTION_MAX,    /* ITEMMDL_SMALL_BOMB_BAG */
    MDL_POTION_MAX,    /* ITEMMDL_MEDIUM_BOMB_BAG */
    MDL_POTION_MAX,    /* ITEMMDL_LARGE_BOMB_BAG */
    MDL_POTION_MAX,    /* ITEMMDL_WHIP */
    MDL_POTION_MAX,    /* ITEMMDL_FIRESHIELD_EARRINGS */
    MDL_POTION_MAX,    /* ITEMMDL_BIG_BUG_NET */
    MDL_POTION_MAX,    /* ITEMMDL_FARON_GRASSHOPPER */
    MDL_POTION_MAX,    /* ITEMMDL_WOODLAND_RHINO_BEETLE */
    MDL_POTION_MAX,    /* ITEMMDL_DEKU_HORNET */
    MDL_POTION_MAX,    /* ITEMMDL_SKYLOFT_MANTIS */
    MDL_POTION_MAX,    /* ITEMMDL_VOLCANIC_LADYBUG */
    MDL_POTION_MAX,    /* ITEMMDL_BLESSED_BUTTERFLY */
    MDL_POTION_MAX,    /* ITEMMDL_LANAYRU_ANT */
    MDL_POTION_MAX,    /* ITEMMDL_SAND_CICADA */
    MDL_POTION_MAX,    /* ITEMMDL_GERUDO_DRAGONFLY */
    MDL_POTION_MAX,    /* ITEMMDL_ELDIN_ROLLER */
    MDL_POTION_MAX,    /* ITEMMDL_SKY_STAG_BEETLE */
    MDL_POTION_MAX,    /* ITEMMDL_STARRY_FIREFLY */
    MDL_POTION_MAX,    /* ITEMMDL_BOTTLE */
    MDL_POTION_MAX,    /* ITEMMDL_CAWLIN_LETTER */
    MDL_POTION_MAX,    /* ITEMMDL_BEEDLE_INSECT_CAGE */
    MDL_POTION_MAX,    /* ITEMMDL_RATTLE */
    MDL_POTION_MAX,    /* ITEMMDL_HORNET_LARVAE */
    MDL_POTION_MAX,    /* ITEMMDL_BIRD_FEATHER */
    MDL_POTION_MAX,    /* ITEMMDL_TUMBLE_WEED */
    MDL_POTION_MAX,    /* ITEMMDL_LIZARD_TAIL */
    MDL_POTION_MAX,    /* ITEMMDL_ELDIN_ORE */
    MDL_POTION_MAX,    /* ITEMMDL_ANCIENT_FLOWER */
    MDL_POTION_MAX,    /* ITEMMDL_AMBER_RELIC */
    MDL_POTION_MAX,    /* ITEMMDL_DUSK_RELIC */
    MDL_POTION_MAX,    /* ITEMMDL_JELLY_BLOB */
    MDL_POTION_MAX,    /* ITEMMDL_MONSTER_CLAW */
    MDL_POTION_MAX,    /* ITEMMDL_MONSTER_HORN */
    MDL_POTION_MAX,    /* ITEMMDL_ORNAMENTAL_SKULL */
    MDL_POTION_MAX,    /* ITEMMDL_EVIL_CRYSTAL */
    MDL_POTION_MAX,    /* ITEMMDL_BLUE_BIRD_FEATHER */
    MDL_POTION_MAX,    /* ITEMMDL_GOLDEN_SKULL */
    MDL_POTION_MAX,    /* ITEMMDL_GODDESS_PLUME */
    MDL_POTION_MAX,    /* ITEMMDL_EMERALD_TABLET */
    MDL_POTION_MAX,    /* ITEMMDL_RUBY_TABLET */
    MDL_POTION_MAX,    /* ITEMMDL_AMBER_TABLET */
    MDL_POTION_MAX,    /* ITEMMDL_STONE_OF_TRIALS */
    MDL_POTION_MAX,    /* ITEMMDL_BALLAD_OF_THE_GODDESS */
    MDL_POTION_MAX,    /* ITEMMDL_FARORES_COURAGE */
    MDL_POTION_MAX,    /* ITEMMDL_NAYRUS_WISDOM */
    MDL_POTION_MAX,    /* ITEMMDL_DINS_POWER */
    MDL_POTION_MAX,    /* ITEMMDL_FARON_SOTH */
    MDL_POTION_MAX,    /* ITEMMDL_ELDIN_SOTH */
    MDL_POTION_MAX,    /* ITEMMDL_LANAYRU_SOTH */
    MDL_POTION_MAX,    /* ITEMMDL_ITEM_SOTH */
    MDL_POTION_MAX,    /* ITEMMDL_REVITALIZING_POTION_PLUS_PLUS */
    MDL_POTION_MAX,    /* ITEMMDL_HOT_SOUP */
    MDL_POTION_MAX,    /* ITEMMDL_COLD_SOUP */
    MDL_POTION_MAX,    /* ITEMMDL_LIFE_TREE_SEED */
    MDL_POTION_MAX,    /* ITEMMDL_LIFE_TREE_FRUIT */
    MDL_POTION_MAX,    /* ITEMMDL_EXTRA_WALLET */
};

const dItemMdlPotion_c::MdlConfig dItemMdlPotion_c::sMdlConfig[] = {
    {"GetBottleMuteki", "GetBottleMuteki", "GetBottleMuteki", "GetBottleMuteki"},
    {  "GetBottleGuts",   "GetBottleGuts",   "GetBottleGuts",   "GetBottleGuts"},
    {   "GetBottleAir",    "GetBottleAir",    "GetBottleAir",    "GetBottleAir"},
    {       "GetGenki",        "GetGenki",        "GetGenki",        "GetGenki"},
};

bool dItemMdlPotion_c::isMdlForItemId(u16 itemId) {
    if (hasItemMdl(itemId) == false) {
        return false;
    } else {
        return sMdlIdx[getItemMdlIdx(itemId)] <= MDL_POTION_MAX - 1;
    }
}

bool dItemMdlPotion_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    s32 mdlIdx = getItemMdlIdx(itemId);
    u8 mdlConfigIdx = sMdlIdx[mdlIdx];

    const MdlConfig *fileAndMdl = &sMdlConfig[mdlConfigIdx];

    void *resData = OarcManager::GetInstance()->getMdlFromArc2(fileAndMdl->resFile);
    if (resData == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile resFile(resData);
    if (!resFile.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl = resFile.GetResMdl(fileAndMdl->mdlName);
    if (!mdl.IsValid()) {
        return false;
    }
    // TODO - what? This makes the function return true if model creation
    // succeeds but the anmChr is invalid.
    bool result = mMdl.create(mdl, allocator, 0x324);
    if (!result) {
        return false;
    }

    nw4r::g3d::ResAnmChr anmChr = resFile.GetResAnmChr(fileAndMdl->anmChrName);
    if (!anmChr.IsValid()) {
        return result;
    }
    TRY_CREATE(mAnmChr.create2(mdl, anmChr, allocator));
    mAnmChr.setAnm(mMdl, anmChr, m3d::PLAY_MODE_0);
    mMdl.setAnm(mAnmChr);

    nw4r::g3d::ResAnmTexSrt anmTexSrt = resFile.GetResAnmTexSrt(fileAndMdl->anmTexSrtName);
    if (!anmTexSrt.IsValid()) {
        return false;
    }
    TRY_CREATE(mAnmTexSrt.create(mdl, anmTexSrt, allocator, nullptr, 1));
    TRY_CREATE(mMdl.setAnm(mAnmTexSrt));

    mpItem = item;
    return true;
}

void dItemMdlPotion_c::setDrawMode(u8 arg) {
    mDrawMode = arg;
}

void dItemMdlPotion_c::setScale(const mVec3_c &scale) {
    mMdl.setScale(scale);
}

void dItemMdlPotion_c::setLocalMtx(const mMtx_c &mtx) {
    mMdl.setLocalMtx(mtx);
}

void dItemMdlPotion_c::draw() {
    mAnmChr.play();
    mAnmTexSrt.play();
    switch (mDrawMode) {
        case DrawMode_Put: mpItem->fn_8002ECD0(&mMdl, 7); break;
        case DrawMode_Get: mpItem->fn_8002ECD0(&mMdl, 6); break;
    }
}

void dItemMdlPotion_c::changeItemId(u16 itemId) {
    // no-op
}

void dItemMdlPotion_c::setPriorityDraw() {
    mMdl.setPriorityDraw(0x82, 0x7F);
}

void dItemMdlPotion_c::unsetPriorityDraw() {
    mMdl.setPriorityDraw(0x7F, 0x7F);
}

m3d::smdl_c *dItemMdlPotion_c::getMdl() {
    return &mMdl;
}
