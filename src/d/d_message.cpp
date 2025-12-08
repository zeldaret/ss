#include "d/d_message.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/d_base.h"
#include "d/d_lang.h"
#include "d/d_light_env.h"
#include "d/d_lyt_hio.h"
#include "d/d_player.h"
#include "d/d_pouch.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/d_tag_processor.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_demo_dowsing.h"
#include "d/lyt/d_lyt_map.h"
#include "d/lyt/d_lyt_mini_game.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/snd/d_snd_state_mgr.h"
#include "egg/core/eggHeap.h"
#include "f/f_base.h"
#include "f/f_profile.h"
#include "f/f_profile_name.h"
#include "libms/flowfile.h"
#include "libms/libms.h"
#include "libms/msgfile.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/dowsing_target.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/fi_context.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/minigame_mgr.h"
#include "toBeSorted/unk_save_time.h"

#include "rvl/OS.h"

#include <cstring>
#include <stdio.h>

s32 dFlow_c::sExitId = -1;

dFlow_c::dFlow_c() {
    field_0x14 = 0;
    mpMsbf = nullptr;
    mCurrentFlowIndex = -1;
    field_0x0E = 0;
    field_0x0F = 0;
    field_0x10 = 1;
    field_0x3C = 0;
    field_0x40 = -1;
    field_0x44 = -1;
    mNextFiFlow = -1;
    mDelayTimer = 0;
    mFiSpeechArgument = -1;
}

dFlow_c::~dFlow_c() {}

u16 dFlow_c::findEntryPoint(u16 labelPart1, u16 labelPart2) {
    char buf[8];
    for (int i = 0; i < 8; i++) {
        buf[i] = '\0';
    }
    u16 ret = 0xFFFF;
    if (labelPart1 < 100) {
        sprintf(buf, "%03d_%03d", labelPart1, labelPart2);
    } else {
        sprintf(buf, "%03d_%02d", labelPart1, labelPart2);
    }
    for (int i = 0; i < 80; i++) {
        if (dMessage_c::getMsbfInfoForIndex(i) != nullptr) {
            int entry = LMS_GetEntrypoint(dMessage_c::getMsbfInfoForIndex(i), buf);
            if (entry >= 0) {
                mpMsbf = dMessage_c::getMsbfInfoForIndex(i);
                s32 fileNumber = dMessage_c::getMsbtIndexForMsbfIndex(i);
                ret = entry;
                dMessage_c::getInstance()->setCurrentTextFileNumber(fileNumber);
            }
        }
    }

    const char *arcName = dMessage_c::getArcNameByIndex(labelPart1 / 100, true);
    dLytMsgWindow_c::getInstance()->setCurrentFlowFilename(arcName);
    dLytMsgWindow_c::getInstance()->setCurrentEntrypointName(buf);
    return ret;
}

u16 dFlow_c::findEntryPoint(const char *label) {
    u16 ret = 0xFFFF;
    for (int i = 0; i < 80; i++) {
        if (dMessage_c::getMsbfInfoForIndex(i) != nullptr) {
            int entry = LMS_GetEntrypoint(dMessage_c::getMsbfInfoForIndex(i), label);
            if (entry >= 0) {
                mpMsbf = dMessage_c::getMsbfInfoForIndex(i);
                s32 fileNumber = dMessage_c::getMsbtIndexForMsbfIndex(i);
                ret = entry;
                dMessage_c::getInstance()->setCurrentTextFileNumber(fileNumber);
            }
        }
    }

    const char *arcName = dMessage_c::getArcNameByIndex(dLytMsgWindow_c::fn_800D7B40() / 10000, true);
    dLytMsgWindow_c::getInstance()->setCurrentFlowFilename(arcName);
    dLytMsgWindow_c::getInstance()->setCurrentEntrypointName(label);
    return ret;
}

void dFlow_c::setNext(u16 next) {
    if (next != 0xFFFF) {
        if (mCurrentFlowIndex != next) {
            mCurrentFlowIndex = next;
        } else {
            clear();
        }
    } else {
        clear();
    }
}

void dFlow_c::advanceFlow() {
    bool keepGoing = true;

    if (dLytMsgWindow_c::getInstance()->getTagProcessor()->getField_0x8FC() >= 0 &&
        dLytMsgWindow_c::getInstance()->getTagProcessor()->getField_0x900() >= 0) {
        triggerEntryPoint(
            dLytMsgWindow_c::getInstance()->getTagProcessor()->getField_0x8FC(),
            dLytMsgWindow_c::getInstance()->getTagProcessor()->getField_0x900()
        );
        field_0x0F = 1;
        dLytMsgWindow_c::getInstance()->getTagProcessor()->setFields_0x8FC_0x900(-1, -1);
    } else {
        while (keepGoing && !vt_0x18() && !dLytMsgWindow_c::getInstance()->getField_0x815() && !checkField0x3C()) {
            MsbFlowInfo *element = LMS_GetFlowElement(mpMsbf, mCurrentFlowIndex);
            s32 prevIdx = mCurrentFlowIndex;
            s32 type = element->type;
            mCurrentTextLabelName = "";
            switch (type) {
                case FLOW_MESSAGE: keepGoing = handleMessage(); break;
                case FLOW_BRANCH:  keepGoing = handleBranch(); break;
                case FLOW_EVENT:
                    handleEvent();
                    keepGoing = false;
                    break;
                case FLOW_ENTRY: keepGoing = handleEntry(); break;
                case FLOW_JUMP:  keepGoing = handleJump(); break;
            }
            field_0x10 = prevIdx != mCurrentFlowIndex;
            if (field_0x10) {
                memcpy(&mFlowInfo, element, sizeof(MsbFlowInfo));
            }
        }
    }
}

bool dFlow_c::advanceUntilEvent(s32 searchParam3, s32 *pOutParams1n2) {
    return advanceUntil(FLOW_EVENT, searchParam3, pOutParams1n2) == true;
}

bool dFlow_c::vt_0x18() const {
    bool ret = false;
    if (field_0x0E && dLytMsgWindow_c::getInstance()->getField_0x815() == false) {
        ret = true;
    }
    return ret;
}

struct FlowSoundDef {
    /* 0x00 */ u32 mParams;
    /* 0x04 */ u32 mSoundMgr;
    /* 0x08 */ WZSound mSoundId;
};

static const FlowSoundDef sSoundDefs[] = {
    { 1, 0,        FAN_ITEM_GET_MINI},
    { 2, 0,             FAN_ITEM_GET},
    { 3, 0,            FAN_HEART_GET},
    { 4, 1,       SE_S_MSG_IMPORTANT},
    { 5, 1,            SE_S_REACTION},
    { 6, 1,            SE_S_MSG_GOOD},
    { 7, 1,         SE_S_MSG_PRESAGE},
    { 8, 0, FAN_TRANSITION_IMPACT_01},
    { 9, 1,       SE_S_READ_RIDDLE_A},
    {10, 1,       SE_S_READ_RIDDLE_B},
};

void dFlow_c::playSound(u32 params) {
    if (params >= 1000) {
        dSndStateMgr_c::GetInstance()->setFlowEvent(params);
        return;
    }
    if (params >= 100) {
        dSndSourceMgr_c::GetInstance()->playFlowSound(params);
        return;
    }

    s32 idx = -1;
    for (int i = 0; i < 10; i++) {
        if (sSoundDefs[i].mParams == params) {
            idx = i;
            break;
        }
    }

    if (idx < 0) {
        return;
    }

    switch (sSoundDefs[idx].mSoundMgr) {
        case 0: dSndBgmMgr_c::GetInstance()->playFanSound(sSoundDefs[idx].mSoundId); break;
        case 1: dSndSmallEffectMgr_c::GetInstance()->playSound(sSoundDefs[idx].mSoundId); break;
    }
}

