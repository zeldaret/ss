#ifndef NW4R_SND_AX_MANAGER_H
#define NW4R_SND_AX_MANAGER_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_global.h"

#include "nw4r/snd/snd_FxBase.h"
#include "nw4r/snd/snd_MoveValue.h"

#include "nw4r/ut/ut_LinkList.h"

#include <rvl/AI/ai.h> // AIDMACallback
#include <rvl/AX/AX.h>
#include <rvl/AX/AXCL.h> // AX_MAX_VOLUME
#include <rvl/AX/AXAux.h> // AXAuxCallback
#include <rvl/AX/AXOut.h> // AXFrameCallback

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class BiquadFilterLpf; }}}
namespace nw4r { namespace snd { namespace detail { class BiquadFilterHpf; }}}
namespace nw4r { namespace snd { namespace detail { class BiquadFilterBpf512; }}}
namespace nw4r { namespace snd { namespace detail { class BiquadFilterBpf1024; }}}
namespace nw4r { namespace snd { namespace detail { class BiquadFilterBpf2048; }}}
namespace nw4r { namespace snd { class BiquadFilterCallback; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a5afe
	class AxManager
	{
	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2a6808
		struct CallbackListNode
		{
		// typedefs
		public:
			typedef ut::LinkList<CallbackListNode, 0x00> LinkList;

		// members
		public:
			ut::LinkListNode	node;		// size 0x08, offset 0x00
			AXFrameCallback		*callback;	// size 0x04, offset 0x08
		}; // size 0x0c

	// methods
	public:
		// instance accessors
		static AxManager &GetInstance();

		// methods
		void Init();
		void Update();
		void Shutdown();

		OutputMode GetOutputMode();
		void const *GetZeroBufferAddress();
		bool CheckInit() { return mInitialized; }
		f32 GetOutputVolume() const;
		FxBase::LinkList &GetEffectList(AuxBus bus)
		{
			NW4RAssertHeaderClampedLValue_Line(173, bus, AUX_A, AUX_BUS_NUM);

			return mFxList[bus];
		}
		BiquadFilterCallback const *GetBiquadFilterCallback(int index)
		{
			return sBiquadFilterCallbackTable[static_cast<u8>(index)];
		}

		void SetOutputMode(OutputMode mode);
		void SetMainOutVolume(f32 volume, int frames);
		void SetBiquadFilterCallback(int type,
		                             BiquadFilterCallback const *callback);

		void RegisterCallback(CallbackListNode *node,
		                      AXFrameCallback *callback);
		void UnregisterCallback(CallbackListNode *node);

		bool AppendEffect(AuxBus bus, FxBase *pFx);
		void ClearEffect(AuxBus bus, int frame);
		void ShutdownEffect(AuxBus bus);

		f32 GetMasterVolume() const {
			return mMasterVolume.GetValue();
		}
		void SetMasterVolume(f32 volume, int frame);
		void PrepareReset();

	private:
		// cdtors
		AxManager();

		// callbacks
		static void AxCallbackFunc();

	// static members
	private:
		static u8 const AUX_CALLBACK_WAIT_FRAME;
		static u16 const AUX_RETURN_VOLUME_MAX = AX_MAX_VOLUME;
		static int const FX_SAMPLE_RATE;
		static SampleFormat const FX_SAMPLE_FORMAT;
		static int const FX_BUFFER_SIZE;
		static int const ZERO_BUFFER_SIZE = 256;
		static int const SAMPLES_PAR_AUDIO_FRAME;
		static int const AUDIO_FRAME_INTERVAL;

		static byte_t sZeroBuffer[ZERO_BUFFER_SIZE];
		static BiquadFilterCallback const *sBiquadFilterCallbackTable[128];
		static BiquadFilterLpf sBiquadFilterLpf;
		static BiquadFilterHpf sBiquadFilterHpf;
		static BiquadFilterBpf512 sBiquadFilterBpf512;
		static BiquadFilterBpf1024 sBiquadFilterBpf1024;
		static BiquadFilterBpf2048 sBiquadFilterBpf2048;

	// members
	private:
		OutputMode					mOutputMode;							// size 0x04, offset 0x00
		void						*mZeroBufferAddress;					// size 0x04, offset 0x04
		CallbackListNode::LinkList	mCallbackList;							// size 0x0c, offset 0x08
		AXFrameCallback				*mNextAxRegisterCallback;				// size 0x04, offset 0x14
		bool						mInitialized;							// size 0x01, offset 0x18
		bool						mUpdateVoicePrioFlag;					// size 0x01, offset 0x19
		/* 2 bytes padding */
		MoveValue<f32, int>			mMasterVolume;							// size 0x10, offset 0x1c
		MoveValue<f32, int>			mMainOutVolume;							// size 0x10, offset 0x2c
		MoveValue<f32, int>			mVolumeForReset;						// size 0x10, offset 0x3c
		AIDMACallback				*mOldAidCallback;						// size 0x04, offset 0x4c
		s32							mResetReadyCounter;						// size 0x04, offset 0x50
		MoveValue<f32, int>			mAuxFadeVolume[AUX_BUS_NUM];			// size 0x30, offset 0x54
		MoveValue<f32, int>			mAuxUserVolume[AUX_BUS_NUM];			// size 0x30, offset 0x84
		FxBase::LinkList			mFxList[AUX_BUS_NUM];					// size 0x24, offset 0xb4
		AXAuxCallback				mAuxCallback[AUX_BUS_NUM];				// size 0x0c, offset 0xd8
		void						*mAuxCallbackContext[AUX_BUS_NUM];		// size 0x0c, offset 0xe4
		u8							mAuxCallbackWaitCounter[AUX_BUS_NUM];	// size 0x03, offset 0xf0
		/* 1 byte padding */
		u32							mEffectProcessTick[AUX_BUS_NUM];		// size 0x0c, offset 0xf4
	}; // size 0x100
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_AX_MANAGER_H
