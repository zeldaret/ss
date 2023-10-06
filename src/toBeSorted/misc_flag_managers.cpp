#include <types.h>
#include <toBeSorted/flag_space.h>
#include <toBeSorted/file_manager.h>
#include <toBeSorted/bitwise_flag_helper.h>

class CommittableFlagManager {
    public:
    bool mNeedsCommit;

    virtual void doCommit() = 0;
    bool commitIfNecessary();
    void setNeedsCommit(bool commit) {
        mNeedsCommit = commit;
    }
    // CommittableFlagManager(bool commit) {
    //     mNeedsCommit = commit;
    // }
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

class TBoxFlagManager: public CommittableFlagManager {
// class TBoxFlagManager {
    // bool mNeedsCommit;
    // u32 pad;
    FlagSpace mFlagSpace;
    u16 mSceneIndex;
    BitwiseFlagHelper mFlagHelper;

    static u16* sFlags;
    public:
    virtual void doCommit();
    bool checkUncommittedFlag(u16 flag);
    TBoxFlagManager();
    virtual ~TBoxFlagManager() {}
    void init();
    void copyFromSave(u16 sceneIndex);
    bool checkFlag(u16 sceneIndex, u16 flag);
    virtual s32 getFlagCount() const;
    void setFlag(u16 flag);
    bool checkUncommittedFlag(u16 sceneIndex, u16 flag);
};

void TBoxFlagManager::doCommit() {
    if (mSceneIndex != 0xFFFF) {
        FileManager::getInstance()->setTBoxFlags(mFlagSpace.getFlagPtrUnchecked(), mSceneIndex * 2, 2);
    }
}

bool TBoxFlagManager::checkUncommittedFlag(u16 flag) {
    return mFlagHelper.checkFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrUnchecked(), mFlagSpace.mCount);
}

TBoxFlagManager::TBoxFlagManager(): mFlagSpace(sFlags, 2) {
    mSceneIndex = 0xFFFF;
}

void TBoxFlagManager::init() {}

void TBoxFlagManager::copyFromSave(u16 sceneIndex) {
    mSceneIndex = sceneIndex;
    u16* flags = FileManager::getInstance()->getTBoxFlags2();
    mFlagSpace.copyFromSaveFile2(flags + sceneIndex * 2, 0, 2);
}

bool TBoxFlagManager::checkFlag(u16 sceneIndex, u16 flag) {
    s32 actualFlag = sceneIndex * 0x20 + flag;
    return mFlagHelper.checkFlag(actualFlag % 16, actualFlag / 16, FileManager::getInstance()->getTBoxFlags2(), getFlagCount());
}

s32 TBoxFlagManager::getFlagCount() const {
    return 0x200;
}

void TBoxFlagManager::setFlag(u16 flag) {

}
