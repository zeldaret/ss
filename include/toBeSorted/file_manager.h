#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "common.h"
#include "d/a/d_a_itembase.h"
#include "egg/core/eggHeap.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "toBeSorted/nand_request_thread.h"
#include "toBeSorted/save_file.h"
#include "toBeSorted/save_manager.h"

enum SAVE_ITEM_ID {
};

class SkipData {
public:
    /** 0x00 */ u16 data[16];
    /** 0x20 */ u32 crc;
};

class SavedSaveFiles {
public:
    /* 0x0000 */ char regionCode[4];
    /* 0x0004 */ u8 unk1[0x1C - 0x04];
    /* 0x001C */ u32 m_0x1C;
    /* 0x0020 */ SaveFile saveFiles[3];
    /* 0xfb60 */ SkipData skipData[3];
    /* 0xfbcc */ u8 unk2[0xfbe0 - 0xfbcc];
};

class FileManager {
public:
    /* 0x0000 */ SavedSaveFiles *mpSavedSaveFiles;
    /* 0x0004 */ SkipData *mpSkipData; // skip data Arrary (3 entries )
    /* 0x0008 */ SaveFile mFileA;
    /* 0x53C8 */ SaveFile mFileB;
    /* 0xA788 */ SkipData mSkipData;
    /* 0xA7AC */ wchar_t mHeroNames[3][9]; // each name is 9 wchars
    /* 0xA7E2 */ wchar_t mHeroName[9];     // The current Hero Name
    /* 0xA7F4 */ char mCurrentArea[32];
    /* 0xA814 */ u32 m_0xA814;
    /* 0xA818 */ s64 mPlayTime[3];
    /* 0xA830 */ s16 mCurrentHealth[3];
    /* 0xA836 */ s16 mCurrentHealthCapacity[3];
    /* 0xA83C */ u8 mSelectedFile;
    /* 0xA83D */ u8 mIsFileEmpty[3];
    /* 0xA840 */ u8 mIsFileUnk1[3];
    /* 0xA843 */ u8 mIsFileInvalid[3];
    /* 0xA846 */ u8 mIsFileDataDirty[3];
    /* 0xA849 */ u8 mIsFileSkipDataDirty[3];
    /* 0xA84C */ u8 m_0xA84C;
    /* 0xA84D */ u8 m_0xA84D;
    /* 0xA84E */ u8 mAntiCommitFlag;
    /* 0xA84F */ u8 m_0xA84F;

public:
    // the following arent part of FileManager i dont think
    // void fn_80009D30(); // some ctor
    // void fn_80009D40(); // some dtor
    // void fn_80009D80(); // return
    // void fn_80009D90(); // return
    // void fn_80009DA0(); // memset(param_1, 0, 0x20) a 0x24 structure is implied here (0x20 data) a crc
    // is at 0x24

    FileManager();

    static FileManager *create(EGG::Heap *);
    bool loadSaveData(NandRequestWriteHolder *out, const char *name, bool isSkipData);
    void saveSaveData(NandRequestLoadSaveFileHolder *request, bool isSkipData);
    void refreshSaveFileData();
    wchar_t *getFileHeroname(int fileNum);
    s64 getFileSaveTime(int fileNum);
    s16 getFileCurrentHealth(int fileNum);
    s16 getFileHealthCapacity(int fileNum);
    void fn_8000A2E0(); // idk something blank save files

    u16 *getStoryFlagsMut();
    const u16 *getStoryFlagsConst() const;
    u16 *getItemFlagsMut();
    const u16 *getItemFlagsConst();
    u16 *getDungeonFlagsMut();
    u16 *getDungeonFlagsConst();
    u16 *getSceneFlagsMut();
    u16 *getSceneFlagsConst();
    u16 *getTBoxFlagsMut();
    u16 *getTBoxFlagsConst();
    u16 *getTempFlagsMut();
    u16 *getTempFlagsConst();
    u16 *getZoneFlagsMut();
    u16 *getZoneFlagsConst();
    u16 *getEnemyDefeatFlagsMut();
    u16 *getEnemyDefeatFlagsConst();

    void setStoryFlags(u16 *flags, u32 offset, u16 count);
    void setItemFlags(u16 *flags, u32 offset, u16 count);
    void setDungeonFlags(u16 *flags, u32 offset, u16 count);
    void setSceneFlags(u16 *flags, u32 offset, u16 count);
    void setTBoxFlags(u16 *flags, u32 offset, u16 count);
    void setTempFlags(u16 *flags, u32 offset, u16 count);
    void setZoneFlags(u16 *flags, u32 offset, u16 count);
    void setEnemyDefeatFlags(u16 *flags, u32 offset, u16 count);

