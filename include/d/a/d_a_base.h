#ifndef D_A_BASE_H
#define D_A_BASE_H

#include "common.h"
#include "d/d_base.h"
#include "d/snd/d_snd_source_if.h"
#include "m/m_allocator.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "toBeSorted/actor_info.h"
#include "toBeSorted/raii_ptr.h"
#include "toBeSorted/sound_info.h"

struct cBgS_PolyInfo;

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
class dAcRef_c : public dAcRefBase_c {
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
    typedef TList<SoundInfo, 12> SoundInfoList;

    /* 0x68 */ mHeapAllocator_c heap_allocator;
    /* 0x84 */ const ActorInfo *mpActorInfo;
    /* 0x88 */ SoundInfoList sound_list;
    /* 0x94 */ RaiiPtr<dSoundSourceIf_c> mpSoundSource;
    /* 0x98 */ mVec3_c *obj_pos;
    /* 0x9C */ mVec3_c pos_copy;
    /* 0xA8 */ u32 params2;
    /* 0xAC */ mAng3_c rot_copy;
    /* 0xB2 */ u16 obj_id;
    /* 0xB4 */ s8 room_id_copy;
    /* 0xB5 */ s8 viewclip_index;
    /* 0xB6 */ u8 subtype;
    /* 0xB8 */ mAng3_c mRotation;
    /* 0xC0 */ mVec3_c position;
    /* 0xCC */ mVec3_c mScale;
    /* 0xD8 */ u32 actor_properties;
    /* 0xDC */ dAcRef_c<dAcBase_c> actor_node;
    /* 0xE8 */ u32 mTgSndAreaFlags;
    /* 0xEC */ s8 roomid;
    /* 0xED */ u8 actor_subtype;
    /* 0xEE */ u8 polyAttr0;
    /* 0xEF */ u8 polyAttr1;
    /* 0xF0 */ u32 JStudio_actor;
    /* 0xF4 */ char someStr[4];
    /* 0xF8 */ char field_0xf8[0xfc - 0xf8];

public:
    /* vt 0x08 */ virtual int create();
    /* vt 0x10 */ virtual void postCreate(MAIN_STATE_e state);
    /* vt 0x18 */ virtual int preDelete();
    /* vt 0x20 */ virtual int execute();
    /* vt 0x24 */ virtual int preExecute();
    /* vt 0x28 */ virtual void postExecute(MAIN_STATE_e state);
    /* vt 0x44 */ virtual bool createHeap();
    /* vt 0x48 */ virtual ~dAcBase_c();
    /* vt 0x4C */ virtual int actorCreate();
    /* vt 0x50 */ virtual int actorPostCreate();
    /* vt 0x54 */ virtual int actorExecute();
    /* vt 0x58 */ virtual int actorExecuteInEvent();
    /* vt 0x5C */ virtual void unkVirtFunc_0x5C();
    /* vt 0x60 */ virtual void unkVirtFunc_0x60();
    /* vt 0x64 */ virtual bool restorePosRotFromCopy();
    /* vt 0x68 */ virtual void registerInEvent();
    /* vt 0x6C */ virtual void unkVirtFunc_0x6C();
    /* vt 0x70 */ virtual void doInteraction(s32);

public:
    dAcBase_c();

    void setPosition(const mVec3_c &r) {
        position = r;
    }
    void setScale(const mVec3_c &r) {
        mScale = r;
    }
    void SetRotation(const mAng3_c &r) {
        mRotation = r;
    }

    void copyPosition() {
        pos_copy = position;
    }
    void copyRotation() {
        rot_copy = mRotation;
    }

    mVec3_c &GetPosition() {
        return position;
    }
    mAng3_c &GetRotation() {
        return mRotation;
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
        s8 viewClipIdx, const ActorInfo *actorInfo
    );

    /* 8002c690 */ dSoundSourceIf_c *createSoundSource();
    /* 8002c710 */ int initAllocatorWork1Heap(int size, char *name, int align);
    /* 8002c720 */ int initAllocator(int size, char *name, EGG::Heap *heap, int align);
    /* 8002c7b0 */ bool addActorToRoom(s32 roomId);
    /* 8002c840 */ void setTgSndAreaFlag(s32);
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
    /* 8002d590 */ void setSoundSourceSubtype(u8 subType);
    /* 8002d5b0 */ void FUN_8002d5b0();
    /* 8002d5d0 */ bool startSound(u32 soundId);
    /* 8002d600 */ bool startSoundWithFloatParam(u32 soundId, f32 param);
    /* 8002d630 */ bool startBgHitSound(u32 soundId, const cBgS_PolyInfo &info, const mVec3_c *position);
    /* 8002d6d0 */ bool startSoundAtPosition(u32 soundId, const mVec3_c *position);
    /* 8002d710 */ bool holdSound(u32 soundId);
    /* 8002d740 */ bool holdSoundWithIntParam(u32 soundId, s32 param);
    /* 8002d770 */ bool holdSoundWithFloatParam(u32 soundId, f32 param);
    /* 8002d7a0 */ bool holdSoundWithParams(u32 soundId, f32 fValue, s32 value);
    /* 8002d7d0 */ void holdSoundSourceFlag(u32 mask);
    /* 8002d7f0 */ void onSoundSourceFlag(u32 mask);
    /* 8002d810 */ void offSoundSourceFlag(u32 mask);
    /* 8002d830 */ bool isPlayingSound(u32 soundId);
    /* 8002d860 */ void setBattleBgmRelated(UNKWORD val);
    /* 8002d880 */ dSoundSourceIf_c *getSoundSource();
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
    /* 80575090 */ static const ActorInfo *s_Create_ActorInfo;
    /* 80575094 */ static u8 s_Create_Subtype;
};

#endif
