#include <common.h>
#include <d/a/obj/d_a_obj_base.h>
#include <d/d_pad.h>
#include <m/m3d/m_anmchr.h>
#include <m/m3d/m_anmmatclr.h>
#include <m/m3d/m_anmtexpat.h>
#include <m/m3d/m_smdl.h>
#include <m/m_allocator.h>
#include <toBeSorted/arc_managers/oarc_manager.h>
#include <toBeSorted/event_manager.h>

static const u32 OFF = 'off ';
static const u32 NONE = 'none';
static const u32 KEEP = 'keep';
static const u32 NEXT = 'next';
static const u32 ON = 'on  ';
static const u32 AWAY = 'away';

mVec3_c attnVectors[4];

mVec3_c getPosCopy3(const dAcObjBase_c &actor) {
    return actor.poscopy3;
}

class SomeRaiiMaybe {
public:
    m3d::anmChr_c *mPtr;

    SomeRaiiMaybe() : mPtr(nullptr) {}
    ~SomeRaiiMaybe() {
        if (mPtr != nullptr) {
            delete mPtr;
            mPtr = nullptr;
        }
    }
};

class InteractionMdl {
public:
    m3d::smdl_c mMdl;
    m3d::anmMatClr_c mAnmClr;
    m3d::anmTexPat_c mAnmTex;
    SomeRaiiMaybe mAnmChr;
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
    bool insertTarget(dAcObjBase_c *actor, u32 unk1, mVec3_c *pos, InteractionType interactionType, u8 field_0x03,
            f32 field_0x04);

    dAcObjBase_c *getActor(s32 i) {
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

class EffectsStruct {
private:
    u8 field_0x00[0x1C - 0x00];

public:
    // vt at 0x1C
    EffectsStruct();
    EffectsStruct(dAcBase_c *);
    virtual ~EffectsStruct();

    inline void init(dAcBase_c *owner) {
        mpOwner = owner;
    }

private:
    u8 field_0x20[0x28 - 0x20];
    /* 0x28 */ dAcBase_c *mpOwner;
    u8 field_0x2C[0x34 - 0x2C];
};

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

private:
    /* 0xBDC */ u8 field_0xBDC;

public:
    bool create();
    bool createHeap();
    bool isInNormalGameState();
    bool checkZButtonPressed();
    bool checkZButtonHeld();
    bool checkLink1();
    bool checkLink2();
    bool execute();
    bool switchTarget(s32 target);
    bool playTargetAnim(s32 target);
    bool draw();

    bool isZButtonPressed();
    bool isZButtonHeld();
    bool checkUnknown();

    bool fn_80096B40(dAcObjBase_c *actor);

    void addTarget(dAcObjBase_c &actor, const InteractionTargetDef &def, u32, mVec3_c *);

    void addPickUpTarget(dAcObjBase_c &actor, f32 field_0x14);
    void addSitTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14);

    void addTalkTarget_unused(dAcObjBase_c &actor);

    void addExamineTalkTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20,
            f32 field_0x24);
    void addExamineTalkTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x20, f32 field_0x24);

    void addCatchTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg);

    void addCatchLikeTarget(dAcObjBase_c &actor, InteractionType interactionType, u32 flags, f32 field_0x14,
            f32 field_0x24_neg, f32 field_0x20_neg);

    void addUnk3Target(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg);

    void addUnk3Target(dAcObjBase_c &actor, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg,
            f32 field_0x20_neg);
    void addUnk3Target(dAcObjBase_c &actor, u32 arg2, mVec3_c *arg3, u32 flags, f32 arg5, f32 field_0x14,
            f32 field_0x24_neg, f32 field_0x20_neg);

    void addNpcTalkTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20, f32 field_0x24);

    void addUnk7Target(dAcObjBase_c &actor, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg,
            f32 field_0x20_neg);

    void addReadTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20, f32 field_0x24);
    void addReadTarget2(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x20, f32 field_0x24);

    static AttentionManager *sInstance;
};

