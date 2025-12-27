// clang-format off
// vtable order
#define NEED_DIRECT_FRAMECTRL_ACCESS

#include "c/c_math.h"
#include "d/d_vec.h"
#include "d/d_player_act.h"
#include "d/snd/d_snd_player_mgr.h"
#include "toBeSorted/minigame_mgr.h"
#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_tbox.h"
#include "d/d_base.h"
#include "d/d_cs_base.h"
#include "d/d_cs_game.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_d2d.h"
#include "d/d_fader.h"
#include "d/d_message.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/d_player.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_map.h"
#include "d/lyt/d_lyt_map_global.h"
#include "d/lyt/d_lyt_unknowns.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "egg/core/eggColorFader.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_pad.h"
#include "m/m_vec.h"
#include "m/m_video.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_types.h"
#include "s/s_StateInterfaces.hpp"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/d_beacon.h"
#include "toBeSorted/event_manager.h"

// clang-format on

struct dLytMap_HIO_c {
    dLytMap_HIO_c();
    virtual ~dLytMap_HIO_c();

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 mFootstepsScale;
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
    /* 0x2A */ u8 mFootstepsAlpha;
    /* 0x2B */ u8 field_0x2B;
    /* 0x2C */ f32 field_0x2C;

    f32 getField_0x04() const {
        return field_0x04;
    }
};

dLytMap_HIO_c sHio;

dLytMap_HIO_c::dLytMap_HIO_c() {
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
    mFootstepsScale = 0.4f;
    mFootstepsAlpha = 0x80;
    field_0x2B = 10;
}

dLytMap_HIO_c::~dLytMap_HIO_c() {}

void dMapFootPrintsQueue_c::init() {
    int j = ARRAY_LENGTH(mEntries) - 1;
    for (int i = 0; i < (int)ARRAY_LENGTH(mEntries); i++) {
        mEntries[i].position.set(0.0f, 0.0f, 0.0f);
        mEntries[i].pNext = &mEntries[j];
        mEntries[j].pPrev = &mEntries[i];
        j = i;
    }

    mpFirst = &mEntries[0];
    mpLast = &mEntries[0];
    mCount = 0;
}

void dMapFootPrintsQueue_c::addStep(const mVec3_c &pos, const mAng &rot) {
    mpLast->position = pos;
    mpLast->rotation = rot;
    if (mpFirst == mpLast && mCount == ARRAY_LENGTH(mEntries)) {
        mpFirst = mpFirst->pPrev;
    }
    mpLast = mpLast->pPrev;
    if (mCount < ARRAY_LENGTH(mEntries)) {
        mCount++;
    }
}

dMapFootPrintEntry *dMapFootPrintsQueue_c::getFirst() const {
    if (mCount == 0) {
        return nullptr;
    }
    return mpLast->pNext;
}

dMapFootPrintEntry *dMapFootPrintsQueue_c::getNext(const dMapFootPrintEntry *it) const {
    dMapFootPrintEntry *ret = nullptr;
    if (it != mpFirst) {
        ret = it->pNext;
    }
    return ret;
}

void dMapFootPrintsMgr_c::execute() {
    dAcPy_c *link = getLinkPtr();
    const mVec3_c &linkPos = link->mPosition;
    if (mQueue.isEmpty()) {
        mQueue.addStep(linkPos, link->mRotation.y);
    } else {
        mVec3_c lastPos = mQueue.getFirst()->position;
        mVec3_c diff = mVec3_c(linkPos - lastPos);
        f32 absXZ = diff.squareMagXZ();
        mAng rot = diff.atan2sX_Z();
        if (absXZ >= mMinStepDistanceSq) {
            mQueue.addStep(linkPos, rot);
        }
    }
}

dLytMapFootPrints_c::dLytMapFootPrints_c(dLytMapGlobal_c *global)
    : mpGlobal(global),
      mpPane(nullptr),
      field_0xA0(0.0f),
      mMaxNumSteps(ARRAY_LENGTH(mFootprintPositions)),
      mCurrentNumSteps(0),
      field_0x198(0.0f),
      field_0x19C(0.0f),
      mVisible(false) {
    for (int i = 0; i < (int)ARRAY_LENGTH(mFootprintPositions); i++) {
        mFootprintPositions[i].set(0.0f, 0.0f);
    }
}

dLytMapGlobal_c *dLytMapFootPrints_c::getGlobal() const {
    return mpGlobal;
}

bool dLytMapFootPrints_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("footPrint_00.brlyt", nullptr);
    nw4r::lyt::Pane *root = mLyt.getLayout()->GetRootPane();
    root->SetVisible(false);
    root->SetAlpha(0);
    root->SetInfluencedAlpha(true);
    mpPane = mLyt.findPane("P_all_01");
    mLyt.calc();
    field_0xA0 = 1.0f;
    return true;
}

bool dLytMapFootPrints_c::remove() {
    return true;
}

bool dLytMapFootPrints_c::execute() {
    dLytMapGlobal_c *global = getGlobal();
    u8 globalAlpha = global->getAlpha();
    nw4r::lyt::Pane *root = mLyt.getLayout()->GetRootPane();
    if (globalAlpha != 0) {
        root->SetVisible(true);
        root->SetAlpha(globalAlpha);
    } else {
        root->SetVisible(false);
        root->SetAlpha(0);
    }
    const dMapFootPrintsQueue_c *queue = dLytControlGame_c::getInstance()->getFootprintsQueue();
    mVec2_c playerPos;
    global->projectOntoMap(playerPos, global->getPlayerPos());
    mMaxNumSteps = sHio.field_0x2B;

    const dMapFootPrintEntry *it = queue->getFirst();
    int i = 0;

    for (; it != nullptr; it = queue->getNext(it)) {
        mVec2_c footstepPos;
        global->projectOntoMap(footstepPos, it->position);
        mVec2_c diff;
        diff = footstepPos - playerPos;
        f32 dist = diff.length();

        for (f32 step = dist / global->getField_0x44(); step >= sHio.field_0x08 / field_0x198;
             step /= global->getField_0x44()) {
            f32 tmp = (sHio.field_0x08 / field_0x198) / step;

            mVec2_c v = mVec2_c((footstepPos - playerPos) * tmp + playerPos);
            mFootprintPositions[i] = v;
            if (++i >= mMaxNumSteps) {
                break;
            }
            playerPos = v;
            diff = footstepPos - playerPos;
            step = diff.length();
        }
        if (i >= mMaxNumSteps) {
            break;
        }
    }
    mCurrentNumSteps = i;
    return true;
}

void dLytMapFootPrints_c::draw() {
    if (!mVisible) {
        return;
    }

    dLytMapGlobal_c *global = getGlobal();
    u8 globalAlpha = global->getAlpha();
    if (globalAlpha == 0) {
        return;
    }

    // TODO - GPR regswap between pane and mFootprintPositions[i]
    nw4r::lyt::Pane *pane = mLyt.getLayout()->GetRootPane();

    f32 scale = field_0x19C * sHio.mFootstepsScale;
    mVec2_c scaleV = mVec2_c(scale, scale);
    mpPane->SetScale(scaleV);

    f32 globalAlphaF = globalAlpha;

    for (int i = 0; i < mCurrentNumSteps; i++) {
        mVec2_c pos = mVec2_c(global->getField_0x20() + mFootprintPositions[i]);
        pane->SetTranslate(vec2ToVec3XY(pos));

        // TODO - FPR regswaps, maybe fewer temps would work
        f32 constantAlpha = sHio.mFootstepsAlpha / 255.0f;
        f32 alpha = globalAlphaF * field_0x19C;
        int range = mMaxNumSteps;
        f32 fadingFactor = ((f32)(range - i) / (f32)range);
        pane->SetAlpha(alpha * fadingFactor * constantAlpha);

        pane->CalculateMtx(mLyt.getDrawInfo());
        pane->Draw(mLyt.getDrawInfo());
    }
}

void dLytMapPutIcon_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("mapPutIcon_00.brlyt", nullptr);
    mVisible = true;
    mLyt.calc();
}

void dLytMapPutIcon_c::remove() {
    // no-op
}

void dLytMapPutIcon_c::execute() {
    mLyt.calc();
}

void dLytMapPutIcon_c::draw() {
    if (mVisible) {
        mLyt.draw();
    }
}

void dLytMapPutIcon_c::setPosition(const mVec2_c &pos) {
    mVec3_c pos3 = vec2ToVec3XY(mVec2_c(pos));
    mLyt.getLayout()->GetRootPane()->SetTranslate(pos3);
}

void dLytMapPutIcon_c::setScale(f32 scale) {
    mLyt.getLayout()->GetRootPane()->SetScale(mVec2_c(scale, scale));
}

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

#define LYT_MAP_PIN_ICON_ANIM_SCALE 0
#define LYT_MAP_PIN_ICON_ANIM_ERASE 1
#define LYT_MAP_PIN_ICON_ANIM_LOOP 2
#define LYT_MAP_PIN_ICON_NUM_ANIMS 3

void dLytMapPinIcon_c::initializeState_Wait() {
    mLyt.calc();
}
void dLytMapPinIcon_c::executeState_Wait() {
    bool keepGoing = false;
    if (field_0x1D0 != nullptr) {
        keepGoing = !field_0x1D0->field_0x04;
    }
    if (!keepGoing) {
        return;
    }

    if (!mIsSet) {
        return;
    }

    if (field_0x1BC == 0) {
        return;
    }

    mStateMgr.changeState(StateID_ToSelect);
}
void dLytMapPinIcon_c::finalizeState_Wait() {}

void dLytMapPinIcon_c::initializeState_ToSelect() {
    d2d::AnmGroup_c *m = &mAnmGroups[LYT_MAP_PIN_ICON_ANIM_ERASE];
    m->bind(false);
    m->setFrame(0.0f);
}
void dLytMapPinIcon_c::executeState_ToSelect() {
    mStateMgr.changeState(StateID_Select);
}
void dLytMapPinIcon_c::finalizeState_ToSelect() {
    field_0x1D0->field_0x04 = true;
}

void dLytMapPinIcon_c::initializeState_Select() {}
void dLytMapPinIcon_c::executeState_Select() {
    if (field_0x1D0->field_0x05 && dPad::getDownTrigC()) {
        removeBeacon();
        field_0x1D0->field_0x04 = false;
        d2d::AnmGroup_c *m = &mAnmGroups[LYT_MAP_PIN_ICON_ANIM_ERASE];
        if (m->isBound()) {
            m->unbind();
        }
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_BEACON_REMOVE);
        mStateMgr.changeState(StateID_Remove);
        return;
    }

    if (field_0x1BC == 0) {
        d2d::AnmGroup_c *m = &mAnmGroups[LYT_MAP_PIN_ICON_ANIM_ERASE];
        m->bind(false);
        m->setFrame(0.0f);
        mLyt.calc();
        m->unbind();
        mStateMgr.changeState(StateID_ToUnselect);
    } else {
        d2d::AnmGroup_c *m = &mAnmGroups[LYT_MAP_PIN_ICON_ANIM_ERASE];
        if (m->isBound()) {
            m->play();
        }
    }
}
void dLytMapPinIcon_c::finalizeState_Select() {}

void dLytMapPinIcon_c::initializeState_ToUnselect() {}
void dLytMapPinIcon_c::executeState_ToUnselect() {
    mStateMgr.changeState(StateID_Wait);
}
void dLytMapPinIcon_c::finalizeState_ToUnselect() {
    field_0x1D0->field_0x04 = false;
}

void dLytMapPinIcon_c::initializeState_Remove() {}
void dLytMapPinIcon_c::executeState_Remove() {
    mStateMgr.changeState(StateID_Wait);
}
void dLytMapPinIcon_c::finalizeState_Remove() {}

void dLytMapPinIcon_c::updatePosition() {
    dLytMapGlobal_c *global = getGlobal();
    if (mIsSet) {
        mVec2_c pos2d;
        global->projectOntoMap(pos2d, mPosition);
        pos2d += global->getField_0x20();
        mVec3_c translate3d = vec2ToVec3XY(pos2d);
        mLyt.getLayout()->GetRootPane()->SetTranslate(translate3d);
    }

    bool vis = mIsSet;
    mLyt.getLayout()->GetRootPane()->SetVisible(mIsSet);
    if (mpBounding != nullptr) {
        mpBounding->SetVisible(vis);
    }

    mLyt.getLayout()->GetRootPane()->SetAlpha(global->getAlpha());
    mLyt.getLayout()->GetRootPane()->SetInfluencedAlpha(true);
    mLyt.calc();
}

void dLytMapPinIcon_c::checkPointedAt() {
    field_0x1BC = 0;
    dCursorHitCheck_c *chk = dCsGame_c::GetInstance()->getCursorHit();
    if (chk != nullptr && chk->getType() == 'lyt ' &&
        static_cast<dCursorHitCheckLyt_c *>(chk)->getHitPane() == mpBounding) {
        field_0x1BC = 1;
    }
}

void dLytMapPinIcon_c::setPosition(const mVec3_c &pos) {
    d2d::AnmGroup_c *anm;

    mPosition = pos;
    dLytMapGlobal_c *global = getGlobal();
    mVec2_c pos2d;
    global->projectOntoMap(pos2d, pos);
    pos2d += global->getField_0x20();
    mVec3_c translate3d = vec2ToVec3XY(mVec2_c(pos2d));

    mLyt.getLayout()->GetRootPane()->SetTranslate(translate3d);
    mLyt.getLayout()->GetRootPane()->SetVisible(true);

    anm = &mAnmGroups[LYT_MAP_PIN_ICON_ANIM_ERASE];

    anm->bind(false);
    anm->setFrame(0.0f);
    mLyt.calc();
    anm->unbind();
    mIsSet = true;
}

void dLytMapPinIcon_c::removeBeacon() {
    mIsSet = false;
    if (mpBounding != nullptr) {
        mpBounding->SetVisible(false);
    }
    setBeaconPosition(nullptr, mIndex);
}

dLytMapPinIcon_c::~dLytMapPinIcon_c() {
    if (dCsMgr_c::GetInstance()->isRegist(&mCsHitCheck)) {
        dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitCheck);
    }
}

dLytMapGlobal_c *dLytMapPinIcon_c::getGlobal() const {
    return dLytMapGlobal_c::GetInstance();
}

static const d2d::LytBrlanMapping sMapPinIconBrlanMap[] = {
    {"mapPutIcon_00_scale.brlan", "G_scale_00"},
    {"mapPutIcon_00_erase.brlan", "G_scale_00"},
    { "mapPutIcon_00_loop.brlan",  "G_loop_00"},
};

bool dLytMapPinIcon_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("mapPutIcon_00.brlyt", nullptr);

    d2d::AnmGroup_c *pAnmGroups = mAnmGroups;

    for (int i = 0; i < LYT_MAP_PIN_ICON_NUM_ANIMS; i++) {
        pAnmGroups[i].init(sMapPinIconBrlanMap[i].mFile, resAcc, mLyt.getLayout(), sMapPinIconBrlanMap[i].mName);
        pAnmGroups[i].bind(false);
        pAnmGroups[i].setFrame(0.0f);
    }

    mLyt.calc();

    for (int i = 0; i < LYT_MAP_PIN_ICON_NUM_ANIMS; i++) {
        pAnmGroups[i].unbind();
    }

    mpBounding = mLyt.findBounding("B_mark_00");
    mCsHitCheck.init(mpBounding, 2, 1, 0);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitCheck);

    mAnmGroups[LYT_MAP_PIN_ICON_ANIM_SCALE].bind(false);
    mAnmGroups[LYT_MAP_PIN_ICON_ANIM_LOOP].bind(false);

    mLyt.calc();

    mStateMgr.changeState(StateID_Wait);
    field_0x1BC = 0;
    mIsSet = false;
    mLoopFrame = 0;
    return true;
}

bool dLytMapPinIcon_c::remove() {
    dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitCheck);
    for (int i = 0; i < LYT_MAP_PIN_ICON_NUM_ANIMS; i++) {
        mAnmGroups[i].remove();
    }
    return true;
}

void dLytMapPinIcon_c::setScale(f32 scale) {
    // TODO - probably inlines
    f32 frame = mAnmGroups[LYT_MAP_PIN_ICON_ANIM_SCALE].getLastFrame();
    d2d::AnmGroup_c *grp = &mAnmGroups[LYT_MAP_PIN_ICON_ANIM_SCALE];
    grp->setFrame(scale * frame);
    mLyt.calc();
}

bool dLytMapPinIcon_c::execute() {
    checkPointedAt();
    mStateMgr.executeState();
    field_0x1BC = 0;
    d2d::AnmGroup_c *grp = &mAnmGroups[LYT_MAP_PIN_ICON_ANIM_LOOP];
    grp->setFrame(mLoopFrame);

    // TODO - probably inlines
    dLytMapGlobal_c *global = getGlobal();
    f32 frame = mAnmGroups[LYT_MAP_PIN_ICON_ANIM_SCALE].getLastFrame();
    grp = &mAnmGroups[LYT_MAP_PIN_ICON_ANIM_SCALE];
    f32 tmp = global->getZoomFrame();
    grp->setFrame(tmp * frame);

    mLyt.calc();
    mCsHitCheck.resetCachedHitboxes();
    mCsHitCheck.execute();
    return true;
}

bool dLytMapPinIcon_c::draw() {
    mLyt.draw();
    return true;
}

void dLytMapPinIconAggregate_c::initializeState_Wait() {}
void dLytMapPinIconAggregate_c::executeState_Wait() {
    for (int i = 0; i < (int)ARRAY_LENGTH(mPins); i++) {
        if (mPins[i].isSelect()) {
            mStateMgr.changeState(StateID_Select);
        }
    }
}
void dLytMapPinIconAggregate_c::finalizeState_Wait() {}

void dLytMapPinIconAggregate_c::initializeState_Select() {}
void dLytMapPinIconAggregate_c::executeState_Select() {
    bool anyRemove = false;
    for (int i = 0; i < (int)ARRAY_LENGTH(mPins); i++) {
        if (mPins[i].isRemove()) {
            mStateMgr.changeState(StateID_Remove);
            anyRemove = true;
            // a bit convoluted, an early return would've done the trick
            break;
        }
    }

    if (!anyRemove) {
        bool allSelect = true;
        for (int i = 0; i < (int)ARRAY_LENGTH(mPins); i++) {
            if (mPins[i].isSelect()) {
                allSelect = false;
                break;
            }
        }

        if (allSelect) {
            mStateMgr.changeState(StateID_Wait);
        }
    }
}
void dLytMapPinIconAggregate_c::finalizeState_Select() {}

void dLytMapPinIconAggregate_c::initializeState_Remove() {}
void dLytMapPinIconAggregate_c::executeState_Remove() {
    field_0x9A0->field_0x04 = false;
    mStateMgr.changeState(StateID_Wait);
}
void dLytMapPinIconAggregate_c::finalizeState_Remove() {}

bool dLytMapPinIconAggregate_c::findNewBeaconId(s32 *pOutId) {
    bool ret = false;
    for (s32 idx = 0; idx < getMaxBeaconCount(); idx++) {
        s32 existingIndex = mPins[idx].mIndex;
        if (!mPins[idx].mIsSet) {
            // If we have an unused id, use that
            *pOutId = idx;
            ret = true;
            break;
        }

        // Otherwise use the lowest id ("oldest beacon"?)
        if (existingIndex < *pOutId) {
            *pOutId = existingIndex;
        }
    }

    return ret;
}

void dLytMapPinIconAggregate_c::setScale(f32 scale) {
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        mPins[idx].setScale(scale);
    }
}

void dLytMapPinIconAggregate_c::updatePosition() {
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        mPins[idx].updatePosition();
    }
}

void dLytMapPinIconAggregate_c::setUnk(LytMap0x80520B5C *unk) {
    field_0x9A0 = unk;
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        mPins[idx].field_0x1D0 = unk;
    }
}

bool dLytMapPinIconAggregate_c::setPosition(s32 index, const mVec3_c &position) {
    bool ret = false;
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        if (mPins[idx].mIndex == index) {
            mPins[idx].setPosition(position);
            ret = true;
            break;
        }
    }
    return ret;
}

void dLytMapPinIconAggregate_c::unsetAll() {
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        mPins[idx].mIsSet = false;
    }
}

u32 dLytMapPinIconAggregate_c::getNumSetPins() const {
    s32 num = 0;
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        if (mPins[idx].mIsSet) {
            num++;
        }
    }
    return num;
}

bool dLytMapPinIconAggregate_c::build(d2d::ResAccIf_c *resAcc) {
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        mPins[idx].build(resAcc);
    }
    mLoopFrame = 0;
    mLoopFrameMax = mPins[0].mAnmGroups[LYT_MAP_PIN_ICON_ANIM_LOOP].getAnimDuration();
    mStateMgr.changeState(StateID_Wait);
    return true;
}

bool dLytMapPinIconAggregate_c::remove() {
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        mPins[idx].remove();
    }
    return true;
}

bool dLytMapPinIconAggregate_c::execute() {
    mLoopFrame++;
    if (mLoopFrame > mLoopFrameMax) {
        mLoopFrame = 0;
    }
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        mPins[idx].mLoopFrame = mLoopFrame;
        mPins[idx].execute();
    }
    mStateMgr.executeState();
    return true;
}

bool dLytMapPinIconAggregate_c::draw() {
    for (int idx = 0; idx < (int)ARRAY_LENGTH(mPins); idx++) {
        mPins[idx].draw();
    }
    return true;
}

void dLytMapFloorBtn_c::init() {
    mStateMgr.changeState(StateID_Wait);
}

void dLytMapFloorBtn_c::execute() {
    mStateMgr.executeState();
}

void dLytMapFloorBtn_c::directlyDecide() {
    if (mpAnmGroups->mOnOff.isBound()) {
        mpAnmGroups->mOnOff.unbind();
    }
    if (mpAnmGroups->mDecide.isBound()) {
        mpAnmGroups->mDecide.unbind();
    }
    mpAnmGroups->mOnOffLight.bind(false);
    mpAnmGroups->mOnOffLight.setToEnd();
    mpOwnerLyt->calc();
    mpAnmGroups->mOnOffLight.unbind();
}

void dLytMapFloorBtn_c::directlyUndecide() {
    if (mpAnmGroups->mOnOffLight.isBound()) {
        mpAnmGroups->mOnOffLight.unbind();
    }
    if (mpAnmGroups->mDecide.isBound()) {
        mpAnmGroups->mDecide.unbind();
    }
    mpAnmGroups->mOnOff.bind(false);
    mpAnmGroups->mOnOff.setFrame(0.0f);
    mpOwnerLyt->calc();
    mpAnmGroups->mOnOff.unbind();
}

void dLytMapFloorBtn_c::initializeState_Wait() {}
void dLytMapFloorBtn_c::executeState_Wait() {
    if (mSelected) {
        dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
        mpAnmGroups->mOnOff.bind(false);
        mpAnmGroups->mOnOff.setFrame(0.0f);
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_POINT_FLOOR);
        mStateMgr.changeState(StateID_ToSelect);
    }
}
void dLytMapFloorBtn_c::finalizeState_Wait() {}

void dLytMapFloorBtn_c::initializeState_ToSelect() {}
void dLytMapFloorBtn_c::executeState_ToSelect() {
    if (mpAnmGroups->mOnOff.isEndReached()) {
        mpOwnerLyt->calc();
        mpAnmGroups->mOnOff.unbind();
        mStateMgr.changeState(StateID_Select);
    }
}
void dLytMapFloorBtn_c::finalizeState_ToSelect() {}

void dLytMapFloorBtn_c::initializeState_Select() {}
void dLytMapFloorBtn_c::executeState_Select() {
    if (!mSelected) {
        mpAnmGroups->mOnOff.bind(false);
        mpAnmGroups->mOnOff.setBackward();
        mpAnmGroups->mOnOff.setToStart();
        mStateMgr.changeState(StateID_ToWait);
    } else if (mDecided) {
        mpAnmGroups->mDecide.bind(false);
        mpAnmGroups->mDecide.setFrame(0.0f);
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_POINT_SELECT_FLOOR);
        mStateMgr.changeState(StateID_ToSelectInDecide);
    }
}
void dLytMapFloorBtn_c::finalizeState_Select() {}

void dLytMapFloorBtn_c::initializeState_ToWait() {}
void dLytMapFloorBtn_c::executeState_ToWait() {
    if (mpAnmGroups->mOnOff.isStop2()) {
        mpOwnerLyt->calc();
        mpAnmGroups->mOnOff.unbind();
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMapFloorBtn_c::finalizeState_ToWait() {}

void dLytMapFloorBtn_c::initializeState_ToSelectInDecide() {}
void dLytMapFloorBtn_c::executeState_ToSelectInDecide() {
    if (mpAnmGroups->mDecide.isStop2()) {
        mpOwnerLyt->calc();
        mpAnmGroups->mDecide.unbind();
        mStateMgr.changeState(StateID_SelectInDecide);
    }
}
void dLytMapFloorBtn_c::finalizeState_ToSelectInDecide() {}

void dLytMapFloorBtn_c::initializeState_SelectInDecide() {}
void dLytMapFloorBtn_c::executeState_SelectInDecide() {
    if (!mSelected) {
        mpAnmGroups->mOnOffLight.bind(false);
        mpAnmGroups->mOnOffLight.setToStart();
        mStateMgr.changeState(StateID_ToDecide);
    }
}
void dLytMapFloorBtn_c::finalizeState_SelectInDecide() {}

void dLytMapFloorBtn_c::initializeState_ToDecide() {}
void dLytMapFloorBtn_c::executeState_ToDecide() {
    if (mpAnmGroups->mOnOffLight.isStop2()) {
        mpAnmGroups->mOnOffLight.unbind();
        mpOwnerLyt->calc();
        mStateMgr.changeState(StateID_Decide);
    }
}
void dLytMapFloorBtn_c::finalizeState_ToDecide() {}

void dLytMapFloorBtn_c::initializeState_Decide() {}
void dLytMapFloorBtn_c::executeState_Decide() {
    if (mSelected) {
        mpAnmGroups->mOnOffLight.bind(false);
        mpAnmGroups->mOnOffLight.setBackward();
        mpAnmGroups->mOnOffLight.setToStart();
        dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_POINT_FLOOR);
        mStateMgr.changeState(StateID_DecideToSelectInDecide);
    } else if (!mDecided) {
        mpAnmGroups->mOnOff.bind(false);
        mpAnmGroups->mOnOff.setFrame(0.0f);
        mpOwnerLyt->getLayout()->Animate(0);
        mpOwnerLyt->calc();
        mpAnmGroups->mOnOff.unbind();
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMapFloorBtn_c::finalizeState_Decide() {}

void dLytMapFloorBtn_c::initializeState_DecideToSelectInDecide() {}
void dLytMapFloorBtn_c::executeState_DecideToSelectInDecide() {
    if (mpAnmGroups->mOnOffLight.isStop2()) {
        mpOwnerLyt->calc();
        mpAnmGroups->mOnOffLight.unbind();
        mStateMgr.changeState(StateID_SelectInDecide);
    }
}
void dLytMapFloorBtn_c::finalizeState_DecideToSelectInDecide() {}

static const char *sFloorBtnBoundings[] = {
    "B_choices_00",
    "B_choices_01",
    "B_choices_02",
    "B_choices_03",
};

static const char *sFloorBtnText[] = {
    "T_text_00",
    "T_text_01",
    "T_text_02",
    "T_text_03",
};

static const char *sFloorBtnTextS[] = {
    "T_textS_00",
    "T_textS_01",
    "T_textS_02",
    "T_textS_03",
};

static const d2d::LytBrlanMapping brlanMapFloorBtnMgr[] = {
    {"mapFloorBtn_00_btnV.brlan", "G_btnV_00"},
};

#define MAP_FLOOR_BTN_MGR_ANIM_BTNV 0
#define MAP_FLOOR_BTN_MGR_NUM_ANIMS 1

static const char *brlanListFloorBtn[] = {
    "mapFloorBtn_00_btnLoop.brlan",
    "mapFloorBtn_00_btnOnOffNormal.brlan",
    "mapFloorBtn_00_btnDecideNormal.brlan",
    "mapFloorBtn_00_btnOnOffLight.brlan",
};

static const char *groupListFloorBtn[][4] = {
    {"G_btnLoop_00", "G_btn_00", "G_btn_00", "G_btn_00"},
    {"G_btnLoop_01", "G_btn_01", "G_btn_01", "G_btn_01"},
    {"G_btnLoop_02", "G_btn_02", "G_btn_02", "G_btn_02"},
    {"G_btnLoop_03", "G_btn_03", "G_btn_03", "G_btn_03"},
};

#define MAP_FLOOR_BTN_MGR_NUM_BTNS 4

bool dLytMapFloorBtnMgr_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("mapFloorBtn_00.brlyt", nullptr);

    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_ANIMS; i++) {
        mAnmGroups[i].init(brlanMapFloorBtnMgr[i].mFile, resAcc, mLyt.getLayout(), brlanMapFloorBtnMgr[i].mName);
    }

    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        mBtnGroups[i].mLoop.init(brlanListFloorBtn[0], resAcc, mLyt.getLayout(), groupListFloorBtn[i][0]);
        mBtnGroups[i].mOnOff.init(brlanListFloorBtn[1], resAcc, mLyt.getLayout(), groupListFloorBtn[i][1]);
        mBtnGroups[i].mDecide.init(brlanListFloorBtn[2], resAcc, mLyt.getLayout(), groupListFloorBtn[i][2]);
        mBtnGroups[i].mOnOffLight.init(brlanListFloorBtn[3], resAcc, mLyt.getLayout(), groupListFloorBtn[i][3]);
    }

    nw4r::lyt::Bounding *bounding;
    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        bounding = mLyt.findBounding(sFloorBtnBoundings[i]);
        mFloorBtns[i].init();
        mFloorBtns[i].mpBounding = bounding;
        bounding->SetVisible(false);
        mCsHitChecks[i].init(bounding, 0x2, 0, 0);
        dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitChecks[i]);
    }

    mpPane = mLyt.findPane("N_all_00");
    mpPane->SetInfluencedAlpha(true);

    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        mFloorBtns[i].mpOwnerLyt = &mLyt;
        mFloorBtns[i].mpAnmGroups = &mBtnGroups[i];
    }

    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        mBtnGroups[i].mOnOff.bind(false);
        mBtnGroups[i].mOnOff.setFrame(0.0f);
    }

    mLyt.getLayout()->Animate(0);

    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        mBtnGroups[i].mOnOff.unbind();
    }

    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        mBtnGroups[i].mLoop.bind(false);
    }

    mLyt.calc();

    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        mCsHitChecks[i].resetCachedHitboxes();
        mCsHitChecks[i].execute();
    }

    mPointerVisible = false;
    mPrevPointerVisible = false;

    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        mFloorBtns[i].mSelected = false;
    }

    mStateMgr.changeState(StateID_Invisible);

    return true;
}

