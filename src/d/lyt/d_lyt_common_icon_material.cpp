#include "d/lyt/d_lyt_common_icon_material.h"

#include "common.h"
#include "d/d_cs_base.h"

#include <cstring>

STATE_DEFINE(dLytCommonIconMaterial_c, None);
STATE_DEFINE(dLytCommonIconMaterial_c, In);
STATE_DEFINE(dLytCommonIconMaterial_c, Wait);
STATE_DEFINE(dLytCommonIconMaterial_c, Out);

static const d2d::LytBrlanMapping brlanMapPart1[] = {
    {      "commonIcon_00_bgV.brlan",       "G_bgV_00"},
    {  "commonIcon_00_numberV.brlan",   "G_numberV_00"},
    {     "commonIcon_00_bugs.brlan",      "G_icon_00"},
    {   "commonIcon_00_shadow.brlan",    "G_shadow_00"},
    {"commonIcon_00_textColor.brlan", "G_textColor_00"},
    {    "commonIcon_00_onOff.brlan",       "G_btn_00"},
};

// S = shadow
static const char *part1TextBoxes[] = {
    "T_comNumberS_00",
    "T_comNumber_00",
};

static const char *part1Bounding = "B_bug_00";

#define MATERIAL_PART_1_ANIM_BG 0
#define MATERIAL_PART_1_ANIM_NUMBERV 1
#define MATERIAL_PART_1_ANIM_ICON 2
#define MATERIAL_PART_1_ANIM_SHADOW 3
#define MATERIAL_PART_1_ANIM_TEXTCOLOR 4
#define MATERIAL_PART_1_ANIM_ONOFF 5

#define MATERIAL_PART_1_NUM_ANIMS 6

bool dLytCommonIconMaterialPart1_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("commonIcon_00.brlyt", nullptr);
    for (int i = 0; i < MATERIAL_PART_1_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapPart1[i].mFile, resAcc, mLyt.getLayout(), brlanMapPart1[i].mName);
        mAnm[i].setDirection(false);
        mAnm[i].setRate(1.0f);
    }

    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(part1TextBoxes[i]);
    }

    mpBounding = mLyt.findBounding(part1Bounding);
    reset();
    return true;
}

bool dLytCommonIconMaterialPart1_c::remove() {
    for (int i = 0; i < MATERIAL_PART_1_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].afterUnbind();
    }
    return true;
}

