

#include "toBeSorted/d_camera_game.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/bg/d_bg_s_roof_chk.h"
#include "d/col/bg/d_bg_s_sph_chk.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_bzs_types.h"
#include "d/d_player_act.h"
#include "d/d_room.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/math/math_types.h"
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
    /* 0x004 */ u8 _0x004[0x0CE - 0x004];

    /* 0x0CE */ s16 field_0x0CE;

    /* 0x0D0 */ u8 _0x0D0[0x0E8 - 0x0D0];

    /* 0x0E8 */ f32 field_0x0E8;

    /* 0x0EC */ u8 _0x0EC[0x138 - 0x0EC];
};

dCameraGame_HIO_c::~dCameraGame_HIO_c() {}
static dCameraGame_HIO_c sHio;

class GameCameraHandler {
public:
    GameCameraHandler() : field_0x00(false), mStyleIdx(0), field_0x0C(0), mCounter(0) {}

    bool doInit(dCameraGame_c *cam, u16 u3, UNKWORD u4);
    bool doExecute();

protected:
    /* 0x00 */ bool field_0x00;
    // vtable at 0x04
    /* vt 0x08 */ virtual bool init() {
        return true;
    }
    /* vt 0x0C */ virtual bool vt_0x0C() {
        return vt_0x10();
    }
    /* vt 0x10 */ virtual bool vt_0x10() {
        return true;
    }

    /* 0x08 */ u16 mStyleIdx;
    /* 0x0C */ UNKWORD field_0x0C;
    /* 0x10 */ CamId field_0x10;
    /* 0x18 */ dCamera_c *mpCamera;
    /* 0x1C */ dCameraGame_c *mpGameCamera;
    /* 0x20 */ dAcObjBase_c *field_0x20;
    /* 0x24 */ UNKWORD mCounter;
};

// "Lockon"
class GameCameraHandler03 : public GameCameraHandler {
public:
    GameCameraHandler03() {}

    virtual bool init() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ dAcRef_c<dAcObjBase_c> field_0x28;
    /* 0x34 */ dAcRef_c<dAcObjBase_c> field_0x34;
    /* 0x40 */ dPolar field_0x40;
    /* 0x4C */ u8 _0x4C[0xA8 - 0x4C];
};

class GameCameraHandler00 : public GameCameraHandler {
public:
private:
};

// "Chase"
class GameCameraHandler01 : public GameCameraHandler {
public:
    GameCameraHandler01() : field_0x2C(0.0f), field_0xAC(0.0f), field_0xB0(0.0f) {}

    virtual bool init() override;
    virtual bool vt_0x0C() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ u8 _0x28[0x2C - 0x28];
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ u8 _0x30[0xAC - 0x30];
    /* 0xAC */ f32 field_0xAC;
    /* 0xB0 */ f32 field_0xB0;
    /* 0xB4 */ u8 _0xB4[0xC0 - 0xB4];
};

class GameCameraHandler02 : public GameCameraHandler {
public:
    GameCameraHandler02() : field_0x6C(0.0f, 0.0f, 0.0f) {}

    virtual bool init() override;
    virtual bool vt_0x0C() override;
    virtual bool vt_0x10() override;

private:
    struct UnkStruct {
        UnkStruct() : field_0x04(0.0f), field_0x08(0.0f), field_0x0C(0.0f) {}

        /* 0x00 */ u8 _0x00[0x04 - 0x00];
        /* 0x04 */ f32 field_0x04;
        /* 0x08 */ f32 field_0x08;
        /* 0x0C */ f32 field_0x0C;
        /* 0x10 */ u8 _0x10[0x18 - 0x10];
    };

    /* 0x28 */ u8 _0x28[0x6C - 0x28];
    /* 0x6C */ mVec3_c field_0x6C;
    /* 0x78 */ u8 _0x88[0x88 - 0x78];
    /* 0x88 */ dAcRef_c<dAcObjBase_c> mRef; // not sure
    /* 0x94 */ u8 _0x94[0xB0 - 0x94];
    /* 0xB0 */ UnkStruct field_0xB0[4];
};

