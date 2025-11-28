#include "d/lyt/d_lyt_control_game.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/d_a_player.h"
#include "d/d_base.h"
#include "d/d_cs_game.h"
#include "d/d_d2d.h"
#include "d/d_heap.h"
#include "d/d_message.h"
#include "d/d_rawarchive.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_draw_mark.h"
#include "d/lyt/d_lyt_help.h"
#include "d/lyt/d_lyt_map.h"
#include "d/lyt/d_lyt_map_global.h"
#include "d/lyt/d_lyt_pause.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "d/snd/d_snd_player_mgr.h"
#include "d/snd/d_snd_source_mgr.h"
#include "d/t/d_t_map_mark.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m2d.h"
#include "m/m_vec.h"
#include "toBeSorted/arc_managers/layout_arc_manager.h"
#include "toBeSorted/d_beacon.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/raii_ptr.h"

class dLytControlGame_HIO_c {
public:
    dLytControlGame_HIO_c();
    virtual ~dLytControlGame_HIO_c();

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ s16 field_0x0C;
    /* 0x0E */ u8 field_0x0E;
    /* 0x0F */ u8 field_0x0F;
};

dLytControlGame_HIO_c sHio;

dLytControlGame_HIO_c::dLytControlGame_HIO_c() : field_0x04(300.0f), field_0x08(1.0f), field_0x0E(1), field_0x0F(0) {
    field_0x0C = 0x1F4;
}
dLytControlGame_HIO_c::~dLytControlGame_HIO_c() {}

SPECIAL_BASE_PROFILE(LYT_CONTROL_GAME, dLytControlGame_c, fProfile::LYT_CONTROL_GAME, 0x2A9, 0x19);

dLytControlGame_c *dLytControlGame_c::sInstance;

STATE_DEFINE(dLytControlGame_c, Normal);
STATE_DEFINE(dLytControlGame_c, Pause_SetRes);
STATE_DEFINE(dLytControlGame_c, Pause_SyncRes);
STATE_DEFINE(dLytControlGame_c, Pause);
STATE_DEFINE(dLytControlGame_c, Pause_DelRes);
STATE_DEFINE(dLytControlGame_c, Map_SetRes);
STATE_DEFINE(dLytControlGame_c, Map_SyncRes);
STATE_DEFINE(dLytControlGame_c, Map);
STATE_DEFINE(dLytControlGame_c, Map_DelRes);

void dLytControlGame_c::initializeState_Normal() {}
void dLytControlGame_c::executeState_Normal() {}
void dLytControlGame_c::finalizeState_Normal() {}

static const char *const sMenuPause = "MenuPause";
static const char *const sMap2D = "Map2D";
static const char *const sMapField = "MapField";
static const char *const sMapSky = "MapSky";
static const char *const sCommonArrow = "CommonArrow";

void dLytControlGame_c::initializeState_Pause_SetRes() {
    dLytMeter_c::GetInstance()->setMeterMode(dLytMeterMain_c::MODE_PAUSE_INIT);
    dBase_c::s_NextExecuteControlFlags |= BASE_PROP_0x1;
    static const char *sLytArcs[] = {sMenuPause, sCommonArrow};
    mLytArcControl.set(sLytArcs, ARRAY_LENGTH(sLytArcs));
}
void dLytControlGame_c::executeState_Pause_SetRes() {
    mLytArcControl.load(nullptr);
    mStateMgr.changeState(StateID_Pause_SyncRes);
}
void dLytControlGame_c::finalizeState_Pause_SetRes() {}

void dLytControlGame_c::initializeState_Pause_SyncRes() {}
void dLytControlGame_c::executeState_Pause_SyncRes() {
    if (!mLytArcControl.allLoaded()) {
        return;
    }
    if (mpHelp != nullptr) {
        return;
    }
    mStateMgr.changeState(StateID_Pause);
}
void dLytControlGame_c::finalizeState_Pause_SyncRes() {}

