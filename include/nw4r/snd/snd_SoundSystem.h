#ifndef NW4R_SND_SOUND_SYSTEM_H
#define NW4R_SND_SOUND_SYSTEM_H
#include "types_nw4r.h"
#include "snd_AxManager.h"

namespace nw4r
{
	namespace snd
	{
        struct SoundSystem
        {
            struct SoundSystemParam
            {

            };

            static inline f32 GetMasterVolume()
            {
                return detail::AxManager::GetInstance().GetMasterVolume();
            }

            static inline void PrepareReset()
            {
                detail::AxManager::GetInstance().PrepareReset();
            }

            void InitSoundSystem(s32, s32);
            void InitSoundSystem(const SoundSystemParam&, void *, u32);
            static void ShutdownSoundSystem();
            static void WaitForResetReady();
            UNKWORD GetRequiredMemSize(const SoundSystemParam &);
        };
	}
}

#endif