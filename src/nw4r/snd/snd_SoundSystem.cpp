#include "nw4r/snd/snd_SoundSystem.h"
#include "nw4r/snd/snd_SeqPlayer.h"
#include "nw4r/snd/snd_SoundThread.h"
#include "nw4r/snd/snd_ChannelManager.h"
#include "nw4r/snd/snd_VoiceManager.h"
#include "nw4r/snd/snd_AxVoiceManager.h"
#include "nw4r/snd/snd_TaskManager.h"
#include "rvl/AX/AXVPB.h"

#include <rvl/AX.h>
#include <rvl/OS.h>
#include <rvl/SC.h>

namespace {

const char *NW4R_SND_Version_ = "<< NW4R    - SND \tfinal   build: Sep 25 2011 05:32:58 (0x4302_202) >>";
static bool sInitialized = false;

} // namespace

namespace nw4r {
namespace snd {

detail::TaskThread SoundSystem::sTaskThread;
int SoundSystem::sMaxVoices;

void SoundSystem::InitSoundSystem(s32 soundThreadPriority,
                                  s32 dvdThreadPriority) {
    // TODO this size is wrong, but where?
    const int defaultWorkSize = 0x155E0; /*DEFAULT_SOUND_THREAD_STACK_SIZE +
                                DEFAULT_DVD_THREAD_STACK_SIZE +
                                detail::AxVoiceManager::WORK_SIZE_MAX +
                                detail::VoiceManager::WORK_SIZE_MAX +
                                detail::ChannelManager::WORK_SIZE_MAX;*/

    static u8 defaultSoundSystemWork[defaultWorkSize] ALIGN_DECL(32);

    SoundSystemParam param;
    param.soundThreadPriority = soundThreadPriority;
    param.dvdThreadPriority = dvdThreadPriority;

    // @bug This function ignores the specified buffer size
    InitSoundSystem(param, defaultSoundSystemWork,
                    sizeof(defaultSoundSystemWork));
}

u32 SoundSystem::GetRequiredMemSize(const SoundSystemParam& rParam) {
    int numVoices = __AXGetNumVoices();
    return rParam.soundThreadStackSize + rParam.dvdThreadStackSize +
           detail::AxVoiceManager::GetInstance().GetRequiredMemSize(numVoices) +
           detail::VoiceManager::GetInstance().GetRequiredMemSize(numVoices) +
           detail::ChannelManager::GetInstance().GetRequiredMemSize(numVoices);
}

void SoundSystem::InitSoundSystem(const SoundSystemParam& rParam, void* pWork,
                                  u32 workSize) {
#pragma unused(workSize)

    if (sInitialized) {
        return;
    }

    sInitialized = true;
    OSRegisterVersion(NW4R_SND_Version_);

    detail::AxManager::GetInstance().Init();

    SCInit();
    while (SCCheckStatus() == SC_STATUS_BUSY) {
        ;
    }

    switch (SCGetSoundMode()) {
    case SC_SND_MONO:
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_MONO);
        break;

    case SC_SND_STEREO:
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_STEREO);
        break;

    case SC_SND_SURROUND:
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_DPL2);
        break;

    default:
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_STEREO);
        break;
    }

    detail::RemoteSpeakerManager::GetInstance().Setup();

    u8* pPtr = static_cast<u8*>(pWork);

    void* pDvdThreadStack = pPtr;
    pPtr += rParam.dvdThreadStackSize;

    void* pSoundThreadStack = pPtr;
    pPtr += rParam.soundThreadStackSize;

    sMaxVoices = __AXGetNumVoices();

    void* pAxVoiceWork = pPtr;
    pPtr += detail::AxVoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices);

    detail::AxVoiceManager::GetInstance().Setup(
        pAxVoiceWork,
        detail::AxVoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices));

    void* pVoiceWork = pPtr;
    pPtr += detail::VoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices);

    detail::VoiceManager::GetInstance().Setup(
        pVoiceWork, detail::VoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices));

    void* pChannelWork = pPtr;
    pPtr += detail::ChannelManager::GetInstance().GetRequiredMemSize(sMaxVoices);

    detail::ChannelManager::GetInstance().Setup(
        pChannelWork,
        detail::ChannelManager::GetInstance().GetRequiredMemSize(sMaxVoices));
    
    detail::SeqPlayer::InitSeqPlayer();

    sTaskThread.Create(rParam.dvdThreadPriority, pDvdThreadStack,
                       rParam.dvdThreadStackSize);

    detail::SoundThread::GetInstance().Create(rParam.soundThreadPriority,
                                              pSoundThreadStack,
                                              rParam.soundThreadStackSize);
}

void SoundSystem::ShutdownSoundSystem() {
    if (!sInitialized) {
        return;
    }

    detail::SoundThread::GetInstance().Shutdown();
    detail::TaskManager::GetInstance().CancelAllTask();
    sTaskThread.Destroy();

    detail::RemoteSpeakerManager::GetInstance().Shutdown();
    detail::ChannelManager::GetInstance().Shutdown();
    detail::VoiceManager::GetInstance().Shutdown();
    detail::AxVoiceManager::GetInstance().Shutdown();
    detail::AxManager::GetInstance().Shutdown();

    sInitialized = false;
}

bool SoundSystem::IsInitializedSoundSystem() {
    return sInitialized;
}

void SoundSystem::WaitForResetReady() {
    if (!sInitialized) {
        return;
    }

    u32 start = OSGetTick();

    while (!detail::AxManager::GetInstance().IsResetReady()) {
        if (OS_TICKS_TO_SEC(OSGetTick() - start) > 0) {
            break;
        }
    }
}

} // namespace snd
} // namespace nw4r
