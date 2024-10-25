#include "d/flag/enemyflag_manager.h"

EnemyDefeatManager *EnemyDefeatManager::sInstance = nullptr;
u16 EnemyDefeatManager::sEnemyDefeatFlags[4096] = {};

/* 0x800BEAC0 */
void EnemyDefeatManager::clearSavedFlags() {
    u16 empty[0x1000];
    memset(empty, 0, 0x2000);
    FileManager::getInstance()->setEnemyDefeatFlags(empty, 0, 0x1000);
}

/* 0x800BEB00 */
bool EnemyDefeatManager::checkUncommittedFlag(u16 flag) {
    if (checkIsValidFlag(flag)) {
        return mFlagHelper.checkFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrUnchecked(), mFlagSpace.mCount);
    } else {
        return false;
    }
}

/* 0x800BEB80 */
EnemyDefeatManager::EnemyDefeatManager()
    : CommittableFlagManager(false), mFlagSpace(sEnemyDefeatFlags, ARRAY_LENGTH(sEnemyDefeatFlags)) {}

/* 0x800BEBC0 */
void EnemyDefeatManager::init() {
    mSceneIndex = 0;
    clearAll();
}

/* 0x800BEBD0 */
void EnemyDefeatManager::copyFromSave(u16 sceneIndex) {
    mSceneIndex = sceneIndex;
    u16 count = mFlagSpace.mCount;
    u16 *flags = FileManager::getInstance()->getEnemyDefeatFlagsConst();
    mFlagSpace.copyFromSaveFile(flags, 0, count);
}

/* 0x800BEC30 */
void EnemyDefeatManager::updateFlagIndex(u16 sceneIndex) {
    if (mSceneIndex == sceneIndex) {
        return;
    }
    mSceneIndex = sceneIndex;
    clearAll();
}

/* 0x800BEC50 */
void EnemyDefeatManager::clearAll() {
    clearSavedFlags();
    mFlagSpace.unsetAll();
}

/* 0x800BEC90 */
bool EnemyDefeatManager::checkIsValidFlag(u16 flag) {
    return flag < 0xFFFF;
}

/* 0x800BECB0 */
bool EnemyDefeatManager::checkFlag(u16 flag) {
    if (!checkIsValidFlag(flag)) {
        return false;
    } else {
        u16 *pData = FileManager::getInstance()->getEnemyDefeatFlagsConst();
        return mFlagHelper.checkFlag(flag / 16, flag % 16, pData, getFlagCount());
    }
}

/* 0x800BED50 */
u16 EnemyDefeatManager::getFlagCount() const {
    return 0x1000;
}

/* 0x800BED60 */
void EnemyDefeatManager::setFlag(u16 flag) {
    if (checkUncommittedFlag2(flag) != 1 && checkIsValidFlag(flag)) {
        mFlagHelper.setFlag(flag / 16, flag % 16, mFlagSpace.getFlagPtrChecked(), mFlagSpace.mCount);
        setNeedsCommit(true);
    }
}