bool dLytMapFloorBtnMgr_c::remove() {
    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitChecks[i]);
    }

    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        mBtnGroups[i].mOnOff.remove();
        mBtnGroups[i].mLoop.remove();
        mBtnGroups[i].mDecide.remove();
        mBtnGroups[i].mOnOffLight.remove();
    }
    // not removing our own anm group...

    dPadNav::setNavEnabled(false, false);

    return true;
}

bool dLytMapFloorBtnMgr_c::execute() {
    field_0x70C = 4;
    if (*mStateMgr.getStateID() != StateID_Invisible) {
        if (mPointerVisible) {
            for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
                mFloorBtns[i].execute();
            }
            mStateMgr.executeState();
        } else {
            // Determine the currently active floor
            s32 activeBtn = 0;
            s32 direction = dPadNav::getFSStickNavDirection();
            if (mPointerVisible != mPrevPointerVisible) {
                // We just turned off pointing, so make the decided button the selected one too
                for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
                    if (mFloorBtns[i].mDecided) {
                        activeBtn = i;
                    }
                }
            } else {
                // Normal handling - pointing off, selected button stays selected
                for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
                    if (mFloorBtns[i].mSelected) {
                        activeBtn = i;
                    }
                }
            }

            // Handle navigation
            if (activeBtn > 0 && (direction == dPadNav::FS_STICK_UP || direction == dPadNav::FS_STICK_UP_RIGHT ||
                                  direction == dPadNav::FS_STICK_UP_LEFT)) {
                activeBtn--;
            }

            if (activeBtn < mNumFloors - 1 &&
                (direction == dPadNav::FS_STICK_DOWN || direction == dPadNav::FS_STICK_DOWN_RIGHT ||
                 direction == dPadNav::FS_STICK_DOWN_LEFT)) {
                activeBtn++;
            }

            for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
                if (i == activeBtn) {
                    mFloorBtns[i].mSelected = true;
                    if (dCsBase_c::GetInstance() != nullptr) {
                        dCsBase_c::GetInstance()->setCursorStickTargetPane(mFloorBtns[i].mpBounding);
                    }
                } else {
                    mFloorBtns[i].mSelected = false;
                }
            }
            for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
                mFloorBtns[i].execute();
            }
        }

        for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
            mBtnGroups[i].mLoop.play();
            if (mBtnGroups[i].mOnOff.isBound()) {
                mBtnGroups[i].mOnOff.play();
            }
            if (mBtnGroups[i].mDecide.isBound()) {
                mBtnGroups[i].mDecide.play();
            }
            if (mBtnGroups[i].mOnOffLight.isBound()) {
                mBtnGroups[i].mOnOffLight.play();
            }
        }
    }
    mLyt.calc();
    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        mCsHitChecks[i].resetCachedHitboxes();
        mCsHitChecks[i].execute();
    }

    return true;
}

void dLytMapFloorBtnMgr_c::checkPointedAtBtn() {
    mPrevPointerVisible = mPointerVisible;
    mPointerVisible = dPadNav::isPointerVisible();
    if (mForceNoNav) {
        dPadNav::stopFSStickNav();
        mPointerVisible = true;
    }

    if (mPointerVisible) {
        for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
            mFloorBtns[i].mSelected = false;
        }

        dCursorHitCheck_c *chk = dCsGame_c::GetInstance()->getCursorHit();
        if (chk != nullptr && chk->getType() == 'lyt ') {
            for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
                if (chk == &mCsHitChecks[i]) {
                    mFloorBtns[i].mSelected = true;
                }
            }
        }
    }
}

s32 dLytMapFloorBtnMgr_c::getPointedAtBtnIdx() const {
    s32 idx = -1;
    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        if (mFloorBtns[i].mSelected) {
            idx = i;
            break;
        }
    }
    return idx;
}

bool dLytMapFloorBtnMgr_c::isUsingPointerNav() const {
    return mPointerVisible;
}

void dLytMapFloorBtnMgr_c::resetFloor(s32 newFloorBtn) {
    mFloorBtns[mSelectedBtnIdx].mDecided = false;
    mFloorBtns[mSelectedBtnIdx].directlyUndecide();
    mFloorBtns[mSelectedBtnIdx].mStateMgr.changeState(dLytMapFloorBtn_c::StateID_Wait);

    mFloorBtns[newFloorBtn].mDecided = true;
    mFloorBtns[newFloorBtn].directlyDecide();
    mFloorBtns[newFloorBtn].mStateMgr.changeState(dLytMapFloorBtn_c::StateID_Decide);

    mSelectedBtnIdx = newFloorBtn;
}

void dLytMapFloorBtnMgr_c::decideSelectedFloor() {
    for (int i = 0; i < mNumFloors; i++) {
        if (mFloorBtns[i].mSelected) {
            if (i != mSelectedBtnIdx) {
                mFloorBtns[mSelectedBtnIdx].mDecided = false;
                mFloorBtns[i].mDecided = true;
            }
            mSelectedBtnIdx = i;
        }
    }
}

bool dLytMapFloorBtnMgr_c::canDecideFloor() const {
    bool ret = false;

    s32 idx = getPointedAtBtnIdx();
    if (idx >= 0) {
        // When pointing at a button that is thus selected but not yet decided,
        // we can decide on that button if it's not busy animating...
        if (!mFloorBtns[idx].mDecided) {
            if (mFloorBtns[idx].mStateMgr.isState(dLytMapFloorBtn_c::StateID_Select)) {
                bool busy = mFloorBtns[idx].mpAnmGroups->mOnOff.isBound() ||
                            mFloorBtns[idx].mpAnmGroups->mDecide.isBound() ||
                            mFloorBtns[idx].mpAnmGroups->mOnOffLight.isBound();
                if (!busy) {
                    ret = true;
                }
            }
        }
    }

    for (int i = 0; i < mNumFloors; i++) {
        // But if any other button is animating and in a changing state, also forbid
        if (i != idx) {
            bool busy = mFloorBtns[i].mpAnmGroups->mOnOff.isBound() || mFloorBtns[i].mpAnmGroups->mDecide.isBound() ||
                        mFloorBtns[i].mpAnmGroups->mOnOffLight.isBound();

            if (busy) {
                if (!mFloorBtns[i].mStateMgr.isState(dLytMapFloorBtn_c::StateID_Wait) &&
                    !mFloorBtns[i].mStateMgr.isState(dLytMapFloorBtn_c::StateID_Decide)) {
                    ret = false;
                    break;
                }
            }
        }
    }

    return ret;
}

s32 dLytMapFloorBtnMgr_c::getCurrentFloor() const {
    s32 ret = -1;
    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        if (mFloorBtns[i].mDecided) {
            ret = i;
        }
    }
    return mBaseFloorOffset - ret;
}

// What it says on the tin
static const char *sFloorBtnLabels[] = {
    "FLOOR_F3", "FLOOR_F2", "FLOOR_F1", "FLOOR_B1", "FLOOR_B2", "FLOOR_B3",
};

void dLytMapFloorBtnMgr_c::init(s32 numFloors, s32 baseFloorOffset, s32 selectedFloor) {
    mNumFloors = numFloors;
    mBaseFloorOffset = baseFloorOffset;
    for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
        bool visible = i < mNumFloors && mNumFloors >= 2;
        mLyt.findPane(sFloorBtnBoundings[i])->SetVisible(visible);
    }

    if (mNumFloors >= 2) {
        mLyt.getLayout()->GetRootPane()->SetVisible(true);
        mAnmGroups[0].bind(false);
        mAnmGroups[0].setFrame(mNumFloors - 2);

        for (int i = 0; i < MAP_FLOOR_BTN_MGR_NUM_BTNS; i++) {
            bool visible = i < mNumFloors && mNumFloors >= 2;
            mLyt.findPane(sFloorBtnBoundings[i])->SetVisible(visible);
            if (visible) {
                int labelIdx = (2 - mBaseFloorOffset) + i;
                dTextBox_c *textBox = mLyt.getTextBox(sFloorBtnText[i]);
                textBox->setMessageWithGlobalTextProcessor2(sFloorBtnLabels[labelIdx], nullptr);
                textBox = mLyt.getTextBox(sFloorBtnTextS[i]);
                textBox->setMessageWithGlobalTextProcessor2(sFloorBtnLabels[labelIdx], nullptr);
            }
        }

        // Floor buttons are descending but selectedFloor is ascending
        s32 tmp = -(selectedFloor - baseFloorOffset);
        mSelectedBtnIdx = tmp;
        mFloorBtns[tmp].mDecided = true;
        mFloorBtns[tmp].mStateMgr.changeState(dLytMapFloorBtn_c::StateID_Decide);

        // TODO probably an inline
        d2d::AnmGroup_c &anm = mBtnGroups[tmp].mOnOffLight;

        anm.bind(false);
        mBtnGroups[tmp].mOnOffLight.setToEnd();
        mLyt.getLayout()->Animate(0);
        mLyt.calc();
        anm.unbind();
        if (mAnmGroups[0].bind(false)) {
            mAnmGroups[0].unbind();
        }
    } else {
        mLyt.getLayout()->GetRootPane()->SetVisible(false);
    }
}

void dLytMapFloorBtnMgr_c::initializeState_Invisible() {}
void dLytMapFloorBtnMgr_c::executeState_Invisible() {}
void dLytMapFloorBtnMgr_c::finalizeState_Invisible() {}

void dLytMapFloorBtnMgr_c::initializeState_Wait() {}
void dLytMapFloorBtnMgr_c::executeState_Wait() {}
void dLytMapFloorBtnMgr_c::finalizeState_Wait() {}

dLytMapFloorBtnMgr_c::~dLytMapFloorBtnMgr_c() {
    for (int i = 0; i < 4; i++) {
        if (dCsMgr_c::GetInstance()->isRegist(&mCsHitChecks[i])) {
            dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitChecks[i]);
        }
    }
    dPadNav::setNavEnabled(false, false);
}

#define MAP_POPUP_INFO_ANIM_IN 0
#define MAP_POPUP_INFO_ANIM_OUT 1
#define MAP_POPUP_INFO_ANIM_INPUT 2

#define MAP_POPUP_INFO_NUM_ANIMS 3

void dLytMapPopupInfo_c::initializeState_Invisible() {}
void dLytMapPopupInfo_c::executeState_Invisible() {}
void dLytMapPopupInfo_c::finalizeState_Invisible() {}

void dLytMapPopupInfo_c::initializeState_In() {
    mAnmGroups[MAP_POPUP_INFO_ANIM_IN].bind(false);
}
void dLytMapPopupInfo_c::executeState_In() {
    if (mAnmGroups[MAP_POPUP_INFO_ANIM_IN].isEndReached()) {
        mLyt.calc();
        mAnmGroups[MAP_POPUP_INFO_ANIM_IN].unbind();
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytMapPopupInfo_c::finalizeState_In() {}

void dLytMapPopupInfo_c::initializeState_Wait() {}
void dLytMapPopupInfo_c::executeState_Wait() {
    if (dLytMsgWindow_c::getInstance()->isVisible()) {
        mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].bind(false);
        mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].setForward();
        mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].setToStart();
        mStateMgr.changeState(StateID_WaitInvalid);
    } else if (mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].isStop2()) {
        mLyt.calc();
        mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].unbind();
    }
}
void dLytMapPopupInfo_c::finalizeState_Wait() {}

void dLytMapPopupInfo_c::initializeState_WaitInvalid() {}
void dLytMapPopupInfo_c::executeState_WaitInvalid() {
    if (!dLytMsgWindow_c::getInstance()->isVisible()) {
        mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].bind(false);
        mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].setBackward();
        mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].setToStart();
        mStateMgr.changeState(StateID_Wait);
    } else if (mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].isStop2()) {
        mLyt.calc();
        mAnmGroups[MAP_POPUP_INFO_ANIM_INPUT].unbind();
    }
}
void dLytMapPopupInfo_c::finalizeState_WaitInvalid() {}

void dLytMapPopupInfo_c::initializeState_Out() {
    mAnmGroups[MAP_POPUP_INFO_ANIM_OUT].bind(false);
}
void dLytMapPopupInfo_c::executeState_Out() {
    if (mAnmGroups[MAP_POPUP_INFO_ANIM_OUT].isEndReached()) {
        mLyt.calc();
        mAnmGroups[MAP_POPUP_INFO_ANIM_OUT].unbind();
    }
}
void dLytMapPopupInfo_c::finalizeState_Out() {}

static const d2d::LytBrlanMapping sMapPopupInfoBrlanMap[] = {
    {   "mapPopupInfo_00_in.brlan", "G_inOut_00"},
    {  "mapPopupInfo_00_out.brlan", "G_inOut_00"},
    {"mapPopupInfo_00_inPut.brlan", "G_input_00"},
};

void dLytMapPopupInfo_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("mapPopupInfo_00.brlyt", nullptr);
    for (int i = 0; i < MAP_POPUP_INFO_NUM_ANIMS; i++) {
        mAnmGroups[i].init(sMapPopupInfoBrlanMap[i].mFile, resAcc, mLyt.getLayout(), sMapPopupInfoBrlanMap[i].mName);
    }
    mStateMgr.changeState(StateID_Invisible);
}

void dLytMapPopupInfo_c::remove() {
    for (int i = 0; i < MAP_POPUP_INFO_NUM_ANIMS; i++) {
        mAnmGroups[i].remove();
    }
}

void dLytMapPopupInfo_c::execute() {
    mStateMgr.executeState();
    mLyt.calc();
    for (int i = 0; i < MAP_POPUP_INFO_NUM_ANIMS; i++) {
        if (mAnmGroups[i].isBound()) {
            mAnmGroups[i].play();
        }
    }
}

void dLytMapPopupInfo_c::draw() {
    mLyt.draw();
}

#define MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT 0
#define MAP_SAVE_POPUP_ACTION_ANIM_INPUT 1

#define MAP_SAVE_POPUP_ACTION_NUM_ANIMS 2

void dLytMapSavePopupAction_c::initializeState_Invisible() {}
void dLytMapSavePopupAction_c::executeState_Invisible() {
    if (mInRequest) {
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMapSavePopupAction_c::finalizeState_Invisible() {}

void dLytMapSavePopupAction_c::initializeState_In() {
    // TODO - explain why this seems unnecessarily complicated
    mpAnmGroupInOut->bind(false);
    mpAnmGroupInOut->setForward();
    mpAnmGroupInOut->setToStart();
    mInOutFrame = mpAnmGroupInOut->getFrame();
    mpAnmGroupInOut->unbind();
}
void dLytMapSavePopupAction_c::executeState_In() {
    // TODO - explain why this seems unnecessarily complicated
    mpAnmGroupInOut->bind(false);
    mpAnmGroupInOut->setFrame(mInOutFrame);
    mpAnmGroupInOut->play();
    if (mpAnmGroupInOut->isEndReached()) {
        mStateMgr.changeState(StateID_Wait);
    }
    mInOutFrame = mpAnmGroupInOut->getFrame();
    mpAnmGroupInOut->unbind();
}
void dLytMapSavePopupAction_c::finalizeState_In() {}

void dLytMapSavePopupAction_c::initializeState_Wait() {}
void dLytMapSavePopupAction_c::executeState_Wait() {
    if (!mInRequest) {
        mStateMgr.changeState(StateID_Out);
    }
}
void dLytMapSavePopupAction_c::finalizeState_Wait() {}

void dLytMapSavePopupAction_c::initializeState_Out() {
    // TODO - explain why this seems unnecessarily complicated
    mpAnmGroupInOut->bind(false);
    mpAnmGroupInOut->setBackward();
    mpAnmGroupInOut->setToStart();
    mInOutFrame = mpAnmGroupInOut->getFrame();
    mpAnmGroupInOut->unbind();
}
void dLytMapSavePopupAction_c::executeState_Out() {
    // TODO - explain why this seems unnecessarily complicated
    mpAnmGroupInOut->bind(false);
    mpAnmGroupInOut->setBackward();
    mpAnmGroupInOut->setFrame(mInOutFrame);
    mpAnmGroupInOut->play();
    if (mpAnmGroupInOut->isStop2()) {
        mStateMgr.changeState(StateID_Wait);
    }
    mInOutFrame = mpAnmGroupInOut->getFrame();
    mpAnmGroupInOut->unbind();
}
void dLytMapSavePopupAction_c::finalizeState_Out() {}

void dLytMapSavePopupAction_c::execute() {
    mStateMgr.executeState();
}

void dLytMapSavePopupAction_c::init(d2d::AnmGroup_c *pGroups) {
    mpAnmGroupInOut = &pGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT];
    mpAnmGroupInput = &pGroups[MAP_SAVE_POPUP_ACTION_ANIM_INPUT];
}

void dLytMapSavePopupAction_c::hide() {
    mpAnmGroupInOut->bind(false);
    mpAnmGroupInOut->setFrame(0.0f);
    mpAnmGroupInOut->unbind();
    mStateMgr.changeState(StateID_Invisible);
}

static const d2d::LytBrlanMapping sMapSavePopupActionBrlanMap[] = {
    {"mapPopup_00_inOut.brlan", "G_inOut_00"},
    {"mapPopup_00_scale.brlan", "G_scale_00"},
};

void dLytMapSavePopup_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("mapPopup_00.brlyt", nullptr);
    for (int i = 0; i < MAP_SAVE_POPUP_ACTION_NUM_ANIMS; i++) {
        mAnmGroups[i].init(
            sMapSavePopupActionBrlanMap[i].mFile, resAcc, mLyt.getLayout(), sMapSavePopupActionBrlanMap[i].mName
        );
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(mActions); i++) {
        mActions[i].init(mAnmGroups);
        mActions[i].mStateMgr.changeState(dLytMapSavePopupAction_c::StateID_Invisible);
    }
}

void dLytMapSavePopup_c::remove() {
    for (int i = 0; i < MAP_SAVE_POPUP_ACTION_NUM_ANIMS; i++) {
        mAnmGroups[i].remove();
    }
}

void dLytMapSavePopup_c::execute() {
    for (int i = 0; i < mStatueNum; i++) {
        mActions[i].mInRequest = mCurrentlyInStatue == i;
    }

    for (int i = 0; i < mStatueNum; i++) {
        mActions[i].execute();
    }
}

void dLytMapSavePopup_c::draw() {
    nw4r::lyt::Bounding *pBounding;

    f32 scaleX = mLyt.getDrawInfo().GetLocationAdjustScale().x;
    nw4r::lyt::Pane *rootPane = mLyt.getLayout()->GetRootPane();
    for (int i = 0; i < mStatueNum; i++) {
        const sStateIDIf_c *stateID = mActions[i].mStateMgr.getStateID();
        if (*stateID != dLytMapSavePopupAction_c::StateID_Invisible) {
            pBounding = mpStatueBoundings[i];
            mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_INPUT].bind(false);
            mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_INPUT].setFrame(30.0f);
            const dMapSaveObjDefinition *def = dLytMap_c::GetInstance()->getSaveObjDefinition(i);
            f32 width = setStatueLabel(def->statueLabel);

            f32 allScaleX = mLyt.findPane("N_all_00")->GetScale().x;
            // TODO fadds regswap
            f32 x = pBounding->GetGlobalMtx()._03 + (0.5f * width * allScaleX * scaleX) + sHio.field_0x04;
            f32 y = pBounding->GetGlobalMtx()._13;
            mVec2_c translate2d(x, y);
            mVec3_c translate = vec2ToVec3XY(translate2d);
            rootPane->SetTranslate(translate);

            if (*stateID == dLytMapSavePopupAction_c::StateID_In) {
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].bind(false);
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].setFrame(mActions[i].mInOutFrame);
                mLyt.calc();
                mLyt.draw();
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].unbind();
            } else if (*stateID == dLytMapSavePopupAction_c::StateID_Wait) {
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].bind(false);
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].setToEnd();
                mLyt.calc();
                mLyt.draw();
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].unbind();
            } else if (*stateID == dLytMapSavePopupAction_c::StateID_Out) {
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].bind(false);
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].setBackward();
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].setFrame(mActions[i].mInOutFrame);
                mLyt.calc();
                mLyt.draw();
                mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_IN_OUT].unbind();
            }
            mAnmGroups[MAP_SAVE_POPUP_ACTION_ANIM_INPUT].unbind();
        }
    }
}

void dLytMapSavePopup_c::set(nw4r::lyt::Bounding **pStatueBoundings, s32 count) {
    mpStatueBoundings = pStatueBoundings;
    mStatueNum = count;
}

f32 dLytMapSavePopup_c::setStatueLabel(const char *label) {
    static const char *sTPopupS = "T_popupS_00";
    static const char *sTPopup = "T_popup_00";
    static const char *sBg = "W_bgP_00";
    dTextBox_c *textBox;

    textBox = mLyt.getTextBox(sTPopupS);
    textBox->setMessageWithGlobalTextProcessor2(label, nullptr);
    textBox->GetLineWidth(nullptr);
    textBox = mLyt.getTextBox(sTPopup);
    textBox->setMessageWithGlobalTextProcessor2(label, nullptr);

    dWindow_c *w = mLyt.getWindow(sBg);
    w->UpdateSize(textBox, 60.0f);
    return w->GetSize().width;
}

void dLytMapSavePopup_c::hide(s32 statueIdx) {
    mActions[statueIdx].hide();
}

#define MAP_SAVE_CAPTION_ANIM_TYPE 0
#define MAP_SAVE_CAPTION_ANIM_TEXTLINE 1
#define MAP_SAVE_CAPTION_ANIM_IN 2
#define MAP_SAVE_CAPTION_ANIM_LOOP 3
#define MAP_SAVE_CAPTION_ANIM_OUT 4

#define MAP_SAVE_CAPTION_NUM_ANIMS 5

void dLytMapSaveCaption_c::initializeState_Invisible() {}
void dLytMapSaveCaption_c::executeState_Invisible() {}
void dLytMapSaveCaption_c::finalizeState_Invisible() {}

void dLytMapSaveCaption_c::initializeState_In() {
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_IN].bind(false);
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_IN].setFrame(0.0f);
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_LOOP].bind(false);
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_LOOP].setFrame(0.0f);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnmGroups[MAP_SAVE_CAPTION_ANIM_LOOP].unbind();
}
void dLytMapSaveCaption_c::executeState_In() {
    if (mAnmGroups[MAP_SAVE_CAPTION_ANIM_IN].isEndReached()) {
        mAnmGroups[MAP_SAVE_CAPTION_ANIM_LOOP].bind(false);
        mLyt.calc();
        mAnmGroups[MAP_SAVE_CAPTION_ANIM_IN].unbind();
        mStateMgr.changeState(StateID_In);
    }
}
void dLytMapSaveCaption_c::finalizeState_In() {}

void dLytMapSaveCaption_c::initializeState_Wait() {}
void dLytMapSaveCaption_c::executeState_Wait() {}
void dLytMapSaveCaption_c::finalizeState_Wait() {}

void dLytMapSaveCaption_c::initializeState_Out() {
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_OUT].bind(false);
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_OUT].setFrame(0.0f);
}
void dLytMapSaveCaption_c::executeState_Out() {
    if (mAnmGroups[MAP_SAVE_CAPTION_ANIM_OUT].isEndReached()) {
        mLyt.calc();
        mAnmGroups[MAP_SAVE_CAPTION_ANIM_OUT].unbind();
        mAnmGroups[MAP_SAVE_CAPTION_ANIM_LOOP].unbind();
        mStateMgr.changeState(StateID_Invisible);
    }
}
void dLytMapSaveCaption_c::finalizeState_Out() {}

static const d2d::LytBrlanMapping sMapSaveCaptionBrlanMap[] = {
    {    "mapSaveCaption_00_type.brlan",     "G_type_00"},
    {"mapSaveCaption_00_textLine.brlan", "G_textLine_00"},
    {      "mapSaveCaption_00_in.brlan",   "G__inOut_00"},
    {    "mapSaveCaption_00_loop.brlan",     "G_loop_00"},
    {     "mapSaveCaption_00_out.brlan",   "G__inOut_00"},
};

void dLytMapSaveCaption_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("mapSaveCaption_00.brlyt", nullptr);
    for (int i = 0; i < MAP_SAVE_CAPTION_NUM_ANIMS; i++) {
        mAnmGroups[i].init(
            sMapSaveCaptionBrlanMap[i].mFile, resAcc, mLyt.getLayout(), sMapSaveCaptionBrlanMap[i].mName
        );
    }

    mStateMgr.changeState(StateID_Invisible);
}

void dLytMapSaveCaption_c::remove() {
    for (int i = 0; i < MAP_SAVE_CAPTION_NUM_ANIMS; i++) {
        mAnmGroups[i].remove();
    }
}

void dLytMapSaveCaption_c::execute() {
    mStateMgr.executeState();
    for (int i = 0; i < MAP_SAVE_CAPTION_NUM_ANIMS; i++) {
        if (mAnmGroups[i].isBound()) {
            mAnmGroups[i].play();
        }
    }
    mLyt.calc();
}

void dLytMapSaveCaption_c::draw() {
    if (*mStateMgr.getStateID() != StateID_Invisible) {
        mLyt.draw();
    }
}

void dLytMapSaveCaption_c::setType(s32 type) {
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_TYPE].bind(false);
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_TYPE].setFrame(type);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnmGroups[MAP_SAVE_CAPTION_ANIM_TYPE].unbind();
}

void dLytMapSaveCaption_c::setLabel(const char *label) {
    mLyt.getTextBox("T_captionS_00")->setMessageWithGlobalTextProcessor2(label, nullptr);
    mLyt.getTextBox("T_caption_00")->setMessageWithGlobalTextProcessor2(label, nullptr);
    // ok
    const char *tmp = "T_captionS_00";
    dTextBox_c *box = mLyt.getTextBox(tmp);
    // Setting message twice...
    box->setMessageWithGlobalTextProcessor2(label, nullptr);

    f32 width = box->GetLineWidth(nullptr);
    const wchar_t *text = dMessage_c::getTextMessageByLabel(label, true, nullptr, 0);
    s32 lines = box->calcTextLines(text, dMessage_c::getGlobalTagProcessor());

    mAnmGroups[MAP_SAVE_CAPTION_ANIM_TEXTLINE].bind(false);
    if (width < 226.0f) {
        if (lines == 1) {
            mAnmGroups[MAP_SAVE_CAPTION_ANIM_TEXTLINE].setFrame(0.0f);
        } else {
            mAnmGroups[MAP_SAVE_CAPTION_ANIM_TEXTLINE].setFrame(1.0f);
        }
    } else {
        if (lines == 1) {
            mAnmGroups[MAP_SAVE_CAPTION_ANIM_TEXTLINE].setFrame(2.0f);
        } else {
            mAnmGroups[MAP_SAVE_CAPTION_ANIM_TEXTLINE].setFrame(3.0f);
        }
    }

    mLyt.getLayout()->Animate(0);
    mLyt.calc();
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_TEXTLINE].unbind();
}