void dLytControlGame_c::initializeState_Pause() {
    dBase_c::createBase(fProfile::LYT_PAUSE, this, 0, OTHER);
    if (dLytPauseMgr_c::GetInstance() == nullptr) {
        return;
    }
    dLytPauseMgr_c::GetInstance()->inRequest();
    if (dCsGame_c::GetInstance()->shouldDraw()) {
        dCsGame_c::GetInstance()->offNextCursor();
    }
    mCommand = CMD_NONE;
}
void dLytControlGame_c::executeState_Pause() {
    if (dLytPauseMgr_c::GetInstance() != nullptr && !dLytPauseMgr_c::GetInstance()->isActive()) {
        mStateMgr.changeState(StateID_Pause_DelRes);
    }
}
void dLytControlGame_c::finalizeState_Pause() {}

void dLytControlGame_c::initializeState_Pause_DelRes() {}
void dLytControlGame_c::executeState_Pause_DelRes() {
    if (!mLytArcControl.release()) {
        return;
    }
    if (dLytPauseMgr_c::GetInstance() != nullptr) {
        dLytPauseMgr_c::GetInstance()->deleteRequest();
    }
    dSndSourceMgr_c::GetInstance()->fn_803852F0();
    mStateMgr.changeState(StateID_Normal);
}
void dLytControlGame_c::finalizeState_Pause_DelRes() {
    dLytMeter_c::GetInstance()->setMeterMode(dLytMeterMain_c::MODE_NONE);
}

void dLytControlGame_c::initializeState_Map_SetRes() {
    // Not sure if this is the right enum
    if (!EventManager::isInEvent() || mMapEvent == dLytMapMain_c::MAP_EVENT_11) {
        dBase_c::s_NextExecuteControlFlags |= BASE_PROP_0x10;
    }
    dLytMeter_c::GetInstance()->setMeterMode(dLytMeterMain_c::MODE_MAP_INIT);
}
void dLytControlGame_c::executeState_Map_SetRes() {
    LayoutArcManager::GetInstance()->loadLayoutArcFromDisk(sMap2D, nullptr);
    if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
        LayoutArcManager::GetInstance()->loadLayoutArcFromDisk(sMapSky, nullptr);
    } else {
        LayoutArcManager::GetInstance()->loadLayoutArcFromDisk(sMapField, nullptr);
    }
    mStateMgr.changeState(StateID_Map_SyncRes);
}
void dLytControlGame_c::finalizeState_Map_SetRes() {}

void dLytControlGame_c::initializeState_Map_SyncRes() {}
void dLytControlGame_c::executeState_Map_SyncRes() {
    dArcResult_e result1 = LayoutArcManager::GetInstance()->ensureLoaded1(sMap2D);
    dArcResult_e result2;
    if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
        result2 = LayoutArcManager::GetInstance()->ensureLoaded1(sMapSky);
    } else {
        result2 = LayoutArcManager::GetInstance()->ensureLoaded1(sMapField);
    }
    if (result1 == D_ARC_RESULT_OK && result2 == D_ARC_RESULT_OK) {
        mStateMgr.changeState(StateID_Map);
    }
}
void dLytControlGame_c::finalizeState_Map_SyncRes() {}

void dLytControlGame_c::setMapEventDone(bool b) {
    mpMap->setMapEventDone(b);
}

