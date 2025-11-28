#include "d/lyt/d_lyt_mini_game.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/d_lang.h"
#include "d/lyt/d2d.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "m/m_vec.h"
#include "toBeSorted/d_d3d.h"

static const d2d::LytBrlanMapping brlanMapPumpkin[] = {
    {    "miniGamePumpkin_00_getPoint.brlan",    "G_scoreAll_00"},
    { "miniGamePumpkin_00_doubleScore.brlan", "G_doubleScore_00"},
    {"miniGamePumpkin_00_scorePattern.brlan",     "G_pattern_00"},
};

#define PUMPKIN_ANIM_GET_POINT 0
#define PUMPKIN_ANIM_DOUBLE_SCORE 1
#define PUMPKIN_ANIM_PATTERN 2

#define PUMPKIN_NUM_ANIMS 3

bool dLytMiniGamePumpkinParts_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("miniGamePumpkin_00.brlyt", nullptr);

    for (int i = 0; i < PUMPKIN_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapPumpkin[i].mFile, resAcc, mLyt.getLayout(), brlanMapPumpkin[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
    }

    mPumpkinWorldPos.set(0.0f, 0.0f, 0.0f);
    mPumpkinScreenPos.set(0.0f, 0.0f, 0.0f);
    field_0x170.set(0.0f, 0.0f);

    return true;
}

bool dLytMiniGamePumpkinParts_c::remove() {
    for (int i = 0; i < PUMPKIN_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    return true;
}

bool dLytMiniGamePumpkinParts_c::execute() {
    if (mAnm[PUMPKIN_ANIM_GET_POINT].isEnabled() && mAnm[PUMPKIN_ANIM_GET_POINT].isStop2()) {
        mAnm[PUMPKIN_ANIM_GET_POINT].setAnimEnable(false);
        field_0x154 = false;
    }

    if (mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].isEnabled() && mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].isStop2()) {
        mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].setAnimEnable(false);
        mHasPlayedDoubleScoreSound = false;
    }

    if (mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].isEnabled() && 20.0f <= mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].getFrame() &&
        !mHasPlayedDoubleScoreSound) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MG_PUMP_SHOT_DOUBLE);
        mHasPlayedDoubleScoreSound = true;
    }

    for (int i = 0; i < PUMPKIN_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    if (field_0x154) {
        d3d::worldToScreen(mPumpkinScreenPos, mPumpkinWorldPos);
        mVec3_c tmp = mPumpkinScreenPos;
        tmp.z = 0.0f;
        field_0x170.set(tmp.x, tmp.y);
        mLyt.getLayout()->GetRootPane()->SetTranslate(tmp);
    }

    mLyt.calc();

    return true;
}

bool dLytMiniGamePumpkinParts_c::draw() {
    mLyt.addToDrawList();
    return true;
}

void dLytMiniGamePumpkinParts_c::init() {
    resetPattern();
    resetDoubleScore();
    resetGetPoint();
    mAnm[PUMPKIN_ANIM_GET_POINT].setToStart();
    mAnm[PUMPKIN_ANIM_GET_POINT].setAnimEnable(true);
    mLyt.calc();
    mAnm[PUMPKIN_ANIM_GET_POINT].setAnimEnable(false);
}

void dLytMiniGamePumpkinParts_c::setPumpkinPos(const mVec3_c &pos) {
    mPumpkinWorldPos = pos;
    d3d::worldToScreen(mPumpkinScreenPos, mPumpkinWorldPos);
    mVec3_c tmp = mPumpkinScreenPos;
    tmp.z = 0.0f;
    field_0x170.set(tmp.x, tmp.y);
    mLyt.getLayout()->GetRootPane()->SetTranslate(tmp);
    mLyt.calc();
}

void dLytMiniGamePumpkinParts_c::getPoint(u8 combo, bool doubleScore) {
    startGetPoint();
    setPattern(combo);
    if (doubleScore) {
        startDoubleScore();
    } else {
        stopDoubleScore();
    }
    field_0x154 = true;
}

bool dLytMiniGamePumpkinParts_c::isActive() const {
    return mAnm[PUMPKIN_ANIM_GET_POINT].isEnabled() || mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].isEnabled();
}

void dLytMiniGamePumpkinParts_c::startGetPoint() {
    mAnm[PUMPKIN_ANIM_GET_POINT].setToStart();
    mAnm[PUMPKIN_ANIM_GET_POINT].setAnimEnable(true);
}

void dLytMiniGamePumpkinParts_c::startDoubleScore() {
    mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].setToStart();
    mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].setAnimEnable(true);
}

void dLytMiniGamePumpkinParts_c::stopDoubleScore() {
    mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].setAnimEnable(false);
}

void dLytMiniGamePumpkinParts_c::resetGetPoint() {
    mAnm[PUMPKIN_ANIM_GET_POINT].setForwardOnce();
    mAnm[PUMPKIN_ANIM_GET_POINT].setToStart();
    mAnm[PUMPKIN_ANIM_GET_POINT].setAnimEnable(false);
}

void dLytMiniGamePumpkinParts_c::resetDoubleScore() {
    mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].setForwardOnce();
    mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].setToStart();
    mAnm[PUMPKIN_ANIM_DOUBLE_SCORE].setAnimEnable(false);
}

void dLytMiniGamePumpkinParts_c::resetPattern() {
    mAnm[PUMPKIN_ANIM_PATTERN].setForwardOnce();
    mAnm[PUMPKIN_ANIM_PATTERN].setFrame(0.0f);
    mAnm[PUMPKIN_ANIM_PATTERN].setRate(0.0f);
    mAnm[PUMPKIN_ANIM_PATTERN].setAnimEnable(false);
}

void dLytMiniGamePumpkinParts_c::setPattern(s32 pat) {
    u32 frame = 0;
    switch (pat) {
        case 0:
            frame = 0;
            dSndSmallEffectMgr_c::GetInstance()->playMinigameScoreUpSound(10);
            break;
        case 1:
            frame = 1;
            dSndSmallEffectMgr_c::GetInstance()->playMinigameScoreUpSound(20);
            break;
        case 2:
            frame = 2;
            dSndSmallEffectMgr_c::GetInstance()->playMinigameScoreUpSound(30);
            break;
        case 3:
            frame = 3;
            dSndSmallEffectMgr_c::GetInstance()->playMinigameScoreUpSound(40);
            break;
        case 4:
            frame = 4;
            dSndSmallEffectMgr_c::GetInstance()->playMinigameScoreUpSound(50);
            break;
    }
    mAnm[PUMPKIN_ANIM_PATTERN].setFrame(frame);
    mAnm[PUMPKIN_ANIM_PATTERN].setAnimEnable(true);
    mLyt.calc();
    mAnm[PUMPKIN_ANIM_PATTERN].setAnimEnable(false);
}

