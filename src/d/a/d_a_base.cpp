#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/d_a_player.h"
#include "f/f_list_nd.h"
#include "m/m_vec.h"
#include "toBeSorted/event.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/misc_flag_managers.h"
#include "toBeSorted/scgame.h"
#include "toBeSorted/special_item_drop_mgr.h"

// .sdata
u32 dAcBase_c::s_Create_RoomId = -1;
u32 dAcBase_c::s_Create_Params2 = -1;
u16 dAcBase_c::s_Create_UnkFlags = -1;
u8 dAcBase_c::s_Create_ViewClipIdx = -1;

// .sbss
mVec3_c *dAcBase_c::s_Create_Position;
mAng3_c *dAcBase_c::s_Create_Rotation;
mVec3_c *dAcBase_c::s_Create_Scale;
dAcBase_c *dAcBase_c::s_Create_Parent;
ObjInfo *dAcBase_c::s_Create_ObjInfo;
u8 dAcBase_c::s_Create_Subtype;

extern "C" ObjInfo *getObjByActorIdAndSubtype_unkNamespace(ProfileName, u8);
extern "C" ObjInfo *getObjByActorName_unkNamespace(char *name);
extern "C" char *getObjectName_8006a730(ObjInfo *);
extern "C" SoundSource *soundForActorInitRelated_803889c0(s8, fBase_c *, char *, u8);
extern "C" s16 targetAngleY(mVec3_c *, mVec3_c *);
extern "C" s16 targetAngleX(mVec3_c *, mVec3_c *);
extern "C" bool checkCollision(mVec3_c *pos);
extern "C" s8 collisionCheckGetRoom();
extern "C" dRoom *getRoomByIndex(RoomManager *mgr, s8 roomid);
extern "C" bool alsoSetAsCurrentEvent(dAcBase_c *, Event *, void *);

bool dAcBase_c::createHeap() {
    return true;
}

// Doesnt Match Yet
dAcBase_c::dAcBase_c()
    : heap_allocator(), obj_info(s_Create_ObjInfo), sound_source(nullptr), sound_list(), obj_pos(&position),
      params2(s_Create_Params2), obj_id(s_Create_UnkFlags), viewclip_index(s_Create_ViewClipIdx), actor_node(nullptr),
      roomid(s_Create_RoomId), actor_subtype(s_Create_Subtype) {
    JStudio_actor = 0;
    someStr[0] = 0;

    if (s_Create_Position) {
        setPostion(*s_Create_Position);
    }

    if (s_Create_Rotation) {
        SetRotation(*s_Create_Rotation);
    }

    if (s_Create_Scale) {
        SetScale(*s_Create_Scale);
    } else {
        scale.set(1.0f, 1.0f, 1.0f);
    }

    if (s_Create_Parent != nullptr) {
        setActorRef(s_Create_Parent);
    }

    fProfile::fActorProfile_c *profile = (fProfile::fActorProfile_c *)((*fProfile::sProfileList)[profile_name]);
    actor_properties = profile->mActorProperties;
    if (obj_info == nullptr) {
        obj_info = getObjByActorIdAndSubtype_unkNamespace(profile_name, actor_subtype);
    }
    someStr[0] = '\0';
}

// 8002c530
// dBase_c::~dBase_c() {}

dAcBase_c::~dAcBase_c() {}

void dAcBase_c::setTempCreateParams(mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, s32 roomId, u32 params2,
        dAcBase_c *parent, u8 subtype, s16 unkFlag, u8 viewClipIdx, ObjInfo *objInfo) {
    s_Create_Position = pos;
    s_Create_Rotation = rot;
    s_Create_Scale = scale;
    s_Create_RoomId = roomId;
    s_Create_Params2 = params2;
    s_Create_Parent = parent;
    s_Create_Subtype = subtype;
    s_Create_UnkFlags = unkFlag;
    s_Create_ViewClipIdx = viewClipIdx;
    s_Create_ObjInfo = objInfo;
}

