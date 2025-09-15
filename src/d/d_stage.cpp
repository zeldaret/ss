#include "d/d_stage.h"

#include "common.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_base.h"
#include "d/d_bzs_types.h"
#include "d/d_fader.h"
#include "d/d_room.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_sc_title.h"
#include "d/d_stage_mgr.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/snd/d_snd_source_mgr.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "m/m_fader_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_types.h"
#include "s/s_FPhase.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/time_area_mgr.h"

#include <cstring>

SPECIAL_BASE_PROFILE(STAGE, dStage_c, fProfile::STAGE, 6, 0x4E0);

sFPhase<dStage_c>::phaseCallback dStage_c::sCallbacks[] = {
    &dStage_c::createRooms,
    &dStage_c::waitForRooms,
    nullptr,
};

extern "C" void fn_8001BE60(f32, f32);
extern "C" void *SCRAPPER_PTR;

dStage_c::dStage_c() : mPhase(this, sCallbacks) {
    memset(loaded_entities, 0, sizeof(loaded_entities));
    sInstance = this;
    dScGame_c::resetUpdateFrameCount();
    SCRAPPER_PTR = nullptr;
    fn_8001BE60(2000.0f, 1700.0f);
}

extern "C" void *SCRAPPER_PICKUP_TARGET;
extern "C" void ScrapperPickupMgr__update(void *);
extern "C" void ScrapperPickupMgr__dtor();
extern "C" void fn_80028A80();
extern "C" void fn_80028EC0();
extern "C" void setPCAMpos2(const PCAM *);

int dStage_c::create() {
    if (mPhase.step() != sFPhaseBase::PHASE_ALL_DONE) {
        return NOT_READY;
    }

    dStageMgr_c *stageMgr = dStageMgr_c::GetInstance();
    const PCAM *pcam = stageMgr->getPcam();
    for (int i = 0; i < stageMgr->getPcamCount(); i++) {
        setPCAMpos2(pcam);
        pcam++;
    }

    if (mapRelated.field_0x1EB == 0) {
        mVec3_c min, max;
        for (s32 i = 0; i < MAX_ROOM_NUMBER - 1; i++) {
            dRoom_c *room = getRoom(i);
            if (room != nullptr) {
                mVec3_c tmin, tmax;
                room->getBounds(&tmin, &tmax);
                if (i == 0) {
                    min = tmin;
                    max = tmax;
                } else {
                    nw4r::math::VEC3Minimize(min, min, tmin);
                    nw4r::math::VEC3Maximize(max, max, tmax);
                }
            }
        }
        max.y += 6000.0f;
        mVec3_c size = max - min;
        mVec3_c center = mVec3_c(min.x + size.x * 0.5f, min.y, min.z + size.z * 0.5f);
        mapRelated.fn_801B4B80(0, center, size);
        mVec3_c v = mVec3_c(10000.0f, 10000.0f, 100000.0f);
        mapRelated.fn_801B4C70(v);
    }
    mapRelated.fn_801B50C0(0);
    fn_80028A80();

    return SUCCEEDED;
}

int dStage_c::doDelete() {
    fn_80028EC0();
    dRumble_c::stop(-1);
    ScrapperPickupMgr__dtor();
    return SUCCEEDED;
}

int dStage_c::execute() {
    mFader.calc();
    ScrapperPickupMgr__update(SCRAPPER_PICKUP_TARGET);
    return SUCCEEDED;
}

extern "C" void resetViewClipVals();

int dStage_c::draw() {
    resetViewClipVals();
    return SUCCEEDED;
}

void dStage_c::deleteReady() {}

sFPhaseBase::sFPhaseState dStage_c::createRooms() {
    // TODO
    // if (dScGame_c::sCo)
    mFader.create();
    mFader.setTypes(dFader_c::FADER_GREY, dFader_c::FADER_GREY);
    mFader.setFadeInFrame(1);
    mFader.fadeIn();
    mFader.calc();
    mFader.setFrames(15, 15);
    mFader.initForLyt();
    // TODO

    if (dScTitle_c::sInstance != nullptr) {
        dBase_c::createBase(fProfile::TITLE, this, 0, OTHER);
    }

    dTimeAreaMgr_c::GetInstance()->setField0x78(false);

    return sFPhaseBase::PHASE_NEXT;
}

sFPhaseBase::sFPhaseState dStage_c::waitForRooms() {
    if (checkChildProcessCreateState()) {
        return sFPhaseBase::PHASE_RETRY;
    }
    return sFPhaseBase::PHASE_NEXT;
}

void dStage_c::setRoom(int roomid, dRoom_c *room) {
    rooms.setRoom(roomid, room);
}

dRoom_c *dStage_c::getRoom(s32 idx) {
    return rooms.getRoom(idx);
}

