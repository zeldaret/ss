#include "d/lyt/d_lyt_map.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_cs_base.h"
#include "d/d_cs_game.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_d2d.h"
#include "d/d_message.h"
#include "d/d_pad.h"
#include "d/d_pad_nav.h"
#include "d/d_player.h"
#include "d/d_rumble.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_map_global.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "egg/core/eggColorFader.h"
#include "m/m_vec.h"
#include "m/m_video.h"
#include "nw4r/g3d/g3d_obj.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_window.h"
#include "s/s_StateInterfaces.hpp"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/d_beacon.h"

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
            const dMapSaveDefinition *def = dLytMap_c::GetInstance()->getSaveDefinition(i);
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

void dLytMapSaveCaption_c::initializeState_Invisible() {}
void dLytMapSaveCaption_c::executeState_Invisible() {}
void dLytMapSaveCaption_c::finalizeState_Invisible() {}

void dLytMapSaveCaption_c::initializeState_In() {}
void dLytMapSaveCaption_c::executeState_In() {}
void dLytMapSaveCaption_c::finalizeState_In() {}

void dLytMapSaveCaption_c::initializeState_Wait() {}
void dLytMapSaveCaption_c::executeState_Wait() {}
void dLytMapSaveCaption_c::finalizeState_Wait() {}

void dLytMapSaveCaption_c::initializeState_Out() {}
void dLytMapSaveCaption_c::executeState_Out() {}
void dLytMapSaveCaption_c::finalizeState_Out() {}

void dLytMapSaveObj_c::initializeState_Wait() {}
void dLytMapSaveObj_c::executeState_Wait() {}
void dLytMapSaveObj_c::finalizeState_Wait() {}

void dLytMapSaveObj_c::initializeState_ToSelect() {}
void dLytMapSaveObj_c::executeState_ToSelect() {}
void dLytMapSaveObj_c::finalizeState_ToSelect() {}

void dLytMapSaveObj_c::initializeState_Select() {}
void dLytMapSaveObj_c::executeState_Select() {}
void dLytMapSaveObj_c::finalizeState_Select() {}

void dLytMapSaveObj_c::initializeState_ToWait() {}
void dLytMapSaveObj_c::executeState_ToWait() {}
void dLytMapSaveObj_c::finalizeState_ToWait() {}

void dLytMapSaveObj_c::initializeState_Decide() {}
void dLytMapSaveObj_c::executeState_Decide() {}
void dLytMapSaveObj_c::finalizeState_Decide() {}

#pragma dont_inline on
dLytMapMain_c::dLytMapMain_c()
    : mStateMgr(*this),
      mFlowMgr(&mFlow),
      mFloorBtnMgr(&mGlobal),
      mFootPrints(&mGlobal),
      field_0x8D38(0.0f, 0.0f),
      field_0x8D40(1.0f),
      field_0x8D44(0),
      field_0x8D46(0),
      field_0x8D48(0),
      field_0x8D4C(0.0f),
      field_0x8D50(0.0f),
      field_0x8D68(0) {}
#pragma dont_inline reset

dLytMapMain_c::~dLytMapMain_c() {
    for (int i = 0; i < (int)ARRAY_LENGTH(field_0x832C); i++) {
        if (dCsMgr_c::GetInstance()->isRegist(&field_0x832C[i])) {
            dCsMgr_c::GetInstance()->unregistCursorTarget(&field_0x832C[i]);
        }
    }
}

dLytMapGlobal_c *dLytMapMain_c::getGlobal() {
    return &mGlobal;
}

void dLytMapMain_c::draw() {}

void dLytMap_c::build() {
    d2d::setLytAllocator();
    void *data = LayoutArcManager::GetInstance()->getLoadedData("Map2D");
    mResAcc.attach(data, "");
    mMapMain.build();
    m2d::setLytAllocator();
    mMapMain.setPriority(0x86);
    // TODO there's another thing here at 0x91C0
}