class UnkAttnClass {
public:
    u8 field_0x00;
    UnkAttnClass();
    virtual ~UnkAttnClass() {}

    void init();
    void remove();

    u16 field_0x08;
    u16 field_0x0A;
    u16 field_0x0C;
    u16 field_0x0E;
    u16 field_0x10;
    u16 field_0x12;
    f32 field_0x14;
};

void UnkAttnClass::init() {

}

void UnkAttnClass::remove() {
    
}

UnkAttnClass::UnkAttnClass()
    : field_0x00(0), field_0x08(field_0x00 | 8), field_0x0A(300), field_0x0C(30), field_0x10(30), field_0x12(10),
      field_0x14(1.0f) {}

UnkAttnClass attnClass;

AttentionManager::AttentionManager() {
    sInstance = this;
}

AttentionManager::~AttentionManager() {
    attnClass.remove();
    sInstance = nullptr;
}

struct CursorStruct {
    const char *mMdlName;
    const char *mAnmClrName;
    const char *mAnmTexPatName;
    const char *anmChrName;
};

bool AttentionManager::create() {
    for (int i = 0; i < 5; i++) {
        mGroups[i].mWhichPool = 0;
        mGroups[i].getPool()->clear();
        mGroups[i].getOtherPool()->clear();
    }


    mModels.field_0xA5C = 0;
    mModels.mCurrentTargetInfoIdx = 0;
    mModels.mState = NONE;
    mTargeted = false;
    field_0xB68 = 0;
    attnClass.init();
    field_0xBDC = 0;
    field_0xBD5 = 0;
    field_0xBD4 = 0;

    if (!mAllocator.createNewTempFrmHeap(-1, dHeap::work1Heap.heap, "Attention", 0x20, 0)) {
        return 0;
    }
    mModels.mMdls[0].field_0x79 = 0;
    mModels.mMdls[1].field_0x79 = 0;
    mModels.mMdls[0].field_0x78 = 0;
    mModels.mMdls[1].field_0x78 = 0;
    bool success = createHeap();
    mAllocator.adjustFrmHeapRestoreCurrent();
    mHasTarget = 0;
    return success;
}

static const CursorStruct s_Cursors[2] = {{"ZcursorA", "ZcursorAInOut_00", "ZcursorALoop_00", nullptr},
        {"ZcursorB", "ZcursorBIn_00", "ZcursorBLoop_00", "ZcursorBIn_00"}};

bool AttentionManager::createHeap() {
    mModels.mResFile = OarcManager::sInstance->getMdlFromArc2("UI_Data");

    for (int i = 0; i < 2; i++) {
        InteractionMdl *iMdl = &mModels.mMdls[i];
        nw4r::g3d::ResMdl m = mModels.mResFile.GetResMdl(s_Cursors[i].mMdlName);
        if (!iMdl->mMdl.create(m, &mAllocator, 0, 1, nullptr)) {
            return false;
        }
        iMdl->mMdl.setPriorityDraw(0x7F, 0x90);
        nw4r::g3d::ResAnmClr anmClr = mModels.mResFile.GetResAnmClr(s_Cursors[i].mAnmClrName);
        if (!iMdl->mAnmClr.create(m, anmClr, &mAllocator, 0, 1)) {
            return false;
        }
        iMdl->mMdl.setAnm(iMdl->mAnmClr);
        nw4r::g3d::ResAnmTexPat anmTexPat = mModels.mResFile.GetResAnmTexPat(s_Cursors[i].mAnmTexPatName);
        if (!iMdl->mAnmTex.create(m, anmTexPat, &mAllocator, 0, 1)) {
            return false;
        }
        iMdl->mMdl.setAnm(iMdl->mAnmTex);
        if (s_Cursors[i].anmChrName != nullptr) {
            m3d::anmChr_c *anm = iMdl->mAnmChr.mPtr = new m3d::anmChr_c();
            if (!anm) {
                return false;
            }
            nw4r::g3d::ResAnmChr anmChr = mModels.mResFile.GetResAnmChr(s_Cursors[i].anmChrName);
            if (!iMdl->mAnmChr.mPtr->create(m, anmChr, &mAllocator, nullptr)) {
                return false;
            }
            iMdl->mMdl.setAnm(*iMdl->mAnmChr.mPtr);
        }
    }

    return true;
}

