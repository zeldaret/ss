#ifndef M_COLOR_FADER_H
#define M_COLOR_FADER_H

#include <common.h>
#include <m/m_color.h>
#include <m/m_fader_base.h>


class mColorFader_c : public mFaderBase_c {
public:
    mColorFader_c(const mColor &color, EStatus status);
    virtual ~mColorFader_c();

    virtual void setStatus(EStatus status) override;
    virtual bool calc() override;
    virtual void draw() override;

    u8 mAspectRatio;
};

#endif
