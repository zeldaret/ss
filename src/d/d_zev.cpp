#include "d/d_zev.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/d_a_salbage_obj.h"
#include "d/d_base.h"
#include "d/d_bzs_types.h"
#include "d/d_camera.h"
#include "d/d_pad.h"
#include "d/d_player_act.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/flag/itemflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/skipflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_area_caption.h"
#include "d/lyt/d_lyt_control_game.h"
#include "d/lyt/d_lyt_end_roll.h"
#include "d/lyt/d_lyt_map.h"
#include "d/lyt/msg_window/d_lyt_msg_window.h"
#include "d/snd/d_snd_small_effect_mgr.h"
#include "d/snd/d_snd_state_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "sized_string.h"
#include "toBeSorted/actor_event.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/d_jstudio.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/salvage_mgr.h"
#include "toBeSorted/save_manager.h"

#include <string.h>

#pragma clang diagnostic ignored "-Wnon-pod-varargs"

inline const char *getSkipOnlyEventName() {
    return "SkipOnly";
}

struct EventParam1 {
    EventParam1();
    virtual ~EventParam1();

    /* 0x04 */ u16 field_0x04;
    /* 0x06 */ s16 field_0x06;
    /* 0x08 */ u16 field_0x08;
    /* 0x0A */ u16 field_0x0A;
};

EventParam1::~EventParam1() {}
EventParam1::EventParam1() : field_0x04(45), field_0x06(3), field_0x08(1), field_0x0A(1) {}

static int getActorRoomId(dAcBase_c *ac) {
    return ac->mRoomID;
}

static const EVNT *getEVNTForRoomAndIndex(u32 idx, s32 room) {
    return dStage_c::GetInstance()->getRoom(room)->getEventForIndex(idx);
}

static const EVNT *getEVNTForActorAndIndex(u32 idx, dAcBase_c *ac) {
    return getEVNTForRoomAndIndex(idx, getActorRoomId(ac));
}

static void *getEventDFile() {
    return OarcManager::GetInstance()->getZevFromArc("EventD");
}

static void *getCurrentStateZevDat() {
    return CurrentStageArcManager::GetInstance()->getFileFromCurrentLay0Arc("dat/zev.dat");
}

static void *getCommonZevDat() {
    return OarcManager::GetInstance()->getZevFromArc("Common");
}

static void infoPrintf(const char *fmt, ...) {
    // no-op
}

static void debugPrintf(const char *fmt, ...) {
    // no-op
}

static void errorPrintf(const char *fmt, ...) {
    // no-op
}

#include "d/dol_default_zev_dat.inc"

static void *getDefaultZevDat() {
    return DEFAULT_ZEV_DAT;
}

static char *formatEvent(u32 event) {
    // NON-MATCHING
    static char buf[5];
    static const int shift[] = {0x18, 0x10, 0x8, 0x00};
    for (int i = 0; i < ARRAY_LENGTH(shift); i++) {
        buf[i] = event >> shift[i];
    }
    buf[4] = '\0';
    return buf;
}

static EventParam1 sParam1;

// I have no idea how this works
static struct Hack {
    Hack() {
        *(u32 *)(DEFAULT_ZEV_DAT + 0x114) = 'free';
        *(u32 *)(DEFAULT_ZEV_DAT + 0x120) = 'free';
    }
} hack;

dZevEvent_c dZevEvent_c::sInstance;

void dZevFile_c::clear() {
    mpHeader = nullptr;
    mpScenes = nullptr;
    mpActors = nullptr;
    mpCuts = nullptr;
    mpCutData = nullptr;
    mpData = nullptr;
    mpInts = nullptr;
    mpFloats = nullptr;
    mpStrings = nullptr;
    mInitialized = false;
}

void dZevFile_c::setZev(void *zev) {
    if (zev == nullptr) {
        clear();
        return;
    }

    mpHeader = static_cast<ZevHeader *>(zev);
    void *dat = zev;
    dat = (u8 *)dat + sizeof(ZevHeader);

    if (mpHeader->numScene != 0) {
        mpScenes = (ZevScene *)dat;
        dat = (u8 *)dat + mpHeader->numScene * sizeof(ZevScene);
    }
    if (mpHeader->numActor != 0) {
        mpActors = (ZevActor *)dat;
        dat = (u8 *)dat + mpHeader->numActor * sizeof(ZevActor);
    }
    if (mpHeader->numCuts != 0) {
        mpCuts = (ZevCut *)dat;
        dat = (u8 *)dat + mpHeader->numCuts * sizeof(ZevCut);
    }
    if (mpHeader->numCutData != 0) {
        mpCutData = (ZevCutData *)dat;
        dat = (u8 *)dat + mpHeader->numCutData * sizeof(ZevCutData);
    }
    if (mpHeader->numData != 0) {
        mpData = (ZevDatum *)dat;
        dat = (u8 *)dat + mpHeader->numData * sizeof(ZevDatum);
    }
    if (mpHeader->numInt != 0) {
        mpInts = (int *)dat;
        dat = (u8 *)dat + mpHeader->numInt * sizeof(int);
    }
    if (mpHeader->numFloat != 0) {
        mpFloats = (f32 *)dat;
        dat = (u8 *)dat + mpHeader->numFloat * sizeof(f32);
    }
    if (mpHeader->numString != 0) {
        mpStrings = (char *)dat;
    }
    mInitialized = true;
}

ZevScene *dZevFile_c::findScene(const char *sceneName) {
    if (mpScenes == nullptr) {
        return nullptr;
    }
    for (int i = 0; i < mpHeader->numScene; i++) {
        ZevScene *ev = getScene(i);
        if (strncmp(sceneName, ev->name, sizeof(ev->name)) == 0) {
            return ev;
        }
    }
    return nullptr;
}

void *dZevFile_c::getDefaultZevDat() {
    return DEFAULT_ZEV_DAT;
}

extern "C" const char *getActorNameFromId(u16);

void dZevActor_c::init(ZevActor *zevActor, dZevEvent_c *event, dAcBase_c *gameActor) {
    init(zevActor, event);
    mActorType = AC_ACTOR;
    mGameActor.link(gameActor);
    mProfileName = gameActor->mProfileName;

    if (zevActor == nullptr) {
        mZevActorName = getActorNameFromId(mProfileName);
    }

    mInit = true;
    field_0x44 = 111; // ???

    // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
    debugPrintf(
        "zev: %04d: start: %s  %s (o)", mpEvent->getCurrentFrame(), isExtra() ? "extra" : "staff", mZevActorName
    );

    if (getCut(mCurrentCutIdx) != nullptr) {
        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        debugPrintf(
            "zev: %04d: start: cut    %s/%s ", mpEvent->getCurrentFrame(), mZevActorName, getCut(mCurrentCutIdx)->name
        );
    }
}

void dZevActor_c::init(ZevActor *zevActor, dZevEvent_c *event, u16 profileName) {
    init(zevActor, event);
    mActorType = profileName == fProfile::GAME ? AC_GAME : AC_PROFILE;
    mProfileName = profileName;

    if (zevActor == nullptr) {
        mZevActorName = getActorNameFromId(mProfileName);
    }

    mInit = true;
    field_0x44 = 103; // ???

    // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
    debugPrintf(
        "zev: %04d: start: %s  %s (g)", mpEvent->getCurrentFrame(), isExtra() ? "extra" : "staff", mZevActorName
    );

    if (getCut(mCurrentCutIdx) != nullptr) {
        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        debugPrintf(
            "zev: %04d: start: cut    %s/%s ", mpEvent->getCurrentFrame(), mZevActorName, getCut(mCurrentCutIdx)->name
        );
    }
}

void dZevActor_c::init(ZevActor *zevActor, dZevEvent_c *event, int arg) {
    init(zevActor, event);
    mActorType = AC_ACTOR;
    if (arg != -1) {
        field_0x40 = arg;
    }

    mInit = true;
    field_0x44 = 105; // ???

    // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
    debugPrintf(
        "zev: %04d: start: %s  %s (i)", mpEvent->getCurrentFrame(), isExtra() ? "extra" : "staff", mZevActorName
    );

    if (getCut(mCurrentCutIdx) != nullptr) {
        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        debugPrintf(
            "zev: %04d: start: cut    %s/%s ", mpEvent->getCurrentFrame(), mZevActorName, getCut(mCurrentCutIdx)->name
        );
    }
}

void dZevActor_c::init(int arg, const char *actorName, dZevEvent_c *event) {
    init(nullptr, event);
    mActorType = AC_ACTOR;
    if (actorName != nullptr) {
        mZevActorName = actorName;
    }

    if (arg != -1) {
        field_0x40 = arg;
    }

    mInit = true;
    field_0x44 = 105; // ???

    debugPrintf("zev: %04d: start: %s  %d (xi)", mpEvent->getCurrentFrame(), isExtra() ? "extra" : "staff", field_0x40);
}

void dZevActor_c::init(dAcBase_c *gameActor, const char *actorName, dZevEvent_c *event) {
    init(nullptr, event);
    field_0x40 = 0x80;
    mGameActor.link(gameActor);
    mProfileName = gameActor->mProfileName;

    if (actorName != nullptr) {
        mZevActorName = actorName;
    } else {
        mZevActorName = getActorNameFromId(mProfileName);
    }

    mActorType = AC_ACTOR;
    mInit = true;
    field_0x44 = 111; // ???

    // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
    debugPrintf(
        "zev: %04d: start: %s  %s (xo)", mpEvent->getCurrentFrame(), isExtra() ? "extra" : "staff", mZevActorName
    );
}

void dZevActor_c::init(u16 profileName, const char *actorName, dZevEvent_c *event) {
    init(nullptr, event);
    field_0x40 = 0x80;
    mProfileName = profileName;

    if (actorName != nullptr) {
        mZevActorName = actorName;
    } else {
        mZevActorName = getActorNameFromId(mProfileName);
    }

    mActorType = AC_PROFILE;
    mInit = true;
    field_0x44 = 103; // ???

    // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
    debugPrintf(
        "zev: %04d: start: %s  %s (xg)", mpEvent->getCurrentFrame(), isExtra() ? "extra" : "staff", mZevActorName
    );
}

void dZevActor_c::init(ZevActor *zevActor, dZevEvent_c *event) {
    field_0x04 = 0;
    mpZevActor = zevActor;
    mpEvent = event;
    mCurrentCutIdx = 0;
    mAvailableCutsMask = 0;
    mDoneCutsMask = 0;
    mAdvanceCondition = 0;
    mAdvanceSignal = 0;

    for (int i = 0; i < (int)ARRAY_LENGTH(field_0x4C); i++) {
        field_0x4C[i] = -1;
    }
    field_0x44 = 45;

    if (mpZevActor != nullptr) {
        for (int i = 0; i < mpZevActor->cutNum; i++) {
            mAvailableCutsMask |= 1 << i;
        }
        mZevActorName = mpZevActor->name;
        field_0x40 = mpZevActor->field_0x20;
        ZevCut *cut = getCut(0);
        if (cut != nullptr) {
            for (int i = 0; i < cut->advanceConditionNum; i++) {
                mAdvanceCondition |= 1 << i;
            }
        }
    } else {
        mZevActorName = "(Extra)";
        field_0x40 = 0x80;
    }
    field_0x04 = 1;
}

