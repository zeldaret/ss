#include "d/a/obj/d_a_obj_tbox.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_room.h"
#include "d/d_stage.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/flag/tboxflag_manager.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_scnleaf.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/blur_and_palette_manager.h"
#include "toBeSorted/counters/goddess_chest_counter.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(TBOX, dAcTbox_c, fProfile::TBOX, 0x018D, 0, 6);

static mVec3_c sDowsingTargetOffsets[] = {
    mVec3_c(0.0f, 54.0f, 0.0f), mVec3_c(0.0f, 34.0f, 0.0f), mVec3_c(0.0f, 66.5f, 0.0f), mVec3_c(0.0f, 49.0f, 0.0f)
};

static mVec3_c sVec1 = mVec3_c(0.0f, 0.0f, -2.0f);
static mVec3_c sVec2 = mVec3_c(1.0f, 1.0f, 0.8f);
static mVec3_c sVec3 = mVec3_c(1.0f, 1.0f, 1.0f);
static mVec3_c sVec4 = mVec3_c(0.0f, 0.0f, 88.0f);

STATE_DEFINE(dAcTbox_c, DugOut);
STATE_DEFINE(dAcTbox_c, WaitAppear);
STATE_DEFINE(dAcTbox_c, DemoAppear);
STATE_DEFINE(dAcTbox_c, WaitOpen);
STATE_DEFINE(dAcTbox_c, GoddessWaitOff);
STATE_DEFINE(dAcTbox_c, GoddessWaitOn);
STATE_DEFINE(dAcTbox_c, DeleteArchive);
STATE_DEFINE(dAcTbox_c, LoadArchive);
STATE_DEFINE(dAcTbox_c, Open);
STATE_DEFINE(dAcTbox_c, PresentItem);
STATE_DEFINE(dAcTbox_c, Close);
STATE_DEFINE(dAcTbox_c, Wait);
STATE_DEFINE(dAcTbox_c, GoddessWait);

fLiMgBa_c dAcTbox_c::sTboxActorList;
static const char *sCurrentObtainingItemOarcName = nullptr;

static char *const sArcNames[] = {
    "TBoxNormalT",
    "TBoxSmallT",
    "TBoxBossT",
    "GoddessTBox",
};

static const char *const sOpenArcName = "FX_TBoxOpen";
static const char *const sOpenMdlName = "FX_TBoxOpen";
static const char *const sOpenAnmChrName = "FX_TBoxOpen";
static const char *const sOpenAnmTexSrtName = "FX_TBoxOpen";
static const char *const sOpenAnmClrName = "FX_TBoxOpen";

static const char *const sAppearAnmName = "TBoxNormalT_appear";

static char *const sOpenEventNames[] = {
    "TreasureBoxOpen",
    "TreasureBoxOpenSmall",
    "TreasureBoxOpenBoss",
    "TreasureBoxOpen",
};

// clang-format off
// TODO just copied from somewhere
const cCcD_SrcGObj dAcTbox_c::sColSrc = {
  /* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
  /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0, {0, 0x407}, 0, 0},
  /* mObjCo */ {0xE9}
};
// clang-format on

static const char sGetTriForce[] = "GetTriForce";
static const char sGetSwordA[] = "GetSwordA";
static const char sGetHarp[] = "GetHarp";
static const char sGetSirenFlower[] = "GetSirenFlower";
static const char sGetBowA[] = "GetBowA";
static const char sGetHookShot[] = "GetHookShot";
static const char sGetBirdStatue[] = "GetBirdStatue";
static const char sGetKeyBoss2A[] = "GetKeyBoss2A";
static const char sGetKeyBoss2B[] = "GetKeyBoss2B";
static const char sGetKeyBoss2C[] = "GetKeyBoss2C";
static const char sGetKeyKakeraA[] = "GetKeyKakeraA";
static const char sGetKeyBossA[] = "GetKeyBossA";
static const char sGetKeyBossB[] = "GetKeyBossB";
static const char sGetKeyBossC[] = "GetKeyBossC";
static const char sGetGenki[] = "GetGenki";
static const char sGetBottleKinokoA[] = "GetBottleKinokoA";
static const char sGetBomb[] = "GetBomb";
static const char sGetVacuum[] = "GetVacuum";
static const char sGetMap[] = "GetMap";
static const char sGetBeetleA[] = "GetBeetleA";
static const char sGetBottleWater[] = "GetBottleWater";
static const char sGetBottleKinokoB[] = "GetBottleKinokoB";
static const char sGetMoleGloveA[] = "GetMoleGloveA";
static const char sGetSeedSet[] = "GetSeedSet";
static const char sGetBottleMuteki[] = "GetBottleMuteki";
static const char sGetUroko[] = "GetUroko";
static const char sGetMedal[] = "GetMedal";
static const char sGetNetA[] = "GetNetA";
static const char sGetFairy[] = "GetFairy";
static const char sGetBottleHoly[] = "GetBottleHoly";
static const char sGetBeetleB[] = "GetBeetleB";
static const char sGetBeetleC[] = "GetBeetleC";
static const char sGetBeetleD[] = "GetBeetleD";
static const char sGetBottleKusuri[] = "GetBottleKusuri";
static const char sGetBottleKusuriS[] = "GetBottleKusuriS";
static const char sGetBottleGuts[] = "GetBottleGuts";
static const char sGetBottleAir[] = "GetBottleAir";
static const char sGetBottleFairy[] = "GetBottleFairy";
static const char sGetBowB[] = "GetBowB";
static const char sGetBowC[] = "GetBowC";
static const char sGetBombBag[] = "GetBombBag";
static const char sGetRupee[] = "GetRupee";
static const char sGetMoleGloveB[] = "GetMoleGloveB";
static const char sGetPachinkoB[] = "GetPachinkoB";
static const char sGetPurseB[] = "GetPurseB";
static const char sGetPurseC[] = "GetPurseC";
static const char sGetPurseD[] = "GetPurseD";
static const char sGetPurseE[] = "GetPurseE";
static const char sGetPouchA[] = "GetPouchA";
static const char sGetPouchB[] = "GetPouchB";
static const char sGetShieldWood[] = "GetShieldWood";
static const char sGetShieldIron[] = "GetShieldIron";
static const char sGetShieldHoly[] = "GetShieldHoly";
static const char sGetShieldHylia[] = "GetShieldHylia";
static const char sGetBottleRepair[] = "GetBottleRepair";
static const char sGetSpareSeedA[] = "GetSpareSeedA";
static const char sGetSpareSeedB[] = "GetSpareSeedB";
static const char sGetSpareSeedC[] = "GetSpareSeedC";
static const char sGetSpareQuiverA[] = "GetSpareQuiverA";
static const char sGetSpareQuiverB[] = "GetSpareQuiverB";
static const char sGetSpareQuiverC[] = "GetSpareQuiverC";
static const char sGetSpareBombBagA[] = "GetSpareBombBagA";
static const char sGetSpareBombBagB[] = "GetSpareBombBagB";
static const char sGetSpareBombBagC[] = "GetSpareBombBagC";
static const char sGetWhip[] = "GetWhip";
static const char sGetEarring[] = "GetEarring";
static const char sGetNetB[] = "GetNetB";
static const char sGetInsectGrasshopper[] = "GetInsectGrasshopper";
static const char sGetInsectBeetle[] = "GetInsectBeetle";
static const char sGetInsectBee[] = "GetInsectBee";
static const char sGetInsectKamakiri[] = "GetInsectKamakiri";
static const char sGetInsectLadybug[] = "GetInsectLadybug";
static const char sGetInsectButterfly[] = "GetInsectButterfly";
static const char sGetInsectAnt[] = "GetInsectAnt";
static const char sGetInsectCicada[] = "GetInsectCicada";
static const char sGetInsectDragonfly[] = "GetInsectDragonfly";
static const char sGetInsectScarab[] = "GetInsectScarab";
static const char sGetInsectKuwagata[] = "GetInsectKuwagata";
static const char sGetInsectFirefly[] = "GetInsectFirefly";
static const char sGetBottleEmpty[] = "GetBottleEmpty";
static const char sGetKobunALetter[] = "GetKobunALetter";
static const char sGetTerryCage[] = "GetTerryCage";
static const char sGetSozaiA[] = "GetSozaiA";
static const char sGetSozaiB[] = "GetSozaiB";
static const char sGetSozaiC[] = "GetSozaiC";
static const char sGetSozaiD[] = "GetSozaiD";
static const char sGetSozaiE[] = "GetSozaiE";
static const char sGetSozaiF[] = "GetSozaiF";
static const char sGetSozaiG[] = "GetSozaiG";
static const char sGetSozaiH[] = "GetSozaiH";
static const char sGetSozaiI[] = "GetSozaiI";
static const char sGetSozaiJ[] = "GetSozaiJ";
static const char sGetSozaiL[] = "GetSozaiL";
static const char sGetSozaiK[] = "GetSozaiK";
static const char sGetSozaiM[] = "GetSozaiM";
static const char sGetSozaiN[] = "GetSozaiN";
static const char sGetSozaiO[] = "GetSozaiO";
static const char sGetSozaiP[] = "GetSozaiP";
static const char sGetSekibanMapA[] = "GetSekibanMapA";
static const char sGetSekibanMapB[] = "GetSekibanMapB";
static const char sGetSekibanMapC[] = "GetSekibanMapC";
static const char sGetSirenKey[] = "GetSirenKey";
static const char sGetBottleRepairS[] = "GetBottleRepairS";
static const char sGetBottlePumpkin[] = "GetBottlePumpkin";
static const char sGetSeedLife[] = "GetSeedLife";
static const char sGetFruitB[] = "GetFruitB";
static const char sGetSparePurse[] = "GetSparePurse";

