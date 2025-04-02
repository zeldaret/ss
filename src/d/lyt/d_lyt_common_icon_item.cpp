#include "d/lyt/d_lyt_common_icon_item.h"

#include "common.h"
#include "d/d_cs_base.h"
#include "d/lyt/d_lyt_common_icon_item_maps.h"
#include "d/lyt/d_structd.h"

#include <cstring>

STATE_DEFINE(dLytCommonIconItem_c, None);
STATE_DEFINE(dLytCommonIconItem_c, In);
STATE_DEFINE(dLytCommonIconItem_c, Wait);
STATE_DEFINE(dLytCommonIconItem_c, Out);

static const d2d::LytBrlanMapping brlanMapPart1[] = {
    {     "commonIcon_02_size.brlan",      "G_size_00"},
    {"commonIcon_02_bocoburin.brlan", "G_bocoburin_00"},
    {  "commonIcon_02_numberV.brlan",   "G_numberV_00"},
    {     "commonIcon_02_item.brlan",      "G_icon_00"},
    {"commonIcon_02_textColor.brlan", "G_textColor_00"},
    {    "commonIcon_02_onOff.brlan",       "G_btn_00"},
    {   "commonIcon_02_decide.brlan",       "G_btn_00"},
};

// S = shadow
static const char *part1TextBoxes[] = {
    "T_comNumberS_01",
    "T_comNumber_01",
};

static const char *part1Bounding = "B_item_00";

#define ITEM_PART_1_ANIM_SIZE 0
#define ITEM_PART_1_ANIM_BOCOBURIN 1
#define ITEM_PART_1_ANIM_NUMBERV 2
#define ITEM_PART_1_ANIM_ICON 3
#define ITEM_PART_1_ANIM_TEXTCOLOR 4
#define ITEM_PART_1_ANIM_ONOFF 5
#define ITEM_PART_1_ANIM_DECIDE 6

#define ITEM_PART_1_NUM_ANIMS 7

bool dLytCommonIconItemPart1_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("commonIcon_02.brlyt", nullptr);
    for (int i = 0; i < ITEM_PART_1_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapPart1[i].mFile, resAcc, mLyt.getLayout(), brlanMapPart1[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
    }

    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(part1TextBoxes[i]);
    }

    mpBounding = mLyt.findBounding(part1Bounding);
    reset();
    return true;
}

