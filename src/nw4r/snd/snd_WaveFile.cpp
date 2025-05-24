#include "nw4r/snd/snd_WaveFile.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_WaveFile.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_AxVoice.h"
#include "nw4r/snd/snd_Channel.h"
#include "nw4r/snd/snd_global.h"

#include "nw4r/ut/ut_algorithm.h" // ut::AddOffsetToPtr
#include "nw4r/ut/ut_binaryFileFormat.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

WaveFileReader::WaveFileReader(WaveFile::FileHeader const *waveFileHeader) :
	mWaveInfo	(nullptr)
{
	NW4RAssert_Line(42, waveFileHeader->fileHeader.signature
	                        == WaveFile::SIGNATURE_FILE);

	WaveFile::InfoBlock const *infoBlock =
		static_cast<WaveFile::InfoBlock const *>(ut::AddOffsetToPtr(
			waveFileHeader, waveFileHeader->infoChunkOffset));

	if (!infoBlock)
		return;

	NW4RAssert_Line(48, infoBlock->blockHeader.kind
	                        == WaveFile::SIGNATURE_INFO_BLOCK);

	mWaveInfo = &infoBlock->waveInfo;
}

WaveFileReader::WaveFileReader(WaveFile::WaveInfo const *waveInfo) :
	mWaveInfo	(waveInfo)
{
}

bool WaveFileReader::ReadWaveInfo(WaveInfo *waveData,
                                  void const *waveDataOffsetOrigin) const
{
	NW4RAssertPointerNonnull_Line(79, mWaveInfo);

	SampleFormat sampleFormat =
		GetSampleFormatFromWaveFileFormat(mWaveInfo->format);

	waveData->sampleFormat = sampleFormat;
	waveData->numChannels = mWaveInfo->numChannels;
	waveData->sampleRate =
		(mWaveInfo->sampleRate24 << 16) + mWaveInfo->sampleRate;
	waveData->loopFlag = mWaveInfo->loopFlag;
	waveData->loopStart = AxVoice::GetSampleByDspAddress(
		nullptr, mWaveInfo->loopStart, sampleFormat);
	waveData->loopEnd = AxVoice::GetSampleByDspAddress(
		nullptr, mWaveInfo->loopEnd, sampleFormat) + 1;

	u32 const *channelInfoOffset = static_cast<u32 const *>(
		ut::AddOffsetToPtr(mWaveInfo, mWaveInfo->channelInfoTableOffset));

	for (int channelIndex = 0; channelIndex < mWaveInfo->numChannels;
	     channelIndex++)
	{
		if (channelIndex >= Channel::CHANNEL_MAX)
			continue;

		ChannelParam &channelParam = waveData->channelParam[channelIndex];

		WaveFile::WaveChannelInfo const *waveChannelInfo =
			static_cast<WaveFile::WaveChannelInfo const *>(
				ut::AddOffsetToPtr(mWaveInfo, channelInfoOffset[channelIndex]));

		if (waveChannelInfo->adpcmOffset)
		{
			WaveFile::AdpcmParamSet const *adpcmParamSet =
				static_cast<WaveFile::AdpcmParamSet const *>(ut::AddOffsetToPtr(
					mWaveInfo, waveChannelInfo->adpcmOffset));

			channelParam.adpcmParam		= adpcmParamSet->adpcmParam;
			channelParam.adpcmLoopParam	= adpcmParamSet->adpcmLoopParam;
		}

		channelParam.dataAddr = const_cast<void *>(
			GetWaveDataAddress(waveChannelInfo, waveDataOffsetOrigin));
	}

	return true;
}

void const *WaveFileReader::GetWaveDataAddress(
	WaveFile::WaveChannelInfo const *waveChannelInfo,
	void const *waveDataOffsetOrigin) const
{
	NW4RAssertPointerNonnull_Line(128, mWaveInfo);
	NW4RAssertPointerNonnull_Line(129, waveChannelInfo);

	void const *waveDataAddress = nullptr;
	bool offsetIsDataBlock = waveDataOffsetOrigin == nullptr;

	if (!waveDataOffsetOrigin)
		waveDataOffsetOrigin = mWaveInfo;

	// TODO: InstInfo::WaveDataLocation::WaveDataLocationType?
	switch (mWaveInfo->dataLocationType)
	{
	case 0:
		waveDataAddress =
			ut::AddOffsetToPtr(waveDataOffsetOrigin, mWaveInfo->dataLocation);

		// TODO: sizeof(InstInfo::WaveDataLocation)?
		if (offsetIsDataBlock)
			waveDataAddress = ut::AddOffsetToPtr(waveDataAddress, 8);

		break;

	case 1:
		waveDataAddress =
			reinterpret_cast<void const *>(mWaveInfo->dataLocation);
		break;

	default:
		return nullptr;
	}

	waveDataAddress = ut::AddOffsetToPtr(
		waveDataAddress, waveChannelInfo->channelDataOffset);

	return waveDataAddress;
}

SampleFormat WaveFileReader::GetSampleFormatFromWaveFileFormat(u8 format)
{
	switch (format)
	{
	case 2:
		return SAMPLE_FORMAT_DSP_ADPCM;

	case 1:
		return SAMPLE_FORMAT_PCM_S16;

	case 0:
		return SAMPLE_FORMAT_PCM_S8;

	default:
		NW4RPanicMessage_Line(167, "Unknown wave data format %d", format);
		return SAMPLE_FORMAT_DSP_ADPCM;
	}
}

}}} // namespace nw4r::snd::detail