void dLytControlGame_c::initializeState_Map() {
    mpMap = new (dHeap::layoutEx2Heap.heap) dLytMap_c();
    mpMap->build();
    mpMap->startMapEvent(mMapEvent, mMapEventArg1, mMapEventArg2);
    dLytMapGlobal_c::GetInstance()->setInMapEvent(mMapEvent != dLytMapMain_c::MAP_EVENT_NONE);
}
void dLytControlGame_c::executeState_Map() {
    if (dLytMap_c::GetInstance()->isOpen()) {
        if (dLytMeter_c::GetInstance()->getMeterMode() != dLytMeterMain_c::MODE_MAP) {
            dLytMeter_c::GetInstance()->setMeterMode(dLytMeterMain_c::MODE_MAP);
            if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_PLUS_BTN_NOTICE)) {
                StoryflagManager::sInstance->unsetFlag(STORYFLAG_PLUS_BTN_NOTICE);
            }
        }
        if (dLytMeter_c::GetInstance()->getBasicPosition() != dLytMeterMain_c::POSITION_MAP) {
            dLytMeter_c::GetInstance()->setBasicPosition(dLytMeterMain_c::POSITION_MAP);
        }
    }

    if (mpMap->checkClose()) {
        mpMap->close();
        if (dMessage_c::getInstance()->getField_0x330()) {
            checkForBeaconNextToMark();
        }
        mStateMgr.changeState(StateID_Map_DelRes);
    }

    if (mpMap->isMapIntroDone()) {
        mStateMgr.changeState(StateID_Map_DelRes);
    }
    mpMap->execute();
}
void dLytControlGame_c::finalizeState_Map() {}

void dLytControlGame_c::initializeState_Map_DelRes() {}
void dLytControlGame_c::executeState_Map_DelRes() {
    // Maybe an inline
    if (dLytMap_c::GetInstance()->isVisible()) {
        mpMap->execute();
        return;
    }

    dArcResult_e result1 = LayoutArcManager::GetInstance()->ensureLoaded1(sMap2D);
    dArcResult_e result2;

    if (result1 != D_ARC_RESULT_ERROR_NOT_FOUND) {
        if (result1 != D_ARC_RESULT_OK) {
            return;
        }
        if (LayoutArcManager::GetInstance()->decrement(sMap2D)) {
            if (mpMap != nullptr) {
                mpMap->remove();
                mpMap.destruct();
                m2d::setLytAllocator();
            }
            dSndSourceMgr_c::GetInstance()->fn_803852F0();
            mStateMgr.changeState(StateID_Normal);
        }
    }

    if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
        result2 = LayoutArcManager::GetInstance()->ensureLoaded1(sMapSky);
    } else {
        result2 = LayoutArcManager::GetInstance()->ensureLoaded1(sMapField);
    }

    if (result2 != D_ARC_RESULT_ERROR_NOT_FOUND) {
        if (result2 != D_ARC_RESULT_OK) {
            return;
        }
        if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
            LayoutArcManager::GetInstance()->decrement(sMapSky);
        } else {
            LayoutArcManager::GetInstance()->decrement(sMapField);
        }
    }
}
void dLytControlGame_c::finalizeState_Map_DelRes() {
    dLytMeter_c::GetInstance()->setMeterMode(dLytMeterMain_c::MODE_NONE);
    dLytMeter_c::GetInstance()->setBasicPosition(dLytMeterMain_c::POSITION_NORMAL);
    dMessage_c::getInstance()->setMapEvent(dLytMapMain_c::MAP_EVENT_MAX);
}

dLytControlGame_c::dLytControlGame_c() : mStateMgr(*this), mCommand(CMD_NONE), mHelpTextIndex(0) {
    sInstance = this;
}

dLytControlGame_c::~dLytControlGame_c() {
    sInstance = nullptr;
}