bool dLytMiniGamePumpkin_c::build(d2d::ResAccIf_c *resAcc) {
    for (int i = 0; i < (int)ARRAY_LENGTH(mParts); i++) {
        mParts[i].build(resAcc);
    }
    sInstance = this;
    return true;
}

bool dLytMiniGamePumpkin_c::remove() {
    if (sInstance == nullptr) {
        return true;
    }
    for (int i = 0; i < (int)ARRAY_LENGTH(mParts); i++) {
        mParts[i].remove();
    }
    sInstance = nullptr;
    return true;
}

bool dLytMiniGamePumpkin_c::execute() {
    if (sInstance == nullptr) {
        return true;
    }
    for (int i = 0; i < (int)ARRAY_LENGTH(mParts); i++) {
        mParts[i].execute();
    }
    return true;
}

bool dLytMiniGamePumpkin_c::draw() {
    if (sInstance == nullptr) {
        return true;
    }
    for (int i = 0; i < (int)ARRAY_LENGTH(mParts); i++) {
        mParts[i].draw();
    }
    return true;
}

void dLytMiniGamePumpkin_c::init() {
    if (sInstance == nullptr) {
        return;
    }
    for (int i = 0; i < (int)ARRAY_LENGTH(mParts); i++) {
        mParts[i].init();
    }
    return;
}

void dLytMiniGamePumpkin_c::getPoint(const mVec3_c &pumpkinPos, s32 unk, bool doubleScore) {
    if (sInstance == nullptr) {
        return;
    }
    calcCombo(unk);
    for (int i = 0; i < (int)ARRAY_LENGTH(mParts); i++) {
        if (!mParts[i].isActive()) {
            mParts[i].setPumpkinPos(pumpkinPos);
            mParts[i].getPoint(mCombo, doubleScore);
            return;
        }
    }
}

void dLytMiniGamePumpkin_c::calcCombo(s32 unk) {
    if (unk < field_0x478) {
        mCombo = 0;
    } else if (field_0x478 < unk) {
        mCombo++;
    }
    if (mCombo >= 5) {
        mCombo = 4;
    }
    field_0x478 = unk;
}

static const d2d::LytBrlanMapping brlanMapBugs[] = {
    {  "miniGameBugs_00_alphaIn.brlan",   "G_inOut_00"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_00"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_01"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_02"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_03"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_04"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_05"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_06"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_07"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_08"},
    {       "miniGameBugs_00_in.brlan", "G_bugIcon_09"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_00"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_01"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_02"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_03"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_04"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_05"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_06"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_07"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_08"},
    {   "miniGameBugs_00_iconOn.brlan", "G_bugIcon_09"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_00"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_01"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_02"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_03"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_04"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_05"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_06"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_07"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_08"},
    {     "miniGameBugs_00_bugs.brlan", "G_texAnim_09"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_00"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_01"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_02"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_03"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_04"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_05"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_06"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_07"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_08"},
    {"miniGameBugs_00_iconFlash.brlan", "G_bugIcon_09"},
    { "miniGameBugs_00_alphaOut.brlan",   "G_inOut_00"},
};

#define BUGS_ANIM_IN 0
#define BUGS_ANIM_ICON_IN_OFFSET 1
#define BUGS_ANIM_ICON_ON_OFFSET 11
#define BUGS_ANIM_ICON_TEX_ANIM_OFFSET 21
#define BUGS_ANIM_ICON_FLASH_OFFSET 31
#define BUGS_ANIM_OUT 41

#define BUGS_NUM_ANIMS 42
#define BUGS_NUM_BUGS 10

bool dLytMiniGameBugs_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("miniGameBugs_00.brlyt", nullptr);

    for (int i = 0; i < BUGS_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapBugs[i].mFile, resAcc, mLyt.getLayout(), brlanMapBugs[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
    }

    sInstance = this;

    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        mBugCollected[i] = 0;
    }

    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        mBugItemIds[i] = ITEM_DEKU_HORNET;
    }

    return true;
}

bool dLytMiniGameBugs_c::remove() {
    if (sInstance == nullptr) {
        return true;
    }
    if (isBug0Flashing()) {
        stopBugIconsFlash();
    }
    for (int i = 0; i < BUGS_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    sInstance = nullptr;
    return true;
}

bool dLytMiniGameBugs_c::execute() {
    if (sInstance == nullptr) {
        return true;
    }

    if (areAllBugIconsOn()) {
        startBugsFlash();
    }

    if (isIconInEndReached()) {
        stopBugIconsIn();
        mBugIconsInEnded = true;
    }

    if (isInEndReached()) {
        stopIn();
        for (int i = 0; i < BUGS_NUM_BUGS; i++) {
            if (mBugCollected[i] == true) {
                collectBug(i);
            }
        }
    }

    if (isOutEndReached()) {
        stopOut();
    }

    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        if (isSlotOnEndReached(i)) {
            stopBugIconOn(i);
        }
    }

    for (int i = 0; i < BUGS_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    mLyt.calc();

    return true;
}

bool dLytMiniGameBugs_c::draw() {
    if (sInstance == nullptr) {
        return true;
    }
    mLyt.addToDrawList();
    return true;
}

void dLytMiniGameBugs_c::init() {
    if (sInstance == nullptr) {
        return;
    }

    switch (mBugsNumLevel) {
        case 0: mBugsNum = 5; break;
        case 1: mBugsNum = 10; break;
    }

    resetBugIconsTex();
    resetBugIconsOn();
    resetBugIconsFlash();
    resetIn();
    resetOut();
    resetBugIconsIn();
}

void dLytMiniGameBugs_c::startAllIconsIn() {
    if (sInstance == nullptr) {
        return;
    }

    if (mInTriggered != true) {
        for (int i = 0; i < mBugsNum; i++) {
            mAnm[i + BUGS_ANIM_ICON_IN_OFFSET].setToStart();
            mAnm[i + BUGS_ANIM_ICON_IN_OFFSET].setAnimEnable(true);
        }
        mInTriggered = true;
        field_0xB4B = true;
    }
}

void dLytMiniGameBugs_c::startOut() {
    if (sInstance == nullptr) {
        return;
    }

    if (mBugIconsInEnded && mInTriggered && field_0xB4B) {
        mAnm[BUGS_ANIM_OUT].setToStart();
        mAnm[BUGS_ANIM_OUT].setAnimEnable(true);
        mInTriggered = false;
        field_0xB4B = false;
    }
}

void dLytMiniGameBugs_c::startIn() {
    if (sInstance == nullptr) {
        return;
    }

    if (mBugIconsInEnded && mInTriggered != true && field_0xB4B) {
        mAnm[BUGS_ANIM_IN].setToStart();
        mAnm[BUGS_ANIM_IN].setAnimEnable(true);
        mInTriggered = true;
    }
}

void dLytMiniGameBugs_c::startOutForced() {
    if (sInstance == nullptr) {
        return;
    }

    if (mBugIconsInEnded && mInTriggered) {
        mAnm[BUGS_ANIM_OUT].setToStart();
        mAnm[BUGS_ANIM_OUT].setAnimEnable(true);
        mInTriggered = false;
    }
}

void dLytMiniGameBugs_c::collectBug(s32 slot) {
    if (sInstance == nullptr) {
        return;
    }

    if (mBugCollected[slot] == false) {
        mBugCollected[slot] = true;
        if (dAcItem_c::checkTreasureTempCollect(mBugItemIds[slot])) {
            field_0xB4C = false;
        }
        if (field_0xB4C) {
            return;
        }
    }

    if (isSlotIconOnEndReached(slot) || isSlotIconOnEnabled(slot)) {
        return;
    }

    mAnm[slot + BUGS_ANIM_ICON_ON_OFFSET].setToStart();
    mAnm[slot + BUGS_ANIM_ICON_ON_OFFSET].setAnimEnable(true);

    u8 bugsCount = 0;
    for (int i = 0; i < mBugsNum; i++) {
        if (mBugCollected[i]) {
            bugsCount++;
        }
    }

    if (bugsCount != mBugsNum) {
        dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_MG_MUSHITORI_GET);
    }
    field_0xB4C = true;
}

void dLytMiniGameBugs_c::startBugsFlash() {
    if (sInstance == nullptr) {
        return;
    }

    if (!mBugIconsFlashStarted) {
        mBugIconsFlashStarted = true;
        return;
    }

    if (!isBug0Flashing()) {
        for (int i = 0; i < mBugsNum; i++) {
            s32 idx = i + BUGS_ANIM_ICON_FLASH_OFFSET;
            mAnm[idx].setToStart();
            mAnm[idx].setAnimEnable(true);
        }
    }
}

// corresponds to BugIndex_e
static const s32 sBugItemIds[] = {
    ITEM_DEKU_HORNET,           ITEM_BLESSED_BUTTERFLY, ITEM_GERUDO_DRAGONFLY, ITEM_STARRY_FIREFLY,
    ITEM_WOODLAND_RHINO_BEETLE, ITEM_VOLCANIC_LADYBUG,  ITEM_SAND_CICADA,      ITEM_SKY_STAG_BEETLE,
    ITEM_FARON_GRASSHOPPER,     ITEM_SKYLOFT_MANTIS,    ITEM_LANAYRU_ANT,      ITEM_ELDIN_ROLLER,
};

void dLytMiniGameBugs_c::setBug(s32 slot, s32 bugIndex) {
    if (sInstance == nullptr) {
        return;
    }

    f32 frame = static_cast<float>(bugIndex);
    mAnm[slot + BUGS_ANIM_ICON_TEX_ANIM_OFFSET].setFrame(frame);
    mAnm[slot + BUGS_ANIM_ICON_TEX_ANIM_OFFSET].setAnimEnable(true);
    mLyt.calc();
    mAnm[slot + BUGS_ANIM_ICON_TEX_ANIM_OFFSET].setAnimEnable(false);
    mBugItemIds[slot] = sBugItemIds[bugIndex];
}

bool dLytMiniGameBugs_c::isSlotIconOnEndReached(s32 slot) const {
    if (sInstance == nullptr) {
        return false;
    }
    s32 idx = slot + BUGS_ANIM_ICON_ON_OFFSET;
    return mAnm[idx].isEndReached();
}

bool dLytMiniGameBugs_c::isSlotIconOnEnabled(s32 slot) const {
    if (sInstance == nullptr) {
        return false;
    }
    s32 idx = slot + BUGS_ANIM_ICON_ON_OFFSET;
    return mAnm[idx].isEnabled_();
}

bool dLytMiniGameBugs_c::isSlotBugCollected(s32 slot) {
    if (sInstance == nullptr) {
        return false;
    }
    return mBugCollected[slot] == true;
}

void dLytMiniGameBugs_c::resetBugIconsIn() {
    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_IN_OFFSET;
        mAnm[idx].setForwardOnce();
        mAnm[idx].setToStart();
        mAnm[idx].setAnimEnable(true);
    }

    mLyt.calc();

    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_IN_OFFSET;
        mAnm[idx].setAnimEnable(false);
    }
}