// has regswap
SoundSource *dAcBase_c::FUN_8002c690() {
    if (obj_info == nullptr) {
        return nullptr;
    }

    s32 unk_val = obj_info->unk_0xA;
    if (unk_val == -1) {
        return nullptr;
    }

    char *objName = getObjectName_8006a730(obj_info);
    return soundForActorInitRelated_803889c0(unk_val, this, objName, subtype);
}

int dAcBase_c::initAllocatorWork1Heap(int size, char *name, int align) {
    return initAllocator(size, name, dHeap::work1Heap.heap, align);
}

int dAcBase_c::initAllocator(int size, char *name, EGG::Heap *heap, int align) {
    if (!heap_allocator.createNewTempFrmHeap(size, heap, name, 0x20, 0)) {
        return 0;
    }
    sound_source = FUN_8002c690();
    int success = createHeap();
    heap_allocator.adjustFrmHeapRestoreCurrent();
    return success;
}

bool dAcBase_c::addActorToRoom(s32 roomId) {
    dBase_c *room = RoomManager::getRoom(roomId);
    if (room == nullptr) {
        return false;
    }
    if (setConnectChild(room)) {
        if (roomId == -1) {
            this->roomid = RoomManager::m_Instance->curr_room_id;
        } else {
            this->roomid = roomId;
        }
        return true;
    }
    return false;
}

void dAcBase_c::setBit_field_0xE8(s32 shift) {
    field_0xe8 |= (1 << shift);
}

int dAcBase_c::actorCreate() {
    return SUCCEEDED;
}

int dAcBase_c::actorPostCreate() {
    return SUCCEEDED;
}

int dAcBase_c::create() {
    if (actor_properties & 0x8000000) {
        return actorPostCreate();
    }
    int success = actorCreate();
    if (success == SUCCEEDED) {
        success = NOT_READY;
        actor_properties |= 0x8000000;
    }
    return success;
}

// 8002c8f0
// loads f2 before f0 instead of f0 then f2
void dAcBase_c::postCreate(fBase_c::MAIN_STATE_e state) {
    if (state == SUCCESS) {
        pos_copy = position;
        rot_copy = rotation;
        room_id_copy = roomid;
    }
    dBase_c::postCreate(state);
}

// 8002c940
int dAcBase_c::preDelete() {}

// 8002cb10
int dAcBase_c::preExecute() {
    if (dBase_c::preExecute() == NOT_READY) {
        return NOT_READY;
    }
    if (actor_properties & 0x10000000) {
        if (actor_properties & 0x40000000) {
            return NOT_READY;
        }
        // TODO: Add event control
    }
    return SUCCEEDED;
}

// 8002cc10
int dAcBase_c::execute() {}

// 8002cca0
int dAcBase_c::actorExecute() {
    return SUCCEEDED;
}

// 8002ccb0
int dAcBase_c::actorExecuteInEvent() {
    return actorExecute();
}

// 8002ccc0
void dAcBase_c::postExecute(fBase_c::MAIN_STATE_e state) {}

// 8002ce90
void dAcBase_c::unkVirtFunc_0x5C() {
    return;
}

// 8002cea0
void dAcBase_c::unkVirtFunc_0x60() {
    return;
}

// 8002ceb0
// loads f2 before f0 instead of f0 then f2
bool dAcBase_c::restorePosRotFromCopy() {
    if (roomid != room_id_copy) {
        return 0;
    }
    position = pos_copy;
    rotation = rot_copy;
    return 1;
}

// 8002cf10
u32 dAcBase_c::itemDroppingAndGivingRelated(mVec3_c *spawnPos, int subtype) {
    if (ScGame::currentSpawnInfo.trial == 1) {
        return 0;
    }

    if (spawnPos == nullptr) {
        spawnPos = &position;
    }

    u32 param2Copy = params2;
    params2 = param2Copy | 0xFF000000;
    // mAng3_c rot = {};
    s16 rot = 0;
    return SpecialItemDropMgr::giveSpecialDropItem(SpecialItemDropMgr::sInstance, param2Copy >> 0x18, roomid, spawnPos,
            subtype, &rot, -1);
}

// 8002cf90
void dAcBase_c::fillUpperParams2Byte() {
    params2 |= 0xFF000000;
}

