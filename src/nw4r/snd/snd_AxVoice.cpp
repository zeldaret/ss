#include "nw4r/snd/snd_AxVoice.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_AxVoice.cpp
 */

/*******************************************************************************
 * headers
 */

#include <cstdarg>
#include <cstring>

#include "common.h"

#include "nw4r/snd/snd_adpcm.h" // DecodeDspAdpcm
#include "nw4r/snd/snd_AxManager.h"
#include "nw4r/snd/snd_AxVoiceManager.h"
#include "nw4r/snd/snd_BiquadFilterCallback.h"
#include "nw4r/snd/snd_global.h"
#include "nw4r/snd/snd_Util.h" // Util::GetRemoteFilterCoefs

#include "nw4r/ut/ut_algorithm.h"
#include "nw4r/ut/ut_Lock.h" // ut::AutoInterruptLock

#include <rvl/AX/AX.h>
#include <rvl/AX/AXCL.h>
#include <rvl/AX/AXVPB.h>
#include <rvl/OS/OSAddress.h>

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * local function declarations
 */

namespace nw4r { namespace snd { namespace detail
{
	inline int CalcAxvpbDelta(u16 initValue, u16 targetValue)
	{
		if (initValue == targetValue)
			return 0;

		return (targetValue - initValue) / AX_SAMPLES_PER_FRAME;
	}
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

AxVoice::AxVoice() :
	mWaveData			(nullptr),
	mFirstMixUpdateFlag	(false),
	mReserveForFreeFlag	(false),
	mCallback			(nullptr),
	mCallbackData		(nullptr)
{
}

AxVoice::~AxVoice() {}

void AxVoice::Setup(void const *waveAddr, SampleFormat format, int sampleRate)
{
	ut::AutoInterruptLock lock;

	mWaveData = waveAddr;
	mFormat = format;
	mSampleRate = sampleRate;

	std::memset(&mMixPrev, 0, sizeof mMixPrev);
	mFirstMixUpdateFlag = true;
}

bool AxVoice::IsPlayFinished() const
{
	ut::AutoInterruptLock lock;

	if (!mWaveData)
		return false;

	u32 dspAddr = GetCurrentPlayingDspAddress();
	void const *zeroBuffer = AxManager::GetInstance().GetZeroBufferAddress();

	u32 beginPos = GetDspAddressBySample(zeroBuffer, 0, mFormat);
	u32 endPos = beginPos;

	switch (mFormat)
	{
	case SAMPLE_FORMAT_DSP_ADPCM:
		endPos += 0x200;
		break;

	case SAMPLE_FORMAT_PCM_S8:
		endPos += 0x100;
		break;

	case SAMPLE_FORMAT_PCM_S16:
		endPos += 0x80;
		break;

	default:
		NW4RPanicMessage_Line(103, "Invalid format\n");
		return false;
	}

	if (beginPos <= dspAddr && dspAddr < endPos)
		return true;
	else
		return false;
}

void AxVoice::SetLoopStart(void const *baseAddress, u32 samples)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	u32 dspAddress = GetDspAddressBySample(baseAddress, samples, mFormat);
	mVpb.SetVoiceLoopAddr(dspAddress);
}

void AxVoice::SetLoopEnd(void const *baseAddress, u32 samples)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	u32 dspAddress = GetDspAddressBySample(baseAddress, samples - 1, mFormat);
	mVpb.SetVoiceEndAddr(dspAddress);
}

void AxVoice::SetLoopFlag(bool loopFlag)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	mVpb.SetVoiceLoop(loopFlag);
}

void AxVoice::StopAtPoint(void const *baseAddress, u32 samples)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	void const *zeroBuffer = AxManager::GetInstance().GetZeroBufferAddress();
	u32 beginPos = GetDspAddressBySample(zeroBuffer, 0, mFormat);
	u32 endPos = GetDspAddressBySample(baseAddress, samples - 1, mFormat);

	mVpb.SetVoiceLoopAddr(beginPos);
	mVpb.SetVoiceEndAddr(endPos);
	mVpb.SetVoiceLoop(false);
}

bool AxVoice::IsDataAddressCoverd(void const *beginAddress,
                                  void const *endAddress) const
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return false;

	// NOTE: parentheses necessary, do not remove
	return mWaveData && (beginAddress <= mWaveData && mWaveData <= endAddress);
}

u32 AxVoice::GetCurrentPlayingSample() const
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return 0;

	if (!mWaveData)
		return 0;

	if (IsPlayFinished())
	{
		u32 samples =
			GetSampleByDspAddress(mWaveData, GetLoopEndDspAddress(), mFormat);

		return samples + 1;
	}
	else
	{
		u32 samples = GetSampleByDspAddress(
			mWaveData, GetCurrentPlayingDspAddress(), mFormat);

		return samples;
	}
}

u32 AxVoice::GetCurrentPlayingDspAddress() const
{
	u32 dspAddr = mVpb.GetCurrentAddress();

	return dspAddr;
}

u32 AxVoice::GetLoopEndDspAddress() const
{
	u32 dspAddr = mVpb.GetEndAddress();

	return dspAddr;
}

void AxVoice::VoiceCallback(void *callbackData)
{
	ut::AutoInterruptLock lock;

	AXVPB *dropVpb = static_cast<AXVPB *>(callbackData);
	AxVoice *voice = reinterpret_cast<AxVoice *>(dropVpb->userContext);
	NW4RAssertPointerNonnull_Line(276, voice);

	voice->mVpb.Clear();
	AxVoiceManager::GetInstance().ReserveForFreeAxVoice(voice);
}

