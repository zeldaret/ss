#include "common.h"
#include "d/snd/d_snd_event.h"
#include "d/snd/d_snd_stage_data.h"
#include "d/snd/d_snd_state_mgr.h"

void todoInititalizeSndEvent();
void todoExecuteSndEvent();
void todoFinalizeSndEvent();

#pragma push
#pragma readonly_strings on
// clang-format off

static const SndEventDef sSndEventDefs_GLOBAL[] = {
    {             "DefaultStart",              SND_EVENT_DefaultStart, todoInititalizeSndEvent,             nullptr,              nullptr},
    {             "SkyDiveStart",              SND_EVENT_SkyDiveStart, todoInititalizeSndEvent,             nullptr,              nullptr},
    {           "ShortDiveStart",            SND_EVENT_ShortDiveStart, todoInititalizeSndEvent,             nullptr,              nullptr},
    {           "ParachuteStart",            SND_EVENT_ParachuteStart, todoInititalizeSndEvent,             nullptr,              nullptr},
    {           "SirenBackStart",            SND_EVENT_SirenBackStart, todoInititalizeSndEvent,             nullptr,              nullptr},
    {               "TimeDoorIn",                SND_EVENT_TimeDoorIn,                 nullptr, todoExecuteSndEvent,              nullptr},
    {              "TimeDoorOut",               SND_EVENT_TimeDoorOut, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {            "ReturnSkyloft",             SND_EVENT_ReturnSkyloft, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                  "Shutter",                   SND_EVENT_Shutter, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {                  "DayTime",                   SND_EVENT_DayTime, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                 "BedStart",                  SND_EVENT_BedStart, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                 "TboxShow",                  SND_EVENT_TboxShow, todoInititalizeSndEvent,             nullptr,              nullptr},
    {          "TreasureBoxOpen",           SND_EVENT_TreasureBoxOpen, todoInititalizeSndEvent,             nullptr,              nullptr},
    {      "TreasureBoxOpenBoss",       SND_EVENT_TreasureBoxOpenBoss, todoInititalizeSndEvent,             nullptr,              nullptr},
    {            "FaysSirenSong",             SND_EVENT_FaysSirenSong, todoInititalizeSndEvent,             nullptr,              nullptr},
    {      "fays_song_fin_siren",       SND_EVENT_fays_song_fin_siren, todoInititalizeSndEvent,             nullptr,              nullptr},
    {             "SDGateInOpen",              SND_EVENT_SDGateInOpen, todoInititalizeSndEvent,             nullptr,              nullptr},
    {           "SealedDoorOpen",            SND_EVENT_SealedDoorOpen, todoInititalizeSndEvent,             nullptr,              nullptr},
    {          "SealedDoorClose",           SND_EVENT_SealedDoorClose, todoInititalizeSndEvent,             nullptr,              nullptr},
    {           "HeartContainer",            SND_EVENT_HeartContainer, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {     "ordinary_sword_sprit",      SND_EVENT_ordinary_sword_sprit, todoInititalizeSndEvent,             nullptr,              nullptr},
    {            "fays_navigate",             SND_EVENT_fays_navigate, todoInititalizeSndEvent,             nullptr,              nullptr},
    {              "fays_allert",               SND_EVENT_fays_allert, todoInititalizeSndEvent,             nullptr,              nullptr},
    {           "DefaultGetItem",            SND_EVENT_DefaultGetItem, todoInititalizeSndEvent,             nullptr,              nullptr},
    {          "ItemGetGorgeous",           SND_EVENT_ItemGetGorgeous, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                     "Save",                      SND_EVENT_Save, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                "SwordDraw",                 SND_EVENT_SwordDraw, todoInititalizeSndEvent,             nullptr,              nullptr},
    {         "SwordDrawDoorNew",          SND_EVENT_SwordDrawDoorNew, todoInititalizeSndEvent,             nullptr,              nullptr},
    {             "DoorBossGame",              SND_EVENT_DoorBossGame, todoInititalizeSndEvent,             nullptr,              nullptr},
    {              "FallRestart",               SND_EVENT_FallRestart, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                "DeadMagma",                 SND_EVENT_DeadMagma, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                 "DeadSink",                  SND_EVENT_DeadSink, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                     "Dead",                      SND_EVENT_Dead, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                 "DeadDive",                  SND_EVENT_DeadDive,                 nullptr, todoExecuteSndEvent,              nullptr},
    {             "SirenChaser2",              SND_EVENT_SirenChaser2,                 nullptr,             nullptr, todoFinalizeSndEvent},
    {                     "JMAP",                      SND_EVENT_JMAP, todoInititalizeSndEvent,             nullptr,              nullptr},
    {              "JMAPAllMove",               SND_EVENT_JMAPAllMove, todoInititalizeSndEvent,             nullptr,              nullptr},
    {            "OracleInSiren",             SND_EVENT_OracleInSiren, todoInititalizeSndEvent,             nullptr,              nullptr},
    {    "HarpPerformanceResult",     SND_EVENT_HarpPerformanceResult,                 nullptr, todoExecuteSndEvent,              nullptr},
    {               "F102Intro2",                SND_EVENT_F102Intro2, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {                       "Uo",                        SND_EVENT_Uo, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {            "WaterfallStop",             SND_EVENT_WaterfallStop,                 nullptr,             nullptr, todoFinalizeSndEvent},
    {               "F103Intro2",                SND_EVENT_F103Intro2, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {                      "Onp",                       SND_EVENT_Onp, todoInititalizeSndEvent, todoExecuteSndEvent, todoFinalizeSndEvent},
    {                "F103_UoMS",                 SND_EVENT_F103_UoMS, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {           "F200IntroByFay",            SND_EVENT_F200IntroByFay, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {            "F202_R02Intro",             SND_EVENT_F202_R02Intro, todoInititalizeSndEvent,             nullptr,              nullptr},
    {          "SearchLightDemo",           SND_EVENT_SearchLightDemo,                 nullptr,             nullptr,              nullptr},
    {         "F300_1IntroByFay",          SND_EVENT_F300_1IntroByFay, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {            "FirstTimeSlip",             SND_EVENT_FirstTimeSlip, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {           "F301IntroByFay",            SND_EVENT_F301IntroByFay, todoInititalizeSndEvent, todoExecuteSndEvent, todoFinalizeSndEvent},
    {        "F302RevivalRairyu",         SND_EVENT_F302RevivalRairyu, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {         "F402L3QuakeStart",          SND_EVENT_F402L3QuakeStart, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {"Girahimu3SecondAppearDemo", SND_EVENT_Girahimu3SecondAppearDemo,                 nullptr, todoExecuteSndEvent, todoFinalizeSndEvent},
    {                 "SF_start",                  SND_EVENT_SF_start, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {        "GirahimuSwordDemo",         SND_EVENT_GirahimuSwordDemo,                 nullptr, todoExecuteSndEvent,              nullptr},
    {               "AsuraStart",                SND_EVENT_AsuraStart, todoInititalizeSndEvent, todoExecuteSndEvent, todoFinalizeSndEvent},
    {              "AsuraStartB",               SND_EVENT_AsuraStartB, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {        "RollRockLspAppear",         SND_EVENT_RollRockLspAppear, todoInititalizeSndEvent,             nullptr,              nullptr},
    {       "RollRockChangeVera",        SND_EVENT_RollRockChangeVera, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                 "MG_start",                  SND_EVENT_MG_start, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {                "BLS_demo2",                 SND_EVENT_BLS_demo2, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {                  "BLS_end",                   SND_EVENT_BLS_end, todoInititalizeSndEvent,             nullptr,              nullptr},
    {       "ECaptain_c_Opening",        SND_EVENT_ECaptain_c_Opening, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {                "D003Intro",                 SND_EVENT_D003Intro, todoInititalizeSndEvent,             nullptr,              nullptr},
    {  "ECaptain_c_OpeningType2",   SND_EVENT_ECaptain_c_OpeningType2, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {                  "SF4_End",                   SND_EVENT_SF4_End,                 nullptr,             nullptr,              nullptr},
    {             "TriforceWarp",              SND_EVENT_TriforceWarp,                 nullptr, todoExecuteSndEvent,              nullptr},
};

static const SndEventDef sSndEventDefs_F000[] = {
    {"CeremonyByRescueTeam", SND_EVENT_CeremonyByRescueTeam, todoInititalizeSndEvent,             nullptr,              nullptr},
    {       "BirdRaceStart",        SND_EVENT_BirdRaceStart,                 nullptr,             nullptr, todoFinalizeSndEvent},
    {     "GoddessGateOpen",      SND_EVENT_GoddessGateOpen,                 nullptr, todoExecuteSndEvent,              nullptr},
    {      "FaysPharosSong",       SND_EVENT_FaysPharosSong, todoInititalizeSndEvent,             nullptr,              nullptr},
    {    "fays_song_pharos",     SND_EVENT_fays_song_pharos, todoInititalizeSndEvent,             nullptr,              nullptr},
    {"fays_song_fin_pharos", SND_EVENT_fays_song_fin_pharos,                 nullptr,             nullptr, todoFinalizeSndEvent},
};

static const SndEventDef sSndEventDefs_F001r[] = {
    {"OracleInSiren", SND_EVENT_OracleInSiren, todoInititalizeSndEvent, nullptr, nullptr},
};

static const SndEventDef sSndEventDefs_F020[] = {
    {                  "ItemGetBird",                   SND_EVENT_ItemGetBird, todoInititalizeSndEvent,             nullptr,              nullptr},
    {                "DivingMGStart",                 SND_EVENT_DivingMGStart, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {               "DiveGameResult",                SND_EVENT_DiveGameResult, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {            "StartCarryPumpkin",             SND_EVENT_StartCarryPumpkin, todoInititalizeSndEvent,             nullptr,              nullptr},
    {    "CarryPumpkinResultsFailed",     SND_EVENT_CarryPumpkinResultsFailed, todoInititalizeSndEvent,             nullptr,              nullptr},
    {"CarryPumpkinResultsSuccessful", SND_EVENT_CarryPumpkinResultsSuccessful, todoInititalizeSndEvent,             nullptr,              nullptr},
};

static const SndEventDef sSndEventDefs_F023[] = {
    {     "NusiAppearDemo",      SND_EVENT_NusiAppearDemo, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {"NusiTHideAppearDemo", SND_EVENT_NusiTHideAppearDemo, todoInititalizeSndEvent,             nullptr,              nullptr},
    {      "TenAppearDemo",       SND_EVENT_TenAppearDemo, todoInititalizeSndEvent, todoExecuteSndEvent, todoFinalizeSndEvent},
    {     "NusiTDeathDemo",      SND_EVENT_NusiTDeathDemo, todoInititalizeSndEvent,             nullptr,              nullptr},
    {      "NusiDeathDemo",       SND_EVENT_NusiDeathDemo, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
};

static const SndEventDef sSndEventDefs_F100[] = {
    {"F100IntroByFay", SND_EVENT_F100IntroByFay, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {   "BC_wakidasi",    SND_EVENT_BC_wakidasi, todoInititalizeSndEvent,             nullptr,              nullptr},
    {     "Kyui3Fall",      SND_EVENT_Kyui3Fall,                 nullptr,             nullptr, todoFinalizeSndEvent},
};

static const SndEventDef sSndEventDefs_F401[] = {
    {"F401IntroByFay", SND_EVENT_F401IntroByFay, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {   "BBossAppear",    SND_EVENT_BBossAppear, todoInititalizeSndEvent, todoExecuteSndEvent, todoFinalizeSndEvent},
    {     "BBFLYDEMO",      SND_EVENT_BBFLYDEMO, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {     "BBossEvac",      SND_EVENT_BBossEvac, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {    "BBoss2Evac",     SND_EVENT_BBoss2Evac, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {    "BBoss3Evac",     SND_EVENT_BBoss3Evac, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {       "BBFuuin",        SND_EVENT_BBFuuin, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {        "BBHDTH",         SND_EVENT_BBHDTH,                 nullptr,             nullptr,              nullptr},
    {        "BBHDRC",         SND_EVENT_BBHDRC,                 nullptr,             nullptr,              nullptr},
    {         "BBFCL",          SND_EVENT_BBFCL,                 nullptr,             nullptr,              nullptr},
    {        "BBTUTO",         SND_EVENT_BBTUTO, todoInititalizeSndEvent,             nullptr,              nullptr},
};

static const SndEventDef sSndEventDefs_D101[] = {
    {     "SF4_Op",      SND_EVENT_SF4_Op, todoInititalizeSndEvent, nullptr, todoFinalizeSndEvent},
    {    "SF4_End",     SND_EVENT_SF4_End, todoInititalizeSndEvent, nullptr,              nullptr},
    {"JMAPAllMove", SND_EVENT_JMAPAllMove, todoInititalizeSndEvent, nullptr,              nullptr},
};

static const SndEventDef sSndEventDefs_D200[] = {
    {"ELizarufos_c_opening", SND_EVENT_ELizarufos_c_opening, todoInititalizeSndEvent, nullptr, todoFinalizeSndEvent},
    {         "D200R04Rock",          SND_EVENT_D200R04Rock, todoInititalizeSndEvent, nullptr,              nullptr},
    {     "RollRockLspJump",      SND_EVENT_RollRockLspJump, todoInititalizeSndEvent, nullptr,              nullptr},
    {                "JMAP",                 SND_EVENT_JMAP, todoInititalizeSndEvent, nullptr,              nullptr},
};

static const SndEventDef sSndEventDefs_D201[] = {
    {  "MoleInitFear",   SND_EVENT_MoleInitFear, todoInititalizeSndEvent, todoExecuteSndEvent,              nullptr},
    {"MolePushSwitch", SND_EVENT_MolePushSwitch, todoInititalizeSndEvent,             nullptr,              nullptr},
    {  "MoleInitTalk",   SND_EVENT_MoleInitTalk, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {     "CatchMole",      SND_EVENT_CatchMole, todoInititalizeSndEvent,             nullptr,              nullptr},
    { "D201IronFence",  SND_EVENT_D201IronFence, todoInititalizeSndEvent,             nullptr,              nullptr},
    {    "MogMolShut",     SND_EVENT_MogMolShut,                 nullptr,             nullptr, todoFinalizeSndEvent},
    {    "MogMolOpen",     SND_EVENT_MogMolOpen, todoInititalizeSndEvent,             nullptr, todoFinalizeSndEvent},
    {     "LavaPlate",      SND_EVENT_LavaPlate, todoInititalizeSndEvent,             nullptr,              nullptr},
    {    "ResqueMole",     SND_EVENT_ResqueMole, todoInititalizeSndEvent,             nullptr,              nullptr},
};

static const SndEventDef sSndEventDefs_B301[] = {
    {    "BKR_start",     SND_EVENT_BKR_start, todoInititalizeSndEvent, todoExecuteSndEvent, todoFinalizeSndEvent},
    {      "BKR_end",       SND_EVENT_BKR_end, todoInititalizeSndEvent, todoExecuteSndEvent, todoFinalizeSndEvent},
    {"BKR_armCaught", SND_EVENT_BKR_armCaught,                 nullptr,             nullptr,              nullptr},
};

static const SndEventDef sSndEventDefs_D003_1[] = {
    {"NeedleUnderground", SND_EVENT_NeedleUnderground, todoInititalizeSndEvent, nullptr, todoFinalizeSndEvent},
};

// clang-format on

#pragma pop

bool dSndStateMgr_c::handleStageEvent(const char *name) {
    if (field_0x08C != SND_EVENT_0x87) {
        const SndEventDef *def = nullptr;
        s32 defCount = 0;

        switch (field_0x044) {
            case SND_STAGE_F000:
                def = sSndEventDefs_F000;
                defCount = ARRAY_LENGTH(sSndEventDefs_F000);
                break;
            case SND_STAGE_F001r:
                def = sSndEventDefs_F001r;
                defCount = ARRAY_LENGTH(sSndEventDefs_F001r);
                break;
            case SND_STAGE_F020:
                def = sSndEventDefs_F020;
                defCount = ARRAY_LENGTH(sSndEventDefs_F020);
                break;
            case SND_STAGE_F023:
                def = sSndEventDefs_F023;
                defCount = ARRAY_LENGTH(sSndEventDefs_F023);
                break;
            case SND_STAGE_F100:
                def = sSndEventDefs_F100;
                defCount = ARRAY_LENGTH(sSndEventDefs_F100);
                break;
            case SND_STAGE_F401:
                def = sSndEventDefs_F401;
                defCount = ARRAY_LENGTH(sSndEventDefs_F401);
                break;
            case SND_STAGE_D101:
                def = sSndEventDefs_D101;
                defCount = ARRAY_LENGTH(sSndEventDefs_D101);
                break;
            case SND_STAGE_D200:
                def = sSndEventDefs_D200;
                defCount = ARRAY_LENGTH(sSndEventDefs_D200);
                break;
            case SND_STAGE_D201:
                def = sSndEventDefs_D201;
                defCount = ARRAY_LENGTH(sSndEventDefs_D201);
                break;
            case SND_STAGE_B301:
                def = sSndEventDefs_B301;
                defCount = ARRAY_LENGTH(sSndEventDefs_B301);
                break;
            case SND_STAGE_D003_1:
                def = sSndEventDefs_D003_1;
                defCount = ARRAY_LENGTH(sSndEventDefs_D003_1);
                break;
        }

        if (def != nullptr && defCount > 0) {
            for (s32 i = 0; i < defCount; def++, i++) {
                if (streq(name, def->eventName)) {
                    field_0x08C = def->eventId;
                    field_0x234 = def;
                    sEventExecuteCallback = def->executeCb;
                    if (def->initializeCb != nullptr) {
                        (def->initializeCb)();
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool dSndStateMgr_c::handleGlobalEvent(const char *name) {
    if (field_0x08C != SND_EVENT_0x87) {
        for (const SndEventDef *def = sSndEventDefs_GLOBAL;
             def < sSndEventDefs_GLOBAL + ARRAY_LENGTH(sSndEventDefs_GLOBAL); def++) {
            if (streq(name, def->eventName)) {
                field_0x08C = def->eventId;
                field_0x234 = def;
                sEventExecuteCallback = def->executeCb;
                if (def->initializeCb != nullptr) {
                    (def->initializeCb)();
                }
                return true;
            }
        }
    }
    return false;
}
