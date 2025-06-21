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

    static dSoundSource_c *createSource(u32 id, dAcBase_c *actor, const char *name, u8 subtype);

    void registerSource(dSoundSource_c *source);
    void unregisterSource(dSoundSource_c *source);
    void onShutdownSource(dSoundSource_c *source);

    void playFlowSound(u32 id);
    static s32 getSourceCategoryForSourceType(s32 sourceType, const char *name);
    static bool isSwOrEOc(const char *name);

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

private:
    static bool isCertainEnemyType(dSoundSource_c *source);
    void removeSourceFromList(dSoundSource_c *source, nw4r::ut::List *list);
    void clearSourceLists();
    void clearSourceList(nw4r::ut::List *list);
    s32 getPlayerSourceRoomId() const;

    /* 0x0010 */ u8 field_0x0010;
    /* 0x0011 */ u8 field_0x0011;
    /* 0x0012 */ u8 field_0x0012;
    /* 0x0013 */ u8 field_0x0013;

    /* 0x0014 */ dSndSourceGroup_c *mpDefaultGroup;
    /* 0x0018 */ nw4r::ut::List mGroupList1;
    /* 0x0024 */ nw4r::ut::List mGroupList2;
    /* 0x0030 */ nw4r::ut::List mGroupList3;
    /* 0x003C */ dSndSourceGroup_c mGroups[NUM_GROUPS];

    // Not sure what these are for
    /* 0x383C */ nw4r::ut::List mAllSources; // node offset 0xE8, probably into dSoundSource::mMgrLink
    /* 0x3848 */ nw4r::ut::List field_0x3848; // node offset 0x15C, possibly a subset of source types
    /* 0x3854 */ nw4r::ut::List field_0x3854; // node offset 0x160

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
