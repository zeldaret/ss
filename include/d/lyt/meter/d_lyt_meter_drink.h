#ifndef D_LYT_METER_DRINK_H
#define D_LYT_METER_DRINK_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

// Size 0x54
class dLytMeterDrinkParts_c {
    friend class dLytMeterDrink_c;

    enum DrinkType_e {
        TYPE_STAMINA,
        TYPE_AIR,
        TYPE_GUARDIAN,
        TYPE_NONE,
    };

    enum DrinkFrame_e {
        FRAME_STAMINA,
        FRAME_STAMINA_PLUS,
        FRAME_AIR,
        FRAME_AIR_PLUS,
        FRAME_GUARDIAN,
        FRAME_GUARDIAN_PLUS,
        FRAME_NONE
    };

public:
    dLytMeterDrinkParts_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDrinkParts_c() {}

    void init();
    void execute();

    bool isDrinkAboutToExpire() const;
    DrinkFrame_e getDrinkFrame(DrinkType_e ty) const;
    void copy(dLytMeterDrinkParts_c *other, dLytMeterDrinkParts_c *third);

private:
    STATE_FUNC_DECLARE(dLytMeterDrinkParts_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterDrinkParts_c, In);
    STATE_FUNC_DECLARE(dLytMeterDrinkParts_c, Visible);
    STATE_FUNC_DECLARE(dLytMeterDrinkParts_c, Out);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterDrinkParts_c);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterDrinkParts_c);
    /* 0x03C */ d2d::AnmGroup_c *mAnmGroups[3];
    /* 0x048 */ s32 field_0x48;
    /* 0x04C */ DrinkType_e mDrinkType;
    /* 0x050 */ u8 field_0x50;
    /* 0x051 */ u8 field_0x51;
};

class dLytMeterDrink_c {
public:
    dLytMeterDrink_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDrink_c() {}

    virtual d2d::dLytSub *getLyt() {
        return &mLyt;
    }
    virtual const char *getName() {
        return mLyt.getName();
    }

    bool build(d2d::ResAccIf_c *);
    bool remove();
    bool draw();
    bool execute();

private:
    bool fn_800E5C40(int);
    int getPartForDrinkType(dLytMeterDrinkParts_c::DrinkType_e ty) const;
    void executeInternal();

    STATE_FUNC_DECLARE(dLytMeterDrink_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterDrink_c, In);
    STATE_FUNC_DECLARE(dLytMeterDrink_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDrink_c, Move);
    STATE_FUNC_DECLARE(dLytMeterDrink_c, Out);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterDrink_c);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMeterDrink_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnmGroups[20];
    /* 0x5D4 */ nw4r::lyt::Pane *mpPartPanes[3];
    /* 0x5E0 */ nw4r::lyt::Pane *mpPane;
    /* 0x5E4 */ dLytMeterDrinkParts_c mParts[3];
    /* 0x6E0 */ mVec3_c field_0x6E0;
    /* 0x6EC */ UNKWORD field_0x6EC;
    /* 0x6F0 */ s32 field_0x6F0;
    /* 0x6F4 */ u8 field_0x6F4;
    /* 0x6F5 */ u8 field_0x6F5[3];
};

#endif
