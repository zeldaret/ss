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
    mPrevIsActive = false;
    return remove();
}

bool dCameraBase_c::doExecute() {
    if (mIsActive && !mPrevIsActive) {
        onBecomeActive();
    } else if (!mIsActive && mPrevIsActive) {
        onBecomeInactive();
    }
    mPrevIsActive = mIsActive;
    if (!mIsActive) {
        return false;
    } else {
        return execute();
    }
}

bool dCameraBase_c::doDraw() {
    if (mPrevIsActive) {
        return draw();
    }
    return false;
}
