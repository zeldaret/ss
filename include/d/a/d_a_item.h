#ifndef D_A_ITEM_H
#define D_A_ITEM_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

enum dAcItemID_e {
    /* 0x000 */ ITEM_NONE = 0,
    /* 0x001 */ ITEM_SMALL_KEY = 1,
    /* 0x002 */ ITEM_GREEN_RUPEE = 2,
    /* 0x003 */ ITEM_BLUE_RUPEE = 3,
    /* 0x004 */ ITEM_RED_RUPEE = 4,
    /* 0x005 */ ITEM_COMPLETE_TRIFORCE = 5,
    /* 0x006 */ ITEM_HEART = 6,
    /* 0x007 */ ITEM_SINGLE_ARROW = 7,
    /* 0x008 */ ITEM_BUNDLE_ARROWS = 8,
    /* 0x009 */ ITEM_GODDESS_WHITESWORD = 9,
    /* 0x00A */ ITEM_PRACTICE_SWORD = 10,
    /* 0x00B */ ITEM_GODDESS_SWORD = 11,
    /* 0x00C */ ITEM_GODDESS_LONGSWORD = 12,
    /* 0x00D */ ITEM_MASTER_SWORD = 13,
    /* 0x00E */ ITEM_TRUE_MASTER_SWORD = 14,
    /* 0x00F */ ITEM_SAILCLOTH = 15,
    /* 0x010 */ ITEM_GODDESS_HARP = 16,
    /* 0x011 */ ITEM_SPIRIT_VESSEL = 17,
    /* 0x012 */ ITEM_UNK_18 = 18,
    /* 0x013 */ ITEM_BOW = 19,
    /* 0x014 */ ITEM_CLAWSHOTS = 20,
    /* 0x015 */ ITEM_BIRD_STATUETTE = 21,
    /* 0x016 */ ITEM_DEKU_HORNET = 22,
    /* 0x017 */ ITEM_UNK_23 = 23,
    /* 0x018 */ ITEM_UNK_24 = 24,
    /* 0x019 */ ITEM_AC_BK = 25,
    /* 0x01A */ ITEM_FS_BK = 26,
    /* 0x01B */ ITEM_SSH_BK = 27,
    /* 0x01C */ ITEM_KEY_PIECE = 28,
    /* 0x01D */ ITEM_SV_BK = 29,
    /* 0x01E */ ITEM_ET_BK = 30,
    /* 0x01F */ ITEM_LMF_BK = 31,
    /* 0x020 */ ITEM_SILVER_RUPEE = 32,
    /* 0x021 */ ITEM_GOLD_RUPEE = 33,
    /* 0x022 */ ITEM_RUPOOR = 34,
    /* 0x023 */ ITEM_5_CRYSTALS = 35,
    /* 0x024 */ ITEM_GLITTERING_SPORES = 36,
    /* 0x025 */ ITEM_UNK_37 = 37,
    /* 0x026 */ ITEM_UNK_38 = 38,
    /* 0x027 */ ITEM_UNK_39 = 39,
    /* 0x028 */ ITEM_5_BOMBS = 40,
    /* 0x029 */ ITEM_10_BOMBS = 41,
    /* 0x02A */ ITEM_STAMINA_FRUIT = 42,
    /* 0x02B */ ITEM_FARORE_TEAR = 43,
    /* 0x02C */ ITEM_DIN_TEAR = 44,
    /* 0x02D */ ITEM_NAYRU_TEAR = 45,
    /* 0x02E */ ITEM_SACRED_TEAR = 46,
    /* 0x02F */ ITEM_LIGHT_FRUIT = 47,
    /* 0x030 */ ITEM_1_CRYSTAL = 48,
    /* 0x031 */ ITEM_GUST_BELLOWS = 49,
    /* 0x032 */ ITEM_DUNGEON_MAP_FI = 50,
    /* 0x033 */ ITEM_DUNGEON_MAP = 51,
    /* 0x034 */ ITEM_SLINGSHOT = 52,
    /* 0x035 */ ITEM_BEETLE = 53,
    /* 0x036 */ ITEM_WATER = 54,
    /* 0x037 */ ITEM_MUSHROOM_SPORES = 55,
    /* 0x038 */ ITEM_DIGGING_MITTS = 56,
    /* 0x039 */ ITEM_5_DEKU_SEEDS = 57,
    /* 0x03A */ ITEM_UNK_58 = 58,
    /* 0x03B */ ITEM_UNK_59 = 59,
    /* 0x03C */ ITEM_10_DEKU_SEEDS = 60,
    /* 0x03D */ ITEM_DUSK_RELIC_MAYBE = 61,
    /* 0x03E */ ITEM_DUSK_RELIC = 62,
    /* 0x03F */ ITEM_EVIL_CRYSTAL = 63,
    /* 0x040 */ ITEM_BLUE_FEATHER_GOLD_SKULL_PLUME = 64,
    /* 0x041 */ ITEM_GUARDIAN_POTION = 65,
    /* 0x042 */ ITEM_GUARDIAN_POTION_PLUS = 66,
    /* 0x043 */ ITEM_UNK_67 = 67,
    /* 0x044 */ ITEM_WATER_SCALE = 68,
    /* 0x045 */ ITEM_UNK_69 = 69,
    /* 0x046 */ ITEM_FAIRY = 70,
    /* 0x047 */ ITEM_BUG_NET = 71,
    /* 0x048 */ ITEM_FAIRY_BUG_NET = 72,
    /* 0x049 */ ITEM_UNK_73 = 73,
    /* 0x04A */ ITEM_SACRED_WATER = 74,
    /* 0x04B */ ITEM_HOOK_BEETLE = 75,
    /* 0x04C */ ITEM_QUICK_BEETLE = 76,
    /* 0x04D */ ITEM_TOUGH_BEETLE = 77,
    /* 0x04E */ ITEM_HEART_POTION = 78,
    /* 0x04F */ ITEM_HEART_POTION_PLUS = 79,
    /* 0x050 */ ITEM_HEART_POTION_ALSO = 80,
    /* 0x051 */ ITEM_HEART_POTION_PLUS_PLUS = 81,
    /* 0x052 */ ITEM_UNK_82 = 82,
    /* 0x053 */ ITEM_GUARDIAN_POTION_NO_TBOX = 83,
    /* 0x054 */ ITEM_STAMINA_POTION = 84,
    /* 0x055 */ ITEM_STAMINA_POTION_PLUS = 85,
    /* 0x056 */ ITEM_AIR_POTION = 86,
    /* 0x057 */ ITEM_AIR_POTION_PLUS = 87,
    /* 0x058 */ ITEM_FAIRY_FIRST = 88,
    /* 0x059 */ ITEM_UNK_89 = 89,
    /* 0x05A */ ITEM_IRON_BOW = 90,
    /* 0x05B */ ITEM_SACRED_BOW = 91,
    /* 0x05C */ ITEM_BOMB_BAG = 92,
    /* 0x05D */ ITEM_HEART_CONTAINER = 93,
    /* 0x05E */ ITEM_HEART_PIECE = 94,
    /* 0x05F */ ITEM_TRI_COURAGE = 95,
    /* 0x060 */ ITEM_TRI_POWER = 96,
    /* 0x061 */ ITEM_TRI_WISDOM = 97,
    /* 0x062 */ ITEM_SEA_CHART = 98,
    /* 0x063 */ ITEM_MOGMA_MITTS = 99,
    /* 0x064 */ ITEM_HEART_MEDAL = 100,
    /* 0x065 */ ITEM_RUPEE_MEDAL = 101,
    /* 0x066 */ ITEM_TREASURE_MEDAL = 102,
    /* 0x067 */ ITEM_POTION_MEDAL = 103,
    /* 0x068 */ ITEM_CURSED_MEDAL = 104,
    /* 0x069 */ ITEM_SCATTERSHOT = 105,
    /* 0x06A */ ITEM_UNK_106 = 106,
    /* 0x06B */ ITEM_UNK_107 = 107,
    /* 0x06C */ ITEM_MEDIUM_WALLET = 108,
    /* 0x06D */ ITEM_BIG_WALLET = 109,
    /* 0x06E */ ITEM_GIANT_WALLET = 110,
    /* 0x06F */ ITEM_TYCOON_WALLET = 111,
    /* 0x070 */ ITEM_ADVENTURE_POUCH = 112,
    /* 0x071 */ ITEM_POUCH_EXPANSION = 113,
    /* 0x072 */ ITEM_LIFE_MEDAL = 114,
    /* 0x073 */ ITEM_UNK_115 = 115,
    /* 0x074 */ ITEM_WOODEN_SHIELD = 116,
    /* 0x075 */ ITEM_BANDED_SHIELD = 117,
    /* 0x076 */ ITEM_BRACED_SHIELD = 118,
    /* 0x077 */ ITEM_IRON_SHIELD = 119,
    /* 0x078 */ ITEM_REINFORCED_SHIELD = 120,
    /* 0x079 */ ITEM_FORTIFIED_SHIELD = 121,
    /* 0x07A */ ITEM_SACRED_SHIELD = 122,
    /* 0x07B */ ITEM_DIVINE_SHIELD = 123,
    /* 0x07C */ ITEM_GODDESS_SHIELD = 124,
    /* 0x07D */ ITEM_HYLIAN_SHIELD = 125,
    /* 0x07E */ ITEM_REVITALIZING_POTION = 126,
    /* 0x07F */ ITEM_REVITALIZING_POTION_PLUS = 127,
    /* 0x080 */ ITEM_SMALL_SEED_SATCHEL = 128,
    /* 0x081 */ ITEM_MEDIUM_SEED_SATCHEL = 129,
    /* 0x082 */ ITEM_LARGE_SEED_SATCHEL = 130,
    /* 0x083 */ ITEM_SMALL_QUIVER = 131,
    /* 0x084 */ ITEM_MEDIUM_QUIVER = 132,
    /* 0x085 */ ITEM_LARGE_QUIVER = 133,
    /* 0x086 */ ITEM_SMALL_BOMB_BAG = 134,
    /* 0x087 */ ITEM_MEDIUM_BOMB_BAG = 135,
    /* 0x088 */ ITEM_LARGE_BOMB_BAG = 136,
    /* 0x089 */ ITEM_WHIP = 137,
    /* 0x08A */ ITEM_FIRESHIELD_EARRINGS = 138,
    /* 0x08B */ ITEM_UNK_139 = 139,
    /* 0x08C */ ITEM_BIG_BUG_NET = 140,
    /* 0x08D */ ITEM_FARON_GRASSHOPPER = 141,
    /* 0x08E */ ITEM_WOODLAND_RHINO_BEETLE = 142,
    /* 0x08F */ ITEM_DEKU_HORNET_ = 143,
    /* 0x090 */ ITEM_SKYLOFT_MANTIS = 144,
    /* 0x091 */ ITEM_VOLCANIC_LADYBUG = 145,
    /* 0x092 */ ITEM_BLESSED_BUTTERFLY = 146,
    /* 0x093 */ ITEM_LANAYRU_ANT = 147,
    /* 0x094 */ ITEM_SAND_CICADA = 148,
    /* 0x095 */ ITEM_GERUDO_DRAGONFLY = 149,
    /* 0x096 */ ITEM_ELDIN_ROLLER = 150,
    /* 0x097 */ ITEM_SKY_STAG_BEETLE = 151,
    /* 0x098 */ ITEM_STARRY_FIREFLY = 152,
    /* 0x099 */ ITEM_BOTTLE = 153,
    /* 0x09A */ ITEM_RUPEE_MEDAL_ALSO = 154,
    /* 0x09B */ ITEM_HEART_MEDAL_ALSO = 155,
    /* 0x09C */ ITEM_UNK_156 = 156,
    /* 0x09D */ ITEM_UNK_157 = 157,
    /* 0x09E */ ITEM_CAWLINS_LETTER = 158,
    /* 0x09F */ ITEM_HORNED_COLOSSOL_BEETLE = 159,
    /* 0x0A0 */ ITEM_BABY_RATTLE = 160,
    /* 0x0A1 */ ITEM_HORNET_LARVAE = 161,
    /* 0x0A2 */ ITEM_BIRD_FEATHER = 162,
    /* 0x0A3 */ ITEM_TUMBLE_WEED = 163,
    /* 0x0A4 */ ITEM_LIZARD_TAIL = 164,
    /* 0x0A5 */ ITEM_ELDIN_ORE = 165,
    /* 0x0A6 */ ITEM_ANCIENT_FLOWER = 166,
    /* 0x0A7 */ ITEM_AMBER_RELIC = 167,
    /* 0x0A8 */ ITEM_DUSK_RELIC_AGAIN = 168,
    /* 0x0A9 */ ITEM_JELLY_BLOB = 169,
    /* 0x0AA */ ITEM_MONSTER_CLAW = 170,
    /* 0x0AB */ ITEM_MONSTER_HORN = 171,
    /* 0x0AC */ ITEM_ORNAMENTAL_SKULL = 172,
    /* 0x0AD */ ITEM_EVIL_CRYSTAL_ITEM = 173,
    /* 0x0AE */ ITEM_BLUE_BIRD_FEATHER = 174,
    /* 0x0AF */ ITEM_GOLDEN_SKULL = 175,
    /* 0x0B0 */ ITEM_GODDESS_PLUME = 176,
    /* 0x0B1 */ ITEM_EMERALD_TABLET = 177,
    /* 0x0B2 */ ITEM_RUBY_TABLET = 178,
    /* 0x0B3 */ ITEM_AMBER_TABLET = 179,
    /* 0x0B4 */ ITEM_STONE_OF_TRIALS = 180,
    /* 0x0B5 */ ITEM_UNK_181 = 181,
    /* 0x0B6 */ ITEM_UNK_182 = 182,
    /* 0x0B7 */ ITEM_UNK_183 = 183,
    /* 0x0B8 */ ITEM_UNK_184 = 184,
    /* 0x0B9 */ ITEM_UNK_185 = 185,
    /* 0x0BA */ ITEM_BALLAD_OF_THE_GODDESS = 186,
    /* 0x0BB */ ITEM_FARORES_COURAGE = 187,
    /* 0x0BC */ ITEM_NAYRUS_WISDOM = 188,
    /* 0x0BD */ ITEM_DINS_POWER = 189,
    /* 0x0BE */ ITEM_FARON_SOTH = 190,
    /* 0x0BF */ ITEM_ELDIN_SOTH = 191,
    /* 0x0C0 */ ITEM_LANAYRU_SOTH = 192,
    /* 0x0C1 */ ITEM_SOTH = 193,
    /* 0x0C2 */ ITEM_REVITALIZING_POTION_PLUS_PLUS = 194,
    /* 0x0C3 */ ITEM_HOT_SOUP = 195,
    /* 0x0C4 */ ITEM_COLD_SOUP = 196,
    /* 0x0C5 */ ITEM_LIFE_TREE_SEED = 197,
    /* 0x0C6 */ ITEM_LIFE_TREE_FRUIT = 198,
    /* 0x0C7 */ ITEM_EXTRA_WALLET = 199,
    /* 0x0C8 */ ITEM_UNK_200 = 200,
    /* 0x18D */ ITEM_LIFE_MEDAL_AGAIN = 397,
    /* 0x18E */ ITEM_POUCH_EXPANSION_GODDESS_CHEST = 398,
    /* 0x1FF */ MAX_ITEM_ID = 511
};