static const char *const sItemToArchiveName[MAX_ITEM_ID + 1] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    sGetTriForce,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    sGetSwordA,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    sGetHarp,
    sGetSirenFlower,
    nullptr,
    sGetBowA,
    sGetHookShot,
    sGetBirdStatue,
    nullptr,
    nullptr,
    nullptr,
    sGetKeyBoss2A,
    sGetKeyBoss2B,
    sGetKeyBoss2C,
    sGetKeyKakeraA,
    sGetKeyBossA,
    sGetKeyBossB,
    sGetKeyBossC,
    nullptr,
    nullptr,
    nullptr,
    sGetGenki,
    sGetBottleKinokoA,
    nullptr,
    nullptr,
    nullptr,
    sGetBomb,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    sGetGenki,
    sGetVacuum,
    sGetMap,
    sGetMap,
    nullptr,
    sGetBeetleA,
    sGetBottleWater,
    sGetBottleKinokoB,
    sGetMoleGloveA,
    nullptr,
    nullptr,
    nullptr,
    sGetSeedSet,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    sGetBottleMuteki,
    sGetBottleMuteki,
    nullptr,
    sGetUroko,
    nullptr,
    sGetMedal,
    sGetNetA,
    sGetFairy,
    nullptr,
    sGetBottleHoly,
    sGetBeetleB,
    sGetBeetleC,
    sGetBeetleD,
    sGetBottleKusuri,
    sGetBottleKusuri,
    nullptr,
    sGetBottleKusuriS,
    nullptr,
    nullptr,
    sGetBottleGuts,
    sGetBottleGuts,
    sGetBottleAir,
    sGetBottleAir,
    sGetBottleFairy,
    nullptr,
    sGetBowB,
    sGetBowC,
    sGetBombBag,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    sGetRupee,
    sGetMoleGloveB,
    sGetMedal,
    sGetMedal,
    sGetMedal,
    sGetMedal,
    sGetMedal,
    sGetPachinkoB,
    nullptr,
    nullptr,
    sGetPurseB,
    sGetPurseC,
    sGetPurseD,
    sGetPurseE,
    sGetPouchA,
    sGetPouchB,
    sGetMedal,
    nullptr,
    sGetShieldWood,
    sGetShieldWood,
    sGetShieldWood,
    sGetShieldIron,
    sGetShieldIron,
    sGetShieldIron,
    sGetShieldHoly,
    sGetShieldHoly,
    sGetShieldHoly,
    sGetShieldHylia,
    sGetBottleRepair,
    sGetBottleRepair,
    sGetSpareSeedA,
    sGetSpareSeedB,
    sGetSpareSeedC,
    sGetSpareQuiverA,
    sGetSpareQuiverB,
    sGetSpareQuiverC,
    sGetSpareBombBagA,
    sGetSpareBombBagB,
    sGetSpareBombBagC,
    sGetWhip,
    sGetEarring,
    nullptr,
    sGetNetB,
    sGetInsectGrasshopper,
    sGetInsectBeetle,
    sGetInsectBee,
    sGetInsectKamakiri,
    sGetInsectLadybug,
    sGetInsectButterfly,
    sGetInsectAnt,
    sGetInsectCicada,
    sGetInsectDragonfly,
    sGetInsectScarab,
    sGetInsectKuwagata,
    sGetInsectFirefly,
    sGetBottleEmpty,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    sGetKobunALetter,
    sGetTerryCage,
    nullptr,
    sGetSozaiA,
    sGetSozaiB,
    sGetSozaiC,
    sGetSozaiD,
    sGetSozaiE,
    sGetSozaiF,
    sGetSozaiG,
    sGetSozaiH,
    sGetSozaiI,
    sGetSozaiJ,
    sGetSozaiL, // not alphabetical
    sGetSozaiK, // not alphabetical
    sGetSozaiM,
    sGetSozaiN,
    sGetSozaiO,
    sGetSozaiP,
    sGetSekibanMapA,
    sGetSekibanMapB,
    sGetSekibanMapC,
    sGetSirenKey,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    sGetHarp,
    sGetHarp,
    sGetHarp,
    sGetHarp,
    sGetHarp,
    sGetHarp,
    sGetHarp,
    sGetHarp,
    sGetBottleRepairS,
    sGetBottlePumpkin,
    sGetBottlePumpkin,
    sGetSeedLife,
    sGetFruitB,
    sGetSparePurse,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

