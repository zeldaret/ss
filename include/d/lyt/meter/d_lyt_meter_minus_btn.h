#ifndef D_LYT_METER_MINUS_BTN_H
#define D_LYT_METER_MINUS_BTN_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"
#include "toBeSorted/effects_struct.h"

// At 0x8, [8], size 0x48
class dLytMeterMinusBtnMenuIcon_c {
public:
    dLytMeterMinusBtnMenuIcon_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeterMinusBtnMenuIcon_c() {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

    void setShouldBeSelect(u32 v) {
        mShouldBeSelect = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterMinusBtnMenuIcon_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnMenuIcon_c, On);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnMenuIcon_c, Select);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnMenuIcon_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterMinusBtnMenuIcon_c);

    /* 0x04 */ UI_STATE_MGR_DECLARE(dLytMeterMinusBtnMenuIcon_c);

    /* 0x40 */ d2d::AnmGroup_c *mpAnm;
    /* 0x44 */ u32 mShouldBeSelect;
};

// At 0x248, standalone
class dLytMeterMinusBtnBlink_c {
public:
    dLytMeterMinusBtnBlink_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterMinusBtnBlink_c() {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

    void setShouldBeSelect(u32 v) {
        mShouldBeSelect = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterMinusBtnBlink_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnBlink_c, On);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnBlink_c, Select);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnBlink_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterMinusBtnBlink_c);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterMinusBtnBlink_c);

    /* 0x3C */ d2d::AnmGroup_c *mpAnm;
    /* 0x40 */ u32 mShouldBeSelect;
};

// At 0x28C, [2], size 0x44
class dLytMeterMinusBtnText_c {
public:
    dLytMeterMinusBtnText_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterMinusBtnText_c() {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

    void setShouldBeSelect(u32 v) {
        mShouldBeSelect = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterMinusBtnText_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnText_c, On);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnText_c, Select);
    STATE_FUNC_DECLARE(dLytMeterMinusBtnText_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterMinusBtnText_c);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterMinusBtnText_c);

    /* 0x3C */ d2d::AnmGroup_c *mpAnm;
    /* 0x40 */ u32 mShouldBeSelect;
};

class dLytMeterMinusBtn_c : public d2d::dSubPane {
public:
    dLytMeterMinusBtn_c() : mStateMgr(*this, sStateID::null) {}
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

    virtual ~dLytMeterMinusBtn_c() {}

    bool demoRelated(s32 arg);

    void setOwnerPane(nw4r::lyt::Pane *pane) {
        mpOwnerPane = pane;
    }

    void setField_0x4E80(s32 value) {
        field_0x4E80 = value;
    }
    
    bool fn_800F75E0() const;

private:
    bool shouldCall() const;
    void fn_800F7300();
    bool fn_800F7600() const;
    bool fn_800F7760() const;
    void realizeShieldDurability();
    bool isUnequippable(s32 num) const;
    void setItemText(s32 item);
    bool isNotMedalOrExtraAmmo(s32 item);

    nw4r::lyt::Pane *getPaneByIndex(s32 idx) const {
        return mpPanes[idx];
    }

    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, MenuSelectingIn);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, MenuSelecting);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, MenuSelectingOut);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, Unuse);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, DemoMove);
    STATE_FUNC_DECLARE(dLytMeterMinusBtn_c, DemoOut);

    STATE_MGR_DEFINE_UTIL_ISSTATE(dLytMeterMinusBtn_c);
    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterMinusBtn_c);

    /* 0x0008 */ dLytMeterMinusBtnMenuIcon_c mIcon[8];
    /* 0x0244 */ dLytMeterMinusBtnBlink_c mBlink;
    /* 0x028C */ dLytMeterMinusBtnText_c mText[2];
    /* 0x0314 */ UI_STATE_MGR_DECLARE(dLytMeterMinusBtn_c);
    /* 0x0350 */ d2d::dLytSub mLyt;
    /* 0x03E4 */ d2d::AnmGroup_c mAnm[33];
    /* 0x0C24 */ nw4r::lyt::Pane *mpPanes[28];
    /* 0x0C94 */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x0C98 */ dWindow_c *mpWindows[2];
    /* 0x0CA0 */ dTextBox_c *mpSizeBoxes[2];
    /* 0x0CA8 */ dTextBox_c *mpTextBoxes[2];
    /* 0x0CB0 */ dLytCommonIconItem_c mItemIcons[8];
    /* 0x4CF0 */ d2d::SubPaneList mSubpaneList;
    /* 0x4CFC */ d2d::SubPaneListNode mSubpanes[8];
    /* 0x4D7C */ EffectsStruct mEffects[4];
    /* 0x4E4C */ mAng3_c mEffectsRot[4];
    /* 0x4E64 */ s32 mSlotForEffect[4];
    /* 0x4E74 */ f32 mArrowRotation;
    /* 0x4E78 */ f32 mArrowLength;
    /* 0x4E7C */ s32 field_0x4E7C;
    /* 0x4E80 */ s32 field_0x4E80;
    /* 0x4E84 */ s32 field_0x4E84;
    /* 0x4E88 */ s32 field_0x4E88;
    /* 0x4E8C */ s32 field_0x4E8C;
    /* 0x4E90 */ s32 mDemoFrame;
    /* 0x4E94 */ s32 mSlotToDemo;
    /* 0x4E98 */ s32 field_0x4E98[8];
    /* 0x4EB8 */ u8 field_0x4EB8;
    /* 0x4EB9 */ u8 field_0x4EB9;
    /* 0x4EBA */ u8 field_0x4EBA;
    /* 0x4EBB */ u8 field_0x4EBB[8];
    /* 0x4EC3 */ u8 field_0x4EC3;
};

#endif
