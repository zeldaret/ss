#ifndef D_LYT_METER_REMOCON_BG_H
#define D_LYT_METER_REMOCON_BG_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytMeterRemoconBg_c : public d2d::dSubPane {
public:
    dLytMeterRemoconBg_c() : mStateMgr(*this) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    virtual ~dLytMeterRemoconBg_c() {}

private:
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, On);
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, Active);
    STATE_FUNC_DECLARE(dLytMeterRemoconBg_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterRemoconBg_c);

    /* 0x08 */ UI_STATE_MGR_DECLARE(dLytMeterRemoconBg_c);
    /* 0x44 */ d2d::dLytSub mLyt;
    /* 0xD8 */ nw4r::lyt::Pane *mpPane[1];
    /* 0xDC */ u32 field_0xDC;
    /* 0xE0 */ u32 field_0xE0;
    /* 0xE4 */ u32 field_0xE4;
    /* 0xE8 */ u32 field_0xE8;
    /* 0xEC */ bool field_0xEC;
};

#endif
