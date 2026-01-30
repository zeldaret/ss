#ifndef D_LYT_METER_CROSS_BTN_H
#define D_LYT_METER_CROSS_BTN_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

// Size 0x80
class dLytMeterCrossBtnParts_c {
    friend class dLytMeterCrossBtn_c;

public:
    dLytMeterCrossBtnParts_c() : mStateMgr(*this) {}
    ~dLytMeterCrossBtnParts_c() {}

    void init();
    void execute(bool bIsVisible);

    bool isActive() const {
        return *mStateMgr.getStateID() == StateID_Active;
    }

private:
    void setMessage(s32 id);

    STATE_FUNC_DECLARE(dLytMeterCrossBtnParts_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterCrossBtnParts_c, On);
    STATE_FUNC_DECLARE(dLytMeterCrossBtnParts_c, Active);
    STATE_FUNC_DECLARE(dLytMeterCrossBtnParts_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterCrossBtnParts_c);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterCrossBtnParts_c);

    /* 0x3C */ d2d::LytBase_c *mpLyt;
    /* 0x40 */ d2d::AnmGroup_c *mpAnm[2];
    /* 0x48 */ dTextBox_c *mpTextBoxes[2];
    /* 0x50 */ dWindow_c *mpWindow;
    /* 0x54 */ dTextBox_c *mpSizeBox;
    /* 0x58 */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x5C */ s32 mIndex;
    /* 0x60 */ s32 mSavedMessage;
    /* 0x64 */ s32 mMessage;
    /* 0x68 */ s32 field_0x68;
    /* 0x6C */ s32 field_0x6C;
    /* 0x70 */ s32 mSavedIcon;
    /* 0x74 */ s32 mCurrentIcon;
    /* 0x78 */ s32 mOnDelay;
    /* 0x7C */ u8 field_0x7C;
};

class dLytMeterCrossBtn_c : public d2d::dSubPane {
public:
    enum CrossIcon_e {
        CROSS_ICON_TOP_1 = 1,
        CROSS_ICON_TOP_3 = 3,
        CROSS_ICON_TOP_4 = 4,

        CROSS_ICON_DOWN_0 = 0,
        CROSS_ICON_DOWN_2 = 2,
        CROSS_ICON_DOWN_5 = 5,

        CROSS_ICON_NONE = 6, 
    };

    dLytMeterCrossBtn_c() : mStateMgr(*this) {}
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

    virtual ~dLytMeterCrossBtn_c() {}

    CrossIcon_e getIconDown() const {
        return mIconDown;
    }

    void setIconDown(CrossIcon_e icon) {
        mIconDown = icon;
    }

    void setIconTop(CrossIcon_e icon) {
        mIconTop = icon;
    }

    void setOwnerPane(nw4r::lyt::Pane *pane);
    bool fn_800FA730() const;

private:
    STATE_FUNC_DECLARE(dLytMeterCrossBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterCrossBtn_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeterCrossBtn_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeterCrossBtn_c, Unuse);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterMinusBtn_c);

    /* 0x00C */ UI_STATE_MGR_DECLARE(dLytMeterCrossBtn_c);
    /* 0x044 */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnm[12];
    /* 0x3D8 */ nw4r::lyt::Pane *mpPane;
    /* 0x3DC */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x3E0 */ dTextBox_c *mpTextboxes[8];
    /* 0x400 */ dWindow_c *mpWindows[4];
    /* 0x410 */ dTextBox_c *mpSizeBoxes[4];
    /* 0x420 */ dLytMeterCrossBtnParts_c mParts[4];
    /* 0x620 */ CrossIcon_e mIconDown;
    /* 0x624 */ CrossIcon_e mIconTop;
    /* 0x628 */ u16 mSavedFiFlow;
    /* 0x62C */ s32 mCallCount0;
    /* 0x630 */ s32 mCallCount1;
    /* 0x634 */ u8 field_0x634;
    /* 0x635 */ u8 field_0x635;
    /* 0x636 */ u8 field_0x636;
    /* 0x637 */ u8 field_0x637;
    /* 0x638 */ u8 field_0x638;
    /* 0x639 */ u8 field_0x639;
    /* 0x63A */ u8 field_0x63A;
    /* 0x63B */ u8 field_0x63B;
    /* 0x63C */ u8 field_0x63C;
};

#endif
