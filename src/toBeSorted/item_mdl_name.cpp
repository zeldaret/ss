#include "common.h"
#include "toBeSorted/item_mdl_name.h"

static const u16 ITEM_TO_MODEL_INDEX[512] = {
    ITEMMDL_MAX,                           /* ITEM_NONE */
    ITEMMDL_SMALL_KEY,                     /* ITEM_SMALL_KEY */
    ITEMMDL_GREEN_RUPEE,                   /* ITEM_GREEN_RUPEE */
    ITEMMDL_BLUE_RUPEE,                    /* ITEM_BLUE_RUPEE */
    ITEMMDL_RED_RUPEE,                     /* ITEM_RED_RUPEE */
    ITEMMDL_COMPLETE_TRIFORCE,             /* ITEM_COMPLETE_TRIFORCE */
    ITEMMDL_HEART,                         /* ITEM_HEART */
    ITEMMDL_SINGLE_ARROW,                  /* ITEM_SINGLE_ARROW */
    ITEMMDL_BUNDLE_ARROWS,                 /* ITEM_BUNDLE_ARROWS */
    ITEMMDL_GODDESS_WHITESWORD,            /* ITEM_GODDESS_WHITESWORD */
    ITEMMDL_PRACTICE_SWORD,                /* ITEM_PRACTICE_SWORD */
    ITEMMDL_GODDESS_SWORD,                 /* ITEM_GODDESS_SWORD */
    ITEMMDL_GODDESS_LONGSWORD,             /* ITEM_GODDESS_LONGSWORD */
    ITEMMDL_MASTER_SWORD,                  /* ITEM_MASTER_SWORD */
    ITEMMDL_TRUE_MASTER_SWORD,             /* ITEM_TRUE_MASTER_SWORD */
    ITEMMDL_SAILCLOTH,                     /* ITEM_SAILCLOTH */
    ITEMMDL_GODDESS_HARP,                  /* ITEM_GODDESS_HARP */
    ITEMMDL_SPIRIT_VESSEL,                 /* ITEM_SPIRIT_VESSEL */
    ITEMMDL_MAX,                           /* ITEM_UNK_18 */
    ITEMMDL_BOW,                           /* ITEM_BOW */
    ITEMMDL_CLAWSHOWS,                     /* ITEM_CLAWSHOTS */
    ITEMMDL_BIRD_STATUETTE,                /* ITEM_BIRD_STATUETTE */
    ITEMMDL_MAX,                           /* ITEM_COMMON_BUG */
    ITEMMDL_MAX,                           /* ITEM_UNCOMMON_BUG */
    ITEMMDL_MAX,                           /* ITEM_RARE_BUG */
    ITEMMDL_AC_BK,                         /* ITEM_AC_BK */
    ITEMMDL_FS_BK,                         /* ITEM_FS_BK */
    ITEMMDL_SSH_BK,                        /* ITEM_SSH_BK */
    ITEMMDL_KEY_PIECE,                     /* ITEM_KEY_PIECE */
    ITEMMDL_SV_BK,                         /* ITEM_SV_BK */
    ITEMMDL_ET_BK,                         /* ITEM_ET_BK */
    ITEMMDL_LMF_BK,                        /* ITEM_LMF_BK */
    ITEMMDL_SILVER_RUPEE,                   /* ITEM_SILVER_RUPEE */
    ITEMMDL_GOLD_RUPEE,                    /* ITEM_GOLD_RUPEE */
    ITEMMDL_RUPOOR,                        /* ITEM_RUPOOR */
    ITEMMDL_5_CRYSTALS,                    /* ITEM_5_CRYSTALS */
    ITEMMDL_GLITTERING_SPORES,             /* ITEM_GLITTERING_SPORES */
    ITEMMDL_MAX,                           /* ITEM_UNK_37 */
    ITEMMDL_MAX,                           /* ITEM_UNK_38 */
    ITEMMDL_MAX,                           /* ITEM_UNK_39 */
    ITEMMDL_5_BOMBS,                       /* ITEM_5_BOMBS */
    ITEMMDL_10_BOMBS,                      /* ITEM_10_BOMBS */
    ITEMMDL_STAMINA_FRUIT,                 /* ITEM_STAMINA_FRUIT */
    ITEMMDL_FARORE_TEAR,                   /* ITEM_FARORE_TEAR */
    ITEMMDL_DIN_TEAR,                      /* ITEM_DIN_TEAR */
    ITEMMDL_NAYRU_TEAR,                    /* ITEM_NAYRU_TEAR */
    ITEMMDL_SACRED_TEAR,                   /* ITEM_SACRED_TEAR */
    ITEMMDL_LIGHT_FRUIT,                   /* ITEM_LIGHT_FRUIT */
    ITEMMDL_1_CRYSTAL,                     /* ITEM_1_CRYSTAL */
    ITEMMDL_GUST_BELLOWS,                  /* ITEM_GUST_BELLOWS */
    ITEMMDL_DUNGEON_MAP_FI,                /* ITEM_DUNGEON_MAP_FI */
    ITEMMDL_DUNGEON_MAP,                   /* ITEM_DUNGEON_MAP */
    ITEMMDL_SLINGSHOT,                     /* ITEM_SLINGSHOT */
    ITEMMDL_BEETLE,                        /* ITEM_BEETLE */
    ITEMMDL_WATER,                         /* ITEM_WATER */
    ITEMMDL_MUSHROOM_SPORES,               /* ITEM_MUSHROOM_SPORES */
    ITEMMDL_DIGGING_MITTS,                 /* ITEM_DIGGING_MITTS */
    ITEMMDL_5_DEKU_SEEDS,                  /* ITEM_5_DEKU_SEEDS */
    ITEMMDL_MAX,                           /* ITEM_UNK_58 */
    ITEMMDL_MAX,                           /* ITEM_UNK_59 */
    ITEMMDL_10_DEKU_SEEDS,                 /* ITEM_10_DEKU_SEEDS */
    ITEMMDL_MAX,                           /* ITEM_COMMON_TREASURE */
    ITEMMDL_MAX,                           /* ITEM_COMMON_TREASURE2 */
    ITEMMDL_MAX,                           /* ITEM_UNCOMMON_TREASURE */
    ITEMMDL_MAX,                           /* ITEM_RARE_TREASURE */
    ITEMMDL_GUARDIAN_POTION,               /* ITEM_GUARDIAN_POTION */
    ITEMMDL_GUARDIAN_POTION_PLUS,          /* ITEM_GUARDIAN_POTION_PLUS */
    ITEMMDL_MAX,                           /* ITEM_UNK_67 */
    ITEMMDL_WATER_SCALE,                   /* ITEM_WATER_SCALE */
    ITEMMDL_MAX,                           /* ITEM_UNK_69 */
    ITEMMDL_BUG_MEDAL,                     /* ITEM_BUG_MEDAL */
    ITEMMDL_BUG_NET,                       /* ITEM_BUG_NET */
    ITEMMDL_FAIRY,                         /* ITEM_FAIRY */
    ITEMMDL_MAX,                           /* ITEM_UNK_73 */
    ITEMMDL_SACRED_WATER,                  /* ITEM_SACRED_WATER */
    ITEMMDL_HOOK_BEETLE,                   /* ITEM_HOOK_BEETLE */
    ITEMMDL_QUICK_BEETLE,                  /* ITEM_QUICK_BEETLE */
    ITEMMDL_TOUGH_BEETLE,                  /* ITEM_TOUGH_BEETLE */
    ITEMMDL_HEART_POTION,                  /* ITEM_HEART_POTION */
    ITEMMDL_HEART_POTION_PLUS,             /* ITEM_HEART_POTION_PLUS */
    ITEMMDL_MAX,                           /* ITEM_HEART_POTION_ALSO */
    ITEMMDL_HEART_POTION_ALSO_PLUS,        /* ITEM_HEART_POTION_PLUS_PLUS */
    ITEMMDL_MAX,                           /* ITEM_UNK_82 */
    ITEMMDL_MAX,                           /* ITEM_GUARDIAN_POTION_NO_TBOX */
    ITEMMDL_STAMINA_POTION,                /* ITEM_STAMINA_POTION */
    ITEMMDL_STAMINA_POTION_PLUS,           /* ITEM_STAMINA_POTION_PLUS */
    ITEMMDL_AIR_POTION,                    /* ITEM_AIR_POTION */
    ITEMMDL_AIR_POTION_PLUS,               /* ITEM_AIR_POTION_PLUS */
    ITEMMDL_BOTTLED_FAIRY,                 /* ITEM_BOTTLED_FAIRY */
    ITEMMDL_MAX,                           /* ITEM_UNK_89 */
    ITEMMDL_IRON_BOW,                      /* ITEM_IRON_BOW */
    ITEMMDL_SACRED_BOW,                    /* ITEM_SACRED_BOW */
    ITEMMDL_BOMB_BAG,                      /* ITEM_BOMB_BAG */
    ITEMMDL_HEART_CONTAINER,               /* ITEM_HEART_CONTAINER */
    ITEMMDL_HEART_PIECE,                   /* ITEM_HEART_PIECE */
    ITEMMDL_TRI_COURAGE,                   /* ITEM_TRI_COURAGE */
    ITEMMDL_TRI_POWER,                     /* ITEM_TRI_POWER */
    ITEMMDL_TRI_WISDOWM,                   /* ITEM_TRI_WISDOM */
    ITEMMDL_SEA_CHART,                     /* ITEM_SEA_CHART */
    ITEMMDL_MOGMA_MITTS,                   /* ITEM_MOGMA_MITTS */
    ITEMMDL_HEART_MEDAL,                   /* ITEM_HEART_MEDAL */
    ITEMMDL_RUPEE_MEDAL,                   /* ITEM_RUPEE_MEDAL */
    ITEMMDL_TREASURE_MEDAL,                /* ITEM_TREASURE_MEDAL */
    ITEMMDL_POTION_MEDAL,                  /* ITEM_POTION_MEDAL */
    ITEMMDL_CURSED_MEDAL,                  /* ITEM_CURSED_MEDAL */
    ITEMMDL_MIGHTY_SCATTERSHOT,            /* ITEM_MIGHTY_SCATTERSHOT */
    ITEMMDL_MAX,                           /* ITEM_UNK_106 */
    ITEMMDL_85,                            /* ITEM_UNK_107 */
    ITEMMDL_MEDIUM_WALLET,                 /* ITEM_MEDIUM_WALLET */
    ITEMMDL_BIG_WALLET,                    /* ITEM_BIG_WALLET */
    ITEMMDL_GIANT_WALLET,                  /* ITEM_GIANT_WALLET */
    ITEMMDL_TYCOON_WALLET,                 /* ITEM_TYCOON_WALLET */
    ITEMMDL_ADVENTURE_POUCH,               /* ITEM_ADVENTURE_POUCH */
    ITEMMDL_POUCH_EXPANSION,               /* ITEM_POUCH_EXPANSION */
    ITEMMDL_LIFE_MEDAL,                    /* ITEM_LIFE_MEDAL */
    ITEMMDL_MAX,                           /* ITEM_UNK_115 */
    ITEMMDL_WOODEN_SHIELD,                 /* ITEM_WOODEN_SHIELD */
    ITEMMDL_BANDED_SHIELD,                 /* ITEM_BANDED_SHIELD */
    ITEMMDL_BRACED_SHIELD,                 /* ITEM_BRACED_SHIELD */
    ITEMMDL_IRON_SHIELD,                   /* ITEM_IRON_SHIELD */
    ITEMMDL_REINFORCED_SHIELD,             /* ITEM_REINFORCED_SHIELD */
    ITEMMDL_FORTIFIED_SHIELD,              /* ITEM_FORTIFIED_SHIELD */
    ITEMMDL_SACRED_SHIELD,                 /* ITEM_SACRED_SHIELD */
    ITEMMDL_DIVINE_SHIELD,                 /* ITEM_DIVINE_SHIELD */
    ITEMMDL_GODDESS_SHIELD,                /* ITEM_GODDESS_SHIELD */
    ITEMMDL_HYLIAN_SHIELD,                 /* ITEM_HYLIAN_SHIELD */
    ITEMMDL_REVITALIZING_POTION,           /* ITEM_REVITALIZING_POTION */
    ITEMMDL_REVITALIZING_POTION_PLUS,      /* ITEM_REVITALIZING_POTION_PLUS */
    ITEMMDL_SMALL_SEED_SATCHEL,            /* ITEM_SMALL_SEED_SATCHEL */
    ITEMMDL_MEDIUM_SEED_SATCHEL,           /* ITEM_MEDIUM_SEED_SATCHEL */
    ITEMMDL_LARGE_SEED_SATCHEL,            /* ITEM_LARGE_SEED_SATCHEL */
    ITEMMDL_SMALL_QUIVER,                  /* ITEM_SMALL_QUIVER */
    ITEMMDL_MEDIUM_QUIVER,                 /* ITEM_MEDIUM_QUIVER */
    ITEMMDL_LARGE_QUIVER,                  /* ITEM_LARGE_QUIVER */
    ITEMMDL_SMALL_BOMB_BAG,                /* ITEM_SMALL_BOMB_BAG */
    ITEMMDL_MEDIUM_BOMB_BAG,               /* ITEM_MEDIUM_BOMB_BAG */
    ITEMMDL_LARGE_BOMB_BAG,                /* ITEM_LARGE_BOMB_BAG */
    ITEMMDL_WHIP,                          /* ITEM_WHIP */
    ITEMMDL_FIRESHIELD_EARRINGS,           /* ITEM_FIRESHIELD_EARRINGS */
    ITEMMDL_MAX,                           /* ITEM_UNK_139 */
    ITEMMDL_BIG_BUG_NET,                   /* ITEM_BIG_BUG_NET */
    ITEMMDL_FARON_GRASSHOPPER,             /* ITEM_FARON_GRASSHOPPER */
    ITEMMDL_WOODLAND_RHINO_BEETLE,         /* ITEM_WOODLAND_RHINO_BEETLE */
    ITEMMDL_DEKU_HORNET,                   /* ITEM_DEKU_HORNET */
    ITEMMDL_SKYLOFT_MANTIS,                /* ITEM_SKYLOFT_MANTIS */
    ITEMMDL_VOLCANIC_LADYBUG,              /* ITEM_VOLCANIC_LADYBUG */
    ITEMMDL_BLESSED_BUTTERFLY,             /* ITEM_BLESSED_BUTTERFLY */
    ITEMMDL_LANAYRU_ANT,                   /* ITEM_LANAYRU_ANT */
    ITEMMDL_SAND_CICADA,                   /* ITEM_SAND_CICADA */
    ITEMMDL_GERUDO_DRAGONFLY,              /* ITEM_GERUDO_DRAGONFLY */
    ITEMMDL_ELDIN_ROLLER,                  /* ITEM_ELDIN_ROLLER */
    ITEMMDL_SKY_STAG_BEETLE,               /* ITEM_SKY_STAG_BEETLE */
    ITEMMDL_STARRY_FIREFLY,                /* ITEM_STARRY_FIREFLY */
    ITEMMDL_BOTTLE,                        /* ITEM_BOTTLE */
    ITEMMDL_MAX,                           /* ITEM_RUPEE_MEDAL_ALSO */
    ITEMMDL_MAX,                           /* ITEM_HEART_MEDAL_ALSO */
    ITEMMDL_MAX,                           /* ITEM_UNK_156 */
    ITEMMDL_MAX,                           /* ITEM_UNK_157 */
    ITEMMDL_CAWLIN_LETTER,                 /* ITEM_CAWLIN_LETTER */
    ITEMMDL_BEEDLE_INSECT_CAGE,            /* ITEM_BEEDLE_INSECT_CAGE */
    ITEMMDL_RATTLE,                        /* ITEM_RATTLE */
    ITEMMDL_HORNET_LARVAE,                 /* ITEM_HORNET_LARVAE */
    ITEMMDL_BIRD_FEATHER,                  /* ITEM_BIRD_FEATHER */
    ITEMMDL_TUMBLE_WEED,                   /* ITEM_TUMBLE_WEED */
    ITEMMDL_LIZARD_TAIL,                   /* ITEM_LIZARD_TAIL */
    ITEMMDL_ELDIN_ORE,                     /* ITEM_ELDIN_ORE */
    ITEMMDL_ANCIENT_FLOWER,                /* ITEM_ANCIENT_FLOWER */
    ITEMMDL_AMBER_RELIC,                   /* ITEM_AMBER_RELIC */
    ITEMMDL_DUSK_RELIC,                    /* ITEM_DUSK_RELIC */
    ITEMMDL_JELLY_BLOB,                    /* ITEM_JELLY_BLOB */
    ITEMMDL_MONSTER_CLAW,                  /* ITEM_MONSTER_CLAW */
    ITEMMDL_MONSTER_HORN,                  /* ITEM_MONSTER_HORN */
    ITEMMDL_ORNAMENTAL_SKULL,              /* ITEM_ORNAMENTAL_SKULL */
    ITEMMDL_EVIL_CRYSTAL,                  /* ITEM_EVIL_CRYSTAL */
    ITEMMDL_BLUE_BIRD_FEATHER,             /* ITEM_BLUE_BIRD_FEATHER */
    ITEMMDL_GOLDEN_SKULL,                  /* ITEM_GOLDEN_SKULL */
    ITEMMDL_GODDESS_PLUME,                 /* ITEM_GODDESS_PLUME */
    ITEMMDL_EMERALD_TABLET,                /* ITEM_EMERALD_TABLET */
    ITEMMDL_RUBY_TABLET,                   /* ITEM_RUBY_TABLET */
    ITEMMDL_AMBER_TABLET,                  /* ITEM_AMBER_TABLET */
    ITEMMDL_STONE_OF_TRIALS,               /* ITEM_STONE_OF_TRIALS */
    ITEMMDL_MAX,                           /* ITEM_UNK_181 */
    ITEMMDL_MAX,                           /* ITEM_UNK_182 */
    ITEMMDL_MAX,                           /* ITEM_UNK_183 */
    ITEMMDL_MAX,                           /* ITEM_UNK_184 */
    ITEMMDL_MAX,                           /* ITEM_UNK_185 */
    ITEMMDL_BALLAD_OF_THE_GODDESS,         /* ITEM_BALLAD_OF_THE_GODDESS */
    ITEMMDL_FARORES_COURAGE,               /* ITEM_FARORES_COURAGE */
    ITEMMDL_NAYRUS_WISDOM,                 /* ITEM_NAYRUS_WISDOM */
    ITEMMDL_DINS_POWER,                    /* ITEM_DINS_POWER */
    ITEMMDL_FARON_SOTH,                    /* ITEM_FARON_SOTH */
    ITEMMDL_ELDIN_SOTH,                    /* ITEM_ELDIN_SOTH */
    ITEMMDL_LANAYRU_SOTH,                  /* ITEM_LANAYRU_SOTH */
    ITEMMDL_ITEM_SOTH,                     /* ITEM_SOTH */
    ITEMMDL_REVITALIZING_POTION_PLUS_PLUS, /* ITEM_REVITALIZING_POTION_PLUS_PLUS */
    ITEMMDL_HOT_SOUP,                      /* ITEM_HOT_SOUP */
    ITEMMDL_COLD_SOUP,                     /* ITEM_COLD_SOUP */
    ITEMMDL_LIFE_TREE_SEED,                /* ITEM_LIFE_TREE_SEED */
    ITEMMDL_LIFE_TREE_FRUIT,               /* ITEM_LIFE_TREE_FRUIT */
    ITEMMDL_EXTRA_WALLET,                  /* ITEM_EXTRA_WALLET */
    ITEMMDL_MAX,                           /* ITEM_UNK_200 */
    ITEMMDL_MAX,                           /* ITEM_LIFE_MEDAL_AGAIN */
    ITEMMDL_MAX,                           /* ITEM_POUCH_EXPANSION_GODDESS_CHEST */
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
    ITEMMDL_MAX,
};

bool hasItemMdl(u16 itemId) {
    if (itemId >= ARRAY_LENGTH(ITEM_TO_MODEL_INDEX)) {
        return false;
    }
    return ITEM_TO_MODEL_INDEX[itemId] <= ITEMMDL_MAX - 1;
}

u16 getItemMdlIdx(u16 itemId) {
    return ITEM_TO_MODEL_INDEX[itemId];
}