bool dFlow_c::handleEventInternal(const MsbFlowInfo *element) {
    u32 params1n2 = element->params1n2;
    switch (element->param3) {
        case EVENT_SET_STORYFLAG:
            StoryflagManager::sInstance->setFlag(params1n2);
            if (params1n2 == 0x52) {
                dLytMeter_c::GetInstance()->setMeterField_0x13775(true);
            }
            if (dLytMsgWindow_c::fn_800D7B40() != 50013 && dLytMsgWindow_c::fn_800D7B40() != 20061) {
                if (params1n2 == 100 || params1n2 == 64 || params1n2 == 271 || params1n2 == 81 || params1n2 == 668 ||
                    params1n2 == 669) {
                    FileManager::GetInstance()->setDowsingSlotIdx(DowsingTarget::SLOT_STORY_EVENT);
                } else if (params1n2 == 106 || params1n2 == 107) {
                    FileManager::GetInstance()->setDowsingSlotIdx(DowsingTarget::SLOT_QUEST);
                }
            }
            break;
        case EVENT_UNSET_STORYFLAG: StoryflagManager::sInstance->unsetFlag(params1n2); break;
        case EVENT_SET_SCENEFLAG:
            dStageMgr_c::GetInstance()->getFlagIndex();
            SceneflagManager::sInstance->setFlag(0x3F, (params1n2 >> 16) & 0xFFFF);
            break;
        case EVENT_UNSET_SCENEFLAG:
            dStageMgr_c::GetInstance()->getFlagIndex();
            SceneflagManager::sInstance->unsetFlag(0x3F, (params1n2 >> 16) & 0xFFFF);
            break;
        case EVENT_SET_ZONEFLAG:
            dStageMgr_c::GetInstance()->getFlagIndex();
            SceneflagManager::sInstance->setZoneflag_i(
                dStage_c::GetInstance()->getCurrRoomId(), (params1n2 >> 16) & 0xFFFF
            );
            break;
        case EVENT_UNSET_ZONEFLAG:
            dStageMgr_c::GetInstance()->getFlagIndex();
            SceneflagManager::sInstance->unsetZoneflag_i(
                dStage_c::GetInstance()->getCurrRoomId(), (params1n2 >> 16) & 0xFFFF
            );
            break;
        case EVENT_DELAY:
            mDelayTimer++;
            if (mDelayTimer < params1n2) {
                return false;
            }
            mDelayTimer = 0;
            break;
        case EVENT_LOAD_FI_FLOW:
            if (params1n2 == -1) {
                s32 hi, lo;
                s32 selectedOption = dLytMsgWindow_c::getInstance()->getTextOptionSelection();
                mFiInfo0 = FiContext::getGlobalFiInfo0(selectedOption);
                switch (mFiInfo0) {
                    case FiContext::KEN8_Summary: {
                        hi = 6;
                        lo = 1;
                        mFiSpeechArgument = FiContext::getNaviTableProgressSummary();
                        break;
                    }
                    case 11:
                        // TODO: Is this one actually used? It completely duplicates
                        // KEN8_Hint (case 1) down below...
                        hi = 6;
                        lo = 100;
                        mFiSpeechArgument = FiContext::getFiAdviceHintEntry();
                        break;
                    case FiContext::KEN8_Objective: {
                        hi = 6;
                        lo = 200;
                        mFiSpeechArgument = FiContext::getObjective();
                        break;
                    }
                    case FiContext::KEN8_Analysis: {
                        FiAnalysisHandle handle = FiContext::getNaviTableEquipmentCheckEntry();
                        if (handle.isValid()) {
                            hi = 6;
                            lo = 300;
                        } else {
                            hi = 6;
                            lo = 301;
                        }
                        mFiSpeechArgument = 5;
                        break;
                    }
                    case FiContext::KEN8_PlayTime:
                        hi = 6;
                        lo = 800;
                        break;
                    case FiContext::KEN8_Advice:
                        hi = 6;
                        lo = 802;
                        break;
                    case FiContext::KEN8_Hint: {
                        hi = 6;
                        lo = 100;
                        mFiSpeechArgument = FiContext::getFiAdviceHintEntry();
                        break;
                    }
                    case FiContext::KEN8_Rumors:
                        hi = 6;
                        lo = 900;
                        break;
                }
                FiContext::do_setAdviceOptions(FiContext::getGlobalFiInfo0(selectedOption));
                mNextFiFlow = lo + hi * 1000;
            } else {
                triggerEntryPoint((params1n2 >> 16) & 0xFFFF, params1n2 & 0xFFFF);
                field_0x0F = 1;
            }
            break;
        case EVENT_RUPEES:
            if (dMessage_c::getInstance()->getField_0x2FC() != -1) {
                dMessage_c::getInstance()->setField_0x2FC(0x3C);
            }
            dAcItem_c::addRupees(params1n2);
            if ((s32)params1n2 > 0) {
                dLytMeter_c::setRupyField_0x8AD(1);
            }
            break;
        case EVENT_SET_ITEM: {
            u16 flag = params1n2 & 0xFFFF;
            ItemflagManager::sInstance->setItemFlag(flag);
            switch (params1n2) {
                case ITEM_FARORES_COURAGE:
                case ITEM_NAYRUS_WISDOM:
                case ITEM_DINS_POWER:
                case ITEM_SOTH:            FileManager::GetInstance()->setDowsingSlotIdx(DowsingTarget::SLOT_STORY_EVENT); break;
            }
            break;
        }
        case EVENT_EXIT: {
            u16 id = (params1n2 >> 16) & 0xFFFF;
            sExitId = id;
            if ((params1n2 & 0xFFFF) == 1) {
                dScGame_c::GetInstance()->triggerExit(
                    dStage_c::GetInstance()->getCurrRoomId(), id, SpawnInfo::RETAIN_TOD, SpawnInfo::NO_TRIAL
                );
            } else {
                dScGame_c::GetInstance()->triggerExit(
                    dStage_c::GetInstance()->getCurrRoomId(), id, SpawnInfo::RETAIN_TOD, SpawnInfo::RETAIN_TRIAL
                );
            }
            return 0;
        }
        case EVENT_12:
            if (params1n2 == 1) {
                dMessage_c::getInstance()->setField_0x2FC(-1);
            } else {
                dLytMeter_c::setRupyField_0x8AC(1);
                dMessage_c::getInstance()->setField_0x2FC(0);
            }
            break;
        case EVENT_COUNTER_THRESHOLD: {
            u16 counter = (params1n2 >> 16) & 0xFFFF;
            u16 threshold = (params1n2 & 0xFFFF);
            if (counter == 0x1F5) {
                counter = dAcItem_c::getRupeeCounter();
            } else if (counter == 0x1ED) {
                counter = dAcItem_c::getTotalSeedCount();
            } else if (counter == 0x1F2) {
                counter = dAcItem_c::getTotalArrowCount();
            } else if (counter == 0x1F3) {
                counter = dAcItem_c::getTotalBombCount();
            } else {
                counter = ItemflagManager::sInstance->getItemCounterOrFlag(counter);
            }

            if (counter >= threshold) {
                mResultFromCounterCheck = 0;
            } else {
                mResultFromCounterCheck = 1;
            }
            break;
        }
        case EVENT_PLAY_SOUND: playSound(params1n2); break;
        case EVENT_ADD_ITEM:   {
            u16 flag = (params1n2 >> 16) & 0xFFFF;
            s16 change = (s16)(params1n2 & 0xFFFF);
            s32 value = ItemflagManager::sInstance->getItemCounterOrFlag(flag);
            value += change;
            if (value < 0) {
                value = 0;
            } else if (value > 0x8000) {
                value = 0x7FFF;
            }
            ItemflagManager::sInstance->setItemFlagOrCounterToValue(flag, value);
            break;
        }
        case EVENT_SET_TEMPFLAG:
            dStageMgr_c::GetInstance()->getFlagIndex();
            SceneflagManager::sInstance->setTempflag_i(0x3F, (params1n2 >> 16) & 0xFFFF);
            break;
        case EVENT_UNSET_TEMPFLAG:
            dStageMgr_c::GetInstance()->getFlagIndex();
            SceneflagManager::sInstance->unsetTempflag_i(0x3F, (params1n2 >> 16) & 0xFFFF);
            break;
        case EVENT_START_MAP_EVENT: {
            s8 p4 = (params1n2 >> 24) & 0xFF;
            s8 p1 = params1n2 & 0xFF;
            s8 arg1 = (params1n2 >> 16) & 0xFF;
            s8 arg2 = (params1n2 >> 8) & 0xFF;
            s32 mapEvent = 1;
            switch (p1) {
                case 1: mapEvent = dLytMapMain_c::MAP_EVENT_DUNGEON_MAP_GET; break;
                case 2: mapEvent = dLytMapMain_c::MAP_EVENT_MAP_INTRO; break;
                case 3: mapEvent = dLytMapMain_c::MAP_EVENT_FIELD_MAP_CHANGE_5; break;
                case 5: mapEvent = dLytMapMain_c::MAP_EVENT_SIGNAL_ADD; break;
                case 6: mapEvent = dLytMapMain_c::MAP_EVENT_FIELD_MAP_CHANGE_8; break;
                case 7: mapEvent = dLytMapMain_c::MAP_EVENT_GODDESS_CUBE; break;
            }
            dMessage_c::getInstance()->setMapEvent(mapEvent);
            dMessage_c::getInstance()->setField_0x329(true);
            if (dMessage_c::getInstance()->getInMapEvent() == false) {
                dMessage_c::getInstance()->setInMapEvent(true);
                dMessage_c::getInstance()->clearLightPillarRelatedArgs();
                if (dLytControlGame_c::getInstance()->isStateNotNormalOrInEvent()) {
                    dLytControlGame_c::getInstance()->openMapEvent(mapEvent, arg1, arg2);
                }
            } else {
                dLytMap_c::GetInstance()->queueMapEvent(mapEvent, arg1, arg2);
            }
            dMessage_c::getInstance()->storeLightPillarRelatedArg(p4);
            break;
        }
        case EVENT_END_MAP_EVENT: {
            // TODO what do these modes do?
            if (params1n2 == 1) {
                // Close map
                if (!dLytControlGame_c::getInstance()->isNotInStateMap()) {
                    dLytControlGame_c::getInstance()->setMapEventDone(true);
                }
            } else if (params1n2 == 2) {
                dMessage_c::getInstance()->setField_0x330(1);
                if (dMessage_c::getInstance()->getMapEvent() != dLytMapMain_c::MAP_EVENT_DUNGEON_MAP_GET &&
                    dMessage_c::getInstance()->getMapEvent() != dLytMapMain_c::MAP_EVENT_MAX) {
                    dMessage_c::getInstance()->setField_0x32A(1);
                }
            } else {
                if (dMessage_c::getInstance()->getMapEvent() != dLytMapMain_c::MAP_EVENT_DUNGEON_MAP_GET &&
                    dMessage_c::getInstance()->getMapEvent() != dLytMapMain_c::MAP_EVENT_MAX) {
                    dMessage_c::getInstance()->setField_0x32A(1);
                }
            }
            dMessage_c::getInstance()->setInMapEvent(false);
            break;
        }
        case EVENT_SET_STORYFLAG_217:
            StoryflagManager::sInstance->setFlagOrCounterToValue(
                STORYFLAG_217, dLytMsgWindow_c::getInstance()->getTextOptionSelection()
            );
            break;
        case EVENT_DEMO_METER_ITEM_SELECT: return dLytMeter_c::GetInstance()->itemSelectDemoRelated(params1n2);
        case EVENT_CAMERA_42:              {
            s32 p1 = (params1n2 >> 16) & 0xFFFF;
            s32 p2 = params1n2 & 0xFFFF;
            dScGame_c::getCamera(0)->doFn_800918E0(p1, p2);
            break;
        }
        case EVENT_LYT_MINI_GAME: {
            if (dMessage_c::getInstance()->hasSetMinigameResult()) {
                // clear old result
                clearMinigame();
            }
            // create new result
            dMessage_c::getInstance()->setMiniGameVariant(params1n2);
            createLytMiniGame();
            break;
        }
        case EVENT_LYT_MINI_GAME_END:
            // hide score/time
            clearMinigame();
            break;
        case EVENT_46:              dMessage_c::getInstance()->setField_0x344(params1n2); break;
        case EVENT_RESET_STORYFLAG: StoryflagManager::sInstance->setFlagOrCounterToValue(params1n2 & 0xFFFF, 0); break;
        case EVENT_SET_ITEMFLAG:    dAcItem_c::setFlag(params1n2 & 0xFFFF); break;
        case EVENT_PALETTE:         {
            s16 p1 = (params1n2 >> 16) & 0xFFFF;
            s16 p2 = params1n2 & 0xFFFF;
            if (&dLightEnv_c::GetInstance() != nullptr) {
                dLightEnv_c::GetInstance().set_palette_transition(-1, p1, p2);
            }
            break;
        }
        case EVENT_DEMO_DOWSING:
            if (dLytDemoDowsing_c::GetInstance() != nullptr) {
                dLytDemoDowsing_c::GetInstance()->start();
            }
            break;
        case EVENT_DEMO_METER_DOWSING:   return dLytMeter_c::GetInstance()->dowsingDemoRelated(params1n2); break;
        case EVENT_DEMO_METER_MINUS_BTN: return dLytMeter_c::GetInstance()->minusBtnDemoRelated(params1n2); break;
        case EVENT_SELECT_STORY_DOWSING:
            FileManager::GetInstance()->setDowsingSlotIdx(DowsingTarget::SLOT_STORY_EVENT);
            break;
        case EVENT_DEMO_COLLECTION_SCREEN:
            if (!MinigameManager::isInMinigameState(MinigameManager::INSECT_CAPTURE)) {
                dLytControlGame_c::getInstance()->openPauseDemo();
            }
            break;
    }

    return true;
}

