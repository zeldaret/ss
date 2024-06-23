#include <m/m3d/m_fanm.h>

namespace m3d {

fanm_c::fanm_c() {
    mEndFrame = 0.0f;
    mCurrentFrame = 0.0f;
    mPlayState = 0.0f;
}
fanm_c::~fanm_c() {}

void fanm_c::play() {
    f32 frame = mpAnmObj->GetFrame();
    f32 rate = mpAnmObj->GetUpdateRate();
    bool rateWasNegative = rate < 0.0f;
    if (rateWasNegative) {
        rate *= -1.0f;
    }
    mCurrentFrame = frame;
    mpAnmObj->SetFrame(getNextFrame(frame, rate, rateWasNegative));
}

inline f32 fanm_c::getNextFrame(f32 frame, f32 rate, bool rateWasNegative) {
    // This inline is needed for the double branch
    if (rateWasNegative || (mPlayState & 2)) {
        if (frame >= rate + mStartFrame) {
            return frame - rate;
        } else if ((mPlayState & 1) == 0) {
            return frame + ((mEndFrame - rate) - mStartFrame);
        } else {
            return mStartFrame;
        }
    } else {
        frame += rate;
        if ((mPlayState & 1) == 0) {
            if (frame >= mEndFrame) {
                frame = frame - mEndFrame;
            }
        } else {
            f32 t = mEndFrame - 1.0f;
            if (frame >= t) {
                frame = t;
            }
        }
        return frame;
    }
}

void fanm_c::set(f32 startFrame, playMode_e mode, f32 updateRate, f32 currentFrame) {
    if (currentFrame < 0.0f) {
        currentFrame = startFrame;
        if (mode == PLAY_MODE_1) {
            currentFrame = 0.0f;
        } else {
            currentFrame -= 1.0f;
        }
    }

    mEndFrame = startFrame;
    mStartFrame = 0.0f;
    mpAnmObj->SetFrame(currentFrame);
    mpAnmObj->SetUpdateRate(updateRate);
    mPlayState = (u8)mode;
    mCurrentFrame = currentFrame;
}

void fanm_c::set2(f32 startFrame, playMode_e mode, f32 endFrame, f32 rate, f32 currentFrame) {
    if (currentFrame < 0.0f) {
        currentFrame = rate < 0.0f ? endFrame - 1.0f : startFrame;
    }

    mStartFrame = startFrame;
    mEndFrame = endFrame;
    mpAnmObj->SetFrame(currentFrame);
    mpAnmObj->SetUpdateRate(rate);
    mPlayState = (u8)mode;
    mCurrentFrame = currentFrame;
}

// assumed name
void fanm_c::setFrameOnly(f32 frame) {
    banm_c::setFrameOnly(frame);
    mCurrentFrame = frame;
}

bool fanm_c::isStop() const {
    f32 frame = mpAnmObj->GetFrame();
    f32 rate = mpAnmObj->GetUpdateRate();
    if (rate < 0.0f || mPlayState == PLAY_MODE_3) {
        return frame <= mStartFrame;
    } else if (mPlayState == PLAY_MODE_1) {
        return frame >= mEndFrame - 1.0f;
    }
    return false;
}

bool fanm_c::checkFrame(f32 frame) const {
    f32 stored = mpAnmObj->GetFrame();
    if (mCurrentFrame == stored) {
        return stored == frame;
    }

    f32 rate = mpAnmObj->GetUpdateRate();
    if (rate < 0.0f || (mPlayState & 2)) {
        if (mCurrentFrame > stored) {
            if (mCurrentFrame > frame && stored <= frame) {
                return true;
            }
        } else if (frame < mCurrentFrame || frame >= stored) {
            return true;
        }
    } else {
        if (mCurrentFrame < stored) {
            if (mCurrentFrame < frame && stored >= frame) {
                return true;
            }
        } else if (frame > mCurrentFrame || frame <= stored) {
            return true;
        }
    }

    return false;
}

bool fanm_c::unk_802EAE70() const {
    f32 frame = mpAnmObj->GetFrame();
    f32 rate = mpAnmObj->GetUpdateRate();
    if (rate < 0.0f || (mPlayState & 2)) {
        return mCurrentFrame < frame;
    } else {
        return mCurrentFrame > frame;
    }
}

} // namespace m3d