#define MAP_SAVE_OBJ_ANIM_ON_OFF 0
#define MAP_SAVE_OBJ_ANIM_DECIDE 1
#define MAP_SAVE_OBJ_ANIM_LOOP 2

#define MAP_SAVE_OBJ_NUM_ANIMS 3

void dLytMapSaveObj_c::initializeState_Wait() {}
void dLytMapSaveObj_c::executeState_Wait() {
    if (mSelectRequest) {
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].bind(false);
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].setForward();
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].setFrame(0.0f);
        mStateMgr.changeState(StateID_ToSelect);
    }
}
void dLytMapSaveObj_c::finalizeState_Wait() {}

void dLytMapSaveObj_c::initializeState_ToSelect() {}
void dLytMapSaveObj_c::executeState_ToSelect() {
    if (mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].isEndReached()) {
        dRumble_c::start(dRumble_c::sRumblePreset1, dRumble_c::FLAG_SLOT0);
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_POINT_START_POINT);
        mLyt.calc();
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].unbind();
        mStateMgr.changeState(StateID_Select);
    }

    if (mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].isBound()) {
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].play();
    }
}
void dLytMapSaveObj_c::finalizeState_ToSelect() {}

void dLytMapSaveObj_c::initializeState_Select() {}
void dLytMapSaveObj_c::executeState_Select() {
    if (!mSelectRequest) {
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].bind(false);
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].setBackward();
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].setToStart();
        mDecideRequest = false;
        mStateMgr.changeState(StateID_ToWait);
    } else if (mDecideRequest) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_DECIDE_START_POINT);
        mAnmGroups[MAP_SAVE_OBJ_ANIM_DECIDE].bind(false);
        mAnmGroups[MAP_SAVE_OBJ_ANIM_DECIDE].setFrame(0.0f);
        mStateMgr.changeState(StateID_Decide);
    }
}
void dLytMapSaveObj_c::finalizeState_Select() {}

void dLytMapSaveObj_c::initializeState_ToWait() {}
void dLytMapSaveObj_c::executeState_ToWait() {
    if (mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].isStop2()) {
        mLyt.calc();
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].unbind();
        mStateMgr.changeState(StateID_Select);
    }

    if (mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].isBound()) {
        mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].play();
    }
}
void dLytMapSaveObj_c::finalizeState_ToWait() {}

void dLytMapSaveObj_c::initializeState_Decide() {}
void dLytMapSaveObj_c::executeState_Decide() {
    if (mAnmGroups[MAP_SAVE_OBJ_ANIM_DECIDE].isEndReached()) {
        mLyt.calc();
        mAnmGroups[MAP_SAVE_OBJ_ANIM_DECIDE].unbind();
        mDecideFinished = true;
    }

    if (mAnmGroups[MAP_SAVE_OBJ_ANIM_DECIDE].isBound()) {
        mAnmGroups[MAP_SAVE_OBJ_ANIM_DECIDE].play();
    }
}
void dLytMapSaveObj_c::finalizeState_Decide() {}

void dLytMapSaveObj_c::setPosition(const mVec3_c &position) {
    mLyt.getLayout()->GetRootPane()->SetTranslate(position);
}

void dLytMapSaveObj_c::setAlpha(u8 alpha) {
    mLyt.getLayout()->GetRootPane()->SetAlpha(alpha);
}

void dLytMapSaveObj_c::init() {
    mSelectRequest = false;
    mDecideRequest = false;
    mDecideFinished = false;
    mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].bind(false);
    mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].setFrame(0.0f);
    mLyt.calc();
    mAnmGroups[MAP_SAVE_OBJ_ANIM_ON_OFF].unbind();

    mAnmGroups[MAP_SAVE_OBJ_ANIM_DECIDE].setFrame(0.0f);
    mAnmGroups[MAP_SAVE_OBJ_ANIM_LOOP].bind(false);

    mStateMgr.changeState(StateID_Wait);
}

static const d2d::LytBrlanMapping sMapSaveObjBrlanMap[] = {
    { "mapSaveObj_00_saveObjOnOff.brlan",     "G_saveBtn_00"},
    {"mapSaveObj_00_saveObjDecide.brlan",     "G_saveBtn_00"},
    {  "mapSaveObj_00_saveObjLoop.brlan", "G_saveBtnLoop_00"},
};

void dLytMapSaveObj_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("mapSaveObj_00.brlyt", nullptr);
    for (int i = 0; i < MAP_SAVE_OBJ_NUM_ANIMS; i++) {
        mAnmGroups[i].init(sMapSaveObjBrlanMap[i].mFile, resAcc, mLyt.getLayout(), sMapSaveObjBrlanMap[i].mName);
    }

    mLyt.getLayout()->GetRootPane()->SetInfluencedAlpha(true);

    mAnmGroups[MAP_SAVE_CAPTION_ANIM_IN].bind(false);
    mAnmGroups[MAP_SAVE_CAPTION_ANIM_IN].setFrame(0.0f);
    mLyt.calc();

    mStateMgr.changeState(StateID_Wait);
}

void dLytMapSaveObj_c::remove() {
    for (int i = 0; i < MAP_SAVE_OBJ_NUM_ANIMS; i++) {
        mAnmGroups[i].remove();
    }
}

void dLytMapSaveObj_c::execute() {
    mStateMgr.executeState();
    if (mAnmGroups[MAP_SAVE_OBJ_ANIM_LOOP].isBound()) {
        mAnmGroups[MAP_SAVE_OBJ_ANIM_LOOP].play();
    }
}

void dLytMapSaveObj_c::draw() {
    nw4r::lyt::Pane *root = mLyt.getLayout()->GetRootPane();
    // @bug calc should be in execute...
    mLyt.calc();
    root->Draw(mLyt.getDrawInfo());
}

#pragma dont_inline on
dLytMapMain_c::dLytMapMain_c()
    : mStateMgr(*this),
      mFlowMgr(&mFlow),
      mFloorBtnMgr(&mGlobal),
      mFootPrints(&mGlobal),
      mAreaGroup(0),
      mMapUpDirectionAfterZoomToDetail(true),
      field_0x8C93(0),
      mMapEvent(MAP_EVENT_NONE),
      mNextMapEvent(MAP_EVENT_NONE),
      field_0x8CAC(0),
      field_0x8D38(0.0f, 0.0f),
      field_0x8D40(1.0f),
      field_0x8D44(0),
      field_0x8D46(0),
      field_0x8D48(0),
      field_0x8D4C(0.0f),
      field_0x8D50(0.0f),
      mCurrentFloor(0),
      mNumFloors(1),
      mBaseFloorOffset(0),
      field_0x8D64(0),
      field_0x8D68(0),
      mEventCanceled(false),
      mEventDone(false),
      mpRegionPane1(nullptr),
      mpRegionPane2(nullptr),
      field_0x8DB0(0),
      mNavEnabled(false),
      mDrawScaleFrame(false),
      mDisplayedBeaconCount(0),
      mShowIslandNames(false),
      field_0x8DBD(0),
      mMapChangeAlpha(0),
      mIsVisible(false),
      field_0x8DC0(-1),
      field_0x8DC4(0) {
    field_0x8D78 = 0.0f;
    field_0x8D7C = 0.0f;
    field_0x8D80 = 0.0f;
    field_0x8D84 = 0.0f;
    field_0x8D88 = 0.0f;
}
#pragma dont_inline reset

dLytMapMain_c::~dLytMapMain_c() {
    for (int i = 0; i < (int)ARRAY_LENGTH(mHitChecks); i++) {
        if (dCsMgr_c::GetInstance()->isRegist(&mHitChecks[i])) {
            dCsMgr_c::GetInstance()->unregistCursorTarget(&mHitChecks[i]);
        }
    }
}

dLytMapGlobal_c *dLytMapMain_c::getGlobal() {
    return &mGlobal;
}

dMapSavedData dLytMapMain_c::sSavedMapData = {
    {
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     },
    false
};

const dMapSavedData dLytMapMain_c::sDefaultMapData = {
    {
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     {dLytMapGlobal_c::MAPMODE_STAGE, 1, 1, 0},
     },
    false
};

static const char *const sTextboxLabelMapping[][2] = {
    {    "T_sNameS_00", "MAP_00"},
    {     "T_sName_00", "MAP_00"},
    {    "T_pNameS_00", "MAP_01"},
    {     "T_pName_00", "MAP_01"},
    {    "T_pNameS_01", "MAP_01"},
    {    "T_fNameS_00", "MAP_02"},
    {     "T_fName_00", "MAP_02"},
    {    "T_fNameS_01", "MAP_02"},
    {   "T_nFnameS_00", "MAP_03"},
    {    "T_nFname_00", "MAP_03"},
    {   "T_nFnameS_01", "MAP_03"},
    {    "T_lNameS_00", "MAP_04"},
    {     "T_lName_00", "MAP_04"},
    {    "T_lNameS_01", "MAP_04"},
    {   "T_sVnameS_00", "MAP_05"},
    {    "T_sVname_00", "MAP_05"},
    {   "T_sVnameS_01", "MAP_05"},
    {   "T_nVnameS_00", "MAP_06"},
    {    "T_nVname_00", "MAP_06"},
    {   "T_nVnameS_01", "MAP_06"},
    {    "T_dNameS_01", "MAP_08"},
    {     "T_dName_01", "MAP_08"},
    {    "T_dNameS_00", "MAP_08"},
    {    "T_dNameS_02", "MAP_07"},
    {     "T_dName_02", "MAP_07"},
    {    "T_dNameS_03", "MAP_07"},
    {    "T_gNameS_01", "MAP_10"},
    {     "T_gName_01", "MAP_10"},
    {    "T_gNameS_04", "MAP_10"},
    {    "T_gNameS_00", "MAP_09"},
    {     "T_gName_00", "MAP_09"},
    {    "T_gNameS_03", "MAP_09"},
    {    "T_gNameS_02", "MAP_40"},
    {     "T_gName_02", "MAP_40"},
    {    "T_gNameS_05", "MAP_40"},
    {    "T_lastDs_00", "MAP_17"},
    {     "T_lastD_00", "MAP_17"},
    { "T_pampkinNS_00", "MAP_27"},
    {  "T_pampkinN_00", "MAP_27"},
    {    "T_teryNS_00", "MAP_28"},
    {     "T_teryN_00", "MAP_28"},
    { "T_tikurinNS_00", "MAP_29"},
    {  "T_tikurinN_00", "MAP_29"},
    {    "T_musiNS_00", "MAP_31"},
    {     "T_musiN_00", "MAP_31"},
    {     "T_utaNS_00", "MAP_30"},
    {      "T_utaN_00", "MAP_30"},
    { "T_ruretouNS_00", "MAP_26"},
    {  "T_ruretouN_00", "MAP_26"},
    { "T_toForestS_00", "MAP_58"},
    {  "T_toForest_00", "MAP_58"},
    {"T_toVolcanoS_00", "MAP_59"},
    { "T_toVolcano_00", "MAP_59"},
    { "T_toDesertS_00", "MAP_60"},
    {  "T_toDesert_00", "MAP_60"},
    {      "T_fD1s_00", "MAP_11"},
    {       "T_fD1_00", "MAP_11"},
    {      "T_fD2s_00", "MAP_14"},
    {       "T_fD2_00", "MAP_14"},
    {      "T_vD2s_00", "MAP_16"},
    {       "T_vD2_00", "MAP_16"},
    {      "T_vD1s_00", "MAP_12"},
    {       "T_vD1_00", "MAP_12"},
    {      "T_dD1s_00", "MAP_13"},
    {       "T_dD1_00", "MAP_13"},
    {      "T_dD2s_00", "MAP_15"},
    {       "T_dD2_00", "MAP_15"},
    {    "T_lastDs_00", "MAP_17"},
    {     "T_lastD_00", "MAP_17"},
};

static const char *sHyliasRealmMapLabel = "MAP_64";

void dLytMapMain_c::loadTextboxes() {
    bool hyliasRealm = dScGame_c::isCurrentStage("F403") || dScGame_c::isCurrentStage("F404");
    for (int i = 0; i < (int)ARRAY_LENGTH(sTextboxLabelMapping); i++) {
        dTextBox_c *box = mLyt.getTextBox(sTextboxLabelMapping[i][0]);
        if (hyliasRealm && (i == 2 || i == 3 || i == 4)) {
            // Replace "Sealed Grounds" with "Hylia's Realm"
            box->setMessageWithGlobalTextProcessor2(sHyliasRealmMapLabel, nullptr);
        } else {
            box->setMessageWithGlobalTextProcessor2(sTextboxLabelMapping[i][1], nullptr);
        }
    }
}
static const d2d::LytBrlanMapping sMapMainBrlanMap[] = {
    {               "map_00_in.brlan",       "G_inOut_00"},
    {       "map_00_titleOnOff.brlan",  "G_titleOnOff_00"},
    {           "map_00_nOnOff.brlan",      "G_nOnOff_00"},
    {             "map_00_mapV.brlan",        "G_mapV_00"},
    {       "map_00_worldState.brlan",     "G_worldState"},
    {    "map_00_player2patten.brlan",    "G_2Pattern_00"},
    {           "map_00_rotate.brlan",      "G_rotate_00"},
    {              "map_00_out.brlan",       "G_inOut_00"},
    {          "map_00_mapDraw.brlan",      "G_plainC_00"},
    {          "map_00_mapDraw.brlan",     "G_forestC_00"},
    {          "map_00_mapDraw.brlan",    "G_nForestC_00"},
    {          "map_00_mapDraw.brlan",   "G_sVolcanoC_00"},
    {          "map_00_mapDraw.brlan",       "G_mineC_00"},
    {          "map_00_mapDraw.brlan",     "G_desertC_00"},
    {          "map_00_mapDraw.brlan",       "G_lakeC_00"},
    {          "map_00_mapDraw.brlan",       "G_glenC_00"},
    {          "map_00_mapDraw.brlan",        "G_seaC_00"},
    {          "map_00_mapDraw.brlan",   "G_nVolcanoC_00"},
    {          "map_00_mapDraw.brlan",       "G_glenC_01"},
    {    "map_00_skyLoftUpDown.brlan",   "G_skyloftUD_00"},
    {"map_00_LinkPositionLight.brlan",  "G_linkPointL_00"},
    {     "map_00_mapZinForest.brlan",   "G_ZoomInOut_00"},
    {       "map_00_zoomWakuIn.brlan",    "G_zoomWaku_00"},
    {      "map_00_zoomWakuOut.brlan",    "G_zoomWaku_00"},
    {    "map_00_mapZoutForest.brlan",   "G_ZoomInOut_00"},
    {      "map_00_mapZinPlant.brlan",   "G_ZoomInOut_00"},
    {    "map_00_mapZaOutPlant.brlan",   "G_ZoomInOut_00"},
    {     "map_00_mapZaInPlant.brlan",   "G_ZoomInOut_00"},
    {     "map_00_mapZoutPlant.brlan",   "G_ZoomInOut_00"},
    {     "map_00_mapZinDesert.brlan",   "G_ZoomInOut_00"},
    {    "map_00_mapZoutDesert.brlan",   "G_ZoomInOut_00"},
    {    "map_00_mapZinVolcano.brlan",   "G_ZoomInOut_00"},
    {   "map_00_mapZoutVolcano.brlan",   "G_ZoomInOut_00"},
    {   "map_00_mapZaOutForest.brlan",   "G_ZoomInOut_00"},
    {   "map_00_mapZaOutDesert.brlan",   "G_ZoomInOut_00"},
    {  "map_00_mapZaOutVolcano.brlan",   "G_ZoomInOut_00"},
    {    "map_00_mapZaInForest.brlan",   "G_ZoomInOut_00"},
    {    "map_00_mapZaInDesert.brlan",   "G_ZoomInOut_00"},
    {   "map_00_mapZaInVolcano.brlan",   "G_ZoomInOut_00"},
    {    "map_00_mapZaInSkyMap.brlan",   "G_ZoomInOut_00"},
    {   "map_00_mapZaOutSkyMap.brlan",   "G_ZoomInOut_00"},
    {   "map_00_mapZaInSkyMap2.brlan",   "G_ZoomInOut_00"},
    {  "map_00_mapZaOutSkyMap2.brlan",   "G_ZoomInOut_00"},
    {  "map_00_mapZaInSkyCorse.brlan",   "G_ZoomInOut_00"},
    { "map_00_mapZaOutSkyCorse.brlan",   "G_ZoomInOut_00"},
    {          "map_00_sunRote.brlan",         "G_sun_00"},
    {          "map_00_inNoCam.brlan",       "G_inOut_00"},
    {         "map_00_outNoCam.brlan",       "G_inOut_00"},
    {        "map_00_cloudLoop.brlan",   "G_cloudLoop_00"},
    {        "map_00_lightLoop.brlan",   "G_lightLoop_00"},
    {        "map_00_nushiLoop.brlan",   "G_nushiLoop_00"},
    {             "map_00_type.brlan",        "G_type_00"},
    {      "map_00_kumoPattern.brlan", "G_kumoPattern_00"},
    {        "map_00_titleLine.brlan",    "G_textLine_00"},
};

#define MAP_MAIN_ANIM_IN 0
#define MAP_MAIN_ANIM_TITLE_ON_OFF 1
#define MAP_MAIN_ANIM_N_ON_OFF 2
#define MAP_MAIN_ANIM_MAP_V 3
#define MAP_MAIN_ANIM_WORLD_STATE 4
#define MAP_MAIN_ANIM_PLAYER_2_PATTERN 5
#define MAP_MAIN_ANIM_ROTATE 6
#define MAP_MAIN_ANIM_OUT 7
#define MAP_MAIN_ANIM_DRAW_PLAIN 8
#define MAP_MAIN_ANIM_DRAW_FOREST 9
#define MAP_MAIN_ANIM_DRAW_N_FOREST 10
#define MAP_MAIN_ANIM_DRAW_S_VOLCANO 11
#define MAP_MAIN_ANIM_DRAW_MINE 12
#define MAP_MAIN_ANIM_DRAW_DESERT 13
#define MAP_MAIN_ANIM_DRAW_LAKE 14
#define MAP_MAIN_ANIM_DRAW_GLEN_00 15
#define MAP_MAIN_ANIM_DRAW_SEA 16
#define MAP_MAIN_ANIM_DRAW_N_VOLCANO 17
#define MAP_MAIN_ANIM_DRAW_GLEN_01 18
#define MAP_MAIN_ANIM_SKYLOFT_UP_DOWN 19
#define MAP_MAIN_ANIM_LINK_POSITION_LIGHT 20

#define MAP_MAIN_ANIM_MAP_Z_IN_FOREST 21
#define MAP_MAIN_ANIM_MAP_Z_WAKU_IN 22

#define MAP_MAIN_ANIM_MAP_Z_IN_PLANT 25

#define MAP_MAIN_ANIM_MAP_ZA_IN_PLANT 27

#define MAP_MAIN_ANIM_MAP_Z_IN_DESERT 29

#define MAP_MAIN_ANIM_MAP_Z_IN_VOLCANO 31

#define MAP_MAIN_ANIM_MAP_ZA_IN_FOREST 36
#define MAP_MAIN_ANIM_MAP_ZA_IN_DESERT 37
#define MAP_MAIN_ANIM_MAP_ZA_IN_VOLCANO 38

#define MAP_MAIN_ANIM_MAP_ZA_IN_SKY_MAP_2 41
#define MAP_MAIN_ANIM_MAP_ZA_OUT_SKY_COURSE 44

#define MAP_MAIN_ANIM_SUN_ROTE 45
#define MAP_MAIN_ANIM_IN_NO_CAM 46
#define MAP_MAIN_ANIM_OUT_NO_CAM 47
#define MAP_MAIN_ANIM_CLOUD_LOOP 48
#define MAP_MAIN_ANIM_LIGHT_LOOP 49
#define MAP_MAIN_ANIM_NUSHI_LOOP 50
#define MAP_MAIN_ANIM_TYPE 51
#define MAP_MAIN_ANIM_KUMO_PATTERN 52
#define MAP_MAIN_ANIM_TITE_LINE 53

#define MAP_MAIN_NUM_ANIMS 54

static const char *sGroupName = "G_ref_00";

static const char *sBoundingNames[] = {
    "B_noroshi_00",  "B_houkoul_00",  "B_remoCon_00",  "B_nunchaku_00", "B_tittle_00",   "B_saveIcon_00",
    "B_saveIcon_01", "B_saveIcon_02", "B_saveIcon_03", "B_saveIcon_04", "B_saveIcon_05", "B_saveIcon_06",
    "B_saveIcon_07", "B_saveIcon_08", "B_saveIcon_28", "B_saveIcon_10", "B_saveIcon_11", "B_saveIcon_12",
    "B_saveIcon_13", "B_saveIcon_14", "B_saveIcon_15", "B_saveIcon_16", "B_saveIcon_17", "B_saveIcon_18",
    "B_saveIcon_19", "B_saveIcon_20", "B_saveIcon_21", "B_saveIcon_22", "B_saveIcon_23", "B_saveIcon_24",
    "B_saveIcon_25", "B_saveIcon_26", "B_saveIcon_27",
};

#define MAP_MAIN_NUM_BOUNDINGS_NORMAL 5
#define MAP_MAIN_OFFSET_BOUNDINGS_SAVEICON 5
#define MAP_MAIN_NUM_BOUNDINGS_SAVEICON 28

#define MAP_MAIN_NUM_BOUNDINGS 33

static const char *sPaneNames[] = {
    "N_pName_00", "N_fName_00", "N_nFname_00", "N_sVname_00", "N_dName_02", "N_dName_00",
    "N_lName_00", "N_gName_02", "N_gName_01",  "N_nVname_00", "N_gName_00",
};

static const char *sDungeonPaneNames[] = {
    "N_fD1_00", "N_vD1_00", "N_dD1_00", "N_fD2_00", "N_dD2_00", "N_vD2_00", "N_lastD_00",
};

static const char *sDungeonPicNames[] = {
    "P_fD1_00", "P_vD1_00", "P_dD1_00", "P_fD2_00", "P_dD2_00", "P_vD2_00", "P_lastD_00",
};

static const char *sPriorityGroupNames[] = {
    "G_priority_00",
    "G_priority_02",
};

void dLytMapMain_c::build() {
    mIsVisible = 0;
    field_0x8DB0 = 0;
    if (dScGame_c::isCurrentStage("F102")) {
        // awesome but unnecessary
        field_0x8DB0 = 0;
    }

    dStage_c::GetInstance()->fn_801B3F20();
    d2d::ResAccIf_c *resAcc = dLytMap_c::getResAcc();
    mLyt.setResAcc(resAcc);
    mLyt.build("map_00.brlyt", nullptr);
    getGlobal()->setField_0x28(mLyt.getDrawInfo().GetLocationAdjustScale());

    for (int i = 0; i < MAP_MAIN_NUM_ANIMS; i++) {
        mAnmGroups[i].init(sMapMainBrlanMap[i].mFile, resAcc, mLyt.getLayout(), sMapMainBrlanMap[i].mName);
    }

    mFloorBtnMgr.build(resAcc);
    mSubpane.mpLytPane = &mFloorBtnMgr;
    mSubpaneList.PushBack(&mSubpane);

    d2d::dSubPane::linkMeters(mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName), &mSubpaneList);
    mFootPrints.build(resAcc);
    mMarkers.build(resAcc);

    mPinIconAggregate.build(resAcc);
    mPutIcon.build(resAcc);

    for (int i = 0; i < (int)ARRAY_LENGTH(mSaveObjs); i++) {
        mSaveObjs[i].build(resAcc);
    }

    mSaveCaption.build(resAcc);
    mSavePopup.build(resAcc);
    mPopupInfo.build(resAcc);

    mAnmGroups[MAP_MAIN_ANIM_IN].bind(false);
    mAnmGroups[MAP_MAIN_ANIM_IN].setToEnd();
    mLyt.calc();

    for (int i = 0; i < (int)ARRAY_LENGTH(sBoundingNames); i++) {
        nw4r::lyt::Bounding *b = mLyt.findBounding(sBoundingNames[i]);
        b->SetVisible(false);
        // TODO - bird statues
        if (i >= 5 && i < 15) {
            mpSaveObjBoundingsFaron[i - 5] = b;
        } else if (i >= 15 && i < 21) {
            mpSaveObjBoundingsEldin[i - 15] = b;
        } else if (i >= 21 && i < 33) {
            mpSaveObjBoundingsLanayru[i - 21] = b;
        }

        mHitChecks[i].init(b, 0x02, 2, 0);
        dCsMgr_c::GetInstance()->registCursorTarget(&mHitChecks[i]);
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(sBoundingNames); i++) {
        mHitChecks[i].resetCachedHitboxes();
        mHitChecks[i].execute();
    }

    mpMapBounding = mLyt.findBounding("B_map_00");
    nw4r::lyt::Size t(1.0f, 1.0f);
    mpMapBounding->SetSize(t);
    mVec3_c boundingPos = mpMapBounding->GetTranslate();
    mVec2_c boundingPos2 = mVec2_c(boundingPos.x, boundingPos.y);
    dLytMapGlobal_c *global = getGlobal();
    global->setField_0x20(boundingPos2);

    mMapCapture.setPicture(mLyt.findPicture("P_map_00"));
    nw4r::lyt::Pane *limitBounding = mLyt.findPane("B_limit_00");
    const nw4r::lyt::Size &sz = limitBounding->GetSize();
    field_0x8D30.x = sz.width - 1.0f;
    field_0x8D30.y = sz.height - 1.0f;
    field_0x8D38.x = sz.width;
    field_0x8D38.y = sz.height;
    field_0x8D38.x *= mLyt.getDrawInfo().GetLocationAdjustScale().x;
    mpAllPane = mLyt.findPane("N_all_00");
    mpNoroshiPane = mLyt.findPane("N_noroshi_00");
    mpScaleFramePane = mLyt.findPane("N_scaleFlame_00");
    mpWakuWindow = mLyt.getWindow("W_waku_01");

    for (int i = 0; i < (int)ARRAY_LENGTH(sPaneNames); i++) {
        if (sPaneNames[i] != nullptr) {
            mpPanes[i] = mLyt.findPane(sPaneNames[i]);
        } else {
            mpPanes[i] = nullptr;
        }
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(sDungeonPaneNames); i++) {
        if (sDungeonPaneNames[i] != nullptr) {
            mpDungeonPanes[i] = mLyt.findPane(sDungeonPaneNames[i]);
        } else {
            mpDungeonPanes[i] = nullptr;
        }

        if (sDungeonPicNames[i] != nullptr) {
            mpDungeonPics[i] = mLyt.findPane(sDungeonPicNames[i]);
        } else {
            mpDungeonPics[i] = nullptr;
        }
    }

    mpNumberTextBox = mLyt.getTextBox("T_number_00");
    mpNumberTextBoxS = mLyt.getTextBox("T_numberS_00");
    mpZoomInOutPane = mLyt.findPane("N_zoomInOut_00");

    for (int i = 0; i < (int)ARRAY_LENGTH(sPriorityGroupNames); i++) {
        mpPriorityGroups[i] = mLyt.findGroupByName(sPriorityGroupNames[i]);
    }

    mMarkers.setField_0x6F4(boundingPos2);

    mPinIconAggregate.setUnk(&field_0xF1C);

    mpPaneBgAll01 = mLyt.findPane("N_mapBgAll_01");
    mpPaneBgAll02 = mLyt.findPane("N_mapBgAll_02");
    mpPaneAll01 = mLyt.findPane("N_mapAll_01");
    mpPaneAll02 = mLyt.findPane("N_mapAll_02");
    field_0x8928.x = mpPaneBgAll01->GetGlobalMtx()._03;
    field_0x8928.y = mpPaneBgAll01->GetGlobalMtx()._13;
    mpPaneRotate00 = mLyt.findPane("N_rotateP_00");
    mpPaneRotate01 = mLyt.findPane("N_rotateP_01");

    nw4r::lyt::Pane *bgAll = mLyt.findPane("N_mapBgAll_00");

    mVec2_c v1(bgAll->GetGlobalMtx()._03, bgAll->GetGlobalMtx()._13);
    mVec2_c tmp = v1 - global->getField_0x20();
    global->setField_0x0C(tmp);

    field_0x8C88 = mAnmGroups[MAP_MAIN_ANIM_IN].getFrameCtrl()->mEndFrame - 1.0f;
    mAnmGroups[MAP_MAIN_ANIM_IN].unbind();

    mAnmGroups[MAP_MAIN_ANIM_OUT].bind(false);
    field_0x8C8C = mAnmGroups[MAP_MAIN_ANIM_OUT].getLastFrame();
    mAnmGroups[MAP_MAIN_ANIM_OUT].unbind();
    field_0x8CAC = 0;
    mDisplayedBeaconCount = getMaxBeaconCount();
    mStateMgr.changeState(StateID_Invisible);
    getGlobal()->setFloor(0);
    mNavEnabled = false;
    mCurrentFloor = 0;
    mNumFloors = 1;
    mBaseFloorOffset = 0;
    field_0x8D64 = 0;
    fn_80143300();
}

