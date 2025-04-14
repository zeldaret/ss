#ifndef D_LYT_METER_DOWSING_H
#define D_LYT_METER_DOWSING_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item.h"
#include "d/lyt/d_textbox.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytMeterDowsingMenuIcon_c {
public:
    dLytMeterDowsingMenuIcon_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeterDowsingMenuIcon_c() {}

    void init();
    void execute();
    void reset();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

    void setShouldBeSelect(u32 v) {
        mShouldBeSelect = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterDowsingMenuIcon_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsingMenuIcon_c, On);
    STATE_FUNC_DECLARE(dLytMeterDowsingMenuIcon_c, Select);
    STATE_FUNC_DECLARE(dLytMeterDowsingMenuIcon_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterDowsingMenuIcon_c);

    /* 0x04 */ UI_STATE_MGR_DECLARE(dLytMeterDowsingMenuIcon_c);

    /* 0x40 */ d2d::AnmGroup_c *mpAnm;
    /* 0x44 */ u32 mShouldBeSelect;
};

class dLytMeterDowsingBlink_c {
public:
    dLytMeterDowsingBlink_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDowsingBlink_c() {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

    void setShouldBeSelect(u32 v) {
        mShouldBeSelect = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterDowsingBlink_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsingBlink_c, On);
    STATE_FUNC_DECLARE(dLytMeterDowsingBlink_c, Select);
    STATE_FUNC_DECLARE(dLytMeterDowsingBlink_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterDowsingBlink_c);

    /* 0x04 */ UI_STATE_MGR_DECLARE(dLytMeterDowsingBlink_c);

    /* 0x40 */ d2d::AnmGroup_c *mpAnm;
    /* 0x44 */ u32 mShouldBeSelect;
};

class dLytMeterDowsingInput_c {
public:
    dLytMeterDowsingInput_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDowsingInput_c() {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

    void setShouldBeSelect(u32 v) {
        mShouldBeSelect = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterDowsingInput_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsingInput_c, On);
    STATE_FUNC_DECLARE(dLytMeterDowsingInput_c, Select);
    STATE_FUNC_DECLARE(dLytMeterDowsingInput_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterDowsingInput_c);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterDowsingInput_c);

    /* 0x3C */ d2d::AnmGroup_c *mpAnm;
    /* 0x40 */ u32 mShouldBeSelect;
};

class dLytMeterDowsingText_c {
public:
    dLytMeterDowsingText_c() : mStateMgr(*this, sStateID::null) {}
    ~dLytMeterDowsingText_c() {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anim) {
        mpAnm = anim;
    }

    void setShouldBeSelect(u32 v) {
        mShouldBeSelect = v;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterDowsingText_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsingText_c, On);
    STATE_FUNC_DECLARE(dLytMeterDowsingText_c, Select);
    STATE_FUNC_DECLARE(dLytMeterDowsingText_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterDowsingText_c);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterDowsingText_c);

    /* 0x3C */ d2d::AnmGroup_c *mpAnm;
    /* 0x40 */ u32 mShouldBeSelect;
};

class dLytMeterDowsing_c : public d2d::dSubPane {
public:
    dLytMeterDowsing_c() : mStateMgr(*this, sStateID::null) {}
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

    virtual ~dLytMeterDowsing_c() {}

    bool shouldCall() const;
    bool fn_800FE3C0(s32 arg);
    bool fn_800FE490() const;
    bool fn_800FE4B0() const;
    bool fn_800FE610();

    u8 getField_0x550A() const {
        return field_0x550A;
    }

private:
    void setItemName(s32 id);
    void setMessage(s32 id);
    u8 getSelectedDowsingSlot() const;
    void fn_800FE110();
    void fn_800FE220();
    s32 convertToLytIndex(s32 slot) const;
    s32 convertSlotIndexToAnm(s32 slot) const;
    s32 fn_800FE9C0(s32) const;

    nw4r::lyt::Pane *getPaneByIndex(s32 idx) const {
        return mpPanes[idx];
    }

    STATE_FUNC_DECLARE(dLytMeterDowsing_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, MenuSelectingIn);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, MenuSelecting);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, MenuSelectingOut);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, Reset);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, Unuse);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, DemoMove);
    STATE_FUNC_DECLARE(dLytMeterDowsing_c, DemoOut);

    STATE_MGR_DEFINE_UTIL_ISSTATE(dLytMeterDowsing_c);
    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterDowsing_c);

    /* 0x0008 */ dLytMeterDowsingMenuIcon_c mIcon[8];
    /* 0x0248 */ dLytMeterDowsingBlink_c mBlink;
    /* 0x028C */ dLytMeterDowsingInput_c mInput;
    /* 0x02D0 */ dLytMeterDowsingText_c mText[2];
    /* 0x0358 */ UI_STATE_MGR_DECLARE(dLytMeterDowsing_c);

    /* 0x0490 */ d2d::dLytSub mLyt;

    /* 0x0428 */ d2d::AnmGroup_c mAnm[29];
    /* 0x0B68 */ nw4r::lyt::Pane *mpPanes[22];
    /* 0x0BC0 */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x0BC4 */ dWindow_c *mpWindows[3];
    /* 0x0BD0 */ dTextBox_c *mpSizeBoxes[3];
    /* 0x0BDC */ dTextBox_c *mpTextBoxes[4];
    /* 0x0BEC */ dLytCommonIconItem_c mItemIcons[9];

    /* 0x5434 */ d2d::SubPaneList mSubpaneList;
    /* 0x5440 */ d2d::SubPaneListNode mSubpanes[9];
    /* 0x54D0 */ f32 mArrowRotation;
    /* 0x54D4 */ f32 mArrowLength;
    /* 0x54D8 */ s32 field_0x54D8;
    /* 0x54D8 */ s32 field_0x54DC;
    /* 0x54E0 */ s32 field_0x54E0;
    /* 0x54E4 */ s32 field_0x54E4;
    /* 0x54E8 */ s32 field_0x54E8;
    /* 0x54EC */ s32 field_0x54EC;
    /* 0x54F0 */ s32 field_0x54F0;
    /* 0x54F4 */ s32 field_0x54F4;
    /* 0x54F8 */ u32 field_0x54F8;
    /* 0x54FC */ s32 mDemoFrame;
    /* 0x5500 */ s32 field_0x5500;
    /* 0x5504 */ u8 field_0x5504;
    /* 0x5505 */ u8 field_0x5505;
    /* 0x5506 */ u8 field_0x5506;
    /* 0x5507 */ u8 field_0x5507;
    /* 0x5508 */ u8 field_0x5508;
    /* 0x5509 */ u8 field_0x5509;
    /* 0x550A */ u8 field_0x550A;
    /* 0x550B */ u8 field_0x550B;
};

#endif
