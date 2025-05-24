#include "nw4r/snd/SeqTrack.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SeqTrack.cpp
 */

/*******************************************************************************
 * headers
 */

#include <decomp.h>
#include <types.h>

#include "nw4r/snd/Channel.h"
#include "nw4r/snd/global.h"
#include "nw4r/snd/Lfo.h"
#include "nw4r/snd/MoveValue.h"
#include "nw4r/snd/NoteOnCallback.h"
#include "nw4r/snd/SeqPlayer.h"
#include "nw4r/snd/SoundThread.h"

#include "nw4r/ut/inlines.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

void SeqTrack::SetPlayerTrackNo(int playerTrackNo)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(38, playerTrackNo, 0,
	                                    SeqPlayer::TRACK_NUM_PER_PLAYER);

	mPlayerTrackNo = playerTrackNo;
}

SeqTrack::SeqTrack() :
	mOpenFlag		(false),
	mSeqPlayer		(nullptr),
	mChannelList	(nullptr)
{
	InitParam();
}

SeqTrack::~SeqTrack()
{
	Close();
}

void SeqTrack::InitParam()
{
	mExtVolume		= 1.0f;
	mExtPitch		= 1.0f;
	mExtPan			= 0.0f;
	mExtSurroundPan	= 0.0f;
	mPanRange		= 1.0f;

	mParserTrackParam.baseAddr			= nullptr;
	mParserTrackParam.currentAddr		= nullptr;
	mParserTrackParam.cmpFlag			= true;
	mParserTrackParam.noteWaitFlag		= true;
	mParserTrackParam.tieFlag			= false;
	mParserTrackParam.monophonicFlag	= false;
	mParserTrackParam.callStackDepth	= 0;
	mParserTrackParam.wait				= 0;
	mParserTrackParam.muteFlag			= false;
	mParserTrackParam.silenceFlag		= false;
	mParserTrackParam.noteFinishWait	= false;
	mParserTrackParam.portaFlag			= false;
	mParserTrackParam.damperFlag		= false;
	mParserTrackParam.bankNo			= 0;
	mParserTrackParam.prgNo				= 0;
	mParserTrackParam.lfoParam.Init();
	mParserTrackParam.lfoTarget			= 0;
	mParserTrackParam.sweepPitch		= 0.0f;
	mParserTrackParam.volume.InitValue(127);
	mParserTrackParam.pan.InitValue(0);
	mParserTrackParam.surroundPan.InitValue(0);
	mParserTrackParam.volume2			= 127;
	mParserTrackParam.velocityRange		= 127;
	mParserTrackParam.pitchBend			= 0;
	mParserTrackParam.bendRange			= DEFAULT_BENDRANGE;
	mParserTrackParam.initPan			= 0;
	mParserTrackParam.transpose			= 0;
	mParserTrackParam.priority			= DEFAULT_PRIORITY;
	mParserTrackParam.portaKey			= DEFAULT_PORTA_KEY;
	mParserTrackParam.portaTime			= 0;
	mParserTrackParam.attack			= INVALID_ENVELOPE;
	mParserTrackParam.decay				= INVALID_ENVELOPE;
	mParserTrackParam.sustain			= INVALID_ENVELOPE;
	mParserTrackParam.release			= INVALID_ENVELOPE;
	mParserTrackParam.envHold			= INVALID_ENVELOPE;
	mParserTrackParam.mainSend			= 127;

	for (int i = 0; i < AUX_BUS_NUM; i++)
		mParserTrackParam.fxSend[i] = 0;

	mParserTrackParam.lpfFreq			= 0.0f;
	mParserTrackParam.biquadType		= 0;
	mParserTrackParam.biquadValue		= 0.0f;

	for (int varNo = 0; varNo < TRACK_VARIABLE_NUM; varNo++)
		mTrackVariable[varNo] = SeqPlayer::VARIABLE_DEFAULT_VALUE;
}

