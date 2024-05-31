#ifndef D_A_BASE_H
#define D_A_BASE_H

#include "UnknownTypeBelongings.h"
#include "d/d_base.h"
#include "d/d_heap.h"
#include "m/m_allocator.h"

// #include "m/types_m.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "toBeSorted/room_manager.h"

class dAcBase_c;

struct SoundSource {
    // TODO
};

struct SoundInfo {
    dAcBase_c *actor;
    SoundSource *sound_source;
    mVec3_c *obj_pos;
    SoundInfo *next;
    SoundInfo *prev;
};

template <typename T, int offset>
struct TList {
    T *GetOffset() {
        return (T *)((u8 *)this - offset);
    }
    TList() {
        sound_info_next = GetOffset();
        sound_info_tail = GetOffset();
        count = 0;
    }
    T *sound_info_tail;
    T *sound_info_next;
    s32 count;
};

// Ghidra: ActorBase
//   size: 0xFC
// non-official name
class dAcBase_c : public dBase_c {
public:
    /* 0x68 */ mHeapAllocator_c heap_allocator;
    /* 0x84 */ ObjInfo *obj_info;
    /* 0x88 */ TList<SoundInfo, 0xC> sound_list;
    /* 0x94 */ SoundSource *sound_source;
    /* 0x9C */ mVec3_c *obj_pos;
    /* 0x9c */ mVec3_c pos_copy;
    /* 0xa8 */ u32 params2;
    /* 0xAC */ mAng3_c rot_copy;
    /* 0xB2 */ u16 obj_id; // enemydefeat flag / id on obj-map
    /* 0xB4 */ s8 room_id_copy;
    /* 0xB5 */ u8 viewclip_index;
    /* 0xB6 */ u8 subtype;
    /* 0xB8 */ mAng3_c rotation;
    /* 0xC0 */ mVec3_c position;
    /* 0xCC */ mVec3_c scale;
    /* 0xD8 */ u32 actor_properties;
    /* 0xDC */ fLiNdBa_c actor_node;
    /* 0xE8 */ u32 field_0xe8;
    /* 0xEC */ s8 roomid;
    /* 0xED */ u8 actor_subtype;
    /* 0xF0 */ u32 JStudio_actor;
    /* 0xF4 */ char someStr[4];
    /* 0xF8 */ char field_0xf8[0xfc - 0xf8];

protected:
    /* 80501544 */ // vtable
    /* 0x08 | 8002c880 */ virtual int create();
    /* 0x10 | 8002c8f0 */ virtual void postCreate(MAIN_STATE_e state);
    /* 0x18 | 8002c940 */ virtual int preDelete();
    /* 0x20 | 8002cc10 */ virtual int execute();                        // calls either actorExecute/executeInEvent
    /* 0x24 | 8002cb10 */ virtual int preExecute();                     // looks at some actor properties
    /* 0x28 | 8002ccc0 */ virtual void postExecute(MAIN_STATE_e state); // not fully understood
    /* 0x44 | 8002c3a0 */ virtual bool createHeap();
    /* 0x48 | 8002c590 */ virtual ~dAcBase_c();
    /* 0x4C | 8002c860 */ virtual int actorCreate();         // name is assumed
    /* 0x50 | 8002c870 */ virtual int actorPostCreate();     // name is assumed
    /* 0x54 | 8002cca0 */ virtual int actorExecute();        // name is assumed
    /* 0x58 | 8002ccb0 */ virtual int actorExecuteInEvent(); // name is assumed
    /* 0x5C | 8002ce90 */ virtual void unkVirtFunc_0x5C();
    /* 0x60 | 8002cea0 */ virtual void unkVirtFunc_0x60();
    /* 0x64 | 8002ceb0 */ virtual bool restorePosRotFromCopy();
    /* 0x68 | 8002db80 */ virtual void *getCurrentEventActor();
    /* 0x6C | 8002db90 */ virtual void unkVirtFunc_0x6C();
    /* 0x70 | 8002dba0 */ virtual void doInteraction(int);

public:
    /* 8002c3b0 */ dAcBase_c();

    void setPostion(mVec3_c &r) {
        position = r;
    }
    void SetScale(mVec3_c &r) {
        scale = r;
    }
    void SetRotation(mAng3_c &r) {
        rotation = r;
    }

    void copyPosition() {
        pos_copy = position;
    }

    const mVec3_c &GetPostion() const {
        return position;
    }
    void copyRotation() {
        rot_copy = rotation;
    }
    mVec3_c GetPostionDifference(const dAcBase_c *other) {
        return position - other->position;
    }

public:
    // funcs found in TU
    /* 8002c650 */ static void setTempCreateParams(mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, s32 roomId, u32 params2,
            dAcBase_c *parent, u8 subtype, s16 unkFlag, u8 viewClipIdx, ObjInfo *objInfo);

