#include "nw4r/snd/snd_BankFile.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_BankFile.cpp
 */

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_Util.h"
#include "nw4r/snd/snd_WaveArchive.h" // WaveArchiveReader
#include "nw4r/snd/snd_WaveFile.h"

#include "nw4r/ut/ut_binaryFileFormat.h" // ut::BinaryFileHeader
#include "nw4r/ut/ut_algorithm.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * local function declarations
 */

namespace nw4r { namespace snd { namespace detail
{
	inline byte_t ReadByte(void const *address)
	{
		return *static_cast<byte_t const *>(address);
	}
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd { namespace detail {

bool BankFileReader::IsValidFileHeader(void const *bankData)
{
	ut::BinaryFileHeader const *fileHeader =
		static_cast<ut::BinaryFileHeader const *>(bankData);

	NW4RAssertMessage_Line(
		59, fileHeader->signature == BankFile::SIGNATURE_FILE,
		"invalid file signature. bank data is not available.");

	if (fileHeader->signature != BankFile::SIGNATURE_FILE)
		return false;

	NW4RAssertMessage_Line(67, fileHeader->version >= NW4R_FILE_VERSION(1, 0),
	                       "bank file is not supported version.\n  please "
	                       "reconvert file using new version tools.\n");

	if (fileHeader->version < NW4R_FILE_VERSION(1, 0))
		return false;

	NW4RAssertMessage_Line(73, fileHeader->version <= SUPPORTED_FILE_VERSION,
	                       "bank file is not supported version.\n  please "
	                       "reconvert file using new version tools.\n");

	if (fileHeader->version > SUPPORTED_FILE_VERSION)
		return false;

	return true;
}

BankFileReader::BankFileReader(void const *bankData) :
	mHeader		(nullptr),
	mDataBlock	(nullptr),
	mWaveBlock	(nullptr)
{
	NW4RAssertPointerNonnull_Line(93, bankData);

	if (!IsValidFileHeader(bankData))
		return;

	mHeader = static_cast<BankFile::Header const *>(bankData);

	if (mHeader->dataBlockOffset)
	{
		mDataBlock = static_cast<BankFile::DataBlock const *>(
			ut::AddOffsetToPtr(mHeader, mHeader->dataBlockOffset));

		NW4RAssert_Line(105, mDataBlock->blockHeader.kind
		                         == BankFile::SIGNATURE_DATA_BLOCK);
	}

	if (mHeader->waveBlockOffset)
	{
		mWaveBlock = static_cast<BankFile::WaveBlock const *>(
			ut::AddOffsetToPtr(mHeader, mHeader->waveBlockOffset));

		NW4RAssert_Line(113, mWaveBlock->blockHeader.kind
		                         == BankFile::SIGNATURE_WAVE_BLOCK);
	}
}

BankFile::InstParam const *BankFileReader::GetInstParam(int prgNo, int key,
                                                        int velocity) const
{
	NW4RAssertPointerNonnull_Line(135, mHeader);
	if (!mHeader)
		return nullptr;

	if (prgNo < 0 || prgNo >= static_cast<int>(mDataBlock->instTable.count))
		return nullptr;

	BankFile::DataRegion const *ref = &mDataBlock->instTable.item[prgNo];

	if (ref->dataType == 4)
		return nullptr;

	if (ref->dataType != 1)
	{
		ref = GetReferenceToSubRegion(ref, key);
		if (!ref)
			return nullptr;
	}

	if (ref->dataType == 4)
		return nullptr;

	if (ref->dataType != 1)
	{
		ref = GetReferenceToSubRegion(ref, velocity);
		if (!ref)
			return nullptr;
	}

	if (ref->dataType != 1)
		return nullptr;

	BankFile::InstParam const *instParam =
		Util::GetDataRefAddress1(*ref, &mDataBlock->instTable);

	return instParam;
}

bool BankFileReader::ReadInstInfo(InstInfo *instInfo, int prgNo, int key,
                                  int velocity) const
{
	NW4RAssertPointerNonnull_Line(188, instInfo);

	BankFile::InstParam const *instParam = GetInstParam(prgNo, key, velocity);
	if (!instParam)
		return false;

	if (instParam->waveDataLocationType
	    == InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_INDEX)
	{
		if (instParam->waveIndex < 0)
			return false;

		instInfo->waveDataLocation.type =
			InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_INDEX;
		instInfo->waveDataLocation.index = instParam->waveIndex;
	}
	else if (instParam->waveDataLocationType
	         == InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_ADDRESS)
	{
		if (!instParam->waveInfoAddress)
			return false;

		instInfo->waveDataLocation.type =
			InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_ADDRESS;
		instInfo->waveDataLocation.address = instParam->waveInfoAddress;
	}
	else if (instParam->waveDataLocationType
	         == InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_CALLBACK)
	{
		if (!instParam->waveDataLocationCallback)
			return false;

		instInfo->waveDataLocation.type =
			InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_CALLBACK;
		instInfo->waveDataLocation.callback =
			instParam->waveDataLocationCallback;
	}
	else
	{
		NW4RPanicMessage_Line(210, "Invalid waveDataLocationType %d",
		                      instParam->waveDataLocationType);
		return false;
	}

	instInfo->attack		= instParam->attack;
	instInfo->hold			= instParam->hold;
	instInfo->decay			= instParam->decay;
	instInfo->sustain		= instParam->sustain;
	instInfo->release		= instParam->release;
	instInfo->originalKey	= instParam->originalKey;
	instInfo->pan			= instParam->pan;

	if (mHeader->fileHeader.version >= NW4R_FILE_VERSION(1, 1))
	{
		instInfo->volume	= instParam->volume;
		instInfo->tune		= instParam->tune;
	}
	else
	{
		instInfo->volume	= 127;
		instInfo->tune		= 1.0f;
	}

	switch (instParam->noteOffType)
	{
	case 0:
		instInfo->noteOffType = InstInfo::NOTE_OFF_TYPE_RELEASE;
		break;

	case 1:
		instInfo->noteOffType = InstInfo::NOTE_OFF_TYPE_IGNORE;
		break;

	default:
		NW4RPanicMessage_Line(240, "Invalid noteOffType %d",
		                      instParam->noteOffType);
		return false;
	}

	instInfo->alternateAssign = instParam->alternateAssign;

	return true;
}

BankFile::DataRegion const *BankFileReader::GetReferenceToSubRegion(
	BankFile::DataRegion const *ref, int splitKey) const
{
	BankFile::DataRegion const *subRef = nullptr;

	switch (ref->dataType)
	{
	case 0:
		break;

	case 1:
		subRef = ref;
		break;

	case 2:
	{
		BankFile::RangeTable const *table =
			Util::GetDataRefAddress2(*ref, &mDataBlock->instTable);

		if (!table)
			return nullptr;

		int index = 0;
		while (splitKey > ReadByte(table->key + index))
		{
			if (++index >= table->tableSize)
				return nullptr;
		}

		u32 refOffset = sizeof(BankFile::DataRegion) * index
		              + ut::RoundUp(table->tableSize + 1, 4);

		/* TODO: fake: how to properly match this call? (the arguments to
		 * AddOffsetToPtr are supposed to be the other way around)
		 */
		subRef = static_cast<BankFile::DataRegion const *>(
			ut::AddOffsetToPtr(reinterpret_cast<void const *>(refOffset),
		                       reinterpret_cast<u32>(table)));
	}
		break;

	case 3:
	{
		BankFile::IndexTable const *table =
			Util::GetDataRefAddress3(*ref, &mDataBlock->instTable);

		if (!table)
			return nullptr;

		if (splitKey < table->min || splitKey > table->max)
			return nullptr;

		subRef = reinterpret_cast<BankFile::DataRegion const *>(
			table->ref
			+ (splitKey - table->min) * sizeof(BankFile::DataRegion));
	}
		break;
	}

	return subRef;
}

bool BankFileReader::ReadWaveInfo(
	WaveInfo *waveParam, InstInfo::WaveDataLocation const &waveDataLocation,
	void const *waveDataAddress, WaveInfo const **waveInfoAddress) const
{
	NW4RAssertPointerNonnull_Line(331, waveParam);

	if (waveInfoAddress)
		*waveInfoAddress = nullptr;

	if (!mHeader)
		return false;

	if (waveDataLocation.type
	    == InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_INDEX)
	{
		int waveIndex = waveDataLocation.index;

		if (!mWaveBlock)
		{
			WaveArchiveReader waveArchiveReader(waveDataAddress);
			WaveFile::FileHeader const *fileHeader =
				waveArchiveReader.GetWaveFile(waveIndex);

			if (!fileHeader)
				return false;

			WaveFileReader waveFileReader(fileHeader);
			return waveFileReader.ReadWaveInfo(waveParam);
		}
		else if (waveIndex >= mWaveBlock->waveInfoTable.count)
		{
			return false;
		}
		else
		{
			WaveFile::WaveInfo const *waveInfo = Util::GetDataRefAddress0(
				mWaveBlock->waveInfoTable.item[waveIndex],
				&mWaveBlock->waveInfoTable);

			if (!waveInfo)
				return false;

			WaveFileReader waveFileReader(waveInfo);
			return waveFileReader.ReadWaveInfo(waveParam, waveDataAddress);
		}
	}
	else if (waveDataLocation.type
	         == InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_ADDRESS)
	{
		if (!waveDataLocation.address)
			return false;

		if (waveInfoAddress)
			*waveInfoAddress = waveDataLocation.address;

		*waveParam = *waveDataLocation.address;
		return true;
	}
	else if (waveDataLocation.type
	         == InstInfo::WaveDataLocation::WAVE_DATA_LOCATION_CALLBACK)
	{
		if (!waveDataLocation.callback)
			return false;

		WaveInfo *waveInfo = waveDataLocation.callback->at_0x08();
		if (!waveInfo)
			return false;

		if (waveInfoAddress)
			*waveInfoAddress = waveInfo;

		*waveParam = *waveInfo;
		return true;
	}
	else
	{
		NW4RPanicMessage_Line(389, "Invalid waveDataLocation.type %d",
		                      waveDataLocation.type);
		return false;
	}
}

}}} // namespace nw4r::snd::detail
