#ifndef D_FLAG_MAP_H
#define D_FLAG_MAP_H

// Main Reference Point:
//      https://github.com/lepelog/skywardsword-tools/blob/master/storyflags.txt
// Story Flag json File:
//      https://github.com/lepelog/skywardsword-tools/blob/master/allstoryflags.json
// Story Flag Sheet:
//      https://docs.google.com/spreadsheets/d/1vkchZ4Ui2gr7oTdQlkoaq115hQ6qhSeFAxCqonwdYhE/edit?usp=sharing
// NOTE: Please notate/edit flags description as they are used. Flag sheet may be more updated and not reflected here.

// IWYU pragma: private, include "d/flag/storyflag_manager.h"
enum StoryFlags_e {
    /** [-]
     *  Story Flag #0 (0x0000) - JP - / US -
     */
    STORYFLAG_0,

    /** [-]
     *  Story Flag #1 (0x0001) - JP - / US -
     */
    STORYFLAG_1,

    /** [-]
     *  Story Flag #2 (0x0002) - JP - / US -
     */
    STORYFLAG_2,

    /** [Goddess Sword acquired]
     *  Story Flag #3 (0x0003) - JP 805ACD59 0x10 / US 805A9AD9 0x10
     */
    STORYFLAG_GODDESS_SWORD,

    /** [Entering Skyview Cutscene (Skyview layer 1)]
     *  Story Flag #4 (0x0004) - JP 805ACD59 0x20 / US 805A9AD9 0x20
     */
    STORYFLAG_4,

    /** [Beat Skyview (Faron Woods, Deep Woods layer 2 / Eldin Volcano, Digging Mitts Cave, FS Entrance, Volcano Summit
     * Waterfall, Thrill Digger layer 1 / Skyview Spring layer 2 / Skyview Boss Room layer 2, ET Boss Room Layer 1)]
     *  Story Flag #5 (0x0005) - JP 805ACD59 0x40 / US 805A9AD9 0x40
     */
    STORYFLAG_5,

    /** [Entering Earth Temple Cutscene]
     *  Story Flag #6 (0x0006) - JP 805ACD59 0x80 / US 805A9AD9 0x80
     */
    STORYFLAG_6,

    /** [Beat Earth Temple (ET Boss Room Layer 2)]
     *  Story Flag #7 (0x0007) - JP 805ACD58 0x01 / US 805A9AD8 0x01
     */
    STORYFLAG_7,

    /** [LMF rising Cutscene]
     *  Story Flag #8 (0x0008) - JP 805ACD58 0x02 / US 805A9AD8 0x02
     */
    STORYFLAG_LMF_RAISED,

    /** [Harp Cutscene (Lanayru Desert layer 2 / Lanayru Mines layer 2 / Temple of Time layer 2 / Behind The Temple
     * layer 3)] Story Flag #9 (0x0009) - JP 805ACD58 0x04 / US 805A9AD8 0x04
     */
    STORYFLAG_HARP_CUTSCENE,

    /** [Entering Ancient Cistern Cutscene]
     *  Story Flag #10 (0x000A) - JP 805ACD58 0x08 / US 805A9AD8 0x08
     */
    STORYFLAG_10,

    /** [Beat Ancient Cistern (Racetracks appear in the Sealed Grounds) (Skyview layer 3 / Outside AC layer 3 / Inside
     * Great Tree layer 2 / Lake Floria layer 2)] Story Flag #11 (0x000B) - JP 805ACD58 0x10 / US 805A9AD8 0x10
     */
    STORYFLAG_11,

    /** [Entering Fire Sanctuary Cutscene]
     *  Story Flag #12 (0x000C) - JP 805ACD58 0x20 / US 805A9AD8 0x20
     */
    STORYFLAG_12,

    /** [Beat Fire Sanctuary (Sealed Temple layer 3 / Sealed Grounds layer 3 / Behind The Temple layer 4 / FS Boss Room
     * layer 2 /  FS Entrance layer 4)] Story Flag #13 (0x000D) - JP 805ACD58 0x40 / US 805A9AD8 0x40
     */
    STORYFLAG_13,

    /** [Entering Sandship Cutscene]
     *  Story Flag #14 (0x000E) - JP 805ACD58 0x80 / US 805A9AD8 0x80
     */
    STORYFLAG_SANDSHIP_ENTERED,

    /** [Beat Sandship (Sand Sea layer 3)]
     *  Story Flag #15 (0x000F) - JP 805ACD5B 0x01 / US 805A9ADB 0x01
     */
    STORYFLAG_15,

    /** [Water Dragon teaches SotH part (Faron Woods, Deep Woods layer 5 / Inside Great Tree layer 3 / Lake Floria layer
     * 3 / Water Dragon's Lair layer 3 / Behind The Temple layer 7)] Story Flag #16 (0x0010) - JP 805ACD5B 0x02 / US
     * 805A9ADB 0x02
     */
    STORYFLAG_WATER_DRAGON_SOTH_PART,

    /** [Triforce Completed]
     *  Story Flag #17 (0x0011) - JP 805ACD5B 0x04 / US 805A9ADB 0x04
     */
    STORYFLAG_TRIFORCE_COMPLETE,

    /** [All Tadtones collected]
     *  Story Flag #18 (0x0012) - JP 805ACD59 0x08 / US 805A9AD9 0x08
     */
    STORYFLAG_ALL_TADTONES_GATHERED,

    /** [Fire Dragon Text after SotH part Cutscene (Eldin Volcano, Digging Mitts Cave, Volcano Summit Waterfall layer
     * 4)] Story Flag #19 (0x0013) - JP 805ACD5B 0x08 / US 805A9ADB 0x08
     */
    STORYFLAG_BOKO_BASE_BEATEN,

    /** [Set when learning 3rd SotH part (Groosenator to Lanayru ready)]
     *  Story Flag #20 (0x0014) - JP 805ACD5B 0x10 / US 805A9ADB 0x10
     */
    STORYFLAG_20,

    /** [Thunder Dragon teaches SotH part (Lanayru Caves layer 4 / Lanayru Mines layer 4)]
     *  Story Flag #21 (0x0015) - JP 805ACD5B 0x20 / US 805A9ADB 0x20
     */
    STORYFLAG_THUNDER_DRAGON_SOTH_COMPLETED,

    /** [Place Stone of Trials in Statue / Sky Keep appears (Also gives Stone of Trials in Inventory) (Skyloft layers
     * 6&7)] 6&7)] Story Flag #22 (0x0016) - JP 805ACD5B 0x40 / US 805A9ADB 0x40
     */
    STORYFLAG_STONE_OF_TRIALS_PLACED,

    /** [Meeting Zelda Cutscene]
     *  Story Flag #23 (0x0017) - JP 805ACD5B 0x80 / US 805A9ADB 0x80
     */
    STORYFLAG_23,

    /** [Fi cutscene around Faron light pillar before landing for first time (also set by Sealed Grounds intro
     * cutscene)] Story Flag #24 (0x0018) - JP 805ACD5A 0x01 / US 805A9ADA 0x01
     */
    STORYFLAG_24,

    /** [Zelda joins you before rescuing Loftwing]
     *  Story Flag #25 (0x0019) - JP 805ACD5A 0x02 / US 805A9ADA 0x02
     */
    STORYFLAG_25,

    /** [Beat Sky Keep (Goddess Statue has fallen on Skyloft) (Skyloft layers 8&9)]
     *  Story Flag #26 (0x001A) - JP 805ACD5A 0x04 / US 805A9ADA 0x04
     */
    STORYFLAG_26,

    /** [Loftwing rescued (Unlocks Loftwing) (Skyloft layer 3 / Sky layer 3)]
     *  Story Flag #27 (0x001B) - JP 805ACD5A 0x08 / US 805A9ADA 0x08
     */
    STORYFLAG_27,

    /** [Set After Training Sword Textbox (allows the sheikah stone cutscene to play)]
     *  Story Flag #28 (0x001C) - JP 805ACD5A 0x10 / US 805A9ADA 0x10
     */
    STORYFLAG_28,

    /** [Meeting Groose Cutscene (Skyloft layer 10)]
     *  Story Flag #29 (0x001D) - JP 805ACD5A 0x20 / US 805A9ADA 0x20
     */
    STORYFLAG_29,

    /** [Adventure Pouch unlocked]
     *  Story Flag #30 (0x001E) - JP 805ACD5A 0x40 / US 805A9ADA 0x40
     */
    STORYFLAG_POUCH_UNLOCKED,

    /** [Wing Ceremony Cutscene Part 2 (Sky layer 1)]
     *  Story Flag #31 (0x001F) - JP 805ACD5A 0x80 / US 805A9ADA 0x80
     */
    STORYFLAG_31,

    /** [Set landing in the circle after getting sailcloth (Makes Fi visible during the Fi Chase, required for scene
     * flags to be set in Link's room)] Story Flag #32 (0x0020) - JP 805ACD5D 0x01 / US 805A9ADD 0x01
     */
    STORYFLAG_32,

    /** [Finish Talking with Pipit and Fledge near Sparring Hall]
     *  Story Flag #33 (0x0021) - JP 805ACD5D 0x02 / US 805A9ADD 0x02
     */
    STORYFLAG_33,

    /** [Cutscene first time seeing Goddess Sword]
     *  Story Flag #34 (0x0022) - JP 805ACD5D 0x04 / US 805A9ADD 0x04
     */
    STORYFLAG_34,

    /** [Get Potion Lady Bottle]
     *  Story Flag #35 (0x0023) - JP 805ACD5D 0x08 / US 805A9ADD 0x08
     */
    STORYFLAG_35,

    /** [Hero's Tunic / Fi Unlocked if you have a sword (Skyloft layers 4&5 / Sky layer 2 / Knight Academy layers 3&4 /
     * Sealed Grounds layer 1 / Behind The Temple layer 1 / Goddess Statue layers 2&3)] Story Flag #36 (0x0024) - JP
     * 805ACD5D 0x10 / US 805A9ADD 0x10
     */
    STORYFLAG_HEROS_TUNIC_FI,

    /** [Horwell Z-target tutorial]
     *  Story Flag #37 (0x0025) - JP 805ACD65 0x08 / US 805A9AE5 0x08
     */
    STORYFLAG_37,

    /** [Cutscene after acquiring Goddess Sword]
     *  Story Flag #38 (0x0026) - JP 805ACD65 0x10 / US 805A9AE5 0x10
     */
    STORYFLAG_38,

    /** [Wing Ceremony completed]
     *  Story Flag #39 (0x0027) - JP 805ACD65 0x20 / US 805A9AE5 0x20
     */
    STORYFLAG_39,

    /** [Set after landing in the circle after getting sailcloth]
     *  Story Flag #40 (0x0028) - JP 805ACD65 0x40 / US 805A9AE5 0x40
     */
    STORYFLAG_40,

    /** [Wing Ceremony Cutscene Part 1]
     *  Story Flag #41 (0x0029) - JP 805ACD61 0x80 / US 805A9AE1 0x80
     */
    STORYFLAG_41,

    /** [Fun Fun Minigame explained]
     *  Story Flag #42 (0x002A) - JP 805ACD67 0x08 / US 805A9AE7 0x08
     */
    STORYFLAG_42,

    /** [First time jumping on a loftwing animation (set before jumping on loftwing for first time before loftwing
     * tutorial, unset after)] tutorial, unset after)] Story Flag #43 (0x002B) - JP 805ACD67 0x10 / US 805A9AE7 0x10
     */
    STORYFLAG_FIRST_TIME_LOFTWING_ANM,

    /** [Fi leaves Knight Academy during Fi Chase]
     *  Story Flag #44 (0x002C) - JP 805ACD67 0x20 / US 805A9AE7 0x20
     */
    STORYFLAG_44,

    /** [First time asking advice to Fortune Teller]
     *  Story Flag #45 (0x002D) - JP 805ACD67 0x40 / US 805A9AE7 0x40
     */
    STORYFLAG_45,

    /** [Faron light pillar]
     *  Story Flag #46 (0x002E) - JP 805ACD67 0x80 / US 805A9AE7 0x80
     */
    STORYFLAG_FARON_PILLAR_OPENED,

    /** [Eldin light pillar]
     *  Story Flag #47 (0x002F) - JP 805ACD66 0x01 / US 805A9AE6 0x01
     */
    STORYFLAG_ELDIN_PILLAR_OPENED,

    /** [Lanayru light pillar (Eldin Volcano, Digging Mitts Cave, FS Entrance, Volcano Summit Waterfall layer 2 /
     * Lanayru Mines layer 1 / Temple of Time layer 1)] Story Flag #48 (0x0030) - JP 805ACD66 0x02 / US 805A9AE6 0x02
     */
    STORYFLAG_LANAYRU_PILLAR_OPENED,

    /** [Talk to Headmaster to learn about Windmills]
     *  Story Flag #49 (0x0031) - JP 805ACD66 0x04 / US 805A9AE6 0x04
     */
    STORYFLAG_49,

    /** [Machi saved from bokoblins]
     *  Story Flag #50 (0x0032) - JP 805ACD5D 0x20 / US 805A9ADD 0x20
     */
    STORYFLAG_50,

    /** [Bonk 2nd mushroom during Machi Chase]
     *  Story Flag #51 (0x0033) - JP 805ACD5D 0x40 / US 805A9ADD 0x40
     */
    STORYFLAG_51,

    /** [Machi located (Kikwi Elder appears) (also sets after Skyview)]
     *  Story Flag #52 (0x0034) - JP 805ACD5D 0x80 / US 805A9ADD 0x80
     */
    STORYFLAG_52,

    /** [Oolo located]
     *  Story Flag #53 (0x0035) - JP 805ACD5C 0x01 / US 805A9ADC 0x01
     */
    STORYFLAG_53,

    /** [Lopsa located]
     *  Story Flag #54 (0x0036) - JP 805ACD5C 0x02 / US 805A9ADC 0x02
     */
    STORYFLAG_54,

    /** [Erla located (unset after Tadtones) (sets when locating him again)]
     *  Story Flag #55 (0x0037) - JP 805ACD5C 0x04 / US 805A9ADC 0x04
     */
    STORYFLAG_55,

    /** [Kikwi Elder located (Kikwi Dowsing)]
     *  Story Flag #56 (0x0038) - JP 805ACD5C 0x08 / US 805A9ADC 0x08
     */
    STORYFLAG_KIKWI_DOWSING,

    /** [Talk to Kikwi Elder after locating every Kikwi (slingshot appears)]
     *  Story Flag #57 (0x0039) - JP 805ACD5C 0x10 / US 805A9ADC 0x10
     */
    STORYFLAG_KIKWI_FIND_FINISHED,

    /** [B-Wheel unlocked (not slingshot)]
     *  Story Flag #58 (0x003A) - JP 805ACD5C 0x20 / US 805A9ADC 0x20
     */
    STORYFLAG_B_WHEEL_UNLOCKED,

    /** [Machi scared to see Link after bokoblin fight]
     *  Story Flag #59 (0x003B) - JP 805ACD5F 0x04 / US 805A9ADF 0x04
     */
    STORYFLAG_59,

    /** [Cutscene on top of Great Tree]
     *  Story Flag #60 (0x003C) - JP 805ACD5C 0x80 / US 805A9ADC 0x80
     */
    STORYFLAG_60,

    /** [Talking to Yerbal again after hearing him out about the Lake Floria Gates]
     *  Story Flag #61 (0x003D) - JP 805ACD5F 0x01 / US 805A9ADF 0x01
     */
    STORYFLAG_61,

    /** [Getting the Lake Floria Gate Quest explained a second time from the Kikwi Ermit Yerbal]
     *  Story Flag #62 (0x003E) - JP 805ACD5F 0x02 / US 805A9ADF 0x02
     */
    STORYFLAG_62,

    /** [Talking to Kikwi Ermit Yerbal]
     *  Story Flag #63 (0x003F) - JP 805ACD5C 0x40 / US 805A9ADC 0x40
     */
    STORYFLAG_63,

    /** [Set when Water Dragon asks for Sacred Water (Sacred Water dowsing)]
     *  Story Flag #64 (0x0040) - JP 805ACD5F 0x08 / US 805A9ADF 0x08
     */
    STORYFLAG_SACRED_WATER_DOWSING,

    /** [Set when healing Water Dragon (Skyview Spring layer 4)]
     *  Story Flag #65 (0x0041) - JP 805ACD5F 0x10 / US 805A9ADF 0x10
     */
    STORYFLAG_WATER_DRAGON_HEALED,

    /** [Set after Water Dragon dispells Waterfall]
     *  Story Flag #66 (0x0042) - JP 805ACD5F 0x20 / US 805A9ADF 0x20
     */
    STORYFLAG_66,

    /** [Triple Stalfos Boss defeated in Skyview 2 (Skyview Boss Room layer 4)]
     *  Story Flag #67 (0x0043) - JP 805ACD5F 0x40 / US 805A9ADF 0x40
     */
    STORYFLAG_67,

    /** [[Sets in Faron Woods map, probably Great Tree]]
     *  Story Flag #68 (0x0044) - JP 805ACD5F 0x80 / US 805A9ADF 0x80
     */
    STORYFLAG_68,

    /** [Machi Cutscene after Skyview]
     *  Story Flag #69 (0x0045) - JP 805ACD5E 0x01 / US 805A9ADE 0x01
     */
    STORYFLAG_69,

    /** [Light Fruit cutscene]
     *  Story Flag #70 (0x0046) - JP 805ACD60 0x40 / US 805A9AE0 0x40
     */
    STORYFLAG_70,

    /** [Learn Farore's Courage (Faron Woods layer 3 / Deep Woods layer 3)]
     *  Story Flag #71 (0x0047) - JP 805ACD65 0x80 / US 805A9AE5 0x80
     */
    STORYFLAG_71,

    /** [Learn Nayru's Wisdom]
     *  Story Flag #72 (0x0048) - JP 805ACD69 0x20 / US 805A9AE9 0x20
     */
    STORYFLAG_72,

    /** [Learn Din's Power]
     *  Story Flag #73 (0x0049) - JP 805ACD69 0x40 / US 805A9AE9 0x40
     */
    STORYFLAG_73,

    /** [Talk to Parella near Great Tree Bottom]
     *  Story Flag #74 (0x004A) - JP 805ACD68 0x08 / US 805A9AE8 0x08
     */
    STORYFLAG_74,

    /** [Talk to Parella near machi chase start]
     *  Story Flag #75 (0x004B) - JP 805ACD68 0x10 / US 805A9AE8 0x10
     */
    STORYFLAG_75,

    /** [Set after talking to Parella near Kikwi Elder before colecting all Tadtones]
     *  Story Flag #76 (0x004C) - JP 805ACD68 0x20 / US 805A9AE8 0x20
     */
    STORYFLAG_76,

    /** [Talk to Parella near Faron Woods Entry statue]
     *  Story Flag #77 (0x004D) - JP 805ACD68 0x40 / US 805A9AE8 0x40
     */
    STORYFLAG_77,

    /** [Talk to Parella near Boulder with tadtone in it]
     *  Story Flag #78 (0x004E) - JP 805ACD68 0x80 / US 805A9AE8 0x80
     */
    STORYFLAG_78,

    /** [Talk to Parella near In The Woods statue]
     *  Story Flag #79 (0x004F) - JP 805ACD6B 0x01 / US 805A9AEB 0x01
     */
    STORYFLAG_79,

    /** [Water Dragon Cutscene in Flooded Faron Woods starting Tadtones watched + tadtones appear (Flooded Great Tree
     * layer 2)] Story Flag #80 (0x0050) - JP 805ACD6B 0x02 / US 805A9AEB 0x02
     */
    STORYFLAG_80,

    /** [Tadtone Dowsing]
     *  Story Flag #81 (0x0051) - JP 805ACD6B 0x04 / US 805A9AEB 0x04
     */
    STORYFLAG_TADTONE_DOWSING,

    /** [Music Scroll for Tadtones (sets before the collecting, unsets when collecting is done)]
     *  Story Flag #82 (0x0052) - JP 805ACD6B 0x08 / US 805A9AEB 0x08
     */
    STORYFLAG_82,

    /** [Ghirahim 1 defeated (Skyview Spring layer 1 / Skyview Boss Room layer 1)]
     *  Story Flag #83 (0x0053) - JP 805ACD69 0x80 / US 805A9AE9 0x80
     */
    STORYFLAG_83,

    /** [Ghirahim 2 defeated (FS Boss Room layer 1)]
     *  Story Flag #84 (0x0054) - JP 805ACD68 0x01 / US 805A9AE8 0x01
     */
    STORYFLAG_84,

    /** [Set when healing Water Dragon]
     *  Story Flag #85 (0x0055) - JP 805ACD68 0x02 / US 805A9AE8 0x02
     */
    STORYFLAG_85,

    /** [Set going back to the sky]
     *  Story Flag #86 (0x0056) - JP 805ACD68 0x04 / US 805A9AE8 0x04
     */
    STORYFLAG_86,

    /** [Faron Woods introductory cutscene (Behind The Temple layer 2)]
     *  Story Flag #87 (0x0057) - JP 805ACD6C 0x02 / US 805A9AEC 0x02
     */
    STORYFLAG_FARON_WOODS_DISCOVERED,

    /** [Lake Floria Introductory Cutscene]
     *  Story Flag #88 (0x0058) - JP 805ACD6C 0x04 / US 805A9AEC 0x04
     */
    STORYFLAG_LAKE_FLORIA_DISCOVERED,

    /** [Skyview Introductory cutscene]
     *  Story Flag #89 (0x0059) - JP 805ACD6C 0x08 / US 805A9AEC 0x08
     */
    STORYFLAG_SKYVIEW_DISCOVERED,

    /** [Sets after the Robot upgrades your Beetle]
     *  Story Flag #90 (0x005A) - JP 805ACD6F 0x80 / US 805A9AEF 0x80
     */
    STORYFLAG_90,

    /** [Deep Woods Introductory cutscene]
     *  Story Flag #91 (0x005B) - JP 805ACD71 0x01 / US 805A9AF1 0x01
     */
    STORYFLAG_DEEP_WOODS_DISCOVERED,

    /** [Lanayru Desert Introductory Cutscene]
     *  Story Flag #92 (0x005C) - JP 805ACD71 0x02 / US 805A9AF1 0x02
     */
    STORYFLAG_LANAYRU_DESERT_DISCOVERED,

    /** [Faron's Trial Gate Fi Singing Cutscene]
     *  Story Flag #93 (0x005D) - JP 805ACD71 0x04 / US 805A9AF1 0x04
     */
    STORYFLAG_FARON_TRIAL_UNLOCKED,

    /** [Faron Silent Realm Introductory Cutscene]
     *  Story Flag #94 (0x005E) - JP 805ACD71 0x08 / US 805A9AF1 0x08
     */
    STORYFLAG_94,

    /** []
     *  Story Flag #95 (0x005F) - JP 805ACD71 0x10 / US 805A9AF1 0x10
     */
    STORYFLAG_95,

    /** [Sets after optaining a map the first time]
     *  Story Flag #96 (0x0060) - JP 805ACD71 0x20 / US 805A9AF1 0x20
     */
    STORYFLAG_96,

    /** [Lanayru Trial Gate Fi Singing Cutscene]
     *  Story Flag #97 (0x0061) - JP 805ACD71 0x40 / US 805A9AF1 0x40
     */
    STORYFLAG_LANAYRU_TRIAL_UNLOCKED,

    /** [Eldin Trial Gate Fi Singing Cutscene]
     *  Story Flag #98 (0x0062) - JP 805ACD71 0x80 / US 805A9AF1 0x80
     */
    STORYFLAG_ELDIN_TRIAL_UNLOCKED,

    /** [Hylia's Trial Gate Fi Singing Cutscene]
     *  Story Flag #99 (0x0063) - JP 805ACD70 0x01 / US 805A9AF0 0x01
     */
    STORYFLAG_HYLIA_TRIAL_UNLOCKED,

    /** [Zelda Dowsing (unset by fi text after harp) (set after 5Cx10 sets)]
     *  Story Flag #100 (0x0064) - JP 805ACD5E 0x02 / US 805A9ADE 0x02
     */
    STORYFLAG_ZELDA_DOWSING,

    /** []
     *  Story Flag #101 (0x0065) - JP 805ACD5E 0x04 / US 805A9ADE 0x04
     */
    STORYFLAG_101,

    /** [Treasure Dowsing]
     *  Story Flag #102 (0x0066) - JP 805ACD5E 0x08 / US 805A9ADE 0x08
     */
    STORYFLAG_TREASURE_DOWSING,

    /** []
     *  Story Flag #103 (0x0067) - JP 805ACD5E 0x10 / US 805A9ADE 0x10
     */
    STORYFLAG_103,

    /** [Crystal Dowsing]
     *  Story Flag #104 (0x0068) - JP 805ACD5E 0x20 / US 805A9ADE 0x20
     */
    STORYFLAG_CRYSTAL_DOWSING,

    /** [Rupee Dowsing]
     *  Story Flag #105 (0x0069) - JP 805ACD5E 0x40 / US 805A9ADE 0x40
     */
    STORYFLAG_RUPEE_DOWSING,

    /** [Key Piece dowsing]
     * Story Flag #106 (0x006A) - JP 805ACD5E 0x80 / US 805A9ADE 0x80
     */
    STORYFLAG_KEYPIECE_DOWSING,

    /** [Generator Dowsing in Lanayru]
     *  Story Flag #107 (0x006B) - JP 805ACD61 0x01 / US 805A9AE1 0x01
     */
    STORYFLAG_GENERATOR_DOWSING,

    /** [Heart Dowsing]
     *  Story Flag #108 (0x006C) - JP 805ACD61 0x02 / US 805A9AE1 0x02
     */
    STORYFLAG_HEART_DOWSING,

    /** [Sets in Lanayru Desert]
     *  Story Flag #109 (0x006D) - JP 805ACD61 0x04 / US 805A9AE1 0x04
     */
    STORYFLAG_109,

    /** [Goddess Cube Dowsing]
     *  Story Flag #110 (0x006E) - JP 805ACD61 0x08 / US 805A9AE1 0x08
     */
    STORYFLAG_GODDESS_CUBE_DOWSING,

    /** [Breaking first Barricade in Lake Floria]
     *  Story Flag #111 (0x006F) - JP 805ACD6B 0x10 / US 805A9AEB 0x10
     */
    STORYFLAG_111,

    /** [CS after blowing up big boulder to continue]
     *  Story Flag #112 (0x0070) - JP 805ACD6B 0x20 / US 805A9AEB 0x20
     */
    STORYFLAG_112,

    /** [Cutscene after Spin-Jumping over first ledge in Lake Floria]
     *  Story Flag #113 (0x0071) - JP 805ACD6B 0x40 / US 805A9AEB 0x40
     */
    STORYFLAG_113,

    /** [Defeat Cranioc in Lake Floria]
     *  Story Flag #114 (0x0072) - JP 805ACD6B 0x80 / US 805A9AEB 0x80
     */
    STORYFLAG_114,

    /** [New map part in Lake Floria discovered]
     *  Story Flag #115 (0x0073) - JP 805ACD6A 0x01 / US 805A9AEA 0x01
     */
    STORYFLAG_FLORIA_INTERIOR_MAP_UNLOCKED,

    /** [Finished following Parella]
     *  Story Flag #116 (0x0074) - JP 805ACD6A 0x02 / US 805A9AEA 0x02
     */
    STORYFLAG_116,

    /** [Cranioc Cutscene in Lake Floria]
     *  Story Flag #117 (0x0075) - JP 805ACD6A 0x04 / US 805A9AEA 0x04
     */
    STORYFLAG_117,

    /** [Water Dragon Text after beating AC]
     *  Story Flag #118 (0x0076) - JP 805ACD6A 0x08 / US 805A9AEA 0x08
     */
    STORYFLAG_118,

    /** [Set when you should respawn at a statue, unset when not?]
     *  Story Flag #119 (0x0077) - JP 805ACD6A 0x10 / US 805A9AEA 0x10
     */
    STORYFLAG_RESPAWN_AT_STATUE,

    /** [Colecting all key pieces]
     *  Story Flag #120 (0x0078) - JP 805ACD6A 0x20 / US 805A9AEA 0x20
     */
    STORYFLAG_KEYPIECE_COMPLETE,

    /** [Fi Text About Faron Trial Dowsing (triggered by Farore's Courage flag)]
     *  Story Flag #121 (0x0079) - JP 805ACD6A 0x40 / US 805A9AEA 0x40
     */
    STORYFLAG_121,

    /** [[Is set on a Hero Mode file]]
     *  Story Flag #122 (0x007A) - JP 805ACD6A 0x80 / US 805A9AEA 0x80
     */
    STORYFLAG_DEKU_HORNET_COLLECTED,

    /** []
     *  Story Flag #123 (0x007B) - JP 805ACD6D 0x01 / US 805A9AED 0x01
     */
    STORYFLAG_123,

    /** []
     *  Story Flag #124 (0x007C) - JP 805ACD6D 0x02 / US 805A9AED 0x02
     */
    STORYFLAG_124,

    /** []
     *  Story Flag #125 (0x007D) - JP 805ACD6D 0x04 / US 805A9AED 0x04
     */
    STORYFLAG_125,

    /** []
     *  Story Flag #126 (0x007E) - JP 805ACD6D 0x08 / US 805A9AED 0x08
     */
    STORYFLAG_126,

    /** [[untriggers one Item Seller object]]
     *  Story Flag #127 (0x007F) - JP 805ACD6D 0x10 / US 805A9AED 0x10
     */
    STORYFLAG_127,

    /** []
     *  Story Flag #128 (0x0080) - JP 805ACD6D 0x20 / US 805A9AED 0x20
     */
    STORYFLAG_128,

    /** [Sets when Robot \"fixes\" your Map of Lanayru Desert]
     *  Story Flag #129 (0x0081) - JP 805ACD6D 0x40 / US 805A9AED 0x40
     */
    STORYFLAG_129,

    /** [Meeting Impa cutscene]
     *  Story Flag #130 (0x0082) - JP 805ACD64 0x01 / US 805A9AE4 0x01
     */
    STORYFLAG_IMPA_MET,

    /** [Imp 1 defeated (Skyloft layers 4&5)]
     *  Story Flag #131 (0x0083) - JP 805ACD64 0x02 / US 805A9AE4 0x02
     */
    STORYFLAG_IMPRISONED_1_DEFEATED,

    /** [Imp 2 defeated (Skyloft layers 4&5 / Sealed Temple layer 4)]
     *  Story Flag #132 (0x0084) - JP 805ACD64 0x04 / US 805A9AE4 0x04
     */
    STORYFLAG_IMPRISONED_2_DEFEATED,

    /** [Imp 3 defeated (Sealed Temple layer 6 / Sealed Grounds layer 5 / Behind The Temple layer 6)]
     *  Story Flag #133 (0x0085) - JP 805ACD64 0x08 / US 805A9AE4 0x08
     */
    STORYFLAG_IMPRISONED_3_DEFEATED,