void dZevActor_c::clear() {
    mpZevActor = nullptr;
    mpEvent = nullptr;
    mAvailableCutsMask = 0;
    mDoneCutsMask = 0;
    mAdvanceCondition = 0;
    mAdvanceSignal = 0;
    mCurrentCutIdx = 0;
    mActorType = AC_NONE;
    mGameActor.unlink();
    mProfileName = fProfile::PROFILE_MAX;
    mZevActorName.empty();

    field_0x40 = -1;
    for (int i = 0; i < (int)ARRAY_LENGTH(field_0x4C); i++) {
        field_0x4C[i] = -1;
    }
    field_0x44 = 45;
    mInit = false;
}

ZevCut *dZevActor_c::getCut(int idx) {
    if (isExtra()) {
        return nullptr;
    }

    if (idx < 0 || idx >= mpZevActor->cutNum) {
        return nullptr;
    }

    return mpEvent->getFile()->getCut(mpZevActor->cutOfs + idx);
}

const char *dZevActor_c::getCutName(int idx) {
    if (idx == -1) {
        idx = mCurrentCutIdx;
    }

    const char *name = getCut(idx)->name;
    if (name != nullptr) {
        return name;
    }
    return "???";
}

ZevCutData *dZevActor_c::getCutData(int dataIdx, int cutIdx) {
    ZevCut *cut = getCut(cutIdx == -1 ? mCurrentCutIdx : cutIdx);
    if (cut == nullptr) {
        return nullptr;
    }

    return mpEvent->getFile()->getCutData(cut->index + dataIdx);
}

bool dZevActor_c::advanceConditionMet(int cutIdx) {
    // TODO Regswap
    return !isOutstandingCut(1 << cutIdx) || (mCurrentCutIdx == cutIdx && mAdvanceCondition == mAdvanceSignal);
}

bool dZevActor_c::advanceBlocked() {
    if (isExtra()) {
        return false;
    }

    bool blocked = true;
    if (mAdvanceCondition == mAdvanceSignal) {
        blocked = false;
    }

    if (mCurrentCutIdx + 1 < mpZevActor->cutNum) {
        ZevCut *cut = getCut(mCurrentCutIdx + 1);
        if (cut != nullptr && cut->waitFor != -1 && !mpEvent->cutCompletedMaybe(cut->waitFor)) {
            return true;
        }
    }

    return blocked;
}

bool dZevActor_c::tryAdvanceCut() {
    if (isExtra()) {
        return false;
    }

    if (getCut(mCurrentCutIdx) != nullptr) {
        ZevCut *cut = getCut(mCurrentCutIdx);
        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        debugPrintf("zev: %04d: !stop: cut    %s/%s", mpEvent->getCurrentFrame(), mZevActorName, cut->name);
    }

    mDoneCutsMask |= 1 << mCurrentCutIdx;
    mCurrentCutIdx++;
    field_0x04 = 1;
    mAdvanceCondition = 0;
    mAdvanceSignal = 0;
    if (mCurrentCutIdx < mpZevActor->cutNum) {
        ZevCut *cut = getCut(mCurrentCutIdx);
        if (cut == nullptr) {
            return true;
        }
        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        debugPrintf("zev: %04d: start: cut    %s/%s", mpEvent->getCurrentFrame(), mZevActorName, cut->name);
        for (int i = 0; i < cut->advanceConditionNum; i++) {
            mAdvanceCondition |= 1 << i;
        }
        return true;
    } else {
        mCurrentCutIdx = mpZevActor->cutNum - 1;
        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        debugPrintf(
            "zev: %04d: !stop: %s  %s ", mpEvent->getCurrentFrame(), isExtra() ? "extra" : "staff", mZevActorName
        );
        return false;
    }
}

void dZevActor_c::signalAdvance(int signal) {
    if (field_0x4C[signal] < mpEvent->getCurrentFrame()) {
        field_0x4C[signal] = mpEvent->getCurrentFrame();
        mAdvanceSignal |= 1 << signal;
    }
}

u32 dZevActor_c::getCommand(int dataIdx) {
    ZevCutData *data = getCutData(dataIdx, -1);
    if (data != nullptr) {
        return data->name;
    } else {
        return '????';
    }
}

int dZevActor_c::getCutUnk(int dataIdx) {
    ZevCutData *data = getCutData(dataIdx, -1);
    if (data != nullptr) {
        return data->field_0x4;
    } else {
        return -1;
    }
}

int dZevActor_c::getIntData(int **outData, int name, int dataIdx) {
    ZevCutData *data = getCutData(dataIdx, -1);
    bool valid = data != nullptr;
    for (int i = 0; valid && i < data->dataNum; i++) {
        ZevDatum *datum = mpEvent->getFile()->getDatum(data->dataOfs + i);
        if (name == datum->command) {
            if (datum->type == ZEV_TYPE_INTEGER) {
                *outData = mpEvent->getFile()->getInteger(datum->index);
                return datum->length;
            } else {
                errorPrintf("param %s is NOT integer data %d", formatEvent(name), datum->type);
            }
        }
    }
    return false;
}

int dZevActor_c::getFloatData(f32 **outData, int name, int dataIdx) {
    ZevCutData *data = getCutData(dataIdx, -1);
    bool valid = data != nullptr;
    for (int i = 0; valid && i < data->dataNum; i++) {
        ZevDatum *datum = mpEvent->getFile()->getDatum(data->dataOfs + i);
        if (name == datum->command) {
            if (datum->type == ZEV_TYPE_FLOAT) {
                *outData = mpEvent->getFile()->getFloat(datum->index);
                return datum->length;
            } else {
                errorPrintf("param %s is NOT float data %d", formatEvent(name), datum->type);
            }
        }
    }
    return false;
}

int dZevActor_c::getVecData(Vec **outData, int name, int dataIdx) {
    ZevCutData *data = getCutData(dataIdx, -1);
    bool valid = data != nullptr;
    for (int i = 0; valid && i < data->dataNum; i++) {
        ZevDatum *datum = mpEvent->getFile()->getDatum(data->dataOfs + i);
        if (datum != nullptr && name == datum->command) {
            if (datum->type == ZEV_TYPE_FLOAT) {
                *outData = (Vec *)mpEvent->getFile()->getFloat(datum->index);
                return datum->length / 3;
            } else {
                errorPrintf("param %s is NOT vector data %d", formatEvent(name), datum->type);
            }
        }
    }
    return false;
}

int dZevActor_c::getStringData(char **outData, int name, int dataIdx) {
    ZevCutData *data = getCutData(dataIdx, -1);
    for (int i = 0; i < data->dataNum; i++) {
        ZevDatum *datum = mpEvent->getFile()->getDatum(data->dataOfs + i);
        if (datum != nullptr && name == datum->command) {
            if (datum->type == ZEV_TYPE_STRING) {
                *outData = (char *)mpEvent->getFile()->getString(datum->index);
                return datum->length;
            } else {
                errorPrintf("param %s is NOT string data %d", formatEvent(name), datum->type);
            }
        }
    }
    return false;
}

void dZevEvent_c::loadFile(int slot, void *data) {
    if (data != nullptr) {
        mZevFiles[slot].setZev(data);
    } else {
        mZevFiles[slot].clear();
    }
}

bool dZevEvent_c::startSceneInternal(const char *sceneName, dAcBase_c *ac1, dAcBase_c *ac2) {
    for (int i = 0; i < (int)ARRAY_LENGTH(mZevFiles); i++) {
        ZevScene *ev = mZevFiles[i].findScene(sceneName);
        if (ev == nullptr) {
            continue;
        }

        mCurrentFrame = 0;
        mEventName = sceneName;
        mIsStart = true;

        if (ac1 != nullptr) {
            mEventStarter.link(ac1);
            mEventSpeaker.link(ac1);
        } else {
            mEventStarter.unlink();
            mEventSpeaker.unlink();
        }

        if (ac2 != nullptr) {
            mEventPartner.link(ac2);
        } else {
            mEventPartner.unlink();
        }

        mpCurrentScene = ev;
        mpCurrentFile = &mZevFiles[i];

        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        debugPrintf(
            "zev: %04d: start: scene  %s [se=%d] [buf=%d] (%s)", mCurrentFrame, mEventName, field_0x024, i,
            checkSceneMask(1) ? "move" : "stop"
        );

        initAllEventActor();

        if (ac1 != nullptr && findActorIndexByPointer(ac1) == -1) {
            findAndInitEventActor(ac1, nullptr);
        }

        if (ac2 != nullptr && findActorIndexByPointer(ac2) == -1) {
            findAndInitEventActor(ac2, nullptr);
        }

        return true;
    }

    errorPrintf("scene %s is NOT found", sceneName);

    dZevFile_c *file = &mZevFiles[ARRAY_LENGTH(mZevFiles) - 1];
    ZevScene *ev = file->findScene("_DEFAULT_EVENT_");
    if (ev != nullptr) {
        mCurrentFrame = 0;
        mEventName = sceneName;
        mIsStart = true;

        if (ac1 != nullptr) {
            mEventStarter.link(ac1);
            mEventSpeaker.link(ac1);
        } else {
            mEventStarter.unlink();
            mEventSpeaker.unlink();
        }

        if (ac2 != nullptr) {
            mEventPartner.link(ac2);
        } else {
            mEventPartner.unlink();
        }

        mpCurrentScene = ev;
        mpCurrentFile = file;

        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        debugPrintf("zev: %04d: start: dummy scene  %s %d", mCurrentFrame, mEventName, field_0x024);

        initAllEventActor();

        if (ac1 != nullptr && findActorIndexByPointer(ac1) == -1) {
            findAndInitEventActor(ac1, nullptr);
        }

        if (ac2 != nullptr && findActorIndexByPointer(ac2) == -1) {
            findAndInitEventActor(ac2, nullptr);
        }

        return true;
    } else {
        errorPrintf("default scene is NOT found");
        clear(false);
        return false;
    }
}

bool dZevEvent_c::startSoloSceneInternal(const char *sceneName, const char *actorName) {
    for (int i = 0; i < (int)ARRAY_LENGTH(mZevFiles); i++) {
        ZevScene *ev = mZevFiles[i].findScene(sceneName);
        if (ev == nullptr) {
            continue;
        }

        mCurrentFrame = 0;
        mEventName = sceneName;
        mIsStart = true;

        mEventStarter.link(EventManager::getCurrentEventActor());
        mEventSpeaker.link(EventManager::getEventSpeaker());
        mEventPartner.link(EventManager::getEventPartner());

        mpCurrentScene = ev;
        mpCurrentFile = &mZevFiles[i];

        // UB: Cannot pass object of non-POD type 'SizedString<32>' through variadic method
        debugPrintf(
            "zev: %04d: start: solo %s %s [seq=%d] [buf=%d] (%s)", mCurrentFrame, mEventName, actorName, field_0x024, i,
            checkSceneMask(1) ? "move" : "stop"
        );

        ZevActor *ac = mpCurrentFile->getActor(mpCurrentScene->actorOfs);
        SizedString<32> name = actorName;
        for (int i = 0; i < mpCurrentScene->actorNum; i++) {
            if (name == ac[i].name) {
                findAndInitEventActor(&ac[i]);
                return true;
            }
        }

        debugPrintf("zev: staff %s not found in scene %s", actorName, sceneName);
        return false;
    }

    debugPrintf("zev: scene %s not found", sceneName);
    return false;
}

