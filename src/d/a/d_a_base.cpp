#include "d/a/d_a_base.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/d_heap.h"
#include "d/d_room.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/flag/enemyflag_manager.h"
#include "egg/core/eggAllocator.h"
#include "egg/math/eggMath.h"
#include "f/f_base.h"
#include "f/f_list_nd.h"
#include "m/m_angle.h"
#include "m/m_heap.h"
#include "m/m_vec.h"
#include "toBeSorted/actor_info.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/event.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/special_item_drop_mgr.h"

u32 dAcBase_c::s_Create_RoomId = -1;
u32 dAcBase_c::s_Create_Params2 = -1;
u16 dAcBase_c::s_Create_UnkFlags = -1;
u8 dAcBase_c::s_Create_ViewClipIdx = -1;

mVec3_c *dAcBase_c::s_Create_Position;
mAng3_c *dAcBase_c::s_Create_Rotation;
mVec3_c *dAcBase_c::s_Create_Scale;
dAcBase_c *dAcBase_c::s_Create_Parent;
const ActorInfo *dAcBase_c::s_Create_ActorInfo;
u8 dAcBase_c::s_Create_Subtype;

bool dAcBase_c::createHeap() {
    return true;
}

// mpSoundSource and mSoundList need to be ironed out before this can match
// NOT MATCHING
dAcBase_c::dAcBase_c()
    : mAllocator(),
      mpActorInfo(s_Create_ActorInfo),
      mSoundList(),
      mpPosition(&mPosition),
      mParams2(s_Create_Params2),
      mObjID(s_Create_UnkFlags),
      mViewClipIdx(s_Create_ViewClipIdx),
      mActorNode(nullptr),
      mRoomID(s_Create_RoomId),
      mActorSubtype(s_Create_Subtype) {
    mJStudioActor = 0;
    someStr[0] = 0;

    if (s_Create_Position) {
        setPosition(*s_Create_Position);
    }

    if (s_Create_Rotation) {
        setRotation(*s_Create_Rotation);
    }

    if (s_Create_Scale) {
        setScale(*s_Create_Scale);
    } else {
        mScale.set(1.0f, 1.0f, 1.0f);
    }

    if (s_Create_Parent != nullptr) {
        setActorRef(s_Create_Parent);
    }

    fProfile::fActorProfile_c *profile = (fProfile::fActorProfile_c *)((*fProfile::sProfileList)[mProfileName]);
    mActorProperties = profile->mActorProperties;
    if (mpActorInfo == nullptr) {
        mpActorInfo = getActorInfoByProfileAndSubtype(mProfileName, mActorSubtype);
    }
    someStr[0] = '\0';
}

dAcBase_c::~dAcBase_c() {}

void dAcBase_c::setTempCreateParams(
    mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, s32 roomId, u32 params2, dAcBase_c *parent, u8 subtype, u16 unkFlag,
    s8 viewClipIdx, const ActorInfo *actorInfo
) {
    s_Create_Position = pos;
    s_Create_Rotation = rot;
    s_Create_Scale = scale;
    s_Create_RoomId = roomId;
    s_Create_Params2 = params2;
    s_Create_Parent = parent;
    s_Create_Subtype = subtype;
    s_Create_UnkFlags = unkFlag;
    s_Create_ViewClipIdx = viewClipIdx;
    s_Create_ActorInfo = actorInfo;
}

dSoundSourceIf_c *dAcBase_c::createSoundSource() {
    if (mpActorInfo == nullptr) {
        return nullptr;
    }

    s32 soundSourceType = mpActorInfo->soundSourceType;
    if (soundSourceType == -1) {
        return nullptr;
    }

    const char *actorName = getActorName(mpActorInfo);
    return dSoundSourceIf_c::create(soundSourceType, this, actorName, mSubtype);
}

int dAcBase_c::initAllocatorWork1Heap(int size, char *name, int align) {
    return initAllocator(size, name, dHeap::work1Heap.heap, align);
}

int dAcBase_c::initAllocator(int size, char *name, EGG::Heap *heap, int align) {
    if (!mAllocator.createFrmHeapToCurrent(size, heap, name, 0x20, mHeap::OPT_NONE)) {
        return 0;
    }
    mpSoundSource = createSoundSource();
    int success = createHeap();
    mAllocator.adjustFrmHeapRestoreCurrent();
    return success;
}