    /** [Horde defeated (Sealed Grounds layer 6 / Hylia's Realm layer 7)]
     *  Story Flag #134 (0x0086) - JP 805ACD64 0x10 / US 805A9AE4 0x10
     */
    STORYFLAG_HORDE_DEFEATED,

    /** [Groose meets Impa for first time cutscene (Starts Harp Tutorial)]
     *  Story Flag #135 (0x0087) - JP 805ACD64 0x20 / US 805A9AE4 0x20
     */
    STORYFLAG_135,

    /** [Imp fight has started (Fi trigger around Sealed Grounds Statue and loading zone to Behind the Temple)]
     *  Story Flag #136 (0x0088) - JP 805ACD64 0x40 / US 805A9AE4 0x40
     */
    STORYFLAG_IMPRISONED_FIGHT,

    /** [Sealed Grounds Introductory cutscene (or Fire Sanctuary outside cutscene) (Trigger for Faron Statue Selection
     * Screen) (Faron Woods, Deep Woods, Inside Great Tree layer 1)] Story Flag #137 (0x0089) - JP 805ACD64 0x80 / US
     * 805A9AE4 0x80
     */
    STORYFLAG_FARON_DISCOVERED,

    /** [Cutscene near sealed temple entrance first time in sealed grounds (Sealed Temple layer 1)]
     *  Story Flag #138 (0x008A) - JP 805ACD67 0x01 / US 805A9AE7 0x01
     */
    STORYFLAG_138,

    /** []
     *  Story Flag #139 (0x008B) - JP 805ACD67 0x02 / US 805A9AE7 0x02
     */
    STORYFLAG_139,

    /** [Groose lands in Faron after Harp (Sealed Temple layer 2)]
     *  Story Flag #140 (0x008C) - JP 805ACD67 0x04 / US 805A9AE7 0x04
     */
    STORYFLAG_GROOSE_LANDS_IN_FARON,

    /** [Ballad of the Goddess played with Impa (Sealed Grounds layer 2)]
     *  Story Flag #141 (0x008D) - JP 805ACD6C 0x01 / US 805A9AEC 0x01
     */
    STORYFLAG_141,

    /** [Talk to FI after beating Imp 1 (or Talk to Headmaster to learn about Windmills)]
     *  Story Flag #142 (0x008E) - JP 805ACD6E 0x01 / US 805A9AEE 0x01
     */
    STORYFLAG_142,

    /** [Imp 1 fight triggered (set at end of Groose scared cutscene outside)]
     *  Story Flag #143 (0x008F) - JP 805ACD6E 0x02 / US 805A9AEE 0x02
     */
    STORYFLAG_IMPRISONED_1_FIGHT,

    /** [Imp 2 fight triggered (set when Groose opens door inside temple)]
     *  Story Flag #144 (0x0090) - JP 805ACD6E 0x04 / US 805A9AEE 0x04
     */
    STORYFLAG_IMPRISONED_2_FIGHT,

    /** [Imp 3 fight triggered (set when Groose calls you entering Sealed Grounds)]
     *  Story Flag #145 (0x0091) - JP 805ACD6E 0x08 / US 805A9AEE 0x08
     */
    STORYFLAG_IMPRISONED_3_FIGHT,

    /** [Cutscene going back through the Gate of Time in Sealed Temple]
     *  Story Flag #146 (0x0092) - JP 805ACD6E 0x10 / US 805A9AEE 0x10
     */
    STORYFLAG_146,

    /** [Panning cutscene showing beacon]
     *  Story Flag #147 (0x0093) - JP 805ACD6E 0x20 / US 805A9AEE 0x20
     */
    STORYFLAG_147,

    /** [Link looking around cutscene first time entering Sealed Temple (Required for Bokoblins to attack Goron Behind
     * the Temple)] Story Flag #148 (0x0094) - JP 805ACD6E 0x40 / US 805A9AEE 0x40
     */
    STORYFLAG_148,

    /** [Post Imp 1 defeat cutscene]
     *  Story Flag #149 (0x0095) - JP 805ACD6E 0x80 / US 805A9AEE 0x80
     */
    STORYFLAG_149,

    /** [Talk to Peter on Bamboo Island for the first time]
     *  Story Flag #150 (0x0096) - JP 805ACD61 0x10 / US 805A9AE1 0x10
     */
    STORYFLAG_150,

    /** [(not implemented yet -- 7F05)]
     *  Story Flag #151 (0x0097) - JP from 805ACE57 0x01 to 805ACE57 0x20 / US from 805A9BD7 0x01 to 805A9BD7 0x20
     */
    STORYFLAG_151,

    /** [Hit Skipper's boat timeshift stone (boat usable in the Sand Sea)]
     *  Story Flag #152 (0x0098) - JP 805ACD61 0x40 / US 805A9AE1 0x40
     */
    STORYFLAG_152,

    /** [Finish Bamboo game with 20 cut or more / Finish Bamboo game after beating FS ]
     *  Story Flag #153 (0x0099) - JP 805ACD61 0x20 / US 805A9AE1 0x20
     */
    STORYFLAG_153,

    /** [Triggers freeing the robots Text (unset outside Sandship)]
     *  Story Flag #154 (0x009A) - JP 805ACD60 0x01 / US 805A9AE0 0x01
     */
    STORYFLAG_154,

    /** [Ancient Cistern Introductory Cutscene (lilypad outside AC appears) (Outside AC layer 2)]
     *  Story Flag #155 (0x009B) - JP 805ACD60 0x02 / US 805A9AE0 0x02
     */
    STORYFLAG_ANCIENT_CISTERN_DISCOVERED,

    /** [Sandship Introductory Cutscene]
     *  Story Flag #156 (0x009C) - JP 805ACD60 0x04 / US 805A9AE0 0x04
     */
    STORYFLAG_SANDSHIP_DISCOVERED,

    /** [Ancient Harbour Introductory Cutscene]
     *  Story Flag #157 (0x009D) - JP 805ACD60 0x08 / US 805A9AE0 0x08
     */
    STORYFLAG_ANCIENT_HARBOR_DISCOVERED,

    /** [First Time in Sand Sea]
     *  Story Flag #158 (0x009E) - JP 805ACD60 0x10 / US 805A9AE0 0x10
     */
    STORYFLAG_158,

    /** [Sky Keep Introductory Cutscene]
     *  Story Flag #159 (0x009F) - JP 805ACD60 0x20 / US 805A9AE0 0x20
     */
    STORYFLAG_SKY_KEEP_DISCOVERED,

    /** [Mogma Mitts restricted by Bokoblin Base]
     *  Story Flag #160 (0x00A0) - JP 805ACD60 0x80 / US 805A9AE0 0x80
     */
    STORYFLAG_160,

    /** [Gust Bellows restricted by Bokoblin Base]
     *  Story Flag #161 (0x00A1) - JP 805ACD63 0x01 / US 805A9AE3 0x01
     */
    STORYFLAG_161,

    /** [Clawshots restricted by Bokoblin Base]
     *  Story Flag #162 (0x00A2) - JP 805ACD63 0x02 / US 805A9AE3 0x02
     */
    STORYFLAG_162,

    /** [Whip restricted by Bokoblin Base]
     *  Story Flag #163 (0x00A3) - JP 805ACD63 0x04 / US 805A9AE3 0x04
     */
    STORYFLAG_163,

    /** [Slingshot restricted by Bokoblin Base]
     *  Story Flag #164 (0x00A4) - JP 805ACD63 0x08 / US 805A9AE3 0x08
     */
    STORYFLAG_164,

    /** [Bombs restricted by Bokoblin Base]
     *  Story Flag #165 (0x00A5) - JP 805ACD63 0x10 / US 805A9AE3 0x10
     */
    STORYFLAG_165,

    /** [Bow / Beetle / Bug Net / Pouch restricted by Bokoblin Base]
     *  Story Flag #166 (0x00A6) - JP 805ACD63 0x20 / US 805A9AE3 0x20
     */
    STORYFLAG_166,

    /** [Swordless due to Bokoblin Base]
     *  Story Flag #167 (0x00A7) - JP 805ACD63 0x40 / US 805A9AE3 0x40
     */
    STORYFLAG_SWORD_RESTRICTED,

    /** [Bokoblin Base Item Map from Plats]
     *  Story Flag #168 (0x00A8) - JP 805ACD63 0x80 / US 805A9AE3 0x80
     */
    STORYFLAG_168,

    /** [Eldin Eruption Cutscene ready to play (set when Levias explains SotH quest, set and unset by Faron and Lanayru
     * SotH)] Story Flag #169 (0x00A9) - JP 805ACD62 0x01 / US 805A9AE2 0x01
     */
    STORYFLAG_ELDIN_VOLCANO_CAN_ERUPT,

    /** [Fire Dragon teaches SotH part Cutscene (Fire Dragon's Lair layer 2)]
     *  Story Flag #170 (0x00AA) - JP 805ACD62 0x02 / US 805A9AE2 0x02
     */
    STORYFLAG_170,

    /** [Cutscene before entering Waterfall Cave before rescuing Loftwing]
     *  Story Flag #171 (0x00AB) - JP 805ACD62 0x04 / US 805A9AE2 0x04
     */
    STORYFLAG_171,

    /** [Fire Sanctuary Introductory Cutscene (unsets after you defeated Ghirahim 2) (Fire Sanctuary B layer 1)]
     *  Story Flag #172 (0x00AC) - JP 805ACD62 0x08 / US 805A9AE2 0x08
     */
    STORYFLAG_172,

    /** [Fire Sanctuary Introductory Cutscene]
     *  Story Flag #173 (0x00AD) - JP 805ACD62 0x10 / US 805A9AE2 0x10
     */
    STORYFLAG_FIRE_SANCTUARY_DISCOVERED,

    /** []
     *  Story Flag #174 (0x00AE) - JP 805ACD62 0x20 / US 805A9AE2 0x20
     */
    STORYFLAG_174,

    /** [Set when Goron talks about Lanayru SotH, unset when healing Thunder Dragon (Sealed Temple layer 6 / Sealed
     * Grounds layer 3)] Story Flag #175 (0x00AF) - JP 805ACD62 0x40 / US 805A9AE2 0x40
     */
    STORYFLAG_175,

    /** [Set when Groose talks to Link before Imp 3, unset when Water Dragon teaches SotH part (Lanayru Caves layer 3 /
     * Lanayru Mines layer 3)] Story Flag #176 (0x00B0) - JP 805ACD62 0x80 / US 805A9AE2 0x80
     */
    STORYFLAG_176,

    /** [Plats gives back Mogma Mitts in Bokoblin Base cutscene]
     *  Story Flag #177 (0x00B1) - JP 805ACD65 0x01 / US 805A9AE5 0x01
     */
    STORYFLAG_177,

    /** [Cutscene showing off Bokoblin Base with Plats]
     *  Story Flag #178 (0x00B2) - JP 805ACD65 0x02 / US 805A9AE5 0x02
     */
    STORYFLAG_178,

    /** [(Fire Sanctuary A layer 1)]
     *  Story Flag #179 (0x00B3) - JP 805ACD65 0x04 / US 805A9AE5 0x04
     */
    STORYFLAG_179,

    /** [Talking to Thrill Digger Mogma for the first time]
     *  Story Flag #180 (0x00B4) - JP 805ACD66 0x08 / US 805A9AE6 0x08
     */
    STORYFLAG_180,

    /** [If you fail really hard at Thrill digger minigame]
     *  Story Flag #181 (0x00B5) - JP 805ACD66 0x10 / US 805A9AE6 0x10
     */
    STORYFLAG_181,

    /** [Eldin Volcano Introductory Cutscene (Eldin Layer 1 only) (if set it activates Statue select screen on entering
     * Eldin Pillar)] Story Flag #182 (0x00B6) - JP 805ACD66 0x20 / US 805A9AE6 0x20
     */
    STORYFLAG_ELDIN_DISCOVERED,

    /** [Earth Temple Introductory Cutscene]
     *  Story Flag #183 (0x00B7) - JP 805ACD66 0x40 / US 805A9AE6 0x40
     */
    STORYFLAG_EARTH_TEMPLE_DISCOVERED,

    /** [Impa Cutscene at Bridge in Eldin]
     *  Story Flag #184 (0x00B8) - JP 805ACD66 0x80 / US 805A9AE6 0x80
     */
    STORYFLAG_184,

    /** [Finishing Thrill Digger minigame for the first time?]
     *  Story Flag #185 (0x00B9) - JP 805ACD69 0x01 / US 805A9AE9 0x01
     */
    STORYFLAG_185,

    /** [Water Basin collected (unsets when scrapper picks up anything else)]
     *  Story Flag #186 (0x00BA) - JP 805ACD69 0x02 / US 805A9AE9 0x02
     */
    STORYFLAG_SCRAPPER_HAS_WATER_BASIN,

    /** [Big Frog Watered]
     *  Story Flag #187 (0x00BB) - JP 805ACD69 0x04 / US 805A9AE9 0x04
     */
    STORYFLAG_SUMMIT_BIG_FROG_WATERED,

    /** [Asking Fi about the Water Basin after finding the Big Flames in fromt of FS]
     *  Story Flag #188 (0x00BC) - JP 805ACD69 0x08 / US 805A9AE9 0x08
     */
    STORYFLAG_188,

    /** [Sets at end of Scaldera cs]
     *  Story Flag #189 (0x00BD) - JP 805ACD69 0x10 / US 805A9AE9 0x10
     */
    STORYFLAG_189,

    /** [Lanayru Mines Introductory Cutscene (Lanayru Desert layer 1)]
     *  Story Flag #190 (0x00BE) - JP 805ACD6D 0x80 / US 805A9AED 0x80
     */
    STORYFLAG_LANAYRU_DISCOVERED,

    /** [(not implemented yet -- 7F62)]
     *  Story Flag #191 (0x00BF) - JP from 805ACE57 0x40 to 805ACE56 0x01 / US from 805A9BD7 0x40 to 805A9BD6 0x01
     */
    STORYFLAG_191,

    /** [(not implemented yet -- 7E0F)]
     *  Story Flag #192 (0x00C0) - JP from 805ACE55 0x01 to 805ACE54 0x80 / US from 805A9BD5 0x01 to 805A9BD4 0x80
     */
    STORYFLAG_192,

    /** [(not implemented yet -- 7D0F)]
     *  Story Flag #193 (0x00C1) - JP from 805ACE53 0x01 to 805ACE52 0x80 / US from 805A9BD3 0x01 to 805A9BD2 0x80
     */
    STORYFLAG_193,

    /** [Thunderhead opened (needs scene flag 2x20 to trigger cutscene)]
     *  Story Flag #194 (0x00C2) - JP 805ACD6C 0x10 / US 805A9AEC 0x10
     */
    STORYFLAG_194,

    /** [Fi Text after Opening the Thunderhead]
     *  Story Flag #195 (0x00C3) - JP 805ACD6C 0x20 / US 805A9AEC 0x20
     */
    STORYFLAG_195,

    /** [Fledge stops you after talking to him after Zelda]
     *  Story Flag #196 (0x00C4) - JP 805ACD6C 0x40 / US 805A9AEC 0x40
     */
    STORYFLAG_196,

    /** [Box Cutscene during Remlit Quest]
     *  Story Flag #197 (0x00C5) - JP 805ACD6C 0x80 / US 805A9AEC 0x80
     */
    STORYFLAG_197,

    /** [Unlocks ability to jump off loftwing (set by Fi text before going to sky for first time or Fi Text before Faron
     * light pillar)] Story Flag #198 (0x00C6) - JP 805ACD6F 0x01 / US 805A9AEF 0x01
     */
    STORYFLAG_198,

    /** [Sword Instructor talks to Link before he attempts to leave the Sparring Hall]
     *  Story Flag #199 (0x00C7) - JP 805ACD6F 0x02 / US 805A9AEF 0x02
     */
    STORYFLAG_199,

    /** [Levias explains SotH quest (The Thunder Dragon can appear) (Sealed Temple layer 5 / Sealed Grounds layer 4 /
     * Behind The Temple layer 5 / Thunderhead layer 2 / Lanayru Gorge layer 1 / Lanayru Caves layer 2 / Water Dragon's
     * Lair layer 2 / Flooded Great Tree layer 1)] Story Flag #200 (0x00C8) - JP 805ACD6F 0x04 / US 805A9AEF 0x04
     */
    STORYFLAG_LEVIAS_FIGHT_DEFEATED,

    /** [Talk to Strich at Bug Heaven for first time]
     *  Story Flag #201 (0x00C9) - JP 805ACD6F 0x08 / US 805A9AEF 0x08
     */
    STORYFLAG_201,

    /** [Fi text before going to sky for first time cleared]
     *  Story Flag #202 (0x00CA) - JP 805ACD6F 0x10 / US 805A9AEF 0x10
     */
    STORYFLAG_202,

    /** [Cutscene showing Fi in Academy Staircase]
     *  Story Flag #203 (0x00CB) - JP 805ACD6F 0x20 / US 805A9AEF 0x20
     */
    STORYFLAG_203,

    /** [Skyloft Introductory Cutscene]
     *  Story Flag #204 (0x00CC) - JP 805ACD6F 0x40 / US 805A9AEF 0x40
     */
    STORYFLAG_204,

    /** [Lanayru Silent Realm Introductory Cutscene]
     *  Story Flag #205 (0x00CD) - JP 805ACD70 0x02 / US 805A9AF0 0x02
     */
    STORYFLAG_205,

    /** [Faron Trial Completed (Skyview layer 2 / Outside AC layer 1 / Faron Woods, Deep Woods layer 4 / Skyview Spring
     * layer 3 / Skyview Boss Room layer 3 / Lake Floria layer 1 / Water Dragon's Lair layer 1)] Story Flag #206
     * (0x00CE) - JP 805ACD70 0x04 / US 805A9AF0 0x04
     */
    STORYFLAG_206,

    /** [Eldin Trial Completed (removes fi trigger in front of Volcano Summit)]
     *  Story Flag #207 (0x00CF) - JP 805ACD70 0x08 / US 805A9AF0 0x08
     */
    STORYFLAG_207,

    /** [Lanayru Trial Completed (Lanayru Caves layer 1)]
     *  Story Flag #208 (0x00D0) - JP 805ACD70 0x10 / US 805A9AF0 0x10
     */
    STORYFLAG_208,

    /** [Hylia's Trial Completed (Triggers Fi Text after Skyloft Trial)]
     *  Story Flag #209 (0x00D1) - JP 805ACD70 0x20 / US 805A9AF0 0x20
     */
    STORYFLAG_209,

    /** [Obtain Stone of Trials]
     *  Story Flag #210 (0x00D2) - JP 805ACD70 0x40 / US 805A9AF0 0x40
     */
    STORYFLAG_210,

    /** [LMF Introductory Cutscene (The loading zone to LMF now exists AND the opened Lanayru Gate of Time now appears
     * in the Harp Cutscene)] Story Flag #211 (0x00D3) - JP 805ACD70 0x80 / US 805A9AF0 0x80
     */
    STORYFLAG_LMF_DISCOVERED,

    /** []
     *  Story Flag #212 (0x00D4) - JP 805ACD73 0x01 / US 805A9AF3 0x01
     */
    STORYFLAG_212,

    /** [Start Bamboo Minigame (unsets after you finished) (Bamboo Island layer 2)]
     *  Story Flag #213 (0x00D5) - JP 805ACD73 0x02 / US 805A9AF3 0x02
     */
    STORYFLAG_213,

    /** [Finished Bamboo Minigame (unsets after parrows Text)]
     *  Story Flag #214 (0x00D6) - JP 805ACD73 0x04 / US 805A9AF3 0x04
     */
    STORYFLAG_214,

    /** [(not implemented yet -- 7F95)]
     *  Story Flag #215 (0x00D7) - JP from 805ACE56 0x02 to 805ACE56 0x40 / US from 805A9BD6 0x02 to 805A9BD6 0x40
     */
    STORYFLAG_215,

    /** [Talk to Peater on Bamboo island]
     *  Story Flag #216 (0x00D8) - JP 805ACD73 0x08 / US 805A9AF3 0x08
     */
    STORYFLAG_216,

    /** [(not implemented yet -- 7C03)]
     *  Story Flag #217 (0x00D9) - JP from 805ACE51 0x01 to 805ACE51 0x08 / US from 805A9BD1 0x01 to 805A9BD1 0x08
     */
    STORYFLAG_217,

    /** [Set going back to the sky from Faron, unset entering a light pillar]
     *  Story Flag #218 (0x00DA) - JP 805ACD73 0x10 / US 805A9AF3 0x10
     */
    STORYFLAG_218,

    /** [Set going back to the sky from Eldin, unset entering a light pillar]
     *  Story Flag #219 (0x00DB) - JP 805ACD73 0x20 / US 805A9AF3 0x20
     */
    STORYFLAG_219,

    /** [Set going to the sky from Lanayru, unset entering a light pillar]
     *  Story Flag #220 (0x00DC) - JP 805ACD73 0x40 / US 805A9AF3 0x40
     */
    STORYFLAG_220,

    /** []
     *  Story Flag #221 (0x00DD) - JP 805ACD73 0x80 / US 805A9AF3 0x80
     */
    STORYFLAG_221,

    /** []
     *  Story Flag #222 (0x00DE) - JP 805ACD72 0x01 / US 805A9AF2 0x01
     */
    STORYFLAG_222,

    /** []
     *  Story Flag #223 (0x00DF) - JP 805ACD72 0x02 / US 805A9AF2 0x02
     */
    STORYFLAG_223,

    /** []
     *  Story Flag #224 (0x00E0) - JP 805ACD72 0x04 / US 805A9AF2 0x04
     */
    STORYFLAG_224,

    /** [Set after Ghirahim 3]
     *  Story Flag #225 (0x00E1) - JP 805ACD72 0x10 / US 805A9AF2 0x10
     */
    STORYFLAG_225,

    /** [Hero Mode]
     *  Story Flag #226 (0x00E2) - JP 805ACD72 0x08 / US 805A9AF2 0x08
     */
    STORYFLAG_HERO_MODE,

    /** [First Goddess Cube (Unlocks ability to skyward strike Goddess Cubes)]
     *  Story Flag #227 (0x00E3) - JP 805ACD72 0x20 / US 805A9AF2 0x20
     */
    STORYFLAG_FIRST_GODDESS_CUBE,

    /** [Goddess Cube near Skyview Entrance]
     *  Story Flag #228 (0x00E4) - JP 805ACD72 0x40 / US 805A9AF2 0x40
     */
    STORYFLAG_228,

    /** [Goddess Cube near Volcano Entry Statue]
     *  Story Flag #229 (0x00E5) - JP 805ACD72 0x80 / US 805A9AF2 0x80
     */
    STORYFLAG_229,

    /** [Goddess Cube in Sand Oasis in Lanayru]
     *  Story Flag #230 (0x00E6) - JP 805ACD75 0x01 / US 805A9AF5 0x01
     */
    STORYFLAG_230,

    /** [Goddess Cube near clawshot target in Faron Woods]
     *  Story Flag #231 (0x00E7) - JP 805ACD75 0x02 / US 805A9AF5 0x02
     */
    STORYFLAG_231,

    /** []
     *  Story Flag #232 (0x00E8) - JP 805ACD75 0x04 / US 805A9AF5 0x04
     */
    STORYFLAG_232,

    /** []
     *  Story Flag #233 (0x00E9) - JP 805ACD75 0x08 / US 805A9AF5 0x08
     */
    STORYFLAG_233,

    /** [Goddess Cube near Digging Mitts Cave entrance]
     *  Story Flag #234 (0x00EA) - JP 805ACD75 0x10 / US 805A9AF5 0x10
     */
    STORYFLAG_234,

    /** [Goddess Cube in Lanayru Mines behind Tower]
     *  Story Flag #235 (0x00EB) - JP 805ACD75 0x20 / US 805A9AF5 0x20
     */
    STORYFLAG_235,

    /** [Goddess Cube near Faron Trial]
     *  Story Flag #236 (0x00EC) - JP 805ACD75 0x40 / US 805A9AF5 0x40
     */
    STORYFLAG_236,

    /** [Goddess Cube near Eldin Temple Entrance]
     *  Story Flag #237 (0x00ED) - JP 805ACD75 0x80 / US 805A9AF5 0x80
     */
    STORYFLAG_237,

    /** [Goddess Cube in Skipper's Retreat]
     *  Story Flag #238 (0x00EE) - JP 805ACD74 0x01 / US 805A9AF4 0x01
     */
    STORYFLAG_238,

    /** [Goddess Cube near Life Tree in Lanayru Gorge]
     *  Story Flag #239 (0x00EF) - JP 805ACD74 0x02 / US 805A9AF4 0x02
     */
    STORYFLAG_239,

    /** [Goddess Cube inside Volcano Summit]
     *  Story Flag #240 (0x00F0) - JP 805ACD74 0x04 / US 805A9AF4 0x04
     */
    STORYFLAG_240,

    /** [Goddess Cube on Branch in Faron]
     *  Story Flag #241 (0x00F1) - JP 805ACD74 0x08 / US 805A9AF4 0x08
     */
    STORYFLAG_241,

    /** [Goddess Cube on Sand Slide]
     *  Story Flag #242 (0x00F2) - JP 805ACD74 0x10 / US 805A9AF4 0x10
     */
    STORYFLAG_242,

    /** [Goddess Cube in Pirate Stronghold]
     *  Story Flag #243 (0x00F3) - JP 805ACD74 0x20 / US 805A9AF4 0x20
     */
    STORYFLAG_243,

    /** [Goddess Cube on top of Skyview Temple]
     *  Story Flag #244 (0x00F4) - JP 805ACD74 0x40 / US 805A9AF4 0x40
     */
    STORYFLAG_244,

    /** [Goddess Cube behind boulder near Eldin Temple Entrance]
     *  Story Flag #245 (0x00F5) - JP 805ACD74 0x80 / US 805A9AF4 0x80
     */
    STORYFLAG_245,

    /** [Goddess Cube on clawshot target ledge in Lanayru Desert]
     *  Story Flag #246 (0x00F6) - JP 805ACD77 0x01 / US 805A9AF7 0x01
     */
    STORYFLAG_246,

    /** [Goddess Cube near Hook Beetle cage]
     *  Story Flag #247 (0x00F7) - JP 805ACD77 0x02 / US 805A9AF7 0x02
     */
    STORYFLAG_247,

    /** [Goddess Cube near Fire Sanctuary Entrance]
     *  Story Flag #248 (0x00F8) - JP 805ACD77 0x04 / US 805A9AF7 0x04
     */
    STORYFLAG_248,

    /** [Goddess Cube outside Ancient Cistern]
     *  Story Flag #249 (0x00F9) - JP 805ACD77 0x08 / US 805A9AF7 0x08
     */
    STORYFLAG_249,

    /** [Goddess Cube in Skyview Spring]
     *  Story Flag #250 (0x00FA) - JP 805ACD77 0x10 / US 805A9AF7 0x10
     */
    STORYFLAG_250,

    /** [Goddess Cube in Waterfall Area in Volcano Summit]
     *  Story Flag #251 (0x00FB) - JP 805ACD77 0x20 / US 805A9AF7 0x20
     */
    STORYFLAG_251,

    /** [Goddess Cube in ToT area Minekart Ride in Lanayru]
     *  Story Flag #252 (0x00FC) - JP 805ACD77 0x40 / US 805A9AF7 0x40
     */
    STORYFLAG_252,

    /** []
     *  Story Flag #253 (0x00FD) - JP 805ACD77 0x80 / US 805A9AF7 0x80
     */
    STORYFLAG_253,

    /** [Goddess Cube in Lake Floria]
     *  Story Flag #254 (0x00FE) - JP 805ACD76 0x01 / US 805A9AF6 0x01
     */
    STORYFLAG_254,

    /** [Goddess Cube inside Digging Mitts Cave]
     *  Story Flag #255 (0x00FF) - JP 805ACD76 0x02 / US 805A9AF6 0x02
     */
    STORYFLAG_255,

    /** [Goddess Cube in Ancient Harbour]
     *  Story Flag #256 (0x0100) - JP 805ACD76 0x04 / US 805A9AF6 0x04
     */
    STORYFLAG_256,

    /** []
     *  Story Flag #257 (0x0101) - JP 805ACD76 0x08 / US 805A9AF6 0x08
     */
    STORYFLAG_257,

    /** []
     *  Story Flag #258 (0x0102) - JP 805ACD76 0x10 / US 805A9AF6 0x10
     */
    STORYFLAG_258,

    /** [Talking to the confined robots after activateing both Generators in Sandship]
     *  Story Flag #259 (0x0103) - JP 805ACD76 0x20 / US 805A9AF6 0x20
     */
    STORYFLAG_259,

    /** []
     *  Story Flag #260 (0x0104) - JP 805ACD76 0x40 / US 805A9AF6 0x40
     */
    STORYFLAG_260,

    /** []
     *  Story Flag #261 (0x0105) - JP 805ACD76 0x80 / US 805A9AF6 0x80
     */
    STORYFLAG_261,

    /** [Set when saveing the confined robots in Sandship]
     *  Story Flag #262 (0x0106) - JP 805ACD79 0x01 / US 805A9AF9 0x01
     */
    STORYFLAG_262,

    /** [Set when talking to the confined robots after activateing one Generator]
     *  Story Flag #263 (0x0107) - JP 805ACD79 0x02 / US 805A9AF9 0x02
     */
    STORYFLAG_263,

    /** [Talk to Skipper for first time]
     *  Story Flag #264 (0x0108) - JP 805ACD79 0x04 / US 805A9AF9 0x04
     */
    STORYFLAG_264,

    /** [Shipyard Introductory Cutscene (Sand Sea layer 4)]
     *  Story Flag #265 (0x0109) - JP 805ACD79 0x08 / US 805A9AF9 0x08
     */
    STORYFLAG_265,

    /** [Skipper's Retreat Introductory Cutscene]
     *  Story Flag #266 (0x010A) - JP 805ACD79 0x10 / US 805A9AF9 0x10
     */
    STORYFLAG_266,

    /** [Outside Pirate Stronghold Introductory Cutscene]
     *  Story Flag #267 (0x010B) - JP 805ACD79 0x20 / US 805A9AF9 0x20
     */
    STORYFLAG_267,

    /** [Skipper's Quest (to the Pirate Stronghold) in the Sand Sea]
     *  Story Flag #268 (0x010C) - JP 805ACD79 0x40 / US 805A9AF9 0x40
     */
    STORYFLAG_268,

    /** [Sea Chart in Skipper's Retreat obtained]
     *  Story Flag #269 (0x010D) - JP 805ACD79 0x80 / US 805A9AF9 0x80
     */
    STORYFLAG_269,

