#include "d/lyt/d_lyt_mini_game.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/d_lang.h"
#include "d/d_message.h"
#include "d/d_rawarchive.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/lyt/d2d.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/lyt/lyt_pane.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/math/math_arithmetic.h"
#include "nw4r/math/math_types.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/minigame_mgr.h"

dLytMiniGamePumpkin_c *dLytMiniGamePumpkin_c::sInstance;
dLytMiniGameBugs_c *dLytMiniGameBugs_c::sInstance;
dLytMiniGameTime_c *dLytMiniGameTime_c::sInstance;
dLytMiniGameStart_c *dLytMiniGameStart_c::sInstance;
dLytMiniGameScore_c *dLytMiniGameScore_c::sInstance;
dLytMiniGameScoreSd_c *dLytMiniGameScoreSd_c::sInstance;
dLytMiniGame_c *dLytMiniGame_c::sInstance;

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

void dLytMiniGameBugs_c::startOutTemp() {
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

bool dLytMiniGameBugs_c::isSlotBugCollected(s32 slot) const {
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
        switch (mTimeFormat) {
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

    if (mTimeFormat == TIMER_SECONDS) {
        if (mCurrentTimerValueSeconds <= 10 && mLastTimerValueSeconds != mCurrentTimerValueSeconds) {
            dSndSmallEffectMgr_c::GetInstance()->playMinigameTimeUpSound(mCurrentTimerValueSeconds);
        }
        saveSecondsTimer();
    }

    if (mVariant == TIME_VARIANT_INSECT_CAPTURE) {
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

    switch (mVariant) {
        case TIME_VARIANT_INSECT_CAPTURE:         mTimeFormat = TIMER_LONG; break;
        case TIME_VARIANT_SPIRAL_CHARGE_TUTORIAL: mTimeFormat = TIMER_SECONDS; break;
        case TIME_VARIANT_PUMPKIN_ARCHERY:        mTimeFormat = TIMER_SECONDS; break;
        case TIME_VARIANT_ROLLERCOASTER:          mTimeFormat = TIMER_LONG; break;
        case TIME_VARIANT_BOSS_RUSH:              mTimeFormat = TIMER_LONG; break;
        case TIME_VARIANT_TRIAL_TIME_ATTACK:      mTimeFormat = TIMER_LONG; break;
    }

    resetAlphaIn();
    resetAlphaOut();
    resetOut();
    resetDigits();
    resetPosition();
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

void dLytMiniGameTime_c::startOut() {
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

void dLytMiniGameTime_c::startIn() {
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

void dLytMiniGameTime_c::startOutTemp() {
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

void dLytMiniGameTime_c::startAlphaIn() {
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

void dLytMiniGameTime_c::startAlphaOut() {
    if (sInstance == nullptr) {
        return;
    }

    mAnm[TIME_ANIM_ALPHA_OUT].setToStart();
    mAnm[TIME_ANIM_ALPHA_OUT].setAnimEnable(true);
}

void dLytMiniGameTime_c::startFinish() {
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

void dLytMiniGameTime_c::setTimerValueInMilliSeconds(s32 time) {
    if (sInstance == nullptr) {
        return;
    }
    mCurrentTimerValueMilliSeconds = time;
}

void dLytMiniGameTime_c::updateLongTimer() {
    s32 seconds;
    s32 timeSeconds = mCurrentTimerValueMilliSeconds / 1000;
    s32 centiSeconds = (mCurrentTimerValueMilliSeconds % 1000) / 10;
    seconds = timeSeconds % 60;

    mCurrentTimerValueSeconds = seconds + (timeSeconds / 60) * 60;

    s32 digits[6];
    // ten-minutes
    digits[5] = (timeSeconds / 60) / 10;
    // minutes
    digits[4] = (timeSeconds / 60) % 10;
    // ten-seconds
    digits[3] = seconds / 10;
    // seconds
    digits[2] = seconds % 10;
    // tenths-of-a-second
    digits[1] = centiSeconds / 10;
    // hundredths-of-a-second
    digits[0] = centiSeconds % 10;

    if (digits[5] != 0) {
        mHighestDigitIndex = 5;
        setTimePosition(true);
    } else {
        mHighestDigitIndex = 4;
        setTimePosition(false);
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
    s32 seconds;

    s32 timeSeconds = time / 1000;
    s32 centiSeconds = time % 1000 / 10;
    seconds = timeSeconds % 60;

    s32 digits[6];
    // ten-minutes
    digits[5] = (timeSeconds / 60) / 10;
    // minutes
    digits[4] = (timeSeconds / 60) % 10;
    // ten-seconds
    digits[3] = seconds / 10;
    // seconds
    digits[2] = seconds % 10;
    // tenths-of-a-second
    digits[1] = centiSeconds / 10;
    // hundredths-of-a-second
    digits[0] = centiSeconds % 10;

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

void dLytMiniGameTime_c::resetPosition() {
    switch (mTimeFormat) {
        case TIMER_LONG:
            setTimePosition(false);
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
        // NOTE: Unnecessary calc here?
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

void dLytMiniGameTime_c::setTimePosition(bool b) {
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

#define START_ANIM_COUNTDOWN 0
#define START_ANIM_FINISH 1
#define START_ANIM_TIMEUP 2

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

    if (mIsVisible) {
        if (checkCountdown3()) {
            dSndSmallEffectMgr_c::GetInstance()->playMinigameCountdownSound(3);
        }

        if (checkCountdown2()) {
            dSndSmallEffectMgr_c::GetInstance()->playMinigameCountdownSound(2);
        }

        if (checkCountdown1()) {
            dSndSmallEffectMgr_c::GetInstance()->playMinigameCountdownSound(1);
        }

        if (checkStart()) {
            dSndSmallEffectMgr_c::GetInstance()->playMinigameStartSound();
        }

        if (checkFinish()) {
            dSndSmallEffectMgr_c::GetInstance()->playMinigameFinishSound();
        }

        if (checkTimeup()) {
            dSndSmallEffectMgr_c::GetInstance()->playMinigameFinishSound();
        }

        if (isCountdownEndReached()) {
            stopCountdown();
            mIsVisible = false;
        }

        if (isFinishEndReached()) {
            stopFinish();
            resetPlayedSounds();
            mIsVisible = false;
        }

        if (isTimeupEndReached()) {
            stopTimeup();
            resetPlayedSounds();
            mIsVisible = false;
        }

        for (int i = 0; i < START_NUM_ANIMS; i++) {
            if (mAnm[i].isEnabled()) {
                mAnm[i].play();
            }
        }
        mLyt.calc();
    }

    return true;
}

bool dLytMiniGameStart_c::draw() {
    if (sInstance == nullptr) {
        return true;
    }

    if (mIsVisible) {
        mLyt.addToDrawList();
    }
    return true;
}

void dLytMiniGameStart_c::init() {
    if (sInstance == nullptr) {
        return;
    }

    mAnm[START_ANIM_COUNTDOWN].setForwardOnce();
    mAnm[START_ANIM_COUNTDOWN].setToEnd2();
    mAnm[START_ANIM_COUNTDOWN].setAnimEnable(true);

    mAnm[START_ANIM_FINISH].setForwardOnce();
    mAnm[START_ANIM_FINISH].setToEnd2();
    mAnm[START_ANIM_FINISH].setAnimEnable(true);

    mAnm[START_ANIM_TIMEUP].setForwardOnce();
    mAnm[START_ANIM_TIMEUP].setToEnd2();
    mAnm[START_ANIM_TIMEUP].setAnimEnable(true);

    mLyt.calc();

    mAnm[START_ANIM_COUNTDOWN].setAnimEnable(false);
    mAnm[START_ANIM_FINISH].setAnimEnable(false);
    mAnm[START_ANIM_TIMEUP].setAnimEnable(false);

    resetPlayedSounds();
}

void dLytMiniGameStart_c::startCountdown() {
    if (sInstance == nullptr) {
        return;
    }

    resetCountdown();
    mAnm[START_ANIM_COUNTDOWN].setAnimEnable(true);
    mIsVisible = true;
}

void dLytMiniGameStart_c::startCountdown120() {
    if (sInstance == nullptr) {
        return;
    }

    resetCountdown();
    mAnm[START_ANIM_COUNTDOWN].setAnimEnable(true);
    mAnm[START_ANIM_COUNTDOWN].setFrame(120.0f);
    mIsVisible = true;
}

void dLytMiniGameStart_c::startFinish() {
    if (sInstance == nullptr) {
        return;
    }

    resetFinish();
    mAnm[START_ANIM_FINISH].setAnimEnable(true);
    mIsVisible = true;
}

void dLytMiniGameStart_c::startTimeup() {
    if (sInstance == nullptr) {
        return;
    }

    resetStart();
    mAnm[START_ANIM_TIMEUP].setAnimEnable(true);
    mIsVisible = true;
}

bool dLytMiniGameStart_c::isCountdownEndReached() const {
    if (sInstance == nullptr) {
        return false;
    }
    if (mAnm[START_ANIM_COUNTDOWN].isEnabled() && mAnm[START_ANIM_COUNTDOWN].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameStart_c::isFinishEndReached() const {
    if (sInstance == nullptr) {
        return false;
    }
    if (mAnm[START_ANIM_FINISH].isEnabled() && mAnm[START_ANIM_FINISH].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameStart_c::isTimeupEndReached() const {
    if (sInstance == nullptr) {
        return false;
    }
    if (mAnm[START_ANIM_TIMEUP].isEnabled() && mAnm[START_ANIM_TIMEUP].isEndReached()) {
        return true;
    }
    return false;
}

void dLytMiniGameStart_c::resetPlayedSounds() {
    for (int i = 0; i < 6; i++) {
        mPlayedSounds[i] = false;
    }
}

void dLytMiniGameStart_c::resetCountdown() {
    mAnm[START_ANIM_COUNTDOWN].setForwardOnce();
    mAnm[START_ANIM_COUNTDOWN].setFrame(0.0f);
    mAnm[START_ANIM_COUNTDOWN].setAnimEnable(false);
}

void dLytMiniGameStart_c::resetFinish() {
    mAnm[START_ANIM_FINISH].setForwardOnce();
    mAnm[START_ANIM_FINISH].setFrame(0.0f);
    mAnm[START_ANIM_FINISH].setAnimEnable(false);
}

void dLytMiniGameStart_c::resetStart() {
    mAnm[START_ANIM_TIMEUP].setForwardOnce();
    mAnm[START_ANIM_TIMEUP].setFrame(0.0f);
    mAnm[START_ANIM_TIMEUP].setAnimEnable(false);
}

void dLytMiniGameStart_c::stopCountdown() {
    mAnm[START_ANIM_COUNTDOWN].setAnimEnable(false);
}

void dLytMiniGameStart_c::stopFinish() {
    mAnm[START_ANIM_FINISH].setAnimEnable(false);
}

void dLytMiniGameStart_c::stopTimeup() {
    mAnm[START_ANIM_TIMEUP].setAnimEnable(false);
}

bool dLytMiniGameStart_c::checkCountdown3() {
    if (mAnm[START_ANIM_COUNTDOWN].isEnabled() && !mPlayedSounds[0] && 32.0f <= mAnm[START_ANIM_COUNTDOWN].getFrame()) {
        mPlayedSounds[0] = true;
        return true;
    }
    return false;
}

bool dLytMiniGameStart_c::checkCountdown2() {
    if (mAnm[START_ANIM_COUNTDOWN].isEnabled() && !mPlayedSounds[1] && 62.0f <= mAnm[START_ANIM_COUNTDOWN].getFrame()) {
        mPlayedSounds[1] = true;
        return true;
    }
    return false;
}

bool dLytMiniGameStart_c::checkCountdown1() {
    if (mAnm[START_ANIM_COUNTDOWN].isEnabled() && !mPlayedSounds[2] && 92.0f <= mAnm[START_ANIM_COUNTDOWN].getFrame()) {
        mPlayedSounds[2] = true;
        return true;
    }
    return false;
}

bool dLytMiniGameStart_c::checkStart() {
    if (mAnm[START_ANIM_COUNTDOWN].isEnabled() && !mPlayedSounds[3] &&
        122.0f <= mAnm[START_ANIM_COUNTDOWN].getFrame()) {
        mPlayedSounds[3] = true;
        return true;
    }
    return false;
}

bool dLytMiniGameStart_c::checkFinish() {
    if (mAnm[START_ANIM_FINISH].isEnabled() && !mPlayedSounds[4] && 0.0f <= mAnm[START_ANIM_FINISH].getFrame()) {
        mPlayedSounds[4] = true;
        return true;
    }
    return false;
}

bool dLytMiniGameStart_c::checkTimeup() {
    if (mAnm[START_ANIM_TIMEUP].isEnabled() && !mPlayedSounds[5] && 0.0f <= mAnm[START_ANIM_TIMEUP].getFrame()) {
        mPlayedSounds[5] = true;
        return true;
    }
    return false;
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

#define SCORE_ANIM_IN 0
#define SCORE_ANIM_ALPHA_IN 1
#define SCORE_ANIM_OUT 2
#define SCORE_ANIM_0_TO_9_OFFSET 3

#define SCORE_ANIM_SET_POSITION 8

#define SCORE_ANIM_GET_SCORE_OFFSET 9
#define SCORE_ANIM_GET_SCORE_GET_TEXT_F 14
#define SCORE_ANIM_FINISH_SCORE_OFFSET 15
#define SCORE_ANIM_FINISH_SCORE_GET_SCORE_W 20
#define SCORE_ANIM_FINISH_SCORE_GET_SCORE_X 21
#define SCORE_ANIM_FINISH_SCORE_GET_SCORE_F 22

#define SCORE_ANIM_LOOP_SCORE 23
#define SCORE_ANIM_RUPEE_CHANGE 24
#define SCORE_ANIM_ALPHA_OUT 25

#define SCORE_NUM_ANIMS 26
#define SCORE_NUM_DIGITS 5

static const char *sScoreTextBoxes[] = {
    "T_unitText_00",
    "T_unitTextS_00",
    "T_unitTextF_00",
};

#define SCORE_NUM_TEXTBOXES 3

static const char *sScorePanes[] = {
    "N_center_00",   "N_positionAll_00", "N_position_00", "N_unit_00",     "N_bg_00",
    "P_bgBase_00",   "P_bgFrameS_00",    "P_bgFrameS_01", "P_bgFrameS_02", "P_bgFrame_00",
    "P_bgFrame_01",  "P_bgFrame_02",     "P_target_00",   "P_rupy_00",     "P_question_00",
    "P_position_00", "P_position_01",    "P_position_02", "P_position_03", "P_position_04",
};

#define SCORE_PANE_CENTER 0
#define SCORE_PANE_POSITION_ALL 1
#define SCORE_PANE_POSITION 2
#define SCORE_PANE_UNIT 3

#define SCORE_PANE_P_POSITION_0 15

#define SCORE_NUM_PANES 20

bool dLytMiniGameScore_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("miniGameScore_00.brlyt", nullptr);

    for (int i = 0; i < SCORE_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapScore[i].mFile, resAcc, mLyt.getLayout(), brlanMapScore[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
        mAnm[i].setAnimEnable(false);
    }

    for (int i = 0; i < SCORE_NUM_TEXTBOXES; i++) {
        mpTextBoxes[i] = mLyt.getTextBox(sScoreTextBoxes[i]);
    }

    for (int i = 0; i < SCORE_NUM_PANES; i++) {
        mpPanes[i] = mLyt.findPane(sScorePanes[i]);
    }

    for (int i = 0; i < 3; i++) {
        const nw4r::math::VEC3 &translate = mpPanes[i + 2]->GetTranslate();
        field_0x834[i][0] = translate.x;
        field_0x834[i][1] = translate.y;
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

    if (field_0x804) {
        if (field_0x80C-- == 0) {
            field_0x804 = false;
            field_0x829 = false;
        } else {
            increaseScore(field_0x810);
        }
    }

    if (mIsVisible) {
        fn_80291BC0();
        if (!field_0x816) {
            switch (mVariant) {
                case SCORE_VARIANT_BAMBOO_CUTTING:
                    if (hasIncreasedScore()) {
                        startGetScore();
                    }
                    break;
                case SCORE_VARIANT_THRILL_DIGGER:
                    if (field_0x829) {
                        if (hasIncreasedScore()) {
                            startGetScore();
                        }

                        if (hasDecreasedScore()) {
                            // nothing
                        }

                        field_0x829 = false;
                    }
                    break;
                case SCORE_VARIANT_FUN_FUN_ISLAND:
                    if (mScore != 1) {
                        if (hasIncreasedScore()) {
                            startGetScore();
                        }
                    }
                    break;
                case SCORE_VARIANT_PUMPKIN_ARCHERY:
                case SCORE_VARIANT_SPIRAL_CHARGE_TUTORIAL:
                    if (hasIncreasedScore()) {
                        startGetScore();
                    }
                    break;
            }
        }

        if (isInEndReached()) {
            stopIn();
            field_0x814 = true;
        }

        if (isOutEndReached()) {
            stopOut();
            stopLoop();
            resetLoop();
            mIsVisible = false;
        }

        if (isAlphaInEndReached()) {
            stopAlphaIn();
            if (field_0x816) {
                startLoop();
            }
        }

        if (isAlphaOutEndReached()) {
            stopAlphaOut();
            if (field_0x816) {
                stopLoop();
                resetLoop();
            }
            mIsVisible = false;
        }

        if (isGetScoreEndReached()) {
            stopGetScore();
        }

        if (isFinishEndReached()) {
            stopFinish();
            startLoop();
        }

        for (int i = 0; i < SCORE_NUM_ANIMS; i++) {
            if (mAnm[i].isEnabled()) {
                mAnm[i].play();
            }
        }

        mLyt.calc();
    }

    if (!field_0x816 && mVariant == SCORE_VARIANT_BAMBOO_CUTTING && hasIncreasedScore()) {
        dSndSmallEffectMgr_c::GetInstance()->playMinigameScoreUpSound(mScore);
    }

    saveScore();

    return true;
}

bool dLytMiniGameScore_c::draw() {
    if (sInstance == nullptr) {
        return true;
    }
    if (mIsVisible) {
        mLyt.addToDrawList();
    }
    return true;
}

// TODO this is totally arbitrary and makes no sense but it sort of looks like it generates close-ish code
// Assume the TODO also covers all the calls to `calc` below
struct PaneCalc {
    PaneCalc() {
        for (int i = 4; i >= 0; i--) {
            work[i][1] = work[i][0] = 0.0f;
        }
    }

    void calc(nw4r::lyt::Pane *p1, nw4r::lyt::Pane *p2) {
        nw4r::lyt::Pane *pp1 = p1->GetParent();
        nw4r::lyt::Pane *pp2 = p2->GetParent();

        work[0][0] = p1->GetScale().x;
        work[0][1] = p1->GetScale().y;

        work[1][0] = p1->GetTranslate().x + pp1->GetTranslate().x;
        work[1][1] = p1->GetTranslate().y;

        work[2][0] = p2->GetTranslate().x + pp2->GetTranslate().x;
        work[2][1] = p2->GetTranslate().y;

        work[3][0] = p1->GetSize().width * p1->GetScale().x;
        work[3][1] = p1->GetSize().height * p1->GetScale().y;

        work[4][0] = p2->GetSize().width * p2->GetScale().x;
        work[4][1] = p2->GetSize().height * p2->GetScale().y;
    }

    f32 work[5][2];
};

void dLytMiniGameScore_c::init() {
    if (sInstance == nullptr) {
        return;
    }

    // maybe?
    PaneCalc calc;

    mAnm[SCORE_ANIM_IN].setForwardOnce();
    mAnm[SCORE_ANIM_IN].setToEnd2();
    mAnm[SCORE_ANIM_IN].setAnimEnable(true);
    mLyt.calc();
    mAnm[SCORE_ANIM_IN].setAnimEnable(false);

    switch (mVariant) {
        case SCORE_VARIANT_BAMBOO_CUTTING:
            field_0x82C = 0;
            field_0x81C = 21;
            field_0x7E4 = 4;
            dMessage_c::getGlobalTagProcessor()->setNumericArg0(mScore);
            loadTextVariant(1);
            fn_80293410();
            switch (field_0x830) {
                case 1:
                    mpTextBoxes[0]->SetTextPositionH(0);
                    mpTextBoxes[1]->SetTextPositionH(0);
                    mpTextBoxes[2]->SetTextPositionH(0);
                    break;
                case 2:
                    mpTextBoxes[0]->SetTextPositionH(2);
                    mpTextBoxes[1]->SetTextPositionH(2);
                    mpTextBoxes[2]->SetTextPositionH(2);
                    break;
            }

            field_0x7E8 = mpTextBoxes[0]->GetLineWidth(nullptr);
            for (int i = 0; i < field_0x7E4; i++) {
                mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C + i);
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);
                mLyt.calc();
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);

                calc.calc(mpPanes[i + 15], mpPanes[15]);
                mFloats[field_0x81C + i] =
                    calc.work[3][0] / 2.0f + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
                mFloats[field_0x81C + i] += field_0x7FC;
                mFloats[field_0x81C + i] += field_0x800;
            }

            fn_80293450(field_0x81C);
            break;
        case SCORE_VARIANT_FUN_FUN_ISLAND:
            field_0x830 = 0;
            field_0x82C = 0;
            field_0x81C = 11;
            field_0x7E4 = 2;

            for (int i = 0; i < field_0x7E4; i++) {
                mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C + i);
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);
                mLyt.calc();
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);

                calc.calc(mpPanes[14], mpPanes[15]);
                mFloats[field_0x81C + i] =
                    calc.work[3][0] / 2.0f + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
            }

            for (int i = 0; i < field_0x7E4; i++) {
                mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C + i + 2);
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);
                mLyt.calc();
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);

                calc.calc(mpPanes[14], mpPanes[15]);
                mFloats[field_0x81C + i + 2] =
                    calc.work[3][0] / 2.0f + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
            }

            mAnm[SCORE_ANIM_RUPEE_CHANGE].setFrame(5.0f);
            mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(true);
            mLyt.calc();
            mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(false);

            for (int i = 0; i < field_0x7E4; i++) {
                mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C + i + 4);
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);
                mLyt.calc();
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);

                calc.calc(mpPanes[14], mpPanes[15]);
                mFloats[field_0x81C + i + 4] =
                    calc.work[3][0] / 2.0f + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
            }

            mAnm[SCORE_ANIM_RUPEE_CHANGE].setFrame(4.0f);
            mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(true);
            mLyt.calc();
            mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(false);

            for (int i = 0; i < field_0x7E4; i++) {
                mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C + i + 6);
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);
                mLyt.calc();
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);

                calc.calc(mpPanes[13], mpPanes[15]);
                mFloats[field_0x81C + i + 6] =
                    calc.work[3][0] / 2.0f + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
            }
            fn_80293450(field_0x81C);
            break;
        case SCORE_VARIANT_THRILL_DIGGER:
            field_0x830 = 0;
            field_0x82C = 0;
            field_0x81C = 6;
            field_0x7E4 = 5;

            for (int i = 0; i < field_0x7E4; i++) {
                mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C + i);
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);
                mLyt.calc();
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);

                calc.calc(mpPanes[14], mpPanes[15]);
                mFloats[field_0x81C + i + 6] =
                    calc.work[3][0] / 2.0f + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
            }

            fn_80293450(field_0x81C);
            break;
        case SCORE_VARIANT_PUMPKIN_ARCHERY:
            field_0x82C = 0;
            field_0x81C = 21;
            field_0x7E4 = 4;
            dMessage_c::getGlobalTagProcessor()->setNumericArg0(mScore);
            loadTextVariant(0);
            fn_80293410();
            switch (field_0x830) {
                case 1:
                    mpTextBoxes[0]->SetTextPositionH(0);
                    mpTextBoxes[1]->SetTextPositionH(0);
                    mpTextBoxes[2]->SetTextPositionH(0);
                    break;
                case 2:
                    mpTextBoxes[0]->SetTextPositionH(2);
                    mpTextBoxes[1]->SetTextPositionH(2);
                    mpTextBoxes[2]->SetTextPositionH(2);
                    break;
            }

            field_0x7E8 = mpTextBoxes[0]->GetLineWidth(nullptr);
            for (int i = 0; i < field_0x7E4; i++) {
                mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C + i);
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);
                mLyt.calc();
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);

                calc.calc(mpPanes[i + 15], mpPanes[15]);
                mFloats[field_0x81C + i] =
                    calc.work[3][0] / 2.0f + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
                mFloats[field_0x81C + i] += field_0x7FC;
                mFloats[field_0x81C + i] += field_0x800;
            }

            fn_80293450(field_0x81C);
            break;
        case SCORE_VARIANT_SPIRAL_CHARGE_TUTORIAL:
            field_0x830 = 0;
            field_0x82C = 0;
            field_0x81C = 19;
            field_0x7E4 = 2;

            for (int i = 0; i < field_0x7E4; i++) {
                mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C + i);
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);
                mLyt.calc();
                mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);

                calc.calc(mpPanes[12], mpPanes[15]);
                // NOTE: 2.0 instead of 2.0f, so this division can't be an inline...
                mFloats[field_0x81C + i] =
                    calc.work[3][0] / 2.0 + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
            }

            fn_80293450(field_0x81C);
            break;
    }

    resetDigits();
    resetSetPosition();
    resetRupeeChange();
    resetGetScore();
    resetFinish();
    resetLoop();
    setScore(0);
    resetAlphaIn();
    resetAlphaOut();
    resetOut();
    resetIn();
    mLastScore = 0;
    field_0x804 = false;
    field_0x808 = 0;
    field_0x80C = 0;
    field_0x810 = 0;
}