    u16 *getSkipFlags();
    void setSkipFlagsChecked(u16 *flags, u32 offset, u16 count);

    void initFile(int fileNum);

    void setCurrentHealthCapacity(s16 health);
    u16 getCurrentHealthCapacity();
    void setCurrentHealth(s16 health);
    u16 getCurrentHealth();

    u16 getLoadRoomT1();
    u16 getLoadRoomT2();
    void setLoadRoomT3(s16 room);
    u16 getLoadRoomT3();

    mVec3_c *getPosT1();
    void setPosT2(mVec3_c *pos);
    mVec3_c *getPosT2();
    void setPosT3(mVec3_c *pos);
    mVec3_c *getPosT3();

    s16 getAngleT1();
    void setAngleT2(s16 angle);
    s16 getAngleT2();
    void setAngleT3(s16 angle);
    s16 getAngleT3();

    void setPouchData(s32 slot, u32 slotData);
    u32 getPouchData(s32 slot);
    void setPouchItem(s32 slot, ITEM_ID item);
    ITEM_ID getPouchItem(s32 slot);
    void setPouchUpperData(s32 slot, s32 data);
    u16 getPouchUpperData(s32 slot);

    void setItemCheckData(u32 idx, u32 itemData);
    u32 getItemCheckData(u32 idx);
    void setItemCheckItem(u32 idx, ITEM_ID item);
    ITEM_ID getItemCheckItem(u32 idx);
    void setItemCheckUpperData(u32 idx, u32 itemData);
    u32 getItemCheckUpperData();

    void setEquippedItem(u8 bWheelItem);
    u8 getEquippedItem();
    void setSelectedPouchSlot(u8 slot);
    u8 getSelectedPouchSlot();
    void setShieldPouchSlot(u8 slot);
    u8 getShieldPouchSlot();

    void setAirPotionTimer(u16 time);
    u16 getAirPotionTimer() const;
    void setAirPotionPlusTimer(u16 time);
    u16 getAirPotionPlusTimer() const;
    void setStaminaPotionTimer(u16 time);
    u16 getStaminaPotionTimer() const;
    void setStaminaPotionPlusTimer(u16 time);
    u16 getStaminaPotionPlusTimer() const;
    void setGuardianPotionTimer(u16 time);
    u16 getGuardianPotionTimer() const;
    void setGuardianPotionPlusTimer(u16 time);
    u16 getGuardianPotionPlusTimer() const;

    void setDowsingSlotIdx(u8 idx);
    u8 getDowsingSlotIdx();

    void setBeaconPos(u32 beaconArea, u32 beaconNum, mVec3_c *pos);
    mVec3_c *getBeaconPos(u32 beaconArea, u32 beaconNum);
    void setEnemyKillCount(u8 enemy, u16 killCount);
    u16 getEnemyKillCount(u8 enemy);
    void setHitCountFromEnemy(u8 enemy, u16 hitCount);
    u16 getHitCountFromEnemy(u8 enemy);

    void setLoadRoomT1_FileB(s16 roomId);
    u16 getLoadRoomT1_FileB();
    void setEntranceT1_FileB(u8 entrance);
    u8 getEntranceT1_FileB();
    void setAreaT1_FileB(const char *name);
    char *getAreaT1_FileB();
    void setNightT1_FileB(bool night);
    u8 getNightT1_FileB();
    void setEntranceT1LoadFlag_FileB(u8 flag);
    void setForcedLayerT1_FileB(u8 layer);
    u8 getForcedLayerT1_FileB();
    void setCurrentHealth_FileB(s16 health);

    void setHeroname(const wchar_t *name);
    wchar_t *getHeroname();

    char *getAreaT1();
    char *getAreaT2();
    void setAreaT3(const char *name);
    char *getAreaT3();

    u8 getForcedLayerT1();
    u8 getForcedLayerT2();
    void setForcedLayerT3(u8 layer);
    u8 getForcedLayerT3();

    u8 getEntranceT1();
    void setEntranceLoadFlagT1(u8 flag);
    u8 getEntranceLoadFlagT1();
    void getEntranceT2();
    void setEntranceT3(u8 entrance);
    u8 getEntranceT3();