bool dAcBase_c::addActorToRoom(s32 roomId) {
    dBase_c *room = dStage_c::getParentForRoom(roomId);
    if (room == nullptr) {
        return false;
    }
    if (setConnectChild(room)) {
        if (roomId == -1) {
            this->mRoomID = dStage_c::GetInstance()->getCurrRoomId();
        } else {
            this->mRoomID = roomId;
        }
        return true;
    }
    return false;
}

void dAcBase_c::setTgSndAreaFlag(s32 shift) {
    mTgSndAreaFlags |= (1 << shift);
}

int dAcBase_c::actorCreate() {
    return SUCCEEDED;
}

int dAcBase_c::actorPostCreate() {
    return SUCCEEDED;
}

int dAcBase_c::create() {
    if (checkActorProperty(AC_PROP_0x8000000)) {
        return actorPostCreate();
    }
    int success = actorCreate();
    if (success == SUCCEEDED) {
        success = NOT_READY;
        setActorProperty(AC_PROP_0x8000000);
    }
    return success;
}

void dAcBase_c::postCreate(fBase_c::MAIN_STATE_e state) {
    if (state == SUCCESS) {
        mPositionCopy = mPosition;
        mRotationCopy = mRotation;
        mRoomIDCopy = mRoomID;
    }
    dBase_c::postCreate(state);
}

// NOT MATCHING
int dAcBase_c::preDelete() {
    int ret = SUCCEEDED;
    if (fBase_c::preDelete() == NOT_READY) {
        ret = NOT_READY;
    }

    if (!checkActorProperty(AC_PROP_0x800) && checkActorProperty(AC_PROP_0x10000000) &&
        fBase_c::getConnectParent()->mLifecycleState != TO_BE_DELETED) {
        if (itemDroppingAndGivingRelated(nullptr, 0) != 0) {
            setEnemyDefeatFlag();
        }

    } else {
        if (mpSoundSource != nullptr) {
            mpSoundSource->stopAllSound(0);
        }
        // TODO - TList
        for (SoundInfoList::Iterator it = mSoundList.GetBeginIter(); it != mSoundList.GetEndIter(); ++it) {
            it->getSource()->stopAllSound(0);
        }
    }

    if (ret == NOT_READY) {
        return NOT_READY;
    }

    if (mpSoundSource != nullptr) {
        mpSoundSource->shutdown();
        if (mpSoundSource->hasPlayingSounds()) {
            return NOT_READY;
        }
    }
    // TODO - TList
    for (SoundInfoList::Iterator it = mSoundList.GetBeginIter(); it != mSoundList.GetEndIter(); ++it) {
        it->getSource()->shutdown();
        if (it->getSource()->hasPlayingSounds()) {
            return NOT_READY;
        }
    }

    if (checkActorProperty(AC_PROP_0x2000000)) {
        changeLoadedEntitiesNoSet();
    }

    return SUCCEEDED;
}

int dAcBase_c::preExecute() {
    if (dBase_c::preExecute() == NOT_READY) {
        return NOT_READY;
    }
    if (checkActorProperty(AC_PROP_0x10000000)) {
        if (checkActorProperty(AC_PROP_0x40000000)) {
            return NOT_READY;
        }

        if (EventManager::isInEvent() && mJStudioActor == nullptr && !EventManager::isInEvent0Or7() &&
            !EventManager::FUN_800a0ba0() && !EventManager::FUN_800a0570(this) && !checkActorProperty(AC_PROP_0x4)) {
            return NOT_READY;
        }
    }
    return SUCCEEDED;
}

int dAcBase_c::execute() {
    if (EventManager::isInEvent() && !EventManager::isInEvent0Or7()) {
        return actorExecuteInEvent();
    }

    return actorExecute();
}

int dAcBase_c::actorExecute() {
    return SUCCEEDED;
}

int dAcBase_c::actorExecuteInEvent() {
    return actorExecute();
}

