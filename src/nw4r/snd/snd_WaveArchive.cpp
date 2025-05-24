#include "nw4r/snd/WaveArchive.h"

/*******************************************************************************
 * headers
 */

#include <types.h> // nullptr

#include "nw4r/snd/Util.h"
#include "nw4r/snd/WaveFile.h"

#include "nw4r/ut/binaryFileFormat.h" // ut::BinaryFileHeader
#include "nw4r/ut/inlines.h" // ut::AddOffsetToPtr

#include <nw4r/NW4RAssert.hpp>

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

WaveArchiveReader::WaveArchiveReader(void const *waveArc) :
	mTableBlock	(nullptr),
	mDataBlock	(nullptr)
{
	NW4RAssertPointerNonnull_Line(44, waveArc);

	if (!VerifyFileHeader(waveArc))
		return;

	WaveArchive::FileHeader const *fileHeader =
		static_cast<WaveArchive::FileHeader const *>(waveArc);

	WaveArchive::TableBlock const *tableBlock =
		static_cast<WaveArchive::TableBlock const *>(
			ut::AddOffsetToPtr(fileHeader, fileHeader->tableChunkOffset));

	NW4RAssert_Line(53, tableBlock->blockHeader.kind
	                        == WaveArchive::SIGNATURE_TABLE_BLOCK);

	WaveArchive::DataBlock const *dataBlock =
		static_cast<WaveArchive::DataBlock const *>(
			ut::AddOffsetToPtr(fileHeader, fileHeader->dataChunkOffset));

	NW4RAssert_Line(58, dataBlock->blockHeader.kind
	                        == WaveArchive::SIGNATURE_DATA_BLOCK);

	mTableBlock	= tableBlock;
	mDataBlock	= dataBlock;
}

WaveFile::FileHeader const *WaveArchiveReader::GetWaveFile(int index) const
{
	if (!mTableBlock)
		return nullptr;

	if (!mDataBlock)
		return nullptr;

	if (index < 0)
		return nullptr;

	if (index >= mTableBlock->fileTable.count)
		return nullptr;

	Util::DataRef<WaveFile::FileHeader> waveFileRef =
		mTableBlock->fileTable.item[index].waveFileRef;

	WaveFile::FileHeader const *fileHeader =
		Util::GetDataRefAddress0(waveFileRef, mDataBlock);

	return fileHeader;
}

bool WaveArchiveReader::VerifyFileHeader(void const *waveArc)
{
	ut::BinaryFileHeader const *fileHeader =
		static_cast<ut::BinaryFileHeader const *>(waveArc);

	NW4RAssertMessage_Line(
		92, fileHeader->signature == WaveArchive::SIGNATURE_FILE,
		"invalid file signature. wave archive is not available.");

	if (fileHeader->signature != WaveArchive::SIGNATURE_FILE)
		return false;

	// specifically not the source variant
	NW4RAssertHeaderClampedLRValue_Line(96, fileHeader->version,
	                                    SUPPORTED_FILE_VERSION_MIN,
	                                    SUPPORTED_FILE_VERSION_MAX);

	if (fileHeader->version < SUPPORTED_FILE_VERSION_MIN)
		return false;

	if (fileHeader->version > SUPPORTED_FILE_VERSION_MAX)
		return false;

	return true;
}

}}} // namespace nw4r::snd::detail
