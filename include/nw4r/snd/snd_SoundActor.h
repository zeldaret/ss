#ifndef NW4R_SND_SOUND_ACTOR_H
#define NW4R_SND_SOUND_ACTOR_H

/*******************************************************************************
 * headers
 */

#include "common.h" // nullptr

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_ExternalSoundPlayer.h"
#include "nw4r/snd/snd_SoundStartable.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { class SoundArchivePlayer; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x278fa
	class SoundActor : public SoundStartable
	{
	// methods
	public:
		// cdtors
		SoundActor(SoundArchivePlayer &player);
		virtual ~SoundActor();

		virtual u32 detail_ConvertLabelStringToSoundId(const char* label) override;
		virtual StartResult detail_SetupSound(SoundHandle *pHandle, u32 soundId, bool holdFlag, const StartInfo *pStartInfo) override;
		
		
		virtual StartResult SetupSound(SoundHandle *pHandle, u32 soundId, const StartInfo *pStartInfo, void*);
		virtual StartResult detail_SetupSoundWithAmbientInfo(SoundHandle*, u32, const StartInfo*, detail::BasicSound::AmbientInfo*, void*);

		
		// methods
		detail::ExternalSoundPlayer *detail_GetActorPlayer(int actorPlayerId)
		{
			if (actorPlayerId < 0 || ACTOR_PLAYER_COUNT <= actorPlayerId)
				return nullptr;

			return &mActorPlayer[actorPlayerId];
		}


		template <typename TForEachFunc>
		TForEachFunc ForEachSound(TForEachFunc pFunc, bool reverse) {
			for (int i = 0; i < ACTOR_PLAYER_COUNT; i++) {
				mActorPlayer[i].ForEachSound(pFunc, reverse);
			}

			return pFunc;
		}

		detail::SoundActorParam const &detail_GetActorParam() const
		{
			return mActorParam;
		}

		void StopAllSound(int fadeFrames);
		void PauseAllSound(bool flag, int fadeFrames);
		int GetPlayingSoundCount(int playerId) const;

	// static members
	public:
		static int const ACTOR_PLAYER_COUNT = 4;

	// members
	private:
		/* base SoundStartable */										// size 0x04, offset 0x00
		SoundArchivePlayer			&mSoundArchivePlayer;				// size 0x04, offset 0x04
		detail::ExternalSoundPlayer	mActorPlayer[ACTOR_PLAYER_COUNT];	// size 0x40, offset 0x08
		detail::SoundActorParam		mActorParam;						// size 0x0c, offset 0x48
	}; // size 0x54
}} // namespace nw4r::snd

#endif // NW4R_SND_SOUND_ACTOR_H
