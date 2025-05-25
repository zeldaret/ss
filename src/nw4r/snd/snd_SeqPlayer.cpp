#include "nw4r/snd/snd_SeqPlayer.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SeqPlayer.cpp
 */

/*******************************************************************************
 * headers
 */

#include <decomp.h>
#include "common.h"

#include "nw4r/snd/snd_BasicPlayer.h"
#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_NoteOnCallback.h"
#include "nw4r/snd/snd_SeqTrack.h"
#include "nw4r/snd/snd_SeqTrackAllocator.h"
#include "nw4r/snd/snd_SoundThread.h"

#include "nw4r/ut/ut_Lock.h" // ut::AutoInterruptLock

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class Channel; }}}

/*******************************************************************************
 * variables
 */

namespace nw4r { namespace snd { namespace detail
{
	// .bss
	s16 SeqPlayer::mGlobalVariable[GLOBAL_VARIABLE_NUM];
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

void SeqPlayer::InitSeqPlayer()
{
	for (int variableNo = 0; variableNo < GLOBAL_VARIABLE_NUM; variableNo++)
		mGlobalVariable[variableNo] = VARIABLE_DEFAULT_VALUE;
}

SeqPlayer::SeqPlayer() :
	mActiveFlag				(false),
	mStartedFlag			(false),
	mPauseFlag				(false),
	mReleasePriorityFixFlag	(false),
	mTempoRatio				(1.0f),
	mTickFraction			(0.0f),
	mSkipTickCounter		(0),
	mSkipTimeCounter		(0.0f)
{
	mPanRange				= 1.0f;
	mTickCounter			= 0;
	mVoiceOutCount			= 0;

	mSeqUserprocCallback	= nullptr;
	mSeqUserprocCallbackArg	= nullptr;

	mParserParam.tempo		= DEFAULT_TEMPO;
	mParserParam.timebase	= DEFAULT_TIMEBASE;
	mParserParam.volume		= 127;
	mParserParam.priority	= 64;
	mParserParam.callback	= nullptr;

	for (int varNo = 0; varNo < PLAYER_VARIABLE_NUM; varNo++)
		mLocalVariable[varNo] = VARIABLE_DEFAULT_VALUE;

	for (int trackNo = 0; trackNo < TRACK_NUM_PER_PLAYER; trackNo++)
		mTracks[trackNo] = nullptr;
}

SeqPlayer::~SeqPlayer()
{
	Shutdown();
}

void SeqPlayer::InitParam(int voiceOutCount, NoteOnCallback *callback)
{
	BasicPlayer::InitParam();

	mStartedFlag			= false;
	mPauseFlag				= false;
	mTempoRatio				= 1.0f;
	mSkipTickCounter		= 0;
	mSkipTimeCounter		= 0.0f;
	mPanRange				= 1.0f;
	mTickCounter			= 0;
	mVoiceOutCount			= voiceOutCount;

	mParserParam.tempo		= DEFAULT_TEMPO;
	mParserParam.timebase	= DEFAULT_TIMEBASE;
	mParserParam.volume		= 127;
	mParserParam.priority	= 64;
	mParserParam.callback	= callback;

	mTickFraction			= 0.0f;

	for (int varNo = 0; varNo < PLAYER_VARIABLE_NUM; varNo++)
		mLocalVariable[varNo] = VARIABLE_DEFAULT_VALUE;

	for (int trackNo = 0; trackNo < TRACK_NUM_PER_PLAYER; trackNo++)
		mTracks[trackNo] = nullptr;
}

SeqPlayer::SetupResult SeqPlayer::Setup(SeqTrackAllocator *trackAllocator,
                                        u32 allocTracks, int voiceOutCount,
                                        NoteOnCallback *callback)
{
	SoundThread::AutoLock lock;

	SeqPlayer::Stop(); // NOTE: qualified name to inhibit dynamic dispatch
	InitParam(voiceOutCount, callback);

	{
		ut::AutoInterruptLock lockIntr;

		int trackCount = 0;

		// popcnt, pretty sure
		for (u32 trackBitMask = allocTracks; trackBitMask; trackBitMask >>= 1)
		{
			if (trackBitMask & 1)
				trackCount++;
		}

		if (trackCount > trackAllocator->GetAllocatableTrackCount())
			return SETUP_ERR_CANNOT_ALLOCATE_TRACK;

		u32 trackBitMask = allocTracks;

		// popcnt again, pretty sure
		for (int trackNo = 0; trackBitMask; trackBitMask >>= 1, trackNo++)
		{
			if (trackBitMask & 1)
			{
				SeqTrack *track = trackAllocator->AllocTrack(this);
				NW4RAssertPointerNonnull_Line(199, track);

				SetPlayerTrack(trackNo, track);
			}
		}

		// end of lockIntr's scope
	}

	DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);

