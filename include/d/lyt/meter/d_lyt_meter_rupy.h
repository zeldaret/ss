#ifndef D_LYT_METER_RUPY_H
#define D_LYT_METER_RUPY_H

#include "d/lyt/d2d.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytMeterRupyStart_c {
public:
    dLytMeterRupyStart_c() : mStateMgr(*this, sStateID::null) {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anm) {
        mpAnm = anm;
    }

    void setShouldStart(bool b) {
        mShouldStart = b;
    }

    bool isStarting() const {
        return mShouldStart;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterRupyStart_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterRupyStart_c, Start);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterRupyStart_c);

    UI_STATE_MGR_DECLARE(dLytMeterRupyStart_c);

    d2d::AnmGroup_c *mpAnm;
    bool mShouldStart;
};

class dLytMeterRupyBlink_c {
public:
    dLytMeterRupyBlink_c() : mStateMgr(*this, sStateID::null) {}

    void init();
    void execute();

    void setAnm(d2d::AnmGroup_c *anm) {
        mpAnm = anm;
    }

    void doFlash() {
        mShouldFlash = true;
    }

    void cancelFlash() {
        mImmediatelyCancelFlash = true;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterRupyBlink_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterRupyBlink_c, Flash);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterRupyBlink_c);

    UI_STATE_MGR_DECLARE(dLytMeterRupyBlink_c);

    d2d::AnmGroup_c *mpAnm;
    bool mShouldFlash;
    bool mImmediatelyCancelFlash;
};

class dLytMeterRupy_c : public d2d::dSubPane {
public:
    dLytMeterRupy_c() : mStateMgr(*this, sStateID::null) {}
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

    virtual ~dLytMeterRupy_c() {}

    mVec3_c getLastVisibleDigitPosition();

    void setField_0x8A9(u8 val) {
        field_0x8A9 = val;
    }

    u8 getField_0x8A9() const {
        return field_0x8A9;
    }

    void setField_0x8AA(u8 val) {
        field_0x8AA = val;
    }

    u8 getField_0x8AC() const {
        return field_0x8AC;
    }

    void setField_0x8AC(u8 val) {
        field_0x8AC = val;
    }

    void setField_0x8AD(u8 val) {
        field_0x8AD = val;
    }

    void setSize(u8 size) {
        mSize = size;
    }

private:
    bool updateDisplayedAmount(bool suppressSound);
    bool hasChangeInRupees() const;
    s32 getRupeeDifference() const;
    void setDigit(s32 index, s32 digit);
    void executeDigitJump();
    void executeDigitBlink(s32 amount);

    STATE_FUNC_DECLARE(dLytMeterRupy_c, In);
    STATE_FUNC_DECLARE(dLytMeterRupy_c, Active);
    STATE_FUNC_DECLARE(dLytMeterRupy_c, Start);
    STATE_FUNC_DECLARE(dLytMeterRupy_c, Out);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterRupy_c);

    UI_STATE_MGR_DECLARE(dLytMeterRupy_c);

    /* 0x044 */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnm[22];
    /* 0x658 */ dLytMeterRupyStart_c mStartParts[4];
    /* 0x768 */ dLytMeterRupyBlink_c mBlinkParts[4];
    /* 0x878 */ nw4r::lyt::Pane *mpPanes[4];
    /* 0x888 */ s32 mNumDisplayedDigits;
    /* 0x88C */ s32 mDisplayedRupeeCount;
    /* 0x890 */ s32 field_0x890;
    /* 0x894 */ s32 mBlinkDelay;
    /* 0x898 */ s32 mRupeeColor;
    /* 0x89C */ u8 mSize;
    /* 0x89D */ u8 mDisplayedDigits[4];
    /* 0x8A1 */ u8 mPrevDigits[4];
    /* 0x8A5 */ u8 mJumpState[4];
    /* 0x8A9 */ u8 field_0x8A9;
    /* 0x8AA */ u8 field_0x8AA;
    /* 0x8AB */ u8 field_0x8AB;
    /* 0x8AC */ u8 field_0x8AC;
    /* 0x8AD */ u8 field_0x8AD;
    /* 0x8AE */ u8 field_0x8AE;
};

#endif
