#include "d/a/obj/d_a_obj_under_cloud.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/d_light_env.h"
#include "d/d_stage.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_UNDER_CLOUD, dAcOunderCloud_c, fProfile::OBJ_UNDER_CLOUD, 0x202, 0, 4);

bool dAcOunderCloud_c::createHeap() {
    void *data = getOarcResFile("F020UnderCloud");
    mResFile = nw4r::g3d::ResFile(data);
    dStage_c::bindStageResToFile(&mResFile);
    dStage_c::bindSkyCmnToResFile(&mResFile);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("F020UnderCloud");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x160));
    nw4r::g3d::ResAnmTexSrt anm = mResFile.GetResAnmTexSrt("F020UnderCloud");
    TRY_CREATE(mAnm.create(mdl, anm, &mAllocator, nullptr, 1));
    mMdl.setOption(0x30001, false);
    return true;
}

int dAcOunderCloud_c::create() {
    CREATE_ALLOCATOR(dAcOunderCloud_c);
    mMdl.setAnm(mAnm);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.setPriorityDraw(0x1C, 9);
    boundingBox.Set(mVec3_c(-550000.0f, -30000.0f, -500000.0f), mVec3_c(500000.0f, -10000.0f, 500000.0f));
    return SUCCEEDED;
}

int dAcOunderCloud_c::doDelete() {
    return SUCCEEDED;
}

int dAcOunderCloud_c::actorExecute() {
    mAnm.play();
    return SUCCEEDED;
}

int dAcOunderCloud_c::actorExecuteInEvent() {
    nw4r::g3d::ScnMdl *mdl = nw4r::g3d::ScnObj::DynamicCast<nw4r::g3d::ScnMdl>(mMdl.getG3dObject());
    nw4r::g3d::ScnMdl::CopiedVisAccess visAccess(mdl, 1);
    switch (mEvent.getCurrentEventCommand()) {
        case 'act0': {
            actorExecute();
            mEvent.advanceNext();
            break;
        };
        case 'act1': {
            actorExecute();
            visAccess.SetVisibility(false);
            break;
        };
        case '????': {
            actorExecute();
            break;
        };
        default: {
            mEvent.advanceNext();
        };
    }
    return SUCCEEDED;
}

int dAcOunderCloud_c::draw() {
    dLightEnv_c &lightEnv = dLightEnv_c::GetInstance();
    if (lightEnv.fn_80024770(6) && !lightEnv.fn_800247A0(6)) {
        return SUCCEEDED;
    }
    drawModelType1(&mMdl);
    return SUCCEEDED;
}
