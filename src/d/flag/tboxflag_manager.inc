#include "d/flag/tboxflag_manager.h"
#include "toBeSorted/file_manager.h"

TBoxFlagManager *TBoxFlagManager::sInstance = nullptr;
u16 TBoxFlagManager::sTBoxFlags[2] = {};

/* 0x800BE810 */
void TBoxFlagManager::doCommit() {
    if (mSceneIndex != 0xFFFF) {
        FileManager::getInstance()->setTBoxFlags(mFlagSpace.getFlagPtrUnchecked(), mSceneIndex * 2, 2);
    }
}

/* 0x800BE870 */
bool TBoxFlagManager::checkUncommittedFlag(u16 flag) {
    return mFlagHelper.checkFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrUnchecked(), mFlagSpace.mCount);
}

/* 0x800BE8E0 */
TBoxFlagManager::TBoxFlagManager() : CommittableFlagManager(false), mFlagSpace(sTBoxFlags, ARRAY_LENGTH(sTBoxFlags)) {
    mSceneIndex = 0xFFFF;
}

/* 0x800BE920 */
void TBoxFlagManager::init() {}

/* 0x800BE930 */
void TBoxFlagManager::copyFromSave(u32 sceneIndex) {
    u16 idx = sceneIndex;
    mSceneIndex = idx;
    u16 *flags = FileManager::getInstance()->getTBoxFlagsConst();
    mFlagSpace.copyFromSaveFile2(flags + (idx * 2), 0, 2);
}

/* 0x800BE990 */
bool TBoxFlagManager::checkFlag(u16 sceneIndex, u16 flag) {
    s32 actualFlag = (flag + sceneIndex * 0x20);
    return mFlagHelper.checkFlag(
        actualFlag / 16, flag % 16, FileManager::getInstance()->getTBoxFlagsConst(), getFlagCount()
    );
}

/* 0x800BEA30 */
u16 TBoxFlagManager::getFlagCount() const {
    return 0x200;
}

/* 0x800BEA40 */
void TBoxFlagManager::setFlag(u16 flag) {
    if (checkUncommittedFlag2(flag) != 1) {
        mFlagHelper.setFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrChecked(), mFlagSpace.mCount);
        setNeedsCommit(true);
    }
}
