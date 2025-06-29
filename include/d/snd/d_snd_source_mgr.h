#ifndef D_SND_SOURCE_MGR_H
#define D_SND_SOURCE_MGR_H

#include "d/snd/d_snd_source.h"
#include "d/snd/d_snd_source_group.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/ut/ut_list.h"

SND_DISPOSER_FORWARD_DECL(dSndSourceMgr_c);

class dSndSourceMgr_c {
    SND_DISPOSER_MEMBERS(dSndSourceMgr_c);

    static const u32 NUM_GROUPS = 128;

public:
    dSndSourceMgr_c();

    static dSoundSourceIf_c *createSource(s32 sourceType, dAcBase_c *actor, const char *name, u8 subtype);

    void registerSource(dSoundSource_c *source);
    void unregisterSource(dSoundSource_c *source);
    void onShutdownSource(dSoundSource_c *source);

    // Only for sources 10, 11, 12, 13, 14
    void registerUnkEnemyType(dSndSourceEnemy_c *);

    void playFlowSound(u32 id);
    static s32 getSourceCategoryForSourceType(s32 sourceType, const char *name);
    static bool isSwOrEOc(const char *name);
    s32 getPlayerSourceRoomId() const;

    static dSoundSource_c *getPlayerSource() {
        return GetInstance()->mpPlayerSource;
    }

    static dSoundSource_c *getBoomerangSource() {
        return GetInstance()->mpBoomerangSource;
    }

    u8 getField_0x0012() const {
        return field_0x0012;
    }

    u8 getField_0x0013() const {
        return field_0x0013;
    }

    void stopAllSound();
    void stopAllNonPlayerSound();

    void incrementEnemyCount() {
        field_0x3868++;
    }

    void decrementEnemyCount() {
        field_0x3868--;
    }

private:
    void calcEnemyObjVolume();

    static bool isCertainEnemyType(dSoundSource_c *source);
    static bool isAnimSoundSource(s32 sourceType, const char *name);
    static bool isMultiSoundSource(s32 sourceType, const char *name);

    void removeSourceFromList(dSoundSource_c *source, nw4r::ut::List *list);
    void clearSourceLists();
    void clearSourceList(nw4r::ut::List *list);

    dSndSourceGroup_c *getActiveGroupForName(const char *name);
    dSndSourceGroup_c *getInactiveGroup();
    dSndSourceGroup_c *getGroup(s32 sourceType, dAcBase_c *actor, const char *name, const char *origName, u8 subtype);
    bool fn_803846D0(s32 sourceType, const char *name, u8 subtype);

    void activateGroup(dSndSourceGroup_c *group);
    bool isActiveGroup(dSndSourceGroup_c *group) const;

    /* 0x0010 */ u8 field_0x0010;
    /* 0x0011 */ u8 field_0x0011;
    /* 0x0012 */ u8 field_0x0012;
    /* 0x0013 */ u8 field_0x0013;

    /* 0x0014 */ dSndSourceGroup_c *mpDefaultGroup;
    /* 0x0018 */ LIST_MEMBER(dSndSourceGroup_c, Group1);
    /* 0x0024 */ LIST_MEMBER(dSndSourceGroup_c, Group2);
    /* 0x0030 */ LIST_MEMBER(dSndSourceGroup_c, Group3);
    /* 0x003C */ dSndSourceGroup_c mGroups[NUM_GROUPS];

    // Not sure what these are for
    /* 0x383C */ LIST_MEMBER(dSoundSource_c, AllSources); // node offset 0xE8, -> dSoundSource_c::mMgrLink
    /* 0x3848 */ nw4r::ut::List field_0x3848;             // node offset 0x15C, -> dSndSourceEnemy_c::mMgrEnemyLink
    /* 0x3854 */ LIST_MEMBER(dSndSourceHarpRelated_c, HarpRelated); // node offset 0x160, -> dSndSourceHarpRelated_c

    /* 0x3860 */ UNKWORD field_0x3860;
    /* 0x3864 */ UNKWORD field_0x3864;
    /* 0x3868 */ UNKWORD field_0x3868;
    /* 0x386C */ f32 field_0x386C;

    /* 0x3870 */ dSoundSource_c *mpPlayerSource;
    /* 0x3874 */ dSoundSource_c *mpKenseiSource;
    /* 0x3878 */ dSoundSource_c *mpBoomerangSource;
    /* 0x387C */ dSoundSource_c *mpTBoatSource;
    /* 0x3880 */ dSoundSource_c *field_0x3880; // fi singing related?
    /* 0x3884 */ dSoundSource_c *field_0x3884;
};

#endif