bool dFlow_c::handleEvent() {
    MsbFlowInfo *element = LMS_GetFlowElement(mpMsbf, mCurrentFlowIndex);
    u16 next = element->next;
    if (handleEventInternal(element)) {
        if (next != 0xFFFF) {
            setNext(next);
            if (checkField0x3C()) {
                field_0x40 = mCurrentFlowIndex;
                field_0x44 = dLytMsgWindow_c::fn_800D7B40();
            }
        } else if (field_0x0F != 0) {
            field_0x0F = 0;
        } else {
            clear();
        }
        return true;
    } else {
        return false;
    }
}

bool dFlow_c::handleMessage() {
    char label[40];
    MsbFlowInfo *flow = LMS_GetFlowElement(mpMsbf, mCurrentFlowIndex);
    int hasLabel = LMS_GetLabelByTextIndex(
        dMessage_c::getMsbtInfoForIndex(dMessage_c::getInstance()->getCurrentTextFileNumber()), flow->param4, label
    );
    mCurrentTextLabelName.set(label);
    u16 next = flow->next;
    // Does this make sense? Result is unused...
    LMS_GetAttribute(dMessage_c::getMsbtInfoForIndex(dMessage_c::getInstance()->getCurrentTextFileNumber()), hasLabel);
    if (mCurrentTextLabelName == "KEN0_08") {
        // "After winning the race in the Wing Ceremony..."
        // Current objective - use value from FiContext::getObjective?
        if (mFiSpeechArgument < 0) {
            mFiSpeechArgument = 8;
        }
        mCurrentTextLabelName.sprintf("KEN0_%02d", mFiSpeechArgument);
    } else if (mCurrentTextLabelName == "KEN1_000") {
        // "This is Skyloft..."
        // Area analysis - use value from FiContext::???
        if (mFiSpeechArgument < 0) {
            mFiSpeechArgument = 0;
        }
        mCurrentTextLabelName.sprintf("KEN1_%03d", mFiSpeechArgument);
    } else if (mCurrentTextLabelName == "KEN2_000") {
        // "To search for Zelda..."
        // Hint - use value from FiContext::getFiAdviceHintEntry?
        if (mFiSpeechArgument < 0) {
            mFiSpeechArgument = 2;
        }
        mCurrentTextLabelName.sprintf("KEN2_%03d", mFiSpeechArgument);
    } else if (mCurrentTextLabelName == "KEN3_500") {
        // "Master, your current selection of pouch items is..."
        // Equipment focus
        FiAnalysisHandle analysis = FiContext::getNaviTableEquipmentCheckEntry();
        SizedString<16> label;
        label.sprintf("KEN3_%03d", analysis.getEquipmentFocus());
        dLytMsgWindow_c::getInstance()->getTagProcessor()->setStringArg(
            dMessage_c::getTextMessageByLabel(label, true, nullptr, nullptr), 0
        );
    } else if (mCurrentTextLabelName == "KEN3_000") {
        // "You are located in the Sealed Grounds..."
        // Threat assessment
        FiAnalysisHandle analysis = FiContext::getNaviTableEquipmentCheckEntry();
        s16 value = analysis.getAreaIndexForFiAreaName();
        if (value < 0) {
            value = 0;
        }
        mCurrentTextLabelName.sprintf("KEN3_%03d", value);
    } else if (mCurrentTextLabelName == "KEN3_501") {
        // "Suitability to current location is..."
        // Suitability analysis
        FiAnalysisHandle analysis = FiContext::getNaviTableEquipmentCheckEntry();
        s32 arg = analysis.getSuitabilityPercentageArg();
        dLytMsgWindow_c::getInstance()->setNumericArg0(arg);
    } else if (mCurrentTextLabelName == "KEN3_100") {
        // "My projections indicate that equipping..."
        // Equipment recommendation
        FiAnalysisHandle analysis = FiContext::getNaviTableEquipmentCheckEntry();
        s16 value = analysis.getEquipmentRecommendation();
        if (value < 0) {
            value = 0;
        }
        mCurrentTextLabelName.sprintf("KEN3_%03d", value);
    } else if (mCurrentTextLabelName == "KEN3_200") {
        // "To make matters worse, your Wooden Shield..."
        // Shield danger (wooden vs fire, iron vs electrical)
        FiAnalysisHandle analysis = FiContext::getNaviTableEquipmentCheckEntry();
        s16 value = analysis.getShieldMessage();
        if (value < 0) {
            value = 0;
        }
        mCurrentTextLabelName.sprintf("KEN3_%03d", value);
    } else if (mCurrentTextLabelName == "KEN4_000") {
        // Your hearts have decreased quite dramatically...
        s32 fiHelpIndex = FiContext::getHelpIndex();
        if (fiHelpIndex < 0) {
            fiHelpIndex = 0;
        }
        if (fiHelpIndex == 5) {
            StoryflagManager::sInstance->setFlag(STORYFLAG_WALLET_FULL_ACK);
        }
        mCurrentTextLabelName.sprintf("KEN4_%03d", fiHelpIndex);
    } else if (mCurrentTextLabelName == "KEN5_000") {
        // This is...
        s32 targetActorId = FiContext::getTargetActorId();
        if (targetActorId < 0) {
            targetActorId = 0;
        }
        if (targetActorId <= 480) {
            mCurrentTextLabelName.sprintf("KEN5_%03d", targetActorId);
        } else {
            // "My apologies. I have no relevant information on the subject in my sizable memory."
            // FiHead
            mCurrentTextLabelName.sprintf("KEN7_000");
        }
    } else if (mCurrentTextLabelName == "KEN6_000") {
        // "Target lock: ..."
        s32 targetActorId = FiContext::getTargetActorId();
        if (targetActorId < 0) {
            targetActorId = 0;
        }
        if (targetActorId <= 91) {
            mCurrentTextLabelName.sprintf("KEN6_%03d", targetActorId);
        } else {
            mCurrentTextLabelName.sprintf("KEN7_000");
        }
    } else if (mCurrentTextLabelName == "KEN6_107") {
        // "You have defeated <X> of this type of enemy. My analysis shows that your battle performance..."
        s32 targetActorId = FiContext::getTargetActorId();
        if (targetActorId < 0) {
            targetActorId = 0;
        }
        u16 killCount = FileManager::GetInstance()->getEnemyKillCount(targetActorId);
        u8 performance = FiContext::rateBattlePerformance(targetActorId);
        if (performance == 0xFF) {
            dLytMsgWindow_c::getInstance()->setNumericArg0(killCount);
            // "You have defeated <X> of this enemy type. I am unable to analyze your battle performance..."
            mCurrentTextLabelName.sprintf("KEN6_108");
        } else {
            dLytMsgWindow_c::getInstance()->setNumericArg0(killCount);
            SizedString<16> tmpLabel;
            // "very strong, ..., very weak"
            tmpLabel.sprintf("KEN6_1%02d", performance);
            dLytMsgWindow_c::getInstance()->getTagProcessor()->setStringArg(
                dMessage_c::getTextMessageByLabel(tmpLabel, true, nullptr, nullptr), 0
            );
        }
    } else if (mCurrentTextLabelName == "KEN7_000") {
        // "My apologies. I have no relevant information on the subject in my sizable memory."
        s32 targetActorId = FiContext::getTargetActorId();
        if (targetActorId < 0) {
            targetActorId = 0;
        }
        if (targetActorId <= 561) {
            // object info
            mCurrentTextLabelName.sprintf("KEN7_%03d", targetActorId);
        } else {
            mCurrentTextLabelName.sprintf("KEN7_000");
        }
    } else if (mCurrentTextLabelName == "KEN8_000" || mCurrentTextLabelName == "KEN2_096") {
        // "Current Session Play Time: ... Total Play Time: ..."
        s32 seconds = OS_TICKS_TO_SEC(SaveTimeRelated::GetInstance()->getField_0x08());
        s32 minutes_ = seconds / 60;
        s32 minutes = minutes_ % 60;
        s32 hours = seconds / 3600;
        if (hours > 99) {
            hours = 99;
            minutes = 59;
        }
        s32 seconds1 = OS_TICKS_TO_SEC(SaveTimeRelated::GetInstance()->fn_801907D0());
        s32 minutes1_ = seconds1 / 60;
        s32 minutes1 = minutes1_ % 60;
        s32 hours1 = seconds1 / 3600;
        if (hours1 > 999) {
            hours1 = 999;
            minutes1 = 59;
        }
        s32 time[4] = {hours, minutes, hours1, minutes1};
        dLytMsgWindow_c::getInstance()->setNumericArgs(time, 4);
    } else if (mCurrentTextLabelName == "KEN9_000") {
        // Random hint
        s32 v;
        if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_BOSS_RUSH_ACTIVE)) {
            // "When you require my analysis..."
            v = 200;
        } else if (dStageMgr_c::GetInstance()->getSTIFArea() == dStageMgr_c::STIF_AREA_SKY) {
            // Sky rumor
            v = cM::rndInt(30);
        } else {
            // Elsewhere rumor
            v = cM::rndInt(30) + 30;
        }
        mCurrentTextLabelName.sprintf("KEN9_%03d", v);
    }

    if (dLytMsgWindow_c::getInstance()->setCurrentLabelName(mCurrentTextLabelName, false) == true) {
        if (!dLytMsgWindow_c::getInstance()->getField_0x80D()) {
            field_0x14 = 1;
        }
        if (next != 0xFFFF) {
            setNext(next);
        } else {
            clear();
        }
    }

    return false;
}

