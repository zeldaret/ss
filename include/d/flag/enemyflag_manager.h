#ifndef D_FLAG_ENEMYFLAG_MANAGER_H
#define D_FLAG_ENEMYFLAG_MANAGER_H

#include "d/flag/bitwise_flag_helper.h"
#include "d/flag/committable_flag_manager.h"
#include "d/flag/flag_space.h"
#include "toBeSorted/file_manager.h"

class EnemyDefeatManager : public CommittableFlagManager {
public:
    FlagSpace mFlagSpace;
    BitwiseFlagHelper mFlagHelper;
    u16 mSceneIndex;

    static u16 sEnemyDefeatFlags[4096];

    static EnemyDefeatManager *sInstance;

    void clearSavedFlags();
    bool checkUncommittedFlag(u16 flag);
    u16 checkUncommittedFlag2(u16 flag) {
        return checkUncommittedFlag(flag);
    }
    EnemyDefeatManager();
    void init();
    void copyFromSave(u16 sceneIndex);
    void updateFlagIndex(u16 sceneIndex);
    void clearAll();
    bool checkIsValidFlag(u16 flag);
    bool checkFlag(u16 flag);
    virtual ~EnemyDefeatManager() {}
    virtual u16 getFlagCount() const;
    void setFlag(u16 flag);

    virtual void doCommit() override {
        FileManager *mgr = FileManager::sInstance;
        mgr->setEnemyDefeatFlags(mFlagSpace.getFlagPtrUnchecked(), 0, 0x1000);
    };
};

#endif
