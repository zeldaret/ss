#ifndef NW4R_SND_BANK_FILE_H
#define NW4R_SND_BANK_FILE_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_WaveFile.h"

#include "nw4r/snd/snd_Util.h"

#include "nw4r/ut/ut_binaryFileFormat.h"

/*******************************************************************************
 * types
 */

namespace nw4r { namespace snd { namespace detail
{
	struct BankFile
	{
		/* Header */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2afe21
		struct Header
		{
			ut::BinaryFileHeader	fileHeader;			// size 0x10, offset 0x00
			u32						dataBlockOffset;	// size 0x04, offset 0x10
			u32						dataBlockSize;		// size 0x04, offset 0x14
			u32						waveBlockOffset;	// size 0x04, offset 0x18
			u32						waveBlockSize;		// size 0x04, offset 0x1c
		}; // size 0x20

		/* DataBlock */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b0666
		struct InstParam
		{
			// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b04df
			union
			{
				s32							waveIndex;					// size 0x04
				WaveInfo					const *waveInfoAddress;		// size 0x04
				WaveDataLocationCallback	*waveDataLocationCallback;	// size 0x04
			}; // size 0x04, offset 0x00
			u8					attack;					// size 0x01, offset 0x04
			u8					decay;					// size 0x01, offset 0x05
			u8					sustain;				// size 0x01, offset 0x06
			u8					release;				// size 0x01, offset 0x07
			u8					hold;					// size 0x01, offset 0x08
			u8					waveDataLocationType;	// size 0x01, offset 0x09
			u8					noteOffType;			// size 0x01, offset 0x0a
			u8					alternateAssign;		// size 0x01, offset 0x0b
			u8					originalKey;			// size 0x01, offset 0x0c
			u8					volume;					// size 0x01, offset 0x0d
			u8					pan;					// size 0x01, offset 0x0e
			u8				padding2; // 2?
			f32					tune;					// size 0x04, offset 0x10
			// TODO: template parameters
			Util::DataRef<void>	lfoTableRef;			// size 0x08, offset 0x14
			Util::DataRef<void>	graphEnvTablevRef;		// size 0x08, offset 0x1c
			Util::DataRef<void>	randomizerTableRef;		// size 0x08, offset 0x24
			u32				reserved;
		}; // size 0x30

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b2114
		struct RangeTable
		{
			u8	tableSize;	// size 0x01, offset 0x00
			u8	key[];		// flexible,  offset 0x01 (unit size 0x01)
		}; // size 0x01

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b203e
		struct IndexTable
		{
			u8		min;		// size 0x01, offset 0x00
			u8		max;		// size 0x01, offset 0x01
			u16	reserved;
			byte_t	ref[];		// flexible,  offset 0x04 (unit size 0x01)
		}; // size 0x04

		typedef Util::DataRef<void, InstParam, RangeTable, IndexTable>
			DataRegion;

		static u32 const SIGNATURE_DATA_BLOCK =
			NW4R_FOUR_BYTE('D', 'A', 'T', 'A');

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b006b
		struct DataBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			Util::Table<DataRegion>	instTable;		// size 0x0c, offset 0x08
		}; // size 0x14

		/* WaveBlock */

		static u32 const SIGNATURE_WAVE_BLOCK =
			NW4R_FOUR_BYTE('W', 'A', 'V', 'E');
		typedef Util::DataRef<WaveFile::WaveInfo> WaveRegion;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b022c
		struct WaveBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			Util::Table<WaveRegion>	waveInfoTable;	// size 0x0c, offset 0x08
		}; // size 0x14

		/* BankFile */

		static u32 const SIGNATURE_FILE =
			NW4R_FOUR_BYTE('R', 'B', 'N', 'K');
		static int const FILE_VERSION = NW4R_FILE_VERSION(1, 2);
	}; // "namespace" BankFile

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b0cdd
	struct InstInfo
	{
	// enums
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b0c84
		enum NoteOffType
		{
			NOTE_OFF_TYPE_RELEASE,
			NOTE_OFF_TYPE_IGNORE,
		};

	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b03e4
		struct WaveDataLocation
		{
		// enums
		public:
			// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b02aa
			enum WaveDataLocationType
			{
				WAVE_DATA_LOCATION_INDEX,
				WAVE_DATA_LOCATION_ADDRESS,
				WAVE_DATA_LOCATION_CALLBACK,
			};

		// members
		public:
			WaveDataLocationType	type;	// size 0x04, offset 0x00
			// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b0335
			union
			{
				s32							index;			// size 0x04
				WaveInfo					const *address;	// size 0x04
				WaveDataLocationCallback	*callback;		// size 0x04
			}; // size 0x04, offset 0x04
		}; // size 0x08

	// members
	public:
		WaveDataLocation	waveDataLocation;	// size 0x08, offset 0x00
		u8					attack;				// size 0x01, offset 0x08
		u8					hold;				// size 0x01, offset 0x09
		u8					decay;				// size 0x01, offset 0x0a
		u8					sustain;			// size 0x01, offset 0x0b
		u8					release;			// size 0x01, offset 0x0c
		/* 3 bytes padding */
		NoteOffType			noteOffType;		// size 0x04, offset 0x10
		u8					alternateAssign;	// size 0x01, offset 0x14
		u8					originalKey;		// size 0x01, offset 0x15
		u8					pan;				// size 0x01, offset 0x16
		u8					volume;				// size 0x01, offset 0x17
		f32					tune;				// size 0x04, offset 0x18
	}; // size 0x1c
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b0a17
	class BankFileReader
	{
	// methods
	public:
		// cdtors
		BankFileReader(void const *bankData);

		// methods
		bool ReadInstInfo(InstInfo *instInfo, int prgNo, int key,
		                  int velocity) const;
		bool ReadWaveInfo(WaveInfo *waveParam,
		                  InstInfo::WaveDataLocation const &waveDataLocation,
		                  void const *waveDataAddress,
		                  WaveInfo const **waveInfoAddress) const;

	private:
		BankFile::InstParam const *GetInstParam(int prgNo, int key,
		                                        int velocity) const;
		BankFile::DataRegion const *GetReferenceToSubRegion(
			BankFile::DataRegion const *ref, int splitKey) const;

		static bool IsValidFileHeader(void const *bankData);

	// static members
	public:
		static int const SUPPORTED_FILE_VERSION = NW4R_FILE_VERSION(1, 2);

	// members
	private:
		BankFile::Header	const *mHeader;		// size 0x04, offset 0x00
		BankFile::DataBlock	const *mDataBlock;	// size 0x04, offset 0x04
		BankFile::WaveBlock	const *mWaveBlock;	// size 0x04, offset 0x08
	}; // size 0x0c
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_BANK_FILE_H
