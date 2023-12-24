#ifndef NW4R_SND_VOICE_H
#define NW4R_SND_VOICE_H
#include "types_nw4r.h"
#include "snd_AxManager.h"
#include "snd_WaveFile.h"
#include "snd_Common.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct Voice
			{
				enum VoiceCallbackStatus
				{
					STATUS_0,
					STATUS_1,
					STATUS_2,
					STATUS_3
				};

				enum VoiceSyncFlag {
					SYNC_AX_SRC_INITIAL = (1 << 0),
					SYNC_AX_VOICE = (1 << 1),
					SYNC_AX_SRC = (1 << 2),
					SYNC_AX_VE = (1 << 3),
					SYNC_AX_MIX = (1 << 4),
					SYNC_AX_LPF = (1 << 5),
					SYNC_AX_REMOTE = (1 << 7),
					SYNC_AX_BIQUAD = (1 << 8),
				};
				
				typedef void (* VoiceCallback)(Voice *, VoiceCallbackStatus, void *);
				
				UNKTYPE Start();
				UNKTYPE Setup(const WaveData &, u32);
				UNKTYPE SetPriority(int);
				UNKTYPE Stop();
				UNKTYPE Free();
				UNKTYPE SetPanMode(PanMode);
				UNKTYPE SetPanCurve(PanCurve);
				UNKTYPE SetVolume(float);
				UNKTYPE SetVeVolume(float, float);
				UNKTYPE SetPitch(float);
				UNKTYPE SetPan(float);
				UNKTYPE SetSurroundPan(float);
				UNKTYPE SetLpfFreq(float);
				UNKTYPE SetRemoteFilter(int);
				UNKTYPE SetOutputLine(int);
				UNKTYPE SetMainOutVolume(float);
				UNKTYPE SetMainSend(float);
				
				UNKTYPE SetFxSend(AuxBus, float);
				
				UNKTYPE SetRemoteOutVolume(int, float);
				UNKTYPE SetRemoteSend(int, float);
				UNKTYPE SetRemoteFxSend(int, float);
				
				UNKTYPE Pause(bool);
			};
		}
	}
}

#endif