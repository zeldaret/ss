#ifndef NW4R_SND_AX_MANAGER_H
#define NW4R_SND_AX_MANAGER_H
#include <nw4r/snd/snd_Common.h>
#include <nw4r/snd/snd_FxBase.h>
#include <nw4r/snd/snd_MoveValue.h>
#include <nw4r/types_nw4r.h>
#include <nw4r/ut.h>
#include <rvl/AI.h>
#include <rvl/AX.h>

namespace nw4r {
namespace snd {
namespace detail {

class AxManager {
public:
    struct CallbackListNode {
        NW4R_UT_LIST_NODE_DECL(); // at 0x0
        AXOutCallback callback;   // at 0x8
    };

    NW4R_UT_LIST_TYPEDEF_DECL(CallbackListNode);

public:
    static AxManager &GetInstance();

    AxManager();
    ~AxManager() {}

    bool CheckInit() {
        return mInitialized;
    }
    bool IsDiskError() const {
        return mDiskError;
    }
    s32 GetResetReadyTimer() const {
        return mResetReadyTimer;
    }
    FxBaseList &GetEffectList(AuxBus bus) {
        return mFxList[bus];
    }

    void Init();
    void Shutdown();
    f32 GetOutputVolume() const;
    void Update();
    void *GetZeroBufferAddress();

    void RegisterCallback(CallbackListNode *node, AXOutCallback callback);
    void UnregisterCallback(CallbackListNode *node);

    void SetOutputMode(OutputMode mode);
    OutputMode GetOutputMode();

    f32 GetMasterVolume() const {
        return mMasterVolume.GetValue();
    }
    void SetMasterVolume(f32 volume, int frame);

    bool AppendEffect(AuxBus bus, FxBase *fx);
    void ClearEffect(AuxBus bus, int frame);
    void ShutdownEffect(AuxBus bus);

    void PrepareReset();

private:
    static void AxCallbackFunc();
    static void AuxCallbackFunc(void *chans, void *context);
    static void AiDmaCallbackFunc();

private:
    static u8 sZeroBuffer[0x100];

    OutputMode mOutputMode;                          // at 0x0
    void *mZeroBuffer;                               // at 0x4
    CallbackListNodeList mCallbackList;              // at 0x8
    AXOutCallback mOldAxCallback;                    // at 0x14
    bool mInitialized;                               // at 0x18
    bool mUpdateVoicePrio;                           // at 0x19
    bool mDiskError;                                 // at 0x1A
    MoveValue<f32, int> mMasterVolume;               // at 0x1C
    MoveValue<f32, int> mMainOutVolume;              // at 0x2C
    MoveValue<f32, int> mVolumeForReset;             // at 0x3C
    AIDMACallback mOldAiCallback;                    // at 0x4C
    s32 mResetReadyTimer;                            // at 0x50
    MoveValue<f32, int> mAuxFadeVolume[AUX_BUS_NUM]; // at 0x54
    MoveValue<f32, int> mAuxUserVolume[AUX_BUS_NUM]; // at 0x84
    FxBaseList mFxList[AUX_BUS_NUM];                 // at 0xB4
    AXAuxCallback mAuxCallback[AUX_BUS_NUM];         // at 0xD8
    void *mAuxCallbackContext[AUX_BUS_NUM];          // at 0xE4
    u8 mAuxCallbackWait[AUX_BUS_NUM];                // at 0xF0
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
