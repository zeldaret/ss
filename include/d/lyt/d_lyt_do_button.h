#ifndef D_LYT_DO_BUTTON_H
#define D_LYT_DO_BUTTON_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_sub.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytDobutton_c {
public:
    dLytDobutton_c() : mStateMgr(*this, sStateID::null) {
        sInstance = this;
    }

    bool init(m2d::ResAccIf_c *resAcc);

    dLytDobutton_c *sInstance;

private:
    STATE_FUNC_DECLARE(dLytDobutton_c, InvisibleWait);
    STATE_FUNC_DECLARE(dLytDobutton_c, InvisibleTimeCnt);
    STATE_FUNC_DECLARE(dLytDobutton_c, In);
    STATE_FUNC_DECLARE(dLytDobutton_c, Wait);
    STATE_FUNC_DECLARE(dLytDobutton_c, Out);

    UI_STATE_MGR_DECLARE(dLytDobutton_c);
    /* 0x040 */ dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnmGroups[12];
    /* 0x3D4 */ u8 field_0x3D4[0x470 - 0x3D4];
    /* 0x470 */ s32 mDoActionToShow;
    /* 0x474 */ s32 field_0x474;
    /* 0x478 */ s32 field_0x478;
    /* 0x47C */ s32 field_0x47C;
    /* 0x480 */ s32 field_0x480;
    /* 0x484 */ s32 mNextDoActionToShow;
    /* 0x488 */ s32 field_0x488;
    /* 0x48C */ s32 field_0x48C;
};

#endif
