#ifndef D_T_TUMBLE_WEED_H
#define D_T_TUMBLE_WEED_H

#include <d/t/d_tg.h>
#include <f/f_list_nd.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

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
    // TODO dAcObjTumbleWeed
    dAcRef_c<dAcBase_c> childTumbleweed;
    u16 someField;
};

#endif
