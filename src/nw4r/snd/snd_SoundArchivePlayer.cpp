#include "nw4r/snd/snd_SoundArchivePlayer.h"

/* Original source:
 * kiwi515/ogws
 * src/nw4r/snd/snd_SoundArchivePlayer.cpp
 */

/*******************************************************************************
 * headers
 */

#include <new>

#include <decomp.h>
#include "common.h"

#include "nw4r/snd/snd_Bank.h"
#include "nw4r/snd/snd_BasicSound.h"
#include "nw4r/snd/snd_SoundArchiveLoader.h"
#include "nw4r/snd/snd_debug.h"
#include "nw4r/snd/snd_DisposeCallbackManager.h"
#include "nw4r/snd/snd_ExternalSoundPlayer.h"
#include "nw4r/snd/snd_MmlParser.h"
#include "nw4r/snd/snd_MmlSeqTrack.h"
#include "nw4r/snd/snd_MmlSeqTrackAllocator.h"
#include "nw4r/snd/snd_PlayerHeap.h"
#include "nw4r/snd/snd_SeqFile.h"
#include "nw4r/snd/snd_SeqPlayer.h"
#include "nw4r/snd/snd_SeqSound.h"
#include "nw4r/snd/snd_SoundActor.h"
#include "nw4r/snd/snd_SoundArchive.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundInstanceManager.h"
#include "nw4r/snd/snd_SoundPlayer.h"
#include "nw4r/snd/snd_SoundStartable.h"
#include "nw4r/snd/snd_SoundSystem.h"
#include "nw4r/snd/snd_SoundThread.h"
#include "nw4r/snd/snd_StrmChannel.h"
#include "nw4r/snd/snd_StrmPlayer.h"
#include "nw4r/snd/snd_StrmSound.h"
#include "nw4r/snd/snd_WaveFile.h"
#include "nw4r/snd/snd_WaveSound.h"
#include "nw4r/snd/snd_WsdFile.h"
#include "nw4r/snd/snd_WsdPlayer.h"

#include "nw4r/ut/ut_FileStream.h"
#include "nw4r/ut/ut_algorithm.h"

#include "nw4r/NW4RAssert.hpp"

/*******************************************************************************
 * functions
 */

