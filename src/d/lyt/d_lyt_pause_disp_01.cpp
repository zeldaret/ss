#include "d/lyt/d_lyt_pause_disp_01.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/d_cs_base.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_pad_nav.h"
#include "d/d_rumble.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "d/lyt/d_lyt_common_icon_material.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_pause.h"
#include "d/lyt/d_textbox.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_group.h"
#include "nw4r/math/math_types.h"
#include "rvl/MTX/mtxvec.h"
#include "sized_string.h"
#include "toBeSorted/counters/counter.h"
#include "toBeSorted/d_emitter.h"

STATE_DEFINE(dLytPauseDisp01_c, None);
STATE_DEFINE(dLytPauseDisp01_c, In);
STATE_DEFINE(dLytPauseDisp01_c, Wait);
STATE_DEFINE(dLytPauseDisp01_c, Select);
STATE_DEFINE(dLytPauseDisp01_c, GetDemo);
STATE_DEFINE(dLytPauseDisp01_c, Out);

static const d2d::LytBrlanMapping brlanMap[] = {
    {        "pause_01_in.brlan",    "G_inOut_00"},
    {     "pause_01_onOff.brlan",  "G_tegamiB_00"},
    {     "pause_01_onOff.brlan", "G_terryBug_00"},
    {     "pause_01_onOff.brlan", "G_garagara_00"},
    {     "pause_01_onOff.brlan",    "G_genki_00"},
    {       "pause_01_out.brlan",    "G_inOut_00"},
    {      "pause_01_have.brlan",  "G_tegamiB_00"},
    {      "pause_01_have.brlan", "G_terryBug_00"},
    {      "pause_01_have.brlan", "G_garagara_00"},
    {      "pause_01_have.brlan",    "G_genki_00"},
    {      "pause_01_loop.brlan",     "G_loop_00"},
    { "pause_01_scrollRIn.brlan",   "G_scroll_00"},
    {"pause_01_scrollROut.brlan",   "G_scroll_00"},
    { "pause_01_scrollLIn.brlan",   "G_scroll_00"},
    {"pause_01_scrollLOut.brlan",   "G_scroll_00"},
};

#define PAUSE_DISP_01_ANIM_IN 0
#define PAUSE_DISP_01_ANIM_ONOFF_TEGAMI 1
#define PAUSE_DISP_01_ANIM_ONOFF_TERRY_BUG 2
#define PAUSE_DISP_01_ANIM_ONOFF_GARAGARA 3
#define PAUSE_DISP_01_ANIM_ONOFF_GENKI 4
#define PAUSE_DISP_01_ANIM_OUT 5
#define PAUSE_DISP_01_ANIM_HAVE_TEGAMI 6
#define PAUSE_DISP_01_ANIM_HAVE_TERRY_BUG 7
#define PAUSE_DISP_01_ANIM_HAVE_GARAGARA 8
#define PAUSE_DISP_01_ANIM_HAVE_GENKI 9
#define PAUSE_DISP_01_ANIM_LOOP 10
#define PAUSE_DISP_01_ANIM_SCROLL_R_IN 11
#define PAUSE_DISP_01_ANIM_SCROLL_R_OUT 12
#define PAUSE_DISP_01_ANIM_SCROLL_L_IN 13
#define PAUSE_DISP_01_ANIM_SCROLL_L_OUT 14

#define PAUSE_DISP_01_ANIM_OFFSET_ONOFF PAUSE_DISP_01_ANIM_ONOFF_TEGAMI
#define PAUSE_DISP_01_ANIM_OFFSET_HAVE PAUSE_DISP_01_ANIM_HAVE_TEGAMI
#define PAUSE_DISP_01_NUM_ANIMS 15

#define PAUSE_DISP_01_NUM_SUBPANES 28

#define PAUSE_DISP_01_OFFSET_INSECTS 0
#define PAUSE_DISP_01_NUM_INSECTS 12
#define PAUSE_DISP_01_OFFSET_MATERIALS PAUSE_DISP_01_NUM_INSECTS
#define PAUSE_DISP_01_NUM_MATERIALS 16

static const char *sGroupName = "G_ref_00";

static const char *sPaneNames[] = {
    "B_tegamiB_00", "B_terryBug_00", "B_garagara_00", "B_genki_00", nullptr, nullptr,
};

#define PAUSE_DISP_01_PANE_TEGAMI 0
#define PAUSE_DISP_01_PANE_TERRY_BUG 1
#define PAUSE_DISP_01_PANE_GARAGARA 2
#define PAUSE_DISP_01_PANE_GENKI 3
#define PAUSE_DISP_01_PANE_LEFT 4
#define PAUSE_DISP_01_PANE_RIGHT 5

#define PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES 4
#define PAUSE_DISP_01_NUM_MY_PANES 6

#define PAUSE_DISP_01_NUM_COMBINED_PANES (PAUSE_DISP_01_NUM_MY_PANES + PAUSE_DISP_01_NUM_SUBPANES)
#define PAUSE_DISP_01_OFFSET_MY_PANES PAUSE_DISP_01_NUM_SUBPANES

dLytPauseDisp01_c::dLytPauseDisp01_c() : mStateMgr(*this, sStateID::null) {}