void dAcBase_c::postExecute(fBase_c::MAIN_STATE_e state) {
    if (mpSoundSource != nullptr) {
        mpSoundSource->calc(*mpPosition);
        mpSoundSource->setPolyAttrs(mPolyAttr0, mPolyAttr1);
    }

    // TODO - TList
    for (SoundInfoList::Iterator it = mSoundList.GetBeginIter(); it != mSoundList.GetEndIter(); ++it) {
        it->calc();
    }

    // TODO - ...
}

void dAcBase_c::unkVirtFunc_0x5C() {
    return;
}

void dAcBase_c::unkVirtFunc_0x60() {
    return;
}

// loads f2 before f0 instead of f0 then f2
bool dAcBase_c::restorePosRotFromCopy() {
    if (mRoomID != mRoomIDCopy) {
        return 0;
    }
    mPosition = mPositionCopy;
    mRotation = mRotationCopy;
    return 1;
}

u32 dAcBase_c::itemDroppingAndGivingRelated(mVec3_c *spawnPos, int subtype) {
    if (dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL) {
        return 0;
    }

    if (spawnPos == nullptr) {
        spawnPos = &mPosition;
    }

    u32 param2Copy = mParams2;
    mParams2 = param2Copy | 0xFF000000;
    // mAng3_c rot = {};
    return SpecialItemDropMgr::GetInstance()->giveSpecialDropItem(
        param2Copy >> 0x18, mRoomID, spawnPos, subtype, 0, -1
    );
}

void dAcBase_c::fillUpperParams2Byte() {
    // Upper byte of param2 determines item drops when actor is deleted
    mParams2 |= 0xFF000000;
}

u32 dAcBase_c::getParams2_ignoreLower() {
    return mParams2 | 0xFFFF;
}

void dAcBase_c::setParams2Upper_ignoreLower(u32 val) {
    mParams2 = val | 0xFFFF;
}

int dAcBase_c::getParams2UpperByte() {
    return mParams2 >> 0x18;
}

void dAcBase_c::setParams2UpperByte(u32 val) {
    mParams2 = (mParams2 & 0xFFFFFF) | val << 0x18;
}

u32 dAcBase_c::buildParams2(u32 lower, u32 upper) {
    return ((upper & 0xFFFF) | 0xFFFF0000) & ((lower << 0x18) | 0xFFFFFF);
}

u32 dAcBase_c::getParams2Lower() const {
    return mParams2 & 0xFFFF;
}

dAcBase_c *dAcBase_c::findActor(char *objName, dAcBase_c *parent) {
    const ActorInfo *actorInfo = getActorInfoByName(objName);
    if (actorInfo == nullptr) {
        return nullptr;
    } else {
        do {
            parent = (dAcBase_c *)fManager_c::searchBaseByProfName(actorInfo->profileId, parent);
            if (parent == nullptr) {
                break;
            }
        } while (parent->mActorSubtype != actorInfo->subtype);
    }
    return parent;
}

// searches for actor based on groupType
FORCE_INLINE dAcBase_c *findActor(dAcBase_c *parent) {
    dAcBase_c *foundActor;
    if (!parent) {
        foundActor = (dAcBase_c *)fManager_c::searchBaseByGroupType(fBase_c::ACTOR, nullptr);
    } else if (parent->mGroupType == fBase_c::ACTOR) {
        foundActor = (dAcBase_c *)fManager_c::searchBaseByGroupType(fBase_c::ACTOR, parent);
    } else {
        foundActor = nullptr;
    }
    return foundActor;
}

// control flow sucks ;-;
// NOT MATCHING
dAcBase_c *dAcBase_c::searchActor(dAcBase_c *parent) {
    dAcBase_c *foundActor = ::findActor(parent);

    return foundActor ? foundActor : (dAcBase_c *)fManager_c::searchBaseByGroupType(STAGE, parent);
}

void dAcBase_c::forEveryActor(void *func(dAcBase_c *, dAcBase_c *), dAcBase_c *parent) {
    dAcBase_c *foundActor = searchActor(nullptr);

    while (foundActor != nullptr) {
        dAcBase_c *nextActor = (dAcBase_c *)func(foundActor, parent);
        foundActor = searchActor(nextActor);
    }
}

mAng dAcBase_c::getXZAngleToPlayer() const {
    return cLib::targetAngleY(this->mPosition, dAcPy_c::LINK->mPosition);
}

