#ifndef NW4R_SND_CHANNEL_H
#define NW4R_SND_CHANNEL_H
#include "types_nw4r.h"
#include "ut_LinkList.h"
#include "snd_Voice.h"
#include "snd_Lfo.h"
#include "snd_InstancePool.h"
#include "snd_WaveFile.h"
#include "snd_EnvGenerator.h"
#include "snd_MoveValue.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct Channel
			{
				enum ChannelCallbackStatus
				{
					CALLBACK_STATUS_0,
					CALLBACK_STATUS_1,
					CALLBACK_STATUS_2,
					CALLBACK_STATUS_3
				};
				
				typedef void (* ChannelCallback)(Channel *, ChannelCallbackStatus, u32);
				
				static void VoiceCallbackFunc(Voice *, Voice::VoiceCallbackStatus, void *);
				
				static Channel * AllocChannel(int, int, int, ChannelCallback, u32);
				static void FreeChannel(Channel *);
				Channel();
				
				UNKTYPE InitParam(ChannelCallback, u32);
				void Update(bool);
				UNKTYPE Start(const WaveData &, int, u32);
				UNKTYPE Release();
				UNKTYPE Stop();
				
				float GetSweepValue() const;
				void UpdateSweep(int);
				void SetSweepParam(float, int, bool);
				
				EnvGenerator mEnvGenerator; // at 0x0
				Lfo mLfo; // at 0x18
				u8 BYTE_0x30;
				bool mPauseFlag; // at 0x31
				bool mActiveFlag; // at 0x32
				bool BOOL_0x33;
				bool mAutoUpdateSweepFlag; // at 0x34
				bool mReleasePriorityFixFlag; // at 0x35
				
				float FLOAT_0x38;
				float FLOAT_0x3C;
				float FLOAT_0x40;
				float FLOAT_0x44;
				float FLOAT_0x48;
				
				int mRemoteFilter; // at 0x4c
				int mOutputLine; // at 0x50
				
				float FLOAT_0x54;
				float FLOAT_0x58;
				
				float ARR_0x5C[3];
				
				float mRemoteOutVolumes[4]; // at 0x68
				float ARR_0x78[4];
				float ARR_0x88[4];
				
				float FLOAT_0x98;
				float FLOAT_0x9C;
				
				int INT_0xA0;
				int INT_0xA4;
				
				float FLOAT_0xA8;
				float FLOAT_0xAC;
				float FLOAT_0xB0;
				float FLOAT_0xB4;
				MoveValue<u8, u16> MV_0xB8;
				UNKWORD INT_0xC0;
				UNKWORD INT_0xC4;
				int mLength; // at 0xc8
				
				PanMode mPanMode; // at 0xcc
				PanCurve mPanCurve; // at 0xd0
				
				ChannelCallback mCallback; // at 0xd4
				u32 INT_0xD8;
				Voice * mVoice; // at 0xdc
				Channel * mNext; // at 0xe0
				ut::LinkListNode mNode; // at 0xe4
				
				inline bool IsActive() const
				{
					return mActiveFlag;
				}
				
				inline void SetRelease(int release)
				{
					if (release >= 0) mEnvGenerator.SetRelease(release);
					
					Release();
				}
				
				inline bool IsAutoUpdateSweep() const
				{
					return mAutoUpdateSweepFlag;
				}
				
				inline bool IsPause() const
				{
					return mPauseFlag;
				}
				
				inline void Pause(bool flag)
				{
					mPauseFlag = flag;
					
					mVoice->Pause(flag);
				}
				
				inline void SetLfoParam(const LfoParam & lfoParam)
				{
					mLfo.SetParam(lfoParam);
				}
				
				inline void SetSilence(bool silentFlag, int time)
				{
					MV_0xB8.SetTarget(silentFlag ? 0 : 0xFF, time);
				}
			};
			
			struct ChannelManager
			{
				ChannelManager();
				
				u32 GetRequiredMemSize();
				void Setup(void *, u32);
				void Shutdown();
				UNKTYPE UpdateAllChannel();
				static void Free(Channel *); //inlined
				Channel * Alloc(); // inlined
				static ChannelManager * GetInstance();
				
				InstancePool<Channel> mPool; // at 0x0
				ut::LinkList<Channel, 0xE4> mList; // at 0x4
				
				bool BOOL_0x10;
				u32 INT_0x14;
				UNKTYPE * PTR_0x18;
				u32 INT_0x1C;
			};
		}
	}
}

#endif