bool dLytPauseDisp01_c::build() {
    dLytPauseMgr_c *pauseMgr = dLytPauseMgr_c::GetInstance();
    d2d::ResAccIf_c *resAcc = pauseMgr->getResAcc1();
    mLyt.setResAcc(resAcc);
    mLyt.build("pause_01.brlyt", nullptr);
    mLyt.setPriority(0x86);

    for (int i = 0; i < PAUSE_DISP_01_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMap[i].mFile, resAcc, mLyt.getLayout(), brlanMap[i].mName);
    }

    resAcc = pauseMgr->getResAcc2();

    for (int i = 0; i < PAUSE_DISP_01_NUM_SUBPANES; i++) {
        if (i < PAUSE_DISP_01_NUM_INSECTS) {
            mIcons[i].build(resAcc, 0);
        } else {
            mIcons[i].build(resAcc, 1);
        }
        mSubpanes[i].mpLytPane = &mIcons[i];
        mSubpaneList.PushBack(&mSubpanes[i]);
    }

    if (mLyt.getLayout()->GetGroupContainer() != nullptr) {
        nw4r::lyt::Group *g = mLyt.getLayout()->GetGroupContainer()->FindGroupByName(sGroupName);
        if (g != nullptr) {
            d2d::dSubPane::linkMeters(g, &mSubpaneList);
        }
    }

    mCsHitCheck.init(mLyt.getLayout()->GetRootPane(), 1, 0, 0);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitCheck);

    for (int i = 0; i < PAUSE_DISP_01_NUM_MY_PANES; i++) {
        if (i < PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES) {
            mpPanes[i] = mLyt.findBounding(sPaneNames[i]);
            mpPanes[i]->SetVisible(true);
        } else {
            mpPanes[i] = pauseMgr->getArrowBounding(i - PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES);
        }
    }

    mLyt.calc();
    mStateMgr.changeState(StateID_None);

    return true;
}

bool dLytPauseDisp01_c::remove() {
    dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitCheck);
    for (d2d::SubPaneList::Iterator it = mSubpaneList.GetBeginIter(); it != mSubpaneList.GetEndIter(); ++it) {
        d2d::dSubPane *subPane = it->mpLytPane;
        if (subPane != nullptr) {
            nw4r::lyt::Pane *parent = subPane->getPane()->GetParent();
            parent->RemoveChild(subPane->getPane());
        }
    }
    for (int i = 0; i < PAUSE_DISP_01_NUM_SUBPANES; i++) {
        mSubpanes[i].mpLytPane->remove();
    }

    mLyt.unbindAnims();

    for (int i = 0; i < PAUSE_DISP_01_NUM_ANIMS; i++) {
        mAnm[i].remove();
    }

    return true;
}

bool dLytPauseDisp01_c::execute() {
    mHasSelection = false;
    mStateMgr.executeState();
    for (int i = 0; i < PAUSE_DISP_01_NUM_SUBPANES; i++) {
        mSubpanes[i].mpLytPane->execute();
    }
    mLyt.calc();
    mCsHitCheck.resetCachedHitboxes();
    mCsHitCheck.execute();
    if (!mStateMgr.getStateID()->isEqual(StateID_None)) {
        mAnm[PAUSE_DISP_01_ANIM_LOOP].play();
    }

    return true;
}

bool dLytPauseDisp01_c::draw() {
    if (mIsVisible == true) {
        mLyt.addToDrawList();
    }
    return true;
}

void dLytPauseDisp01_c::drawDirectly() {
    mLyt.draw();
}

void dLytPauseDisp01_c::requestIn(bool scroll) {
    mInRequest = true;
    mDoScrollAnim = scroll;
}

void dLytPauseDisp01_c::requestOut(bool scroll) {
    mOutRequest = true;
    mDoScrollAnim = scroll;
}

void dLytPauseDisp01_c::requestSelect() {
    mSelectToggle = true;
}

void dLytPauseDisp01_c::requestUnselect() {
    mSelectToggle = true;
}

void dLytPauseDisp01_c::initializeState_None() {
    for (int i = 0; i < PAUSE_DISP_01_NUM_ANIMS; i++) {
        mAnm[i].unbind();
    }

    mIsChangingState = false;
    mInRequest = false;
    mOutRequest = false;
    mIsVisible = false;
    mHasSelection = false;
    mStep = 0;
    mPrevNavTarget = 0;
    mCurrentNavTarget = 0;
    mGetDemoTimer = 0;

    for (int i = 0; i < PAUSE_DISP_01_NUM_SUBPANES; i++) {
        mIcons[i].reset();
        mIcons[i].setVisible(true);
    }

    setAnm(PAUSE_DISP_01_ANIM_ONOFF_TEGAMI, 0.0f);
    setAnm(PAUSE_DISP_01_ANIM_ONOFF_TERRY_BUG, 0.0f);
    setAnm(PAUSE_DISP_01_ANIM_ONOFF_GARAGARA, 0.0f);
    setAnm(PAUSE_DISP_01_ANIM_ONOFF_GENKI, 0.0f);

    for (int i = 0; i < PAUSE_DISP_01_NUM_MY_PANES; i++) {
        if (i < PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES) {
            mpPanes[i]->SetVisible(false);
        }
    }

    setAnm(PAUSE_DISP_01_ANIM_LOOP, 0.0f);
}
void dLytPauseDisp01_c::executeState_None() {
    if (mInRequest == true) {
        mInRequest = false;
        mStateMgr.changeState(StateID_In);
    }
}
void dLytPauseDisp01_c::finalizeState_None() {}

