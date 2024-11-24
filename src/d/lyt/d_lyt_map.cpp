#include "d/lyt/d_lyt_map.h"

#include "egg/core/eggColorFader.h"
#include "m/m_video.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

struct LytMapConfigMaybe {
    LytMapConfigMaybe();
    virtual ~LytMapConfigMaybe();

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0C;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1A */ u8 field_0x1A;
    /* 0x1B */ u8 field_0x1B;
    /* 0x1C */ u8 field_0x1C;
    /* 0x1D */ u8 field_0x1D;
    /* 0x1E */ u8 field_0x1E;
    /* 0x1F */ u8 field_0x1F;
    /* 0x20 */ u8 field_0x20;
    /* 0x21 */ u8 field_0x21;
    /* 0x22 */ u8 field_0x22;
    /* 0x23 */ u8 field_0x23;
    /* 0x24 */ u8 field_0x24;
    /* 0x25 */ u8 field_0x25;
    /* 0x26 */ u8 field_0x26;
    /* 0x27 */ u8 field_0x27;
    /* 0x28 */ u8 field_0x28;
    /* 0x29 */ u8 field_0x29;
    /* 0x2A */ u8 field_0x2A;
    /* 0x2B */ u8 field_0x2B;
    /* 0x2C */ f32 field_0x2C;
};

LytMapConfigMaybe sLytMapConfig;

LytMapConfigMaybe::LytMapConfigMaybe() {
    field_0x04 = -10.0f;
    field_0x10 = 14.0f;
    field_0x14 = 32.0f;
    field_0x18 = 0;
    field_0x19 = 0xFF;
    field_0x1A = 0xF;
    field_0x1B = 0x12;
    field_0x1C = 9;
    field_0x2C = 0.0f;
    field_0x1D = 0x2D;
    field_0x1E = 0x2D;
    field_0x1F = 0x1E;
    field_0x20 = 0x17;
    field_0x21 = 0x5A;
    field_0x22 = 0x2D;
    field_0x23 = 0x2D;
    field_0x24 = 0x35;
    field_0x25 = 0x2D;
    field_0x26 = 0x5A;
    field_0x27 = 0x2D;
    field_0x28 = 0x5A;
    field_0x29 = 0x5;
    field_0x08 = 10.0f;
    field_0x0C = 0.4f;
    field_0x2A = 0x80;
    field_0x2B = 10;
}

LytMapConfigMaybe::~LytMapConfigMaybe() {}

STATE_DEFINE(dLytMapPinIcon_c, Wait);
STATE_DEFINE(dLytMapPinIcon_c, ToSelect);
STATE_DEFINE(dLytMapPinIcon_c, Select);
STATE_DEFINE(dLytMapPinIcon_c, ToUnselect);
STATE_DEFINE(dLytMapPinIcon_c, Remove);

STATE_DEFINE(dLytMapPinIconAggregate_c, Wait);
STATE_DEFINE(dLytMapPinIconAggregate_c, Select);
STATE_DEFINE(dLytMapPinIconAggregate_c, Remove);

STATE_DEFINE(dLytMapFloorBtn_c, Wait);
STATE_DEFINE(dLytMapFloorBtn_c, ToSelect);
STATE_DEFINE(dLytMapFloorBtn_c, Select);
STATE_DEFINE(dLytMapFloorBtn_c, ToWait);
STATE_DEFINE(dLytMapFloorBtn_c, ToDecide);
STATE_DEFINE(dLytMapFloorBtn_c, Decide);
STATE_DEFINE(dLytMapFloorBtn_c, ToSelectInDecide);
STATE_DEFINE(dLytMapFloorBtn_c, SelectInDecide);
STATE_DEFINE(dLytMapFloorBtn_c, DecideToSelectInDecide);

STATE_DEFINE(dLytMapFloorBtnMgr_c, Invisible);
STATE_DEFINE(dLytMapFloorBtnMgr_c, Wait);

nw4r::ut::Color sMapFaderColor(0xFF, 0xFF, 0xFF, 0xFF);

STATE_DEFINE(dLytMapPopupInfo_c, Invisible);
STATE_DEFINE(dLytMapPopupInfo_c, In);
STATE_DEFINE(dLytMapPopupInfo_c, Wait);
STATE_DEFINE(dLytMapPopupInfo_c, WaitInvalid);
STATE_DEFINE(dLytMapPopupInfo_c, Out);

STATE_DEFINE(dLytMapSavePopupAction_c, Invisible);
STATE_DEFINE(dLytMapSavePopupAction_c, In);
STATE_DEFINE(dLytMapSavePopupAction_c, Wait);
STATE_DEFINE(dLytMapSavePopupAction_c, Out);

STATE_DEFINE(dLytMapSaveCaption_c, Invisible);
STATE_DEFINE(dLytMapSaveCaption_c, In);
STATE_DEFINE(dLytMapSaveCaption_c, Wait);
STATE_DEFINE(dLytMapSaveCaption_c, Out);

