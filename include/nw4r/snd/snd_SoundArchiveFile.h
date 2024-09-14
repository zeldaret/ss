#ifndef NW4R_SND_SOUND_ARCHIVE_FILE_H
#define NW4R_SND_SOUND_ARCHIVE_FILE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_SoundArchive.h>
#include <nw4r/snd/snd_Util.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {
namespace SoundArchiveFile {

/******************************************************************************
 *
 * Symbol (SYMB) block
 *
 ******************************************************************************/
struct StringTreeNode {
    u16 flags;    // at 0x0
    u16 bit;      // at 0x2
    u32 leftIdx;  // at 0x4
    u32 rightIdx; // at 0x8
    u32 strIdx;   // at 0xC
    u32 id;       // at 0x10
};

struct StringTree {
    u32 rootIdx;                           // at 0x0
    Util::Table<StringTreeNode> nodeTable; // at 0x4
};

struct StringTable {
    Util::Table<u32> offsetTable; // at 0x0
};

struct StringChunk {
    u32 tableOffset;      // at 0x0
    u32 soundTreeOffset;  // at 0x4
    u32 playerTreeOffset; // at 0x8
    u32 groupTreeOffset;  // at 0xC
    u32 bankTreeOffset;   // at 0x10
};

struct StringBlock {
    union {
        StringTable stringTable;
        StringChunk stringChunk;
    }; // at 0x0
};

struct SymbolBlock {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    StringBlock stringBlock;           // at 0x8
};

/******************************************************************************
 *
 * Info (INFO) block
 *
 ******************************************************************************/
struct SeqSoundInfo {
    u32 dataOffset;        // at 0x0
    u32 bankId;            // at 0x4
    u32 allocTrack;        // at 0x8
    u8 channelPriority;    // at 0xC
    u8 releasePriorityFix; // at 0xD
};

struct StrmSoundInfo {};

struct WaveSoundInfo {
    s32 subNo;             // at 0x0
    u32 allocTrack;        // at 0x4
    u8 channelPriority;    // at 0x8
    u8 releasePriorityFix; // at 0x9
};
typedef Util::DataRef<void, SeqSoundInfo, StrmSoundInfo, WaveSoundInfo>
    SoundInfoOffset;

struct Sound3DParam {
    u32 flags;     // at 0x0
    u8 decayCurve; // at 0x4
    u8 decayRatio; // at 0x5
};

struct SoundCommonInfo {
    u32 stringId;                           // at 0x0
    u32 fileId;                             // at 0x4
    u32 playerId;                           // at 0x8
    Util::DataRef<Sound3DParam> param3dRef; // at 0xC
    u8 volume;                              // at 0x14
    u8 playerPriority;                      // at 0x15
    u8 soundType;                           // at 0x16
    u8 remoteFilter;                        // at 0x17
    SoundInfoOffset soundInfoRef;           // at 0x18
    u32 userParam[2];                       // at 0x20
    u8 panMode;                             // at 0x28
    u8 panCurve;                            // at 0x29
};
typedef Util::Table<Util::DataRef<SoundCommonInfo> > SoundCommonTable;

struct BankInfo {
    u32 stringId; // at 0x0
    u32 fileId;   // at 0x4
};
typedef Util::Table<Util::DataRef<BankInfo> > BankTable;

struct PlayerInfo {
    u32 stringId;          // at 0x0
    u8 playableSoundCount; // at 0x4
    u32 heapSize;          // at 0x8
};
typedef Util::Table<Util::DataRef<PlayerInfo> > PlayerTable;

typedef Util::Table<Util::DataRef<SoundArchive::FilePos> > FilePosTable;

struct FileInfo {
    u32 fileSize;                                // at 0x0
    u32 waveDataSize;                            // at 0x4
    s32 entryNum;                                // at 0x8
    Util::DataRef<char> extFilePathRef;          // at 0xC
    Util::DataRef<FilePosTable> filePosTableRef; // at 0x14
};
typedef Util::Table<Util::DataRef<FileInfo> > FileTable;

struct GroupItemInfo {
    u32 fileId;         // at 0x0
    u32 offset;         // at 0x4
    u32 size;           // at 0x8
    u32 waveDataOffset; // at 0xC
    u32 waveDataSize;   // at 0x10
};
typedef Util::Table<Util::DataRef<GroupItemInfo> > GroupItemTable;

struct GroupInfo {
    u32 stringId;                               // at 0x0
    s32 entryNum;                               // at 0x4
    Util::DataRef<char> extFilePathRef;         // at 0x8
    u32 offset;                                 // at 0x10
    u32 size;                                   // at 0x14
    u32 waveDataOffset;                         // at 0x18
    u32 waveDataSize;                           // at 0x1C
    Util::DataRef<GroupItemTable> itemTableRef; // at 0x20
};
typedef Util::Table<Util::DataRef<GroupInfo> > GroupTable;

struct SoundArchivePlayerInfo {
    u16 seqSoundCount;    // at 0x0
    u16 seqTrackCount;    // at 0x2
    u16 strmSoundCount;   // at 0x4
    u16 strmTrackCount;   // at 0x6
    u16 strmChannelCount; // at 0x8
    u16 waveSoundCount;   // at 0xA
    u16 waveTrackCount;   // at 0xC
};

struct Info {
    Util::DataRef<SoundCommonTable> soundTableRef;                   // at 0x0
    Util::DataRef<BankTable> bankTableRef;                           // at 0x8
    Util::DataRef<PlayerTable> playerTableRef;                       // at 0x10
    Util::DataRef<FileTable> fileTableRef;                           // at 0x18
    Util::DataRef<GroupTable> groupTableRef;                         // at 0x20
    Util::DataRef<SoundArchivePlayerInfo> soundArchivePlayerInfoRef; // at 0x28
};

struct InfoBlock {
    ut::BinaryBlockHeader blockHeader; // at 0x0
    Info info;                         // at 0x8
};

/******************************************************************************
 *
 * File header
 *
 ******************************************************************************/
struct Header {
    ut::BinaryFileHeader fileHeader; // at 0x0
    u32 symbolDataOffset;            // at 0x10
    u32 symbolDataSize;              // at 0x14
    u32 infoOffset;                  // at 0x18
    u32 infoSize;                    // at 0x1C
    u32 fileImageOffset;             // at 0x20
    u32 fileImageSize;               // at 0x24
};

// TODO: How is this calculated?
static const int HEADER_AREA_SIZE = ROUND_UP(sizeof(Header), 32) + 40;

} // namespace SoundArchiveFile

