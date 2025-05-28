#include "nw4r/snd/snd_FxReverbStdDpl2.h"

#include "common.h"
#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/ut/ut_algorithm.h"

namespace nw4r {
namespace snd {

FxReverbStdDpl2::FxReverbStdDpl2() : mIsActive(0) {
    detail::FxReverbStdParam p;
    SetParam(p);
}

u32 FxReverbStdDpl2::GetRequiredMemSize() {
    return ut::Max(
        ut::RoundUp(AXFXReverbStdExpGetMemSizeDpl2(&mAxfxParamDpl) + detail::AxfxImpl::HEAP_SIZE_MIN, 32),
        ut::RoundUp(AXFXReverbStdExpGetMemSize(&mAxfxParam) + detail::AxfxImpl::HEAP_SIZE_MIN, 32)
    );
}

bool FxReverbStdDpl2::AssignWorkBuffer(void *pBuffer, u32 size) {
    return mImpl.CreateHeap(pBuffer, size);
}

void FxReverbStdDpl2::ReleaseWorkBuffer() {
    mImpl.DestroyHeap();
}

bool FxReverbStdDpl2::StartUp() {
    u32 memSize = GetRequiredMemSize();
    if (memSize > mImpl.GetHeapTotalSize()) {
        return false;
    }

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    BOOL success;
    switch (detail::AxManager::GetInstance().GetOutputMode()) {
        case OUTPUT_MODE_DPL2: {
            mOutputMode = 1;

            mImpl.HookAlloc(&allocHook, &freeHook);
            success = AXFXReverbStdExpInitDpl2(&mAxfxParamDpl);
            mImpl.RestoreAlloc(allocHook, freeHook);

            (void)AXFXReverbStdExpGetMemSizeDpl2(&mAxfxParamDpl); // debug leftover
            break;
        }
        default: {
            mOutputMode = 0;

            mImpl.HookAlloc(&allocHook, &freeHook);
            success = AXFXReverbStdExpInit(&mAxfxParam);
            mImpl.RestoreAlloc(allocHook, freeHook);

            (void)AXFXReverbStdExpGetMemSize(&mAxfxParam); // debug leftover
            break;
        }
    }

    mIsActive = 1;

    return success;
}

void FxReverbStdDpl2::Shutdown() {
    if (!mIsActive) {
        return;
    }
    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    mIsActive = 0;
    mImpl.HookAlloc(&allocHook, &freeHook);

    switch (mOutputMode) {
        case 1: {
            AXFXReverbStdExpShutdownDpl2(&mAxfxParamDpl);
            break;
        }
        default: {
            AXFXReverbStdExpShutdown(&mAxfxParam);
            break;
        }
    }
    mImpl.RestoreAlloc(allocHook, freeHook);
}

bool FxReverbStdDpl2::SetParam(const detail::FxReverbStdParam &rParam) {
    mParam = rParam;

    f32 f1 = ut::Max(rParam.field_0x18, 0.0f);
    bool b = f1 != mAxfxParam.field_0xB8;

    mAxfxParam.field_0xB4 = mAxfxParamDpl.field_0xD0 = rParam.field_0x14;
    mAxfxParam.field_0xB8 = mAxfxParamDpl.field_0xD4 = f1;

    mAxfxParam.field_0xBC = mAxfxParamDpl.field_0xD8 = ut::Clamp(rParam.field_0x00, 0.0f, rParam.field_0x18);
    mAxfxParam.field_0xC0 = mAxfxParamDpl.field_0xDC = rParam.field_0x1C;
    mAxfxParam.field_0xC4 = mAxfxParamDpl.field_0xE0 = ut::Max(rParam.field_0x04, 0.0f);
    mAxfxParam.field_0xC8 = mAxfxParamDpl.field_0xE4 = ut::Clamp(rParam.field_0x08, 0.0f, 1.0f);
    mAxfxParam.field_0xCC = mAxfxParamDpl.field_0xE8 = ut::Clamp(rParam.field_0x0C, 0.0f, 1.0f);
    mAxfxParam.field_0xD0 = mAxfxParamDpl.field_0xEC = ut::Clamp(rParam.field_0x20, 0.0f, 1.0f);
    mAxfxParam.field_0xD4 = mAxfxParamDpl.field_0xF0 = ut::Clamp(rParam.field_0x24, 0.0f, 1.0f);
    mAxfxParam.field_0xE0 = mAxfxParamDpl.field_0xFC = ut::Clamp(rParam.field_0x10, 0.0f, 1.0f);
    
    mAxfxParam.field_0xD8 = 0;
    mAxfxParam.field_0xDC = 0;
    mAxfxParam.field_0xE4 = 0.0f;

    mAxfxParamDpl.field_0xF4 = 0;
    mAxfxParamDpl.field_0xF8 = 0;
    mAxfxParamDpl.field_0x100 = 0.0f;

    if (!mIsActive) {
        return true;
    }

    u32 memSize = GetRequiredMemSize();
    if (memSize > mImpl.GetHeapTotalSize()) {
        return false;
    }

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    BOOL success;
    if (b) {
        mImpl.HookAlloc(&allocHook, &freeHook);
        switch (mOutputMode) {
            case 1:  success = AXFXReverbStdExpSettingsDpl2(&mAxfxParamDpl); break;
            default: success = AXFXReverbStdExpSettings(&mAxfxParam); break;
        }

        mImpl.RestoreAlloc(allocHook, freeHook);
    } else {
        switch (mOutputMode) {
            case 1:  success = AXFXReverbStdExpSettingsUpdateDpl2(&mAxfxParamDpl); break;
            default: success = AXFXReverbStdExpSettingsUpdate(&mAxfxParam); break;
        }
    }

    return success;
}

void FxReverbStdDpl2::UpdateBuffer(
    int channels, void **ppBuffer, u32 size, SampleFormat sampleFormat, f32 sampleRate, OutputMode outputMode
) {
    if (!mIsActive) {
        return;
    }

    if ((outputMode == OUTPUT_MODE_DPL2) != mOutputMode) {
        return;
    }

    switch (mOutputMode) {
        case 1: {
            void *chans[AX_DPL2_MAX];
            chans[AX_DPL2_L] = ppBuffer[AX_DPL2_L];
            chans[AX_DPL2_R] = ppBuffer[AX_DPL2_R];
            chans[AX_DPL2_LS] = ppBuffer[AX_DPL2_LS];
            chans[AX_DPL2_RS] = ppBuffer[AX_DPL2_RS];
            AXFXReverbStdExpCallbackDpl2(chans, &mAxfxParamDpl);
            break;
        }
        default: {
            void *chans[3];
            chans[0] = ppBuffer[0];
            chans[1] = ppBuffer[1];
            chans[2] = ppBuffer[2];
            AXFXReverbStdExpCallback(chans, &mAxfxParam);
            break;
        }
    }
}

void FxReverbStdDpl2::OnChangeOutputMode() {
    if (mOutputMode != (detail::AxManager::GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2)) {
        Shutdown();
        StartUp();
    }
}

} // namespace snd
} // namespace nw4r
