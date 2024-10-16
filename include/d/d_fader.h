#ifndef D_FADER_H
#define D_FADER_H

#include "d/lyt/d_lyt_fader.h"
#include "d/lyt/d_screen_fader.h"
#include "m/m_color_fader.h"
#include "m/m_fader.h"

class dFader_c : mFader_c {
public:
    dFader_c() : mpColorFader(nullptr), mpLytFader(nullptr), mpScreenFader(nullptr) {}
    virtual ~dFader_c();

    bool create();
    bool setFader(s32 fader);
    u8 getColorFaderAlpha();
    bool initForLyt();

private:
    /* 0x08 */ mColorFader_c *mpColorFader;
    /* 0x0C */ dLytFader_c *mpLytFader;
    /* 0x10 */ dScreenFader_c *mpScreenFader;
    /* 0x14 */ UNKWORD field_0x14;
    /* 0x18 */ s32 mFadeInType;
    /* 0x1C */ s32 mFadeOutType;
    /* 0x20 */ u16 field_0x20;
    /* 0x22 */ u16 field_0x22;
};

#endif