    /** [Fi Text after Molderach 2's defeat (Shipyard Construction Bay layer 2)]
     *  Story Flag #270 (0x010E) - JP 805ACD78 0x01 / US 805A9AF8 0x01
     */
    STORYFLAG_270,

    /** [Ship Dowsing (activates invisable Ship)]
     *  Story Flag #271 (0x010F) - JP 805ACD78 0x02 / US 805A9AF8 0x02
     */
    STORYFLAG_SANDSHIP_DOWSING,

    /** [Sandship made visible (Sand Sea layer 5)]
     *  Story Flag #272 (0x0110) - JP 805ACD78 0x04 / US 805A9AF8 0x04
     */
    STORYFLAG_272,

    /** [Skipper takes you onto his boat after talking to him in habour]
     *  Story Flag #273 (0x0111) - JP 805ACD78 0x08 / US 805A9AF8 0x08
     */
    STORYFLAG_273,

    /** [Set when talking to the Goron for the Minecart Minigame after Molderach 2 is defeated]
     *  Story Flag #274 (0x0112) - JP 805ACD78 0x10 / US 805A9AF8 0x10
     */
    STORYFLAG_274,

    /** [Set at Fi text for Ship Dowsing]
     *  Story Flag #275 (0x0113) - JP 805ACD78 0x20 / US 805A9AF8 0x20
     */
    STORYFLAG_275,

    /** [[Sets in Cs in Ancient Habour]]
     *  Story Flag #276 (0x0114) - JP 805ACD78 0x40 / US 805A9AF8 0x40
     */
    STORYFLAG_276,

    /** [Skipper's text after you free the Sandship timeshift stone]
     *  Story Flag #277 (0x0115) - JP 805ACD78 0x80 / US 805A9AF8 0x80
     */
    STORYFLAG_277,

    /** [Finish Talking to Skipper in Skipper's Retreat (name on map appears)]
     *  Story Flag #278 (0x0116) - JP 805ACD7B 0x01 / US 805A9AFB 0x01
     */
    STORYFLAG_278,

    /** [Shipyard related (name on map appears)]
     *  Story Flag #279 (0x0117) - JP 805ACD7B 0x02 / US 805A9AFB 0x02
     */
    STORYFLAG_279,

    /** [Pirate Stronghold related (name on map appears)]
     *  Story Flag #280 (0x0118) - JP 805ACD7B 0x04 / US 805A9AFB 0x04
     */
    STORYFLAG_280,

    /** [Land on Fun Fun Island for first time (name on map appears)]
     *  Story Flag #281 (0x0119) - JP 805ACD7B 0x08 / US 805A9AFB 0x08
     */
    STORYFLAG_FUN_FUN_ISLAND_DISCOVERED,

    /** [Land on Lumpy Pumpkin Island for first time (name on map appears)]
     *  Story Flag #282 (0x011A) - JP 805ACD7B 0x10 / US 805A9AFB 0x10
     */
    STORYFLAG_LUMPY_PUMPKIN_DISCOVERED,

    /** [Talk to Beedle on his island for first time (name on map appears)]
     *  Story Flag #283 (0x011B) - JP 805ACD7B 0x20 / US 805A9AFB 0x20
     */
    STORYFLAG_BEEDLE_ISLAND_DISCOVERED,

    /** [Land on Bamboo Island for first time (name on map appears)]
     *  Story Flag #284 (0x011C) - JP 805ACD7B 0x40 / US 805A9AFB 0x40
     */
    STORYFLAG_BAMBOO_ISLAND_DISCOVERED,

    /** [Land on Isle of Songs for first time (name on map appears)]
     *  Story Flag #285 (0x011D) - JP 805ACD7B 0x80 / US 805A9AFB 0x80
     */
    STORYFLAG_ISLE_OF_SONGS_DISCOVERED,

    /** [Talk to Strich at Bug Heaven for first time (name on map appears)]
     *  Story Flag #286 (0x011E) - JP 805ACD7A 0x01 / US 805A9AFA 0x01
     */
    STORYFLAG_BUG_HAVEN_DISCOVERED,

    /** []
     *  Story Flag #287 (0x011F) - JP 805ACD7A 0x02 / US 805A9AFA 0x02
     */
    STORYFLAG_287,

    /** [Bilocyte fight triggered]
     *  Story Flag #288 (0x0120) - JP 805ACD7A 0x04 / US 805A9AFA 0x04
     */
    STORYFLAG_BILOCYCTE_FIGHT_TRIGGERED,

    /** [Owlan's wooden shield]
     *  Story Flag #289 (0x0121) - JP 805ACD7A 0x08 / US 805A9AFA 0x08
     */
    STORYFLAG_289,

    /** [set after breaking chandelier in Lumpy pumkin]
     *  Story Flag #290 (0x0122) - JP 805ACD7A 0x10 / US 805A9AFA 0x10
     */
    STORYFLAG_290,

    /** [Set after taking first Quest from Pumm (deliver hot pumpkin soup)]
     *  Story Flag #291 (0x0123) - JP 805ACD7A 0x20 / US 805A9AFA 0x20
     */
    STORYFLAG_291,

    /** [Set after delivering hot pumpkin soup]
     *  Story Flag #292 (0x0124) - JP 805ACD7A 0x40 / US 805A9AFA 0x40
     */
    STORYFLAG_292,

    /** [Set after 2nd Quest from Pumm (harvest Pumkins)]
     *  Story Flag #293 (0x0125) - JP 805ACD7A 0x80 / US 805A9AFA 0x80
     */
    STORYFLAG_293,

    /** [Finish carrying pumpkins for Kina]
     *  Story Flag #294 (0x0126) - JP 805ACD7D 0x01 / US 805A9AFD 0x01
     */
    STORYFLAG_294,

    /** []
     *  Story Flag #295 (0x0127) - JP 805ACD7D 0x02 / US 805A9AFD 0x02
     */
    STORYFLAG_295,

    /** [Finished last Quest for Pumm]
     *  Story Flag #296 (0x0128) - JP 805ACD7D 0x04 / US 805A9AFD 0x04
     */
    STORYFLAG_296,

    /** [Talking to Item Seller at night (he buys treasure)]
     *  Story Flag #297 (0x0129) - JP 805ACD7D 0x08 / US 805A9AFD 0x08
     */
    STORYFLAG_297,

    /** [Set after talking to Strich at night]
     *  Story Flag #298 (0x012A) - JP 805ACD7D 0x10 / US 805A9AFD 0x10
     */
    STORYFLAG_298,

    /** [Wryna asks Link for help]
     *  Story Flag #299 (0x012B) - JP 805ACD7D 0x20 / US 805A9AFD 0x20
     */
    STORYFLAG_299,

    /** [Set when talking to Strich]
     *  Story Flag #300 (0x012C) - JP 805ACD7D 0x40 / US 805A9AFD 0x40
     */
    STORYFLAG_300,

    /** [Kukiel screams]
     *  Story Flag #301 (0x012D) - JP 805ACD7D 0x80 / US 805A9AFD 0x80
     */
    STORYFLAG_301,

    /** []
     *  Story Flag #302 (0x012E) - JP 805ACD7C 0x01 / US 805A9AFC 0x01
     */
    STORYFLAG_302,

    /** [Wryna asks Link to find Kukiel (allows Kukiel to scream to unlock Batreaux's door)]
     *  Story Flag #303 (0x012F) - JP 805ACD7C 0x02 / US 805A9AFC 0x02
     */
    STORYFLAG_303,

    /** [Start Batreaux quest (enables Gratitude crystals)]
     *  Story Flag #304 (0x0130) - JP 805ACD7C 0x04 / US 805A9AFC 0x04
     */
    STORYFLAG_GRATITUDE_QUEST_STARTED,

    /** [Start Fledge's Pumpkin Minigame (unset after Fledge's Text) / Start Strich Bug Heaven minigame (unset after
     * Strich text)] Story Flag #305 (0x0131) - JP 805ACD7C 0x08 / US 805A9AFC 0x08
     */
    STORYFLAG_305,

    /** [Finish Fledge's Pumpkin Minigame (unset after Fledge's Text) / Win at Bug Heaven (unset after Strich text)]
     *  Story Flag #306 (0x0132) - JP 805ACD7C 0x10 / US 805A9AFC 0x10
     */
    STORYFLAG_306,

    /** []
     *  Story Flag #307 (0x0133) - JP 805ACD7C 0x20 / US 805A9AFC 0x20
     */
    STORYFLAG_307,

    /** []
     *  Story Flag #308 (0x0134) - JP 805ACD7C 0x40 / US 805A9AFC 0x40
     */
    STORYFLAG_308,

    /** [(not implemented yet -- 7C43)]
     *  Story Flag #309 (0x0135) - JP from 805ACE51 0x10 to 805ACE51 0x80 / US from 805A9BD1 0x10 to 805A9BD1 0x80
     */
    STORYFLAG_309,

    /** [(not implemented yet -- 7C85)]
     *  Story Flag #310 (0x0136) - JP from 805ACE50 0x01 to 805ACE50 0x20 / US from 805A9BD0 0x01 to 805A9BD0 0x20
     */
    STORYFLAG_310,

    /** [(not implemented yet -- 7B06)]
     *  Story Flag #311 (0x0137) - JP from 805ACE4F 0x01 to 805ACE4F 0x40 / US from 805A9BCF 0x01 to 805A9BCF 0x40
     */
    STORYFLAG_311,

    /** [(not implemented yet -- 7B73)]
     *  Story Flag #312 (0x0138) - JP from 805ACE4F 0x80 to 805ACE4E 0x04 / US from 805A9BCF 0x80 to 805A9BCE 0x04
     */
    STORYFLAG_312,

    /** [(not implemented yet -- 7A05)]
     *  Story Flag #313 (0x0139) - JP from 805ACE4D 0x01 to 805ACE4D 0x20 / US from 805A9BCD 0x01 to 805A9BCD 0x20
     */
    STORYFLAG_313,

    /** [(not implemented yet -- 7A66)]
     *  Story Flag #314 (0x013A) - JP from 805ACE4D 0x40 to 805ACE4C 0x10 / US from 805A9BCD 0x40 to 805A9BCC 0x10
     */
    STORYFLAG_314,

    /** [(not implemented yet -- 7BB3)]
     *  Story Flag #315 (0x013B) - JP from 805ACE4E 0x08 to 805ACE4E 0x40 / US from 805A9BCE 0x08 to 805A9BCE 0x40
     */
    STORYFLAG_315,

    /** [(not implemented yet -- 7905)]
     *  Story Flag #316 (0x013C) - JP from 805ACE4B 0x01 to 805ACE4B 0x20 / US from 805A9BCB 0x01 to 805A9BCB 0x20
     */
    STORYFLAG_316,

    /** [(not implemented yet -- 7966)]
     *  Story Flag #317 (0x013D) - JP from 805ACE4B 0x40 to 805ACE4A 0x10 / US from 805A9BCB 0x40 to 805A9BCA 0x10
     */
    STORYFLAG_317,

    /** [(not implemented yet -- 7803)]
     *  Story Flag #318 (0x013E) - JP from 805ACE49 0x01 to 805ACE49 0x08 / US from 805A9BC9 0x01 to 805A9BC9 0x08
     */
    STORYFLAG_318,

    /** [(not implemented yet -- 7845)]
     *  Story Flag #319 (0x013F) - JP from 805ACE49 0x10 to 805ACE48 0x02 / US from 805A9BC9 0x10 to 805A9BC8 0x02
     */
    STORYFLAG_319,

    /** [(not implemented yet -- 7706)]
     *  Story Flag #320 (0x0140) - JP from 805ACE47 0x01 to 805ACE47 0x40 / US from 805A9BC7 0x01 to 805A9BC7 0x40
     */
    STORYFLAG_320,

    /** [Talk to Gondo to repair Scrapper but not giveing him a Ancient Flower]
     *  Story Flag #321 (0x0141) - JP 805ACD7C 0x80 / US 805A9AFC 0x80
     */
    STORYFLAG_321,

    /** [Learn about Scrapper near Village Windmill]
     *  Story Flag #322 (0x0142) - JP 805ACD7F 0x01 / US 805A9AFF 0x01
     */
    STORYFLAG_322,

    /** [Scrapper Repaired]
     *  Story Flag #323 (0x0143) - JP 805ACD7F 0x02 / US 805A9AFF 0x02
     */
    STORYFLAG_SCRAPPER_UNLOCKED,

    /** [Windmill Propeller picked up]
     *  Story Flag #324 (0x0144) - JP 805ACD7F 0x04 / US 805A9AFF 0x04
     */
    STORYFLAG_324,

    /** [Village Windmill reparation cutscene]
     *  Story Flag #325 (0x0145) - JP 805ACD7F 0x08 / US 805A9AFF 0x08
     */
    STORYFLAG_325,

    /** [Wyrna's 5 gratitude crystals collected (not the crystals themselves)]
     *  Story Flag #326 (0x0146) - JP 805ACD7F 0x10 / US 805A9AFF 0x10
     */
    STORYFLAG_326,

    /** [Talk to the Headmaster to learn more about the Song of the Hero (he doesn't know anything)]
     *  Story Flag #327 (0x0147) - JP 805ACD7F 0x20 / US 805A9AFF 0x20
     */
    STORYFLAG_327,

    /** [Headmaster sends you see Owlan to locate Levias]
     *  Story Flag #328 (0x0148) - JP 805ACD7F 0x40 / US 805A9AFF 0x40
     */
    STORYFLAG_328,

    /** [Sleeping after starting Batreaux quest]
     *  Story Flag #329 (0x0149) - JP 805ACD7F 0x80 / US 805A9AFF 0x80
     */
    STORYFLAG_329,

    /** [Eldin Silent Realm Introductory Cutscene]
     *  Story Flag #330 (0x014A) - JP 805ACD7E 0x01 / US 805A9AFE 0x01
     */
    STORYFLAG_330,

    /** [Hylia's Silent Realm Introductory Cutscene]
     *  Story Flag #331 (0x014B) - JP 805ACD7E 0x02 / US 805A9AFE 0x02
     */
    STORYFLAG_331,

    /** [(not implemented yet -- 7CE1)]
     *  Story Flag #332 (0x014C) - JP from 805ACE50 0x40 to 805ACE50 0x80 / US from 805A9BD0 0x40 to 805A9BD0 0x80
     */
    STORYFLAG_332,

    /** [-]
     *  Story Flag #333 (0x014D) - JP - / US -
     */
    STORYFLAG_333,

    /** [-]
     *  Story Flag #334 (0x014E) - JP - / US -
     */
    STORYFLAG_334,

    /** [-]
     *  Story Flag #335 (0x014F) - JP - / US -
     */
    STORYFLAG_335,

    /** [-]
     *  Story Flag #336 (0x0150) - JP - / US -
     */
    STORYFLAG_336,

    /** [-]
     *  Story Flag #337 (0x0151) - JP - / US -
     */
    STORYFLAG_337,

    /** [-]
     *  Story Flag #338 (0x0152) - JP - / US -
     */
    STORYFLAG_338,

    /** [-]
     *  Story Flag #339 (0x0153) - JP - / US -
     */
    STORYFLAG_339,

    /** [Gate of Time raised]
     *  Story Flag #340 (0x0154) - JP 805ACD7E 0x04 / US 805A9AFE 0x04
     */
    STORYFLAG_340,

    /** [Gate of Time opened (Hylia's Temple layer 1)]
     *  Story Flag #341 (0x0155) - JP 805ACD7E 0x08 / US 805A9AFE 0x08
     */
    STORYFLAG_341,

    /** [Set after beating Sky Keep (Entering the Sealed Grounds now loads the map with the Goddess Statue/ Horde
     * Cutscene can play correctly) (Hylia's Temple layer 2 / Sealed Temple layer 7 / Behind The Temple layer 8 /
     * Hylia's Realm layer 13 / Sealed Grounds with Goddess Statue layer 1)] Story Flag #342 (0x0156) - JP 805ACD7E 0x10
     * / US 805A9AFE 0x10
     */
    STORYFLAG_342,

    /** [[should set after obtaining the BotG, but because of a coding error doesn't]]
     *  Story Flag #343 (0x0157) - JP 805ACD7E 0x20 / US 805A9AFE 0x20
     */
    STORYFLAG_343,

    /** [Gate of Time opening cutscene]
     *  Story Flag #344 (0x0158) - JP 805ACD7E 0x40 / US 805A9AFE 0x40
     */
    STORYFLAG_344,

    /** [Ghirahim 3 attacks the Sealed Temple after Sky Keep Cutscene]
     *  Story Flag #345 (0x0159) - JP 805ACD7E 0x80 / US 805A9AFE 0x80
     */
    STORYFLAG_345,

    /** []
     *  Story Flag #346 (0x015A) - JP 805ACD81 0x01 / US 805A9B01 0x01
     */
    STORYFLAG_346,

    /** [Horde Cutscene (Hylia's Realm layer 1)]
     *  Story Flag #347 (0x015B) - JP 805ACD81 0x02 / US 805A9B01 0x02
     */
    STORYFLAG_347,

    /** [Cutscene before Ghirahim 3]
     *  Story Flag #348 (0x015C) - JP 805ACD81 0x04 / US 805A9B01 0x04
     */
    STORYFLAG_348,

    /** [Past Impa Text before Zelda]
     *  Story Flag #349 (0x015D) - JP 805ACD81 0x08 / US 805A9B01 0x08
     */
    STORYFLAG_349,

    /** [True Master Sword Cutscene]
     *  Story Flag #350 (0x015E) - JP 805ACD81 0x10 / US 805A9B01 0x10
     */
    STORYFLAG_350,

    /** [[only apears to get unset by Boss Rush]]
     *  Story Flag #351 (0x015F) - JP 805ACD81 0x20 / US 805A9B01 0x20
     */
    STORYFLAG_351,

    /** [[only apears to get unset by Boss Rush]]
     *  Story Flag #352 (0x0160) - JP 805ACD81 0x40 / US 805A9B01 0x40
     */
    STORYFLAG_352,

    /** [Sets when refighting G1 (unsets after the fight)]
     *  Story Flag #353 (0x0161) - JP 805ACD81 0x80 / US 805A9B01 0x80
     */
    STORYFLAG_353,

    /** [Set when refighting G1 or Scaldera (unsets after the fight)]
     *  Story Flag #354 (0x0162) - JP 805ACD80 0x04 / US 805A9B00 0x04
     */
    STORYFLAG_354,

    /** [Sets when fighting any early fight (unsets after the fight)]
     *  Story Flag #355 (0x0163) - JP 805ACD80 0x02 / US 805A9B00 0x02
     */
    STORYFLAG_355,

    /** [Sets when fighting any early fight or fighting Koloktos (unsets after the fight)]
     *  Story Flag #356 (0x0164) - JP 805ACD80 0x01 / US 805A9B00 0x01
     */
    STORYFLAG_356,

    /** [Sets when fighting any early fight or fighting Koloktos (unsets after the fight)]
     *  Story Flag #357 (0x0165) - JP 805ACD80 0x08 / US 805A9B00 0x08
     */
    STORYFLAG_357,

    /** [Set after Fi text before going to sky for first time (Unlocks Interface choice)]
     *  Story Flag #358 (0x0166) - JP 805ACD80 0x10 / US 805A9B00 0x10
     */
    STORYFLAG_UI_MODE_UNLOCKED,

    /** [Getting Fledge's Arrow minigame HP]
     *  Story Flag #359 (0x0167) - JP 805ACD80 0x20 / US 805A9B00 0x20
     */
    STORYFLAG_359,

    /** [Batreaux turns human]
     *  Story Flag #360 (0x0168) - JP 805ACD80 0x40 / US 805A9B00 0x40
     */
    STORYFLAG_GRATITUDE_QUEST_FINISHED,

    /** [Set after collecting 5 gratitude crystal from Pipit's Mom (allows Pipit scolding his mom)]
     *  Story Flag #361 (0x0169) - JP 805ACD80 0x80 / US 805A9B00 0x80
     */
    STORYFLAG_361,

    /** [Talk to Professor Owlan inside Academy to locate Levias]
     *  Story Flag #362 (0x016A) - JP 805ACD83 0x01 / US 805A9B03 0x01
     */
    STORYFLAG_362,

    /** [Prof Owlan explains Spiral Charge on Skyloft]
     *  Story Flag #363 (0x016B) - JP 805ACD83 0x02 / US 805A9B03 0x02
     */
    STORYFLAG_363,

    /** [Spiral Charge Ability]
     *  Story Flag #364 (0x016C) - JP 805ACD83 0x04 / US 805A9B03 0x04
     */
    STORYFLAG_364,

    /** [Pumpkin Soup for Levias Collected, unset by picking up something else]
     *  Story Flag #365 (0x016D) - JP 805ACD83 0x08 / US 805A9B03 0x08
     */
    STORYFLAG_365,

    /** [Unset when Scrapper has a job, set when he has finished it]
     *  Story Flag #366 (0x016E) - JP 805ACD83 0x10 / US 805A9B03 0x10
     */
    STORYFLAG_366,

    /** [[conected to a disapearence of a npc in lumpy pumpkin]]
     *  Story Flag #367 (0x016F) - JP 805ACD83 0x20 / US 805A9B03 0x20
     */
    STORYFLAG_367,

    /** [Pumpkin Soup delivered on Rainbow Island]
     *  Story Flag #368 (0x0170) - JP 805ACD83 0x40 / US 805A9B03 0x40
     */
    STORYFLAG_LEVIAS_FIGHT_STARTED,

    /** [Learn Song of the Hero (Waterfall Cave layer 1)]
     *  Story Flag #369 (0x0171) - JP 805ACD83 0x80 / US 805A9B03 0x80
     */
    STORYFLAG_369,

    /** [ Talking to past Impa after meeting Batreaux or Meeting Batreaux after talking to past Impa  (Bazaar layer 2)]
     *  Story Flag #370 (0x0172) - JP 805ACD82 0x01 / US 805A9B02 0x01
     */
    STORYFLAG_370,

    /** [Accepting the Fortune Teller Crystal Sidequest]
     *  Story Flag #371 (0x0173) - JP 805ACD82 0x02 / US 805A9B02 0x02
     */
    STORYFLAG_371,

    /** [Sets after Sprapper picked the Crystal Ball up]
     *  Story Flag #372 (0x0174) - JP 805ACD82 0x04 / US 805A9B02 0x04
     */
    STORYFLAG_372,

    /** [Sets before the Fortune Teller gives his Crystals]
     *  Story Flag #373 (0x0175) - JP 805ACD82 0x08 / US 805A9B02 0x08
     */
    STORYFLAG_373,

    /** [Sets when talking to the Fortune Teller in the Bazzar after 8x08 (Fortunes only cost half)]
     *  Story Flag #374 (0x0176) - JP 805ACD82 0x10 / US 805A9B02 0x10
     */
    STORYFLAG_374,

    /** [Set after Sleeping after the Fortune Teller crystals (Bazaar layer 3)]
     *  Story Flag #375 (0x0177) - JP 805ACD82 0x20 / US 805A9B02 0x20
     */
    STORYFLAG_375,

    /** [Set after Sleeping after Groose cs in BtT]
     *  Story Flag #376 (0x0178) - JP 805ACD82 0x40 / US 805A9B02 0x40
     */
    STORYFLAG_376,

    /** [Set when accepting to clean Pipit House]
     *  Story Flag #377 (0x0179) - JP 805ACD82 0x80 / US 805A9B02 0x80
     */
    STORYFLAG_377,

    /** [Break stuffs in item sellers house]
     *  Story Flag #378 (0x017A) - JP 805ACD85 0x01 / US 805A9B05 0x01
     */
    STORYFLAG_378,

    /** [Thunderhead entered for first time]
     *  Story Flag #379 (0x017B) - JP 805ACD85 0x02 / US 805A9B05 0x02
     */
    STORYFLAG_THUNDERHEAD_ENTERED,

    /** [Parrow gives his Crystal Sidequest (to find Orielle)]
     *  Story Flag #380 (0x017C) - JP 805ACD8A 0x40 / US 805A9B0A 0x40
     */
    STORYFLAG_380,

    /** [Orielle gives her Sidequest (to go back to Parrow)]
     *  Story Flag #381 (0x017D) - JP 805ACD8A 0x80 / US 805A9B0A 0x80
     */
    STORYFLAG_381,

    /** [Parrow gives his final crystal Sidequest (heal Orielle's bird)]
     *  Story Flag #382 (0x017E) - JP 805ACD8D 0x01 / US 805A9B0D 0x01
     */
    STORYFLAG_382,

    /** [Heal Orielle's bird]
     *  Story Flag #383 (0x017F) - JP 805ACD8D 0x02 / US 805A9B0D 0x02
     */
    STORYFLAG_383,

    /** [Talk to Parrow after healing Orielle's bird]
     *  Story Flag #384 (0x0180) - JP 805ACD8D 0x04 / US 805A9B0D 0x04
     */
    STORYFLAG_384,

    /** []
     *  Story Flag #385 (0x0181) - JP 805ACD8D 0x08 / US 805A9B0D 0x08
     */
    STORYFLAG_385,

    /** [Talk to Bertie at Night after Clawshots]
     *  Story Flag #386 (0x0182) - JP 805ACD8D 0x10 / US 805A9B0D 0x10
     */
    STORYFLAG_386,

    /** [Talk to Bertie after getting Rattle (for 5 crystals)]
     *  Story Flag #387 (0x0183) - JP 805ACD8D 0x20 / US 805A9B0D 0x20
     */
    STORYFLAG_RATTLE_GIVEN,

    /** [Talk to Fledge at night after LMF]
     *  Story Flag #388 (0x0184) - JP 805ACD8D 0x40 / US 805A9B0D 0x40
     */
    STORYFLAG_388,

    /** [Give Fledge 1st stamina potion]
     *  Story Flag #389 (0x0185) - JP 805ACD8D 0x80 / US 805A9B0D 0x80
     */
    STORYFLAG_389,

    /** [Set when entering Fledge room when he is Training the second time]
     *  Story Flag #390 (0x0186) - JP 805ACD8C 0x01 / US 805A9B0C 0x01
     */
    STORYFLAG_390,

    /** [Set after Fledge sees the second Stamina Potion]
     *  Story Flag #391 (0x0187) - JP 805ACD8C 0x02 / US 805A9B0C 0x02
     */
    STORYFLAG_391,

    /** [Give Fledge the second Stamina Potion]
     *  Story Flag #392 (0x0188) - JP 805ACD8C 0x04 / US 805A9B0C 0x04
     */
    STORYFLAG_392,

    /** [Set when entering Fledge room when he is Training the third time]
     *  Story Flag #393 (0x0189) - JP 805ACD8C 0x08 / US 805A9B0C 0x08
     */
    STORYFLAG_393,

    /** [Fledge's 5 crystals collected]
     *  Story Flag #394 (0x018A) - JP 805ACD8C 0x10 / US 805A9B0C 0x10
     */
    STORYFLAG_394,

    /** [set when Parrow attempts to give you his bottle with a full Pouch]
     *  Story Flag #395 (0x018B) - JP 805ACD8C 0x80 / US 805A9B0C 0x80
     */
    STORYFLAG_395,

    /** [Search for Party Wheel Crystal Quest started]
     *  Story Flag #396 (0x018C) - JP 805ACD8F 0x20 / US 805A9B0F 0x20
     */
    STORYFLAG_396,

    /** [Set after Scrapper picks up Party Wheel]
     *  Story Flag #397 (0x018D) - JP 805ACD8F 0x80 / US 805A9B0F 0x80
     */
    STORYFLAG_397,

    /** [Set before Fun Fun Owner gives his Crystals]
     *  Story Flag #398 (0x018E) - JP 805ACD8E 0x01 / US 805A9B0E 0x01
     */
    STORYFLAG_398,

    /** [Sets after you get a free try at Diveing game]
     *  Story Flag #399 (0x018F) - JP 805ACD91 0x80 / US 805A9B11 0x80
     */
    STORYFLAG_399,

    /** [Talk to Fledge for pumpkin minigame]
     *  Story Flag #400 (0x0190) - JP 805ACD90 0x01 / US 805A9B10 0x01
     */
    STORYFLAG_400,

    /** [Exhausting Goddess Wall in Sealed Grounds (unset after starting your File)]
     *  Story Flag #401 (0x0191) - JP 805ACD85 0x04 / US 805A9B05 0x04
     */
    STORYFLAG_401,

    /** [Exhausting a unknown goddess wall (Unset after S&Q)]
     *  Story Flag #402 (0x0192) - JP 805ACD85 0x08 / US 805A9B05 0x08
     */
    STORYFLAG_402,

    /** [Exhausting Goddess Wall in Skyview 2 (unset after starting your File)]
     *  Story Flag #403 (0x0193) - JP 805ACD85 0x10 / US 805A9B05 0x10
     */
    STORYFLAG_403,

    /** [Exhausting Goddess Wall in FS (bottom of central stairway) (unset after starting your File)]
     *  Story Flag #404 (0x0194) - JP 805ACD85 0x20 / US 805A9B05 0x20
     */
    STORYFLAG_404,

    /** [Exhausting a unknown goddess wall (Unset after S&Q)]
     *  Story Flag #405 (0x0195) - JP 805ACD85 0x40 / US 805A9B05 0x40
     */
    STORYFLAG_405,

    /** [Exhausting a unknown goddess wall (Unset after S&Q)]
     *  Story Flag #406 (0x0196) - JP 805ACD85 0x80 / US 805A9B05 0x80
     */
    STORYFLAG_406,

    /** [Exhausting a unknown goddess wall (Unset after S&Q)]
     *  Story Flag #407 (0x0197) - JP 805ACD84 0x01 / US 805A9B04 0x01
     */
    STORYFLAG_407,

    /** [Exhausting a unknown goddess wall (Unset after S&Q)]
     *  Story Flag #408 (0x0198) - JP 805ACD84 0x02 / US 805A9B04 0x02
     */
    STORYFLAG_408,

    /** [Exhausting a unknown goddess wall (Unset after S&Q)]
     *  Story Flag #409 (0x0199) - JP 805ACD84 0x04 / US 805A9B04 0x04
     */
    STORYFLAG_409,

    /** [Exhausting a unknown goddess wall (Unset after S&Q)]
     *  Story Flag #410 (0x019A) - JP 805ACD84 0x08 / US 805A9B04 0x08
     */
    STORYFLAG_410,

    /** [Exhausting a unknown goddess wall (Unset after S&Q)]
     *  Story Flag #411 (0x019B) - JP 805ACD84 0x10 / US 805A9B04 0x10
     */
    STORYFLAG_411,

    /** [Exhausting Goddess Wall in AC Whirpool (unset after starting your File)]
     *  Story Flag #412 (0x019C) - JP 805ACD84 0x20 / US 805A9B04 0x20
     */
    STORYFLAG_412,

