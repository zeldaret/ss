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

    f32 newFrame;

    if (rateWasNegative || (mPlayState & 2)) {
        newFrame = mStartFrame;
        if (frame >= rate + mStartFrame) {
            newFrame = frame - rate;
        } else if ((mPlayState & 1) == 0) {
            newFrame = frame + ((mEndFrame - rate) - mStartFrame);
        }
    } else {
        newFrame = frame + rate;
        if ((mPlayState & 1) == 0) {
            if (newFrame >= mEndFrame) {
                newFrame = newFrame - mEndFrame;
            }
        } else {
            f32 t = mEndFrame - 1.0f;
            if (newFrame >= t) {
                newFrame = t;
            }
        }
    }
    mpAnmObj->SetFrame(newFrame);
}

void fanm_c::set(f32 startFrame, playMode_e mode, f32 updateRate, f32 currentFrame) {
    if (currentFrame < 0.0f) {
        // TODO shuffle
        f32 newFrame = mode == PLAY_MODE_1 ? 0.0f : startFrame - 1.0f;
        currentFrame = newFrame;
    }

    mEndFrame = startFrame;
    mStartFrame = 0.0f;
    mpAnmObj->SetFrame(currentFrame);
    mpAnmObj->SetUpdateRate(updateRate);
    mPlayState = (u8)mode;
    mCurrentFrame = currentFrame;
}

void fanm_c::setFrame(f32) {
    // TODO g3d headers
}

// assumed name
void fanm_c::setFrameOnly(f32 frame) {
    banm_c::setFrameOnly(frame);
    mCurrentFrame = frame;
}

bool fanm_c::isStop() const {
    // TODO g3d headers
}

bool fanm_c::checkFrame(f32) const {
    // TODO g3d headers
}

bool fanm_c::unk_802EAE70() const {
    // TODO g3d headers
}

} // namespace m3d
