#ifndef EGG_COLOR_FADER_H
#define EGG_COLOR_FADER_H

#include "egg/core/eggFader.h"
#include "egg/prim/eggBitFlag.h"
#include "nw4r/ut.h"

#include <common.h>

namespace EGG {

class ColorFader : public Fader {
public:
    // vtable at 0x00
    /* vt 0x08 */ virtual void setStatus(EStatus);
    /* vt 0x0C */ virtual EStatus getStatus() const { return mStatus; };
    /* vt 0x10 */ virtual bool fadeIn();
    /* vt 0x14 */ virtual bool fadeOut();
    /* vt 0x18 */ virtual bool calc();
    /* vt 0x1C */ virtual void draw();
    /* vt 0x1C */ virtual ~ColorFader() {}

public:
    /* 0x04 */ EStatus mStatus;
    /* 0x08 */ TBitFlag<u8> mFlags;
    /* 0x0A */ u16 mFrame;
    /* 0x0C */ u16 mFadeTimer;
    /* 0x0E */ u16 field_0x0E;
    /* 0x10 */ nw4r::ut::Color mColor;
    /* 0x14 */ f32 mStartX;
    /* 0x18 */ f32 mStartY;
    /* 0x1C */ f32 mEndX;
    /* 0x20 */ f32 mEndY;

public:
    ColorFader(f32 startX, f32 startY, f32 lengthX, f32 lengthY, nw4r::ut::Color color, EStatus status);
    void setFrame(u16 frame);
    void setColor(nw4r::ut::Color);
    float getWidth() const { return mEndX - mStartX; }
    float getHeight() const { return mEndY - mStartY; }
};

} // namespace EGG

#endif
