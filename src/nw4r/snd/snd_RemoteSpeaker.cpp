#include "nw4r/snd/snd_RemoteSpeaker.h"
#include "common.h"
#include "nw4r/snd/snd_RemoteSpeakerManager.h"
#include "nw4r/ut/ut_Lock.h"
#include <cstring>

namespace nw4r {
namespace snd {

RemoteSpeaker::RemoteSpeaker()
    : mInitFlag(false),
      mPlayFlag(false),
      mEnableFlag(false),
      mFirstEncodeFlag(false),
      mValidCallbackFlag(false),
      mCommandBusyFlag(false),
      // mForceResumeFlag(false),
      mState(STATE_INVALID),
      mUserCommand(COMMAND_NONE),
      mInternalCommand(COMMAND_NONE),
      mWpadCallback(nullptr) {

    OSCreateAlarm(&mContinueAlarm);
    OSSetAlarmUserData(&mContinueAlarm, this);

    OSCreateAlarm(&mIntervalAlarm);
    OSSetAlarmUserData(&mIntervalAlarm, this);
}

void RemoteSpeaker::InitParam() {
    ClearParam();

    // mForceResumeFlag = false;
    mContinueFlag = false;
    mPlayFlag = false;
    mEnableFlag = true;
    mIntervalFlag = false;
}

void RemoteSpeaker::ClearParam() {
    mPlayFlag = false;
    mEnableFlag = false;

    OSCancelAlarm(&mContinueAlarm);
    mContinueFlag = false;

    OSCancelAlarm(&mIntervalAlarm);
    mIntervalFlag = false;
}

bool RemoteSpeaker::Setup(WPADCallback pCallback) {
    ut::AutoInterruptLock lock;

    InitParam();

    if (mWpadCallback != NULL) {
        mWpadCallback(mChannelIndex, WPAD_ERR_OK);
        mValidCallbackFlag = false;
    }

    mWpadCallback = pCallback;
    mUserCommand = COMMAND_SPEAKER_ON;
    mInitFlag = true;

    return true;
}

void RemoteSpeaker::Shutdown(WPADCallback pCallback) {
    ut::AutoInterruptLock lock;

    ClearParam();

    if (mWpadCallback != NULL) {
        mWpadCallback(mChannelIndex, WPAD_ERR_OK);
        mValidCallbackFlag = false;
    }

    mUserCommand = COMMAND_SPEAKER_OFF;
    mWpadCallback = pCallback;
    mInitFlag = false;
}

bool RemoteSpeaker::EnableOutput(bool enable) {
    if (!mInitFlag) {
        return false;
    }

    mEnableFlag = enable;
    return true;
}

bool RemoteSpeaker::IsEnabledOutput() const {
    if (!mInitFlag) {
        return false;
    }

    return mEnableFlag;
}

void RemoteSpeaker::Update() {
    if (mCommandBusyFlag) {
        return;
    }

    SpeakerCommand command =
        mUserCommand != COMMAND_NONE ? mUserCommand : mInternalCommand;

    mUserCommand = COMMAND_NONE;
    mInternalCommand = COMMAND_NONE;

    ExecCommand(command);
}

void RemoteSpeaker::ExecCommand(SpeakerCommand command) {
    switch (command) {
    case COMMAND_NONE: {
        break;
    }

    case COMMAND_SPEAKER_ON: {
        mValidCallbackFlag = true;
        mCommandBusyFlag = true;
        mState = STATE_EXEC_SPEAKER_ON;
        WPADControlSpeaker(mChannelIndex, WPAD_SPEAKER_ON, SpeakerOnCallback);
        break;
    }

    case COMMAND_SPEAKER_PLAY: {
        mValidCallbackFlag = true;
        mCommandBusyFlag = true;
        mState = STATE_EXEC_SPEAKER_PLAY;
        WPADControlSpeaker(mChannelIndex, WPAD_SPEAKER_PLAY,
                           SpeakerPlayCallback);
        break;
    }

    case COMMAND_SPEAKER_OFF: {
        mValidCallbackFlag = true;
        mCommandBusyFlag = true;
        mState = STATE_EXEC_SPEAKER_OFF;
        WPADControlSpeaker(mChannelIndex, WPAD_SPEAKER_OFF, SpeakerOffCallback);
        break;
    }
    }
}

void RemoteSpeaker::UpdateStreamData(const s16* pRmtSamples) {
    if (!IsAvailable()) {
        return;
    }

    bool playFlag = true;
    bool silentFlag = mEnableFlag ? IsAllSampleZero(pRmtSamples) : true;

    if (silentFlag /*|| mForceResumeFlag*/) {
        playFlag = false;
    }

    bool firstFlag = !mPlayFlag && playFlag;
    bool lastFlag = mPlayFlag && !playFlag;

    if (playFlag) {
        ut::AutoInterruptLock lock;

        if (!WPADCanSendStreamData(mChannelIndex)) {
            return;
        }

        u32 wencMode = !mFirstEncodeFlag ? WENC_FLAG_USER_INFO : 0;
        mFirstEncodeFlag = false;

        u8 adpcmBuffer[SAMPLES_PER_ENCODED_PACKET];
        WENCGetEncodeData(&mEncodeInfo, wencMode, pRmtSamples,
                          SAMPLES_PER_AUDIO_PACKET, adpcmBuffer);

        s32 result = WPADSendStreamData(mChannelIndex, adpcmBuffer,
                                        SAMPLES_PER_ENCODED_PACKET);
        if (result != WPAD_ERR_OK) {
            mInternalCommand = COMMAND_SPEAKER_ON;
            mState = STATE_INVALID;
            InitParam();

            return;
        }
    }

    if (firstFlag) {
        ut::AutoInterruptLock lock;

        if (!mContinueFlag) {
            OSSetAlarm(&mContinueAlarm,
                       OS_SEC_TO_TICKS(CONTINUOUS_PLAY_INTERVAL_MINUTES * 60LL),
                       ContinueAlarmHandler);

            mContinueBeginTime = OSGetTime();
            mContinueFlag = true;
        }

        OSCancelAlarm(&mIntervalAlarm);
        mIntervalFlag = false;
    }

    if (lastFlag) {
        ut::AutoInterruptLock lock;

        mIntervalFlag = true;
        OSCancelAlarm(&mIntervalAlarm);
        OSSetAlarm(&mIntervalAlarm, OS_SEC_TO_TICKS(1LL), IntervalAlarmHandler);
    }

    mPlayFlag = playFlag;
}

bool RemoteSpeaker::IsAllSampleZero(const s16* pSample) {
    const u32* pBuffer = reinterpret_cast<const u32*>(pSample);
    bool zeroFlag = true;

    for (int i = 0; i < SAMPLES_PER_ENCODED_PACKET; i++) {
        if (pBuffer[i] != 0) {
            zeroFlag = false;
            break;
        }
    }

    return zeroFlag;
}

void RemoteSpeaker::SpeakerOnCallback(s32 chan, s32 result) {
    RemoteSpeaker& r =
        detail::RemoteSpeakerManager::GetInstance().GetRemoteSpeaker(chan);

    switch (result) {
    case WPAD_ERR_OK: {
        r.mFirstEncodeFlag = true;
        std::memset(&r.mEncodeInfo, 0, sizeof(WENCInfo));

        r.mState = STATE_SPEAKER_ON;
        r.mInternalCommand = COMMAND_SPEAKER_PLAY;
        break;
    }

    case WPAD_ERR_BUSY: {
        r.mInternalCommand = COMMAND_SPEAKER_ON;
        break;
    }

    case WPAD_ERR_TRANSFER: {
        r.mState = STATE_INVALID;
        break;
    }

    case WPAD_ERR_NO_CONTROLLER: {
        r.mState = STATE_INVALID;
        break;
    }

    default: {
        r.mState = STATE_INVALID;
        break;
    }
    }

    if (result != WPAD_ERR_OK && result != WPAD_ERR_BUSY) {
        r.NotifyCallback(chan, result);
    }

    r.mCommandBusyFlag = false;
}

void RemoteSpeaker::SpeakerPlayCallback(s32 chan, s32 result) {
    RemoteSpeaker& r =
        detail::RemoteSpeakerManager::GetInstance().GetRemoteSpeaker(chan);

    switch (result) {
    case WPAD_ERR_OK: {
        r.mState = STATE_SPEAKER_PLAY;
        break;
    }

    case WPAD_ERR_BUSY: {
        r.mInternalCommand = COMMAND_SPEAKER_PLAY;
        break;
    }

    case WPAD_ERR_TRANSFER: {
        r.mState = STATE_INVALID;
        break;
    }

    case WPAD_ERR_NO_CONTROLLER: {
        r.mState = STATE_INVALID;
        break;
    }

    default: {
        r.mState = STATE_INVALID;
        break;
    }
    }

    if (result != WPAD_ERR_BUSY) {
        r.NotifyCallback(chan, result);
    }

    r.mCommandBusyFlag = false;
}

void RemoteSpeaker::SpeakerOffCallback(s32 chan, s32 result) {
    RemoteSpeaker& r =
        detail::RemoteSpeakerManager::GetInstance().GetRemoteSpeaker(chan);

    switch (result) {
    case WPAD_ERR_OK: {
        r.mState = STATE_SPEAKER_OFF;
        break;
    }

    case WPAD_ERR_BUSY: {
        r.mInternalCommand = COMMAND_SPEAKER_OFF;
        break;
    }

    case WPAD_ERR_TRANSFER: {
        r.mState = STATE_INVALID;
        break;
    }

    case WPAD_ERR_NO_CONTROLLER: {
        r.mState = STATE_INVALID;
        break;
    }

    default: {
        r.mState = STATE_INVALID;
        break;
    }
    }

    if (result != WPAD_ERR_BUSY) {
        r.NotifyCallback(chan, result);
    }

    r.mCommandBusyFlag = false;
}

void RemoteSpeaker::NotifyCallback(s32 chan, s32 result) {
    if (mValidCallbackFlag && mWpadCallback != NULL) {
        mWpadCallback(chan, result);
        mWpadCallback = NULL;
    }
}

void RemoteSpeaker::ContinueAlarmHandler(OSAlarm* pAlarm, OSContext* pCtx) {
#pragma unused(pCtx)

    ut::AutoInterruptLock lock;
    RemoteSpeaker* p = static_cast<RemoteSpeaker*>(OSGetAlarmUserData(pAlarm));

    // p->mForceResumeFlag = true;
    p->mContinueFlag = false;
}

void RemoteSpeaker::IntervalAlarmHandler(OSAlarm* pAlarm, OSContext* pCtx) {
#pragma unused(pCtx)

    ut::AutoInterruptLock lock;
    RemoteSpeaker* p = static_cast<RemoteSpeaker*>(OSGetAlarmUserData(pAlarm));

    if (p->mIntervalFlag) {
        OSCancelAlarm(&p->mContinueAlarm);
        // p->mForceResumeFlag = false;
        p->mContinueFlag = false;
    }

    p->mIntervalFlag = false;
}

} // namespace snd
} // namespace nw4r
