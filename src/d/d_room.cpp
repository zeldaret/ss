#include "d/d_room.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_itembase.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w_base.h"
#include "d/col/bg/d_bg_w_kcol.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/d_stage_parse.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/t/d_t_mass_obj.h"
#include "egg/math/eggMath.h"
#include "f/f_base.h"
#include "m/m3d/m_anmmatclr.h"
#include "m/m3d/m_anmtexpat.h"
#include "m/m3d/m_anmtexsrt.h"
#include "m/m3d/m_anmvis.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_smdl.h"
#include "m/m_heap.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resanmvis.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/math/math_geometry.h"
#include "nw4r/math/math_types.h"
#include "sized_string.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/d_d3d.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_BASE_PROFILE(ROOM, dRoom_c, fProfile::ROOM, 0x9, 96);

STATE_DEFINE(dRoom_c, Active);
STATE_DEFINE(dRoom_c, NonActive);

int dRoom_c::create() {
    roomid = mParams & 0x3F;
    mCanHavePastState = dStageMgr_c::GetInstance()->getSTIFunk1() == 0 &&
                        // SSH machine room (less sure about D303...)
                        !(dScGame_c::isCurrentStage("D301") && roomid == 12) &&
                        !(dScGame_c::isCurrentStage("D303") && roomid == 12) &&
                        // LMF first two rooms, Gust Bellows room
                        !(dScGame_c::isCurrentStage("D300") && (roomid == 0 || roomid == 1 || roomid == 4)) &&
                        // LMF crawlspace, spike maze
                        !(dScGame_c::isCurrentStage("D300_1") && (roomid == 7 || roomid == 9));
    if (!mAllocator.createFrmHeapToCurrent(
            -1, CurrentStageArcManager::GetInstance()->getHeap(roomid), "dRoom_c::m_allocator", 0x20, mHeap::OPT_NONE
        )) {
        return FAILED;
    }

    bool anyError = false;

    mRoomRes = nw4r::g3d::ResFile(CurrentStageArcManager::GetInstance()->loadFromRoomArc(roomid, "g3d/room.brres"));
    dStage_c::bindStageResToFile(&mRoomRes);
    dStage_c::bindSkyCmnToResFile(&mRoomRes);

    model_c *mdl = mModels;
    for (s32 i = 0; i < ROOM_NUM_MODELS; i++, mdl++) {
        if (i != 2 || roomid != 1 || dScGame_c::currentSpawnInfo.layer != 14 || !dScGame_c::isCurrentStage("F406")) {
            if (!mdl->create(mRoomRes, mAllocator, i, &mWaterThing)) {
                anyError = true;
                break;
            }
            if (mdl->mpAnmPat) {
                mHasAnmTexPat = true;
            }
        }
    }

    if (!anyError) {
        if (mWaterThing.hasLinks()) {
            u32 val = 0;
            dStageMgr_c *stageMr = dStageMgr_c::GetInstance();
            if (stageMr->checkFlag0x88A0(2)) {
                val = 3;
            }
            stageMr->initUnkWithWater(val, &mWaterThing);
        }
        anyError = setupBg();
    }
    mAllocator.adjustFrmHeapRestoreCurrent();

    if (anyError) {
        return FAILED;
    }

    dStage_c::GetInstance()->setRoom(roomid, this);
    BZS = CurrentStageArcManager::GetInstance()->loadFromRoomArc(roomid, "dat/room.bzs");
    parseRoomBzs(roomid, BZS);
    mDidAlreadyInit = (mParams >> 6) & 1;
    mStateMgr.changeState(StateID_Active);
    if (roomid != dScGame_c::currentSpawnInfo.room) {
        mFlags |= 2;
        deactivateUpdates();
    }

    return SUCCEEDED;
}