bool dAcBase_c::getDistanceToActor(dAcBase_c *actor, f32 distThresh, f32 *outDist) {
    f32 distSquared = 3.402823e+38;
    bool isWithinThreshhold = false;

    if (actor != nullptr) {
        distSquared = PSVECSquareDistance(mPosition, actor->mPosition);

        if (distSquared <= distThresh * distThresh) {
            isWithinThreshhold = true;
        }
    }

    if (outDist != nullptr) {
        *outDist = distSquared <= 0.0f ? 0.0f : nw4r::math::FrSqrt(distSquared) * distSquared;
    }

    return isWithinThreshhold;
}

bool dAcBase_c::getDistanceAndAngleToActor(
    dAcBase_c *actor, f32 distThresh, s16 yAngle, s16 xAngle, f32 *outDist, s16 *outDiffAngleY, s16 *outDiffAngleX
) {
    f32 distSquared = EGG::Math<f32>::epsilon();
    bool isWithinRange = false;

    mAng angleToActorY(0), angleToActorX(0);

    if (actor != nullptr) {
        distSquared = PSVECSquareDistance(mPosition, actor->mPosition);
        angleToActorY.set(cLib::targetAngleY(mPosition, actor->mPosition));
        angleToActorX.set(cLib::targetAngleX(mPosition, actor->mPosition));

        if ((distSquared <= distThresh * distThresh)) {
            if (mAng::abs((s32)(mRotation.y - angleToActorY)) <= yAngle &&
                mAng::abs((s32)(mRotation.x - angleToActorX)) <= xAngle) {
                isWithinRange = true;
            }
        }
    }

    if (outDist != nullptr) {
        *outDist = distSquared <= 0.0f ? 0.0f : nw4r::math::FrSqrt(distSquared) * distSquared;
    }

    if (outDiffAngleY != nullptr) {
        *outDiffAngleY = angleToActorY;
    }

    if (outDiffAngleX != nullptr) {
        *outDiffAngleX = angleToActorX;
    }

    return isWithinRange;
}

bool dAcBase_c::isWithinPlayerRadius(f32 radius) const {
    f32 dist_diff = getSquareDistanceTo(dAcPy_c::LINK->mPosition);
    return dist_diff < radius * radius;
}

bool dAcBase_c::getDistanceAndAngleToPlayer(
    f32 distThresh, s16 yAngle, s16 xAngle, f32 *outDist, s16 *outDiffAngleY, s16 *outDiffAngleX
) {
    return getDistanceAndAngleToActor(dAcPy_c::LINK, distThresh, yAngle, xAngle, outDist, outDiffAngleY, outDiffAngleX);
}

f32 dAcBase_c::getDistToPlayer() {
    return EGG::Math<f32>::sqrt(PSVECSquareDistance(mPosition, dAcPy_c::LINK->mPosition));
}

f32 dAcBase_c::getSquareDistToPlayer() {
    return PSVECSquareDistance(mPosition, dAcPy_c::LINK->mPosition);
}

// Some weirdness with the float registers being used
void dAcBase_c::updateRoomId(f32 yOffset) {
    if (getConnectParent()->mProfileName != fProfile::ROOM) {
        mVec3_c actorPos(mPosition.x, mPosition.y + yOffset, mPosition.z);

        if (dBgS_ObjGndChk::CheckPos(actorPos)) {
            mRoomID = dBgS_ObjGndChk::GetRoomID();
        } else {
            mRoomID = dStage_c::GetInstance()->getCurrRoomId();
        }
    }
}

bool dAcBase_c::isRoomFlags_0x6_Set() {
    dRoom_c *room = dStage_c::GetInstance()->getRoom(mRoomID);
    return (room->checkFlag(0x4 | 0x2));
}

void dAcBase_c::setSoundSourceSubtype(u8 subType) {
    if (mpSoundSource == nullptr) {
        return;
    }
    mpSoundSource->setSubtype(subType);
}

void dAcBase_c::FUN_8002d5b0() {
    if (mpSoundSource == nullptr) {
        return;
    }
    mpSoundSource->vt_0x0F0_noop();
}

bool dAcBase_c::startSound(u32 soundId) {
    if (mpSoundSource == nullptr) {
        return false;
    }
    return mpSoundSource->startSound(soundId);
}