    /* 8002c690 */ SoundSource *FUN_8002c690();
    /* 8002c710 */ int initAllocatorWork1Heap(int size, char *name, int align);
    /* 8002c720 */ int initAllocator(int size, char *name, EGG::Heap *heap, int align);
    /* 8002c7b0 */ bool addActorToRoom(s32 roomId);
    /* 8002c840 */ void setBit_field_0xE8(s32);
    /* 8002cf10 */ u32 itemDroppingAndGivingRelated(mVec3_c *spawnPos, int subtype);
    /* 8002cf90 */ void fillUpperParams2Byte();
    /* 8002cfa0 */ u32 getParams2_ignoreLower();
    /* 8002cfb0 */ void setParams2Upper_ignoreLower(u32 val);
    /* 8002cfc0 */ u8 getParams2UpperByte();
    /* 8002cfd0 */ void setParams2UpperByte(u32 val);
    /* 8002cff0 */ static u32 buildParams2(u32 lower, u32 upper);
    /* 8002d010 */ u32 getParams2Lower();
    /* 8002d020 */ static dAcBase_c *findActor(char *objName, dAcBase_c *parent);
    /* 8002d0a0 */ static dAcBase_c *searchActor(dAcBase_c *parent);
    // Kinda performs the code of the first param on every actor (second param is optional parent)
    /* 8002d130 */ static void forEveryActor(void *func(dAcBase_c *, dAcBase_c *), dAcBase_c *parent);
    // Not really static, but we currently dont have a type for the return (not just simply a s16)
    /* 8002d190 */ mAng getXZAngleToPlayer(s16 *angle);
    // returns true if under the distThresh, False if not. the actual distance is returned in outDist
    /* 8002d1d0 */ bool getDistanceToActor(dAcBase_c *actor, f32 distThresh, f32 *outDist);
    // same concept as above
    /* 8002d290 */ bool getDistanceAndAngleToActor(dAcBase_c *actor, f32 distThresh, s16 yAngle, s16 xAngle,
            f32 *outDist, s16 *outDiffAngleY, s16 *outDiffAngleX);
    /* 8002d3e0 */ bool isWithinPlayerRadius(f32 radius);
    /* 8002d440 */ bool getDistanceAndAngleToPlayer(f32 distThresh, s16 yAngle, s16 xAngle, f32 *outDist,
            s16 *outDiffAngleY, s16 *outDiffAngleX);
    /* 8002d470 */ f32 getDistToPlayer();
    /* 8002d4a0 */ f32 getSquareDistToPlayer();
    /* 8002d4b0 */ void updateRoomId(f32 yOffs);
    /* 8002d540 */ bool isRoomFlags_0x6_Set();

    // Start of SoundSource stuff
    /* 8002d590 */ void FUN_8002d590();
    /* 8002d5b0 */ void FUN_8002d5b0();
    /* 8002d5d0 */ void playSound();
    /* 8002d600 */ void FUN_8002d600();
    /* 8002d630 */ void FUN_8002d630();
    /* 8002d6d0 */ void FUN_8002d6d0();
    /* 8002d710 */ void playSoundEffect1();
    /* 8002d740 */ void FUN_8002d740();
    /* 8002d770 */ void FUN_8002d770();
    /* 8002d7a0 */ void FUN_8002d7a0();
    /* 8002d7d0 */ void FUN_8002d7d0();
    /* 8002d7f0 */ void FUN_8002d7f0();
    /* 8002d810 */ void FUN_8002d810();
    /* 8002d830 */ void FUN_8002d830();
    /* 8002d860 */ void FUN_8002d860();
    /* 8002d880 */ SoundSource *getSoundSource();
    // End of SoundSource stuff

    /* 8002d890 */ void FUN_8002d890();
    /* 8002d920 */ void setActorRef(dBase_c *);
    // next three funcs are related
    /* 8002d930 */ void setEnemyDefeatFlag();
    /* 8002d940 */ void changeLoadedEntitiesWithSet();
    /* 8002d960 */ void changeLoadedEntitiesNoSet();

    /* 8002d980 */ dAcBase_c *createActor(ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale,
            u32 params2, s32 roomId, dBase_c *ref);

    /* 8002da80 */ dAcBase_c *createActorStage(ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot,
            mVec3_c *scale, u32 params2, s32 roomId, dBase_c *ref);

    /* 8002dc20 */ void FUN_8002dc20(s16 *, s16 *);
    /* 8002dc50 */ void incrementKillCounter();
    /* 8002dcd0 */ void FUN_8002dcd0();
    /* 8002dd10 */ void FUN_8002dd10();
    /* 8002dd50 */ void FUN_8002dd50();
    /* 8002dd90 */ void FUN_8002dd90();
    /* 8002ddd0 */ void FUN_8002ddd0();
    /* 8002de30 */ void FUN_8002de30();

public:
    /* 80571920 */ static u32 s_Create_RoomId;
    /* 80571924 */ static u32 s_Create_Params2;
    /* 80571928 */ static u16 s_Create_UnkFlags;
    /* 8057192A */ static u8 s_Create_ViewClipIdx;
    /* 80575080 */ static mVec3_c *s_Create_Position;
    /* 80575084 */ static mAng3_c *s_Create_Rotation;
    /* 80575088 */ static mVec3_c *s_Create_Scale;
    /* 8057508C */ static dAcBase_c *s_Create_Parent;
    /* 80575090 */ static ObjInfo *s_Create_ObjInfo;
    /* 80575094 */ static u8 s_Create_Subtype;
};

#endif
