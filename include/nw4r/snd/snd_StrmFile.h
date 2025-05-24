#ifndef NW4R_SND_STRM_FILE_H
#define NW4R_SND_STRM_FILE_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_adpcm.h"
#include "nw4r/snd/snd_global.h" // SampleFormat
#include "nw4r/snd/snd_Util.h" // Util::DataRef

#include "nw4r/ut/ut_binaryFileFormat.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace ut { class FileStream; }}

namespace nw4r { namespace snd { namespace detail
{
	struct StrmFile
	{
		/* Header */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f271d
		struct Header
		{
			ut::BinaryFileHeader	fileHeader;			// size 0x10, offset 0x00
			u32						headBlockOffset;	// size 0x04, offset 0x10
			u32						headBlockSize;		// size 0x04, offset 0x14
			u32						adpcBlockOffset;	// size 0x04, offset 0x18
			u32						adpcBlockSize;		// size 0x04, offset 0x1c
			u32						dataBlockOffset;	// size 0x04, offset 0x20
			u32						dataBlockSize;		// size 0x04, offset 0x24
		}; // size 0x28

		/* HeadBlock */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f2d81
		struct StrmDataInfo
		{
			u8	format;					// size 0x01, offset 0x00
			u8	loopFlag;				// size 0x01, offset 0x01
			u8	numChannels;			// size 0x01, offset 0x02
			u8	sampleRate24;			// size 0x01, offset 0x03
			u16	sampleRate;				// size 0x02, offset 0x04
			u16	blockHeaderOffset;		// size 0x02, offset 0x06
			u32	loopStart;				// size 0x04, offset 0x08
			u32	loopEnd;				// size 0x04, offset 0x0c
			u32	dataOffset;				// size 0x04, offset 0x10
			u32	numBlocks;				// size 0x04, offset 0x14
			u32	blockSize;				// size 0x04, offset 0x18
			u32	blockSamples;			// size 0x04, offset 0x1c
			u32	lastBlockSize;			// size 0x04, offset 0x20
			u32	lastBlockSamples;		// size 0x04, offset 0x24
			u32	lastBlockPaddedSize;	// size 0x04, offset 0x28
			u32	adpcmDataInterval;		// size 0x04, offset 0x2c
			u32	adpcmDataSize;			// size 0x04, offset 0x30
		}; // size 0x34

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f3a1e
		struct TrackInfo
		{
			u8	channelCount;			// size 0x01, offset 0x00
			u8	channelIndexTable[];	// flexible,  offset 0x01 (unit size 0x01)
		}; // size 0x01

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f3196
		struct TrackInfoEx
		{
			u8		volume;					// size 0x01, offset 0x00
			u8		pan;					// size 0x01, offset 0x01
			u8	padding[2];
			u32	reserved;
			u8		channelCount;			// size 0x01, offset 0x08
			u8		channelIndexTable[];	// flexible,  offset 0x09 (unit size 0x01)
			/* 3 bytes padding */
		}; // size 0x0c

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f34ef
		struct TrackTable
		{
			u8										trackCount;			// size 0x01, offset 0x00
			u8										trackDataType;		// size 0x01, offset 0x01
			u8									padding[2];
			Util::DataRef<TrackInfo, TrackInfoEx>	refTrackHeader[];	// flexible,  offset 0x04 (unit size 0x08)
		}; // size 0x04

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f336f
		struct AdpcmParamSet
		{
			AdpcmParam		adpcmParam;		// size 0x28, offset 0x00
			AdpcmLoopParam	adpcmLoopParam;	// size 0x06, offset 0x28
		}; // size 0x2e

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f3cbb
		struct ChannelInfo
		{
			Util::DataRef<AdpcmParamSet>	refAdpcmInfo;	// size 0x08, offset 0x00
		}; // size 0x08

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f3780
		struct ChannelTable
		{
			u8							channelCount;		// size 0x01, offset 0x00
			u8						padding[3];
			Util::DataRef<ChannelInfo>	refChannelHeader[];	// flexible,  offset 0x04 (unit size 0x08)
		}; // size 0x04

		static u32 const SIGNATURE_HEAD_BLOCK =
			NW4R_FOUR_BYTE('H', 'E', 'A', 'D');

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f2ab0
		struct HeadBlock
		{
			ut::BinaryBlockHeader		blockHeader;		// size 0x08, offset 0x00
			Util::DataRef<StrmDataInfo>	refDataHeader;		// size 0x08, offset 0x08
			Util::DataRef<TrackTable>	refTrackTable;		// size 0x08, offset 0x10
			Util::DataRef<ChannelTable>	refChannelTable;	// size 0x08, offset 0x18
		}; // size 0x20