void dLytMapMain_c::loadStageProperties() {
    mAreaGroup = getAreaGroup(dStageMgr_c::GetInstance()->getSTIFArea());
    mRoomType = getRoomType();

    // Yes, it does need to be written this way, otherwise the catch-all else
    // branch is collapsed with the other ones.

    if (mRoomType == ROOMTYPE_FIELD) {
        mCurrentMapMode = dLytMapGlobal_c::MAPMODE_ZOOM;
        return;
    }

    if (mRoomType == ROOMTYPE_DUNGEON) {
        mCurrentMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
        return;
    }

    if (mRoomType == ROOMTYPE_SKYLOFT) {
        mCurrentMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
        return;
    }

    if (mRoomType == ROOMTYPE_BOSS_HOUSE) {
        mCurrentMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
        return;
    }

    mCurrentMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
}

void dLytMapMain_c::setupStage() {
    loadStageProperties();
    if (mMapEvent == MAP_EVENT_SAVE_OBJ) {
        mLyt.findPane("N_skyloft_00")->SetVisible(false);
    }

    mAnmGroups[MAP_MAIN_ANIM_PLAYER_2_PATTERN].bind(false);
    if (dAcPy_c::getCurrentTunicType() == 1) {
        mAnmGroups[MAP_MAIN_ANIM_PLAYER_2_PATTERN].setFrame(1.0f);
    } else {
        mAnmGroups[MAP_MAIN_ANIM_PLAYER_2_PATTERN].setFrame(0.0f);
    }

    if (mRoomType == ROOMTYPE_SKYLOFT ||
        (mAreaGroup == AREAGROUP_SKY && (mRoomType == ROOMTYPE_BOSS_HOUSE || mRoomType == ROOMTYPE_SKYFIELD_INTERIOR ||
                                         mRoomType == ROOMTYPE_DUNGEON))) {
        mAnmGroups[MAP_MAIN_ANIM_SUN_ROTE].bind(false);
    }

    s32 type = 0;
    mAnmGroups[MAP_MAIN_ANIM_TYPE].bind(false);
    if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL) {
        type = 8;
    } else if (mAreaGroup == AREAGROUP_FARON || mAreaGroup == AREAGROUP_SEALED_GROUNDS) {
        if (dStageMgr_c::GetInstance()->getSTIFArea() == dStageMgr_c::STIF_AREA_SEALED_GROUNDS) {
            type = 7;
        } else if (mRoomType == ROOMTYPE_FIELD) {
            type = 1;
        } else {
            type = 2;
        }
    } else if (mAreaGroup == AREAGROUP_ELDIN) {
        if (mRoomType == ROOMTYPE_FIELD) {
            type = 3;
        } else {
            type = 4;
        }
    } else if (mAreaGroup == AREAGROUP_LANAYRU) {
        if (mRoomType == ROOMTYPE_FIELD) {
            type = 5;
        } else {
            type = 6;
        }
    } else if (mAreaGroup == AREAGROUP_SKY) {
        if (dStageMgr_c::GetInstance()->getSTIFRoomType() == dStageMgr_c::STIF_ROOMTYPE_DUNGEON) {
            type = 11;
        } else {
            // TODO - probably an enum but this matches too
            type = 9;
            if (dScGame_c::currentSpawnInfo.getTimeOfDay() == SpawnInfo::NIGHT) {
                type = 10;
            }
        }
    }
    mAnmGroups[MAP_MAIN_ANIM_TYPE].setFrame(type);
    mLyt.calc();
    mAnmGroups[MAP_MAIN_ANIM_TYPE].unbind();

    field_0x8C7C = 0.0f;
    field_0x8C80 = 0.0f;
    mAnmGroups[MAP_MAIN_ANIM_IN].bind(false);
    mAnmGroups[MAP_MAIN_ANIM_IN].setFrame(0.0f);
    mAnmGroups[MAP_MAIN_ANIM_MAP_V].bind(false);
    fn_801431E0();
    mLyt.calc();
    mAnmGroups[MAP_MAIN_ANIM_MAP_V].unbind();
    mAnmGroups[MAP_MAIN_ANIM_IN].unbind();
    if (mRoomType == ROOMTYPE_BOSS_HOUSE || mRoomType == ROOMTYPE_SKYFIELD_INTERIOR) {
        mLyt.findPane("N_houkoul_00")->SetVisible(false);
    }
    if (mMapEvent != MAP_EVENT_SAVE_OBJ) {
        loadTextboxes();
    }
    fn_8013AD50();
}

void dLytMapMain_c::remove() {
    for (int i = 0; i < (int)ARRAY_LENGTH(mHitChecks); i++) {
        dCsMgr_c::GetInstance()->unregistCursorTarget(&mHitChecks[i]);
    }

    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }
    mPopupInfo.remove();
    mSavePopup.remove();
    mSaveCaption.remove();
    for (int i = 0; i < (int)ARRAY_LENGTH(mSaveObjs); i++) {
        mSaveObjs[i].remove();
    }
    mFloorBtnMgr.remove();
    mFootPrints.remove();
    mMarkers.remove();
    mPinIconAggregate.remove();
    mPutIcon.remove();
    for (int i = 0; i < MAP_MAIN_NUM_ANIMS; i++) {
        mAnmGroups[i].remove();
    }
    dStage_c::GetInstance()->fn_801B3F30();
}

void dLytMapMain_c::execute() {
    if ((*mStateMgr.getStateID() == StateID_Invisible)) {
        return;
    }

    if ((*mStateMgr.getStateID() == StateID_RenderingWait)) {
        mStateMgr.executeState();
        mMapCapture.execute();
        return;
    }

    field_0xF1C.field_0x05 = 0;
    dLytMapGlobal_c *global = getGlobal();
    mDrawScaleFrame = false;
    mPointerOnMap = false;
    mPointerCanPlaceBeacon = false;
    checkCursorPointedAtMap();

    if (dPad::getDownTrigDown() && canCenterCursor(mCurrentMapMode) && canCenterCursor(mNextMapMode)) {
        dPad::ex_c::getInstance()->centerCursor(mPad::getCurrentCoreID(), true);
    }

    if (mRoomType == ROOMTYPE_DUNGEON && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mFloorBtnMgr.setForceNoNav(true);
    } else {
        mFloorBtnMgr.setForceNoNav(false);
    }

    mFloorBtnMgr.checkPointedAtBtn();
    mStateMgr.executeState();
    if (*mStateMgr.getStateID() != StateID_ResetPosWithFloorChange) {
        mFloorBtnMgr.execute();
    }

    s32 beaconId = -1;
    field_0x8DBD = false;
    if (canCenterCursor1(mCurrentMapMode) && canCenterCursor1(mNextMapMode) && canPlaceBeacons(mCurrentMapMode) &&
        canPlaceBeacons(mNextMapMode)) {
        bool exhaustedPins = mPinIconAggregate.getNumSetPins() >= getMaxBeaconCount();
        if (!exhaustedPins && mPointerCanPlaceBeacon &&
            *mPinIconAggregate.mStateMgr.getStateID() != dLytMapPinIconAggregate_c::StateID_Select &&
            isPointingAtMainMap()) {
            mPinIconAggregate.findNewBeaconId(&beaconId);
            if (beaconId >= 0) {
                field_0x8DBD = true;
            }
        }
    }

    if (field_0x8DBD) {
        mPutIcon.setPosition(dCsGame_c::GetInstance()->getCursorIf()->getCursorPos());
        mPutIcon.setScale(1.0f);
        mPutIcon.setVisible(true);
    } else {
        mPutIcon.setVisible(false);
    }

    static SizedWString<0x20> buf;
    buf.sprintf(L"%ld", (mDisplayedBeaconCount - mPinIconAggregate.getNumSetPins()) % 10);
    mpNumberTextBox->setTextWithGlobalTextProcessor(buf, nullptr);
    mpNumberTextBoxS->setTextWithGlobalTextProcessor(buf, nullptr);

    if (canChangeFloor(mCurrentMapMode) && canChangeFloor(mNextMapMode)) {
        const sStateIDIf_c *state = mStateMgr.getStateID();
        if (*state != StateID_FloorChange && *state != StateID_RenderingWait &&
            *state != StateID_ResetPosWithFloorChange) {
            mMapCapture.renderRequest();
        }
    }

    mMapCapture.execute();
    fn_80143360();
    global->setCurrentMapMode(mCurrentMapMode);
    global->setNextMapMode(mNextMapMode);
    mFootPrints.setVisible(shouldDrawFootprints());

    f32 f1 = 1.0f;
    if (mRoomType != ROOMTYPE_BOSS_HOUSE && mRoomType != ROOMTYPE_SKYFIELD_INTERIOR) {
        // why does everyone clamp things differently ???
        f32 tmp = global->getField_0x58();
        if (tmp < 0.0f) {
            tmp = 0.0f;
        } else if (tmp > 1.0f) {
            tmp = 1.0f;
        }
        f1 = tmp;
    }
    f32 f2 = fn_80142D90(dLytMapGlobal_c::MAPMODE_ZOOM);
    if (mRoomType == ROOMTYPE_BOSS_HOUSE || mRoomType == ROOMTYPE_SKYFIELD_INTERIOR) {
        f2 = fn_80142D90(dLytMapGlobal_c::MAPMODE_STAGE);
    }
    mFootPrints.setField_0x198(f2);
    mFootPrints.setField_0x19C(f1);
    mFootPrints.execute();
    setCursorType();

    if (canCenterCursor1(mCurrentMapMode) && canCenterCursor1(mNextMapMode) &&
        (mNumFloors < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
        global->setField_0x4D(1);
    } else {
        global->setField_0x4D(0);
    }

    if (mMapEvent == MAP_EVENT_DUNGEON_MAP_GET || mMapEvent == MAP_EVENT_FIELD_MAP_CHANGE_5 ||
        mMapEvent == MAP_EVENT_FOREST_MAP_CHANGE || mMapEvent == MAP_EVENT_SW_BANK_SMALL) {
        clearButtonMessages();
    } else {
        setButtonMessages(mCurrentMapMode, mMapUpDirection, mNextMapMode, mNextMapUpDirection);
    }

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY && mAnmGroups[MAP_MAIN_ANIM_SUN_ROTE].isBound()) {
        mAnmGroups[MAP_MAIN_ANIM_SUN_ROTE].play();
    }

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY ||
        mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
        if (mAnmGroups[MAP_MAIN_ANIM_LIGHT_LOOP].isBound()) {
            mAnmGroups[MAP_MAIN_ANIM_LIGHT_LOOP].play();
        }
        mAnmGroups[MAP_MAIN_ANIM_SKYLOFT_UP_DOWN].play();
    }

    switch (mCurrentMapMode) {
        case dLytMapGlobal_c::MAPMODE_PROVINCE: {
            if (mpRegionPane1 != nullptr) {
                mpRegionPane1->SetVisible(true);
            }
            if (mpRegionPane2 != nullptr) {
                mpRegionPane2->SetVisible(true);
            }

            if (mAnmGroups[MAP_MAIN_ANIM_LINK_POSITION_LIGHT].isBound()) {
                mAnmGroups[MAP_MAIN_ANIM_LINK_POSITION_LIGHT].play();
            }
            break;
        }
        case dLytMapGlobal_c::MAPMODE_WORLD: {
            if (mpRegionPane1 != nullptr) {
                mpRegionPane1->SetVisible(false);
            }
            if (mpRegionPane2 != nullptr) {
                mpRegionPane2->SetVisible(false);
            }
            break;
        }
    }

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) {
        if (mAnmGroups[MAP_MAIN_ANIM_CLOUD_LOOP].isBound()) {
            mAnmGroups[MAP_MAIN_ANIM_CLOUD_LOOP].play();
        }
        if (mAnmGroups[MAP_MAIN_ANIM_NUSHI_LOOP].isBound()) {
            mAnmGroups[MAP_MAIN_ANIM_NUSHI_LOOP].play();
        }
    }

    if (mMapEvent == MAP_EVENT_SAVE_OBJ) {
        bool visible =
            mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE || mNextMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE;
        setSaveObjsVisible(visible);
        mPopupInfo.executeState_Invisible();
    }
    mSaveCaption.execute();
    mPutIcon.execute();

    mLyt.calc();

    u8 alpha = 0;
    if (mpAllPane->IsVisible()) {
        alpha = mpAllPane->GetAlpha();
    }
    global->setAlpha(alpha);
    fn_80143300();
    mPinIconAggregate.setScale(global->getZoomFrame());
    mMarkers.execute();

    if (mNextMapEvent == MAP_EVENT_FIELD_MAP_CHANGE_5 || mNextMapEvent == MAP_EVENT_FIELD_MAP_CHANGE_8) {
        mStateMgr.changeState(StateID_EventFieldMapChange_Step1);
        mMapEvent = mNextMapEvent;
        mNextMapEvent = 0;
    }
    mMaxBeaconCount = 0;
}

void dLytMapMain_c::draw() {
    if (!mIsVisible) {
        return;
    }

    dLytMapGlobal_c *global = getGlobal();
    fn_80143120(1);
    mLyt.draw();
    if (shouldDrawFootprints() && mCurrentFloor == global->getFloor()) {
        mFootPrints.draw();
    }

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM ||
        mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE || mNextMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mMarkers.draw();
    }

    if (mRoomType != ROOMTYPE_BOSS_HOUSE && mRoomType != ROOMTYPE_SKYFIELD_INTERIOR &&
        mMapEvent != MAP_EVENT_SW_BANK_SMALL) {
        mPinIconAggregate.draw();
    }

    fn_80143120(0);

    if ((mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) &&
        (mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE || mNextMapMode == dLytMapGlobal_c::MAPMODE_ZOOM)) {
        mpZoomInOutPane->SetVisible(false);
    } else {
        mpZoomInOutPane->SetVisible(true);
    }

    mpScaleFramePane->SetVisible(mDrawScaleFrame ? true : false);
    mLyt.getLayout()->GetRootPane()->Draw(mLyt.getDrawInfo());
    if ((mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE || mNextMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) &&
        mMapEvent == MAP_EVENT_SAVE_OBJ) {
        initSaveObjs();
    }
    fn_80143120(-2);
    mMarkers.drawPopups();
    mPutIcon.draw();
    if (mMapEvent == MAP_EVENT_SAVE_OBJ) {
        mPopupInfo.draw();
    }

    if (*mStateMgr.getStateID() == StateID_EventSaveObjSelect) {
        mSavePopup.draw();
    }
    mSaveCaption.draw();
}

bool dLytMapMain_c::checkClose() const {
    bool ret = false;
    const sStateIDIf_c &stateID = *mStateMgr.getStateID();
    switch (mMapEvent) {
        case MAP_EVENT_NONE:
        case MAP_EVENT_11:   {
            if (stateID == StateID_Active && (dPad::getDownTrigPlus() || dPad::getDownTrigB())) {
                ret = true;
            }
            break;
        }
        case MAP_EVENT_1:
        case MAP_EVENT_MAP_INTRO:
        case MAP_EVENT_DUNGEON_MAP_GET:
        case MAP_EVENT_FOREST_MAP_CHANGE:
        case MAP_EVENT_SIGNAL_ADD:
        case MAP_EVENT_FIELD_MAP_CHANGE_8:
        case MAP_EVENT_GODDESS_CUBE:
        case MAP_EVENT_SAVE_OBJ:           {
            if (mEventDone) {
                ret = true;
            }
            break;
        }
        case MAP_EVENT_FIELD_MAP_CHANGE_5: {
            if (stateID == StateID_EventFieldMapChange_Step4 && mEventDone) {
                ret = true;
            }
            break;
        }
        case MAP_EVENT_SW_BANK_SMALL: {
            if (stateID == StateID_EventSwBankSmall_Step3 && mEventDone) {
                ret = true;
            }
            break;
        }
    }

    return ret;
}

bool dLytMapMain_c::canChangeUpDirection(s32 mapMode, bool upDirection) const {
    bool ret = false;
    if (mapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        switch (mRoomType) {
            case ROOMTYPE_FIELD:    ret = true; break;
            case ROOMTYPE_DUNGEON:  ret = true; break;
            case ROOMTYPE_SKYLOFT:  ret = true; break;
            case ROOMTYPE_SKYFIELD: ret = true; break;
        }
    }

    return ret;
}

bool dLytMapMain_c::canZoomIn(s32 mapMode) const {
    bool ret = false;
    switch (mMapEvent) {
        case MAP_EVENT_SIGNAL_ADD:
        case MAP_EVENT_FIELD_MAP_CHANGE_8:
        case MAP_EVENT_GODDESS_CUBE:
        case MAP_EVENT_NONE:
        case MAP_EVENT_1:
        case MAP_EVENT_11:                 {
            if (mRoomType == ROOMTYPE_SKYLOFT) {
                if (mapMode == dLytMapGlobal_c::MAPMODE_WORLD || mapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY ||
                    mapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
                    ret = true;
                }
            } else if (mAreaGroup == AREAGROUP_SKY && mRoomType == ROOMTYPE_BOSS_HOUSE) {
                if (mapMode == dLytMapGlobal_c::MAPMODE_WORLD || mapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) {
                    ret = true;
                }
            } else if (mAreaGroup == AREAGROUP_SKY && mRoomType == ROOMTYPE_DUNGEON) {
                if (mapMode == dLytMapGlobal_c::MAPMODE_WORLD || mapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY ||
                    mapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
                    ret = true;
                }
            } else if (mRoomType == ROOMTYPE_SKYFIELD) {
                if (mapMode == dLytMapGlobal_c::MAPMODE_WORLD || mapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
                    ret = true;
                }
            } else if (mRoomType == ROOMTYPE_FIELD) {
                if (mapMode == dLytMapGlobal_c::MAPMODE_STAGE || mapMode == dLytMapGlobal_c::MAPMODE_WORLD ||
                    mapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
                    ret = true;
                }
            } else if (mRoomType == ROOMTYPE_DUNGEON) {
                if (mapMode == dLytMapGlobal_c::MAPMODE_STAGE || mapMode == dLytMapGlobal_c::MAPMODE_WORLD ||
                    mapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
                    ret = true;
                }
            } else if (mRoomType == ROOMTYPE_BOSS_HOUSE) {
                if (mapMode == dLytMapGlobal_c::MAPMODE_WORLD || mapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
                    ret = true;
                }
            } else if (mRoomType == ROOMTYPE_SKYFIELD_INTERIOR) {
                if (mapMode == dLytMapGlobal_c::MAPMODE_WORLD) {
                    ret = true;
                }
            }
            break;
        }
    }

    return ret;
}

bool dLytMapMain_c::canZoomOut(s32 mapMode) const {
    if (!StoryflagManager::sInstance->getFlag(STORYFLAG_FARON_DISCOVERED) &&
        ((mRoomType == ROOMTYPE_SKYLOFT && mapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) ||
         mRoomType != ROOMTYPE_SKYLOFT && mapMode == dLytMapGlobal_c::MAPMODE_STAGE)) {
        return false;
    }

    bool ret = false;

    if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL) {
        if (mapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
            return true;
        }
    } else {
        switch (mMapEvent) {
            case MAP_EVENT_SIGNAL_ADD:
            case MAP_EVENT_FIELD_MAP_CHANGE_8:
            case MAP_EVENT_GODDESS_CUBE:
            case MAP_EVENT_NONE:
            case MAP_EVENT_1:
            case MAP_EVENT_11:                 {
                if (mRoomType == ROOMTYPE_SKYLOFT) {
                    if (mapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY || mapMode == dLytMapGlobal_c::MAPMODE_STAGE ||
                        mapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
                        ret = true;
                    }
                } else if (mAreaGroup == AREAGROUP_SKY && mRoomType == ROOMTYPE_BOSS_HOUSE) {
                    if (mapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY || mapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
                        ret = true;
                    }
                } else if (mAreaGroup == AREAGROUP_SKY && mRoomType == ROOMTYPE_DUNGEON) {
                    if (mapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY || mapMode == dLytMapGlobal_c::MAPMODE_STAGE ||
                        mapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
                        ret = true;
                    }
                } else if (mRoomType == ROOMTYPE_SKYFIELD) {
                    if (mapMode == dLytMapGlobal_c::MAPMODE_STAGE || mapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
                        ret = true;
                    }
                } else if (mRoomType == ROOMTYPE_FIELD) {
                    if (mapMode == dLytMapGlobal_c::MAPMODE_ZOOM || mapMode == dLytMapGlobal_c::MAPMODE_STAGE ||
                        mapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
                        ret = true;
                    }
                } else if (mRoomType == ROOMTYPE_DUNGEON) {
                    if (mapMode == dLytMapGlobal_c::MAPMODE_ZOOM || mapMode == dLytMapGlobal_c::MAPMODE_STAGE ||
                        mapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
                        ret = true;
                    }
                } else if (mRoomType == ROOMTYPE_BOSS_HOUSE) {
                    if (mapMode == dLytMapGlobal_c::MAPMODE_STAGE || mapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
                        ret = true;
                    }
                } else if (mRoomType == ROOMTYPE_SKYFIELD_INTERIOR) {
                    if (mapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
                        ret = true;
                    }
                }
                break;
            }
        }
    }

    return ret;
}

bool dLytMapMain_c::canCenterCursor(s32 mapMode) const {
    bool ret = false;
    const sStateIDIf_c &stateID = *mStateMgr.getStateID();

    if ((stateID != StateID_Invisible && stateID != StateID_RenderingWait && stateID != StateID_In &&
         stateID != StateID_Out && stateID != StateID_EventSaveObjConfirmMsgWindow &&
         stateID != StateID_EventSaveObjDecide && stateID != StateID_EventSaveObjMsgWindow)) {
        if ((mMapEvent == MAP_EVENT_SIGNAL_ADD || mMapEvent == MAP_EVENT_FIELD_MAP_CHANGE_8 ||
             mMapEvent == MAP_EVENT_GODDESS_CUBE || mMapEvent == MAP_EVENT_NONE || mMapEvent == MAP_EVENT_1 ||
             mMapEvent == MAP_EVENT_11) &&
            (mapMode == dLytMapGlobal_c::MAPMODE_STAGE || mapMode == dLytMapGlobal_c::MAPMODE_ZOOM) &&
            (!EventManager::isInEvent() ||
             // TODO - why is this being double checked?
             ((EventManager::isInEvent() && !dMessage_c::getInstance()->getInMapEvent()) ||
              (EventManager::isInEvent() && !dMessage_c::getInstance()->getInMapEvent()))) &&
            (!needsNav(mapMode) || !mFloorBtnMgr.hasPointedAtABtnIdx() || mFloorBtnMgr.isUsingPointerNav())) {
            ret = true;
        }
        if (mMapEvent == MAP_EVENT_SAVE_OBJ && mapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
            ret = true;
        }
    }
    return ret;
}

bool dLytMapMain_c::needsNav(s32 mapMode) const {
    bool ret = false;

    // TODO - Why is this checking mCurrentMapMode
    if (mNumFloors >= 2 &&
        (mMapEvent == MAP_EVENT_NONE || mMapEvent == MAP_EVENT_1 || mMapEvent == MAP_EVENT_SIGNAL_ADD) &&
        (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE ||
         (mapMode == dLytMapGlobal_c::MAPMODE_ZOOM && mFloorBtnMgr.hasPointedAtABtnIdx()))) {
        ret = true;
    }

    return ret;
}

s32 dLytMapMain_c::getSelectedSaveObjIdx() const {
    bool has = false;
    s32 start;
    s32 end;
    if (mAreaGroup == AREAGROUP_FARON) {
        start = 5;
        end = 14;
        has = true;
    } else if (mAreaGroup == AREAGROUP_ELDIN) {
        start = 15;
        end = 20;
        has = true;
    } else if (mAreaGroup == AREAGROUP_LANAYRU) {
        start = 21;
        end = 32;
        has = true;
    }

    if (!has) {
        // @bug this should probably return -1 but it doesn't matter
        return 0;
    }

    s32 idx = -1;
    dCursorHitCheck_c *d = dCsGame_c::GetInstance()->getCursorHit();
    if (d != nullptr && d->getType() == 'lyt ') {
        for (int i = start; i <= end; i++) {
            if (d == &mHitChecks[i]) {
                idx = i - start;
                break;
            }
        }
    }

    return idx;
}

bool dLytMapMain_c::isPaneVisible(nw4r::lyt::Pane *pane) const {
    bool ret = true;
    while (pane != nullptr) {
        if (!pane->IsVisible() || pane->GetAlpha() == 0) {
            ret = false;
            break;
        }
        pane = pane->GetParent();
    }
    return ret;
}

void dLytMapMain_c::setSaveObjsVisible(bool visible) {
    nw4r::lyt::Pane *p;
    nw4r::lyt::Bounding **ptrs = mpCurrentSaveObjBoundings;
    for (int i = 0; i < mNumSaveObjs; i++) {
        p = ptrs[i];
        if (visible) {
            // This makes like no sense - the isPaneVisible check ensures
            // that the pane is already visible when we enter the branch...
            if (p != nullptr && p != nullptr && isPaneVisible(p)) {
                p->SetVisible(true);
            }
        } else {
            p->SetVisible(false);
        }
    }
}

void dLytMapMain_c::setSaveObjPanePtrs() {
    nw4r::lyt::Bounding **p = nullptr;
    u32 count = 0;
    if (mMapEventArg1 == SURFACE_PROVINCE_FARON) {
        p = mpSaveObjBoundingsFaron;
        count = ARRAY_LENGTH(mpSaveObjBoundingsFaron);
    } else if (mMapEventArg1 == SURFACE_PROVINCE_ELDIN) {
        p = mpSaveObjBoundingsEldin;
        count = ARRAY_LENGTH(mpSaveObjBoundingsEldin);
    } else if (mMapEventArg1 == SURFACE_PROVINCE_LANAYRU) {
        p = mpSaveObjBoundingsLanayru;
        count = ARRAY_LENGTH(mpSaveObjBoundingsLanayru);
    }
    mNumSaveObjs = count;
    mpCurrentSaveObjBoundings = p;
    mSavePopup.set(mpCurrentSaveObjBoundings, mNumSaveObjs);
}

void dLytMapMain_c::initSaveObjs() {
    nw4r::lyt::Bounding **ptrs = mpCurrentSaveObjBoundings;
    nw4r::lyt::Bounding *p;
    for (u32 i = 0; i < mNumSaveObjs; i++) {
        p = *ptrs;
        if (p->IsVisible()) {
            mVec3_c pos(p->GetGlobalMtx()._03, p->GetGlobalMtx()._13, 0.0f);
            mSaveObjs[i].setPosition(pos);
            mSaveObjs[i].setAlpha(p->GetGlobalAlpha());
            mSaveObjs[i].draw();
        }
        ptrs++;
    }
}

bool dLytMapMain_c::isPointingAtMainMap() const {
    bool ret = true;
    dCursorHitCheck_c *d = dCsGame_c::GetInstance()->getCursorHit();
    if (d != nullptr && d->getType() == 'lyt ') {
        for (int i = 0; i < MAP_MAIN_NUM_BOUNDINGS_NORMAL; i++) {
            if (d == &mHitChecks[i]) {
                ret = false;
                break;
            }
        }
    }
    return ret;
}

bool dLytMapMain_c::canPlaceBeacons(s32 mapMode) {
    switch (mMapEvent) {
        case 7:
        case 8:
        case 9:
        case 0:
        case 1: {
            if (!StoryflagManager::sInstance->getFlag(STORYFLAG_IMPA_MET)) {
                // Impa introduces beacons
                return false;
            }
            break;
        }
        default: return false;
    }
    bool ret = false;
    if (!doesStageForbidBeaconPlacement() &&
        (mapMode == dLytMapGlobal_c::MAPMODE_STAGE || mapMode == dLytMapGlobal_c::MAPMODE_ZOOM)) {
        ret = true;
    }
    return ret;
}

bool dLytMapMain_c::canResetPosition(s32 mapMode, bool mapUpDirection) const {
    return (mMapEvent == MAP_EVENT_NONE && ((mapMode == dLytMapGlobal_c::MAPMODE_ZOOM) ||
                                            (mapMode == dLytMapGlobal_c::MAPMODE_STAGE && mapUpDirection == false))) ||
           (isMapEventEq1Or7Or8Or9Or11() && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM);
}

bool dLytMapMain_c::isMapIntroDone() const {
    return mMapEvent == MAP_EVENT_MAP_INTRO && mEventDone;
}

