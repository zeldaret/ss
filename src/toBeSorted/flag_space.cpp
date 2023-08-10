#include <libc.h>
#include <types.h>
#include <toBeSorted/file_manager.h>
#include <toBeSorted/flag_space.h>

u16* FlagSpace::getFlagPtrChecked() {
    filemanagerCheck();
    return mpFlags;
}
u16* FlagSpace::getFlagPtrUnchecked() {
    return mpFlags;
}
void FlagSpace::unsetAll() {
    setAllToZero(0, mCount);
}
void FlagSpace::setAllToZero(u16 offset, u16 flagCount) {
    filemanagerCheck();
    memset(mpFlags + offset, 0, (u16)(flagCount * 2));
}
void FlagSpace::copyFromSaveFile2(u16 *pSaved, u16 offset, u16 flagCount) {
    filemanagerCheck();
    checkedMemcpy(mpFlags + offset, mCount * 2, pSaved, flagCount * 2);
}

void FlagSpace::copyFromSaveFile(u16 *pSaved, u16 offset, u16 flagCount) {
    checkedMemcpy(mpFlags + offset, mCount * 2, pSaved, flagCount * 2);

}
void FlagSpace::filemanagerCheck() {
    if (FileManager::sInstance->isFileUnk1[0]) {
        return;
    }
    while (true) {}
}