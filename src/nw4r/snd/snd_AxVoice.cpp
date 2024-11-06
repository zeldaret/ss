#include "nw4r/snd/snd_AxVoice.h"

#include "__va_arg.h"
#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_AxVoiceManager.h"
#include "nw4r/snd/snd_Common.h"
#include "nw4r/snd/snd_Util.h"
#include "nw4r/snd/snd_adpcm.h"

#include <string.h>

namespace nw4r {
namespace snd {
namespace detail {

AxVoice::AxVoice()
    : mWaveData(NULL), mFirstMixUpdateFlag(false), mReserveForFreeFlag(false), mCallback(NULL), mCallbackData(NULL) {}

AxVoice::~AxVoice() {}

void AxVoice::Setup(const void *pWave, SampleFormat fmt, int rate) {
    ut::AutoInterruptLock lock;

    mWaveData = pWave;
    mFormat = fmt;
    mSampleRate = rate;

    memset(&mMixPrev, 0, sizeof(MixParam));
    mFirstMixUpdateFlag = true;
}

bool AxVoice::IsPlayFinished() const {
    ut::AutoInterruptLock lock;

    if (mWaveData == NULL) {
        return false;
    }

    u32 dspAddr = GetCurrentPlayingDspAddress();
    const void *pBuffer = AxManager::GetInstance().GetZeroBufferAddress();
    u32 samples = GetDspAddressBySample(pBuffer, 0, mFormat);

    u32 addr = samples;

    switch (mFormat) {
        case SAMPLE_FORMAT_DSP_ADPCM: {
            addr += 0x200;
            break;
        }

        case SAMPLE_FORMAT_PCM_S8: {
            addr += 0x100;
            break;
        }

        case SAMPLE_FORMAT_PCM_S16: {
            addr += 0x80;
            break;
        }
        default: return false;
    }

    if (samples <= dspAddr && dspAddr < addr) {
        return true;
    }

    return false;
}

void AxVoice::SetLoopStart(const void *pBase, u32 samples) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    u32 addr = GetDspAddressBySample(pBase, samples, mFormat);
    mVpb.SetVoiceLoopAddr(addr);
}

void AxVoice::SetLoopEnd(const void *pBase, u32 samples) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    u32 addr = GetDspAddressBySample(pBase, samples - 1, mFormat);
    mVpb.SetVoiceEndAddr(addr);
}

void AxVoice::SetLoopFlag(bool loop) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    mVpb.SetVoiceLoop(loop);
}

void AxVoice::StopAtPoint(const void *pBase, u32 samples) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    const void *pBuffer = AxManager::GetInstance().GetZeroBufferAddress();
    u32 begin = GetDspAddressBySample(pBuffer, 0, mFormat);
    u32 end = GetDspAddressBySample(pBase, samples - 1, mFormat);

    mVpb.SetVoiceLoopAddr(begin);
    mVpb.SetVoiceEndAddr(end);
    mVpb.SetVoiceLoop(false);
}

bool AxVoice::IsDataAddressCoverd(const void *pBegin, const void *pEnd) const {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return false;
    }

    return mWaveData != NULL && (pBegin <= mWaveData && mWaveData <= pEnd);
}

u32 AxVoice::GetCurrentPlayingSample() const {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return 0;
    }

    if (mWaveData == NULL) {
        return 0;
    }

    if (IsPlayFinished()) {
        u32 end = GetLoopEndDspAddress();
        u32 samples = GetSampleByDspAddress(mWaveData, end, mFormat);
        return samples + 1;
    }

    u32 now = GetCurrentPlayingDspAddress();
    u32 samples = GetSampleByDspAddress(mWaveData, now, mFormat);
    return samples;
}

u32 AxVoice::GetCurrentPlayingDspAddress() const {
    return mVpb.GetCurrentAddress();
}

u32 AxVoice::GetLoopEndDspAddress() const {
    return mVpb.GetEndAddress();
}

void AxVoice::VoiceCallback(void *pArg) {
    ut::AutoInterruptLock lock;

    AXVPB *pVpb = static_cast<AXVPB *>(pArg);
    AxVoice *p = reinterpret_cast<AxVoice *>(pVpb->userContext);

    p->mVpb.Clear();
    AxVoiceManager::GetInstance().ReserveForFreeAxVoice(p);
}

u32 AxVoice::GetDspAddressBySample(const void *pBase, u32 samples, SampleFormat fmt) {
    if (pBase != NULL) {
        pBase = OSCachedToPhysical(pBase);
    }

    u32 addr = 0;

    switch (fmt) {
        case SAMPLE_FORMAT_DSP_ADPCM: {
            // clang-format off
        addr = (samples / AX_ADPCM_SAMPLES_PER_FRAME * AX_ADPCM_NIBBLES_PER_FRAME) +
               (samples % AX_ADPCM_SAMPLES_PER_FRAME) +
               (reinterpret_cast<u32>(pBase) * sizeof(u16)) +
               sizeof(u16);
            // clang-format on
            break;
        }

        case SAMPLE_FORMAT_PCM_S8: {
            addr = reinterpret_cast<u32>(pBase) + samples;

            break;
        }
        case SAMPLE_FORMAT_PCM_S16: {
            addr = reinterpret_cast<u32>(pBase) / sizeof(u16) + samples;
            break;
        }
    }

    return addr;
}

