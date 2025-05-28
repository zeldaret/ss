#include "nw4r/snd/snd_Sound3DManager.h"

#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_Sound3DEngine.h"
#include "nw4r/snd/snd_Sound3DListener.h"
#include "nw4r/snd/snd_SoundHandle.h"

#include "nw4r/math.h"

#include <cmath>

namespace nw4r {
namespace snd {
namespace {

static Sound3DEngine sEngine;

} // namespace

Sound3DManager::Sound3DManager()
    : mpEngine(&sEngine), mMaxPriorityReduction(32), field_0x20(0.9f), field_0x24(0.0f), mBiquadFilterType(0) {}

u32 Sound3DManager::GetRequiredMemSize(const SoundArchive *pArchive) {
    u32 numParam = 0;

    SoundArchive::SoundArchivePlayerInfo playerInfo;
    if (pArchive->ReadSoundArchivePlayerInfo(&playerInfo)) {
        numParam += playerInfo.seqSoundCount;
        numParam += playerInfo.strmSoundCount;
        numParam += playerInfo.waveSoundCount;
    }

    return numParam * sizeof(Sound3DParam);
}

bool Sound3DManager::Setup(const SoundArchive *pArchive, void *pBuffer, u32 size) {
#pragma unused(pArchive)

    mParamPool.Create(pBuffer, size);
    return true;
}

void Sound3DManager::SetEngine(Sound3DEngine *engine) {
    mpEngine = engine;
}

void Sound3DManager::detail_UpdateAmbientParam(
    const void *param, u32 unused1, int unused2, SoundAmbientParam *ambientParam
) {
    const Sound3DParam *p = static_cast<const Sound3DParam *>(param);
    if (mpEngine != NULL) {
        mpEngine->UpdateAmbientParam(this, p, unused1, unused2, ambientParam);
    }
}

int Sound3DManager::detail_GetAmbientPriority(const void *param, u32 arg) {
    int result = 0;
    if (mpEngine != NULL) {
        const Sound3DParam *p = static_cast<const Sound3DParam *>(param);
        result = mpEngine->GetAmbientPriority(this, p, arg);
    }
    return result;
}

int Sound3DManager::detail_GetRequiredVoiceOutCount(const void *param, u32 arg) {
    int result = 1;
    if (mpEngine != NULL) {
        const Sound3DParam *p = static_cast<const Sound3DParam *>(param);
        result = mpEngine->GetRequiredVoiceOutCount(this, p, arg);
    }
    return result;
}

void *Sound3DManager::detail_AllocAmbientArg(u32 size) {
    if (size != sizeof(Sound3DParam)) {
        return NULL;
    }

    return mParamPool.Alloc();
}

void Sound3DManager::detail_FreeAmbientArg(void *pArg, const detail::BasicSound *pSound) {
#pragma unused(pSound)

    mParamPool.Free(static_cast<Sound3DParam *>(pArg));
}

void Sound3DManager::SetBiquadFilterType(int type) {
    mBiquadFilterType = type;
}

Sound3DParam::Sound3DParam() {
    field_0x18 = 0;
    decayCurve = Sound3DManager::DECAY_CURVE_LOGARITHMIC;
    decayRatio = 128;
    field_0x1E = 0;
    field_0x20 = 0;
    field_0x24 = 0;
}

} // namespace snd
} // namespace nw4r
