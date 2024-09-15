#ifndef NW4R_SND_SOUND_ARCHIVE_H
#define NW4R_SND_SOUND_ARCHIVE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_Common.h>

#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

enum SoundType {
    SOUND_TYPE_INVALID,
    SOUND_TYPE_SEQ,
    SOUND_TYPE_STRM,
    SOUND_TYPE_WAVE
};

class SoundArchive {
public:
    struct SoundInfo {
        u32 fileId;                // at 0x0
        u32 playerId;              // at 0x4
        int playerPriority;        // at 0x8
        int volume;                // at 0xC
        int remoteFilter;          // at 0x10
        detail::PanMode panMode;   // at 0x14
        detail::PanCurve panCurve; // at 0x18
    };

    struct SeqSoundInfo {
        u32 dataOffset;              // at 0x0
        u32 bankId;                  // at 0x4
        u32 allocTrack;              // at 0x8
        int channelPriority;         // at 0xC
        bool releasePriorityFixFlag; // at 0x10
    };

    struct StrmSoundInfo {};

    struct WaveSoundInfo {
        int subNo;                   // at 0x0
        int channelPriority;         // at 0x4
        bool releasePriorityFixFlag; // at 0x8
    };

    struct Sound3DParam {
        u32 flags;     // at 0x0
        u8 decayCurve; // at 0x4
        u8 decayRatio; // at 0x5
    };

    struct BankInfo {
        u32 fileId; // at 0x0
    };

    struct PlayerInfo {
        int playableSoundCount; // at 0x0
        u32 heapSize;           // at 0x4
    };

    struct GroupInfo {
        u32 itemCount;           // at 0x0
        const char* extFilePath; // at 0x4
        u32 offset;              // at 0x8
        u32 size;                // at 0xC
        u32 waveDataOffset;      // at 0x10
        u32 waveDataSize;        // at 0x14
    };
    struct GroupItemInfo {
        u32 fileId;         // at 0x0
        u32 offset;         // at 0x4
        u32 size;           // at 0x8
        u32 waveDataOffset; // at 0xC
        u32 waveDataSize;   // at 0x10
    };

    struct FileInfo {
        u32 fileSize;            // at 0x0
        u32 waveDataFileSize;    // at 0x4
        const char* extFilePath; // at 0x8
        u32 filePosCount;        // at 0xC
    };
    struct FilePos {
        u32 groupId; // at 0x0
        u32 index;   // at 0x4
    };

    struct SoundArchivePlayerInfo {
        int seqSoundCount;    // at 0x0
        int seqTrackCount;    // at 0x4
        int strmSoundCount;   // at 0x8
        int strmTrackCount;   // at 0xC
        int strmChannelCount; // at 0x10
        int waveSoundCount;   // at 0x14
        int waveTrackCount;   // at 0x18
    };

    static const u32 INVALID_ID = 0xFFFFFFFF;

public:
    SoundArchive();
    virtual ~SoundArchive(); // at 0x8

    virtual const void* detail_GetFileAddress(u32 id) const = 0; // at 0xC

    virtual const void*
    detail_GetWaveDataFileAddress(u32 id) const = 0; // at 0x10

    virtual int detail_GetRequiredStreamBufferSize() const = 0; // at 0x14

    virtual ut::FileStream* OpenStream(void* pBuffer, int bufferSize,
                                       u32 offset,
                                       u32 length) const = 0; // at 0x18

    virtual ut::FileStream* OpenExtStream(void* pBuffer, int bufferSize,
                                          const char* pExtPath, u32 offset,
                                          u32 length) const = 0; // at 0x1C

    bool IsAvailable() const;

    void Setup(detail::SoundArchiveFileReader* pReader);
    void Shutdown();

    u32 GetPlayerCount() const;
    u32 GetGroupCount() const;

    const char* GetSoundLabelString(u32 id) const;
    u32 ConvertLabelStringToSoundId(const char* pLabel) const;
    u32 ConvertLabelStringToPlayerId(const char* pLabel) const;
    u32 ConvertLabelStringToGroupId(const char* pLabel) const;

    u32 GetSoundUserParam(u32 id) const;
    SoundType GetSoundType(u32 id) const;

    bool ReadSoundInfo(u32 id, SoundInfo* pInfo) const;
    bool detail_ReadSeqSoundInfo(u32 id, SeqSoundInfo* pInfo) const;
    bool detail_ReadStrmSoundInfo(u32 id, StrmSoundInfo* pInfo) const;
    bool detail_ReadWaveSoundInfo(u32 id, WaveSoundInfo* pInfo) const;

    bool ReadPlayerInfo(u32 id, PlayerInfo* pInfo) const;
    bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* pInfo) const;

    bool detail_ReadSound3DParam(u32 id, Sound3DParam* pParam) const;
    bool detail_ReadBankInfo(u32 id, BankInfo* pInfo) const;
    bool detail_ReadGroupInfo(u32 id, GroupInfo* pInfo) const;
    bool detail_ReadGroupItemInfo(u32 groupId, u32 itemId,
                                  GroupItemInfo* pInfo) const;

    bool detail_ReadFileInfo(u32 id, FileInfo* pInfo) const;
    bool detail_ReadFilePos(u32 fileId, u32 posId, FilePos* pPos) const;

    ut::FileStream* detail_OpenFileStream(u32 id, void* pBuffer,
                                          int bufferSize) const;
    ut::FileStream* detail_OpenGroupStream(u32 id, void* pBuffer,
                                           int bufferSize) const;
    ut::FileStream* detail_OpenGroupWaveDataStream(u32 id, void* pBuffer,
                                                   int bufferSize) const;

    void SetExternalFileRoot(const char* pExtFileRoot);

protected:
    static const int FILE_PATH_MAX = 256;

private:
    ut::FileStream* OpenExtStreamImpl(void* pBuffer, int bufferSize,
                                      const char* pExtPath, u32 offset,
                                      u32 size) const;

private:
    detail::SoundArchiveFileReader* mFileReader; // at 0x4
    char mExtFileRoot[FILE_PATH_MAX];            // at 0x8
};

} // namespace snd
} // namespace nw4r

#endif