void dZevEvent_c::initAllEventActor() {
    ZevActor *ac = mpCurrentFile->getActor(mpCurrentScene->actorOfs);
    for (int i = 0; i < mpCurrentScene->actorNum; i++) {
        findAndInitEventActor(&ac[i]);
    }
}

void dZevEvent_c::clearFiles() {
    for (int i = 0; i < (int)ARRAY_LENGTH(mZevFiles); i++) {
        mZevFiles[i].clear();
    }
    clear(true);
}

void dZevEvent_c::clear(bool force) {
    if (force || mIsStart) {
        mEventName.empty();
        for (int i = 0; i < (int)ARRAY_LENGTH(mActors); i++) {
            mActors[i].clear();
        }
        mAvailableActorsMask = 0;
        mDoneActorsMask = 0;
    }
    mIsStart = false;
    mIsOver = false;
}

bool dZevEvent_c::cutCompletedMaybe(int cutIdx) {
    ZevActor *ac = mpCurrentFile->getActor(mpCurrentFile->getCut(cutIdx)->actorNo);
    for (int i = 0; i < ARRAY_LENGTH(mActors); i++) {
        if (ac == mActors[i].getZevActor()) {
            return !isOutstandingActor(1 << i) ||
                   mActors[i].advanceConditionMet(mpCurrentFile->getCut(cutIdx)->field_0x14);
        }
    }

    return true;
}

struct FourCCHelper {
    FourCCHelper(const char *src) {
        if (src != nullptr) {
            int i;
            bool nul = false;
            for (i = 0; i < 4; i++) {
                nul = nul || src[i] == '\0';
                buf[i] = !nul ? src[i] : ' ';
            }
        }
    }

    int value() const {
        return *(int *)buf;
    }

    /* 0x00 */ char buf[4];
};

// maybe not entirely right
inline void fourcc(char *buf, const char *src) {
    if (src != nullptr) {
        int i;
        bool nul = false;
        for (i = 0; i < 4; i++) {
            nul = nul || src[i] == '\0';
            buf[i] = !nul ? src[i] : ' ';
        }
    }
}

int dZevEvent_c::findAndInitEventActor(ZevActor *ac) {
    int targetSlot = -1;
    for (int i = ARRAY_LENGTH(mActors) - 1; i >= 0; i--) {
        if (!mActors[i].isInit()) {
            targetSlot = i;
        } else if (ac != nullptr) {
            if (mActors[i].isInitAndName(ac->name)) {
                targetSlot = i;
            }
        }
    }

    if (targetSlot != -1) {
        int f = ac->field_0x20;
        if (f == 0 || f == 1 || f == 3 || f == 4 || f == 5 || f == 55) {
            mActors[targetSlot].init(ac, this, f);
        } else if (strchr(ac->name, '@') != nullptr || strchr(ac->name, ':') != nullptr) {
            dAcBase_c *gameActor = findActorBySpecialName(ac->name);
            if (gameActor != nullptr) {
                mActors[targetSlot].init(ac, this, gameActor);
            } else {
                FourCCHelper cc(ac->name);
                if (cc.value() == '@all') {
                    mActors[targetSlot].init(ac, this, (u16)fProfile::GAME);
                } else {
                    /* unknown staff name */
                    errorPrintf("%s: 不明なスタッフ名です。\n", ac->name);
                    mActors[targetSlot].init(ac, this, 0x80);
                }
            }
        } else if (strncmp(ac->name, "Link", 5 /* why 5? */) == 0) {
            mActors[targetSlot].init(ac, this, dAcPy_c::GetLinkM());
        } else {
            u16 profile = getProfileIdForName(ac->name);
            if (profile != fProfile::PROFILE_MAX) {
                dAcBase_c *gameActor = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(profile, nullptr));
                if (gameActor == nullptr) {
                    /* object not found */
                    errorPrintf("%s: オブジェクトが見つかりませんでした。\n", ac->name);
                    mActors[targetSlot].init(ac, this, 0x80);
                } else {
                    dAcBase_c *gameActor2 =
                        static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(profile, gameActor));
                    if (gameActor2 != nullptr) {
                        mActors[targetSlot].init(ac, this, profile);
                    } else {
                        mActors[targetSlot].init(ac, this, gameActor);
                    }
                }
            } else {
                /* unknown staff name */
                errorPrintf("%s: 不明なスタッフ名です。\n", ac->name);
                mActors[targetSlot].init(ac, this, 0x80);
            }
        }
    }

    if (targetSlot == -1) {
        return targetSlot;
    }

    mAvailableActorsMask |= (1 << targetSlot);
    return targetSlot;
}

int dZevEvent_c::findAndInitEventActor(const char *name) {
    int targetSlot = -1;
    int sameActor = -1;

    u16 profileName = getProfileIdForName(name);

    for (int i = ARRAY_LENGTH(mActors) - 1; i >= 0; i--) {
        if (!mActors[i].isInit()) {
            targetSlot = i;
        } else {
            if (name != nullptr) {
                if (mActors[i].isInitAndName(name)) {
                    return i;
                }
            }

            if (mActors[i].isInitAndProfile(profileName)) {
                sameActor = i;
            }
        }
    }

    if (sameActor == -1 && targetSlot != -1) {
        mActors[targetSlot].init(profileName, name, this);
        mAvailableActorsMask |= (1 << targetSlot);
    }
    return targetSlot;
}

int dZevEvent_c::findAndInitEventActor(int arg, const char *name) {
    int targetSlot = -1;

    for (int i = ARRAY_LENGTH(mActors) - 1; i >= 0; i--) {
        if (!mActors[i].isInit()) {
            targetSlot = i;
        } else {
            if (name != nullptr) {
                if (mActors[i].isInitAndArg(arg)) {
                    return i;
                }
            }
        }
    }

    if (targetSlot != -1) {
        mActors[targetSlot].init(arg, name, this);
        mAvailableActorsMask |= (1 << targetSlot);
    }
    return targetSlot;
}

int dZevEvent_c::findAndInitEventActor(dAcBase_c *ac, const char *name) {
    int targetSlot = -1;
    int sameActor = -1;

    for (int i = ARRAY_LENGTH(mActors) - 1; i >= 0; i--) {
        if (!mActors[i].isInit()) {
            targetSlot = i;
        } else {
            if (name != nullptr) {
                if (mActors[i].isInitAndName(name)) {
                    mActors[i].link(ac);
                    return i;
                }
            }

            if (mActors[i].isInitAndProfile(ac->mProfileName)) {
                sameActor = i;
            }
        }
    }

    if (sameActor != -1) {
        mActors[sameActor].link(ac);
    } else {
        if (targetSlot != -1) {
            mActors[targetSlot].init(ac, name, this);
            mAvailableActorsMask |= (1 << targetSlot);
        } else {
            /* could not be registered */
            debugPrintf("\x1b[31m%s は登録できませんでした\n\x1b[m", name);
            return targetSlot;
        }
    }
    return targetSlot;
}

int dZevEvent_c::findAndForceInitEventActor(dAcBase_c *ac, const char *name) {
    int targetSlot = -1;

    for (int i = ARRAY_LENGTH(mActors) - 1; i >= 0; i--) {
        if (!mActors[i].isInit()) {
            targetSlot = i;
        } else {
            if (name != nullptr) {
                if (mActors[i].isInitAndName(name)) {
                    mActors[i].link(ac);
                    return i;
                }
            }
        }
    }

    if (targetSlot != -1) {
        mActors[targetSlot].init(ac, name, this);
        mAvailableActorsMask |= (1 << targetSlot);
    } else {
        /* could not be registered */
        debugPrintf("\x1b[31m%s は登録できませんでした\n\x1b[m", name);
        return targetSlot;
    }
    return targetSlot;
}

int dZevEvent_c::findAndInitEventActor(u16 profile, const char *name) {
    int targetSlot = -1;

    for (int i = ARRAY_LENGTH(mActors) - 1; i >= 0; i--) {
        if (!mActors[i].isInit()) {
            targetSlot = i;
        } else {
            if (mActors[i].isInitAndProfile(profile)) {
                return i;
            }
        }
    }

    if (targetSlot != -1) {
        mActors[targetSlot].init(profile, name, this);
        mAvailableActorsMask |= (1 << targetSlot);
    } else {
        /* could not be registered */
        debugPrintf("\x1b[31m%s は登録できませんでした\n\x1b[m", name);
        return targetSlot;
    }
    return targetSlot;
}

dAcBase_c *dZevEvent_c::findActorBySpecialName(const char *name) {
    SplitHelper split1;
    SplitHelper split2;

    dAcBase_c *ret = nullptr;

    if (name[0] == '@') {
        FourCCHelper cc(name);
        // comments are best guess
        switch (cc.value()) {
            case '@sta':
                /* @starter, @star? */
                ret = mEventStarter.get();
                break;
            case '@pla':
                /* @player */
                ret = dAcPy_c::GetLinkM();
                break;
            case '@par':
                /* @partner */
                ret = mEventPartner.get();
                break;
            case '@spe':
                /* @speaker */
                ret = mEventSpeaker.get();
                break;
            case '@[0]':
            case '@[1]':
            case '@[2]':
            case '@[3]': ret = mOtherActors[(cc.value() >> 8) & 0xF].get(); break;
        }
    } else if (trySplit(name, '@', split1)) {
        u16 profile = getProfileIdForName(split1.left);
        if (split1.right == "near") {
            if (profile != fProfile::PROFILE_MAX) {
                dAcPy_c *player = dAcPy_c::GetLinkM();
                ret = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(profile));
                if (ret != nullptr) {
                    mVec3_c diff = player->getPosition() - ret->getPosition();
                    f32 bestF = diff.mag();
                    int i = 1;
                    dAcBase_c *candidate = ret;
                    while (true) {
                        candidate = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(profile, candidate));
                        if (candidate == nullptr) {
                            break;
                        }
                        if (!trySplit(split1.left, ':', split2) || split2.right == candidate->mSomeStr) {
                            diff = player->getPosition() - candidate->getPosition();
                            f32 f = diff.mag();
                            if (f < bestF) {
                                bestF = f;
                                ret = candidate;
                            }
                        }
                        if (i++ >= 1000) {
                            break;
                        }
                    }
                }
            }
        } else if (split1.right[0] == '(') {
            if (profile != fProfile::PROFILE_MAX) {
                int vals[3] = {0, 0, 0};
                int j = 0;
                char op = '+';
                // fun condition
                for (int i = 1; i != 0; i++) {
                    int token = split1.right[i];
                    if (token == '-') {
                        op = '-';
                    } else if (token >= '0' && token <= '9') {
                        vals[j] = vals[j] * 10 + (token - '0');
                    } else if (token == 'H') {
                        vals[j] = vals[j] * 100; // hundred
                    } else if (token == 'T') {
                        vals[j] = vals[j] * 1000; // thousand
                    } else if (token == ',' || token == ' ') {
                        if (op == '-') {
                            vals[j] = -vals[j];
                        }
                        j++;
                    } else if (token == ')' || i > 0x20) {
                        if (op == '-') {
                            vals[j] = -vals[j];
                        }
                        break;
                    }
                }

                mVec3_c final;
                final.x = vals[0];
                final.y = vals[1];
                final.z = vals[2];
                ret = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(profile));
                if (ret != nullptr) {
                    mVec3_c diff = final - ret->getPosition();
                    f32 bestF = diff.mag();
                    int i = 1;
                    dAcBase_c *candidate = ret;
                    while (true) {
                        candidate = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(profile, candidate));
                        if (candidate == nullptr) {
                            break;
                        }
                        diff = final - candidate->getPosition();
                        f32 f = diff.mag();
                        if (f < bestF) {
                            bestF = f;
                            ret = candidate;
                        }
                        if (i++ >= 1000) {
                            break;
                        }
                    }
                }
            }
        }
    } else if (trySplit(name, ':', split1)) {
        u16 profile = getProfileIdForName(split1.left);
        if (profile != fProfile::PROFILE_MAX) {
            int i = 1;
            while (true) {
                ret = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(profile, ret));
                if (ret == nullptr || split1.right == ret->mSomeStr) {
                    break;
                }
                if (i++ >= 1000) {
                    break;
                }
            }
        }
    } else {
        u16 profile = getProfileIdForName(name);
        if (profile == fProfile::PLAYER) {
            ret = dAcPy_c::GetLinkM();
        } else if (profile != fProfile::PROFILE_MAX) {
            ret = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(profile));
        }
    }

    return ret;
}

