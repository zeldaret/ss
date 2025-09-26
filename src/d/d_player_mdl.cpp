#include "d/d_player_mdl.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_heap.h"
#include "d/d_pouch.h"
#include "d/d_rawarchive.h"
#include "d/d_sc_game.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "egg/core/eggHeap.h"
#include "m/m3d/m3d.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_mdl.h"
#include "m/m_allocator.h"
#include "m/m_heap.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/g3d/g3d_anmobj.h"
#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_types.h"
#include "rvl/CX/cx.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtxvec.h"
#include "sized_string.h"
#include "toBeSorted/cx_util.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/stage_render_stuff.h"

static u8 sSavedHandMats[2] = {};
static u8 sHandMats[14] = {};

#pragma push
#pragma readonly_strings on
static const char *sFaceResNames[] = {
    "F_Default",
    "Fmaba01",
    "F_M",
    "F_Ha",
    "F_Cut",
    "F_CutAround",
    "F_Damage",
    "F_DamageLarge",
    "F_Die",
    "F_DieLargeDamage",
    "F_DieMagma",
    "F_Pain",
    "F_Wa",
    "F_DrinkSt",
    "F_Drink",
    "F_DrinkEd",
    "F_Breath",
    "F_Close",
    "F_entrance",
    "F_Binout",
    "F_GetBigWait",
    "F_getAwaitNice",
    "F_WaitLookRound",
    "F_Danger",
    "F_BinShield",
    "F_swimDieA",
    "F_WaitServiceA",
    "F_WaitServiceHotA",
    "F_BoxOp",
    "F_Notice",
    "F_swimP",
    "F_BedSleep",
    "F_BedAwake",
    "F_SwordPullSt",
    "F_SwordPullWait",
    "F_SwordPull",
    "F_SwordSirenSt",
    "F_SwordSirenRaise",
    "F_SwordSirenStick",
    "F_SwordSirenAfter",
    "F_Bird_Reach",
    "F_Bird_GetWait",
    "F_DieTruck",
    "F_Bird_FestivalStart",
    "F_GetSurprise",
    "F_AsuraCut",
    "F_AsuraCutSt",
    "F_AsuraCutEd",
    "F_CannonWait",
    "F_KrakenAppearance",
    "F_GetFlower",
    "F_Harp",
    "F_SitWaitServiceA",
    "F_Lightning",
    "F_LastAttack",
    "F_Bird_Call",
    // this one will go in sbss2 if you explicitly define each string
    // with const char[], so either a pragma or a file wide flag is used,
    // and this whole array is part of the .data pool
    "",
};

