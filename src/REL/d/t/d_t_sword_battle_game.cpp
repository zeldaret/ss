#include "d/t/d_t_sword_battle_game.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/d_sc_game.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_mini_game.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "sized_string.h"
#include "toBeSorted/event.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/minigame_mgr.h"

const char *sGameEvents[] = {"StartSwordBattleGame", "EndSwordBattleGame", "EndSwordBattleGame2"};
const char *sEndEventNames[] = {
    "dAcBvd_c_DeadDEMO", "MG_end", "AsuraDeadB", "BKR_end", "BBossEvac", "BBoss2Evac", "BBoss3Evac", "BLS_end",
};

STATE_VIRTUAL_DEFINE(dTgSwordBattleGame_c, Wait);
STATE_VIRTUAL_DEFINE(dTgSwordBattleGame_c, Manage);

SPECIAL_ACTOR_PROFILE(TAG_SWORD_BATTLE_GAME, dTgSwordBattleGame_c, fProfile::TAG_SWORD_BATTLE_GAME, 0x6F, 0, 4);

int dTgSwordBattleGame_c::create() {
    if (!checkInBossRushFlag() && !MinigameManager::GetInstance()->checkInBossRush()) {
        return FAILED;
    }

    init();
    mStateMgr.changeState(StateID_Wait);
    return SUCCEEDED;
}

int dTgSwordBattleGame_c::doDelete() {
    fBase_c::doDelete();
    mTimer.vt_0x10();
    return SUCCEEDED;
}

