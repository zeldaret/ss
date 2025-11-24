#include "d/a/obj/d_a_obj_door.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_room.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "sized_string.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_DOOR, dAcOdoor_c, fProfile::OBJ_DOOR, 0x188, 0, 7);

namespace {

const char *const DoorAnim = "DoorAnim";
const char *const DoorAnimPull = "DoorAnimPull";
const char *const DoorAnimPush = "DoorAnimPush";

struct DoorFileMapping {
    const u32 mType;
    const char *mResName;
    const char *mMdlName;
};
DoorFileMapping sDoorMappings[8] = {
    {0, "DoorA00", "DoorA00"},
    {1, "DoorA01", "DoorA01"},
    {6, "DoorA02", "DoorA02"},
    {4, "DoorB00", "DoorB00"},
    {2, "DoorC00", "DoorC00"},
    {3, "DoorC01", "DoorC01"},
    {5,   "DoorE", "DoorE_T"},
    {8,   "DoorH",   "DoorH"},
};

bool getDoorMapping(u32 type, const DoorFileMapping *&filemap) {
    bool found = false;
    int i = 0;
    while (!found && i <= (int)ARRAY_LENGTH(sDoorMappings) - 1) {
        if (sDoorMappings[i].mType == type) {
            found = true;
        } else {
            i++;
        }
    }

    if (found) {
        filemap = &sDoorMappings[i];
    }

    return found;
}

bool getDoorResName(u32 type, const char *&name) {
    const DoorFileMapping *filemap = nullptr;
    bool found = getDoorMapping(type, filemap);
    if (found) {
        name = filemap->mResName;
    }
    return found;
}

bool getDoorMdlName(u32 type, const char *&name) {
    const DoorFileMapping *filemap = nullptr;
    bool found = getDoorMapping(type, filemap);
    if (found) {
        name = filemap->mMdlName;
    }
    return found;
}

static u8 sDoorUnknown[2] = {0, 1};

bool getDoorUnknown(u8 search, u8 &idx) {
    bool found = false;
    int i = 0;
    while (!found && i < ARRAY_LENGTH(sDoorUnknown)) {
        if (sDoorUnknown[i] == search) {
            found = true;
        } else {
            i++;
        }
    }

    if (found) {
        idx = i;
    }

    return found;
}

static u32 sSpecialDoorMapTypes[5] = {
    sDoorMappings[3].mType, sDoorMappings[3].mType, sDoorMappings[4].mType,
    sDoorMappings[5].mType, sDoorMappings[6].mType,
};

bool isDoorSpecial(u32 type) {
    bool found = false;
    int i = 0;
    while (!found && i <= (int)ARRAY_LENGTH(sSpecialDoorMapTypes) - 1) {
        if (sSpecialDoorMapTypes[i] == type) {
            found = true;
        } else {
            i++;
        }
    }

    return found;
}

const char *getDoorInteractionName() {
    return "DoorTalk";
}

const char *const sSandShipOuterStages[2] = {"B301", "D301_1"};
bool isInOuterSandship() {
    bool found = true;
    int i = 0;
    while (found && i <= (int)ARRAY_LENGTH(sSandShipOuterStages) - 1) {
        if (dScGame_c::isCurrentStage(sSandShipOuterStages[i])) {
            found = false;
        } else {
            i++;
        }
    }

    return !found;
}

bool isInEvent(const char *eventName) {
    return strequals(EventManager::getCurrentEventName(), eventName);
}

const char *const sTimeDoorEventNames[2] = {
    "TimeDoorIn",
    "TimeDoorOut",
};

bool isInTimeDoorEvent() {
    int i = 0;
    bool found = true;
    while (found && i <= (int)ARRAY_LENGTH(sTimeDoorEventNames) - 1) {
        if (isInEvent(sTimeDoorEventNames[i])) {
            found = false;
        } else {
            i++;
        }
    }
    return !found;
}

bool checkRoom(u8 roomID, bool &b) {
    dRoom_c *pRoom = dStage_c::GetInstance()->getRoom(roomID);
    if (pRoom == nullptr) {
        return false;
    }

    b = pRoom->checkFlag(0x1E) != 0;
    return true;
}

} // namespace
const char *const DoorF = "DoorF";
const char *const DoorF_Open = "DoorF_Open";
const char *const DoorE_N = "DoorE_N";

