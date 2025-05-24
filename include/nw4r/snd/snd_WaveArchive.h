#ifndef NW4R_SND_WAVE_ARCHIVE_H
#define NW4R_SND_WAVE_ARCHIVE_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_Util.h"
#include "nw4r/snd/snd_WaveFile.h"

#include "nw4r/ut/ut_binaryFileFormat.h"

/*******************************************************************************
 * types
 */

namespace nw4r { namespace snd { namespace detail
{
	struct WaveArchive
	{
		/* Header */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x307f00
		struct FileHeader
		{
			ut::BinaryFileHeader	fileHeader;			// size 0x10, offset 0x00
			u32						tableChunkOffset;	// size 0x04, offset 0x10
			u32						tableChunkSize;		// size 0x04, offset 0x14
			u32						dataChunkOffset;	// size 0x04, offset 0x18
			u32						dataChunkSize;		// size 0x04, offset 0x1c
		}; // size 0x20

		/* TableBlock */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b289b
		struct TableItem
		{
			Util::DataRef<WaveFile::FileHeader>	waveFileRef;	// size 0x08, offset 0x00
			u32									waveFileSize;	// size 0x04, offset 0x08
		}; // size 0x0c

		static u32 const SIGNATURE_TABLE_BLOCK =
			NW4R_FOUR_BYTE('T', 'A', 'B', 'L');

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b29a1
		struct TableBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			Util::Table<TableItem>	fileTable;		// size 0x10, offset 0x08
		}; // size 0x18

		/* DataBlock */

		static u32 const SIGNATURE_DATA_BLOCK =
			NW4R_FOUR_BYTE('D', 'A', 'T', 'A');

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b2a3c
		struct DataBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			byte_t					data[1];		// size 0x01, offset 0x08
			/* 3 bytes padding */
		}; // size 0x0c

		/* WaveArchive */

		static u32 const SIGNATURE_FILE =
			NW4R_FOUR_BYTE('R', 'W', 'A', 'R');
		static int const FILE_VERSION = NW4R_FILE_VERSION(1, 0);
	}; // "namespace" WaveArchive
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2b2bb8
	class WaveArchiveReader
	{
	// methods
	public:
		// cdtors
		WaveArchiveReader(void const *waveArc);

		// methods
		WaveFile::FileHeader const *GetWaveFile(int index) const;

	private:
		bool VerifyFileHeader(void const *waveArc);

	// static members
	public:
		static u16 const SUPPORTED_FILE_VERSION_MAX = NW4R_FILE_VERSION(1, 0);
		static u16 const SUPPORTED_FILE_VERSION_MIN = NW4R_FILE_VERSION(1, 0);

	// members
	private:
		WaveArchive::TableBlock	const *mTableBlock;	// size 0x04, offset 0x00
		WaveArchive::DataBlock	const *mDataBlock;	// size 0x04, offset 0x04
	}; // size 0x08
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_WAVE_ARCHIVE_H
