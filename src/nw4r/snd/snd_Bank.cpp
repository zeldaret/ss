#include "nw4r/snd/snd_Bank.h"

namespace nw4r {
namespace snd {
namespace detail {

Bank::Bank(const void* pBankBin)
    : mBankReader(pBankBin), mWaveDataAddress(NULL) {}

Bank::~Bank() {}

Channel* Bank::NoteOn(const NoteOnInfo& rInfo) const {
    InstInfo instInfo;
    if (!mBankReader.ReadInstInfo(&instInfo, rInfo.prgNo, rInfo.key,
                                  rInfo.velocity)) {
        return NULL;
    }

    WaveInfo waveData;
    if (!mBankReader.ReadWaveParam(&waveData, instInfo.waveIndex,
                                   mWaveDataAddress)) {
        return NULL;
    }

    Channel* pChannel = Channel::AllocChannel(
        ut::Min<int>(waveData.numChannels, CHANNEL_MAX), rInfo.voiceOutCount,
        rInfo.priority, rInfo.channelCallback, rInfo.channelCallbackData);

    if (pChannel == NULL) {
        return NULL;
    }

    pChannel->SetKey(rInfo.key);
    pChannel->SetOriginalKey(instInfo.originalKey);

    f32 initVolume = rInfo.velocity / 127.0f;
    initVolume *= initVolume;
    initVolume *= instInfo.volume / 127.0f;
    pChannel->SetInitVolume(initVolume);

    pChannel->SetTune(instInfo.tune);
    pChannel->SetAttack(instInfo.attack);
    pChannel->SetHold(instInfo.hold);
    pChannel->SetDecay(instInfo.decay);
    pChannel->SetSustain(instInfo.sustain);
    pChannel->SetRelease(instInfo.release);

    f32 initPan = (instInfo.pan - 64) / 63.0f;
    initPan += rInfo.initPan / 63.0f;
    pChannel->SetInitPan(initPan);

    pChannel->SetInitSurroundPan(0.0f);
    pChannel->Start(waveData, rInfo.length, 0);

    return pChannel;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
