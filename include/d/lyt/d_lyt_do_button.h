#ifndef D_LYT_DO_BUTTON_H
#define D_LYT_DO_BUTTON_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytDobutton_c {
public:
    dLytDobutton_c() : mStateMgr(*this, sStateID::null) {
        sInstance = this;
    }
    virtual ~dLytDobutton_c() {
        sInstance = nullptr;
    }

    bool init(m2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();

    static dLytDobutton_c *GetInstance() {
        return sInstance;
    }

    static s32 getField0x47C() {
        if (sInstance != nullptr) {
            return sInstance->field_0x47C;
        } else {
            return 0x5E;
        }
    }

    static s32 getField0x480() {
        if (sInstance != nullptr) {
            return sInstance->field_0x480;
        } else {
            return 0x29;
        }
    }

    static s32 getFn0x8010E5E0() {
        if (sInstance != nullptr) {
            return sInstance->fn_8010E5E0();
        } else {
            return 0x29;
        }
    }

    static s32 getFn0x8010E5D0() {
        if (sInstance != nullptr) {
            return sInstance->fn_8010E5D0();
        } else {
            return 0x5E;
        }
    }

private:
    s32 fn_8010E5D0() const;
    s32 fn_8010E5E0() const;

    static dLytDobutton_c *sInstance;

    STATE_FUNC_DECLARE(dLytDobutton_c, InvisibleWait);
    STATE_FUNC_DECLARE(dLytDobutton_c, InvisibleTimeCnt);
    STATE_FUNC_DECLARE(dLytDobutton_c, In);
    STATE_FUNC_DECLARE(dLytDobutton_c, Wait);
    STATE_FUNC_DECLARE(dLytDobutton_c, Out);

    UI_STATE_MGR_DECLARE(dLytDobutton_c);
    /* 0x040 */ d2d::dLytSub mLyt;
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
    /* 0x490 */ s32 field_0x490;
};

#endif
