#ifndef D_A_BASE_H
#define D_A_BASE_H

#include "common.h"
#include "d/d_base.h"
#include "d/snd/d_snd_source_if.h"
#include "f/f_profile_name.h"
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

    dAcRef_c<T> *getPrev() const {
        return static_cast<dAcRef_c<T> *>(cListNd_c::getPrev());
    }

    dAcRef_c<T> *getNext() const {
        return static_cast<dAcRef_c<T> *>(cListNd_c::getNext());
    }
};

// Ghidra: ActorBase
//   size: 0xFC
// non-official name
class dAcBase_c : public dBase_c {
public:
    enum AcProperties_e {
        AC_PROP_0x1 = (1 << 0),
        AC_PROP_0x4 = (1 << 2),
        AC_PROP_0x100 = (1 << 8),
        AC_PROP_0x400 = (1 << 10),
        AC_PROP_0x800 = (1 << 11),
        AC_PROP_0x2000000 = (1 << 25),
        AC_PROP_0x4000000 = (1 << 26),
        AC_PROP_0x8000000 = (1 << 27),
        AC_PROP_0x10000000 = (1 << 28),
        AC_PROP_0x20000000 = (1 << 29),
        AC_PROP_0x40000000 = (1 << 30),
    };

public:
    typedef TList<SoundInfo, 12> SoundInfoList;

    /* 0x68 */ mHeapAllocator_c mAllocator;
    /* 0x84 */ const ActorInfo *mpActorInfo;
    /* 0x88 */ SoundInfoList mSoundList;
    /* 0x94 */ RaiiPtr<dSoundSourceIf_c> mpSoundSource;
    /* 0x98 */ mVec3_c *mpPosition;
    /* 0x9C */ mVec3_c mPositionCopy;
    /* 0xA8 */ u32 mParams2;
    /* 0xAC */ mAng3_c mRotationCopy;
    /* 0xB2 */ u16 mObjID;
    /* 0xB4 */ s8 mRoomIDCopy;
    /* 0xB5 */ s8 mViewClipIdx;
    /* 0xB6 */ u8 mSubtype;
    /* 0xB8 */ mAng3_c mRotation;
    /* 0xC0 */ mVec3_c mPosition;
    /* 0xCC */ mVec3_c mScale;
    /* 0xD8 */ u32 mActorProperties;
    /* 0xDC */ dAcRef_c<dAcBase_c> mActorNode;
    /* 0xE8 */ u32 mTgSndAreaFlags;
    /* 0xEC */ s8 mRoomID;
    /* 0xED */ u8 mActorSubtype;
    /* 0xEE */ u8 mPolyAttr0;
    /* 0xEF */ u8 mPolyAttr1;
    /* 0xF0 */ u32 mJStudioActor;
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
    /* vt 0x70 */ virtual void doInteraction(s32 /* InteractionType */);

public:
    dAcBase_c();

    void setSubtype(u8 sub) {
        mSubtype = sub;
    }
    void setPosition(const mVec3_c &r) {
        mPosition = r;
    }
    void setScale(const mVec3_c &r) {
        mScale = r;
    }
    void setRotation(const mAng3_c &r) {
        mRotation = r;
    }

    void copyPosition() {
        mPositionCopy = mPosition;
    }
    void copyRotation() {
        mRotationCopy = mRotation;
    }

    mVec3_c &getPosition() {
        return mPosition;
    }
    mAng3_c &getRotation() {
        return mRotation;
    }

    mVec3_c getPostionDifference(const dAcBase_c &other) const {
        return mPosition - other.mPosition;
    }

    f32 getHeightDifference(const dAcBase_c &b) const {
        return mPosition.y - b.mPosition.y;
    }

    f32 getSquareDistanceTo(const mVec3_c &point) const {
        mVec3_c diff = mPosition - point;
        return diff.x * diff.x + diff.z * diff.z;
    }

    f32 getDistanceTo(const mVec3_c &to) const {
        return mPosition.distance(to);
    }

    bool checkBeyondRadius(const mVec3_c &point, f32 radius) {
        return getSquareDistanceTo(point) > radius;
    }
    bool checkInRadius(const mVec3_c &point, f32 radius) {
        return getSquareDistanceTo(point) < radius;
    }

    u32 getRoomId() {
        return mRoomID;
    }

    bool isRoomID(s8 room) const {
        return mRoomID == room;
    }

    void unsetActorProperty(u32 property) {
        mActorProperties &= ~property;
    }
    void setActorProperty(u32 property) {
        mActorProperties |= property;
    }
    bool checkActorProperty(u32 property) const {
        return mActorProperties & property;
    }

