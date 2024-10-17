#ifndef D_LYT_BOSS_GAUGE_H
#define D_LYT_BOSS_GAUGE_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytBossGauge_c {
public:
    dLytBossGauge_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytBossGauge_c() {}

    bool build();
    bool remove();
    bool draw();

private:
    STATE_FUNC_DECLARE(dLytBossGauge_c, None);
    STATE_FUNC_DECLARE(dLytBossGauge_c, In);
    STATE_FUNC_DECLARE(dLytBossGauge_c, Move);
    STATE_FUNC_DECLARE(dLytBossGauge_c, Out);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytBossGauge_c);
    /* 0x040 */ d2d::ResAccIf_c mResAcc;
    /* 0x3B0 */ d2d::dLytSub mLyt;
    /* 0x444 */ d2d::AnmGroup_c mAnmGroups[4];
};

#endif
