#define NEED_DIRECT_FRAMECTRL_ACCESS
#include "d/lyt/d_lyt_map.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_tbox.h"
#include "d/d_base.h"
#include "d/d_cs_base.h"
#include "d/d_cs_game.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_d2d.h"
#include "d/d_message.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/d_player.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_control_game.h"
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

inline mVec3_c vec2ToVec3XY(const mVec2_c &v) {
    return mVec3_c(v.x, v.y, 0.0f);
}

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
      field_0x1A0(0) {
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
    if (!field_0x1A0) {
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
    for (int i = 0; i < 5; i++) {
        if (mPins[i].isSelect()) {
            mStateMgr.changeState(StateID_Select);
        }
    }
}
void dLytMapPinIconAggregate_c::finalizeState_Wait() {}

void dLytMapPinIconAggregate_c::initializeState_Select() {}
void dLytMapPinIconAggregate_c::executeState_Select() {
    bool anyRemove = false;
    for (int i = 0; i < 5; i++) {
        if (mPins[i].isRemove()) {
            mStateMgr.changeState(StateID_Remove);
            anyRemove = true;
            // a bit convoluted, an early return would've done the trick
            break;
        }
    }

    if (!anyRemove) {
        bool allSelect = true;
        for (int i = 0; i < 5; i++) {
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

s32 dLytMapPinIconAggregate_c::getNumSetPins() const {
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
    if (field_0x712) {
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
      field_0x8C6C(0),
      mMapUpDirectionAfterZoomToDetail(true),
      field_0x8C93(0),
      field_0x8C94(0),
      field_0x8C98(0),
      field_0x8CAC(0),
      field_0x8D38(0.0f, 0.0f),
      field_0x8D40(1.0f),
      field_0x8D44(0),
      field_0x8D46(0),
      field_0x8D48(0),
      field_0x8D4C(0.0f),
      field_0x8D50(0.0f),
      field_0x8D58(0),
      field_0x8D5C(1),
      field_0x8D60(0),
      field_0x8D64(0),
      field_0x8D68(0),
      field_0x8D6A(0),
      field_0x8D6B(0),
      field_0x8D6C(0),
      field_0x8D70(0),
      field_0x8DB0(0),
      mNavEnabled(false),
      mDrawScaleFrame(false),
      mDisplayedBeaconCount(0),
      mShowIslandNames(false),
      field_0x8DBD(0),
      field_0x8DBE(0),
      field_0x8DBF(0),
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

#define MAP_MAIN_ANIM_SUN_ROTE 45
#define MAP_MAIN_ANIM_IN_NO_CAM 46
#define MAP_MAIN_ANIM_OUT_NO_CAM 47
#define MAP_MAIN_ANIM_CLOUD_LOOK 48
#define MAP_MAIN_ANIM_LIGHT_LOOP 49
#define MAP_MAIN_ANIM_NUSHI_LOOP 50
#define MAP_MAIN_ANIM_TYPE 51
#define MAP_MAIN_ANIM_KUMO_PATTERN 52
#define MAP_MAIN_ANIM_TITE_LINE 53

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

static const char *sPaneNamesUnk1[] = {
    "N_fD1_00", "N_vD1_00", "N_dD1_00", "N_fD2_00", "N_dD2_00", "N_vD2_00", "N_lastD_00",
};

static const char *sPaneNamesUnk2[] = {
    "P_fD1_00", "P_vD1_00", "P_dD1_00", "P_fD2_00", "P_dD2_00", "P_vD2_00", "P_lastD_00",
};

static const char *sPriorityGroupNames[] = {
    "G_priority_00",
    "G_priority_02",
};

void dLytMapMain_c::build() {
    field_0x8DBF = 0;
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

    // TODO define
    for (int i = 0; i < 54; i++) {
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
            field_0x821C[i - 5] = b;
        } else if (i >= 15 && i < 21) {
            field_0x8244[i - 15] = b;
        } else if (i >= 21 && i < 33) {
            field_0x825C[i - 21] = b;
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

    for (int i = 0; i < (int)ARRAY_LENGTH(sPaneNamesUnk1); i++) {
        if (sPaneNamesUnk1[i] != nullptr) {
            mpUnkPanes1[i] = mLyt.findPane(sPaneNamesUnk1[i]);
        } else {
            mpUnkPanes1[i] = nullptr;
        }

        if (sPaneNamesUnk2[i] != nullptr) {
            mpUnkPanes2[i] = mLyt.findPane(sPaneNamesUnk2[i]);
        } else {
            mpUnkPanes2[i] = nullptr;
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
    field_0x8D58 = 0;
    field_0x8D5C = 1;
    field_0x8D60 = 0;
    field_0x8D64 = 0;
    fn_80143300();
}

void dLytMapMain_c::draw() {
    if (!field_0x8DBF) {
        return;
    }

    dLytMapGlobal_c *global = getGlobal();
    fn_80143120(1);
    mLyt.draw();
    if (shouldDrawFootprints() && field_0x8D58 == global->getFloor()) {
        mFootPrints.draw();
    }

    if (mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE || mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM ||
        mNextMapMode == dLytMapGlobal_c::MAPMODE_STAGE || mNextMapMode == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mMarkers.draw();
    }

    if (field_0x8C68 != 3 && field_0x8C68 != 5 && field_0x8C94 != 2) {
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
        field_0x8C94 == 10) {
        fn_80138D80();
    }
    fn_80143120(-2);
    mMarkers.drawPopups();
    mPutIcon.draw();
    if (field_0x8C94 == 10) {
        mPopupInfo.draw();
    }

    if (*mStateMgr.getStateID() == StateID_EventSaveObjSelect) {
        mSavePopup.draw();
    }
    mSaveCaption.draw();
}

s32 dLytMapMain_c::getSelectedSaveObjIdx() const {
    bool has = false;
    s32 start;
    s32 end;
    if (field_0x8C6C == 2) {
        start = 5;
        end = 14;
        has = true;
    } else if (field_0x8C6C == 3) {
        start = 15;
        end = 20;
        has = true;
    } else if (field_0x8C6C == 4) {
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

void lytMapusesSizedWString() {
    static SizedWString<0x20> sString;
    sString.sprintf(L"%ld", 0);
}

void dLytMapMain_c::saveUnkMapData() {
    dMapSavedDataEntry &data = sSavedMapData.entries[field_0x8C68];
    data.field_0x06 = 1;
    data.mapMode = mCurrentMapMode;
    data.mapUpDirection = mMapUpDirection;
    data.mapUpDirectionAfterZoomToDetail = mMapUpDirectionAfterZoomToDetail;
    sSavedMapData.showIslandNames = mShowIslandNames;
}

void dLytMapMain_c::initUnkMapData() {
    for (int i = 0; i < 6; i++) {
        sSavedMapData.entries[i].field_0x06 = sDefaultMapData.entries[i].field_0x06;
        sSavedMapData.entries[i].mapMode = sDefaultMapData.entries[i].mapMode;
        sSavedMapData.entries[i].mapUpDirection = sDefaultMapData.entries[i].mapUpDirection;
        sSavedMapData.entries[i].mapUpDirectionAfterZoomToDetail =
            sDefaultMapData.entries[i].mapUpDirectionAfterZoomToDetail;
    }
    sSavedMapData.showIslandNames = sDefaultMapData.showIslandNames;
}

void dLytMapMain_c::loadUnkMapData() {
    mCurrentMapMode = sSavedMapData.entries[field_0x8C68].mapMode;
    mMapUpDirection = sSavedMapData.entries[field_0x8C68].mapUpDirection;
    mMapUpDirectionAfterZoomToDetail = sSavedMapData.entries[field_0x8C68].mapUpDirectionAfterZoomToDetail;
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
        if ((field_0x8C68 == 0 && currentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE) ||
            (field_0x8C68 == 2 && currentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) ||
            (field_0x8C68 == 4 && currentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) ||
            (field_0x8C68 == 3 && currentMapMode == dLytMapGlobal_c::MAPMODE_WORLD_SKY) ||
            (field_0x8C68 == 5 && currentMapMode == dLytMapGlobal_c::MAPMODE_STAGE) ||
            (field_0x8C68 == 1 && currentMapMode == dLytMapGlobal_c::MAPMODE_PROVINCE)) {
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

    switch (field_0x8C68) {
        case 2: {
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
        case 0:
        case 4: {
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

            if (field_0x8C68 == 4 && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE &&
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
        case 1: {
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

void dLytMapMain_c::setCursorType() {
    if (canCenterCursor1(mCurrentMapMode) && canCenterCursor1(mNextMapMode)) {
        if ((field_0x8C94 == 10 || ((field_0x8C68 == 3 || field_0x8C68 == 1) && mPointerOnMap)) ||
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

void dLytMapMain_c::initializeState_In() {}
void dLytMapMain_c::executeState_In() {}
void dLytMapMain_c::finalizeState_In() {}

void dLytMapMain_c::zoomIn() {
    if (field_0x8C68 == 2 || (field_0x8C6C == 1 && (field_0x8C68 == 3 || field_0x8C68 == 1))) {
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

                if (mPointerOnMap && (field_0x8D5C < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_CsrRot);
                } else {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
                }
            }
        }
    } else if (field_0x8C68 == 4 || field_0x8C68 == 5) {
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

                if (mPointerOnMap && (field_0x8D5C < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_CsrRot);
                } else {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
                }
            }
        }
    } else if (field_0x8C68 == 0) {
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

                if (mPointerOnMap && (field_0x8D5C < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_CsrRot);
                } else {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
                }
            }
        }
    } else if (field_0x8C68 == 1) {
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

                if (mPointerOnMap && (field_0x8D5C < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_CsrRot);
                } else {
                    mStateMgr.changeState(StateID_ChgDispAreaMode_MapRot);
                }
            }
        }
    } else if (field_0x8C68 == 3) {
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
    if (field_0x8C68 == 2 || (field_0x8C6C == 1 && (field_0x8C68 == 3 || field_0x8C68 == 1))) {
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
    } else if (field_0x8C68 == 4 || field_0x8C68 == 5) {
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
    } else if (field_0x8C68 == 0) {
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
    } else if (field_0x8C68 == 1) {
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
    } else if (field_0x8C68 == 3) {
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
    if (dMessage_c::getInstance()->getField_0x328()) {
        return;
    }

    // TODO figure out and double check numbers
    if (field_0x8C68 == 4 && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE &&
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
        if (mPointerOnMap && (field_0x8D5C < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
            // Can zoom in around cursor, use the cursor pos
            getGlobal()->unprojectFromMap(field_0x8CF4, dCsGame_c::GetInstance()->getCursorIf()->getCursorPos());
            f32 tmp = fn_80142D90(4);
            fn_8013FB70(field_0x8CF4, getGlobal()->getField_0x44() / tmp);
            mDrawScaleFrame = true;
        } else {
            // Cannot zoom in around cursor, use the player pos
            f32 tmp = fn_80142D90(4);
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
               (dPad::getDownTrigA() && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_ZOOM && canZoomOut(mCurrentMapMode)
               )) {
        zoomOut();
    } else if ((dPad::getDownTrigRight() && canZoomIn(mCurrentMapMode)) ||
               (dPad::getDownTrigA() && mCurrentMapMode == dLytMapGlobal_c::MAPMODE_STAGE && canZoomIn(mCurrentMapMode)
               )) {
        zoomIn();
    } else if ((field_0x8C68 == dLytMapGlobal_c::MAPMODE_WORLD || field_0x8C68 == dLytMapGlobal_c::MAPMODE_PROVINCE ||
                field_0x8C68 == dLytMapGlobal_c::MAPMODE_WORLD_SKY || field_0x8C68 == dLytMapGlobal_c::MAPMODE_ZOOM) &&
               canResetPosition(mCurrentMapMode, mMapUpDirection) && dPad::getDownTrigUp()) {
        mVec3_c pos;
        fn_80142F00(pos, mCurrentMapMode, mMapUpDirection, global->getMapRotationCenter(), global->getField_0x56());
        f32 distSq = global->getMapScroll().squareDistanceToXZ(pos);
        if (field_0x8D58 != global->getFloor()) {
            mFloorBtnMgr.resetFloor(-(field_0x8D58 - field_0x8D60));
            global->setFloor(field_0x8D58);
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
    if (field_0x8C94 == 10) {
        mPopupInfo.mStateMgr.changeState(StateID_Out);
    }
    dLytMeter_c::GetInstance()->setMeterField_0x13750(0);
    if (field_0x8CAD || field_0x8C94 == 3) {
        mpOutAnmGroup = &mAnmGroups[MAP_MAIN_ANIM_OUT];
    } else {
        mpOutAnmGroup = &mAnmGroups[MAP_MAIN_ANIM_OUT_NO_CAM];
    }

    mpOutAnmGroup->bind(false);
    mpOutAnmGroup->setFrame(0.0f);
    field_0x8C8C = mpOutAnmGroup->getLastFrame();

    if (fn_80141530()) {
        dBase_c::s_NextExecuteControlFlags &= ~dBase_c::BASE_PROP_0x10;
    }

    dPadNav::setNavEnabled(false, false);
}
void dLytMapMain_c::executeState_Out() {
    if (mpOutAnmGroup->isEndReached()) {
        bool specialMode = isSomeFieldEq0Or1Or7Or9Or11();
        // Kind of a weird way to write `specialMode || ... != 3 `
        if ((specialMode && dScGame_c::getCamera(0)->getField_0xDA8() != 3) || !specialMode) {
            dBase_c::s_NextExecuteControlFlags &= ~dBase_c::BASE_PROP_0x10;
            mLyt.calc();
            mpOutAnmGroup->unbind();
            field_0x8DBF = 0;
            mStateMgr.changeState(StateID_Invisible);
        }
    }

    if (mpOutAnmGroup->isBound()) {
        mpOutAnmGroup->play();
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

    if (field_0x8C94 == 3) {
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
    if (field_0x8C94 == 3 && field_0x8D6A) {
        fn_80140B90();
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
            if (field_0x8C94 == 3) {
                mStateMgr.changeState(StateID_EventMapIntro_Step4);
            } else {
                mStateMgr.changeState(StateID_Active);
            }
        } else {
            field_0x8C70++;
        }

        if (field_0x8C94 == 3) {
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
    if (field_0x8C94 == 3) {
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
    if (field_0x8C94 == 3 && field_0x8D6A) {
        fn_80140B90();
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
            if (field_0x8C94 == 3 && field_0x0108 > 0) {
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

                if (field_0x8C94 == 3) {
                    mStateMgr.changeState(StateID_EventMapIntro_Step4);
                } else {
                    mStateMgr.changeState(StateID_Active);
                    mCurrentMapMode = mNextMapMode;
                }
            }
        } else {
            field_0x8C70++;
        }
        if (field_0x8C94 == 3) {
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

    field_0x8D6B = 1;
}
void dLytMapMain_c::finalizeState_EventSwBankSmall_Step3() {}

void dLytMapMain_c::initializeState_EventMapIntro_Step1() {
    mEventTimer = sHio.field_0x1F;
}
void dLytMapMain_c::executeState_EventMapIntro_Step1() {
    if (field_0x8D6A) {
        fn_80140B90();
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
    if (field_0x8D6A) {
        fn_80140B90();
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
    if (field_0x8D6A) {
        fn_80140B90();
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
    if (field_0x8D6A) {
        fn_80140B90();
        return;
    }

    if (mEventTimer != 0) {
        mEventTimer--;
    } else {
        field_0x8D6B = 1;
    }
    EventManager::execute();
}
void dLytMapMain_c::finalizeState_EventMapIntro_Step4() {}

void dLytMapMain_c::fn_80140B90() {
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
    field_0x8DBE = 0;
    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_EXTEND);
}
void dLytMapMain_c::executeState_EventDungeonMapGet_Step2() {
    dLytMapGlobal_c *global = getGlobal();
    if (mEventTimer != 0 || field_0x8CB4 != 0 || field_0x8CB8 != 0) {
        if (mEventTimer != 0) {
            mEventTimer--;
        }
        u8 u1 = (1.0f - (f32)mEventTimer / sHio.field_0x23) * 255.0f;
        global->setField_0x4F(u1);
        field_0x8DBE = u1;
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
        field_0x8D6B = 1;
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
    field_0x8DBE = 0;
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
    } else if (field_0x8C94 == 8) {
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
    field_0x8DBE = 0;
}
void dLytMapMain_c::executeState_EventForestMapChange_Step2() {
    dLytMapGlobal_c *global = getGlobal();
    if (mEventTimer != 0 || field_0x8CB4 != 0 || field_0x8CB8 != 0) {
        if (mEventTimer != 0) {
            mEventTimer--;
        }
        u8 u1 = (1.0f - (f32)mEventTimer / sHio.field_0x23) * 255.0f;
        global->setField_0x4F(u1);
        field_0x8DBE = u1;
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
        field_0x8D6B = 1;
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
    return EventManager::isInEvent() && field_0x8C94 != 11;
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

void dLytMapMain_c::initializeState_EventGoddessCube_Step2() {}
void dLytMapMain_c::executeState_EventGoddessCube_Step2() {}
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
    if (mProvince == PROVINCE_FARON && checkStoryflag(STORYFLAG_LEVIAS_FIGHT_DEFEATED) &&
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
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, STORYFLAG_801},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, STORYFLAG_802},
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, STORYFLAG_803},
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
    {SAVE_OBJ_FLAG_KIND_SCENEFLAG, STORYFLAG_807},
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
    for (int i = 0; i < field_0x8320; i++) {
        mSaveObjs[i].init();
    }

    s32 numFlags = ARRAY_LENGTH(sSaveObjFlagsFaron);
    nw4r::lyt::Bounding **pBoundings = field_0x821C;
    const dLytMapSaveObjFlagDefinition *flagSet = sSaveObjFlagsFaron;
    switch (mProvince) {
        case PROVINCE_ELDIN: {
            pBoundings = field_0x8244;
            flagSet = sSaveObjFlagsEldin;
            numFlags = ARRAY_LENGTH(sSaveObjFlagsEldin);
            break;
        }
        case PROVINCE_LANAYRU: {
            pBoundings = field_0x825C;
            flagSet = sSaveObjFlagsLanayru;
            numFlags = ARRAY_LENGTH(sSaveObjFlagsLanayru);
            break;
        }
    }

    bool isInFaronSothMode = false;
    bool isInFaronFloodedMode = false;

    if (mProvince == PROVINCE_FARON) {
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
    for (int i = 0; i < field_0x8320; i++) {
        if (mSaveObjs[i].mStateMgr.isState(dLytMapSaveObj_c::StateID_Decide)) {
            anyDecide = true;
        }
    }

    for (int i = 0; i < field_0x8320; i++) {
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
    for (int i = 0; i < field_0x8320; i++) {
        if (mSaveObjs[i].mDecideFinished) {
            found = true;
            statueIdx = i;
            break;
        }
    }

    if (found) {
        const dMapSaveObjDefinition *def = getSaveObjDefinition(mProvince, statueIdx);
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

void dLytMapMain_c::initializeState_EventSaveObjConfirmMsgWindow() {
    // "Descend into this area? [1]Yes.[2-]No."
    mFlowMgr.triggerEntryPoint(4, 17, 0, 0);
}
void dLytMapMain_c::executeState_EventSaveObjConfirmMsgWindow() {}
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

void dLytMap_c::build() {
    d2d::setLytAllocator();
    void *data = LayoutArcManager::GetInstance()->getLoadedData("Map2D");
    mResAcc.attach(data, "");
    mMapMain.build();
    m2d::setLytAllocator();
    mMapMain.setPriority(0x86);
    // TODO there's another thing here at 0x91C0
}