bool dLytCommonIconItemPart1_c::remove() {
    for (int i = 0; i < ITEM_PART_1_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytCommonIconItemPart1_c::execute() {
    if (mAnm[ITEM_PART_1_ANIM_ONOFF].isEnabled() && mAnm[ITEM_PART_1_ANIM_ONOFF].isStop2()) {
        mAnm[ITEM_PART_1_ANIM_ONOFF].setAnimEnable(false);
    }

    if (mAnm[ITEM_PART_1_ANIM_DECIDE].isEnabled() && mAnm[ITEM_PART_1_ANIM_DECIDE].isStop2()) {
        mAnm[ITEM_PART_1_ANIM_DECIDE].setAnimEnable(false);
    }

    for (int i = 0; i < ITEM_PART_1_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    return true;
}

void dLytCommonIconItemPart1_c::reset() {
    mAnm[ITEM_PART_1_ANIM_ICON].setFrame(0.0f);
    mAnm[ITEM_PART_1_ANIM_ICON].setAnimEnable(true);

    mAnm[ITEM_PART_1_ANIM_SIZE].setFrame(1.0f);
    mAnm[ITEM_PART_1_ANIM_SIZE].setAnimEnable(true);

    mAnm[ITEM_PART_1_ANIM_BOCOBURIN].setFrame(0.0f);
    mAnm[ITEM_PART_1_ANIM_BOCOBURIN].setAnimEnable(true);

    mAnm[ITEM_PART_1_ANIM_NUMBERV].setFrame(0.0f);
    mAnm[ITEM_PART_1_ANIM_NUMBERV].setAnimEnable(true);

    mAnm[ITEM_PART_1_ANIM_TEXTCOLOR].setFrame(0.0f);
    mAnm[ITEM_PART_1_ANIM_TEXTCOLOR].setAnimEnable(true);

    mAnm[ITEM_PART_1_ANIM_ONOFF].setBackwardsOnce();
    mAnm[ITEM_PART_1_ANIM_ONOFF].setToEnd2();
    mAnm[ITEM_PART_1_ANIM_ONOFF].setAnimEnable(true);

    mAnm[ITEM_PART_1_ANIM_DECIDE].setForwardOnce();
    mAnm[ITEM_PART_1_ANIM_DECIDE].setToEnd2();
    mAnm[ITEM_PART_1_ANIM_DECIDE].setAnimEnable(true);

    mLyt.calc();

    mAnm[ITEM_PART_1_ANIM_DECIDE].setAnimEnable(false);
    mAnm[ITEM_PART_1_ANIM_ONOFF].setAnimEnable(false);
    mAnm[ITEM_PART_1_ANIM_TEXTCOLOR].setAnimEnable(false);
    mAnm[ITEM_PART_1_ANIM_NUMBERV].setAnimEnable(false);
    mAnm[ITEM_PART_1_ANIM_BOCOBURIN].setAnimEnable(false);
    mAnm[ITEM_PART_1_ANIM_SIZE].setAnimEnable(false);
    mAnm[ITEM_PART_1_ANIM_ICON].setAnimEnable(false);
}

void dLytCommonIconItemPart1_c::setItem(u8 item) {
    realizeUnk();
    realizeNumberV();
    realizeSize();
    realizeBocoburin();
    realizeItem(item);
}

void dLytCommonIconItemPart1_c::realizeUnk() {
    // Probably related to field_0x2B4
}

void dLytCommonIconItemPart1_c::realizeNumberV() {
    if (mHasNumber) {
        mAnm[ITEM_PART_1_ANIM_NUMBERV].setFrame(1.0f);
    } else {
        mAnm[ITEM_PART_1_ANIM_NUMBERV].setFrame(0.0f);
    }
    mAnm[ITEM_PART_1_ANIM_NUMBERV].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_1_ANIM_NUMBERV].setAnimEnable(false);
}

void dLytCommonIconItemPart1_c::realizeSize() {
    if (mSize) {
        mAnm[ITEM_PART_1_ANIM_SIZE].setFrame(0.0f);
    } else {
        mAnm[ITEM_PART_1_ANIM_SIZE].setFrame(1.0f);
    }
    mAnm[ITEM_PART_1_ANIM_SIZE].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_1_ANIM_SIZE].setAnimEnable(false);
}

void dLytCommonIconItemPart1_c::realizeBocoburin() {
    if (mBocoburinLocked) {
        mAnm[ITEM_PART_1_ANIM_BOCOBURIN].setFrame(1.0f);
    } else {
        mAnm[ITEM_PART_1_ANIM_BOCOBURIN].setFrame(0.0f);
    }
    mAnm[ITEM_PART_1_ANIM_BOCOBURIN].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_1_ANIM_BOCOBURIN].setAnimEnable(false);
}

void dLytCommonIconItemPart1_c::realizeItem(u8 item) {
    // Inline?
    if (mAnm[ITEM_PART_1_ANIM_ICON].getEndFrameRaw() - 1.0f < item) {
        mItem = 0;
    } else {
        mItem = item;
    }

    mAnm[ITEM_PART_1_ANIM_ICON].setFrame(mItem);
    mAnm[ITEM_PART_1_ANIM_ICON].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_1_ANIM_ICON].setAnimEnable(false);
    mpBounding->SetVisible(true);
}

void dLytCommonIconItemPart1_c::setNumber(s32 number) {
    std::memset(mNumberBuf, 0, sizeof(mNumberBuf));
    swprintf(mNumberBuf, ARRAY_LENGTH(mNumberBuf), L"%d", number);
    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i]->setTextWithGlobalTextProcessor(mNumberBuf);
    }
}

void dLytCommonIconItemPart1_c::setNumberColor(u8 color) {
    mColor = color;
    switch (mColor) {
        case 0: mAnm[ITEM_PART_1_ANIM_TEXTCOLOR].setFrame(0.0f); break;
        case 1: mAnm[ITEM_PART_1_ANIM_TEXTCOLOR].setFrame(1.0f); break;
        case 2: mAnm[ITEM_PART_1_ANIM_TEXTCOLOR].setFrame(2.0f); break;
    }
    mAnm[ITEM_PART_1_ANIM_TEXTCOLOR].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_1_ANIM_TEXTCOLOR].setAnimEnable(false);
}

