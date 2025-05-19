#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/col/cc/d_cc_s.h"
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
#include "nw4r/math/math_types.h"
#include "rvl/CX/cx.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtxvec.h"
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
static const char *sFaceResNames[] = {
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
    mpSoundData = nullptr;
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

void daPlBaseHeadCallback_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result, nw4r::g3d::ResMdl) {
    mpPlayer->headModelTimingB(nodeId, result);
}

void daPlBaseHandsCallback_c::ExecCallbackC(
    nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *pFuncObj
) {
    mpPlayer->handsCallbackC(pMtxArray, mdl, pFuncObj);
}

// TODO explain
bool daPlayerModelBase_c::isBodyAnmPart_0_2_4(s32 part) {
    return part == 0 || part == 2 || part == 4;
}

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
    mHeadMdl.setCallback(&mHeadCallback);
    mHeadCallback.setPlayer(this);

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
            animObj->Bind(bodyMdl, PLAYER_MAIN_NODE_WAIST, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
            animObj->Bind(bodyMdl, PLAYER_MAIN_NODE_CENTER, nw4r::g3d::AnmObjChr::BIND_ONE);
        } else {
            animObj->Bind(bodyMdl, PLAYER_MAIN_NODE_BACKBONE_1, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
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

    nw4r::g3d::ResMdl::DrawEnumerator drawEnumerator = mdl.ConstructDrawEnumerator();
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
    createGenericSmdl(sheathMdl, mSheathMdl, &mSwordAllocator, 0x800);
    mScnCallback4.attach(mSheathMdl);

    nw4r::g3d::ResMdl swordMdl = mSwordRes.GetResMdl(swordName);
    createGenericSmdl(swordMdl, mSwordMdl, &mSwordAllocator, 0x810);
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
    "EquipShieldWoodBroken", "EquipShieldIronBroken", "EquipShieldHolyBroken",
    "EquipShieldHylia", "EquipShieldWoodBroken",
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

void daPlayerModelBase_c::applyWorldRotationMaybe(
    nw4r::math::MTX34 *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order
) {
    mVec3_c v;
    v.x = result->_03;
    v.y = result->_13;
    v.z = result->_23;
    mMtx_c work;
    if (off != nullptr) {
        MTXTrans(work, off->x, off->y, off->z);
    } else {
        MTXTrans(work, v.x, v.y, v.z);
    }
    work.YrotM(rotation.y);
    if (order) {
        work.ZYXrotM(x, y, z);
    } else {
        work.ZXYrotM(x, y, z);
    }
    work.YrotM(-rotation.y);
    mMtx_c translateBack;
    translateBack.transS(-v.x, -v.y, -v.z);
    MTXConcat(work, translateBack, work);
    MTXConcat(work, *result, *result);
}

void daPlayerModelBase_c::applyWorldRotationMaybe(
    nw4r::g3d::WorldMtxManip *result, mAng x, mAng y, mAng z, mVec3_c *off, bool order
) {
    mMtx_c mtx;
    result->GetMtx(mtx);
    applyWorldRotationMaybe(mtx, x, y, z, off, order);
    result->SetMtxUnchecked(mtx);
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
        MTXConcat(mtx2, mtx, mtx);

        mMtx_c orig;
        result->GetMtx(orig);
        mVec3_c origTrans;
        orig.getTranslation(origTrans);
        MTXConcat(mtx, orig, orig);
        orig.setTranslation(origTrans);
        if (nodeId == PLAYER_MAIN_NODE_ARM_R1) {
            applyWorldRotationMaybe(orig, 1500, 0, 0, nullptr, false);
        }
        result->SetMtxUnchecked(orig);
    } else if (mAnimations[3] == 0xE0 && nodeId == PLAYER_MAIN_NODE_ARM_R2) {
        mMtx_c mtx;
        mtx.makeQ(mQuat1);
        mtx.YrotM(-rotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(rotation.y);
        MTXConcat(mtx2, mtx, mtx);

        mMtx_c orig;
        result->GetMtx(orig);
        mVec3_c origTrans;
        orig.getTranslation(origTrans);
        MTXConcat(mtx, orig, orig);
        orig.setTranslation(origTrans);
        result->SetMtxUnchecked(orig);
    } else if (nodeId == PLAYER_MAIN_NODE_BACKBONE_1) {
        transformBackbone1(result);
        applyWorldRotationMaybe(
            result, field_0x1268 >> 1, field_0x126A * 0.4f + getArmZRot(2), field_0x126C, nullptr, false
        );
    } else if (nodeId == PLAYER_MAIN_NODE_BACKBONE_2) {
        applyWorldRotationMaybe(result, field_0x1268 >> 1, field_0x126A * 0.6f + getArmZRot(2), 0, nullptr, false);
    } else if (nodeId == PLAYER_MAIN_NODE_FSKIRT_L1 && *field_0x136C != 0) {
        mVec3_c v;
        vt_0x30C(v);
        if (*field_0x136C < 0) {
            v.z *= -1.0f;
        }
        applyWorldRotationMaybe(result, v.x * *field_0x136C, 0, v.z * *field_0x136C, nullptr, true);
    } else if (nodeId == PLAYER_MAIN_NODE_FSKIRT_R1 && *field_0x1370) {
        mVec3_c v;
        vt_0x30C(v);
        if (*field_0x1370 > 0) {
            v.z *= -1.0f;
        }
        applyWorldRotationMaybe(result, v.x * *field_0x1370, 0, v.z * *field_0x1370, nullptr, true);
    } else if (nodeId == PLAYER_MAIN_NODE_POD) {
        if (field_0x1258 != 0) {
            mMtx_c mtx;
            mtx.YrotS(field_0x125A);
            mtx.XrotM(field_0x1258);
            mtx.YrotM(-field_0x125A);
            mMtx_c orig;
            result->GetMtx(orig);
            mVec3_c origTrans;
            orig.getTranslation(origTrans);
            MTXConcat(mtx, orig, orig);
            orig.setTranslation(origTrans);
            result->SetMtxUnchecked(orig);
        }
        if (field_0x1268 < 0) {
            applyWorldRotationMaybe(result, -(field_0x1268 >> 1), 0, 0, nullptr, false);
        }
    } else if ((nodeId == PLAYER_MAIN_NODE_ARM_R2 || nodeId == PLAYER_MAIN_NODE_HAND_R) && isMPPose()) {
        mMtx_c mtx;
        mtx.makeQ(mQuat2);
        mtx.YrotM(-rotation.y);
        mMtx_c mtx2;
        mtx2.YrotS(rotation.y);
        MTXConcat(mtx2, mtx, mtx);

        mMtx_c orig;
        result->GetMtx(orig);
        mVec3_c origTrans;
        orig.getTranslation(origTrans);
        MTXConcat(mtx, orig, orig);
        orig.setTranslation(origTrans);
        result->SetMtxUnchecked(orig);
    } else if (nodeId == PLAYER_MAIN_NODE_HAND_R || nodeId == PLAYER_MAIN_NODE_HAND_L) {
        if (isOnTightRope()) {
            mAng rot;
            if (nodeId == PLAYER_MAIN_NODE_HAND_R) {
                rot = -5461;
            } else {
                rot = -7282;
            }
            applyWorldRotationMaybe(result, rot, 0, 0, nullptr, false);
        } else if (isOnVines()) {
            // TODO what even is this
            mAng v1 = -3277;
            mAng v2 = 910;
            if (nodeId == PLAYER_MAIN_NODE_HAND_R) {
                v1 *= -1;
                v2 *= -1;
            }
            applyWorldRotationMaybe(result, 0xec17, v1, v2, nullptr, true);
        }
    }
}

void daPlayerModelBase_c::updateMainBlend1(f32 blend) {
    // TODO document numbers
    mAnmChrBlend.setWeight(1, (1.0f - blend) * (1.0f - field_0x1274));
    mAnmChrBlend.setWeight(3, (1.0f - blend) * field_0x1274);
    mAnmChrBlend.setWeight(5, blend);
}

void daPlayerModelBase_c::updateMainBlend2(f32 blend, bool save) {
    // TODO document numbers
    f32 blend4 = 1.0f - mAnmChrBlend.getWeight(4);
    mAnmChrBlend.setWeight(0, blend4 * (1.0f - blend));
    mAnmChrBlend.setWeight(2, blend4 * blend);
    f32 blend5 = 1.0f - mAnmChrBlend.getWeight(5);
    mAnmChrBlend.setWeight(1, blend5 * (1.0f - blend));
    mAnmChrBlend.setWeight(3, blend5 * blend);
    if (save) {
        field_0x1274 = blend;
    }
}

void daPlayerModelBase_c::updateBlendWeights(PlayerMainModelNode_e nodeId) {
    switch (field_0x1219) {
        case 9:
            if (nodeId == PLAYER_MAIN_NODE_CENTER) {
                updateMainBlend2(field_0x1274, false);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_R) {
                if (field_0x1274 < 0.5f) {
                    updateMainBlend2(0.0f, false);
                } else {
                    updateMainBlend2(1.0f, false);
                }
            }
            break;
        case 1:
            if (nodeId == PLAYER_MAIN_NODE_CENTER) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 4:
            if (nodeId == PLAYER_MAIN_NODE_CENTER) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_L) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 2:
            if (nodeId == PLAYER_MAIN_NODE_WEAPON_L) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_R) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 3:
            if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_L) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 5:
            if (nodeId == PLAYER_MAIN_NODE_HEAD) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(field_0x1270);
            }
            break;
        case 7:
            if (nodeId == PLAYER_MAIN_NODE_CENTER) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_HEAD) {
                updateMainBlend1(1.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_WEAPON_R) {
                updateMainBlend1(1.0f);
            }
            break;
        case 6:
            if (nodeId == PLAYER_MAIN_NODE_HEAD) {
                updateMainBlend2(1.0f, false);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend2(0.0f, false);
            }
            break;
        case 8:
            if (nodeId == PLAYER_MAIN_NODE_HEAD) {
                updateMainBlend1(0.0f);
            } else if (nodeId == PLAYER_MAIN_NODE_POD) {
                updateMainBlend1(field_0x1270);
            }
            break;
    }
}