void dLytPauseDisp01_c::initializeState_In() {
    dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
    if (mDoScrollAnim == true) {
        setAnm(PAUSE_DISP_01_ANIM_IN, 0.0f);
        mAnm[PAUSE_DISP_01_ANIM_IN].setFrame(mAnm[PAUSE_DISP_01_ANIM_IN].getAnimDuration());
        if (pause->isNavLeft()) {
            setAnm(PAUSE_DISP_01_ANIM_SCROLL_L_IN, 0.0f);
        } else {
            setAnm(PAUSE_DISP_01_ANIM_SCROLL_R_IN, 0.0f);
        }

        if (pause->getField_0x0840()) {
            if (pause->isNavLeft()) {
                mPrevNavTarget = PAUSE_DISP_01_OFFSET_MY_PANES + PAUSE_DISP_01_PANE_LEFT + 1;
            } else {
                mPrevNavTarget = PAUSE_DISP_01_OFFSET_MY_PANES + PAUSE_DISP_01_PANE_RIGHT + 1;
            }
        }
    } else {
        setAnm(PAUSE_DISP_01_ANIM_IN, 0.0f);
    }
    mIsVisible = true;
    setupDisp();
}
void dLytPauseDisp01_c::executeState_In() {
    dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();

    s32 anim = PAUSE_DISP_01_ANIM_IN;
    if (mDoScrollAnim == true) {
        anim = pause->isNavLeft() ? PAUSE_DISP_01_ANIM_SCROLL_L_IN : PAUSE_DISP_01_ANIM_SCROLL_R_IN;
    }

    d2d::AnmGroup_c &anm = mAnm[anim];

    if (anm.isEndReached() == true) {
        if (dLytControlGame_c::getInstance()->isPauseDemo()) {
            mStateMgr.changeState(StateID_GetDemo);
        } else {
            mStateMgr.changeState(StateID_Wait);
        }
    } else {
        anm.play();
    }
}
void dLytPauseDisp01_c::finalizeState_In() {
    dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
    if (pause->isNavLeft()) {
        stopAnm(PAUSE_DISP_01_ANIM_SCROLL_L_IN);
    } else {
        stopAnm(PAUSE_DISP_01_ANIM_SCROLL_R_IN);
    }

    mDoScrollAnim = false;
    if (pause->getField_0x083E()) {
        if (!pause->getField_0x0832()) {
            mCurrentNavTarget = PAUSE_DISP_01_OFFSET_MY_PANES + PAUSE_DISP_01_PANE_RIGHT + 1;
        } else {
            mCurrentNavTarget = PAUSE_DISP_01_OFFSET_MY_PANES + PAUSE_DISP_01_PANE_LEFT + 1;
        }
        mPrevNavTarget = mCurrentNavTarget;
    }
}

void dLytPauseDisp01_c::initializeState_Wait() {
    mIsChangingState = true;
    showInsectsAndMaterials();
}
void dLytPauseDisp01_c::executeState_Wait() {
    mIsChangingState = false;
    if (mOutRequest == true) {
        mOutRequest = false;
        hideInsectsAndMaterials();
        mStateMgr.changeState(StateID_Out);
    } else if (mSelectToggle == true) {
        mStateMgr.changeState(StateID_Select);
        dSndSmallEffectMgr_c *mgr = dSndSmallEffectMgr_c::GetInstance();
        s32 itemIdx = mCurrentNavTarget - 1;
        if (itemIdx < PAUSE_DISP_01_NUM_INSECTS) {
            mgr->playSound(SE_S_MENU_P2_SELECT_INSECT);
        } else if (itemIdx < PAUSE_DISP_01_NUM_INSECTS + PAUSE_DISP_01_NUM_MATERIALS) {
            mgr->playSound(SE_S_MENU_P2_SELECT_MATERIAL);
        } else if (itemIdx <
                   PAUSE_DISP_01_NUM_INSECTS + PAUSE_DISP_01_NUM_MATERIALS + PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES) {
            mgr->playSound(SE_S_MENU_P2_SELECT_IMPORTANT);
        }

    } else if (updateSelection() != 0) {
        mHasSelection = true;
    }
}
void dLytPauseDisp01_c::finalizeState_Wait() {}