    /** [Exhausting Goddess Wall in AC Basement (unset after starting your File)]
     *  Story Flag #413 (0x019D) - JP 805ACD84 0x40 / US 805A9B04 0x40
     */
    STORYFLAG_413,

    /** [Exhausting a unknown goddess wall (Unset after S&Q)]
     *  Story Flag #414 (0x019E) - JP 805ACD84 0x80 / US 805A9B04 0x80
     */
    STORYFLAG_414,

    /** [Exhausting Goddess Wall in Sandship (B1)(unset after starting your File)]
     *  Story Flag #415 (0x019F) - JP 805ACD87 0x01 / US 805A9B07 0x01
     */
    STORYFLAG_415,

    /** [Exhausting Goddess Wall in Sandship on Deck (unset after starting your File)]
     *  Story Flag #416 (0x01A0) - JP 805ACD87 0x02 / US 805A9B07 0x02
     */
    STORYFLAG_416,

    /** []
     *  Story Flag #417 (0x01A1) - JP 805ACD87 0x04 / US 805A9B07 0x04
     */
    STORYFLAG_417,

    /** [Exhausting Goddess Wall in FS (1F) (unset after starting your File)]
     *  Story Flag #418 (0x01A2) - JP 805ACD87 0x08 / US 805A9B07 0x08
     */
    STORYFLAG_418,

    /** [Exhausting Goddess Wall in FS (after you jumped into Mouth) (unset after starting your File)]
     *  Story Flag #419 (0x01A3) - JP 805ACD87 0x10 / US 805A9B07 0x10
     */
    STORYFLAG_419,

    /** [Exhausting Goddess Wall in Skykeep]
     *  Story Flag #420 (0x01A4) - JP 805ACD87 0x20 / US 805A9B07 0x20
     */
    STORYFLAG_420,

    /** [drawing random shape on Sealed grounds goddess wall (unsets after S&Q)]
     *  Story Flag #421 (0x01A5) - JP 805ACD87 0x40 / US 805A9B07 0x40
     */
    STORYFLAG_421,

    /** [drawing a rupee on Sealed grounds goddess wall (unsets after S&Q)]
     *  Story Flag #422 (0x01A6) - JP 805ACD87 0x80 / US 805A9B07 0x80
     */
    STORYFLAG_422,

    /** [drawing a heart on  Sealed grounds goddess wall  (unsets after S&Q)]
     *  Story Flag #423 (0x01A7) - JP 805ACD86 0x01 / US 805A9B06 0x01
     */
    STORYFLAG_423,

    /** [drawing a Triforce on  Sealed grounds goddess wall  (unsets after S&Q)]
     *  Story Flag #424 (0x01A8) - JP 805ACD86 0x02 / US 805A9B06 0x02
     */
    STORYFLAG_424,

    /** [drawing a Bomb on Sealed grounds goddess wall  (unsets after S&Q)]
     *  Story Flag #425 (0x01A9) - JP 805ACD86 0x04 / US 805A9B06 0x04
     */
    STORYFLAG_425,

    /** [drawing a Arrow on Sealed grounds goddess wall (unsets after S&Q)]
     *  Story Flag #426 (0x01AA) - JP 805ACD86 0x08 / US 805A9B06 0x08
     */
    STORYFLAG_426,

    /** [set after getting Gorko's request for goddess Wall (unsets after S&Q)]
     *  Story Flag #427 (0x01AB) - JP 805ACD86 0x10 / US 805A9B06 0x10
     */
    STORYFLAG_427,

    /** []
     *  Story Flag #428 (0x01AC) - JP 805ACD86 0x20 / US 805A9B06 0x20
     */
    STORYFLAG_428,

    /** []
     *  Story Flag #429 (0x01AD) - JP 805ACD86 0x40 / US 805A9B06 0x40
     */
    STORYFLAG_429,

    /** []
     *  Story Flag #430 (0x01AE) - JP 805ACD86 0x80 / US 805A9B06 0x80
     */
    STORYFLAG_430,

    /** [Reveal Goddess Wall in Sealed Grounds]
     *  Story Flag #431 (0x01AF) - JP 805ACD89 0x01 / US 805A9B09 0x01
     */
    STORYFLAG_431,

    /** []
     *  Story Flag #432 (0x01B0) - JP 805ACD89 0x02 / US 805A9B09 0x02
     */
    STORYFLAG_432,

    /** [Reveal Goddess Wall in Skyview 2]
     *  Story Flag #433 (0x01B1) - JP 805ACD89 0x04 / US 805A9B09 0x04
     */
    STORYFLAG_433,

    /** [Reveal Goddess Wall in FS (bottom of central stairway)]
     *  Story Flag #434 (0x01B2) - JP 805ACD89 0x08 / US 805A9B09 0x08
     */
    STORYFLAG_434,

    /** []
     *  Story Flag #435 (0x01B3) - JP 805ACD89 0x10 / US 805A9B09 0x10
     */
    STORYFLAG_435,

    /** []
     *  Story Flag #436 (0x01B4) - JP 805ACD89 0x20 / US 805A9B09 0x20
     */
    STORYFLAG_436,

    /** []
     *  Story Flag #437 (0x01B5) - JP 805ACD89 0x40 / US 805A9B09 0x40
     */
    STORYFLAG_437,

    /** []
     *  Story Flag #438 (0x01B6) - JP 805ACD89 0x80 / US 805A9B09 0x80
     */
    STORYFLAG_438,

    /** []
     *  Story Flag #439 (0x01B7) - JP 805ACD88 0x01 / US 805A9B08 0x01
     */
    STORYFLAG_439,

    /** []
     *  Story Flag #440 (0x01B8) - JP 805ACD88 0x02 / US 805A9B08 0x02
     */
    STORYFLAG_440,

    /** []
     *  Story Flag #441 (0x01B9) - JP 805ACD88 0x04 / US 805A9B08 0x04
     */
    STORYFLAG_441,

    /** [Reveal Goddess Wall in AC (in Basement)]
     *  Story Flag #442 (0x01BA) - JP 805ACD88 0x08 / US 805A9B08 0x08
     */
    STORYFLAG_442,

    /** [Reveal Goddess Wall in AC (near Whirlpool)]
     *  Story Flag #443 (0x01BB) - JP 805ACD88 0x10 / US 805A9B08 0x10
     */
    STORYFLAG_443,

    /** []
     *  Story Flag #444 (0x01BC) - JP 805ACD88 0x20 / US 805A9B08 0x20
     */
    STORYFLAG_444,

    /** [Reveal Goddess Wall In Sandship (B1)]
     *  Story Flag #445 (0x01BD) - JP 805ACD88 0x40 / US 805A9B08 0x40
     */
    STORYFLAG_445,

    /** [Reveal Goddess Wall in Sandship on Deck]
     *  Story Flag #446 (0x01BE) - JP 805ACD88 0x80 / US 805A9B08 0x80
     */
    STORYFLAG_446,

    /** []
     *  Story Flag #447 (0x01BF) - JP 805ACD8B 0x01 / US 805A9B0B 0x01
     */
    STORYFLAG_447,

    /** [Reveal Goddess Wall In FS (1F)]
     *  Story Flag #448 (0x01C0) - JP 805ACD8B 0x02 / US 805A9B0B 0x02
     */
    STORYFLAG_448,

    /** [Reveal Goddess Wall in FS (after you jumped into Mouth)]
     *  Story Flag #449 (0x01C1) - JP 805ACD8B 0x04 / US 805A9B0B 0x04
     */
    STORYFLAG_449,

    /** [Reveal Goddess Wall in Skykeep (Skyview Room)]
     *  Story Flag #450 (0x01C2) - JP 805ACD8B 0x08 / US 805A9B0B 0x08
     */
    STORYFLAG_450,

    /** []
     *  Story Flag #451 (0x01C3) - JP 805ACD8B 0x10 / US 805A9B0B 0x10
     */
    STORYFLAG_451,

    /** []
     *  Story Flag #452 (0x01C4) - JP 805ACD8B 0x20 / US 805A9B0B 0x20
     */
    STORYFLAG_452,

    /** []
     *  Story Flag #453 (0x01C5) - JP 805ACD8B 0x40 / US 805A9B0B 0x40
     */
    STORYFLAG_453,

    /** []
     *  Story Flag #454 (0x01C6) - JP 805ACD8B 0x80 / US 805A9B0B 0x80
     */
    STORYFLAG_454,

    /** []
     *  Story Flag #455 (0x01C7) - JP 805ACD8A 0x01 / US 805A9B0A 0x01
     */
    STORYFLAG_455,

    /** []
     *  Story Flag #456 (0x01C8) - JP 805ACD8A 0x02 / US 805A9B0A 0x02
     */
    STORYFLAG_456,

    /** []
     *  Story Flag #457 (0x01C9) - JP 805ACD8A 0x04 / US 805A9B0A 0x04
     */
    STORYFLAG_457,

    /** []
     *  Story Flag #458 (0x01CA) - JP 805ACD8A 0x08 / US 805A9B0A 0x08
     */
    STORYFLAG_458,

    /** []
     *  Story Flag #459 (0x01CB) - JP 805ACD8A 0x10 / US 805A9B0A 0x10
     */
    STORYFLAG_459,

    /** []
     *  Story Flag #460 (0x01CC) - JP 805ACD8A 0x20 / US 805A9B0A 0x20
     */
    STORYFLAG_460,

    /** [Panning Camera Cutscene in Lanayru Caves showing crawlspace to Lanayru Gorge]
     *  Story Flag #461 (0x01CD) - JP 805ACD8C 0x20 / US 805A9B0C 0x20
     */
    STORYFLAG_LANAYRU_CAVES_DISCOVERED,

    /** [Set when Thunder Dragon teaches SotH part]
     *  Story Flag #462 (0x01CE) - JP 805ACD8C 0x40 / US 805A9B0C 0x40
     */
    STORYFLAG_THUNDER_DRAGON_SONG,

    /** [Cutscene of Guld getting picked up]
     *  Story Flag #463 (0x01CF) - JP 805ACD8F 0x01 / US 805A9B0F 0x01
     */
    STORYFLAG_463,

    /** [Ghirahim 2 Intro Cutscene]
     *  Story Flag #464 (0x01D0) - JP 805ACD8F 0x02 / US 805A9B0F 0x02
     */
    STORYFLAG_464,

    /** [Sets if Lanayru SotH is your last SotH part]
     *  Story Flag #465 (0x01D1) - JP 805ACD8F 0x04 / US 805A9B0F 0x04
     */
    STORYFLAG_465,

    /** [Ghirahim 1 Intro Cutscene]
     *  Story Flag #466 (0x01D2) - JP 805ACD8F 0x08 / US 805A9B0F 0x08
     */
    STORYFLAG_466,

    /** [Goron Text near Big Frog]
     *  Story Flag #467 (0x01D3) - JP 805ACD8F 0x10 / US 805A9B0F 0x10
     */
    STORYFLAG_467,

    /** [Goron Text after Watering 1st Frog]
     *  Story Flag #468 (0x01D4) - JP 805ACD8F 0x40 / US 805A9B0F 0x40
     */
    STORYFLAG_468,

    /** [Goron Text after Watering 2nd frog]
     *  Story Flag #469 (0x01D5) - JP 805ACD8E 0x02 / US 805A9B0E 0x02
     */
    STORYFLAG_469,

    /** [Kina's Crystal Quest started]
     *  Story Flag #470 (0x01D6) - JP 805ACD8E 0x04 / US 805A9B0E 0x04
     */
    STORYFLAG_470,

    /** [Guld picked up (unsets after)]
     *  Story Flag #471 (0x01D7) - JP 805ACD8E 0x08 / US 805A9B0E 0x08
     */
    STORYFLAG_471,

    /** [Kina's Crystal Quest related]
     *  Story Flag #472 (0x01D8) - JP 805ACD8E 0x10 / US 805A9B0E 0x10
     */
    STORYFLAG_472,

    /** [Kina's Crystal Quest related]
     *  Story Flag #473 (0x01D9) - JP 805ACD8E 0x20 / US 805A9B0E 0x20
     */
    STORYFLAG_473,

    /** [Talk to Beedle about lost Horned Collossus Beetle at night]
     *  Story Flag #474 (0x01DA) - JP 805ACD8E 0x40 / US 805A9B0E 0x40
     */
    STORYFLAG_474,

    /** [Ask Strich about Beedle's Horned Collossus Beetle at Bug Heaven]
     *  Story Flag #475 (0x01DB) - JP 805ACD8E 0x80 / US 805A9B0E 0x80
     */
    STORYFLAG_475,

    /** [Beedle's Insect Cage collected]
     *  Story Flag #476 (0x01DC) - JP 805ACD91 0x01 / US 805A9B11 0x01
     */
    STORYFLAG_BEEDLE_CAGE_AQUIRED,

    /** [Set after giving Beedle the cage]
     *  Story Flag #477 (0x01DD) - JP 805ACD91 0x02 / US 805A9B11 0x02
     */
    STORYFLAG_BEEDLE_CAGE_GIVEN,

    /** [Talking to Beedle again after giveing him the cage]
     *  Story Flag #478 (0x01DE) - JP 805ACD91 0x04 / US 805A9B11 0x04
     */
    STORYFLAG_478,

    /** [Owlans Crystal Sidequest started]
     *  Story Flag #479 (0x01DF) - JP 805ACD91 0x08 / US 805A9B11 0x08
     */
    STORYFLAG_479,

    /** []
     *  Story Flag #480 (0x01E0) - JP 805ACD91 0x10 / US 805A9B11 0x10
     */
    STORYFLAG_480,

    /** [Oolo picked up with Scrapper]
     *  Story Flag #481 (0x01E1) - JP 805ACD91 0x20 / US 805A9B11 0x20
     */
    STORYFLAG_481,

    /** [Owlan's 5 crystals collected]
     *  Story Flag #482 (0x01E2) - JP 805ACD91 0x40 / US 805A9B11 0x40
     */
    STORYFLAG_482,

    /** [Imp 3 related]
     *  Story Flag #483 (0x01E3) - JP 805ACD90 0x02 / US 805A9B10 0x02
     */
    STORYFLAG_483,

    /** [Goddess Statue falling onto the Sealed Grounds Cutscene]
     *  Story Flag #484 (0x01E4) - JP 805ACD90 0x04 / US 805A9B10 0x04
     */
    STORYFLAG_GODDESS_STATUE_FALLEN,

    /** [Groose goes back to the Groosenator after defeating Imp 3]
     *  Story Flag #485 (0x01E5) - JP 805ACD90 0x08 / US 805A9B10 0x08
     */
    STORYFLAG_485,

    /** [Ghirahim 3 defeated (Hylia's Temple layer 3 / Hylia's Realm layer 3 / Sealed Grounds with Goddess Statue layer
     * 2)] Story Flag #486 (0x01E6) - JP 805ACD90 0x10 / US 805A9B10 0x10
     */
    STORYFLAG_486,

    /** []
     *  Story Flag #487 (0x01E7) - JP 805ACD90 0x20 / US 805A9B10 0x20
     */
    STORYFLAG_487,

    /** [Zelda and Impa Reunion Cutscene (during credits)]
     *  Story Flag #488 (0x01E8) - JP 805ACD90 0x40 / US 805A9B10 0x40
     */
    STORYFLAG_488,

    /** [Set after Ghirahim 3]
     *  Story Flag #489 (0x01E9) - JP 805ACD90 0x80 / US 805A9B10 0x80
     */
    STORYFLAG_489,

    /** [Skip last cutscene during credits (Groose appears in the Groosenator, re-enables groosenator to lanayru after
     * sky keep)] Story Flag #490 (0x01EA) - JP 805ACD93 0x01 / US 805A9B13 0x01
     */
    STORYFLAG_GAME_COMPLETE,

    /** [sets after finishing request for gorko at goddess wall (or S&Q) (unsets after getting new request)]
     *  Story Flag #491 (0x01EB) - JP 805ACD93 0x02 / US 805A9B13 0x02
     */
    STORYFLAG_491,

    /** [Statues activated in Sealed Grounds]
     *  Story Flag #492 (0x01EC) - JP 805ACD93 0x04 / US 805A9B13 0x04
     */
    STORYFLAG_STATUES_ACTIVATED,

    /** [Set after talking to Skyview 2 Mogma after revealing Goddess Wall or going past him (unlocks Goddess Walls)]
     *  Story Flag #493 (0x01ED) - JP 805ACD93 0x08 / US 805A9B13 0x08
     */
    STORYFLAG_493,

    /** [Gate to Faron Woods Opened]
     *  Story Flag #494 (0x01EE) - JP 805ACD93 0x10 / US 805A9B13 0x10
     */
    STORYFLAG_494,

    /** [Set to reset beating imp 3 after fighting him in Boss refights (only if you have beaten him before)]
     *  Story Flag #495 (0x01EF) - JP 805ACD93 0x20 / US 805A9B13 0x20
     */
    STORYFLAG_495,

    /** [Talk to Skipper after Sea Chart in Skipper's Retreat]
     *  Story Flag #496 (0x01F0) - JP 805ACD93 0x40 / US 805A9B13 0x40
     */
    STORYFLAG_496,

    /** [Talk to Skipper after defeating Molderach 2 (Shipyard Construction Bay layer 3)]
     *  Story Flag #497 (0x01F1) - JP 805ACD93 0x80 / US 805A9B13 0x80
     */
    STORYFLAG_497,

    /** [Talking to Skipper after getting Ship Dowsing]
     *  Story Flag #498 (0x01F2) - JP 805ACD92 0x01 / US 805A9B12 0x01
     */
    STORYFLAG_498,

    /** [First Quest from Skipper given in the Sand Sea (to Skipper's Retreat)]
     *  Story Flag #499 (0x01F3) - JP 805ACD92 0x02 / US 805A9B12 0x02
     */
    STORYFLAG_499,

    /** [[sets when after minecart minigame when not haveing 94 0x40]]
     *  Story Flag #500 (0x01F4) - JP 805ACD92 0x04 / US 805A9B12 0x04
     */
    STORYFLAG_500,

    /** [Starting minecart minigame (Shipyard layer 2)]
     *  Story Flag #501 (0x01F5) - JP 805ACD92 0x08 / US 805A9B12 0x08
     */
    STORYFLAG_501,

    /** []
     *  Story Flag #502 (0x01F6) - JP 805ACD92 0x10 / US 805A9B12 0x10
     */
    STORYFLAG_502,

    /** [Talking to Gortram for the second time after defeating Moldy 2]
     *  Story Flag #503 (0x01F7) - JP 805ACD92 0x20 / US 805A9B12 0x20
     */
    STORYFLAG_503,

    /** [(not implemented yet -- 7603)]
     *  Story Flag #504 (0x01F8) - JP from 805ACE45 0x01 to 805ACE45 0x08 / US from 805A9BC5 0x01 to 805A9BC5 0x08
     */
    STORYFLAG_504,

    /** [(not implemented yet -- 7645)]
     *  Story Flag #505 (0x01F9) - JP from 805ACE45 0x10 to 805ACE44 0x02 / US from 805A9BC5 0x10 to 805A9BC4 0x02
     */
    STORYFLAG_505,

    /** [(not implemented yet -- 7506)]
     *  Story Flag #506 (0x01FA) - JP from 805ACE43 0x01 to 805ACE43 0x40 / US from 805A9BC3 0x01 to 805A9BC3 0x40
     */
    STORYFLAG_506,

    /** [Talking to Gortram before beating Modly 2]
     *  Story Flag #507 (0x01FB) - JP 805ACD92 0x40 / US 805A9B12 0x40
     */
    STORYFLAG_507,

    /** [Sets after freeing the Timeshift Stone on Sandship]
     *  Story Flag #508 (0x01FC) - JP 805ACD92 0x80 / US 805A9B12 0x80
     */
    STORYFLAG_508,

    /** [Talking to the Thunder dragon for the first time after Healing him]
     *  Story Flag #509 (0x01FD) - JP 805ACD95 0x08 / US 805A9B15 0x08
     */
    STORYFLAG_509,

    /** [Sets after text from Thunder dragon after haveing your first Refight]
     *  Story Flag #510 (0x01FE) - JP 805ACD95 0x10 / US 805A9B15 0x10
     */
    STORYFLAG_510,

    /** [Sets after getting HP from Thunder dragon]
     *  Story Flag #511 (0x01FF) - JP 805ACD95 0x20 / US 805A9B15 0x20
     */
    STORYFLAG_511,

    /** [Sets after getting Hylian Shield from Thunder dragon]
     *  Story Flag #512 (0x0200) - JP 805ACD95 0x40 / US 805A9B15 0x40
     */
    STORYFLAG_512,

    /** [(not implemented yet -- 76A3)]
     *  Story Flag #513 (0x0201) - JP from 805ACE44 0x04 to 805ACE44 0x20 / US from 805A9BC4 0x04 to 805A9BC4 0x20
     */
    STORYFLAG_513,

    /** [(not implemented yet -- 7575)]
     *  Story Flag #514 (0x0202) - JP from 805ACE43 0x80 to 805ACE42 0x10 / US from 805A9BC3 0x80 to 805A9BC2 0x10
     */
    STORYFLAG_514,

    /** [(not implemented yet -- 7406)]
     *  Story Flag #515 (0x0203) - JP from 805ACE41 0x01 to 805ACE41 0x40 / US from 805A9BC1 0x01 to 805A9BC1 0x40
     */
    STORYFLAG_515,

    /** [(not implemented yet -- 7473)]
     *  Story Flag #516 (0x0204) - JP from 805ACE41 0x80 to 805ACE40 0x04 / US from 805A9BC1 0x80 to 805A9BC0 0x04
     */
    STORYFLAG_516,

    /** [(not implemented yet -- 7305)]
     *  Story Flag #517 (0x0205) - JP from 805ACE3F 0x01 to 805ACE3F 0x20 / US from 805A9BBF 0x01 to 805A9BBF 0x20
     */
    STORYFLAG_517,

    /** [(not implemented yet -- 7366)]
     *  Story Flag #518 (0x0206) - JP from 805ACE3F 0x40 to 805ACE3E 0x10 / US from 805A9BBF 0x40 to 805A9BBE 0x10
     */
    STORYFLAG_518,

    /** [Ancient Sea Chart activates]
     *  Story Flag #519 (0x0207) - JP 805ACD95 0x01 / US 805A9B15 0x01
     */
    STORYFLAG_SEA_CHART_ACTIVATED,

    /** [Skipper's Text in Sand Sea after Shipyard]
     *  Story Flag #520 (0x0208) - JP 805ACD95 0x02 / US 805A9B15 0x02
     */
    STORYFLAG_520,

    /** [Skipper's Text in Sand Sea after Pirate Stronghold]
     *  Story Flag #521 (0x0209) - JP 805ACD95 0x04 / US 805A9B15 0x04
     */
    STORYFLAG_521,

    /** [Sets before Boss Rush Starts]
     *  Story Flag #522 (0x020A) - JP 805ACD95 0x80 / US 805A9B15 0x80
     */
    STORYFLAG_522,

    /** [sets when redoing Silent realms]
     *  Story Flag #523 (0x020B) - JP 805ACD94 0x01 / US 805A9B14 0x01
     */
    STORYFLAG_523,

    /** [Talk to Thunder Dragon for first time (he needs medication)]
     *  Story Flag #524 (0x020C) - JP 805ACD94 0x02 / US 805A9B14 0x02
     */
    STORYFLAG_524,

    /** [Sets when giveing Golo Rupees for his diging]
     *  Story Flag #525 (0x020D) - JP 805ACD94 0x04 / US 805A9B14 0x04
     */
    STORYFLAG_525,

    /** [Lanayru Gorge Introductory Cutscene]
     *  Story Flag #526 (0x020E) - JP 805ACD94 0x08 / US 805A9B14 0x08
     */
    STORYFLAG_LANAYRU_GORGE_DISCOVERED,

    /** [Flooded Faron Woods Introductory Cutscene]
     *  Story Flag #527 (0x020F) - JP 805ACD94 0x10 / US 805A9B14 0x10
     */
    STORYFLAG_FLOODED_FARON_WOODS_DISCOVERED,

    /** [Finishing a Silent Realm for the First time]
     *  Story Flag #528 (0x0210) - JP 805ACD94 0x20 / US 805A9B14 0x20
     */
    STORYFLAG_528,

    /** [Starting the heart-stopping track in the minecart minigame (unset when doing scary track) ]
     *  Story Flag #529 (0x0211) - JP 805ACD94 0x40 / US 805A9B14 0x40
     */
    STORYFLAG_529,

    /** [Set and unset by Boss Rush (Hylia's Realm layer 1 / Skyview Boss Room layer 5 / FS Boss Room layer 3 / LMF Boss
     * Room layer 2 / Sandship Boss Room layer 2, ET Boss Room layer 3)] Story Flag #530 (0x0212) - JP 805ACD94 0x80 /
     * US 805A9B14 0x80
     */
    STORYFLAG_BOSS_RUSH_ACTIVE,

    /** [sets when redoing Silent realms]
     *  Story Flag #531 (0x0213) - JP 805ACD97 0x01 / US 805A9B17 0x01
     */
    STORYFLAG_531,

    /** []
     *  Story Flag #532 (0x0214) - JP 805ACD97 0x02 / US 805A9B17 0x02
     */
    STORYFLAG_532,

    /** [(not implemented yet -- 6D05)]
     *  Story Flag #533 (0x0215) - JP from 805ACE33 0x01 to 805ACE33 0x20 / US from 805A9BB3 0x01 to 805A9BB3 0x20
     */
    STORYFLAG_533,

    /** [(not implemented yet -- 7205)]
     *  Story Flag #534 (0x0216) - JP from 805ACE3D 0x01 to 805ACE3D 0x20 / US from 805A9BBD 0x01 to 805A9BBD 0x20
     */
    STORYFLAG_534,

    /** [(not implemented yet -- 7266)]
     *  Story Flag #535 (0x0217) - JP from 805ACE3D 0x40 to 805ACE3C 0x10 / US from 805A9BBD 0x40 to 805A9BBC 0x10
     */
    STORYFLAG_535,

    /** [(not implemented yet -- 6D65)]
     *  Story Flag #536 (0x0218) - JP from 805ACE33 0x40 to 805ACE32 0x08 / US from 805A9BB3 0x40 to 805A9BB2 0x08
     */
    STORYFLAG_536,

    /** [(not implemented yet -- 7145)]
     *  Story Flag #537 (0x0219) - JP from 805ACE3B 0x10 to 805ACE3A 0x02 / US from 805A9BBB 0x10 to 805A9BBA 0x02
     */
    STORYFLAG_537,

    /** [(not implemented yet -- 7006)]
     *  Story Flag #538 (0x021A) - JP from 805ACE39 0x01 to 805ACE39 0x40 / US from 805A9BB9 0x01 to 805A9BB9 0x40
     */
    STORYFLAG_538,

    /** [(not implemented yet -- 71A5)]
     *  Story Flag #539 (0x021B) - JP from 805ACE3A 0x04 to 805ACE3A 0x80 / US from 805A9BBA 0x04 to 805A9BBA 0x80
     */
    STORYFLAG_539,

    /** [(not implemented yet -- 7075)]
     *  Story Flag #540 (0x021C) - JP from 805ACE39 0x80 to 805ACE38 0x10 / US from 805A9BB9 0x80 to 805A9BB8 0x10
     */
    STORYFLAG_540,

    /** [(not implemented yet -- 6F06)]
     *  Story Flag #541 (0x021D) - JP from 805ACE37 0x01 to 805ACE37 0x40 / US from 805A9BB7 0x01 to 805A9BB7 0x40
     */
    STORYFLAG_541,

    /** [(not implemented yet -- 6F75)]
     *  Story Flag #542 (0x021E) - JP from 805ACE37 0x80 to 805ACE36 0x10 / US from 805A9BB7 0x80 to 805A9BB6 0x10
     */
    STORYFLAG_542,

    /** [(not implemented yet -- 6E05)]
     *  Story Flag #543 (0x021F) - JP from 805ACE35 0x01 to 805ACE35 0x20 / US from 805A9BB5 0x01 to 805A9BB5 0x20
     */
    STORYFLAG_543,

    /** [(not implemented yet -- 6E66)]
     *  Story Flag #544 (0x0220) - JP from 805ACE35 0x40 to 805ACE34 0x10 / US from 805A9BB5 0x40 to 805A9BB4 0x10
     */
    STORYFLAG_544,

    /** [Cutscene of Ghost asking for Help]
     *  Story Flag #545 (0x0221) - JP 805ACD97 0x04 / US 805A9B17 0x04
     */
    STORYFLAG_545,

    /** [Set after trying to open the Door to Ghost in Bathroom]
     *  Story Flag #546 (0x0222) - JP 805ACD97 0x08 / US 805A9B17 0x08
     */
    STORYFLAG_546,

    /** [Obtain Cawlin's Letter]
     *  Story Flag #547 (0x0223) - JP 805ACD97 0x10 / US 805A9B17 0x10
     */
    STORYFLAG_547,

    /** [Set when giving Cawlin's Letter to the toilet ghost]
     *  Story Flag #548 (0x0224) - JP 805ACD97 0x20 / US 805A9B17 0x20
     */
    STORYFLAG_548,

    /** [Set when giving Cawlin's Letter to the toilet ghost, unset when sleeping]
     *  Story Flag #549 (0x0225) - JP 805ACD97 0x40 / US 805A9B17 0x40
     */
    STORYFLAG_549,

    /** [See Toilet Hand for first time]
     *  Story Flag #550 (0x0226) - JP 805ACD97 0x80 / US 805A9B17 0x80
     */
    STORYFLAG_550,

    /** [Set when giving Cawlin's Letter to toilet ghost]
     *  Story Flag #551 (0x0227) - JP 805ACD96 0x01 / US 805A9B16 0x01
     */
    STORYFLAG_551,

    /** [Set when giving Cawlin's Letter to toilet ghost]
     *  Story Flag #552 (0x0228) - JP 805ACD96 0x02 / US 805A9B16 0x02
     */
    STORYFLAG_CAWLINS_LETTER_GHOST,

    /** [Toilet Hand 5 crystals collected]
     *  Story Flag #553 (0x0229) - JP 805ACD96 0x04 / US 805A9B16 0x04
     */
    STORYFLAG_553,

    /** [Cawlin runs away crying after giving letter to toilet ghost/Set by Pipit giving 5 crystals]
     *  Story Flag #554 (0x022A) - JP 805ACD96 0x08 / US 805A9B16 0x08
     */
    STORYFLAG_554,

    /** [Giving Cawlin's Letter to Karane]
     *  Story Flag #555 (0x022B) - JP 805ACD96 0x10 / US 805A9B16 0x10
     */
    STORYFLAG_CAWLINS_LETTER_KARANE,

    /** [Talking to Pipit about Cawlin's Letter]
     *  Story Flag #556 (0x022C) - JP 805ACD96 0x20 / US 805A9B16 0x20
     */
    STORYFLAG_556,

