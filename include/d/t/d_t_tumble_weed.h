#ifndef D_T_TUMBLE_WEED_H
#define D_T_TUMBLE_WEED_H

#include "d/a/obj/d_a_obj_tumble_weed.h"
#include "d/t/d_tg.h"
#include "s/s_State.hpp"

class dTgTumbleWeed_c : public dTg_c {
public:
    dTgTumbleWeed_c() : mStateMgr(*this, sStateID::null), childTumbleweed(nullptr) {}
    virtual ~dTgTumbleWeed_c() {}
    virtual int create() override;
    virtual int actorExecute() override;
    virtual int draw() override;
    virtual int doDelete() override;

    STATE_FUNC_DECLARE(dTgTumbleWeed_c, AreaOut);
    STATE_FUNC_DECLARE(dTgTumbleWeed_c, AreaIn);
    STATE_FUNC_DECLARE(dTgTumbleWeed_c, Wind);

private:
    bool shouldSpawnTumbleweed();
    bool shouldDoWind();
    void doSpawnTumbleweed();
    void getWind(mVec3_c *);

    STATE_MGR_DECLARE(dTgTumbleWeed_c);
    u16 tumbleweedTimer;
    u16 padding;
    u16 windTimer;
    dAcRef_c<dAcOTumbleWeed_c> childTumbleweed;
    u16 someField;
};

#endif
