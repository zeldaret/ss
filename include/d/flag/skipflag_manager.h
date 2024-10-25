#ifndef D_FLAG_SKIPFLAG_MANAGER_H
#define D_FLAG_SKIPFLAG_MANAGER_H


#include "d/flag/bitwise_flag_helper.h"
#include "d/flag/flag_space.h"

class SkipflagManager {
public:
    SkipflagManager();
    void init();

    void copyFromSave();
    void setCommitFlag(u16 flag);
    void thunk_copyFromSave();
    void setFlag(u16 flag);
    bool checkFlag(u16 flag);
    bool commitFlags();
    static SkipflagManager *sInstance;

private:
    static u16 sSkipFlags[16];

    bool mShouldCommit;
    BitwiseFlagHelper mFlagHelper;
    FlagSpace mFlagSpace;
};

#endif