bool AttentionManager::isInNormalGameState() {
    return false;
}

bool AttentionManager::isZButtonPressed() {
    return dPad::checkButtonZPressed();
}

bool AttentionManager::isZButtonHeld() {
    return dPad::checkButtonZHeld();
}

bool AttentionManager::checkUnknown() {
    return false;
}

bool AttentionManager::checkLink1() {}

bool AttentionManager::checkLink2() {}

bool AttentionManager::execute() {
    field_0xBDC = 0;
    if (!isInNormalGameState() && !field_0xBD5) {
        if (!EventManager::isInEvent() && !EventManager::isInEventOtherThan7()) {
            return false;
        }

        mModels.mState = NONE;
        mTargeted = false;
        return false;
    }

    for (int i = 0; i < 5; i++) {
        if (i != 1) {
            mGroups[i].mWhichPool %= 2;
        }
    }

    switchTarget(1);
    playTargetAnim(1);
    return true;
}

void debugPrint8(const char *msg, ...);

bool AttentionManager::switchTarget(s32 idx) {
    if (field_0xBD5) {
        mGroups[idx].getPool()->clear();
        return true;
    } else {
        if (!checkLink1()) {
            mGroups[idx].mWhichPool = 0;
            mGroups[idx].getPool()->clear();
            mGroups[idx].getOtherPool()->clear();
        } else {
            bool zPressed = checkZButtonPressed();
            mHoldingZ = checkZButtonHeld();
            AttentionPool *left = mGroups[idx].getPool();
            AttentionPool *right = mGroups[idx].getOtherPool();
            if ((attnClass.field_0x08 & 8) != 0 && (mModels.mState == OFF || mModels.mState == NONE) && checkLink2() && right->mNumUsedRefs != 0) {
                zPressed = true;
            }

            if (checkUnknown() && mModels.mState != AWAY) {
                mModels.mState = OFF;
                mTargeted = false;
            }
        }
    }


    debugPrint8("%4.4s -> %4.4s (%c pos=%d, cnt=%d)");
}

void debugPrint8(const char *msg, ...) {}

