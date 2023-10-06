#pragma once

#include "types.h"
#include "nw4r/ut/Color.h"
#include "egg/core/eggFader.h"
#include "egg/prim/eggBitFlag.h"

namespace EGG
{

class ColorFader : public Fader {
public:
    // vtable at 0x00 | 8056eae8
    /* vt 0x08 | 80497a10 */ virtual void setStatus(EStatus);
    /* vt 0x0C | 00000000 */ virtual EStatus getStatus() const;
    /* vt 0x10 | 80497a50 */ virtual bool fadeIn();
    /* vt 0x14 | 80497a80 */ virtual bool fadeOut();
    /* vt 0x18 | 80497ab0 */ virtual bool calc();
    /* vt 0x1C | 80497ba0 */ virtual bool draw();
    /* vt 0x1C | 80131b70 */ virtual ~ColorFader();
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
    /* 80497930 */ ColorFader(
        f32 startX, f32 startY, f32 lengthX, f32 lengthY, 
        nw4r::ut::Color color,EStatus status);
    /* 804979e0 */ void setFrame(u16 frame);
    /* 804979f0 */ void setColor(nw4r::ut::Color);
};

} // namespace EGG
