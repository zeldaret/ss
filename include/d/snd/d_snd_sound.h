#ifndef D_SND_SOUND_H
#define D_SND_SOUND_H

#include "common.h"
#include "nw4r/snd/snd_SeqTrack.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/ut/ut_list.h"

class dSndSound_c;

// Not sure, handles a bunch of floats, exists in a ton of variants
class dSndControl_c {
public:
    dSndControl_c(f32 value, f32 min, f32 max)
        : mResetValue(value),
          mMax(max),
          mMin(min),
          mCurrValue(value),
          mStartValue(value),
          mTargetValue(value),
          mDuration(0),
          mTimer(0),
          mIsLinked(false) {}

    ~dSndControl_c() {}

    virtual void configure(f32 value, f32 min, f32 max); // vt 0x08
    virtual void reset();                                // vt 0x0C
    virtual void set(f32 value, s32 frames);             // vt 0x10
    virtual void calc();                                 // vt 0x14
    virtual void stop();                                 // vt 0x18

    bool isLinked() const {
        return mIsLinked;
    }

    void setLinked(bool linked) {
        mIsLinked = linked;
    }

    f32 getCurrentValue() const {
        return mCurrValue;
    }

    bool isFinished() const {
        return mTimer >= mDuration;
    }

protected:
    /* 0x04 */ f32 mResetValue;
    /* 0x08 */ f32 mMax;
    /* 0x0C */ f32 mMin;
    /* 0x10 */ f32 mCurrValue;
    /* 0x14 */ f32 mStartValue;
    /* 0x18 */ f32 mTargetValue;
    /* 0x1C */ s32 mDuration;
    /* 0x20 */ s32 mTimer;
    /* 0x24 */ nw4r::ut::Node mNode;
    /* 0x2C */ bool mIsLinked;
};

class dSndControlSound_c : public dSndControl_c {
public:
    dSndControlSound_c(f32 value, f32 min, f32 max) : dSndControl_c(value, min, max) {}
    virtual void apply(dSndSound_c *pHandle) = 0; // vt 0x1C

private:
};

class dSndControlSoundVolume_c : public dSndControlSound_c {
public:
    dSndControlSoundVolume_c() : dSndControlSound_c(1.0f, 0.0f, 2.0f) {}
    virtual void apply(dSndSound_c *pHandle) override;
};

class dSndControlSoundPitch_c : public dSndControlSound_c {
public:
    dSndControlSoundPitch_c() : dSndControlSound_c(1.0f, 0.25f, 4.0f) {}
    virtual void apply(dSndSound_c *pHandle) override;
};

/** Stores pitch as a value from -24 to 24, exponentially scaling it to 0.25-4.0 */
class dSndControlSoundPitchLinear_c : public dSndControlSound_c {
public:
    dSndControlSoundPitchLinear_c();
    virtual void apply(dSndSound_c *pHandle) override;
    virtual void set(f32 value, s32 frames) override;

    void setControl(dSndControlSound_c *ctrl) {
        mpOtherControl = ctrl;
    }

private:
    /* 0x30 */ dSndControlSound_c *mpOtherControl;
};

class dSndControlSoundSeqTempoRatio_c : public dSndControlSound_c {
public:
    dSndControlSoundSeqTempoRatio_c() : dSndControlSound_c(1.0f, 0.25f, 4.0f) {}
    virtual void apply(dSndSound_c *pHandle) override;
};

class dSndControlSoundSeqTrackVolume_c : public dSndControlSound_c {
public:
    dSndControlSoundSeqTrackVolume_c() : dSndControlSound_c(1.0f, 0.0f, 2.0f) {}
    virtual void apply(dSndSound_c *pHandle) override;

    void setMask(u16 mask) {
        mMask = mask;
    }

private:
    /* 0x2E */ u16 mMask;
};

class dSndControlSoundStrmTrackVolume_c : public dSndControlSound_c {
public:
    dSndControlSoundStrmTrackVolume_c() : dSndControlSound_c(1.0f, 0.0f, 2.0f) {}
    virtual void apply(dSndSound_c *pHandle) override;

