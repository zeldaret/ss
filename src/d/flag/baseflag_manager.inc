#include "d/flag/baseflag_manager.h"

ItemStoryManagerBase::ItemStoryManagerBase()
    : mFlagCount(0), mFlagSizeBytes(0), mpFlagSpace(nullptr), mpFlagIndex(nullptr), mDirty(false) {}
ItemStoryManagerBase::~ItemStoryManagerBase() {
    if (mpFlagIndex != nullptr) {
        delete mpFlagIndex;
    }
}

void ItemStoryManagerBase::setFlagSizes(u16 flagCount, u16 flagSizeBytes) {
    mFlagCount = flagCount;
    mFlagSizeBytes = flagSizeBytes;
}

void ItemStoryManagerBase::createFlagIndex(FlagDefinition *def, u16 count) {
    mpFlagIndex = new FlagIndex(count, def);
}

void ItemStoryManagerBase::copyFromSave() {
    FlagSpace *current = mpFlagSpace;
    const u16 *saved = getSaveFlagSpace();
    current->copyFromSaveFile(saved, 0, mFlagCount);
}

void ItemStoryManagerBase::init() {
    const u16 *space = getSaveFlagSpace();
    if (space == nullptr || mpFlagIndex == nullptr) {
        initFlagSpace();
        copyFlagsFromSaveFirstTime();
        setupFlagIndex();
    }
}

void ItemStoryManagerBase::initFlagSpace() {}

u16 ItemStoryManagerBase::getFlag(u16 flag) const {
    const u16 *data = getSaveFlagSpace();
    return mpFlagIndex->getCounterOrFlag(flag, data, mFlagCount);
}

u16 ItemStoryManagerBase::getUncommittedValue_Priv(u16 flag) const {
    u16 *data = mpFlagSpace->getFlagPtrUnchecked();
    return mpFlagIndex->getCounterOrFlag(flag, data, mFlagCount);
}

void ItemStoryManagerBase::setOrClearFlag(u16 flag, u16 value) {
    FlagSpace *storyFlagsPtr = mpFlagSpace;
    u16 *pData = storyFlagsPtr->getFlagPtrChecked();
    mpFlagIndex->setCounterOrFlag(flag, pData, storyFlagsPtr->mCount, value);
    onFlagChange(flag);
}

void ItemStoryManagerBase::setFlag(u16 flag) {
    setOrClearFlag(flag, true);
}

void ItemStoryManagerBase::unsetFlag(u16 flag) {
    setOrClearFlag(flag, false);
}

void ItemStoryManagerBase::setFlagOrCounterToValue(u16 flag, u16 value) {
    setOrClearFlag(flag, value);
}

u16 ItemStoryManagerBase::getCounterOrFlag(u16 flag) const {
    return ItemStoryManagerBase::getFlag(flag);
}

u16 ItemStoryManagerBase::getUncommittedValue(u16 flag) const {
    return getUncommittedValue_Priv(flag);
}

u16 ItemStoryManagerBase::unk3(u16 flag) {
    return getMaskForFlag(flag);
}

void ItemStoryManagerBase::onFlagChange(u16 flag) {
    mDirty = true;
    onDirty();
}

void ItemStoryManagerBase::onDirty() {}

u16 ItemStoryManagerBase::getMaskForFlag(u16 flag) {
    return mpFlagIndex->maskForIdx(flag, mFlagCount);
}

void ItemStoryManagerBase::doCommit_Priv() {
    doCommit();
    mDirty = false;
}

void ItemStoryManagerBase::postCommit() {}
