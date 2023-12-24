#include <common.h>
#include <libc.h>
#include <toBeSorted/bitwise_flag_helper.h>
#include <toBeSorted/file_manager.h>
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

bool CommittableFlagManager::commitIfNecessary() {
    if (mNeedsCommit) {
        doCommit();
        mNeedsCommit = false;
        return true;
    } else {
        return false;
    }
}

class TBoxFlagManager : public CommittableFlagManager {
    // class TBoxFlagManager {
    // bool mNeedsCommit;
    // u32 pad;
    FlagSpace mFlagSpace;
    u16 mSceneIndex;
    BitwiseFlagHelper mFlagHelper;

    static u16 *sFlags;

public:
    virtual void doCommit() override;
    bool checkUncommittedFlag(u16 flag);
    TBoxFlagManager();
    virtual ~TBoxFlagManager() {}
    void init();
    void copyFromSave(u16 sceneIndex);
    bool checkFlag(u16 sceneIndex, u16 flag);
    virtual u16 getFlagCount() const;
    void setFlag(u16 flag);
    bool checkUncommittedFlag(u16 sceneIndex, u16 flag);
    u16 checkUncommittedFlag2(u16 flag) {
        return checkUncommittedFlag(flag);
    }
};

void TBoxFlagManager::doCommit() {
    if (mSceneIndex != 0xFFFF) {
        FileManager::getInstance()->setTBoxFlags(mFlagSpace.getFlagPtrUnchecked(), mSceneIndex * 2, 2);
    }
}

bool TBoxFlagManager::checkUncommittedFlag(u16 flag) {
    return mFlagHelper.checkFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrUnchecked(), mFlagSpace.mCount);
}

TBoxFlagManager::TBoxFlagManager() : CommittableFlagManager(false), mFlagSpace(sFlags, 2) {
    mSceneIndex = 0xFFFF;
}

void TBoxFlagManager::init() {}

void TBoxFlagManager::copyFromSave(u16 sceneIndex) {
    // mr should be a clrlwi
    mSceneIndex = sceneIndex;
    u16 *flags = FileManager::getInstance()->getTBoxFlagsConst();
    mFlagSpace.copyFromSaveFile2(flags + (sceneIndex * 2), 0, 2);
}

bool TBoxFlagManager::checkFlag(u16 sceneIndex, u16 flag) {
    s32 actualFlag = (flag + sceneIndex * 0x20);
    return mFlagHelper.checkFlag(actualFlag / 16, flag % 16, FileManager::getInstance()->getTBoxFlagsConst(),
            getFlagCount());
}

u16 TBoxFlagManager::getFlagCount() const {
    return 0x200;
}

void TBoxFlagManager::setFlag(u16 flag) {
    if (checkUncommittedFlag2(flag) != 1) {
        mFlagHelper.setFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrChecked(), mFlagSpace.mCount);
        setNeedsCommit(true);
    }
}

class EnemyDefeatManager : public CommittableFlagManager {
public:
    FlagSpace mFlagSpace;
    BitwiseFlagHelper mFlagHelper;
    u16 mSceneIndex;

    static u16 *sFlags;

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

void EnemyDefeatManager::clearSavedFlags() {
    u16 empty[0x1000];
    memset(empty, 0, 0x2000);
    FileManager::getInstance()->setEnemyDefeatFlags(empty, 0, 0x1000);
}

bool EnemyDefeatManager::checkUncommittedFlag(u16 flag) {
    if (checkIsValidFlag(flag)) {
        return mFlagHelper.checkFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrUnchecked(), mFlagSpace.mCount);
    } else {
        return false;
    }
}

EnemyDefeatManager::EnemyDefeatManager() : CommittableFlagManager(false), mFlagSpace(sFlags, 12 /* later */) {}

void EnemyDefeatManager::init() {
    mSceneIndex = 0;
    clearAll();
}

void EnemyDefeatManager::copyFromSave(u16 sceneIndex) {
    mSceneIndex = sceneIndex;
    u16 count = mFlagSpace.mCount;
    u16 *flags = FileManager::getInstance()->getEnemyDefeatFlagsConst();
    mFlagSpace.copyFromSaveFile(flags, 0, count);
}

void EnemyDefeatManager::updateFlagIndex(u16 sceneIndex) {
    if (mSceneIndex == sceneIndex) {
        return;
    }
    mSceneIndex = sceneIndex;
    clearAll();
}

void EnemyDefeatManager::clearAll() {
    clearSavedFlags();
    mFlagSpace.unsetAll();
}

bool EnemyDefeatManager::checkIsValidFlag(u16 flag) {
    return flag < 0xFFFF;
}

bool EnemyDefeatManager::checkFlag(u16 flag) {
    if (!checkIsValidFlag(flag)) {
        return false;
    } else {
        u16 *pData = FileManager::getInstance()->getEnemyDefeatFlagsConst();
        return mFlagHelper.checkFlag(flag / 16, flag % 16, pData, getFlagCount());
    }
}

u16 EnemyDefeatManager::getFlagCount() const {
    return 0x1000;
}

void EnemyDefeatManager::setFlag(u16 flag) {
    if (checkUncommittedFlag2(flag) != 1 && checkIsValidFlag(flag)) {
        mFlagHelper.setFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrChecked(), mFlagSpace.mCount);
        setNeedsCommit(true);
    }
}
