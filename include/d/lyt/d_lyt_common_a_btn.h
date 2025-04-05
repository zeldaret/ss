#ifndef D_LYT_COMMON_A_BTN_H
#define D_LYT_COMMON_A_BTN_H

#include "d/lyt/d2d.h"

/**
 * Made up name.
 *
 * An A button used in various message windows, as well as
 * the title menu. Can be hidden, visible, animating in, animating out.
 */
class dLytCommonABtn_c : public d2d::dSubPane {
public:
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

    bool isDoneOut() const {
        return mIsDoneOut;
    }

    bool requestIn();
    bool requestOut(bool b);

    enum SoundVariant {
        VARIANT_NORMAL,
        VARIANT_TITLE,
    };

    void setSoundVariant(SoundVariant);
    void resetToInvisble();

    enum ABtnState {
        STATE_INVISIBLE,
        STATE_IN,
        STATE_VISIBLE,
        STATE_OUT,
    };

private:
    enum OutState {
        OUT_STATE_DECIDING,
        OUT_STATE_GOTO_OUT,
        OUT_STATE_OUT,
        OUT_STATE_GOTO_INVISIBLE,
    };

    void setState(ABtnState state);

    void runExecuteFunc();

    void executeStateInvisible();
    void executeStateIn();
    void executeStateVisible();
    void executeStateDecideOut();

    void gotoStateInvisible();
    void gotoStateIn();
    void goToStateVisible();
    void goToStateDecideOut();

    typedef void (dLytCommonABtn_c::*executeFunc)(void);
    static const executeFunc sExecuteFuncs[4];

    /* 0x008 */ d2d::dLytSub mLyt;
    /* 0x09C */ d2d::AnmGroup_c mAnm[4];
    /* 0x19C */ ABtnState mState;
    /* 0x1A0 */ OutState mOutState;
    /* 0x1A4 */ SoundVariant mSoundVariant;
    /* 0x1A8 */ u8 field_0x1A8;
    /* 0x1A9 */ bool mInRequested;
    /* 0x1AA */ bool mOutRequested;
    /* 0x1AB */ bool mIsDoneOut;
    /* 0x1AC */ u8 field_0x1AC;
    /* 0x1AD */ bool mNoDecide;
    /* 0x1AE */ u8 field_0x1AE;
    /* 0x1AF */ u8 field_0x1AF;
};

#endif
