#include "d/lyt/d_lyt_map_markers.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_save.h"
#include "d/a/obj/d_a_obj_tbox.h"
#include "d/d_cs_base.h"
#include "d/d_cursor_hit_check.h"
#include "d/d_gfx.h"
#include "d/d_message.h"
#include "d/d_player.h"
#include "d/d_player_act.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_map.h"
#include "d/lyt/d_lyt_map_global.h"
#include "d/lyt/d_window.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_bounding.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_types.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"

inline mVec3_c vec2ToVec3XY(const mVec2_c &v) {
    return mVec3_c(v.x, v.y, 0.0f);
}

static bool checkHasMap() {
    return DungeonflagManager::sInstance->getCounterOrFlag(2, 8);
}

static bool projectOntoMap(const mVec3_c &position, mVec2_c &result) {
    dLytMapGlobal_c::GetInstance()->projectOntoMap(result, position);
    return true;
}

static const d2d::LytBrlanMapping brlanMapMapPopup[] = {
    {"mapPopup_00_scale.brlan", "G_scale_00"},
    {"mapPopup_00_inOut.brlan", "G_inOut_00"},
};

#define MAP_POPUP_ANIM_SCALE 0
#define MAP_POPUP_ANIM_INOUT 1
#define MAP_POPUP_NUM_ANIMS 2

static const char *sMapPopupPaneNames[] = {
    "N_all_00",
    "N_inOut_00",
};

#define MAP_POPUP_PANE_ALL 0
#define MAP_POPUP_PANE_IN_OUT 1
#define MAP_POPUP_NUM_PANES 2

static const char *sMapPopupTextboxNames[] = {
    "T_popup_00",
    "T_popupS_00",
};

#define MAP_POPUP_NUM_TEXTBOXES 2

static const char *sMapPopupSizeBoxNames[] = {"W_bgP_00"};

bool dLytMapPopup_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("mapPopup_00.brlyt", nullptr);

    for (int i = 0; i < MAP_POPUP_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapMapPopup[i].mFile, resAcc, mLyt.getLayout(), brlanMapMapPopup[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < MAP_POPUP_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sMapPopupPaneNames[i]);
    }

    for (int i = 0; i < MAP_POPUP_NUM_TEXTBOXES; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sMapPopupTextboxNames[i]);
    }

    for (int i = 0; i < 1; i++) {
        mpWindow[i] = mLyt.getWindow(sMapPopupSizeBoxNames[i]);
        mpSizeBox[i] = mLyt.getSizeBoxInWindow(sMapPopupSizeBoxNames[i]);
    }

    realizeText();
    setInitialState();

    mMaxScale = mAnm[MAP_POPUP_ANIM_SCALE].getLastFrame();

    return true;
}