u16 dFlow_c::getSwitchChoice(const MsbFlowInfo *element, u16 param) const {
    u16 result = 0;
    if (param < BRANCH_NPC_1 || param > BRANCH_NPC_3) {
        result = (this->*(sBranchHandlers[param]))(element);
    }
    return result;
}

inline void syncFiButtonText(s32 argIdx, s32 btnIdx) {
    dTagProcessor_c *p = dLytMsgWindow_c::getInstance()->getTagProcessor();
    p->setStringArg(FiContext::getMessageForFiInfo(argIdx), btnIdx);
}

inline void setFiButtonTextDirectly(s32 textLabel, s32 btnIdx) {
    dTagProcessor_c *p = dLytMsgWindow_c::getInstance()->getTagProcessor();
    p->setStringArg(FiContext::getButtonText(textLabel), btnIdx);
}

u16 dFlow_c::branchHandler00(const MsbFlowInfo *element) const {
    return dLytMsgWindow_c::getInstance()->getTextOptionSelection();
}

u16 dFlow_c::branchHandler01(const MsbFlowInfo *element) const {
    return dLytMsgWindow_c::getInstance()->getTextOptionSelection();
}

u16 dFlow_c::branchHandler02(const MsbFlowInfo *element) const {
    if (FiContext::getIsInFiMainMenuChecked()) {
        if (dLytMsgWindow_c::getInstance()->getTextOptionSelection() == 0 &&
            FiContext::getGlobalFiInfo0(0) == FiContext::KEN8_Advice) {
            // HACK (?): If the user presses "Advice" on the Fi main menu,
            // which is known to be button 0, update Fi's buttons since that
            // menu isn't entirely integrated into the flow
            FiContext::do_resetAdviceOptions();
            syncFiButtonText(0, 0);
            syncFiButtonText(1, 1);
            syncFiButtonText(2, 2);
        } else {
            FiContext::setIsInFiMainMenu(false);
        }
    }
    return dLytMsgWindow_c::getInstance()->getTextOptionSelection();
}

u16 dFlow_c::branchHandler03(const MsbFlowInfo *element) const {
    return !StoryflagManager::sInstance->getCounterOrFlag(element->params1n2);
}

u16 dFlow_c::branchHandler04(const MsbFlowInfo *element) const {
    return 0;
}

u16 dFlow_c::branchHandler05(const MsbFlowInfo *element) const {
    return !SceneflagManager::sInstance->checkZoneflag_i(
        dStage_c::GetInstance()->getCurrRoomId(), element->params1n2 & 0xFFFF
    );
}

u16 dFlow_c::branchHandler06(const MsbFlowInfo *element) const {
    return !SceneflagManager::sInstance->checkFlag(0x3F, (element->params1n2 & 0xFFFF));
}

u16 dFlow_c::branchHandler07(const MsbFlowInfo *element) const {
    return mResultFromCounterCheck != 0;
}

u16 dFlow_c::branchHandler08(const MsbFlowInfo *element) const {
    return mResultFromCounterCheck != 0;
}

u16 dFlow_c::branchHandler09(const MsbFlowInfo *element) const {
    return !SceneflagManager::sInstance->checkTempflag_i(0x3F, element->params1n2 & 0xFFFF);
}

u16 dFlow_c::branchHandler10(const MsbFlowInfo *element) const {
    u16 threshold = element->params1n2 & 0xFFFF;
    return threshold > dAcItem_c::getRupeeCounter();
}

u16 dFlow_c::branchHandler11(const MsbFlowInfo *element) const {
    return (s32)(cM::rnd() * 2.0f);
}

u16 dFlow_c::branchHandler12(const MsbFlowInfo *element) const {
    return (s32)(cM::rnd() * 3.0f);
}

u16 dFlow_c::branchHandler13(const MsbFlowInfo *element) const {
    return (s32)(cM::rnd() * 4.0f);
}

u16 dFlow_c::branchHandler14(const MsbFlowInfo *element) const {
    return 0;
}

u16 dFlow_c::branchHandler15(const MsbFlowInfo *element) const {
    return 0;
}

u16 dFlow_c::branchHandler16(const MsbFlowInfo *element) const {
    return 0;
}

u16 dFlow_c::branchHandler17(const MsbFlowInfo *element) const {
    bool ret = false;
    if (adventurePouchFindItemSlot(ITEM_NONE) != POUCH_SLOT_NONE) {
        ret = true;
    }
    return ret;
}

u16 dFlow_c::branchHandler18(const MsbFlowInfo *element) const {
    bool ret = false;
    if (!getLinkPtr()->isItemFairyFromBugnet()) {
        ret = true;
    }
    return ret;
}

u16 dFlow_c::branchHandler19(const MsbFlowInfo *element) const {
    u16 ret = 1;
    switch (element->params1n2 & 0xFFFF) {
        case 0:
            if (dAcPy_c::getCurrentlyEquippedShieldType() != 4) {
                ret = 0;
            }
            break;
        case 1:
            if (adventurePouchFindItemSlot(ITEM_BOTTLE) != POUCH_SLOT_NONE) {
                ret = 0;
            }
            break;
        case 2:
            if (adventurePouchFindItemSlot(ITEM_HOT_SOUP) != POUCH_SLOT_NONE) {
                ret = 0;
            }
            break;
        case 3:
            if (adventurePouchFindItemSlot(ITEM_COLD_SOUP) != POUCH_SLOT_NONE) {
                ret = 0;
            }
            break;
        case 4:
            if (adventurePouchFindItemSlot(ITEM_STAMINA_POTION) != POUCH_SLOT_NONE ||
                adventurePouchFindItemSlot(ITEM_STAMINA_POTION_PLUS) != POUCH_SLOT_NONE) {
                ret = 0;
            }
            break;
        case 5:
            if (adventurePouchFindItemSlot(ITEM_MUSHROOM_SPORES) != POUCH_SLOT_NONE ||
                adventurePouchFindItemSlot(ITEM_GLITTERING_SPORES) != POUCH_SLOT_NONE) {
                ret = 0;
            }
            break;
        case 6: {
            FiAnalysisHandle handle = FiContext::getNaviTableEquipmentCheckEntry();
            switch (handle.getThreatenedShield()) {
                case 0:
                    if (dAcPy_c::getCurrentlyEquippedShieldType() == 0) {
                        ret = 0;
                    }
                    break;
                case 1:
                    if (dAcPy_c::getCurrentlyEquippedShieldType() == 1) {
                        ret = 0;
                    }
                    break;
            }
            break;
        }
        case 7:
            if (adventurePouchFindItemSlot(ITEM_SACRED_WATER) != POUCH_SLOT_NONE) {
                ret = 0;
            }
            break;
        case 8:
            if (adventurePouchCountItem(ITEM_HEART_MEDAL) != 0) {
                ret = 0;
            }
            break;
        case 9:
            if (hasAnyShields()) {
                ret = 0;
            }
            break;
    }
    return ret;
}

