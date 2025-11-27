#include "d/a/obj/d_a_obj_door.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_fence.h"
#include "d/a/obj/d_a_obj_lock.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/c/c_m3d_g_aab.h"
#include "d/d_room.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/math/eggMath.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_anmmdl.h"
#include "m/m3d/m_fanm.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_arithmetic.h"
#include "s/s_Math.h"
#include "sized_string.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_DOOR, dAcOdoor_c, fProfile::OBJ_DOOR, 0x188, 0, 7);

namespace {

const char *const DoorAnim = "DoorAnim";
const char *const DoorAnim_ = "DoorAnim";
const char *const DoorAnimPull = "DoorAnimPull";
const char *const DoorAnimPush = "DoorAnimPush";

struct DoorFileMapping {
    const dAcOdoor_c::Subtype_e mType;
    const char *mResName;
    const char *mMdlName;
};
DoorFileMapping sDoorMappings[8] = {
    {           dAcOdoor_c::DOOR_NORMAL, "DoorA00", "DoorA00"},
    {            dAcOdoor_c::DOOR_METAL, "DoorA01", "DoorA01"},
    {         dAcOdoor_c::DOOR_BATREAUX, "DoorA02", "DoorA02"},
    {      dAcOdoor_c::DOOR_TEMPLE_SIDE, "DoorB00", "DoorB00"},
    {           dAcOdoor_c::DOOR_DOUBLE, "DoorC00", "DoorC00"},
    {   dAcOdoor_c::DOOR_DOUBLE_OUTSIDE, "DoorC01", "DoorC01"},
    {dAcOdoor_c::DOOR_SANDSEA_TIMESHIFT,   "DoorE", "DoorE_T"},
    {    dAcOdoor_c::DOOR_SPARRING_HALL,   "DoorH",   "DoorH"},
};

const char *const DoorF = "DoorF";
const char *const DoorF_ = "DoorF";
const char *const DoorF_Open = "DoorF_Open";
const char *const DoorF_Close = "DoorF_Close";

static const u8 sDoorCallbackTypeMap[2] = {
    0, // Doesnt have callbacks
    1  // Has Callbacks
};

const dAcOdoor_c::Subtype_e sDoubleDoorTypes[5] = {
    sDoorMappings[3].mType, // dAcOdoor_c::DOOR_TEMPLE_SIDE
    sDoorMappings[4].mType, // dAcOdoor_c::DOOR_DOUBLE
    sDoorMappings[5].mType, // dAcOdoor_c::DOOR_DOUBLE_OUTSIDE
    sDoorMappings[6].mType, // dAcOdoor_c::DOOR_SANDSEA_TIMESHIFT
    sDoorMappings[7].mType, // dAcOdoor_c::DOOR_SPARRING_HALL
};

const char *const DoorPull = "DoorPull";
const char *const DoorPush = "DoorPush";
const char *const DoorPullLock = "DoorPullLock";
const char *const DoorPushLock = "DoorPushLock";
const char *const DoorTalk = "DoorTalk";

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

bool getDoorCallbackTypeMapped(u8 search, u8 &idx) {
    bool found = false;
    int i = 0;
    while (!found && i <= (int)ARRAY_LENGTH(sDoorCallbackTypeMap) - 1) {
        if (sDoorCallbackTypeMap[i] == search) {
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

bool isDoubleDoorType(s32 type) {
    bool found = false;
    int i = 0;
    while (!found && i <= (int)ARRAY_LENGTH(sDoubleDoorTypes) - 1) {
        if (sDoubleDoorTypes[i] == type) {
            found = true;
        } else {
            i++;
        }
    }

    return found;
}
const char *getDoorInteractionName() {
    return DoorTalk;
}

const char *const sSandShipBossStages[2] = {"B301", "D301_1"};
bool isInSandshipBoss() {
    bool found = true;
    int i = 0;
    while (found && i <= (int)ARRAY_LENGTH(sSandShipBossStages) - 1) {
        if (dScGame_c::isCurrentStage(sSandShipBossStages[i])) {
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

bool checkRoom(s8 roomID, bool &b) {
    dRoom_c *pRoom = dStage_c::GetInstance()->getRoom(roomID);
    if (pRoom == nullptr) {
        return false;
    }

    b = pRoom->checkFlag(0x1E) != 0;
    return true;
}

} // namespace

void rodata0() {
    const f32 a[] = {0.f, 16.f, 60.f, 0.f, 80.f, 100.f, 0.f, 160.f, 100.f};
    const u32 b[] = {0x2D2D7832, 0x73010000, 0x03000000};
}
static mVec3_c sDoorknobOffset0(70.f, 0.f, 0.f);
static mVec3_c sDoorknobOffset1(52.f, 110.f, 0.f);

static mVec3_c sVec2(0.f, 0.f, 70.f);
static mVec3_c sVec3(0.f, 0.f, -70.f);

static const InteractionTargetDef sOpenInteraction = {
    0, 2, 1, OPEN, 0, 1000.f, 60.f, 0.f, -100.f, 100.f, 50.f, 1.f,
};
static const InteractionTargetDef sLockedInteraction = {
    0, 2, 1, EXAMINE_TALK, 0, 1000.f, 60.f, 0.f, -100.f, 100.f, 50.f, 1.f,
};
void rodata1() {
    const f32 a[] = {200.f, 0.f};
}

SizedString<32> sStageF402("F402");

bool dAcOdoor_c::createHeap() {
    s32 type = getFromParams(0, 0x3F);
    if (type == DOOR_TEMPLE_MAIN) {
        nw4r::g3d::ResFile resFile(getOarcResFile(DoorF));
        if (!resFile.IsValid()) {
            return false;
        }
        mAnmChr.create(resFile.ptr(), DoorF_, DoorF_Open, &mAllocator, 0x120);
    } else {
        nw4r::g3d::ResFile resFileAnim(getOarcResFile(DoorAnim));
        if (!resFileAnim.IsValid()) {
            return false;
        }
        if (!mAnmChr.create(resFileAnim.ptr(), DoorAnim_, DoorAnimPull, &mAllocator, 0)) {
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

        if (isTimeEffected()) {
            nw4r::g3d::ResMdl resMdl = resFileMdl.GetResMdl("DoorE_N");
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
    // NONMATCHING - Regalloc issues
    s32 subtype = getType();
    setSubtype(subtype);
    changeLoadedEntitiesWithSet();

    if (subtype == DOOR_SANDSEA_TIMESHIFT &&
        !(dScGame_c::isCurrentStage("B301") || dScGame_c::isCurrentStage("D301_1"))) {
        setTimeEffected();
    } else {
        setTimeStatic();
    }

    CREATE_ALLOCATOR(dAcOdoor_c);

    mSceneflag = getSceneflag();

    if (getLockParameter() == LOCK_KEY) {
        if (mSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
            mLock.link(
                static_cast<dAcOLock_c *>(
                    create(this, fProfile::OBJ_SHUTTER_LOCK, 0, nullptr, nullptr, nullptr, 0, 0x3F)
                )
            );
        }
    } else {
        u8 type = getLockBehavior();
        if (type <= LOCK_NIGHT) {
            mLockBehavior = type;
        } else {
            mLockBehavior = LOCK_NEVER;
        }
    }

    u8 callbackType;
    if (!getDoorCallbackTypeMapped(getDoorCallbackType(), callbackType)) {
        callbackType = 0; // Default to no callback
    }
    setEventCallbackType(callbackType);

    setFrontRoom(getFrontRoomParam());
    setBackRoom(getBackRoomParam());

    mRotation.z = mAng(0);
    mRotation.x = mAng(0);
    {
        s32 roomId = mRoomID;
        addActorToRoom(-1);
        setRoomId(roomId);
    }

    if (isDoubleDoorType(subtype)) {
        setDoubleDoor();
    } else {
        setNotDoubleDoor();
    }

    if (getType() == DOOR_TEMPLE_MAIN) {
        mAnmChr.setAnm(DoorF_Open, m3d::PLAY_MODE_4, 0.f);
        mAnmChr.setFrame(mAnmChr.getAnm().getStartFrame());
        mAnmChr.getModel().setPriorityDraw(0x1C, 0);
    } else {
        mAnmChr.setAnm(DoorAnimPull, m3d::PLAY_MODE_4, 0.f);
        mAnmChr.setFrame(mAnmChr.getAnm().getStartFrame());
        if (isTimeEffected()) {
            mMdl0.setPriorityDraw(0xF, 0);
            mMdl1.setPriorityDraw(0xB, 0);
        } else {
            mMdl0.setPriorityDraw(0x1C, 0);
        }
    }

    bool inAcademy = dScGame_c::isCurrentStage("F001r");
    if (inAcademy) {
        inAcademy = mVec3_c(1832.5f, 0.f, -1978.0f).squareDistanceToXZ(mPosition) < 100.f;
    }
    mbKobunDoor = inAcademy;

    return SUCCEEDED;
}

extern "C" void fn_80067290(dTimeBits *pBits, s32 roomId, const mVec3_c &pos, f32);

int dAcOdoor_c::actorPostCreate() {
    if (mbDoubleDoor) {
        if (!isConnectedToOtherDoor()) {
            bool foundPair = false;
            dAcOdoor_c *pDoor =
                static_cast<dAcOdoor_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_DOOR, nullptr));
            while (!foundPair && pDoor != nullptr) {
                if (mID != pDoor->mID && pDoor->mbDoubleDoor && !pDoor->isConnectedToOtherDoor() &&
                    mPosition.squareDistance(pDoor->mPosition) < 40000.f) {
                    foundPair = true;
                    this->getConnectedDoorRef().link(pDoor);
                    pDoor->getConnectedDoorRef().link(this);

                    // Not really sure the point of this
                    // How does this prevent Door ordering issues
                    // This may be why the field goes unused?
                    if (getLockParameter() == LOCK_KEY) {
                        this->setSingleDoor();
                        pDoor->setNotSingleDoor();
                    } else {
                        this->setNotSingleDoor();
                        pDoor->setSingleDoor();
                    }
                }
                pDoor = static_cast<dAcOdoor_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_DOOR, pDoor));
            }

            if (!foundPair) {
                setSingleDoor();
            }
        }
    } else {
        setSingleDoor();
    }

    mVec3_c pos = mPosition + mVec3_c::Ey * 10.f;
    bool gndChk = dBgS_ObjGndChk::CheckPos(pos);
    dBgS_ObjGndChk *objGndCheck = dBgS_ObjGndChk::GetPInstance();
    if (gndChk) {
        mLightingInfo.mLightingCode = dBgS_ObjGndChk::GetLightingCode();
        if (objGndCheck != nullptr) {
            setPolyAttrs(*objGndCheck);
        }
    }

    if (isInSandshipBoss()) {
        setInSandshipBoss();
        unsetActorProperty(AC_PROP_0x1);

        if (gndChk) {
            dBgS *pBgS = dBgS::GetInstance();
            if (pBgS != nullptr && objGndCheck != nullptr) {
                const dAcObjBase_c *pObj = pBgS->GetActorPointer(*objGndCheck);
                if (pObj) {
                    mObjRef.link(const_cast<dAcObjBase_c *>(pObj));
                    mVec3_c v(mPosition.x, dBgS_ObjGndChk::GetGroundHeight(), mPosition.z);
                    v -= pObj->mPosition;

                    v.rotY(-pObj->mRotation.y);

                    mAng rotY = mRotation.y - pObj->mRotation.y;
                    mMtx.transS(v);
                    mMtx.YrotM(rotY);
                }
            }
        }
    }

    if (!mObjRef.isLinked()) {
        s32 type = getType();
        updateMatrix();
        mAnmChr.getModel().setLocalMtx(mWorldMtx);
        mAnmChr.getModel().calc(false);

        if (type != DOOR_TEMPLE_MAIN) {
            mMtx_c m;
            mAnmChr.getModel().getNodeWorldMtx(1, m);
            m.transM(sDoorknobOffset0);
            mMdl0.setLocalMtx(m);
            if (isTimeEffected()) {
                mMdl1.setLocalMtx(m);
            }
        }

        dAcOLock_c *pLock = mLock.get();
        if (pLock == nullptr) {
            setDoorKnobPosition();
        } else {
            mPositionCopy2.set(pLock->mPosition);
            mPositionCopy3.set(pLock->mPosition);
        }

        mVec3_c min, max;
        min.set(999999.f, 999999.f, 999999.f);
        max.set(-999999.f, -999999.f, -999999.f);

        if (type == DOOR_TEMPLE_MAIN) {
            mAnmChr.getModel().getBounds(&min, &max);
        } else {
            mVec3_c min_l, max_l;
            mMtx_c local;
            mMdl0.getBounds(&min_l, &max_l);
            mMdl0.getLocalMtx(local);

            mMtx_c world = mWorldMtx;
            world.inverse();

            mVec3_c minPos = min_l;
            local.multVec(minPos, minPos);
            world.multVec(minPos, minPos);

            // clang-format off
            min.set(
                nw4r::ut::Min(minPos.x, min.x),
                nw4r::ut::Min(minPos.y, min.y),
                nw4r::ut::Min(minPos.z, min.z)
            );
            max.set(
                nw4r::ut::Max(minPos.x, max.x),
                nw4r::ut::Max(minPos.y, max.y),
                nw4r::ut::Max(minPos.z, max.z)
            );
            // clang-format on

            mVec3_c maxPos = max_l;
            local.multVec(maxPos, maxPos);
            world.multVec(maxPos, maxPos);
            // clang-format off
            min.set(
                nw4r::ut::Min(maxPos.x, min.x),
                nw4r::ut::Min(maxPos.y, min.y),
                nw4r::ut::Min(maxPos.z, min.z)
            );
            max.set(
                nw4r::ut::Max(maxPos.x, max.x),
                nw4r::ut::Max(maxPos.y, max.y),
                nw4r::ut::Max(maxPos.z, max.z)
            );
            // clang-format on

            mVec3_c vecs[6] = {
                mVec3_c(min_l.x, min_l.y, max_l.z), mVec3_c(min_l.x, max_l.y, min_l.z),
                mVec3_c(min_l.x, max_l.y, max_l.z), mVec3_c(max_l.x, min_l.y, min_l.z),
                mVec3_c(max_l.x, min_l.y, max_l.z), mVec3_c(max_l.x, max_l.y, min_l.z),
            };

            for (int i = 0, j = 0; i < (int)ARRAY_LENGTH(vecs); i++, j++) {
                local.multVec(vecs[i], vecs[j]);
                world.multVec(vecs[i], vecs[j]);
                // clang-format off
                min.set(
                    nw4r::ut::Min(vecs[j].x, min.x),
                    nw4r::ut::Min(vecs[j].y, min.y),
                    nw4r::ut::Min(vecs[j].z, min.z)
                );
                max.set(
                    nw4r::ut::Max(vecs[j].x, max.x),
                    nw4r::ut::Max(vecs[j].y, max.y),
                    nw4r::ut::Max(vecs[j].z, max.z)
                );
                // clang-format on
            }
        }
        mBoundingBox.Set(min, max);
    }

    if (isTimeEffected()) {
        fn_80067290(&mTimeBits, mRoomID, mPosition, 200.f);
    }

    mCullingDistance = EGG::Math<f32>::maxNumber();

    return SUCCEEDED;
}

int dAcOdoor_c::doDelete() {
    changeLoadedEntitiesNoSet();
    return SUCCEEDED;
}

int dAcOdoor_c::actorExecuteInEvent() {
    if (isTimeEffected()) {
        fn_80067290(&mTimeBits, mRoomID, mPosition, 200.f);
    }

    if (mEventRelated.isAdvance()) {
        mFramesInEvent = 0;
    } else if (mFramesInEvent < 0xFF) {
        mFramesInEvent++;
    }

    if (isTimeDoorEventActive()) {
        setObjectProperty(OBJ_PROP_0x200);
        return SUCCEEDED;
    }

    switch (mEventCallbackType) {
        case 0: {
            bool b;
            checkRoom(mRoomID, b);
            if (b) {
                setObjectProperty(OBJ_PROP_0x200);
                return SUCCEEDED;
            }
        } break;
        case 1: {
            bool b;
            checkRooms(b);
            if (b) {
                setObjectProperty(OBJ_PROP_0x200);
                return SUCCEEDED;
            }
        } break;
    }

    unsetObjectProperty(OBJ_PROP_0x200);
    s32 type = getType();

    static mVec3_c v1(0.f, 0.f, 70.f);
    static mVec3_c v2(0.f, 0.f, -70.f);

    f32 frame0 = -1.f;
    f32 frame1 = -1.f;

    switch (mEventRelated.getCurrentEventCommand()) {
        case 'pllB': {
            f32 frame = dAcPy_c::GetLink()->getCurrentAnimFrame();
            if (mEventRelated.isAdvance()) {
                mAnmChr.setAnm(DoorAnimPull, m3d::PLAY_MODE_4);
                mAnmChr.setFrame(frame);
                mExitTimer = 45;

                frame0 = frame;
                if (mEventCallbackType == s8(1)) {
                    transitionPullRoomFlags();
                }
            } else {
                frame0 = mAnmChr.getAnm().getFrame();
            }
            mAnmChr.setFrame(frame);
            frame1 = mAnmChr.getAnm().getFrame();
            bool old_time = 0 == mExitTimer;
            bool new_time = 0 == sLib::calcTimer(&mExitTimer);
            switch (mEventCallbackType) {
                case 0: {
                    if (!old_time && new_time) {
                        triggerExit();
                    }
                } break;
                case 1: {
                    if (new_time) {
                        mEventRelated.advanceNext();
                    }
                } break;
            }
        } break;
        case 'pllE': {
            bool isStop;
            const dAcPy_c *pPlayer = dAcPy_c::GetLink();
            f32 frame = pPlayer->getCurrentAnimFrame();
            if (mEventRelated.isAdvance()) {
                isStop = false;
                switch (mEventCallbackType) {
                    case 0: {
                        frame0 = frame;
                    } break;
                    case 1: {
                        frame0 = mAnmChr.getAnm().getFrame();
                    } break;
                }
                mAnmChr.setAnm(DoorAnimPull, m3d::PLAY_MODE_4);
                mAnmChr.setFrame(frame);
            } else {
                isStop = mAnmChr.getAnm().isStop();
                frame0 = mAnmChr.getAnm().getFrame();

                if (!isStop) {
                    mAnmChr.setFrame(frame);
                }
            }
            bool postStop = mAnmChr.getAnm().isStop();
            frame1 = mAnmChr.getAnm().getFrame();
            if (!isStop && postStop) {
                if (mEventCallbackType == 1 && getField_0x5B4() != getField_0x5B5()) {
                    dRoom_c *pRoom = dStage_c::GetInstance()->getRoom(getField_0x5B4());
                    if (pRoom != nullptr) {
                        pRoom->setFlag(0x4);
                        dStage_c::GetInstance()->fn_801B3C60(getField_0x5B5());
                    }
                }
                mEventRelated.advanceNext();

                bool search = true;
                dAcOFence_c *pFence =
                    static_cast<dAcOFence_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_FENCE, nullptr));
                while (search && pFence != nullptr) {
                    if (mAng::abs(pPlayer->mRotation.y - pFence->mRotation.y) < 0x4000) {
                        const mVec3_c &posFence = pFence->mPosition;
                        const mVec3_c &position = mPosition;
                        if (posFence.squareDistanceToXZ(position) < 22500.f &&
                            nw4r::math::FAbs(posFence.y - position.y) < 500.f) {
                            search = false;
                            continue;
                        }
                    }
                    pFence = static_cast<dAcOFence_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_FENCE, pFence));
                }

                if (!search) {
                    if (pFence->fn_550_11B0()) {
                        pFence->changeToRequestConfineEvent();
                    }
                    pFence->fn_550_12C0();
                }

                if (mLock.get() != nullptr) {
                    mLock.get()->deleteRequest();
                }
            }
        } break;
        case 'pshB': {
            f32 frame = dAcPy_c::GetLink()->getCurrentAnimFrame();
            if (mEventRelated.isAdvance()) {
                if (type == DOOR_TEMPLE_MAIN) {
                    mAnmChr.setAnm(DoorF_Open, m3d::PLAY_MODE_4);
                    mExitTimer = 120;
                } else {
                    mAnmChr.setAnm(DoorAnimPush, m3d::PLAY_MODE_4);
                    mExitTimer = 45;
                }
                if (mEventCallbackType == s8(1)) {
                    transitionPushRoomFlags();
                }
                frame0 = frame;
            } else {
                frame0 = mAnmChr.getAnm().getFrame();
            }
            mAnmChr.setFrame(frame);
            frame1 = mAnmChr.getAnm().getFrame();
            bool old_time = 0 == mExitTimer;
            bool new_time = 0 == sLib::calcTimer(&mExitTimer);
            switch (mEventCallbackType) {
                case 0: {
                    if (!old_time && new_time) {
                        triggerExit();
                    }
                } break;
                case 1: {
                    if (new_time) {
                        mEventRelated.advanceNext();
                    }
                } break;
            }
        } break;
        case 'pshE': {
            bool isStop;
            f32 frame = dAcPy_c::GetLink()->getCurrentAnimFrame();
            if (mEventRelated.isAdvance()) {
                isStop = false;
                switch (mEventCallbackType) {
                    case 0: {
                        frame0 = frame;
                    } break;
                    case 1: {
                        frame0 = mAnmChr.getAnm().getFrame();
                    } break;
                }
                if (type == DOOR_TEMPLE_MAIN) {
                    mAnmChr.setAnm(DoorF_Close, m3d::PLAY_MODE_4);
                } else {
                    mAnmChr.setAnm(DoorAnimPush, m3d::PLAY_MODE_4);
                }
                mAnmChr.setFrame(frame);
            } else {
                isStop = mAnmChr.getAnm().isStop();
                frame0 = mAnmChr.getAnm().getFrame();
                if (!isStop) {
                    mAnmChr.setFrame(frame);
                }
            }
            bool stop = mAnmChr.getAnm().isStop();
            frame1 = mAnmChr.getAnm().getFrame();
            if (!isStop && stop) {
                if (mEventCallbackType == 1 && getField_0x5B4() != getField_0x5B5()) {
                    dRoom_c *pRoom = dStage_c::GetInstance()->getRoom(getField_0x5B5());
                    if (pRoom != nullptr) {
                        pRoom->setFlag(0x4);
                        dStage_c::GetInstance()->fn_801B3C60(getField_0x5B4());
                    }
                }
                mEventRelated.advanceNext();

                bool search = true;
                const dAcPy_c *pPlayer = dAcPy_c::GetLink();
                dAcOFence_c *pFence =
                    static_cast<dAcOFence_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_FENCE, nullptr));
                while (search && pFence != nullptr) {
                    if (mAng::abs(pPlayer->mRotation.y - pFence->mRotation.y) < 0x4000) {
                        const mVec3_c &posFence = pFence->mPosition;
                        const mVec3_c &position = mPosition;
                        if (posFence.squareDistanceToXZ(position) < 22500.f &&
                            nw4r::math::FAbs(posFence.y - position.y) < 500.f) {
                            search = false;
                            continue;
                        }
                    }
                    pFence = static_cast<dAcOFence_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_FENCE, pFence));
                }