u32 AxVoice::GetDspAddressBySample(void const *baseAddress, u32 samples,
                                   SampleFormat format)
{
	if (baseAddress)
		baseAddress = OSCachedToPhysical(const_cast<void *>(baseAddress));

	u32 addr = 0;

	switch (format)
	{
	case SAMPLE_FORMAT_DSP_ADPCM:
		addr =
			(samples / AX_ADPCM_SAMPLES_PER_FRAME * AX_ADPCM_NIBBLES_PER_FRAME)
			+ (samples % AX_ADPCM_SAMPLES_PER_FRAME)
			+ (reinterpret_cast<u32>(baseAddress) * sizeof(u16)) + sizeof(u16);
		break;

	case SAMPLE_FORMAT_PCM_S8:
		addr = reinterpret_cast<u32>(baseAddress) + samples;
		break;

	case SAMPLE_FORMAT_PCM_S16:
		addr = reinterpret_cast<u32>(baseAddress) / sizeof(u16) + samples;
		break;

	default:
		NW4RPanicMessage_Line(318, "Invalid format\n");
		break;
	}

	return addr;
}

u32 AxVoice::GetSampleByDspAddress(void const *baseAddress, u32 addr,
                                   SampleFormat format)
{
	if (baseAddress)
		baseAddress = OSCachedToPhysical(const_cast<void *>(baseAddress));

	u32 samples = 0;

	switch (format)
	{
	case SAMPLE_FORMAT_DSP_ADPCM:
		samples = addr - reinterpret_cast<u32>(baseAddress) * sizeof(u16);
		samples = (samples % AX_ADPCM_NIBBLES_PER_FRAME)
		        + (samples / AX_ADPCM_NIBBLES_PER_FRAME
		           * AX_ADPCM_SAMPLES_PER_FRAME)
		        - sizeof(u16);
		break;

	case SAMPLE_FORMAT_PCM_S8:
		samples = addr - reinterpret_cast<u32>(baseAddress);
		break;

	case SAMPLE_FORMAT_PCM_S16:
		samples = addr - reinterpret_cast<u32>(baseAddress) / sizeof(u16);
		break;

	default:
		NW4RPanicMessage_Line(350, "Invalid format\n");
		break;
	}

	return samples;
}

void AxVoice::SetPriority(u32 priority)
{
	mVpb.SetVoicePriority(priority);
}

void AxVoice::SetVoiceType(VoiceType type)
{
	mVpb.SetVoiceType(type);
}

void AxVoice::ResetDelta()
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	mVpb.UpdateDelta();

	AXPBMIX mix;
	mix.vL			= mMixPrev.vL;
	mix.vDeltaL		= 0;
	mix.vR			= mMixPrev.vR;
	mix.vDeltaR		= 0;
	mix.vAuxAL		= mMixPrev.vAuxAL;
	mix.vDeltaAuxAL	= 0;
	mix.vAuxAR		= mMixPrev.vAuxAR;
	mix.vDeltaAuxAR	= 0;
	mix.vAuxBL		= mMixPrev.vAuxBL;
	mix.vDeltaAuxBL	= 0;
	mix.vAuxBR		= mMixPrev.vAuxBR;
	mix.vDeltaAuxBR	= 0;
	mix.vAuxCL		= mMixPrev.vAuxCL;
	mix.vDeltaAuxCL	= 0;
	mix.vAuxCR		= mMixPrev.vAuxCR;
	mix.vDeltaAuxCR	= 0;
	mix.vS			= mMixPrev.vS;
	mix.vDeltaS		= 0;
	mix.vAuxAS		= mMixPrev.vAuxAS;
	mix.vDeltaAuxAS	= 0;
	mix.vAuxBS		= mMixPrev.vAuxBS;
	mix.vDeltaAuxBS	= 0;
	mix.vAuxCS		= mMixPrev.vAuxCS;
	mix.vDeltaAuxCS	= 0;

	mVpb.SetVoiceMix(mix, true);
}

void AxVoice::SetAddr(bool loopFlag, void const *waveAddr, u32 startOffset,
                      u32 loopStart, u32 loopEnd)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	u32 startPos;
	u32 endPos;
	u32 loopPos;

	if (startOffset > loopEnd)
	{
		void const *zeroBuffer =
			AxManager::GetInstance().GetZeroBufferAddress();
		loopFlag = false;

		startPos = GetDspAddressBySample(zeroBuffer, 0, mFormat);
		loopPos = GetDspAddressBySample(zeroBuffer, 0, mFormat);
		endPos = GetDspAddressBySample(zeroBuffer, 1, mFormat);
	}
	else
	{
		if (loopFlag)
		{
			loopPos = GetDspAddressBySample(waveAddr, loopStart, mFormat);
		}
		else
		{
			void const *zeroBuffer =
				AxManager::GetInstance().GetZeroBufferAddress();

			loopPos = GetDspAddressBySample(zeroBuffer, 0, mFormat);
		}

		startPos = GetDspAddressBySample(waveAddr, startOffset, mFormat);
		endPos = GetDspAddressBySample(waveAddr, loopEnd - 1, mFormat);
	}

	AXPBADDR addr;
	addr.loopFlag			= loopFlag;
	addr.format				= GetAxFormatFromSampleFormat(mFormat);
	addr.loopAddressHi		= loopPos >> 16;
	addr.loopAddressLo		= loopPos & 0xffff;
	addr.endAddressHi		= endPos >> 16;
	addr.endAddressLo		= endPos & 0xffff;
	addr.currentAddressHi	= startPos >> 16;
	addr.currentAddressLo	= startPos & 0xffff;

	mVpb.SetVoiceAddr(addr);
}

