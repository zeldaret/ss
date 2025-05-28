#ifndef NW4R_SND_SOUND_ARCHIVE_FILE_H
#define NW4R_SND_SOUND_ARCHIVE_FILE_H

/*******************************************************************************
 * headers
 */

#include "common.h"

#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_Util.h"

#include "nw4r/ut/ut_binaryFileFormat.h"
#include "nw4r/ut/ut_algorithm.h"

/*******************************************************************************
 * types
 */

namespace nw4r { namespace snd { namespace detail
{
	struct SoundArchiveFile
	{
		/* Header */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x240a6
		struct Header
		{
			ut::BinaryFileHeader	fileHeader;			// size 0x10, offset 0x00
			u32						symbolDataOffset;	// size 0x04, offset 0x10
			u32						symbolDataSize;		// size 0x04, offset 0x14
			u32						infoOffset;			// size 0x04, offset 0x18
			u32						infoSize;			// size 0x04, offset 0x1c
			u32						fileImageOffset;	// size 0x04, offset 0x20
			u32						fileImageSize;		// size 0x04, offset 0x24
		}; // size 0x28

		static const int HEADER_AREA_SIZE = ROUND_UP(sizeof(Header), 32) + 40;

		/* SymbolBlock */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x24881
		struct StringTreeNode
		{
			u16	flags;		// size 0x02, offset 0x00
			u16	bit;		// size 0x02, offset 0x02
			u32		leftIdx;	// size 0x04, offset 0x04
			u32		rightIdx;	// size 0x04, offset 0x08
			u32		strIdx;		// size 0x04, offset 0x0c
			u32		id;			// size 0x04, offset 0x10
		}; // size 0x14

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x24a2e
		struct StringTree
		{
			u32							rootIdx;	// size 0x04, offset 0x00
			Util::Table<StringTreeNode>	nodeTable;	// size 0x18, offset 0x04
		}; // size 0x1c

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x24830
		struct StringTable
		{
			Util::Table<u32>	offsetTable;	// size 0x08, offset 0x00
		}; // size 0x08

		struct StringChunk
		{
			u32	tableOffset;		// size 0x04, offset 0x00
			u32	soundTreeOffset;	// size 0x04, offset 0x04
			u32	playerTreeOffset;	// size 0x04, offset 0x08
			u32	groupTreeOffset;	// size 0x04, offset 0x0c
			u32	bankTreeOffset;		// size 0x04, offset 0x10
		}; // size 0x14

		union StringBlock
		{
			StringTable	stringTable;	// size 0x08
			StringChunk	stringChunk;	// size 0x14
		}; // size 0x14

		static u32 const SIGNATURE_SYMB_BLOCK =
			NW4R_FOUR_BYTE('S', 'Y', 'M', 'B');

		struct SymbolBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			StringBlock				stringBlock;	// size 0x14, offset 0x08
		}; // size 0x20

		/* InfoBlock */

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2da8f9
		enum SoundType
		{
			SOUND_TYPE_INVALID,

			SOUND_TYPE_SEQ,
			SOUND_TYPE_STRM,
			SOUND_TYPE_WAVE,
		};

		struct Sound3DParam {
			u32 flags;
			u8 decayCurve;
			u8 decayRatio;
			u8 field_0x06;
		};

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25183
		struct SeqSoundInfo
		{
			u32		dataOffset;			// size 0x04, offset 0x00
			u32		bankId;				// size 0x04, offset 0x04
			u32		allocTrack;			// size 0x04, offset 0x08
			u8		channelPriority;	// size 0x01, offset 0x0c
			u8		releasePriorityFix;	// size 0x01, offset 0x0d
			u8	padding[2];
			u32	reserved;
		}; // size 0x14

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25089
		struct StrmSoundInfo
		{
			u32		startPosition;		// size 0x04, offset 0x00
			u16		allocChannelCount;	// size 0x02, offset 0x04
			u16	allocTrackFlag;		// size 0x02, offset 0x06
			u32	reserved;
		}; // size 0x0c

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x24f62
		struct WaveSoundInfo
		{
			s32		subNo;				// size 0x04, offset 0x00
			u32		allocTrack;			// size 0x04, offset 0x04
			u8		channelPriority;	// size 0x01, offset 0x08
			u8		releasePriorityFix;	// size 0x01, offset 0x09
			u8	padding[2];
			u32	reserved;
		}; // size 0x10

