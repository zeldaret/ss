#include "toBeSorted/file_manager.h"

#include "f/f_base.h"
#include "m/m_heap.h"
#include "s/s_Crc.h"

// clang-format off
#include "sized_string.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/nand_request_thread.h"
#include "toBeSorted/save_manager.h"
// clang-format on

// This class here makes no sense and the name might
// be a total misnomer, but this gets the sinit section correct
class UnkClass {
public:
    UnkClass();

    virtual ~UnkClass();

    static UnkClass sInstance;
};
// This seems really pointless since the class only has a virtual destructor
// and no members but /shrug
UnkClass UnkClass::sInstance;

UnkClass::UnkClass() {}
UnkClass::~UnkClass() {}

FileManager *FileManager::sInstance;

extern "C" {
void fn_80009D80() {}
void fn_80009D90() {}
void fn_80009DA0(void *ptr) {
    memset(ptr, 0, 0x20);
}
}

FileManager::FileManager() {
    // TODO the assembly code looks really wild
    mHeroNames[0][0] = '\0';
    u32 num_files = (u32)(mHeroName - mHeroNames[0]);
    num_files = num_files / sizeof(mHeroName);
    for (int i = 1; mHeroNames[i] < mHeroName && i < num_files; i++) {
        mHeroNames[i][0] = '\0';
    }
    mHeroName[0] = '\0';
    mCurrentArea[0] = '\0';
    sInstance = this;
    // TODO these should probably use the new operators?
    mpSavedSaveFiles = (SavedSaveFiles *)mHeap::g_gameHeaps[0]->alloc(sizeof(SavedSaveFiles), 0x20);
    mpSkipData = (SkipData *)mHeap::g_gameHeaps[0]->alloc(0x80, 0x20);

    fn_8000A2E0();
}

FileManager *FileManager::create(EGG::Heap *heap) {
    return new (heap, 0x04) FileManager();
}
bool FileManager::loadSaveData(NandRequestWriteHolder *out, const char *name, bool isSkipData) {}
void FileManager::saveSaveData(NandRequestLoadSaveFileHolder *unk, bool isSkipData) {}
void FileManager::refreshSaveFileData() {}
wchar_t *FileManager::getFileHeroname(int fileNum) {}
s64 FileManager::getFileSaveTime(int fileNum) {}
s16 FileManager::getFileCurrentHealth(int fileNum) {}
s16 FileManager::getFileHealthCapacity(int fileNum) {}
void FileManager::fn_8000A2E0() {
    // maybe call this function "reset"
    mIsFileUnk1[0] = true;
    initBlankSaveFiles();
    m_0xA84D = 0;
    mSelectedFile = 1;
}

u16 *FileManager::getStoryFlagsMut() {
    return getCurrentFile()->getStoryFlags0();
}
const u16 *FileManager::getStoryFlagsConst() const {
    return getCurrentFile()->getStoryFlags1();
}
u16 *FileManager::getItemFlagsMut() {}
const u16 *FileManager::getItemFlagsConst() {}
u16 *FileManager::getDungeonFlagsMut() {}
u16 *FileManager::getDungeonFlagsConst() {}
u16 *FileManager::getSceneFlagsMut() {}
u16 *FileManager::getSceneFlagsConst() {}
u16 *FileManager::getTBoxFlagsMut() {}
u16 *FileManager::getTBoxFlagsConst() {}
u16 *FileManager::getTempFlagsMut() {}
u16 *FileManager::getTempFlagsConst() {}
u16 *FileManager::getZoneFlagsMut() {}
u16 *FileManager::getZoneFlagsConst() {}
u16 *FileManager::getEnemyDefeatFlagsMut() {}
u16 *FileManager::getEnemyDefeatFlagsConst() {}
void FileManager::setStoryFlags(u16 *flags, u32 offset, u16 count) {}
void FileManager::setItemFlags(u16 *flags, u32 offset, u16 count) {}
void FileManager::setDungeonFlags(u16 *flags, u32 offset, u16 count) {}
void FileManager::setSceneFlags(u16 *flags, u32 offset, u16 count) {}
void FileManager::setTBoxFlags(u16 *flags, u32 offset, u16 count) {}
void FileManager::setTempFlags(u16 *flags, u32 offset, u16 count) {}
void FileManager::setZoneFlags(u16 *flags, u32 offset, u16 count) {}
void FileManager::setEnemyDefeatFlags(u16 *flags, u32 offset, u16 count) {}