int dLytControlGame_c::create() {
    if (dStage_c::GetInstance() == nullptr) {
        return NOT_READY;
    }
    dLytMap_c::initMapState();
    mMeter.build();
    mMsgWindow.build();
    mSimpleWindow.build();
    mWipe.build();

    dBase_c::createBase(fProfile::LYT_ENEMY_ICON, this, 0, OTHER);
    dBase_c::createBase(fProfile::LYT_DROP_LINE, this, 0, OTHER);
    dBase_c::createBase(fProfile::LYT_FORCE_LINE, this, 0, OTHER);
    dBase_c::createBase(fProfile::LYT_SAVE_MGR, this, 0, OTHER);

    if (dScGame_c::currentSpawnInfo.stageName == "F103_1" || dScGame_c::currentSpawnInfo.stageName == "t_tkm26" ||
        dScGame_c::currentSpawnInfo.stageName == "F103" || dScGame_c::currentSpawnInfo.stageName == "t_tkm24") {
        dBase_c::createBase(fProfile::LYT_SUIRYU_SCORE, this, 0, OTHER);
        dBase_c::createBase(fProfile::LYT_SUIRYU_SCORE_COMP, this, 0, OTHER);
    } else if (dScGame_c::isCurrentStage("B301")) {
        dBase_c::createBase(fProfile::LYT_DEMO_DOWSING, this, 0, OTHER);
    }

    mCommand = CMD_NONE;
    mHelpTextIndex = 0;

    StoryflagManager *mgr = StoryflagManager::sInstance;

    mCurrentPauseDisp = mgr->getCounterOrFlag(STORYFLAG_ON_COLLECTION_SCREEN);
    mPauseDisp00Tab = mgr->getCounterOrFlag(STORYFLAG_SAVED_DISP00_TAB);

    field_0x15C66 = 0;
    mIsPauseDemo = false;

    mItemForPauseDemo = 0;
    mPickupItemCount = 0;
    mItemCountForPauseDemo = 0;
    mPauseDemoDisp = 0;

    mStateMgr.changeState(StateID_Normal);

    mpHelp = nullptr;
    mpDrawMark = nullptr;

    field_0x15C54 = 0;
    field_0x15C58 = 0;

    return SUCCEEDED;
}

int dLytControlGame_c::doDelete() {
    if (!releaseRes()) {
        return NOT_READY;
    }
    if (!decrementRes()) {
        return NOT_READY;
    }
    mMsgWindow.remove();
    mSimpleWindow.remove();
    mMeter.remove();
    mWipe.remove();
    if (mpHelp != nullptr) {
        mpHelp->remove();
        delete mpHelp;
        mpHelp = nullptr;
    }
    if (mpDrawMark != nullptr) {
        mpDrawMark->remove();
        delete mpDrawMark;
        mpDrawMark = nullptr;
    }

    if (mpMap != nullptr) {
        mpMap->remove();
        mpMap.destruct();
        m2d::setLytAllocator();
    }

    return SUCCEEDED;
}

int dLytControlGame_c::execute() {
    if (!isInSomeMapState()) {
        mFootprintsMgr.setFootstepMinDistSq(sHio.field_0x04 * sHio.field_0x04);
        mFootprintsMgr.execute();
    }

    if (*mStateMgr.getStateID() == StateID_Normal) {
        fn_802CFA90();
    } else {
        if (dCsGame_c::GetInstance() != nullptr) {
            dCsGame_c::GetInstance()->setField_0x9A2(true);
        }
    }

    if (mpDrawMark != nullptr) {
        if (field_0x15C54 == 0) {
            mpDrawMark->setField_0x941(1);
        }
        mpDrawMark->execute();
        if (mpDrawMark->getField_0x940() != 0) {
            mpDrawMark->remove();
            delete mpDrawMark;
            mpDrawMark = nullptr;
        }
    } else {
        if (field_0x15C54 != 0) {
            mpDrawMark = new dLytDrawMark_c();
            mpDrawMark->build(field_0x15C58);
        }
    }

    mMeter.execute();
    mMsgWindow.execute();
    mSimpleWindow.execute();
    mWipe.execute();

    if (mpHelp != nullptr) {
        mpHelp->execute();

        if (dLytHelp_c::GetInstance() != nullptr) {
            if (mpHelp->isStateEnd()) {
                mpHelp->remove();
                delete mpHelp;
                mpHelp = nullptr;
            }
        }
    }

    mStateMgr.executeState();

    return SUCCEEDED;
}