		typedef Util::DataRef<void, SeqSoundInfo, StrmSoundInfo, WaveSoundInfo>
			SoundInfoRef;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2546c
		struct SoundCommonInfo
		{
			u32							stringId;		// size 0x04, offset 0x00
			u32							fileId;			// size 0x04, offset 0x04
			u32							playerId;		// size 0x04, offset 0x08
			Util::DataRef<Sound3DParam>	param3dRef;		// size 0x08, offset 0x0c
			u8							volume;			// size 0x01, offset 0x14
			u8							playerPriority;	// size 0x01, offset 0x15
			u8							soundType;		// size 0x01, offset 0x16
			u8							remoteFilter;	// size 0x01, offset 0x17
			SoundInfoRef				soundInfoRef;	// size 0x08, offset 0x18
			u32					userParam[2];	// size 0x08, offset 0x20
			u8							panMode;		// size 0x01, offset 0x28
			u8							panCurve;		// size 0x01, offset 0x29
			u8							actorPlayerId;	// size 0x01, offset 0x2a
			u8						reserved;
		}; // size 0x2c

		typedef Util::Table<Util::DataRef<SoundCommonInfo> >
			SoundCommonInfoTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2db9b5
		struct SoundArchivePlayerInfo
		{
			u16		seqSoundCount;		// size 0x02, offset 0x00
			u16		seqTrackCount;		// size 0x02, offset 0x02
			u16		strmSoundCount;		// size 0x02, offset 0x04
			u16		strmTrackCount;		// size 0x02, offset 0x06
			u16		strmChannelCount;	// size 0x02, offset 0x08
			u16		waveSoundCount;		// size 0x02, offset 0x0a
			u16		waveTrackCount;		// size 0x02, offset 0x0c
			u16	padding;
			u32	reserved;
		}; // size 0x14

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x24eab
		struct BankInfo
		{
			u32		stringId;	// size 0x04, offset 0x00
			u32		fileId;		// size 0x04, offset 0x04
			u32	reserved;
		}; // size 0x0c

		typedef Util::Table<Util::DataRef<BankInfo> > BankInfoTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x24d8f
		struct PlayerInfo
		{
			u32		stringId;			// size 0x04, offset 0x00
			u8		playableSoundCount;	// size 0x01, offset 0x04
			u8	padding;
			u16	padding2;
			u32		heapSize;			// size 0x04, offset 0x08
			u32	reserved;
		}; // size 0x10

		typedef Util::Table<Util::DataRef<PlayerInfo> > PlayerInfoTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2db6e3
		struct GroupItemInfo
		{
			u32		fileId;			// size 0x04, offset 0x00
			u32		offset;			// size 0x04, offset 0x04
			u32		size;			// size 0x04, offset 0x08
			u32		waveDataOffset;	// size 0x04, offset 0x0c
			u32		waveDataSize;	// size 0x04, offset 0x10
			u32	reserved;
		}; // size 0x18

		typedef Util::Table<Util::DataRef<GroupItemInfo> > GroupItemInfoTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x24c1b
		struct GroupInfo
		{
			u32									stringId;		// size 0x04, offset 0x00
			s32									entryNum;		// size 0x04, offset 0x04
			Util::DataRef<char>					extFilePathRef;	// size 0x08, offset 0x08
			u32									offset;			// size 0x04, offset 0x10
			u32									size;			// size 0x04, offset 0x14
			u32									waveDataOffset;	// size 0x04, offset 0x18
			u32									waveDataSize;	// size 0x04, offset 0x1c
			Util::DataRef<GroupItemInfoTable>	itemTableRef;	// size 0x08, offset 0x20
		}; // size 0x28