    /** [Set before Pipit gives his Crystals]
     *  Story Flag #557 (0x022D) - JP 805ACD96 0x40 / US 805A9B16 0x40
     */
    STORYFLAG_557,

    /** [Pipit 5 crystals collected]
     *  Story Flag #558 (0x022E) - JP 805ACD96 0x80 / US 805A9B16 0x80
     */
    STORYFLAG_558,

    /** [Set after talking to Pipit about Cawlin's Letter]
     *  Story Flag #559 (0x022F) - JP 805ACD99 0x01 / US 805A9B19 0x01
     */
    STORYFLAG_559,

    /** [Set by Pipit's 5 crystals]
     *  Story Flag #560 (0x0230) - JP 805ACD99 0x02 / US 805A9B19 0x02
     */
    STORYFLAG_560,

    /** [Set by Pipit's 5 crystals]
     *  Story Flag #561 (0x0231) - JP 805ACD99 0x04 / US 805A9B19 0x04
     */
    STORYFLAG_561,

    /** [Volcano Summit Introductory Cutscene]
     *  Story Flag #562 (0x0232) - JP 805ACD99 0x08 / US 805A9B19 0x08
     */
    STORYFLAG_VOLCANO_SUMMIT_DISCOVERED,

    /** [(not implemented yet -- 74B3)]
     *  Story Flag #563 (0x0233) - JP from 805ACE40 0x08 to 805ACE40 0x40 / US from 805A9BC0 0x08 to 805A9BC0 0x40
     */
    STORYFLAG_563,

    /** [(not implemented yet -- 7AD1)]
     *  Story Flag #564 (0x0234) - JP from 805ACE4C 0x20 to 805ACE4C 0x40 / US from 805A9BCC 0x20 to 805A9BCC 0x40
     */
    STORYFLAG_564,

    /** [Z Button bipping (can be unset)]
     *  Story Flag #565 (0x0235) - JP 805ACD99 0x10 / US 805A9B19 0x10
     */
    STORYFLAG_Z_BTN_NOTICE,

    /** [C Button Bipping (can be unset)]
     *  Story Flag #566 (0x0236) - JP 805ACD99 0x20 / US 805A9B19 0x20
     */
    STORYFLAG_C_BTN_NOTICE,

    /** [Map Button Bipping (can be unset)]
     *  Story Flag #567 (0x0237) - JP 805ACD99 0x40 / US 805A9B19 0x40
     */
    STORYFLAG_PLUS_BTN_NOTICE,

    /** [Pouch Button bipping (can be unset)]
     *  Story Flag #568 (0x0238) - JP 805ACD99 0x80 / US 805A9B19 0x80
     */
    STORYFLAG_MINUS_BTN_NOTICE,

    /** [B Button bipping (can be unset)]
     *  Story Flag #569 (0x0239) - JP 805ACD98 0x01 / US 805A9B18 0x01
     */
    STORYFLAG_B_BTN_NOTICE,

    /** [Interface selection bipping (can be unset)]
     *  Story Flag #570 (0x023A) - JP 805ACD98 0x02 / US 805A9B18 0x02
     */
    STORYFLAG_UI_MODE_NOTICE,

    /** [Gear button bipping (can be unset)]
     *  Story Flag #571 (0x023B) - JP 805ACD98 0x04 / US 805A9B18 0x04
     */
    STORYFLAG_1_BTN_NOTICE,

    /** [In Boss Rush mode (can be unset)]
     *  Story Flag #572 (0x023C) - JP 805ACD98 0x08 / US 805A9B18 0x08
     */
    STORYFLAG_BOSSRUSH_ACTIVE,

    /** [In redoing slient realm mode?]
     *  Story Flag #573 (0x023D) - JP 805ACD98 0x10 / US 805A9B18 0x10
     */
    STORYFLAG_573,

    /** [Set when accepting to clean Pipit House]
     *  Story Flag #574 (0x023E) - JP 805ACD98 0x20 / US 805A9B18 0x20
     */
    STORYFLAG_574,

    /** [5 gratitude crystal from Pipit's Mom collected (not the crystals)]
     *  Story Flag #575 (0x023F) - JP 805ACD98 0x40 / US 805A9B18 0x40
     */
    STORYFLAG_575,

    /** [Set after pipit taks to link after scolding his mom]
     *  Story Flag #576 (0x0240) - JP 805ACD98 0x80 / US 805A9B18 0x80
     */
    STORYFLAG_576,

    /** [Sets at the end of pipit scolding his mom]
     *  Story Flag #577 (0x0241) - JP 805ACD9B 0x01 / US 805A9B1B 0x01
     */
    STORYFLAG_577,

    /** [Set after pipit taks to link after scolding his mom]
     *  Story Flag #578 (0x0242) - JP 805ACD9B 0x02 / US 805A9B1B 0x02
     */
    STORYFLAG_578,

    /** [Talk to Pipit after telling him you saw him scolding his mom]
     *  Story Flag #579 (0x0243) - JP 805ACD9B 0x04 / US 805A9B1B 0x04
     */
    STORYFLAG_579,

    /** [Talk to Marrella after you saw her getting scold by pipit]
     *  Story Flag #580 (0x0244) - JP 805ACD9B 0x08 / US 805A9B1B 0x08
     */
    STORYFLAG_580,

    /** [Set after collecting 20 rupees from Pipit's Mom?]
     *  Story Flag #581 (0x0245) - JP 805ACD9B 0x10 / US 805A9B1B 0x10
     */
    STORYFLAG_581,

    /** [Set when beating Ancient Cistern]
     *  Story Flag #582 (0x0246) - JP 805ACD9B 0x20 / US 805A9B1B 0x20
     */
    STORYFLAG_582,

    /** [obtaining the Goddess White Sword (Unlocks 4 new dowsing slots)]
     *  Story Flag #583 (0x0247) - JP 805ACD9B 0x40 / US 805A9B1B 0x40
     */
    STORYFLAG_UPGRADED_DOWSING,

    /** [(not implemented yet -- 6C05)]
     *  Story Flag #584 (0x0248) - JP from 805ACE31 0x01 to 805ACE31 0x20 / US from 805A9BB1 0x01 to 805A9BB1 0x20
     */
    STORYFLAG_584,

    /** [(not implemented yet -- 6C65)]
     *  Story Flag #585 (0x0249) - JP from 805ACE31 0x40 to 805ACE30 0x08 / US from 805A9BB1 0x40 to 805A9BB0 0x08
     */
    STORYFLAG_585,

    /** [(not implemented yet -- 6B06)]
     *  Story Flag #586 (0x024A) - JP from 805ACE2F 0x01 to 805ACE2F 0x40 / US from 805A9BAF 0x01 to 805A9BAF 0x40
     */
    STORYFLAG_586,

    /** [(not implemented yet -- 6B75)]
     *  Story Flag #587 (0x024B) - JP from 805ACE2F 0x80 to 805ACE2E 0x10 / US from 805A9BAF 0x80 to 805A9BAE 0x10
     */
    STORYFLAG_587,

    /** [(not implemented yet -- 6A05)]
     *  Story Flag #588 (0x024C) - JP from 805ACE2D 0x01 to 805ACE2D 0x20 / US from 805A9BAD 0x01 to 805A9BAD 0x20
     */
    STORYFLAG_588,

    /** [(not implemented yet -- 6A66)]
     *  Story Flag #589 (0x024D) - JP from 805ACE2D 0x40 to 805ACE2C 0x10 / US from 805A9BAD 0x40 to 805A9BAC 0x10
     */
    STORYFLAG_589,

    /** [(not implemented yet -- 6905)]
     *  Story Flag #590 (0x024E) - JP from 805ACE2B 0x01 to 805ACE2B 0x20 / US from 805A9BAB 0x01 to 805A9BAB 0x20
     */
    STORYFLAG_590,

    /** [(not implemented yet -- 6965)]
     *  Story Flag #591 (0x024F) - JP from 805ACE2B 0x40 to 805ACE2A 0x08 / US from 805A9BAB 0x40 to 805A9BAA 0x08
     */
    STORYFLAG_591,

    /** [(not implemented yet -- 6806)]
     *  Story Flag #592 (0x0250) - JP from 805ACE29 0x01 to 805ACE29 0x40 / US from 805A9BA9 0x01 to 805A9BA9 0x40
     */
    STORYFLAG_592,

    /** [(not implemented yet -- 6875)]
     *  Story Flag #593 (0x0251) - JP from 805ACE29 0x80 to 805ACE28 0x10 / US from 805A9BA9 0x80 to 805A9BA8 0x10
     */
    STORYFLAG_593,

    /** [(not implemented yet -- 6705)]
     *  Story Flag #594 (0x0252) - JP from 805ACE27 0x01 to 805ACE27 0x20 / US from 805A9BA7 0x01 to 805A9BA7 0x20
     */
    STORYFLAG_594,

    /** [(not implemented yet -- 6766)]
     *  Story Flag #595 (0x0253) - JP from 805ACE27 0x40 to 805ACE26 0x10 / US from 805A9BA7 0x40 to 805A9BA6 0x10
     */
    STORYFLAG_595,

    /** [(not implemented yet -- 6605)]
     *  Story Flag #596 (0x0254) - JP from 805ACE25 0x01 to 805ACE25 0x20 / US from 805A9BA5 0x01 to 805A9BA5 0x20
     */
    STORYFLAG_596,

    /** [(not implemented yet -- 6665)]
     *  Story Flag #597 (0x0255) - JP from 805ACE25 0x40 to 805ACE24 0x08 / US from 805A9BA5 0x40 to 805A9BA4 0x08
     */
    STORYFLAG_597,

    /** [(not implemented yet -- 6506)]
     *  Story Flag #598 (0x0256) - JP from 805ACE23 0x01 to 805ACE23 0x40 / US from 805A9BA3 0x01 to 805A9BA3 0x40
     */
    STORYFLAG_598,

    /** [(not implemented yet -- 6575)]
     *  Story Flag #599 (0x0257) - JP from 805ACE23 0x80 to 805ACE22 0x10 / US from 805A9BA3 0x80 to 805A9BA2 0x10
     */
    STORYFLAG_599,

    /** [(not implemented yet -- 6405)]
     *  Story Flag #600 (0x0258) - JP from 805ACE21 0x01 to 805ACE21 0x20 / US from 805A9BA1 0x01 to 805A9BA1 0x20
     */
    STORYFLAG_600,

    /** [(not implemented yet -- 6466)]
     *  Story Flag #601 (0x0259) - JP from 805ACE21 0x40 to 805ACE20 0x10 / US from 805A9BA1 0x40 to 805A9BA0 0x10
     */
    STORYFLAG_601,

    /** [(not implemented yet -- 6305)]
     *  Story Flag #602 (0x025A) - JP from 805ACE1F 0x01 to 805ACE1F 0x20 / US from 805A9B9F 0x01 to 805A9B9F 0x20
     */
    STORYFLAG_602,

    /** [(not implemented yet -- 6365)]
     *  Story Flag #603 (0x025B) - JP from 805ACE1F 0x40 to 805ACE1E 0x08 / US from 805A9B9F 0x40 to 805A9B9E 0x08
     */
    STORYFLAG_603,

    /** [(not implemented yet -- 6206)]
     *  Story Flag #604 (0x025C) - JP from 805ACE1D 0x01 to 805ACE1D 0x40 / US from 805A9B9D 0x01 to 805A9B9D 0x40
     */
    STORYFLAG_604,

    /** [(not implemented yet -- 6275)]
     *  Story Flag #605 (0x025D) - JP from 805ACE1D 0x80 to 805ACE1C 0x10 / US from 805A9B9D 0x80 to 805A9B9C 0x10
     */
    STORYFLAG_605,

    /** [(not implemented yet -- 6105)]
     *  Story Flag #606 (0x025E) - JP from 805ACE1B 0x01 to 805ACE1B 0x20 / US from 805A9B9B 0x01 to 805A9B9B 0x20
     */
    STORYFLAG_606,

    /** [(not implemented yet -- 6166)]
     *  Story Flag #607 (0x025F) - JP from 805ACE1B 0x40 to 805ACE1A 0x10 / US from 805A9B9B 0x40 to 805A9B9A 0x10
     */
    STORYFLAG_607,

    /** [(not implemented yet -- 6005)]
     *  Story Flag #608 (0x0260) - JP from 805ACE19 0x01 to 805ACE19 0x20 / US from 805A9B99 0x01 to 805A9B99 0x20
     */
    STORYFLAG_608,

    /** [(not implemented yet -- 6065)]
     *  Story Flag #609 (0x0261) - JP from 805ACE19 0x40 to 805ACE18 0x08 / US from 805A9B99 0x40 to 805A9B98 0x08
     */
    STORYFLAG_609,

    /** [(not implemented yet -- 5F06)]
     *  Story Flag #610 (0x0262) - JP from 805ACE17 0x01 to 805ACE17 0x40 / US from 805A9B97 0x01 to 805A9B97 0x40
     */
    STORYFLAG_610,

    /** [(not implemented yet -- 5F75)]
     *  Story Flag #611 (0x0263) - JP from 805ACE17 0x80 to 805ACE16 0x10 / US from 805A9B97 0x80 to 805A9B96 0x10
     */
    STORYFLAG_611,

    /** [(not implemented yet -- 5E05)]
     *  Story Flag #612 (0x0264) - JP from 805ACE15 0x01 to 805ACE15 0x20 / US from 805A9B95 0x01 to 805A9B95 0x20
     */
    STORYFLAG_612,

    /** [(not implemented yet -- 5E66)]
     *  Story Flag #613 (0x0265) - JP from 805ACE15 0x40 to 805ACE14 0x10 / US from 805A9B95 0x40 to 805A9B94 0x10
     */
    STORYFLAG_613,

    /** [(not implemented yet -- 5D05)]
     *  Story Flag #614 (0x0266) - JP from 805ACE13 0x01 to 805ACE13 0x20 / US from 805A9B93 0x01 to 805A9B93 0x20
     */
    STORYFLAG_614,

    /** [(not implemented yet -- 5D65)]
     *  Story Flag #615 (0x0267) - JP from 805ACE13 0x40 to 805ACE12 0x08 / US from 805A9B93 0x40 to 805A9B92 0x08
     */
    STORYFLAG_615,

    /** [(not implemented yet -- 5C06)]
     *  Story Flag #616 (0x0268) - JP from 805ACE11 0x01 to 805ACE11 0x40 / US from 805A9B91 0x01 to 805A9B91 0x40
     */
    STORYFLAG_616,

    /** [(not implemented yet -- 5C75)]
     *  Story Flag #617 (0x0269) - JP from 805ACE11 0x80 to 805ACE10 0x10 / US from 805A9B91 0x80 to 805A9B90 0x10
     */
    STORYFLAG_617,

    /** [(not implemented yet -- 5B05)]
     *  Story Flag #618 (0x026A) - JP from 805ACE0F 0x01 to 805ACE0F 0x20 / US from 805A9B8F 0x01 to 805A9B8F 0x20
     */
    STORYFLAG_618,

    /** [(not implemented yet -- 5B66)]
     *  Story Flag #619 (0x026B) - JP from 805ACE0F 0x40 to 805ACE0E 0x10 / US from 805A9B8F 0x40 to 805A9B8E 0x10
     */
    STORYFLAG_619,

    /** [(not implemented yet -- 5A05)]
     *  Story Flag #620 (0x026C) - JP from 805ACE0D 0x01 to 805ACE0D 0x20 / US from 805A9B8D 0x01 to 805A9B8D 0x20
     */
    STORYFLAG_620,

    /** [(not implemented yet -- 5A65)]
     *  Story Flag #621 (0x026D) - JP from 805ACE0D 0x40 to 805ACE0C 0x08 / US from 805A9B8D 0x40 to 805A9B8C 0x08
     */
    STORYFLAG_621,

    /** [(not implemented yet -- 5906)]
     *  Story Flag #622 (0x026E) - JP from 805ACE0B 0x01 to 805ACE0B 0x40 / US from 805A9B8B 0x01 to 805A9B8B 0x40
     */
    STORYFLAG_622,

    /** [(not implemented yet -- 5975)]
     *  Story Flag #623 (0x026F) - JP from 805ACE0B 0x80 to 805ACE0A 0x10 / US from 805A9B8B 0x80 to 805A9B8A 0x10
     */
    STORYFLAG_623,

    /** [(not implemented yet -- 5805)]
     *  Story Flag #624 (0x0270) - JP from 805ACE09 0x01 to 805ACE09 0x20 / US from 805A9B89 0x01 to 805A9B89 0x20
     */
    STORYFLAG_624,

    /** [(not implemented yet -- 5866)]
     *  Story Flag #625 (0x0271) - JP from 805ACE09 0x40 to 805ACE08 0x10 / US from 805A9B89 0x40 to 805A9B88 0x10
     */
    STORYFLAG_625,

    /** [(not implemented yet -- 550D)]
     *  Story Flag #626 (0x0272) - JP from 805ACE03 0x01 to 805ACE02 0x20 / US from 805A9B83 0x01 to 805A9B82 0x20
     */
    STORYFLAG_626,

    /** [(not implemented yet -- 570D)]
     *  Story Flag #627 (0x0273) - JP from 805ACE07 0x01 to 805ACE06 0x20 / US from 805A9B87 0x01 to 805A9B86 0x20
     */
    STORYFLAG_627,

    /** [Talk to Peatrice at the bazaar 2nd time (need Peatrice Switch to be 1)]
     *  Story Flag #628 (0x0274) - JP 805ACD9B 0x80 / US 805A9B1B 0x80
     */
    STORYFLAG_628,

    /** [Talk to Peatrice in the Bazaar 4th time (need 9Bx80 and Peatrice Switch to be 1)]
     *  Story Flag #629 (0x0275) - JP 805ACD9A 0x01 / US 805A9B1A 0x01
     */
    STORYFLAG_629,

    /** [Talk to Peatrice a 6th time in the Bazaar (need 9Ax01 and Peatrice Switch to be 1)]
     *  Story Flag #630 (0x0276) - JP 805ACD9A 0x02 / US 805A9B1A 0x02
     */
    STORYFLAG_630,

    /** [(not implemented yet -- 7103)]
     *  Story Flag #631 (0x0277) - JP from 805ACE3B 0x01 to 805ACE3B 0x08 / US from 805A9BBB 0x01 to 805A9BBB 0x08
     */
    STORYFLAG_631,

    /** [Set when Talking to Peatrice, unsets when sleeping (prevents you from getting more convos)]
     *  Story Flag #632 (0x0278) - JP 805ACD9A 0x04 / US 805A9B1A 0x04
     */
    STORYFLAG_632,

    /** [Start Peater's Quest to protect Peatrice]
     *  Story Flag #633 (0x0279) - JP 805ACD9A 0x08 / US 805A9B1A 0x08
     */
    STORYFLAG_633,

    /** [Peatrice ask you to meet her at her House at Night (needs 9Ax08)]
     *  Story Flag #634 (0x027A) - JP 805ACD9A 0x10 / US 805A9B1A 0x10
     */
    STORYFLAG_634,

    /** [Finish talking to Peatrice at night (needs 9Ax10)]
     *  Story Flag #635 (0x027B) - JP 805ACD9A 0x20 / US 805A9B1A 0x20
     */
    STORYFLAG_635,

    /** [5 crystals from Peatrice collected]
     *  Story Flag #636 (0x027C) - JP 805ACD9A 0x40 / US 805A9B1A 0x40
     */
    STORYFLAG_636,

    /** [Talking to Peatrice in Bazaar after getting her crystals]
     *  Story Flag #637 (0x027D) - JP 805ACD9A 0x80 / US 805A9B1A 0x80
     */
    STORYFLAG_637,

    /** [Sleeping after talking to Peatrice at night (requires 9Ax20)]
     *  Story Flag #638 (0x027E) - JP 805ACD9D 0x01 / US 805A9B1D 0x01
     */
    STORYFLAG_638,

    /** [5 crystals from Peater collected]
     *  Story Flag #639 (0x027F) - JP 805ACD9D 0x02 / US 805A9B1D 0x02
     */
    STORYFLAG_639,

    /** [(not implemented yet -- 5604)]
     *  Story Flag #640 (0x0280) - JP from 805ACE05 0x01 to 805ACE05 0x10 / US from 805A9B85 0x01 to 805A9B85 0x10
     */
    STORYFLAG_640,

    /** [Spiral Charge Training Introductory Cutscene]
     *  Story Flag #641 (0x0281) - JP 805ACD9D 0x04 / US 805A9B1D 0x04
     */
    STORYFLAG_641,

    /** [Spiral Charge Training Complete]
     *  Story Flag #642 (0x0282) - JP 805ACD9D 0x08 / US 805A9B1D 0x08
     */
    STORYFLAG_642,

    /** [Prof Owlan ready to teach Spiral Charge (can be unset) (Sky layer 6) ]
     *  Story Flag #643 (0x0283) - JP 805ACD9D 0x10 / US 805A9B1D 0x10
     */
    STORYFLAG_643,

    /** [Talk to Peater for first time]
     *  Story Flag #644 (0x0284) - JP 805ACD9D 0x20 / US 805A9B1D 0x20
     */
    STORYFLAG_644,

    /** [Set when beating Sky Keep]
     *  Story Flag #645 (0x0285) - JP 805ACD9D 0x40 / US 805A9B1D 0x40
     */
    STORYFLAG_645,

    /** [[Was intended to set after beating Sky keep in the triforce room on layer 5]]
     *  Story Flag #646 (0x0286) - JP 805ACD9D 0x80 / US 805A9B1D 0x80
     */
    STORYFLAG_646,

    /** [Pirate Stronghold Camera panning cutscene before Ship Dowsing]
     *  Story Flag #647 (0x0287) - JP 805ACD9C 0x01 / US 805A9B1C 0x01
     */
    STORYFLAG_647,

    /** [Koloktos defeated]
     *  Story Flag #648 (0x0288) - JP 805ACD9C 0x02 / US 805A9B1C 0x02
     */
    STORYFLAG_648,

    /** [At least one goddess cube (triggers Fi text about goddess chests in the sky?)]
     *  Story Flag #649 (0x0289) - JP 805ACD9C 0x04 / US 805A9B1C 0x04
     */
    STORYFLAG_649,

    /** []
     *  Story Flag #650 (0x028A) - JP 805ACD9C 0x08 / US 805A9B1C 0x08
     */
    STORYFLAG_650,

    /** []
     *  Story Flag #651 (0x028B) - JP 805ACD9C 0x10 / US 805A9B1C 0x10
     */
    STORYFLAG_651,

    /** [(not implemented yet -- 79D1)]
     *  Story Flag #652 (0x028C) - JP from 805ACE4A 0x20 to 805ACE4A 0x40 / US from 805A9BCA 0x20 to 805A9BCA 0x40
     */
    STORYFLAG_652,

    /** [[unsets when sleeping to Night] [can be set before Skyview]]
     *  Story Flag #653 (0x028D) - JP 805ACD9C 0x20 / US 805A9B1C 0x20
     */
    STORYFLAG_653,

    /** [(not implemented yet -- 6DC3)]
     *  Story Flag #654 (0x028E) - JP from 805ACE32 0x10 to 805ACE32 0x80 / US from 805A9BB2 0x10 to 805A9BB2 0x80
     */
    STORYFLAG_654,

    /** [(not implemented yet -- 78A1)]
     *  Story Flag #655 (0x028F) - JP from 805ACE48 0x04 to 805ACE48 0x08 / US from 805A9BC8 0x04 to 805A9BC8 0x08
     */
    STORYFLAG_655,

    /** [Place the Timeshift Orb on the pedestal to open the Shark Head (Sand Sea layer 2 / Pirate Stronghold layer 1)]
     *  Story Flag #656 (0x0290) - JP 805ACD9C 0x40 / US 805A9B1C 0x40
     */
    STORYFLAG_656,

    /** [Sets when entering the Sandship miniboss room (unset after leaveing)]
     *  Story Flag #657 (0x0291) - JP 805ACD9C 0x80 / US 805A9B1C 0x80
     */
    STORYFLAG_657,

    /** [Sets when entering the BK room in Sandship (unsets after leaveing)]
     *  Story Flag #658 (0x0292) - JP 805ACD9F 0x01 / US 805A9B1F 0x01
     */
    STORYFLAG_658,

    /** [Sets when entering the Boss Door room in Sandship (unsets after leaveing)]
     *  Story Flag #659 (0x0293) - JP 805ACD9F 0x02 / US 805A9B1F 0x02
     */
    STORYFLAG_659,

    /** []
     *  Story Flag #660 (0x0294) - JP 805ACD9F 0x04 / US 805A9B1F 0x04
     */
    STORYFLAG_660,

    /** [Village Windmill Repaired (Required to start Scrapper-related sidequests)]
     *  Story Flag #661 (0x0295) - JP 805ACD9F 0x08 / US 805A9B1F 0x08
     */
    STORYFLAG_661,

    /** [Light Tower Raised]
     *  Story Flag #662 (0x0296) - JP 805ACD9F 0x10 / US 805A9B1F 0x10
     */
    STORYFLAG_662,

    /** [Set when giving Cawlin's Letter to toilet ghost/Set after Pipit gives 5 crystals]
     *  Story Flag #663 (0x0297) - JP 805ACD9F 0x20 / US 805A9B1F 0x20
     */
    STORYFLAG_663,

    /** [Talk to Cawlin after giveing the letter to Karane]
     *  Story Flag #664 (0x0298) - JP 805ACD9F 0x40 / US 805A9B1F 0x40
     */
    STORYFLAG_664,

    /** []
     *  Story Flag #665 (0x0299) - JP 805ACD9F 0x80 / US 805A9B1F 0x80
     */
    STORYFLAG_665,

    /** [Sandship escape sequence starts when entering boss]
     *  Story Flag #666 (0x029A) - JP 805ACD9E 0x01 / US 805A9B1E 0x01
     */
    STORYFLAG_666,

    /** [Getting the fastest time at minecrat minigame after already getting the Heart piece]
     *  Story Flag #667 (0x029B) - JP 805ACD9E 0x02 / US 805A9B1E 0x02
     */
    STORYFLAG_667,

    /** [Propeller Dowsing]
     *  Story Flag #668 (0x029C) - JP 805ACD9E 0x04 / US 805A9B1E 0x04
     */
    STORYFLAG_PROPELLER_DOWSING,

    /** [Water Basin Dowsing]
     *  Story Flag #669 (0x029D) - JP 805ACD9E 0x08 / US 805A9B1E 0x08
     */
    STORYFLAG_WATER_BASIN_DOWSING,

    /** [Crystal Ball Dowsing (unsets after you get the Ball)]
     *  Story Flag #670 (0x029E) - JP 805ACD9E 0x10 / US 805A9B1E 0x10
     */
    STORYFLAG_CRYSTAL_BALL_DOWSING,

    /** [(not implemented yet -- 78C2)]
     *  Story Flag #671 (0x029F) - JP from 805ACE48 0x10 to 805ACE48 0x40 / US from 805A9BC8 0x10 to 805A9BC8 0x40
     */
    STORYFLAG_671,

    /** [Goddess Statue Opened]
     *  Story Flag #672 (0x02A0) - JP 805ACDA0 0x02 / US 805A9B20 0x02
     */
    STORYFLAG_672,

    /** [Set when Scrapper picks up Oolo]
     *  Story Flag #673 (0x02A1) - JP 805ACDA0 0x04 / US 805A9B20 0x04
     */
    STORYFLAG_673,

    /** [set when repairing]
     *  Story Flag #674 (0x02A2) - JP 805ACDA0 0x08 / US 805A9B20 0x08
     */
    STORYFLAG_674,

    /** [Set whent 3rd Quest of Pumm starts (play harp with Kina) (unsets after you finish)]
     *  Story Flag #675 (0x02A3) - JP 805ACDA0 0x10 / US 805A9B20 0x10
     */
    STORYFLAG_675,

    /** [Talk to Mogma for hint in Waterfall area of Volcano Summit]
     *  Story Flag #676 (0x02A4) - JP 805ACDA0 0x20 / US 805A9B20 0x20
     */
    STORYFLAG_676,

    /** [Water Basin Collected (Eldin Volcano, Digging Mitts Cave, FS Entrance, Volcano Summit Waterfall layer 3)]
     *  Story Flag #677 (0x02A5) - JP 805ACDA0 0x40 / US 805A9B20 0x40
     */
    STORYFLAG_677,

    /** [Entering Sky Keep Cutscene]
     *  Story Flag #678 (0x02A6) - JP 805ACDA2 0x01 / US 805A9B22 0x01
     */
    STORYFLAG_678,

    /** [Set after Fi text before going to sky for first time (also sets after sleeping)]
     *  Story Flag #679 (0x02A7) - JP 805ACDA2 0x02 / US 805A9B22 0x02
     */
    STORYFLAG_679,

    /** [Pumpkin Dowsing (unsets you talk to Guld)]
     *  Story Flag #680 (0x02A8) - JP 805ACD9E 0x20 / US 805A9B1E 0x20
     */
    STORYFLAG_PUMPKIN_DOWSING,

    /** [New Plant Species Dowsing (unsets after you find Oolo)]
     *  Story Flag #681 (0x02A9) - JP 805ACD9E 0x40 / US 805A9B1E 0x40
     */
    STORYFLAG_KIKWI_SPECIES_DOWSING,

    /** [Party Wheel Dowsing (unsets after you find the wheel)]
     *  Story Flag #682 (0x02AA) - JP 805ACD9E 0x80 / US 805A9B1E 0x80
     */
    STORYFLAG_PARTY_WHEEL_DOWSING,

    /** []
     *  Story Flag #683 (0x02AB) - JP 805ACDA1 0x01 / US 805A9B21 0x01
     */
    STORYFLAG_683,

    /** [Starting the Bug Wrangler game at Bug island]
     *  Story Flag #684 (0x02AC) - JP 805ACDA1 0x02 / US 805A9B21 0x02
     */
    STORYFLAG_684,

    /** [Fi Text about control Panel in Sky Keep after moveing rooms the first time]
     *  Story Flag #685 (0x02AD) - JP 805ACDA1 0x04 / US 805A9B21 0x04
     */
    STORYFLAG_685,

    /** [Sets at the Start of Scaldera cs]
     *  Story Flag #686 (0x02AE) - JP 805ACDA1 0x08 / US 805A9B21 0x08
     */
    STORYFLAG_686,

    /** [Set after Scrapper picks up Guld]
     *  Story Flag #687 (0x02AF) - JP 805ACDA1 0x10 / US 805A9B21 0x10
     */
    STORYFLAG_687,

    /** [Reloading somehow after Finishing Pumm's 3rd Quest (play harp with Kina) (activates the new chandelier CS)]
     *  Story Flag #688 (0x02B0) - JP 805ACDA1 0x20 / US 805A9B21 0x20
     */
    STORYFLAG_688,

