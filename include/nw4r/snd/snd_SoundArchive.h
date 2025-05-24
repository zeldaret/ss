#ifndef NW4R_SND_SOUND_ARCHIVE_H
#define NW4R_SND_SOUND_ARCHIVE_H

/*******************************************************************************
 * headers
 */

#include <types.h>

#include "global.h"

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class SoundArchiveFileReader; }}}

namespace nw4r { namespace ut { class FileStream; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x265b9
	class SoundArchive
	{
	// enums
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x26182
		enum SoundType
		{
			SOUND_TYPE_INVALID,

			SOUND_TYPE_SEQ,
			SOUND_TYPE_STRM,
			SOUND_TYPE_WAVE,
		};

	// nested types
	public:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25c18
		struct BankInfo
		{
			u32	fileId;	// size 0x04, offset 0x00
		}; // size 0x04

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25ac7
		struct FileInfo
		{
			u32		fileSize;			// size 0x04, offset 0x00
			u32		waveDataFileSize;	// size 0x04, offset 0x04
			char	const *extFilePath;	// size 0x04, offset 0x08
			u32		filePosCount;		// size 0x04, offset 0x0c
		}; // size 0x10

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25a5b
		struct FilePos
		{
			u32	groupId;	// size 0x04, offset 0x00
			u32	index;		// size 0x04, offset 0x04
		}; // size 0x08

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2593d
		struct GroupInfo
		{
			u32		itemCount;			// size 0x04, offset 0x00
			char	const *extFilePath;	// size 0x04, offset 0x04
			u32		offset;				// size 0x04, offset 0x08
			u32		size;				// size 0x04, offset 0x0c
			u32		waveDataOffset;		// size 0x04, offset 0x10
			u32		waveDataSize;		// size 0x04, offset 0x14
		}; // size 0x18

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2584d
		struct GroupItemInfo
		{
			u32	fileId;			// size 0x04, offset 0x00
			u32	offset;			// size 0x04, offset 0x04
			u32	size;			// size 0x04, offset 0x08
			u32	waveDataOffset;	// size 0x04, offset 0x0c
			u32	waveDataSize;	// size 0x04, offset 0x10
		}; // size 0x14

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25b9b
		struct PlayerInfo
		{
			int	playableSoundCount;	// size 0x04, offset 0x00
			u32	heapSize;			// size 0x04, offset 0x04
		}; // size 0x08

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25dc1
		struct SeqSoundInfo
		{
			u32		dataOffset;				// size 0x04, offset 0x00
			u32		bankId;					// size 0x04, offset 0x04
			u32		allocTrack;				// size 0x04, offset 0x08
			int		channelPriority;		// size 0x04, offset 0x0c
			bool	releasePriorityFixFlag;	// size 0x01, offset 0x10
			/* 3 bytes padding */
		}; // size 0x14

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25d0f
		struct StrmSoundInfo
		{
			u32		startPosition;		// size 0x04, offset 0x00
			u16		allocChannelCount;	// size 0x02, offset 0x04
			byte2_t	allocTrackFlag;		// size 0x02, offset 0x06
		}; // size 0x08

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x25c5f
		struct WaveSoundInfo
		{
			s32		subNo;					// size 0x04, offset 0x00
			int		channelPriority;		// size 0x04, offset 0x04
			bool	releasePriorityFixFlag;	// size 0x01, offset 0x08
			/* 3 bytes padding */
		}; // size 0x0c

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x256dc
		struct SoundArchivePlayerInfo
		{
			int	seqSoundCount;		// size 0x04, offset 0x00
			int	seqTrackCount;		// size 0x04, offset 0x04
			int	strmSoundCount;		// size 0x04, offset 0x08
			int	strmTrackCount;		// size 0x04, offset 0x0c
			int	strmChannelCount;	// size 0x04, offset 0x10
			int	waveSoundCount;		// size 0x04, offset 0x14
			int	waveTrackCount;		// size 0x04, offset 0x18
		}; // size 0x1c

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x26012
		struct SoundInfo
		{
			u32			fileId;			// size 0x04, offset 0x00
			u32			playerId;		// size 0x04, offset 0x04
			int			actorPlayerId;	// size 0x04, offset 0x08
			int			playerPriority;	// size 0x04, offset 0x0c
			int			volume;			// size 0x04, offset 0x10
			int			remoteFilter;	// size 0x04, offset 0x14
			PanMode		panMode;		// size 0x04, offset 0x18
			PanCurve	panCurve;		// size 0x04, offset 0x1c
		}; // size 0x20

	// methods
	public:
		// cdtors
		SoundArchive();
		virtual ~SoundArchive();

		// virtual function ordering
		// vtable SoundArchive
		virtual void const *detail_GetFileAddress(u32 fileId) const = 0;
		virtual void const *detail_GetWaveDataFileAddress(u32 fileId) const = 0;
		virtual int detail_GetRequiredStreamBufferSize() const = 0;
		virtual ut::FileStream *OpenStream(void *buffer, int size, u32 begin,
		                                   u32 length) const = 0;
		virtual ut::FileStream *OpenExtStream(void *buffer, int size,
		                                      char const *extFilePath,
		                                      u32 begin, u32 length) const = 0;

		// methods
		void Setup(detail::SoundArchiveFileReader *fileReader);
		void Shutdown();

		u32 GetPlayerCount() const;
		u32 GetGroupCount() const;
		bool IsAvailable() const;
		SoundType GetSoundType(u32 soundId) const;

		u32 ConvertLabelStringToSoundId(char const *label) const;

		bool ReadSoundInfo(u32 soundId, SoundInfo *info) const;
		bool ReadSeqSoundInfo(u32 soundId, SeqSoundInfo *info) const;
		bool detail_ReadStrmSoundInfo(u32 soundId, StrmSoundInfo *info) const;
		bool detail_ReadWaveSoundInfo(u32 soundId, WaveSoundInfo *info) const;

		bool ReadPlayerInfo(u32 playerId, PlayerInfo *info) const;
		bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo *info) const;

		bool ReadBankInfo(u32 bankId, BankInfo *info) const;

		bool detail_ReadGroupInfo(u32 groupId, GroupInfo *info) const;
		bool detail_ReadGroupItemInfo(u32 groupId, u32 index,
		                              GroupItemInfo *info) const;

		u32 detail_GetFileCount() const;
		bool detail_ReadFileInfo(u32 fileId, FileInfo *info) const;
		bool detail_ReadFilePos(u32 fileId, u32 index, FilePos *info) const;

		ut::FileStream *detail_OpenFileStream(u32 fileId, void *buffer,
		                                      int size) const;

	private:
		ut::FileStream *OpenExtStreamImpl(void *buffer, int size,
		                                  char const *extFilePath, u32 begin,
		                                  u32 length) const;

	// static members
	public:
		static int const FILE_PATH_MAX = 255;
		static u32 const INVALID_ID = -1;

	// members
	private:
		/* vtable */														// size 0x004, offset 0x000
		detail::SoundArchiveFileReader	*mFileReader;						// size 0x004, offset 0x004
		char							mExtFileRoot[FILE_PATH_MAX + 1];	// size 0x100, offset 0x008
	}; // size 0x108
}} // namespace nw4r::snd

#endif // NW4R_SND_SOUND_ARCHIVE_H