		typedef Util::Table<Util::DataRef<GroupInfo> > GroupInfoTable;

		typedef Util::Table<Util::DataRef<SoundArchive::FilePos> > FilePosTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2dc40b
		struct FileInfo
		{
			u32							fileSize;			// size 0x04, offset 0x00
			u32							waveDataFileSize;	// size 0x04, offset 0x04
			s32							entryNum;			// size 0x04, offset 0x08
			Util::DataRef<char>			extFilePathRef;		// size 0x08, offset 0x0c
			Util::DataRef<FilePosTable>	filePosTableRef;	// size 0x08, offset 0x14
		}; // size 0x1c

		typedef Util::Table<Util::DataRef<FileInfo> > FileInfoTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x24665
		struct Info
		{
			Util::DataRef<SoundCommonInfoTable>		soundTableRef;				// size 0x08, offset 0x00
			Util::DataRef<BankInfoTable>			bankTableRef;				// size 0x08, offset 0x08
			Util::DataRef<PlayerInfoTable>			playerTableRef;				// size 0x08, offset 0x10
			Util::DataRef<FileInfoTable>			fileTableRef;				// size 0x08, offset 0x18
			Util::DataRef<GroupInfoTable>			groupTableRef;				// size 0x08, offset 0x20
			Util::DataRef<SoundArchivePlayerInfo>	soundArchivePlayerInfoRef;	// size 0x08, offset 0x28
		}; // size 0x30

		static u32 const SIGNATURE_INFO_BLOCK =
			NW4R_FOUR_BYTE('I', 'N', 'F', 'O');

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2da76d
		struct InfoBlock
		{
			ut::BinaryBlockHeader	blockHeader;	// size 0x08, offset 0x00
			Info					info;			// size 0x30, offset 0x08
		}; // size 0x38

		/* SoundArchiveFile */