bool dLytMapPopup_c::remove() {
    for (int i = 0; i < MAP_POPUP_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMapPopup_c::execute() {
    checkMapMode();
    updateScale();
    if (mAnm[MAP_POPUP_ANIM_INOUT].isEnabled()) {
        if (mAnm[MAP_POPUP_ANIM_INOUT].isStop2()) {
            if (mAnm[MAP_POPUP_ANIM_INOUT].isPlayingForwardsOnce()) {
                mAnm[MAP_POPUP_ANIM_INOUT].setAnimEnable(false);
            }
            if (mAnm[MAP_POPUP_ANIM_INOUT].isPlayingBackwardsOnce()) {
                field_0x178 = 0;
                field_0x180 = 0x30;
                mAnm[MAP_POPUP_ANIM_INOUT].setAnimEnable(false);
            }
        }
    }

    for (int i = 0; i < MAP_POPUP_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

void dLytMapPopup_c::draw() {
    u8 alpha = dLytMapGlobal_c::GetInstance()->getAlpha();
    mpPanes[MAP_POPUP_PANE_ALL]->SetAlpha(alpha);
    mpPanes[MAP_POPUP_PANE_ALL]->SetTranslate(vec2ToVec3XY(mPosition));
    mpPanes[MAP_POPUP_PANE_ALL]->Animate(0);
    mpPanes[MAP_POPUP_PANE_ALL]->CalculateMtx(mLyt.getDrawInfo());
    mpPanes[MAP_POPUP_PANE_ALL]->Draw(mLyt.getDrawInfo());
}

void dLytMapPopup_c::setLabel(const char *label) {
    mLabel = label;
}

void dLytMapPopup_c::realizeText() {
    const wchar_t *text = dMessage_c::getTextMessageByLabel(mLabel, true, nullptr, 0);
    if (text != nullptr) {
        for (int i = 0; i < MAP_POPUP_NUM_TEXTBOXES; i++) {
            mpTextboxes[i]->setTextWithGlobalTextProcessor(text);
        }
        mpWindow[0]->UpdateSize(mpSizeBox[0], 32.0f);
    }
}

void dLytMapPopup_c::setInout(f32 value) {
    if (0.0f <= value && value <= mAnm[MAP_POPUP_ANIM_INOUT].getLastFrame()) {
        mAnm[MAP_POPUP_ANIM_INOUT].setFrame(value);
        mAnm[MAP_POPUP_ANIM_INOUT].setAnimEnable(true);
        mpPanes[MAP_POPUP_PANE_ALL]->Animate(0);
        mAnm[MAP_POPUP_ANIM_INOUT].setAnimEnable(false);
    }
}

f32 dLytMapPopup_c::getTextRenderWidth() const {
    f32 lineWidth = mpTextboxes[0]->getLineDrawWidth();
    return lineWidth * mpPanes[MAP_POPUP_PANE_ALL]->GetScale().x;
}

void dLytMapPopup_c::setInitialState() {
    mAnm[MAP_POPUP_ANIM_SCALE].setRate(1.0f);
    mAnm[MAP_POPUP_ANIM_SCALE].setForwardOnce();
    mAnm[MAP_POPUP_ANIM_SCALE].setToStart();
    mAnm[MAP_POPUP_ANIM_SCALE].setAnimEnable(true);

    mAnm[MAP_POPUP_ANIM_INOUT].setRate(1.0f);
    mAnm[MAP_POPUP_ANIM_INOUT].setBackwardsOnce();
    mAnm[MAP_POPUP_ANIM_INOUT].setToEnd2();
    mAnm[MAP_POPUP_ANIM_INOUT].setAnimEnable(true);

    mLyt.calc();

    mAnm[MAP_POPUP_ANIM_INOUT].setAnimEnable(false);
    mAnm[MAP_POPUP_ANIM_SCALE].setAnimEnable(false);
}

void dLytMapPopup_c::updateScale() {
    f32 frame = 0.0f;
    f32 zoomFrame = dLytMapGlobal_c::GetInstance()->getZoomFrame();

    switch (mModeCheckResult) {
        case MODE_TRANSITION_TO_ZOOM:  frame = mMaxScale * zoomFrame; break;
        case MODE_TRANSITION_TO_STAGE: frame = mMaxScale * zoomFrame; break;
        case MODE_STABLE_STAGE:        frame = 0.0f; break;
        case MODE_STABLE_ZOOM:         frame = mMaxScale; break;
    }

    if (mMaxScale < frame) {
        frame = mMaxScale;
    } else if (frame < 0.0f) {
        frame = 0.0f;
    }

    mAnm[MAP_POPUP_ANIM_SCALE].setAnimEnable(true);
    mAnm[MAP_POPUP_ANIM_SCALE].setFrame(frame);
    mpPanes[MAP_POPUP_PANE_ALL]->Animate(0);
    mAnm[MAP_POPUP_ANIM_SCALE].setAnimEnable(false);
}

void dLytMapPopup_c::checkMapMode() {
    s32 current = dLytMapGlobal_c::GetInstance()->getCurrentMapMode();
    s32 next = dLytMapGlobal_c::GetInstance()->getNextMapMode();

    if (current == dLytMapGlobal_c::MAPMODE_PROVINCE && next == dLytMapGlobal_c::MAPMODE_ZOOM) {
        return;
    }

    if (current == dLytMapGlobal_c::MAPMODE_ZOOM && next == dLytMapGlobal_c::MAPMODE_PROVINCE) {
        return;
    }

    if (current == dLytMapGlobal_c::MAPMODE_STAGE && next == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mModeCheckResult = MODE_TRANSITION_TO_ZOOM;
        return;
    }

    if (current == dLytMapGlobal_c::MAPMODE_ZOOM && next == dLytMapGlobal_c::MAPMODE_STAGE) {
        mModeCheckResult = MODE_TRANSITION_TO_STAGE;
        return;
    }

    if (current == next && current == dLytMapGlobal_c::MAPMODE_PROVINCE) {
        return;
    }

    if (current == next && current == dLytMapGlobal_c::MAPMODE_STAGE) {
        mModeCheckResult = MODE_STABLE_STAGE;
        return;
    }

    if (current == next && current == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mModeCheckResult = MODE_STABLE_ZOOM;
        return;
    }
}

static const d2d::LytBrlanMapping brlanMapMapPlace[] = {
    {   "mapPlace_00_in.brlan", "G_inOut_00"},
    {"mapPlace_00_scale.brlan", "G_scale_00"},
    {"mapPlace_00_color.brlan", "G_color_00"},
};

#define MAP_PLACE_ANIM_IN 0
#define MAP_PLACE_ANIM_SCALE 1
#define MAP_PLACE_ANIM_COLOR 2

#define MAP_PLACE_NUM_ANIMS 3

static const char *sMapPlacePaneNames[] = {
    "N_all_00",
    "N_all_01",
};

#define MAP_PLACE_PANE_ALL_00 0
#define MAP_PLACE_PANE_ALL_01 1
#define MAP_PLACE_NUM_PANES 2

static const char *sMapPlaceBgPaneNames[] = {
    "P_bg_00",
    "P_bg_01",
    "P_bg_02",
};

#define MAP_PLACE_PANE_BG_00 0
#define MAP_PLACE_PANE_BG_01 1
#define MAP_PLACE_PANE_BG_02 2
#define MAP_PLACE_NUM_BG_PANES 3

static const char *sMapPlaceTextboxNames[] = {
    "T_area_00",
    "T_areaS_00",
    "T_areaS_01",
};

#define MAP_PLACE_NUM_TEXTBOXES 3

static const char *sMapPlaceSizeBoxNames[] = {"W_bgP_00"};

bool dLytMapPlace_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("mapPlace_00.brlyt", nullptr);

    for (int i = 0; i < MAP_PLACE_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapMapPlace[i].mFile, resAcc, mLyt.getLayout(), brlanMapMapPlace[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < MAP_PLACE_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sMapPlacePaneNames[i]);
    }

    for (int i = 0; i < MAP_PLACE_NUM_BG_PANES; i++) {
        mpBgPanes[i] = mLyt.findPane(sMapPlaceBgPaneNames[i]);
    }

    for (int i = 0; i < MAP_PLACE_NUM_TEXTBOXES; i++) {
        mpTextboxes[i] = mLyt.getTextBox(sMapPlaceTextboxNames[i]);
    }

    for (int i = 0; i < 1; i++) {
        mpWindow[i] = mLyt.getWindow(sMapPlaceSizeBoxNames[i]);
        mpSizeBox[i] = mLyt.getSizeBoxInWindow(sMapPlaceSizeBoxNames[i]);
    }

    realizeText();

    f32 windowWidth = mpWindow[0]->GetSize().width;
    f32 w1 = mpBgPanes[0]->GetSize().width;
    f32 w2 = mpBgPanes[1]->GetSize().width;
    f32 w3 = mpBgPanes[2]->GetSize().width;

    f32 lineWidth = mpTextboxes[0]->GetLineWidth(nullptr);

    f32 bgWidth = (w1 + w2 + w3);
    field_0x1C4 = windowWidth - lineWidth;
    field_0x1C0 = windowWidth - bgWidth;

    realizeTextSize();
    realizeBgSize();
    setInitialState();

    mMaxScale = mAnm[MAP_PLACE_ANIM_SCALE].getLastFrame();

    return true;
}

bool dLytMapPlace_c::remove() {
    for (int i = 0; i < MAP_PLACE_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytMapPlace_c::execute() {
    if (mAnm[MAP_PLACE_ANIM_IN].isEnabled() && mAnm[MAP_PLACE_ANIM_IN].isStop2()) {
        mAnm[MAP_PLACE_ANIM_IN].setAnimEnable(false);
    }
    for (int i = 0; i < MAP_PLACE_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    mLyt.calc();
    return true;
}

void dLytMapPlace_c::draw() {
    mpPanes[MAP_PLACE_PANE_ALL_00]->SetAlpha(dLytMapGlobal_c::GetInstance()->getAlpha());
    mpPanes[MAP_PLACE_PANE_ALL_00]->SetTranslate(vec2ToVec3XY(field_0x1C8));
    mpPanes[MAP_PLACE_PANE_ALL_00]->Animate(0);
    mpPanes[MAP_PLACE_PANE_ALL_00]->CalculateMtx(mLyt.getDrawInfo());
    mpPanes[MAP_PLACE_PANE_ALL_00]->Draw(mLyt.getDrawInfo());
}

void dLytMapPlace_c::setLabel(const char *label) {
    mLabel = label;
}

void dLytMapPlace_c::realize() {
    realizeText();
    realizeTextSize();
    realizeBgSize();
}

void dLytMapPlace_c::setInout(f32 value) {
    if (/* 0.0f <= value && */ value <= mAnm[MAP_PLACE_ANIM_IN].getLastFrame()) {
        mAnm[MAP_PLACE_ANIM_IN].setFrame(value);
        mAnm[MAP_PLACE_ANIM_IN].setAnimEnable(true);
        mpPanes[MAP_POPUP_PANE_ALL]->Animate(0);
        mAnm[MAP_PLACE_ANIM_IN].setAnimEnable(false);
    }
}

void dLytMapPlace_c::setColor(u32 value) {
    f32 frame = value;
    mAnm[MAP_PLACE_ANIM_COLOR].setAnimEnable(true);
    mAnm[MAP_PLACE_ANIM_COLOR].setFrame(frame);
    mpPanes[MAP_PLACE_PANE_ALL_00]->Animate(0);
    mAnm[MAP_PLACE_ANIM_COLOR].setAnimEnable(false);
}

void dLytMapPlace_c::setInitialState() {
    mAnm[MAP_PLACE_ANIM_COLOR].setRate(1.0f);
    mAnm[MAP_PLACE_ANIM_COLOR].setForwardOnce();
    mAnm[MAP_PLACE_ANIM_COLOR].setToStart();
    mAnm[MAP_PLACE_ANIM_COLOR].setAnimEnable(true);

    mAnm[MAP_PLACE_ANIM_SCALE].setRate(1.0f);
    mAnm[MAP_PLACE_ANIM_SCALE].setForwardOnce();
    mAnm[MAP_PLACE_ANIM_SCALE].setToStart();
    mAnm[MAP_PLACE_ANIM_SCALE].setAnimEnable(true);

    mAnm[MAP_PLACE_ANIM_IN].setRate(1.0f);
    mAnm[MAP_PLACE_ANIM_IN].setForwardOnce();
    mAnm[MAP_PLACE_ANIM_IN].setToEnd2();
    mAnm[MAP_PLACE_ANIM_IN].setAnimEnable(true);

    mLyt.calc();

    mAnm[MAP_PLACE_ANIM_IN].setAnimEnable(false);
    mAnm[MAP_PLACE_ANIM_SCALE].setAnimEnable(false);
    mAnm[MAP_PLACE_ANIM_COLOR].setAnimEnable(false);
}

void dLytMapPlace_c::updateScale() {
    // @bug mModeCheckResult is never set
    f32 frame = 0.0f;
    f32 zoomFrame = dLytMapGlobal_c::GetInstance()->getZoomFrame();

    switch (mModeCheckResult) {
        case MODE_TRANSITION_TO_ZOOM:  frame = mMaxScale * zoomFrame; break;
        case MODE_TRANSITION_TO_STAGE: frame = mMaxScale * zoomFrame; break;
        case MODE_STABLE_STAGE:        frame = 0.0f; break;
        case MODE_STABLE_ZOOM:         frame = mMaxScale; break;
    }

    if (mMaxScale < frame) {
        frame = mMaxScale;
    } else if (frame < 0.0f) {
        frame = 0.0f;
    }

    mAnm[MAP_PLACE_ANIM_SCALE].setAnimEnable(true);
    mAnm[MAP_PLACE_ANIM_SCALE].setFrame(frame);
    mpPanes[MAP_PLACE_PANE_ALL_01]->Animate(0);
    mAnm[MAP_PLACE_ANIM_SCALE].setAnimEnable(false);
}

void dLytMapPlace_c::realizeText() {
    const wchar_t *text = dMessage_c::getTextMessageByLabel(mLabel, true, nullptr, 0);
    if (text != nullptr) {
        for (int i = 0; i < MAP_PLACE_NUM_TEXTBOXES; i++) {
            mpTextboxes[i]->setTextWithGlobalTextProcessor(text);
        }
    }
}

void dLytMapPlace_c::realizeTextSize() {
    const wchar_t *text = dMessage_c::getTextMessageByLabel(mLabel, true, nullptr, 0);
    if (text != nullptr) {
        mpWindow[0]->UpdateSize(mpSizeBox[0], 0.0f);
        nw4r::lyt::Size size(mpWindow[0]->GetSize());
        size.width += field_0x1C4;
        mpWindow[0]->SetSize(size);
    }
}

static const d2d::LytBrlanMapping brlanMapMapIcon01[] = {
    { "mapIcon_01_player2pattern.brlan",    "G_2pattern_00"},
    {     "mapIcon_01_playerLoop.brlan",  "G_playerLoop_00"},
    {          "mapIcon_01_scale.brlan",       "G_scale_00"},
    {       "mapIcon_01_nusiAnim.brlan",    "G_nusiLoop_00"},
    {       "mapIcon_01_areaLoop.brlan",    "G_areaLoop_00"},
    {      "mapIcon_01_areaColor.brlan",   "G_areaColor_00"},
    {      "mapIcon_01_cloudLoop.brlan",  "G_cloudBLoop_00"},
    {     "mapIcon_01_cloudAlpha.brlan", "G_cloudBAlpha_00"},
    {           "mapIcon_01_loop.brlan",        "G_loop_00"},
    {   "mapIcon_01_goddessOnOff.brlan",     "G_goddess_00"},
    {"mapIcon_01_saveObjectColor.brlan",   "G_saveColor_00"},
    {        "mapIcon_01_iconOut.brlan",   "G_tresInOut_00"},
    {        "mapIcon_01_iconOut.brlan",   "G_markInOut_00"},
    {         "mapIcon_01_iconIn.brlan",   "G_tresInOut_00"},
    {         "mapIcon_01_iconIn.brlan",   "G_markInOut_00"},
    {         "mapIcon_01_iconIn.brlan",   "G_doorInOut_00"},
    {         "mapIcon_01_iconIn.brlan",   "G_saveInOut_00"},
    {         "mapIcon_01_iconIn.brlan",  "G_forceInOut_00"},
};

#define MAP_ICON_01_ANIM_PLAYER_2_PATTERN 0
#define MAP_ICON_01_ANIM_PLAYER_LOOP 1
#define MAP_ICON_01_ANIM_SCALE 2
#define MAP_ICON_01_ANIM_NUSI_ANIM 3
#define MAP_ICON_01_ANIM_AREA_LOOP 4
#define MAP_ICON_01_ANIM_AREA_COLOR 5
#define MAP_ICON_01_ANIM_CLOUD_LOOP 6
#define MAP_ICON_01_ANIM_CLOUD_ALPHA 7
#define MAP_ICON_01_ANIM_LOOP 8
#define MAP_ICON_01_ANIM_GODDESS_ON_OFF 9
#define MAP_ICON_01_ANIM_SAVE_OBJECT_COLOR 10
#define MAP_ICON_01_ANIM_TRES_OUT 11
#define MAP_ICON_01_ANIM_MARK_OUT 12
#define MAP_ICON_01_ANIM_TRES_IN 13
#define MAP_ICON_01_ANIM_MARK_IN 14
#define MAP_ICON_01_ANIM_DOOR_IN 15
#define MAP_ICON_01_ANIM_SAVE_IN 16
#define MAP_ICON_01_ANIM_FORCE_IN 17
#define MAP_ICON_01_NUM_ANIMS 18

static const char *sPaneNamesIcon01[] = {
    "N_treasure_00",  "N_mark_000",     "N_door_000",      "N_saveIcon_00",  "N_triforce_00", "N_player_000",
    "N_player_01",    "N_start_000",    "N_sword_00",      "N_shizuku_00",   "N_shizuku_01",  "N_goddessC_00",
    "N_terryShop_00", "N_cloudBig_000", "N_treasure_01",   "N_areaLight_00", "N_nusi_00",     "N_doorKey_00",
    "N_target_00",    "N_mushi_00",     "N_seekerIcon_00", "N_doddess_00",   "N_domitory_00", "N_shop_00",
    "N_mHusband_00",  "N_mWife_00",     "N_tool_00",       "N_keep_00",      "N_junk_00",     "N_fortune_00",
    "N_ruletou_000",  "N_pampkin_000",  "N_tery_000",      "N_tikurin_000",  "N_uta_000",     "N_musi_000",
    "N_skyloft_000",  "N_roulette_00",  "N_takegiri_00",   "N_kobunB_00",    "N_sebasun_00",  "N_commonIcon_00",
};

#define MAP_ICON_01_PANE_TREASURE_00 0
#define MAP_ICON_01_PANE_MARK_000 1
#define MAP_ICON_01_PANE_DOOR_000 2
#define MAP_ICON_01_PANE_SAVEICON_00 3
#define MAP_ICON_01_PANE_TRIFORCE_00 4
#define MAP_ICON_01_PANE_PLAYER_000 5
#define MAP_ICON_01_PANE_PLAYER_01 6
#define MAP_ICON_01_PANE_START_000 7
#define MAP_ICON_01_PANE_SWORD_00 8
#define MAP_ICON_01_PANE_SHIZUKU_00 9
#define MAP_ICON_01_PANE_SHIZUKU_01 10
#define MAP_ICON_01_PANE_GODDESSC_00 11
#define MAP_ICON_01_PANE_TERRYSHOP_00 12
#define MAP_ICON_01_PANE_CLOUDBIG_000 13
#define MAP_ICON_01_PANE_TREASURE_01 14
#define MAP_ICON_01_PANE_AREALIGHT_00 15
#define MAP_ICON_01_PANE_NUSI_00 16
#define MAP_ICON_01_PANE_DOORKEY_00 17
#define MAP_ICON_01_PANE_TARGET_00 18
#define MAP_ICON_01_PANE_MUSHI_00 19
#define MAP_ICON_01_PANE_SEEKERICON_00 20
#define MAP_ICON_01_PANE_DODDESS_00 21
#define MAP_ICON_01_PANE_DOMITORY_00 22
#define MAP_ICON_01_PANE_SHOP_00 23
#define MAP_ICON_01_PANE_MHUSBAND_00 24
#define MAP_ICON_01_PANE_MWIFE_00 25
#define MAP_ICON_01_PANE_TOOL_00 26
#define MAP_ICON_01_PANE_KEEP_00 27
#define MAP_ICON_01_PANE_JUNK_00 28
#define MAP_ICON_01_PANE_FORTUNE_00 29
#define MAP_ICON_01_PANE_RULETOU_000 30
#define MAP_ICON_01_PANE_PAMPKIN_000 31
#define MAP_ICON_01_PANE_TERY_000 32
#define MAP_ICON_01_PANE_TIKURIN_000 33
#define MAP_ICON_01_PANE_UTA_000 34
#define MAP_ICON_01_PANE_MUSI_000 35
#define MAP_ICON_01_PANE_SKYLOFT_000 36
#define MAP_ICON_01_PANE_ROULETTE_00 37
#define MAP_ICON_01_PANE_TAKEGIRI_00 38
#define MAP_ICON_01_PANE_KOBUNB_00 39
#define MAP_ICON_01_PANE_SEBASUN_00 40
#define MAP_ICON_01_PANE_COMMONICON_00 41

#define MAP_ICON_01_NUM_PANES 42

static const char *sMapIcon01BoundingNames[] = {
    "B_doddess_00",    "B_domitory_00",    "B_shop_00",       "B_mHusband_00",   "B_mWife_00",      "B_tool_00",
    "B_keep_00",       "B_junk_00",        "B_fortune_00",    "B_ruletou_00",    "B_pampkin_00",    "B_tery_00",
    "B_tikurin_00",    "B_uta_01",         "B_musi_01",       "B_skyloft_01",    "B_roulette_00",   "B_takegiri_00",
    "B_kobunB_00",     "B_sebasun_00",     "B_uta_00",        "B_musi_00",       "B_skyloft_00",    "B_nusi_00",
    "B_terryShop_00",  "B_seekerStone_00", "B_commonIcon_00", "B_commonIcon_01", "B_commonIcon_02", "B_commonIcon_03",
    "B_commonIcon_04", "B_commonIcon_05",  "B_commonIcon_06", "B_commonIcon_07", "B_commonIcon_08", "B_commonIcon_09",
    "B_commonIcon_10", "B_commonIcon_11",  "B_commonIcon_12", "B_commonIcon_13", "B_commonIcon_14", "B_commonIcon_15",
    "B_commonIcon_16", "B_commonIcon_17",  "B_commonIcon_18", "B_commonIcon_19", "B_commonIcon_20", "B_commonIcon_21",
    "B_commonIcon_22", "B_commonIcon_23",  "B_commonIcon_24", "B_commonIcon_25", "B_commonIcon_26", "B_commonIcon_27",
    "B_commonIcon_28", "B_commonIcon_29",  "B_saveIcon_00",   "B_mushi_00",      "B_areaLight_00",
};

#define MAP_ICON_01_BOUNDING_DODDESS_00 0
#define MAP_ICON_01_BOUNDING_DOMITORY_00 1
#define MAP_ICON_01_BOUNDING_SHOP_00 2
#define MAP_ICON_01_BOUNDING_MHUSBAND_00 3
#define MAP_ICON_01_BOUNDING_MWIFE_00 4
#define MAP_ICON_01_BOUNDING_TOOL_00 5
#define MAP_ICON_01_BOUNDING_KEEP_00 6
#define MAP_ICON_01_BOUNDING_JUNK_00 7
#define MAP_ICON_01_BOUNDING_FORTUNE_00 8
#define MAP_ICON_01_BOUNDING_RULETOU_00 9
#define MAP_ICON_01_BOUNDING_PAMPKIN_00 10
#define MAP_ICON_01_BOUNDING_TERY_00 11
#define MAP_ICON_01_BOUNDING_TIKURIN_00 12
#define MAP_ICON_01_BOUNDING_UTA_01 13
#define MAP_ICON_01_BOUNDING_MUSI_01 14
#define MAP_ICON_01_BOUNDING_SKYLOFT_01 15
#define MAP_ICON_01_BOUNDING_ROULETTE_00 16
#define MAP_ICON_01_BOUNDING_TAKEGIRI_00 17
#define MAP_ICON_01_BOUNDING_KOBUNB_00 18
#define MAP_ICON_01_BOUNDING_SEBASUN_00 19
#define MAP_ICON_01_BOUNDING_UTA_00 20
#define MAP_ICON_01_BOUNDING_MUSI_00 21
#define MAP_ICON_01_BOUNDING_SKYLOFT_00 22
#define MAP_ICON_01_BOUNDING_NUSI_00 23
#define MAP_ICON_01_BOUNDING_TERRYSHOP_00 24
#define MAP_ICON_01_BOUNDING_SEEKERSTONE_00 25
#define MAP_ICON_01_BOUNDING_COMMONICON_00 26
#define MAP_ICON_01_BOUNDING_COMMONICON_01 27
#define MAP_ICON_01_BOUNDING_COMMONICON_02 28
#define MAP_ICON_01_BOUNDING_COMMONICON_03 29
#define MAP_ICON_01_BOUNDING_COMMONICON_04 30
#define MAP_ICON_01_BOUNDING_COMMONICON_05 31
#define MAP_ICON_01_BOUNDING_COMMONICON_06 32
#define MAP_ICON_01_BOUNDING_COMMONICON_07 33
#define MAP_ICON_01_BOUNDING_COMMONICON_08 34
#define MAP_ICON_01_BOUNDING_COMMONICON_09 35
#define MAP_ICON_01_BOUNDING_COMMONICON_10 36
#define MAP_ICON_01_BOUNDING_COMMONICON_11 37
#define MAP_ICON_01_BOUNDING_COMMONICON_12 38
#define MAP_ICON_01_BOUNDING_COMMONICON_13 39
#define MAP_ICON_01_BOUNDING_COMMONICON_14 40
#define MAP_ICON_01_BOUNDING_COMMONICON_15 41
#define MAP_ICON_01_BOUNDING_COMMONICON_16 42
#define MAP_ICON_01_BOUNDING_COMMONICON_17 43
#define MAP_ICON_01_BOUNDING_COMMONICON_18 44
#define MAP_ICON_01_BOUNDING_COMMONICON_19 45
#define MAP_ICON_01_BOUNDING_COMMONICON_20 46
#define MAP_ICON_01_BOUNDING_COMMONICON_21 47
#define MAP_ICON_01_BOUNDING_COMMONICON_22 48
#define MAP_ICON_01_BOUNDING_COMMONICON_23 49
#define MAP_ICON_01_BOUNDING_COMMONICON_24 50
#define MAP_ICON_01_BOUNDING_COMMONICON_25 51
#define MAP_ICON_01_BOUNDING_COMMONICON_26 52
#define MAP_ICON_01_BOUNDING_COMMONICON_27 53
#define MAP_ICON_01_BOUNDING_COMMONICON_28 54
#define MAP_ICON_01_BOUNDING_COMMONICON_29 55
#define MAP_ICON_01_BOUNDING_SAVEICON_00 56
#define MAP_ICON_01_BOUNDING_MUSHI_00 57
#define MAP_ICON_01_BOUNDING_AREALIGHT_00 58

#define MAP_ICON_01_BOUNDING_COMMONICON_LAST (MAP_ICON_01_BOUNDING_COMMONICON_29 + 1)

#define MAP_ICON_01_NUM_BOUNDINGS 59

static const char *sMapIcon01SpecialPaneNames[] = {
    "N_doddess_000",  "N_domitory_000",  "N_shop_000",       "N_mHusband_000", "N_mWife_000",    "N_tool_000",
    "N_keep_000",     "N_junk_000",      "N_fortune_000",    "N_ruletou_000",  "N_pampkin_000",  "N_tery_000",
    "N_tikurin_000",  "N_uta_000",       "N_musi_000",       "N_skyloft_02",   "N_roulette_01",  "N_takegiri_01",
    "N_kobunB_01",    "N_sebasun_01",    "N_commonIcon_000", "N_saveIcon_000", "N_areaLight_01", "N_nusi_000",
    "N_terryShop_01", "N_seekerIcon_01", "N_mushi_01",
};

#define MAP_ICON_01_SPECIAL_PANE_DODDESS_000 0
#define MAP_ICON_01_SPECIAL_PANE_DOMITORY_000 1
#define MAP_ICON_01_SPECIAL_PANE_SHOP_000 2
#define MAP_ICON_01_SPECIAL_PANE_MHUSBAND_000 3
#define MAP_ICON_01_SPECIAL_PANE_MWIFE_000 4
#define MAP_ICON_01_SPECIAL_PANE_TOOL_000 5
#define MAP_ICON_01_SPECIAL_PANE_KEEP_000 6
#define MAP_ICON_01_SPECIAL_PANE_JUNK_000 7
#define MAP_ICON_01_SPECIAL_PANE_FORTUNE_000 8
#define MAP_ICON_01_SPECIAL_PANE_RULETOU_000 9
#define MAP_ICON_01_SPECIAL_PANE_PAMPKIN_000 10
#define MAP_ICON_01_SPECIAL_PANE_TERY_000 11
#define MAP_ICON_01_SPECIAL_PANE_TIKURIN_000 12
#define MAP_ICON_01_SPECIAL_PANE_UTA_000 13
#define MAP_ICON_01_SPECIAL_PANE_MUSI_000 14
#define MAP_ICON_01_SPECIAL_PANE_SKYLOFT_02 15
#define MAP_ICON_01_SPECIAL_PANE_ROULETTE_01 16
#define MAP_ICON_01_SPECIAL_PANE_TAKEGIRI_01 17
#define MAP_ICON_01_SPECIAL_PANE_KOBUNB_01 18
#define MAP_ICON_01_SPECIAL_PANE_SEBASUN_01 19
#define MAP_ICON_01_SPECIAL_PANE_COMMONICON_000 20
#define MAP_ICON_01_SPECIAL_PANE_SAVEICON_000 21
#define MAP_ICON_01_SPECIAL_PANE_AREALIGHT_01 22
#define MAP_ICON_01_SPECIAL_PANE_NUSI_000 23
#define MAP_ICON_01_SPECIAL_PANE_TERRYSHOP_01 24
#define MAP_ICON_01_SPECIAL_PANE_SEEKERICON_01 25
#define MAP_ICON_01_SPECIAL_PANE_MUSHI_01 26

#define MAP_ICON_01_NUM_SPECIAL_PANES 27

static const char *sMapIcon01SpecialPictureNames[] = {
    "P_doddess_00",  "P_domitory_00", "P_shop_00",    "P_mHusband_00", "P_mWife_00",  "P_tool_00",    "P_keep_00",
    "P_junk_00",     "P_fortune_00",  "P_ruletou_00", "P_pampkin_00",  "P_tery_00",   "P_tikurin_00", "P_uta_00",
    "P_musi_00",     "P_skyloft_00",  "P_icon_01",    "P_icon_02",     "P_icon_00",   "P_icon_04",    "P_commonIcon_00",
    "P_saveIcon_01", "P_light_00_00", "P_nusi_00",    "P_terry_01",    "P_seeker_00", "P_mushi_00",

};

#define MAP_ICON_01_SPECIAL_PICTURE_DODDESS_00 0
#define MAP_ICON_01_SPECIAL_PICTURE_DOMITORY_00 1
#define MAP_ICON_01_SPECIAL_PICTURE_SHOP_00 2
#define MAP_ICON_01_SPECIAL_PICTURE_MHUSBAND_00 3
#define MAP_ICON_01_SPECIAL_PICTURE_MWIFE_00 4
#define MAP_ICON_01_SPECIAL_PICTURE_TOOL_00 5
#define MAP_ICON_01_SPECIAL_PICTURE_KEEP_00 6
#define MAP_ICON_01_SPECIAL_PICTURE_JUNK_00 7
#define MAP_ICON_01_SPECIAL_PICTURE_FORTUNE_00 8
#define MAP_ICON_01_SPECIAL_PICTURE_RULETOU_00 9
#define MAP_ICON_01_SPECIAL_PICTURE_PAMPKIN_00 10
#define MAP_ICON_01_SPECIAL_PICTURE_TERY_00 11
#define MAP_ICON_01_SPECIAL_PICTURE_TIKURIN_00 12
#define MAP_ICON_01_SPECIAL_PICTURE_UTA_00 13
#define MAP_ICON_01_SPECIAL_PICTURE_MUSI_00 14
#define MAP_ICON_01_SPECIAL_PICTURE_SKYLOFT_00 15
#define MAP_ICON_01_SPECIAL_PICTURE_ICON_01 16
#define MAP_ICON_01_SPECIAL_PICTURE_ICON_02 17
#define MAP_ICON_01_SPECIAL_PICTURE_ICON_00 18
#define MAP_ICON_01_SPECIAL_PICTURE_ICON_04 19
#define MAP_ICON_01_SPECIAL_PICTURE_COMMONICON_00 20
#define MAP_ICON_01_SPECIAL_PICTURE_SAVEICON_01 21
#define MAP_ICON_01_SPECIAL_PICTURE_LIGHT_00_00 22
#define MAP_ICON_01_SPECIAL_PICTURE_NUSI_00 23
#define MAP_ICON_01_SPECIAL_PICTURE_TERRY_01 24
#define MAP_ICON_01_SPECIAL_PICTURE_SEEKER_00 25
#define MAP_ICON_01_SPECIAL_PICTURE_MUSHI_00 26

#define MAP_ICON_01_NUM_SPECIAL_PICTURES 27

static const char *sSaveObjPrefix = "SAVEOBJ_NAME_";
static const char *sTerryOnOff = "N_terryOnOff_00";

bool dLytMapIcon01_c::build(d2d::ResAccIf_c *resAcc) {
    field_0x1848 = 30;
    mCurrentFloor = dLytMapGlobal_c::GetInstance()->getFloor();
    field_0x0C83 = dLytMapGlobal_c::GetInstance()->getField_0x4F();
    field_0x0C84 = dLytMapGlobal_c::GetInstance()->getField_0x4C();
    mLastFloor = mCurrentFloor;
    mpResAcc = resAcc;
    mLyt.setResAcc(resAcc);
    mLyt.build("mapIcon_01.brlyt", nullptr);
    for (int i = 0; i < MAP_ICON_01_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapMapIcon01[i].mFile, mpResAcc, mLyt.getLayout(), brlanMapMapIcon01[i].mName);
        mAnm[i].bind(false);
    }

    if (daPlayerActBase_c::getCurrentTunicType() == 1) {
        setLinkTunic(1);
    } else {
        setLinkTunic(0);
    }

    // TODO bunch of setters

    if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_TRIFORCE_COMPLETE)) {
        setGoddessStatue(false);
    } else {
        setGoddessStatue(true);
    }

    for (int i = 0; i < MAP_ICON_01_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sPaneNamesIcon01[i]);
    }

    for (int i = 0; i < MAP_ICON_01_NUM_BOUNDINGS; i++) {
        mpBoundings[i] = mLyt.findBounding(sMapIcon01BoundingNames[i]);
    }

    // TODO - why 56 and not 59
    for (int i = 0; i < MAP_ICON_01_BOUNDING_COMMONICON_LAST; i++) {
        mPopupConfigs[i].bounding = mpBoundings[i];
    }

    for (int i = 0; i < MAP_ICON_01_NUM_BOUNDINGS; i++) {
        mpBoundings[i]->SetVisible(false);
    }

    mLyt.calc();

    for (int i = 0; i < MAP_ICON_01_NUM_SPECIAL_PANES; i++) {
        mpSpecialPanes[i] = mLyt.findPane(sMapIcon01SpecialPaneNames[i]);
    }

    for (int i = 0; i < MAP_ICON_01_NUM_SPECIAL_PICTURES; i++) {
        mpSpecialPictures[i] = mLyt.findPane(sMapIcon01SpecialPictureNames[i]);
    }

    mpTerryOnOffPane = mLyt.findPane(sTerryOnOff);
    setTerry(false);

    for (int i = 0; i < 35; i++) {
        mPassIdxes[i] = i;
    }

    checkMapMode();
    loadFlags();

    for (int i = 0; i < (int)ARRAY_LENGTH(mIconAnims); i++) {
        mIconAnims[i].cmdIndex = ARRAY_LENGTH(mCommands);
        mIconAnims[i].frame = 0.0f;
        mIconAnims[i].animIn = false;
        mIconAnims[i].animOut = false;
        mIconAnims[i].visible = false;
        mIconAnims[i].prevVisible = false;
        mIconAnims[i].render = false;
    }

    setupActorDrawCommands();
    setupStageDrawCommands();
    setupLinkDrawCommand();
    if (field_0x0C70 == 1) {
        setupTriforceDrawCommands();
    }

    if (field_0x0C70 == 4) {
        setupCloudBigDrawCommand();
    }

    recordAnimVisState();
    transitionAnims();
    sortPanes();

    mCsHitCheck.init(mLyt.getLayout()->GetRootPane(), 0x1, 0, 0);
    dCsMgr_c::GetInstance()->registCursorTarget(&mCsHitCheck);
    resetDrawCommands();
    return true;
}

bool dLytMapIcon01_c::remove() {
    for (int i = 0; i < MAP_ICON_01_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    dCsMgr_c::GetInstance()->unregistCursorTarget(&mCsHitCheck);
    return true;
}

bool dLytMapIcon01_c::execute() {
    for (int i = 0; i < MAP_ICON_01_ANIM_FORCE_IN + 1 - MAP_ICON_01_ANIM_TRES_OUT; i++) {
        if (mAnm[MAP_ICON_01_ANIM_TRES_OUT + i].isEnabled() && mAnm[MAP_ICON_01_ANIM_TRES_OUT + i].isStop2()) {
            mAnm[MAP_ICON_01_ANIM_TRES_OUT + i].setAnimEnable(false);
        }
    }
    resetDrawCommands();
    checkMapMode();
    loadFlags();

    mCurrentFloor = dLytMapGlobal_c::GetInstance()->getFloor();
    field_0x0C83 = dLytMapGlobal_c::GetInstance()->getField_0x4F();
    field_0x0C84 = dLytMapGlobal_c::GetInstance()->getField_0x4C();

    updateScale();
    setupActorDrawCommands();
    setupStageDrawCommands();
    setupLinkDrawCommand();
    if (field_0x0C70 == 1) {
        setupTriforceDrawCommands();
    }

    if (field_0x0C70 == 4) {
        setupCloudBigDrawCommand();
    }

    for (int i = 0; i < MAP_ICON_01_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    if (dLytMapGlobal_c::GetInstance()->getField_0x4D() &&
        dLytMapGlobal_c::GetInstance()->getMapEvent() != dLytMapMain_c::MAP_EVENT_SAVE_OBJ) {
        checkBoundingPointing();
    }

    mCsHitCheck.resetCachedHitboxes();
    mCsHitCheck.execute();

    transitionAnims();
    recordAnimVisState();

    if (!field_0x1844) {
        field_0x1844 = true;
        if (field_0x0C84 == 0) {
            for (int i = 0; i < 50; i++) {
                mIconAnims[i].animIn = false;
                mIconAnims[i].animOut = false;
            }
        }
    }

    return true;
}

void dLytMapIcon01_c::draw() {
    // TODO - tons of GPR regswaps, mAng nonsense, otherwise OK
    mAng mapRot = dLytMapGlobal_c::GetInstance()->getMapRotation();
    // Maybe compiler-generated
    u8 *idxes = mPassIdxes;
    for (int pass = 0; pass < 35; pass++, idxes++) {
        s32 idx = *idxes;
        if (mNumCommandsPerPass[idx]) {
            for (int cmd = 0; cmd < mNumCommands; cmd++) {
                // Maybe compiler-generated
                const mAng &commandRot = mCommands[cmd].rotation;
                if (idx == mCommands[cmd].passIdx) {
                    s32 paneIdx = mCommands[cmd].paneIdx;
                    nw4r::lyt::Pane *pane = mpPanes[paneIdx];
                    switch (paneIdx) {
                        case MAP_ICON_01_PANE_PLAYER_000: {
                            mVec3_c rotate(0.0f, 0.0f, 0.0f);
                            if (dLytMapGlobal_c::GetInstance()->getField_0x55() != 0) {
                                dAcPy_c *link = getLinkPtr();
                                rotate.z = mAng(link->mRotation.y + link->vt_0x19C() + -mapRot).degree_c();
                            } else {
                                rotate.z = mAng(field_0x0D3A + -mapRot).degree_c();
                            }
                            pane->SetRotate(rotate);
                            pane = pane->GetParent();
                            break;
                        }
                        case MAP_ICON_01_PANE_DOOR_000:
                        case MAP_ICON_01_PANE_START_000: {
                            mVec3_c rotate(0.0f, 0.0f, mAng(-mapRot + commandRot).degree_c());
                            pane->SetRotate(rotate);
                            pane = mpPanes[paneIdx]->GetParent();
                            break;
                        }
                        case MAP_ICON_01_PANE_MARK_000:
                        case MAP_ICON_01_PANE_RULETOU_000:
                        case MAP_ICON_01_PANE_PAMPKIN_000:
                        case MAP_ICON_01_PANE_TERY_000:
                        case MAP_ICON_01_PANE_TIKURIN_000:
                        case MAP_ICON_01_PANE_UTA_000:
                        case MAP_ICON_01_PANE_MUSI_000:
                        case MAP_ICON_01_PANE_SKYLOFT_000: {
                            mVec3_c rotate(0.0f, 0.0f, (-mapRot).degree_c());
                            pane->SetRotate(rotate);
                            pane = mpPanes[paneIdx]->GetParent();
                            break;
                        }
                        case MAP_ICON_01_PANE_AREALIGHT_00: {
                            if (field_0x0D48 != ARRAY_LENGTH(mCommands) && field_0x0D48 == cmd) {
                                setAreaLight(0, pane);
                            } else if (field_0x0D49 != ARRAY_LENGTH(mCommands) && field_0x0D49 == cmd) {
                                setAreaLight(1, pane);
                            } else if (field_0x0D4A != ARRAY_LENGTH(mCommands) && field_0x0D4A == cmd) {
                                setAreaLight(2, pane);
                            }
                            break;
                        }
                        case MAP_ICON_01_PANE_SAVEICON_00: {
                            if (field_0x0D4B[cmd] != ARRAY_LENGTH(mCommands)) {
                                setSaveObj(field_0x0D4B[cmd], pane);
                            }
                            break;
                        }
                    }

                    if (paneIdx == MAP_ICON_01_PANE_CLOUDBIG_000) {
                        if (dScGame_c::isCurrentStage("F020")) {
                            if (mThunderheadEntered) {
                                setCloud(1, pane);
                            } else {
                                setCloud(0, pane);
                            }
                        } else if (dScGame_c::isCurrentStage("F023")) {
                            setCloud(2, pane);
                        }
                        mVec3_c rotate(0.0f, 0.0f, (-mapRot).degree_c());
                        pane->SetRotate(rotate);
                        pane = mpPanes[paneIdx]->GetParent();
                    }

                    u8 alpha = dLytMapGlobal_c::GetInstance()->getAlpha();
                    bool found = false;
                    int i = 0;
                    for (; i < mNumAnims; i++) {
                        if (cmd == mIconAnims[i].cmdIndex) {
                            found = true;
                            break;
                        }
                    }

                    if (found == true) {
                        if (field_0x0C84 && mIconAnims[i].animIn == true) {
                            if (field_0x1845 == 1) {
                                mIconAnims[i].visible = true;
                                mIconAnims[i].animIn = true;
                            } else {
                                mIconAnims[i].visible = false;
                                mIconAnims[i].animIn = false;
                            }
                        }

                        if (mCurrentFloor != mLastFloor) {
                            mIconAnims[i].animIn = false;
                            mIconAnims[i].animOut = false;
                        }

                        if (mIconAnims[i].animIn) {
                            f32 f = mIconAnims[i].frame;
                            if (f <= field_0x1838) {
                                drawWithAnimIn(paneIdx, pane, f);
                                mIconAnims[i].frame += 1.0f;
                                mIconAnims[i].render = true;
                                if (!field_0x184C) {
                                    dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MAP_ICON_APPEAR);
                                    field_0x184C = true;
                                }
                            } else {
                                mIconAnims[i].animIn = false;
                                mIconAnims[i].render = true;
                                field_0x184C = false;
                            }
                        } else if (mIconAnims[i].animOut) {
                            f32 f = mIconAnims[i].frame;
                            if (f <= field_0x183C) {
                                drawWithAnimOut(paneIdx, pane, f);
                                mIconAnims[i].frame += 1.0f;
                                mIconAnims[i].render = true;
                            } else {
                                mIconAnims[i].animOut = false;
                                mIconAnims[i].render = true;
                            }
                        } else if (mIconAnims[i].visible) {
                            drawFullyIn(paneIdx, pane);
                            mIconAnims[i].render = true;
                        }

                        if (mIconAnims[i].render == true) {
                            pane->SetAlpha(alpha);
                            pane->SetTranslate(vec2ToVec3XY(mCommands[cmd].position + field_0x0D40));
                            pane->Animate(0);
                            pane->CalculateMtx(mLyt.getDrawInfo());
                            pane->Draw(mLyt.getDrawInfo());
                        }
                    } else {
                        // No animation data - simply draw
                        pane->SetAlpha(alpha);
                        pane->SetTranslate(vec2ToVec3XY(mCommands[cmd].position + field_0x0D40));
                        pane->Animate(0);
                        pane->CalculateMtx(mLyt.getDrawInfo());
                        pane->Draw(mLyt.getDrawInfo());
                    }
                }
            }
        }
    }

    mVec2_c v2(0.0f, 0.0f);
    for (int i = 0; i < field_0x0E13; i++) {
        if (mPopupConfigs[i + MAP_ICON_01_BOUNDING_COMMONICON_00].bounding->IsVisible()) {
            v2 = mPopupConfigs[i + MAP_ICON_01_BOUNDING_COMMONICON_00].boundingPos;
            mPopupConfigs[i + MAP_ICON_01_BOUNDING_COMMONICON_00].bounding->SetTranslate(
                vec2ToVec3XY(v2 + field_0x0D40)
            );
            mPopupConfigs[i + MAP_ICON_01_BOUNDING_COMMONICON_00].bounding->CalculateMtx(mLyt.getDrawInfo());
            mPopupConfigs[i + MAP_ICON_01_BOUNDING_COMMONICON_00].bounding->Draw(mLyt.getDrawInfo());
        }
    }

    mLastFloor = mCurrentFloor;
}

static const char *sMapPrefix = "MAP_";

void dLytMapIcon01_c::resetDrawCommands() {
    for (int i = 0; i < (int)ARRAY_LENGTH(mCommands); i++) {
        mCommands[i].position.set(0.0f, 0.0f);
        mCommands[i].rotation = 0.0f;
        mCommands[i].paneIdx = MAP_ICON_01_NUM_PANES;
        mCommands[i].passIdx = 35;
        field_0x0D4B[i] = ARRAY_LENGTH(mCommands);
        field_0x0C85[i] = 0;
    }

    mCurrentFloor = 0;

    for (int i = 0; i < (int)ARRAY_LENGTH(mNumCommandsPerPass); i++) {
        mNumCommandsPerPass[i] = 0;
    }

    mNumCommands = 0;

    for (int i = 0; i < (int)ARRAY_LENGTH(mPopupConfigs); i++) {
        mPopupConfigs[i].cmdIdx = ARRAY_LENGTH(mCommands);
        mPopupConfigs[i].boundingPos.set(0.0f, 0.0f);
        mPopupConfigs[i].textPos.set(0.0f, 0.0f);
        mPopupConfigs[i].pointedAt = false;
        mPopupConfigs[i].objId = 0;
        mPopupConfigs[i].labelPrefix = sMapPrefix;
    }

    field_0x0E13 = 0;
    field_0x0C83 = 0;
    field_0x0C84 = 0;

    fn_80181C40();

    field_0x0D48 = ARRAY_LENGTH(mCommands);
    field_0x0D49 = ARRAY_LENGTH(mCommands);
    field_0x0D4A = ARRAY_LENGTH(mCommands);

    mFunFunIslandDiscovered = false;
    mLumpyPumkpinDiscovered = false;
    mBeedleIslandDiscovered = false;
    mBambooIslandDiscovered = false;
    mIsleOfSongsDiscovered = false;
    mBugHavenDiscovered = false;
    mBilocyteFightTriggered = false;
    mFaronPillarOpened = false;
    mFaronDiscovered = false;
    mEldinPillarOpened = false;
    mEldinDiscovered = false;
    mLanayruPillarOpened = false;
    mLanaryuDiscovered = false;
    mThunderheadEntered = false;
    mBeedleShopEntered = false;

    for (int i = 0; i < (int)ARRAY_LENGTH(mIconAnims); i++) {
        mIconAnims[i].render = false;
    }

    mNumAnims = 0;
}

void dLytMapIcon01_c::loadFlags() {
    StoryflagManager *mgr = StoryflagManager::sInstance;
    mFunFunIslandDiscovered = mgr->getCounterOrFlag(STORYFLAG_FUN_FUN_ISLAND_DISCOVERED);
    mLumpyPumkpinDiscovered = mgr->getCounterOrFlag(STORYFLAG_LUMPY_PUMPKIN_DISCOVERED);
    mBeedleIslandDiscovered = mgr->getCounterOrFlag(STORYFLAG_BEEDLE_ISLAND_DISCOVERED);
    mBambooIslandDiscovered = mgr->getCounterOrFlag(STORYFLAG_BAMBOO_ISLAND_DISCOVERED);
    mIsleOfSongsDiscovered = mgr->getCounterOrFlag(STORYFLAG_ISLE_OF_SONGS_DISCOVERED);
    mBugHavenDiscovered = mgr->getCounterOrFlag(STORYFLAG_BUG_HAVEN_DISCOVERED);
    mBilocyteFightTriggered = mgr->getCounterOrFlag(STORYFLAG_BILOCYCTE_FIGHT_TRIGGERED);
    mFaronPillarOpened = mgr->getCounterOrFlag(STORYFLAG_FARON_PILLAR_OPENED);
    mFaronDiscovered = mgr->getCounterOrFlag(STORYFLAG_FARON_DISCOVERED);
    mEldinPillarOpened = mgr->getCounterOrFlag(STORYFLAG_ELDIN_PILLAR_OPENED);
    mEldinDiscovered = mgr->getCounterOrFlag(STORYFLAG_ELDIN_DISCOVERED);
    mLanayruPillarOpened = mgr->getCounterOrFlag(STORYFLAG_LANAYRU_PILLAR_OPENED);
    mLanaryuDiscovered = mgr->getCounterOrFlag(STORYFLAG_LANAYRU_DISCOVERED);
    mThunderheadEntered = mgr->getCounterOrFlag(STORYFLAG_THUNDERHEAD_ENTERED);
    mBeedleShopEntered = mgr->getCounterOrFlag(STORYFLAG_BEEDLE_SHOP_ENTERED);
}

void dLytMapIcon01_c::setLinkTunic(s32 type) {
    f32 frame = 0.0f;
    switch (type) {
        case 1: frame = 0.0f; break;
        case 0: frame = 1.0f; break;
    }

    mAnm[MAP_ICON_01_ANIM_PLAYER_2_PATTERN].setAnimEnable(true);
    mAnm[MAP_ICON_01_ANIM_PLAYER_2_PATTERN].setFrame(frame);
    mLyt.calc();
    mAnm[MAP_ICON_01_ANIM_PLAYER_2_PATTERN].setAnimEnable(false);
}

void dLytMapIcon01_c::setAreaLight(u32 color, nw4r::lyt::Pane *pane) {
    f32 frame = color;
    mAnm[MAP_ICON_01_ANIM_AREA_COLOR].setAnimEnable(true);
    mAnm[MAP_ICON_01_ANIM_AREA_COLOR].setFrame(frame);
    pane->Animate(0);
    mAnm[MAP_ICON_01_ANIM_AREA_COLOR].setAnimEnable(false);
}

void dLytMapIcon01_c::setCloud(u32 alpha, nw4r::lyt::Pane *pane) {
    f32 frame = alpha;
    mAnm[MAP_ICON_01_ANIM_CLOUD_ALPHA].setAnimEnable(true);
    mAnm[MAP_ICON_01_ANIM_CLOUD_ALPHA].setFrame(frame);
    pane->Animate(0);
    mAnm[MAP_ICON_01_ANIM_CLOUD_ALPHA].setAnimEnable(false);
}

void dLytMapIcon01_c::setGoddessStatue(u32 present) {
    f32 frame = 0.0f;
    if (!present) {
        frame = 1.0f;
    }
    mAnm[MAP_ICON_01_ANIM_GODDESS_ON_OFF].setFrame(frame);
    mAnm[MAP_ICON_01_ANIM_GODDESS_ON_OFF].setAnimEnable(true);
    mLyt.calc();
    mAnm[MAP_ICON_01_ANIM_GODDESS_ON_OFF].setAnimEnable(false);
}

void dLytMapIcon01_c::setSaveObj(u32 color, nw4r::lyt::Pane *pane) {
    f32 frame = color;
    mAnm[MAP_ICON_01_ANIM_SAVE_OBJECT_COLOR].setAnimEnable(true);
    mAnm[MAP_ICON_01_ANIM_SAVE_OBJECT_COLOR].setFrame(frame);
    pane->Animate(0);
    mAnm[MAP_ICON_01_ANIM_SAVE_OBJECT_COLOR].setAnimEnable(false);
}

void dLytMapIcon01_c::setTerry(bool present) {
    mpTerryOnOffPane->SetVisible(present);
}

void dLytMapIcon01_c::drawWithAnimIn(u32 paneIdx, nw4r::lyt::Pane *pane, f32 frame) {
    if (paneIdx == MAP_ICON_01_PANE_DOORKEY_00) {
        paneIdx = MAP_ICON_01_PANE_DOOR_000;
    }

    // NOTE: Treasure/Mark/Door/Save/Triforce order is the same between anims and panes
    u8 anmIdx = paneIdx - MAP_ICON_01_PANE_TREASURE_00 + MAP_ICON_01_ANIM_TRES_IN;

    if (frame <= mAnm[anmIdx].getLastFrame()) {
        mAnm[anmIdx].setFrame(frame);
        // Maybe inlines/macros
        d2d::AnmGroup_c &anm = mAnm[anmIdx];
        anm.setAnimEnable(true);
        pane->Animate(0);
        anm.setAnimEnable(false);
    }
}

void dLytMapIcon01_c::drawWithAnimOut(u32 paneIdx, nw4r::lyt::Pane *pane, f32 frame) {
    if (paneIdx == MAP_ICON_01_PANE_DOORKEY_00) {
        paneIdx = MAP_ICON_01_PANE_DOOR_000;
    }

    // NOTE: Treasure/Mark/Door/Save/Triforce order is the same between anims and panes
    u8 anmIdx = paneIdx - MAP_ICON_01_PANE_TREASURE_00 + MAP_ICON_01_ANIM_TRES_OUT;

    if (frame <= mAnm[anmIdx].getLastFrame()) {
        mAnm[anmIdx].setFrame(frame);
        // Maybe inlines/macros
        d2d::AnmGroup_c &anm = mAnm[anmIdx];
        anm.setAnimEnable(true);
        pane->Animate(0);
        anm.setAnimEnable(false);
    }
}

void dLytMapIcon01_c::drawFullyIn(u32 paneIdx, nw4r::lyt::Pane *pane) {
    if (paneIdx == MAP_ICON_01_PANE_DOORKEY_00) {
        paneIdx = MAP_ICON_01_PANE_DOOR_000;
    }

    // NOTE: Treasure/Mark/Door/Save/Triforce order is the same between anims and panes
    u8 anmIdx = paneIdx - MAP_ICON_01_PANE_TREASURE_00 + MAP_ICON_01_ANIM_TRES_IN;

    mAnm[anmIdx].setToEnd2();
    // Maybe inlines/macros
    d2d::AnmGroup_c &anm = mAnm[anmIdx];
    anm.setAnimEnable(true);
    pane->Animate(0);
    anm.setAnimEnable(false);
}

void dLytMapIcon01_c::checkMapMode() {
    s32 current = dLytMapGlobal_c::GetInstance()->getCurrentMapMode();
    s32 next = dLytMapGlobal_c::GetInstance()->getNextMapMode();

    if (current == dLytMapGlobal_c::MAPMODE_PROVINCE && next == dLytMapGlobal_c::MAPMODE_ZOOM) {
        return;
    }

    if (current == dLytMapGlobal_c::MAPMODE_ZOOM && next == dLytMapGlobal_c::MAPMODE_PROVINCE) {
        return;
    }

    if (current == dLytMapGlobal_c::MAPMODE_STAGE && next == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mModeCheckResult = MODE_TRANSITION_TO_ZOOM;
        return;
    }

    if (current == dLytMapGlobal_c::MAPMODE_ZOOM && next == dLytMapGlobal_c::MAPMODE_STAGE) {
        mModeCheckResult = MODE_TRANSITION_TO_STAGE;
        return;
    }

    if (current == next && current == dLytMapGlobal_c::MAPMODE_PROVINCE) {
        return;
    }

    if (current == next && current == dLytMapGlobal_c::MAPMODE_STAGE) {
        mModeCheckResult = MODE_STABLE_STAGE;
        return;
    }

    if (current == next && current == dLytMapGlobal_c::MAPMODE_ZOOM) {
        mModeCheckResult = MODE_STABLE_ZOOM;
        return;
    }
}

void dLytMapIcon01_c::setupActorDrawCommands() {
    dAcBase_c *ac = nullptr;
    while ((ac = static_cast<dAcBase_c *>(fManager_c::searchBaseByGroupType(fBase_c::ACTOR, ac))) != nullptr) {
        if (field_0x0C70 == 4 && mBilocyteFightTriggered) {
            setupNpcBNusiDrawCommand(ac);
        }

        if (field_0x0C70 == 1 && checkHasMap()) {
            setupObjD3DummyDrawCommand(ac);
        }

        if (dScGame_c::currentSpawnInfo.getTimeOfDay() != SpawnInfo::NIGHT) {
            setupObjTerryShopDrawCommand(ac);
        }

        setupTboxDrawCommand(ac);
        setupObjShutterDrawCommand(ac);
        setupObjDoorDrawCommand(ac);
        setupObjSealedDoorDrawCommand(ac);
        setupObjHarpHintDrawCommand(ac);

        if (field_0x184D) {
            setupNpcBeeDrawCommand(ac);
        }

        setupNpcKenseiDrawCommand(ac);
        setupObjSaveDrawCommand(ac);
        setupObjLightLineDrawCommand(ac);
        setupObjRoAtTargetDrawCommand(ac);

        if (mNumCommands >= ARRAY_LENGTH(mCommands)) {
            break;
        }
    }
}

void dLytMapIcon01_c::setupNpcKenseiDrawCommand(dAcBase_c *actor) {
    if (mNumCommands < ARRAY_LENGTH(mCommands)) {
        if (actor->mProfileName == fProfile::NPC_KENSEI && !actor->checkActorProperty(dAcBase_c::AC_PROP_0x100) &&
            mCurrentFloor == dStage_c::GetInstance()->getMapRelated()->fn_801B4F10(actor->mRoomID, actor->mPosition)) {
            mVec2_c pos(0.0f, 0.0f);
            projectOntoMap(actor->mPosition, pos);
            mCommands[mNumCommands].passIdx = 18;
            mCommands[mNumCommands].paneIdx = MAP_ICON_01_PANE_SWORD_00;
            mCommands[mNumCommands].position.x = pos.x;
            mCommands[mNumCommands].position.y = pos.y;

            mNumCommandsPerPass[18]++;
            mNumCommands++;
        }
    }
}

void dLytMapIcon01_c::setupTboxDrawCommand(dAcBase_c *actor) {
    if (actor->mProfileName == fProfile::TBOX && !actor->checkActorProperty(dAcBase_c::AC_PROP_0x100) &&
        mCurrentFloor == dStage_c::GetInstance()->getMapRelated()->fn_801B4F10(actor->mRoomID, actor->mPosition)) {
        dAcTbox_c *box = static_cast<dAcTbox_c *>(actor);
        if (box->getVariant() == dAcTbox_c::GODDESS) {
            u16 flag = box->getParams2Lower();
            if (StoryflagManager::sInstance->getCounterOrFlag(flag) != 0 && box->hasBeenOpened() != true) {
                setupTboxDrawCommandGoddessClosed(box);
            }
        } else if (box->hasBeenOpened() != true) {
            if (field_0x0C70 == 1) {
                setupTboxDrawCommandClosed(box);
            }
        } else {
            setupTboxDrawCommandOpen(box);
        }
    }
}

void dLytMapIcon01_c::setupTboxDrawCommandClosed(dAcTbox_c *box) {
    if (mNumCommands < ARRAY_LENGTH(mCommands)) {
        mVec2_c pos(0.0f, 0.0f);
        projectOntoMap(box->mPosition, pos);
        mCommands[mNumCommands].passIdx = 17;
        mCommands[mNumCommands].paneIdx = MAP_ICON_01_PANE_TREASURE_00;
        mCommands[mNumCommands].position.x = pos.x;
        mCommands[mNumCommands].position.y = pos.y;

        if (checkHasMap()) {
            if (field_0x0C84) {
                mIconAnims[mNumAnims].cmdIndex = mNumCommands;
                mIconAnims[mNumAnims].visible = true;
                mIconAnims[mNumAnims].animIn = true;
            } else {
                mIconAnims[mNumAnims].cmdIndex = mNumCommands;
                mIconAnims[mNumAnims].visible = true;
                mIconAnims[mNumAnims].animIn = false;
            }
        } else {
            mIconAnims[mNumAnims].cmdIndex = mNumCommands;
            mIconAnims[mNumAnims].visible = false;
        }

        mNumAnims++;
        mNumCommandsPerPass[17]++;
        mNumCommands++;
    }
}

void dLytMapIcon01_c::setupTboxDrawCommandOpen(dAcTbox_c *box) {
    if (mNumCommands < ARRAY_LENGTH(mCommands)) {
        mVec2_c pos(0.0f, 0.0f);
        projectOntoMap(box->mPosition, pos);
        mCommands[mNumCommands].passIdx = 17;
        mCommands[mNumCommands].paneIdx = MAP_ICON_01_PANE_TREASURE_01;
        mCommands[mNumCommands].position.x = pos.x;
        mCommands[mNumCommands].position.y = pos.y;

        mNumCommandsPerPass[17]++;
        mNumCommands++;
    }
}

void dLytMapIcon01_c::setupTboxDrawCommandGoddessClosed(dAcTbox_c *box) {
    if (mNumCommands < ARRAY_LENGTH(mCommands)) {
        mVec2_c pos(0.0f, 0.0f);
        projectOntoMap(box->mPosition, pos);
        mCommands[mNumCommands].passIdx = 28;
        mCommands[mNumCommands].paneIdx = MAP_ICON_01_PANE_GODDESSC_00;
        mCommands[mNumCommands].position.x = pos.x;
        mCommands[mNumCommands].position.y = pos.y;

        mNumCommandsPerPass[28]++;
        mNumCommands++;
    }
}

void dLytMapIcon01_c::setupObjShutterDrawCommand(dAcBase_c *actor) {
    // TODO - needs door classes
}

void dLytMapIcon01_c::setupObjDoorDrawCommand(dAcBase_c *actor) {
    // TODO - needs door classes
}

void dLytMapIcon01_c::setupObjSaveDrawCommand(dAcBase_c *actor) {
    if (mNumCommands < ARRAY_LENGTH(mCommands)) {
        if (actor->mProfileName == fProfile::OBJ_SAVE && !actor->checkActorProperty(dAcBase_c::AC_PROP_0x100) &&
            mCurrentFloor == dStage_c::GetInstance()->getMapRelated()->fn_801B4F10(actor->mRoomID, actor->mPosition)) {
            dAcOSave_c *save = static_cast<dAcOSave_c *>(actor);

            bool b1 = false;
            bool b2 = false;
            bool activated = save->checkStatueFlag();
            if (field_0x0C70 == 1) {
                if (checkHasMap()) {
                    if (field_0x0C84) {
                        if (!activated) {
                            b1 = true;
                            b2 = true;
                        } else {
                            b1 = true;
                        }
                    } else {
                        b1 = true;
                    }
                } else if (activated) {
                    b1 = true;
                }
            } else if (activated) {
                b1 = true;
            }

            mVec2_c pos(0.0f, 0.0f);
            projectOntoMap(actor->mPosition, pos);
            mCommands[mNumCommands].passIdx = 9;
            mCommands[mNumCommands].paneIdx = MAP_ICON_01_PANE_SAVEICON_00;
            mCommands[mNumCommands].position.x = pos.x;
            mCommands[mNumCommands].position.y = pos.y;

            if (b1) {
                if (save->getStatueType() == dAcOSave_c::TYPE_NORMAL) {
                    field_0x0D4B[mNumCommands] = 0;
                } else if (save->getStatueType() == dAcOSave_c::TYPE_OVERWORLD) {
                    field_0x0D4B[mNumCommands] = 1;
                } else if (save->getStatueType() == dAcOSave_c::TYPE_DUNGEON) {
                    field_0x0D4B[mNumCommands] = 2;
                } else {
                    field_0x0D4B[mNumCommands] = 0;
                }

                if (b2) {
                    mIconAnims[mNumAnims].cmdIndex = mNumCommands;
                    mIconAnims[mNumAnims].visible = true;
                    mIconAnims[mNumAnims].animIn = true;
                } else {
                    mIconAnims[mNumAnims].cmdIndex = mNumCommands;
                    mIconAnims[mNumAnims].visible = true;
                    mIconAnims[mNumAnims].animIn = false;
                }
            } else {
                mIconAnims[mNumAnims].cmdIndex = mNumCommands;
                mIconAnims[mNumAnims].visible = false;
                mIconAnims[mNumAnims].animIn = false;
            }

            if (field_0x0C70 != 4 && mIconAnims[mNumAnims].visible == true &&
                field_0x0E13 < MAP_ICON_01_BOUNDING_COMMONICON_LAST - MAP_ICON_01_BOUNDING_COMMONICON_00) {
                s32 idx = field_0x0E13 + MAP_ICON_01_BOUNDING_COMMONICON_00;
                u32 id = save->getStatueNameId();
                mPopupConfigs[idx].cmdIdx = mNumCommands;
                mPopupConfigs[idx].boundingPos.x = pos.x;
                mPopupConfigs[idx].boundingPos.y = pos.y;
                mPopupConfigs[idx].objId = id;
                mPopupConfigs[idx].labelPrefix = sSaveObjPrefix;
                nw4r::lyt::Bounding *b = mPopupConfigs[idx].bounding;
                nw4r::lyt::Size bSize = mpBoundings[MAP_ICON_01_BOUNDING_SAVEICON_00]->GetSize();
                b->SetSize(bSize);
                mVec2_c textPos(0.0f, 0.0f);
                fn_80181880(
                    mpSpecialPictures[MAP_ICON_01_SPECIAL_PICTURE_SAVEICON_01],
                    mpSpecialPanes[MAP_ICON_01_SPECIAL_PANE_SAVEICON_000], textPos
                );

                mPopupConfigs[idx].textPos.x = textPos.x;
                mPopupConfigs[idx].textPos.y = textPos.y;
                mPopupConfigs[idx].bounding->SetVisible(true);
                field_0x0E13++;
            }

            mNumAnims++;
            mNumCommandsPerPass[9]++;
            mNumCommands++;
        }
    }
}

void dLytMapIcon01_c::setupObjLightLineDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupNpcBNusiDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupObjTerryShopDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupObjRoAtTargetDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupObjD3DummyDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupObjSealedDoorDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupObjHarpHintDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupNpcBeeDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupStageDrawCommands() {
    dAcBase_c *ac = nullptr;
    while ((ac = static_cast<dAcBase_c *>(fManager_c::searchBaseByGroupType(fBase_c::STAGE, ac))) != nullptr) {
        setupTgMapMarkDrawCommand(ac);

        if (dLytMapGlobal_c::GetInstance()->isInMapEvent()) {
            setupTgMapInstDrawCommand(ac);
        }

        setupTgForceGetFlagDrawCommand(ac);

        if (field_0x184D) {
            setupTgInsectDrawCommand(ac);
        }

        if (mNumCommands >= ARRAY_LENGTH(mCommands)) {
            break;
        }
    }
}

void dLytMapIcon01_c::setupTgMapMarkDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupTgMapMarkDrawCommand1(dTgMapMark_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupTgMapMarkDrawCommand2(dTgMapMark_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupTgMapInstDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupTgForceGetFlagDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupTgInsectDrawCommand(dAcBase_c *actor) {
    // TODO - ...
}

void dLytMapIcon01_c::setupLinkDrawCommand() {
    if (dAcPy_c::GetLinkM() != nullptr) {
        setupLinkDrawCommand1(dAcPy_c::GetLinkM());

        if (dLytMapGlobal_c::GetInstance()->getMapEvent() != dLytMapMain_c::MAP_EVENT_MAP_INTRO) {
            setupLinkDrawCommand2();
        }
    }
}

void dLytMapIcon01_c::setupLinkDrawCommand1(dAcPy_c *ac) {
    // TODO - ...
}

void dLytMapIcon01_c::setupLinkDrawCommand2() {
    // TODO - ...
}

void dLytMapIcon01_c::setupCloudBigDrawCommand() {
    if (mNumCommands < ARRAY_LENGTH(mCommands)) {
        mVec3_c thunderHeadPos(-135210.0f, 0.0f, -84790.0f);
        mVec2_c pos(0.0f, 0.0f);
        projectOntoMap(thunderHeadPos, pos);
        mCommands[mNumCommands].passIdx = 31;
        mCommands[mNumCommands].paneIdx = MAP_ICON_01_PANE_CLOUDBIG_000;
        mCommands[mNumCommands].position.x = pos.x;
        mCommands[mNumCommands].position.y = pos.y;

        mNumCommandsPerPass[31]++;
        mNumCommands++;
    }
}

void dLytMapIcon01_c::setupTriforceDrawCommands() {
    dAcObjBase_c *ac = nullptr;
    while ((ac = dAcObjBase_c::getNextObject(&dAcItem_c::sItemList, ac)) != nullptr) {
        if (static_cast<dAcItem_c *>(ac)->isTriforce() && !ac->checkActorProperty(dAcBase_c::AC_PROP_0x100) &&
            mCurrentFloor == dStage_c::GetInstance()->getMapRelated()->fn_801B4F10(ac->mRoomID, ac->mPosition)) {
            // @bug mNumCommands not checked?
            mVec2_c pos(0.0f, 0.0f);
            projectOntoMap(ac->mPosition, pos);
            mCommands[mNumCommands].passIdx = 23;
            mCommands[mNumCommands].paneIdx = MAP_ICON_01_PANE_TRIFORCE_00;
            mCommands[mNumCommands].position.x = pos.x;
            mCommands[mNumCommands].position.y = pos.y;

            mNumCommandsPerPass[23]++;
            mNumCommands++;
        }
    }
}

void dLytMapIcon01_c::fn_80181880(nw4r::lyt::Pane *a, nw4r::lyt::Pane *b, mVec2_c &result) const {
    // TODO - maybe equivalent but floating point hell
    // Ignoring the get16x9to4x3WidthScaleF stuff this basically does
    // (a->translate + a->parent->translate) * b->scale + (Ex * a->size * a->scale * b->scale / 2.0f)
    mVec2_c scaleA = a->GetScale();
    mVec2_c scaleB = b->GetScale();

    mVec3_c parentTranslateA = a->GetParent()->GetTranslate();
    parentTranslateA.x /= dGfx_c::get16x9to4x3WidthScaleF();

    mVec3_c translateA = a->GetTranslate();
    translateA.x /= dGfx_c::get16x9to4x3WidthScaleF();
    translateA.x += parentTranslateA.x;
    translateA.x *= scaleB.x;
    translateA.y += parentTranslateA.y;
    translateA.y *= scaleB.y;

    nw4r::lyt::Size sizeA = a->GetSize();
    sizeA.width /= dGfx_c::get16x9to4x3WidthScaleF();
    sizeA.width *= scaleA.x;
    sizeA.width *= scaleB.x;
    sizeA.height *= scaleA.y;
    sizeA.height *= scaleB.y;

    translateA.x += (sizeA.width / 2.0f);

    result.x = translateA.x;
    result.y = translateA.y;
}

void dLytMapIcon01_c::recordAnimVisState() {
    for (int i = 0; i < mNumAnims; i++) {
        mIconAnims[i].prevVisible = mIconAnims[i].visible;
    }
}

void dLytMapIcon01_c::transitionAnims() {
    for (int i = 0; i < mNumAnims; i++) {
        if (mIconAnims[i].visible != mIconAnims[i].prevVisible) {
            if (mIconAnims[i].prevVisible == true) {
                mIconAnims[i].animIn = false;
                mIconAnims[i].animOut = true;
            } else if (mIconAnims[i].visible == true) {
                mIconAnims[i].animIn = true;
                mIconAnims[i].animOut = false;
            }
        }
    }
}

void dLytMapIcon01_c::checkBoundingPointing() {
    for (int i = 0; i < MAP_ICON_01_BOUNDING_COMMONICON_LAST; i++) {
        if (mPopupConfigs[i].bounding->IsVisible() && isPointingAtBounding(mPopupConfigs[i].bounding)) {
            if (i == MAP_ICON_01_BOUNDING_UTA_00) {
                mPopupConfigs[MAP_ICON_01_BOUNDING_UTA_01].pointedAt = true;
            } else if (i == MAP_ICON_01_BOUNDING_MUSI_00) {
                mPopupConfigs[MAP_ICON_01_BOUNDING_MUSI_01].pointedAt = true;
            } else if (i == MAP_ICON_01_BOUNDING_SKYLOFT_00) {
                mPopupConfigs[MAP_ICON_01_BOUNDING_SKYLOFT_01].pointedAt = true;
            } else {
                mPopupConfigs[i].pointedAt = true;
            }
        }
    }
}

bool dLytMapIcon01_c::isPointingAtBounding(nw4r::lyt::Pane *pane) const {
    dCursorHitCheck_c *d = dCsBase_c::GetInstance()->getHitCheck();
    if (d != nullptr && d->getType() == 'lyt ' && static_cast<dCursorHitCheckLyt_c *>(d)->getHitPane() == pane) {
        return true;
    }
    return false;
}

void dLytMapIcon01_c::fn_80181C40() {
    for (int i = 0; i < MAP_ICON_01_BOUNDING_COMMONICON_LAST; i++) {
        mpBoundings[i]->SetVisible(false);
    }
}

bool dLytMapIcon00_c::build(d2d::ResAccIf_c *resAcc) {
    // TODO - ...
    return true;
}

bool dLytMapIcon00_c::remove() {
    // TODO - ...
    return true;
}

bool dLytMapIcon00_c::execute() {
    // TODO - ...
    return true;
}

void dLytMapIcon00_c::draw() {
    // TODO - ...
}

void dLytMapIcon00_c::resetDrawCommands() {
    for (int i = 0; i < (int)ARRAY_LENGTH(mCommands); i++) {
        mCommands[i].position.set(0.0f, 0.0f);
        mCommands[i].rotation = 0.0f;
        mCommands[i].paneIdx = 39;
        mCommands[i].passIdx = 35;
        field_0x0F20[i] = ARRAY_LENGTH(mCommands);
        field_0x13A8[i] = 0;
        field_0x1342[i] = 0;
        field_0x0E5B[i] = 0;
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(mNumCommandsPerPass); i++) {
        mNumCommandsPerPass[i] = 0;
    }

    field_0x13A7 = 0;
    mNumCommands = 0;

    for (int i = 0; i < 36; i++) {
        field_0x0EEC[i] = 0;
    }

    for (int i = 0; i < (int)ARRAY_LENGTH(mPopupConfigs); i++) {
        mPopupConfigs[i].cmdIdx = ARRAY_LENGTH(mCommands);
        mPopupConfigs[i].boundingPos.set(0.0f, 0.0f);
        mPopupConfigs[i].textPos.set(0.0f, 0.0f);
        mPopupConfigs[i].pointedAt = false;
        mPopupConfigs[i].objId = 0;
        mPopupConfigs[i].labelPrefix = sMapPrefix;
    }

    field_0x140C = 0;
    field_0x0E59 = 0;
    field_0x0E5A = 0;

    fn_80189750();

    field_0x0E50 = 0;
    field_0x0E51 = 0;
    field_0x0E52 = 0;
    field_0x0E53 = 0;
    field_0x0E54 = 0;
    field_0x0E55 = 0;
    field_0x0E56 = 0;
    field_0x0E57 = 0;
    field_0x0E58 = 0;

    for (int i = 0; i < (int)ARRAY_LENGTH(mIconAnims); i++) {
        mIconAnims[i].render = false;
    }

    mNumAnims = 0;
    field_0x1CE4 = 0;
}

static const char *sMapPopPrefix = "MAP_POP_";

bool dLytMapMarkers_c::build(d2d::ResAccIf_c *resAcc) {
    mMapPopup.build(resAcc);
    if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
        void *data = LayoutArcManager::GetInstance()->getLoadedData("MapSky");
        mResAcc01.attach(data, "");

        // TODO - ...

        mIcon01.build(&mResAcc01);
    } else {
        void *data = LayoutArcManager::GetInstance()->getLoadedData("MapField");
        mResAcc00.attach(data, "");

        // TODO - ...

        mIcon00.build(&mResAcc00);
    }
    return true;
}

bool dLytMapMarkers_c::remove() {
    mMapPopup.remove();
    if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
        mIcon01.remove();
        mResAcc01.detach();
    } else {
        mIcon00.remove();
        mResAcc00.detach();
    }
    // TODO - ...
    return true;
}

bool dLytMapMarkers_c::execute() {
    return true;
}

void dLytMapMarkers_c::draw() {
    if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
        mIcon01.draw();
    } else {
        mIcon00.draw();
    }
}

void dLytMapMarkers_c::setIslandNamesOn(bool on) {
    if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
        mShowIslandNames = on;
        mIcon01.mShowIslandNames = on;
    }
}

void dLytMapMarkers_c::drawPopups() {
    for (int i = 0; i < mNumPopups; i++) {
        if (mPopups[i].visible) {
            SizedString<0x40> label;
            if (mPopups[i].objId == 0xFF) {
                label.sprintf("SAVEOBJ_NAME_UNKNOWN");
            } else {
                label.sprintf("%s%02d", mPopups[i].labelPrefix, mPopups[i].objId);
            }
            mMapPopup.setLabel(label);
            mMapPopup.realizeText();
            f32 width = mMapPopup.getTextRenderWidth();

            // TODO - probably equivalent but FPR, order, stack problems
            mVec2_c boundingPos = mPopups[i].boundingPos;
            mVec2_c textPos = mPopups[i].textPos;
            mVec2_c pos = boundingPos + textPos + field_0x06F4;
            pos.x += width / 2.0f;

            mMapPopup.setPosition(pos);

            mMapPopup.setInout(mPopups[i].inAnimFrame);
            mMapPopup.draw();
            if (mPopups[i].inAnimFrame <= mMapPopup.getInAnimDuration()) {
                mPopups[i].inAnimFrame += 1.0f;
            }
            if (mMapPopup.getInAnimDuration() <= mPopups[i].inAnimFrame) {
                mPopups[i].inAnimFrame = mMapPopup.getInAnimDuration();
            }
        } else if (0.0f < mPopups[i].inAnimFrame) {
            SizedString<0x40> label;
            if (mPopups[i].objId == 0xFF) {
                label.sprintf("SAVEOBJ_NAME_UNKNOWN");
            } else {
                label.sprintf("%s%02d", mPopups[i].labelPrefix, mPopups[i].objId);
            }
            mMapPopup.setLabel(label);
            mMapPopup.realizeText();
            f32 width = mMapPopup.getTextRenderWidth();

            // TODO - probably equivalent but FPR, order, stack problems
            mVec2_c boundingPos = mPopups[i].boundingPos;
            mVec2_c textPos = mPopups[i].textPos;
            mVec2_c pos = boundingPos + textPos + field_0x06F4;
            pos.x += width / 2.0f;

            mMapPopup.setPosition(pos);

            mPopups[i].inAnimFrame -= 1.0f;
            mMapPopup.setInout(mPopups[i].inAnimFrame);
            mMapPopup.draw();
        }
    }
}

void dLytMapMarkers_c::resetPopups() {
    for (int i = 0; i < (int)ARRAY_LENGTH(mPopups); i++) {
        mPopups[i].inAnimFrame = 0.0f;
        mPopups[i].textPos.set(0.0f, 0.0f);
        mPopups[i].boundingPos.set(0.0f, 0.0f);
        mPopups[i].visible = false;
        mPopups[i].objId = 0;
        mPopups[i].labelPrefix = sMapPopPrefix;
    }
}

void dLytMapMarkers_c::loadPopups() {
    for (int i = 0; i < mNumPopups; i++) {
        // TODO - this looks weird
        const mVec2_c &v = mpPopupConfigs[i].textPos;
        mPopups[i].textPos.x = v.x;
        mPopups[i].textPos.y = v.y;
        mPopups[i].boundingPos = mpPopupConfigs[i].boundingPos;
        mPopups[i].visible = mpPopupConfigs[i].pointedAt;
        mPopups[i].objId = mpPopupConfigs[i].objId;
        mPopups[i].labelPrefix = mpPopupConfigs[i].labelPrefix;
    }
}