                if (!search) {
                    if (pFence->fn_550_11B0()) {
                        pFence->changeToRequestConfineEvent();
                    }
                    pFence->fn_550_12C0();
                }

                if (mLock.get() != nullptr) {
                    mLock.get()->deleteRequest();
                }
            }
            if (type == DOOR_TEMPLE_MAIN) {
                bool b;
                if (frame0 == frame1) {
                    b = frame0 == 55.f;
                } else {
                    b = frame0 < 55.f && 55.f <= frame1;
                }
                if (b) {
                    dRumble_c::start(dRumble_c::sRumblePreset5, dRumble_c::FLAG_SLOT0);
                }
            }
        } break;
        case 'wait': {
            mEventRelated.advanceNext();
        } break;
        case 'tktn': {
            mEventRelated.advanceNext();
        } break;
        case 'tkwt': {
            mEventRelated.advanceNext();
        } break;
        case 'tked': {
            mEventRelated.advanceNext();
        } break;
        case 'cAna': {
            mVec3_c pos;
            mAng rot;
            if (type == DOOR_TEMPLE_MAIN) {
                pos.set(0.f, 0.f, 85.f);
                rot = mAng(0x8000);
                rot += mRotation.y;
            } else {
                int dir;
                mEventRelated.getSingleShortData(&dir, 'dir ', 0);
                if (dir == 0) {
                    pos.set(v1);
                    rot = mAng(0x8000);
                    rot += mRotation.y;
                } else {
                    pos.set(v2);
                    rot = mRotation.y;
                }
            }
            stepTowards(pos);
            dAcPy_c *pPlayer = dAcPy_c::GetLinkM();

            if (mFramesInEvent > 30) {
                pPlayer->setPosYRot(pos, rot);
                mEventRelated.advanceNext();
            } else {
                mVec3_c tmp = pPlayer->mPosition;
                cLib::addCalcPos(&tmp, pos, 0.25f, 200.f, 0.f);

                mAng ang = pPlayer->mRotation.y;
                sLib::addCalcAngle(ang.ref(), rot, 4, 0x7FFF, 0);

                if (pos.squareDistanceToXZ(tmp) < 25.f && mAng::abs(ang - mAng((s32)rot)) < 182) {
                    pPlayer->setPosYRot(pos, rot);
                    mEventRelated.advanceNext();
                } else {
                    pPlayer->setPosYRot(tmp, ang);
                }
            }
        } break;
        case 'cDeg': {
            mVec3_c pos;
            mAng rot;
            if (mEventRelated.isAdvance()) {
                if (type == DOOR_TEMPLE_MAIN) {
                    pos.set(0.f, 0.f, 90.f);
                    rot = mRotation.y;
                } else {
                    int dir;
                    mEventRelated.getSingleShortData(&dir, 'dir ', 0);
                    if (dir == 0) {
                        pos.set(v1);
                        rot = mRotation.y - 0x8000;
                    } else {
                        pos.set(v2);
                        rot = mRotation.y;
                    }
                }
                stepTowards(pos);
                dAcPy_c *pPlayer = dAcPy_c::GetLinkM();
                pPlayer->setPosYRot(pos, rot);
                mEventRelated.advanceNext();
            }
        } break;
        case 'talk':
        case 'lock': {
            if (mEventRelated.isAdvance()) {
                // Gets the message to show when the door is locked and is failed to be opened
                s32 flow = mLock.get() == nullptr ? mParams >> 16 : 4001;
                s32 part2 = flow < 10000U ? 1000 : 100;
                s32 truncate = (flow / part2);

                u16 lower = flow - truncate * part2;

                s32 part1 = flow < 10000U ? 1000 : 100;
                u16 upper = flow / part1;
                mFlowMgr.triggerEntryPoint(upper, lower, 0, 0);
            }
            if (mFlowMgr.checkFinished()) {
                mEventRelated.advanceNext();
            }
        } break;
        case 'unlk': {
            dAcOLock_c *pLock = mLock.get();
            if (pLock == nullptr) {
                mEventRelated.advanceNext();
            } else {
                if (mEventRelated.isAdvance()) {
                    pLock->setField_0x2090();
                    setDoorKnobPosition();
                }
                if (pLock->checkField_0x2091()) {
                    mEventRelated.advanceNext();
                }
            }
        } break;
        default: {
            mEventRelated.advanceNext();
        }
        case '????': {
        } break;
    }

    dAcObjBase_c *pObj = mObjRef.get();
    if (pObj != nullptr) {
        mMtx_c m = pObj->mWorldMtx;
        m += mMtx;
        mWorldMtx = m;
        mAnmChr.getModel().setLocalMtx(mWorldMtx);
    }

    mAnmChr.getModel().calc(false);

    if (type != DOOR_TEMPLE_MAIN) {
        mMtx_c nodeMtx;
        mAnmChr.getModel().getNodeWorldMtx(1, nodeMtx);
        nodeMtx.transM(sDoorknobOffset0);
        mMdl0.setLocalMtx(nodeMtx);
        if (isTimeEffected()) {
            mMdl1.setLocalMtx(nodeMtx);
        }
    }

    if (pObj != nullptr) {
        mVec3_c min, max;
        min.set(999999.f, 999999.f, 999999.f);
        max.set(-999999.f, -999999.f, -999999.f);

        if (type == DOOR_TEMPLE_MAIN) {
            mAnmChr.getModel().getBounds(&min, &max);
        } else {
            mVec3_c min_l, max_l;
            mMtx_c local;
            mMdl0.getBounds(&min_l, &max_l);
            mMdl0.getLocalMtx(local);

            mMtx_c world = mWorldMtx;
            world.inverse();

            mVec3_c minPos = min_l;
            local.multVec(minPos, minPos);
            world.multVec(minPos, minPos);

            // clang-format off
            min.set(
                nw4r::ut::Min(minPos.x, min.x),
                nw4r::ut::Min(minPos.y, min.y),
                nw4r::ut::Min(minPos.z, min.z)
            );
            max.set(
                nw4r::ut::Max(minPos.x, max.x),
                nw4r::ut::Max(minPos.y, max.y),
                nw4r::ut::Max(minPos.z, max.z)
            );
            // clang-format on

            mVec3_c maxPos = max_l;
            local.multVec(maxPos, maxPos);
            world.multVec(maxPos, maxPos);
            // clang-format off
            min.set(
                nw4r::ut::Min(maxPos.x, min.x),
                nw4r::ut::Min(maxPos.y, min.y),
                nw4r::ut::Min(maxPos.z, min.z)
            );
            max.set(
                nw4r::ut::Max(maxPos.x, max.x),
                nw4r::ut::Max(maxPos.y, max.y),
                nw4r::ut::Max(maxPos.z, max.z)
            );
            // clang-format on

            mVec3_c vecs[6] = {
                mVec3_c(min_l.x, min_l.y, max_l.z), mVec3_c(min_l.x, max_l.y, min_l.z),
                mVec3_c(min_l.x, max_l.y, max_l.z), mVec3_c(max_l.x, min_l.y, min_l.z),
                mVec3_c(max_l.x, min_l.y, max_l.z), mVec3_c(max_l.x, max_l.y, min_l.z),
            };

            for (int i = 0, j = 0; i < (int)ARRAY_LENGTH(vecs); j++, i++) {
                local.multVec(vecs[i], vecs[j]);
                world.multVec(vecs[i], vecs[j]);
                // clang-format off
                min.set(
                    nw4r::ut::Min(vecs[j].x, min.x),
                    nw4r::ut::Min(vecs[j].y, min.y),
                    nw4r::ut::Min(vecs[j].z, min.z)
                );
                max.set(
                    nw4r::ut::Max(vecs[j].x, max.x),
                    nw4r::ut::Max(vecs[j].y, max.y),
                    nw4r::ut::Max(vecs[j].z, max.z)
                );
                // clang-format on
            }
        }
        mBoundingBox.Set(min, max);
    }

    if (mEmmiterL.hasEmitters()) {
        int node = mAnmChr.getModel().getNodeID("DoorF_L");
        if (node >= 0) {
            mMtx_c nodeMtx;
            mAnmChr.getModel().getNodeWorldMtx(node, nodeMtx);
            mEmmiterL.setTransform(nodeMtx);
        }
    }
    if (mEmmiterR.hasEmitters()) {
        int node = mAnmChr.getModel().getNodeID("DoorF_R");
        if (node >= 0) {
            mMtx_c nodeMtx;
            mAnmChr.getModel().getNodeWorldMtx(node, nodeMtx);
            mEmmiterR.setTransform(nodeMtx);
        }
    }

    switch (mEventRelated.getCurrentEventCommand()) {
        case 'pshB': {
            if (mEventRelated.isAdvance() && type == DOOR_TEMPLE_MAIN) {
                int node;
                node = mAnmChr.getModel().getNodeID("DoorF_L");
                if (node >= 0) {
                    mMtx_c nodeMtx;
                    mAnmChr.getModel().getNodeWorldMtx(node, nodeMtx);
                    mEmmiterL.startEffect(PARTICLE_RESOURCE_ID_MAPPING_935_, nodeMtx, nullptr, nullptr);
                }
                node = mAnmChr.getModel().getNodeID("DoorF_R");
                if (node >= 0) {
                    mMtx_c nodeMtx;
                    mAnmChr.getModel().getNodeWorldMtx(node, nodeMtx);
                    mEmmiterR.startEffect(PARTICLE_RESOURCE_ID_MAPPING_936_, nodeMtx, nullptr, nullptr);
                }
            }
        } break;
        case 'pshE': {
            if (mEventRelated.isAdvance() && type == DOOR_TEMPLE_MAIN) {
                int node;
                node = mAnmChr.getModel().getNodeID("DoorF_L");
                if (node >= 0) {
                    mMtx_c nodeMtx;
                    mAnmChr.getModel().getNodeWorldMtx(node, nodeMtx);
                    mEmmiterL.startEffect(PARTICLE_RESOURCE_ID_MAPPING_933_, nodeMtx, nullptr, nullptr);
                }
                node = mAnmChr.getModel().getNodeID("DoorF_R");
                if (node >= 0) {
                    mMtx_c nodeMtx;
                    mAnmChr.getModel().getNodeWorldMtx(node, nodeMtx);
                    mEmmiterR.startEffect(PARTICLE_RESOURCE_ID_MAPPING_934_, nodeMtx, nullptr, nullptr);
                }
            }
        } break;
    }

    if (type != DOOR_TEMPLE_MAIN) {
        bool b;
        if (frame0 == frame1) {
            b = frame0 == 16.f;
        } else {
            b = frame0 < 16.f && 16.f <= frame1;
        }
        if (b) {
            startSound(SE_Door_W_OPEN);
        }
    }

    if (type != DOOR_TEMPLE_MAIN) {
        bool b;
        if (frame0 == frame1) {
            b = frame0 == 60.f;
        } else {
            b = frame0 < 60.f && 60.f <= frame1;
        }
        if (b) {
            startSound(SE_Door_W_CLOSE);
        }
    }

    if (mEventRelated.getCurrentEventCommand() == 'pshB' && type == DOOR_TEMPLE_MAIN) {
        if (frame0 < 50.f && 50.f <= frame1) {
            // TODO(Zeldex) - Why is this a check - This seems to indicate it was a getter for a pointer
            if (&dRumble_c::sRumblePreset3) {
                mRumbleIdx =
                    dRumble_c::start(dRumble_c::sRumblePreset3, dRumble_c::FLAG_SLOT0 | dRumble_c::FLAG_INITIALIZE);
            }
        }
        if (frame0 <= 115.f && 115.f < frame1) {
            dRumble_c::stop(mRumbleIdx);
        }
    }
    return SUCCEEDED;
}

