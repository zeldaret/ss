#include "d/flag/baseflag_manager.h"

ItemStoryManagerBase::ItemStoryManagerBase(): mFlagCount(0), mFlagSizeBytes(0), mpFlagSpace(nullptr), mpFlagIndex(nullptr), mDirty(false) {}
ItemStoryManagerBase::~ItemStoryManagerBase() {
    if (mpFlagIndex != nullptr) {
        delete mpFlagIndex;
    }
}

void ItemStoryManagerBase::setFlagSizes(u16 flagCount, u16 flagSizeBytes) {
    mFlagCount = flagCount;
    mFlagSizeBytes = flagSizeBytes;
}

/** 800bf320 */
void ItemStoryManagerBase::createFlagIndex(FlagDefinition *def, u16 count) {
    mpFlagIndex = new FlagIndex(count, def);
}

/** 800bf380 */
void ItemStoryManagerBase::copyFromSave() {
    FlagSpace *current = mpFlagSpace;
    const u16 *saved = getSaveFlagSpace();
    current->copyFromSaveFile(saved, 0, mFlagCount);
}

/** 800bf3e0 */
void ItemStoryManagerBase::init() {
    const u16 *space = getSaveFlagSpace();
    if (space == nullptr || mpFlagIndex == nullptr) {
        initFlagSpace();
        copyFlagsFromSaveFirstTime();
        setupFlagIndex();
    }
}

/** 800bf470 */
void ItemStoryManagerBase::initFlagSpace() {

}

/** 800bf480 */
u16 ItemStoryManagerBase::getFlag(u16 flag) const {
    const u16 *data = getSaveFlagSpace();
    return mpFlagIndex->getCounterOrFlag(flag, data, mFlagCount);
}

/** 800bf4e0 */
u16 ItemStoryManagerBase::getUncommittedValue_Priv(u16 flag) const {
    u16 *data = mpFlagSpace->getFlagPtrUnchecked();
    return mpFlagIndex->getCounterOrFlag(flag, data, mFlagCount);
}

/** 800bf530 */
void ItemStoryManagerBase::setOrClearFlag(u16 flag, u16 value) {
    FlagSpace *storyFlagsPtr = mpFlagSpace;
    u16 *pData = storyFlagsPtr->getFlagPtrChecked();
    mpFlagIndex->setCounterOrFlag(flag, pData, storyFlagsPtr->mCount, value);
    onFlagChange(flag);
}

/** 800bf5b0 */
void ItemStoryManagerBase::setFlag(u16 flag) {
    setOrClearFlag(flag, true);
}

/** 800bf5c0 */
void ItemStoryManagerBase::unsetFlag(u16 flag) {
    setOrClearFlag(flag, false);
}

/** 800bf5d0 */
void ItemStoryManagerBase::setFlagOrCounterToValue(u16 flag, u16 value) {
    setOrClearFlag(flag, value);
}

/** 800bf5e0 */
u16 ItemStoryManagerBase::getCounterOrFlag(u16 flag) const {
    return ItemStoryManagerBase::getFlag(flag);
}

/** 800bf5f0 */
u16 ItemStoryManagerBase::getUncommittedValue(u16 flag) const {
    return getUncommittedValue_Priv(flag);
}

/** 800bf600 */
u16 ItemStoryManagerBase::unk3(u16 flag) {
    return getMaskForFlag(flag);
}

/** 800bf610 */
void ItemStoryManagerBase::onFlagChange(u16 flag) {
    mDirty = true;
    onDirty();
}

/** 800bf630 */
void ItemStoryManagerBase::onDirty() {

}

/** 800bf640 */
u16 ItemStoryManagerBase::getMaskForFlag(u16 flag) {
    return mpFlagIndex->maskForIdx(flag, mFlagCount);
}

/** 800bf650 */
void ItemStoryManagerBase::doCommit_Priv() {
    doCommit();
    mDirty = false;
}

/* 800bf690 */
void ItemStoryManagerBase::postCommit() {

}