void SeqTrack::SetSeqData(void const *seqBase, s32 seqOffset)
{
	mParserTrackParam.baseAddr = static_cast<byte_t const *>(seqBase);
	mParserTrackParam.currentAddr = mParserTrackParam.baseAddr + seqOffset;
}

void SeqTrack::Open()
{
	mParserTrackParam.noteFinishWait	= false;
	mParserTrackParam.callStackDepth	= 0;
	mParserTrackParam.wait				= 0;

	mOpenFlag = true;
}

void SeqTrack::Close()
{
	SoundThread::AutoLock lock;

	ReleaseAllChannel(MUTE_RELEASE_VALUE);
	FreeAllChannel();

	mOpenFlag = false;
}

void SeqTrack::UpdateChannelLength()
{
	SoundThread::AutoLock lock;

	if (!mOpenFlag)
		return;

	for (Channel *channel = mChannelList; channel;
	     channel = channel->GetNextTrackChannel())
	{
		if (channel->GetLength() > 0)
			channel->SetLength(channel->GetLength() - 1);

		UpdateChannelRelease(channel);

		if (!channel->IsAutoUpdateSweep())
			channel->UpdateSweep(1);
	}
}

void SeqTrack::UpdateChannelRelease(Channel *channel)
{
	if (channel->GetLength() == 0 && !channel->IsRelease()
	    && !mParserTrackParam.damperFlag)
	{
		channel->NoteOff();
	}
}

int SeqTrack::ParseNextTick(bool doNoteOn)
{
	SoundThread::AutoLock lock;

	if (!mOpenFlag)
		return 0;

	mParserTrackParam.volume.Update();
	mParserTrackParam.pan.Update();
	mParserTrackParam.surroundPan.Update();

	if (mParserTrackParam.noteFinishWait)
	{
		if (mChannelList)
			return 1;

		mParserTrackParam.noteFinishWait = false;
	}

	if (mParserTrackParam.wait > 0 && --mParserTrackParam.wait > 0)
		return 1;

	if (mParserTrackParam.currentAddr)
	{
		while (mParserTrackParam.wait == 0 && !mParserTrackParam.noteFinishWait)
		{
			ParseResult result = Parse(doNoteOn);
			if (result == PARSE_RESULT_FINISH)
				return -1;
		}
	}

	return 1;
}

void SeqTrack::StopAllChannel()
{
	SoundThread::AutoLock lock;

	for (Channel *channel = mChannelList; channel;
	     channel = channel->GetNextTrackChannel())
	{
		Channel::FreeChannel(channel);
		channel->Stop();
	}

	mChannelList = nullptr;
}

void SeqTrack::ReleaseAllChannel(int release)
{
	SoundThread::AutoLock lock;

	UpdateChannelParam();

	for (Channel *channel = mChannelList; channel;
	     channel = channel->GetNextTrackChannel())
	{
		if (channel->IsActive())
		{
			if (release >= 0)
			{
				NW4RAssertHeaderClampedLRValue_Line(329, release, 0,
				                                    MAX_ENVELOPE_VALUE);

				channel->SetRelease(static_cast<u8>(release));
			}

			channel->Release();
		}
	}
}

void SeqTrack::PauseAllChannel(bool flag)
{
	SoundThread::AutoLock lock;

	for (Channel *channel = mChannelList; channel;
	     channel = channel->GetNextTrackChannel())
	{
		if (channel->IsActive() && flag != channel->IsPause())
			channel->Pause(flag);
	}
}

void SeqTrack::AddChannel(Channel *channel)
{
	SoundThread::AutoLock lock;

	channel->SetNextTrackChannel(mChannelList);
	mChannelList = channel;
}