u32 AxVoice::GetSampleByDspAddress(const void *pBase, u32 addr, SampleFormat fmt) {
    if (pBase != NULL) {
        pBase = OSCachedToPhysical(pBase);
    }

    u32 samples = 0;

    switch (fmt) {
        case SAMPLE_FORMAT_DSP_ADPCM: {
            samples = addr - reinterpret_cast<u32>(pBase) * sizeof(u16);
            // clang-format off
        samples = (samples % AX_ADPCM_NIBBLES_PER_FRAME) +
                  (samples / AX_ADPCM_NIBBLES_PER_FRAME * AX_ADPCM_SAMPLES_PER_FRAME) -
                  sizeof(u16);
            // clang-format on
            break;
        }

        case SAMPLE_FORMAT_PCM_S8: {
            samples = addr - reinterpret_cast<u32>(pBase);
            break;
        }

        case SAMPLE_FORMAT_PCM_S16: {
            samples = addr - reinterpret_cast<u32>(pBase) / sizeof(u16);
            break;
        }
    }

    return samples;
}

u32 AxVoice::GetSampleByByte(u32 addr, SampleFormat fmt) {
    u32 samples = 0;
    u32 frac;

    switch (fmt) {
        case SAMPLE_FORMAT_PCM_S32: {
            samples = addr / AX_ADPCM_FRAME_SIZE * AX_ADPCM_SAMPLES_PER_FRAME;
            frac = addr % AX_ADPCM_FRAME_SIZE;
            if (frac != 0) {
                samples += (frac - 1) * sizeof(u16);
            }
            break;
        }

        case SAMPLE_FORMAT_PCM_S8: {
            samples = addr;
            break;
        }

        case SAMPLE_FORMAT_PCM_S16: {
            samples = addr / sizeof(u16);
            break;
        }
    }

    return samples;
}

void AxVoice::SetPriority(u32 priority) {
    mVpb.SetVoicePriority(priority);
}

void AxVoice::SetVoiceType(VoiceType type) {
    mVpb.SetVoiceType(type);
}

void AxVoice::EnableRemote(bool enable) {
    mVpb.SetVoiceRmtOn(enable);
}

void AxVoice::ResetDelta() {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    mVpb.UpdateDelta();

    AXPBMIX mix;

    mix.vL = mMixPrev.vL;
    mix.vDeltaL = 0;

    mix.vR = mMixPrev.vR;
    mix.vDeltaR = 0;

    mix.vAuxAL = mMixPrev.vAuxAL;
    mix.vDeltaAuxAL = 0;

    mix.vAuxAR = mMixPrev.vAuxAR;
    mix.vDeltaAuxAR = 0;

    mix.vAuxBL = mMixPrev.vAuxBL;
    mix.vDeltaAuxBL = 0;

    mix.vAuxBR = mMixPrev.vAuxBR;
    mix.vDeltaAuxBR = 0;

    mix.vAuxCL = mMixPrev.vAuxCL;
    mix.vDeltaAuxCL = 0;

    mix.vAuxCR = mMixPrev.vAuxCR;
    mix.vDeltaAuxCR = 0;

    mix.vS = mMixPrev.vS;
    mix.vDeltaS = 0;

    mix.vAuxAS = mMixPrev.vAuxAS;
    mix.vDeltaAuxAS = 0;

    mix.vAuxBS = mMixPrev.vAuxBS;
    mix.vDeltaAuxBS = 0;

    mix.vAuxCS = mMixPrev.vAuxCS;
    mix.vDeltaAuxCS = 0;

    mVpb.SetVoiceMix(mix, true);
}

int ConvertFormat(SampleFormat fmt) {
    switch (fmt) {
        case SAMPLE_FORMAT_DSP_ADPCM: return 0;
        case SAMPLE_FORMAT_PCM_S8:    return 25;
        case SAMPLE_FORMAT_PCM_S16:   return 10;
        default:                      return 0;
    }
}

