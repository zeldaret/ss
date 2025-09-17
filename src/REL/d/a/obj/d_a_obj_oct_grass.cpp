#include "d/a/obj/d_a_obj_oct_grass.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_stage.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_OCT_GRASS, dAcOOctGrass_c, fProfile::OBJ_OCT_GRASS, 0x236, 0, 3);

STATE_DEFINE(dAcOOctGrass_c, Wait);

const char *dAcOOctGrass_c::GRASS_OARCS[4] = {
    "GrassOcta",
    "GrassRollGrow",
    "GrassGerm",
    "GrassMain",
};

const char *dAcOOctGrass_c::GRASS_CUT_MODELS[4] = {
    "GrassOctaCut",
    "GrassRollCut",
    "GrassGermCut",
    "GrassMainCut",
};

bool dAcOOctGrass_c::createHeap() {
    u8 typeIdx = getFromParams(0, 0xF);
    void *data = getOarcResFile(GRASS_OARCS[typeIdx]);
    mResFile = nw4r::g3d::ResFile(data);
    if (typeIdx == 3) {
        dStage_c::bindSkyCmnToResFile(&mResFile);
    }
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl(GRASS_CUT_MODELS[typeIdx]);
    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120, 1, nullptr));
    return true;
}

int dAcOOctGrass_c::create() {
    CREATE_ALLOCATOR(dAcOOctGrass_c);
    forwardAccel = 0.0f;
    forwardMaxSpeed = -40.0f;
    if ((mParams & 0xF) != 0) {
        mMdl.setPriorityDraw(0x1C, 9);
    }
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    spawnOcGrsL();
    poscopy2 = mPosition;
    poscopy3 = mPosition;
    poscopy3.y += 20.0f;
    mStateMgr.changeState(StateID_Wait);
    boundingBox.Set(mVec3_c(-50.0f, -10.0f, -50.0f), mVec3_c(50.0f, 100.0f, 50.0f));
    return SUCCEEDED;
}

int dAcOOctGrass_c::doDelete() {
    return SUCCEEDED;
}

int dAcOOctGrass_c::actorExecute() {
    return SUCCEEDED;
}

int dAcOOctGrass_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOOctGrass_c::initializeState_Wait() {}
void dAcOOctGrass_c::executeState_Wait() {}
void dAcOOctGrass_c::finalizeState_Wait() {}

void dAcOOctGrass_c::spawnOcGrsL() {
    u32 type = getFromParams(0, 0xF);
    if (type == 0xF) {
        type = 0;
    }

    dAcObjBase_c::create(
        "OcGrsL", getRoomId(), type & 0xF, &mPosition, nullptr, nullptr, getParams2_ignoreLower(), 0xFFFF,
        viewclip_index
    );
}
