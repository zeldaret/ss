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
    bool checkDungeonFlag();
    virtual int create() override;
    virtual int actorExecute() override;

    u16 getSceneflag();
    s32 getSceneflagPolarity();
    u32 getStoryflag();
    s32 getStoryflagPolarity();


private:
    /* 0xFC */ s8 mSceneflagPolarity;
    /* 0xFD */ s8 mStoryflagPolarity;
};



#endif
