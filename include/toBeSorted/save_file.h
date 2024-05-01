#ifndef SAVE_FILE_H
#define SAVE_FILE_H

#include "m/m_vec.h"
#include <common.h>

// Ghidra: SaveFile
//   Size: 0x53c0
// non-offical name
struct SaveFile {
public:
    /* 0x 0000 */ s64 playTime;
    /* 0x 0008 */ s64 savedTime;
    /* 0x 0010 */ mVec3_c pos_t1;
    /* 0x 001c */ mVec3_c pos_t2;
    /* 0x 0028 */ mVec3_c pos_t3;
    /* 0x 0034 */ mVec3_c beacon_pos[32][5];
    /* 0x 07b4 */ s32 beedleShopPathSegment;
    /* 0x 07b8 */ f32 beedlShopPathSegFrac;
    /* 0x 07bc */ u32 field_0x07BC;
    /* 0x 07c0 */ s32 pouch_items[8];
    /* 0x 07e0 */ s32 item_check_items[60];
    /* 0x 08d0 */ int file_area_index;
    /* 0x 08d4 */ wchar_t player_name[8];
    /* 0x 08e4 */ u16 story_flags[128];
    /* 0x 09e4 */ u16 item_flags[64];
    /* 0x 0a64 */ u16 dungeon_flags[8][22];
    /* 0x 0bc4 */ char field_0x0BC4[0x1a64 - 0x0bc4];
    /* 0x 1a64 */ u16 scene_flags[208];
    /* 0x 1c04 */ char field_0x1c04[0x2a64 - 0x1c04];
    /* 0x 2a64 */ u16 tbox_flags[32]; // size is guessed
    /* 0x 2aa4 */ char field_0x2AA4[0x2f64 - 0x2aa4];
    /* 0x 2f64 */ u16 enemyKillCounts[100];
    /* 0x 302c */ u16 hitByEnemyCounts[100];
    /* 0x 30f4 */ u16 temp_flags[4];
    /* 0x 30fc */ u16 zone_flags[252];
    /* 0x 32f4 */ u16 enemy_flags[4096];
    /* 0x 52f4 */ u16 air_potion_timer;
    /* 0x 52f6 */ u16 air_potion_plus_timer;
    /* 0x 52f8 */ u16 stamina_potion_timer;
    /* 0x 52fa */ u16 stamina_potion_plus_timer;
    /* 0x 52fc */ u16 gaurdian_potion_timer;
    /* 0x 52fe */ u16 gaurdian_potion_plus_timer;
    /* 0x 5300 */ u16 field_0x5300;
    /* 0x 5302 */ u16 health_capacity;
    /* 0x 5304 */ u16 unused_heart_related;
    /* 0x 5306 */ u16 current_health;
    /* 0x 5308 */ u16 room_id_t1;
    /* 0x 530a */ u16 room_id_t2;
    /* 0x 530c */ u16 room_id_t3;
    /* 0x 530e */ s16 angle_t1;
    /* 0x 5310 */ s16 angle_t2;
    /* 0x 5312 */ s16 angle_t3;
    /* 0x 5314 */ s16 beedle_shop_rotation;
    /* 0x 5316 */ u16 field_0x5316;
    /* 0x 5318 */ s16 scene_flag_index;
    /* 0x 531a */ u16 field_0x531A;
    /* 0x 531c */ char area_t1[32];
    /* 0x 533c */ s8 area_t2[32];
    /* 0x 535c */ s8 area_t3[32];
    /* 0x 537c */ u8 placed_beacon_flags[32];
    /* 0x 539c */ u8 skykeep_puzzle[9];
    /* 0x 53a5 */ u8 forced_layer_t1;
    /* 0x 53a6 */ u8 forced_layer_t2;
    /* 0x 53a7 */ u8 forced_layer_t3;
    /* 0x 53a8 */ u8 entrance_t1;
    /* 0x 53a9 */ u8 entrance_t1_load_flag;
    /* 0x 53aa */ u8 entrance_t2;
    /* 0x 53ab */ u8 entrance_t3;
    /* 0x 53ac */ u8 field_0x53ac;
    /* 0x 53ad */ bool new_file; // set on new file, unset by saving
    /* 0x 53ae */ u8 equipped_b_item;
    /* 0x 53af */ u8 field_0x53af;
    /* 0x 53b0 */ u8 lastUsedPouchItemSlot;
    /* 0x 53b1 */ u8 shield_pouch_slot;
    /* 0x 53b2 */ u8 selectedDowsingSlot;
    /* 0x 53b3 */ u8 night_t1;
    /* 0x 53b4 */ u8 night_t3;
    /* 0x 53b5 */ char field_0x53b5[0x53bc - 0x53b5];
    /* 0x 53bc */ u32 checksum;

    // ----------------------------------------------------------
    //  NOTE: There are functions that are exactly the same code
    //        these end in a number, and likely one is const.
    //        it could also potentially be a static function
    // ----------------------------------------------------------
public:
    /* 800099b0 */ u16 *getStoryFlags0();
    /* 800099c0 */ const u16 *getStoryFlags1() const;
    /* 800099d0 */ u16 *getItemFlags0();
    /* 800099e0 */ const u16 *getItemFlags1() const;
    /* 800099F0 */ u16 *getDungeonFlags0();
    /* 80009A00 */ const u16 *getDungeonFlags1() const;
    /* 80009A10 */ u16 *getSceneFlags0();
    /* 80009A20 */ const u16 *getSceneFlags1() const;
    /* 80009A30 */ u16 *getTboxFlags0();
    /* 80009A40 */ const u16 *getTboxFlags1() const;
    /* 80009A50 */ u16 *getTempFlags0();
    /* 80009A60 */ const u16 *getTempFlags1() const;
    /* 80009A70 */ u16 *getZoneFlags0();
    /* 80009A80 */ const u16 *getZoneFlags1() const;
    /* 80009A90 */ u16 *getEnemyFlags0();
    /* 80009AA0 */ const u16 *getEnemyFlags1() const;
    /* 80009AB0 */ wchar_t *getPlayerName(); // UTF16-BE
    /* 80009AC0 */ void setAreaT1(char *name);
    /* 80009BE0 */ char *getAreaT1();
    /* 80009BF0 */ void setAreaT2(char *name);
    /* 80009D10 */ s8 *getAreaT2();
    /* 80009D20 */ s8 *getAreaT3();
};

#endif