		/* StrmFile */

		static u32 const SIGNATURE_FILE =
			NW4R_FOUR_BYTE('R', 'S', 'T', 'M');
		static int const FILE_VERSION = NW4R_FILE_VERSION(1, 0);
	}; // "namespace" StrmFile
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f2b8e
	class StrmFileReader
	{
	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f0e7
		struct StrmInfo
		{
			SampleFormat	sampleFormat;			// size 0x04, offset 0x00
			bool			loopFlag;				// size 0x01, offset 0x04
			/* 3 bytes padding */
			int				numChannels;			// size 0x04, offset 0x08
			int				sampleRate;				// size 0x04, offset 0x0c
			u16				blockHeaderOffset;		// size 0x02, offset 0x10
			/* 2 bytes padding */
			u32				loopStart;				// size 0x04, offset 0x14
			u32				loopEnd;				// size 0x04, offset 0x18
			u32				dataOffset;				// size 0x04, offset 0x1c
			u32				numBlocks;				// size 0x04, offset 0x20
			u32				blockSize;				// size 0x04, offset 0x24
			u32				blockSamples;			// size 0x04, offset 0x28
			u32				lastBlockSize;			// size 0x04, offset 0x2c
			u32				lastBlockSamples;		// size 0x04, offset 0x30
			u32				lastBlockPaddedSize;	// size 0x04, offset 0x34
			u32				adpcmDataInterval;		// size 0x04, offset 0x38
			u32				adpcmDataSize;			// size 0x04, offset 0x3c
		}; // size 0x40

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2fa44
		struct StrmTrackInfo
		{
			u8	volume;					// size 0x01, offset 0x00
			u8	pan;					// size 0x01, offset 0x01
			/* 2 bytes padding */
			int	channelCount;			// size 0x04, offset 0x04
			u8	channelIndexTable[32];	// size 0x20, offset 0x08
		}; // size 0x28

	// methods
	public:
		// cdtors
		StrmFileReader();

		// methods
		void Setup(void const *strmData);

		bool IsAvailable() const { return mHeader != nullptr; }
		u32 GetAdpcBlockOffset() const
		{
			if (IsAvailable())
				return mHeader->adpcBlockOffset;

			return 0;
		}

		int GetTrackCount() const;
		int GetChannelCount() const;

		bool ReadStrmInfo(StrmInfo *strmInfo) const;
		bool ReadStrmTrackInfo(StrmTrackInfo *trackInfo, int trackIndex) const;
		bool ReadAdpcmInfo(AdpcmParam *adpcmParam,
		                   AdpcmLoopParam *adpcmLoopParam,
		                   int channelIndex) const;

	private:
		static bool IsValidFileHeader(void const *strmData);

		static SampleFormat GetSampleFormatFromStrmFileFormat(u8 format);

	// static members
	public:
		static int const SUPPORTED_FILE_VERSION = NW4R_FILE_VERSION(1, 0);

	// members
	private:
		StrmFile::Header	const *mHeader;		// size 0x04, offset 0x00
		StrmFile::HeadBlock	const *mHeadBlock;	// size 0x04, offset 0x04

	// friends
	private:
		friend class StrmFileLoader;
	}; // size 0x08

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2f3fc4
	class StrmFileLoader
	{
	// methods
	public:
		// cdtors
		StrmFileLoader(ut::FileStream &stream) : mStream(stream) {}

		// methods
		bool LoadFileHeader(void *buffer, u32 size);

		int GetTrackCount() const;
		int GetChannelCount() const;

		bool ReadStrmInfo(StrmFileReader::StrmInfo *strmInfo) const;
		bool ReadStrmTrackInfo(StrmFileReader::StrmTrackInfo *trackInfo,
		                       int trackIndex) const;
		bool ReadAdpcmInfo(AdpcmParam *adpcmParam,
		                   AdpcmLoopParam *adpcmLoopParam,
		                   int channelIndex) const;
		bool ReadAdpcBlockData(u16 *yn1, u16 *yn2, int blockIndex,
		                       int channelCount);

	// members
	private:
		ut::FileStream	&mStream;	// size 0x04, offset 0x00
		StrmFileReader	mReader;	// size 0x08, offset 0x04
	}; // size 0x0c
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_STRM_FILE_H
