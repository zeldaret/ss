#ifndef D_T_TUMBLE_WEED_H
#define D_T_TUMBLE_WEED_H

#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dAcOTumbleWeed_c;
class dTgTumbleWeed_c : public dTg_c {
public:
    dTgTumbleWeed_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dTgTumbleWeed_c() {}
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dTgTumbleWeed_c, AreaOut);
    STATE_FUNC_DECLARE(dTgTumbleWeed_c, AreaIn);
    STATE_FUNC_DECLARE(dTgTumbleWeed_c, Wind);

    STATE_MGR_DEFINE_UTIL_EXECUTESTATE(dTgTumbleWeed_c);

private:
    bool shouldSpawnTumbleweed();
    bool shouldDoWind();
    void doSpawnTumbleweed();
    void getWind(mVec3_c &);

    /* 0x0FC */ STATE_MGR_DECLARE(dTgTumbleWeed_c);

    /* 0x138 */ u16 mSpawnTimer;
    /* 0x13A */ u16 field_0x13A;
    /* 0x13C */ u16 mWindTimer;
    /* 0x140 */ dAcRef_c<dAcOTumbleWeed_c> mSpawnedTumbleWeed;
    /* 0x14C */ u16 field_0x14C;
};

#endif