void dLytMiniGameBugs_c::resetIn() {
    mAnm[BUGS_ANIM_IN].setForwardOnce();
    mAnm[BUGS_ANIM_IN].setToStart();
    mAnm[BUGS_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[BUGS_ANIM_IN].setAnimEnable(false);
}

void dLytMiniGameBugs_c::resetOut() {
    mAnm[BUGS_ANIM_OUT].setForwardOnce();
    mAnm[BUGS_ANIM_OUT].setToStart();
    mAnm[BUGS_ANIM_OUT].setAnimEnable(true);
    mLyt.calc();
    mAnm[BUGS_ANIM_OUT].setAnimEnable(false);
}

void dLytMiniGameBugs_c::resetBugIconsOn() {
    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_ON_OFFSET;
        mAnm[idx].setForwardOnce();
        mAnm[idx].setToStart();
        mAnm[idx].setAnimEnable(true);
    }

    mLyt.calc();

    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_ON_OFFSET;
        mAnm[idx].setAnimEnable(false);
    }
}

void dLytMiniGameBugs_c::resetBugIconsTex() {
    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_TEX_ANIM_OFFSET;
        mAnm[idx].setForwardOnce();
        mAnm[idx].setToStart();
        mAnm[idx].setAnimEnable(true);
    }

    mLyt.calc();

    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_TEX_ANIM_OFFSET;
        mAnm[idx].setAnimEnable(false);
    }
}

void dLytMiniGameBugs_c::resetBugIconsFlash() {
    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_FLASH_OFFSET;
        mAnm[idx].setForwardLoop();
        mAnm[idx].setToStart();
        mAnm[idx].setAnimEnable(true);
    }

    mLyt.calc();

    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_FLASH_OFFSET;
        mAnm[idx].setAnimEnable(false);
    }
}

void dLytMiniGameBugs_c::stopBugIconsIn() {
    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_IN_OFFSET;
        mAnm[idx].setAnimEnable(false);
    }
}

void dLytMiniGameBugs_c::stopIn() {
    mAnm[BUGS_ANIM_IN].setAnimEnable(false);
}

void dLytMiniGameBugs_c::stopOut() {
    mAnm[BUGS_ANIM_OUT].setAnimEnable(false);
}

