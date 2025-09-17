#include "d/a/obj/d_a_obj_amber.h"

#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "f/f_base.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "sized_string.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(OBJ_AMBER, dAcOAmber_c, fProfile::OBJ_AMBER, 0x284, 0, 3);

bool dAcOAmber_c::createHeap() {
    void *data = getOarcResFile("Amber");
    if (data == nullptr) {
        return false;
    }

    nw4r::g3d::ResFile resFile(data);
    if (!resFile.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdl = resFile.GetResMdl("Amber");
    if (!mdl.IsValid()) {
        return false;
    }

    return mMdl.create(mdl, &heap_allocator, 0x120, 1, nullptr);
}

int dAcOAmber_c::actorCreate() {
    return SUCCEEDED;
}

int dAcOAmber_c::actorPostCreate() {
    CREATE_ALLOCATOR(dAcOAmber_c);
    if (dBgS_ObjGndChk::CheckPos(mPosition + mVec3_c::Ey * 10.0f)) {
        mLightingInfo.mLightingCode = dBgS_ObjGndChk::GetLightingCode();
        if (&dBgS_ObjGndChk::GetInstance() != nullptr) {
            setPolyAttrs(dBgS_ObjGndChk::GetInstance());
        }
    }
    mMdl.getBounds(&mMin, &mMax);
    boundingBox.Set(mMin, mMax);
    mShadowRot.v = (mMin + mMax) * 0.5f;
    mShadowRot.w = ((mMax - mMin) * 0.5f).mag();
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mVec3_c chkPos = mPosition + mVec3_c::Ey * mMax.y;
    if (dBgS_ObjGndChk::CheckPos(chkPos)) {
        f32 deltaHeight = mPosition.y - dBgS_ObjGndChk::GetGroundHeight();
        field_0x37c = deltaHeight < 0.0f ? 0.0f : deltaHeight;
    } else {
        field_0x37c = 1.0e+9f;
    }
    return SUCCEEDED;
}

int dAcOAmber_c::draw() {
    drawModelType1(&mMdl);
    if (field_0x37c != 1.0e+9f) {
        drawShadow(mShadowCircle, nullptr, mWorldMtx, &mShadowRot, -1, -1, -1, -1, -1, field_0x37c);
    }
    return SUCCEEDED;
}

void dAcOAmber_c::registerInEvent() {
    const char *eventName = EventManager::sInstance->getCurrentEventName();
    if (strequals(eventName, "TimeDoorIn") || strequals(eventName, "TimeDoorOut")) {
        mObjectActorFlags |= 0x200;
    }
}

void dAcOAmber_c::unkVirtFunc_0x6C() {
    mObjectActorFlags &= ~0x200;
}