    /** [Finish talking to Peatrice at the bazaar 2nd time (clears question)]
     *  Story Flag #689 (0x02B1) - JP 805ACDA1 0x40 / US 805A9B21 0x40
     */
    STORYFLAG_689,

    /** [Finish talking to Peatrice for the 4th time (clears question)]
     *  Story Flag #690 (0x02B2) - JP 805ACDA1 0x80 / US 805A9B21 0x80
     */
    STORYFLAG_690,

    /** [Finish Talking to Peatrice 6th time (clears question)]
     *  Story Flag #691 (0x02B3) - JP 805ACDA0 0x01 / US 805A9B20 0x01
     */
    STORYFLAG_691,

    /** [(not implemented yet -- 7771)]
     *  Story Flag #692 (0x02B4) - JP from 805ACE47 0x80 to 805ACE46 0x01 / US from 805A9BC7 0x80 to 805A9BC6 0x01
     */
    STORYFLAG_692,

    /** [Going near first door in Sky keep (Servo Room Layer 2)]
     *  Story Flag #693 (0x02B5) - JP 805ACDA0 0x80 / US 805A9B20 0x80
     */
    STORYFLAG_693,

    /** [Sets when beeing in AC Room in Skykeep]
     *  Story Flag #694 (0x02B6) - JP 805ACDA3 0x01 / US 805A9B23 0x01
     */
    STORYFLAG_694,

    /** [Sets when beeing in ET Room in Skykeep]
     *  Story Flag #695 (0x02B7) - JP 805ACDA3 0x02 / US 805A9B23 0x02
     */
    STORYFLAG_695,

    /** [Sets when beeing in FS Room in Skykeep]
     *  Story Flag #696 (0x02B8) - JP 805ACDA3 0x04 / US 805A9B23 0x04
     */
    STORYFLAG_696,

    /** [Sets when beeing in Sandship Room in Skykeep]
     *  Story Flag #697 (0x02B9) - JP 805ACDA3 0x08 / US 805A9B23 0x08
     */
    STORYFLAG_697,

    /** [Sets when beeing in LMF Room in Skykeep]
     *  Story Flag #698 (0x02BA) - JP 805ACDA3 0x10 / US 805A9B23 0x10
     */
    STORYFLAG_698,

    /** [Sets when beeing in Skyview Room in Skykeep]
     *  Story Flag #699 (0x02BB) - JP 805ACDA3 0x20 / US 805A9B23 0x20
     */
    STORYFLAG_699,

    /** [Sets when beeing in Miniboss Room in Skykeep]
     *  Story Flag #700 (0x02BC) - JP 805ACDA3 0x40 / US 805A9B23 0x40
     */
    STORYFLAG_700,

    /** [Sets when beeing in the Entrance Room in Skykeep]
     *  Story Flag #701 (0x02BD) - JP 805ACDA3 0x80 / US 805A9B23 0x80
     */
    STORYFLAG_701,

    /** [[Set by cs in Sandship]]
     *  Story Flag #702 (0x02BE) - JP 805ACDA2 0x04 / US 805A9B22 0x04
     */
    STORYFLAG_702,

    /** [Sets when you refight imp 1 (Sealed Grounds layer 2)]
     *  Story Flag #703 (0x02BF) - JP 805ACDA2 0x08 / US 805A9B22 0x08
     */
    STORYFLAG_IMPRISONED1_BOSSRUSH,

    /** [Sets when you refight imp 2 (Sealed Grounds layer 3)]
     *  Story Flag #704 (0x02C0) - JP 805ACDA2 0x10 / US 805A9B22 0x10
     */
    STORYFLAG_IMPRISONED2_BOSSRUSH,

    /** [Sets when you refight imp3 (Sealed Grounds layer 4)]
     *  Story Flag #705 (0x02C1) - JP 805ACDA2 0x20 / US 805A9B22 0x20
     */
    STORYFLAG_IMPRISONED3_BOSSRUSH,

    /** [Minecart Escort done (Lanayru Gorge layer 2)]
     *  Story Flag #706 (0x02C2) - JP 805ACDA2 0x40 / US 805A9B22 0x40
     */
    STORYFLAG_706,

    /** [(not implemented yet -- 7791)]
     *  Story Flag #707 (0x02C3) - JP from 805ACE46 0x02 to 805ACE46 0x04 / US from 805A9BC6 0x02 to 805A9BC6 0x04
     */
    STORYFLAG_707,

    /** [(not implemented yet -- 77B1)]
     *  Story Flag #708 (0x02C4) - JP from 805ACE46 0x08 to 805ACE46 0x10 / US from 805A9BC6 0x08 to 805A9BC6 0x10
     */
    STORYFLAG_708,

    /** [Enter Beedle's Shop for first time]
     *  Story Flag #709 (0x02C5) - JP 805ACDA2 0x80 / US 805A9B22 0x80
     */
    STORYFLAG_BEEDLE_SHOP_ENTERED,

    /** [Sets after Treasure dowsing]
     *  Story Flag #710 (0x02C6) - JP 805ACDA5 0x01 / US 805A9B25 0x01
     */
    STORYFLAG_710,

    /** []
     *  Story Flag #711 (0x02C7) - JP 805ACDA5 0x02 / US 805A9B25 0x02
     */
    STORYFLAG_711,

    /** [Item Check Girl Peatrice's House entered (name on map appears)]
     *  Story Flag #712 (0x02C8) - JP 805ACDA5 0x04 / US 805A9B25 0x04
     */
    STORYFLAG_712,

    /** [Piper's House entered (name on map appears)]
     *  Story Flag #713 (0x02C9) - JP 805ACDA5 0x08 / US 805A9B25 0x08
     */
    STORYFLAG_713,

    /** [Kukiel's House entered (name on map appears)]
     *  Story Flag #714 (0x02CA) - JP 805ACDA5 0x10 / US 805A9B25 0x10
     */
    STORYFLAG_714,

    /** [Orielle and Parrow's House entered (name on map appears)]
     *  Story Flag #715 (0x02CB) - JP 805ACDA5 0x20 / US 805A9B25 0x20
     */
    STORYFLAG_715,

    /** [Gear Peddler Rupin's House entered (name on map appears)]
     *  Story Flag #716 (0x02CC) - JP 805ACDA5 0x40 / US 805A9B25 0x40
     */
    STORYFLAG_716,

    /** [Pipit's House entered (name on map appears)]
     *  Story Flag #717 (0x02CD) - JP 805ACDA5 0x80 / US 805A9B25 0x80
     */
    STORYFLAG_717,

    /** [Fortune Teller Sparrot's House entered (name on map appears)]
     *  Story Flag #718 (0x02CE) - JP 805ACDA4 0x01 / US 805A9B24 0x01
     */
    STORYFLAG_718,

    /** [Scrap Shop Owner Gondo's House entered (name on map appears)]
     *  Story Flag #719 (0x02CF) - JP 805ACDA4 0x02 / US 805A9B24 0x02
     */
    STORYFLAG_719,

    /** [Potion Shop Owner Bertie's House entered (name on map appears)]
     *  Story Flag #720 (0x02D0) - JP 805ACDA4 0x04 / US 805A9B24 0x04
     */
    STORYFLAG_720,

    /** [(not implemented yet -- 5656)]
     *  Story Flag #721 (0x02D1) - JP from 805ACE05 0x20 to 805ACE04 0x08 / US from 805A9B85 0x20 to 805A9B84 0x08
     */
    STORYFLAG_721,

    /** [(not implemented yet -- 5405)]
     *  Story Flag #722 (0x02D2) - JP from 805ACE01 0x01 to 805ACE01 0x20 / US from 805A9B81 0x01 to 805A9B81 0x20
     */
    STORYFLAG_722,

    /** [Set when Scrapper gets destoryed (unsets after his Text after the respawn)]
     *  Story Flag #723 (0x02D3) - JP 805ACDA4 0x08 / US 805A9B24 0x08
     */
    STORYFLAG_723,

    /** [First time going through a light pillar (works both ways)]
     *  Story Flag #724 (0x02D4) - JP 805ACDA4 0x10 / US 805A9B24 0x10
     */
    STORYFLAG_724,

    /** [Set when going near Beds in the Lumpy Pumkin / unsets upon leaving]
     *  Story Flag #725 (0x02D5) - JP 805ACDA4 0x20 / US 805A9B24 0x20
     */
    STORYFLAG_725,

    /** [[Gets Set in a refight](Hylia's Realm layer 4)]
     *  Story Flag #726 (0x02D6) - JP 805ACDA4 0x40 / US 805A9B24 0x40
     */
    STORYFLAG_DEMISE_BOSSRUSH,

    /** [Talking to Fi after filling Wallet]
     *  Story Flag #727 (0x02D7) - JP 805ACDA4 0x80 / US 805A9B24 0x80
     */
    STORYFLAG_WALLET_FULL_ACK,

    /** [Triforce of Power]
     *  Story Flag #728 (0x02D8) - JP 805ACDA7 0x01 / US 805A9B27 0x01
     */
    STORYFLAG_728,

    /** [Triforce of Courage]
     *  Story Flag #729 (0x02D9) - JP 805ACDA7 0x02 / US 805A9B27 0x02
     */
    STORYFLAG_729,

    /** [Triforce of Wisdom (Wisdom Room Layer 2)]
     *  Story Flag #730 (0x02DA) - JP 805ACDA7 0x04 / US 805A9B27 0x04
     */
    STORYFLAG_730,

    /** [Trying to open Main node with only Fire or Lightning Node or Both]
     *  Story Flag #731 (0x02DB) - JP 805ACDA7 0x08 / US 805A9B27 0x08
     */
    STORYFLAG_731,

    /** [Trying to open Main node with only Water or Fire Node or Both]
     *  Story Flag #732 (0x02DC) - JP 805ACDA7 0x10 / US 805A9B27 0x10
     */
    STORYFLAG_732,

    /** [Trying to open Main node with only Water or Lighting Node or Both]
     *  Story Flag #733 (0x02DD) - JP 805ACDA7 0x20 / US 805A9B27 0x20
     */
    STORYFLAG_733,

    /** [Sets when talking to Batreaux after getting Crystals (allows Crystal Dowsing?)]
     *  Story Flag #734 (0x02DE) - JP 805ACDA7 0x40 / US 805A9B27 0x40
     */
    STORYFLAG_734,

    /** [Talking to Flegde after rescueing Kukiel at day]
     *  Story Flag #735 (0x02DF) - JP 805ACDA7 0x80 / US 805A9B27 0x80
     */
    STORYFLAG_735,

    /** [Talk to Owlan to start Oolo sidequest]
     *  Story Flag #736 (0x02E0) - JP 805ACDA6 0x01 / US 805A9B26 0x01
     */
    STORYFLAG_736,

    /** [Set when sleeping to night, unset when sleeping to day]
     *  Story Flag #737 (0x02E1) - JP 805ACDA6 0x02 / US 805A9B26 0x02
     */
    STORYFLAG_737,

    /** [Set after Fi text after Sea Chart in Skipper's house]
     *  Story Flag #738 (0x02E2) - JP 805ACDA6 0x04 / US 805A9B26 0x04
     */
    STORYFLAG_738,

    /** [Set after Fi text after Sea Chart in Skipper's house]
     *  Story Flag #739 (0x02E3) - JP 805ACDA6 0x08 / US 805A9B26 0x08
     */
    STORYFLAG_739,

    /** [Fi text about bugs on top of Skipper's house after sitting on chair]
     *  Story Flag #740 (0x02E4) - JP 805ACDA6 0x10 / US 805A9B26 0x10
     */
    STORYFLAG_740,

    /** [Set defeating Molderach 2]
     *  Story Flag #741 (0x02E5) - JP 805ACDA6 0x20 / US 805A9B26 0x20
     */
    STORYFLAG_741,

    /** [Sets when sitting down near Golo]
     *  Story Flag #742 (0x02E6) - JP 805ACDA6 0x40 / US 805A9B26 0x40
     */
    STORYFLAG_742,

    /** [set after scrappers text at start of Water Basin Escord]
     *  Story Flag #743 (0x02E7) - JP 805ACDA6 0x80 / US 805A9B26 0x80
     */
    STORYFLAG_WATER_BASIN_ESCORT_STARTED,

    /** [Scrapper text at start of scrapper escort]
     *  Story Flag #744 (0x02E8) - JP 805ACDA9 0x01 / US 805A9B29 0x01
     */
    STORYFLAG_744,

    /** [Multiple beacons cutscene in Lanayru Desert]
     *  Story Flag #745 (0x02E9) - JP 805ACDA9 0x02 / US 805A9B29 0x02
     */
    STORYFLAG_745,

    /** [Talking to Beedle again after giveing him his cage & loadingzone Tmp flag 8x04]
     *  Story Flag #746 (0x02EA) - JP 805ACDA9 0x04 / US 805A9B29 0x04
     */
    STORYFLAG_746,

    /** [Talking to Beedle again after giveing him his cage & loadingzone Tmp flag 8x08]
     *  Story Flag #747 (0x02EB) - JP 805ACDA9 0x08 / US 805A9B29 0x08
     */
    STORYFLAG_747,

    /** [Sets at the start of Scaldera cs / unsets at end of cs]
     *  Story Flag #748 (0x02EC) - JP 805ACDA9 0x10 / US 805A9B29 0x10
     */
    STORYFLAG_748,

    /** []
     *  Story Flag #749 (0x02ED) - JP 805ACDA9 0x20 / US 805A9B29 0x20
     */
    STORYFLAG_749,

    /** [Robot text after digging up Life Tree Seedling in Lanayru Gorge]
     *  Story Flag #750 (0x02EE) - JP 805ACDA9 0x40 / US 805A9B29 0x40
     */
    STORYFLAG_750,

    /** [-]
     *  Story Flag #751 (0x02EF) - JP - / US -
     */
    STORYFLAG_751,

    /** [Guess -> Respawn at Demise Area?]
     *  Story Flag #752 (0x02F0) - JP 805ACDA8 0x01 / US 805A9B28 0x01
     */
    STORYFLAG_752,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #753 (0x02F1) - JP 805ACDA8 0x02 / US 805A9B28 0x02
     */
    STORYFLAG_753,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #754 (0x02F2) - JP 805ACDA8 0x04 / US 805A9B28 0x04
     */
    STORYFLAG_754,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #755 (0x02F3) - JP 805ACDA8 0x08 / US 805A9B28 0x08
     */
    STORYFLAG_755,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #756 (0x02F4) - JP 805ACDA8 0x10 / US 805A9B28 0x10
     */
    STORYFLAG_756,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #757 (0x02F5) - JP 805ACDA8 0x20 / US 805A9B28 0x20
     */
    STORYFLAG_757,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #758 (0x02F6) - JP 805ACDA8 0x40 / US 805A9B28 0x40
     */
    STORYFLAG_758,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #759 (0x02F7) - JP 805ACDA8 0x80 / US 805A9B28 0x80
     */
    STORYFLAG_759,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #760 (0x02F8) - JP 805ACDAB 0x01 / US 805A9B2B 0x01
     */
    STORYFLAG_760,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #761 (0x02F9) - JP 805ACDAB 0x02 / US 805A9B2B 0x02
     */
    STORYFLAG_761,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #762 (0x02FA) - JP 805ACDAB 0x04 / US 805A9B2B 0x04
     */
    STORYFLAG_762,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #763 (0x02FB) - JP 805ACDAB 0x08 / US 805A9B2B 0x08
     */
    STORYFLAG_763,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #764 (0x02FC) - JP 805ACDAB 0x10 / US 805A9B2B 0x10
     */
    STORYFLAG_764,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #765 (0x02FD) - JP 805ACDAB 0x20 / US 805A9B2B 0x20
     */
    STORYFLAG_765,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #766 (0x02FE) - JP 805ACDAB 0x40 / US 805A9B2B 0x40
     */
    STORYFLAG_766,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #767 (0x02FF) - JP 805ACDAB 0x80 / US 805A9B2B 0x80
     */
    STORYFLAG_767,

    /** [[Is set on a Hero Mode File]]
     *  Story Flag #768 (0x0300) - JP 805ACDAA 0x01 / US 805A9B2A 0x01
     */
    STORYFLAG_768,

    /** [Set after completing Hylia's Trial]
     *  Story Flag #769 (0x0301) - JP 805ACDAA 0x02 / US 805A9B2A 0x02
     */
    STORYFLAG_769,

    /** [Start Remlit quest]
     *  Story Flag #770 (0x0302) - JP 805ACDAA 0x04 / US 805A9B2A 0x04
     */
    STORYFLAG_770,

    /** [Set when Cawlin runs away crying after giving the letter to toilet ghost]
     *  Story Flag #771 (0x0303) - JP 805ACDAA 0x08 / US 805A9B2A 0x08
     */
    STORYFLAG_771,

    /** [Talking to Fi in G1 fight after dieing]
     *  Story Flag #772 (0x0304) - JP 805ACDAA 0x10 / US 805A9B2A 0x10
     */
    STORYFLAG_772,

    /** [Talking to Fi in Scaldera fight after dieing]
     *  Story Flag #773 (0x0305) - JP 805ACDAA 0x20 / US 805A9B2A 0x20
     */
    STORYFLAG_773,

    /** [Talking to Fi in Molderach fight after dieing]
     *  Story Flag #774 (0x0306) - JP 805ACDAA 0x40 / US 805A9B2A 0x40
     */
    STORYFLAG_774,

    /** [Talking to Fi in Kolotos fight after dieing]
     *  Story Flag #775 (0x0307) - JP 805ACDAA 0x80 / US 805A9B2A 0x80
     */
    STORYFLAG_775,

    /** [Talking to Fi in Kolotos fight after dieing]
     *  Story Flag #776 (0x0308) - JP 805ACDAD 0x01 / US 805A9B2D 0x01
     */
    STORYFLAG_776,

    /** [Talking to Fi in G2 fight after dieing]
     *  Story Flag #777 (0x0309) - JP 805ACDAD 0x02 / US 805A9B2D 0x02
     */
    STORYFLAG_777,

    /** [Talking to Fi after already dying to imp 1 (needs ACx04)]
     *  Story Flag #778 (0x030A) - JP 805ACDAD 0x04 / US 805A9B2D 0x04
     */
    STORYFLAG_778,

    /** [Talking to Fi after already dying to imp 2 (needs ACx04)]
     *  Story Flag #779 (0x030B) - JP 805ACDAD 0x08 / US 805A9B2D 0x08
     */
    STORYFLAG_779,

    /** [Talking to Fi after already dying to imp 3 (needs ACx04)]
     *  Story Flag #780 (0x030C) - JP 805ACDAD 0x10 / US 805A9B2D 0x10
     */
    STORYFLAG_780,

    /** [Talking to Fi in bilocyte fight after dieing / sets after Levias explains SotH quest]
     *  Story Flag #781 (0x030D) - JP 805ACDAD 0x20 / US 805A9B2D 0x20
     */
    STORYFLAG_781,

    /** [Talking to Fi in G3 fight after dieing / Set after Ghirahim 3]
     *  Story Flag #782 (0x030E) - JP 805ACDAD 0x40 / US 805A9B2D 0x40
     */
    STORYFLAG_782,

    /** [Talking to Fi in Demise fight after dieing]
     *  Story Flag #783 (0x030F) - JP 805ACDAD 0x80 / US 805A9B2D 0x80
     */
    STORYFLAG_783,

    /** []
     *  Story Flag #784 (0x0310) - JP 805ACDAC 0x01 / US 805A9B2C 0x01
     */
    STORYFLAG_784,

    /** [dying inside a imp fight]
     *  Story Flag #785 (0x0311) - JP 805ACDAC 0x02 / US 805A9B2C 0x02
     */
    STORYFLAG_785,

    /** [set after starting a imp fight again (with dieing)]
     *  Story Flag #786 (0x0312) - JP 805ACDAC 0x04 / US 805A9B2C 0x04
     */
    STORYFLAG_786,

    /** [Set at the beginning of Ghirahim 3 fight]
     *  Story Flag #787 (0x0313) - JP 805ACDAC 0x08 / US 805A9B2C 0x08
     */
    STORYFLAG_787,

    /** [reenter Ghirahim 3 fight]
     *  Story Flag #788 (0x0314) - JP 805ACDAC 0x10 / US 805A9B2C 0x10
     */
    STORYFLAG_788,

    /** [Set after skyward striking imp spike first time (allows dowsing option [requires a sword])]
     *  Story Flag #789 (0x0315) - JP 805ACDAC 0x20 / US 805A9B2C 0x20
     */
    STORYFLAG_DOWSING_UNLOCKED,

    /** [Set going close to imp spike first time (unset later)]
     *  Story Flag #790 (0x0316) - JP 805ACDAC 0x40 / US 805A9B2C 0x40
     */
    STORYFLAG_790,

    /** [Set going close to imp spike first time (unset later)]
     *  Story Flag #791 (0x0317) - JP 805ACDAC 0x80 / US 805A9B2C 0x80
     */
    STORYFLAG_791,

    /** [Door to sealed temple unlocking cutscene]
     *  Story Flag #792 (0x0318) - JP 805ACDAF 0x01 / US 805A9B2F 0x01
     */
    STORYFLAG_792,

    /** [Set entering Sealed Temple for first time]
     *  Story Flag #793 (0x0319) - JP 805ACDAF 0x02 / US 805A9B2F 0x02
     */
    STORYFLAG_793,

    /** [Cutscene showing off that Life Tree has grown + jingle]
     *  Story Flag #794 (0x031A) - JP 805ACDAF 0x04 / US 805A9B2F 0x04
     */
    STORYFLAG_794,

    /** [Imp 1 defeat related / dieing inside Imp 1 fight]
     *  Story Flag #795 (0x031B) - JP 805ACDAF 0x08 / US 805A9B2F 0x08
     */
    STORYFLAG_795,

    /** [Set when Imp fight is triggered, unset once fight starts]
     *  Story Flag #796 (0x031C) - JP 805ACDAF 0x10 / US 805A9B2F 0x10
     */
    STORYFLAG_796,

    /** [Set after Fi text after skyward striking imp spike first time]
     *  Story Flag #797 (0x031D) - JP 805ACDAF 0x20 / US 805A9B2F 0x20
     */
    STORYFLAG_797,

    /** [Talking to Groose after Zelda get captured]
     *  Story Flag #798 (0x031E) - JP 805ACDAF 0x40 / US 805A9B2F 0x40
     */
    STORYFLAG_798,

    /** [Talking to Groose after beating the game on Hero mode]
     *  Story Flag #799 (0x031F) - JP 805ACDAF 0x80 / US 805A9B2F 0x80
     */
    STORYFLAG_799,

    /** [Check Faron Woods Entry Statue]
     *  Story Flag #800 (0x0320) - JP 805ACDAE 0x01 / US 805A9B2E 0x01
     */
    STORYFLAG_800,

    /** [Check In The Woods Statue]
     *  Story Flag #801 (0x0321) - JP 805ACDAE 0x02 / US 805A9B2E 0x02
     */
    STORYFLAG_801,

    /** [Check Viewing Platform Statue]
     *  Story Flag #802 (0x0322) - JP 805ACDAE 0x04 / US 805A9B2E 0x04
     */
    STORYFLAG_802,

    /** [Check Great Tree Statue]
     *  Story Flag #803 (0x0323) - JP 805ACDAE 0x08 / US 805A9B2E 0x08
     */
    STORYFLAG_803,

    /** [Check Volcano Entry Statue]
     *  Story Flag #804 (0x0324) - JP 805ACDAE 0x10 / US 805A9B2E 0x10
     */
    STORYFLAG_804,

    /** [Check Volcano East Statue]
     *  Story Flag #805 (0x0325) - JP 805ACDAE 0x20 / US 805A9B2E 0x20
     */
    STORYFLAG_805,

    /** [Check Volcano Ascent]
     *  Story Flag #806 (0x0326) - JP 805ACDAE 0x40 / US 805A9B2E 0x40
     */
    STORYFLAG_806,

    /** [Check Temple Entrance Statue]
     *  Story Flag #807 (0x0327) - JP 805ACDAE 0x80 / US 805A9B2E 0x80
     */
    STORYFLAG_807,

    /** [Fi text for low Hearts (unsets after S&Q)]
     *  Story Flag #808 (0x0328) - JP 805ACDB1 0x01 / US 805A9B31 0x01
     */
    STORYFLAG_LOW_HEART_NOTICE,

    /** [[item Seller related]]
     *  Story Flag #809 (0x0329) - JP 805ACDB1 0x02 / US 805A9B31 0x02
     */
    STORYFLAG_809,

    /** [Talk to Stich after Learing the Song of the Hero]
     *  Story Flag #810 (0x032A) - JP 805ACDB1 0x04 / US 805A9B31 0x04
     */
    STORYFLAG_810,

    /** [Talking to Parrow after Stich finds Bug Heaven, but before giving the beetle to Beedle]
     *  Story Flag #811 (0x032B) - JP 805ACDB1 0x08 / US 805A9B31 0x08
     */
    STORYFLAG_811,

    /** [Opening the door after Molderach (LMF Boss Room layer 3)]
     *  Story Flag #812 (0x032C) - JP 805ACDB1 0x10 / US 805A9B31 0x10
     */
    STORYFLAG_812,

    /** []
     *  Story Flag #813 (0x032D) - JP 805ACDB1 0x20 / US 805A9B31 0x20
     */
    STORYFLAG_813,

    /** []
     *  Story Flag #814 (0x032E) - JP 805ACDB1 0x40 / US 805A9B31 0x40
     */
    STORYFLAG_814,

    /** [Skipper's text after beating Sandship (changes Sandship layer)]
     *  Story Flag #815 (0x032F) - JP 805ACDB1 0x80 / US 805A9B31 0x80
     */
    STORYFLAG_815,

    /** []
     *  Story Flag #816 (0x0330) - JP 805ACDB0 0x01 / US 805A9B30 0x01
     */
    STORYFLAG_816,

    /** [ if set different text for cupboards]
     *  Story Flag #817 (0x0331) - JP 805ACDB0 0x02 / US 805A9B30 0x02
     */
    STORYFLAG_ALTERNATE_WARDROBE_TEXT,

    /** [Dowsing button bipping (can be unset)]
     *  Story Flag #818 (0x0332) - JP 805ACDB0 0x04 / US 805A9B30 0x04
     */
    STORYFLAG_DOWSING_NOTICE,

    /** [Sets after mogma talks about Hidden room in FS]
     *  Story Flag #819 (0x0333) - JP 805ACDB0 0x08 / US 805A9B30 0x08
     */
    STORYFLAG_819,

    /** [Sets after mogma disapears and other mogma apears in FS]
     *  Story Flag #820 (0x0334) - JP 805ACDB0 0x10 / US 805A9B30 0x10
     */
    STORYFLAG_820,

    /** [Sets while chaseing Mogma]
     *  Story Flag #821 (0x0335) - JP 805ACDB0 0x20 / US 805A9B30 0x20
     */
    STORYFLAG_821,

    /** [Sets after catching Mogma]
     *  Story Flag #822 (0x0336) - JP 805ACDB0 0x40 / US 805A9B30 0x40
     */
    STORYFLAG_822,

    /** [Goron Text about Lanayru SotH]
     *  Story Flag #823 (0x0337) - JP 805ACDB0 0x80 / US 805A9B30 0x80
     */
    STORYFLAG_823,

    /** [Groose talks about his groosenator and how he can fling you everywere]
     *  Story Flag #824 (0x0338) - JP 805ACDB3 0x01 / US 805A9B33 0x01
     */
    STORYFLAG_824,

    /** [Old man on Lumpy Pumkin talks about Batreaux]
     *  Story Flag #825 (0x0339) - JP 805ACDB3 0x02 / US 805A9B33 0x02
     */
    STORYFLAG_825,

    /** [Fi Text near Bazaar Goddess Chest]
     *  Story Flag #826 (0x033A) - JP 805ACDB3 0x04 / US 805A9B33 0x04
     */
    STORYFLAG_826,

    /** [Get Mushroom Spores from Parrow (unsets after Textbox)]
     *  Story Flag #827 (0x033B) - JP 805ACDB3 0x08 / US 805A9B33 0x08
     */
    STORYFLAG_827,

    /** [Goron Text about Lanayru SotH]
     *  Story Flag #828 (0x033C) - JP 805ACDB3 0x10 / US 805A9B33 0x10
     */
    STORYFLAG_828,

    /** [Goron Text about Lanayru SotH]
     *  Story Flag #829 (0x033D) - JP 805ACDB3 0x20 / US 805A9B33 0x20
     */
    STORYFLAG_829,

    /** []
     *  Story Flag #830 (0x033E) - JP 805ACDB3 0x40 / US 805A9B33 0x40
     */
    STORYFLAG_830,

    /** [Goron Text about Thunder Dragon]
     *  Story Flag #831 (0x033F) - JP 805ACDB3 0x80 / US 805A9B33 0x80
     */
    STORYFLAG_831,

    /** [Help button bipping (can be unset)]
     *  Story Flag #832 (0x0340) - JP 805ACDB2 0x01 / US 805A9B32 0x01
     */
    STORYFLAG_2_BTN_NOTICE,

    /** [[sets before skyview]]
     *  Story Flag #833 (0x0341) - JP 805ACDB2 0x02 / US 805A9B32 0x02
     */
    STORYFLAG_833,

    /** [Talking to Old man in the Bazzar after the Fortune Teller disapears]
     *  Story Flag #834 (0x0342) - JP 805ACDB2 0x04 / US 805A9B32 0x04
     */
    STORYFLAG_834,

    /** [Talk to bug Kid after going to Beedle's Airshop]
     *  Story Flag #835 (0x0343) - JP 805ACDB2 0x08 / US 805A9B32 0x08
     */
    STORYFLAG_835,

    /** [Fledge talks about Training at night]
     *  Story Flag #836 (0x0344) - JP 805ACDB2 0x10 / US 805A9B32 0x10
     */
    STORYFLAG_836,

    /** [Sets when going into Shed]
     *  Story Flag #837 (0x0345) - JP 805ACDB2 0x20 / US 805A9B32 0x20
     */
    STORYFLAG_837,

    /** [CS of Parrow asking were his Sister is]
     *  Story Flag #838 (0x0346) - JP 805ACDB2 0x40 / US 805A9B32 0x40
     */
    STORYFLAG_838,

    /** [Save a Hero Mode file at the end of the credits (Triforce symbol appears on file select)]
     *  Story Flag #839 (0x0347) - JP 805ACDB2 0x80 / US 805A9B32 0x80
     */
    STORYFLAG_839,

    /** [UI Mode. Valid Options: 0, 1, 2)]
     *  Story Flag #840 (0x0348) - JP from 805ACE46 0x20 to 805ACE46 0x40 / US from 805A9BC6 0x20 to 805A9BC6 0x40
     */
    STORYFLAG_UI_MODE,