class GameCameraHandler04 : public GameCameraHandler {
public:
    virtual bool init() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ u8 _0x28[0x6C - 0x28];
    /* 0x6C */ dPolar field_0x6C;
};

class GameCameraHandler05 : public GameCameraHandler {
public:
    virtual bool init() override;
    virtual bool vt_0x0C() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ u8 _0x28[0x6C - 0x28];
    /* 0x6C */ dPolar field_0x6C;
    /* 0x78 */ dAcRef_c<dAcObjBase_c> field_0x78;
    /* 0x84 */ u8 _0x84[0x8C - 0x84];
};

class GameCameraHandler06 : public GameCameraHandler {
public:
    GameCameraHandler06() : field_0x70(0.0f), field_0x9C(0.0f), field_0xA0(0.0f) {}

    virtual bool init() override;
    virtual bool vt_0x0C() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ dAcRef_c<dAcObjBase_c> field_0x28;
    /* 0x34 */ u8 _0x34[0x4C - 0x34];
    /* 0x4C */ dPolar field_0x4C;
    /* 0x58 */ u8 _0x58[0x70 - 0x58];
    /* 0x70 */ f32 field_0x70;
    /* 0x74 */ u8 _0x74[0x9C - 0x74];
    /* 0x9C */ f32 field_0x9C;
    /* 0xA0 */ f32 field_0xA0;
    /* 0xA4 */ u8 _0xA4[0xB8 - 0xA4];
};

class GameCameraHandler07 : public GameCameraHandler {
public:
    virtual bool init() override;
    virtual bool vt_0x0C() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ dAcRef_c<dAcObjBase_c> field_0x28;
    /* 0x34 */ u8 _0x34[0x68 - 0x34];
};

class GameCameraHandler08 : public GameCameraHandler {
public:
    GameCameraHandler08() : field_0x78(0.0f) {}

    virtual bool init() override;
    virtual bool vt_0x0C() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ dAcRef_c<dAcObjBase_c> field_0x28;
    /* 0x34 */ u8 _0x34[0x4C - 0x34];
    /* 0x4C */ dPolar field_0x4C;
    /* 0x58 */ u8 _0x58[0x78 - 0x58];
    /* 0x78 */ f32 field_0x78;
    /* 0x7C */ u8 _0x7C[0x94 - 0x7C];
    /* 0x94 */ dPolar field_0x94;
};

class GameCameraHandler09 : public GameCameraHandler {
public:
    GameCameraHandler09() : field_0x2C(0.0f) {}

    virtual bool init() override;
    virtual bool vt_0x0C() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ u8 _0x28[0x2C - 0x28];
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ u8 _0x30[0xA0 - 0x30];
};

class GameCameraHandler10 : public GameCameraHandler {
public:
    virtual bool init() override;
    virtual bool vt_0x0C() override;
    virtual bool vt_0x10() override;

private:
};

class GameCameraHandler11 : public GameCameraHandler {
public:
    GameCameraHandler11() : field_0x38(0.0f), field_0xC4(0.0f) {}

    virtual bool init() override;
    virtual bool vt_0x0C() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ dAcRef_c<dAcObjBase_c> field_0x28;
    /* 0x34 */ u8 _0x34[0x38 - 0x24];
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ u8 _0x3C[0xC4 - 0x3C];
    /* 0xC4 */ f32 field_0xC4;
};

class GameCameraHandler12 : public GameCameraHandler {
public:
    virtual bool init() override;
    virtual bool vt_0x10() override;

private:
    /* 0x28 */ u8 _0x28[0x50 - 0x28];
};

// TODO this sinit function is awful

f32 floats00[] = {0.0f};
u16 flags00[] = {0};
CamStyle style00(0, 0, floats00, ARRAY_LENGTH(floats00), flags00, 1);
static UNKWORD word00;

