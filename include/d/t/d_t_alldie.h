#ifndef D_T_ALL_DIE_H
#define D_T_ALL_DIE_H

#include "d/t/d_tg.h"
#include "m/m_mtx.h"
#include "toBeSorted/time_area_mgr.h"

class dTgAllDie_c : public dTg_c {
public:
    dTgAllDie_c() {}
    virtual ~dTgAllDie_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorExecute() override;

    static dTgAllDie_c *getNearestToPlayer(s32 roomId);

private:
    /* 0x0FC */ mMtx_c mArea;
    /* 0x12C */ TimeAreaStruct mTimeArea;
    /* 0x138 */ s32 mNumEnemiesInArea;
    /* 0x13C */ u16 mTimer0x13C;
    /* 0x13E */ u8 mType;
    /* 0x13F */ u8 mSceneflag;
    /* 0x140 */ u8 mTimer0x140;
    /* 0x144 */ u32 field_0x144;
    /* 0x145 */ u8 field_0x148;
};

#endif