u16 *FileManager::getSkipFlags() {}
void FileManager::setSkipFlagsChecked(u16 *flags, u32 offset, u16 count) {}

void FileManager::initFile(int fileNum) {
    mIsFileInvalid[1] = 1;
    SaveFile *file = getFileA();
    if (fileNum != 0) {
        file = &mFileB;
    }

    file->new_file = 0;
    file->health_capacity = 24; // 6 hearts * 4 quarters
    file->unused_heart_related = 24;
    file->current_health = 24;
    file->shield_pouch_slot = 8;
    file->equipped_b_item = 0xb; // TODO (B Wheel Id)
    file->selectedDowsingSlot = DowsingTarget::SLOT_NONE;
    file->lastUsedPouchItemSlot = 8;

    SizedString<32> buf;
    buf = "F405";
    file->setAreaT1(buf);
    file->room_id_t1 = 0;
    file->forced_layer_t1 = 0;
    file->entrance_t1_load_flag = 1;
}

void FileManager::setCurrentHealthCapacity(s16 health) {}
u16 FileManager::getCurrentHealthCapacity() {}
void FileManager::setCurrentHealth(s16 health) {}
u16 FileManager::getCurrentHealth() {}

u16 FileManager::getLoadRoomT1() {}
u16 FileManager::getLoadRoomT2() {}
void FileManager::setLoadRoomT3(s16 room) {}
u16 FileManager::getLoadRoomT3() {}

mVec3_c *FileManager::getPosT1() {}
void FileManager::setPosT2(mVec3_c *pos) {}
mVec3_c *FileManager::getPosT2() {}
void FileManager::setPosT3(mVec3_c *pos) {}
mVec3_c *FileManager::getPosT3() {}

s16 FileManager::getAngleT1() {}
void FileManager::setAngleT2(s16 angle) {}
s16 FileManager::getAngleT2() {}
void FileManager::setAngleT3(s16 angle) {}
s16 FileManager::getAngleT3() {}

void FileManager::setPouchData(s32 slot, u32 slotData) {}
u32 FileManager::getPouchData(s32 slot) {}
void FileManager::setPouchItem(s32 slot, ITEM_ID item) {}
ITEM_ID FileManager::getPouchItem(s32 slot) {}
void FileManager::setPouchUpperData(s32 slot, s32 data) {}
u16 FileManager::getPouchUpperData(s32 slot) {}

void FileManager::setItemCheckData(u32 idx, u32 itemData) {}
u32 FileManager::getItemCheckData(u32 idx) {}
void FileManager::setItemCheckItem(u32 idx, ITEM_ID item) {}
ITEM_ID FileManager::getItemCheckItem(u32 idx) {}
void FileManager::setItemCheckUpperData(u32 idx, u32 itemData) {}
u32 FileManager::getItemCheckUpperData() {}

void FileManager::setEquippedItem(u8 bWheelItem) {}
u8 FileManager::getEquippedItem() {}
void FileManager::setSelectedPouchSlot(u8 slot) {}
u8 FileManager::getSelectedPouchSlot() {}
void FileManager::setShieldPouchSlot(u8 slot) {}
u8 FileManager::getShieldPouchSlot() {}

