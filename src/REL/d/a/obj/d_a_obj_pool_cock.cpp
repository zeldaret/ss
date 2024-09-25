#include <d/a/obj/d_a_obj_pool_cock.h>
#include <d/a/obj/d_a_obj_vortex.h>
#include <s/s_Math.h>
#include <toBeSorted/sceneflag_manager.h>

SPECIAL_ACTOR_PROFILE(OBJ_POOL_COCK, dAcOPoolCock_c, fProfile::OBJ_POOL_COCK, 0x024D, 0, 7);

STATE_DEFINE(dAcOPoolCock_c, Wait);

bool dAcOPoolCock_c::createHeap() {
    mBrres = getOarcResFile("WaterD101");
    RoomManager::bindStageResToFile(&mBrres);
    nw4r::g3d::ResMdl mdl = mBrres.GetResMdl("PoolCockD101");
    for (int i = 0; i < 2; i++) {
        TRY_CREATE(mModels[i].create(mdl, &heap_allocator, 0x120));
    }
    return true;
}

int dAcOPoolCock_c::actorCreate() {
    CREATE_ALLOCATOR(dAcOPoolCock_c);

    forwardAccel = -0.0f;
    forwardMaxSpeed = -40.0f;
    mOpenSceneflag = params & 0xFF;
    mOpenDirection = mVec3_c::Ez;
    mOpenDirection.rotY(rotation.y);

    if (mOpenSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mOpenSceneflag)) {
        mOpenProgress = 400.0f;
    }

    mStateMgr.changeState(StateID_Wait);

    setBoundingBox(mVec3_c(-300.0f, -100.0f, -300.0f), mVec3_c(300.0f, 100.0f, 300.0f));
    return SUCCEEDED;
}

int dAcOPoolCock_c::actorPostCreate() {
    return SUCCEEDED;
}

int dAcOPoolCock_c::doDelete() {
    return SUCCEEDED;
}

int dAcOPoolCock_c::actorExecute() {
    mStateMgr.executeState();
    calcVelocity();
    position += velocity;
    position += posIncrements;
    updateMatrix();
    mMtx_c mdl1Transform;
    mMtx_c mdl2Transform;
    mdl1Transform = worldMatrix;
    mdl2Transform = worldMatrix;

    mMtx_c tmp1;
    mMtx_c tmp2;

    mVec3_c translation = mOpenDirection * mOpenProgress;
    PSMTXTrans(tmp1, translation.x, translation.y, translation.z);
    PSMTXConcat(mdl1Transform, tmp1, mdl1Transform);

    mVec3_c translation2 = -translation;
    PSMTXTrans(tmp2, translation2.x, translation2.y, translation2.z);
    PSMTXConcat(mdl2Transform, tmp2, mdl2Transform);

    mdl2Transform.YrotM(-0x8000);
    mModels[0].setLocalMtx(mdl1Transform);
    mModels[1].setLocalMtx(mdl2Transform);

    return SUCCEEDED;
}

int dAcOPoolCock_c::draw() {
    if (mOpenProgress < 400.0f) {
        drawModelType1(&mModels[0]);
        drawModelType1(&mModels[1]);
    }
    return SUCCEEDED;
}

void dAcOPoolCock_c::initializeState_Wait() {}

void dAcOPoolCock_c::executeState_Wait() {
    if (mOpenSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mOpenSceneflag)) {
        sLib::chase(&mOpenProgress, 400.0f, 5.0f);
        if (!mHasActivatedVortex) {
            dAcObjVortex_c *vortex = (dAcObjVortex_c *)fManager_c::searchBaseByProfName(fProfile::OBJ_VORTEX, nullptr);
            if (vortex) {
                vortex->actor_properties |= 0x4;
                vortex->field_0x870 = 1;
            }
            mHasActivatedVortex = true;
        }
    }
}

void dAcOPoolCock_c::finalizeState_Wait() {}
