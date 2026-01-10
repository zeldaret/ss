#ifndef TO_BE_SORTED_D_CAMERA_BASE_H
#define TO_BE_SORTED_D_CAMERA_BASE_H

#include "common.h"
#include "m/m_vec.h"

// A basic description of a camera perspective, simplified.
struct CamView {
    CamView() : mPosition(0.0f, 0.0f, 0.0f), mTarget(0.0f, 0.0f, 1.0f), mFov(50.0f), mTilt(0.0f) {}
    CamView(const mVec3_c &pos, const mVec3_c &target, f32 fov, f32 tilt) : mPosition(pos), mTarget(target), mFov(fov), mTilt(tilt) {}
    ~CamView() {}

    /* 0x00 */ mVec3_c mPosition;
    /* 0x0C */ mVec3_c mTarget;
    /* 0x18 */ f32 mFov;
    /* 0x1C */ f32 mTilt;
};

// Abstract camera base class. The game camera class will hold various subclasses
// of this base class.
class dCameraBase_c {
protected:
    /* 0x00 */ bool mCreated;
    /* 0x01 */ u8 mPrevIsActive;
    /* 0x02 */ bool mIsActive;
    /* 0x04 */ s32 mIndex;
    /* 0x08 */ CamView mView;

public:
    dCameraBase_c() : mCreated(false), mPrevIsActive(0), mIsActive(false), mIndex(-1) {}

    bool doCreate(s32 index);
    bool doRemove();
    bool doExecute();
    bool doDraw();

    // vt at 0x28
    /* vt 0x08 */ virtual void onBecomeActive() {}
    /* vt 0x0C */ virtual void onBecomeInactive() {}
    /* vt 0x10 */ virtual bool create();
    /* vt 0x14 */ virtual bool remove();
    /* vt 0x18 */ virtual bool execute();
    /* vt 0x1C */ virtual bool draw();
    /* vt 0x20 */ virtual ~dCameraBase_c() {}
    /* vt 0x24 */ virtual CamView &getView() {
        // TODO - const?
        return mView;
    }
    /* vt 0x28 */ virtual void activate() {
        mIsActive = true;
    }
    /* vt 0x2C */ virtual void deactivate() {
        mIsActive = false;
    }
    /* vt 0x30 */ virtual void setView(const CamView &view) {
        // TODO - is this actually a base impl?
        mView = view;
    }
};

#endif