void AxVoice::SetAddr(bool loop, const void *pWave, u32 offset, u32 loopStart, u32 loopEnd) {
    ut::AutoInterruptLock lock;

    u32 startAddr;
    u32 endAddr;
    u32 loopAddr;
    const void *pBuffer;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (offset > loopEnd) {
        pBuffer = AxManager::GetInstance().GetZeroBufferAddress();
        loop = false;

        startAddr = GetDspAddressBySample(pBuffer, 0, mFormat);
        loopAddr = GetDspAddressBySample(pBuffer, 0, mFormat);
        endAddr = GetDspAddressBySample(pBuffer, 1, mFormat);
    } else {
        if (loop) {
            loopAddr = GetDspAddressBySample(pWave, loopStart, mFormat);
        } else {
            pBuffer = AxManager::GetInstance().GetZeroBufferAddress();
            loopAddr = GetDspAddressBySample(pBuffer, 0, mFormat);
        }

        startAddr = GetDspAddressBySample(pWave, offset, mFormat);
        endAddr = GetDspAddressBySample(pWave, loopEnd - 1, mFormat);
    }

    AXPBADDR addr;

    addr.loopFlag = loop;
    addr.format = ConvertFormat(mFormat);
    addr.loopAddressHi = loopAddr >> 16 & 0xFFFF;
    addr.loopAddressLo = loopAddr & 0xFFFF;

    addr.endAddressHi = endAddr >> 16 & 0xFFFF;
    addr.endAddressLo = endAddr & 0xFFFF;

    addr.currentAddressHi = startAddr >> 16 & 0xFFFF;
    addr.currentAddressLo = startAddr & 0xFFFF;

    mVpb.SetVoiceAddr(addr);
}

void AxVoice::SetSrcType(SrcType type, f32 pitch) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (type == SRC_4TAP_AUTO) {
        f32 ratio = GetDspRatio(pitch);

        if (ratio > 4.0f / 3.0f) {
            type = SRC_4TAP_8K;
        } else if (ratio > 1.0f) {
            type = SRC_4TAP_12K;
        } else {
            type = SRC_4TAP_16K;
        }
    }

    mVpb.SetVoiceSrcType(type);
}

void AxVoice::SetAdpcm(const AdpcmParam *pParam) {
    ut::AutoInterruptLock lock;
    AXPBADPCM adpcm;

    if (!mVpb.IsAvailable()) {
        return;
    }

    switch (mFormat) {
        case SAMPLE_FORMAT_DSP_ADPCM: {
            memcpy(adpcm.a, pParam->coef, sizeof(adpcm.a));
            adpcm.gain = pParam->gain;
            adpcm.pred_scale = pParam->pred_scale;
            adpcm.yn1 = pParam->yn1;
            adpcm.yn2 = pParam->yn2;
            break;
        }

        case SAMPLE_FORMAT_PCM_S16: {
            memset(adpcm.a, 0, sizeof(adpcm.a));
            adpcm.gain = 0x800;
            adpcm.pred_scale = 0;
            adpcm.yn1 = 0;
            adpcm.yn2 = 0;
            break;
        }

        case SAMPLE_FORMAT_PCM_S8: {
            memset(adpcm.a, 0, sizeof(adpcm.a));
            adpcm.gain = 0x100;
            adpcm.pred_scale = 0;
            adpcm.yn1 = 0;
            adpcm.yn2 = 0;
            break;
        }
    }

    mVpb.SetVoiceAdpcm(adpcm);
}

void AxVoice::SetAdpcmLoop(const AdpcmLoopParam *pParam) {
    ut::AutoInterruptLock lock;
    AXPBADPCMLOOP loop;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (mFormat == SAMPLE_FORMAT_DSP_ADPCM) {
        loop.loop_pred_scale = pParam->loop_pred_scale;
        loop.loop_yn1 = pParam->loop_yn1;
        loop.loop_yn2 = pParam->loop_yn2;
    } else {
        loop.loop_pred_scale = 0;
        loop.loop_yn1 = 0;
        loop.loop_yn2 = 0;
    }

    mVpb.SetVoiceAdpcmLoop(loop);
}

