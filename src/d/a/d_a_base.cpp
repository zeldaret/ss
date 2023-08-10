#include <d/a/d_a_base.h>


// .sdata
/* 0x80571920 */ u32 dAcBase_c::s_Create_RoomId = -1;
/* 0x80571924 */ u32 dAcBase_c::s_Create_Params2 = -1;
/* 0x80571928 */ u16 dAcBase_c::s_Create_UnkFlags = -1;
/* 0x8057192A */ u8  dAcBase_c::s_Create_ViewClipIdx = -1;

// .sbss
/* 0x80575080 */ Vec3f* dAcBase_c::s_Create_Position;
/* 0x80575084 */ Vec3s* dAcBase_c::s_Create_Rotation;
/* 0x80575088 */ Vec3f* dAcBase_c::s_Create_Scale;
/* 0x8057508C */ dAcBase_c* dAcBase_c::s_Create_Parent;
/* 0x80575090 */ ObjInfoPtr* dAcBase_c::s_Create_ObjInfo;
/* 0x80575094 */ u8 dAcBase_c::s_Create_Subtype;

/* 8002c3a0 */
bool dAcBase_c::createHeap() {
    return true;
}

/* 8002c3b0 */
dAcBase_c::dAcBase_c() {}

/* 8002c530 */ // dont know how this got here
dBase_c::~dBase_c() {}

/* 8002c590 */
dAcBase_c::~dAcBase_c() {}

/* 8002c650 */
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

/* 8002c690 */
void* dAcBase_c::FUN_8002c690() {}

/* 8002c710 */
int dAcBase_c::initAllocatorWork1Heap(int size, char* name, int align) {
    initAllocator(size, name, dHeap::work1Heap, align);
}

extern "C" int fn_802EE510(mAllocator_c*, int size, EGG::Heap* heap, char* name, int align, int unk);

/* 8002c720 */
int dAcBase_c::initAllocator(int size, char* name, EGG::Heap* heap, int align) {
    if (fn_802EE510(&mHeapAllocator, size, heap, name, 0x20, 0) == 0){
        return 0;
    }
    soundRelated = FUN_8002c690();
    int success = createHeap();
    mHeapAllocator.adjustFrmHeapRestoreCurrent();
    return success;
}

/* 8002c7b0 */
bool dAcBase_c::addActorToRoom(s32 roomId) {}

/* 8002c840 */
void dAcBase_c::setBit_field_0xE8(s32) {}

/* 8002c860 */ // virt func, initiates the allocator
int dAcBase_c::actorCreate() {}

/* 8002c870 */ // virt func
int dAcBase_c::actorReCreate() {}

/* 8002c880 */
int dAcBase_c::create() {}

/* 8002c8f0 */
void dAcBase_c::postCreate(fBase_c::MAIN_STATE_e state) {}

/* 8002c940 */
int dAcBase_c::preDelete() {}

/* 8002cb10 */
int dAcBase_c::preExecute() {}

/* 8002cc10 */
int dAcBase_c::execute() {}

/* 8002cca0 */
int dAcBase_c::actorExecute() {}

/* 8002ccb0 */
int dAcBase_c::actorExecuteInEvent() {}

/* 8002ccc0 */
void dAcBase_c::postExecute(fBase_c::MAIN_STATE_e state) {}

/* 8002ce90 */
void dAcBase_c::unkVirtFunc_0x5C() { return; }

/* 8002cea0 */
void dAcBase_c::unkVirtFunc_0x60() { return; }

/* 8002ceb0 */
bool dAcBase_c::restorePosRotFromCopy() {}

/* 8002cf10 */
u32 dAcBase_c::itemDroppingAndGivingRelated(Vec3f* spawnPos, int subtype) {}

/* 8002cf90 */
void dAcBase_c::fillUpperParams2Byte() {}

/* 8002cfa0 */
u32 dAcBase_c::getParams2_ignoreLower() {}

/* 8002cfb0 */
void dAcBase_c::setParams2Upper_ignoreLower(u32 val) {}

/* 8002cfc0 */
u8 dAcBase_c::getParams2UpperByte() {}

/* 8002cfd0 */
void dAcBase_c::setParams2UpperByte(u32 val) {}

/* 8002cff0 */
u32 dAcBase_c::buildParams2(u32 lower, u8 upper) {}

/* 8002d010 */
u32 dAcBase_c::getParams2Lower() {}

/* 8002d020 */
dAcBase_c* dAcBase_c::findActor(char* objName, dAcBase_c* parent) {}

/* 8002d0a0 */ // searches for actor based on groupType
dAcBase_c* dAcBase_c::searchActor(dAcBase_c& optionalParent) {}

/* 8002d130 */
void dAcBase_c::forEachActor(void*, dAcBase_c&) {}

/* 8002d190 */
void dAcBase_c::getXZAngleToPlayer(s16&, dAcBase_c&) {}

