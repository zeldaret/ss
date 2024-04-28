#include "toBeSorted/bitwise_flag_helper.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/flag_space.h"
#include "toBeSorted/unk_flag_stuff.h"
#include <libc.h>

// TODO move
extern "C" UnkFlagDefinition lbl_80511AF0[];

class DungeonflagManager {
public:
    bool mShouldCommit;
    u16 mFlagIndex;
    UnkFlagStuff *mFlagStuff;
    FlagSpace mFlagSpace;

    static u16 sDungeonFlags[8];
    static DungeonflagManager *sInstance;

    void copyFromSave(s16 flag);
    void copyFromSave(u16 flagIndex);
    void setCommitFlag(u16 flag);
    DungeonflagManager();
    void setupFlagStuff();
    void setToValue(u16 flag, u32 value);
    void setFlag(u16 flag);
    u32 getDungeonFlag(u16 flag);
    bool doCommit();
};

/** 80575404 */
DungeonflagManager *DungeonflagManager::sInstance = nullptr;
/** 805a9c58 */
u16 DungeonflagManager::sDungeonFlags[8] = {};

/** 800bf9e0 */
void DungeonflagManager::copyFromSave(u16 flagIndex) {
    u16 *ptr = FileManager::sInstance->getDungeonFlagsConst();
    mFlagSpace.copyFromSaveFile(ptr + (flagIndex & 0x1fff) * 8, 0, 0x8);
}

/** 800bf930 */
void DungeonflagManager::setCommitFlag(u16 flag) {
    mShouldCommit = true;
}

/** 0x800bf940 */
DungeonflagManager::DungeonflagManager()
    : mShouldCommit(false), mFlagIndex(-1), mFlagStuff(nullptr),
      mFlagSpace(sDungeonFlags, ARRAY_LENGTH(sDungeonFlags)) {}

/** 800bf980 */
void DungeonflagManager::setupFlagStuff() {
    mFlagStuff = new UnkFlagStuff(0x11, lbl_80511AF0);
    mShouldCommit = false;
}

/** 800bf9e0 */
void DungeonflagManager::copyFromSave(s16 flag) {
    mFlagIndex = flag;
    copyFromSave((u16)flag);
}

/** 800bf9f0 */
void DungeonflagManager::setToValue(u16 flag, u32 value) {
    u16 *data = mFlagSpace.getFlagPtrChecked();
    mFlagStuff->setCounterOrFlag(flag, data, 8, value);
    setCommitFlag(flag);
}

/** 800bfa60 */
void DungeonflagManager::setFlag(u16 flag) {
    u16 *data = mFlagSpace.getFlagPtrChecked();
    mFlagStuff->setCounterOrFlag(flag, data, 8, 1);
    setCommitFlag(flag);
}

/** 800fbac0 */
u32 DungeonflagManager::getDungeonFlag(u16 flag) {
    u16 *data = mFlagSpace.getFlagPtrUnchecked();
    return mFlagStuff->getCounterOrFlag(flag, data, 8);
}

/** 800fbb10 */
bool DungeonflagManager::doCommit() {
    FileManager *instance;
    u16 idx = mFlagIndex;
    if (idx == 0xFFFF) {
        return false;
    } else if (mShouldCommit) {
        instance = FileManager::sInstance;
        u16 *ptr = mFlagSpace.getFlagPtrUnchecked();
        instance->setDungeonFlags(ptr, (idx & 0x1fff) * 8, 0x08);
        mShouldCommit = false;
        return true;
    }
    return false;
}