    /** [Heart Medal obtained on Hero Mode]
     *  Story Flag #841 (0x0349) - JP 805ACDB5 0x01 / US 805A9B35 0x01
     */
    STORYFLAG_841,

    /** [Set when starting a new game]
     *  Story Flag #842 (0x034A) - JP 805ACDB5 0x02 / US 805A9B35 0x02
     */
    STORYFLAG_842,

    /** [first time entering an Area with a overword statue visited]
     *  Story Flag #843 (0x034B) - JP 805ACDB5 0x04 / US 805A9B35 0x04
     */
    STORYFLAG_843,

    /** [Asking Fi about Spiders with targeting them]
     *  Story Flag #844 (0x034C) - JP 805ACDB5 0x08 / US 805A9B35 0x08
     */
    STORYFLAG_844,

    /** [Asking Fi again about Spiders with targeting them]
     *  Story Flag #845 (0x034D) - JP 805ACDB5 0x10 / US 805A9B35 0x10
     */
    STORYFLAG_845,

    /** [Saved Disp00 Tab - Not in Spreadsheet]
     *  Story Flag #846 (0x034E) - JP from 805ACE44 0x40 to 805ACE44 0x80 / US from 805A9BC4 0x40 to 805A9BC4 0x80
     */
    STORYFLAG_SAVED_DISP00_TAB,

    /** [On collection inventory screen (can be unset)]
     *  Story Flag #847 (0x034F) - JP 805ACDB5 0x20 / US 805A9B35 0x20
     */
    STORYFLAG_ON_COLLECTION_SCREEN,

    /** [Talking to Robots after meeting Levias]
     *  Story Flag #848 (0x0350) - JP 805ACDB5 0x40 / US 805A9B35 0x40
     */
    STORYFLAG_848,

    /** [Bug Net Purchased]
     *  Story Flag #849 (0x0351) - JP 805ACDB5 0x80 / US 805A9B35 0x80
     */
    STORYFLAG_849,

    /** [set if you collect a map the second time]
     *  Story Flag #850 (0x0352) - JP 805ACDB4 0x01 / US 805A9B34 0x01
     */
    STORYFLAG_850,

    /** [Sets when reaching Closed gate in long lava path in ET]
     *  Story Flag #851 (0x0353) - JP 805ACDB4 0x02 / US 805A9B34 0x02
     */
    STORYFLAG_851,

    /** [Imp 1 related]
     *  Story Flag #852 (0x0354) - JP 805ACDB4 0x04 / US 805A9B34 0x04
     */
    STORYFLAG_852,

    /** [Imp 2 related]
     *  Story Flag #853 (0x0355) - JP 805ACDB4 0x08 / US 805A9B34 0x08
     */
    STORYFLAG_853,

    /** [Imp 3 related]
     *  Story Flag #854 (0x0356) - JP 805ACDB4 0x10 / US 805A9B34 0x10
     */
    STORYFLAG_854,

    /** []
     *  Story Flag #855 (0x0357) - JP 805ACDB4 0x20 / US 805A9B34 0x20
     */
    STORYFLAG_855,

    /** []
     *  Story Flag #856 (0x0358) - JP 805ACDB4 0x40 / US 805A9B34 0x40
     */
    STORYFLAG_856,

    /** []
     *  Story Flag #857 (0x0359) - JP 805ACDB4 0x80 / US 805A9B34 0x80
     */
    STORYFLAG_857,

    /** []
     *  Story Flag #858 (0x035A) - JP 805ACDB7 0x01 / US 805A9B37 0x01
     */
    STORYFLAG_858,

    /** []
     *  Story Flag #859 (0x035B) - JP 805ACDB7 0x02 / US 805A9B37 0x02
     */
    STORYFLAG_859,

    /** []
     *  Story Flag #860 (0x035C) - JP 805ACDB7 0x04 / US 805A9B37 0x04
     */
    STORYFLAG_860,

    /** []
     *  Story Flag #861 (0x035D) - JP 805ACDB7 0x08 / US 805A9B37 0x08
     */
    STORYFLAG_861,

    /** []
     *  Story Flag #862 (0x035E) - JP 805ACDB7 0x10 / US 805A9B37 0x10
     */
    STORYFLAG_862,

    /** []
     *  Story Flag #863 (0x035F) - JP 805ACDB7 0x20 / US 805A9B37 0x20
     */
    STORYFLAG_863,

    /** []
     *  Story Flag #864 (0x0360) - JP 805ACDB7 0x40 / US 805A9B37 0x40
     */
    STORYFLAG_864,

    /** []
     *  Story Flag #865 (0x0361) - JP 805ACDB7 0x80 / US 805A9B37 0x80
     */
    STORYFLAG_865,

    /** []
     *  Story Flag #866 (0x0362) - JP 805ACDB6 0x01 / US 805A9B36 0x01
     */
    STORYFLAG_866,

    /** []
     *  Story Flag #867 (0x0363) - JP 805ACDB6 0x02 / US 805A9B36 0x02
     */
    STORYFLAG_867,

    /** []
     *  Story Flag #868 (0x0364) - JP 805ACDB6 0x04 / US 805A9B36 0x04
     */
    STORYFLAG_868,

    /** []
     *  Story Flag #869 (0x0365) - JP 805ACDB6 0x08 / US 805A9B36 0x08
     */
    STORYFLAG_869,

    /** []
     *  Story Flag #870 (0x0366) - JP 805ACDB6 0x10 / US 805A9B36 0x10
     */
    STORYFLAG_870,

    /** []
     *  Story Flag #871 (0x0367) - JP 805ACDB6 0x20 / US 805A9B36 0x20
     */
    STORYFLAG_871,

    /** []
     *  Story Flag #872 (0x0368) - JP 805ACDB6 0x40 / US 805A9B36 0x40
     */
    STORYFLAG_872,

    /** []
     *  Story Flag #873 (0x0369) - JP 805ACDB6 0x80 / US 805A9B36 0x80
     */
    STORYFLAG_873,

    /** []
     *  Story Flag #874 (0x036A) - JP 805ACDB9 0x01 / US 805A9B39 0x01
     */
    STORYFLAG_874,

    /** []
     *  Story Flag #875 (0x036B) - JP 805ACDB9 0x02 / US 805A9B39 0x02
     */
    STORYFLAG_875,

    /** []
     *  Story Flag #876 (0x036C) - JP 805ACDB9 0x04 / US 805A9B39 0x04
     */
    STORYFLAG_876,

    /** []
     *  Story Flag #877 (0x036D) - JP 805ACDB9 0x08 / US 805A9B39 0x08
     */
    STORYFLAG_877,

    /** []
     *  Story Flag #878 (0x036E) - JP 805ACDB9 0x10 / US 805A9B39 0x10
     */
    STORYFLAG_878,

    /** []
     *  Story Flag #879 (0x036F) - JP 805ACDB9 0x20 / US 805A9B39 0x20
     */
    STORYFLAG_879,

    /** []
     *  Story Flag #880 (0x0370) - JP 805ACDB9 0x40 / US 805A9B39 0x40
     */
    STORYFLAG_880,

    /** []
     *  Story Flag #881 (0x0371) - JP 805ACDB9 0x80 / US 805A9B39 0x80
     */
    STORYFLAG_881,

    /** []
     *  Story Flag #882 (0x0372) - JP 805ACDB8 0x01 / US 805A9B38 0x01
     */
    STORYFLAG_882,

    /** []
     *  Story Flag #883 (0x0373) - JP 805ACDB8 0x02 / US 805A9B38 0x02
     */
    STORYFLAG_883,

    /** []
     *  Story Flag #884 (0x0374) - JP 805ACDB8 0x04 / US 805A9B38 0x04
     */
    STORYFLAG_884,

    /** []
     *  Story Flag #885 (0x0375) - JP 805ACDB8 0x08 / US 805A9B38 0x08
     */
    STORYFLAG_885,

    /** []
     *  Story Flag #886 (0x0376) - JP 805ACDB8 0x10 / US 805A9B38 0x10
     */
    STORYFLAG_886,

    /** []
     *  Story Flag #887 (0x0377) - JP 805ACDB8 0x20 / US 805A9B38 0x20
     */
    STORYFLAG_887,

    /** []
     *  Story Flag #888 (0x0378) - JP 805ACDB8 0x40 / US 805A9B38 0x40
     */
    STORYFLAG_888,

    /** []
     *  Story Flag #889 (0x0379) - JP 805ACDB8 0x80 / US 805A9B38 0x80
     */
    STORYFLAG_889,

    /** []
     *  Story Flag #890 (0x037A) - JP 805ACDBB 0x01 / US 805A9B3B 0x01
     */
    STORYFLAG_890,

    /** []
     *  Story Flag #891 (0x037B) - JP 805ACDBB 0x02 / US 805A9B3B 0x02
     */
    STORYFLAG_891,

    /** []
     *  Story Flag #892 (0x037C) - JP 805ACDBB 0x04 / US 805A9B3B 0x04
     */
    STORYFLAG_892,

    /** []
     *  Story Flag #893 (0x037D) - JP 805ACDBB 0x08 / US 805A9B3B 0x08
     */
    STORYFLAG_893,

    /** []
     *  Story Flag #894 (0x037E) - JP 805ACDBB 0x10 / US 805A9B3B 0x10
     */
    STORYFLAG_894,

    /** []
     *  Story Flag #895 (0x037F) - JP 805ACDBB 0x20 / US 805A9B3B 0x20
     */
    STORYFLAG_895,

    /** []
     *  Story Flag #896 (0x0380) - JP 805ACDBB 0x40 / US 805A9B3B 0x40
     */
    STORYFLAG_896,

    /** []
     *  Story Flag #897 (0x0381) - JP 805ACDBB 0x80 / US 805A9B3B 0x80
     */
    STORYFLAG_897,

    /** []
     *  Story Flag #898 (0x0382) - JP 805ACDBA 0x01 / US 805A9B3A 0x01
     */
    STORYFLAG_898,

    /** []
     *  Story Flag #899 (0x0383) - JP 805ACDBA 0x02 / US 805A9B3A 0x02
     */
    STORYFLAG_899,

    /** []
     *  Story Flag #900 (0x0384) - JP 805ACDBA 0x04 / US 805A9B3A 0x04
     */
    STORYFLAG_900,

    /** []
     *  Story Flag #901 (0x0385) - JP 805ACDBA 0x08 / US 805A9B3A 0x08
     */
    STORYFLAG_901,

    /** []
     *  Story Flag #902 (0x0386) - JP 805ACDBA 0x10 / US 805A9B3A 0x10
     */
    STORYFLAG_902,

    /** []
     *  Story Flag #903 (0x0387) - JP 805ACDBA 0x20 / US 805A9B3A 0x20
     */
    STORYFLAG_903,

    /** []
     *  Story Flag #904 (0x0388) - JP 805ACDBA 0x40 / US 805A9B3A 0x40
     */
    STORYFLAG_904,

    /** []
     *  Story Flag #905 (0x0389) - JP 805ACDBA 0x80 / US 805A9B3A 0x80
     */
    STORYFLAG_905,

    /** []
     *  Story Flag #906 (0x038A) - JP 805ACDBD 0x01 / US 805A9B3D 0x01
     */
    STORYFLAG_906,

    /** []
     *  Story Flag #907 (0x038B) - JP 805ACDBD 0x02 / US 805A9B3D 0x02
     */
    STORYFLAG_907,

    /** []
     *  Story Flag #908 (0x038C) - JP 805ACDBD 0x04 / US 805A9B3D 0x04
     */
    STORYFLAG_908,

    /** []
     *  Story Flag #909 (0x038D) - JP 805ACDBD 0x08 / US 805A9B3D 0x08
     */
    STORYFLAG_909,

    /** []
     *  Story Flag #910 (0x038E) - JP 805ACDBD 0x10 / US 805A9B3D 0x10
     */
    STORYFLAG_910,

    /** []
     *  Story Flag #911 (0x038F) - JP 805ACDBD 0x20 / US 805A9B3D 0x20
     */
    STORYFLAG_911,

    /** []
     *  Story Flag #912 (0x0390) - JP 805ACDBD 0x40 / US 805A9B3D 0x40
     */
    STORYFLAG_912,

    /** []
     *  Story Flag #913 (0x0391) - JP 805ACDBD 0x80 / US 805A9B3D 0x80
     */
    STORYFLAG_913,

    /** []
     *  Story Flag #914 (0x0392) - JP 805ACDBC 0x01 / US 805A9B3C 0x01
     */
    STORYFLAG_914,

    /** []
     *  Story Flag #915 (0x0393) - JP 805ACDBC 0x02 / US 805A9B3C 0x02
     */
    STORYFLAG_915,

    /** []
     *  Story Flag #916 (0x0394) - JP 805ACDBC 0x04 / US 805A9B3C 0x04
     */
    STORYFLAG_916,

    /** []
     *  Story Flag #917 (0x0395) - JP 805ACDBC 0x08 / US 805A9B3C 0x08
     */
    STORYFLAG_917,

    /** []
     *  Story Flag #918 (0x0396) - JP 805ACDBC 0x10 / US 805A9B3C 0x10
     */
    STORYFLAG_918,

    /** []
     *  Story Flag #919 (0x0397) - JP 805ACDBC 0x20 / US 805A9B3C 0x20
     */
    STORYFLAG_919,

    /** []
     *  Story Flag #920 (0x0398) - JP 805ACDBC 0x40 / US 805A9B3C 0x40
     */
    STORYFLAG_920,

    /** []
     *  Story Flag #921 (0x0399) - JP 805ACDBC 0x80 / US 805A9B3C 0x80
     */
    STORYFLAG_921,

    /** []
     *  Story Flag #922 (0x039A) - JP 805ACDBF 0x01 / US 805A9B3F 0x01
     */
    STORYFLAG_922,

    /** []
     *  Story Flag #923 (0x039B) - JP 805ACDBF 0x02 / US 805A9B3F 0x02
     */
    STORYFLAG_923,

    /** []
     *  Story Flag #924 (0x039C) - JP 805ACDBF 0x04 / US 805A9B3F 0x04
     */
    STORYFLAG_924,

    /** []
     *  Story Flag #925 (0x039D) - JP 805ACDBF 0x08 / US 805A9B3F 0x08
     */
    STORYFLAG_925,

    /** []
     *  Story Flag #926 (0x039E) - JP 805ACDBF 0x10 / US 805A9B3F 0x10
     */
    STORYFLAG_926,

    /** []
     *  Story Flag #927 (0x039F) - JP 805ACDBF 0x20 / US 805A9B3F 0x20
     */
    STORYFLAG_927,

    /** []
     *  Story Flag #928 (0x03A0) - JP 805ACDBF 0x40 / US 805A9B3F 0x40
     */
    STORYFLAG_928,

    /** []
     *  Story Flag #929 (0x03A1) - JP 805ACDBF 0x80 / US 805A9B3F 0x80
     */
    STORYFLAG_929,

    /** []
     *  Story Flag #930 (0x03A2) - JP 805ACDBE 0x01 / US 805A9B3E 0x01
     */
    STORYFLAG_930,

    /** []
     *  Story Flag #931 (0x03A3) - JP 805ACDBE 0x02 / US 805A9B3E 0x02
     */
    STORYFLAG_931,

    /** []
     *  Story Flag #932 (0x03A4) - JP 805ACDBE 0x04 / US 805A9B3E 0x04
     */
    STORYFLAG_932,

    /** []
     *  Story Flag #933 (0x03A5) - JP 805ACDBE 0x08 / US 805A9B3E 0x08
     */
    STORYFLAG_933,

    /** []
     *  Story Flag #934 (0x03A6) - JP 805ACDBE 0x10 / US 805A9B3E 0x10
     */
    STORYFLAG_934,

    /** []
     *  Story Flag #935 (0x03A7) - JP 805ACDBE 0x20 / US 805A9B3E 0x20
     */
    STORYFLAG_935,

    /** []
     *  Story Flag #936 (0x03A8) - JP 805ACDBE 0x40 / US 805A9B3E 0x40
     */
    STORYFLAG_936,

    /** []
     *  Story Flag #937 (0x03A9) - JP 805ACDBE 0x80 / US 805A9B3E 0x80
     */
    STORYFLAG_937,

    /** []
     *  Story Flag #938 (0x03AA) - JP 805ACDC1 0x01 / US 805A9B41 0x01
     */
    STORYFLAG_938,

    /** []
     *  Story Flag #939 (0x03AB) - JP 805ACDC1 0x02 / US 805A9B41 0x02
     */
    STORYFLAG_939,

    /** []
     *  Story Flag #940 (0x03AC) - JP 805ACDC1 0x04 / US 805A9B41 0x04
     */
    STORYFLAG_940,

    /** []
     *  Story Flag #941 (0x03AD) - JP 805ACDC1 0x08 / US 805A9B41 0x08
     */
    STORYFLAG_941,

    /** []
     *  Story Flag #942 (0x03AE) - JP 805ACDC1 0x10 / US 805A9B41 0x10
     */
    STORYFLAG_942,

    /** []
     *  Story Flag #943 (0x03AF) - JP 805ACDC1 0x20 / US 805A9B41 0x20
     */
    STORYFLAG_943,

    /** []
     *  Story Flag #944 (0x03B0) - JP 805ACDC1 0x40 / US 805A9B41 0x40
     */
    STORYFLAG_944,

    /** []
     *  Story Flag #945 (0x03B1) - JP 805ACDC1 0x80 / US 805A9B41 0x80
     */
    STORYFLAG_945,

    /** []
     *  Story Flag #946 (0x03B2) - JP 805ACDC0 0x01 / US 805A9B40 0x01
     */
    STORYFLAG_946,

    /** []
     *  Story Flag #947 (0x03B3) - JP 805ACDC0 0x02 / US 805A9B40 0x02
     */
    STORYFLAG_947,

    /** []
     *  Story Flag #948 (0x03B4) - JP 805ACDC0 0x04 / US 805A9B40 0x04
     */
    STORYFLAG_948,

    /** []
     *  Story Flag #949 (0x03B5) - JP 805ACDC0 0x08 / US 805A9B40 0x08
     */
    STORYFLAG_949,

    /** []
     *  Story Flag #950 (0x03B6) - JP 805ACDC0 0x10 / US 805A9B40 0x10
     */
    STORYFLAG_950,

    /** []
     *  Story Flag #951 (0x03B7) - JP 805ACDC0 0x20 / US 805A9B40 0x20
     */
    STORYFLAG_951,

    /** []
     *  Story Flag #952 (0x03B8) - JP 805ACDC0 0x40 / US 805A9B40 0x40
     */
    STORYFLAG_952,

    /** []
     *  Story Flag #953 (0x03B9) - JP 805ACDC0 0x80 / US 805A9B40 0x80
     */
    STORYFLAG_953,

    /** []
     *  Story Flag #954 (0x03BA) - JP 805ACDC3 0x01 / US 805A9B43 0x01
     */
    STORYFLAG_954,

    /** []
     *  Story Flag #955 (0x03BB) - JP 805ACDC3 0x02 / US 805A9B43 0x02
     */
    STORYFLAG_955,

    /** []
     *  Story Flag #956 (0x03BC) - JP 805ACDC3 0x04 / US 805A9B43 0x04
     */
    STORYFLAG_956,

    /** []
     *  Story Flag #957 (0x03BD) - JP 805ACDC3 0x08 / US 805A9B43 0x08
     */
    STORYFLAG_957,

    /** []
     *  Story Flag #958 (0x03BE) - JP 805ACDC3 0x10 / US 805A9B43 0x10
     */
    STORYFLAG_958,

    /** []
     *  Story Flag #959 (0x03BF) - JP 805ACDC3 0x20 / US 805A9B43 0x20
     */
    STORYFLAG_959,

    /** []
     *  Story Flag #960 (0x03C0) - JP 805ACDC3 0x40 / US 805A9B43 0x40
     */
    STORYFLAG_960,

    /** []
     *  Story Flag #961 (0x03C1) - JP 805ACDC3 0x80 / US 805A9B43 0x80
     */
    STORYFLAG_961,

    /** []
     *  Story Flag #962 (0x03C2) - JP 805ACDC2 0x01 / US 805A9B42 0x01
     */
    STORYFLAG_962,

    /** []
     *  Story Flag #963 (0x03C3) - JP 805ACDC2 0x02 / US 805A9B42 0x02
     */
    STORYFLAG_963,

    /** []
     *  Story Flag #964 (0x03C4) - JP 805ACDC2 0x04 / US 805A9B42 0x04
     */
    STORYFLAG_964,

    /** []
     *  Story Flag #965 (0x03C5) - JP 805ACDC2 0x08 / US 805A9B42 0x08
     */
    STORYFLAG_965,

    /** []
     *  Story Flag #966 (0x03C6) - JP 805ACDC2 0x10 / US 805A9B42 0x10
     */
    STORYFLAG_966,

    /** []
     *  Story Flag #967 (0x03C7) - JP 805ACDC2 0x20 / US 805A9B42 0x20
     */
    STORYFLAG_967,

    /** []
     *  Story Flag #968 (0x03C8) - JP 805ACDC2 0x40 / US 805A9B42 0x40
     */
    STORYFLAG_968,

    /** []
     *  Story Flag #969 (0x03C9) - JP 805ACDC2 0x80 / US 805A9B42 0x80
     */
    STORYFLAG_969,

    /** []
     *  Story Flag #970 (0x03CA) - JP 805ACDC5 0x01 / US 805A9B45 0x01
     */
    STORYFLAG_970,

    /** []
     *  Story Flag #971 (0x03CB) - JP 805ACDC5 0x02 / US 805A9B45 0x02
     */
    STORYFLAG_971,

    /** []
     *  Story Flag #972 (0x03CC) - JP 805ACDC5 0x04 / US 805A9B45 0x04
     */
    STORYFLAG_972,

    /** []
     *  Story Flag #973 (0x03CD) - JP 805ACDC5 0x08 / US 805A9B45 0x08
     */
    STORYFLAG_973,

    /** []
     *  Story Flag #974 (0x03CE) - JP 805ACDC5 0x10 / US 805A9B45 0x10
     */
    STORYFLAG_974,

    /** []
     *  Story Flag #975 (0x03CF) - JP 805ACDC5 0x20 / US 805A9B45 0x20
     */
    STORYFLAG_975,

    /** []
     *  Story Flag #976 (0x03D0) - JP 805ACDC5 0x40 / US 805A9B45 0x40
     */
    STORYFLAG_976,

    /** []
     *  Story Flag #977 (0x03D1) - JP 805ACDC5 0x80 / US 805A9B45 0x80
     */
    STORYFLAG_977,

    /** []
     *  Story Flag #978 (0x03D2) - JP 805ACDC4 0x01 / US 805A9B44 0x01
     */
    STORYFLAG_978,

    /** []
     *  Story Flag #979 (0x03D3) - JP 805ACDC4 0x02 / US 805A9B44 0x02
     */
    STORYFLAG_979,

    /** []
     *  Story Flag #980 (0x03D4) - JP 805ACDC4 0x04 / US 805A9B44 0x04
     */
    STORYFLAG_980,

    /** []
     *  Story Flag #981 (0x03D5) - JP 805ACDC4 0x08 / US 805A9B44 0x08
     */
    STORYFLAG_981,

    /** []
     *  Story Flag #982 (0x03D6) - JP 805ACDC4 0x10 / US 805A9B44 0x10
     */
    STORYFLAG_982,

    /** []
     *  Story Flag #983 (0x03D7) - JP 805ACDC4 0x20 / US 805A9B44 0x20
     */
    STORYFLAG_983,

    /** []
     *  Story Flag #984 (0x03D8) - JP 805ACDC4 0x40 / US 805A9B44 0x40
     */
    STORYFLAG_984,

    /** []
     *  Story Flag #985 (0x03D9) - JP 805ACDC4 0x80 / US 805A9B44 0x80
     */
    STORYFLAG_985,

    /** []
     *  Story Flag #986 (0x03DA) - JP 805ACDC7 0x01 / US 805A9B47 0x01
     */
    STORYFLAG_986,

    /** []
     *  Story Flag #987 (0x03DB) - JP 805ACDC7 0x02 / US 805A9B47 0x02
     */
    STORYFLAG_987,

    /** []
     *  Story Flag #988 (0x03DC) - JP 805ACDC7 0x04 / US 805A9B47 0x04
     */
    STORYFLAG_988,

    /** []
     *  Story Flag #989 (0x03DD) - JP 805ACDC7 0x08 / US 805A9B47 0x08
     */
    STORYFLAG_989,

    /** []
     *  Story Flag #990 (0x03DE) - JP 805ACDC7 0x10 / US 805A9B47 0x10
     */
    STORYFLAG_990,

    /** []
     *  Story Flag #991 (0x03DF) - JP 805ACDC7 0x20 / US 805A9B47 0x20
     */
    STORYFLAG_991,

    /** []
     *  Story Flag #992 (0x03E0) - JP 805ACDC7 0x40 / US 805A9B47 0x40
     */
    STORYFLAG_992,

    /** []
     *  Story Flag #993 (0x03E1) - JP 805ACDC7 0x80 / US 805A9B47 0x80
     */
    STORYFLAG_993,

    /** []
     *  Story Flag #994 (0x03E2) - JP 805ACDC6 0x01 / US 805A9B46 0x01
     */
    STORYFLAG_994,

    /** []
     *  Story Flag #995 (0x03E3) - JP 805ACDC6 0x02 / US 805A9B46 0x02
     */
    STORYFLAG_995,

    /** []
     *  Story Flag #996 (0x03E4) - JP 805ACDC6 0x04 / US 805A9B46 0x04
     */
    STORYFLAG_996,

    /** []
     *  Story Flag #997 (0x03E5) - JP 805ACDC6 0x08 / US 805A9B46 0x08
     */
    STORYFLAG_997,

    /** []
     *  Story Flag #998 (0x03E6) - JP 805ACDC6 0x10 / US 805A9B46 0x10
     */
    STORYFLAG_998,

    /** []
     *  Story Flag #999 (0x03E7) - JP 805ACDC6 0x20 / US 805A9B46 0x20
     */
    STORYFLAG_999,

    /** []
     *  Story Flag #1000 (0x03E8) - JP 805ACDC6 0x40 / US 805A9B46 0x40
     */
    STORYFLAG_1000,

    /** []
     *  Story Flag #1001 (0x03E9) - JP 805ACDC6 0x80 / US 805A9B46 0x80
     */
    STORYFLAG_1001,

    /** []
     *  Story Flag #1002 (0x03EA) - JP 805ACDC9 0x01 / US 805A9B49 0x01
     */
    STORYFLAG_1002,

    /** []
     *  Story Flag #1003 (0x03EB) - JP 805ACDC9 0x02 / US 805A9B49 0x02
     */
    STORYFLAG_1003,

    /** []
     *  Story Flag #1004 (0x03EC) - JP 805ACDC9 0x04 / US 805A9B49 0x04
     */
    STORYFLAG_1004,

    /** []
     *  Story Flag #1005 (0x03ED) - JP 805ACDC9 0x08 / US 805A9B49 0x08
     */
    STORYFLAG_1005,

    /** []
     *  Story Flag #1006 (0x03EE) - JP 805ACDC9 0x10 / US 805A9B49 0x10
     */
    STORYFLAG_1006,

    /** []
     *  Story Flag #1007 (0x03EF) - JP 805ACDC9 0x20 / US 805A9B49 0x20
     */
    STORYFLAG_1007,

    /** []
     *  Story Flag #1008 (0x03F0) - JP 805ACDC9 0x40 / US 805A9B49 0x40
     */
    STORYFLAG_1008,

    /** []
     *  Story Flag #1009 (0x03F1) - JP 805ACDC9 0x80 / US 805A9B49 0x80
     */
    STORYFLAG_1009,

    /** []
     *  Story Flag #1010 (0x03F2) - JP 805ACDC8 0x01 / US 805A9B48 0x01
     */
    STORYFLAG_1010,

    /** []
     *  Story Flag #1011 (0x03F3) - JP 805ACDC8 0x02 / US 805A9B48 0x02
     */
    STORYFLAG_1011,

    /** []
     *  Story Flag #1012 (0x03F4) - JP 805ACDC8 0x04 / US 805A9B48 0x04
     */
    STORYFLAG_1012,

    /** []
     *  Story Flag #1013 (0x03F5) - JP 805ACDC8 0x08 / US 805A9B48 0x08
     */
    STORYFLAG_1013,

    /** []
     *  Story Flag #1014 (0x03F6) - JP 805ACDC8 0x10 / US 805A9B48 0x10
     */
    STORYFLAG_1014,

    /** []
     *  Story Flag #1015 (0x03F7) - JP 805ACDC8 0x20 / US 805A9B48 0x20
     */
    STORYFLAG_1015,

    /** []
     *  Story Flag #1016 (0x03F8) - JP 805ACDC8 0x40 / US 805A9B48 0x40
     */
    STORYFLAG_1016,

    /** []
     *  Story Flag #1017 (0x03F9) - JP 805ACDC8 0x80 / US 805A9B48 0x80
     */
    STORYFLAG_1017,

    /** []
     *  Story Flag #1018 (0x03FA) - JP 805ACDCB 0x01 / US 805A9B4B 0x01
     */
    STORYFLAG_1018,

    /** []
     *  Story Flag #1019 (0x03FB) - JP 805ACDCB 0x02 / US 805A9B4B 0x02
     */
    STORYFLAG_1019,

    /** []
     *  Story Flag #1020 (0x03FC) - JP 805ACDCB 0x04 / US 805A9B4B 0x04
     */
    STORYFLAG_1020,

    /** []
     *  Story Flag #1021 (0x03FD) - JP 805ACDCB 0x08 / US 805A9B4B 0x08
     */
    STORYFLAG_1021,

    /** []
     *  Story Flag #1022 (0x03FE) - JP 805ACDCB 0x10 / US 805A9B4B 0x10
     */
    STORYFLAG_1022,

    /** []
     *  Story Flag #1023 (0x03FF) - JP 805ACDCB 0x20 / US 805A9B4B 0x20
     */
    STORYFLAG_1023,

    /** []
     *  Story Flag #1024 (0x0400) - JP 805ACDCB 0x40 / US 805A9B4B 0x40
     */
    STORYFLAG_1024,

    /** []
     *  Story Flag #1025 (0x0401) - JP 805ACDCB 0x80 / US 805A9B4B 0x80
     */
    STORYFLAG_1025,

    /** []
     *  Story Flag #1026 (0x0402) - JP 805ACDCA 0x01 / US 805A9B4A 0x01
     */
    STORYFLAG_1026,

    /** []
     *  Story Flag #1027 (0x0403) - JP 805ACDCA 0x02 / US 805A9B4A 0x02
     */
    STORYFLAG_1027,