// TODO enum, item comments
static const u8 sItemToTBoxVariant[MAX_ITEM_ID + 1] = {
    0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static char *const sAnmNames[] = {
    "TBoxNormalT",
    "TBoxSmallT",
    "TBoxBossT",
    "TBoxNormalT",
};

static char *const sMdlNames[] = {
    "TBoxNormalT",
    "TBoxSmallT",
    "TBoxBossT",
    "GoddessTBox",
};

static char *const sCloseCcNames[] = {
    "TBoxNormalTClose",
    "TBoxSmallTClose",
    "TBoxBossTClose",
    "GoddessTBoxClose",
};

static char *const sOpenCcNames[] = {
    "TBoxNormalTOpen",
    "TBoxSmallTOpen",
    "TBoxBossTOpen",
    "GoddessTBoxOpen",
};

bool dAcTbox_c::tryGetDowsingTargetOffset(int variant, mVec3_c &out) {
    bool ok = dAcTbox_c::isValidVariant(variant);
    if (ok) {
        out.x = sDowsingTargetOffsets[variant].x;
        out.y = sDowsingTargetOffsets[variant].y;
        out.z = sDowsingTargetOffsets[variant].z;
    }
    return ok;
}

void dAcTbox_c::getDowsingTargetOffset(int variant, mVec3_c &out) {
    tryGetDowsingTargetOffset(variant, out);
}

f32 dAcTbox_c::getSomeRate() {
    return 1.0f / 8.333333f;
}

bool dAcTbox_c::isValidGroupIndex(int idx) {
    return 0 <= idx && idx < 15;
}
extern "C" void *SIREN_TAG;
extern "C" bool hasCollectedAllTears(void *SIREN_TAG);
bool dAcTbox_c::hasCollectedAllTears() {
    if (SIREN_TAG == nullptr) {
        return false;
    }
    return ::hasCollectedAllTears(SIREN_TAG);
}

bool dAcTbox_c::getGroundHeight(f32 *height, const mVec3_c &pos) {
    if (height == nullptr) {
        return false;
    }
    bool ok = dBgS_ObjGndChk::CheckPos(pos);
    if (ok) {
        *height = dBgS_ObjGndChk::GetGroundHeight();
    }
    return ok;
}

bool dAcTbox_c::isBelowGroundAtPos(f32 height, const mVec3_c &pos) {
    f32 gndHeight;
    if (!getGroundHeight(&gndHeight, pos)) {
        return false;
    }
    return height < gndHeight;
}

dAcTbox_c::dAcTbox_c()
    : mStateMgr(*this, sStateID::null),
      mScnCallback(this),
      mEvent(*this, nullptr),
      mTboxListNode(this),
      mDowsingTarget(this, DowsingTarget::SLOT_NONE),
      mGoddessDowsingTarget(this, DowsingTarget::SLOT_NONE) {
    field_0x120B = 0;
    field_0x120E = 0;
    mDoObstructedCheck = false;
    sTboxActorList.append(&mTboxListNode);
    unkByteTargetFiRelated = 2;
}

dAcTbox_c::~dAcTbox_c() {
    sTboxActorList.remove(&mTboxListNode);
}

bool dAcTbox_c::createHeap() {
    void *data = getOarcResFile(sArcNames[mVariant]);
    if (data == nullptr) {
        return false;
    }
    if (!mMdl1.create(data, sMdlNames[mVariant], sAnmNames[mVariant], &heap_allocator, 0x32F, 1, nullptr)) {
        return false;
    }
    mMdl1.getModel().setLocalMtx(mWorldMtx);
    mScnCallback.attach(mMdl1.getModel());
    if (!initBgW(mBgWs[0], sArcNames[mVariant], sCloseCcNames[mVariant])) {
        return false;
    }
    if (!initBgW(mBgWs[1], sArcNames[mVariant], sOpenCcNames[mVariant])) {
        return false;
    }

    if (mVariant == GODDESS) {
        nw4r::g3d::ResFile res(data);
        if (!res.IsValid()) {
            return false;
        }
        nw4r::g3d::ResMdl mdl = mMdl1.getModel().getResMdl();
        if (!mdl.IsValid()) {
            return false;
        }
        nw4r::g3d::ResAnmTexPat anmTexPat = res.GetResAnmTexPat("GoddessTBox");
        if (!anmTexPat.IsValid()) {
            return false;
        }
        if (!mAnmGoddessPat.create(mdl, anmTexPat, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mMdl1.getModel().setAnm(mAnmGoddessPat);
        u16 goddessTBoxActive = getParams2Lower();
        if (StoryflagManager::sInstance->getCounterOrFlag(goddessTBoxActive) && !mHasBeenOpened) {
            nw4r::g3d::ResAnmTexSrt anmTexSrt = res.GetResAnmTexSrt("GoddessTBox");
            if (!anmTexSrt.IsValid()) {
                return false;
            }
            if (!mAnmGoddessTexSrt.create(mdl, anmTexSrt, &heap_allocator, nullptr, 1)) {
                return false;
            }
            mMdl1.getModel().setAnm(mAnmGoddessTexSrt);
        }
    } else if (mVariant == NORMAL) {
        nw4r::g3d::ResFile res(data);
        if (!res.IsValid()) {
            return false;
        }
        nw4r::g3d::ResAnmClr anmClr = res.GetResAnmClr("TBoxNormalTAppear");
        if (!anmClr.IsValid()) {
            return false;
        }
        nw4r::g3d::ResMdl mdl = mMdl1.getModel().getResMdl();
        if (!mdl.IsValid()) {
            return false;
        }
        if (!mAnmMatClr1.create(mdl, anmClr, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mAnmMatClr1.setRate(1.0f, 0);
        mMdl1.getModel().setAnm(mAnmMatClr1);
    }

    if (isNotSmall()) {
        void *fxData = getOarcResFile(sOpenArcName);
        if (fxData == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile fxRes(fxData);
        if (!fxRes.IsValid()) {
            return false;
        }

        nw4r::g3d::ResMdl openMdl = fxRes.GetResMdl(sOpenMdlName);
        if (!openMdl.IsValid()) {
            return false;
        }
        if (!mOpenFxMdl.create(openMdl, &heap_allocator, 0x120, 1, nullptr)) {
            return false;
        }
        mOpenFxMdl.setPriorityDraw(0x7F, 0x86);

        nw4r::g3d::ResAnmChr openAnm = fxRes.GetResAnmChr(sOpenAnmChrName);
        if (!openAnm.IsValid()) {
            return false;
        }
        if (!mAnmChr.create(openMdl, openAnm, &heap_allocator, nullptr)) {
            return false;
        }
        mOpenFxMdl.setAnm(mAnmChr);

        nw4r::g3d::ResAnmTexSrt anmTexSrt = fxRes.GetResAnmTexSrt(sOpenAnmTexSrtName);
        if (!anmTexSrt.IsValid()) {
            return false;
        }
        if (!mAnmTexSrt1.create(openMdl, anmTexSrt, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mOpenFxMdl.setAnm(mAnmTexSrt1);

        nw4r::g3d::ResAnmClr anmClr = fxRes.GetResAnmClr(sOpenAnmClrName);
        if (!anmClr.IsValid()) {
            return false;
        }
        if (!mAnmMatClr2.create(openMdl, anmClr, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mOpenFxMdl.setAnm(mAnmMatClr2);
        mVec3_c fxPos;
        fn_8026B380(fxPos);
        mMtx_c fxTransform;
        fxTransform.transS(fxPos);
        // TODO address calculations here, otherwise this function
        // matches surprisingly well
        fxTransform.ZXYrotM(rotation.x, rotation.y, rotation.z);
        mOpenFxMdl.setLocalMtx(fxTransform);
        mOpenFxMdl.setScale(fn_8026B3C0());
    }

    return true;
}

static u32 sSomeCounters[15] = {0};

int dAcTbox_c::create() {
    if (!isActualVisibleBox()) {
        return FAILED;
    }
    int roomId_tmp = roomid;
    if (addActorToRoom(-1)) {
        roomid = roomId_tmp;
        changeLoadedEntitiesWithSet();
    }
    setItemId((ITEM_ID)(rotation.z & 0x1FF));
    if (mItemId > MAX_ITEM_ID) {
        return FAILED;
    }
    mVariant = (TboxVariant_e)sItemToTBoxVariant[mItemId];
    if (mVariant == GODDESS) {
        setItemId((ITEM_ID)(MAX_ITEM_ID - mItemId));
    }
    mSpawnSceneFlag = (params >> 0x14) & 0xFF;
    mSetSceneFlag = rotation.x & 0xFF;
    setChestFlag();
    field_0x120F = ((rotation.x >> 8) & 1) == 0;
    if (!noObstructionCheck()) {
        setDoObstructionCheck();
    }
    field_0x1208 = (rotation.x >> 0xA) & 0xF;
    switch (fn_8026B370()) {
        case 0:  field_0x120A = 0; break;
        case 1:  field_0x120A = 1; break;
        case 3:  field_0x120A = 3; break;
        default: field_0x120A = 3; break;
    }
    rotation.z = 0;
    rotation.x = 0;

    // This part of the code checks if there's another chest with similar properties
    // and only keeps one of them.
    if (isValidGroupIndex(field_0x1208) && !field_0x1210) {
        sSomeCounters[field_0x1208]++;
        field_0x1210 = true;
    }
    field_0x120D = 0;
    if (fn_8026D670()) {
        u32 counterValue = 0;
        getSomeCounter(&counterValue);
        if (counterValue > 1) {
            dAcTbox_c *other;
            bool keepGoing = true;
            dAcObjBase_c *cursor = nullptr;
            do {
                other = static_cast<dAcTbox_c *>(getNextObject(&sTboxActorList, cursor));
                cursor = other;
                if (other != nullptr && this != other && !other->field_0x120D && field_0x1208 == other->field_0x1208) {
                    keepGoing = false;
                }
            } while (keepGoing && cursor != nullptr);

            bool b;
            if (checkTboxFlag()) {
                b = true;
            } else if (other->checkTboxFlag()) {
                b = false;
            } else {
                b = fn_8026D560();
            }

            if (b) {
                other->deleteRequest();
                other->field_0x120D = true;
            } else {
                field_0x120D = true;
                return FAILED;
            }
        }
    }

    if (checkTboxFlag() ||
        (mSetSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mSetSceneFlag))) {
        mHasBeenOpened = true;
    } else {
        mHasBeenOpened = false;
        if (isItemRupee()) {
            initDowsingTarget(DowsingTarget::SLOT_RUPEE);
        } else if (dAcItem_c::isKeyPiece((ITEM_ID)mItemId)) {
            initDowsingTarget(DowsingTarget::SLOT_QUEST);
        } else if (dAcItem_c::isTreasure((ITEM_ID)mItemId)) {
            initDowsingTarget(DowsingTarget::SLOT_TREASURE);
        }

        if (mVariant == GODDESS && StoryflagManager::sInstance->getCounterOrFlag(getParams2Lower())) {
            initDowsingTargetCube();
        }
    }

    if (isItemRupee() || dAcItem_c::isTreasure((ITEM_ID)mItemId)) {
        mRegisterDowsingTarget = &dAcTbox_c::registerRupeeOrTreasureDowsing;
        mUnregisterDowsingTarget = &dAcTbox_c::unregisterDowsing;
    } else if (dAcItem_c::isKeyPiece((ITEM_ID)mItemId)) {
        mRegisterDowsingTarget = &dAcTbox_c::registerKeyPieceDowsing;
        mUnregisterDowsingTarget = &dAcTbox_c::noUnregisterDowsing;
    } else {
        mRegisterDowsingTarget = &dAcTbox_c::noRegisterDowsing;
        mUnregisterDowsingTarget = &dAcTbox_c::noUnregisterDowsing;
    }

    updateMatrix();

    CREATE_ALLOCATOR(dAcTbox_c);

    mStts.SetRank(0xD);
    static const dCcD_SrcUnk s1 = {
        sColSrc,
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    };
    mCcD3.Set(s1);
    mCcD3.SetStts(mStts);
    static const dCcD_SrcCyl s2 = {
        sColSrc,
        {1.0f, 1.0f},
    };
    mCcD4.Set(s2);
    mCcD4.SetStts(mStts);
    if (mVariant == SMALL) {
        // clang-format off
        static const dCcD_SrcUnk s3 = {{
            /* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
            /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x1010111, {7, 0x407}, 0, 0},
            /* mObjCo */ {0x0}},
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
        };
        // clang-format on
        field_0x0D48.addCc(mCcD1, s3);
        field_0x0D48.addCc(mCcD2, s3);
    } else {
        // clang-format off
        static const dCcD_SrcUnk s4 = {{
            /* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
            /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x1010111, {6, 0x407}, 0, 0},
            /* mObjCo */ {0x0}},
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
        };
        // clang-format on
        field_0x0D48.addCc(mCcD1, s4);
        field_0x0D48.addCc(mCcD2, s4);
    }
    field_0x0D48.SetStts(mStts);
    // TODO figure out the right fields
    mCcD1.SetTg_0x4C(-1);
    mCcD2.SetTg_0x4C(-1);
    mMdl1.setAnm(sAnmNames[mVariant], m3d::PLAY_MODE_4);
    if (mHasBeenOpened == true) {
        mMdl1.setFrame(mMdl1.getAnm().getEndFrame());
    } else {
        mMdl1.setFrame(mMdl1.getAnm().getStartFrame());
    }
    mMdl1.getModel().calc(false);
    mMtx_c nodeMtx;
    mMdl1.getModel().getNodeWorldMtx(mMdl1.getModel().getNodeID("Cover"), nodeMtx);

    nodeMtx.getTranslation(field_0x11D8);
    if (mVariant == GODDESS) {
        if (!mHasBeenOpened && StoryflagManager::sInstance->getCounterOrFlag(getParams2Lower())) {
            mAnmGoddessPat.setFrame(1.0f, 0);
        } else {
            mAnmGoddessPat.setFrame(0.0f, 0);
        }
    }

    field_0x11F0 = 0;
    switch (mVariant) {
        case NORMAL: {
            static mVec3_c bbLo = mVec3_c(-65.0f, 0.0f, -100.0f);
            static mVec3_c bbUp = mVec3_c(65.0f, 150.0f, 45.0f);
            boundingBox.Set(bbLo, bbUp);
        } break;
        case SMALL: {
            static mVec3_c bbLo = mVec3_c(-38.0f, 0.0f, -70.0f);
            static mVec3_c bbUp = mVec3_c(38.0f, 110.0f, 35.0f);
            boundingBox.Set(bbLo, bbUp);
        } break;
        case BOSS: {
            static mVec3_c bbLo = mVec3_c(-90.0f, 0.0f, -140.0f);
            static mVec3_c bbUp = mVec3_c(90.0f, 170.0f, 60.0f);
            boundingBox.Set(bbLo, bbUp);
        } break;
        case GODDESS: {
            static mVec3_c bbLo = mVec3_c(-65.0f, 0.0f, -100.0f);
            static mVec3_c bbUp = mVec3_c(65.0f, 150.0f, 50.0f);
            boundingBox.Set(bbLo, bbUp);
        } break;
    }

    field_0x11EC = 1.0f;
    field_0x11FC = 0;

    field_0x4E8.r = 0;
    field_0x4E8.g = 0;
    field_0x4E8.b = 0;

    field_0x4EC = 0.0f;
    field_0x4DC.x = position.x;
    field_0x4DC.z = position.z;
    field_0x4DC.y = position.y + 100.0f;

    return SUCCEEDED;
}

int dAcTbox_c::doDelete() {
    if (field_0x1210 != 0) {
        sSomeCounters[field_0x1208]--;
        field_0x1210 = 0;
    }
    if (sCurrentObtainingItemOarcName != nullptr) {
        OarcManager *mng = OarcManager::GetInstance();
        if (!mng->ensureLoaded1(sCurrentObtainingItemOarcName)) {
            mng->decrement(sCurrentObtainingItemOarcName);
            sCurrentObtainingItemOarcName = nullptr;
        }
    }

    if (sCurrentObtainingItemOarcName == nullptr) {
        changeLoadedEntitiesNoSet();
        return SUCCEEDED;
    }
    return NOT_READY;
}

int dAcTbox_c::actorExecute() {
    if (field_0x120D != 0) {
        deleteRequest();
        return SUCCEEDED;
    }

    setActionState();
    if (mStateMgr.isState(StateID_WaitOpen) || mStateMgr.isState(StateID_GoddessWaitOn)) {
        (this->*mRegisterDowsingTarget)();
    }

    dRoom_c *r = dStage_c::GetInstance()->getRoom(roomid);
    bool hasFlags = r->checkFlag(0x1E);
    if (hasFlags) {
        setObjectProperty(0x200);
    } else {
        clearObjectProperty(0x200);
    }

    if (hasFlags) {
        return SUCCEEDED;
    }

    checkForLinkBonk();
    fn_8026E630();
    mStateMgr.executeState();
    if (fn_8026D540()) {
        fn_8026D3C0();
    }
    if (mVariant == GODDESS && mAnmGoddessPat.getFrame(0) == 1.0f) {
        mAnmGoddessTexSrt.play();
    }
    syncScaleToMdl(&mMdl1.getModel());
    mMdl1.getModel().calc(false);
    if ((field_0x11F4 & 2) != 0) {
        mVec3_c v1, v2;
        getCCBounds(&v1, &v2);
        v1 *= field_0x11E8;
        v2 *= field_0x11E8;
        mCcD3.Set(v1, v2);
        mCcD3.Set(position, rotation.y);
        mVec3_c cylC;
        f32 width, height;
        getCylParams(&cylC, &width, &height);
        mCcD4.SetC(cylC);
        mCcD4.SetR(width);
        mCcD4.SetH(height);
        dCcS::GetInstance()->Set(&mCcD4);

        switch ((u32)mVariant) {
            case NORMAL:
                switch (field_0x120C) {
                    case 0:
                        v1.set(-61.0f, 0.0f, -42.0f);
                        v2.set(61.0f, 100.0f, 42.0f);
                        break;
                    case 1:
                        v1.set(-61.0f, 0.0f, -42.0f);
                        v2.set(61.0f, 62.0f, 42.0f);
                        break;
                }
                break;
            case SMALL:
                switch (field_0x120C) {
                    case 0:
                        v1.set(-38.0f, 0.0f, -35.0f);
                        v2.set(38.0f, 45.0f, 31.0f);
                        break;
                    case 1:
                        v1.set(-38.0f, 0.0f, -35.0f);
                        v2.set(38.0f, 45.0f, 31.0f);
                        break;
                }
                break;
            case BOSS:
                switch (field_0x120C) {
                    case 0:
                        v1.set(-90.0f, 0.0f, -53.0f);
                        v2.set(90.0f, 133.0f, 56.0f);
                        break;
                    case 1:
                        v1.set(-80.0f, 0.0f, -53.0f);
                        v2.set(80.0f, 62.0f, 60.0f);
                        break;
                }
                break;
            case GODDESS:
                switch (field_0x120C) {
                    case 0:
                        v1.set(-62.0f, 0.0f, -47.0f);
                        v2.set(62.0f, 100.0f, 47.0f);
                        break;
                    case 1:
                        v1.set(-62.0f, 0.0f, -47.0f);
                        v2.set(62.0f, 62.0f, 55.0f);
                        break;
                }
                break;
        }
        v1 *= field_0x11E8;
        v2 *= field_0x11E8;
        mCcD1.Set(v1, v2);
        mCcD1.Set(position, rotation.y);
        if (field_0x120C == 1) {
            mCcD2.OnTgSet();
            switch ((u32)mVariant) {
                case NORMAL:
                    v1.set(-61.0f, 0.0f, -50.0f);
                    v2.set(61.0f, 80.0f, 0.0);
                    break;
                case SMALL:
                    v1.set(-38.0f, -5.0f, -35.0f);
                    v2.set(38.0f, 65.0f, 0.0f);
                    break;
                case BOSS:
                    v1.set(-90.0f, -10.0f, -85.0f);
                    v2.set(90.0f, 105.0f, 0.0f);
                    break;
                case GODDESS:
                    v1.set(-62.0f, 0.0f, -57.0f);
                    v2.set(62.0f, 80.0f, 0.0f);
                    break;
            }
            v1 *= field_0x11E8;
            v2 *= field_0x11E8;
            mCcD2.Set(v1, v2);
            mCcD2.Set(field_0x11D8, rotation.y);
        } else {
            mCcD2.ClrTgSet();
        }
        field_0x0D48.registerColliders();
    }

    return SUCCEEDED;
}

int dAcTbox_c::actorExecuteInEvent() {
    if (field_0x120D != 0) {
        deleteRequest();
        return SUCCEEDED;
    }

    setActionState();
    if (mStateMgr.isState(StateID_WaitOpen) || mStateMgr.isState(StateID_GoddessWaitOn)) {
        (this->*mRegisterDowsingTarget)();
    }

    dRoom_c *r = dStage_c::GetInstance()->getRoom(roomid);
    bool hasFlags = r->checkFlag(0x1E);
    if (hasFlags) {
        setObjectProperty(0x200);
    } else {
        clearObjectProperty(0x200);
    }

    if (hasFlags) {
        return SUCCEEDED;
    }

    checkForLinkBonk();
    fn_8026E630();

    switch (mEvent.getCurrentEventCommand()) {
        case 'wait': mEvent.advanceNext(); break;
        case 'open':
            if (mEvent.isAdvance()) {
                if (mItemId == 0) {
                    mItemModelIdx = 0x10;
                    mStateMgr.changeState(StateID_DeleteArchive);
                } else {
                    mStateMgr.changeState(StateID_Open);
                }
            }
            mStateMgr.executeState();
            break;
        case 'aper':
            if (mEvent.isAdvance()) {
                mStateMgr.changeState(StateID_DemoAppear);
            }
            mStateMgr.executeState();
            break;
        case 'crct': {
            mVec3_c v1;
            mAng a1;
            fn_8026DAD0(&sVec4, &v1);
            fn_8026DAC0(a1);
            dAcPy_c *link = dAcPy_c::LINK;
            if (field_0x120B < 3) {
                mVec3_c pos = link->position;
                f32 f9 = cLib::addCalcPosXZ(&pos, v1, 0.25, 200.0f, 0.1f);
                s16 rot2 = link->rotation.y;
                s16 d = sLib::addCalcAngle(&rot2, a1, 2, 0x3FFF, 1);
                if (f9 < 1.0f && labs(d) < 0xB6) {
                    link->setPosYRot(v1, a1, 0, 0, 0);
                    mEvent.advanceNext();
                } else {
                    link->setPosYRot(pos, rot2, 0, 0, 0);
                }
            } else {
                link->setPosYRot(v1, a1, 0, 0, 0);
                mEvent.advanceNext();
            }

            if (field_0x120B < 0xFF) {
                field_0x120B++;
            }
        } break;
        case '????': mStateMgr.executeState(); break;
        default:     mEvent.advanceNext(); break;
    }

    // copied from actorExecute
    if (fn_8026D540()) {
        fn_8026D3C0();
    }
    if (mVariant == GODDESS && mAnmGoddessPat.getFrame(0) == 1.0f) {
        mAnmGoddessTexSrt.play();
    }
    syncScaleToMdl(&mMdl1.getModel());
    mMdl1.getModel().calc(false);
    if ((field_0x11F4 & 2) != 0) {
        mVec3_c v1, v2;
        getCCBounds(&v1, &v2);
        v1 *= field_0x11E8;
        v2 *= field_0x11E8;
        mCcD3.Set(v1, v2);
        mCcD3.Set(position, rotation.y);
        mVec3_c cylC;
        f32 width, height;
        getCylParams(&cylC, &width, &height);
        mCcD4.SetC(cylC);
        mCcD4.SetR(width);
        mCcD4.SetH(height);
        dCcS::GetInstance()->Set(&mCcD4);

        switch ((u32)mVariant) {
            case NORMAL:
                switch (field_0x120C) {
                    case 0:
                        v1.set(-61.0f, 0.0f, -42.0f);
                        v2.set(61.0f, 100.0f, 42.0f);
                        break;
                    case 1:
                        v1.set(-61.0f, 0.0f, -42.0f);
                        v2.set(61.0f, 62.0f, 42.0f);
                        break;
                }
                break;
            case SMALL:
                switch (field_0x120C) {
                    case 0:
                        v1.set(-38.0f, 0.0f, -35.0f);
                        v2.set(38.0f, 45.0f, 31.0f);
                        break;
                    case 1:
                        v1.set(-38.0f, 0.0f, -35.0f);
                        v2.set(38.0f, 45.0f, 31.0f);
                        break;
                }
                break;
            case BOSS:
                switch (field_0x120C) {
                    case 0:
                        v1.set(-90.0f, 0.0f, -53.0f);
                        v2.set(90.0f, 133.0f, 56.0f);
                        break;
                    case 1:
                        v1.set(-80.0f, 0.0f, -53.0f);
                        v2.set(80.0f, 62.0f, 60.0f);
                        break;
                }
                break;
            case GODDESS:
                switch (field_0x120C) {
                    case 0:
                        v1.set(-62.0f, 0.0f, -47.0f);
                        v2.set(62.0f, 100.0f, 47.0f);
                        break;
                    case 1:
                        v1.set(-62.0f, 0.0f, -47.0f);
                        v2.set(62.0f, 62.0f, 55.0f);
                        break;
                }
                break;
        }
        v1 *= field_0x11E8;
        v2 *= field_0x11E8;
        mCcD1.Set(v1, v2);
        mCcD1.Set(position, rotation.y);
        if (field_0x120C == 1) {
            mCcD2.OnTgSet();
            switch ((u32)mVariant) {
                case NORMAL:
                    v1.set(-61.0f, 0.0f, -50.0f);
                    v2.set(61.0f, 80.0f, 0.0);
                    break;
                case SMALL:
                    v1.set(-38.0f, -5.0f, -35.0f);
                    v2.set(38.0f, 65.0f, 0.0f);
                    break;
                case BOSS:
                    v1.set(-90.0f, -10.0f, -85.0f);
                    v2.set(90.0f, 105.0f, 0.0f);
                    break;
                case GODDESS:
                    v1.set(-62.0f, 0.0f, -57.0f);
                    v2.set(62.0f, 80.0f, 0.0f);
                    break;
            }
            v1 *= field_0x11E8;
            v2 *= field_0x11E8;
            mCcD2.Set(v1, v2);
            mCcD2.Set(field_0x11D8, rotation.y);
        } else {
            mCcD2.ClrTgSet();
        }
        field_0x0D48.registerColliders();
    }

    return SUCCEEDED;
}

int dAcTbox_c::draw() {
    bool isInvisible = fabsf(mScale.x) <= FLT_EPSILON;
    if (isInvisible) {
        return SUCCEEDED;
    }

    drawModelType1(&mMdl1.getModel());
    if (fn_8026D540()) {
        drawModelType1(&mOpenFxMdl);
    }

    static mQuat_c shadowDirs[4] = {
        mQuat_c(0.0f, 45.0f, 0.0f, 100.0f),
        mQuat_c(0.0f, 25.0f, 0.0f, 80.0f),
        mQuat_c(0.0f, 55.0f, 0.0f, 120.0f),
        mQuat_c(0.0f, 45.0f, 0.0f, 100.0f),
    };

    drawShadow(mShadowCircle, nullptr, mWorldMtx, &shadowDirs[mVariant], -1, -1, -1, -1, -1, 0.0f);

    return SUCCEEDED;
}

void dAcTbox_c::setActionState() {
    if (!field_0x120E) {
        if (field_0x120F) {
            mStateMgr.changeState(StateID_DugOut);
        } else if (mHasBeenOpened == 1) {
            if (mVariant == GODDESS) {
                mStateMgr.changeState(StateID_GoddessWait);
            } else {
                mStateMgr.changeState(StateID_Wait);
            }
        } else if (mVariant == GODDESS) {
            if (StoryflagManager::sInstance->getCounterOrFlag(getParams2Lower())) {
                mStateMgr.changeState(StateID_GoddessWaitOn);
            } else {
                mStateMgr.changeState(StateID_GoddessWaitOff);
            }
        } else if (mVariant == NORMAL) {
            switch (field_0x120A) {
                case 0:
                    if (hasCollectedAllTears()) {
                        mStateMgr.changeState(StateID_WaitOpen);
                    } else {
                        mStateMgr.changeState(StateID_WaitAppear);
                    }
                    break;
                case 1: mStateMgr.changeState(StateID_WaitOpen); break;
                case 3:
                    if (mSpawnSceneFlag >= 0xFF ||
                        SceneflagManager::sInstance->checkBoolFlag(roomid, mSpawnSceneFlag)) {
                        mStateMgr.changeState(StateID_WaitOpen);
                    } else {
                        mStateMgr.changeState(StateID_WaitAppear);
                    }
                    break;
            }
        } else {
            mStateMgr.changeState(StateID_WaitOpen);
        }
        field_0x120E = true;
    }
}

bool dAcTbox_c::isValidVariant(int variant) {
    return 0 <= variant && variant <= 3;
}

void *dAcTbox_c::getObjectListEntry() {
    return &mTboxListNode;
}

void dAcTbox_c::setFlags(u32 flags) {
    field_0x11F4 = flags;
}

void dAcTbox_c::setItemId(u16 itemId) {
    mItemId = itemId;
}

void dAcTbox_c::setDoObstructionCheck() {
    mDoObstructedCheck = true;
}

int dAcTbox_c::isActualVisibleBox() const {
    return (params >> 0x1C);
}

bool dAcTbox_c::noObstructionCheck() const {
    return (rotation.x >> 9) & 1;
}

int dAcTbox_c::fn_8026B370() const {
    return (rotation.x >> 0xE) & 0x3;
}

void dAcTbox_c::fn_8026B380(mVec3_c &out) const {
    if (mVariant == BOSS) {
        out.x = position.x;
        out.y = position.y;
        out.z = position.z;
    } else {
        fn_8026DAD0(&sVec1, &out);
    }
}

const mVec3_c &dAcTbox_c::fn_8026B3C0() const {
    if (mVariant == BOSS) {
        return sVec3;
    }
    return sVec2;
}

void dAcTbox_c::initializeState_DugOut() {
    mScale.set(0.0f, 0.0f, 1.0f);
    field_0x11E8 = 0.0f;
    field_0x11F4 &= ~2;
    if (mHasBeenOpened == 1) {
        field_0x120C = 1;
    } else {
        field_0x120C = 0;
    }
    setActorProperty(0x100);
    if (mVariant == NORMAL) {
        mMdl1.setAnm(sAnmNames[0], m3d::PLAY_MODE_4);
    }
    if (mVariant == NORMAL) {
        if (checkTboxFlag()) {
            mMdl1.setFrame(mMdl1.getAnm().getEndFrame());
        } else {
            mMdl1.setFrame(mMdl1.getAnm().getStartFrame());
        }
    }
    if (mVariant == NORMAL) {
        mAnmMatClr1.setFrame(mAnmMatClr1.getFrameMax(0), 0);
    }
    fn_8026D140();
}
void dAcTbox_c::executeState_DugOut() {
    field_0x11F4 |= 2;
    if (field_0x11E8 < 1.0f) {
        bool result = sLib::chase(&field_0x11E8, 1.0f, 0.2f);
        switch ((u32)mVariant) {
            case NORMAL:
                if (mHasBeenOpened == 1) {
                    field_0x11C0.set(-61.0f, 0.0f, -90.0f);
                    field_0x11CC.set(61.0f, 142.0f, 42.0f);
                } else {
                    field_0x11C0.set(-61.0f, 0.0f, -42.0f);
                    field_0x11CC.set(61.0f, 100.0f, 42.0f);
                }
                break;
            case SMALL:
                if (mHasBeenOpened == 1) {
                    field_0x11C0.set(-38.0f, 0.0f, -68.0f);
                    field_0x11CC.set(38.0f, 101.0f, 31.0f);
                } else {
                    field_0x11C0.set(-38.0f, 0.0f, -35.0f);
                    field_0x11CC.set(38.0f, 68.0f, 35.0f);
                }
                break;
            case BOSS:
                if (mHasBeenOpened == 1) {
                    field_0x11C0.set(-90.0f, 0.0f, -135.0f);
                    field_0x11CC.set(90.0f, 163.0f, 56.0f);
                } else {
                    field_0x11C0.set(-90.0f, 0.0f, -53.0f);
                    field_0x11CC.set(90.0f, 133.0f, 56.0f);
                }
                break;
            case GODDESS:
                if (mHasBeenOpened == 1) {
                    field_0x11C0.set(-62.0f, 0.0f, -95.0f);
                    field_0x11CC.set(62.0f, 140.0f, 47.0f);
                } else {
                    field_0x11C0.set(-62.0f, 0.0f, -47.0f);
                    field_0x11CC.set(62.0f, 100.0f, 47.0f);
                }
                break;
        }
        field_0x11C0 *= field_0x11E8;
        field_0x11CC *= field_0x11E8;
        if (!result) {
            return;
        }
    }

    bool bX = sLib::chase(&mScale.x, 1.0f, 0.2f);
    bool bY = sLib::chase(&mScale.y, 1.0f, 0.2f);
    if (bX && bY) {
        if (mVariant == GODDESS) {
            if (mHasBeenOpened == true) {
                mStateMgr.changeState(StateID_GoddessWait);
            } else {
                if (StoryflagManager::sInstance->getCounterOrFlag(getParams2Lower())) {
                    mStateMgr.changeState(StateID_GoddessWaitOn);
                } else {
                    mStateMgr.changeState(StateID_GoddessWaitOff);
                }
            }
        } else if (mHasBeenOpened == true) {
            mStateMgr.changeState(StateID_Wait);
        } else {
            mStateMgr.changeState(StateID_WaitOpen);
        }
    }
}
void dAcTbox_c::finalizeState_DugOut() {}

void dAcTbox_c::initializeState_WaitAppear() {
    mScale.set(0.0f, 0.0f, 0.0f);
    setActorProperty(0x100);
    fn_8026D140();
    field_0x11E8 = 0.0f;
    field_0x11F4 &= ~0x2;
    field_0x120C = 0;
    field_0x11C0.set(0.0f, 0.0f, 0.0f);
    field_0x11CC.set(0.0f, 0.0f, 0.0f);
}
void dAcTbox_c::executeState_WaitAppear() {
    // @bug doIt is uninitialized here, so WaitAppear is UB unless field_0x120A == 0 or field_0x120A == 3
    bool doIt;
    switch (field_0x120A) {
        case 0: doIt = hasCollectedAllTears(); break;
        case 3: doIt = SceneflagManager::sInstance->checkBoolFlag(roomid, mSpawnSceneFlag); break;
    }

    if (doIt) {
        Event e("TboxShow", 100, 0x100001, nullptr, nullptr);
        mEvent.scheduleEvent(e, 0);
    }
}
void dAcTbox_c::finalizeState_WaitAppear() {}

void dAcTbox_c::initializeState_DemoAppear() {
    field_0x11F8 = 0;
    mScale.set(0.0f, 0.0f, 0.0f);
    field_0x11E8 = 0.0f;
    setActorProperty(0x100);
    mMdl1.setAnm(sAppearAnmName, m3d::PLAY_MODE_4);
    mMdl1.setFrame(mMdl1.getAnm().getStartFrame());
    mAnmMatClr1.setFrame(0.0f, 0);
    fn_8026D140();
    field_0x11E8 = 0.0f;
    field_0x11F4 &= ~0x2;
    field_0x120C = 0;
    field_0x11C0.set(0.0f, 0.0f, 0.0f);
    field_0x11CC.set(0.0f, 0.0f, 0.0f);
}
extern "C" void SmallSoundManager__playSound(void *, u32);
extern "C" void *SOUND_EFFECT_SOUND_MGR;
void dAcTbox_c::executeState_DemoAppear() {
    int val = field_0x11F8++;
    if (field_0x11F8 < 11) {
        field_0x11C0.set(-61.0f, 0.0f, -42.0f);
        field_0x11CC.set(61.0f, 100.0f, 42.0f);
        f32 f = field_0x11F8 * 0.1f;
        field_0x11C0 *= f;
        field_0x11CC *= f;
    }
    if (field_0x11F8 >= 20) {
        bool b = val < 20;
        if (b) {
            spawnAppearEffect();
        }
        if (b) {
            mScale.set(1.0f, 1.0f, 1.0f);
        }
        bool wasStop = mMdl1.getAnm().isStop();
        if (!wasStop) {
            mMdl1.play();
        }
        bool isStop = mMdl1.getAnm().isStop();
        if (!wasStop) {
            field_0x11E8 = mMdl1.getAnm().getFrame() / (mMdl1.getAnm().getEndFrame() - mMdl1.getAnm().getStartFrame());
        }
        if (0.0f < field_0x11E8) {
            field_0x11F4 |= 2;
        }
        mAnmMatClr1.play();
        if (isStop && mAnmMatClr1.isStop(0) && field_0x11F8 > 0x5A) {
            mEvent.advanceNext();
            mStateMgr.changeState(StateID_WaitOpen);
            SmallSoundManager__playSound(SOUND_EFFECT_SOUND_MGR, 0x13AD);
        }
    }
}
void dAcTbox_c::finalizeState_DemoAppear() {}

void dAcTbox_c::initializeState_WaitOpen() {
    mScale.set(1.0f, 1.0f, 1.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    switch ((u32)mVariant) {
        case NORMAL:
            field_0x11C0.set(-61.0f, 0.0f, -42.0f);
            field_0x11CC.set(61.0f, 100.0f, 42.0f);
            break;
        case SMALL:
            field_0x11C0.set(-38.0f, 0.0f, -35.0f);
            field_0x11CC.set(38.0f, 68.0f, 35.0f);
            break;
        case BOSS:
            field_0x11C0.set(-90.0f, 0.0f, -53.0f);
            field_0x11CC.set(90.0f, 133.0f, 56.0f);
            break;
            // WaitOpen not used for Goddess Chests
    }
    field_0x120C = 0;
    clearActorProperty(0x100);
    if (mVariant == NORMAL) {
        mMdl1.setAnm(sAnmNames[0], m3d::PLAY_MODE_4);
    }
    if (mVariant == NORMAL) {
        mMdl1.setFrame(mMdl1.getAnm().getStartFrame());
    }
    if (mVariant == NORMAL) {
        mAnmMatClr1.setFrame(mAnmMatClr1.getFrameMax(0), 0);
    }
    fn_8026D130();
}
void dAcTbox_c::executeState_WaitOpen() {
    if ((!mDoObstructedCheck || checkIsClear()) && fn_8026D120()) {
        AttentionManager *mgr = AttentionManager::GetInstance();
        mgr->addTarget(*this, getInteractionTargetDef(), 0, nullptr);
    }
}
void dAcTbox_c::finalizeState_WaitOpen() {
    (this->*mUnregisterDowsingTarget)();
}

void dAcTbox_c::initializeState_GoddessWaitOff() {
    clearActorProperty(0x100);
    fn_8026D130();
    field_0x11C0.set(-62.0f, 0.0f, -47.0f);
    field_0x11CC.set(62.0f, 100.0f, 47.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    field_0x120C = 0;
}
void dAcTbox_c::executeState_GoddessWaitOff() {}
void dAcTbox_c::finalizeState_GoddessWaitOff() {}

void dAcTbox_c::initializeState_GoddessWaitOn() {
    clearActorProperty(0x100);
    fn_8026D130();
    field_0x11C0.set(-62.0f, 0.0f, -47.0f);
    field_0x11CC.set(62.0f, 100.0f, 47.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    mGoddessDowsingTarget.doRegister();
}
void dAcTbox_c::executeState_GoddessWaitOn() {
    if (fn_8026D120()) {
        AttentionManager *mgr = AttentionManager::GetInstance();
        mgr->addTarget(*this, getInteractionTargetDef(), 0, nullptr);
    }
}
void dAcTbox_c::finalizeState_GoddessWaitOn() {
    (this->*mUnregisterDowsingTarget)();
    mGoddessDowsingTarget.doUnregister();
}

void dAcTbox_c::initializeState_DeleteArchive() {
    mScale.set(1.0f, 1.0f, 1.0f);
    clearActorProperty(0x100);
    if (mVariant == NORMAL) {
        mMdl1.setAnm(sAnmNames[0], m3d::PLAY_MODE_4);
    }
    if (mVariant == NORMAL) {
        mMdl1.setFrame(mMdl1.getAnm().getStartFrame());
    }
    if (mVariant == NORMAL) {
        mAnmMatClr1.setFrame(mAnmMatClr1.getFrameMax(0), 0);
    }
    fn_8026D140();
    field_0x11C0.set(-61.0f, 0.0f, -42.0f);
    field_0x11CC.set(61.0f, 100.0f, 42.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    field_0x120C = 0;
}
void dAcTbox_c::executeState_DeleteArchive() {
    if (sCurrentObtainingItemOarcName != nullptr) {
        OarcManager *mng = OarcManager::GetInstance();
        if (!mng->ensureLoaded1(sCurrentObtainingItemOarcName)) {
            mng->decrement(sCurrentObtainingItemOarcName);
            sCurrentObtainingItemOarcName = nullptr;
        }
    }

    if (sCurrentObtainingItemOarcName == nullptr) {
        if (sItemToArchiveName[mItemModelIdx] == nullptr) {
            mStateMgr.changeState(StateID_Open);
        } else {
            mStateMgr.changeState(StateID_LoadArchive);
        }
    }
}
void dAcTbox_c::finalizeState_DeleteArchive() {}

void dAcTbox_c::initializeState_LoadArchive() {
    mScale.set(1.0f, 1.0f, 1.0f);
    clearActorProperty(0x100);
    if (mVariant == NORMAL) {
        mMdl1.setAnm(sAnmNames[0], m3d::PLAY_MODE_4);
    }
    if (mVariant == NORMAL) {
        mMdl1.setFrame(mMdl1.getAnm().getStartFrame());
    }
    if (mVariant == NORMAL) {
        mAnmMatClr1.setFrame(mAnmMatClr1.getFrameMax(0), 0);
    }
    fn_8026D140();
    sCurrentObtainingItemOarcName = sItemToArchiveName[mItemModelIdx];
    OarcManager::GetInstance()->loadObjectArcFromDisk(sCurrentObtainingItemOarcName, nullptr);
    field_0x11C0.set(-61.0f, 0.0f, -42.0f);
    field_0x11CC.set(61.0f, 100.0f, 42.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    field_0x120C = 0;
}
void dAcTbox_c::executeState_LoadArchive() {
    if (!OarcManager::GetInstance()->ensureLoaded1(sCurrentObtainingItemOarcName)) {
        mStateMgr.changeState(StateID_Open);
    }
}
void dAcTbox_c::finalizeState_LoadArchive() {}

extern "C" void fn_800298B0(u16 effectIndex, mVec3_c *, mAng3_c *, mVec3_c *, void *, void *, void *, void *);
extern "C" u16 PARTICLE_RESOURCE_ID_MAPPING_209_;
extern "C" bool isPouchItem(u16);
extern "C" u8 adventurePouchFindItemSlot(ITEM_ID item);
extern "C" u16 findItemInItemCheck(ITEM_ID item);
extern "C" dAcItem_c *giveItem3(u16 item, s32);

void dAcTbox_c::initializeState_Open() {
    mScale.set(1.0f, 1.0f, 1.0f);
    playSound(0xA36);
    clearActorProperty(0x100);
    if (mVariant == NORMAL) {
        mAnmMatClr1.setFrame(mAnmMatClr1.getFrameMax(0), 0);
    }
    fn_8026D370();

    if (isNotSmall()) {
        mAnmMatClr2.setFrame(mAnmMatClr2.getFrameStart(0), 0);
        mAnmChr.setFrameOnly(mAnmChr.getStartFrame());
        mAnmTexSrt1.setFrame(mAnmTexSrt1.getFrameStart(0), 0);
    }

    if (isNotSmall()) {
        mVec3_c pos;
        fn_8026B380(pos);
        mVec3_c p2 = fn_8026B3C0();
        fn_800298B0(PARTICLE_RESOURCE_ID_MAPPING_209_, &pos, &rotation, &p2, nullptr, nullptr, nullptr, nullptr);
    }
    fn_8026D140();
    ITEM_ID itemId = mItemId != 0 ? (ITEM_ID)mItemId : ITEM_GODDESS_HARP;
    if (isPouchItem(itemId) && adventurePouchFindItemSlot(ITEM_NONE) == 8 && findItemInItemCheck(ITEM_NONE) == 0x3C) {
        setShouldCloseFlag();
    }
    dAcItem_c *item = giveItem3(itemId, -1);
    if (item != nullptr) {
        mItemRef.link(item);
        const char *evName;
        if (dAcItem_c::getItemGetEventName(itemId, &evName)) {
            Event ev(evName, 300, 4, (void *)dAcItem_c::itemGetEventStart, (void *)dAcItem_c::itemGetEventEnd);
            EventManager::changeOwnEvent(this, item, &ev, 0);
        }
    }
    field_0x11C0.set(-61.0f, 0.0f, -90.0f);
    field_0x11CC.set(61.0f, 142.0f, 42.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    field_0x120C = 1;
}
void dAcTbox_c::executeState_Open() {
    mMdl1.setFrame(dAcPy_c::LINK->getCurrentAnimFrame());
    if (mVariant == GODDESS) {
        if (0.4f < field_0x11EC) {
            sLib::chase(&field_0x11EC, 0.4f, getSomeRate());
            BlurAndPaletteManager::GetInstance().fn_80022AF0(field_0x11EC);
            BlurAndPaletteManager::GetInstance().setField_0x2F20(field_0x11EC);
        }
    } else if (mVariant != SMALL) {
        // exact same code as in the other branch
        if (0.4f < field_0x11EC) {
            sLib::chase(&field_0x11EC, 0.4f, getSomeRate());
            BlurAndPaletteManager::GetInstance().fn_80022AF0(field_0x11EC);
            BlurAndPaletteManager::GetInstance().setField_0x2F20(field_0x11EC);
        }
    }
    if (mMdl1.getAnm().isStop()) {
        mStateMgr.changeState(StateID_PresentItem);
    }
}
void dAcTbox_c::finalizeState_Open() {}

void dAcTbox_c::initializeState_PresentItem() {
    if (mVariant == NORMAL) {
        mAnmMatClr1.setFrame(mAnmMatClr1.getFrameMax(0), 0);
    }
    field_0x11C0.set(-61.0f, 0.0f, -90.0f);
    field_0x11CC.set(61.0f, 142.0f, 42.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    field_0x120C = 1;
}

void dAcTbox_c::executeState_PresentItem() {
    // Copied from executeState_Open
    if (mVariant == GODDESS) {
        if (0.4f < field_0x11EC) {
            sLib::chase(&field_0x11EC, 0.4f, getSomeRate());
            BlurAndPaletteManager::GetInstance().fn_80022AF0(field_0x11EC);
            BlurAndPaletteManager::GetInstance().setField_0x2F20(field_0x11EC);
        }
    } else if (mVariant != SMALL) {
        if (0.4f < field_0x11EC) {
            sLib::chase(&field_0x11EC, 0.4f, getSomeRate());
            BlurAndPaletteManager::GetInstance().fn_80022AF0(field_0x11EC);
            BlurAndPaletteManager::GetInstance().setField_0x2F20(field_0x11EC);
        }
    }
    mEvent.advanceNext();
    if (checkShouldClose()) {
        unsetShouldCloseFlag();
        mStateMgr.changeState(StateID_Close);
    } else {
        mHasBeenOpened = true;
        if (mSetSceneFlag < 0xFF) {
            SceneflagManager::sInstance->setFlag(roomid, mSetSceneFlag);
        }
        setTboxFlag();
        if (mVariant == GODDESS) {
            mStateMgr.changeState(StateID_GoddessWait);
            GoddessChestCounter::sInstance.checkedAdd(1);
        } else {
            mStateMgr.changeState(StateID_Wait);
        }
    }
}
void dAcTbox_c::finalizeState_PresentItem() {}

void dAcTbox_c::initializeState_Close() {
    mMdl1.getAnm().setPlayState(m3d::PLAY_MODE_3);
    if (mVariant == NORMAL) {
        mAnmMatClr1.setFrame(mAnmMatClr1.getFrameMax(0), 0);
    }
    field_0x11FC = 0x2D;
    field_0x11C0.set(-61.0f, 0.0f, -90.0f);
    field_0x11CC.set(61.0f, 142.0f, 42.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    field_0x120C = 1;
}
void dAcTbox_c::executeState_Close() {
    switch ((u32)mVariant) {
        case NORMAL:
        case BOSS:
            if (field_0x11FC <= 0) {
                if (field_0x11EC < 1.0f) {
                    sLib::chase(&field_0x11EC, 1.0f, getSomeRate());
                    BlurAndPaletteManager::GetInstance().fn_80022AF0(field_0x11EC);
                    BlurAndPaletteManager::GetInstance().setField_0x2F20(field_0x11EC);
                }
            } else {
                field_0x11FC--;
            }
            if (!EventManager::isInEvent()) {
                if (mMdl1.getAnm().isStop()) {
                    mMdl1.getAnm().setPlayState(m3d::PLAY_MODE_1);
                    mStateMgr.changeState(StateID_WaitOpen);
                } else {
                    mMdl1.play();
                }
            }
            break;
        case SMALL:
            if (!EventManager::isInEvent()) {
                if (mMdl1.getAnm().isStop()) {
                    mMdl1.getAnm().setPlayState(m3d::PLAY_MODE_1);
                    mStateMgr.changeState(StateID_WaitOpen);
                } else {
                    mMdl1.play();
                }
            }
            break;
        case GODDESS:
            if (field_0x11FC <= 0) {
                if (field_0x11EC < 1.0f) {
                    sLib::chase(&field_0x11EC, 1.0f, getSomeRate());
                    BlurAndPaletteManager::GetInstance().fn_80022AF0(field_0x11EC);
                    BlurAndPaletteManager::GetInstance().setField_0x2F20(field_0x11EC);
                }
            } else {
                field_0x11FC--;
            }
            if (!EventManager::isInEvent()) {
                if (mMdl1.getAnm().isStop()) {
                    mMdl1.getAnm().setPlayState(m3d::PLAY_MODE_1);
                    mStateMgr.changeState(StateID_GoddessWaitOn);
                } else {
                    mMdl1.play();
                }
            }
            break;
    }
}
void dAcTbox_c::finalizeState_Close() {}

void dAcTbox_c::initializeState_Wait() {
    mScale.set(1.0f, 1.0f, 1.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    switch ((u32)mVariant) {
        case NORMAL:
            field_0x11C0.set(-61.0f, 0.0f, -90.0f);
            field_0x11CC.set(61.0f, 142.0f, 42.0f);
            break;
        case SMALL:
            field_0x11C0.set(-38.0f, 0.0f, -68.0f);
            field_0x11CC.set(38.0f, 101.0f, 31.0f);
            break;
        case BOSS:
            field_0x11C0.set(-90.0f, 0.0f, -135.0f);
            field_0x11CC.set(90.0f, 163.0f, 56.0f);
            break;
    }
    field_0x120C = 1;
    clearActorProperty(0x100);
    if (mVariant == 0) {
        mMdl1.setAnm(sAnmNames[0], m3d::PLAY_MODE_4);
        mAnmMatClr1.setFrame(mAnmMatClr1.getFrameMax(0), 0);
    }
    mMdl1.setFrame(mMdl1.getAnm().getEndFrame());
    field_0x11FC = 0x2D;
    fn_8026D130();
}
void dAcTbox_c::executeState_Wait() {
    if (mVariant == NORMAL || mVariant == BOSS) {
        if (field_0x11FC <= 0) {
            if (field_0x11EC < 1.0f) {
                sLib::chase(&field_0x11EC, 1.0f, getSomeRate());
                BlurAndPaletteManager::GetInstance().fn_80022AF0(field_0x11EC);
                BlurAndPaletteManager::GetInstance().setField_0x2F20(field_0x11EC);
            }
        } else {
            field_0x11FC--;
        }
    }
}
void dAcTbox_c::finalizeState_Wait() {}

void dAcTbox_c::initializeState_GoddessWait() {
    mScale.set(1.0f, 1.0f, 1.0f);
    field_0x11C0.set(-62.0f, 0.0f, -95.0f);
    field_0x11CC.set(62.0f, 140.0f, 47.0f);
    field_0x11E8 = 1.0f;
    field_0x11F4 |= 2;
    field_0x120C = 1;
    clearActorProperty(0x100);
    field_0x11FC = 0x2D;
    fn_8026D130();
}
void dAcTbox_c::executeState_GoddessWait() {
    if (field_0x11FC <= 0) {
        if (field_0x11EC < 1.0f) {
            sLib::chase(&field_0x11EC, 1.0f, getSomeRate());
            BlurAndPaletteManager::GetInstance().fn_80022AF0(field_0x11EC);
            // this is apparently not needed here
            // BlurAndPaletteManager::GetInstance().setField_0x2F20(field_0x11EC);
        }
    } else {
        field_0x11FC--;
    }
}
void dAcTbox_c::finalizeState_GoddessWait() {}

void dAcTbox_c::getDowsingTargetOffset(mVec3_c *out) const {
    return getDowsingTargetOffset(mVariant, *out);
}

bool dAcTbox_c::fn_8026D120() const {
    return true;
}

void dAcTbox_c::fn_8026D130() {
    setActorProperty(0x1);
}

void dAcTbox_c::fn_8026D140() {
    clearActorProperty(0x1);
}

void dAcTbox_c::doInteraction(s32 _unused) {
    void *zevDat = getOarcZev(sArcNames[mVariant]);
    if (zevDat != nullptr) {
        Event ev(sOpenEventNames[mVariant], zevDat, 100, 0x100001, 0, 0);
        mEvent.scheduleEvent(ev, 0);
    }
}

extern "C" dAcBase_c *getCurrentEventActor();
void dAcTbox_c::registerInEvent() {
    if (getCurrentEventActor() == mItemRef.get()) {
        if (EventManager::isCurrentEvent("ItemGetDefaultTBox") || EventManager::isCurrentEvent("ItemGetGorgeousTBox")) {
            field_0x11F4 |= 0x8;
        }
    }
}

void dAcTbox_c::unkVirtFunc_0x6C() {
    if ((field_0x11F4 & 8) != 0) {
        if (mVariant == GODDESS && mHasBeenOpened == true) {
            mAnmGoddessPat.setFrame(0.0f, 0);
        }
        field_0x11F4 &= ~8;
    }
}

bool dAcTbox_c::checkShouldClose() {
    return field_0x11F4 & 0x1;
}

void dAcTbox_c::setShouldCloseFlag() {
    setFlags(field_0x11F4 | 0x1);
}

void dAcTbox_c::unsetShouldCloseFlag() {
    setFlags(field_0x11F4 & ~0x1);
}

void dAcTbox_c::fn_8026D370() {
    if (isNotSmall()) {
        field_0x11F0 = 1;
        BlurAndPaletteManager::GetInstance().fn_800223A0(&field_0x4DC);
    }
}

void dAcTbox_c::fn_8026D3C0() {
    if (mAnmChr.isStop() && mAnmTexSrt1.isStop(0) && mAnmMatClr2.isStop(0)) {
        field_0x11F0 = 0;
        BlurAndPaletteManager::GetInstance().fn_80022440(&field_0x4DC);
    } else {
        mAnmChr.play();
        mAnmTexSrt1.play();
        mAnmMatClr2.play();
        field_0x4E8.r = 0xAA;
        field_0x4E8.g = 0x96;
        field_0x4E8.b = 0x96;
        field_0x4EC = 125.0f;
    }
}

void dAcTbox_c::setChestFlag() {
    field_0x1207 = (rotation.z >> 9) & 0x3F;
}

void dAcTbox_c::setTboxFlag() {
    if (field_0x1207 <= 0x1F) {
        TBoxflagManager::sInstance->setFlag(field_0x1207);
    }
}

bool dAcTbox_c::checkTboxFlag() const {
    if (field_0x1207 <= 0x1F) {
        return TBoxflagManager::sInstance->checkFlagOnCurrentScene(field_0x1207);
    }
    return false;
}

bool dAcTbox_c::isNotSmall() const {
    return mVariant != SMALL;
}

bool dAcTbox_c::fn_8026D540() {
    return field_0x11F0 == 1;
}

bool dAcTbox_c::fn_8026D560() const {
    if (isValidGroupIndex(field_0x1208)) {
        u32 counterVal;
        getSomeCounter(&counterVal);
        f32 cmp = 1.0f / counterVal;
        return cM::rndF(1.0f) <= cmp;
    }
    return true;
}

bool dAcTbox_c::getSomeCounter(u32 *outValue) const {
    bool ok = isValidGroupIndex(field_0x1208);
    if (ok) {
        *outValue = sSomeCounters[field_0x1208];
        ok = *outValue;
    }
    return ok;
}

bool dAcTbox_c::fn_8026D670() const {
    return isValidGroupIndex(field_0x1208);
}

void dAcTbox_c::syncScaleToMdl(m3d::scnLeaf_c *lf) {
    lf->setScale(mScale);
}

bool dAcTbox_c::initBgW(dBgW &bg, const char *arcName, const char *fileName) {
    void *dzb = getOarcDZB(arcName, fileName);
    if (dzb == nullptr) {
        return false;
    }
    void *plc = getOarcPLC(arcName, fileName);
    if (plc == nullptr) {
        return false;
    }
    // TODO return types
    bool ok = !(BOOL)bg.Set((cBgD_t *)dzb, (PLC *)plc, 0x81, &mWorldMtx, &mScale);
    if (!ok) {
        return false;
    }
    bg.SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
    return true;
}

void dAcTbox_c::initDowsingTarget(DowsingTarget *target, DowsingTarget::DowsingSlot slot, mVec3_c *offset) {
    target->initialize(slot, 0, offset, 0.0f);
}

bool dAcTbox_c::isItemRupee() const {
    return dAcItem_c::isRupee((ITEM_ID)mItemId);
}

void dAcTbox_c::initDowsingTarget(DowsingTarget::DowsingSlot slot) {
    mVec3_c offset;
    getDowsingTargetOffset(&offset);
    initDowsingTarget(&mDowsingTarget, slot, &offset);
}

void dAcTbox_c::initDowsingTargetCube() {
    mVec3_c offset;
    getDowsingTargetOffset(&offset);
    initDowsingTarget(&mGoddessDowsingTarget, DowsingTarget::SLOT_GODDESS_CUBE, &offset);
}

void dAcTbox_c::noRegisterDowsing() {}

void dAcTbox_c::registerKeyPieceDowsing() {
    if ((field_0x11F4 & 4) == 0 && StoryflagManager::sInstance->getCounterOrFlag(0x6A)) {
        mDowsingTarget.doRegister();
        field_0x11F4 |= 4;
    }
}

void dAcTbox_c::registerRupeeOrTreasureDowsing() {
    if ((field_0x11F4 & 4) == 0) {
        mDowsingTarget.doRegister();
        field_0x11F4 |= 4;
    }
}

void dAcTbox_c::noUnregisterDowsing() {}

void dAcTbox_c::unregisterDowsing() {
    if ((field_0x11F4 & 4) != 0) {
        mDowsingTarget.doUnregister();
        field_0x11F4 &= ~4;
    }
}

extern "C" u16 PARTICLE_RESOURCE_ID_MAPPING_208_;

void dAcTbox_c::spawnAppearEffect() {
    fn_800298B0(PARTICLE_RESOURCE_ID_MAPPING_208_, &position, &rotation, nullptr, nullptr, nullptr, nullptr, nullptr);
}

bool dAcTbox_c::checkIsClear() const {
    f32 fs[] = {
        position.y,
        position.y,
    };
    fs[0] += 20.0f;
    fs[1] += 60.0f;
    static mVec3_c offsets[] = {
        mVec3_c(50.0f, 500.0f, 50.0f),
        mVec3_c(50.0f, 500.0f, -50.0f),
        mVec3_c(-50.0f, 500.0f, 50.0f),
        mVec3_c(-50.0f, 500.0f, -50.0f),
    };
    mVec3_c points[4];
    for (u32 i = 0; i <= 3; i++) {
        fn_8026DAD0(&offsets[i], &points[i]);
    }
    static const int fsIdxes[] = {0, 0, 1, 1};
    bool isClear = true;
    for (u32 i = 0; isClear && i <= 3;) {
        // @bug should this be points[i] instead?
        if (isBelowGroundAtPos(fs[fsIdxes[i]], points[0])) {
            isClear = false;
        } else {
            i++;
        }
    }

    return isClear;
}

void dAcTbox_c::fn_8026DAC0(mAng &ang) {
    ang = rotation.y - 0x8000;
}

void dAcTbox_c::fn_8026DAD0(const mVec3_c *a, mVec3_c *b) const {
    PSMTXMultVec(mWorldMtx, *a, *b);
}

void dAcTbox_c::getCylParams(mVec3_c *c, f32 *r, f32 *h) const {
    switch ((u32)mVariant) {
        case NORMAL:
            switch (field_0x120C) {
                case 0:
                    if (c != nullptr) {
                        c->set(0.0f, 0.0f, 0.0f);
                    }
                    if (r != nullptr) {
                        *r = 70.0f;
                    }
                    if (h != nullptr) {
                        *h = 100.0f;
                    }
                    break;
                case 1:
                    if (c != nullptr) {
                        c->set(0.0f, 0.0f, -24.0f);
                    }
                    if (r != nullptr) {
                        *r = 86.0f;
                    }
                    if (h != nullptr) {
                        *h = 140.0f;
                    }
                    break;
            }
            break;
        case SMALL:
            switch (field_0x120C) {
                case 0:
                    if (c != nullptr) {
                        c->set(0.0f, 0.0f, 0.0f);
                    }
                    if (r != nullptr) {
                        *r = 48.0f;
                    }
                    if (h != nullptr) {
                        *h = 68.0f;
                    }
                    break;
                case 1:
                    if (c != nullptr) {
                        c->set(0.0f, 0.0f, -14.0f);
                    }
                    if (r != nullptr) {
                        *r = 60.0f;
                    }
                    if (h != nullptr) {
                        *h = 100.0f;
                    }
                    break;
            }
            break;
        case BOSS:
            switch (field_0x120C) {
                case 0:
                    if (c != nullptr) {
                        c->set(0.0f, 0.0f, 0.0f);
                    }
                    if (r != nullptr) {
                        *r = 100.0f;
                    }
                    if (h != nullptr) {
                        *h = 132.0f;
                    }
                    break;
                case 1:
                    if (c != nullptr) {
                        c->set(0.0f, 0.0f, -44.0f);
                    }
                    if (r != nullptr) {
                        *r = 122.0f;
                    }
                    if (h != nullptr) {
                        *h = 160.0f;
                    }
                    break;
            }
            break;
        case GODDESS:
            switch (field_0x120C) {
                case 0:
                    if (c != nullptr) {
                        c->set(0.0f, 0.0f, 0.0f);
                    }
                    if (r != nullptr) {
                        *r = 73.0f;
                    }
                    if (h != nullptr) {
                        *h = 100.0f;
                    }
                    break;
                case 1:
                    if (c != nullptr) {
                        c->set(0.0f, 0.0f, -23.0f);
                    }
                    if (r != nullptr) {
                        *r = 88.0f;
                    }
                    if (h != nullptr) {
                        *h = 140.0f;
                    }
                    break;
            }
            break;
    }
    if (c != nullptr) {
        PSMTXMultVec(mWorldMtx, *c, *c);
    }
}

void dAcTbox_c::getCCBounds(mVec3_c *lo, mVec3_c *up) const {
    if (lo != nullptr) {
        switch ((u32)mVariant) {
            case NORMAL:
                switch (field_0x120C) {
                    case 0: lo->set(-61.0f, 0.0f, -42.0f); break;
                    case 1: lo->set(-61.0f, 0.0f, -90.0f); break;
                }
                break;
            case SMALL:
                switch (field_0x120C) {
                    case 0: lo->set(-38.0f, 0.0f, -35.0f); break;
                    case 1: lo->set(-38.0f, 0.0f, -68.0f); break;
                }
                break;
            case BOSS:
                switch (field_0x120C) {
                    case 0: lo->set(-90.0f, 0.0f, -53.0f); break;
                    case 1: lo->set(-90.0f, 0.0f, -135.0f); break;
                }
                break;
            case GODDESS:
                switch (field_0x120C) {
                    case 0: lo->set(-62.0f, 0.0f, -47.0f); break;
                    case 1: lo->set(-62.0f, 0.0f, -95.0f); break;
                }
                break;
        }
    }
    if (up != nullptr) {
        switch ((u32)mVariant) {
            case NORMAL:
                switch (field_0x120C) {
                    case 0: up->set(61.0f, 100.0f, 42.0f); break;
                    case 1: up->set(61.0f, 142.0f, 42.0f); break;
                }
                break;
            case SMALL:
                switch (field_0x120C) {
                    case 0: up->set(38.0f, 68.0f, 35.0f); break;
                    case 1: up->set(38.0f, 101.0f, 31.0f); break;
                }
                break;
            case BOSS:
                switch (field_0x120C) {
                    case 0: up->set(90.0f, 133.0f, 56.0f); break;
                    case 1: up->set(90.0f, 163.0f, 56.0f); break;
                }
                break;
            case GODDESS:
                switch (field_0x120C) {
                    case 0: up->set(62.0f, 100.0f, 47.0f); break;
                    case 1: up->set(62.0f, 140.0f, 47.0f); break;
                }
                break;
        }
    }
}

const InteractionTargetDef &dAcTbox_c::getInteractionTargetDef() const {
    switch ((u32)mVariant) {
        case NORMAL:
        default:     {
            static const InteractionTargetDef tmpTarget = {0,     3,     1,       OPEN,   0,    200.0f,
                                                           60.0f, 60.0f, -100.0f, 100.0f, 0.0f, 1.0f};
            return tmpTarget;
        }
        case SMALL: {
            static const InteractionTargetDef tmpTarget = {0,     3,     1,       OPEN,   0,    160.0f,
                                                           60.0f, 60.0f, -100.0f, 100.0f, 0.0f, 1.0f};
            return tmpTarget;
        }
        case BOSS: {
            static const InteractionTargetDef tmpTarget = {0,     3,     1,       OPEN,   0,    200.0f,
                                                           60.0f, 60.0f, -100.0f, 100.0f, 0.0f, 1.0f};
            return tmpTarget;
        }
        case GODDESS: {
            static const InteractionTargetDef tmpTarget = {0,     3,     1,       OPEN,   0,    200.0f,
                                                           60.0f, 60.0f, -100.0f, 100.0f, 0.0f, 1.0f};
            return tmpTarget;
        }
    }
}

bool dAcTbox_c::checkForLinkBonk() {
    if (!mCcD4.ChkCoHit()) {
        return false;
    }
    dAcPy_c *link = dAcPy_c::LINK;
    if (link == nullptr) {
        return false;
    }
    if (link->getCurrentAction() != /* ROLL */ 12) {
        return false;
    }

    mVec3_c linkPos;
    PSMTXMultVec(mCcD3.mInvMtx, link->position, linkPos);
    mVec3_c ccLo, ccHi;
    getCCBounds(&ccLo, &ccHi);
    // Math performed in local coordinate space
    // All of this is wrong
    if (!(ccLo.y <= linkPos.y + 100.0f && linkPos.y <= ccHi.y)) {
        return false;
    }

    bool bonk = false;
    bonk = bonk ||
           (ccLo.x <= linkPos.x && linkPos.x <= ccHi.x) && (ccLo.z - 80.0f <= linkPos.z && linkPos.z <= ccHi.z + 80.0f);
    bonk = bonk ||
           (ccLo.z <= linkPos.z && linkPos.z <= ccHi.z) && (ccLo.x - 80.0f <= linkPos.x && linkPos.x <= ccHi.x + 80.0f);
    bonk = bonk || (linkPos.x < ccLo.x && linkPos.z < ccLo.z) && (linkPos.inprodXZ(ccLo) <= 6400.0f);
    bonk = bonk || (ccHi.x < linkPos.x && ccHi.z < linkPos.z) && (linkPos.inprodXZ(ccLo) <= 6400.0f);
    bonk = bonk || (linkPos.x < ccLo.x && ccLo.z < linkPos.z) && (linkPos.inprodXZ(ccLo) <= 6400.0f);
    bonk = bonk || (ccHi.x < linkPos.x && linkPos.z < ccHi.z) && (linkPos.inprodXZ(ccLo) <= 6400.0f);

    if (!bonk) {
        return false;
    }

    int direction = 0;
    mVec3_c diff = mVec3_c(linkPos.x - mCcD3.mField_0xA8.x, 0.0f, linkPos.z - mCcD3.mField_0xA8.z);
    if (VEC3LenSq(diff) <= FLT_EPSILON) {
        return false;
    }

    // TODO

    link->bonk();

    return false;
}

void dAcTbox_c::fn_8026E630() {
    mVec3_c offset = mVec3_c::Ey * 10.0f;
    mVec3_c checkPos = position + offset;
    // TODO reference vs pointer
    if (dBgS_ObjGndChk::CheckPos(checkPos) && &dBgS_ObjGndChk::GetInstance()) {
        cBgS_PolyInfo p = dBgS_ObjGndChk::GetInstance();
        dBgS::GetInstance()->SetLightingCode(this, p);
        setPolyAttrsDupe(p);
    }
}
