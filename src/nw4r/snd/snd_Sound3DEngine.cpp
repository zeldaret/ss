#include "nw4r/snd/snd_Sound3DEngine.h"

#include "nw4r/snd/snd_Sound3DCalculator.h"
#include "nw4r/snd/snd_Sound3DListener.h"
#include "nw4r/snd/snd_Sound3DManager.h"
#include "nw4r/snd/snd_global.h"

namespace nw4r {
namespace snd {

Sound3DEngine::Sound3DEngine() {}

void Sound3DEngine::UpdateAmbientParam(
    const Sound3DManager *mgr, const Sound3DParam *pParam, u32, u32 flags, SoundAmbientParam *pAmbientParam
) {
    if (flags & 1) {
        pAmbientParam->volume = 0.0f;
    }

    if (flags & 8) {
        pAmbientParam->priority = -mgr->GetMaxPriorityReduction();
    }

    if (mgr->GetListenerList().GetSize() > 1) {
        flags &= -0x17;
    }

    NW4R_RANGE_FOR(it, mgr->GetListenerList()) {
        const Sound3DListener &listener = *it;
        if (flags & 9) {
            f32 volume;
            int priority;
            Sound3DCalculator::CalcVolumeAndPriority(*mgr, listener, *pParam, &volume, &priority);
            if (flags & 1) {
                pAmbientParam->volume = ut::Max(volume, pAmbientParam->volume);
            }

            if (flags & 8) {
                pAmbientParam->priority = ut::Max(priority, pAmbientParam->priority);
            }
        }
        if (flags & 6) {
            f32 pan, surroundPan;
            Sound3DCalculator::CalcPan(*mgr, listener, *pParam, mPanParam, &pan, &surroundPan);
            if (flags & 2) {
                pAmbientParam->pan = pan;
            }
            if (flags & 4) {
                pAmbientParam->surroundPan = surroundPan;
            }
        }

        if (flags & 0x10) {
            f32 pitch;
            Sound3DCalculator::CalcPitch(*mgr, listener, *pParam, &pitch);
            pAmbientParam->pitch = pitch;
        }

        if (flags & 0x20) {
            f32 biquadFilterValue;
            Sound3DCalculator::CalcBiquadFilterValue(*mgr, listener, *pParam, &biquadFilterValue);
            pAmbientParam->biquadFilterType = mgr->GetBiquadFilterType();
            pAmbientParam->biquadFilterValue = ut::Min(biquadFilterValue, pAmbientParam->biquadFilterValue);
        }
    }
}

void Sound3DEngine::UpdateAmbientParam(
    const Sound3DManager *mgr, const Sound3DParam *param, u32 unused1, int unused2, SoundAmbientParam *pAmbientParam
) {
    u32 flags = 0x1F;
    if (param->field_0x18 & 1) {
        flags &= ~1;
    }
    if (param->field_0x18 & 2) {
        flags &= ~2;
    }
    if (param->field_0x18 & 4) {
        flags &= ~4;
    }
    if (param->field_0x18 & 8) {
        flags &= ~8;
    }
    if (!param->field_0x1E) {
        flags &= ~0x10;
    }
    if (param->field_0x18 & 0x10) {
        flags |= 0x20;
    }
    UpdateAmbientParam(mgr, param, unused1, flags, pAmbientParam);
}

s32 Sound3DEngine::GetAmbientPriority(const Sound3DManager *mgr, const Sound3DParam *param, u32 arg) {
    u32 flags = 0x1000008;
    if (param->field_0x18 & 8) {
        flags &= ~8;
    }

    SoundAmbientParam ambientParam;

    UpdateAmbientParam(mgr, param, arg, flags, &ambientParam);
    return ambientParam.priority;
}

s32 Sound3DEngine::GetRequiredVoiceOutCount(const Sound3DManager *, const Sound3DParam *, u32) {
    return 1;
}

} // namespace snd
} // namespace nw4r