void dLytMiniGameScore_c::fn_80291410() {
    if (sInstance == nullptr) {
        return;
    }

    if (field_0x815 == true) {
        return;
    }

    mAnm[SCORE_ANIM_ALPHA_IN].setToEnd2();
    mAnm[SCORE_ANIM_ALPHA_IN].setAnimEnable(true);
    mAnm[SCORE_ANIM_IN].setToStart();
    mAnm[SCORE_ANIM_IN].setAnimEnable(true);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[SCORE_ANIM_ALPHA_IN].setAnimEnable(false);
    field_0x815 = true;
    field_0x820 = true;
    mIsVisible = true;

    if (mVariant == SCORE_VARIANT_FUN_FUN_ISLAND) {
        setScore(1);
        calcNumDigits();
        mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C);
        mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);
        fn_80292C30();
        mAnm[SCORE_ANIM_0_TO_9_OFFSET].setFrame(1.0f);
        mAnm[SCORE_ANIM_0_TO_9_OFFSET].setAnimEnable(true);

        mLyt.getLayout()->Animate(0);
        mLyt.calc();

        mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);
        mAnm[SCORE_ANIM_0_TO_9_OFFSET].setAnimEnable(false);
    }
}

void dLytMiniGameScore_c::startOut() {
    if (sInstance == nullptr) {
        return;
    }

    if (field_0x814 && field_0x815) {
        mAnm[SCORE_ANIM_OUT].setToStart();
        mAnm[SCORE_ANIM_OUT].setAnimEnable(true);

        field_0x815 = false;
        field_0x820 = false;
    }
}