bool dZevEvent_c::trySplit(const char *src, char sep, SplitHelper &out) {
    bool ret = false;

    out.left = src;

    for (int i = 0; i < out.left.len(); i++) {
        if (out.left.mChars[i] == '\0') {
            break;
        }
        if (out.left.mChars[i] == sep) {
            char *right = out.left.mChars + i + 1;
            out.left.mChars[i] = '\0';
            out.right = right;
            ret = true;
            break;
        }
    }

    return ret;
}

int dZevEvent_c::findActorIndexByName(const char *name) {
    for (int i = 0; i < (int)ARRAY_LENGTH(mActors); i++) {
        if (mActors[i].isInitAndName(name)) {
            return i;
        }
    }
    return -1;
}

int dZevEvent_c::findActorIndexByPointer(dAcBase_c *ac) {
    int ret = -1;
    for (int i = 0; i < (int)ARRAY_LENGTH(mActors); i++) {
        if (mActors[i].isInitAndActor(ac)) {
            return i;
        }

        if (mActors[i].getField_0x44() == 0x67) {
            if (mActors[i].isInitAndProfile(ac->mProfileName)) {
                ret = i;
            }
        }
    }
    return ret;
}

int dZevEvent_c::findActorIndexByArg(int arg) {
    for (int i = 0; i < (int)ARRAY_LENGTH(mActors); i++) {
        if (mActors[i].isInitAndArg(arg)) {
            return i;
        }
    }
    return -1;
}

bool dZevEvent_c::processAdvancement() {
    if (!mIsOver) {
        for (int i = 0; i < (int)ARRAY_LENGTH(mActors); i++) {
            if (isOutstandingActor(1 << i)) {
                if (mActors[i].getField_0x04() <= 0) {
                    errorPrintf("NO access %s/%s dead", mActors[i].getName(), mActors[i].getCutName(-1));
                    if (!mActors[i].tryAdvanceCut()) {
                        mDoneActorsMask |= 1 << i;
                    }
                }
                mActors[i].tickField_0x04();
                if (!mActors[i].advanceBlocked() && !mActors[i].tryAdvanceCut()) {
                    mDoneActorsMask |= (1 << i);
                    if (mAvailableActorsMask == mDoneActorsMask) {
                        debugPrintf("zev: %04d: allover: scene  %s [%d]", mCurrentFrame, getEventName(), field_0x024);
                        mIsOver = true;
                    }
                }
            }
        }
    }
    mCurrentFrame++;
    return !mIsOver;
}

bool dZevEvent_c::stopSceneInternal(const char *name) {
    if (isStartAndName(name)) {
        debugPrintf("zev: %04d: !stop: scene  %s [%d]", mCurrentFrame, getEventName(), field_0x024);
        field_0x024++;
        clear(false);
        return true;
    } else {
        debugPrintf("zev: %04d: cannot !stop: scene %s %d", mCurrentFrame, getEventName(), field_0x024);
        field_0x024++;
        return false;
    }
}

bool dZevEvent_c::checkSceneMask(u32 mask) {
    return (mpCurrentScene->field_0x20 & mask) != 0;
}

bool dZevEvent_c::setActorRef(int idx, dAcBase_c *ac) {
    if (ac != nullptr) {
        mOtherActors[idx].link(ac);
    } else {
        mOtherActors[idx].unlink();
    }
}

void dZevEvent_c::init() {
    debugPrintf("zev system init");
    clearFiles();
}

void dZevEvent_c::stop() {
    clear(false);
}

bool dZevEvent_c::startScene(const char *sceneName, dAcBase_c *ac1, dAcBase_c *ac2) {
    if (isIdle() != false) {
        return startSceneInternal(sceneName, ac1, ac2);
    }
    return false;
}

bool dZevEvent_c::startSoloScene(const char *sceneName, const char *actorName) {
    if (isIdle() != false) {
        return startSoloSceneInternal(sceneName, actorName);
    }
    return false;
}

bool dZevEvent_c::stopScene(const char *name) {
    if (isBusy() == false) {
        return false;
    }
    if (isStartAndName(name) == false) {
        return false;
    }
    stopSceneInternal(name);
    stop();
    return true;
}

bool dZevEvent_c::isIdle() const {
    return !mIsStart;
}

bool dZevEvent_c::isBusy() const {
    return mIsStart;
}

bool dZevEvent_c::calc() {
    return processAdvancement();
}

ActorEventRelated::ActorEventRelated(dAcBase_c &ac, dZevEvent_c *instance) {
    if (instance != nullptr) {
        mpZev = instance;
    } else {
        mpZev = dZevEvent_c::GetInstance();
    }
    initActor(&ac);
}

ActorEventRelated::ActorEventRelated(int arg, dZevEvent_c *instance) {
    if (instance != nullptr) {
        mpZev = instance;
    } else {
        mpZev = dZevEvent_c::GetInstance();
    }
    initArg(arg);
}

ActorEventRelated::~ActorEventRelated() {
    remove();
}

void ActorEventRelated::initActor(dAcBase_c *actor) {
    remove();
    mInit = true;
    mAcRef.link(actor);
}

void ActorEventRelated::initArg(int arg) {
    remove();
    mInit = true;
    mArg = arg;
}

void ActorEventRelated::initName(const char *name) {
    remove();
    mInit = true;
    mName = name;
}

void ActorEventRelated::remove() {
    mInit = false;
    mAcRef.unlink();
    mArg = -1;
    mName.empty();
    field_0x38 = -1;
    reset();
}

void ActorEventRelated::reset() {
    field_0x3C = -1;
    mCutIndex = -1;
    mZevActorIndex = -1;
    mFlags = 0;
}

void ActorEventRelated::setup() {
    reset();
    if (mArg != -1) {
        dZevEvent_c::GetInstance()->findAndInitEventActor(mArg, nullptr);
    } else if (mAcRef.isLinked()) {
        dZevEvent_c::GetInstance()->findAndInitEventActor(mAcRef.get(), nullptr);
    } else {
        dZevEvent_c::GetInstance()->findAndInitEventActor(mName);
    }
}

int ActorEventRelated::updateInternal() {
    if (!mInit) {
        if (field_0x3C != -1) {
            reset();
        }
        return -1;
    }

    int state;
    int frame = mpZev->getCurrentFrame();
    if (mpZev->isInit() != 0) {
        state = mpZev->getField_0x024();
    } else {
        state = -2;
    }

    if (state == field_0x3C && frame == field_0x38) {
        return mZevActorIndex;
    }

    mFlags = 0;
    if (state != field_0x3C) {
        reset();
        if (field_0x3C != -1) {
            setFlag(FLAG_0x08);
        }

        if (state != -1) {
            if (mArg != -1) {
                mZevActorIndex = mpZev->findActorIndexByArg(mArg);
            } else if (mAcRef.isLinked()) {
                mZevActorIndex = mpZev->findActorIndexByPointer(mAcRef.get());
            } else {
                mZevActorIndex = mpZev->findActorIndexByName(mName);
            }
            if (mZevActorIndex != -1) {
                setFlag(FLAG_0x04);
            }
        } else {
            mZevActorIndex = -1;
        }
        field_0x3C = state;
    }
    if (mZevActorIndex != -1) {
        getActor()->setField_0x04(1);
        setFlag(FLAG_0x01);
        if (getActor()->isExtra()) {
            mCutIndex = -1;
        } else {
            if (getActor()->getCurrentCutIdx() != mCutIndex) {
                mCutIndex = getActor()->getCurrentCutIdx();
                setFlag(FLAG_0x10);
            }
        }
    }
    field_0x38 = frame;
    return mZevActorIndex;
}

bool ActorEventRelated::cutEnd() {
    return updateInternal() != -1;
}

int ActorEventRelated::getCutIndex() {
    updateInternal();
    return mCutIndex;
}

u32 ActorEventRelated::getCurrentEventCommand() {
    u32 ret = '????';
    if (getCutIndex() != -1) {
        (void)getActor()->getCut(mCutIndex);
        ret = getActor()->getCommand(0);
    }
    return ret;
}

bool ActorEventRelated::isAdvance() {
    updateInternal();
    return checkFlag(FLAG_0x10);
}

void ActorEventRelated::advanceNext() {
    if (getCutIndex() != -1) {
        getActor()->signalAdvance(0);
    }
}

int ActorEventRelated::getSingleShortData(int *result, u32 code, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        int *res;
        ret = getActor()->getIntData(&res, code, idx);
        if (ret != 0) {
            *result = res[0];
        }
    }
    return ret;
}

int ActorEventRelated::getSingleIntData(int *result, u32 code, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        int *res;
        ret = getActor()->getIntData(&res, code, idx);
        if (ret != 0) {
            *result = res[0];
        }
    }
    return ret;
}

int ActorEventRelated::getSingleFloatData(f32 *result, u32 code, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        f32 *res;
        ret = getActor()->getFloatData(&res, code, idx);
        if (ret != 0) {
            *result = res[0];
        }
    }
    return ret;
}

int ActorEventRelated::getSingleVecData(mVec3_c *result, u32 code, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        Vec *res;
        ret = getActor()->getVecData(&res, code, idx);
        if (ret != 0) {
            result->copyFrom(res);
        }
    }
    return ret;
}

int ActorEventRelated::getSingleShortDataWithDefault(int *result, u32 code, const int &def, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        int *res;
        ret = getActor()->getIntData(&res, code, idx);
        *result = ret != 0 ? res[0] : def;
    }
    return ret;
}