void SeqTrack::UpdateChannelParam()
{
	SoundThread::AutoLock lock;

	if (!mOpenFlag)
		return;

	if (!mChannelList)
		return;

	f32 volume = 1.0f;
	f32 parserVolume = mParserTrackParam.volume.GetValue() / 127.0f;
	f32 parserVolume2 = mParserTrackParam.volume2 / 127.0f;
	f32 parserMainVolume = mSeqPlayer->GetParserPlayerParam().volume / 127.0f;

	volume *= parserVolume * parserVolume;
	volume *= parserVolume2 * parserVolume2;
	volume *= parserMainVolume * parserMainVolume;
	volume *= mExtVolume;
	volume *= mSeqPlayer->GetVolume();

	f32 pitch =
		mParserTrackParam.pitchBend / 128.0f * mParserTrackParam.bendRange;

	f32 pitchRatio = 1.0f;
	pitchRatio *= mSeqPlayer->GetPitch();
	pitchRatio *= mExtPitch;

	f32 pan = 0.0f;
	pan += ut::Clamp(mParserTrackParam.pan.GetValue() / 63.0f, -1.0f, 1.0f);
	pan *= mPanRange;
	pan *= mSeqPlayer->GetPanRange();
	pan += mExtPan;
	pan += mSeqPlayer->GetPan();

	f32 surroundPan = 0.0f;
	surroundPan +=
		ut::Clamp(mParserTrackParam.surroundPan.GetValue() / 63.0f, 0.0f, 2.0f);
	surroundPan += mExtSurroundPan;
	surroundPan += mSeqPlayer->GetSurroundPan();

	f32 lpfFreq = 0.0f;
	lpfFreq += mParserTrackParam.lpfFreq;
	lpfFreq += mSeqPlayer->GetLpfFreq();

	int biquadType = mParserTrackParam.biquadType;
	f32 biquadValue = mParserTrackParam.biquadValue;

	if (mSeqPlayer->GetBiquadType() != 0)
	{
		biquadType = mSeqPlayer->GetBiquadType();
		biquadValue = mSeqPlayer->GetBiquadValue();
	}

	int remoteFilter = 0;
	remoteFilter += mSeqPlayer->GetRemoteFilter();

	f32 mainSend = 0.0f;
	mainSend += mParserTrackParam.mainSend / 127.0f - 1.0f;
	mainSend += mSeqPlayer->GetMainSend();

	f32 fxSend[AUX_BUS_NUM];
	for (int i = 0; i < AUX_BUS_NUM; i++)
	{
		AuxBus bus = static_cast<AuxBus>(i);
		fxSend[i] = 0.0f;
		fxSend[i] += mParserTrackParam.fxSend[i] / 127.0f;
		fxSend[i] += mSeqPlayer->GetFxSend(bus);
	}

	for (Channel *channel = mChannelList; channel;
	     channel = channel->GetNextTrackChannel())
	{
		channel->SetUserVolume(volume);
		channel->SetUserPitch(pitch);
		channel->SetUserPitchRatio(pitchRatio);
		channel->SetUserPan(pan);
		channel->SetUserSurroundPan(surroundPan);
		channel->SetUserLpfFreq(lpfFreq);
		channel->SetBiquadFilter(biquadType, biquadValue);
		channel->SetRemoteFilter(remoteFilter);
		channel->SetOutputLine(mSeqPlayer->GetOutputLine());
		channel->SetMainOutVolume(mSeqPlayer->GetMainOutVolume());
		channel->SetMainSend(mainSend);

		for (int i = 0; i < AUX_BUS_NUM; i++)
		{
			AuxBus bus = static_cast<AuxBus>(i);
			channel->SetFxSend(bus, fxSend[i]);
		}

		for (int i = 0; i < mSeqPlayer->GetVoiceOutCount(); i++)
			channel->SetVoiceOutParam(i, mSeqPlayer->GetVoiceOutParam(i));

		channel->SetLfoParam(mParserTrackParam.lfoParam);
		channel->SetLfoTarget(
			static_cast<Channel::LfoTarget>(mParserTrackParam.lfoTarget));
	}
}

void SeqTrack::FreeAllChannel()
{
	SoundThread::AutoLock lock;

	for (Channel *channel = mChannelList; channel;
	     channel = channel->GetNextTrackChannel())
	{
		Channel::FreeChannel(channel);
	}

	mChannelList = nullptr;
}