u16 AxVoice::GetAxFormatFromSampleFormat(SampleFormat sampleFormat)
{
	switch (sampleFormat)
	{
	case SAMPLE_FORMAT_DSP_ADPCM:
		return 0;

	case SAMPLE_FORMAT_PCM_S8:
		return 25;

	case SAMPLE_FORMAT_PCM_S16:
		return 10;

	default:
		NW4RPanicMessage_Line(503, "Invalid format\n");
		return 0;
	}
}

void AxVoice::SetSrcType(SrcType type, f32 pitch)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	if (type == SRC_4TAP_AUTO)
	{
		f32 ratio = GetDspRatio(pitch);

		if (ratio > 4.0f / 3.0f)
			type = SRC_4TAP_8K;
		else if (ratio > 1.0f)
			type = SRC_4TAP_12K;
		else
			type = SRC_4TAP_16K;
	}

	mVpb.SetVoiceSrcType(type);
}

void AxVoice::SetAdpcm(AdpcmParam const *param)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	AXPBADPCM adpcm;

	switch (mFormat)
	{
	case SAMPLE_FORMAT_DSP_ADPCM:
		NW4RAssertPointerNonnull_Line(533, param);

		std::memcpy(adpcm.a, param->coef, sizeof adpcm.a);

		adpcm.gain			= param->gain;
		adpcm.pred_scale	= param->pred_scale;
		adpcm.yn1			= param->yn1;
		adpcm.yn2			= param->yn2;

		break;

	case SAMPLE_FORMAT_PCM_S16:
		std::memset(adpcm.a, 0, sizeof adpcm.a);

		adpcm.gain			= 0x800;
		adpcm.pred_scale	= 0;
		adpcm.yn1			= 0;
		adpcm.yn2			= 0;

		break;

	case SAMPLE_FORMAT_PCM_S8:
		std::memset(adpcm.a, 0, sizeof adpcm.a);

		adpcm.gain			= 0x100;
		adpcm.pred_scale	= 0;
		adpcm.yn1			= 0;
		adpcm.yn2			= 0;

		break;

	default:
		NW4RPanicMessage_Line(555, "Invalid format\n");
		break;
	}

	mVpb.SetVoiceAdpcm(adpcm);
}

bool AxVoice::IsNeedNextUpdate(MixParam const &param) const
{
	if (mMixPrev.vL != param.vL)
		return true;

	if (mMixPrev.vR != param.vR)
		return true;

	if (mMixPrev.vS != param.vS)
		return true;

	if (mMixPrev.vAuxAL != param.vAuxAL)
		return true;

	if (mMixPrev.vAuxAR != param.vAuxAR)
		return true;

	if (mMixPrev.vAuxAS != param.vAuxAS)
		return true;

	if (mMixPrev.vAuxBL != param.vAuxBL)
		return true;

	if (mMixPrev.vAuxBR != param.vAuxBR)
		return true;

	if (mMixPrev.vAuxBS != param.vAuxBS)
		return true;

	if (mMixPrev.vAuxCL != param.vAuxCL)
		return true;

	if (mMixPrev.vAuxCR != param.vAuxCR)
		return true;

	if (mMixPrev.vAuxCS != param.vAuxCS)
		return true;

	return false;
}

void AxVoice::SetAdpcmLoop(AdpcmLoopParam const *param)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	AXPBADPCMLOOP adpcmloop;

	if (mFormat == SAMPLE_FORMAT_DSP_ADPCM)
	{
		NW4RAssertPointerNonnull_Line(587, param);

		adpcmloop.loop_pred_scale	= param->loop_pred_scale;
		adpcmloop.loop_yn1			= param->loop_yn1;
		adpcmloop.loop_yn2			= param->loop_yn2;
	}
	else
	{
		adpcmloop.loop_pred_scale	= 0;
		adpcmloop.loop_yn1			= 0;
		adpcmloop.loop_yn2			= 0;
	}

	mVpb.SetVoiceAdpcmLoop(adpcmloop);
}

