#ifndef NW4R_SND_SEQ_PLAYER_H
#define NW4R_SND_SEQ_PLAYER_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_BasicPlayer.h"
#include "nw4r/snd/snd_DisposeCallbackManager.h" // DisposeCallback
#include "nw4r/snd/snd_SoundThread.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class Channel; }}}
namespace nw4r { namespace snd { namespace detail { class NoteOnCallback; }}}
namespace nw4r { namespace snd { namespace detail { struct NoteOnInfo; }}}
namespace nw4r { namespace snd { namespace detail { class SeqTrack; }}}
namespace nw4r { namespace snd { namespace detail { class SeqTrackAllocator; }}}

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2dbb2
	struct SeqUserprocCallbackParam
	{
		s16		volatile *localVariable;	// size 0x04, offset 0x00
		s16		volatile *globalVariable;	// size 0x04, offset 0x04
		s16		volatile *trackVariable;	// size 0x04, offset 0x08
		bool	cmpFlag;					// size 0x01, offset 0x0C
		/* 3 bytes padding */
	}; // size 0x10
}} // namespace nw4r::snd

/*******************************************************************************
 * classes
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2dfad
	class SeqPlayer : public BasicPlayer,
	                  public DisposeCallback,
	                  public SoundThread::PlayerCallback
	{
	// enums
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2dee8
		enum OffsetType
		{
			OFFSET_TYPE_TICK,
			OFFSET_TYPE_MILLISEC,
		};

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2df3b
		enum SetupResult
		{
			SETUP_SUCCESS,

			SETUP_ERR_CANNOT_ALLOCATE_TRACK,

			SETUP_ERR_UNKNOWN
		};

	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2cce4
		struct ParserPlayerParam
		{
			u8				volume;		// size 0x01, offset 0x00
			u8				priority;	// size 0x01, offset 0x01
			u8				timebase;	// size 0x01, offset 0x02
			/* 1 byte padding */
			u16				tempo;		// size 0x02, offset 0x04
			/* 2 bytes padding */
			NoteOnCallback	*callback;	// size 0x04, offset 0x08
		}; // size 0x0c

	// typedefs
	public:
		typedef void SeqUserprocCallback(u16 procId,
		                                 SeqUserprocCallbackParam *param,
		                                 void *callbackData);

	// methods
	public:
		// cdtors
		SeqPlayer();
		virtual ~SeqPlayer();

		// virtual function ordering
		// vtable BasicPlayer
		virtual bool Start();
		virtual void Stop();
		virtual void Pause(bool flag);
		virtual bool IsActive() const { return mActiveFlag; }
		virtual bool IsStarted() const { return mStartedFlag; }
		virtual bool IsPause() const { return mPauseFlag; }

		// vtable DisposeCallback
		virtual void InvalidateData(void const *start, void const *end);
		virtual void InvalidateWaveData(void const *, void const *) {}

		// vtable SeqPlayer
		/* WARNING: must come before SoundThread::PlayerCallback virtual
		 * functions
		 */
		virtual void ChannelCallback(Channel *channel ATTR_UNUSED) {}

		// vtable SoundThread::PlayerCallback
		virtual void OnUpdateFrameSoundThread() { Update(); }
		virtual void OnShutdownSoundThread() { Stop(); }

		// methods
		SetupResult Setup(SeqTrackAllocator *trackAllocator, u32 allocTracks,
		                  int voiceOutCount, NoteOnCallback *callback);
		void Update();
		void Skip(OffsetType offsetType, int offset);
		void Shutdown();

		bool IsReleasePriorityFix() const { return mReleasePriorityFixFlag; }
		f32 GetPanRange() const { return mPanRange; }
		int GetVoiceOutCount() const { return mVoiceOutCount; }
		ParserPlayerParam &GetParserPlayerParam() { return mParserParam; }
		SeqTrack *GetPlayerTrack(int trackNo);
		s16 volatile *GetVariablePtr(int varNo);

		void SetReleasePriorityFix(bool fix);
		void SetChannelPriority(int priority);
		void SetSeqUserprocCallback(SeqUserprocCallback *callback, void *arg);
		void SetPlayerTrack(int trackNo, SeqTrack *track);

		f32 CalcTickPerMsec() const
		{
			return CalcTickPerMinute() / (60 * 1000);
		}
		f32 CalcTickPerMinute() const
		{
			return mParserParam.timebase * mParserParam.tempo * mTempoRatio;
		}

		void SetSeqData(void const *seqBase, s32 seqOffset);

		void CallSeqUserprocCallback(u16 procId, SeqTrack *track);
		Channel *NoteOn(int bankNo, NoteOnInfo const &noteOnInfo);

		static void InitSeqPlayer();

	private:
		void InitParam(int voiceOutCount, NoteOnCallback *callback);

		void UpdateChannelParam();

		void CloseTrack(int trackNo);
		void FinishPlayer();

		int ParseNextTick(bool doNoteOn); // meant to be bool?
		void UpdateTick(int msec);
		void SkipTick();

	// static members
	public:
		static int const MAX_SKIP_TICK_PER_FRAME = 768;
		static int const DEFAULT_TEMPO = 120;
		static int const DEFAULT_TIMEBASE = 48;
		static int const VARIABLE_DEFAULT_VALUE = -1;
		static int const TRACK_NUM_PER_PLAYER = 16;
		static int const GLOBAL_VARIABLE_NUM = 16;
		static int const PLAYER_VARIABLE_NUM = 16;

	private:
		static s16 mGlobalVariable[GLOBAL_VARIABLE_NUM];

	// members
	private:
		/* base BasicPlayer */												// size 0x0a4, offset 0x000
		/* base DisposeCallback */											// size 0x00c, offset 0x0a4
		/* base SoundThread::PlayerCallback */								// size 0x00c, offset 0x0b0
		bool				mActiveFlag;									// size 0x001, offset 0x0ec
		bool				mStartedFlag;									// size 0x001, offset 0x0ed
		bool				mPauseFlag;										// size 0x001, offset 0x0ee
		bool				mReleasePriorityFixFlag;						// size 0x001, offset 0x0ef
		f32					mPanRange;										// size 0x004, offset 0x0f0
		f32					mTempoRatio;									// size 0x004, offset 0x0f4
		f32					mTickFraction;									// size 0x004, offset 0x0f8
		u32					mSkipTickCounter;								// size 0x004, offset 0x0fc
		f32					mSkipTimeCounter;								// size 0x004, offset 0x100
		s32					mVoiceOutCount;									// size 0x004, offset 0x104
		ParserPlayerParam	mParserParam;									// size 0x00c, offset 0x108
		SeqTrackAllocator	*mSeqTrackAllocator;							// size 0x004, offset 0x114
		SeqUserprocCallback	*mSeqUserprocCallback;							// size 0x004, offset 0x118
		void				*mSeqUserprocCallbackArg;						// size 0x004, offset 0x11c
		SeqTrack			*mTracks[TRACK_NUM_PER_PLAYER];					// size 0x040, offset 0x120
		s16					volatile mLocalVariable[PLAYER_VARIABLE_NUM];	// size 0x020, offset 0x160
		u32					mTickCounter;									// size 0x004, offset 0x180
	}; // size 0x184
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_SEQ_PLAYER_H
