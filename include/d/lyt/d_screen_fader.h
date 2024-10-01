#ifndef D_SCREEN_FADER_H
#define D_SCREEN_FADER_H

#include <m/m_fader_base.h>

// A fader that closes and opens vertically like a camera shutter with two blades
class dScreenFader_c : public mFaderBase_c {
public:
    dScreenFader_c(const mColor &color, EStatus status);
    virtual ~dScreenFader_c();

    virtual void setStatus(EStatus status) override;
    virtual bool fadeIn() override;
    virtual bool fadeOut() override;
    virtual bool calc() override;
    virtual void draw() override;

private:
    /* 0x014 */ f32 mProgress;
};

#endif
