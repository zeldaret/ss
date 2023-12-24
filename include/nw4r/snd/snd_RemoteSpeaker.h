#ifndef NW4R_SND_REMOTE_SPEAKER_H
#define NW4R_SND_REMOTE_SPEAKER_H
#include <OS.h>
#include <OSAlarm.h>
#include "types_nw4r.h"

namespace nw4r
{
	namespace snd
	{
		struct RemoteSpeaker
		{
			typedef void (*RemoteSpeakerCallback)(s32, s32);
			
			enum SpeakerCommand
			{
				SpeakerCommand_None,
				SpeakerCommand_On,
				SpeakerCommand_Play,
				SpeakerCommand_Off,
			};
			
			bool mPoweredFlag; // at 0x0
			bool BOOL_0x1;
			bool mEnabledOutputFlag; // at 0x2
			bool BOOL_0x3;
			bool BOOL_0x4;
			bool BOOL_0x5;
			bool BOOL_0x6;
			bool BOOL_0x7;
			volatile bool BOOL_0x8;
			
			UNKWORD WORD_0xC;
			SpeakerCommand CMD_0x10;
			SpeakerCommand CMD_0x14;
			char UNK_0x18[0x20];
			s32 mChannelIndex; // at 0x38
			RemoteSpeakerCallback mCallback; // at 0x3c
			OSAlarm ALARM_0x40;
			OSAlarm ALARM_0x70;
			s64 mTime; // at 0xa0
			
			RemoteSpeaker();
			void ClearParam(); //inlined
			void InitParam(); //inlined
			
			bool Setup(RemoteSpeakerCallback);
			void Shutdown(RemoteSpeakerCallback);
			
			bool EnableOutput(bool);
			bool IsEnabledOutput() const;
			
			void Update();
			void ExecCommand(SpeakerCommand);
			
			bool IsAllSampleZero(const s16 *); //inlined
			void UpdateStreamData(const s16 *);
			
			void NotifyCallback(s32, s32); //inlined
			static void SpeakerOnCallback(s32, s32);
			static void SpeakerPlayCallback(s32, s32);
			static void SpeakerOffCallback(s32, s32);
			
			static void ContinueAlarmHandler(OSAlarm *, OSContext *);
			static void IntervalAlarmHandler(OSAlarm *, OSContext *);
			
			inline void SetChannelIndex(int channelIndex)
			{
				mChannelIndex = channelIndex;
			}
		};
	}
}

#endif