u16 dFlow_c::branchHandler20(const MsbFlowInfo *element) const {
    u16 ret = 0;
    if (itemCheckFindItemSlot(ITEM_NONE) != ITEM_CHECK_SLOT_NONE) {
        ret = 1;
    }
    return ret;
}

u16 dFlow_c::branchHandler21(const MsbFlowInfo *element) const {
    u16 ret = 0;
    if (shouldHideKillCountForActor(FiContext::getTargetActorId())) {
        ret = 1;
    }
    return ret;
}

u16 dFlow_c::branchHandler22(const MsbFlowInfo *element) const {
    u16 ret = StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_692);
    if (ret > 3) {
        ret = 3;
    }
    return ret;
}

dFlow_c::BranchHandler dFlow_c::sBranchHandlers[] = {
    &dFlow_c::branchHandler00, // BRANCH_SELECTED_OPTION_0
    &dFlow_c::branchHandler01, // BRANCH_SELECTED_OPTION_1
    &dFlow_c::branchHandler02, // BRANCH_SELECTED_OPTION_FI
    &dFlow_c::branchHandler03, // BRANCH_STORYFLAG
    &dFlow_c::branchHandler04, // BRANCH_NONE
    &dFlow_c::branchHandler05, // BRANCH_ZONEFLAG
    &dFlow_c::branchHandler06, // BRANCH_SCENEFLAG
    &dFlow_c::branchHandler07, // BRANCH_EVENT_COUNTER_THRESHOLD_1
    &dFlow_c::branchHandler08, // BRANCH_EVENT_COUNTER_THRESHOLD_2
    &dFlow_c::branchHandler09, // BRANCH_TEMPFLAG
    &dFlow_c::branchHandler10, // BRANCH_CURRENT_RUPEES
    &dFlow_c::branchHandler11, // BRANCH_RAND_2
    &dFlow_c::branchHandler12, // BRANCH_RAND_3
    &dFlow_c::branchHandler13, // BRANCH_RAND_4
    &dFlow_c::branchHandler14, // BRANCH_NPC_1
    &dFlow_c::branchHandler15, // BRANCH_NPC_2
    &dFlow_c::branchHandler16, // BRANCH_NPC_3
    &dFlow_c::branchHandler17, // BRANCH_FREE_SPACE_IN_POUCH
    &dFlow_c::branchHandler18, // BRANCH_18
    &dFlow_c::branchHandler19, // BRANCH_19
    &dFlow_c::branchHandler20, // BRANCH_FREE_SPACE_IN_ITEM_CHECK
    &dFlow_c::branchHandler21, // BRANCH_TARGET_ACTOR_HAS_KILL_COUNT
    &dFlow_c::branchHandler22, // BRANCH_22
};

bool dFlow_c::handleBranch() {
    MsbFlowInfo *info = LMS_GetFlowElement(mpMsbf, mCurrentFlowIndex);
    u16 next = getSwitchChoice(info, info->param3);
    u16 *branchPoints = LMS_GetBranchPoints(mpMsbf, mCurrentFlowIndex);
    if (info->param5 != 0xFFFF) {
        setNext(branchPoints[next]);
    } else {
        clear();
    }
    return true;
}

bool dFlow_c::handleEntry() {
    MsbFlowInfo *element = LMS_GetFlowElement(mpMsbf, mCurrentFlowIndex);
    setNext(element->next);
    return true;
}

bool dFlow_c::handleJump() {
    // unimplemented
    return true;
}

void dFlow_c::triggerEntryPoint(s32 labelPart1, s32 labelPart2) {
    if (labelPart1 == 6) {
        // "Your hearts have decreased quite dramatically..."
        // 400 -> may introduce heart dowsing
        // 401 -> no heart dowsing introduction
        if (labelPart2 == 400) {
            FiContext::setField_0x48(true);
            labelPart2 = 401;
        } else if (labelPart2 == 401) {
            StoryflagManager::sInstance->setFlag(STORYFLAG_LOW_HEART_NOTICE);
            // Don't introduce heart dowsing in areas where you might
            // not be able to dowse
            if (dStageMgr_c::GetInstance()->isAreaTypeNormal()) {
                labelPart2 = 400;
            } else {
                labelPart2 = 401;
            }
        } else if (labelPart2 == 402 && (dLytMsgWindow_c::getInstance()->getTagProcessor()->getField_0x8FC() < 0 ||
                                         dLytMsgWindow_c::getInstance()->getTagProcessor()->getField_0x900() < 0)) {
            // "You have elected to engage Hero Mode..."
            FiContext::setField_0x4A(true);
            labelPart2 = 401;
        }
    }

    if (labelPart1 == 6 && labelPart2 == 801) {
        // "You called for me, Master?"
        FiContext::do_prepareFiCallOptions();
        FiContext::setIsInFiMainMenu(true);
        FiContext::do_fn_8016CB40();
        syncFiButtonText(0, 0);
        syncFiButtonText(1, 1);
        syncFiButtonText(2, 2);
        setFiButtonTextDirectly(FiContext::KEN8_Nevermind, 3);
    } else if (labelPart1 == 6 && labelPart2 == 802) {
        // Doesn't seem to exist in the files
        FiContext::setIsInFiMainMenu(true);
        syncFiButtonText(0, 0);
        syncFiButtonText(1, 1);
        syncFiButtonText(2, 2);
        setFiButtonTextDirectly(FiContext::KEN8_Nevermind, 3);
    }
    u16 entry = findEntryPoint(labelPart1, labelPart2);
    start(entry);
}

void dFlow_c::triggerEntryPoint(const char *label) {
    u16 entry = findEntryPoint(label);
    start(entry);
}

u16 dFlow_c::getField_0x44() const {
    return field_0x44;
}

u16 dFlow_c::getNextFiFlow() const {
    return mNextFiFlow;
}

bool dFlow_c::triggerEntryPointChecked(s32 labelPart1, s32 labelPart2) {
    if (checkField0x3C()) {
        s32 old = field_0x40;
        dFlow_c::triggerEntryPoint(labelPart1, labelPart2);
        setNext(old);
        return true;
    }
    return false;
}

void dFlow_c::setField0x3C() {
    field_0x3C = 1;
}

bool dFlow_c::checkField0x3C() const {
    return field_0x3C == 1;
}

extern "C" dFlow_c *CURRENT_ACTOR_EVENT_FLOW_MANAGER;
void dFlow_c::start(u16 entry) {
    field_0x0E = 0;
    field_0x0F = 0;
    field_0x3C = 0;
    field_0x40 = -1;
    field_0x44 = -1;
    mNextFiFlow = -1;
    mDelayTimer = 0;
    std::memset(&mFlowInfo, 0, sizeof(MsbFlowInfo));
    CURRENT_ACTOR_EVENT_FLOW_MANAGER = this;
    field_0x0C = 0xFFFF;
    mCurrentFlowIndex = -1;
    field_0x10 = 1;
    setNext(entry);
}

bool dFlow_c::advanceUntil(s32 searchType, s32 searchParam3, s32 *pOutParams1n2) {
    bool keepGoing = true;
    while (keepGoing && !vt_0x18()) {
        MsbFlowInfo *element = LMS_GetFlowElement(mpMsbf, mCurrentFlowIndex);
        s32 type = element->type;
        if (searchType == type) {
            if (element->type == FLOW_EVENT) {
                if (element->param3 == searchParam3) {
                    if (pOutParams1n2 != nullptr) {
                        *pOutParams1n2 = element->params1n2;
                    }
                    return true;
                }
            } else {
                return true;
            }
        }
        switch (type) {
            case FLOW_MESSAGE:
                // Skip text processing, simply advance
                setNext(element->next);
                keepGoing = true;
                break;
            case FLOW_BRANCH:
                if (element->param3 <= BRANCH_SELECTED_OPTION_FI) {
                    // Skip evaluating if next branch depends on user input
                    keepGoing = false;
                } else {
                    // Otherwise
                    keepGoing = handleBranch();
                }
                break;
            case FLOW_EVENT:
                switch (element->param3) {
                    case EVENT_LOAD_FI_FLOW:
                    case EVENT_COUNTER_THRESHOLD:
                    case 27:                      keepGoing = handleEvent(); continue;
                    case EVENT_END_MAP_EVENT:
                        if (element->params1n2 == 2) {
                            keepGoing = false;
                            continue;
                        }
                        // fall-through
                    default:
                        setNext(element->next);
                        keepGoing = true;
                        break;
                }
                break;
            case FLOW_ENTRY: keepGoing = handleEntry(); break;
            case FLOW_JUMP:  keepGoing = handleJump(); break;
        }
    }
    return 0;
}

