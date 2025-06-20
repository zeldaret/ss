#include "toBeSorted/fi_context.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/d_message.h"
#include "d/d_pad.h"
#include "d/d_pouch.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "sized_string.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/unk_save_time.h"


// https://github.com/lepelog/skywardsword-tools/wiki/Navi-Table-(Fi-Advice)

// Portability hazard, only works for 32 bit architectures

struct AdviceSummaryEntry {
    /* 0x00 */ u16 storyflag;
    /* 0x02 */ u16 KEN0_entry_num;
};

struct AdviceHintEntry {
    /* 0x00 */ char *stage_name_ptr;
    /* 0x04 */ s16 storyflag;
    /* 0x06 */ u16 KEN1_entry_num;
};

struct ObjectiveEntry {
    /* 0x00 */ u16 storyflag;
    /* 0x02 */ u16 KEN2_entry_num;
};

struct AnalysisEntry {
    /* 0x00 */ char *stage_name_ptr;
    /* 0x04 */ s16 storyflag;
    /* 0x06 */ u16 KEN3_danger_entry_num;
    /* 0x08 */ u8 sutability_percent[4];
    /* 0x0C */ u16 KEN3_sutability_entry_num[4];
    /* 0x14 */ u16 area_shield_danger; // -1-no danger, 0-wooden burns, 1- iron electrifies
    /* 0x16 */ u16 KEN3_shield_entry_num;
};

struct FileHeader {
    /* 0x00 */ u32 magic; // Is `V001`
    /* 0x04 */ u16 summary_count;
    /* 0x06 */ u16 hint_count;
    /* 0x08 */ u16 objective_count;
    /* 0x0A */ u16 analysis_count;
    /* 0x0C */ AdviceSummaryEntry *summary; // ptr to AdviceSummaryEntry[summary_count]
    /* 0x10 */ AdviceHintEntry *hint;       // ptr to AdviceHintEntry[hint_count]
    /* 0x14 */ ObjectiveEntry *objective;   // ptr to ObjectiveEntry[objective_count]
    /* 0x18 */ AnalysisEntry *analysis;     // ptr to AnalysisEntry[analysis_count]
};

bool FiAnalysisHandle::isValid() const {
    return mpEntry != nullptr;
}

s16 FiAnalysisHandle::getAreaIndexForFiAreaName() const {
    if (isValid()) {
        return mpEntry->KEN3_danger_entry_num;
    } else {
        return -1;
    }
}

FiAnalysisHandle::FiAnalysisEquipmentFocus_e FiAnalysisHandle::getCurrentFocus() {
    s32 pouchContents = summarizePouchContents(nullptr, nullptr, nullptr);
    if (pouchContents == 0) {
        return FOCUS_COMBAT;
    } else if (pouchContents == 1) {
        return FOCUS_BALANCED;
    } else if (pouchContents == 2) {
        return FOCUS_TREASURE;
    } else if (pouchContents == 3) {
        return FOCUS_SURVIVAL;
    }
    return FOCUS_COMBAT;
}

s16 FiAnalysisHandle::getEquipmentFocus() const {
    static const s16 sEquipmentFocuses[] = {400, 403, 402, 401};
    if (!isValid()) {
        return -1;
    } else {
        if (StoryflagManager::sInstance->getCounterOrFlag(530)) {
            // "I cannot offer my usual analysis because you are currently engaged in a challenge created by the Thunder
            // Dragon Lanayru."
            return 180;
        }
        return sEquipmentFocuses[getCurrentFocus()];
    }
}

s32 FiAnalysisHandle::getSuitabilityPercentageArg() const {
    if (!isValid()) {
        return -1;
    } else {
        return mpEntry->sutability_percent[getCurrentFocus()];
    }
}

s16 FiAnalysisHandle::getEquipmentRecommendation() const {
    if (!isValid()) {
        return -1;
    } else {
        return mpEntry->KEN3_sutability_entry_num[getCurrentFocus()];
    }
}

