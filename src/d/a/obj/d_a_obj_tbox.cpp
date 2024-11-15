#include "d/a/obj/d_a_obj_tbox.h"

#include "common.h"
#include "d/flag/storyflag_manager.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_resanmchr.h"
#include "nw4r/g3d/g3d_resanmtexpat.h"
#include "nw4r/g3d/g3d_resanmtexsrt.h"
#include "nw4r/g3d/g3d_resfile.h"
#include "nw4r/g3d/g3d_resmdl.h"
#include "toBeSorted/dowsing_target.h"

SPECIAL_ACTOR_PROFILE(TBOX, dAcTbox_c, fProfile::TBOX, 0x018D, 0, 6);

extern "C" mVec3_c vecs[] = {
    mVec3_c(0.0f, 54.0f, 0.0f), mVec3_c(0.0f, 34.0f, 0.0f), mVec3_c(0.0f, 66.5f, 0.0f), mVec3_c(0.0f, 49.0f, 0.0f)
};

extern "C" mVec3_c v1 = mVec3_c(0.0f, 0.0f, -2.0f);
extern "C" mVec3_c v2 = mVec3_c(1.0f, 1.0f, 0.8f);
extern "C" mVec3_c v3 = mVec3_c(1.0f, 1.0f, 1.0f);
extern "C" mVec3_c v4 = mVec3_c(0.0f, 0.0f, 88.0f);

STATE_DEFINE(dAcTbox_c, DugOut);
STATE_DEFINE(dAcTbox_c, WaitAppear);
STATE_DEFINE(dAcTbox_c, DemoAppear);
STATE_DEFINE(dAcTbox_c, WaitOpen);
STATE_DEFINE(dAcTbox_c, GoddessWaitOff);
STATE_DEFINE(dAcTbox_c, GoddessWaitOn);
STATE_DEFINE(dAcTbox_c, DeleteArchive);
STATE_DEFINE(dAcTbox_c, LoadArchive);
STATE_DEFINE(dAcTbox_c, Open);
STATE_DEFINE(dAcTbox_c, PresentItem);
STATE_DEFINE(dAcTbox_c, Close);
STATE_DEFINE(dAcTbox_c, Wait);
STATE_DEFINE(dAcTbox_c, GoddessWait);

fLiMgBa_c dAcTbox_c::sTboxActorList;

static char *const sArcNames[] = {
    "TBoxNormalT",
    "TBoxSmallT",
    "TBoxBossT",
    "GoddessTBox",
};

static const char *const sOpenArcName = "FX_TBoxOpen";
static const char *const sOpenMdlName = "FX_TBoxOpen";
static const char *const sOpenAnmChrName = "FX_TBoxOpen";
static const char *const sOpenAnmTexSrtName = "FX_TBoxOpen";
static const char *const sOpenAnmClrName = "FX_TBoxOpen";

static const char *const sAppearAnmName = "TBoxNormalT_appear";

extern "C" char *const sOpenEventNames[] = {
    "TreasureBoxOpen",
    "TreasureBoxOpenSmall",
    "TreasureBoxOpenBoss",
    "TreasureBoxOpen",
};

static char *const sMdlNames[] = {
    "TBoxNormalT",
    "TBoxSmallT",
    "TBoxBossT",
    "GoddessTBox",
};

static char *const sAnmNames[] = {
    "TBoxNormalT",
    "TBoxSmallT",
    "TBoxBossT",
    "TBoxNormalT",
};

static char *const sCloseCcNames[] = {
    "TBoxNormalTClose",
    "TBoxSmallTClose",
    "TBoxBossTClose",
    "GoddessTBoxClose",
};

static char *const sOpenCcNames[] = {
    "TBoxNormalTOpen",
    "TBoxSmallTOpen",
    "TBoxBossTOpen",
    "GoddessTBoxOpen",
};

