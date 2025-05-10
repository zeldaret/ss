#ifndef D_LYT_WIPE_H
#define D_LYT_WIPE_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytWipeMain_c {
public:
    dLytWipeMain_c();
    virtual ~dLytWipeMain_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();

    void gotoIn();
    void gotoWait();
    void gotoOut();
    void gotoNone();

    bool isStateEndReached() const {
        return mStateEndReached;
    }

private:
    STATE_FUNC_DECLARE(dLytWipeMain_c, ModeNone);
    STATE_FUNC_DECLARE(dLytWipeMain_c, ModeIn);
    STATE_FUNC_DECLARE(dLytWipeMain_c, ModeWait);
    STATE_FUNC_DECLARE(dLytWipeMain_c, ModeOut);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytWipeMain_c);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytWipeMain_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnm[1];
    /* 0x110 */ bool mStateEndReached;
    /* 0x111 */ u8 _0x111[0x132 - 0x111];
    /* 0x132 */ bool mIsIn;
};

class dLytWipe_c {
public:
    dLytWipe_c() : mStateMgr(*this, sStateID::null) {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

private:
    static dLytWipe_c *sInstance;

    STATE_FUNC_DECLARE(dLytWipe_c, None);
    STATE_FUNC_DECLARE(dLytWipe_c, In);
    STATE_FUNC_DECLARE(dLytWipe_c, Wait);
    STATE_FUNC_DECLARE(dLytWipe_c, Out);

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytWipe_c);
    /* 0x03C */ d2d::ResAccIf_c mResAcc;
    /* 0x3AC */ dLytWipeMain_c mMain;

    /* 0x4E0 */ u8 _0x4E0[0x59C - 0x4E0];

    /* 0x59C */ bool mWantsIn;
    /* 0x59D */ bool mWantsOut;
    /* 0x59E */ bool mIsSettled;
    /* 0x59F */ bool mVisible;
    /* 0x5A0 */ s32 mTransitionStage;
    /* 0x5A4 */ s32 mFadeDelay;
};

#endif
