#include <d/a/d_a_base.h>

// RoomManager* RoomManager::m_Instance;

// .sdata
u32 dAcBase_c::s_Create_RoomId = -1;
u32 dAcBase_c::s_Create_Params2 = -1;
u16 dAcBase_c::s_Create_UnkFlags = -1;
u8  dAcBase_c::s_Create_ViewClipIdx = -1;

// .sbss
mVec3_c* dAcBase_c::s_Create_Position;
mAng3_c* dAcBase_c::s_Create_Rotation;
mVec3_c* dAcBase_c::s_Create_Scale;
dAcBase_c* dAcBase_c::s_Create_Parent;
ObjInfo* dAcBase_c::s_Create_ObjInfo;
u8 dAcBase_c::s_Create_Subtype;

extern "C" ObjInfo* getObjByActorIdAndSubtype_unkNamespace(ProfileName, u8);
extern "C" char* getObjectName_8006a730(ObjInfo*);
extern "C" void* soundForActorInitRelated_803889c0(s8, fBase_c*, char*, u8);

bool dAcBase_c::createHeap() {
    return true;
}

// Doesnt Match Yet
dAcBase_c::dAcBase_c() :
    heap_allocator() ,
    obj_info(s_Create_ObjInfo)
    // sound_list(&heap_allocator.mHeap, &heap_allocator.mHeap)
    // obj_sound(0),
    // obj_pos(&position), 
    // params2(s_Create_Params2),
    // obj_id(s_Create_UnkFlags), 
    // viewclip_index(s_Create_ViewClipIdx),
    // actor_node(0), 
    // roomid(s_Create_RoomId)
{

        // obj_info = s_Create_ObjInfo; 
    sound_info_tail = nullptr;
    sound_info_next = nullptr;
    obj_sound = nullptr;
    obj_pos = &position;
    params2 = (s_Create_Params2);
    obj_id = (s_Create_UnkFlags);
    viewclip_index = (s_Create_ViewClipIdx);
    actor_node = (0);
    roomid  =(s_Create_RoomId);
    
    actor_subtype = s_Create_Subtype;
    field_0xF0 = 0;
    someStr[0] = 0;
    if (s_Create_Position != nullptr) {
        // void set(f32 fx, f32 fy, f32 fz) { x = fx; y = fy; z = fz; }
        position.set(
            s_Create_Position->x,
            s_Create_Position->y,
            s_Create_Position->z 
        );
        // position = *s_Create_Position;
    }
    if (s_Create_Rotation != 0) {
        rotation = *(s_Create_Rotation);
    }
    if (s_Create_Scale != 0) {
        setScale(s_Create_Scale->x, s_Create_Scale->y, s_Create_Scale->z);
    } else {
        setScale(1.0, 1.0, 1.0);
    }
    if (s_Create_Parent != 0){
        setActorRef(s_Create_Parent);
    }
    fProfile::fActorProfile_c* profile = (fProfile::fActorProfile_c*)((*fProfile::sProfileList)[profile_name]);
    actor_properties = profile->mActorProperties;
    if (obj_info == nullptr) {
        obj_info = getObjByActorIdAndSubtype_unkNamespace(profile_name, actor_subtype);
    }
    someStr[0] = '\0';
}   

/* 8002c530 */ dBase_c::~dBase_c() {}

dAcBase_c::~dAcBase_c() {}

