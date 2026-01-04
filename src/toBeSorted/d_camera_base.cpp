#include "toBeSorted/d_camera_base.h"

#include "common.h"

// TODO: Weak function order (CamView dtor needs to appear after a bunch of weak functions)
// TODO: I'm not sure why all those weak functions even got emitted here, there's nothing
// that forces the compiler to put anything here so using this as a hack. I really believe the
// text split is correct, and this file doesn't appear to have any other sections.
void CreatedCameraBase_cHack() {
    dCameraBase_c *c = new dCameraBase_c();
}

bool dCameraBase_c::doCreate(s32 index) {
    if (!mCreated) {
        if (!create()) {
            return false;
        }
        mCreated = true;
        mIndex = index;
    }
    return true;
}

bool dCameraBase_c::doRemove() {
    field_0x01 = false;
    return remove();
}

bool dCameraBase_c::doExecute() {
    if (field_0x02 && !field_0x01) {
        vt_0x08();
    } else if (!field_0x02 && field_0x01) {
        vt_0x0C();
    }
    field_0x01 = field_0x02;
    if (!field_0x02) {
        return false;
    } else {
        return execute();
    }
}

bool dCameraBase_c::doDraw() {
    if (field_0x01) {
        return draw();
    }
    return false;
}
