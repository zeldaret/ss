#ifndef NW4R_SND_WSD_FILE_H
#define NW4R_SND_WSD_FILE_H

/*******************************************************************************
 * headers
 */

#include <macros.h>
#include <types.h>

#include "Util.h"

#include "../ut/binaryFileFormat.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { struct WaveInfo; }}}
namespace nw4r { namespace snd { namespace detail { struct WaveSoundInfo; }}}
namespace nw4r { namespace snd { namespace detail { struct WaveSoundNoteInfo; }}}

namespace nw4r { namespace snd { namespace detail
{
	struct WsdFile
	{
		/* Header */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e4ce7
		struct Header
		{
			ut::BinaryFileHeader	fileHeader;			// size 0x10, offset 0x00
			u32						dataBlockOffset;	// size 0x04, offset 0x10
			u32						dataBlockSize;		// size 0x04, offset 0x14
			u32						waveBlockOffset;	// size 0x04, offset 0x18
			u32						waveBlockSize;		// size 0x04, offset 0x1c
		}; // size 0x20

		/* DataBlock */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x30abf7
		struct WsdInfo
		{
			f32					pitch;				// size 0x04, offset 0x00
			u8					pan;				// size 0x01, offset 0x04
			u8					surroundPan;		// size 0x01, offset 0x05
			u8					fxSendA;			// size 0x01, offset 0x06
			u8					fxSendB;			// size 0x01, offset 0x07
			u8					fxSendC;			// size 0x01, offset 0x08
			u8					mainSend;			// size 0x01, offset 0x09
			byte1_t				padding[2];
			// TODO: template parameters
			Util::DataRef<void>	graphEnvTablevRef;	// size 0x08, offset 0x0c
			Util::DataRef<void>	randomizerTableRef;	// size 0x08, offset 0x14
			byte4_t				reserved;
		}; // size 0x20

		struct TrackInfo;

		typedef Util::Table<Util::DataRef<TrackInfo> > TrackInfoTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x30af52
		struct NoteInfo
		{
			s32					waveIndex;			// size 0x04, offset 0x00
			u8					attack;				// size 0x01, offset 0x04
			u8					decay;				// size 0x01, offset 0x05
			u8					sustain;			// size 0x01, offset 0x06
			u8					release;			// size 0x01, offset 0x07
			u8					hold;				// size 0x01, offset 0x08
			byte1_t				padding[3];
			u8					originalKey;		// size 0x01, offset 0x0c
			u8					volume;				// size 0x01, offset 0x0d
			u8					pan;				// size 0x01, offset 0x0e
			u8					surroundPan;		// size 0x01, offset 0x0f
			f32					pitch;				// size 0x04, offset 0x10
			// TODO: template parameters
			Util::DataRef<void>	lfoTableRef;		// size 0x08, offset 0x14
			Util::DataRef<void>	graphEnvTablevRef;	// size 0x08, offset 0x1c
			Util::DataRef<void>	randomizerTableRef;	// size 0x08, offset 0x24
			byte4_t				reserved;
		}; // size 0x30

		typedef Util::Table<Util::DataRef<NoteInfo> > NoteInfoTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x30ab32
		struct Wsd
		{
			Util::DataRef<WsdInfo>			refWsdInfo;		// size 0x08, offset 0x00
			Util::DataRef<TrackInfoTable>	refTrackTable;	// size 0x08, offset 0x08
			Util::DataRef<NoteInfoTable>	refNoteTable;	// size 0x08, offset 0x10
		}; // size 0x18

		static byte4_t const SIGNATURE_DATA_BLOCK =
			NW4R_FOUR_BYTE('D', 'A', 'T', 'A');

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e4ec8
		struct DataBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			u32						wsdCount;		// size 0x04, offset 0x08
			Util::DataRef<Wsd>		refWsd[];		// flexible,  offset 0x0c (unit size 0x08)
		}; // size 0x0c

		/* WaveBlock */

		static byte4_t const SIGNATURE_WAVE_BLOCK =
			NW4R_FOUR_BYTE('W', 'A', 'V', 'E');

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e4f87
		struct WaveBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x04, offset 0x00
			u32						waveCount;		// size 0x04, offset 0x08
			u32						offsetTable[];	// flexible,  offset 0x0c (unit size 0x04)
		}; // size 0x0c

		/* WaveBlockOld */

		// FILE_VERSION <= NW4R_FILE_VERSION(1, 0)
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x30b523
		struct WaveBlockOld
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			u32						offsetTable[];	// flexible,  offset 0x08 (unit size 0x04)
		}; // size 0x08

		/* WsdFile */

		static byte4_t const SIGNATURE_FILE =
			NW4R_FOUR_BYTE('R', 'W', 'S', 'D');
		static int const FILE_VERSION = NW4R_FILE_VERSION(1, 3);
	}; // "namespace" WsdFile
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e502c
	class WsdFileReader
	{
	// methods
	public:
		// cdtors
		WsdFileReader(void const *wsdData);

		// methods
		bool ReadWaveInfo(int waveIndex, WaveInfo *waveData,
		                  void const *waveDataAddress) const;
		bool ReadWaveSoundInfo(WaveSoundInfo *info, int index) const;
		bool ReadWaveSoundNoteInfo(WaveSoundNoteInfo *noteInfo, int index,
		                           int noteIndex) const;

	private:
		bool IsValidFileHeader(void const *wsdData);

	// static members
	private:
		static int const SUPPORTED_FILE_VERSION = NW4R_FILE_VERSION(1, 3);

	// members
	private:
		WsdFile::Header		const *mHeader;		// size 0x04, offset 0x00
		WsdFile::DataBlock	const *mDataBlock;	// size 0x04, offset 0x04
		WsdFile::WaveBlock	const *mWaveBlock;	// size 0x04, offset 0x08
	}; // size 0x0c
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_WSD_FILE_H
