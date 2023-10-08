#include "toBeSorted/file_manager.h"
#include "f/f_base.h"

extern "C" {
    /* 80009D30 */ void fn_80009D30() {} // some ctor
    /* 80009D40 */ void fn_80009D40() {} // some dtor
    /* 80009D80 */ void fn_80009D80() {} // return
    /* 80009D90 */ void fn_80009D90() {} // return
    /* 80009DA0 */ void fn_80009DA0() {} // memset(param_1, 0, 0x20) a 0x24 structure is implied here (0x20 data) a crc is at 0x24
}

/* 80009DB0 */ FileManager::FileManager() {}
/* 80009EE0 */ // mVec3();

/* 80009EF0 */ FileManager FileManager::create(EGG::Heap*){}
/* 80009F30 */ bool FileManager::loadSaveData(void* out, char* name, bool isSkipData){}
/* 80009F70 */ void FileManager::saveSaveData(void* unk, bool isSkipData){}
/* 8000A000 */ void FileManager::refreshSaveFileData(){}
/* 8000A260 */ wchar_t* FileManager::getFileHeroname(int fileNum){}
/* 8000A280 */ s64 FileManager::getFileSaveTime(int fileNum){}
/* 8000A2A0 */ s16 FileManager::getFileCurrentHealth(int fileNum){}
/* 8000A2C0 */ s16 FileManager::getFileHealthCapacity(int fileNum){}
/* 8000A2E0 */ void FileManager::fn_8000A2E0(){}

/* 8000A330 */ 
u16* FileManager::getStoryFlagsMut() {
    return getCurrentFile()->getStoryFlags0();
}
/* 8000A360 */ u16* FileManager::getStoryFlagsConst() {
    return  (isFileInactive() ? mFileB : mFileA).getStoryFlags1();
}
/* 8000A3B0 */ u16* FileManager::getItemFlagsMut() {}
/* 8000A3E0 */ u16* FileManager::getItemFlagsConst() {}
/* 8000A430 */ u16* FileManager::getDungeonFlagsMut() {}
/* 8000A460 */ u16* FileManager::getDungeonFlagsConst() {}
/* 8000A4B0 */ u16* FileManager::getSceneFlagsMut() {}
/* 8000A4E0 */ u16* FileManager::getSceneFlagsConst() {}
/* 8000A530 */ u16* FileManager::getTBoxFlagsMut() {}
/* 8000A560 */ u16* FileManager::getTBoxFlagsConst() {}
/* 8000A5B0 */ u16* FileManager::getTempFlagsMut() {}
/* 8000A5E0 */ u16* FileManager::getTempFlagsConst() {}
/* 8000A630 */ u16* FileManager::getZoneFlagsMut() {}
/* 8000A660 */ u16* FileManager::getZoneFlagsConst() {}
/* 8000A6B0 */ u16* FileManager::getEnemyDefeatFlagsMut() {}
/* 8000A6E0 */ u16* FileManager::getEnemyDefeatFlagsConst() {}
/* 8000A730 */ void FileManager::setStoryFlags(u16* flags, u32 offset, u16 count) {}
/* 8000A790 */ void FileManager::setItemFlags(u16* flags, u32 offset, u16 count) {}
/* 8000A7F0 */ void FileManager::setDungeonFlags(u16* flags, u32 offset, u16 count) {}
/* 8000A850 */ void FileManager::setSceneFlags(u16* flags, u32 offset, u16 count) {}
/* 8000A8B0 */ void FileManager::setTBoxFlags(u16* flags, u32 offset, u16 count) {}
/* 8000A910 */ void FileManager::setTempFlags(u16* flags, u32 offset, u16 count) {}
/* 8000A970 */ void FileManager::setZoneFlags(u16* flags, u32 offset, u16 count) {}
/* 8000A9D0 */ void FileManager::setEnemyDefeatFlags(u16* flags, u32 offset, u16 count) {}

/* 8000AA30 */ u16* FileManager::getSkipFlags() {}
/* 8000AA40 */ void FileManager::setSkipFlagsChecked(u16* flags, u32 offset, u16 count) {}

/* 8000AAA0 */ void FileManager::initFile(int fileNum) {}