f32 floats01[] = {
    0.0f,  400.0f, 400.0f, 0.5f, 20.0f, 20.0f, 0.25f, 10.0f, 10.0f, 0.25f,
    60.0f, 60.0f,  0.15f,  0.4f, 0.5f,  1.0f,  25.0f, 0.0f,  1.0f,
};
u16 flags01[] = {0x96};
CamStyle style01(0, 0, floats01, ARRAY_LENGTH(floats01), flags01, 16);
static UNKWORD word01;

f32 floats02[] = {
    0.0f, 0.0f, 20.0f, 0.0f, 50.0f, 0.0f, 20.0f, 8.0f,
};
u16 flags02[] = {0};
CamStyle style02(0, 0, floats02, ARRAY_LENGTH(floats02), flags02, 14);
static UNKWORD word02;

f32 floats03[] = {
    0.0f, 100.0f, 1000.0f, 0.1f, 400.0f, 400.0f, 0.2f, 0.0f, 0.0f, 0.4f, 10.0f, 10.0f,
    0.2f, 50.0f,  20.0f,   0.2f, 50.0f,  50.0f,  0.2f, 0.6f, 0.0f, 1.0f, 0.0f,
};
u16 flags03[] = {0x4A};
CamStyle style03(0, 0, floats03, ARRAY_LENGTH(floats03), flags03, 8);
static UNKWORD word03;

f32 floats04[] = {
    0.0f, 1.0f, 1.0f, 0.5f, 40.0f, 1000.0f,
};
u16 flags04[] = {0x00};
CamStyle style04(0, 0, floats04, ARRAY_LENGTH(floats04), flags04, 2);
static UNKWORD word04;

f32 floats05[] = {
    0.0f,
    50.0f,
    1.0f,
    1.0f,
};
u16 flags05[] = {0x2};
CamStyle style05(0, 0, floats05, ARRAY_LENGTH(floats05), flags05, 4);
static UNKWORD word05;

f32 floats06[] = {
    0.0f,  5.0f,  20.0f, 400.0f, 400.0f, 0.5f, 20.0f, 20.0f, 0.25f, 10.0f, 10.0f, 0.25f, 60.0f,
    60.0f, 0.15f, 0.4f,  0.5f,   1.0f,   0.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.2f,
};
u16 flags06[] = {0x202};
CamStyle style06(0, 0, floats06, ARRAY_LENGTH(floats06), flags06, 14);
static UNKWORD word06;

f32 floats07[] = {
    0.0f, -1.0f, 0.0f, 1.0f, 0.5f,
};
u16 flags07[] = {0x2};
CamStyle style07(0, 0, floats07, ARRAY_LENGTH(floats07), flags07, 4);
static UNKWORD word07;

f32 floats08[] = {
    0.0f,  5.0f,  20.0f, 400.0f, 400.0f, 0.5f, 20.0f, 20.0f, 0.25f, 10.0f, 10.0f,
    0.25f, 60.0f, 60.0f, 0.15f,  0.4f,   0.5f, 1.0f,  0.0f,  0.0f,  0.0f,
};
u16 flags08[] = {0x6};
CamStyle style08(0, 0, floats08, ARRAY_LENGTH(floats08), flags08, 3);
static UNKWORD word08;

f32 floats09[] = {
    0.0f,  400.0f, 400.0f, 0.5f,  20.0f, 20.0f, 0.25f,  60.0f,  10.0f,  0.25f, 60.0f,  60.0f,
    0.15f, 0.4f,   0.5f,   0.75f, 25.0f, 1.0f,  500.0f, 150.0f, 250.0f, 0.0f,  150.0f, 1.0f,
};
u16 flags09[] = {0x46};
CamStyle style09(0, 0, floats09, ARRAY_LENGTH(floats09), flags09, 7);
static UNKWORD word09;