int dAcOdoor_c::actorExecute() {
    if (isTimeEffected()) {
        fn_80067290(&mTimeBits, mRoomID, mPosition, 200.f);
    }
    switch (mEventCallbackType) {
        case 0: {
            bool b;
            checkRoom(mRoomID, b);
            if (b) {
                setObjectProperty(OBJ_PROP_0x200);
                return SUCCEEDED;
            }
        } break;
        case 1: {
            bool b;
            checkRooms(b);
            if (b) {
                setObjectProperty(OBJ_PROP_0x200);
                return SUCCEEDED;
            }
        } break;
    }
    unsetObjectProperty(OBJ_PROP_0x200);
    s32 type = getType();

    if (getLockParameter() == LOCK_KEY) {
        if (mLock.get() == nullptr) {
            if (isPlayerInteractable()) {
                AttentionManager::GetInstance()->addTarget(*this, sOpenInteraction, 0, nullptr);
            }
        } else {
            if (isPlayerInteractableLocked()) {
                AttentionManager::GetInstance()->addTarget(*this, sOpenInteraction, 0, nullptr);
            }
        }
    } else {
        switch (mLockBehavior) {
            case 0: {
                if (isPlayerInteractable()) {
                    AttentionManager::GetInstance()->addTarget(*this, sOpenInteraction, 0, nullptr);
                }
            } break;
            case 1:
            case 4:
            case 5:
            case 6: {
                if (isPlayerInteractable()) {
                    if (isLocked()) {
                        AttentionManager::GetInstance()->addTarget(*this, sLockedInteraction, 0, nullptr);
                    } else {
                        AttentionManager::GetInstance()->addTarget(*this, sOpenInteraction, 0, nullptr);
                    }
                }
            } break;
            case 2: {
                if (isPlayerInteractable()) {
                    AttentionManager::GetInstance()->addTarget(*this, sOpenInteraction, 0, nullptr);
                }
            } break;
            case 3: {
                if (!isLocked() && isPlayerInteractable()) {
                    AttentionManager::GetInstance()->addTarget(*this, sOpenInteraction, 0, nullptr);
                }
            } break;
        }
    }

    dAcObjBase_c *pObj = mObjRef.get();
    if (pObj != nullptr) {
        mMtx_c m = pObj->mWorldMtx;
        m += mMtx;
        mWorldMtx = m;
        mAnmChr.getModel().setLocalMtx(mWorldMtx);
        mAnmChr.getModel().calc(false);

        if (type != DOOR_TEMPLE_MAIN) {
            mMtx_c nodeMtx;
            mAnmChr.getModel().getNodeWorldMtx(1, nodeMtx);
            nodeMtx.transM(sDoorknobOffset0);
            mMdl0.setLocalMtx(nodeMtx);
            if (isTimeEffected()) {
                mMdl1.setLocalMtx(nodeMtx);
            }
        }
    }

    if (pObj != nullptr) {
        mVec3_c min, max;
        min.set(999999.f, 999999.f, 999999.f);
        max.set(-999999.f, -999999.f, -999999.f);

        if (type == DOOR_TEMPLE_MAIN) {
            mAnmChr.getModel().getBounds(&min, &max);
        } else {
            mVec3_c min_l, max_l;
            mMtx_c local;
            mMdl0.getBounds(&min_l, &max_l);
            mMdl0.getLocalMtx(local);

            mMtx_c world = mWorldMtx;
            world.inverse();

            mVec3_c minPos = min_l;
            local.multVec(minPos, minPos);
            world.multVec(minPos, minPos);

            // clang-format off
            min.set(
                nw4r::ut::Min(minPos.x, min.x),
                nw4r::ut::Min(minPos.y, min.y),
                nw4r::ut::Min(minPos.z, min.z)
            );
            max.set(
                nw4r::ut::Max(minPos.x, max.x),
                nw4r::ut::Max(minPos.y, max.y),
                nw4r::ut::Max(minPos.z, max.z)
            );
            // clang-format on

            mVec3_c maxPos = max_l;
            local.multVec(maxPos, maxPos);
            world.multVec(maxPos, maxPos);
            // clang-format off
            min.set(
                nw4r::ut::Min(maxPos.x, min.x),
                nw4r::ut::Min(maxPos.y, min.y),
                nw4r::ut::Min(maxPos.z, min.z)
            );
            max.set(
                nw4r::ut::Max(maxPos.x, max.x),
                nw4r::ut::Max(maxPos.y, max.y),
                nw4r::ut::Max(maxPos.z, max.z)
            );
            // clang-format on

            mVec3_c vecs[6] = {
                mVec3_c(min_l.x, min_l.y, max_l.z), mVec3_c(min_l.x, max_l.y, min_l.z),
                mVec3_c(min_l.x, max_l.y, max_l.z), mVec3_c(max_l.x, min_l.y, min_l.z),
                mVec3_c(max_l.x, min_l.y, max_l.z), mVec3_c(max_l.x, max_l.y, min_l.z),
            };

            for (int i = 0, j = 0; i < (int)ARRAY_LENGTH(vecs); j++, i++) {
                local.multVec(vecs[i], vecs[j]);
                world.multVec(vecs[i], vecs[j]);
                // clang-format off
                min.set(
                    nw4r::ut::Min(vecs[j].x, min.x),
                    nw4r::ut::Min(vecs[j].y, min.y),
                    nw4r::ut::Min(vecs[j].z, min.z)
                );
                max.set(
                    nw4r::ut::Max(vecs[j].x, max.x),
                    nw4r::ut::Max(vecs[j].y, max.y),
                    nw4r::ut::Max(vecs[j].z, max.z)
                );
                // clang-format on
            }
        }
        mBoundingBox.Set(min, max);
    }
    return SUCCEEDED;
}