    bool getNightT1();
    void setNightT3(bool night);
    u8 getNightT3();

    bool isNew_FileA();

    void setSceneFlagIndex(s16 idx);
    u32 getSceneFlagIndex();
    s32 getFileAreaIndex();

    void fn_8000CBD0(u8);
    u8 fn_8000CC00();

    void setFileTimes();
    void setPlayTime(s64 time);

    s64 getSavedTime();
    void setSavedTime(s64 time);

    void setBeedleShopPathSegment(u32 path);
    u32 getBeedleShopPathSegment();
    void setBeedleShopPathSegFrac(f32 segFrac);
    f32 getBeedleShopPathSegFrac();
    void setBeedleShopRotation(s16 rot);
    s16 getBeedleShopRotation();

    void fn_8000CF00(u32 shift);
    void fn_8000CF70(u32 shift);

    void setSkykeepPuzzle(u32 spot, u8 tile);
    u8 getSkykeepPuzzleTile(u32 spot);

    void checkFileStatus();
    bool checkSkipDataCRCs();
    void saveOrClearSelectedFileToFileA();
    void saveOrClearToFileA(int fileNum);
    void copyFileBToCurrentFile();
    void copyFileAToSelectedFile();
    void copyFileAToFile(int fileNum);
    void copyFile(int from, int to);
    void saveFileAToSelectedFile();
    void saveFileAToFile(int fileNum);
    void copyCurrentToFileB();
    void copySelectedFileSkipData();
    void copySkipData(u8 fileNum);
    void setInfo_FileB();
    void clearFileA();

    void initBlankSaveFiles();
    void initSkipData();

    void unsetFileANewFile();
    void saveT1SaveInfo(bool entranceT1LoadFlag);
    void copyFileSkipData(int fileNum);
    void clearTempFileData();
    void saveAfterCredits();
    SaveFile *getCurrentFile();
    inline const SaveFile *getCurrentFile() const {
        return isFileInactive() ? &mFileB : &mFileA;
    }
    u16 *getSkipFlags2();
    SaveFile *getFileA();
    SaveFile *getFileB();
    u32 calcFileCRC(const void *data, u32 length);
    void updateEmptyFiles();
    void updateEmptyFileFlags();
    bool isFileEmpty(u8 fileNum);
    bool isFileDirty(int fileNum);
    u32 get_0xA84C();
    bool checkRegionCode();
    bool checkFileCRC(u8 fileNum);
    bool isFileInactive() const;
    void setPlayerInfoFileA();
    void setT3Info(const mVec3_c &pos, const mAng3_c &rot);
    static void getRegionVersion(char *out);

    static FileManager *GetInstance() {
        return sInstance;
    }

    bool hasGuardianPotionPlus() const {
        return getGuardianPotionPlusTimer() != 0;
    }

    bool hasGuardianPotionNormal() const {
        return getGuardianPotionTimer() != 0;
    }

    bool hasAirPotionPlus() const {
        return getAirPotionPlusTimer() != 0;
    }

    bool hasAirPotionNormal() const {
        return getAirPotionTimer() != 0;
    }

    bool hasStaminaPotionPlus() const {
        return getStaminaPotionPlusTimer() != 0;
    }

    bool hasStaminaPotionNormal() const {
        return getStaminaPotionTimer() != 0;
    }

    u8 isFileInvalid() const {
        return mIsFileInvalid[2];
    }

    void setField0xA840(u8 val) {
        mIsFileUnk1[0] = val;
    }

    void setField0xA841(u8 val) {
        mIsFileUnk1[1] = val;
    }

    void setField0xA842(u8 val) {
        mIsFileUnk1[2] = val;
    }

    void setField_0xA843(u8 val) {
        mIsFileInvalid[1] = val;
    }

    u8 getField_0xA841() const {
        return mIsFileUnk1[1];
    }

    u8 getField_0xA842() const {
        return mIsFileUnk1[2];
    }

    u8 getField_0xA843() const {
        return mIsFileInvalid[1];
    }
    u8 getField_0xA84D() const {
        return m_0xA84D;
    }

    void setField0xA84C(u8 val) {
        m_0xA84C = val;
    }

    void setField0xA84D(u8 val) {
        m_0xA84D = val;
    }

    void setSelectedFileNum(u8 val) {
        mSelectedFile = val;
    }

    static FileManager *sInstance;

private:
};

#endif
