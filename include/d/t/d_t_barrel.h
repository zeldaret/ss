#ifndef D_T_BARREL_H
#define D_T_BARREL_H

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/t/d_tg.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"

class dTgBarrelPos_c;
class dAcOBarrel_c;
class dAcOstageSink_c;

class dTgBarrel_c : public dTg_c {
public:
    dTgBarrel_c() : mStateMgr(*this) {}
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
    /* 0x138 */ dAcObjRef_c mStageRef;
    /* 0x144 */ dAcRef_c<dTgBarrelPos_c> mTgBarrelPosArr[10];
    /* 0x1BC */ dAcObjRef_c mBarrelArr[10];
    /* 0x234 */ mVec3_c mSpawnPosition;
    /* 0x240 */ u8 mStopFlag;       ///< Stop Sceneflag - Note there is no checking done to see if valid
    /* 0x241 */ u8 mSpawnInterval;  ///< Barrel Spawn Interval in Seconds
    /* 0x242 */ u8 mSpawnTimer;     ///< Barrel Spawn Timer
    /* 0x243 */ u8 mBarrelPosCount; ///< Represents the index of the highest dTgBarrelPos_c in mTgBarrelPosArr
    /* 0x244 */ u8 mBarrelPosIndex; ///< Rolling index to spawn dAcOBarrel_c from a dTgBarrelPos_c
    /* 0x245 */ u8 mLinkId;         ///< Link Id used to find correct dTgBarrelPos_c
    /* 0x246 */ bool mSpawnActive;  ///< Used to reset the spawn timer after the player has already entered the area
};

#endif