int dLytControlGame_c::draw() {
    mMeter.draw();
    // okay
    if (dLytMap_c::GetInstance() != nullptr && dLytMap_c::GetInstance() != nullptr) {
        mpMap->draw();
    }
    mMsgWindow.draw();
    mSimpleWindow.addToDrawList();
    mWipe.draw();
    if (mpHelp != nullptr) {
        mpHelp->draw();
    }
    if (mpDrawMark != nullptr) {
        mpDrawMark->draw();
    }
    return SUCCEEDED;
}

bool dLytControlGame_c::checkStoryflag(s32 flag) const {
    return StoryflagManager::sInstance->getCounterOrFlag(flag);
}

void dLytControlGame_c::fn_802CFA90() {
    if (mCommand != CMD_NONE) {
        if (dLytMeter_c::GetInstance()->itemSelectFn800EFDF0(true) ||
            dLytMeter_c::GetInstance()->minusBtnFn800F7600() || dLytMeter_c::GetInstance()->dowsingFn800FE4B0()) {
            mCommand = CMD_NONE;
            return;
        }

        if (!mIsEvent && !EventManager::fn_8009F990() && mCommand != CMD_HELP) {
            mCommand = CMD_NONE;
        }
    }

    if (mCommand == CMD_MAP) {
        if (mMapEvent == dLytMapMain_c::MAP_EVENT_SAVE_OBJ) {
            mCommand = CMD_NONE;
        }
        bool b = false;
        if (dLytMap_c::GetInstance() != nullptr) {
            b = dLytMap_c::GetInstance()->isVisible();
        }

        if (!b) {
            mStateMgr.changeState(StateID_Map_SetRes);
            dSndPlayerMgr_c::GetInstance()->enterMap();
            dSndSourceMgr_c::GetInstance()->fn_803852B0();
            hideHelp();
            dLytMeter_c::setPlusBtnCall(false);
        }
        mCommand = CMD_NONE;
    }

    if (mCommand == CMD_PAUSE) {
        mStateMgr.changeState(StateID_Pause_SetRes);
        dSndPlayerMgr_c::GetInstance()->enterMenu();
        dSndSourceMgr_c::GetInstance()->fn_803852B0();
        hideHelp();
        mCommand = CMD_NONE;
    }

    if (mCommand == CMD_HELP) {
        if (mpHelp == nullptr) {
            mpHelp = new dLytHelp_c();
            mpHelp->build();
        }
        if (dLytHelp_c::GetInstance() != nullptr) {
            mpHelp->setText(mHelpTextIndex);
            mCommand = CMD_NONE;
        }
    }
}

void dLytControlGame_c::hideHelp() {
    if (dLytHelp_c::GetInstance() != nullptr) {
        mpHelp->requestOut();
    }
}

void dLytControlGame_c::changeHelpText(s32 textIndex) {
    if (dLytHelp_c::GetInstance() != nullptr) {
        mpHelp->changeText(textIndex);
    }
}

bool dLytControlGame_c::isStateNotNormalOrInEvent() const {
    if (*mStateMgr.getStateID() != StateID_Normal) {
        return false;
    }
    return EventManager::isInEvent();
}

bool dLytControlGame_c::isInSomeMapState() const {
    bool ret = false;
    const sStateIDIf_c &id = *mStateMgr.getStateID();
    if (id == StateID_Map_SetRes || id == StateID_Map_SyncRes || id == StateID_Map || id == StateID_Map_DelRes) {
        ret = true;
    }
    return ret;
}

bool dLytControlGame_c::isNotInStateMap() const {
    return !(*mStateMgr.getStateID() == StateID_Map);
}

bool dLytControlGame_c::fn_802CFF60() const {
    bool ret = false;
    const sStateIDIf_c &id = *mStateMgr.getStateID();
    if (id == StateID_Map_SetRes || id == StateID_Map_SyncRes) {
        ret = true;
    }
    return ret;
}

void dLytControlGame_c::openMap() {
    mCommand = CMD_MAP;
    mMapEvent = dLytMapMain_c::MAP_EVENT_NONE;
    mIsEvent = false;
}

