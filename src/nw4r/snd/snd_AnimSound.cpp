/* Only implemented to the extent necessary to match data sections. */

#include "nw4r/snd/snd_AnimSound.h"

#include "common.h" // nullptr
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SeqSound.h"
#include "nw4r/snd/snd_SeqSoundHandle.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"

#include <cmath>

namespace nw4r {
namespace snd {
namespace detail {

AnimSoundImpl::AnimSoundImpl(SoundStartable &startable, AnimEventPlayer *player, int numSounds)
    : mStartable(startable),
      mpSounds(player),
      mNumSounds(numSounds),
      mCurrentFrame(0.0f),
      mIsActive(false),
      mNeedFrameReset(false),
      mNeedTriggerEventsAtCurrentFrame(false),
      mCycleCounter(0),
      mCallback(NULL),
      field_0x28(1.0f) {}

AnimSoundImpl::~AnimSoundImpl() {
    Shutdown();
}

bool AnimSoundImpl::Setup(const void *data) {
    Shutdown();
    if (!mReader.Setup(data)) {
        return false;
    }

    mNeedFrameReset = true;
    mIsActive = true;
    mCurrentFrame = 0.0f;
    mCycleCounter = 0;
    return true;
}

void AnimSoundImpl::Shutdown() {
    if (mIsActive) {
        for (int i = 0; i < mNumSounds; i++) {
            if (mpSounds[i].GetHandle()->IsAttachedSound() && mpSounds[i].IsRunning()) {
                mpSounds[i].GetHandle()->Stop(0);
            }
        }

        mReader.Shutdown();
        mIsActive = false;
    }
}

void AnimSoundImpl::ResetFrame(f32 f, int i) {
    mCurrentFrame = f;
    mCycleCounter = i;
    mNeedTriggerEventsAtCurrentFrame = true;
    mNeedFrameReset = false;
}

void AnimSoundImpl::UpdateFrame(f32 frame, PlayDirection dir) {
    for (int i = 0; i < mNumSounds; i++) {
        mpSounds[i].UpdateFrame();
    }

    if (mNeedFrameReset) {
        if (dir == FORWARD) {
            ResetFrame(0.0f, 0);
        } else {
            u32 duration = mReader.GetAnimDuration();
            ResetFrame(duration - 1.0f, 0);
        }
        mNeedFrameReset = false;
    }

    mVariableValue = (frame - mCurrentFrame) / field_0x28;
    if (dir == FORWARD) {
        UpdateForward(frame);
    } else {
        UpdateBackward(frame);
    }
    mCurrentFrame = frame;
    mNeedTriggerEventsAtCurrentFrame = false;
}

void AnimSoundImpl::UpdateForward(f32 frame) {
    s32 currFrameFloor = std::floorf(mCurrentFrame);
    s32 targetFrame = std::floorf(frame);

    if (mNeedTriggerEventsAtCurrentFrame && mCurrentFrame == currFrameFloor) {
        currFrameFloor -= 1;
    }

    if (currFrameFloor == targetFrame) {
        return;
    }

    s32 loopFrame = currFrameFloor + 1;
    while (true) {
        if (loopFrame == mReader.GetAnimDuration()) {
            loopFrame -= mReader.GetAnimDuration();
            mCycleCounter++;
        }

        UpdateOneFrame(loopFrame, FORWARD);

        if (loopFrame == targetFrame) {
            break;
        }
        loopFrame++;
    }
}

void AnimSoundImpl::UpdateBackward(f32 frame) {
    s32 currFrameCeil = std::ceilf(mCurrentFrame);
    s32 targetFrame = std::ceilf(frame);

    if (currFrameCeil >= mReader.GetAnimDuration()) {
        currFrameCeil -= mReader.GetAnimDuration();
    }

    if (targetFrame >= mReader.GetAnimDuration()) {
        targetFrame -= mReader.GetAnimDuration();
    }

    if (mNeedTriggerEventsAtCurrentFrame && mCurrentFrame == currFrameCeil) {
        currFrameCeil += 1;
    }

    if (currFrameCeil == targetFrame) {
        return;
    }

    s32 loopFrame = currFrameCeil - 1;
    while (true) {
        if (loopFrame == -1) {
            loopFrame += mReader.GetAnimDuration();
            mCycleCounter--;
        }

        UpdateOneFrame(loopFrame, BACKWARD);

        if (loopFrame == targetFrame) {
            break;
        }
        loopFrame--;
    }
}

void AnimSoundImpl::UpdateOneFrame(s32 duration, PlayDirection direction) {
    for (u32 eventIdx = 0; eventIdx < mReader.GetEventCount(); eventIdx++) {
        const AnimEventRef *ref = mReader.GetEventRef(eventIdx);
        if (ref == NULL) {
            continue;
        }
        if (IsPlayableLoopCount(ref->mInfo)) {
            if ((ref->mInfo.flags) & 1) {
                UpdateTrigger(ref, duration, direction);
            } else {
                if (direction == FORWARD) {
                    UpdateForwardRange(ref, duration);
                } else {
                    UpdateBackwardRange(ref, duration);
                }
            }
        }
    }
}

void AnimSoundImpl::UpdateTrigger(const AnimEventRef *ref, s32 frame, PlayDirection direction) {
    const AnimEvent *event = mReader.GetEvent(ref);
    if (event == NULL) {
        return;
    }

    if (ref->mInfo.flags & 4) {
        return;
    }

    if (!event->MatchesDirection(direction)) {
        return;
    }

    if ((ref->mInfo.flags & 2)) {
        if (ref->mInfo.field_0x00 == frame) {
            StopEvent(event);
            if (mCallback != NULL) {
                (mCallback)(0, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
            }
            StartEvent(event, true);
        }
    } else {
        if (ref->mInfo.field_0x00 == frame) {
            StopEvent(event);
            if (mCallback != NULL) {
                (mCallback)(0, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
            }
            StartEvent(event, true);
        }

        s32 u;
        if (direction == FORWARD) {
            u = ref->mInfo.field_0x04;
        } else {
            u = ut::Max(ref->mInfo.field_0x00 * 2 - ref->mInfo.field_0x04, 0);
        }

        if (u == frame) {
            if (mCallback != NULL) {
                (mCallback)(1, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
            }
            StopEvent(event);
        }
    }
}

void AnimSoundImpl::UpdateForwardRange(const AnimEventRef *ref, s32 frame) {
    const AnimEvent *event = mReader.GetEvent(ref);
    if (event == NULL) {
        return;
    }

    if (ref->mInfo.flags & 4 && ref->mInfo.flags & 2) {
        HoldEvent(event, true);
        return;
    }

    if (ref->mInfo.flags & 4) {
        if (ref->mInfo.field_0x04 == frame && mCallback != NULL) {
            (mCallback)(3, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
        }
        if (mCycleCounter < ref->mInfo.field_0x09) {
            HoldEvent(event, true);
        } else if (mCycleCounter == ref->mInfo.field_0x09) {
            if (frame < ref->mInfo.field_0x04) {
                HoldEvent(event, true);
            } else {
                StopEvent(event);
            }
        }
    } else if ((ref->mInfo.flags & 2)) {
        if (ref->mInfo.field_0x00 == frame && mCallback != NULL) {
            (mCallback)(2, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
        }
        if (mCycleCounter > ref->mInfo.field_0x09) {
            HoldEvent(event, true);
        } else if (mCycleCounter == ref->mInfo.field_0x09 && ref->mInfo.field_0x00 <= frame) {
            HoldEvent(event, true);
        }
    } else {
        if (ref->mInfo.field_0x00 == frame && mCallback != NULL) {
            (mCallback)(2, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
        }
        if (ref->mInfo.field_0x04 == frame && mCallback != NULL) {
            (mCallback)(3, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
        }
        if (ref->mInfo.field_0x00 <= frame && frame < ref->mInfo.field_0x04) {
            HoldEvent(event, true);
        } else {
            StopEvent(event);
        }
    }
}

void AnimSoundImpl::UpdateBackwardRange(const AnimEventRef *ref, s32 frame) {
    const AnimEvent *event = mReader.GetEvent(ref);
    if (event == NULL) {
        return;
    }

    if (ref->mInfo.flags & 4 && ref->mInfo.flags & 2) {
        HoldEvent(event, true);
        return;
    }

    if (ref->mInfo.flags & 4) {
        if (ref->mInfo.field_0x04 == frame && mCallback != NULL) {
            (mCallback)(2, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
        }
        if (mCycleCounter < ref->mInfo.field_0x09) {
            HoldEvent(event, true);
        } else if (mCycleCounter == ref->mInfo.field_0x09 && frame <= ref->mInfo.field_0x04) {
            HoldEvent(event, true);
        }
    } else if ((ref->mInfo.flags & 2)) {
        if (ref->mInfo.field_0x00 == frame && mCallback != NULL) {
            (mCallback)(3, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
        }
        if (mCycleCounter > ref->mInfo.field_0x09) {
            HoldEvent(event, true);
        } else if (mCycleCounter == ref->mInfo.field_0x09) {
            if (ref->mInfo.field_0x00 < frame) {
                HoldEvent(event, true);
            } else {
                StopEvent(event);
            }
        }
    } else {
        if (ref->mInfo.field_0x00 == frame && mCallback != NULL) {
            (mCallback)(3, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
        }
        if (ref->mInfo.field_0x04 == frame && mCallback != NULL) {
            (mCallback)(2, frame, event->GetSoundLabel(), event->field_0x1C, mUserData);
        }
        if (ref->mInfo.field_0x00 < frame && frame <= ref->mInfo.field_0x04) {
            HoldEvent(event, true);
        } else {
            StopEvent(event);
        }
    }
}

void AnimSoundImpl::StartEvent(const AnimEvent *event, bool b) {
    int leastPriority = 0x80;
    int leastPriorityIdx = -1;
    int i = 0;
    for (; i < mNumSounds; i++) {
        if (!mpSounds[i].IsAttachedSound()) {
            break;
        }
        int priority = mpSounds[i].GetPriority();
        if (leastPriority > priority) {
            leastPriorityIdx = i;
            leastPriority = priority;
        }
    }

    if (i == mNumSounds) {
        mpSounds[leastPriorityIdx].ForceStop();
        i = leastPriorityIdx;
    }

    mpSounds[i].StartEvent(event, &mStartable, b);

    u32 varNo = 0;
    if (event->GetVarNo(&varNo)) {
        mpSounds[i].SetVariable(event, varNo, mVariableValue);
    }
}

void AnimSoundImpl::HoldEvent(const AnimEvent *event, bool b) {
    int leastPriority = 0x80;
    int leastPriorityIdx = -1;
    int i = 0;
    for (; i < mNumSounds; i++) {
        if (mpSounds[i].IsCurrentEvent(event)) {
            return;
        }
        if (!mpSounds[i].IsAttachedSound()) {
            leastPriorityIdx = i;
            leastPriority = -1;
        } else {
            int priority = mpSounds[i].GetPriority();
            if (leastPriority > priority) {
                leastPriorityIdx = i;
                leastPriority = priority;
            }
        }
    }

    if (i == mNumSounds) {
        mpSounds[leastPriorityIdx].ForceStop();
        i = leastPriorityIdx;
    }

    mpSounds[i].HoldEvent(event, &mStartable, b);

    u32 varNo = 0;
    if (event->GetVarNo(&varNo)) {
        mpSounds[i].SetVariable(event, varNo, mVariableValue);
    }
}

void AnimSoundImpl::StopEvent(const AnimEvent *event) {
    for (int i = 0; i < mNumSounds; i++) {
        mpSounds[i].StopEvent(event);
    }
}

bool AnimSoundImpl::IsPlayableLoopCount(const nw4r::snd::detail::AnimEventFrameInfo &ref) {
    if (!(ref.flags & 1) && (ref.flags & 4 || ref.flags & 2)) {
        return true;
    }
    int i1 = ut::Max((int)ref.field_0x09, 0);
    int i2 = ut::Abs(mCycleCounter);
    if (ref.field_0x0A == 0) {
        if (i2 < i1) {
            return false;
        }
    } else {
        if (i2 < i1) {
            return false;
        }
        if ((i2 - i1) % ref.field_0x0A != 0) {
            return false;
        }
    }

    return true;
}

void AnimEventPlayer::SetVolumePitch(const AnimEvent *event, bool b) {
    if (event->volume < 128) {
        SetVolume(event->volume / 128.0f);
    }
    if (event->pitch != 1.0f) {
        SetPitch(event->pitch);
    }

    mpEvent = event;
    if (b) {
        b = !event->ShouldPreventStart();
    }
    mIsRunning = b;
}

void AnimEventPlayer::SetVariable(const AnimEvent *event, u32 varNo, f32 f) {
    if (varNo < 16) {
        if (GetHandle()->IsAttachedSound()) {
            SeqSoundHandle handle(GetHandle());
            int iVal = f * 100.0f;
            handle.WriteVariable(varNo, ut::Clamp(iVal, -0x8000, 0x7FFF));
        }
    } else if (varNo < 32) {
        int iVal = f * 100.0f;
        SeqSound::WriteGlobalVariable(varNo - 16, ut::Clamp(iVal, -0x8000, 0x7FFF));
    }
}

AnimEventPlayer::AnimEventPlayer() : mpEvent(NULL), mIsRunning(false) {}

AnimEventPlayer::~AnimEventPlayer() {
    if (mHandle.IsAttachedSound() && IsRunning()) {
        mHandle.Stop(0);
    }
}

void AnimEventPlayer::StartEvent(const AnimEvent *event, SoundStartable *startable, bool b) {
    if (event->soundId == 0xFFFFFFFF) {
        if (!startable->StartSound(GetHandle(), event->GetSoundLabel())) {
            return;
        }
    } else {
        if (!startable->StartSound(GetHandle(), event->soundId)) {
            return;
        }
    }

    SetVolumePitch(event, b);
}

void AnimEventPlayer::HoldEvent(const AnimEvent *event, SoundStartable *startable, bool b) {
    if (event->soundId == 0xFFFFFFFF) {
        if (!startable->HoldSound(GetHandle(), event->GetSoundLabel())) {
            return;
        }
    } else {
        if (!startable->HoldSound(GetHandle(), event->soundId)) {
            return;
        }
    }

    GetHandle()->detail_GetAttachedSound()->SetAutoStopCounter(0);
    SetVolumePitch(event, b);
}

void AnimEventPlayer::ForceStop() {
    if (IsAttachedSound()) {
        mHandle.detail_GetAttachedSound()->Stop(0);
    }
    mpEvent = 0;
}

void AnimEventPlayer::UpdateFrame() {
    if (!mHandle.IsAttachedSound()) {
        mpEvent = 0;
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r
