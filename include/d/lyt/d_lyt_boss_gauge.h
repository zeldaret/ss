#ifndef D_LYT_BOSS_GAUGE_H
#define D_LYT_BOSS_GAUGE_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytBossGauge_c {
public:
    dLytBossGauge_c() : mStateMgr(*this) {}
    virtual ~dLytBossGauge_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

    void setMode(s32 mode);

    void fn_80158940();

    void setField_0x545(bool value) {
        field_0x545 = value;
    }

private:
    STATE_FUNC_DECLARE(dLytBossGauge_c, None);
    STATE_FUNC_DECLARE(dLytBossGauge_c, In);
    STATE_FUNC_DECLARE(dLytBossGauge_c, Move);
    STATE_FUNC_DECLARE(dLytBossGauge_c, Out);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytBossGauge_c);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytBossGauge_c);
    /* 0x040 */ d2d::ResAccIf_c mResAcc;
    /* 0x3B0 */ d2d::dLytSub mLyt;
    /* 0x444 */ d2d::AnmGroup_c mAnmGroups[4];
    /* 0x544 */ u8 field_0x544;
    /* 0x545 */ bool field_0x545;
    /* 0x546 */ u8 _0x546[0x550 - 0x546];
};

#endif
