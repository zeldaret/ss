

#include "toBeSorted/d_camera_game.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_s_roof_chk.h"
#include "d/d_bzs_types.h"
#include "d/d_player_act.h"
#include "d/d_room.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/attention.h"

class dCameraGame_HIO_c {
public:
    dCameraGame_HIO_c();
    virtual ~dCameraGame_HIO_c();

    void create();
    void remove();

public:
    /* 0x004 */ u8 _0x004[0x0E8 - 0x004];

    /* 0x0E8 */ f32 field_0x0E8;

    /* 0x0EC */ u8 _0x0EC[0x138 - 0x0EC];
};

static dCameraGame_HIO_c sHio;
dCameraGame_HIO_c::~dCameraGame_HIO_c() {}

CamParamHeader *dCameraGame_c::sHeader;
f32 *dCameraGame_c::sFloats;
u16 *dCameraGame_c::sFlags;
CamParamStyle *dCameraGame_c::sStyles;
CamParamTrend *dCameraGame_c::sTrends;

s32 dCameraGame_c::sTrendIdxes[CAM_TREND_IDX_MAX];
s32 dCameraGame_c::sStyleIdxes[CAM_STYLE_IDX_MAX];

void debugPrintf4(const char *fmt, ...) {
    // no-op
}

static s32 getCurrentRoomId() {
    return dAcPy_c::GetLinkM()->mRoomID;
}

const CAM *getCam(s32 camId, s32 roomId) {
    if (camId == -1) {
        return nullptr;
    }
    roomId = roomId >= 0 ? roomId : getCurrentRoomId();
    return dStage_c::GetInstance()->getRoom(roomId)->getCamForIndex(camId);
}

const CAM *getCam(const CamId &id) {
    return getCam(id.roomCamId, id.roomId);
}

s32 getCamTrend(const CamId &id) {
    return dCameraGame_c::getTrendIndex(getCam(id)->trendName);
}

s32 dCameraGame_c::getTrendIndex(const char *name) {
    SizedString<32> lName(name);
    for (int i = 0; i < sHeader->numTrends; i++) {
        // MWCC doesn't complain about this one but clangd doesn't like it
        if (lName == sTrends[i].name) {
            return i;
        }
    }
    return 0;
}

s32 dCameraGame_c::getStyleIndex(u32 id) {
    for (int i = 0; i < sHeader->numStyles; i++) {
        // Temp needed... inline?
        s32 otherId = sStyles[i].id;
        if (otherId == id) {
            return i;
        }
    }
    return -1;
}

