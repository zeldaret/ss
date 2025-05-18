#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/d_heap.h"
#include "d/d_pouch.h"
#include "d/d_rawarchive.h"
#include "d/d_sc_game.h"
#include "d/lyt/meter/d_lyt_meter.h"
#include "egg/core/eggHeap.h"
#include "m/m3d/m3d.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_mdl.h"
#include "m/m_allocator.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/g3d/g3d_anmobj.h"
#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/CX/cx.h"
#include "rvl/GX/GXTypes.h"
#include "sized_string.h"
#include "toBeSorted/cx_util.h"
#include "toBeSorted/file_manager.h"
#include "toBeSorted/stage_render_stuff.h"

// TODO: This file contains the vtable and a bunch of weak functions
// for daPlayerModelBase_c. How do we generate the vtable here though?
// All virtual methods of daPlayerModelBase_c seem to be inline, so
// there's not much opportunity to get the vtable to be emitted here.
// An explicit constructor seems unlikely given that we assume that the vtable
// of the base class is at 80533128, much later in the binary.

static u8 sSavedHandMats[2] = {};
static u8 sHandMats[14] = {};

#pragma push
#pragma readonly_strings on
const char *sFaceResNames[] = {
    "F_Default",
    "Fmaba01",
    "F_M",
    "F_Ha",
    "F_Cut",
    "F_CutAround",
    "F_Damage",
    "F_DamageLarge",
    "F_Die",
    "F_DieLargeDamage",
    "F_DieMagma",
    "F_Pain",
    "F_Wa",
    "F_DrinkSt",
    "F_Drink",
    "F_DrinkEd",
    "F_Breath",
    "F_Close",
    "F_entrance",
    "F_Binout",
    "F_GetBigWait",
    "F_getAwaitNice",
    "F_WaitLookRound",
    "F_Danger",
    "F_BinShield",
    "F_swimDieA",
    "F_WaitServiceA",
    "F_WaitServiceHotA",
    "F_BoxOp",
    "F_Notice",
    "F_swimP",
    "F_BedSleep",
    "F_BedAwake",
    "F_SwordPullSt",
    "F_SwordPullWait",
    "F_SwordPull",
    "F_SwordSirenSt",
    "F_SwordSirenRaise",
    "F_SwordSirenStick",
    "F_SwordSirenAfter",
    "F_Bird_Reach",
    "F_Bird_GetWait",
    "F_DieTruck",
    "F_Bird_FestivalStart",
    "F_GetSurprise",
    "F_AsuraCut",
    "F_AsuraCutSt",
    "F_AsuraCutEd",
    "F_CannonWait",
    "F_KrakenAppearance",
    "F_GetFlower",
    "F_Harp",
    "F_SitWaitServiceA",
    "F_Lightning",
    "F_LastAttack",
    "F_Bird_Call",
    // this one will go in sbss2 if you explicitly define each string
    // with const char[], so either a pragma or a file wide flag is used,
    // and this whole array is part of the .data pool
    "",
};
#pragma pop

const u8 daPlayerModelBase_c::sShieldDurabilities[10] = {16, 24, 32, 24, 36, 48, 12, 16, 20, 48};
const u8 daPlayerModelBase_c::sShieldRegenTimes[10] = {0, 0, 0, 0, 0, 0, 90, 90, 90, 0};

/**
 * Watch the player's hand material indices and change the ResMat
 * so that the right hand mats are visible and all others are hidden.
 */
void daPlBaseScnObjCallback_c::ExecCallback_DRAW_OPA(
    nw4r::g3d::ScnObj::Timing timing, nw4r::g3d::ScnObj *pObj, u32 param, void *pInfo
) {
    if (timing != nw4r::g3d::ScnObj::CALLBACK_TIMING_C) {
        nw4r::g3d::ScnMdl *pMdl = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdl>(pObj);
        nw4r::g3d::ResMdl mdl = pMdl->GetResMdl();
        nw4r::g3d::ResShp shp(nullptr);
        for (int i = 0; i < 2; i++) {
            u8 defaultMat = sSavedHandMats[i];
            // TODO: Is there a way to avoid this cast?
            u8 playerHandMat = static_cast<daPlayerModelBase_c *>(field_0x0C)->getHandMat(i);
            if (playerHandMat != defaultMat) {
                if (defaultMat != 0xFE) {
                    shp = mdl.GetResShp(sHandMats[defaultMat]);
                    shp.SetVisibility(false);
                    shp.DCStore(false);
                }
                if (playerHandMat != 0xFE) {
                    shp = mdl.GetResShp(sHandMats[playerHandMat]);
                    shp.SetVisibility(true);
                    shp.DCStore(false);
                }
                sSavedHandMats[i] = playerHandMat;
            }
        }
    }
}

