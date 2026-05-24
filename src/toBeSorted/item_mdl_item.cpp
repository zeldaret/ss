#include "toBeSorted/item_mdl_item.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/item_mdl_name.h"

enum ItemIdx {
    MDL_ITEM_COMPLETE_TRIFORCE,
    MDL_ITEM_GODDESS_WHITESWORD,
    MDL_ITEM_PRACTICE_SWORD,
    MDL_ITEM_GODDESS_SWORD,
    MDL_ITEM_GODDESS_LONGSWORD,
    MDL_ITEM_MASTER_SWORD,
    MDL_ITEM_TRUE_MASTER_SWORD,
    MDL_ITEM_SAILCLOTH,
    MDL_ITEM_GODDESS_HARP,
    MDL_ITEM_SPIRIT_VESSEL,
    MDL_ITEM_BOW,
    MDL_ITEM_CLAWSHOTS,
    MDL_ITEM_BIRD_STATUETTE,
    MDL_ITEM_AC_BK,
    MDL_ITEM_FS_BK,
    MDL_ITEM_SSH_BK,
    MDL_ITEM_KEY_PIECE,
    MDL_ITEM_SV_BK,
    MDL_ITEM_ET_BK,
    MDL_ITEM_LMF_BK,
    MDL_ITEM_GLITTERING_SPORES,
    MDL_ITEM_5_BOMBS,
    MDL_ITEM_GUST_BELLOWS,
    MDL_ITEM_DUNGEON_MAP,
    MDL_ITEM_SLINGSHOT,
    MDL_ITEM_BEETLE,
    MDL_ITEM_MUSHROOM_SPORES,
    MDL_ITEM_DIGGING_MITTS,
    MDL_ITEM_28, // unused
    MDL_ITEM_10_DEKU_SEEDS,
    MDL_ITEM_WATER_SCALE,
    MDL_ITEM_BUG_MEDAL,
    MDL_ITEM_BUG_NET,
    MDL_ITEM_HOOK_BEETLE,
    MDL_ITEM_QUICK_BEETLE,
    MDL_ITEM_TOUGH_BEETLE,
    MDL_ITEM_IRON_BOW,
    MDL_ITEM_SACRED_BOW,
    MDL_ITEM_BOMB_BAG,
    MDL_ITEM_SEA_CHART,
    MDL_ITEM_MOGMA_MITTS,
    MDL_ITEM_HEART_MEDAL,
    MDL_ITEM_RUPEE_MEDAL,
    MDL_ITEM_TREASURE_MEDAL,
    MDL_ITEM_POTION_MEDAL,
    MDL_ITEM_CURSED_MEDAL,
    MDL_ITEM_MIGHTY_SCATTERSHOT,
    MDL_ITEM_47,
    MDL_ITEM_MEDIUM_WALLET,
    MDL_ITEM_BIG_WALLET,
    MDL_ITEM_GIANT_WALLET,
    MDL_ITEM_TYCOON_WALLET,
    MDL_ITEM_ADVENTURE_POUCH,
    MDL_ITEM_POUCH_EXPANSION,
    MDL_ITEM_LIFE_MEDAL,
    MDL_ITEM_HYLIAN_SHIELD,
    MDL_ITEM_REVITALIZING_POTION,
    MDL_ITEM_SMALL_SEED_SATCHEL,
    MDL_ITEM_MEDIUM_SEED_SATCHEL,
    MDL_ITEM_LARGE_SEED_SATCHEL,
    MDL_ITEM_SMALL_QUIVER,
    MDL_ITEM_MEDIUM_QUIVER,
    MDL_ITEM_LARGE_QUIVER,
    MDL_ITEM_SMALL_BOMB_BAG,
    MDL_ITEM_MEDIUM_BOMB_BAG,
    MDL_ITEM_LARGE_BOMB_BAG,
    MDL_ITEM_WHIP,
    MDL_ITEM_FIRESHIELD_EARRINGS,
    MDL_ITEM_BIG_BUG_NET,
    MDL_ITEM_FARON_GRASSHOPPER,
    MDL_ITEM_WOODLAND_RHINO_BEETLE,
    MDL_ITEM_DEKU_HORNET,
    MDL_ITEM_SKYLOFT_MANTIS,
    MDL_ITEM_VOLCANIC_LADYBUG,
    MDL_ITEM_BLESSED_BUTTERFLY,
    MDL_ITEM_LANAYRU_ANT,
    MDL_ITEM_SAND_CICADA,
    MDL_ITEM_GERUDO_DRAGONFLY,
    MDL_ITEM_ELDIN_ROLLER,
    MDL_ITEM_SKY_STAG_BEETLE,
    MDL_ITEM_BOTTLE,
    MDL_ITEM_CAWLIN_LETTER,
    MDL_ITEM_BEEDLE_INSECT_CAGE,
    MDL_ITEM_HORNET_LARVAE,
    MDL_ITEM_BIRD_FEATHER,
    MDL_ITEM_TUMBLE_WEED,
    MDL_ITEM_LIZARD_TAIL,
    MDL_ITEM_ELDIN_ORE,
    MDL_ITEM_ANCIENT_FLOWER,
    MDL_ITEM_AMBER_RELIC,
    MDL_ITEM_DUSK_RELIC,
    MDL_ITEM_MONSTER_CLAW,
    MDL_ITEM_MONSTER_HORN,
    MDL_ITEM_ORNAMENTAL_SKULL,
    MDL_ITEM_EVIL_CRYSTAL,
    MDL_ITEM_BLUE_BIRD_FEATHER,
    MDL_ITEM_GOLDEN_SKULL,
    MDL_ITEM_GODDESS_PLUME,
    MDL_ITEM_EMERALD_TABLET,
    MDL_ITEM_RUBY_TABLET,
    MDL_ITEM_AMBER_TABLET,
    MDL_ITEM_STONE_OF_TRIALS,
    MDL_ITEM_REVITALIZING_POTION_PLUS_PLUS,
    MDL_ITEM_LIFE_TREE_SEED,
    MDL_ITEM_LIFE_TREE_FRUIT,
    MDL_ITEM_EXTRA_WALLET,
    MDL_ITEM_MAX = 106,
};