	mSeqTrackAllocator = trackAllocator;
	mActiveFlag = true;

	return SETUP_SUCCESS;
}

void SeqPlayer::Shutdown()
{
	SoundThread::AutoLock lock;

	FinishPlayer();
}

void SeqPlayer::SetSeqData(void const *seqBase, s32 seqOffset)
{
	SoundThread::AutoLock lock;

	SeqTrack *seqTrack = GetPlayerTrack(0);
	NW4RAssertPointerNonnull_Line(245, seqTrack);

	if (seqBase)
	{
		seqTrack->SetSeqData(seqBase, seqOffset);
		seqTrack->Open();
	}
}

bool SeqPlayer::Start()
{
	SoundThread::AutoLock lock;

	SoundThread::GetInstance().RegisterPlayerCallback(this);
	mStartedFlag = true;

	return true;
}

void SeqPlayer::Stop()
{
	SoundThread::AutoLock lock;

	FinishPlayer();
}

void SeqPlayer::Pause(bool flag)
{
	SoundThread::AutoLock lock;

	mPauseFlag = flag;

	SeqTrack *track; // declared before trackNo in ketteiban dwarf

	for (int trackNo = 0; trackNo < TRACK_NUM_PER_PLAYER; trackNo++)
	{
		track = GetPlayerTrack(trackNo);

		if (track)
			track->PauseAllChannel(flag);
	}
}

void SeqPlayer::Skip(OffsetType offsetType, int offset)
{
	SoundThread::AutoLock lock;

	if (!mActiveFlag)
		return;

	switch (offsetType)
	{
	case OFFSET_TYPE_TICK:
		mSkipTickCounter += offset;
		break;

	case OFFSET_TYPE_MILLISEC:
		mSkipTimeCounter += offset;
		break;
	}
}

// SeqPlayer::SetTempoRatio ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13781)
DECOMP_FORCE(NW4RAssert_String(tempoRatio >= 0.0f));

void SeqPlayer::SetTempoRatio(f32 tempo) {
    mTempoRatio = tempo;
}

void SeqPlayer::SetChannelPriority(int priority)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(358, priority, 0, 127);