void dLytPauseDisp01_c::initializeState_Select() {
    mSelectToggle = false;
}
void dLytPauseDisp01_c::executeState_Select() {
    if (mSelectToggle == true) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dLytPauseDisp01_c::finalizeState_Select() {
    mSelectToggle = false;
}

void dLytPauseDisp01_c::initializeState_GetDemo() {
    mIsChangingState = true;
    mStep = 0;
    mGetDemoTimer = 0;
}
void dLytPauseDisp01_c::executeState_GetDemo() {
    switch (mStep) {
        case 0: {
            mIsChangingState = false;
            if (mGetDemoTimer < 2) {
                mGetDemoTimer++;
            } else {
                mGetDemoTimer = 0;
                mStep = 1;
            }
            break;
        }
        case 1: {
            s32 thingIdx = 0;
            mStep = 2;
            u16 item = dLytControlGame_c::getInstance()->getItemForPauseDemo();
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_ITEM_SET_FX);
            if (item == ITEM_1_CRYSTAL || item == ITEM_5_CRYSTALS) {
                // gratitude crystal
                mAnm[PAUSE_DISP_01_ANIM_HAVE_GENKI].setFrame(1.0f);
                nw4r::math::MTX34 mtx = mpPanes[PAUSE_DISP_01_PANE_GENKI]->GetGlobalMtx();
                nw4r::math::VEC3 v(0.0f, 0.0f, 0.0f);
                MTXMultVec(mtx, v, v);
                mVec3_c v1;
                v1.set(v.x, v.y, v.z);
                dJEffManager_c::spawnUIEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_989_, v1, nullptr, nullptr, nullptr, nullptr
                );
            } else if (item >= ITEM_HORNET_LARVAE) {
                // material
                s32 thingIdx;
                for (s32 idx = 0; idx < PAUSE_DISP_01_NUM_MATERIALS; idx++) {
                    if (getMaterialItemId(idx) == item) {
                        thingIdx = idx;
                        break;
                    }
                }
                s32 iconIdx = PAUSE_DISP_01_OFFSET_MATERIALS + thingIdx;
                if (dAcItem_c::isPerformingInitialCollection()) {
                    mIcons[iconIdx].setShadow(false);
                    mIcons[iconIdx].setHasNumber(false);
                    s32 count = getMaterialCountByIndex(thingIdx);
                    mIcons[iconIdx].setNumber(count);
                    s32 icon = getMaterialIcon(thingIdx);
                    mIcons[iconIdx].setItem(icon);
                }
                nw4r::math::MTX34 mtx = mIcons[iconIdx].getBounding()->GetGlobalMtx();
                nw4r::math::VEC3 v(0.0f, 0.0f, 0.0f);
                MTXMultVec(mtx, v, v);
                mVec3_c v1;
                v1.set(v.x, v.y, v.z);
                dJEffManager_c::spawnUIEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_989_, v1, nullptr, nullptr, nullptr, nullptr
                );
            } else {
                // insect
                for (s32 idx = 0; idx < PAUSE_DISP_01_NUM_INSECTS; idx++) {
                    if (getInsectItemId(idx) == item) {
                        thingIdx = idx;
                        break;
                    }
                }
                s32 iconIdx = PAUSE_DISP_01_OFFSET_INSECTS + thingIdx;
                if (dAcItem_c::isPerformingInitialCollection()) {
                    mIcons[iconIdx].setShadow(false);
                    mIcons[iconIdx].setHasNumber(false);
                    s32 count = getInsectCountByIndex(thingIdx);
                    mIcons[iconIdx].setNumber(count);
                    s32 icon = getInsectIcon(thingIdx);
                    mIcons[iconIdx].setItem(icon);
                }
                nw4r::math::MTX34 mtx = mIcons[iconIdx].getBounding()->GetGlobalMtx();
                nw4r::math::VEC3 v(0.0f, 0.0f, 0.0f);
                MTXMultVec(mtx, v, v);
                mVec3_c v1;
                v1.set(v.x, v.y, v.z);
                dJEffManager_c::spawnUIEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_989_, v1, nullptr, nullptr, nullptr, nullptr
                );
            }
            break;
        }
        case 2: {
            if (mGetDemoTimer < 35) {
                mGetDemoTimer++;
            } else {
                mGetDemoTimer = 0;
                mStep = 3;
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MENU_ITEM_SET_COUNT_UP);
                u16 item = dLytControlGame_c::getInstance()->getItemForPauseDemo();
                s32 thingIdx = 0;
                if (item == ITEM_1_CRYSTAL || item == ITEM_5_CRYSTALS) {
                    dTextBox_c *box;
                    s32 count = dAcItem_c::getGratitudeCrystalCount();
                    SizedWString<32> buf;
                    buf.sprintf(L"%d", count);
                    box = mLyt.getTextBox("T_count_28");
                    box->setTextWithGlobalTextProcessor(buf);
                    box->SetVisible(true);
                    box = mLyt.getTextBox("T_countS_28");
                    box->setTextWithGlobalTextProcessor(buf);
                    box->SetVisible(true);
                } else if (item >= ITEM_HORNET_LARVAE) {
                    // material
                    for (s32 idx = 0; idx < PAUSE_DISP_01_NUM_MATERIALS; idx++) {
                        if (getMaterialItemId(idx) == item) {
                            thingIdx = idx;
                            break;
                        }
                    }
                    s32 iconIdx = PAUSE_DISP_01_OFFSET_MATERIALS + thingIdx;
                    mIcons[iconIdx].setShadow(false);
                    mIcons[iconIdx].setHasNumber(true);
                    s32 count = getMaterialCountByIndex(thingIdx);
                    mIcons[iconIdx].setNumber(count);
                    s32 icon = getMaterialIcon(thingIdx);
                    mIcons[iconIdx].setItem(icon);
                } else {
                    // insect
                    for (s32 idx = 0; idx < PAUSE_DISP_01_NUM_INSECTS; idx++) {
                        if (getInsectItemId(idx) == item) {
                            thingIdx = idx;
                            break;
                        }
                    }
                    s32 iconIdx = PAUSE_DISP_01_OFFSET_INSECTS + thingIdx;
                    mIcons[iconIdx].setShadow(false);
                    mIcons[iconIdx].setHasNumber(true);
                    s32 count = getInsectCountByIndex(thingIdx);
                    mIcons[iconIdx].setNumber(count);
                    s32 icon = getInsectIcon(thingIdx);
                    mIcons[iconIdx].setItem(icon);
                }
            }
            break;
        }
        case 3: {
            if (mGetDemoTimer < 37) {
                mGetDemoTimer++;
            } else {
                mGetDemoTimer = 0;
                mStateMgr.changeState(StateID_Wait);
            }
            break;
        }
    }
}
void dLytPauseDisp01_c::finalizeState_GetDemo() {}

void dLytPauseDisp01_c::initializeState_Out() {
    stopAnm(PAUSE_DISP_01_ANIM_IN);
    if (mDoScrollAnim == true) {
        if (dLytPauseMgr_c::GetInstance()->isNavLeft()) {
            setAnm(PAUSE_DISP_01_ANIM_SCROLL_R_OUT, 0.0f);
        } else {
            setAnm(PAUSE_DISP_01_ANIM_SCROLL_L_OUT, 0.0f);
        }
    } else {
        setAnm(PAUSE_DISP_01_ANIM_OUT, 0.0f);
    }
    mStep = 0;
}
void dLytPauseDisp01_c::executeState_Out() {
    s32 anim = PAUSE_DISP_01_ANIM_OUT;
    if (mDoScrollAnim == true) {
        anim = dLytPauseMgr_c::GetInstance()->isNavLeft() ? PAUSE_DISP_01_ANIM_SCROLL_R_OUT :
                                                            PAUSE_DISP_01_ANIM_SCROLL_L_OUT;
    }

    d2d::AnmGroup_c &anm = mAnm[anim];

    switch (mStep) {
        case 0: {
            if (anm.isEndReached() == true) {
                mStep = 1;
                mIsChangingState = true;
            }
            break;
        }
        case 1: {
            mStateMgr.changeState(StateID_None);
            return;
        }
    }

    anm.play();
}
void dLytPauseDisp01_c::finalizeState_Out() {}

