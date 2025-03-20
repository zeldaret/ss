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

void EGG::BaseCamera::draw(EGG::BaseCamera *cam) {
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
    // NONMATCHING
    mOtherMtx.copyFrom(mViewMtx);
    EGG::Vector3f posRight = mPosition;
    posRight -= mRight;
    posRight.normalise();

    EGG::Vector3f forward = posRight.cross(mUp);
    forward.normalise();

    EGG::Vector3f up = posRight.cross(forward);
    up.normalise();

    mViewMtx.setTranslation(Vector3f((forward.dot(mPosition)), (up.dot(mPosition)), (posRight.dot(mPosition))));
    mViewMtx.setBase(0, forward);
    mViewMtx.setBase(1, up);
    mViewMtx.setBase(2, posRight);
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
    // NONMATCHING
    f32 z = field_0x8C;
    f32 sin = Math<f32>::sin(field_0x88);
    f32 cos = Math<f32>::cos(field_0x88);
    mPosition.set(mRight.x, mRight.y, z);
    mUp.set(sin, cos, 0.0f);
}

void OrthoCamera::doUpdateMatrix() {
    update_parms();
    LookAtCamera::doUpdateMatrix();
}

} // namespace EGG