void dLytMiniGameScore_c::startIn() {
    if (sInstance == nullptr) {
        return;
    }

    if (field_0x814 && field_0x815 != true && field_0x820) {
        if (!mAnm[SCORE_ANIM_ALPHA_IN].isEnabled()) {
            if (mAnm[SCORE_ANIM_ALPHA_OUT].isEnabled()) {
                mAnm[SCORE_ANIM_ALPHA_OUT].setAnimEnable(false);
                // TODO maybe these inlines are wrong
                f32 frame = mAnm[SCORE_ANIM_ALPHA_OUT].getLastFrame();
                f32 t = mAnm[SCORE_ANIM_ALPHA_IN].getLastFrame();
                frame -= mAnm[SCORE_ANIM_ALPHA_OUT].getFrame();
                if (t <= frame) {
                    frame = t - 1.0f;
                }
                mAnm[SCORE_ANIM_ALPHA_IN].setFrame(frame);
            } else {
                mAnm[SCORE_ANIM_ALPHA_IN].setToStart();
            }

            mAnm[SCORE_ANIM_ALPHA_IN].setAnimEnable(true);
            field_0x815 = true;
        }

        mIsVisible = true;
    }
}

void dLytMiniGameScore_c::startOutTemp() {
    if (sInstance == nullptr) {
        return;
    }

    if (field_0x814 && field_0x815) {
        if (!mAnm[SCORE_ANIM_ALPHA_OUT].isEnabled()) {
            if (mAnm[SCORE_ANIM_ALPHA_IN].isEnabled()) {
                mAnm[SCORE_ANIM_ALPHA_IN].setAnimEnable(false);
                // TODO maybe these inlines are wrong
                f32 frame = mAnm[SCORE_ANIM_ALPHA_IN].getLastFrame();
                f32 t = mAnm[SCORE_ANIM_ALPHA_OUT].getLastFrame();
                frame -= mAnm[SCORE_ANIM_ALPHA_IN].getFrame();
                if (t <= frame) {
                    frame = t - 1.0f;
                }
                mAnm[SCORE_ANIM_ALPHA_OUT].setFrame(frame);
            } else {
                mAnm[SCORE_ANIM_ALPHA_OUT].setToStart();
            }

            mAnm[SCORE_ANIM_ALPHA_OUT].setAnimEnable(true);
            field_0x815 = false;
        }
    }
}

void dLytMiniGameScore_c::startAlphaIn() {
    if (sInstance == nullptr) {
        return;
    }

    mAnm[SCORE_ANIM_ALPHA_OUT].setAnimEnable(false);

    mAnm[SCORE_ANIM_IN].setToEnd2();
    mAnm[SCORE_ANIM_IN].setAnimEnable(true);

    mAnm[SCORE_ANIM_ALPHA_IN].setToStart();
    mAnm[SCORE_ANIM_ALPHA_IN].setAnimEnable(true);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[SCORE_ANIM_IN].setAnimEnable(false);
    mIsVisible = true;
}

void dLytMiniGameScore_c::startAlphaOut() {
    if (sInstance == nullptr) {
        return;
    }

    mAnm[SCORE_ANIM_ALPHA_OUT].setToStart();
    mAnm[SCORE_ANIM_ALPHA_OUT].setAnimEnable(true);
}

void dLytMiniGameScore_c::startGetScore() {
    if (sInstance == nullptr) {
        return;
    }

    if (!mIsVisible) {
        return;
    }

    for (int i = 0; i <= mHighestDigitIndex; i++) {
        mAnm[i + SCORE_ANIM_GET_SCORE_OFFSET].setToStart();
        mAnm[i + SCORE_ANIM_GET_SCORE_OFFSET].setAnimEnable(true);
    }

    mAnm[SCORE_ANIM_GET_SCORE_GET_TEXT_F].setToStart();
    mAnm[SCORE_ANIM_GET_SCORE_GET_TEXT_F].setAnimEnable(true);
}

void dLytMiniGameScore_c::startFinish() {
    if (sInstance == nullptr) {
        return;
    }

    calcNumDigits();

    for (int i = 0; i <= mHighestDigitIndex; i++) {
        mAnm[i + SCORE_ANIM_FINISH_SCORE_OFFSET].setToStart();
        mAnm[i + SCORE_ANIM_FINISH_SCORE_OFFSET].setAnimEnable(true);
    }

    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_W].setToStart();
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_W].setAnimEnable(true);

    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_X].setToStart();
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_X].setAnimEnable(true);

    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_F].setToStart();
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_F].setAnimEnable(true);
}

void dLytMiniGameScore_c::startLoop() {
    if (sInstance == nullptr) {
        return;
    }

    mAnm[SCORE_ANIM_LOOP_SCORE].setFrame(0.0f);
    mAnm[SCORE_ANIM_LOOP_SCORE].setAnimEnable(true);
}

void dLytMiniGameScore_c::setScore(s32 score) {
    if (sInstance == nullptr) {
        return;
    }
    mScore = score;
}

void dLytMiniGameScore_c::fn_80291BC0() {
    s32 digits[5];

    digits[4] = mScore / 10000;

    s32 v = mScore;
    v %= 10000;
    digits[3] = v / 1000;
    v %= 1000;
    digits[2] = v / 100;
    v %= 100;
    digits[1] = v / 10;
    v %= 10;
    digits[0] = v;

    calcNumDigits();
    realizePosition();

    if (mVariant == SCORE_VARIANT_FUN_FUN_ISLAND) {
        if (field_0x828) {
            fn_80291EF0();
        }
    } else if (mVariant == SCORE_VARIANT_THRILL_DIGGER) {
        fn_80292040();
    }

    fn_80292C30();

    for (int i = 0; i <= mHighestDigitIndex; i++) {
        setDigit(i, digits[i]);
    }
}