void dLytPauseDisp01_c::setAnm(int idx, f32 value) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.setAnimEnable(true);
    anm.bind(false);
    anm.setFrame(value);
}

void dLytPauseDisp01_c::stopAnm(int idx) {
    d2d::AnmGroup_c &anm = mAnm[idx];
    anm.unbind();
}

void dLytPauseDisp01_c::setupDisp() {
    static const char *sPaneTable[] = {
        "N_bug_00",   "N_bug_01",   "N_bug_02",   "N_bug_03",   "N_bug_04",   "N_bug_05",   "N_bug_06",
        "N_bug_07",   "N_bug_08",   "N_bug_09",   "N_bug_10",   "N_bug_11",   "N_sozai_00", "N_sozai_01",
        "N_sozai_02", "N_sozai_03", "N_sozai_04", "N_sozai_05", "N_sozai_06", "N_sozai_07", "N_sozai_08",
        "N_sozai_09", "N_sozai_10", "N_sozai_11", "N_sozai_12", "N_sozai_13", "N_sozai_14", "N_sozai_15",
    };

    // Fixes a regswap...
    const char **name = sPaneTable;
    for (int i = 0; i < PAUSE_DISP_01_NUM_SUBPANES; i++) {
        mLyt.findPane(*name)->SetVisible(true);
        name++;
    }

    setupInsects();
    setupMaterials();

    if (StoryflagManager::sInstance->getFlag(552) || StoryflagManager::sInstance->getFlag(555)) {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_TEGAMI, 2.0f);
        mpPanes[PAUSE_DISP_01_PANE_TEGAMI]->SetVisible(true);
    } else if (dAcItem_c::checkFlag(ITEM_CAWLIN_LETTER)) {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_TEGAMI, 1.0f);
        mpPanes[PAUSE_DISP_01_PANE_TEGAMI]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_TEGAMI, 0.0f);
    }

    if (StoryflagManager::sInstance->getFlag(477)) {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_TERRY_BUG, 2.0f);
        mpPanes[PAUSE_DISP_01_PANE_TERRY_BUG]->SetVisible(true);
    } else if (StoryflagManager::sInstance->getFlag(476)) {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_TERRY_BUG, 1.0f);
        mpPanes[PAUSE_DISP_01_PANE_TERRY_BUG]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_TERRY_BUG, 0.0f);
    }

    if (StoryflagManager::sInstance->getFlag(387)) {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_GARAGARA, 2.0f);
        mpPanes[PAUSE_DISP_01_PANE_GARAGARA]->SetVisible(true);
    } else if (dAcItem_c::checkFlag(ITEM_RATTLE)) {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_GARAGARA, 1.0f);
        mpPanes[PAUSE_DISP_01_PANE_GARAGARA]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_GARAGARA, 0.0f);
    }

    dLytControlGame_c *lytControl = dLytControlGame_c::getInstance();
    u16 demoItem = lytControl->getItemForPauseDemo();
    u32 crystalCount = dAcItem_c::getGratitudeCrystalCount();
    if (crystalCount != 0) {
        bool isDemo = false;
        if (lytControl->isPauseDemo() && (demoItem == ITEM_1_CRYSTAL || demoItem == ITEM_5_CRYSTALS)) {
            isDemo = true;
            crystalCount = demoItem == ITEM_1_CRYSTAL ? crystalCount - 1 : crystalCount - 5;
        }

        if (StoryflagManager::sInstance->getFlag(360)) {
            setAnm(PAUSE_DISP_01_ANIM_HAVE_GENKI, 2.0f);
        } else if (isDemo && crystalCount == 0) {
            setAnm(PAUSE_DISP_01_ANIM_HAVE_GENKI, 0.0f);
        } else {
            setAnm(PAUSE_DISP_01_ANIM_HAVE_GENKI, 1.0f);
        }

        SizedWString<32> buf;
        buf.sprintf(L"%d", crystalCount);
        dTextBox_c *box;
        box = mLyt.getTextBox("T_count_28");
        box->setTextWithGlobalTextProcessor(buf);
        if (isDemo && crystalCount == 0) {
            box->SetVisible(false);
        }

        box = mLyt.getTextBox("T_countS_28");
        box->setTextWithGlobalTextProcessor(buf);
        if (isDemo && crystalCount == 0) {
            box->SetVisible(false);
        }

        mpPanes[PAUSE_DISP_01_PANE_GENKI]->SetVisible(true);
    } else {
        setAnm(PAUSE_DISP_01_ANIM_HAVE_GENKI, 0.0f);
    }
}

void dLytPauseDisp01_c::setupInsects() {
    dLytControlGame_c *lytControl = dLytControlGame_c::getInstance();
    int insectIdx = 0;
    for (int i = PAUSE_DISP_01_OFFSET_INSECTS; i < PAUSE_DISP_01_OFFSET_INSECTS + PAUSE_DISP_01_NUM_INSECTS; i++) {
        mIcons[i].setBg(true);
        if (shouldInsectBeDisplayed(insectIdx)) {
            mIcons[i].setShadow(false);
            mIcons[i].setHasNumber(true);
        } else {
            mIcons[i].setShadow(true);
            mIcons[i].setHasNumber(false);
        }

        s32 count = getInsectCountByIndex(insectIdx);
        if (lytControl->isPauseDemo() && lytControl->getItemForPauseDemo() == getInsectItemId(insectIdx)) {
            count = lytControl->getItemCountForPauseDemo();
        }

        mIcons[i].setNumber(count);
        s32 icon = getInsectIcon(insectIdx);
        mIcons[i].setItem(icon);

        insectIdx++;
    }
}

