#include "nw4r/snd/snd_Bank.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_Bank.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_BankFile.h" // InstInfo
#include "nw4r/snd/snd_Channel.h"
#include "nw4r/snd/snd_NoteOnCallback.h" // NoteOnInfo
#include "nw4r/snd/snd_WaveFile.h" // WaveInfo

#include "nw4r/ut/ut_algorithm.h" // ut::Min

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

Bank::Bank(void const *bankData) :
	mBankReader			(bankData),
	mWaveDataAddress	(nullptr)
{
}

Bank::~Bank() {}

Channel *Bank::NoteOn(NoteOnInfo const &noteOnInfo) const
{
	bool result;

	InstInfo instInfo;
	result = mBankReader.ReadInstInfo(&instInfo, noteOnInfo.prgNo,
	                                  noteOnInfo.key, noteOnInfo.velocity);
	if (!result)
		return nullptr;

	WaveInfo waveParam;
	WaveInfo const *waveInfoAddress;
	result = mBankReader.ReadWaveInfo(&waveParam, instInfo.waveDataLocation,
	                                  mWaveDataAddress, &waveInfoAddress);
	if (!result)
		return nullptr;

	int voiceChannelCount =
		ut::Min(waveParam.numChannels, Channel::CHANNEL_MAX);
	Channel *ch_p = Channel::AllocChannel(voiceChannelCount,
		noteOnInfo.voiceOutCount, noteOnInfo.priority,
		noteOnInfo.channelCallback, noteOnInfo.channelCallbackData);
	if (!ch_p)
		return nullptr;

	ch_p->SetKey(noteOnInfo.key);
	ch_p->SetOriginalKey(instInfo.originalKey);

	f32 initVolume = noteOnInfo.velocity / 127.0f;
	initVolume *= initVolume;
	initVolume *= instInfo.volume / 127.0f;
	ch_p->SetInitVolume(initVolume);

	ch_p->SetTune(instInfo.tune);
	ch_p->SetAttack(instInfo.attack);
	ch_p->SetHold(instInfo.hold);
	ch_p->SetDecay(instInfo.decay);
	ch_p->SetSustain(instInfo.sustain);
	ch_p->SetRelease(instInfo.release);

	f32 initPan = (instInfo.pan - 64) / 63.0f;
	initPan += noteOnInfo.initPan / 63.0f;
	ch_p->SetInitPan(initPan);

	ch_p->SetInitSurroundPan(0.0f);
	ch_p->SetAlternateAssignId(instInfo.alternateAssign);
	ch_p->SetReleaseIgnore(instInfo.noteOffType
	                       == InstInfo::NOTE_OFF_TYPE_IGNORE);

	if (instInfo.waveDataLocation.type
	    == InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_CALLBACK)
	{
		ch_p->SetWaveDataLocationCallback(instInfo.waveDataLocation.callback,
		                                  waveInfoAddress);
	}

	ch_p->Start(waveParam, noteOnInfo.length, 0);

	return ch_p;
}

}}} // namespace nw4r::snd::detail
