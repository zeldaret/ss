#include "egg/core/eggHeap.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "toBeSorted/save_file.h"
#include <common.h>


enum ITEM_ID {};
enum SAVE_ITEM_ID {};

class FileManager {
public:
    /* 0x0000 */ void *mpSavedSaveFiles;
    /* 0x0004 */ void *mpSkipData; // skip data Arrary (3 entries )
    /* 0x0008 */ SaveFile mFileA;
    /* 0x53C8 */ SaveFile mFileB;
    /* 0xA788 */ u16 mSkipFlags[16];
    /* 0xA7A8 */ u32 mSkipFlagsCRC;
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
    // /* 80009D30 */ void fn_80009D30(); // some ctor
    // /* 80009D40 */ void fn_80009D40(); // some dtor
    // /* 80009D80 */ void fn_80009D80(); // return
    // /* 80009D90 */ void fn_80009D90(); // return
    // /* 80009DA0 */ void fn_80009DA0(); // memset(param_1, 0, 0x20) a 0x24 structure is implied here (0x20 data) a crc
    // is at 0x24

    /* 80009DB0 */ FileManager(); //
    /* 80009EE0 */                // mVec3();

    /* 80009EF0 */ static FileManager create(EGG::Heap *);
    /* 80009F30 */ bool loadSaveData(void *out, char *name, bool isSkipData);
    /* 80009F70 */ void saveSaveData(void *unk, bool isSkipData);
    /* 8000A000 */ void refreshSaveFileData();
    /* 8000A260 */ wchar_t *getFileHeroname(int fileNum);
    /* 8000A280 */ s64 getFileSaveTime(int fileNum);
    /* 8000A2A0 */ s16 getFileCurrentHealth(int fileNum);
    /* 8000A2C0 */ s16 getFileHealthCapacity(int fileNum);
    /* 8000A2E0 */ void fn_8000A2E0(); // idk something blank save files

    /* 8000A330 */ u16 *getStoryFlagsMut();
    /* 8000A360 */ u16 *getStoryFlagsConst();
    /* 8000A3B0 */ u16 *getItemFlagsMut();
    /* 8000A3E0 */ u16 *getItemFlagsConst();
    /* 8000A430 */ u16 *getDungeonFlagsMut();
    /* 8000A460 */ u16 *getDungeonFlagsConst();
    /* 8000A4B0 */ u16 *getSceneFlagsMut();
    /* 8000A4E0 */ u16 *getSceneFlagsConst();
    /* 8000A530 */ u16 *getTBoxFlagsMut();
    /* 8000A560 */ u16 *getTBoxFlagsConst();
    /* 8000A5B0 */ u16 *getTempFlagsMut();
    /* 8000A5E0 */ u16 *getTempFlagsConst();
    /* 8000A630 */ u16 *getZoneFlagsMut();
    /* 8000A660 */ u16 *getZoneFlagsConst();
    /* 8000A6B0 */ u16 *getEnemyDefeatFlagsMut();
    /* 8000A6E0 */ u16 *getEnemyDefeatFlagsConst();

    /* 8000A730 */ void setStoryFlags(u16 *flags, u32 offset, u16 count);
    /* 8000A790 */ void setItemFlags(u16 *flags, u32 offset, u16 count);
    /* 8000A7F0 */ void setDungeonFlags(u16 *flags, u32 offset, u16 count);
    /* 8000A850 */ void setSceneFlags(u16 *flags, u32 offset, u16 count);
    /* 8000A8B0 */ void setTBoxFlags(u16 *flags, u32 offset, u16 count);
    /* 8000A910 */ void setTempFlags(u16 *flags, u32 offset, u16 count);
    /* 8000A970 */ void setZoneFlags(u16 *flags, u32 offset, u16 count);
    /* 8000A9D0 */ void setEnemyDefeatFlags(u16 *flags, u32 offset, u16 count);

    /* 8000AA30 */ u16 *getSkipFlags();
    /* 8000AA40 */ void setSkipFlagsChecked(u16 *flags, u32 offset, u16 count);

    /* 8000AAA0 */ void initFile(int fileNum);

    /* 8000ABD0 */ void setCurrentHealthCapacity(s16 health);
    /* 8000AC00 */ s16 getCurrentHealthCapacity();
    /* 8000AC50 */ void setCurrentHealth(s16 health);
    /* 8000AC80 */ s16 getCurrentHealth();

    /* 8000ACD0 */ u16 getLoadRoomT1();
    /* 8000AD20 */ u16 getLoadRoomT2();
    /* 8000AD70 */ void setLoadRoomT3(s16 room);
    /* 8000ADA0 */ u16 getLoadRoomT3();

    /* 8000ADF0 */ mVec3_c *getPosT1();
    /* 8000AE40 */ void setPosT2(mVec3_c *pos);
    /* 8000AE90 */ mVec3_c *getPosT2();
    /* 8000AEE0 */ void setPosT3(mVec3_c *pos);
    /* 8000AF30 */ mVec3_c *getPosT3();

