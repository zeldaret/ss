#ifndef D_T_BARREL_H
#define D_T_BARREL_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/t/d_tg.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"

class dTgBarrelPos_c;
class dAcOBarrel_c;
class dAcOstageSink_c;

class dTgBarrel_c : public dTg_c {
public:
    dTgBarrel_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgBarrel_c() {}

    virtual int doDelete() override;
    virtual int draw() override;
    virtual int actorCreate() override;
    virtual int actorPostCreate() override;
    virtual int actorExecute() override;

    STATE_FUNC_DECLARE(dTgBarrel_c, Wait);
    STATE_FUNC_DECLARE(dTgBarrel_c, Stop);

    bool checkPlayerPos(const mVec3_c &playerPos);

private:
    /* 0x0FC */ STATE_MGR_DECLARE(dTgBarrel_c);
    /* 0x138 */ dAcRef_c<dAcOstageSink_c> mStageRef;
    /* 0x144 */ dAcRef_c<dTgBarrelPos_c> mTgBarrelPosArr[10];
    /* 0x1BC */ dAcRef_c<dAcOBarrel_c> mBarrelArr[10];
    /* 0x234 */ mVec3_c mSpawnPosition;
    /* 0x240 */ u8 field_0x240; ///< Stop Sceneflag
    /* 0x241 */ u8 field_0x241; ///< Barrel Spawn Interval in Seconds
    /* 0x242 */ u8 field_0x242; ///< Barrel Spawn Timer
    /* 0x243 */ u8 field_0x243;
    /* 0x244 */ u8 field_0x244;
    /* 0x245 */ u8 field_0x245;
    /* 0x246 */ u8 field_0x246;
};

#endif
