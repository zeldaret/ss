#ifndef NW4R_SND_CHANNEL_H
#define NW4R_SND_CHANNEL_H

/*******************************************************************************
 * headers
 */

#include <climits> // USHRT_MAX

#include "common.h"

#include "nw4r/snd/snd_EnvGenerator.h"
#include "nw4r/snd/snd_InstancePool.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_Lfo.h"
#include "nw4r/snd/snd_MoveValue.h"
#include "nw4r/snd/snd_Voice.h"

#include "nw4r/ut/ut_LinkList.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class WaveDataLocationCallback; }}}
namespace nw4r { namespace snd { namespace detail { struct WaveInfo; }}}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2c353
	// NOTE: different from ketteiban: no remote fields
	class Channel
	{
	// enums
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x290ad
		enum ChannelCallbackStatus
		{
			CALLBACK_STATUS_STOPPED,
			CALLBACK_STATUS_DROP,
			CALLBACK_STATUS_FINISH,
			CALLBACK_STATUS_CANCEL,
		};

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2c2f1
		enum LfoTarget
		{
			LFO_TARGET_PITCH,
			LFO_TARGET_VOLUME,
			LFO_TARGET_PAN,
		};

	// typedefs
	public:
		typedef void Callback(Channel *dropChannel,
		                      ChannelCallbackStatus status,
		                      u32 userData);

		typedef ut::LinkList<Channel, 0xc8> LinkList;

	// methods
	public:
		// instance managers
		static Channel *AllocChannel(int voiceChannelCount, int voiceOutCount,
		                             int priority, Callback *callback,
		                             u32 callbackData);

		static void FreeChannel(Channel *channel);

		// methods
		void InitParam(Callback *callback, u32 callbackData);
		void Start(WaveInfo const &waveParam, int length, u32 startOffset);
		void Pause(bool flag)
		{
			mPauseFlag = flag;
			mVoice->Pause(flag);
		}
		void Stop();

		bool IsRelease() const
		{
			return mEnvelope.GetStatus() == EnvGenerator::STATUS_RELEASE;
		}
		bool IsPause() const { return mPauseFlag != false; }
		bool IsActive() const { return mActiveFlag; }
		bool IsAutoUpdateSweep() const { return mAutoSweep; }
		f32 GetSweepValue() const;
		s32 GetLength() const { return mLength; }
		int GetAlternateAssignId() const { return mAlternateAssign; }
		Voice *GetVoice() const { return mVoice; }
		Channel *GetNextTrackChannel() const { return mNextLink; }

		void SetDecay(int decay) { mEnvelope.SetDecay(decay); }
		void SetRelease(int release) { mEnvelope.SetRelease(release); }
		void SetAttack(int attack) { mEnvelope.SetAttack(attack); }
		void SetSustain(int sustain) { mEnvelope.SetSustain(sustain); }
		void SetHold(int hold) { mEnvelope.SetHold(hold); }
		void SetLfoParam(LfoParam const &param) { mLfo.SetParam(param); }
		void SetLfoTarget(LfoTarget type) { mLfoTarget = type; }
		void SetReleasePriorityFix(bool fix) { mReleasePriorityFixFlag = fix; }
		void SetReleaseIgnore(bool ignore) { mReleaseIgnoreFlag = ignore; }
		void SetBiquadFilter(int type, f32 value);
		void SetRemoteFilter(int filter) { mRemoteFilter = filter; }
		void SetUserVolume(f32 volume) { mUserVolume = volume; }
		void SetUserPitchRatio(f32 pitchRatio) { mUserPitchRatio = pitchRatio; }
		void SetUserPan(f32 pan) { mUserPan = pan; }
		void SetUserSurroundPan(f32 surroundPan)
		{
			mUserSurroundPan = surroundPan;
		}
		void SetUserLpfFreq(f32 lpfFreq) { mUserLpfFreq = lpfFreq; }
		void SetOutputLine(int lineFlag) { mOutputLineFlag = lineFlag; }
		void SetMainOutVolume(f32 volume) { mMainOutVolume = volume; }
		void SetMainSend(f32 send) { mMainSend = send; }
		void SetFxSend(AuxBus bus, f32 send) { mFxSend[bus] = send; }
		void SetUserPitch(f32 pitch) { mUserPitch = pitch; }
		void SetSweepParam(f32 sweepPitch, int sweepTime, bool autoUpdate);
		void SetInitVolume(f32 volume) { mInitVolume = volume; }
		void SetInitPan(f32 pan) { mInitPan = pan; }
		void SetInitSurroundPan(f32 surroundPan)
		{
			mInitSurroundPan = surroundPan;
		}
		void SetTune(f32 tune) { mTune = tune; }
		void SetSilence(bool silenceFlag, int fadeTimes)
		{
			NW4RAssertHeaderClampedLRValue_Line(138, fadeTimes, 0, USHRT_MAX);

			mSilenceVolume.SetTarget(silenceFlag ? SILENCE_VOLUME_MIN
		                                         : SILENCE_VOLUME_MAX,
		                             fadeTimes);
		}
		void SetKey(int key) { mKey = key; }
		void SetOriginalKey(int key) { mOriginalKey = key; }
		void SetLength(s32 length) { mLength = length; }
		void SetPanMode(PanMode panMode) { mPanMode = panMode; }
		void SetPanCurve(PanCurve panCurve) { mPanCurve = panCurve; }
		void SetAlternateAssignId(int id) { mAlternateAssign = id; }
		void SetWaveDataLocationCallback(WaveDataLocationCallback *callback,
		                                 WaveInfo const *waveInfo)
		{
			mWaveDataLocationCallback = callback;
			mWaveInfo = waveInfo;
		}
		void SetVoiceOutParam(int index, VoiceOutParam const &param)
		{
			mVoice->SetVoiceOutParam(index, param);
		}
		void SetNextTrackChannel(Channel *channel) { mNextLink = channel; }

		void Update(bool doPeriodicProc);
		void UpdateSweep(int count);
		void Release();
		void NoteOff();

	private:
		// cdtors
		Channel();
		~Channel();

		// callbacks
		static void VoiceCallbackFunc(Voice *voice,
		                              Voice::VoiceCallbackStatus status,
		                              void *arg);

	// static members
	public:
		static u8 const SILENCE_VOLUME_MIN = 0;
		static u8 const SILENCE_VOLUME_MAX = 255;

		static int const ORIGINAL_KEY_INIT = 60;
		static int const KEY_INIT = 60;

		static int const PRIORITY_RELEASE = 1;

		static int const CHANNEL_MAX = Voice::CHANNEL_MAX;
		static int const CHANNEL_MIN = 1;
		static int const CHANNEL_NUM;

	// members
	private:
		EnvGenerator				mEnvelope;					// size 0x1c, offset 0x00
		Lfo							mLfo;						// size 0x18, offset 0x1c
		u8							mLfoTarget;					// size 0x01, offset 0x34
		bool						mPauseFlag;					// size 0x01, offset 0x35
		bool						mActiveFlag;				// size 0x01, offset 0x36
		bool						mAllocFlag;					// size 0x01, offset 0x37
		bool						mAutoSweep;					// size 0x01, offset 0x38
		bool						mReleasePriorityFixFlag;	// size 0x01, offset 0x39
		u8							mReleaseIgnoreFlag;			// size 0x01, offset 0x3a // not a bool?
		u8							mBiquadType;				// size 0x01, offset 0x3b
		u8							mRemoteFilter;				// size 0x01, offset 0x3c
		/* 3 bytes padding */
		f32							mUserVolume;				// size 0x04, offset 0x40
		f32							mUserPitchRatio;			// size 0x04, offset 0x44
		f32							mUserPan;					// size 0x04, offset 0x48
		f32							mUserSurroundPan;			// size 0x04, offset 0x4c
		f32							mUserLpfFreq;				// size 0x04, offset 0x50
		f32							mBiquadValue;				// size 0x04, offset 0x54
		int							mOutputLineFlag;			// size 0x04, offset 0x58
		f32							mMainOutVolume;				// size 0x04, offset 0x5c
		f32							mMainSend;					// size 0x04, offset 0x60
		f32							mFxSend[AUX_BUS_NUM];		// size 0x0c, offset 0x64
		f32							mUserPitch;					// size 0x04, offset 0x70
		f32							mSweepPitch;				// size 0x04, offset 0x74
		s32							mSweepCounter;				// size 0x04, offset 0x78
		s32							mSweepLength;				// size 0x04, offset 0x7c
		f32							mInitVolume;				// size 0x04, offset 0x80
		f32							mInitPan;					// size 0x04, offset 0x84
		f32							mInitSurroundPan;			// size 0x04, offset 0x88
		f32							mTune;						// size 0x04, offset 0x8c
		MoveValue<u8, u16>			mSilenceVolume;				// size 0x06, offset 0x90
		/* 2 bytes padding */
		int							mKey;						// size 0x04, offset 0x98
		int							mOriginalKey;				// size 0x04, offset 0x9c
		s32							mLength;					// size 0x04, offset 0xa0
		PanMode						mPanMode;					// size 0x04, offset 0xa4
		PanCurve					mPanCurve;					// size 0x04, offset 0xa8
		int							mAlternateAssign;			// size 0x04, offset 0xac
		Callback					*mCallback;					// size 0x04, offset 0xb0
		u32							mCallbackData;				// size 0x04, offset 0xb4
		WaveDataLocationCallback	*mWaveDataLocationCallback;	// size 0x04, offset 0xb8
		WaveInfo					const *mWaveInfo;			// size 0x04, offset 0xbc
		Voice						*mVoice;					// size 0x04, offset 0xc0
		Channel						*mNextLink;					// size 0x04, offset 0xc4
	public:
		ut::LinkListNode			mLink;						// size 0x08, offset 0xc8

	// friends
	private:
		// for calling private constructor
		template <class> friend class InstancePool;
	}; // size 0xd0

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b895c
	class ChannelManager
	{
	// methods
	public:
		// instance accessors
		static ChannelManager &GetInstance();

		// methods
		u32 GetRequiredMemSize(int channelCount);
		void Setup(void *mem, u32 memSize);
		void Shutdown();

		Channel* Alloc();
		void Free(Channel *channel);

		void UpdateAllChannel();

	private:
		// cdtors
		ChannelManager();

	// members
	private:
		InstancePool<Channel>	mPool;			// size 0x04, offset 0x00
		Channel::LinkList		mChannelList;	// size 0x0c, offset 0x04
		bool					mInitialized;	// size 0x01, offset 0x10
		/* 3 bytes padding */
		u32						mChannelCount;	// size 0x04, offset 0x14
		void					*mMem;			// size 0x04, offset 0x18
		u32						mMemSize;		// size 0x04, offset 0x1c
	}; // size 0x20
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_CHANNEL_H