s16 FiAnalysisHandle::getThreatenedShield() const {
    if (isValid()) {
        return mpEntry->area_shield_danger;
    } else {
        return -1;
    }
}

s16 FiAnalysisHandle::getShieldMessage() const {
    if (isValid()) {
        return mpEntry->KEN3_shield_entry_num;
    } else {
        return -1;
    }
}

FiContext *FiContext::sInstance;
static FileHeader *sNaviTable;

#define BIND_PTR(type, header, pointer)                                                                                \
    pointer = reinterpret_cast<type *>(reinterpret_cast<char *>(header) + reinterpret_cast<u32>(pointer))

void FiContext::initialize(void *data) {
    FileHeader *header = reinterpret_cast<FileHeader *>(data);
    BIND_PTR(AdviceSummaryEntry, header, header->summary);
    BIND_PTR(AdviceHintEntry, header, header->hint);
    AdviceHintEntry *hint = header->hint;
    for (s32 i = 0; i < header->hint_count; i++) {
        BIND_PTR(char, header, hint[i].stage_name_ptr);
    }
    BIND_PTR(ObjectiveEntry, header, header->objective);
    BIND_PTR(AnalysisEntry, header, header->analysis);
    AnalysisEntry *analysis = header->analysis;
    for (s32 i = 0; i < header->analysis_count; i++) {
        BIND_PTR(char, header, analysis[i].stage_name_ptr);
    }
    sNaviTable = header;
}

s16 FiContext::getNaviTableProgressSummary() {
    if (StoryflagManager::sInstance->getFlag(530)) {
        // When in boss rush, no progress is available since story flags
        // are unreliable here.
        // "You are currently reliving some of your previous battles within the Thunder Dragon's Lightning Round..."
        return 40;
    } else {
        // Find the last entry with a set story flag
        AdviceSummaryEntry *last = sNaviTable->summary + (sNaviTable->summary_count - 1);
        for (s32 i = 0; i < sNaviTable->summary_count; i++) {
            if (StoryflagManager::sInstance->getFlag(last->storyflag)) {
                return last->KEN0_entry_num;
            }
            last--;
        }
        return -1;
    }
}

s16 FiContext::getFiAdviceHintEntry() {
    if (StoryflagManager::sInstance->getFlag(530)) {
        // When in boss rush, no hint since location doesn't make sense
        // "Master, this is a world built from your memories by Lanayru, the Thunder Dragon"
        return 37;
    } else {
        // Find the first entry that matches the current stage, if its story flag matches (if any)
        AdviceHintEntry *first = sNaviTable->hint;
        for (s32 i = 0; i < sNaviTable->hint_count; i++) {
            if (dScGame_c::isCurrentStage(first->stage_name_ptr) &&
                (first->storyflag < 0 || StoryflagManager::sInstance->getFlag(first->storyflag))) {
                return first->KEN1_entry_num;
            }
            first++;
        }
        // "This place has no name registered in my memory, and I have no useful data..."
        return 31;
    }
}

s16 FiContext::getObjective() {
    if (StoryflagManager::sInstance->getFlag(530)) {
        // When in boss rush, objective is to beat the mode
        // "Master, your primary objective is to defeat the enemy here and overcome the grueling task the Thunder
        // Dragon, Lanayru, has set before you..."
        return 92;
    } else {
        if (StoryflagManager::sInstance->getFlag(136) && StoryflagManager::sInstance->getFlag(143) &&
            StoryflagManager::sInstance->getFlag(144) && StoryflagManager::sInstance->getFlag(145) &&
            !StoryflagManager::sInstance->getFlag(133)) {
            // "Demise has once again broken the seal that binds him..."
            return 70;
        }

        if (!StoryflagManager::sInstance->getCounterOrFlag(21)) {
            if (ItemflagManager::sInstance->getFlagDirect(ITEM_LIFE_TREE_SEED) &&
                !ItemflagManager::sInstance->getFlagDirect(ITEM_LIFE_TREE_FRUIT)) {
                // "I project that the soil in Lanayru Province will not provide enough nourishment for the Life Tree
                // Seedling..."
                return 90;
            }

            if (ItemflagManager::sInstance->getFlagDirect(ITEM_LIFE_TREE_FRUIT)) {
                // "Master, I recommend you take the Life Tree Fruit to the Thunder Dragon..."
                return 91;
            }
        }

        // Find the last entry with a set story flag
        ObjectiveEntry *last = sNaviTable->objective + (sNaviTable->objective_count - 1);
        for (s32 i = 0; i < sNaviTable->objective_count; i++) {
            if (StoryflagManager::sInstance->getFlag(last->storyflag)) {
                return last->KEN2_entry_num;
            }
            last--;
        }
        return -1;
    }
}