int dRoom_c::doDelete() {
    destroyModels();
    if (mWaterThing.hasLinks()) {
        u32 val = 0;
        dStageMgr_c *stageMr = dStageMgr_c::GetInstance();
        if (stageMr->checkFlag0x88A0(2)) {
            val = 3;
        }
        stageMr->destroyUnkWithWater(val, &mWaterThing);
    }

    dStage_c::GetInstance()->setRoom(roomid, nullptr);
    return SUCCEEDED;
}

int dRoom_c::execute() {
    mStateMgr.executeState();
    f32 val = 0.0f;
    if (mCanHavePastState) {
        val = dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(roomid, mVec3_c::Zero, nullptr, 1000000.0f);
        mSkipDrawing = !mHasAnmTexPat && (!dTimeAreaMgr_c::GetInstance()->isInLanayruMiningFacility() || val > 0.0f) &&
                       dTimeAreaMgr_c::GetInstance()->isField0x78();
    } else {
        mSkipDrawing = false;
    }

    if ((mFlags & 2) != 0 || mSkipDrawing) {
        releaseBg();
        return SUCCEEDED;
    }

    executeBg();
    for (s32 i = 0; i < ROOM_NUM_MODELS; i++) {
        mModels[i].execute(i, mCanHavePastState, val);
    }

    return SUCCEEDED;
}

int dRoom_c::draw() {
    if ((mFlags & 2) != 0 || mSkipDrawing) {
        return SUCCEEDED;
    }
    for (s32 i = 0; i < ROOM_NUM_MODELS; i++) {
        mModels[i].draw(roomid);
    }
    return SUCCEEDED;
}

const PLY *dRoom_c::getEntranceById(u8 entranceId) const {
    s32 i = mPlyCount;
    const PLY *ply = mpPly;
    while (i > 0) {
        if (ply->entranceId == entranceId) {
            return ply;
        }
        ply++;
        i--;
    }
    return mpPly;
}

const CAM *dRoom_c::getCamForIndex(u32 idx) const {
    return &mpCam[idx];
}

const EVNT *dRoom_c::getEventForIndex(u32 idx) const {
    return &mpEvnt[idx];
}

void deactivateUpdatesCb(dAcBase_c *ac) {
    if (!ac->checkActorProperty(dAcBase_c::AC_PROP_0x400)) {
        return;
    }
    ac->unkVirtFunc_0x60();
}

void dRoom_c::deactivateUpdates() {
    if (!mUpdatesDeactivated) {
        foreachObject(deactivateUpdatesCb);
        setProcControl(ROOT_DISABLE_EXECUTE | ROOT_DISABLE_DRAW);
        mUpdatesDeactivated = true;
    }
}

void activateUpdatesCb(dAcBase_c *ac) {
    if (!ac->checkActorProperty(dAcBase_c::AC_PROP_0x400)) {
        return;
    }
    ac->restorePosRotFromCopy();
}

void dRoom_c::activateUpdates() {
    if (mUpdatesDeactivated) {
        foreachObject(activateUpdatesCb);
        unsetProcControl(ROOT_DISABLE_EXECUTE);
        unsetProcControl(ROOT_DISABLE_DRAW);
        mUpdatesDeactivated = false;
    }
}

s32 dRoom_c::foreachObject(foreachObjCallback cb) {
    s32 count = 0;

    fBase_c *start = getConnectBrNext();
    if (start == nullptr) {
        start = getConnectParent();
    }

    for (fBase_c *base = getConnectChild(); base != nullptr && base != start; base = getConnectTreeNext(base)) {
        if (base->mGroupType == ACTOR || base->mGroupType == STAGE) {
            cb(static_cast<dAcBase_c *>(base));
            count++;
        }
    }
    return count;
}

struct BgData {
    const char *kcl;
    const char *plc;
};

static const BgData sRoomBg[] = {
    {  "kcl/room.kcl",   "dat/room.plc"},
    {"kcl/room_n.kcl", "dat/room_n.plc"},
};