void daPlayerModelBase_c::fn_8005F890(nw4r::math::MTX34 *result) {
    // Probably related to IK?
    static const u16 sNodeIdsArms1[] = {PLAYER_MAIN_NODE_ARM_L1, PLAYER_MAIN_NODE_ARM_R1};
    static const Vec sArmVecs[] = {
        {29.0f, 0.0f, 0.0f},
        {26.5f, 0.0f, 0.0f},
        { 0.0f, 0.0f, 0.0f}
    };

    nw4r::g3d::ResMdl mdl = mMainMdl.getResMdl();

    if (vt_0x2E8(result, sNodeIdsArms1, false)) {
        mVec3_c swordOffset;
        if (checkCurrentAction(/* SWORD_IN_DIAL */ 0x86) || checkCurrentAction(/* ON_BIRD */ 0x8A)) {
            swordOffset = vt_0x304() * 0.5f;
        } else {
            swordOffset = mVec3_c::Zero;
        }

        mVec3_c dstVec;
        // (1) For some reason the compiler sets up pointers to stack vars here...
        for (s32 arm = 0; arm < 2; arm++) {
            mAng rot1;
            mAng rot2;
            isOnClawTargetMaybe(arm, rot1, rot2);
            u16 armNode = sNodeIdsArms1[arm];
            mVec3_c *dst = nullptr;
            mAng *angs;
            if (arm == 0) {
                angs = field_0x1374;
            } else {
                angs = field_0x1378;
            }
            nw4r::math::MTX34 *targetMtx;
            // Walk down the arm bones and transform the whole thing again?
            for (s32 i = 0; i < 3; i++) {
                u32 mtxId = mdl.GetResNode(armNode).GetMtxID();
                targetMtx = &result[mtxId];
                applyWorldRotationMaybe(targetMtx, rot1, rot2, angs[i], dst, false);
                mVec3_c v;
                v.x = sArmVecs[i].x;
                v.y = sArmVecs[i].y;
                v.z = sArmVecs[i].z;
                MTXMultVec(*targetMtx, v, dstVec);
                if (i != 2 && arm == 1) {
                    // sword hand
                    dstVec -= swordOffset;
                }
                dst = &dstVec;
                armNode++;
            }
            mVec3_c translate;
            mMainMdl.getNodeResult(armNode)->GetTranslate(translate);
            MTXMultVec(*targetMtx, translate, *dst);
            // (2) ... here it stores the passed-by-value angles to the stack,
            // then calls GetResNode and uses the previously set up pointers as arguments?
            applyWorldRotationMaybe(&result[mdl.GetResNode(armNode).GetMtxID()], rot1, rot2, angs[2], &dstVec, false);
        }
    }
}

