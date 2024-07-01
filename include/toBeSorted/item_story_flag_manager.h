#ifndef ITEM_STORY_FLAG_MANAGER_H
#define ITEM_STORY_FLAG_MANAGER_H

#include <toBeSorted/flag_space.h>
#include <toBeSorted/unk_flag_stuff.h>

// TODO These classes have an interesting relation and there are like 5 vtables, so
// the stuff in this header should not be relied upon for actually implementing these,
// but we need the interface

class ItemStoryManagerBase {
public:
    ItemStoryManagerBase();

    /** 0x08 */ virtual ~ItemStoryManagerBase();
    /** 0x0C */ virtual void setFlagszptr();
    /** 0x10 */ virtual void onDirty();
    /** 0x14 */ virtual void copyFlagsFromSave() = 0;
    /** 0x18 */ virtual void setupUnkFlagsStuff() = 0;
    /** 0x1C */ virtual bool doCommit() = 0;
    /** 0x20 */ virtual void setFlag(u16 flag);
    /** 0x24 */ virtual void unsetFlag(u16 flag);
    /** 0x28 */ virtual void setFlagOrCounterToValue(u16 flag, u16 value);
    /** 0x2C */ virtual u16 getCounterOrFlag(u16 flag) const;
    /** 0x30 */ virtual u16 getUncommittedValue(u16 flag);
    /** 0x34 */ virtual void unk3();
    /** 0x38 */ virtual u16 *getSaveFlagSpace() = 0;

    u16 getFlag(u16 flag) const;
};

class StoryFlagManager : public ItemStoryManagerBase {
    FlagSpace storyFlags;

public:
    static StoryFlagManager *sInstance;
};

class ItemFlagManager : public ItemStoryManagerBase {
    FlagSpace itemFlags;

public:
    static ItemFlagManager *sInstance;
};

#endif