bool AttentionManager::playTargetAnim(s32 idx) {
    if (mModels.mMdls[1].field_0x79) {
        mModels.mMdls[1].mMdl.play();
        mModels.mMdls[1].mAnmTex.play();
        if (mModels.mMdls[1].mAnmChr.mPtr != nullptr) {
            if (mModels.mMdls[1].mAnmChr.mPtr->isStop()) {
                nw4r::g3d::ResAnmChr anmChr = mModels.mResFile.GetResAnmChr("ZcursorBLoop_00");
                mModels.mMdls[1].mAnmChr.mPtr->setAnm(mModels.mMdls[1].mMdl, anmChr, m3d::PLAY_MODE_4);
                mModels.mMdls[1].mMdl.setAnm(*mModels.mMdls[1].mAnmChr.mPtr);
            } else {
                mModels.mMdls[1].mAnmChr.mPtr->play();
            }
        }
    }

    if (mModels.mMdls[0].field_0x79) {
        mModels.mMdls[0].mMdl.play();
        mModels.mMdls[0].mAnmTex.play();
        if (mModels.mMdls[0].field_0x78 && mModels.mMdls[0].mAnmChr.mPtr->isStop()) {
            nw4r::g3d::ResAnmClr anmClr = mModels.mResFile.GetResAnmClr("ZcursorALoop_00");
            mModels.mMdls[0].mAnmClr.setAnm(mModels.mMdls[1].mMdl, anmClr, 0, m3d::PLAY_MODE_4);
        }
    }

    bool doSomething = false;

    dAcObjBase_c *actor = mGroups[idx].getOtherPool()->getActor(mModels.mCurrentTargetInfoIdx);
    if (actor != nullptr) {
        AttentionPool *pool = mGroups[idx].getOtherPool();
        dAcObjBase_c *actor = pool->getActor(mModels.mCurrentTargetInfoIdx);
        mMtx_c mtx;
        mVec3_c v = pool->mInfos[mModels.mCurrentTargetInfoIdx].field_0x08;
        PSMTXTrans(mtx, v.x, v.y, v.z);

        mModels.mMdls->mMdl.setLocalMtx(mtx);
        mModels.mMdls->mMdl.setScale(attnClass.field_0x14, attnClass.field_0x14, attnClass.field_0x14);
        doSomething = true;
    }

    if (mModels.mState != NONE) {}

    // TODO this is just temporarily here for data segments
    nw4r::g3d::ResAnmClr anmClr = mModels.mResFile.GetResAnmClr("ZcursorBIn_00");
    anmClr = mModels.mResFile.GetResAnmClr("ZcursorBOut_00");

    if (doSomething) {
        if (mModels.mMdls[0].field_0x79 == 0 || mModels.mMdls[0].field_0x78 != 0) {
            nw4r::g3d::ResAnmClr anmClr = mModels.mResFile.GetResAnmClr("ZcursorAInOut_00");
            mModels.mMdls[0].mAnmClr.setAnm(mModels.mMdls[0].mMdl, anmClr, 0, m3d::PLAY_MODE_4);
            mModels.mMdls[0].field_0x78 = 0;
        }
        mModels.mMdls[0].field_0x79 = 1;
    } else if (mModels.mMdls[0].field_0x79 != 0) {
        if (mModels.mMdls[0].field_0x78 != 0) {
            if (mModels.mMdls[0].mAnmClr.isStop(0)) {
                mModels.mMdls[0].field_0x79 = 0;
            }
        } else {
            nw4r::g3d::ResAnmClr anmClr = mModels.mResFile.GetResAnmClr("ZcursorAInOut_00");
            mModels.mMdls[0].mAnmClr.setAnm(mModels.mMdls[0].mMdl, anmClr, 0, m3d::PLAY_MODE_3);
            mModels.mMdls[0].field_0x78 = 1;
        }
    }

    return true;
}

bool AttentionManager::draw() {
    if (mHasTarget || !(isInNormalGameState() || field_0xBD5)) {
        mHasTarget = 0;
        mModels.mMdls[0].field_0x79 = 0;
        mModels.mMdls[1].field_0x79 = 0;
        return true;
    } else {
        mHasTarget = 0;
        if (mModels.mMdls[1].field_0x79 && !field_0xBD4) {
            mModels.mMdls[1].mMdl.entry();
        }
        if (mModels.mMdls[0].field_0x79 != 0) {
            mModels.mMdls[0].mMdl.entry();
        }
        return true;
    }
}

void AttentionManager::addTarget(dAcObjBase_c &, const InteractionTargetDef &, u32, mVec3_c *) {}

int AttentionPool::fn_80096190(dAcObjBase_c *actor, u8 arg, InteractionType arg2) {
    s32 interactType = (s32)arg2 != UNK_16 ? arg2 : -1;

    for (int i = 0; i < mNumUsedRefs; i++) {
        s32 bVar4 = i < mNumUsedRefs ? mInfos[i].mInteractionType : UNK_0;

        s32 IVar4 = bVar4 == UNK_16 ? interactType : i < mNumUsedRefs ? mInfos[i].mInteractionType : UNK_0;

        dAcObjBase_c *existingActor = getActor(i);
        if (existingActor == actor) {
            s32 unk;
            if (i < mNumUsedRefs) {
                unk = mInfos[i].unk;
            } else {
                unk = -1;
            }
            if (arg == unk && (IVar4 == interactType || interactType == -1)) {
                return i;
            }
        }
    }

    return -1;
}

