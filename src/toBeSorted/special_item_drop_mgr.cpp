#include "toBeSorted/special_item_drop_mgr.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
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

// TODO: Convert to enum once work on items has started
static const u16 SPECIAL_ITEM_ARRAY[28] = {0,    6,    6,  2,  3,   4,   0x2b, 0x28, 0x29, 7, 8,  0x39, 0, 0,
                                           0x1c, 0xa5, 58, 59, 183, 184, 185,  72,   72,   1, 34, 0,    0, 0};
static const u16 RAND_RUPEE_ARRAY[4] = {2, 3, 4, 0};

// 800c7bb0
int fn_800C7BB0(SpecialItemDropMgr *mgr, int specialItemId) {}

// 800c7d00
short fn_800C7D00(SpecialItemDropMgr *mgr, int specialItemId) {}

// 800c7d20
short fn_800C7D20(SpecialItemDropMgr *mgr, int specialItemId) {}

// 800c7d40
int giveSpecialDropItem(
    SpecialItemDropMgr *mgr, int specialItemId, int roomid, mVec3_c *pos, int subtype, s16 *rot, s32 unused
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
bool spawnSpecialDropItem(SpecialItemDropMgr *mgr, int specialItemId, int roomid, mVec3_c *pos, int subtype, s16 *rot) {
    s32 unk = fn_800C7BB0(mgr, specialItemId);
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
        currentRot = *rot;
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