/* 8000ABD0 */ void FileManager::setCurrentHealthCapacity(s16 health) {}
/* 8000AC00 */ s16 FileManager::getCurrentHealthCapacity() {}
/* 8000AC50 */ void FileManager::setCurrentHealth(s16 health) {}
/* 8000AC80 */ s16 FileManager::getCurrentHealth() {}

/* 8000ACD0 */ u16  FileManager::getLoadRoomT1() {}
/* 8000AD20 */ u16  FileManager::getLoadRoomT2() {}
/* 8000AD70 */ void FileManager::setLoadRoomT3(s16 room) {}
/* 8000ADA0 */ u16  FileManager::getLoadRoomT3() {}

/* 8000ADF0 */ mVec3_c* FileManager::getPosT1() {}
/* 8000AE40 */ void     FileManager::setPosT2(mVec3_c* pos) {}
/* 8000AE90 */ mVec3_c* FileManager::getPosT2() {}
/* 8000AEE0 */ void     FileManager::setPosT3(mVec3_c* pos) {}
/* 8000AF30 */ mVec3_c* FileManager::getPosT3() {}

/* 8000AF80 */ s16  FileManager::getAngleT1() {}
/* 8000AFD0 */ void FileManager::setAngleT2(s16 angle) {}
/* 8000B000 */ s16  FileManager::getAngleT2() {}
/* 8000B050 */ void FileManager::setAngleT3(s16 angle) {}
/* 8000B080 */ s16  FileManager::getAngleT3() {}

/* 8000B0D0 */ void FileManager::setPouchData(s32 slot, u32 slotData) {}
/* 8000B130 */ u32 FileManager::getPouchData(s32 slot) {}
/* 8000B1B0 */ void FileManager::setPouchItem(s32 slot, ITEM_ID item) {}
/* 8000B220 */ ITEM_ID FileManager::getPouchItem(s32 slot) {}
/* 8000B250 */ void FileManager::setPouchUpperData(s32 slot, s32 data) {}
/* 8000B2C0 */ u16 FileManager::getPouchUpperData(s32 slot) {}

/* 8000B2F0 */ void FileManager::setItemCheckData(u32 idx, u32 itemData){}
/* 8000B360 */ u32 FileManager::getItemCheckData(u32 idx){}
/* 8000B3F0 */ void FileManager::setItemCheckItem(u32 idx, ITEM_ID item){}
/* 8000B480 */ ITEM_ID FileManager::getItemCheckItem(u32 idx){}
/* 8000B4B0 */ void FileManager::setItemCheckUpperData(u32 idx, u32 itemData){}
/* 8000B540 */ u32 FileManager::getItemCheckUpperData(){}

/* 8000B570 */ void FileManager::setEquippedItem(u8 bWheelItem){}
/* 8000B5A0 */ u8 FileManager::getEquippedItem(){}
/* 8000B5F0 */ void FileManager::setSelectedPouchSlot(u8 slot){}
/* 8000B620 */ u8 FileManager::getSelectedPouchSlot(){}
/* 8000B670 */ void FileManager::setShieldPouchSlot(u8 slot){}
/* 8000B6A0 */ u8 FileManager::getShieldPouchSlot(){}

/* 8000B6F0 */ void FileManager::setAirPotionTimer(s16 time){}
/* 8000B720 */ s16 FileManager::getAirPotionTimer(){}
/* 8000B770 */ void FileManager::setAirPotionPlusTimer(s16 time){}
/* 8000B7A0 */ s16 FileManager::getAirPotionPlusTimer(){}
/* 8000B7F0 */ void FileManager::setStaminaPotionTimer(s16 time){}
/* 8000B820 */ s16 FileManager::getStaminaPotionTimer(){}
/* 8000B870 */ void FileManager::setStaminaPotionPlusTimer(s16 time){}
/* 8000B8A0 */ s16 FileManager::getStaminaPotionPlusTimer(){}
/* 8000B8F0 */ void FileManager::setGuardianPotionTimer(s16 time){}
/* 8000B920 */ s16 FileManager::getGuardianPotionTimer(){}
/* 8000B970 */ void FileManager::setGuardianPotionPlusTimer(s16 time){}
/* 8000B9A0 */ s16 FileManager::getGuardianPotionPlusTimer(){}

