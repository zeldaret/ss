#include "nw4r/snd.h"
#include "nw4r/snd/snd_SoundStartable.h"


namespace nw4r {
namespace snd {

SoundStartable::StartResult
SoundStartable::detail_StartSound(SoundHandle *pHandle, u32 id, const StartInfo *pStartInfo) {
    StartResult result = detail_SetupSound(pHandle, id, false, pStartInfo);

    if (result != START_SUCCESS) {
        return result;
    }

    pHandle->StartPrepared();
    return START_SUCCESS;
}

SoundStartable::StartResult
SoundStartable::detail_StartSound(SoundHandle *pHandle, const char *name, const StartInfo *pStartInfo) {
    u32 id = detail_ConvertLabelStringToSoundId(name);
    if (id == -1) {
        return START_ERR_INVALID_LABEL_STRING;
    }
    return detail_StartSound(pHandle, id, pStartInfo);
}

SoundStartable::StartResult
SoundStartable::detail_HoldSound(SoundHandle *pHandle, u32 id, const StartInfo *pStartInfo) {
    if (pHandle->IsAttachedSound() && id == pHandle->GetId()) {
        pHandle->detail_GetAttachedSound()->SetAutoStopCounter(1);
        return START_SUCCESS;
    }

    StartResult result = detail_SetupSound(pHandle, id, true, pStartInfo);

    if (result != START_SUCCESS) {
        return result;
    }

    pHandle->StartPrepared();
    pHandle->detail_GetAttachedSound()->SetAutoStopCounter(1);
    return START_SUCCESS;
}

SoundStartable::StartResult
SoundStartable::detail_HoldSound(SoundHandle *pHandle, const char *name, const StartInfo *pStartInfo) {
    u32 id = detail_ConvertLabelStringToSoundId(name);
    if (id == -1) {
        return START_ERR_INVALID_LABEL_STRING;
    }
    return detail_HoldSound(pHandle, id, pStartInfo);
}

SoundStartable::StartResult
SoundStartable::detail_PrepareSound(SoundHandle *handle, u32 targetID, const StartInfo *startInfo) {
    return detail_SetupSound(handle, targetID, false, startInfo);
}

SoundStartable::StartResult
SoundStartable::detail_PrepareSound(SoundHandle *pHandle, const char *name, const StartInfo *pStartInfo) {
    u32 id = detail_ConvertLabelStringToSoundId(name);
    if (id == -1) {
        return START_ERR_INVALID_LABEL_STRING;
    }
    return detail_PrepareSound(pHandle, id, pStartInfo);
}

} // namespace snd
} // namespace nw4r
