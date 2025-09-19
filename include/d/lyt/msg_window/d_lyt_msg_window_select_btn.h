#ifndef D_LYT_MSG_WINDOW_SELECT_BTN_H
#define D_LYT_MSG_WINDOW_SELECT_BTN_H

#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

// Size 0x54?
struct SelectBtnHelper {
    SelectBtnHelper();
    ~SelectBtnHelper();

    /**
     * Describes number of buttons and their layout.
     */
    enum SelectMode_e {
        /** Two buttons in the order right, left */
        MODE_2,
        /** Three buttons in the order up, right, left */
        MODE_3_UP,
        /** 4 buttons in an X formation in the order top right, bottom right, top left, bottom left */
        MODE_4,
        /** Three buttons in the order right, down, left */
        MODE_3_DOWN,
    };

    enum ExecuteResult_e {
        /** Nothing of interest happened */
        EXECUTE_NONE,
        /** Selected a pane using the Wiimote motion / pointing */
        EXECUTE_SWITCH_CURSOR,
        /** Selected a pane using the Nunchuk stick */
        EXECUTE_SWITCH_STICK,
        /** Not sure, unreachable */
        EXECUTE_RESET_CURSOR,
    };

    /* 0x00 */ nw4r::lyt::Pane *panes[9];
    /* 0x24 */ mVec2_c field_0x24;
    /* 0x2C */ mVec2_c field_0x2C;
    /* 0x34 */ f32 mAngX;
    /* 0x38 */ f32 mAngY;
    /* 0x3C */ f32 mAngXCenter;
    /* 0x40 */ f32 mAngYCenter;
    /* 0x44 */ s32 mCursorSelectTimer;
    /* 0x48 */ s32 mButtonMode;
    /* 0x4C */ s32 field_0x4C;
    /* 0x50 */ s8 mSelectedBtnIdx;
    /* 0x51 */ bool mIsCursorActive;
    /* 0x52 */ bool mIsNavEnabled;

    void init();
    void remove();
    void resetCursor();
    u8 execute();
    bool handleCursorInput();
    bool handleButtonInput();
    void fn_8011E110(mVec2_c screenPos) const;
    void convertScreenPosToDpdPos(mVec2_c *pOutDpd, mVec2_c *screenPos) const;

    s8 calculatePointedAtButton(f32 x, f32 y);

    /**
     * Checks whether the Nunchuk stick button has just been pushed in
     * a direction that should cause a new pane to be selected.
     */
    bool hasNewFSStickButtonSelect() const;

    /**
     * Gets the angle of the button in degrees. 0° = up, counterclockwise.
     */
    f32 getAngleForButtonIdx(s32 idx) const;

    const nw4r::lyt::Pane *getRootPane() const {
        return panes[0];
    }

    const nw4r::lyt::Pane *getPosPane(s32 idx) const {
        s32 paneIdx = 1 + idx;
        return panes[paneIdx];
    }

    const nw4r::lyt::Pane *getBounding(s32 idx) const {
        return panes[5 + idx];
    }

    s8 getSelectedtBtnIdx() const {
        return mSelectedBtnIdx;
    }
};

class dLytMsgWindowSelectBtnParts_c {
    friend class dLytMsgWindowSelectBtn_c;

public:
    dLytMsgWindowSelectBtnParts_c() : mStateMgr(*this) {}
    ~dLytMsgWindowSelectBtnParts_c() {}

    void init();
    void execute();

private:
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtnParts_c, Wait);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtnParts_c, On);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtnParts_c, Select);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtnParts_c, Off);

    STATE_MGR_DEFINE_UTIL_INSTANCIATE_STATE(dLytMsgWindowSelectBtnParts_c);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMsgWindowSelectBtnParts_c);
    /* 0x3C */ d2d::AnmGroup_c *mpAnms[3];
    /* 0x48 */ u32 mShouldBeOn;
    /* 0x4C */ u32 field_0x4C;
    /* 0x50 */ u8 field_0x50;
};

class dLytMsgWindowSelectBtn_c : public d2d::dSubPane {
public:
    dLytMsgWindowSelectBtn_c() : mStateMgr(*this) {}
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

    virtual ~dLytMsgWindowSelectBtn_c() {}

    bool draw();

    bool isVisible() const {
        return mIsVisible;
    }

    s32 getConfirmedBtnIdx() const {
        return mConfirmedBtnIdx;
    }

    s32 getDecidedBtnIdx() const {
        return mDecidedBtnIdx;
    }

    void setField_0x9B8(s32 value) {
        field_0x9B8 = value;
    }

    void setCancelBtnIdx(s32 value) {
        mCancelBtnIdx = value;
    }

    void setFlipBtnLayout(s32 value) {
        mFlipBtnLayout = value;
    }

    void setField_0x990(s32 value) {
        mBtnHelper.field_0x4C = value;
    }

    void setInSound(s32 value) {
        mInSound = value;
    }

    void setTagProcessor(dTagProcessor_c *value) {
        mpTagProcessor = value;
    }

    void setSkipOutAnim(s32 value) {
        mSkipOutAnim = value;
    }

    void requestIn(s32 numBtns, bool playSound);

    dTextBox_c *getSelectTextBox(s32 option, s32 idx) const {
        return mpSelectTextBoxes[option][idx];
    }

    BOOL isStateWait() {
        return *mStateMgr.getStateID() == StateID_Wait;
    }

protected:
    s32 getSelectBtnMode();

    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, In);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, WaitSelect);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, CursorInOut);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, WaitDecide);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, WaitCancel);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, Out);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytMsgWindowSelectBtn_c);
    /* 0x044 */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnm[27];
    /* 0x798 */ nw4r::lyt::Pane *mpSelectPanes[7];
    /* 0x7B4 */ dTextBox_c *mpSelectTextBoxes[4][2];
    /* 0x7D4 */ dTextBox_c *mpDecideTextBoxes[2];
    /* 0x7DC */ nw4r::lyt::Pane *mpBoundings[4];
    /* 0x7EC */ dWindow_c *mpWindow[1];
    /* 0x7F0 */ dTextBox_c *mpSizeBox[1];
    /* 0x7F4 */ dLytMsgWindowSelectBtnParts_c mParts[4];
    /* 0x944 */ SelectBtnHelper mBtnHelper;
    /* 0x998 */ dTagProcessor_c *mpTagProcessor;
    /* 0x99C */ s32 mSkipOutAnim;
    /* 0x9A0 */ s32 mInSound;
    /* 0x9A4 */ bool mIsVisible;
    /* 0x9A8 */ s32 mNumBtns;
    /* 0x9AC */ s32 field_0x9AC;
    /* 0x9B0 */ s32 mConfirmedBtnIdx;
    /* 0x9B0 */ s32 mDecidedBtnIdx;
    /* 0x9B8 */ s32 field_0x9B8;
    /* 0x9BC */ s32 mCancelBtnIdx;
    /* 0x9C0 */ s32 mWaitCancelTimer;
    /* 0x9C4 */ s32 mFlipBtnLayout;
    /* 0x9C8 */ s32 mStep;
    /* 0x9CC */ s8 mCanceledViaBBtnIdx;
    /* 0x9CD */ u8 field_0x9CD;
    /* 0x9CE */ u8 field_0x9CE;
    /* 0x9CF */ bool mPlayInSound;
    /* 0x9D0 */ bool field_0x9D0;
    /* 0x9D1 */ u8 field_0x9D1;
};

#endif