void dLytCommonIconItemPart1_c::setOn() {
    if (mAnm[ITEM_PART_1_ANIM_ONOFF].isPlayingBackwardsOnce()) {
        mAnm[ITEM_PART_1_ANIM_ONOFF].setForwardOnce();
        mAnm[ITEM_PART_1_ANIM_ONOFF].setAnimEnable(true);
    }
}

void dLytCommonIconItemPart1_c::setOff() {
    if (mAnm[ITEM_PART_1_ANIM_ONOFF].isPlayingForwardsOnce()) {
        mAnm[ITEM_PART_1_ANIM_ONOFF].setBackwardsOnce();
        mAnm[ITEM_PART_1_ANIM_ONOFF].setAnimEnable(true);
    }
}

void dLytCommonIconItemPart1_c::startDecide() {
    mAnm[ITEM_PART_1_ANIM_DECIDE].setForwardOnce();
    mAnm[ITEM_PART_1_ANIM_DECIDE].setToStart();
    mAnm[ITEM_PART_1_ANIM_DECIDE].setAnimEnable(true);
}

bool dLytCommonIconItemPart1_c::isDoneDeciding() const {
    return mAnm[ITEM_PART_1_ANIM_DECIDE].isStop2();
}

bool dLytCommonIconItemPart1_c::isCursorOver() const {
    d2d::dLytStructD *d = dCsBase_c::GetInstance()->getUnk();
    if (d != nullptr && d->getType() == 'lyt ' && d->field_0x24 == mpBounding) {
        return true;
    }
    return false;
}

void dLytCommonIconItemPart1_c::setVisible(bool visible) {
    if (visible) {
        mpBounding->SetVisible(true);
    } else {
        mpBounding->SetVisible(false);
    }
}

static const d2d::LytBrlanMapping brlanMapPart2[] = {
    { "commonIcon_03_shieldOnOff.brlan", "G_shiledOnOff_00"},
    {  "commonIcon_03_shieldType.brlan",        "G_type_00"},
    {"commonIcon_03_shieldUpDown.brlan",       "G_gauge_00"},
    {        "commonIcon_03_size.brlan",        "G_size_00"},
    {   "commonIcon_03_bocoburin.brlan",   "G_bocoburin_00"},
    {     "commonIcon_03_numberV.brlan",     "G_numberV_00"},
    {       "commonIcon_03_poach.brlan",        "G_icon_00"},
    {   "commonIcon_03_textColor.brlan",   "G_textColor_00"},
    {       "commonIcon_03_onOff.brlan",         "G_btn_00"},
    {      "commonIcon_03_decide.brlan",         "G_btn_00"},
};

static const char *part2TextBoxes[] = {
    "T_comNumberS_01",
    "T_comNumber_01",
};

static const char *part2Bounding = "B_poach_00";

#define ITEM_PART_2_ANIM_SHIELD_ONOFF 0
#define ITEM_PART_2_ANIM_SHIELD_TYPE 1
#define ITEM_PART_2_ANIM_SHIELD_GAUGE 2
#define ITEM_PART_2_ANIM_SIZE 3
#define ITEM_PART_2_ANIM_BOCOBURIN 4
#define ITEM_PART_2_ANIM_NUMBERV 5
#define ITEM_PART_2_ANIM_ICON 6
#define ITEM_PART_2_ANIM_TEXTCOLOR 7
#define ITEM_PART_2_ANIM_ONOFF 8
#define ITEM_PART_2_ANIM_DECIDE 9

#define ITEM_PART_2_NUM_ANIMS 10

bool dLytCommonIconItemPart2_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("commonIcon_03.brlyt", nullptr);
    for (int i = 0; i < ITEM_PART_2_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapPart2[i].mFile, resAcc, mLyt.getLayout(), brlanMapPart2[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
    }

    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(part2TextBoxes[i]);
    }

    mpBounding = mLyt.findBounding(part2Bounding);
    reset();
    return true;
}