void daPlayerModelBase_c::mainModelTimingA(u32 nodeId, nw4r::g3d::ChrAnmResult *result) {
    adjustMainModelChrAnm((PlayerMainModelNode_e)nodeId, result);
}

void daPlayerModelBase_c::mainModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result) {
    adjustMainModelWorldMtx((PlayerMainModelNode_e)nodeId, result);
    if (field_0x1219 != 0) {
        if (nodeId == PLAYER_MAIN_NODE_CENTER || nodeId == PLAYER_MAIN_NODE_POD ||
            nodeId == PLAYER_MAIN_NODE_WEAPON_L || nodeId == PLAYER_MAIN_NODE_HEAD ||
            nodeId == PLAYER_MAIN_NODE_WEAPON_R) {
            updateBlendWeights((PlayerMainModelNode_e)nodeId);
        }
    }
}

void daPlayerModelBase_c::mainModelTimingC(nw4r::math::MTX34 *result) {}

void daPlayerModelBase_c::handsCallbackC(
    nw4r::math::MTX34 *pMtxArray, nw4r::g3d::ResMdl mdl, nw4r::g3d::FuncObjCalcWorld *pFuncObj
) {
    mMainMdl.getNodeWorldMtx(PLAYER_MAIN_NODE_HAND_L, &pMtxArray[mdl.GetResNode(PLAYER_HANDS_NODE_HAND_L).GetMtxID()]);
    mMainMdl.getNodeWorldMtx(PLAYER_MAIN_NODE_HAND_R, &pMtxArray[mdl.GetResNode(PLAYER_HANDS_NODE_HAND_R).GetMtxID()]);
}