void dCameraGame_c::cacheIdxes() {
    sTrendIdxes[CAM_TREND_IDX_BalanceBall] = getTrendIndex("BalanceBall");
    sTrendIdxes[CAM_TREND_IDX_Swim] = getTrendIndex("Swim");
    sTrendIdxes[CAM_TREND_IDX_Surface] = getTrendIndex("Surface");
    sTrendIdxes[CAM_TREND_IDX_SkyDive] = getTrendIndex("SkyDive");
    sTrendIdxes[CAM_TREND_IDX_Dive] = getTrendIndex("Dive");
    sTrendIdxes[CAM_TREND_IDX_Lift] = getTrendIndex("Lift");
    sTrendIdxes[CAM_TREND_IDX_Crawl] = getTrendIndex("Crawl");
    sTrendIdxes[CAM_TREND_IDX_Field] = getTrendIndex("Field");
    sTrendIdxes[CAM_TREND_IDX_Dungeon] = getTrendIndex("Dungeon");
    sTrendIdxes[CAM_TREND_IDX_mogu] = getTrendIndex("mogu");
    sTrendIdxes[CAM_TREND_IDX_mogu2] = getTrendIndex("mogu2");
    sTrendIdxes[CAM_TREND_IDX_Tornade] = getTrendIndex("Tornade");
    sTrendIdxes[CAM_TREND_IDX_Rope] = getTrendIndex("Rope");
    sTrendIdxes[CAM_TREND_IDX_Tightrope] = getTrendIndex("Tightrope");
    sTrendIdxes[CAM_TREND_IDX_Parachute] = getTrendIndex("Parachute");
    sTrendIdxes[CAM_TREND_IDX_DialKey] = getTrendIndex("DialKey");
    sTrendIdxes[CAM_TREND_IDX_BirdRide] = getTrendIndex("BirdRide");
    sTrendIdxes[CAM_TREND_IDX_BirdRideLD] = getTrendIndex("BirdRideLD");
    sTrendIdxes[CAM_TREND_IDX_Trolley] = getTrendIndex("Trolley");
    sTrendIdxes[CAM_TREND_IDX_Boomerang] = getTrendIndex("Boomerang");
    sTrendIdxes[CAM_TREND_IDX_Room] = getTrendIndex("Room");
    sTrendIdxes[CAM_TREND_IDX_AfterCrawl] = getTrendIndex("AfterCrawl");
    if (dScGame_c::currentSpawnInfo.stageName == "F020") {
        sTrendIdxes[CAM_TREND_IDX_TerryShip_Island] = getTrendIndex("Island");
    } else {
        sTrendIdxes[CAM_TREND_IDX_TerryShip_Island] = getTrendIndex("TerryShip");
    }
    sTrendIdxes[CAM_TREND_IDX_Catapult] = getTrendIndex("Catapult");
    sTrendIdxes[CAM_TREND_IDX_TightropeHang] = getTrendIndex("TightropeHang");
    sTrendIdxes[CAM_TREND_IDX_BossKey] = getTrendIndex("BossKey");
    sTrendIdxes[CAM_TREND_IDX_SkyDiveNear] = getTrendIndex("SkyDiveNear");
    sTrendIdxes[CAM_TREND_IDX_ClawShot] = getTrendIndex("ClawShot");
    sTrendIdxes[CAM_TREND_IDX_Town] = getTrendIndex("Town");
    sTrendIdxes[CAM_TREND_IDX_Cannon] = getTrendIndex("Cannon");
    sTrendIdxes[CAM_TREND_IDX_Nushi] = getTrendIndex("Nushi");
    sTrendIdxes[CAM_TREND_IDX_Island] = getTrendIndex("Island");
    sTrendIdxes[CAM_TREND_IDX_Windmill] = getTrendIndex("Windmill");

    sStyleIdxes[CAM_STYLE_IDX_TA22] = getStyleIndex('TA22');
}

bool dCameraGame_c::initCamParamDat() {
    void *data = OarcManager::GetInstance()->getData("Camera", "arc/camparam.dat");

    if (data == nullptr) {
        return false;
    }

    CamParamHeader *header = (CamParamHeader *)data;
    sHeader = header;
    sFloats = (f32 *)((u32)data + sizeof(CamParamHeader));
    sFlags = (u16 *)((u32)sFloats + sizeof(f32) * header->numFloats);
    sStyles = (CamParamStyle *)((u32)sFlags + sizeof(u16) * header->numFlags);
    sTrends = (CamParamTrend *)((u32)sStyles + sizeof(CamParamStyle) * header->numStyles);
    debugPrintf4("*** camera parameter data *** ", sTrends); // ??? - need this in r4...
    debugPrintf4("    %3d float numbers", sHeader->numFloats);
    debugPrintf4("    %3d (x16) flags", sHeader->numFlags);
    debugPrintf4("    %3d styles", sHeader->numStyles);
    debugPrintf4("    %3d trends", sHeader->numTrends);
    cacheIdxes();
    return true;
}

dCameraGame_HIO_c::dCameraGame_HIO_c() {
    // TODO - ...
}

void dCameraGame_HIO_c::create() {
    // no-op
}

void dCameraGame_HIO_c::remove() {
    // no-op
}

void UnkCamChecks::fn_8007E130(dCameraGame_c *system) {
    mChk2.field_0x000 = 0;
    mChk1.field_0x000 = 0;
    mRef.field_0x00 = false;
    mRef.mMoveBgActorRef.unlink();
    field_0x16A = 0;
    field_0x16B = 0;

    field_0x164.roomCamId = -1;
    field_0x164.roomId = dAcPy_c::GetLinkM()->getRoomId();
    field_0x164.field_0x04 = 0;
    mpCamera = system;
}

