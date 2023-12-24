#ifndef NW4R_SND_SEQ_SOUND_H
#define NW4R_SND_SEQ_SOUND_H
#include <OSMutex.h>
#include "types_nw4r.h"
#include "snd_BasicSound.h"
#include "snd_SeqPlayer.h"
#include "snd_SoundInstanceManager.h"
#include "snd_Task.h"
#include "snd_NoteOn.h"
#include "ut_FileStream.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct SeqSound : BasicSound
			{
				typedef void (*SeqLoadCallback)(bool, const void *, void *);
				
				//sizeof(SeqLoadTask) = 0x24
				struct SeqLoadTask : Task
				{
					ut::FileStream * mFileStream; // at 0x10
					void * mData; // at 0x14
					int mSize; // at 0x18
					SeqLoadCallback mCallback; // at 0x1c
					SeqSound * mSound; // at 0x20
					
					inline SeqLoadTask() : mFileStream(NULL), mData(NULL), mCallback(NULL), mSound(NULL) {}
					
					void Execute();
					void Cancel();
					void OnCancel();
				};
				
				static ut::detail::RuntimeTypeInfo typeInfo;
				
				SeqPlayer mPlayer; // at 0xd8
				SeqSoundHandle * mTempSpecialHandle; // at 0x1f4
				SoundInstanceManager<SeqSound> * mManager; // at 0x1f8
				s32 INT_0x1FC;
				SeqPlayer::OffsetType mOffsetType; // at 0x200
				int mOffset; // at 0x204
				bool mLoadingFlag; // at 0x208
				volatile bool mPreparedFlag; // at 0x209
				ut::FileStream * mFileStream; // at 0x20c
				char UNK_0x210[0x200];
				SeqLoadTask mLoadTask; // at 0x410
				OSMutex MUTEX_0x434; // at 0x434
				
				SeqSound(SoundInstanceManager<SeqSound> *);
				UNKTYPE InitParam();
				UNKTYPE Setup(SeqTrackAllocator *, u32, int, NoteOnCallback *);
				
				UNKTYPE Skip(SeqPlayer::OffsetType, int); //inlined
				bool LoadData(SeqLoadCallback, void *); //inlined
				void Prepare(const void *, s32, SeqPlayer::OffsetType, int);
				void Prepare(ut::FileStream *, s32, SeqPlayer::OffsetType, int);
				static void NotifyLoadAsyncEndSeqData(bool, const void *, void *);
				
				void Shutdown();
				
				UNKTYPE SetTempoRatio(float);
				UNKTYPE SetChannelPriority(int);
				UNKTYPE SetReleasePriorityFix(bool);
				
				UNKTYPE SetPlayerPriority(int);
				
				UNKTYPE SetTrackVolume(u32, float);
				UNKTYPE SetTrackPitch(u32, float);
				
				bool WriteVariable(int, short);
				static bool WriteGlobalVariable(int, short);
				
				bool IsAttachedTempSpecialHandle();
				UNKTYPE DetachTempSpecialHandle();
				
				inline const BasicPlayer * GetBasicPlayer() const
				{
					return &mPlayer;
				}
				
				inline BasicPlayer * GetBasicPlayer()
				{
					return &mPlayer;
				}
				
				inline bool IsPrepared() const
				{
					return mPreparedFlag;
				}
				
				inline const ut::detail::RuntimeTypeInfo * GetRuntimeTypeInfo() const
				{
					return &typeInfo;
				}
			};
		}
	}
}

#endif