bool AxVoice::SetMix(const MixParam &rParam) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return false;
    }

    if (mFirstMixUpdateFlag || !IsRun()) {
        mMixPrev = rParam;
        mFirstMixUpdateFlag = false;
    }

    bool needUpdate = false;

    if (mMixPrev.vL != rParam.vL) {
        needUpdate = true;
    }

    if (mMixPrev.vR != rParam.vR) {
        needUpdate = true;
    }

    if (mMixPrev.vS != rParam.vS) {
        needUpdate = true;
    }

    if (mMixPrev.vAuxAL != rParam.vAuxAL) {
        needUpdate = true;
    }

    if (mMixPrev.vAuxAR != rParam.vAuxAR) {
        needUpdate = true;
    }

    if (mMixPrev.vAuxAS != rParam.vAuxAS) {
        needUpdate = true;
    }

    if (mMixPrev.vAuxBL != rParam.vAuxBL) {
        needUpdate = true;
    }

    if (mMixPrev.vAuxBR != rParam.vAuxBR) {
        needUpdate = true;
    }

    if (mMixPrev.vAuxBS != rParam.vAuxBS) {
        needUpdate = true;
    }

    if (mMixPrev.vAuxCL != rParam.vAuxCL) {
        needUpdate = true;
    }

    if (mMixPrev.vAuxCR != rParam.vAuxCR) {
        needUpdate = true;
    }

    if (mMixPrev.vAuxCS != rParam.vAuxCS) {
        needUpdate = true;
    }

    AXPBMIX mix;

    mix.vL = mMixPrev.vL;
    mix.vR = mMixPrev.vR;
    mix.vS = mMixPrev.vS;

    mix.vAuxAL = mMixPrev.vAuxAL;
    mix.vAuxAR = mMixPrev.vAuxAR;
    mix.vAuxAS = mMixPrev.vAuxAS;

    mix.vAuxBL = mMixPrev.vAuxBL;
    mix.vAuxBR = mMixPrev.vAuxBR;
    mix.vAuxBS = mMixPrev.vAuxBS;

    mix.vAuxCL = mMixPrev.vAuxCL;
    mix.vAuxCR = mMixPrev.vAuxCR;
    mix.vAuxCS = mMixPrev.vAuxCS;

    int vDeltaL = CalcAxvpbDelta(mMixPrev.vL, rParam.vL);
    int vDeltaR = CalcAxvpbDelta(mMixPrev.vR, rParam.vR);
    int vDeltaS = CalcAxvpbDelta(mMixPrev.vS, rParam.vS);

    int vDeltaAuxAL = CalcAxvpbDelta(mMixPrev.vAuxAL, rParam.vAuxAL);
    int vDeltaAuxAR = CalcAxvpbDelta(mMixPrev.vAuxAR, rParam.vAuxAR);
    int vDeltaAuxAS = CalcAxvpbDelta(mMixPrev.vAuxAS, rParam.vAuxAS);

    int vDeltaAuxBL = CalcAxvpbDelta(mMixPrev.vAuxBL, rParam.vAuxBL);
    int vDeltaAuxBR = CalcAxvpbDelta(mMixPrev.vAuxBR, rParam.vAuxBR);
    int vDeltaAuxBS = CalcAxvpbDelta(mMixPrev.vAuxBS, rParam.vAuxBS);

    int vDeltaAuxCL = CalcAxvpbDelta(mMixPrev.vAuxCL, rParam.vAuxCL);
    int vDeltaAuxCR = CalcAxvpbDelta(mMixPrev.vAuxCR, rParam.vAuxCR);
    int vDeltaAuxCS = CalcAxvpbDelta(mMixPrev.vAuxCS, rParam.vAuxCS);

    mix.vDeltaL = vDeltaL;
    mix.vDeltaR = vDeltaR;
    mix.vDeltaS = vDeltaS;

    mix.vDeltaAuxAL = vDeltaAuxAL;
    mix.vDeltaAuxAR = vDeltaAuxAR;
    mix.vDeltaAuxAS = vDeltaAuxAS;

    mix.vDeltaAuxBL = vDeltaAuxBL;
    mix.vDeltaAuxBR = vDeltaAuxBR;
    mix.vDeltaAuxBS = vDeltaAuxBS;

    mix.vDeltaAuxCL = vDeltaAuxCL;
    mix.vDeltaAuxCR = vDeltaAuxCR;
    mix.vDeltaAuxCS = vDeltaAuxCS;

    mVpb.SetVoiceMix(mix, false);

    if (rParam.vL == 0 || vDeltaL == 0) {
        mMixPrev.vL = rParam.vL;
    } else {
        mMixPrev.vL += vDeltaL * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vR == 0 || vDeltaR == 0) {
        mMixPrev.vR = rParam.vR;
    } else {
        mMixPrev.vR += vDeltaR * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vS == 0 || vDeltaS == 0) {
        mMixPrev.vS = rParam.vS;
    } else {
        mMixPrev.vS += vDeltaS * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxAL == 0 || vDeltaAuxAL == 0) {
        mMixPrev.vAuxAL = rParam.vAuxAL;
    } else {
        mMixPrev.vAuxAL += vDeltaAuxAL * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxAR == 0 || vDeltaAuxAR == 0) {
        mMixPrev.vAuxAR = rParam.vAuxAR;
    } else {
        mMixPrev.vAuxAR += vDeltaAuxAR * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxAS == 0 || vDeltaAuxAS == 0) {
        mMixPrev.vAuxAS = rParam.vAuxAS;
    } else {
        mMixPrev.vAuxAS += vDeltaAuxAS * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxBL == 0 || vDeltaAuxBL == 0) {
        mMixPrev.vAuxBL = rParam.vAuxBL;
    } else {
        mMixPrev.vAuxBL += vDeltaAuxBL * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxBR == 0 || vDeltaAuxBR == 0) {
        mMixPrev.vAuxBR = rParam.vAuxBR;
    } else {
        mMixPrev.vAuxBR += vDeltaAuxBR * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxBS == 0 || vDeltaAuxBS == 0) {
        mMixPrev.vAuxBS = rParam.vAuxBS;
    } else {
        mMixPrev.vAuxBS += vDeltaAuxBS * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxCL == 0 || vDeltaAuxCL == 0) {
        mMixPrev.vAuxCL = rParam.vAuxCL;
    } else {
        mMixPrev.vAuxCL += vDeltaAuxCL * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxCR == 0 || vDeltaAuxCR == 0) {
        mMixPrev.vAuxCR = rParam.vAuxCR;
    } else {
        mMixPrev.vAuxCR += vDeltaAuxCR * AX_SAMPLES_PER_FRAME;
    }

    if (rParam.vAuxCS == 0 || vDeltaAuxCS == 0) {
        mMixPrev.vAuxCS = rParam.vAuxCS;
    } else {
        mMixPrev.vAuxCS += vDeltaAuxCS * AX_SAMPLES_PER_FRAME;
    }

    return needUpdate;
}