    void setMask(u16 mask) {
        mMask = mask;
    }

private:
    /* 0x2E */ u16 mMask;
};

class dSndSound_c : public nw4r::snd::SoundHandle {
public:
    dSndSound_c();
    ~dSndSound_c();

    virtual void cancel();                                                                        // 0x08
    virtual void executeCtrls();                                                                   // 0x0C
    virtual void fadeIn(u32 id, s32 fadeFrames);                                                   // 0x10
    virtual nw4r::snd::SoundStartable::StartResult prepareSound(u32 soundId, u32 startOffset);       // 0x14
    virtual nw4r::snd::SoundStartable::StartResult prepareSound(const char *label, u32 startOffset); // 0x18
    virtual void onPreparing(u32 soundId, u32 startOffset);                                            // 0x1C
    virtual void stop(s32 fadeFrames);                                                             // 0x20
    virtual void pause(bool pauseFlag, s32 fadeFrames);                                            // 0x24

    bool isStrmSound();
    bool isWaveSound();
    bool isSeqSound();

    f32 getCurrentStrmTrackVolume(u32 index) const;

    void setVolume(f32 volume, s32 frames);
    void setPitchRelated(f32 pitch, s32 frames);
    void setLinearPitch(f32 pitch, s32 frames);
    void setTrackVolume(u32 trackFlags, f32 volume, s32 frames);
    void setStrmTrackVolume(u32 trackFlags, f32 volume, s32 frames);
    void setSingleSeqTrackVolume(u16 index, f32 volume, s32 frames);
    void setSingleStrmTrackVolume(u16 index, f32 volume, s32 frames);

    // why f32 frames?
    void setEachSeqTrackVolume(u32 trackFlags, f32 frames);
    void setEachStrmTrackVolume(u32 trackFlags, f32 frames);

    void setSeqTempoRatio(f32 ratio, s32 frames);
    void setSeqTrackMute(u32 trackFlags, nw4r::snd::SeqMute mute);
    void forceStop();

    s16 readSeqTrackVariable(int varNo);
    void writeSeqTrackVariable(int varNo, s16 value);

    void linkCtrl(dSndControlSound_c *);
    void unlinkCtrl(dSndControlSound_c *);

private:
    bool cannotStart() const {
        return mIsRunning && !mIsFadingOut && IsAttachedSound();
    }

    bool isRunning() const {
        return mIsRunning && IsAttachedSound();
    }

    bool isPreparing() const {
        return mIsPreparing && !mIsRunning;
    }

    bool canCancel() const {
        return mIsRunning || mIsPreparing || IsAttachedSound();
    }

    bool isPreparingSoundId(u32 soundId) const {
        bool ret = mIsPreparing;
        if (ret) {
            ret = !mIsRunning;
        }
        if (ret) {
            ret = GetId() == soundId;
        }
        return ret;
    }

    void resetControls();
    void resetTrackVolumes();

    void setControlValue(dSndControlSound_c *ctrl, f32 value, s32 frames);

    /* 0x08 */ u8 _0x08[0x10 - 0x08];

    /* 0x10 */ u32 mPrevStartOffset;
    /* 0x14 */ bool mIsPreparing;
    /* 0x15 */ bool mPauseFlag;
    /* 0x16 */ bool mIsRunning;
    /* 0x17 */ bool mIsFadingOut;
    /* 0x18 */ dSndControlSoundVolume_c mCtrlVolume;
    /* 0x48 */ dSndControlSoundPitch_c mCtrlPitch;
    /* 0x78 */ dSndControlSoundPitchLinear_c mLinearPitch;
    /* 0xAC */ dSndControlSoundSeqTrackVolume_c *mpCtrlSeqTrackVolume;
    /* 0xB0 */ dSndControlSoundStrmTrackVolume_c *mpCtrlStrmTrackVolume;
    /* 0xB4 */ dSndControlSoundSeqTempoRatio_c mpCtrlSeqTempoRatio;
    /* 0xE4 */ nw4r::ut::List mList;
};

#endif