bool dLytCommonIconMaterialPart1_c::execute() {
    if (mAnm[MATERIAL_PART_1_ANIM_ONOFF].isEnabled() && mAnm[MATERIAL_PART_1_ANIM_ONOFF].isStop2()) {
        mAnm[MATERIAL_PART_1_ANIM_ONOFF].setAnimEnable(false);
    }

    for (int i = 0; i < MATERIAL_PART_1_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    return true;
}

void dLytCommonIconMaterialPart1_c::reset() {
    mAnm[MATERIAL_PART_1_ANIM_ICON].setFrame(0.0f);
    mAnm[MATERIAL_PART_1_ANIM_ICON].setAnimEnable(true);

    mAnm[MATERIAL_PART_1_ANIM_BG].setFrame(0.0f);
    mAnm[MATERIAL_PART_1_ANIM_BG].setAnimEnable(true);

    mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setFrame(0.0f);
    mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setAnimEnable(true);

    mAnm[MATERIAL_PART_1_ANIM_SHADOW].setFrame(0.0f);
    mAnm[MATERIAL_PART_1_ANIM_SHADOW].setAnimEnable(true);

    mAnm[MATERIAL_PART_1_ANIM_TEXTCOLOR].setFrame(0.0f);
    mAnm[MATERIAL_PART_1_ANIM_TEXTCOLOR].setAnimEnable(true);

    mAnm[MATERIAL_PART_1_ANIM_ONOFF].setBackwardsOnce();
    mAnm[MATERIAL_PART_1_ANIM_ONOFF].setToEnd2();
    mAnm[MATERIAL_PART_1_ANIM_ONOFF].setAnimEnable(true);

    mLyt.calc();

    mAnm[MATERIAL_PART_1_ANIM_ONOFF].setAnimEnable(false);
    mAnm[MATERIAL_PART_1_ANIM_TEXTCOLOR].setAnimEnable(false);
    mAnm[MATERIAL_PART_1_ANIM_SHADOW].setAnimEnable(false);
    mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setAnimEnable(false);
    mAnm[MATERIAL_PART_1_ANIM_BG].setAnimEnable(false);
    mAnm[MATERIAL_PART_1_ANIM_ICON].setAnimEnable(false);
}

void dLytCommonIconMaterialPart1_c::setItem(u8 item) {
    realizeBg();
    realizeNumberV();
    realizeShadow();
    realizeItem(item);
}

void dLytCommonIconMaterialPart1_c::realizeBg() {
    if (mBg) {
        mAnm[MATERIAL_PART_1_ANIM_BG].setFrame(1.0f);
    } else {
        mAnm[MATERIAL_PART_1_ANIM_BG].setFrame(0.0f);
    }

    if (mShadow) {
        mAnm[MATERIAL_PART_1_ANIM_BG].setFrame(0.0f);
    }

    mAnm[MATERIAL_PART_1_ANIM_BG].setAnimEnable(true);
    mLyt.calc();
    mAnm[MATERIAL_PART_1_ANIM_BG].setAnimEnable(false);
}

void dLytCommonIconMaterialPart1_c::realizeNumberV() {
    if (mNumberV) {
        mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setFrame(1.0f);
    } else {
        mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setFrame(0.0f);
    }

    if (mShadow) {
        mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setFrame(0.0f);
    }

    mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setAnimEnable(true);
    mLyt.calc();
    mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setAnimEnable(false);
}

void dLytCommonIconMaterialPart1_c::realizeShadow() {
    if (mShadow) {
        mAnm[MATERIAL_PART_1_ANIM_SHADOW].setFrame(1.0f);
    } else {
        mAnm[MATERIAL_PART_1_ANIM_SHADOW].setFrame(0.0f);
    }

    mAnm[MATERIAL_PART_1_ANIM_SHADOW].setAnimEnable(true);
    mLyt.calc();
    mAnm[MATERIAL_PART_1_ANIM_SHADOW].setAnimEnable(false);
}

void dLytCommonIconMaterialPart1_c::realizeItem(u8 item) {
    // Inline?
    if (mAnm[MATERIAL_PART_1_ANIM_ICON].getEndFrameRaw() - 1.0f < item) {
        mItem = 0;
    } else {
        mItem = item;
    }

    mAnm[MATERIAL_PART_1_ANIM_ICON].setFrame(mItem);
    mAnm[MATERIAL_PART_1_ANIM_ICON].setAnimEnable(true);
    mLyt.calc();
    mAnm[MATERIAL_PART_1_ANIM_ICON].setAnimEnable(false);
    mpBounding->SetVisible(true);
}

void dLytCommonIconMaterialPart1_c::setNumber(s32 number) {
    std::memset(mNumberBuf, 0, sizeof(mNumberBuf));
    swprintf(mNumberBuf, ARRAY_LENGTH(mNumberBuf), L"%d", number);
    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i]->fn_800AF930(mNumberBuf);
    }
}

void dLytCommonIconMaterialPart1_c::setOn() {
    if (mAnm[MATERIAL_PART_1_ANIM_ONOFF].isPlayingBackwardsOnce()) {
        mAnm[MATERIAL_PART_1_ANIM_ONOFF].setForwardOnce();
        mAnm[MATERIAL_PART_1_ANIM_ONOFF].setAnimEnable(true);
    }
}

void dLytCommonIconMaterialPart1_c::setOff() {
    if (mAnm[MATERIAL_PART_1_ANIM_ONOFF].isPlayingForwardsOnce()) {
        mAnm[MATERIAL_PART_1_ANIM_ONOFF].setBackwardsOnce();
        mAnm[MATERIAL_PART_1_ANIM_ONOFF].setAnimEnable(true);
    }
}

bool dLytCommonIconMaterialPart1_c::isCursorOver() const {
    d2d::dLytStructD *d = dCsBase_c::GetInstance()->getUnk();
    if (d != nullptr && d->getType() == 'lyt ' && d->field_0x24 == mpBounding) {
        return true;
    }
    return false;
}

