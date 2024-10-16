#include "toBeSorted/save_file.h"

#include <string.h>


// 0x800099b0   getStoryFlags0__8SaveFileFv
u16 *SaveFile::getStoryFlags0() {
    return story_flags;
}
// 0x800099c0   getStoryFlags1__8SaveFileCFv
const u16 *SaveFile::getStoryFlags1() const {
    return story_flags;
}
// 0x800099d0   getItemFlags0__8SaveFileFv
u16 *SaveFile::getItemFlags0() {
    return item_flags;
}
// 0x800099e0   getItemFlags1__8SaveFileCFv
const u16 *SaveFile::getItemFlags1() const {
    return item_flags;
}
// 0x800099F0   getDungeonFlags0__8SaveFileFv
u16 *SaveFile::getDungeonFlags0() {
    return dungeon_flags[0];
}
// 0x80009A00   getDungeonFlags1__8SaveFileCFv
const u16 *SaveFile::getDungeonFlags1() const {
    return dungeon_flags[0];
}
// 0x80009A10   getSceneFlags0__8SaveFileFv
u16 *SaveFile::getSceneFlags0() {
    return scene_flags;
}
// 0x80009A20   getSceneFlags1__8SaveFileCFv
const u16 *SaveFile::getSceneFlags1() const {
    return scene_flags;
}
// 0x80009A30   getTboxFlags0__8SaveFileFv
u16 *SaveFile::getTboxFlags0() {
    return tbox_flags;
}
// 0x80009A40   getTboxFlags1__8SaveFileCFv
const u16 *SaveFile::getTboxFlags1() const {
    return tbox_flags;
}
// 0x80009A50   getTempFlags0__8SaveFileFv
u16 *SaveFile::getTempFlags0() {
    return temp_flags;
}
// 0x80009A60   getTempFlags1__8SaveFileCFv
const u16 *SaveFile::getTempFlags1() const {
    return temp_flags;
}
// 0x80009A70   getZoneFlags0__8SaveFileFv
u16 *SaveFile::getZoneFlags0() {
    return zone_flags;
}
// 0x80009A80   getZoneFlags1__8SaveFileCFv
const u16 *SaveFile::getZoneFlags1() const {
    return zone_flags;
}
// 0x80009A90   getEnemyFlags0__8SaveFileFv
u16 *SaveFile::getEnemyFlags0() {
    return enemy_flags;
}
// 0x80009AA0   getEnemyFlags1__8SaveFileCFv
const u16 *SaveFile::getEnemyFlags1() const {
    return enemy_flags;
}
// 0x80009AB0   getPlayerName__8SaveFileFv
wchar_t *SaveFile::getPlayerName() {
    return player_name;
}

// 0x80009AC0   setAreaT1__8SaveFileFPc
void SaveFile::setAreaT1(char *name) {
    char nameCopy[32];
    memset(nameCopy, 0, 32);

    for (int i = 0; i < sizeof(nameCopy) - 1; i++) {
        nameCopy[i] = name[i];
    }
    for (int i = 0; i < sizeof(nameCopy); i++) {
        area_t1[i & 0x1F] = nameCopy[i];
    }
}
// 0x80009BE0   getAreaT1__8SaveFileFv
char *SaveFile::getAreaT1() {
    return area_t1;
}
// 0x80009BF0   setAreaT2__8SaveFileFPc
void SaveFile::setAreaT2(char *name) {
    char nameCopy[32];
    memset(nameCopy, 0, 32);

    for (int i = 0; i < sizeof(nameCopy) - 1; i++) {
        nameCopy[i] = name[i];
    }
    for (int i = 0; i < sizeof(nameCopy); i++) {
        area_t2[i & 0x1F] = nameCopy[i];
    }
}
// 0x80009D10   getAreaT2__8SaveFileFv
s8 *SaveFile::getAreaT2() {
    return area_t2;
}
// 0x80009D20   getAreaT3__8SaveFileFv
s8 *SaveFile::getAreaT3() {
    return area_t3;
}
