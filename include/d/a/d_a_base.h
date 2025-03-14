#ifndef D_A_BASE_H
#define D_A_BASE_H

#include "UnknownTypeBelongings.h"
#include "d/d_base.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "toBeSorted/raii_ptr.h"
#include "toBeSorted/tlist.h"

class dAcBase_c;
struct cBgS_PolyInfo;

struct SoundSource {
    virtual ~SoundSource() {}
};

struct SoundInfo {
    dAcBase_c *actor;
    SoundSource *sound_source;
    mVec3_c *obj_pos;
    TListNode<SoundInfo> mLink;
};

/**
 * A list node that will automatically unlink upon destruction.
 */
class dAcRefBase_c : public fLiNdBa_c {
public:
    dAcRefBase_c(fBase_c *owner) : fLiNdBa_c(owner) {}
    ~dAcRefBase_c() {
        unlink();
    }
};

/**
 * A type-safe list node that can hold a specific actor reference.
 * Unlinks upon destruction. This setup is inferred from
 * double null checks in inline dtors and instantiated ctors/dtors
 * for arrays of these nodes in classes.
 */
template <typename T>
class dAcRef_c : dAcRefBase_c {
public:
    dAcRef_c(T *owner) : dAcRefBase_c(owner) {}
    dAcRef_c() : dAcRefBase_c(nullptr) {}
    ~dAcRef_c() {}

    void link(T *ref) {
        fLiNdBa_c::link(ref);
    }
    void unlink() {
        fLiNdBa_c::unlink();
    }
    T *get() {
        return static_cast<T *>(p_owner);
    }
    const T *get() const {
        return static_cast<const T *>(p_owner);
    }
    BOOL isLinked() const {
        return get() != nullptr;
    }
};

// Ghidra: ActorBase
//   size: 0xFC
// non-official name
class dAcBase_c : public dBase_c {
public:
    /* 0x68 */ mHeapAllocator_c heap_allocator;
    /* 0x84 */ ObjInfo *obj_info;
    /* 0x88 */ TList<SoundInfo, 12> sound_list;
    /* 0x94 */ RaiiPtr<SoundSource> sound_source;
    /* 0x98 */ mVec3_c *obj_pos;
    /* 0x9C */ mVec3_c pos_copy;
    /* 0xA8 */ u32 params2;
    /* 0xAC */ mAng3_c rot_copy;
    /* 0xB2 */ u16 obj_id; // enemydefeat flag / id on obj-map
    /* 0xB4 */ s8 room_id_copy;
    /* 0xB5 */ s8 viewclip_index;
    /* 0xB6 */ u8 subtype;
    /* 0xB8 */ mAng3_c rotation;
    /* 0xC0 */ mVec3_c position;
    /* 0xCC */ mVec3_c mScale;
    /* 0xD8 */ u32 actor_properties;
    /* 0xDC */ dAcRef_c<dAcBase_c> actor_node;
    /* 0xE8 */ u32 field_0xe8;
    /* 0xEC */ s8 roomid;
    /* 0xED */ u8 actor_subtype;
    /* 0xEE */ u8 polyAttr0;
    /* 0xEF */ u8 polyAttr1;
    /* 0xF0 */ u32 JStudio_actor;
    /* 0xF4 */ char someStr[4];
    /* 0xF8 */ char field_0xf8[0xfc - 0xf8];

public:
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
    /* 0x68 | 8002db80 */ virtual void registerInEvent();
    /* 0x6C | 8002db90 */ virtual void unkVirtFunc_0x6C();
    /* 0x70 | 8002dba0 */ virtual void doInteraction(s32);

public:
    /* 8002c3b0 */ dAcBase_c();

    void setPostion(const mVec3_c &r) {
        position = r;
    }
    void SetScale(const mVec3_c &r) {
        mScale = r;
    }
    void SetRotation(const mAng3_c &r) {
        rotation = r;
    }

    void copyPosition() {
        pos_copy = position;
    }
    void copyRotation() {
        rot_copy = rotation;
    }

    mVec3_c &GetPostion() {
        return position;
    }
    mAng3_c &GetRotation() {
        return rotation;
    }

    mVec3_c GetPostionDifference(const dAcBase_c &other) {
        return position - other.position;
    }

    f32 getSquareDistanceTo(const mVec3_c &point) const {
        mVec3_c diff = position - point;
        return diff.x * diff.x + diff.z * diff.z;
    }

    f32 getDistanceTo(const mVec3_c &to) const {
        return position.distance(to);
    }

    bool IsOutOfRange(const mVec3_c &point, f32 radius) {
        return getSquareDistanceTo(point) > radius;
    }

    u32 getRoomId() {
        return roomid;
    }

    void clearActorProperty(u32 property) {
        actor_properties &= ~property;
    }
    void setActorProperty(u32 property) {
        actor_properties |= property;
    }
    bool checkActorProperty(u32 property) const {
        return actor_properties & property;
    }

public:
    // funcs found in TU
    /* 8002c650 */ static void setTempCreateParams(
        mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, s32 roomId, u32 params2, dAcBase_c *parent, u8 subtype, u16 unkFlag,
        s8 viewClipIdx, ObjInfo *objInfo
    );