void dLytCommonIconMaterialPart1_c::setVisible(bool visible) {
    if (visible) {
        mpBounding->SetVisible(true);
    } else {
        mpBounding->SetVisible(false);
    }
}

static const d2d::LytBrlanMapping brlanMapPart2[] = {
    {      "commonIcon_01_bgV.brlan",       "G_bgV_00"},
    {  "commonIcon_01_numberV.brlan",   "G_numberV_00"},
    {    "commonIcon_01_sozai.brlan",      "G_icon_00"},
    {   "commonIcon_01_shadow.brlan",    "G_shadow_00"},
    {"commonIcon_01_textColor.brlan", "G_textColor_00"},
    {    "commonIcon_01_onOff.brlan",       "G_btn_00"},
};

// S = shadow
static const char *part2TextBoxes[] = {
    "T_comNumberS_00",
    "T_comNumber_00",
};

static const char *part2Bounding = "B_sozai_00";

#define MATERIAL_PART_2_ANIM_BG 0
#define MATERIAL_PART_2_ANIM_NUMBERV 1
#define MATERIAL_PART_2_ANIM_ICON 2
#define MATERIAL_PART_2_ANIM_SHADOW 3
#define MATERIAL_PART_2_ANIM_TEXTCOLOR 4
#define MATERIAL_PART_2_ANIM_ONOFF 5

#define MATERIAL_PART_2_NUM_ANIMS 6

bool dLytCommonIconMaterialPart2_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("commonIcon_01.brlyt", nullptr);
    for (int i = 0; i < MATERIAL_PART_2_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapPart2[i].mFile, resAcc, mLyt.getLayout(), brlanMapPart2[i].mName);
        mAnm[i].setDirection(false);
        mAnm[i].setRate(1.0f);
    }

    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(part2TextBoxes[i]);
    }

    mpBounding = mLyt.findBounding(part2Bounding);
    reset();
    return true;
}

bool dLytCommonIconMaterialPart2_c::remove() {
    for (int i = 0; i < MATERIAL_PART_2_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].afterUnbind();
    }
    return true;
}

