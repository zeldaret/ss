#include "m/m_vec.h"
#include "m/m_Angle.h"
#include "d/a/d_a_player.h"
#include "toBeSorted/special_item_drop_mgr.h"

SpecialItemDropMgr* SpecialItemDropMgr::sInstance = nullptr;

extern "C" short targetAngleY(mVec3_c *, mVec3_c *);
extern "C" int rndInt(int);
extern "C" void spawnItem1(s16 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem2(s16 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem3(s16 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem4(s16 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem5(s16 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem6(s16 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem7(s16 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem8(s16 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);
extern "C" void spawnItem9(s16 itemid, u32 roomid, mVec3_c *pos, mAng3_c *rot, u32 param2, u32 unk);

// 800c7b80
SpecialItemDropMgr::SpecialItemDropMgr() {
    SpecialItemDropMgr::sInstance = this;
}

// TODO: Convert to enum once work on items has started
int SPECIAL_ITEM_ARRAY[28] = {0, 6, 6, 2, 3, 4, 0x2b, 0x28, 0x29, 7, 8, 0x39, 0, 0, 0x1c, 0xa5, 58, 59, 183, 184, 185, 72, 72, 1, 34, 0, 0, 0};
int RAND_RUPEE_ARRAY[4] = {2, 3, 4, 0};

// 800c7bb0
int fn_800C7BB0(SpecialItemDropMgr *mgr, int specialItemId) {}

// 800c7d00
short fn_800C7D00(SpecialItemDropMgr *mgr, int specialItemId) {}

// 800c7d20
short fn_800C7D20(SpecialItemDropMgr *mgr, int specialItemId) {}

// 800c7d40
int giveSpecialDropItem(SpecialItemDropMgr *mgr, int specialItemId, int roomid, mVec3_c *pos, int subtype, s16 *rot, s32 unused) {}

// 800c7ef0
// Very unmatching. Just here as a starting point
bool spawnSpecialDropItem(SpecialItemDropMgr *mgr, int specialItemId, int roomid, mVec3_c *pos, int subtype, s16 *rot) {
    u32 unk = fn_800C7BB0(mgr, specialItemId);
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

    s16 currentRot;
    s16 tempOther;
    u16 itemid = SPECIAL_ITEM_ARRAY[unk];
    mAng3_c itemRot(0, 0, 0);

    if (subtype == 2 || subtype == 6) {
        currentRot = *rot;
        tempOther = -0xe39;
    } else {
        currentRot = targetAngleY(&(dAcPy_c::LINK->position), pos);
        tempOther = -0x8000;
        currentRot += 0x4000;
    }
    s16 angleDecrement = tempOther / itemCount;
    tempOther = angleDecrement / 2;
    currentRot += tempOther;
    tempOther /= 2;

    for (s8 currentItemIndex = 0; currentItemIndex < itemCount; currentItemIndex++) {
        s16 out;
        SpecialItemDropMgr::fn_800C81D0(out, tempOther, -tempOther);

        itemRot.y = currentRot + out;
        if (unk < 15) {
            itemid = RAND_RUPEE_ARRAY[rndInt(3)];
            spawnItem1(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
        } else if (subtype == 2) {
            spawnItem2(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
        } else {
            if (subtype == 1) {
                spawnItem3(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else if (subtype == 4) {
                spawnItem4(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else if (subtype == 3) {
                spawnItem5(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else if (subtype == 5) {
                spawnItem6(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else if (subtype == 6) {
                spawnItem7(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else if (subtype == 7) {
                spawnItem8(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            } else {
                spawnItem9(itemid, roomid, pos, &itemRot, 0xFFFFFFFF, 0);
            }
        }
        currentRot -= angleDecrement;
    }
    return true;
}

// 800c81d0
void fn_800C81D0(s16, s16, s16) {};

// 800c8270
SpecialItemDropMgr::~SpecialItemDropMgr() {}
