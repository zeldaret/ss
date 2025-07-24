#include "toBeSorted/attention.h"

#include "c/c_lib.h"
#include "d/a/d_a_player.h"
#include "d/d_heap.h"
#include "d/d_pad.h"
#include "m/m_angle.h"
#include "m/m_heap.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "toBeSorted/arc_managers/oarc_manager.h"
#include "toBeSorted/event_manager.h"

mVec3_c attnVectors[4];

mVec3_c getPosCopy3(const dAcObjBase_c &actor) {
    return actor.poscopy3;
}

class dAttention_HIO_c {
public:
    u8 field_0x00;
    dAttention_HIO_c();
    virtual ~dAttention_HIO_c() {}

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

void dAttention_HIO_c::init() {}

void dAttention_HIO_c::remove() {}

dAttention_HIO_c::dAttention_HIO_c()
    : field_0x00(0),
      field_0x08(field_0x00 | 8),
      field_0x0A(300),
      field_0x0C(30),
      field_0x10(30),
      field_0x12(10),
      field_0x14(1.0f) {}

dAttention_HIO_c sHio;

AttentionManager::AttentionManager() {
    sInstance = this;
}

AttentionManager::~AttentionManager() {
    sHio.remove();
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
    sHio.init();
    field_0xBDC = 0;
    field_0xBD5 = 0;
    field_0xBD4 = 0;

    if (!mAllocator.createFrmHeapToCurrent(-1, dHeap::work1Heap.heap, "Attention", 0x20, mHeap::OPT_NONE)) {
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

static const CursorStruct s_Cursors[2] = {
    {"ZcursorA", "ZcursorAInOut_00", "ZcursorALoop_00",         nullptr},
    {"ZcursorB",    "ZcursorBIn_00", "ZcursorBLoop_00", "ZcursorBIn_00"}
};

bool AttentionManager::createHeap() {
    mModels.mResFile = nw4r::g3d::ResFile(OarcManager::GetInstance()->getMdlFromArc2("UI_Data"));

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

bool AttentionManager::isInNormalGameState() const {
    return false;
}

bool AttentionManager::isZButtonPressed() const {
    return dPad::checkButtonZPressed();
}

bool AttentionManager::isZButtonHeld() const {
    return dPad::checkButtonZHeld();
}

bool AttentionManager::checkUnknown() const {
    return false;
}

bool AttentionManager::checkLink1() const {
    dAcPy_c *link = dAcPy_c::LINK;
    if (!isInNormalGameState() || !link->checkFlags0x350(0x40000)) {
        return false;
    }
    return true;
}

bool AttentionManager::checkLink2() const {}

bool AttentionManager::execute() {
    field_0xBDC = 0;
    if (!isInNormalGameState() && !field_0xBD5) {
        if (!EventManager::isInEvent() || !EventManager::isInEventOtherThan0Or7()) {
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
            if ((sHio.field_0x08 & 8) != 0 && (mModels.mState == OFF || mModels.mState == NONE) && checkLink2() &&
                right->mNumUsedRefs != 0) {
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
        mModels.mMdls->mMdl.setScale(sHio.field_0x14, sHio.field_0x14, sHio.field_0x14);
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

bool AttentionPool::insertTarget(
    dAcObjBase_c *actor, u32 unk1, mVec3_c *pos, InteractionType interactionType, u8 field_0x03, f32 field_0x04
) {
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
    AttentionPool *left = getPool();
    AttentionPool *right = getOtherPool();
    for (int i = 0; i < left->mNumUsedRefs; i++) {
        // This effectively copies a position vector from "left" to "right",
        // where left and right are swapped sometimes. So this is effectively
        // a double-buffered object pool?
        int target = right->fn_80096190(
            left->mRefs[left->mInfos[i].mActorIdx].get(), left->mInfos[i].unk,
            (InteractionType)left->mInfos[i].mInteractionType
        );
        if (target != -1) {
            right->mInfos[target].field_0x08 = left->mInfos[i].field_0x08;
        }
    }
}

extern "C" f32 lbl_8057CD9C;

f32 AttentionManager::targetScore(dAcObjBase_c *target, dAcObjBase_c *origin) {
    s16 angle = cLib::targetAngleY(target->position, origin->position) - origin->rotation.y.mVal;
    f32 viewAngle = fabsf(angle * lbl_8057CD9C);
    if (viewAngle > 0.5f) {
        return viewAngle;
    }
    return viewAngle * 0.5f + 0.25f;
}

// Maybe checks if actor is currently targeted
bool AttentionManager::fn_80096B40(dAcObjBase_c *actor) {
    AttentionPool *refs = mGroups[1].getOtherPool();
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

void AttentionManager::addExamineTalkTarget(
    dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20, f32 field_0x24
) {
    static InteractionTargetDef examineTalkTarget = {1,     3,     0,    EXAMINE_TALK, 0,     0.0f,
                                                     60.0f, 60.0f, 0.0f, 0.0f,         50.0f, 1.0f};
    examineTalkTarget.interactFlags = flags;
    examineTalkTarget.field_0x14 = field_0x14;
    examineTalkTarget.field_0x20 = field_0x20;
    examineTalkTarget.field_0x24 = field_0x24;

    addTarget(actor, examineTalkTarget, 0, nullptr);
}

void AttentionManager::addExamineTalkTarget(
    dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x20, f32 field_0x24
) {
    static InteractionTargetDef examineTalkTarget2 = {0,     3,     0,    EXAMINE_TALK, 0,     0.0f,
                                                      60.0f, 60.0f, 0.0f, 0.0f,         50.0f, 1.0f};
    examineTalkTarget2.interactFlags = flags;
    examineTalkTarget2.field_0x14 = field_0x14;
    examineTalkTarget2.field_0x20 = field_0x20;
    examineTalkTarget2.field_0x24 = field_0x24;

    addTarget(actor, examineTalkTarget2, 0, nullptr);
}

void AttentionManager::addCatchTarget(
    dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg
) {
    static InteractionTargetDef catchTarget = {3, 2, 1, CATCH, 0, 0.0f, 60.0f, 0.0f, 0.0f, 0.0f, 100.0f, 1.0f};
    catchTarget.interactFlags = flags;
    catchTarget.field_0x14 = field_0x14;
    catchTarget.field_0x20 = -field_0x20_neg;
    catchTarget.field_0x24 = -field_0x24_neg;

    addTarget(actor, catchTarget, 0, nullptr);
}

void AttentionManager::addCatchLikeTarget(
    dAcObjBase_c &actor, InteractionType interactionType, u32 flags, f32 field_0x14, f32 field_0x24_neg,
    f32 field_0x20_neg
) {
    static InteractionTargetDef catchTarget2 = {4, 2, 1, CATCH, 0, 0.0f, 60.0f, 0.0f, 0.0f, 0.0f, 100.0f, 1.0f};

    catchTarget2.interactFlags = flags;
    catchTarget2.interactType = interactionType;
    catchTarget2.field_0x14 = field_0x14;
    catchTarget2.field_0x20 = -field_0x20_neg;
    catchTarget2.field_0x24 = -field_0x24_neg;

    addTarget(actor, catchTarget2, 0, nullptr);
}

void AttentionManager::addUnk3Target(
    dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg
) {
    static InteractionTargetDef unk3Target = {2, 2, 1, UNK_3, 0, 0.0f, 90.0f, 0.0f, 0.0f, 0.0f, 50.0f, 1.0f};

    unk3Target.interactFlags = flags;
    unk3Target.field_0x14 = field_0x14;
    unk3Target.field_0x20 = -field_0x20_neg;
    unk3Target.field_0x24 = -field_0x24_neg;

    addTarget(actor, unk3Target, 0, nullptr);
}

void AttentionManager::addUnk3Target(
    dAcObjBase_c &actor, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg
) {
    addUnk3Target(actor, 0, nullptr, flags, arg5, field_0x14, field_0x24_neg, field_0x20_neg);
}

void AttentionManager::addUnk3Target(
    dAcObjBase_c &actor, u32 arg2, mVec3_c *arg3, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg,
    f32 field_0x20_neg
) {
    static InteractionTargetDef unk3Target2 = {1, 1, 1, UNK_3, 0, 0.0f, 60.0f, 60.0f, 0.0f, 0.0f, 50.0f, 1.2f};
    unk3Target2.interactFlags = flags;
    unk3Target2.field_0x18 = arg5 - field_0x14;
    unk3Target2.field_0x14 = field_0x14;
    unk3Target2.field_0x20 = -field_0x20_neg;
    unk3Target2.field_0x24 = -field_0x24_neg;

    addTarget(actor, unk3Target2, arg2, arg3);
}

void AttentionManager::addNpcTalkTarget(
    dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20, f32 field_0x24
) {
    static InteractionTargetDef talkTarget = {1, 3, 0, TALK, 0, 0.0f, 45.0f, 45.0f, 0.0f, 0.0f, 50.0f, 1.0f};
    talkTarget.interactFlags = flags;
    talkTarget.field_0x14 = field_0x14;
    talkTarget.field_0x20 = field_0x20;
    talkTarget.field_0x24 = field_0x24;

    addTarget(actor, talkTarget, 0, nullptr);
}

void AttentionManager::addUnk7Target(
    dAcObjBase_c &actor, u32 flags, f32 arg5, f32 field_0x14, f32 field_0x24_neg, f32 field_0x20_neg
) {
    static InteractionTargetDef unk7Target = {1, 1, 1, UNK_7, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 50.0f, 0.5f};
    unk7Target.interactFlags = flags;
    unk7Target.field_0x18 = arg5 - field_0x14;
    unk7Target.field_0x14 = field_0x14;
    unk7Target.field_0x20 = -field_0x20_neg;
    unk7Target.field_0x24 = -field_0x24_neg;

    addTarget(actor, unk7Target, 0, nullptr);
}

void AttentionManager::addReadTarget(
    dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 ignored, f32 field_0x20, f32 field_0x24
) {
    static InteractionTargetDef readTarget = {1, 3, 0, READ, 0, 0.0f, 60.0f, 60.0f, 0.0f, 0.0f, 50.0f, 1.0f};
    readTarget.interactFlags = flags;
    readTarget.field_0x14 = field_0x14;
    readTarget.field_0x20 = field_0x20;
    readTarget.field_0x24 = field_0x24;

    addTarget(actor, readTarget, 0, nullptr);
}

void AttentionManager::addReadTarget2(dAcObjBase_c &actor, u32 flags, f32 field_0x14, f32 field_0x20, f32 field_0x24) {
    static InteractionTargetDef readTarget2 = {0, 3, 0, READ, 0, 0.0f, 60.0f, 60.0f, 0.0f, 0.0f, 50.0f, 1.0f};
    readTarget2.interactFlags = flags;
    readTarget2.field_0x14 = field_0x14;
    readTarget2.field_0x20 = field_0x20;
    readTarget2.field_0x24 = field_0x24;

    addTarget(actor, readTarget2, 0, nullptr);
}