bool AttentionPool::insertTarget(dAcObjBase_c *actor, u32 unk1, mVec3_c *pos, InteractionType interactionType,
        u8 field_0x03, f32 field_0x04) {
    // Bunch of regshuffles
    for (s32 i = 0; i < 8; i++) {
        if (i >= mNumUsedRefs) {
            // Free slot, just use that
            mRefs[mNumUsedRefs].link(actor);
            mInfos[i].unk = unk1;
            mInfos[i].mActorIdx = i;
            mInfos[i].field_0x04 = field_0x04;
            mInfos[i].mInteractionType = interactionType;
            mInfos[i].field_0x08 = *pos;
            mInfos[i].field_0x03 = field_0x03;
            mNumUsedRefs++;
            break;
        }

        if (field_0x04 > mInfos[i].field_0x04) {
            s32 idx = mNumUsedRefs;
            if (mNumUsedRefs < 8) {
                mNumUsedRefs = mNumUsedRefs + 1;
            } else {
                s32 oldSlot = mNumUsedRefs - 1;
                idx = mInfos[oldSlot].mActorIdx;
                mRefs[mInfos[oldSlot].mActorIdx].unlink();
            }

            s32 y = mNumUsedRefs - 1;
            s32 end = y;
            s32 target = y;
            s32 x = target;
            // For some reason the comparison happens in reverse here lol
            for (s32 j = i; j < end; j++) {
                s32 vec_idx = y - 1;
                mInfos[target].unk = mInfos[x - 1].unk;
                s32 actorIdx = mInfos[x - 1].mActorIdx;
                mInfos[target].mActorIdx = mInfos[x - 1].mActorIdx;
                mInfos[target].mInteractionType = mInfos[x - 1].mInteractionType;
                mInfos[target].field_0x03 = mInfos[x - 1].field_0x03;
                mInfos[target].field_0x04 = mInfos[x - 1].field_0x04;
                mInfos[target].field_0x08 = mInfos[vec_idx].field_0x08;

                if (actorIdx != -1 && (actor != mRefs[actorIdx].get() || (s32)unk1 != mInfos[target].unk)) {
                    target--;
                }

                x--;
                y--;
            }

            mRefs[idx].link(actor);
            mInfos[i].mActorIdx = i;
            mInfos[i].unk = unk1;
            mInfos[i].field_0x04 = field_0x04;
            mInfos[i].mInteractionType = interactionType;
            mInfos[i].field_0x08 = *pos;
            mInfos[i].field_0x03 = field_0x03;
            break;
        }

        if (actor == mRefs[mInfos[i].mActorIdx].get() && (s32)unk1 == mInfos[i].unk) {
            // Actor and unk already present, bail
            return false;
        }
    }
    return true;
}

void AttentionGroup::fn_800964B0() {
    AttentionPool *left = &mPools[mWhichPool];
    AttentionPool *right = &mPools[mWhichPool ^ 1];
    for (int i = 0; i < left->mNumUsedRefs; i++) {
        // This effectively copies a position vector from "left" to "right",
        // where left and right are swapped sometimes. So this is effectively
        // a double-buffered object pool?
        int target = right->fn_80096190(left->mRefs[left->mInfos[i].mActorIdx].get(), left->mInfos[i].unk,
                (InteractionType)left->mInfos[i].mInteractionType);
        if (target != -1) {
            right->mInfos[target].field_0x08 = left->mInfos[i].field_0x08;
        }
    }
}