bool dLytCommonIconMaterialPart2_c::execute() {
    if (mAnm[MATERIAL_PART_2_ANIM_ONOFF].isEnabled() && mAnm[MATERIAL_PART_2_ANIM_ONOFF].isStop2()) {
        mAnm[MATERIAL_PART_2_ANIM_ONOFF].setAnimEnable(false);
    }

    for (int i = 0; i < MATERIAL_PART_2_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    return true;
}

void dLytCommonIconMaterialPart2_c::reset() {
    mAnm[MATERIAL_PART_2_ANIM_ICON].setFrame(0.0f);
    mAnm[MATERIAL_PART_2_ANIM_ICON].setAnimEnable(true);

    mAnm[MATERIAL_PART_2_ANIM_BG].setFrame(0.0f);
    mAnm[MATERIAL_PART_2_ANIM_BG].setAnimEnable(true);

    mAnm[MATERIAL_PART_2_ANIM_NUMBERV].setFrame(0.0f);
    mAnm[MATERIAL_PART_2_ANIM_NUMBERV].setAnimEnable(true);

    mAnm[MATERIAL_PART_2_ANIM_SHADOW].setFrame(0.0f);
    mAnm[MATERIAL_PART_2_ANIM_SHADOW].setAnimEnable(true);

    mAnm[MATERIAL_PART_2_ANIM_TEXTCOLOR].setFrame(0.0f);
    mAnm[MATERIAL_PART_2_ANIM_TEXTCOLOR].setAnimEnable(true);

    mAnm[MATERIAL_PART_2_ANIM_ONOFF].setBackwardsOnce();
    mAnm[MATERIAL_PART_2_ANIM_ONOFF].setToEnd2();
    mAnm[MATERIAL_PART_2_ANIM_ONOFF].setAnimEnable(true);

    mLyt.calc();

    mAnm[MATERIAL_PART_2_ANIM_ONOFF].setAnimEnable(false);
    mAnm[MATERIAL_PART_2_ANIM_TEXTCOLOR].setAnimEnable(false);
    mAnm[MATERIAL_PART_2_ANIM_SHADOW].setAnimEnable(false);
    mAnm[MATERIAL_PART_2_ANIM_NUMBERV].setAnimEnable(false);
    mAnm[MATERIAL_PART_2_ANIM_BG].setAnimEnable(false);
    mAnm[MATERIAL_PART_2_ANIM_ICON].setAnimEnable(false);
}

void dLytCommonIconMaterialPart2_c::setItem(u8 item) {
    realizeBg();
    realizeNumberV();
    realizeShadow();
    realizeItem(item);
}

void dLytCommonIconMaterialPart2_c::realizeBg() {
    if (mBg) {
        mAnm[MATERIAL_PART_1_ANIM_BG].setFrame(1.0f);
    } else {
        mAnm[MATERIAL_PART_1_ANIM_BG].setFrame(0.0f);
    }

    if (mShadow) {
        mAnm[MATERIAL_PART_1_ANIM_BG].setFrame(0.0f);
    }

    mAnm[MATERIAL_PART_1_ANIM_BG].setAnimEnable(true);
    mLyt.calc();
    mAnm[MATERIAL_PART_1_ANIM_BG].setAnimEnable(false);
}

void dLytCommonIconMaterialPart2_c::realizeNumberV() {
    if (mNumberV) {
        mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setFrame(1.0f);
    } else {
        mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setFrame(0.0f);
    }

    if (mShadow) {
        mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setFrame(0.0f);
    }

    mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setAnimEnable(true);
    mLyt.calc();
    mAnm[MATERIAL_PART_1_ANIM_NUMBERV].setAnimEnable(false);
}

void dLytCommonIconMaterialPart2_c::realizeShadow() {
    if (mShadow) {
        mAnm[MATERIAL_PART_1_ANIM_SHADOW].setFrame(1.0f);
    } else {
        mAnm[MATERIAL_PART_1_ANIM_SHADOW].setFrame(0.0f);
    }

    mAnm[MATERIAL_PART_1_ANIM_SHADOW].setAnimEnable(true);
    mLyt.calc();
    mAnm[MATERIAL_PART_1_ANIM_SHADOW].setAnimEnable(false);
}

void dLytCommonIconMaterialPart2_c::realizeItem(u8 item) {
    // Inline?
    if (mAnm[MATERIAL_PART_2_ANIM_ICON].getEndFrameRaw() - 1.0f < item) {
        mItem = 0;
    } else {
        mItem = item;
    }

    mAnm[MATERIAL_PART_2_ANIM_ICON].setFrame(mItem);
    mAnm[MATERIAL_PART_2_ANIM_ICON].setAnimEnable(true);
    mLyt.calc();
    mAnm[MATERIAL_PART_2_ANIM_ICON].setAnimEnable(false);
    mpBounding->SetVisible(true);
}

void dLytCommonIconMaterialPart2_c::setNumber(s32 number) {
    std::memset(mNumberBuf, 0, sizeof(mNumberBuf));
    swprintf(mNumberBuf, ARRAY_LENGTH(mNumberBuf), L"%d", number);
    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i]->fn_800AF930(mNumberBuf);
    }
}

void dLytCommonIconMaterialPart2_c::setOn() {
    if (mAnm[MATERIAL_PART_2_ANIM_ONOFF].isPlayingBackwardsOnce()) {
        mAnm[MATERIAL_PART_2_ANIM_ONOFF].setForwardOnce();
        mAnm[MATERIAL_PART_2_ANIM_ONOFF].setAnimEnable(true);
    }
}

void dLytCommonIconMaterialPart2_c::setOff() {
    if (mAnm[MATERIAL_PART_2_ANIM_ONOFF].isPlayingForwardsOnce()) {
        mAnm[MATERIAL_PART_2_ANIM_ONOFF].setBackwardsOnce();
        mAnm[MATERIAL_PART_2_ANIM_ONOFF].setAnimEnable(true);
    }
}

bool dLytCommonIconMaterialPart2_c::isCursorOver() const {
    d2d::dLytStructD *d = dCsBase_c::GetInstance()->getUnk();
    if (d != nullptr && d->getType() == 'lyt ' && d->field_0x24 == mpBounding) {
        return true;
    }
    return false;
}

void dLytCommonIconMaterialPart2_c::setVisible(bool visible) {
    if (visible) {
        mpBounding->SetVisible(true);
    } else {
        mpBounding->SetVisible(false);
    }
}

