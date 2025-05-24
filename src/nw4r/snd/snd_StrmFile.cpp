#include "nw4r/snd/StrmFile.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_StrmFile.cpp
 */

/*******************************************************************************
 * headers
 */

#include <macros.h>
#include <types.h>

#include "nw4r/snd/adpcm.h"
#include "nw4r/snd/global.h"
#include "nw4r/snd/Util.h"
#include "nw4r/snd/WaveFile.h"

#include "nw4r/ut/binaryFileFormat.h"
#include "nw4r/ut/FileStream.h"
#include "nw4r/ut/inlines.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

bool StrmFileReader::IsValidFileHeader(void const *strmData)
{
	NW4RAssertPointerNonnull_Line(42, strmData);

	ut::BinaryFileHeader const *fileHeader =
		static_cast<ut::BinaryFileHeader const *>(strmData);

	NW4RAssertMessage_Line(
		51, fileHeader->signature == StrmFile::SIGNATURE_FILE,
		"invalid file signature. strm data is not available.");

	if (fileHeader->signature != StrmFile::SIGNATURE_FILE)
		return false;

	NW4RAssertMessage_Line(59, fileHeader->version >= NW4R_FILE_VERSION(1, 0),
	            "strm file is not supported version.\n"
	            "  please reconvert file using new version tools.\n");

	if (fileHeader->version < NW4R_FILE_VERSION(1, 0))
		return false;

	NW4RAssertMessage_Line(65, fileHeader->version <= SUPPORTED_FILE_VERSION,
	            "strm file is not supported version.\n"
	            "  please reconvert file using new version tools.\n");

	if (fileHeader->version > SUPPORTED_FILE_VERSION)
		return false;

	return true;
}

StrmFileReader::StrmFileReader() :
	mHeader		(nullptr),
	mHeadBlock	(nullptr)
{
}

void StrmFileReader::Setup(void const *strmData)
{
	NW4RAssertPointerNonnull_Line(97, strmData);

	if (!IsValidFileHeader(strmData))
		return;

	mHeader		= static_cast<StrmFile::Header const *>(strmData);
	mHeadBlock	= static_cast<StrmFile::HeadBlock const *>(
		ut::AddOffsetToPtr(mHeader, mHeader->headBlockOffset));

	NW4RAssert_Line(106, mHeadBlock->blockHeader.kind
	                         == StrmFile::SIGNATURE_HEAD_BLOCK);

	StrmFile::StrmDataInfo const *info = Util::GetDataRefAddress0(
		mHeadBlock->refDataHeader, &mHeadBlock->refDataHeader);

	// definitely could have just used align assert here
	NW4RAssert_Line(113, info->blockSize % 32 == 0);
}

int StrmFileReader::GetTrackCount() const
{
	NW4RAssertPointerNonnull_Line(142, mHeader);

	StrmFile::TrackTable const *trackTable = Util::GetDataRefAddress0(
		mHeadBlock->refTrackTable, &mHeadBlock->refDataHeader);

	return trackTable->trackCount;
}

int StrmFileReader::GetChannelCount() const
{
	NW4RAssertPointerNonnull_Line(163, mHeader);

	StrmFile::ChannelTable const *channelTable = Util::GetDataRefAddress0(
		mHeadBlock->refChannelTable, &mHeadBlock->refDataHeader);

	return channelTable->channelCount;
}