class SoundArchiveFileReader {
public:
    static const u32 SIGNATURE = 'RSAR';
    static const int VERSION = NW4R_VERSION(1, 3);

public:
    SoundArchiveFileReader();

    void Init(const void* pSoundArchiveBin);
    bool IsValidFileHeader(const void* pSoundArchiveBin);

    void SetStringChunk(const void* pChunk, u32 size);
    void SetInfoChunk(const void* pChunk, u32 size);

    SoundType GetSoundType(u32 id) const;
    bool ReadSoundInfo(u32 id, SoundArchive::SoundInfo* pInfo) const;
    bool ReadSound3DParam(u32 id, SoundArchive::Sound3DParam* pParam) const;
    bool ReadSeqSoundInfo(u32 id, SoundArchive::SeqSoundInfo* pInfo) const;
    bool ReadStrmSoundInfo(u32 id, SoundArchive::StrmSoundInfo* pInfo) const;
    bool ReadWaveSoundInfo(u32 id, SoundArchive::WaveSoundInfo* pInfo) const;
    bool ReadBankInfo(u32 id, SoundArchive::BankInfo* pInfo) const;
    bool ReadPlayerInfo(u32 id, SoundArchive::PlayerInfo* pInfo) const;
    bool ReadGroupInfo(u32 id, SoundArchive::GroupInfo* pInfo) const;
    bool ReadGroupItemInfo(u32 groupId, u32 itemId,
                           SoundArchive::GroupItemInfo* pInfo) const;
    bool ReadSoundArchivePlayerInfo(
        SoundArchive::SoundArchivePlayerInfo* pInfo) const;

    u32 GetSoundStringId(u32 id) const;
    u32 GetPlayerCount() const;
    u32 GetGroupCount() const;

    const char* GetSoundLabelString(u32 id) const;
    u32 GetSoundUserParam(u32 id) const;

    bool ReadFileInfo(u32 id, SoundArchive::FileInfo* pInfo) const;
    bool ReadFilePos(u32 fileId, u32 id, SoundArchive::FilePos* pPos) const;

    const char* GetString(u32 id) const;

    u32 ConvertLabelStringToSoundId(const char* pLabel) const {
        return ConvertLabelStringToId(mStringTreeSound, pLabel);
    }
    u32 ConvertLabelStringToPlayerId(const char* pLabel) const {
        return ConvertLabelStringToId(mStringTreePlayer, pLabel);
    }
    u32 ConvertLabelStringToGroupId(const char* pLabel) const {
        return ConvertLabelStringToId(mStringTreeGroup, pLabel);
    }

    u16 GetVersion() const {
        return mHeader.fileHeader.version;
    }

    u32 GetLabelStringChunkOffset() const {
        return mHeader.symbolDataOffset;
    }

    u32 GetLabelStringChunkSize() const {
        return mHeader.symbolDataSize;
    }

    u32 GetInfoChunkOffset() const {
        return mHeader.infoOffset;
    }

    u32 GetInfoChunkSize() const {
        return mHeader.infoSize;
    }

private:
    u32 ConvertLabelStringToId(const SoundArchiveFile::StringTree* pTree,
                               const char* pLabel) const;

    const SoundArchiveFile::SoundCommonInfo* impl_GetSoundInfo(u32 id) const;
    SoundArchiveFile::SoundInfoOffset impl_GetSoundInfoOffset(u32 id) const
        DECOMP_DONT_INLINE;

    const SoundArchiveFile::SeqSoundInfo* impl_GetSeqSoundInfo(u32 id) const;
    const SoundArchiveFile::StrmSoundInfo* impl_GetStrmSoundInfo(u32 id) const;
    const SoundArchiveFile::WaveSoundInfo* impl_GetWaveSoundInfo(u32 id) const;

    const SoundArchiveFile::BankInfo* impl_GetBankInfo(u32 id) const;
    const SoundArchiveFile::PlayerInfo* impl_GetPlayerInfo(u32 id) const;
    const SoundArchiveFile::GroupInfo* impl_GetGroupInfo(u32 id) const;

    const void* GetPtrConst(const void* pBase, u32 offset) const {
        if (offset == 0) {
            return NULL;
        }

        return ut::AddOffsetToPtr(pBase, offset);
    }

private:
    SoundArchiveFile::Header mHeader;                      // at 0x0
    const SoundArchiveFile::Info* mInfo;                   // at 0x28
    const void* mStringBase;                               // at 0x2C
    const SoundArchiveFile::StringTable* mStringTable;     // at 0x30
    const SoundArchiveFile::StringTree* mStringTreeSound;  // at 0x34
    const SoundArchiveFile::StringTree* mStringTreePlayer; // at 0x38
    const SoundArchiveFile::StringTree* mStringTreeGroup;  // at 0x3C
    const SoundArchiveFile::StringTree* mStringTreeBank;   // at 0x40
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
