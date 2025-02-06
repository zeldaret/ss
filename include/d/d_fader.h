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

    void fadeIn() {
        setFader(mFadeInType);
        mpFader->setFrame(mFadeInFrame);
        mpFader->fadeIn();
    }

    void fadeOut() {
        setFader(mFadeOutType);
        mpFader->setFrame(mFadeOutFrame);
        mpFader->fadeOut();
    }

    void setTypes(s32 fadeInType, s32 fadeOutType) {
        mFadeInType = fadeInType;
        mFadeOutType = fadeOutType;
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
