#ifndef NW4R_SND_BASIC_SOUND_H
#define NW4R_SND_BASIC_SOUND_H
#include "nw4r/snd/snd_Common.h"
#include "nw4r/snd/snd_MoveValue.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h"  // IWYU pragma: export
#include "rvl/WPAD.h" // IWYU pragma: export

namespace nw4r {
namespace snd {
namespace detail {

class BasicSound {
    friend class SoundHandle;

public:
    NW4R_UT_RTTI_DECL(BasicSound);

    struct AmbientParamUpdateCallback {
        enum ParamUpdateFlags {
            PARAM_UPDATE_VOLUME = (1 << 0),
            PARAM_UPDATE_PAN = (1 << 1),
            PARAM_UPDATE_SURROUND_PAN = (1 << 2),
            PARAM_UPDATE_PRIORITY = (1 << 3),
        };

        virtual void detail_Update(
            SoundParam *pParam, u32 id, BasicSound *pSound, const void *pArg,
            u32 flags
        ) = 0; // at 0xC
    };

    struct AmbientArgUpdateCallback {
        virtual void detail_Update(void *pArg,
                                   const BasicSound *pSound) = 0; // at 0xC
    };

    struct AmbientArgAllocaterCallback {
        virtual void *detail_AllocAmbientArg(u32 size) = 0; // at 0xC

        virtual void detail_FreeAmbientArg(void *pArg,
                                           const BasicSound *pSound) = 0; // at 0x10
    };

    struct AmbientArgInfo {
        AmbientParamUpdateCallback *paramUpdateCallback;   // at 0x0
        AmbientArgUpdateCallback *argUpdateCallback;       // at 0x4
        AmbientArgAllocaterCallback *argAllocaterCallback; // at 0x8
        void *arg;                                         // at 0xC
        u32 argSize;                                       // at 0x10
    };

    static const u32 INVALID_ID = 0xFFFFFFFF;
    static const int PRIORITY_MAX = 127;

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

    virtual void SetInitialVolume(f32 vol);       // at 0x34
    virtual void SetVolume(f32 vol, int frames);  // at 0x38
    virtual void SetPitch(f32 pitch);             // at 0x3C
    virtual void SetPan(f32 pan);                 // at 0x40
    virtual void SetSurroundPan(f32 pan);         // at 0x44
    virtual void SetLpfFreq(f32 freq);            // at 0x48
    virtual void SetPlayerPriority(int priority); // at 0x4C
    virtual void SetRemoteFilter(int filter);     // at 0x50
    virtual void SetPanMode(PanMode mode);        // at 0x54
    virtual void SetPanCurve(PanCurve curve);     // at 0x58

    virtual bool IsAttachedTempSpecialHandle() = 0; // at 0x5C
    virtual void DetachTempSpecialHandle() = 0;     // at 0x60

    virtual void InitParam();                              // at 0x64
    virtual BasicPlayer &GetBasicPlayer() = 0;             // at 0x68
    virtual const BasicPlayer &GetBasicPlayer() const = 0; // at 0x6C

    PlayerHeap *GetPlayerHeap() {
        return mHeap;
    }
    void SetPlayerHeap(PlayerHeap *pHeap) {
        mHeap = pHeap;
    }

    bool IsAttachedGeneralHandle();
    void DetachGeneralHandle();

    bool IsAttachedTempGeneralHandle();
    void DetachTempGeneralHandle();

    SoundPlayer *GetSoundPlayer() {
        return mSoundPlayer;
    }
    void SetSoundPlayer(SoundPlayer *pPlayer) {
        mSoundPlayer = pPlayer;
    }

    ExternalSoundPlayer *GetExternalSoundPlayer() {
        return mExtSoundPlayer;
    }
    void SetExternalSoundPlayer(ExternalSoundPlayer *pExtPlayer) {
        mExtSoundPlayer = pExtPlayer;
    }

    AmbientParamUpdateCallback *GetAmbientParamUpdateCallback() {
        return mAmbientParamUpdateCallback;
    }

    AmbientArgUpdateCallback *GetAmbientArgUpdateCallback() {
        return mAmbientArgUpdateCallback;
    }
    void ClearAmbientArgUpdateCallback() {
        mAmbientArgUpdateCallback = NULL;
    }

    AmbientArgAllocaterCallback *GetAmbientArgAllocaterCallback() {
        return mAmbientArgAllocaterCallback;
    }

    void *GetAmbientArg() {
        return mAmbientArg;
    }

    SoundParam &GetAmbientParam() {
        return mAmbientParam;
    }

    void SetAmbientParamCallback(
        AmbientParamUpdateCallback *pParamUpdate, AmbientArgUpdateCallback *pArgUpdate,
        AmbientArgAllocaterCallback *pArgAlloc, void *pArg
    );

    void SetPriority(int priority) {
        mPriority = priority;
    }

    u32 GetId() const {
        return mId;
    }
    void SetId(u32 id);

    f32 GetMoveVolume() {
        return mExtMoveVolume.GetValue();
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
        return ut::Clamp(mPriority + mAmbientParam.priority, 0, PRIORITY_MAX);
    }

private:
    PlayerHeap *mHeap;                    // at 0x4
    SoundHandle *mGeneralHandle;          // at 0x8
    SoundHandle *mTempGeneralHandle;      // at 0xC
    SoundPlayer *mSoundPlayer;            // at 0x10
    ExternalSoundPlayer *mExtSoundPlayer; // at 0x14

    AmbientParamUpdateCallback *mAmbientParamUpdateCallback;   // at 0x18
    AmbientArgUpdateCallback *mAmbientArgUpdateCallback;       // at 0x1C
    AmbientArgAllocaterCallback *mAmbientArgAllocaterCallback; // at 0x20
    void *mAmbientArg;                                         // at 0x24
    SoundParam mAmbientParam;                                  // at 0x28

    MoveValue<f32, int> mFadeVolume;      // at 0x44
    MoveValue<f32, int> mPauseFadeVolume; // at 0x54
    bool mStartFlag;                      // at 0x64
    bool mStartedFlag;                    // at 0x65
    bool mAutoStopFlag;                   // at 0x66
    bool mPauseFlag;                      // at 0x67
    bool mPauseFadeFlag;                  // at 0x68
    bool mFadeOutFlag;                    // at 0x69
    int mAutoStopCounter;                 // at 0x6C
    u32 mUpdateCounter;                   // at 0x70

    u8 mPriority; // at 0x74
    u32 mId;      // at 0x78

    MoveValue<f32, int> mExtMoveVolume; // at 0x7C
    f32 mInitVolume;                    // at 0x8C
    f32 mExtPan;                        // at 0x90
    f32 mExtSurroundPan;                // at 0x94
    f32 mExtPitch;                      // at 0x98

    bool mOutputLineFlagEnable;                 // at 0x9C
    int mOutputLineFlag;                        // at 0xA0
    f32 mMainOutVolume;                         // at 0xA4
    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // at 0xA8

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