    /* 8002c690 */ SoundSource *FUN_8002c690();
    /* 8002c710 */ int initAllocatorWork1Heap(int size, char *name, int align);
    /* 8002c720 */ int initAllocator(int size, char *name, EGG::Heap *heap, int align);
    /* 8002c7b0 */ bool addActorToRoom(s32 roomId);
    /* 8002c840 */ void setBit_field_0xE8(s32);
    /* 8002cf10 */ u32 itemDroppingAndGivingRelated(mVec3_c *spawnPos, int subtype);
    /* 8002cf90 */ void fillUpperParams2Byte();
    /* 8002cfa0 */ u32 getParams2_ignoreLower();
    /* 8002cfb0 */ void setParams2Upper_ignoreLower(u32 val);
    /* 8002cfc0 */ int getParams2UpperByte();
    /* 8002cfd0 */ void setParams2UpperByte(u32 val);
    /* 8002cff0 */ static u32 buildParams2(u32 lower, u32 upper);
    /* 8002d010 */ u32 getParams2Lower() const;
    /* 8002d020 */ static dAcBase_c *findActor(char *objName, dAcBase_c *parent);
    /* 8002d0a0 */ static dAcBase_c *searchActor(dAcBase_c *parent);
    // Kinda performs the code of the first param on every actor (second param is optional parent)
    /* 8002d130 */ static void forEveryActor(void *func(dAcBase_c *, dAcBase_c *), dAcBase_c *parent);
    /* 8002d190 */ mAng getXZAngleToPlayer();
    // returns true if under the distThresh, False if not. the actual distance is returned in outDist
    /* 8002d1d0 */ bool getDistanceToActor(dAcBase_c *actor, f32 distThresh, f32 *outDist);
    // same concept as above
    /* 8002d290 */ bool getDistanceAndAngleToActor(
        dAcBase_c *actor, f32 distThresh, s16 yAngle, s16 xAngle, f32 *outDist, s16 *outDiffAngleY, s16 *outDiffAngleX
    );
    /* 8002d3e0 */ bool isWithinPlayerRadius(f32 radius) const;
    /* 8002d440 */ bool getDistanceAndAngleToPlayer(
        f32 distThresh, s16 yAngle, s16 xAngle, f32 *outDist, s16 *outDiffAngleY, s16 *outDiffAngleX
    );
    /* 8002d470 */ f32 getDistToPlayer();
    /* 8002d4a0 */ f32 getSquareDistToPlayer();
    /* 8002d4b0 */ void updateRoomId(f32 yOffs);
    /* 8002d540 */ bool isRoomFlags_0x6_Set();

    // Start of SoundSource stuff
    /* 8002d590 */ void FUN_8002d590();
    /* 8002d5b0 */ void FUN_8002d5b0();
    /* 8002d5d0 */ void playSound(u16 effect);
    /* 8002d600 */ void FUN_8002d600();
    /* 8002d630 */ void FUN_8002d630();
    /* 8002d6d0 */ void FUN_8002d6d0();
    /* 8002d710 */ void playSoundEffect1(u16 effect);
    /* 8002d740 */ void FUN_8002d740();
    /* 8002d770 */ void FUN_8002d770(u16, f32);
    /* 8002d7a0 */ void FUN_8002d7a0();
    /* 8002d7d0 */ void FUN_8002d7d0();
    /* 8002d7f0 */ void FUN_8002d7f0();
    /* 8002d810 */ void FUN_8002d810();
    /* 8002d830 */ void FUN_8002d830();
    /* 8002d860 */ void FUN_8002d860();
    /* 8002d880 */ SoundSource *getSoundSource();
    // End of SoundSource stuff

    /* 8002d890 */ void removeSoundInfo(SoundInfo *);
    /* 8002d920 */ void setActorRef(dAcBase_c *);
    // next three funcs are related
    /* 8002d930 */ void setEnemyDefeatFlag();
    /* 8002d940 */ void changeLoadedEntitiesWithSet();
    /* 8002d960 */ void changeLoadedEntitiesNoSet();

    /* 8002d980 */ dAcBase_c *createActor(
        ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, s32 roomId,
        dBase_c *ref
    );

    /* 8002da80 */ dAcBase_c *createActorStage(
        ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, s32 roomId,
        dBase_c *ref
    );

    /* 8002dc20 */ static void roundAngleToNearest90(s16 *, s16 *);
    /* 8002dc50 */ void incrementKillCounter();
    /* 8002dcd0 */ void killNoItemDrop();
    /* 8002dd10 */ void killWithFlag();
    /* 8002dd50 */ void killWithFlagNoItemDrop();
    /* 8002dd90 */ void deleteWithFlagNoItemDrop();
    /* 8002ddd0 */ void setPolyAttrs(cBgS_PolyInfo &p);
    /* 8002de30 */ void setPolyAttrsDupe(cBgS_PolyInfo &p);

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
