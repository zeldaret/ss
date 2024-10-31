#ifndef SPECIAL_ITEM_DROP_MGR_H
#define SPECIAL_ITEM_DROP_MGR_H

#include "common.h"
#include "m/m_angle.h"
#include "m/m_vec.h"

class SpecialItemDropMgr {
public:
    static SpecialItemDropMgr *sInstance;

    SpecialItemDropMgr();
    virtual ~SpecialItemDropMgr() {}

    static SpecialItemDropMgr *create();

    int fn_800C7BB0(int specialItemId);
    bool shouldTryExtraHearts(int specialItemId);
    bool shouldTryExtraRupees(int specialItemId);
    int giveSpecialDropItem(
        int specialItemId, int roomid, mVec3_c *pos, int subtype, mAng rot, s32 unused
    );
    bool
    spawnSpecialDropItem(int specialItemId, int roomid, mVec3_c *pos, int subtype, mAng rot);
};

#endif
