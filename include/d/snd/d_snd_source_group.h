#ifndef D_SND_SOURCE_GROUP_H
#define D_SND_SOURCE_GROUP_H

#include "common.h"
#include "d/snd/d_snd_misc.h"
#include "d/snd/d_snd_types.h"
#include "d/snd/d_snd_util.h"
#include "nw4r/ut/ut_list.h"
#include "sized_string.h"

/**
 * All similar dSoundSource_c's are put in a group. The game can then find the closest
 * source for a given type by name/id, for example for changing things about the music
 * depending on proximity to a certain object or enemy.
 *
 * Through its dSndSourceParam, it can also "send" parameters to all dSnd3DActors
 * corresponding to these sound sources.
 *
 * Size: 0x70
 */
class dSndSourceGroup_c {
public:
    dSndSourceGroup_c();
    dSndSourceGroup_c(s32 sourceType, const char *name, const char *origName, s32 subtype);
    ~dSndSourceGroup_c();

    void set(s32 sourceType, const char *name, const char *origName, s32 subtype);
    void setTemp(s32 sourceType, const char *name, s32 subtype);

    void registerSource(dSoundSource_c *);
    void unregisterSource(dSoundSource_c *);

    dSndSourceParam *getAmbientParam() {
        return &mParam;
    }

    dSoundSource_c *getSourceClosestToListener();
    dSoundSource_c *getSourceClosestToPlayer();

    s32 getNumSources() const;
    void setParam(s32 type, const char *name);
    void calc();
    void clear();
    void clearTemp();

    const char *getName() const {
        return mName;
    }

    const char *getOrigName() const {
        return mpOrigName;
    }

    s32 getSourceType() const {
        return mSourceType;
    }

    bool isActive() const {
        return mIsActive;
    }

    void setIsActive(bool active) {
        mIsActive = active;
    }

private:
    bool setParamFromName(const char *name);
    void resetSoundSourceParam();
    void clearList();

    /* 0x00 */ nw4r::ut::Node mNode1;
    /* 0x08 */ nw4r::ut::Node mNode2;
    /* 0x10 */ s32 mSourceType;
    /* 0x14 */ s32 mSourceCategory;
    /* 0x18 */ s32 mSubtype;
    /* 0x1C */ bool mIsActive;
    /* 0x1D */ bool field_0x1D;
    /* 0x1E */ SizedString<32> mName;
    /* 0x40 */ const char *mpOrigName;
    /* 0x44 */ LIST_MEMBER(dSoundSource_c, SubSource);
    /* 0x50 */ dSndSourceParam mParam;
    /* 0x64 */ dSoundSource_c *mpCachedClosestSourceToListener;
    /* 0x68 */ dSoundSource_c *mpCachedClosestSourceToPlayer;
    /* 0x6C */ bool mCalculatedClosestToListener;
    /* 0x6D */ bool mCalculatedClosestToPlayer;
};

#endif