int ActorEventRelated::getSingleIntDataWithDefault(int *result, u32 code, const int &def, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        int *res;
        ret = getActor()->getIntData(&res, code, idx);
        *result = ret != 0 ? res[0] : def;
    }
    return ret;
}

int ActorEventRelated::getSingleFloatDataWithDefault(f32 *result, u32 code, const f32 &def, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        f32 *res;
        ret = getActor()->getFloatData(&res, code, idx);
        *result = ret != 0 ? res[0] : def;
    }
    return ret;
}

int ActorEventRelated::getSingleVecDataWithDefault(mVec3_c *result, u32 code, const mVec3_c &def, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        Vec *res;
        ret = getActor()->getVecData(&res, code, idx);
        result->copyFrom(ret != 0 ? (const Vec *)res : def);
    }
    return ret;
}

int ActorEventRelated::getShortDataBuf(int **result, u32 code, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        ret = getActor()->getIntData(result, code, idx);
    }
    return ret;
}

int ActorEventRelated::getFloatDataBuf(f32 **result, u32 code, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        ret = getActor()->getFloatData(result, code, idx);
    }
    return ret;
}

int ActorEventRelated::getVecDataBuf(mVec3_c **result, u32 code, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        ret = getActor()->getVecData((Vec **)result, code, idx);
    }
    return ret;
}

int ActorEventRelated::getStringDataBuf(char **result, u32 code, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        ret = getActor()->getStringData(result, code, idx);
    }
    return ret;
}

int ActorEventRelated::getActorByStringData(dAcBase_c **result, u32 code, u32 idx) {
    int ret = 0;
    if (getCutIndex() != -1) {
        char *name[4]; // single char** doesn't work, maybe a confusion with char[4]?
        ret = getActor()->getStringData(name, code, idx);
        if (ret != 0) {
            dAcBase_c *ac = mpZev->findActorBySpecialName(*name);
            if (ac != nullptr) {
                *result = ac;
            } else {
                ret = 0;
            }
        }
    }
    return ret;
}

int ActorEventRelated::getActorByStringDataWithDefault(dAcBase_c **result, u32 code, dAcBase_c *def, u32 idx) {
    dAcBase_c *tmp;
    *result = getActorByStringData(&tmp, code, idx) != 0 ? tmp : def;
}

int ActorEventRelated::getActorByStringDataOrSpecialName(dAcBase_c **result, u32 code, const char *name, u32 idx) {
    int ret = 0;
    dAcBase_c *tmp;

    ret = getActorByStringData(&tmp, code, idx);
    *result = ret != 0 ? tmp : mpZev->findActorBySpecialName(name);
    return ret;
}

bool ActorEventRelated::scheduleEvent(Event &event, dAcBase_c *actor) {
    bool ret = false;
    if (mArg != -1) {
        ret = EventManager::alsoSetAsCurrentEvent(mArg, &event, actor);
    } else if (mAcRef.isLinked()) {
        ret = EventManager::alsoSetAsCurrentEvent(mAcRef.get(), &event, actor);
    }
    return ret;
}

bool ActorEventRelated::isInEvent(char *name) {
    bool ret = false;
    if (mArg != -1) {
        ret = EventManager::isInEvent(mArg, name);
    } else if (mAcRef.isLinked()) {
        ret = EventManager::isInEvent(mAcRef.get(), name);
    }
    return ret;
}

bool ActorEventRelated::isThisActorInEvent() {
    bool ret = false;
    if (mArg != -1) {
        ret = EventManager::isInEvent(mArg, nullptr);
    } else if (mAcRef.isLinked()) {
        ret = EventManager::isInEvent(mAcRef.get(), nullptr);
    }
    return ret;
}

struct EventParam2 {
    EventParam2();
    virtual ~EventParam2();

    /* 0x04 */ u16 field_0x04;
    /* 0x06 */ u16 field_0x06;
    /* 0x08 */ u16 field_0x08;
    /* 0x0A */ u16 field_0x0A;
    /* 0x0C */ u16 field_0x0C;
    /* 0x0E */ u16 field_0x0E;
};

EventParam2::EventParam2() : field_0x04(1), field_0x06(0), field_0x08(0), field_0x0A(0), field_0x0C(0), field_0x0E(0) {}
EventParam2::~EventParam2() {}

static EventParam2 sParam2;

Event sEventSkipOnly("SkipOnly", 0x1, 0, nullptr, nullptr);

static ActorEventRelatedDirector sDirector1;
static ActorEventRelatedDirector sDirector2;
static ActorEventRelatedSound sSound;
static ActorEventRelatedFlow sFlow;
static ActorEventRelatedEmitter sEmitter;

struct AnotherEventStruct {
    /* 0x00 */ dAcRef_c<dAcBase_c> mRef;
    /* 0x0C */ u8 _0x0C[0x90 - 0x0C];
};

static AnotherEventStruct sAnotherStructs[50];

EventManager::EventManager()
    : mState(0), mFlags(0), field_0x18C(-1), field_0x18E(-1), field_0x190(0), field_0x192(0), field_0x194(0) {}

EventManager::~EventManager() {}

void EventManager::startEventManager() {
    if (sInstance != nullptr) {
        return;
    }

    sInstance = new EventManager();
    sInstance->field_0x004.clear();
    getCurrentEvent()->clear();
    sInstance->field_0x0C4.clear();
    sInstance->field_0x124.clear();
    sInstance->field_0x18C = 0xFFFF;

    dZevEvent_c::GetInstance()->init();
    dZevEvent_c::GetInstance()->loadFile(0, getEventDFile());
    dZevEvent_c::GetInstance()->loadFile(2, getCurrentStateZevDat());
    dZevEvent_c::GetInstance()->loadFile(3, getCommonZevDat());
    dZevEvent_c::GetInstance()->loadFile(4, dZevFile_c::getDefaultZevDat());
    sInstance->mFlags |= FLAGS_0x10000;

    sDirector1.initName("Director");
    sDirector2.initName("AD");
    sSound.vt_0x14();
    sFlow.vt_0x14();
    sEmitter.vt_0x14();
    sInstance->field_0x192 = 0;
    sInstance->field_0x194 = 0;
    sInstance->field_0x190 = 0;
    sInstance->field_0x196 = -1;
    sInstance->field_0x18C = 0xFFFF;
    sInstance->field_0x18E = 0xFFFF;
    sInstance->field_0x198 = 0;

    infoPrintf("start event system");
}

Event *EventManager::getSkipOnlyEvent() {
    return &sEventSkipOnly;
}

void EventManager::killEventManager() {
    if (sInstance == nullptr) {
        return;
    }
    infoPrintf("kill event system");
    fn_800094F0(lbl_80574FE8);
    delete EventManager::sInstance;
    EventManager::sInstance = nullptr;
}

dAcBase_c *registerInEvent(dAcBase_c *ac) {
    if (!ac->mDeleteRequest && !ac->checkProcControl(fBase_c::DISABLE_EXECUTE)) {
        ac->registerInEvent();
    }
    return ac;
}

dAcBase_c *draw(dAcBase_c *ac, void *cbArg) {
    if (!ac->mDeleteRequest && !ac->checkProcControl(fBase_c::DISABLE_EXECUTE)) {
        ac->unkVirtFunc_0x6C();
    }
    return ac;
}

bool EventManager::fn_8009E0A0() {
    EventManager *instance = sInstance;
    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (instance->field_0x004.field_0x00) {
        if (instance->field_0x004.mEventStarter.get() != link) {
            if ((instance->field_0x004.mEvent.mEventFlags & 0x1) != 0 &&
                !link->checkModelUpdateFlag(daPlayerActBase_c::UPDATE_MODEL_0x2000)) {
                return false;
            }

            if ((instance->field_0x004.mEvent.mEventFlags & 0x4) != 0 &&
                !link->checkModelUpdateFlag(daPlayerActBase_c::UPDATE_MODEL_0x20000000)) {
                return false;
            }
        }
    } else {
        return false;
    }
    return true;
}

void EventManager::fn_8009E110() {
    sInstance->mState = STATE_5;
    fn_800094F0(lbl_80574FE8);
}

