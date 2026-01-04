#ifndef TO_BE_SORTED_D_CAMERA_BASE_H
#define TO_BE_SORTED_D_CAMERA_BASE_H

#include "common.h"
#include "m/m_vec.h"

// A basic description of a camera perspective, with
// position, target point, a field of view, and something else.
struct CamView {
    CamView() : field_0x00(0.0f, 0.0f, 0.0f), field_0x0C(0.0f, 0.0f, 1.0f), mFov(50.0f), field_0x1C(0.0f) {}
    ~CamView() {}

    /* 0x00 */ mVec3_c field_0x00;
    /* 0x0C */ mVec3_c field_0x0C;
    /* 0x18 */ f32 mFov;
    /* 0x1C */ f32 field_0x1C;
};

// Abstract camera base class. The game camera class will hold various subclasses
// of this base class.
class dCameraBase_c {
protected:
    /* 0x00 */ bool mCreated;
    /* 0x01 */ u8 field_0x01;
    /* 0x02 */ u8 field_0x02;
    /* 0x04 */ s32 mIndex;
    /* 0x08 */ CamView mView;

public:
    dCameraBase_c() : mCreated(false), field_0x01(0), field_0x02(0), mIndex(-1) {}

    bool doCreate(s32 index);
    bool doRemove();
    bool doExecute();
    bool doDraw();

    // vt at 0x28
    /* vt 0x08 */ virtual void vt_0x08() {}
    /* vt 0x0C */ virtual void vt_0x0C() {}
    /* vt 0x10 */ virtual bool create() {
        return true;
    }
    /* vt 0x14 */ virtual bool remove() {
        return true;
    }
    /* vt 0x18 */ virtual bool execute() {
        return true;
    }
    /* vt 0x1C */ virtual bool draw() {
        return true;
    }
    /* vt 0x20 */ virtual ~dCameraBase_c() {}
    /* vt 0x24 */ virtual CamView &getView() {
        // TODO - const?
        return mView;
    }
    /* vt 0x28 */ virtual void vt_0x28() {
        field_0x02 = 1;
    }
    /* vt 0x2C */ virtual void vt_0x2C() {
        field_0x02 = 0;
    }
    /* vt 0x30 */ virtual void setView(const CamView &view) {
        // TODO - is this actually a base impl?
        mView = view;
    }
};

#endif