const u8 dItemMdlItem_c::sMdlIdx[] = {
    MDL_ITEM_MAX,                           /* ITEMMDL_SMALL_KEY */
    MDL_ITEM_MAX,                           /* ITEMMDL_GREEN_RUPEE */
    MDL_ITEM_MAX,                           /* ITEMMDL_BLUE_RUPEE */
    MDL_ITEM_MAX,                           /* ITEMMDL_RED_RUPEE */
    MDL_ITEM_COMPLETE_TRIFORCE,             /* ITEMMDL_COMPLETE_TRIFORCE */
    MDL_ITEM_MAX,                           /* ITEMMDL_HEART */
    MDL_ITEM_MAX,                           /* ITEMMDL_SINGLE_ARROW */
    MDL_ITEM_MAX,                           /* ITEMMDL_BUNDLE_ARROWS */
    MDL_ITEM_GODDESS_WHITESWORD,            /* ITEMMDL_GODDESS_WHITESWORD */
    MDL_ITEM_PRACTICE_SWORD,                /* ITEMMDL_PRACTICE_SWORD */
    MDL_ITEM_GODDESS_SWORD,                 /* ITEMMDL_GODDESS_SWORD */
    MDL_ITEM_GODDESS_LONGSWORD,             /* ITEMMDL_GODDESS_LONGSWORD */
    MDL_ITEM_MASTER_SWORD,                  /* ITEMMDL_MASTER_SWORD */
    MDL_ITEM_TRUE_MASTER_SWORD,             /* ITEMMDL_TRUE_MASTER_SWORD */
    MDL_ITEM_SAILCLOTH,                     /* ITEMMDL_SAILCLOTH */
    MDL_ITEM_GODDESS_HARP,                  /* ITEMMDL_GODDESS_HARP */
    MDL_ITEM_SPIRIT_VESSEL,                 /* ITEMMDL_SPIRIT_VESSEL */
    MDL_ITEM_BOW,                           /* ITEMMDL_BOW */
    MDL_ITEM_CLAWSHOTS,                     /* ITEMMDL_CLAWSHOTS */
    MDL_ITEM_BIRD_STATUETTE,                /* ITEMMDL_BIRD_STATUETTE */
    MDL_ITEM_AC_BK,                         /* ITEMMDL_AC_BK */
    MDL_ITEM_FS_BK,                         /* ITEMMDL_FS_BK */
    MDL_ITEM_SSH_BK,                        /* ITEMMDL_SSH_BK */
    MDL_ITEM_KEY_PIECE,                     /* ITEMMDL_KEY_PIECE */
    MDL_ITEM_SV_BK,                         /* ITEMMDL_SV_BK */
    MDL_ITEM_ET_BK,                         /* ITEMMDL_ET_BK */
    MDL_ITEM_LMF_BK,                        /* ITEMMDL_LMF_BK */
    MDL_ITEM_MAX,                           /* ITEMMDL_SILVER_RUPEE */
    MDL_ITEM_MAX,                           /* ITEMMDL_GOLD_RUPEE */
    MDL_ITEM_MAX,                           /* ITEMMDL_RUPOOR */
    MDL_ITEM_MAX,                           /* ITEMMDL_5_CRYSTALS */
    MDL_ITEM_GLITTERING_SPORES,             /* ITEMMDL_GLITTERING_SPORES */
    MDL_ITEM_5_BOMBS,                       /* ITEMMDL_5_BOMBS */
    MDL_ITEM_MAX,                           /* ITEMMDL_10_BOMBS */
    MDL_ITEM_MAX,                           /* ITEMMDL_STAMINA_FRUIT */
    MDL_ITEM_MAX,                           /* ITEMMDL_FARORE_TEAR */
    MDL_ITEM_MAX,                           /* ITEMMDL_DIN_TEAR */
    MDL_ITEM_MAX,                           /* ITEMMDL_NAYRU_TEAR */
    MDL_ITEM_MAX,                           /* ITEMMDL_SACRED_TEAR */
    MDL_ITEM_MAX,                           /* ITEMMDL_LIGHT_FRUIT */
    MDL_ITEM_MAX,                           /* ITEMMDL_1_CRYSTAL */
    MDL_ITEM_GUST_BELLOWS,                  /* ITEMMDL_GUST_BELLOWS */
    MDL_ITEM_DUNGEON_MAP,                   /* ITEMMDL_DUNGEON_MAP_FI */
    MDL_ITEM_DUNGEON_MAP,                   /* ITEMMDL_DUNGEON_MAP */
    MDL_ITEM_SLINGSHOT,                     /* ITEMMDL_SLINGSHOT */
    MDL_ITEM_BEETLE,                        /* ITEMMDL_BEETLE */
    MDL_ITEM_MAX,                           /* ITEMMDL_WATER */
    MDL_ITEM_MUSHROOM_SPORES,               /* ITEMMDL_MUSHROOM_SPORES */
    MDL_ITEM_DIGGING_MITTS,                 /* ITEMMDL_DIGGING_MITTS */
    MDL_ITEM_MAX,                           /* ITEMMDL_5_DEKU_SEEDS */
    MDL_ITEM_10_DEKU_SEEDS,                 /* ITEMMDL_10_DEKU_SEEDS */
    MDL_ITEM_MAX,                           /* ITEMMDL_GUARDIAN_POTION */
    MDL_ITEM_MAX,                           /* ITEMMDL_GUARDIAN_POTION_PLUS */
    MDL_ITEM_WATER_SCALE,                   /* ITEMMDL_WATER_SCALE */
    MDL_ITEM_BUG_MEDAL,                     /* ITEMMDL_BUG_MEDAL */
    MDL_ITEM_BUG_NET,                       /* ITEMMDL_BUG_NET */
    MDL_ITEM_MAX,                           /* ITEMMDL_FAIRY */
    MDL_ITEM_MAX,                           /* ITEMMDL_SACRED_WATER */
    MDL_ITEM_HOOK_BEETLE,                   /* ITEMMDL_HOOK_BEETLE */
    MDL_ITEM_QUICK_BEETLE,                  /* ITEMMDL_QUICK_BEETLE */
    MDL_ITEM_TOUGH_BEETLE,                  /* ITEMMDL_TOUGH_BEETLE */
    MDL_ITEM_MAX,                           /* ITEMMDL_HEART_POTION */
    MDL_ITEM_MAX,                           /* ITEMMDL_HEART_POTION_PLUS */
    MDL_ITEM_MAX,                           /* ITEMMDL_HEART_POTION_ALSO_PLUS */
    MDL_ITEM_MAX,                           /* ITEMMDL_STAMINA_POTION */
    MDL_ITEM_MAX,                           /* ITEMMDL_STAMINA_POTION_PLUS */
    MDL_ITEM_MAX,                           /* ITEMMDL_AIR_POTION */
    MDL_ITEM_MAX,                           /* ITEMMDL_AIR_POTION_PLUS */
    MDL_ITEM_MAX,                           /* ITEMMDL_BOTTLED_FAIRY */
    MDL_ITEM_IRON_BOW,                      /* ITEMMDL_IRON_BOW */
    MDL_ITEM_SACRED_BOW,                    /* ITEMMDL_SACRED_BOW */
    MDL_ITEM_BOMB_BAG,                      /* ITEMMDL_BOMB_BAG */
    MDL_ITEM_MAX,                           /* ITEMMDL_HEART_CONTAINER */
    MDL_ITEM_MAX,                           /* ITEMMDL_HEART_PIECE */
    MDL_ITEM_MAX,                           /* ITEMMDL_TRI_COURAGE */
    MDL_ITEM_MAX,                           /* ITEMMDL_TRI_POWER */
    MDL_ITEM_MAX,                           /* ITEMMDL_TRI_WISDOWM */
    MDL_ITEM_SEA_CHART,                     /* ITEMMDL_SEA_CHART */
    MDL_ITEM_MOGMA_MITTS,                   /* ITEMMDL_MOGMA_MITTS */
    MDL_ITEM_HEART_MEDAL,                   /* ITEMMDL_HEART_MEDAL */
    MDL_ITEM_RUPEE_MEDAL,                   /* ITEMMDL_RUPEE_MEDAL */
    MDL_ITEM_TREASURE_MEDAL,                /* ITEMMDL_TREASURE_MEDAL */
    MDL_ITEM_POTION_MEDAL,                  /* ITEMMDL_POTION_MEDAL */
    MDL_ITEM_CURSED_MEDAL,                  /* ITEMMDL_CURSED_MEDAL */
    MDL_ITEM_MIGHTY_SCATTERSHOT,            /* ITEMMDL_MIGHTY_SCATTERSHOT */
    MDL_ITEM_47,                            /* ITEMMDL_85 */
    MDL_ITEM_MEDIUM_WALLET,                 /* ITEMMDL_MEDIUM_WALLET */
    MDL_ITEM_BIG_WALLET,                    /* ITEMMDL_BIG_WALLET */
    MDL_ITEM_GIANT_WALLET,                  /* ITEMMDL_GIANT_WALLET */
    MDL_ITEM_TYCOON_WALLET,                 /* ITEMMDL_TYCOON_WALLET */
    MDL_ITEM_ADVENTURE_POUCH,               /* ITEMMDL_ADVENTURE_POUCH */
    MDL_ITEM_POUCH_EXPANSION,               /* ITEMMDL_POUCH_EXPANSION */
    MDL_ITEM_LIFE_MEDAL,                    /* ITEMMDL_LIFE_MEDAL */
    MDL_ITEM_MAX,                           /* ITEMMDL_WOODEN_SHIELD */
    MDL_ITEM_MAX,                           /* ITEMMDL_BANDED_SHIELD */
    MDL_ITEM_MAX,                           /* ITEMMDL_BRACED_SHIELD */
    MDL_ITEM_MAX,                           /* ITEMMDL_IRON_SHIELD */
    MDL_ITEM_MAX,                           /* ITEMMDL_REINFORCED_SHIELD */
    MDL_ITEM_MAX,                           /* ITEMMDL_FORTIFIED_SHIELD */
    MDL_ITEM_MAX,                           /* ITEMMDL_SACRED_SHIELD */
    MDL_ITEM_MAX,                           /* ITEMMDL_DIVINE_SHIELD */
    MDL_ITEM_MAX,                           /* ITEMMDL_GODDESS_SHIELD */
    MDL_ITEM_HYLIAN_SHIELD,                 /* ITEMMDL_HYLIAN_SHIELD */
    MDL_ITEM_REVITALIZING_POTION,           /* ITEMMDL_REVITALIZING_POTION */
    MDL_ITEM_REVITALIZING_POTION,           /* ITEMMDL_REVITALIZING_POTION_PLUS */
    MDL_ITEM_SMALL_SEED_SATCHEL,            /* ITEMMDL_SMALL_SEED_SATCHEL */
    MDL_ITEM_MEDIUM_SEED_SATCHEL,           /* ITEMMDL_MEDIUM_SEED_SATCHEL */
    MDL_ITEM_LARGE_SEED_SATCHEL,            /* ITEMMDL_LARGE_SEED_SATCHEL */
    MDL_ITEM_SMALL_QUIVER,                  /* ITEMMDL_SMALL_QUIVER */
    MDL_ITEM_MEDIUM_QUIVER,                 /* ITEMMDL_MEDIUM_QUIVER */
    MDL_ITEM_LARGE_QUIVER,                  /* ITEMMDL_LARGE_QUIVER */
    MDL_ITEM_SMALL_BOMB_BAG,                /* ITEMMDL_SMALL_BOMB_BAG */
    MDL_ITEM_MEDIUM_BOMB_BAG,               /* ITEMMDL_MEDIUM_BOMB_BAG */
    MDL_ITEM_LARGE_BOMB_BAG,                /* ITEMMDL_LARGE_BOMB_BAG */
    MDL_ITEM_WHIP,                          /* ITEMMDL_WHIP */
    MDL_ITEM_FIRESHIELD_EARRINGS,           /* ITEMMDL_FIRESHIELD_EARRINGS */
    MDL_ITEM_BIG_BUG_NET,                   /* ITEMMDL_BIG_BUG_NET */
    MDL_ITEM_FARON_GRASSHOPPER,             /* ITEMMDL_FARON_GRASSHOPPER */
    MDL_ITEM_WOODLAND_RHINO_BEETLE,         /* ITEMMDL_WOODLAND_RHINO_BEETLE */
    MDL_ITEM_DEKU_HORNET,                   /* ITEMMDL_DEKU_HORNET */
    MDL_ITEM_SKYLOFT_MANTIS,                /* ITEMMDL_SKYLOFT_MANTIS */
    MDL_ITEM_VOLCANIC_LADYBUG,              /* ITEMMDL_VOLCANIC_LADYBUG */
    MDL_ITEM_BLESSED_BUTTERFLY,             /* ITEMMDL_BLESSED_BUTTERFLY */
    MDL_ITEM_LANAYRU_ANT,                   /* ITEMMDL_LANAYRU_ANT */
    MDL_ITEM_SAND_CICADA,                   /* ITEMMDL_SAND_CICADA */
    MDL_ITEM_GERUDO_DRAGONFLY,              /* ITEMMDL_GERUDO_DRAGONFLY */
    MDL_ITEM_ELDIN_ROLLER,                  /* ITEMMDL_ELDIN_ROLLER */
    MDL_ITEM_SKY_STAG_BEETLE,               /* ITEMMDL_SKY_STAG_BEETLE */
    MDL_ITEM_MAX,                           /* ITEMMDL_STARRY_FIREFLY */
    MDL_ITEM_BOTTLE,                        /* ITEMMDL_BOTTLE */
    MDL_ITEM_CAWLIN_LETTER,                 /* ITEMMDL_CAWLIN_LETTER */
    MDL_ITEM_BEEDLE_INSECT_CAGE,            /* ITEMMDL_BEEDLE_INSECT_CAGE */
    MDL_ITEM_MAX,                           /* ITEMMDL_RATTLE */
    MDL_ITEM_HORNET_LARVAE,                 /* ITEMMDL_HORNET_LARVAE */
    MDL_ITEM_BIRD_FEATHER,                  /* ITEMMDL_BIRD_FEATHER */
    MDL_ITEM_TUMBLE_WEED,                   /* ITEMMDL_TUMBLE_WEED */
    MDL_ITEM_LIZARD_TAIL,                   /* ITEMMDL_LIZARD_TAIL */
    MDL_ITEM_ELDIN_ORE,                     /* ITEMMDL_ELDIN_ORE */
    MDL_ITEM_ANCIENT_FLOWER,                /* ITEMMDL_ANCIENT_FLOWER */
    MDL_ITEM_AMBER_RELIC,                   /* ITEMMDL_AMBER_RELIC */
    MDL_ITEM_DUSK_RELIC,                    /* ITEMMDL_DUSK_RELIC */
    MDL_ITEM_MAX,                           /* ITEMMDL_JELLY_BLOB */
    MDL_ITEM_MONSTER_CLAW,                  /* ITEMMDL_MONSTER_CLAW */
    MDL_ITEM_MONSTER_HORN,                  /* ITEMMDL_MONSTER_HORN */
    MDL_ITEM_ORNAMENTAL_SKULL,              /* ITEMMDL_ORNAMENTAL_SKULL */
    MDL_ITEM_EVIL_CRYSTAL,                  /* ITEMMDL_EVIL_CRYSTAL */
    MDL_ITEM_BLUE_BIRD_FEATHER,             /* ITEMMDL_BLUE_BIRD_FEATHER */
    MDL_ITEM_GOLDEN_SKULL,                  /* ITEMMDL_GOLDEN_SKULL */
    MDL_ITEM_GODDESS_PLUME,                 /* ITEMMDL_GODDESS_PLUME */
    MDL_ITEM_EMERALD_TABLET,                /* ITEMMDL_EMERALD_TABLET */
    MDL_ITEM_RUBY_TABLET,                   /* ITEMMDL_RUBY_TABLET */
    MDL_ITEM_AMBER_TABLET,                  /* ITEMMDL_AMBER_TABLET */
    MDL_ITEM_STONE_OF_TRIALS,               /* ITEMMDL_STONE_OF_TRIALS */
    MDL_ITEM_GODDESS_HARP,                  /* ITEMMDL_BALLAD_OF_THE_GODDESS */
    MDL_ITEM_GODDESS_HARP,                  /* ITEMMDL_FARORES_COURAGE */
    MDL_ITEM_GODDESS_HARP,                  /* ITEMMDL_NAYRUS_WISDOM */
    MDL_ITEM_GODDESS_HARP,                  /* ITEMMDL_DINS_POWER */
    MDL_ITEM_GODDESS_HARP,                  /* ITEMMDL_FARON_SOTH */
    MDL_ITEM_GODDESS_HARP,                  /* ITEMMDL_ELDIN_SOTH */
    MDL_ITEM_GODDESS_HARP,                  /* ITEMMDL_LANAYRU_SOTH */
    MDL_ITEM_GODDESS_HARP,                  /* ITEMMDL_ITEM_SOTH */
    MDL_ITEM_REVITALIZING_POTION_PLUS_PLUS, /* ITEMMDL_REVITALIZING_POTION_PLUS_PLUS */
    MDL_ITEM_MAX,                           /* ITEMMDL_HOT_SOUP */
    MDL_ITEM_MAX,                           /* ITEMMDL_COLD_SOUP */
    MDL_ITEM_LIFE_TREE_SEED,                /* ITEMMDL_LIFE_TREE_SEED */
    MDL_ITEM_LIFE_TREE_FRUIT,               /* ITEMMDL_LIFE_TREE_FRUIT */
    MDL_ITEM_EXTRA_WALLET,                  /* ITEMMDL_EXTRA_WALLET */
};