bool dAcOdoor_c::createHeap() {
    s32 type = getFromParams(0, 0x3F);
    if (type == 7) {
        nw4r::g3d::ResFile resFile(getOarcResFile(DoorF));
        if (!resFile.IsValid()) {
            return false;
        }
        mAnmChr.create(resFile.ptr(), DoorF, DoorF_Open, &mAllocator, 0x120);
    } else {
        nw4r::g3d::ResFile resFileAnim(getOarcResFile(DoorAnim));
        if (!resFileAnim.IsValid()) {
            return false;
        }
        if (!mAnmChr.create(resFileAnim.ptr(), DoorAnim, DoorAnimPull, &mAllocator, 0)) {
            return false;
        }

        const char *resFileName = nullptr;
        getDoorResName(type, resFileName);
        if (resFileName == nullptr) {
            return false;
        }

        nw4r::g3d::ResFile resFileMdl(getOarcResFile(resFileName));
        if (!resFileMdl.IsValid()) {
            return false;
        }

        const char *resMdlName = nullptr;
        getDoorMdlName(type, resMdlName);
        if (resMdlName == nullptr) {
            return false;
        }

        nw4r::g3d::ResMdl resMdl = resFileMdl.GetResMdl(resMdlName);
        if (!resMdl.IsValid()) {
            return false;
        }

        if (!mMdl0.create(resMdl, &mAllocator, 0x120, 1, nullptr)) {
            return false;
        }

        if (fn_572_4430()) {
            nw4r::g3d::ResMdl resMdl = resFileMdl.GetResMdl(DoorE_N);
            if (!resMdl.IsValid()) {
                return false;
            }

            if (!mMdl1.create(resMdl, &mAllocator, 0x120, 1, nullptr)) {
                return false;
            }
        }
    }

    return true;
}

int dAcOdoor_c::actorCreate() {
    mSubtype = getFromParams(0, 0x3F);
    changeLoadedEntitiesWithSet();

    if (mSubtype == SUBTYPE_5 && !dScGame_c::isCurrentStage("B301") && !dScGame_c::isCurrentStage("D301_1")) {
        fn_572_4110();
    } else {
        fn_572_4120();
    }

    CREATE_ALLOCATOR(dAcOdoor_c);

    // if ()
}

int dAcOdoor_c::actorPostCreate() {}

int dAcOdoor_c::doDelete() {}

int dAcOdoor_c::actorExecuteInEvent() {}

int dAcOdoor_c::actorExecute() {}

int dAcOdoor_c::draw() {}

bool dAcOdoor_c::isLocked() {}

bool dAcOdoor_c::canOpen() {}

/** fn_572_33E0 - I Think this gets the Door open range*/
f32 dAcOdoor_c::fn_572_33E0() {} // -80.f

/** */
void dAcOdoor_c::startPullEventWithoutCallback(void *zevData) {}

/** */
void dAcOdoor_c::startPullEventWithCallback(void *zevData) {}

/** */
void dAcOdoor_c::startPushEventWithoutCallback(void *zevData) {}

/** */
void dAcOdoor_c::startPushEventWithCallback(void *zevData) {}

/** */
void dAcOdoor_c::startPullEvent(void *zevData) {}

/** */
void dAcOdoor_c::startPushEvent(void *zevData) {}

/** */
void dAcOdoor_c::startOpenEvent(void *zevData) {}

/** */
void dAcOdoor_c::doorPullEventCallback(dAcOdoor_c *) {}

/** */
void dAcOdoor_c::doorPushEventCallback(dAcOdoor_c *) {}

/** */
void dAcOdoor_c::startDoorPullLockedEvent(void *zevData) {}

/** */
void dAcOdoor_c::startDoorPushLockedEvent(void *zevData) {}

/** */
void dAcOdoor_c::startLockedEvent(void *zevData) {}

/** */
void dAcOdoor_c::pullLockedEventCallback(dAcOdoor_c *) {}

/** */
void dAcOdoor_c::pushLockedEventCallback(dAcOdoor_c *) {}

/** */
void dAcOdoor_c::startUnlockEvent() {}

/** */
void dAcOdoor_c::unlockEventCallback(dAcOdoor_c *) {}

/** */
void dAcOdoor_c::openCallbackCommon(dAcOdoor_c *) {}

void dAcOdoor_c::registerInEvent() {
    if (isInTimeDoorEvent()) {
        fn_572_40F0();
    }
}

void dAcOdoor_c::unkVirtFunc_0x6C() {}

void dAcOdoor_c::doInteraction(s32 interaction) {}

/** */
bool dAcOdoor_c::isKeyLocked() {} // getFromParams(6, 0x1)

/**  */
u8 dAcOdoor_c::getSubtype2() {} // (mRotation.x >> 0) & 0xFF

/**  */
u8 dAcOdoor_c::getSceneflag() {} // (mRotation.x >> 8) & 0xFF

/** fn_572_3F60 */
u8 dAcOdoor_c::fn_572_3F60() {} // (mRotation.z >> 0) & 0xF

/** fn_572_3F70 */
u8 dAcOdoor_c::fn_572_3F70() {} // (mRotation.z >> 4) & 0x3F

/** fn_572_3F80 */
u8 dAcOdoor_c::fn_572_3F80() {} // (mRotation.z >> 10) & 0x3F