bool dStage_c::fadeIn(s32 fadeType, u16 frames) {
    if (mFader.getStatus() != mFaderBase_c::FADED_OUT) {
        return false;
    }
    mFader.setFadeInType(fadeType);
    mFader.setFadeInFrame(frames);
    mFader.fadeIn();
    dSndSourceMgr_c::GetInstance()->setMutedFromFader(false);
    return true;
}

bool dStage_c::fadeOut(s32 fadeType, u16 frames) {
    if (mFader.getStatus() != mFaderBase_c::FADED_IN) {
        return false;
    }
    mFader.setFadeOutType(fadeType);
    mFader.setFadeOutFrame(frames);
    mFader.fadeOut();
    dSndSourceMgr_c::GetInstance()->setMutedFromFader(true);
    return true;
}

void dStage_c::forceFadeOut() {
    mFader.setStatus(mFaderBase_c::FADED_OUT);
    dSndSourceMgr_c::GetInstance()->setMutedFromFader(true);
}

void dStage_c::forceFadeIn() {
    mFader.setStatus(mFaderBase_c::FADED_IN);
    dSndSourceMgr_c::GetInstance()->setMutedFromFader(false);
}

void dStage_c::drawMap(mMtx_c *mtx, int param) {
    dBgS::GetInstance()->UpdateScrollTex();
    for (s32 i = 0; i < MAX_ROOM_NUMBER - 1; i++) {
        dRoom_c *room = getRoom(i);
        if (room != nullptr) {
            room->drawOnMapIfVisible(mtx, param);
        }
    }
    dBgS::GetInstance()->DrawSkyKeepMap(mtx, param);
}

static void getFlagPositionForRoomVisit(int roomid, u16 *o_flag, u16 *o_shift) {
    *o_flag = (roomid / 16) + 8;
    *o_shift = 1 << (roomid % 16);
}

bool dStage_c::hasVisitedRoom(int roomid) const {
    u16 flag, shift;
    getFlagPositionForRoomVisit(roomid, &flag, &shift);
    DungeonflagManager *mg = DungeonflagManager::sInstance;
    return (mg->getCounterOrFlag(flag, 8) & shift) != 0;
}

void dStage_c::setRoomVisited(int roomid) {
    // TODO this is probably an inline but inline bools cause extra instructions
    if (EventManager::getCurrentEventName() != nullptr &&
        std::strncmp(EventManager::getCurrentEventName(), "Save", 0x20) == 0) {
        return;
    }

    if (dScGame_c::sInstance == nullptr || dScGame_c::GetInstance()->profile_name == fProfile::GAME) {
        u16 flag, shift;
        getFlagPositionForRoomVisit(roomid, &flag, &shift);
        DungeonflagManager *mg = DungeonflagManager::sInstance;
        u16 newVal = shift | mg->getCounterOrFlag(flag, 8);
        mg->setToValue(flag, newVal);
    }
}

dRoom_c *dStage_c::createRoom(int roomid, bool flag0x40) {
    return static_cast<dRoom_c *>(
        dBase_c::createBase(fProfile::ROOM, this, (roomid & 0x3F) | (flag0x40 ? 0x40 : 0), OTHER)
    );
}

void dStage_c::createLytSeekerStone() {
    dBase_c::createBase(fProfile::LYT_CONTROL_GAME, this, 0, OTHER);
    if (LayoutArcManager::GetInstance()->hasEntry("SeekerStone")) {
        dBase_c::createBase(fProfile::LYT_SEEKER_STONE, this, 0, OTHER);
    }
}

dBase_c *dStage_c::getParentForRoom(s32 idx) {
    if (idx == -1) {
        return dStage_c::GetInstance();
    }
    return dStage_c::GetInstance()->getRoom(idx);
}

void dStage_c::bindStageResToFile(nw4r::g3d::ResFile *file) {
    nw4r::g3d::ResFile f = nw4r::g3d::ResFile(CurrentStageArcManager::GetInstance()->getData("g3d/stage.brres"));
    if (f.IsValid()) {
        file->Bind(f);
    }
}

void dStage_c::bindSkyCmnToResFile(nw4r::g3d::ResFile *file) {
    nw4r::g3d::ResFile f = nw4r::g3d::ResFile(OarcManager::GetInstance()->getSubEntryData("SkyCmn", "g3d/model.brres"));
    if (f.IsValid()) {
        file->Bind(f);
    }
}

static const char *sMatsToTry[] = {
    "MA01",
    "MA02",
    "MA04",
};