void FileManager::setAirPotionTimer(u16 time) {}
u16 FileManager::getAirPotionTimer() const {}
void FileManager::setAirPotionPlusTimer(u16 time) {}
u16 FileManager::getAirPotionPlusTimer() const {}
void FileManager::setStaminaPotionTimer(u16 time) {}
u16 FileManager::getStaminaPotionTimer() const {}
void FileManager::setStaminaPotionPlusTimer(u16 time) {}
u16 FileManager::getStaminaPotionPlusTimer() const {}
void FileManager::setGuardianPotionTimer(u16 time) {}
u16 FileManager::getGuardianPotionTimer() const {}
void FileManager::setGuardianPotionPlusTimer(u16 time) {}
u16 FileManager::getGuardianPotionPlusTimer() const {}

void FileManager::setDowsingSlotIdx(u8 idx) {}
u8 FileManager::getDowsingSlotIdx() {}

void FileManager::setBeaconPos(u32 beaconArea, u32 beaconNum, mVec3_c *pos) {}
mVec3_c *FileManager::getBeaconPos(u32 beaconArea, u32 beaconNum) {}
void FileManager::setEnemyKillCount(u8 enemy, u16 killCount) {}
u16 FileManager::getEnemyKillCount(u8 enemy) {}
void FileManager::setHitCountFromEnemy(u8 enemy, u16 hitCount) {}
u16 FileManager::getHitCountFromEnemy(u8 enemy) {}

void FileManager::setLoadRoomT1_FileB(s16 roomId) {}
u16 FileManager::getLoadRoomT1_FileB() {}
void FileManager::setEntranceT1_FileB(u8 entrance) {}
u8 FileManager::getEntranceT1_FileB() {}
void FileManager::setAreaT1_FileB(const char *name) {}
char *FileManager::getAreaT1_FileB() {}
void FileManager::setNightT1_FileB(bool night) {}
u8 FileManager::getNightT1_FileB() {}
void FileManager::setEntranceT1LoadFlag_FileB(u8 flag) {}
void FileManager::setForcedLayerT1_FileB(u8 layer) {}
u8 FileManager::getForcedLayerT1_FileB() {}
void FileManager::setCurrentHealth_FileB(s16 health) {}

void FileManager::setHeroname(const wchar_t *name) {}
wchar_t *FileManager::getHeroname() {}

char *FileManager::getAreaT1() {}
char *FileManager::getAreaT2() {}
void FileManager::setAreaT3(const char *name) {}
char *FileManager::getAreaT3() {}

u8 FileManager::getForcedLayerT1() {}
u8 FileManager::getForcedLayerT2() {}
void FileManager::setForcedLayerT3(u8 layer) {}
u8 FileManager::getForcedLayerT3() {}

u8 FileManager::getEntranceT1() {}
void FileManager::setEntranceLoadFlagT1(u8 flag) {}
u8 FileManager::getEntranceLoadFlagT1() {}
void FileManager::getEntranceT2() {}
void FileManager::setEntranceT3(u8 entrance) {}
u8 FileManager::getEntranceT3() {}

bool FileManager::getNightT1() {}
void FileManager::setNightT3(bool night) {}
u8 FileManager::getNightT3() {}

bool FileManager::isNew_FileA() {}

void FileManager::setSceneFlagIndex(s16 idx) {}
u32 FileManager::getSceneFlagIndex() {}
s32 FileManager::getFileAreaIndex() {}

void FileManager::fn_8000CBD0(u8) {}
u8 FileManager::fn_8000CC00() {}

void FileManager::setFileTimes() {}
void FileManager::setPlayTime(s64 time) {}

s64 FileManager::getSavedTime() {}
void FileManager::setSavedTime(s64 time) {}

void FileManager::setBeedleShopPathSegment(u32 path) {}
u32 FileManager::getBeedleShopPathSegment() {}
void FileManager::setBeedleShopPathSegFrac(f32 segFrac) {}
f32 FileManager::getBeedleShopPathSegFrac() {}
void FileManager::setBeedleShopRotation(s16 rot) {}
s16 FileManager::getBeedleShopRotation() {}