void AxVoice::SetRmtMix(const RemoteMixParam &rParam) {
    AXPBRMTMIX mix;

    mix.vMain0 = rParam.vMain0;
    mix.vDeltaMain0 = 0;

    mix.vAux0 = rParam.vAux0;
    mix.vDeltaAux0 = 0;

    mix.vMain1 = rParam.vMain1;
    mix.vDeltaMain1 = 0;

    mix.vAux1 = rParam.vAux1;
    mix.vDeltaAux1 = 0;

    mix.vMain2 = rParam.vMain2;
    mix.vDeltaMain2 = 0;

    mix.vAux2 = rParam.vAux2;
    mix.vDeltaAux2 = 0;

    mix.vMain3 = rParam.vMain3;
    mix.vDeltaMain3 = 0;

    mix.vAux3 = rParam.vAux3;
    mix.vDeltaAux3 = 0;

    mVpb.SetVoiceRmtMix(mix);
}

void AxVoice::SetSrc(f32 ratio, bool initial) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (initial) {
        ratio = GetDspRatio(ratio);
        u32 ratioBits = 65536.0f * ut::Clamp(ratio, 0.0f, 655365.0f);

        AXPBSRC src;

        src.ratioHi = ratioBits >> 16 & 0xFFFF;
        src.ratioLo = ratioBits & 0xFFFF;

        src.currentAddressFrac = 0;

        src.last_samples[0] = 0;
        src.last_samples[1] = 0;
        src.last_samples[2] = 0;
        src.last_samples[3] = 0;

        mVpb.SetVoiceSrc(src);
    } else {
        ratio = GetDspRatio(ratio);
        mVpb.SetVoiceSrcRatio(ratio);
    }
}

void AxVoice::SetVe(f32 volume, f32 initVolume) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    mVpb.SetVoiceVe(32767 * volume, 32767 * initVolume);
}

void AxVoice::SetLpf(u16 freq) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (freq >= 16000) {
        AXPBLPF lpf;
        lpf.on = 0;
        lpf.yn1 = 0;
        mVpb.SetVoiceLpf(lpf);
    } else if (mVpb.IsLpfEnable()) {
        u16 a0, b0;
        AXGetLpfCoefs(freq, &a0, &b0);
        mVpb.SetVoiceLpfCoefs(a0, b0);
    } else {
        AXPBLPF lpf;
        lpf.on = AX_PB_LPF_ON;
        lpf.yn1 = 0;
        AXGetLpfCoefs(freq, &lpf.a0, &lpf.b0);
        mVpb.SetVoiceLpf(lpf);
    }
}

void AxVoice::SetRemoteFilter(u8 filter) {
    ut::AutoInterruptLock lock;

    if (!mVpb.IsAvailable()) {
        return;
    }

    if (filter == 0) {
        AXPBRMTIIR iir;
        iir.lpf.on = 0;

        mVpb.SetVoiceRmtIIR(iir);
    } else if (mVpb.IsRmtIirEnable()) {
        u16 b0, b1, b2, a1, a2;
        Util::GetRemoteFilterCoefs(filter, &b0, &b1, &b2, &a1, &a2);

        mVpb.SetVoiceRmtIIRCoefs(AX_PB_BIQUAD_ON, b0, b1, b2, a1, a2);
    } else {
        AXPBRMTIIR iir;
        iir.biquad.on = AX_PB_BIQUAD_ON;
        iir.biquad.xn1 = 0;
        iir.biquad.xn2 = 0;
        iir.biquad.yn1 = 0;
        iir.biquad.yn2 = 0;

        Util::GetRemoteFilterCoefs(
            filter, &iir.biquad.b0, &iir.biquad.b1, &iir.biquad.b2, &iir.biquad.a1, &iir.biquad.a2
        );
        mVpb.SetVoiceRmtIIR(iir);
    }
}

