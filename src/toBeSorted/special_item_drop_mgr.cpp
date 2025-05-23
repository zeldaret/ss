#include "toBeSorted/special_item_drop_mgr.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/d_pouch.h"
#include "d/d_sc_game.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

SpecialItemDropMgr *SpecialItemDropMgr::sInstance = nullptr;

extern "C" void spawnItem1(u32 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem2(u32 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem3(u32 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem4(u32 itemid, u32, u32 param2);
extern "C" void spawnItem5(u32 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem6(u32 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem7(u32 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem8(u32 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, void *, void *, u32 param2, u32 unk);
extern "C" void spawnItem9(u32 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot);

// 800c7b80
SpecialItemDropMgr::SpecialItemDropMgr() {
    SpecialItemDropMgr::sInstance = this;
}

SpecialItemDropMgr *SpecialItemDropMgr::create() {
    return new SpecialItemDropMgr();
}

struct DropEntry {
    /* 0x0 */ u8 mEntryIndex;
    /* 0x1 */ u8 mDropChance;
};

struct DropList {
    /* 0x0 */ u16 mLen;
    /* 0x2 */ u16 mFlags;
    /* 0x4 */ const DropEntry *mpEntries;
};

static const DropEntry e01[] = {
    {0x01, 20},
    {0x03, 40},
    {0x04, 10},
};

static const DropEntry e02[] = {
    {0x01, 20},
    {0x03, 10},
    {0x04, 40},
};

static const DropEntry e03[] = {
    {0x01, 20},
    {0x04, 25},
    {0x05, 25},
};

static const DropEntry e04[] = {
    {0x01, 100},
};

static const DropEntry e05[] = {
    {0x01, 60},
    {0x02, 10},
};

static const DropEntry e06[] = {
    {0x02, 100},
};

static const DropEntry e07[] = {
    {0x03, 40},
    {0x04, 10},
};

static const DropEntry e08[] = {
    {0x03, 50},
    {0x04, 40},
    {0x05, 10},
};

static const DropEntry e09[] = {
    {0x04, 70},
    {0x05, 30},
};

static const DropEntry e10[] = {
    {0x06, 100},
};

static const DropEntry e11[] = {
    {0x08, 100},
};

static const DropEntry e12[] = {
    {0x0A, 100},
};

static const DropEntry e13[] = {
    {0x0B, 100},
};

static const DropEntry e14[] = {
    {0x0C, 100},
};

static const DropEntry e15[] = {
    {0x0D, 100},
};

static const DropEntry e16[] = {
    {0x03, 7},
    {0x04, 3},
};

static const DropEntry e17[] = {
    {0x01, 7},
    {0x03, 3},
};

static const DropEntry e18[] = {
    {0x03, 100},
};

static const DropEntry e19[] = {
    {0x04, 100},
};

static const DropEntry e20[] = {
    {0x05, 100},
};

static const DropEntry e21[] = {
    {0x0E, 100},
};

static const DropEntry e22[] = {
    {0x0F, 100},
};

static const DropEntry e23[] = {
    {0x03, 60},
    {0x04, 30},
    {0x0F, 10},
};

static const DropEntry e24[] = {
    {0x03, 30},
    {0x04, 30},
    {0x05, 20},
    {0x0F, 20},
};

static const DropEntry e25[] = {
    {0x10, 100},
};

static const DropEntry e26[] = {
    {0x12, 100},
};

static const DropEntry e27[] = {
    {0x13, 100},
};

static const DropEntry e28[] = {
    {0x14, 100},
};

static const DropEntry e29[] = {
    {0x10, 10},
};

static const DropEntry e30[] = {
    {0x0B, 100},
};

static const DropEntry e31[] = {
    {0x01, 100},
};

static const DropEntry e32[] = {
    {0x01, 12},
};

static const DropEntry e33[] = {
    {0x01, 35},
};

static const DropEntry e34[] = {
    {0x16, 30},
    {0x15, 10},
    {0x02, 20},
    {0x01, 20},
};

static const DropEntry e35[] = {
    {0x15, 100},
};

static const DropEntry e36[] = {
    {0x16, 100},
};

static const DropEntry e37[] = {
    {0x01, 20},
    {0x03, 30},
    {0x04, 15},
};

static const DropEntry e38[] = {
    {0x04, 30},
    {0x05,  5},
    {0x03, 45},
};

static const DropEntry e39[] = {
    {0x01, 10},
};

static const DropEntry e40[] = {
    {0x03, 10},
    {0x04,  5},
};

static const DropEntry e41[] = {
    {0x17, 100},
};

static const DropEntry e42[] = {
    {0x01, 20},
};

static const DropEntry e44[] = {
    {0x01, 7},
};

static const DropEntry e45[] = {
    {0x03, 14},
    {0x04,  6},
};

static const DropEntry e46[] = {
    {0x01,  7},
    {0x03, 13},
};

static const DropEntry e47[] = {
    {0x18, 100},
};

static const DropEntry e48[] = {
    {0x01, 7},
};

static const DropEntry e49[] = {
    {0x01, 14},
};

static const DropEntry e50[] = {
    {0x01, 50},
};

static const DropEntry e51[] = {
    {0x0B, 20},
    {0x03, 20},
    {0x04, 10},
};

static const DropEntry e52[] = {
    {0x0A, 60},
    {0x01, 20},
    {0x04, 20},
};

static const DropEntry e53[] = {
    {0x08, 20},
    {0x0A, 20},
    {0x0B, 20},
    {0x03, 10},
    {0x04,  5},
};

#define FLAG_EXTRA_HEARTS 1
#define FLAG_EXTRA_RUPEES 2

#define FLAG_EXTRA_ALL (FLAG_EXTRA_HEARTS | FLAG_EXTRA_RUPEES)

static const DropList sList[] = {
    {0,                 0, nullptr},
    {3,    FLAG_EXTRA_ALL,     e01},
    {3,    FLAG_EXTRA_ALL,     e02},
    {3,    FLAG_EXTRA_ALL,     e03},
    {1,    FLAG_EXTRA_ALL,     e04},
    {2,    FLAG_EXTRA_ALL,     e05},
    {1,    FLAG_EXTRA_ALL,     e06},
    {2,    FLAG_EXTRA_ALL,     e07},
    {3,    FLAG_EXTRA_ALL,     e08},
    {2,    FLAG_EXTRA_ALL,     e09},
    {1,                 0,     e10},
    {1,                 0,     e11},
    {1,                 0,     e12},
    {1,                 0,     e13},
    {1,                 0,     e14},
    {1,                 0,     e15},
    {2,    FLAG_EXTRA_ALL,     e16},
    {2,    FLAG_EXTRA_ALL,     e17},
    {1,                 0,     e18},
    {1,                 0,     e19},
    {1,                 0,     e20},
    {1,                 0,     e21},
    {1,                 0,     e22},
    {3,    FLAG_EXTRA_ALL,     e23},
    {4,    FLAG_EXTRA_ALL,     e24},
    {1,                 0,     e25},
    {1,                 0,     e26},
    {1,                 0,     e27},
    {1,                 0,     e28},
    {1,                 0,     e29},
    {1,                 0,     e30},
    {1,    FLAG_EXTRA_ALL,     e31},
    {1,    FLAG_EXTRA_ALL,     e32},
    {1,    FLAG_EXTRA_ALL,     e33},
    {4,    FLAG_EXTRA_ALL,     e34},
    {1,                 0,     e35},
    {1,                 0,     e36},
    {3,    FLAG_EXTRA_ALL,     e37},
    {3,    FLAG_EXTRA_ALL,     e38},
    {1,                 0,     e39},
    {2,                 0,     e40},
    {1,                 0,     e41},
    {1, FLAG_EXTRA_HEARTS,     e42},
    {0,                 0, nullptr},
    {1, FLAG_EXTRA_HEARTS,     e44},
    {2,    FLAG_EXTRA_ALL,     e45},
    {2,    FLAG_EXTRA_ALL,     e46},
    {1,                 0,     e47},
    {1, FLAG_EXTRA_HEARTS,     e48},
    {1, FLAG_EXTRA_HEARTS,     e49},
    {1,                 0,     e50},
    {3,    FLAG_EXTRA_ALL,     e51},
    {3,                 0,     e52},
    {5, FLAG_EXTRA_RUPEES,     e53},
};

struct LowHealthReplacement {
    s32 mOriginalIdx;
    s32 mReplacementIndex;
};

const LowHealthReplacement LOW_HEALTH_REPLACEMENTS[] = {
    {
     0x10, 0x11,
     },
    {
     0x1E, 0x1F,
     },
    {
     0x2B, 0x2C,
     },
    {
     0x2D, 0x2E,
     },
};

static const u16 SPECIAL_ITEM_ARRAY[] = {ITEM_NONE,       ITEM_HEART,        ITEM_HEART,         ITEM_GREEN_RUPEE,
                                         ITEM_BLUE_RUPEE, ITEM_RED_RUPEE,    ITEM_FARORE_TEAR,   ITEM_5_BOMBS,
                                         ITEM_10_BOMBS,   ITEM_SINGLE_ARROW, ITEM_BUNDLE_ARROWS, ITEM_5_DEKU_SEEDS,
                                         ITEM_NONE,       ITEM_NONE,         ITEM_KEY_PIECE,     ITEM_ELDIN_ORE,
                                         ITEM_UNK_58,     ITEM_UNK_59,       ITEM_UNK_183,       ITEM_UNK_184,
                                         ITEM_UNK_185,    ITEM_FAIRY,        ITEM_FAIRY,         ITEM_SMALL_KEY,
                                         ITEM_RUPOOR};

static const u32 sNumDropEntries = 0x36;
static const struct {
    u16 mNumEntries;
    const DropList *mList;
} sDropTable = {
    0x36,
    sList,
};
static const u16 RAND_RUPEE_ARRAY[3] = {ITEM_GREEN_RUPEE, ITEM_BLUE_RUPEE, ITEM_RED_RUPEE};

// 800c7bb0
int SpecialItemDropMgr::fn_800C7BB0(int specialItemId) {
    const DropList *list = &sDropTable.mList[specialItemId];
    const DropEntry *e = list->mpEntries;
    int random = cM::rndInt(100);

    int acc = 0;

    for (int i = 0; i < list->mLen; i++) {
        int entryIdx = e->mEntryIndex;
        int weight = e->mDropChance;

        if (entryIdx == 9 || entryIdx == 10) {
            if (dAcPy_c::getCurrentBowType() == 0) {
                // No arrows until you get the Bow
                weight = 0;
            }
        } else if (entryIdx == 11) {
            if (dAcPy_c::getCurrentSlingshotType() == 0) {
                // No seeds until you get the Slingshot
                weight = 0;
            }
        } else if (specialItemId != 0x27 && (entryIdx == 1 || entryIdx == 2) && dScGame_c::isHeroMode()) {
            // Hearts generally don't drop on Hero Mode unless it's a special kind of heart (TODO which?)
            weight = 0;
        } else if ((entryIdx == 7 || entryIdx == 8) && !dAcItem_c::checkFlag(ITEM_BOMB_BAG)) {
            // Bombs won't drop until you get the Bomb Bag
            weight = 0;
        }

        if (weight != 0) {
            int x = 0x1E / weight;
            acc += weight;
            if (x != 0) {
                int tmp = x <= 3 ? x : 3;
                if (random % tmp != 0) {
                    return 0;
                }
            }

            if (random < acc) {
                return entryIdx;
            }
        }
        e++;
    }

    return 0;
}

bool SpecialItemDropMgr::shouldTryExtraHearts(int specialItemId) {
    return sDropTable.mList[specialItemId].mFlags & FLAG_EXTRA_HEARTS;
}
bool SpecialItemDropMgr::shouldTryExtraRupees(int specialItemId) {
    return sDropTable.mList[specialItemId].mFlags & FLAG_EXTRA_RUPEES;
}

// 800c7d40
int SpecialItemDropMgr::giveSpecialDropItem(
    int specialItemId, int roomid, mVec3_c *pos, int subtype, mAng rot, s32 unused
) {
    if (specialItemId == 0xFF) {
        return 0;
    }

    for (u32 i = 0; i < 4; i++) {
        const LowHealthReplacement &entry = LOW_HEALTH_REPLACEMENTS[i];
        if (specialItemId == entry.mOriginalIdx && dAcPy_c::LINK->hasLessThanQuarterHealth(false)) {
            specialItemId = entry.mReplacementIndex;
            break;
        }
    }

    bool ret = spawnSpecialDropItem(specialItemId, roomid, pos, subtype, rot);
    if (shouldTryExtraHearts(specialItemId)) {
        for (u32 i = 0; i < adventurePouchCountItem(ITEM_HEART_MEDAL); i++) {
            bool ok = spawnSpecialDropItem(0x27, roomid, pos, subtype, rot);
            ret |= ok;
        }
    }

    if (shouldTryExtraRupees(specialItemId)) {
        for (u32 i = 0; i < adventurePouchCountItem(ITEM_RUPEE_MEDAL) + adventurePouchCountItem(ITEM_CURSED_MEDAL);
             i++) {
            bool ok = spawnSpecialDropItem(0x28, roomid, pos, subtype, rot);
            ret |= ok;
        }
    }

    return ret;
}

static s32 SOME_ANG = -3641;

// 800c7ef0
// Very unmatching. Just here as a starting point
bool SpecialItemDropMgr::spawnSpecialDropItem(int specialItemId, int roomid, mVec3_c *pos, int subtype, mAng rot) {
    s32 unk = fn_800C7BB0(specialItemId);
    if (unk == 0) {
        return false;
    }

    s8 itemCount = 1;
    if (unk == 2) {
        itemCount = 3;
    } else if (unk == 12) {
        itemCount = 10;
    } else if (unk == 13) {
        itemCount = 5;
    } else if (unk == 22) {
        itemCount = 2;
    }

    u32 itemid = SPECIAL_ITEM_ARRAY[unk];
    mAng3_c itemRot(0, 0, 0);

    s32 max;
    s32 min;
    if (subtype == 2 || subtype == 6) {
        max = rot;
        min = SOME_ANG;
    } else {
        max = (s16)cLib::targetAngleY(dAcPy_c::LINK->position, *pos) + 0x4000;
        min = -0x8000;
    }

    s16 stepSize = s16(min) / itemCount;
    mAng range = stepSize / 2;
    s32 step = s16(max) + range;
    mAng rndMax = range / 2;
    mAng rndMin = -rndMax;

    for (int currentItemIndex = 0; currentItemIndex < itemCount; currentItemIndex++) {
        itemRot.y = mAng(step) + cM::rndRange(rndMin, rndMax);
        if (unk >= 12 && unk < 14) {
            itemid = RAND_RUPEE_ARRAY[cM::rndInt(3)];
            spawnItem1(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
        } else if (subtype == 2) {
            spawnItem2(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
        } else {
            if (subtype == 1) {
                spawnItem3(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else if (subtype == 4) {
                spawnItem4(itemid, 0, 0xFFFFFFFF);
            } else if (subtype == 3) {
                spawnItem5(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else if (subtype == 5) {
                spawnItem6(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else if (subtype == 6) {
                spawnItem7(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else if (subtype == 7) {
                spawnItem8(itemid, roomid, pos, &itemRot, nullptr, nullptr, 0xFFFFFFFF, 0);
            } else {
                spawnItem9(itemid, roomid, pos, &itemRot);
            }
        }
        step = mAng(step) - stepSize;
    }
    return true;
}

// 800c8270
// SpecialItemDropMgr::~SpecialItemDropMgr() {}