bool StrmFileReader::ReadStrmInfo(StrmInfo *strmInfo) const
{
	NW4RAssertPointerNonnull_Line(184, mHeader);

	StrmFile::StrmDataInfo const *info = Util::GetDataRefAddress0(
		mHeadBlock->refDataHeader, &mHeadBlock->refDataHeader);

	NW4RAssertAligned_Line(192, info->blockHeaderOffset, 32);
	NW4RAssertAligned_Line(193, info->blockSize, 32);
	NW4RAssertAligned_Line(194, info->lastBlockPaddedSize, 32);

	// clang-format off
	strmInfo->sampleFormat			= GetSampleFormatFromStrmFileFormat(info->format);
	strmInfo->loopFlag				= info->loopFlag;
	strmInfo->numChannels			= info->numChannels;
	strmInfo->sampleRate			= (info->sampleRate24 << 16) + info->sampleRate;
	strmInfo->blockHeaderOffset		= info->blockHeaderOffset;
	strmInfo->loopStart				= info->loopStart;
	strmInfo->loopEnd				= info->loopEnd;
	strmInfo->dataOffset			= info->dataOffset;
	strmInfo->numBlocks				= info->numBlocks;
	strmInfo->blockSize				= info->blockSize;
	strmInfo->blockSamples			= info->blockSamples;
	strmInfo->lastBlockSize			= info->lastBlockSize;
	strmInfo->lastBlockSamples		= info->lastBlockSamples;
	strmInfo->lastBlockPaddedSize	= info->lastBlockPaddedSize;
	strmInfo->adpcmDataInterval		= info->adpcmDataInterval;
	strmInfo->adpcmDataSize			= info->adpcmDataSize;
	// clang-format on

	return true;
}

bool StrmFileReader::ReadStrmTrackInfo(StrmTrackInfo *trackInfo,
                                       int trackIndex) const
{
	NW4RAssertPointerNonnull_Line(218, mHeader);

	StrmFile::TrackTable const *trackTable = Util::GetDataRefAddress0(
		mHeadBlock->refTrackTable, &mHeadBlock->refDataHeader);
	if (trackIndex >= trackTable->trackCount)
		return false;

	switch (trackTable->trackDataType)
	{
	case 0:
	{
		StrmFile::TrackInfo const *src = Util::GetDataRefAddress0(
			trackTable->refTrackHeader[trackIndex], &mHeadBlock->refDataHeader);
		if (!src)
			return false;

		trackInfo->volume		= 127;
		trackInfo->pan			= 64;
		trackInfo->channelCount	= src->channelCount;

		int count = ut::Min(trackInfo->channelCount, 32);

		for (int i = 0; i < count; i++)
			trackInfo->channelIndexTable[i] = src->channelIndexTable[i];
	}
		break;

	case 1:
	{
		StrmFile::TrackInfoEx const *src = Util::GetDataRefAddress1(
			trackTable->refTrackHeader[trackIndex], &mHeadBlock->refDataHeader);
		if (!src)
			return false;

		trackInfo->volume		= src->volume;
		trackInfo->pan			= src->pan;
		trackInfo->channelCount	= src->channelCount;

		int count = ut::Min(trackInfo->channelCount, 32);

		for (int i = 0; i < count; i++)
			trackInfo->channelIndexTable[i] = src->channelIndexTable[i];
	}
		break;

	default:
		NW4RPanic_Line(268);
		// return false here for NDEBUG?
		break;
	}

	return true;
}

bool StrmFileReader::ReadAdpcmInfo(AdpcmParam *adpcmParam,
                                   AdpcmLoopParam *adpcmLoopParam,
                                   int channelIndex) const
{
	NW4RAssertPointerNonnull_Line(289, mHeader);
	NW4RAssertPointerNonnull_Line(290, adpcmParam);
	NW4RAssertPointerNonnull_Line(291, adpcmLoopParam);

	StrmFile::StrmDataInfo const *info = Util::GetDataRefAddress0(
		mHeadBlock->refDataHeader, &mHeadBlock->refDataHeader);
	if (info->format != 2)
		return false;

	StrmFile::ChannelTable const *channelTable = Util::GetDataRefAddress0(
		mHeadBlock->refChannelTable, &mHeadBlock->refDataHeader);
	if (channelIndex >= channelTable->channelCount)
		return false;

	StrmFile::ChannelInfo const *channelInfo =
		Util::GetDataRefAddress0(channelTable->refChannelHeader[channelIndex],
	                               &mHeadBlock->refDataHeader);

	StrmFile::AdpcmParamSet const *src = Util::GetDataRefAddress0(
		channelInfo->refAdpcmInfo, &mHeadBlock->refDataHeader);

	*adpcmParam		= src->adpcmParam;
	*adpcmLoopParam	= src->adpcmLoopParam;

	return true;
}