f32 floats10[] = {
    0.0f,
};
u16 flags10[] = {0x00};
CamStyle style10(0, 0, floats10, ARRAY_LENGTH(floats10), flags10, 1);
static UNKWORD word10;

f32 floats11[] = {
    0.0f,  1000.0f, 5000.0f, 400.0f, 400.0f, 0.5f,  20.0f, 20.0f, 0.25f, 10.0f, 10.0f, 0.25f,  60.0f,
    60.0f, 0.15f,   0.4f,    0.5f,   1.0f,   25.0f, 0.0f,  1.0f,  75.0f, 0.0f,  0.0f,  300.0f, 1500.0f,
};
u16 flags11[] = {0x116};
CamStyle style11(0, 0, floats11, ARRAY_LENGTH(floats11), flags11, 10);
static UNKWORD word11;

f32 floats12[] = {
    0.0f, 500.0f, 50.0f, 0.4f, 0.1f, 0.25f,
};
u16 flags12[] = {0x00};
CamStyle style12(0, 0, floats12, ARRAY_LENGTH(floats12), flags12, 2);
static UNKWORD word12;

CamParamHeader *dCameraGame_c::sHeader;
f32 *dCameraGame_c::sFloats;
u16 *dCameraGame_c::sFlags;
CamParamStyle *dCameraGame_c::sStyles;
CamParamTrend *dCameraGame_c::sTrends;

s32 dCameraGame_c::sTrendIdxes[CAM_TREND_IDX_MAX];
s32 dCameraGame_c::sStyleIdxes[CAM_STYLE_IDX_MAX];

dAcRef_c<dAcObjBase_c> sRef;

struct CamVecBundleNotSure {
    CamVecBundleNotSure(mVec3_c _00, mVec3_c _01, f32 _02, f32 _03)
        : field_0x00(_00), field_0x0C(_01), field_0x18(_02), field_0x1C(_03) {}
    /* 0x00 */ mVec3_c field_0x00;
    /* 0x0C */ mVec3_c field_0x0C;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1C;
};

CamVecBundleNotSure sVecs1(mVec3_c(-550.0f, 500.0f, -1000.0f), mVec3_c(-150.0f, 320.0f, -350.0f), 60.0f, 0.0f);
CamVecBundleNotSure sVecs2(mVec3_c(-550.0f, 500.0f, -1000.0f), mVec3_c(-150.0f, -100.0f, -350.0f), 60.0f, 0.0f);
CamVecBundleNotSure sVecs3(mVec3_c(-900.0f, 680.0f, 850.0f), mVec3_c(-440.0f, 380.0f, 280.0f), 60.0f, 0.0f);
CamVecBundleNotSure sVecs4(mVec3_c(-90.0f, 80.0f, -285.0f), mVec3_c(0.0f, 100.0f, 0.0f), 55.0f, 0.0f);

GameCameraHandler06 camHandlerUnused00;
GameCameraHandler08 camHandlerUnused01;

GameCameraHandler00 camHandler00;
GameCameraHandler01 camHandler01;
GameCameraHandler02 camHandler02;
GameCameraHandler03 camHandler03;
GameCameraHandler04 camHandler04;
GameCameraHandler05 camHandler05;
GameCameraHandler06 camHandler06;
GameCameraHandler07 camHandler07;
GameCameraHandler08 camHandler08;
GameCameraHandler09 camHandler09;
GameCameraHandler10 camHandler10;
GameCameraHandler11 camHandler11;
GameCameraHandler12 camHandler12;

// clang-format off
/////////////////////////////////////////////////////////////////
//                                                             //
//                    START ACTUAL CODE                        //
//                                                             //
/////////////////////////////////////////////////////////////////
// clang-format on

void debugPrintf4(const char *fmt, ...) {
    // no-op
}

