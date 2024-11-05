#ifndef NW4R_SND_BASIC_PLAYER_H
#define NW4R_SND_BASIC_PLAYER_H
#include "common.h"
#include "nw4r/snd/snd_Common.h"
#include "nw4r/snd/snd_Voice.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h"  // IWYU pragma: export
#include "rvl/WPAD.h" // IWYU pragma: export

namespace nw4r {
namespace snd {
namespace detail {

struct PlayerParamSet {
    PlayerParamSet() {
        Init();
    }
    void Init();

    f32 mVolume;            // at 0x0
    f32 mPitch;             // at 0x4
    f32 mPan;               // at 0x8
    f32 mSurroundPan;       // at 0xC
    f32 mLpfFreq;           // at 0x10
    f32 mBiquadFilterValue; // at 0x14
    u8 mBiquadFilter;       // at 0x18
    u8 mRemoteFilter;       // at 0x19

    int mOutputLine;                                    // at 0x1C
    f32 mMainOutVolume;                                 // at 0x20
    f32 mMainSend;                                      // at 0x24
    PanMode mPanMode;                                   // at 0x28
    PanCurve mPanCurve;                                 // at 0x2C
    f32 mFxSend[AUX_BUS_NUM];                           // at 0x30
    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS];         // at 0x40
    VoiceOutParam mVoiceOutParam[WPAD_MAX_CONTROLLERS]; // 0x50
};

class BasicPlayer {
public:
    BasicPlayer();
    virtual ~BasicPlayer() {} // at 0x8

    virtual bool Start() = 0;           // at 0xC
    virtual void Stop() = 0;            // at 0x10
    virtual void Pause(bool flag) = 0;  // at 0x14
    virtual bool IsActive() const = 0;  // at 0x18
    virtual bool IsStarted() const = 0; // at 0x1C
    virtual bool IsPause() const = 0;   // at 0x20

    void InitParam();

    u32 GetId() const {
        return mId;
    }
    void SetId(u32 id) {
        mId = id;
    }

    f32 GetVolume() const {
        return mParamSet.mVolume;
    }
    void SetVolume(f32 volume) {
        mParamSet.mVolume = volume;
    }

    f32 GetPitch() const {
        return mParamSet.mPitch;
    }
    void SetPitch(f32 pitch) {
        mParamSet.mPitch = pitch;
    }

    f32 GetPan() const {
        return mParamSet.mPan;
    }
    void SetPan(f32 pan) {
        mParamSet.mPan = pan;
    }

    f32 GetSurroundPan() const {
        return mParamSet.mSurroundPan;
    }
    void SetSurroundPan(f32 pan) {
        mParamSet.mSurroundPan = pan;
    }

    f32 GetLpfFreq() const {
        return mParamSet.mLpfFreq;
    }
    void SetLpfFreq(f32 freq) {
        mParamSet.mLpfFreq = freq;
    }

    int GetOutputLine() const {
        return mParamSet.mOutputLine;
    }
    void SetOutputLine(int flags) {
        mParamSet.mOutputLine = flags;
    }

    f32 GetMainOutVolume() const {
        return mParamSet.mMainOutVolume;
    }
    void SetMainOutVolume(f32 volume) {
        mParamSet.mMainOutVolume = volume;
    }

    f32 GetMainSend() const {
        return mParamSet.mMainSend;
    }
    void SetMainSend(f32 send) {
        mParamSet.mMainSend = send;
    }

    void SetFxSend(AuxBus bus, f32 send);
    f32 GetFxSend(AuxBus bus) const;

    void SetRemoteOutVolume(int remote, f32 volume);
    f32 GetRemoteOutVolume(int remote) const;

    void SetBiquadFilter(int filter, f32 filterValue);
    f32 GetBiquadFilterValue() const {
        return mParamSet.mBiquadFilterValue;
    }
    void SetRemoteFilter(int filter);

    f32 GetRemoteSend(int remote) const;
    f32 GetRemoteFxSend(int remote) const;

    int GetRemoteFilter() const {
        return mParamSet.mRemoteFilter;
    }

    PanMode GetPanMode() const {
        return mParamSet.mPanMode;
    }
    void SetPanMode(PanMode mode) {
        mParamSet.mPanMode = mode;
    }

    PanCurve GetPanCurve() const {
        return mParamSet.mPanCurve;
    }
    void SetPanCurve(PanCurve curve) {
        mParamSet.mPanCurve = curve;
    }

    const VoiceOutParam &GetVoiceOutParam(int index) const {
        return mParamSet.mVoiceOutParam[index];
    }

private:
    PlayerParamSet mParamSet;
    u32 mId; // at 0x9C
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