// TODO: Used KEN6 as reference, will be set from dAcBase_c::targetFiTextId (0x114)
static const s32 sActorsWithoutKillCount[] = {
    0,  // Remlit (Day)
    1,  // Remlit (Night)
    10, // Moldarach
    46, // Stalmaster
    47, // LD-002S Scervo
    48, // Koloktos
    49, // Ghirahim (G1)
    50, // Ghirahim (G2)
    51, // Ghirahim (G3.1)
    52, // The Imprisoned
    53, // The Imprisoned
    54, // The Imprisoned
    55, // Demise
    65, // Scaldera
    67, // <?>
    77, // Tentalus
    81, // Peahat
    87, // Sentrobe Bomb
    88, // LD-003D Dreadfuse
    89, // Ghirahim (G3.2)
    90, // Ghirahim (G3.3)
    -1  // <end of array>
};

bool dFlow_c::shouldHideKillCountForActor(s32 id) const {
    for (s32 i = 0; sActorsWithoutKillCount[i] >= 0; i++) {
        if (sActorsWithoutKillCount[i] == id) {
            return true;
        }
    }
    return false;
}

void dFlow_c::clear() {
    mCurrentFlowIndex = -1;
    field_0x0E = 1;
    field_0x3C = 0;
    field_0x40 = -1;
    field_0x44 = -1;
}

void dFlow_c::createLytMiniGame() {
    if (dLytMiniGame_c::GetInstance() == nullptr) {
        switch (dMessage_c::getInstance()->getMiniGameVariant()) {
            case dMessage_c::MG_THRILL_DIGGER:
                dBase_c::createBase(
                    fProfile::LYT_MINI_GAME, dLytControlGame_c::getInstance(),
                    dLytMiniGame_c::VARIANT_THRILL_DIGGER_RESULT, fBase_c::OTHER
                );
                break;
            case dMessage_c::MG_BAMBOO_CUTTING:
                dBase_c::createBase(
                    fProfile::LYT_MINI_GAME, dLytControlGame_c::getInstance(),
                    dLytMiniGame_c::VARIANT_BAMBOO_CUTTING_RESULT, fBase_c::OTHER
                );
                break;
            case dMessage_c::MG_INSECT_CAPTURE:
                dBase_c::createBase(
                    fProfile::LYT_MINI_GAME, dLytControlGame_c::getInstance(),
                    dLytMiniGame_c::VARIANT_INSECT_CAPTURE_RESULT, fBase_c::OTHER
                );
                break;
            case dMessage_c::MG_ROLLERCOASTER:
                dBase_c::createBase(
                    fProfile::LYT_MINI_GAME, dLytControlGame_c::getInstance(),
                    dLytMiniGame_c::VARIANT_ROLLERCOASTER_RESULT, fBase_c::OTHER
                );
                break;
            case dMessage_c::MG_PUMPKIN_ARCHERY:
                dBase_c::createBase(
                    fProfile::LYT_MINI_GAME, dLytControlGame_c::getInstance(),
                    dLytMiniGame_c::VARIANT_PUMPKIN_ARCHERY_RESULT, fBase_c::OTHER
                );
                break;
            case dMessage_c::MG_FUN_FUN_ISLAND:
                dBase_c::createBase(
                    fProfile::LYT_MINI_GAME, dLytControlGame_c::getInstance(), dLytMiniGame_c::VARIANT_FUN_FUN_ISLAND,
                    fBase_c::OTHER
                );
                break;
            case dMessage_c::MG_TRIAL_TIME_ATTACK:
                dBase_c::createBase(
                    fProfile::LYT_MINI_GAME, dLytControlGame_c::getInstance(),
                    dLytMiniGame_c::VARIANT_TRIAL_TIME_ATTACK_RESULT, fBase_c::OTHER
                );
                break;
            case dMessage_c::MG_BOSS_RUSH:
                dBase_c::createBase(
                    fProfile::LYT_MINI_GAME, dLytControlGame_c::getInstance(), dLytMiniGame_c::VARIANT_BOSS_RUSH_RESULT,
                    fBase_c::OTHER
                );
                break;
        }
    }
}

void dFlow_c::clearMinigame() {
    if (dMessage_c::getInstance()->hasSetMinigameResult()) {
        switch (dMessage_c::getInstance()->getMiniGameVariant()) {
            case dMessage_c::MG_THRILL_DIGGER:
            case dMessage_c::MG_BAMBOO_CUTTING:
            case dMessage_c::MG_PUMPKIN_ARCHERY:
            case dMessage_c::MG_FUN_FUN_ISLAND:
                if (dLytMiniGame_c::GetInstance() != nullptr) {
                    dLytMiniGame_c::GetInstance()->endScoreResult();
                }
                break;
            case dMessage_c::MG_INSECT_CAPTURE:
            case dMessage_c::MG_ROLLERCOASTER:
            case dMessage_c::MG_TRIAL_TIME_ATTACK:
            case dMessage_c::MG_BOSS_RUSH:
                if (dLytMiniGame_c::GetInstance() != nullptr) {
                    dLytMiniGame_c::GetInstance()->endTimeResult();
                }
                break;
        }
    }
    dMessage_c::getInstance()->setHasSetMinigameResult(false);
    dMessage_c::getInstance()->setMiniGameVariant(dMessage_c::MG_NONE);
}

SPECIAL_BASE_PROFILE(MESSAGE, dMessage_c, fProfile::MESSAGE, 0x2A8, 0);

static const char *sMsbtFileNames[82] = {
    "000-Test.msbt",
    "001-Action.msbt",
    "002-System.msbt",
    "003-ItemGet.msbt",
    "004-Object.msbt",
    "005-Tutorial.msbt",
    "006-KenseiNormal.msbt",
    "006-1KenseiNormal.msbt",
    "006-2KenseiNormal.msbt",
    "006-3KenseiNormal.msbt",
    "006-4KenseiNormal.msbt",
    "006-5KenseiNormal.msbt",
    "006-6KenseiNormal.msbt",
    "006-7KenseiNormal.msbt",
    "006-8KenseiNormal.msbt",
    "006-9KenseiNormal.msbt",
    "007-MapText.msbt",
    "008-Hint.msbt",
    "word.msbt",
    "100-Town.msbt",
    "101-Shop.msbt",
    "102-Zelda.msbt",
    "103-DaiShinkan.msbt",
    "104-Rival.msbt",
    "105-Terry.msbt",
    "106-DrugStore.msbt",
    "107-Kanban.msbt",
    "108-ShinkanA.msbt",
    "109-TakeGoron.msbt",
    "110-DivingGame.msbt",
    "111-FortuneTeller.msbt",
    "112-Trustee.msbt",
    "113-RemodelStore.msbt",
    "114-Friend.msbt",
    "115-Town2.msbt",
    "116-InsectGame.msbt",
    "117-Pumpkin.msbt",
    "118-Town3.msbt",
    "119-Captain.msbt",
    "120-Nushi.msbt",
    "121-AkumaKun.msbt",
    "122-Town4.msbt",
    "123-Town5.msbt",
    "124-Town6.msbt",
    "125-D3.msbt",
    "150-Siren.msbt",
    "198-Test.msbt",
    "199-Demo.msbt",
    "200-Forest.msbt",
    "201-ForestD1.msbt",
    "202-ForestD2.msbt",
    "203-ForestF2.msbt",
    "204-ForestF3.msbt",
    "250-ForestSiren.msbt",
    "251-Salvage.msbt",
    "299-Demo.msbt",
    "300-Mountain.msbt",
    "301-MountainD1.msbt",
    "302-Anahori.msbt",
    "303-MountainF2.msbt",
    "304-MountainD2.msbt",
    "305-MountainF3.msbt",
    "350-MountainSiren.msbt",
    "351-Salvage.msbt",
    "399-Demo.msbt",
    "400-Desert.msbt",
    "401-DesertD2.msbt",
    "402-DesertF2.msbt",
    "403-DesertD1.msbt",
    "404-DesertF3.msbt",
    "405-DesertD2Clear.msbt",
    "406-TrolleyRace.msbt",
    "450-DesertSiren.msbt",
    "451-Salvage.msbt",
    "460-RairyuMinigame.msbt",
    "499-Demo.msbt",
    "500-CenterField.msbt",
    "501-Inpa.msbt",
    "502-CenterFieldBack.msbt",
    "503-Goron.msbt",
    "510-Salvage.msbt",
    "599-Demo.msbt",
};