STATE_DEFINE(dLytMapSaveObj_c, Wait);
STATE_DEFINE(dLytMapSaveObj_c, ToSelect);
STATE_DEFINE(dLytMapSaveObj_c, Select);
STATE_DEFINE(dLytMapSaveObj_c, ToWait);
STATE_DEFINE(dLytMapSaveObj_c, Decide);

STATE_DEFINE(dLytMapMain_c, Invisible);
STATE_DEFINE(dLytMapMain_c, RenderingWait);
STATE_DEFINE(dLytMapMain_c, In);
STATE_DEFINE(dLytMapMain_c, Active);
STATE_DEFINE(dLytMapMain_c, Out);
STATE_DEFINE(dLytMapMain_c, FloorChange);
STATE_DEFINE(dLytMapMain_c, ChgMapUpDirection);
STATE_DEFINE(dLytMapMain_c, ChgDispAreaMode_Map);
STATE_DEFINE(dLytMapMain_c, ChgDispAreaMode_MapRot);
STATE_DEFINE(dLytMapMain_c, ChgDispAreaMode_WA);
STATE_DEFINE(dLytMapMain_c, ChgDispAreaMode_CsrRot);
STATE_DEFINE(dLytMapMain_c, ResetPos);
STATE_DEFINE(dLytMapMain_c, ResetPosWithFloorChange);
STATE_DEFINE(dLytMapMain_c, EventSwBankSmall_Step1);
STATE_DEFINE(dLytMapMain_c, EventSwBankSmall_Step2);
STATE_DEFINE(dLytMapMain_c, EventSwBankSmall_Step3);
STATE_DEFINE(dLytMapMain_c, EventMapIntro_Step1);
STATE_DEFINE(dLytMapMain_c, EventMapIntro_Step2);
STATE_DEFINE(dLytMapMain_c, EventMapIntro_Step3);
STATE_DEFINE(dLytMapMain_c, EventMapIntro_Step4);
STATE_DEFINE(dLytMapMain_c, EventDungeonMapGet_Step1);
STATE_DEFINE(dLytMapMain_c, EventDungeonMapGet_Step2);
STATE_DEFINE(dLytMapMain_c, EventDungeonMapGet_Step3);
STATE_DEFINE(dLytMapMain_c, EventFieldMapChange_Step1);
STATE_DEFINE(dLytMapMain_c, EventFieldMapChange_Step2);
STATE_DEFINE(dLytMapMain_c, EventFieldMapChange_Step3);
STATE_DEFINE(dLytMapMain_c, EventFieldMapChange_Step4);
STATE_DEFINE(dLytMapMain_c, EventForestMapChange_Step1);
STATE_DEFINE(dLytMapMain_c, EventForestMapChange_Step2);
STATE_DEFINE(dLytMapMain_c, EventForestMapChange_Step3);
STATE_DEFINE(dLytMapMain_c, EventSignalAdd_Step1);
STATE_DEFINE(dLytMapMain_c, EventSignalAdd_Step2);
STATE_DEFINE(dLytMapMain_c, EventSignalAdd_Step3);
STATE_DEFINE(dLytMapMain_c, EventGoddessCube_Step1);
STATE_DEFINE(dLytMapMain_c, EventGoddessCube_Step2);
STATE_DEFINE(dLytMapMain_c, EventGoddessCube_Step3);
STATE_DEFINE(dLytMapMain_c, EventSaveObjMsgWindow);
STATE_DEFINE(dLytMapMain_c, EventSaveObjSelect);
STATE_DEFINE(dLytMapMain_c, EventSaveObjConfirmMsgWindow);
STATE_DEFINE(dLytMapMain_c, EventSaveObjDecide);

dLytMapFader_c::dLytMapFader_c()
    : mFader(
          0.0f, 0.0f, mVideo::m_video->pRenderMode->fbWidth, mVideo::m_video->pRenderMode->efbHeight, sMapFaderColor,
          EGG::ColorFader::STATUS_PREPARE_OUT
      ) {}

dLytMapFader_c::~dLytMapFader_c() {}

void dLytMapFader_c::calc() {
    mFader.calc();
}

void dLytMapFader_c::draw() {
    mFader.draw();
}

#pragma dont_inline on
dLytMapMain_c::dLytMapMain_c() : mStateMgr(*this), mFloorBtnMgr(&field_0x8DC8) {}
#pragma dont_inline reset

dLytMapMain_c::~dLytMapMain_c() {}

void dLytMapMain_c::draw() {}

extern "C" void fn_80016690();
void dLytMap_c::build() {
    fn_80016690();
    void *data = LayoutArcManager::sInstance->getLoadedData("Map2D");
    mResAcc.attach(data, "");
    mMapMain.build();
    m2d::getAllocator();
    mMapMain.setPriority(0x86);
    // TODO there's another thing here at 0x91C0
}

void lytMapusesSizedWString() {
    static SizedWString<0x20> sString;
    sString.sprintf(L"%ld", 0);
}