/* 8002d1d0 */
bool dAcBase_c::getDistanceToActor(dAcBase_c& actor, f32 distThresh, f32* outDist) {}

/* 8002d290 */
bool dAcBase_c::getDistanceAndAngleToActor( \
    dAcBase_c& actor, f32 distThresh, s16 yAngle, s16 xAngle, \
    f32* outDist, s16* outDiffAngleY, s16* outDiffAngleX) 
{

}

/* 8002d3e0 */
bool dAcBase_c::isWithinPlayerRadius(f32 radius) {}

/* 8002d440 */
bool dAcBase_c::getDistanceAndAngleToPlayer( \
    f32 distThresh, s16 yAngle, s16 xAngle, \
    f32* outDist, s16* outDiffAngleY, s16* outDiffAngleX) 
{
    // return getDistanceAndAngleToActor(PLAYER, distThresh, yAngle, xAngle, outDist, outDiffAngleY, outDiffAngleX);
}

/* 8002d470 */
f32 dAcBase_c::getDistToPlayer() {}

/* 8002d4a0 */
f32 dAcBase_c::getSquareDistToPlayer() {}

/* 8002d4b0 */ 
void dAcBase_c::updateRoomId(f32 yOffs) {}

/* 8002d540 */ // currently named Global__isRoomFlags_6not0
bool dAcBase_c::isRoomFlags_0x6_Set() {}

/* 8002d590 */
void dAcBase_c::FUN_8002d590() {}

/* 8002d5b0 */
void dAcBase_c::FUN_8002d5b0() {}

/* 8002d5d0*/
void dAcBase_c::playSound() {}

/* 8002d600 */
void dAcBase_c::FUN_8002d600() {}

/* 8002d630 */
void dAcBase_c::FUN_8002d630() {}

/* 8002d6d0 */
void dAcBase_c::FUN_8002d6d0() {}

/* 8002d710 */ // currently named ActorBase__playSoundEffect1
void dAcBase_c::playSoundEffect1() {}

/* 8002d740 */
void dAcBase_c::FUN_8002d740() {}

/* 8002d770 */
void dAcBase_c::FUN_8002d770() {}

/* 8002d7a0 */
void dAcBase_c::FUN_8002d7a0() {}

/* 8002d7d0 */
void dAcBase_c::FUN_8002d7d0() {}

/* 8002d7f0 */
void dAcBase_c::FUN_8002d7f0() {}

/* 8002d810 */
void dAcBase_c::FUN_8002d810() {}

/* 8002d830 */
void dAcBase_c::FUN_8002d830() {}

/* 8002d860 */
void dAcBase_c::FUN_8002d860() {}

/* 8002d880 */
void dAcBase_c::FUN_8002d880() {}

/* 8002d890 */ // first param is not dAcBase_c
void dAcBase_c::FUN_8002d890() {}

/* 8002d920 */ // current name is Global__setActorRef
void dAcBase_c::setActorRef(dBase_c&) {}

/* 8002d930 */ // May not be only purpose
void dAcBase_c::setUnkFlag() {}

/* 8002d940 */
void dAcBase_c::FUN_8002d940() {}

/* 8002d960 */
void dAcBase_c::FUN_8002d960() {}

/* 8002d980 */ // spawns GroupType2 (ACTOR)
dAcBase_c dAcBase_c::createActor( \
    ProfileName actorId, u32 params1, \
    Vec3f* pos, Vec3s* rot, Vec3f* scale, \
    u32 params2, s32 roomId, dBase_c* ref) {}

/* 8002da80 */ // spawns GroupType3 (TG_FORCE_LINE ? )
dAcBase_c dAcBase_c::createActorUnkGroup3( \
    ProfileName actorId, u32 params1, \
    Vec3f* pos, Vec3s* rot, Vec3f* scale, \
    u32 params2, s32 roomId, dBase_c* ref) {}

/* 8002db80 */
void* dAcBase_c::getCurrentEventActor() {}

/* 8002db90 */
void dAcBase_c::unkVirtFunc_0x6C() {}

/* 8002dba0 */
void dAcBase_c::doInteraction(s32) {}

/* 8002dc20 */
void dAcBase_c::FUN_8002dc20(s16*, s16*) {}

/* 8002dc50 */
void dAcBase_c::incrementKillCounter() {}

/* 8002dcd0 */
void dAcBase_c::FUN_8002dcd0() {}

/* 8002dd10 */
void dAcBase_c::FUN_8002dd10() {}

/* 8002dd50 */
void dAcBase_c::FUN_8002dd50() {}

/* 8002dd90 */
void dAcBase_c::FUN_8002dd90() {}

/* 8002ddd0 */
void dAcBase_c::FUN_8002ddd0() {}

/* 8002de30 */
void dAcBase_c::FUN_8002de30() {}