/** fn_572_3F90 */
u8 dAcOdoor_c::fn_572_3F90() {} // (getParams2Lower() >> 0) & 0xFF

/** fn_572_3FC0 */
f32 dAcOdoor_c::getInteractionMinX() {} // -80.f

/** fn_572_3FD0 */
f32 dAcOdoor_c::getInteractionMaxX() {} // 80.f

/** fn_572_3FE0 */
f32 dAcOdoor_c::getInteractionMinZ() {} // -100.f

/** fn_572_3FF0 */
f32 dAcOdoor_c::getInteractionMaxZ() {} // 100.f

/** fn_572_4000 */
f32 dAcOdoor_c::getInteractionLockMinX() {} // -160.f

/** fn_572_4010 */
f32 dAcOdoor_c::getInteractionLockMaxX() {} // 160.f

/** fn_572_4020 */
f32 dAcOdoor_c::getInteractionLockMinZ() {} // -100.f

/** fn_572_4030 */
f32 dAcOdoor_c::getInteractionLockMaxZ() {} // 100.f

/** */
void dAcOdoor_c::setRoomId(s8 roomId) {} // mRoomId = roomId;

/** fn_572_4050 */
void dAcOdoor_c::fn_572_4050(u32 flags) {} // field_0x5A8 = flags

/** fn_572_4060 */
void dAcOdoor_c::fn_572_4060(u8) {} // field_0x5B3 = in

/** fn_572_4070 */
void dAcOdoor_c::fn_572_4070(s8) {} // field_0x5B4 = in

/** fn_572_4080 */
void dAcOdoor_c::fn_572_4080(s8) {} // field_0x5B5 = in

/** fn_572_4090 */
void dAcOdoor_c::fn_572_4090() {} // field_0x5B7 = true

/** fn_572_40A0 */
void dAcOdoor_c::fn_572_40A0() {} // field_0x5B7 = false

/** fn_572_40B0 */
void dAcOdoor_c::fn_572_40B0() {} // field_0x5B8 = true

/** fn_572_40C0 */
void dAcOdoor_c::fn_572_40C0() {} // field_0x5B8 = false

/** fn_572_40D0 */
void dAcOdoor_c::fn_572_40D0() {} // field_0x5B9 = true

/** fn_572_40E0 */
bool dAcOdoor_c::fn_572_40E0() const {} // field_0x5A8 & 1

/** fn_572_40F0 */
void dAcOdoor_c::fn_572_40F0() {} // field_0x5A8 |= 1 (Time Door)

/** fn_572_4100 */
void dAcOdoor_c::fn_572_4100() {} // field_0x5A8 &= ~1

/** fn_572_4110 */
void dAcOdoor_c::fn_572_4110() {} // field_0x5A8 |= 2

/** fn_572_4120 */
void dAcOdoor_c::fn_572_4120() {} // field_0x5A8 &= ~2

/** fn_572_4130 */
bool dAcOdoor_c::fn_572_4130(bool &b) const {} // checkRoom(field_0x5B4,b)

/** fn_572_4140 */
bool dAcOdoor_c::fn_572_4140(bool &b) const {} // checkRoom(field_0x5B5,b)

/** fn_572_4150 */
bool dAcOdoor_c::fn_572_4150(bool &b) const {} // fn_572_4130(b0) && fn_572_4140(b1) -> b = b0 && b1

/** */
void dAcOdoor_c::triggerExit() {}

/** */
bool dAcOdoor_c::isLeavingSealedTempleSideDoorPostSkyKeep() const {}

/** fn_572_4310 */
void dAcOdoor_c::transitionPushRoomFlags() const {}

/** fn_572_4320 */
void dAcOdoor_c::fn_572_4320() const {} // nop

/** fn_572_4430 */
bool dAcOdoor_c::transitionPullRoomFlags() const {}

/** fn_572_4340 */
void dAcOdoor_c::fn_572_4340() const {} // nop

/** fn_572_4370 */
bool dAcOdoor_c::fn_572_4370(const mVec3_c &point) const {} // Is in front of door

/** */
bool dAcOdoor_c::isPlayerInFrontOfDoor() const {}

/** */
bool dAcOdoor_c::hasFlowEntryPoint() const {} // getFromParams(16, 0xFFFF)

/** fn_572_4430 */
bool dAcOdoor_c::fn_572_4430() const {} // field_0x5A8 & 2

/** fn_572_4440 */
void dAcOdoor_c::fn_572_4440() {} // Sets the Postion Copies

/** */
bool dAcOdoor_c::isPlayerInteractable() const {}

/** fn_572_4600 */
bool dAcOdoor_c::isPlayerInteractableLocked() const {}

/** */
void dAcOdoor_c::playInteractionLocked() const {}