void daPlayerModelBase_c::headModelTimingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result) {
    if (nodeId >= PLAYER_HEAD_NODE_MOMI_Z_CAP_1) {
        mMtx_c mtx;
        result->GetMtx(mtx);
        mVec3_c v;
        mtx.getTranslation(v);
        mMtx_c rot;
        rot.makeQ(field_0x1300[nodeId - 6]);
        MTXConcat(rot, mtx, mtx);
        mtx.setTranslation(v);
        result->SetMtxUnchecked(mtx);
    } else if (nodeId >= PLAYER_HEAD_NODE_HAIR_L) {
        mAng oldYRot = rotation.y;
        rotation.y = field_0x1256;
        u32 idx = nodeId - 1;
        applyWorldRotationMaybe(result, field_0x1238[idx], 0, field_0x1242[idx], nullptr, false);
        rotation.y = oldYRot;
    }
}

void daPlayerModelBase_c::setTransformAndCalc(m3d::scnLeaf_c &lf, const mMtx_c *mtx) {
    if (mtx != nullptr) {
        lf.setLocalMtx(*mtx);
    }
    lf.calc(false);
}

void daPlayerModelBase_c::setShieldTransform(bool inHand) {
    mMtx_c targetTransform;
    if (inHand) {
        mMainMdl.getNodeWorldMtx(PLAYER_MAIN_NODE_WEAPON_L, targetTransform);
    } else {
        mMainMdl.getNodeWorldMtx(PLAYER_MAIN_NODE_POD, targetTransform);
        mMtx_c mtx2;
        MTXTrans(mtx2, 4.2f, 4.4f, 20.0f);
        MTXConcat(targetTransform, mtx2, targetTransform);
        targetTransform.ZYXrotM(mAng::fromDeg(91.0f), mAng::fromDeg(-123.0f), 0);
    }
    setTransformAndCalc(mShieldMdl, &targetTransform);
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

void daPlayerModelBase_c::loadSound(nw4r::g3d::ResFile file, const char *name, s32 animIdx) {
    SoundSource *s = getSoundSource();
    mCurrentAnmChrIdx = animIdx;
    SizedString<64> path;
    path.sprintf("%s.brasd", name);
    void *dat = file.GetExternalData(path);
    if (dat == nullptr && file == mCurrentRes) {
        dat = mPlCommonSoundRes.GetExternalData(path);
    }
    mMainMdl.setSoundRelated(mAnimations[animIdx], dat);
    s->load(dat, name);
}

void daPlayerModelBase_c::syncSoundWithAnim() {
    f32 frame = mAnmChrs[mCurrentAnmChrIdx].getFrame();
    getSoundSource()->setFrame(frame);
}

void daPlayerModelBase_c::registMassObj(cCcD_Obj* obj, u8 priority) {
    dCcS::GetInstance()->Set(obj);
    dCcS::GetInstance()->GetMassMng().SetObj(obj, priority);
}

void daPlayerModelBase_c::updateModelColliders() {
    mVec3_c bodyTranslation;
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_CENTER, bodyTranslation);

    mVec3_c bottom;
    bottom = ((mToeTranslation[0] + mToeTranslation[1]) * 0.5f + bodyTranslation) * 0.5f;
    f32 f = mToeTranslation[0].y > mToeTranslation[1].y ? mToeTranslation[1].y : mToeTranslation[0].y;
    f32 height;
    if (bodyTranslation.y > f) {
        height = bodyTranslation.y - f;
        bottom.y = f;
    } else {
        bottom.y = bodyTranslation.y;
        height = f - bodyTranslation.y;
    }

    if (height < 60.0f) {
        bottom.y -= (60.0f - height) * 0.5f;
        height = 60.0f;
    }
    mCcCyls[0].SetC(bottom);
    mCcCyls[0].SetH(height);

    if (!checkActionFlagsCont(0x20000)) {
        bottom = (bodyTranslation + mHeadTranslation) * 0.5f;
    }

    if (bodyTranslation.y > mHeadTranslation.y) {
        height = bodyTranslation.y - mHeadTranslation.y;
        bottom.y = mHeadTranslation.y;
    } else {
        bottom.y = bodyTranslation.y;
        height = mHeadTranslation.y - bodyTranslation.y;
    }

    if (height < 60.0f) {
        bottom.y -= (60.0f - height) * 0.5f;
        height = 60.0f;
    }

    mCcCyls[2].SetC(bottom);
    mCcCyls[2].SetH(height);

    bottom = (mCcCyls[0].GetC() + mCcCyls[2].GetC()) * 0.5f;
    f32 midH = (mCcCyls[0].GetH() + mCcCyls[2].GetH()) * 0.5f;

    mCcCyls[1].SetC(bottom);
    mCcCyls[1].SetH(midH);
}