static char *sMsbfFileNames[80] = {
    "000-Test.msbf",
    "001-Action.msbf",
    "002-System.msbf",
    "003-ItemGet.msbf",
    "004-Object.msbf",
    "005-Tutorial.msbf",
    "006-KenseiNormal.msbf",
    "006-1KenseiNormal.msbf",
    "006-2KenseiNormal.msbf",
    "006-3KenseiNormal.msbf",
    "006-4KenseiNormal.msbf",
    "006-5KenseiNormal.msbf",
    "006-6KenseiNormal.msbf",
    "006-7KenseiNormal.msbf",
    "006-8KenseiNormal.msbf",
    "006-9KenseiNormal.msbf",
    "008-Hint.msbf",
    "100-Town.msbf",
    "101-Shop.msbf",
    "102-Zelda.msbf",
    "103-DaiShinkan.msbf",
    "104-Rival.msbf",
    "105-Terry.msbf",
    "106-DrugStore.msbf",
    "107-Kanban.msbf",
    "108-ShinkanA.msbf",
    "109-TakeGoron.msbf",
    "110-DivingGame.msbf",
    "111-FortuneTeller.msbf",
    "112-Trustee.msbf",
    "113-RemodelStore.msbf",
    "114-Friend.msbf",
    "115-Town2.msbf",
    "116-InsectGame.msbf",
    "117-Pumpkin.msbf",
    "118-Town3.msbf",
    "119-Captain.msbf",
    "120-Nushi.msbf",
    "121-AkumaKun.msbf",
    "122-Town4.msbf",
    "123-Town5.msbf",
    "124-Town6.msbf",
    "125-D3.msbf",
    "150-Siren.msbf",
    "198-Test.msbf",
    "199-Demo.msbf",
    "200-Forest.msbf",
    "201-ForestD1.msbf",
    "202-ForestD2.msbf",
    "203-ForestF2.msbf",
    "204-ForestF3.msbf",
    "250-ForestSiren.msbf",
    "251-Salvage.msbf",
    "299-Demo.msbf",
    "300-Mountain.msbf",
    "301-MountainD1.msbf",
    "302-Anahori.msbf",
    "303-MountainF2.msbf",
    "304-MountainD2.msbf",
    "305-MountainF3.msbf",
    "350-MountainSiren.msbf",
    "351-Salvage.msbf",
    "399-Demo.msbf",
    "400-Desert.msbf",
    "401-DesertD2.msbf",
    "402-DesertF2.msbf",
    "403-DesertD1.msbf",
    "404-DesertF3.msbf",
    "405-DesertD2Clear.msbf",
    "406-TrolleyRace.msbf",
    "450-DesertSiren.msbf",
    "451-Salvage.msbf",
    "460-RairyuMinigame.msbf",
    "499-Demo.msbf",
    "500-CenterField.msbf",
    "501-Inpa.msbf",
    "502-CenterFieldBack.msbf",
    "503-Goron.msbf",
    "510-Salvage.msbf",
    "599-Demo.msbf",
};

dMessage_c *dMessage_c::sInstance;
dTagProcessor_c *dMessage_c::sTagProcessor;

static void *msbAlloc(size_t size) {
    if (dLyt_HIO_c::getInstance()->getShouldHookAllocations() == false) {
        return EGG::Heap::alloc(size, 0x20, nullptr);
    } else {
        return dLyt_HIO_c::getInstance()->allocUnk(size, 0x20);
    }
}

static void msbFree(void *ptr) {
    if (dLyt_HIO_c::getInstance()->getShouldHookAllocations() == false) {
        EGG::Heap::free(ptr, nullptr);
    } else {
        dLyt_HIO_c::getInstance()->destroyUnk(ptr);
    }
}

dMessage_c::dMessage_c() {
    sInstance = this;
}

int dMessage_c::create() {
    mLanguage = getLanguageIdentifier();

    mCurrentTextFileNumber = -1;
    field_0x2FC = 0;
    field_0x344 = 0;
    LMS_SetMemFuncs(msbAlloc, msbFree);
    setZevFromMsbArc();

    for (s32 i = 0; i < 82; i++) {
        SizedString<0x40> fileName = getMsbtFileName(i);
        s32 num = getMsbtNumberByIndex(i);
        void *data = getDataFromMsbArc(num, fileName, true);
        if (data == nullptr) {
            mpMsgs[i] = nullptr;
        } else {
            mpMsgs[i] = LMS_InitMessage(data);
        }
    }

    for (s32 i = 0; i < 80; i++) {
        SizedString<0x40> fileName = getMsbfFileName(i);
        s32 num = getMsbfNumberByIndex(i);
        void *data = getDataFromMsbArc(num, fileName, true);
        if (data == nullptr) {
            mpFlows[i] = nullptr;
        } else {
            mpFlows[i] = LMS_InitFlow(data);
        }
    }

    sTagProcessor = new dTagProcessor_c();
    FiContext::create();
    reset();
    return SUCCEEDED;
}

int dMessage_c::doDelete() {
    dLyt_HIO_c::getInstance()->destroy();
    if (sTagProcessor != nullptr) {
        delete sTagProcessor;
        sTagProcessor = nullptr;
    }

    for (int i = 0; i < 82; i++) {
        if (mpMsgs[i] != nullptr) {
            LMS_CloseMessage(mpMsgs[i]);
        }
    }

    for (int i = 0; i < 80; i++) {
        if (mpFlows[i] != nullptr) {
            LMS_CloseFlow(mpFlows[i]);
        }
    }
    return SUCCEEDED;
}

int dMessage_c::execute() {
    if (field_0x2FC != 0 && !EventManager::isInEvent()) {
        field_0x2FC = 0;
    }

    if (field_0x2FC > 0) {
        field_0x2FC--;
    }
    executeMinigame();
    sTagProcessor->execute();

    return SUCCEEDED;
}

int dMessage_c::draw() {
    return SUCCEEDED;
}

const wchar_t *dMessage_c::getTextMessageByLabel(const char *label, bool global, wchar_t *dstBuf, u32 maxLen) {
    return sInstance->getTextMessageByLabelInternal(label, nullptr, global, dstBuf, maxLen);
}

const wchar_t *dMessage_c::getTextMessageByLabel(
    const char *label, dTagProcessor_c *pTagProcessor, bool global, wchar_t *dstBuf, u32 maxLen
) {
    return sInstance->getTextMessageByLabelInternal(label, pTagProcessor, global, dstBuf, maxLen);
}

const wchar_t *dMessage_c::formatText(const wchar_t *text) {
    return sInstance->formatTextInternal(text);
}

const wchar_t *dMessage_c::getTextMessageByLabelInternal(
    const char *label, dTagProcessor_c *pTagProcessor, bool global, wchar_t *dstBuf, u32 maxLen
) {
    s32 fileIndex = mCurrentTextFileNumber;
    MsbtInfo *info = nullptr;

    if (global) {
        fileIndex = getMsbtIndexForLabelInternal(label);
    }

    if (fileIndex >= 0) {
        info = getMsbtInfoForIndex(fileIndex);
    }

    const wchar_t *text = LMS_GetTextByLabel(info, label);
    if (pTagProcessor == nullptr) {
        pTagProcessor = sTagProcessor;
    }

    u32 outLen = 0;
    s32 textIdx = LMS_GetTextIndexByLabel(info, label);
    MsbtAttrInfo *att = LMS_GetAttribute(info, textIdx);
    pTagProcessor->setMsgWindowSubtype(att->c_0x00);
    pTagProcessor->setField_0x90D(att->c_0x01);

    // Strip trailing newline
    wchar_t *end;
    s32 i;

    if (dstBuf != nullptr) {
        pTagProcessor->format(nullptr, text, dstBuf, maxLen, &outLen, nullptr);
        i = outLen - 1;
        end = &dstBuf[i];
        for (; i > 0; i--) {
            if (*end != L'\n') {
                break;
            }
            *end-- = L'\0';
        }
        // no return - might be intentional since dstBuf already has the result
    } else {
        static wchar_t sBuf[0x400] = {};
        pTagProcessor->format(nullptr, text, sBuf, ARRAY_LENGTH(sBuf) - 1, &outLen, nullptr);
        i = outLen - 1;
        end = &sBuf[i];
        for (; i > 0; i--) {
            if (*end != L'\n') {
                break;
            }
            *end = L'\0';
            end--;
        }

        return sBuf;
    }

    return nullptr;
}

const wchar_t *dMessage_c::formatTextInternal(const wchar_t *text) {
    u32 outLen = 0;
    static wchar_t sBuf[0x200] = {};
    sTagProcessor->format(nullptr, text, sBuf, ARRAY_LENGTH(sBuf), &outLen, nullptr);
    s32 i = outLen - 1;
    wchar_t *end = &sBuf[i];
    for (; i > 0; i--) {
        if (*end != L'\n') {
            break;
        }
        *end = L'\0';
        end--;
    }

    return sBuf;
}

bool dMessage_c::isValidTextLabel(const char *name) {
    return sInstance->checkIsValidTextLabel(name);
}

bool dMessage_c::checkIsValidTextLabel(const char *name) {
    if (name == nullptr) {
        return false;
    }
    return getTextIndexForLabel(name) >= 0;
}

const char *dMessage_c::getMsbtFileName(s32 idx) {
    static SizedString<128> sPath;

    s32 arcIdx = getArcIndexForFile(sMsbtFileNames[idx]);
    const char *arcName = getArcNameByIndex(arcIdx, true);
    sPath.sprintf("%s/%s", arcName, sMsbtFileNames[idx]);
    return sPath;
}

const char *dMessage_c::getMsbfFileName(s32 idx) {
    static SizedString<128> sPath;

    s32 arcIdx = getArcIndexForFile(sMsbfFileNames[idx]);
    const char *arcName = getArcNameByIndex(arcIdx, true);
    sPath.sprintf("%s/%s", arcName, sMsbfFileNames[idx]);
    return sPath;
}