void SeqTrack::ChannelCallbackFunc(Channel *dropChannel,
                                   Channel::ChannelCallbackStatus status,
                                   register_t userData)
{
	SoundThread::AutoLock lock;

	SeqTrack *track = reinterpret_cast<SeqTrack *>(userData);

	NW4RAssertPointerNonnull_Line(586, dropChannel);
	NW4RAssertPointerNonnull_Line(587, track);

	switch (status)
	{
	case Channel::CALLBACK_STATUS_STOPPED:
	case Channel::CALLBACK_STATUS_FINISH:
		Channel::FreeChannel(dropChannel);
		break;
	}

	if (track->mSeqPlayer)
		track->mSeqPlayer->ChannelCallback(dropChannel);

	if (track->mChannelList == dropChannel)
	{
		track->mChannelList = dropChannel->GetNextTrackChannel();
		return;
	}

	Channel *channel = track->mChannelList;
	NW4RAssertPointerNonnull_Line(608, channel);

	for (; channel->GetNextTrackChannel();
	     channel = channel->GetNextTrackChannel())
	{
		if (channel->GetNextTrackChannel() == dropChannel)
		{
			channel->SetNextTrackChannel(dropChannel->GetNextTrackChannel());
			return;
		}
	}

	NW4RAssert_Line(617, false);
}

void SeqTrack::SetMute(SeqMute mute)
{
	SoundThread::AutoLock lock;

	switch (mute)
	{
	case MUTE_OFF:
		mParserTrackParam.muteFlag = false;
		break;

	case MUTE_STOP:
		StopAllChannel();
		mParserTrackParam.muteFlag = true;
		break;

	case MUTE_RELEASE:
		ReleaseAllChannel(-1);
		FreeAllChannel();
		mParserTrackParam.muteFlag = true;
		break;

	case MUTE_NO_STOP:
		mParserTrackParam.muteFlag = true;
		break;
	}
}

void SeqTrack::SetSilence(bool silenceFlag, int fadeTimes)
{
	SoundThread::AutoLock lock;

	mParserTrackParam.silenceFlag = silenceFlag;

	for (Channel *channel = mChannelList; channel;
	     channel = channel->GetNextTrackChannel())
	{
		channel->SetSilence(silenceFlag, (fadeTimes + 2) / 3);
	}
}

void SeqTrack::SetVolume(f32 volume)
{
	mExtVolume = volume;
}

void SeqTrack::SetPitch(f32 pitch)
{
	NW4RAssert_Line(668, pitch >= 0.0f);

	mExtPitch = pitch;
}

void SeqTrack::SetPan(f32 pan)
{
	mExtPan = pan;
}

void SeqTrack::SetSurroundPan(f32 surroundPan)
{
	mExtSurroundPan = surroundPan;
}

void SeqTrack::SetPanRange(f32 panRange)
{
	mPanRange = panRange;
}

void SeqTrack::SetLpfFreq(f32 lpfFreq)
{
	mParserTrackParam.lpfFreq = lpfFreq;
}

void SeqTrack::SetBiquadFilter(int type, f32 value)
{
	mParserTrackParam.biquadType	= type;
	mParserTrackParam.biquadValue	= value;
}

void SeqTrack::SetModDepth(f32 modDepth)
{
	mParserTrackParam.lfoParam.depth = modDepth;
}

void SeqTrack::SetModSpeed(f32 modSpeed)
{
	mParserTrackParam.lfoParam.speed = modSpeed;
}

// SeqTrack::GetTrackVariable? maybe both?
DECOMP_FORCE(NW4RAssertHeaderClampedLValue_String(varNo));

s16 volatile *SeqTrack::GetVariablePtr(int varNo)
{
	NW4RAssertHeaderClampedLRValue_Line(754, varNo, 0, TRACK_VARIABLE_NUM);

	if (varNo < TRACK_VARIABLE_NUM)
		return &mTrackVariable[varNo];

	return nullptr;
}

