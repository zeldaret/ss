#ifndef NW4R_SND_VOICE_MANAGER_H
#define NW4R_SND_VOICE_MANAGER_H
#include "types_nw4r.h"
#include "snd_Voice.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct VoiceManager
			{
				static VoiceManager& GetInstance();

				Voice * AllocVoice(int, int, int, Voice::VoiceCallback, void *);
				void UpdateAllVoices();
				void NotifyVoiceUpdate();
				UNKTYPE UpdateAllVoicesSync(u32);
			};
		}
	}
}

#endif