const dItemMdlItem_c::MdlConfig dItemMdlItem_c::sMdlConfig[] = {
    {         "GetTriForce",          "GetTriForce"},
    {            "GetRupee",             "GetRupee"},
    {           "GetSwordA",            "GetSwordA"},
    {            "GetRupee",             "GetRupee"},
    {            "GetRupee",             "GetRupee"},
    {            "GetRupee",             "GetRupee"},
    {            "GetRupee",             "GetRupee"},
    {            "GetRupee",             "GetRupee"},
    {             "GetHarp",              "GetHarp"},
    {      "GetSirenFlower",       "GetSirenFlower"},
    {             "GetBowA",              "GetBowA"},
    {         "GetHookShot",          "GetHookShot"},
    {       "GetBirdStatue",        "GetBirdStatue"},
    {        "GetKeyBoss2A",         "GetKeyBoss2A"},
    {        "GetKeyBoss2B",         "GetKeyBoss2B"},
    {        "GetKeyBoss2C",         "GetKeyBoss2C"},
    {        "GetKeyKakera",                nullptr},
    {         "GetKeyBossA",          "GetKeyBossA"},
    {         "GetKeyBossB",          "GetKeyBossB"},
    {         "GetKeyBossC",          "GetKeyBossC"},
    {    "GetBottleKinokoA",     "GetBottleKinokoA"},
    {             "GetBomb",              "GetBomb"},
    {           "GetVacuum",            "GetVacuum"},
    {              "GetMap",               "GetMap"},
    {        "GetPachinkoA",         "GetPachinkoA"},
    {          "GetBeetleA",           "GetBeetleA"},
    {    "GetBottleKinokoB",     "GetBottleKinokoB"},
    {       "GetMoleGloveA",        "GetMoleGloveA"},
    {            "GetRupee",             "GetRupee"},
    {          "GetSeedSet",           "GetSeedSet"},
    {            "GetUroko",             "GetUroko"},
    {            "GetMedal",       "GetMedalReturn"},
    {             "GetNetA",              "GetNetA"},
    {          "GetBeetleB",           "GetBeetleB"},
    {          "GetBeetleC",           "GetBeetleC"},
    {          "GetBeetleD",           "GetBeetleD"},
    {             "GetBowB",              "GetBowB"},
    {             "GetBowC",              "GetBowC"},
    {          "GetBombBag",           "GetBombBag"},
    {           "GetMapSea",            "GetMapSea"},
    {       "GetMoleGloveB",        "GetMoleGloveB"},
    {            "GetMedal",        "GetMedalHeart"},
    {            "GetMedal",        "GetMedalLucky"},
    {            "GetMedal",       "GetMedalHunter"},
    {            "GetMedal",      "GetMedalDurable"},
    {            "GetMedal",        "GetMedalDemon"},
    {        "GetPachinkoB",         "GetPachinkoB"},
    {            "GetRupee",             "GetRupee"},
    {           "GetPurseB",            "GetPurseB"},
    {           "GetPurseC",            "GetPurseC"},
    {           "GetPurseD",            "GetPurseD"},
    {           "GetPurseE",            "GetPurseE"},
    {           "GetPouchA",            "GetPorchA"},
    {           "GetPouchB",            "GetPorchB"},
    {            "GetMedal",         "GetMedalLife"},
    {      "GetShieldHylia",       "GetShieldHylia"},
    {     "GetBottleRepair",      "GetBottleRepair"},
    {       "GetSpareSeedA",        "GetSpareSeedA"},
    {       "GetSpareSeedB",        "GetSpareSeedB"},
    {       "GetSpareSeedC",        "GetSpareSeedC"},
    {     "GetSpareQuiverA",      "GetSpareQuiverA"},
    {     "GetSpareQuiverB",      "GetSpareQuiverB"},
    {     "GetSpareQuiverC",      "GetSpareQuiverC"},
    {    "GetSpareBombBagA",     "GetSpareBombBagA"},
    {    "GetSpareBombBagB",     "GetSpareBombBagB"},
    {    "GetSpareBombBagC",     "GetSpareBombBagC"},
    {             "GetWhip",              "GetWhip"},
    {          "GetEarring",           "GetEarring"},
    {             "GetNetB",              "GetNetB"},
    {"GetInsectGrasshopper", "GetInsectGrasshopper"},
    {     "GetInsectBeetle",      "GetInsectBeetle"},
    {        "GetInsectBee",         "GetInsectBee"},
    {   "GetInsectKamakiri",    "GetInsectKamakiri"},
    {    "GetInsectLadybug",     "GetInsectLadybug"},
    {  "GetInsectButterfly",   "GetInsectButterfly"},
    {        "GetInsectAnt",         "GetInsectAnt"},
    {     "GetInsectCicada",      "GetInsectCicada"},
    {  "GetInsectDragonfly",   "GetInsectDragonfly"},
    {     "GetInsectScarab",      "GetInsectScarab"},
    {   "GetInsectKuwagata",    "GetInsectKuwagata"},
    {      "GetBottleEmpty",       "GetBottleEmpty"},
    {     "GetKobunALetter",      "GetKobunALetter"},
    {        "GetTerryCage",         "GetTerryCage"},
    {           "GetSozaiA",            "GetSozaiA"},
    {           "GetSozaiB",            "GetSozaiB"},
    {           "GetSozaiC",            "GetSozaiC"},
    {           "GetSozaiD",            "GetSozaiD"},
    {           "GetSozaiE",            "GetSozaiE"},
    {           "GetSozaiF",            "GetSozaiF"},
    {           "GetSozaiG",            "GetSozaiG"},
    {           "GetSozaiH",            "GetSozaiH"},
    {           "GetSozaiJ",            "GetSozaiJ"},
    {           "GetSozaiL",            "GetSozaiL"},
    {           "GetSozaiK",            "GetSozaiK"},
    {           "GetSozaiM",            "GetSozaiM"},
    {           "GetSozaiN",            "GetSozaiN"},
    {           "GetSozaiO",            "GetSozaiO"},
    {           "GetSozaiP",            "GetSozaiP"},
    {      "GetSekibanMapA",          "SekibanMapA"},
    {      "GetSekibanMapB",          "SekibanMapB"},
    {      "GetSekibanMapC",          "SekibanMapC"},
    {         "GetSirenKey",          "GetSirenKey"},
    {    "GetBottleRepairS",     "GetBottleRepairS"},
    {         "GetSeedLife",          "GetSeedLife"},
    {           "GetFruitB",            "GetFruitB"},
    {       "GetSparePurse",        "GetSparePurse"},
};