    /** []
     *  Story Flag #1028 (0x0404) - JP 805ACDCA 0x04 / US 805A9B4A 0x04
     */
    STORYFLAG_1028,

    /** []
     *  Story Flag #1029 (0x0405) - JP 805ACDCA 0x08 / US 805A9B4A 0x08
     */
    STORYFLAG_1029,

    /** []
     *  Story Flag #1030 (0x0406) - JP 805ACDCA 0x10 / US 805A9B4A 0x10
     */
    STORYFLAG_1030,

    /** []
     *  Story Flag #1031 (0x0407) - JP 805ACDCA 0x20 / US 805A9B4A 0x20
     */
    STORYFLAG_1031,

    /** []
     *  Story Flag #1032 (0x0408) - JP 805ACDCA 0x40 / US 805A9B4A 0x40
     */
    STORYFLAG_1032,

    /** []
     *  Story Flag #1033 (0x0409) - JP 805ACDCA 0x80 / US 805A9B4A 0x80
     */
    STORYFLAG_1033,

    /** []
     *  Story Flag #1034 (0x040A) - JP 805ACDCD 0x01 / US 805A9B4D 0x01
     */
    STORYFLAG_1034,

    /** []
     *  Story Flag #1035 (0x040B) - JP 805ACDCD 0x02 / US 805A9B4D 0x02
     */
    STORYFLAG_1035,

    /** []
     *  Story Flag #1036 (0x040C) - JP 805ACDCD 0x04 / US 805A9B4D 0x04
     */
    STORYFLAG_1036,

    /** []
     *  Story Flag #1037 (0x040D) - JP 805ACDCD 0x08 / US 805A9B4D 0x08
     */
    STORYFLAG_1037,

    /** []
     *  Story Flag #1038 (0x040E) - JP 805ACDCD 0x10 / US 805A9B4D 0x10
     */
    STORYFLAG_1038,

    /** []
     *  Story Flag #1039 (0x040F) - JP 805ACDCD 0x20 / US 805A9B4D 0x20
     */
    STORYFLAG_1039,

    /** []
     *  Story Flag #1040 (0x0410) - JP 805ACDCD 0x40 / US 805A9B4D 0x40
     */
    STORYFLAG_1040,

    /** []
     *  Story Flag #1041 (0x0411) - JP 805ACDCD 0x80 / US 805A9B4D 0x80
     */
    STORYFLAG_1041,

    /** []
     *  Story Flag #1042 (0x0412) - JP 805ACDCC 0x01 / US 805A9B4C 0x01
     */
    STORYFLAG_1042,

    /** []
     *  Story Flag #1043 (0x0413) - JP 805ACDCC 0x02 / US 805A9B4C 0x02
     */
    STORYFLAG_1043,

    /** []
     *  Story Flag #1044 (0x0414) - JP 805ACDCC 0x04 / US 805A9B4C 0x04
     */
    STORYFLAG_1044,

    /** []
     *  Story Flag #1045 (0x0415) - JP 805ACDCC 0x08 / US 805A9B4C 0x08
     */
    STORYFLAG_1045,

    /** []
     *  Story Flag #1046 (0x0416) - JP 805ACDCC 0x10 / US 805A9B4C 0x10
     */
    STORYFLAG_1046,

    /** []
     *  Story Flag #1047 (0x0417) - JP 805ACDCC 0x20 / US 805A9B4C 0x20
     */
    STORYFLAG_1047,

    /** []
     *  Story Flag #1048 (0x0418) - JP 805ACDCC 0x40 / US 805A9B4C 0x40
     */
    STORYFLAG_1048,

    /** []
     *  Story Flag #1049 (0x0419) - JP 805ACDCC 0x80 / US 805A9B4C 0x80
     */
    STORYFLAG_1049,

    /** []
     *  Story Flag #1050 (0x041A) - JP 805ACDCF 0x01 / US 805A9B4F 0x01
     */
    STORYFLAG_1050,

    /** []
     *  Story Flag #1051 (0x041B) - JP 805ACDCF 0x02 / US 805A9B4F 0x02
     */
    STORYFLAG_1051,

    /** []
     *  Story Flag #1052 (0x041C) - JP 805ACDCF 0x04 / US 805A9B4F 0x04
     */
    STORYFLAG_1052,

    /** []
     *  Story Flag #1053 (0x041D) - JP 805ACDCF 0x08 / US 805A9B4F 0x08
     */
    STORYFLAG_1053,

    /** []
     *  Story Flag #1054 (0x041E) - JP 805ACDCF 0x10 / US 805A9B4F 0x10
     */
    STORYFLAG_1054,

    /** []
     *  Story Flag #1055 (0x041F) - JP 805ACDCF 0x20 / US 805A9B4F 0x20
     */
    STORYFLAG_1055,

    /** []
     *  Story Flag #1056 (0x0420) - JP 805ACDCF 0x40 / US 805A9B4F 0x40
     */
    STORYFLAG_1056,

    /** []
     *  Story Flag #1057 (0x0421) - JP 805ACDCF 0x80 / US 805A9B4F 0x80
     */
    STORYFLAG_1057,

    /** []
     *  Story Flag #1058 (0x0422) - JP 805ACDCE 0x01 / US 805A9B4E 0x01
     */
    STORYFLAG_1058,

    /** []
     *  Story Flag #1059 (0x0423) - JP 805ACDCE 0x02 / US 805A9B4E 0x02
     */
    STORYFLAG_1059,

    /** []
     *  Story Flag #1060 (0x0424) - JP 805ACDCE 0x04 / US 805A9B4E 0x04
     */
    STORYFLAG_1060,

    /** []
     *  Story Flag #1061 (0x0425) - JP 805ACDCE 0x08 / US 805A9B4E 0x08
     */
    STORYFLAG_1061,

    /** []
     *  Story Flag #1062 (0x0426) - JP 805ACDCE 0x10 / US 805A9B4E 0x10
     */
    STORYFLAG_1062,

    /** []
     *  Story Flag #1063 (0x0427) - JP 805ACDCE 0x20 / US 805A9B4E 0x20
     */
    STORYFLAG_1063,

    /** []
     *  Story Flag #1064 (0x0428) - JP 805ACDCE 0x40 / US 805A9B4E 0x40
     */
    STORYFLAG_1064,

    /** []
     *  Story Flag #1065 (0x0429) - JP 805ACDCE 0x80 / US 805A9B4E 0x80
     */
    STORYFLAG_1065,

    /** []
     *  Story Flag #1066 (0x042A) - JP 805ACDD1 0x01 / US 805A9B51 0x01
     */
    STORYFLAG_1066,

    /** []
     *  Story Flag #1067 (0x042B) - JP 805ACDD1 0x02 / US 805A9B51 0x02
     */
    STORYFLAG_1067,

    /** []
     *  Story Flag #1068 (0x042C) - JP 805ACDD1 0x04 / US 805A9B51 0x04
     */
    STORYFLAG_1068,

    /** []
     *  Story Flag #1069 (0x042D) - JP 805ACDD1 0x08 / US 805A9B51 0x08
     */
    STORYFLAG_1069,

    /** []
     *  Story Flag #1070 (0x042E) - JP 805ACDD1 0x10 / US 805A9B51 0x10
     */
    STORYFLAG_1070,

    /** []
     *  Story Flag #1071 (0x042F) - JP 805ACDD1 0x20 / US 805A9B51 0x20
     */
    STORYFLAG_1071,

    /** []
     *  Story Flag #1072 (0x0430) - JP 805ACDD1 0x40 / US 805A9B51 0x40
     */
    STORYFLAG_1072,

    /** []
     *  Story Flag #1073 (0x0431) - JP 805ACDD1 0x80 / US 805A9B51 0x80
     */
    STORYFLAG_1073,

    /** []
     *  Story Flag #1074 (0x0432) - JP 805ACDD0 0x01 / US 805A9B50 0x01
     */
    STORYFLAG_1074,

    /** [Getting text from Potion lady about Air Potion]
     *  Story Flag #1075 (0x0433) - JP 805ACDD0 0x02 / US 805A9B50 0x02
     */
    STORYFLAG_1075,

    /** [Sets when reaching Boko Camp near Thrill Digger (layer 1 only)]
     *  Story Flag #1076 (0x0434) - JP 805ACDD0 0x04 / US 805A9B50 0x04
     */
    STORYFLAG_1076,

    /** [Set when reaching the Room with the Key Piece after the Sand Slide (layer 1 only)]
     *  Story Flag #1077 (0x0435) - JP 805ACDD0 0x08 / US 805A9B50 0x08
     */
    STORYFLAG_1077,

    /** [Set when beeing near Bridge to VS on ET side (layer 1 only)]
     *  Story Flag #1078 (0x0436) - JP 805ACDD0 0x10 / US 805A9B50 0x10
     */
    STORYFLAG_1078,

    /** [Set when beeing near Bridge to VS on VS side (layer 1 only)]
     *  Story Flag #1079 (0x0437) - JP 805ACDD0 0x20 / US 805A9B50 0x20
     */
    STORYFLAG_1079,

    /** [Sets before entering the Room with the Key Piece after the Sand Slide (layer 1 only)]
     *  Story Flag #1080 (0x0438) - JP 805ACDD0 0x40 / US 805A9B50 0x40
     */
    STORYFLAG_1080,

    /** [Set when entering the Area after climbing intial log in Faron woods]
     *  Story Flag #1081 (0x0439) - JP 805ACDD0 0x80 / US 805A9B50 0x80
     */
    STORYFLAG_1081,

    /** [Healing Orielle's bird]
     *  Story Flag #1082 (0x043A) - JP 805ACDD3 0x01 / US 805A9B53 0x01
     */
    STORYFLAG_1082,

    /** [[sets in Room 10 in Sandship]]
     *  Story Flag #1083 (0x043B) - JP 805ACDD3 0x02 / US 805A9B53 0x02
     */
    STORYFLAG_1083,

    /** [Sets near Desert intro cs]
     *  Story Flag #1084 (0x043C) - JP 805ACDD3 0x04 / US 805A9B53 0x04
     */
    STORYFLAG_1084,

    /** [Sets in ToT area  when near Exit to Sand Oasis]
     *  Story Flag #1085 (0x043D) - JP 805ACDD3 0x08 / US 805A9B53 0x08
     */
    STORYFLAG_1085,

    /** [Sets when entering or leaveing LMF Boss Room through Boss Door]
     *  Story Flag #1086 (0x043E) - JP 805ACDD3 0x10 / US 805A9B53 0x10
     */
    STORYFLAG_1086,

    /** [Goddess Chest Count]
     *  Story Flag #1087 (0x043F) - JP from 805ACE01 0x40 to 805ACE00 0x04 / US from 805A9B81 0x40 to 805A9B80 0x04
     */
    STORYFLAG_GODDESS_CHEST_COUNT,

    /** []
     *  Story Flag #1088 (0x0440) - JP 805ACDD3 0x40 / US 805A9B53 0x40
     */
    STORYFLAG_1088,

    /** [Talking to Beedle in his shop]
     *  Story Flag #1089 (0x0441) - JP 805ACDD3 0x80 / US 805A9B53 0x80
     */
    STORYFLAG_1089,

    /** []
     *  Story Flag #1090 (0x0442) - JP 805ACDD2 0x01 / US 805A9B52 0x01
     */
    STORYFLAG_1090,

    /** [Talking to Batreaux after he truns human]
     *  Story Flag #1091 (0x0443) - JP 805ACDD2 0x02 / US 805A9B52 0x02
     */
    STORYFLAG_1091,

    /** [[talking to some mogma at some point]]
     *  Story Flag #1092 (0x0444) - JP 805ACDD2 0x04 / US 805A9B52 0x04
     */
    STORYFLAG_1092,

    /** [Bug minigame related]
     *  Story Flag #1093 (0x0445) - JP 805ACDD2 0x08 / US 805A9B52 0x08
     */
    STORYFLAG_1093,

    /** [Talking to Oolo after Flooded Faron]
     *  Story Flag #1094 (0x0446) - JP 805ACDD2 0x10 / US 805A9B52 0x10
     */
    STORYFLAG_1094,

    /** [Goddess shield text]
     *  Story Flag #1095 (0x0447) - JP 805ACDD2 0x20 / US 805A9B52 0x20
     */
    STORYFLAG_1095,

    /** []
     *  Story Flag #1096 (0x0448) - JP 805ACDD2 0x40 / US 805A9B52 0x40
     */
    STORYFLAG_1096,

    /** []
     *  Story Flag #1097 (0x0449) - JP 805ACDD2 0x80 / US 805A9B52 0x80
     */
    STORYFLAG_1097,

    /** [ Set in a CS in the Skykeep Entrance Room (Courage Lever Room Layer 2)]
     *  Story Flag #1098 (0x044A) - JP 805ACDD5 0x01 / US 805A9B55 0x01
     */
    STORYFLAG_1098,

    /** [Sets {when going past Gate / when beeing near Chest} in first Left Room of LMF]
     *  Story Flag #1099 (0x044B) - JP 805ACDD5 0x02 / US 805A9B55 0x02
     */
    STORYFLAG_1099,

    /** [Set when beeing were the bombs are for lowering the lava to the Mogma Turf]
     *  Story Flag #1100 (0x044C) - JP 805ACDD5 0x04 / US 805A9B55 0x04
     */
    STORYFLAG_1100,

    /** [Set when going into the Sink Sand labyrinth from the beginning of the LMF Main room]
     *  Story Flag #1101 (0x044D) - JP 805ACDD5 0x08 / US 805A9B55 0x08
     */
    STORYFLAG_1101,

    /** []
     *  Story Flag #1102 (0x044E) - JP 805ACDD5 0x10 / US 805A9B55 0x10
     */
    STORYFLAG_1102,

    /** []
     *  Story Flag #1103 (0x044F) - JP 805ACDD5 0x20 / US 805A9B55 0x20
     */
    STORYFLAG_1103,

    /** []
     *  Story Flag #1104 (0x0450) - JP 805ACDD5 0x40 / US 805A9B55 0x40
     */
    STORYFLAG_1104,

    /** []
     *  Story Flag #1105 (0x0451) - JP 805ACDD5 0x80 / US 805A9B55 0x80
     */
    STORYFLAG_1105,

    /** []
     *  Story Flag #1106 (0x0452) - JP 805ACDD4 0x01 / US 805A9B54 0x01
     */
    STORYFLAG_1106,

    /** []
     *  Story Flag #1107 (0x0453) - JP 805ACDD4 0x02 / US 805A9B54 0x02
     */
    STORYFLAG_1107,

    /** [Fi text about that you need to get the propeller]
     *  Story Flag #1108 (0x0454) - JP 805ACDD4 0x04 / US 805A9B54 0x04
     */
    STORYFLAG_1108,

    /** [[set by CS in ET]]
     *  Story Flag #1109 (0x0455) - JP 805ACDD4 0x08 / US 805A9B54 0x08
     */
    STORYFLAG_1109,

    /** [Failing to land once after getting the Sailcloth]
     *  Story Flag #1110 (0x0456) - JP 805ACDD4 0x10 / US 805A9B54 0x10
     */
    STORYFLAG_1110,

    /** [Sets when activating the mogma talking in right room of skyview]
     *  Story Flag #1111 (0x0457) - JP 805ACDD4 0x20 / US 805A9B54 0x20
     */
    STORYFLAG_1111,

    /** [Sets when talking to mogma in left room of skyview]
     *  Story Flag #1112 (0x0458) - JP 805ACDD4 0x40 / US 805A9B54 0x40
     */
    STORYFLAG_1112,

    /** [Set when beeing past the Tree in the ToT Area (towards ToT)]
     *  Story Flag #1113 (0x0459) - JP 805ACDD4 0x80 / US 805A9B54 0x80
     */
    STORYFLAG_1113,

    /** [Bow text]
     *  Story Flag #1114 (0x045A) - JP 805ACDD7 0x01 / US 805A9B57 0x01
     */
    STORYFLAG_1114,

    /** [extra bomb bag text]
     *  Story Flag #1115 (0x045B) - JP 805ACDD7 0x02 / US 805A9B57 0x02
     */
    STORYFLAG_1115,

    /** [Talk to kukiel after Betreaux turns Human]
     *  Story Flag #1116 (0x045C) - JP 805ACDD7 0x04 / US 805A9B57 0x04
     */
    STORYFLAG_1116,

    /** []
     *  Story Flag #1117 (0x045D) - JP 805ACDD7 0x08 / US 805A9B57 0x08
     */
    STORYFLAG_1117,

    /** []
     *  Story Flag #1118 (0x045E) - JP 805ACDD7 0x10 / US 805A9B57 0x10
     */
    STORYFLAG_1118,

    /** []
     *  Story Flag #1119 (0x045F) - JP 805ACDD7 0x20 / US 805A9B57 0x20
     */
    STORYFLAG_1119,

    /** [Sets in a Area in the FS Room in Skykeep]
     *  Story Flag #1120 (0x0460) - JP 805ACDD7 0x40 / US 805A9B57 0x40
     */
    STORYFLAG_1120,

    /** [Sets at end of Sink Sand that you would normaly go trough after Hoook Beetle]
     *  Story Flag #1121 (0x0461) - JP 805ACDD7 0x80 / US 805A9B57 0x80
     */
    STORYFLAG_1121,

    /** []
     *  Story Flag #1122 (0x0462) - JP 805ACDD6 0x01 / US 805A9B56 0x01
     */
    STORYFLAG_1122,

    /** []
     *  Story Flag #1123 (0x0463) - JP 805ACDD6 0x02 / US 805A9B56 0x02
     */
    STORYFLAG_1123,

    /** []
     *  Story Flag #1124 (0x0464) - JP 805ACDD6 0x04 / US 805A9B56 0x04
     */
    STORYFLAG_1124,

    /** []
     *  Story Flag #1125 (0x0465) - JP 805ACDD6 0x08 / US 805A9B56 0x08
     */
    STORYFLAG_1125,

    /** []
     *  Story Flag #1126 (0x0466) - JP 805ACDD6 0x10 / US 805A9B56 0x10
     */
    STORYFLAG_1126,

    /** []
     *  Story Flag #1127 (0x0467) - JP 805ACDD6 0x20 / US 805A9B56 0x20
     */
    STORYFLAG_1127,

    /** []
     *  Story Flag #1128 (0x0468) - JP 805ACDD6 0x40 / US 805A9B56 0x40
     */
    STORYFLAG_1128,

    /** []
     *  Story Flag #1129 (0x0469) - JP 805ACDD6 0x80 / US 805A9B56 0x80
     */
    STORYFLAG_1129,

    /** []
     *  Story Flag #1130 (0x046A) - JP 805ACDD9 0x01 / US 805A9B59 0x01
     */
    STORYFLAG_1130,

    /** []
     *  Story Flag #1131 (0x046B) - JP 805ACDD9 0x02 / US 805A9B59 0x02
     */
    STORYFLAG_1131,

    /** []
     *  Story Flag #1132 (0x046C) - JP 805ACDD9 0x04 / US 805A9B59 0x04
     */
    STORYFLAG_1132,

    /** []
     *  Story Flag #1133 (0x046D) - JP 805ACDD9 0x08 / US 805A9B59 0x08
     */
    STORYFLAG_1133,

    /** []
     *  Story Flag #1134 (0x046E) - JP 805ACDD9 0x10 / US 805A9B59 0x10
     */
    STORYFLAG_1134,

    /** []
     *  Story Flag #1135 (0x046F) - JP 805ACDD9 0x20 / US 805A9B59 0x20
     */
    STORYFLAG_1135,

    /** []
     *  Story Flag #1136 (0x0470) - JP 805ACDD9 0x40 / US 805A9B59 0x40
     */
    STORYFLAG_1136,

    /** []
     *  Story Flag #1137 (0x0471) - JP 805ACDD9 0x80 / US 805A9B59 0x80
     */
    STORYFLAG_1137,

    /** []
     *  Story Flag #1138 (0x0472) - JP 805ACDD8 0x01 / US 805A9B58 0x01
     */
    STORYFLAG_1138,

    /** []
     *  Story Flag #1139 (0x0473) - JP 805ACDD8 0x02 / US 805A9B58 0x02
     */
    STORYFLAG_1139,

    /** []
     *  Story Flag #1140 (0x0474) - JP 805ACDD8 0x04 / US 805A9B58 0x04
     */
    STORYFLAG_1140,

    /** []
     *  Story Flag #1141 (0x0475) - JP 805ACDD8 0x08 / US 805A9B58 0x08
     */
    STORYFLAG_1141,

    /** []
     *  Story Flag #1142 (0x0476) - JP 805ACDD8 0x10 / US 805A9B58 0x10
     */
    STORYFLAG_1142,

    /** []
     *  Story Flag #1143 (0x0477) - JP 805ACDD8 0x20 / US 805A9B58 0x20
     */
    STORYFLAG_1143,

    /** []
     *  Story Flag #1144 (0x0478) - JP 805ACDD8 0x40 / US 805A9B58 0x40
     */
    STORYFLAG_1144,

    /** []
     *  Story Flag #1145 (0x0479) - JP 805ACDD8 0x80 / US 805A9B58 0x80
     */
    STORYFLAG_1145,

    /** []
     *  Story Flag #1146 (0x047A) - JP 805ACDDB 0x01 / US 805A9B5B 0x01
     */
    STORYFLAG_1146,

    /** []
     *  Story Flag #1147 (0x047B) - JP 805ACDDB 0x02 / US 805A9B5B 0x02
     */
    STORYFLAG_1147,

    /** []
     *  Story Flag #1148 (0x047C) - JP 805ACDDB 0x04 / US 805A9B5B 0x04
     */
    STORYFLAG_1148,

    /** []
     *  Story Flag #1149 (0x047D) - JP 805ACDDB 0x08 / US 805A9B5B 0x08
     */
    STORYFLAG_1149,

    /** []
     *  Story Flag #1150 (0x047E) - JP 805ACDDB 0x10 / US 805A9B5B 0x10
     */
    STORYFLAG_1150,

    /** []
     *  Story Flag #1151 (0x047F) - JP 805ACDDB 0x20 / US 805A9B5B 0x20
     */
    STORYFLAG_1151,

    /** []
     *  Story Flag #1152 (0x0480) - JP 805ACDDB 0x40 / US 805A9B5B 0x40
     */
    STORYFLAG_1152,

    /** []
     *  Story Flag #1153 (0x0481) - JP 805ACDDB 0x80 / US 805A9B5B 0x80
     */
    STORYFLAG_1153,

    /** []
     *  Story Flag #1154 (0x0482) - JP 805ACDDA 0x01 / US 805A9B5A 0x01
     */
    STORYFLAG_1154,

    /** [Set from Fi after talking to Erla]
     *  Story Flag #1155 (0x0483) - JP 805ACDDA 0x02 / US 805A9B5A 0x02
     */
    STORYFLAG_1155,

    /** []
     *  Story Flag #1156 (0x0484) - JP 805ACDDA 0x04 / US 805A9B5A 0x04
     */
    STORYFLAG_1156,

    /** []
     *  Story Flag #1157 (0x0485) - JP 805ACDDA 0x08 / US 805A9B5A 0x08
     */
    STORYFLAG_1157,

    /** []
     *  Story Flag #1158 (0x0486) - JP 805ACDDA 0x10 / US 805A9B5A 0x10
     */
    STORYFLAG_1158,

    /** []
     *  Story Flag #1159 (0x0487) - JP 805ACDDA 0x20 / US 805A9B5A 0x20
     */
    STORYFLAG_1159,

    /** []
     *  Story Flag #1160 (0x0488) - JP 805ACDDA 0x40 / US 805A9B5A 0x40
     */
    STORYFLAG_1160,

    /** []
     *  Story Flag #1161 (0x0489) - JP 805ACDDA 0x80 / US 805A9B5A 0x80
     */
    STORYFLAG_1161,

    /** []
     *  Story Flag #1162 (0x048A) - JP 805ACDDD 0x01 / US 805A9B5D 0x01
     */
    STORYFLAG_1162,

    /** []
     *  Story Flag #1163 (0x048B) - JP 805ACDDD 0x02 / US 805A9B5D 0x02
     */
    STORYFLAG_1163,

    /** []
     *  Story Flag #1164 (0x048C) - JP 805ACDDD 0x04 / US 805A9B5D 0x04
     */
    STORYFLAG_1164,

    /** []
     *  Story Flag #1165 (0x048D) - JP 805ACDDD 0x08 / US 805A9B5D 0x08
     */
    STORYFLAG_1165,

    /** []
     *  Story Flag #1166 (0x048E) - JP 805ACDDD 0x10 / US 805A9B5D 0x10
     */
    STORYFLAG_1166,

    /** []
     *  Story Flag #1167 (0x048F) - JP 805ACDDD 0x20 / US 805A9B5D 0x20
     */
    STORYFLAG_1167,

    /** []
     *  Story Flag #1168 (0x0490) - JP 805ACDDD 0x40 / US 805A9B5D 0x40
     */
    STORYFLAG_1168,

    /** [beeing were you blow up the boxes in Main Room]
     *  Story Flag #1169 (0x0491) - JP 805ACDDD 0x80 / US 805A9B5D 0x80
     */
    STORYFLAG_1169,

    /** [Set in LMF BK room near when entering the Puzzle solution side]
     *  Story Flag #1170 (0x0492) - JP 805ACDDC 0x01 / US 805A9B5C 0x01
     */
    STORYFLAG_1170,

    /** []
     *  Story Flag #1171 (0x0493) - JP 805ACDDC 0x02 / US 805A9B5C 0x02
     */
    STORYFLAG_1171,

    /** []
     *  Story Flag #1172 (0x0494) - JP 805ACDDC 0x04 / US 805A9B5C 0x04
     */
    STORYFLAG_1172,

    /** []
     *  Story Flag #1173 (0x0495) - JP 805ACDDC 0x08 / US 805A9B5C 0x08
     */
    STORYFLAG_1173,

    /** []
     *  Story Flag #1174 (0x0496) - JP 805ACDDC 0x10 / US 805A9B5C 0x10
     */
    STORYFLAG_1174,

    /** []
     *  Story Flag #1175 (0x0497) - JP 805ACDDC 0x20 / US 805A9B5C 0x20
     */
    STORYFLAG_1175,

    /** []
     *  Story Flag #1176 (0x0498) - JP 805ACDDC 0x40 / US 805A9B5C 0x40
     */
    STORYFLAG_1176,

    /** []
     *  Story Flag #1177 (0x0499) - JP 805ACDDC 0x80 / US 805A9B5C 0x80
     */
    STORYFLAG_1177,

    /** []
     *  Story Flag #1178 (0x049A) - JP 805ACDDF 0x01 / US 805A9B5F 0x01
     */
    STORYFLAG_1178,

    /** []
     *  Story Flag #1179 (0x049B) - JP 805ACDDF 0x02 / US 805A9B5F 0x02
     */
    STORYFLAG_1179,

    /** []
     *  Story Flag #1180 (0x049C) - JP 805ACDDF 0x04 / US 805A9B5F 0x04
     */
    STORYFLAG_1180,

    /** []
     *  Story Flag #1181 (0x049D) - JP 805ACDDF 0x08 / US 805A9B5F 0x08
     */
    STORYFLAG_1181,

    /** []
     *  Story Flag #1182 (0x049E) - JP 805ACDDF 0x10 / US 805A9B5F 0x10
     */
    STORYFLAG_1182,

    /** []
     *  Story Flag #1183 (0x049F) - JP 805ACDDF 0x20 / US 805A9B5F 0x20
     */
    STORYFLAG_1183,

    /** []
     *  Story Flag #1184 (0x04A0) - JP 805ACDDF 0x40 / US 805A9B5F 0x40
     */
    STORYFLAG_1184,

    /** []
     *  Story Flag #1185 (0x04A1) - JP 805ACDDF 0x80 / US 805A9B5F 0x80
     */
    STORYFLAG_1185,

    /** []
     *  Story Flag #1186 (0x04A2) - JP 805ACDDE 0x01 / US 805A9B5E 0x01
     */
    STORYFLAG_1186,

    /** []
     *  Story Flag #1187 (0x04A3) - JP 805ACDDE 0x02 / US 805A9B5E 0x02
     */
    STORYFLAG_1187,

    /** []
     *  Story Flag #1188 (0x04A4) - JP 805ACDDE 0x04 / US 805A9B5E 0x04
     */
    STORYFLAG_1188,

    /** []
     *  Story Flag #1189 (0x04A5) - JP 805ACDDE 0x08 / US 805A9B5E 0x08
     */
    STORYFLAG_1189,

    /** []
     *  Story Flag #1190 (0x04A6) - JP 805ACDDE 0x10 / US 805A9B5E 0x10
     */
    STORYFLAG_1190,

    /** []
     *  Story Flag #1191 (0x04A7) - JP 805ACDDE 0x20 / US 805A9B5E 0x20
     */
    STORYFLAG_1191,

    /** [Sets in a Area near Waterfall in AC]
     *  Story Flag #1192 (0x04A8) - JP 805ACDDE 0x40 / US 805A9B5E 0x40
     */
    STORYFLAG_1192,

    /** []
     *  Story Flag #1193 (0x04A9) - JP 805ACDD3 0x20 / US 805A9B53 0x20
     */
    STORYFLAG_1193,

    /** [Entering the Statue in AC from main floor?]
     *  Story Flag #1194 (0x04AA) - JP 805ACDDE 0x80 / US 805A9B5E 0x80
     */
    STORYFLAG_1194,

    /** [Stone Tablet in AC giveing the Sword Lock Puzzle hint]
     *  Story Flag #1195 (0x04AB) - JP 805ACDE1 0x02 / US 805A9B61 0x02
     */
    STORYFLAG_1195,

    /** [Sets were West Desert Statue is]
     *  Story Flag #1196 (0x04AC) - JP 805ACDE1 0x04 / US 805A9B61 0x04
     */
    STORYFLAG_1196,

    /** [Beeing near Minecart track to ToT when comeing from North Desert Entry]
     *  Story Flag #1197 (0x04AD) - JP 805ACDE1 0x08 / US 805A9B61 0x08
     */
    STORYFLAG_1197,

    /** [Set Before entering ToT area from near 5 beacons trigger]
     *  Story Flag #1198 (0x04AE) - JP 805ACDE1 0x10 / US 805A9B61 0x10
     */
    STORYFLAG_1198,

    /** [On top of Wall Between LMF and 5 beacons / on top of Wall Between Clawshots trial and 5 Beacons trigger]
     *  Story Flag #1199 (0x04AF) - JP 805ACDE1 0x20 / US 805A9B61 0x20
     */
    STORYFLAG_1199,

    /** [sleeping after Batreaux truns Human]
     *  Story Flag #1200 (0x04B0) - JP 805ACDE1 0x40 / US 805A9B61 0x40
     */
    STORYFLAG_1200,

    STORYFLAG_MAX,
};

#endif
