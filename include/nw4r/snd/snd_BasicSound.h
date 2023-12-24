#ifndef NW4R_SND_BASIC_SOUND_H
#define NW4R_SND_BASIC_SOUND_H
#include <nw4r/snd/snd_Common.h>
#include <nw4r/snd/snd_MoveValue.h>
#include <nw4r/types_nw4r.h>
#include <nw4r/ut.h>
#include <rvl/WPAD.h>

namespace nw4r {
namespace snd {
namespace detail {

class BasicSound {
    friend class SoundHandle;

public:
    NW4R_UT_RTTI_DECL(BasicSound);

    struct AmbientParamUpdateCallback {
        virtual void detail_Update(SoundParam *param, u32 id, BasicSound *sound, const void *arg, u32 flags) = 0;
    };

    struct AmbientArgUpdateCallback {
        virtual void detail_Update(void *arg, const BasicSound *sound) = 0;
    };

    struct AmbientArgAllocaterCallback {
        virtual void *detail_AllocAmbientArg(u32 size) = 0;
        virtual void detail_FreeAmbientArg(void *arg, const BasicSound *sound) = 0;
    };

    struct AmbientArgInfo {
        AmbientParamUpdateCallback *mParamUpdateCallback;   // at 0x0
        AmbientArgUpdateCallback *mArgUpdateCallback;       // at 0x4
        AmbientArgAllocaterCallback *mArgAllocaterCallback; // at 0x8
        void *arg;                                          // at 0xC
        u32 argSize;                                        // at 0x10
    };

public:
    BasicSound();
    virtual ~BasicSound() {} // at 0xC

    virtual void Update();                      // at 0x10
    virtual void StartPrepared();               // at 0x14
    virtual void Stop(int frames);              // at 0x18
    virtual void Pause(bool flag, int frames);  // at 0x1C
    virtual void SetAutoStopCounter(int count); // at 0x20
    virtual void FadeIn(int frames);            // at 0x24
    virtual void Shutdown();                    // at 0x28
    virtual bool IsPrepared() const = 0;        // at 0x2C
    virtual bool IsPause() const;               // at 0x30

    virtual void SetInitialVolume(f32 vol);                // at 0x34
    virtual void SetVolume(f32 vol, int frames);           // at 0x38
    virtual void SetPitch(f32 pitch);                      // at 0x3C
    virtual void SetPan(f32 pan);                          // at 0x40
    virtual void SetSurroundPan(f32 pan);                  // at 0x44
    virtual void SetLpfFreq(f32 freq);                     // at 0x48
    virtual void SetPlayerPriority(int prio);              // at 0x4C
    virtual void SetRemoteFilter(int filter);              // at 0x50
    virtual void SetPanMode(PanMode mode);                 // at 0x54
    virtual void SetPanCurve(PanCurve curve);              // at 0x58
    virtual bool IsAttachedTempSpecialHandle() = 0;        // at 0x5C
    virtual void DetachTempSpecialHandle() = 0;            // at 0x60
    virtual void InitParam();                              // at 0x64
    virtual BasicPlayer *GetBasicPlayer() = 0;             // at 0x68
    virtual const BasicPlayer *GetBasicPlayer() const = 0; // at 0x6C

    PlayerHeap *GetPlayerHeap() {
        return mPlayerHeap;
    }
    void SetPlayerHeap(PlayerHeap *heap) {
        mPlayerHeap = heap;
    }

    bool IsAttachedGeneralHandle();
    void DetachGeneralHandle();

    bool IsAttachedTempGeneralHandle();
    void DetachTempGeneralHandle();

    SoundPlayer *GetSoundPlayer() {
        return mSoundPlayer;
    }
    void SetSoundPlayer(SoundPlayer *player) {
        mSoundPlayer = player;
    }

    ExternalSoundPlayer *GetExternalSoundPlayer() {
        return mExtSoundPlayer;
    }
    void SetExternalSoundPlayer(ExternalSoundPlayer *player) {
        mExtSoundPlayer = player;
    }

    AmbientParamUpdateCallback *GetAmbientParamUpdateCallback() {
        return mParamUpdateCallback;
    }

    AmbientArgUpdateCallback *GetAmbientArgUpdateCallback() {
        return mArgUpdateCallback;
    }
    void ClearAmbientArgUpdateCallback() {
        mArgUpdateCallback = NULL;
    }

