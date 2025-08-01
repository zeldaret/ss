#ifndef D_SND_PLAYER_MGR_H
#define D_SND_PLAYER_MGR_H

#include "d/snd/d_snd_mgr.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/snd/snd_MemorySoundArchive.h"
#include "nw4r/snd/snd_SoundArchivePlayer.h"
#include "nw4r/snd/snd_SoundHandle.h"
#include "nw4r/snd/snd_SoundStartable.h"

SND_DISPOSER_FORWARD_DECL(dSndPlayerMgr_c);

/**
 * The main interface for managing sound stuff. Will delegate to dSndMgr_c for
 * most things, but handles demo (cutscene) sound effects (SE_DEMO*) by itself.
 *
 * Previous Ghidra name: BgmMgr
 */
class dSndPlayerMgr_c {
public:
    SND_DISPOSER_MEMBERS(dSndPlayerMgr_c);

public:
    dSndPlayerMgr_c();

    void setup(s32 channel);
    void shutdown(s32 channel);

    void shutdown();
    void calc();

    void enterMenu();
    void leaveMenu();
    void enterMap();
    void leaveMap();
    void enterHelp();
    void leaveHelp();

    void enterHbm();
    void leaveHbm();

    void fn_8035E1B0(u16);
    bool fn_8035E220();
    void fn_8035E250(u16);
    bool fn_8035E2E0();
    void fn_8035E310();
    void fn_8035E620();
    void fn_8035E6E0();

    u32 getFreeSize();
    bool loadDemoArchive(const char *demoArchiveName);
    const char *getSoundArchivePath();
    u32 convertLabelStringToSoundId(const char *label) const;

    nw4r::snd::SoundArchivePlayer &getSoundArchivePlayerForType(u8 sourceType);
    bool canUseThisPlayer(u8 sourceType) const;

    enum PlayerMgrFlag_e {
        MGR_HBM = 0x1,
        MGR_UNK_0x2 = 0x2,
        MGR_PAUSE = 0x4,
        MGR_MAP = 0x8,
        MGR_HELP = 0x10,
    };

    bool checkFlag(u32 mask) const {
        return mFlags & mask;
    }

    void onFlag(u32 mask) {
        mFlags |= mask;
    }

    void offFlag(u32 mask) {
        mFlags &= ~mask;
    }

    u32 getEventMuteMask(u32 id) {
        nw4r::snd::SoundArchive *archive;
        if (mSoundArchivePlayer.IsAvailable()) {
            archive = &mSoundArchive;
        } else {
            archive = dSndMgr_c::GetInstance()->getArchive();
        }
        return archive->GetSoundUserParam(id) & sEventMuteFlagsMask;
    }

    enum FanfareUnmuteParam_e {
        FANFARE_UNMUTE_BGM = 0x10000,
        FANFARE_UNMUTE_STAGE_EFFECTS = 0x20000,
        FANFARE_UNMUTE_ENEMY = 0x40000,
        FANFARE_UNMUTE_OBJECTS = 0x80000,
    };

    // seem to be used for Small and Event sounds
    enum UserParamEventSmall_e {
        MUTE_BGM_PART = 0x10000,
        MUTE_BGM_FULL = 0x20000,
        MUTE_STAGE_EFFECTS_PART = 0x40000,
        MUTE_STAGE_EFFECTS_FULL = 0x80000,
        MUTE_ENEMY_PART = 0x100000,
        MUTE_ENEMY_FULL = 0x200000,
        MUTE_OBJECTS_PART = 0x400000,
        MUTE_OBJECTS_FULL = 0x800000,
    };

    static const u32 sEventMuteFlagsMask;

    // For a full overview over which sounds are played by which players,
    // see https://gist.github.com/robojumper/f81a55857cebed2c79cf3cf3c1ec8f04
    enum PlayerIndex_e {
        /** Background music */
        PLAYER_BGM = 0,
        /** Background music, boss battle */
        PLAYER_BGM_BOSS = 1,
        /** "fanfare?" */
        PLAYER_FAN = 2,
        /** small sound effects, only a few (UI navigation related) */
        PLAYER_SMALL_IMPORTANT = 3,
        /** small sound, often UI related */
        PLAYER_SMALL_NORMAL = 4,
        /** hit, roll attack, cloth, sink */
        PLAYER_LINK_BODY = 5,
        /** Link voice */
        PLAYER_LINK_VOICE = 6,
        /** Link footstep, swimming */
        PLAYER_LINK_FOOTSTEP = 7,
        /** Sword, beetle, ... */
        PLAYER_LINK_EQUIPMENT = 8,
        /** Enemy sound effects */
        PLAYER_ENEMY = 9,
        /** Enemy voices */
        PLAYER_ENEMY_VOICE = 10,
        /** Enemy footstep */
        PLAYER_ENEMY_FOOTSTEP = 11,
        /** Misc objects */
        PLAYER_OBJECT_1 = 12,
        /** Misc objects */
        PLAYER_OBJECT_2 = 13,
        /** NPC footsteps */
        PLAYER_NPC_FOOTSTEP = 14,
        /** NPC voices */
        PLAYER_NPC_VOICE = 15,
        /** leaves, grass */
        PLAYER_FOLIAGE = 16,
        /** misc TgSound environmental effects */
        PLAYER_TG_SOUND = 17,
        /** area sound effects (Faron birds...) */
        PLAYER_AREA = 18,
        /** not sure, only SE_A_IN_WATER_LV */
        PLAYER_AREA_IN_WATER_LV = 19,
        /** event */
        PLAYER_EVENT = 20,
        /** not sure */
        PLAYER_UNK_21 = 21,
    };

private:
    /* 0x010 */ u8 field_0x010;
    /* 0x011 */ u8 field_0x011;
    /* 0x014 */ s32 field_0x014;
    /* 0x018 */ s32 field_0x018;
    /* 0x01C */ s32 field_0x01C;
    /* 0x020 */ u32 mFlags;

    // system menu, inventory, map
    void enterPauseState();
    void leavePauseState();

    virtual nw4r::snd::SoundStartable::StartResult
    startSound(nw4r::snd::SoundHandle *pHandle, u32 soundId, const nw4r::snd::SoundStartable::StartInfo *pStartInfo);
    virtual nw4r::snd::SoundStartable::StartResult startSound(
        nw4r::snd::SoundHandle *pHandle, const char *soundLabel, const nw4r::snd::SoundStartable::StartInfo *pStartInfo
    );

    /* 0x028 */ nw4r::snd::MemorySoundArchive mSoundArchive;
    /* 0x178 */ nw4r::snd::SoundArchivePlayer mSoundArchivePlayer;
};

#endif
