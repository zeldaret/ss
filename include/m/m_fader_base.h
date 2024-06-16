#ifndef M_FADER_BASE_H
#define M_FADER_BASE_H

#include <common.h>
#include <m/m_color.h>


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
    virtual ~mFaderBase_c();

    virtual void setStatus(EStatus status) = 0;
    virtual EStatus getStatus() const;
    virtual bool fadeIn();
    virtual bool fadeOut();
    virtual u8 calc();
    virtual void draw() = 0;

    void setFrame(u16 frame);
    void setColor(const mColor &color);

    EStatus mStatus;
    u8 mFlag;
    u16 mFrame;
    u16 mElapsed;
    mColor mFaderColor;
};

#endif
