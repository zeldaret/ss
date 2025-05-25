#include "nw4r/snd/snd_SoundThread.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SoundThread.cpp
 */

/*******************************************************************************
 * headers
 */

#include <decomp.h>
#include "common.h"

#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_AxVoiceManager.h"
#include "nw4r/snd/snd_Channel.h" // ChannelManager
#include "nw4r/snd/snd_Util.h" // Util::CalcRandom
#include "nw4r/snd/snd_VoiceManager.h"

#include "nw4r/ut/ut_Lock.h" // ut::detail::AutoLock

#include <rvl/OS/OSMessage.h>
#include <rvl/OS/OSMutex.h>
#include <rvl/OS/OSThread.h>
#include <rvl/OS/OSTime.h>

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

SoundThread::SoundThread() :
	mStackEnd	(nullptr),
	mCreateFlag	(false)
{
	OSInitMessageQueue(&mMsgQueue, mMsgBuffer, THREAD_MESSAGE_BUFSIZE);
	OSInitThreadQueue(&mThreadQueue);
	OSInitMutex(&mMutex);
}

SoundThread &SoundThread::GetInstance()
{
	static SoundThread instance;

	return instance;
}

bool SoundThread::Create(s32 priority, void *stack, u32 stackSize)
{
	NW4RAssertMessage_Line(78, AxManager::GetInstance().CheckInit(),
	                       "not initialized nw4r::AxManager.\n");
	NW4RAssertPointerNonnull_Line(79, stack);
	NW4RAssertAligned_Line(80, stack, 4);

	if (mCreateFlag)
		return true;

	mCreateFlag = true;
	mStackEnd = static_cast<u32 *>(stack);

	BOOL result = OSCreateThread(&mThread, &SoundThreadFunc, &GetInstance(),
	                             static_cast<byte_t *>(stack) + stackSize,
	                             stackSize, priority, OS_THREAD_NO_FLAGS);

	if (result)
		OSResumeThread(&mThread);

	return result;
}

void SoundThread::Shutdown()
{
	if (!mCreateFlag)
		return;

	BOOL result = OSJamMessageAny(&GetInstance().mMsgQueue, MESSAGE_SHUTDOWN,
	                              OS_MESSAGE_FLAG_PERSISTENT);
	NW4RAssert_Line(124, result);

	result = OSJoinThread(&mThread, nullptr);
	NW4RAssert_Line(128, result);

	mCreateFlag = false;
}

void SoundThread::AxCallbackFunc()
{
	SoundThread *soundThread = &GetInstance();

	soundThread->AxCallbackProc();
}

void SoundThread::AxCallbackProc()
{
	BOOL result ATTR_UNUSED =
		OSSendMessageAny(&mMsgQueue, MESSAGE_AX_CALLBACK, OS_MESSAGE_NO_FLAGS);

	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mPlayerCallbackList)
	{
		decltype(itr) curItr = itr++;

		curItr->OnUpdateVoiceSoundThread();
	}

	VoiceManager::GetInstance().NotifyVoiceUpdate();
}

void *SoundThread::SoundThreadFunc(void *arg)
{
	SoundThread *th = static_cast<SoundThread *>(arg);

	AxManager::GetInstance().RegisterCallback(&th->mAxCallbackNode,
	                                          &AxCallbackFunc);

	th->SoundThreadProc();

	AxManager::GetInstance().UnregisterCallback(&th->mAxCallbackNode);

	return nullptr;
}

/* SoundThread::RegisterSoundFrameCallback
 * ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14509)
 */
DECOMP_FORCE_CLASS_METHOD(SoundThread::SoundFrameCallback::LinkList,
                          PushBack(nullptr));

/* SoundThread::UnregisterSoundFrameCallback
 * ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14510)
 */
DECOMP_FORCE_CLASS_METHOD(SoundThread::SoundFrameCallback::LinkList,
                          Erase(nullptr));

void SoundThread::RegisterPlayerCallback(PlayerCallback *callback)
{
	ut::detail::AutoLock<OSMutex> lock(mMutex);

	mPlayerCallbackList.PushBack(callback);
}

void SoundThread::UnregisterPlayerCallback(PlayerCallback *callback)
{
	ut::detail::AutoLock<OSMutex> lock(mMutex);

	mPlayerCallbackList.Erase(callback);
}

void SoundThread::SoundThreadProc()
{
	OSMessage message;

	while (true)
	{
		OSReceiveMessage(&mMsgQueue, &message, OS_MESSAGE_FLAG_PERSISTENT);

		if (reinterpret_cast<u32>(message) == MESSAGE_AX_CALLBACK)
		{
			ut::detail::AutoLock<OSMutex> lock(mMutex);

			NW4R_RANGE_FOR_NO_AUTO_INC(itr, mSoundFrameCallbackList)
			{
				decltype(itr) curItr = itr++;

				curItr->at_0x0c();
			}

			OSTick tick = OSGetTick();

			{
				// Sound frame
				AxVoiceManager::GetInstance().FreeAllReservedAxVoice();
				AxManager::GetInstance().Update();

				NW4R_RANGE_FOR_NO_AUTO_INC(itr, mPlayerCallbackList)
				{
					decltype(itr) curItr = itr++;

					curItr->OnUpdateFrameSoundThread();
				}

				ChannelManager::GetInstance().UpdateAllChannel();
				(void)Util::CalcRandom(); // ?
				VoiceManager::GetInstance().UpdateAllVoices();
			}

			mProcessTick = OSGetTick() - tick;

			NW4R_RANGE_FOR_NO_AUTO_INC(itr, mSoundFrameCallbackList)
			{
				decltype(itr) curItr = itr++;

				curItr->at_0x10();
			}
		}
		else if (reinterpret_cast<u32>(message) == MESSAGE_SHUTDOWN)
		{
			break;
		}

		NW4RAssert_Line(313, *mStackEnd == OS_THREAD_STACK_MAGIC);
	}

	NW4R_RANGE_FOR_NO_AUTO_INC(itr, mPlayerCallbackList)
	{
		decltype(itr) curItr = itr++;

		curItr->OnShutdownSoundThread();
	}
}

}}} // namespace nw4r::snd::detail