// clang-format off
const PlayerAnimation daPlayerModelBase_c::sAnimations[] = {
    {                "atrFW",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {              "atrFW_R",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "atrFD",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {              "atrFD_R",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "atrbW",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {              "atrbW_R",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "atrbD",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {              "atrbD_R",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "atrsWL",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "atrsDL",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "atrsWR",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "atrsDR",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "atsWL",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "atsDL",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "atsWR",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "atsDR",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {          "atsWL_right",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {          "atsDL_right",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                  "atL",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                  "atR",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "dashA",  13,  56,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "dashS",  13,  56,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "atrbWH",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                 "atbW",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                 "atbD",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "walkH",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "waitS",  13,  56,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "walkS",  13,  56,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "stepL",  13,  56,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "stepR",  13,  56,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {              "atsJLSt",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "atsJLEd",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "atsJRSt",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "atsJREd",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "atsJFSt",  13,  56,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "atsJFEd",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "rollBSt",  13,  56,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "rollBEd",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "waitB",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "waitBtoA",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "sjmpSt",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "sjmpEd",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "rollF",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "rollFmis",   7,   0,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {               "jumpSt",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "jumpSEd",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                 "vjmp",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "vjmpChA",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "vjmpChB",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "vjmpCl",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "hangL",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "hangR",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                 "Hang",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "climbHang",  12,  55,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "climbSlideL",  12,  55,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "climbSlideR",  12,  55,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {        "climbHangmiss",  12,  55,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {        "climbStruggle",  12,   5,               PLAYER_FACE_PAIN,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_PAIN},
    {         "ClimbIvyHang",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "ClimbIvyL",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "ClimbIvyR",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "ClimbIvyL_St",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "ClimbIvyR_St",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {       "ClimbIvySlideL",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {       "ClimbIvySlideR",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {   "ClimbIvySlantLU_RD",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {   "ClimbIvySlideRU_LD",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "ladUpSt",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "ladUpEdL",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "ladUpEdR",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {              "ladDwSt",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "ladDWEdL",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "ladDWEdR",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {              "ladRtoL",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {              "ladLtoR",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {          "LadJumpUpSt",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {        "LadJumpDownSt",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {          "LadJumpUpEd",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {        "LadJumpDownEd",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                 "cutT",  13, 100,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "cutTB",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "cutTL",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "cutTLB",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "cutJSt",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "cutJEd",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                 "damF",  13,  56,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {                 "damB",  13,  56,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {                 "damL",  13,  56,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {                 "damR",  13,  56,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {         "DamageMFront",  13,  56,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {          "DamageMBack",  13,  56,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {          "DamageMLeft",  13,  56,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {         "DamageMRight",  13,  56,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {                "damFF",  13,  56,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {                "damFB",  13,  56,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {                "damFL",  13,  56,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {                "damFR",  13,  56,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {              "damFFup",   7,  50,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {              "damFBup",   7,  50,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {              "damFLup",   7,  50,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {              "damFRup",   7,  50,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {    "DamageLFrontSlide",   7,  50,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {     "DamageLBackSlide",   7,  50,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {     "DamageLLeftSlide",   7,  50,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {    "DamageLRightSlide",   7,  50,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {                  "dam",  13,  56,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {             "landDamA",   7,  50,               PLAYER_FACE_NONE,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_NONE},
    {           "landDamAst",  13,  50,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {                "cutST",   9,  52,               PLAYER_FACE_PAIN,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_PAIN},
    {              "atDefNG",   9,  52,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {                  "die",  13,  56,                PLAYER_FACE_DIE,                PLAYER_FACE_DIE,             PLAYER_FACE_DIE},
    {  "DieLargeDamageFront",   7,   0,     PLAYER_FACE_DIELARGEDAMAGE,     PLAYER_FACE_DIELARGEDAMAGE,  PLAYER_FACE_DIELARGEDAMAGE},
    {   "DieLargeDamageBack",   7,   0,     PLAYER_FACE_DIELARGEDAMAGE,     PLAYER_FACE_DIELARGEDAMAGE,  PLAYER_FACE_DIELARGEDAMAGE},
    {   "DieLargeDamageLeft",   7,   0,     PLAYER_FACE_DIELARGEDAMAGE,     PLAYER_FACE_DIELARGEDAMAGE,  PLAYER_FACE_DIELARGEDAMAGE},
    {  "DieLargeDamageRight",   7,   0,     PLAYER_FACE_DIELARGEDAMAGE,     PLAYER_FACE_DIELARGEDAMAGE,  PLAYER_FACE_DIELARGEDAMAGE},
    {             "swimwait",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "swimingA",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "swimP",   7,   0,              PLAYER_FACE_SWIMP,              PLAYER_FACE_SWIMP,           PLAYER_FACE_SWIMP},
    {               "atsSwL",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "atsSwR",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "atbSw",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "swimDive",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {            "swimingSt",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "swimingB",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "swimingC",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {       "SwimAttackMiss",   7,   0,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {               "damSwF",  13,   6,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {               "damSwB",  13,   6,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {               "damSwL",  13,   6,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {               "damSwR",  13,   6,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {               "damSwW",  13,   6,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {             "swimDieA",  13,   6,           PLAYER_FACE_SWIMDIEA,           PLAYER_FACE_SWIMDIEA,        PLAYER_FACE_SWIMDIEA},
    {             "swimDieP",   7,   0,                 PLAYER_FACE_WA,                PLAYER_FACE_DIE,              PLAYER_FACE_WA},
    {             "DieMagma",   7,   0,           PLAYER_FACE_DIEMAGMA,           PLAYER_FACE_DIEMAGMA,        PLAYER_FACE_DIEMAGMA},
    {               "slideF",   7,  50,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {               "slideB",   7,  50,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {             "slideFEd",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "slideBEd",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "HSjmpL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "HSjmpR",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "HSshoot",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "HShangEdL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "HShangEdR",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "HShangL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "HShangR",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "HShangwaitL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "HShangwaitR",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "HShangshootL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "HShangshootR",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "HSwallEdL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "HSwallEdR",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "HSwallL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "HSwallR",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "HSwallwaitL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "HSwallwaitR",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "HSwallshootL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "HSwallshootR",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                  "lie",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                 "lieF",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "waitST",   9,   2,               PLAYER_FACE_PAIN,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_PAIN},
    {   "WaitStruggleDamage",   9,   2,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {               "waitPP",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "pushW",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "pullW",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "jumpBSt",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_HA},
    {                "jumpB",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "waitD",  13,  56,               PLAYER_FACE_NONE,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_NONE},
    {             "waitAtoD",  13,  56,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {         "WaitServiceA",  13,   6,       PLAYER_FACE_WAITSERVICEA,       PLAYER_FACE_WAITSERVICEA,    PLAYER_FACE_WAITSERVICEA},
    {      "WaitServiceHotA",  13,   6,    PLAYER_FACE_WAITSERVICEHOTA,    PLAYER_FACE_WAITSERVICEHOTA, PLAYER_FACE_WAITSERVICEHOTA},
    {              "dashUpL",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "dashUpR",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "climbjumpUSt",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "climbjumpDSt",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "climbjumpLSt",  12,  55,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "climbjumpRSt",  12,  55,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "climbjumpUEd",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "climbjumpDEd",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "climbjumpLEd",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "climbjumpREd",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "hangjumpLSt",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "hangjumpRSt",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "hangjumpLEd",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "hangjumpREd",   7,  50,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "DoorKnobPull",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {         "DoorKnobPush",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {     "DoorKnobPullLock",  13,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {     "DoorKnobPushLock",  13,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {              "DoorOpD",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "dashB",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "MgDig",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "MPCutU",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "MPCutU_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "MPCutLU",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "MPCutLU_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "MPCutL",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "MPCutL_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "MPCutLD",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "MPCutLD_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "MPCutRD",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "MPCutRD_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "MPCutR",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "MPCutR_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "MPCutRU",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "MPCutRU_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "MPCutD_right",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {      "MPCutD_right_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "MPCutD_left",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {       "MPCutD_left_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "CutPush",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "CutPush_Re",  13,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {       "EnemyGuardJust",  13, 100,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {         "LiftB_Before",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {         "LiftM_Before",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "LiftB",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "LiftS",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "LiftM",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "LiftL",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "ThrowBU",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "ThrowSU",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "ThrowMU",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "ThrowLU",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "ThrowBD",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "ThrowSD",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {        "MPGuardShield",   7,   2,                PLAYER_FACE_CUT,            PLAYER_FACE_DEFAULT,             PLAYER_FACE_CUT},
    { "MPGuardShield_Impact",   7,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    { "MPGuardShield_Normal",   7,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "vjmpChA",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "RopeHoldWait",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "RopeClimbL",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "RopeHoldWait",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "RopePull",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {        "RopeTightHold",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "RopeFront",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "RopeBack",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "RopeStruggle",   8,   1,               PLAYER_FACE_PAIN,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_PAIN},
    {               "DiveSt",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                 "Dive",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "StoleLand",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "DiveDrop",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "Float",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {            "FloatAway",   9,   2,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {          "DiveToFloat",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {    "RocketBeetle_Wait",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {   "RocketBeetle_Shoot",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {   "RocketBeetle_Reset",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {    "RocketBeetle_Back",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "WhipU",   8,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "WhipLU",   8,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "WhipL",   8,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "WhipLD",   8,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "WhipRD",   8,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                "WhipR",   8,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "WhipRU",   8,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "WhipBackRD",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "WhipBackR",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "WhipBackRU",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "WhipBackU",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "WhipBackLU",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "WhipBackL",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {           "WhipBackLD",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "turnBack",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "atLtoR",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "atRtoL",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {       "ChuchuStruggle",   9,  52,               PLAYER_FACE_PAIN,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_PAIN},
    {   "ChuchuStruggle_big",   9,  52,               PLAYER_FACE_PAIN,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_PAIN},
    {   "ChuchuStruggle_act",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {"ChuchuStruggle_Escape",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "Surprise",  13,  56,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {               "Danger",   9,  52,             PLAYER_FACE_DANGER,             PLAYER_FACE_DANGER,          PLAYER_FACE_DANGER},
    {             "RockRide",   9,  52,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {               "Notice",  13,  56,             PLAYER_FACE_NOTICE,             PLAYER_FACE_NOTICE,          PLAYER_FACE_NOTICE},
    {           "waitinsect",   9,  52,               PLAYER_FACE_PAIN,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_PAIN},
    {                "numbA",  12,  55,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {                "numbB",  12,   5,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {               "damFEF",   7,  50,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {               "damFEB",   7,  50,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {               "damFEL",   7,  50,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {               "damFER",   7,  50,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {                 "getA",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "getAwait",   7,   0,              PLAYER_FACEMABA01,       PLAYER_FACE_GETAWAITNICE,    PLAYER_FACE_GETAWAITNICE},
    {               "GetBig",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {           "GetBigWait",   7,   0,              PLAYER_FACEMABA01,         PLAYER_FACE_GETBIGWAIT,      PLAYER_FACE_GETBIGWAIT},
    {           "BoxOpSmall",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {                "BoxOp",   7,   0,              PLAYER_FACEMABA01,              PLAYER_FACE_BOXOP,           PLAYER_FACE_BOXOP},
    {               "cutUSt",   7, 100,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {                 "cutU",   7, 100,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "cutUEd",   9, 100,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {               "cutUNG",   9, 100,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {        "WaitLookRound",  13,  56,              PLAYER_FACEMABA01,      PLAYER_FACE_WAITLOOKROUND,   PLAYER_FACE_WAITLOOKROUND},
    {             "entrance",  13,   6,           PLAYER_FACE_ENTRANCE,           PLAYER_FACE_ENTRANCE,        PLAYER_FACE_ENTRANCE},
    {          "MPPosePower",  13, 255,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "MPPosePowerL",  13, 255,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "BinswingS",  13,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {            "BinswingU",  13,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "Binout",  13,   5,             PLAYER_FACE_BINOUT,             PLAYER_FACE_BINOUT,          PLAYER_FACE_BINOUT},
    {                "Drift",   7,   0,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {       "swimDieARevive",  13,   6,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {         "MgLieFRevive",   7,   0,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {               "CatchL",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {               "CatchR",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {          "BattleReady",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "RollJump",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "TansuOpen",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {           "TansuClose",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {            "BedSleepL",  13,   6,           PLAYER_FACE_BEDSLEEP,           PLAYER_FACE_BEDSLEEP,        PLAYER_FACE_BEDSLEEP},
    {             "BedSleep",  13,   6,           PLAYER_FACE_BEDSLEEP,           PLAYER_FACE_BEDSLEEP,        PLAYER_FACE_BEDSLEEP},
    {             "BedAwake",   8,   1,           PLAYER_FACE_BEDAWAKE,           PLAYER_FACE_BEDAWAKE,        PLAYER_FACE_BEDAWAKE},
    {           "Bird_Glide",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {     "Bird_GlideAround",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {           "Bird_Reach",   8,   5,         PLAYER_FACE_BIRD_REACH,         PLAYER_FACE_BIRD_REACH,      PLAYER_FACE_BIRD_REACH},
    {         "Bird_GetWait",   8,   0,       PLAYER_FACE_BIRD_GETWAIT,       PLAYER_FACE_BIRD_GETWAIT,    PLAYER_FACE_BIRD_GETWAIT},
    {           "Bird_Accel",   8,   1,                PLAYER_FACE_CUT,            PLAYER_FACE_DEFAULT,             PLAYER_FACE_CUT},
    {          "Bird_Damage",   8,   1,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {     "Bird_DamageSmall",   8,   1,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {   "Bird_DamageTornado",   8,   1,        PLAYER_FACE_DAMAGELARGE,            PLAYER_FACE_DEFAULT,     PLAYER_FACE_DAMAGELARGE},
    {            "Bird_Call",  10,   3,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,       PLAYER_FACE_BIRD_CALL},
    {          "SwordPullSt",  13,   6,        PLAYER_FACE_SWORDPULLST,        PLAYER_FACE_SWORDPULLST,     PLAYER_FACE_SWORDPULLST},
    {        "SwordPullWait",   8,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,   PLAYER_FACE_SWORDPULLWAIT},
    {            "SwordPull",   8,   1,          PLAYER_FACE_SWORDPULL,          PLAYER_FACE_SWORDPULL,       PLAYER_FACE_SWORDPULL},
    {         "SwordSirenSt",  13,   1,       PLAYER_FACE_SWORDSIRENST,       PLAYER_FACE_SWORDSIRENST,    PLAYER_FACE_SWORDSIRENST},
    {     "SwordSirenStWait",   8,   1,       PLAYER_FACE_SWORDSIRENST,       PLAYER_FACE_SWORDSIRENST,    PLAYER_FACE_SWORDSIRENST},
    {      "SwordSirenRaise",   8,   1,    PLAYER_FACE_SWORDSIRENRAISE,    PLAYER_FACE_SWORDSIRENRAISE, PLAYER_FACE_SWORDSIRENRAISE},
    {  "SwordSirenRaiseWait",   8,   1,    PLAYER_FACE_SWORDSIRENRAISE,    PLAYER_FACE_SWORDSIRENRAISE, PLAYER_FACE_SWORDSIRENRAISE},
    {      "SwordSirenStick",   8,   1,    PLAYER_FACE_SWORDSIRENSTICK,    PLAYER_FACE_SWORDSIRENSTICK, PLAYER_FACE_SWORDSIRENSTICK},
    {      "SwordSirenAfter",   8, 100,    PLAYER_FACE_SWORDSIRENAFTER,    PLAYER_FACE_SWORDSIRENAFTER, PLAYER_FACE_SWORDSIRENAFTER},
    {        "SwitchDial_st",   9,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {       "SwitchDial_stL",   9,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {      "SwitchDial_LtoR",   9,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {      "SwitchDial_push",   9,   4,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "TruckLtoR",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {     "TruckFrontToBack",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {          "TruckJumpSt",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "TruckJump",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {         "TruckJumpEnd",  12,   5,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,              PLAYER_FACE_WA},
    {           "TruckBrake",  12,   5,               PLAYER_FACE_PAIN,            PLAYER_FACE_DEFAULT,            PLAYER_FACE_PAIN},
    {              "TruckIn",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "TruckOut",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "DieTruck",  13,  56,           PLAYER_FACE_DIETRUCK,           PLAYER_FACE_DIETRUCK,        PLAYER_FACE_DIETRUCK},
    {       "DieTruckRevive",  13,  56,              PLAYER_FACE_CLOSE,            PLAYER_FACE_DEFAULT,           PLAYER_FACE_CLOSE},
    {              "MgDigIn",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "MgDigOut",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "MgLieF_L",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {             "MgLieF_R",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_DEFAULT},
    {            "MgAttackL",   7,   0,                PLAYER_FACE_CUT,            PLAYER_FACE_DEFAULT,             PLAYER_FACE_CUT},
    {            "MgAttackR",   7,   0,                PLAYER_FACE_CUT,            PLAYER_FACE_DEFAULT,             PLAYER_FACE_CUT},
    {              "MgDashL",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {              "MgDashR",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {             "MgDamage",   7,   0,             PLAYER_FACE_DAMAGE,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DAMAGE},
    {         "MgWaitBreath",   7,   0,             PLAYER_FACE_BREATH,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_BREATH},
    {              "MgLieUp",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_M},
    {            "LiftRemly",  13,   6,              PLAYER_FACEMABA01,       PLAYER_FACE_GETAWAITNICE,    PLAYER_FACE_GETAWAITNICE},
    {         "ThrowRemlyUp",  13,   6,              PLAYER_FACEMABA01,       PLAYER_FACE_GETAWAITNICE,    PLAYER_FACE_GETAWAITNICE},
    {        "LiftRemlyUtoM",  13,   6,              PLAYER_FACEMABA01,       PLAYER_FACE_GETAWAITNICE,    PLAYER_FACE_GETAWAITNICE},
    {   "Bird_FestivalStart",   9,   2, PLAYER_FACE_BIRD_FESTIVALSTART, PLAYER_FACE_BIRD_FESTIVALSTART, PLAYER_FACE_BIRD_FESTIVALSTART},
    {          "GetSurprise",  13,   6,        PLAYER_FACE_GETSURPRISE,        PLAYER_FACE_GETSURPRISE,      PLAYER_FACE_GETSURPRISE},
    {       "GirahimGrapple",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {          "GirahimPull",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {        "GirahimPullUp",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {        "GirahimChange",  13, 100,                 PLAYER_FACE_WA,            PLAYER_FACE_DEFAULT,               PLAYER_FACE_WA},
    {             "RopeWalk",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {             "RopeBend",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {         "RopeHangBend",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {           "RopeHangUp",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {          "RopeBalance", 255, 255,                 PLAYER_FACE_WA,                PLAYER_FACE_MAX,               PLAYER_FACE_WA},
    {          "SetSekibanA",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {          "SetSekibanB",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {          "SetSekibanC",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {            "pushTSWst",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {              "pushTSW",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {              "PushLog",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {        "AsuraCutRight",  12,   5,           PLAYER_FACE_ASURACUT,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_ASURACUT},
    {         "AsuraCutLeft",  12,   5,           PLAYER_FACE_ASURACUT,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_ASURACUT},
    {           "AsuraCutUp",  12,   5,           PLAYER_FACE_ASURACUT,            PLAYER_FACE_DEFAULT,         PLAYER_FACE_ASURACUT},
    {      "AsuraCutRightSt",  12,   5,         PLAYER_FACE_ASURACUTST,            PLAYER_FACE_DEFAULT,       PLAYER_FACE_ASURACUTST},
    {       "AsuraCutLeftSt",  12,   5,         PLAYER_FACE_ASURACUTST,            PLAYER_FACE_DEFAULT,       PLAYER_FACE_ASURACUTST},
    {         "AsuraCutUpSt",  12,   5,         PLAYER_FACE_ASURACUTST,            PLAYER_FACE_DEFAULT,       PLAYER_FACE_ASURACUTST},
    {      "AsuraCutRightEd",  12,   5,         PLAYER_FACE_ASURACUTED,            PLAYER_FACE_DEFAULT,       PLAYER_FACE_ASURACUTED},
    {       "AsuraCutLeftEd",  12,   5,         PLAYER_FACE_ASURACUTED,            PLAYER_FACE_DEFAULT,       PLAYER_FACE_ASURACUTED},
    {         "AsuraCutUpEd",  12,   5,         PLAYER_FACE_ASURACUTED,            PLAYER_FACE_DEFAULT,       PLAYER_FACE_ASURACUTED},
    {       "AsuraSwordWait",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {       "AsuraSwordLift",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {             "BinScoop",  13,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {                 "Fire",   9,   2,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {               "crouch",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {           "CannonWait",  13,   6,         PLAYER_FACE_CANNONWAIT,         PLAYER_FACE_CANNONWAIT,       PLAYER_FACE_CANNONWAIT},
    {                 "Seal",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {             "SealLoop",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {           "SealFinish",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {             "FinishEd",  13, 100,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {              "SitDown",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {              "SitWait",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {      "SitWaitServiceA",  13,   6,    PLAYER_FACE_SITWAITSERVICEA,    PLAYER_FACE_SITWAITSERVICEA,  PLAYER_FACE_SITWAITSERVICEA},
    {             "roofwait",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {             "roofhang",  12,   5,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {           "MgStruggle",   9,  52,               PLAYER_FACE_PAIN,            PLAYER_FACE_DEFAULT,             PLAYER_FACE_PAIN},
    {       "MgStruggle_act",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {    "MgStruggle_Escape",   9,  52,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,                PLAYER_FACE_M},
    {     "KrakenAppearance",   9,   2,   PLAYER_FACE_KRAKENAPPEARANCE,   PLAYER_FACE_KRAKENAPPEARANCE, PLAYER_FACE_KRAKENAPPEARANCE},
    {               "MgBury",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {            "GetFlower",   7,   0,          PLAYER_FACE_GETFLOWER,          PLAYER_FACE_GETFLOWER,        PLAYER_FACE_GETFLOWER},
    {        "DoorLargeOpen",  13,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {       "DoorLargeClose",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {       "HarpPlayFinish",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {           "LightningL",  13,   4,          PLAYER_FACE_LIGHTNING,          PLAYER_FACE_LIGHTNING,        PLAYER_FACE_LIGHTNING},
    {           "LightningR",  13,   4,          PLAYER_FACE_LIGHTNING,          PLAYER_FACE_LIGHTNING,        PLAYER_FACE_LIGHTNING},
    {           "LastAttack",   8, 100,         PLAYER_FACE_LASTATTACK,         PLAYER_FACE_LASTATTACK,       PLAYER_FACE_LASTATTACK},
    {                   "at", 255, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {                 "take", 255, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    {                "takeL", 255, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    {                "takeR", 255, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    {                "Take2", 255, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    {             "waitAtoS",   9,   1,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {                 "damD", 255,  56,             PLAYER_FACE_DAMAGE,                PLAYER_FACE_MAX,           PLAYER_FACE_DAMAGE},
    {           "Ashootwait",  13,   1,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {              "Arelord",   8,   3,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {               "Ashoot",   8,   3,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {          "ArelordTame",   8,   3,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {           "Ashootwait",  13,   1,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {              "Arelord",   8,   3,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {               "Ashoot",   8,   3,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {                "damSw",  13,   6,             PLAYER_FACE_DAMAGE,                PLAYER_FACE_MAX,           PLAYER_FACE_DAMAGE},
    {               "HSwait",   8,   1,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {           "VacuumWait",  12,   5,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    {          "MPPoseFront",  13, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {         "MPPoseAround",  13, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    {     "MPPoseAroundBack",  13, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {      "MPPoseDashFront",  13, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {     "MPPoseDashAround",  13, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    { "MPPoseDashAroundBack",  13, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {           "LiftB_UtoD",   7,   0,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {           "LiftS_UtoD",   7,   0,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    {            "LiftLWalk",   7,   0,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,                PLAYER_FACE_M},
    {          "MPGuardPose", 255, 255,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    {             "RockRide", 255,  56,                 PLAYER_FACE_WA,                PLAYER_FACE_MAX,               PLAYER_FACE_WA},
    {           "BinDrinkSt", 255,   5,            PLAYER_FACE_DRINKST,            PLAYER_FACE_DRINKST,          PLAYER_FACE_DRINKST},
    {             "BinDrink", 255,   5,              PLAYER_FACE_DRINK,                PLAYER_FACE_MAX,            PLAYER_FACE_DRINK},
    {           "BinDrinkEd", 255,   5,            PLAYER_FACE_DRINKED,                PLAYER_FACE_MAX,          PLAYER_FACE_DRINKED},
    {             "HarpPose",   7,   6,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {           "HarpPlayGo",   7,   1,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {         "HarpPlayCome",   7,   0,              PLAYER_FACEMABA01,            PLAYER_FACE_DEFAULT,          PLAYER_FACE_DEFAULT},
    {              "BinHold", 255,   5,                PLAYER_FACE_MAX,                PLAYER_FACE_MAX,              PLAYER_FACE_MAX},
    {            "BinShield",   9,   5,          PLAYER_FACE_BINSHIELD,          PLAYER_FACE_BINSHIELD,        PLAYER_FACE_BINSHIELD},
};
// clang-format on
#pragma pop

const s32 daPlayerModelBase_c::sUnkDuration = 0x2400;

const u8 daPlayerModelBase_c::sShieldDurabilities[10] = {16, 24, 32, 24, 36, 48, 12, 16, 20, 48};
const u8 daPlayerModelBase_c::sShieldRegenTimes[10] = {0, 0, 0, 0, 0, 0, 90, 90, 90, 0};

/**
 * Watch the player's hand material indices and change the ResMat
 * so that the right hand mats are visible and all others are hidden.
 */
void daPlBaseScnObjCallback_c::ExecCallback_DRAW_OPA(
    nw4r::g3d::ScnObj::Timing timing, nw4r::g3d::ScnObj *pObj, u32 param, void *pInfo
) {
    if (timing != nw4r::g3d::ScnObj::CALLBACK_TIMING_C) {
        nw4r::g3d::ScnMdl *pMdl = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdl>(pObj);
        nw4r::g3d::ResMdl mdl = pMdl->GetResMdl();
        nw4r::g3d::ResShp shp(nullptr);
        for (int i = 0; i < 2; i++) {
            u8 defaultMat = sSavedHandMats[i];
            // TODO: Is there a way to avoid this cast?
            u8 playerHandMat = static_cast<daPlayerModelBase_c *>(mpPlayer)->getHandMat(i);
            if (playerHandMat != defaultMat) {
                if (defaultMat != 0xFE) {
                    shp = mdl.GetResShp(sHandMats[defaultMat]);
                    shp.SetVisibility(false);
                    shp.DCStore(false);
                }
                if (playerHandMat != 0xFE) {
                    shp = mdl.GetResShp(sHandMats[playerHandMat]);
                    shp.SetVisibility(true);
                    shp.DCStore(false);
                }
                sSavedHandMats[i] = playerHandMat;
            }
        }
    }
}

void daPlBaseScnObjCallback_c::ExecCallback_CALC_MAT(
    nw4r::g3d::ScnObj::Timing timing, nw4r::g3d::ScnObj *pObj, u32 param, void *pInfo
) {
    if (timing != nw4r::g3d::ScnObj::CALLBACK_TIMING_C) {
        return;
    }
    dScnCallback_c::ExecCallback_CALC_MAT(timing, pObj, param, pInfo);
}

void daPlBaseMdlCallback_c::ExecCallbackA(
    nw4r::g3d::ChrAnmResult *result, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *calcWorld
) {
    u16 nodeId = calcWorld->GetCallbackNodeID();
    // Only run the full callback for certain specified nodes - but under which conditions is
    // mNodeIdMin == daPlayerModelBase_c::PLAYER_MAIN_NODE_WAIST?
    if ((nodeId >= mNodeIdMin && nodeId < mNodeIdMax) || (mNodeIdMin == daPlayerModelBase_c::PLAYER_MAIN_NODE_WAIST &&
                                                          nodeId == daPlayerModelBase_c::PLAYER_MAIN_NODE_CENTER)) {
        m3d::mdl_c::mdlCallback_c::ExecCallbackA(result, mdl, calcWorld);
        return;
    }
    // Otherwise only save the result
    mpNodes[nodeId] = *result;
    if (mpBaseCallback != nullptr) {
        mpBaseCallback->timingA(nodeId, result, mdl);
    }
}

bool daPlBaseMdl_c::create(
    daPlayerModelBase_c *player, nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption, u32 nView, u32 *pSize
) {
    if (!m3d::mdl_c::create(mdl, &mCallback, alloc, bufferOption, nView, pSize)) {
        return false;
    }

    setBlendNodeRange(0, 0xFFFF, 0.0f);
    field_0x58 = 0;
    mpSoundData = nullptr;
    field_0x5A = 0xFFFF;
    dSoundSourceIf_c *sound = player->getSoundSource();
    if (sound->hasAnimSound()) {
        sound->load(nullptr, "");
    }
    return true;
}

bool daPlBaseMdl_c::setAnm(m3d::banm_c &anm) {
    return m3d::bmdl_c::setAnm(anm);
}

void daPlBaseMdl_c::remove() {
    mCallback.remove();
    m3d::mdl_c::remove();
}

void daPlBaseMdl_c::setBlendNodeRange(u16 nodeIdMin, u16 nodeIdMax, f32 blendFrame) {
    mCallback.setBlendFrame(blendFrame + 1.0f);
    mCallback.setNodeIds(nodeIdMin, nodeIdMax);
    mCallback.calcBlend();
}

void daPlBaseMainCallback_c::timingA(u32 nodeId, nw4r::g3d::ChrAnmResult *result, nw4r::g3d::ResMdl mdl) {
    mpPlayer->mainModelTimingA(nodeId, result);
}

void daPlBaseMainCallback_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result, nw4r::g3d::ResMdl mdl) {
    mpPlayer->mainModelTimingB(nodeId, result);
}

void daPlBaseMainCallback_c::timingC(nw4r::math::MTX34 *result, nw4r::g3d::ResMdl mdl) {
    mpPlayer->mainModelTimingC(result);
}

void daPlBaseAnmChr_c::play() {
    m3d::fanm_c::play();
    field_0x36 = 0;
}

bool daPlBaseAnmChr_c::isFinished() {
    if (field_0x35 && !field_0x36) {
        return true;
    }

    if (isStop() || nw4r::math::FAbs(getRate()) < 0.001f) {
        field_0x35 = 1;
        field_0x36 = 1;
    }
    return false;
}

void daPlBaseHeadCallback_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result, nw4r::g3d::ResMdl) {
    mpPlayer->headModelTimingB(nodeId, result);
}

void daPlBaseHandsCallback_c::ExecCallbackC(
    nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *pFuncObj
) {
    mpPlayer->handsCallbackC(pMtxArray, mdl, pFuncObj);
}

// TODO explain
bool daPlayerModelBase_c::isBodyAnmPart_0_2_4(s32 part) {
    return part == 0 || part == 2 || part == 4;
}

void daPlayerModelBase_c::freeFrmHeap(mHeapAllocator_c *allocator) {
    EGG::Heap::toFrmHeap(allocator->getHeap())->free(0x1 | 0x2);
}

void daPlayerModelBase_c::allocFrmHeap(mHeapAllocator_c *allocator, u32 size, const char *name) {
    allocator->createFrmHeapToCurrent(size, dHeap::work1Heap.heap, name, 0x20, mHeap::OPT_NONE);
    new u8[size]();
    allocator->adjustFrmHeapRestoreCurrent();
}

void daPlayerModelBase_c::allocExternalDataBuffers() {
    mpExternalAnmCharBuffer = mAllocator.alloc(0x2400 * 6);
    mpAnmCharBuffer = mAllocator.alloc(0x1000);
    mpTexPatBuffer = mAllocator.alloc(0x1000);
    mpTexSrtBuffer = mAllocator.alloc(0x1000);
}

void daPlayerModelBase_c::initModelHeaps() {
    allocFrmHeap(&mModelAllocator, 0x14000, "LinkClothes");
    allocFrmHeap(&mSwordAllocator, 0xE000, "LinkSword");
    allocFrmHeap(&mShieldAllocator, 0x1C100, "LinkShield");
}

void daPlayerModelBase_c::updateSwordShieldModelsIfNeeded() {
    if (mModelUpdateFlags & UPDATE_MODEL_SWORD) {
        updateSwordModel();
    }
    FileManager *f = FileManager::GetInstance();
    if (mModelUpdateFlags & UPDATE_MODEL_SHIELD) {
        f->setShieldPouchSlot(mShieldPouchSlot);
        updateShieldModel();
    }
}

m3d::anmTexSrt_c *daPlayerModelBase_c::createAnmTexSrt(const char *resName, m3d::bmdl_c &mdl) {
    m3d::anmTexSrt_c *anm = new m3d::anmTexSrt_c();
    (void)mAlink2Res.GetResAnmTexSrt(resName);
    anm->create(mdl.getResMdl(), mAlink2Res.GetResAnmTexSrt(resName), &mAllocator, nullptr, 1);
    mdl.setAnm(*anm);
    return anm;
}

void daPlayerModelBase_c::updateEarringsColor() {
    if (mTunicType != TUNIC_SILENT) {
        mColor color = getEarringsColor();
        mFaceMdl.setTevKColor(mFaceMdl.getMatID("al_face_m"), GX_KCOLOR1, color, false);
    }
}

static const char *sBodyMdls[] = {"al", "pl_body", "Sl"};
static const char *sHeadMdls[] = {"al_head", "pl_head", "Sl_head"};
static const char *sFaceMdls[] = {"al_face", "al_face", "Sl_face"};
static const char *sHandMdls[] = {"al_hands", "pl_hands", "Sl_hands"};

void daPlayerModelBase_c::loadBodyModels() {
    mQuat1.set(1.0f, 0.0, 0.0f, 0.0f);
    mQuat2.set(1.0f, 0.0, 0.0f, 0.0f);
    freeFrmHeap(&mModelAllocator);

    nw4r::g3d::ResMdl bodyMdl = mAlink2Res.GetResMdl(sBodyMdls[mTunicType]);
    mMainMdl.create(this, bodyMdl, &mModelAllocator, 0x930, 1, nullptr);
    mMainMdl.setPriorityDraw(0x82, 0x7F);
    bodyMdl.ref().info.envelope_mtx_mode = nw4r::g3d::ResMdlInfoDataTypedef::EVPMATRIXMODE_APPROX;

    nw4r::g3d::ResMdl headMdl = mAlink2Res.GetResMdl(sHeadMdls[mTunicType]);
    createGenericMdl(headMdl, mHeadMdl, &mModelAllocator, 0x800);

    nw4r::g3d::ResMdl faceMdl = mAlink2Res.GetResMdl(sFaceMdls[mTunicType]);
    createGenericSmdl(faceMdl, mFaceMdl, &mModelAllocator, 0x807);

    mMainMdl.setCallback(&mMainBodyCallback);
    mMainBodyCallback.setPlayer(this);
    mHeadMdl.setCallback(&mHeadCallback);
    mHeadCallback.setPlayer(this);

    mScnCallback1.attach(mMainMdl);
    mScnCallback2.attach(mHeadMdl);

    nw4r::g3d::ResMdl handMdl = mAlink2Res.GetResMdl(sHandMdls[mTunicType]);
    createGenericSmdl(handMdl, mHandsMdl, &mModelAllocator, 0x800);
    nw4r::g3d::ScnMdlSimple *s = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mHandsMdl.getG3dObject());
    s->SetScnMdlCallback(&mCalcWorldCallback);
    s->EnableScnMdlCallbackTiming(nw4r::g3d::ScnObj::CALLBACK_TIMING_C);
    mCalcWorldCallback.setPlayer(this);
    mHandsMdl.setCallback(&mScnObjCallback);
    // TODO args types
    mHandsMdl.enableCallbackTiming(
        nw4r::g3d::ScnObj::Timing(nw4r::g3d::ScnObj::CALLBACK_TIMING_A | nw4r::g3d::ScnObj::CALLBACK_TIMING_C)
    );
    mHandsMdl.enableCallbackOp(
        nw4r::g3d::ScnObj::ExecOp(nw4r::g3d::ScnObj::EXECOP_DRAW_OPA | nw4r::g3d::ScnObj::EXECOP_CALC_MAT)
    );
    loadHandsModels();

    mBeltMatId = mMainMdl.getMatID("al_belt_upper_m");
    mSkinMatId = mMainMdl.getMatID("al_skin_m");
    mPouchMatId = mMainMdl.getMatID("al_pouch_m");
    mUpbodyMatId = mMainMdl.getMatID("al_upbody_m");
    mUpotherMatId = mMainMdl.getMatID("al_upother_m");
    mBelt2MatId = mBeltMatId;
    mLowbodyMapId = mMainMdl.getMatID("al_lowbody_m");
    mLowotherMapId = mMainMdl.getMatID("al_lowother_m");
    mPouch2MatId = mPouchMatId;
    mCapMatId = mHeadMdl.getMatID("al_cap_m");

    offSwordAndMoreStates(NO_SHIETH | POUCH | FIRST_PERSON);

    if (mPouchMatId >= 0) {
        mMainMdl.setCullMode(mPouchMatId, GX_CULL_BACK, false);
    }
    mMainMdl.setCullMode(mBeltMatId, GX_CULL_BACK, false);
    mMainMdl.setCullMode(mSkinMatId, GX_CULL_BACK, false);
    updateEarringsColor();
}

void daPlayerModelBase_c::loadBody() {
    loadBodyModels();
    nw4r::g3d::ResMdl bodyMdl = mMainMdl.getResMdl();
    mAnmChrBlend.create(bodyMdl, 6, &mAllocator);
    mAnmChrBlend.getAnimObj()->SetAnmFlag(nw4r::g3d::AnmObj::FLAG_USE_QUATERNION_ROTATION_BLEND, true);

    daPlBaseAnmChr_c *anms = mAnmChrs;
    // "waitS"
    nw4r::g3d::ResAnmChr resAnmChr26 = getExternalAnmChr(sAnimations[26].animName, mpExternalAnmCharBuffer, 0x2400);
    nw4r::g3d::AnmObjChr *animObj;
    for (s32 i = 0; i < 6; i++) {
        anms->create2(bodyMdl, resAnmChr26, &mAllocator);
        anms->setAnm(mMainMdl, resAnmChr26, m3d::PLAY_MODE_0);
        f32 f;
        switch (i) {
            case 0:
            case 1:  f = 1.0f; break;
            default: f = 0.0f; break;
        }
        animObj = static_cast<nw4r::g3d::AnmObjChr *>(anms->getAnimObj());
        animObj->Release();
        if (isBodyAnmPart_0_2_4(i)) {
            animObj->Bind(bodyMdl, PLAYER_MAIN_NODE_WAIST, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
            animObj->Bind(bodyMdl, PLAYER_MAIN_NODE_CENTER, nw4r::g3d::AnmObjChr::BIND_ONE);
        } else {
            animObj->Bind(bodyMdl, PLAYER_MAIN_NODE_BACKBONE_1, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
        }
        mAnmChrBlend.attach(i, anms, f);
        anms++;
    }

    mMainMdl.setAnm(mAnmChrBlend);

    // "F_default"
    mFaceAnmChr.create(
        mFaceMdl.getResMdl(), getExternalAnmChr(sFaceResNames[0], mpAnmCharBuffer, 0x1000), &mAllocator, nullptr
    );
    mFaceMdl.setAnm(mFaceAnmChr);
    mFaceAnmChrIdx1 = PLAYER_FACE_DEFAULT;
    mFaceAnmChrIdx2 = PLAYER_FACE_MAX;

    // "Fmaba01"
    mFaceTexPat.create(
        mFaceMdl.getResMdl(), getExternalAnmTexPat(sFaceResNames[1], mpTexPatBuffer, 0x1000), &mAllocator, nullptr, 1
    );
    mFaceMdl.setAnm(mFaceTexPat);
    mFaceAnmTexPatIdx1 = PLAYER_FACEMABA01;
    mFaceAnmTexPatIdx2 = PLAYER_FACE_MAX;

    bool isInTrial = dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL;
    mFaceTexSrt.create(
        mFaceMdl.getResMdl(), getExternalAnmTexSrt(sFaceResNames[0], mpTexSrtBuffer, 0x1000), &mAllocator, nullptr,
        isInTrial ? 2 : 1
    );
    mFaceMdl.setAnm(mFaceTexSrt);
    mFaceAnmTexSrtIdx1 = PLAYER_FACE_DEFAULT;
    mFaceAnmTexSrtIdx2 = PLAYER_FACE_MAX;

    // no allocator arg?
    mHeadAnmChr.create(mHeadMdl.getResMdl(), mPlCommonAnimeRes.GetResAnmChr("H_Default"), nullptr, nullptr);
    mHeadMdl.setAnm(mHeadAnmChr);
}

void daPlayerModelBase_c::loadHandsModels() {
    nw4r::g3d::ResMdl mdl = mHandsMdl.getResMdl();
    SizedString<16> handName("al_handRA_m");
    s32 ids[14];
    // Hand mats: al_handRA_m - al_handRG_m
    //            al_handLA_m - al_handLG_m
    for (s32 i = 0; i < 14; i++) {
        if (i < 7) {
            handName.mChars[8] = i + 'A';
        } else {
            if (i == 7) {
                handName.mChars[7] = 'L';
            }
            handName.mChars[8] = i + ('A' - 7);
        }
        ids[i] = m3d::getMatID(mdl, handName);
        sHandMats[i] = 0xFF;
    }

    nw4r::g3d::ResShp shp(nullptr);

    sSavedHandMats[0] = 7;
    sSavedHandMats[1] = 0;

    nw4r::g3d::ResMdl::DrawEnumerator drawEnumerator = mdl.ConstructDrawEnumerator();
    while (drawEnumerator.IsValid()) {
        for (int j = 0; j < 14; j++) {
            if (ids[j] == drawEnumerator.GetMatID()) {
                sHandMats[j] = drawEnumerator.GetShpID();
                break;
            }
        }

        drawEnumerator.MoveNext();
    }

    for (int i = 0; i < mdl.GetResShpNumEntries(); i++) {
        shp = mdl.GetResShp(i);
        bool visible = sHandMats[sSavedHandMats[0]] == i || sHandMats[sSavedHandMats[1]] == i;
        if (visible) {
            shp.SetVisibility(true);
        } else {
            shp.SetVisibility(false);
        }
        shp.DCStore(false);
    }
}

void daPlayerModelBase_c::setSwordAnm(const char *name) {
    nw4r::g3d::ResAnmClr clr = mAlinkRes.GetResAnmClr(name);
    mSwordAnmMatClr.setAnm(mSwordMdl, clr, 0, m3d::PLAY_MODE_4);
    mSwordMdl.setAnm(mSwordAnmMatClr);
}
static const f32 sSwordRelatedX[] = {9.0f, 12.0, 12.0, 12.0, 12.0, 12.0, 9.0f};
static const f32 sSwordRelatedUnk[] = {73.0f, 80.0f, 100.0f, 100.0f, 120.0f, 120.0f, 73.0f};
static const f32 sSwordRelatedZ[] = {0.7f, 0.7f, 0.9f, 0.9f, 1.0f, 1.0f, 0.7f};
static const f32 sSwordRelatedY[] = {0.7f, 0.8f, 0.9f, 0.9f, 1.0f, 1.0f, 0.7f};

void daPlayerModelBase_c::initSwordModel() {
    static const char *sSwordPods[] = {"EquipPodA", "EquipPodB",      "EquipPodC", "EquipPodD",
                                       "EquipPodE", "EquipPodMaster", "EquipPodA"};

    updateCurrentSword();
    freeFrmHeap(&mSwordAllocator);
    const char *swordName = getSwordName(sCurrentSword);
    mSwordRes = getItemResFile(swordName, mSwordAllocator);
    field_0x1278 = sSwordRelatedUnk[sCurrentSword];
    field_0x127C.x = sSwordRelatedX[sCurrentSword];
    field_0x127C.y = sSwordRelatedY[sCurrentSword];
    field_0x127C.z = sSwordRelatedZ[sCurrentSword];

    nw4r::g3d::ResMdl sheathMdl = mSwordRes.GetResMdl(sSwordPods[sCurrentSword]);
    createGenericSmdl(sheathMdl, mSheathMdl, &mSwordAllocator, 0x800);
    mScnCallback4.attach(mSheathMdl);

    nw4r::g3d::ResMdl swordMdl = mSwordRes.GetResMdl(swordName);
    createGenericSmdl(swordMdl, mSwordMdl, &mSwordAllocator, 0x810);
    mSwordMdl.setPriorityDraw(0x84, 0x7F);
    nw4r::g3d::ResAnmClr swordAnmClr = mAlinkRes.GetResAnmClr("appearance");
    nw4r::g3d::ResMdl swordResMdl = mSwordMdl.getResMdl();
    mSwordAnmMatClr.create(swordResMdl, swordAnmClr, &mSwordAllocator, nullptr, 1);
    mSwordMdl.setAnm(mSwordAnmMatClr);
    mScnCallback3.attach(mSwordMdl);
    offFlags_0x340(1);
    offSwordAndMoreStates(SWORD_AWAY);

    // TODO ids
    if (sCurrentSword == 0) {
        mSkywardStrikeChargeDuration = 82;
    } else if (sCurrentSword == 5) {
        if (dScGame_c::isHeroMode()) {
            mSkywardStrikeChargeDuration = 10;
        } else {
            mSkywardStrikeChargeDuration = 40;
        }
    } else {
        if (dScGame_c::isHeroMode()) {
            field_0x127C.z = 1.0f;
            mSkywardStrikeChargeDuration = 40;
        } else {
            mSkywardStrikeChargeDuration = 55;
        }
    }
}

void daPlayerModelBase_c::updateSwordModel() {
    mMtx_c sheathMtx;
    mMtx_c swordMtx;
    mSheathMdl.getLocalMtx(sheathMtx);
    mSwordMdl.getLocalMtx(swordMtx);

    mSwordAnmMatClr.remove();
    mSheathMdl.remove();
    mSwordMdl.remove();

    initSwordModel();

    setTransformAndCalc(mSheathMdl, &sheathMtx);
    setTransformAndCalc(mSwordMdl, &swordMtx);
    mModelUpdateFlags &= ~UPDATE_MODEL_SWORD;
}

u32 daPlayerModelBase_c::getCurrentShieldPouchSlot() {
    // Compiler quirk, inline function not inlined due to GetLink call
    if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL || dAcPy_c::GetLink()->isInBambooCuttingMinigame()) {
        return POUCH_SLOT_NONE;
    }
    return FileManager::GetInstance()->getShieldPouchSlot();
}

s32 daPlayerModelBase_c::getCurrentlyEquippedShieldId() {
    s32 id;
    u32 slot = getCurrentShieldPouchSlot();
    if (slot < POUCH_SLOT_NONE) {
        id = getShieldType(FileManager::GetInstance()->getPouchItem(slot));
    } else {
        id = 10;
    }
    if (id >= 11) {
        id = 10;
    }
    return id;
}

s32 daPlayerModelBase_c::getShieldType(s32 item) {
    if (!isItemShield(item)) {
        item = ITEM_WOODEN_SHIELD;
    }
    return item - ITEM_WOODEN_SHIELD;
}

static const char *sShieldModelsBase[] = {
    "EquipShieldWood", "EquipShieldIron", "EquipShieldHoly", "EquipShieldHylia", "EquipShieldWood",
};

static const char *sShieldModelsBroken[] = {
    "EquipShieldWoodBroken", "EquipShieldIronBroken", "EquipShieldHolyBroken",
    "EquipShieldHylia",      "EquipShieldWoodBroken",
};

void daPlayerModelBase_c::initShieldModel() {
    static const u16 shieldEffects[] = {
        PARTICLE_RESOURCE_ID_MAPPING_537_, PARTICLE_RESOURCE_ID_MAPPING_535_, PARTICLE_RESOURCE_ID_MAPPING_529_,
        PARTICLE_RESOURCE_ID_MAPPING_529_, PARTICLE_RESOURCE_ID_MAPPING_529_,
    };
    freeFrmHeap(&mShieldAllocator);
    s32 shieldType = getCurrentlyEquippedShieldType();
    s32 shieldId = getCurrentlyEquippedShieldId();
    nw4r::g3d::ResFile shieldRes = getItemResFile(sShieldModelsBase[shieldType], mShieldAllocator);
    createGenericSmdl(shieldRes.GetResMdl(sShieldModelsBase[shieldType]), mShieldMdl, &mShieldAllocator, 0x903);
    mScnCallback5.attach(mShieldMdl);
    if (shieldType != /* HYLIA */ 3) {
        mShieldAnmTexPat1.create(
            mShieldMdl.getResMdl(), shieldRes.GetResAnmTexPat(sShieldModelsBase[shieldType]), &mShieldAllocator,
            nullptr, 1
        );
        mShieldMdl.setAnm(mShieldAnmTexPat1);
        mShieldAnmTexPat1.setFrame(shieldId % 3, 0);

        createGenericSmdl(shieldRes.GetResMdl(sShieldModelsBroken[shieldType]), mUnkMdl1, &mShieldAllocator, 0x103);
        mAnmChr.create(
            mUnkMdl1.getResMdl(), shieldRes.GetResAnmChr(sShieldModelsBroken[shieldType]), &mShieldAllocator, nullptr
        );
        mUnkMdl1.setAnm(mAnmChr);
        mAnmChr.setFrameOnly(0.0f);

        mShieldAnmTexPat2.create(
            mUnkMdl1.getResMdl(), shieldRes.GetResAnmTexPat(sShieldModelsBase[shieldType]), &mShieldAllocator, nullptr,
            1
        );
        mShieldAnmTexPat2.setFrame(mShieldAnmTexPat1.getFrame(0), 0);
        mUnkMdl1.setAnm(mShieldAnmTexPat2);
    }
    mBreakingEffect = shieldEffects[shieldType];
    dLytMeter_c::setShieldId(shieldId);
    dLytMeter_c::setShieldMaxDurability(sShieldDurabilities[shieldId]);
    dAcPy_c::GetLink2()->onShieldUpdate();
}

void daPlayerModelBase_c::updateShieldModel() {
    mModelUpdateFlags &= ~(UPDATE_MODEL_SHIELD | 8); // TODO ???
    if (getCurrentShieldPouchSlot() != POUCH_SLOT_NONE) {
        mMtx_c mtx;
        mShieldMdl.getLocalMtx(mtx);
        mShieldAnmTexPat1.remove();
        mShieldAnmTexPat2.remove();
        mShieldMdl.remove();
        mAnmChr.remove();
        mUnkMdl1.remove();
        initShieldModel();
        setTransformAndCalc(mShieldMdl, &mtx);
    }
}

bool daPlayerModelBase_c::createGenericSmdl(
    nw4r::g3d::ResMdl resMdl, m3d::smdl_c &mdl, mAllocator_c *alloc, u32 bufferOption
) {
    bool ok = mdl.create(resMdl, alloc, bufferOption | 0x120, 1, nullptr);
    mdl.setPriorityDraw(0x82, 0x7F);
    return ok;
}

bool daPlayerModelBase_c::createGenericMdl(
    nw4r::g3d::ResMdl resMdl, m3d::mdl_c &mdl, mAllocator_c *alloc, u32 bufferOption
) {
    bool ok = mdl.create(resMdl, alloc, bufferOption | 0x120, 1, nullptr);
    mdl.setPriorityDraw(0x82, 0x7F);
    return ok;
}

void daPlayerModelBase_c::applyWorldRotationMaybe(
    nw4r::math::MTX34 *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order
) {
    mVec3_c v;
    v.x = result->_03;
    v.y = result->_13;
    v.z = result->_23;
    mMtx_c work;
    if (off != nullptr) {
        MTXTrans(work, off->x, off->y, off->z);
    } else {
        MTXTrans(work, v.x, v.y, v.z);
    }
    work.YrotM(mRotation.y);
    if (order) {
        work.ZYXrotM(x, y, z);
    } else {
        work.ZXYrotM(x, y, z);
    }
    work.YrotM(-mRotation.y);
    mMtx_c translateBack;
    translateBack.transS(-v.x, -v.y, -v.z);
    MTXConcat(work, translateBack, work);
    MTXConcat(work, *result, *result);
}

void daPlayerModelBase_c::applyWorldRotationMaybe(
    nw4r::g3d::WorldMtxManip *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order
) {
    mMtx_c mtx;
    result->GetMtx(mtx);
    applyWorldRotationMaybe(mtx, x, y, z, off, order);
    result->SetMtxUnchecked(mtx);
}

void daPlayerModelBase_c::adjustMainModelChrAnm(PlayerMainModelNode_e nodeId, nw4r::g3d::ChrAnmResult *result) {
    mMtx_c mtx;
    result->GetMtx(mtx);
    if (nodeId == PLAYER_MAIN_NODE_HEAD) {
        mtx.YrotM(-mHeadYRot);
        mtx.ZrotM(mHeadZRot);
        mtx.XrotM(mHeadXRot);
    } else if (nodeId == PLAYER_MAIN_NODE_WAIST) {
        mtx.ZrotM(mWaistZRot);
        mtx.YrotM(-mWaistYRot);
    } else if (nodeId == PLAYER_MAIN_NODE_CENTER) {
        transformModelCenter(&mtx);
    } else if (nodeId == PLAYER_MAIN_NODE_ARM_R1 || nodeId == PLAYER_MAIN_NODE_ARM_R2) {
        mtx.ZrotM(getArmZRot(1));
    } else if (nodeId == PLAYER_MAIN_NODE_ARM_L1 || nodeId == PLAYER_MAIN_NODE_ARM_L2) {
        mtx.ZrotM(getArmZRot(0));
    } else if (nodeId == PLAYER_MAIN_NODE_RSKIRT_L1 || nodeId == PLAYER_MAIN_NODE_RSKIRT_L2) {
        if (getRidingActorType() == RIDING_LOFTWING) {
            if (nodeId == PLAYER_MAIN_NODE_RSKIRT_L1) {
                mtx.ZrotM(mRSkirtL1Rot);
            } else {
                mtx.ZrotM(3000);
            }
        }
    } else if (nodeId == PLAYER_MAIN_NODE_RSKIRT_R1 || nodeId == PLAYER_MAIN_NODE_RSKIRT_R2) {
        if (getRidingActorType() == RIDING_LOFTWING) {
            if (nodeId == PLAYER_MAIN_NODE_RSKIRT_R1) {
                mtx.ZrotM(mRSkirtR1Rot);
            } else {
                mtx.ZrotM(3000);
            }
        }
    } else if (mRightHandRotation != 0) {
        if (nodeId == PLAYER_MAIN_NODE_HAND_R) {
            static const mVec3_c v1(1.0f, 1.0f, 0.0f);
            mMtx_c rotMtx;
            rotMtx.setAxisRotation(v1, mRightHandRotation.radian2());
            mtx += rotMtx;
        }
    }

    result->SetMtx(mtx);
}

void daPlayerModelBase_c::adjustMainModelWorldMtx(PlayerMainModelNode_e nodeId, nw4r::g3d::WorldMtxManip *result) {
    if (checkCurrentAction(0xA9) && (nodeId == PLAYER_MAIN_NODE_ARM_R1 || nodeId == PLAYER_MAIN_NODE_ARM_R2)) {
        mMtx_c mtx;
        mtx.makeQ(mQuat1);
        mtx.YrotM(-mRotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(mRotation.y);
        MTXConcat(mtx2, mtx, mtx);

        mMtx_c orig;
        result->GetMtx(orig);
        mVec3_c origTrans;
        orig.getTranslation(origTrans);
        MTXConcat(mtx, orig, orig);
        orig.setTranslation(origTrans);
        if (nodeId == PLAYER_MAIN_NODE_ARM_R1) {
            applyWorldRotationMaybe(orig, 1500, 0, 0, nullptr, false);
        }
        result->SetMtxUnchecked(orig);
    } else if (mAnimations[3] == 0xE0 && nodeId == PLAYER_MAIN_NODE_ARM_R2) {
        mMtx_c mtx;
        mtx.makeQ(mQuat1);
        mtx.YrotM(-mRotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(mRotation.y);
        MTXConcat(mtx2, mtx, mtx);

        mMtx_c orig;
        result->GetMtx(orig);
        mVec3_c origTrans;
        orig.getTranslation(origTrans);
        MTXConcat(mtx, orig, orig);
        orig.setTranslation(origTrans);
        result->SetMtxUnchecked(orig);
    } else if (nodeId == PLAYER_MAIN_NODE_BACKBONE_1) {
        transformBackbone1(result);
        applyWorldRotationMaybe(
            result, field_0x1268 >> 1, field_0x126A * 0.4f + getArmZRot(2), field_0x126C, nullptr, false
        );
    } else if (nodeId == PLAYER_MAIN_NODE_BACKBONE_2) {
        applyWorldRotationMaybe(result, field_0x1268 >> 1, field_0x126A * 0.6f + getArmZRot(2), 0, nullptr, false);
    } else if (nodeId == PLAYER_MAIN_NODE_FSKIRT_L1 && *field_0x136C != 0) {
        mVec3_c v;
        vt_0x30C(v);
        if (*field_0x136C < 0) {
            v.z *= -1.0f;
        }
        applyWorldRotationMaybe(result, v.x * *field_0x136C, 0, v.z * *field_0x136C, nullptr, true);
    } else if (nodeId == PLAYER_MAIN_NODE_FSKIRT_R1 && *field_0x1370) {
        mVec3_c v;
        vt_0x30C(v);
        if (*field_0x1370 > 0) {
            v.z *= -1.0f;
        }
        applyWorldRotationMaybe(result, v.x * *field_0x1370, 0, v.z * *field_0x1370, nullptr, true);
    } else if (nodeId == PLAYER_MAIN_NODE_POD) {
        if (field_0x1258 != 0) {
            mMtx_c mtx;
            mtx.YrotS(field_0x125A);
            mtx.XrotM(field_0x1258);
            mtx.YrotM(-field_0x125A);
            mMtx_c orig;
            result->GetMtx(orig);
            mVec3_c origTrans;
            orig.getTranslation(origTrans);
            MTXConcat(mtx, orig, orig);
            orig.setTranslation(origTrans);
            result->SetMtxUnchecked(orig);
        }
        if (field_0x1268 < 0) {
            applyWorldRotationMaybe(result, -(field_0x1268 >> 1), 0, 0, nullptr, false);
        }
    } else if ((nodeId == PLAYER_MAIN_NODE_ARM_R2 || nodeId == PLAYER_MAIN_NODE_HAND_R) && isMPPose()) {
        mMtx_c mtx;
        mtx.makeQ(mQuat2);
        mtx.YrotM(-mRotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(mRotation.y);
        MTXConcat(mtx2, mtx, mtx);

        mMtx_c orig;
        result->GetMtx(orig);
        mVec3_c origTrans;
        orig.getTranslation(origTrans);
        MTXConcat(mtx, orig, orig);
        orig.setTranslation(origTrans);
        result->SetMtxUnchecked(orig);
    } else if (nodeId == PLAYER_MAIN_NODE_HAND_R || nodeId == PLAYER_MAIN_NODE_HAND_L) {
        if (isOnTightRope()) {
            mAng rot;
            if (nodeId == PLAYER_MAIN_NODE_HAND_R) {
                rot = -5461;
            } else {
                rot = -7282;
            }
            applyWorldRotationMaybe(result, rot, 0, 0, nullptr, false);
        } else if (isOnVines()) {
            // TODO what even is this
            mAng v1 = -3277;
            mAng v2 = 910;
            if (nodeId == PLAYER_MAIN_NODE_HAND_R) {
                v1 *= (s32)-1;
                v2 *= (s32)-1;
            }
            applyWorldRotationMaybe(result, 0xec17, v1, v2, nullptr, true);
        }
    }
}

void daPlayerModelBase_c::updateMainBlend1(f32 blend) {
    // TODO document numbers
    mAnmChrBlend.setWeight(1, (1.0f - blend) * (1.0f - field_0x1274));
    mAnmChrBlend.setWeight(3, (1.0f - blend) * field_0x1274);
    mAnmChrBlend.setWeight(5, blend);
}

void daPlayerModelBase_c::updateMainBlend2(f32 blend, bool save) {
    // TODO document numbers
    f32 blend4 = 1.0f - mAnmChrBlend.getWeight(4);
    mAnmChrBlend.setWeight(0, blend4 * (1.0f - blend));
    mAnmChrBlend.setWeight(2, blend4 * blend);
    f32 blend5 = 1.0f - mAnmChrBlend.getWeight(5);
    mAnmChrBlend.setWeight(1, blend5 * (1.0f - blend));
    mAnmChrBlend.setWeight(3, blend5 * blend);
    if (save) {
        field_0x1274 = blend;
    }
}

void daPlayerModelBase_c::updateBlendWeights(PlayerMainModelNode_e nodeId) {
    switch (field_0x1219) {
        case 9:
            if (nodeId == PLAYER_MAIN_NODE_CENTER) {
                updateMainBlend2(field_0x1274, false);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_R) {
                if (field_0x1274 < 0.5f) {
                    updateMainBlend2(0.0f, false);
                } else {
                    updateMainBlend2(1.0f, false);
                }
            }
            break;
        case 1:
            if (nodeId == PLAYER_MAIN_NODE_CENTER) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 4:
            if (nodeId == PLAYER_MAIN_NODE_CENTER) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_L) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 2:
            if (nodeId == PLAYER_MAIN_NODE_WEAPON_L) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_R) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 3:
            if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_L) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 5:
            if (nodeId == PLAYER_MAIN_NODE_HEAD) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 7:
            if (nodeId == PLAYER_MAIN_NODE_CENTER) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_HEAD) {
                updateMainBlend1(1.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_R) {
                updateMainBlend1(1.0f);
            }
            break;
        case 6:
            if (nodeId == PLAYER_MAIN_NODE_HEAD) {
                updateMainBlend2(1.0f, false);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend2(0.0f, false);
            }
            break;
        case 8:
            if (nodeId == PLAYER_MAIN_NODE_HEAD) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(field_0x1270);
            }
            break;
    }
}

void daPlayerModelBase_c::fn_8005F890(nw4r::math::MTX34 *result) {
    // Probably related to IK?
    static const u16 sNodeIdsArms1[] = {PLAYER_MAIN_NODE_ARM_L1, PLAYER_MAIN_NODE_ARM_R1};
    static const Vec sArmVecs[] = {
        {29.0f, 0.0f, 0.0f},
        {26.5f, 0.0f, 0.0f},
        { 0.0f, 0.0f, 0.0f}
    };

    nw4r::g3d::ResMdl mdl = mMainMdl.getResMdl();

    if (vt_0x2E8(result, sNodeIdsArms1, false)) {
        mVec3_c dstVec;
        mVec3_c swordOffset;
        mVec3_c translate;
        if (checkCurrentAction(/* SWORD_IN_DIAL */ 0x86) || checkCurrentAction(/* ON_BIRD */ 0x8A)) {
            swordOffset = vt_0x304() * 0.5f;
        } else {
            swordOffset = mVec3_c::Zero;
        }

        nw4r::math::MTX34 *targetMtx;
        for (s32 arm = 0; arm < 2; arm++) {
            mAng rot1;
            mAng rot2;
            // TODO maybe there's a more natural order
            s32 i;
            isOnClawTargetMaybe(arm, rot1, rot2);
            u16 armNode = sNodeIdsArms1[arm];
            mAng *angs;
            mVec3_c *dst = nullptr;
            if (arm == 0) {
                angs = field_0x1374;
            } else {
                angs = field_0x1378;
            }
            // Walk down the arm bones and transform the whole thing again?
            for (i = 0; i < 3; i++) {
                targetMtx = &result[mdl.GetResNode(armNode).GetMtxID()];
                applyWorldRotationMaybe(targetMtx, rot1, rot2, angs[i], dst, false);
                MTXMultVec(*targetMtx, mVec3_c(sArmVecs[i]), dstVec);
                if (i != 2 && arm == 1) {
                    // sword hand
                    dstVec -= swordOffset;
                }
                dst = &dstVec;
                armNode++;
            }
            mMainMdl.getNodeResult(armNode)->GetTranslate(translate);
            MTXMultVec(*targetMtx, translate, *dst);
            applyWorldRotationMaybe(&result[mdl.GetResNode(armNode).GetMtxID()], rot1, rot2, angs[2], &dstVec, false);
        }
    }
}

void daPlayerModelBase_c::fn_8005FB90(nw4r::math::MTX34 *result) {
    // Suffer from the same problem as above
    static const u16 sNodeIdsLegs1[] = {PLAYER_MAIN_NODE_LEG_L1, PLAYER_MAIN_NODE_LEG_R1};
    static const Vec sLegVecs[] = {
        {   30.0f, 0.0f, 0.0f},
        {39.3635f, 0.0f, 0.0f},
        {  14.18f, 0.0f, 0.0f}
    };

    nw4r::g3d::ResMdl mdl = mMainMdl.getResMdl();

    if (vt_0x2E8(result, sNodeIdsLegs1, true)) {
        mVec3_c dstVec;
        mVec3_c legOffset = vt_0x304() * 0.5f;

        for (s32 leg = 0; leg < 2; leg++) {
            // TODO maybe there's a more natural order
            mVec3_c *dst = nullptr;
            s32 i;
            u16 legNode = sNodeIdsLegs1[leg];
            mAng *angs;
            if (leg == 0) {
                angs = field_0x136C;
            } else {
                angs = field_0x1370;
            }
            // Walk down the leg bones and transform the whole thing again?
            for (i = 0; i < 3; i++) {
                u32 mtxId = mdl.GetResNode(legNode).GetMtxID();
                nw4r::math::MTX34 *targetMtx = &result[mtxId];
                applyWorldRotationMaybe(targetMtx, angs[i], 0, 0, dst, false);
                MTXMultVec(*targetMtx, mVec3_c(sLegVecs[i]), dstVec);
                if (i != 2) {
                    dstVec -= legOffset;
                }
                dst = &dstVec;
                legNode++;
            }
            applyWorldRotationMaybe(&result[mdl.GetResNode(legNode).GetMtxID()], angs[2], 0, 0, &dstVec, false);
        }
    }
}

void daPlayerModelBase_c::mainModelTimingA(u32 nodeId, nw4r::g3d::ChrAnmResult *result) {
    adjustMainModelChrAnm((PlayerMainModelNode_e)nodeId, result);
}

void daPlayerModelBase_c::mainModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result) {
    adjustMainModelWorldMtx((PlayerMainModelNode_e)nodeId, result);
    if (field_0x1219 != 0) {
        if (nodeId == PLAYER_MAIN_NODE_CENTER || nodeId == PLAYER_MAIN_NODE_POD ||
            nodeId == PLAYER_MAIN_NODE_WEAPON_L || nodeId == PLAYER_MAIN_NODE_HEAD ||
            nodeId == PLAYER_MAIN_NODE_WEAPON_R) {
            updateBlendWeights((PlayerMainModelNode_e)nodeId);
        }
    }
}

void daPlayerModelBase_c::mainModelTimingC(nw4r::math::MTX34 *result) {
    fn_8005FB90(result);
    fn_8005F890(result);
    mMainMdl.setField_0x58(1);
}

void daPlayerModelBase_c::handsCallbackC(
    nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *pFuncObj
) {
    mMainMdl.getNodeWorldMtx(PLAYER_MAIN_NODE_HAND_L, &pMtxArray[mdl.GetResNode(PLAYER_HANDS_NODE_HAND_L).GetMtxID()]);
    mMainMdl.getNodeWorldMtx(PLAYER_MAIN_NODE_HAND_R, &pMtxArray[mdl.GetResNode(PLAYER_HANDS_NODE_HAND_R).GetMtxID()]);
}

void daPlayerModelBase_c::headModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result) {
    if (nodeId >= PLAYER_HEAD_NODE_MOMI_Z_CAP_1) {
        mMtx_c mtx;
        result->GetMtx(mtx);
        mVec3_c v;
        mtx.getTranslation(v);
        mMtx_c rot;
        rot.makeQ(field_0x1300[nodeId - 6]);
        MTXConcat(rot, mtx, mtx);
        mtx.setTranslation(v);
        result->SetMtxUnchecked(mtx);
    } else if (nodeId >= PLAYER_HEAD_NODE_HAIR_L) {
        mAng oldYRot = mRotation.y;
        mRotation.y = field_0x1256;
        u32 idx = nodeId - 1;
        applyWorldRotationMaybe(result, field_0x1238[idx], 0, field_0x1242[idx], nullptr, false);
        mRotation.y = oldYRot;
    }
}

void daPlayerModelBase_c::setTransformAndCalc(m3d::scnLeaf_c &lf, const mMtx_c *mtx) {
    if (mtx != nullptr) {
        lf.setLocalMtx(*mtx);
    }
    lf.calc(false);
}

void daPlayerModelBase_c::setShieldTransform(bool inHand) {
    mMtx_c targetTransform;
    if (inHand) {
        mMainMdl.getNodeWorldMtx(PLAYER_MAIN_NODE_WEAPON_L, targetTransform);
    } else {
        mMainMdl.getNodeWorldMtx(PLAYER_MAIN_NODE_POD, targetTransform);
        mMtx_c mtx2;
        MTXTrans(mtx2, 4.2f, -4.4f, 20.0f);
        MTXConcat(targetTransform, mtx2, targetTransform);
        targetTransform.ZYXrotM(mAng::fromDeg(91.0f), mAng::fromDeg(-123.0f), 0);
    }
    setTransformAndCalc(mShieldMdl, &targetTransform);
}

void daPlayerModelBase_c::loadTwoAnmChrs(s32 childIdx, s32 animIdx, s32 animIdx2, bool b) {
    void *data = (u8 *)mpExternalAnmCharBuffer + childIdx * 0x2400;
    u32 size1;
    void *data2;
    u32 size2;

    s32 otherChildIdx = 3;

    if (childIdx == 0) {
        otherChildIdx = 1;
    }

    if (animIdx == animIdx2) {
        size1 = 0x2400 * 2;
        if (b) {
            size1 *= 2;
        }
        size2 = 0;
        data2 = data;
    } else {
        size1 = 0x2400;
        if (b) {
            size1 *= 2;
        }
        size2 = size1;
        data2 = (u8 *)data + size1;
    }
    loadAnmChr(childIdx, animIdx, data, size1);
    loadAnmChr(otherChildIdx, animIdx2, data2, size2);
}

nw4r::g3d::ResFile daPlayerModelBase_c::getItemResFile(const char *name, mAllocator_c &allocator) {
    SizedString<64> buf;
    buf.sprintf("dat/%s.brresC", name);
    void *dat = getOarcFile("PLCompItem", buf);
    u32 size = CXGetUncompressedSize(dat);
    void *decompressedBuf = allocator.alloc(size);
    lzSafeDecompress(dat, size, decompressedBuf);
    nw4r::g3d::ResFile file(decompressedBuf);
    file.Init();
    file.Bind();
    BindSystemModelsAndLighting(file);
    return file;
}

nw4r::g3d::ResFile
daPlayerModelBase_c::getExternalCompressedFile(const char *name, const char *extension, void *dest, u32 maxSize) {
    if (maxSize != 0) {
        SizedString<64> buf;
        buf.sprintf("%s.%s", name, extension);
        void *externalData = mPlCommonAnimeRes.GetExternalData(buf);
        lzSafeDecompress(externalData, maxSize, dest);
    }
    nw4r::g3d::ResFile file(dest);
    file.Init();
    return file;
}

nw4r::g3d::ResAnmChr daPlayerModelBase_c::getExternalAnmChr(const char *name, void *dest, u32 maxSize) {
    nw4r::g3d::ResAnmChr result;
    if (mCurrentRes.IsValid()) {
        result = mCurrentRes.GetResAnmChr(name);
        if (result.IsValid()) {
            return result;
        }
    }
    nw4r::g3d::ResFile file = getExternalCompressedFile(name, "rchaC", dest, maxSize);
    result = file.GetResAnmChr(name);
    return result;
}

nw4r::g3d::ResAnmTexPat daPlayerModelBase_c::getExternalAnmTexPat(const char *name, void *dest, u32 maxSize) {
    nw4r::g3d::ResAnmTexPat result;
    if (mCurrentRes.IsValid()) {
        result = mCurrentRes.GetResAnmTexPat(name);
        if (result.IsValid()) {
            result.Bind(mAlink2Res);
            return result;
        }
    }
    nw4r::g3d::ResFile file = getExternalCompressedFile(name, "rtpaC", dest, maxSize);
    result = file.GetResAnmTexPat(name);
    result.Bind(mAlink2Res);
    return result;
}

nw4r::g3d::ResAnmTexSrt daPlayerModelBase_c::getExternalAnmTexSrt(const char *name, void *dest, u32 maxSize) {
    nw4r::g3d::ResAnmTexSrt result;
    if (mCurrentRes.IsValid()) {
        result = mCurrentRes.GetResAnmTexSrt(name);
        if (result.IsValid()) {
            return result;
        }
    }
    nw4r::g3d::ResFile file = getExternalCompressedFile(name, "rtsaC", dest, maxSize);
    result = file.GetResAnmTexSrt(name);
    return result;
}

bool daPlayerModelBase_c::canStart(bool force, u16 faceIdx, u16 invalidValue, u16 *out1, u16 *out2) const {
    if (force) {
        *out2 = faceIdx;
    } else {
        *out1 = faceIdx;
        if (*out2 != invalidValue) {
            return false;
        }
    }
    return true;
}

void daPlayerModelBase_c::setFaceTexPat(s32 faceIdx, bool force) {
    if ((!force && ((mFaceAnmTexPatIdx1 == faceIdx && mFaceAnmTexPatIdx1 == 1) ||
                    (checkFaceUpdateFlags(0x40000000) && (faceIdx == PLAYER_FACEMABA01))))) {
        return;
    }

    if (faceIdx != 0x38) {
        offFaceUpdateFlags(0x40000000);
        if (canStart(force, faceIdx, PLAYER_FACE_MAX, &mFaceAnmTexPatIdx1, &mFaceAnmTexPatIdx2)) {
            nw4r::g3d::ResAnmTexPat anm = getExternalAnmTexPat(sFaceResNames[faceIdx], mpTexPatBuffer, 0x1000);
            mFaceTexPat.setAnm(mFaceMdl, anm, 0, m3d::PLAY_MODE_4);
            mFaceTexPat.setFrame(0.0f, 0);
        }
    }
}

void daPlayerModelBase_c::checkFaceTexPat() {
    if (mFaceAnmTexPatIdx2 == PLAYER_FACE_MAX) {
        return;
    }
    s32 prev = mFaceAnmTexPatIdx1;
    mFaceAnmTexPatIdx2 = PLAYER_FACE_MAX;
    if (prev >= PLAYER_FACE_BEDSLEEP) {
        prev = PLAYER_FACEMABA01;
    }
    mFaceAnmTexPatIdx1 = PLAYER_FACE_MAX;
    setFaceTexPat(prev, false);
}

void daPlayerModelBase_c::setFaceTexSrt(s32 faceIdx, bool force) {
    offFaceUpdateFlags(0x80000000);
    if (canStart(force, faceIdx, PLAYER_FACE_MAX, &mFaceAnmTexSrtIdx1, &mFaceAnmTexSrtIdx2)) {
        nw4r::g3d::ResAnmTexSrt anm = getExternalAnmTexSrt(sFaceResNames[faceIdx], mpTexSrtBuffer, 0x1000);
        mFaceTexSrt.setAnm(mFaceMdl, anm, 0, m3d::PLAY_MODE_4);
        mFaceTexSrt.setFrame(0.0f, 0);
    }
}

void daPlayerModelBase_c::checkFaceTexSrt() {
    if (mFaceAnmTexSrtIdx2 == PLAYER_FACE_MAX) {
        return;
    }
    s32 prev = mFaceAnmTexSrtIdx1;
    mFaceAnmTexSrtIdx2 = PLAYER_FACE_MAX;
    if (prev >= PLAYER_FACE_BEDSLEEP) {
        prev = PLAYER_FACE_DEFAULT;
    }
    mFaceAnmTexSrtIdx1 = PLAYER_FACE_MAX;
    setFaceTexSrt(prev, false);
}

void daPlayerModelBase_c::setFaceAnmChr(s32 faceIdx, bool force) {
    if (faceIdx == PLAYER_FACE_NONE || (!force && (checkFaceUpdateFlags(0x20000000) &&
                                                   (faceIdx == PLAYER_FACE_DEFAULT || faceIdx == PLAYER_FACE_M)))) {
        return;
    }
    offFaceUpdateFlags(0x20000000);
    if (canStart(force, faceIdx, PLAYER_FACE_MAX, &mFaceAnmChrIdx1, &mFaceAnmChrIdx2)) {
        nw4r::g3d::ResAnmChr anm = getExternalAnmChr(sFaceResNames[faceIdx], mpAnmCharBuffer, 0x1000);
        mFaceAnmChr.setAnm(mFaceMdl, anm, m3d::PLAY_MODE_4);
        mFaceAnmChr.setFrameOnly(0.0f);
        mFaceMdl.setAnm(mFaceAnmChr);
    }
}

void daPlayerModelBase_c::checkFaceAnmChr() {
    if (mFaceAnmChrIdx2 == PLAYER_FACE_MAX) {
        return;
    }
    s32 prev = mFaceAnmChrIdx1;
    mFaceAnmChrIdx2 = PLAYER_FACE_MAX;
    if (prev >= PLAYER_FACE_BEDSLEEP) {
        prev = PLAYER_FACE_DEFAULT;
    }
    mFaceAnmChrIdx1 = PLAYER_FACE_MAX;
    setFaceAnmChr(prev, false);
}

void daPlayerModelBase_c::loadAnmChr(s32 childIdx, s32 animIdx, void *dest, u32 maxSize) {
    nw4r::g3d::ResAnmChr anm = getExternalAnmChr(sAnimations[animIdx].animName, dest, maxSize);
    mAnmChrs[childIdx].setAnm(mMainMdl, anm, m3d::PLAY_MODE_4);
    nw4r::g3d::AnmObjChr *animObj = static_cast<nw4r::g3d::AnmObjChr *>(mAnmChrs[childIdx].getAnimObj());
    animObj->Release();
    nw4r::g3d::ResMdl bodyMdl = mMainMdl.getResMdl();
    if (isBodyAnmPart_0_2_4(childIdx)) {
        animObj->Bind(bodyMdl, PLAYER_MAIN_NODE_WAIST, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
        animObj->Bind(bodyMdl, PLAYER_MAIN_NODE_CENTER, nw4r::g3d::AnmObjChr::BIND_ONE);
    } else {
        animObj->Bind(bodyMdl, PLAYER_MAIN_NODE_BACKBONE_1, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
    }
    mAnmChrBlend.attach(childIdx, &mAnmChrs[childIdx], 1.0f);
    mMainMdl.setAnm(mAnmChrBlend);
    mAnimations[childIdx] = animIdx;
    mAnmChrs[childIdx].setField_0x35(0);
}

void daPlayerModelBase_c::removeAnmChr(s32 childIdx) {
    mAnmChrs[childIdx].getAnimObj()->Release();
    mAnmChrBlend.detach(childIdx);
    mAnimations[childIdx] = PLAYER_ANIM_MAX;
}

void daPlayerModelBase_c::loadSound(nw4r::g3d::ResFile &file, const char *name, s32 childIdx) {
    dSoundSourceIf_c *s = getSoundSource();
    mCurrentAnmChrIdx = childIdx;
    SizedString<64> path;
    path.sprintf("%s.brasd", name);
    void *dat = file.GetExternalData(path);
    if (dat == nullptr && file == mCurrentRes) {
        dat = mPlCommonSoundRes.GetExternalData(path);
    }
    mMainMdl.setSoundRelated(mAnimations[childIdx], dat);
    s->load(dat, name);
}

void daPlayerModelBase_c::loadSoundForAnim(s32 childIdx) {
    dSoundSourceIf_c *s = getSoundSource();
    s32 anim = mAnimations[childIdx];
    mCurrentAnmChrIdx = childIdx;
    if (anim != mMainMdl.getField_0x5A()) {
        loadSound(mCurrentRes.IsValid() ? mCurrentRes : mPlCommonSoundRes, sAnimations[anim].animName, childIdx);
    } else {
        s->load(mMainMdl.getSoundData(), sAnimations[anim].animName);
    }
    s->setRate(mAnmChrs[childIdx].getRate());
}

void daPlayerModelBase_c::syncSoundWithAnim() {
    f32 frame = mAnmChrs[mCurrentAnmChrIdx].getFrame();
    getSoundSource()->setFrame(frame);
}

void daPlayerModelBase_c::registMassObj(cCcD_Obj *obj, u8 priority) {
    dCcS::GetInstance()->Set(obj);
    dCcS::GetInstance()->GetMassMng().SetObj(obj, priority);
}

void daPlayerModelBase_c::updateModelColliders() {
    mVec3_c bodyTranslation;
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_CENTER, bodyTranslation);

    mVec3_c bottom;
    bottom = ((mToeTranslation[0] + mToeTranslation[1]) * 0.5f + bodyTranslation) * 0.5f;
    f32 f = mToeTranslation[0].y > mToeTranslation[1].y ? mToeTranslation[1].y : mToeTranslation[0].y;
    f32 height;
    if (bodyTranslation.y > f) {
        height = bodyTranslation.y - f;
        bottom.y = f;
    } else {
        bottom.y = bodyTranslation.y;
        height = f - bodyTranslation.y;
    }

    if (height < 60.0f) {
        bottom.y -= (60.0f - height) * 0.5f;
        height = 60.0f;
    }
    mCcCyls[0].SetC(bottom);
    mCcCyls[0].SetH(height);

    if (!checkActionFlagsCont(0x20000)) {
        bottom = (bodyTranslation + mHeadTranslation) * 0.5f;
    }

    if (bodyTranslation.y > mHeadTranslation.y) {
        height = bodyTranslation.y - mHeadTranslation.y;
        bottom.y = mHeadTranslation.y;
    } else {
        bottom.y = bodyTranslation.y;
        height = mHeadTranslation.y - bodyTranslation.y;
    }

    if (height < 60.0f) {
        bottom.y -= (60.0f - height) * 0.5f;
        height = 60.0f;
    }

    mCcCyls[2].SetC(bottom);
    mCcCyls[2].SetH(height);

    bottom = (mCcCyls[0].GetC() + mCcCyls[2].GetC()) * 0.5f;
    f32 midH = (mCcCyls[0].GetH() + mCcCyls[2].GetH()) * 0.5f;

    mCcCyls[1].SetC(bottom);
    mCcCyls[1].SetH(midH);
}

void daPlayerModelBase_c::updateCachedPositions() {
    static const Vec sPosAboveLink = {0.0f, -8.0f, 0.0f};
    static const Vec sPos1 = {12.0f, -8.0f, 0.0f};
    static const Vec sHeadPos = {0.0f, -28.0f, 0.0f};

    mMainMdl.getNodeWorldMtxMultVec(PLAYER_MAIN_NODE_HEAD, sPos1, mPositionCopy2);
    mMainMdl.getNodeWorldMtxMultVec(PLAYER_MAIN_NODE_HEAD, sPosAboveLink, mPositionAboveLink);
    mMainMdl.getNodeWorldMtxMultVec(PLAYER_MAIN_NODE_HEAD, sHeadPos, mHeadTranslation);

    static const Vec sUnusedVec1 = {0.0f, 55.75f, 15.0f};
    static const Vec sUnusedVec2 = {9.5f, 47.0f, 24.5f};
    static const Vec sUnusedVec3 = {1.75f, 55.0f, 25.5f};
    static const Vec sUnusedVec4 = {0.0f, 55.0f, 25.0f};

    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_TOE_L, mToeTranslation[0]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_TOE_R, mToeTranslation[1]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_HAND_L, mTranslationHand[0]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_HAND_R, mTranslationHand[1]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_WEAPON_L, mTranslationWeapon[0]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_WEAPON_R, mTranslationWeapon[1]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_CENTER, mCenterTranslation);

    if (checkCurrentAction(/* DOWSE_LOOK */ 0x68)) {
        mVec3_c v(0.0f, 18.0f, 0.0f);
        v.rotX(field_0x1268);
        v.rotY(mRotation.y + field_0x126A);
        mPositionCopy2 += v;
    }
}

void daPlayerModelBase_c::setPosCopy3() {
    static const Vec posCopy3v0 = {0.0f, 150.0f, 0.0f};
    static const Vec posCopy3v1 = {0.0f, 40.0f, 0.0f};
    static const Vec posCopy3v2 = {0.0f, 95.0f, 0.0f};
    static const Vec posCopy3v3 = {0.0f, 14.0f, 0.0f};
    if (checkCurrentAction(/* FREE_FALL*/ 0x13) || checkCurrentAction(/* WALKING_ON_TIGHTROPE */ 0x81)) {
        mPositionCopy3 = mPosition;
        mPositionCopy3.y += 180.0f;
    } else if (checkCurrentAction(/* BEING_PULLED_BY_CLAWS */ 0x5A)) {
        mPositionCopy3 = mPositionCopy2;
    } else if (checkActionFlags(FLG0_SWING_ROPE)) {
        mPositionCopy3 = mPosition;
    } else if (checkActionFlagsCont(0x20000000) || checkCurrentAction(0x70) ||
               checkCurrentAction(/* HANG_ON_ZIP */ 0x85)) {
        mPositionCopy3 = mPosition;
        mPositionCopy3.y -= 100.0f;
    } else {
        // TODO maybe fakematch, is there a way to avoid this goto?
        if (checkActionFlags(FLG0_HANGING_LEDGE)) {
            goto label;
        }

        switch (getCurrentAction()) {
            case 0x82:
            case 0x83:
            case 0x84: /* CLIMBING_ONTO_TIGHTROPE */ {
            label:
                const nw4r::math::MTX34 *c = mHeadMdl.getLocalMtx();
                mPositionCopy3.x = c->_03;
                mPositionCopy3.y = c->_13;
                mPositionCopy3.z = c->_23;
                break;
            }
            case 0x09: {
                // SLIDING
                mPositionCopy3 = mPosition + mVec3_c(posCopy3v2);
                break;
            }
            default: {
                mMtx_c mtx;
                mMainMdl.getLocalMtx(mtx);

                f32 f;
                const Vec *v;
                if (checkActionFlags(FLG0_IN_WATER) || checkCurrentAction(/* SWIM_DASH_INFO_AIR */ 0x57)) {
                    v = &posCopy3v3;
                    f = mPosition.y;
                } else {
                    if (checkCurrentAction(/* VOID_SAND */ 0x4C)) {
                        v = &posCopy3v3;
                    } else {
                        v = checkActionFlags(FLG0_CRAWLING) ? &posCopy3v1 : &posCopy3v0;
                    }
                    f = mtx.m[1][3];
                }
                mPositionCopy3.copyFrom(v);
                mPositionCopy3.x += mPosition.x;
                mPositionCopy3.y += f;
                mPositionCopy3.z += mPosition.z;
                break;
            }
        }
    }
}

bool daPlayerModelBase_c::fn_80061410() {
    return (field_0x120A || alwaysRet0()) && !(someFlags_0x354 & 0x800);
}

// I believe this is the only strong virtual function of daPlayerModelBase_c,
// and this causes the vtable and all other weak functions to be here
/* vt 0x114 */ void daPlayerModelBase_c::somethingWithCarriedActorFlags() {
    if (mCarriedActorRef.get() != nullptr) {
        mCarriedActorRef.get()->setObjectProperty(OBJ_PROP_0x200);
    }
}
