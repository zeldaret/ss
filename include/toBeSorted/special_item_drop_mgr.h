#ifndef SPECIAL_ITEM_DROP_MGR_H
#define SPECIAL_ITEM_DROP_MGR_H

#include "common.h"
#include "m/m_vec.h"

class SpecialItemDropMgr {
public:
    static SpecialItemDropMgr *sInstance;

    SpecialItemDropMgr();
    virtual ~SpecialItemDropMgr() {}

    static SpecialItemDropMgr* create();

    static int fn_800C7BB0(SpecialItemDropMgr *mgr, int specialItemId);
    static short fn_800C7D00(SpecialItemDropMgr *mgr, int specialItemId);
    static short fn_800C7D20(SpecialItemDropMgr *mgr, int specialItemId);
    static int giveSpecialDropItem(
        SpecialItemDropMgr *mgr, int specialItemId, int roomid, mVec3_c *pos, int subtype, s16 *rot, s32 unused
    );
    static bool
    spawnSpecialDropItem(SpecialItemDropMgr *mgr, int specialItemId, int roomid, mVec3_c *pos, int subtype, s16 *rot);
    static void fn_800C81D0(s16, s16, s16);
};

#endif