SampleFormat StrmFileReader::GetSampleFormatFromStrmFileFormat(u8 format)
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
		NW4RPanicMessage_Line(333, "Unknown strm data format %d", format);
		return SAMPLE_FORMAT_DSP_ADPCM;
	}
}

bool StrmFileLoader::LoadFileHeader(void *buffer, u32 size)
{
	byte_t buffer2[32 + ROUND_UP(sizeof(StrmFile::Header), 0x20)];

	mStream.Seek(0, ut::FileStream::SEEK_ORIGIN_SET);

	s32 readSize = mStream.Read(ut::RoundUp(buffer2, 32),
	                            ROUND_UP(sizeof(StrmFile::Header), 0x20));
	if (readSize != ROUND_UP(sizeof(StrmFile::Header), 0x20))
		return false;

	StrmFile::Header *header =
		static_cast<StrmFile::Header *>(ut::RoundUp(buffer2, 32));

	StrmFileReader reader;
	if (!reader.IsValidFileHeader(header))
		return false;

	if (header->adpcBlockOffset > size)
		return false;

	u32 loadSize = header->headBlockOffset + header->headBlockSize;

	mStream.Seek(0, ut::FileStream::SEEK_ORIGIN_SET);

	readSize = mStream.Read(buffer, loadSize);
	if (readSize != loadSize)
		return false;

	mReader.Setup(buffer);

	return true;
}

int StrmFileLoader::GetTrackCount() const
{
	if (!mReader.IsAvailable())
		return 0;

	return mReader.GetTrackCount();
}

int StrmFileLoader::GetChannelCount() const
{
	if (!mReader.IsAvailable())
		return 0;

	return mReader.GetChannelCount();
}

bool StrmFileLoader::ReadStrmInfo(StrmFileReader::StrmInfo *strmInfo) const
{
	if (!mReader.IsAvailable())
		return false;

	mReader.ReadStrmInfo(strmInfo);
	return true;
}

bool StrmFileLoader::ReadStrmTrackInfo(StrmFileReader::StrmTrackInfo *trackInfo,
                                       int trackIndex) const
{
	if (!mReader.IsAvailable())
		return false;

	mReader.ReadStrmTrackInfo(trackInfo, trackIndex);
	return true;
}

bool StrmFileLoader::ReadAdpcmInfo(AdpcmParam *adpcmParam,
                                   AdpcmLoopParam *adpcmLoopParam,
                                   int channelIndex) const
{
	if (!mReader.IsAvailable())
		return false;

	mReader.ReadAdpcmInfo(adpcmParam, adpcmLoopParam, channelIndex);
	return true;
}

bool StrmFileLoader::ReadAdpcBlockData(u16 *yn1, u16 *yn2, int blockIndex,
                                       int channelCount)
{
	if (!mReader.IsAvailable())
		return false;

	s32 readOffset = mReader.GetAdpcBlockOffset()
	               + blockIndex * channelCount * (sizeof(u16) * 2)
	               + sizeof(ut::BinaryBlockHeader);

	mStream.Seek(readOffset, ut::FileStream::SEEK_ORIGIN_SET);

	u32 readDataSize = channelCount * (sizeof(u16) * 2);
	NW4RAssert_Line(499, readDataSize <= 32);

	alignas(32) u16 buffer[2 * 8];

	int readSize = mStream.Read(buffer, sizeof buffer);
	if (readSize != 32u)
		return false;

	for (int i = 0; i < channelCount; i++)
	{
		yn1[i] = buffer[i * 2];
		yn2[i] = buffer[i * 2 + 1];
	}

	return true;
}

}}} // namespace nw4r::snd::detail
