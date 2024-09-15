#ifndef NW4R_SND_SOUND_STARTABLE_H
#define NW4R_SND_SOUND_STARTABLE_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BasicSound.h>

namespace nw4r {
namespace snd {

// Forward declarations
class SoundHandle;
namespace detail {
class ExternalSoundPlayer;
}

class SoundStartable {
public:
    enum StartResult {
        START_SUCCESS,
        START_ERR_LOW_PRIORITY,
        START_ERR_INVALID_LABEL_STRING,
        START_ERR_INVALID_SOUNDID,
        START_ERR_NOT_DATA_LOADED,
        START_ERR_NOT_ENOUGH_PLAYER_HEAP,
        START_ERR_CANNOT_OPEN_FILE,
        START_ERR_NOT_AVAILABLE,
        START_ERR_CANNOT_ALLOCATE_TRACK,
        START_ERR_NOT_ENOUGH_INSTANCE,
        START_ERR_INVALID_PARAMETER,
        START_ERR_INVALID_SEQ_START_LOCATION_LABEL,

        START_ERR_USER = 128,
        START_ERR_UNKNOWN = 255,
    };

    struct StartInfo {
        enum EnableFlag {
            ENABLE_START_OFFSET = (1 << 0),
            ENABLE_PLAYER_ID = (1 << 1),
            ENABLE_PLAYER_PRIORITY = (1 << 2)
        };

        enum StartOffsetType {
            START_OFFSET_TYPE_MILLISEC,
            START_OFFSET_TYPE_TICK,
            START_OFFSET_TYPE_SAMPLE
        };

        u32 enableFlag;                  // at 0x0
        StartOffsetType startOffsetType; // at 0x4
        int startOffset;                 // at 0x8
        u32 playerId;                    // at 0xC
        int playerPriority;              // at 0x10
        int voiceOutCount;               // at 0x14
    };

public:
    virtual ~SoundStartable() {} // at 0x8

    virtual StartResult
    detail_SetupSound(SoundHandle* pHandle, u32 id,
                      detail::BasicSound::AmbientArgInfo* pArgInfo,
                      detail::ExternalSoundPlayer* pPlayer, bool hold,
                      const StartInfo* pStartInfo) = 0; // at 0xC

    virtual u32
    detail_ConvertLabelStringToSoundId(const char* pLabel) = 0; // at 0x10

    bool StartSound(SoundHandle* pHandle, u32 id) {
        return detail_StartSound(pHandle, id, NULL, NULL, NULL) ==
               START_SUCCESS;
    }
    bool StartSound(SoundHandle* pHandle, unsigned int id) {
        return detail_StartSound(pHandle, id, NULL, NULL, NULL) ==
               START_SUCCESS;
    }
    bool StartSound(SoundHandle* pHandle, int id) {
        return detail_StartSound(pHandle, id, NULL, NULL, NULL) ==
               START_SUCCESS;
    }

    bool HoldSound(SoundHandle* pHandle, u32 id) {
        return detail_HoldSound(pHandle, id, NULL, NULL, NULL) == START_SUCCESS;
    }
    bool HoldSound(SoundHandle* pHandle, unsigned int id) {
        return detail_HoldSound(pHandle, id, NULL, NULL, NULL) == START_SUCCESS;
    }
    bool HoldSound(SoundHandle* pHandle, int id) {
        return detail_HoldSound(pHandle, id, NULL, NULL, NULL) == START_SUCCESS;
    }

    bool PrepareSound(SoundHandle* pHandle, u32 id) {
        return detail_PrepareSound(pHandle, id, NULL, NULL, NULL) ==
               START_SUCCESS;
    }
    bool PrepareSound(SoundHandle* pHandle, unsigned int id) {
        return detail_PrepareSound(pHandle, id, NULL, NULL, NULL) ==
               START_SUCCESS;
    }
    bool PrepareSound(SoundHandle* pHandle, int id) {
        return detail_PrepareSound(pHandle, id, NULL, NULL, NULL) ==
               START_SUCCESS;
    }

private:
    StartResult detail_StartSound(SoundHandle* pHandle, u32 id,
                                  detail::BasicSound::AmbientArgInfo* pArgInfo,
                                  detail::ExternalSoundPlayer* pPlayer,
                                  const StartInfo* pStartInfo);

    StartResult detail_HoldSound(SoundHandle* pHandle, u32 id,
                                 detail::BasicSound::AmbientArgInfo* pArgInfo,
                                 detail::ExternalSoundPlayer* pPlayer,
                                 const StartInfo* pStartInfo);

    StartResult
    detail_PrepareSound(SoundHandle* pHandle, u32 id,
                        detail::BasicSound::AmbientArgInfo* pArgInfo,
                        detail::ExternalSoundPlayer* pPlayer,
                        const StartInfo* pStartInfo);
};

} // namespace snd
} // namespace nw4r

#endif
