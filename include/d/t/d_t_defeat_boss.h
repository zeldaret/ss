#ifndef D_T_DEFEAT_BOSS_H
#define D_T_DEFEAT_BOSS_H

#include "d/t/d_tg.h"

class dTgDefeatBoss_c : public dTg_c {
public:
    dTgDefeatBoss_c() {}
    virtual ~dTgDefeatBoss_c() {}
    virtual int update() override;
    u16 getSceneflag();
    u16 fn_494_360();
    u16 getStoryflag();
    u16 fn_494_380();
    static void getDungeonFlagIndex(u32 &outFlagIdx);
    bool fn_494_D0();
    bool init();

    /* 0xFC */ u8 var8_1;//area?
    /* 0xFD */ u8 var8_2;//scene flag? I assume this is only 2 byte


private:
};



#endif