bool dLytMapMain_c::isOpen() const {
    const sStateIDIf_c &stateID = *mStateMgr.getStateID();

    return stateID == StateID_Active || stateID == StateID_ChgMapUpDirection || stateID == StateID_FloorChange ||
           stateID == StateID_ChgDispAreaMode_Map || stateID == StateID_ChgDispAreaMode_MapRot ||
           stateID == StateID_ChgDispAreaMode_WA || stateID == StateID_ChgDispAreaMode_CsrRot ||
           stateID == StateID_ResetPos || stateID == StateID_ResetPosWithFloorChange ||
           stateID == StateID_EventSwBankSmall_Step1 || stateID == StateID_EventSwBankSmall_Step2 ||
           stateID == StateID_EventSwBankSmall_Step3 || stateID == StateID_EventMapIntro_Step1 ||
           stateID == StateID_EventMapIntro_Step2 || stateID == StateID_EventMapIntro_Step3 ||
           stateID == StateID_EventMapIntro_Step4 || stateID == StateID_EventDungeonMapGet_Step1 ||
           stateID == StateID_EventDungeonMapGet_Step2 || stateID == StateID_EventDungeonMapGet_Step3 ||
           stateID == StateID_EventFieldMapChange_Step1 || stateID == StateID_EventFieldMapChange_Step2 ||
           stateID == StateID_EventFieldMapChange_Step3 || stateID == StateID_EventFieldMapChange_Step4 ||
           stateID == StateID_EventSignalAdd_Step1 || stateID == StateID_EventSignalAdd_Step2 ||
           stateID == StateID_EventSignalAdd_Step3 || stateID == StateID_EventGoddessCube_Step1 ||
           stateID == StateID_EventGoddessCube_Step2 || stateID == StateID_EventGoddessCube_Step3 ||
           stateID == StateID_EventSaveObjMsgWindow || stateID == StateID_EventSaveObjSelect ||
           stateID == StateID_EventSaveObjConfirmMsgWindow || stateID == StateID_EventSaveObjDecide;
}

u8 dLytMapMain_c::getRoomAlpha(s32 roomid) const {
    u8 ret = 0xFF;

    if (dStageMgr_c::GetInstance()->isAreaTypeDungeonOrBoss()) {
        if (!dStage_c::GetInstance()->hasVisitedRoom(roomid)) {
            ret = mMapChangeAlpha;
        }
    } else {
        // Lake Floria interior
        if (mMapEvent == 6 && dScGame_c::isCurrentStage("F102") && (roomid == 3 || roomid == 4)) {
            ret = mMapChangeAlpha;
        }
    }
    return ret;
}

bool dLytMapMain_c::canOpen() const {
    if (dStage_c::GetInstance() != nullptr) {
        if (*mStateMgr.getStateID() == StateID_Invisible) {
            return true;
        }
    }
    return false;
}

bool dLytMapMain_c::isVisible() const {
    return !(*mStateMgr.getStateID() == StateID_Invisible);
}

void dLytMapMain_c::queueMapEvent(s32 mapEvent, s32 arg1, s32 arg2) {
    mNextMapEvent = mapEvent;
    mMapEventArg1 = arg1;
    mMapEventArg2 = arg2;
}

void dLytMapMain_c::startMapEvent(s32 mapEvent, s32 arg1, s32 arg2) {
    if (canOpen()) {
        mMapEvent = mapEvent;
        mMapEventArg1 = arg1;
        mMapEventArg2 = arg2;
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_OPEN_PAPER_NOISE);
        dLytMapGlobal_c *global = getGlobal();
        global->setMapEvent(mMapEvent);
        mStateMgr.changeState(StateID_RenderingWait);
    }
}

static const char *sTitle00 = "T_title_00";
static const char *sTitleS00 = "T_titleS_00";

void dLytMapMain_c::setupEvent(s32 event, s32 arg1, s32 arg2) {
    mRoomType = getRoomType();
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (link->checkActionFlagsCont(0x40)) {
        field_0x8CAE = true;
    } else {
        field_0x8CAE = false;
    }

    mMarkers.setLinkRot(link->getRotation());
    // TODO - mAng nonsense?
    field_0x8D68 = dScGame_c::getCamera(0)->fn_8019E3B0();
    mpAllPane->SetVisible(false);
    getGlobal()->setField_0x55(field_0x8CAE);
    mDoCameraTransition = false;
    switch (event) {
        case MAP_EVENT_NONE: {
            if (mRoomType != ROOMTYPE_BOSS_HOUSE && mRoomType != ROOMTYPE_SKYFIELD_INTERIOR && field_0x8CAE) {
                mCurrentMapMode = dLytMapGlobal_c::MAPMODE_ZOOM;
                mNextMapMode = mCurrentMapMode;
                mMapUpDirection = false;
                mDoCameraTransition = true;
            } else {
                if (!sSavedMapData.entries[mRoomType].isValid) {
                    if (MinigameManager::isInMinigameState(MinigameManager::SPIRAL_CHARGE_TUTORIAL)) {
                        mCurrentMapMode = dLytMapGlobal_c::MAPMODE_ZOOM;
                        fn_80142C80(mCurrentMapMode);
                        // TODO - regswap (???)
                        mMapUpDirection = false;
                        mDoCameraTransition = true;
                    } else {
                        loadMapState();
                        if (mRoomType == ROOMTYPE_SKYFIELD) {
                            mCurrentMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                            mMapUpDirection = true;
                        } else if (mRoomType == ROOMTYPE_BOSS_HOUSE || mRoomType == ROOMTYPE_SKYFIELD_INTERIOR) {
                            mCurrentMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                            mMapUpDirection = true;
                        }
                        if (mMapUpDirection == false) {
                            mDoCameraTransition = true;
                        }
                    }
                } else {
                    loadMapState();
                    if (mMapUpDirection == false) {
                        mDoCameraTransition = true;
                    }
                }
                if (link->checkActionFlags(daPlayerActBase_c::FLG0_CRAWLING)) {
                    mDoCameraTransition = false;
                }
            }
            if (mRoomType == ROOMTYPE_DUNGEON) {
                mDoCameraTransition = false;
            }
            break;
        }
        case MAP_EVENT_1:
        case MAP_EVENT_SIGNAL_ADD:
        case MAP_EVENT_GODDESS_CUBE:
        case MAP_EVENT_11:           {
            // why does everyone clamp things differently ???
            s32 tmp = arg1;
            if (arg1 > mBaseFloorOffset) {
                tmp = mBaseFloorOffset;
            } else if (tmp < field_0x8D64) {
                tmp = field_0x8D64;
            }
            getGlobal()->setFloor(tmp);
            switch (arg2) {
                case -2: mCurrentMapMode = dLytMapGlobal_c::MAPMODE_WORLD; break;
                case -1:
                    if (mRoomType == ROOMTYPE_SKYLOFT) {
                        mCurrentMapMode = dLytMapGlobal_c::MAPMODE_WORLD_SKY;
                    } else {
                        mCurrentMapMode = dLytMapGlobal_c::MAPMODE_PROVINCE;
                    }
                    break;
                case 0: mCurrentMapMode = dLytMapGlobal_c::MAPMODE_STAGE; break;
                case 1: mCurrentMapMode = dLytMapGlobal_c::MAPMODE_ZOOM; break;
            }
            mMapUpDirection = true;
            mDoCameraTransition = false;
            break;
        }
        case MAP_EVENT_MAP_INTRO: {
            if (mRoomType == ROOMTYPE_SKYLOFT) {
                mCurrentMapMode = dLytMapGlobal_c::MAPMODE_WORLD_SKY;
            } else {
                mCurrentMapMode = dLytMapGlobal_c::MAPMODE_WORLD;
            }
            mMapUpDirection = true;
            mDoCameraTransition = false;
            break;
        }
        case MAP_EVENT_DUNGEON_MAP_GET:
        case MAP_EVENT_FOREST_MAP_CHANGE: {
            dLytMapGlobal_c *global = getGlobal();
            global->setField_0x4C(1);
            global->setField_0x4F(0);
            // fall-through
        }
        case MAP_EVENT_SW_BANK_SMALL: {
            mCurrentMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
            mMapUpDirection = true;
            mDoCameraTransition = false;
            break;
        }
        case MAP_EVENT_FIELD_MAP_CHANGE_5:
        case MAP_EVENT_FIELD_MAP_CHANGE_8: {
            mCurrentMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
            mMapUpDirection = true;
            mDoCameraTransition = false;
            break;
        }
        case MAP_EVENT_SAVE_OBJ: {
            mCurrentMapMode = dLytMapGlobal_c::MAPMODE_PROVINCE;
            mMapUpDirection = true;
            mDoCameraTransition = false;
            break;
        }
    }

    mNextMapUpDirection = mMapUpDirection;
    mNextMapMode = mCurrentMapMode;
    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE ||
        mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) {
        mDoCameraTransition = false;
    }

    if (mDoCameraTransition) {
        dScGame_c::getCamera(0)->doFn_80093380();
    } else {
        dScGame_c::getCamera(0)->doFn_80093360();
        dScGame_c::getCamera(0)->doFn_80093340();
    }
    dScGame_c::getCamera(0)->fn_8019E410();
    dLytMapMain_vt0x10();
    s32 floor = dStage_c::GetInstance()->getMapRelated()->getField_0x1EC();
    mCurrentFloor = floor;
    if (event != MAP_EVENT_1 && event != MAP_EVENT_SIGNAL_ADD) {
        getGlobal()->setFloor(floor);
    }
    mFloorBtnMgr.init(mNumFloors, mBaseFloorOffset, getGlobal()->getFloor());
    s32 mapLabelNumber = dStageMgr_c::GetInstance()->getMapNameId();
    if (mapLabelNumber >= 0) {
        SizedString<16> label;
        label.sprintf("MAP_%02d", mapLabelNumber);
        dTextBox_c *box = mLyt.getTextBox(sTitle00);
        box->setMessageWithGlobalTextProcessor2(label, nullptr);
        const wchar_t *t2 = dMessage_c::getTextMessageByLabel(label, 1, nullptr, 0);
        mNumTitleLines = box->calcTextLines(t2, dMessage_c::getGlobalTagProcessor());
        // instruction swap...
        box = mLyt.getTextBox(sTitleS00);
        box->setMessageWithGlobalTextProcessor2(label, nullptr);
    }
    getGlobal()->setCurrentMapMode(mCurrentMapMode);
    getGlobal()->setNextMapMode(mNextMapMode);

    if (dStageMgr_c::GetInstance()->getSTIFRoomType() != dStageMgr_c::STIF_ROOMTYPE_BOSS_HOUSE &&
        (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE ||
         mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE)) {
        getGlobal()->setZoomFrame(0.0f);
        getGlobal()->setField_0x58(1.0f);
    } else {
        f32 f1 = fn_80142D90(dLytMapGlobal_c::MAPMODE_STAGE);
        f32 f2 = fn_80142D90(dLytMapGlobal_c::MAPMODE_ZOOM);
        getGlobal()->setZoomFrame(1.0f);
        getGlobal()->setField_0x58(f2 / f1);
    }
    field_0x8C84 = 0;
    (void)getLinkPtr();
}

void dLytMapMain_c::close() {
    dBase_c::s_DrawControlFlags &= ~dBase_c::BASE_PROP_0x10;
    mDoCameraTransition = false;
    if (mMapEvent == MAP_EVENT_NONE) {
        if (mRoomType == ROOMTYPE_FIELD || mRoomType == ROOMTYPE_SKYLOFT) {
            if (!field_0x8CAE && !mMapUpDirection) {
                mDoCameraTransition = true;
            }
        } else if (mRoomType == ROOMTYPE_DUNGEON && !field_0x8CAE && !mMapUpDirection) {
            mDoCameraTransition = false;
            dScGame_c::getCamera(0)->doFn_80093340();
        }
    }

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE ||
        mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) {
        mDoCameraTransition = false;
    }

    if (mDoCameraTransition) {
        dScGame_c::getCamera(0)->doFn_80093380();
    } else {
        dScGame_c::getCamera(0)->doFn_80093360();
    }

    if (mMapEvent == MAP_EVENT_NONE) {
        saveMapState();
    }

    mEventDone = false;
    dScGame_c::getCamera(0)->fn_8019E430();

    dSndPlayerMgr_c::GetInstance()->leaveMap();
    mStateMgr.changeState(StateID_Out);
}

bool dLytMapMain_c::isVisibleNoIntro() const {
    return isVisible() && mMapEvent != MAP_EVENT_MAP_INTRO;
}

void dLytMapMain_c::saveMapState() {
    dMapSavedDataEntry &data = sSavedMapData.entries[mRoomType];
    data.isValid = true;
    data.mapMode = mCurrentMapMode;
    data.mapUpDirection = mMapUpDirection;
    data.mapUpDirectionAfterZoomToDetail = mMapUpDirectionAfterZoomToDetail;
    sSavedMapData.showIslandNames = mShowIslandNames;
}

void dLytMapMain_c::initMapState() {
    for (int i = 0; i < 6; i++) {
        sSavedMapData.entries[i].isValid = sDefaultMapData.entries[i].isValid;
        sSavedMapData.entries[i].mapMode = sDefaultMapData.entries[i].mapMode;
        sSavedMapData.entries[i].mapUpDirection = sDefaultMapData.entries[i].mapUpDirection;
        sSavedMapData.entries[i].mapUpDirectionAfterZoomToDetail =
            sDefaultMapData.entries[i].mapUpDirectionAfterZoomToDetail;
    }
    sSavedMapData.showIslandNames = sDefaultMapData.showIslandNames;
}

void dLytMapMain_c::loadMapState() {
    mCurrentMapMode = sSavedMapData.entries[mRoomType].mapMode;
    mMapUpDirection = sSavedMapData.entries[mRoomType].mapUpDirection;
    mMapUpDirectionAfterZoomToDetail = sSavedMapData.entries[mRoomType].mapUpDirectionAfterZoomToDetail;
    mShowIslandNames = sSavedMapData.showIslandNames;
}

void dLytMapMain_c::clearButtonMessages() {
    // @bug this clears flags 16k times
    for (int i = 0; i <= METER_BTN_0x4000; i++) {
        dLytMeter_c::GetInstance()->clearFlags(i);
    }
}

void dLytMapMain_c::setButtonMessages(
    s32 currentMapMode, bool currentUpDirection, s32 nextMapMode, bool nextUpDirection
) {
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_C, LytDoButtonRelated::ACT_IE_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::ACT_IE_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_BG, LytDoButtonRelated::ACT_IE_NONE);
    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_NONE);

    if (canCenterCursor(currentMapMode) || canCenterCursor(nextMapMode)) {
        dLytMeter_c::GetInstance()->setFlags(METER_BTN_CROSS_DOWN);
        LytDoButtonRelated::setCrossDown(LytDoButtonRelated::ACT_IE_ETC_CENTER, true);
    }

    // TODO - some of these parts directly access members instead of using function args...

    // TODO - canCenterCursor1 looks like an inline that somehow failed to get inlined (it's emitted right after this
    // function...)
    if (canCenterCursor1(mCurrentMapMode) && canCenterCursor1(mNextMapMode) && canPlaceBeacons(mCurrentMapMode) &&
        canPlaceBeacons(mNextMapMode)) {
        if (field_0x8DBD && *mPinIconAggregate.mStateMgr.getStateID() != dLytMapPinIconAggregate_c::StateID_Select) {
            dLytMeter_c::GetInstance()->setFlags(METER_BTN_C);
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_C, LytDoButtonRelated::ACT_IE_ETC_ADD_BEACON);
        } else if (*mPinIconAggregate.mStateMgr.getStateID() == dLytMapPinIconAggregate_c::StateID_Select) {
            dLytMeter_c::GetInstance()->setFlags(METER_BTN_C);
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_C, LytDoButtonRelated::ACT_IE_ETC_REMOVE_BEACON);
        } else if (!(*mStateMgr.getStateID() == StateID_In) && !(*mStateMgr.getStateID() == StateID_Out)) {
            dLytMeter_c::GetInstance()->setFlags(METER_BTN_NUN_STK);
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_NONE);
        }
    }

    if (canZoomOut(currentMapMode) && canZoomOut(nextMapMode)) {
        if ((mRoomType == ROOMTYPE_FIELD && currentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) ||
            (mRoomType == ROOMTYPE_SKYLOFT && currentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) ||
            (mRoomType == ROOMTYPE_SKYFIELD && currentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) ||
            (mRoomType == ROOMTYPE_BOSS_HOUSE && currentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) ||
            (mRoomType == ROOMTYPE_SKYFIELD_INTERIOR && currentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) ||
            (mRoomType == ROOMTYPE_DUNGEON && currentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE)) {
            dLytMeter_c::GetInstance()->setFlags(METER_BTN_CROSS_LEFT);
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_L, LytDoButtonRelated::ACT_IE_ETC_WORLD_MAP);
        } else {
            dLytMeter_c::GetInstance()->setFlags(METER_BTN_CROSS_LEFT);
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_L, LytDoButtonRelated::ACT_IE_ETC_ZOOM_OUT);

            if (currentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_NUN_STK);
                LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_ETC_ZOOM_OUT);
            }
        }
    }

    if (canZoomIn(currentMapMode) && canZoomIn(nextMapMode)) {
        dLytMeter_c::GetInstance()->setFlags(METER_BTN_CROSS_RIGHT);
        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_CROSS_R, LytDoButtonRelated::ACT_IE_ETC_ZOOM_IN);

        if (currentMapMode == dLytMapGlobal_c::MAPMODE_STAGE && nextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            dLytMeter_c::GetInstance()->setFlags(METER_BTN_NUN_STK);
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_ETC_ZOOM_IN);
        }
    }

    // Directly accessing members instead of using function args...
    if (needsNav(mCurrentMapMode) && needsNav(mNextMapMode)) {
        dLytMeter_c::GetInstance()->setFlags(METER_BTN_NUN_STK);
        dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x1000);
        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_ETC_SELECT_FLOOR);

        if (mFloorBtnMgr.hasPointedAtABtnIdx() && mFloorBtnMgr.canDecideFloor()) {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_ETC_DISPLAY);
        }
    }

    if (canChangeUpDirection(currentMapMode, currentUpDirection) &&
        canChangeUpDirection(nextMapMode, nextUpDirection)) {
        dLytMeter_c::GetInstance()->setFlags(METER_BTN_Z);

        if (!currentUpDirection) {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::ACT_IE_ETC_ROTATE_MAP);
        } else {
            LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::ACT_IE_ETC_ROTATE_MAP_2);
        }
    }

    switch (mRoomType) {
        case ROOMTYPE_SKYLOFT: {
            if (currentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_CROSS_UP);
                LytDoButtonRelated::setCrossTop(LytDoButtonRelated::ACT_IE_ETC_CURRENT_LOCATION, true);
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_Z);
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x1000);
                LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_ETC_SCROLL);
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x4000);
            } else if (currentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
                if (canResetPosition(mCurrentMapMode, mMapUpDirection)) {
                    dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x1000);
                    LytDoButtonRelated::set(
                        LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_ETC_SCROLL
                    );
                } else {
                    dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x1000);
                    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_NONE);
                }
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x4000);
            }
            break;
        }
        case ROOMTYPE_FIELD:
        case ROOMTYPE_SKYFIELD: {
            if (currentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_CROSS_UP);
                LytDoButtonRelated::setCrossTop(LytDoButtonRelated::ACT_IE_ETC_CURRENT_LOCATION, true);
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x1000);
                LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_ETC_SCROLL);
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x4000);
            } else if (currentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
                if (canResetPosition(mCurrentMapMode, mMapUpDirection)) {
                    dLytMeter_c::GetInstance()->setFlags(METER_BTN_CROSS_UP);
                    LytDoButtonRelated::setCrossTop(LytDoButtonRelated::ACT_IE_ETC_CURRENT_LOCATION, true);
                    dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x1000);
                    LytDoButtonRelated::set(
                        LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_ETC_SCROLL
                    );
                } else {
                    dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x1000);
                    LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_NONE);
                }
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x4000);
            }

            if (mRoomType == ROOMTYPE_SKYFIELD && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE &&
                mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_Z);
                if (mShowIslandNames) {
                    LytDoButtonRelated::set(
                        LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::ACT_IE_ETC_HIDE_ISLAND_NAME
                    );
                } else {
                    LytDoButtonRelated::set(
                        LytDoButtonRelated::DO_BUTTON_Z, LytDoButtonRelated::ACT_IE_ETC_SHOW_ISLAND_NAME
                    );
                }
            }
            break;
        }
        case ROOMTYPE_DUNGEON: {
            if (currentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_CROSS_UP);
                LytDoButtonRelated::setCrossTop(LytDoButtonRelated::ACT_IE_ETC_CURRENT_LOCATION, true);
                dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x1000);
                LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_NUN_STK, LytDoButtonRelated::ACT_IE_ETC_SCROLL);

                if (needsNav(mCurrentMapMode) && needsNav(mNextMapMode) && mFloorBtnMgr.hasPointedAtABtnIdx()) {
                    if (mFloorBtnMgr.canDecideFloor()) {
                        LytDoButtonRelated::set(
                            LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_ETC_DISPLAY
                        );
                    } else {
                        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_NONE);
                    }
                }

                dLytMeter_c::GetInstance()->setFlags(METER_BTN_0x4000);

            } else if (currentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
                if (needsNav(mCurrentMapMode) && needsNav(mNextMapMode) && mFloorBtnMgr.hasPointedAtABtnIdx()) {
                    if (mFloorBtnMgr.canDecideFloor()) {
                        LytDoButtonRelated::set(
                            LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_ETC_DISPLAY
                        );
                    } else {
                        LytDoButtonRelated::set(LytDoButtonRelated::DO_BUTTON_A, LytDoButtonRelated::ACT_IE_NONE);
                    }
                }
            }
            break;
        }
    }
}

bool dLytMapMain_c::canCenterCursor1(s32 mapMode) const {
    return canCenterCursor(mapMode);
}

void dLytMapMain_c::setCursorType() {
    if (canCenterCursor1(mCurrentMapMode) && canCenterCursor1(mNextMapMode)) {
        if ((mMapEvent == MAP_EVENT_SAVE_OBJ ||
             ((mRoomType == ROOMTYPE_BOSS_HOUSE || mRoomType == ROOMTYPE_DUNGEON) && mPointerOnMap)) ||
            (mPointerCanPlaceBeacon && isPointingAtMainMap()) ||
            (*mPinIconAggregate.mStateMgr.getStateID() == dLytMapPinIconAggregate_c::StateID_Select) ||
            (needsNav(mCurrentMapMode) && needsNav(mNextMapMode) && mFloorBtnMgr.hasPointedAtABtnIdx())) {
            dCsGame_c::GetInstance()->pointerDefSet();
        } else {
            dCsGame_c::GetInstance()->pointerCatSet();
        }
    }
}

void dLytMapMain_c::calculateBoundingWorldHitboxes() {
    dLytMapGlobal_c *global = getGlobal();
    f32 sc = global->getField_0x28().x;
    f32 f1 = field_0x8D40 * global->getField_0x44();
    f32 f2 = f1 * sc;
    mAng rot = global->getField_0x56();

    for (int i = 0; i <= MAP_MAIN_NUM_BOUNDINGS_NORMAL - 1; i++) {
        mVec3_c v1;
        mVec3_c v2;

        f32 v1x = (f32)mHitChecks[i].getBoundingData()->mMinX;
        f32 v2x = (f32)mHitChecks[i].getBoundingData()->mMaxX;
        f32 v1z = (f32)mHitChecks[i].getBoundingData()->mMaxY;
        f32 v2z = (f32)mHitChecks[i].getBoundingData()->mMinY;

        v1.x = v1x / f2;
        v1.y = 0.0f;
        v1.z = -v1z / f1;

        v2.x = v2x / f2;
        v2.y = 0.0f;
        v2.z = -v2z / f1;

        v1.rotY(rot);
        v2.rotY(rot);

        // Looks like this is manually unprojecting the UI element
        // hitbox corners?
        s16 v = rot.mVal;
        if (v == 0) {
            // 0° rotation
            mBoundingWorldBounds[i].min.x = field_0x8CC4.x + v1.x;
            mBoundingWorldBounds[i].min.y = 0.0f;
            mBoundingWorldBounds[i].min.z = field_0x8CC4.z + v1.z;

            mBoundingWorldBounds[i].max.x = field_0x8CC4.x + v2.x;
            mBoundingWorldBounds[i].max.y = 0.0f;
            mBoundingWorldBounds[i].max.z = field_0x8CC4.z + v2.z;
        } else if (v == 0x4000) {
            // 90° rotation
            mBoundingWorldBounds[i].min.x = field_0x8CC4.x + v1.x;
            mBoundingWorldBounds[i].min.y = 0.0f;
            mBoundingWorldBounds[i].min.z = field_0x8CC4.z + v2.z;

            mBoundingWorldBounds[i].max.x = field_0x8CC4.x + v2.x;
            mBoundingWorldBounds[i].max.y = 0.0f;
            mBoundingWorldBounds[i].max.z = field_0x8CC4.z + v1.z;
        } else if (v == -0x8000) {
            // 180° rotation
            mBoundingWorldBounds[i].min.x = field_0x8CC4.x + v2.x;
            mBoundingWorldBounds[i].min.y = 0.0f;
            mBoundingWorldBounds[i].min.z = field_0x8CC4.z + v2.z;

            mBoundingWorldBounds[i].max.x = field_0x8CC4.x + v1.x;
            mBoundingWorldBounds[i].max.y = 0.0f;
            mBoundingWorldBounds[i].max.z = field_0x8CC4.z + v1.z;
        }
    }
}

void dLytMapMain_c::initializeState_Invisible() {}
void dLytMapMain_c::executeState_Invisible() {}
void dLytMapMain_c::finalizeState_Invisible() {}

void dLytMapMain_c::initializeState_RenderingWait() {}
void dLytMapMain_c::executeState_RenderingWait() {}
void dLytMapMain_c::finalizeState_RenderingWait() {}

struct dLytMapStoryflagPaneMapping {
    /* 0x00 */ s32 storyFlag;
    /* 0x04 */ const char *paneName;
};

static const dLytMapStoryflagPaneMapping sStoryflagPaneMap[] = {
    {      STORYFLAG_FARON_PILLAR_OPENED,   "N_forestP_00"},
    {      STORYFLAG_ELDIN_PILLAR_OPENED,  "N_volcanoP_00"},
    {    STORYFLAG_LANAYRU_PILLAR_OPENED,   "N_desertP_00"},
    {         STORYFLAG_FARON_DISCOVERED,  "N_toForest_00"},
    {         STORYFLAG_ELDIN_DISCOVERED, "N_toVolcano_00"},
    {       STORYFLAG_LANAYRU_DISCOVERED,  "N_toDesert_00"},
    { STORYFLAG_ISLE_OF_SONGS_DISCOVERED,      "N_utaN_00"},
    {STORYFLAG_FUN_FUN_ISLAND_DISCOVERED,  "N_ruretouN_00"},
    { STORYFLAG_LUMPY_PUMPKIN_DISCOVERED,  "N_pampkinN_00"},
    { STORYFLAG_BEEDLE_ISLAND_DISCOVERED,     "N_teryN_00"},
    { STORYFLAG_BAMBOO_ISLAND_DISCOVERED,  "N_tikurinN_00"},
    {     STORYFLAG_BUG_HAVEN_DISCOVERED,     "N_musiN_00"},
    { STORYFLAG_ISLE_OF_SONGS_DISCOVERED,      "N_utaI_00"},
    {STORYFLAG_FUN_FUN_ISLAND_DISCOVERED,  "N_ruretouI_00"},
    { STORYFLAG_LUMPY_PUMPKIN_DISCOVERED,  "N_pampkinI_00"},
    { STORYFLAG_BEEDLE_ISLAND_DISCOVERED,     "N_teryI_00"},
    { STORYFLAG_BAMBOO_ISLAND_DISCOVERED,  "N_tikurinI_00"},
    {     STORYFLAG_BUG_HAVEN_DISCOVERED,     "N_musiI_00"},
    {STORYFLAG_BILOCYCTE_FIGHT_TRIGGERED,     "N_nushi_00"},
};

// 255 is a placeholder for "no flag", despite it corresponding to an actual story flag...
static const s32 sFlagsRenameMe[] = {
    255, STORYFLAG_FARON_PILLAR_OPENED, STORYFLAG_ELDIN_PILLAR_OPENED, STORYFLAG_LANAYRU_PILLAR_OPENED,
    STORYFLAG_THUNDERHEAD_ENTERED
};

extern const s32 unkPadding[];
const s32 unkPadding[14] = {0};

// Not sure what the second number means
static const s32 sFlags2RenameMe[][2] = {
    {         STORYFLAG_FARON_DISCOVERED, 5},
    {   STORYFLAG_FARON_WOODS_DISCOVERED, 2},
    {    STORYFLAG_DEEP_WOODS_DISCOVERED, 2},
    {         STORYFLAG_ELDIN_DISCOVERED, 3},
    {       STORYFLAG_LANAYRU_DISCOVERED, 4},
    {STORYFLAG_LANAYRU_DESERT_DISCOVERED, 4},
    {   STORYFLAG_LAKE_FLORIA_DISCOVERED, 2},
    { STORYFLAG_LANAYRU_CAVES_DISCOVERED, 4},
    {STORYFLAG_ANCIENT_HARBOR_DISCOVERED, 4},
    {STORYFLAG_VOLCANO_SUMMIT_DISCOVERED, 3},
    { STORYFLAG_LANAYRU_GORGE_DISCOVERED, 4},
};