void dLytMiniGameScore_c::fn_80291D40(s32 score) {
    if (sInstance == nullptr) {
        return;
    }

    s32 diff = score - mScore;
    switch (diff) {
        case 1:
            field_0x80C = 1;
            field_0x810 = 1;
            break;
        case 5:
            field_0x80C = 5;
            field_0x810 = 1;
            break;
        case 20:
            field_0x80C = 10;
            field_0x810 = 2;
            break;
        case 100:
            field_0x80C = 20;
            field_0x810 = 5;
            break;
        case 300:
            field_0x80C = 25;
            field_0x810 = 12;
            break;
        case 0: return;
    }

    if (score < mScore) {
        field_0x80C = nw4r::math::FAbs(diff);
        field_0x810 = -1;
    }
    field_0x808 = score;
    field_0x804 = true;
    field_0x829 = true;
}

bool dLytMiniGameScore_c::hasIncreasedScore() const {
    if (sInstance == nullptr) {
        return false;
    }
    return mLastScore < mScore;
}

bool dLytMiniGameScore_c::hasDecreasedScore() const {
    if (sInstance == nullptr) {
        return false;
    }
    return mScore < mLastScore;
}

void dLytMiniGameScore_c::fn_80291ED0(s32 arg) {
    if (sInstance == nullptr) {
        return;
    }
    field_0x824 = arg;
}

void dLytMiniGameScore_c::fn_80291EF0() {
    f32 f1 = 0.0f;
    switch (field_0x824) {
        case 0: f1 = 4.0; break;
        case 1: f1 = 0.0; break;
        case 2: f1 = 1.0; break;
        case 3: f1 = 2.0; break;
        case 4: f1 = 5.0; break;
        case 6: f1 = 3.0; break;
    }

    f32 f2 = 0.0f;
    if (field_0x824 == 4) {
        f2 = 2.0;
    } else if (field_0x824 == 0) {
        f2 = 4.0;
    }

    f32 frame = mAnm[SCORE_ANIM_SET_POSITION].getFrame();
    mAnm[SCORE_ANIM_SET_POSITION].setFrame(frame + 2.0f + f2);
    mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);

    mAnm[SCORE_ANIM_RUPEE_CHANGE].setFrame(f1);
    mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(false);
    mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);
}

void dLytMiniGameScore_c::increaseScore(s32 arg) {
    setScore(mScore + arg);
}

void dLytMiniGameScore_c::fn_80292040() {
    mAnm[SCORE_ANIM_RUPEE_CHANGE].setFrame(6.0f);
    mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(false);
}

s32 dLytMiniGameScore_c::calcNumDigits() {
    s32 num;
    if (mScore >= 10000) {
        num = 4;
    } else if (mScore >= 1000) {
        num = 3;
    } else if (mScore >= 100) {
        num = 2;
    } else if (mScore >= 10) {
        num = 1;
    } else {
        num = 0;
    }
    mHighestDigitIndex = num;
    return num;
}

void dLytMiniGameScore_c::realizePosition() {
    mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C + mHighestDigitIndex);
    mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);
}

void dLytMiniGameScore_c::setDigit(s32 digitIndex, s32 number) {
    s32 idx = digitIndex + SCORE_ANIM_0_TO_9_OFFSET;
    f32 frame = number;

    mAnm[idx].setFrame(frame);
    mAnm[idx].setAnimEnable(true);

    mLyt.calc();

    mAnm[idx].setAnimEnable(false);
}

void dLytMiniGameScore_c::saveScore() {
    mLastScore = mScore;
}

void dLytMiniGameScore_c::resetIn() {
    mAnm[SCORE_ANIM_IN].setForwardOnce();
    mAnm[SCORE_ANIM_IN].setToStart();
    mAnm[SCORE_ANIM_IN].setAnimEnable(true);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[SCORE_ANIM_IN].setAnimEnable(false);
}

void dLytMiniGameScore_c::resetDigits() {
    // NOTE: includes SCORE_ANIM_SET_POSITION
    for (int i = 0; i <= SCORE_NUM_DIGITS; i++) {
        s32 idx = i + SCORE_ANIM_0_TO_9_OFFSET;
        mAnm[idx].setForwardOnce();
        mAnm[idx].setFrame(0.0f);
        mAnm[idx].setAnimEnable(false);
    }
}

void dLytMiniGameScore_c::resetOut() {
    mAnm[SCORE_ANIM_OUT].setForwardOnce();
    mAnm[SCORE_ANIM_OUT].setToStart();
    mAnm[SCORE_ANIM_OUT].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_OUT].setAnimEnable(false);
}

void dLytMiniGameScore_c::resetAlphaIn() {
    mAnm[SCORE_ANIM_ALPHA_IN].setForwardOnce();
    mAnm[SCORE_ANIM_ALPHA_IN].setToStart();
    mAnm[SCORE_ANIM_ALPHA_IN].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_ALPHA_IN].setAnimEnable(false);
}

void dLytMiniGameScore_c::resetAlphaOut() {
    mAnm[SCORE_ANIM_ALPHA_OUT].setForwardOnce();
    mAnm[SCORE_ANIM_ALPHA_OUT].setToStart();
    mAnm[SCORE_ANIM_ALPHA_OUT].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_ALPHA_OUT].setAnimEnable(false);
}

void dLytMiniGameScore_c::resetSetPosition() {
    mAnm[SCORE_ANIM_SET_POSITION].setForwardOnce();
    mAnm[SCORE_ANIM_SET_POSITION].setFrame(field_0x81C);
    mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_SET_POSITION].setAnimEnable(false);
}

void dLytMiniGameScore_c::resetRupeeChange() {
    mAnm[SCORE_ANIM_RUPEE_CHANGE].setForwardOnce();
    mAnm[SCORE_ANIM_RUPEE_CHANGE].setToEnd2();
    mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_RUPEE_CHANGE].setAnimEnable(false);
}

void dLytMiniGameScore_c::resetGetScore() {
    for (int i = 0; i < SCORE_NUM_DIGITS; i++) {
        mAnm[i + SCORE_ANIM_GET_SCORE_OFFSET].setForwardOnce();
        mAnm[i + SCORE_ANIM_GET_SCORE_OFFSET].setToStart();
        mAnm[i + SCORE_ANIM_GET_SCORE_OFFSET].setAnimEnable(true);
    }

    mAnm[SCORE_ANIM_GET_SCORE_GET_TEXT_F].setForwardOnce();
    mAnm[SCORE_ANIM_GET_SCORE_GET_TEXT_F].setToStart();
    mAnm[SCORE_ANIM_GET_SCORE_GET_TEXT_F].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_GET_SCORE_GET_TEXT_F].setAnimEnable(false);

    for (int i = 0; i < SCORE_NUM_DIGITS; i++) {
        mAnm[i + SCORE_ANIM_GET_SCORE_OFFSET].setAnimEnable(false);
    }
}

void dLytMiniGameScore_c::resetFinish() {
    for (int i = 0; i < SCORE_NUM_DIGITS; i++) {
        mAnm[i + SCORE_ANIM_FINISH_SCORE_OFFSET].setForwardOnce();
        mAnm[i + SCORE_ANIM_FINISH_SCORE_OFFSET].setToStart();
        mAnm[i + SCORE_ANIM_FINISH_SCORE_OFFSET].setAnimEnable(true);
    }

    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_W].setForwardOnce();
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_W].setToStart();
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_W].setAnimEnable(true);

    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_X].setForwardOnce();
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_X].setToStart();
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_X].setAnimEnable(true);

    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_F].setForwardOnce();
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_F].setToStart();
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_F].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_F].setAnimEnable(false);
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_X].setAnimEnable(false);
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_W].setAnimEnable(false);

    for (int i = 0; i < SCORE_NUM_DIGITS; i++) {
        mAnm[i + SCORE_ANIM_FINISH_SCORE_OFFSET].setAnimEnable(false);
    }
}

void dLytMiniGameScore_c::resetLoop() {
    mAnm[SCORE_ANIM_LOOP_SCORE].setForwardLoop();
    mAnm[SCORE_ANIM_LOOP_SCORE].setFrame(0.0f);
    mAnm[SCORE_ANIM_LOOP_SCORE].setAnimEnable(true);

    mLyt.getLayout()->Animate(0);
    mLyt.calc();

    mAnm[SCORE_ANIM_LOOP_SCORE].setAnimEnable(false);
}

void dLytMiniGameScore_c::stopIn() {
    mAnm[SCORE_ANIM_IN].setAnimEnable(false);
}

void dLytMiniGameScore_c::stopOut() {
    mAnm[SCORE_ANIM_OUT].setAnimEnable(false);
}

void dLytMiniGameScore_c::stopAlphaIn() {
    mAnm[SCORE_ANIM_ALPHA_IN].setAnimEnable(false);
}

void dLytMiniGameScore_c::stopAlphaOut() {
    mAnm[SCORE_ANIM_ALPHA_OUT].setAnimEnable(false);
}

void dLytMiniGameScore_c::stopGetScore() {
    for (int i = 0; i < SCORE_NUM_DIGITS; i++) {
        mAnm[i + SCORE_ANIM_GET_SCORE_OFFSET].setAnimEnable(false);
    }
    mAnm[SCORE_ANIM_GET_SCORE_GET_TEXT_F].setAnimEnable(false);
}

void dLytMiniGameScore_c::stopFinish() {
    for (int i = 0; i < SCORE_NUM_DIGITS; i++) {
        mAnm[i + SCORE_ANIM_FINISH_SCORE_OFFSET].setAnimEnable(false);
    }

    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_W].setAnimEnable(false);
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_X].setAnimEnable(false);
    mAnm[SCORE_ANIM_FINISH_SCORE_GET_SCORE_F].setAnimEnable(false);
}

void dLytMiniGameScore_c::stopLoop() {
    mAnm[SCORE_ANIM_LOOP_SCORE].setAnimEnable(false);
}