int dTgSwordBattleGame_c::actorExecute() {
    if (EventManager::isInEvent()) {
        static const char *l_currentEvent = EventManager::getCurrentEventName();

        if (strequals(l_currentEvent, sEndEventNames[0]) || strequals(l_currentEvent, sEndEventNames[1]) ||
            strequals(l_currentEvent, sEndEventNames[2]) || strequals(l_currentEvent, sEndEventNames[3]) ||
            strequals(l_currentEvent, sEndEventNames[4]) || strequals(l_currentEvent, sEndEventNames[5]) ||
            strequals(l_currentEvent, sEndEventNames[6]) || strequals(l_currentEvent, sEndEventNames[7])) {
            field_0x1AB = true;
        }
    }
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dTgSwordBattleGame_c::actorExecuteInEvent() {
    bool advance = mEventRelated.isAdvance();

    int ret = NOT_READY;
    switch (mEventRelated.getCurrentEventCommand()) {
        case 'none': {
            mEventRelated.advanceNext();
            ret = SUCCEEDED;
        } break;
        case 'gini': {
            mEventRelated.advanceNext();
            ret = SUCCEEDED;
        } break;
        case 'cndn': {
            if (advance) {
                dLytMiniGame_c::GetInstance()->startCountdown();

            } else if (dLytMiniGame_c::GetInstance()->isStartCountdownDone()) {
                dLytMiniGame_c::GetInstance()->timeRelatedInit();
                dLytMiniGame_c::GetInstance()->setBestTime(getBossHighscore());
                dLytMiniGame_c::GetInstance()->setShowBestTime(true);
                mTimer.initUpCounter();
                field_0x1A8 = true;
                field_0x1AC = true;
                mEventRelated.advanceNext();
            }
            ret = SUCCEEDED;
        } break;
        case 'fnsh': {
            if (advance) {
                dLytMiniGame_c::GetInstance()->startFinish();
            } else if (dLytMiniGame_c::GetInstance()->isFinishAnimDone()) {
                mEventRelated.advanceNext();
            }
            ret = SUCCEEDED;
        } break;
        case 'gend': {
            if (advance) {
                dScGame_c::GetInstance()->triggerExit(mRoomID, getFromParams(0, 0xFF));
            }
            ret = SUCCEEDED;
        } break;
        default: {
            if (dAcPy_c::GetLink()->checkFlags0x340(0x200) && !field_0x1AD &&
                MinigameManager::endMinigame(MinigameManager::BOSS_RUSH)) {
                field_0x1AD = true;
                unsetInBossRushFlag();
                unsetEarlyFightFlags();
                unsetImprisonedFightFlags();
                unsetDemiseFightFlag();
                setCurrentBossNumber(0);
            }

            if (!field_0x1AE && field_0x1A9 && !field_0x1AA) {
                field_0x1AE = true;
                saveCurrentTime(getCurrentTime());
                setInBossRushFlag();
                setCurrentBossNumber(mBossNumber + 1);
                if (MinigameManager::endMinigame(MinigameManager::BOSS_RUSH)) {
                    dLytMiniGame_c::GetInstance()->startFinish();
                    dLytMiniGame_c::GetInstance()->endTimeResult();
                }
            }
        } break;
    }
    return ret;
}

int dTgSwordBattleGame_c::draw() {
    return SUCCEEDED;
}

void dTgSwordBattleGame_c::initializeState_Wait() {}
void dTgSwordBattleGame_c::executeState_Wait() {
    if (MinigameManager::GetInstance()->checkInBossRush()) {
        if (field_0x1A8) {
            mStateMgr.changeState(StateID_Manage);
        } else {
            checkInEvent(sGameEvents[0]);
        }
    }
}
void dTgSwordBattleGame_c::finalizeState_Wait() {}

void dTgSwordBattleGame_c::initializeState_Manage() {}
void dTgSwordBattleGame_c::executeState_Manage() {
    if (!field_0x1A9) {
        mTimer.vt_0x14();
    }

    s32 time = getCurrentTime();
    dLytMiniGame_c::GetInstance()->setDisplayedTime(time);

    if (!field_0x1AE && field_0x1A9 && field_0x1AA) {
        saveCurrentTime(time);
        setInBossRushFlag();
        setCurrentBossNumber(mBossNumber + 1);
        if (MinigameManager::endMinigame(MinigameManager::BOSS_RUSH) && checkInEvent(sGameEvents[1])) {
            field_0x1AE = true;
        }
    }

    if (field_0x1AB && checkInEvent(sGameEvents[2])) {
        field_0x1AB = false;
    }
}
void dTgSwordBattleGame_c::finalizeState_Manage() {}

void dTgSwordBattleGame_c::init() {
    if (MinigameManager::GetInstance()->checkInBossRush()) {
        createBase(fProfile::LYT_MINI_GAME, this, dLytMiniGame_c::VARIANT_BOSS_RUSH, OTHER);
    }
    mTimer.vt_0x0C();

    field_0x1A8 = false;
    field_0x1A9 = false;
    field_0x1AA = false;
    field_0x1AB = false;
    field_0x1AC = false;
    field_0x1AD = false;
    field_0x1AE = false;

    mBossNumber = getCurrentBossNumber();
}

bool dTgSwordBattleGame_c::checkInEvent(const char *eventName) {
    if (EventManager::isInEvent() && strequals(EventManager::getCurrentEventName(), eventName)) {
        return true;
    }

    if (EventManager::isInEvent0Or7()) {
        static volatile u32 FLAGS_1 = 0x00000001;
        static u32 FLAGS_2 = 0x00100001;
        u32 f1 = FLAGS_1;
        u32 f2 = FLAGS_2;
        u32 eventFlag = f2 & ~f1;
        Event event = Event(eventName, 100, eventFlag, nullptr, nullptr);
        EventManager::alsoSetAsCurrentEvent(this, &event, nullptr);
    }
    return false;
}

s32 dTgSwordBattleGame_c::getCurrentTime() const {
    s32 time = mTimer.getTime();
    if (time > TIME_CONVERT(59, 59, 99)) {
        time = TIME_CONVERT(59, 59, 99);
    }
    return time;
}

bool dTgSwordBattleGame_c::checkInBossRushFlag() {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_BOSS_RUSH_ACTIVE);
}

void dTgSwordBattleGame_c::unsetInBossRushFlag() {
    StoryflagManager::sInstance->unsetFlag(STORYFLAG_BOSS_RUSH_ACTIVE);
}

void dTgSwordBattleGame_c::unsetEarlyFightFlags() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;

    // Not naming these flags due to the weirdness...
    pMgr->unsetFlag(STORYFLAG_351);
    pMgr->unsetFlag(STORYFLAG_352);
    pMgr->unsetFlag(STORYFLAG_353);
    pMgr->unsetFlag(STORYFLAG_354);
    pMgr->unsetFlag(STORYFLAG_355);
    pMgr->unsetFlag(STORYFLAG_356);
    pMgr->unsetFlag(STORYFLAG_357);
}