// 8002cfa0
u32 dAcBase_c::getParams2_ignoreLower() {
    return params2 | 0xFFFF;
}

// 8002cfb0
void dAcBase_c::setParams2Upper_ignoreLower(u32 val) {
    params2 = val | 0xFFFF;
}

// 8002cfc0
u8 dAcBase_c::getParams2UpperByte() {
    return params2 >> 0x18;
}

// 8002cfd0
void dAcBase_c::setParams2UpperByte(u32 val) {
    params2 = (params2 & 0xFFFFFF) | val << 0x18;
}

// 8002cff0
u32 dAcBase_c::buildParams2(u32 lower, u32 upper) {
    return ((upper & 0xFFFF) | 0xFFFF0000) & ((lower << 0x18) | 0xFFFFFF);
}

// 8002d010
u32 dAcBase_c::getParams2Lower() {
    return params2 & 0xFFFF;
}

// 8002d020
dAcBase_c *dAcBase_c::findActor(char *objName, dAcBase_c *parent) {
    ObjInfo *objInfo = getObjByActorName_unkNamespace(objName);
    if (objInfo == nullptr) {
        return nullptr;
    } else {
        do {
            parent = (dAcBase_c *)fManager_c::searchBaseByProfName(objInfo->obj_id, parent);
            if (parent == nullptr) {
                break;
            }
        } while (parent->actor_subtype != objInfo->subtype);
    }
    return parent;
}

// searches for actor based on groupType
// 8002d0a0
DECOMP_INLINE dAcBase_c *findActor(dAcBase_c *parent) {
    if (!parent) {
        return (dAcBase_c *)fManager_c::searchBaseByGroupType(2, nullptr);
    }
    if (parent->group_type == 2) {
        return (dAcBase_c *)fManager_c::searchBaseByGroupType(2, parent);
    }
    return nullptr;
}

dAcBase_c *dAcBase_c::searchActor(dAcBase_c *parent) {
    dAcBase_c *foundActor = ::findActor(parent);

    if (foundActor) {
        return foundActor;
    } else {
        return (dAcBase_c *)fManager_c::searchBaseByGroupType(STAGE, parent);
    }
}

// 8002d130
void dAcBase_c::forEveryActor(void *func(dAcBase_c *, dAcBase_c *), dAcBase_c *parent) {
    dAcBase_c *foundActor = searchActor(nullptr);

    while (foundActor != nullptr) {
        dAcBase_c *nextActor = (dAcBase_c *)func(foundActor, parent);
        foundActor = searchActor(nextActor);
    }
}

// 8002d190
mAng dAcBase_c::getXZAngleToPlayer() {
    return targetAngleY(&this->position, &dAcPy_c::LINK->position);
}

// 8002d1d0
bool dAcBase_c::getDistanceToActor(dAcBase_c *actor, f32 distThresh, f32 *outDist) {
    f32 distSquared = 3.402823e+38;
    bool isWithinThreshhold = false;

    if (actor != nullptr) {
        distSquared = PSVECSquareDistance(position, actor->position);

        if (distSquared <= distThresh * distThresh) {
            isWithinThreshhold = true;
        }
    }

    if (outDist != nullptr) {
        *outDist = distSquared <= 0.0f ? 0.0f : nw4r::math::FrSqrt(distSquared) * distSquared;
    }

    return isWithinThreshhold;
}

