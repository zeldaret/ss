#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include "d/d_base.h"
#include "toBeSorted/unk_with_water.h"

class dStageMgr_c : dBase_c {
public:
    bool isAreaTypeDungeon() const;
    bool isAreaTypeSky() const;
    bool isAreaTypeDungeonOrBoss() const;
    bool isAreaTypeOverworldOrSkyloft() const;
    s32 getSTIFunk1() const;

    s32 getAreaType() const;

    static dStageMgr_c *GetInstance() {
        return sInstance;
    }

    bool checkFlag0x88A0(u32 flag) const {
        return (field_0x88A0 & flag) != 0;
    }

    void initUnkWithWater(u32 val, UnkWithWater *waterThing);
    void destroyUnkWithWater(u32 val, UnkWithWater *waterThing);

private:
    static dStageMgr_c *sInstance;

    /* 0x0068 */ u8 _0x0068[0x88A0 - 0x0068];
    /* 0x88A0 */ u32 field_0x88A0;
};

#endif
