#ifndef NW4R_SND_SOUND_SYSTEM_H
#define NW4R_SND_SOUND_SYSTEM_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_AxManager.h>
#include <nw4r/snd/snd_RemoteSpeakerManager.h>
#include <nw4r/snd/snd_TaskThread.h>

namespace nw4r {
namespace snd {

class SoundSystem {
public:
    struct SoundSystemParam {
        s32 soundThreadPriority;  // at 0x0
        u32 soundThreadStackSize; // at 0x4
        s32 dvdThreadPriority;    // at 0x8
        u32 dvdThreadStackSize;   // at 0xC

        SoundSystemParam()
            : soundThreadPriority(DEFAULT_SOUND_THREAD_PRIORITY),
              soundThreadStackSize(DEFAULT_SOUND_THREAD_STACK_SIZE),
              dvdThreadPriority(DEFAULT_DVD_THREAD_PRIORITY),
              dvdThreadStackSize(DEFAULT_DVD_THREAD_STACK_SIZE) {}
    };

public:
    static void InitSoundSystem(s32 soundThreadPrio, s32 dvdThreadPriority);
    static u32 GetRequiredMemSize(const SoundSystemParam& rParam);

    static void InitSoundSystem(const SoundSystemParam& rParam, void* pWork,
                                u32 workSize);

    static void ShutdownSoundSystem();
    static void WaitForResetReady();

    static void PrepareReset() {
        detail::AxManager::GetInstance().PrepareReset();
    }

    static void SetOutputMode(OutputMode mode) {
        detail::AxManager::GetInstance().SetOutputMode(mode);
    }

    static f32 GetMasterVolume() {
        return detail::AxManager::GetInstance().GetMasterVolume();
    }
    static void SetMasterVolume(f32 volume, int frame) {
        detail::AxManager::GetInstance().SetMasterVolume(volume, frame);
    }

    static RemoteSpeaker& GetRemoteSpeaker(int i) {
        return detail::RemoteSpeakerManager::GetInstance().GetRemoteSpeaker(i);
    }

    static void AppendEffect(AuxBus bus, FxBase* pFx) {
        detail::AxManager::GetInstance().AppendEffect(bus, pFx);
    }
    static void ClearEffect(AuxBus bus, int frame) {
        detail::AxManager::GetInstance().ClearEffect(bus, frame);
    }

private:
    static const int DEFAULT_DVD_THREAD_PRIORITY = 3;
    static const int DEFAULT_DVD_THREAD_STACK_SIZE = 0x4000;

    static const int DEFAULT_SOUND_THREAD_PRIORITY = 4;
    static const int DEFAULT_SOUND_THREAD_STACK_SIZE = 0x4000;

private:
    static detail::TaskThread sTaskThread;
};

} // namespace snd
} // namespace nw4r

#endif