void dLytControlGame_c::openMapEvent(s32 mapEvent, s32 arg1, s32 arg2) {
    mCommand = CMD_MAP;
    mMapEvent = mapEvent;
    mIsEvent = true;
    mMapEventArg1 = arg1;
    mMapEventArg2 = arg2;

    if (mapEvent != dLytMapMain_c::MAP_EVENT_SAVE_OBJ) {
        return;
    }

    bool showStatueSelect = true;
    u8 exitId = 0;

    if (dAcPy_c::GetLinkM()->getPosition().z > -78700.0f && dAcPy_c::GetLinkM()->getPosition().z < -76700.0f) {
        mMapEventArg1 = dLytMapMain_c::SURFACE_PROVINCE_ELDIN;
        if (!checkStoryflag(STORYFLAG_ELDIN_DISCOVERED)) {
            exitId = 1;
            showStatueSelect = false;
        }
    } else if (dAcPy_c::GetLinkM()->getPosition().z > 62800.0f && dAcPy_c::GetLinkM()->getPosition().z < 64800) {
        mMapEventArg1 = dLytMapMain_c::SURFACE_PROVINCE_LANAYRU;
        if (!checkStoryflag(STORYFLAG_LANAYRU_DISCOVERED)) {
            exitId = 2;
            showStatueSelect = false;
        }
    } else {
        mMapEventArg1 = dLytMapMain_c::SURFACE_PROVINCE_FARON;
        if (!checkStoryflag(STORYFLAG_FARON_DISCOVERED)) {
            exitId = 0;
            showStatueSelect = false;
        }
    }

    if (mMapEventArg1 == dLytMapMain_c::SURFACE_PROVINCE_FARON && checkStoryflag(STORYFLAG_HARP_CUTSCENE) &&
        !checkStoryflag(STORYFLAG_GROOSE_LANDS_IN_FARON)) {
        // Forced descent to Sealed Grounds with Groose
        exitId = 15;
        showStatueSelect = false;
    } else if (mMapEventArg1 == dLytMapMain_c::SURFACE_PROVINCE_ELDIN &&
               checkStoryflag(STORYFLAG_ELDIN_VOLCANO_CAN_ERUPT) && !checkStoryflag(STORYFLAG_BOKO_BASE_BEATEN)) {
        // Forced descent to Boko Base
        exitId = 8;
        showStatueSelect = false;
    } else if (mMapEventArg1 == dLytMapMain_c::SURFACE_PROVINCE_ELDIN &&
               checkStoryflag(STORYFLAG_SCRAPPER_HAS_WATER_BASIN) &&
               !checkStoryflag(STORYFLAG_SUMMIT_BIG_FROG_WATERED) &&
               !checkStoryflag(STORYFLAG_WATER_BASIN_ESCORT_STARTED)) {
        // Forced descent to Eldin entry with Scrapper and the water basin
        exitId = 1;
        showStatueSelect = false;
    } else if (checkStoryflag(STORYFLAG_LEVIAS_FIGHT_DEFEATED) && !checkStoryflag(STORYFLAG_WATER_DRAGON_SOTH_PART)) {
        // TODO: When is this condition hit? showStatueSelect is only false here if we haven't discovered a surface
        // province, but with Levias beaten we should've set foot in each province
        showStatueSelect = true;
    }

    if (!showStatueSelect) {
        dScGame_c::GetInstance()->triggerExit(dAcPy_c::GetLink()->mRoomID, exitId);
        mCommand = CMD_NONE;
    }
}

void dLytControlGame_c::openPause() {
    if (canOpenPause() == true) {
        mCommand = CMD_PAUSE;
        mIsEvent = false;
        mIsPauseDemo = false;
    }
}

extern "C" dAcBase_c *getCurrentEventActor();