void dLytPauseDisp01_c::setupMaterials() {
    dLytControlGame_c *lytControl = dLytControlGame_c::getInstance();
    int materialIdx = 0;
    for (int i = PAUSE_DISP_01_OFFSET_MATERIALS; i < PAUSE_DISP_01_OFFSET_MATERIALS + PAUSE_DISP_01_NUM_MATERIALS;
         i++) {
        mIcons[i].setBg(true);
        if (shouldMaterialBeDisplayed(materialIdx)) {
            mIcons[i].setShadow(false);
            mIcons[i].setHasNumber(true);
        } else {
            mIcons[i].setShadow(true);
            mIcons[i].setHasNumber(false);
        }

        s32 count = getMaterialCountByIndex(materialIdx);
        if (lytControl->isPauseDemo() && lytControl->getItemForPauseDemo() == getMaterialItemId(materialIdx)) {
            count = lytControl->getItemCountForPauseDemo();
        }

        mIcons[i].setNumber(count);
        s32 icon = getMaterialIcon(materialIdx);
        mIcons[i].setItem(icon);

        materialIdx++;
    }
}

void dLytPauseDisp01_c::showInsectsAndMaterials() {
    for (int i = PAUSE_DISP_01_OFFSET_INSECTS; i < PAUSE_DISP_01_OFFSET_INSECTS + PAUSE_DISP_01_NUM_INSECTS; i++) {
        mIcons[i].setVisible(true);
    }

    for (int i = PAUSE_DISP_01_OFFSET_MATERIALS; i < PAUSE_DISP_01_OFFSET_MATERIALS + PAUSE_DISP_01_NUM_MATERIALS;
         i++) {
        mIcons[i].setVisible(true);
    }
}

// A matrix controlling navigation with the Nunchuk stick.
// The row index is the currently selected target, and the row contains
// the list of targets, one entry per stick direction  (starting up and going clockwise)
//
// If someone was really bored they could probably replace all of these
// with constants...
static const u8 sNavTable[34][8] = {
    { 0,  2,  2,  6,  5,  5, 33,  0},
    { 0,  3,  3,  7,  6,  5,  1,  1},
    { 0,  4,  4,  8,  7,  6,  2,  2},
    { 0, 13, 13, 17,  8,  7,  3,  3},
    { 1,  2,  6, 10,  9, 33, 33,  1},
    { 2,  3,  7, 11, 10,  9,  5,  1},
    { 3,  4,  8, 12, 11, 10,  6,  2},
    { 4, 13, 17, 21, 12, 11,  7,  3},
    { 5,  6, 10, 30, 29, 29, 33, 33},
    { 6,  7, 11, 31, 30, 29,  9,  5},
    { 7,  8, 12, 32, 31, 30, 10,  6},
    { 8, 17, 21, 25, 32, 31, 11,  7},
    { 0, 14, 14, 18, 17,  8,  4,  4},
    { 0, 15, 15, 19, 18, 17, 13, 13},
    { 0, 16, 16, 20, 19, 18, 14, 14},
    { 0,  0, 34, 20, 20, 19, 15, 15},
    {13, 14, 18, 22, 21, 12,  8,  4},
    {14, 15, 19, 23, 22, 21, 17, 13},
    {15, 16, 20, 24, 23, 22, 18, 14},
    {16, 16, 34, 34, 24, 23, 19, 15},
    {17, 18, 22, 26, 25, 32, 12,  8},
    {18, 19, 23, 27, 26, 25, 21, 17},
    {19, 20, 24, 28, 27, 26, 22, 18},
    {20, 34, 34, 28, 28, 27, 23, 19},
    {21, 22, 26, 26,  0, 32, 32, 12},
    {22, 23, 27, 27,  0, 25, 25, 21},
    {23, 24, 28, 28,  0, 26, 26, 22},
    {24, 24, 34,  0,  0, 27, 27, 23},
    { 9, 10, 30, 30,  0,  0, 33,  9},
    {10, 11, 31, 31,  0, 29, 29,  9},
    {11, 12, 32, 32,  0, 30, 30, 10},
    {12, 21, 25, 25,  0, 31, 31, 11},
    { 1,  1,  5,  9,  9,  0,  0,  0},
    {16,  0,  0,  0, 24, 24, 20, 16},
};