dAcTbox_c::dAcTbox_c()
    : mStateMgr(*this, sStateID::null), mScnCallback(this), mEvent(*this, nullptr), mTboxListNode(this),
      mDowsingTarget1(this, DowsingTarget::SLOT_NONE), mDowsingTarget2(this, DowsingTarget::SLOT_NONE),
      field_0x4E8(-1) {
    sTboxActorList.append(&mTboxListNode);
    unkByteTargetFiRelated = 2;
}

dAcTbox_c::~dAcTbox_c() {
    sTboxActorList.remove(&mTboxListNode);
}

bool dAcTbox_c::createHeap() {
    void *data = getOarcResFile(sArcNames[mVariant]);
    if (data == nullptr) {
        return false;
    }
    if (!mMdl1.create(data, data, sMdlNames[mVariant], sAnmNames[mVariant], &heap_allocator, 0x32F, 1, nullptr)) {
        return false;
    }
    mMdl1.getModel().setLocalMtx(mWorldMtx);
    mScnCallback.attach(mMdl1.getModel());
    if (!initBgW(mBgWs[0], sArcNames[mVariant], sCloseCcNames[mVariant])) {
        return false;
    }
    if (!initBgW(mBgWs[1], sArcNames[mVariant], sOpenCcNames[mVariant])) {
        return false;
    }

    if (mVariant == 3) {
        if (data == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile res = data;
        nw4r::g3d::ResMdl mdl = mMdl1.getModel().getResMdl();
        if (!mdl.IsValid()) {
            return false;
        }
        nw4r::g3d::ResAnmTexPat anmTexPat = res.GetResAnmTexPat("GoddessTBox");
        if (!anmTexPat.mAnmTexPat.IsValid()) {
            return false;
        }
        if (!mAnmGoddessPat.create(mdl, anmTexPat, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mMdl1.getModel().setAnm(mAnmGoddessPat);
        u16 goddessTBoxActive = getParams2Lower();
        if (StoryflagManager::sInstance->getCounterOrFlag(goddessTBoxActive) && !mHasBeenOpened) {
            nw4r::g3d::ResAnmTexSrt anmTexSrt = res.GetResAnmTexSrt("GoddessTBox");
            if (!anmTexSrt.mAnmTexSrt.IsValid()) {
                return false;
            }
            if (!mAnmGoddessTexSrt.create(mdl, anmTexSrt, &heap_allocator, nullptr, 1)) {
                return false;
            }
            mMdl1.getModel().setAnm(mAnmGoddessTexSrt);
        }
    } else if (mVariant == 0) {
        if (data == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile res = data;
        nw4r::g3d::ResAnmClr anmClr = res.GetResAnmClr("TBoxNormalTAppear");
        if (!anmClr.mAnmClr.IsValid()) {
            return false;
        }
        nw4r::g3d::ResMdl mdl = mMdl1.getModel().getResMdl();
        if (!mdl.IsValid()) {
            return false;
        }
        if (!mAnmMatClr1.create(mdl, anmClr, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mAnmMatClr1.setRate(1.0f, 0);
        mMdl1.getModel().setAnm(mAnmMatClr1);
    }

    if (isNotSmall()) {
        void *fxData = getOarcResFile(sOpenArcName);
        if (fxData == nullptr) {
            return false;
        }
        nw4r::g3d::ResFile fxRes = fxData;
        if (!fxRes.mFile.IsValid()) {
            return false;
        }

        nw4r::g3d::ResMdl openMdl = fxRes.GetResMdl(sOpenMdlName);
        if (!openMdl.IsValid()) {
            return false;
        }
        if (!mOpenFxMdl.create(openMdl, &heap_allocator, 0x120)) {
            return false;
        }
        mOpenFxMdl.setPriorityDraw(0x7F, 0x86);

        nw4r::g3d::ResAnmChr openAnm = fxRes.GetResAnmChr(sOpenAnmChrName);
        if (!openAnm.mAnmChr.IsValid()) {
            return false;
        }
        if (!mAnmChr.create(openMdl, openAnm, &heap_allocator, nullptr)) {
            return false;
        }
        mOpenFxMdl.setAnm(mAnmChr);

        nw4r::g3d::ResAnmTexSrt anmTexSrt = fxRes.GetResAnmTexSrt(sOpenAnmTexSrtName);
        if (!anmTexSrt.mAnmTexSrt.IsValid()) {
            return false;
        }
        if (!mAnmTexSrt1.create(openMdl, anmTexSrt, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mOpenFxMdl.setAnm(mAnmTexSrt1);

        nw4r::g3d::ResAnmClr anmClr = fxRes.GetResAnmClr(sOpenAnmClrName);
        if (!anmClr.mAnmClr.IsValid()) {
            return false;
        }
        if (!mAnmMatClr2.create(openMdl, anmClr, &heap_allocator, nullptr, 1)) {
            return false;
        }
        mOpenFxMdl.setAnm(mAnmMatClr2);
        mVec3_c fxPos;
        fn_8026B380(fxPos);
        mMtx_c fxTransform;
        fxTransform.transS(fxPos);
        // TODO address calculations here
        fxTransform.ZXYrotM(rotation.x, rotation.y, rotation.y);
        mOpenFxMdl.setLocalMtx(mWorldMtx);
        mOpenFxMdl.setScale(fn_8026B3C0());
    }

    return true;
}

void dAcTbox_c::initializeState_DugOut() {}
void dAcTbox_c::executeState_DugOut() {}
void dAcTbox_c::finalizeState_DugOut() {}
void dAcTbox_c::initializeState_WaitAppear() {}
void dAcTbox_c::executeState_WaitAppear() {}
void dAcTbox_c::finalizeState_WaitAppear() {}
void dAcTbox_c::initializeState_DemoAppear() {}
void dAcTbox_c::executeState_DemoAppear() {}
void dAcTbox_c::finalizeState_DemoAppear() {}
void dAcTbox_c::initializeState_WaitOpen() {}
void dAcTbox_c::executeState_WaitOpen() {}
void dAcTbox_c::finalizeState_WaitOpen() {}
void dAcTbox_c::initializeState_GoddessWaitOff() {}
void dAcTbox_c::executeState_GoddessWaitOff() {}
void dAcTbox_c::finalizeState_GoddessWaitOff() {}
void dAcTbox_c::initializeState_GoddessWaitOn() {}
void dAcTbox_c::executeState_GoddessWaitOn() {}
void dAcTbox_c::finalizeState_GoddessWaitOn() {}
void dAcTbox_c::initializeState_DeleteArchive() {}
void dAcTbox_c::executeState_DeleteArchive() {}
void dAcTbox_c::finalizeState_DeleteArchive() {}
void dAcTbox_c::initializeState_LoadArchive() {}
void dAcTbox_c::executeState_LoadArchive() {}
void dAcTbox_c::finalizeState_LoadArchive() {}
void dAcTbox_c::initializeState_Open() {}
void dAcTbox_c::executeState_Open() {}
void dAcTbox_c::finalizeState_Open() {}
void dAcTbox_c::initializeState_PresentItem() {}
void dAcTbox_c::executeState_PresentItem() {}
void dAcTbox_c::finalizeState_PresentItem() {}
void dAcTbox_c::initializeState_Close() {}
void dAcTbox_c::executeState_Close() {}
void dAcTbox_c::finalizeState_Close() {}
void dAcTbox_c::initializeState_Wait() {}
void dAcTbox_c::executeState_Wait() {}
void dAcTbox_c::finalizeState_Wait() {}
void dAcTbox_c::initializeState_GoddessWait() {}
void dAcTbox_c::executeState_GoddessWait() {}
void dAcTbox_c::finalizeState_GoddessWait() {}

bool dAcTbox_c::isNotSmall() const {
    return mVariant != 1;
}