void daPlBaseScnObjCallback_c::ExecCallback_CALC_MAT(
    nw4r::g3d::ScnObj::Timing timing, nw4r::g3d::ScnObj *pObj, u32 param, void *pInfo
) {
    if (timing != nw4r::g3d::ScnObj::CALLBACK_TIMING_C) {
        return;
    }
    dScnCallback_c::ExecCallback_CALC_MAT(timing, pObj, param, pInfo);
}

void daPlBaseMdlCallback_c::ExecCallbackA(
    nw4r::g3d::ChrAnmResult *result, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *calcWorld
) {
    u16 nodeId = calcWorld->GetCallbackNodeID();
    // Only run the full callback for certain specified nodes - TODO which ones?
    if ((nodeId >= mNodeIdMin && nodeId < mNodeIdMax) || (mNodeIdMin == 16 && nodeId == 0)) {
        m3d::mdl_c::mdlCallback_c::ExecCallbackA(result, mdl, calcWorld);
        return;
    }
    // Otherwise only save the result
    mpNodes[nodeId] = *result;
    if (mpBaseCallback != nullptr) {
        mpBaseCallback->timingA(nodeId, result, mdl);
    }
}

bool daPlBaseMdl_c::create(
    daPlayerModelBase_c *player, nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption, u32 nView, u32 *pSize
) {
    if (!m3d::mdl_c::create(mdl, &mCallback, alloc, bufferOption, nView, pSize)) {
        return false;
    }

    setBlendNodeRange(0, 0xFFFF, 0.0f);
    field_0x58 = 0;
    field_0x5C = 0;
    field_0x5A = 0xFFFF;
    SoundSource *sound = player->getSoundSource();
    if (sound->isReadyMaybe()) {
        sound->load(nullptr, "");
    }
    return true;
}

bool daPlBaseMdl_c::setAnm(m3d::banm_c &anm) {
    return m3d::bmdl_c::setAnm(anm);
}

void daPlBaseMdl_c::remove() {
    mCallback.remove();
    m3d::mdl_c::remove();
}

void daPlBaseMdl_c::setBlendNodeRange(u16 nodeIdMin, u16 nodeIdMax, f32 blendFrame) {
    mCallback.setBlendFrame(blendFrame + 1.0f);
    mCallback.setNodeIds(nodeIdMin, nodeIdMax);
    mCallback.calcBlend();
}

void daPlBaseMainCallback_c::timingA(u32 nodeId, nw4r::g3d::ChrAnmResult *result, nw4r::g3d::ResMdl mdl) {
    mpPlayer->mainModelTimingA(nodeId, result);
}

void daPlBaseMainCallback_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result, nw4r::g3d::ResMdl mdl) {
    mpPlayer->mainModelTimingB(nodeId, result);
}

void daPlBaseMainCallback_c::timingC(nw4r::math::MTX34 *result, nw4r::g3d::ResMdl mdl) {
    mpPlayer->mainModelTimingC(result);
}

void daPlBaseAnmChr_c::play() {
    m3d::fanm_c::play();
    field_0x36 = 0;
}

bool daPlBaseAnmChr_c::isFinished() {
    if (field_0x35 && !field_0x36) {
        return true;
    }

    if (isStop() || nw4r::math::FAbs(getRate()) < 0.001f) {
        field_0x35 = 1;
        field_0x36 = 1;
    }
    return false;
}

void daPlBaseHeadCallback_c::timingB(u32, nw4r::g3d::WorldMtxManip *, nw4r::g3d::ResMdl) {}

void daPlBaseCalcWorldCallback_c::ExecCallbackC(
    nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *pFuncObj
) {}

void daPlayerModelBase_c::freeFrmHeap(mHeapAllocator_c *allocator) {
    EGG::Heap::toFrmHeap(allocator->getHeap())->free(0x1 | 0x2);
}

void daPlayerModelBase_c::allocFrmHeap(mHeapAllocator_c *allocator, u32 size, const char *name) {
    // TODO fix char constness
    allocator->createNewTempFrmHeap(size, dHeap::work1Heap.heap, (char *)name, 0x20, 0);
    new u8[size]();
    allocator->adjustFrmHeapRestoreCurrent();
}

void daPlayerModelBase_c::allocExternalDataBuffers() {
    mpExternalAnmCharBuffer = heap_allocator.alloc(0xD800);
    mpAnmCharBuffer = heap_allocator.alloc(0x1000);
    mpTexPatBuffer = heap_allocator.alloc(0x1000);
    mpTexSrtBuffer = heap_allocator.alloc(0x1000);
}

void daPlayerModelBase_c::initModelHeaps() {
    allocFrmHeap(&mModelAllocator, 0x14000, "LinkClothes");
    allocFrmHeap(&mSwordAllocator, 0xE000, "LinkSword");
    allocFrmHeap(&mShieldAllocator, 0x1C100, "LinkShield");
}