bool dRoom_c::setupBg() {
    for (int i = 0; i < ROOM_NUM_BG; i++) {
        dBgWKCol *bg = &mBg[i];
        void *kcl = CurrentStageArcManager::GetInstance()->getDataFromRoomArc(roomid, sRoomBg[i].kcl);
        if (kcl != nullptr) {
            void *plc = CurrentStageArcManager::GetInstance()->loadFromRoomArc(roomid, sRoomBg[i].plc);
            bg->Set(kcl, plc);
            bg->SetRoomId(roomid);
            bg->SetPriority(dBgW_Base::PRIORITY_0);
            bool ok = bg->InitMapStuff(&mAllocator);
            if (!ok) {
                return true;
            }

            if (mCanHavePastState && !mHasAnmTexPat) {
                bg->SetUnkBase(1);
            }
            // ???
            dBgS::GetInstance()->Regist(bg, (dAcObjBase_c *)nullptr);
            dBgS::GetInstance()->RegistBg(bg, nullptr);
        }
    }
    return false;
}

void dRoom_c::executeBg() {
    for (int i = 0; i < ROOM_NUM_BG; i++) {
        if (!mBg[i].ChkNotReady()) {
            dBgS::GetInstance()->Regist(&mBg[i], (dAcObjBase_c *)nullptr);
        }
    }
}

void dRoom_c::releaseBg() {
    for (int i = 0; i < 2; i++) {
        if (!mBg[i].ChkNotReady()) {
            dBgS::GetInstance()->Release(&mBg[i]);
        }
    }
}

void dRoom_c::drawOnMapIfVisible(mMtx_c *mtx, int param) {
    // Dungeon map shows rooms but doesn't fill them out
    // Visited rooms are shown filled out
    bool drawFully = true;
    if (dStageMgr_c::GetInstance()->isAreaTypeDungeonOrBoss()) {
        drawFully = dStage_c::GetInstance()->hasVisitedRoom(roomid);
        if (!drawFully && !dAcItem_c::checkFlag(ITEM_DUNGEON_MAP_FI)) {
            return;
        }
    } else {
        // Lake Floria interior
        if (dStageMgr_c::GetInstance()->isAreaTypeOverworldOrSkyloft() && dScGame_c::isCurrentStage("F102") &&
            !StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_FLORIA_INTERIOR_MAP_UNLOCKED) && roomid >= 3 &&
            roomid < 5) {
            return;
        }
    }

    dBgS::GetInstance()->DrawMap(roomid, mtx, drawFully, param);
}

void dRoom_c::getBounds(mVec3_c *min, mVec3_c *max) const {
    for (int i = 0; i < ROOM_NUM_MODELS; i++) {
        mVec3_c tMin, tMax;
        mModels[i].getBounds(&tMin, &tMax);
        if (i == 0) {
            *min = tMin;
            *max = tMax;
        } else {
            nw4r::math::VEC3Minimize(min, min, tMin);
            nw4r::math::VEC3Maximize(max, max, tMax);
        }
    }
}

void dRoom_c::formatObj(int obj, SizedString<8> &str) {
    str.sprintf("obj%d", obj);
}

void dRoom_c::updateObjNodeInEachRoom(int obj, bool visible) {
    SizedString<8> objName;
    formatObj(obj, objName);
    for (int i = 0; i < ROOM_NUM_MODELS; i++) {
        mModels[i].updateObjNode(objName, visible);
    }
}

void dRoom_c::destroyModels() {
    for (int i = 0; i < ROOM_NUM_MODELS; i++) {
        mModels[i].destroy();
    }
}

// Called from d_a_lastboss
void dRoom_c::someLastBossThing(bool arg) {
    const char *name = "model0";
    if (!arg) {
        name = "model_obj0";
    }
    nw4r::g3d::ResAnmTexSrt anmSrt = mRoomRes.GetResAnmTexSrt(name);
    if (anmSrt.IsValid()) {
        mModels[0].mpAnmSrt->setAnm(mModels[0].mMdl, anmSrt, 0, m3d::PLAY_MODE_4);
        mModels[0].mMdl.setAnm(*mModels[0].mpAnmSrt);
    }
}