void UnkCamChecks::fn_8007E1B0(dAcObjBase_c *ac) {
    dAcPy_c *link = dAcPy_c::GetLinkM();

    mVec3_c pos = ac->mPosition;
    if (ac == link && link->checkActionFlags(daPlayerActBase_c::FLG0_CRAWLING)) {
        pos = ac->mPositionCopy2;
        pos.y = ac->mPosition.y;
    }

    mVec3_c pos2 = pos;
    pos.y += 20.0f;

    dBgS_RoofChk roofChk;
    roofChk.SetUnderwaterRoof();
    roofChk.SetField_0x7C(1);
    roofChk.SetPos(&pos2);
    f32 rChk = dBgS::GetInstance()->RoofChk(&roofChk);
    if (pos2.y < rChk) {
        pos2.y = rChk;
    }

    mChk2.chk.SetCam();
    mChk2.chk.ClrObj();
    mChk2.chk.SetPos(&pos);
    mChk2.field_0x094 = dBgS::GetInstance()->GroundCross(&mChk2.chk);
    mChk2.field_0x000 = mChk2.field_0x094 != -1e9f;
    field_0x16B = 0;

    bool moveBg = dBgS::GetInstance()->ChkMoveBG(mChk2.chk, true);
    if (moveBg && mRef.field_0x00) {
        mRef.mMoveBgActorRef.link(const_cast<dAcObjBase_c *>(dBgS::GetInstance()->GetActorPointer(mChk2.chk)));
        mVec3_c v_c4 = ac->mPosition;
        dBgS::GetInstance()->MoveBgCrrPos(mChk2.chk, true, &v_c4, nullptr, nullptr, 0, 0);
        mRef.field_0x04 = v_c4 - ac->mPosition;
    } else {
        mRef.mMoveBgActorRef.unlink();
    }
    mRef.field_0x00 = moveBg;

    mChk1.chk.SetPos(&pos2);
    mChk1.field_0x094 = dBgS::GetInstance()->GroundCross(&mChk1.chk);
    mChk1.field_0x000 = mChk1.field_0x094 != -1e9f;

    if (link->checkActionFlags(daPlayerActBase_c::FLG0_IN_WATER) && mChk1.field_0x094 < ac->mPositionCopy3.y + 40.0f) {
        field_0x16A = 1;
    } else if (link->getRidingActorType() != daPlayerActBase_c::RIDING_NONE ||
               link->checkActionFlags(daPlayerActBase_c::FLG0_HANGING_LEDGE) ||
               link->checkActionFlags(daPlayerActBase_c::FLG0_ON_VINES)) {
        field_0x16A = 1;
    } else {
        if (ac->mPosition.y - mChk2.field_0x094 > sHio.field_0x0E8) {
            field_0x16A = 0;
        } else {
            field_0x16A = 1;
        }
    }

    if (mChk1.field_0x000 && ac == link && link->checkActionFlags(daPlayerActBase_c::FLG0_IN_WATER)) {
        mChk1.camId.roomCamId = dBgS::GetInstance()->GetRoomCamId(mChk1.chk);
        mChk1.camId.roomId = dBgS::GetInstance()->GetRoomId(mChk1.chk);
        if (!mChk1.camId.someCheck()) {
            mChk1.camId.clear();
        }
    } else {
        mChk1.camId.clear();
    }

    if (!field_0x16A) {
        mChk2.camId = mpCamera->getCamId0x60();
    } else if (mChk2.field_0x000) {
        mChk2.camId.roomCamId = dBgS::GetInstance()->GetRoomCamId(mChk2.chk);
        mChk2.camId.roomId = dBgS::GetInstance()->GetRoomId(mChk2.chk);
        if (!mChk2.camId.someCheck()) {
            mChk2.camId.clear();
        }
    } else {
        mChk2.camId.clear();
    }

    s32 roomId = getCurrentRoomId();
    if (roomId >= 0) {
        dRoom_c *room = dStage_c::GetInstance()->getRoom(roomId);
        if (room->getFile() != nullptr && room->getFile()->field_0x00 != -1) {
            field_0x16C.roomCamId = room->getFile()->field_0x00;
            field_0x16C.roomId = roomId;
            field_0x16C.field_0x04 = 0;
        } else {
            field_0x16C.clear();
        }
    }
}

dCameraGame_c::dCameraGame_c()
    : field_0x058(0),
      field_0x05C(0),
      mCurrentTrendIdx(-1),
      field_0x06C(0),
      field_0x070(0xFFFF),
      field_0x074(0),
      field_0x078(0),
      field_0x07C(0),
      field_0x08C(-1),
      field_0x090(-1),
      field_0x094(-1),
      field_0x096(0),
      field_0x098(0),
      field_0x0A8(0),
      field_0x0AC(0.0f),
      field_0x0B0(0.0f),
      field_0x22C(32),
      field_0x230(45),
      field_0x234(0.0f, 0.0f, 0.0f),
      field_0x240(0.0f, 0.0f, 0.0f) {}