    dAcBase_c *searchNextActor(dAcBase_c *parent) {
        return static_cast<dAcBase_c *>(fManager_c::searchBaseByGroupType(dAcBase_c::ACTOR, parent));
    }

public:
    static void setTempCreateParams(
        mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, s32 roomId, u32 params2, dAcBase_c *parent, u8 subtype, u16 unkFlag,
        s8 viewClipIdx, const ActorInfo *actorInfo
    );

    dSoundSourceIf_c *createSoundSource();
    int initAllocatorWork1Heap(int size, char *name, int align);
    int initAllocator(int size, char *name, EGG::Heap *heap, int align);
    bool addActorToRoom(s32 roomId);
    void setTgSndAreaFlag(s32);
    u32 itemDroppingAndGivingRelated(mVec3_c *spawnPos, int subtype);
    void fillUpperParams2Byte();
    u32 getParams2_ignoreLower();
    void setParams2Upper_ignoreLower(u32 val);
    int getParams2UpperByte();
    void setParams2UpperByte(u32 val);
    static u32 buildParams2(u32 lower, u32 upper);
    u32 getParams2Lower() const;
    static dAcBase_c *findActor(char *objName, dAcBase_c *parent);
    static dAcBase_c *searchActor(dAcBase_c *parent);
    // Kinda performs the code of the first param on every actor (second param is optional parent)
    static void forEveryActor(void *func(dAcBase_c *, dAcBase_c *), dAcBase_c *parent);
    mAng getXZAngleToPlayer() const;
    // returns true if under the distThresh, False if not. the actual distance is returned in outDist
    bool getDistanceToActor(dAcBase_c *actor, f32 distThresh, f32 *outDist);
    // same concept as above
    bool getDistanceAndAngleToActor(
        dAcBase_c *actor, f32 distThresh, s16 yAngle, s16 xAngle, f32 *outDist, s16 *outDiffAngleY, s16 *outDiffAngleX
    );
    bool isWithinPlayerRadius(f32 radius) const;
    bool getDistanceAndAngleToPlayer(
        f32 distThresh, s16 yAngle, s16 xAngle, f32 *outDist, s16 *outDiffAngleY, s16 *outDiffAngleX
    );
    f32 getDistToPlayer();
    f32 getSquareDistToPlayer();
    void updateRoomId(f32 yOffs);
    bool isRoomFlags_0x6_Set();

    // Start of SoundSource stuff
    void setSoundSourceSubtype(u8 subType);
    void FUN_8002d5b0();
    bool startSound(u32 soundId);
    bool startSoundWithFloatParam(u32 soundId, f32 param);
    bool startBgHitSound(u32 soundId, const cBgS_PolyInfo &info, const mVec3_c *position);
    bool startSoundAtPosition(u32 soundId, const mVec3_c *position);
    bool holdSound(u32 soundId);
    bool holdSoundWithIntParam(u32 soundId, s32 param);
    bool holdSoundWithFloatParam(u32 soundId, f32 param);
    bool holdSoundWithParams(u32 soundId, f32 fValue, s32 value);
    void holdSoundSourceFlag(u32 mask);
    void onSoundSourceFlag(u32 mask);
    void offSoundSourceFlag(u32 mask);
    bool isPlayingSound(u32 soundId);
    void setBattleBgmRelated(UNKWORD val);
    dSoundSourceIf_c *getSoundSource();
    // End of SoundSource stuff

    void removeSoundInfo(SoundInfo *);
    void setActorRef(dAcBase_c *);

    // next three funcs are related
    void setEnemyDefeatFlag();
    void changeLoadedEntitiesWithSet();
    void changeLoadedEntitiesNoSet();

    dAcBase_c *createActor(
        ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, s32 roomId,
        dBase_c *ref
    );

    dAcBase_c *createActorStage(
        ProfileName actorId, u32 params1, mVec3_c *pos, mAng3_c *rot, mVec3_c *scale, u32 params2, s32 roomId,
        dBase_c *ref
    );

    static void roundAngleToNearest90(s16 *, s16 *);
    void incrementKillCounter();
    void killNoItemDrop();
    void killWithFlag();
    void killWithFlagNoItemDrop();
    void deleteWithFlagNoItemDrop();
    void setPolyAttrs(cBgS_PolyInfo &p);
    void setPolyAttrsDupe(cBgS_PolyInfo &p);

public:
    static u32 s_Create_RoomId;
    static u32 s_Create_Params2;
    static u16 s_Create_UnkFlags;
    static u8 s_Create_ViewClipIdx;
    static mVec3_c *s_Create_Position;
    static mAng3_c *s_Create_Rotation;
    static mVec3_c *s_Create_Scale;
    static dAcBase_c *s_Create_Parent;
    static const ActorInfo *s_Create_ActorInfo;
    static u8 s_Create_Subtype;
};

#endif
