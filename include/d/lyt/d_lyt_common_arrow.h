#ifndef D_LYT_COMMON_ARROW_H
#define D_LYT_COMMON_ARROW_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_structd.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

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
    UI_STATE_MGR_DECLARE(dLytCommonArrow_c);
    /* 0x40 */ d2d::LytBase_c mLytBase;
    d2d::ResAccIf_c mResAcc;
    d2d::AnmGroup_c mAnmGroups[0x9];
    d2d::dLytStructD mStructD;
    nw4r::lyt::Bounding *mBoundingL;
    nw4r::lyt::Bounding *mBoundingR;
    s32 mType;
    s32 field_0x6B4;
    s32 field_0x6B8;
    s32 field_0x6BC;
    s32 field_0x6C0;
    s32 mTimer;
    u8 mInRequested;
    u8 mOutRequested;
    u8 field_0x6CA;
    u8 field_0x6CB;
    u8 field_0x6CC;
};

#endif
