#ifndef M_FADER_BASE_H
#define M_FADER_BASE_H

#include "common.h"
#include "m/m_color.h"

class mFaderBase_c {
public:
    enum EStatus {
        FADED_OUT = 0,
        FADED_IN = 1,
        FADING_IN = 2,
        FADING_OUT = 3,
    };

    enum EFlag {
        FLAG_1 = 1,
        FLAG_2 = 2,
    };

    mFaderBase_c(const mColor &color, EStatus status);
    /* vt 0x08 */ virtual ~mFaderBase_c();

    /* vt 0x0C */ virtual void setStatus(EStatus status) = 0;
    /* vt 0x10 */ virtual EStatus getStatus() const;
    /* vt 0x14 */ virtual bool fadeIn();
    /* vt 0x18 */ virtual bool fadeOut();
    /* vt 0x1C */ virtual bool calc();
    /* vt 0x20 */ virtual void draw() = 0;

    void setFrame(u16 frame);
    void setColor(const mColor &color);

    EStatus mStatus;
    u8 mFlag;
    u16 mFrame;
    u16 mElapsed;
    mColor mFaderColor;
};

#endif