bool dAcBase_c::startSoundWithFloatParam(u32 soundId, f32 param) {
    if (mpSoundSource == nullptr) {
        return false;
    }
    return mpSoundSource->startSoundWithFloatParam(soundId, param);
}

bool dAcBase_c::startBgHitSound(u32 soundId, const cBgS_PolyInfo &info, const mVec3_c *position) {
    if (mpSoundSource == nullptr) {
        return false;
    }
    return mpSoundSource->startBgHitSound(
        soundId, dBgS::GetInstance()->GetPolyAtt0(info), dBgS::GetInstance()->GetPolyAtt1(info),
        position != nullptr ? position : &this->mPosition
    );
}

bool dAcBase_c::startSoundAtPosition(u32 soundId, const mVec3_c *position) {
    if (mpSoundSource == nullptr) {
        return false;
    }
    return mpSoundSource->startSoundAtPosition(soundId, position != nullptr ? position : &this->mPosition);
}

bool dAcBase_c::holdSound(u32 soundId) {
    if (mpSoundSource == nullptr) {
        return false;
    }
    return mpSoundSource->holdSound(soundId);
}

bool dAcBase_c::holdSoundWithIntParam(u32 soundId, s32 param) {
    if (mpSoundSource == nullptr) {
        return false;
    }
    return mpSoundSource->holdSoundWithIntParam(soundId, param);
}

bool dAcBase_c::holdSoundWithFloatParam(u32 soundId, f32 param) {
    if (mpSoundSource == nullptr) {
        return false;
    }
    return mpSoundSource->holdSoundWithFloatParam(soundId, param);
}

bool dAcBase_c::holdSoundWithParams(u32 soundId, f32 fValue, s32 value) {
    if (mpSoundSource == nullptr) {
        return false;
    }
    return mpSoundSource->holdSoundWithParams(soundId, fValue, value);
}

void dAcBase_c::holdSoundSourceFlag(u32 mask) {
    if (mpSoundSource == nullptr) {
        return;
    }
    mpSoundSource->holdFlag(mask);
}

void dAcBase_c::onSoundSourceFlag(u32 mask) {
    if (mpSoundSource == nullptr) {
        return;
    }
    mpSoundSource->onFlag(mask);
}

void dAcBase_c::offSoundSourceFlag(u32 mask) {
    if (mpSoundSource == nullptr) {
        return;
    }
    mpSoundSource->offFlag(mask);
}

bool dAcBase_c::isPlayingSound(u32 soundId) {
    if (mpSoundSource == nullptr) {
        return false;
    }
    return mpSoundSource->isPlayingSound(soundId);
}

void dAcBase_c::setBattleBgmRelated(UNKWORD param) {
    if (mpSoundSource == nullptr) {
        return;
    }
    mpSoundSource->setBattleBgmRelated(param);
}

dSoundSourceIf_c *dAcBase_c::getSoundSource() {
    return mpSoundSource.get();
}

void dAcBase_c::removeSoundInfo(SoundInfo *soundInfo) {
    if (mSoundList.GetPosition(soundInfo) != mSoundList.GetEndIter()) {
        mSoundList.remove(soundInfo);
    }
}

void dAcBase_c::setActorRef(dAcBase_c *ref) {
    mActorNode.link(ref);
}

void dAcBase_c::setEnemyDefeatFlag() {
    EnemyflagManager::sInstance->setFlag(mObjID);
}

void dAcBase_c::changeLoadedEntitiesWithSet() {
    dStage_c::GetInstance()->changeLoadedEntities(mObjID, true);
}

void dAcBase_c::changeLoadedEntitiesNoSet() {
    dStage_c::GetInstance()->changeLoadedEntities(mObjID, false);
}

// spawns GroupType2 (ACTOR)
dAcBase_c *dAcBase_c::createActor(
    ProfileName actorId, u32 actorParams1, mVec3_c *actorPosition, mAng3_c *actorRotation, mVec3_c *actorScale,
    u32 actorParams2, s32 actorRoomid, dBase_c *actorRef
) {
    if (actorPosition == nullptr) {
        actorPosition = &mPosition;
    }

    if (actorRotation == nullptr) {
        actorRotation = &mRotation;
    }

    if (actorScale == nullptr) {
        actorScale = &mScale;
    }

    if (actorRoomid == 63) {
        actorRoomid = mRoomID;
    }

    u32 newParams2 = actorParams2 != 0 ? getParams2_ignoreLower() : -1;

    setTempCreateParams(
        actorPosition, actorRotation, actorScale, actorRoomid, newParams2, (dAcBase_c *)actorRef, 0, -1, -1, nullptr
    );
    dBase_c *room = dStage_c::getParentForRoom(mRoomID);
    return (dAcBase_c *)dBase_c::createBase(actorId, room, actorParams1, ACTOR);
}

