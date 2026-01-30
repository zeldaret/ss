#ifndef D_T_SKYENEMY_H
#define D_T_SKYENEMY_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgSkyEnemyTag : public dTg_c {
public:
    dTgSkyEnemyTag() : mStateMgr(*this) {}
    virtual ~dTgSkyEnemyTag() {}

    STATE_FUNC_DECLARE(dTgSkyEnemyTag, Wait);
    STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthRupeeGue);
    STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthSkytail);
    STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthTornado);
    STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthBirdNpc);
    STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthFrontBirdNpc);
    STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthNusiNpc);
    STATE_FUNC_DECLARE(dTgSkyEnemyTag, BirthHighSpeed);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dTgSkyEnemyTag);
};

#endif