// Incredible use of the state manager here
void dLytCommonIconMaterial_c::initializeState_None() {}
void dLytCommonIconMaterial_c::executeState_None() {}
void dLytCommonIconMaterial_c::finalizeState_None() {}

void dLytCommonIconMaterial_c::initializeState_In() {}
void dLytCommonIconMaterial_c::executeState_In() {}
void dLytCommonIconMaterial_c::finalizeState_In() {}

void dLytCommonIconMaterial_c::initializeState_Wait() {}
void dLytCommonIconMaterial_c::executeState_Wait() {}
void dLytCommonIconMaterial_c::finalizeState_Wait() {}

void dLytCommonIconMaterial_c::initializeState_Out() {}
void dLytCommonIconMaterial_c::executeState_Out() {}
void dLytCommonIconMaterial_c::finalizeState_Out() {}

bool dLytCommonIconMaterial_c::build(d2d::ResAccIf_c *resAcc) {
    build(resAcc, 1);
    return true;
}

bool dLytCommonIconMaterial_c::build(d2d::ResAccIf_c *resAcc, u8 variant) {
    mPart = variant;
    switch (mPart) {
        case 0: mPart1.build(resAcc); break;
        case 1: mPart2.build(resAcc); break;
    }
    return true;
}

bool dLytCommonIconMaterial_c::remove() {
    switch (mPart) {
        case 0: mPart1.remove(); break;
        case 1: mPart2.remove(); break;
    }
    return true;
}

bool dLytCommonIconMaterial_c::execute() {
    switch (mPart) {
        case 0: mPart1.execute(); break;
        case 1: mPart2.execute(); break;
    }
    return true;
}

void dLytCommonIconMaterial_c::reset() {
    switch (mPart) {
        case 0: mPart1.reset(); break;
        case 1: mPart2.reset(); break;
    }
}

void dLytCommonIconMaterial_c::setBg(bool bg) {
    switch (mPart) {
        case 0: mPart1.setBg(bg); break;
        case 1: mPart2.setBg(bg); break;
    }
}

void dLytCommonIconMaterial_c::setHasNumber(bool hasNumber) {
    switch (mPart) {
        case 0: mPart1.setHasNumber(hasNumber); break;
        case 1: mPart2.setHasNumber(hasNumber); break;
    }
}

void dLytCommonIconMaterial_c::setShadow(bool shadow) {
    switch (mPart) {
        case 0: mPart1.setShadow(shadow); break;
        case 1: mPart2.setShadow(shadow); break;
    }
}

void dLytCommonIconMaterial_c::setItem(u8 item) {
    switch (mPart) {
        case 0: mPart1.setItem(item); break;
        case 1: mPart2.setItem(item); break;
    }
}

void dLytCommonIconMaterial_c::setNumber(s32 number) {
    switch (mPart) {
        case 0: mPart1.setNumber(number); break;
        case 1: mPart2.setNumber(number); break;
        case 2: break;
    }
}

void dLytCommonIconMaterial_c::setOn() {
    switch (mPart) {
        case 0: mPart1.setOn(); break;
        case 1: mPart2.setOn(); break;
        case 2: break;
    }
}

void dLytCommonIconMaterial_c::setOff() {
    switch (mPart) {
        case 0: mPart1.setOff(); break;
        case 1: mPart2.setOff(); break;
        case 2: break;
    }
}

bool dLytCommonIconMaterial_c::isCursorOver() const {
    switch (mPart) {
        case 0:
            if (mPart1.isCursorOver()) {
                return true;
            }
            break;
        case 1:
            if (mPart2.isCursorOver()) {
                return true;
            }
            break;
    }
    return false;
}

void dLytCommonIconMaterial_c::setVisible(bool visible) {
    switch (mPart) {
        case 0: mPart1.setVisible(visible); break;
        case 1: mPart2.setVisible(visible); break;
    }
}

const nw4r::lyt::Bounding *dLytCommonIconMaterial_c::getBounding() const {
    const nw4r::lyt::Bounding *result = nullptr;
    switch (mPart) {
        case 0: result = mPart1.getBounding(); break;
        case 1: result = mPart2.getBounding(); break;
    }
    return result;
}
