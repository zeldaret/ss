#ifndef D_LYT_METER_NUN_STK_H
#define D_LYT_METER_NUN_STK_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytMeterNunStk_c : public d2d::dSubPane {
public:
    dLytMeterNunStk_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override;
    virtual d2d::LytBase_c *getLyt() override;
    virtual const char *getName() const override;

    virtual ~dLytMeterNunStk_c() {}
private:
    STATE_FUNC_DECLARE(dLytMeterNunStk_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterNunStk_c, On);
    STATE_FUNC_DECLARE(dLytMeterNunStk_c, Active);
    STATE_FUNC_DECLARE(dLytMeterNunStk_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterNunStk_c);
    d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnmGroups[2];
    /* 0x??? */ u8 padding[0x12350 - 0x12320];
};

#endif
