#include "egg/gfx/eggCamera.h"

#include "common.h"
#include "egg/math/eggMatrix.h"
#include "egg/math/eggVector.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTransform.h"

namespace EGG {

void BaseCamera::updateMatrix() {
    doUpdateMatrix();
}

void BaseCamera::draw(EGG::BaseCamera *cam) {
    cam->loadMatrix();
    doDraw();
}

void BaseCamera::setG3DCamera(nw4r::g3d::Camera &cam) {
    cam.SetCameraMtxDirectly(*reinterpret_cast<const nw4r::math::MTX34 *>(&getViewMatrix()));
}

Matrix34f &LookAtCamera::getViewMatrixOld() {
    return mOtherMtx;
}

void LookAtCamera::doUpdateMatrix() {
    mOtherMtx = mViewMtx;

    Vector3f right(mPos);

    right -= mAt;
    right.normalise();

    Vector3f forward(mUp.cross(right).normalize());

    Vector3f up(right.cross(forward));
    up.normalise();

    f32 tx = -forward.dot(mPos);
    f32 ty = -up.dot(mPos);
    f32 tz = -right.dot(mPos);

    f32 t[] = { tx, ty, tz};

    mViewMtx(0, 0) = forward(0);
    mViewMtx(0, 1) = forward(1);
    mViewMtx(0, 2) = forward(2);
    mViewMtx(0, 3) = tx;

    mViewMtx(1, 0) = up(0);
    mViewMtx(1, 1) = up(1);
    mViewMtx(1, 2) = up(2);
    mViewMtx(1, 3) = ty;

    mViewMtx(2, 0) = right(0);
    mViewMtx(2, 1) = right(1);
    mViewMtx(2, 2) = right(2);
    mViewMtx(2, 3) = tz;
}

void LookAtCamera::doDraw() {}

void LookAtCamera::loadMatrix() {
    Matrix34f mtx;
    GXLoadPosMtxImm(mViewMtx.m, 0);
    mViewMtx.inverseTransposeTo(mtx);
    GXLoadNrmMtxImm(mtx.m, 0);
}

void LookAtCamera::loadOldMatrix() {}

OrthoCamera::OrthoCamera() {
    field_0x8C = 0.5f;
    field_0x88 = 0.0f;
    update_parms();
}

void OrthoCamera::update_parms() {
    f32 z = field_0x8C;
    f32 sin = field_0x88.sin();
    f32 cos = field_0x88.cos();

    mPos(1) = mAt(1);
    mPos(0) = mAt(0);
    mPos(2) = z;
    mUp(2) = 0.f;
    mUp(1) = cos;
    mUp(0) = sin;
}

void OrthoCamera::doUpdateMatrix() {
    update_parms();
    LookAtCamera::doUpdateMatrix();
}

} // namespace EGG
