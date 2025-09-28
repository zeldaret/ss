#include "d/lyt/d_lyt_map_markers.h"

#include "common.h"
#include "d/d_message.h"
#include "d/lyt/d_lyt_map_global.h"
#include "d/lyt/d_window.h"
#include "m/m_vec.h"

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
    mVec3_c pos;
    pos.x = mPosition.x;
    pos.y = mPosition.y;
    pos.z = 0.0f;
    mpPanes[MAP_POPUP_PANE_ALL]->SetTranslate(pos);
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