	mParserParam.priority	= priority;
}

void SeqPlayer::SetReleasePriorityFix(bool fix)
{
	mReleasePriorityFixFlag	= fix;
}

void SeqPlayer::SetSeqUserprocCallback(SeqUserprocCallback *callback, void *arg)
{
	mSeqUserprocCallback	= callback;
	mSeqUserprocCallbackArg	= arg;
}

void SeqPlayer::CallSeqUserprocCallback(u16 procId, SeqTrack *track)
{
	if (!mSeqUserprocCallback)
		return;

	NW4RAssertPointerNonnull_Line(377, track);

	SeqTrack::ParserTrackParam &trackParam = track->GetParserTrackParam();

	SeqUserprocCallbackParam param;
	param.localVariable		= GetVariablePtr(0);
	param.globalVariable	= GetVariablePtr(16);
	param.trackVariable		= track->GetVariablePtr(0);
	param.cmpFlag			= trackParam.cmpFlag;

	(*mSeqUserprocCallback)(procId, &param, mSeqUserprocCallbackArg);

	trackParam.cmpFlag = param.cmpFlag;
}

// SeqPlayer::GetLocalVariable? maybe all of them?
DECOMP_FORCE(NW4RAssertHeaderClampedLValue_String(varNo));

// SeqPlayer::SetTrackMute ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13791)
DECOMP_FORCE(&SeqTrack::SetMute);

// SeqPlayer::SetTrackSilence ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13792)
DECOMP_FORCE(&SeqTrack::SetSilence);

// SeqPlayer::SetTrackVolume ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13793)
DECOMP_FORCE(&SeqTrack::SetVolume);
DECOMP_FORCE(NW4RAssert_String(volume >= 0.0f));

// SeqPlayer::SetTrackPitch ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13794)
DECOMP_FORCE(&SeqTrack::SetPitch);
DECOMP_FORCE(NW4RAssert_String(pitch >= 0.0f));

// SeqPlayer::SetTrackPan ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13795)
DECOMP_FORCE(&SeqTrack::SetPan);

// SeqPlayer::SetTrackSurroundPan ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13796)
DECOMP_FORCE(&SeqTrack::SetSurroundPan);

// SeqPlayer::SetTrackLpfFreq ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13797)
DECOMP_FORCE(&SeqTrack::SetLpfFreq);

// SeqPlayer::SetTrackBiquadFilter ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13798)
DECOMP_FORCE(&SeqTrack::SetBiquadFilter);

// SeqPlayer::SetTrackPanRange ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13799)
DECOMP_FORCE(&SeqTrack::SetPanRange);

// SeqPlayer::SetTrackModDepth ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13800)
DECOMP_FORCE(&SeqTrack::SetModDepth);

// SeqPlayer::SetTrackModSpeed ([R89JEL]:/bin/RVL/Debug/mainD.MAP:13801)
DECOMP_FORCE(&SeqTrack::SetModSpeed);

void SeqPlayer::InvalidateData(void const *start, void const *end)
{
	SoundThread::AutoLock lock;

	if (mActiveFlag)
	{
		for (int trackNo = 0; trackNo < TRACK_NUM_PER_PLAYER; trackNo++)
		{
			SeqTrack *track = GetPlayerTrack(trackNo);
			if (!track)
				continue;

			byte_t const *cur = track->GetParserTrackParam().baseAddr;
			if (start <= cur && cur <= end)
			{
				// NOTE: qualified name to inhibit dynamic dispatch
				SeqPlayer::Stop();
				break;
			}
		}
	}
}

SeqTrack *SeqPlayer::GetPlayerTrack(int trackNo)
{
	if (trackNo > TRACK_NUM_PER_PLAYER - 1)
		return nullptr;

	return mTracks[trackNo];
}

void SeqPlayer::CloseTrack(int trackNo)
{
	SoundThread::AutoLock lock;

	// specifically not the source variant
	NW4RAssertHeaderClampedLValue_Line(609, trackNo, 0, TRACK_NUM_PER_PLAYER);

	SeqTrack *track = GetPlayerTrack(trackNo);
	if (!track)
		return;

	track->Close();

	mSeqTrackAllocator->FreeTrack(mTracks[trackNo]);
	mTracks[trackNo] = nullptr;
}

void SeqPlayer::SetPlayerTrack(int trackNo, SeqTrack *track)
{
	SoundThread::AutoLock lock;

	if (trackNo > TRACK_NUM_PER_PLAYER - 1)
		return;

	mTracks[trackNo] = track;
	track->SetPlayerTrackNo(trackNo);
}

void SeqPlayer::FinishPlayer()
{
	SoundThread::AutoLock lock;

	if (mStartedFlag)
	{
		SoundThread::GetInstance().UnregisterPlayerCallback(this);
		mStartedFlag = false;
	}

	if (mActiveFlag)
	{
		DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(this);
		mActiveFlag = false;
	}

	for (int trackNo = 0; trackNo < TRACK_NUM_PER_PLAYER; trackNo++)
		CloseTrack(trackNo);
}

void SeqPlayer::UpdateChannelParam()
{
	SoundThread::AutoLock lock;

	SeqTrack *track; // declared before trackNo in ketteiban dwarf

	for (int trackNo = 0; trackNo < TRACK_NUM_PER_PLAYER; trackNo++)
	{
		track = GetPlayerTrack(trackNo);

		if (track)
			track->UpdateChannelParam();
	}
}

BOOL SeqPlayer::ParseNextTick(bool doNoteOn)
{
	SoundThread::AutoLock lock;

	bool activeFlag = false;

	for (int trackNo = 0; trackNo < TRACK_NUM_PER_PLAYER; trackNo++)
	{
		SeqTrack *track = GetPlayerTrack(trackNo);
		if (!track)
			continue;

		track->UpdateChannelLength();

		if (track->ParseNextTick(doNoteOn) < 0)
			CloseTrack(trackNo);

		if (track->IsOpened())
			activeFlag = true;
	}

	if (!activeFlag)
		return TRUE;

	return FALSE;
}

s16 volatile *SeqPlayer::GetVariablePtr(int varNo)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(
		746, varNo, 0, PLAYER_VARIABLE_NUM + GLOBAL_VARIABLE_NUM);