int dAcOdoor_c::draw() {
    if (isTimeEffected()) {
        if (!mTimeBits.field_0x00) {
            drawModelType1(&mMdl1);
        }
        if (!mTimeBits.field_0x01) {
            drawModelType1(&mMdl0);
        }
    } else {
        if (getType() == DOOR_TEMPLE_MAIN) {
            drawModelType1(&mAnmChr.getModel());
        } else {
            drawModelType1(&mMdl0);
        }
    }
    return SUCCEEDED;
}

bool dAcOdoor_c::isLocked() {
    if (getLockParameter() == LOCK_KEY) {
        return mLock.isLinked();
    }

    switch (mLockBehavior) {
        case LOCK_NEVER: {
            return false;
        }
        case LOCK_NOT_SET:
        case LOCK_TIMESHIFT: {
            return mSceneflag < 0xFF ? !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag) : true;
        }
        case LOCK_NEVER1: {
            return false;
        }
        case LOCK_SET: {
            return mSceneflag < 0xFF ? SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag) : true;
        }
        case LOCK_DAY: {
            return dScGame_c::currentSpawnInfo.getTimeOfDay() == SpawnInfo::DAY;
        }
        case LOCK_NIGHT: {
            return dScGame_c::currentSpawnInfo.getTimeOfDay() == SpawnInfo::NIGHT;
        }
        default: {
            return true;
        }
    }
}