s32 dLytPauseDisp01_c::updateSelection() {
    dLytPauseMgr_c *pause = dLytPauseMgr_c::GetInstance();
    if (!pause->isStateWait()) {
        pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
        return 0;
    }

    s32 target = 0;
    if (dPadNav::isPointerVisible()) {
        for (int i = 0; i < PAUSE_DISP_01_NUM_SUBPANES; i++) {
            if (mIcons[i].isCursorOver()) {
                target = i + 1;
                // missed optimization
                // break;
            }
        }
        if (target == 0) {
            s32 myPane = getPointerPane();
            if (myPane >= 0) {
                target = myPane + PAUSE_DISP_01_OFFSET_MY_PANES + 1;
            }
        }
    } else {
        if (mCurrentNavTarget != 0) {
            if (!dPadNav::isPrevPointerVisible() && dPadNav::getFSStickNavDirection() != dPadNav::FS_STICK_NONE) {
                const u8 *directions = sNavTable[mCurrentNavTarget - 1];
                s32 newTarget = directions[dPadNav::getFSStickNavDirection() - 1];
                if (newTarget != 0) {
                    mCurrentNavTarget = newTarget;
                }
            }
        } else {
            mCurrentNavTarget = PAUSE_DISP_01_OFFSET_MY_PANES + PAUSE_DISP_01_PANE_TEGAMI + 1;
        }

        target = mCurrentNavTarget;
    }

    // Cursor outline
    if (target != 0) {
        if (target - 1 >= PAUSE_DISP_01_OFFSET_MY_PANES + PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES) {
            // left-right boundings
            s32 paneIdx = target - PAUSE_DISP_01_OFFSET_MY_PANES - 1;
            dCsBase_c::GetInstance()->setCursorStickTargetPane(mpPanes[paneIdx]);
            pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
            pause->setSelectedArrowBounding(
                target - (PAUSE_DISP_01_OFFSET_MY_PANES + PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES + 1)
            );
        } else if (target - 1 >= PAUSE_DISP_01_NUM_SUBPANES) {
            // quest items
            s32 paneIdx = target - PAUSE_DISP_01_NUM_SUBPANES - 1;
            dCsBase_c::GetInstance()->setCursorStickTargetPane(mpPanes[paneIdx]);
            s32 itemId = getQuestItemId(target - PAUSE_DISP_01_NUM_SUBPANES - 1);
            pause->setSelection(dLytPauseMgr_c::SELECT_QUEST_MATERIAL, itemId, false);
        } else {
            const nw4r::lyt::Bounding *bounding = mIcons[target - 1].getBounding();
            dCsBase_c::GetInstance()->setCursorStickTargetPane(bounding);

            if (target - 1 >= PAUSE_DISP_01_OFFSET_MATERIALS) {
                // materials
                s32 paneIdx = target - PAUSE_DISP_01_OFFSET_MATERIALS - 1;
                s32 itemId = getMaterialItemId(paneIdx);
                pause->setSelection(dLytPauseMgr_c::SELECT_QUEST_MATERIAL, itemId, false);
            } else {
                // insects
                s32 paneIdx = target - PAUSE_DISP_01_OFFSET_INSECTS - 1;
                s32 itemId = getInsectItemId(paneIdx);
                pause->setSelection(dLytPauseMgr_c::SELECT_INSECT, itemId, false);
            }
        }
    }

    // make the selected item slightly larger
    for (int i = 0; i < PAUSE_DISP_01_NUM_SUBPANES; i++) {
        if (target != 0 && target - 1 == i) {
            if (i >= PAUSE_DISP_01_OFFSET_MATERIALS) {
                if (shouldMaterialBeDisplayed(i - PAUSE_DISP_01_OFFSET_MATERIALS)) {
                    mIcons[i].setOn();
                } else {
                    mIcons[i].setOff();
                }
            } else {
                if (shouldInsectBeDisplayed(i)) {
                    mIcons[i].setOn();
                } else {
                    mIcons[i].setOff();
                }
            }
        } else {
            mIcons[i].setOff();
        }
    }

    // TODO - loop strength reduction doesn't seem to work here
    int i;
    d2d::AnmGroup_c *anm = &mAnm[PAUSE_DISP_01_ANIM_OFFSET_ONOFF];
    for (i = 0; i < PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES; i++) {
        if (target == i + PAUSE_DISP_01_OFFSET_MY_PANES + 1) {
            anm->play();
        } else {
            f32 frame = anm->getFrame();
            if (frame) {
                frame -= 1.0f;
                if (frame <= 0.0f) {
                    frame = 0.0f;
                }
                anm->setFrame(frame);
            }
        }
        anm++;
    }

    if (target != 0) {
        mCurrentNavTarget = target;
    }

    if (target != 0 && mPrevNavTarget != target) {
        dRumble_c::start(dRumble_c::sRumblePreset1, 0x1);

        dSndSmallEffectMgr_c *mgr = dSndSmallEffectMgr_c::GetInstance();
        if (target - 1 < PAUSE_DISP_01_OFFSET_MATERIALS) {
            // insect
            mgr->playSound(SE_S_MENU_P2_POINT_INSECT);
        } else if (target - 1 < PAUSE_DISP_01_OFFSET_MY_PANES) {
            // materials
            mgr->playSound(SE_S_MENU_P2_POINT_MATERIAL);
        } else if (target - 1 < PAUSE_DISP_01_OFFSET_MY_PANES + PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES) {
            // quest
            mgr->playSound(SE_S_MENU_P2_POINT_IMPORTANT);
        }
    }

    mPrevNavTarget = target;
    if (target == 0) {
        pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
    }

    if (target != 0) {
        s32 pane = target - 1;
        if (pane < PAUSE_DISP_01_NUM_SUBPANES) {
            if (pane < PAUSE_DISP_01_OFFSET_MATERIALS) {
                if (!shouldInsectBeDisplayed(pane)) {
                    target = 0;
                }
            } else {
                if (!shouldMaterialBeDisplayed(pane - PAUSE_DISP_01_OFFSET_MATERIALS)) {
                    target = 0;
                }
            }
        } else if (pane >= PAUSE_DISP_01_OFFSET_MY_PANES + PAUSE_DISP_01_NUM_SPECIAL_ICON_PANES) {
            pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
            target = 0;
        } else {
            s32 paneIdx = pane - PAUSE_DISP_01_OFFSET_MY_PANES;
            if (!mpPanes[paneIdx]->IsVisible()) {
                pause->setSelection(dLytPauseMgr_c::SELECT_NONE, 0, false);
                target = 0;
            }
        }
    }

    return target;
}

s32 dLytPauseDisp01_c::getPointerPane() const {
    int idx = -1;
    dCursorHitCheck_c *d = dCsBase_c::GetInstance()->getHitCheck();
    if (d != nullptr && d->getType() == 'lyt ') {
        for (int i = 0; i < PAUSE_DISP_01_NUM_MY_PANES; i++) {
            if (static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == mpPanes[i]) {
                idx = i;
                // missed optimization:
                // break;
            }
        }
    }
    return idx;
}