    /* 8000AF80 */ s16 getAngleT1();
    /* 8000AFD0 */ void setAngleT2(s16 angle);
    /* 8000B000 */ s16 getAngleT2();
    /* 8000B050 */ void setAngleT3(s16 angle);
    /* 8000B080 */ s16 getAngleT3();

    /* 8000B0D0 */ void setPouchData(s32 slot, u32 slotData);
    /* 8000B130 */ u32 getPouchData(s32 slot);
    /* 8000B1B0 */ void setPouchItem(s32 slot, ITEM_ID item);
    /* 8000B220 */ ITEM_ID getPouchItem(s32 slot);
    /* 8000B250 */ void setPouchUpperData(s32 slot, s32 data);
    /* 8000B2C0 */ u16 getPouchUpperData(s32 slot);

    /* 8000B2F0 */ void setItemCheckData(u32 idx, u32 itemData);
    /* 8000B360 */ u32 getItemCheckData(u32 idx);
    /* 8000B3F0 */ void setItemCheckItem(u32 idx, ITEM_ID item);
    /* 8000B480 */ ITEM_ID getItemCheckItem(u32 idx);
    /* 8000B4B0 */ void setItemCheckUpperData(u32 idx, u32 itemData);
    /* 8000B540 */ u32 getItemCheckUpperData();

    /* 8000B570 */ void setEquippedItem(u8 bWheelItem);
    /* 8000B5A0 */ u8 getEquippedItem();
    /* 8000B5F0 */ void setSelectedPouchSlot(u8 slot);
    /* 8000B620 */ u8 getSelectedPouchSlot();
    /* 8000B670 */ void setShieldPouchSlot(u8 slot);
    /* 8000B6A0 */ u8 getShieldPouchSlot();

    /* 8000B6F0 */ void setAirPotionTimer(s16 time);
    /* 8000B720 */ s16 getAirPotionTimer();
    /* 8000B770 */ void setAirPotionPlusTimer(s16 time);
    /* 8000B7A0 */ s16 getAirPotionPlusTimer();
    /* 8000B7F0 */ void setStaminaPotionTimer(s16 time);
    /* 8000B820 */ s16 getStaminaPotionTimer();
    /* 8000B870 */ void setStaminaPotionPlusTimer(s16 time);
    /* 8000B8A0 */ s16 getStaminaPotionPlusTimer();
    /* 8000B8F0 */ void setGuardianPotionTimer(s16 time);
    /* 8000B920 */ s16 getGuardianPotionTimer();
    /* 8000B970 */ void setGuardianPotionPlusTimer(s16 time);
    /* 8000B9A0 */ s16 getGuardianPotionPlusTimer();

    /* 8000B9F0 */ void setDowsingSlotIdx(u8 idx);
    /* 8000BA20 */ u8 getDowsingSlotIdx();

    /* 8000BA70 */ void setBeaconPos(u32 beaconArea, u32 beaconNum, mVec3_c *pos);
    /* 8000BB80 */ mVec3_c *getBeaconPos(u32 beaconArea, u32 beaconNum);
    /* 8000BC70 */ void setEnemyKillCount(u32 enemy, u16 killCount);
    /* 8000BCE0 */ u16 getEnemyKillCount(u32 enemy);
    /* 8000BD60 */ void setHitCountFromEnemy(u32 enemy, u16 hitCount);
    /* 8000BDD0 */ u16 getHitCountFromEnemy(u32 enemy);

    /* 8000BE50 */ void setLoadRoomT1_FileB(s16 roomId);
    /* 8000BE80 */ s16 getLoadRoomT1_FileB();
    /* 8000BE90 */ void setEntranceT1_FileB(u8 entrance);
    /* 8000BEC0 */ u8 getEntranceT1_FileB();
    /* 8000BED0 */ void setAreaT1_FileB(const char *name);
    /* 8000BF80 */ char *getAreaT1_FileB();
    /* 8000C080 */ void setNightT1_FileB(bool night);
    /* 8000C0B0 */ bool getNightT1_FileB();
    /* 8000C0C0 */ void setEntranceT1LoadFlag_FileB(u8 flag);
    /* 8000C0F0 */ void setForcedLayerT1_FileB(u8 layer);
    /* 8000C120 */ void getForcedLayerT1_FileB();
    /* 8000C130 */ void setCurrentHealth_FileB(s16 health);

    /* 8000C160 */ void setHeroname(const wchar_t *name);
    /* 8000C230 */ wchar_t *getHeroname();

    /* 8000C360 */ char *getAreaT1();
    /* 8000C470 */ char *getAreaT2();
    /* 8000C580 */ void setAreaT3(const char *name);
    /* 8000C630 */ char *getAreaT3();