f32 dRoom_c::getFrame() const {
    return mModels[0].getFrame();
}

void dRoom_c::initializeState_Active() {
    SceneflagManager::sInstance->unsetZoneflagsForRoom(roomid);
    executeBg();
    parseRoomActivateBzs(roomid, BZS);
    if (mDidAlreadyInit) {
        parseRoomReactivateBzs(roomid, BZS);
    } else {
        mFlags |= 8 | 2;
        mDidAlreadyInit = true;
    }
}

void dRoom_c::executeState_Active() {
    u8 flags = mFlags;
    field_0x571 = (flags & (4 | 2)) != 0;
    if (field_0x571) {
        if ((flags & 8) != 0) {
            mStateMgr.changeState(StateID_NonActive);
        } else {
            deactivateUpdates();
        }
        return;
    }

    activateUpdates();
}

void dRoom_c::finalizeState_Active() {}

void deleteActor(dAcBase_c *ac) {
    ac->setActorProperty(dAcBase_c::AC_PROP_0x800);
    ac->deleteRequest();
}

void dRoom_c::initializeState_NonActive() {
    foreachObject(deleteActor);
    if (dTgMassObj_c::GetInstance() != nullptr) {
        dTgMassObj_c::GetInstance()->unloadRoom(roomid);
    }
}

void dRoom_c::executeState_NonActive() {
    if ((mFlags & 8) != 0) {
        return;
    }
    mStateMgr.changeState(StateID_Active);
}

void dRoom_c::finalizeState_NonActive() {
    mFlags = mFlags & ~8;
    destroyModels();
}

void dRoom_c::mdl_c::remove() {
    if (mpAabb != nullptr) {
        delete[] mpAabb;
        mpAabb = nullptr;
        doSomethingWithVis(true);
    }
}

bool dRoom_c::mdl_c::create(nw4r::g3d::ResMdl resMdl, mAllocator_c &alloc) {
    if (!m3d::smdl_c::create(resMdl, &alloc, 0x40)) {
        return false;
    }
    calc(false);
    u32 numEntries = resMdl.GetResNodeNumEntries();
    mpAabb = new nw4r::math::AABB[numEntries];

    if (mpAabb == nullptr) {
        return false;
    }

    nw4r::math::AABB *buf = mpAabb;
    u32 id = 0;
    if (numEntries != 0) {
        for (; id < numEntries; id++) {
            mMtx_c mtx;
            getNodeWorldMtx(id, mtx);
            nw4r::g3d::ResNode resNd = resMdl.GetResNode(id);
            mVec3_c min, max;
            min.copyFrom(resNd.GetBoundsMin());
            max.copyFrom(resNd.GetBoundsMax());
            nw4r::math::AABB bb(min, max);
            buf->Set(&bb, mtx);
            buf++;
        }
    }

    doSomethingWithVis(false);
    return true;
}

extern "C" bool fn_801BB750(nw4r::math::AABB *aabb, f32 f);

void dRoom_c::mdl_c::configureSomething() {
    nw4r::g3d::ResMdl mdl = getResMdl();
    u32 numNodes = mdl.GetResNodeNumEntries();

    nw4r::math::AABB *aabbs = mpAabb;
    for (u32 i = 0; i < numNodes; i++) {
        nw4r::g3d::ResNode nd = mdl.GetResNode(i);
        if (nd.IsVisible() && (nd.ref().flags & nw4r::g3d::ResNodeData::FLAG_GEOMETRY) &&
            nd.GetBillboardMode() == nw4r::g3d::ResNodeDataTypedef::BILLBOARD_OFF) {
            somethingVisibility(i, !fn_801BB750(&aabbs[i], EGG::Math<f32>::maxNumber()));
        }
    }
}

