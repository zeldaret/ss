#ifndef NW4R_SND_SEQ_FILE_H
#define NW4R_SND_SEQ_FILE_H

/*******************************************************************************
 * headers
 */

#include <macros.h>
#include <types.h>

#include "Util.h" // Util::Table

#include "../ut/binaryFileFormat.h"

/*******************************************************************************
 * types
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2d23df (yes, really!)
	struct SeqFile
	{
		/* Header */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2ce38c
		struct Header
		{
			ut::BinaryFileHeader	fileHeader;			// size 0x10, offset 0x00
			u32						dataBlockOffset;	// size 0x04, offset 0x10
			u32						dataBlockSize;		// size 0x04, offset 0x14
			u32						labelBlockOffset;	// size 0x04, offset 0x18
			u32						labelBlockSize;		// size 0x04, offset 0x1c
		}; // size 0x20

		/* LabelBlock */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2ce87c
		struct LabelInfo
		{
			u32		offset;		// size 0x04, offset 0x00
			u32		nameLen;	// size 0x04, offset 0x04
			char	name[1];	// size 0x01, offset 0x08
			/* 3 bytes padding */
		}; // size 0x0c

		static byte4_t const SIGNATURE_LABEL_BLOCK;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2ce7dc
		struct LabelBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			Util::Table<u32>		labelInfoTable;	// size 0x08, offset 0x08
		}; // size 0x10

		/* DataBlock */

		static byte4_t const SIGNATURE_DATA_BLOCK =
			NW4R_FOUR_BYTE('D', 'A', 'T', 'A');

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2ce491
		struct DataBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			u32						baseOffset;		// size 0x04, offset 0x08
		}; // size 0x0c

		/* SeqFile */

		static byte4_t const SIGNATURE_FILE =
			NW4R_FOUR_BYTE('R', 'S', 'E', 'Q');
		static int const FILE_VERSION = NW4R_FILE_VERSION(1, 2);
	}; // "namespace" SeqFile
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2ce50a
	class SeqFileReader
	{
	// methods
	public:
		// cdtors
		SeqFileReader(void const *seqData);

		// methods
		void const *GetBaseAddress() const;

		bool IsValidFileHeader(void const *seqData);

		bool ReadOffsetByLabel(char const *labelName, u32 *offsetPtr) const;

	// static members
	public:
		static u16 const SUPPORTED_FILE_VERSION = NW4R_FILE_VERSION(1, 0);

	// members
	private:
		SeqFile::Header		const *mHeader;		// size 0x04, offset 0x00
		SeqFile::DataBlock	const *mDataBlock;	// size 0x04, offset 0x04
	}; // size 0x08
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_SEQ_FILE_H