// spawns GroupType2 (STAGE)
dAcBase_c *dAcBase_c::createActorStage(
    ProfileName actorId, u32 actorParams1, mVec3_c *actorPosition, mAng3_c *actorRotation, mVec3_c *actorScale,
    u32 actorParams2, s32 actorRoomid, dBase_c *actorRef
) {
    if (actorPosition == nullptr) {
        actorPosition = &mPosition;
    }

    if (actorRotation == nullptr) {
        actorRotation = &mRotation;
    }

    if (actorScale == nullptr) {
        actorScale = &mScale;
    }

    if (actorRoomid == 63) {
        actorRoomid = mRoomID;
    }

    u32 newParams2 = actorParams2 != 0 ? getParams2_ignoreLower() : -1;

    setTempCreateParams(
        actorPosition, actorRotation, actorScale, actorRoomid, newParams2, (dAcBase_c *)actorRef, 0, -1, -1, nullptr
    );
    dBase_c *room = dStage_c::getParentForRoom(mRoomID);
    return (dAcBase_c *)dBase_c::createBase(actorId, room, actorParams1, STAGE);
}

void dAcBase_c::registerInEvent() {}

void dAcBase_c::unkVirtFunc_0x6C() {}

void dAcBase_c::doInteraction(s32 param) {
    if (param == TALK || param == EXAMINE_TALK || param == READ) {
        Event event = Event("DefaultTalk", 400, 0x100001, nullptr, nullptr);
        EventManager::alsoSetAsCurrentEvent(this, &event, nullptr);
    }
}

// Only called by dPlayer::dig
// Rounds angle to nearest 90 deg?
void dAcBase_c::roundAngleToNearest90(s16 *dst_angle, s16 *src_angle) {
    s32 roundedAngle = *src_angle;

    if (roundedAngle < 0) {
        roundedAngle += 0x10000;
    }
    roundedAngle += 0x2000;

    *dst_angle = (roundedAngle / 0x4000) * 0x4000;
}

void dAcBase_c::incrementKillCounter() {
    dAcObjBase_c *object = static_cast<dAcObjBase_c *>(this); // Probably wrong

    if (mGroupType == ACTOR && object->mTargetFiRelated == 1) {
        int killCounterId = object->mTargetFiTextID;

        if (killCounterId < 91 && (killCounterId & 0x300) == 0) {
            FileManager *fileMgr = FileManager::GetInstance();
            u16 killCount = fileMgr->getEnemyKillCount(killCounterId) + 1;
            fileMgr->setEnemyKillCount(killCounterId, killCount);
        }
    }
}

void dAcBase_c::killNoItemDrop() {
    fillUpperParams2Byte();
    fBase_c::deleteRequest();
    incrementKillCounter();
}

void dAcBase_c::killWithFlag() {
    setEnemyDefeatFlag();
    fBase_c::deleteRequest();
    incrementKillCounter();
}

void dAcBase_c::killWithFlagNoItemDrop() {
    fillUpperParams2Byte();
    killWithFlag();
}

void dAcBase_c::deleteWithFlagNoItemDrop() {
    fillUpperParams2Byte();
    setEnemyDefeatFlag();
    fBase_c::deleteRequest();
}

void dAcBase_c::setPolyAttrs(cBgS_PolyInfo &pPolyInfo) {
    mPolyAttr0 = dBgS::GetInstance()->GetPolyAtt0(pPolyInfo);
    mPolyAttr1 = dBgS::GetInstance()->GetPolyAtt1(pPolyInfo);
}

// Idk what's up with this function. It's only used once.
void dAcBase_c::setPolyAttrsDupe(cBgS_PolyInfo &pPolyInfo) {
    setPolyAttrs(pPolyInfo);
}
