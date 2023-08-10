#include <d/a/d_a_base.h>


// .sdata
u32 dAcBase_c::s_Create_RoomId = -1;
u32 dAcBase_c::s_Create_Params2 = -1;
u16 dAcBase_c::s_Create_UnkFlags = -1;
u8  dAcBase_c::s_Create_ViewClipIdx = -1;

// .sbss
Vec3f* dAcBase_c::s_Create_Position;
Vec3s* dAcBase_c::s_Create_Rotation;
Vec3f* dAcBase_c::s_Create_Scale;
dAcBase_c* dAcBase_c::s_Create_Parent;
ObjInfoPtr* dAcBase_c::s_Create_ObjInfo;
u8 dAcBase_c::s_Create_Subtype;

bool dAcBase_c::createHeap() {
    return true;
}

dAcBase_c::dAcBase_c() {}

/* 8002c530 */ dBase_c::~dBase_c() {}

dAcBase_c::~dAcBase_c() {}

void dAcBase_c::setTempCreateParams( \
    Vec3f* pos, Vec3s* rot, Vec3f* scale, \
    s32 roomId, u32 params2, dAcBase_c* parent, \
    u8 subtype, s16 unkFlag, u8 viewClipIdx,\
    ObjInfoPtr* objInfo ) 
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
void* dAcBase_c::FUN_8002c690() {}
int dAcBase_c::initAllocatorWork1Heap(int size, char* name, int align) {
    initAllocator(size, name, dHeap::work1Heap, align);
}

extern "C" int fn_802EE510(mAllocator_c*, int size, EGG::Heap* heap, char* name, int align, int unk);
int dAcBase_c::initAllocator(int size, char* name, EGG::Heap* heap, int align) {
    if (fn_802EE510(&mHeapAllocator, size, heap, name, 0x20, 0) == 0){
        return 0;
    }
    soundRelated = FUN_8002c690();
    int success = createHeap();
    mHeapAllocator.adjustFrmHeapRestoreCurrent();
    return success;
}

bool dAcBase_c::addActorToRoom(s32 roomId) {}
void dAcBase_c::setBit_field_0xE8(s32) {}
int dAcBase_c::actorCreate() {}
int dAcBase_c::actorReCreate() {}
int dAcBase_c::create() {}
void dAcBase_c::postCreate(fBase_c::MAIN_STATE_e state) {}
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
void dAcBase_c::setActorRef(dBase_c&) {}
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