bool dLytCommonIconItemPart2_c::remove() {
    for (int i = 0; i < ITEM_PART_2_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytCommonIconItemPart2_c::execute() {
    if (mAnm[ITEM_PART_2_ANIM_ONOFF].isEnabled() && mAnm[ITEM_PART_2_ANIM_ONOFF].isStop2()) {
        mAnm[ITEM_PART_2_ANIM_ONOFF].setAnimEnable(false);
    }

    if (mAnm[ITEM_PART_2_ANIM_DECIDE].isEnabled() && mAnm[ITEM_PART_2_ANIM_DECIDE].isStop2()) {
        mAnm[ITEM_PART_2_ANIM_DECIDE].setAnimEnable(false);
    }

    for (int i = 0; i < ITEM_PART_2_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    return true;
}

void dLytCommonIconItemPart2_c::reset() {
    mAnm[ITEM_PART_2_ANIM_SHIELD_ONOFF].setFrame(0.0f);
    mAnm[ITEM_PART_2_ANIM_SHIELD_ONOFF].setAnimEnable(true);

    mAnm[ITEM_PART_2_ANIM_SHIELD_TYPE].setFrame(0.0f);
    mAnm[ITEM_PART_2_ANIM_SHIELD_TYPE].setAnimEnable(true);

    mAnm[ITEM_PART_2_ANIM_SHIELD_GAUGE].setFrame(0.0f);
    mAnm[ITEM_PART_2_ANIM_SHIELD_GAUGE].setAnimEnable(true);

    mAnm[ITEM_PART_2_ANIM_ICON].setFrame(0.0f);
    mAnm[ITEM_PART_2_ANIM_ICON].setAnimEnable(true);

    mAnm[ITEM_PART_2_ANIM_SIZE].setFrame(1.0f);
    mAnm[ITEM_PART_2_ANIM_SIZE].setAnimEnable(true);

    mAnm[ITEM_PART_2_ANIM_BOCOBURIN].setFrame(0.0f);
    mAnm[ITEM_PART_2_ANIM_BOCOBURIN].setAnimEnable(true);

    mAnm[ITEM_PART_2_ANIM_NUMBERV].setFrame(0.0f);
    mAnm[ITEM_PART_2_ANIM_NUMBERV].setAnimEnable(true);

    mAnm[ITEM_PART_2_ANIM_TEXTCOLOR].setFrame(0.0f);
    mAnm[ITEM_PART_2_ANIM_TEXTCOLOR].setAnimEnable(true);

    mAnm[ITEM_PART_2_ANIM_ONOFF].setBackwardsOnce();
    mAnm[ITEM_PART_2_ANIM_ONOFF].setToEnd2();
    mAnm[ITEM_PART_2_ANIM_ONOFF].setAnimEnable(true);

    mAnm[ITEM_PART_2_ANIM_DECIDE].setForwardOnce();
    mAnm[ITEM_PART_2_ANIM_DECIDE].setToEnd2();
    mAnm[ITEM_PART_2_ANIM_DECIDE].setAnimEnable(true);

    mLyt.calc();

    mAnm[ITEM_PART_2_ANIM_DECIDE].setAnimEnable(false);
    mAnm[ITEM_PART_2_ANIM_ONOFF].setAnimEnable(false);
    mAnm[ITEM_PART_2_ANIM_TEXTCOLOR].setAnimEnable(false);
    mAnm[ITEM_PART_2_ANIM_NUMBERV].setAnimEnable(false);
    mAnm[ITEM_PART_2_ANIM_BOCOBURIN].setAnimEnable(false);
    mAnm[ITEM_PART_2_ANIM_SIZE].setAnimEnable(false);
    mAnm[ITEM_PART_2_ANIM_ICON].setAnimEnable(false);
    mAnm[ITEM_PART_2_ANIM_SHIELD_GAUGE].setAnimEnable(false);
    mAnm[ITEM_PART_2_ANIM_SHIELD_TYPE].setAnimEnable(false);
    mAnm[ITEM_PART_2_ANIM_SHIELD_ONOFF].setAnimEnable(false);
}

void dLytCommonIconItemPart2_c::setShieldOnOff(bool onOff) {
    if (getShieldType() != 4) {
        mOnOff = onOff;
        realizeShieldOnOff(mOnOff);
    }
}

void dLytCommonIconItemPart2_c::setItem(u8 item) {
    if (item == LYT_CMN_Pouch32) {
        return;
    }

    u8 mappedItem = item;
    realizeUnk();
    if (item == LYT_CMN_PouchPotionHealthPlusPlusFull) {
        if (mNumber == 1) {
            mappedItem = LYT_CMN_PouchPotionHealthPlusPlusHalf;
        } else if (mNumber == 2) {
            mappedItem = LYT_CMN_PouchPotionHealthPlusPlusFull;
        }
        mHasNumber = false;
        realizeNumberV();
    } else if (item == LYT_CMN_PouchPotionRepairPlusPlusFull) {
        if (mNumber == 1) {
            mappedItem = LYT_CMN_PouchPotionRepairPlusPlusHalf;
        } else if (mNumber == 2) {
            mappedItem = LYT_CMN_PouchPotionRepairPlusPlusFull;
        }
        mHasNumber = false;
        realizeNumberV();
    } else {
        realizeNumberV();
    }

    realizeSize();
    realizeBocoburin();
    realizeItem(mappedItem);
    u8 shieldType = getShieldType();
    if (shieldType != 4) {
        realizeShieldOnOff(true);
    } else {
        realizeShieldOnOff(false);
    }

    if (shieldType != 4) {
        realizeShieldType(shieldType);
        setDurability(0.0f);
    }
}

void dLytCommonIconItemPart2_c::realizeUnk() {
    // Probably related to field_0x???
}

void dLytCommonIconItemPart2_c::realizeNumberV() {
    if (mHasNumber) {
        mAnm[ITEM_PART_2_ANIM_NUMBERV].setFrame(1.0f);
    } else {
        mAnm[ITEM_PART_2_ANIM_NUMBERV].setFrame(0.0f);
    }
    mAnm[ITEM_PART_2_ANIM_NUMBERV].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_2_ANIM_NUMBERV].setAnimEnable(false);
}

void dLytCommonIconItemPart2_c::realizeSize() {
    if (mSize) {
        mAnm[ITEM_PART_2_ANIM_SIZE].setFrame(0.0f);
    } else {
        mAnm[ITEM_PART_2_ANIM_SIZE].setFrame(1.0f);
    }
    mAnm[ITEM_PART_2_ANIM_SIZE].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_2_ANIM_SIZE].setAnimEnable(false);
}

void dLytCommonIconItemPart2_c::realizeBocoburin() {
    if (mBocoburinLocked) {
        mAnm[ITEM_PART_2_ANIM_BOCOBURIN].setFrame(1.0f);
    } else {
        mAnm[ITEM_PART_2_ANIM_BOCOBURIN].setFrame(0.0f);
    }
    mAnm[ITEM_PART_2_ANIM_BOCOBURIN].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_2_ANIM_BOCOBURIN].setAnimEnable(false);
}

