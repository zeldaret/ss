#ifndef EGG_CAMERA_H
#define EGG_CAMERA_H

#include "egg/math/eggMatrix.h"
#include "egg/math/eggRotation.h"
#include "egg/math/eggVector.h"
#include "nw4r/g3d/g3d_camera.h"

namespace EGG {

class BaseCamera {
public:
    BaseCamera() {}
    virtual Matrix34f &getViewMatrix() = 0;
    virtual const Matrix34f &getViewMatrix() const = 0;
    virtual void updateMatrix();
    virtual void doUpdateMatrix() = 0;
    virtual void loadMatrix() = 0;
    virtual void loadOldMatrix() = 0;
    virtual EGG::Vector3f getPosition() = 0;
    virtual void draw(EGG::BaseCamera *cam);
    virtual void doDraw() = 0;
    virtual Matrix34f &getViewMatrixOld() = 0;

    void setG3DCamera(nw4r::g3d::Camera &);
    // Vector3f getRightVector();
    // Vector3f getLookVector();
    // Vector3f getUpVector();

protected:
    /* 0x04 */ Matrix34f mViewMtx;
    /* 0x34 */ Matrix34f mOtherMtx;
};

class LookAtCamera : public BaseCamera {
public:
    LookAtCamera() : mPos(0.0f, 10.0f, 0.0f), mAt(0.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f) {}
    // Which way around?
    virtual Matrix34f &getViewMatrix() override {
        return mViewMtx;
    }
    virtual const Matrix34f &getViewMatrix() const override {
        return mViewMtx;
    }

    virtual void doUpdateMatrix() override;
    virtual void loadMatrix() override;
    virtual void loadOldMatrix() override;
    virtual EGG::Vector3f getPosition() override {
        return mPos;
    }

    virtual void doDraw() override;
    virtual Matrix34f &getViewMatrixOld() override;

protected:
    /* 0x64 */ Vector3f mPos;
    /* 0x70 */ Vector3f mAt;
    /* 0x7C */ Vector3f mUp;
};

class OrthoCamera : public LookAtCamera {
public:
    OrthoCamera();

    virtual void doUpdateMatrix() override;

private:
    void update_parms();

    /* 0x88 */ EGG::Rotation<f32> field_0x88;
    /* 0x8C */ EGG::Rotation<f32> field_0x8C;
};

} // namespace EGG

#endif