void dLytPauseDisp01_c::hideInsectsAndMaterials() {
    for (int i = 0; i < PAUSE_DISP_01_NUM_SUBPANES; i++) {
        mIcons[i].setOff();
        mIcons[i].setVisible(false);
    }
}

bool dLytPauseDisp01_c::shouldInsectBeDisplayed(s32 insectIdx) const {
    if (dLytControlGame_c::getInstance()->isPauseDemo() &&
        dLytControlGame_c::getInstance()->getItemForPauseDemo() == getInsectItemId(insectIdx) &&
        dAcItem_c::isPerformingInitialCollection()) {
        return false;
    }

    if (insectIdx != 0) {
        return dAcItem_c::checkFlag(getInsectItemId(insectIdx));
    } else {
        return StoryflagManager::sInstance->getFlag(122);
    }
}

u16 dLytPauseDisp01_c::getInsectItemId(s32 insectIdx) const {
    static const u16 sInsectItemTable[] = {
        ITEM_DEKU_HORNET,           ITEM_BLESSED_BUTTERFLY, ITEM_GERUDO_DRAGONFLY, ITEM_STARRY_FIREFLY,
        ITEM_WOODLAND_RHINO_BEETLE, ITEM_VOLCANIC_LADYBUG,  ITEM_SAND_CICADA,      ITEM_SKY_STAG_BEETLE,
        ITEM_FARON_GRASSHOPPER,     ITEM_SKYLOFT_MANTIS,    ITEM_LANAYRU_ANT,      ITEM_ELDIN_ROLLER,
    };
    return sInsectItemTable[insectIdx];
}

s32 dLytPauseDisp01_c::getInsectIcon(s32 insectIdx) const {
    static const u8 sInsectIconTable[] = {
        LYT_CMN_BugDekuHornet,    LYT_CMN_BugBlessedButterfly,   LYT_CMN_BugGerudoDragonfly,
        LYT_CMN_BugStarryFirefly, LYT_CMN_BugWoodlandRhinoBetle, LYT_CMN_BugVolcanicLadyBug,
        LYT_CMN_BugSandCicada,    LYT_CMN_BugSkyStagBeetle,      LYT_CMN_BugFaronGrasshopper,
        LYT_CMN_BugSkyloftMantis, LYT_CMN_BugLanayruAnt,         LYT_CMN_BugEldinRoller,
    };
    return sInsectIconTable[insectIdx];
}

s32 dLytPauseDisp01_c::getInsectCountByIndex(s32 insectIdx) const {
    // TODO - ids
    static const u16 sInsectCounterTable[] = {12, 15, 18, 21, 11, 14, 17, 20, 10, 13, 16, 19};
    return getCounterByIndex(sInsectCounterTable[insectIdx]);
}

u16 dLytPauseDisp01_c::getQuestItemId(s32 questItemIndex) const {
    static const u16 sQuestItemTable[] = {ITEM_CAWLIN_LETTER, ITEM_BEEDLE_INSECT_CAGE, ITEM_RATTLE, ITEM_1_CRYSTAL};
    return sQuestItemTable[questItemIndex];
}

bool dLytPauseDisp01_c::shouldMaterialBeDisplayed(s32 materialIdx) const {
    if (dLytControlGame_c::getInstance()->isPauseDemo() &&
        dLytControlGame_c::getInstance()->getItemForPauseDemo() == getMaterialItemId(materialIdx) &&
        dAcItem_c::isPerformingInitialCollection()) {
        return false;
    }
    return dAcItem_c::checkFlag(getMaterialItemId(materialIdx));
}

u16 dLytPauseDisp01_c::getMaterialItemId(s32 materialIdx) const {
    static const u16 sMaterialItemTable[] = {
        ITEM_HORNET_LARVAE, ITEM_BIRD_FEATHER,      ITEM_TUMBLE_WEED,  ITEM_LIZARD_TAIL,
        ITEM_ELDIN_ORE,     ITEM_ANCIENT_FLOWER,    ITEM_AMBER_RELIC,  ITEM_DUSK_RELIC,
        ITEM_JELLY_BLOB,    ITEM_MONSTER_CLAW,      ITEM_MONSTER_HORN, ITEM_ORNAMENTAL_SKULL,
        ITEM_EVIL_CRYSTAL,  ITEM_BLUE_BIRD_FEATHER, ITEM_GOLDEN_SKULL, ITEM_GODDESS_PLUME,
    };
    return sMaterialItemTable[materialIdx];
}

s32 dLytPauseDisp01_c::getMaterialIcon(s32 materialIdx) const {
    static const u8 sMaterialIconTable[] = {
        LYT_CMN_HORNET_LARVAE, LYT_CMN_BIRD_FEATHER,      LYT_CMN_TUMBLEWEED,   LYT_CMN_LIZALFOS_TAIL,
        LYT_CMN_ELDIN_ORE,     LYT_CMN_ANCIENT_FLOWER,    LYT_CMN_AMBER_RELIC,  LYT_CMN_DUSK_RELOC,
        LYT_CMN_JELLY_BLOB,    LYT_CMN_MONSTER_CLAW,      LYT_CMN_MONSTER_HORN, LYT_CMN_ORNAMENTAL_SKULL,
        LYT_CMN_EVIL_CRYSTAL,  LYT_CMN_BLUE_BIRD_FEATHER, LYT_CMN_GOLDEN_SKULL, LYT_CMN_GODDESS_PLUME,
    };
    return sMaterialIconTable[materialIdx];
}

s32 dLytPauseDisp01_c::getMaterialCountByIndex(s32 materialIdx) const {
    // TODO - ids
    static const u16 sMaterialCounterTable[] = {
        22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
    };
    return getCounterByIndex(sMaterialCounterTable[materialIdx]);
}