void dLytCommonIconItemPart2_c::realizeItem(u8 item) {
    // Inline?
    if (mAnm[ITEM_PART_2_ANIM_ICON].getEndFrameRaw() - 1.0f < item) {
        mItem = 0;
    } else {
        mItem = item;
    }

    mAnm[ITEM_PART_2_ANIM_ICON].setFrame(mItem);
    mAnm[ITEM_PART_2_ANIM_ICON].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_2_ANIM_ICON].setAnimEnable(false);
    mpBounding->SetVisible(true);
}

void dLytCommonIconItemPart2_c::realizeShieldOnOff(bool onOff) {
    mOnOff = onOff;
    if (onOff) {
        mAnm[ITEM_PART_2_ANIM_SHIELD_ONOFF].setFrame(0.0f);
    } else {
        mAnm[ITEM_PART_2_ANIM_SHIELD_ONOFF].setFrame(1.0f);
    }
    mAnm[ITEM_PART_2_ANIM_SHIELD_ONOFF].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_2_ANIM_SHIELD_ONOFF].setAnimEnable(false);
}

u8 dLytCommonIconItemPart2_c::getShieldType() const {
    switch (mItem) {
        case LYT_CMN_PouchShieldWoodA:
        case LYT_CMN_PouchShieldWoodB:
        case LYT_CMN_PouchShieldWoodC: {
            return 0;
        }
        case LYT_CMN_PouchShieldIronA:
        case LYT_CMN_PouchShieldIronB:
        case LYT_CMN_PouchShieldIronC: {
            return 1;
        }
        case LYT_CMN_PouchShieldHolyA:
        case LYT_CMN_PouchShieldHolyB:
        case LYT_CMN_PouchShieldHolyC: {
            return 2;
        }
        case LYT_CMN_PouchShieldHylian: {
            return 3;
        }
        default: {
            return 4;
        }
    }
}

void dLytCommonIconItemPart2_c::realizeShieldType(u8 type) {
    if (mOnOff) {
        mAnm[ITEM_PART_2_ANIM_SHIELD_TYPE].setFrame(type);
        mAnm[ITEM_PART_2_ANIM_SHIELD_TYPE].setAnimEnable(true);
        mLyt.calc();
        mAnm[ITEM_PART_2_ANIM_SHIELD_TYPE].setAnimEnable(false);
    }
}