void dRoom_c::mdl_c::somethingVisibility(u32 id, bool visible) {
    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdl>(getG3dObject());
    nw4r::g3d::ScnMdl::CopiedVisAccess v(mdl, id);
    v.SetVisibility(visible);
}

void dRoom_c::mdl_c::doSomethingWithVis(bool arg) {
    nw4r::g3d::ResMdl mdl = getResMdl();
    const char *mdlName = mdl.GetName();
    if (strequals(mdlName, "model0_s") && dScGame_c::isStageSkyloftLayer20()) {
        nw4r::g3d::ResMat mat = mdl.GetResMat("MA03_C_Door_Kasan");
        setMatVisible(mat.GetID(), arg);
    }
}

bool dRoom_c::model_c::create(nw4r::g3d::ResFile resFile, mAllocator_c &alloc, s32 idx, d3d::UnkWithWater *waterThing) {
    SizedString<16> mdlName;
    mdlName.sprintf("model%d", idx >> 1);
    if ((idx & 1) != 0) {
        mdlName += "_s";
    }

    nw4r::g3d::ResMdl mdl = resFile.GetResMdl(mdlName);
    if (!mdl.IsValid()) {
        return true;
    }

    bool linkResult = d3d::UnkWithWater::linkMdl(mdl, waterThing);
    if (!mMdl.create(mdl, alloc)) {
        return false;
    }

    const DrawPriorityConfig *cfg = getDrawPriority(idx);
    u8 xlu = linkResult != 0 ? dStageMgr_c::GetInstance()->checkFlag0x88A0(0x2) ? cfg->pDrawXlu : 0x84 : cfg->pDrawXlu;
    mMdl.setPriorityDraw(cfg->pDrawOpa1, xlu);
    bool anyAnims = false;
    nw4r::g3d::ResAnmClr anmClr = resFile.GetResAnmClr(mdlName);
    if (anmClr.IsValid()) {
        mpAnmClr = new m3d::anmMatClr_c();
        if (!mpAnmClr) {
            return false;
        }
        if (!mpAnmClr->create(mdl, anmClr, &alloc, nullptr, 1)) {
            return false;
        }
        mMdl.setAnm(*mpAnmClr);
        anyAnims = true;
    }

    nw4r::g3d::ResAnmTexPat anmPat = resFile.GetResAnmTexPat(mdlName);
    if (anmPat.IsValid()) {
        mpAnmPat = new m3d::anmTexPat_c();
        if (!mpAnmPat) {
            return false;
        }
        if (!mpAnmPat->create(mdl, anmPat, &alloc, nullptr, 1)) {
            return false;
        }
        mMdl.setAnm(*mpAnmPat);
        anyAnims = true;
    }

    nw4r::g3d::ResAnmTexSrt anmSrt = resFile.GetResAnmTexSrt(mdlName);
    if (!anmSrt.IsValid()) {
        dStage_c::getMA0IndirectSrt(&anmSrt, mdl);
    }
    if (anmSrt.IsValid()) {
        mpAnmSrt = new m3d::anmTexSrt_c();
        if (!mpAnmSrt) {
            return false;
        }
        if (!mpAnmSrt->create(mdl, anmSrt, &alloc, nullptr, 1)) {
            return false;
        }
        mMdl.setAnm(*mpAnmSrt);
        anyAnims = true;
    }

    nw4r::g3d::ResAnmVis anmVis = resFile.GetResAnmVis(mdlName);
    if (anmVis.IsValid()) {
        mpAnmVis = new m3d::anmVis_c();
        if (!mpAnmVis) {
            return false;
        }
        if (!mpAnmVis->create(mdl, anmVis, &alloc, nullptr)) {
            return false;
        }
        mMdl.setAnm(*mpAnmVis);
        anyAnims = true;
    }

    if (anyAnims) {
        mMdl.setOption(nw4r::g3d::ScnObj::OPTION_DISABLE_CALC_WORLD, false);
    }

    return true;
}