bool dAcOdoor_c::canOpen() {
    // NONMATCHING
    // https://decomp.me/scratch/dHRfo
    // Duplicate load of  dStageMgr_c::sInstance for nullptr check and function call
    // Happens in d_a_obj_door_boss as well
    dStageMgr_c *pStageMgr = dStageMgr_c::GetInstance();
    if (pStageMgr != nullptr && pStageMgr->isAreaTypeDungeonOrBoss()) {
        return getLockParameter() == LOCK_KEY && mLock.isLinked();
    }
    return isLocked();
}

f32 dAcOdoor_c::fn_572_33E0() {
    return -80.f;
}

// HACK
inline u32 createBits(const u32 &a, const u32 &b) {
    return a | b;
}

void dAcOdoor_c::startPullEventWithoutCallback(void *zevData) {
    Event event(DoorPull, zevData, 100, createBits(0x100001, 0x2), nullptr, nullptr);
    mEventRelated.scheduleEvent(event, 0);
}

void dAcOdoor_c::startPullEventWithCallback(void *zevData) {
    Event event(DoorPull, zevData, 100, createBits(0x100001, 0x2), (void *)doorPullEventCallback, nullptr);
    mEventRelated.scheduleEvent(event, 0);
}

void dAcOdoor_c::startPushEventWithoutCallback(void *zevData) {
    Event event(DoorPush, zevData, 100, createBits(0x100001, 0x2), nullptr, nullptr);
    mEventRelated.scheduleEvent(event, 0);
}

