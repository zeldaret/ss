#ifndef D_FADER_H
#define D_FADER_H

#include "d/lyt/d_lyt_fader.h"
#include "d/lyt/d_screen_fader.h"
#include "m/m_color_fader.h"
#include "m/m_fader.h"

class dFader_c : public mFader_c {
public:
    dFader_c() : mpColorFader(nullptr), mpLytFader(nullptr), mpScreenFader(nullptr), field_0x14(0) {}
    virtual ~dFader_c();

    bool create();
    bool setFader(s32 fader);
    u8 getColorFaderAlpha();
    bool initForLyt();

    bool fadeIn() {
        setFader(mFadeInType);
        mpFader->setFrame(mFadeInFrame);
        return mpFader->fadeIn();
    }

    bool fadeOut() {
        setFader(mFadeOutType);
        mpFader->setFrame(mFadeOutFrame);
        return mpFader->fadeOut();
    }

    void setTypes(s32 fadeInType, s32 fadeOutType) {
        mFadeInType = fadeInType;
        mFadeOutType = fadeOutType;
    }

    void setFadeInType(s32 fadeInType) {
        mFadeInType = fadeInType;
    }

    void setFadeOutType(s32 fadeOutType) {
        mFadeOutType = fadeOutType;
    }

    void resetFrames() {
        mFadeInFrame = 30;
        mFadeOutFrame = 30;
    }

    void setFrames(u16 fadeIn, u16 fadeOut) {
        mFadeInFrame = fadeIn;
        mFadeOutFrame = fadeOut;
    }

    u16 getFadeOutFrame() const {
        return mFadeOutFrame;
    }

    void setFadeInFrame(u16 frame) {
        mFadeInFrame = frame;
    }

    void setFadeOutFrame(u16 frame) {
        mFadeOutFrame = frame;
    }

private:
    /* 0x08 */ mColorFader_c *mpColorFader;
    /* 0x0C */ dLytFader_c *mpLytFader;
    /* 0x10 */ dScreenFader_c *mpScreenFader;
    /* 0x14 */ u8 field_0x14;
    /* 0x18 */ s32 mFadeInType;
    /* 0x1C */ s32 mFadeOutType;
    /* 0x20 */ u16 mFadeInFrame;
    /* 0x22 */ u16 mFadeOutFrame;
};

#endif