Channel *SeqTrack::NoteOn(int key, int velocity, s32 length, bool tieFlag)
{
	SoundThread::AutoLock lock;

	NW4RAssertHeaderClampedLRValue_Line(787, key, 0, 127);
	NW4RAssertHeaderClampedLRValue_Line(788, velocity, 0, 127);

	SeqPlayer::ParserPlayerParam const &playerParam =
		mSeqPlayer->GetParserPlayerParam();

	Channel *channel = nullptr;
	velocity = velocity * mParserTrackParam.velocityRange / 127;

	if (tieFlag)
	{
		channel = GetLastChannel();
		if (channel)
		{
			channel->SetKey(static_cast<u8>(key));

			f32 initVolume = velocity / 127.0f;
			channel->SetInitVolume(initVolume * initVolume);
		}
	}

	if (GetParserTrackParam().monophonicFlag)
	{
		channel = GetLastChannel();
		if (channel)
		{
			if (channel->IsRelease())
			{
				channel->Stop();
				channel = nullptr;
			}
			else
			{
				channel->SetKey(static_cast<u8>(key));

				f32 initVolume = velocity / 127.0f;
				channel->SetInitVolume(initVolume * initVolume);
				channel->SetLength(length);
			}
		}
	}

	if (!channel)
	{
		NoteOnInfo info =
		{
			mParserTrackParam.prgNo,
			key,
			velocity,
			tieFlag ? -1 : length,
			mParserTrackParam.initPan,
			playerParam.priority + GetParserTrackParam().priority,
			mSeqPlayer->GetVoiceOutCount(),
			&ChannelCallbackFunc,
			reinterpret_cast<register_t>(this)
		};

		channel = mSeqPlayer->NoteOn(mParserTrackParam.bankNo, info);
		if (!channel)
			return nullptr;

		if (channel->GetAlternateAssignId() > 0)
		{
			for (Channel *itr = mChannelList; itr;
			     itr = itr->GetNextTrackChannel())
			{
				if (itr->GetAlternateAssignId()
				    == channel->GetAlternateAssignId())
				{
					itr->Release();
				}
			}
		}

		AddChannel(channel);
	}

	if (mParserTrackParam.attack <= MAX_ENVELOPE_VALUE)
		channel->SetAttack(mParserTrackParam.attack);

	if (mParserTrackParam.decay <= MAX_ENVELOPE_VALUE)
		channel->SetDecay(mParserTrackParam.decay);

	if (mParserTrackParam.sustain <= MAX_ENVELOPE_VALUE)
		channel->SetSustain(mParserTrackParam.sustain);

	if (mParserTrackParam.release <= MAX_ENVELOPE_VALUE)
		channel->SetRelease(mParserTrackParam.release);

	if (mParserTrackParam.envHold <= MAX_ENVELOPE_VALUE)
		channel->SetHold(mParserTrackParam.envHold);

	f32 sweepPitch = mParserTrackParam.sweepPitch;
	if (mParserTrackParam.portaFlag)
		sweepPitch += mParserTrackParam.portaKey - key;

	if (!mParserTrackParam.portaTime)
	{
		NW4RCheckMessage_Line(905, length != 0, "portatime zero is invalid.");

		channel->SetSweepParam(sweepPitch, length, false);
	}
	else
	{
		int length = mParserTrackParam.portaTime;
		length *= length;
		length *= sweepPitch >= 0.0f ? sweepPitch : -sweepPitch;
		length >>= 5;
		length *= 5;

		channel->SetSweepParam(sweepPitch, length, true);
	}

	mParserTrackParam.portaKey = key;

	channel->SetSilence(mParserTrackParam.silenceFlag != false, 0);
	channel->SetReleasePriorityFix(mSeqPlayer->IsReleasePriorityFix());
	channel->SetPanMode(mSeqPlayer->GetPanMode());
	channel->SetPanCurve(mSeqPlayer->GetPanCurve());

	return channel;
}

}}} // namespace nw4r::snd::detail