void debugPrintf6(UNKWORD a, UNKWORD b, const char *fmt, ...) {
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
        // Temp needed...
        u32 otherId = *(u32 *)sStyles[i].id;
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

bool GameCameraHandler::doInit(dCameraGame_c *cam, u16 u3, UNKWORD u4) {
    mStyleIdx = u3;
    field_0x0C = u4;
    mCounter = 0;
    field_0x00 = 0;
    field_0x20 = dAcPy_c::GetLinkM();
    mpCamera = dScGame_c::getCamera();
    mpGameCamera = cam;
    field_0x10 = cam->getCamId0x60();
    init();
    return true;
}

bool GameCameraHandler::doExecute() {
    field_0x20 = dAcPy_c::GetLinkM();
    if (!field_0x00) {
        field_0x00 = vt_0x0C();
    } else {
        field_0x00 = vt_0x10();
    }
    mCounter++;
    return field_0x00;
}

bool GameCameraHandler03::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler03::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler01::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler01::vt_0x0C() {
    // TODO - ...
    return true;
}

bool GameCameraHandler01::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler02::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler02::vt_0x0C() {
    // TODO - ...
    return true;
}

bool GameCameraHandler02::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler04::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler04::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler05::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler05::vt_0x0C() {
    // TODO - ...
    return true;
}

bool GameCameraHandler05::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler06::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler06::vt_0x0C() {
    // TODO - ...
    return true;
}

bool GameCameraHandler06::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler07::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler07::vt_0x0C() {
    // TODO - ...
    return true;
}

bool GameCameraHandler07::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler08::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler08::vt_0x0C() {
    // TODO - ...
    return true;
}

bool GameCameraHandler08::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler09::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler09::vt_0x0C() {
    // TODO - ...
    return true;
}

bool GameCameraHandler09::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler10::init() {
    // TODO - ...
    // style10.loadStyle(mStyleIdx);
    return true;
}

bool GameCameraHandler10::vt_0x0C() {
    return true;
}

bool GameCameraHandler10::vt_0x10() {
    field_0x20 = dAcPy_c::GetLinkM();
    return true;
}

bool GameCameraHandler11::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler11::vt_0x0C() {
    // TODO - ...
    return true;
}

bool GameCameraHandler11::vt_0x10() {
    // TODO - ...
    return true;
}

bool GameCameraHandler12::init() {
    // TODO - ...
    return true;
}

bool GameCameraHandler12::vt_0x10() {
    // TODO - ...
    return true;
}

struct CameraDefinition {
    /* 0x00 */ GameCameraHandler *pHandler;
    /* 0x04 */ UNKWORD id;
    /* 0x08 */ UNKWORD unk;
    /* 0x0C */ CamStyle *pStyle;
};

CameraDefinition sCamDefs[] = {
    {&camHandler00,  0, word00, &style00},
    {&camHandler01,  1, word01, &style01},
    {&camHandler02,  2, word02, &style02},
    {&camHandler03,  3, word03, &style03},
    {&camHandler04,  4, word04, &style04},
    {&camHandler05,  5, word05, &style05},
    {&camHandler06,  6, word06, &style06},
    {&camHandler07,  7, word07, &style07},
    {&camHandler08,  8, word08, &style08},
    {&camHandler09,  9, word09, &style09},
    {&camHandler10, 10, word10, &style10},
    {&camHandler11, 11, word11, &style11},
    {&camHandler12, 12, word12, &style12},
};

void CamStyle::loadStyle(u16 styleIdx) {
    mStyleIndex = styleIdx;
    mId[0] = dCameraGame_c::getStyle(styleIdx)->id[0];
    mId[1] = dCameraGame_c::getStyle(styleIdx)->id[1];
    mId[2] = dCameraGame_c::getStyle(styleIdx)->id[2];
    mId[3] = dCameraGame_c::getStyle(styleIdx)->id[3];
    mId[4] = '\0';
    mpParamFloats = dCameraGame_c::getFloats(dCameraGame_c::getStyle(styleIdx)->floatParamOffset);
    mpParamFlags = dCameraGame_c::getFlags(dCameraGame_c::getStyle(styleIdx)->flagParamOffset);

    for (int i = 0; i < mNumFloats; i++) {
        mpFloats[i] = mpParamFloats[i];
    }

    // TODO: Why 14???
    for (int i = 0; i < mNumFlags; i += 14) {
        mpFlags[i] = mpParamFlags[i];
    }
}

f32 CamStyle::getFloat(s32 idx) const {
    return mpFloats[idx];
}

bool CamStyle::getFlag(s32 flag) const {
    return ((1 << (flag & ((1 << 14) - 1))) & mpFlags[flag >> 14]) != 0;
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
        if (!mChk1.camId.isValid()) {
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
        if (!mChk2.camId.isValid()) {
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
    : mFlags(0),
      field_0x05C(0),
      mCurrentTrendIdx(-1),
      field_0x06C(0),
      field_0x070(0xFFFF),
      field_0x074(0),
      field_0x078(false),
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
    mFlags = 0xA00;
    field_0x07C = 0;
    field_0x096 = 0;
    field_0x078 = false;

    mChk.fn_8007E130(this);
    mChk.fn_8007E1B0(field_0x098);

    field_0x22C = 32;
    field_0x230 = 45;

    mView.mTarget = field_0x098->mPosition;
    mView.mTarget.y += 150.0f;

    field_0x02C.Set(320.0f, 15.0f, mAng::s2d_c(field_0x098->mRotation.y) + 180.0f);
    const mVec3_c &v = mView.mTarget; // needed to avoid double load... TODO maybe inline?
    mView.mPosition = v + field_0x02C.toCartesian();
    mView.mFov = 60.0f;
    mView2 = mView;

    field_0x264 = v.y;
    field_0x268 = mView.mPosition.y;

    GROOSENATOR_REF = nullptr;
    lbl_80575364 = nullptr;
    field_0x079 = 0;
    field_0x07A = 0;

    return true;
}

bool dCameraGame_c::execute() {
    // TODO - ...
}

// TODO: make these static when they're actually used

bool chkCamera(mVec3_c &v1, mVec3_c &v2, dAcObjBase_c *obj) {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    bool isNotLink = obj != nullptr && obj != link;
    return dCcS::GetInstance()->ChkCamera(
        v1, v2, 15.0f, link, isNotLink ? obj : nullptr, (dAcObjBase_c *)link->getLinkage().mControllingActor.p_owner
    );
}

bool chkCo(const mVec3_c &v1) {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    return dCcS::GetInstance()->ChkCo(v1, link);
}

bool chkCameraPoint(const mVec3_c &p2, const mVec3_c &p3, mVec3_c &result, dAcObjBase_c *obj) {
    mVec3_c v1 = p3;

    // This matches but not sure about regalloc...
    dAcPy_c *link = dAcPy_c::LINK;
    int i;

    dAcObjBase_c *other = nullptr;

    if (obj != nullptr && obj != link) {
        other = obj;
    }

    mVec3_c v2 = p3;

    bool found;
    dAcObjBase_c *controllingActor = (dAcObjBase_c *)link->getLinkage().mControllingActor.p_owner;
    found = false;

    mVec3_c ret;
    mVec3_c v3 = v2 - p2;
    v3.normalize();
    v3 *= 2.0f;

    for (i = 0; i < 10; i++) {
        ret = v1;
        if (!dCcS::GetInstance()->ChkCameraPoint(p2, v2, v1, 15.0f, link, other, controllingActor)) {
            break;
        }

        if (p2 == v1) {
            break;
        }
        found = true;
        v2 = v1 - v3;
    }

    if (found) {
        debugPrintf6(400, 300, "CO %d", i);
        result = ret;
    }
    return found;
}

inline bool linCrossAttentionTarget_(const mVec3_c &from, const mVec3_c &to, dBgS_LinChk &chk) {
    dAcObjBase_c *ac = AttentionManager::GetInstance()->getTargetedActor();
    chk.Set(&from, &to, ac);
    return dBgS::GetInstance()->LineCross(&chk);
}

bool linCrossAttentionTarget(const mVec3_c &from, const mVec3_c &to, dBgS_LinChk &chk, UNKWORD _ignored) {
    return linCrossAttentionTarget_(from, to, chk);
}

bool linCross(dBgS_LinChk &chk, UNKWORD _ignored) {
    return dBgS::GetInstance()->LineCross(&chk);
}

bool linCrossAttentionTarget(const mVec3_c &from, const mVec3_c &to, UNKWORD _ignored) {
    dBgS_CamLinChk chk;
    return linCrossAttentionTarget(from, to, chk, _ignored);
}

bool linCrossAttentionTarget(const mVec3_c &from, const mVec3_c &to, mVec3_c &result) {
    dBgS_CamLinChk chk;
    bool ret = linCrossAttentionTarget_(from, to, chk);
    if (ret) {
        result = chk.GetLinEnd();
    }
    return ret;
}

u8 unkCamLineChk(dBgS_CamLinChk &chk1, dBgS_CamLinChk &chk2, u32 flags) {
    // TODO - ...
    return 'W';
}

struct UnkCallbackArg {
    UnkCallbackArg() : field_0x08(nullptr) {}

    void set(mVec3_c *v1, mVec3_c *v3, f32 f) {
        field_0x00 = v1;
        field_0x10 = v3;
        field_0x14 = mVec3_c::Zero;
        field_0x20 = mVec3_c::Zero;
        field_0x04 = f;
        field_0x08 = 0;
        field_0x0C = 0.0f;
    }

    /* 0x00 */ mVec3_c *field_0x00;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ UNKWORD field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ mVec3_c *field_0x10;
    /* 0x14 */ mVec3_c field_0x14;
    /* 0x20 */ mVec3_c field_0x20;
};


void unkCamCallback1(dBgS_SphChk *, cBgD_Vtx_t *, int, int, int, cM3dGPla *plane, void *cbArg) {
    UnkCallbackArg *arg = static_cast<UnkCallbackArg*>(cbArg);
    f32 f = cM3d_SignedLenPlaAndPos(plane, arg->field_0x00);

    // TODO Regswap
    if (plane->mD + nw4r::math::VEC3Dot(plane->mNormal, arg->field_0x10) >= -2.0f && f > -arg->field_0x04 && f < arg->field_0x04) {
        mVec3_c n = plane->mNormal;
        f32 t2 = (arg->field_0x04 + 0.001f - f);

        if (arg->field_0x0C < t2) {
            arg->field_0x0C = t2;
        }

        mVec3_c tmp = n * t2;
        arg->field_0x08++;
        if (tmp.x > arg->field_0x14.x) {
            arg->field_0x14.x = tmp.x;
        }
        if (tmp.y > arg->field_0x14.y) {
            arg->field_0x14.y = tmp.y;
        }
        if (tmp.z > arg->field_0x14.z) {
            arg->field_0x14.z = tmp.z;
        }
        if (tmp.x < arg->field_0x20.x) {
            arg->field_0x20.x = tmp.x;
        }
        if (tmp.y < arg->field_0x20.y) {
            arg->field_0x20.y = tmp.y;
        }
        if (tmp.z < arg->field_0x20.z) {
            arg->field_0x20.z = tmp.z;
        }
    }
}

void unkCamCallback2(dBgS_SphChk *, cBgD_Vtx_t *, int, int, int, cM3dGPla *plane, void *cbArg) {
    UnkCallbackArg *arg = static_cast<UnkCallbackArg*>(cbArg);
    f32 f = cM3d_SignedLenPlaAndPos(plane, arg->field_0x00);

    // TODO Regswap
    if (plane->mD + nw4r::math::VEC3Dot(plane->mNormal, arg->field_0x10) >= -2.0f && f > -arg->field_0x04 && f < arg->field_0x04) {
        mVec3_c n = plane->mNormal;
        mVec3_c tmp = n * (arg->field_0x04 + 0.001f - f);
        arg->field_0x08++;
        if (std::fabsf(tmp.x) > arg->field_0x14.x) {
            arg->field_0x14.x = tmp.x;
        }
        if (std::fabsf(tmp.y) > arg->field_0x14.y) {
            arg->field_0x14.y = tmp.y;
        }
        if (std::fabsf(tmp.z) > arg->field_0x14.z) {
            arg->field_0x14.z = tmp.z;
        }
    }
}


void fn_8007FE10(mVec3_c &v1, mVec3_c &v2, mVec3_c &v3, f32 f) {
    v1 = v2;

    dBgS_CamSphChk chk;
    UnkCallbackArg arg;

    chk.mCallback = unkCamCallback1;

    for (int i = 0; i < sHio.field_0x0CE; i++) {
        arg.set(&v1, &v3, f);

        chk.Set(&v1, f);

        if (!dBgS::GetInstance()->SphChk(&chk, &arg)) {
            break;
        }

        if (arg.field_0x0C < 0.0f) {
            break;
        }

        arg.field_0x14 += arg.field_0x20;
        arg.field_0x14.normalize();
        arg.field_0x14 *= arg.field_0x0C;
        v1 += arg.field_0x14;
    }

    chk.mCallback = unkCamCallback2;
    chk.Set(&v1, f);
    arg.set(&v1, &v3, f);

    if (dBgS::GetInstance()->SphChk(&chk, &arg)) {
        v1 += arg.field_0x14;
    }
}

bool dCameraGame_c::draw() {
    return true;
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

void dCameraGame_c::overrideCam(s32 camId, s32 priority, s16 roomId, bool unk) {
    if (priority < field_0x094) {
        return;
    }

    field_0x094 = priority;
    if (roomId < 0) {
        roomId = getCurrentRoomId();
    }

    if (camId == -1) {
        if (field_0x086.isValid()) {
            field_0x086.clear();
        }
    } else {
        (void)getCam(camId, roomId);
        field_0x086.roomCamId = camId;
        field_0x086.roomId = roomId;
        s16 val = 1;
        if (unk) {
            val = 2;
        }
        field_0x086.field_0x04 = val;
    }
}

void dCameraGame_c::setCam(s16 roomCamId, s16 roomId) {
    if (roomId < 0) {
        roomId = getCurrentRoomId();
    }
    (void)getCam(roomCamId, roomId);
    field_0x080.roomCamId = roomCamId;
    field_0x080.roomId = roomId;
    field_0x080.field_0x04 = 3;
}

void dCameraGame_c::setCam(const CamId &id) {
    setCam(id.roomCamId, id.roomId);
}

void dCameraGame_c::clearCamIds() {
    field_0x086.clear();
    field_0x080.clear();
    mChk.field_0x164.clear();
    mChk.field_0x16C.clear();
}

CamId dCameraGame_c::getCamId() {
    CamId ret = field_0x060;
    if (field_0x080.isValid()) {
        ret = field_0x080;
    } else if (field_0x086.isValid()) {
        ret = field_0x086;
    } else if (mChk.field_0x164.isValid()) {
        ret = mChk.field_0x164;
    } else if (mChk.field_0x16C.isValid()) {
        ret = mChk.field_0x16C;
    } else {
        ret.clear();
    }

    return ret;
}

bool dCameraGame_c::isCurrentTrend(const char *name) const {
    return getTrendIndex(name) == mCurrentTrendIdx;
}

void dCameraGame_c::setView(const CamView &view) {
    mView2 = view;
    mView = view;
    mVec3_c diff = view.mPosition - view.mTarget;
    field_0x02C.setCartesian(diff);
    field_0x078 = false;
}

void dCameraGame_c::onBecomeActive() {
    // TODO - ...
}