namespace nw4r { namespace snd {

SoundArchivePlayer::SoundArchivePlayer() :
	mSoundArchive			(nullptr),
	mGroupTable				(nullptr),
	mFileTable				(nullptr),
	mFileManager			(nullptr),
	mSeqCallback			(*this),
	mWsdCallback			(*this),
	mSeqUserprocCallback	(nullptr),
	mSeqUserprocCallbackArg	(nullptr),
	mSoundPlayerCount		(0),
	mSoundPlayers			(nullptr),
	mMmlSeqTrackAllocator	(&mMmlParser),
	mSetupBufferAddress		(nullptr),
	mSetupBufferSize		(0)
{
	detail::DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);
}

SoundArchivePlayer::~SoundArchivePlayer()
{
	detail::DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(this);
}

bool SoundArchivePlayer::IsAvailable() const
{
	if (mSoundArchive == nullptr)
		return false;

	return mSoundArchive->IsAvailable();
}

bool SoundArchivePlayer::Setup(SoundArchive const *arc, void *buffer, u32 size,
                               void *strmBuffer, u32 strmBufferSize)
{
	NW4RAssert_Line(131, SoundSystem::IsInitializedSoundSystem());
	if (!SoundSystem::IsInitializedSoundSystem())
		return false;

	NW4RAssertPointerNonnull_Line(137, arc);
	NW4RAssertPointerNonnull_Line(138, buffer);
	if (strmBufferSize)
		NW4RAssertPointerNonnull_Line(140, strmBuffer);

	NW4RAssert_Line(142, strmBufferSize >= GetRequiredStrmBufferSize( arc ));

	if (!SetupMram(arc, buffer, size))
		return false;

	if (!SetupStrmBuffer(arc, strmBuffer, strmBufferSize))
		return false;

	mSeqTrackAllocator = &mMmlSeqTrackAllocator;

	return true;
}

void SoundArchivePlayer::Shutdown()
{
	mSoundArchive		= nullptr;
	mGroupTable			= nullptr;
	mFileTable			= nullptr;
	mFileManager		= nullptr;
	mSeqTrackAllocator	= nullptr;

	for (u32 playerId = 0; playerId < mSoundPlayerCount; playerId++)
		mSoundPlayers[playerId].~SoundPlayer();

	mSoundPlayerCount	= 0;
	mSoundPlayers		= nullptr;

	mStrmBufferPool.Shutdown();

	if (mSetupBufferAddress)
	{
		mSeqSoundInstanceManager.Destroy(mSetupBufferAddress, mSetupBufferSize);
		mStrmSoundInstanceManager.Destroy(mSetupBufferAddress,
		                                  mSetupBufferSize);
		mWaveSoundInstanceManager.Destroy(mSetupBufferAddress,
		                                  mSetupBufferSize);
		mMmlSeqTrackAllocator.Destroy(mSetupBufferAddress, mSetupBufferSize);

		mSetupBufferAddress	= nullptr;
		mSetupBufferSize	= 0;
	}
}

u32 SoundArchivePlayer::GetRequiredMemSize(SoundArchive const *arc)
{
	NW4RAssertPointerNonnull_Line(210, arc);

	u32 size = 0;

	u32 playerCount = arc->GetPlayerCount();
	size += ut::RoundUp(sizeof(SoundPlayer) * playerCount, 4);

	for (u32 playerId = 0; playerId < playerCount; playerId++)
	{
		SoundArchive::PlayerInfo playerInfo;
		if (!arc->ReadPlayerInfo(playerId, &playerInfo))
			continue;

		for (int i = 0; i < playerInfo.playableSoundCount; i++)
		{
			if (playerInfo.heapSize == 0)
				continue;

			size += ut::RoundUp(sizeof(detail::PlayerHeap), 4);
			size = ut::RoundUp(size, 32);
			size += ut::RoundUp(playerInfo.heapSize, 4);
		}
	}

	size += ut::RoundUp(sizeof(u32) + arc->GetGroupCount() * 8, 4);

	SoundArchive::SoundArchivePlayerInfo soundArchivePlayerInfo;
	if (arc->ReadSoundArchivePlayerInfo(&soundArchivePlayerInfo))
	{
		size += ut::RoundUp(soundArchivePlayerInfo.seqSoundCount
		                        * sizeof(detail::SeqSound),
		                    4);
		size += ut::RoundUp(soundArchivePlayerInfo.strmSoundCount
		                        * sizeof(detail::StrmSound),
		                    4);
		size += ut::RoundUp(soundArchivePlayerInfo.waveSoundCount
		                        * sizeof(detail::WaveSound),
		                    4);
		size += ut::RoundUp(soundArchivePlayerInfo.seqTrackCount
		                        * sizeof(detail::MmlSeqTrack),
		                    4);
	}

	size += ut::RoundUp(sizeof(u32) + arc->detail_GetFileCount() * 8, 4);

	return size;
}

u32 SoundArchivePlayer::GetRequiredStrmBufferSize(SoundArchive const *arc)
{
	NW4RAssertPointerNonnull_Line(272, arc);

	int strmChannelCount = 0;

	SoundArchive::SoundArchivePlayerInfo soundArchivePlayerInfo;
	if (arc->ReadSoundArchivePlayerInfo(&soundArchivePlayerInfo))
		strmChannelCount = soundArchivePlayerInfo.strmChannelCount;

	// TODO (from ogws): How is this calculated?
	// is this the size of one strmChannel? (whatever that is?)
	u32 memSize = strmChannelCount * 0xa000;
	return memSize;
}

bool SoundArchivePlayer::SetupMram(SoundArchive const *arc, void *buffer,
                                   u32 size)
{
	NW4RAssertPointerNonnull_Line(304, arc);
	NW4RAssertPointerNonnull_Line(305, buffer);
	NW4RAssertAligned_Line(306, buffer, 4);
	NW4RAssert_Line(307, size >= GetRequiredMemSize( arc ));

	void *endp = static_cast<byte_t *>(buffer) + size;
	void *buf = buffer;

	if (!SetupSoundPlayer(arc, &buf, endp))
		return false;
	if (!CreateGroupAddressTable(arc, &buf, endp))
		return false;
	if (!CreateFileAddressTable(arc, &buf, endp))
		return false;

	SoundArchive::SoundArchivePlayerInfo soundArchivePlayerInfo;
	if (arc->ReadSoundArchivePlayerInfo(&soundArchivePlayerInfo))
	{
		if (!SetupSeqSound(arc, soundArchivePlayerInfo.seqSoundCount, &buf,
		                   endp))
		{
			return false;
		}

		if (!SetupStrmSound(arc, soundArchivePlayerInfo.strmSoundCount, &buf,
		                   endp))
		{
			return false;
		}

		if (!SetupWaveSound(arc, soundArchivePlayerInfo.waveSoundCount, &buf,
		                   endp))
		{
			return false;
		}

		if (!SetupSeqTrack(arc, soundArchivePlayerInfo.seqTrackCount, &buf,
		                   endp))
		{
			return false;
		}
	}

	NW4RAssert_Line(351, static_cast<char*>(buf) - static_cast<char*>(buffer)
	                         == GetRequiredMemSize( arc ));

	mSoundArchive		= arc;
	mSetupBufferAddress	= buffer;
	mSetupBufferSize	= size;

	return true;
}

detail::PlayerHeap *SoundArchivePlayer::CreatePlayerHeap(void **buffer,
                                                         void *endp,
                                                         u32 heapSize)
{
	void *ep = ut::RoundUp(ut::AddOffsetToPtr(*buffer, 32), 4);
	if (ut::ComparePtr(ep, endp) > 0)
		return nullptr;

	void *buf = *buffer;
	*buffer = ep;

	detail::PlayerHeap *playerHeap = new (buf) detail::PlayerHeap;
	*buffer = ut::RoundUp(*buffer, 32);

	ep = ut::RoundUp(ut::AddOffsetToPtr(*buffer, heapSize), 4);
	if (ut::ComparePtr(ep, endp) > 0)
		return nullptr;

	buf = *buffer;
	*buffer = ep;

	bool result = playerHeap->Create(buf, heapSize);
	if (!result)
		return nullptr;

	return playerHeap;
}

bool SoundArchivePlayer::SetupSoundPlayer(SoundArchive const *arc,
                                          void **buffer, void *endp)
{
	u32 playerCount = arc->GetPlayerCount();
	u32 requireSize = sizeof(SoundPlayer) * playerCount;

	void *ep = ut::RoundUp(ut::AddOffsetToPtr(*buffer, requireSize), 4);

	if (ut::ComparePtr(ep, endp) > 0)
		return false;

	void *buf = *buffer;
	*buffer = ep;

	mSoundPlayers = static_cast<SoundPlayer *>(buf);
	mSoundPlayerCount = playerCount;

	byte_t *ptr = static_cast<byte_t *>(buf);

	for (u32 playerId = 0; playerId < playerCount;
	     playerId++, ptr += sizeof(SoundPlayer))
	{
		SoundPlayer *player = new (ptr) SoundPlayer;

		SoundArchive::PlayerInfo playerInfo;
		if (!arc->ReadPlayerInfo(playerId, &playerInfo))
			continue;

		player->SetPlayableSoundCount(playerInfo.playableSoundCount);

		if (!playerInfo.heapSize)
			continue;

		for (int i = 0; i < playerInfo.playableSoundCount; i++)
		{
			detail::PlayerHeap *playerHeap =
				CreatePlayerHeap(buffer, endp, playerInfo.heapSize);

			NW4RCheckMessage_Line(
				442, playerHeap,
				"failed to create player heap. ( player id = %d )", playerId);
			if (!playerHeap)
				return false;

			player->detail_AppendPlayerHeap(playerHeap);
		}

		player->detail_SetPlayableSoundLimit(playerInfo.playableSoundCount);
	}

	return true;
}

bool SoundArchivePlayer::CreateGroupAddressTable(SoundArchive const *arc,
                                                 void **buffer, void *endp)
{
	u32 requireSize = sizeof(u32) + sizeof(GroupAddress) * arc->GetGroupCount();

	void *ep = ut::RoundUp(ut::AddOffsetToPtr(*buffer, requireSize), 4);

	if (ut::ComparePtr(ep, endp) > 0)
		return false;

	mGroupTable = static_cast<GroupAddressTable *>(*buffer);
	*buffer = ep;

	mGroupTable->count = arc->GetGroupCount();

	for (int i = 0; i < mGroupTable->count; i++)
	{
		mGroupTable->item[i].address			= nullptr;
		mGroupTable->item[i].waveDataAddress	= nullptr;
	}

	return true;
}

bool SoundArchivePlayer::CreateFileAddressTable(SoundArchive const *arc,
                                                void **buffer, void *endp)
{
	u32 requireSize =
		sizeof(u32) + sizeof(FileAddress) * arc->detail_GetFileCount();

	void *ep = ut::RoundUp(ut::AddOffsetToPtr(*buffer, requireSize), 4);

	if (ut::ComparePtr(ep, endp) > 0)
		return false;

	mFileTable = static_cast<FileAddressTable *>(*buffer);
	*buffer = ep;

	mFileTable->count = arc->detail_GetFileCount();

	for (int i = 0; i < mFileTable->count; i++)
	{
		mFileTable->item[i].address			= nullptr;
		mFileTable->item[i].waveDataAddress	= nullptr;
	}

	return true;
}

bool SoundArchivePlayer::SetupSeqSound(SoundArchive const *arc ATTR_UNUSED,
                                       int numSounds, void **buffer, void *endp)
{
	u32 requireSize = sizeof(detail::SeqSound) * numSounds;

	void *ep = ut::RoundUp(ut::AddOffsetToPtr(*buffer, requireSize), 4);

	if (ut::ComparePtr(ep, endp) > 0)
		return false;

	u32 createNum ATTR_MAYBE_UNUSED =
		mSeqSoundInstanceManager.Create(*buffer, requireSize);

	NW4RAssert_Line(536, createNum == numSounds);

	*buffer = ep;

	return true;
}

bool SoundArchivePlayer::SetupWaveSound(SoundArchive const *arc ATTR_UNUSED,
                                        int numSounds, void **buffer,
                                        void *endp)
{
	u32 requireSize = sizeof(detail::WaveSound) * numSounds;

	void *ep = ut::RoundUp(ut::AddOffsetToPtr(*buffer, requireSize), 4);

	if (ut::ComparePtr(ep, endp) > 0)
		return false;

	u32 createNum ATTR_MAYBE_UNUSED =
		mWaveSoundInstanceManager.Create(*buffer, requireSize);

	NW4RAssert_Line(568, createNum == numSounds);

	*buffer = ep;

	return true;
}

bool SoundArchivePlayer::SetupStrmSound(SoundArchive const *arc ATTR_UNUSED,
                                        int numSounds, void **buffer,
                                        void *endp)
{
	u32 requireSize = sizeof(detail::StrmSound) * numSounds;

	void *ep = ut::RoundUp(ut::AddOffsetToPtr(*buffer, requireSize), 4);

	if (ut::ComparePtr(ep, endp) > 0)
		return false;

	u32 createNum ATTR_MAYBE_UNUSED =
		mStrmSoundInstanceManager.Create(*buffer, requireSize);

	NW4RAssert_Line(600, createNum == numSounds);

	*buffer = ep;

	return true;
}

bool SoundArchivePlayer::SetupSeqTrack(SoundArchive const *arc ATTR_UNUSED,
                                       int numTracks, void **buffer, void *endp)
{
	// TODO: why is this sizeof(MmlSeqTrack) instead of sizeof(SeqTrack)?
	u32 requireSize = sizeof(detail::MmlSeqTrack) * numTracks;

	void *ep = ut::RoundUp(ut::AddOffsetToPtr(*buffer, requireSize), 4);

	if (ut::ComparePtr(ep, endp) > 0)
		return false;

	u32 createNum ATTR_MAYBE_UNUSED =
		mMmlSeqTrackAllocator.Create(*buffer, requireSize);

	NW4RAssert_Line(632, createNum == numTracks);

	*buffer = ep;

	return true;
}

bool SoundArchivePlayer::SetupStrmBuffer(SoundArchive const *arc, void *buffer,
                                         u32 size)
{
	if (size < GetRequiredStrmBufferSize(arc))
		return false;

	int strmChannelCount = 0;

	SoundArchive::SoundArchivePlayerInfo soundArchivePlayerInfo;
	if (arc->ReadSoundArchivePlayerInfo(&soundArchivePlayerInfo))
		strmChannelCount = soundArchivePlayerInfo.strmChannelCount;

	mStrmBufferPool.Setup(buffer, size, strmChannelCount);

	return true;
}

void SoundArchivePlayer::Update()
{
	for (u32 playerId = 0; playerId < mSoundPlayerCount; playerId++)
		GetSoundPlayer(playerId).Update();

	mSeqSoundInstanceManager.SortPriorityList();
	mStrmSoundInstanceManager.SortPriorityList();
	mWaveSoundInstanceManager.SortPriorityList();
}

SoundArchive const &SoundArchivePlayer::GetSoundArchive() const
{
	NW4RAssertMessage_Line(691, mSoundArchive, "Setup is not completed.");

	return *mSoundArchive;
}

SoundPlayer &SoundArchivePlayer::GetSoundPlayer(u32 playerId)
{
	// specifically not the source variant
	NW4RAssertHeaderClampedLValue_Line(697, playerId, 0, mSoundPlayerCount);

	return mSoundPlayers[playerId];
}

#if 0
/* SoundArchivePlayer::GetSoundPlayer(char const *)
 * ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14216)
 */
DECOMP_FORCE(NW4RAssertPointerNonnull_String(mSoundArchive));
#endif

void const *SoundArchivePlayer::detail_GetFileAddress(u32 fileId) const
{
	if (mFileManager)
	{
		if (void const *addr = mFileManager->GetFileAddress(fileId))
			return addr;
	}

	if (void const *addr = mSoundArchive->detail_GetFileAddress(fileId))
		return addr;

	if (void const *fileData = GetFileAddress(fileId))
		return fileData;

	SoundArchive::FileInfo fileInfo;
	if (!mSoundArchive->detail_ReadFileInfo(fileId, &fileInfo))
		return nullptr;

	for (unsigned i = 0; i < fileInfo.filePosCount; i++)
	{
		SoundArchive::FilePos filePos;
		if (!mSoundArchive->detail_ReadFilePos(fileId, i, &filePos))
			continue;

		void const *groupData = GetGroupAddress(filePos.groupId);
		if (!groupData)
			continue;

		SoundArchive::GroupItemInfo itemInfo;
		if (!mSoundArchive->detail_ReadGroupItemInfo(filePos.groupId,
		                                             filePos.index, &itemInfo))
		{
			continue;
		}

		return static_cast<byte_t const *>(groupData) + itemInfo.offset;
	}

	return nullptr;
}

void const *SoundArchivePlayer::detail_GetFileWaveDataAddress(u32 fileId) const
{
	if (mFileManager)
	{
		if (void const *addr = mFileManager->GetFileWaveDataAddress(fileId))
			return addr;
	}

	if (void const *addr = mSoundArchive->detail_GetWaveDataFileAddress(fileId))
		return addr;

	if (void const *fileData = GetFileWaveDataAddress(fileId))
		return fileData;

	SoundArchive::FileInfo fileInfo;
	if (!mSoundArchive->detail_ReadFileInfo(fileId, &fileInfo))
		return nullptr;

	for (unsigned i = 0; i < fileInfo.filePosCount; i++)
	{
		SoundArchive::FilePos filePos;
		if (!mSoundArchive->detail_ReadFilePos(fileId, i, &filePos))
			continue;

		void const *groupData = GetGroupWaveDataAddress(filePos.groupId);
		if (!groupData)
			continue;

		SoundArchive::GroupItemInfo itemInfo;
		if (!mSoundArchive->detail_ReadGroupItemInfo(filePos.groupId,
		                                             filePos.index, &itemInfo))
		{
			continue;
		}

		return static_cast<byte_t const *>(groupData) + itemInfo.waveDataOffset;
	}

	return nullptr;
}

void const *SoundArchivePlayer::GetGroupAddress(u32 groupId) const
{
	if (!mGroupTable)
	{
		NW4RCheckMessage_Line(840, mGroupTable,
		                      "Failed to SoundArchivePlayer::GetGroupAddress "
		                      "because group table is not allocated.\n");

		return nullptr;
	}

	if (groupId >= mGroupTable->count)
		return nullptr;

	return mGroupTable->item[groupId].address;
}

#if 0
// SoundArchivePlayer::SetGroupAddress ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14220)
DECOMP_FORCE("Failed to SoundArchivePlayer::SetGroupAddress because group "
             "table is not allocated.\n");
#endif

void SoundArchivePlayer::SetGroupAddress(u32 id, const void* pAddr)
{
	if (mGroupTable == NULL)
		return;

	mGroupTable->item[id].address = pAddr;
}

void const *SoundArchivePlayer::GetGroupWaveDataAddress(u32 groupId) const
{
	if (!mGroupTable)
	{
		NW4RCheckMessage_Line(
			891, mGroupTable,
			"Failed to SoundArchivePlayer::GetGroupWaveDataAddress "
			"because group table is not allocated.\n");

		return nullptr;
	}

	if (groupId >= mGroupTable->count)
		return nullptr;

	return mGroupTable->item[groupId].waveDataAddress;
}

void SoundArchivePlayer::SetGroupWaveDataAddress(u32 id, const void* pAddr)
{
	if (mGroupTable == NULL)
		return;

	mGroupTable->item[id].waveDataAddress = pAddr;
}

#if 0
/* SoundArchivePlayer::SetGroupWaveDataAddress
 * ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14222)
 */
DECOMP_FORCE(NW4RAssertHeaderClampedLValue_String(groupId));
DECOMP_FORCE("Failed to SoundArchivePlayer::SetGroupWaveDataAddress because "
             "group table is not allocated.\n");

// SoundArchivePlayer::SetFileAddress ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14224)
DECOMP_FORCE("Failed to SoundArchivePlayer::SetFileAddress because file table "
             "is not allocated.\n");

/* SoundArchivePlayer::SetFileWaveDataAddress
 * ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14226)
 */
DECOMP_FORCE(NW4RAssertHeaderClampedLValue_String(fileId));
DECOMP_FORCE("Failed to SoundArchivePlayer::SetFileWaveDataAddress because "
             "file table is not allocated.\n");
#endif

void const *SoundArchivePlayer::GetFileAddress(u32 fileId) const
{
	if (!mFileTable)
	{
		NW4RCheckMessage_Line(942, mFileTable,
		                      "Failed to SoundArchivePlayer::GetFileAddress "
		                      "because file table is not allocated.\n");

		return nullptr;
	}

	if (fileId >= mFileTable->count)
		return nullptr;

	return mFileTable->item[fileId].address;
}

void const *SoundArchivePlayer::GetFileWaveDataAddress(u32 fileId) const
{
	if (!mFileTable)
	{
		NW4RCheckMessage_Line(
			993, mFileTable,
			"Failed to SoundArchivePlayer::GetFileWaveDataAddress "
			"because file table is not allocated.\n");

		return nullptr;
	}

	if (fileId >= mFileTable->count)
		return nullptr;

	return mFileTable->item[fileId].waveDataAddress;
}

SoundStartable::StartResult SoundArchivePlayer::detail_SetupSound(
	SoundHandle *handle, u32 soundId, bool holdFlag,
	SoundStartable::StartInfo const *startInfo)
{
	return detail_SetupSoundImpl(handle, soundId, nullptr, nullptr, holdFlag,
	                             startInfo);
}

template <class Sound>
Sound *SoundArchivePlayer::AllocSound(
	detail::SoundInstanceManager<Sound> *manager, u32 soundId,
	int priority, int ambientPriority,
	detail::BasicSound::AmbientInfo *ambientArgInfo)
{
	NW4RAssertPointerNonnull_Line(1050, manager);

	Sound *sound = manager->Alloc(priority, ambientPriority);
	if (!sound)
		return nullptr;

	sound->SetId(soundId);

	if (ambientArgInfo)
		sound->SetAmbientInfo(*ambientArgInfo);

	return sound;
}

SoundStartable::StartResult SoundArchivePlayer::detail_SetupSoundImpl(
	SoundHandle *handle, u32 soundId,
	detail::BasicSound::AmbientInfo *ambientArgInfo, SoundActor *actor,
	bool holdFlag, SoundStartable::StartInfo const *startInfo)
{
	NW4RAssertPointerNonnull_Line(1108, handle);

	if (!IsAvailable())
		return START_ERR_NOT_AVAILABLE;

	if (handle->IsAttachedSound())
		handle->DetachSound();

	SoundArchive::SoundInfo soundInfo;
	if (!mSoundArchive->ReadSoundInfo(soundId, &soundInfo))
		return START_ERR_INVALID_SOUNDID;

	SoundStartable::StartInfo::StartOffsetType startOffsetType =
		SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC;

	int startOffset = 0;
	int playerPriority = soundInfo.playerPriority;
	u32 playerId = soundInfo.playerId;
	int actorPlayerId = soundInfo.actorPlayerId;
	void const *externalSeqDataAddress = nullptr;
	char const *externalSeqStartLabel = nullptr;

	if (startInfo)
	{
		if (startInfo->enableFlag
		    & SoundStartable::StartInfo::ENABLE_START_OFFSET)
		{
			startOffsetType = startInfo->startOffsetType;
			startOffset = startInfo->startOffset;
		}

		if (startInfo->enableFlag
		    & SoundStartable::StartInfo::ENABLE_PLAYER_PRIORITY)
		{
			playerPriority = startInfo->playerPriority;
		}

		if (startInfo->enableFlag & SoundStartable::StartInfo::ENABLE_PLAYER_ID)
			playerId = startInfo->playerId;

		if (startInfo->enableFlag
		    & SoundStartable::StartInfo::ENABLE_ACTOR_PLAYER_ID)
		{
			actorPlayerId = startInfo->actorPlayerId;
		}

		if (startInfo->enableFlag
		    & SoundStartable::StartInfo::ENABLE_SEQ_SOUND_INFO)
		{
			externalSeqDataAddress = startInfo->seqSoundInfo.seqDataAddress;
			externalSeqStartLabel = startInfo->seqSoundInfo.startLocationLabel;
		}
	}

	int priority = playerPriority;
	if (holdFlag)
		priority--;

	int ambientPriority = 0;
	if (ambientArgInfo)
	{
		ambientPriority =
			detail::BasicSound::GetAmbientPriority(*ambientArgInfo, soundId);
	}

	int allocPriority = priority + ambientPriority;
	allocPriority = ut::Clamp(allocPriority, 0, 127);

	detail::ExternalSoundPlayer *extPlayer = nullptr;
	if (actor)
	{
		extPlayer = actor->detail_GetActorPlayer(actorPlayerId);
		if (!extPlayer)
		{
			// this feels like a macro since it has the name in the string
			NW4RWarningMessage_Line(1181,
			                        "actorPlayerId(%d) is out of range. (0-%d)",
			                        actorPlayerId, 3);

			return START_ERR_INVALID_PARAMETER;
		}
	}

	detail::SoundThread::AutoLock lock;

	SoundPlayer &player = GetSoundPlayer(playerId);
	if (!player.detail_CanPlaySound(allocPriority))
		return START_ERR_LOW_PRIORITY;

	if (extPlayer)
	{
		if (!extPlayer->detail_CanPlaySound(allocPriority))
			return START_ERR_LOW_PRIORITY;
	}

	detail::BasicSound *sound = nullptr;
	detail::SeqSound *seqSound = nullptr;
	detail::StrmSound *strmSound = nullptr;
	detail::WaveSound *waveSound = nullptr;

	switch (mSoundArchive->GetSoundType(soundId))
	{
	case SoundArchive::SOUND_TYPE_SEQ:
		seqSound = AllocSound(&mSeqSoundInstanceManager, soundId, priority,
		                        ambientPriority, ambientArgInfo);
		if (!seqSound)
		{
			NW4RCheckMessage_Line(
				1220,
				!detail::Debug_GetWarningFlag(
					DEBUG_WARNING_NOT_ENOUGH_SEQSOUND),
				"Failed to start sound (id:%d) for not enough SeqSound "
				"instance.",
				soundId);

			return START_ERR_NOT_ENOUGH_INSTANCE;
		}

		sound = seqSound;
		break;

	case SoundArchive::SOUND_TYPE_STRM:
		strmSound = AllocSound(&mStrmSoundInstanceManager, soundId, priority,
		                        ambientPriority, ambientArgInfo);
		if (!strmSound)
		{
			NW4RCheckMessage_Line(
				1240,
				!detail::Debug_GetWarningFlag(
					DEBUG_WARNING_NOT_ENOUGH_STRMSOUND),
				"Failed to start sound (id:%d) for not enough StrmSound "
				"instance.",
				soundId);

			return START_ERR_NOT_ENOUGH_INSTANCE;
		}

		sound = strmSound;
		break;

	case SoundArchive::SOUND_TYPE_WAVE:
		waveSound = AllocSound(&mWaveSoundInstanceManager, soundId, priority,
		                        ambientPriority, ambientArgInfo);
		if (!waveSound)
		{
			NW4RCheckMessage_Line(
				1260,
				!detail::Debug_GetWarningFlag(
					DEBUG_WARNING_NOT_ENOUGH_WAVESOUND),
				"Failed to start sound (id:%d) for not enough WaveSound "
				"instance.",
				soundId);

			return START_ERR_NOT_ENOUGH_INSTANCE;
		}

		sound = waveSound;
		break;

	default:
		return START_ERR_INVALID_SOUNDID;
	}

	if (!player.detail_AppendSound(sound))
	{
		sound->Shutdown();

		return START_ERR_UNKNOWN;
	}

	switch (mSoundArchive->GetSoundType(soundId))
	{
	case SoundArchive::SOUND_TYPE_SEQ:
	{
		NW4RAssertPointerNonnull_Line(1282, seqSound);

		player.detail_AllocPlayerHeap(seqSound);

		SoundArchive::SeqSoundInfo info;
		if (!mSoundArchive->ReadSeqSoundInfo(soundId, &info))
		{
			seqSound->Shutdown();
			return START_ERR_INVALID_SOUNDID;
		}

		SoundStartable::StartResult result = PrepareSeqImpl(
			seqSound, &soundInfo, &info, startOffsetType,
			startOffset, externalSeqDataAddress, externalSeqStartLabel);

		if (result == SoundStartable::START_SUCCESS)
			break;

		seqSound->Shutdown();
		return result;
	}

	case SoundArchive::SOUND_TYPE_STRM:
	{
		NW4RAssertPointerNonnull_Line(1314, strmSound);

		SoundArchive::StrmSoundInfo info;
		if (!mSoundArchive->detail_ReadStrmSoundInfo(soundId, &info))
		{
			strmSound->Shutdown();
			return START_ERR_INVALID_SOUNDID;
		}

		SoundStartable::StartResult result = PrepareStrmImpl(
			strmSound, &soundInfo, &info, startOffsetType,
			startOffset);

		if (result == SoundStartable::START_SUCCESS)
			break;

		strmSound->Shutdown();
		return result;
	}

	case SoundArchive::SOUND_TYPE_WAVE:
	{
		NW4RAssertPointerNonnull_Line(1341, waveSound);

		SoundArchive::WaveSoundInfo info;
		if (!mSoundArchive->detail_ReadWaveSoundInfo(soundId, &info))
		{
			waveSound->Shutdown();
			return START_ERR_INVALID_SOUNDID;
		}

		SoundStartable::StartResult result = PrepareWaveSoundImpl(
			waveSound, &soundInfo, &info, startOffsetType,
			startOffset);

		if (result == SoundStartable::START_SUCCESS)
			break;

		waveSound->Shutdown();
		return result;
	}

	default:
		NW4RPanic_Line(1367);

		sound->Shutdown();
		return START_ERR_INVALID_SOUNDID;
	}

	if (extPlayer)
	{
		if (!extPlayer->AppendSound(sound))
		{
			sound->Shutdown();
			return START_ERR_UNKNOWN;
		}
	}

	if (actor)
		sound->AttachSoundActor(actor);

	if (holdFlag)
		sound->SetPlayerPriority(playerPriority);

	handle->detail_AttachSound(sound);

	return START_SUCCESS;
}

SoundStartable::StartResult SoundArchivePlayer::PrepareSeqImpl(
	detail::SeqSound *sound, SoundArchive::SoundInfo const *commonInfo,
	SoundArchive::SeqSoundInfo const *info,
	SoundStartable::StartInfo::StartOffsetType startOffsetType, int startOffset,
	void const *externalSeqDataAddress, char const *externalSeqStartLabel)
{
	NW4RAssertPointerNonnull_Line(1422, info);

	detail::SeqFile const *seqFile = nullptr;
	ut::FileStream *fileStream = nullptr;
	u32 seqOffset = 0;
	u32 allocTrack = info->allocTrack;

	if (externalSeqDataAddress)
	{
		seqFile = static_cast<detail::SeqFile const *>(externalSeqDataAddress);
		seqOffset = 0;

		detail::SeqFileReader seqFileReader(seqFile);

		if (externalSeqStartLabel)
		{
			bool result = seqFileReader.ReadOffsetByLabel(externalSeqStartLabel,
			                                              &seqOffset);
			if (!result)
				return START_ERR_INVALID_SEQ_START_LOCATION_LABEL;
		}

		seqOffset = detail::MmlParser::ParseAllocTrack(
			seqFileReader.GetBaseAddress(), seqOffset, &allocTrack);
	}

	if (!seqFile)
	{
		seqFile = static_cast<detail::SeqFile const *>(
			detail_GetFileAddress(commonInfo->fileId));
		seqOffset = info->dataOffset;

		if (externalSeqStartLabel)
		{
			detail::SeqFileReader seqFileReader(seqFile);

			bool result = seqFileReader.ReadOffsetByLabel(externalSeqStartLabel,
			                                              &seqOffset);
			if (!result)
				return START_ERR_INVALID_SEQ_START_LOCATION_LABEL;
		}
	}

	if (!seqFile)
	{
		detail::PlayerHeap *heap = sound->GetPlayerHeap();
		if (!heap)
			return START_ERR_NOT_DATA_LOADED;

		void *fileStreamBuffer = sound->GetFileStreamBuffer();
		s32 fileStreamBufferSize = sound->GetFileStreamBufferSize();
		NW4RAssertPointerNonnull_Line(1477, fileStreamBuffer);

		fileStream = mSoundArchive->detail_OpenFileStream(
			commonInfo->fileId, fileStreamBuffer, fileStreamBufferSize);
		if (!fileStream)
			return START_ERR_CANNOT_OPEN_FILE;

		if (heap->GetFreeSize() < fileStream->GetSize())
		{
			fileStream->Close();
			return SoundStartable::START_ERR_NOT_ENOUGH_PLAYER_HEAP;
		}
	}

	detail::SeqPlayer::SetupResult result =
		sound->Setup(mSeqTrackAllocator, allocTrack, &mSeqCallback);

	while (result != detail::SeqPlayer::SETUP_SUCCESS)
	{
		if (result == detail::SeqPlayer::SETUP_ERR_CANNOT_ALLOCATE_TRACK)
		{
			if (mSeqSoundInstanceManager.GetActiveCount() == 1)
			{
				if (fileStream)
					fileStream->Close();

				NW4RCheckMessage_Line(1520,
				                      !detail::Debug_GetWarningFlag(
				                          DEBUG_WARNING_NOT_ENOUGH_SEQTRACK),
				                      "Failed to start sound (id:%d) for not "
				                      "enough SeqTrack instance.",
				                      sound->GetId());

				return START_ERR_NOT_ENOUGH_INSTANCE;
			}

			detail::SeqSound *lowest =
				mSeqSoundInstanceManager.GetLowestPrioritySound();

			if (sound == lowest)
			{
				if (fileStream)
					fileStream->Close();

				NW4RCheckMessage_Line(1536,
				                      !detail::Debug_GetWarningFlag(
				                          DEBUG_WARNING_NOT_ENOUGH_SEQTRACK),
				                      "Failed to start sound (id:%d) for not "
				                      "enough SeqTrack instance.",
				                      sound->GetId());

				return START_ERR_NOT_ENOUGH_INSTANCE;
			}

			NW4RCheckMessage_Line(
				1545,
				!detail::Debug_GetWarningFlag(
					DEBUG_WARNING_NOT_ENOUGH_SEQTRACK),
				"Sound (id:%d) is stopped for not enough SeqTrack instance.",
				lowest->GetId());

			lowest->Stop(0);
			result = sound->Setup(mSeqTrackAllocator, allocTrack,
			                      &mSeqCallback);
		}
		else
		{
			return SoundStartable::START_ERR_UNKNOWN;
		}
	}

	UpdateCommonSoundParam(sound, commonInfo);

	sound->SetChannelPriority(info->channelPriority);
	sound->SetReleasePriorityFix(info->releasePriorityFixFlag);
	sound->SetSeqUserprocCallback(mSeqUserprocCallback,
	                              mSeqUserprocCallbackArg);

	detail::SeqPlayer::OffsetType seqOffsetType;
	switch (startOffsetType)
	{
	case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC:
		seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_MILLISEC;
		break;

	case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK:
		seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_TICK;
		break;

	case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE:
		seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_TICK;
		startOffset = 0;
		break;

	default:
		seqOffsetType = detail::SeqPlayer::OFFSET_TYPE_TICK;
		startOffset = 0;
		break;
	}

	if (seqFile)
	{
		detail::SeqFileReader seqFileReader(seqFile);

		sound->Prepare(seqFileReader.GetBaseAddress(), seqOffset, seqOffsetType,
		               startOffset);
	}
	else
	{
		sound->Prepare(fileStream, seqOffset, seqOffsetType, startOffset);
	}

	return SoundStartable::START_SUCCESS;
}

SoundStartable::StartResult SoundArchivePlayer::PrepareStrmImpl(
	detail::StrmSound *sound, SoundArchive::SoundInfo const *commonInfo,
	SoundArchive::StrmSoundInfo const *info,
	SoundStartable::StartInfo::StartOffsetType startOffsetType, int startOffset)
{
	detail::StrmPlayer::SetupResult setupResult = sound->Setup(
		&mStrmBufferPool, info->allocChannelCount, info->allocTrackFlag);

	while (setupResult != detail::StrmPlayer::SETUP_SUCCESS)
	{
		if (setupResult == detail::StrmPlayer::SETUP_ERR_CANNOT_ALLOCATE_BUFFER)
		{
			if (mStrmSoundInstanceManager.GetActiveCount() == 1)
			{
				NW4RCheckMessage_Line(1648,
				                      !detail::Debug_GetWarningFlag(
				                          DEBUG_WARNING_NOT_ENOUGH_SEQTRACK),
				                      "Failed to start sound (id:%d) for not "
				                      "enough StrmChannel instance.",
				                      sound->GetId());

				return START_ERR_NOT_ENOUGH_INSTANCE;
			}

			detail::StrmSound *lowest =
				mStrmSoundInstanceManager.GetLowestPrioritySound();
			if (sound == lowest)
			{
				NW4RCheckMessage_Line(1660,
				                      !detail::Debug_GetWarningFlag(
				                          DEBUG_WARNING_NOT_ENOUGH_SEQTRACK),
				                      "Failed to start sound (id:%d) for not "
				                      "enough StrmChannel instance.",
				                      sound->GetId());

				return START_ERR_NOT_ENOUGH_INSTANCE;
			}

			NW4RCheckMessage_Line(
				1669,
				!detail::Debug_GetWarningFlag(
					DEBUG_WARNING_NOT_ENOUGH_STRMCHANNEL),
				"Sound (id:%d) is stopped for not enough StrmChannel instance.",
				lowest->GetId());

			lowest->Stop(0);
			setupResult =
				sound->Setup(&mStrmBufferPool, info->allocChannelCount,
			                 info->allocTrackFlag);
		}
		else
		{
			return SoundStartable::START_ERR_UNKNOWN;
		}
	}

	detail::StrmPlayer::StartOffsetType strmStartOffsetType;
	switch (startOffsetType)
	{
	case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC:
		strmStartOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_MILLISEC;
		break;

	case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK:
		strmStartOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
		startOffset = 0;
		break;

	case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE:
		strmStartOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
		break;

	default:
		strmStartOffsetType = detail::StrmPlayer::START_OFFSET_TYPE_SAMPLE;
		startOffset = 0;
		break;
	}

	void *fileStreamBuffer = sound->GetFileStreamBuffer();
	s32 fileStreamBufferSize = sound->GetFileStreamBufferSize();
	NW4RAssertPointerNonnull_Line(1704, fileStreamBuffer);

	ut::FileStream *fileStream = mSoundArchive->detail_OpenFileStream(
		commonInfo->fileId, fileStreamBuffer, fileStreamBufferSize);
	if (!fileStream)
		return START_ERR_CANNOT_OPEN_FILE;

	bool result = sound->Prepare(strmStartOffsetType, startOffset, fileStream);
	if (!result)
		return START_ERR_UNKNOWN;

	UpdateCommonSoundParam(sound, commonInfo);

	return SoundStartable::START_SUCCESS;
}

SoundStartable::StartResult SoundArchivePlayer::PrepareWaveSoundImpl(
	detail::WaveSound *sound, SoundArchive::SoundInfo const *commonInfo,
	SoundArchive::WaveSoundInfo const *info,
	SoundStartable::StartInfo::StartOffsetType startOffsetType, int startOffset)
{
	NW4RAssertPointerNonnull_Line(1751, info);

	void const *wsdData = detail_GetFileAddress(commonInfo->fileId);
	if (!wsdData)
		return START_ERR_NOT_DATA_LOADED;

	detail::WsdPlayer::StartOffsetType wsdStartOffsetType;
	switch (startOffsetType)
	{
	case SoundStartable::StartInfo::START_OFFSET_TYPE_MILLISEC:
		wsdStartOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_MILLISEC;
		break;

	case SoundStartable::StartInfo::START_OFFSET_TYPE_TICK:
		wsdStartOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_SAMPLE;
		startOffset = 0;
		break;

	case SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE:
		wsdStartOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_SAMPLE;
		break;

	default:
		wsdStartOffsetType = detail::WsdPlayer::START_OFFSET_TYPE_SAMPLE;
		startOffset = 0;
		break;
	}

	bool result =
		sound->Prepare(wsdData, info->subNo, wsdStartOffsetType, startOffset,
	                   &mWsdCallback, commonInfo->fileId);
	if (!result)
		return START_ERR_UNKNOWN;

	UpdateCommonSoundParam(sound, commonInfo);

	sound->SetChannelPriority(info->channelPriority);
	sound->SetReleasePriorityFix(info->releasePriorityFixFlag);

	return SoundStartable::START_SUCCESS;
}

void SoundArchivePlayer::UpdateCommonSoundParam(
	detail::BasicSound *sound, SoundArchive::SoundInfo const *commonInfo)
{
	NW4RAssertPointerNonnull_Line(1814, sound);
	NW4RAssertPointerNonnull_Line(1815, commonInfo);

	sound->SetInitialVolume(commonInfo->volume / 127.0f);
	sound->SetRemoteFilter(commonInfo->remoteFilter);
	sound->SetPanMode(commonInfo->panMode);
	sound->SetPanCurve(commonInfo->panCurve);
}

#if 0
/* SoundArchivePlayer::LoadGroup(u32, SoundMemoryAllocatable *, u32)
 * ([R89JEL]:/bin/RVL/Debug/mainD.MAP:14234)
 */
DECOMP_FORCE(NW4RAssertAligned_String(loadBlockSize, 32));
#endif

bool SoundArchivePlayer::LoadGroup(u32 id, SoundMemoryAllocatable* pAllocatable,
                                   u32 blockSize) {
    if (!IsAvailable()) {
        return false;
    }

    if (id >= mSoundArchive->GetGroupCount()) {
        return false;
    }

    if (GetGroupAddress(id) != NULL) {
        return true;
    }

    if (pAllocatable == NULL) {
        return false;
    }

    detail::SoundArchiveLoader loader(*mSoundArchive);

    void* pWaveBuffer;
    const void* pGroup =
        loader.LoadGroup(id, pAllocatable, &pWaveBuffer, blockSize);

    if (pGroup == NULL) {
        return NULL;
    }

    SetGroupAddress(id, pGroup);
    SetGroupWaveDataAddress(id, pWaveBuffer);

    return true;
}

bool SoundArchivePlayer::LoadGroup(const char* pLabel,
                                   SoundMemoryAllocatable* pAllocatable,
                                   u32 blockSize) {
    u32 id = mSoundArchive->ConvertLabelStringToGroupId(pLabel);
    if (id == SoundArchive::INVALID_ID) {
        return false;
    }

    return LoadGroup(id, pAllocatable, blockSize);
}

bool SoundArchivePlayer::IsLoadedGroup(u32 groupId) const
{
	void const *groupAddress = GetGroupAddress(groupId);
	return groupAddress != NULL;
}

void SoundArchivePlayer::InvalidateData(void const *start, void const *end)
{
	if (mFileTable)
	{
		for (int i = 0; i < mFileTable->count; i++)
		{
			void const *addr = mFileTable->item[i].address;

			if (start <= addr && addr <= end)
				mFileTable->item[i].address = nullptr;
		}
	}

	if (mGroupTable)
	{
		for (int i = 0; i < mGroupTable->count; i++)
		{
			void const *addr = mGroupTable->item[i].address;

			if (start <= addr && addr <= end)
				mGroupTable->item[i].address = nullptr;
		}
	}
}

void SoundArchivePlayer::InvalidateWaveData(void const *start, void const *end)
{
	if (mFileTable)
	{
		for (int i = 0; i < mFileTable->count; i++)
		{
			void const *addr = mFileTable->item[i].waveDataAddress;

			if (start <= addr && addr <= end)
				mFileTable->item[i].waveDataAddress = nullptr;
		}
	}

	if (mGroupTable)
	{
		for (int i = 0; i < mGroupTable->count; i++)
		{
			void const *addr = mGroupTable->item[i].waveDataAddress;

			if (start <= addr && addr <= end)
				mGroupTable->item[i].waveDataAddress = nullptr;
		}
	}
}

detail::Channel *SoundArchivePlayer::SeqNoteOnCallback::NoteOn(
	detail::SeqPlayer *seqPlayer, int bankNo ATTR_UNUSED,
	detail::NoteOnInfo const &noteOnInfo)
{
	if (!mSoundArchivePlayer.IsAvailable())
		return nullptr;

	SoundArchive const &sndArc = mSoundArchivePlayer.GetSoundArchive();
	u32 soundId = seqPlayer->GetId();

	SoundArchive::SeqSoundInfo seqInfo;
	if (!sndArc.ReadSeqSoundInfo(soundId, &seqInfo))
		return nullptr;

	SoundArchive::BankInfo bankInfo;
	if (!sndArc.ReadBankInfo(seqInfo.bankId, &bankInfo))
		return nullptr;

	void const *bankData =
		mSoundArchivePlayer.detail_GetFileAddress(bankInfo.fileId);
	if (!bankData)
		return nullptr;

	detail::Bank bank(bankData);

	void const *waveData =
		mSoundArchivePlayer.detail_GetFileWaveDataAddress(bankInfo.fileId);
	if (!waveData)
		return nullptr;

	bank.SetWaveDataAddress(waveData);

	detail::Channel *channel = bank.NoteOn(noteOnInfo);
	return channel;
}

bool SoundArchivePlayer::WsdCallback::GetWaveSoundData(
	detail::WaveSoundInfo *info, detail::WaveSoundNoteInfo *noteInfo,
	detail::WaveInfo *waveData, void const *waveSoundData, int index,
	int noteIndex, u32 userData) const
{
	u32 fileID = userData;

	if (!mSoundArchivePlayer.IsAvailable())
		return false;

	SoundArchive const &sndArc ATTR_UNUSED =
		mSoundArchivePlayer.GetSoundArchive();

	void const *dataAddr =
		mSoundArchivePlayer.detail_GetFileWaveDataAddress(fileID);
	if (!dataAddr)
		return false;

	detail::WsdFileReader reader(waveSoundData);

	if (!reader.ReadWaveSoundInfo(info, index))
		return false;

	if (!reader.ReadWaveSoundNoteInfo(noteInfo, index, noteIndex))
		return false;

	if (!reader.ReadWaveInfo(noteInfo->waveIndex, waveData, dataAddr))
		return false;

	return true;
}

}} // namespace nw4r::snd