static const s32 sDungeonDiscoveredFlags[] = {
    STORYFLAG_SKYVIEW_DISCOVERED,         STORYFLAG_EARTH_TEMPLE_DISCOVERED, STORYFLAG_LMF_DISCOVERED,
    STORYFLAG_ANCIENT_CISTERN_DISCOVERED, STORYFLAG_SANDSHIP_DISCOVERED,     STORYFLAG_FIRE_SANCTUARY_DISCOVERED,
    STORYFLAG_SKY_KEEP_DISCOVERED,
};

static const char *sAreaPicNamesL[] = {
    "P_plainL_00", "P_forestL_00", "P_nForestL_00", "P_sVolcanoL_00", "P_mineL_00",    "P_desertL_00",
    "P_lakeL_00",  "P_glenL_00",   "P_seaL_00",     "P_nVolcanoL_00", "P_skyloftL_00", "P_glenL_01",
};

static const char *sAreaPicNamesLink[] = {
    "P_plainLink_00", "P_forestLink_00", "P_nForestLink_00", "P_sVolLink_00", "P_mineLink_00",    "P_desertLink_00",
    "P_lakeLink_00",  "P_glenLink_00",   "P_seaLink_00",     "P_nVolLink_00", "P_skyloftLink_00", "P_glenLink_01",
};

#define MAP_MAIN_AREA_PIC_PLAIN 0
#define MAP_MAIN_AREA_PIC_FOREST 1
#define MAP_MAIN_AREA_PIC_N_FOREST 2
#define MAP_MAIN_AREA_PIC_S_VOLCANO 3
#define MAP_MAIN_AREA_PIC_MINE 4
#define MAP_MAIN_AREA_PIC_DESERT 5
#define MAP_MAIN_AREA_PIC_LACE 6
#define MAP_MAIN_AREA_PIC_GLEN_00 7
#define MAP_MAIN_AREA_PIC_SEA 8
#define MAP_MAIN_AREA_PIC_N_VOLCANO 9
#define MAP_MAIN_AREA_PIC_SKYLOFT 10
#define MAP_MAIN_AREA_PIC_GLEN_01 11

#define MAP_MAIN_NUM_AREA_PICS 12

void dLytMapMain_c::setupFlags() {
    if (mMapEvent == MAP_EVENT_DUNGEON_MAP_GET || mMapEvent == MAP_EVENT_FOREST_MAP_CHANGE) {
        mMapChangeAlpha = 0;
    } else {
        mMapChangeAlpha = 0xFF;
    }
    mEventDone = false;
    mEventCanceled = false;

    mpAllPane->SetVisible(true);
    s32 currentRegion;
    switch (dStageMgr_c::GetInstance()->getSTIFArea()) {
        default:                                      currentRegion = MAP_MAIN_AREA_PIC_SKYLOFT; break;
        case dStageMgr_c::STIF_AREA_SEALED_GROUNDS:   currentRegion = MAP_MAIN_AREA_PIC_PLAIN; break;
        case dStageMgr_c::STIF_AREA_FARON_WOODS:      currentRegion = MAP_MAIN_AREA_PIC_FOREST; break;
        case dStageMgr_c::STIF_AREA_DEEP_WOODS:       currentRegion = MAP_MAIN_AREA_PIC_N_FOREST; break;
        case dStageMgr_c::STIF_AREA_LAKE_FLORIA:      currentRegion = MAP_MAIN_AREA_PIC_LACE; break;
        case dStageMgr_c::STIF_AREA_VOLCANO_SOUTH:    currentRegion = MAP_MAIN_AREA_PIC_S_VOLCANO; break;
        case dStageMgr_c::STIF_AREA_VOLCANO_NORTH:    currentRegion = MAP_MAIN_AREA_PIC_N_VOLCANO; break;
        case dStageMgr_c::STIF_AREA_LANAYRU_MINE:     currentRegion = MAP_MAIN_AREA_PIC_MINE; break;
        case dStageMgr_c::STIF_AREA_LANAYRU_DESERT:   currentRegion = MAP_MAIN_AREA_PIC_DESERT; break;
        case dStageMgr_c::STIF_AREA_LANAYRU_SAND_SEA: currentRegion = MAP_MAIN_AREA_PIC_SEA; break;
        case dStageMgr_c::STIF_AREA_LANAYRU_CAVES:    currentRegion = MAP_MAIN_AREA_PIC_GLEN_00; break;
        case dStageMgr_c::STIF_AREA_LANAYRU_GORGE:    currentRegion = MAP_MAIN_AREA_PIC_GLEN_01; break;
    }
    // TODO - regswap between i and ok
    for (int i = 0; i < (int)ARRAY_LENGTH(sStoryflagPaneMap); i++) {
        nw4r::lyt::Pane *p = mLyt.findPane(sStoryflagPaneMap[i].paneName);
        bool ok = StoryflagManager::sInstance->getFlag(sStoryflagPaneMap[i].storyFlag) != 0;
        if (i >= 0 && i < 6 && (dScGame_c::isCurrentStage("F403") || dScGame_c::isCurrentStage("F404"))) {
            // If we're in the past, light pillars aren't a thing
            ok = false;
        }
        if (ok) {
            p->SetVisible(true);
        } else {
            p->SetVisible(false);
        }
    }

    if (StoryflagManager::sInstance->getFlag(STORYFLAG_BILOCYCTE_FIGHT_TRIGGERED)) {
        mAnmGroups[MAP_MAIN_ANIM_NUSHI_LOOP].bind(false);
    }
    mAnmGroups[MAP_MAIN_ANIM_KUMO_PATTERN].bind(false);

    f32 titleLineFrame = 4.0f;
    // TODO - lots of regswaps
    for (int i = ARRAY_LENGTH(sFlagsRenameMe) - 1; i >= 0; i--) {
        s32 flag = sFlagsRenameMe[i];
        // This might be an inline...
        bool doBreak = true;
        bool flagSet = flag != 255 && StoryflagManager::sInstance->getCounterOrFlag(flag);
        if (!flagSet && flag != 255) {
            doBreak = false;
        }
        if (doBreak) {
            break;
        }
        titleLineFrame -= 1.0f;
    }
    mAnmGroups[MAP_MAIN_ANIM_KUMO_PATTERN].setFrame(titleLineFrame);
    mAnmGroups[MAP_MAIN_ANIM_MAP_V].bind(false);
    if (mRoomType == ROOMTYPE_DUNGEON || mRoomType == ROOMTYPE_BOSS_HOUSE) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
            mAnmGroups[MAP_MAIN_ANIM_MAP_V].setFrame(3.0f);
        }
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            mAnmGroups[MAP_MAIN_ANIM_MAP_V].setFrame(2.0f);
        }
    }

    field_0x8880[0] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_PLAIN];
    field_0x8880[1] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_FOREST];
    field_0x8880[2] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_N_FOREST];
    field_0x8880[3] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_S_VOLCANO];
    field_0x8880[4] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_MINE];
    field_0x8880[5] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_DESERT];
    field_0x8880[6] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_LAKE];
    field_0x8880[7] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_GLEN_01];
    field_0x8880[8] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_SEA];
    field_0x8880[9] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_N_VOLCANO];
    field_0x8880[10] = &mAnmGroups[MAP_MAIN_ANIM_DRAW_GLEN_00];

    for (int i = 0; i < (int)ARRAY_LENGTH(field_0x8880); i++) {
        if (field_0x8880[i] != nullptr) {
            field_0x8880[i]->bind(false);
            field_0x8880[i]->setToEnd();
        }
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(mpPanes); i++) {
        if (mpPanes[i] != nullptr) {
            mpPanes[i]->SetVisible(true);
        }
    }

    field_0x88AC = -1;

    for (int i = ARRAY_LENGTH(sFlags2RenameMe) - 1; i >= 0; i--) {
        s32 flag = sFlags2RenameMe[i][0];
        bool flagSet = flag != 255 && StoryflagManager::sInstance->getCounterOrFlag(flag);
        if (flagSet) {
            field_0x88AC = i;
            break;
        }
        if (mpPanes[i] != nullptr) {
            mpPanes[i]->SetVisible(false);
        }
        if (field_0x8880[i] != nullptr) {
            field_0x8880[i]->setFrame(0.0f);
        }
    }

    if (mMapEvent == MAP_EVENT_MAP_INTRO) {
        if (field_0x88AC < 0) {
            field_0x88AC = 0;
        }
        if (field_0x88AC >= 0) {
            if (mpPanes[field_0x88AC] != nullptr) {
                mpPanes[field_0x88AC]->SetVisible(true);
            }
            if (field_0x8880[field_0x88AC] != nullptr) {
                field_0x8880[field_0x88AC]->setFrame(0.0f);
            }
        }
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(sDungeonDiscoveredFlags); i++) {
        if (mpDungeonPanes[i] != nullptr) {
            mpDungeonPanes[i]->SetVisible(true);
        }
        if (mpDungeonPics[i] != nullptr) {
            mpDungeonPics[i]->SetVisible(true);
        }
    }

    for (int i = ARRAY_LENGTH(sDungeonDiscoveredFlags) - 1; i >= 0; i--) {
        s32 flag = sDungeonDiscoveredFlags[i];
        bool flagSet = flag != 255 && StoryflagManager::sInstance->getCounterOrFlag(flag);
        if (flagSet) {
            break;
        }
        if (mpDungeonPanes[i] != nullptr) {
            mpDungeonPanes[i]->SetVisible(false);
        }
        if (mpDungeonPics[i] != nullptr) {
            mpDungeonPics[i]->SetVisible(false);
        }
    }

    if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_TRIFORCE_COMPLETE) &&
        !dScGame_c::isCurrentStage("F403") && !dScGame_c::isCurrentStage("F404")) {
        if (mpDungeonPanes[6] != nullptr) {
            mpDungeonPanes[6]->SetVisible(false);
        }
        if (mpDungeonPics[6] != nullptr) {
            mpDungeonPics[6]->SetVisible(false);
        }
    }

    mAnmGroups[MAP_MAIN_ANIM_WORLD_STATE].bind(false);

    if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_TRIFORCE_COMPLETE) &&
        !dScGame_c::isCurrentStage("F403") && !dScGame_c::isCurrentStage("F404")) {
        mAnmGroups[MAP_MAIN_ANIM_WORLD_STATE].setFrame(1.0f);
    } else {
        mAnmGroups[MAP_MAIN_ANIM_WORLD_STATE].setFrame(0.0f);
    }
    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    if (mAnmGroups[MAP_MAIN_ANIM_WORLD_STATE].isBound()) {
        mAnmGroups[MAP_MAIN_ANIM_WORLD_STATE].unbind();
    }

    for (int i = 0; i < 11; i++) {
        if (field_0x8880[i] != nullptr) {
            field_0x8880[i]->unbind();
        }
    }

    mAnmGroups[MAP_MAIN_ANIM_LINK_POSITION_LIGHT].bind(false);
    mAnmGroups[MAP_MAIN_ANIM_ROTATE].bind(false);

    d2d::AnmGroup_c *grpZIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_Z_IN_FOREST];
    d2d::AnmGroup_c *grpZAIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_ZA_IN_FOREST];
    d2d::AnmGroup_c *grpZWakuIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_Z_WAKU_IN];
    if (mAreaGroup == AREAGROUP_SEALED_GROUNDS) {
        grpZIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_Z_IN_PLANT];
        grpZAIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_ZA_IN_PLANT];
    } else if (mAreaGroup == AREAGROUP_ELDIN) {
        grpZIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_Z_IN_VOLCANO];
        grpZAIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_ZA_IN_VOLCANO];
    } else if (mAreaGroup == AREAGROUP_LANAYRU) {
        grpZIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_Z_IN_DESERT];
        grpZAIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_ZA_IN_DESERT];
    }

    if (mRoomType == ROOMTYPE_SKYLOFT ||
        (mAreaGroup == AREAGROUP_SKY && (mRoomType == ROOMTYPE_BOSS_HOUSE || mRoomType == ROOMTYPE_DUNGEON))) {
        grpZIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_ZA_IN_SKY_MAP_2];
        grpZAIn = &mAnmGroups[MAP_MAIN_ANIM_MAP_ZA_OUT_SKY_COURSE];
    }

    mAnmGroups[MAP_MAIN_ANIM_SKYLOFT_UP_DOWN].bind(false);
    mAnmGroups[MAP_MAIN_ANIM_CLOUD_LOOP].bind(false);
    mAnmGroups[MAP_MAIN_ANIM_LIGHT_LOOP].bind(false);

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        grpZAIn->bind(false);
        grpZAIn->setFrame(0.0f);
        grpZWakuIn->bind(false);
        grpZWakuIn->setToEnd();
    } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE ||
               mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) {
        grpZAIn->bind(false);
        grpZAIn->setToEnd();
        // no group1->bind(false) ?
        grpZIn->setToEnd();
        grpZWakuIn->bind(false);
        grpZWakuIn->setFrame(0.0f);
    } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD) {
        grpZIn->bind(false);
        grpZIn->setFrame(0.0f);
        grpZWakuIn->bind(false);
        grpZWakuIn->setFrame(0.0f);
    }

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mAnmGroups[MAP_MAIN_ANIM_ROTATE].setFrame(0.0f);
    } else {
        mAnmGroups[MAP_MAIN_ANIM_ROTATE].setToEnd();
    }

    if (mMapEvent == MAP_EVENT_SW_BANK_SMALL) {
        mAnmGroups[MAP_MAIN_ANIM_N_ON_OFF].bind(false);
        mAnmGroups[MAP_MAIN_ANIM_N_ON_OFF].setToEnd();
    } else {
        mAnmGroups[MAP_MAIN_ANIM_N_ON_OFF].bind(false);
        mAnmGroups[MAP_MAIN_ANIM_TITLE_ON_OFF].bind(false);
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
            mAnmGroups[MAP_MAIN_ANIM_N_ON_OFF].setFrame(0.0f);
            mAnmGroups[MAP_MAIN_ANIM_TITLE_ON_OFF].setToEnd();
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD ||
                   mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE ||
                   mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) {
            mAnmGroups[MAP_MAIN_ANIM_N_ON_OFF].setToEnd();
            mAnmGroups[MAP_MAIN_ANIM_TITLE_ON_OFF].setFrame(0.0f);
        }
    }

    mAnmGroups[MAP_MAIN_ANIM_TITE_LINE].bind(false);
    mAnmGroups[MAP_MAIN_ANIM_TITE_LINE].setFrame(mNumTitleLines - 1);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnmGroups[MAP_MAIN_ANIM_TITE_LINE].unbind();
    if (mAnmGroups[MAP_MAIN_ANIM_N_ON_OFF].isBound()) {
        mAnmGroups[MAP_MAIN_ANIM_N_ON_OFF].unbind();
    }
    if (mAnmGroups[MAP_MAIN_ANIM_TITLE_ON_OFF].isBound()) {
        mAnmGroups[MAP_MAIN_ANIM_TITLE_ON_OFF].unbind();
    }
    if (grpZIn->isBound()) {
        grpZIn->unbind();
    }
    if (grpZAIn->isBound()) {
        grpZAIn->unbind();
    }
    if (grpZWakuIn->isBound()) {
        grpZWakuIn->unbind();
    }
    mAnmGroups[MAP_MAIN_ANIM_ROTATE].unbind();
    mpAllPane->SetVisible(false);
    if (mAnmGroups[MAP_MAIN_ANIM_KUMO_PATTERN].isBound()) {
        mAnmGroups[MAP_MAIN_ANIM_KUMO_PATTERN].unbind();
    }
    getGlobal()->setAlpha(0);
    // we had JUST set this to invisible...
    mpAllPane->SetVisible(true);
    mpRegionPane1 = nullptr;
    for (int i = 0; i < MAP_MAIN_NUM_AREA_PICS; i++) {
        nw4r::lyt::Pane *p1 = mLyt.findPane(sAreaPicNamesL[i]);
        nw4r::lyt::Pane *p2 = mLyt.findPane(sAreaPicNamesLink[i]);
        if (i == currentRegion) {
            p1->SetVisible(true);
            p2->SetVisible(true);
            mpRegionPane1 = p1;
            // TODO - was this meant to be p2?
            mpRegionPane2 = p1;
        } else {
            p1->SetVisible(false);
            p2->SetVisible(false);
        }
    }

    if (mRoomType == ROOMTYPE_SKYFIELD || mRoomType == ROOMTYPE_SKYFIELD_INTERIOR) {
        mLyt.findPane("P_skyloftLink_00")->SetVisible(false);
    }

    if (canPlaceBeacons(mCurrentMapMode) && canPlaceBeacons(mNextMapMode)) {
        mPutIcon.setVisible(true);
        mpNoroshiPane->SetVisible(true);
    } else {
        mpNoroshiPane->SetVisible(false);
        mPutIcon.setVisible(false);
    }

    if (mRoomType == ROOMTYPE_SKYFIELD) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE && mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            mMarkers.setIslandNamesOn(mShowIslandNames);
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM && mNextMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
            mMarkers.setIslandNamesOn(false);
        }
    }
}

void dLytMapMain_c::initializeState_In() {
    if (mMapEvent == MAP_EVENT_SAVE_OBJ) {
        displaySaveObjs();
        if (mMapEvent == MAP_EVENT_SAVE_OBJ) {
            mPopupInfo.mStateMgr.changeState(dLytMapPopupInfo_c::StateID_In);
        }
    }

    if (mDoCameraTransition || mMapEvent == MAP_EVENT_MAP_INTRO) {
        mpInOutAnmGroup = &mAnmGroups[MAP_MAIN_ANIM_IN];
    } else {
        mpInOutAnmGroup = &mAnmGroups[MAP_MAIN_ANIM_IN_NO_CAM];
    }

    field_0x8C88 = mpInOutAnmGroup->getLastFrame();
    mpInOutAnmGroup->bind(false);
    mpInOutAnmGroup->setFrame(0.0f);
    mPinIconAggregate.setScale(getGlobal()->getZoomFrame());
}
void dLytMapMain_c::executeState_In() {
    mIsVisible = 1;

    dLytMapGlobal_c *global = getGlobal();
    if ((!mDoCameraTransition || !field_0x8C84) && mDoCameraTransition) {
        return;
    }

    f32 ratio = (mpInOutAnmGroup->getFrame() / mpInOutAnmGroup->getLastFrame());

    f32 f1 = fn_80142D90(mCurrentMapMode) * 2.0f;
    f32 f2 = fn_80142D90(mCurrentMapMode);
    f32 f3 = cLib::easeOut(ratio, 3.0f);
    global->setField_0x44(f1 + f3 * (f2 - f1));

    mVec3_c v1;
    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE ||
        mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) {
        fn_80142F00(v1, mCurrentMapMode, true, global->getMapRotationCenter(), global->getField_0x56());
    } else {
        fn_80142F00(v1, mCurrentMapMode, mMapUpDirection, global->getMapRotationCenter(), global->getField_0x56());
    }
    global->setMapScroll(v1);
    if (mpInOutAnmGroup->isEndReached()) {
        dCsGame_c::GetInstance()->setPriorityDraw(0x8A);
        mLyt.getLayout()->Animate(0);
        mLyt.calc();
        mAnmGroups[MAP_MAIN_ANIM_IN].unbind();
        mpInOutAnmGroup->unbind();
        for (int i = 0; i < (int)ARRAY_LENGTH(mHitChecks); i++) {
            mHitChecks[i].resetCachedHitboxes();
            mHitChecks[i].execute();
        }

        switch (mMapEvent) {
            default: {
                mFloorBtnMgr.mStateMgr.changeState(dLytMapFloorBtnMgr_c::StateID_Wait);
                mStateMgr.changeState(StateID_Active);
                break;
            }
            case MAP_EVENT_SW_BANK_SMALL: {
                mStateMgr.changeState(StateID_EventSwBankSmall_Step1);
                break;
            }
            case MAP_EVENT_MAP_INTRO: {
                mStateMgr.changeState(StateID_EventMapIntro_Step1);
                break;
            }
            case MAP_EVENT_DUNGEON_MAP_GET: {
                mStateMgr.changeState(StateID_EventDungeonMapGet_Step1);
                break;
            }
            case MAP_EVENT_FIELD_MAP_CHANGE_5:
            case MAP_EVENT_FIELD_MAP_CHANGE_8: {
                mStateMgr.changeState(StateID_EventFieldMapChange_Step1);
                break;
            }
            case MAP_EVENT_FOREST_MAP_CHANGE: {
                mStateMgr.changeState(StateID_EventForestMapChange_Step1);
                break;
            }
            case MAP_EVENT_SIGNAL_ADD: {
                mStateMgr.changeState(StateID_EventSignalAdd_Step1);
                break;
            }
            case MAP_EVENT_GODDESS_CUBE: {
                mStateMgr.changeState(StateID_EventGoddessCube_Step1);
                break;
            }
            case MAP_EVENT_SAVE_OBJ: {
                displaySaveObjs();
                mStateMgr.changeState(StateID_EventSaveObjMsgWindow);
                break;
            }
        }
    }

    if (mpInOutAnmGroup->isBound()) {
        mpInOutAnmGroup->play();
    }
}
void dLytMapMain_c::finalizeState_In() {
    if (fn_80141530()) {
        dBase_c::s_NextExecuteControlFlags |= dBase_c::BASE_PROP_0x10;
    }
    dBase_c::s_DrawControlFlags |= dBase_c::BASE_PROP_0x10;
}

s32 dLytMapMain_c::getAreaGroup(s32 stifArea) const {
    s32 ret = AREAGROUP_MAX;
    if (mMapEvent == MAP_EVENT_SAVE_OBJ) {
        switch (mMapEventArg1) {
            case SURFACE_PROVINCE_FARON:   ret = AREAGROUP_FARON; break;
            case SURFACE_PROVINCE_ELDIN:   ret = AREAGROUP_ELDIN; break;
            case SURFACE_PROVINCE_LANAYRU: ret = AREAGROUP_LANAYRU; break;
        }
    } else {
        switch (stifArea) {
            case dStageMgr_c::STIF_AREA_SKY:              ret = AREAGROUP_SKY; break;
            case dStageMgr_c::STIF_AREA_SEALED_GROUNDS:   ret = AREAGROUP_SEALED_GROUNDS; break;
            case dStageMgr_c::STIF_AREA_FARON_WOODS:
            case dStageMgr_c::STIF_AREA_DEEP_WOODS:
            case dStageMgr_c::STIF_AREA_LAKE_FLORIA:      ret = AREAGROUP_FARON; break;
            case dStageMgr_c::STIF_AREA_VOLCANO_SOUTH:
            case dStageMgr_c::STIF_AREA_VOLCANO_NORTH:    ret = AREAGROUP_ELDIN; break;
            case dStageMgr_c::STIF_AREA_LANAYRU_MINE:
            case dStageMgr_c::STIF_AREA_LANAYRU_DESERT:
            case dStageMgr_c::STIF_AREA_LANAYRU_SAND_SEA:
            case dStageMgr_c::STIF_AREA_LANAYRU_CAVES:
            case dStageMgr_c::STIF_AREA_LANAYRU_GORGE:    ret = AREAGROUP_LANAYRU; break;
        }
    }

    return ret;
}

s32 dLytMapMain_c::getRoomType() const {
    s32 ret = ROOMTYPE_FIELD;
    s32 stifRoomType = dStageMgr_c::GetInstance()->getSTIFRoomType();
    if (mMapEvent == MAP_EVENT_SAVE_OBJ) {
        ret = ROOMTYPE_FIELD;
    }

    if (dScGame_c::isCurrentStage("F011r") ||
        (dScGame_c::isCurrentStage("F002r") && dScGame_c::currentSpawnInfo.isNight()) ||
        dScGame_c::isCurrentStage("F010r") || dScGame_c::isCurrentStage("F019r")) {
        ret = ROOMTYPE_SKYFIELD_INTERIOR;
    } else if (stifRoomType == dStageMgr_c::STIF_ROOMTYPE_FIELD) {
        ret = ROOMTYPE_FIELD;
    } else if (stifRoomType == dStageMgr_c::STIF_ROOMTYPE_DUNGEON) {
        ret = ROOMTYPE_DUNGEON;
    } else if (stifRoomType == dStageMgr_c::STIF_ROOMTYPE_SKYLOFT) {
        ret = ROOMTYPE_SKYLOFT;
    } else if (stifRoomType == dStageMgr_c::STIF_ROOMTYPE_BOSS_HOUSE) {
        ret = ROOMTYPE_BOSS_HOUSE;
    } else if (stifRoomType == dStageMgr_c::STIF_ROOMTYPE_SKYFIELD) {
        ret = ROOMTYPE_SKYFIELD;
    }
    return ret;
}

void dLytMapMain_c::zoomIn() {
    if (mRoomType == ROOMTYPE_SKYLOFT ||
        (mAreaGroup == AREAGROUP_SKY && (mRoomType == ROOMTYPE_BOSS_HOUSE || mRoomType == ROOMTYPE_DUNGEON))) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD) {
            if (dPad::getDownTrigRight()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_AREA);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_WORLD_SKY;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) {
            if (dPad::getDownTrigRight()) {
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_COURSE);
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            if (dPad::getDownTrigRight() || dPad::getDownTrigA()) {
                mNextMapUpDirection = mMapUpDirectionAfterZoomToDetail;
                mNextMapMode = dLytMapGlobal_c::MAPMODE_ZOOM;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_DETAIL);

                if (mPointerOnMap && (mNumFloors < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_CsrRot);
                } else {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
                }
            }
        }
    } else if (mRoomType == ROOMTYPE_SKYFIELD || mRoomType == ROOMTYPE_SKYFIELD_INTERIOR) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD) {
            if (dPad::getDownTrigRight()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_COURSE);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
            if (dPad::getDownTrigRight()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_COURSE);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            if (dPad::getDownTrigRight() || dPad::getDownTrigA()) {
                mNextMapUpDirection = mMapUpDirectionAfterZoomToDetail;
                mNextMapMode = dLytMapGlobal_c::MAPMODE_ZOOM;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_DETAIL);

                if (mPointerOnMap && (mNumFloors < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_CsrRot);
                } else {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
                }
            }
        }
    } else if (mRoomType == ROOMTYPE_FIELD) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD) {
            if (dPad::getDownTrigRight()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_AREA);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_PROVINCE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
            if (dPad::getDownTrigRight()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_COURSE);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            if (dPad::getDownTrigRight() || dPad::getDownTrigA()) {
                mNextMapUpDirection = mMapUpDirectionAfterZoomToDetail;
                mNextMapMode = dLytMapGlobal_c::MAPMODE_ZOOM;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_DETAIL);

                if (mPointerOnMap && (mNumFloors < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_CsrRot);
                } else {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
                }
            }
        }
    } else if (mRoomType == ROOMTYPE_DUNGEON) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD) {
            if (dPad::getDownTrigRight()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_AREA);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_PROVINCE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
            if (dPad::getDownTrigRight()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_COURSE);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            if (dPad::getDownTrigRight() || (!mFloorBtnMgr.hasPointedAtABtnIdx() && dPad::getDownTrigA())) {
                mNextMapUpDirection = mMapUpDirectionAfterZoomToDetail;
                mNextMapMode = dLytMapGlobal_c::MAPMODE_ZOOM;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_DETAIL);

                if (mPointerOnMap && (mNumFloors < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_CsrRot);
                } else {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
                }
            }
        }
    } else if (mRoomType == ROOMTYPE_BOSS_HOUSE) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD) {
            if (dPad::getDownTrigRight()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_AREA);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_PROVINCE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
            if (dPad::getDownTrigRight()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_COURSE);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        }
    }
}

