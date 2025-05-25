#ifndef NW4R_SND_SEQ_SOUND_H
#define NW4R_SND_SEQ_SOUND_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_debug.h"
#include "nw4r/snd/snd_SeqPlayer.h"
#include "nw4r/snd/snd_Task.h"

#include "nw4r/ut/ut_LinkList.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class NoteOnCallback; }}}
namespace nw4r { namespace snd { namespace detail { class SeqTrackAllocator; }}}
namespace nw4r { namespace snd { namespace detail { template <class> class SoundInstanceManager; }}}
namespace nw4r { namespace snd { class SeqSoundHandle; }}

namespace nw4r { namespace ut { namespace detail { class RuntimeTypeInfo; }}}
namespace nw4r { namespace ut { class FileStream; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2ed0a
	class SeqSound : public BasicSound
	{
	// typedefs
	public:
		/* redeclare with this class instead of BasicSound for
		 * SoundInstanceManager
		 */
		typedef ut::LinkList<SeqSound, 0xe0> PriorityLinkList;

	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2eb5e
		class SeqLoadTask : public Task
		{
		// typedefs
		public:
			typedef void Callback(bool result, void const *seqBase,
			                      void *userData);

		// methods
		public:
			// cdtors
			SeqLoadTask();

			// virtual function ordering
			// vtable Task
			virtual void Execute();
			virtual void Cancel();
			virtual void OnCancel();

		// members
		public:
			/* base Task */					// size 0x10, offset 0x00
			ut::FileStream	*mFileStream;	// size 0x04, offset 0x10
			void			*mBuffer;		// size 0x04, offset 0x14
			int				mBufferSize;	// size 0x04, offset 0x18
			Callback		*mCallback;		// size 0x04, offset 0x1c
			void			*mCallbackData;	// size 0x04, offset 0x20
		}; // size 0x24

	// methods
	public:
		// cdtors
		SeqSound(SoundInstanceManager<SeqSound> *manager, int priority,
		         int ambientPriority);

		// virtual function ordering
		// vtable BasicSound
		virtual ut::detail::RuntimeTypeInfo const *GetRuntimeTypeInfo() const
		{
			return &typeInfo;
		}
		virtual void Shutdown();
		virtual bool IsPrepared() const { return mPreparedFlag; }
		virtual bool IsAttachedTempSpecialHandle();
		virtual void DetachTempSpecialHandle();
		virtual void InitParam();
		virtual BasicPlayer &GetBasicPlayer() { return mSeqPlayer; }
		virtual BasicPlayer const &GetBasicPlayer() const { return mSeqPlayer; }
		virtual void OnUpdatePlayerPriority();

		// methods
		SeqPlayer::SetupResult Setup(SeqTrackAllocator *trackAllocator,
		                             u32 allocTracks,
		                             NoteOnCallback *callback);
		void Prepare(void const *seqBase, s32 seqOffset,
		             SeqPlayer::OffsetType startOffsetType, int startOffset);
		void Prepare(ut::FileStream *fileStream, s32 seqOffset,
		             SeqPlayer::OffsetType startOffsetType, int startOffset);

		void *GetFileStreamBuffer() { return mFileStreamBuffer; }
		s32 GetFileStreamBufferSize() { return sizeof mFileStreamBuffer; }

		void SetReleasePriorityFix(bool fix);
		void SetTempoRatio(f32 tempo);
		void SetChannelPriority(int priority);
		void SetSeqUserprocCallback(SeqPlayer::SeqUserprocCallback *callback,
		                            void *arg);

		void Skip(SeqPlayer::OffsetType offsetType, int offset);

		bool LoadData(SeqLoadTask::Callback *callback,
		              void *callbackArg);

		static void NotifyLoadAsyncEndSeqData(bool result, void const *seqBase,
		                                      void *userData);

		static DebugSoundType GetSoundType()
		{
			return DEBUG_SOUND_TYPE_SEQSOUND;
		}

	// static members
	public:
		static int const FILE_STREAM_BUFFER_SIZE = 128;

		static ut::detail::RuntimeTypeInfo const typeInfo;

	// members
	private:
		/* base BasicSound */														// size 0x100, offset 0x000
		SeqPlayer						mSeqPlayer;									// size 0x154, offset 0x100
		SeqSoundHandle					*mTempSpecialHandle;						// size 0x004, offset 0x254
		SoundInstanceManager<SeqSound>	*mManager;									// size 0x004, offset 0x258
		s32								mSeqOffset;									// size 0x004, offset 0x25c
		SeqPlayer::OffsetType			mStartOffsetType;							// size 0x004, offset 0x260
		int								mStartOffset;								// size 0x004, offset 0x264
		bool							volatile mLoadingFlag;						// size 0x001, offset 0x268
		bool							volatile mPreparedFlag;						// size 0x001, offset 0x269
		/* 2 bytes padding */
		ut::FileStream					*mFileStream;								// size 0x004, offset 0x26c
		int								mFileStreamBuffer[FILE_STREAM_BUFFER_SIZE];	// size 0x200, offset 0x2a0
		SeqLoadTask						mSeqLoadTask;								// size 0x024, offset 0x4a0

	// friends
	private:
		friend class snd::SeqSoundHandle;
	}; // size 0x4c4
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_SEQ_SOUND_H