bool dStage_c::getMA0AnmTexSrt(nw4r::g3d::ResAnmTexSrt *o_srt, const char *matName) {
    const char **candidate = sMatsToTry;
    for (int i = 0; i < ARRAY_LENGTH(sMatsToTry); i++, candidate++) {
        if (!std::strncmp(matName, *candidate, std::strlen(*candidate))) {
            nw4r::g3d::ResFile mdl = nw4r::g3d::ResFile(OarcManager::GetInstance()->getMdlFromArc2("Common"));
            *o_srt = mdl.GetResAnmTexSrt(*candidate);
            return true;
        }
    }
    return false;
}

bool dStage_c::getMA0IndirectSrt(nw4r::g3d::ResAnmTexSrt *o_srt, const nw4r::g3d::ResMdl &mdl) {
    for (u32 i = 0; i < mdl.GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat = mdl.GetResMat(i);
        const char *name = mat.GetName();
        if (getMA0AnmTexSrt(o_srt, name)) {
            return true;
        }
    }
    return false;
}

dRoomTable_c::dRoomTable_c() {
    // Okay
    mRooms[0] = nullptr;
    mRooms[1] = nullptr;
    mRooms[2] = nullptr;
    mRooms[3] = nullptr;
    mRooms[4] = nullptr;
    mRooms[5] = nullptr;
    mRooms[6] = nullptr;
    mRooms[7] = nullptr;
    mRooms[8] = nullptr;
    mRooms[9] = nullptr;
    mRooms[10] = nullptr;
    mRooms[11] = nullptr;
    mRooms[12] = nullptr;
    mRooms[13] = nullptr;
    mRooms[14] = nullptr;
    mRooms[15] = nullptr;
    mRooms[16] = nullptr;
    mRooms[17] = nullptr;
    mRooms[18] = nullptr;
    mRooms[19] = nullptr;
    mRooms[20] = nullptr;
    mRooms[21] = nullptr;
    mRooms[22] = nullptr;
    mRooms[23] = nullptr;
    mRooms[24] = nullptr;
    mRooms[25] = nullptr;
    mRooms[26] = nullptr;
    mRooms[27] = nullptr;
    mRooms[28] = nullptr;
    mRooms[29] = nullptr;
    mRooms[30] = nullptr;
    mRooms[31] = nullptr;
    mRooms[32] = nullptr;
    mRooms[33] = nullptr;
    mRooms[34] = nullptr;
    mRooms[35] = nullptr;
    mRooms[36] = nullptr;
    mRooms[37] = nullptr;
    mRooms[38] = nullptr;
    mRooms[39] = nullptr;
    mRooms[40] = nullptr;
    mRooms[41] = nullptr;
    mRooms[42] = nullptr;
    mRooms[43] = nullptr;
    mRooms[44] = nullptr;
    mRooms[45] = nullptr;
    mRooms[46] = nullptr;
    mRooms[47] = nullptr;
    mRooms[48] = nullptr;
    mRooms[49] = nullptr;
    mRooms[50] = nullptr;
    mRooms[51] = nullptr;
    mRooms[52] = nullptr;
    mRooms[53] = nullptr;
    mRooms[54] = nullptr;
    mRooms[55] = nullptr;
    mRooms[56] = nullptr;
    mRooms[57] = nullptr;
    mRooms[58] = nullptr;
    mRooms[59] = nullptr;
    mRooms[60] = nullptr;
    mRooms[61] = nullptr;
    mRooms[62] = nullptr;
}

MapRelated::MapRelated()
    : field_0x0FC(0.0f, 0.0f, 0.0f),
      field_0x108(0.0f),
      field_0x10C(0.0f),
      field_0x110(0.0f),
      field_0x114(0.0f),
      field_0x118(0.0f),
      field_0x11C(0.0f),
      field_0x120(0.0f),
      field_0x124(0.0f),
      field_0x128(0.0f),
      field_0x12C(0.0f),
      field_0x1E0(0),
      field_0x1E6(0),
      field_0x1E8(0),
      field_0x1E9(4),
      field_0x1EA(-3),
      field_0x1EB(0),
      field_0x1EC(0),
      field_0x1ED(0),
      field_0x1EE(0),
      field_0x1F0(0) {
    mPostEffect.setField0x30(0.15f);
    bool showPastVariant = true;
    bool showDesertPast = dScGame_c::isCurrentStage("S300") ||
                          ((dScGame_c::isCurrentStage("F300") || dScGame_c::isCurrentStage("F300_4")) &&
                           SceneflagManager::sInstance->checkTempOrSceneflag(3));
    if (!showDesertPast) {
        bool hideSeaPast =
            !(dScGame_c::isCurrentStage("F301_1") &&
              StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_SEA_CHART_ACTIVATED));
        if (hideSeaPast) {
            showPastVariant = false;
        }
    }
    field_0x1EF = showPastVariant;
}

extern "C" void fn_801B4AE0();
MapRelated::~MapRelated() {
    fn_801B4AE0();
}
