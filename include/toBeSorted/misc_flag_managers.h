#ifndef MISC_FLAG_MANAGERS_H
#define MISC_FLAG_MANAGERS_H

#include <common.h>
#include <toBeSorted/bitwise_flag_helper.h>
#include <toBeSorted/flag_space.h>

class CommittableFlagManager {
public:
    bool mNeedsCommit;

    virtual void doCommit() = 0;
    bool commitIfNecessary();
    void setNeedsCommit(bool commit) {
        mNeedsCommit = commit;
    }
    CommittableFlagManager() {
        mNeedsCommit = false;
    }
    CommittableFlagManager(bool commit) {
        mNeedsCommit = commit;
    }
};

class TBoxFlagManager : public CommittableFlagManager {
public:
    FlagSpace mFlagSpace;
    u16 mSceneIndex;
    BitwiseFlagHelper mFlagHelper;

    static u16 sTBoxFlags[2];

    static TBoxFlagManager *sInstance;

    virtual void doCommit() override;
    bool checkUncommittedFlag(u16 flag);
    TBoxFlagManager();
    virtual ~TBoxFlagManager() {}
    void init();
    void copyFromSave(s16 sceneIndex);
    bool checkFlag(u16 sceneIndex, u16 flag);
    virtual u16 getFlagCount() const;
    void setFlag(u16 flag);
    bool checkUncommittedFlag(u16 sceneIndex, u16 flag);
    u16 checkUncommittedFlag2(u16 flag) {
        return checkUncommittedFlag(flag);
    }
};

// NOTE: Not actually Enemy Defeat.
// This is a little more than that, it keeps track of live objects based on their id as a whole
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
};

#endif
