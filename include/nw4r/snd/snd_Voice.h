#ifndef NW4R_SND_VOICE_H
#define NW4R_SND_VOICE_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_adpcm.h" // AdpcmLoopParam
#include "nw4r/snd/snd_AxVoice.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_DisposeCallbackManager.h" // DisposeCallback

#include "nw4r/ut/ut_LinkList.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { struct WaveInfo; }}}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b832
	// NOTE: different from ketteiban: no remote fields
	class Voice : public DisposeCallback
	{
	// enums
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b6c2
		enum VoiceCallbackStatus
		{
			CALLBACK_STATUS_FINISH_WAVE,
			CALLBACK_STATUS_CANCEL,
			CALLBACK_STATUS_DROP_VOICE,
			CALLBACK_STATUS_DROP_DSP,
		};

	// typedefs
	public:
		typedef ut::LinkList<Voice, 0xec> LinkList;

		typedef void Callback(Voice *dropVoice, VoiceCallbackStatus status,
		                      void *callbackData);

	// methods
	public:
		// cdtors
		Voice();
		virtual ~Voice();

		// virtual function ordering
		// vtable DisposeCallback
		virtual void InvalidateData(void const *, void const *) {}
		virtual void InvalidateWaveData(void const *start, void const *end);

		// methods
		void Setup(WaveInfo const &waveParam, u32 startOffset);

		int GetPriority() const { return mPriority; }

		void SetVoiceOutParam(int voiceOutIndex,
		                      VoiceOutParam const &voiceOutParam);
		void SetVoiceOutParamPitchDisableFlag(bool flag)
		{
			mVoiceOutParamPitchDisableFlag = flag;
		}
		void SetRemoteFilter(int filter);
		void SetBiquadFilter(int type, f32 value);
		void SetPriority(int priority);
		void SetPan(f32 pan);
		void SetSurroundPan(f32 surroundPan);
		void SetLpfFreq(f32 lpfFreq);
		void SetOutputLine(int flag);
		void SetMainOutVolume(f32 volume);
		void SetMainSend(f32 send);
		void SetFxSend(AuxBus bus, f32 send);
		void SetPitch(f32 pitch);
		void SetVolume(f32 volume);
		void SetVeVolume(f32 targetVolume, f32 initVolume);
		void SetPanMode(PanMode panMode);
		void SetPanCurve(PanCurve panCurve);

		bool IsActive() const { return mAxVoice[0][0] != nullptr; }
		bool IsPlayFinished() const
		{
			return IsActive() && mAxVoice[0][0]->IsPlayFinished();
		}
		SampleFormat GetFormat() const;
		u32 GetCurrentPlayingSample() const;
		int GetPhysicalVoiceCount() const
		{
			return mChannelCount * mVoiceOutCount;
		}

		void SetVoiceType(AxVoice::VoiceType type);
		void SetLoopFlag(bool loopFlag);
		void SetLoopStart(int channelIndex, void const *baseAddress,
		                  u32 samples);
		void SetLoopEnd(int channelIndex, void const *baseAddress, u32 samples);
		void SetAdpcmLoop(int channelIndex, AdpcmLoopParam const *param);

		bool Acquire(int channelCount, int voiceOutCount, int priority,
		             Callback *callback, void *callbackData);
		void Free();

		void InitParam(int channelCount, int voiceOutCount, Callback *callback,
		               void *callbackData);

		void Start();
		void Pause(bool flag);
		void Stop();
		void StopAtPoint(int channelIndex, void const *baseAddress,
		                 u32 samples);

	private:
		void RunAllAxVoice();
		void StopAllAxVoice();
		void SyncAxVoice();
		void StopFinished();

		void Calc();
		void CalcAxSrc(bool initialUpdate);
		void CalcAxVe();
		bool CalcAxMix();
		void CalcAxLpf();
		void CalcAxBiquadFilter();
		void CalcAxRemoteFilter();

		void CalcMixParam(int channelIndex, int voiceOutIndex,
		                  AxVoice::MixParam *mix,
		                  AxVoice::RemoteMixParam *rmtmix);

		void Update();
		void UpdateVoicesPriority();

		void ResetDelta();

		void TransformDpl2Pan(f32 *outPan, f32 *outSurroundPan, f32 inPan,
		                      f32 inSurroundPan);

		static void AxVoiceCallbackFunc(AxVoice *dropVoice,
		                                AxVoice::AxVoiceCallbackStatus status,
		                                void *callbackData);

	// static members
	public:
		static f32 const SURROUND_ATTENUATED_DB;
		static u32 const VOICE_PRIORITY_RELEASE;
		static u32 const VOICE_PRIORITY_ALLOC;
		static u32 const VOICE_PRIORITY_USE;
		static int const VOICE_PRIORITY_FREE;
		static u32 const VOICE_PRIORITY_NODROP;
		static u32 const VOICE_PRIORITY_MAX = 31; // just going to put it here
		static int const PRIORITY_RELEASE;
		static int const PRIORITY_NODROP;
		static int const PRIORITY_MIN = 0;
		static int const PRIORITY_MAX = 255;
		static int const REMOTE_FILTER_MAX = 127;
		static int const REMOTE_FILTER_MIN = 0;
		static f32 const SEND_MAX;
		static f32 const SEND_MIN;
		static f32 const BIQUAD_VALUE_MAX;
		static f32 const BIQUAD_VALUE_MIN;
		static f32 const CUTOFF_FREQ_MAX;
		static f32 const CUTOFF_FREQ_MIN;
		static f32 const SPAN_CENTER;
		static f32 const SPAN_REAR;
		static f32 const SPAN_FRONT;
		static f32 const PAN_CENTER;
		static f32 const PAN_RIGHT;
		static f32 const PAN_LEFT;
		static f32 const VOLUME_MAX;
		static f32 const VOLUME_MIN;
		static int const CHANNEL_MAX = 2;
		static int const UPDATE_BIQUAD = 1 << 8;
		static int const UPDATE_REMOTE_FILTER = 1 << 7;
		// 1 << 6 skipped
		static int const UPDATE_LPF = 1 << 5;
		static int const UPDATE_MIX = 1 << 4;
		static int const UPDATE_VE = 1 << 3;
		static int const UPDATE_SRC = 1 << 2;
		static int const UPDATE_PAUSE = 1 << 1;
		static int const UPDATE_START = 1 << 0;

	// members
	private:
		/* base DisposeCallback */							// size 0x0c, offset 0x00
		AxVoice				*mAxVoice[CHANNEL_MAX][4];		// size 0x20, offset 0x0c
		VoiceOutParam		mVoiceOutParam[4];				// size 0x60, offset 0x2c
		int					mChannelCount;					// size 0x04, offset 0x8c
		int					mVoiceOutCount;					// size 0x04, offset 0x90
		Callback			*mCallback;						// size 0x04, offset 0x94
		void				*mCallbackData;					// size 0x04, offset 0x98
		bool				mActiveFlag;					// size 0x01, offset 0x9c
		bool				mStartFlag;						// size 0x01, offset 0x9d
		bool				mStartedFlag;					// size 0x01, offset 0x9e
		bool				mPauseFlag;						// size 0x01, offset 0x9f
		bool				mPausingFlag;					// size 0x01, offset 0xa0
		bool				mVoiceOutParamPitchDisableFlag;	// size 0x01, offset 0xa1
		u16				mSyncFlag;						// size 0x02, offset 0xa2
		u8					mRemoteFilter;					// size 0x01, offset 0xa4
		u8					mBiquadType;					// size 0x01, offset 0xa5
		/* 2 bytes padding */
		int					mPriority;						// size 0x04, offset 0xa8
		f32					mPan;							// size 0x04, offset 0xac
		f32					mSurroundPan;					// size 0x04, offset 0xb0
		f32					mLpfFreq;						// size 0x04, offset 0xb4
		f32					mBiquadValue;					// size 0x04, offset 0xb8
		int					mOutputLineFlag;				// size 0x04, offset 0xbc
		f32					mMainOutVolume;					// size 0x04, offset 0xc0
		f32					mMainSend;						// size 0x04, offset 0xc4
		f32					mFxSend[AUX_BUS_NUM];			// size 0x0c, offset 0xc8
		f32					mPitch;							// size 0x04, offset 0xd4
		f32					mVolume;						// size 0x04, offset 0xd8
		f32					mVeInitVolume;					// size 0x04, offset 0xdc
		f32					mVeTargetVolume;				// size 0x04, offset 0xe0
		PanMode				mPanMode;						// size 0x04, offset 0xe4
		PanCurve			mPanCurve;						// size 0x04, offset 0xe8
	public:
		ut::LinkListNode	mLinkNode;						// size 0x08, offset 0xec

	// friends
	private:
		friend class VoiceManager;
	}; // size 0xf4
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_VOICE_H
