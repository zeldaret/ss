#ifndef SPECIAL_ITEM_DROP_MGR_H
#define SPECIAL_ITEM_DROP_MGR_H

#include "common.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

enum SpecialItemDropTable {
    SPECIAL_ITEM_0 = 0,
    SPECIAL_ITEM_1,
    SPECIAL_ITEM_2,
    SPECIAL_ITEM_3,
    SPECIAL_ITEM_4,
    SPECIAL_ITEM_5,
    SPECIAL_ITEM_6,
    SPECIAL_ITEM_7,
    SPECIAL_ITEM_8,
    SPECIAL_ITEM_9,
    SPECIAL_ITEM_10,
    SPECIAL_ITEM_11,
    SPECIAL_ITEM_12,
    SPECIAL_ITEM_13,
    // ...
};

class SpecialItemDropMgr {
public:
    static SpecialItemDropMgr *sInstance;

    SpecialItemDropMgr();
    virtual ~SpecialItemDropMgr() {}

    static SpecialItemDropMgr *create();

    int fn_800C7BB0(int specialItemId);
    bool shouldTryExtraHearts(int specialItemId);
    bool shouldTryExtraRupees(int specialItemId);
    int giveSpecialDropItem(int specialItemId, int roomid, mVec3_c *pos, int subtype, mAng rot, s32 unused);
    bool spawnSpecialDropItem(int specialItemId, int roomid, mVec3_c *pos, int subtype, mAng rot);
};

#endif