void AxVoice::CalcOffsetAdpcmParam(
    u16 *pPredScale, u16 *pYN1, u16 *pYN2, u32 offset, const void *pData, const AdpcmParam &rParam
) {
    AXPBADPCM adpcm;
    memcpy(adpcm.a, rParam.coef, sizeof(adpcm.a));
    adpcm.gain = rParam.gain;
    adpcm.pred_scale = rParam.pred_scale;
    adpcm.yn1 = rParam.yn1;
    adpcm.yn2 = rParam.yn2;

    u32 addr = GetDspAddressBySample(pData, 0, SAMPLE_FORMAT_PCM_S32);
    u32 end = GetDspAddressBySample(pData, offset, SAMPLE_FORMAT_PCM_S32);

    while (addr < end) {
        if (addr % AX_ADPCM_NIBBLES_PER_FRAME == 0) {
            adpcm.pred_scale = *static_cast<u8 *>(OSPhysicalToCached(addr / sizeof(u16)));

            addr += sizeof(u16);
        }

        u8 byte = *static_cast<u8 *>(OSPhysicalToCached(addr / sizeof(u16)));

        u8 nibble;
        if (addr % sizeof(u16) != 0) {
            nibble = byte & 0x0F;
        } else {
            nibble = byte >> 4;
        }

        DecodeDspAdpcm(&adpcm, nibble);
        addr++;
    }

    *pPredScale = adpcm.pred_scale;
    *pYN1 = adpcm.yn1;
    *pYN2 = adpcm.yn2;
}

void AxVoiceParamBlock::Sync() {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.ve.currentVolume = mPrevVeSetting.currentVolume;

    s16 deltaIn = (mVolume - mPrevVeSetting.currentVolume) / AX_SAMPLES_PER_FRAME;
    s16 deltaOut = (deltaIn + (deltaIn > 0 ? 1 : 0) != 0) ? 1 : -1;

    int predIn = mPrevVeSetting.currentVolume + (deltaIn * AX_SAMPLES_PER_FRAME);
    int predOut = mPrevVeSetting.currentVolume + (deltaOut * AX_SAMPLES_PER_FRAME);

    if (ut::Abs(mVolume - predIn) < ut::Abs(mVolume - predOut)) {
        mVpb->pb.ve.currentDelta = deltaIn;
    } else {
        mVpb->pb.ve.currentDelta = deltaOut;
    }

    int nextVolume = mPrevVeSetting.currentVolume + (mVpb->pb.ve.currentDelta * AX_SAMPLES_PER_FRAME);

    if (nextVolume < 0) {
        mVpb->pb.ve.currentDelta = -mPrevVeSetting.currentVolume / AX_SAMPLES_PER_FRAME;
    } else if (nextVolume > 65535) {
        mVpb->pb.ve.currentDelta = (65535 - mPrevVeSetting.currentVolume) / AX_SAMPLES_PER_FRAME;
    }

    if (mVpb->pb.ve.currentDelta == 0 && mPrevVeSetting.currentDelta == 0) {
        mVpb->pb.ve.currentVolume = mVolume;
    }

    mSync &= ~AX_PBSYNC_VE_DELTA;
    mSync |= AX_PBSYNC_VE;

    mPrevVeSetting.currentVolume = mVpb->pb.ve.currentVolume;
    mPrevVeSetting.currentDelta = mVpb->pb.ve.currentDelta;

    mVpb->sync |= mSync;
    mSync = 0;
}

bool AxVoiceParamBlock::IsRmtIirEnable() const {
    return IsAvailable() && mVpb->pb.rmtIIR.biquad.on == AX_PB_BIQUAD_ON;
}

AxVoiceParamBlock::AxVoiceParamBlock()
    : mVpb(NULL), mSync(0), mPrevVeSetting(), mFirstVeUpdateFlag(false), mVolume(DEFAULT_VOLUME) {
    mPrevVeSetting.currentVolume = DEFAULT_VOLUME;
    mPrevVeSetting.currentDelta = 0;
}

void AxVoiceParamBlock::Set(AXVPB *pVpb) {
    mVpb = pVpb;
    mSync = 0;
    mFirstVeUpdateFlag = true;
    mVolume = DEFAULT_VOLUME;
    mPrevVeSetting.currentVolume = DEFAULT_VOLUME;
    mPrevVeSetting.currentDelta = 0;
}

void AxVoiceParamBlock::Clear() {
    mVpb = NULL;
    mSync = 0;
}

void AxVoiceParamBlock::SetVoiceType(u16 type) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.type = type;
    mSync |= AX_PBSYNC_TYPE;
}

void AxVoiceParamBlock::SetVoiceVe(u16 volume, u16 initVolume) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    if (mFirstVeUpdateFlag) {
        mPrevVeSetting.currentVolume = initVolume;
        mFirstVeUpdateFlag = false;
    }

    mVolume = volume;
}