bool AxVoice::SetMix(MixParam const &param)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return false;

	AXPBMIX mix;

	if (mFirstMixUpdateFlag || !IsRun())
	{
		mMixPrev = param;
		mFirstMixUpdateFlag = false;
	}

	bool needUpdateFlag = IsNeedNextUpdate(param);

	mix.vL			= mMixPrev.vL;
	mix.vR			= mMixPrev.vR;
	mix.vS			= mMixPrev.vS;
	mix.vAuxAL		= mMixPrev.vAuxAL;
	mix.vAuxAR		= mMixPrev.vAuxAR;
	mix.vAuxAS		= mMixPrev.vAuxAS;
	mix.vAuxBL		= mMixPrev.vAuxBL;
	mix.vAuxBR		= mMixPrev.vAuxBR;
	mix.vAuxBS		= mMixPrev.vAuxBS;
	mix.vAuxCL		= mMixPrev.vAuxCL;
	mix.vAuxCR		= mMixPrev.vAuxCR;
	mix.vAuxCS		= mMixPrev.vAuxCS;

	int vDeltaL		= CalcAxvpbDelta(mMixPrev.vL, param.vL);
	int vDeltaR		= CalcAxvpbDelta(mMixPrev.vR, param.vR);
	int vDeltaS		= CalcAxvpbDelta(mMixPrev.vS, param.vS);
	int vDeltaAuxAL	= CalcAxvpbDelta(mMixPrev.vAuxAL, param.vAuxAL);
	int vDeltaAuxAR	= CalcAxvpbDelta(mMixPrev.vAuxAR, param.vAuxAR);
	int vDeltaAuxAS	= CalcAxvpbDelta(mMixPrev.vAuxAS, param.vAuxAS);
	int vDeltaAuxBL	= CalcAxvpbDelta(mMixPrev.vAuxBL, param.vAuxBL);
	int vDeltaAuxBR	= CalcAxvpbDelta(mMixPrev.vAuxBR, param.vAuxBR);
	int vDeltaAuxBS	= CalcAxvpbDelta(mMixPrev.vAuxBS, param.vAuxBS);
	int vDeltaAuxCL	= CalcAxvpbDelta(mMixPrev.vAuxCL, param.vAuxCL);
	int vDeltaAuxCR	= CalcAxvpbDelta(mMixPrev.vAuxCR, param.vAuxCR);
	int vDeltaAuxCS	= CalcAxvpbDelta(mMixPrev.vAuxCS, param.vAuxCS);

	mix.vDeltaL		= vDeltaL;
	mix.vDeltaR		= vDeltaR;
	mix.vDeltaS		= vDeltaS;
	mix.vDeltaAuxAL	= vDeltaAuxAL;
	mix.vDeltaAuxAR	= vDeltaAuxAR;
	mix.vDeltaAuxAS	= vDeltaAuxAS;
	mix.vDeltaAuxBL	= vDeltaAuxBL;
	mix.vDeltaAuxBR	= vDeltaAuxBR;
	mix.vDeltaAuxBS	= vDeltaAuxBS;
	mix.vDeltaAuxCL	= vDeltaAuxCL;
	mix.vDeltaAuxCR	= vDeltaAuxCR;
	mix.vDeltaAuxCS	= vDeltaAuxCS;

	mVpb.SetVoiceMix(mix, false);

	if (param.vL == 0 || vDeltaL == 0)
		mMixPrev.vL = param.vL;
	else
		mMixPrev.vL += vDeltaL * AX_SAMPLES_PER_FRAME;

	if (param.vR == 0 || vDeltaR == 0)
		mMixPrev.vR = param.vR;
	else
		mMixPrev.vR += vDeltaR * AX_SAMPLES_PER_FRAME;

	if (param.vS == 0 || vDeltaS == 0)
		mMixPrev.vS = param.vS;
	else
		mMixPrev.vS += vDeltaS * AX_SAMPLES_PER_FRAME;

	if (param.vAuxAL == 0 || vDeltaAuxAL == 0)
		mMixPrev.vAuxAL = param.vAuxAL;
	else
		mMixPrev.vAuxAL += vDeltaAuxAL * AX_SAMPLES_PER_FRAME;

	if (param.vAuxAR == 0 || vDeltaAuxAR == 0)
		mMixPrev.vAuxAR = param.vAuxAR;
	else
		mMixPrev.vAuxAR += vDeltaAuxAR * AX_SAMPLES_PER_FRAME;

	if (param.vAuxAS == 0 || vDeltaAuxAS == 0)
		mMixPrev.vAuxAS = param.vAuxAS;
	else
		mMixPrev.vAuxAS += vDeltaAuxAS * AX_SAMPLES_PER_FRAME;

	if (param.vAuxBL == 0 || vDeltaAuxBL == 0)
		mMixPrev.vAuxBL = param.vAuxBL;
	else
		mMixPrev.vAuxBL += vDeltaAuxBL * AX_SAMPLES_PER_FRAME;

	if (param.vAuxBR == 0 || vDeltaAuxBR == 0)
		mMixPrev.vAuxBR = param.vAuxBR;
	else
		mMixPrev.vAuxBR += vDeltaAuxBR * AX_SAMPLES_PER_FRAME;

	if (param.vAuxBS == 0 || vDeltaAuxBS == 0)
		mMixPrev.vAuxBS = param.vAuxBS;
	else
		mMixPrev.vAuxBS += vDeltaAuxBS * AX_SAMPLES_PER_FRAME;

	if (param.vAuxCL == 0 || vDeltaAuxCL == 0)
		mMixPrev.vAuxCL = param.vAuxCL;
	else
		mMixPrev.vAuxCL += vDeltaAuxCL * AX_SAMPLES_PER_FRAME;

	if (param.vAuxCR == 0 || vDeltaAuxCR == 0)
		mMixPrev.vAuxCR = param.vAuxCR;
	else
		mMixPrev.vAuxCR += vDeltaAuxCR * AX_SAMPLES_PER_FRAME;

	if (param.vAuxCS == 0 || vDeltaAuxCS == 0)
		mMixPrev.vAuxCS = param.vAuxCS;
	else
		mMixPrev.vAuxCS += vDeltaAuxCS * AX_SAMPLES_PER_FRAME;

	return needUpdateFlag;
}