    /* 8000C740 */ u8 getForcedLayerT1();
    /* 8000C790 */ u8 getForcedLayerT2();
    /* 8000C7E0 */ void setForcedLayerT3(u8 layer);
    /* 8000C810 */ u8 getForcedLayerT3();

    /* 8000C860 */ u8 getEntranceT1();
    /* 8000C8B0 */ void setEntranceLoadFlagT1(u8 flag);
    /* 8000C8E0 */ u8 getEntranceLoadFlagT1();
    /* 8000C930 */ void getEntranceT2();
    /* 8000C980 */ void setEntranceT3(u8 entrance);
    /* 8000C9B0 */ u8 getEntranceT3();

    /* 8000CA00 */ bool getNightT1();
    /* 8000CA50 */ void setNightT3(bool night);
    /* 8000CA80 */ bool getNightT3();

    /* 8000CAD0 */ bool isNew_FileA();

    /* 8000CB00 */ void setSceneFlagIndex(u16 idx);
    /* 8000CB30 */ u16 getSceneFlagIndex();
    /* 8000CB80 */ s32 getFileAreaIndex();

    /* 8000CBD0 */ void fn_8000CBD0(u8);
    /* 8000CC00 */ void fn_8000CC00();

    /* 8000CC50 */ void setFileTimes();
    /* 8000CCB0 */ void setPlayTime(s64 time);

    /* 8000CCF0 */ s64 getSavedTime();
    /* 8000CD40 */ void setSavedTime(s64 time);

    /* 8000CD80 */ void setBeedleShopPathSegment(u32 path);
    /* 8000CDB0 */ u32 getBeedleShopPathSegment();
    /* 8000CE00 */ void setBeedleShopPathSegFrac(f32 segFrac);
    /* 8000CE30 */ f32 getBeedleShopPathSegFrac();
    /* 8000CE80 */ void setBeedleShopRotation(s16 rot);
    /* 8000CEB0 */ s16 getBeedleShopRotation();

    /* 8000CF00 */ void fn_8000CF00(u32 shift);
    /* 8000CF70 */ void fn_8000CF70(u32 shift);

    /* 8000CFE0 */ void setSkykeepPuzzle(u32 spot, u8 tile);
    /* 8000D040 */ u8 getSkykeepPuzzleTile(u32 spot);

    /* 8000D0B0 */ void checkFileStatus();
    /* 8000D1D0 */ void checkSkipDataCRCs();
    /* 8000D270 */ void saveOrClearSelectedFileToFileA();
    /* 8000D280 */ void saveOrClearToFileA(int fileNum);
    /* 8000D9C0 */ void copyFileBToCurrentFile();
    /* 8000E060 */ void copyFileAToSelectedFile();
    /* 8000E070 */ void copyFileAToFile(int fileNum);
    /* 8000E7C0 */ void copyFile(int from, int to);
    /* 8000EF80 */ void saveFileAToSelectedFile();
    /* 8000EF90 */ void saveFileAToFile(int fileNum);
    /* 8000F730 */ void copyCurrentToFileB();
    /* 8000FDF0 */ void copySelectedFileSkipData();
    /* 8000FE00 */ void copySkipData(int fileNum);
    /* 8000FEB0 */ void setInfo_FileB();
    /* 8000FF60 */ void clearFileA();

    /* 80010000 */ void initBlankSaveFiles();
    /* 80010160 */ void initSkipData();

    /* 800101F0 */ void unsetFileANewFile();
    /* 80010220 */ void saveT1SaveInfo(u8 entranceT1LoadFlag);
    /* 80010350 */ void copyFileSkipData(int fileNum);
    /* 80010440 */ void clearTempFileData();
    /* 800104A0 */ void saveAfterCredits();
    /* 80011210 */ SaveFile *getCurrentFile();
    /* 80011250 */ u16 *getSkipFlags2();
    /* 80011260 */ SaveFile *getFileA();
    /* 80011270 */ SaveFile *getFileB();
    /* 80011280 */ void calcFileCRC(const SaveFile *file, u32 length);
    /* 80011290 */ void updateEmptyFiles();
    /* 800112D0 */ void updateEmptyFileFlags();
    /* 80011370 */ bool isFileEmpty(int fileNum);
    /* 80011390 */ bool isFileUnk3(int fileNum);
    /* 800113B0 */ u8 get_0xA84C();
    /* 800113C0 */ bool checkRegionCode();
    /* 80011440 */ bool checkFileCRC(int fileNum);
    /* 80011490 */ bool isFileInactive();
    /* 80011500 */ void setPlayerInfoFileA();
    /* 800115E0 */ void setT3Info(mVec3_c *pos, mAng3_c *rot);
    /* 800116C0 */ static void getRegionVersion(char *out);
    // /* 800116F0 */ void sinit();

    static FileManager *getInstance() {
        return sInstance;
    }
    static FileManager *sInstance;
};
