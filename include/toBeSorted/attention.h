#ifndef TOBESORTED_ATTENTION_H
#define TOBESORTED_ATTENTION_H

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_smdl.h"
#include "m/m_allocator.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/raii_ptr.h"

static const u32 OFF = 'off ';
static const u32 NONE = 'none';
static const u32 KEEP = 'keep';
static const u32 NEXT = 'next';
static const u32 ON = 'on  ';
static const u32 AWAY = 'away';

class InteractionMdl {
public:
    m3d::smdl_c mMdl;
    m3d::anmMatClr_c mAnmClr;
    m3d::anmTexPat_c mAnmTex;
    RaiiPtr<m3d::anmChr_c> mAnmChr;
    u8 field_0x78;
    u8 field_0x79;

    InteractionMdl() {}
    ~InteractionMdl() {}
};

enum InteractionType {
    UNK_0,
    PICK_UP,
    OPEN,
    UNK_3,
    TALK,
    EXAMINE_TALK,
    EXAMINE_6,
    UNK_7,
    GET_IN,
    READY_SWORD,
    UNK_10,
    UNK_11,
    READ,
    GRAB,
    CATCH,
    UNK_15,
    UNK_16,
    DIG,
    UNK_18,
    SIT,
    UNK_20,
};

class AttentionInfo {
public:
    AttentionInfo() {}
    ~AttentionInfo() {}

    /* 0x00 */ u8 unk;
    /* 0x01 */ u8 mActorIdx;
    /* 0x02 */ u8 mInteractionType;
    /* 0x03 */ u8 field_0x03;
    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ mVec3_c field_0x08;
};

class AttentionPool {
public:
    AttentionPool() {}
    ~AttentionPool() {}
    dAcRef_c<dAcObjBase_c> mRefs[8];
    AttentionInfo mInfos[8];
    s32 mNumUsedRefs;

    int fn_80096190(dAcObjBase_c *actor, u8 unk, InteractionType interactionType);
    bool insertTarget(
        dAcObjBase_c *actor, u32 unk1, mVec3_c *pos, InteractionType interactionType, u8 field_0x03, f32 field_0x04
    );

    dAcObjBase_c *getActor(s32 i) {
        // This only matches if the comparision is always true
        // Logic is hard sometimes I guess xD
        return i < mNumUsedRefs && mInfos[i].mActorIdx != -1 ? mRefs[mInfos[i].mActorIdx].get() : nullptr;
    }

    void clear() {
        for (int i = 0; i < 8; i++) {
            mInfos[i].unk = 0;
            mInfos[i].mActorIdx = -1;
            mInfos[i].field_0x04 = 0.0f;
            mInfos[i].field_0x03 = 0;
            mInfos[i].mInteractionType = 0;
            mRefs[i].unlink();
        }
        mNumUsedRefs = 0;
    }
};

class AttentionGroup {
public:
    AttentionGroup() {}
    ~AttentionGroup() {}

    AttentionPool mPools[2];
    u32 mWhichPool;

    AttentionPool *getOtherPool() {
        return &mPools[mWhichPool ^ 1];
    }

    AttentionPool *getPool() {
        return &mPools[mWhichPool];
    }

    void fn_800964B0();
};

struct InteractionTargetDef {
    s32 field_0x00;
    u32 field_0x04;
    u32 field_0x08;
    InteractionType interactType;
    u32 interactFlags;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2C;
};

// TODO this may include more things from AttentionManager, see AttentionManager's dtor
class InteractionModels {
public:
    /* 0x00 */ UNKWORD field_0xA5C;
    /* 0x04 */ s32 mCurrentTargetInfoIdx;
    /* 0x08 */ u32 mState;
    /* 0x0C */ nw4r::g3d::ResFile mResFile;
    /* 0x10 */ InteractionMdl mMdls[2];
};

class AttentionManager {
    mHeapAllocator_c mAllocator;
    /* 0x01C */ AttentionGroup mGroups[5];
    /* 0xA58 */ bool mTargeted;
    /* 0xA59 */ bool mHoldingZ;
    /* 0xA5A */ u8 field_0xA58[0xA5C - 0xA5A];
    /* 0xA5C */ InteractionModels mModels;
    /* 0xB64 */ u8 mHasTarget;
    /* 0xB65 */ u8 field_0xB65[0xB68 - 0xB65];
    /* 0xB68 */ UNKWORD field_0xB68;
    /* 0xB6C */ EffectsStruct mEffect1;
    /* 0xBA0 */ EffectsStruct mEffect2;
    /* 0xBD4 */ u8 field_0xBD4;
    /* 0xBD5 */ u8 field_0xBD5;

public:
    AttentionManager();
    /* vt at 0xBDC */ virtual ~AttentionManager();

    static AttentionManager *GetInstance() {
        return sInstance;
    }

private:
    /* 0xBDC */ u8 field_0xBDC;

    static AttentionManager *sInstance;

public:
    bool create();
    bool createHeap();
    bool execute();
    bool draw();

    void addTarget(dAcObjBase_c &actor, const InteractionTargetDef &def, u32, mVec3_c *);

    void addPickUpTarget(dAcObjBase_c &actor, f32 field_0x14);
    void addSitTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14);

    void addTalkTarget_unused(dAcObjBase_c &actor);

    void
    addExamineTalkTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20, f32 field_0x24);
    void addExamineTalkTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x20, f32 field_0x24);

    void addCatchTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg);

    void addCatchLikeTarget(
        dAcObjBase_c &actor, InteractionType interactionType, u32 flags, f32 field_0x14, f32 field_0x24_neg,
        f32 field_0x20_neg
    );

    void addUnk3Target(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg);

    void
    addUnk3Target(dAcObjBase_c &actor, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg);
    void addUnk3Target(
        dAcObjBase_c &actor, u32 arg2, mVec3_c *arg3, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg,
        f32 field_0x20_neg
    );

    void addNpcTalkTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20, f32 field_0x24);

    void
    addUnk7Target(dAcObjBase_c &actor, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg);

    void addReadTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20, f32 field_0x24);
    void addReadTarget2(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x20, f32 field_0x24);

    bool checkLink2() const;

private:
    bool isInNormalGameState() const;
    bool checkZButtonPressed() const;
    bool checkZButtonHeld() const;
    bool checkLink1() const;
    bool switchTarget(s32 target);
    bool playTargetAnim(s32 target);

    bool isZButtonPressed() const;
    bool isZButtonHeld() const;
    bool checkUnknown() const;

    bool collisionCheck0(const mVec3_c &a, const mVec3_c &b);
    bool collisionCheck1(const mVec3_c &a, const mVec3_c &b);
    bool collisionCheck2(const mVec3_c &a, const mVec3_c &b);

    bool fn_80096B40(dAcObjBase_c *actor);
    f32 targetScore(dAcObjBase_c *target, dAcObjBase_c *origin);
};

#endif
