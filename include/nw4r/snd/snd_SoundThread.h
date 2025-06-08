#ifndef NW4R_SND_SOUND_THREAD_H
#define NW4R_SND_SOUND_THREAD_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_AxManager.h"

#include "nw4r/ut/ut_algorithm.h" // ut::NonCopyable
#include "nw4r/ut/ut_LinkList.h"

#include <rvl/OS/OSMessage.h>
#include <rvl/OS/OSMutex.h>
#include <rvl/OS/OSThread.h>

/*******************************************************************************
 * classes
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2bc156
	class SoundThread
	{
	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2bbe45
		class AutoLock : private ut::NonCopyable
		{
		// methods
		public:
			// cdtors
			AutoLock() { SoundThread::GetInstance().Lock(); }
			~AutoLock() { SoundThread::GetInstance().Unlock(); }
		}; // size 0x01 (0x00 for inheritance)

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2bc03f
		class SoundFrameCallback
		{
		// typedefs
		public:
			typedef ut::LinkList<SoundFrameCallback, 0x00> LinkList;

		// members
		private:
			ut::LinkListNode	mLink;	// size 0x08, offset 0x00
			/* vtable */				// size 0x04, offset 0x08

		// late virtual methods
		public:
			virtual void at_0x08();
			virtual void at_0x0c();
			virtual void at_0x10();
		}; // size 0x0c

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2cc6f
		class PlayerCallback
		{
		// typedefs
		public:
			typedef ut::LinkList<PlayerCallback, 0x00> LinkList;

		// members
		private:
			ut::LinkListNode	mLink;	// size 0x08, offset 0x00
			/* vtable */				// size 0x04, offset 0x08

		// late virtual methods
		public:
			virtual ~PlayerCallback() {}
			virtual void OnUpdateFrameSoundThread() {}
			virtual void OnUpdateVoiceSoundThread() {}
			virtual void OnShutdownSoundThread() {}
		}; // size 0x0c

	// methods
	public:
		// instance accessors
		static SoundThread &GetInstance();

		// methods
		bool Create(s32 priority, void *stack, u32 stackSize);
		void Shutdown();

		void Lock() { OSLockMutex(&mMutex); }
		void Unlock() { OSUnlockMutex(&mMutex); }

		void RegisterPlayerCallback(PlayerCallback *callback);
		void UnregisterPlayerCallback(PlayerCallback *callback);

	private:
		// cdtors
		SoundThread();

		// fibers, callbacks, and procedures
		static void AxCallbackFunc();
		void AxCallbackProc();

		static void *SoundThreadFunc(void *arg);
		void SoundThreadProc();
		void FrameProcess();

	// static members
	private:
		static int const MESSAGE_SHUTDOWN = 1 << 1;
		static int const MESSAGE_AX_CALLBACK = 1 << 0;
		static int const THREAD_MESSAGE_BUFSIZE = 4;

	// members
	private:
		OSThread						mThread;							// size 0x318, offset 0x000
		OSThreadQueue					mThreadQueue;						// size 0x008, offset 0x318
		OSMessageQueue					mMsgQueue;							// size 0x020, offset 0x320
		OSMessage						mMsgBuffer[THREAD_MESSAGE_BUFSIZE];	// size 0x010, offset 0x340
		u32							*mStackEnd;							// size 0x004, offset 0x350
		OSMutex							mMutex;								// size 0x018, offset 0x354
		AxManager::CallbackListNode		mAxCallbackNode;					// size 0x00c, offset 0x36c
		SoundFrameCallback::LinkList	mSoundFrameCallbackList;			// size 0x00c, offset 0x378
		PlayerCallback::LinkList		mPlayerCallbackList;				// size 0x00c, offset 0x384
		u32								mProcessTick;						// size 0x004, offset 0x390
		bool							mCreateFlag;						// size 0x001, offset 0x394
		bool							field_0x395;						// size 0x001, offset 0x395
		/* 3 bytes padding */
	}; // size 0x398
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_SOUND_THREAD_H
