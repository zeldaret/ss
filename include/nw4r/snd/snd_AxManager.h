#ifndef NW4R_SND_AX_MANAGER_H
#define NW4R_SND_AX_MANAGER_H
#include "nw4r/snd/snd_BiquadFilterPreset.h"
#include "nw4r/snd/snd_Common.h"
#include "nw4r/snd/snd_FxBase.h"
#include "nw4r/snd/snd_MoveValue.h"
#include "nw4r/types_nw4r.h"
#include "nw4r/ut.h" // IWYU pragma: export
#include "rvl/AI.h"  // IWYU pragma: export
#include "rvl/AX.h"  // IWYU pragma: export

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

    void Init();
    void Shutdown();
    void Update();
    void PrepareReset();

    bool CheckInit() {
        return mInitialized;
    }

    bool IsResetReady() const {
        return mResetReadyCounter == 0;
    }

    f32 GetOutputVolume() const;
    void *GetZeroBufferAddress();

    void RegisterCallback(CallbackListNode *pNode, AXOutCallback pCallback);
    void UnregisterCallback(CallbackListNode *pNode);

    void SetOutputMode(OutputMode mode);
    OutputMode GetOutputMode();

    f32 GetMasterVolume() const {
        return mMasterVolume.GetValue();
    }
    void SetMasterVolume(f32 volume, int frame);

    bool AppendEffect(AuxBus bus, FxBase *pFx);
    void ClearEffect(AuxBus bus, int frame);
    void ShutdownEffect(AuxBus bus);

    FxBaseList &GetEffectList(AuxBus bus) {
        return mFxList[bus];
    }

private:
    static const u8 AUX_CALLBACK_WAIT_FRAME = 6;

    static const int FX_SAMPLE_RATE = AX_SAMPLE_RATE;
    static const SampleFormat FX_SAMPLE_FORMAT = SAMPLE_FORMAT_PCM_S32;
    static const int FX_BUFFER_SIZE = AX_FRAME_SIZE;

    static const int ZERO_BUFFER_SIZE = 256;

private:
    AxManager();

    static void AxCallbackFunc();
    static void AuxCallbackFunc(void *pChans, void *pContext);
    static void AiDmaCallbackFunc();

private:
    OutputMode mOutputMode;                          // at 0x0
    void *mZeroBufferAddress;                        // at 0x4
    CallbackListNodeList mCallbackList;              // at 0x8
    AXOutCallback mNextAxRegisterCallback;           // at 0x14
    bool mInitialized;                               // at 0x18
    bool mUpdateVoicePrioFlag;                       // at 0x19
    MoveValue<f32, int> mMasterVolume;               // at 0x1C
    MoveValue<f32, int> mMainOutVolume;              // at 0x2C
    MoveValue<f32, int> mVolumeForReset;             // at 0x3C
    AIDMACallback mOldAidCallback;                   // at 0x4C
    volatile s32 mResetReadyCounter;                 // at 0x50
    MoveValue<f32, int> mAuxFadeVolume[AUX_BUS_NUM]; // at 0x54
    MoveValue<f32, int> mAuxUserVolume[AUX_BUS_NUM]; // at 0x84
    FxBaseList mFxList[AUX_BUS_NUM];                 // at 0xB4
    AXAuxCallback mAuxCallback[AUX_BUS_NUM];         // at 0xD8
    void *mAuxCallbackContext[AUX_BUS_NUM];          // at 0xE4
    u8 mAuxCallbackWaitCounter[AUX_BUS_NUM];         // at 0xF0
    u32 mEffectProcessTick[AUX_BUS_NUM];             // at 0xF4
    u32 field_0x100;                                 // at 0x100

    static u8 sZeroBuffer[ZERO_BUFFER_SIZE];

    static const BiquadFilterLpf sBiquadFilterLpf;
    static const BiquadFilterHpf sBiquadFilterHpf;
    static const BiquadFilterBpf512 sBiquadFilterBpf512;
    static const BiquadFilterBpf1024 sBiquadFilterBpf1024;
    static const BiquadFilterBpf2048 sBiquadFilterBpf2048;
    static const BiquadFilterCallback *sBiquadFilterCallbackTable[0x80];
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