	if (varNo < PLAYER_VARIABLE_NUM)
		return &mLocalVariable[varNo];

	if (varNo < PLAYER_VARIABLE_NUM + GLOBAL_VARIABLE_NUM)
		return &mGlobalVariable[varNo - PLAYER_VARIABLE_NUM];

	return nullptr;
}

void SeqPlayer::Update()
{
	SoundThread::AutoLock lock;

	NW4RAssert_Line(772, mActiveFlag);

	if (!mActiveFlag)
		return;

	if (!mStartedFlag)
		return;

	if (mSkipTickCounter || mSkipTimeCounter > 0.0f)
		SkipTick();
	else if (!mPauseFlag)
		UpdateTick(3);

	UpdateChannelParam();
}

void SeqPlayer::UpdateTick(int msec)
{
	f32 tickPerMsec = CalcTickPerMsec();
	if (tickPerMsec == 0.0f)
		return;

	f32 restMsec = static_cast<f32>(msec);
	f32 nextMsec = mTickFraction / tickPerMsec;

	while (nextMsec < restMsec)
	{
		restMsec -= nextMsec;

		if (ParseNextTick(true))
		{
			FinishPlayer();
			return;
		}

		mTickCounter++;

		tickPerMsec = CalcTickPerMsec();
		if (tickPerMsec == 0.0f)
			return;

		nextMsec = 1.0f / tickPerMsec;
	}

	nextMsec -= restMsec;
	mTickFraction = nextMsec * tickPerMsec;
}

void SeqPlayer::SkipTick()
{
	for (int trackNo = 0; trackNo < TRACK_NUM_PER_PLAYER; trackNo++)
	{
		SeqTrack *track = GetPlayerTrack(trackNo);

		if (track)
		{
			track->ReleaseAllChannel(127);
			track->FreeAllChannel();
		}
	}

	// TODO: can combine into for loop?
	int skipCount = 0;
	while (mSkipTickCounter || mSkipTimeCounter * CalcTickPerMsec() >= 1.0f)
	{
		if (skipCount >= MAX_SKIP_TICK_PER_FRAME)
			return;

		if (mSkipTickCounter)
		{
			mSkipTickCounter--;
		}
		else
		{
			f32 tickPerMsec = CalcTickPerMsec();
			NW4RAssert_Line(856, tickPerMsec > 0.0f);

			f32 msecPerTick = 1.0f / tickPerMsec;

			mSkipTimeCounter -= msecPerTick;
		}

		if (ParseNextTick(false))
		{
			FinishPlayer();
			return;
		}

		skipCount++;
		mTickCounter++;
	}

	mSkipTimeCounter = 0.0f;
}

Channel *SeqPlayer::NoteOn(int bankNo, NoteOnInfo const &noteOnInfo)
{
	Channel *channel = mParserParam.callback->NoteOn(this, bankNo, noteOnInfo);

	return channel;
}

}}} // namespace nw4r::snd::detail