class dAcItemBase_c : public dAcObjBase_c {
public:
    dAcItemBase_c() {}
    virtual ~dAcItemBase_c() {}
};

class dAcItem_c : public dAcItemBase_c {
public:
    dAcItem_c();
    virtual ~dAcItem_c();

    STATE_FUNC_DECLARE(dAcItem_c, Wait);
    STATE_FUNC_DECLARE(dAcItem_c, Carry);
    STATE_FUNC_DECLARE(dAcItem_c, GetBeetle);
    STATE_FUNC_DECLARE(dAcItem_c, WaitGet);
    STATE_FUNC_DECLARE(dAcItem_c, Get);
    STATE_FUNC_DECLARE(dAcItem_c, WaitGetDemo);
    STATE_FUNC_DECLARE(dAcItem_c, WaitForcedGetDemo);
    STATE_FUNC_DECLARE(dAcItem_c, GetDemo);
    STATE_FUNC_DECLARE(dAcItem_c, WaitTBoxGetDemo);
    STATE_FUNC_DECLARE(dAcItem_c, ResurgeWait);
    STATE_FUNC_DECLARE(dAcItem_c, WaitTurnOff);
    STATE_FUNC_DECLARE(dAcItem_c, WaitSacredDewGetEffect);

    static u32 createItemParams(u16 itemId, u32 subType, u32 unk, u8 unk2, u8, u8);

    static void spawnItem(u16 item, u32 room, const mVec3_c &pos, const mAng3_c &rot, u32 params, u32 arg);
    static void spawnDrop(u16 item, u32 room, const mVec3_c &pos, const mAng3_c &rot);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcItem_c);
};

#endif
