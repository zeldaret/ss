#include "toBeSorted/special_item_drop_mgr.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_item.h"
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
    /* 0x0 */ u8 field_0x00;
    /* 0x0 */ u8 field_0x01;
};

struct DropList {
    /* 0x0 */ u16 mLen;
    /* 0x2 */ u16 mFlags;
    /* 0x4 */ const DropEntry *mpEntries;
};

static const DropEntry e01[] = {
    {0x01, 0x14},
    {0x03, 0x28},
    {0x04, 0x0A},
};

static const DropEntry e02[] = {
    {0x01, 0x14},
    {0x03, 0x0A},
    {0x04, 0x28},
};

static const DropEntry e03[] = {
    {0x01, 0x14},
    {0x03, 0x28},
    {0x04, 0x0A},
};

static const DropEntry e04[] = {
    {0x01, 0x14},
};

static const DropEntry e05[] = {
    {0x01, 0x14},
    {0x03, 0x28},
};

static const DropEntry e53[] = {
    {0x08, 0x14},
    {0x0A, 0x14},
    {0x0B, 0x14},
    {0x03, 0x0A},
    {0x04, 0x05},
};

static const DropList sList[] = {
    {0, 0, nullptr},
    {3, 3,     e01},
    {3, 3,     e02},
    {3, 3,     e03},
    {1, 3,     e04},
    {2, 3,     e01},
    {1, 3,     e01},
    {2, 3,     e01},
    {3, 3,     e01},
    {2, 3,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {2, 3,     e01},
    {2, 3,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {3, 3,     e01},
    {4, 3,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {1, 3,     e01},
    {1, 3,     e01},
    {1, 3,     e01},
    {4, 3,     e01},
    {1, 0,     e01},
    {1, 0,     e01},
    {3, 3,     e01},
    {3, 3,     e01},
    {1, 0,     e01},
    {2, 0,     e01},
    {1, 0,     e01},
    {1, 1,     e01},
    {0, 0, nullptr},
    {1, 1,     e01},
    {2, 3,     e01},
    {2, 3,     e01},
    {1, 0,     e01},
    {1, 1,     e01},
    {1, 1,     e01},
    {1, 0,     e01},
    {3, 3,     e01},
    {3, 0,     e01},
    {5, 2,     e53},
};

extern const u32 MORE_ITEMS[] = {0x10, 0x11, 0x1E, 0x1F, 0x2B, 0x2C, 0x2D, 0x2E};

// TODO: Convert to enum once work on items has started
static const u16 SPECIAL_ITEM_ARRAY[28] = {0,    6,    6,  2,  3,   4,   0x2b, 0x28, 0x29, 7, 8,  0x39, 0, 0,
                                           0x1c, 0xa5, 58, 59, 183, 184, 185,  72,   72,   1, 34, 0,    0, 0};

static const u16 RAND_RUPEE_ARRAY[4] = {2, 3, 4, 0};

static const u32 sNumDropEntries = 0x36;
static const struct {
    u32 mNumEntries;
    const DropList *mList;
} sDropTable = {
    0x36,
    sList,
};

extern "C" int getCurrentBowType();
extern "C" int getCurrentSlingshotType();
extern "C" bool isHeroMode();

// 800c7bb0
int SpecialItemDropMgr::fn_800C7BB0(int specialItemId) {
    const DropList *list = &sDropTable.mList[specialItemId];
    const DropEntry *e = list->mpEntries;
    int random = cM::rndInt(100);

    int acc = 0;

    for (int i = 0; i < list->mLen; i++) {
        int u1 = e->field_0x00;
        int u2 = e->field_0x01;

        if (u1 >= 9 && u1 < 11) {
            if (getCurrentBowType() == 0) {
                u2 = 0;
            }
        } else if (u1 == 11) {
            if (getCurrentSlingshotType() == 0) {
                u2 = 0;
            }
        } else if (specialItemId != 39 && !(u1 < 1 || u1 >= 3) && isHeroMode()) {
            u2 = 0;
        } else if (u1 >= 7 && u1 < 9 && !dAcItem_c::checkFlag(92)) {
            u2 = 0;
        }

        if (u2 != 0) {
            int x = 0x1E / u2;
            acc += u2;
            if (x != 0) {
                int tmp = x <= 3 ? x : 3;
                if (random % tmp != 0) {
                    return 0;
                }
            }

            if (random < acc) {
                return u1;
            }
        }
        e++;
    }

    return 0;
}

bool SpecialItemDropMgr::shouldTryExtraHearts(int specialItemId) {
    return sDropTable.mList[specialItemId].mFlags & 1;
}
bool SpecialItemDropMgr::shouldTryExtraRupees(int specialItemId) {
    return sDropTable.mList[specialItemId].mFlags & 2;
}

// 800c7d40
int SpecialItemDropMgr::giveSpecialDropItem(
    int specialItemId, int roomid, mVec3_c *pos, int subtype, mAng rot, s32 unused
) {}

// TODO move this somewhere
template <typename T>
T rndRange(T min, T max) {
    f32 r = cM::rndF(max - min);
    f32 m = min;
    m += r;
    return m;
}

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

    mAng currentRot;
    mAng tempOther;
    u32 itemid = SPECIAL_ITEM_ARRAY[unk];
    mAng3_c itemRot(0, 0, 0);

    if (subtype == 2 || subtype == 6) {
        currentRot = rot;
        tempOther = -0xe39;
    } else {
        currentRot = cLib::targetAngleY(dAcPy_c::LINK->position, *pos);
        tempOther = -0x8000;
        currentRot += 0x4000;
    }
    s32 angleDecrement = tempOther / itemCount;
    tempOther = angleDecrement / 2;
    currentRot += tempOther;
    tempOther = tempOther / 2;

    for (int currentItemIndex = 0; currentItemIndex < itemCount; currentItemIndex++) {
        mAng out = rndRange(tempOther, -tempOther);

        itemRot.y = currentRot + out;
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
        currentRot.mVal -= angleDecrement;
    }
    return true;
}

// 800c8270
// SpecialItemDropMgr::~SpecialItemDropMgr() {}