void dAcBase_c::setTempCreateParams( \
    mVec3_c* pos, mAng3_c* rot, mVec3_c* scale, \
    s32 roomId, u32 params2, dAcBase_c* parent, \
    u8 subtype, s16 unkFlag, u8 viewClipIdx,\
    ObjInfo* objInfo ) 
{
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
void* dAcBase_c::FUN_8002c690() {
    if (obj_info == nullptr) {
        return nullptr;
    }
    s8 unk_val = obj_info->unk_0xA;
    if ( unk_val == -1) {
        return nullptr;
    }
    char* objName = getObjectName_8006a730(obj_info);
    return soundForActorInitRelated_803889c0(unk_val, this, objName, subtype);
}

int dAcBase_c::initAllocatorWork1Heap(int size, char* name, int align) {
    initAllocator(size, name, dHeap::work1Heap, align);
}

extern "C" int fn_802EE510(mAllocator_c*, int size, EGG::Heap* heap, char* name, int align, int unk);
int dAcBase_c::initAllocator(int size, char* name, EGG::Heap* heap, int align) {
    if (fn_802EE510(&heap_allocator, size, heap, name, 0x20, 0) == 0){
        return 0;
    }
    obj_sound = FUN_8002c690();
    int success = createHeap();
    heap_allocator.adjustFrmHeapRestoreCurrent();
    return success;
}

bool dAcBase_c::addActorToRoom(s32 roomId) {
    dBase_c* room = RoomManager::getRoom(roomId);
    if (room == nullptr) {
        return false;
    }
    if (setConnectChild(room)){
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
    if (success == SUCCEEDED){
        success = NOT_READY;
        actor_properties |= 0x8000000;
    }
    return success;
}

void dAcBase_c::postCreate(fBase_c::MAIN_STATE_e state) {
    if (state == SUCCESS) {
        pos_copy = position; 
        copyRotation(); 
        room_id_copy = roomid;
    }
    dBase_c::postCreate(state);
}

int dAcBase_c::preDelete() {}
int dAcBase_c::preExecute() {}
int dAcBase_c::execute() {}
int dAcBase_c::actorExecute() {}
int dAcBase_c::actorExecuteInEvent() {}
void dAcBase_c::postExecute(fBase_c::MAIN_STATE_e state) {}
void dAcBase_c::unkVirtFunc_0x5C() { return; }
void dAcBase_c::unkVirtFunc_0x60() { return; }
bool dAcBase_c::restorePosRotFromCopy() {}
u32 dAcBase_c::itemDroppingAndGivingRelated(Vec3f* spawnPos, int subtype) {}
void dAcBase_c::fillUpperParams2Byte() {}
u32 dAcBase_c::getParams2_ignoreLower() {}
void dAcBase_c::setParams2Upper_ignoreLower(u32 val) {}
u8 dAcBase_c::getParams2UpperByte() {}
void dAcBase_c::setParams2UpperByte(u32 val) {}
u32 dAcBase_c::buildParams2(u32 lower, u8 upper) {}
u32 dAcBase_c::getParams2Lower() {}
dAcBase_c* dAcBase_c::findActor(char* objName, dAcBase_c* parent) {}
// searches for actor based on groupType
dAcBase_c* dAcBase_c::searchActor(dAcBase_c& optionalParent) {}

void dAcBase_c::forEachActor(void*, dAcBase_c&) {}
mAng dAcBase_c::getXZAngleToPlayer() {}
bool dAcBase_c::getDistanceToActor(dAcBase_c& actor, f32 distThresh, f32* outDist) {}
bool dAcBase_c::getDistanceAndAngleToActor( \
    dAcBase_c& actor, f32 distThresh, s16 yAngle, s16 xAngle, \
    f32* outDist, s16* outDiffAngleY, s16* outDiffAngleX) 
{

}
bool dAcBase_c::isWithinPlayerRadius(f32 radius) {}
bool dAcBase_c::getDistanceAndAngleToPlayer( \
    f32 distThresh, s16 yAngle, s16 xAngle, \
    f32* outDist, s16* outDiffAngleY, s16* outDiffAngleX) 
{
    // return getDistanceAndAngleToActor(PLAYER, distThresh, yAngle, xAngle, outDist, outDiffAngleY, outDiffAngleX);
}

f32 dAcBase_c::getDistToPlayer() {}
f32 dAcBase_c::getSquareDistToPlayer() {} 
void dAcBase_c::updateRoomId(f32 yOffs) {}
bool dAcBase_c::isRoomFlags_0x6_Set() {}
void dAcBase_c::FUN_8002d590() {}
void dAcBase_c::FUN_8002d5b0() {}
void dAcBase_c::playSound() {}
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
void dAcBase_c::FUN_8002d880() {}
 // first param is not dAcBase_c
void dAcBase_c::FUN_8002d890() {}
 // current name is Global__setActorRef
void dAcBase_c::setActorRef(dBase_c*) {}
 // May not be only purpose
void dAcBase_c::setUnkFlag() {}
void dAcBase_c::FUN_8002d940() {}
void dAcBase_c::FUN_8002d960() {}

// spawns GroupType2 (ACTOR)
dAcBase_c dAcBase_c::createActor( \
    ProfileName actorId, u32 params1, \
    Vec3f* pos, Vec3s* rot, Vec3f* scale, \
    u32 params2, s32 roomId, dBase_c* ref) {}

dAcBase_c dAcBase_c::createActorUnkGroup3( \
    ProfileName actorId, u32 params1, \
    Vec3f* pos, Vec3s* rot, Vec3f* scale, \
    u32 params2, s32 roomId, dBase_c* ref) {}

void* dAcBase_c::getCurrentEventActor() {}
void dAcBase_c::unkVirtFunc_0x6C() {}
void dAcBase_c::doInteraction(s32) {}
void dAcBase_c::FUN_8002dc20(s16*, s16*) {}
void dAcBase_c::incrementKillCounter() {}
void dAcBase_c::FUN_8002dcd0() {}
void dAcBase_c::FUN_8002dd10() {}
void dAcBase_c::FUN_8002dd50() {}
void dAcBase_c::FUN_8002dd90() {}
void dAcBase_c::FUN_8002ddd0() {}
void dAcBase_c::FUN_8002de30() {}