bool dLytMiniGameScore_c::isInEndReached() const {
    if (mAnm[SCORE_ANIM_IN].isEnabled() && mAnm[SCORE_ANIM_IN].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameScore_c::isOutEndReached() const {
    if (mAnm[SCORE_ANIM_OUT].isEnabled() && mAnm[SCORE_ANIM_OUT].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameScore_c::isAlphaInEndReached() const {
    if (mAnm[SCORE_ANIM_ALPHA_IN].isEnabled() && mAnm[SCORE_ANIM_ALPHA_IN].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameScore_c::isAlphaOutEndReached() const {
    if (mAnm[SCORE_ANIM_ALPHA_OUT].isEnabled() && mAnm[SCORE_ANIM_ALPHA_OUT].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameScore_c::isGetScoreEndReached() const {
    if (mAnm[SCORE_ANIM_GET_SCORE_OFFSET].isEnabled() && mAnm[SCORE_ANIM_GET_SCORE_OFFSET].isEndReached()) {
        return true;
    }
    return false;
}

bool dLytMiniGameScore_c::isFinishEndReached() const {
    if (mAnm[SCORE_ANIM_FINISH_SCORE_OFFSET].isEnabled() && mAnm[SCORE_ANIM_FINISH_SCORE_OFFSET].isEndReached()) {
        return true;
    }
    return false;
}

void dLytMiniGameScore_c::fn_80292C30() {
    s32 t = 0;
    if (field_0x828) {
        t = field_0x824 == 4 ? 4 : field_0x824 == 0 ? 6 : 0;
    }

    PaneCalc calc;

    switch (mVariant) {
        case SCORE_VARIANT_BAMBOO_CUTTING: {
            dMessage_c::getGlobalTagProcessor()->setNumericArg0(mScore);
            loadTextVariant(1);
            field_0x7E8 = mpTextBoxes[0]->GetLineWidth(nullptr);

            // TODO
            calc.calc(mpPanes[mHighestDigitIndex + 15], mpPanes[15]);
            mFloats[field_0x81C + mHighestDigitIndex] =
                calc.work[3][0] / 2.0f + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
            mFloats[field_0x81C + mHighestDigitIndex] += field_0x7FC;
            mFloats[field_0x81C + mHighestDigitIndex] += field_0x800;

            break;
        }
        case SCORE_VARIANT_PUMPKIN_ARCHERY: {
            dMessage_c::getGlobalTagProcessor()->setNumericArg0(mScore);
            loadTextVariant(0);
            field_0x7E8 = mpTextBoxes[0]->GetLineWidth(nullptr);

            // TODO
            calc.calc(mpPanes[mHighestDigitIndex + 15], mpPanes[15]);
            mFloats[field_0x81C + mHighestDigitIndex] =
                calc.work[3][0] / 2.0f + calc.work[4][0] / 2.0f + (calc.work[1][0] - calc.work[2][0]);
            mFloats[field_0x81C + mHighestDigitIndex] += field_0x7FC;
            mFloats[field_0x81C + mHighestDigitIndex] += field_0x800;

            break;
        }
    }

    f32 f = mFloats[mHighestDigitIndex + t + field_0x81C];

    if (field_0x830 != 0) {
        nw4r::lyt::Size size = mpTextBoxes[0]->GetSize();
        switch (field_0x830) {
            case 1: {
                mVec3_c pos(
                    f / 2.0f + size.width / 2.0f - field_0x7E8 + 0.0f + (f32)field_0x7F4 - (f32)field_0x800,
                    field_0x834[1][1] + (f32)field_0x7EC + (f32)field_0x7F8, 0.0f
                );
                mpPanes[SCORE_PANE_UNIT]->SetTranslate(pos);
                break;
            }
            case 2: {
                mVec3_c pos(
                    (f32)field_0x7E8 - f / 2.0f - size.width / 2.0f + 0.0 + (f32)field_0x7F4 + (f32)field_0x800,
                    field_0x834[1][1] + (f32)field_0x7EC + (f32)field_0x7F8, 0.0f
                );
                mpPanes[SCORE_PANE_UNIT]->SetTranslate(pos);
                break;
            }
        }
    }

    // TODO - this uses similar annoying calculations and stack as above but
    // I can't be bothered right now. Let's at least give this code
    // a chance to be equivalent, even if it's not matching...

    nw4r::lyt::Pane *p = mpPanes[SCORE_PANE_P_POSITION_0];
    f32 fVar2 = p->GetSize().width * p->GetScale().x;
    f32 fVar1 = field_0x834[0][1];
    f32 fVar3 = p->GetParent()->GetTranslate().x + p->GetTranslate().x;
    f32 local_e0 = 0.0;
    if (field_0x830 == 0) {
        local_e0 = ((-fVar3 + f * 0.5) - fVar2 * 0.5) + 0.0;
    } else if (field_0x830 == 1) {
        local_e0 = (((((-fVar3 + f * 0.5) - fVar2 * 0.5) - field_0x7E8) + 0.0) - (f32)field_0x7FC) - (f32)field_0x804;
    } else if (field_0x830 == 2) {
        local_e0 = ((-fVar3 + f * 0.5) - fVar2 * 0.5) + 0.0;
    }
    mVec3_c translate(local_e0 + (f32)field_0x7F4, fVar1 + field_0x7F8, 0.0f);
    mpPanes[SCORE_PANE_POSITION]->SetTranslate(translate);

    fn_80293450(mHighestDigitIndex + t + field_0x81C);
    nw4r::math::MTX34 mtx = mpPanes[SCORE_PANE_CENTER]->GetGlobalMtx();
    mVec3_c pos(mtx._03, mtx._13, 0.0f);
    mpPanes[SCORE_PANE_POSITION_ALL]->SetTranslate(pos);
}

void dLytMiniGameScore_c::loadTextVariant(s32 variant) {
    for (int i = 0; i < SCORE_NUM_TEXTBOXES; i++) {
        mLyt.loadTextVariant(mpTextBoxes[i], variant);
    }
}

void dLytMiniGameScore_c::fn_80293410() {
    (void)getCurrentLanguage1();
    field_0x830 = 1;
}

void dLytMiniGameScore_c::fn_80293450(s32 idx) {
    // TODO - aaaaaaaaaaaaa
    f32 tmp = field_0x7F0 + mFloats[idx];

    mpPanes[11]->SetSize(nw4r::lyt::Size(tmp, mpPanes[11]->GetSize().height));
    mpPanes[11]->SetTranslate(mVec3_c(0.0f, 0.0f, 0.0f));

    tmp /= 2.0f;

    mpPanes[9]->SetTranslate(mVec3_c(-tmp - mpPanes[9]->GetSize().width, 0.0f, 0.0f));
    mpPanes[10]->SetTranslate(mVec3_c(tmp + mpPanes[10]->GetSize().width, 0.0f, 0.0f));

    tmp = field_0x7F0 + mFloats[idx];

    mpPanes[8]->SetSize(nw4r::lyt::Size(tmp, mpPanes[8]->GetSize().height));
    mpPanes[8]->SetTranslate(mVec3_c(-2.0f, -2.0f, 0.0f));

    tmp /= 2.0f;

    mpPanes[6]->SetTranslate(mVec3_c(-tmp - mpPanes[6]->GetSize().width - 2.0f, -2.0f, 0.0f));
    mpPanes[7]->SetTranslate(mVec3_c(tmp + mpPanes[7]->GetSize().width - 2.0f, -2.0f, 0.0f));

    mpPanes[5]->SetSize(nw4r::lyt::Size(
        mpPanes[10]->GetSize().width + mpPanes[11]->GetSize().width + mpPanes[9]->GetSize().width - 36.0f,
        mpPanes[5]->GetSize().height
    ));

    mpPanes[5]->SetTranslate(mVec3_c(0.0f, 0.0f, 0.0f));
    mpPanes[4]->SetTranslate(mVec3_c(field_0x834[2][0], field_0x834[2][1], 0.0f));
}

static const d2d::LytBrlanMapping brlanMapScoreSd[] = {
    {         "miniGameScoreSd_00_0to9.brlan",        "G_0to9_00"},
    {         "miniGameScoreSd_00_0to9.brlan",        "G_0to9_01"},
    {"miniGameScoreSd_00_setPosition00.brlan", "G_setPosition_00"},
    {      "miniGameScoreSd_00_scoreUp.brlan",     "G_animAll_00"},
    {    "miniGameScoreSd_00_scoreDown.brlan",     "G_animAll_00"},
};

#define SCORE_SD_ANIM_0_TO_9_OFFSET 0
#define SCORE_SD_ANIM_SET_POSITION 2
#define SCORE_SD_ANIM_SCORE_UP 3
#define SCORE_SD_ANIM_SCORE_DOWN 4

#define SCORE_SD_NUM_ANIMS 5

#define SCORE_SD_NUM_DIGITS 2

bool dLytMiniGameScoreSd_c::build(d2d::ResAccIf_c *resAcc) {
    mLyt.setResAcc(resAcc);
    mLyt.build("miniGameScoreSd_00.brlyt", nullptr);

    for (int i = 0; i < SCORE_SD_NUM_ANIMS; i++) {
        mAnm[i].init(brlanMapScoreSd[i].mFile, resAcc, mLyt.getLayout(), brlanMapScoreSd[i].mName);
        mAnm[i].bind(false);
        mAnm[i].setRate(1.0f);
        mAnm[i].setAnimEnable(false);
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

    if (mIsVisible) {
        if (isScoreUpEndReached()) {
            stopScoreUp();
            mIsVisible = false;
        }

        if (isScoreDownEndReached()) {
            stopScoreDown();
            mIsVisible = false;
        }

        for (int i = 0; i < SCORE_SD_NUM_ANIMS; i++) {
            if (mAnm[i].isEnabled()) {
                mAnm[i].play();
            }
        }

        mLyt.calc();
    }

    return true;
}

bool dLytMiniGameScoreSd_c::draw() {
    if (sInstance == nullptr) {
        return true;
    }
    if (mIsVisible) {
        mLyt.addToDrawList();
    }
    return true;
}

void dLytMiniGameScoreSd_c::init() {
    if (sInstance == nullptr) {
        return;
    }

    resetScoreUp();
    resetScoreDown();
    resetDigits();
    resetPosition();
    setScoreInternal(0);

    mAnm[SCORE_SD_ANIM_SCORE_UP].setToEnd2();
    mAnm[SCORE_SD_ANIM_SCORE_UP].setAnimEnable(true);
    mLyt.calc();
    mAnm[SCORE_SD_ANIM_SCORE_UP].setAnimEnable(false);
}

void dLytMiniGameScoreSd_c::setScoreUp(s32 score) {
    if (sInstance == nullptr) {
        return;
    }
    setScoreInternal(score);
    startScoreUp();
    dSndSmallEffectMgr_c::GetInstance()->playMinigameScoreUpSound(score);
    mIsVisible = true;
}

void dLytMiniGameScoreSd_c::setScoreDown(s32 score) {
    if (sInstance == nullptr) {
        return;
    }
    setScoreInternal(score);
    startScoreDown();
    dSndSmallEffectMgr_c::GetInstance()->playMinigameScoreDownSound(score);
    mIsVisible = true;
}

void dLytMiniGameScoreSd_c::setScoreInternal(s32 score) {
    s32 digits[2];

    digits[1] = score / 10;
    digits[0] = score % 10;

    s32 num = 0;
    if (digits[1] != 0) {
        num = 1;
    } else if (digits[0] != 0) {
        num = 0;
    }
    setHighestDigitIndex(num);
    for (int i = 0; i <= num; i++) {
        setDigit(i, digits[i]);
    }
    field_0x1D4 = score;
}

void dLytMiniGameScoreSd_c::setHighestDigitIndex(s32 num) {
    f32 frame = num + 6.0f;

    mAnm[SCORE_SD_ANIM_SET_POSITION].setRate(0.0f);
    mAnm[SCORE_SD_ANIM_SET_POSITION].setFrame(frame);
    mAnm[SCORE_SD_ANIM_SET_POSITION].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_SD_ANIM_SET_POSITION].setAnimEnable(false);
}

void dLytMiniGameScoreSd_c::setDigit(s32 digitIndex, s32 number) {
    u8 idx = digitIndex + SCORE_SD_ANIM_0_TO_9_OFFSET;
    f32 frame = 0.0f;
    switch (number) {
        case 0: frame = 0.0f; break;
        case 1: frame = 1.0f; break;
        case 2: frame = 2.0f; break;
        case 4: frame = 3.0f; break;
        case 6: frame = 4.0f; break;
        case 8: frame = 5.0f; break;
    }

    mAnm[idx].setFrame(frame);
    mAnm[idx].setAnimEnable(true);

    mLyt.calc();

    mAnm[idx].setAnimEnable(false);
}

void dLytMiniGameScoreSd_c::resetScoreUp() {
    mAnm[SCORE_SD_ANIM_SCORE_UP].setForwardOnce();
    mAnm[SCORE_SD_ANIM_SCORE_UP].setToEnd2();
    mAnm[SCORE_SD_ANIM_SCORE_UP].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_SD_ANIM_SCORE_UP].setAnimEnable(false);
}

void dLytMiniGameScoreSd_c::resetScoreDown() {
    mAnm[SCORE_SD_ANIM_SCORE_DOWN].setForwardOnce();
    mAnm[SCORE_SD_ANIM_SCORE_DOWN].setToEnd2();
    mAnm[SCORE_SD_ANIM_SCORE_DOWN].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_SD_ANIM_SCORE_DOWN].setAnimEnable(false);
}

void dLytMiniGameScoreSd_c::resetDigits() {
    // NOTE: includes SCORE_SD_ANIM_SET_POSITION
    for (int i = 0; i <= SCORE_SD_NUM_DIGITS; i++) {
        mAnm[i + SCORE_SD_ANIM_0_TO_9_OFFSET].setForwardOnce();
        mAnm[i + SCORE_SD_ANIM_0_TO_9_OFFSET].setFrame(0.0f);
        mAnm[i + SCORE_SD_ANIM_0_TO_9_OFFSET].setAnimEnable(false);
    }
}

void dLytMiniGameScoreSd_c::resetPosition() {
    mAnm[SCORE_SD_ANIM_SET_POSITION].setForwardOnce();
    mAnm[SCORE_SD_ANIM_SET_POSITION].setFrame(6.0f);
    mAnm[SCORE_SD_ANIM_SET_POSITION].setAnimEnable(true);

    mLyt.calc();

    mAnm[SCORE_SD_ANIM_SET_POSITION].setAnimEnable(false);
}

void dLytMiniGameScoreSd_c::startScoreUp() {
    mAnm[SCORE_SD_ANIM_SCORE_UP].setToStart();
    mAnm[SCORE_SD_ANIM_SCORE_UP].setAnimEnable(true);
}

void dLytMiniGameScoreSd_c::startScoreDown() {
    mAnm[SCORE_SD_ANIM_SCORE_DOWN].setToStart();
    mAnm[SCORE_SD_ANIM_SCORE_DOWN].setAnimEnable(true);
}

void dLytMiniGameScoreSd_c::stopScoreUp() {
    mAnm[SCORE_SD_ANIM_SCORE_UP].setAnimEnable(false);
}

void dLytMiniGameScoreSd_c::stopScoreDown() {
    mAnm[SCORE_SD_ANIM_SCORE_DOWN].setAnimEnable(false);
}

bool dLytMiniGameScoreSd_c::isScoreUpEndReached() const {
    s32 idx = SCORE_SD_ANIM_SCORE_UP;
    if (mAnm[idx].isEnabled() && mAnm[idx].isStop2()) {
        return true;
    }
    return false;
}

bool dLytMiniGameScoreSd_c::isScoreDownEndReached() const {
    s32 idx = SCORE_SD_ANIM_SCORE_DOWN;
    if (mAnm[idx].isEnabled() && mAnm[idx].isStop2()) {
        return true;
    }
    return false;
}

SPECIAL_BASE_PROFILE(LYT_MINI_GAME, dLytMiniGame_c, fProfile::LYT_MINI_GAME, 0x2B4, 0x6F9);

static const char *sMiniGame = "MiniGame";
static const char *sMiniGameScore = "MiniGameScore";
static const char *sMiniGameTime = "MiniGameTime";
static const char *sMiniGameBugs = "MiniGameBugs";
static const char *sMiniGamePumpkin = "MiniGamePumpkin";
static const char *sMiniGameSd = "MiniGameSd";

int dLytMiniGame_c::create() {
    mVariant = mParams;
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING: {
            if (loadData(sMiniGame, SLOT_MINI_GAME) || loadData(sMiniGameScore, SLOT_MINI_GAME_SCORE)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGame) || isLoading(sMiniGameScore)) {
                return NOT_READY;
            }

            attachLoadedData(sMiniGame, mResAccStart);
            attachLoadedData(sMiniGameScore, mResAccScore);
            mScore.setVariant(dLytMiniGameScore_c::SCORE_VARIANT_BAMBOO_CUTTING);
            mScore.build(&mResAccScore);
            mStart.build(&mResAccStart);
            break;
        }
        case VARIANT_FUN_FUN_ISLAND: {
            if (loadData(sMiniGame, SLOT_MINI_GAME) || loadData(sMiniGameSd, SLOT_MINI_GAME_SCORE_SD) ||
                loadData(sMiniGameScore, SLOT_MINI_GAME_SCORE)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGame) || isLoading(sMiniGameSd) || isLoading(sMiniGameScore)) {
                return NOT_READY;
            }

            attachLoadedData(sMiniGame, mResAccStart);
            attachLoadedData(sMiniGameSd, mResAccScoreSd);
            attachLoadedData(sMiniGameScore, mResAccScore);
            mScore.setVariant(dLytMiniGameScore_c::SCORE_VARIANT_FUN_FUN_ISLAND);
            mScore.build(&mResAccScore);
            mScoreSd.build(&mResAccScoreSd);
            mStart.build(&mResAccStart);
            break;
        }
        case VARIANT_THRILL_DIGGER: {
            if (loadData(sMiniGame, SLOT_MINI_GAME) || loadData(sMiniGameScore, SLOT_MINI_GAME_SCORE)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGame) || isLoading(sMiniGameScore)) {
                return NOT_READY;
            }

            attachLoadedData(sMiniGame, mResAccStart);
            attachLoadedData(sMiniGameScore, mResAccScore);
            mScore.setVariant(dLytMiniGameScore_c::SCORE_VARIANT_THRILL_DIGGER);
            mScore.build(&mResAccScore);
            mStart.build(&mResAccStart);
            break;
        }
        case VARIANT_INSECT_CAPTURE: {
            if (loadData(sMiniGame, SLOT_MINI_GAME) || loadData(sMiniGameBugs, SLOT_MINI_GAME_BUGS) ||
                loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGame) || isLoading(sMiniGameBugs) || isLoading(sMiniGameTime)) {
                return NOT_READY;
            }

            attachLoadedData(sMiniGame, mResAccStart);
            attachLoadedData(sMiniGameBugs, mResAccBugs);
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_INSECT_CAPTURE);
            mTime.build(&mResAccTime);
            mBugs.build(&mResAccBugs);
            mStart.build(&mResAccStart);
            break;
        }
        case VARIANT_SPIRAL_CHARGE_TUTORIAL: {
            if (loadData(sMiniGame, SLOT_MINI_GAME) || loadData(sMiniGameScore, SLOT_MINI_GAME_SCORE) ||
                loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGame) || isLoading(sMiniGameScore) || isLoading(sMiniGameTime)) {
                return NOT_READY;
            }

            attachLoadedData(sMiniGame, mResAccStart);
            attachLoadedData(sMiniGameScore, mResAccScore);
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_SPIRAL_CHARGE_TUTORIAL);
            mTime.build(&mResAccTime);
            mScore.setVariant(dLytMiniGameScore_c::SCORE_VARIANT_SPIRAL_CHARGE_TUTORIAL);
            mScore.build(&mResAccScore);
            mStart.build(&mResAccStart);
            break;
        }
        case VARIANT_PUMPKIN_ARCHERY: {
            if (loadData(sMiniGame, SLOT_MINI_GAME) || loadData(sMiniGamePumpkin, SLOT_MINI_GAME_PUMPKIN) ||
                loadData(sMiniGameScore, SLOT_MINI_GAME_SCORE) || loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGame) || isLoading(sMiniGamePumpkin) || isLoading(sMiniGameScore) ||
                isLoading(sMiniGameTime)) {
                return NOT_READY;
            }

            attachLoadedData(sMiniGame, mResAccStart);
            attachLoadedData(sMiniGamePumpkin, mResAccPumpkin);
            attachLoadedData(sMiniGameScore, mResAccScore);
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_PUMPKIN_ARCHERY);
            mTime.build(&mResAccTime);
            mScore.setVariant(dLytMiniGameScore_c::SCORE_VARIANT_PUMPKIN_ARCHERY);
            mScore.build(&mResAccScore);
            mStart.build(&mResAccStart);
            mPumpkin.build(&mResAccPumpkin);
            break;
        }
        case VARIANT_ROLLERCOASTER: {
            if (loadData(sMiniGame, SLOT_MINI_GAME) || loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGame) || isLoading(sMiniGameTime)) {
                return NOT_READY;
            }

            attachLoadedData(sMiniGame, mResAccStart);
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_ROLLERCOASTER);
            mTime.build(&mResAccTime);
            mStart.build(&mResAccStart);
            break;
        }
        case VARIANT_BOSS_RUSH: {
            if (loadData(sMiniGame, SLOT_MINI_GAME) || loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGame) || isLoading(sMiniGameTime)) {
                return NOT_READY;
            }

            attachLoadedData(sMiniGame, mResAccStart);
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_BOSS_RUSH);
            mTime.build(&mResAccTime);
            mStart.build(&mResAccStart);
            break;
        }
        case VARIANT_TRIAL_TIME_ATTACK: {
            if (loadData(sMiniGame, SLOT_MINI_GAME) || loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGame) || isLoading(sMiniGameTime)) {
                return NOT_READY;
            }

            attachLoadedData(sMiniGame, mResAccStart);
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_TRIAL_TIME_ATTACK);
            mTime.build(&mResAccTime);
            mStart.build(&mResAccStart);
            break;
        }
        case VARIANT_10: {
            if (loadData(sMiniGameScore, SLOT_MINI_GAME_SCORE)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGameScore)) {
                return NOT_READY;
            }
            attachLoadedData(sMiniGameScore, mResAccScore);
            mScore.setVariant(dLytMiniGameScore_c::SCORE_VARIANT_BAMBOO_CUTTING);
            mScore.setField_0x816(true);
            mScore.build(&mResAccScore);
            break;
        }
        case VARIANT_11: {
            if (loadData(sMiniGameScore, SLOT_MINI_GAME_SCORE)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGameScore)) {
                return NOT_READY;
            }
            attachLoadedData(sMiniGameScore, mResAccScore);
            mScore.setVariant(dLytMiniGameScore_c::SCORE_VARIANT_THRILL_DIGGER);
            mScore.setField_0x816(true);
            mScore.build(&mResAccScore);
            break;
        }
        case VARIANT_12: {
            if (loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGameTime)) {
                return NOT_READY;
            }
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_INSECT_CAPTURE);
            mTime.setField_0x7EE(true);
            mTime.build(&mResAccTime);
            break;
        }
        case VARIANT_13: {
            if (loadData(sMiniGameScore, SLOT_MINI_GAME_SCORE)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGameScore)) {
                return NOT_READY;
            }
            attachLoadedData(sMiniGameScore, mResAccScore);
            mScore.setVariant(dLytMiniGameScore_c::SCORE_VARIANT_PUMPKIN_ARCHERY);
            mScore.setField_0x816(true);
            mScore.build(&mResAccScore);
            break;
        }
        case VARIANT_14: {
            if (loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGameTime)) {
                return NOT_READY;
            }
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_ROLLERCOASTER);
            mTime.setField_0x7EE(true);
            mTime.build(&mResAccTime);
            break;
        }
        case VARIANT_15: {
            if (loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGameTime)) {
                return NOT_READY;
            }
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_BOSS_RUSH);
            mTime.setField_0x7EE(true);
            mTime.build(&mResAccTime);
            break;
        }
        case VARIANT_16: {
            if (loadData(sMiniGameTime, SLOT_MINI_GAME_TIME)) {
                return NOT_READY;
            }
            if (isLoading(sMiniGameTime)) {
                return NOT_READY;
            }
            attachLoadedData(sMiniGameTime, mResAccTime);
            mTime.setVariant(dLytMiniGameTime_c::TIME_VARIANT_TRIAL_TIME_ATTACK);
            mTime.setField_0x7EE(true);
            mTime.build(&mResAccTime);
            break;
        }
    }

    init();

    sInstance = this;
    return SUCCEEDED;
}