void FileManager::fn_8000CF00(u32 shift) {}
void FileManager::fn_8000CF70(u32 shift) {}

void FileManager::setSkykeepPuzzle(u32 spot, u8 tile) {}
u8 FileManager::getSkykeepPuzzleTile(u32 spot) {}

void FileManager::checkFileStatus() {
    mIsFileInvalid[2] = 0;
    SkipData *data;
    SavedSaveFiles *files = mpSavedSaveFiles;

    if (!checkRegionCode()) {
        mIsFileInvalid[2] = 1;
    }
    if (files->m_0x1C != 0x1d) {
        mIsFileInvalid[2] = 1;
    }

    for (u8 i = 0; i < 3; i++) {
        if (checkFileCRC(i) == 0) {
            mIsFileDataDirty[i] = 1;
        } else {
            mIsFileDataDirty[i] = 0;
        }
    }

    int i;
    for (i = 0, data = files->skipData; i < 3; i++, data++) {
        u32 crc = calcFileCRC(data->data, sizeof(data->data));
        if (crc != data->crc) {
            fn_80009DA0(data);
            data->crc = calcFileCRC(data->data, sizeof(data->data));
        }
    }
}
bool FileManager::checkSkipDataCRCs() {
    SkipData *data;
    bool dirty = false;
    u8 i;
    for (data = mpSkipData, i = 0; (s32)i < 3; i++, data++) {
        u32 crc = calcFileCRC(data->data, sizeof(data->data));
        if (crc == data->crc) {
            mIsFileSkipDataDirty[i] = 0;
        } else {
            mIsFileSkipDataDirty[i] = 1;
            dirty = true;
        }
    }
    return dirty;
}
void FileManager::saveOrClearSelectedFileToFileA() {
    saveOrClearToFileA(mSelectedFile);
}
void FileManager::saveOrClearToFileA(int fileNum) {}
void FileManager::copyFileBToCurrentFile() {}
void FileManager::copyFileAToSelectedFile() {
    copyFileAToFile(mSelectedFile);
}
void FileManager::copyFileAToFile(int fileNum) {}
void FileManager::copyFile(int from, int to) {}
void FileManager::saveFileAToSelectedFile() {
    saveFileAToFile(mSelectedFile);
}
void FileManager::saveFileAToFile(int fileNum) {}
void FileManager::copyCurrentToFileB() {}
void FileManager::copySelectedFileSkipData() {
    copySkipData(mSelectedFile);
}
void FileManager::copySkipData(u8 fileNum) {
    if (fileNum < 3) {
        SkipData *curr = &mSkipData;
        SkipData *data = mpSkipData;
        curr->crc = calcFileCRC(&curr->data, sizeof(mSkipData.data));
        data[fileNum] = *curr;
    }
}
void FileManager::setInfo_FileB() {}
void FileManager::clearFileA() {
    SkipData *data;
    SaveFile *file = getFileA();
    memset(file, 0, sizeof(SaveFile));
    file->new_file = 1;
    file->checksum = calcFileCRC(file, sizeof(SaveFile) - sizeof(u32));
    data = &mSkipData;
    memset(&data->data, 0, sizeof(SkipData));
    data->crc = calcFileCRC(data->data, sizeof(mSkipData.data));
}

