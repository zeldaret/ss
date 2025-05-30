#ifndef NW4R_SND_WAVE_SOUND_H
#define NW4R_SND_WAVE_SOUND_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_debug.h"
#include "nw4r/snd/snd_WsdPlayer.h"

#include "nw4r/ut/ut_LinkList.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { template <class> class SoundInstanceManager; }}}
namespace nw4r { namespace snd { class WaveSoundHandle; }}

namespace nw4r { namespace ut { namespace detail { class RuntimeTypeInfo; }}}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	class WaveSound : public BasicSound
	{
	// typedefs
	public:
		/* redeclare with this class instead of BasicSound for
		 * SoundInstanceManager
		 */
		typedef ut::LinkList<WaveSound, 0xf0> PriorityLinkList;

	// methods
	public:
		// cdtors
		WaveSound(SoundInstanceManager<WaveSound> *manager, int priority,
		          int ambientPriority);
		/* virtual ~WaveSound() {} */ // virtual function ordering

		// virtual function ordering
		// vtable BasicSound
		virtual ut::detail::RuntimeTypeInfo const *GetRuntimeTypeInfo() const
		{
			return &typeInfo;
		}
		virtual ~WaveSound() {}
		virtual void Shutdown();
		virtual bool IsPrepared() const { return mPreparedFlag; }
		virtual bool IsAttachedTempSpecialHandle();
		virtual void DetachTempSpecialHandle();
		virtual BasicPlayer &GetBasicPlayer() { return mWsdPlayer; }
		virtual BasicPlayer const &GetBasicPlayer() const { return mWsdPlayer; }
		virtual void OnUpdatePlayerPriority();

		// methods
		bool Prepare(void const *waveSoundBase, s32 waveSoundOffset,
		             WsdPlayer::StartOffsetType startOffsetType, s32 offset,
		             WsdPlayer::WsdCallback const *callback,
		             u32 callbackData);

		void SetChannelPriority(int priority);
		void SetReleasePriorityFix(bool flag);

		static DebugSoundType GetSoundType()
		{
			return DEBUG_SOUND_TYPE_WAVESOUND;
		}

	// static members
	public:
		static ut::detail::RuntimeTypeInfo const typeInfo;

		// members
	private:
		/* base BasicSound */									// size 0x100, offset 0x000
		WsdPlayer						mWsdPlayer;				// size 0x138, offset 0x100
		WaveSoundHandle					*mTempSpecialHandle;	// size 0x004, offset 0x238
		SoundInstanceManager<WaveSound>	*mManager;				// size 0x004, offset 0x23c
		bool							mPreparedFlag;			// size 0x001, offset 0x240
		/* 3 bytes padding */

	// friends
	private:
		friend class snd::WaveSoundHandle;
	}; // size 0x244
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_WAVE_SOUND_H
