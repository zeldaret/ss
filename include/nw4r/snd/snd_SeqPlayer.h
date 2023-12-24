#ifndef NW4R_SND_SEQ_PLAYER_H
#define NW4R_SND_SEQ_PLAYER_H
#include "types_nw4r.h"
#include "ut_lock.h"
#include "snd_BasicPlayer.h"
#include "snd_InstancePool.h"
#include "snd_DisposeCallback.h"
#include "snd_SoundThread.h"
#include "snd_NoteOn.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			// Used an anonymous enum instead of a macro
			// because I wanted the magic number to have
			// namespace scope
			enum
			{
				SEQ_TRACKS_PER_PLAYER = 0x10,
				SEQ_VARIABLE_COUNT = 0x10
			};
			
			//sizeof(SeqPlayer) = 0x11C
			struct SeqPlayer : BasicPlayer, DisposeCallback, SoundThread::PlayerCallback
			{
				enum OffsetType
				{
					OFFSET_TYPE_0,
					OFFSET_TYPE_1
				};
				
				
				bool Start(); // at 0xc
				void Stop(); // at 0x10
				void Pause(bool); // at 0x14
				bool IsActive() const; // at 0x18
				bool IsStarted() const; // at 0x1c
				bool IsPause() const; // at 0x20
				
				void InvalidateData(const void *, const void *);
				inline void InvalidateWaveData(const void *, const void *) {}				
				inline virtual void ChannelCallback(Channel *) {}
				
				bool mActiveFlag; // at 0x8c
				bool mStartedFlag; // at 0x8d
				bool mPauseFlag; // at 0x8e
				
				bool mReleasePriorityFixFlag; // at 0x8f
				float FLOAT_0x90;
				float mTempoRatio; // at 0x94
				float FLOAT_0x98;
				int OFFSET_0x9C;
				float OFFSET_0xA0;
				int INT_0xA4;
				char BYTE_0xA8;
				u8 mChannelPriority; // at 0xa9
				u8 BYTE_0xAA;
				u16 SHORT_0xAC;
				
				NoteOnCallback * mNoteOnCallback; // at 0xb0
				SeqTrackAllocator * mTrackAllocator; // at 0xb4
				SeqTrack * mPlayerTracks[SEQ_TRACKS_PER_PLAYER]; // at 0xb8
				volatile s16 mLocalVariables[SEQ_VARIABLE_COUNT]; // at 0xf8
				
				int INT_0x118;
				
				SeqPlayer();
				~SeqPlayer();
				
				UNKTYPE InitParam(int, NoteOnCallback *);
				void CloseTrack(int);
				void FinishPlayer(); //inlined
				
				bool Setup(SeqTrackAllocator *, u32, int, NoteOnCallback *);
				void SetSeqData(const void *, s32);
				void Skip(OffsetType, int);
				
				void SetTempoRatio(float);
				void SetChannelPriority(int);
				void SetReleasePriorityFix(bool);
				void SetLocalVariable(int, short);
				static void InitGlobalVariable(); //inline
				static void SetGlobalVariable(int, short);
				
				template <typename T>
				inline void SetTrackParam(u32 flags, void (SeqTrack::* pSetter)(T), T param)
				{
					ut::AutoInterruptLock lock;
					
					for (int i = 0; i < SEQ_TRACKS_PER_PLAYER && flags; flags >>= 1, i++)
					{
						if (flags & 1)
						{
							SeqTrack * pPlayerTrack = GetPlayerTrack(i);
							
							if (pPlayerTrack) (pPlayerTrack->*pSetter)(param);
						}
					}
				}
				
				void SetTrackVolume(u32, float);
				void SetTrackPitch(u32, float);
				
				void SetPlayerTrack(int, SeqTrack *); //inlined
				SeqTrack * GetPlayerTrack(int);
				bool ParseNextTick(bool);
				volatile s16 * GetVariablePtr(int);
				void UpdateChannelParam(); //inlined
				void Update();
				void UpdateTick();
				void UpdateTick(int);
				bool NoteOn(int, const NoteOnInfo &);
				inline void OnUpdateFrameSoundThread()
				{
					Update();
				}
				
				inline void OnShutdownSoundThread()
				{
					Stop();
				}
				
				inline float GetBaseTempo() const
				{
					return mTempoRatio * (BYTE_0xAA * SHORT_0xAC) / 60000.0f;
				}
				
				static volatile s16 mGlobalVariable[SEQ_VARIABLE_COUNT];
				static bool mGobalVariableInitialized; // typo
			};
		}
	}
}

#endif