// Maybe checks if actor is currently targeted
bool AttentionManager::fn_80096B40(dAcObjBase_c *actor) {
    AttentionPool *refs = &mGroups[1].mPools[mGroups[1].mWhichPool ^ 1];
    dAcObjBase_c *ac = refs->getActor(mModels.mCurrentTargetInfoIdx);
    if (ac == actor) {
        mHasTarget = 1;
    }
    return true;
}

void AttentionManager::addPickUpTarget(dAcObjBase_c &actor, f32 field_0x14) {
    static InteractionTargetDef pickUpTarget = {0, 2, 1, PICK_UP, 0x100, 0.0f, 60.0f, 0.0f, -50.0f, 70.0f, 20.0f, 1.0f};
    pickUpTarget.field_0x14 = field_0x14;
    addTarget(actor, pickUpTarget, 0, nullptr);
}

void AttentionManager::addSitTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14) {
    static InteractionTargetDef sitTarget = {0, 2, 0, SIT, 2, 0.0f, 60.0f, 0.0f, -75.0f, 75.0f, 20.0f, 1.0f};
    sitTarget.interactFlags = flags;
    sitTarget.field_0x14 = field_0x14;
    addTarget(actor, sitTarget, 0, nullptr);
}

void AttentionManager::addTalkTarget_unused(dAcObjBase_c &actor) {
    static InteractionTargetDef talkTarget = {0, 3, 0, TALK, 0, 0.0f, 45.0f, 45.0f, 0.0f, 0.0f, 50.0f, 1.0f};

    addTarget(actor, talkTarget, 0, nullptr);
}

void AttentionManager::addExamineTalkTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored,
        f32 field_0x20, f32 field_0x24) {
    static InteractionTargetDef examineTalkTarget = {1, 3, 0, EXAMINE_TALK, 0, 0.0f, 60.0f, 60.0f, 0.0f, 0.0f, 50.0f,
            1.0f};
    examineTalkTarget.interactFlags = flags;
    examineTalkTarget.field_0x14 = field_0x14;
    examineTalkTarget.field_0x20 = field_0x20;
    examineTalkTarget.field_0x24 = field_0x24;

    addTarget(actor, examineTalkTarget, 0, nullptr);
}

void AttentionManager::addExamineTalkTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x20,
        f32 field_0x24) {
    static InteractionTargetDef examineTalkTarget2 = {0, 3, 0, EXAMINE_TALK, 0, 0.0f, 60.0f, 60.0f, 0.0f, 0.0f, 50.0f,
            1.0f};
    examineTalkTarget2.interactFlags = flags;
    examineTalkTarget2.field_0x14 = field_0x14;
    examineTalkTarget2.field_0x20 = field_0x20;
    examineTalkTarget2.field_0x24 = field_0x24;

    addTarget(actor, examineTalkTarget2, 0, nullptr);
}

void AttentionManager::addCatchTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x24_neg,
        f32 field_0x20_neg) {
    static InteractionTargetDef catchTarget = {3, 2, 1, CATCH, 0, 0.0f, 60.0f, 0.0f, 0.0f, 0.0f, 100.0f, 1.0f};
    catchTarget.interactFlags = flags;
    catchTarget.field_0x14 = field_0x14;
    catchTarget.field_0x20 = -field_0x20_neg;
    catchTarget.field_0x24 = -field_0x24_neg;

    addTarget(actor, catchTarget, 0, nullptr);
}

void AttentionManager::addCatchLikeTarget(dAcObjBase_c &actor, InteractionType interactionType, u32 flags,
        f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg) {
    static InteractionTargetDef catchTarget2 = {4, 2, 1, CATCH, 0, 0.0f, 60.0f, 0.0f, 0.0f, 0.0f, 100.0f, 1.0f};

    catchTarget2.interactFlags = flags;
    catchTarget2.interactType = interactionType;
    catchTarget2.field_0x14 = field_0x14;
    catchTarget2.field_0x20 = -field_0x20_neg;
    catchTarget2.field_0x24 = -field_0x24_neg;

    addTarget(actor, catchTarget2, 0, nullptr);
}