FiAnalysisHandle FiContext::getNaviTableEquipmentCheckEntry() {
    AnalysisEntry *first = sNaviTable->analysis;
    for (s32 i = 0; i < sNaviTable->analysis_count; i++) {
        // Find the first entry that matches the current stage, if its story flag matches (if any)
        if (dScGame_c::isCurrentStage(first->stage_name_ptr) &&
            (first->storyflag < 0 || StoryflagManager::sInstance->getFlag(first->storyflag))) {
            return FiAnalysisHandle(first);
        }
        first++;
    }
    return FiAnalysisHandle(nullptr);
}

s32 FiContext::setTargetedActorTextId(s32 id) {
    setTargetActorId(-1);
    s32 ret;
    if (id < 256) {
        ret = 6600; // Flow?
        setTargetActorId(id);
    } else if (id < 512) {
        ret = 6500; // Flow?
        setTargetActorId(id - 256);
    } else if (id < 768) {
        ret = 6700; // Flow?
        setTargetActorId(id - 512);
    } else {
        ret = 6500; // Flow?
        setTargetActorId(id - 668);
    }
    return ret;
}

u8 FiContext::rateBattlePerformance(u8 id) {
    u8 ret = 0xFF;
    u16 killCount = FileManager::GetInstance()->getEnemyKillCount(id);
    u16 enemyHitCount = FileManager::GetInstance()->getHitCountFromEnemy(id);
    if (killCount > 3 || enemyHitCount > 5) {
        if (killCount == 0) {
            ret = 6;
        } else {
            f32 ratio = (f32)enemyHitCount / (f32)killCount;
            if (ratio <= 0.2f) {
                ret = 0;
            } else if (ratio <= (1.0f / 3.0f)) {
                ret = 1;
            } else if (ratio <= 1.0f) {
                ret = 2;
            } else if (ratio <= 1.5f) {
                ret = 3;
            } else if (ratio <= 2.5f) {
                ret = 4;
            } else if (ratio <= 4.0f) {
                ret = 5;
            } else {
                ret = 6;
            }
        }
    }
    return ret;
}

u16 FiContext::prepareFiHelpIndex() {
    u16 ret = 0xFFFF;
    setHelpIndex(-1);

    if (dLytMeter_c::getShieldMaxDurability() > 0.0f && dLytMeter_c::getShieldCurrentDurability() > 0.0f) {
        if (dLytMeter_c::getShieldCurrentDurability() <= 6.0f) {
            if (!getField_0x4A()) {
                ret = 6402;
                // "The integrity of your shield has weakened..."
                setHelpIndex(1);
            }
        } else {
            if (getField_0x4A()) {
                setField_0x4A(false);
            }
        }
    }

    if (FileManager::GetInstance()->getCurrentHealth() <= 12 && !StoryflagManager::sInstance->getCounterOrFlag(808)) {
        ret = 6401;
        // "Your hearts have decreased quite dramatically..."
        setHelpIndex(0);
    }

    if (dPad::ex_c::getInstance()->isLowBattery() || dPad::ex_c::getInstance()->isOutOfBattery()) {
        if (!getField_0x48()) {
            ret = 6400;
            if (dPad::ex_c::getInstance()->isOutOfBattery()) {
                // "Master, the batteries in your Wii Remote will be depleted any moment."
                setHelpIndex(3);
            } else {
                // "Master, the batteries in your Wii Remote are nearly depleted."
                setHelpIndex(2);
            }
        }
    } else {
        if (getField_0x48()) {
            setField_0x48(false);
        }
    }

    u32 numRupees = dAcItem_c::getRupeeCounter();
    u32 walletCapacity = dAcItem_c::getCurrentWalletCapacity();

    if (!StoryflagManager::sInstance->getFlag(727) && walletCapacity != 0 && numRupees == walletCapacity &&
        !getField_0x47()) {
        ret = 6400;
        // "Master, your wallet is full..."
        setHelpIndex(5);
    }

    return ret;
}