int dLytMiniGame_c::doDelete() {
    mScore.remove();
    mScoreSd.remove();
    mStart.remove();
    mTime.remove();
    mBugs.remove();
    mPumpkin.remove();

    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING: {
            mResAccStart.detach();
            mResAccScore.detach();
            unloadData(sMiniGame);
            unloadData(sMiniGameScore);
            break;
        }
        case VARIANT_FUN_FUN_ISLAND: {
            mResAccStart.detach();
            mResAccScoreSd.detach();
            mResAccScore.detach();
            unloadData(sMiniGame);
            unloadData(sMiniGameSd);
            unloadData(sMiniGameScore);
            break;
        }
        case VARIANT_THRILL_DIGGER: {
            mResAccStart.detach();
            mResAccScore.detach();
            unloadData(sMiniGame);
            unloadData(sMiniGameScore);
            break;
        }
        case VARIANT_INSECT_CAPTURE: {
            mResAccStart.detach();
            mResAccBugs.detach();
            mResAccTime.detach();
            unloadData(sMiniGame);
            unloadData(sMiniGameBugs);
            unloadData(sMiniGameTime);
            break;
        }
        case VARIANT_SPIRAL_CHARGE_TUTORIAL: {
            mResAccStart.detach();
            mResAccScore.detach();
            mResAccTime.detach();
            unloadData(sMiniGame);
            unloadData(sMiniGameScore);
            unloadData(sMiniGameTime);
            break;
        }
        case VARIANT_PUMPKIN_ARCHERY: {
            mResAccStart.detach();
            mResAccPumpkin.detach();
            mResAccScore.detach();
            mResAccTime.detach();
            unloadData(sMiniGame);
            unloadData(sMiniGamePumpkin);
            unloadData(sMiniGameScore);
            unloadData(sMiniGameTime);
            break;
        }
        case VARIANT_ROLLERCOASTER: {
            mResAccStart.detach();
            mResAccTime.detach();
            unloadData(sMiniGame);
            unloadData(sMiniGameTime);
            break;
        }
        case VARIANT_BOSS_RUSH: {
            mResAccStart.detach();
            mResAccTime.detach();
            unloadData(sMiniGame);
            unloadData(sMiniGameTime);
            break;
        }
        case VARIANT_TRIAL_TIME_ATTACK: {
            mResAccStart.detach();
            mResAccTime.detach();
            unloadData(sMiniGame);
            unloadData(sMiniGameTime);
            break;
        }
        case VARIANT_10: {
            mResAccScore.detach();
            unloadData(sMiniGameScore);
            break;
        }
        case VARIANT_11: {
            mResAccScore.detach();
            unloadData(sMiniGameScore);
            break;
        }
        case VARIANT_12: {
            mResAccTime.detach();
            unloadData(sMiniGameTime);
            break;
        }
        case VARIANT_13: {
            mResAccScore.detach();
            unloadData(sMiniGameScore);
            break;
        }
        case VARIANT_14: {
            mResAccTime.detach();
            unloadData(sMiniGameTime);
            break;
        }
        case VARIANT_15: {
            mResAccTime.detach();
            unloadData(sMiniGameTime);
            break;
        }
        case VARIANT_16: {
            mResAccTime.detach();
            unloadData(sMiniGameTime);
            break;
        }
    }

    sInstance = nullptr;
    return SUCCEEDED;
}