void daPlayerModelBase_c::updateCachedPositions() {
    static const Vec sPos1 = {12.0f, -8.0f, 0.0f};
    mMainMdl.getNodeWorldMtxMultVec(PLAYER_MAIN_NODE_HEAD, sPos1, poscopy2);
    static const Vec sPosAboveLink = {0.0f, -8.0f, 0.0f};
    mMainMdl.getNodeWorldMtxMultVec(PLAYER_MAIN_NODE_HEAD, sPosAboveLink, mPositionAboveLink);
    static const Vec sHeadPos = {0.0f, -28.0f, 0.0f};
    mMainMdl.getNodeWorldMtxMultVec(PLAYER_MAIN_NODE_HEAD, sHeadPos, mHeadTranslation);

    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_TOE_L, mToeTranslation[0]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_TOE_R, mToeTranslation[1]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_HAND_L, mTranslationHand[0]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_HAND_R, mTranslationHand[1]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_WEAPON_L, mTranslationWeapon[0]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_WEAPON_R, mTranslationWeapon[1]);
    mMainMdl.getNodeWorldMtxMultVecZero(PLAYER_MAIN_NODE_CENTER, mCenterTranslation);

    if (checkCurrentAction(/* DOWSE_LOOK */ 0x68)) {
        mVec3_c v(0.0f, 18.0f, 0.0f);
        v.rotX(field_0x1268);
        v.rotY(rotation.y + field_0x126A);
        poscopy2 += v;
    }
}

void daPlayerModelBase_c::fn_80061410() {
    // Just getting weak functions to appear here
    alwaysRet0();
}

// I believe this is the only strong virtual function of daPlayerModelBase_c,
// and this causes the vtable and all other weak functions to be here
/* vt 0x114 */ void daPlayerModelBase_c::somethingWithCarriedActorFlags() {
    // TODO
}
mColor daPlayerModelBase_c::sGuideColor1(0x00, 0x82, 0xDC, 0xFF);
mColor daPlayerModelBase_c::sGuideColor2(0x64, 0xFF, 0xFF, 0xFF);
mColor daPlayerModelBase_c::sGuideColor3(0x46, 0xC8, 0xFF, 0xFF);
