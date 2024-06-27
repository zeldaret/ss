#include <m/m_fader_base.h>

mFaderBase_c::mFaderBase_c(const mColor &color, EStatus status) : mFlag(0), mFrame(20), mElapsed(0) {
    setColor(color);
    mFlag |= FLAG_2;

    switch (status) {
    case FADED_OUT:
        mStatus = FADED_OUT;
        break;
    case FADED_IN:
        mStatus = FADED_IN;
        break;
    }
}

mFaderBase_c::~mFaderBase_c() {}

void mFaderBase_c::setFrame(u16 frame) {
    mFrame = frame;
}

void mFaderBase_c::setColor(const mColor &newColor) {
    mFaderColor.r = newColor.r;
    mFaderColor.g = newColor.g;
    mFaderColor.b = newColor.b;
}

mFaderBase_c::EStatus mFaderBase_c::getStatus() const {
    return mStatus;
}

bool mFaderBase_c::fadeIn() {
    bool canFadeIn = mStatus == FADED_OUT;
    if (canFadeIn) {
        mStatus = FADING_IN;
        mElapsed = 0;
    }

    return canFadeIn;
}

bool mFaderBase_c::fadeOut() {
    bool canFadeOut = mStatus == FADED_IN;
    if (canFadeOut) {
        mStatus = FADING_OUT;
        mElapsed = 0;
    }

    return canFadeOut;
}

u8 mFaderBase_c::calc() {
    u8 result = 0;
    if (mStatus == FADING_IN) {
        if (mElapsed++ > mFrame) {
            mStatus = FADED_IN;
            result = (mFlag & FLAG_1) != 0;
        }
    } else if (mStatus == FADING_OUT) {
        if (++mElapsed > mFrame + 1) {
            mStatus = FADED_OUT;
            result = (mFlag & FLAG_2) != 0;
        }
    }

    return result;
}