int dLytMiniGame_c::execute() {
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING:
        case VARIANT_10:                     fn_80295F90(); break;
        case VARIANT_FUN_FUN_ISLAND:         fn_80296000(); break;
        case VARIANT_THRILL_DIGGER:
        case VARIANT_11:                     fn_80296070(); break;
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_12:                     fn_80296150(); break;
        case VARIANT_SPIRAL_CHARGE_TUTORIAL: fn_80296210(); break;
        case VARIANT_PUMPKIN_ARCHERY:
        case VARIANT_13:                     fn_802962A0(); break;
        case VARIANT_ROLLERCOASTER:
        case VARIANT_14:                     fn_80296330(); break;
        case VARIANT_BOSS_RUSH:
        case VARIANT_15:                     fn_802963E0(); break;
        case VARIANT_TRIAL_TIME_ATTACK:
        case VARIANT_16:                     fn_80296480(); break;
    }

    mScore.execute();
    mScoreSd.execute();
    mTime.execute();
    mBugs.execute();
    mPumpkin.execute();

    if (!isPopupOpenExceptHelp()) {
        mStart.execute();
    }

    return SUCCEEDED;
}

int dLytMiniGame_c::draw() {
    mPumpkin.draw();
    mScoreSd.draw();
    mBugs.draw();
    mScore.draw();
    mTime.draw();

    if (!isPopupOpenExceptHelp()) {
        mStart.draw();
    }

    return SUCCEEDED;
}

void dLytMiniGame_c::scoreRelatedExecute() {
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING:
        case VARIANT_THRILL_DIGGER:
        case VARIANT_SPIRAL_CHARGE_TUTORIAL:
        case VARIANT_PUMPKIN_ARCHERY:        mScore.fn_80291410(); break;
        case VARIANT_FUN_FUN_ISLAND:
            if (fn_80295DB0()) {
                mScore.startAlphaIn();
            } else {
                mScore.fn_80291410();
                mScore.setField_0x828(false);
            }
            break;
        case VARIANT_10:
        case VARIANT_11:
        case VARIANT_13: mScore.startAlphaIn();
    }
}

void dLytMiniGame_c::scoreRelated() {
    switch (mVariant) {
        case VARIANT_FUN_FUN_ISLAND:
            if (fn_80295DB0()) {
                mScore.startAlphaOut();
            } else {
                mScore.startOut();
            }
            break;
        case VARIANT_10:
        case VARIANT_11:
        case VARIANT_13: mScore.startAlphaOut(); break;
    }
}

void dLytMiniGame_c::setDisplayedPoints(s32 points) {
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING:
        case VARIANT_SPIRAL_CHARGE_TUTORIAL:
        case VARIANT_PUMPKIN_ARCHERY:        mScore.setScore(points); break;
        case VARIANT_FUN_FUN_ISLAND:
            fn_80295DB0();
            mScore.setScore(points);
            break;
        case VARIANT_THRILL_DIGGER: mScore.fn_80291D40(points); break;
        case VARIANT_10:
        case VARIANT_13:            mScore.setScore(points); break;
        case VARIANT_11:
            mScore.setScore(points);
            field_0x3864 = 1;
            field_0x3865 = 0;
            break;
    }
}

void dLytMiniGame_c::fn_80295900(s32 arg) {
    mScore.fn_80291ED0(arg);
    mScore.setField_0x828(true);
}

void dLytMiniGame_c::startCountdown() {
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING:
        case VARIANT_FUN_FUN_ISLAND:
        case VARIANT_THRILL_DIGGER:
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_SPIRAL_CHARGE_TUTORIAL:
        case VARIANT_PUMPKIN_ARCHERY:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_TRIAL_TIME_ATTACK:      mStart.startCountdown(); break;
        case VARIANT_BOSS_RUSH:              mStart.startCountdown120(); break;
    }
}

void dLytMiniGame_c::startFinish() {
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING:
        case VARIANT_FUN_FUN_ISLAND:
        case VARIANT_THRILL_DIGGER:
        case VARIANT_PUMPKIN_ARCHERY:
            mStart.startFinish();
            mScore.startFinish();
            break;
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_BOSS_RUSH:
        case VARIANT_TRIAL_TIME_ATTACK:
            mStart.startFinish();
            mTime.startFinish();
            break;
        case VARIANT_SPIRAL_CHARGE_TUTORIAL:
            mStart.startFinish();
            mScore.startFinish();
            break;
    }
}

void dLytMiniGame_c::fn_80295A20() {
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING:
        case VARIANT_FUN_FUN_ISLAND:
        case VARIANT_THRILL_DIGGER:
            // case VARIANT_PUMPKIN_ARCHERY:
            mStart.startTimeup();
            mScore.startFinish();
            break;
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_BOSS_RUSH:
        case VARIANT_TRIAL_TIME_ATTACK:
            mStart.startTimeup();
            mTime.startFinish();
            break;
        case VARIANT_SPIRAL_CHARGE_TUTORIAL:
            mStart.startTimeup();
            mScore.startFinish();
            break;
    }
}

bool dLytMiniGame_c::isStartCountdownDone() {
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING:
        case VARIANT_FUN_FUN_ISLAND:
        case VARIANT_THRILL_DIGGER:
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_SPIRAL_CHARGE_TUTORIAL:
        case VARIANT_PUMPKIN_ARCHERY:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_BOSS_RUSH:
        case VARIANT_TRIAL_TIME_ATTACK:      return mStart.isCountdownEndReached();
    }
    return false;
}

bool dLytMiniGame_c::isFinishAnimDone() {
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING:
        case VARIANT_FUN_FUN_ISLAND:
        case VARIANT_THRILL_DIGGER:
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_SPIRAL_CHARGE_TUTORIAL:
        case VARIANT_PUMPKIN_ARCHERY:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_BOSS_RUSH:
        case VARIANT_TRIAL_TIME_ATTACK:      return mStart.isFinishEndReached();
    }
    return false;
}

bool dLytMiniGame_c::isTimeupAnimDone() {
    switch (mVariant) {
        case VARIANT_BAMBOO_CUTTING:
        case VARIANT_FUN_FUN_ISLAND:
        case VARIANT_THRILL_DIGGER:
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_SPIRAL_CHARGE_TUTORIAL:
        // case VARIANT_PUMPKIN_ARCHERY:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_BOSS_RUSH:
        case VARIANT_TRIAL_TIME_ATTACK:      return mStart.isTimeupEndReached();
    }
    return false;
}

void dLytMiniGame_c::fn_80295B20(s32 score) {
    mScoreSd.setScoreUp(score);
}

void dLytMiniGame_c::fn_80295B30(s32 score) {
    mScoreSd.setScoreDown(score);
}

