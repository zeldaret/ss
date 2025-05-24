#ifndef NW4R_SND_WAVE_FILE_H
#define NW4R_SND_WAVE_FILE_H

/*******************************************************************************
 * headers
 */

#include <macros.h> // NW4R_FOUR_BYTE
#include <types.h>

#include "Channel.h"
#include "global.h" // SampleFormat
#include "adpcm.h"

#include "../ut/binaryFileFormat.h"

/*******************************************************************************
 * types
 */

namespace nw4r { namespace snd { namespace detail
{
	struct WaveFile
	{
		/* Header */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b2ab1
		struct FileHeader
		{
			ut::BinaryFileHeader	fileHeader;			// size 0x10, offset 0x00
			u32						infoChunkOffset;	// size 0x04, offset 0x10
			u32						infoChunkSize;		// size 0x04, offset 0x14
			u32						dataChunkOffset;	// size 0x04, offset 0x18
			u32						dataChunkSize;		// size 0x04, offset 0x1c
		}; // size 0x20

		/* InfoBlock */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b2185
		struct WaveInfo
		{
			u8		format;					// size 0x01, offset 0x00
			u8		loopFlag;				// size 0x01, offset 0x01
			u8		numChannels;			// size 0x01, offset 0x02
			u8		sampleRate24;			// size 0x01, offset 0x03
			u16		sampleRate;				// size 0x02, offset 0x04
			u8		dataLocationType;		// size 0x01, offset 0x06
			byte1_t	padding;
			u32		loopStart;				// size 0x04, offset 0x08
			u32		loopEnd;				// size 0x04, offset 0x0c
			u32		channelInfoTableOffset;	// size 0x04, offset 0x10
			u32		dataLocation;			// size 0x04, offset 0x14
			byte4_t	reserved;
		}; // size 0x1c

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b2c9d
		struct WaveChannelInfo
		{
			u32	channelDataOffset;	// size 0x04, offset 0x00
			u32	adpcmOffset;		// size 0x04, offset 0x04
			u32	volumeFrontLeft;	// size 0x04, offset 0x08
			u32	volumeFrontRight;	// size 0x04, offset 0x0c
			u32	volumeRearLeft;		// size 0x04, offset 0x10
			u32	volumeRearRight;	// size 0x04, offset 0x14
			u32	reserved;
		}; // size 0x1c

		static byte4_t const SIGNATURE_INFO_BLOCK =
			NW4R_FOUR_BYTE('I', 'N', 'F', 'O');

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x3087c5
		struct InfoBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			WaveInfo				waveInfo;		// size 0x1c, offset 0x08
		}; // size 0x24

		/* Other */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x308a10
		struct AdpcmParamSet
		{
			AdpcmParam		adpcmParam;		// size 0x28, offset 0x00
			AdpcmLoopParam	adpcmLoopParam;	// size 0x06, offset 0x28
		}; // size 0x2e

		/* WaveFile */

		static byte4_t const SIGNATURE_FILE =
			NW4R_FOUR_BYTE('R', 'W', 'A', 'V');
		static int const FILE_VERSION; // does it even have one?
	}; // "namespace" WaveFile

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x29336
	struct ChannelParam
	{
		void			*dataAddr;			// size 0x04, offset 0x00
		AdpcmParam		adpcmParam;			// size 0x28, offset 0x04
		AdpcmLoopParam	adpcmLoopParam;		// size 0x06, offset 0x2c
		/* 2 bytes padding */
	}; // size 0x34

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x29401
	struct WaveInfo
	{
		SampleFormat	sampleFormat;						// size 0x04, offset 0x00
		bool			loopFlag;							// size 0x01, offset 0x04
		/* 3 bytes padding */
		int				numChannels;						// size 0x04, offset 0x08
		int				sampleRate;							// size 0x04, offset 0x0c
		u32				loopStart;							// size 0x04, offset 0x10
		u32				loopEnd;							// size 0x04, offset 0x14
		ChannelParam	channelParam[Channel::CHANNEL_MAX];	// size 0x68, offset 0x18
	}; // size 0x80

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e58c
	struct WaveSoundInfo
	{
		f32	pitch;			// size 0x04, offset 0x00
		u8	pan;			// size 0x01, offset 0x04
		u8	surroundPan;	// size 0x01, offset 0x05
		u8	fxSendA;		// size 0x01, offset 0x06
		u8	fxSendB;		// size 0x01, offset 0x07
		u8	fxSendC;		// size 0x01, offset 0x08
		u8	mainSend;		// size 0x01, offset 0x09
		/* 2 bytes padding */
	}; // size 0x0c

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e6c2
	struct WaveSoundNoteInfo
	{
		s32	waveIndex;		// size 0x04, offset 0x00
		u8	attack;			// size 0x01, offset 0x04
		u8	hold;			// size 0x01, offset 0x05
		u8	decay;			// size 0x01, offset 0x06
		u8	sustain;		// size 0x01, offset 0x07
		u8	release;		// size 0x01, offset 0x08
		u8	originalKey;	// size 0x01, offset 0x09
		u8	pan;			// size 0x01, offset 0x0a
		u8	surroundPan;	// size 0x01, offset 0x0b
		u8	volume;			// size 0x01, offset 0x0c
		/* 3 bytes padding */
		f32	pitch;			// size 0x04, offset 0x10
	}; // size 0x14
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b2e05
	class WaveFileReader
	{
	// methods
	public:
		// cdtors
		WaveFileReader(WaveFile::FileHeader const *waveFileHeader);
		WaveFileReader(WaveFile::WaveInfo const *waveInfo);

		// methods
		bool ReadWaveInfo(WaveInfo *waveData) const
		{
			return ReadWaveInfo(waveData, nullptr);
		}

		bool ReadWaveInfo(WaveInfo *waveData,
		                  void const *waveDataOffsetOrigin) const;

		void const *GetWaveDataAddress(
			WaveFile::WaveChannelInfo const *waveChannelInfo,
			void const *waveDataOffsetOrigin) const;

		static SampleFormat GetSampleFormatFromWaveFileFormat(u8 format);

	// members
	private:
		WaveFile::WaveInfo	const *mWaveInfo;	// size 0x04, offset 0x00
	}; // size 0x04

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2954b
	class WaveDataLocationCallback
	{
	// methods
	public:
		// virtual function ordering
		// vtable WaveDataLocationCallback
		virtual WaveInfo *at_0x08();
		virtual void at_0x0c(WaveInfo const *);

		// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_WAVE_FILE_H
