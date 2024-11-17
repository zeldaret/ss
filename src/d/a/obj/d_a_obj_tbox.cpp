#include "d/a/obj/d_a_obj_tbox.h"

#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/col/cc/d_cc_d.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "m/m3d/m_fanm.h"
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

// clang-format off
// TODO just copied from somewhere
const cCcD_SrcGObj dAcTbox_c::sColSrc = {
  /* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
  /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x1080111, {6, 0x407}, 0, 0},
  /* mObjCo */ {0x0}
};
// clang-format on

// TODO enum, item comments
static const u8 sItemToTBoxVariant[MAX_ITEM_ID + 1] = {
    0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 2, 2, 2, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 3, 3, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
      mDowsingTarget1(this, DowsingTarget::SLOT_NONE), mDowsingTarget2(this, DowsingTarget::SLOT_NONE) {
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
    if (!mMdl1.create(data, sMdlNames[mVariant], sAnmNames[mVariant], &heap_allocator, 0x32F, 1, nullptr)) {
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

    if (mVariant == GODDESS) {
        nw4r::g3d::ResFile res = data;
        if (!res.mFile.IsValid()) {
            return false;
        }
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
    } else if (mVariant == NORMAL) {
        nw4r::g3d::ResFile res = data;
        if (!res.mFile.IsValid()) {
            return false;
        }
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
        if (!mOpenFxMdl.create(openMdl, &heap_allocator, 0x120, 1, nullptr)) {
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
        // TODO address calculations here, otherwise this function
        // matches surprisingly well
        fxTransform.ZXYrotM(rotation.x, rotation.y, rotation.z);
        mOpenFxMdl.setLocalMtx(fxTransform);
        mOpenFxMdl.setScale(fn_8026B3C0());
    }

    return true;
}

static u32 sSomeCounters[16] = {};

int dAcTbox_c::create() {
    if (!isActualVisibleBox()) {
        return FAILED;
    }
    int roomId_tmp = roomid;
    if (addActorToRoom(-1)) {
        roomid = roomId_tmp;
        changeLoadedEntitiesWithSet();
    }
    setItemId((ITEM_ID)(rotation.z & 0x1FF));
    if (mItemId > MAX_ITEM_ID) {
        return FAILED;
    }
    mVariant = sItemToTBoxVariant[mItemId];
    if (mVariant == GODDESS) {
        setItemId((ITEM_ID)(MAX_ITEM_ID - mItemId));
    }
    mSpawnSceneFlag = (params >> 0x14) & 0xFF;
    mSetSceneFlag = rotation.x & 0xFF;
    setChestFlag();
    field_0x120F = ((rotation.x >> 8) & 1) == 0;
    if (!noObstructionCheck()) {
        setDoObstructionCheck();
    }
    field_0x1208 = (rotation.x >> 0xA) & 0xF;
    switch (fn_8026B370()) {
        case 0:  field_0x120A = 0; break;
        case 1:  field_0x120A = 1; break;
        case 3:  field_0x120A = 3; break;
        default: field_0x120A = 3; break;
    }
    rotation.z = 0;
    rotation.x = 0;

    // This part of the code checks if there's another chest with similar properties
    // and only keeps one of them.
    if (fn_80268660(field_0x1208) && !field_0x1210) {
        sSomeCounters[field_0x1208]++;
        field_0x1210 = true;
    }
    field_0x120D = 0;
    if (fn_8026D670()) {
        u32 counterValue = 0;
        getSomeCounter(&counterValue);
        if (counterValue > 1) {
            bool keepGoing = true;
            dAcTbox_c *other;
            dAcObjBase_c *cursor = nullptr;
            do {
                cursor = getNextObject(&sTboxActorList, cursor);
                other = static_cast<dAcTbox_c *>(cursor);
                if (other != nullptr && this != other && !other->field_0x120D && field_0x1208 == other->field_0x1208) {
                    keepGoing = false;
                }
            } while (keepGoing && cursor != nullptr);

            bool b;
            if (checkTboxFlag()) {
                b = true;
            } else if (other->checkTboxFlag()) {
                b = false;
            } else {
                b = fn_8026D560();
            }

            if (b) {
                other->deleteRequest();
                field_0x120D = true;
            } else {
                field_0x120D = true;
                return FAILED;
            }
        }
    }

    if (checkTboxFlag() ||
        (mSetSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mSetSceneFlag))) {
        mHasBeenOpened = true;
    } else {
        mHasBeenOpened = false;
        if (isItemRupee()) {
            initDowsingTarget(DowsingTarget::SLOT_RUPEE);
        } else if (dAcItem_c::isKeyPiece((ITEM_ID)mItemId)) {
            initDowsingTarget(DowsingTarget::SLOT_QUEST);
        } else if (dAcItem_c::isTreasure((ITEM_ID)mItemId)) {
            initDowsingTarget(DowsingTarget::SLOT_TREASURE);
        }

        if (mVariant == GODDESS && StoryflagManager::sInstance->getCounterOrFlag(getParams2Lower())) {
            initDowsingTargetCube();
        }
    }

    if (isItemRupee() || dAcItem_c::isTreasure((ITEM_ID)mItemId)) {
        mRegisterDowsingTarget = &dAcTbox_c::registerRupeeOrTreasureDowsing;
        mUnregisterDowsingTarget = &dAcTbox_c::unregisterDowsing;
    } else if (dAcItem_c::isKeyPiece((ITEM_ID)mItemId)) {
        mRegisterDowsingTarget = &dAcTbox_c::registerKeyPieceDowsing;
        mUnregisterDowsingTarget = &dAcTbox_c::noUnregisterDowsing;
    } else {
        mRegisterDowsingTarget = &dAcTbox_c::noRegisterDowsing;
        mUnregisterDowsingTarget = &dAcTbox_c::noUnregisterDowsing;
    }

    updateMatrix();

    CREATE_ALLOCATOR(dAcTbox_c);

    mStts.SetRank(0xD);
    static const dCcD_SrcUnk s1 = {
        sColSrc,
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    };
    mCcD3.Set(s1);
    mCcD4.SetStts(mStts);
    static const dCcD_SrcCyl s2 = {
        sColSrc,
        {1.0f, 1.0f},
    };
    mCcD4.Set(s2);
    mCcD4.SetStts(mStts);
    if (mVariant == SMALL) {
        // clang-format off
        static const dCcD_SrcUnk s3 = {{
            /* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
            /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x1080111, {6, 0x407}, 0, 0},
            /* mObjCo */ {0x0}},
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
        };
        // clang-format on
        field_0x0D48.addCc(mCcD1, s3);
        field_0x0D48.addCc(mCcD2, s3);
    } else {
        // clang-format off
        static const dCcD_SrcUnk s4 = {{
            /* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
            /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x80000 | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x1080111, {6, 0x407}, 0, 0},
            /* mObjCo */ {0x0}},
            {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
        };
        // clang-format on
        field_0x0D48.addCc(mCcD1, s4);
        field_0x0D48.addCc(mCcD2, s4);
    }
    field_0x0D48.SetStts(mStts);
    // TODO figure out the right fields
    mCcD1.SetTgType(-1);
    mCcD2.SetTgType(-1);
    mMdl1.setAnm(sAnmNames[mVariant], m3d::PLAY_MODE_4);
    if (mHasBeenOpened == true) {
        mMdl1.setFrame(mMdl1.getAnm().getEndFrame());
    } else {
        mMdl1.setFrame(mMdl1.getAnm().getStartFrame());
    }
    mMdl1.getModel().calc(false);
    mMtx_c nodeMtx;
    mMdl1.getModel().getNodeWorldMtx(mMdl1.getModel().getNodeID("Cover"), nodeMtx);

    nodeMtx.getTranslation(field_0x11D8);
    if (mVariant == GODDESS) {
        if (!mHasBeenOpened && StoryflagManager::sInstance->getCounterOrFlag(getParams2Lower())) {
            mAnmGoddessPat.setFrame(1.0f, 0);
        } else {
            mAnmGoddessPat.setFrame(0.0f, 0);
        }
    }

    field_0x11F0 = 0;
    switch (mVariant) {
        case NORMAL: {
            static mVec3_c bbLo = mVec3_c(-65.0f, 0.0f, -100.0f);
            static mVec3_c bbUp = mVec3_c(65.0f, 150.0f, 45.0f);
            boundingBox.Set(bbLo, bbUp);
        } break;
        case SMALL: {
            static mVec3_c bbLo = mVec3_c(-38.0f, 0.0f, -70.0f);
            static mVec3_c bbUp = mVec3_c(38.0f, 110.0f, 35.0f);
            boundingBox.Set(bbLo, bbUp);
        } break;
        case BOSS: {
            static mVec3_c bbLo = mVec3_c(-90.0f, 0.0f, -140.0f);
            static mVec3_c bbUp = mVec3_c(90.0f, 170.0f, 60.0f);
            boundingBox.Set(bbLo, bbUp);
        } break;
        case GODDESS: {
            static mVec3_c bbLo = mVec3_c(-65.0f, 0.0f, -100.0f);
            static mVec3_c bbUp = mVec3_c(65.0f, 150.0f, 50.0f);
            boundingBox.Set(bbLo, bbUp);
        } break;
    }

    field_0x11EC = 1.0f;
    field_0x11FC = 0;

    field_0x4E8.r = 0;
    field_0x4E8.g = 0;
    field_0x4E8.b = 0;

    field_0x4EC = 0.0f;
    field_0x4DC = position;
    field_0x4DC.y += 100.0f;

    return SUCCEEDED;
}

void dAcTbox_c::initializeState_DugOut() {}
void dAcTbox_c::executeState_DugOut() {}
void dAcTbox_c::finalizeState_DugOut() {}
void dAcTbox_c::initializeState_WaitAppear() {}
void dAcTbox_c::executeState_WaitAppear() {}
void dAcTbox_c::finalizeState_WaitAppear() {}
void dAcTbox_c::initializeState_DemoAppear() {
    // TODO, just referencing this bit of data
    mMdl1.setAnm(sAppearAnmName, m3d::PLAY_MODE_4);
}
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
    return mVariant != SMALL;
}