void AxVoice::SetSrc(f32 ratio, bool initialUpdate)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	if (initialUpdate)
	{
		ratio = ut::Clamp(GetDspRatio(ratio), 0.0f, 65535.0f);

		u32 srcBits = 65536 * ratio;

		AXPBSRC src;
		src.ratioHi				= srcBits >> 16;
		src.ratioLo				= srcBits & 0xffff;
		src.currentAddressFrac	= 0;
		src.last_samples[0]		= 0;
		src.last_samples[1]		= 0;
		src.last_samples[2]		= 0;
		src.last_samples[3]		= 0;

		mVpb.SetVoiceSrc(src);
	}
	else
	{
		ratio = GetDspRatio(ratio);

		mVpb.SetVoiceSrcRatio(ratio);
	}
}

void AxVoice::SetVe(f32 volume, f32 initVolume)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	mVpb.SetVoiceVe(volume * (AX_MAX_VOLUME - 1),
	                initVolume * (AX_MAX_VOLUME - 1));
}

void AxVoice::SetLpf(u16 freq)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	if (freq >= 16000)
	{
		AXPBLPF lpf;
		lpf.on	= AX_PB_OFF;
		lpf.yn1	= 0;

		mVpb.SetVoiceLpf(lpf);
	}
	else if (mVpb.IsLpfEnable())
	{
		u16 a0, b0;
		AXGetLpfCoefs(freq, &a0, &b0);

		mVpb.SetVoiceLpfCoefs(a0, b0);
	}
	else
	{
		AXPBLPF lpf;
		lpf.on	= AX_PB_LPF_ON;
		lpf.yn1	= 0;
		AXGetLpfCoefs(freq, &lpf.a0, &lpf.b0);

		mVpb.SetVoiceLpf(lpf);
	}
}

void AxVoice::SetBiquad(u8 filterType, f32 value)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	BiquadFilterCallback const *biquadFilter =
		AxManager::GetInstance().GetBiquadFilterCallback(filterType);

	bool filterEnable = true;

	if (filterType == 0)
		filterEnable = false;

	if (!biquadFilter)
		filterEnable = false;

	if (!filterEnable)
	{
		AXPBBIQUAD biquad;
		biquad.on	= AX_PB_OFF;
		biquad.xn1	= 0;
		biquad.xn2	= 0;
		biquad.yn1	= 0;
		biquad.yn2	= 0;

		mVpb.SetVoiceBiquad(biquad);
	}
	else
	{
		BiquadFilterCallback::BiquadCoef coef;
		biquadFilter->GetCoef(filterType, value, &coef);

		if (mVpb.IsBiquadEnable())
		{
			mVpb.SetVoiceBiquadCoefs(coef.b0, coef.b1, coef.b2, coef.a1,
			                         coef.a2);
		}
		else
		{
			AXPBBIQUAD biquad;
			biquad.on	= AX_PB_BIQUAD_ON;
			biquad.xn1	= 0;
			biquad.xn2	= 0;
			biquad.yn1	= 0;
			biquad.yn2	= 0;
			biquad.b0	= coef.b0;
			biquad.b1	= coef.b1;
			biquad.b2	= coef.b2;
			biquad.a1	= coef.a1;
			biquad.a2	= coef.a2;

			mVpb.SetVoiceBiquad(biquad);
		}
	}
}

void AxVoice::SetRemoteFilter(u8 filter)
{
	ut::AutoInterruptLock lock;

	if (!mVpb.IsAvailable())
		return;

	if (filter == 0)
	{
		__AXPBRMTIIR iir;
		iir.lpf.on = AX_PB_OFF;

		mVpb.SetVoiceRmtIIR(iir);
	}
	else if (mVpb.IsRmtIirEnable())
	{
		u16 b0, b1, b2, a1, a2;
		Util::GetRemoteFilterCoefs(filter, &b0, &b1, &b2, &a1, &a2);

		mVpb.SetVoiceRmtIIRCoefs(AX_PB_BIQUAD_ON, b0, b1, b2, a1, a2);
	}
	else
	{
		__AXPBRMTIIR iir;
		iir.biquad.on	= AX_PB_BIQUAD_ON;
		iir.biquad.xn1	= 0;
		iir.biquad.xn2	= 0;
		iir.biquad.yn1	= 0;
		iir.biquad.yn2	= 0;

		Util::GetRemoteFilterCoefs(filter, &iir.biquad.b0, &iir.biquad.b1,
		                           &iir.biquad.b2, &iir.biquad.a1,
		                           &iir.biquad.a2);

		mVpb.SetVoiceRmtIIR(iir);
	}
}

