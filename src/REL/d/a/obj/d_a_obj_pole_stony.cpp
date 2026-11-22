#include "d/a/obj/d_a_obj_pole_stony.h"
#include "common.h"
#include "d/d_heap_alloc.h"
#include "f/f_base.h"
#include "m/m3d/m_smdl.h"

#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_POLE_STONY, dAcOPoleStony_c, fProfile::OBJ_POLE_STONY, 0x272, 0, 2);

int dAcOPoleStony_c::draw()
{
    for (int i = 0; i <= 3; i++)
    {
        drawModel(&field_0x330[i]);
    }
    return SUCCEEDED;
}

bool dAcOPoleStony_c::createHeap() {
    void *data = getOarcResFile("Pole");
    if (data == nullptr) {
        return false;
    }

    nw4r::g3d::ResFile file(data);
    if (!file.IsValid()) {
        return false;
    }

    nw4r::g3d::ResMdl mdlHead = file.GetResMdl("PoleHead");
    if (!mdlHead.IsValid()) {
        return false;
    }

    if (!field_0x330[0].create(mdlHead, &mAllocator, 0x120,1,0)) {
        return false;
    }

    nw4r::g3d::ResMdl mdlBody = file.GetResMdl("PoleBody");
    if (!mdlBody.IsValid()) {
        return false;
    }

    for(UINT8 i = 1; i <= 3; i++) {
        if(!field_0x330[i].create(mdlBody, &mAllocator, 0x120, 1, 0)) {
            return false;
        }
    }
    
    return true;
}

void fn_307_180() {
    
}

dAcOPoleStony_c* fn_307_70(dAcOPoleStony_c* param_1,int param_2)
{
  if ((param_1 != nullptr) && (param_2 > 0)) {
    operator delete[](param_1);
  }
  return param_1;
}