void lytMapusesSizedWString() {
    static SizedWString<0x20> sString;
    sString.sprintf(L"%ld", 0);
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

void dLytMapMain_c::initializeState_Active() {}
void dLytMapMain_c::executeState_Active() {
    dLytMapGlobal_c *global = getGlobal();
    if (dMessage_c::getInstance()->getField_0x328()) {
        return;
    }

    // TODO figure out and double check numbers
    if (field_0x8C68 == 4 && field_0x8CA4 == dLytMapGlobal_c::MAPMODE_STAGE &&
        field_0x8CA8 == dLytMapGlobal_c::MAPMODE_STAGE && dPad::getDownTrigZ()) {
        if (mIslandNamesOn) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ISLAND_NAME_OFF);
        } else {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ISLAND_NAME_ON);
        }
        mIslandNamesOn = !mIslandNamesOn;
        field_0x2060.setIslandNamesOn(mIslandNamesOn);
    }
    checkScroll();

    if (needsNav(field_0x8CA4) && needsNav(field_0x8CA8)) {
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

    if (field_0x8CA4 == dLytMapGlobal_c::MAPMODE_STAGE && field_0x8CA8 == dLytMapGlobal_c::MAPMODE_STAGE &&
        canZoomIn(dLytMapGlobal_c::MAPMODE_STAGE)) {
        if (field_0x8CC0 && (field_0x8D5C < 2 || !mFloorBtnMgr.hasPointedAtABtnIdx())) {
            const mVec3_c &pos = global->getPlayerPos();
            getGlobal()->unprojectFromMap(field_0x8CF4, dCsGame_c::GetInstance()->getCursorIf()->getCursorPos());
            f32 tmp = fn_80142D90(4);
            fn_8013FB70(field_0x8CF4, getGlobal()->getField_0x44() / tmp);
            field_0x8DB5 = true;
        } else {
            f32 tmp = fn_80142D90(4);
            f32 val = getGlobal()->getField_0x44() / tmp;
            fn_8013FB70(getGlobal()->getPlayerPos(), val);
            field_0x8DB5 = true;
        }
    }

    if (field_0x8DBD && *mPinIconAggregate.mStateMgr.getStateID() != dLytMapPinIconAggregate_c::StateID_Select &&
        dPad::getDownTrigC() && field_0x8CC1) {
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
    } else if ((dPad::getDownTrigLeft() && canZoomOut(field_0x8CA4)) ||
               (dPad::getDownTrigA() && field_0x8CA4 == dLytMapGlobal_c::MAPMODE_ZOOM && canZoomOut(field_0x8CA4))) {
        zoomOut();
    } else if ((dPad::getDownTrigRight() && canZoomIn(field_0x8CA4)) ||
               (dPad::getDownTrigA() && field_0x8CA4 == dLytMapGlobal_c::MAPMODE_STAGE && canZoomIn(field_0x8CA4))) {
        zoomIn();
    } else if ((field_0x8C68 == dLytMapGlobal_c::MAPMODE_WORLD || field_0x8C68 == dLytMapGlobal_c::MAPMODE_PROVINCE ||
                field_0x8C68 == dLytMapGlobal_c::MAPMODE_WORLD_SKY || field_0x8C68 == dLytMapGlobal_c::MAPMODE_ZOOM) &&
               canResetPosition(field_0x8CA4, field_0x8C90) && dPad::getDownTrigUp()) {
        mVec3_c pos;
        fn_80142F00(pos, field_0x8CA4, field_0x8C90, global->getMapRotationCenter(), global->getField_0x56());
        f32 distSq = global->getField_0x00().squareDistanceToXZ(pos);
        if (field_0x8D58 != global->getFloor()) {
            mFloorBtnMgr.resetFloor(-(field_0x8D58 - field_0x8D60));
            global->setFloor(field_0x8D58);
            mStateMgr.changeState(StateID_ResetPosWithFloorChange);
        } else if (distSq >= 1.0f) {
            mStateMgr.changeState(StateID_ResetPos);
        } else {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_RESET_NOT_MOVE);
        }
    } else if (canChangeUpDirection(field_0x8CA4, field_0x8C90) && dPad::getDownTrigZ()) {
        mStateMgr.changeState(StateID_ChgMapUpDirection);
    }

    if ((field_0x8CA4 == dLytMapGlobal_c::MAPMODE_STAGE && field_0x8CA8 == dLytMapGlobal_c::MAPMODE_STAGE) ||
        (field_0x8CA4 == dLytMapGlobal_c::MAPMODE_ZOOM && field_0x8CA8 == dLytMapGlobal_c::MAPMODE_ZOOM)) {
        field_0xF1C.field_0x05 = true;
    }

    mPinIconAggregate.execute();

    for (int idx = 0; idx < (int)ARRAY_LENGTH(field_0x832C); idx++) {
        field_0x832C[idx].resetCachedHitboxes();
        field_0x832C[idx].execute();
    }
}
void dLytMapMain_c::finalizeState_Active() {}

