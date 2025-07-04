#ifndef NW4R_SND_SOUND_ARCHIVE_PLAYER_H
#define NW4R_SND_SOUND_ARCHIVE_PLAYER_H

/*******************************************************************************
 * headers
 */

#include "common.h"

// WARNING: DO NOT REORDER these #include directives, data pooling depends on it

// clang-format off
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_DisposeCallbackManager.h" // detail::DisposeCallback
#include "nw4r/snd/snd_MmlParser.h"
#include "nw4r/snd/snd_NoteOnCallback.h" // This needs to be
#include "nw4r/snd/snd_MmlSeqTrackAllocator.h" // before this
#include "nw4r/snd/snd_SeqPlayer.h"
#include "nw4r/snd/snd_SeqSound.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundInstanceManager.h"
#include "nw4r/snd/snd_SoundMemoryAllocatable.h"
#include "nw4r/snd/snd_WsdPlayer.h" // and this needs to be
#include "nw4r/snd/snd_SoundStartable.h" // before this
#include "nw4r/snd/snd_StrmChannel.h" // detail::StrmBufferPool
#include "nw4r/snd/snd_StrmSound.h"
#include "nw4r/snd/snd_Util.h" // Util::Table
#include "nw4r/snd/snd_WaveFile.h"
#include "nw4r/snd/snd_WaveSound.h"
// clang-format on

#include <nw4r/NW4RAssert.hpp>

/*******************************************************************************
 * types
 */

// forward declarations
namespace nw4r { namespace snd { namespace detail { class PlayerHeap; }}}
namespace nw4r { namespace snd { namespace detail { class SeqTrackAllocator; }}}
namespace nw4r { namespace snd { class SoundActor; }}
namespace nw4r { namespace snd { class SoundHandle; }}
namespace nw4r { namespace snd { class SoundPlayer; }}

/*******************************************************************************
 * classes and functions
 */

namespace nw4r { namespace snd
{
	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x28a19
	class SoundArchivePlayer_FileManager
	{
	// methods
	public:
		// virtual function ordering
		// vtable SoundArchivePlayer_FileManager
		virtual void const *GetFileAddress(u32) = 0;
		virtual void const *GetFileWaveDataAddress(u32) = 0;

	// members
	private:
		/* vtable */	// size 0x04, offset 0x00
	}; // size 0x04

	// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x31598
	class SoundArchivePlayer : public detail::DisposeCallback,
	                           public SoundStartable
	{
	// nested types
	private:
		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e0ad7
		struct GroupAddress
		{
			void	const *address;			// size 0x04, offset 0x00
			void	const *waveDataAddress;	// size 0x04, offset 0x04
		}; // size 0x08

		typedef detail::Util::Table<GroupAddress> GroupAddressTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e0bdd
		struct FileAddress
		{
			void	const *address;			// size 0x04, offset 0x00
			void	const *waveDataAddress;	// size 0x04, offset 0x04
		}; // size 0x08

		typedef detail::Util::Table<FileAddress> FileAddressTable;

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e4f9
		class SeqNoteOnCallback : public detail::NoteOnCallback
		{
		// methods
		public:
			// cdtors
			SeqNoteOnCallback(SoundArchivePlayer const &player) :
				mSoundArchivePlayer(player)
			{
			}

			virtual ~SeqNoteOnCallback() {}

			// virtual function ordering
			// vtable NoteOnCallback
			virtual detail::Channel *NoteOn(
				detail::SeqPlayer *seqPlayer, int bankNo,
				detail::NoteOnInfo const &noteOnInfo);

			// members
		private:
			/* base NoteOnCallback */						// size 0x04, offset 0x00
			SoundArchivePlayer	const &mSoundArchivePlayer;	// size 0x04, offset 0x04
		}; // size 0x08

		// [R89JEL]:/bin/RVL/Debug/mainD.elf:.debug::0x2e8e1
		class WsdCallback : public detail::WsdPlayer::WsdCallback
		{
		// methods
		public:
			// cdtors
			WsdCallback(SoundArchivePlayer const &player) :
				mSoundArchivePlayer(player)
			{
			}
			virtual ~WsdCallback() {}

			// virtual function ordering
			// vtable detail::WsdPlayer::WsdCallback
			virtual bool GetWaveSoundData(detail::WaveSoundInfo *info,
			                              detail::WaveSoundNoteInfo *noteInfo,
			                              detail::WaveInfo *waveData,
			                              void const *waveSoundData, int index,
			                              int noteIndex,
			                              u32 userData) const;

		// members
		private:
			/* base detail::WsdPlayer::WsdCallback */		// size 0x04, offset 0x00
			SoundArchivePlayer	const &mSoundArchivePlayer;	// size 0x04, offset 0x04
		}; // size 0x08

	// methods
	public:
		// cdtors
		SoundArchivePlayer();
		virtual ~SoundArchivePlayer();

		// virtual function ordering
		// vtable detail::DisposeCallback
		virtual void InvalidateData(void const *start, void const *end);
		virtual void InvalidateWaveData(void const *start, void const *end);

		// vtable SoundStartable
		// WARNING: DO NOT REORDER these declarations
		virtual u32 detail_ConvertLabelStringToSoundId(char const *label)
		{
			// specifically not the header variant
			NW4RAssertPointerNonnull_Line(194, mSoundArchive);

			return mSoundArchive->ConvertLabelStringToSoundId(label);
		}
		virtual StartResult detail_SetupSound(SoundHandle *handle, u32 soundId,
		                                      bool holdFlag,
		                                      StartInfo const *startInfo);

		// methods
		bool Setup(SoundArchive const *arc, void *buffer, u32 size,
		           void *strmBuffer, u32 strmBufferSize);
		void Shutdown();

		bool LoadGroup(u32 id, SoundMemoryAllocatable *pAllocatable, u32 blockSize);
		bool LoadGroup(const char *pLabel, SoundMemoryAllocatable *pAllocatable, u32 blockSize);

		bool LoadGroup(int id, SoundMemoryAllocatable *pAllocatable, u32 blockSize) {
			return LoadGroup(static_cast<u32>(id), pAllocatable, blockSize);
		}
		bool LoadGroup(unsigned int id, SoundMemoryAllocatable *pAllocatable, u32 blockSize) {
			return LoadGroup(static_cast<u32>(id), pAllocatable, blockSize);
		}

		bool IsLoadedGroup(u32 groupId) const;

		SoundArchive const &GetSoundArchive() const;
		u32 GetSoundPlayerCount() const { return mSoundPlayerCount; }
		SoundPlayer &GetSoundPlayer(u32 playerId);

		bool IsAvailable() const;
		u32 GetRequiredMemSize(SoundArchive const *arc);
		u32 GetRequiredStrmBufferSize(SoundArchive const *arc);
		void const *GetGroupAddress(u32 groupId) const;
		void SetGroupAddress(u32 id, const void* pAddr);
		void const *GetGroupWaveDataAddress(u32 groupId) const;
		void SetGroupWaveDataAddress(u32 id, const void* pAddr);
		void const *GetFileAddress(u32 fileId) const;
		void const *GetFileWaveDataAddress(u32 fileId) const;
		void const *detail_GetFileAddress(u32 fileId) const;
		void const *detail_GetFileWaveDataAddress(u32 fileId) const;

		bool SetupMram(SoundArchive const *arc, void *buffer, u32 size);
		bool SetupStrmBuffer(SoundArchive const *arc, void *buffer, u32 size);
		bool SetupSoundPlayer(SoundArchive const *arc, void **buffer,
		                      void *endp);

		bool SetupSeqSound(SoundArchive const *arc, int numSounds,
		                   void **buffer, void *endp);
		bool SetupWaveSound(SoundArchive const *arc, int numSounds,
		                    void **buffer, void *endp);
		bool SetupStrmSound(SoundArchive const *arc, int numSounds,
		                    void **buffer, void *endp);
		bool SetupSeqTrack(SoundArchive const *arc, int numTracks,
		                   void **buffer, void *endp);

		bool CreateGroupAddressTable(SoundArchive const *arc, void **buffer,
		                             void *endp);
		bool CreateFileAddressTable(SoundArchive const *arc, void **buffer,
		                            void *endp);

		detail::PlayerHeap *CreatePlayerHeap(void **buffer, void *endp,
		                                     u32 heapSize);

		void Update();
		void UpdateCommonSoundParam(detail::BasicSound *sound,
		                            SoundArchive::SoundInfo const *commonInfo);

		SoundStartable::StartResult detail_SetupSoundImpl(
			SoundHandle *handle, u32 soundId,
			detail::BasicSound::AmbientInfo *ambientArgInfo, SoundActor *actor,
			bool holdFlag, SoundStartable::StartInfo const *startInfo);

		// Made up
		void detail_SetFileManager(SoundArchivePlayer_FileManager *manager) {
			mFileManager = manager;
		}

	private:
		template <typename Sound>
		Sound *AllocSound(
			detail::SoundInstanceManager<Sound> *manager, u32 soundId,
			int priority, int ambientPriority,
			detail::BasicSound::AmbientInfo *ambientArgInfo);

		SoundStartable::StartResult PrepareSeqImpl(
			detail::SeqSound *sound, SoundArchive::SoundInfo const *commonInfo,
			SoundArchive::SeqSoundInfo const *info,
			SoundStartable::StartInfo::StartOffsetType startOffsetType,
			int startOffset, void const *externalSeqDataAddress,
			char const *externalSeqStartLabel);
		SoundStartable::StartResult PrepareStrmImpl(
			detail::StrmSound *sound, SoundArchive::SoundInfo const *commonInfo,
			SoundArchive::StrmSoundInfo const *info,
			SoundStartable::StartInfo::StartOffsetType startOffsetType,
			int startOffset);
		SoundStartable::StartResult PrepareWaveSoundImpl(
			detail::WaveSound *sound, SoundArchive::SoundInfo const *commonInfo,
			SoundArchive::WaveSoundInfo const *info,
			SoundStartable::StartInfo::StartOffsetType startOffsetType,
			int startOffset);

	// static members
	public:
		static int const DEFAULT_STREAM_BLOCK_COUNT;

	// members
	private:
		/* base detail::DisposeCallback */											// size 0x0c, offset 0x00
		/* base SoundStartable */													// size 0x04, offset 0x0c
		SoundArchive									const *mSoundArchive;		// size 0x04, offset 0x10
		GroupAddressTable								*mGroupTable;				// size 0x04, offset 0x14
		FileAddressTable								*mFileTable;				// size 0x04, offset 0x18
		SoundArchivePlayer_FileManager					*mFileManager;				// size 0x04, offset 0x1c
		SeqNoteOnCallback								mSeqCallback;				// size 0x08, offset 0x20
		WsdCallback										mWsdCallback;				// size 0x08, offset 0x28
		detail::SeqTrackAllocator						*mSeqTrackAllocator;		// size 0x04, offset 0x30
		detail::SeqPlayer::SeqUserprocCallback			*mSeqUserprocCallback;		// size 0x04, offset 0x34
		void											*mSeqUserprocCallbackArg;	// size 0x04, offset 0x38
		u32												mSoundPlayerCount;			// size 0x04, offset 0x3c
		SoundPlayer										*mSoundPlayers;				// size 0x04, offset 0x40
		detail::SoundInstanceManager<detail::SeqSound>	mSeqSoundInstanceManager;	// size 0x28, offset 0x44
		detail::SoundInstanceManager<detail::StrmSound>	mStrmSoundInstanceManager;	// size 0x28, offset 0x6c
		detail::SoundInstanceManager<detail::WaveSound>	mWaveSoundInstanceManager;	// size 0x28, offset 0x94
		detail::MmlSeqTrackAllocator					mMmlSeqTrackAllocator;		// size 0x0c, offset 0xbc
		detail::StrmBufferPool							mStrmBufferPool;			// size 0x18, offset 0xc8
		detail::MmlParser								mMmlParser;					// size 0x04, offset 0xe0
		void											*mSetupBufferAddress;		// size 0x04, offset 0xe4
		u32												mSetupBufferSize;			// size 0x04, offset 0xe8
	}; // size 0xec
}} // namespace nw4r::snd

#endif // NW4R_SND_SOUND_ARCHIVE_PLAYER_H
