#include "nw4r/snd/snd_SoundSystem.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SoundSystem.cpp
 */

/*******************************************************************************
 * headers
 */

#include <decomp.h>
#include "common.h"

#include "nw4r/snd/snd_AxVoiceManager.h"
#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_Channel.h" // ChannelManager
#include "nw4r/snd/snd_RemoteSpeakerManager.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_SeqPlayer.h"
#include "nw4r/snd/snd_SoundThread.h"
#include "nw4r/snd/snd_TaskManager.h"
#include "nw4r/snd/snd_TaskThread.h"
#include "nw4r/snd/snd_VoiceManager.h"

#include <rvl/OS/OS.h> // OSRegisterVersion
#include <rvl/AX/AXVPB.h> // AXGetMaxVoices
#include <rvl/SC/scsystem.h>
#include <rvl/SC/scapi.h>

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * macros
 */

#define BUILDSTAMP_LIB_GROUP_NAME	"NW4R   "
#define BUILDSTAMP_LIB_NAME			"SND"

// You probably want to change these
#define BUILDSTAMP_BUILD_TYPE		"final  "
#define BUILDSTAMP_DATE				"Sep 25 2011"
#define BUILDSTAMP_TIME				"05:32:58"
#define BUILDSTAMP_CW_MAJOR_REV		STR(__CWCC__)
#define BUILDSTAMP_CW_MINOR_REV		STR(__CWBUILD__)

#define BUILDSTAMP_STRING	\
	"<< " \
	BUILDSTAMP_LIB_GROUP_NAME " - " BUILDSTAMP_LIB_NAME " \t" \
	BUILDSTAMP_BUILD_TYPE " build: " \
	BUILDSTAMP_DATE " " BUILDSTAMP_TIME \
	" (" BUILDSTAMP_CW_MAJOR_REV "_" BUILDSTAMP_CW_MINOR_REV ")" \
	" >>"

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd
{
	// .data, .sdata
	extern "C" char const *NW4R_SND_Version_ = BUILDSTAMP_STRING;

	// .bss
	detail::TaskThread SoundSystem::sTaskThread;

	// .sbss
	namespace
	{
		bool sInitialized;
	} // unnamed namespace

	int SoundSystem::sMaxVoices;
}} // namespace nw4r::snd

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

void SoundSystem::InitSoundSystem(s32 soundThreadPriority,
                                  s32 dvdThreadPriority) {
    const int defaultWorkSize = DEFAULT_SOUND_THREAD_STACK_SIZE +
                                DEFAULT_DVD_THREAD_STACK_SIZE +
                                detail::AxVoiceManager::WORK_SIZE_MAX +
                                detail::VoiceManager::WORK_SIZE_MAX +
                                detail::ChannelManager::WORK_SIZE_MAX;

    static u8 defaultSoundSystemWork[defaultWorkSize] ALIGN_DECL(32);

    // OSRegisterVersion(NW4R_SND_Version_);

    SoundSystemParam param;
    param.soundThreadPriority = soundThreadPriority;
    param.dvdThreadPriority = dvdThreadPriority;

    // @bug This function ignores the specified buffer size
    InitSoundSystem(param, defaultSoundSystemWork,
                    sizeof(defaultSoundSystemWork));
}

u32 SoundSystem::GetRequiredMemSize(SoundSystemParam const &param)
{
	// could have just used align assert? idk
	NW4RAssert_Line(106, param.soundThreadStackSize % 8 == 0);
	NW4RAssert_Line(107, param.dvdThreadStackSize % 8 == 0);

	int maxVoices = AXGetMaxVoices();

	return param.soundThreadStackSize + param.dvdThreadStackSize
	     + detail::AxVoiceManager::GetInstance().GetRequiredMemSize(maxVoices)
	     + detail::VoiceManager::GetInstance().GetRequiredMemSize(maxVoices)
	     + detail::ChannelManager::GetInstance().GetRequiredMemSize(maxVoices);
}