void dLytMapMain_c::initializeState_Out() {}
void dLytMapMain_c::executeState_Out() {}
void dLytMapMain_c::finalizeState_Out() {}

void dLytMapMain_c::initializeState_FloorChange() {}
void dLytMapMain_c::executeState_FloorChange() {}
void dLytMapMain_c::finalizeState_FloorChange() {}

void dLytMapMain_c::initializeState_ChgMapUpDirection() {}
void dLytMapMain_c::executeState_ChgMapUpDirection() {}
void dLytMapMain_c::finalizeState_ChgMapUpDirection() {}

void dLytMapMain_c::initializeState_ChgDispAreaMode_MapRot() {}
void dLytMapMain_c::executeState_ChgDispAreaMode_MapRot() {}
void dLytMapMain_c::finalizeState_ChgDispAreaMode_MapRot() {}

void dLytMapMain_c::initializeState_ChgDispAreaMode_Map() {}
void dLytMapMain_c::executeState_ChgDispAreaMode_Map() {}
void dLytMapMain_c::finalizeState_ChgDispAreaMode_Map() {}

void dLytMapMain_c::initializeState_ChgDispAreaMode_WA() {}
void dLytMapMain_c::executeState_ChgDispAreaMode_WA() {}
void dLytMapMain_c::finalizeState_ChgDispAreaMode_WA() {}

void dLytMapMain_c::initializeState_ChgDispAreaMode_CsrRot() {}
void dLytMapMain_c::executeState_ChgDispAreaMode_CsrRot() {}
void dLytMapMain_c::finalizeState_ChgDispAreaMode_CsrRot() {}

void dLytMapMain_c::initializeState_ResetPos() {}
void dLytMapMain_c::executeState_ResetPos() {}
void dLytMapMain_c::finalizeState_ResetPos() {}

void dLytMapMain_c::initializeState_ResetPosWithFloorChange() {}
void dLytMapMain_c::executeState_ResetPosWithFloorChange() {}
void dLytMapMain_c::finalizeState_ResetPosWithFloorChange() {}

void dLytMapMain_c::initializeState_EventSwBankSmall_Step1() {}
void dLytMapMain_c::executeState_EventSwBankSmall_Step1() {}
void dLytMapMain_c::finalizeState_EventSwBankSmall_Step1() {}

void dLytMapMain_c::initializeState_EventSwBankSmall_Step2() {}
void dLytMapMain_c::executeState_EventSwBankSmall_Step2() {}
void dLytMapMain_c::finalizeState_EventSwBankSmall_Step2() {}

void dLytMapMain_c::initializeState_EventSwBankSmall_Step3() {}
void dLytMapMain_c::executeState_EventSwBankSmall_Step3() {}
void dLytMapMain_c::finalizeState_EventSwBankSmall_Step3() {}

void dLytMapMain_c::initializeState_EventMapIntro_Step1() {}
void dLytMapMain_c::executeState_EventMapIntro_Step1() {}
void dLytMapMain_c::finalizeState_EventMapIntro_Step1() {}

void dLytMapMain_c::initializeState_EventMapIntro_Step2() {}
void dLytMapMain_c::executeState_EventMapIntro_Step2() {}
void dLytMapMain_c::finalizeState_EventMapIntro_Step2() {}

void dLytMapMain_c::initializeState_EventMapIntro_Step3() {}
void dLytMapMain_c::executeState_EventMapIntro_Step3() {}
void dLytMapMain_c::finalizeState_EventMapIntro_Step3() {}

void dLytMapMain_c::initializeState_EventMapIntro_Step4() {}
void dLytMapMain_c::executeState_EventMapIntro_Step4() {}
void dLytMapMain_c::finalizeState_EventMapIntro_Step4() {}

void dLytMapMain_c::initializeState_EventDungeonMapGet_Step1() {}
void dLytMapMain_c::executeState_EventDungeonMapGet_Step1() {}
void dLytMapMain_c::finalizeState_EventDungeonMapGet_Step1() {}

void dLytMapMain_c::initializeState_EventDungeonMapGet_Step2() {}
void dLytMapMain_c::executeState_EventDungeonMapGet_Step2() {}
void dLytMapMain_c::finalizeState_EventDungeonMapGet_Step2() {}