void dLytCommonIconItemPart2_c::setNumber(s32 number) {
    mNumber = number;
    std::memset(mNumberBuf, 0, sizeof(mNumberBuf));
    swprintf(mNumberBuf, ARRAY_LENGTH(mNumberBuf), L"%d", number);
    for (int i = 0; i < 2; i++) {
        mpTextBoxes[i]->setTextWithGlobalTextProcessor(mNumberBuf);
    }
}

void dLytCommonIconItemPart2_c::setNumberColor(u8 color) {
    mColor = color;
    switch (mColor) {
        case 0: mAnm[ITEM_PART_2_ANIM_TEXTCOLOR].setFrame(0.0f); break;
        case 1: mAnm[ITEM_PART_2_ANIM_TEXTCOLOR].setFrame(1.0f); break;
        case 2: mAnm[ITEM_PART_2_ANIM_TEXTCOLOR].setFrame(2.0f); break;
    }
    mAnm[ITEM_PART_2_ANIM_TEXTCOLOR].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_2_ANIM_TEXTCOLOR].setAnimEnable(false);
}

void dLytCommonIconItemPart2_c::setOn() {
    if (mAnm[ITEM_PART_2_ANIM_ONOFF].isPlayingBackwardsOnce()) {
        mAnm[ITEM_PART_2_ANIM_ONOFF].setForwardOnce();
        mAnm[ITEM_PART_2_ANIM_ONOFF].setAnimEnable(true);
    }
}

void dLytCommonIconItemPart2_c::setOff() {
    if (mAnm[ITEM_PART_2_ANIM_ONOFF].isPlayingForwardsOnce()) {
        mAnm[ITEM_PART_2_ANIM_ONOFF].setBackwardsOnce();
        mAnm[ITEM_PART_2_ANIM_ONOFF].setAnimEnable(true);
    }
}

void dLytCommonIconItemPart2_c::startDecide() {
    mAnm[ITEM_PART_2_ANIM_DECIDE].setForwardOnce();
    mAnm[ITEM_PART_2_ANIM_DECIDE].setToStart();
    mAnm[ITEM_PART_2_ANIM_DECIDE].setAnimEnable(true);
}

bool dLytCommonIconItemPart2_c::isDoneDeciding() const {
    return mAnm[ITEM_PART_2_ANIM_DECIDE].isStop2();
}

bool dLytCommonIconItemPart2_c::isCursorOver() const {
    d2d::dLytStructD *d = dCsBase_c::GetInstance()->getUnk();
    if (d != nullptr && d->getType() == 'lyt ' && d->field_0x24 == mpBounding) {
        return true;
    }
    return false;
}

void dLytCommonIconItemPart2_c::setVisible(bool visible) {
    if (visible) {
        mpBounding->SetVisible(true);
    } else {
        mpBounding->SetVisible(false);
    }
}

void dLytCommonIconItemPart2_c::setDurability(f32 durability) {
    if (mOnOff) {
        mDurability = durability;
        mAnm[ITEM_PART_2_ANIM_SHIELD_GAUGE].setFrameRatio(durability);
        mAnm[ITEM_PART_2_ANIM_SHIELD_GAUGE].setAnimEnable(true);
        mLyt.calc();
        mAnm[ITEM_PART_2_ANIM_SHIELD_GAUGE].setAnimEnable(false);
    }
}

static const d2d::LytBrlanMapping brlanMapPart3[] = {
    {     "commonIcon_04_size.brlan",      "G_size_00"},
    {"commonIcon_04_bocoburin.brlan", "G_bocoburin_00"},
    {  "commonIcon_04_dauzing.brlan",      "G_icon_00"},
};

static const char *part3Bounding = "B_dauz_00";

#define ITEM_PART_3_ANIM_SIZE 0
#define ITEM_PART_3_ANIM_BOCOBURIN 1
#define ITEM_PART_3_ANIM_ICON 2

#define ITEM_PART_3_NUM_ANIMS 3

bool dLytCommonIconItemPart3_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("commonIcon_04.brlyt", nullptr);
    for (int i = 0; i < ITEM_PART_3_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapPart3[i].mFile, resAcc, mLyt.getLayout(), brlanMapPart3[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
    }

    mpBounding = mLyt.findBounding(part3Bounding);
    reset();
    return true;
}

bool dLytCommonIconItemPart3_c::remove() {
    for (int i = 0; i < ITEM_PART_3_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }
    return true;
}

