#include "nw4r/snd/snd_SeqFile.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SeqFile.cpp
 */

/*******************************************************************************
 * headers
 */

#include <cstring>

#include "common.h"

#include "nw4r/snd/snd_Util.h"

#include "nw4r/ut/ut_binaryFileFormat.h"
#include "nw4r/ut/ut_algorithm.h" // ut::AddOffsetToPtr

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

bool SeqFileReader::IsValidFileHeader(void const *seqData)
{
	ut::BinaryFileHeader const *fileHeader =
		static_cast<ut::BinaryFileHeader const *>(seqData);

	NW4RAssertMessage_Line(
		43, fileHeader->signature == SeqFile::SIGNATURE_FILE,
		"invalid file signature. seq data is not available.");

	if (fileHeader->signature != SeqFile::SIGNATURE_FILE)
		return false;

	u16 version = Util::ReadBigEndian(fileHeader->version);

	NW4RAssertMessage_Line(
		51, version >= NW4R_FILE_VERSION(1, 0),
		"seq file is not supported version.\n"
		"  please reconvert file using new version tools.\n");

	version = Util::ReadBigEndian(fileHeader->version); // ? again?
	if (version < NW4R_FILE_VERSION(1, 0))
		return false;

	NW4RAssertMessage_Line(
		59, version <= SUPPORTED_FILE_VERSION,
		"seq file is not supported version.\n"
		"  please reconvert file using new version tools.\n");

	if (version > SUPPORTED_FILE_VERSION)
		return false;

	return true;
}

SeqFileReader::SeqFileReader(void const *seqData) :
	mHeader		(nullptr),
	mDataBlock	(nullptr)
{
	NW4RAssertPointerNonnull_Line(78, seqData);

	if (!IsValidFileHeader(seqData))
		return;

	mHeader = static_cast<SeqFile::Header const *>(seqData);
	mDataBlock = static_cast<SeqFile::DataBlock const *>(ut::AddOffsetToPtr(
		mHeader, Util::ReadBigEndian(mHeader->dataBlockOffset)));

	NW4RAssert_Line(87, mDataBlock->blockHeader.kind
	                        == SeqFile::SIGNATURE_DATA_BLOCK);
}

void const *SeqFileReader::GetBaseAddress() const
{
	NW4RAssertPointerNonnull_Line(101, mHeader);

	return ut::AddOffsetToPtr(mDataBlock,
	                          Util::ReadBigEndian(mDataBlock->baseOffset));
}

bool SeqFileReader::ReadOffsetByLabel(char const *labelName,
                                      u32 *offsetPtr) const
{
	NW4RAssertPointerNonnull_Line(117, offsetPtr);

	// NOTE: reinterpret_cast necessary instead of static_cast for regalloc(???)
	SeqFile::LabelBlock const *labelBlock =
		reinterpret_cast<SeqFile::LabelBlock const *>(ut::AddOffsetToPtr(
			mHeader, Util::ReadBigEndian(mHeader->labelBlockOffset)));

	if (!labelBlock)
		return false;

	u32 labelNameLen = std::strlen(labelName);

	for (int index = 0;
	     index < Util::ReadBigEndian(labelBlock->labelInfoTable.count); ++index)
	{
		u32 ofs = labelBlock->labelInfoTable.item[index];

		// NOTE: reinterpret_cast necessary here too
		SeqFile::LabelInfo const *labelInfo =
			reinterpret_cast<SeqFile::LabelInfo const *>(
				ut::AddOffsetToPtr(labelBlock, Util::ReadBigEndian(ofs) + 8));

		NW4RAssertPointerNonnull_Line(133, labelInfo);

		if (labelNameLen == Util::ReadBigEndian(labelInfo->nameLen)
		    && std::strncmp(labelName, labelInfo->name, labelNameLen) == 0)
		{
			*offsetPtr = Util::ReadBigEndian(labelInfo->offset);
			return true;
		}
	}

	return false;
}

}}} // namespace nw4r::snd::detail