void dLytMapMain_c::zoomOut() {
    if (mRoomType == ROOMTYPE_SKYLOFT ||
        (mAreaGroup == AREAGROUP_SKY && (mRoomType == ROOMTYPE_BOSS_HOUSE || mRoomType == ROOMTYPE_DUNGEON))) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) {
            if (dPad::getDownTrigLeft()) {
                mNextMapMode = dLytMapGlobal_c::MAPMODE_WORLD;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_WORLD);
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            if (dPad::getDownTrigLeft()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_AREA);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_WORLD_SKY;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
            if (dPad::getDownTrigLeft() || dPad::getDownTrigA()) {
                mMapUpDirectionAfterZoomToDetail = mMapUpDirection;
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                mNextMapUpDirection = true;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_COURSE);

                mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
            }
        }
    } else if (mRoomType == ROOMTYPE_SKYFIELD || mRoomType == ROOMTYPE_SKYFIELD_INTERIOR) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
            if (dPad::getDownTrigLeft()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_WORLD);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_WORLD;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            if (dPad::getDownTrigLeft()) {
                mNextMapMode = dLytMapGlobal_c::MAPMODE_WORLD;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_WORLD);
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
            if (dPad::getDownTrigLeft() || dPad::getDownTrigA()) {
                mMapUpDirectionAfterZoomToDetail = mMapUpDirection;
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                mNextMapUpDirection = true;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_COURSE);

                mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
            }
        }
    } else if (mRoomType == ROOMTYPE_FIELD) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
            if (dPad::getDownTrigLeft()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_WORLD);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_WORLD;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            if (dPad::getDownTrigLeft()) {
                mNextMapMode = dLytMapGlobal_c::MAPMODE_PROVINCE;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_AREA);
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
            if (dPad::getDownTrigLeft() || dPad::getDownTrigA()) {
                mMapUpDirectionAfterZoomToDetail = mMapUpDirection;
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                mNextMapUpDirection = true;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_COURSE);

                mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
            }
        }
    } else if (mRoomType == ROOMTYPE_DUNGEON) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
            if (dPad::getDownTrigLeft()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_WORLD);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_WORLD;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            if (dPad::getDownTrigLeft()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_AREA);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_PROVINCE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
            if (dPad::getDownTrigLeft() || (!mFloorBtnMgr.hasPointedAtABtnIdx() && dPad::getDownTrigA())) {
                mMapUpDirectionAfterZoomToDetail = mMapUpDirection;
                mNextMapMode = dLytMapGlobal_c::MAPMODE_STAGE;
                mNextMapUpDirection = true;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_COURSE);

                mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
            }
        }
    } else if (mRoomType == ROOMTYPE_BOSS_HOUSE) {
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) {
            if (dPad::getDownTrigLeft()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_WORLD);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_WORLD;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        } else if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            if (dPad::getDownTrigLeft()) {
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMOUT_TO_AREA);
                mNextMapMode = dLytMapGlobal_c::MAPMODE_PROVINCE;
                mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
            }
        }
    }
}

void dLytMapMain_c::initializeState_Active() {}
void dLytMapMain_c::executeState_Active() {
    dLytMapGlobal_c *global = getGlobal();
    if (dMessage_c::getInstance()->getInMapEvent()) {
        return;
    }

    if (mRoomType == ROOMTYPE_SKYFIELD && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE &&
        mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE && dPad::getDownTrigZ()) {
        if (mShowIslandNames) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ISLAND_NAME_OFF);
        } else {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ISLAND_NAME_ON);
        }
        mShowIslandNames = !mShowIslandNames;
        mMarkers.setIslandNamesOn(mShowIslandNames);
    }
    checkScroll();

    if (needsNav(mCurrentMapMode) && needsNav(mNextMapMode)) {
        if (!mNavEnabled) {
            dPadNav::setNavEnabled(true, false);
            mNavEnabled = true;
        }
        if (mFloorBtnMgr.canDecideFloor() && dPad::getDownTrigA()) {
            mFloorBtnMgr.decideSelectedFloor();
            field_0x8C64 = mFloorBtnMgr.getCurrentFloor();
            global->setFloor(field_0x8C64);
            mStateMgr.changeState(StateID_FloorChange);
        }
    } else {
        if (mNavEnabled) {
            dPadNav::setNavEnabled(false, false);
            mNavEnabled = false;
        }
    }

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE && mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE &&
        canZoomIn(dLytMapGlobal_c::MAPMODE_STAGE)) {
        if (mPointerOnMap && (mNumFloors < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
            // Can zoom in around cursor, use the cursor pos
            getGlobal()->unprojectFromMap(field_0x8CF4, dCsGame_c::GetInstance()->getCursorIf()->getCursorPos());
            f32 tmp = fn_80142D90(dLytMapGlobal_c::MAPMODE_ZOOM);
            fn_8013FB70(field_0x8CF4, getGlobal()->getField_0x44() / tmp);
            mDrawScaleFrame = true;
        } else {
            // Cannot zoom in around cursor, use the player pos
            f32 tmp = fn_80142D90(dLytMapGlobal_c::MAPMODE_ZOOM);
            f32 val = getGlobal()->getField_0x44() / tmp;
            fn_8013FB70(getGlobal()->getPlayerPos(), val);
            mDrawScaleFrame = true;
        }
    }

    if (field_0x8DBD && *mPinIconAggregate.mStateMgr.getStateID() != dLytMapPinIconAggregate_c::StateID_Select &&
        dPad::getDownTrigC() && mPointerCanPlaceBeacon) {
        s32 id = -1;
        mPinIconAggregate.findNewBeaconId(&id);
        if (id >= 0) {
            mVec3_c pos3d;
            dLytMapGlobal_c *global = getGlobal();
            global->unprojectFromMap(pos3d, dCsGame_c::GetInstance()->getCursorIf()->getCursorPos());
            mPinIconAggregate.setPosition(id, pos3d);
            setBeaconPositionChecked(&pos3d, id);
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_BEACON_SET);
        }
    } else if ((dPad::getDownTrigLeft() && canZoomOut(mCurrentMapMode)) ||
               (dPad::getDownTrigA() && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM &&
                canZoomOut(mCurrentMapMode))) {
        zoomOut();
    } else if ((dPad::getDownTrigRight() && canZoomIn(mCurrentMapMode)) ||
               (dPad::getDownTrigA() && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE &&
                canZoomIn(mCurrentMapMode))) {
        zoomIn();
    } else if ((mRoomType == ROOMTYPE_FIELD || mRoomType == ROOMTYPE_DUNGEON || mRoomType == ROOMTYPE_SKYLOFT ||
                mRoomType == ROOMTYPE_SKYFIELD) &&
               canResetPosition(mCurrentMapMode, mMapUpDirection) && dPad::getDownTrigUp()) {
        mVec3_c pos;
        fn_80142F00(pos, mCurrentMapMode, mMapUpDirection, global->getMapRotationCenter(), global->getField_0x56());
        f32 distSq = global->getMapScroll().squareDistanceToXZ(pos);
        if (mCurrentFloor != global->getFloor()) {
            mFloorBtnMgr.resetFloor(-(mCurrentFloor - mBaseFloorOffset));
            global->setFloor(mCurrentFloor);
            mStateMgr.changeState(StateID_ResetPosWithFloorChange);
        } else if (distSq >= 1.0f) {
            mStateMgr.changeState(StateID_ResetPos);
        } else {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_RESET_NOT_MOVE);
        }
    } else if (canChangeUpDirection(mCurrentMapMode, mMapUpDirection) && dPad::getDownTrigZ()) {
        mStateMgr.changeState(StateID_ChgMapUpDirection);
    }

    if ((mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE && mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) ||
        (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM && mNextMapMode == dLytMapGlobal_c::MAPMODE_ZOOM)) {
        field_0xF1C.field_0x05 = true;
    }

    mPinIconAggregate.execute();

    for (int idx = 0; idx < (int)ARRAY_LENGTH(mHitChecks); idx++) {
        mHitChecks[idx].resetCachedHitboxes();
        mHitChecks[idx].execute();
    }
}
void dLytMapMain_c::finalizeState_Active() {
    mDrawScaleFrame = false;
}

void dLytMapMain_c::initializeState_Out() {
    if (mMapEvent == MAP_EVENT_SAVE_OBJ) {
        mPopupInfo.mStateMgr.changeState(StateID_Out);
    }
    dLytMeter_c::GetInstance()->setMeterMode(dLytMeterMain_c::MODE_MAP_INIT);
    if (mDoCameraTransition || mMapEvent == MAP_EVENT_MAP_INTRO) {
        mpInOutAnmGroup = &mAnmGroups[MAP_MAIN_ANIM_OUT];
    } else {
        mpInOutAnmGroup = &mAnmGroups[MAP_MAIN_ANIM_OUT_NO_CAM];
    }

    mpInOutAnmGroup->bind(false);
    mpInOutAnmGroup->setFrame(0.0f);
    field_0x8C8C = mpInOutAnmGroup->getLastFrame();

    if (fn_80141530()) {
        dBase_c::s_NextExecuteControlFlags &= ~dBase_c::BASE_PROP_0x10;
    }

    dPadNav::setNavEnabled(false, false);
}
void dLytMapMain_c::executeState_Out() {
    if (mpInOutAnmGroup->isEndReached()) {
        bool specialMode = isMapEventEq0Or1Or7Or9Or11();
        // Kind of a weird way to write `specialMode || ... != 3 `
        if ((specialMode && dScGame_c::getCamera(0)->getField_0xDA8() != 3) || !specialMode) {
            dBase_c::s_NextExecuteControlFlags &= ~dBase_c::BASE_PROP_0x10;
            mLyt.calc();
            mpInOutAnmGroup->unbind();
            mIsVisible = 0;
            mStateMgr.changeState(StateID_Invisible);
        }
    }

    if (mpInOutAnmGroup->isBound()) {
        mpInOutAnmGroup->play();
    }
}
void dLytMapMain_c::finalizeState_Out() {}

void dLytMapMain_c::initializeState_FloorChange() {
    mMapCapture.renderRequest();
}
void dLytMapMain_c::executeState_FloorChange() {
    if (!mMapCapture.isBusyRendering()) {
        mStateMgr.changeState(StateID_Active);
        dPadNav::stopFSStickNav();
    }
}
void dLytMapMain_c::finalizeState_FloorChange() {}

void dLytMapMain_c::initializeState_ChgMapUpDirection() {
    mNextMapUpDirection = !mMapUpDirection;
    field_0x8D44 = getGlobal()->getMapRotation();

    if (mMapEvent == MAP_EVENT_MAP_INTRO) {
        field_0x8D46 = dAcPy_c::GetLink()->mRotation.y + mAng(0x8000);
        mNextMapUpDirection = false;
    } else {
        fn_80142D10(mCurrentMapMode, mNextMapUpDirection, field_0x8D46);
    }
    (void)getGlobal();
    dLytMapGlobal_c *global = getGlobal();
    mPlayerPos = global->getPlayerPos();
    mVec2_c v(0.0f, 0.0f);
    global->unprojectFromMap(mMapScroll, v, field_0x8D44);
    global->setMapRotationCenter(mPlayerPos);
    const mVec3_c &center = global->getMapRotationCenter();
    fn_80143060(mPlayerPos, mPlayerPos, center, field_0x8D46);
    fn_80143060(mMapScroll, mMapScroll, center, field_0x8D44);
    global->setMapScroll(mMapScroll);
    field_0x8C70 = 0;
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_CHANGE_DIRECT);
}
void dLytMapMain_c::executeState_ChgMapUpDirection() {
    if (mMapEvent == MAP_EVENT_MAP_INTRO && mEventCanceled) {
        forceOut();
    } else {
        dLytMapGlobal_c *global = getGlobal();
        f32 factor = cLib::easeOut((f32)field_0x8C70 / sHio.field_0x1B, 2.0f);

        // Lerps

        // TODO stack and reg swaps
        global->setMapRotation(factor * mAng(field_0x8D46 - field_0x8D44) + field_0x8D44);

        mVec3_c actualPos = (mPlayerPos - mMapScroll) * factor + mMapScroll;
        global->setMapScroll(actualPos);

        if (field_0x8C70 >= sHio.field_0x1B) {
            mLyt.calc();
            mMapUpDirection = mNextMapUpDirection;
            if (mMapEvent == MAP_EVENT_MAP_INTRO) {
                mStateMgr.changeState(StateID_EventMapIntro_Step4);
            } else {
                mStateMgr.changeState(StateID_Active);
            }
        } else {
            field_0x8C70++;
        }

        if (mMapEvent == MAP_EVENT_MAP_INTRO) {
            EventManager::execute();
        }
    }
}
void dLytMapMain_c::finalizeState_ChgMapUpDirection() {}

void dLytMapMain_c::initializeState_ChgDispAreaMode_MapRot() {
    dLytMapGlobal_c *global = getGlobal();
    mMapScroll = global->getMapScroll();
    field_0x8D50 = global->getField_0x44();
    field_0x8D44 = global->getMapRotation();

    field_0x8D4C = fn_80142D90(mNextMapMode);
    if (mMapEvent == MAP_EVENT_MAP_INTRO) {
        field_0x8D46 = dAcPy_c::GetLink()->mRotation.y + mAng(0x8000);
        mNextMapUpDirection = false;
    } else {
        mNextMapUpDirection = fn_80142D10(mNextMapMode, mNextMapUpDirection, field_0x8D46);
    }

    fn_80142F00(mPlayerPos, mNextMapMode, mNextMapUpDirection, global->getMapRotationCenter(), global->getField_0x56());
    field_0x8C70 = 0;

    if (mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
        global->setZoomFrame(1.0f);
        global->setField_0x58(field_0x8D50 / field_0x8D4C);
    } else {
        global->setZoomFrame(0.0f);
        global->setField_0x58(1.0f);
    }

    if (mNextMapMode == dLytMapGlobal_c::MAPMODE_ZOOM && !mNextMapUpDirection) {
        mVec2_c v(0.0f, 0.0f);
        global->unprojectFromMap(mMapScroll, v, field_0x8D44);
        mVec3_c v2 = mPlayerPos;
        global->setMapRotationCenter(v2);
        dLytMapGlobal_c *global2 = getGlobal();
        const mVec3_c &center = global2->getMapRotationCenter();
        fn_80143060(mPlayerPos, v2, center, field_0x8D46);
        mVec3_c v3 = mMapScroll;
        fn_80143060(mMapScroll, v3, center, field_0x8D44);
        mVec3_c v4 = mMapScroll;
        global2->setMapScroll(v4);
    } else {
        mVec2_c v(0.0f, 0.0f);
        global->unprojectFromMap(mMapScroll, v, field_0x8D44);
        if (mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            mPlayerPos = field_0x8CC4;
        } else {
            mPlayerPos = global->getPlayerPos();
        }

        global->setMapRotationCenter(mMapScroll);
        mVec3_c v2 = mPlayerPos;
        dLytMapGlobal_c *global2 = getGlobal();
        const mVec3_c &center = global2->getMapRotationCenter();
        fn_80143060(mPlayerPos, v2, center, field_0x8D46);
        mVec3_c v3 = mMapScroll;
        fn_80143060(mMapScroll, v3, center, field_0x8D44);
        mVec3_c v4 = mMapScroll;
        global2->setMapScroll(v4);
    }

    mAnmGroups[MAP_MAIN_ANIM_ROTATE].bind(false);
    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mAnmGroups[MAP_MAIN_ANIM_ROTATE].setFrame(0.0f);
    } else {
        mAnmGroups[MAP_MAIN_ANIM_ROTATE].setToEnd();
    }
    if (mNextMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mMarkers.setIslandNamesOn(false);
    }
    if (mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
        mMarkers.setIslandNamesOn(mShowIslandNames);
    }

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM && mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
        mVec2_c v(global->getField_0x20().x, global->getField_0x20().y);
        global->unprojectFromMap(field_0x8CF4, v);
    }
}
void dLytMapMain_c::executeState_ChgDispAreaMode_MapRot() {
    if (mMapEvent == MAP_EVENT_MAP_INTRO && mEventCanceled) {
        forceOut();
    } else {
        dLytMapGlobal_c *global = getGlobal();
        f32 f1 = (f32)field_0x8C70 / sHio.field_0x1B;
        f32 factor = cLib::easeInOut(f1, 2.0f);

        f32 f2, f3;
        if (mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            f32 ratio = field_0x8D50 / field_0x8D4C;
            f1 = 1.0f - f1;
            f2 = ratio + (1.0f - ratio) * factor;
            f3 = 1.0f - factor;
        } else {
            f32 ratio = field_0x8D4C / field_0x8D50;
            f2 = ratio + (1.0f - ratio) * (1.0f - factor);
            f3 = factor;
        }
        global->setZoomFrame(f1);
        global->setField_0x58(f2);

        // TODO - there is tons of dead code here

        // Lerps
        f32 f4 = field_0x8D50 + factor * (field_0x8D4C - field_0x8D50);
        global->setField_0x44(f4);

        // TODO stack and reg swaps
        global->setMapRotation(factor * mAng(field_0x8D46 - field_0x8D44) + field_0x8D44);

        mVec3_c actualPos;
        if (mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            actualPos = (mPlayerPos - mMapScroll) * (1.0f - factor) * (1.0f / (f4 / field_0x8D50)) + mMapScroll;
        } else {
            actualPos = (mPlayerPos - mMapScroll) * (1.0f - factor) * (1.0f / (f4 / field_0x8D50)) + mMapScroll;
        }

        global->setMapScroll(actualPos);

        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM && mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
            fn_8013FB70(field_0x8CF4, f4 / field_0x8D50);
            mDrawScaleFrame = true;
        }

        // TODO dead code
        f32 length = mAnmGroups[MAP_MAIN_ANIM_ROTATE].getLastFrame();
        mAnmGroups[MAP_MAIN_ANIM_ROTATE].setFrame(length - f3 * length);

        if (field_0x8C70 >= sHio.field_0x1C) {
            if (mMapEvent == MAP_EVENT_MAP_INTRO && field_0x0108 > 0) {
                field_0x0108--;
            } else {
                mDrawScaleFrame = false;
                if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
                    mAnmGroups[MAP_MAIN_ANIM_ROTATE].setToEnd();
                } else {
                    mAnmGroups[MAP_MAIN_ANIM_ROTATE].setFrame(0.0f);
                }
                mLyt.calc();
                mAnmGroups[MAP_MAIN_ANIM_ROTATE].unbind();
                mDrawScaleFrame = false;
                mMapUpDirection = mNextMapUpDirection;
                global->setMapRotation(field_0x8D46);
                global->setField_0x44(field_0x8D4C);
                global->setMapScroll(mPlayerPos);

                if (mMapEvent == MAP_EVENT_MAP_INTRO) {
                    mStateMgr.changeState(StateID_EventMapIntro_Step4);
                } else {
                    mStateMgr.changeState(StateID_Active);
                    mCurrentMapMode = mNextMapMode;
                }
            }
        } else {
            field_0x8C70++;
        }
        if (mMapEvent == MAP_EVENT_MAP_INTRO) {
            EventManager::execute();
        }
    }
}
void dLytMapMain_c::finalizeState_ChgDispAreaMode_MapRot() {}

void dLytMapMain_c::initializeState_ChgDispAreaMode_Map() {}
void dLytMapMain_c::executeState_ChgDispAreaMode_Map() {
    // Won't attempt this until the dead code in executeState_ChgDispAreaMode_MapRot is solved
}
void dLytMapMain_c::finalizeState_ChgDispAreaMode_Map() {}

void dLytMapMain_c::initializeState_ChgDispAreaMode_WA() {}
void dLytMapMain_c::executeState_ChgDispAreaMode_WA() {}
void dLytMapMain_c::finalizeState_ChgDispAreaMode_WA() {}

void dLytMapMain_c::initializeState_ChgDispAreaMode_CsrRot() {}
void dLytMapMain_c::executeState_ChgDispAreaMode_CsrRot() {}
void dLytMapMain_c::finalizeState_ChgDispAreaMode_CsrRot() {}

void dLytMapMain_c::initializeState_ResetPos() {
    dLytMapGlobal_c *global = getGlobal();
    field_0x8C70 = 0;
    mMapScroll = global->getMapScroll();
    fn_80142F00(mPlayerPos, mCurrentMapMode, mMapUpDirection, global->getMapRotationCenter(), global->getField_0x56());
    field_0x8C70 = 0;
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_RESET_START);
}
void dLytMapMain_c::executeState_ResetPos() {
    dLytMapGlobal_c *global = getGlobal();
    f32 factor = cLib::easeOut((f32)field_0x8C70 / sHio.field_0x1A, 2.0f);
    mVec3_c pos = (mPlayerPos - mMapScroll) * factor + mMapScroll;
    global->setMapScroll(pos);
    dSndSmallEffectMgr_c::GetInstance()->holdSound(SE_S_MAP_RESET_LV);
    if (field_0x8C70 >= sHio.field_0x1A) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_RESET_END);
        mStateMgr.changeState(StateID_Active);
    } else {
        field_0x8C70++;
    }
}
void dLytMapMain_c::finalizeState_ResetPos() {}

void dLytMapMain_c::initializeState_ResetPosWithFloorChange() {
    dPadNav::stopFSStickNav();
    mMapCapture.renderRequest();
}
void dLytMapMain_c::executeState_ResetPosWithFloorChange() {
    if (mMapCapture.isBusyRendering()) {
        return;
    }
    dLytMapGlobal_c *global = getGlobal();
    mVec3_c pos;
    fn_80142F00(pos, mCurrentMapMode, mMapUpDirection, global->getMapRotationCenter(), global->getField_0x56());
    if (global->getMapScroll().squareDistanceToXZ(pos) >= 1.0f) {
        mStateMgr.changeState(StateID_ResetPos);
    } else {
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMapMain_c::finalizeState_ResetPosWithFloorChange() {}

void dLytMapMain_c::initializeState_EventSwBankSmall_Step1() {
    mEventTimer = sHio.field_0x1D;
    field_0x8CB4 = 0;
}
void dLytMapMain_c::executeState_EventSwBankSmall_Step1() {
    if (mEventTimer != 0) {
        mEventTimer--;
        return;
    }

    mMarkers.setField_0x0703(1);
    mStateMgr.changeState(StateID_EventSwBankSmall_Step2);
}
void dLytMapMain_c::finalizeState_EventSwBankSmall_Step1() {}

void dLytMapMain_c::initializeState_EventSwBankSmall_Step2() {
    mEventTimer = mMarkers.getField_0x0704();
}
void dLytMapMain_c::executeState_EventSwBankSmall_Step2() {
    if (mEventTimer != 0) {
        mEventTimer--;
        return;
    }
    mStateMgr.changeState(StateID_EventSwBankSmall_Step3);
}
void dLytMapMain_c::finalizeState_EventSwBankSmall_Step2() {}

void dLytMapMain_c::initializeState_EventSwBankSmall_Step3() {
    mEventTimer = sHio.field_0x1E;
}
void dLytMapMain_c::executeState_EventSwBankSmall_Step3() {
    if (mEventTimer != 0) {
        mEventTimer--;
        return;
    }

    mEventDone = true;
}
void dLytMapMain_c::finalizeState_EventSwBankSmall_Step3() {}

void dLytMapMain_c::initializeState_EventMapIntro_Step1() {
    mEventTimer = sHio.field_0x1F;
}
void dLytMapMain_c::executeState_EventMapIntro_Step1() {
    if (mEventCanceled) {
        forceOut();
        return;
    }

    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        mStateMgr.changeState(StateID_EventMapIntro_Step2);
    }
    EventManager::execute();
}
void dLytMapMain_c::finalizeState_EventMapIntro_Step1() {}

void dLytMapMain_c::initializeState_EventMapIntro_Step2() {
    if (field_0x8880[field_0x88AC] != nullptr) {
        field_0x8880[field_0x88AC]->bind(false);
        field_0x8880[field_0x88AC]->setFrame(0.0f);
        mLyt.calc();
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_WORLD_EXTEND);
    }
}
void dLytMapMain_c::executeState_EventMapIntro_Step2() {
    if (mEventCanceled) {
        forceOut();
        return;
    }

    if (field_0x8880[field_0x88AC] != nullptr) {
        if (field_0x8880[field_0x88AC]->isEndReached()) {
            field_0x8880[field_0x88AC]->unbind();
            mStateMgr.changeState(StateID_EventMapIntro_Step3);
        } else {
            field_0x8880[field_0x88AC]->play();
        }
    } else {
        mStateMgr.changeState(StateID_EventMapIntro_Step3);
    }

    EventManager::execute();
}
void dLytMapMain_c::finalizeState_EventMapIntro_Step2() {}

void dLytMapMain_c::initializeState_EventMapIntro_Step3() {
    mEventTimer = 0;
}
void dLytMapMain_c::executeState_EventMapIntro_Step3() {
    if (mEventCanceled) {
        forceOut();
        return;
    }

    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_AREA);
        mNextMapMode = dLytMapGlobal_c::MAPMODE_PROVINCE;
        field_0x0108 = sHio.field_0x20;
        mStateMgr.changeState(StateID_ChgDispAreaMode_WA);
    }
    EventManager::execute();
}
void dLytMapMain_c::finalizeState_EventMapIntro_Step3() {}

void dLytMapMain_c::initializeState_EventMapIntro_Step4() {
    mEventTimer = sHio.field_0x21;
}
void dLytMapMain_c::executeState_EventMapIntro_Step4() {
    if (mEventCanceled) {
        forceOut();
        return;
    }

    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        mEventDone = true;
    }
    EventManager::execute();
}
void dLytMapMain_c::finalizeState_EventMapIntro_Step4() {}

void dLytMapMain_c::forceOut() {
    mAnmGroups[MAP_MAIN_ANIM_OUT].bind(false);
    mAnmGroups[MAP_MAIN_ANIM_OUT].setToEnd();
    mLyt.calc();
    mAnmGroups[MAP_MAIN_ANIM_OUT].unbind();
    dBase_c::s_NextExecuteControlFlags &= ~dBase_c::BASE_PROP_0x10;
    dBase_c::s_DrawControlFlags &= ~dBase_c::BASE_PROP_0x10;
    dScGame_c::getCamera(0)->doFn_80093360();
    dScGame_c::getCamera(0)->fn_8019E430();
    mStateMgr.changeState(StateID_Invisible);
}

void dLytMapMain_c::initializeState_EventDungeonMapGet_Step1() {
    mEventTimer = sHio.field_0x22;
}
void dLytMapMain_c::executeState_EventDungeonMapGet_Step1() {
    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        mStateMgr.changeState(StateID_EventDungeonMapGet_Step2);
    }
}
void dLytMapMain_c::finalizeState_EventDungeonMapGet_Step1() {}

void dLytMapMain_c::initializeState_EventDungeonMapGet_Step2() {
    mEventTimer = sHio.field_0x23;
    field_0x8CB4 = sHio.field_0x24;
    field_0x8CB8 = mMarkers.getField_0x0704();
    mMapChangeAlpha = 0;
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_EXTEND);
}
void dLytMapMain_c::executeState_EventDungeonMapGet_Step2() {
    dLytMapGlobal_c *global = getGlobal();
    if (mEventTimer != 0 || field_0x8CB4 != 0 || field_0x8CB8 != 0) {
        if (mEventTimer != 0) {
            mEventTimer--;
        }
        u8 alpha = (1.0f - (f32)mEventTimer / sHio.field_0x23) * 255.0f;
        global->setField_0x4F(alpha);
        mMapChangeAlpha = alpha;
        if (field_0x8CB4 != 0) {
            field_0x8CB4--;
        } else {
            mMarkers.setField_0x0702(1);
            if (field_0x8CB8 != 0) {
                field_0x8CB8--;
            }
        }
    } else {
        global->setField_0x4C(0);
        mStateMgr.changeState(StateID_EventDungeonMapGet_Step3);
    }
}
void dLytMapMain_c::finalizeState_EventDungeonMapGet_Step2() {}

void dLytMapMain_c::initializeState_EventDungeonMapGet_Step3() {
    mEventTimer = sHio.field_0x25;
}
void dLytMapMain_c::executeState_EventDungeonMapGet_Step3() {
    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        mEventDone = true;
    }
}
void dLytMapMain_c::finalizeState_EventDungeonMapGet_Step3() {}

void dLytMapMain_c::initializeState_EventFieldMapChange_Step1() {
    mEventTimer = sHio.field_0x26;
}
void dLytMapMain_c::executeState_EventFieldMapChange_Step1() {
    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        mStateMgr.changeState(StateID_EventFieldMapChange_Step2);
    }
}
void dLytMapMain_c::finalizeState_EventFieldMapChange_Step1() {}

void dLytMapMain_c::initializeState_EventFieldMapChange_Step2() {
    mEventTimer = sHio.field_0x27;
    mMapChangeAlpha = 0;
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_EXTEND);
}
void dLytMapMain_c::executeState_EventFieldMapChange_Step2() {
    dStage_c *stage = dStage_c::GetInstance();
    u32 duration = sHio.field_0x27;
    if (mEventTimer != 0) {
        u8 u1 = (f32)(duration - mEventTimer) / (f32)duration * 255.0f;
        stage->getMapRelated()->setField_0x1E8(u1);
        mEventTimer--;
    } else {
        stage->getMapRelated()->setField_0x1E8(0);
        stage->getMapRelated()->setField_0x1EF(1);
        mStateMgr.changeState(StateID_EventFieldMapChange_Step3);
    }
}
void dLytMapMain_c::finalizeState_EventFieldMapChange_Step2() {}

