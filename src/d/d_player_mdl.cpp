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

const PlayerAnimation daPlayerModelBase_c::sAnimations[] = {
    {                "atrFW",  13,   6,  1,  0,  0},
    {              "atrFW_R",   7,   0,  1,  0,  0},
    {                "atrFD",  13,   6,  1,  0,  0},
    {              "atrFD_R",   7,   0,  1,  0,  0},
    {                "atrbW",  13,   6,  1,  0,  0},
    {              "atrbW_R",   7,   0,  1,  0,  0},
    {                "atrbD",  13,   6,  1,  0,  0},
    {              "atrbD_R",   7,   0,  1,  0,  0},
    {               "atrsWL",  13,   6,  1,  0,  0},
    {               "atrsDL",  13,   6,  1,  0,  0},
    {               "atrsWR",  13,   6,  1,  0,  0},
    {               "atrsDR",  13,   6,  1,  0,  0},
    {                "atsWL",  13,   6,  1,  0,  0},
    {                "atsDL",  13,   6,  1,  0,  0},
    {                "atsWR",  13,   6,  1,  0,  0},
    {                "atsDR",  13,   6,  1,  0,  0},
    {          "atsWL_right",  13,   6,  1,  0,  0},
    {          "atsDL_right",  13,   6,  1,  0,  0},
    {                  "atL",   9,   2,  1,  0,  0},
    {                  "atR",   9,   2,  1,  0,  0},
    {                "dashA",  13,  56,  1,  0,  0},
    {                "dashS",  13,  56,  1,  0,  0},
    {               "atrbWH",   7,   0,  1,  0,  0},
    {                 "atbW",  13,   6,  1,  0,  0},
    {                 "atbD",  13,   6,  1,  0,  0},
    {                "walkH",   7,  50,  1,  0,  0},
    {                "waitS",  13,  56,  1,  0,  0},
    {                "walkS",  13,  56,  1,  0,  0},
    {                "stepL",  13,  56,  1,  0,  0},
    {                "stepR",  13,  56,  1,  0,  0},
    {              "atsJLSt",   7,  50,  1,  0,  2},
    {              "atsJLEd",   9,  52,  1,  0,  2},
    {              "atsJRSt",   7,  50,  1,  0,  2},
    {              "atsJREd",   9,  52,  1,  0,  2},
    {              "atsJFSt",  13,  56,  1,  0,  2},
    {              "atsJFEd",   9,  52,  1,  0,  2},
    {              "rollBSt",  13,  56,  1,  0,  2},
    {              "rollBEd",   9,  52,  1,  0,  2},
    {                "waitB",   9,  52,  1,  0,  2},
    {             "waitBtoA",   9,  52,  1,  0,  0},
    {               "sjmpSt",   7,  50,  1,  0,  0},
    {               "sjmpEd",   7,  50,  1,  0,  0},
    {                "rollF",   7,  50,  1,  0,  2},
    {             "rollFmis",   7,   0, 17,  0, 17},
    {               "jumpSt",   7,  50,  1,  0,  2},
    {              "jumpSEd",   7,  50,  1,  0,  2},
    {                 "vjmp",   7,  50,  1,  0,  2},
    {              "vjmpChA",   7,  50,  1,  0,  2},
    {              "vjmpChB",   7,  50,  1,  0,  2},
    {               "vjmpCl",   7,  50,  1,  0,  2},
    {                "hangL",   7,  50,  1,  0,  2},
    {                "hangR",   7,  50,  1,  0,  2},
    {                 "Hang",   7,  50,  1,  0,  2},
    {            "climbHang",  12,  55,  1,  0,  2},
    {          "climbSlideL",  12,  55,  1,  0,  2},
    {          "climbSlideR",  12,  55,  1,  0,  2},
    {        "climbHangmiss",  12,  55,  1,  0,  2},
    {        "climbStruggle",  12,   5, 11,  0, 11},
    {         "ClimbIvyHang",  12,   5,  1,  0,  2},
    {            "ClimbIvyL",  12,   5,  1,  0,  2},
    {            "ClimbIvyR",  12,   5,  1,  0,  2},
    {         "ClimbIvyL_St",  12,   5,  1,  0,  2},
    {         "ClimbIvyR_St",  12,   5,  1,  0,  2},
    {       "ClimbIvySlideL",  12,   5,  1,  0,  2},
    {       "ClimbIvySlideR",  12,   5,  1,  0,  2},
    {   "ClimbIvySlantLU_RD",  12,   5,  1,  0,  2},
    {   "ClimbIvySlideRU_LD",  12,   5,  1,  0,  2},
    {              "ladUpSt",  13,   6,  1,  0,  0},
    {             "ladUpEdL",  13,   6,  1,  0,  0},
    {             "ladUpEdR",  13,   6,  1,  0,  0},
    {              "ladDwSt",  13,   6,  1,  0,  0},
    {             "ladDWEdL",  13,   6,  1,  0,  0},
    {             "ladDWEdR",  13,   6,  1,  0,  0},
    {              "ladRtoL",  13,   6,  1,  0,  0},
    {              "ladLtoR",  13,   6,  1,  0,  0},
    {          "LadJumpUpSt",  13,   6,  1,  0,  0},
    {        "LadJumpDownSt",  13,   6,  1,  0,  0},
    {          "LadJumpUpEd",  13,   6,  1,  0,  0},
    {        "LadJumpDownEd",  13,   6,  1,  0,  0},
    {                 "cutT",  13, 100,  1,  0,  2},
    {                "cutTB",  13,   4,  1,  0,  2},
    {                "cutTL",  13,   4,  1,  0,  2},
    {               "cutTLB",  13,   4,  1,  0,  2},
    {               "cutJSt",  13,   4,  1,  0,  2},
    {               "cutJEd",  13,   4,  1,  0,  2},
    {                 "damF",  13,  56,  6,  0,  6},
    {                 "damB",  13,  56,  6,  0,  6},
    {                 "damL",  13,  56,  6,  0,  6},
    {                 "damR",  13,  56,  6,  0,  6},
    {         "DamageMFront",  13,  56,  6,  0,  6},
    {          "DamageMBack",  13,  56,  6,  0,  6},
    {          "DamageMLeft",  13,  56,  6,  0,  6},
    {         "DamageMRight",  13,  56,  6,  0,  6},
    {                "damFF",  13,  56,  7,  0,  7},
    {                "damFB",  13,  56,  7,  0,  7},
    {                "damFL",  13,  56,  7,  0,  7},
    {                "damFR",  13,  56,  7,  0,  7},
    {              "damFFup",   7,  50, 17,  0, 17},
    {              "damFBup",   7,  50, 17,  0, 17},
    {              "damFLup",   7,  50, 17,  0, 17},
    {              "damFRup",   7,  50, 17,  0, 17},
    {    "DamageLFrontSlide",   7,  50, 12,  0, 12},
    {     "DamageLBackSlide",   7,  50, 12,  0, 12},
    {     "DamageLLeftSlide",   7,  50, 12,  0, 12},
    {    "DamageLRightSlide",   7,  50, 12,  0, 12},
    {                  "dam",  13,  56,  6,  0,  6},
    {             "landDamA",   7,  50, 56,  0, 56},
    {           "landDamAst",  13,  50, 17,  0, 17},
    {                "cutST",   9,  52, 11,  0, 11},
    {              "atDefNG",   9,  52,  6,  0,  6},
    {                  "die",  13,  56,  8,  8,  8},
    {  "DieLargeDamageFront",   7,   0,  9,  9,  9},
    {   "DieLargeDamageBack",   7,   0,  9,  9,  9},
    {   "DieLargeDamageLeft",   7,   0,  9,  9,  9},
    {  "DieLargeDamageRight",   7,   0,  9,  9,  9},
    {             "swimwait",   7,   0,  1,  0,  0},
    {             "swimingA",   7,   0,  1,  0,  0},
    {                "swimP",   7,   0, 30, 30, 30},
    {               "atsSwL",   7,   0,  1,  0,  0},
    {               "atsSwR",   7,   0,  1,  0,  0},
    {                "atbSw",   7,   0,  1,  0,  0},
    {             "swimDive",   7,   0,  1,  0,  0},
    {            "swimingSt",   7,   0,  1,  0,  0},
    {             "swimingB",   7,   0,  1,  0,  0},
    {             "swimingC",   7,   0,  1,  0,  2},
    {       "SwimAttackMiss",   7,   0, 17,  0, 17},
    {               "damSwF",  13,   6,  6,  0,  6},
    {               "damSwB",  13,   6,  6,  0,  6},
    {               "damSwL",  13,   6,  6,  0,  6},
    {               "damSwR",  13,   6,  6,  0,  6},
    {               "damSwW",  13,   6,  6,  0,  6},
    {             "swimDieA",  13,   6, 25, 25, 25},
    {             "swimDieP",   7,   0, 12,  8, 12},
    {             "DieMagma",   7,   0, 10, 10, 10},
    {               "slideF",   7,  50, 12,  0, 12},
    {               "slideB",   7,  50, 12,  0, 12},
    {             "slideFEd",   7,  50,  1,  0,  0},
    {             "slideBEd",   7,  50,  1,  0,  0},
    {               "HSjmpL",   8,   1,  1,  0,  2},
    {               "HSjmpR",   8,   1,  1,  0,  2},
    {              "HSshoot",   8,   1,  1,  0,  2},
    {            "HShangEdL",   8,   1,  1,  0,  2},
    {            "HShangEdR",   8,   1,  1,  0,  2},
    {              "HShangL",   8,   1,  1,  0,  2},
    {              "HShangR",   8,   1,  1,  0,  2},
    {          "HShangwaitL",   8,   1,  1,  0,  2},
    {          "HShangwaitR",   8,   1,  1,  0,  2},
    {         "HShangshootL",   8,   1,  1,  0,  2},
    {         "HShangshootR",   8,   1,  1,  0,  2},
    {            "HSwallEdL",   8,   1,  1,  0,  2},
    {            "HSwallEdR",   8,   1,  1,  0,  2},
    {              "HSwallL",   8,   1,  1,  0,  2},
    {              "HSwallR",   8,   1,  1,  0,  2},
    {          "HSwallwaitL",   8,   1,  1,  0,  2},
    {          "HSwallwaitR",   8,   1,  1,  0,  2},
    {         "HSwallshootL",   8,   1,  1,  0,  2},
    {         "HSwallshootR",   8,   1,  1,  0,  2},
    {                  "lie",   7,   0,  1,  0,  0},
    {                 "lieF",   7,   0,  1,  0,  0},
    {               "waitST",   9,   2, 11,  0, 11},
    {   "WaitStruggleDamage",   9,   2,  6,  0,  6},
    {               "waitPP",   7,   0,  1,  0,  0},
    {                "pushW",   7,   0,  1,  0,  2},
    {                "pullW",   8,   1,  1,  0,  2},
    {              "jumpBSt",   7,   0,  1,  0,  3},
    {                "jumpB",   7,   0,  1,  0,  2},
    {                "waitD",  13,  56, 56,  0, 56},
    {             "waitAtoD",  13,  56,  1,  0,  0},
    {         "WaitServiceA",  13,   6, 26, 26, 26},
    {      "WaitServiceHotA",  13,   6, 27, 27, 27},
    {              "dashUpL",   9,  52,  1,  0,  2},
    {              "dashUpR",   9,  52,  1,  0,  2},
    {         "climbjumpUSt",  12,   5,  1,  0,  2},
    {         "climbjumpDSt",  12,   5,  1,  0,  2},
    {         "climbjumpLSt",  12,  55,  1,  0,  2},
    {         "climbjumpRSt",  12,  55,  1,  0,  2},
    {         "climbjumpUEd",  12,   5,  1,  0,  2},
    {         "climbjumpDEd",  12,   5,  1,  0,  2},
    {         "climbjumpLEd",  12,   5,  1,  0,  2},
    {         "climbjumpREd",  12,   5,  1,  0,  2},
    {          "hangjumpLSt",   7,  50,  1,  0,  2},
    {          "hangjumpRSt",   7,  50,  1,  0,  2},
    {          "hangjumpLEd",   7,  50,  1,  0,  2},
    {          "hangjumpREd",   7,  50,  1,  0,  2},
    {         "DoorKnobPull",  13,   6,  1,  0,  0},
    {         "DoorKnobPush",  13,   6,  1,  0,  0},
    {     "DoorKnobPullLock",  13,   5,  1,  0,  0},
    {     "DoorKnobPushLock",  13,   5,  1,  0,  0},
    {              "DoorOpD",   7,   0,  1,  0,  2},
    {                "dashB",  13,   6,  1,  0,  2},
    {                "MgDig",  13,   6,  1,  0,  0},
    {               "MPCutU",  13,   4,  1,  0,  2},
    {            "MPCutU_Re",  13,   4,  1,  0,  2},
    {              "MPCutLU",  13,   4,  1,  0,  2},
    {           "MPCutLU_Re",  13,   4,  1,  0,  2},
    {               "MPCutL",  13,   4,  1,  0,  2},
    {            "MPCutL_Re",  13,   4,  1,  0,  2},
    {              "MPCutLD",  13,   4,  1,  0,  2},
    {           "MPCutLD_Re",  13,   4,  1,  0,  2},
    {              "MPCutRD",  13,   4,  1,  0,  2},
    {           "MPCutRD_Re",  13,   4,  1,  0,  2},
    {               "MPCutR",  13,   4,  1,  0,  2},
    {            "MPCutR_Re",  13,   4,  1,  0,  2},
    {              "MPCutRU",  13,   4,  1,  0,  2},
    {           "MPCutRU_Re",  13,   4,  1,  0,  2},
    {         "MPCutD_right",  13,   4,  1,  0,  2},
    {      "MPCutD_right_Re",  13,   4,  1,  0,  2},
    {          "MPCutD_left",  13,   4,  1,  0,  2},
    {       "MPCutD_left_Re",  13,   4,  1,  0,  2},
    {              "CutPush",  13,   4,  1,  0,  2},
    {           "CutPush_Re",  13,   4,  1,  0,  2},
    {       "EnemyGuardJust",  13, 100,  6,  0,  6},
    {         "LiftB_Before",   7,   0,  1,  0,  0},
    {         "LiftM_Before",   7,   0,  1,  0,  0},
    {                "LiftB",   7,   0,  1,  0,  0},
    {                "LiftS",   7,   0,  1,  0,  0},
    {                "LiftM",   7,   0,  1,  0,  0},
    {                "LiftL",   7,   0,  1,  0,  2},
    {              "ThrowBU",   7,   0,  1,  0,  2},
    {              "ThrowSU",   7,   0,  1,  0,  2},
    {              "ThrowMU",   7,   0,  1,  0,  2},
    {              "ThrowLU",   7,   0,  1,  0,  2},
    {              "ThrowBD",   7,   0,  1,  0,  2},
    {              "ThrowSD",   7,   0,  1,  0,  2},
    {        "MPGuardShield",   7,   2,  4,  0,  4},
    { "MPGuardShield_Impact",   7,   2,  1,  0,  2},
    { "MPGuardShield_Normal",   7,   2,  1,  0,  2},
    {              "vjmpChA",   8,   1,  1,  0,  2},
    {         "RopeHoldWait",   8,   1,  1,  0,  2},
    {           "RopeClimbL",   8,   1,  1,  0,  2},
    {         "RopeHoldWait",   8,   1,  1,  0,  2},
    {             "RopePull",   8,   1,  1,  0,  2},
    {        "RopeTightHold",   8,   1,  1,  0,  2},
    {            "RopeFront",   8,   1,  1,  0,  2},
    {             "RopeBack",   8,   1,  1,  0,  2},
    {         "RopeStruggle",   8,   1, 11,  0, 11},
    {               "DiveSt",   7,   0,  1,  0,  2},
    {                 "Dive",   7,   0,  1,  0,  2},
    {            "StoleLand",  13,   6,  1,  0,  2},
    {             "DiveDrop",   7,   0,  1,  0,  2},
    {                "Float",   9,   2,  1,  0,  0},
    {            "FloatAway",   9,   2, 12,  0, 12},
    {          "DiveToFloat",   9,   2,  1,  0,  2},
    {    "RocketBeetle_Wait",   9,   2,  1,  0,  0},
    {   "RocketBeetle_Shoot",   9,   2,  1,  0,  0},
    {   "RocketBeetle_Reset",   9,   2,  1,  0,  0},
    {    "RocketBeetle_Back",   9,   2,  1,  0,  0},
    {                "WhipU",   8,   4,  1,  0,  2},
    {               "WhipLU",   8,   4,  1,  0,  2},
    {                "WhipL",   8,   4,  1,  0,  2},
    {               "WhipLD",   8,   4,  1,  0,  2},
    {               "WhipRD",   8,   4,  1,  0,  2},
    {                "WhipR",   8,   4,  1,  0,  2},
    {               "WhipRU",   8,   4,  1,  0,  2},
    {           "WhipBackRD",   7,   0,  1,  0,  2},
    {            "WhipBackR",   7,   0,  1,  0,  2},
    {           "WhipBackRU",   7,   0,  1,  0,  2},
    {            "WhipBackU",   7,   0,  1,  0,  2},
    {           "WhipBackLU",   7,   0,  1,  0,  2},
    {            "WhipBackL",   7,   0,  1,  0,  2},
    {           "WhipBackLD",   7,   0,  1,  0,  2},
    {             "turnBack",  13,   6,  1,  0,  2},
    {               "atLtoR",   7,   0,  1,  0,  0},
    {               "atRtoL",   7,   0,  1,  0,  0},
    {       "ChuchuStruggle",   9,  52, 11,  0, 11},
    {   "ChuchuStruggle_big",   9,  52, 11,  0, 11},
    {   "ChuchuStruggle_act",   9,  52,  1,  0,  2},
    {"ChuchuStruggle_Escape",   9,  52,  1,  0,  2},
    {             "Surprise",  13,  56, 12,  0, 12},
    {               "Danger",   9,  52, 23, 23, 23},
    {             "RockRide",   9,  52, 12,  0, 12},
    {               "Notice",  13,  56, 29, 29, 29},
    {           "waitinsect",   9,  52, 11,  0, 11},
    {                "numbA",  12,  55,  7,  0,  7},
    {                "numbB",  12,   5,  7,  0,  7},
    {               "damFEF",   7,  50,  7,  0,  7},
    {               "damFEB",   7,  50,  7,  0,  7},
    {               "damFEL",   7,  50,  7,  0,  7},
    {               "damFER",   7,  50,  7,  0,  7},
    {                 "getA",   7,   0,  1,  0,  0},
    {             "getAwait",   7,   0,  1, 21, 21},
    {               "GetBig",   7,   0,  1,  0,  0},
    {           "GetBigWait",   7,   0,  1, 20, 20},
    {           "BoxOpSmall",   7,   0,  1,  0,  0},
    {                "BoxOp",   7,   0,  1, 28, 28},
    {               "cutUSt",   7, 100,  1,  0,  2},
    {                 "cutU",   7, 100,  1,  0,  2},
    {               "cutUEd",   9, 100,  1,  0,  2},
    {               "cutUNG",   9, 100,  1,  0,  2},
    {        "WaitLookRound",  13,  56,  1, 22, 22},
    {             "entrance",  13,   6, 18, 18, 18},
    {          "MPPosePower",  13, 255,  1,  0,  2},
    {         "MPPosePowerL",  13, 255,  1,  0,  2},
    {            "BinswingS",  13,   5,  1,  0,  0},
    {            "BinswingU",  13,   5,  1,  0,  0},
    {               "Binout",  13,   5, 19, 19, 19},
    {                "Drift",   7,   0,  7,  0,  7},
    {       "swimDieARevive",  13,   6, 17,  0, 17},
    {         "MgLieFRevive",   7,   0, 17,  0, 17},
    {               "CatchL",  13,   6,  1,  0,  0},
    {               "CatchR",  13,   6,  1,  0,  0},
    {          "BattleReady",   9,  52,  1,  0,  2},
    {             "RollJump",  13,   6,  1,  0,  2},
    {            "TansuOpen",  13,   6,  1,  0,  0},
    {           "TansuClose",  13,   6,  1,  0,  0},
    {            "BedSleepL",  13,   6, 31, 31, 31},
    {             "BedSleep",  13,   6, 31, 31, 31},
    {             "BedAwake",   8,   1, 32, 32, 32},
    {           "Bird_Glide",   8,   1,  1,  0,  0},
    {     "Bird_GlideAround",   8,   1,  1,  0,  0},
    {           "Bird_Reach",   8,   5, 40, 40, 40},
    {         "Bird_GetWait",   8,   0, 41, 41, 41},
    {           "Bird_Accel",   8,   1,  4,  0,  4},
    {          "Bird_Damage",   8,   1,  6,  0,  6},
    {     "Bird_DamageSmall",   8,   1,  6,  0,  6},
    {   "Bird_DamageTornado",   8,   1,  7,  0,  7},
    {            "Bird_Call",  10,   3,  1,  0, 55},
    {          "SwordPullSt",  13,   6, 33, 33, 33},
    {        "SwordPullWait",   8,   1,  1,  0, 34},
    {            "SwordPull",   8,   1, 35, 35, 35},
    {         "SwordSirenSt",  13,   1, 36, 36, 36},
    {     "SwordSirenStWait",   8,   1, 36, 36, 36},
    {      "SwordSirenRaise",   8,   1, 37, 37, 37},
    {  "SwordSirenRaiseWait",   8,   1, 37, 37, 37},
    {      "SwordSirenStick",   8,   1, 38, 38, 38},
    {      "SwordSirenAfter",   8, 100, 39, 39, 39},
    {        "SwitchDial_st",   9,   4,  1,  0,  2},
    {       "SwitchDial_stL",   9,   4,  1,  0,  2},
    {      "SwitchDial_LtoR",   9,   4,  1,  0,  2},
    {      "SwitchDial_push",   9,   4,  1,  0,  2},
    {            "TruckLtoR",  12,   5,  1,  0,  2},
    {     "TruckFrontToBack",  12,   5,  1,  0,  2},
    {          "TruckJumpSt",  12,   5,  1,  0,  2},
    {            "TruckJump",  12,   5,  1,  0,  2},
    {         "TruckJumpEnd",  12,   5, 12,  0, 12},
    {           "TruckBrake",  12,   5, 11,  0, 11},
    {              "TruckIn",  13,   6,  1,  0,  0},
    {             "TruckOut",  13,   6,  1,  0,  0},
    {             "DieTruck",  13,  56, 42, 42, 42},
    {       "DieTruckRevive",  13,  56, 17,  0, 17},
    {              "MgDigIn",   7,   0,  1,  0,  2},
    {             "MgDigOut",   7,   0,  1,  0,  2},
    {             "MgLieF_L",   7,   0,  1,  0,  0},
    {             "MgLieF_R",   7,   0,  1,  0,  0},
    {            "MgAttackL",   7,   0,  4,  0,  4},
    {            "MgAttackR",   7,   0,  4,  0,  4},
    {              "MgDashL",   7,   0,  1,  0,  2},
    {              "MgDashR",   7,   0,  1,  0,  2},
    {             "MgDamage",   7,   0,  6,  0,  6},
    {         "MgWaitBreath",   7,   0, 16,  0, 16},
    {              "MgLieUp",   7,   0,  1,  0,  2},
    {            "LiftRemly",  13,   6,  1, 21, 21},
    {         "ThrowRemlyUp",  13,   6,  1, 21, 21},
    {        "LiftRemlyUtoM",  13,   6,  1, 21, 21},
    {   "Bird_FestivalStart",   9,   2, 43, 43, 43},
    {          "GetSurprise",  13,   6, 44, 44, 44},
    {       "GirahimGrapple",   9,   2,  1,  0,  2},
    {          "GirahimPull",   9,   2,  1,  0,  2},
    {        "GirahimPullUp",   9,   2,  1,  0,  2},
    {        "GirahimChange",  13, 100, 12,  0, 12},
    {             "RopeWalk",   7,   0,  1,  0,  2},
    {             "RopeBend",   9,   2,  1,  0,  2},
    {         "RopeHangBend",  12,   5,  1,  0,  0},
    {           "RopeHangUp",  12,   5,  1,  0,  0},
    {          "RopeBalance", 255, 255, 12, 57, 12},
    {          "SetSekibanA",  13,   6,  1,  0,  0},
    {          "SetSekibanB",  13,   6,  1,  0,  0},
    {          "SetSekibanC",  13,   6,  1,  0,  0},
    {            "pushTSWst",   7,   0,  1,  0,  2},
    {              "pushTSW",   7,   0,  1,  0,  2},
    {              "PushLog",   7,   0,  1,  0,  2},
    {        "AsuraCutRight",  12,   5, 45,  0, 45},
    {         "AsuraCutLeft",  12,   5, 45,  0, 45},
    {           "AsuraCutUp",  12,   5, 45,  0, 45},
    {      "AsuraCutRightSt",  12,   5, 46,  0, 46},
    {       "AsuraCutLeftSt",  12,   5, 46,  0, 46},
    {         "AsuraCutUpSt",  12,   5, 46,  0, 46},
    {      "AsuraCutRightEd",  12,   5, 47,  0, 47},
    {       "AsuraCutLeftEd",  12,   5, 47,  0, 47},
    {         "AsuraCutUpEd",  12,   5, 47,  0, 47},
    {       "AsuraSwordWait",  12,   5,  1,  0,  2},
    {       "AsuraSwordLift",  12,   5,  1,  0,  2},
    {             "BinScoop",  13,   5,  1,  0,  0},
    {                 "Fire",   9,   2,  1,  0,  2},
    {               "crouch",  13,   6,  1,  0,  2},
    {           "CannonWait",  13,   6, 48, 48, 48},
    {                 "Seal",  13,   6,  1,  0,  2},
    {             "SealLoop",  13,   6,  1,  0,  2},
    {           "SealFinish",  13,   6,  1,  0,  2},
    {             "FinishEd",  13, 100,  1,  0,  2},
    {              "SitDown",  13,   6,  1,  0,  0},
    {              "SitWait",  13,   6,  1,  0,  0},
    {      "SitWaitServiceA",  13,   6, 52, 52, 52},
    {             "roofwait",  12,   5,  1,  0,  0},
    {             "roofhang",  12,   5,  1,  0,  0},
    {           "MgStruggle",   9,  52, 11,  0, 11},
    {       "MgStruggle_act",   9,  52,  1,  0,  2},
    {    "MgStruggle_Escape",   9,  52,  1,  0,  2},
    {     "KrakenAppearance",   9,   2, 49, 49, 49},
    {               "MgBury",   7,   0,  1,  0,  0},
    {            "GetFlower",   7,   0, 50, 50, 50},
    {        "DoorLargeOpen",  13,   6,  1,  0,  0},
    {       "DoorLargeClose",   7,   0,  1,  0,  0},
    {       "HarpPlayFinish",   7,   0,  1,  0,  0},
    {           "LightningL",  13,   4, 53, 53, 53},
    {           "LightningR",  13,   4, 53, 53, 53},
    {           "LastAttack",   8, 100, 54, 54, 54},
    {                   "at", 255, 255, 57, 57,  2},
    {                 "take", 255, 255, 57, 57, 57},
    {                "takeL", 255, 255, 57, 57, 57},
    {                "takeR", 255, 255, 57, 57, 57},
    {                "Take2", 255, 255, 57, 57, 57},
    {             "waitAtoS",   9,   1, 57, 57,  2},
    {                 "damD", 255,  56,  6, 57,  6},
    {           "Ashootwait",  13,   1, 57, 57,  2},
    {              "Arelord",   8,   3, 57, 57,  2},
    {               "Ashoot",   8,   3, 57, 57,  2},
    {          "ArelordTame",   8,   3, 57, 57,  2},
    {           "Ashootwait",  13,   1, 57, 57,  2},
    {              "Arelord",   8,   3, 57, 57,  2},
    {               "Ashoot",   8,   3, 57, 57,  2},
    {                "damSw",  13,   6,  6, 57,  6},
    {               "HSwait",   8,   1, 57, 57,  2},
    {           "VacuumWait",  12,   5, 57, 57, 57},
    {          "MPPoseFront",  13, 255, 57, 57,  2},
    {         "MPPoseAround",  13, 255, 57, 57, 57},
    {     "MPPoseAroundBack",  13, 255, 57, 57,  2},
    {      "MPPoseDashFront",  13, 255, 57, 57,  2},
    {     "MPPoseDashAround",  13, 255, 57, 57, 57},
    { "MPPoseDashAroundBack",  13, 255, 57, 57,  2},
    {           "LiftB_UtoD",   7,   0, 57, 57,  2},
    {           "LiftS_UtoD",   7,   0, 57, 57, 57},
    {            "LiftLWalk",   7,   0, 57, 57,  2},
    {          "MPGuardPose", 255, 255, 57, 57, 57},
    {             "RockRide", 255,  56, 12, 57, 12},
    {           "BinDrinkSt", 255,   5, 13, 13, 13},
    {             "BinDrink", 255,   5, 14, 57, 14},
    {           "BinDrinkEd", 255,   5, 15, 57, 15},
    {             "HarpPose",   7,   6,  1,  0,  0},
    {           "HarpPlayGo",   7,   1,  1,  0,  0},
    {         "HarpPlayCome",   7,   0,  1,  0,  0},
    {              "BinHold", 255,   5, 57, 57, 57},
    {            "BinShield",   9,   5, 24, 24, 24},
};
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
    SoundSource *sound = player->getSoundSource();
    if (sound->isReadyMaybe()) {
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
    // TODO fix char constness
    allocator->createNewTempFrmHeap(size, dHeap::work1Heap.heap, (char *)name, 0x20, 0);
    new u8[size]();
    allocator->adjustFrmHeapRestoreCurrent();
}

