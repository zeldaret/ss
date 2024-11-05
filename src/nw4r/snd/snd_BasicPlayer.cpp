#include "nw4r/snd/snd_BasicPlayer.h"

#include "nw4r/snd.h"

namespace nw4r {
namespace snd {
namespace detail {

void PlayerParamSet::Init() {
    mPan = 1.0f;

    mVolume = 1.0f;
    mPitch = 1.0f;
    mPan = 0.0f;
    mSurroundPan = 0.0f;
    mLpfFreq = 0.0f;
    mBiquadFilter = 0;
    mBiquadFilterValue = 0.0f;
    mRemoteFilter = 0;

    mOutputLine = OUTPUT_LINE_MAIN;
    mMainOutVolume = 1.0f;
    mMainSend = 0.0f;
    mPanMode = PAN_MODE_DUAL;
    mPanCurve = PAN_CURVE_SQRT;

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mFxSend[i] = 0.0f;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = 1.0f;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mVoiceOutParam[i].volume = 1.0f;
        mVoiceOutParam[i].pitch = 1.0f;
        mVoiceOutParam[i].pan = 0.0f;
        mVoiceOutParam[i].surroundPan = 0.0f;
        mVoiceOutParam[i].fxSend = 0.0f;
        mVoiceOutParam[i].lpf = 0.0f;
    }
}

BasicPlayer::BasicPlayer() : mId(BasicSound::INVALID_ID) {
    mId = -1;
    InitParam();
}

void BasicPlayer::InitParam() {
    mParamSet.Init();
}

void BasicPlayer::SetFxSend(AuxBus bus, f32 send) {
    mParamSet.mFxSend[bus] = send;
}

f32 BasicPlayer::GetFxSend(AuxBus bus) const {
    return mParamSet.mFxSend[bus];
}

void BasicPlayer::SetBiquadFilter(int filter, f32 filterValue) {
    mParamSet.mBiquadFilter = filter;
    mParamSet.mBiquadFilterValue = filterValue;
}

void BasicPlayer::SetRemoteFilter(int filter) {
    mParamSet.mRemoteFilter = filter;
}

void BasicPlayer::SetRemoteOutVolume(int remote, f32 volume) {
    mParamSet.mRemoteOutVolume[remote] = volume;
}

f32 BasicPlayer::GetRemoteOutVolume(int remote) const {
    return mParamSet.mRemoteOutVolume[remote];
}
/*
f32 BasicPlayer::GetRemoteSend(int remote) const {
    return mParamSet.mRemoteSend[remote];
}

f32 BasicPlayer::GetRemoteFxSend(int remote) const {
    return mParamSet.mRemoteFxSend[remote];
}
*/

} // namespace detail
} // namespace snd
} // namespace nw4r
