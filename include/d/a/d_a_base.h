#pragma once

#include <d/d_base.h>
#include <d/d_heap.h>
#include <m/mAllocator.h>
#include <UnknownTypeBelongings.h>
// Ghidra: ActorBase
//   size: 0xFC
// non-official name
class dAcBase_c : public dBase_c {
public:
    /* 0x68 */ mHeapAllocator mHeapAllocator; // mHeapAlloctor
    /* 0x84 */ ObjInfoPtr* objInfo; //
    /* 0x88 */ int field_0x88;
    /* 0x8C */ int field_0x8C;
    /* 0x90 */ int field_0x90;
    /* 0x94 */ void* soundRelated;
protected:
    /* 80501544 */ // vtable
    /* 0x08 */ virtual int create();
    /* 0x0C */ // virtual int preCreate(); // uses fBase_c::preCreate()
    /* 0x10 */ virtual void postCreate(MAIN_STATE_e state);
    /* 0x14 */ // virtual int doDelete(); / uses fBase_c::doDelete
    /* 0x18 */ virtual int preDelete();
    /* 0x1C */ // virtual void postDelete(MAIN_STATE_e state); // uses fBase_c::postDelete
    /* 0x20 */ virtual int execute(); // calls either actorExecute/executeInEvent
    /* 0x24 */ virtual int preExecute(); // looks at some actor properties
    /* 0x28 */ virtual void postExecute(MAIN_STATE_e state); // not fully understood
    /* 0x2C */ // virtual int draw(); // uses fBase_c::draw
    /* 0x30 */ // virtual int preDraw(); // uses dBase_c::preDraw
    /* 0x34 */ // virtual void postDraw(MAIN_STATE_e state); // uses dBase_c::postDraw
    /* 0x38 */ // virtual void deleteReady(); // uses fBase_c::deleteReady
    /* 0x3C */ // virtual bool entryFrmHeap(unsigned long size, EGG::Heap *parentHeap); // uses fBase_c::entryFrmHeap
    /* 0x40 */ // virtual bool entryFrmHeapNonAdjust(unsigned long size, EGG::Heap *parentHeap); // uses fBase_c::entryFrmHeapNonAdjust
    /* 0x44 */ virtual bool createHeap(); 
    /* 0x48 */ virtual ~dAcBase_c(); 
    // Start of dAcBase_c vtable additions (after dBase_c)
    /* 0x4C */ virtual int actorCreate(); // name is assumed
    /* 0x50 */ virtual int actorReCreate(); // name is assumed
    /* 0x54 */ virtual int actorExecute(); // name is assumed
    /* 0x58 */ virtual int actorExecuteInEvent(); // name is assumed
    /* 0x5C */ virtual void unkVirtFunc_0x5C();
    /* 0x60 */ virtual void unkVirtFunc_0x60();
    /* 0x64 */ virtual bool restorePosRotFromCopy();
    /* 0x68 */ virtual void* getCurrentEventActor();
    /* 0x6C */ virtual void unkVirtFunc_0x6C();
    /* 0x70 */ virtual void doInteraction(s32);

public:
    dAcBase_c();

public:
    // funcs found in TU 
    static void setTempCreateParams( \
        Vec3f* pos, Vec3s* rot, Vec3f* scale, \
        s32 roomId, u32 params2, dAcBase_c* parent, \
        u8 subtype, s16 unkFlag, u8 viewClipIdx,\
        ObjInfoPtr* objInfo );
    
    void* FUN_8002c690();
    int initAllocatorWork1Heap(int size, char* name);
    int initAllocator(int size, char* name, EGG::Heap* heap);
    bool addActorToRoom(s32 roomId);
    void setBit_field_0xE8(s32);
    u32 itemDroppingAndGivingRelated(Vec3f* spawnPos, int subtype);
    void fillUpperParams2Byte();
    u32 getParams2_ignoreLower();
    void setParams2Upper_ignoreLower(u32 val);
    u8 getParams2UpperByte();
    void setParams2UpperByte(u32 val);
    static u32 buildParams2(u32 lower, u8 upper);
    u32 getParams2Lower();
    static dAcBase_c* findActor(char* objName, dAcBase_c* parent);
    static dAcBase_c* searchActor(dAcBase_c& optionalParent);
    // Kinda performs the code of the first param on every actor (second param is optional parent)
    static void forEachActor(void*, dAcBase_c&);
    // Not really static, but we currently dont have a type for the return (not just simply a s16)
    static void getXZAngleToPlayer(s16&, dAcBase_c&);
    // returns true if under the distThresh, False if not. the actual distance is returned in outDist
    bool getDistanceToActor(dAcBase_c& actor, f32 distThresh, f32* outDist);
    // same concept as above
    bool getDistanceAndAngleToActor(dAcBase_c& actor, f32 distThresh, s16 yAngle, s16 xAngle, f32* outDist, s16* outDiffAngleY, s16* outDiffAngleX);
    bool isWithinPlayerRadius(f32 radius);
    bool getDistanceAndAngleToPlayer(f32 distThresh, s16 yAngle, s16 xAngle, f32* outDist, s16* outDiffAngleY, s16* outDiffAngleX);
    f32 getDistToPlayer();
    f32 getSquareDistToPlayer();
    void updateRoomId(f32 yOffs);
    bool isRoomFlags_0x6_Set();
    // Begin Sound Effect Related
    void FUN_8002d590();
    void FUN_8002d5b0();
    void playSound();
    void FUN_8002d600();
    void FUN_8002d630();
    void FUN_8002d6d0();
    void playSoundEffect1();
    void FUN_8002d740();
    void FUN_8002d770();
    void FUN_8002d7a0();
    void FUN_8002d7d0();
    void FUN_8002d7f0();
    void FUN_8002d810();
    void FUN_8002d830();
    void FUN_8002d860();
    void FUN_8002d880();
    // End Sound Effect Related
    void FUN_8002d890();
    void setActorRef(dBase_c&);
    // next three funcs are related
    void setUnkFlag();
    void FUN_8002d940();
    void FUN_8002d960();

    static dAcBase_c createActor(ProfileName actorId, u32 params1, Vec3f* pos, Vec3s* rot, \
        Vec3f* scale, u32 params2, s32 roomId, dBase_c* ref);

    static dAcBase_c createActorUnkGroup3(ProfileName actorId, u32 params1, Vec3f* pos, Vec3s* rot, \
        Vec3f* scale, u32 params2, s32 roomId, dBase_c* ref);

    void FUN_8002dc20(s16*, s16*);
    void incrementKillCounter();
    void FUN_8002dcd0();
    void FUN_8002dd10();
    void FUN_8002dd50();
    void FUN_8002dd90();
    void FUN_8002ddd0();
    void FUN_8002de30();

public:
    // static vars
    static u32 s_Create_RoomId;
    static u32 s_Create_Params2;
    static u16 s_Create_UnkFlags;
    static u8  s_Create_ViewClipIdx;
    static Vec3f* s_Create_Position;
    static Vec3s* s_Create_Rotation;
    static Vec3f* s_Create_Scale;
    static dAcBase_c* s_Create_Parent;
    static ObjInfoPtr* s_Create_ObjInfo;
    static u8 s_Create_Subtype;
};