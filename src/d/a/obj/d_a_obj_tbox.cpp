#include "d/a/obj/d_a_obj_tbox.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "m/m3d/m_fanm.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_resanmchr.h"
#include "nw4r/g3d/g3d_resanmtexpat.h"
#include "nw4r/g3d/g3d_resanmtexsrt.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "nw4r/g3d/g3d_resmdl.h"
#include "s/s_Math.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/room_manager.h"

SPECIAL_ACTOR_PROFILE(TBOX, dAcTbox_c, fProfile::TBOX, 0x018D, 0, 6);

extern "C" mVec3_c vecs[] = {
    mVec3_c(0.0f, 54.0f, 0.0f), mVec3_c(0.0f, 34.0f, 0.0f), mVec3_c(0.0f, 66.5f, 0.0f), mVec3_c(0.0f, 49.0f, 0.0f)
};

extern "C" mVec3_c v1 = mVec3_c(0.0f, 0.0f, -2.0f);
extern "C" mVec3_c v2 = mVec3_c(1.0f, 1.0f, 0.8f);
extern "C" mVec3_c v3 = mVec3_c(1.0f, 1.0f, 1.0f);
extern "C" mVec3_c v4 = mVec3_c(0.0f, 0.0f, 88.0f);

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

extern "C" char *const sOpenEventNames[] = {
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

static char *const sMdlNames[] = {
    "TBoxNormalT",
    "TBoxSmallT",
    "TBoxBossT",
    "GoddessTBox",
};

static char *const sAnmNames[] = {
    "TBoxNormalT",
    "TBoxSmallT",
    "TBoxBossT",
    "TBoxNormalT",
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

dAcTbox_c::dAcTbox_c()
    : mStateMgr(*this, sStateID::null), mScnCallback(this), mEvent(*this, nullptr), mTboxListNode(this),
      mDowsingTarget1(this, DowsingTarget::SLOT_NONE), mDowsingTarget2(this, DowsingTarget::SLOT_NONE) {
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
        nw4r::g3d::ResFile res = data;
        if (!res.mFile.IsValid()) {
            return false;
        }
        nw4r::g3d::ResMdl mdl = mMdl1.getModel().getResMdl();
        if (!mdl.IsValid()) {
            return false;
        }
        nw4r::g3d::ResAnmTexPat anmTexPat = res.GetResAnmTexPat("GoddessTBox");
        if (!anmTexPat.mAnmTexPat.IsValid()) {
            return false;
        }
        if (!mAnmGoddessPat.create(mdl, anmTexPat, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mMdl1.getModel().setAnm(mAnmGoddessPat);
        u16 goddessTBoxActive = getParams2Lower();
        if (StoryflagManager::sInstance->getCounterOrFlag(goddessTBoxActive) && !mHasBeenOpened) {
            nw4r::g3d::ResAnmTexSrt anmTexSrt = res.GetResAnmTexSrt("GoddessTBox");
            if (!anmTexSrt.mAnmTexSrt.IsValid()) {
                return false;
            }
            if (!mAnmGoddessTexSrt.create(mdl, anmTexSrt, &heap_allocator, nullptr, 1)) {
                return false;
            }
            mMdl1.getModel().setAnm(mAnmGoddessTexSrt);
        }
    } else if (mVariant == NORMAL) {
        nw4r::g3d::ResFile res = data;
        if (!res.mFile.IsValid()) {
            return false;
        }
        nw4r::g3d::ResAnmClr anmClr = res.GetResAnmClr("TBoxNormalTAppear");
        if (!anmClr.mAnmClr.IsValid()) {
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
        nw4r::g3d::ResFile fxRes = fxData;
        if (!fxRes.mFile.IsValid()) {
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
        if (!openAnm.mAnmChr.IsValid()) {
            return false;
        }
        if (!mAnmChr.create(openMdl, openAnm, &heap_allocator, nullptr)) {
            return false;
        }
        mOpenFxMdl.setAnm(mAnmChr);

        nw4r::g3d::ResAnmTexSrt anmTexSrt = fxRes.GetResAnmTexSrt(sOpenAnmTexSrtName);
        if (!anmTexSrt.mAnmTexSrt.IsValid()) {
            return false;
        }
        if (!mAnmTexSrt1.create(openMdl, anmTexSrt, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mOpenFxMdl.setAnm(mAnmTexSrt1);

        nw4r::g3d::ResAnmClr anmClr = fxRes.GetResAnmClr(sOpenAnmClrName);
        if (!anmClr.mAnmClr.IsValid()) {
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

static u32 sSomeCounters[16] = {};

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
    mVariant = sItemToTBoxVariant[mItemId];
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
    if (fn_80268660(field_0x1208) && !field_0x1210) {
        sSomeCounters[field_0x1208]++;
        field_0x1210 = true;
    }
    field_0x120D = 0;
    if (fn_8026D670()) {
        u32 counterValue = 0;
        getSomeCounter(&counterValue);
        if (counterValue > 1) {
            bool keepGoing = true;
            dAcTbox_c *other;
            dAcObjBase_c *cursor = nullptr;
            do {
                cursor = getNextObject(&sTboxActorList, cursor);
                other = static_cast<dAcTbox_c *>(cursor);
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
                field_0x120D = true;
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
    mCcD4.SetStts(mStts);
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
    mCcD1.SetTgType(-1);
    mCcD2.SetTgType(-1);
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
        OarcManager *mng = OarcManager::sInstance;
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

    dRoom *r = RoomManager::m_Instance->GetRoomByIndex(roomid);
    bool hasFlags = r->checkFlag(0x1E);
    if (hasFlags) {
        setObjectProperty(0x200);
    } else {
        clearObjectProperty(0x200);
    }

    if (hasFlags) {
        return SUCCEEDED;
    }

    fn_8026E090();
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
        fn_8026DD70(&v1, &v2);
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
                        v2.set(38.0f, 45.0f, 35.0f);
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
            mCcD2.OffTgSet();
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

    dRoom *r = RoomManager::m_Instance->GetRoomByIndex(roomid);
    bool hasFlags = r->checkFlag(0x1E);
    if (hasFlags) {
        setObjectProperty(0x200);
    } else {
        clearObjectProperty(0x200);
    }

    if (hasFlags) {
        return SUCCEEDED;
    }

    fn_8026E090();
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
            fn_8026DAD0(&v4, &v1);
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
        fn_8026DD70(&v1, &v2);
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
                        v2.set(38.0f, 45.0f, 35.0f);
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
            mCcD2.OffTgSet();
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
        fn_8026DAD0(&v1, &out);
    }
}

const mVec3_c &dAcTbox_c::fn_8026B3C0() const {
    if (mVariant == BOSS) {
        return v3;
    }
    return v2;
}

void dAcTbox_c::initializeState_DugOut() {}
void dAcTbox_c::executeState_DugOut() {}
void dAcTbox_c::finalizeState_DugOut() {}
void dAcTbox_c::initializeState_WaitAppear() {
    mScale.set(0.0f, 0.0f, 0.0f);
    setActorProperty(0x100);
    fn_8026D140();
    field_0x11E8 = 0.0f;
    field_0x11F4 &= ~0x2;
    field_0x120C = 0;
    // field_0x11C0
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
    // TODO, just referencing this bit of data
    mMdl1.setAnm(sAppearAnmName, m3d::PLAY_MODE_4);
}
void dAcTbox_c::executeState_DemoAppear() {}
void dAcTbox_c::finalizeState_DemoAppear() {}
void dAcTbox_c::initializeState_WaitOpen() {}
void dAcTbox_c::executeState_WaitOpen() {}
void dAcTbox_c::finalizeState_WaitOpen() {}
void dAcTbox_c::initializeState_GoddessWaitOff() {}
void dAcTbox_c::executeState_GoddessWaitOff() {}
void dAcTbox_c::finalizeState_GoddessWaitOff() {}
void dAcTbox_c::initializeState_GoddessWaitOn() {}
void dAcTbox_c::executeState_GoddessWaitOn() {}
void dAcTbox_c::finalizeState_GoddessWaitOn() {}
void dAcTbox_c::initializeState_DeleteArchive() {}
void dAcTbox_c::executeState_DeleteArchive() {
    if (sCurrentObtainingItemOarcName != nullptr) {
        OarcManager *mng = OarcManager::sInstance;
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
void dAcTbox_c::initializeState_LoadArchive() {}
void dAcTbox_c::executeState_LoadArchive() {}
void dAcTbox_c::finalizeState_LoadArchive() {}
void dAcTbox_c::initializeState_Open() {}
void dAcTbox_c::executeState_Open() {}
void dAcTbox_c::finalizeState_Open() {}
void dAcTbox_c::initializeState_PresentItem() {}
void dAcTbox_c::executeState_PresentItem() {}
void dAcTbox_c::finalizeState_PresentItem() {}
void dAcTbox_c::initializeState_Close() {}
void dAcTbox_c::executeState_Close() {}
void dAcTbox_c::finalizeState_Close() {}
void dAcTbox_c::initializeState_Wait() {}
void dAcTbox_c::executeState_Wait() {}
void dAcTbox_c::finalizeState_Wait() {}
void dAcTbox_c::initializeState_GoddessWait() {}
void dAcTbox_c::executeState_GoddessWait() {}
void dAcTbox_c::finalizeState_GoddessWait() {}

bool dAcTbox_c::isNotSmall() const {
    return mVariant != SMALL;
}

extern "C" const u32 DAT_804ee478[] = {0, 0, 1, 1};

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
