#ifndef NW4R_SND_REMOTE_SPEAKER_MANAGER_H
#define NW4R_SND_REMOTE_SPEAKER_MANAGER_H
#include <OSAlarm.h>
#include "snd_RemoteSpeaker.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			struct RemoteSpeakerManager
			{
				bool mPoweredFlag; // at 0x0
				char UNK_0x4[4];
				OSAlarm mAlarm; // at 0x8
				RemoteSpeaker mRemoteSpeakers[4];
				
				RemoteSpeakerManager(); //inlined
				
				static RemoteSpeakerManager * GetInstance();
				
				RemoteSpeaker * GetRemoteSpeaker(int);
				
				void Setup();
				void Shutdown();
				
				static void RemoteSpeakerAlarmProc(OSAlarm *, OSContext *);
			};
		}
	}
}

#endif