void dLytMiniGame_c::timeRelatedExecute() {
    switch (mVariant) {
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_PUMPKIN_ARCHERY:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_BOSS_RUSH:
        case VARIANT_TRIAL_TIME_ATTACK: mTime.fn_8028DD80(); break;
        case VARIANT_SPIRAL_CHARGE_TUTORIAL:
            if (!dScGame_c::GetInstance()->isFadingIn()) {
                mTime.fn_8028DD80();
            }
            break;
        case VARIANT_12:
        case VARIANT_14:
        case VARIANT_15:
        case VARIANT_16: mTime.startAlphaIn(); break;
    }
}

void dLytMiniGame_c::timeRelated() {
    switch (mVariant) {
        case VARIANT_12:
        case VARIANT_14:
        case VARIANT_15:
        case VARIANT_16: mTime.startAlphaOut(); break;
    }
}

void dLytMiniGame_c::setDisplayedTime(s32 time) {
    switch (mVariant) {
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_BOSS_RUSH:
        case VARIANT_TRIAL_TIME_ATTACK:      mTime.setTimerValueInMilliSeconds(time); break;
        case VARIANT_PUMPKIN_ARCHERY:        mTime.setTimerSecondsValueInMilliSeconds(time); break;
        case VARIANT_SPIRAL_CHARGE_TUTORIAL: mTime.setTimerSecondsValueInMilliSeconds(time); break;
        case VARIANT_12:
        case VARIANT_14:
        case VARIANT_15:
        case VARIANT_16:                     mTime.setTimerValueInMilliSeconds(time); break;
    }
}

void dLytMiniGame_c::setHighscore(s32 time) {
    switch (mVariant) {
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_BOSS_RUSH:
        case VARIANT_TRIAL_TIME_ATTACK:
        // case VARIANT_SPIRAL_CHARGE_TUTORIAL:
        case VARIANT_PUMPKIN_ARCHERY:
        case VARIANT_12:
        case VARIANT_14:
        case VARIANT_15:
        case VARIANT_16:                mTime.setBestTime(time); break;
    }
}

void dLytMiniGame_c::setShowBestTime(bool show) {
    switch (mVariant) {
        case VARIANT_INSECT_CAPTURE:
        case VARIANT_ROLLERCOASTER:
        case VARIANT_BOSS_RUSH:
        case VARIANT_TRIAL_TIME_ATTACK:
        // case VARIANT_SPIRAL_CHARGE_TUTORIAL:
        case VARIANT_PUMPKIN_ARCHERY:
        case VARIANT_12:
        case VARIANT_14:
        case VARIANT_15:
        case VARIANT_16:                mTime.setShowBestTime(show); break;
    }
}

void dLytMiniGame_c::startBugsAllIconsIn() {
    mBugs.startAllIconsIn();
}

void dLytMiniGame_c::set5Bugs() {
    mBugs.setBugsNumLevel(0);
}

void dLytMiniGame_c::set10Bugs() {
    mBugs.setBugsNumLevel(1);
}

void dLytMiniGame_c::reInitBugs() {
    mBugs.init();
}

void dLytMiniGame_c::setBug(s32 slot, s32 bugIndex) {
    mBugs.setBug(slot, bugIndex);
}

void dLytMiniGame_c::collectBug(s32 slot) {
    mBugs.collectBug(slot);
}

void dLytMiniGame_c::startBugsFlash() {
    mBugs.startBugsFlash();
}

bool dLytMiniGame_c::isBugIconOnEndReached(s32 slot) const {
    return mBugs.isSlotIconOnEndReached(slot);
}

bool dLytMiniGame_c::isBugCollected(s32 slot) const {
    return mBugs.isSlotBugCollected(slot);
}

void dLytMiniGame_c::fn_80295D80() {
    scoreRelatedExecute();
}

void dLytMiniGame_c::fn_80295D90(s32 points) {
    setDisplayedPoints(points);
}

void dLytMiniGame_c::getPumpkin(const mVec3_c &pumpkinPos, s32 unk, bool doubleScore) {
    mPumpkin.getPoint(pumpkinPos, unk, doubleScore);
}

// cannot be const...
bool dLytMiniGame_c::fn_80295DB0() {
    bool ret = false;
    switch (mVariant) {
        case VARIANT_10:
        case VARIANT_11:
        case VARIANT_12:
        case VARIANT_13:
        case VARIANT_14:
        case VARIANT_15:
        case VARIANT_16: ret = true; break;
        case VARIANT_FUN_FUN_ISLAND:
            if (!MinigameManager::GetInstance()->checkInFunFunIsland()) {
                ret = true;
            }
            break;
    }
    return ret;
}

bool dLytMiniGame_c::isInEvent() const {
    bool ret = false;
    if (EventManager::isInEvent()) {
        ret = true;
    }
    return ret;
}

bool dLytMiniGame_c::isPopupOpen() const {
    bool ret = false;
    if (dLytMeter_c::isPopupOpen()) {
        ret = true;
    }
    return ret;
}

bool dLytMiniGame_c::isPopupOpenExceptHelp() const {
    bool ret = false;
    if (isPopupOpen()) {
        ret = true;
    }

    if (dLytMeter_c::GetInstance()->isHelpOpen()) {
        ret = false;
    }

    return ret;
}

bool dLytMiniGame_c::isFadingOut() const {
    bool ret = false;
    if (dStageMgr_c::GetInstance()->isFadingOut() || dScGame_c::GetInstance()->isFadingOut()) {
        ret = true;
    }
    return ret;
}

void dLytMiniGame_c::fn_80295F90() {
    if (fn_80295DB0()) {
        // nothing
    } else {
        if (isPopupOpen()) {
            mScore.startOutTemp();
        } else {
            mScore.startIn();
        }

        if (isFadingOut()) {
            mScore.startOut();
        }
    }
}

void dLytMiniGame_c::fn_80296000() {
    if (fn_80295DB0()) {
        mScore.setField_0x816(true);
    } else {
        mScore.setField_0x816(false);
        if (isPopupOpen()) {
            mScore.startOutTemp();
        } else {
            mScore.startIn();
        }
    }
}

void dLytMiniGame_c::fn_80296070() {
    if (fn_80295DB0()) {
        if (field_0x3864 == 1) {
            s32 score = mScore.getScore();
            s32 rupeeDiff = dLytMeter_c::getRupeeDifference();
            if (score != 0) {
                if (rupeeDiff != 0) {
                    field_0x3865 = 1;
                }

                if (field_0x3865 == 1 && rupeeDiff < score) {
                    mScore.setScore(rupeeDiff);
                }
            }
        }
    } else {
        if (isPopupOpen()) {
            mScore.startOutTemp();
        } else {
            mScore.startIn();
        }

        if (isFadingOut()) {
            mScore.startOut();
        }
    }
}

void dLytMiniGame_c::fn_80296150() {
    if (fn_80295DB0()) {
        // nothing
    } else {
        if (isInEvent() || isPopupOpen()) {
            if (!field_0x3866) {
                mBugs.startOutTemp();
                mTime.startOutTemp();
            } else {
                mBugs.startIn();
                mTime.startIn();
            }
        } else {
            mBugs.startIn();
            mTime.startIn();
        }

        if (isFadingOut()) {
            mBugs.startOut();
            mTime.startOut();
        }
    }
}

void dLytMiniGame_c::fn_80296210() {
    if (isInEvent() || isPopupOpen()) {
        mScore.startOutTemp();
        mTime.startOutTemp();
    } else {
        mScore.startIn();
        mTime.startIn();
    }

    if (isFadingOut()) {
        mScore.startOut();
        mTime.startOut();
    }
}

void dLytMiniGame_c::fn_802962A0() {
    if (!fn_80295DB0()) {
        if (isPopupOpen()) {
            mScore.startOutTemp();
            mTime.startOutTemp();
        } else {
            mScore.startIn();
            mTime.startIn();
        }

        if (isFadingOut()) {
            mScore.startOut();
            mTime.startOut();
        }
    }
}

void dLytMiniGame_c::fn_80296330() {
    if (fn_80295DB0()) {
        // nothing
    } else {
        if (isInEvent() || isPopupOpen()) {
            if (!field_0x3866) {
                mTime.startOutTemp();
            } else {
                mTime.startIn();
            }
        } else {
            mTime.startIn();
        }

        if (isFadingOut() && field_0x3866 == 1) {
            mTime.startOut();
        }
    }
}

// These two do the same thing
void dLytMiniGame_c::fn_802963E0() {
    if (fn_80295DB0()) {
        // nothing
    } else {
        if (isInEvent() || isPopupOpen()) {
            if (!field_0x3866) {
                mTime.startOutTemp();
            } else {
                mTime.startIn();
            }
        } else {
            mTime.startIn();
        }

        if (isFadingOut()) {
            mTime.startOut();
        }
    }
}

void dLytMiniGame_c::fn_80296480() {
    if (fn_80295DB0()) {
        // nothing
    } else {
        if (isInEvent() || isPopupOpen()) {
            if (!field_0x3866) {
                mTime.startOutTemp();
            } else {
                mTime.startIn();
            }
        } else {
            mTime.startIn();
        }

        if (isFadingOut()) {
            mTime.startOut();
        }
    }
}

void dLytMiniGame_c::init() {
    mScore.init();
    mScoreSd.init();
    mStart.init();
    mTime.init();
    mBugs.init();
    mPumpkin.init();
}

bool dLytMiniGame_c::loadData(const char *name, s32 slot) {
    bool loadRequest = false;
    switch (slot) {
        case SLOT_MINI_GAME:          loadRequest = field_0x0068; break;
        case SLOT_MINI_GAME_SCORE:    loadRequest = field_0x0069; break;
        case SLOT_MINI_GAME_TIME:     loadRequest = field_0x006A; break;
        case SLOT_MINI_GAME_BUGS:     loadRequest = field_0x006B; break;
        case SLOT_MINI_GAME_PUMPKIN:  loadRequest = field_0x006C; break;
        case SLOT_MINI_GAME_SCORE_SD: loadRequest = field_0x006D; break;
    }

    if (!loadRequest) {
        LayoutArcManager::GetInstance()->loadLayoutArcFromDisk(name, nullptr);
        switch (slot) {
            case SLOT_MINI_GAME:          field_0x0068 = true; break;
            case SLOT_MINI_GAME_SCORE:    field_0x0069 = true; break;
            case SLOT_MINI_GAME_TIME:     field_0x006A = true; break;
            case SLOT_MINI_GAME_BUGS:     field_0x006B = true; break;
            case SLOT_MINI_GAME_PUMPKIN:  field_0x006C = true; break;
            case SLOT_MINI_GAME_SCORE_SD: field_0x006D = true; break;
        }
        return true;
    }
    return false;
}

bool dLytMiniGame_c::isLoading(const char *name) const {
    return LayoutArcManager::GetInstance()->ensureLoaded1(name) != D_ARC_RESULT_OK;
}

void dLytMiniGame_c::attachLoadedData(const char *name, d2d::ResAccIf_c &resAcc) {
    resAcc.attach(LayoutArcManager::GetInstance()->getLoadedData(name), "");
}

void dLytMiniGame_c::unloadData(const char *name) {
    s32 res = LayoutArcManager::GetInstance()->ensureLoaded2(name);
    if (res == D_ARC_RESULT_ERROR_NOT_FOUND) {
        return;
    }
    if (res == D_ARC_RESULT_OK) {
        LayoutArcManager::GetInstance()->decrement(name);
    }
}