bool dLytCommonIconItemPart3_c::execute() {
    for (int i = 0; i < ITEM_PART_3_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }
    return true;
}

void dLytCommonIconItemPart3_c::reset() {
    mAnm[ITEM_PART_3_ANIM_ICON].setFrame(0.0f);
    mAnm[ITEM_PART_3_ANIM_ICON].setAnimEnable(true);

    mAnm[ITEM_PART_3_ANIM_SIZE].setFrame(1.0f);
    mAnm[ITEM_PART_3_ANIM_SIZE].setAnimEnable(true);

    mAnm[ITEM_PART_3_ANIM_BOCOBURIN].setFrame(0.0f);
    mAnm[ITEM_PART_3_ANIM_BOCOBURIN].setAnimEnable(true);

    mLyt.calc();

    mAnm[ITEM_PART_3_ANIM_BOCOBURIN].setAnimEnable(false);
    mAnm[ITEM_PART_3_ANIM_SIZE].setAnimEnable(false);
    mAnm[ITEM_PART_3_ANIM_ICON].setAnimEnable(false);
}

void dLytCommonIconItemPart3_c::setItem(u8 item) {
    realizeSize();
    realizeBocoburin();
    realizeItem(item);
}

void dLytCommonIconItemPart3_c::realizeSize() {
    if (mSize) {
        mAnm[ITEM_PART_3_ANIM_SIZE].setFrame(0.0f);
    } else {
        mAnm[ITEM_PART_3_ANIM_SIZE].setFrame(1.0f);
    }
    mAnm[ITEM_PART_3_ANIM_SIZE].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_3_ANIM_SIZE].setAnimEnable(false);
}

void dLytCommonIconItemPart3_c::realizeBocoburin() {
    if (mBocoburinLocked) {
        mAnm[ITEM_PART_3_ANIM_BOCOBURIN].setFrame(1.0f);
    } else {
        mAnm[ITEM_PART_3_ANIM_BOCOBURIN].setFrame(0.0f);
    }
    mAnm[ITEM_PART_3_ANIM_BOCOBURIN].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_3_ANIM_BOCOBURIN].setAnimEnable(false);
}

void dLytCommonIconItemPart3_c::realizeItem(u8 item) {
    if (mAnm[ITEM_PART_3_ANIM_ICON].getEndFrameRaw() - 1.0f < item) {
        mItem = 0;
    } else {
        mItem = item;
    }
    // @bug ignoring the check above
    mItem = item;

    mAnm[ITEM_PART_3_ANIM_ICON].setFrame(mItem);
    mAnm[ITEM_PART_3_ANIM_ICON].setAnimEnable(true);
    mLyt.calc();
    mAnm[ITEM_PART_3_ANIM_ICON].setAnimEnable(false);
    mpBounding->SetVisible(true);
}

bool dLytCommonIconItemPart3_c::isCursorOver() const {
    d2d::dLytStructD *d = dCsBase_c::GetInstance()->getUnk();
    if (d != nullptr && d->getType() == 'lyt ' && d->field_0x24 == mpBounding) {
        return true;
    }
    return false;
}

void dLytCommonIconItemPart3_c::setVisible(bool visible) {
    if (visible) {
        mpBounding->SetVisible(true);
    } else {
        mpBounding->SetVisible(false);
    }
}

// Incredible use of the state manager here
void dLytCommonIconItem_c::initializeState_None() {}
void dLytCommonIconItem_c::executeState_None() {}
void dLytCommonIconItem_c::finalizeState_None() {}

void dLytCommonIconItem_c::initializeState_In() {}
void dLytCommonIconItem_c::executeState_In() {}
void dLytCommonIconItem_c::finalizeState_In() {}

void dLytCommonIconItem_c::initializeState_Wait() {}
void dLytCommonIconItem_c::executeState_Wait() {}
void dLytCommonIconItem_c::finalizeState_Wait() {}

void dLytCommonIconItem_c::initializeState_Out() {}
void dLytCommonIconItem_c::executeState_Out() {}
void dLytCommonIconItem_c::finalizeState_Out() {}

bool dLytCommonIconItem_c::build(d2d::ResAccIf_c *resAcc) {
    build(resAcc, 1);
    return true;
}