bool dItemMdlItem_c::isMdlForItemId(u16 itemId) {
    if (hasItemMdl(itemId) == false) {
        return false;
    } else {
        return sMdlIdx[getItemMdlIdx(itemId)] <= MDL_ITEM_MAX - 1;
    }
}

bool dItemMdlItem_c::init(u16 itemId, dAcItem_c *item, mAllocator_c *allocator) {
    const MdlConfig *mdlConfig;
    bool scattershotOverride = dAcItem_c::isSlingshot((ITEM_ID)itemId) && dAcItem_c::checkFlag(ITEM_MIGHTY_SCATTERSHOT);
    if (scattershotOverride) {
        mdlConfig = &sMdlConfig[sMdlIdx[getItemMdlIdx(ITEM_MIGHTY_SCATTERSHOT)]];
    } else {
        mdlConfig = &sMdlConfig[sMdlIdx[getItemMdlIdx(itemId)]];
    }

    void *resData = OarcManager::GetInstance()->getMdlFromArc2(mdlConfig->resFile);
    if (resData == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile res(resData);
    if (!res.IsValid()) {
        return false;
    }

    SizedString<0x20> str;
    if (dAcItem_c::isKeyPiece((ITEM_ID)itemId)) {
        if (dAcItem_c::getKeyPieceCount() < dAcItem_c::getTotalRequiredKeyPieceCount() - 1) {
            str = "GetKeyKakeraA";
        } else {
            str = "GetKeyKakeraB";
        }
    } else {
        str = mdlConfig->mdlName;
    }

    nw4r::g3d::ResMdl mdl = res.GetResMdl(str);
    if (!mdl.IsValid()) {
        return false;
    }
    if (!mMdl.create(mdl, allocator, 0x120)) {
        return false;
    }

    mpItem = item;
    return true;
}

void dItemMdlItem_c::setDrawMode(u8 arg) {
    mDrawMode = arg;
}

void dItemMdlItem_c::setScale(const mVec3_c &scale) {
    mMdl.setScale(scale);
}

void dItemMdlItem_c::setLocalMtx(const mMtx_c &mtx) {
    mMdl.setLocalMtx(mtx);
}

void dItemMdlItem_c::draw() {
    if (mDrawMode == DrawMode_Get) {
        mpItem->fn_8002ECD0(&mMdl, 6);
    } else {
        mpItem->fn_8002ECD0(&mMdl, 7);
    }
}

void dItemMdlItem_c::changeItemId(u16 itemId) {
    // no-op
}

void dItemMdlItem_c::setPriorityDraw() {
    mMdl.setPriorityDraw(0x82, 0x7F);
}

void dItemMdlItem_c::unsetPriorityDraw() {
    mMdl.setPriorityDraw(0x7F, 0x7F);
}

m3d::smdl_c *dItemMdlItem_c::getMdl() {
    return &mMdl;
}