void dAcOdoor_c::startPushEventWithCallback(void *zevData) {
    Event event(DoorPush, zevData, 100, createBits(0x100001, 0x2), (void *)doorPushEventCallback, nullptr);
    mEventRelated.scheduleEvent(event, 0);
}

void dAcOdoor_c::startPullEvent(void *zevData) {
    switch (mEventCallbackType) {
        case 0: startPullEventWithoutCallback(zevData); break;
        case 1: startPullEventWithCallback(zevData); break;
    }
}

void dAcOdoor_c::startPushEvent(void *zevData) {
    switch (mEventCallbackType) {
        case 0: startPushEventWithoutCallback(zevData); break;
        case 1: startPushEventWithCallback(zevData); break;
    }
}

void dAcOdoor_c::startOpenEvent(void *zevData) {
    if (checkPullDoor()) {
        startPullEvent(zevData);
    } else {
        // Cawlin and Strich's room has an awkward angle when pushing.
        if (mbKobunDoor) {
            Event event("KobunDoorPush", zevData, 100, 0x100001, nullptr, nullptr);
            mEventRelated.scheduleEvent(event, 0);
        } else {
            startPushEvent(zevData);
        }
    }
}

void dAcOdoor_c::doorPullEventCallback(void *pDoor) {
    openCallbackCommon(pDoor);
}