void AxVoiceParamBlock::SetVoiceMix(const AXPBMIX &rMix, bool syncNow) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    const u16 *pSrc = reinterpret_cast<const u16 *>(&rMix);
    u16 *pDst = reinterpret_cast<u16 *>(&mVpb->pb.mix);

    u32 ctrl = 0;

    // vL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_L;
    }
    // vDeltaL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA | AX_MIXER_CTRL_L;
    }
    // vR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_R;
    }
    // vDeltaR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA | AX_MIXER_CTRL_R;
    }

    // vAuxAL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_AL;
    }
    // vDeltaAuxAL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_A | AX_MIXER_CTRL_AL;
    }
    // vAuxAR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_AR;
    }
    // vDeltaAuxAR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_A | AX_MIXER_CTRL_AR;
    }

    // vAuxBL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_BL;
    }
    // vDeltaAuxBL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_B | AX_MIXER_CTRL_BL;
    }
    // vAuxBR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_BR;
    }
    // vDeltaAuxBR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_B | AX_MIXER_CTRL_BR;
    }

    // vAuxCL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_CL;
    }
    // vDeltaAuxCL
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_C | AX_MIXER_CTRL_CL;
    }
    // vAuxCR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_CR;
    }
    // vDeltaAuxCR
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_C | AX_MIXER_CTRL_CR;
    }

    // vS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_S;
    }
    // vDeltaS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_S | AX_MIXER_CTRL_S;
    }

    // vAuxAS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_AS;
    }
    // vDeltaAuxAS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_AS | AX_MIXER_CTRL_AS;
    }

    // vAuxBS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_BS;
    }
    // vDeltaAuxBS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_BS | AX_MIXER_CTRL_BS;
    }

    // vAuxCS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_CS;
    }
    // vDeltaAuxCS
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_DELTA_CS | AX_MIXER_CTRL_CS;
    }

    mVpb->pb.mixerCtrl = ctrl;

    if (syncNow) {
        mVpb->sync |= AX_PBSYNC_MIX | AX_PBSYNC_MIXER_CTRL;
    } else {
        mSync |= AX_PBSYNC_MIX | AX_PBSYNC_MIXER_CTRL;
    }
}

void AxVoiceParamBlock::SetVoiceLoop(u16 loop) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.addr.loopFlag = loop;

    if (!(mVpb->sync & AX_PBSYNC_ADDR)) {
        mVpb->sync |= AX_PBSYNC_LOOP_FLAG;
    }
}

void AxVoiceParamBlock::SetVoiceLoopAddr(u32 addr) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.addr.loopAddressHi = addr >> 16 & 0xFFFF;
    mVpb->pb.addr.loopAddressLo = addr & 0xFFFF;

    if (!(mVpb->sync & AX_PBSYNC_ADDR)) {
        mVpb->sync |= AX_PBSYNC_LOOP_ADDR;
    }
}

void AxVoiceParamBlock::SetVoiceEndAddr(u32 addr) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.addr.endAddressHi = addr >> 16 & 0xFFFF;
    mVpb->pb.addr.endAddressLo = addr & 0xFFFF;

    if (!(mVpb->sync & AX_PBSYNC_ADDR)) {
        mVpb->sync |= AX_PBSYNC_END_ADDR;
    }
}

void AxVoiceParamBlock::SetVoiceAdpcm(const AXPBADPCM &rAdpcm) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    memcpy(&mVpb->pb.adpcm, &rAdpcm, sizeof(AXPBADPCM));
    mSync |= AX_PBSYNC_ADPCM;
}

void AxVoiceParamBlock::SetVoiceSrcType(u32 type) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    switch (type) {
        case AxVoice::SRC_NONE: {
            mVpb->pb.srcSelect = 2;
            break;
        }

        case AxVoice::SRC_LINEAR: {
            mVpb->pb.srcSelect = 1;
            break;
        }

        case AxVoice::SRC_4TAP_8K: {
            mVpb->pb.srcSelect = 0;
            mVpb->pb.coefSelect = 0;
            break;
        }

        case AxVoice::SRC_4TAP_12K: {
            mVpb->pb.srcSelect = 0;
            mVpb->pb.coefSelect = 1;
            break;
        }

        case AxVoice::SRC_4TAP_16K: {
            mVpb->pb.srcSelect = 0;
            mVpb->pb.coefSelect = 2;
            break;
        }
    }

    mSync |= AX_PBSYNC_SELECT;
}

void AxVoiceParamBlock::SetVoiceSrc(const AXPBSRC &rSrc) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    memcpy(&mVpb->pb.src, &rSrc, sizeof(AXPBSRC));
    mSync &= ~AX_PBSYNC_SRC_RATIO;
    mSync |= AX_PBSYNC_SRC;
}

