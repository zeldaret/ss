#ifndef D_FLAG_DUNGEONFLAG_MANAGER_H
#define D_FLAG_DUNGEONFLAG_MANAGER_H

#include "d/flag/flag_index.h"
#include "d/flag/flag_space.h"
#include "toBeSorted/file_manager.h"

class DungeonflagManager {
public:
    bool mShouldCommit;
    u16 mStageIndex;
    FlagIndex *mpFlagIndex;
    FlagSpace mFlagSpace;

    static u16 sDungeonFlags[8];

    void copyFromSave(u32 flag);
    void copyFromSave_Internal(u16 flagIndex);
    void setCommitFlag(u16 flag);
    DungeonflagManager();
    void init();
    void setToValue(u16 flag, u32 value);
    void setFlag(u16 flag);
    u16 getDungeonFlag(u16 flag);
    bool doCommit();

    /** inline shenanigans to get copyFromSave to match */
    static inline u16 *saveFilePtr(u16 flagIndex) {
        u32 offset = (flagIndex & 0x1fff) * 8;
        return FileManager::sInstance->getDungeonFlagsConst() + offset;
    }

    static DungeonflagManager *sInstance;
};

#endif