s32 dMessage_c::getMsbtNumberByIndex(s32 index) {
    return atoi(getMsbtFileName(index));
}

s32 dMessage_c::getMsbfNumberByIndex(s32 index) {
    return atoi(getMsbfFileName(index));
}

// Skipping 007-MapText.msbt and word.msbt
static const s32 sMsbfToMsbt[] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55,
    56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
};

s32 dMessage_c::getMsbtIndexForMsbfIndex(s32 index) {
    return sMsbfToMsbt[index];
}

s32 dMessage_c::getTextIndexForLabel(const char *label) {
    s32 idx = getMsbtIndexForLabelInternal(label);
    MsbtInfo *info = nullptr;
    if (idx >= 0) {
        info = getMsbtInfoForIndex(idx);
    }
    return LMS_GetTextIndexByLabel(info, label);
}

s32 dMessage_c::getMsbtIndexForLabel(const char *label) {
    return sInstance->getMsbtIndexForLabelInternal(label);
}

s32 dMessage_c::getMsbtIndexForLabelInternal(const char *label) {
    if (mCurrentTextFileNumber <= 81) {
        MsbtInfo *info = getMsbtInfoForIndex(mCurrentTextFileNumber);
        if (LMS_GetTextIndexByLabel(info, label) >= 0) {
            return mCurrentTextFileNumber;
        }
    }

    for (s32 i = 0; i < 82; i++) {
        if (getMsbtInfoForIndex(i) != nullptr) {
            if (LMS_GetTextIndexByLabel(getMsbtInfoForIndex(i), label) >= 0) {
                return i;
            }
        }
    }

    return -1;
}

MsbtInfo *dMessage_c::getMsbtInfoForIndex(s32 index) {
    return sInstance->getMsbtInfoForIndexInternal(index);
}

MsbtInfo *dMessage_c::getMsbtInfoForIndexInternal(s32 index) {
    dLyt_HIO_c *thing = dLyt_HIO_c::getInstance();
    MsbtInfo *overrideMsbt = thing->getOverrideMsbtInfo(index);
    if (overrideMsbt != nullptr && thing->getShouldOverrideData() == true) {
        return overrideMsbt;
    }
    return mpMsgs[index];
}

MsbfInfo *dMessage_c::getMsbfInfoForIndex(s32 index) {
    return sInstance->getMsbfInfoForIndexInternal(index);
}

MsbfInfo *dMessage_c::getMsbfInfoForIndexInternal(s32 index) {
    dLyt_HIO_c *thing = dLyt_HIO_c::getInstance();
    MsbfInfo *overrideMsbf = thing->getOverrideMsbfInfo(index);
    if (overrideMsbf != nullptr && thing->getShouldOverrideData() == true) {
        return overrideMsbf;
    }
    return mpFlows[index];
}

u32 dMessage_c::getLightPillarRelatedArg(s32 arg) {
    return field_0x300[arg];
}

void dMessage_c::storeLightPillarRelatedArg(u32 arg) {
    for (s32 i = 0; i < ARRAY_LENGTH(field_0x300); i++) {
        if (field_0x300[i] == 0xFFFFFFFF) {
            field_0x300[i] = arg;
            return;
        }
    }
}

void dMessage_c::clearLightPillarRelatedArgs() {
    for (s32 i = 0; i < ARRAY_LENGTH(field_0x300); i++) {
        field_0x300[i] = 0xFFFFFFFF;
    }
}

void dMessage_c::executeMinigame() {
    if (mMinigameVariant == MG_NONE) {
        return;
    }
    if (!dMessage_c::getInstance()->hasSetMinigameResult()) {
        switch (mMinigameVariant) {
            case MG_THRILL_DIGGER:
            case MG_BAMBOO_CUTTING:
            case MG_PUMPKIN_ARCHERY:
            case MG_FUN_FUN_ISLAND:
                if (dLytMiniGame_c::GetInstance() != nullptr) {
                    dLytMiniGame_c::GetInstance()->scoreRelatedInit();
                    dLytMiniGame_c::GetInstance()->setDisplayedPoints(mMinigameResultPoints);
                    sInstance->mHasSetMinigameResult = true;
                }
                break;
            case MG_INSECT_CAPTURE:
            case MG_ROLLERCOASTER:
            case MG_TRIAL_TIME_ATTACK:
            case MG_BOSS_RUSH:
                if (dLytMiniGame_c::GetInstance() != nullptr) {
                    dLytMiniGame_c::GetInstance()->timeRelatedInit();
                    dLytMiniGame_c::GetInstance()->setDisplayedTime(mMinigameTime);
                    sInstance->mHasSetMinigameResult = true;
                }
                break;
        }
    }
}

void dMessage_c::init() {
    clearLightPillarRelatedArgs();
    // Probably inlines
    mInMapEvent = false;
    field_0x329 = 0;
    field_0x32A = 0;
    sInstance->setMapEvent(dLytMapMain_c::MAP_EVENT_MAX);
    field_0x330 = 0;
}

void dMessage_c::reset() {
    init();
    mMinigameVariant = MG_NONE;
    mHasSetMinigameResult = false;
    mMinigameResultPoints = 0;
    mMinigameTime = 0;
}

static SizedString<8> sCurrentLanguage;
const char *dMessage_c::getLanguageIdentifier() {
    u8 lang = getCurrentLanguage1();
    if (lang == D_LANG_FR) {
        sCurrentLanguage = "fr_US";
    } else if (lang == D_LANG_ES) {
        sCurrentLanguage = "es_US";
    } else {
        sCurrentLanguage = "en_US";
    }
    return sCurrentLanguage;
}

const char *sLytMsbts_Unused[] = {
    "basic/remoConBtn_00.msbt",
    "basic/remoConBtn_01.msbt",
    "basic/remoConBtn_03.msbt",
    "basic/remoConBtn_04.msbt",
    "basic/remoConBtn_05.msbt",
    "basic/nunBtn_03.msbt",
    "common/commonTitle_00.msbt",
    "endroll/endScroll_00.msbt",
    "fileSelect/fileSelect_00.msbt",
    "information/skip_00.msbt",
    "map/map_00.msbt",
    "messageWindow/messageBtn_00.msbt",
    "miniGame/start_00.msbt",
    "miniGameScore/miniGameScore_00.msbt",
    "miniGameTime/miniGameTime_00.msbt",
    "pause/pauseInfo_00.msbt",
    "pause/pause_00.msbt",
    "shop/depositBox_00.msbt",
    "shop/itemSelect_00.msbt",
    "shop/materialCheck_00.msbt",
    "softwareKeyboard/toolbarBtn_00.msbt",
    "systemWindow/systemWindow_00.msbt",
    "title/titleBG_00.msbt",
};

const char *dMessage_c::getArcNameByIndex(s32 idx, bool global) {
    return getArcNameByIndexInternal(idx, global);
}

static char *sArcNames[] = {
    "0-Common", "1-Town", "2-Forest", "3-Mountain", "4-Desert", "5-CenterField",
};

const char *dMessage_c::getArcNameByIndexInternal(s32 idx, bool global) {
    return sArcNames[idx];
}

void *dMessage_c::getDataFromMsbArc(s32 number, const char *fileName, bool global) {
    SizedString<128> path;
    path.sprintf("%s/%s", getLanguageIdentifier(), getArcNameByIndex(number, global));
    return OarcManager::GetInstance()->getData(path, fileName);
}

void dMessage_c::setZevFromMsbArc() {
    SizedString<128> path;
    path.sprintf("%s/dat/zev.dat", getArcNameByIndex(0, false));
    sZev0 = getDataFromMsbArc(0, path, false);
    sZevStage = nullptr;
}

void dMessage_c::setStageZevFromMsbArc() {
    SizedString<32> stage = dScGame_c::currentSpawnInfo.stageName;
    char buf[2];
    buf[0] = stage[1];
    buf[1] = '\0';

    SizedString<128> path;

    int i = atoi(buf) + 1;

    // TODO figure out what this is
    if (stage == "t_tkm24" || stage == "t_tkm26") {
        i = 2;
    } else if (i < 1 || 6 <= i) {
        i = 0;
    }
    path.sprintf("%s/dat/zev.dat", getArcNameByIndex(i, false));
    sZevStage = getDataFromMsbArc(i, path, false);
}

void *dMessage_c::sZev0;
void *dMessage_c::sZevStage;

void *dMessage_c::getZev0Internal() {
    dLyt_HIO_c *thing = dLyt_HIO_c::getInstance();
    void *overrideZev = thing->getOverrideZev0();
    if (overrideZev != nullptr && thing->getShouldOverrideData() == true) {
        return overrideZev;
    }
    return sZev0;
}

void *dMessage_c::getZevStageInternal() {
    dLyt_HIO_c *thing = dLyt_HIO_c::getInstance();
    void *overrideZev = thing->getOverrideStageZev();
    if (overrideZev != nullptr && thing->getShouldOverrideData() == true) {
        return overrideZev;
    }
    return sZevStage;
}

s32 dMessage_c::getArcIndexForFile(const char *fileName) {
    char buf[2];
    buf[0] = fileName[0];
    buf[1] = '\0';
    return atoi(buf);
}