void SoundSystem::InitSoundSystem(SoundSystemParam const &param, void *workMem,
                                  u32 workMemSize)
{
	bool result; // presumably up here

	NW4RAssertAligned_Line(144, workMem, 32);
	NW4RAssert_Line(145, workMemSize >= GetRequiredMemSize( param ));

	if (sInitialized)
		return;

	sInitialized = true;

	OSRegisterVersion(NW4R_SND_Version_);

	detail::AxManager::GetInstance().Init();

	SCInit();

	SCStatus initStatus;
	do
		initStatus = SCCheckStatus();
	while (initStatus == SC_STATUS_BUSY);

	NW4RAssert_Line(171, initStatus == SC_STATUS_OK);

	SCSoundMode soundMode = SCGetSoundMode();
	switch (soundMode)
	{
	case SC_SND_MONO:
		detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_MONO);
		break;

	case SC_SND_STEREO:
		detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_STEREO);
		break;

	case SC_SND_SURROUND:
		detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_DPL2);
		break;

	default:
		detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_STEREO);
		break;
	}

	detail::RemoteSpeakerManager::GetInstance().Setup();

	byte_t *ptr = static_cast<byte_t *>(workMem);

	void *dvdThreadStack = ptr;
	ptr += param.dvdThreadStackSize;

	void *soundThreadStack = ptr;
	ptr += param.soundThreadStackSize;

	sMaxVoices = AXGetMaxVoices();

	void *axVoiceWork = ptr;
	ptr += detail::AxVoiceManager::GetInstance().GetRequiredMemSize(
		sMaxVoices);

	detail::AxVoiceManager::GetInstance().Setup(
		axVoiceWork,
		detail::AxVoiceManager::GetInstance().GetRequiredMemSize(
			sMaxVoices));

	void *voiceWork = ptr;
	ptr += detail::VoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices);

	detail::VoiceManager::GetInstance().Setup(
		voiceWork,
		detail::VoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices));

	void *channelWork = ptr;
	ptr += detail::ChannelManager::GetInstance().GetRequiredMemSize(
		sMaxVoices);

	detail::ChannelManager::GetInstance().Setup(
		channelWork,
		detail::ChannelManager::GetInstance().GetRequiredMemSize(
			sMaxVoices));

	detail::SeqPlayer::InitSeqPlayer();

	result = sTaskThread.Create(param.dvdThreadPriority, dvdThreadStack,
	                   param.dvdThreadStackSize);
	NW4RAssert_Line(247, result);

	result = detail::SoundThread::GetInstance().Create(
		param.soundThreadPriority, soundThreadStack,
		param.soundThreadStackSize);
	NW4RAssert_Line(255, result);

	NW4RAssert_Line(257, ptr <= reinterpret_cast<u8*>( workMem ) + workMemSize);
}

void SoundSystem::ShutdownSoundSystem()
{
	if (!sInitialized)
		return;

	detail::SoundThread::GetInstance().Shutdown();

	detail::TaskManager::GetInstance().CancelAllTask();
	sTaskThread.Destroy();

	detail::ChannelManager::GetInstance().Shutdown();
	detail::VoiceManager::GetInstance().Shutdown();
	detail::AxVoiceManager::GetInstance().Shutdown();
	detail::AxManager::GetInstance().Shutdown();

	sInitialized = false;
}

bool SoundSystem::IsInitializedSoundSystem()
{
	return sInitialized;
}


void SoundSystem::WaitForResetReady() {
    if (!sInitialized) {
        return;
    }

    u32 start = OSGetTick();

    while (!detail::AxManager::GetInstance().IsResetReady()) {
        if (OS_TICKS_TO_SEC(OSGetTick() - start) > 0) {
            break;
        }
    }
}
#if 0
// SoundSystem::WaitForResetReady ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14493)
DECOMP_FORCE("SoundSystem::WaitForResetReady is TIME OUT.\n");
#endif

}} // namespace nw4r::snd