void dLytMiniGameBugs_c::stopBugIconOn(s32 slot) {
    mAnm[slot + BUGS_ANIM_ICON_ON_OFFSET].setAnimEnable(false);
}

void dLytMiniGameBugs_c::stopBugIconsFlash() {
    for (int i = 0; i < BUGS_NUM_BUGS; i++) {
        s32 idx = i + BUGS_ANIM_ICON_FLASH_OFFSET;
        mAnm[idx].setAnimEnable(false);
    }
}

bool dLytMiniGameBugs_c::isIconInEndReached() const {
    s32 idx = 0 + BUGS_ANIM_ICON_IN_OFFSET;
    if (mAnm[idx].isEnabled() && mAnm[idx].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameBugs_c::isInEndReached() const {
    s32 idx = BUGS_ANIM_IN;
    if (mAnm[idx].isEnabled() && mAnm[idx].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameBugs_c::isOutEndReached() const {
    s32 idx = BUGS_ANIM_OUT;
    if (mAnm[idx].isEnabled() && mAnm[idx].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameBugs_c::isSlotOnEndReached(s32 slot) const {
    s32 idx = slot + BUGS_ANIM_ICON_ON_OFFSET;
    if (mAnm[idx].isEnabled() && mAnm[idx].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameBugs_c::isBug0Flashing() const {
    s32 idx = 0 + BUGS_ANIM_ICON_FLASH_OFFSET;
    return mAnm[idx].isEnabled_();
}

bool dLytMiniGameBugs_c::areAllBugIconsOn() const {
    bool ret = true;
    for (int i = 0; i < mBugsNum; i++) {
        if (!isSlotIconOnEndReached(i)) {
            ret = false;
            break;
        }
    }
    return ret;
}

static const d2d::LytBrlanMapping brlanMapTime[] = {
    {         "miniGameTime_00_in.brlan",      "G_inOut_00"},
    {    "miniGameTime_00_alphaIn.brlan", "G_alphaInOut_00"},
    {        "miniGameTime_00_out.brlan",      "G_inOut_00"},
    { "miniGameTime_00_signChange.brlan", "G_signChange_00"},
    {       "miniGameTime_00_0to9.brlan",       "G_0to9_00"},
    {       "miniGameTime_00_0to9.brlan",       "G_0to9_01"},
    {       "miniGameTime_00_0to9.brlan",       "G_0to9_02"},
    {       "miniGameTime_00_0to9.brlan",       "G_0to9_03"},
    {       "miniGameTime_00_0to9.brlan",       "G_0to9_04"},
    {       "miniGameTime_00_0to9.brlan",       "G_0to9_05"},
    {       "miniGameTime_00_0to9.brlan",      "G_0to9B_00"},
    {       "miniGameTime_00_0to9.brlan",      "G_0to9B_01"},
    {       "miniGameTime_00_0to9.brlan",      "G_0to9B_02"},
    {       "miniGameTime_00_0to9.brlan",      "G_0to9B_03"},
    {       "miniGameTime_00_0to9.brlan",      "G_0to9B_04"},
    {       "miniGameTime_00_0to9.brlan",      "G_0to9B_05"},
    {"miniGameTime_00_setPosition.brlan",    "G_setPosi_00"},
    {"miniGameTime_00_setPosition.brlan",   "G_setPosiB_00"},
    {"miniGameTime_00_setPosition.brlan",      "G_0to9B_05"},
    {"miniGameTime_00_bScoreOnOff.brlan",  "G_bestScore_00"},
    { "miniGameTime_00_finishTime.brlan",     "G_finish_00"},
    { "miniGameTime_00_finishTime.brlan",     "G_finish_01"},
    { "miniGameTime_00_finishTime.brlan",     "G_finish_02"},
    { "miniGameTime_00_finishTime.brlan",     "G_finish_03"},
    { "miniGameTime_00_finishTime.brlan",     "G_finish_04"},
    { "miniGameTime_00_finishTime.brlan",     "G_finish_05"},
    { "miniGameTime_00_finishTime.brlan",    "G_finishW_00"},
    {   "miniGameTime_00_loopTime.brlan",   "G_loopTime_00"},
    {   "miniGameTime_00_alphaOut.brlan", "G_alphaInOut_00"},
};

#define TIME_ANIM_IN 0
#define TIME_ANIM_ALPHA_IN 1
#define TIME_ANIM_OUT 2
#define TIME_ANIM_SIGN_CHANGE 3
#define TIME_ANIM_0_TO_9_OFFSET 4
#define TIME_ANIM_0_TO_9_B_OFFSET 10
#define TIME_ANIM_SET_POSITION 16
#define TIME_ANIM_SET_POSITION_B 17
#define TIME_ANIM_SET_POSITION_0_TO_9_B 18
#define TIME_ANIM_BEST_SCORE_ON_OFF 19
#define TIME_ANIM_FINISH_OFFSET 20
#define TIME_ANIM_FINISH_W 26
#define TIME_ANIM_LOOP_TIME 27
#define TIME_ANIM_ALPHA_OUT 28

#define TIME_NUM_ANIMS 29

#define TIME_NUM_DIGITS 6

bool dLytMiniGameTime_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("miniGameTime_00.brlyt", nullptr);

    for (int i = 0; i < TIME_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapTime[i].mFile, resAcc, mLyt.getLayout(), brlanMapTime[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
        mAnm[i].setAnimEnable(false);
    }

    sInstance = this;
    return true;
}

bool dLytMiniGameTime_c::remove() {
    if (sInstance == nullptr) {
        return true;
    }
    for (int i = 0; i < TIME_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    sInstance = nullptr;
    return true;
}

bool dLytMiniGameTime_c::execute() {
    if (sInstance == nullptr) {
        return true;
    }

    if (mIsVisible) {
        switch (mTimerVariant) {
            case TIMER_LONG:    updateLongTimer(); break;
            case TIMER_SECONDS: updateSecondsTimer(); break;
        }

        if (isInEndReached()) {
            stopIn();
            field_0x7EC = true;
        }

        if (isOutEndReached()) {
            stopOut();
            stopLoop();
            resetLoop();
            mIsVisible = false;
        }

        if (isAlphaInEndReached()) {
            stopAlphaIn();
            if (field_0x7EE) {
                startLoop();
            }
        }

        if (isAlphaOutEndReached()) {
            stopAlphaOut();
            if (field_0x7EE) {
                stopLoop();
                resetLoop();
            }
            mIsVisible = false;
        }

        if (isFinishEndReached()) {
            stopFinish();
            startLoop();
        }

        for (int i = 0; i < TIME_NUM_ANIMS; i++) {
            if (mAnm[i].isEnabled()) {
                mAnm[i].play();
            }
        }
        mLyt.calc();
    }

    if (mTimerVariant == TIMER_SECONDS) {
        if (mCurrentTimerValueSeconds <= 10 && mLastTimerValueSeconds != mCurrentTimerValueSeconds) {
            dSndSmallEffectMgr_c::GetInstance()->playMinigameTimeUpSound(mCurrentTimerValueSeconds);
        }
        saveSecondsTimer();
    }

    if (field_0x7F0 == 0) {
        if (mCurrentTimerValueSeconds >= 590 && mLastTimerValueSeconds != mCurrentTimerValueSeconds &&
            mCurrentTimerValueSeconds != 600) {
            dSndSmallEffectMgr_c::GetInstance()->playMinigameTimeUpSound(600 - mCurrentTimerValueSeconds);
        }
        saveSecondsTimer();
    }

    return true;
}

bool dLytMiniGameTime_c::draw() {
    if (sInstance == nullptr) {
        return true;
    }
    if (mIsVisible) {
        mLyt.addToDrawList();
    }
    return true;
}

void dLytMiniGameTime_c::init() {
    if (sInstance == nullptr) {
        return;
    }

    switch (field_0x7F0) {
        case 0: mTimerVariant = TIMER_LONG; break;
        case 1: mTimerVariant = TIMER_SECONDS; break;
        case 2: mTimerVariant = TIMER_SECONDS; break;
        case 3: mTimerVariant = TIMER_LONG; break;
        case 4: mTimerVariant = TIMER_LONG; break;
        case 5: mTimerVariant = TIMER_LONG; break;
    }

    resetAlphaIn();
    resetAlphaOut();
    resetOut();
    resetDigits();
    fn_8028EBB0();
    resetShowBestTime();
    resetFinish();
    resetLoop();
    initSignChange();
    resetIn();
}

void dLytMiniGameTime_c::fn_8028DD80() {
    if (sInstance == nullptr) {
        return;
    }

    if (field_0x7ED == true) {
        return;
    }

    mAnm[TIME_ANIM_ALPHA_IN].setToEnd2();
    mAnm[TIME_ANIM_ALPHA_IN].setAnimEnable(true);

    mAnm[TIME_ANIM_IN].setToStart();
    mAnm[TIME_ANIM_IN].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_ALPHA_IN].setAnimEnable(false);

    field_0x7ED = true;
    field_0x7F4 = true;
    mIsVisible = true;
}

void dLytMiniGameTime_c::fn_8028DE40() {
    if (sInstance == nullptr) {
        return;
    }

    if (field_0x7EC && field_0x7ED) {
        mAnm[TIME_ANIM_OUT].setToStart();
        mAnm[TIME_ANIM_OUT].setAnimEnable(true);

        field_0x7ED = false;
        field_0x7F4 = false;
    }
}

void dLytMiniGameTime_c::fn_8028DED0() {
    if (sInstance == nullptr) {
        return;
    }

    if (field_0x7EC && field_0x7ED != true && field_0x7F4) {
        if (!mAnm[TIME_ANIM_ALPHA_IN].isEnabled()) {
            if (mAnm[TIME_ANIM_ALPHA_OUT].isEnabled()) {
                mAnm[TIME_ANIM_ALPHA_OUT].setAnimEnable(false);
                // TODO maybe these inlines are wrong
                f32 frame = mAnm[TIME_ANIM_ALPHA_OUT].getLastFrame();
                f32 t = mAnm[TIME_ANIM_ALPHA_IN].getLastFrame();
                frame -= mAnm[TIME_ANIM_ALPHA_OUT].getFrame();
                if (t <= frame) {
                    frame = t - 1.0f;
                }
                mAnm[TIME_ANIM_ALPHA_IN].setFrame(frame);
            } else {
                mAnm[TIME_ANIM_ALPHA_IN].setToStart();
            }

            mAnm[TIME_ANIM_ALPHA_IN].setAnimEnable(true);
            field_0x7ED = true;
        }

        mIsVisible = true;
    }
}

void dLytMiniGameTime_c::fn_8028DFD0() {
    if (sInstance == nullptr) {
        return;
    }

    if (field_0x7EC && field_0x7ED) {
        if (!mAnm[TIME_ANIM_ALPHA_OUT].isEnabled()) {
            if (mAnm[TIME_ANIM_ALPHA_IN].isEnabled()) {
                mAnm[TIME_ANIM_ALPHA_IN].setAnimEnable(false);
                // TODO maybe these inlines are wrong
                f32 frame = mAnm[TIME_ANIM_ALPHA_IN].getLastFrame();
                f32 t = mAnm[TIME_ANIM_ALPHA_OUT].getLastFrame();
                frame -= mAnm[TIME_ANIM_ALPHA_IN].getFrame();
                if (t <= frame) {
                    frame = t - 1.0f;
                }
                mAnm[TIME_ANIM_ALPHA_OUT].setFrame(frame);
            } else {
                mAnm[TIME_ANIM_ALPHA_OUT].setToStart();
            }

            mAnm[TIME_ANIM_ALPHA_OUT].setAnimEnable(true);
            field_0x7ED = false;
        }
    }
}

void dLytMiniGameTime_c::fn_8028E0C0() {
    if (sInstance == nullptr) {
        return;
    }

    mAnm[TIME_ANIM_IN].setToEnd2();
    mAnm[TIME_ANIM_IN].setAnimEnable(true);

    mAnm[TIME_ANIM_ALPHA_IN].setToStart();
    mAnm[TIME_ANIM_ALPHA_IN].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_IN].setAnimEnable(false);
    mIsVisible = true;
}

void dLytMiniGameTime_c::fn_8028E170() {
    if (sInstance == nullptr) {
        return;
    }

    mAnm[TIME_ANIM_ALPHA_OUT].setToStart();
    mAnm[TIME_ANIM_ALPHA_OUT].setAnimEnable(true);
}

void dLytMiniGameTime_c::fn_8028E1D0() {
    if (sInstance == nullptr) {
        return;
    }

    for (int i = 0; i <= mHighestDigitIndex; i++) {
        mAnm[i + TIME_ANIM_FINISH_OFFSET].setToStart();
        mAnm[i + TIME_ANIM_FINISH_OFFSET].setAnimEnable(true);
    }

    mAnm[TIME_ANIM_FINISH_W].setToStart();
    mAnm[TIME_ANIM_FINISH_W].setAnimEnable(true);
}

void dLytMiniGameTime_c::startLoop() {
    if (sInstance == nullptr) {
        return;
    }

    mAnm[TIME_ANIM_LOOP_TIME].setFrame(0.0f);
    mAnm[TIME_ANIM_LOOP_TIME].setAnimEnable(true);
}

void dLytMiniGameTime_c::setTimerValueInMilliSeconds(s32 a) {
    if (sInstance == nullptr) {
        return;
    }
    mCurrentTimerValueMilliSeconds = a;
}

void dLytMiniGameTime_c::updateLongTimer() {
    s32 msec = mCurrentTimerValueMilliSeconds % 1000;
    s32 sec = mCurrentTimerValueMilliSeconds / 1000;
    s32 csec = msec / 10;
    s32 min = sec / 60;
    s32 sec1 = sec % 60;

    mCurrentTimerValueSeconds = sec1 + min * 60;

    s32 digits[6];
    // ten-minutes
    digits[5] = min / 10;
    // minutes
    digits[4] = min % 10;
    // ten-seconds
    digits[3] = sec1 / 10;
    // seconds
    digits[2] = sec1 % 10;
    // tenths-of-a-second
    digits[1] = csec / 10;
    // hundredths-of-a-second
    digits[0] = csec % 10;

    if (digits[5] != 0) {
        mHighestDigitIndex = 5;
        fn_8028F240(true);
    } else {
        mHighestDigitIndex = 4;
        fn_8028F240(false);
    }

    for (int i = 0; i <= mHighestDigitIndex; i++) {
        setDigit(i, digits[i]);
    }
}

void dLytMiniGameTime_c::setTimerSecondsValueInMilliSeconds(s32 time) {
    if (sInstance == nullptr) {
        return;
    }
    mCurrentTimerValueSeconds = time / 1000;
}

void dLytMiniGameTime_c::updateSecondsTimer() {
    if (mCurrentTimerValueSeconds > 999) {
        mCurrentTimerValueSeconds = 999;
    }
    s32 seconds = mCurrentTimerValueSeconds % 100;

    s32 digits[3];
    // 100-seconds
    digits[2] = mCurrentTimerValueSeconds / 100;
    // ten-seconds
    digits[1] = seconds / 10;
    // seconds
    digits[0] = seconds % 10;

    if (mCurrentTimerValueSeconds < 10) {
        mHighestDigitIndex = 0;
    } else if (mCurrentTimerValueSeconds < 100) {
        mHighestDigitIndex = 1;
    } else {
        mHighestDigitIndex = 2;
    }
    setHighestDigitIndex(mHighestDigitIndex);
    for (int i = 0; i <= mHighestDigitIndex; i++) {
        setDigit(i + 2, digits[i]);
    }
}

void dLytMiniGameTime_c::saveSecondsTimer() {
    mLastTimerValueSeconds = mCurrentTimerValueSeconds;
}

void dLytMiniGameTime_c::setBestTime(s32 time) {
    if (sInstance == nullptr) {
        return;
    }

    s32 offset = 1;

    s32 msec = time % 1000;
    s32 sec = time / 1000;
    s32 csec = msec / 10;
    s32 min = sec / 60;
    s32 sec1 = sec % 60;

    s32 digits[6];
    // ten-minutes
    digits[5] = min / 10;
    // minutes
    digits[4] = min % 10;
    // ten-seconds
    digits[3] = sec1 / 10;
    // seconds
    digits[2] = sec1 % 10;
    // tenths-of-a-second
    digits[1] = csec / 10;
    // hundredths-of-a-second
    digits[0] = csec % 10;

    if (digits[5] != 0) {
        offset = 0;
        setBestHasSixDigits(true);
    } else {
        setBestHasSixDigits(false);
    }

    for (int i = 0; i < 6 - offset; i++) {
        setDigitBest(i, digits[i]);
    }
}

void dLytMiniGameTime_c::setShowBestTime(bool show) {
    if (sInstance == nullptr) {
        return;
    }

    f32 frame = 1.0f;
    if (show) {
        frame = 0.0f;
    }
    mAnm[TIME_ANIM_BEST_SCORE_ON_OFF].setFrame(frame);
    mAnm[TIME_ANIM_BEST_SCORE_ON_OFF].setAnimEnable(true);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[TIME_ANIM_BEST_SCORE_ON_OFF].setAnimEnable(false);
}

void dLytMiniGameTime_c::resetIn() {
    mAnm[TIME_ANIM_IN].setForwardOnce();
    mAnm[TIME_ANIM_IN].setToStart();
    mAnm[TIME_ANIM_IN].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_IN].setAnimEnable(false);
}

void dLytMiniGameTime_c::resetOut() {
    mAnm[TIME_ANIM_OUT].setForwardOnce();
    mAnm[TIME_ANIM_OUT].setToStart();
    mAnm[TIME_ANIM_OUT].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_OUT].setAnimEnable(false);
}

void dLytMiniGameTime_c::resetAlphaIn() {
    mAnm[TIME_ANIM_ALPHA_IN].setForwardOnce();
    mAnm[TIME_ANIM_ALPHA_IN].setToStart();
    mAnm[TIME_ANIM_ALPHA_IN].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_ALPHA_IN].setAnimEnable(false);
}

void dLytMiniGameTime_c::resetAlphaOut() {
    mAnm[TIME_ANIM_ALPHA_OUT].setForwardOnce();
    mAnm[TIME_ANIM_ALPHA_OUT].setToStart();
    mAnm[TIME_ANIM_ALPHA_OUT].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_ALPHA_OUT].setAnimEnable(false);
}

void dLytMiniGameTime_c::resetDigits() {
    for (int i = 0; i < TIME_NUM_DIGITS; i++) {
        s32 idx = i + TIME_ANIM_0_TO_9_OFFSET;
        mAnm[idx].setForwardOnce();
        mAnm[idx].setToStart();
        mAnm[idx].setAnimEnable(true);
    }

    for (int i = 0; i < TIME_NUM_DIGITS; i++) {
        s32 idx = i + TIME_ANIM_0_TO_9_B_OFFSET;
        mAnm[idx].setForwardOnce();
        mAnm[idx].setToStart();
        mAnm[idx].setAnimEnable(true);
    }

    mLyt.calc();

    for (int i = 0; i < TIME_NUM_DIGITS; i++) {
        s32 idx = i + TIME_ANIM_0_TO_9_OFFSET;
        mAnm[idx].setAnimEnable(false);
    }

    for (int i = 0; i < TIME_NUM_DIGITS; i++) {
        s32 idx = i + TIME_ANIM_0_TO_9_B_OFFSET;
        mAnm[idx].setAnimEnable(false);
    }
}

void dLytMiniGameTime_c::fn_8028EBB0() {
    switch (mTimerVariant) {
        case TIMER_LONG:
            fn_8028F240(false);
            setBestHasSixDigits(false);
            break;
        case TIMER_SECONDS: setHighestDigitIndex(0); break;
    }
}

void dLytMiniGameTime_c::resetShowBestTime() {
    setShowBestTime(false);
}

void dLytMiniGameTime_c::resetFinish() {
    for (int i = 0; i < TIME_NUM_DIGITS; i++) {
        mAnm[i + TIME_ANIM_FINISH_OFFSET].setForwardOnce();
        mAnm[i + TIME_ANIM_FINISH_OFFSET].setToStart();
        mAnm[i + TIME_ANIM_FINISH_OFFSET].setAnimEnable(true);
        mLyt.calc();
    }

    mAnm[TIME_ANIM_FINISH_W].setForwardOnce();
    mAnm[TIME_ANIM_FINISH_W].setToStart();
    mAnm[TIME_ANIM_FINISH_W].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_FINISH_W].setAnimEnable(false);

    for (int i = 0; i < TIME_NUM_DIGITS; i++) {
        mAnm[i + TIME_ANIM_FINISH_OFFSET].setAnimEnable(false);
    }
}

void dLytMiniGameTime_c::resetLoop() {
    mAnm[TIME_ANIM_LOOP_TIME].setForwardLoop();
    mAnm[TIME_ANIM_LOOP_TIME].setFrame(0.0f);
    mAnm[TIME_ANIM_LOOP_TIME].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_LOOP_TIME].setAnimEnable(false);
}

void dLytMiniGameTime_c::initSignChange() {
    if (getCurrentLanguage1() == D_LANG_FR) {
        setSignChange(2);
        field_0x7D4 = 3;
    } else {
        setSignChange(1);
        field_0x7D4 = 3;
    }
}

void dLytMiniGameTime_c::stopIn() {
    mAnm[TIME_ANIM_IN].setAnimEnable(false);
}

void dLytMiniGameTime_c::stopOut() {
    mAnm[TIME_ANIM_OUT].setAnimEnable(false);
}

void dLytMiniGameTime_c::stopAlphaIn() {
    mAnm[TIME_ANIM_ALPHA_IN].setAnimEnable(false);
}

void dLytMiniGameTime_c::stopAlphaOut() {
    mAnm[TIME_ANIM_ALPHA_OUT].setAnimEnable(false);
}

void dLytMiniGameTime_c::stopFinish() {
    mAnm[TIME_ANIM_FINISH_W].setAnimEnable(false);

    for (int i = 0; i < TIME_NUM_DIGITS; i++) {
        mAnm[i + TIME_ANIM_FINISH_OFFSET].setAnimEnable(false);
    }
}

void dLytMiniGameTime_c::stopLoop() {
    mAnm[TIME_ANIM_LOOP_TIME].setAnimEnable(false);
}

bool dLytMiniGameTime_c::isInEndReached() const {
    if (mAnm[TIME_ANIM_IN].isEnabled() && mAnm[TIME_ANIM_IN].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameTime_c::isOutEndReached() const {
    if (mAnm[TIME_ANIM_OUT].isEnabled() && mAnm[TIME_ANIM_OUT].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameTime_c::isAlphaInEndReached() const {
    if (mAnm[TIME_ANIM_ALPHA_IN].isEnabled() && mAnm[TIME_ANIM_ALPHA_IN].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameTime_c::isAlphaOutEndReached() const {
    if (mAnm[TIME_ANIM_ALPHA_OUT].isEnabled() && mAnm[TIME_ANIM_ALPHA_OUT].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameTime_c::isFinishEndReached() const {
    if (mAnm[TIME_ANIM_FINISH_OFFSET].isEnabled() && mAnm[TIME_ANIM_FINISH_OFFSET].isEndReached()) {
        return true;
    }
    return false;
}

void dLytMiniGameTime_c::setHighestDigitIndex(u8 arg) {
    f32 frame = arg + field_0x7D4 + 2;

    mAnm[TIME_ANIM_SET_POSITION].setForwardOnce();
    mAnm[TIME_ANIM_SET_POSITION].setFrame(frame);
    mAnm[TIME_ANIM_SET_POSITION].setAnimEnable(true);

    mAnm[TIME_ANIM_SET_POSITION_B].setForwardOnce();
    mAnm[TIME_ANIM_SET_POSITION_B].setFrame(frame);
    mAnm[TIME_ANIM_SET_POSITION_B].setAnimEnable(true);

    mAnm[TIME_ANIM_SET_POSITION_0_TO_9_B].setForwardOnce();
    mAnm[TIME_ANIM_SET_POSITION_0_TO_9_B].setFrame(frame);
    mAnm[TIME_ANIM_SET_POSITION_0_TO_9_B].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_SET_POSITION].setAnimEnable(false);
    mAnm[TIME_ANIM_SET_POSITION_B].setAnimEnable(false);
    mAnm[TIME_ANIM_SET_POSITION_0_TO_9_B].setAnimEnable(false);
}

void dLytMiniGameTime_c::setSignChange(u32 arg) {
    f32 frame = arg;

    mAnm[TIME_ANIM_SIGN_CHANGE].setFrame(frame);
    mAnm[TIME_ANIM_SIGN_CHANGE].setAnimEnable(true);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[TIME_ANIM_SIGN_CHANGE].setAnimEnable(false);
}

void dLytMiniGameTime_c::fn_8028F240(bool b) {
    f32 frame = 0.0f;
    if (b) {
        frame = 1.0f;
    }

    mAnm[TIME_ANIM_SET_POSITION].setForwardOnce();
    mAnm[TIME_ANIM_SET_POSITION].setFrame(frame);
    mAnm[TIME_ANIM_SET_POSITION].setAnimEnable(true);

    mLyt.calc();

    mAnm[TIME_ANIM_SET_POSITION].setAnimEnable(false);
}

void dLytMiniGameTime_c::setBestHasSixDigits(bool b) {
    f32 frame = 0.0f;
    if (b) {
        frame = 1.0f;
    }

    mAnm[TIME_ANIM_SET_POSITION_B].setForwardOnce();
    mAnm[TIME_ANIM_SET_POSITION_B].setFrame(frame);
    mAnm[TIME_ANIM_SET_POSITION_B].setAnimEnable(true);

    mAnm[TIME_ANIM_SET_POSITION_0_TO_9_B].setForwardOnce();
    mAnm[TIME_ANIM_SET_POSITION_0_TO_9_B].setFrame(frame);
    mAnm[TIME_ANIM_SET_POSITION_0_TO_9_B].setAnimEnable(true);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[TIME_ANIM_SET_POSITION_0_TO_9_B].setAnimEnable(false);
    mAnm[TIME_ANIM_SET_POSITION_B].setAnimEnable(false);
}

void dLytMiniGameTime_c::setDigit(s32 digitIndex, s32 number) {
    s32 idx = digitIndex + TIME_ANIM_0_TO_9_OFFSET;
    f32 frame = number;

    mAnm[idx].setFrame(frame);
    mAnm[idx].setAnimEnable(true);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[idx].setAnimEnable(false);
}

void dLytMiniGameTime_c::setDigitBest(s32 digitIndex, s32 number) {
    s32 idx = digitIndex + TIME_ANIM_0_TO_9_B_OFFSET;
    f32 frame = number;

    mAnm[idx].setFrame(frame);
    mAnm[idx].setAnimEnable(true);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[idx].setAnimEnable(false);
}

static const d2d::LytBrlanMapping brlanMapStart[] = {
    {"start_00_321start.brlan", "G_321start_00"},
    {  "start_00_finish.brlan",   "G_finish_00"},
    {  "start_00_finish.brlan",   "G_timeUp_00"},
};

#define START_NUM_ANIMS 3

bool dLytMiniGameStart_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("start_00.brlyt", nullptr);

    for (int i = 0; i < START_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapStart[i].mFile, resAcc, mLyt.getLayout(), brlanMapStart[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
    }

    sInstance = this;
    return true;
}

bool dLytMiniGameStart_c::remove() {
    if (sInstance == nullptr) {
        return true;
    }
    for (int i = 0; i < START_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    sInstance = nullptr;
    return true;
}

bool dLytMiniGameStart_c::execute() {
    if (sInstance == nullptr) {
        return true;
    }

    for (int i = 0; i < START_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    mLyt.calc();

    return true;
}

bool dLytMiniGameStart_c::draw() {
    if (sInstance == nullptr) {
        return true;
    }
    mLyt.addToDrawList();
    return true;
}

void dLytMiniGameStart_c::init() {
    // TODO - ...
}

static const d2d::LytBrlanMapping brlanMapScore[] = {
    {           "miniGameScore_00_in.brlan",       "G_inOut_00"},
    {      "miniGameScore_00_alphaIn.brlan",  "G_alphaInOut_00"},
    {          "miniGameScore_00_out.brlan",       "G_inOut_00"},
    {         "miniGameScore_00_0to9.brlan",        "G_0to9_00"},
    {         "miniGameScore_00_0to9.brlan",        "G_0to9_01"},
    {         "miniGameScore_00_0to9.brlan",        "G_0to9_02"},
    {         "miniGameScore_00_0to9.brlan",        "G_0to9_03"},
    {         "miniGameScore_00_0to9.brlan",        "G_0to9_04"},
    {"miniGameScore_00_setPosition00.brlan", "G_setPosition_00"},
    {     "miniGameScore_00_getScore.brlan",    "G_getScore_00"},
    {     "miniGameScore_00_getScore.brlan",    "G_getScore_01"},
    {     "miniGameScore_00_getScore.brlan",    "G_getScore_02"},
    {     "miniGameScore_00_getScore.brlan",    "G_getScore_03"},
    {     "miniGameScore_00_getScore.brlan",    "G_getScore_04"},
    {     "miniGameScore_00_getScore.brlan",    "G_getTextF_00"},
    {  "miniGameScore_00_finishScore.brlan",    "G_getScore_00"},
    {  "miniGameScore_00_finishScore.brlan",    "G_getScore_01"},
    {  "miniGameScore_00_finishScore.brlan",    "G_getScore_02"},
    {  "miniGameScore_00_finishScore.brlan",    "G_getScore_03"},
    {  "miniGameScore_00_finishScore.brlan",    "G_getScore_04"},
    {  "miniGameScore_00_finishScore.brlan",   "G_getScoreW_00"},
    {  "miniGameScore_00_finishScore.brlan",   "G_getScoreX_00"},
    {  "miniGameScore_00_finishScore.brlan",    "G_getTextF_00"},
    {    "miniGameScore_00_loopScore.brlan",   "G_loopScore_00"},
    {  "miniGameScore_00_rupeeChange.brlan",  "G_rupyChange_00"},
    {     "miniGameScore_00_alphaOut.brlan",  "G_alphaInOut_00"},
};

#define SCORE_NUM_ANIMS 26

bool dLytMiniGameScore_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("miniGameScore_00.brlyt", nullptr);

    for (int i = 0; i < SCORE_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapScore[i].mFile, resAcc, mLyt.getLayout(), brlanMapScore[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
    }

    sInstance = this;
    return true;
}

bool dLytMiniGameScore_c::remove() {
    if (sInstance == nullptr) {
        return true;
    }
    for (int i = 0; i < SCORE_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    sInstance = nullptr;
    return true;
}

bool dLytMiniGameScore_c::execute() {
    if (sInstance == nullptr) {
        return true;
    }

    for (int i = 0; i < SCORE_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    mLyt.calc();

    return true;
}

bool dLytMiniGameScore_c::draw() {
    if (sInstance == nullptr) {
        return true;
    }
    mLyt.addToDrawList();
    return true;
}

void dLytMiniGameScore_c::init() {
    // TODO - ...
}

static const d2d::LytBrlanMapping brlanMapScoreSd[] = {
    {         "miniGameScoreSd_00_0to9.brlan",        "G_0to9_00"},
    {         "miniGameScoreSd_00_0to9.brlan",        "G_0to9_01"},
    {"miniGameScoreSd_00_setPosition00.brlan", "G_setPosition_00"},
    {      "miniGameScoreSd_00_scoreUp.brlan",     "G_animAll_00"},
    {    "miniGameScoreSd_00_scoreDown.brlan",     "G_animAll_00"},
};

#define SCORE_SD_NUM_ANIMS 5

bool dLytMiniGameScoreSd_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("miniGameScoreSd_00.brlyt", nullptr);

    for (int i = 0; i < SCORE_SD_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapScoreSd[i].mFile, resAcc, mLyt.getLayout(), brlanMapScoreSd[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
    }

    sInstance = this;
    return true;
}

bool dLytMiniGameScoreSd_c::remove() {
    if (sInstance == nullptr) {
        return true;
    }
    for (int i = 0; i < SCORE_SD_NUM_ANIMS; i++) {
        mAnm[i].unbind();
        mAnm[i].remove();
    }

    sInstance = nullptr;
    return true;
}

bool dLytMiniGameScoreSd_c::execute() {
    if (sInstance == nullptr) {
        return true;
    }

    for (int i = 0; i < SCORE_SD_NUM_ANIMS; i++) {
        if (mAnm[i].isEnabled()) {
            mAnm[i].play();
        }
    }

    mLyt.calc();

    return true;
}

bool dLytMiniGameScoreSd_c::draw() {
    if (sInstance == nullptr) {
        return true;
    }
    mLyt.addToDrawList();
    return true;
}

void dLytMiniGameScoreSd_c::init() {
    // TODO - ...
}

SPECIAL_BASE_PROFILE(LYT_MINI_GAME, dLytMiniGame_c, fProfile::LYT_MINI_GAME, 0x2B7, 1);
