/* Only implemented to the extent necessary to match data sections. */

#include "nw4r/snd/snd_AnimSoundReader.h"
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"

// Class and non-inlined function names from InazumaWii
// Names of automatically inlined functions from BBA

namespace nw4r {
namespace snd {
namespace detail {

class AnimEventPlayer {
public:
    AnimEventPlayer();
    ~AnimEventPlayer();

    SoundHandle *GetHandle() {
        return &mHandle;
    }

    bool IsAttachedSound() const {
        return mHandle.IsAttachedSound();
    }

    bool IsCurrentEvent(const AnimEvent *event) const {
        return event == mpEvent;
    }

    int GetPriority() const {
        if (!IsAttachedSound()) {
            return 0;
        }
        return mHandle.detail_GetAttachedSound()->GetPriority();
    }

    bool IsRunning() const {
        return mIsRunning;
    }

    void UpdateFrame();

    void ForceStop();

    void StopEvent(const AnimEvent *event) {
        if (mpEvent == event) {
            ForceStop();
        }
    }

    void SetVolume(f32 volume) {
        if (IsAttachedSound()) {
            mHandle.detail_GetAttachedSound()->SetVolume(volume, 0);
        }
    }

    void SetPitch(f32 pitch) {
        if (IsAttachedSound()) {
            mHandle.detail_GetAttachedSound()->SetPitch(pitch);
        }
    }

    void StartEvent(const AnimEvent *event, SoundStartable *startable, bool b);
    void HoldEvent(const AnimEvent *event, SoundStartable *startable, bool b);

    // Not sure about these, this could be related to
    // InitParam__Q44nw4r3snd6detail15AnimEventPlayerFPCQ44nw4r3snd6detail9AnimEventb
    // from BBA, but SetVariable is almost certainly an inline (duplicated code)
    // but the float argument is read from AnimSoundImpl so something seems to have
    // changed.
    void SetVolumePitch(const AnimEvent *event, bool b);
    void SetVariable(const AnimEvent *event, u32 varNo, f32 f);

private:
    SoundHandle mHandle;      // at 0x00
    const AnimEvent *mpEvent; // at 0x04
    bool mIsRunning;          // at 0x08
};

class AnimSoundImpl {
public:
    enum PlayDirection {
        FORWARD,
        BACKWARD,
    };

    AnimSoundImpl(SoundStartable &startable, AnimEventPlayer *player, int);
    ~AnimSoundImpl();

    bool Setup(const void *data);
    void Shutdown();
    void ResetFrame(f32, int cycle);
    void UpdateFrame(f32 frame, PlayDirection dir);
    void UpdateForward(f32 frame);
    void UpdateBackward(f32 frame);
    void UpdateOneFrame(s32 duration, PlayDirection direction);
    void UpdateTrigger(const AnimEventRef *, s32, PlayDirection);
    void UpdateForwardRange(const AnimEventRef *, s32);
    void UpdateBackwardRange(const AnimEventRef *, s32);

    void StartEvent(const AnimEvent *, bool);
    void HoldEvent(const AnimEvent *, bool);
    void StopEvent(const AnimEvent *);
    bool IsPlayableLoopCount(const nw4r::snd::detail::AnimEventFrameInfo &);

    typedef void (*Callback)(int, s32, const char *, UNKWORD, void *userData);

    void SetCallback(Callback cb, void *userData) {
        mCallback = cb;
        mUserData = userData;
    }

    u32 GetAnimDuration() const {
        return mReader.GetAnimDuration();
    }

private:
    /* 0x00 */ SoundStartable &mStartable;
    /* 0x04 */ AnimSoundFileReader mReader;
    /* 0x0C */ f32 mCurrentFrame;
    /* 0x10 */ AnimEventPlayer *mpSounds;
    /* 0x14 */ int mNumSounds;
    /* 0x18 */ bool mIsActive;
    /* 0x19 */ bool mNeedFrameReset;
    /* 0x1A */ bool mNeedTriggerEventsAtCurrentFrame;
    /* 0x1C */ int mCycleCounter;
    /* 0x20 */ Callback mCallback;
    /* 0x24 */ void *mUserData;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 mVariableValue;
};

} // namespace detail

// Not sure about this one but it appears game code isn't meant to access the "detail"
// namespace so I guess some way for game code to use the above things need to exist
class AnimSound {
public:
    AnimSound(SoundStartable &startable) : mImpl(startable, mPlayers, 8) {}

    enum PlayDirection {
        FORWARD,
        BACKWARD,
    };

    typedef void (*Callback)(int, s32, const char *, UNKWORD, void *userData);

    bool Setup(const void *data) {
        return mImpl.Setup(data);
    }

    void UpdateFrame(f32 frame, PlayDirection dir) {
        mImpl.UpdateFrame(frame, (detail::AnimSoundImpl::PlayDirection)dir);
    }

    void ResetFrame(f32 frame, int cycle) {
        mImpl.ResetFrame(frame, cycle);
    }

    void Shutdown() {
        mImpl.Shutdown();
    }

    void SetCallback(Callback cb, void *userData) {
        mImpl.SetCallback(cb, userData);
    }

    u32 GetAnimDuration() const {
        return mImpl.GetAnimDuration();
    }

private:
    /* 0x00 */ detail::AnimSoundImpl mImpl;
    /* 0x30 */ detail::AnimEventPlayer mPlayers[8];
};

} // namespace snd
} // namespace nw4r
