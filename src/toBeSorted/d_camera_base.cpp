#include "toBeSorted/d_camera_base.h"

#include "common.h"

// All of these base functions and the vtable got stripped
bool dCameraBase_c::create() {
    return true;
}

bool dCameraBase_c::remove() {
    return true;
}

bool dCameraBase_c::execute() {
    return true;
}

bool dCameraBase_c::draw() {
    return true;
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