void daPlayerModelBase_c::updateSwordShieldModelsIfNeeded() {
    if (mModelUpdateFlags & UPDATE_MODEL_SWORD) {
        updateSwordModel();
    }
    FileManager *f = FileManager::GetInstance();
    if (mModelUpdateFlags & UPDATE_MODEL_SHIELD) {
        f->setShieldPouchSlot(mShieldPouchSlot);
        updateShieldModel();
    }
}

m3d::anmTexSrt_c *daPlayerModelBase_c::createAnmTexSrt(const char *resName, m3d::bmdl_c &mdl) {
    m3d::anmTexSrt_c *anm = new m3d::anmTexSrt_c();
    (void)mAlink2Res.GetResAnmTexSrt(resName);
    anm->create(mdl.getResMdl(), mAlink2Res.GetResAnmTexSrt(resName), &heap_allocator, nullptr, 1);
    mdl.setAnm(*anm);
    return anm;
}

void daPlayerModelBase_c::updateEarringsColor() {
    if (mTunicType != TUNIC_SILENT) {
        mColor color = getEarringsColor();
        mFaceMdl.setTevKColor(mFaceMdl.getMatID("al_face_m"), GX_KCOLOR1, color, false);
    }
}

static const char *sBodyMdls[] = {"al", "pl_body", "Sl"};
static const char *sHeadMdls[] = {"al_head", "pl_head", "Sl_head"};
static const char *sFaceMdls[] = {"al_face", "al_face", "Sl_face"};
static const char *sHandMdls[] = {"al_hands", "pl_hands", "Sl_hands"};

void daPlayerModelBase_c::loadBodyModels() {
    mQuat1.set(1.0f, 0.0, 0.0f, 0.0f);
    mQuat2.set(1.0f, 0.0, 0.0f, 0.0f);
    freeFrmHeap(&mModelAllocator);

    nw4r::g3d::ResMdl bodyMdl = mAlink2Res.GetResMdl(sBodyMdls[mTunicType]);
    mMainMdl.create(this, bodyMdl, &mModelAllocator, 0x930, 1, nullptr);
    mMainMdl.setPriorityDraw(0x82, 0x7F);
    bodyMdl.ref().info.envelope_mtx_mode = nw4r::g3d::ResMdlInfoDataTypedef::EVPMATRIXMODE_APPROX;

    nw4r::g3d::ResMdl headMdl = mAlink2Res.GetResMdl(sHeadMdls[mTunicType]);
    createGenericMdl(headMdl, mHeadMdl, &mModelAllocator, 0x800);

    nw4r::g3d::ResMdl faceMdl = mAlink2Res.GetResMdl(sFaceMdls[mTunicType]);
    createGenericSmdl(faceMdl, mFaceMdl, &mModelAllocator, 0x807);

    mMainMdl.setCallback(&mMainBodyCallback);
    mMainBodyCallback.setPlayer(this);
    mHeadMdl.setCallback(&mheadCallback);
    mheadCallback.setPlayer(this);

    mScnCallback1.attach(mMainMdl);
    mScnCallback2.attach(mHeadMdl);

    nw4r::g3d::ResMdl handMdl = mAlink2Res.GetResMdl(sHandMdls[mTunicType]);
    createGenericSmdl(handMdl, mHandsMdl, &mModelAllocator, 0x800);
    nw4r::g3d::ScnMdlSimple *s = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdlSimple>(mHandsMdl.getG3dObject());
    s->SetScnMdlCallback(&mCalcWorldCallback);
    s->EnableScnMdlCallbackTiming(nw4r::g3d::ScnObj::CALLBACK_TIMING_C);
    mCalcWorldCallback.setPlayer(this);
    mHandsMdl.setCallback(&mScnObjCallback);
    // TODO args types
    mHandsMdl.enableCallbackTiming(
        nw4r::g3d::ScnObj::Timing(nw4r::g3d::ScnObj::CALLBACK_TIMING_A | nw4r::g3d::ScnObj::CALLBACK_TIMING_C)
    );
    mHandsMdl.enableCallbackOp(
        nw4r::g3d::ScnObj::ExecOp(nw4r::g3d::ScnObj::EXECOP_DRAW_OPA | nw4r::g3d::ScnObj::EXECOP_CALC_MAT)
    );
    loadHandsModels();

    mBeltMatId = mMainMdl.getMatID("al_belt_upper_m");
    mSkinMatId = mMainMdl.getMatID("al_skin_m");
    mPouchMatId = mMainMdl.getMatID("al_pouch_m");
    mUpbodyMatId = mMainMdl.getMatID("al_upbody_m");
    mUpotherMatId = mMainMdl.getMatID("al_upother_m");
    mBelt2MatId = mBeltMatId;
    mLowbodyMapId = mMainMdl.getMatID("al_lowbody_m");
    mLowotherMapId = mMainMdl.getMatID("al_lowother_m");
    mPouch2MatId = mPouchMatId;
    mCapMatId = mHeadMdl.getMatID("al_cap_m");

    offSwordAndMoreStates(NO_SHIETH | POUCH | FIRST_PERSON);

    if (mPouchMatId >= 0) {
        mMainMdl.setCullMode(mPouchMatId, GX_CULL_BACK, false);
    }
    mMainMdl.setCullMode(mBeltMatId, GX_CULL_BACK, false);
    mMainMdl.setCullMode(mSkinMatId, GX_CULL_BACK, false);
    updateEarringsColor();
}

