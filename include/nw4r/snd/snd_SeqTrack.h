#ifndef NW4R_SND_SEQ_TRACK_H
#define NW4R_SND_SEQ_TRACK_H
#include "types_nw4r.h"
#include "snd_Lfo.h"
#include "snd_Channel.h"

namespace nw4r
{
	namespace snd
	{
		enum SeqMute
		{
			SEQ_MUTE_0,
			SEQ_MUTE_1,
			SEQ_MUTE_2,
			SEQ_MUTE_3
		};
		
		namespace detail
		{
			struct SeqTrack
			{
				virtual ~SeqTrack();
				virtual UNKWORD Parse(bool) = 0;
				
				u8 mPlayerTrackNo; // at 0x4
				bool mOpenFlag; // at 0x5
				
				float mVolume; // at 0x8
				float mPitch; // at 0xc
				float FLOAT_0x10;
				float FLOAT_0x14;
				float FLOAT_0x18;
				float FLOAT_0x1C;
				float FLOAT_0x20;
				float FLOATS_0x24[3];
				float FLOATS_0x30[4];
				float FLOATS_0x40[4];
				
				const u8 * PTR_0x50;
				const u8 * PTR_0x54;
				
				int TIMER_0x58;
				
				bool BOOL_0x5C;
				char BYTE_0x5D;
				bool BOOL_0x5E;
				bool BOOL_0x5F;
				bool BOOL_0x60;
				
				int INT_0x64;
				UNKWORD WORD_0x68;
				
				LfoParam mLfoParam; // at 0x6c
				
				u8 BYTE_0x7C;
				float FLOAT_0x80;
				u8 BYTE_0x84;
				u8 BYTE_0x85;
				s8 BYTE_0x86;
				u8 BYTE_0x87;
				s8 BYTE_0x88;
				s8 BYTE_0x89;
				s8 BYTE_0x8A;
				char BYTE_0x8B;
				u8 BYTE_0x8C;
				char BYTE_0x8D;
				u8 BYTE_0x8E;
				
				u8 mAttack; // at 0x8f
				u8 mDecay; // at 0x90
				u8 mSustain; // at 0x91
				u8 mRelease; // at 0x92
				
				u8 BYTE_0x93;
				u8 BYTES_0x94[3];
				char BYTE_0x97;
				
				s16 mVariables[0x10]; // at 0x98
				
				SeqPlayer * mPlayer; // at 0xb8
				Channel * mChannel; // at 0xbc
				
				void SetPlayerTrackNo(int);
				
				SeqTrack();
				
				UNKTYPE InitParam();
				
				void SetSeqData(const void *, s32);
				
				void Open();
				void Close();
				
				void UpdateChannelRelease(Channel *); //inlined
				void UpdateChannelLength();
				UNKWORD ParseNextTick(bool);
				void ReleaseAllChannel(int);
				UNKTYPE PauseAllChannel(bool);
				void UpdateChannelParam();
				UNKTYPE FreeAllChannel();
				
				static void ChannelCallbackFunc(Channel *, Channel::ChannelCallbackStatus, u32);
				
				void StopAllChannel(); //inlined
				void SetMute(SeqMute);
				void SetVolume(float);
				void SetPitch(float);
				s16 * GetVariablePtr(int);
				void AddChannel(Channel *); //inlined
				Channel * NoteOn(int, int, s32, bool);
				
				inline bool IsOpened() const
				{
					return mOpenFlag;
				}
			};
			
			struct SeqTrackAllocator
			{
				inline virtual ~SeqTrackAllocator() {}
				virtual SeqTrack * AllocTrack(SeqPlayer *) = 0;
				virtual void FreeTrack(SeqTrack *) = 0;
				virtual int GetAllocatableTrackCount() const = 0;
			};
		}
	}
}
#endif