void dLytControlGame_c::openPauseDemo() {
    if (canOpenPause() == true) {
        dAcItem_c *item = static_cast<dAcItem_c *>(getCurrentEventActor());
        if (item != nullptr) {
            mItemForPauseDemo = item->getItemId_i();
            mCommand = CMD_PAUSE;
            mIsEvent = true;
            mIsPauseDemo = true;
            if (mItemForPauseDemo == ITEM_EXTRA_WALLET) {
                mPauseDemoDisp = dLytPauseMgr_c::DISP_00_INVENTORY;
            } else {
                mPauseDemoDisp = dLytPauseMgr_c::DISP_01_COLLECTION;
            }
            mItemCountForPauseDemo = dAcItem_c::getCollectionCurrentCount();
            mPickupItemCount = item->getQuantity() != 0 ? item->getQuantity() : 1;
        }
    }
}

void dLytControlGame_c::openHelp(s32 textIndex) {
    if (dLytMeter_c::GetInstance()->itemSelectFn800EFDF0(true) || dLytMeter_c::GetInstance()->minusBtnFn800F7600() ||
        dLytMeter_c::GetInstance()->dowsingFn800FE4B0()) {
        return;
    }

    if (mpHelp == nullptr) {
        mCommand = CMD_HELP;
        mIsEvent = false;
        mHelpTextIndex = textIndex;
    }
}

bool dLytControlGame_c::releaseRes() {
    return mLytArcControl.release();
}

bool dLytControlGame_c::decrementRes() {
    dArcResult_e result1 = LayoutArcManager::GetInstance()->ensureLoaded2(sMap2D);
    if (result1 != D_ARC_RESULT_ERROR_NOT_FOUND) {
        if (result1 != D_ARC_RESULT_OK) {
            return false;
        }
        LayoutArcManager::GetInstance()->decrement(sMap2D);
    }

    dArcResult_e result2;
    if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
        result2 = LayoutArcManager::GetInstance()->ensureLoaded2(sMapSky);
    } else {
        result2 = LayoutArcManager::GetInstance()->ensureLoaded2(sMapField);
    }
    if (result2 != D_ARC_RESULT_ERROR_NOT_FOUND) {
        if (result2 != D_ARC_RESULT_OK) {
            return false;
        }
        if (dStageMgr_c::GetInstance()->isSTIFAreaSky()) {
            LayoutArcManager::GetInstance()->decrement(sMapSky);
        } else {
            LayoutArcManager::GetInstance()->decrement(sMapField);
        }
    }

    return true;
}

void dLytControlGame_c::fn_802D04F0() {
    field_0x15C66 = 0;
    d2d::defaultSet();
}

bool dLytControlGame_c::canOpenPause() {
    if (dLytMeter_c::GetInstance()->itemSelectFn800EFDF0(true) || dLytMeter_c::GetInstance()->minusBtnFn800F7600() ||
        dLytMeter_c::GetInstance()->dowsingFn800FE4B0()) {
        return false;
    }

    if (mpHelp == nullptr || mpHelp->isWaiting()) {
        return true;
    }

    return false;
}

void dLytControlGame_c::checkForBeaconNextToMark() {
    fBase_c *base = nullptr;
    bool found = false;
    mVec3_c markPos;
    while ((base = fManager_c::searchBaseByGroupType(STAGE, base)) != nullptr) {
        if (base->mProfileName == fProfile::T_MAP_MARK) {
            dTgMapMark_c *mark = static_cast<dTgMapMark_c *>(base);
            if (!mark->checkActorProperty(dAcBase_c::AC_PROP_0x100) && mark->rotz_shift0_0xf == 0) {
                found = true;
                markPos = mark->mPosition;
            }
        }
    }

    if (found) {
        for (u32 i = 0; i < 5; i++) {
            const mVec3_c *pos = getBeaconPosition(i);
            if (pos != nullptr) {
                mVec3_c diff = markPos - *pos;
                if (diff.mag() <= sHio.field_0x0C) {
                    SceneflagManager::sInstance->setFlag(0x3F, 0x18);
                }
            }
        }
    }
}

void dLytControlGame_c::resetFootsteps() {
    mFootprintsMgr.reset();
}