/* 8000B9F0 */ void FileManager::setDowsingSlotIdx(u8 idx) {}
/* 8000BA20 */ u8 FileManager::getDowsingSlotIdx() {}

/* 8000BA70 */ void FileManager::setBeaconPos(u32 beaconArea, u32 beaconNum, mVec3_c* pos) {}
/* 8000BB80 */ mVec3_c* FileManager::getBeaconPos(u32 beaconArea, u32 beaconNum) {}
/* 8000BC70 */ void FileManager::setEnemyKillCount(u32 enemy, u16 killCount) {}
/* 8000BCE0 */ u16 FileManager::getEnemyKillCount(u32 enemy) {}
/* 8000BD60 */ void FileManager::setHitCountFromEnemy(u32 enemy, u16 hitCount) {}
/* 8000BDD0 */ u16 FileManager::getHitCountFromEnemy(u32 enemy) {}

/* 8000BE50 */ void FileManager::setLoadRoomT1_FileB(s16 roomId) {}
/* 8000BE80 */ s16 FileManager::getLoadRoomT1_FileB() {}
/* 8000BE90 */ void FileManager::setEntranceT1_FileB(u8 entrance) {}
/* 8000BEC0 */ u8  FileManager::getEntranceT1_FileB() {}
/* 8000BED0 */ void FileManager::setAreaT1_FileB(const char* name) {}
/* 8000BF80 */ char* FileManager::getAreaT1_FileB() {}
/* 8000C080 */ void FileManager::setNightT1_FileB(bool night) {}
/* 8000C0B0 */ bool FileManager::getNightT1_FileB() {}
/* 8000C0C0 */ void FileManager::setEntranceT1LoadFlag_FileB(u8 flag) {}
/* 8000C0F0 */ void FileManager::setForcedLayerT1_FileB(u8 layer) {}
/* 8000C120 */ void FileManager::getForcedLayerT1_FileB() {}
/* 8000C130 */ void FileManager::setCurrentHealth_FileB(s16 health) {}

/* 8000C160 */ void FileManager::setHeroname(const wchar_t* name) {}
/* 8000C230 */ wchar_t* FileManager::getHeroname() {}

/* 8000C360 */ char* FileManager::getAreaT1() {}
/* 8000C470 */ char* FileManager::getAreaT2() {}
/* 8000C580 */ void FileManager::setAreaT3(const char* name) {}
/* 8000C630 */ char* FileManager::getAreaT3() {}

/* 8000C740 */ u8 FileManager::getForcedLayerT1() {}
/* 8000C790 */ u8 FileManager::getForcedLayerT2() {}
/* 8000C7E0 */ void FileManager::setForcedLayerT3(u8 layer) {}
/* 8000C810 */ u8 FileManager::getForcedLayerT3() {}

/* 8000C860 */ u8 FileManager::getEntranceT1() {}
/* 8000C8B0 */ void FileManager::setEntranceLoadFlagT1(u8 flag) {}
/* 8000C8E0 */ u8 FileManager::getEntranceLoadFlagT1() {}
/* 8000C930 */ void FileManager::getEntranceT2() {}
/* 8000C980 */ void FileManager::setEntranceT3(u8 entrance) {}
/* 8000C9B0 */ u8 FileManager::getEntranceT3() {}

/* 8000CA00 */ bool FileManager::getNightT1() {}
/* 8000CA50 */ void FileManager::setNightT3(bool night) {}
/* 8000CA80 */ bool FileManager::getNightT3() {}

/* 8000CAD0 */ bool FileManager::isNew_FileA() {}

/* 8000CB00 */ void FileManager::setSceneFlagIndex(u16 idx) {}
/* 8000CB30 */ u16 FileManager::getSceneFlagIndex() {}
/* 8000CB80 */ s32 FileManager::getFileAreaIndex() {}

/* 8000CBD0 */ void FileManager::fn_8000CBD0(u8) {}
/* 8000CC00 */ void FileManager::fn_8000CC00() {}

/* 8000CC50 */ void FileManager::setFileTimes() {}
/* 8000CCB0 */ void FileManager::setPlayTime(s64 time) {}

/* 8000CCF0 */ s64 FileManager::getSavedTime() {}
/* 8000CD40 */ void FileManager::setSavedTime(s64 time) {}