void dAcOdoor_c::doorPushEventCallback(void *pDoor) {
    openCallbackCommon(pDoor);
}

void dAcOdoor_c::startDoorPullLockedEvent(void *zevData) {
    Event event(DoorPullLock, zevData, 100, 0x100001, (void *)pullLockedEventCallback, nullptr);
    mEventRelated.scheduleEvent(event, 0);
}

void dAcOdoor_c::startDoorPushLockedEvent(void *zevData) {
    Event event(DoorPushLock, zevData, 100, 0x100001, (void *)pushLockedEventCallback, nullptr);
    mEventRelated.scheduleEvent(event, 0);
}

void dAcOdoor_c::startLockedEvent(void *zevData) {
    if (checkPullDoor()) {
        startDoorPullLockedEvent(zevData);
    } else {
        startDoorPushLockedEvent(zevData);
    }
}

void dAcOdoor_c::pullLockedEventCallback(void *vDoor) {
    dAcOdoor_c *pDoor = static_cast<dAcOdoor_c *>(vDoor);
    if (pDoor->hasFlowEntryPoint()) {
        pDoor->playInteractionLocked();
    }
    u16 flag = pDoor->getFailedToOpenFlag();
    if (flag < 0xFF) {
        SceneflagManager::sInstance->setFlag(pDoor->mRoomID, flag);
    }
}

void dAcOdoor_c::pushLockedEventCallback(void *vDoor) {
    dAcOdoor_c *pDoor = static_cast<dAcOdoor_c *>(vDoor);
    if (pDoor->hasFlowEntryPoint()) {
        pDoor->playInteractionLocked();
    }
    u16 flag = pDoor->getFailedToOpenFlag();
    if (flag < 0xFF) {
        SceneflagManager::sInstance->setFlag(pDoor->mRoomID, flag);
    }
}

void dAcOdoor_c::startUnlockEvent() {
    Event event("ShutterUnlock", 100, 0x100001, (void *)unlockEventCallback, nullptr);
    mEventRelated.scheduleEvent(event, 0);
}

extern "C" void increaseSmallKeyCounter(int amt);

void dAcOdoor_c::unlockEventCallback(void *vDoor) {
    dAcOdoor_c *pDoor = static_cast<dAcOdoor_c *>(vDoor);
    void *zevData = getOarcZev("DoorAnim");
    if (pDoor->checkPullDoor()) {
        switch (pDoor->mEventCallbackType) {
            case 0: {
                Event event(DoorPull, zevData, 100, createBits(0x100001, 0x2), nullptr, nullptr);
                EventManager::setEvent(pDoor, &event, nullptr);
            } break;
            case 1: {
                Event event(DoorPull, zevData, 100, createBits(0x100001, 0x2), (void *)doorPullEventCallback, nullptr);
                EventManager::setEvent(pDoor, &event, nullptr);
            } break;
        }

    } else {
        switch (pDoor->mEventCallbackType) {
            case 0: {
                Event event(DoorPull, zevData, 100, createBits(0x100001, 0x2), nullptr, nullptr);
                EventManager::setEvent(pDoor, &event, nullptr);
            } break;
            case 1: {
                Event event(DoorPull, zevData, 100, createBits(0x100001, 0x2), (void *)doorPullEventCallback, nullptr);
                EventManager::setEvent(pDoor, &event, nullptr);
            } break;
        }
    }

    if (pDoor->mSceneflag < 0xFF) {
        SceneflagManager::sInstance->setFlag(pDoor->mRoomID, pDoor->mSceneflag);
    }

    increaseSmallKeyCounter(-1);
}

void dAcOdoor_c::openCallbackCommon(void *vDoor) {
    if (vDoor == nullptr) {
        return;
    }
    dAcOdoor_c *pDoor = static_cast<dAcOdoor_c *>(vDoor);

    bool search = true;
    dAcOFence_c *pFence = static_cast<dAcOFence_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_FENCE, nullptr));
    while (search && pFence != nullptr) {
        if (mAng::abs(dAcPy_c::GetLink()->mRotation.y - pFence->mRotation.y) < 0x4000) {
            const mVec3_c &posFence = pFence->mPosition;
            const mVec3_c &position = pDoor->mPosition;
            if (posFence.squareDistanceToXZ(position) < 22500.f && nw4r::math::FAbs(posFence.y - position.y) < 500.f) {
                pFence->setObjectProperty(OBJ_PROP_0x200);
                pFence->fn_550_1250();
                pFence->fn_550_12B0();
                search = false;
                continue;
            }
        }
        pFence = static_cast<dAcOFence_c *>(fManager_c::searchBaseByProfName(fProfile::OBJ_FENCE, pFence));
    }
}

void dAcOdoor_c::registerInEvent() {
    if (isInTimeDoorEvent()) {
        setTimeDoorEventActive();
    }
}

void dAcOdoor_c::unkVirtFunc_0x6C() {
    setTimeDoorEventInactive();
}

void dAcOdoor_c::doInteraction(s32 interaction) {
    if (getType() == DOOR_TEMPLE_MAIN) {
        void *zevData = getOarcZev("DoorF");
        Event event("FTypeDoorOut", zevData, 100, createBits(0x100001, 0x2), nullptr, nullptr);
        mEventRelated.scheduleEvent(event, 0);
        return;
    }

    void *zevData = getOarcZev("DoorAnim");

    switch (interaction) {
        case OPEN: {
            if (getLockParameter() == LOCK_KEY) {
                if (!mLock.isLinked()) {
                    startOpenEvent(zevData);
                } else {
                    if (dAcItem_c::getSmallKeyCount() != 0) {
                        if (isConnectedToOtherDoor()) {
                            if (isPlayerInteractable()) {
                                startUnlockEvent();
                            } else {
                                dAcOdoor_c *pDoor = getConnectedDoorRef().get();
                                pDoor->mLock.link(mLock.get());
                                pDoor->startUnlockEvent();
                            }
                        } else {
                            startOpenEvent(zevData);
                        }
                    } else {
                        Event event(getDoorInteractionName(), 100, 0x100001, nullptr, nullptr);
                        mEventRelated.scheduleEvent(event, 0);
                    }
                }
            } else {
                startOpenEvent(zevData);
                if (mLockBehavior == LOCK_NEVER1 && mSceneflag < 0xFF) {
                    SceneflagManager::sInstance->setFlag(mRoomID, mSceneflag);
                }
            }
        } break;
        case EXAMINE_TALK: {
            startLockedEvent(zevData);
        } break;
    }
}