s32 FiContext::getGlobalFiInfo0(s32 idx) {
    if (sInstance != nullptr) {
        return sInstance->mFiButtonOptions[idx];
    }
    return KEN8_None;
}

const wchar_t *FiContext::getButtonText(s32 idx) {
    SizedString<16> str;
    str.sprintf("KEN8_1%02d", idx + 1);
    return dMessage_c::getTextMessageByLabel(str, true, nullptr, 0);
}

void FiContext::create() {
    sInstance = new FiContext();
    sInstance->reset();
}

void FiContext::reset() {
    for (s32 i = 0; i < 10; i++) {
        mFiButtonOptions[i] = KEN8_None;
        field_0x3D[i] = 0xFF;
    }
    field_0x47 = false;
    field_0x48 = false;
    field_0x49 = false;
    field_0x4A = false;
    mSaveTimeRelated = 0xFF;

    fn_8016CB40();

    mTargetActorId = -1;
    mFiHelpIndex = -1;
    mIsInFiMainMenu = false;
}

void FiContext::resetSaveTimeRelated() {
    mSaveTimeRelated = 0xFF;
}

extern "C" bool isInAnySkyKeepRoom();

void FiContext::prepareFiCallOptions() {
    s32 seconds1 = OS_TICKS_TO_SEC(SaveTimeRelated::GetInstance()->getField_0x30());
    s32 seconds2 = OS_TICKS_TO_SEC(SaveTimeRelated::GetInstance()->getField_0x08());
    u8 val = 2;
    if (seconds2 < 300) {
        val = 0;
    } else if (seconds1 < 300) {
        val = 1;
    }
    if (mSaveTimeRelated != val) {
        mSaveTimeRelated = val;
    }

    mFiButtonOptions[0] = KEN8_Advice;
    if (dStageMgr_c::GetInstance()->getSTIFbyte4() == 0 && !isInAnySkyKeepRoom() && !isInLeviasFightMaybe()) {
        // In the Sky, show play time
        mFiButtonOptions[1] = KEN8_PlayTime;
    } else {
        // Elswhere, and in Sky Keep and Levias fight, present analysis
        mFiButtonOptions[1] = KEN8_Analysis;
    }
    mFiButtonOptions[2] = KEN8_Objective;
}

void FiContext::setAdviceOptions(s32 unused) {
    mFiButtonOptions[0] = KEN8_Hint;
    mFiButtonOptions[1] = KEN8_Rumors;
    mFiButtonOptions[2] = KEN8_Summary;
}

void FiContext::resetAdviceOptions() {
    mFiButtonOptions[0] = KEN8_Hint;
    mFiButtonOptions[1] = KEN8_Rumors;
    mFiButtonOptions[2] = KEN8_Summary;
}

void FiContext::fn_8016CB40() {
    field_0x28 = 0;
    field_0x2C = 1;
    field_0x30 = 2;
}

bool FiContext::isInLeviasFightMaybe() {
    if (dScGame_c::isCurrentStage("F023") && StoryflagManager::sInstance->getFlag(368) &&
        !StoryflagManager::sInstance->getFlag(200)) {
        return true;
    }
    return false;
}
