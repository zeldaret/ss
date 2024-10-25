#include "common.h"
#include "d/flag/flag_managers.h"
#include "d/flag/flag_space.h"
#include "toBeSorted/file_manager.h"

u16 *FlagSpace::getFlagPtrChecked() const {
    filemanagerCheck();
    return mpFlags;
}
u16 *FlagSpace::getFlagPtrUnchecked() const {
    return mpFlags;
}
void FlagSpace::unsetAll() {
    setAllToZero(0, mCount);
}
void FlagSpace::setAllToZero(u16 offset, u16 flagCount) {
    filemanagerCheck();
    memset(mpFlags + offset, 0, (u16)(flagCount * 2));
}
void FlagSpace::copyFromSaveFile2(const u16 *pSaved, u16 offset, u16 flagCount) {
    filemanagerCheck();
    checkedMemcpy(mpFlags + offset, (u16)(mCount * 2), pSaved, (u16)(flagCount * 2));
}

void FlagSpace::copyFromSaveFile(const u16 *pSaved, u16 offset, u16 flagCount) {
    checkedMemcpy(mpFlags + offset, (u16)(mCount * 2), pSaved, (u16)(flagCount * 2));
}
void FlagSpace::filemanagerCheck() const {
    if (FileManager::sInstance->mIsFileUnk1[0]) {
        return;
    }
    while (true) {}
}
