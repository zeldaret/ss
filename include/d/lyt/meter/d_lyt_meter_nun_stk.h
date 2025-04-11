#ifndef D_LYT_METER_NUN_STK_H
#define D_LYT_METER_NUN_STK_H

#include "d/lyt/d2d.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytMeterNunStk_c : public d2d::dSubPane {
public:
    dLytMeterNunStk_c() : mStateMgr(*this, sStateID::null) {}
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

    nw4r::lyt::Pane *getBgPane() {
        return mpPane;
    }

    virtual ~dLytMeterNunStk_c() {}

private:
    void setMessage(s32 id);

    STATE_FUNC_DECLARE(dLytMeterNunStk_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterNunStk_c, On);
    STATE_FUNC_DECLARE(dLytMeterNunStk_c, Active);
    STATE_FUNC_DECLARE(dLytMeterNunStk_c, Off);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytMeterNunStk_c);
    /* 0x044 */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnm[2];
    /* 0x158 */ nw4r::lyt::Pane *mpPane;
    /* 0x15C */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x160 */ dTextBox_c *mpTextBoxes[2];
    /* 0x168 */ dWindow_c *mpWindow[1];
    /* 0x16C */ dTextBox_c *mpSizeBox[1];

    /* 0x170 */ s32 field_0x170;
    /* 0x174 */ s32 field_0x174;
    /* 0x178 */ BOOL field_0x178;
    /* 0x17C */ s32 field_0x17C;
    /* 0x180 */ s32 mOnDelay;
    /* 0x184 */ bool field_0x184;
    /* 0x185 */ u8 field_0x185;
};

#endif