void EventManager::execute() {
    if (sInstance == nullptr || sInstance->mState == STATE_5) {
        return;
    }

    bool canSkip = canSkipCurrentEvent();
    dAcPy_c *link = dAcPy_c::GetLinkM();

    if (link == nullptr || dScGame_c::getCamera() == nullptr) {
        return;
    }

    if (sInstance->mState == STATE_6 || sInstance->mState == STATE_7) {
        dZevEvent_c::GetInstance()->stopScene(getCurrentEvent()->mEvent.mEventName);
        fn_8009EEA0();
        if (!checkFlag(FLAGS_0x4)) {
            dAcBase_c::forEveryActor(draw, nullptr);
            sInstance->mState = STATE_0;
            dLytAreaCaption_c::check();
            dLytMsgWindow_c::getInstance()->fn_80117380();
            debugPrintf("event stop!");
            dScGame_c::getCamera()->setField_0x2A1(0);
            sDirector1.fn_800A18E0();
            sDirector2.fn_800A18E0();
            sSound.vt_0x10();
            sFlow.fn_800A4530();
            sEmitter.vt_0x10();
        }
    }

    if (sInstance->mState == STATE_0 && checkFlag(FLAGS_0x200000) && !checkFlag(FLAGS_0x20000) &&
        !checkFlag(FLAGS_0x4)) {
        if (dStage_c::GetInstance()->fadeIn(0, sParam1.field_0x06)) {
            sInstance->field_0x192 = sParam1.field_0x06;
            unsetFlag(FLAGS_0x200000);
            infoPrintf("fade in %d", sParam1.field_0x06);
            if (dLytMap_c::GetInstance() != nullptr) {
                dLytMap_c::GetInstance()->cancelEvent();
            }
            unsetFlag(FLAGS_0x20000);
        } else {
            infoPrintf("\x1b[41;37mfailed fade in ");
        }
    }
    unsetFlag(FLAGS_0x80000);

    if (sInstance->mState != STATE_0) {
        bool b = (dBase_c::s_ExecuteControlFlags & dBase_c::BASE_PROP_0x10) == 0 && !dZevEvent_c::GetInstance()->calc();
        if (sInstance->mState == STATE_3) {
            if (checkFlag(FLAGS_0x40000) && sInstance->field_0x192 != 0) {
                sInstance->field_0x192--;
                if (sInstance->field_0x192 == 0) {
                    sInstance->field_0x198 = 1;
                    debugPrintf("event %s skip trigger ", getCurrentEvent()->mEvent.mEventName.mChars);
                    setFlag(FLAGS_0x80000);
                    sInstance->field_0x190 = 2;
                }
            }
        } else if (b) {
            sInstance->field_0x198 = 1;
        } else if (canSkip && checkButton2HeldForCutscenes()) {
            if (dStage_c::GetInstance()->fadeOut(0, sParam1.field_0x06)) {
                debugPrintf("event %s skipped!", getCurrentEvent()->mEvent.mEventName.mChars);
                sInstance->mState = STATE_3;
                setFlag(FLAGS_0x20000);
                dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_SKIP);
                setFlag(FLAGS_0x40000);
                sInstance->field_0x192 = sParam1.field_0x06;
                infoPrintf("fade out %d", sParam1.field_0x06);
                if (dLytMsgWindow_c::getInstance() != nullptr) {
                    infoPrintf("message close");
                    dLytMsgWindow_c::getInstance()->setField_0x81B(1);
                    dLytMsgWindow_c::getInstance()->setField_0x819(1);
                    dLytMsgWindow_c::getInstance()->setField_0x81A(1);
                }
            } else {
                infoPrintf("\x1b[41;37mfailed fade out ");
            }
        }

        if (sInstance->field_0x198 != 0) {
            sInstance->field_0x198 = 0;
            sInstance->mState = STATE_4;
        }
    }

    if (sInstance->mState == STATE_4 && sInstance->field_0x190 == 0) {
        if (checkFlag(FLAGS_0x1)) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_READ_RIDDLE_A);
            unsetFlag(FLAGS_0x1);
            sInstance->field_0x190 = sParam1.field_0x04;
        } else if (checkFlag(FLAGS_0x8)) {
            dSndSmallEffectMgr_c::GetInstance()->playSound(SE_S_READ_RIDDLE_B);
            unsetFlag(FLAGS_0x8);
            sInstance->field_0x190 = sParam1.field_0x04;
        } else {
            sInstance->field_0x190 = 1;
        }
    }

    if (sInstance->mState == STATE_4) {
        if (checkFlag(FLAGS_0x4)) {
            sInstance->field_0x190 = -1;
        } else if (!checkFlag(FLAGS_0x2) && checkFlag(FLAGS_0x200) && sInstance->field_0x190 < 2) {
            sInstance->field_0x190 = 2;
        }
    }

    if (sInstance->field_0x190 > 0) {
        sInstance->field_0x190--;
        if (sInstance->field_0x190 == 0 && !checkFlag(FLAGS_0x4) && !checkFlag(FLAGS_0x8000)) {
            EventManager *inst = sInstance; // ???
            (void)dScGame_c::getCamera();
            if (inst->mCurrentEvent.mEvent.p.toolDataId != -1) {
                const EVNT *ev = getEVNTForRoomAndIndex(
                    inst->mCurrentEvent.mEvent.p.toolDataId, inst->mCurrentEvent.mEvent.p.roomId
                );
                bool changes = false;
                StoryflagManager *storyMgr = StoryflagManager::sInstance;
                if (storyMgr->checkFlagValid(ev->storyFlag1)) {
                    debugPrintf("event flag %d on!", ev->storyFlag1);
                    storyMgr->setFlag(ev->storyFlag1);
                    changes = true;
                }
                if (storyMgr->checkFlagValid(ev->storyFlag2)) {
                    debugPrintf("event flag %d on!", ev->storyFlag2);
                    storyMgr->setFlag(ev->storyFlag2);
                    changes = true;
                }
                SceneflagManager *sceneMgr = SceneflagManager::sInstance;
                if (ev->sceneFlag1 < 0xFF) {
                    debugPrintf("stage bit %d on!", ev->sceneFlag1);
                    sceneMgr->setTempOrSceneflag(ev->sceneFlag1);
                    changes = true;
                }
                if (ev->sceneFlag2 < 0xFF) {
                    debugPrintf("stage bit %d on!", ev->sceneFlag2);
                    sceneMgr->setTempOrSceneflag(ev->sceneFlag2);
                    changes = true;
                }

                ItemflagManager *itemMgr = ItemflagManager::sInstance;
                if (itemMgr->checkFlagValid(ev->item)) {
                    debugPrintf("item flag %d on!", ev->item);
                    itemMgr->setItemFlag(ev->item);
                    changes = true;
                }
                if (changes) {
                    setFlag(FLAGS_0x8000);
                    sInstance->field_0x190 = 1;
                }
            }
        }
        if (sInstance->field_0x190 == 0) {
            getCurrentEvent()->field_0x00 = 0;
            debugPrintf("event %s finish!", getCurrentEvent()->mEvent.mEventName.mChars);

            bool update = false;
            if (checkFlag(FLAGS_0x20000) && sInstance->field_0x0C4.field_0x00 &&
                (sInstance->field_0x0C4.mEvent.mEventFlags & 0x10) == 0) {
                setFlag(FLAGS_0x100);
                update = true;
            }
            if (!update && sInstance->field_0x124.field_0x00) {
                setFlag(FLAGS_0x100);
                update = true;
            }

            if (update) {
                sInstance->mState = STATE_6;
                debugPrintf("event continue!");
            } else {
                sInstance->mState = STATE_7;
            }
            dScGame_c::getCamera()->getEventCam()->fn_80091B90();
        }
    }

    if (sInstance->mState == STATE_0) {
        // ???
        bool tmp = sInstance->field_0x004.field_0x00 && fn_8009E0A0();
        if (tmp) {
            dScGame_c::getCamera()->getEventCam()->fn_80091B90();
            unsetFlag(
                FLAGS_0x1000000 | FLAGS_0x8000 | FLAGS_0x80 | FLAGS_0x40 | FLAGS_0x20 | FLAGS_0x10 | FLAGS_0x4 |
                FLAGS_0x2
            );
            copyQueuedEventToCurrentAndHandleSkipEvent();
            if (getCurrentEvent()->field_0x00) {
                debugPrintf("event %s start!", getCurrentEvent()->mEvent.mEventName.mChars);
                sInstance->field_0x196 = -1;

                void *data = getCurrentEvent()->mEvent.eventZevData;
                if (data != nullptr) {
                    dZevEvent_c::GetInstance()->loadFile(1, data);
                }
                dZevEvent_c::GetInstance()->startScene(
                    getCurrentEvent()->mEvent.mEventName, getCurrentEvent()->mEventStarter.get(),
                    getCurrentEvent()->mEventPartner.get()
                );

                sDirector1.start();
                sDirector2.start();
                sSound.start();
                sFlow.start();
                sEmitter.start();
                if (dZevEvent_c::GetInstance()->calc()) {
                    dAcBase_c::forEveryActor(draw, nullptr);
                    const EVNT *ev = getCurrentExecutedEVNT();
                    if (ev != nullptr && ev->field_0x0A == 1) {
                        setFlag(FLAGS_0x1);
                    } else if (ev != nullptr && ev->field_0x0A == 2) {
                        setFlag(FLAGS_0x8);
                    }
                } else {
                    debugPrintf("event %s finish!!", getCurrentEvent()->mEvent.mEventName.mChars);
                    sInstance->mState = STATE_4;
                }

                if (link->checkActionFlagsCont(0x400000)) {
                    dCamera_c *cam = dScGame_c::getCamera();
                    cam->setField_0x2A1(1);
                    cam->setField_0x2A0(0);
                }
            } else {
                sInstance->mState = STATE_4;
            }
        }
    }

    sInstance->field_0x004.clear();

    if (sInstance->mState != STATE_0) {
        if ((dBase_c::s_ExecuteControlFlags & dBase_c::BASE_PROP_0x10) == 0) {
            sDirector1.executeInEvent();
            sDirector2.executeInEvent();
            sSound.executeInEvent();
            sFlow.executeInEvent();
            sEmitter.executeInEvent();
            unsetFlag(0x4000 | 0x2000 | 0x1000 | 0x800 | 0x400 | 0x200 | 0x100);
        }
    } else {
        sInstance->field_0x192 = 0;
        sInstance->field_0x194 = 0;
        sInstance->field_0x190 = 0;
        unsetFlag(
            0x20000 | 0x8000 | 0x4000 | 0x2000 | 0x1000 | 0x800 | 0x400 | 0x200 | 0x100 | 0x80 | 0x40 | 0x20 | 0x10 |
            0x8 | 0x4 | 0x2
        );
        sInstance->field_0x18E = -1;
        sInstance->field_0x18C = -1;
    }
}

void EventManager::copyQueuedEventToCurrentAndHandleSkipEvent() {
    EventManager *instance = sInstance;
    getCurrentEvent()->copyFrom(sInstance->field_0x004);
    UNKWORD soundParam = -1;
    instance = sInstance;
    sInstance->field_0x0C4.clear();
    sInstance->field_0x18E = -1;
    sInstance->field_0x124.clear();

    if (instance->mCurrentEvent.mEvent.p.toolDataId != -1) {
        infoPrintf(
            "use tool data ID %d room %d", instance->mCurrentEvent.mEvent.p.toolDataId,
            instance->mCurrentEvent.mEvent.p.roomId
        );
        const EVNT *ev = getEVNTForRoomAndIndex(
            instance->mCurrentEvent.mEvent.p.toolDataId, instance->mCurrentEvent.mEvent.p.roomId
        );
        if (ev->skipFlag != 0xFF) {
            u8 skip = ev->skipFlag;
            infoPrintf("skip ID %d", skip);
            if (getCurrentEvent()->mEventStarter.isLinked()) {
                if (ev->skipEvent == 0xFF) {
                    attachSkipEvent(
                        getCurrentEvent()->mEventStarter.get(), skip, &sEventSkipOnly,
                        getCurrentEvent()->mEventPartner.get()
                    );
                } else {
                    Event e(ev->skipEvent, instance->mCurrentEvent.mEvent.p.roomId, 0, nullptr, nullptr);
                    attachSkipEvent(
                        getCurrentEvent()->mEventStarter.get(), skip, &e, getCurrentEvent()->mEventPartner.get()
                    );
                }
            }
        }
        if (ev->field_0x0E != 0xFF) {
            infoPrintf("next ID %d", ev->field_0x0E);
            if (getCurrentEvent()->mEventStarter.isLinked()) {
                Event e(ev->field_0x0E, instance->mCurrentEvent.mEvent.p.roomId, 0, nullptr, nullptr);
                setEvent(getCurrentEvent()->mEventStarter.get(), &e, getCurrentEvent()->mEventPartner.get());
            } else if (getCurrentEvent()->field_0x1C != -1) {
                Event e(ev->field_0x0E, instance->mCurrentEvent.mEvent.p.roomId, 0, nullptr, nullptr);
                fn_8009FE00(getCurrentEvent()->field_0x1C, &e, getCurrentEvent()->mEventPartner.get());
            }
        }

        setFlag(FLAGS_0x100);
        if (ev->field_0x12 != -1) {
            soundParam = ev->field_0x12;
        }
    }

    setState(STATE_2);
    Event::EventCallback cb = instance->mCurrentEvent.mEvent.mStartCallback;
    if (cb != nullptr) {
        (*cb)(getCurrentEvent()->mEventStarter.get());
    }
    dSndStateMgr_c::GetInstance()->setStbEventName(dStageMgr_c::GetInstance()->getDemoName());
    dSndStateMgr_c::GetInstance()->setEvent(getCurrentEvent()->mEvent.mEventName, soundParam);
}