void FileManager::initBlankSaveFiles() {
    memset(mpSavedSaveFiles, 0, sizeof(SavedSaveFiles));
    mSelectedFile = 0;
    memset(mIsFileEmpty, 0, 3);
    SkipData *data;

    SaveFile *file;
    SavedSaveFiles *saved = mpSavedSaveFiles;

    mHeroNames[0][0] = '\0';
    mPlayTime[0] = 0;
    mCurrentHealth[0] = 0;
    mCurrentHealthCapacity[0] = 0;

    mHeroNames[1][0] = '\0';
    mPlayTime[1] = 0;
    mCurrentHealth[1] = 0;
    mCurrentHealthCapacity[1] = 0;

    mHeroNames[2][0] = '\0';
    mPlayTime[2] = 0;
    mCurrentHealth[2] = 0;
    mCurrentHealthCapacity[2] = 0;

    getRegionVersion(saved->regionCode);
    saved->m_0x1C = 0x1d;

    file = saved->saveFiles;
    for (int num = 0; num < 3; num++, file++) {
        file->new_file = 1;
        u32 crc = calcFileCRC(file, sizeof(SaveFile) - sizeof(u32));
        file->checksum = crc;
    }

    int i;
    for (i = 0, data = saved->skipData; i < 3; i++, data++) {
        u32 crc = calcFileCRC(data->data, sizeof(data->data));
        data->crc = crc;
    }

    clearFileA();
    mIsFileUnk1[1] = 0;
    mIsFileUnk1[2] = 0;
    mIsFileInvalid[0] = 0;
    mIsFileInvalid[1] = 0;
    m_0xA84C = 0;
    mIsFileInvalid[2] = 0;
    mIsFileDataDirty[0] = 0;
    mIsFileDataDirty[1] = 0;
    mIsFileDataDirty[2] = 0;
    initSkipData();
}
void FileManager::initSkipData() {
    memset(mpSkipData, 0, 0x80);
    SkipData *data;
    int i;
    for (i = 0, data = mpSkipData; i < 3; i++, data++) {
        u32 crc = calcFileCRC(data->data, sizeof(data->data));
        data->crc = crc;
    }
    mIsFileSkipDataDirty[0] = 0;
    mIsFileSkipDataDirty[1] = 0;
    mIsFileSkipDataDirty[2] = 0;
}

void FileManager::unsetFileANewFile() {}
void FileManager::saveT1SaveInfo(bool entranceT1LoadFlag) {}
void FileManager::copyFileSkipData(int fileNum) {}
extern "C" void fn_800C01F0(); // todo flag managers
void FileManager::clearTempFileData() {
    memset(&mFileA, 0, sizeof(SaveFile));
    memset(&mFileB, 0, sizeof(SaveFile));
    memset(&mSkipData, 0, sizeof(SkipData));
    fn_800C01F0();
}
void FileManager::saveAfterCredits() {}

SaveFile *FileManager::getCurrentFile() {
    return isFileInactive() ? &mFileB : &mFileA;
}
u16 *FileManager::getSkipFlags2() {}
SaveFile *FileManager::getFileA() {
    return &mFileA;
}
SaveFile *FileManager::getFileB() {
    return &mFileB;
}
u32 FileManager::calcFileCRC(const void *data, u32 length) {
    return sCrc::calcCRC32(data, length);
}
void FileManager::updateEmptyFiles() {
    updateEmptyFileFlags();
    refreshSaveFileData();
}
void FileManager::updateEmptyFileFlags() {
    SaveFile *saves = mpSavedSaveFiles->saveFiles;
    for (int i = 0; i < 3; i++) {
        if (saves[i].new_file == 1) {
            mIsFileEmpty[i] = 1;
        } else {
            mIsFileEmpty[i] = 0;
        }
    }
}
bool FileManager::isFileEmpty(u8 fileNum) {}
bool FileManager::isFileDirty(int fileNum) {}
u32 FileManager::get_0xA84C() {}
bool FileManager::checkRegionCode() {}
bool FileManager::checkFileCRC(u8 fileNum) {}

bool FileManager::isFileInactive() const {
    fBase_c *actor = fManager_c::searchBaseByGroupType(1, nullptr);
    if (actor) {
        if (actor->mProfileName == fProfile::TITLE && !mAntiCommitFlag) {
            return true;
        }
        return false;
    }
    return false;
}
void FileManager::setPlayerInfoFileA() {}
void FileManager::setT3Info(const mVec3_c &pos, const mAng3_c &rot) {}
void FileManager::getRegionVersion(char *out) {}
