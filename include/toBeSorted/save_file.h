#ifndef SAVE_FILE_H
#define SAVE_FILE_H

#include "common.h"
#include "m/m_vec.h"

// Ghidra: SaveFile
//   Size: 0x53c0
// non-offical name
struct SaveFile {
public:
    /* 0x0000 */ s64 playTime;
    /* 0x0008 */ s64 savedTime;
    /* 0x0010 */ mVec3_c pos_t1;
    /* 0x001c */ mVec3_c pos_t2;
    /* 0x0028 */ mVec3_c pos_t3;
    /* 0x0034 */ mVec3_c beacon_pos[32][5];
    /* 0x07b4 */ s32 beedleShopPathSegment;
    /* 0x07b8 */ f32 beedlShopPathSegFrac;
    /* 0x07bc */ u32 field_0x07BC;
    /* 0x07c0 */ s32 pouch_items[8];
    /* 0x07e0 */ s32 item_check_items[60];
    /* 0x08d0 */ int file_area_index;
    /* 0x08d4 */ wchar_t player_name[8];
    /* 0x08e4 */ u16 story_flags[128];
    /* 0x09e4 */ u16 item_flags[64];
    /* 0x0a64 */ u16 dungeon_flags[8][22];
    /* 0x0bc4 */ char field_0x0BC4[0x1a64 - 0x0bc4];
    /* 0x1a64 */ u16 scene_flags[208];
    /* 0x1c04 */ char field_0x1c04[0x2a64 - 0x1c04];
    /* 0x2a64 */ u16 tbox_flags[32]; // size is guessed
    /* 0x2aa4 */ char field_0x2AA4[0x2f64 - 0x2aa4];
    /* 0x2f64 */ u16 enemyKillCounts[100];
    /* 0x302c */ u16 hitByEnemyCounts[100];
    /* 0x30f4 */ u16 temp_flags[4];
    /* 0x30fc */ u16 zone_flags[252];
    /* 0x32f4 */ u16 enemy_flags[4096];
    /* 0x52f4 */ u16 air_potion_timer;
    /* 0x52f6 */ u16 air_potion_plus_timer;
    /* 0x52f8 */ u16 stamina_potion_timer;
    /* 0x52fa */ u16 stamina_potion_plus_timer;
    /* 0x52fc */ u16 gaurdian_potion_timer;
    /* 0x52fe */ u16 gaurdian_potion_plus_timer;
    /* 0x5300 */ u16 field_0x5300;
    /* 0x5302 */ u16 health_capacity;
    /* 0x5304 */ u16 unused_heart_related;
    /* 0x5306 */ u16 current_health;
    /* 0x5308 */ u16 room_id_t1;
    /* 0x530a */ u16 room_id_t2;
    /* 0x530c */ u16 room_id_t3;
    /* 0x530e */ s16 angle_t1;
    /* 0x5310 */ s16 angle_t2;
    /* 0x5312 */ s16 angle_t3;
    /* 0x5314 */ s16 beedle_shop_rotation;
    /* 0x5316 */ u16 field_0x5316;
    /* 0x5318 */ s16 scene_flag_index;
    /* 0x531a */ u16 field_0x531A;
    /* 0x531c */ char area_t1[32];
    /* 0x533c */ s8 area_t2[32];
    /* 0x535c */ s8 area_t3[32];
    /* 0x537c */ u8 placed_beacon_flags[32];
    /* 0x539c */ u8 skykeep_puzzle[9];
    /* 0x53a5 */ u8 forced_layer_t1;
    /* 0x53a6 */ u8 forced_layer_t2;
    /* 0x53a7 */ u8 forced_layer_t3;
    /* 0x53a8 */ u8 entrance_t1;
    /* 0x53a9 */ u8 entrance_t1_load_flag;
    /* 0x53aa */ u8 entrance_t2;
    /* 0x53ab */ u8 entrance_t3;
    /* 0x53ac */ u8 field_0x53ac;
    /* 0x53ad */ bool new_file; // set on new file, unset by saving
    /* 0x53ae */ u8 equipped_b_item;
    /* 0x53af */ u8 field_0x53af;
    /* 0x53b0 */ u8 lastUsedPouchItemSlot;
    /* 0x53b1 */ u8 shield_pouch_slot;
    /* 0x53b2 */ u8 selectedDowsingSlot;
    /* 0x53b3 */ u8 night_t1;
    /* 0x53b4 */ u8 night_t3;
    /* 0x53b5 */ char field_0x53b5[0x53bc - 0x53b5];
    /* 0x53bc */ u32 checksum;

    // ----------------------------------------------------------
    //  NOTE: There are functions that are exactly the same code
    //        these end in a number, and likely one is const.
    //        it could also potentially be a static function
    // ----------------------------------------------------------
public:
    u16 *getStoryFlags0();
    const u16 *getStoryFlags1() const;
    u16 *getItemFlags0();
    const u16 *getItemFlags1() const;
    u16 *getDungeonFlags0();
    const u16 *getDungeonFlags1() const;
    u16 *getSceneFlags0();
    const u16 *getSceneFlags1() const;
    u16 *getTboxFlags0();
    const u16 *getTboxFlags1() const;
    u16 *getTempFlags0();
    const u16 *getTempFlags1() const;
    u16 *getZoneFlags0();
    const u16 *getZoneFlags1() const;
    u16 *getEnemyFlags0();
    const u16 *getEnemyFlags1() const;
    wchar_t *getPlayerName(); // UTF16-BE
    void setAreaT1(char *name);
    char *getAreaT1();
    void setAreaT2(char *name);
    s8 *getAreaT2();
    s8 *getAreaT3();
};

#endif
