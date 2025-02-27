#include "d/a/obj/d_a_obj_time_stage_bg.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "m/m_color.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_Math.h"
#include "toBeSorted/arc_managers/current_stage_arc_manager.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(OBJ_TIME_STAGE_BG, dAcOTimeStageBg_c, fProfile::OBJ_TIME_STAGE_BG, 0x25B, 0, 6);

static const char *sMdlNames[] = {
    "TeniObj_0", "TeniObj_1", "TeniObj_2", "TeniObj_3", "Teniobj_0",
};

static const char *sMdl2Names[] = {
    "TeniObj_0", "TeniObj_1", "TeniObj_2G", "TeniObj_3G", "Teniobj_0",
};

STATE_DEFINE(dAcOTimeStageBg_c, Wait);

bool dAcOTimeStageBg_c::createHeap() {
    mRes = nw4r::g3d::ResFile(CurrentStageArcManager::sInstance->getData("g3d/stage.brres"));
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl(sMdlNames[mSubType]);
    TRY_CREATE(mMdl1.create(mdl, &heap_allocator, 0x128));
    nw4r::g3d::ResNode nd = mdl.GetResNode(sMdlNames[mSubType]);

    field_0x3EC.copyFrom((nd.ref().volume_min + nd.ref().volume_max) * 0.5f);
    if (mSubType == 4) {
        nw4r::g3d::ResAnmClr a = mRes.GetResAnmClr("Teniobj_0");
        TRY_CREATE(mAnm.create(mdl, a, &heap_allocator, nullptr, 1));
        mMdl1.setAnm(mAnm);
    }

    if (secondMdl()) {
        mdl = mRes.GetResMdl(sMdl2Names[mSubType]);
        TRY_CREATE(mMdl2.create(mdl, &heap_allocator, 0x120));
    }
    return true;
}

int dAcOTimeStageBg_c::actorCreate() {
    mSubType = params & 0xF;
    if (mSubType > 5) {
        mSubType = 0;
    }
    CREATE_ALLOCATOR(dAcOTimeStageBg_c);

    forwardAccel = 0.0f;
    forwardMaxSpeed = -40.0f;
    updateMatrix();
    mSceneCallback.attach(mMdl1);
    mMdl1.setLocalMtx(mWorldMtx);
    mMdl1.calc(true);
    mVec3_c v;
    mMdl1.getNodeWorldMtxMultVecZero(0, v);
    field_0x3EC += v;
    field_0x3EC.y = 0.0f;
    mMdl1.setPriorityDraw(0x1C, 9);

    if (secondMdl()) {
        mMdl2.setLocalMtx(mWorldMtx);
        mMdl2.setPriorityDraw(0x1C, 9);
    }

    mVec3_c min, max;
    mMdl1.getBounds(&min, &max);
    mStateMgr.changeState(StateID_Wait);
    boundingBox.Set(min, max);

    return SUCCEEDED;
}

int dAcOTimeStageBg_c::actorPostCreate() {
    mTimeArea.check(roomid, field_0x3EC, 0, 30.0f, 0.1f);
    if (dTimeAreaMgr_c::sInstance->fn_800B9B60(roomid, field_0x3EC)) {
        field_0x3F8 = 255.0f;
    } else {
        field_0x3F8 = 0.0f;
    }
    return SUCCEEDED;
}

int dAcOTimeStageBg_c::doDelete() {
    return SUCCEEDED;
}

int dAcOTimeStageBg_c::actorExecute() {
    mStateMgr.executeState();
    if (mSubType == 4) {
        mAnm.play();
    }
    return SUCCEEDED;
}

int dAcOTimeStageBg_c::draw() {
    if (field_0x3F8 > 0.0f) {
        if (!secondMdl() || field_0x3F8 != 255.0f) {
            drawModelType1(&mMdl1);
        }
    }

    if (secondMdl() && field_0x3F8 == 255.0f) {
        drawModelType1(&mMdl2);
    }
    return SUCCEEDED;
}

void dAcOTimeStageBg_c::initializeState_Wait() {}
void dAcOTimeStageBg_c::executeState_Wait() {
    f32 target = 0.0f;
    mTimeArea.check(roomid, field_0x3EC, 0, 30.0f, 0.1f);
    if (dTimeAreaMgr_c::sInstance->fn_800B9B60(roomid, field_0x3EC)) {
        if (mTimeArea.getDistMaybe() == 1.0f) {
            target = 255.0f;
        }
    }
    sLib::chase(&field_0x3F8, target, 8.5f);
    mMdl1.setTevKColorAll(GX_KCOLOR1, mColor(0, 0, 0, field_0x3F8), false);
}
void dAcOTimeStageBg_c::finalizeState_Wait() {}