void AxVoice::CalcOffsetAdpcmParam(u16 *outPredScale, u16 *outYn1, u16 *outYn2,
                                   u32 offset, void *dataAddr,
                                   AdpcmParam const &adpcmParam)
{
	AXPBADPCM adpcm;
	std::memcpy(adpcm.a, adpcmParam.coef, sizeof adpcm.a);
	adpcm.gain			= adpcmParam.gain;
	adpcm.pred_scale	= adpcmParam.pred_scale;
	adpcm.yn1			= adpcmParam.yn1;
	adpcm.yn2			= adpcmParam.yn2;

	u32 currentPos =
		GetDspAddressBySample(dataAddr, 0, SAMPLE_FORMAT_DSP_ADPCM);
	u32 endPos =
		GetDspAddressBySample(dataAddr, offset, SAMPLE_FORMAT_DSP_ADPCM);

	while (currentPos < endPos)
	{
		if (currentPos % AX_ADPCM_NIBBLES_PER_FRAME == 0)
		{
			byte_t byte = *static_cast<byte_t *>(OSPhysicalToCached(
				currentPos / sizeof(u16)));

			adpcm.pred_scale = byte;
			currentPos += sizeof(u16);
		}

		byte_t byte = *static_cast<byte_t *>(OSPhysicalToCached(
			currentPos / sizeof(u16)));

		u8 nibble;
		if (currentPos % sizeof(u16) != 0)
			nibble = byte & 0x0f;
		else
			nibble = byte >> 4;

		DecodeDspAdpcm(&adpcm, nibble);
		currentPos++;
	}

	*outPredScale	= adpcm.pred_scale;
	*outYn1			= adpcm.yn1;
	*outYn2			= adpcm.yn2;
}

void AxVoiceParamBlock::Sync()
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	mVpb->pb.ve.currentVolume = mPrevVeSetting.currentVolume;

	s16 deltaIn =
		(mVolume - mPrevVeSetting.currentVolume) / AX_SAMPLES_PER_FRAME;
	s16 deltaOut = deltaIn + BOOLIFY_TERNARY(deltaIn > 0) ? 1 : -1;

	int predIn = mPrevVeSetting.currentVolume + deltaIn * AX_SAMPLES_PER_FRAME;
	int predOut =
		mPrevVeSetting.currentVolume + deltaOut * AX_SAMPLES_PER_FRAME;

	if (ut::Abs(mVolume - predIn) < ut::Abs(mVolume - predOut))
		mVpb->pb.ve.currentDelta = deltaIn;
	else
		mVpb->pb.ve.currentDelta = deltaOut;

	int nextVolume = mPrevVeSetting.currentVolume
	               + mVpb->pb.ve.currentDelta * AX_SAMPLES_PER_FRAME;

	if (nextVolume < 0)
	{
		mVpb->pb.ve.currentDelta =
			-mPrevVeSetting.currentVolume / AX_SAMPLES_PER_FRAME;
	}
	else if (nextVolume > 32767)
	{
		mVpb->pb.ve.currentDelta =
			(32767 - mPrevVeSetting.currentVolume) / AX_SAMPLES_PER_FRAME;
	}

	if (mVpb->pb.ve.currentDelta == 0 && mPrevVeSetting.currentDelta == 0)
		mVpb->pb.ve.currentVolume = mVolume;

	mSync &= ~AX_VPB_SYNC_FLAG_VE_DELTA;
	mSync |= AX_VPB_SYNC_FLAG_VE;

	mPrevVeSetting.currentVolume = mVpb->pb.ve.currentVolume;
	mPrevVeSetting.currentDelta = mVpb->pb.ve.currentDelta;

	mVpb->sync |= mSync;
	mSync = 0;
}

bool AxVoiceParamBlock::IsRmtIirEnable() const
{
	return IsAvailable() && mVpb->pb.rmtIIR.biquad.on == AX_PB_BIQUAD_ON;
}

AxVoiceParamBlock::AxVoiceParamBlock() :
	mVpb				(nullptr),
	mSync				(),
	mFirstVeUpdateFlag	(false),
	mVolume				(DEFAULT_VOLUME)
{
	mPrevVeSetting.currentVolume = DEFAULT_VOLUME;
	mPrevVeSetting.currentDelta = 0;
}

void AxVoiceParamBlock::Set(AXVPB *vpb)
{
	NW4RAssertPointerNonnull_Line(1044, vpb);

	mVpb							= vpb;
	mSync							= 0;
	mFirstVeUpdateFlag				= true;
	mVolume							= DEFAULT_VOLUME;

	mPrevVeSetting.currentVolume	= DEFAULT_VOLUME;
	mPrevVeSetting.currentDelta		= 0;
}

void AxVoiceParamBlock::Clear()
{
	mVpb = nullptr;
	mSync = 0;
}

void AxVoiceParamBlock::SetVoiceType(u16 type)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	mVpb->pb.type = type;

	mSync |= AX_VPB_SYNC_FLAG_TYPE;
}

void AxVoiceParamBlock::SetVoiceVe(u16 volume, u16 initVolume)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	if (mFirstVeUpdateFlag)
	{
		mPrevVeSetting.currentVolume = initVolume;
		mFirstVeUpdateFlag = false;
	}

	mVolume = volume;
}