extern "C" void *GROOSENATOR_REF;
extern "C" void *lbl_80575364;

bool dCameraGame_c::create() {
    field_0x098 = dAcPy_c::GetLinkM();
    if (field_0x098 == nullptr) {
        return false;
    }

    field_0x240 = field_0x234 = field_0x098->mPositionCopy3;
    initCamParamDat();
    field_0x060.clear();
    mCurrentTrendIdx = -1;
    field_0x06C = 0;
    field_0x070 = 0xFFFF;
    sHio.create();
    field_0x086.clear();
    field_0x080.clear();
    field_0x08C = -1;
    field_0x094 = -1;
    field_0x090 = -1;

    field_0x0AC = 0.0f;
    field_0x0B0 = 0.0f;
    field_0x058 = 0xA00;
    field_0x07C = 0;
    field_0x096 = 0;
    field_0x078 = 0;

    mChk.fn_8007E130(this);
    mChk.fn_8007E1B0(field_0x098);

    field_0x22C = 32;
    field_0x230 = 45;

    mView.field_0x0C = field_0x098->mPosition;
    mView.field_0x0C.y += 150.0f;

    field_0x02C.Set(320.0f, 15.0f, mAng::s2d_c(field_0x098->mRotation.y) + 180.0f);
    const mVec3_c &v = mView.field_0x0C; // needed to avoid double load... TODO maybe inline?
    mView.field_0x00 = v + field_0x02C.toCartesian();
    mView.mFov = 60.0f;
    mView2 = mView;

    field_0x264 = v.y;
    field_0x268 = mView.field_0x00.y;

    GROOSENATOR_REF = nullptr;
    lbl_80575364 = nullptr;
    field_0x079 = 0;
    field_0x07A = 0;

    return true;
}

bool dCameraGame_c::draw() {
    return true;
}

// TODO: make these static when they're actually used

inline bool linChkAttentionTarget_(const mVec3_c &from, const mVec3_c &to, dBgS_LinChk &chk) {
    dAcObjBase_c *ac = AttentionManager::GetInstance()->getTargetedActor();
    chk.Set(&from, &to, ac);
    return dBgS::GetInstance()->LineCross(&chk);
}

bool linChkAttentionTarget(const mVec3_c &from, const mVec3_c &to, dBgS_LinChk &chk, UNKWORD _ignored) {
    return linChkAttentionTarget_(from, to, chk);
}

bool linCross(dBgS_LinChk &chk, UNKWORD _ignored) {
    return dBgS::GetInstance()->LineCross(&chk);
}

bool camLinChkAttentionTarget(const mVec3_c &from, const mVec3_c &to, UNKWORD _ignored) {
    dBgS_CamLinChk chk;
    return linChkAttentionTarget(from, to, chk, _ignored);
}

bool linCrossAttentionTarget(const mVec3_c &from, const mVec3_c &to, mVec3_c &result) {
    dBgS_CamLinChk chk;
    bool ret = linChkAttentionTarget_(from, to, chk);
    if (ret) {
        result = chk.GetLinEnd();
    }
    return ret;
}

bool dCameraGame_c::execute() {
    // TODO - ...
}

bool dCameraGame_c::remove() {
    sHeader = nullptr;
    sFloats = nullptr;
    sFlags = nullptr;
    sStyles = nullptr;
    sTrends = nullptr;
    sHio.remove();
    return true;
}

void dCameraGame_c::clearCamIds() {
    field_0x086.clear();
    field_0x080.clear();
    mChk.field_0x164.clear();
    mChk.field_0x16C.clear();
}

bool dCameraGame_c::isCurrentTrend(const char *name) const {
    return getTrendIndex(name) == mCurrentTrendIdx;
}

void dCameraGame_c::setView(const CamView &view) {
    mView2 = view;
    mView = view;
    mVec3_c diff = view.field_0x00 - view.field_0x0C;
    field_0x02C.setCartesian(diff);
    field_0x078 = 0;
}

void dCameraGame_c::vt_0x08() {
    // TODO - ...
}