void EventManager::fn_8009EEA0() {
    bool b1 = false;
    if (checkFlag(FLAGS_0x20000) && sInstance->field_0x0C4.field_0x00) {
        if ((sInstance->field_0x0C4.mEvent.mEventFlags & 0x10) == 0) {
            sInstance->field_0x004.copyFrom(sInstance->field_0x0C4);
            if (checkFlag(FLAGS_0x40000)) {
                unsetFlag(FLAGS_0x40000);
                setFlag(FLAGS_0x200000);
            }
            b1 = true;
            setFlag(FLAGS_0x100);
        } else {
            unsetFlag(FLAGS_0x20000);
            if (checkFlag(FLAGS_0x40000)) {
                unsetFlag(FLAGS_0x40000);
                setFlag(FLAGS_0x200000);
            }
        }
        sInstance->field_0x0C4.clear();
        dSndStateMgr_c::GetInstance()->onSkipEvent();
        if (dLytMsgWindow_c::getInstance() != nullptr) {
            infoPrintf("once more! message close");
            dLytMsgWindow_c::getInstance()->setField_0x81B(1);
            dLytMsgWindow_c::getInstance()->setField_0x819(1);
        }
        dRumble_c::stop(-1);
    } else if (checkFlag(FLAGS_0x20000)) {
        unsetFlag(FLAGS_0x20000);
    }

    if (!b1 && sInstance->field_0x124.field_0x00) {
        sInstance->field_0x004.copyFrom(sInstance->field_0x124);
        setFlag(FLAGS_0x100);
        sInstance->field_0x124.clear();
        b1 = true;
    }

    dCamera_c *cam = dScGame_c::getCamera();
    EventManager *instance = sInstance;

    bool b2 = false;

    if (sInstance->field_0x18C != 0xFFFF) {
        SkipflagManager *skipMgr = SkipflagManager::sInstance;
        if (sInstance->field_0x18C != sInstance->field_0x18E &&
            (sInstance->mCurrentEvent.mEvent.mEventFlags & 0x8) == 0) {
            // TODO ???
            bool tmp;
            if (((sInstance->field_0x18C >> 4) & 0xFFF) >= 0x10) {
                tmp = 0;
            } else {
                tmp = sInstance->field_0x18C >= 0 && sInstance->field_0x18C < 16;
            }
            if (tmp != 0 && !skipMgr->checkFlag(sInstance->field_0x18C)) {
                debugPrintf("skip flag %d on!", sInstance->field_0x18C);
                skipMgr->setFlag(sInstance->field_0x18C);
                SaveMgr::GetInstance()->writeSkipDat();
                sInstance->field_0x18E = sInstance->field_0x18C;
            }
        }
    }

    if (!b1 && instance->mCurrentEvent.mEvent.p.toolDataId != -1) {
        const EVNT *ev = getEVNTForRoomAndIndex(
            instance->mCurrentEvent.mEvent.p.toolDataId, instance->mCurrentEvent.mEvent.p.roomId
        );
        if (ev->exitId != 0xff) {
            dScGame_c::GetInstance()->triggerExit(
                instance->mCurrentEvent.mEvent.p.roomId, ev->exitId, SpawnInfo::RETAIN_TOD, SpawnInfo::RETAIN_TRIAL
            );
            b2 = true;
            setFlag(FLAGS_0x4);
        }
    }

    if (!b2) {
        if (lbl_80574FE8_getField_0x1b8(lbl_80574FE8) == 1) {
            fn_800094F0(lbl_80574FE8);
        }

        if (dLytMsgWindow_c::getInstance() != nullptr) {
            dLytMsgWindow_c::getInstance()->setField_0x81A(0);
        }

        Event::EventCallback cb = instance->mCurrentEvent.mEvent.mEndCallback;
        if (cb != nullptr) {
            (*cb)(getCurrentEvent()->mEventStarter.get());
        }

        sInstance->mCurrentEvent.clear();
        dZevEvent_c::GetInstance()->loadFile(1, nullptr);
        cam->getGameCam1()->onFlag(0x2);
    }

    cam->setField_0x1FC(0);
}

bool EventManager::alsoSetAsCurrentEvent(dAcBase_c *actor, const Event *event, dAcBase_c *actor2) {
    if (checkFlag(FLAGS_0x400)) {
        return false;
    }

    dAcPy_c *link = dAcPy_c::GetLinkM();
    if (actor != link) {
        if ((event->mEventFlags & 0x1) != 0 && !link->checkModelUpdateFlag(daPlayerActBase_c::UPDATE_MODEL_0x2000)) {
            return false;
        }

        if ((event->mEventFlags & 0x4) != 0 &&
            !link->checkModelUpdateFlag(daPlayerActBase_c::UPDATE_MODEL_0x20000000)) {
            return false;
        }
    }

    if (sInstance->field_0x004.mEvent.field_0x00 > event->field_0x00) {
        sInstance->field_0x004.field_0x00 = 1;
        sInstance->field_0x004.mEventStarter.link(actor);
        if (actor2 != nullptr) {
            sInstance->field_0x004.mEventPartner.link(actor2);
        } else {
            sInstance->field_0x004.mEventPartner.unlink();
        }
        sInstance->field_0x004.field_0x1C = -1;
        sInstance->field_0x004.mEvent = *event;

        StoryflagManager *storyMgr = StoryflagManager::sInstance;
        if (sInstance->field_0x004.mEvent.isName("CloudHole") ||
            sInstance->field_0x004.mEvent.isName("DiveSceneChange")) {
            if (!storyMgr->getCounterOrFlag(STORYFLAG_724)) {
                {
                    sInstance->field_0x004.mEvent = Event("NewWorld", 10, 0x0, nullptr, nullptr);
                }
                storyMgr->setFlag(STORYFLAG_724);
            } else if (dSalvageMgr_c::sInstance->mCurrentPickupIdx == SALVAGE_OBJ_WATER_JAR &&
                       !storyMgr->getCounterOrFlag(STORYFLAG_WATER_BASIN_ESCORT_STARTED)) {
                dAcPy_c *link = dAcPy_c::GetLinkM();
                infoPrintf("z %f", link->getPosition().z);

                if (link->getPosition().z > -82700.0f && link->getPosition().z < -72700.0f) {
                    sInstance->field_0x004.mEvent = Event("CloudHoleWithSalbo", 10, 0x0, nullptr, nullptr);
                }
            }
        }
    }
    return true;
}

bool EventManager::alsoSetAsCurrentEvent(int arg, const Event *event, dAcBase_c *actor) {
    if (checkFlag(FLAGS_0x400)) {
        return false;
    }

    if (sInstance->field_0x004.mEvent.field_0x00 > event->field_0x00 && arg >= 0 && arg <= 0xFF - 1) {
        sInstance->field_0x004.field_0x00 = 1;
        sInstance->field_0x004.mEventStarter.unlink();
        if (actor != nullptr) {
            sInstance->field_0x004.mEventPartner.link(actor);
        } else {
            sInstance->field_0x004.mEventPartner.unlink();
        }
        sInstance->field_0x004.field_0x1C = arg;
        sInstance->field_0x004.mEvent = *event;
    }

    return true;
}

void EventManager_Unused_EventCancel() {
    debugPrintf("event cancel");
}

bool EventManager::fn_8009F990() {
    bool ret = false;
    infoPrintf("2D request!");
    if (!sInstance->field_0x004.field_0x00) {
        if (!isInEvent()) {
            ret = true;
            setFlag(FLAGS_0x400);
        }
    }

    return ret;
}

bool EventManager::fn_8009FA10(dAcBase_c *actor, const Event *event, void *unknown) {
    return false;
}

extern "C" const u32 lbl_805720D0;
extern "C" const u32 lbl_805720D4;

bool EventManager::attachSkipEvent(dAcBase_c *actor, u8 flag, const Event *event, dAcBase_c *other) {
    SkipflagManager *skipMgr = SkipflagManager::sInstance;
    sInstance->field_0x18C = flag;
    infoPrintf("skip flag %d", flag);
    if (skipMgr->checkFlag(flag) && sInstance->mCurrentEvent.mEventStarter.get() == actor) {
        sInstance->field_0x0C4.field_0x00 = 1;
        sInstance->field_0x0C4.mEventStarter.link(actor);
        if (other != nullptr) {
            sInstance->field_0x0C4.mEventPartner.link(other);
        } else {
            sInstance->field_0x0C4.mEventPartner.unlink();
        }

        // No idea what's going on here
        sInstance->field_0x0C4.field_0x1C = -1;
        EventManager *instance = sInstance;
        instance->field_0x0C4.mEvent = *event;
        instance->field_0x0C4.mEvent.field_0x00 = 1;
        instance->field_0x0C4.mEvent.mEventFlags &= ~lbl_805720D0;
        if (instance->field_0x0C4.mEvent.mEventName == getSkipOnlyEventName()) {
            instance->field_0x0C4.mEvent.mEventFlags |= lbl_805720D4;
        }
        infoPrintf("skip enable", flag);
        return true;
    } else {
        return false;
    }
}

bool EventManager::setEvent(dAcBase_c *actor, const Event *event, dAcBase_c *other) {
    if (sInstance->mCurrentEvent.mEventStarter.get() == actor) {
        sInstance->field_0x124.field_0x00 = 1;
        sInstance->field_0x124.mEventStarter.link(actor);
        if (other != nullptr) {
            sInstance->field_0x124.mEventPartner.link(other);
        } else {
            sInstance->field_0x124.mEventPartner.unlink();
        }

        sInstance->field_0x124.field_0x1C = -1;
        EventManager *instance = sInstance;
        instance->field_0x124.mEvent = *event;
        instance->field_0x124.mEvent.field_0x00 = 1;
        return true;
    } else {
        return false;
    }
}

bool EventManager::changeOwnEvent(dAcBase_c *actor, dAcBase_c *other, const Event *event, dAcBase_c *third) {
    if (sInstance->mCurrentEvent.mEventStarter.get() == actor) {
        sInstance->field_0x124.field_0x00 = 1;
        sInstance->field_0x124.mEventStarter.link(other);
        if (third != nullptr) {
            sInstance->field_0x124.mEventPartner.link(third);
        } else {
            sInstance->field_0x124.mEventPartner.unlink();
        }

        sInstance->field_0x124.field_0x1C = -1;
        EventManager *instance = sInstance;
        instance->field_0x124.mEvent = *event;
        instance->field_0x124.mEvent.field_0x00 = 1;
        return true;
    } else {
        return false;
    }
}

bool EventManager::fn_8009FE00(UNKWORD arg, const Event *event, dAcBase_c *actor) {
    if (sInstance->mCurrentEvent.field_0x1C == arg) {
        sInstance->field_0x124.field_0x00 = 1;
        sInstance->field_0x124.mEventStarter.unlink();
        if (actor != nullptr) {
            sInstance->field_0x124.mEventPartner.link(actor);
        } else {
            sInstance->field_0x124.mEventPartner.unlink();
        }

        sInstance->field_0x124.field_0x1C = arg;
        EventManager *instance = sInstance;
        instance->field_0x124.mEvent = *event;
        instance->field_0x124.mEvent.field_0x00 = 1;
        return true;
    } else {
        return false;
    }
}

bool EventManager::isInEvent(dAcBase_c *actor, const char *eventName) {
    if (isInEvent() && actor == sInstance->mCurrentEvent.mEventStarter.get()) {
        if (eventName != nullptr) {
            if (sInstance->mCurrentEvent.mEvent.isName(eventName)) {
                return true;
            }
        } else {
            return true;
        }
    }

    return false;
}