void AxVoiceParamBlock::SetVoiceMix(AXPBMIX const &mix, bool immediatelySync)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	u16 *src = const_cast<u16 *>(reinterpret_cast<u16 const *>(&mix));
	u16 *dst = reinterpret_cast<u16 *>(&mVpb->pb.mix);

	u32 mixerCtrl = 0;

	if ((*dst++ = *src++)) // vL
		mixerCtrl |= AX_MIXER_CTRL_L;

	if ((*dst++ = *src++)) // vDeltaL
		mixerCtrl |= AX_MIXER_CTRL_L | AX_MIXER_CTRL_DELTA;

	if ((*dst++ = *src++)) // vR
		mixerCtrl |= AX_MIXER_CTRL_R;

	if ((*dst++ = *src++)) // vDeltaR
		mixerCtrl |= AX_MIXER_CTRL_R | AX_MIXER_CTRL_DELTA;

	if ((*dst++ = *src++)) // vAuxAL
		mixerCtrl |= AX_MIXER_CTRL_A_L;

	if ((*dst++ = *src++)) // vDeltaAuxAL
		mixerCtrl |= AX_MIXER_CTRL_A_L | AX_MIXER_CTRL_A_DELTA;

	if ((*dst++ = *src++)) // vAuxAR
		mixerCtrl |= AX_MIXER_CTRL_A_R;

	if ((*dst++ = *src++)) // vDeltaAuxAR
		mixerCtrl |= AX_MIXER_CTRL_A_R | AX_MIXER_CTRL_A_DELTA;

	if ((*dst++ = *src++)) // vAuxBL
		mixerCtrl |= AX_MIXER_CTRL_B_L;

	if ((*dst++ = *src++)) // vDeltaAuxBL
		mixerCtrl |= AX_MIXER_CTRL_B_L | AX_MIXER_CTRL_B_DELTA;

	if ((*dst++ = *src++)) // vAuxBR
		mixerCtrl |= AX_MIXER_CTRL_B_R;

	if ((*dst++ = *src++)) // vDeltaAuxBR
		mixerCtrl |= AX_MIXER_CTRL_B_R | AX_MIXER_CTRL_B_DELTA;

	if ((*dst++ = *src++)) // vAuxCL
		mixerCtrl |= AX_MIXER_CTRL_C_L;

	if ((*dst++ = *src++)) // vDeltaAuxCL
		mixerCtrl |= AX_MIXER_CTRL_C_L | AX_MIXER_CTRL_C_DELTA;

	if ((*dst++ = *src++)) // vAuxCR
		mixerCtrl |= AX_MIXER_CTRL_C_R;

	if ((*dst++ = *src++)) // vDeltaAuxCR
		mixerCtrl |= AX_MIXER_CTRL_C_R | AX_MIXER_CTRL_C_DELTA;

	if ((*dst++ = *src++)) // vS
		mixerCtrl |= AX_MIXER_CTRL_S;

	if ((*dst++ = *src++)) // vDeltaS
		mixerCtrl |= AX_MIXER_CTRL_S | AX_MIXER_CTRL_DELTA_S;

	if ((*dst++ = *src++)) // vAuxAS
		mixerCtrl |= AX_MIXER_CTRL_A_S;

	if ((*dst++ = *src++)) // vDeltaAuxAS
		mixerCtrl |= AX_MIXER_CTRL_A_S | AX_MIXER_CTRL_A_DELTA_S;

	if ((*dst++ = *src++)) // vAuxBS
		mixerCtrl |= AX_MIXER_CTRL_B_S;

	if ((*dst++ = *src++)) // vDeltaAuxBS
		mixerCtrl |= AX_MIXER_CTRL_B_S | AX_MIXER_CTRL_B_DELTA_S;

	if ((*dst++ = *src++)) // vAuxCS
		mixerCtrl |= AX_MIXER_CTRL_C_S;

	if ((*dst++ = *src++)) // vDeltaAuxCS
		mixerCtrl |= AX_MIXER_CTRL_C_S | AX_MIXER_CTRL_C_DELTA_S;

	mVpb->pb.mixerCtrl = mixerCtrl;

	if (immediatelySync)
		mVpb->sync |= AX_VPB_SYNC_FLAG_MIX | AX_VPB_SYNC_FLAG_MIXER_CTRL;
	else
		mSync |= AX_VPB_SYNC_FLAG_MIX | AX_VPB_SYNC_FLAG_MIXER_CTRL;
}

void AxVoiceParamBlock::SetVoiceLoop(u16 loop)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	mVpb->pb.addr.loopFlag = loop;

	if (!(mVpb->sync & AX_VPB_SYNC_FLAG_ADDR))
		mVpb->sync |= AX_VPB_SYNC_FLAG_ADDR_LOOP_FLAG;
}

void AxVoiceParamBlock::SetVoiceLoopAddr(u32 addr)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	mVpb->pb.addr.loopAddressHi = addr >> 16;
	mVpb->pb.addr.loopAddressLo = addr & 0xffff;

	if (!(mVpb->sync & AX_VPB_SYNC_FLAG_ADDR))
		mVpb->sync |= AX_VPB_SYNC_FLAG_ADDR_LOOP_ADDR;
}

void AxVoiceParamBlock::SetVoiceEndAddr(u32 addr)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	mVpb->pb.addr.endAddressHi = addr >> 16;
	mVpb->pb.addr.endAddressLo = addr & 0xffff;

	if (!(mVpb->sync & AX_VPB_SYNC_FLAG_ADDR))
		mVpb->sync |= AX_VPB_SYNC_FLAG_ADDR_END_ADDR;
}

void AxVoiceParamBlock::SetVoiceAdpcm(AXPBADPCM const &adpcm)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	std::memcpy(&mVpb->pb.adpcm, &adpcm, sizeof mVpb->pb.adpcm);

	mSync |= AX_VPB_SYNC_FLAG_ADPCM;
}