s32 doAbs(s16 val) {
    return labs(val);
}
// Similar weirdness as the above function. Also, r29->31 are initted in the wrong order?
// 8002d290
bool dAcBase_c::getDistanceAndAngleToActor(dAcBase_c *actor, f32 distThresh, s16 yAngle, s16 xAngle, f32 *outDist,
        s16 *outDiffAngleY, s16 *outDiffAngleX) {
    f32 distSquared = 3.402823e+38f;
    s16 angleToActorY, angleToActorX;
    bool isWithinRange = false;

    angleToActorY = 0;
    angleToActorX = 0;

    if (actor != nullptr) {
        distSquared = PSVECSquareDistance(position, actor->position);
        angleToActorY = targetAngleY(&position, &actor->position);
        angleToActorX = targetAngleX(&position, &actor->position);

        if ((distSquared <= distThresh * distThresh) && (doAbs(s32(rotation.y.mVal - angleToActorY)) <= yAngle) &&
                (doAbs(s32(rotation.x.mVal - angleToActorX)) <= xAngle)) {
            isWithinRange = true;
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

// 8002d3e0
bool dAcBase_c::isWithinPlayerRadius(f32 radius) const {
    f32 dist_diff = getSquareDistanceTo(dAcPy_c::LINK->position);
    return dist_diff < radius * radius;
}

// 8002d440
bool dAcBase_c::getDistanceAndAngleToPlayer(f32 distThresh, s16 yAngle, s16 xAngle, f32 *outDist, s16 *outDiffAngleY,
        s16 *outDiffAngleX) {
    return getDistanceAndAngleToActor(dAcPy_c::LINK, distThresh, yAngle, xAngle, outDist, outDiffAngleY, outDiffAngleX);
}

// 8002d470
f32 dAcBase_c::getDistToPlayer() {
    return EGG::Math<f32>::sqrt(PSVECSquareDistance(position, dAcPy_c::LINK->position));
}

// 8002d4a0
f32 dAcBase_c::getSquareDistToPlayer() {
    return PSVECSquareDistance(position, dAcPy_c::LINK->position);
}

// Some weirdness with the float registers being used
// 8002d4b0
void dAcBase_c::updateRoomId(f32 yOffset) {
    if (getConnectParent()->profile_name != 701 /* fProfile::PROFILE_NAME_e::ROOM */) {
        mVec3_c actorPos(position.x, position.y + yOffset, position.z);

        if (checkCollision(&actorPos)) {
            roomid = collisionCheckGetRoom();
        } else {
            roomid = RoomManager::m_Instance->curr_room_id;
        }
    }
}

// 8002d540
bool dAcBase_c::isRoomFlags_0x6_Set() {
    dRoom *room = RoomManager::m_Instance->GetRoomByIndex(roomid);
    return (room->flags & 6);
}

// Start of SoundSource stuff
void dAcBase_c::FUN_8002d590() {}
void dAcBase_c::FUN_8002d5b0() {}
void dAcBase_c::playSound(u16) {}
void dAcBase_c::FUN_8002d600() {}
void dAcBase_c::FUN_8002d630() {}
void dAcBase_c::FUN_8002d6d0() {}
// currently named ActorBase__playSoundEffect1
void dAcBase_c::playSoundEffect1() {}
void dAcBase_c::FUN_8002d740() {}
void dAcBase_c::FUN_8002d770() {}
void dAcBase_c::FUN_8002d7a0() {}
void dAcBase_c::FUN_8002d7d0() {}
void dAcBase_c::FUN_8002d7f0() {}
void dAcBase_c::FUN_8002d810() {}
void dAcBase_c::FUN_8002d830() {}
void dAcBase_c::FUN_8002d860() {}

// 8002d880
SoundSource *dAcBase_c::getSoundSource() {
    return sound_source;
}
// End of SoundSource stuff

// first param is not dAcBase_c
// 8002d890
void dAcBase_c::FUN_8002d890() {}

// current name is Global__setActorRef
void dAcBase_c::setActorRef(dAcBase_c *ref) {
    actor_node.link(ref);
}

// May not be only purpose
void dAcBase_c::setEnemyDefeatFlag() {
    EnemyDefeatManager::sInstance->setFlag(obj_id);
}

// 8002d940
void dAcBase_c::changeLoadedEntitiesWithSet() {
    RoomManager::m_Instance->changeLoadedEntities(RoomManager::m_Instance, obj_id, true);
}

// 8002d960
void dAcBase_c::changeLoadedEntitiesNoSet() {
    RoomManager::m_Instance->changeLoadedEntities(RoomManager::m_Instance, obj_id, false);
}

// spawns GroupType2 (ACTOR)
// 8002d980
dAcBase_c *dAcBase_c::createActor(ProfileName actorId, u32 actorParams1, mVec3_c *actorPosition, mAng3_c *actorRotation,
        mVec3_c *actorScale, u32 actorParams2, s32 actorRoomid, dBase_c *actorRef) {
    if (actorPosition == nullptr) {
        actorPosition = &position;
    }

    if (actorRotation == nullptr) {
        actorRotation = &rotation;
    }

    if (actorScale == nullptr) {
        actorScale = &scale;
    }

    if (actorRoomid == 63) {
        actorRoomid = roomid;
    }

    u32 newParams2 = actorParams2 != 0 ? getParams2_ignoreLower() : -1;

    setTempCreateParams(actorPosition, actorRotation, actorScale, actorRoomid, newParams2, (dAcBase_c *)actorRef, 0, -1,
            0xFF, nullptr);
    dBase_c *room = RoomManager::getRoom(roomid);
    return (dAcBase_c *)dBase_c::createBase(actorId, room, actorParams1, ACTOR);
}

// spawns GroupType2 (STAGE)
// 8002da80
dAcBase_c *dAcBase_c::createActorStage(ProfileName actorId, u32 actorParams1, mVec3_c *actorPosition,
        mAng3_c *actorRotation, mVec3_c *actorScale, u32 actorParams2, s32 actorRoomid, dBase_c *actorRef) {
    if (actorPosition == nullptr) {
        actorPosition = &position;
    }

    if (actorRotation == nullptr) {
        actorRotation = &rotation;
    }

    if (actorScale == nullptr) {
        actorScale = &scale;
    }

    if (actorRoomid == 63) {
        actorRoomid = roomid;
    }

    u32 newParams2 = actorParams2 != 0 ? getParams2_ignoreLower() : -1;

    setTempCreateParams(actorPosition, actorRotation, actorScale, actorRoomid, newParams2, (dAcBase_c *)actorRef, 0, -1,
            0xFF, nullptr);
    dBase_c *room = RoomManager::getRoom(roomid);
    return (dAcBase_c *)dBase_c::createBase(actorId, room, actorParams1, STAGE);
}

// 8002db80
void *dAcBase_c::getCurrentEventActor() {}

// 8002db90
void dAcBase_c::unkVirtFunc_0x6C() {}

// 8002dba0
void dAcBase_c::doInteraction(s32 param) {
    if (param == 4 || param == 5 || param == 12) {
        Event event = Event("DefaultTalk", 400, 0x100001, nullptr, nullptr);
        alsoSetAsCurrentEvent(this, &event, nullptr);
    }
}

// Only called by dPlayer::dig and that function fails to decomp in ghidra?
// 8002dc20
void dAcBase_c::FUN_8002dc20(s16 *, s16 *) {}

// 8002dc50
void dAcBase_c::incrementKillCounter() {
    dAcObjBase_c *object = (dAcObjBase_c *)this; // Probably wrong

    if (group_type == ACTOR && object->unkByteTargetFiRelated == 1) {
        int killCounterId = object->targetFiTextId;

        if (killCounterId < 91 && (killCounterId & 0x300) == 0) {
            FileManager *fileMgr = FileManager::sInstance;
            u16 killCount = fileMgr->getEnemyKillCount(killCounterId) + 1;
            fileMgr->setEnemyKillCount(killCounterId, killCount);
        }
    }
}

// 8002dcd0
void dAcBase_c::FUN_8002dcd0() {
    fillUpperParams2Byte();
    fBase_c::deleteRequest();
    incrementKillCounter();
}

// 8002dd10
void dAcBase_c::FUN_8002dd10() {
    setEnemyDefeatFlag();
    fBase_c::deleteRequest();
    incrementKillCounter();
}

// 8002dd50
void dAcBase_c::FUN_8002dd50() {
    fillUpperParams2Byte();
    FUN_8002dd10();
}

// 8002dd90
void dAcBase_c::FUN_8002dd90() {
    fillUpperParams2Byte();
    setEnemyDefeatFlag();
    fBase_c::deleteRequest();
}

// Some collision related thing
// 8002ddd0
void dAcBase_c::FUN_8002ddd0() {}

// 8002de30
void dAcBase_c::FUN_8002de30() {
    FUN_8002ddd0();
}
