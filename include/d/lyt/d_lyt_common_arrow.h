#ifndef D_LYT_COMMON_ARROW_H
#define D_LYT_COMMON_ARROW_H

#include "d/lyt/d2d.h"
// clang-format off
// vtable order
#include "d/d_cursor_hit_check.h"
// clang-format on
#include "nw4r/lyt/lyt_bounding.h"
#include "s/s_State.hpp"

class dLytCommonArrow_c {
public:
    dLytCommonArrow_c();
    virtual ~dLytCommonArrow_c() {}

    bool build();

    STATE_FUNC_DECLARE(dLytCommonArrow_c, None);
    STATE_FUNC_DECLARE(dLytCommonArrow_c, In);
    STATE_FUNC_DECLARE(dLytCommonArrow_c, Wait);
    STATE_FUNC_DECLARE(dLytCommonArrow_c, Out);

    void setState(s32 state);
    bool draw();

    bool remove();
    bool execute();
    bool requestIn();
    bool requestOut();

private:
    void setTranslate(const Vec *);
    bool fn_80168760();
    void displayElement(s32, f32);
    void unbindAt(s32);
    void tickDown(d2d::AnmGroup_c *);
    void fn_80168880();
    
    /* 0x04 */ UI_STATE_MGR_DECLARE(dLytCommonArrow_c);
    /* 0x40 */ d2d::LytBase_c mLytBase;
    d2d::ResAccIf_c mResAcc;
    d2d::AnmGroup_c mAnmGroups[9];
    dCursorHitCheckLyt_c mCsHitCheck;
    /* 0x6A8 */ nw4r::lyt::Bounding *mpBoundings[2];
    /* 0x6B0 */ s32 mType;
    /* 0x6B4 */ s32 mTargetedBounding;
    /* 0x6B8 */ s32 field_0x6B8;
    /* 0x6BC */ s32 field_0x6BC;
    /* 0x6C0 */ s32 field_0x6C0;
    /* 0x6C4 */ s32 mTimer;
    /* 0x6C8 */ bool mInRequested;
    /* 0x6C9 */ bool mOutRequested;
    /* 0x6CA */ u8 field_0x6CA;
    /* 0x6CB */ u8 field_0x6CB;
    /* 0x6CC */ u8 field_0x6CC;
};

#endif
