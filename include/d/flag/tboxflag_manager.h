#ifndef D_FLAG_TBOXFLAG_MANAGER_H
#define D_FLAG_TBOXFLAG_MANAGER_H

#include "d/flag/bitwise_flag_helper.h"
#include "d/flag/committable_flag_manager.h"
#include "d/flag/flag_space.h"

class TBoxflagManager : public CommittableFlagManager {
public:
    FlagSpace mFlagSpace;
    u16 mSceneIndex;
    BitwiseFlagHelper mFlagHelper;

    static u16 sTBoxFlags[2];

    static TBoxflagManager *sInstance;

    virtual void doCommit() override;
    bool checkUncommittedFlag(u16 flag);
    TBoxflagManager();
    virtual ~TBoxflagManager() {}
    void init();
    void copyFromSave(u32 sceneIndex);
    u16 checkFlag(u16 sceneIndex, u16 flag);
    virtual u16 getFlagCount() const;
    void setFlag(u16 flag);
    bool checkUncommittedFlag(u16 sceneIndex, u16 flag);
    u16 checkUncommittedFlag2(u16 flag) {
        return checkUncommittedFlag(flag);
    }
};

#endif