/* 8000CD80 */ void FileManager::setBeedleShopPathSegment(u32 path) {}
/* 8000CDB0 */ u32 FileManager::getBeedleShopPathSegment() {}
/* 8000CE00 */ void FileManager::setBeedleShopPathSegFrac(f32 segFrac) {}
/* 8000CE30 */ f32 FileManager::getBeedleShopPathSegFrac() {}
/* 8000CE80 */ void FileManager::setBeedleShopRotation(s16 rot) {}
/* 8000CEB0 */ s16 FileManager::getBeedleShopRotation() {}

/* 8000CF00 */ void FileManager::fn_8000CF00(u32 shift) {}
/* 8000CF70 */ void FileManager::fn_8000CF70(u32 shift) {}

/* 8000CFE0 */ void FileManager::setSkykeepPuzzle(u32 spot, u8 tile) {}
/* 8000D040 */ u8 FileManager::getSkykeepPuzzleTile(u32 spot) {}

/* 8000D0B0 */ void FileManager::checkFileStatus() {}
/* 8000D1D0 */ void FileManager::checkSkipDataCRCs() {}
/* 8000D270 */ void FileManager::saveOrClearSelectedFileToFileA() {}
/* 8000D280 */ void FileManager::saveOrClearToFileA(int fileNum) {}
/* 8000D9C0 */ void FileManager::copyFileBToCurrentFile() {}
/* 8000E060 */ void FileManager::copyFileAToSelectedFile() {}
/* 8000E070 */ void FileManager::copyFileAToFile(int fileNum) {}
/* 8000E7C0 */ void FileManager::copyFile(int from, int to) {}
/* 8000EF80 */ void FileManager::saveFileAToSelectedFile() {}
/* 8000EF90 */ void FileManager::saveFileAToFile(int fileNum) {}
/* 8000F730 */ void FileManager::copyCurrentToFileB() {}
/* 8000FDF0 */ void FileManager::copySelectedFileSkipData() {}
/* 8000FE00 */ void FileManager::copySkipData(int fileNum) {}
/* 8000FEB0 */ void FileManager::setInfo_FileB() {}
/* 8000FF60 */ void FileManager::clearFileA() {}

/* 80010000 */ void FileManager::initBlankSaveFiles() {}
/* 80010160 */ void FileManager::initSkipData() {}

/* 800101F0 */ void FileManager::unsetFileANewFile() {}
/* 80010220 */ void FileManager::saveT1SaveInfo(u8 entranceT1LoadFlag) {}
/* 80010350 */ void FileManager::copyFileSkipData(int fileNum) {}
/* 80010440 */ void FileManager::clearTempFileData() {}
/* 800104A0 */ void FileManager::saveAfterCredits() {}

/* 80011210 */ SaveFile* FileManager::getCurrentFile() {}
/* 80011250 */ u16* FileManager::getSkipFlags2() {}
/* 80011260 */ SaveFile* FileManager::getFileA() {}
/* 80011270 */ SaveFile* FileManager::getFileB() {}
/* 80011280 */ void FileManager::calcFileCRC(const SaveFile* file, u32 length) {}
/* 80011290 */ void FileManager::updateEmptyFiles() {}
/* 800112D0 */ void FileManager::updateEmptyFileFlags() {}
/* 80011370 */ bool FileManager::isFileEmpty(int fileNum) {}
/* 80011390 */ bool FileManager::isFileUnk3(int fileNum) {}
/* 800113B0 */ u8 FileManager::get_0xA84C() {}
/* 800113C0 */ bool FileManager::checkRegionCode() {}
/* 80011440 */ bool FileManager::checkFileCRC(int fileNum) {}
/* 80011490 */
bool FileManager::isFileInactive() {
    fBase_c* actor = fManager_c::searchBaseByGroupType(1, nullptr);
    if (actor) {
        if (actor->profile_name == fProfile::TITLE && !mAntiCommitFlag)
            return true;
        return false;
    }
    return false;
}
/* 80011500 */ void FileManager::setPlayerInfoFileA() {}
/* 800115E0 */ void FileManager::setT3Info(mVec3_c* pos, mAng3_c* rot) {}
/* 800116C0 */ void FileManager::getRegionVersion(char* out) {}