void daPlayerModelBase_c::allocExternalDataBuffers() {
    mpExternalAnmCharBuffer = heap_allocator.alloc(0x2400 * 6);
    mpAnmCharBuffer = heap_allocator.alloc(0x1000);
    mpTexPatBuffer = heap_allocator.alloc(0x1000);
    mpTexSrtBuffer = heap_allocator.alloc(0x1000);
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
    anm->create(mdl.getResMdl(), mAlink2Res.GetResAnmTexSrt(resName), &heap_allocator, nullptr, 1);
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
    mAnmChrBlend.create(bodyMdl, 6, &heap_allocator);
    mAnmChrBlend.getAnimObj()->SetAnmFlag(nw4r::g3d::AnmObj::FLAG_USE_QUATERNION_ROTATION_BLEND, true);

    daPlBaseAnmChr_c *anms = mAnmChrs;
    // "waitS"
    nw4r::g3d::ResAnmChr resAnmChr26 = getExternalAnmChr(sAnimations[26].animName, mpExternalAnmCharBuffer, 0x2400);
    nw4r::g3d::AnmObjChr *animObj;
    for (s32 i = 0; i < 6; i++) {
        anms->create2(bodyMdl, resAnmChr26, &heap_allocator);
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
        mFaceMdl.getResMdl(), getExternalAnmChr(sFaceResNames[0], mpAnmCharBuffer, 0x1000), &heap_allocator, nullptr
    );
    mFaceMdl.setAnm(mFaceAnmChr);
    mFaceAnmChrIdx1 = 0;
    mFaceAnmChrIdx2 = 57; // invalid?

    // "Fmaba01"
    mFaceTexPat.create(
        mFaceMdl.getResMdl(), getExternalAnmTexPat(sFaceResNames[1], mpTexPatBuffer, 0x1000), &heap_allocator, nullptr,
        1
    );
    mFaceMdl.setAnm(mFaceTexPat);
    mFaceAnmTexPatIdx1 = 1;
    mFaceAnmTexPatIdx2 = 57; // invalid

    bool isInTrial = dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL;
    mFaceTexSrt.create(
        mFaceMdl.getResMdl(), getExternalAnmTexSrt(sFaceResNames[0], mpTexSrtBuffer, 0x1000), &heap_allocator, nullptr,
        isInTrial ? 2 : 1
    );
    mFaceMdl.setAnm(mFaceTexSrt);
    mFaceAnmTexSrtIdx1 = 0;
    mFaceAnmTexSrtIdx2 = 57; // invalid

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
    work.YrotM(rotation.y);
    if (order) {
        work.ZYXrotM(x, y, z);
    } else {
        work.ZXYrotM(x, y, z);
    }
    work.YrotM(-rotation.y);
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
        mtx.YrotM(-rotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(rotation.y);
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
        mtx.YrotM(-rotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(rotation.y);
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
        mtx.YrotM(-rotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(rotation.y);
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
                v1 *= -1;
                v2 *= -1;
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
        mAng oldYRot = rotation.y;
        rotation.y = field_0x1256;
        u32 idx = nodeId - 1;
        applyWorldRotationMaybe(result, field_0x1238[idx], 0, field_0x1242[idx], nullptr, false);
        rotation.y = oldYRot;
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
        prev = 1;
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
        prev = 0;
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
        prev = 0;
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
    SoundSource *s = getSoundSource();
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
    SoundSource *s = getSoundSource();
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

    mMainMdl.getNodeWorldMtxMultVec(PLAYER_MAIN_NODE_HEAD, sPos1, poscopy2);
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
        v.rotY(rotation.y + field_0x126A);
        poscopy2 += v;
    }
}

void daPlayerModelBase_c::setPosCopy3() {
    static const Vec posCopy3v0 = {0.0f, 150.0f, 0.0f};
    static const Vec posCopy3v1 = {0.0f, 40.0f, 0.0f};
    static const Vec posCopy3v2 = {0.0f, 95.0f, 0.0f};
    static const Vec posCopy3v3 = {0.0f, 14.0f, 0.0f};
    if (checkCurrentAction(/* FREE_FALL*/ 0x13) || checkCurrentAction(/* WALKING_ON_TIGHTROPE */ 0x81)) {
        poscopy3 = position;
        poscopy3.y += 180.0f;
    } else if (checkCurrentAction(/* BEING_PULLED_BY_CLAWS */ 0x5A)) {
        poscopy3 = poscopy2;
    } else if (checkActionFlags(FLG0_SWING_ROPE)) {
        poscopy3 = position;
    } else if (checkActionFlagsCont(0x20000000) || checkCurrentAction(0x70) ||
               checkCurrentAction(/* HANG_ON_ZIP */ 0x85)) {
        poscopy3 = position;
        poscopy3.y -= 100.0f;
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
                poscopy3.x = c->_03;
                poscopy3.y = c->_13;
                poscopy3.z = c->_23;
                break;
            }
            case 0x09: {
                // SLIDING
                poscopy3 = position + mVec3_c(posCopy3v2);
                break;
            }
            default: {
                mMtx_c mtx;
                mMainMdl.getLocalMtx(mtx);

                f32 f;
                const Vec *v;
                if (checkActionFlags(FLG0_IN_WATER) || checkCurrentAction(/* SWIM_DASH_INFO_AIR */ 0x57)) {
                    v = &posCopy3v3;
                    f = position.y;
                } else {
                    if (checkCurrentAction(/* VOID_SAND */ 0x4C)) {
                        v = &posCopy3v3;
                    } else {
                        v = checkActionFlags(FLG0_CRAWLING) ? &posCopy3v1 : &posCopy3v0;
                    }
                    f = mtx.m[1][3];
                }
                poscopy3.copyFrom(v);
                poscopy3.x += position.x;
                poscopy3.y += f;
                poscopy3.z += position.z;
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
        mCarriedActorRef.get()->setObjectProperty(0x200);
    }
}