s32 dAcOdoor_c::getLockParameter() {
    return getFromParams(6, 0x1);
}

s32 dAcOdoor_c::getLockBehavior() {
    return (mRotation.x >> 0) & 0xFF;
}

u8 dAcOdoor_c::getSceneflag() {
    return (mRotation.x >> 8) & 0xFF;
}

u8 dAcOdoor_c::getDoorCallbackType() {
    return (mRotation.z >> 0) & 0xF;
}

u8 dAcOdoor_c::getFrontRoomParam() {
    return (mRotation.z >> 4) & 0x3F;
}

u8 dAcOdoor_c::getBackRoomParam() {
    return (mRotation.z >> 10) & 0x3F;
}

u16 dAcOdoor_c::getFailedToOpenFlag() {
    return getParams2Lower() & 0xFF;
}

f32 dAcOdoor_c::getInteractionMinX() {
    return -80.f;
}

f32 dAcOdoor_c::getInteractionMaxX() {
    return 80.f;
}

f32 dAcOdoor_c::getInteractionMinZ() {
    return -100.f;
}

f32 dAcOdoor_c::getInteractionMaxZ() {
    return 100.f;
}

f32 dAcOdoor_c::getInteractionLockMinX() {
    return -160.f;
}

f32 dAcOdoor_c::getInteractionLockMaxX() {
    return 160.f;
}

f32 dAcOdoor_c::getInteractionLockMinZ() {
    return -100.f;
}

f32 dAcOdoor_c::getInteractionLockMaxZ() {
    return 100.f;
}

void dAcOdoor_c::setRoomId(s8 roomId) {
    mRoomID = roomId;
}

void dAcOdoor_c::setFlag(u32 flags) {
    mFlags = flags;
}

void dAcOdoor_c::setEventCallbackType(u8 in) {
    mEventCallbackType = in;
}

void dAcOdoor_c::setFrontRoom(s8 in) {
    mFrontRoomId = in;
}

void dAcOdoor_c::setBackRoom(s8 in) {
    mBackRoomId = in;
}

void dAcOdoor_c::setDoubleDoor() {
    mbDoubleDoor = true;
}

void dAcOdoor_c::setNotDoubleDoor() {
    mbDoubleDoor = false;
}

void dAcOdoor_c::setSingleDoor() {
    mbSingleDoor = true;
}

void dAcOdoor_c::setNotSingleDoor() {
    mbSingleDoor = false;
}

void dAcOdoor_c::setInSandshipBoss() {
    mbInSandshipBoss = true;
}

bool dAcOdoor_c::isTimeDoorEventActive() const {
    return mFlags & 1;
}

void dAcOdoor_c::setTimeDoorEventActive() {
    setFlag(mFlags | 1);
}

void dAcOdoor_c::setTimeDoorEventInactive() {
    setFlag(mFlags & ~1);
}

void dAcOdoor_c::setTimeEffected() {
    setFlag(mFlags | 2);
}

void dAcOdoor_c::setTimeStatic() {
    setFlag(mFlags & ~2);
}

bool dAcOdoor_c::checkFrontRoom(bool &b) const {
    return checkRoom(mFrontRoomId, b);
}

bool dAcOdoor_c::checkBackRoom(bool &b) const {
    return checkRoom(mBackRoomId, b);
}

bool dAcOdoor_c::checkRooms(bool &b) const {
    bool b0, b1;
    if (!checkFrontRoom(b0)) {
        return false;
    }
    if (!checkBackRoom(b1)) {
        return false;
    }
    b = b0 && b1;
    return true;
}

void dAcOdoor_c::triggerExit() {
    if (isLeavingSealedTempleSideDoorPostSkyKeep()) {
        dScGame_c::GetInstance()->triggerExit(mRoomID, 11, 2, 2);
    } else {
        dScGame_c::GetInstance()->triggerExit(mRoomID, getFromParams(8, 0xFF), 2, 2);
    }
}

bool dAcOdoor_c::isLeavingSealedTempleSideDoorPostSkyKeep() const {
    return s32(mParams & 0x3F) == DOOR_TEMPLE_SIDE && mBackRoomId == 1 &&
           StoryflagManager::sInstance->getFlag(STORYFLAG_342) && dScGame_c::isCurrentStage(sStageF402);
}

void dAcOdoor_c::transitionPushRoomFlags() const {
    if (dStage_c::GetInstance() == nullptr) {
        return;
    }
    dStage_c::GetInstance()->updateRoomFlags(getField_0x5B4(), getField_0x5B5());
}

void dAcOdoor_c::transitionPullRoomFlags() const {
    if (dStage_c::GetInstance() == nullptr) {
        return;
    }
    dStage_c::GetInstance()->updateRoomFlags(getField_0x5B5(), getField_0x5B4());
}

bool dAcOdoor_c::isPositionInFrontOfDoor(const mVec3_c &point) const {
    s32 diff = mRotation.y - (point - mPosition).atan2sX_Z();
    return mAng::abs(diff) <= 0x4000;
}

bool dAcOdoor_c::checkPullDoor() const {
    return isPositionInFrontOfDoor(dAcPy_c::GetLink()->mPosition);
}

bool dAcOdoor_c::hasFlowEntryPoint() {
    return getFromParams(16, 0xFFFF);
}

bool dAcOdoor_c::isTimeEffected() const {
    return mFlags & 2;
}

void dAcOdoor_c::setDoorKnobPosition() {
    mVec3_c pos;
    if (getType() == DOOR_TEMPLE_MAIN) {
        pos.set(mPosition);
        pos.y += 150.f;
    } else {
        mMtx_c m;
        mAnmChr.getModel().getNodeWorldMtx(1, m);
        m.transM(sDoorknobOffset0);
        m.multVec(sDoorknobOffset1, pos);
    }
    mPositionCopy2.set(pos);
    mPositionCopy3.set(pos);
}

bool dAcOdoor_c::isPlayerInteractable() const {
    mVec3_c v = dAcPy_c::GetLink()->mPosition;
    v -= mPosition;
    v.rotY(-mRotation.y);

    if (v.x < getInteractionMinX()) {
        return false;
    }
    if (getInteractionMaxX() < v.x) {
        return false;
    }
    if (v.z < getInteractionMinZ()) {
        return false;
    }
    if (getInteractionMaxZ() < v.z) {
        return false;
    }
    return true;
}

bool dAcOdoor_c::isPlayerInteractableLocked() const {
    mVec3_c v = dAcPy_c::GetLink()->mPosition;
    v -= mPosition;
    v.rotY(-mRotation.y);
    v.x += 80.f;

    if (v.x < getInteractionLockMinX()) {
        return false;
    }
    if (getInteractionLockMaxX() < v.x) {
        return false;
    }
    if (v.z < getInteractionLockMinZ()) {
        return false;
    }
    if (getInteractionLockMaxZ() < v.z) {
        return false;
    }
    return true;
}

void dAcOdoor_c::playInteractionLocked() {
    Event event(getDoorInteractionName(), 100, 0x100001, nullptr, nullptr);
    EventManager::setEvent(this, &event, nullptr);
}