void dRoom_c::model_c::execute(s32 idx, bool canHavePastState, f32 pastState) {
    if (!mMdl.hasModel()) {
        return;
    }

    if (mpAnmClr) {
        mpAnmClr->play();
    }

    if (mpAnmSrt) {
        mpAnmSrt->play();
    }

    if (mpAnmVis) {
        mpAnmVis->play();
    }

    if (canHavePastState && !(idx >= 6)) {
        if (mpAnmPat) {
            int frame = (pastState > 0.0f);
            mpAnmPat->setFrame(frame, 0);
        } else {
            const DrawPriorityConfig *cfg = getDrawPriority(idx);
            if (pastState > 0.0f) {
                mMdl.setPriorityDraw(cfg->pDrawOpa2, cfg->pDrawXlu);
            } else {
                mMdl.setPriorityDraw(cfg->pDrawOpa1, cfg->pDrawXlu);
            }
        }
    } else {
        if (mpAnmPat) {
            mpAnmPat->play();
        }
    }
}

void dRoom_c::model_c::draw(int roomid) {
    if (!mMdl.hasModel()) {
        return;
    }
    mMdl.configureSomething();
    configureSomething(roomid, &mMdl);
    mMdl.entry();
}

void dRoom_c::model_c::configureSomething(int roomid, mdl_c *mdl) {
    nw4r::g3d::ResMdl resMdl = mdl->getResMdl();
    d3d::setRoomTevColors(resMdl, true, false);
}

const DrawPriorityConfig cfg[] = {
    {0x18, 0x5, 0x7},
    {0x1E, 0x5, 0x9},
    {0x19, 0x6, 0x7},
    {0x1F, 0x6, 0x9},
    {0x1A, 0x7, 0x7},
    {0x20, 0x7, 0x9},
    {0x1B, 0x8, 0x7},
    {0x21, 0x8, 0x9},
};

const DrawPriorityConfig *dRoom_c::model_c::getDrawPriority(s32 idx) {
    return &cfg[idx];
}

bool dRoom_c::model_c::getBounds(mVec3_c *min, mVec3_c *max) const {
    return mMdl.getBounds(min, max);
}

nw4r::g3d::ResNode dRoom_c::model_c::getResNode(const char *nodeName) {
    return mMdl.getResMdl().GetResNode(nodeName);
}

void dRoom_c::model_c::updateObjNode(const char *node, bool visible) {
    if (!mMdl.hasModel()) {
        return;
    }

    nw4r::g3d::ResNode nd = getResNode(node);
    if (!nd.IsValid()) {
        return;
    }

    nd.SetVisibility(visible);
    mMdl.somethingVisibility(nd.GetID(), visible);
    if (visible) {
        dTimeAreaMgr_c::GetInstance()->setField0x7A(true);
    } else {
        dTimeAreaMgr_c::GetInstance()->setField0x7A(false);
    }
}

void dRoom_c::model_c::destroy() {
    if (!mMdl.hasModel()) {
        return;
    }

    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdl>(mMdl.getG3dObject());
    nw4r::g3d::ResMdl resMdl = mMdl.getResMdl();
    for (u32 i = 0; i < resMdl.GetResNodeNumEntries(); i++) {
        nw4r::g3d::ResNode nd = resMdl.GetResNode(i);
        nd.SetVisibility(true);
        nw4r::g3d::ScnMdl::CopiedVisAccess v(mdl, i);
        v.SetVisibility(true);
    }
}

f32 dRoom_c::model_c::getFrame() const {
    if (!mMdl.hasModel() || !mpAnmSrt) {
        return 0.0f;
    }
    return mpAnmSrt->getFrame(0);
}