    AmbientArgAllocaterCallback *GetAmbientArgAllocaterCallback() {
        return mArgAllocCallback;
    }

    void *GetAmbientArg() {
        return mCallbackArg;
    }

    SoundParam &GetAmbientParam() {
        return mAmbientParam;
    }

    void SetAmbientParamCallback(AmbientParamUpdateCallback *paramUpdate, AmbientArgUpdateCallback *argUpdate,
            AmbientArgAllocaterCallback *argAlloc, void *arg);

    u32 GetId() const {
        return mId;
    }
    void SetId(u32 id);

    f32 GetMoveVolume() {
        return mExtVolume.GetValue();
    }

    f32 GetInitialVolume() const;
    f32 GetPan() const;
    f32 GetSurroundPan() const;
    f32 GetPitch() const;

    void SetOutputLine(int flag);
    bool IsEnabledOutputLine() const;
    int GetOutputLine() const;

    f32 GetMainOutVolume() const;
    void SetMainOutVolume(f32 vol);

    f32 GetRemoteOutVolume(int remote) const;
    void SetRemoteOutVolume(int remote, f32 vol);

    void SetFxSend(AuxBus bus, f32 send);

    int CalcCurrentPlayerPriority() const {
        return ut::Clamp(mPriority + mAmbientParam.priority, PRIORITY_MIN, PRIORITY_MAX);
    }

public:
    static const int PRIORITY_MIN = 0;
    static const int PRIORITY_MAX = 127;

private:
    PlayerHeap *mPlayerHeap;                          // at 0x4
    SoundHandle *mGeneralHandle;                      // at 0x8
    SoundHandle *mTempGeneralHandle;                  // at 0xC
    SoundPlayer *mSoundPlayer;                        // at 0x10
    ExternalSoundPlayer *mExtSoundPlayer;             // at 0x14
    AmbientParamUpdateCallback *mParamUpdateCallback; // at 0x18
    AmbientArgUpdateCallback *mArgUpdateCallback;     // at 0x1C
    AmbientArgAllocaterCallback *mArgAllocCallback;   // at 0x20
    void *mCallbackArg;                               // at 0x24
    SoundParam mAmbientParam;                         // at 0x28
    MoveValue<f32, int> mFadeVolume;                  // at 0x44
    MoveValue<f32, int> mPauseFadeVolume;             // at 0x54
    bool mIsStarting;                                 // at 0x64
    bool mIsStarted;                                  // at 0x65
    bool mIsAutoStop;                                 // at 0x66
    bool mIsPause;                                    // at 0x67
    bool mIsPauseFade;                                // at 0x68
    bool mIsFadeOut;                                  // at 0x69
    int mAutoStopCounter;                             // at 0x6C
    u32 mUpdateCounter;                               // at 0x70
    u8 mPriority;                                     // at 0x74
    u32 mId;                                          // at 0x78
    MoveValue<f32, int> mExtVolume;                   // at 0x7C
    f32 mInitVolume;                                  // at 0x8C
    f32 mExtPan;                                      // at 0x90
    f32 mExtSurroundPan;                              // at 0x94
    f32 mExtPitch;                                    // at 0x98
    bool mEnableOutputLine;                           // at 0x9C
    int mOutputLine;                                  // at 0xA0
    f32 mMainOutVolume;                               // at 0xA4
    f32 mRemoteOutVolumes[WPAD_MAX_CONTROLLERS];      // at 0xA8

public:
    NW4R_UT_LIST_NODE_DECL_EX(Prio);       // at 0xB8
    NW4R_UT_LIST_NODE_DECL_EX(PlayerPlay); // at 0xC0
    NW4R_UT_LIST_NODE_DECL_EX(PlayerPrio); // at 0xC8
    NW4R_UT_LIST_NODE_DECL_EX(ExtPlay);    // at 0xD0
};

NW4R_UT_LIST_TYPEDEF_DECL_EX(BasicSound, Prio);
NW4R_UT_LIST_TYPEDEF_DECL_EX(BasicSound, PlayerPlay);
NW4R_UT_LIST_TYPEDEF_DECL_EX(BasicSound, PlayerPrio);
NW4R_UT_LIST_TYPEDEF_DECL_EX(BasicSound, ExtPlay);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