void dLytMapMain_c::initializeState_EventDungeonMapGet_Step3() {}
void dLytMapMain_c::executeState_EventDungeonMapGet_Step3() {}
void dLytMapMain_c::finalizeState_EventDungeonMapGet_Step3() {}

void dLytMapMain_c::initializeState_EventFieldMapChange_Step1() {}
void dLytMapMain_c::executeState_EventFieldMapChange_Step1() {}
void dLytMapMain_c::finalizeState_EventFieldMapChange_Step1() {}

void dLytMapMain_c::initializeState_EventFieldMapChange_Step2() {}
void dLytMapMain_c::executeState_EventFieldMapChange_Step2() {}
void dLytMapMain_c::finalizeState_EventFieldMapChange_Step2() {}

void dLytMapMain_c::initializeState_EventFieldMapChange_Step3() {}
void dLytMapMain_c::executeState_EventFieldMapChange_Step3() {}
void dLytMapMain_c::finalizeState_EventFieldMapChange_Step3() {}

void dLytMapMain_c::initializeState_EventFieldMapChange_Step4() {}
void dLytMapMain_c::executeState_EventFieldMapChange_Step4() {}
void dLytMapMain_c::finalizeState_EventFieldMapChange_Step4() {}

void dLytMapMain_c::initializeState_EventForestMapChange_Step1() {}
void dLytMapMain_c::executeState_EventForestMapChange_Step1() {}
void dLytMapMain_c::finalizeState_EventForestMapChange_Step1() {}

void dLytMapMain_c::initializeState_EventForestMapChange_Step2() {}
void dLytMapMain_c::executeState_EventForestMapChange_Step2() {}
void dLytMapMain_c::finalizeState_EventForestMapChange_Step2() {}

void dLytMapMain_c::initializeState_EventForestMapChange_Step3() {}
void dLytMapMain_c::executeState_EventForestMapChange_Step3() {}
void dLytMapMain_c::finalizeState_EventForestMapChange_Step3() {}

void dLytMapMain_c::initializeState_EventSignalAdd_Step1() {}
void dLytMapMain_c::executeState_EventSignalAdd_Step1() {}
void dLytMapMain_c::finalizeState_EventSignalAdd_Step1() {}

void dLytMapMain_c::initializeState_EventSignalAdd_Step2() {}
void dLytMapMain_c::executeState_EventSignalAdd_Step2() {}
void dLytMapMain_c::finalizeState_EventSignalAdd_Step2() {}

void dLytMapMain_c::initializeState_EventSignalAdd_Step3() {}
void dLytMapMain_c::executeState_EventSignalAdd_Step3() {}
void dLytMapMain_c::finalizeState_EventSignalAdd_Step3() {}

void dLytMapMain_c::initializeState_EventGoddessCube_Step1() {}
void dLytMapMain_c::executeState_EventGoddessCube_Step1() {}
void dLytMapMain_c::finalizeState_EventGoddessCube_Step1() {}

void dLytMapMain_c::initializeState_EventGoddessCube_Step2() {}
void dLytMapMain_c::executeState_EventGoddessCube_Step2() {}
void dLytMapMain_c::finalizeState_EventGoddessCube_Step2() {}

void dLytMapMain_c::initializeState_EventGoddessCube_Step3() {}
void dLytMapMain_c::executeState_EventGoddessCube_Step3() {}
void dLytMapMain_c::finalizeState_EventGoddessCube_Step3() {}

void dLytMapMain_c::initializeState_EventSaveObjMsgWindow() {}
void dLytMapMain_c::executeState_EventSaveObjMsgWindow() {}
void dLytMapMain_c::finalizeState_EventSaveObjMsgWindow() {}

void dLytMapMain_c::initializeState_EventSaveObjSelect() {}
void dLytMapMain_c::executeState_EventSaveObjSelect() {}
void dLytMapMain_c::finalizeState_EventSaveObjSelect() {}

void dLytMapMain_c::initializeState_EventSaveObjConfirmMsgWindow() {}
void dLytMapMain_c::executeState_EventSaveObjConfirmMsgWindow() {}
void dLytMapMain_c::finalizeState_EventSaveObjConfirmMsgWindow() {}

void dLytMapMain_c::initializeState_EventSaveObjDecide() {}
void dLytMapMain_c::executeState_EventSaveObjDecide() {}
void dLytMapMain_c::finalizeState_EventSaveObjDecide() {}