void AxVoiceParamBlock::SetVoiceSrcRatio(f32 ratio) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    u32 ratioU32 = 65536 * ratio;
    mVpb->pb.src.ratioHi = ratioU32 >> 16 & 0xFFFF;
    mVpb->pb.src.ratioLo = ratioU32 & 0xFFFF;

    if (!(mSync & AX_PBSYNC_SRC)) {
        mSync |= AX_PBSYNC_SRC_RATIO;
    }
}

void AxVoiceParamBlock::SetVoiceAdpcmLoop(const AXPBADPCMLOOP &rLoop) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    memcpy(&mVpb->pb.adpcmLoop, &rLoop, sizeof(AXPBADPCMLOOP));
    mSync |= AX_PBSYNC_ADPCM_LOOP;
}

void AxVoiceParamBlock::SetVoiceLpf(const AXPBLPF &rLpf) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    memcpy(&mVpb->pb.lpf, &rLpf, sizeof(AXPBLPF));
    mSync |= AX_PBSYNC_LPF;
}

void AxVoiceParamBlock::SetVoiceLpfCoefs(u16 a0, u16 b0) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.lpf.a0 = a0;
    mVpb->pb.lpf.b0 = b0;
    mSync |= AX_PBSYNC_LPF_COEFS;
}

void AxVoiceParamBlock::SetVoiceRmtOn(u16 on) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mVpb->pb.remote = on;
    mSync |= AX_PBSYNC_REMOTE;
}

void AxVoiceParamBlock::SetVoiceRmtMix(const AXPBRMTMIX &rMix) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    u16 *pDst = reinterpret_cast<u16 *>(&mVpb->pb.rmtMix);
    const u16 *pSrc = reinterpret_cast<const u16 *>(&rMix);

    u16 ctrl = 0;

    // vMain0
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_M0;
    }
    // vDeltaMain0
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_M0;
    }
    // vAux0
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_A0;
    }
    // vDeltaAux0
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_A0;
    }

    // vMain1
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_M1;
    }
    // vDeltaMain1
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_M1;
    }
    // vAux1
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_A1;
    }
    // vDeltaAux1
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_A1;
    }

    // vMain2
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_M2;
    }
    // vDeltaMain2
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_M2;
    }
    // vAux2
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_A2;
    }
    // vDeltaAux2
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_A2;
    }

    // vMain3
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_M3;
    }
    // vDeltaMain3
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_M3;
    }
    // vAux3
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_A3;
    }
    // vDeltaAux3
    if ((*pDst++ = *pSrc++) != 0) {
        ctrl |= AX_MIXER_CTRL_RMT_DELTA_A3;
    }

    mVpb->pb.rmtMixerCtrl = ctrl;
    mSync |= AX_PBSYNC_RMT_MIXER_CTRL | AX_PBSYNC_RMTMIX;
}

void AxVoiceParamBlock::SetVoiceRmtIIR(const AXPBRMTIIR &rIir) {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    memcpy(&mVpb->pb.rmtIIR, &rIir, sizeof(AXPBRMTIIR));
    mSync |= AX_PBSYNC_RMTIIR;
}

void AxVoiceParamBlock::SetVoiceRmtIIRCoefs(u16 type, ...) {
    ut::AutoInterruptLock lock;

    int i;
    int argc;

    if (!IsAvailable()) {
        return;
    }

    if (type == AX_PB_LPF_ON) {
        argc = 2;
    } else if (type == AX_PB_BIQUAD_ON) {
        argc = 5;
    } else {
        return;
    }

    va_list argv;
    u16 coefs[5];

    va_start(argv, type);
    for (i = 0; i < argc; i++) {
        coefs[i] = va_arg(argv, s32);
    }
    va_end(argv);

    if (type == AX_PB_LPF_ON) {
        mVpb->pb.rmtIIR.lpf.a0 = coefs[0];
        mVpb->pb.rmtIIR.lpf.b0 = coefs[1];
        mSync |= AX_PBSYNC_RMTIIR_LPF_COEFS;
    } else {
        mVpb->pb.rmtIIR.biquad.b0 = coefs[0];
        mVpb->pb.rmtIIR.biquad.b1 = coefs[1];
        mVpb->pb.rmtIIR.biquad.b2 = coefs[2];
        mVpb->pb.rmtIIR.biquad.a1 = coefs[3];
        mVpb->pb.rmtIIR.biquad.a2 = coefs[4];
        mSync |= AX_PBSYNC_RMTIIR_BIQUAD_COEFS;
    }
}

void AxVoiceParamBlock::UpdateDelta() {
    ut::AutoInterruptLock lock;

    if (!IsAvailable()) {
        return;
    }

    mPrevVeSetting.currentVolume += mVpb->pb.ve.currentDelta * AX_SAMPLES_PER_FRAME;

    mVpb->pb.ve.currentVolume = mPrevVeSetting.currentVolume;
    mVpb->pb.ve.currentDelta = 0;

    mVpb->sync |= AX_PBSYNC_VE;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
