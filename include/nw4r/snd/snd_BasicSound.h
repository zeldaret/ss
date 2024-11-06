#ifndef NW4R_SND_BASIC_SOUND_H
#define NW4R_SND_BASIC_SOUND_H
#include "common.h"
#include "nw4r/snd/snd_Common.h"
#include "nw4r/snd/snd_MoveValue.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h"  // IWYU pragma: export
#include "rvl/WPAD.h" // IWYU pragma: export

namespace nw4r {
namespace snd {
namespace detail {

struct SoundActorParam {
    f32 volume;
    f32 pitch;
    f32 pan;
};

enum PauseState {
    PAUSE_STATE_NORMAL,
    PAUSE_STATE_PAUSING,
    PAUSE_STATE_PAUSED,
    PAUSE_STATE_UNPAUSING,
};

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

        virtual ~AmbientParamUpdateCallback() {}

        virtual void detail_Update(
            SoundParam *pParam, u32 id, BasicSound *pSound, const void *pArg,
            u32 flags
        ) = 0; // at 0xC
    };

    struct AmbientArgUpdateCallback {
        virtual ~AmbientArgUpdateCallback() {}
        virtual void detail_Update(void *pArg,
                                   const BasicSound *pSound) = 0; // at 0xC
    };

    struct AmbientArgAllocaterCallback {
        virtual ~AmbientArgAllocaterCallback() {}
        virtual void *detail_AllocAmbientArg(u32 size) = 0; // at 0xC

        virtual void detail_FreeAmbientArg(void *pArg,
                                           const BasicSound *pSound) = 0; // at 0x10
    };

    struct AmbientInfo {
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

    void Update();                       // at 0x10
    void StartPrepared();                // at 0x14
    void Stop(int frames);               // at 0x18
    void Pause(bool flag, int frames);   // at 0x1C
    void SetAutoStopCounter(int count);  // at 0x20
    void FadeIn(int frames);             // at 0x24
    virtual void Shutdown();             // at 0x28
    virtual bool IsPrepared() const = 0; // at 0x2C
    bool IsPause() const;                // at 0x30

    void SetInitialVolume(f32 vol);       // at 0x34
    void SetVolume(f32 vol, int frames);  // at 0x38
    void SetPitch(f32 pitch);             // at 0x3C
    void SetPan(f32 pan);                 // at 0x40
    void SetSurroundPan(f32 pan);         // at 0x44
    void SetLpfFreq(f32 freq);            // at 0x48
    void SetPlayerPriority(int priority); // at 0x4C
    void SetRemoteFilter(int filter);     // at 0x50
    void SetPanMode(PanMode mode);        // at 0x54
    void SetPanCurve(PanCurve curve);     // at 0x58

    virtual bool IsAttachedTempSpecialHandle() = 0; // at 0x5C
    virtual void DetachTempSpecialHandle() = 0;     // at 0x60

    virtual void InitParam();                              // at 0x64
    virtual BasicPlayer &GetBasicPlayer() = 0;             // at 0x68
    virtual const BasicPlayer &GetBasicPlayer() const = 0; // at 0x6C

    virtual void OnUpdatePlayerPriority() {}
    virtual void UpdateMoveValue() {}
    virtual void UpdateParam() {}

    PlayerHeap *GetPlayerHeap() {
        return mPlayerHeap;
    }

    bool IsAttachedGeneralHandle();
    void DetachGeneralHandle();

    bool IsAttachedTempGeneralHandle();
    void DetachTempGeneralHandle();
    void AttachPlayerHeap(PlayerHeap *pHeap);
    void DetachPlayerHeap(PlayerHeap *pHeap);

    SoundPlayer *GetSoundPlayer() {
        return mSoundPlayer;
    }
    void SetSoundPlayer(SoundPlayer *pPlayer) {
        mSoundPlayer = pPlayer;
    }

    void AttachSoundPlayer(SoundPlayer *pPlayer);
    void DetachSoundPlayer(SoundPlayer *pPlayer);