void dTgSwordBattleGame_c::unsetImprisonedFightFlags() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;

    pMgr->unsetFlag(STORYFLAG_IMPRISONED1_BOSSRUSH);
    pMgr->unsetFlag(STORYFLAG_IMPRISONED2_BOSSRUSH);
    pMgr->unsetFlag(STORYFLAG_IMPRISONED3_BOSSRUSH);
}

void dTgSwordBattleGame_c::unsetDemiseFightFlag() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;

    pMgr->unsetFlag(STORYFLAG_DEMISE_BOSSRUSH);
}

void dTgSwordBattleGame_c::setInBossRushFlag() {
    StoryflagManager::sInstance->setFlag(STORYFLAG_BOSSRUSH_ACTIVE);
}

void dTgSwordBattleGame_c::saveCurrentTime(s32 time) const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    pMgr->setFlagOrCounterToValue(STORYFLAG_BOSSRUSH_CURRENT_MIN, (time / 1000) / 60);
    pMgr->setFlagOrCounterToValue(STORYFLAG_BOSSRUSH_CURRENT_SEC, (time / 1000) % 60);
    pMgr->setFlagOrCounterToValue(STORYFLAG_BOSSRUSH_CURRENT_MS, (time % 1000) / 10);
}

s32 dTgSwordBattleGame_c::getBossHighscore() const {
    s32 time;
    switch (getBossIdx()) {
        case 0:  time = getGhirahim1Highscore(); break;
        case 1:  time = getScalderaHighscore(); break;
        case 2:  time = getMolderachHighscore(); break;
        case 4:  time = getKoloktosHighscore(); break;
        case 5:  time = getTentalusHighscore(); break;
        case 6:  time = getGhirahim2Highscore(); break;
        case 3:  time = getImprisoned1Highscore(); break;
        case 7:  time = getImprisoned2Highscore(); break;
        case 8:  time = getImprisoned3Highscore(); break;
        case 9:  time = getHordeHighscore(); break;
        case 10: time = getGhirahim3Highscore(); break;
        case 11: time = getDemiseHighscore(); break;
    }

    if (time > TIME_CONVERT(59, 59, 99)) {
        time = TIME_CONVERT(59, 59, 99);
    }
    return time;
}

s32 dTgSwordBattleGame_c::getGhirahim1Highscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_GHIRAHIM1_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_GHIRAHIM1_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_GHIRAHIM1_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getScalderaHighscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_SCALDERA_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_SCALDERA_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_SCALDERA_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getMolderachHighscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_MOLDERACH_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_MOLDERACH_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_MOLDERACH_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getKoloktosHighscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_KOLOKTOS_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_KOLOKTOS_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_KOLOKTOS_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getTentalusHighscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_TENTALUS_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_TENTALUS_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_TENTALUS_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getGhirahim2Highscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_GHIRAHIM2_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_GHIRAHIM2_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_GHIRAHIM2_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getImprisoned1Highscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_IMP1_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_IMP1_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_IMP1_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getImprisoned2Highscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_IMP2_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_IMP2_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_IMP2_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getImprisoned3Highscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_IMP3_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_IMP3_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_IMP3_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getHordeHighscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_HORDE_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_HORDE_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_HORDE_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getGhirahim3Highscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_GHIRAHIM3_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_GHIRAHIM3_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_GHIRAHIM3_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getDemiseHighscore() const {
    StoryflagManager *pMgr = StoryflagManager::sInstance;
    u16 min = pMgr->getCounterOrFlag(STORYFLAG_DEMISE_TIME_MIN);
    u16 sec = pMgr->getCounterOrFlag(STORYFLAG_DEMISE_TIME_SEC);
    u16 msec = pMgr->getCounterOrFlag(STORYFLAG_DEMISE_TIME_MS);
    return TIME_CONVERT(min, sec, msec);
}

s32 dTgSwordBattleGame_c::getBossIdx() const {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_563);
}

s32 dTgSwordBattleGame_c::getCurrentBossNumber() const {
    return StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_654);
}

void dTgSwordBattleGame_c::setCurrentBossNumber(s32 num) {
    StoryflagManager::sInstance->setFlagOrCounterToValue(STORYFLAG_654, num);
}

void dTgSwordBattleGame_c::notifyBossDeath(bool ghirahim_related) {
    field_0x1A9 = true;
    field_0x1AA = ghirahim_related;
    dLytMiniGame_c::GetInstance()->setComplete();
}

bool dTgSwordBattleGame_c::checkFightStarted() const {
    return field_0x1AC;
}