bool EventManager::isInEvent(int arg, const char *eventName) {
    if (isInEvent() && arg == sInstance->mCurrentEvent.field_0x1C && arg >= 0 && arg <= 0xFF - 1) {
        if (eventName != nullptr) {
            if (sInstance->mCurrentEvent.mEvent.isName(eventName)) {
                return true;
            }
        } else {
            return true;
        }
    }

    return false;
}

bool EventManager::finishEvent(dAcBase_c *actor, const char *eventName) {
    infoPrintf("finish call!");
    if (isInEvent(actor, eventName) && sInstance->mState == STATE_2) {
        sInstance->field_0x198 = 1;
        setFlag(FLAGS_0x2);
        return true;
    }
    return false;
}

const EVNT *EventManager::getCurrentExecutedEVNT() {
    if (!isInEvent()) {
        return nullptr;
    }

    s32 roomId = getCurrentEvent()->mEvent.p.roomId;
    s32 toolDataId = getCurrentEvent()->mEvent.p.toolDataId;

    if (toolDataId == -1) {
        return nullptr;
    }

    if (roomId != -1) {
        return getEVNTForRoomAndIndex(toolDataId, roomId);
    } else {
        return getEVNTForActorAndIndex(toolDataId, getCurrentEvent()->mEventStarter.get());
    }
}

s32 EventManager::getCurrentEventRoomId() {
    return isInEvent() ? sInstance->mCurrentEvent.mEvent.p.roomId : -1;
}

void EventManager::onRoomChange() {
    // no-op
}

void EventManager::processEventFlags() {
    EventManager *instance = sInstance;

    if (sInstance->mState == STATE_2 && !checkFlag(FLAGS_0x8000)) {
        // branch copied from ::execute
        (void)dScGame_c::getCamera();
        if (instance->mCurrentEvent.mEvent.p.toolDataId != -1) {
            const EVNT *ev = getEVNTForRoomAndIndex(
                instance->mCurrentEvent.mEvent.p.toolDataId, instance->mCurrentEvent.mEvent.p.roomId
            );
            bool changes = false;
            StoryflagManager *storyMgr = StoryflagManager::sInstance;
            if (storyMgr->checkFlagValid(ev->storyFlag1)) {
                debugPrintf("event flag %d on!", ev->storyFlag1);
                storyMgr->setFlag(ev->storyFlag1);
                changes = true;
            }
            if (storyMgr->checkFlagValid(ev->storyFlag2)) {
                debugPrintf("event flag %d on!", ev->storyFlag2);
                storyMgr->setFlag(ev->storyFlag2);
                changes = true;
            }
            SceneflagManager *sceneMgr = SceneflagManager::sInstance;
            if (ev->sceneFlag1 < 0xFF) {
                debugPrintf("stage bit %d on!", ev->sceneFlag1);
                sceneMgr->setTempOrSceneflag(ev->sceneFlag1);
                changes = true;
            }
            if (ev->sceneFlag2 < 0xFF) {
                debugPrintf("stage bit %d on!", ev->sceneFlag2);
                sceneMgr->setTempOrSceneflag(ev->sceneFlag2);
                changes = true;
            }

            ItemflagManager *itemMgr = ItemflagManager::sInstance;
            if (itemMgr->checkFlagValid(ev->item)) {
                debugPrintf("item flag %d on!", ev->item);
                itemMgr->setItemFlag(ev->item);
                changes = true;
            }
            if (changes) {
                setFlag(FLAGS_0x8000);
            }
        }
    }

    // branch copied from ::fn_8009EEA0
    if (sInstance->field_0x18C != 0xFFFF) {
        SkipflagManager *skipMgr = SkipflagManager::sInstance;
        if (sInstance->field_0x18C != sInstance->field_0x18E &&
            (sInstance->mCurrentEvent.mEvent.mEventFlags & 0x8) == 0) {
            // TODO ???
            bool tmp;
            if (((sInstance->field_0x18C >> 4) & 0xFFF) >= 0x10) {
                tmp = 0;
            } else {
                tmp = sInstance->field_0x18C >= 0 && sInstance->field_0x18C < 16;
            }
            if (tmp != 0 && !skipMgr->checkFlag(sInstance->field_0x18C)) {
                debugPrintf("skip flag %d on!", sInstance->field_0x18C);
                skipMgr->setFlag(sInstance->field_0x18C);
                SaveMgr::GetInstance()->writeSkipDat();
                sInstance->field_0x18E = sInstance->field_0x18C;
            }
        }
    }
}

void EventManager::initEventActor(dAcBase_c *actor) {
    dZevEvent_c::GetInstance()->findAndInitEventActor(actor, nullptr);
}

bool EventManager::isActorInEvent(dAcBase_c *actor) {
    if (isInEvent() && dZevEvent_c::GetInstance()->isInit()) {
        return dZevEvent_c::GetInstance()->findActorIndexByPointer(actor) != -1;
    }
    return false;
}

void EventManager::clearEventActor(dAcBase_c *actor) {
    int idx = dZevEvent_c::GetInstance()->findActorIndexByPointer(actor);
    if (idx != -1) {
        dZevEvent_c::GetInstance()->getActorsPtr()[idx].clear();
    }
}

void EventManager::findAndForceInitEventActor(dAcBase_c *ac, const char *name) {
    dZevEvent_c::GetInstance()->findAndForceInitEventActor(ac, name);
}

void EventManager::findAndInitEventActor(u16 profileId, const char *name) {
    dZevEvent_c::GetInstance()->findAndInitEventActor(profileId, name);
}

dAcBase_c *EventManager::getCurrentEventActor() {
    if (isInEvent()) {
        return dZevEvent_c::GetInstance()->getEventStarter();
    }
    return nullptr;
}

dAcBase_c *EventManager::getEventPartner() {
    if (isInEvent()) {
        return dZevEvent_c::GetInstance()->getEventPartner();
    }
    return nullptr;
}

dAcBase_c *EventManager::getEventSpeaker() {
    if (isInEvent()) {
        return dZevEvent_c::GetInstance()->getEventSpeaker();
    }
    return nullptr;
}

void EventManager::setEventPartner(dAcBase_c *ac) {
    if (isInEvent()) {
        getCurrentEvent()->mEventPartner.link(ac);
        dZevEvent_c::GetInstance()->setEventPartner(ac);
    }
}

void EventManager::setEventSpeaker(dAcBase_c *ac) {
    if (isInEvent()) {
        dZevEvent_c::GetInstance()->setEventSpeaker(ac);
    }
}

void EventManager::setActorRef(int idx, dAcBase_c *ac) {
    if (isInEvent()) {
        dZevEvent_c::GetInstance()->setActorRef(idx, ac);
    }
}

dAcBase_c *EventManager::findActorBySpecialName(const char *name) {
    return dZevEvent_c::GetInstance()->findActorBySpecialName(name);
}

dAcBase_c *EventManager::getCurrentActorIfGroup(u8 groupType) {
    dAcBase_c *ac = dZevEvent_c::GetInstance()->getEventStarter();
    if (isInEvent()) {
        if (ac != nullptr && ac->mGroupType == groupType) {
            return ac;
        }
    }
    return nullptr;
}

dAcBase_c *EventManager::getSpeakerIfGroup(u8 groupType) {
    dAcBase_c *ac = dZevEvent_c::GetInstance()->getEventSpeaker();
    if (isInEvent()) {
        if (ac != nullptr && ac->mGroupType == groupType) {
            return ac;
        }
    }
    return nullptr;
}

bool EventManager::checkButton2HeldForCutscenes() {
    if (dPad::getDown2()) {
        sInstance->field_0x194++;
        if (sInstance->field_0x194 == 4) {
            return true;
        }
    } else {
        sInstance->field_0x194 = 0;
    }
    return false;
}

bool EventManager::canSkipCurrentEvent() {
    return sInstance->field_0x0C4.field_0x00 && sInstance->mState == STATE_2 && !checkFlag(FLAGS_0x1000000) &&
           dStage_c::GetInstance()->isFaderSettled() && !dStage_c::GetInstance()->isFadedOut() &&
           !dLytEndRoll_c::isField_0xF705() &&
           !(dLytControlGame_c::getInstance() != nullptr && dLytControlGame_c::getInstance()->isInSomeMapState()) &&
           (dBase_c::s_ExecuteControlFlags & dBase_c::BASE_PROP_0x10) == 0 &&
           dZevEvent_c::GetInstance()->getCurrentFrame() > sParam1.field_0x06 + 5;
}

bool EventManager::fn_800A0B80() {
    return checkFlag(FLAGS_0x80000);
}

bool EventManager::eventRelatedStateFlags_shift0x11_1() {
    return checkFlag(FLAGS_0x20000);
}

bool EventManager::fn_800A0BA0() {
    if (isInEvent() && dZevEvent_c::GetInstance()->isInit()) {
        return dZevEvent_c::GetInstance()->checkSceneMask(0x1) || checkFlag(FLAGS_0x400000);
    }
    return true;
}

Event::Event(const char *eventName, u32 unk, u32 eventFlags, EventCallback startCallback, EventCallback endCallback) {
    mEventName = eventName;
    field_0x00 = unk;
    p.toolDataId = -1;
    p.roomId = -1;
    mEventFlags = eventFlags;
    eventZevData = nullptr;
    this->mStartCallback = startCallback;
    this->mEndCallback = endCallback;
}

Event::Event(
    const char *eventName, void *zevDat, u32 unk, u32 eventFlags, EventCallback startCallback, EventCallback endCallback
) {
    mEventName = eventName;
    field_0x00 = unk;
    p.toolDataId = -1;
    p.roomId = -1;
    eventZevData = zevDat;
    mEventFlags = eventFlags;
    this->mStartCallback = startCallback;
    this->mEndCallback = endCallback;
}

Event::Event(s32 eventId, s32 roomId, u32 eventFlags, EventCallback startCallback, EventCallback endCallback) {
    const EVNT *ev = getEVNTForRoomAndIndex(eventId, roomId);
    if (ev->field_0x07 == 0) {
        mEventName = "JMAP";
    } else if (ev->field_0x07 == 2) {
        mEventName = "STB";
    } else {
        mEventName = ev->name;
    }
    field_0x00 = ev->field_0x06;
    p.toolDataId = eventId < 0xFF ? eventId : -1;
    p.roomId = roomId;
    mEventFlags = eventFlags | ev->field_0x0B;
    eventZevData = nullptr;
    this->mStartCallback = startCallback;
    this->mEndCallback = endCallback;
}

Event::Event(s32 eventId, s32 roomId, void *zevDat, u32 eventFlags, EventCallback startCallback, EventCallback endCallback) {
    const EVNT *ev = getEVNTForRoomAndIndex(eventId, roomId);
    if (ev->field_0x07 == 0) {
        mEventName = "JMAP";
    } else if (ev->field_0x07 == 2) {
        mEventName = "STB";
    } else {
        mEventName = ev->name;
    }
    field_0x00 = ev->field_0x06;
    p.toolDataId = eventId < 0xFF ? eventId : -1;
    p.roomId = roomId;
    eventZevData = zevDat;
    mEventFlags = eventFlags | ev->field_0x0B;
    this->mStartCallback = startCallback;
    this->mEndCallback = endCallback;
}

Event::~Event() {}
