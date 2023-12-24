#ifndef NW4R_SND_SOUND_ARCHIVE_H
#define NW4R_SND_SOUND_ARCHIVE_H
#include "ut_FileStream.h"
#include "types_nw4r.h"

namespace nw4r
{
	namespace snd
	{
		struct SoundArchive
		{
			struct Sound3DParam
			{
				UNKWORD WORD_0x0;
				u8 BYTE_0x4;
				u8 BYTE_0x5;
			};
			
			struct SoundArchivePlayerInfo
			{
				u32 SIZE_0x0;
				UNKWORD WORD_0x4;
				u32 SIZE_0x8;
				UNKWORD WORD_0xC;
				UNKWORD WORD_0x10;
				u32 SIZE_0x14;
				UNKWORD WORD_0x18;
			};
			
			struct SoundInfo
			{
				UNKWORD WORD_0x0;
				UNKWORD WORD_0x4;
				UNKWORD WORD_0x8;
				UNKWORD WORD_0xC;
				UNKWORD WORD_0x10;
				UNKWORD WORD_0x14;
				UNKWORD WORD_0x18;
			};
			
			struct SeqSoundInfo
			{
				UNKWORD WORD_0x0;
				UNKWORD WORD_0x4;
				UNKWORD WORD_0x8;
				UNKWORD WORD_0xC;
				bool BOOL_0x10;
			};
			
			struct StrmSoundInfo
			{
				
			};
			
			struct WaveSoundInfo
			{
				UNKWORD WORD_0x0;
				UNKWORD WORD_0x4;
				bool BOOL_0x8;
			};
			
			struct BankInfo
			{
				UNKWORD WORD_0x0;
			};
			
			struct PlayerInfo
			{
				UNKWORD WORD_0x0;
				UNKWORD WORD_0x4;
			};
			
			struct GroupInfo
			{
				u32 mCount; // at 0x0
				const char * mExternalFileName; // at 0x4
				u32 INT_0x8;
				u32 INT_0xC;
				u32 INT_0x10;
				u32 INT_0x14;
			};
			
			struct GroupItemInfo
			{
				UNKWORD WORD_0x0;
				u32 INT_0x4;
				u32 INT_0x8;
				u32 INT_0xC;
				UNKWORD WORD_0x10;
			};
			
			struct FileInfo
			{
				UNKWORD WORD_0x0;
				UNKWORD WORD_0x4;
				const char * mExternalFileName; // at 0x8
				u32 SIZE_0xC;
			};
			
			struct FilePos
			{
				u32 mGroupIndex; // at 0x0
				u32 mGroupItemIndex; // at 0x4
			};
			
			virtual ~SoundArchive(); // at 0x8
			virtual const void * detail_GetFileAddress(u32) const = 0; // at 0xc
			virtual const void * detail_GetWaveDataFileAddress(u32) const = 0; // at 0x10
			virtual int detail_GetRequiredStreamBufferSize() const = 0; // at 0x14
			virtual ut::FileStream * OpenStream(void *, int, u32, u32) const = 0; // at 0x18
			virtual ut::FileStream * OpenExtStream(void *, int, const char *, u32, u32) const = 0; // at 0x1c
			
			detail::SoundArchiveFileReader * mFileReader; // at 0x4
			char mExternalFileRoot[0x100]; // at 0x8
			
			SoundArchive();
			
			bool IsAvailable() const;
			
			void Setup(detail::SoundArchiveFileReader *);
			void Shutdown();
			
			u32 GetPlayerCount() const;
			u32 GetGroupCount() const;
			
			const char * GetSoundLabelString(u32) const;
			
			u32 ConvertLabelStringToSoundId(const char *) const;
			u32 ConvertLabelStringToPlayerId(const char *) const;
			u32 ConvertLabelStringToGroupId(const char *) const;
			
			UNKWORD GetSoundUserParam(u32) const;
			UNKWORD GetSoundType(u32) const;
			
			bool ReadSoundInfo(u32, SoundInfo *) const;
			
			bool detail_ReadSeqSoundInfo(u32, SeqSoundInfo *) const;
			bool detail_ReadStrmSoundInfo(u32, StrmSoundInfo *) const;
			bool detail_ReadWaveSoundInfo(u32, WaveSoundInfo *) const;
			
			bool ReadPlayerInfo(u32, PlayerInfo *) const;
			
			bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo *) const;
			
			bool detail_ReadSound3DParam(u32, Sound3DParam *) const;
			
			bool detail_ReadBankInfo(u32, BankInfo *) const;
			bool detail_ReadGroupInfo(u32, GroupInfo *) const;
			bool detail_ReadGroupItemInfo(u32, u32, GroupItemInfo *) const;
			bool detail_ReadFileInfo(u32, FileInfo *) const;
			bool detail_ReadFilePos(u32, u32, FilePos *) const;
			
			ut::FileStream * OpenExtStreamImpl(void *, int, const char *, u32, u32) const; //inlined
			ut::FileStream * detail_OpenFileStream(u32, void *, int) const;
			ut::FileStream * detail_OpenGroupStream(u32, void *, int) const;
			ut::FileStream * detail_OpenGroupWaveDataStream(u32, void *, int) const;
			
			UNKTYPE SetExternalFileRoot(const char *);
		};
	}
}

#endif