		static u32 const SIGNATURE_FILE =
			NW4R_FOUR_BYTE('R', 'S', 'A', 'R');
		static int const FILE_VERSION = NW4R_FILE_VERSION(1, 4);
	}; // "namespace" SoundArchiveFile
}}} // namespace nw4r::snd::detail

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd { namespace detail
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x261f8
	class SoundArchiveFileReader
	{
	// methods
	public:
		// cdtors
		SoundArchiveFileReader();
		~SoundArchiveFileReader() {} // not default?

		// methods
		void Init(void const *soundArchiveData);

		u16 GetVersion() const { return mHeader.fileHeader.version; }
		u32 GetLabelStringChunkOffset() const
		{
			return mHeader.symbolDataOffset;
		}
		u32 GetLabelStringChunkSize() const { return mHeader.symbolDataSize; }
		u32 GetInfoChunkOffset() const { return mHeader.infoOffset; }
		u32 GetInfoChunkSize() const { return mHeader.infoSize; }

		char const *GetString(u32 id) const;
		SoundArchive::SoundType GetSoundType(u32 soundId) const;
		u32 GetSoundStringId(u32 id) const;
		u32 GetBankCount() const;
		u32 GetPlayerCount() const;
		u32 GetGroupCount() const;
		u32 GetFileCount() const;
		void const *GetPtrConst(void const *base, u32 offset) const
		{
			if (offset == 0)
				return nullptr;

			return ut::AddOffsetToPtr(base, offset);
		}

		void SetStringChunk(void const *stringChunk, u32 stringChunkSize);
		void SetInfoChunk(void const *infoChunk, u32 infoChunkSize);

		bool ReadSeqSoundInfo(u32 soundId,
		                      SoundArchive::SeqSoundInfo *info) const;
		bool ReadStrmSoundInfo(u32 soundId,
		                       SoundArchive::StrmSoundInfo *info) const;
		bool ReadWaveSoundInfo(u32 soundId,
		                       SoundArchive::WaveSoundInfo *info) const;
		bool ReadSoundArchivePlayerInfo(
			SoundArchive::SoundArchivePlayerInfo *info) const;
		bool ReadSoundInfo(u32 soundId, SoundArchive::SoundInfo *info) const;

		bool ReadBankInfo(u32 bankId, SoundArchive::BankInfo *info) const;

		bool ReadPlayerInfo(u32 playerId, SoundArchive::PlayerInfo *info) const;
		bool ReadSound3DParam(u32 soundId, nw4r::snd::SoundArchive::Sound3DParam* info) const;

		bool ReadGroupItemInfo(u32 groupId, u32 index,
		                       SoundArchive::GroupItemInfo *info) const;
		bool ReadGroupInfo(u32 groupId, SoundArchive::GroupInfo *info) const;

		bool ReadFileInfo(u32 fileId, SoundArchive::FileInfo *info) const;
		bool ReadFilePos(u32 fileId, u32 index,
		                 SoundArchive::FilePos *filePos) const;

		u32 ConvertLabelStringToId(SoundArchiveFile::StringTree const *tree,
		                           char const *str) const;
		const char* GetSoundLabelString(u32 id) const;
		u32 GetSoundUserParam(u32 id) const;

		u32 ConvertLabelStringToSoundId(const char* pLabel) const {
			return ConvertLabelStringToId(mStringTreeSound, pLabel);
		}
		u32 ConvertLabelStringToPlayerId(const char* pLabel) const {
			return ConvertLabelStringToId(mStringTreePlayer, pLabel);
		}
		u32 ConvertLabelStringToGroupId(const char* pLabel) const {
			return ConvertLabelStringToId(mStringTreeGroup, pLabel);
		}
		u32 ConvertLabelStringToBankId(const char* pLabel) const {
			return ConvertLabelStringToId(mStringTreeBank, pLabel);
		}

	private:
		static bool IsValidFileHeader(void const *soundArchiveData);

		SoundArchiveFile::SeqSoundInfo const *impl_GetSeqSoundInfo(
			u32 soundId) const;
		SoundArchiveFile::StrmSoundInfo const *impl_GetStrmSoundInfo(
			u32 soundId) const;
		SoundArchiveFile::WaveSoundInfo const *impl_GetWaveSoundInfo(
			u32 soundId) const;
		SoundArchiveFile::SoundCommonInfo const *impl_GetSoundInfo(
			u32 soundId) const;

		bool impl_GetSoundInfoOffset(
			u32 soundId, SoundArchiveFile::SoundInfoRef *soundInfoRef) const;

		SoundArchiveFile::BankInfo const *impl_GetBankInfo(u32 bankId) const;

		SoundArchiveFile::PlayerInfo const *impl_GetPlayerInfo(
			u32 playerId) const;

		SoundArchiveFile::GroupInfo const *impl_GetGroupInfo(u32 groupId) const;

	// static members
	public:
		static u16 const SUPPORTED_FILE_VERSION = NW4R_FILE_VERSION(1, 4);

	// members
	private:
		SoundArchiveFile::Header		mHeader;					// size 0x28, offset 0x00
		SoundArchiveFile::Info			const *mInfo;				// size 0x04, offset 0x28
		void							const *mStringBase;			// size 0x04, offset 0x2c
		SoundArchiveFile::StringTable	const *mStringTable;		// size 0x04, offset 0x30
		SoundArchiveFile::StringTree	const *mStringTreeSound;	// size 0x04, offset 0x34
		SoundArchiveFile::StringTree	const *mStringTreePlayer;	// size 0x04, offset 0x38
		SoundArchiveFile::StringTree	const *mStringTreeGroup;	// size 0x04, offset 0x3c
		SoundArchiveFile::StringTree	const *mStringTreeBank;		// size 0x04, offset 0x40
	}; // size 0x44
}}} // namespace nw4r::snd::detail

#endif // NW4R_SND_SOUND_ARCHIVE_FILE_H
