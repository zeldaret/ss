#include "libc.h"
#include "toBeSorted/bitwise_flag_helper.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/flag_space.h"
#include "toBeSorted/unk_flag_stuff.h"


// TODO move
extern "C" UnkFlagDefinition DungeonflagManager__FLAG_DEFINITIONS[];

class DungeonflagManager {
public:
    bool mShouldCommit;
    u16 mStageIndex;
    UnkFlagStuff *mFlagStuff;
    FlagSpace mFlagSpace;

    static u16 sDungeonFlags[8];
    // static DungeonflagManager *sInstance;

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

/** 80575404 */
DungeonflagManager *DungeonflagManager::sInstance = nullptr;

/** 805a9c58 */
u16 DungeonflagManager::sDungeonFlags[8] = {};

/** 800bf8d0 */
void DungeonflagManager::copyFromSave_Internal(u16 flagIndex) {
    mFlagSpace.copyFromSaveFile(saveFilePtr(flagIndex), 0, 0x8);
}

/** 800bf930 */
void DungeonflagManager::setCommitFlag(u16 flag) {
    mShouldCommit = true;
}

/** 0x800bf940 */
DungeonflagManager::DungeonflagManager()
    : mShouldCommit(false), mStageIndex(-1), mFlagStuff(nullptr),
      mFlagSpace(sDungeonFlags, ARRAY_LENGTH(sDungeonFlags)) {}

/** 800bf980 */
void DungeonflagManager::init() {
    mFlagStuff = new UnkFlagStuff(0x11, DungeonflagManager__FLAG_DEFINITIONS);
    mShouldCommit = false;
}

/** 800bf9e0 */
void DungeonflagManager::copyFromSave(u32 flag) {
    mStageIndex = flag;
    copyFromSave_Internal(flag);
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
u16 DungeonflagManager::getDungeonFlag(u16 flag) {
    u16 *data = mFlagSpace.getFlagPtrUnchecked();
    return mFlagStuff->getCounterOrFlag(flag, data, 8);
}

/** 800fbb10 */
bool DungeonflagManager::doCommit() {
    FileManager *instance;
    u16 idx = mStageIndex;
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