    ExternalSoundPlayer *GetExternalSoundPlayer() {
        return mExtSoundPlayer;
    }
    void SetExternalSoundPlayer(ExternalSoundPlayer *pExtPlayer) {
        mExtSoundPlayer = pExtPlayer;
    }

    AmbientParamUpdateCallback *GetAmbientParamUpdateCallback() {
        return mAmbientInfo.paramUpdateCallback;
    }

    AmbientArgUpdateCallback *GetAmbientArgUpdateCallback() {
        return mAmbientInfo.argUpdateCallback;
    }
    void ClearAmbientArgUpdateCallback() {
        mAmbientInfo.paramUpdateCallback = NULL;
    }

    AmbientArgAllocaterCallback *GetAmbientArgAllocaterCallback() {
        return mAmbientInfo.argAllocaterCallback;
    }

    void *GetAmbientArg() {
        return mAmbientInfo.arg;
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
    PlayerHeap *mPlayerHeap;              // at 0x4
    SoundHandle *mGeneralHandle;          // at 0x8
    SoundHandle *mTempGeneralHandle;      // at 0xC
    SoundPlayer *mSoundPlayer;            // at 0x10
    SoundActor *mSoundActor;              // at 0x14
    ExternalSoundPlayer *mExtSoundPlayer; // at 0x18

    AmbientInfo mAmbientInfo;    // at 0x18
    SoundParam mAmbientParam;    // at 0x30
    SoundActorParam mActorParam; // at 0x54

    UNKWORD field_0x60;
    // needs to be 0x64
    MoveValue<f32, int> mFadeVolume;      // at 0x64
    MoveValue<f32, int> mPauseFadeVolume; // at 0x74
    bool mStartFlag;                      // at 0x84
    bool mStartedFlag;                    // at 0x85
    bool mAutoStopFlag;                   // at 0x86
    bool mFadeOutFlag;                    // at 0x87
    PauseState mPauseState;               // at 0x88
    bool mUnPauseFlag;                    // at 0x8C
    int mAutoStopCounter;                 // at 0x90
    u32 mUpdateCounter;                   // at 0x94

    u8 mPriority;         // at 0x98
    u8 mVoiceOutCount;    // at 0x99
    u8 mBiquadFilterType; // at 0x9A
    u32 mId; // at 0x9B

    MoveValue<f32, int> mExtMoveVolume; // at 0xA0
    f32 mInitVolume;                    // at 0xB0
    f32 mExtPan;                        // at 0xB4
    f32 mExtSurroundPan;                // at 0xB8
    f32 mExtPitch;                      // at 0xBC

    f32 mLpfFreq;                               // at 0xC0
    f32 mBiquadFilterValue;                     // at 0xC4
    int mOutputLineFlag;                        // at 0xC8
    f32 mMainOutVolume;                         // at 0xCC
    f32 mMainSend;                              // at 0xD0
    f32 mFxSend[AUX_BUS_NUM];          // at 0xD4
    f32 mRemoteOutVolume[WPAD_MAX_CONTROLLERS]; // at 0xF0

public:
    NW4R_UT_LIST_NODE_DECL_EX(Prio);       // at 0x100
    NW4R_UT_LIST_NODE_DECL_EX(PlayerPlay); // at 0x108
    NW4R_UT_LIST_NODE_DECL_EX(PlayerPrio); // at 0x110
    NW4R_UT_LIST_NODE_DECL_EX(ExtPlay);    // at 0x118
};

NW4R_UT_LIST_TYPEDEF_DECL_EX(BasicSound, Prio);
NW4R_UT_LIST_TYPEDEF_DECL_EX(BasicSound, PlayerPlay);
NW4R_UT_LIST_TYPEDEF_DECL_EX(BasicSound, PlayerPrio);
NW4R_UT_LIST_TYPEDEF_DECL_EX(BasicSound, ExtPlay);

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
