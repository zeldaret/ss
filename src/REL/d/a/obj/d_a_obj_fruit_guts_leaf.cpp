#include "d/a/obj/d_a_obj_fruit_guts_leaf.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(OBJ_FRUIT_GUTS_LEAF, dAcOFruitGutsLeaf_c, fProfile::OBJ_FRUIT_GUTS_LEAF, 0x2E, 0, 3);

static const char *const mdlFileName = "PutGuts";
static const char *const mdlName = "PutGutsLeaf";

const mVec3_c bbMin(-50.0f, 0.0f, -50.0f);
const mVec3_c bbMax(50.0f, 15.0f, 50.0f);

bool dAcOFruitGutsLeaf_c::createHeap() {
    void *data = getOarcResFile(mdlFileName);
    if (data == nullptr) {
        return false;
    }
    nw4r::g3d::ResFile f(data);
    if (!f.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl m = f.GetResMdl(mdlName);

    if (!m.IsValid()) {
        return false;
    }

    TRY_CREATE(mMdl.create(m, &heap_allocator, 0x120));

    return true;
}

int dAcOFruitGutsLeaf_c::actorCreate() {
    CREATE_ALLOCATOR(dAcOFruitGutsLeaf_c);
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);

    if (getType() == 0) {
        mMdl.setPriorityDraw(0x82, 0x7F);
    } else {
        mMdl.setPriorityDraw(0x7F, 0x7F);
    }

    boundingBox.Set(bbMin, bbMax);

    return SUCCEEDED;
}

int dAcOFruitGutsLeaf_c::actorPostCreate() {
    if (hideInPast() && !dTimeAreaMgr_c::sInstance->fn_800B9B60(roomid, position)) {
        mScale.x = 0.0f;
        mScale.y = 0.0f;
        mScale.z = 0.0f;
    } else {
        mScale.x = 1.0f;
        mScale.y = 1.0f;
        mScale.z = 1.0f;
    }
    mMdl.setScale(mScale);
    return SUCCEEDED;
}
int dAcOFruitGutsLeaf_c::actorExecute() {
    if (hideInPast()) {
        if (dTimeAreaMgr_c::sInstance->fn_800B9B60(roomid, position)) {
            if (mScale.x != 1.0f) {
                sLib::chase(&mScale.x, 1.0f, 0.05f);
                mScale.z = mScale.x;
                mScale.y = mScale.x;
                mMdl.setScale(mScale);
            }
        } else {
            if (mScale.y != 0.0f) {
                sLib::chase(&mScale.x, 0.0f, 0.05f);
                mScale.z = mScale.x;
                mScale.y = mScale.x;
                mMdl.setScale(mScale);
            }
        }
    }

    return SUCCEEDED;
}

int dAcOFruitGutsLeaf_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

bool dAcOFruitGutsLeaf_c::hideInPast() {
    return getPastBehavior() == 0;
}

s32 dAcOFruitGutsLeaf_c::getType() {
    return params & 0xFF;
}

s32 dAcOFruitGutsLeaf_c::getPastBehavior() {
    return (params >> 8) & 1;
}