void daPlayerModelBase_c::loadBody() {
    loadBodyModels();
    nw4r::g3d::ResMdl bodyMdl = mMainMdl.getResMdl();
    mAnmChrBlend.create(bodyMdl, 6, &heap_allocator);
    mAnmChrBlend.getAnimObj()->SetAnmFlag(nw4r::g3d::AnmObj::FLAG_USE_QUATERNION_ROTATION_BLEND, true);

    daPlBaseAnmChr_c *anms = mAnmChrs;
    // "waitS"
    nw4r::g3d::ResAnmChr resAnmChr26 = getExternalAnmChr(sAnimations[26].animName, mpExternalAnmCharBuffer, 0x2400);
    nw4r::g3d::AnmObjChr *animObj;
    for (s32 i = 0; i < 6; i++) {
        // TODO: stack swap, need to mess with inlines more
        anms->create(bodyMdl, resAnmChr26, &heap_allocator);
        anms->setAnm(mMainMdl, resAnmChr26, m3d::PLAY_MODE_0);
        f32 f;
        switch (i) {
            case 0:
            case 1:  f = 1.0f; break;
            default: f = 0.0f; break;
        }
        animObj = static_cast<nw4r::g3d::AnmObjChr *>(anms->getAnimObj());
        animObj->Release();
        if (isBodyAnmPart_0_2_4(i)) {
            animObj->Bind(bodyMdl, 16, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
            animObj->Bind(bodyMdl, 0, nw4r::g3d::AnmObjChr::BIND_ONE);
        } else {
            animObj->Bind(bodyMdl, 1, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
        }
        mAnmChrBlend.attach(i, anms, f);
        anms++;
    }

    mMainMdl.setAnm(mAnmChrBlend);

    // "F_default"
    mFaceAnmChr.create(
        mFaceMdl.getResMdl(), getExternalAnmChr(sFaceResNames[0], mpAnmCharBuffer, 0x1000), &heap_allocator, nullptr
    );
    mFaceMdl.setAnm(mFaceAnmChr);
    mFaceAnmChrIdx1 = 0;
    mFaceAnmChrIdx2 = 57; // invalid?

    // "Fmaba01"
    mFaceTexPat.create(
        mFaceMdl.getResMdl(), getExternalAnmTexPat(sFaceResNames[1], mpTexPatBuffer, 0x1000), &heap_allocator, nullptr,
        1
    );
    mFaceMdl.setAnm(mFaceTexPat);
    mFaceAnmTexPatIdx1 = 1;
    mFaceAnmTexPatIdx2 = 57; // invalid

    bool isInTrial = dScGame_c::currentSpawnInfo.getTrial() == SpawnInfo::TRIAL;
    mFaceTexSrt.create(
        mFaceMdl.getResMdl(), getExternalAnmTexSrt(sFaceResNames[0], mpTexSrtBuffer, 0x1000), &heap_allocator, nullptr,
        isInTrial ? 2 : 1
    );
    mFaceMdl.setAnm(mFaceTexSrt);
    mFaceAnmTexSrtIdx1 = 0;
    mFaceAnmTexSrtIdx2 = 57; // invalid

    // no allocator arg?
    mHeadAnmChr.create(mHeadMdl.getResMdl(), mPlCommonAnimeRes.GetResAnmChr("H_Default"), nullptr, nullptr);
    mHeadMdl.setAnm(mHeadAnmChr);
}

void daPlayerModelBase_c::loadHandsModels() {
    nw4r::g3d::ResMdl mdl = mHandsMdl.getResMdl();
    SizedString<16> handName("al_handRA_m");
    s32 ids[14];
    // Hand mats: al_handRA_m - al_handRG_m
    //            al_handLA_m - al_handLG_m
    for (s32 i = 0; i < 14; i++) {
        if (i < 7) {
            handName.mChars[8] = i + 'A';
        } else {
            if (i == 7) {
                handName.mChars[7] = 'L';
            }
            handName.mChars[8] = i + ('A' - 7);
        }
        ids[i] = m3d::getMatID(mdl, handName);
        sHandMats[i] = 0xFF;
    }

    nw4r::g3d::ResShp shp(nullptr);

    sSavedHandMats[0] = 7;
    sSavedHandMats[1] = 0;

    nw4r::g3d::ResMdl::DrawEnumerator drawEnumerator(mdl.GetResByteCode("DrawOpa"), mdl.GetResByteCode("DrawXlu"));
    while (drawEnumerator.IsValid()) {
        for (int j = 0; j < 14; j++) {
            if (ids[j] == drawEnumerator.GetMatID()) {
                sHandMats[j] = drawEnumerator.GetShpID();
                break;
            }
        }

        drawEnumerator.MoveNext();
    }

    for (int i = 0; i < mdl.GetResShpNumEntries(); i++) {
        shp = mdl.GetResShp(i);
        bool visible = sHandMats[sSavedHandMats[0]] == i || sHandMats[sSavedHandMats[1]] == i;
        if (visible) {
            shp.SetVisibility(true);
        } else {
            shp.SetVisibility(false);
        }
        shp.DCStore(false);
    }
}

void daPlayerModelBase_c::setSwordAnm(const char *name) {
    nw4r::g3d::ResAnmClr clr = mAlinkRes.GetResAnmClr(name);
    mSwordAnmMatClr.setAnm(mSwordMdl, clr, 0, m3d::PLAY_MODE_4);
    mSwordMdl.setAnm(mSwordAnmMatClr);
}
static const f32 sSwordRelatedX[] = {9.0f, 12.0, 12.0, 12.0, 12.0, 12.0, 9.0f};
static const f32 sSwordRelatedUnk[] = {73.0f, 80.0f, 100.0f, 100.0f, 120.0f, 120.0f, 73.0f};
static const f32 sSwordRelatedZ[] = {0.7f, 0.7f, 0.9f, 0.9f, 1.0f, 1.0f, 0.7f};
static const f32 sSwordRelatedY[] = {0.7f, 0.8f, 0.9f, 0.9f, 1.0f, 1.0f, 0.7f};

static const char *sSwordPods[] = {"EquipPodA", "EquipPodB",      "EquipPodC", "EquipPodD",
                                   "EquipPodE", "EquipPodMaster", "EquipPodA"};

void daPlayerModelBase_c::initSwordModel() {
    updateCurrentSword();
    freeFrmHeap(&mSwordAllocator);
    const char *swordName = getSwordName(sCurrentSword);
    mSwordRes = getItemResFile(swordName, mSwordAllocator);
    field_0x1278 = sSwordRelatedUnk[sCurrentSword];
    field_0x127C.x = sSwordRelatedX[sCurrentSword];
    field_0x127C.y = sSwordRelatedY[sCurrentSword];
    field_0x127C.z = sSwordRelatedZ[sCurrentSword];
    // TODO instruction order
    const char *pod = sSwordPods[sCurrentSword];

    nw4r::g3d::ResMdl sheathMdl = mSwordRes.GetResMdl(pod);
    createGenericSmdl(sheathMdl, mSheathMdl, &heap_allocator, 0x800);
    mScnCallback4.attach(mSheathMdl);

    nw4r::g3d::ResMdl swordMdl = mSwordRes.GetResMdl(swordName);
    createGenericSmdl(swordMdl, mSwordMdl, &heap_allocator, 0x810);
    mSwordMdl.setPriorityDraw(0x84, 0x7F);
    nw4r::g3d::ResAnmClr swordAnmClr = mAlinkRes.GetResAnmClr("appearance");
    nw4r::g3d::ResMdl swordResMdl = mSwordMdl.getResMdl();
    mSwordAnmMatClr.create(swordResMdl, swordAnmClr, &mSwordAllocator, nullptr, 1);
    mSwordMdl.setAnm(mSwordAnmMatClr);
    mScnCallback3.attach(mSwordMdl);
    offFlags_0x340(1);
    offSwordAndMoreStates(SWORD_AWAY);

    // TODO ids
    if (sCurrentSword == 0) {
        mSkywardStrikeChargeDuration = 82;
    } else if (sCurrentSword == 5) {
        if (dScGame_c::isHeroMode()) {
            mSkywardStrikeChargeDuration = 10;
        } else {
            mSkywardStrikeChargeDuration = 40;
        }
    } else {
        if (dScGame_c::isHeroMode()) {
            field_0x127C.z = 1.0f;
            mSkywardStrikeChargeDuration = 40;
        } else {
            mSkywardStrikeChargeDuration = 55;
        }
    }
}

void daPlayerModelBase_c::updateSwordModel() {
    mMtx_c sheathMtx;
    mMtx_c swordMtx;
    mSheathMdl.getLocalMtx(sheathMtx);
    mSwordMdl.getLocalMtx(swordMtx);

    mSwordAnmMatClr.remove();
    mSheathMdl.remove();
    mSwordMdl.remove();

    initSwordModel();

    setTransformAndCalc(mSheathMdl, &sheathMtx);
    setTransformAndCalc(mSwordMdl, &swordMtx);
    mModelUpdateFlags &= ~UPDATE_MODEL_SWORD;
}

static const char *sShieldModelsBase[] = {
    "EquipShieldWood", "EquipShieldIron", "EquipShieldHoly", "EquipShieldHylia", "EquipShieldWood",
};

static const char *sShieldModelsBroken[] = {
    "EquipShieldWoodBroken",  "EquipShieldIronBroken", "EquipShieldHolyBroken",
    "EquipShieldHyliaBroken", "EquipShieldWoodBroken",
};

extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_529_;
extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_535_;
extern "C" const u16 PARTICLE_RESOURCE_ID_MAPPING_537_;
void daPlayerModelBase_c::initShieldModel() {
    static const u16 shieldEffects[] = {
        PARTICLE_RESOURCE_ID_MAPPING_537_, PARTICLE_RESOURCE_ID_MAPPING_535_, PARTICLE_RESOURCE_ID_MAPPING_529_,
        PARTICLE_RESOURCE_ID_MAPPING_529_, PARTICLE_RESOURCE_ID_MAPPING_529_,
    };
    freeFrmHeap(&mShieldAllocator);
    s32 shieldType = getCurrentlyEquippedShieldType();
    s32 shieldId = getCurrentlyEquippedShieldId();
    nw4r::g3d::ResFile shieldRes = getItemResFile(sShieldModelsBase[shieldType], mShieldAllocator);
    createGenericSmdl(shieldRes.GetResMdl(sShieldModelsBase[shieldType]), mShieldMdl, &mShieldAllocator, 0x903);
    mScnCallback5.attach(mShieldMdl);
    if (shieldType != /* HYLIA */ 3) {
        mShieldAnmTexPat1.create(
            mShieldMdl.getResMdl(), shieldRes.GetResAnmTexPat(sShieldModelsBase[shieldType]), &mShieldAllocator,
            nullptr, 1
        );
        mShieldMdl.setAnm(mShieldAnmTexPat1);
        mShieldAnmTexPat1.setFrame(shieldId % 3, 0);

        createGenericSmdl(shieldRes.GetResMdl(sShieldModelsBroken[shieldType]), mUnkMdl1, &mShieldAllocator, 0x103);
        mAnmChr.create(
            mUnkMdl1.getResMdl(), shieldRes.GetResAnmChr(sShieldModelsBroken[shieldType]), &mShieldAllocator, nullptr
        );
        mUnkMdl1.setAnm(mAnmChr);
        mAnmChr.setFrameOnly(0.0f);

        mShieldAnmTexPat2.create(
            mUnkMdl1.getResMdl(), shieldRes.GetResAnmTexPat(sShieldModelsBase[shieldType]), &mShieldAllocator, nullptr,
            1
        );
        mShieldAnmTexPat2.setFrame(mShieldAnmTexPat1.getFrame(0), 0);
        mUnkMdl1.setAnm(mShieldAnmTexPat2);
    }
    mBreakingEffect = shieldEffects[shieldType];
    dLytMeter_c::setShieldId(shieldId);
    dLytMeter_c::setShieldMaxDurability(sShieldDurabilities[shieldId]);
    dAcPy_c::GetLink2()->onShieldUpdate();
}

void daPlayerModelBase_c::updateShieldModel() {
    mModelUpdateFlags &= ~(UPDATE_MODEL_SHIELD | 8); // TODO ???
    if (getCurrentShieldPouchSlot() != POUCH_SLOT_NONE) {
        mMtx_c mtx;
        mShieldMdl.getLocalMtx(mtx);
        mShieldAnmTexPat1.remove();
        mShieldAnmTexPat2.remove();
        mShieldMdl.remove();
        mAnmChr.remove();
        mUnkMdl1.remove();
        initShieldModel();
        setTransformAndCalc(mShieldMdl, &mtx);
    }
}

bool daPlayerModelBase_c::createGenericSmdl(
    nw4r::g3d::ResMdl resMdl, m3d::smdl_c &mdl, mAllocator_c *alloc, u32 bufferOption
) {
    bool ok = mdl.create(resMdl, alloc, bufferOption | 0x120, 1, nullptr);
    mdl.setPriorityDraw(0x82, 0x7F);
    return ok;
}

bool daPlayerModelBase_c::createGenericMdl(
    nw4r::g3d::ResMdl resMdl, m3d::mdl_c &mdl, mAllocator_c *alloc, u32 bufferOption
) {
    bool ok = mdl.create(resMdl, alloc, bufferOption | 0x120, 1, nullptr);
    mdl.setPriorityDraw(0x82, 0x7F);
    return ok;
}

void daPlayerModelBase_c::adjustMainModelChrAnm(PlayerMainModelNode_e nodeId, nw4r::g3d::ChrAnmResult *result) {
    mMtx_c mtx;
    result->GetMtx(mtx);
    if (nodeId == PLAYER_MAIN_NODE_HEAD) {
        mtx.YrotM(-mHeadYRot);
        mtx.ZrotM(mHeadZRot);
        mtx.XrotM(mHeadXRot);
    } else if (nodeId == PLAYER_MAIN_NODE_WAIST) {
        mtx.ZrotM(mWaistZRot);
        mtx.YrotM(-mWaistYRot);
    } else if (nodeId == PLAYER_MAIN_NODE_CENTER) {
        transformModelCenter(&mtx);
    } else if (nodeId == PLAYER_MAIN_NODE_ARM_R1 || nodeId == PLAYER_MAIN_NODE_ARM_R2) {
        mtx.ZrotM(getArmZRot(1));
    } else if (nodeId == PLAYER_MAIN_NODE_ARM_L1 || nodeId == PLAYER_MAIN_NODE_ARM_L2) {
        mtx.ZrotM(getArmZRot(0));
    } else if (nodeId == PLAYER_MAIN_NODE_RSKIRT_L1 || nodeId == PLAYER_MAIN_NODE_RSKIRT_L2) {
        if (getRidingActorType() == RIDING_LOFTWING) {
            if (nodeId == PLAYER_MAIN_NODE_RSKIRT_L1) {
                mtx.ZrotM(mRSkirtL1Rot);
            } else {
                mtx.ZrotM(3000);
            }
        }
    } else if (nodeId == PLAYER_MAIN_NODE_RSKIRT_R1 || nodeId == PLAYER_MAIN_NODE_RSKIRT_R2) {
        if (getRidingActorType() == RIDING_LOFTWING) {
            if (nodeId == PLAYER_MAIN_NODE_RSKIRT_R1) {
                mtx.ZrotM(mRSkirtR1Rot);
            } else {
                mtx.ZrotM(3000);
            }
        }
    } else if (mRightHandRotation != 0) {
        if (nodeId == PLAYER_MAIN_NODE_HAND_R) {
            static const mVec3_c v1(1.0f, 1.0f, 0.0f);
            mMtx_c rotMtx;
            rotMtx.setAxisRotation(v1, mRightHandRotation.radian2());
            mtx += rotMtx;
        }
    }

    result->SetMtx(mtx);
}

void daPlayerModelBase_c::adjustMainModelWorldMtx(PlayerMainModelNode_e nodeId, nw4r::g3d::WorldMtxManip *result) {
    if (checkCurrentAction(0xA9) && (nodeId == PLAYER_MAIN_NODE_ARM_R1 || nodeId == PLAYER_MAIN_NODE_ARM_R2)) {
        mMtx_c mtx;
        mtx.makeQ(mQuat1);
        mtx.YrotM(-rotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(rotation.y);
        mtx += mtx2;

        mMtx_c orig;
        result->GetMtx(orig);
        mVec3_c origTrans(orig.m[0][3], orig.m[1][3], orig.m[2][3]);
        orig += mtx;
        orig.m[0][3] = origTrans.x;
        orig.m[1][3] = origTrans.y;
        orig.m[2][3] = origTrans.z;
        if (nodeId == PLAYER_MAIN_NODE_ARM_R1) {
            fn_8005E900(&orig, 1500, 0, 0, nullptr, false);
        }
        result->SetMtxUnchecked(orig);
    } else if (mAnimations[3] == 0xE0 && nodeId == PLAYER_MAIN_NODE_ARM_R2) {
        mMtx_c mtx;
        mtx.makeQ(mQuat1);
        mtx.YrotM(-rotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(rotation.y);
        mtx += mtx2;

        mMtx_c orig;
        result->GetMtx(orig);
        mVec3_c origTrans(orig.m[0][3], orig.m[1][3], orig.m[2][3]);
        orig += mtx;
        orig.m[0][3] = origTrans.x;
        orig.m[1][3] = origTrans.y;
        orig.m[2][3] = origTrans.z;
        result->SetMtxUnchecked(orig);
    } else if (nodeId == PLAYER_MAIN_NODE_BACKBONE_1) {
        transformBackbone1(result);
        fn_8005EA20(result, field_0x1268 >> 1, field_0x126A * 0.4f + getArmZRot(2), field_0x126C, nullptr, false);
    } else if (nodeId == PLAYER_MAIN_NODE_BACKBONE_2) {
        fn_8005EA20(result, field_0x1268 >> 1, field_0x126A * 0.6f + getArmZRot(2), 0, nullptr, false);
    } else if (nodeId == PLAYER_MAIN_NODE_FSKIRT_L1 && *field_0x136C != 0) {
        mVec3_c v;
        vt_0x30C(v);
        if (*field_0x136C < 0) {
            v.z *= -1.0f;

        }
        fn_8005EA20(result, v.x * *field_0x136C, 0, v.z * *field_0x136C, nullptr, true);
    } else if (nodeId == PLAYER_MAIN_NODE_FSKIRT_R1 && field_0x1370) {
        mVec3_c v;
        vt_0x30C(v);
        if (*field_0x1370 > 0) {
            v.z *= -1.0f;

        }
        fn_8005EA20(result, v.x * *field_0x1370, 0, v.z * *field_0x1370, nullptr, true);
    } else if (nodeId == PLAYER_MAIN_NODE_POD) {
        mAng a1 = 0;
        mAng a2 = getArmZRot(2);

        fn_8005EA20(result, field_0x1268 / 2, field_0x126A * 0.6f + a1, a1, nullptr, false);
    } else if ((nodeId == PLAYER_MAIN_NODE_ARM_R2 || nodeId == PLAYER_MAIN_NODE_HAND_R) && isMPPose()) {
        mAng a1 = 0;
        mAng a2 = getArmZRot(2);

        fn_8005EA20(result, field_0x1268 / 2, field_0x126A * 0.6f + a1, a1, nullptr, false);
    }

    // Just getting weak functions to appear here
    mVec3_c v;
    vt_0x30C(v);
    isMPPose();
    isOnTightRope();
    isOnVines();
}

void daPlayerModelBase_c::fn_8005F890() {
    // Just getting weak functions to appear here
    vt_0x2E8();
    vt_0x304();
    mAng a1, a2;
    isOnClawTargetMaybe(0, a1, a2);
}

void daPlayerModelBase_c::mainModelTimingA(u32 nodeId, nw4r::g3d::ChrAnmResult *result) {
    adjustMainModelChrAnm((PlayerMainModelNode_e)nodeId, result);
}

void daPlayerModelBase_c::mainModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result) {}

void daPlayerModelBase_c::mainModelTimingC(nw4r::math::MTX34 *result) {}

void daPlayerModelBase_c::setTransformAndCalc(m3d::scnLeaf_c &lf, const mMtx_c *mtx) {
    if (mtx != nullptr) {
        lf.setLocalMtx(*mtx);
    }
    lf.calc(false);
}

nw4r::g3d::ResFile daPlayerModelBase_c::getItemResFile(const char *name, mAllocator_c &allocator) {
    SizedString<64> buf;
    buf.sprintf("dat/%s.brresC", name);
    void *dat = getOarcFile("PLCompItem", buf);
    u32 size = CXGetUncompressedSize(dat);
    void *decompressedBuf = allocator.alloc(size);
    lzSafeDecompress(dat, size, decompressedBuf);
    nw4r::g3d::ResFile file(decompressedBuf);
    file.Init();
    file.Bind();
    BindSystemModelsAndLighting(file);
    return file;
}

nw4r::g3d::ResFile
daPlayerModelBase_c::getExternalCompressedFile(const char *name, const char *extension, void *dest, u32 maxSize) {
    if (maxSize != 0) {
        SizedString<64> buf;
        buf.sprintf("%s.%s", name, extension);
        void *externalData = mPlCommonAnimeRes.GetExternalData(buf);
        lzSafeDecompress(externalData, maxSize, dest);
    }
    nw4r::g3d::ResFile file(dest);
    file.Init();
    return file;
}

nw4r::g3d::ResAnmChr daPlayerModelBase_c::getExternalAnmChr(const char *name, void *dest, u32 maxSize) {
    nw4r::g3d::ResAnmChr result;
    if (mCurrentRes.IsValid()) {
        result = mCurrentRes.GetResAnmChr(name);
        if (result.IsValid()) {
            return result;
        }
    }
    nw4r::g3d::ResFile file = getExternalCompressedFile(name, "rchaC", dest, maxSize);
    result = file.GetResAnmChr(name);
    return result;
}

nw4r::g3d::ResAnmTexPat daPlayerModelBase_c::getExternalAnmTexPat(const char *name, void *dest, u32 maxSize) {
    nw4r::g3d::ResAnmTexPat result;
    if (mCurrentRes.IsValid()) {
        result = mCurrentRes.GetResAnmTexPat(name);
        if (result.IsValid()) {
            result.Bind(mAlink2Res);
            return result;
        }
    }
    nw4r::g3d::ResFile file = getExternalCompressedFile(name, "rtpaC", dest, maxSize);
    result = file.GetResAnmTexPat(name);
    result.Bind(mAlink2Res);
    return result;
}

nw4r::g3d::ResAnmTexSrt daPlayerModelBase_c::getExternalAnmTexSrt(const char *name, void *dest, u32 maxSize) {
    nw4r::g3d::ResAnmTexSrt result;
    if (mCurrentRes.IsValid()) {
        result = mCurrentRes.GetResAnmTexSrt(name);
        if (result.IsValid()) {
            return result;
        }
    }
    nw4r::g3d::ResFile file = getExternalCompressedFile(name, "rtsaC", dest, maxSize);
    result = file.GetResAnmTexSrt(name);
    return result;
}

void daPlayerModelBase_c::fn_80061410() {
    // Just getting weak functions to appear here
    alwaysRet0();
}

/* vt 0x114 */ void daPlayerModelBase_c::somethingWithCarriedActorFlags() {
    // TODO
}