void AxVoiceParamBlock::SetVoiceSrcType(u32 type)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	switch (type)
	{
	case AxVoice::SRC_NONE:
		mVpb->pb.srcSelect = 2;
		break;

	case AxVoice::SRC_LINEAR:
		mVpb->pb.srcSelect = 1;
		break;

	case AxVoice::SRC_4TAP_8K:
		mVpb->pb.srcSelect = 0;
		mVpb->pb.coefSelect = 0;
		break;

	case AxVoice::SRC_4TAP_12K:
		mVpb->pb.srcSelect = 0;
		mVpb->pb.coefSelect = 1;
		break;

	case AxVoice::SRC_4TAP_16K:
		mVpb->pb.srcSelect = 0;
		mVpb->pb.coefSelect = 2;
		break;
	}

	mSync |= AX_VPB_SYNC_FLAG_SRC_TYPE;
}

void AxVoiceParamBlock::SetVoiceSrc(AXPBSRC const &src)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	std::memcpy(&mVpb->pb.src, &src, sizeof mVpb->pb.src);

	mSync &= ~AX_VPB_SYNC_FLAG_SRC_RATIO;
	mSync |= AX_VPB_SYNC_FLAG_SRC;
}

void AxVoiceParamBlock::SetVoiceSrcRatio(f32 ratio)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	u32 r = 65536 * ratio;
	mVpb->pb.src.ratioHi = r >> 16;
	mVpb->pb.src.ratioLo = r & 0xffff;

	if (!(mSync & AX_VPB_SYNC_FLAG_SRC))
		mSync |= AX_VPB_SYNC_FLAG_SRC_RATIO;
}

void AxVoiceParamBlock::SetVoiceAdpcmLoop(AXPBADPCMLOOP const &adpcmloop)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	std::memcpy(&mVpb->pb.adpcmLoop, &adpcmloop, sizeof mVpb->pb.adpcmLoop);

	mSync |= AX_VPB_SYNC_FLAG_ADPCM_LOOP;
}

void AxVoiceParamBlock::SetVoiceLpf(AXPBLPF const &lpf)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	std::memcpy(&mVpb->pb.lpf, &lpf, sizeof mVpb->pb.lpf);

	mSync |= AX_VPB_SYNC_FLAG_LPF;
}

void AxVoiceParamBlock::SetVoiceLpfCoefs(u16 a0, u16 b0)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	mVpb->pb.lpf.a0 = a0;
	mVpb->pb.lpf.b0 = b0;

	mSync |= AX_VPB_SYNC_FLAG_LPF_COEFS;
}

void AxVoiceParamBlock::SetVoiceBiquad(AXPBBIQUAD const &biquad)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	std::memcpy(&mVpb->pb.biquad, &biquad, sizeof mVpb->pb.biquad);

	mSync |= AX_VPB_SYNC_FLAG_BIQUAD;
}

void AxVoiceParamBlock::SetVoiceBiquadCoefs(u16 b0, u16 b1, u16 b2, u16 a1,
                                            u16 a2)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	mVpb->pb.biquad.b0 = b0;
	mVpb->pb.biquad.b1 = b1;
	mVpb->pb.biquad.b2 = b2;
	mVpb->pb.biquad.a1 = a1;
	mVpb->pb.biquad.a2 = a2;

	mSync |= AX_VPB_SYNC_FLAG_BIQUAD_COEFS;
}

void AxVoiceParamBlock::SetVoiceRmtIIR(__AXPBRMTIIR const &iir)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	std::memcpy(&mVpb->pb.rmtIIR, &iir, sizeof mVpb->pb.rmtIIR);

	mSync |= AX_VPB_SYNC_FLAG_RMT_IIR;
}

void AxVoiceParamBlock::SetVoiceRmtIIRCoefs(u16 type, ...)
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	std::va_list argp;
	s32 ii;
	s32 num;

	if (type == AX_PB_LPF_ON)
		num = 2;
	else if (type == AX_PB_BIQUAD_ON)
		num = 5;
	else
		return;

	u16 coefs[5];

	va_start(argp, type);

	for (ii = 0; ii < num; ii++)
		coefs[ii] = va_arg(argp, unsigned long);

	va_end(argp);

	if (type == AX_PB_LPF_ON)
	{
		mVpb->pb.rmtIIR.lpf.a0 = coefs[0];
		mVpb->pb.rmtIIR.lpf.b0 = coefs[1];

		mSync |= AX_VPB_SYNC_FLAG_RMT_IIR_LPF_COEFS;
	}
	else
	{
		mVpb->pb.rmtIIR.biquad.b0 = coefs[0];
		mVpb->pb.rmtIIR.biquad.b1 = coefs[1];
		mVpb->pb.rmtIIR.biquad.b2 = coefs[2];
		mVpb->pb.rmtIIR.biquad.a1 = coefs[3];
		mVpb->pb.rmtIIR.biquad.a2 = coefs[4];

		mSync |= AX_VPB_SYNC_FLAG_RMT_IIR_BIQUAD_COEFS;
	}
}

void AxVoiceParamBlock::UpdateDelta()
{
	ut::AutoInterruptLock lock;

	if (!IsAvailable())
		return;

	mPrevVeSetting.currentVolume +=
		mVpb->pb.ve.currentDelta * AX_SAMPLES_PER_FRAME;

	mVpb->pb.ve.currentVolume = mPrevVeSetting.currentVolume;
	mVpb->pb.ve.currentDelta = 0;

	mVpb->sync |= AX_VPB_SYNC_FLAG_VE;
}

}}} // namespace nw4r::snd::detail