void AttentionManager::addUnk3Target(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x24_neg,
        f32 field_0x20_neg) {
    static InteractionTargetDef unk3Target = {2, 2, 1, UNK_3, 0, 0.0f, 90.0f, 0.0f, 0.0f, 0.0f, 50.0f, 1.0f};

    unk3Target.interactFlags = flags;
    unk3Target.field_0x14 = field_0x14;
    unk3Target.field_0x20 = -field_0x20_neg;
    unk3Target.field_0x24 = -field_0x24_neg;

    addTarget(actor, unk3Target, 0, nullptr);
}

void AttentionManager::addUnk3Target(dAcObjBase_c &actor, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg,
        f32 field_0x20_neg) {
    addUnk3Target(actor, 0, nullptr, flags, arg5, field_0x14, field_0x24_neg, field_0x20_neg);
}

void AttentionManager::addUnk3Target(dAcObjBase_c &actor, u32 arg2, mVec3_c *arg3, u32 flags, f32 arg5,
        f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg) {
    static InteractionTargetDef unk3Target2 = {1, 1, 1, UNK_3, 0, 0.0f, 60.0f, 60.0f, 0.0f, 0.0f, 50.0f, 1.2f};
    unk3Target2.interactFlags = flags;
    unk3Target2.field_0x18 = arg5 - field_0x14;
    unk3Target2.field_0x14 = field_0x14;
    unk3Target2.field_0x20 = -field_0x20_neg;
    unk3Target2.field_0x24 = -field_0x24_neg;

    addTarget(actor, unk3Target2, arg2, arg3);
}

void AttentionManager::addNpcTalkTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20,
        f32 field_0x24) {
    static InteractionTargetDef talkTarget = {1, 3, 0, TALK, 0, 0.0f, 45.0f, 45.0f, 0.0f, 0.0f, 50.0f, 1.0f};
    talkTarget.interactFlags = flags;
    talkTarget.field_0x14 = field_0x14;
    talkTarget.field_0x20 = field_0x20;
    talkTarget.field_0x24 = field_0x24;

    addTarget(actor, talkTarget, 0, nullptr);
}

void AttentionManager::addUnk7Target(dAcObjBase_c &actor, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg,
        f32 field_0x20_neg) {
    static InteractionTargetDef unk7Target = {1, 1, 1, UNK_7, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 50.0f, 0.5f};
    unk7Target.interactFlags = flags;
    unk7Target.field_0x18 = arg5 - field_0x14;
    unk7Target.field_0x14 = field_0x14;
    unk7Target.field_0x20 = -field_0x20_neg;
    unk7Target.field_0x24 = -field_0x24_neg;

    addTarget(actor, unk7Target, 0, nullptr);
}

void AttentionManager::addReadTarget(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20,
        f32 field_0x24) {
    static InteractionTargetDef readTarget = {1, 3, 0, READ, 0, 0.0f, 60.0f, 60.0f, 0.0f, 0.0f, 50.0f, 1.0f};
    readTarget.interactFlags = flags;
    readTarget.field_0x14 = field_0x14;
    readTarget.field_0x20 = field_0x20;
    readTarget.field_0x24 = field_0x24;

    addTarget(actor, readTarget, 0, nullptr);
}

void AttentionManager::addReadTarget2(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x20,
        f32 field_0x24) {
    static InteractionTargetDef readTarget2 = {0, 3, 0, READ, 0, 0.0f, 60.0f, 60.0f, 0.0f, 0.0f, 50.0f, 1.0f};
    readTarget2.interactFlags = flags;
    readTarget2.field_0x14 = field_0x14;
    readTarget2.field_0x20 = field_0x20;
    readTarget2.field_0x24 = field_0x24;

    addTarget(actor, readTarget2, 0, nullptr);
}