void dLytMapMain_c::initializeState_EventFieldMapChange_Step3() {
    mEventTimer = sHio.field_0x28;
}
void dLytMapMain_c::executeState_EventFieldMapChange_Step3() {
    if (mEventTimer != 0) {
        mEventTimer--;
    } else if (mMapEvent == MAP_EVENT_FIELD_MAP_CHANGE_8) {
        mStateMgr.changeState(StateID_Active);
    } else {
        mStateMgr.changeState(StateID_EventFieldMapChange_Step4);
    }
}
void dLytMapMain_c::finalizeState_EventFieldMapChange_Step3() {}

void dLytMapMain_c::initializeState_EventFieldMapChange_Step4() {}
void dLytMapMain_c::executeState_EventFieldMapChange_Step4() {}
void dLytMapMain_c::finalizeState_EventFieldMapChange_Step4() {}

void dLytMapMain_c::initializeState_EventForestMapChange_Step1() {
    mEventTimer = sHio.field_0x22;
}
void dLytMapMain_c::executeState_EventForestMapChange_Step1() {
    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        mStateMgr.changeState(StateID_EventForestMapChange_Step2);
    }
}
void dLytMapMain_c::finalizeState_EventForestMapChange_Step1() {}

void dLytMapMain_c::initializeState_EventForestMapChange_Step2() {
    mEventTimer = sHio.field_0x23;
    field_0x8CB4 = sHio.field_0x24;
    field_0x8CB8 = mMarkers.getField_0x0704();
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_EXTEND);
    mMapChangeAlpha = 0;
}
void dLytMapMain_c::executeState_EventForestMapChange_Step2() {
    dLytMapGlobal_c *global = getGlobal();
    if (mEventTimer != 0 || field_0x8CB4 != 0 || field_0x8CB8 != 0) {
        if (mEventTimer != 0) {
            mEventTimer--;
        }
        u8 alpha = (1.0f - (f32)mEventTimer / sHio.field_0x23) * 255.0f;
        global->setField_0x4F(alpha);
        mMapChangeAlpha = alpha;
        if (field_0x8CB4 != 0) {
            field_0x8CB4--;
        } else {
            mMarkers.setField_0x0702(1);
            if (field_0x8CB8 != 0) {
                field_0x8CB8--;
            }
        }
    } else {
        global->setField_0x4C(0);
        mStateMgr.changeState(StateID_EventForestMapChange_Step3);
    }
}
void dLytMapMain_c::finalizeState_EventForestMapChange_Step2() {}

void dLytMapMain_c::initializeState_EventForestMapChange_Step3() {
    mEventTimer = sHio.field_0x25;
}
void dLytMapMain_c::executeState_EventForestMapChange_Step3() {
    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        mEventDone = true;
    }
}
void dLytMapMain_c::finalizeState_EventForestMapChange_Step3() {}

void dLytMapMain_c::initializeState_EventSignalAdd_Step1() {}
void dLytMapMain_c::executeState_EventSignalAdd_Step1() {
    if (mDisplayedBeaconCount != getMaxBeaconCount()) {
        mStateMgr.changeState(StateID_EventSignalAdd_Step2);
    }
}
void dLytMapMain_c::finalizeState_EventSignalAdd_Step1() {}

void dLytMapMain_c::initializeState_EventSignalAdd_Step2() {
    mEventTimer = sHio.field_0x29;
}
void dLytMapMain_c::executeState_EventSignalAdd_Step2() {
    if (mDisplayedBeaconCount == getMaxBeaconCount()) {
        mStateMgr.changeState(StateID_EventSignalAdd_Step3);
    } else if (--mEventTimer == 0) {
        mEventTimer = sHio.field_0x29;
        mDisplayedBeaconCount++;
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_BEACON_COUNT_UP);
    }
}
void dLytMapMain_c::finalizeState_EventSignalAdd_Step2() {}

void dLytMapMain_c::initializeState_EventSignalAdd_Step3() {
    mEventTimer = sHio.field_0x25;
}
void dLytMapMain_c::executeState_EventSignalAdd_Step3() {
    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMapMain_c::finalizeState_EventSignalAdd_Step3() {}

bool dLytMapMain_c::fn_80141530() const {
    return EventManager::isInEvent() && mMapEvent != MAP_EVENT_11;
}

dAcTbox_c *dLytMapMain_c::findGoddessChestForStoryflag(s32 flag) const {
    dAcTbox_c *ret = nullptr;
    fBase_c *it = nullptr;
    while ((it = fManager_c::searchBaseByGroupType(fBase_c::ACTOR, it)) != nullptr) {
        if (it->mProfileName == fProfile::TBOX) {
            dAcTbox_c *box = static_cast<dAcTbox_c *>(it);
            if (box->getVariant() == dAcTbox_c::GODDESS) {
                u16 boxFlag = box->getParams2Lower();
                if (flag == boxFlag) {
                    ret = box;
                    break;
                }
            }
        }
    }
    return ret;
}

void dLytMapMain_c::initializeState_EventGoddessCube_Step1() {
    mEventTimer = 30;
}
void dLytMapMain_c::executeState_EventGoddessCube_Step1() {
    if (mEventTimer != 0) {
        mEventTimer--;
    }
    if (mEventTimer == 0) {
        mStateMgr.changeState(StateID_Active);
    }
}
void dLytMapMain_c::finalizeState_EventGoddessCube_Step1() {
    mEventTimer = 50;
}

void dLytMapMain_c::initializeState_EventGoddessCube_Step2() {
    dAcTbox_c *box = findGoddessChestForStoryflag(STORYFLAG_FIRST_GODDESS_CUBE);
    mGoddessChestWorldPosition = box->getPosition();
    mNextMapMode = dLytMapGlobal_c::MAPMODE_ZOOM;
    dLytMapGlobal_c *global = getGlobal();
    mMapScroll = global->getMapScroll();
    field_0x8D50 = global->getField_0x44();
    field_0x8D44 = global->getMapRotation();
    field_0x8D4C = fn_80142D90(mNextMapMode);
    fn_80142D10(mNextMapMode, mNextMapUpDirection, field_0x8D46);
    mVec3_c center = mGoddessChestWorldPosition;
    mVec2_c result(0.0f, 0.0f);
    global->unprojectFromMap(mMapScroll, result, field_0x8D44);
    global->setMapRotationCenter(center);

    mVec3_c offset(field_0x8CD0.x * 0.5f, 0.0f, field_0x8CD0.z * 0.5f);
    offset.rotY(-global->getField_0x56());
    if (offset.x < 0.0f) {
        offset.x = -offset.x;
    }
    if (offset.z < 0.0f) {
        offset.z = -offset.z;
    }

    mVec3_c diff1(field_0x8CC4.x - offset.x, 0.0f, field_0x8CC4.z - offset.z);
    mVec3_c diff2(field_0x8CC4.x + offset.x, 0.0f, field_0x8CC4.z + offset.z);

    if (mPlayerPos.x < diff1.x) {
        mPlayerPos.x = diff1.x;
    }
    if (mPlayerPos.z < diff1.z) {
        mPlayerPos.z = diff1.z;
    }
    if (mPlayerPos.x > diff2.x) {
        mPlayerPos.x = diff2.x;
    }
    if (mPlayerPos.z > diff2.z) {
        mPlayerPos.z = diff2.z;
    }
    const mVec3_c &c = global->getMapRotationCenter();
    fn_80143060(mPlayerPos, center, c, field_0x8D46);
    mVec3_c tmp = mMapScroll;
    fn_80143060(mMapScroll, tmp, c, field_0x8D44);
    fn_80143060(field_0x8D18, center, global->getPlayerPos(), field_0x8D46);

    global->setMapScroll(mMapScroll);
    const mVec2_c &csPos = dCsGame_c::GetInstance()->getCursorIf()->getCursorPos();
    global->unprojectFromMap(field_0x8CF4, csPos);
    fn_8013FB70(field_0x8CF4, field_0x8D50 / field_0x8D4C);
    mpScaleFramePane->SetVisible(true);
    field_0x8C70 = 0;
    if (mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
        global->setZoomFrame(1.0f);
        global->setField_0x58(field_0x8D50 / field_0x8D4C);
    } else {
        global->setZoomFrame(0.0f);
        global->setField_0x58(1.0f);
    }
    mAnmGroups[MAP_MAIN_ANIM_ROTATE].bind(false);
    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mAnmGroups[MAP_MAIN_ANIM_ROTATE].setFrame(0.0f);
    } else {
        mAnmGroups[MAP_MAIN_ANIM_ROTATE].setToEnd();
    }

    if (mNextMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mMarkers.setIslandNamesOn(false);
    }

    if (mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
        mMarkers.setIslandNamesOn(mShowIslandNames);
    }
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ZOOMIN_TO_DETAIL);
}
void dLytMapMain_c::executeState_EventGoddessCube_Step2() {
    dLytMapGlobal_c *global = getGlobal();
    f32 f1 = (f32)field_0x8C70 / sHio.field_0x1C;
    f32 factor = cLib::easeInOut(f1, 2.0f);

    f32 f2, f3;
    if (mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE) {
        f32 ratio = field_0x8D50 / field_0x8D4C;
        f1 = 1.0f - f1;
        f2 = ratio + (1.0f - ratio) * factor;
        f3 = 1.0f - factor;
    } else {
        f32 ratio = field_0x8D4C / field_0x8D50;
        f2 = ratio + (1.0f - ratio) * (1.0f - factor);
        f3 = factor;
    }
    global->setZoomFrame(f1);
    global->setField_0x58(f2);

    // unused
    mVec3_c tmp = mMapScroll;
    // unused
    mVec3_c tmp2 = mPlayerPos;

    // Lerps
    f32 f4 = field_0x8D50 + factor * (field_0x8D4C - field_0x8D50);
    global->setField_0x44(f4);

    // TODO stack and reg swaps
    global->setMapRotation(factor * mAng(field_0x8D46 - field_0x8D44) + field_0x8D44);

    f32 length = mAnmGroups[MAP_MAIN_ANIM_ROTATE].getLastFrame();

    // unused - the f1 store is here so that the fcmpu isn't optimized away
    if (field_0x8C7C - field_0x8C80) {
        f1 = 0.0f;
    }

    mAnmGroups[MAP_MAIN_ANIM_ROTATE].setFrame(length - f3 * length);

    global->setMapScroll(mPlayerPos + (mMapScroll - mPlayerPos) * (1.0f / (f4 / field_0x8D50)) * (1.0f - factor));

    fn_8013FB70(field_0x8CF4, f4 / field_0x8D4C);

    mpScaleFramePane->SetVisible(true);
    if (field_0x8C70 >= sHio.field_0x1C) {
        mpScaleFramePane->SetVisible(false);
        if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
            mAnmGroups[MAP_MAIN_ANIM_ROTATE].setToEnd();
        } else {
            mAnmGroups[MAP_MAIN_ANIM_ROTATE].setFrame(0.0f);
        }
        mLyt.calc();
        mAnmGroups[MAP_MAIN_ANIM_ROTATE].unbind();
        mMapUpDirection = mNextMapUpDirection;
        global->setMapRotation(field_0x8D46);
        global->setField_0x44(field_0x8D4C);
        global->setMapScroll(field_0x8D18);
        if (dAcPy_c::LINK != nullptr) {
            global->setMapRotationCenter(global->getPlayerPos());
        }
        mCurrentMapMode = mNextMapMode;
        mStateMgr.changeState(StateID_EventGoddessCube_Step3);
    } else {
        field_0x8C70++;
    }
}
void dLytMapMain_c::finalizeState_EventGoddessCube_Step2() {}

void dLytMapMain_c::initializeState_EventGoddessCube_Step3() {}
void dLytMapMain_c::executeState_EventGoddessCube_Step3() {
    mStateMgr.changeState(StateID_Active);
}
void dLytMapMain_c::finalizeState_EventGoddessCube_Step3() {}

bool dLytMapMain_c::checkStoryflag(s32 flag) const {
    return StoryflagManager::sInstance->getFlag(flag);
}

void dLytMapMain_c::initializeState_EventSaveObjMsgWindow() {
    if (mMapEventArg1 == SURFACE_PROVINCE_FARON && checkStoryflag(STORYFLAG_LEVIAS_FIGHT_DEFEATED) &&
        !checkStoryflag(STORYFLAG_FLOODED_FARON_WOODS_DISCOVERED)) {
        // "A report, Master. An unusual phenomenon is taking place in Faron Woods, making it impossible to descend
        // directly into them at the moment."
        mFlowMgr.triggerEntryPoint(4, 18, 0, 0);
    } else {
        // "It is possible to descend to a specific..." / "Point to your preferred descent area..."
        mFlowMgr.triggerEntryPoint(4, 19, 0, 0);
    }
}
void dLytMapMain_c::executeState_EventSaveObjMsgWindow() {
    if (mFlowMgr.checkFinished()) {
        mStateMgr.changeState(StateID_EventSaveObjSelect);
    }
}
void dLytMapMain_c::finalizeState_EventSaveObjMsgWindow() {}

enum dLytMapSaveObjFlagKind {
    SAVE_OBJ_FLAG_KIND_SCENEFLAG = 0,
    SAVE_OBJ_FLAG_KIND_STORYFLAG = 1,
};

struct dLytMapSaveObjFlagDefinition {
    /* 0x00 */ dLytMapSaveObjFlagKind flagType;
    // I wanted designated initializers but MWCC doesn't have them...
    /* 0x04 */ union {
        struct {
            /* 0x04 */ u16 flagArea;
            /* 0x06 */ u16 sceneFlag;
        };
        struct {
            /* 0x04 */ s16 storyFlag;
            /* 0x06 */ u16 _padding;
        };
    };
};

static const dLytMapSaveObjFlagDefinition sSaveObjFlagsFaron[] = {
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 10, 31},
    {SAVE_OBJ_FLAG_KIND_STORYFLAG, STORYFLAG_800},
    {SAVE_OBJ_FLAG_KIND_STORYFLAG, STORYFLAG_801},
    {SAVE_OBJ_FLAG_KIND_STORYFLAG, STORYFLAG_802},
    {SAVE_OBJ_FLAG_KIND_STORYFLAG, STORYFLAG_803},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 1, 104},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 1, 103},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 2, 32},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 2, 33},
    {SAVE_OBJ_FLAG_KIND_STORYFLAG, STORYFLAG_FARON_DISCOVERED},
};

static const dLytMapSaveObjFlagDefinition sSaveObjFlagsEldin[] = {
    {SAVE_OBJ_FLAG_KIND_STORYFLAG, STORYFLAG_804},
    {SAVE_OBJ_FLAG_KIND_STORYFLAG, STORYFLAG_805},
    {SAVE_OBJ_FLAG_KIND_STORYFLAG, STORYFLAG_806},
    {SAVE_OBJ_FLAG_KIND_STORYFLAG, STORYFLAG_807},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 5, 101},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 15, 114},
};

static const dLytMapSaveObjFlagDefinition sSaveObjFlagsLanayru[] = {
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 7, 68},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 7, 66},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 7, 51},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 7, 67},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 7,  2},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 7, 77},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 7, 78},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 8, 10},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 8, 28},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 8, 85},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 8, 84},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, 9, 12},
};

void dLytMapMain_c::displaySaveObjs() {
    for (int i = 0; i < mNumSaveObjs; i++) {
        mSaveObjs[i].init();
    }

    s32 numFlags = ARRAY_LENGTH(sSaveObjFlagsFaron);
    nw4r::lyt::Bounding **pBoundings = mpSaveObjBoundingsFaron;
    const dLytMapSaveObjFlagDefinition *flagSet = sSaveObjFlagsFaron;
    switch (mMapEventArg1) {
        case SURFACE_PROVINCE_ELDIN: {
            pBoundings = mpSaveObjBoundingsEldin;
            flagSet = sSaveObjFlagsEldin;
            numFlags = ARRAY_LENGTH(sSaveObjFlagsEldin);
            break;
        }
        case SURFACE_PROVINCE_LANAYRU: {
            pBoundings = mpSaveObjBoundingsLanayru;
            flagSet = sSaveObjFlagsLanayru;
            numFlags = ARRAY_LENGTH(sSaveObjFlagsLanayru);
            break;
        }
    }

    bool isInFaronSothMode = false;
    bool isInFaronFloodedMode = false;

    if (mMapEventArg1 == SURFACE_PROVINCE_FARON) {
        if (checkStoryflag(STORYFLAG_LEVIAS_FIGHT_DEFEATED) &&
            !checkStoryflag(STORYFLAG_FLOODED_FARON_WOODS_DISCOVERED)) {
            isInFaronSothMode = true;
        } else if (checkStoryflag(STORYFLAG_FLOODED_FARON_WOODS_DISCOVERED) &&
                   !checkStoryflag(STORYFLAG_WATER_DRAGON_SOTH_PART)) {
            isInFaronFloodedMode = true;
        }
    }

    for (int i = 0; i < numFlags; i++) {
        bool visible = false;
        if (isInFaronSothMode) {
            if (i == 0 || i == 9) {
                visible = true;
            }
        } else if (isInFaronFloodedMode) {
            if (i == 0 || i == 4 || i == 9) {
                visible = true;
            }
        } else if (flagSet[i].flagType == SAVE_OBJ_FLAG_KIND_SCENEFLAG) {
            visible = SceneflagManager::sInstance->checkSceneflagGlobal(flagSet[i].flagArea, flagSet[i].sceneFlag);
        } else if (flagSet[i].flagType == SAVE_OBJ_FLAG_KIND_STORYFLAG) {
            visible = checkStoryflag(flagSet[i].storyFlag);
        }
        // TODO - SetVisible clrlwi
        pBoundings[i]->SetVisible(visible);
    }
}

void dLytMapMain_c::initializeState_EventSaveObjSelect() {}
void dLytMapMain_c::executeState_EventSaveObjSelect() {
    mSavePopup.setCurrentStatue(-1);
    s32 currentSelected = getSelectedSaveObjIdx();

    bool anyDecide = false;
    for (int i = 0; i < mNumSaveObjs; i++) {
        if (mSaveObjs[i].mStateMgr.isState(dLytMapSaveObj_c::StateID_Decide)) {
            anyDecide = true;
        }
    }

    for (int i = 0; i < mNumSaveObjs; i++) {
        mSaveObjs[i].mSelectRequest = false;
        if (currentSelected == i && !anyDecide) {
            mSaveObjs[i].mSelectRequest = true;
            if (!mSaveObjs[i].mDecideRequest) {
                mSavePopup.setCurrentStatue(i);
            }
        }
        mSaveObjs[i].execute();
        bool select = false;
        if (mSaveObjs[i].mSelectRequest) {
            if (*mSaveObjs[i].mStateMgr.getStateID() == dLytMapSaveObj_c::StateID_Select) {
                select = true;
            }
        }

        if (select && dPad::getDownTrigA()) {
            mSaveObjs[i].mDecideRequest = true;
            mSavePopup.setCurrentStatue(-1);
            mSavePopup.hide(i);
        }
    }

    field_0x8DC0 = currentSelected;

    bool found = false;
    s32 statueIdx = 0;
    for (int i = 0; i < mNumSaveObjs; i++) {
        if (mSaveObjs[i].mDecideFinished) {
            found = true;
            statueIdx = i;
            break;
        }
    }

    if (found) {
        const dMapSaveObjDefinition *def = getSaveObjDefinition(mMapEventArg1, statueIdx);
        mSaveCaption.setLabel(def->statueLabel);
        mSaveCaption.setType(def->type);
        mSaveCaption.mStateMgr.changeState(dLytMapSaveCaption_c::StateID_In);
        mStateMgr.changeState(StateID_EventSaveObjConfirmMsgWindow);
    } else {
        mSavePopup.execute();
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(mHitChecks); i++) {
        mHitChecks[i].resetCachedHitboxes();
        mHitChecks[i].execute();
    }
}
void dLytMapMain_c::finalizeState_EventSaveObjSelect() {}

static const dMapSaveObjDefinition sFaronSaveObjs[] = {
    {  "F400", 0, 0, 10, 4, "SAVEOBJ_NAME_00"},
    {  "F100", 0, 0, 50, 1, "SAVEOBJ_NAME_01"},
    {  "F100", 0, 0, 51, 1, "SAVEOBJ_NAME_02"},
    {  "F100", 0, 0, 52, 1, "SAVEOBJ_NAME_03"},
    {  "F100", 0, 0, 53, 1, "SAVEOBJ_NAME_04"},
    {  "F101", 0, 0, 13, 1, "SAVEOBJ_NAME_05"},
    {  "F101", 0, 0, 14, 1, "SAVEOBJ_NAME_06"},
    {  "F102", 3, 0,  2, 1, "SAVEOBJ_NAME_07"},
    {"F102_1", 0, 0,  5, 1, "SAVEOBJ_NAME_08"},
    {  "F401", 1, 0,  8, 4, "SAVEOBJ_NAME_28"},
};
static const dMapSaveObjDefinition sFloodedFaronSaveObj = {"F103", 0, 0, 9, 1, "SAVEOBJ_NAME_09"};
static const dMapSaveObjDefinition sFaronLoweredGoddessStatueSaveObj = {"F406", 1, 0, 5, 4, "SAVEOBJ_NAME_29"};
static const dMapSaveObjDefinition sEldinSaveObjs[] = {
    {  "F200",  0, 0, 2, 2, "SAVEOBJ_NAME_10"},
    {  "F200",  2, 0, 6, 2, "SAVEOBJ_NAME_11"},
    {  "F200",  2, 0, 7, 2, "SAVEOBJ_NAME_12"},
    {  "F200",  4, 0, 7, 2, "SAVEOBJ_NAME_13"},
    {"F201_3",  0, 0, 3, 2, "SAVEOBJ_NAME_14"},
    {  "D201", 10, 0, 2, 2, "SAVEOBJ_NAME_15"},
};

static const dMapSaveObjDefinition sLanayruSaveObjs[] = {
    {"F300_1", 0, 0,  2, 3, "SAVEOBJ_NAME_16"},
    {  "F300", 0, 0, 15, 3, "SAVEOBJ_NAME_17"},
    {  "F300", 0, 0, 16, 3, "SAVEOBJ_NAME_18"},
    {  "F300", 0, 0, 17, 3, "SAVEOBJ_NAME_19"},
    {  "F300", 0, 0, 18, 3, "SAVEOBJ_NAME_20"},
    {"F300_4", 0, 0, 16, 3, "SAVEOBJ_NAME_21"},
    {"F300_4", 0, 0, 17, 3, "SAVEOBJ_NAME_22"},
    {  "F301", 0, 0, 10, 3, "SAVEOBJ_NAME_23"},
    {"F301_3", 0, 0, 10, 3, "SAVEOBJ_NAME_24"},
    {"F301_4", 0, 0, 10, 3, "SAVEOBJ_NAME_25"},
    {"F301_6", 0, 0, 10, 3, "SAVEOBJ_NAME_26"},
    {  "F302", 0, 0, 12, 3, "SAVEOBJ_NAME_27"},
};

const dMapSaveObjDefinition *dLytMapMain_c::getSaveObjDefinition(s32 province, s32 index) const {
    const dMapSaveObjDefinition *ret = nullptr;
    switch (province) {
        case SURFACE_PROVINCE_FARON: {
            if (index == 4 && checkStoryflag(STORYFLAG_FLOODED_FARON_WOODS_DISCOVERED) &&
                !checkStoryflag(STORYFLAG_WATER_DRAGON_SOTH_PART)) {
                ret = &sFloodedFaronSaveObj;
            } else if (index == 9 && checkStoryflag(STORYFLAG_GODDESS_STATUE_FALLEN)) {
                ret = &sFaronLoweredGoddessStatueSaveObj;
            } else {
                ret = &sFaronSaveObjs[index];
            }
            break;
        }
        case SURFACE_PROVINCE_ELDIN: {
            ret = &sEldinSaveObjs[index];
            break;
        }
        case SURFACE_PROVINCE_LANAYRU: {
            ret = &sLanayruSaveObjs[index];
            break;
        }
    }
    return ret;
}

void dLytMapMain_c::initializeState_EventSaveObjConfirmMsgWindow() {
    // "Descend into this area? [1]Yes.[2-]No."
    mFlowMgr.triggerEntryPoint(4, 17, 0, 0);
}
void dLytMapMain_c::executeState_EventSaveObjConfirmMsgWindow() {
    if (!mFlowMgr.checkFinished()) {
        return;
    }

    if (dLytMsgWindow_c::getInstance()->getTextOptionSelection() == 0) {
        // confirm
        dBase_c::s_NextExecuteControlFlags &= ~dBase_c::BASE_PROP_0x10;
        dBase_c::s_DrawControlFlags &= ~dBase_c::BASE_PROP_0x10;
        s32 idx = 0;
        s32 statueIdx = -1;
        for (int i = 0; i < mNumSaveObjs; i++) {
            if (mSaveObjs[i].mDecideFinished) {
                statueIdx = i;
                break;
            }
        }

        const dMapSaveObjDefinition *def = getSaveObjDefinition(mMapEventArg1, statueIdx);
        dScGame_c::GetInstance()->triggerEntrance(
            def->stageName, def->room, def->layer, def->entrance, SpawnInfo::RETAIN_TOD, SpawnInfo::RETAIN_TRIAL,
            dFader_c::FADER_GREY
        );
        dScGame_c::GetInstance()->setTargetingScreenPrio(0x8B);
        dLytMap_c::GetInstance()->fadeOut();
        mStateMgr.changeState(StateID_EventSaveObjDecide);
        mSaveCaption.mStateMgr.changeState(dLytMapSaveCaption_c::StateID_Out);
    } else {
        // cancel
        for (int i = 0; i < mNumSaveObjs; i++) {
            mSaveObjs[i].init();
        }
        mStateMgr.changeState(StateID_EventSaveObjSelect);
        mSaveCaption.mStateMgr.changeState(dLytMapSaveCaption_c::StateID_Out);
    }
}
void dLytMapMain_c::finalizeState_EventSaveObjConfirmMsgWindow() {}

void dLytMapMain_c::initializeState_EventSaveObjDecide() {}
void dLytMapMain_c::executeState_EventSaveObjDecide() {}
void dLytMapMain_c::finalizeState_EventSaveObjDecide() {}

void dLytMapMain_c::checkCursorPointedAtMap() {
    const mVec2_c *pos2d;
    dLytMapGlobal_c *global = getGlobal();
    bool pointerOnMap = true;
    bool canPlaceBeacon = true;

    pos2d = &dCsGame_c::GetInstance()->getCursorIf()->getCursorPos();
    mVec3_c pos3d(0.0f, 0.0f, 0.0f);
    global->unprojectFromMap(pos3d, *pos2d, global->getMapRotation());

    f32 tmp = global->getField_0x28().x * 368.0f;
    f32 nTmp = -tmp;
    if (pos2d->x > tmp || pos2d->x < nTmp || pos2d->y > 209.0f || pos2d->y < -193.0f) {
        // Pointer off screen?
        pointerOnMap = false;
        canPlaceBeacon = false;
    }

    if (pointerOnMap && (pos3d.x < field_0x8930.x || pos3d.x > field_0x893C.x || pos3d.z < field_0x8930.z ||
                         pos3d.z > field_0x893C.z)) {
        // Pointer too close to the edge?
        canPlaceBeacon = false;
    }

    if (pointerOnMap) {
        for (int i = 0; i < MAP_MAIN_NUM_BOUNDINGS_NORMAL; i++) {
            if (pos3d.x >= mBoundingWorldBounds[i].min.x && pos3d.x <= mBoundingWorldBounds[i].max.x &&
                pos3d.z >= mBoundingWorldBounds[i].min.z && pos3d.z <= mBoundingWorldBounds[i].max.z) {
                // Cannot place beacon when hovering over UI element (remocon, ...)
                canPlaceBeacon = false;
                break;
            }
        }
    }

    mPointerOnMap = pointerOnMap;
    mPointerCanPlaceBeacon = canPlaceBeacon;
}

dLytMap_c *dLytMap_c::sInstance;

bool dLytMap_c::build() {
    d2d::setLytAllocator();
    void *data = LayoutArcManager::GetInstance()->getLoadedData("Map2D");
    mResAcc.attach(data, "");
    mMapMain.build();
    m2d::setLytAllocator();
    mMapMain.setPriority(0x86);
    mMapFader.setPriority(0x8A);
    return true;
}

bool dLytMap_c::remove() {
    d2d::setLytAllocator();
    mMapMain.remove();
    mResAcc.detach();
    dCsGame_c::GetInstance()->setPriorityDraw(0x88);
    dScGame_c::GetInstance()->setTargetingScreenPrio(0x85);
    return true;
}

bool dLytMap_c::execute() {
    mMapMain.execute();
    if (mMapFader.isVisible()) {
        mMapFader.calc();
    }
    return true;
}

bool dLytMap_c::draw() {
    mMapMain.addToDrawList();
    if (mMapFader.isVisible()) {
        mMapFader.addToDrawList();
    }
    return true;
}

void dLytMap_c::fadeOut() {
    mMapFader.setVisible(true);
    mMapFader.fadeOut();
}
