#ifndef NW4R_SND_SOUND_ARCHIVE_FILE_H
#define NW4R_SND_SOUND_ARCHIVE_FILE_H
#include "types_nw4r.h"
#include "ut_binaryFileFormat.h"
#include "snd_Util.h"
#include "snd_SoundArchive.h"

namespace nw4r
{
	namespace snd
	{
		namespace detail
		{
			namespace SoundArchiveFile
			{
				struct Header
				{
					ut::BinaryFileHeader mHeader; // at 0x0
					u32 mLabelStringChunkOffset; // at 0x10
					u32 mLabelStringChunkSize; // at 0x14
					u32 mInfoChunkOffset; // at 0x18
					u32 mInfoChunkSize; // at 0x1c
					UNKWORD UNK_0x20[0x8 / sizeof(UNKWORD)]; // at 0x20
				};
				
				struct Sound3DParam
				{
					UNKWORD WORD_0x0;
					u8 BYTE_0x4;
					u8 BYTE_0x5;
				};
				
				struct SeqSoundInfo
				{
					UNKWORD WORD_0x0;
					UNKWORD WORD_0x4;
					UNKWORD WORD_0x8;
					u8 BYTE_0xC;
					u8 BYTE_0xD;
				};
				
				struct StrmSoundInfo
				{
					
				};
				
				struct WaveSoundInfo
				{
					UNKWORD WORD_0x0;
					char UNK_0x4[0x4];
					u8 BYTE_0x8;
					u8 BYTE_0x9;
				};
				
				typedef Util::DataRef<void, SeqSoundInfo, StrmSoundInfo, WaveSoundInfo> SoundInfoOffset;
				
				struct SoundCommonInfo
				{
					u32 mSoundStringId; // at 0x0
					UNKWORD WORD_0x4;
					UNKWORD WORD_0x8;
					Util::DataRef<Sound3DParam, void, void, void> mSound3DParam; // at 0xc
					u8 BYTE_0x14;
					u8 BYTE_0x15;
					u8 mType; // at 0x16
					u8 BYTE_0x17;
					SoundInfoOffset mSoundInfoOffset; // at 0x18
					UNKWORD mSoundUserParam; // at 0x20
					char UNK_0x24[0x4];
					u8 BYTE_0x28;
					u8 BYTE_0x29;
				};
				
				typedef Util::Table<Util::DataRef<SoundCommonInfo, void, void, void> > SoundCommonTable;
				
				struct BankInfo
				{
					char UNK_0x0[0x4];
					UNKWORD WORD_0x4;
				};
				
				typedef Util::Table<Util::DataRef<BankInfo, void, void, void> > BankTable;
				
				struct PlayerInfo
				{
					char UNK_0x0[0x4];
					u8 BYTE_0x4;
					UNKWORD WORD_0x8;
				};
				
				typedef Util::Table<Util::DataRef<PlayerInfo, void, void, void> > PlayerTable;
				
				struct GroupItemInfo
				{
					UNKWORD WORD_0x0;
					u32 INT_0x4;
					u32 INT_0x8;
					u32 INT_0xC;
					UNKWORD WORD_0x10;
				};
				
				typedef Util::Table<Util::DataRef<GroupItemInfo, void, void, void> > GroupItemTable;
				
				struct GroupInfo
				{
					char UNK_0x0[0x8];
					Util::DataRef<char, void, void, void> mExternalFileName; // at 0x8
					u32 INT_0x10;
					u32 INT_0x14;
					u32 INT_0x18;
					u32 INT_0x1C;
					Util::DataRef<GroupItemTable, void, void, void> mGroupItemTable; // at 0x20
				};
				
				typedef Util::Table<Util::DataRef<GroupInfo, void, void, void> > GroupTable;
				
				struct SoundArchivePlayerInfo
				{
					u16 SIZE_0x0;
					u16 SHORT_0x2;
					u16 SIZE_0x4;
					u16 SHORT_0x6;
					u16 SHORT_0x8;
					u16 SIZE_0xA;
					u16 SHORT_0xC;
				};
				
				typedef Util::Table<Util::DataRef<SoundArchive::FilePos, void, void, void> > FilePosTable;
				
				struct FileInfo
				{
					UNKWORD WORD_0x0;
					UNKWORD WORD_0x4;
					char UNK_0x8[0x4];
					Util::DataRef<char, void, void, void> mExternalFileName; // at 0xc
					Util::DataRef<FilePosTable, void, void, void> TABLE_0x14;
				};
				
				typedef Util::Table<Util::DataRef<FileInfo, void, void, void> > FileTable;
				
				struct StringNode
				{
					enum
					{
						FLAG_LEAF = 0x0001
					};
					
					u16 mFlags; // at 0x0
					u16 SHORT_0x2;
					u32 mLeftIndex; // at 0x4
					u32 mRightIndex; // at 0x8
					u32 mStringId; // at 0xc
					u32 mId; // at 0x10
				};
				