bool dLytCommonIconItem_c::build(d2d::ResAccIf_c *resAcc, u8 variant) {
    mPart = variant;
    switch (mPart) {
        case 0: mPart1.build(resAcc); break;
        case 1: mPart2.build(resAcc); break;
        case 2: mPart3.build(resAcc); break;
    }
    return true;
}

bool dLytCommonIconItem_c::remove() {
    switch (mPart) {
        case 0: mPart1.remove(); break;
        case 1: mPart2.remove(); break;
        case 2: mPart3.remove(); break;
    }
    return true;
}

bool dLytCommonIconItem_c::execute() {
    switch (mPart) {
        case 0: mPart1.execute(); break;
        case 1: mPart2.execute(); break;
        case 2: mPart3.execute(); break;
    }
    return true;
}

void dLytCommonIconItem_c::reset() {
    switch (mPart) {
        case 0: mPart1.reset(); break;
        case 1: mPart2.reset(); break;
        case 2: mPart3.reset(); break;
    }
}

void dLytCommonIconItem_c::setUnk(bool unk) {
    switch (mPart) {
        case 0: mPart1.setUnk(unk); break;
        case 1: mPart2.setUnk(unk); break;
        case 2: break;
    }
}

void dLytCommonIconItem_c::setHasNumber(bool hasNumber) {
    switch (mPart) {
        case 0: mPart1.setHasNumber(hasNumber); break;
        case 1: mPart2.setHasNumber(hasNumber); break;
        case 2: break;
    }
}

void dLytCommonIconItem_c::setSize(bool size) {
    switch (mPart) {
        case 0: mPart1.setSize(size); break;
        case 1: mPart2.setSize(size); break;
        case 2: mPart3.setSize(size); break;
    }
}

void dLytCommonIconItem_c::setBocoburinLocked(bool locked) {
    switch (mPart) {
        case 0: mPart1.setBocoburinLocked(locked); break;
        case 1: mPart2.setBocoburinLocked(locked); break;
        case 2: mPart3.setBocoburinLocked(locked); break;
    }
}

void dLytCommonIconItem_c::setItem(u8 item) {
    switch (mPart) {
        case 0: mPart1.setItem(item); break;
        case 1: mPart2.setItem(item); break;
        case 2: mPart3.setItem(item); break;
    }
}

void dLytCommonIconItem_c::setShieldDurability(f32 durability) {
    switch (mPart) {
        case 0: break;
        case 1: mPart2.setDurability(durability); break;
        case 2: break;
    }
}

void dLytCommonIconItem_c::setShieldOnOff(bool onOff) {
    switch (mPart) {
        case 0: break;
        case 1: mPart2.setShieldOnOff(onOff); break;
        case 2: break;
    }
}

void dLytCommonIconItem_c::setNumber(s32 number) {
    switch (mPart) {
        case 0: mPart1.setNumber(number); break;
        case 1: mPart2.setNumber(number); break;
        case 2: break;
    }
}

void dLytCommonIconItem_c::setNumberColor(u8 color) {
    switch (mPart) {
        case 0: mPart1.setNumberColor(color); break;
        case 1: mPart2.setNumberColor(color); break;
        case 2: break;
    }
}

void dLytCommonIconItem_c::setOn() {
    switch (mPart) {
        case 0: mPart1.setOn(); break;
        case 1: mPart2.setOn(); break;
        case 2: break;
    }
}

void dLytCommonIconItem_c::setOff() {
    switch (mPart) {
        case 0: mPart1.setOff(); break;
        case 1: mPart2.setOff(); break;
        case 2: break;
    }
}

void dLytCommonIconItem_c::startDecide() {
    switch (mPart) {
        case 0: mPart1.startDecide(); break;
        case 1: mPart2.startDecide(); break;
        case 2: break;
    }
}

bool dLytCommonIconItem_c::isDoneDeciding() const {
    switch (mPart) {
        case 0:
            if (mPart1.isDoneDeciding()) {
                return true;
            }
            break;
        case 1:
            if (mPart2.isDoneDeciding()) {
                return true;
            }
            break;
    }
    return false;
}

bool dLytCommonIconItem_c::isCursorOver() const {
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
        case 2:
            if (mPart3.isCursorOver()) {
                return true;
            }
            break;
    }
    return false;
}

void dLytCommonIconItem_c::setVisible(bool visible) {
    switch (mPart) {
        case 0: mPart1.setVisible(visible); break;
        case 1: mPart2.setVisible(visible); break;
        case 2: mPart3.setVisible(visible); break;
    }
}