				struct StringTree
				{
					u32 mRootIndex; // at 0x0
					u32 INT_0x4;
					StringNode mNodes[]; // at 0x8
				};
				
				struct StringChunk
				{
					u32 mTableOffset; // at 0x0
					u32 mSoundTreeOffset; // at 0x4
					u32 mPlayerTreeOffset; // at 0x8
					u32 mGroupTreeOffset; // at 0xc
					u32 OFFSET_0x10;
				};
				
				struct InfoChunk
				{
					Util::DataRef<SoundCommonTable, void, void, void> mSoundCommonTable; // at 0x0
					Util::DataRef<BankTable, void, void, void> mBankTable; // at 0x8
					Util::DataRef<PlayerTable, void, void, void> mPlayerTable; // at 0x10
					Util::DataRef<FileTable, void, void, void> mFileTable; // at 0x18
					Util::DataRef<GroupTable, void, void, void> mGroupTable; // at 0x20
					Util::DataRef<SoundArchivePlayerInfo, void, void, void> mSoundArchivePlayerInfo; // at 0x28
				};
			};
			
			struct SoundArchiveFileReader
			{
				struct StringBlock
				{
					ut::BinaryBlockHeader mHeader; // at 0x0
					SoundArchiveFile::StringChunk mChunk; // at 0x8
				};
				
				struct InfoBlock
				{
					ut::BinaryBlockHeader mHeader; // at 0x0
					SoundArchiveFile::InfoChunk mChunk; // at 0x8
				};
				
				SoundArchiveFile::Header mHeader; // at 0x0
				
				const SoundArchiveFile::InfoChunk * mInfoChunk; // at 0x28
				const SoundArchiveFile::StringChunk * mStringChunk; // at 0x2c
				const Util::Table<u32> * mStringTable; // at 0x30
				const SoundArchiveFile::StringTree * mSoundStringTree; // at 0x34
				const SoundArchiveFile::StringTree * mPlayerStringTree; // at 0x38
				const SoundArchiveFile::StringTree * mGroupStringTree; // at 0x3c
				const UNKTYPE * PTR_0x40;
				
				SoundArchiveFileReader();
				
				bool IsValidFileHeader(const void *); //inlined
				void Init(const void *);
				
				const void * GetPtrConst(const void *, u32) const; //inlined
				
				void SetStringChunk(const void *, u32);
				void SetInfoChunk(const void *, u32);
				
				UNKWORD GetSoundType(u32) const;
				
				const SoundArchiveFile::SoundCommonInfo * impl_GetSoundInfo(u32) const; //inlined
				bool ReadSoundInfo(u32, SoundArchive::SoundInfo *) const;
				bool ReadSound3DParam(u32, SoundArchive::Sound3DParam *) const;
				bool ReadSeqSoundInfo(u32, SoundArchive::SeqSoundInfo *) const;
				bool ReadStrmSoundInfo(u32, SoundArchive::StrmSoundInfo *) const;
				bool ReadWaveSoundInfo(u32, SoundArchive::WaveSoundInfo *) const;
				const SoundArchiveFile::BankInfo * impl_GetBankInfo(u32) const; //inlined
				bool ReadBankInfo(u32, SoundArchive::BankInfo *) const;
				const SoundArchiveFile::PlayerInfo * impl_GetPlayerInfo(u32) const; //inlined
				bool ReadPlayerInfo(u32, SoundArchive::PlayerInfo *) const;
				const SoundArchiveFile::GroupInfo * impl_GetGroupInfo(u32) const; //inlined
				bool ReadGroupInfo(u32, SoundArchive::GroupInfo *) const;
				bool ReadGroupItemInfo(u32, u32, SoundArchive::GroupItemInfo *) const;
				bool ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo *) const;
				
				u32 GetPlayerCount() const;
				u32 GetGroupCount() const;
				
				u32 GetSoundStringId(u32) const; //inlined
				const char * GetString(u32) const; //inlined
				const char * GetSoundLabelString(u32) const;
				UNKWORD GetSoundUserParam(u32) const;
				
				bool ReadFileInfo(u32, SoundArchive::FileInfo *) const;
				bool ReadFilePos(u32, u32, SoundArchive::FilePos *) const;
				
				u32 ConvertLabelStringToId(const SoundArchiveFile::StringTree *, const char *) const;
				
				SoundArchiveFile::SoundInfoOffset impl_GetSoundInfoOffset(u32) const;
				
				inline u16 GetVersion() const
				{
					return mHeader.mHeader.version;
				}
				
				inline u32 GetLabelStringChunkOffset() const
				{
					return mHeader.mLabelStringChunkOffset;
				}
				
				inline u32 GetLabelStringChunkSize() const
				{
					return mHeader.mLabelStringChunkSize;
				}
				
				inline u32 GetInfoChunkOffset() const
				{
					return mHeader.mInfoChunkOffset;
				}
				
				inline u32 GetInfoChunkSize() const
				{
					return mHeader.mInfoChunkSize;
				}
			};
		}
	}
}

#endif